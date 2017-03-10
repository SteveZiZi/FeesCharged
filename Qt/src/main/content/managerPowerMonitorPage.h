/*
 *  @bref:
 *      �õ������
 */
#pragma once

#include "inc/baseContent.h"
#include "src/dataLevel/datamanager.h"

class CDataManager;
class CPowerMonitorReport;
class QLabel;
class CManagerPowerMonitorPage : public CBaseContent
{
public:
    CManagerPowerMonitorPage(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~CManagerPowerMonitorPage(void);

    enum {
        BTN_UP = 0,
        BTN_DOWN,
        BTN_BACK =3
    };

    virtual void initUI();

protected:
    /*
     *  @Func:  OnF1Press ~ OnF4Press
     *  @Desc:  ��Ӧ��Ӧ�˵����µ��¼�
     *  
     *  @Param[in]:
     *      NONE
     *  
     *  @Return:
     *      true:   �˵������ɣ���ʹ�����˲˵���Ӧ���л�ҳ��Ҳ������ʾҳ��
     *      false:  �����˲˵���Ӧ���л�ҳ�潫����ʾ��ҳ��
     */
    virtual bool OnF1Press();
    virtual bool OnF2Press();

private:
    void initMenu();

    void createSerialLabel();
    void createPowerMonitorReport();

    void updateSerialLabel(int index);
    void updatePowerMonitorReport(int index);

private:
    CDataManager*               m_pDataMgr;
    CPowerMonitorReport*        m_pPowerMonitorReport;
    QLabel*                     m_pSerialLabel;

    QList<DM_PowerMonitorInfo>  m_lPowerMonitorInfoList;
    QList<DM_DeviceNum>         m_lDeviceNumList;

    int                         m_iCurrentIndex;
};
