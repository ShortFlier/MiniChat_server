#ifndef WEBTELECOM_H
#define WEBTELECOM_H

#include <QObject>

#include "websocketconnect.h"
#include "controller.h"

#define HLENGTH 100

class WebTelecom : public QObject
{
    Q_OBJECT
public:
    explicit WebTelecom(QObject *parent = nullptr);
    ~WebTelecom();

public slots:
    //文本数据处理
    void textMsgHandler(WebSocketConnect* wsc, QString msg);
    //二进制数据处理
    void binaryHandler(WebSocketConnect* wsc, QByteArray data);

signals:

private:
    Controller* ctrl=nullptr;
};

#endif // WEBTELECOM_H
