#pragma once

#include <QList>
#include <QtGui/QRgb>
#include "inc/control/diagramCtrl/diagramCommon.h"
#include "inc/control/diagramCtrl/decorate.h"

namespace MDIAGRAM
{
    template <class Tx, class Ty, int WAVE_NUM, int POINT_NUM>
    class CTrend : public CDecorator
    {
    public: 
        struct WaveData {
            Tx xData;
            Ty yData[WAVE_NUM];
        };

        CTrend(int id, CComponent* component) 
        : CDecorator(id, component)
        , m_ptScaleInfo(NULL)
        {
            m_pPointBuf = new QPoint[POINT_NUM];
        }

        virtual ~CTrend()
        {
            delete[] m_pPointBuf;
        }

        void Draw(QPainter* painter, const QRect& rect);
        void AppendData(const WaveData& data) {
            m_data.push_back(data);
        }
        bool ReplaceData(int pos, const WaveData& data)
        {
            if (pos < m_data.count()) {
                memcpy((void*)(&m_data.at(pos)), &data, sizeof(WaveData));
                return true;
            }
            return false;
        }
        void RemoveAll() {
            m_data.clear();
        }
        bool AccquireData(int pos, WaveData* data) {
            if (pos < m_data.count()) {
                memcpy(&data, &m_data.at(pos), sizeof(WaveData));
                return true;
            }
            return false;
        }
        const WaveData* endData() {
            Q_ASSERT(m_data.count() > 0);
            return &m_data.at(m_data.count()-1);
        }
        int DataCount() {
            return m_data.count();
        }

        void SetColor(int nItem, QRgb clr) {
            if (nItem < WAVE_NUM) {
                m_waveClr[nItem] = clr;
            }
        }


        //必须要挂一个ScaleInfo结构体指针 
        void AttachScale(ScaleInfo* ptScale) {  m_ptScaleInfo = ptScale; }

    private:
        typedef QList<WaveData> DATA_LIST;
        DATA_LIST m_data;
        ScaleInfo* m_ptScaleInfo;
        QRgb m_waveClr[WAVE_NUM];
        QPoint  *m_pPointBuf;            ///< 指向待绘制波形的数据缓存区的指针
    };

    //override
    template<class Tx, class Ty, int WAVE_NUM, int POINT_NUM> 
    void CTrend<Tx, Ty, WAVE_NUM, POINT_NUM>::Draw(QPainter* painter, const QRect& rect)
    {
        Q_ASSERT(m_ptScaleInfo);

        GetComponent()->Draw(painter, rect);

        int len = (int)(m_data.count());
        int first = 0;
        if (len > POINT_NUM) {
            len = POINT_NUM;
        }
        else if (len == 0) {
            return ;
        }

        painter->save();

        WaveData* pData = NULL;
        QRect waveRect(rect);
        waveRect.adjust(m_ptScaleInfo->magin.left, m_ptScaleInfo->magin.top, 0-(m_ptScaleInfo->magin.right), 0-(m_ptScaleInfo->magin.bottom));

        QPoint center = m_ptScaleInfo->GetCenterPoint();
        float xSpanWidth = m_ptScaleInfo->GetSpanWidthX();
        float ySpanWidth = m_ptScaleInfo->GetSpanWidthY();
        for(int num = 0; num < WAVE_NUM; num++) {
            for(int i = 0; i < len; i++) {
                pData = &m_data[i];
                m_pPointBuf[i].setX((int)(pData->xData/m_ptScaleInfo->fScaleX*xSpanWidth) + center.x());
                m_pPointBuf[i].setY(center.y() - (int)(pData->yData[num]/m_ptScaleInfo->fScaleY*ySpanWidth));
                if (m_pPointBuf[i].x() > waveRect.right()) {
                    m_pPointBuf[i].setX(waveRect.right());
                }
                else if (m_pPointBuf[i].x() < waveRect.left()) {
                    m_pPointBuf[i].setX(waveRect.left());
                }
                if (m_pPointBuf[i].y() > waveRect.bottom()) {
                    m_pPointBuf[i].setY(waveRect.bottom());
                }
                else if (m_pPointBuf[i].y() < waveRect.top()) {
                    m_pPointBuf[i].setY(waveRect.top());
                }
            }

            // 绘制波形曲线
            painter->setPen(QPen(QColor(m_waveClr[num]), WFG_WAVE_LINE_WIDTH, WFG_WAVE_LINE_STYLE));
            painter->drawPolyline(m_pPointBuf,len);
        }
        painter->restore();
    }
};