#ifndef DOWNCONNECT_H
#define DOWNCONNECT_H

#include "entity.h"
#include "filecontroller.h"
#include "websocketconnect.h"

#include <QObject>
#include <QWebSocket>

//专门用于文件传输的连接
class DownConnect :public QObject
{
    Q_OBJECT
public:
    explicit DownConnect(const QString& account, WebSocketConnect* sock, QObject *parent = nullptr);
    ~DownConnect();
    void sendBinay(QString& path, QJsonDocument jd, QByteArray& data);
    QWebSocket* getSocket(){return socket->getSocket();}

    //图片发送 用于从数据库取出的图片聊天记录发送
    void sendimg(const QString& name);
    //图片发送 用于及时消息转发
    void sendimg(Information& info, QByteArray& data);
signals:

private:
    QString account;
    WebSocketConnect* socket=nullptr;
    FileController* fc=nullptr;

    void BinayHandler(const QByteArray& data);
};

#endif // DOWNCONNECT_H
