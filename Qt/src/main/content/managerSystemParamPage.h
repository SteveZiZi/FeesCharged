/*
 *  @bref:
 *      系统参数界面
 */
#pragma once

#include "inc/baseContent.h"

class CSystemParamReport;
class CDataManager;
class CManagerSystemParamPage : public CBaseContent
{
public:
    CManagerSystemParamPage(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~CManagerSystemParamPage(void);

    enum {
        BTN_SWITCH,
        BTN_REDUCE,
        BTN_INCREACE,
        BTN_BACK = 3
    };

    enum SELECTED_TYPE{
        BACKLIGHT,
        LOOPTIME,
        BACK_LIGHT_START,
        BACK_LIGHT_END
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
    virtual bool OnF2Press();
    virtual bool OnF3Press();

private:
    void initMenu();
    void updateSystemParamReport();

private:
    CSystemParamReport*         m_pSystemParamReport;
    CDataManager*               m_pDataMgr;

    SELECTED_TYPE               m_eSelectedType;
};
