#include "inc/control/mPushButton.h"
#include "../encrypt/encrypt.h"

#include <QStylePainter>
#include <QKeyEvent>
#include <QDebug>
#include <QTimer>
#include <QStyleOptionButton>


MPushButton::MPushButton(QWidget* parent) 
: QPushButton(parent) 
, m_bPressed(false)
, m_priorityDrawStatus(PRESSED_PRIORITY)
, m_bSplash(false)
, m_bSplashing(false)
{

}

MPushButton::~MPushButton()
{

}

void MPushButton::SetPressMode(bool bPressed)
{
    if (m_bPressed != bPressed) {
        m_bPressed = bPressed;
    }
    emit this->repaint();
}

void MPushButton::paintEvent(QPaintEvent *e)
{
    QStylePainter p(this);
    QStyleOptionButton option;
    initStyleOption(&option);

    if (m_priorityDrawStatus == PRESSED_PRIORITY) {
        if (m_bPressed) {
            option.state &= ~QStyle::State_Raised;
            option.state |= QStyle::State_Sunken;
        }
    }
    else {
        if (m_bSplash) {
            if (!m_bSplashing) {
                m_bSplashing = true;
                QTimer::singleShot(200, this, SLOT(finishSplash()));
            }
            option.state &= ~QStyle::State_Raised;
            option.state |= QStyle::State_Sunken;
        }
        else {
            if (!(option.state&QStyle::State_HasFocus)) {
                if (m_bPressed) {
                    option.state &= ~QStyle::State_Raised;
                    option.state |= QStyle::State_Sunken;
                }
            }
        }
    }

    p.drawControl(QStyle::CE_PushButton, option);    
}

void MPushButton::keyPressEvent(QKeyEvent* event)
{
    if (!verify_encrypt()) {
        return ;
    }

    switch(event->key()) 
    {
    case Qt::Key_Enter:
    case Qt::Key_Return:
        click();
        if (priorityDrawStatus() != PRESSED_PRIORITY)
            splash();
        event->ignore();
        break;
    default:
        event->ignore();
        break;
    }
    QPushButton::keyPressEvent(event);
}

void MPushButton::splash(void)
{
    m_bSplash = true;
    m_bSplashing = false;
    repaint();
}

void MPushButton::finishSplash(void)
{
    m_bSplash = false;
    m_bSplashing = false;
    if (m_priorityDrawStatus == SPLASHED_LOST_FOCUS_SET_PRESSED) {
        SetPressMode(!pressMode());
    }
    else {
        repaint();
    }
}