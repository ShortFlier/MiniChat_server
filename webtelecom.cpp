#include "webtelecom.h"

#include "datahead.h"


//网络通信类，处理客户端的请求，与客户端通信
WebTelecom::WebTelecom(QObject *parent)
    : QObject{parent}
{

}

void WebTelecom::textMsgHandler(WebSocketConnect *wsc, QString msg)
{
    QTextStream ts(&msg);
    QString hdstr=ts.readLine();
    DataHead head(hdstr);
    head.show();
}
