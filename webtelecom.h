#ifndef WEBTELECOM_H
#define WEBTELECOM_H

#include <QObject>

#include "websocketconnect.h"
#include "controller.h"

class WebTelecom : public QObject
{
    Q_OBJECT
public:
    explicit WebTelecom(QObject *parent = nullptr);
    ~WebTelecom();

public slots:
    void textMsgHandler(WebSocketConnect* wsc, QString msg);

signals:

private:
    Controller* ctrl=nullptr;
};

#endif // WEBTELECOM_H
