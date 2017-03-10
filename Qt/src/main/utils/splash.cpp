#include "splash.h"
#include "src/main/common.h"
#include <QEvent>
#include "inc/xtDebug.h"

SplashDlg::SplashDlg(QWidget *parent, Qt::WFlags flags)
    : QDialog(parent, flags)
{
    setWindowFlags(Qt::FramelessWindowHint);
    this->resize(SCREEN_SIZE_W, SCREEN_SIZE_H);
    this->hide();
    this->setObjectName("SplashDlg");
}

SplashDlg::~SplashDlg()
{

}


SplashDlg* SplashDlg::getInstance()
{
    static SplashDlg splash;

    return &splash;
}


bool SplashDlg::event ( QEvent * event )
{
#if JUST_FOR_DEMO
    return QDialog::event(event);
#else
    QEvent::Type type = event->type();
    if (type == QEvent::MouseButtonPress ||
        type == QEvent::KeyPress ||
        type == QEvent::MouseMove)
    {
        this->hide();
        emit exitSplash();
    }
    else {
        return QDialog::event(event);
    }

    return true;
#endif
}

void SplashDlg::splash()
{
    if (!this->isVisible()) {
        this->show();
    }
}