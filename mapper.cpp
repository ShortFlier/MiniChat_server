#include "mapper.h"
#include "filecontroller.h"

#include <QFile>
#include <QJsonArray>

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
        qDebug()<< "Error login_mapper:" << query.lastError().text();
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
        qDebug()<< "Error hasEmail_mapper:" << query.lastError().text();
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
        qDebug()<< "Error newUser:" << query.lastError().text();
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
        user->imgPath=FileController::getUImg(account);
    }else{
        qDebug()<< "Error userinfo:" << query.lastError().text();
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
    if(!b)
        qDebug()<< "Error rename:" << query.lastError().text();
    dbpool->append(db);
    return b;
}

bool Mapper::isFriend(const QString &act, const QString &frd)
{
    int c=0;
    SQL("search");
    QUERY;
    query.bindValue(":account",act);
    query.bindValue(":friend",frd);
    if(query.exec() && query.next())
        c=query.value(0).toInt();
    else
        qDebug()<< "Error isFriend:" << query.lastError().text();
    dbpool->append(db);
    return c;
}

bool Mapper::invite(DataResult &result)
{
    QJsonObject jo=result.jsdata.object();
    QString group;
    if(jo.value("group")!=QJsonValue::Undefined){
        group=jo.value("group").toString();
    }
    SQL("invite");
    QUERY;
    query.bindValue(":inviter", jo.value("inviter").toString());
    query.bindValue(":invitees", jo.value("invitees").toString());
    query.bindValue(":group", jo.value("group").toString());
    query.bindValue(":note", jo.value("note").toString());
    bool b=query.exec();
    if(!b)
        qDebug()<< "Error invite:" << query.lastError().text();
    dbpool->append(db);
    return b;
}

QJsonArray Mapper::myinvite(const QString &account)
{
    QJsonArray jar;
    SQL("myinvite");
    QUERY;
    query.bindValue(":invitees", account);
    if(query.exec()){
        while(query.next()){
            int id=query.value(0).toUInt();//id
            QString act=query.value(1).toString();//邀请人账号
            QString name=query.value(2).toString();//邀请人名
            QString group;//群号，不一定有
            QString note=query.value(4).toString();//备注
            QJsonObject jo;
            jo.insert("id",id);
            jo.insert("account",act);
            jo.insert("name", name);
            jo.insert("note", note);
            if(query.value(3).isNull()){//群邀请
                group=query.value(3).toString();
                jo.insert("group", group);
            }else{//好友邀请
                jo.insert("group", group);
            }
            jar.append(jo);
        }
    }else
        qDebug()<< "Error myinvite:" << query.lastError().text();
    dbpool->append(db);
    return jar;
}

bool Mapper::delinvite(int id)
{
    SQL("delinvite");
    QUERY;
    query.bindValue(":id", id);
    bool b=query.exec();
    if(!b)
        qDebug()<< "Error myinvite:" << query.lastError().text();
    dbpool->append(db);
    return b;
}

bool Mapper::newfriend(const QString &act, const QString &frd, const QString &name, const QString& selfname)
{
    SQL("newfriend");
    QUERY;

    // 开始事务
    if (!db.transaction()) {
        qDebug() << "Failed to start transaction newfriend:" << db.lastError().text();
        dbpool->append(db);
        return false;  // 或者根据需要采取其他措施
    }

    query.bindValue(":account", act);
    query.bindValue(":friend", frd);
    query.bindValue(":name", selfname);
    bool b=query.exec();
    if(!b){
        qDebug()<< "Error newfriend:" << query.lastError().text();
        db.rollback();
        dbpool->append(db);
        return false;
    }

    query.bindValue(":account", frd);
    query.bindValue(":friend", act);
    query.bindValue(":name", name);
    b=query.exec();
    if(!b){
        qDebug()<< "Error newfriend:" << query.lastError().text();
        db.rollback();
        dbpool->append(db);
        return false;
    }

    // 提交事务
    if (!db.commit()) {
        qDebug() << "Failed to commit transaction newfriend:" << db.lastError().text();
        dbpool->append(db);
        return false; // 或者根据需要采取其他措施
    }

    dbpool->append(db);
    return b;
}

std::vector<Frd> Mapper::friendlist(const QString &act)
{
    SQL("friendlist");
    QUERY;
    query.bindValue(":account",act);
    std::vector<Frd> fls;
    if(query.exec()){
        while(query.next()){
            fls.push_back(Frd(query.value(0).toInt(),query.value(1).toString(),query.value(2).toString()));
        }
    }else{
        qDebug()<< "Error friendlist:" << query.lastError().text();
    }
    dbpool->append(db);
    return fls;
}

bool Mapper::dlefriend(const QString &act, const QString &frd)
{
    SQL("dlefriend");
    QUERY;
    query.bindValue(":act",act);
    query.bindValue(":fdr",act);
    query.bindValue(":frd",frd);
    query.bindValue(":atc",frd);
    bool b=query.exec();
    if(!b)
        qDebug()<< "Error myinvite:" << query.lastError().text();
    query.lastQuery();
    dbpool->append(db);
    return b;
}
