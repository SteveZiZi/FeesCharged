#include "managerMainPage.h"

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

CManagerMainPage::CManagerMainPage(QWidget* parent, Qt::WindowFlags f)
: CBaseContent(parent,f)
{
    m_pDataMgr = CDataManager::getInstance();
    setContentCaption(tr("运行管理"));
    initUI();
    updateMenuInfo();
}

CManagerMainPage::~CManagerMainPage(void)
{

}

void CManagerMainPage::initUI()
{
    CBaseContent::initUI();
    initMenu();
    m_pTerminalInfoWidget = new CTerminalInfoReport(this);
    m_pTerminalInfoWidget->setGeometry(TABLE_L, TABLE_T, TABLE_W, TABLE_H);
    m_pTerminalInfoWidget->setCaptionWidth(TABLE_CAPTION_W);
    m_pTerminalInfoWidget->setContentWidth(TABLE_CONTENT_W);
    m_pTerminalInfoWidget->setObjectName("tabelWidget_terminalInfo");
    m_pTerminalInfoWidget->createTableWidget(CTerminalInfoReport::TERMINAL_INFO_ROW, 2);
    m_pTerminalInfoWidget->updateCaptionColumn();
    updateTerminalInfo();
}

void CManagerMainPage::initMenu()
{
    setMenuCaption(BTN_MANAGER, tr("管理维护"));
    setMenuCaption(BTN_MONITOR, tr("用电检测"));
    setMenuCaption(BTN_BACK, tr("退出"));

    setMenuContentPage(BTN_MANAGER, MANAGER_MAINTAIN_PAGE_ID);
    setMenuContentPage(BTN_MONITOR, MANAGER_POWER_MONITOR_ID);
    setMenuContentPage(BTN_BACK, USER_MAIN_PAGE_ID);
}

void CManagerMainPage::updateTerminalInfo()
{
    DM_TerminalInfo terminalInfo;
    m_pDataMgr->getTerminalInfo(terminalInfo);
    m_pTerminalInfoWidget->updateTerminalInfo(terminalInfo);
}
