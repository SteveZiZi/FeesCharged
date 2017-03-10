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
     *  @Desc:      ��״̬�Ƿ�ı�
     *  @PaamIn:    none
     *  @Return:
     *      true:   ����״̬�ı���
     *      false:  ����״̬û�ı�
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
