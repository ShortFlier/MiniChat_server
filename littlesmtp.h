#ifndef LITTLESMTP_H
#define LITTLESMTP_H

#include <QObject>

#include <QString>
#include <QTcpSocket>
#include <QQueue>
#include <QThread>

//单个邮件发送类，采取短连接方式发送一份邮件
class LittleSMTP : public QObject
{
    Q_OBJECT
public:
    explicit LittleSMTP(QObject *parent = nullptr);
    ~LittleSMTP();
    void send(const QString& target, const QString& title, const QString& content);
    static void load();

signals:
    void finished();

private slots:
    void msgHandler();


private:
    static QString address;
    static QString port;
    static QString account;
    static QString password;

    QTcpSocket* smtp=nullptr;

    QQueue<QString>* msgQueue=nullptr;

    void build();
    void data(const QString& target, const QString& title, const QString& content);
    void send();

    void close();
};


//批量邮件发送类，一封一封的按顺序进行邮件发送
class EmailSender : public QObject{
    Q_OBJECT

public:
    struct email{
        QString target;
        QString title;
        QString content;
        email(const QString& tar, const QString& ctent, const QString& tle="Minichat Server"):target(tar), title(tle), content(ctent){}
        email(){};
    };

    static const QString regist_code;
    static const QString regist_success;

    EmailSender(QObject *parent = nullptr);
    ~EmailSender();
    void add(const email& em);

private:
    QQueue<email>* emailQue=nullptr;

    void send();

};

#endif // LITTLESMTP_H
