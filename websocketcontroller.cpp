#include "websocketcontroller.h"

#include "validconnect.h"
#include "webconnectpool.h"

#include <QJsonArray>


WebSocketController::WebSocketController(QObject *parent)
    : QObject{parent}
{
    mapper=Mapper::getInstance(10);

    fmap.insert("send", &WebSocketController::send);
}

void WebSocketController::handler(WebSocketConnect *wsc, DataHead &head, DataResult &result)
{
    qDebug()<<head.getUrl();
    QString path=*head._path;
    FUN fun=fmap.value(path);
    (this->*fun)(wsc, head, result);
}

void WebSocketController::send(WebSocketConnect *wsc, DataHead &head, DataResult &result)
{
    //取出数据
    Information info=Information::enjson(result.jsdata.object());
    info.time=QDateTime::currentDateTime();
    //转发消息
    result.code=DataResult::code_success;
    QJsonArray ja;
    ja.append(result.jsdata.object());
    result.jsdata=QJsonDocument(ja);
    ValidConnect* vc=WebConnectPool::instanse().getValidConnect(info.reciver);
    if(vc){
        qDebug()<<info.sender<<" send to "<<info.reciver;
        vc->sendText(head,result);
    }else//对方不在线，保存数据库
        mapper->newmsg(info);
}
