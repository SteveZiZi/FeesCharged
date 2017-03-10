#include "chargePage.h"
#include <QKeyEvent>
#include <QLabel>

#include "inc/control/mTablewidget.h"
#include "inc/control/mPushButton.h"
#include "src/main/common.h"
#include "src/common_ctrl/reportWidget.h"
#include "src/common_ctrl/inputMoneyWidget.h"

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

#define INPUT_WIDGET_L  200
#define INPUT_WIDGET_T  100
#define INPUT_WIDGET_W  280
#define INPUT_WIDGET_H  270

#define AFFIRM_WIDGET_L 200
#define AFFIRM_WIDGET_T 100
#define AFFIRM_WIDGET_W 280
#define AFFIRM_WIDGET_H 270

CChargePage::CChargePage(QWidget* parent /*= 0*/, Qt::WindowFlags f /*= 0*/)
: CBaseContent(parent, f)
, m_pInputMoneyWidget(NULL)
, m_pMessageLabel(NULL)
, m_eReportType(REPORT_CHARGE)
, m_eWidgetType(REPORT_WIDGET_TYPE)
, m_eChargeResult(VALUE_AVALIABLE)
, m_eAffirmFrom(FROM_REPORT_WIDGET)
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
    updateReportMenu();
    createReportWidget();

    QLabel* resultLabel = new QLabel(tr("充值"), this);
    resultLabel->setObjectName("label_userCharge");
    resultLabel->move(LABEL_L, LABEL_T);
}

bool CChargePage::OnF1Press()
{
    if (REPORT_WIDGET_TYPE == m_eWidgetType)
    {
        showInputWidget();
    }
    else if(INPUT_WIDGET_TYPE == m_eWidgetType)
    {
        m_pInputMoneyWidget->nextPosition();
    }
    else if (AFFIRM_WIDGET_TYPE == m_eWidgetType)
    {
        if(VALUE_AVALIABLE == m_eChargeResult)
        {
            if (chargeResult())
            {
                m_eChargeResult = CHARGE_SUCCESS;
            }
            else 
            {
                m_eChargeResult = CHARGE_FALSE;
                
            }
            showAffirmWidget();
        }
        else if(VALUE_UNAVALIABLE == m_eChargeResult)
        {
            showInputWidget();

            m_eChargeResult = VALUE_AVALIABLE;
        }
        else if (CHARGE_SUCCESS == m_eChargeResult)
        {
            showReportWidget();
        }
        else if (CHARGE_FALSE == m_eChargeResult)
        {
            showReportWidget();
        }
    }
    return true;
}

bool CChargePage::OnF2Press()
{
    if (REPORT_WIDGET_TYPE == m_eWidgetType)
    {
        showAffirmWidget();
    }
    else if(INPUT_WIDGET_TYPE == m_eWidgetType)
    {
        m_pInputMoneyWidget->selectedContent();
    }
    else if (AFFIRM_WIDGET_TYPE == m_eWidgetType)
    {

    }
    
    return true;
}

bool CChargePage::OnF3Press()
{
    if (REPORT_WIDGET_TYPE == m_eWidgetType)
    {
        return false;
    }
    else if (INPUT_WIDGET_TYPE == m_eWidgetType)
    {
        if (m_pInputMoneyWidget->inputMoney().toFloat() > 0 &&
            m_pInputMoneyWidget->inputMoney().toFloat() <= MAX_MONEY_VALUE)
        {
            m_eChargeResult = VALUE_AVALIABLE;

            m_userChargeReport->item(5,1)->setText(m_pInputMoneyWidget->inputMoney());
            m_pInputMoneyWidget->clearLastValue();
            showReportWidget();
        }
        else
        {
            m_eChargeResult = VALUE_UNAVALIABLE;

            m_userChargeReport->item(5,1)->setText("100.00");
            m_pInputMoneyWidget->clearLastValue();
            showAffirmWidget();
        }
    }
    else if (AFFIRM_WIDGET_TYPE == m_eWidgetType)
    {

    }

    return true;
}

bool CChargePage::OnF4Press()
{
    if (REPORT_WIDGET_TYPE == m_eWidgetType)
    {
        return false;
    }
    else if (INPUT_WIDGET_TYPE == m_eWidgetType)
    {
        m_pInputMoneyWidget->clearLastValue();
        showReportWidget();
    }
    else if (AFFIRM_WIDGET_TYPE == m_eWidgetType)
    {
        if (VALUE_AVALIABLE == m_eChargeResult)
        {
            if (FROM_REPORT_WIDGET == m_eAffirmFrom)
            {
                showReportWidget();
            }
            else if (FROM_INPUT_WIDGET == m_eAffirmFrom)
            {
                showInputWidget();
            }
        }
        else if (CHARGE_SUCCESS == m_eChargeResult)
        {
            return false;
        }
        else if (CHARGE_FALSE == m_eChargeResult)
        {
            return false;
        }
    }

    return true;
}

bool CChargePage::chargeResult()
{
    if (m_eReportType == REPORT_CHARGE) {
        DM_UserChargeInfo chargeInfo;
        chargeInfo.chargeMeterNo = m_userChargeReport->item(3,1)->text();
        chargeInfo.money = m_userChargeReport->item(5,1)->text().toFloat();
        if (m_dataMgr->userCharge(chargeInfo)) {
            DM_UserCurrentElectro curElec;
            m_dataMgr->getUserCurrentElectro(curElec);
            m_userChargeReport->updateBalance(curElec.balance);
        }
        else{
            return false;
        }
    }

    return true;
}

void CChargePage::showReportWidget()
{
    if (m_pInputMoneyWidget)
    {
        m_pInputMoneyWidget->hide();
    }
    
    if(m_pMessageLabel)
    {
        m_pMessageLabel->hide();
    }

    if(!m_userChargeReport)
    {
        createReportWidget();
    }

    updateReportMenu();
    m_userChargeReport->show();
    m_eWidgetType = REPORT_WIDGET_TYPE;
    m_eAffirmFrom = FROM_REPORT_WIDGET;
}

void CChargePage::showInputWidget()
{
    m_userChargeReport->hide();
    if(m_pMessageLabel)
    {
        m_pMessageLabel->hide();
    }

    if (!m_pInputMoneyWidget) {
        createInputWidget();
    }

    updateInputMenu();
    m_pInputMoneyWidget->show();
    m_eWidgetType = INPUT_WIDGET_TYPE;
    m_eAffirmFrom = FROM_INPUT_WIDGET;
}

void CChargePage::showAffirmWidget()
{
    if (m_pInputMoneyWidget)
    {
        m_pInputMoneyWidget->hide();
    }

    m_userChargeReport->hide();

    if(!m_pMessageLabel)
    {
        createAffirmWidget();
    }

    updateAffirmMenu();
    m_pMessageLabel->show();
    m_eWidgetType = AFFIRM_WIDGET_TYPE;
}

void CChargePage::createReportWidget()
{
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
}

void CChargePage::createInputWidget()
{
    m_pInputMoneyWidget = new CInputMoneyWidget(this);
    m_pInputMoneyWidget->setGeometry(INPUT_WIDGET_L, INPUT_WIDGET_T, INPUT_WIDGET_W, INPUT_WIDGET_H);
}

void CChargePage::createAffirmWidget()
{
    m_pMessageLabel = new QLabel(this);

    m_pMessageLabel->setGeometry(AFFIRM_WIDGET_L, AFFIRM_WIDGET_T, AFFIRM_WIDGET_W, AFFIRM_WIDGET_H);
}

void CChargePage::updateReportMenu()
{
    setMenuCaption(BTN_EDIT, tr("选择金额"));
    setMenuCaption(BTN_CHARGE, tr("确认充值"));
    setMenuCaption(BTN_TEMP, tr(""));
    setMenuCaption(BTN_RETURN, tr("返回"));

    setMenuContentPage(BTN_RETURN, USER_MAIN_PAGE_ID);
}

void CChargePage::updateInputMenu()
{
    setMenuCaption(BTN_SWITCH, tr("切换"));
    setMenuCaption(BTN_SELECT, tr("选择"));
    setMenuCaption(BTN_SET, tr("确认"));
    setMenuCaption(BTN_CANCEL, tr("返回"));
}

void CChargePage::updateAffirmMenu()
{
    switch(m_eChargeResult)
    {
    case VALUE_NONE:
        break;
    case VALUE_AVALIABLE:                                               // 输入金额后进行确认
        {
            QString money = QString::number(m_userChargeReport->item(5,1)->text().toFloat(),'f',2);
            m_pMessageLabel->setText(tr("确认充值") + money + tr("元?"));

            m_pMessageLabel->setStyleSheet("color:yellow");

            setMenuCaption(BTN_SURE, tr("确认"));
            setMenuCaption(BTN_NONE1, tr(""));
            setMenuCaption(BTN_NONE2, tr(""));
            setMenuCaption(BTN_CAL, tr("取消"));
        }
        break;
    case VALUE_UNAVALIABLE:                                             // 输入金额不在允许范围内
        m_pMessageLabel->setText(tr("输入的金额不在允许范围内,\n请重新输入!"));

        m_pMessageLabel->setStyleSheet("color:red");

        setMenuCaption(BTN_SURE, tr("确认"));
        setMenuCaption(BTN_NONE1, tr(""));
        setMenuCaption(BTN_NONE2, tr(""));
        setMenuCaption(BTN_CAL, tr(""));
        break;
    case CHARGE_SUCCESS:                                                // 充值成功
        m_pMessageLabel->setText(tr("充值成功!"));

        m_pMessageLabel->setStyleSheet("color:green");

        setMenuCaption(BTN_SURE, tr("继续充值"));
        setMenuCaption(BTN_NONE1, tr(""));
        setMenuCaption(BTN_NONE2, tr(""));
        setMenuCaption(BTN_CAL, tr("返回"));

        setMenuContentPage(BTN_CAL, USER_MAIN_PAGE_ID);
        break;
    case CHARGE_FALSE:                                                  // 充值失败
        m_pMessageLabel->setText(tr("充值失败!"));

        m_pMessageLabel->setStyleSheet("color:red");

        setMenuCaption(BTN_SURE, tr("重新充值"));
        setMenuCaption(BTN_NONE1, tr(""));
        setMenuCaption(BTN_NONE2, tr(""));
        setMenuCaption(BTN_CAL, tr("返回"));

        setMenuContentPage(BTN_CAL, USER_MAIN_PAGE_ID);
        break;
    default:
        break;
    }
}