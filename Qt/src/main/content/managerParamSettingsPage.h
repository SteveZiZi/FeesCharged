/*
 *  @bref:
 *      参数设置界面 + 测试通道
 */
#pragma once

#include "inc/baseContent.h"

class CTerminalInfoReport;
class CDataManager;
class MTableWidget;
class CManagerParamSettingsPage : public CBaseContent
{
public:
    CManagerParamSettingsPage(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~CManagerParamSettingsPage(void);
    enum {
        BTN_SYSTEM_PARAM,
        BTN_TEST_CHANNEL,
        BTN_SYSTEM_MAINTAIN,
        BTN_BACK = 3
    };

    enum REPORT_TYPE{
        TERMINAL_INFO_REPORT,
        TEST_CHANNEL_REPORT
    };

    enum {
        BTN_START_TEST,
        BTN_STOP_TEST,
        BTN_TEMP,
        BTN_RETURN = 3
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
    void initMenu();

    void createTerminalInfoReport();
    void createTestChannelReport();

    void showTerminalInfoReport();
    void showTestChannelReport();

    void updateTerminalInfoMenu();
    void updateTestChannelMenu();

    void updateTestChannelReport();

    void clearTestChannelReport();

private:
    CTerminalInfoReport*        m_pTerminalInfoWidget;
    CDataManager*               m_pDataMgr;
    MTableWidget*               m_pTestChannelReport;

    REPORT_TYPE                 m_eReportType;
};
