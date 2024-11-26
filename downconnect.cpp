#include "downconnect.h"

#include "websocketconnect.h"
#include "datahead.h"
#include "webtelecom.h"

#include <QFile>

DownConnect::DownConnect(const QString& account, WebSocketConnect* sock, QObject *parent)
    : QObject{parent}
{
    this->account=account;
    fc=new FileController();
    socket=sock;
    sock->disconnect();
    sock->getSocket()->disconnect();

    connect(socket->getSocket(), &QWebSocket::binaryMessageReceived, this, &DownConnect::BinayHandler);
}

DownConnect::~DownConnect()
{
    delete socket;
    delete fc;
}



void DownConnect::sendBinay(QString &path, QJsonDocument jd, QByteArray &data)
{
    QString head=path+DataHead::sepe+jd.toJson();
    QByteArray h=head.toUtf8();
    if(h.length()>HLENGTH){
        qDebug()<<"error 数据同步大于"+QString::number(HLENGTH)+"字节";
        qDebug()<<path;
        qDebug()<<path.length();
        return;
    }
    h.resize(HLENGTH);
    socket->getSocket()->sendBinaryMessage(h+data);
}

void DownConnect::sendimg(const QString &name)
{
    QString path="sendimg";
    QJsonObject jo;
    jo.insert("name", name);
    QJsonDocument jd(jo);
    QByteArray* d=fc->getChatImg(name);
    if(d){
        QByteArray data(std::move(*d));
        sendBinay(path, jd, data);
        //删除
        fc->delChatImg(name);
    }
    delete d;
}

void DownConnect::sendimg(Information &info, QByteArray &data)
{
    QString path="reciverimg";
    QJsonObject jo;
    jo.insert("sender", info.sender);
    jo.insert("time", info.time.toString("yyyy-MM-dd HH:mm:ss"));
    QJsonDocument jd(jo);
    sendBinay(path, jd, data);
}

void DownConnect::BinayHandler(const QByteArray &data)
{
    //分离
    QByteArray h= data.mid(0, HLENGTH);
    //请求路径部分
    int i = h.indexOf(DataHead::sepe.toUtf8());
    QString path=h.left(i);
    qDebug()<<path;
    //json数据部分
    QByteArray js = h.mid(i+2);
    //提取有效json部分
    i=js.lastIndexOf('\n');
    js=js.left(i);
    qDebug()<<js;
    QJsonDocument json = QJsonDocument::fromJson(js);
    //数据部分
    QByteArray d=data.mid(HLENGTH);
}
