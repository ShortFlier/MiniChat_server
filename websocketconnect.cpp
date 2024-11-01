#include "websocketconnect.h"

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



WebSocketConnect::~WebSocketConnect(){
    if(socket){
        socket->disconnect();
        socket->close();
        socket->deleteLater();
    }
}
