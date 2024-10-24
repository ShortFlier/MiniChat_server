#include <QCoreApplication>

#include <QSqlDatabase>
#include <QSqlError>

#include "webmanager.h"

#include "littlesmtp.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Set up code that uses the Qt event loop here.
    // Call a.quit() or a.exit() to quit the application.
    // A not very useful example would be including
    // #include <QTimer>
    // near the top of the file and calling
    // QTimer::singleShot(5000, &a, &QCoreApplication::quit);
    // which quits the application after 5 seconds.

    // If you do not need a running Qt event loop, remove the call
    // to a.exec() or use the Non-Qt Plain C++ Application template.


    WebManager wm;
    wm.start();


    qDebug()<<QSqlDatabase::drivers();

    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setDatabaseName("minichat");
    db.setPassword("administrator");
    db.setPort(3306);

    if(!db.open()){
        qDebug()<<"失败："<<db.lastError().text();
    }else
        qDebug()<<"成功";

    EmailSender smtp;

    return a.exec();
}
