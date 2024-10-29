#include "application.h"
#include <cstdlib>
#include <ctime>

Application::Application(QObject *parent)
    : QObject{parent}
{
    srand(time(0));
}

void Application::start()
{
    wm.start();
}
