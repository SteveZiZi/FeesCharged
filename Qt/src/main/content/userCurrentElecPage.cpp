#include "userCurrentElecPage.h"
#include <QLabel>

#include "inc/control/mTablewidget.h"
#include "inc/control/mPushButton.h"
#include "src/main/common.h"
#include "src/common_ctrl/reportWidget.h"

#include "src/dataLevel/datamanager.h"


#define TABLE_L         192
#define TABLE_T         89
#define TABLE_W         305
#define TABLE_H         211

#define LABEL_L         TABLE_L+100
#define LABEL_T         TABLE_T-30


CUserCurrentElecPage::CUserCurrentElecPage(QWidget* parent /*= 0*/, Qt::WindowFlags f /*= 0*/)
: CBaseContent(parent, f)
{
    m_dataMgr = CDataManager::getInstance();
    setContentCaption(tr("查询->当前用电"));
    initUI();
    updateMenuInfo();
}

CUserCurrentElecPage::~CUserCurrentElecPage()
{

}

void CUserCurrentElecPage::initUI()
{
    CBaseContent::initUI();

    initMenu();
    m_userResultWidget = new CUserQueryResultReport(this);
    m_userResultWidget->setGeometry(TABLE_L, TABLE_T, TABLE_W, TABLE_H);
    m_userResultWidget->setObjectName("tabelWidget_userQueryResultInfo");
    m_userResultWidget->createTableWidget(CUserQueryResultReport::QUERY_RESULT_ROW, 2);
    m_userResultWidget->updateCaptionColumn();
    updateUserInfo();

    QLabel* resultLabel = new QLabel(tr("查询结果"), this);
    resultLabel->setObjectName("label_queryResult");
    resultLabel->move(LABEL_L, LABEL_T);
}

void CUserCurrentElecPage::initMenu()
{
    setMenuCaption(BTN_RETURN, tr("返回"));
    setMenuContentPage(BTN_RETURN, USER_QUERY_PAGE_ID);
}

void CUserCurrentElecPage::updateUserInfo()
{
    DM_UserInfo userInfo;
    m_dataMgr->getUserInfo(userInfo);
    m_userResultWidget->updateUserInfo(userInfo);

    DM_UserCurrentElectro curElec;
    m_dataMgr->getUserCurrentElectro(curElec);
    m_userResultWidget->updateUserResultInfo(curElec);
}