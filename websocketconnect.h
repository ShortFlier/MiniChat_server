#ifndef WEBSOCKETCONNECT_H
#define WEBSOCKETCONNECT_H

#include <QObject>
#include <QWebSocket>
#include "datahead.h"

class WebSocketConnect : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketConnect(QWebSocket* sock);
    //移动构造
    explicit WebSocketConnect(WebSocketConnect&& wsc);
    virtual ~WebSocketConnect()=0;
    QWebSocket* getSocket(){return socket;}
    void sendText(const DataHead& head, const DataResult& result);
    void sendBinary(const DataHead& head, int code, QByteArray& data);
    //二进制数据发送
    void sendBinary(DataHead& head, QJsonDocument& jd, QByteArray& data);


signals:
    void closed();

protected:
    QWebSocket* socket;

};

#endif // WEBSOCKETCONNECT_H
