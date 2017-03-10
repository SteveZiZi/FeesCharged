#include "managerAndMaintainPage.h"

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


CManagerAndMaintainPage::CManagerAndMaintainPage(QWidget* parent, Qt::WindowFlags f)
: CBaseContent(parent,f)
{
    m_pDataMgr = CDataManager::getInstance();
    setContentCaption(tr("管理维护"));
    initUI();
    updateMenuInfo();
}

CManagerAndMaintainPage::~CManagerAndMaintainPage(void)
{

}

void CManagerAndMaintainPage::initUI()
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

void CManagerAndMaintainPage::initMenu()
{
    setMenuCaption(BTN_INFO, tr("终端信息"));
    setMenuCaption(BTN_PARAM_SET, tr("参数设置"));
    setMenuCaption(BTN_BACK, tr("返回"));

     setMenuContentPage(BTN_INFO, MANAGER_TERMINAL_INFO_ID);
     setMenuContentPage(BTN_PARAM_SET, MANAGER_PARAM_SETTING_ID);
    setMenuContentPage(BTN_BACK, MANAGER_MAIN_PAGE_ID);
}

void CManagerAndMaintainPage::updateTerminalInfo()
{
    DM_TerminalInfo terminalInfo;
    m_pDataMgr->getTerminalInfo(terminalInfo);
    m_pTerminalInfoWidget->updateTerminalInfo(terminalInfo);
}