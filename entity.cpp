#include "entity.h"

void User::bindValues(QSqlQuery &query)
{
    query.bindValue(":email", email);
    query.bindValue(":account", account);
    query.bindValue(":password", password);
    query.bindValue(":name", name);
    query.bindValue(":create_time", create_time.toString("yyyy-MM-dd HH:mm:ss"));
}

QJsonObject User::json()
{
    QJsonObject jo;
    jo.insert("id",id);
    jo.insert("email",email);
    jo.insert("account",account);
    jo.insert("password",password);
    jo.insert("name",name);
    jo.insert("create_time",create_time.toString("yyyy-MM-dd HH:mm:ss"));
    jo.insert("imgPath", imgPath);
    return jo;
}

QJsonObject Frd::json()
{
    QJsonObject jo;
    jo.insert("id",id);
    jo.insert("account",act);
    jo.insert("name",name);
    jo.insert("online",online);
    return jo;
}


Information Information::enjson(const QJsonObject &jo)
{
    Information info;

    // 从 JSON 对象中提取数据
    info.id = jo.value("id").toVariant().toLongLong(); // 解析 id
    info.sender = jo.value("sender").toString();      // 解析 sender
    info.reciver = jo.value("reciver").toString();    // 解析 reciver
    info.time = QDateTime::fromString(jo.value("time").toString(), "yyyy-MM-dd HH:mm:ss"); // 解析 time
    info.type = jo.value("type").toString().at(0).toLatin1(); // 解析 type，取第一个字符
    info.msg = jo.value("msg").toString();            // 解析 msg

    return info;
}

QJsonObject Information::json()
{
    QJsonObject jo;

    // 将信息的各个字段添加到 JSON 对象中
    jo["id"] = static_cast<long long>(id); // 将 id 转换为 long long
    jo["sender"] = sender;
    jo["reciver"] = reciver;
    jo["time"] = time.toString("yyyy-MM-dd HH:mm:ss");
    jo["type"] = QString(type);               // 将 char 转换为 QString
    jo["msg"] = msg;

    return jo;
}
