#ifndef VALIDCONNECT_H
#define VALIDCONNECT_H

#include "websocketconnect.h"

#include <QObject>

class ValidConnect:public WebSocketConnect
{
    Q_OBJECT
public:
    ValidConnect(WebSocketConnect& wsc, const QString& act);
    QString getAccount() const {return account;}

private:
    QString account;
};

#endif // VALIDCONNECT_H
