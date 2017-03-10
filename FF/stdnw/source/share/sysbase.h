/** @file
 *  @brief 用户层系统基础类定义
 *  @author 
 *  @date 2011/12/06
 *  @version 0.1
 */
#ifndef __NW_SYS_BASE_H__
#define __NW_SYS_BASE_H__
#include "basedef.h"
#include "syscfg.h"
#include "mptype.h"
#include "channel.h"
#include "bitsetex.h"
#include "osrelate.h"
#include "basetype.h"
#include "htrace.h"
#include "datafile.h"
#include "nwbtype.h"
#include "nwmscmd.h"

#define CONS_CHANNEL_NAME_MAX_LEN		16

enum ClassDataType
{
	CLASSDATA_1 = 1, ///< 国网一类数据
	CLASSDATA_2, ///< 国网二类数据
	CLASSDATA_3, ///< 国网三类数据
	CLASSDATA_4, ///< 轮询任务数据
	CLASSDATA_5, ///< 补抄上二日/月数据
	CLASSDATA_6, ///< 补抄上三日/月数据
	CLASSDATA_7, ///< 补抄上四日/月数据
	CLASSDATA_8, ///< 补抄上五日/月数据
	CLASSDATA_9, ///< 补抄上六日/月数据

	CLASSDATA_MAX
};

/*国网一二三类数据任务标识*/
#define TASK_SIGN_PREFIX_CLASS1	(CLASSDATA_1<<24)
#define TASK_SIGN_PREFIX_CLASS2	(CLASSDATA_2<<24)
#define TASK_SIGN_PREFIX_CLASS3	(CLASSDATA_3<<24)
#define TASK_SIGN_PREFIX_CLASS4	(CLASSDATA_4<<24)

#define TASK_SIGN_PREFIX_CLASS5	(CLASSDATA_5<<24)
#define TASK_SIGN_PREFIX_CLASS6	(CLASSDATA_6<<24)
#define TASK_SIGN_PREFIX_CLASS7	(CLASSDATA_7<<24)
#define TASK_SIGN_PREFIX_CLASS8	(CLASSDATA_8<<24)
#define TASK_SIGN_PREFIX_CLASS9	(CLASSDATA_9<<24)

/* 数据位类型定义*/
struct SAlarmBitSet:public BitSetEx<CONS_MAX_ALARM_NUMBERS> //告警数据位定义
{
};
struct S_VCOMBS:public BitSetEx<SYSTEM_SERIAL_PORT_MAX> // 虚拟串口
{
};

/** @class S_SYSCFG
 *  @brief 系统配置结构体
 */
struct S_SYSCFG
{
	/** 终端地址
	 */
	S_RTUA m_RTUA; ///< 

	/** 登陆信息
	 */
	char m_SysUser[20];
	char m_SysPassword[20];

	/** 配置信息
	 */
	bool m_InitIRDA;
	char m_ZoneName[64];
	char m_CfgVer[12]; ///< 终端参数配置版本号
	char m_CfgDate[20]; ///< 终端参数配置版本发布日期
	S_BYTE m_TermType; ///< 终端类型[CONS_TERM_TYPE_]
	INT16U m_HeartBeatRetryTimes;///< 心跳次数  次
	INT16U m_HeartBeatIdleTimeS; ///<  空闲心跳时间
	INT16U m_ReportAgainIntervalM;///< 当前任务补报时间间隔
	INT16U m_BatteryStandTimeM;///< 电池供电时间长度，用于切电。默认为3分钟
	INT16U m_TermStandTimeS; ///< 掉电无电池情况下，终端切电时间

	/** 端口映射表
	 */
	S_BYTE m_PortMap[SYSTEM_SERIAL_PORT_MAX]; ///< 485端口映射表
	S_VCOMBS m_ProgramKeyPort; ///< 编程键响应的端口号

	/** 本地通信模块配置信息
	 */
	int m_DrRouteMode; ///< 东软路由模式3=3代;4=4代

public:
	void Init(void);
	/** @brief 设置终端地址 */
	void SetRTUA(const char *pRTUA);
	/** @brief 获取终端地址 */
	char *GetRTUA(char *pRTUA);
};

/** @class S_SYSRUN
 *  @brief 系统运行信息结构体
 */
struct S_SYSRUN
{
	/** 版本信息
	 */
	char m_ReleaseDate[32]; ///< 发布日期
	char m_HWVersion[32]; ///< 硬件版本号
	char m_SWVersion[32]; ///< 软件版本号
	char m_MPVersion[12]; ///< 主程序版本号

	/** 终端掉电信息
	 */
	S_TIME m_timePowerCut; ///< 终端掉电时间
	S_WORD m_cntPowerCut; ///< 终端切电次数,统计切电次数是给生产测试用

	/** 下行模块信息 */
	char m_RouterName[32]; ///< 路由模块名称
	char m_RouterType[8]; ///< XC #XC,DR,YXD,RB,SR,LME,DX
	char m_ChipCode[8];
	char m_ChipVerDate[16];
	char m_ChipVersion[8]; ///< V1 #模块版本号,00为无效

	/** MODEM信息 */
	char m_ModemFactory[16]; ///< unknow
	char m_ModemModel[16]; ///< M590
	S_DWORD m_SimIP;

public:
	void Init(void);
};

/** @class S_CHANNELS
 *  @brief 通道组合模板 
 *  @note 目前仅有上行通道(12)和下行通道(3)
 */
template <int ChannlCnt>
struct S_CHANNELS
{
	struct S_CHANNEL m_Item[ChannlCnt];

	void Init(void){memset(this, 0x00, sizeof(*this));}
	/** @brief 按通道名查找通道 */
	struct S_CHANNEL *FindChannel(const char *ChannlName)
	{
		for (int i = 0; i < ChannlCnt; ++i)
		{
			if (m_Item[i].m_Enable == true && strcasecmp((char *)m_Item[i].m_Name, ChannlName) == 0)
				return &m_Item[i];
		}
		return NULL;
	}

	/** @brief 按虚拟串口号查找通道 */
	struct S_CHANNEL *FindChannel(INT8U VcomNo, bool fVCom)
	{
		for (int i = 0; i < ChannlCnt; ++i)
		{
			if (m_Item[i].m_Enable == true && m_Item[i].m_Type == CHANNEL_TYPE_COM
				&& m_Item[i].m_PortSet.m_ComSet.m_PortNO == VcomNo)
			{
				return &m_Item[i];
			}
		}
		return NULL;
	}

	/** @brief 按通道索引查找通道 */
	struct S_CHANNEL *FindChannel(int ChannlIdx)
	{
		if (ChannlIdx >= ChannlCnt || ChannlIdx < 0)
			return NULL;
		return &m_Item[ChannlIdx];
	}
};

struct S_UpChannel:public S_CHANNELS<CHANNEL_UP_COUNT>
{
};

struct S_DnChannel:public S_CHANNELS<CHANNEL_DN_COUNT>
{
};

/** @brief 表计规约名称 */
#define NW_MRULE_NAME_DLT645_1997 "DL/T645-1997"
#define NW_MRULE_NAME_DLT645_2007 "DL/T645-2007"
#define NW_MRULE_NAME_GUANGDONG_97 "GUANGDONG_97"
#define NW_MRULE_NAME_GUANGDONG_07 "GUANGDONG_07"
#define NW_MRULE_NAME_WEI_SHENG "WEI_SHENG"
#define NW_MRULE_NAME_LGR_B "LGR_B"
#define NW_MRULE_NAME_LGR_D "LGR_D"
#define NW_MRULE_NAME_EMDI "EMDI"
#define NW_MRULE_NAME_ABB "ABB"
#define NW_MRULE_NAME_ACTARIS "ACTARIS"
#define NW_MRULE_NAME_EMAIL "EMAIL"
#define NW_MRULE_NAME_ISKRA "ISKRA"

class C_NwMRuleName
{
public:
	/** @brief 转表计规约名称 */
	static const char *MakeMeterRuleName(int ID);
	/** @brief 转表计规约ID */
	static int MakeMeterRuleID(const char *pRuleName);
	/** @brief 转表计规约索引 */
	static int MakeMeterRuleIndex(int ID);
	/** @brief 转表计规约索引 */
	static int MakeMeterRuleIndex(const char *pRuleName);
	/** @brief 串行化表计规约名称 */
	static S_WORD SerializeMeterRuleName(string &strRuleName, char fill, char delimit);
};

/** @brief 表计类型定义 */
enum
{
	MP_PROPERTY_UNKNOW = 0,
	MP_PROPERTY_RS485 = 1,///< 485表
	MP_PROPERTY_ANALOG,///< 模拟量
	MP_PROPERTY_PULSE,///< 脉冲量
	MP_PROPERTY_SUM,///< 计算值
	MP_PROPERTY_AC ///< 交流采样
};
enum
{
	METER_FUN_UNKNOW = 0,
	METER_FUN_SINGLE_PHASE = 1, ///< 单相表
	METER_FUN_MULTI_FUN ///< 多功能表
};
enum
{
	METER_TYPE_USER = 0, ///< 用户表
	METER_TYPE_MAIN ///< 总表
};
enum
{
	METER_USER_NORMAL = 0, ///< 普通用户
	METER_USER_VIP ///< 重点用户
};
enum
{
	METER_POWER_CUT_NO = 0, ///< 不带拉闸
	METER_POWER_CUT_YES ///< 带拉闸
};
typedef enum
{
	METER_RULE_GB645_1997 = 0, //00H：DL/T645-1997规约；
	METER_RULE_GB645_2007 = 1, //01H：DL/T645-2007规约；
	METER_RULE_GUANGDONG_97 = 2, //02H：广东97电表规约；
	METER_RULE_GUANGDONG_07 = 3, //03H：广东07电表规约；
	METER_RULE_WEISHENG = 4, //04H：威胜自定义规约；
	METER_RULE_LGR_B = 5, //05H：兰吉尔B表规约；
	METER_RULE_LGR_D = 6, //06H：兰吉尔D表规约；
	METER_RULE_EMDI = 7, //07H：EDMI表规约；
	METER_RULE_ABB = 8, //08H：ABB方表规约；
	METER_RULE_ACTARIS = 9, //09H：埃托利表规约；
	METER_RULE_EMAIL = 10, //0AH：EMAIL表规约；
//	METER_RULE_OTHER = 11, //0BH：其他规约
	METER_RULE_ISKRA = 11, //0BH 伊斯卡表

	METER_RULE_AC_GATHER = 100, //交流采样装置通信协议

	METER_RULE_MAX //MAX值对应的是系统支持的表计规约数
}T_emMETER_RULE;

#define RELAY_ADDRESS_LEN_MAX	6
#define METER_ADDRESS_LEN_MAX	6
#define METER_PASSWORD_LEN_MAX	8
#define METER_USERNAME_LEN_MAX	8
struct S_MeterInfo
{
	bool m_Enable; ///< 测量点的状态 0：无效；1：有效
	S_WORD m_MeasureNo; ///< 所属测量点号[注意在数组结构里与序号一致]
	S_BYTE  m_RuleID; ///< 通信协议类型 00H：DL/T645-1997规约；01H：DL/T645-2007规约；02H：其他规约
	S_BYTE  m_FunType; ///< 电能表类型：01，单相电子表；02，多功能表
	bool   m_MainFlag; ///< 总分类型：0，用户表；1，总表
	bool   m_VipFlag; ///< 重点用户属性：0，普通用户；1，重点用户
	bool   m_PowerCut; ///< 拉闸功能：0，不带拉闸；1，带拉闸；
	S_BYTE  m_TariffCnt; ///< 电能费率个数
	S_WORD m_CT; ///< CT变化
	S_WORD m_PT; ///< PT变化
	S_COMSET m_PortSet; ///< 端口号测量点端口号：00H~1EH表示485表端口号；1FH表示载波通道；20H表示无线；30H表示其他。

	S_BYTE m_WireType;///< 接线方式  01单相，03:三相三线；04:三相四线
	S_WORD m_RatedVoltage;///< 额定电压BCD
	S_DWORD m_RatedCur;///< 额定电流BCD
	S_WORD m_RatedContent;///< 报装容量BCD
	S_WORD m_VTContent;///< 变压器容量BCD

	char   m_Address[METER_ADDRESS_LEN_MAX]; ///< 通信地址
	char   m_RelayAddr[RELAY_ADDRESS_LEN_MAX]; ///< 测量点对应采集终端地址：全为00H或全为FFH则表示不带采集器
	char   m_Password[METER_PASSWORD_LEN_MAX];
	char   m_UserName[METER_USERNAME_LEN_MAX+2];

public:
	S_MeterInfo(void){ClearMeterInfo();}
	bool Existed(void)const{return m_Enable == true;}
	void ClearMeterInfo(void){memset(this,0x00,sizeof(*this));}
	int ExtractBaseInfo(S_MeterBase &MeterBaseInfo)const;
	bool ValidRelayAddress(void)const;

	bool SetRelayAddress(const char *pstrAddr);
	char* GetRelayAddress(char *pstrAddr)const;
	bool SetMeterAddress(const char *pstrAddr);
	char* GetMeterAddress(char *pstrAddr)const;
	bool SetMeterPassword(const char *pstrPwd);
	char *GetMeterPassword(char *pstrPwd)const;
	const S_COMSET GetPortSet(void)const{return m_PortSet;}

	int PrintMeterInfo(string &strInfo)const;

private:
	bool SetAddress(const char *pstrAddr, char *pBcdAddr);
	char* GetAddress(const char *pBcdAddr, char *pstrAddr)const;
};

struct S_MeterInfos
{
	struct S_MeterInfo m_Item[CONS_METER_COUNT_MAX];

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
	int PrintMeterInfo(S_WORD MinMP, S_WORD MaxMP, string &strInfo)const;
};

//
//轮询任务数据
//
struct S_PollingData
{
	bool m_Enable;
	S_TIME m_timePollMeter;

	char m_Date_C010[3]; ///< 日期
	char m_Time_C011[3]; ///< 时间
	char m_Zxyg_9010[5]; ///< 正向有功
	char m_Zxwg_9110[4]; ///< 正向无功
	char m_ActivePowerSum_B630[3]; ///< 三相有功功率总
	char m_ActivePowerA_B631[3]; ///< A相有功功率  
	char m_ActivePowerB_B632[3]; ///< B相有功功率  
	char m_ActivePowerC_B633[3]; ///< C相有功功率  
	char m_PwFactorSum_B650[2]; ///< 功率因数总
	char m_PwFactorA_B651[2]; ///< 功率因数A相
	char m_PwFactorB_B652[2]; ///< 功率因数B相
	char m_PwFactorC_B653[2]; ///< 功率因数C相
	char m_ReactivePowerSum_B640[3]; ///< 三相无功功率总
	char m_ReactivePowerA_B641[3]; ///< A相无功功率  
	char m_ReactivePowerB_B642[3]; ///< B相无功功率  
	char m_ReactivePowerC_B643[3]; ///< C相无功功率  

	char m_DemandSum_A010[3]; ///< 正有需量
	char m_DemandSum_B010[4]; ///< 正有需量时间 
	char m_DemandA_60A1[3]; ///< A正有需量  
	char m_DemandA_60B1[4]; ///< A正有需量时间  
	char m_DemandB_60A2[3]; ///< B正有需量  
	char m_DemandB_60B2[4]; ///< B正有需量时间  
	char m_DemandC_60A3[3]; ///< C正有需量  
	char m_DemandC_60B3[4]; ///< C正有需量时间  

	char m_IA_B621[3]; ///< A相电流
	char m_IB_B622[3]; ///< B相电流
	char m_IC_B623[3]; ///< C相电流
	char m_I0_6010[3]; ///< 零序电流
	char m_UA_B611[2]; ///< A相电压
	char m_UB_B612[2]; ///< B相电压
	char m_UC_B613[2]; ///< B相电压
	char m_PowerSZ_6020[3]; ///< 视在功率
};

struct S_PollingDatas
{
	struct S_PollingData m_Item[BASEB_MP_COUNT_MAX];

public:
	void Init(void){memset(this, 0xEE, sizeof(*this));}
	/** @return >0:成功; <=0:失败*/
	int Load(S_WORD MP, S_PollingData &PollData);
	/** @return >=0:成功; <0:失败*/
	int Save(S_TIME timeNow, S_WORD MP, const S_REGISTERS &Registers, const strings &strDatas);

private:
	int _Save(S_PollingData &PollData, const S_REGISTERS &Registers, const strings &strDatas);
};

//********************************************************/
//*               抄表任务参数信息                       */
//********************************************************/
struct S_GatherPortInfo
{
	bool m_Enable; ///< 有效性标志
	S_TIME m_timeGatherStd; ///< 抄表基准时间
	S_BYTE m_GatherPeriodUnit; ///< 抄表周期单位 0-3：分时日月
	S_BYTE m_GatherPeriod; ///< 抄表周期
};
struct S_GatherPortInfos
{
	struct S_GatherPortInfo m_ItemInfo[CONS_TASK_COUNT_MAX];	// 抄表端口信息,

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
};

#define DBCJ_TASK_INFO_DADT_CNT_MAX		256
struct S_GatherTaskInfo
{
	bool m_Enable; ///< 有效性标志
	S_WORD m_RecordMax; ///< 存储点数	为0表示按最大存储容量存储
	S_BYTE m_MeterType; ///< 电表类型	BIN	1	01：单相电子表；02：多功能表03：其他类型；  ff：无效类型	//注：针对公变、专变及厂站终端，该字段固定填充为ff
	S_BYTE m_VipFlag; ///< 重点用户属性	BIN	1	0：普通用户；1：重点用户ff：无效类型//注：针对公变、专变及厂站终端，该字段固定填充为ff
	S_WORD m_DaCnt; ///< 信息点标识组数	BIN	1	信息点标识1~n的组数注： 针对集中器，该字段固定填充为0
	S_WORD m_DtCnt; ///< 数据标识编码组数	BIN	1	数据标识编码1~m的组数
	S_WORD m_DA[DBCJ_TASK_INFO_DADT_CNT_MAX]; ///< DA格式：高字节DA2，低字节DA1
	S_DWORD m_DT[DBCJ_TASK_INFO_DADT_CNT_MAX]; ///< DT格式：高->低,DI3 DI2 DI1 DI0

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
};

struct S_GatherTaskInfos
{
	struct S_GatherTaskInfo m_ItemInfo[CONS_TASK_COUNT_MAX];	//任务参数信息

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
};

//********************************************************/
//*               普通任务参数信息                       */
//********************************************************/
#define NORMAL_TASK_MAX	256
#define NORMAL_TASK_ID_MIN	0xE0000301UL
#define NORMAL_TASK_ID_MAX	0xE00003FDUL
#define REPORT_TASK_INFO_DADT_CNT_MAX	256
struct S_NormalTaskInfo
{
	bool m_Enable; ///< =false
	bool m_Active; ///< =true
	S_TIME m_ReportStdTime;///< =2009-10-14 #年-月-日00:00:00 #时：分：秒
	S_BYTE m_ReportPeriodUnit; ///< =hour #minute,hour,day,month
	S_BYTE m_ReportPeriodVal;///<   =1  定时上报周期
	S_BYTE m_DataFormat; ///< =0 #0表示自描述格式组织数据，1表示按任务定义的数据格式组织数据
	S_TIME m_SampleStdTime; ///< 2009-10-14 #年-月-日00:00:00 #时：分：秒
	S_BYTE m_SamplePeriodUnit; ///< =hour #minute,hour,day,month   定时采样周期单位
	S_BYTE m_SamplePeriodVal; ///< =1  定时采样周期
	S_BYTE m_ExtractRate; ///< =1 #按照此倍率抽取数据上送
	S_WORD m_MaxTimes;  ///< =0 #0表示永远执行，非0表示任务执行次数
	S_WORD m_DaCnt; ///< 信息点标识组数	BIN	1	信息点标识1~n的组数注： 针对集中器，该字段固定填充为0
	S_WORD m_DtCnt; ///< 数据标识编码组数	BIN	1	数据标识编码1~m的组数
	S_WORD m_DA[REPORT_TASK_INFO_DADT_CNT_MAX]; ///< DA格式：高字节DA2，低字节DA1
	S_DWORD m_DT[REPORT_TASK_INFO_DADT_CNT_MAX]; ///< DT格式：高->低,DI3 DI2 DI1 DI0

public:
	bool HistoryDayTask(void)const{return (m_SamplePeriodUnit == 2 && m_ReportPeriodUnit == 2);}
};

struct S_HistoryTask
{
	S_TIME m_timeReportBegin; ///< 历史日任务取数开始时间
	S_TIME m_timeReportEnd; ///< 历史日任务取数结束时间
	S_TIME m_timeLastReport; ///< 任务最后一次启动时间
	S_MPBS   m_MPBS;///<信息点记录,目前仅记忆上报失败的表计
};
#define TASK_REPORT_HISTORY_DAY		7
struct S_HistoryTasks
{
	struct S_HistoryTask m_Item[TASK_REPORT_HISTORY_DAY];
};
struct S_NormalTaskStatus
{
	S_TIME m_timeLastReport; ///< 最近一次上报时间
	struct S_HistoryTasks m_HistoryTasks;
};

struct S_NormalTaskInfos
{
	struct S_NormalTaskInfo m_ItemInfo[NORMAL_TASK_MAX];
	struct S_NormalTaskStatus m_ItemStatus[NORMAL_TASK_MAX];

	void Init(void){memset(this, 0x00, sizeof(*this));}
	void InitTaskInfo(void){memset(m_ItemInfo, 0x00, sizeof(m_ItemInfo));}
};

//********************************************************/
//*               中继任务参数信息                       */
//********************************************************/
#define FORWARD_TASK_MAX	256
#define FORWARD_TASK_ID_MIN	0xE0000401UL
#define FORWARD_TASK_ID_MAX	0xE00004FDUL
#define FORWARD_CMD_LEN_MAX	256
struct S_ForwardTaskInfo
{
	bool m_Enable;
	bool m_Active;
	S_TIME m_ReportStdTime; ///< =2009-10-14 #年-月-日0:0:0 #时:分:秒
	S_BYTE m_ReportPeriodUnit; ///< =hour #minute,hour,day,month
	S_BYTE m_ReportPeriodVal;
	S_TIME m_SampleStdTime; ///< =2009-10-14 #年-月-日0:0:0 #时:分:秒
	S_BYTE m_SamplePeriodUnit; ///<=hour #minute,hour,day,month
	S_BYTE m_SamplePeriodVal;
	S_BYTE m_ExtractRate; ///< =1 #按照此倍率抽取数据上送
	S_BYTE m_ForwardType; ///< =0 #00普通中继；01拉闸命令；02合闸命令；03保电投入命令；04保电解除命令
	S_WORD m_MaxTimes; ///<=0 #0表示永远执行，非0表示任务执行次数
	S_COMSET m_PortSet; ///< 端口号测量点端口号：00H~1EH表示485表端口号；1FH表示载波通道；20H表示无线；30H表示其他。
	S_BYTE m_TimeoutS; ///< =30
	S_BYTE m_ForwardCmdLen;
	S_CHAR m_ForwardCmd[FORWARD_CMD_LEN_MAX];
};

struct S_ForwardTaskStatus
{
	S_TIME m_timeLastReport; ///< 最近一次上报时间
};

struct S_ForwardTaskInfos
{
	struct S_ForwardTaskInfo m_ItemInfo[FORWARD_TASK_MAX];
	struct S_ForwardTaskStatus m_ItemStatus[FORWARD_TASK_MAX];

	void Init(void){memset(this, 0x00, sizeof(*this));}
	void InitTaskInfo(void){memset(m_ItemInfo, 0x00, sizeof(m_ItemInfo));}
};

struct S_ForwardData
{
	S_BYTE m_Datalen;
	S_CHAR m_ForwardData[FORWARD_CMD_LEN_MAX];
};
struct S_ForwardTaskDatas
{
	struct S_ForwardData m_ForwardData[FORWARD_TASK_MAX];

	void Init(void){memset(this, 0x00, sizeof(*this));}
};

#define FORWARD_CHANNL_SLAVE		0
#define FORWARD_CHANNL_GATHER	1
#define FORWARD_CHANNL_MAX		2
struct S_ForwardInfo
{
	bool m_Enable; ///< 级联使能
	S_DWORD m_SlaveAddr[NW_FORWARD_ADDR_MAX]; ///< 级联从终端地址1~4（某地址等于FFFFFFF时表示不存在）
	S_DWORD m_MasterAddr; ///< 主终端地址（等于FFFFFFFF时表示不存在）
	S_DWORD m_GatherAddr; ///< 被抄终端地址
	S_BYTE m_WaitSlaveResponseS; ///< 主终端等待从终端应答的超时判定时间，缺省值20秒
	S_BYTE m_WaitMasterCtrlS; ///< 从终端等待主终端级联控制命令的超时判定时间，缺省值20秒
	S_WORD m_CT; ///< 级联台区终端CT倍率
	S_WORD m_PT; ///< 级联台区终端PT倍率
	S_WORD m_PollIntervalS; ///< 轮询周期
	char m_ChannlName[FORWARD_CHANNL_MAX][CONS_CHANNEL_NAME_MAX_LEN]; ///< #级联通道,被抄通道

	void Init(void)
	{
		memset(this, 0xFF, sizeof(*this));
		m_WaitSlaveResponseS = 20, m_WaitMasterCtrlS = 20;
		m_CT = 1, m_PT = 1;
		m_PollIntervalS = 300;
	}

	void SetRTUA(const char *pName, const char *pRTUA);
	char *GetRTUA(const char *pName, char *pRTUA);
	S_DWORD GetForwardedAddr(int i);
	bool AnySlaveAddress(void);
};

struct S_PowerLimit
{
	S_BYTE m_OverLoadM;///< 分 判断过负荷、电流过负荷、超合同容量用电、电流不平衡度、无功欠补偿、无功过补偿的时间缺省5分钟
	S_BYTE m_OverLoadEndM;///< 分 判断过负荷、电流过负荷、超合同容量用电、电流不平衡度、无功欠补偿、无功过补偿恢复的时间缺省5分钟
	S_BYTE m_MissingPhaseM;///< 分 判断断相、失压时间，缺省值15分钟
	S_BYTE m_MissingPhaseEndM;///< 分 判断断相、失压恢复时间，缺省值15分钟
	S_BYTE m_ReversedPhaseM;///< 分 判断电压逆相序时间，缺省值15分钟
	S_BYTE m_ReversedPhaseEndM;///< 分 判断电压逆相序恢复时间，缺省值15分钟
	S_BYTE m_ReversedPolarityM;///< 分 判断电流反极性时间，缺省值15分钟
	S_BYTE m_ReversedPolarityEndM;///< 分 判断电流反极性恢复时间，缺省值15分钟
	S_BYTE m_MeterTimeDiffM;///< 分 电能表时钟超差阈值，缺省值1分钟
	S_BYTE m_MeterFly;///< 电能表飞走阈值，缺省为10倍
	S_WORD m_MeterStop;///< NN.NN (BCD) kWh	电能表停走阈值：电量示度停止增长时按照有功功率计算应走电量值，越此值则电能表停走，缺省为0.1kWh
	S_DWORD m_MeterArrearage;///< 电表欠费
	S_DWORD m_MeterLastFee;///< NNNN.NN (BCD) 剩余金额不足阈值：缺省为20.00 元
	void Init(void)
	{
		m_OverLoadM = m_OverLoadEndM = 5;
		m_MissingPhaseM = m_MissingPhaseEndM = 15;
		m_ReversedPhaseM = m_ReversedPhaseEndM = 15;
		m_ReversedPolarityM = m_ReversedPolarityEndM = 15;
		m_MeterTimeDiffM = 2, m_MeterFly = 10, m_MeterStop = 0x10;
		m_MeterLastFee=20;
	}
};

struct S_IpConfig
{
	INT8U m_TermIP[4];
	INT8U m_NetMask[4];
	INT8U m_GateWay[4];
	INT8U m_MAC[32];
	INT8U m_IP_eth1[4];
	INT8U m_NetMask_eth1[4];
	INT8U m_GateWay_eth1[4];
	INT8U m_MAC_eth1[32];
	
	INT8U m_ProxyIP[4];
	INT8U m_ProxyType;
	INT8U m_ProxyLink;
	INT16U m_ProxyPort;
	INT8U m_UserName[24];
	INT8U m_Password[24];
	INT8U m_IpMethod; ///< 有线网络IP地址获取方式 0：手动设置IP地址 1：自动设置IP地址

	void Init(void){memset(this, 0x00, sizeof(*this));}
};

//
// 终端数据
//
enum
{
	RESET_METHOD_NO,
	RESET_METHOD_HW,

	RESET_METHOD_DATA,
	RESET_METHOD_PARAM,
	RESET_METHOD_ALL,
	RESET_METHOD_FACT,

	RESET_METHOD_MAX
};
struct S_CommunData
{
	S_DWORD m_CommunicationFlux; ///< 当前通讯流量累加值
	S_TIME m_timeCommunication; ///< 最近通讯时间
//	S_TIME m_timeMeterCommunication[CONS_METER_COUNT_MAX]; ///< 最近电表通讯时间
	S_DWORD m_LastDayCommunicationFlux; ///< 最近一天的流量记录累加值
	S_DWORD m_LastMonCommunicationFlux; ///< 最近一月的流量记录累加值
//	S_TIME m_LastYGPowerChangeTime[CONS_METER_COUNT_MAX]; ///< 最近总有功功率改变时间
	S_TIME m_timeFreezeDay;
	S_TIME m_timeFreezeMon;
	S_DWORD m_FreezeDayCummFlux; ///< (冻结)日终端流量
	S_DWORD m_FreezeMonCummFlux;///< (冻结)月终端流量
};
struct S_ResetData
{
	S_WORD m_DayResetTimes; ///< (当前)日终端复位次数
	S_WORD m_MonResetTimes;///< (当前)月终端复位次数

	S_TIME m_timeFreezeDay;
	S_TIME m_timeFreezeMon;
	S_WORD m_FreezeDayResetTimes; ///< (冻结)日终端复位次数
	S_WORD m_FreezeMonResetTimes;///< (冻结)月终端复位次数

	S_BYTE m_ResetMethod;///< 主站命令重启方式[RESET_METHOD_]
};
struct S_PowerSupplyData
{
	S_DWORD m_PowerSupplyM; ///< 终端供电总时间（单位分钟)
	S_DWORD m_LastDayPowerSupplyM; ///< 上日为止供电总时间
	S_DWORD m_LastMonPowerSupplyM; ///< 上月为止供电总时间

	S_TIME m_timeFreezeDay;
	S_TIME m_timeFreezeMon;
	S_WORD m_FreezeDayPowerSupplyM; ///< 上日冻结
	S_WORD m_FreezeMonPowerSupplyM; ///< 上月冻结
};
struct S_MeterData
{
	S_TIME m_timeMeterCommunication[CONS_METER_COUNT_MAX]; ///< 最近电表通讯时间
	S_TIME m_LastYGPowerChangeTime[CONS_METER_COUNT_MAX]; ///< 最近总有功功率改变时间
};
/* 采集信息统计*/
struct S_GatherStatInfo
{
	S_TIME m_timeTaskStart; ///< 任务开始时间
	S_TIME m_timeTaskFinish; ///< 任务结束时间
	S_MPBS m_mpbsSuccess; ///< 成功表计：成功置1
	S_MPBS m_mpbsFailure; ///< 失败表计：失败置1

public:
	int SetInfo(S_TIME timeTaskStart, S_WORD MP, bool fSuccess);
	int SetTaskFinish(S_TIME timeTaskFinish);
};
struct S_GatherStatInfos
{
	S_GatherStatInfo m_VipInfo; ///< 曲线统计信息
	S_GatherStatInfo m_DayInfo; ///< 日统计信息
	S_GatherStatInfo m_MonInfo; ///< 月统计信息
	S_WORD m_GatherFailedTimes[CONS_MP_COUNT_MAX]; ///< 表计采集失败次数

public:
	int SetInfo(int FreezeType, S_TIME timeTaskStart, S_WORD MP, bool fSuccess);
	int SetTaskFinish(int FreezeType, S_TIME timeTaskFinish);
};
struct S_TermData
{
	struct S_CommunData m_CommunData; ///< 主站通讯流量数据
	struct S_ResetData m_ResetData; ///< 复位相关数据
	struct S_PowerSupplyData m_PowerSupplyData; ///< 终端供电时间数据
	struct S_MeterData m_MeterData;
	struct S_GatherStatInfos m_GatherStatInfos;
	
	void Init(void){memset(this, 0x00, sizeof(*this));}
};

enum
{
	HTTX1_COMM_STATUS_NOTHING,
	HTTX1_COMM_STATUS_CLIENT_LINK, // 客户端连接
	HTTX1_COMM_STATUS_LISTEN, // 终端正在侦听
	HTTX1_COMM_STATUS_SERVER_LINK, // 服务端连接

	HTTX1_COMM_STATUS_MAX
};

/** @brief 主站通讯状态
 */
struct S_ConnectStatus
{
	volatile bool m_HttxNeedPPP[MP_HTTX_MAX];
	volatile bool m_Reconnect;
	volatile bool m_PppOnline;
	volatile bool m_Httx1OnPpp; ///< Httx1通道正在用PPP
	volatile bool m_Httx1Link;
	volatile bool m_Httx1Logon;
	volatile S_BYTE m_Httx1CommStatus; ///< HTTX1_COMM_STATUS_
	volatile S_TIME m_Httx1LogonTime; ///< HTTX1登陆时间
	volatile S_TIME m_Httx1CommTime; ///< HTTX1 通信时间

	void Init(void){memset(this, 0x00, sizeof(*this));}
};

/** @brief 系统状态
 */
struct S_PowerStatus
{
	bool m_fPowerOff; ///< 掉电状态
	bool m_fCharge; ///< 充电状态
	S_WORD m_BatteryCap; ///< 电池电量(0-100)

	void Init(void)
	{
		m_fPowerOff = false;
		m_fCharge = false;
		m_BatteryCap = 100;
	}
};


/** @brief 运算量定义*/
#define SUM_ALL_ITEM_MAX		256
struct S_ANALOG
{
	bool m_Enable;
	S_BYTE m_AnalogType; ///< 1 电压 2 电流  3 电压监测时间
	S_BYTE m_Phase; ///< 1-A相  2-B相  3-C相 
	S_WORD m_MP; ///< 测量点
};
struct S_ANALOGS
{
	S_WORD m_AnalogCnt;
	S_ANALOG m_Item[SUM_ALL_ITEM_MAX];

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
};

struct S_PULSE
{
	bool m_Enable;
	S_BYTE m_PulseType; ///< 脉冲类型 01 正向有功02 反向有功03 正向感性无功（I）04 正向容性无功（IV）05反向容性无功（II）06反向感性无功（III）
	S_WORD m_MP; ///< 测量点号
	S_DWORD m_PulseConst; ///< 脉冲常数
};

struct S_PULSES
{
	S_WORD m_PulseCnt;
	S_PULSE m_Item[SUM_ALL_ITEM_MAX];

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
};

struct S_SUM
{
	bool m_Enable;
	S_WORD m_MP; ///< 该运算量所属的测量点号
	S_WORD m_SumMp[8]; ///< 测量点号1~8
	S_BYTE m_SumOperator[8]; ///< 01(加), 81(减),00(无效)
	S_DWORD m_DI; ///< 表明测量点NNNN的数据项DI3DI2DI1DI0等于测量点NNNN 1~8的数据项DI3DI2DI1DI0之和或差
};

struct S_SUMS
{
	S_WORD m_SumCnt;
	S_SUM m_Item[SUM_ALL_ITEM_MAX];

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
};

struct S_DiffRule
{
	bool m_Enable;
	S_DWORD m_DI; ///< 数据项
	S_WORD m_MP1; ///< 比较测量点号
	S_WORD m_MP2; ///< 被比较测量点号
	S_DWORD m_DiffMin; ///< MMMM.MM (最低位小数)：采用差动比率报警的最小绝对差值
	S_WORD m_DiffAlarmLimit; ///< RR.RR(最低位小数)差动比率报警阈值(%)
	S_WORD m_DiffEndLimit; ///< SS.SS(最低位小数)差动比率报警恢复阈值(%)
};

struct S_DiffRuleS
{
	S_WORD m_DiffRuleCnt;
	S_DiffRule m_Item[SUM_ALL_ITEM_MAX];

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
};

struct S_AlarmInfo
{
	SAlarmBitSet m_ReportAlarmEven;///< 需要上报的告警
	SAlarmBitSet m_AlarmEven;///< 告警事件
	SAlarmBitSet m_OnlyEvent;///< 告警事件

	void Init(void){m_ReportAlarmEven.reset();m_AlarmEven.reset();m_OnlyEvent.reset();}
	bool NeedRecord(S_DWORD DI);
};

//
/** @brief 负荷管理参数相关 */
//
/** @brief 测量点累加标识*/
struct S_SumFlag
{
	INT16U m_cTN[8];/*表示参与累加测量点号 */
	C_BITSET<1> m_SumFlag;/*累加标志，1表示加，0表示减s*/
};
/** @brief 时段起始时间及费率号E0000D*1~E0000D*8 */
struct S_TimeArray
{
	INT8U m_A1;//
	INT8U m_A0;//
};
struct S_PwrTime
{
	INT8U m_Enable;//是否投入标志
	S_TimeArray m_Time;//小时 分钟
	S_DWORD m_PwrValve;//功率定值KW NNNNNN.NN
};
/** @brief 执行时间E0000D*9 */
struct S_ExeTime
{
	S_TimeArray m_StartDate;//开始时间 **月**日
	S_TimeArray m_EndDate;//结束时间 **月**日
	INT8U m_TI;//频率单位
	BitSetEx<32> m_N;//日期
};
/** @brief 临时限电控相关参数 */
struct S_TPBCtrlParam
{
	INT8U m_Enable;/* E0000C70临时限电控有效标识*/
	INT8S m_TPBCoefficient;/*E0000C70临时限电指标 */
	INT16U m_MinuteCount;/*E0000C70 临时限电的时间，单位分钟*/
	INT8U m_WindowCount;/* E0000C71临时限电的计算滑窗时间*/
};
/** @brief 时段控相关参数 *///zhgx
struct S_TimeCtrlParam
{
	INT8U m_SDMaxNum;/*E0000D00、E0000D10、E0000D20、E0000D30时段数*/
	S_PwrTime m_PwrTime[8];// E0000D01~E0000D08、E0000D11~E0000D18、E0000D21~E0000D28、E0000D31~E0000D38
	S_ExeTime m_ExeTime;//	 E0000D09、E0000D19、E0000D29、E0000D39
};
/** @brief 厂休控相关参数 */
struct S_FactoryCtrlParam
{
	INT8U m_Enable;/*E0000E10厂休控是否投入*/
	
	S_LONG m_CtrlLimitVar;/*E0000E11厂休控定值*/
	INT8U m_Sign;/*E0000E11厂休控定值 符号位*/
	S_TimeArray m_StartTime;//E0000E11 开始时间 **时**分
	INT16U m_ContinueTime;//E0000E11 持续时间 单位分钟
	C_BITSET<1> m_CtrlDate;//E0000E11 每周限电日：D1~D7表示星期一~星期日，D0=0	
};
/** @brief 月度电量控制相关参数 */
struct S_MonthEnergyCtrlParam
{
	INT8U m_Enable;/*E0000C61月度电量控制是否有效*/
	C_BITSET<1> m_TurnEnable;/*E0000C64月电量控制跳闸轮次有效标志*/	
	INT8S m_EnergyCoefficient; /*E0000C6C月电量定值浮动系数*/
	INT32U m_MonthEnergyLimit;	/*E0000E00月电量控制，月电量限额（一次值）*/
	S_DWORD m_TurnEnergy;		/*E0000E01月电量控制，轮次电量（一次值） */
};

/** @brief 购电量控制相关参数 */
struct S_BuyEnergyCtrlParam
{
	INT8U m_Enable;/*E0000C62购电量控制是否有效*/
	C_BITSET<1> m_TurnEnable;/*E0000C65购电量控制跳闸轮次有效标志*/
	
	INT32S m_NowBuyEnergy;	/*E0000E02购电量控制，本次购电量*/
	INT8U m_NowBuyDLSign; /*E0000E02剩余电量 符号位，0：+；1：-*/
	INT8U m_NowBuyDLNo; /*E0000E02本次购电单序号*/
	
	INT32S m_NowLeftEnergy;	/*E0000E03购电量控制，剩余电量*/
	INT8U m_NowLeftDLSign; /*E0000E03剩余电量 符号位，0：+；1：-*/
};

/** @brief 负控基本控制参数*/
struct S_FKCtrlBaseInfo
{
	C_BITSET<2> m_SwitchType;/*E0000C00~E0000C0F开关接点类型*/
	C_BITSET<2> m_SwitchInState;/*E0000C10~E0000C1F输入开关接点状态(只读)*/
	//C_BITSET<2> m_SwitchOurState;/*E0000C20~E0000C2F输出开关接点状态(只写)*/
	C_BITSET<2> m_SwitchTurnTab;/*E0000C30~E0000C3F输出开关轮次*/
	
	unsigned short m_SwitchDelayTime;/*E0000C40遥控拉闸时间*/
	bool m_BDFlag; /* E0000C50保电状态,01保电状态，00非保电状态*/
	INT32U m_TZOverLoadTimeSec;/* E0000C66超过设定值跳闸的过负荷持续时间,单位Sec*/
	INT32U m_TZUnderLoadTimeSec; /*E0000C67超过设定值跳闸恢复的负荷恢复持续时间,单位Sec*/
	INT16U m_TZAlarmDelaySec;/*E0000C68拉闸搞警延时,单位Sec*/
	S_DWORD m_MinPower;		/*E0000C6D最低负荷 单位KW*/
	
	void Init(){memset(this,0x00,sizeof(S_FKCtrlBaseInfo));}
};
/** @brief 负控相关参数*/
struct S_PWRCtrlInfo
{
	INT8U m_Enable;/*E0000C60功率控制是否有效*/
	C_BITSET<1> m_TurnEnable;/*E0000C63功率控制各轮次跳闸是否有效*/
	INT8S m_PwrCoefficient;/*E0000C6B功率定值浮动系数*/
	S_SumFlag m_struPwrSum;/*E0000C69功率控制用功率累加标志*/

	S_TPBCtrlParam m_TPBCtrlParam;/*临时限电控相关参数 */
	S_TimeCtrlParam m_TimeCtrlParam[4];/*时段控相关参数*/
	S_FactoryCtrlParam m_FactoryCtrlParam;/*厂休空相关参数*/
	void Init(){memset(this,0x00,sizeof(S_PWRCtrlInfo));}
};
/** @brief 负控相关参数*/
struct S_DLCtrlInfo
{
	S_SumFlag m_struEnergySum;/*E0000C6A电量控制用电量加标志*/

	S_MonthEnergyCtrlParam m_MonthEnergyCtrlParam;/*月度电量控制相关参数 */
	S_BuyEnergyCtrlParam m_BuyEnergyCtrlParam;/*购电量控制相关参数 */

	void Init(){memset(this,0x00,sizeof(S_DLCtrlInfo));}	
};


//
//process info
//
#define HTTX_CHANNL_MAX		3
struct S_HttxInfo
{
	bool m_ReportEnable;
	bool m_CompressEnable;
	S_BYTE m_MsgConfirmCode; ///<  = 0 #0表示不进行消息认证,1-255:消息认证方案号
	S_BYTE m_HeartBeatPeriodM; ///< =30 #0表示不要心跳
	S_BYTE m_ResendTimes; ///< =3 #重发次数
	S_WORD m_WaitResponseTimeoutS; ///< =6 #终端等待从动站响应的超时时间
	S_WORD m_LoginPeriodS; ///< =60 #0表示不要登陆
	S_DWORD m_MonTrafficLimit; ///< =0 #0表示不须要流量控制
	char m_ChannlName[HTTX_CHANNL_MAX][CONS_CHANNEL_NAME_MAX_LEN];

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
	bool NeedHeartBeat(void){return m_HeartBeatPeriodM>0;}
	bool NeedLogon(void){return m_LoginPeriodS>0;}
};

struct S_HttxStatus
{
//	S_HttxSN m_HttxSN;
	bool m_fReconnect;
	bool m_fMasterTerm;
	bool m_fForwardEnable;
	S_WORD m_ResponseCmdCnt;
	S_WORD m_ReportCmdCnt;
	S_TIME m_tickRecvMsData;

	void Init(void)
	{
//		m_HttxSN = MP_HTTX_1;
		m_fReconnect = false;
		m_fMasterTerm = true;
		m_fForwardEnable = true;
		m_ResponseCmdCnt = 0;
		m_ReportCmdCnt = 0;
		m_tickRecvMsData = 0;
	}
};

#define PPP_CHANNL_MODEM	0
#define PPP_CHANNL_GPRS	1
#define PPP_CHANNL_AT		2
#define PPP_CHANNL_SMS		3
#define PPP_CHANNL_MAX	4
#define PPP_APN_LENGTH_MAX	32
#define PPP_APN_COUNT_MAX		20

struct S_PppInfo
{
	int m_ModemPowerOffDelayS;
	int m_ModemPowerOnDelayS;
	S_WORD m_RefreshSignalInterS;///< 刷新无线信号间隔，缺省300秒
	S_WORD m_RedialInterM; ///< 掉线重拨间隔，缺省30分钟
	S_WORD m_RedialTimesPerDay; ///< #日重拨次数
	S_DialInfo m_DialInfo; ///< 拨号信息
	char m_MasterPhone[16]; ///< 主站电话号码
	char m_SmsCenterNo[16]; ///< 信息中心号码
	char m_ApnList[PPP_APN_COUNT_MAX][PPP_APN_LENGTH_MAX];
	char m_ChannlName[PPP_CHANNL_MAX][CONS_CHANNEL_NAME_MAX_LEN]; ///< #MODEM通道,GPRS通道,AT命令通道,SMS通道

	void Init(void)
	{
		memset(this, 0x00, sizeof(*this));
		m_ModemPowerOffDelayS = 2;
		m_ModemPowerOnDelayS = 4;
		m_RefreshSignalInterS = 300;
		m_RedialInterM = 30;
		m_RedialTimesPerDay = 3;
	}
};

struct S_PppStatus
{
	bool m_fGprsDialing; ///< 拨号中
	char m_ModemFactory[32];
	char m_ModemModel[32];
	char m_ModemSim[32];
	char m_ModemVer[64];
	S_BYTE m_ModemType; ///< MODEM 类型
	S_BYTE m_WirelessNetType; ///< 注册上的网络类型0-3:GPRS,CDMA,3G,4G
	S_BYTE m_LcdBottomInfoIndicator; ///< 液晶显示PPP状态指示器
	S_BYTE m_Signal; ///< 信号强度(0~31)
	S_TIME m_timeSignal; ///< 取信号时间
	S_DWORD m_SimIP;
	S_TIME m_timeLastOnline;
	S_PppPackage m_PppPackage;

	void Init(void){memset(this, 0x00, sizeof(*this));}
};

#define LCD_TERM_NAME_LEN_MAX	32
struct S_LcdInfo
{
	S_BYTE m_LCDGreyScale; ///< =195 #液晶灰度值
	S_BYTE m_LCDBrightness; ///< 液晶背光亮度
	S_BYTE m_LCDContrast; ///< =80 #液晶对比度 0-99
	S_BYTE m_LCDDirection; ///< =0 #液晶旋转方向 0-3

	int m_LCDPassword; ///< =0 #液晶调试设置权限密码
	int m_LCDLightTimeoutS; ///< =60 #液晶背光灯点亮时间
	int m_BackDesktopTimeoutS; ///< =40 #返回桌面的时间

	S_DWORD m_ZXYGFn; ///< [0-255] #正向有功的日冻结Fn值
	S_DWORD m_ZXWGFn; ///< [0-255] 
	S_DWORD m_FXYGFn; ///< [0-255] 
	S_DWORD m_FXWGFn; ///< [0-255] 
	S_DWORD m_YXXWGFn; ///< [0-255] 
	S_DWORD m_EXXWGFn; ///< [0-255] 
	S_DWORD m_SXXWGFn; ///< [0-255] 
	S_DWORD m_FXXWGFn; ///< [0-255] 
	S_DWORD m_VOLTFn; ///< [0-255] 
	S_DWORD m_CURRFn; ///< [0-255] 
	S_DWORD m_ACTPWRFn; ///< [0-255] 
	S_DWORD m_WATTLESSPWRFn; ///< [0-255] 
	S_DWORD m_PWRFACTORFn; ///< [0-255] 
	S_DWORD m_ZXYGXLFn; ///< [0-255] 
	S_DWORD m_FXYGXLFn; ///< [0-255] 
	

	char m_TermName[LCD_TERM_NAME_LEN_MAX]; ///< =***台区集中器

	void Init(void)
	{
		memset(this, 0x00, sizeof(*this));
		m_LCDPassword = 0;
		m_LCDGreyScale = 90;
		m_LCDBrightness = 90;
		m_LCDContrast = 90;
		m_LCDLightTimeoutS = 60;
		m_BackDesktopTimeoutS = 40;

		m_ZXYGFn = 0X05060100;
		m_ZXWGFn = 0X05060300;
		m_FXYGFn = 0X05060200;
		m_FXWGFn = 0X05060400;
		m_YXXWGFn = 1;
		m_EXXWGFn = 2;
		m_SXXWGFn = 2;
		m_FXXWGFn = 1;
		m_VOLTFn = 27;
		m_CURRFn = 29;
		m_ACTPWRFn = 81;
		m_WATTLESSPWRFn = 85;
		m_PWRFACTORFn = 105;
		m_ZXYGXLFn = 3;
		m_FXYGXLFn = 4;

		strcpy(m_TermName, "集中器");
	}

};
#define LCD_PROGRAM_KEY_DELAY_S		180
struct S_LcdStatus
{
	bool m_UsbDiskPlugin; ///< U盘插入
	bool m_UsbCopyFile; ///< U盘拷贝文件
	bool m_Eth0LinePlugin; ///< 网线插入
	bool m_UpdateDlog; ///< 升级消息框显示
	int m_UpdateInfo; ///< 升级消息的序号
	int m_ProgramKeyDelayS; ///< 编程键延时

	S_TIME m_timeFlashStorage; ///< FLASH操作时刻
	S_TIME m_timeSerialComm[SYSTEM_SERIAL_PORT_MAX]; ///< 串口通信时刻

	void Init(void){memset(this, 0x00, sizeof(*this));}
};

struct S_DbcjParam
{
	S_BYTE m_MeterAutoUpdate; ///< 电能表通信参数自动维护功能
	S_WORD m_LineLossRate; ///< 日线损率越限报警阈值,格式NN.N0,高字节是整数,低字节是小数
	S_DWORD m_Operator; ///< 表计操作码

	void Init(void){memset(this, 0x00, sizeof(*this));}
};

#define DBCJ_CHANNL_RS485I		0
#define DBCJ_CHANNL_RS485II	1
#define DBCJ_CHANNL_ZBWX	2
#define DBCJ_CHANNL_MAX	3
struct S_DbcjInfo
{
	S_TIME m_GatherStdTime;///< 采集标准开始时间
	S_TIME_UNIT m_GatherPeriodUnit;///< 采集周期单位
	S_BYTE m_GatherPeriodVal;///< 采集周期值
	S_BYTE m_AlmPollCycleM; ///< 告警查询周期
	S_BYTE m_VipBeginTime[2]; ///< ={10,40} #重点户任务采集开始时间 分钟
	S_BYTE m_DayBeginTime[2]; ///< =1:10 #日任务采集开始时间 小时:分钟
	S_BYTE m_DayVolFreezeTime[2]; ///< =1:10 #日电压合格率数据冻结开始时间 小时:分钟
	S_BYTE m_MonBeginDate; ///< =1 #月任务采集开始日期
	S_BYTE m_MonBeginTime[2]; ///< =1:30 #月任务采集开始时间 小时:分钟
	S_BYTE m_MonVolFreezeDate; ///< =1 #月电压合格率数据冻结 采集开始日期
	S_BYTE m_MonVolFreezeTime[2]; ///< =1:30 #月电压合格率数据冻结开始时间 小时:分钟
	char m_ChannlName[DBCJ_CHANNL_MAX][CONS_CHANNEL_NAME_MAX_LEN];

	void Init(void){memset(this, 0x00, sizeof(*this));}
};
enum
{
	DBCJ_UPDATE_METER_IDLE = 0,
	DBCJ_UPDATE_METER_STANDBY,
	DBCJ_UPDATE_METER_NOTIFY,

	DBCJ_UPDATE_METER_MAX
};
enum
{
	DBCJ_METER_CTRL_NOTHING = 0,
	DBCJ_METER_CTRL_STOP, ///< 暂停抄表
	DBCJ_METER_CTRL_RESUME, ///< 恢复抄表
	DBCJ_METER_CTRL_RESET, ///< 重新抄表

	DBCJ_METER_CTRL_MAX
};
struct S_DbcjStatus
{
	bool m_fReconnect;
	S_BYTE m_UpdateMeter485I; ///< 485I通道更新表标志 [DBCJ_UPDATE_METER_]
	S_BYTE m_UpdateMeter485II; ///< 485II通道更新表标志 [DBCJ_UPDATE_METER_]
	S_BYTE m_UpdateMeterZBWX; ///< ZBWX通道更新表标志 [DBCJ_UPDATE_METER_]

	S_BYTE m_ControlMeter485I; ///< 485I通道抄表控制 [DBCJ_METER_CTRL_]
	S_BYTE m_ControlMeter485II; ///< 485II通道抄表控制  [DBCJ_METER_CTRL_]
	S_BYTE m_ControlMeterZBWX; ///< ZBWX通道抄表控制  [DBCJ_METER_CTRL_]

	S_BYTE m_LcdBottomInfoIndicator; ///< 液晶显示DBCJ状态指示器

	S_TIME m_timeAddMeter; ///< 增删电表
	S_TIME m_timeAddTask; ///< 增删任务
	S_TIME m_timeAddAlarm; ///< 下发告警屏幕
	S_TIME m_timeManualRoute; ///< 手动路由
	S_TIME m_timeRouteError; ///< 路由出错
	S_TIME m_timeReportMeter; ///< 下发搜表任务

	char m_RouterAddr[8]; ///< 模块地址	BCD	6
	char m_RouterFactory[4]; ///< 厂商代码	ASCII	2
	char m_RouterChip[4]; ///< 芯片代码	ASCII	2
	char m_RouterDate[3]; ///< 版本日期-日月年	BCD	3
	char m_RouterVersion[2];///< 版本	BCD	2

	S_MPBS m_mpbsMyMeter;
	S_VCOMBS m_bsMyVCom;

	void Init(void){memset(this, 0x00, sizeof(*this));}
};

#define JLCY_CHANNL_METER_CHIP	0
#define JLCY_CHANNL_AC_GATHER		1
#define JLCY_CHANNL_ADJUST_METER	2
#define JLCY_CHANNL_MAX	3
struct S_JlcyInfo
{
	char m_ChannlName[JLCY_CHANNL_MAX][CONS_CHANNEL_NAME_MAX_LEN];

	void Init(void){memset(this, 0x00, sizeof(*this));}
};

#define JLCY_StopupCal	0     //停止校表或未进入校表态
#define JLCY_StartupCal	90		//进入校表态或正在校表
struct S_JlcyStatus
{
	int m_StartupCal;

	void Init(void){memset(this, 0x00, sizeof(*this));}
};

struct S_CtcyInfo
{
	int m_dummy;

	void Init(void){memset(this, 0x00, sizeof(*this));}
};
struct S_CtcyStatus
{
	int m_dummy;

	void Init(void){memset(this, 0x00, sizeof(*this));}
};

struct S_FhkzInfo
{
	S_FKCtrlBaseInfo m_BaseInfo;

	void Init(void){memset(this, 0x00, sizeof(*this));}
};

struct S_FhkzStatus
{
	int m_Dummy;

	void Init(void){memset(this, 0x00, sizeof(*this));}
};

//system info define
//测量点结构信息
enum
{
	MP_TYPE_UNKNOW, // 
	MP_TYPE_METER, // 表计
	MP_TYPE_ANALOG, // 模拟量
	MP_TYPE_PULSE, // 脉冲
	MP_TYPE_SUM, // 计算量
	MP_TYPE_AC, // 交采

	MP_TYPE_MAX
};
struct S_MpInfo
{
	S_WORD m_AutoGather:1;//数值范围0～1，"1"表示投抄，"0"表示停抄
	S_WORD m_Type:3;//[MP_TYPE_] 
	S_WORD m_SN:12;//序号
};
struct S_MpInfos
{
	struct S_MpInfo m_Item[CONS_MP_COUNT_MAX];

	void Init(void){memset(this, 0x00, sizeof(*this));}
	/** 测试MP位置是不是type类型的测量点 */
	bool Test(S_WORD MP, S_WORD type) const;
	/** 测试MP位置能不能放置type类型的测量点 */
	bool Test(S_WORD MP, S_WORD type, S_WORD SN) const;
	bool Set(S_WORD MP, S_WORD type, S_WORD SN);
	bool Reset(S_WORD MP, S_WORD type, S_WORD SN);
};
//
//测量点分类信息
//
struct S_ALLMP
{
	struct S_MTBS m_MTBS;
	struct S_MPBS m_MultiMPBS;
	struct S_MPBS m_VipMPBS;

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
	void RefreshMeterInfo(S_BYTE PortZB, S_MeterInfos *pMeterInfos, S_MpInfos *pMpInfos);
};

//
// 测量点限值参数
//电能质量监测参数
#define Max_PowerFactorVal_num  256
struct S_PowerFactor
{
	S_WORD m_PowerFactorVal;//功率因数区段定值为
};
struct S_MPointLimit
{
	bool m_Enable;
	S_WORD m_IUBRatio;//电流不平衡度阈值，缺省值三相三线30% 三相四线：50%
	S_WORD m_IUBResumeRatio;//电流不平衡度恢复阈值，缺省值三相三线25% 三相四线：40%
	S_WORD m_Uhgsx;//合格电压上限(二次值，未设置或0时，按照缺省规则判断)，缺省值为额定电压的1.07倍
	S_WORD m_Uhgxx;//合格电压下限(二次值，未设置或0时，按照缺省规则判断)，缺省值为额定电压的0.93倍
	S_WORD m_IoverRatio;//判断电流过流的相对额定值的比例，缺省为1.3
	S_WORD m_FhgzRatio;//判断负荷过载相对额定值的比例，缺省为1.2
	S_WORD m_FhhfRatio;//判断负荷过载恢复的相对额定值的比例，缺省为0.95
	S_WORD m_IZeroRatio;//零相电流报警阈值，缺省值额定电流的25%
	S_WORD m_IZeroRRatio;//零相电流报警恢复阈值，缺省值额定电流的15%
	S_WORD m_IresumeRatio;//判断电流过负荷恢复的相对额定值的比例，缺省值为1.0
	S_WORD m_UUBRatio;//电压不平衡度阈值
	S_WORD m_UUBResumeRatio;//电压不平衡度恢复阈值
	S_WORD m_UOpenRatio;//启动电压缺省为额定电压78%
	S_WORD m_UReturnRatio;//返回电压缺省为额定电压85%
	S_WORD m_IOpenRatio;//启动电流缺省为额定电流0.5%
	S_WORD m_IReturnRatio;//返回电流缺省为额定电流5%
	S_WORD m_UjbRatio;//电压畸变率阈值，缺省值5%
	S_WORD m_IjbRatio;//电流畸变率阈值，缺省值5%
	S_WORD m_UjbResumeRatio;//电压畸变恢复阈值，缺省值3%
	S_WORD m_IjbResumeRatio;//电流畸变恢复阈值，缺省值3%
	S_BYTE m_PFSectionNum;//功率因数区段数
	S_WORD m_PowerFactorVal[Max_PowerFactorVal_num];//功率因数区段定值
	S_BYTE  m_HarmonicNum;//监测谐波次数
	S_WORD m_UbcsxRatio;//电压偏差上限
	S_WORD m_UbcxxRatio;//电压偏差下限
	S_WORD m_FrebcsxRatio;//频率偏差上限
	S_WORD m_FrebcxxRatio;//频率偏差下限
	S_WORD m_ULTwinkleVal;//电压长闪限值
	S_WORD m_USTwinkleVal;//电压短闪限值
	S_WORD m_WGCompensate;//无功过补偿、无功欠补偿告警发生阈值总功率因数缺省0.8
	S_WORD m_WGrCompensate;//无功过补偿、无功欠补偿告警恢复发生阈值总功率因数缺省0.9
};
struct S_MPLS
{
	struct S_MPointLimit Item[CONS_METER_COUNT_MAX];

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
};

/** @brief 任务模板 */
struct S_TaskTemplate
{
	bool m_Enable;
	char m_TaskType[TASKINFO_NAME_MAX];
	S_BYTE m_Priority;///< 优先级，越大表示优先级越高
	S_TIME_UNIT m_TimeUnit;///< 周期单位
	S_BYTE m_PeriodVal;///< 周期值:0表示立即启动
	S_BYTE m_RetryTimes;///< 一个周期内重试次数:0表示不限次数
	S_BYTE m_MaxStartTimes;///< 最大启动次数:0表示不限次数
	S_TIME m_StandardTime;///< 标准开始时间
	S_DWORD m_ExecTimeS;///< 周期内最大执行时间，单位秒,0表示不限时间
	S_DWORD m_ReportPeriodS;///< 数据主动上报周期，表计上报周期
};
#define TASK_TEMPLATE_MAX		30
struct S_TaskTemplates
{
	struct S_TaskTemplate m_Item[TASK_TEMPLATE_MAX];

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
};

//
//主站数据项
//
#define MSDI_FLAG_TIME	0x01
//#define MSDI_FLAG_TARIFF	0x02
#define MSDI_FLAG_BLOCK	0x04
enum
{
	MSDI_TYPE_NORMAL,
	MSDI_TYPE_TARIFF,
	MSDI_TYPE_STATUS,
	MSDI_TYPE_HARMONIOUS,
	MSDI_TYPE_CLASS1_HOUR,

	MSDI_TYPE_MAX
};
enum
{
	MSDI_FREEZE_HOUR,
	MSDI_FREEZE_DAY,
	MSDI_FREEZE_MONTH,

	MSDI_FREEZE_MAX
};
struct S_MSDI
{
	bool m_Enable;
	S_BYTE m_Class;
	S_BYTE m_MSDI;
	S_BYTE m_Type;///Normal,Tariff,Status,Harmonious MSDI_TYPE_
	S_BYTE m_Flag; ///< MSDI_FLAG_
	S_BYTE m_Freeze; ///< #Freeze:冻结类型,HOUR曲线,DAY日,MONTH月MSDI_FREEZE_
	S_BYTE m_TariffCnt;
	S_DWORD m_DI; ///< 南网规约要求数据项ID
	S_DWORD m_CommRegID[MSDI_COMM_REG_MAX];
	S_DIBS m_DisableCommReg;

public:
	S_MSDI(void){memset(this, 0x00, sizeof(*this));}
	int SerializeData(const S_REGISTERS &CommRegs, const strings &strRegDatas, string &strData)const;
	int FillData(string &strData)const;

private:
	int SerializeData_Normal(const S_REGISTERS &CommRegs, const strings &strRegDatas, string &strData)const;
	int SerializeData_Tariff(const S_REGISTERS &CommRegs, const strings &strRegDatas, string &strData)const;
	int SerializeData_Status(const S_REGISTERS &CommRegs, const strings &strRegDatas, string &strData)const;
	int SerializeData_Harmonious(const S_REGISTERS &CommRegs, const strings &strRegDatas, string &strData)const;
	int SerializeData_Class1Hour(const S_REGISTERS &CommRegs, const strings &strRegDatas, string &strData)const;

	S_BYTE FindMaxTariffCnt(const S_REGISTERS &CommRegs, const strings &strRegDatas)const;
	int FixDataTariff(const S_REGISTERS &CommRegs, strings &strRegDatas, S_BYTE MaxTariffCnt)const;
};

struct S_MSDIF
{
	bool m_Enable;
	S_BYTE m_Type:4;///< Normal,Tariff,Status,Harmonious,Class1Hour MSDI_TYPE_
	S_BYTE m_Freeze:4; ///< #Freeze:冻结类型,HOUR曲线,DAY日,MONTH月MSDI_FREEZE_
	S_BYTE m_Flag; ///< MSDI_FLAG_
	S_BYTE m_TariffCnt;
	S_DWORD m_DI; ///< 南网规约要求数据项ID
	S_WORD m_idxRegBegin;
	S_WORD m_idxRegEnd;
	S_DIBS m_DisableReg;
};

#define MSDI_ITEM_MAX	256
#define MSDI_DATA_ID_MAX		2048
struct S_MSDIS
{
	S_MSDIF m_ItemClass1[MSDI_ITEM_MAX];
	S_MSDIF m_ItemClass2[MSDI_ITEM_MAX];
	S_MSDIF m_ItemClass3[MSDI_ITEM_MAX];
	S_MSDIF m_ItemClass4[MSDI_ITEM_MAX];
	S_MSDIF m_ItemClass5[MSDI_ITEM_MAX];
	S_MSDIF m_ItemClass6[MSDI_ITEM_MAX];
	S_MSDIF m_ItemClass7[MSDI_ITEM_MAX];
	S_MSDIF m_ItemClass8[MSDI_ITEM_MAX];
	S_MSDIF m_ItemClass9[MSDI_ITEM_MAX];
	S_WORD m_idxCommReg;
	S_DWORD m_CommRegID[MSDI_DATA_ID_MAX];

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
};

//
//Register area
//
#define COMMON_REGISTER_MAX		512
struct S_CommRegisters
{
	S_WORD m_Cnt;
	struct S_REGISTER m_Item[COMMON_REGISTER_MAX]; 

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
};

#endif//__NW_SYS_BASE_H__



