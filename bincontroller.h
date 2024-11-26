#ifndef BINCONTROLLER_H
#define BINCONTROLLER_H

#include "datahead.h"
#include "filecontroller.h"
#include "mapper.h"
#include "websocketconnect.h"

#include <QObject>
#include <map>


class BinController : public QObject
{
    Q_OBJECT

    typedef std::pair<QString, void(BinController::*)(WebSocketConnect*, DataHead&, QJsonDocument&, QByteArray& data)> Pair;

public:
    explicit BinController(QObject *parent = nullptr);
    void handler(WebSocketConnect *wsc, DataHead &head, QJsonDocument &json, QByteArray& data);

signals:

private:
    std::map<QString, void (BinController::*)(WebSocketConnect*, DataHead&, QJsonDocument&, QByteArray& data)> fmaps;
    FileController fc;
    Mapper* mapper;
    //用户头像上传
    void uimgload(WebSocketConnect* wsc, DataHead& head, QJsonDocument& json, QByteArray& data);
    //下载用户头像
    void uimgdown(WebSocketConnect* wsc, DataHead& head, QJsonDocument& json, QByteArray& data);
    //图片接收
    void sendimg(WebSocketConnect* wsc, DataHead& head, QJsonDocument& json, QByteArray& data);
    //图片发送
    void imgsend(WebSocketConnect* wsc, const QString& imgname, QByteArray* data);
};

#endif // BINCONTROLLER_H
