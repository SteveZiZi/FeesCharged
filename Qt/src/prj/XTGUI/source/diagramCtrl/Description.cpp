#include "inc/control/diagramCtrl/Description.h"
#include <QtGui/QPainter>

using namespace MDIAGRAM;


CDescription::CDescription(quint32 id, CComponent* pComponent)
: CDecorator(id, pComponent)
, m_bShowLabel(TRUE)
, m_pos(0, 0)
, m_strDescription("")
{
    m_labelClr = qRgb(255, 0, 0);
    m_descriptionClr = qRgb(255,255,255);

    m_font = QFont(QString::fromLocal8Bit(WFG_FONT_STYLE), WFG_TITLE_FONT_SIZE, QFont::Normal);
}

CDescription::~CDescription()
{
}

//override
void CDescription::Draw(QPainter* painter, const QRect& rect)
{
    GetComponent()->Draw(painter, rect);

    painter->save();
    painter->setPen(m_descriptionClr);
    painter->setFont(m_font);

    int pos = 0;
    QFontMetrics fontMetrics = painter->fontMetrics();
    if (m_bShowLabel)
    {
        pos = fontMetrics.height();
        QRect label(m_pos.x(), m_pos.y(), pos, pos);
        label.adjust(3,3, -3,-3);
        painter->fillRect(label, m_labelClr);
        pos += 2;
    }

    painter->drawText(m_pos.x()+pos, 
                      m_pos.y(), 
                      fontMetrics.width(m_strDescription), 
                      fontMetrics.height(), 
                      Qt::AlignCenter, 
                      m_strDescription);

    painter->restore();
}