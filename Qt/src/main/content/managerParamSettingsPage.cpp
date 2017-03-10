#include <QtGui/QScrollBar>
#include <QtGui/QHeaderView>
#include "managerParamSettingsPage.h"

#include "src/main/common.h"
#include "src/common_ctrl/reportWidget.h"

#include "src/dataLevel/datamanager.h"

#define TABLE_L         176
#define TABLE_T         144
#define TABLE_W         336
#define TABLE_H         71
#define TABLE_ITEM_H    35
#define TABLE_CAPTION_W 168
#define TABLE_CONTENT_W 168

#define TEST_CHANNEL_L         144
#define TEST_CHANNEL_T         24
#define TEST_CHANNEL_W         400
#define TEST_CHANNEL_H         352
#define TEST_CHANNEL_ITEM_H    32
#define TEST_CHANNEL_ONE_W     64
#define TEST_CHANNEL_TWO_W     176
#define TEST_CHANNEL_THREE_W   160
#define TEST_CHANNEL_ROW_COUNT 10
#define TEST_CHANNEL_COLUMN_COUNT 3


CManagerParamSettingsPage::CManagerParamSettingsPage(QWidget* parent, Qt::WindowFlags f)
: CBaseContent(parent,f)
, m_pTestChannelReport(NULL)
, m_eReportType(TERMINAL_INFO_REPORT)
{
    m_pDataMgr = CDataManager::getInstance();
    setContentCaption(tr("管理维护->参数设置"));
    initUI();
    initMenu();
}

CManagerParamSettingsPage::~CManagerParamSettingsPage(void)
{

}

void CManagerParamSettingsPage::initUI()
{
    CBaseContent::initUI();
    createTerminalInfoReport();
}

bool CManagerParamSettingsPage::OnF1Press()
{

    return false;
}

bool CManagerParamSettingsPage::OnF2Press()
{
    if (TERMINAL_INFO_REPORT == m_eReportType)
    {
        showTestChannelReport();
        return true;
    }

    return false;
}

bool CManagerParamSettingsPage::OnF3Press()
{

    return false;
}

bool CManagerParamSettingsPage::OnF4Press()
{

    return false;
}

void CManagerParamSettingsPage::initMenu()
{
    setMenuCaption(BTN_SYSTEM_PARAM, tr("系统参数"));
    setMenuCaption(BTN_TEST_CHANNEL, tr("测试通道"));
    setMenuCaption(BTN_SYSTEM_MAINTAIN, tr("系统维护"));
    setMenuCaption(BTN_BACK, tr("返回"));

    setMenuContentPage(BTN_SYSTEM_PARAM, MANAGER_SYSTEM_PARAM_ID);
    setMenuContentPage(BTN_SYSTEM_MAINTAIN, MANAGER_SYSTEM_MAINTENACE_ID);
    setMenuContentPage(BTN_BACK, MANAGER_MAINTAIN_PAGE_ID);
}

void CManagerParamSettingsPage::createTerminalInfoReport()
{
    m_pTerminalInfoWidget = new CTerminalInfoReport(this);
    m_pTerminalInfoWidget->setGeometry(TABLE_L, TABLE_T, TABLE_W, TABLE_H);
    m_pTerminalInfoWidget->setCaptionWidth(TABLE_CAPTION_W);
    m_pTerminalInfoWidget->setContentWidth(TABLE_CONTENT_W);
    m_pTerminalInfoWidget->setObjectName("tabelWidget_terminalInfo");
    m_pTerminalInfoWidget->createTableWidget(CTerminalInfoReport::TERMINAL_INFO_ROW, 2);
    m_pTerminalInfoWidget->updateCaptionColumn();
    // update
    DM_TerminalInfo terminalInfo;
    m_pDataMgr->getTerminalInfo(terminalInfo);
    m_pTerminalInfoWidget->updateTerminalInfo(terminalInfo);
}

void CManagerParamSettingsPage::createTestChannelReport()
{
    const QString headerCaption[5] = {
        tr("序号"),
        tr("表号"),
        tr("485通讯状况")
    };

    m_pTestChannelReport = new MTableWidget(TEST_CHANNEL_ROW_COUNT,TEST_CHANNEL_COLUMN_COUNT,this);
    m_pTestChannelReport->setGeometry(TEST_CHANNEL_L, TEST_CHANNEL_T, 
                                      TEST_CHANNEL_W, TEST_CHANNEL_H);

    m_pTestChannelReport->verticalHeader()->hide();
    m_pTestChannelReport->horizontalScrollBar()->hide();
    m_pTestChannelReport->verticalScrollBar()->hide();

    m_pTestChannelReport->setShowGrid(false);
    m_pTestChannelReport->setGridStyle(new MGridStyle(QColor(TABLE_LINE_CLR), true, true));
    m_pTestChannelReport->setShowTopBottomLine(true, true);
    m_pTestChannelReport->setShowLeftRightLine(true, true);
    m_pTestChannelReport->setAlternatingRowColors(false);

    m_pTestChannelReport->horizontalHeader()->setFixedHeight(TEST_CHANNEL_ITEM_H);

    for(int i = 0; i < m_pTestChannelReport->columnCount(); i++) {
        QTableWidgetItem *headerItem = new QTableWidgetItem(headerCaption[i]);
        m_pTestChannelReport->setHorizontalHeaderItem(i, headerItem);
        headerItem->setTextAlignment(Qt::AlignCenter);
    }

    m_pTestChannelReport->setColumnWidth(0, TEST_CHANNEL_ONE_W);
    m_pTestChannelReport->setColumnWidth(1, TEST_CHANNEL_TWO_W);
    m_pTestChannelReport->setColumnWidth(2, TEST_CHANNEL_THREE_W);

    for (int row = 0; row < m_pTestChannelReport->rowCount(); ++row) {
        for (int column = 0; column < m_pTestChannelReport->columnCount(); ++column) {
            QTableWidgetItem *newItem = new QTableWidgetItem();
            if (column == 0) {
                newItem->setFlags(Qt::ItemIsSelectable);
                newItem->setTextAlignment(Qt::AlignCenter);
            }
            else {
                newItem->setTextAlignment(Qt::AlignCenter);
            }
            m_pTestChannelReport->setItem(row, column, newItem);
        }
        m_pTestChannelReport->setRowHeight(row, TEST_CHANNEL_ITEM_H);
    }
}

void CManagerParamSettingsPage::showTerminalInfoReport()
{
    if (m_pTestChannelReport)
    {
        m_pTestChannelReport->hide();
    }

    m_pTerminalInfoWidget->show();

    //updateTerminalInfoMenu();

    m_eReportType = TERMINAL_INFO_REPORT;
}

void CManagerParamSettingsPage::showTestChannelReport()
{
    m_pTerminalInfoWidget->hide();

    if (!m_pTestChannelReport)
    {
        createTestChannelReport();
    }

    m_pTestChannelReport->show();

    updateTestChannelReport();

    //updateTestChannelMenu();

    setContentCaption(tr("管理维护->参数设置->测试通道"));

    m_eReportType = TEST_CHANNEL_REPORT;
}

void CManagerParamSettingsPage::updateTerminalInfoMenu()
{
    setMenuCaption(BTN_SYSTEM_PARAM, tr("系统参数"));
    setMenuCaption(BTN_TEST_CHANNEL, tr("测试通道"));
    setMenuCaption(BTN_SYSTEM_MAINTAIN, tr("系统维护"));
    setMenuCaption(BTN_BACK, tr("返回"));
}

void CManagerParamSettingsPage::updateTestChannelMenu()
{
    setMenuCaption(BTN_START_TEST, tr("开始测试"));
    setMenuCaption(BTN_STOP_TEST, tr("停止测试"));
    setMenuCaption(BTN_TEMP, tr(""));
    setMenuCaption(BTN_RETURN, tr("返回"));
}

void CManagerParamSettingsPage::updateTestChannelReport()
{
    QList<DM_TestChannel> testChannel;
    m_pDataMgr->getTestChannelInfo(testChannel);

    clearTestChannelReport();

    for(int row = 0;row < testChannel.size();row++)
    {
        QString str = QString::number(testChannel[row].num);
        m_pTestChannelReport->item(row,0)->setText(str);

        str = testChannel[row].deviceNum;
        m_pTestChannelReport->item(row,1)->setText(str);

        str = testChannel[row].communicateState;
        m_pTestChannelReport->item(row,2)->setText(str);
    }
}

void CManagerParamSettingsPage::clearTestChannelReport()
{
    if (m_pTestChannelReport)
    {
        for (int row = 0;row < m_pTestChannelReport->rowCount();row++)
        {
            for(int column = 0; column < 3;column++)
            {
                m_pTestChannelReport->item(row,column)->setText(" ");
            }
        }
    }
}