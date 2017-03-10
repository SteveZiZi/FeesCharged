#ifndef _ALARM_H_
#define _ALARM_H_

#include "sysbase.h"
#include "almevtbase.h"

class CAlarm
{
public:
	CAlarm(INT8U alarmId)
	{
		m_AlarmId = alarmId;
		
	}
	virtual ~CAlarm(){}
	INT8U GetAlarmID(void){return m_AlarmId;}
	virtual bool Detect(void){return false;}
	virtual void Action(void){return;}
	
private:
	INT8U m_AlarmId;
};


/***************************************************/
/**       class CAlarm type define                 */
/**            									   */	
/***************************************************/
//抄表失败告警
class CAlarm_ReadMeterFail:public CAlarm
{
public:
	CAlarm_ReadMeterFail():CAlarm(ALARM_READMETER_FAIL)
	{
	}
	bool Detect(void);
	void Action(void){return;}
private:
	S_FBS m_FailMetersDbs;
};

//潮流反向告警
class CAlarm_CurrentReverse_A:public CAlarm
{
public:
	CAlarm_CurrentReverse_A():CAlarm(ALARM_CURRENT_REVERSE_A)
	{
	}
	bool Detect(void);
	void Action(void){return;}
};

class CAlarm_CurrentReverse_B:public CAlarm
{
public:
	CAlarm_CurrentReverse_B():CAlarm(ALARM_CURRENT_REVERSE_B)
	{
	}
	bool Detect(void);
	void Action(void){return;}
};

class CAlarm_CurrentReverse_C:public CAlarm
{
public:
	CAlarm_CurrentReverse_C():CAlarm(ALARM_CURRENT_REVERSE_C)
	{
	}
	bool Detect(void);
	void Action(void){return;}
};



//失流告警
class CAlarm_LoseCurrent_A:public CAlarm
{
public:
	CAlarm_LoseCurrent_A():CAlarm(ALARM_LOSE_CURRENT_A)
	{
	}
	bool Detect(void);
	void Action(void){return;}
};

class CAlarm_LoseCurrent_B:public CAlarm
{
public:
	CAlarm_LoseCurrent_B():CAlarm(ALARM_LOSE_CURRENT_B)
	{
	}
	bool Detect(void);
	void Action(void){return;}
};

class CAlarm_LoseCurrent_C:public CAlarm
{
public:
	CAlarm_LoseCurrent_C():CAlarm(ALARM_LOSE_CURRENT_C)
	{
	}
	bool Detect(void);
	void Action(void){return;}
};


//失压告警
class CAlarm_LoseVoltage_A:public CAlarm
{
public:
	CAlarm_LoseVoltage_A():CAlarm(ALARM_LOSE_VOLTAGE_A)
	{
	}
	bool Detect(void);
	void Action(void){return;}
};

class CAlarm_LoseVoltage_B:public CAlarm
{
public:
	CAlarm_LoseVoltage_B():CAlarm(ALARM_LOSE_VOLTAGE_B)
	{
	}
	bool Detect(void);
	void Action(void){return;}
};

class CAlarm_LoseVoltage_C:public CAlarm
{
public:
	CAlarm_LoseVoltage_C():CAlarm(ALARM_LOSE_VOLTAGE_C)
	{
	}
	bool Detect(void);
	void Action(void){return;}
};

class CAlarm_MeterDegreeDec:public CAlarm
{
public:
	CAlarm_MeterDegreeDec():CAlarm(ALARM_DEGREE_DECREASE)
	{
	}
	bool Detect(void);
	void Action(void){return;}
};

class CAlarm_PeriodTariffChange:public CAlarm
{
public:
	CAlarm_PeriodTariffChange():CAlarm(ALARM_PERIOD_TARIFF_CHANGE)
	{
	}
	bool Detect(void);
	void Action(void){return;}
};

//终端停电
class CAlarm_TerminalPowerOff:public CAlarm
{
public:
	CAlarm_TerminalPowerOff():CAlarm(ALARM_TERMINAL_POWEROFF)
	{
	}
	bool Detect(void);
	void Action(void){return;}
};

//终端上电
class CAlarm_TerminalPowerOn:public CAlarm
{
public:
	CAlarm_TerminalPowerOn():CAlarm(ALARM_TERMINAL_POWERON)
	{
	}
	bool Detect(void);
	void Action(void){return;}
};


//月流量越限
class CAlarm_MonthFlowOverRange:public CAlarm
{
public:
	CAlarm_MonthFlowOverRange():CAlarm(ALARM_MONTH_FLOW_OVERRANGE)
	{
	}
	bool Detect(void);
	void Action(void){return;}
};


class CAlarm_RelayChange:public CAlarm
{
public:
	CAlarm_RelayChange():CAlarm(ALARM_RELAY_CHANGE)
	{
	}
	bool Detect(void);
	void Action(void){return;}
};


//ABC失压次数告警
class CAlarm_LoseVoltageTimes:public CAlarm
{
public:
	CAlarm_LoseVoltageTimes():CAlarm(ALARM_LOSE_VOLTAGE_TIMES)
	{
	}
	bool Detect(void);
	void Action(void){return;}
};

//ABC失流次数告警
class CAlarm_LoseCurrentTimes:public CAlarm
{
public:
	CAlarm_LoseCurrentTimes():CAlarm(ALARM_LOSE_CURRENT_TIMES)
	{
	}
	bool Detect(void);
	void Action(void){return;}
};

//ABC潮流反向次数告警
class CAlarm_CurrentReverseTimes:public CAlarm
{
public:
	CAlarm_CurrentReverseTimes():CAlarm(ALARM_LOSE_CURRENT_REVERSE_TIMES)
	{
	}
	bool Detect(void);
	void Action(void){return;}
};


class CAlarm_MeterProgramTime:public CAlarm
{
public:
	CAlarm_MeterProgramTime():CAlarm(ALARM_METER_PROGRAM_TIME)
	{
	}
	bool Detect(void);
	void Action(void){return;}
};

class CAlarm_MeterStop:public CAlarm
{
public:
	CAlarm_MeterStop():CAlarm(ALARM_METER_STOP)
	{
	}
	bool Detect(void);
	void Action(void){return;}
};


class CAlarm_MeterClockAbnormal:public CAlarm
{
public:
	CAlarm_MeterClockAbnormal():CAlarm(ALARM_METER_CLOCK_ABNORMAL)
	{
	}
	bool Detect(void);
	void Action(void){return;}
};

class CAlarm_LackOfMoney:public CAlarm
{
public:
	CAlarm_LackOfMoney():CAlarm(ALARM_LACK_OF_MONERY)
	{
	}
	bool Detect(void);
	void Action(void){return;}
};

#endif


