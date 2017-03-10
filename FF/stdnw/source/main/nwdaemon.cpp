/** @file
 *  @brief 南网守护进程服务扩展
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

/** @brief 掉电判断
 *  @return true 系统掉电；false 系统未掉电
 */
bool C_BATTERY::GetPowerOff(void)
{
	S_PowerStatus *pPowerStatus = CONTEXT.GetPowerStatus();
	return pPowerStatus->m_fPowerOff;
}

/** @brief 刷新充电状态
 *  @param[in] fCharge true 正在充电;false 未充电
 */
int C_BATTERY::SetBatteryChargeStatus(bool fCharge)
{
	S_PowerStatus *pPowerStatus = CONTEXT.GetPowerStatus();
	pPowerStatus->m_fCharge = fCharge;
	return 0;
}

/** @brief 刷新当前电池电量
 *  @param[in] Capability 当前电池电量
 */
int C_BATTERY::SetBatteryCapability(int Capability)
{
	S_PowerStatus *pPowerStatus = CONTEXT.GetPowerStatus();
	if (Capability >= 0)
		pPowerStatus->m_BatteryCap = Capability;
	return 0;
}

/** @brief 取电池最小充电电量
 *  @return 电池最小充电电量
 */
int C_BATTERY::GetBatteryChargeLimit(void)
{
	return C_BatteryBase::GetBatteryChargeLimit();
}

/** @brief 取电池满电量
 *  @return 电池满电量
 */
int C_BATTERY::GetBatteryChargeFull(void)
{
	return C_BatteryBase::GetBatteryChargeFull();
}

/** @brief 取电池充电时间
 *  @return 电池充电时间
 *  @remarks 单位：秒
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
	m_CtrlAlarmLed = 0; ///< 告警灯控制
}

C_NwDaemonServer::~C_NwDaemonServer()
{
}

/** @brief 本函数先于服务调用 */
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

	FreshSoftVersion();	/** @brief 检查版本号变更 */
	FreshZoneInformation(); /** @brief 刷新区域信息*/
	STATUS.IncResetTimes();	/** @brief 终端复位次数统计*/
	m_Battery.InitDevice();

	/* 终端无电池停上电检测*/
	CheckPowerCutAlarm();
	return 0;
}

/** @brief 检查5秒任务 */
int C_NwDaemonServer::CheckTask5S(void)
{
	m_Battery.BatteryCharge();

	return C_DaemonServer::CheckTask5S();
}

/** @brief 检查2秒任务 */
int C_NwDaemonServer::CheckTask2S(void)
{
	CheckUsbDiskPlugin();
	CheckBatteryDisable();
	CheckNetLinePlugin();
	return C_DaemonServer::CheckTask2S();
}

/** @brief 检查1秒任务 */
int C_NwDaemonServer::CheckTask1S(void)
{
	S_AlarmStatus *pAS = STATUS.GetAlarmStatus();
	if (pAS->m_ImportantNewCnt > 0 || pAS->m_NormalNewCnt > 0)
		m_CtrlAlarmLed = !m_CtrlAlarmLed;
	else
		m_CtrlAlarmLed = 0;
	io_AlarmLedLight(m_CtrlAlarmLed);

	CheckPowerOff(); // 一秒钟一次
	return C_DaemonServer::CheckTask1S();
}

/** @升级消息通知 */
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
		if (pLcdStatus->m_UsbDiskPlugin == false) // U盘刚插入
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
		m_tickPowerCut = 0x00; // 清除切电标志
		if (pSysRun->m_timePowerCut != 0x00)
			Alarm_PowerOn(pSysRun->m_timePowerCut);
		pSysRun->m_timePowerCut = 0x00;			
		logvt<<"系统上电..."<<endl;//power on
	}
	else if (state == 0)
	{
		m_tickPowerCut = CONTEXT.GetSystemTick(); // 设置切电标志
		pSysRun->m_timePowerCut = C_TIME::TimeNow();
		pSysRun->m_cntPowerCut = 0; // 清除切电次数
		Alarm_PowerDown();
		SaveSystemStatus();
		C_FILE::SyncFileSystem();
		logvt<<"系统掉电..."<<endl;//power off
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
		loget<<" 终端停上电告警对象创建失败"<<endl;
	}
	else
	{
		pAlarm->Detect();
	}
	logvt<<"终端停上电事件产生"<<endl;
	CEventRecrod_ERD4::EventRecord(EVT_COMMID(EVENT_POWER_OFFON),timePowerCut,C_TIME::TimeNow());
	return 0;
}

int C_NwDaemonServer::Alarm_PowerDown(void)
{
	S_AlarmInfo *pAlarmInfo = STATUS.GetAlarmInfo();
	CAlarm *pAlarm = C_AlarmFactory::GetAlarmEventObj((INT8U)(ALARM_TERMINAL_POWEROFF));
	if (pAlarm == NULL || pAlarmInfo->m_AlarmEven.test(ALARM_TERMINAL_POWEROFF) == false)
	{
		loget<<" 终端停上电告警对象创建失败"<<endl;
	}
	else
	{
		logvt<<"终端掉电告警产生"<<endl;
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

	if ((pPowerStatus->m_BatteryCap == 0x00 && tickNow - m_tickPowerCut > pSysCfg->m_TermStandTimeS)  // 无电池情况下切电
		|| (tickNow - m_tickPowerCut > CONS_BATTERY_DISABLE_DELAY_S)) // 正常情况下切电
	{
		if (pPowerStatus->m_BatteryCap == 0x00)
			logwt<<"终端掉电"<<pSysCfg->m_TermStandTimeS<<"秒,关闭电池供电..."<<endl;
		else
			logwt<<"终端掉电"<<CONS_BATTERY_DISABLE_DELAY_S<<"秒,关闭电池供电..."<<endl;
		KillServers(); // 关电停止功能
		pSysRun->m_cntPowerCut += 1; // 切电次数+1
		SaveSystemStatus();
		C_FILE::SyncFileSystem();
		C_FILE::SyncFileSystem(); // again
		SleepSecond(1); // 延时一下等任务完成
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
		loget<<" 终端停上电告警对象创建失败"<<endl;
	}
	else
	{
		logwt<<"拔电池上电告警产生"<<endl;
		pAlarm->Detect();
	}
	logvt<<"终端停上电事件产生"<<endl;
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
			logwt<<"主程序版本号变更:"<<pSysRun->m_MPVersion<<" -> "<<CONS_MAIN_VERSION<<endl;
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
		logwt<<"区域插件名称:"<<pZoneVersion->GetZonePluginNameE()<<endl;
		logwt<<"区域插件版本:"<<pZoneVersion->GetZonePluginVersion()<<endl;
		logwt<<"区域插件发布日期:"<<pZoneVersion->GetZonePluginReleaseDate()<<endl;
	}

	S_SYSCFG *pSysCfg = STATUS.GetSystemConfig();
	if (strcmp(pSysCfg->m_ZoneName, pZoneVersion->GetZonePluginNameC()) != 0)
	{
		strncpy(pSysCfg->m_ZoneName, pZoneVersion->GetZonePluginNameC(), sizeof(pSysCfg->m_ZoneName)-4);
		STATUS.SaveSystemConfig();
	}
	return true;
}


