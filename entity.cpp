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
    return jo;
}
