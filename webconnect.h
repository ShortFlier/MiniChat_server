#ifndef WEBCONNECT_H
#define WEBCONNECT_H

#include "websocketconnect.h"


//临时连接，未进行登入验证
class TempConnect : public WebSocketConnect
{
public:
    explicit TempConnect(QWebSocket* sock);
    ~TempConnect();
};

#endif // WEBCONNECT_H
