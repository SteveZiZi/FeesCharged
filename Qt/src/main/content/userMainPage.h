#pragma once

#include "inc/baseContent.h"

class CUserInfoReport;
class CDataManager;
class CUserMainPage : public CBaseContent {
public:
    CUserMainPage(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~CUserMainPage();

    enum {
        BTN_CHARGE,
        BTN_QUERY,
        BTN_REPLY,
        BTN_NOTICE
    };

    virtual void initUI();

private:
    void initMenu();

    void updateUserInfo();

private:
    CUserInfoReport* m_userInfoWidget;
    CDataManager *m_dataMgr;
};