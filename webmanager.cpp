#include "webmanager.h"
#include <QFile>
#include <QDebug>

WebManager::WebManager(QObject *parent)
    : QObject{parent}
{
    wpool=new WebConnectPool;
}

WebManager::~WebManager()
{
    delete wpool;
    if(server){
        server->disconnect();
        server->close();
        server->deleteLater();
    }
}

QUrl WebManager::loadcfg()
{
    QFile file(":/assets/server.config.txt");
    file.open(QIODeviceBase::ReadOnly);
    if(file.isOpen()){
        QTextStream ts(&file);
        QString ip=ts.readLine();
        QString port=ts.readLine();
        return QUrl(ip+":"+port);
    }else{
        qDebug()<<"server.config文件读取出错";
        return QUrl("ws://127.0.0.1:10023");
    }
}

void WebManager::start()
{
    server=new QWebSocketServer("MiniChat_Server", QWebSocketServer::NonSecureMode);
    QUrl url=loadcfg();
    qDebug()<<"服务器启动："<<url;
    server->listen(QHostAddress(url.host()), url.port());
    connect(server, &QWebSocketServer::newConnection, [this](){
        QWebSocket* sock=server->nextPendingConnection();
        wpool->addTempConnect(new TempConnect(sock));
    });
}
