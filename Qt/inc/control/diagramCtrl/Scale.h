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

        //����Ҫ��һ��ScaleInfo�ṹ��ָ�� 
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

        // �����η���
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

        //����
        QString m_strTitleX;                                            //  X�����
        QString m_strTitleY;                                            //  Y�����
        QPoint  m_ptTitlePosX;                                          //  X�����λ��
        QPoint  m_ptTitlePosY;
        bool    m_bShowTitleX;                                          //  �Ƿ���ʾ����
        bool    m_bShowTitleY;
        //�̶ȳ�
        int     m_nScaleLineDirX;                                       //  �̶��߷���
        int     m_nScaleLineDirY;
        int     m_nScaleLineLengthX;                                    //  �̶��ߵĳ���
        int     m_nScaleLineLengthY;
        int     m_nDivScaleLineLengthX;                                 //  С�̶��ߵĳ���
        int     m_nDivScaleLineLengthY;
        bool    m_bShowScaleLineX;                                      //  �Ƿ���ʾ�̶���
        bool    m_bShowScaleLineY;
        bool    m_bShowDivScaleLineX;                                   //  �Ƿ���ʾС�̶���
        bool    m_bShowDivScaleLineY;
        bool    m_bShowXScaleVal;                                       //  �Ƿ���ʾ�̶�ֵ
        bool    m_bShowYScaleVal;
        bool    m_bShowXArrow;                                          //  �Ƿ���ʾ������ļ�ͷ
        bool    m_bShowYArrow;

        QFont   m_cTitleFont;                                          ///< ��������
        QFont   m_cScaleValFont;                                       ///< X-Y��ߵ�����
        QRgb    m_clTitleColor;                                        ///< ����ͼ�������ɫ
        QRgb    m_clScaleValColor;                                     ///< ����ͼ��X-Y��ߵĿ̶ȵ���ɫ

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