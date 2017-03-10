/// @file
///     diagramCtrl.cpp
///
/// @brief
///     图表控件
///
/// @note
///
/// Copyright (c) 
///
/// 作者：
///    yangyuchao  2015.10.23
///
/// 版本：
///    1.0.x.x
/// 
/// 修改历史：
///    ： 时间         : 版本      :  详细信息    
///    :-------------- :-----------:----------------------------------------------------------
///    |2015.10.23     |1.0.000    |创建类
///
#include "inc/control/diagramCtrl/diagramCtrl.h"
#include "../../encrypt/encrypt.h"
#include <QtGui/QPainter>

using namespace MDIAGRAM;

///
/// @brief
///     构造函数
///
CDiagramCtrl::CDiagramCtrl(CComponent* bg, CComponent* fg, QWidget* parent)
: QWidget(parent)
, m_pBackGround(bg)
, m_pForeGround(fg)
, m_backgroundPixmap(NULL)
, m_foregroundPixmap(NULL)
{
}

///
/// @brief
///     析构函数
///
CDiagramCtrl::~CDiagramCtrl()
{
    if (m_pBackGround) {
        delete m_pBackGround;
        m_pBackGround = NULL;
    }
    if (m_pForeGround) {
        delete m_pForeGround;
        m_pForeGround = NULL;
    }

    if (m_backgroundPixmap) {
        delete m_backgroundPixmap;
        m_backgroundPixmap = NULL;
    }
    if (m_foregroundPixmap) {
        delete m_foregroundPixmap;
        m_foregroundPixmap = NULL;
    }
}

///
/// @brief
///     初始化控件
///
/// @return
///
///
void CDiagramCtrl::Init()
{
    if (!verify_encrypt()) {
        return ;
    }
    DrawBkgnd();
    ResetForgnd();
}

///
/// @brief
///     擦除波形显示区域的全部波形信息
///
/// @return
///
///
void CDiagramCtrl::EraseWave()
{
    ResetForgnd();
}

///
/// @brief
///     重绘消息处理函数
///
/// @return
///
void CDiagramCtrl::paintEvent(QPaintEvent *event)
{
    if (m_foregroundPixmap) {
        QPainter painter(this);
        painter.drawPixmap(0, 0, *m_foregroundPixmap);
    }
    else {
        QWidget::paintEvent(event);
    }
}

///
/// @brief
///     绘制控件背景
///
/// @return
///
void CDiagramCtrl::DrawBkgnd()
{
    if (m_pBackGround) {
        if (!m_backgroundPixmap) {
            m_backgroundPixmap = new QPixmap(size());
        }
        QPainter painter(m_backgroundPixmap);
        m_pBackGround->Draw(&painter, rect());
    }
}

///
/// @brief
///     重置控件前景
///
/// @return
///
void CDiagramCtrl::ResetForgnd()
{
    if (m_backgroundPixmap) {
        if (m_foregroundPixmap)  {
            *m_foregroundPixmap = *m_backgroundPixmap;
        }
        else {
            m_foregroundPixmap = new QPixmap(*m_backgroundPixmap);
        }
    }

}

void CDiagramCtrl::DrawForegound()
{
    if (m_pForeGround && m_foregroundPixmap) {
        QPainter painter(m_foregroundPixmap);
        m_pForeGround->Draw(&painter, rect());
    }
}

