#include "datahead.h"
#include <QJsonObject>
#include <qdebug.h>

const QString DataHead::websocket="websocket";
const QString DataHead::http="http";

const QString DataHead::request="request";
const QString DataHead::response="response";

const QString DataHead::seph="://";
const QString DataHead::sep="/";
const QString DataHead::sepe="\r\n";

//通信头字符串解析
DataHead::DataHead(QString str, QObject *parent)
    : QObject{parent}
{
    str.remove(sepe);
    QStringList slist = str.split(seph);
    if(slist.size()>0){
        _tp_type=new QString(slist[0]);
        if(*_tp_type==websocket){   //消息推送模式
            QStringList list=slist[1].split(sep);
            _path=new QString(list[0]);
        }else if(*_tp_type==http){  //http模式
            QStringList list=slist[1].split(sep);
            _http_type=new QString(list[0]);
            _path=new QString(list[1]);
            _tpid=new QString(list[2]);
        }else{
            delete _tp_type;
            _tp_type=nullptr;
        }
    }
}

DataHead::~DataHead()
{
    delete _tp_type;
    delete _http_type;
    delete _path;
    delete _tpid;
}

int DataHead::show()
{
    int i=0;
    if(_tp_type){
        ++i;
        qDebug()<<"_tp_type: "<<*_tp_type;
    }
    if(_http_type){
        ++i;
        qDebug()<<"_http_type: "<<*_http_type;
    }
    if(_tpid){
        ++i;
        qDebug()<<"_tpid: "<<*_tpid;
    }
    if(_path){
        ++i;
        qDebug()<<"_path: "<<*_path;
    }
    return i;
}

DataHead::DataHead(const QString& _tp_type,const QString& _http_type,const QString& _path,const QString& _tpid)
{
    this->_tp_type=new QString(_tp_type);
    this->_http_type=new QString(_http_type);
    this->_tpid=new QString(_tpid);
    this->_path=new QString(_path);
}

QString DataHead::getUrl() const
{
    QString url;
    url=*_tp_type+seph;
    if(*_tp_type==websocket){
        url+=*_path+sep+0;
    }else if(*_tp_type==http){
        url+=*_http_type+sep+*_path+sep+*_tpid;
    }
    url+=sepe;
    return url;
}

QString DataHead::getUrl(const QString& _tp_type,const QString& _http_type,const QString& _path,const QString& _tpid)
{
    QString url;
    url=_tp_type+seph;
    if(_tp_type==websocket){
        url+=_path+sep+0;
    }else if(_tp_type==http){
        url+=_http_type+sep+_path+sep+_tpid;
    }
    url+=sepe;
    return url;
}

void DataHead::invert()
{
    if(_http_type!=nullptr){
        if(*_http_type==request)
            *_http_type=response;
        else
            *_http_type=request;
    }
}

DataResult::DataResult(const QString &datastr)
{
    if(!datastr.isEmpty()){
        QStringList strs=datastr.split(DataHead::sepe);
        bool s;
        code = strs[0].toInt(&s);
        if(s){  //请求——响应式
            if(strs.size()>1)   //防止出现仅包含响应码而不包含数据部分
                jsdata=QJsonDocument::fromJson(strs[1].toUtf8());
        }else{  //消息推送式
            code=0;
            jsdata=QJsonDocument::fromJson(strs[0].toUtf8());
        }
    }
}

QString DataResult::data() const
{
    QString data;
    if(code!=0){//说明不是消息推送
        data=QString::number(code)+DataHead::sepe;
    }
    if(!jsdata.isNull())
        data+=jsdata.toJson();
    return data;
}

DataResult DataResult::error(const QString& msg)
{
    QJsonObject jo;
    jo.insert("msg", msg);
    return DataResult(code_error, QJsonDocument(jo));
}

DataResult DataResult::success(const QString &msg)
{
    QJsonObject jo;
    jo.insert("msg", msg);
    return DataResult(code_success, QJsonDocument(jo));
}
