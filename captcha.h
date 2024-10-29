#ifndef CAPTCHA_H
#define CAPTCHA_H

#include <QObject>
#include <QMap>

class Captcha : public QObject
{
    Q_OBJECT
public:
    explicit Captcha(int life_scends=180, QObject *parent = nullptr);
    //根据key获取value，不需要delete改指针
    QString getValue(const QString& key);
    //新赠一个key-value
    void add(const QString& key, const QString& value);


    //验证码生成函数
    static QString generateCaptcha(int length);

    //邮箱格式验证
    static bool emailFormat(const QString& email);

protected:
    QMap<QString, QString> code;
    int life;
private:
    QMap<QString, int> code_life;
    //定时清理过期的验证码
    void overTime();
signals:
};

QString generateRandomNumberString();

#endif // CAPTCHA_H
