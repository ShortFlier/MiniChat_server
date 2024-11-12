#include "application.h"
#include <cstdlib>
#include <ctime>

WebManager* Application::wm=new WebManager;

Application::Application(QObject *parent)
    : QObject{parent}
{
    srand(time(0));
}

void Application::start()
{
    wm->start();
}

bool Application::online(const QString &act)
{
    return wm->online(act);
}
