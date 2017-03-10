#pragma once

#include "inc/baseContent.h"
#include "src/dataLevel/datamanager.h"
#include <QLabel>

class CUserInfoWidget : public QWidget
{
public:
    CUserInfoWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~CUserInfoWidget();

    void initUI();
    void updateUserInfo(const DM_UserInfo &userInfo);

private:
    QLabel m_labelName;
    QLabel m_labelHouseholdNum;
    QLabel m_labelHouseNum;
};

class CUserInfoReport;
class CDataManager;
class QGroupBox;
class MTableWidget;
class CUserChargeRecordPage : public CBaseContent {
public:
    CUserChargeRecordPage(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~CUserChargeRecordPage();

    enum {
        BTN_RECORD_CHARGED,
        BTN_RECORD_REPLY,
        BTN_POWER_CUT,
        BTN_RETURN
    };

    enum REPORT_TYPE {
        REPORT_NONE = -1,
        REPORT_CHARGED,
        REPORT_REPLY,
        REPORT_POWER_CUT
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

private:
    void initMenu();
    void updateUserInfo();
    void userInfoWidgetMove();

    void showChargeRecord();
    void showReplyRecord();
    void showPowerCutRecord();

    void createCharegeRecord();
    void createReplyRecord();
    void createPowerCutRecord();

    void clearChargeRecord();
    void clearReplyRecord();
    void clearPowerCutRecord();

    void updateChargeRecord();
    void updateReplyRecord();
    void updatePowderCutRecord();

private:
    CUserInfoReport* m_userInfoReport;
    CDataManager *m_dataMgr;

    CUserInfoWidget *m_userInfoWidget;

    REPORT_TYPE      m_eReportType;
    QLabel          *m_labelCaption;
    QGroupBox       *m_groupbox;
    MTableWidget    *m_reportCharge;
    MTableWidget    *m_reportReply;
    MTableWidget    *m_reportPowerCut;
};