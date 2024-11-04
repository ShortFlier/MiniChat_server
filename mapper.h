#ifndef MAPPER_H
#define MAPPER_H

#include "dbconnect.h"

#include <QObject>


//数据库业务管理类
class Mapper : public QObject{
    Q_OBJECT
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
    //重命名
    bool rename(const QString& account, const QString& newname);

private:
    DBConnectPool* dbpool;
    QMap<QString, QString> path_sql;

};

#endif // MAPPER_H
