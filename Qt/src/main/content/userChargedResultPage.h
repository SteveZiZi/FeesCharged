#pragma once

#include "inc/baseContent.h"

class CUserChargeReport;
class CDataManager;
class CChargePage : public CBaseContent {
public:
    CChargePage(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~CChargePage();

    enum {
        BTN_CHARGE,
        BTN_EDIT,

        BTN_RETURN = 3
    };

    enum REPORT_TYPE {
        REPORT_NONE = -1,
        REPORT_CHARGE,
        REPORT_EDIT
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

private:
    CUserChargeReport* m_userChargeReport;
    CDataManager *m_dataMgr;
};