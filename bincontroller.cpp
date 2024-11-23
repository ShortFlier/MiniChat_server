#include "bincontroller.h"
#include "entity.h"

#define INSERT(path,ptr) fmaps.insert(Pair(path, ptr))

BinController::BinController(QObject *parent)
    : QObject{parent}
{
    mapper=Mapper::getInstance();
    INSERT("uimgload", &BinController::uimgload);
    INSERT("uimgdown", &BinController::uimgdown);
    INSERT("sendimg", &BinController::sendimg);
}

void BinController::handler(WebSocketConnect *wsc, DataHead &head, QJsonDocument &json, QByteArray& data)
{
    QString path=*head._path;
    (this->*fmaps[path])(wsc, head, json, data);
}

void BinController::uimgload(WebSocketConnect *wsc, DataHead &head, QJsonDocument &json, QByteArray& data)
{
    qDebug()<<"uimgload";
    //保存图片到用户头像文件夹
    head.invert();
    QString account=json.object().value("account").toString();
    bool b=fc.uimgsave(account, data);
    QByteArray ba;
    if(b)
        wsc->sendBinary(head, DataResult::code_success, ba);
    else
        wsc->sendBinary(head, DataResult::code_error, ba);
}

void BinController::uimgdown(WebSocketConnect *wsc, DataHead &head, QJsonDocument &json, QByteArray &data)
{
    qDebug()<<"uimgdown";
    head.invert();
    QString account=json.object().value("account").toString();
    QByteArray* dt= fc.uimgdown(account);
    if(dt){
        QByteArray b(std::move(*dt));
        wsc->sendBinary(head, DataResult::code_success, b);
        delete dt;
    }else{
        QByteArray b;
        wsc->sendBinary(head, DataResult::code_error, b);
    }

}

void BinController::sendimg(WebSocketConnect *wsc, DataHead &head, QJsonDocument &json, QByteArray &data)
{
    qDebug()<<"sendimg";
    Information info;
    QJsonObject jo=json.object();
    info.sender=jo.value("sender").toString();
    info.reciver=jo.value("reciver").toString();
    info.type=jo.value("type").toInt();
    info.msg="image.jpg";
    info.time=QDateTime::currentDateTime();
    long id=mapper->sendimg(info);
    if(id)
        fc.chatimg(id, data);
}
