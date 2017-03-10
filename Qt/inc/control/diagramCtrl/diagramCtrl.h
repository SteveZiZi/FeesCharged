#pragma once

#include "decorate.h"
#include <QtGui/QWidget>
#include "decorate.h"

namespace MDIAGRAM
{
    class CDiagramCtrl : public QWidget
    {
        Q_OBJECT

    public:
        CDiagramCtrl(CComponent* bg = NULL, CComponent* fg = NULL, QWidget* parent = 0);
        virtual ~CDiagramCtrl();

        ///
        /// @brief
        ///     初始化波形图控件
        ///
        void Init();

        void SetComponent(CComponent* bg, CComponent* fg) {
            if (m_pBackGround ) {
                delete m_pBackGround;
            }
            if (m_pForeGround) {
                delete m_pForeGround;
            }
            m_pBackGround = bg;
            m_pForeGround = fg;
        }

        ///
        /// @brief
        ///     擦除波形显示区域的全部波形信息
        ///
        void EraseWave();
        void ResetForgnd();
        void DrawBkgnd();
        void DrawForegound();

    protected:
        virtual void paintEvent(QPaintEvent *event);

    private:
        CComponent* m_pBackGround;
        CComponent* m_pForeGround;

        QPixmap    *m_backgroundPixmap; 
        QPixmap    *m_foregroundPixmap; 
    };
};
