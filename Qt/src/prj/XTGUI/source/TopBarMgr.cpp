#include "inc/TopBarMgr.h"
#include "src/main/common.h"
#include "../encrypt/encrypt.h"

#include <QLabel>

#define MENU_ICON_L     TOP_BAR_POS_L
#define MENU_ICON_T     TOP_BAR_POS_T
#define MENU_ICON_W     112
#define MENU_ICON_H     TOP_BAR_SIZE_H

#define CAPTION_L      (MENU_ICON_W+2)
#define CAPTION_T       MENU_ICON_T
#define CAPTION_W      (SCREEN_SIZE_W-MENU_ICON_W-2)
#define CAPTION_H       MENU_ICON_H


class CTopBarMgrPrivate
{
private:
    QLabel* caption;
    QLabel* menuIcon;

    friend class CTopBarMgr;
};

CTopBarMgr::CTopBarMgr(QWidget* parent/* = 0*/, Qt::WindowFlags f/* = 0*/)
: QWidget(parent,f)
, m_data(new CTopBarMgrPrivate())
{
    if (!verify_encrypt()) {
        return ;
    }
    this->setGeometry(TOP_BAR_POS_L, TOP_BAR_POS_T, TOP_BAR_SIZE_W, TOP_BAR_SIZE_H);
    this->setObjectName(QString::fromLocal8Bit("topBarMgr"));

    m_data->menuIcon = new QLabel(this);
    m_data->menuIcon->setText(tr("菜单图标"));
    m_data->menuIcon->setGeometry(MENU_ICON_L, MENU_ICON_T, MENU_ICON_W, MENU_ICON_H);
    m_data->menuIcon->setObjectName("topBar_menuIcon");
    m_data->menuIcon->setAlignment(Qt::AlignCenter);

    m_data->caption = new QLabel(this);
    m_data->caption->setText(tr("菜单名"));
    m_data->caption->setGeometry(CAPTION_L, CAPTION_T, CAPTION_W, CAPTION_H);
    m_data->caption->setObjectName("topBar_caption");
    m_data->caption->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
}

CTopBarMgr::~CTopBarMgr()
{
    delete m_data;
}

void CTopBarMgr::setMenuIcon(const QPixmap& pixmap)
{
    Q_ASSERT(m_data->menuIcon);
    m_data->menuIcon->setPixmap(pixmap);
}

void CTopBarMgr::setMenuCaption(const QString& caption)
{
    Q_ASSERT(m_data->caption);
    m_data->caption->setText(caption);
}