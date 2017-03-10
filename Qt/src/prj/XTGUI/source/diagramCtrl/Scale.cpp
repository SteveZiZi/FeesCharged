#include "inc/control/diagramCtrl/Scale.h"
#include "../../encrypt/encrypt.h"
#include <QPainter>

using namespace MDIAGRAM;

CScale::CScale(quint32 id, CComponent* pComponent/* = NULL*/, int xAxisType/* = POSITIVE*/, int yAxisType/* = POSITIVE*/)
: CDecorator(id, pComponent)
, m_nScaleLineDirX(POSITIVE|NEGATIVE)
, m_nScaleLineDirY(POSITIVE|NEGATIVE)
, m_strTitleX("")
, m_strTitleY("")
, m_bShowTitleX(false)
, m_bShowTitleY(false)
, m_nScaleLineLengthX(9)
, m_nScaleLineLengthY(9)
, m_nDivScaleLineLengthX(5)
, m_nDivScaleLineLengthY(5)
, m_bShowScaleLineX(true)
, m_bShowScaleLineY(true)
, m_bShowDivScaleLineX(true)
, m_bShowDivScaleLineY(true)
, m_bShowXScaleVal(true)
, m_bShowYScaleVal(true)
, m_clTitleColor(WFG_TITLE_FONT_COLOR)
, m_clScaleValColor(WFG_SCALE_FONT_COLOR)
, m_ptScaleInfo(NULL)
, m_scaleValPos(10, 10)
, m_ptTitlePosX(-1, -1)
, m_ptTitlePosY(-1, -1)
, m_bShowXArrow(true)
, m_bShowYArrow(true)
{
    if (!verify_encrypt()) {
        return ;
    }
    m_cTitleFont = QFont(QString::fromLocal8Bit(WFG_FONT_STYLE), WFG_TITLE_FONT_SIZE, QFont::Normal);
    m_cScaleValFont = QFont(QString::fromLocal8Bit(WFG_FONT_STYLE), WFG_SCALE_FONT_SIZE, QFont::Normal);
}



CScale::~CScale()
{
}

void CScale::Draw(QPainter* painter, const QRect& rect)
{
    Q_ASSERT(m_ptScaleInfo && painter);

    GetComponent()->Draw(painter, rect);

    painter->save();
    painter->setPen(QColor(m_clScaleValColor));
    painter->setFont(m_cTitleFont);

    //X Öá
    int x0 = rect.left()+m_ptScaleInfo->magin.left;
    int y0 = m_ptScaleInfo->GetCenterPoint().y();
    int x1 = x0 + rect.width()-m_ptScaleInfo->magin.left-m_ptScaleInfo->magin.right;
    int y1 = y0;
    painter->drawLine(x0, y0, x1, y1);

    //Y Öá
    x0 = m_ptScaleInfo->GetCenterPoint().x();
    y0 = rect.top()+m_ptScaleInfo->magin.top;
    x1 = x0;
    y1 = y0 + rect.height()-m_ptScaleInfo->magin.top-m_ptScaleInfo->magin.bottom;
    painter->drawLine(x0, y0, x1, y1);

    drawTitle(painter, rect);
    drawScaleLine(painter, rect);
    drawDivScaleLine(painter, rect);
    drawScaleVal(painter, rect);
    drawTriangle(painter, rect);

    painter->restore();
}


void CScale::drawScaleLine(QPainter* painter, const QRect& rect)
{
    painter->setPen(QColor(m_clScaleValColor));
    //xÖá 
    if (m_bShowScaleLineX) 
    {
        int x0 = m_ptScaleInfo->GetCenterPoint().x();
        int y0 = m_ptScaleInfo->GetCenterPoint().y();
        int x = 0;

        if (m_nScaleLineDirX == NEGATIVE) {

        }
        else if (m_nScaleLineDirX == POSITIVE) {
            y0 -= m_nScaleLineLengthX;
        }
        else {
            y0 -= m_nScaleLineLengthX/2;
        }
        int y = y0+m_nScaleLineLengthX;

        float xSpanWidth = m_ptScaleInfo->GetSpanWidthX();
        if (m_ptScaleInfo->axisDirX == POSITIVE) {
            for(int i = m_ptScaleInfo->intervalX; i <= m_ptScaleInfo->gridCountX; i+=m_ptScaleInfo->intervalX) {
                x = x0 + (int)(i * xSpanWidth);
                painter->drawLine(x, y0, x, y);
            }
        }
        else if (m_ptScaleInfo->axisDirX == NEGATIVE) {
            for(int i = m_ptScaleInfo->intervalX; i <= m_ptScaleInfo->gridCountX; i+=m_ptScaleInfo->intervalX) {
                x = x0 - (int)(i * xSpanWidth);
                painter->drawLine(x, y0, x, y);
            }
        }
        else  {
            for(int i = 1; i <= m_ptScaleInfo->gridCountX/m_ptScaleInfo->intervalX/2; i++) {
                x = x0 + (int)(xSpanWidth*i*m_ptScaleInfo->intervalX);
                painter->drawLine(x, y0, x, y);
                x = x0 - (int)(xSpanWidth*i*m_ptScaleInfo->intervalX);
                painter->drawLine(x, y0, x, y);
            }
        }
    }

    //yÖá 
    if (m_bShowScaleLineY) 
    {
        int x0 = m_ptScaleInfo->GetCenterPoint().x();
        int y0 = m_ptScaleInfo->GetCenterPoint().y();
        int y = 0;

        if (m_nScaleLineDirY == POSITIVE) {

        }
        else if (m_nScaleLineDirY == NEGATIVE) {
            x0 -= m_nScaleLineLengthY;
        }
        else {
            x0 -= m_nScaleLineLengthY/2;
        }
        int x = x0 + m_nScaleLineLengthY;
    

        float ySpanWidth = m_ptScaleInfo->GetSpanWidthY();
        if (m_ptScaleInfo->axisDirY == POSITIVE) {
            for(int i = m_ptScaleInfo->intervalY; i <= m_ptScaleInfo->gridCountY; i+=m_ptScaleInfo->intervalY) {
                y = y0 - (int)(i * ySpanWidth);
                painter->drawLine(x0, y, x, y);
            }
        }
        else if (m_ptScaleInfo->axisDirY == NEGATIVE) {
            for(int i = m_ptScaleInfo->intervalY; i <= m_ptScaleInfo->gridCountY; i+=m_ptScaleInfo->intervalY) {
                y = y0 + (int)(i * ySpanWidth);
                painter->drawLine(x0, y, x, y);
            }
        }
        else  {
            for(int i = 1; i <= m_ptScaleInfo->gridCountY/m_ptScaleInfo->intervalY/2; i++) {
                y = y0 - (int)(ySpanWidth*i*m_ptScaleInfo->intervalY);
                painter->drawLine(x0, y, x, y);
                y = y0 + (int)(ySpanWidth*i*m_ptScaleInfo->intervalY);
                painter->drawLine(x0, y, x, y);
            }
        }
    }
}

void CScale::drawDivScaleLine(QPainter* painter, const QRect& rect)
{
    painter->setPen(QColor(m_clScaleValColor));
    //xÖá 
    if (m_bShowDivScaleLineX) 
    {
        int x0 = m_ptScaleInfo->GetCenterPoint().x();
        int y0 = m_ptScaleInfo->GetCenterPoint().y();
        int x = 0;

        if (m_nScaleLineDirX == NEGATIVE) {

        }
        else if (m_nScaleLineDirX == POSITIVE) {
            y0 -= m_nDivScaleLineLengthX;
        }
        else {
            y0 -= m_nDivScaleLineLengthX/2;
        }
        int y = y0+m_nDivScaleLineLengthX;

        float xSpanWidth = m_ptScaleInfo->GetSpanWidthX();
        if (m_ptScaleInfo->axisDirX == POSITIVE) {
            for(int i = m_ptScaleInfo->divIntervalX; i <= m_ptScaleInfo->gridCountX; i+=m_ptScaleInfo->divIntervalX) {
                x = x0 + (int)(i * xSpanWidth);
                painter->drawLine(x, y0, x, y);
            }
        }
        else if (m_ptScaleInfo->axisDirX == NEGATIVE) {
            for(int i = m_ptScaleInfo->divIntervalX; i <= m_ptScaleInfo->gridCountX; i+=m_ptScaleInfo->divIntervalX) {
                x = x0 - (int)(i * xSpanWidth);
                painter->drawLine(x, y0, x, y);
            }
        }
        else {
            for(int i = 1; i <= m_ptScaleInfo->gridCountX/m_ptScaleInfo->divIntervalX/2; i++) {
                x = x0 + (int)(xSpanWidth*i*m_ptScaleInfo->divIntervalX);
                painter->drawLine(x, y0, x, y);
                x = x0 - (int)(xSpanWidth*i*m_ptScaleInfo->divIntervalX);
                painter->drawLine(x, y0, x, y);
            }
        }
    }

    //yÖá 
    if (m_bShowDivScaleLineY) 
    {
        int x0 = m_ptScaleInfo->GetCenterPoint().x();
        int y0 = m_ptScaleInfo->GetCenterPoint().y();
        int y = 0;

        if (m_nScaleLineDirY == POSITIVE) {

        }
        else if (m_nScaleLineDirY == NEGATIVE) {
            x0 -= m_nDivScaleLineLengthY;
        }
        else {
            x0 -= m_nDivScaleLineLengthY/2;
        }
        int x = x0+m_nDivScaleLineLengthY;

        float ySpanWidth = m_ptScaleInfo->GetSpanWidthY();
        if (m_ptScaleInfo->axisDirY == POSITIVE) {
            for(int i = m_ptScaleInfo->divIntervalY; i <= m_ptScaleInfo->gridCountY; i+=m_ptScaleInfo->divIntervalY) {
                y = y0 - (int)(i * ySpanWidth);
                painter->drawLine(x0, y, x, y);
            }
        }
        else if (m_ptScaleInfo->axisDirY == NEGATIVE) {
            for(int i = m_ptScaleInfo->divIntervalY; i <= m_ptScaleInfo->gridCountY; i+=m_ptScaleInfo->divIntervalY) {
                y = y0 + (int)(i * ySpanWidth);
                painter->drawLine(x0, y, x, y);
            }
        }
        else {
            for(int i = 1; i <= m_ptScaleInfo->gridCountY/m_ptScaleInfo->divIntervalY/2; i++) {
                y = y0 - (int)(ySpanWidth*i*m_ptScaleInfo->divIntervalY);
                painter->drawLine(x0, y, x, y);
                y = y0 + (int)(ySpanWidth*i*m_ptScaleInfo->divIntervalY);
                painter->drawLine(x0, y, x, y);
            }
        }
    }
}





//
//-----------------------------------------------------------------------------
// »æÖÆÈý½ÇÐÎÍ¼±ê, combobox
//-----------------------------------------------------------------------------
//
void  CScale::drawTriangleIcon (QPainter* painter, const QPoint& center, int rad, TriangleIconDir dir, bool isClosed, bool isFill)
{
#define TRIANGLE_POINT_NUM  3

    // Ñ¡Ôñ»­±Ê
    painter->save();

    if (isFill) {
        painter->setBrush(QColor(m_clScaleValColor));
    }
    else {
        painter->setBrush(Qt::NoBrush);
    }

    // ¼ÆËãÎ»ÖÃ
    QRect  triangleRectUpDown(center.x()-rad, center.y()-rad/2, 2*rad, rad);
    QRect  triangleRectLeftRight(center.x()-rad/2, center.y()-rad, rad, 2*rad);
    QPoint  trianglePoints[TRIANGLE_POINT_NUM];
    switch (dir)
    {
    case TRI_LEFT:
        trianglePoints[0] = QPoint(triangleRectLeftRight.right(), triangleRectLeftRight.top());
        trianglePoints[1] = QPoint(triangleRectLeftRight.left(), triangleRectLeftRight.center().y());
        trianglePoints[2] = QPoint(triangleRectLeftRight.right(), triangleRectLeftRight.bottom());
        break;
    case TRI_UP:
        trianglePoints[0] = QPoint(triangleRectUpDown.left(), triangleRectUpDown.bottom());
        trianglePoints[1] = QPoint(triangleRectUpDown.center().x(), triangleRectUpDown.top());
        trianglePoints[2] = QPoint(triangleRectUpDown.right(), triangleRectUpDown.bottom());
        break;
    case TRI_RIGHT:
        trianglePoints[0] = QPoint(triangleRectLeftRight.left(), triangleRectLeftRight.top());
        trianglePoints[1] = QPoint(triangleRectLeftRight.right(), triangleRectLeftRight.center().y());
        trianglePoints[2] = QPoint(triangleRectLeftRight.left(), triangleRectLeftRight.bottom());
        break;
    case TRI_DOWN:
        trianglePoints[0] = QPoint(triangleRectUpDown.left(), triangleRectUpDown.top());
        trianglePoints[1] = QPoint(triangleRectUpDown.center().x(), triangleRectUpDown.bottom());
        trianglePoints[2] = QPoint(triangleRectUpDown.right(), triangleRectUpDown.top());
        break;
    }

    if (isClosed) {
        painter->drawPolygon(trianglePoints, TRIANGLE_POINT_NUM);
    }
    else {
        painter->drawPolyline(trianglePoints, TRIANGLE_POINT_NUM);
    }

    painter->restore();
}

void CScale::drawTriangle(QPainter* painter, const QRect& rect)
{
    painter->setPen(QColor(m_clScaleValColor));
    if (m_bShowXArrow) {
        if (m_ptScaleInfo->axisDirX == NEGATIVE) {
            drawTriangleIcon(painter, QPoint(rect.left()+m_ptScaleInfo->magin.left, m_ptScaleInfo->GetCenterPoint().y()), 5, TRI_LEFT, true, true);
        }
        else {
            drawTriangleIcon(painter, QPoint(rect.right()-m_ptScaleInfo->magin.right, m_ptScaleInfo->GetCenterPoint().y()), 5, TRI_RIGHT, true, true);
        }
    }
    else {
        int x, w;
        int y = m_ptScaleInfo->GetCenterPoint().y();
        if (m_ptScaleInfo->axisDirX == NEGATIVE) {
            x = rect.left();
            w = m_ptScaleInfo->magin.left;
        }
        else {
            x = rect.right()-m_ptScaleInfo->magin.right;
            w = m_ptScaleInfo->magin.right;
        }
        painter->drawLine(x, y, x+w, y);
    }

    if (m_bShowYArrow) {
        if (m_ptScaleInfo->axisDirY == NEGATIVE) {
            drawTriangleIcon(painter, QPoint(m_ptScaleInfo->GetCenterPoint().x(), rect.bottom()-m_ptScaleInfo->magin.bottom), 5, TRI_DOWN, true, true);
        }
        else {
            drawTriangleIcon(painter, QPoint(m_ptScaleInfo->GetCenterPoint().x(), rect.top()+m_ptScaleInfo->magin.top), 5, TRI_UP, true, true);
        }
    }
    else {
        int y, h;
        int x = m_ptScaleInfo->GetCenterPoint().x();
        if (m_ptScaleInfo->axisDirX == NEGATIVE) {
            y = rect.bottom()-m_ptScaleInfo->magin.bottom;
            h = m_ptScaleInfo->magin.bottom;
        }
        else {
            y = rect.top();
            h = m_ptScaleInfo->magin.top;
        }
        painter->drawLine(x, y, x, y+h);
    }
}

void CScale::drawScaleVal(QPainter* painter, const QRect& rect)
{
    QFontMetrics fontMetrics = painter->fontMetrics();
    int textH = qMax(m_scaleValPos.y(), fontMetrics.height());
    painter->setPen(QColor(m_clScaleValColor));
    //xÖá 
    if (m_bShowXScaleVal) 
    {
        int x0 = m_ptScaleInfo->GetCenterPoint().x();
        int y0 = m_ptScaleInfo->GetCenterPoint().y();
        int x = 0;
        QString str;

        if (m_ptScaleInfo->axisDirY == NEGATIVE) {
            y0 -= textH;

            if (m_nScaleLineDirX == NEGATIVE) {
                y0 -= m_nScaleLineLengthX;
            }
            else if (m_nScaleLineDirX == (NEGATIVE|POSITIVE)) {
                y0 -= m_nScaleLineLengthX/2;
            }
        }
        else {
            if (m_nScaleLineDirX == NEGATIVE) {
                y0 += m_nScaleLineLengthX;
            }
            else if (m_nScaleLineDirX == (NEGATIVE|POSITIVE)) {
                y0 += m_nScaleLineLengthX/2;
            }
        }

        float xSpanWidth = m_ptScaleInfo->GetSpanWidthX();
        if (m_ptScaleInfo->axisDirX == POSITIVE) {
            for(int i = m_ptScaleInfo->intervalX; i <= m_ptScaleInfo->gridCountX; i+=m_ptScaleInfo->intervalX) {
                x = x0 + (int)(i * xSpanWidth);
                str.setNum(i);
                int w = fontMetrics.width(str);
                painter->drawText(QRect(x-w/2, y0, w, textH), Qt::AlignCenter, str);
            }
        }
        else if (m_ptScaleInfo->axisDirX == NEGATIVE) {
            for(int i = m_ptScaleInfo->intervalX; i <= m_ptScaleInfo->gridCountX; i+=m_ptScaleInfo->intervalX) {
                x = x0 - (int)(i * xSpanWidth);
                str.setNum(-i);
                int w = fontMetrics.width(str);
                painter->drawText(QRect(x-w/2, y0, w, textH), Qt::AlignCenter, str);
            }
        }
        else  {
            int nInterval = 0;
            for(int i = 1; i <= m_ptScaleInfo->gridCountX/m_ptScaleInfo->intervalX/2; i++) {
                nInterval = i*m_ptScaleInfo->intervalX;
                x = x0 + (int)(xSpanWidth*nInterval);
                str.setNum(nInterval);
                int w = fontMetrics.width(str);
                painter->drawText(QRect(x-w/2, y0, w, textH), Qt::AlignCenter, str);

                x = x0 - (int)(xSpanWidth*nInterval);
                str.setNum(-nInterval);
                w = fontMetrics.width(str);
                painter->drawText(QRect(x-w/2, y0, w, textH), Qt::AlignCenter, str);
            }
        }
    }

    //yÖá 
    if (m_bShowYScaleVal) 
    {
        int x0 = m_ptScaleInfo->GetCenterPoint().x();
        int y0 = m_ptScaleInfo->GetCenterPoint().y();
        int y = 0;
        QString str;

        if (m_ptScaleInfo->axisDirX == NEGATIVE) {
            if (m_nScaleLineDirY == NEGATIVE) {
                x0 += m_nScaleLineLengthX ;
            }
            else if (m_nScaleLineDirY == (NEGATIVE|POSITIVE)) {
                x0 += m_nScaleLineLengthX/2;
            }
            x0 += 5;
        }
        else {
            x0 -= m_scaleValPos.x();
            if (m_nScaleLineDirY == NEGATIVE) {
                x0 -= m_nScaleLineLengthX;
            }
            else if (m_nScaleLineDirY == (NEGATIVE|POSITIVE)) {
                x0 -= m_nScaleLineLengthX/2;
            }
            x0 -= 5;
        }

        float ySpanWidth = m_ptScaleInfo->GetSpanWidthY();
        if (m_ptScaleInfo->axisDirY == POSITIVE) {
            for(int i = m_ptScaleInfo->intervalY; i <= m_ptScaleInfo->gridCountY; i+=m_ptScaleInfo->intervalY) {
                y = y0 - (int)(i * ySpanWidth);
                str.setNum(i);
                painter->drawText(QRect(x0, y-textH/2, m_scaleValPos.x(), textH), Qt::AlignRight|Qt::AlignVCenter, str);
            }
        }
        else if (m_ptScaleInfo->axisDirY == NEGATIVE) {
            for(int i = m_ptScaleInfo->intervalY; i <= m_ptScaleInfo->gridCountY; i+=m_ptScaleInfo->intervalY) {
                y = y0 + (int)(i * ySpanWidth);
                str.setNum(-i);
                painter->drawText(QRect(x0, y-textH/2, m_scaleValPos.x(), textH), Qt::AlignRight|Qt::AlignVCenter, str);
            }
        }
        else  {
            int nInterval = 0;
            for(int i = 1; i <= m_ptScaleInfo->gridCountY/m_ptScaleInfo->intervalY/2; i++) {
                nInterval = i*m_ptScaleInfo->intervalY;
                y = y0 - (int)(ySpanWidth*nInterval);
                str.setNum(nInterval);
                painter->drawText(QRect(x0, y-textH/2, m_scaleValPos.x(), textH), Qt::AlignRight|Qt::AlignVCenter, str);

                y = y0 + (int)(ySpanWidth*nInterval);
                str.setNum(-nInterval);
                painter->drawText(QRect(x0, y-textH/2, m_scaleValPos.x(), textH), Qt::AlignRight|Qt::AlignVCenter, str);
            }
        }
    }
}

void CScale::drawTitle(QPainter* painter, const QRect& rect)
{
    if (!m_bShowTitleX && !m_bShowTitleY) {
        return ;
    }

    painter->save();

    painter->setFont(m_cTitleFont);
    painter->setPen(QColor(m_clTitleColor));

    QFontMetrics fontMetrics = painter->fontMetrics();
    int textH = qMax(m_scaleValPos.y(), fontMetrics.height());

    if (m_ptTitlePosX.x() == -1 || m_ptTitlePosX.y() == -1) {
        if (m_ptScaleInfo->axisDirX == NEGATIVE) {
            m_ptTitlePosX.setX(rect.left() + m_ptScaleInfo->magin.left - fontMetrics.width(m_strTitleX) - 5);
        }
        else {
            m_ptTitlePosX.setX(rect.right() - m_ptScaleInfo->magin.right + 5);
        }
        m_ptTitlePosX.setY(m_ptScaleInfo->GetCenterPoint().y() - textH/2);
    }
    if (m_ptTitlePosY.x() == -1 || m_ptTitlePosY.y() == -1) {
        if (m_ptScaleInfo->axisDirY == NEGATIVE) {
            m_ptTitlePosY.setY(rect.bottom() - m_ptScaleInfo->magin.bottom + textH + 5);
        }
        else {
            m_ptTitlePosY.setY(rect.top() + m_ptScaleInfo->magin.top - textH - 5);
        }
        m_ptTitlePosY.setX(m_ptScaleInfo->GetCenterPoint().x() - fontMetrics.width(m_strTitleY)/2);
    }

    if (m_bShowTitleX) {
        painter->drawText(m_ptTitlePosX.x(), 
                          m_ptTitlePosX.y(), 
                          fontMetrics.width(m_strTitleX),
                          textH,
                          Qt::AlignCenter,
                          m_strTitleX);
    }
    if (m_bShowTitleY) {
        painter->drawText(m_ptTitlePosY.x(), 
                          m_ptTitlePosY.y(), 
                          fontMetrics.width(m_strTitleY),
                          textH,
                          Qt::AlignCenter,
                          m_strTitleY);
    }

    painter->restore();
}

