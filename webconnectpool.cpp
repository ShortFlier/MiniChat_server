#include "webconnectpool.h"
#include <QMutexLocker>

WebConnectPool::WebConnectPool(QObject *parent)
    : QObject{parent}
{
    webTelecom=new WebTelecom();
}

WebConnectPool::~WebConnectPool()
{
    QMutexLocker<QMutex> locker(&mutex);
    delete webTelecom;
    for(auto v : tempcs){
        delete v;
    }
}

void WebConnectPool::addTempConnect(TempConnect *temp)
{
    QMutexLocker<QMutex> locker(&mutex);
    tempcs.append(temp);
    //temp断开连接时，从列表中移除
    connect(temp->getSocket(), &QWebSocket::disconnected, [=](){
        removeTempConnect(temp);
    });
    //可读取文本数据
    connect(temp->getSocket(), &QWebSocket::textMessageReceived, [=](const QString& msg){
        webTelecom->textMsgHandler(temp, msg);
    });
}

void WebConnectPool::removeTempConnect(TempConnect *temp)
{
    QMutexLocker<QMutex> locker(&mutex);
    //移除temp
    tempcs.removeOne(temp);
    delete temp;
}
