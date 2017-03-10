#include "inc/control/diagramCtrl/Histogram.h"
#include "../../encrypt/encrypt.h"

#include <QtGui/QPainter>
using namespace MDIAGRAM;


void CHistogram::Draw(QPainter* painter, const QRect& rect)
{
    Q_ASSERT(m_ptScaleInfo);

    if (!verify_encrypt()) {
        return ;
    }

    GetComponent()->Draw(painter, rect);

    QRect waveRect(rect);
    waveRect.adjust(m_ptScaleInfo->magin.left, m_ptScaleInfo->magin.top, 0-(m_ptScaleInfo->magin.right), 0-(m_ptScaleInfo->magin.bottom));

    painter->save();
    QPoint center = m_ptScaleInfo->GetCenterPoint();
    float xSpanWidth = m_ptScaleInfo->GetSpanWidthX();
    float ySpanWidth = m_ptScaleInfo->GetSpanWidthY();
    int x0, y0;
    int w, h;
    QString text;
    QFontMetrics fontMetrics = painter->fontMetrics();
    for(mapPillarsInfo::ConstIterator it = m_pillarsInfo.constBegin(); it != m_pillarsInfo.constEnd(); it++)
    {
        h = static_cast<int>(it->height/m_ptScaleInfo->fScaleY*ySpanWidth);
        w = static_cast<int>(qMin<float>(it->width, xSpanWidth));
        x0 = static_cast<int>(center.x() + it.key()/m_ptScaleInfo->fScaleX*xSpanWidth-w/2);
        y0 = static_cast<int>(center.y() - h);

        if (x0 > waveRect.right()) {
            x0 = waveRect.right();
            w = 0;
        }
        else if (x0 < waveRect.left()) {
            x0 = waveRect.left();
        }

        if (y0 > waveRect.bottom()) {
            y0 = waveRect.bottom();
            h = 0;
        }
        else if (y0 < waveRect.top()) {
            y0 = waveRect.top();
        }

        QRect rect(x0, y0-1, w, h);
        painter->setPen(QPen(QColor(it->graphClr), WFG_WAVE_LINE_WIDTH, WFG_WAVE_LINE_STYLE));
        painter->setBrush(QBrush(QColor(it->graphClr)));
        painter->drawRect(rect);

        painter->setPen(QPen(QColor(it->textClr), WFG_WAVE_LINE_WIDTH, WFG_WAVE_LINE_STYLE));
        text = QString("%1").arg(it->height, 0, 'f', 2, QChar('0'));
        if (!it->unit.isEmpty()) {
            text.append(' ');
            text.append(it->unit);
        }
        w = fontMetrics.width(text);
        h = fontMetrics.height();
        x0 = rect.center().x() - w/2;
        y0 = y0-h-3;
        painter->drawText(x0, y0, w, h, Qt::AlignCenter,text);
    }
    painter->restore();
}



void CHistogram::setPillarItem(int item, const PillarItemInfo &pillarInfo)
{
    mapPillarsInfo::iterator it = m_pillarsInfo.find(item);
    if (it != m_pillarsInfo.end())
    {
        it->graphClr = pillarInfo.graphClr;
        it->height = pillarInfo.height;
        it->width = pillarInfo.width;
        it->unit = pillarInfo.unit;
        it->textClr = pillarInfo.textClr;
    }
    else {
        m_pillarsInfo.insert(item, pillarInfo);
    }
}

bool CHistogram::updatePillarInfo(int item, quint32 w)
{
    mapPillarsInfo::iterator it = m_pillarsInfo.find(item);
    if (it != m_pillarsInfo.end())
    {
        it->width = w;
        return true;
    }

    return false;
}

bool CHistogram::updatePillarInfo(int item, qreal h)
{
    mapPillarsInfo::iterator it = m_pillarsInfo.find(item);
    if (it != m_pillarsInfo.end())
    {
        it->height = h;
        return true;
    }

    return false;
}

bool CHistogram::updatePillarInfo(int item, const QRgb &clr)
{
    mapPillarsInfo::iterator it = m_pillarsInfo.find(item);
    if (it != m_pillarsInfo.end())
    {
        it->graphClr = clr;
        return true;
    }

    return false;
}