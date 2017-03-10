#pragma once

#include <QtGui/QWidget>

//
//-----------------------------------------------------------------------------
// CNaviMenuMgr
//
//  管理导航菜单的创建
//-----------------------------------------------------------------------------
//
class CBottomBarMgrPrivate;
class CBottomBarMgr : public QWidget
{
    Q_OBJECT

public:
    CBottomBarMgr(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~CBottomBarMgr();

public:
    /*
     *  @Func:  setStatusColor
     *  @Desc:  设置状态闪烁的颜色值
     *  
     *  @Param[in]:
     *      defaultClr:   默认情况下的颜色
     *      splashClr:    闪烁的颜色，但闪烁的颜色和默认颜色相同时，即可实现不闪烁
     */
    void setStatusColor(QRgb defaultClr, QRgb splashClr);

public slots:
    /*
     *  @Func:  showStatus
     *  @Desc:  显示状态栏上的运行状态
     *  
     *  @Param[in]:
     *      text:   状态内容
     *      timesS: 需要显示的时间  -1:一直显示，知道有新的状态到来
     */
    void showStatus(const QString& text, int timesS);

protected slots:
    void updateTime();

private:
    CBottomBarMgrPrivate *m_data;
};