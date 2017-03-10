#pragma once

#include "inc/control/mTablewidget.h"
#include "src/dataLevel/datamanager.h"


class CReportWidget : public MTableWidget
{
public:
    CReportWidget(QWidget* parent = 0);
    virtual ~CReportWidget();

    virtual void createTableWidget(int row, int column);

    void setCommonRowHeight(int rowH) {this->rowH = rowH;}
    void setCaptionWidth(int captionW) {this->captionW = captionW;}
    void setContentWidth(int contentW) {this->contentW = contentW;}

protected:
    int captionW;
    int contentW;
    int rowH;
};



class CUserInfoReport : public CReportWidget
{
public:
    CUserInfoReport(QWidget* parent = 0);
    virtual ~CUserInfoReport();

    enum {USER_INFO_ROW = 3};

    void updateCaptionColumn();
    void updateUserInfo(const DM_UserInfo &userInfo);
};

class CUserQueryResultReport : public CUserInfoReport {
public:
    CUserQueryResultReport(QWidget *parent = 0) : CUserInfoReport(parent) {}

    enum {QUERY_RESULT_ROW = 6};

    void updateCaptionColumn();
    void updateUserResultInfo(const DM_UserCurrentElectro &userInfo);
};

class CUserChargeReport : public CUserInfoReport {
public:
    CUserChargeReport(QWidget *parent = 0) : CUserInfoReport(parent) {}

    enum {ROWS = 6};

    void updateCaptionColumn();
    void updateChargedInfo(const DM_UserCurrentElectro &currentElectro, const DM_UserChargeInfo &chargeInfo);
    void updateChargeMemony(float money);
    void updateBalance(float balance);
};

class CUserNoticeReport : public CUserQueryResultReport
{
public:
    CUserNoticeReport(QWidget* parent = 0);
    virtual ~CUserNoticeReport();

    enum {USER_NOTICE_INFO_ROW = 2};

    void updateCaptionColumn();
    void updateUserNoticeInfo(const DM_UserNoticeInfo &userNoticeInfo);
};


/************************************************************************/
/* 以下为维护卡信息                                                     */
/************************************************************************/
// 终端信息
class CTerminalInfoReport : public CReportWidget
{
public:
    CTerminalInfoReport(QWidget* parent = 0);
    virtual ~CTerminalInfoReport();

    enum {TERMINAL_INFO_ROW = 2};

    void updateCaptionColumn();
    void updateTerminalInfo(const DM_TerminalInfo &terminalInfo);
};

// 用电检测
class CPowerMonitorReport : public CReportWidget
{
public:
    CPowerMonitorReport(QWidget* parent = 0);
    virtual ~CPowerMonitorReport();

    enum {POWER_MONITOR_ROW = 7};

    void updateCaptionColumn();
    void updatePowerMonitorInfo(const DM_PowerMonitorInfo &powerMonitorInfo);
};

// 基本信息
class CBaseInfoReport : public CReportWidget
{
public:
    CBaseInfoReport(QWidget* parent = 0);
    virtual ~CBaseInfoReport();

    enum {BASE_INFO_ROW = 5};

    void updateCaptionColumn();
    void updateBaseInfo(const DM_TerminalBaseInfo &baseInfo);
};

// 通讯参数
class CCommunicateInfoReport : public CReportWidget
{
public:
    CCommunicateInfoReport(QWidget *parent = 0);
    virtual ~CCommunicateInfoReport();

    enum {COMMUNICATE_INFO_ROW = 8};

    void updateCaptionColumn();
    void updateCommunicateInfo(const DM_CommunicateInfo &communicateInfo);
};

// 系统参数
class CSystemParamReport : public CReportWidget
{
public:
    CSystemParamReport(QWidget *parent = 0);
    virtual ~CSystemParamReport();

    enum {SYSTEM_PARAM_ROW = 4};

    void updateCaptionColumn();
};