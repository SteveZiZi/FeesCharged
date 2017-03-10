#pragma once

#include "decorate.h"
#include <vector>

namespace MDIAGRAM
{
    class CForeground : public CComponent
    {
    public: 

        CForeground(int id) 
        : CComponent(id)
        {
        }

        virtual ~CForeground()
        {
        }

        void Draw(QPainter* painter, const QRect& rect) {}
    };

};