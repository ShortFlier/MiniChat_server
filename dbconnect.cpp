#include "dbconnect.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMutexLocker>
#include <QRegularExpression>

DBConnectPool::DBConnectPool(QObject *parent)
    : QObject{parent}
{
    QFile file(":/assets/databse.config.txt");
    file.open(QIODeviceBase::ReadOnly);
    if(file.isOpen()){
        QTextStream ts(&file);
        sql=ts.readLine();
        ip=ts.readLine();
        port=ts.readLine().toInt();
        user=ts.readLine();
        password=ts.readLine();
        database=ts.readLine();
    }else
        qDebug()<<"assets/databse.config.txt 打开失败";
    file.close();
}

DBConnectPool::~DBConnectPool()
{
    while(!dblist.isEmpty()){
        QSqlDatabase* db = dblist.takeFirst();
        db->close();
        delete db;
    }
}

bool DBConnectPool::init(int size)
{
    this->size=size;
    qDebug()<<QSqlDatabase::drivers();
    for(int i=0; i<size; ++i){
        QSqlDatabase db=QSqlDatabase::addDatabase(sql,sql+QString::number(i+1));
        db.setHostName(ip);
        db.setUserName(user);
        db.setDatabaseName(database);
        db.setPassword(password);
        db.setPort(port);

        if(!db.open()){
            qDebug()<<"失败："<<db.lastError().text();
            return false;
        }
        //加入连接池
        dblist.append(&db);
    }
    return true;
}

QSqlDatabase *DBConnectPool::getdb()
{
    while(true){
        QMutexLocker<QMutex> locker(&mutex);
        if(dblist.isEmpty()){
            locker.unlock();
            cdv.wait(&mutex);
        }else{

            return dblist.takeFirst();
        }
    }
}

void DBConnectPool::append(QSqlDatabase *db)
{
    {
        QMutexLocker<QMutex> locker(&mutex);
        dblist.append(db);
    }
    cdv.notify_one();
}

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
    QFile file(":/assets/sql.txt");
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
