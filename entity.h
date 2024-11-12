#ifndef ENTITY_H
#define ENTITY_H

#include <QDateTime>
#include <QJsonObject>
#include <QSqlQuery>
#include <QString>



struct User
{
    int id;
    QString email;
    QString account;
    QString password;
    QString name;
    QDateTime create_time;
    QString imgPath;

    User(const QString& email, const QString account, const QString password, const QString name)
        :email(email),account(account),password(password),name(name)
    {
        create_time=QDateTime::currentDateTime();
    }
    User(){}
    User(const int& id, const QString& email, const QString account, const QString password, const QString name, const QDateTime& create_time)
        :id(id),email(email),account(account),password(password),name(name),create_time(create_time){}

    void bindValues(QSqlQuery& query);
    QJsonObject json();
};


struct Frd{
    int id;
    QString act;
    QString name;
    bool online;

    Frd(int id, const QString& act, const QString& name, bool online=false):id(id),act(act),name(name),online(online){}
    QJsonObject json();
};

#endif // ENTITY_H
