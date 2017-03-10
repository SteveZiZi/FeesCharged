#include "userMainPage.h"

#include "inc/control/mTablewidget.h"
#include "inc/control/mPushButton.h"
#include "src/main/common.h"
#include "src/common_ctrl/reportWidget.h"

#include "src/dataLevel/datamanager.h"

#define TABLE_L         192
#define TABLE_T         144
#define TABLE_W         305
#define TABLE_H         106
#define TABLE_ITEM_H    35


CUserMainPage::CUserMainPage(QWidget* parent /*= 0*/, Qt::WindowFlags f /*= 0*/)
: CBaseContent(parent, f)
{
    m_dataMgr = CDataManager::getInstance();
    setContentCaption(tr("用户卡"));
    initUI();
    updateMenuInfo();

}

CUserMainPage::~CUserMainPage()
{

}

void CUserMainPage::initUI()
{
    CBaseContent::initUI();
    initMenu();
    m_userInfoWidget = new CUserInfoReport(this);
    m_userInfoWidget->setGeometry(TABLE_L, TABLE_T, TABLE_W, TABLE_H);
    m_userInfoWidget->setObjectName("tabelWidget_userInfo");
    m_userInfoWidget->createTableWidget(CUserInfoReport::USER_INFO_ROW, 2);
    m_userInfoWidget->updateCaptionColumn();
    updateUserInfo();
}

void CUserMainPage::initMenu()
{
    setMenuCaption(BTN_CHARGE, tr("充值"));
    setMenuCaption(BTN_QUERY, tr("查询"));
    setMenuCaption(BTN_REPLY, tr("复电"));
    setMenuCaption(BTN_NOTICE, tr("公告"));

    setMenuContentPage(BTN_CHARGE, USER_CHARGE_PAGE_ID);
    setMenuContentPage(BTN_QUERY, USER_QUERY_PAGE_ID);
    setMenuContentPage(BTN_REPLY, USER_REPAY);
    setMenuContentPage(BTN_NOTICE, USER_NOTICE_ID);
}

void CUserMainPage::updateUserInfo()
{
    DM_UserInfo userInfo;
    m_dataMgr->getUserInfo(userInfo);
    m_userInfoWidget->updateUserInfo(userInfo);
}