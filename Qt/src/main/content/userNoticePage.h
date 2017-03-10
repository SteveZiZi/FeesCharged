#pragma once

#include "inc/baseContent.h"

class CDataManager;
class CUserNoticeReport;
class CUserNoticePage : public CBaseContent
{
public:
    CUserNoticePage(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~CUserNoticePage(void);

    enum {
        BTN_BACK = 3
    };

    virtual void initUI();

private:
    void initMenu();
    void createNoticePage();
    void updateUserNoticeInfo();

private:
    CDataManager        *m_dataMgr;
    CUserNoticeReport   *m_pUserNoticeReport;
};
