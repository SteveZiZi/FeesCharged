#pragma once

#include "inc/control/diagramCtrl/decorate.h"

#include <QtGui/QRgb>
#include <QtGui/QFont>


class CRectDecorator : public MDIAGRAM::CDecorator
{
public: 
    CRectDecorator(quint32 id, MDIAGRAM::CComponent* pComponent = NULL) 
        : MDIAGRAM::CDecorator(id, pComponent) 
    {
        x= 0; y = 0; w = 0; h = 0;
    }
    virtual ~CRectDecorator() {}

    //override
    virtual void Draw(QPainter* painter, const QRect& rect);

    void setGeometry(int ax, int ay, int aw, int ah) {
        this->x = ax;
        this->y = ay;
        this->w = aw;
        this->h = ah;
    }
    void setGeometry(const QRect &rect) {
        this->x = rect.left();
        this->y = rect.top();
        this->w = rect.width();
        this->h = rect.height();
    }

    void setBackgroundColor(const QRgb &clr) {this->clr = clr;}

private:
    int x;
    int y;
    int w;
    int h;
    QRgb clr;
};

inline void CRectDecorator::Draw(QPainter* painter, const QRect&)
{
    painter->save();
    painter->setPen(QPen(QColor(this->clr), WFG_WAVE_LINE_WIDTH, WFG_WAVE_LINE_STYLE));
    painter->setBrush(QBrush(QColor(this->clr)));
    painter->drawRect(x, y, w, h);
    painter->restore();
}