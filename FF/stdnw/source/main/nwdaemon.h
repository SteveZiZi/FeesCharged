/** @file
 *  @brief 南网守护进程服务扩展
 *  @author 
 *  @date 2011/12/26
 *  @version 0.1
 *  @see mprocess/daemon.h
 */
#ifndef __NW_DAEMON_H__
#define __NW_DAEMON_H__
#include "daemon.h"
#include "batteryb.h"

class C_BATTERY:public C_BatteryBase
{
public:
	C_BATTERY(void);
	~C_BATTERY();

protected:
	/** @brief 掉电判断
	 *  @return true 系统掉电；false 系统未掉电
	 */
	virtual bool GetPowerOff(void);
	/** @brief 刷新充电状态
	 *  @param[in] fCharge true 正在充电;false 未充电
	 */
	virtual int SetBatteryChargeStatus(bool fCharge);
	/** @brief 刷新当前电池电量
	 *  @param[in] Capability 当前电池电量
	 */
	virtual int SetBatteryCapability(int Capability);
	/** @brief 取电池最小充电电量
	 *  @return 电池最小充电电量
	 */
	virtual int GetBatteryChargeLimit(void);
	/** @brief 取电池满电量
	 *  @return 电池满电量
	 */
	virtual int GetBatteryChargeFull(void);
	/** @brief 取电池充电时间
	 *  @return 电池充电时间
	 *  @remarks 单位：秒
	 */
	virtual int GetBatteryChargeHours(void);
};


/** @class
 *  @brief 南网守护进程服务类
 */
class C_NwDaemonServer:public C_DaemonServer
{
public:
	C_NwDaemonServer(void);
	virtual ~C_NwDaemonServer();

	/** @brief 本函数先于服务调用 */
	virtual int BeforeService(void);

protected:
	/** @brief 检查5秒任务 */
	virtual int CheckTask5S(void);
	/** @brief 检查2秒任务 */
	virtual int CheckTask2S(void);
	/** @brief 检查1秒任务 */
	virtual int CheckTask1S(void);
	/** @升级消息通知 */
	virtual bool UpdateNotify(int msg);

	int CheckUsbDiskPlugin(void);
	int CheckNetLinePlugin(void);
	int CheckPowerOff(void);
	int Alarm_PowerOn(S_TIME timePowerCut);
	int Alarm_PowerDown(void);
	int CheckBatteryDisable(void);
	/* 终端上电检测函数*/
	bool CheckPowerCutAlarm(void);
	bool FreshSoftVersion(void);
	bool FreshZoneInformation(void);

protected:
	S_TIME m_tickPowerCut; ///< 切电开始时间
	C_BATTERY m_Battery;
	int m_CtrlAlarmLed; ///< 告警灯控制
};

#endif//__NW_DAEMON_H__


