#include <QLabel>
#include <QVBoxLayout>

#include "userNoticePage.h"
#include "src/main/common.h"
#include "src/common_ctrl/reportWidget.h"

#include "src/dataLevel/datamanager.h"

#define TABLE_L         192
#define TABLE_T         89
#define TABLE_W         305
#define TABLE_H         281

#define LABEL_L         TABLE_L+115
#define LABEL_T         TABLE_T-30

CUserNoticePage::CUserNoticePage(QWidget* parent, Qt::WindowFlags f)
: CBaseContent(parent,f)
{
    m_dataMgr = CDataManager::getInstance();
    initUI();
}

CUserNoticePage::~CUserNoticePage(void)
{

}

void CUserNoticePage::initUI()
{
    CBaseContent::initUI();

    createNoticePage();
    initMenu();
}

void CUserNoticePage::initMenu()
{
    setMenuCaption(BTN_BACK, tr("·µ»Ø"));

    setMenuContentPage(BTN_BACK, USER_MAIN_PAGE_ID);
}

void CUserNoticePage::createNoticePage()
{
    m_pUserNoticeReport = new CUserNoticeReport(this);
    m_pUserNoticeReport->setGeometry(TABLE_L, TABLE_T, TABLE_W, TABLE_H);
    m_pUserNoticeReport->setObjectName("tabelWidget_userNoticeReport");
    m_pUserNoticeReport->createTableWidget(CUserQueryResultReport::QUERY_RESULT_ROW + CUserNoticeReport::USER_NOTICE_INFO_ROW, 2);
    m_pUserNoticeReport->updateCaptionColumn();
    updateUserNoticeInfo();

    QLabel* headLineLabel = new QLabel(this);
    headLineLabel->setText(tr("¹« ¸æ"));
    headLineLabel->setObjectName("label_noticeHeadLine");
    headLineLabel->move(LABEL_L, LABEL_T);
}

void CUserNoticePage::updateUserNoticeInfo()
{
    DM_UserInfo userInfo;
    m_dataMgr->getUserInfo(userInfo);
    m_pUserNoticeReport->updateUserInfo(userInfo);

    DM_UserCurrentElectro curElec;
    m_dataMgr->getUserCurrentElectro(curElec);
    m_pUserNoticeReport->updateUserResultInfo(curElec);

    DM_UserNoticeInfo userNoticeInfo;
    m_dataMgr->getUserNoticeInfo(userNoticeInfo);
    m_pUserNoticeReport->updateUserNoticeInfo(userNoticeInfo);
}
