#include "userRepay.h"
#include <QLabel>

#include "inc/control/mTablewidget.h"
#include "inc/control/mPushButton.h"
#include "src/main/common.h"
#include "src/common_ctrl/reportWidget.h"

#include "src/dataLevel/datamanager.h"

#define TABLE_L         192
#define TABLE_T         144
#define TABLE_W         305
#define TABLE_H         106

#define LABEL_L         TABLE_L+100
#define LABEL_T         TABLE_T-30


CUserRepayPage::CUserRepayPage(QWidget* parent /*= 0*/, Qt::WindowFlags f /*= 0*/)
: CBaseContent(parent, f)
{
    m_dataMgr = CDataManager::getInstance();
    setContentCaption(tr("复电"));
    initUI();
    updateMenuInfo();
}

CUserRepayPage::~CUserRepayPage()
{

}

void CUserRepayPage::initUI()
{
    CBaseContent::initUI();

    initMenu();
    m_userInfoReport = new CUserInfoReport(this);
    m_userInfoReport->setGeometry(TABLE_L, TABLE_T, TABLE_W, TABLE_H);
    m_userInfoReport->setObjectName("tabelWidget_userInfo");
    m_userInfoReport->createTableWidget(CUserInfoReport::USER_INFO_ROW, 2);
    m_userInfoReport->updateCaptionColumn();
    updateUserInfo();

    QLabel* resultLabel = new QLabel(tr("复电结果"), this);
    resultLabel->setObjectName("label_repayResult");
    resultLabel->move(LABEL_L, LABEL_T);
}

void CUserRepayPage::initMenu()
{
    setMenuCaption(BTN_RETURN, tr("返回"));
    setMenuContentPage(BTN_RETURN, USER_MAIN_PAGE_ID);
}

void CUserRepayPage::updateUserInfo()
{
    DM_UserInfo userInfo;
    m_dataMgr->getUserInfo(userInfo);
    m_userInfoReport->updateUserInfo(userInfo);
}