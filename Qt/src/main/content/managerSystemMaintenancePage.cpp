#include <QLabel>
#include "managerSystemMaintenancePage.h"

#include "src/main/common.h"

#define LABEL_L     250
#define LABEL_T     150

CManagerSystemMaintenancePage::CManagerSystemMaintenancePage(QWidget* parent, Qt::WindowFlags f)
: CBaseContent(parent,f)
{
    setContentCaption(tr("����ά��->��������->ϵͳά��"));
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
    m_pMessageLabel->setText(tr("�Ƿ������ն�?"));
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
    setMenuCaption(BTN_VERIFY, tr("ȷ��"));
    setMenuCaption(BTN_CANCEL, tr(""));
    setMenuCaption(BTN_TEMP, tr(""));
    setMenuCaption(BTN_BACK, tr("����"));

    setMenuContentPage(BTN_BACK, MANAGER_PARAM_SETTING_ID);
}