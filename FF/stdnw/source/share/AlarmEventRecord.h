#ifndef __ALARMRECORD_H_
#define __ALARMRECORD_H_

#include "sysbase.h"
#include "context.h"
#include "status.h"
#include "confuse.h"

#define REGS_ARD2	0xE2000000	

typedef struct _SProgDataArray
{
	INT32U m_dataId;
	string m_data;
}SProgDataArray;

typedef enum
{
	CTL_RELAY_OPEN =1,
	CTL_RELAY_CLOSE,
}T_emControlType;

typedef vector<SProgDataArray> SProgDataArrays;
/*********************************************************/
/**               告警内容保存类						 */	
/**														 */ 	
/*********************************************************/

class CAlarmRecrod_ARD1
{
public:
	/*
		brief: save alram data to alarm file ,as a record.
		param [in] CMeter meter:the class of meter operation
				   INT32U alarmFn:alarm fn id
				   T_emAlarmStatus state:alarm state
				   S_TIME alarmTime: alarm record saved time
		return bool 
		author:WanQingsong
		date: 2012-04-09
	*/  
	static bool AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime);
	static bool AppendStateTime(string &strAlmData, T_emAlarmStatus state, S_TIME alarmTime);
};

class CAlarmRecrod_ARD2
{	
public:	
	CAlarmRecrod_ARD2()
	{
		Init();
	}
	void Init();
	/*
		brief: save alram data to alarm file ,as a record.
		param [in] CMeter meter:the class of meter operation
				   INT32U alarmFn:alarm fn id
				   T_emAlarmStatus state:alarm state
				   S_TIME alarmTime: alarm record saved time
		return bool 
		author:WanQingsong
		date: 2012-04-09
	*/  
	static bool AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime);
	static bool InitTermData(S_RegDatas &CtxRegData);

private:
	static vector<INT32U> m_gatherFnIDs;
};

class CAlarmRecrod_ARD3
{	
public:	
	CAlarmRecrod_ARD3()
	{
		Init();
	}
	void Init();
	/*
		brief: save alram data to alarm file ,as a record.
		param [in] CMeter meter:the class of meter operation
				   INT32U alarmFn:alarm fn id
				   T_emAlarmStatus state:alarm state
				   S_TIME alarmTime: alarm record saved time
		return bool 
		author:WanQingsong
		date: 2012-04-09
	*/  
	static bool AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime);

private:
	static vector<INT32U> m_gatherFnIDs;
};

class CAlarmRecrod_ARD4
{	
public:	
	CAlarmRecrod_ARD4()
	{
		Init();
	}
	void Init();
	/*
		brief: save alram data to alarm file ,as a record.
		param [in] CMeter meter:the class of meter operation
				   INT32U alarmFn:alarm fn id
				   T_emAlarmStatus state:alarm state
				   S_TIME alarmTime: alarm record saved time
		return bool 
		author:WanQingsong
		date: 2012-04-09
	*/  
	static bool AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime);

private:
	static vector<INT32U> m_gatherFnIDs;

};

class CAlarmRecrod_ARD5
{	
public:	
	CAlarmRecrod_ARD5()
	{
		Init();
	}
	void Init();
	/*
		brief: save alram data to alarm file ,as a record.
		param [in] CMeter meter:the class of meter operation
				   INT32U alarmFn:alarm fn id
				   T_emAlarmStatus state:alarm state
				   S_TIME alarmTime: alarm record saved time
		return bool 
		author:WanQingsong
		date: 2012-04-09
	*/  
	static bool AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime);

private:
	static vector<INT32U> m_BeforeGatherFnIDs;
	static vector<INT32U> m_AfterGatherFnIDs;

};

class CAlarmRecrod_ARD6
{	
public:	
	CAlarmRecrod_ARD6()
	{
		Init();
	}
	void Init();
	/*
		brief: save alram data to alarm file ,as a record.
		param [in] CMeter meter:the class of meter operation
				   INT32U alarmFn:alarm fn id
				   T_emAlarmStatus state:alarm state
				   S_TIME alarmTime: alarm record saved time
		return bool 
		author:WanQingsong
		date: 2012-04-09
	*/  
	static bool AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime);

private:
	static vector<INT32U> m_BeforeGatherFnIDs;
	static vector<INT32U> m_AfterGatherFnIDs;

};

class CAlarmRecrod_ARD7
{	
public:	
	CAlarmRecrod_ARD7()
	{
		Init();
	}
	void Init();
	/*
		brief: save alram data to alarm file ,as a record.
		param [in] CMeter meter:the class of meter operation
				   INT32U alarmFn:alarm fn id
				   T_emAlarmStatus state:alarm state
				   S_TIME alarmTime: alarm record saved time
		return bool 
		author:WanQingsong
		date: 2012-04-09
	*/  
	static bool AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime);

private:
	static vector<INT32U> m_BeforeGatherFnIDs;

};

class CAlarmRecrod_ARD8
{	
public:	
	CAlarmRecrod_ARD8()
	{
		Init();
	}
	void Init();
	/*
		brief: save alram data to alarm file ,as a record.
		param [in] CMeter meter:the class of meter operation
				   INT32U alarmFn:alarm fn id
				   T_emAlarmStatus state:alarm state
				   S_TIME alarmTime: alarm record saved time
		return bool 
		author:WanQingsong
		date: 2012-04-09
	*/  
	static bool AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime);

private:
	static vector<INT32U> m_BeforeGatherFnIDs;
	static vector<INT32U> m_AfterGatherFnIDs;

};

class CAlarmRecrod_ARD9
{	
public:	
	CAlarmRecrod_ARD9()
	{
		Init();
	}
	void Init();
	/*
		brief: save alram data to alarm file ,as a record.
		param [in] CMeter meter:the class of meter operation
				   INT32U alarmFn:alarm fn id
				   T_emAlarmStatus state:alarm state
				   S_TIME alarmTime: alarm record saved time
		return bool 
		author:WanQingsong
		date: 2012-04-09
	*/  
	static bool AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime);

private:
	static vector<INT32U> m_BeforeGatherFnIDs;
	static vector<INT32U> m_AfterGatherFnIDs;

};

class CAlarmRecrod_ARD10
{	
public:	
	CAlarmRecrod_ARD10()
	{
		Init();
	}
	void Init();
	/*
		brief: save alram data to alarm file ,as a record.
		param [in] CMeter meter:the class of meter operation
				   INT32U alarmFn:alarm fn id
				   T_emAlarmStatus state:alarm state
				   S_TIME alarmTime: alarm record saved time
		return bool 
		author:WanQingsong
		date: 2012-04-09
	*/  
	static bool AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime, S_DWORD monthFlow);

private:
	static vector<INT32U> m_gatherFnIDs;
};

class CAlarmRecrod_ARD11
{	
public:	
	CAlarmRecrod_ARD11()
	{
		Init();
	}
	void Init();
	/*
		brief: save alram data to alarm file ,as a record.
		param [in] CMeter meter:the class of meter operation
				   INT32U alarmFn:alarm fn id
				   T_emAlarmStatus state:alarm state
				   S_TIME alarmTime: alarm record saved time
		return bool 
		author:WanQingsong
		date: 2012-04-09
	*/  
	static bool AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime);

private:
	static vector<INT32U> m_gatherFnIDs;
};

class CAlarmRecrod_ARD12
{	
public:	
	CAlarmRecrod_ARD12()
	{
		Init();
	}
	void Init();
	/*
		brief: save alram data to alarm file ,as a record.
		param [in] CMeter meter:the class of meter operation
				   INT32U alarmFn:alarm fn id
				   T_emAlarmStatus state:alarm state
				   S_TIME alarmTime: alarm record saved time
		return bool 
		author:WanQingsong
		date: 2012-04-09
	*/  
	static bool AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime);
	static bool AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime, string meterStateChar3);

private:
	static vector<INT32U> m_gatherFnIDs;
};


class CAlarmRecrod_ARD13
{	
public:	
	CAlarmRecrod_ARD13()
	{
		Init();
	}
	void Init();
	/*
		brief: save alram data to alarm file ,as a record.
		param [in] CMeter meter:the class of meter operation
				   INT32U alarmFn:alarm fn id
				   T_emAlarmStatus state:alarm state
				   S_TIME alarmTime: alarm record saved time
		return bool 
		author:WanQingsong
		date: 2012-04-09
	*/  
	static bool AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime);

private:
	static vector<INT32U> m_gatherFnIDs;
};


class CAlarmRecrod_ARD14
{	
public:	
	CAlarmRecrod_ARD14()
	{
		Init();
	}
	void Init();
	/*
		brief: save alram data to alarm file ,as a record.
		param [in] CMeter meter:the class of meter operation
				   INT32U alarmFn:alarm fn id
				   T_emAlarmStatus state:alarm state
				   S_TIME alarmTime: alarm record saved time
		return bool 
		author:WanQingsong
		date: 2012-04-09
	*/  
	static bool AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime);

private:
	static vector<INT32U> m_gatherFnIDs;
};

class CAlarmRecrod_ARD15
{	
public:	
	CAlarmRecrod_ARD15()
	{
		Init();
	}
	void Init();
	/*
		brief: save alram data to alarm file ,as a record.
		param [in] CMeter meter:the class of meter operation
				   INT32U alarmFn:alarm fn id
				   T_emAlarmStatus state:alarm state
				   S_TIME alarmTime: alarm record saved time
		return bool 
		author:WanQingsong
		date: 2012-04-09
	*/  
	static bool AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime);

private:
	static vector<INT32U> m_gatherFnIDs;
};

class CAlarmRecrod_ARD16
{	
public:	
	CAlarmRecrod_ARD16()
	{
		Init();
	}
	void Init();
	/*
		brief: save alram data to alarm file ,as a record.
		param [in] CMeter meter:the class of meter operation
				   INT32U alarmFn:alarm fn id
				   T_emAlarmStatus state:alarm state
				   S_TIME alarmTime: alarm record saved time
		return bool 
		author:WanQingsong
		date: 2012-04-09
	*/  
	static bool AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime);

private:
	static vector<INT32U> m_gatherFnIDs;
};


class CAlarmRecrod_ARD17
{	
public:	
	CAlarmRecrod_ARD17()
	{
		Init();
	}
	void Init();
	/*
		brief: save alram data to alarm file ,as a record.
		param [in] CMeter meter:the class of meter operation
				   INT32U alarmFn:alarm fn id
				   T_emAlarmStatus state:alarm state
				   S_TIME alarmTime: alarm record saved time
		return bool 
		author:WanQingsong
		date: 2012-04-09
	*/  
	static bool AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime);

private:
	static vector<INT32U> m_gatherFnIDs;
};


class CAlarmRecrod_ARD18
{	
public:	
	CAlarmRecrod_ARD18()
	{
		Init();
	}
	void Init();
	/*
		brief: save alram data to alarm file ,as a record.
		param [in] CMeter meter:the class of meter operation
				   INT32U alarmFn:alarm fn id
				   T_emAlarmStatus state:alarm state
				   S_TIME alarmTime: alarm record saved time
		return bool 
		author:WanQingsong
		date: 2012-04-09
	*/  
	static bool AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime);

private:
	static vector<INT32U> m_gatherFnIDs;
};

class CAlarmRecrod_ARD19
{	
public:	
	CAlarmRecrod_ARD19()
	{
		Init();
	}
	void Init();
	/*
		brief: save alram data to alarm file ,as a record.
		param [in] CMeter meter:the class of meter operation
				   INT32U alarmFn:alarm fn id
				   T_emAlarmStatus state:alarm state
				   S_TIME alarmTime: alarm record saved time
		return bool 
		author:WanQingsong
		date: 2012-04-09
	*/  
	static bool AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime);

private:
	static vector<INT32U> m_gatherFnIDs;
};

class CAlarmRecrod_ARD21
{	
public:	
	CAlarmRecrod_ARD21()
	{
	}

	/*
		brief: save alram data to alarm file ,as a record.
		param [in] CMeter meter:the class of meter operation
				   INT32U alarmFn:alarm fn id
				   T_emAlarmStatus state:alarm state
				   S_TIME alarmTime: alarm record saved time
		return bool 
		author:WanQingsong
		date: 2012-04-09
	*/  
	static bool AlarmRecord(INT32U alarmFn,S_FBS *pFBS,T_emAlarmStatus state,S_TIME alarmTime);

private:

};

/*********************************************************/
/**               事件内容保存类						 */	
/**														 */ 	
/*********************************************************/
class CEventRecrod_ERD1
{	
public:	
	static bool AppendTime(string &strAlmData, S_TIME alarmTime);
	/*
		brief: save terminal program event record
		param [in] CMeter meter:the class of meter operation
				   INT32U eventFn:event fn id
				   S_TIME progTime:the terminal program time 
				   T_emChannelType comType: the channel type
				   int progDataNum: the program data id number 
				   SProgDataArrays: the program data arrays
		return bool 
		author:WanQingsong
		date: 2012-04-09
	*/  
	static bool EventRecord(S_MeterInfo& meter,INT32U eventFn,S_TIME progTime,T_emChannelType comType,INT8U progDataNum,SProgDataArrays progDataArrays);

};

class CEventRecrod_ERD2
{	
public:	
	/*
		brief: save terminal program event record
		param [in] CMeter meter:the class of meter operation
				   INT32U eventFn:event fn id
				   S_TIME cleanUpTime:the max demand value clean time
		return bool 
		author:WanQingsong
		date: 2012-04-09
	*/  
	static bool EventRecord(S_MeterInfo& meter,INT32U eventFn,S_TIME cleanUpTime);

};

class CEventRecrod_ERD4
{	
public:	
	/*
		brief: save terminal program event record
		param [in] CMeter meter:the class of meter operation
				   INT32U eventFn:event fn id
				   S_TIME cleanUpTime:the max demand value clean time
		return bool 
		author:WanQingsong
		date: 2012-04-09
	*/  
	static bool EventRecord(INT32U eventFn,S_TIME powerOffTime,S_TIME powerOnsTime);

};	

class CEventRecrod_ERD6
{	
public:	
	/*
		brief: save terminal program event record
		param [in] CMeter meter:the class of meter operation
				   INT32U eventFn:event fn id
				   S_TIME cleanUpTime:the max demand value clean time
		return bool 
		author:WanQingsong
		date: 2013-10-09
	*/  
	static bool EventRecord(S_MeterInfo& meter,INT32U eventFn,T_emControlType ctlType,string energyYG,S_TIME eventTime);
};

class CEventRecrod_ERD8
{
public:
	static bool EventRecord(INT32U eventFn, S_TIME eventTime, const string &strParamChange);
};

#endif 


