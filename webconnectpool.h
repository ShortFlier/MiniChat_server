#ifndef WEBCONNECTPOOL_H
#define WEBCONNECTPOOL_H

#include <QObject>
#include <QList>
#include "webconnect.h"
#include "webtelecom.h"
#include <QMutex>
#include <QMap>

class WebConnectPool : public QObject
{
    Q_OBJECT
public:
    explicit WebConnectPool(QObject *parent = nullptr);
    ~WebConnectPool();

public slots:
    void addTempConnect(TempConnect* temp);
    void removeTempConnect(TempConnect* temp);
    void upgraded(ValidConnect* vc, TempConnect* oldtc);

    bool online(const QString& act);

    ValidConnect* getValidConnect(const QString& account);

    static WebConnectPool& instanse();

signals:

private:
    QMutex mutex;
    //临时连接池
    QList<TempConnect*> tempcs;
    WebTelecom* webTelecom=nullptr;
    //正规连接池
    QMap<QString,ValidConnect*> vcmap;

    static WebConnectPool webpool;
};

#endif // WEBCONNECTPOOL_H
