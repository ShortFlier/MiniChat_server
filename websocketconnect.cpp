#include "websocketconnect.h"

#include <QFile>

WebSocketConnect::WebSocketConnect(QWebSocket* sock)
{
    socket=sock;
}

void WebSocketConnect::sendText(const DataHead &head,const DataResult &result)
{
    socket->sendTextMessage(head.getUrl()+result.data());
}



WebSocketConnect::~WebSocketConnect(){
    socket->disconnect();
    socket->close();
    socket->deleteLater();
}
