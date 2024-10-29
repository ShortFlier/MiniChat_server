#include "entity.h"

void User::bindValues(QSqlQuery &query)
{
    query.bindValue(":email", email);
    query.bindValue(":account", account);
    query.bindValue(":password", password);
    query.bindValue(":name", name);
    query.bindValue(":create_time", create_time.toString("yyyy-MM-dd HH:mm:ss"));
}
