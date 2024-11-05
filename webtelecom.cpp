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

void WebTelecom::binaryHandler(WebSocketConnect *wsc, QByteArray data)
{
    //分离
    QByteArray h= data.mid(0, HLENGTH);
    //请求路径部分
    int i = h.indexOf(DataHead::sepe.toUtf8());
    QString hstr=h.left(i);
    DataHead head(hstr);
    //json数据部分
    QByteArray js = h.mid(i+2);
    //提取有效json部分
    i=js.lastIndexOf('\n');
    js=js.left(i);
    QJsonDocument json = QJsonDocument::fromJson(js);
    //数据部分
    QByteArray d=data.mid(HLENGTH);
    ctrl->binHandler(wsc, head, json, d);
}
