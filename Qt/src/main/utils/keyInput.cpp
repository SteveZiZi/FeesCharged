#include "keyInput.h"

#ifndef WIN32

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <linux/input.h>

#include <QApplication>
#include <QKeyEvent>
#include <QWidget>

#include "inc/xtDebug.h"

#define DEV_NAME "/dev/input/event0"

CKeyInputThread::CKeyInputThread(QObject *parent /* = 0 */)
: MThread(parent)
, m_fd(-1)
{
    devInit();
}

CKeyInputThread::~CKeyInputThread()
{
    devDeinit();
}

void CKeyInputThread::_postKeyEvent(quint16 code, quint32 value)
{
    if (code>= 112 && code <= 116) {
        int key = Qt::Key_F1 + code-112;
        QWidget * _focusWidget = QApplication::focusWidget();
        if (value == 0) {
            if (_focusWidget && _focusWidget->isActiveWindow())
            {
                xtRelease(("KeyRelease: %d  %d", code, value));
                if (_focusWidget->objectName() == "SplashDlg") {
                    QKeyEvent e(QEvent::KeyRelease,key,Qt::NoModifier);
                    QCoreApplication::sendEvent(_focusWidget, &e);
                }
                else {
                    QKeyEvent *e = new QKeyEvent(QEvent::KeyRelease,key,Qt::NoModifier);
                    QCoreApplication::postEvent(_focusWidget, e);
                }
            }
        }
        else {
            if (_focusWidget && _focusWidget->isActiveWindow()) {
                xtRelease(("KeyPress: %d  %d", code, value));
                if (_focusWidget->objectName() == "SplashDlg") {
                    QKeyEvent e(QEvent::KeyPress,key,Qt::NoModifier);
                    QCoreApplication::sendEvent(_focusWidget, &e);
                }
                else {
                    QKeyEvent *e = new QKeyEvent(QEvent::KeyPress,key,Qt::NoModifier);
                    QCoreApplication::postEvent(_focusWidget, e);
                }
            }
        }
    }
}

void CKeyInputThread::run()
{
    fd_set rset;
    int ret;
    size_t rb;
    struct input_event ev;
    do {
        FD_ZERO(&rset);
        FD_SET(m_fd, &rset);
        ret = ::select(m_fd+1, &rset, NULL, NULL, NULL);
        if (ret > 0) {
            rb=::read(m_fd,&ev,sizeof(struct input_event));
            if (ev.type == EV_KEY ){
                _postKeyEvent(ev.code, ev.value);
            }
        }
        else {
            msleep(500);
        }
    } while(!waitCondition(10));
}

bool CKeyInputThread::devInit()
{
    m_fd = ::open(DEV_NAME, O_RDWR|O_NDELAY);
    if (m_fd < 0) {
        xtError( ("open key device failed.") );
        return false;
    }

    int version;
    /* ioctl() accesses the underlying driver */
    if (::ioctl(m_fd, EVIOCGVERSION, &version)) {
        xtError(("evdev ioctl failed."));
    }

    /* the EVIOCGVERSION ioctl() returns an int */
    /* so we unpack it and display it */
    xtRelease(("evdev driver version is %d.%d.%d\n",
            version >> 16, (version >> 8) & 0xff,
            version & 0xff));

    return true;
}

void CKeyInputThread::devDeinit()
{
    if (this->isRunning()) {
        this->exitThread();
        while(!waitCondition(100));
        this->quit();
    }

    if (m_fd >= 0) {
        ::close(m_fd);
        m_fd = -1;
    }
}

#endif