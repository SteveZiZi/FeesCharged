#pragma once

#include <QEvent>
#include "inc/dataTypes.h"


enum CCT_EVENT_ENUM {
    CCT_EVENT_CARD_STATUS = QEvent::User+100,       //  card status changed event
    CCT_EVENT_TOP_BAR,
    CCT_EVENT_BOTTOM_BAR,

    CCT_MAX_EVENT
};

/*
 *  用户卡、运维卡 状态变化事件
 */
class CCardStatusEvent : public QEvent
{
public:
    CCardStatusEvent(CCT::CARD_TYPE cardType, CCT::CARD_STATUS status)
        : QEvent(static_cast<Type>(CCT_EVENT_CARD_STATUS)), card_type(cardType), card_status(status)
    {  }

    CCT::CARD_TYPE cardType() {return this->card_type;}
    CCT::CARD_STATUS cardStatus() {return this->card_status;}
private:
    CCT::CARD_TYPE      card_type;
    CCT::CARD_STATUS    card_status;
};

/*
 *  修改顶部菜单事件
 */
class QIcon;
class CTopBarEvent : public QEvent {
public:
    CTopBarEvent(const QString& caption, QIcon* captionIcon = 0)
        : QEvent(static_cast<Type>(CCT_EVENT_TOP_BAR))
    { 
        this->icon = captionIcon; 
        this->caption = caption;
    }

    QIcon* icon_caption() {return this->icon;}
    const QString& str_caption() {return this->caption;}
private:
    QIcon* icon;
    QString caption;
};