#pragma once

#include "inc/baseContent.h"

class CUserInfoReport;
class CDataManager;
class CUserRepayPage : public CBaseContent {
public:
    CUserRepayPage(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~CUserRepayPage();

    enum {
        BTN_RETURN =3
    };

    virtual void initUI();

private:
    void initMenu();
    void updateUserInfo();

private:
    CUserInfoReport* m_userInfoReport;
    CDataManager *m_dataMgr;
};