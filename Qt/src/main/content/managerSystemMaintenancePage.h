/*
 *  @bref:
 *      系统维护界面
 */
#pragma once

#include "inc/baseContent.h"

class QLabel;
class CManagerSystemMaintenancePage : public CBaseContent
{
public:
    CManagerSystemMaintenancePage(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~CManagerSystemMaintenancePage(void);
    enum {
        BTN_VERIFY,
        BTN_CANCEL,
        BTN_TEMP,
        BTN_BACK = 3
    };

    virtual void initUI();

protected:
    /*
     *  @Func:  OnF1Press ~ OnF4Press
     *  @Desc:  响应对应菜单按下的事件
     *  
     *  @Param[in]:
     *      NONE
     *  
     *  @Return:
     *      true:   菜单项处理完成，即使设置了菜单对应的切换页面也不会显示页面
     *      false:  设置了菜单对应的切换页面将会显示该页面
     */
    virtual bool OnF1Press();

private:
    void initMenu();

private:
    QLabel*                 m_pMessageLabel;
};
