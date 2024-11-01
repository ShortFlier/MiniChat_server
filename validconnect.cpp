#include "validconnect.h"

ValidConnect::ValidConnect(WebSocketConnect& wsc, const QString& act):WebSocketConnect(std::move(wsc)) {
    account=act;
}
