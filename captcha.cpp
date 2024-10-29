#include "captcha.h"
#include <cstdlib>
#include <ctime>

#include <QRegularExpression>
#include <QTimer>

Captcha::Captcha(int life_scends, QObject *parent)
    : QObject{parent}
{
    life=life_scends;
    //每秒执行一次的定时器
    QTimer* timer=new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Captcha::overTime);
    timer->start(1000);
}

QString Captcha::getValue(const QString &key)
{
    if(code.contains(key)){
        return code[key];
    }else
        return QString();
}

void Captcha::add(const QString &key, const QString &value)
{
    code[key]=value;
    code_life[key]=life;
}


QString Captcha::generateCaptcha(int length)
{
    const QString characters = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    QString captcha;

    for (int i =0; i < length; ++i) {
        //生成随机索引
        int index = std::rand() % characters.length();
        //追加到验证码字符串
        captcha += characters[index];
    }
        return captcha;
    }

bool Captcha::emailFormat(const QString &email)
{
    QRegularExpression emailPattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    QRegularExpressionMatch match = emailPattern.match(email);
    return match.hasMatch();
}

void Captcha::overTime()
{
    for(auto i=code_life.begin(); i!=code_life.end(); ++i){
        i.value()--;
        if(i.value()<=0){
            code.remove(i.key());
            code_life.remove(i.key());
        }
    }
}


QString generateRandomNumberString() {
    int length=9;
    length+=rand()%3;
    static QString numbers = "0123456789";
    QString result;

    // 随机选择不同的数字
    while (result.length() < length) {
        int index = rand() %10; // 在0-9之间随机选择
        result += numbers[index];
    }
    if(result[0]=='0'){
        int index =1+rand()%9; //1-9
        result[0]=numbers[index];
    }

    return result;
}

