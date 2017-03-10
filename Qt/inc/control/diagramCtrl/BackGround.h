#pragma once

#include "decorate.h"
#include <QtGui/QPainter>

namespace MDIAGRAM
{
    class CBackGround : public CComponent
    {
    public: 
        CBackGround(quint32 id) : CComponent(id) {}
        ~CBackGround() {}

        //override
        void Draw(QPainter* painter, const QRect& rect) { painter->fillRect(rect, WFG_CTRL_BKGND_COLOR); }
    };

};