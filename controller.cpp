#include "controller.h"
#include "application.h"
#include "entity.h"
#include "webconnect.h"

#include <QJsonArray>

#define INSERT(path, ptr) fmap.insert(Pair(path, ptr));
#define HEADSHOW     head.showHTTP();   head.invert();

//中间处理层
Controller::Controller(QObject *parent)
    : QObject{parent}
{
    hctrl=new HttpController();
    bctrl=new BinController();
    sctrl=new WebSocketController();
}

Controller::~Controller()
{
    delete hctrl;
    delete bctrl;
    delete sctrl;
}

void Controller::handle(WebSocketConnect *wsc, DataHead &head, DataResult &result)
{
    if(*head._tp_type==DataHead::http){ //处理http通信
        auto fun = hctrl->fmap.at(*head._path);
        (hctrl->*fun)(wsc, head, result);
    }else if(*head._tp_type==DataHead::websocket){  //处理消息推送
        sctrl->handler(wsc,head,result);
    }else{
        unknow(wsc, head, result);
    }
}

void Controller::unknow(WebSocketConnect *wsc, DataHead &head, DataResult &result)
{
    head.showHTTP();
    head.invert();
    wsc->sendText(head, DataResult::error("未知通信"));
}

void Controller::binHandler(WebSocketConnect *wsc, DataHead &head, QJsonDocument &json, QByteArray& data)
{
    head.showHTTP();
    bctrl->handler(wsc,head,json,data);
}

HttpController::HttpController()
{
    mapper=Mapper::getInstance(10);

    fmap.insert(Pair("login", &HttpController::login));
    fmap.insert(Pair("regist_code", &HttpController::regist_code));
    fmap.insert(Pair("regist_confirm", &HttpController::regist_confirm));
    fmap.insert(Pair("userinfo", &HttpController::userinfo));
    INSERT("rename", &HttpController::rename);
    INSERT("search", &HttpController::search);
    INSERT("invite", &HttpController::invite);
    INSERT("myinvite", &HttpController::myinvite);
    INSERT("reinvite", &HttpController::reinvite);
    INSERT("friendlist", &HttpController::friendlist);
    INSERT("dlefriend", &HttpController::dlefriend);
}

HttpController::~HttpController()
{
    delete mapper;
}

void HttpController::login(WebSocketConnect *wsc, DataHead &head, DataResult &result)
{
    head.showHTTP();
    bool v = mapper->login_mapper(head, result);
    head.invert();
    if(v){
        QJsonObject jo;
        QString account=result.jsdata.object().value("account").toString();
        jo.insert("account", account);
        wsc->sendText(head, DataResult(200, QJsonDocument(jo)));
        //升级连接
        TempConnect* tc=dynamic_cast<TempConnect*>(wsc);
        tc->upgrade(account);
    }else{
        DataResult rs = DataResult::error("密码错误或账号不存在");
        wsc->sendText(head, rs);
    }
}

void HttpController::regist_code(WebSocketConnect *wsc, DataHead &head, DataResult &result)
{
    head.showHTTP();
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
    head.showHTTP();
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

void HttpController::userinfo(WebSocketConnect *wsc, DataHead &head, DataResult &result)
{
    head.showHTTP();
    QString account=result.getstr("account");
    head.invert();
    User* user=mapper->userinfo(account);
    DataResult rs;
    if(user){
        rs.code=DataResult::code_success;
        rs.jsdata=QJsonDocument(user->json());
    }else{
        rs=DataResult::error("用户不存在");
    }
    wsc->sendText(head, rs);
    delete user;
}

void HttpController::rename(WebSocketConnect *wsc, DataHead &head, DataResult &result)
{
    head.showHTTP();
    head.invert();
    QString account=result.getstr("account");
    QString newname=result.getstr("newname");
    bool b=mapper->rename(account, newname);
    if(b){
        wsc->sendText(head, DataResult::success());
    }else
        wsc->sendText(head, DataResult::error("修改失败"));
}

void HttpController::search(WebSocketConnect *wsc, DataHead &head, DataResult &result)
{
    head.showHTTP();
    head.invert();
    QString act=result.getstr("account");
    QString frd=result.getstr("friend");
    if(frd.length()>8){
        //查找用户
        User* user=mapper->userinfo(frd);
        if(user==nullptr){
            wsc->sendText(head, DataResult::error("用户不存在"));
            return;
        }
        //查看是否为好友
        bool b=mapper->isFriend(act, frd);
        QJsonObject jo=user->json();
        jo.insert("isfriend",b);
        wsc->sendText(head, DataResult(DataResult::code_success, QJsonDocument(jo)));
    }else{
        //查找群
        wsc->sendText(head, DataResult::error("群聊不存在"));
    }
}

void HttpController::invite(WebSocketConnect *wsc, DataHead &head, DataResult &result)
{
    head.showHTTP();
    head.invert();
    bool b = mapper->invite(result);
    if(b){
        wsc->sendText(head, DataResult::success());
    }
}

void HttpController::myinvite(WebSocketConnect *wsc, DataHead &head, DataResult &result)
{
    HEADSHOW;
    QString act=result.getstr("account");
    QJsonArray ja=mapper->myinvite(act);
    wsc->sendText(head, DataResult(DataResult::code_success, QJsonDocument(ja)));
}

void HttpController::reinvite(WebSocketConnect *wsc, DataHead &head, DataResult &result)
{
    HEADSHOW;
    QJsonObject jo=result.jsdata.object();
    qDebug()<<jo;
    int id=jo.value("id").toInt();
    bool status=jo.value("status").toBool();
    mapper->delinvite(id);
    QString invitees=result.getstr("invitees");//被邀请人
    QString name=result.getstr("name");
    QString selfname=result.getstr("self");
    if(status){
        if(jo.value("group").isUndefined()){//好友邀请
            QString act=result.getstr("account");
            mapper->newfriend(act, invitees, name, selfname);
        }else{//群邀请
            QString group=result.getstr("group");
        }
    }
}

void HttpController::friendlist(WebSocketConnect *wsc, DataHead &head, DataResult &result)
{
    HEADSHOW;
    QString act=result.getstr("account");
    auto flist = mapper->friendlist(act);
    QJsonArray ja;
    for(Frd& v : flist){
        v.online=Application::online(v.act);
        ja.append(v.json());
    }
    wsc->sendText(head, DataResult(DataResult::code_success, QJsonDocument(ja)));
}

void HttpController::dlefriend(WebSocketConnect *wsc, DataHead &head, DataResult &result)
{
    HEADSHOW;
    QString act=result.getstr("account");
    QString frd=result.getstr("friend");
    mapper->dlefriend(act,frd);
}
