#ifndef MAPPER_H
#define MAPPER_H

#include "dbconnect.h"

#include <QObject>
#include <vector>


//数据库业务管理类
class Mapper : public QObject{
    Q_OBJECT
public:
    static Mapper* getInstance(int size=10);
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
    //好友检查
    bool isFriend(const QString& act, const QString& frd);
    //邀请
    bool invite(DataResult& result);
    //获取邀请
    QJsonArray myinvite(const QString& account);
    //删除邀请
    bool delinvite(int id);
    //新好友
    bool newfriend(const QString& act, const QString& frd, const QString& name, const QString& selfname);
    //好友列表
    std::vector<Frd> friendlist(const QString& act);
    //删除好友
    bool dlefriend(const QString& act, const QString& frd);
    //新的用户间消息
    bool newmsg(Information& info);
    //用户登入后推送消息
    std::vector<Information> loginedmsg(const QString& act);
    bool loginedmsgdle(QJsonArray& ids);

private:
    explicit Mapper(int size=10);
    DBConnectPool* dbpool;
    QMap<QString, QString> path_sql;
    static Mapper* m;
};

#endif // MAPPER_H
