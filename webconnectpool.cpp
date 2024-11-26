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
    for(auto v : dcmap){
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
    connect(temp, &TempConnect::down, this, &WebConnectPool::down);
}

void WebConnectPool::removeTempConnect(TempConnect *temp)
{
    temp->disconnect();
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
    //登入后消息推送
    //WebSocketController::loginedmsg(vc);
}

void WebConnectPool::down(const QString &act, TempConnect *oldtc)
{
    DownConnect* dc=new DownConnect(act, oldtc);
    addDownConnect(act,dc);
    qDebug()<<act<<" down连接";
    //登入后消息推送
    WebSocketController::loginedmsg(getValidConnect(act));

    oldtc->disconnect();
    QMutexLocker<QMutex> locker(&mutex);
    //移除temp
    tempcs.removeOne(oldtc);
}

void WebConnectPool::addDownConnect(const QString &act, DownConnect *dc)
{
    dcmap.insert(act,dc);
    //连接断开时，删除
    connect(dc->getSocket(), &QWebSocket::disconnected, [=](){
        dcmap.remove(act);
        qDebug()<<act<<" 断开down";
    });
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

DownConnect *WebConnectPool::getDownConnect(const QString &account)
{
    DownConnect *dc=nullptr;
    if(dcmap.contains(account))
        dc=dcmap.value(account);
    return dc;
}

WebConnectPool& WebConnectPool::instanse()
{
    return webpool;
}
