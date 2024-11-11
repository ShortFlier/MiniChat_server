#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "datahead.h"
#include "websocketconnect.h"
#include <map>
#include "bincontroller.h"
#include "captcha.h"
#include "littlesmtp.h"
#include "mapper.h"

#define WEB_PARAMETER WebSocketConnect *wsc, DataHead& head, DataResult& result

class HttpController;

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);
    ~Controller();
    void handle(WebSocketConnect *wsc, DataHead& head, DataResult& result);
    void unknow(WebSocketConnect *wsc, DataHead& head, DataResult& result);
    //二进制处理
    void binHandler(WebSocketConnect *wsc, DataHead& head, QJsonDocument& json, QByteArray& data);

signals:

private:
    HttpController* hctrl;
    BinController* bctrl;
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
    //获取用户信息
    void userinfo(WebSocketConnect *wsc, DataHead& head, DataResult& result);
    //修改昵称
    void rename(WEB_PARAMETER);
    //用户、群搜索
    void search(WEB_PARAMETER);
    //邀请
    void invite(WEB_PARAMETER);
    //获取邀请
    void myinvite(WEB_PARAMETER);

public:
    std::map<QString, void(HttpController::*)(WebSocketConnect *, DataHead&, DataResult&)> fmap;
    Mapper* mapper;
    //注册验证码
    Captcha enroll_captcha;
    //邮件发送
    EmailSender smtp;
};

#endif // CONTROLLER_H
