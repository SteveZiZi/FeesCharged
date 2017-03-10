#include "chargePage.h"
#include <QKeyEvent>
#include <QLabel>

#include "inc/control/mTablewidget.h"
#include "inc/control/mPushButton.h"
#include "src/main/common.h"
#include "src/common_ctrl/reportWidget.h"

#include "src/dataLevel/datamanager.h"

#define TABLE_L         144
#define TABLE_T         100
#define TABLE_W         393
#define TABLE_H         210
#define TABLE_ITEM_H    35

#define CAPTION_W       192
#define CONTENT_W       200

#define LABEL_L         TABLE_L+100
#define LABEL_T         TABLE_T-30



CChargePage::CChargePage(QWidget* parent /*= 0*/, Qt::WindowFlags f /*= 0*/)
: CBaseContent(parent, f)
{
    m_dataMgr = CDataManager::getInstance();
    setContentCaption(tr("充值"));
    initUI();
    updateMenuInfo();
}

CChargePage::~CChargePage()
{

}

void CChargePage::initUI()
{
    CBaseContent::initUI();
    initMenu();

    m_userChargeReport = new CUserChargeReport(this);
    m_userChargeReport->setGeometry(TABLE_L, TABLE_T, TABLE_W, TABLE_H);
    m_userChargeReport->setObjectName("tabelWidget_userChargeReport");
    m_userChargeReport->createTableWidget(CUserChargeReport::ROWS, 2);
    m_userChargeReport->setColumnWidth(0, CAPTION_W);
    m_userChargeReport->setColumnWidth(1, CONTENT_W);
    m_userChargeReport->updateCaptionColumn();

    DM_UserInfo userInfo;
    m_dataMgr->getUserInfo(userInfo);
    m_userChargeReport->updateUserInfo(userInfo);

    DM_UserCurrentElectro curElec;
    m_dataMgr->getUserCurrentElectro(curElec);
    DM_UserChargeInfo chargeInfo;
    chargeInfo.chargeMeterNo = "152366";
    chargeInfo.money = 100.0f;
    m_userChargeReport->updateChargedInfo(curElec, chargeInfo);


    QLabel* resultLabel = new QLabel(tr("充值"), this);
    resultLabel->setObjectName("label_userCharge");
    resultLabel->move(LABEL_L, LABEL_T);
}

bool CChargePage::OnF1Press()
{

    return true;
}

bool CChargePage::OnF2Press()
{

    return true;
}

bool CChargePage::OnF3Press()
{

    return true;
}

void CChargePage::initMenu()
{
    setMenuCaption(BTN_CHARGE, tr("确认充值"));
    setMenuCaption(BTN_EDIT,   tr("输入金额"));
    setMenuCaption(BTN_RETURN, tr("返回"));

    setMenuContentPage(BTN_RETURN, USER_MAIN_PAGE_ID);
}