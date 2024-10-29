#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "datahead.h"
#include "dbconnect.h"
#include "websocketconnect.h"
#include <map>
#include "captcha.h"
#include "littlesmtp.h"

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
    //注册请求处理
    //邮箱注册验证码发送path=regist_code
    void regist_code(WebSocketConnect *wsc, DataHead& head, DataResult& result);
    //邮箱注册验证码接收确认path=regist_confirm
    void regist_confirm(WebSocketConnect *wsc, DataHead& head, DataResult& result);
public:
    std::map<QString, void(HttpController::*)(WebSocketConnect *, DataHead&, DataResult&)> fmap;
    Mapper* mapper;
    //注册验证码
    Captcha enroll_captcha;
    //邮件发送
    EmailSender smtp;
};

#endif // CONTROLLER_H
