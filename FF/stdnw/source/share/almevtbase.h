/** @file
 *  @brief 告警基础类定义
 *  @author 万青松
 *  @date 2013/09/09
 *  @version 0.1
 */
#ifndef __NW_ALMEVT_BASE_H__
#define __NW_ALMEVT_BASE_H__

#include "basedef.h"
#include "syscfg.h"
#include "bitsetex.h"
#include "basetype.h"

//
//告警数据
//
#define ALARM_REGISTER_DATA_LEN_MAX		90

#define ALM_COMMID(id)	(0xE2000001+id)	//上行通讯规约定义的告警数据标识
#define EVT_COMMID(id)	(0xE2010001+id)	//上行通讯规约定义的事件数据标识

#define CONS_METER_COMFAIL_COUNT_MAX	256	//256个字节BIT

/** @brief 告警检测方式 */
#define ALM_DETECT_METHOD_TRIGGER		0x01 /*触发式*/
#define ALM_DETECT_METHOD_REALTIME		0x02 /*实时查询式*/


struct S_FBS:public C_BITSET<CONS_METER_COMFAIL_COUNT_MAX> //抄表失败表定义
{
};

typedef enum
{//对应规约序号减1
	ALARM_CURRENT_REVERSE_A = 12,//潮流反向
	ALARM_CURRENT_REVERSE_B = 13,
	ALARM_CURRENT_REVERSE_C = 14,
	ALARM_LOSE_CURRENT_A=18,//失流
	ALARM_LOSE_CURRENT_B=19,
	ALARM_LOSE_CURRENT_C=20,
	ALARM_LOSE_VOLTAGE_A=21,//失压
	ALARM_LOSE_VOLTAGE_B=22,
	ALARM_LOSE_VOLTAGE_C=23,
	ALARM_LACK_OF_MONERY=42, //电表余额不足
	ALARM_DEGREE_DECREASE=43,//示度下降
	ALARM_METER_STOP=45,//电表停走
	ALARM_TERMINAL_POWEROFF=50,//终端掉电
	ALARM_TERMINAL_POWERON=51,//终端上电
	ALARM_METER_PROGRAM_TIME=52,//电表编程时间
	ALARM_PERIOD_TARIFF_CHANGE=53,//时段或费率
	ALARM_MONTH_FLOW_OVERRANGE=57,//月流量越限
	ALARM_RELAY_CHANGE=58,//继电器变位
	ALARM_LAHEZHA_FAIL=59,//拉合闸失败
	ALARM_READMETER_FAIL=60,//抄表失败
	ALARM_METER_CLOCK_ABNORMAL=61,//电能表时钟异常
	ALARM_LOSE_VOLTAGE_TIMES=63,//ABC相失压次数
	ALARM_LOSE_CURRENT_TIMES=64,//ABC相失流次数
	ALARM_LOSE_CURRENT_REVERSE_TIMES=65,//ABC相潮流反向次数
	ALARM_METER_PROGRAM_TIMES=66,//电表编程次数
	ALARM_MAX
}T_emAlarmID;

typedef enum
{// 对应规约序号减1
	EVENT_POWER_OFFON = 9,// 终端停上电

	EVENT_ID_MAX
}T_emEventID;

typedef enum
{
	ALARMSTATE_RESUME=0x00,
	ALARMSTATE_CREATE=0x01,
	ALARMSTATE_INVALID=0xff
}T_emAlarmStatus;

struct S_AlarmState
{
	INT8U m_AlarmId;
	INT8U m_State;
};


struct S_MpAlarmState
{
	S_AlarmState  m_AlarmStates[CONS_ALARM_TYPS_COUNT_MAX];
 	//void InitAlarmState(INT8U index, INT8U alarmId,INT8U state);
	bool SetAlarmState(INT8U alarmId,INT8U state);
	T_emAlarmStatus GetAlarmState(INT8U alarmId);
};

/** @brief 告警状态
 */
struct S_AlarmStatus
{
	S_TIME m_tickImportantHappen;
	int m_ImportantNewCnt;
	int m_NormalNewCnt;
	int m_EventNewCnt;
	BitSetEx<256> m_bsNewAlarmID; ///< 新增告警
	S_MpAlarmState m_MpAlarmStates[CONS_METER_COUNT_MAX];//记录电表的所有告警事件的当前状态
public:
	void Init(void);
	void Flush(void);
};

//
// 告警相关标志和数据
//
struct S_AlmFlag
{
	bool m_fTaskFinish;
	S_MPBS m_mpbsSuccess;
};
#define ALM_FLAGS_ITEM_MAX	128
struct S_AlmFlags
{
	struct S_AlmFlag m_Current;
	struct S_AlmFlag m_itemAlm[ALM_FLAGS_ITEM_MAX];

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
	/** @note 调用顺序
	 * 1 TestTask; 2 TestMeter; 3 ResetTask
	 */
	bool TestTask(S_BYTE ERC);
	bool ResetTask(S_BYTE ERC);
	bool TestMeter(S_BYTE ERC, S_WORD MP);
	bool SetTask(S_BYTE ERC, const S_MPBS &mpbsSuccess);
};

struct S_AlmData
{
	bool m_Enable;
	S_BYTE m_DataLen;
	char m_Data[ALARM_REGISTER_DATA_LEN_MAX];
};

struct S_AlmDatas
{
	struct S_AlmData m_ERC00[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC13[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC14[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC15[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC19[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC20[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC21[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC22[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC23[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC24[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC43[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC44[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC46[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC53[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC54[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC59[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC61[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC62[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC64[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC65[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC66[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC67[CONS_METER_COUNT_MAX];


public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
	/** @return >0:成功; <=0:失败*/
	int Load(S_DWORD ERC, S_WORD MP, string &strAlmData);
	/** @return >=0:成功; <0:失败*/
	int Save(S_DWORD ERC, S_WORD MP, const string &strAlmData);

private:
	int _Load(const S_AlmData &AlmData, string &strAlmData);
	int _Save(S_AlmData &AlmData, const string &strAlmData);
};

// 告警过程数据 缓存于文件中，不要频繁刷新，有更新才刷新
class C_AlmPhaseData
{
public:
	/** @return >0:成功; <=0:失败*/
	int Load(S_DWORD ERC, S_WORD MP, string &strAlmData);
	/** @return >=0:成功; <0:失败*/
	int Save(S_DWORD ERC, S_WORD MP, const string &strAlmData);
};

//
//
//
struct S_RegData
{
	S_DWORD m_regid;
	string m_data;
};

struct S_RegDatas:public vector<S_RegData>
{

};

class CAlarmEvtCommIdData
{
public:
	static bool GetCommRegDataFromSTS(INT16U mp,S_DWORD DI,S_RegDatas &rtSTSRegDatas);
	static bool GetCommRegDataFromCTX(INT16U mp,S_DWORD DI,S_RegDatas &rtCTXRegDatas);
public:
	static bool IsValidData(const char *pdata, INT8U len);
};



#endif

