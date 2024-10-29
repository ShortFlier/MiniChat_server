#ifndef APPLICATION_H
#define APPLICATION_H

#include "webmanager.h"

#include <QObject>

class Application : public QObject
{
    Q_OBJECT
public:
    explicit Application(QObject *parent = nullptr);
    void start();

signals:

private:
    //网络调度
    WebManager wm;

};



#endif // APPLICATION_H
