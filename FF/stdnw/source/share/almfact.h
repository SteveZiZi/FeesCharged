/** @file
 *  @brief 告警对象工厂
 *  @author 
 *  @date 2013/04/12
 *  @version 0.1
 */
#ifndef __ALARM_FACTORY_H__
#define __ALARM_FACTORY_H__

#include "alarm.h"
#include "almevtbase.h"
#include "bitsetex.h"

/** @class
 *  @breif 告警对象工厂
 */
class C_AlarmFactory
{
public:
	static int InitAlarmStatus(void);
	/** @fn
	 *  @brief 获取告警对象
	 *  @param[in] ERC 告警ID [1-64]
	 *  @return 告警对象指针
	 */
	static CAlarm *GetAlarmEventObj(S_BYTE ERC);
	static bool PrintStatus(C_BITSET<8> &ParamStatus);
};

class C_AlarmDetect
{
public:
	static bool SupportDetectMethod(S_BYTE ERC,S_BYTE Methed);
};
#endif //__ALARM_FACTORY_H__

