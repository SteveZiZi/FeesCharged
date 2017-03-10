#include "CardStateManager.h"
#include <QDebug>
#include "src/dataLevel/dataComm/IfDataComm.h"
#include "inc/xtDebug.h"

CCardStateManager::CCardStateManager(void)
{
    m_bCardStateChanged = false;
    m_eCardType = CCT::USER_CARD;
    m_eCardState = CCT::NONE_CARD;
    m_eOldCardState = CCT::NONE_CARD;
}

CCardStateManager::~CCardStateManager(void)
{

}

CCardStateManager* CCardStateManager::getInstance()
{
    static CCardStateManager cardStateMgr;
    return &cardStateMgr;
}

bool CCardStateManager::cardStateChanged()
{
    if(m_eOldCardState == getCardState())
    {
        return false;
    }
    else 
    {
        m_eOldCardState = getCardState();
        return true;
    }
}

CCT::CARD_TYPE CCardStateManager::getCardType()
{
    if (INVALID_CARD == CIfDatacomm::getCardType())
    {
        m_eCardType = CCT::NO_CARD;
    }
    else if(PRESET_CARD == CIfDatacomm::getCardType())
    {
        m_eCardType = CCT::MAINTAINER_CARD;
    }
    else if(USER_CARD == CIfDatacomm::getCardType())
    {
        m_eCardType = CCT::USER_CARD;
    }

    return m_eCardType;
}

CCT::CARD_STATUS CCardStateManager::getCardState()
{
    if(CARD_OUT == CIfDatacomm::getCardState())
    {
        m_eCardState = CCT::REMOVE_CARD;
    }
    else if (CARD_IN == CIfDatacomm::getCardState())
    {
        m_eCardState = CCT::INSERT_CARD;
    }

    return m_eCardState;
}