/*
 *  @bref:
 *      用电检测界面
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
     *  @Desc:  响应对应菜单按下的事件
     *  
     *  @Param[in]:
     *      NONE
     *  
     *  @Return:
     *      true:   菜单项处理完成，即使设置了菜单对应的切换页面也不会显示页面
     *      false:  设置了菜单对应的切换页面将会显示该页面
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
