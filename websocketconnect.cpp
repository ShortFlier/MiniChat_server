#include "websocketconnect.h"
#include "webtelecom.h"

#include <QFile>

WebSocketConnect::WebSocketConnect(QWebSocket* sock)
{
    socket=sock;
}

WebSocketConnect::WebSocketConnect(WebSocketConnect &&wsc)
{
    socket=wsc.socket;
    wsc.socket=nullptr;
}

void WebSocketConnect::sendText(const DataHead &head,const DataResult &result)
{
    socket->sendTextMessage(head.getUrl()+result.data());
}

void WebSocketConnect::sendBinary(const DataHead &head, int code, QByteArray &data)
{
    QString h=head.getUrl();
    QString c=QString::number(code)+DataHead::sepe;
    QByteArray hd=(h+c).toUtf8();
    int i=hd.length();
    if(i>HLENGTH)
        qDebug()<<"HLENGTH over!!!";
    hd.resize(HLENGTH);
    socket->sendBinaryMessage(hd+data);
}

void WebSocketConnect::sendBinary(DataHead &head, QJsonDocument &jd, QByteArray &data)
{
    QString path=head.getUrl()+jd.toJson();
    QByteArray h=path.toUtf8();
    if(h.length()>HLENGTH){
        qDebug()<<"error: 数据同步大于"+QString::number(HLENGTH)+"字节";
        qDebug()<<path;
        qDebug()<<path.length();
        return;
    }
    h.resize(HLENGTH);
    socket->sendBinaryMessage(h+data);
}


WebSocketConnect::~WebSocketConnect(){
    if(socket){
        socket->disconnect();
        socket->close();
        socket->deleteLater();
    }
}
