/*
 *  @bref:
 *      终端信息界面
 */
#pragma once

#include "inc/baseContent.h"
#include "src/dataLevel/datamanager.h"

class MTableWidget;
class CTerminalInfoReport;
class CBaseInfoReport;
class CCommunicateInfoReport;
class CDataManager;
class CManagerTerminalInfoPage : public CBaseContent
{
public:
    CManagerTerminalInfoPage(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~CManagerTerminalInfoPage(void);
    enum {
        BTN_BASE_INFO,
        BTN_USER_FILES,
        BTN_COMMUNICATE_PARAM,
        BTN_BACK = 3
    };

    enum REPORT_TYOE {
        TERMINAL_INFO_REPORT,
        BASE_INFO_RPORT,
        USER_FILE_INFO_REPORT,
        COMMUNICATE_INFO_REPORT
    };

    enum {
        BTN_UP,
        BTN_DOWN,
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
    void createBaseInfoReport();
    void createUserFileInfoReport();
    void createCommunicateInfoReport();

    void showTerminalInfoReport();
    void showBaseInfoReport();
    void showUserFileInfoReport();
    void showCommunicateInfoReport();

    void updateUserFileInfoReport();

    void updateTerminalInfoMenu();
    void updateUserFileMenu();

    void clearUserFileInfoReport();

private:
    CTerminalInfoReport*        m_pTerminalInfoWidget;
    CBaseInfoReport*            m_pBaseInfoReport;
    CCommunicateInfoReport*     m_pCommunicateInfoReport;
    MTableWidget*               m_pUserFileInfoReport;

    CDataManager*               m_pDataMgr;
    QList<DM_UserFileInfo>      m_lUserFileInfoList;

    REPORT_TYOE                 m_eReportType;
    int                         m_iUserFilePageIndex;
};
