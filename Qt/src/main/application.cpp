#include "application.h"
#include <QTimer>
#include <QEvent>

#define USER_INACTIVE_TIMER_INTERVAL      1000

Application::Application(int &argc, char **argv)
: QApplication(argc,argv)
, inactiveCnt(0)
, timeoutTimeS(60)
{
    inactiveTimer = new QTimer(this);
    inactiveTimer->setInterval(USER_INACTIVE_TIMER_INTERVAL);
    connect(inactiveTimer, SIGNAL(timeout()),this,SLOT(onTimeout()));

    userActive();
}

bool Application::notify(QObject *obj, QEvent *e)
{
    QEvent::Type type = e->type();
    if( type == QEvent::MouseButtonPress ||
        type == QEvent::KeyPress ||
        type == QEvent::MouseMove)
    {
        this->inactiveCnt = 0;
    }

    return QApplication::notify(obj,e);
}

void Application::onTimeout()
{
    this->inactiveCnt++;
    if (this->inactiveCnt >= this->timeoutTimeS) {
        this->inactiveTimer->stop();
        emit this->userInActived();
    }
}

void Application::userActive()
{
    this->inactiveCnt = 0;
    if (!this->inactiveTimer->isActive()) {
        this->inactiveTimer->start();
    }
}