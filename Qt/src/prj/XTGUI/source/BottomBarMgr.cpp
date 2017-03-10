#include "inc/BottomBarMgr.h"
#include "src/main/common.h"
#include "../encrypt/encrypt.h"

#include <QLabel>
#include <QTimer>
#include <QDateTime>

#define TIME_SHOW_L     0
#define TIME_SHOW_T     0
#define TIME_SHOW_W     240
#define TIME_SHOW_H     BOTTOM_BAR_SIZE_H

#define STATUS_SHOW_L  (TIME_SHOW_W+2)
#define STATUS_SHOW_T   TIME_SHOW_T
#define STATUS_SHOW_W  (SCREEN_SIZE_W-TIME_SHOW_W-2)
#define STATUS_SHOW_H   TIME_SHOW_H


class CBottomBarMgrPrivate {
public:
    void splash();

private:
    QLabel* label_time;
    QLabel* label_status;
    QTimer* timer;

    int showTimes;      //闪烁的次数

    QRgb defaultClr;
    QRgb splashClr;
    bool splashFlag;

    friend class CBottomBarMgr;
};

void CBottomBarMgrPrivate::splash()
{
    QColor clr = defaultClr;
    if(splashFlag) {
        clr = splashClr;
    }
    QString str_clr = QString("color:#%1%2%3").arg(clr.red(), 2, 16, QChar('0'))
                                              .arg(clr.green(), 2, 16, QChar('0'))
                                              .arg(clr.blue(), 2, 16, QChar('0'));
    label_status->setStyleSheet(str_clr);
    splashFlag = !splashFlag;
}


CBottomBarMgr::CBottomBarMgr(QWidget* parent/* = 0*/, Qt::WindowFlags f/* = 0*/)
: QWidget(parent, f)
, m_data(new CBottomBarMgrPrivate())
{
    if (!verify_encrypt()) {
        return;
    }
    this->setGeometry(BOTTOM_BAR_POS_L, BOTTOM_BAR_POS_T, BOTTOM_BAR_SIZE_W, BOTTOM_BAR_SIZE_H);
    this->setObjectName(QString::fromLocal8Bit("bottomBarMgr"));

    m_data->label_time = new QLabel(this);
    m_data->label_time->setGeometry(TIME_SHOW_L, TIME_SHOW_T, TIME_SHOW_W, TIME_SHOW_H);
    m_data->label_time->setObjectName("bottomBar_systemTime");
    m_data->label_time->setAlignment(Qt::AlignCenter);

    m_data->timer = new QTimer(this);
    m_data->timer->start(500);
    connect(m_data->timer, SIGNAL(timeout()), this, SLOT(updateTime()));

    m_data->label_status = new QLabel(this);
    m_data->label_status->setGeometry(STATUS_SHOW_L, STATUS_SHOW_T, STATUS_SHOW_W, STATUS_SHOW_H);
    m_data->label_status->setObjectName("bottomBar_status");
    m_data->label_status->setText("");
    m_data->label_status->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

    m_data->defaultClr = qRgb(255, 0, 0);
    m_data->splashClr = qRgb(255, 255, 0);
    m_data->splashFlag = false;
    m_data->showTimes = 0;
}

CBottomBarMgr::~CBottomBarMgr()
{
    if (m_data->timer && m_data->timer->isActive()) {
        m_data->timer->stop();
    }    
    delete m_data;
}

/*
 *  @Func:  setStatusColor
 *  @Desc:  设置状态闪烁的颜色值
 *  
 *  @Param[in]:
 *      defaultClr:   默认情况下的颜色
 *      splashClr:    闪烁的颜色，但闪烁的颜色和默认颜色相同时，即可实现不闪烁
 */
void CBottomBarMgr::setStatusColor(QRgb defaultClr, QRgb splashClr)
{
    m_data->defaultClr = defaultClr;
    m_data->splashClr = splashClr;
}

void CBottomBarMgr::updateTime()
{
    if (m_data->label_time) {
        m_data->label_time->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    }

    if (m_data->showTimes == -1) {
        m_data->splash();
    }
    else if (m_data->showTimes != 0) {
        m_data->splash();
        m_data->showTimes--;
    }
    else {
        m_data->label_status->setText("");
    }
}

/*
 *  @Func:  showStatus
 *  @Desc:  显示状态栏上的运行状态
 *  
 *  @Param[in]:
 *      text:   状态内容
 *      timesS: 需要显示的时间  -1:一直显示，知道有新的状态到来
 */
void CBottomBarMgr::showStatus(const QString& text, int timesS)
{
    if(timesS != -1) {
        m_data->showTimes = timesS*2;
    }
    else m_data->showTimes = -1;

    m_data->label_status->setText(text);
    updateTime();
}