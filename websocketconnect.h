#ifndef WEBSOCKETCONNECT_H
#define WEBSOCKETCONNECT_H

#include <QObject>
#include <QWebSocket>

class WebSocketConnect : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketConnect(QWebSocket* sock);
    //移动构造
    explicit WebSocketConnect(WebSocketConnect&& wsc);
    virtual ~WebSocketConnect()=0;
    QWebSocket* getSocket(){return socket;}

signals:
    void closed();

protected:
    QWebSocket* socket;

};

#endif // WEBSOCKETCONNECT_H
