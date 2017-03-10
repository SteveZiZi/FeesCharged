/*
 *  @bref:
 *      管理维护界面
 */
#pragma once

#include "inc/baseContent.h"

class CTerminalInfoReport;
class CDataManager;
class CManagerAndMaintainPage : public CBaseContent
{
public:
    CManagerAndMaintainPage(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~CManagerAndMaintainPage(void);

    enum {
        BTN_INFO = 0,
        BTN_PARAM_SET,
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
