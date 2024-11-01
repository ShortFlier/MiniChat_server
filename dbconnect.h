#ifndef DBCONNECT_H
#define DBCONNECT_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QList>
#include <QMutex>
#include <QWaitCondition>
#include <QMap>
#include <QJsonObject>
#include <QSqlQuery>

#include "datahead.h"
#include "entity.h"

//数据库连接池
class DBConnectPool : public QObject
{
    Q_OBJECT
public:
    explicit DBConnectPool(QObject *parent = nullptr);
    ~DBConnectPool();
    bool init(int size=10);
    //取出一个连接
    QSqlDatabase getdb();
    //将已取出的连接放回
    void append(QSqlDatabase db);

signals:


private:
    int size;

    QString sql;
    QString ip;
    int port;
    QString user;
    QString password;
    QString database;

    QList<QSqlDatabase> dblist;

    QMutex mutex;
    QWaitCondition cdv;
};


//数据库业务管理类
class Mapper{
public:
    explicit Mapper(int size=10);
    ~Mapper();
    //sql语句解析
    void load();

    //验证账号密码是否正确
    bool login_mapper(DataHead& head, DataResult& result);
    //邮箱是否已注册
    bool hasEmail_mapper(const QString& email);
    //新用户
    bool newUser(User& user);
    //获取用户信息
    User* userinfo(const QString& account);

private:
    DBConnectPool* dbpool;
    QMap<QString, QString> path_sql;

};

#endif // DBCONNECT_H
