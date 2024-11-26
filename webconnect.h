#ifndef WEBCONNECT_H
#define WEBCONNECT_H

#include "validconnect.h"
#include "websocketconnect.h"


//临时连接，未进行登入验证
class TempConnect : public WebSocketConnect
{
    Q_OBJECT
public:
    explicit TempConnect(QWebSocket* sock);
    ~TempConnect();
    ValidConnect* upgrade(QString account);

signals:
    void upgraded(ValidConnect*, TempConnect* oldtc);
    void down(const QString& act, TempConnect* oldtc);
};

#endif // WEBCONNECT_H
