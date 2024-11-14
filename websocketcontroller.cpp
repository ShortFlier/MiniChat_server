#include "websocketcontroller.h"


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
    mapper->newmsg(info);
}
