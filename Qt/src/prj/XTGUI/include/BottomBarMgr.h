#pragma once

#include <QtGui/QWidget>

//
//-----------------------------------------------------------------------------
// CNaviMenuMgr
//
//  �������˵��Ĵ���
//-----------------------------------------------------------------------------
//
class CBottomBarMgrPrivate;
class CBottomBarMgr : public QWidget
{
    Q_OBJECT

public:
    CBottomBarMgr(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~CBottomBarMgr();

public:
    /*
     *  @Func:  setStatusColor
     *  @Desc:  ����״̬��˸����ɫֵ
     *  
     *  @Param[in]:
     *      defaultClr:   Ĭ������µ���ɫ
     *      splashClr:    ��˸����ɫ������˸����ɫ��Ĭ����ɫ��ͬʱ������ʵ�ֲ���˸
     */
    void setStatusColor(QRgb defaultClr, QRgb splashClr);

public slots:
    /*
     *  @Func:  showStatus
     *  @Desc:  ��ʾ״̬���ϵ�����״̬
     *  
     *  @Param[in]:
     *      text:   ״̬����
     *      timesS: ��Ҫ��ʾ��ʱ��  -1:һֱ��ʾ��֪�����µ�״̬����
     */
    void showStatus(const QString& text, int timesS);

protected slots:
    void updateTime();

private:
    CBottomBarMgrPrivate *m_data;
};