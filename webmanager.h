#ifndef WEBMANAGER_H
#define WEBMANAGER_H

#include <QObject>

#include <QWebSocketServer>
#include <QWebSocket>

#include "webconnectpool.h"

class WebManager : public QObject
{
    Q_OBJECT
public:
    explicit WebManager(QObject *parent = nullptr);
    ~WebManager();
    QUrl loadcfg();
    //启动服务器
    void start();
    bool online(const QString& act);

private:
    QWebSocketServer* server=nullptr;
    WebConnectPool* wpool=nullptr;

signals:
};

#endif // WEBMANAGER_H
