#include "bincontroller.h"

#define INSERT(path,ptr) fmaps.insert(Pair(path, ptr))

BinController::BinController(QObject *parent)
    : QObject{parent}
{
    INSERT("uimgload", &BinController::uimgload);
    INSERT("uimgdown", &BinController::uimgdown);
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
    if(dt){ //这里可能存在内存泄漏
        QByteArray b(std::move(*dt));
        wsc->sendBinary(head, DataResult::code_success, b);
        delete dt;
    }else{
        QByteArray b;
        wsc->sendBinary(head, DataResult::code_error, b);
    }

}
