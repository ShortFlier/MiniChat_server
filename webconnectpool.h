#ifndef WEBCONNECTPOOL_H
#define WEBCONNECTPOOL_H

#include <QObject>
#include <QList>
#include "webconnect.h"
#include "webtelecom.h"
#include <QMutex>

class WebConnectPool : public QObject
{
    Q_OBJECT
public:
    explicit WebConnectPool(QObject *parent = nullptr);
    ~WebConnectPool();

public slots:
    void addTempConnect(TempConnect* temp);
    void removeTempConnect(TempConnect* temp);

signals:

private:
    QMutex mutex;
    QList<TempConnect*> tempcs;
    WebTelecom* webTelecom=nullptr;
};

#endif // WEBCONNECTPOOL_H
