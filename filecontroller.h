#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H

#include <QObject>

#define UIMAGE_DIR "./assets/uimages/"
#define CHATIMG_DIR "./assets/chatimgs/"

#define UIMAGE_TAIL ".jpg"

#define IMAGE_TAIL ".jpg"

//对用户头像等文件数据管理
class FileController : public QObject
{
    Q_OBJECT
public:
    explicit FileController(QObject *parent = nullptr);
    //保存用户头像
    bool uimgsave(const QString& account, QByteArray& data);
    //尝试打开用户头像文件，如果没有，返回空字符串
    static QString getUImg(const QString& account);
    //获取用户头像，如果没有返回nullptr
    QByteArray* uimgdown(QString account);
    //保存聊天图片
    bool chatimg(const long& id, QByteArray& data);
signals:
};

#endif // FILECONTROLLER_H
