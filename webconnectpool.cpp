#include "webconnectpool.h"
#include <QMutexLocker>

WebConnectPool WebConnectPool::webpool=WebConnectPool();

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
    for(auto v : vcmap){
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
    connect(temp, &TempConnect::upgraded, this, &WebConnectPool::upgraded);
}

void WebConnectPool::removeTempConnect(TempConnect *temp)
{
    QMutexLocker<QMutex> locker(&mutex);
    //移除temp
    tempcs.removeOne(temp);
    delete temp;
}

void WebConnectPool::upgraded(ValidConnect *vc, TempConnect* oldtc)
{
    removeTempConnect(oldtc);
    vcmap.insert(vc->getAccount(), vc);
    qDebug()<<vc->getAccount()<<" 上线了!";
    vc->getSocket()->disconnect();
    //可读取文本数据
    connect(vc->getSocket(), &QWebSocket::textMessageReceived, [=](const QString& msg){
        webTelecom->textMsgHandler(vc, msg);
    });
    //可读取二进制数据
    connect(vc->getSocket(), &QWebSocket::binaryMessageReceived, [=](const QByteArray& data){
        webTelecom->binaryHandler(vc, data);
    });
    //vc断开连接时，从map中移除
    connect(vc->getSocket(), &QWebSocket::disconnected, [=](){
        vcmap.remove(vc->getAccount());
        qDebug()<<vc->getAccount()<<" 下线了!";
    });
    //消息推送
    WebSocketController::loginedmsg(vc);
}

bool WebConnectPool::online(const QString &act)
{
    return vcmap.contains(act);
}

ValidConnect *WebConnectPool::getValidConnect(const QString &account)
{
    ValidConnect* vc=nullptr;
    if(vcmap.contains(account))
        vc=vcmap.value(account);
    return vc;
}

WebConnectPool& WebConnectPool::instanse()
{
    return webpool;
}
