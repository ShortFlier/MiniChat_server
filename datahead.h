#ifndef DATAHEAD_H
#define DATAHEAD_H

#include <QObject>

class DataHead : public QObject
{
    Q_OBJECT
public:
    explicit DataHead(QString str, QObject *parent = nullptr);
    DataHead operator=(const DataHead&)=delete;
    DataHead(const DataHead&)=delete;
    ~DataHead();
    int show();
    DataHead(const QString& _tp_type,const QString& _http_type,const QString& _path,const QString& _tpid);
    QString getUrl() const;
    static QString getUrl(const QString& _tp_type,const QString& _http_type,const QString& _path,const QString& _tpid);


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

#endif // DATAHEAD_H
