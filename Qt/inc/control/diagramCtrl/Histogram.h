#pragma once

#include <QtCore/QtGlobal>
#include <QtGui/QColor>
#include <QMap>

#include "inc/control/diagramCtrl/decorate.h"

namespace MDIAGRAM
{

    struct PillarItemInfo {
        quint32 width;      //���ӿ��ռ�õ����ص�
        qreal  height;      //������ʵ�ĸ߶�ֵ 
        QRgb   graphClr;    //���ӵ���ɫ
        QRgb   textClr;     //�����Ϸ���ʾ�ı�����ɫ
        QString unit;       //��Ҫ��ʾ�ĵ�λ
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