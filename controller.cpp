#include "controller.h"

//中间处理层
Controller::Controller(QObject *parent)
    : QObject{parent}
{
    hctrl=new HttpController();
}

Controller::~Controller()
{
    delete hctrl;
}

void Controller::handle(WebSocketConnect *wsc, DataHead &head, DataResult &result)
{
    if(*head._tp_type==DataHead::http){ //处理http通信
        auto fun = hctrl->fmap.at(*head._path);
        (hctrl->*fun)(wsc, head, result);
    }else if(*head._tp_type==DataHead::websocket){  //处理消息推送

    }else{
        unknow(wsc, head, result);
    }
}

void Controller::unknow(WebSocketConnect *wsc, DataHead &head, DataResult &result)
{
    head.show();
    head.invert();
    wsc->sendText(head, DataResult::error("未知通信"));
}

HttpController::HttpController()
{
    mapper=new Mapper(10);
    fmap.insert(Pair("login", &HttpController::login));
}

HttpController::~HttpController()
{
    delete mapper;
}

void HttpController::login(WebSocketConnect *wsc, DataHead &head, DataResult &result)
{
    qDebug()<<"登入请求处理";
}
