#include "costctrlterminal.h"
#include "src/main/common.h"
#include "inc/customEvent.h"
#include "inc/xtDebug.h"

#include "inc/TopBarMgr.h"
#include "inc/BottomBarMgr.h"
#include "inc/contentMgr.h"

#include "src/main/content/userMainPage.h"
#include "src/main/content/chargePage.h"
#include "src/main/content/userQueryPage.h"
#include "src/main/content/userCurrentElecPage.h"
#include "src/main/content/userHistoryElecPage.h"
#include "src/main/content/userChargeReocrdPage.h"
#include "src/main/content/userRepay.h"
#include "src/main/content/userNoticePage.h"

#include "src/main/content/managerMainPage.h"
#include "src/main/content/managerAndMaintainPage.h"
#include "src/main/content/managerPowerMonitorPage.h"
#include "src/main/content/managerTerminalInfoPage.h"
#include "src/main/content/managerParamSettingsPage.h"
#include "src/main/content/managerSystemParamPage.h"
#include "src/main/content/managerSystemMaintenancePage.h"

#include "src/dataLevel/CardStateMgr/CardStateManager.h"

#include <QTimer>
#include <QKeyEvent>
#include <QCoreApplication>

#if JUST_FOR_DEMO 
#include "src/main/utils/splash.h"
#include "src/main/utils/publicNotice.h"
#include "src/main/utils/xt_utility.h"
#endif

#define CARD_STATE_UPDATE_TIME      200


CBaseContent* createContent( int nPageID, QWidget* parent)
{
    CBaseContent* content = NULL;

    switch(nPageID)
    {
    case USER_MAIN_PAGE_ID:
        content = new CUserMainPage(parent);
        break;

    case USER_CHARGE_PAGE_ID:
        content = new CChargePage(parent);
        break;

    case USER_QUERY_PAGE_ID:
        content = new CUserQueryPage(parent);
        break;
    case USER_QUERY_CURRENT_ELEC_ID:
        content = new CUserCurrentElecPage(parent);
        break;
    case USER_QUERY_HISTORY_ELEC_ID:
        content = new CUserHistoryElecPage(parent);
        break;
    case USER_ELEC_RECORD_ID:
        content = new CUserChargeRecordPage(parent);
        break;

    case USER_REPAY:
        content = new CUserRepayPage(parent);
        break;

    case USER_NOTICE_ID:
        content = new CUserNoticePage(parent);
        break;

    // Î¬»¤¿¨
    case MANAGER_MAIN_PAGE_ID:
        content = new CManagerMainPage(parent);
        break;

    case MANAGER_MAINTAIN_PAGE_ID:
        content = new CManagerAndMaintainPage(parent);
        break;
    case MANAGER_TERMINAL_INFO_ID:
        content = new CManagerTerminalInfoPage(parent);
        break;
    case MANAGER_PARAM_SETTING_ID:
        content = new CManagerParamSettingsPage(parent);
        break;
    case MANAGER_SYSTEM_PARAM_ID:
        content = new CManagerSystemParamPage(parent);
        break;
    case MANAGER_SYSTEM_MAINTENACE_ID:
        content = new CManagerSystemMaintenancePage(parent);
        break;

    case MANAGER_POWER_MONITOR_ID:
        content = new CManagerPowerMonitorPage(parent);
        break;

    default:
        Q_ASSERT(FALSE);
    }

    return content;
}



CostCtrlTerminal::CostCtrlTerminal(QWidget *parent, Qt::WFlags flags)
: QDialog(parent, flags)
, m_bottomBar(NULL)
, m_topBar(NULL)
, m_pCardStateMgr(CCardStateManager::getInstance())
{
    setWindowFlags(Qt::FramelessWindowHint);
    this->resize(SCREEN_SIZE_W, SCREEN_SIZE_H);

    m_bottomBar = new CBottomBarMgr(this);
    m_bottomBar->showStatus(tr("ÔËÐÐ×´Ì¬"), -1);
    m_topBar = new CTopBarMgr(this);

    m_contentMgr = CContentMgr::GetInstance();
    m_contentMgr->setParent(this);
    m_contentMgr->setCreateContentFn(createContent);
    m_contentMgr->ShowPage(USER_MAIN_PAGE_ID);

    m_pCardStateTimer = new QTimer(this);
    m_pCardStateTimer->start(CARD_STATE_UPDATE_TIME);

    localConnect();

//     xt_msleep(2000);
//     cardStatusChanged(m_pCardStateMgr->getCardType(),m_pCardStateMgr->getCardState());
}

CostCtrlTerminal::~CostCtrlTerminal()
{

}


void CostCtrlTerminal::cardStatusChanged(CCT::CARD_TYPE cardType, CCT::CARD_STATUS status)
{
    CCT::CARD_TYPE newType = cardType;
    if (status == CCT::REMOVE_CARD) {
        newType = CCT::NO_CARD;
        xtRelease(("remove card******************************"));
#if JUST_FOR_DEMO
        m_contentMgr->ShowPage(USER_MAIN_PAGE_ID);
        xt_msleep(100);
#if 1
        CPublicNotice::getInstance()->splash();
#else
        SplashDlg::getInstance()->splash();
#endif
#endif
    }
    else
    {
        xtRelease(("insert card*******************************"));
#if JUST_FOR_DEMO
        if (CCT::USER_CARD == cardType)
        {
            xtRelease(("user card*******************************"));
#if 1
            CPublicNotice::getInstance()->stopSplash();
#else
            SplashDlg::getInstance()->hide();
#endif
            xt_msleep(200);
            m_contentMgr->ShowPage(USER_MAIN_PAGE_ID);
        }
        else if (CCT::MAINTAINER_CARD == cardType)
        {
            xtRelease(("maintainer card*******************************"));
#if 1
            CPublicNotice::getInstance()->stopSplash();
#else
            SplashDlg::getInstance()->hide();
#endif
            xt_msleep(200);
            m_contentMgr->ShowPage(MANAGER_MAIN_PAGE_ID);
        }
        
#endif
    }

    if (m_currentCardType != newType)
    {
        m_currentCardType = newType;
    }
}


bool CostCtrlTerminal::event(QEvent *e)
{
    QEvent::Type type = e->type();
    if (type >= CCT_EVENT_CARD_STATUS && type < CCT_MAX_EVENT) {
        switch(type) 
        {
        case CCT_EVENT_CARD_STATUS:
            {
                CCardStatusEvent* cardEvent = dynamic_cast<CCardStatusEvent*>(e);
                if (cardEvent) {
                    cardStatusChanged(cardEvent->cardType(), cardEvent->cardStatus());
                }
            }
            break;
        case CCT_EVENT_TOP_BAR:
            {
                CTopBarEvent* topEvent = dynamic_cast<CTopBarEvent*>(e);
                m_topBar->setMenuCaption(topEvent->str_caption());
            }
            break;
        }
    }
    else {
        return QWidget::event(e);
    }



    return true;
}

//test
void CostCtrlTerminal::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F9) {
        cardStatusChanged(CCT::USER_CARD, CCT::INSERT_CARD);
    }
    else if (event->key() == Qt::Key_F10) {
        cardStatusChanged(CCT::MAINTAINER_CARD, CCT::INSERT_CARD);
    }
    else if (event->key() == Qt::Key_F11) {
        cardStatusChanged(CCT::USER_CARD, CCT::REMOVE_CARD);
    }

    return QDialog::keyPressEvent(event);
}

void CostCtrlTerminal::onCardStateTimer()
{
    if(m_pCardStateMgr->cardStateChanged())
    {
        QCoreApplication::postEvent(this,new CCardStatusEvent(m_pCardStateMgr->getCardType(),m_pCardStateMgr->getCardState()));
    }
}

void CostCtrlTerminal::localConnect()
{
    connect(m_pCardStateTimer,SIGNAL(timeout()),this,SLOT(onCardStateTimer()));
}