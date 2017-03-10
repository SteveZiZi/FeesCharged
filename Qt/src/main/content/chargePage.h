#pragma once

#include "inc/baseContent.h"

class CUserChargeReport;
class CDataManager;
class CInputMoneyWidget;
class QLabel;
class CChargePage : public CBaseContent {
public:
    CChargePage(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~CChargePage();

    enum {
        BTN_EDIT,
        BTN_CHARGE,
        BTN_TEMP,
        BTN_RETURN = 3
    };

    enum REPORT_TYPE {
        REPORT_NONE = -1,
        REPORT_CHARGE,
        REPORT_EDIT
    };

    enum{
        BTN_SWITCH,
        BTN_SELECT,
        BTN_SET,
        BTN_CANCEL = 3
    };

    enum{
        BTN_SURE,
        BTN_NONE1,
        BTN_NONE2,
        BTN_CAL = 3
    };
    // 充值结果
    enum CHARGE_RESULT{
        VALUE_NONE = -1,
        VALUE_AVALIABLE,
        VALUE_UNAVALIABLE,
        CHARGE_SUCCESS,
        CHARGE_FALSE
    };
    // 当前显示页面类型
    enum CHARGE_WIDGET_TYPE {
        REPORT_WIDGET_TYPE,
        INPUT_WIDGET_TYPE,
        AFFIRM_WIDGET_TYPE
    };
    // 确认按钮来源
    enum AFFIRM_FROM {
        FROM_REPORT_WIDGET,
        FROM_INPUT_WIDGET
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
    virtual bool OnF4Press();

private:
    bool chargeResult();                                                // 充值结果 true为成功 false为失败

    void showReportWidget();                                            // 显示充值表格页面信息
    void showInputWidget();                                             // 显示输入金额页面信息
    void showAffirmWidget();                                            // 显示确认和提示页面信息

    void createReportWidget();
    void createInputWidget();
    void createAffirmWidget();

    void updateReportMenu();
    void updateInputMenu();
    void updateAffirmMenu();


private:
    CUserChargeReport       *m_userChargeReport;
    CDataManager            *m_dataMgr;
    CInputMoneyWidget       *m_pInputMoneyWidget;
    QLabel                  *m_pMessageLabel;                           // 用于提示和确认信息的Label

    REPORT_TYPE             m_eReportType;
    CHARGE_WIDGET_TYPE      m_eWidgetType;
    CHARGE_RESULT           m_eChargeResult;
    AFFIRM_FROM             m_eAffirmFrom;
};