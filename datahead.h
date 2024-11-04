#ifndef DATAHEAD_H
#define DATAHEAD_H

#include <QObject>
#include <qjsondocument.h>
#include <QJsonObject>

class DataHead : public QObject
{
    Q_OBJECT
public:
    explicit DataHead(QString str, QObject *parent = nullptr);
    DataHead operator=(const DataHead&)=delete;
    DataHead(const DataHead&)=delete;
    ~DataHead();
    int show();
    void showHTTP(){qDebug()<<*_tp_type<<seph<<*_http_type<<sep<<*_path<<sep<<*_tpid<<sepe;}
    DataHead(const QString& _tp_type,const QString& _http_type,const QString& _path,const QString& _tpid);
    QString getUrl() const;
    static QString getUrl(const QString& _tp_type,const QString& _http_type,const QString& _path,const QString& _tpid);
    void invert();


public:
    enum tp_type{ws, htp};
    static const QString websocket;
    static const QString http;

    enum http_type{rqe, rsp};
    static const QString request;
    static const QString response;

    static const QString seph;
    static const QString sep;
    static const QString sepe;

signals:


public:
    QString* _tp_type=nullptr;
    QString* _http_type=nullptr;
    QString* _tpid=nullptr;
    QString* _path=nullptr;
};

class DataResult{
public:
    DataResult(){}
    DataResult(const int& code, const QJsonDocument& jsdata):code(code),jsdata(jsdata){}
    DataResult(const QString& datastr);
    //合并code和jsdata,转成格式可以发送的数据部分
    QString data() const;
    QString getstr(const QString& str)const { return jsdata.object().value(str).toString();}
    void show()const{ qDebug()<<"code:"<<code<<jsdata.object();}
    //生成一个错误result
    static DataResult error(const QString& msg);
    //生成成功的result
    static DataResult success(const QString& msg="success");

public:
    enum code_result{code_success=200, code_error=400};

public:
    int code=0;
    QJsonDocument jsdata;

};

#endif // DATAHEAD_H
