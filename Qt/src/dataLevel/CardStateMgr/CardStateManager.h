#pragma once

#include "inc/dataTypes.h"

class CCardStateManager
{
private:
    CCardStateManager(void);
    ~CCardStateManager(void);

public:
    static CCardStateManager* getInstance();
    /*
     *  @Func:      cardStateChanged
     *  @Desc:      卡状态是否改变
     *  @PaamIn:    none
     *  @Return:
     *      true:   卡的状态改变了
     *      false:  卡的状态没改变
     */
    bool cardStateChanged();
    CCT::CARD_TYPE getCardType();
    CCT::CARD_STATUS getCardState();

private:
    CCT::CARD_TYPE      m_eCardType;
    CCT::CARD_STATUS    m_eCardState;
    CCT::CARD_STATUS    m_eOldCardState;

    bool                m_bCardStateChanged;
};
