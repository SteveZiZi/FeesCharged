/// @file
///     diagramCtrl.cpp
///
/// @brief
///     ͼ��ؼ�
///
/// @note
///
/// Copyright (c) 
///
/// ���ߣ�
///    yangyuchao  2015.10.23
///
/// �汾��
///    1.0.x.x
/// 
/// �޸���ʷ��
///    �� ʱ��         : �汾      :  ��ϸ��Ϣ    
///    :-------------- :-----------:----------------------------------------------------------
///    |2015.10.23     |1.0.000    |������
///
#include "inc/control/diagramCtrl/diagramCtrl.h"
#include "../../encrypt/encrypt.h"
#include <QtGui/QPainter>

using namespace MDIAGRAM;

///
/// @brief
///     ���캯��
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
///     ��������
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
///     ��ʼ���ؼ�
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
///     ����������ʾ�����ȫ��������Ϣ
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
///     �ػ���Ϣ������
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
///     ���ƿؼ�����
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
///     ���ÿؼ�ǰ��
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

