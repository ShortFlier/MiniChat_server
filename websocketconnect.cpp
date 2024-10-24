#include "websocketconnect.h"

#include <QFile>

WebSocketConnect::WebSocketConnect(QWebSocket* sock)
{
    socket=sock;
}



WebSocketConnect::~WebSocketConnect(){
    socket->disconnect();
    socket->close();
    socket->deleteLater();
}
