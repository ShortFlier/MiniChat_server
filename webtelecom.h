#ifndef WEBTELECOM_H
#define WEBTELECOM_H

#include <QObject>

#include "websocketconnect.h"

class WebTelecom : public QObject
{
    Q_OBJECT
public:
    explicit WebTelecom(QObject *parent = nullptr);

public slots:
    void textMsgHandler(WebSocketConnect* wsc, QString msg);

signals:
};

#endif // WEBTELECOM_H
