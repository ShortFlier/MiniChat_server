#include "websocketcontroller.h"

#include "validconnect.h"
#include "webconnectpool.h"

#include <QJsonArray>


WebSocketController::WebSocketController(QObject *parent)
    : QObject{parent}
{
    mapper=Mapper::getInstance(10);

    fmap.insert("send", &WebSocketController::send);
    fmap.insert("loginedmsg", &WebSocketController::loginedmsgdle);
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

void WebSocketController::loginedmsg(ValidConnect *vc)
{
    qDebug()<<"loginedmsg";
    std::vector<Information> data=Mapper::getInstance()->loginedmsg(vc->getAccount());
    if(data.size()){
        QJsonArray ja;
        for(int i=0; i<data.size(); i++){
            ja.append(data[i].json());
        }
        DataHead head=DataHead::wsHead("loginedmsg");
        DataResult result(0, QJsonDocument(ja));
        vc->sendText(head, result);
    }
}

void WebSocketController::loginedmsgdle(WebSocketConnect *wsc, DataHead &head, DataResult &result)
{
    QJsonArray ids=result.jsdata.array();
    mapper->loginedmsgdle(ids);
}
