#pragma once

#include "inc/baseContent.h"

class CUserInfoReport;
class CDataManager;
class CUserQueryPage : public CBaseContent {
public:
    CUserQueryPage(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~CUserQueryPage();

    enum {
        BTN_CURRENT_ELECTRO,
        BTN_HISTORY_ELECTRO,
        BTN_CHARGE_RECORD,
        BTN_RETURN
    };

    virtual void initUI();

private:
    void initMenu();
    void updateUserInfo();

private:
    CUserInfoReport* m_userInfoWidget;
    CDataManager *m_dataMgr;
};