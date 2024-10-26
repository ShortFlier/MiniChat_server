#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "datahead.h"
#include "dbconnect.h"
#include "websocketconnect.h"
#include <map>

class HttpController;

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);
    ~Controller();
    void handle(WebSocketConnect *wsc, DataHead& head, DataResult& result);
    void unknow(WebSocketConnect *wsc, DataHead& head, DataResult& result);

signals:

private:
    HttpController* hctrl;
};

class HttpController{
    typedef std::pair<QString, void(HttpController::*)(WebSocketConnect *, DataHead&, DataResult&)> Pair;
public:
    HttpController();
    ~HttpController();
    //登入处理 path=login
    void login(WebSocketConnect *wsc, DataHead& head, DataResult& result);
public:
    std::map<QString, void(HttpController::*)(WebSocketConnect *, DataHead&, DataResult&)> fmap;
    Mapper* mapper;
};

#endif // CONTROLLER_H
