#pragma once

#include "diagramCommon.h"
#include "decorate.h"
#include <QtGui/QRgb>
#include <QtGui/QFont>

namespace MDIAGRAM
{
    class CScale : public CDecorator
    {
    public: 
        CScale(quint32 id, CComponent* pComponent = NULL, int xAxisType = POSITIVE, int yAxisType = POSITIVE);
        virtual ~CScale();

        //必须要挂一个ScaleInfo结构体指针 
        void AttachScale(ScaleInfo* ptScale);

        void SetMargin(const int left, const int right, const int top, const int bottom);
        void SetMargin(const int x, const int y);

        void SetTitle(const QString &xTitle, const QString &yTitle);
        void SetTitlePos(const QPoint &xPos, const QPoint &yPos);
        void SetTitleColor(QRgb clr);
        void SetTitleFont(QFont font);

        void SetShowTitle(bool bShowX, bool bShowY);
        void SetShowScaleVal(bool bShowX, bool bShowY);
        void SetShowScaleLine(bool bShowX, bool bShowY);
        void SetShowDivScaleLine(bool bShowX, bool bShowY);

        void SetScaleLineDir(int xDir, int yDir);
        void SetScaleValFont(QFont font);
        void SetScaleValColor(QRgb clr);

        void SetScaleValPos(const QPoint &pos) {m_scaleValPos = pos;}
        void setScaleValPos(int x = -1, int y = -1) {
            if (x != -1)
                m_scaleValPos.setX(x); 
            if (y != -1)
                m_scaleValPos.setY(y);
        }

        void setShowArrow(bool bShowX, bool bShowY);

        //override
        virtual void Draw(QPainter* painter, const QRect& rect);

    private:
        void drawTitle(QPainter* painter, const QRect& rect);
        void drawScaleLine(QPainter* painter, const QRect& rect);
        void drawDivScaleLine(QPainter* painter, const QRect& rect);
        void drawScaleVal(QPainter* painter, const QRect& rect);
        void drawTriangle(QPainter* painter, const QRect& rect);

        // 三角形方向
        enum TriangleIconDir
        {
            TRI_LEFT,
            TRI_UP,
            TRI_RIGHT,
            TRI_DOWN
        };
        void  drawTriangleIcon (QPainter* painter, const QPoint& center, int rad, TriangleIconDir dir, bool isClosed, bool isFill);

    protected:
        ScaleInfo* m_ptScaleInfo;

        //标题
        QString m_strTitleX;                                            //  X轴标题
        QString m_strTitleY;                                            //  Y轴标题
        QPoint  m_ptTitlePosX;                                          //  X轴标题位置
        QPoint  m_ptTitlePosY;
        bool    m_bShowTitleX;                                          //  是否显示标题
        bool    m_bShowTitleY;
        //刻度尺
        int     m_nScaleLineDirX;                                       //  刻度线方向
        int     m_nScaleLineDirY;
        int     m_nScaleLineLengthX;                                    //  刻度线的长度
        int     m_nScaleLineLengthY;
        int     m_nDivScaleLineLengthX;                                 //  小刻度线的长度
        int     m_nDivScaleLineLengthY;
        bool    m_bShowScaleLineX;                                      //  是否显示刻度线
        bool    m_bShowScaleLineY;
        bool    m_bShowDivScaleLineX;                                   //  是否显示小刻度线
        bool    m_bShowDivScaleLineY;
        bool    m_bShowXScaleVal;                                       //  是否显示刻度值
        bool    m_bShowYScaleVal;
        bool    m_bShowXArrow;                                          //  是否显示坐标轴的箭头
        bool    m_bShowYArrow;

        QFont   m_cTitleFont;                                          ///< 标题字体
        QFont   m_cScaleValFont;                                       ///< X-Y标尺的字体
        QRgb    m_clTitleColor;                                        ///< 波形图标题的颜色
        QRgb    m_clScaleValColor;                                     ///< 波形图的X-Y标尺的刻度的颜色

        QPoint  m_scaleValPos;
    };

    inline void CScale::AttachScale(ScaleInfo* ptScale)
    {
        m_ptScaleInfo = ptScale;
    }

    inline void CScale::SetTitle(const QString &xTitle, const QString &yTitle) 
    {
        m_strTitleX = xTitle;
        m_strTitleY = yTitle;
    }
    inline void CScale::SetTitlePos(const QPoint &xPos, const QPoint &yPos)
    {
        m_ptTitlePosX = xPos;
        m_ptTitlePosY = yPos;
    }

    inline void CScale::SetShowScaleVal(bool bShowX, bool bShowY)
    {
        m_bShowXScaleVal = bShowX;
        m_bShowYScaleVal = bShowY;
    }
    inline void CScale::SetShowScaleLine(bool bShowX, bool bShowY)
    {
        m_bShowScaleLineX = bShowX;
        m_bShowScaleLineY = bShowY;
    }
    inline void CScale::SetShowDivScaleLine(bool bShowX, bool bShowY) 
    {
        m_bShowDivScaleLineX = bShowX;
        m_bShowDivScaleLineY = bShowY;
    }

    inline void CScale::SetScaleLineDir(int xDir, int yDir)
    {
        m_nScaleLineDirX = xDir;
        m_nScaleLineDirY = yDir;
    }

    inline void CScale::SetTitleFont(QFont font)
    {
        m_cTitleFont = font;
    }
    inline void CScale::SetTitleColor(QRgb clr)
    {
        m_clTitleColor = clr;
    }

    inline void CScale::SetScaleValFont(QFont font)
    {
        m_cScaleValFont = font;
    }
    inline void CScale::SetScaleValColor(QRgb clr)
    {
        m_clScaleValColor = clr;
    }

    inline void CScale::SetShowTitle(bool bShowX, bool bShowY)
    {
        m_bShowTitleX = bShowX;
        m_bShowTitleY = bShowY;
    }

    inline void CScale::setShowArrow(bool bShowX, bool bShowY)
    {
        m_bShowXArrow = bShowX;
        m_bShowYArrow = bShowY;
    }
};