#include <QtGui/QScrollBar>
#include <QtGui/QHeaderView>
#include "managerTerminalInfoPage.h"

#include "src/main/common.h"
#include "src/common_ctrl/reportWidget.h"


#define TABLE_L         176
#define TABLE_T         144
#define TABLE_W         336
#define TABLE_H         71
#define TABLE_ITEM_H    35
#define TABLE_CAPTION_W 168
#define TABLE_CONTENT_W 168

#define BASE_INFO_REPORT_L         144
#define BASE_INFO_REPORT_T         97
#define BASE_INFO_REPORT_W         400
#define BASE_INFO_REPORT_H         170
#define BASE_INFO_REPORT_ITEM_H    34
#define BASE_INFO_REPORT_CAPTION_W 200
#define BASE_INFO_REPORT_CONTENT_W 200

#define COMMUNICATE_INFO_REPORT_L         144
#define COMMUNICATE_INFO_REPORT_T         57
#define COMMUNICATE_INFO_REPORT_W         416
#define COMMUNICATE_INFO_REPORT_H         264
#define COMMUNICATE_INFO_REPORT_ITEM_H    33
#define COMMUNICATE_INFO_REPORT_CAPTION_W 200
#define COMMUNICATE_INFO_REPORT_CONTENT_W 216

#define USER_FILE_INFO_REPORT_L         48
#define USER_FILE_INFO_REPORT_T         24
#define USER_FILE_INFO_REPORT_W         592
#define USER_FILE_INFO_REPORT_H         352
#define USER_FILE_INFO_REPORT_ITEM_H    32
#define USER_FILE_INFO_REPORT_ONE_W     64
#define USER_FILE_INFO_REPORT_TWO_W     88
#define USER_FILE_INFO_REPORT_THREE_W   88
#define USER_FILE_INFO_REPORT_FOUR_W    176
#define USER_FILE_INFO_REPORT_FIVE_W    176
#define USER_FILE_INFO_ROW_COUNT        10
#define USET_FILE_INFO_COLUMN_COUNT     5

CManagerTerminalInfoPage::CManagerTerminalInfoPage(QWidget* parent, Qt::WindowFlags f)
: CBaseContent(parent,f)
, m_pBaseInfoReport(NULL)
, m_pCommunicateInfoReport(NULL)
, m_pUserFileInfoReport(NULL)
, m_eReportType(TERMINAL_INFO_REPORT)
, m_iUserFilePageIndex(0)
{
    m_pDataMgr = CDataManager::getInstance();
    setContentCaption(tr("管理维护->终端信息"));
    initUI();
    initMenu();
}

CManagerTerminalInfoPage::~CManagerTerminalInfoPage(void)
{

}

void CManagerTerminalInfoPage::initUI()
{
    CBaseContent::initUI();
    createTerminalInfoReport();
}

bool CManagerTerminalInfoPage::OnF1Press()
{
    if (TERMINAL_INFO_REPORT == m_eReportType)
    {
        showBaseInfoReport();
    }
    else if (BASE_INFO_RPORT == m_eReportType)
    {
        showBaseInfoReport();
    }
    else if (USER_FILE_INFO_REPORT == m_eReportType)
    {
        m_iUserFilePageIndex--;
        if (m_iUserFilePageIndex < 0)
        {
            m_iUserFilePageIndex = 0;
        }

        updateUserFileInfoReport();
    }
    else if (COMMUNICATE_INFO_REPORT == m_eReportType)
    {
        showBaseInfoReport();
    }

    return true;
}

bool CManagerTerminalInfoPage::OnF2Press()
{
    if (TERMINAL_INFO_REPORT == m_eReportType)
    {
        showUserFileInfoReport();
    }
    else if (BASE_INFO_RPORT == m_eReportType)
    {
        showUserFileInfoReport();
    }
    else if (USER_FILE_INFO_REPORT == m_eReportType)
    {
        m_iUserFilePageIndex++;
        if (m_lUserFileInfoList.size() % 10)
        {
            if (m_iUserFilePageIndex >= m_lUserFileInfoList.size() / 10 + 1)
            {
                m_iUserFilePageIndex = m_lUserFileInfoList.size() / 10;
            }
        }
        else
        {
            if (m_iUserFilePageIndex >= m_lUserFileInfoList.size() / 10)
            {
                m_iUserFilePageIndex = m_lUserFileInfoList.size() / 10 - 1;
            }
        }

        updateUserFileInfoReport();
    }
    else if (COMMUNICATE_INFO_REPORT == m_eReportType)
    {
        showUserFileInfoReport();
    }

    return true;
}

bool CManagerTerminalInfoPage::OnF3Press()
{
    if (TERMINAL_INFO_REPORT == m_eReportType)
    {
        showCommunicateInfoReport();
    }
    else if (BASE_INFO_RPORT == m_eReportType)
    {
        showCommunicateInfoReport();
    }
    else if (USER_FILE_INFO_REPORT == m_eReportType)
    {

    }
    else if (COMMUNICATE_INFO_REPORT == m_eReportType)
    {
        showCommunicateInfoReport();
    }

    return true;
}

bool CManagerTerminalInfoPage::OnF4Press()
{
    if (TERMINAL_INFO_REPORT == m_eReportType)
    {

    }
    else if (BASE_INFO_RPORT == m_eReportType)
    {

    }
    else if (USER_FILE_INFO_REPORT == m_eReportType)
    {
        showTerminalInfoReport();
        return true;
    }
    else if (COMMUNICATE_INFO_REPORT == m_eReportType)
    {

    }

    return false;
}

void CManagerTerminalInfoPage::initMenu()
{
    setMenuCaption(BTN_BASE_INFO, tr("基本信息"));
    setMenuCaption(BTN_USER_FILES, tr("用户档案"));
    setMenuCaption(BTN_COMMUNICATE_PARAM, tr("通讯参数"));
    setMenuCaption(BTN_BACK, tr("返回"));

    setMenuContentPage(BTN_BACK,MANAGER_MAINTAIN_PAGE_ID);
}

void CManagerTerminalInfoPage::createTerminalInfoReport()
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

void CManagerTerminalInfoPage::createBaseInfoReport()
{
    m_pBaseInfoReport = new CBaseInfoReport(this);
    m_pBaseInfoReport->setGeometry(BASE_INFO_REPORT_L, BASE_INFO_REPORT_T, BASE_INFO_REPORT_W, BASE_INFO_REPORT_H);
    m_pBaseInfoReport->setCommonRowHeight(BASE_INFO_REPORT_ITEM_H);
    m_pBaseInfoReport->setCaptionWidth(BASE_INFO_REPORT_CAPTION_W);
    m_pBaseInfoReport->setContentWidth(BASE_INFO_REPORT_CONTENT_W);
    m_pBaseInfoReport->setObjectName("tabelWidget_baseInfo");
    m_pBaseInfoReport->createTableWidget(CBaseInfoReport::BASE_INFO_ROW, 2);
    m_pBaseInfoReport->updateCaptionColumn();
    // update
    DM_TerminalBaseInfo baseInfo;
    m_pDataMgr->getTerminalBaseInfo(baseInfo);
    m_pBaseInfoReport->updateBaseInfo(baseInfo);
}

void CManagerTerminalInfoPage::createUserFileInfoReport()
{
    const QString headerCaption[5] = {
        tr("序号"),
        tr("户号"),
        tr("门牌号"),
        tr("表号"),
        tr("表地址")
    };

    m_pUserFileInfoReport = new MTableWidget(USER_FILE_INFO_ROW_COUNT,USET_FILE_INFO_COLUMN_COUNT,this);
    m_pUserFileInfoReport->setGeometry(USER_FILE_INFO_REPORT_L, USER_FILE_INFO_REPORT_T, 
                                       USER_FILE_INFO_REPORT_W, USER_FILE_INFO_REPORT_H);

    m_pUserFileInfoReport->verticalHeader()->hide();
    m_pUserFileInfoReport->horizontalScrollBar()->hide();
    m_pUserFileInfoReport->verticalScrollBar()->hide();

    m_pUserFileInfoReport->setShowGrid(false);
    m_pUserFileInfoReport->setGridStyle(new MGridStyle(QColor(TABLE_LINE_CLR), true, true));
    m_pUserFileInfoReport->setShowTopBottomLine(true, true);
    m_pUserFileInfoReport->setShowLeftRightLine(true, true);
    m_pUserFileInfoReport->setAlternatingRowColors(false);

    m_pUserFileInfoReport->horizontalHeader()->setFixedHeight(USER_FILE_INFO_REPORT_ITEM_H);

    for(int i = 0; i < m_pUserFileInfoReport->columnCount(); i++) {
        QTableWidgetItem *headerItem = new QTableWidgetItem(headerCaption[i]);
        m_pUserFileInfoReport->setHorizontalHeaderItem(i, headerItem);
        headerItem->setTextAlignment(Qt::AlignCenter);
    }

    m_pUserFileInfoReport->setColumnWidth(0, USER_FILE_INFO_REPORT_ONE_W);
    m_pUserFileInfoReport->setColumnWidth(1, USER_FILE_INFO_REPORT_TWO_W);
    m_pUserFileInfoReport->setColumnWidth(2, USER_FILE_INFO_REPORT_THREE_W);
    m_pUserFileInfoReport->setColumnWidth(3, USER_FILE_INFO_REPORT_FOUR_W);
    m_pUserFileInfoReport->setColumnWidth(4, USER_FILE_INFO_REPORT_FIVE_W);

    for (int row = 0; row < m_pUserFileInfoReport->rowCount(); ++row) {
        for (int column = 0; column < m_pUserFileInfoReport->columnCount(); ++column) {
            QTableWidgetItem *newItem = new QTableWidgetItem();
            if (column == 0) {
                newItem->setFlags(Qt::ItemIsSelectable);
                newItem->setTextAlignment(Qt::AlignCenter);
            }
            else {
                newItem->setTextAlignment(Qt::AlignCenter);
            }
            m_pUserFileInfoReport->setItem(row, column, newItem);
        }
        m_pUserFileInfoReport->setRowHeight(row, USER_FILE_INFO_REPORT_ITEM_H);
    }

    m_pDataMgr->getUserFileInfo(m_lUserFileInfoList);
}

void CManagerTerminalInfoPage::createCommunicateInfoReport()
{
    m_pCommunicateInfoReport = new CCommunicateInfoReport(this);
    m_pCommunicateInfoReport->setGeometry(COMMUNICATE_INFO_REPORT_L, COMMUNICATE_INFO_REPORT_T, COMMUNICATE_INFO_REPORT_W, COMMUNICATE_INFO_REPORT_H);
    m_pCommunicateInfoReport->setCommonRowHeight(COMMUNICATE_INFO_REPORT_ITEM_H);
    m_pCommunicateInfoReport->setCaptionWidth(COMMUNICATE_INFO_REPORT_CAPTION_W);
    m_pCommunicateInfoReport->setContentWidth(COMMUNICATE_INFO_REPORT_CONTENT_W);
    m_pCommunicateInfoReport->setObjectName("tabelWidget_communicateInfo");
    m_pCommunicateInfoReport->createTableWidget(CCommunicateInfoReport::COMMUNICATE_INFO_ROW, 2);
    m_pCommunicateInfoReport->updateCaptionColumn();
    // update
    DM_CommunicateInfo communicateInfo;
    m_pDataMgr->getCommunicateInfo(communicateInfo);
    m_pCommunicateInfoReport->updateCommunicateInfo(communicateInfo);
}

void CManagerTerminalInfoPage::showTerminalInfoReport()
{
    setContentCaption(tr("管理维护->终端信息"));

    if (m_pCommunicateInfoReport)
    {
        m_pCommunicateInfoReport->hide();
    }

    if (m_pUserFileInfoReport)
    {
        m_pUserFileInfoReport->hide();
    }

    if (m_pBaseInfoReport)
    {
        m_pBaseInfoReport->hide();
    }

    m_pTerminalInfoWidget->show();

    updateTerminalInfoMenu();

    m_eReportType = TERMINAL_INFO_REPORT;
}

void CManagerTerminalInfoPage::showBaseInfoReport()
{
    setContentCaption(tr("管理维护->终端信息->基本信息"));

    m_pTerminalInfoWidget->hide();

    if (m_pCommunicateInfoReport)
    {
        m_pCommunicateInfoReport->hide();
    }

    if (m_pUserFileInfoReport)
    {
        m_pUserFileInfoReport->hide();
    }

    if (!m_pBaseInfoReport)
    {
        createBaseInfoReport();
    }
    m_pBaseInfoReport->show();

    m_eReportType = BASE_INFO_RPORT;
}

void CManagerTerminalInfoPage::showUserFileInfoReport()
{
    setContentCaption(tr("管理维护->终端信息->用户档案"));

    m_pTerminalInfoWidget->hide();

    if (m_pBaseInfoReport)
    {
        m_pBaseInfoReport->hide();
    }

    if (m_pCommunicateInfoReport)
    {
        m_pCommunicateInfoReport->hide();
    }

    if (!m_pUserFileInfoReport)
    {
        createUserFileInfoReport();
    }

    m_pUserFileInfoReport->show();

    updateUserFileInfoReport();
    updateUserFileMenu();

    m_eReportType = USER_FILE_INFO_REPORT;
}

void CManagerTerminalInfoPage::showCommunicateInfoReport()
{
    setContentCaption(tr("管理维护->终端信息->通讯参数"));

    m_pTerminalInfoWidget->hide();

    if (m_pBaseInfoReport)
    {
        m_pBaseInfoReport->hide();
    }

    if (m_pUserFileInfoReport)
    {
        m_pUserFileInfoReport->hide();
    }

    if (!m_pCommunicateInfoReport)
    {
        createCommunicateInfoReport();
    }
    m_pCommunicateInfoReport->show();

    m_eReportType = COMMUNICATE_INFO_REPORT;
}

void CManagerTerminalInfoPage::updateUserFileInfoReport()
{
    int rowCount;
    if (m_lUserFileInfoList.size())
    {
        if (m_lUserFileInfoList.size() % 10)
        {
            if (m_iUserFilePageIndex < m_lUserFileInfoList.size() / 10)
            {
                rowCount = 10;
            }
            else if (m_iUserFilePageIndex == m_lUserFileInfoList.size() / 10)
            {
                rowCount = m_lUserFileInfoList.size() % 10;
            }
        }
        else
        {
            rowCount = 10;
        }

        clearUserFileInfoReport();

        for(int i = 0; i < rowCount; i++)
        {
            m_pUserFileInfoReport->item(i,0)->setText(QString::number(m_lUserFileInfoList[m_iUserFilePageIndex * 10 + i].num));
            m_pUserFileInfoReport->item(i,1)->setText(m_lUserFileInfoList[m_iUserFilePageIndex * 10 + i].residenceNum);
            m_pUserFileInfoReport->item(i,2)->setText(m_lUserFileInfoList[m_iUserFilePageIndex * 10 + i].houseNum);
            m_pUserFileInfoReport->item(i,3)->setText(m_lUserFileInfoList[m_iUserFilePageIndex * 10 + i].deviceNum);
            m_pUserFileInfoReport->item(i,4)->setText(m_lUserFileInfoList[m_iUserFilePageIndex * 10 + i].deviceAddr);
        }
    }
}

void CManagerTerminalInfoPage::updateTerminalInfoMenu()
{
    setMenuCaption(BTN_BASE_INFO, tr("基本信息"));
    setMenuCaption(BTN_USER_FILES, tr("用户档案"));
    setMenuCaption(BTN_COMMUNICATE_PARAM, tr("通讯参数"));
    setMenuCaption(BTN_BACK, tr("返回"));
}

void CManagerTerminalInfoPage::updateUserFileMenu()
{
    setMenuCaption(BTN_UP, tr("上翻"));
    setMenuCaption(BTN_DOWN, tr("下翻"));
    setMenuCaption(BTN_TEMP, tr(""));
    setMenuCaption(BTN_RETURN, tr("返回"));
}

void CManagerTerminalInfoPage::clearUserFileInfoReport()
{
    if (m_pUserFileInfoReport)
    {
        for (int row = 0;row < m_pUserFileInfoReport->rowCount();row++)
        {
            for(int column = 0; column < 5;column++)
            {
                m_pUserFileInfoReport->item(row,column)->setText(" ");
            }
        }
    }
}