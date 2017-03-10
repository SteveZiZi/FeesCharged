#pragma once

#include "inc/baseContent.h"

class CUserQueryResultReport;
class CDataManager;
class CUserCurrentElecPage : public CBaseContent {
public:
    CUserCurrentElecPage(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~CUserCurrentElecPage();

    enum {
        BTN_RETURN =3
    };

    virtual void initUI();

private:
    void initMenu();
    void updateUserInfo();

private:
    CUserQueryResultReport* m_userResultWidget;
    CDataManager *m_dataMgr;
};