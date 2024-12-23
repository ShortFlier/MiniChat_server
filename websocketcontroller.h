#ifndef WEBSOCKETCONTROLLER_H
#define WEBSOCKETCONTROLLER_H

#include <QObject>
#include <QMap>

#include "datahead.h"
#include "websocketconnect.h"
#include "mapper.h"
#include "validconnect.h"

#define WEB_PARAMETER WebSocketConnect *wsc, DataHead& head, DataResult& result

//websocket消息处理类
class WebSocketController : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketController(QObject *parent = nullptr);
    //分发处理
    void handler(WEB_PARAMETER);
    //用户间消息通信
    void send(WEB_PARAMETER);
    //登入后消息推送
    static void loginedmsg(ValidConnect* vc);
    //登入后消息推送后删除数据库消息
    void loginedmsgdle(WEB_PARAMETER);



    typedef void(WebSocketController::*FUN)(WebSocketConnect*, DataHead&, DataResult&);

signals:

private:
    QMap<QString, FUN> fmap;
    Mapper* mapper;
};

#endif // WEBSOCKETCONTROLLER_H
