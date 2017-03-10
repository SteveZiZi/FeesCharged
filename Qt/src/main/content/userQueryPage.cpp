#include "userQueryPage.h"

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


CUserQueryPage::CUserQueryPage(QWidget* parent /*= 0*/, Qt::WindowFlags f /*= 0*/)
: CBaseContent(parent, f)
{
    m_dataMgr = CDataManager::getInstance();
    setContentCaption(tr("查询"));
    initUI();
    updateMenuInfo();
}

CUserQueryPage::~CUserQueryPage()
{

}

void CUserQueryPage::initUI()
{
    CBaseContent::initUI();

    initMenu();
    m_userInfoWidget = new CUserInfoReport(this);
    m_userInfoWidget->createTableWidget(CUserInfoReport::USER_INFO_ROW, 2);
    m_userInfoWidget->updateCaptionColumn();
    m_userInfoWidget->setGeometry(TABLE_L, TABLE_T, TABLE_W, TABLE_H);
    m_userInfoWidget->setObjectName("tabelWidget_userInfo");
    updateUserInfo();
}

void CUserQueryPage::initMenu()
{
    setMenuCaption(BTN_CURRENT_ELECTRO, tr("当前用电"));
    setMenuCaption(BTN_HISTORY_ELECTRO, tr("历史用电"));
    setMenuCaption(BTN_CHARGE_RECORD, tr("购电记录"));
    setMenuCaption(BTN_RETURN, tr("返回"));

    setMenuContentPage(BTN_CURRENT_ELECTRO, USER_QUERY_CURRENT_ELEC_ID);
    setMenuContentPage(BTN_HISTORY_ELECTRO, USER_QUERY_HISTORY_ELEC_ID);
    setMenuContentPage(BTN_CHARGE_RECORD, USER_ELEC_RECORD_ID);
    setMenuContentPage(BTN_RETURN, USER_MAIN_PAGE_ID);
}

void CUserQueryPage::updateUserInfo()
{
    DM_UserInfo userInfo;
    m_dataMgr->getUserInfo(userInfo);
    m_userInfoWidget->updateUserInfo(userInfo);
}