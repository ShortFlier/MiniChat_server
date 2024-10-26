#include "webtelecom.h"
#include "datahead.h"
#include <QJsonDocument>
#include <qjsonobject.h>


//网络通信类，处理客户端的请求，与客户端通信
WebTelecom::WebTelecom(QObject *parent)
    : QObject{parent}
{
    ctrl=new Controller();
}

WebTelecom::~WebTelecom()
{
    delete ctrl;
}

void WebTelecom::textMsgHandler(WebSocketConnect *wsc, QString msg)
{
    QTextStream ts(&msg);
    QString hdstr=ts.readLine();
    DataHead head(hdstr);
    DataResult result(ts.readAll());
    ctrl->handle(wsc, head, result);
}
