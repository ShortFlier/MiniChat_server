#include "littlesmtp.h"

#include <QFile>
#include <qdebug.h>
#include <QThread>

LittleSMTP::LittleSMTP(QObject *parent)
    : QObject{parent}
{

}

LittleSMTP::~LittleSMTP()
{
    delete msgQueue;
    delete smtp;
}

void LittleSMTP::send(const QString& target, const QString& title, const QString& content)
{
    data(target, title, content);
    build();
}

QString LittleSMTP::address;
QString LittleSMTP::port;
QString LittleSMTP::account;
QString LittleSMTP::password;

void LittleSMTP::load()
{
    //解析配置文件
    QFile file(":/assets/email.config.txt");
    file.open(QIODeviceBase::ReadOnly);
    if(file.isOpen()){
        QTextStream ts(&file);
        //内容解析
        address=ts.readLine();
        port=ts.readLine();
        account=ts.readLine();
        password=ts.readLine();
        qDebug()<<"address: "<<address;
        qDebug()<<"account: "<<account;
        qDebug()<<"password: "<<password;

    }else
        qDebug()<<"email.config.txt 无法打开";
    file.close();
}

//消息处理
void LittleSMTP::msgHandler()
{
    QTextStream ts(smtp);
    int code;
    ts>>code;
    QString msg=ts.readAll();
    if(code>=400){
        qDebug()<<"错误------------"<<code;
        qDebug()<<msg;
        close();
    }else{
        send();
    }
}

//与服务器建立连接
void LittleSMTP::build()
{
    smtp=new QTcpSocket();
    smtp->connectToHost(address, port.toInt());
    connect(smtp, &QTcpSocket::connected, [this](){
        connect(smtp, &QTcpSocket::readyRead, this, &LittleSMTP::msgHandler);
    });
    connect(smtp, &QTcpSocket::disconnected, this, &LittleSMTP::close);
}

void LittleSMTP::data(const QString& target, const QString& title, const QString& content)
{
    msgQueue=new QQueue<QString>;
    msgQueue->enqueue("HELO minichat \r\n");
    //身份认证
    msgQueue->enqueue("AUTH LOGIN\r\n");
    msgQueue->enqueue(account.toUtf8().toBase64()+"\r\n");
    msgQueue->enqueue(password.toUtf8().toBase64()+"\r\n");
    //发送邮件信封
    //发件人
    QString sender("MAIL FROM:<%1>\r\n");
    sender = sender.arg(account);
    msgQueue->enqueue(sender);
    //收件人
    QString reciver("RCPT TO:<%1>\r\n");
    reciver = reciver.arg(target);
    msgQueue->enqueue(reciver);
    //发送邮件内容
    msgQueue->enqueue("DATA\r\n");
    QString msg("From:MiniChat<%1>\r\nTo:%2\r\nSubject:%3\r\n\r\n%4\r\n.\r\n");
    msg = msg.arg(account).arg(target).arg(title).arg(content);
    msgQueue->enqueue(msg);
//    msgQueue->enqueue("NOOP");
}

void LittleSMTP::send()
{
    if(msgQueue){
        if(!msgQueue->isEmpty()){
            QString msg = msgQueue->dequeue();
            smtp->write(msg.toUtf8());
        }else
            close();
    }
}


//关闭连接
void LittleSMTP::close()
{
    smtp->disconnect();
    smtp->write("QUIT\r\n");
    smtp->close();
    qDebug()<<"smtp连接关闭";
    emit finished();
    deleteLater();
}



EmailSender::EmailSender(QObject *parent)
    : QObject{parent}
{
    LittleSMTP::load();
    emailQue=new QQueue<email>;
}

EmailSender::~EmailSender()
{
    delete emailQue;
}

void EmailSender::add(const email &em)
{
    emailQue->enqueue(em);
    send();
}

void EmailSender::send()
{
    if(!emailQue->isEmpty()){
        const email& em=emailQue->dequeue();
        LittleSMTP* smtp=new LittleSMTP();
        smtp->send(em.target, em.title, em.content);
    }
}
