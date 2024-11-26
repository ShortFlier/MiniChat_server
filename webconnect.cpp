#include "webconnect.h"

TempConnect::TempConnect(QWebSocket* sock):WebSocketConnect(sock)
{
}


TempConnect::~TempConnect()
{

}

ValidConnect* TempConnect::upgrade(QString account)
{
    ValidConnect* vc=new ValidConnect(*this, account);
    qDebug()<<"vc";
    emit upgraded(vc, this);
    return vc;
}

