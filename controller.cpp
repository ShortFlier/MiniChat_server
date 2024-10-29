#include "controller.h"
#include "entity.h"

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
    fmap.insert(Pair("regist_code", &HttpController::regist_code));
    fmap.insert(Pair("regist_confirm", &HttpController::regist_confirm));
}

HttpController::~HttpController()
{
    delete mapper;
}

void HttpController::login(WebSocketConnect *wsc, DataHead &head, DataResult &result)
{
    qDebug()<<"登入请求处理";
    bool v = mapper->login_mapper(head, result);
    head.invert();
    if(v){
        QJsonObject jo;
        jo.insert("account", result.jsdata.object().value("account").toString());
        wsc->sendText(head, DataResult(200, QJsonDocument(jo)));
    }else{
        DataResult rs = DataResult::error("密码错误或账号不存在");
        wsc->sendText(head, rs);
    }
}

void HttpController::regist_code(WebSocketConnect *wsc, DataHead &head, DataResult &result)
{
    //获取邮箱
    QString email=result.jsdata.object().value("email").toString();
    qDebug()<<email;
    head.invert();
    //邮箱格式验证
    if(!Captcha::emailFormat(email)){
        wsc->sendText(head, DataResult::error("邮箱格式错误"));
        return;
    }
    //查询是否已经注册
    bool s=mapper->hasEmail_mapper(email);
    if(s){//已经注册
        qDebug()<<"邮箱已被注册";
        wsc->sendText(head, DataResult::error("邮箱已被注册"));
    }else{//未注册
        //生成验证码，如果有查询发送
        QString code = enroll_captcha.getValue(email);
        qDebug()<<code;
        if(code.isNull()){//生成验证码
            enroll_captcha.add(email, Captcha::generateCaptcha(6));
            code=enroll_captcha.getValue(email);
        }
        wsc->sendText(head, DataResult::success("验证码已发出"));
        //发送邮箱验证码
        QString content=EmailSender::regist_code;
        content = content.arg(email).arg(code);
        EmailSender::email e(email, content);
        smtp.add(e);
    }
}

void HttpController::regist_confirm(WebSocketConnect *wsc, DataHead &head, DataResult &result)
{
    //邮箱、验证码
    QString email=result.jsdata.object().value("email").toString();
    QString code=result.jsdata.object().value("code").toString();
    //取出临时验证码
    QString cd=enroll_captcha.getValue(email);
    head.invert();
    if(cd.isNull()){
        wsc->sendText(head, DataResult::error("验证码错误"));
    }else if(cd==code){
        //生成新的账号和密码
        QString account=generateRandomNumberString();
        QString password=Captcha::generateCaptcha(8);
        //加入数据库
        User user(email, account, password, account);
        bool b=mapper->newUser(user);
        if(b){
            wsc->sendText(head, DataResult::success("注册成功"));
            //将账号和密码发送邮箱
            QString content=EmailSender::regist_success;
            content=content.arg(email).arg(account).arg(password);
            EmailSender::email e(email,content);
            smtp.add(e);
        }else
            wsc->sendText(head, DataResult::error("注册失败，请重试"));
    }else
        wsc->sendText(head, DataResult::error("验证码错误"));
}
