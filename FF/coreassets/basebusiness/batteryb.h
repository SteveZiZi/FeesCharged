/**
 *  @file
 *  @brief 电池软件充电控制
 *  @author 
 *  @date 2011-12-26
 *  @version v1.0
 */
#ifndef __BASEB_BATTERY_BASE_H__
#define __BASEB_BATTERY_BASE_H__
#include "basedef.h"
#include "timeop.h"

/** @class
 *  @brief 电池充电控制
 *  @remarks 调用BatteryCharge()之前先调用InitDevice()
 */
class C_BatteryBase
{
public:
	enum{BATTERY_CHARGE_LIMIT=90, BATTERY_CHARGE_FULL=99};// base-on 0
	enum{CHARGE_MODE_DAY, CHARGE_MODE_LOWER};// 每天冲一次，低电量充电
	enum{BATTERY_CHARGE_INTER_S=86400, BATTERY_CHARGE_TIME=72000, BATTERY_CHARGE_TIME_AFTER_RESET=30};

public:
	C_BatteryBase(void);
	virtual ~C_BatteryBase();

	/** @brief 设置充电模式*/
	int SetBatteryChargeMode(int ChargeMode);
	/** @设置电池电量临界值*/
	int SetBatteryCriticalValue(int minCap, int MaxCap);
	/** @brief 充电设备初始化 */
	int InitDevice(void);
	/** @brief 充电过程 */
	int BatteryCharge(void);

protected:
	/** @brief 充电过程 */
	int _BatteryCharge(void);
	/** @brief 终端一上电充电过程 */
	int _BatteryChargeAfterReset(void);
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

protected:
	int _BatteryCharge_LowerMode(int BatteryLevel);
	int _BatteryCharge_DayMode(int BatteryLevel);

	int BatteryChargeProcess(int CurBatteryLevel);
	int BatteryNormalProcess(int CurBatteryLevel);

	int BatteryChargeProcess(void);
	int BatteryNormalProcess(void);

	int GetBatteryCap(void);
	int _GetBatteryCap(void);
	int EnableBatteryCharge(void);
	int DisableBatteryCharge(void);

protected:
	int m_BatteryChargeMode;
	bool m_BatteryChargeState;///< 电池状态:true 充电;false正常
	S_TIME m_Charge100Time;///< 电池充到100的时间，从此时间再冲AfterCap100ChargeHours时间，结束
	int m_CurBatteryCap;///< 当前电池电量
	int m_PreBatteryCap; ///< 前一次的电量
	S_TIME m_timeChargeBegin;///< 开始充电时间
	S_TIME m_timeChargeAfterReset;///< 终端上电冲电时间

	static const int AfterCap100ChargeHours = 1*60*60;///< 充到100%后，再充多长时间
};

#endif//__BASEB_BATTERY_BASE_H__

