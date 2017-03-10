#include <QLabel>
#include "managerPowerMonitorPage.h"

#include "src/main/common.h"
#include "src/common_ctrl/reportWidget.h"

#define TABLE_L         128
#define TABLE_T         101
#define TABLE_W         424
#define TABLE_H         231
#define TABLE_ITEM_H    33
#define TABLE_CAPTION_W 224
#define TABLE_CONTENT_W 200

#define LABEL_L         TABLE_L+100
#define LABEL_T         TABLE_T-30

CManagerPowerMonitorPage::CManagerPowerMonitorPage(QWidget* parent, Qt::WindowFlags f)
: CBaseContent(parent,f)
, m_iCurrentIndex(0)
{
    m_pDataMgr = CDataManager::getInstance();
    setContentCaption(tr("用电检测"));
    initUI();
    initMenu();
}

CManagerPowerMonitorPage::~CManagerPowerMonitorPage(void)
{

}

void CManagerPowerMonitorPage::initUI()
{
    CBaseContent::initUI();
    createSerialLabel();
    createPowerMonitorReport();
}

bool CManagerPowerMonitorPage::OnF1Press()
{
    if (m_lPowerMonitorInfoList.size())
    {
        m_iCurrentIndex--;
        if (m_iCurrentIndex < 0)
        {
            m_iCurrentIndex = 0;
        }

        updateSerialLabel(m_iCurrentIndex);

        updatePowerMonitorReport(m_iCurrentIndex);
    }

    return true;
}

bool CManagerPowerMonitorPage::OnF2Press()
{
    if (m_lPowerMonitorInfoList.size())
    {
        m_iCurrentIndex++;
        if (m_iCurrentIndex >= m_lPowerMonitorInfoList.size())
        {
            m_iCurrentIndex = m_lPowerMonitorInfoList.size() - 1;
        }

        updateSerialLabel(m_iCurrentIndex);

        updatePowerMonitorReport(m_iCurrentIndex);
    }

    return true;
}

void CManagerPowerMonitorPage::initMenu()
{
    setMenuCaption(BTN_UP, tr("上翻"));
    setMenuCaption(BTN_DOWN, tr("下翻"));
    setMenuCaption(BTN_BACK, tr("返回"));

    setMenuContentPage(BTN_BACK, MANAGER_MAIN_PAGE_ID);
}

void CManagerPowerMonitorPage::createSerialLabel()
{
    m_pSerialLabel = new QLabel(this);
    m_pSerialLabel->setObjectName("label_serial");
    m_pSerialLabel->move(LABEL_L, LABEL_T);

    m_pDataMgr->getDeviceNum(m_lDeviceNumList);
    updateSerialLabel(m_iCurrentIndex);
}

void CManagerPowerMonitorPage::createPowerMonitorReport()
{
    m_pPowerMonitorReport = new CPowerMonitorReport(this);
    m_pPowerMonitorReport->setGeometry(TABLE_L, TABLE_T, TABLE_W, TABLE_H);
    m_pPowerMonitorReport->setObjectName("tabelWidget_powerMonitorReport");
    m_pPowerMonitorReport->setCaptionWidth(TABLE_CAPTION_W);
    m_pPowerMonitorReport->setContentWidth(TABLE_CONTENT_W);
    m_pPowerMonitorReport->setCommonRowHeight(TABLE_ITEM_H);
    m_pPowerMonitorReport->createTableWidget(CPowerMonitorReport::POWER_MONITOR_ROW, 2);
    m_pPowerMonitorReport->updateCaptionColumn();

    m_pDataMgr->getPowerMonitorInfo(m_lPowerMonitorInfoList);
    updatePowerMonitorReport(m_iCurrentIndex);
}

void CManagerPowerMonitorPage::updateSerialLabel(int index)
{
    if (index < 0)
    {
        return;
    }

    m_pSerialLabel->setText(tr("%1    表号:  %2").
                        arg(m_lDeviceNumList[index].number).
                        arg(m_lDeviceNumList[index].deviceNum));
}

void CManagerPowerMonitorPage::updatePowerMonitorReport(int index)
{
    if (index < 0)
    {
        return;
    }

    m_pPowerMonitorReport->updatePowerMonitorInfo(m_lPowerMonitorInfoList[index]);
}