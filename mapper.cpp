#include "mapper.h"

#include <QFile>

#define SQL(str) QString sql=path_sql.value(str);
//取出连接，准备sql
#define QUERY     QSqlDatabase db=dbpool->getdb(); QSqlQuery query(db); query.prepare(sql);

Mapper::Mapper(int size)
{
    dbpool=new DBConnectPool();
    dbpool->init(size);
    load();
}

Mapper::~Mapper()
{
    delete dbpool;
}

void Mapper::load()
{
    QFile file("./assets/sql.txt");
    file.open(QIODeviceBase::ReadOnly);
    if(file.isOpen()){
        //解析sql语句
        QTextStream ts(&file);
        while (!ts.atEnd()) {
            //获取xml 头部
            QString head=ts.readLine();
            QRegularExpression regex("^<sql.*>$");
            QRegularExpressionMatch match = regex.match(head);
            if(match.hasMatch()){
                //获取path
                int pos = head.indexOf("\"");
                QString path;
                while(true){
                    ++pos;
                    if(head.at(pos)!='\"')
                        path.append(head.at(pos));
                    else
                        break;
                }
                //搜索到结尾
                QString sql;
                while(!ts.atEnd()){
                    QString str=ts.readLine();
                    if(str=="</sql>")
                        break;
                    else
                        sql.append(str);
                }
                //载入map
                path_sql.insert(path, sql.trimmed());
            }
        }
        qDebug()<<path_sql;
    }else
        qDebug()<<"assets/sql.txt 打开失败";
}

bool Mapper::login_mapper(DataHead &head, DataResult &result)
{
    QString sql=path_sql.value(*head._path);
    QJsonObject jsob=result.jsdata.object();
    QStringList keys=jsob.keys();
    QSqlDatabase db=dbpool->getdb();
    QSqlQuery query(db);
    query.prepare(sql);
    for(QString& key : keys)
        query.bindValue(":"+key, jsob.value(key));
    int count=0;
    if(query.exec() && query.next()){
        count = query.value(0).toInt();
    }else
        qDebug()<< "Error inserting data:" << query.lastError().text();
    dbpool->append(db);
    return count;
}

bool Mapper::hasEmail_mapper(const QString& email)
{
    QString sql=path_sql.value("hasEmail");
    QSqlDatabase db=dbpool->getdb();
    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":email", email);
    int count=0;
    if(query.exec() && query.next()){
        count=query.value(0).toInt();
        qDebug()<<count;
    }else{
        qDebug()<< "Error inserting data:" << query.lastError().text();
    }
    dbpool->append(db);
    qDebug()<<"append";
    return count;
}

bool Mapper::newUser(User &user)
{
    QString sql=path_sql.value("newUser");
    QSqlDatabase db=dbpool->getdb();
    QSqlQuery query(db);
    query.prepare(sql);
    user.bindValues(query);
    if(query.exec()){
        dbpool->append(db);
        return true;
    }else{
        qDebug()<< "Error inserting data:" << query.lastError().text();
        dbpool->append(db);
        return false;
    }
}

User* Mapper::userinfo(const QString &account)
{
    SQL("userinfo");
    QUERY;
    query.bindValue(":account", account);
    User* user=nullptr;
    if(query.exec() & query.next()){
        //id,email,account,name,create_time,imgPath
        user=new User;
        user->id=query.value(0).toInt();
        user->email=query.value(1).toString();
        user->account=query.value(2).toString();
        user->name=query.value(3).toString();
        user->create_time=query.value(4).toDateTime();
        user->imgPath=query.value(5).toString();
    }else{
        qDebug()<< "Error inserting data:" << query.lastError().text();
    }
    dbpool->append(db);
    return user;
}

bool Mapper::rename(const QString &account, const QString &newname)
{
    SQL("rename");
    QUERY;
    query.bindValue(":account", account);
    query.bindValue(":name", newname);
    bool b=query.exec();
    dbpool->append(db);
    return b;
}
