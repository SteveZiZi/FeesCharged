/*
 *  @bref:
 *      维护卡主界面
 */
#pragma once

#include "inc/baseContent.h"

class CTerminalInfoReport;
class CDataManager;
class CManagerMainPage : public CBaseContent
{
public:
    CManagerMainPage(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~CManagerMainPage(void);
    enum {
        BTN_MANAGER = 0,
        BTN_MONITOR,
        BTN_BACK = 3
    };

    virtual void initUI();

private:
    void initMenu();

    void updateTerminalInfo();

private:
    CTerminalInfoReport*        m_pTerminalInfoWidget;
    CDataManager*               m_pDataMgr;
};
