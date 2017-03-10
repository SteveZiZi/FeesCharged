/** @file
 *  @brief �����ػ����̷�����չ
 *  @author 
 *  @date 2011/12/26
 *  @version 0.1
 *  @see mprocess/daemon.h
 */
#include "nwdaemon.h"
#include "context.h"
#include "baseio.h"
#include "htrace.h"
#include "status.h"
#include "AlarmEventRecord.h"
#include "alarm.h"
#include "almfact.h"
#include "cfg_path.h"
#include "zonebase.h"
#include "metercom.h"
#include "version.h"
#include "testapi.h"

//
//
//
C_BATTERY::C_BATTERY(void)
{
}

C_BATTERY::~C_BATTERY()
{
}

/** @brief �����ж�
 *  @return true ϵͳ���磻false ϵͳδ����
 */
bool C_BATTERY::GetPowerOff(void)
{
	S_PowerStatus *pPowerStatus = CONTEXT.GetPowerStatus();
	return pPowerStatus->m_fPowerOff;
}

/** @brief ˢ�³��״̬
 *  @param[in] fCharge true ���ڳ��;false δ���
 */
int C_BATTERY::SetBatteryChargeStatus(bool fCharge)
{
	S_PowerStatus *pPowerStatus = CONTEXT.GetPowerStatus();
	pPowerStatus->m_fCharge = fCharge;
	return 0;
}

/** @brief ˢ�µ�ǰ��ص���
 *  @param[in] Capability ��ǰ��ص���
 */
int C_BATTERY::SetBatteryCapability(int Capability)
{
	S_PowerStatus *pPowerStatus = CONTEXT.GetPowerStatus();
	if (Capability >= 0)
		pPowerStatus->m_BatteryCap = Capability;
	return 0;
}

/** @brief ȡ�����С������
 *  @return �����С������
 */
int C_BATTERY::GetBatteryChargeLimit(void)
{
	return C_BatteryBase::GetBatteryChargeLimit();
}

/** @brief ȡ���������
 *  @return ���������
 */
int C_BATTERY::GetBatteryChargeFull(void)
{
	return C_BatteryBase::GetBatteryChargeFull();
}

/** @brief ȡ��س��ʱ��
 *  @return ��س��ʱ��
 *  @remarks ��λ����
 */
int C_BATTERY::GetBatteryChargeHours(void)
{
	return C_BatteryBase::GetBatteryChargeHours();
}


//
//
//
C_NwDaemonServer::C_NwDaemonServer(void)
{
	m_tickPowerCut = 0x00;
	m_CtrlAlarmLed = 0; ///< �澯�ƿ���
}

C_NwDaemonServer::~C_NwDaemonServer()
{
}

/** @brief ���������ڷ������ */
int C_NwDaemonServer::BeforeService(void)
{
	if (C_DaemonServer::BeforeService() != 0)
		return -1;
	S_SYSCFG *pSysCfg = STATUS.GetSystemConfig();
	if (pSysCfg->m_InitIRDA == true)
		io_InitIRDA();

	C_MeterCom MeterCom;
	MeterCom.SetFillChar((char)0xFF);

	C_AlarmFactory::InitAlarmStatus();

	FreshSoftVersion();	/** @brief ���汾�ű�� */
	FreshZoneInformation(); /** @brief ˢ��������Ϣ*/
	STATUS.IncResetTimes();	/** @brief �ն˸�λ����ͳ��*/
	m_Battery.InitDevice();

	/* �ն��޵��ͣ�ϵ���*/
	CheckPowerCutAlarm();
	return 0;
}

/** @brief ���5������ */
int C_NwDaemonServer::CheckTask5S(void)
{
	m_Battery.BatteryCharge();

	return C_DaemonServer::CheckTask5S();
}

/** @brief ���2������ */
int C_NwDaemonServer::CheckTask2S(void)
{
	CheckUsbDiskPlugin();
	CheckBatteryDisable();
	CheckNetLinePlugin();
	return C_DaemonServer::CheckTask2S();
}

/** @brief ���1������ */
int C_NwDaemonServer::CheckTask1S(void)
{
	S_AlarmStatus *pAS = STATUS.GetAlarmStatus();
	if (pAS->m_ImportantNewCnt > 0 || pAS->m_NormalNewCnt > 0)
		m_CtrlAlarmLed = !m_CtrlAlarmLed;
	else
		m_CtrlAlarmLed = 0;
	io_AlarmLedLight(m_CtrlAlarmLed);

	CheckPowerOff(); // һ����һ��
	return C_DaemonServer::CheckTask1S();
}

/** @������Ϣ֪ͨ */
bool C_NwDaemonServer::UpdateNotify(int msg)
{
	S_LcdStatus *pLcdStatus = CONTEXT.GetLcdStatus();

	pLcdStatus->m_UpdateInfo = msg;
	if (msg == DYJC_UPDATE_BEGIN)
		pLcdStatus->m_UpdateDlog = true;
	return true;
}

int C_NwDaemonServer::CheckUsbDiskPlugin(void)
{
#define USB_DISK_FLAG "KGEDUMMY.FLG"
	char FlagFile[260];
	S_LcdStatus *pLcdStatus = CONTEXT.GetLcdStatus();

	sprintf(FlagFile, "%s%s", C_USBPATH::GetUsbRoot(), USB_DISK_FLAG);
	if (access(FlagFile, F_OK)  != 0)//Check Usb Disk
	{
		pLcdStatus->m_UsbDiskPlugin = false;
		pLcdStatus->m_UsbCopyFile = false;
	}
	else
	{
		if (pLcdStatus->m_UsbDiskPlugin == false) // U�̸ղ���
			pLcdStatus->m_UsbCopyFile = true;
		pLcdStatus->m_UsbDiskPlugin = true;
	}
	return 0;
}

int C_NwDaemonServer::CheckNetLinePlugin(void)
{
	S_LcdStatus *pLcdStatus = CONTEXT.GetLcdStatus();
	pLcdStatus->m_Eth0LinePlugin = C_BaseFn::CheckNetLinePlugin("eth0");
	return 0;
}

int C_NwDaemonServer::CheckPowerOff(void)
{
	int state = io_GetPowerState();
	if (state < 0)
		return 0;

	bool PowerOff = state? false:true;
	S_PowerStatus *pPowerStatus = CONTEXT.GetPowerStatus();
	if (pPowerStatus->m_fPowerOff == PowerOff)
		return 0;
	
	pPowerStatus->m_fPowerOff = PowerOff;
	S_SYSRUN *pSysRun = STATUS.GetSysRunInfo();
	if (state == 1)
	{
		m_tickPowerCut = 0x00; // ����е��־
		if (pSysRun->m_timePowerCut != 0x00)
			Alarm_PowerOn(pSysRun->m_timePowerCut);
		pSysRun->m_timePowerCut = 0x00;			
		logvt<<"ϵͳ�ϵ�..."<<endl;//power on
	}
	else if (state == 0)
	{
		m_tickPowerCut = CONTEXT.GetSystemTick(); // �����е��־
		pSysRun->m_timePowerCut = C_TIME::TimeNow();
		pSysRun->m_cntPowerCut = 0; // ����е����
		Alarm_PowerDown();
		SaveSystemStatus();
		C_FILE::SyncFileSystem();
		logvt<<"ϵͳ����..."<<endl;//power off
	}
	STATUS.SaveSysRunInfo();
	C_FILE::SyncFileSystem(); //again
	return 0;
}

int C_NwDaemonServer::Alarm_PowerOn(S_TIME timePowerCut)
{
	S_AlarmInfo *pAlarmInfo = STATUS.GetAlarmInfo();
	CAlarm *pAlarm = C_AlarmFactory::GetAlarmEventObj((INT8U)(ALARM_TERMINAL_POWERON));
	if (pAlarm == NULL || pAlarmInfo->m_AlarmEven.test(ALARM_TERMINAL_POWERON) == false)
	{
		loget<<" �ն�ͣ�ϵ�澯���󴴽�ʧ��"<<endl;
	}
	else
	{
		pAlarm->Detect();
	}
	logvt<<"�ն�ͣ�ϵ��¼�����"<<endl;
	CEventRecrod_ERD4::EventRecord(EVT_COMMID(EVENT_POWER_OFFON),timePowerCut,C_TIME::TimeNow());
	return 0;
}

int C_NwDaemonServer::Alarm_PowerDown(void)
{
	S_AlarmInfo *pAlarmInfo = STATUS.GetAlarmInfo();
	CAlarm *pAlarm = C_AlarmFactory::GetAlarmEventObj((INT8U)(ALARM_TERMINAL_POWEROFF));
	if (pAlarm == NULL || pAlarmInfo->m_AlarmEven.test(ALARM_TERMINAL_POWEROFF) == false)
	{
		loget<<" �ն�ͣ�ϵ�澯���󴴽�ʧ��"<<endl;
	}
	else
	{
		logvt<<"�ն˵���澯����"<<endl;
		pAlarm->Detect();
	}
	return 0;
}

int C_NwDaemonServer::CheckBatteryDisable(void)
{
///#define CONS_BATTERY_DISABLE_DELAY_S	180
	if (m_tickPowerCut == 0x00)
		return 0;

	S_SYSCFG *pSysCfg = STATUS.GetSystemConfig();
	S_SYSRUN *pSysRun = STATUS.GetSysRunInfo();
	S_PowerStatus *pPowerStatus = CONTEXT.GetPowerStatus();

	S_TIME tickNow = CONTEXT.GetSystemTick();
	INT16U CONS_BATTERY_DISABLE_DELAY_S = pSysCfg->m_BatteryStandTimeM*60;

	if ((pPowerStatus->m_BatteryCap == 0x00 && tickNow - m_tickPowerCut > pSysCfg->m_TermStandTimeS)  // �޵��������е�
		|| (tickNow - m_tickPowerCut > CONS_BATTERY_DISABLE_DELAY_S)) // ����������е�
	{
		if (pPowerStatus->m_BatteryCap == 0x00)
			logwt<<"�ն˵���"<<pSysCfg->m_TermStandTimeS<<"��,�رյ�ع���..."<<endl;
		else
			logwt<<"�ն˵���"<<CONS_BATTERY_DISABLE_DELAY_S<<"��,�رյ�ع���..."<<endl;
		KillServers(); // �ص�ֹͣ����
		pSysRun->m_cntPowerCut += 1; // �е����+1
		SaveSystemStatus();
		C_FILE::SyncFileSystem();
		C_FILE::SyncFileSystem(); // again
		SleepSecond(1); // ��ʱһ�µ��������
		io_BatteryEnable(false);
	}
	return 0;
}

bool C_NwDaemonServer::CheckPowerCutAlarm(void)
{
	S_SYSRUN *pSysRun = STATUS.GetSysRunInfo();

	if (pSysRun->m_timePowerCut == 0x00)
		return false;

	CAlarm *pAlarm = C_AlarmFactory::GetAlarmEventObj((INT8U)(ALARM_TERMINAL_POWERON));
	S_AlarmInfo *pAlarmInfo = STATUS.GetAlarmInfo();
	
	if (pAlarm == NULL || pAlarmInfo == NULL || pAlarmInfo->m_AlarmEven.test(ALARM_TERMINAL_POWERON) == false)
	{
		loget<<" �ն�ͣ�ϵ�澯���󴴽�ʧ��"<<endl;
	}
	else
	{
		logwt<<"�ε���ϵ�澯����"<<endl;
		pAlarm->Detect();
	}
	logvt<<"�ն�ͣ�ϵ��¼�����"<<endl;
	CEventRecrod_ERD4::EventRecord(EVT_COMMID(EVENT_POWER_OFFON),pSysRun->m_timePowerCut,C_TIME::TimeNow());

	pSysRun->m_timePowerCut = 0x00;
	STATUS.SaveSysRunInfo();
	return true;
}

bool C_NwDaemonServer::FreshSoftVersion(void)
{
	S_SYSRUN *pSysRun = STATUS.GetSysRunInfo();
	/* Version compare
	 */
	if (strcmp(pSysRun->m_SWVersion, CONS_SOFT_VERSION) == 0)//same version
	{
		if (strcmp(pSysRun->m_MPVersion, CONS_MAIN_VERSION) != 0)
		{
			logwt<<"������汾�ű��:"<<pSysRun->m_MPVersion<<" -> "<<CONS_MAIN_VERSION<<endl;
			strcpy(pSysRun->m_MPVersion, CONS_MAIN_VERSION);//fresh main program version
			strcpy(pSysRun->m_ReleaseDate, CONS_SW_REAL_DATE);
			STATUS.SaveSysRunInfo();
		}
		else if (strcmp(pSysRun->m_ReleaseDate, CONS_SW_REAL_DATE) != 0)
		{
			strcpy(pSysRun->m_ReleaseDate, CONS_SW_REAL_DATE);
			STATUS.SaveSysRunInfo();
		}
		return true;
	}
	/* Version change alarm
	 */
//	C_AlarmEvent *pAlarmEvent = C_AlarmFactory::GetAlarmEventObj(1);
//	if (pAlarmEvent != NULL && pAlarmEvent->NeedRecord() == true)
//	{
//		S_DataERC001 DataErc001;
//		DataErc001.m_DataInit = false;
//		DataErc001.m_pCurVer = CONS_SOFT_VERSION;
//		DataErc001.m_pPreVer = pSysRun->m_SWVersion;
//		pAlarmEvent->Action((void *)&DataErc001, 0);
//	}

	/* fresh version
	 */
	strcpy(pSysRun->m_MPVersion, CONS_MAIN_VERSION);//fresh main program version
	strcpy(pSysRun->m_SWVersion, CONS_SOFT_VERSION);
	strcpy(pSysRun->m_ReleaseDate, CONS_SW_REAL_DATE);
	STATUS.SaveSysRunInfo();
	return true;
}

bool C_NwDaemonServer::FreshZoneInformation(void)
{
	C_ZoneVersion *pZoneVersion = C_ZoneVersion::GetZoneVersionObject();
	if (pZoneVersion != NULL)
	{
		logwt<<"����������:"<<pZoneVersion->GetZonePluginNameE()<<endl;
		logwt<<"�������汾:"<<pZoneVersion->GetZonePluginVersion()<<endl;
		logwt<<"��������������:"<<pZoneVersion->GetZonePluginReleaseDate()<<endl;
	}

	S_SYSCFG *pSysCfg = STATUS.GetSystemConfig();
	if (strcmp(pSysCfg->m_ZoneName, pZoneVersion->GetZonePluginNameC()) != 0)
	{
		strncpy(pSysCfg->m_ZoneName, pZoneVersion->GetZonePluginNameC(), sizeof(pSysCfg->m_ZoneName)-4);
		STATUS.SaveSystemConfig();
	}
	return true;
}


