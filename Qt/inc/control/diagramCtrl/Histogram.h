#pragma once

#include <QtCore/QtGlobal>
#include <QtGui/QColor>
#include <QMap>

#include "inc/control/diagramCtrl/decorate.h"

namespace MDIAGRAM
{

    struct PillarItemInfo {
        quint32 width;      //柱子宽度占用的像素点
        qreal  height;      //柱子真实的高度值 
        QRgb   graphClr;    //柱子的颜色
        QRgb   textClr;     //柱子上方显示文本的颜色
        QString unit;       //需要显示的单位
    };

    class CHistogram : public CDecorator
    {
    public:
        typedef QMap<int, PillarItemInfo> mapPillarsInfo;


        CHistogram(int id, CComponent *component) : CDecorator(id, component) {}
        ~CHistogram() {}

        void Draw(QPainter* painter, const QRect& rect);
        void AttachScale(ScaleInfo* ptScale) {  m_ptScaleInfo = ptScale; }

        void setPillarItem(int item, const PillarItemInfo &pillarInfo);
        bool updatePillarInfo(int item, quint32 w);
        bool updatePillarInfo(int item, qreal h);
        bool updatePillarInfo(int item, const QRgb &clr);

    private:
        ScaleInfo* m_ptScaleInfo;
        mapPillarsInfo m_pillarsInfo;
    };

};