#include <QLabel>
#include "managerSystemMaintenancePage.h"

#include "src/main/common.h"

#define LABEL_L     250
#define LABEL_T     150

CManagerSystemMaintenancePage::CManagerSystemMaintenancePage(QWidget* parent, Qt::WindowFlags f)
: CBaseContent(parent,f)
{
    setContentCaption(tr("管理维护->参数设置->系统维护"));
    initUI();
    initMenu();
}

CManagerSystemMaintenancePage::~CManagerSystemMaintenancePage(void)
{

}

void CManagerSystemMaintenancePage::initUI()
{
    CBaseContent::initUI();
    m_pMessageLabel = new QLabel(this);
    m_pMessageLabel->setText(tr("是否重启终端?"));
    m_pMessageLabel->setStyleSheet("color:yellow;");
    m_pMessageLabel->move(LABEL_L,LABEL_T);
}

bool CManagerSystemMaintenancePage::OnF1Press()
{
#ifndef WIN32
    system("reboot");
#else

#endif
    return true;
}

void CManagerSystemMaintenancePage::initMenu()
{
    setMenuCaption(BTN_VERIFY, tr("确认"));
    setMenuCaption(BTN_CANCEL, tr(""));
    setMenuCaption(BTN_TEMP, tr(""));
    setMenuCaption(BTN_BACK, tr("返回"));

    setMenuContentPage(BTN_BACK, MANAGER_PARAM_SETTING_ID);
}