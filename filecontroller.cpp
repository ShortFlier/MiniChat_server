#include "filecontroller.h"

#include <QFile>
#include <QDebug>

#define PATH(act) QString path=UIMAGE_DIR+act+UIMAGE_TAIL;

FileController::FileController(QObject *parent)
    : QObject{parent}
{}

bool FileController::uimgsave(const QString &account, QByteArray &data)
{
    QString path=UIMAGE_DIR+account+UIMAGE_TAIL;
    QFile file(path);
    file.open(QIODeviceBase::WriteOnly);
    if(file.isOpen()){
        file.write(data);
    }else{
        qDebug()<<"uimgsave write error: "+path;
        file.close();
        return false;
    }
    file.close();
    return true;
}

QString FileController::getUImg(const QString &account)
{
    PATH(account);
    if(QFile::exists(path))
        return account+UIMAGE_TAIL;
    return QString();
}

QByteArray *FileController::uimgdown(QString account)
{
    QByteArray* data=nullptr;
    PATH(account);
    if(QFile::exists(path)){
        QFile file(path);
        file.open(QIODeviceBase::ReadOnly);
        if(file.isOpen()){
            data=new QByteArray(file.readAll());
        }
        file.close();
    }
    return data;
}

bool FileController::chatimg(const long &id, QByteArray &data)
{
    QString name=QString::number(id)+IMAGE_TAIL;
    QString path=CHATIMG_DIR+name;
    QFile file(path);
    file.open(QIODeviceBase::WriteOnly);
    bool b;
    if(b=file.isOpen()){
        file.write(data);
    }
    file.close();
    return b;
}
