/**
 *  @file
 *  @brief 基本用户类型定义
 *  @author 
 *  @date 2011-07-28
 *  @version v1.0
 *  @note
 *  基础业务基本数据类型定义
 */
#ifndef __BASEB_BASETYPE_H__
#define __BASEB_BASETYPE_H__
#include "basedef.h"
#include "bitsetex.h"
#include "timeop.h"
#include "channel.h"

/** @brief 后台通讯进程序号 */
enum S_HttxSN
{
	MP_HTTX_1 = 0,
	MP_HTTX_2,
	MP_HTTX_3,
	MP_HTTX_4,
	MP_HTTX_5,
	MP_HTTX_6,
	MP_HTTX_7,
	MP_HTTX_8,
	MP_HTTX_9,
	MP_HTTX_10,
	MP_HTTX_11,
	MP_HTTX_12,

	MP_HTTX_MAX
};
/** @brief 电表采集进程序号 */
enum S_DbcjSN
{
	MP_DBCJ_1 = 0,
	MP_DBCJ_2,
	MP_DBCJ_3,
	MP_DBCJ_4,
	MP_DBCJ_5,
	MP_DBCJ_6,
	MP_DBCJ_7,
	MP_DBCJ_8,
	MP_DBCJ_9,
	MP_DBCJ_10,
	MP_DBCJ_11,
	MP_DBCJ_12,

	MP_DBCJ_MAX
};
/** @brief 后台通讯通道序号 */
enum S_ChannelSN
{
	HTTX_CHANNEL_1 = 0,
	HTTX_CHANNEL_2,
	HTTX_CHANNEL_3
};

//
//测量点位图结构
//
#define BASEB_MP_COUNT_MAX	1201
struct S_MPBS:public BitSetEx<BASEB_MP_COUNT_MAX> //MP数据位定义
{
};
struct S_MBS:public BitSetEx<BASEB_MP_COUNT_MAX> //Meter数据位定义
{
};
struct S_MTBS
{
	S_MPBS m_AllMPBS;///< 有效的电表,所有电表
	S_MPBS m_485MPBS;///< 直抄表,终端通过485抄
	S_MPBS m_ZBMPBS;///< 非直抄表,终端通过载波或无线抄
//	S_MBS m_Vip485MPBS;///< 重点户直抄表
	S_MPBS m_VipZBMPBS;///< 重点户非直抄表
};

//
//主站数据项映射通用寄存器
//
#define MSDI_COMM_REG_MAX	32
struct S_DIBS:public BitSetEx<MSDI_COMM_REG_MAX>
{
};


/** @brief 数据存储最大记录数 */
struct S_StoreMax
{
	S_WORD m_DayData; //日数据最大记录数
	S_WORD m_MonData; //月数据最大记录数
	S_WORD m_VipData; //重点户数据最大记录数
	S_WORD m_CbrData; //抄表日数据最大记录数
};

//
//DA解析
//
class C_DA
{
public:
	C_DA(INT16U DA):m_DA1((INT8U*)&DA, 1),m_DA2(DA>>8){}
	~C_DA(){}
	
	/** @brief MP统计
	 *  @return 0 终端测量点
	 *  @return 1-8 表计等测量点
	 *  @return 0xFF 所有测量点
	 */
	INT8U GetMpCount(void)const;
	void ResetMP(int pos){m_DA1.ClearBit(pos);}
	/** @brief MP计算 */
	bool TestMP(int pos)const;
	INT16U GetMP(int pos)const;
	int PrintMPBS(S_MPBS &MPBS)const;

	INT8U GetDA1(void)const{const INT8U *p = m_DA1.GetData(); return *p;}
	INT8U GetDA2(void)const{return m_DA2;}
	INT16U GetDA(void)const{const INT8U *p = m_DA1.GetData(); return (m_DA2<<8)|*p;}

private:
	C_BITSET<1> m_DA1;
	INT8U m_DA2;
};

//
//DT 解析
//
class C_DT
{
public:
	C_DT(S_WORD DT):m_DT1((S_BYTE*)&DT, 1),m_DT2(DT>>8){}
	~C_DT(){}
	
	/** @brief 功能点统计
	 *  @return 0 无功能点
	 *  @return 1-8 功能点置位
	 */
	S_BYTE GetFnCount(void){return m_DT1.CountBit();}
	void ResetFN(int pos){m_DT1.ClearBit(pos);}
	/** @brief 功能点计算 pos[0-7]*/
	bool TestFN(int pos){return m_DT1.TestBit(pos);}
	S_WORD GetFN(int pos){return m_DT2*8 + (pos+1);}

	S_BYTE GetDT1(void){INT8U *p = m_DT1.GetData(); return *p;}
	S_BYTE GetDT2(void){return m_DT2;}
	S_WORD GetDT(void){INT8U *p = m_DT1.GetData(); return (m_DT2<<8)|*p;}

private:
	C_BITSET<1> m_DT1;
	S_BYTE m_DT2;
};

class C_DAPN
{
public:
	/** @brief 将PN转换成DA
	 * @param[in] PN 测量点号
	 * @return 有效DA
	 */
	static S_WORD PN2DA(S_WORD PN);
	/** @brief 将DA转换成PN
	 * @param[in] DA 信息点
	 * @return 有效PN
	 */
	static S_WORD DA2PN(S_WORD DA);
};

class C_DTFN
{
public:
	/** @brief 将FN转换成DT
	 * @param[in] FN 信息功能点
	 * @return 有效DT
	 */
	static S_WORD FN2DT(S_WORD FN);
	/** @brief 将DT转换成FN
	 * @param[in] DT 信息类
	 * @return 有效FN
	 */
	static S_WORD DT2FN(S_WORD DT);
};

/** @brief 表计规约名称定义集 */
//{{{
#define MRULE_NAME_UNKNOW	"unknow"
#define MRULE_NAME_GB645_1997 "GB645-1997"
#define MRULE_NAME_GB645_2007 "GB645-2007"
#define MRULE_NAME_IEC1107 "IEC1107"

#define MRULE_NAME_ISKRA "ISKRA"
#define MRULE_NAME_JLCY	"ACGather"

//}}}

/** @brief IEC1107规约特殊寄存器ID定义 */
enum
{
	IEC_MSG_NOTHING,

	IEC_MSG_REQUEST,
	IEC_MSG_RECOGNISE,
	IEC_MSG_CHOISE,
	IEC_MSG_DATA,
	IEC_MSG_COMPLETE,

	IEC_MSG_MAX	
};

/** @brief 数据报文结构 */
struct S_DATAGRAM
{
	string m_strProtocol; ///< 协议名称
	string m_strUserData; ///< 协议报文
};

/** @brief 终端地址结构 */
#define TERM_ADDR_LEN_MAX		6
struct S_TERMINAL
{
	string m_strProtocol; ///< 协议名称
	string m_strAddress; ///< 终端地址
};
struct S_TERMINALS:public vector<S_TERMINAL>
{
public:
	bool InsertUnique(S_TERMINAL &terminal);
};
bool operator<(const S_TERMINAL &left, const S_TERMINAL &right);

//
//表计寄存器定义
//
enum
{
	MR_TYPE_UNKNOW,
	MR_TYPE_BCD,
	MR_TYPE_BIN,
	MR_TYPE_BCD_BLOCK,
	MR_TYPE_BCD_TARIFF,
	MR_TYPE_BCD_TIME,
	MR_TYPE_BCD_TIME_BLOCK,
	MR_TYPE_ASCII,
	MR_TYPE_STATUS,
	MR_TYPE_TIME,
	MR_TYPE_TIME_BLOCK,
	MR_TYPE_TIME_TARIFF,
	MR_TYPE_TIME_BIN,
	MR_TYPE_TIME_BIN_BLOCK,
	MR_TYPE_COMB,

	MR_TYPE_OTHER = 0x7F
};
/** @brief 表计寄存器定义 */
struct S_REGISTER
{
	bool m_Enable;///< 规约支持
	S_DWORD m_ID;///< 寄存器ID
	bool m_Sign;///< 符号位
	S_BYTE m_Type; ///< 值类型 MR_TYPE_
	S_BYTE m_Len; ///< 值长度，字节数，变长用0表示
	S_BYTE m_Format; ///< 类型相关,BCD类为整数小数位,如0x62

public:
	S_REGISTER(void){memset(this, 0x00, sizeof(*this));}
	const char *TypeName(void)const;
	int FillData(string &strData)const;
};
struct S_REGISTERS:public vector<S_REGISTER>
{
};
bool operator<(const S_REGISTER &left, const S_REGISTER &right);

//
//表计基础信息
//
enum S_ComObj
{
	GATHER_COM_OBJ_NULL,
	GATHER_COM_OBJ_TERM, //采本地寄存器的对象
	GATHER_COM_OBJ_ANALOG, //采电压电流模拟量的对象
	GATHER_COM_OBJ_PULSE, //采脉冲的对象
	GATHER_COM_OBJ_METER, //采表计的对象

	GATHER_COM_OBJ_MAX
};
struct S_MeterBase
{
	S_WORD m_MP; ///< 测量点
	int m_RuleID; ///< 表计规约
	S_COMSET m_ComSet; ///< 通讯配置
	string m_strAddress; ///< 表计地址
	string m_strRelay; ///< 采集器地址

public:
	S_MeterBase(void)
	{
		m_MP = 0x00;
		m_RuleID = 0;
		m_ComSet.m_PortNO = 0;
	}
};

//
//表计数据主动上报数据结构
//
#define REPORT_DATA_FLAG_METER_SKIP	0x00000001UL
struct S_ReportData
{
	S_WORD m_MP; ///< 表计测量点
	S_WORD m_RuleID; ///< 表计规约ID
	S_BYTE m_idxReg; ///< 寄存器索引
	S_BYTE m_Reserve; ///< 保留
	S_WORD m_Flag; ///< 标志位
	string m_strAddress; ///< 表计地址
	strings m_strDatas; ///< 表计数据

public:
	S_ReportData(void){m_MP = 0x00, m_RuleID = 0x00, m_idxReg = 0x00, m_Reserve = 0x00, m_Flag = 0x00;}
};
struct S_ReportDatas:public vector<S_ReportData>
{
};
bool operator<(const S_ReportData &left, const S_ReportData &right);


//
//采集任务信息
//
#define TASKINFO_NAME_MAX	32
enum
{
//系统任务索引及总数
	SYS_TASK_IDLE,
	SYS_TASK_ROUTE,
	SYS_TASK_REAL_TIME,
	SYS_TASK_BROADCAST,
	SYS_TASK_ONE_OFF,
	SYS_TASK_SEEK_METER,

	SYS_TASK_MAX,

//特殊任务
	USER_TASK_ROUTE_EX = SYS_TASK_MAX,

//用户任务
	USER_TASK_VIP,
	USER_TASK_DAY,
	USER_TASK_MON,
	USER_TASK_FIX_DAY,
	USER_TASK_POLLING,
	USER_TASK_TERM_VIP,
	USER_TASK_TERM_DAY,
	USER_TASK_TERM_MON,
	USER_TASK_STAT_DAY,
	USER_TASK_STAT_MON,
	USER_TASK_POLL_ALARM,
	USER_TASK_DAY_ALARM,

	USER_TASK_TYPE_ID_MAX
};
//用户任务
#define USER_TASK_MAX		160
#define GATHER_TASK_MAX	(USER_TASK_MAX+SYS_TASK_MAX)
//系统任务ID开始
#define SYS_TASK_ID_BEGIN	0xFFFFFF00
//优先级区段
#define TASK_PRIORITY_USER_BEGIN	10
#define TASK_PRIORITY_USER_END		200
#define TASK_PRIORITY_IDLE	1
#define TASK_PRIORITY_REALTIME	(TASK_PRIORITY_USER_END+1)
#define TASK_PRIORITY_ONEOFF		(TASK_PRIORITY_USER_END+2)
//任务名
#define TASK_NAME_IDLE	"IdleTask"
#define TASK_NAME_ROUTE	"RouteTask"
#define TASK_NAME_REALTIME	"RealtimeTask"
#define TASK_NAME_BROADCAST	"BroadcastTask"
#define TASK_NAME_ONEOFF		"OneoffTask"
#define TASK_NAME_SEEKMETER		"SeekMeterTask"
//特殊任务名
#define TASK_NAME_ROUTE_EX	"RouteExTask" // 主要用于测试环境下处理有的载波需要轮抄的情况,如东软三代模式下的抄表
//用户任务名
#define TASK_NAME_VIP	"VipTask"
#define TASK_NAME_DAY	"DayTask"
#define TASK_NAME_MON	"MonTask"
#define TASK_NAME_FIX_DAY	"FixDayTask"
#define TASK_NAME_POLLING	"PollingTask" ///< 直抄表分钟轮询任务
#define TASK_NAME_TERM_VIP	"TermVipTask"  ///< 冻结终端数据如GPRS信号，流量
#define TASK_NAME_TERM_DAY	"TermDayTask"  ///< 冻结终端数据如GPRS信号，流量
#define TASK_NAME_TERM_MON	"TermMonTask"  ///< 冻结终端数据如GPRS信号，流量
#define TASK_NAME_STAT_DAY	"StatDayTask" ///< 冻结终端统计数据，如国网二类F25-30, F33-38
#define TASK_NAME_STAT_MON	"StatMonTask" ///< 冻结终端统计数据，如国网二类F25-30, F33-38
#define TASK_NAME_POLL_ALARM			"PollAlmTask" ///< 轮询类告警数据采集任务
#define TASK_NAME_DAY_ALARM		"DayAlmTask" ///< 日告警数据采集任务
//系统任务标识
#define MSDI_TASK_IDLE			(SYS_TASK_ID_BEGIN + SYS_TASK_IDLE)
#define MSDI_TASK_ROUTE		(SYS_TASK_ID_BEGIN + SYS_TASK_ROUTE)
#define MSDI_TASK_REALTIME		(SYS_TASK_ID_BEGIN + SYS_TASK_REAL_TIME)
#define MSDI_TASK_BROADCAST	(SYS_TASK_ID_BEGIN + SYS_TASK_BROADCAST)
#define MSDI_TASK_ONEOFF		(SYS_TASK_ID_BEGIN + SYS_TASK_ONE_OFF)
#define MSDI_TASK_SEEKMETER	(SYS_TASK_ID_BEGIN + SYS_TASK_SEEK_METER)
//特殊任务标识
#define MSDI_TASK_ROUTE_EX		(SYS_TASK_ID_BEGIN + SYS_TASK_MAX + 1)
// 手动启动任务时间标识
#define TASK_MANUAL_START_TIME	0x00000001UL

struct S_TaskInfo
{
	bool m_Enable;
	char m_TypeName[TASKINFO_NAME_MAX];///< 任务类型名
	S_DWORD m_MSDI;///< 主站数据ID，任务之间需要唯一
	S_BYTE m_Priority;///< 优先级，越大表示优先级越高
	S_BYTE m_RetryTimes;///< 一个周期内重试次数:0表示不限次数
	S_BYTE m_MaxStartTimes;///< 最大启动次数:0表示不限次数
	S_BYTE m_PeriodVal;///< 周期值:0表示立即启动
	S_TIME_UNIT m_TimeUnit;///< 周期单位
	S_TIME m_StandardTime;///< 标准开始时间
	S_DWORD m_ExecTimeS;///< 周期内最大执行时间，单位秒,0表示不限时间
	S_DWORD m_ReportPeriodS;///< 数据主动上报周期，表计上报周期

	S_MPBS m_MPBS; ///< 表计屏蔽位
	S_REGISTERS m_CommRegs;///< 通用数据寄存器
	S_DIBS m_DisableRegs; ///< 寄存器屏蔽位

	string m_strPrivateData;///< 私有数据

public:
	S_TaskInfo(void);
};

enum
{
	TASK_STATUS_BLOCK = 0,
	TASK_STATUS_READY,
	TASK_STATUS_RUNNING,

	TASK_STATUS_MAX
};
enum
{
	TASK_GATHER_STATUS_INIT = 0,
	TASK_GATHER_STATUS_DIRECT,
	TASK_GATHER_STATUS_REPORT,
	TASK_GATHER_STATUS_INDIRECT,

	TASK_GATHER_STATUS_FINISH
};
enum
{
	TASK_SEEK_STATUS_INIT = 0,
	TASK_SEEK_STATUS_485I,
	TASK_SEEK_STATUS_485II,
	TASK_SEEK_STATUS_ZBWX,

	TASK_SEEK_STATUS_FINISH
};
struct S_TaskCtrlStatus
{
	int m_GatherError; ///< 本次采集错误信息[METER_READ_ERROR_]
	S_TIME m_timeStart;///< 本次启动开始的实际时间
	S_TIME m_timeStdStart; ///< 本次启动开始的标准时间
	S_TIME m_timeReportStart; ///< 本次上报模式开始的实际时间
	S_BYTE m_TaskStatus; ///< 任务状态[TASK_STATUS_]

	S_BYTE m_CurrTimes;///< 当前执行次数,由任务执行器修改
	S_BYTE m_GatherStatus; ///< 采集任务状态[TASK_GATHER_STATUS_]
	S_BYTE m_idxReg; ///< 寄存器索引
	S_WORD m_mpMeter; ///< 当前正在采的测量点

	strings m_strDatas; ///< 普通方式采集数据
	S_ReportDatas m_ReportDatas; ///< 上报方式采集数据

public:
	S_TaskCtrlStatus(void);
};

//
//采集任务持久化信息
//
struct S_TaskFreezeStatus
{
	bool m_Enable;
	char m_TypeName[TASKINFO_NAME_MAX];///< 任务类型名
	S_DWORD m_MSDI; ///< 主站数据标识，唯一性
	S_TIME m_timeLastRaise; ///< 最后一次启动时间(标准时间)
	S_TIME m_timeStart;///< 本次任务开始时间(实际时间)
	S_TIME m_timeFinish;///< 本次任务完成时间(实际时间)
	S_TIME m_timeStdStart; ///< m_SuccessfulMBS有效时间(标准时间)
	S_MPBS m_SuccessfulMPBS; ///< 记录冻结成功的表计
	S_MTBS *m_pMTBS; ///< 表计类型指针

public:
	S_TaskFreezeStatus(void);
	int PrintStatusInfo(string &strInfo);
	bool ClearData(void);
};

struct S_TaskStatus
{
	struct S_TaskFreezeStatus m_Item[GATHER_TASK_MAX];

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
	void BeforeFind(S_MTBS *pMTBS);
	S_TaskFreezeStatus *Find(const S_TaskInfo &TaskInfo);
	void AfterFind(void);
	S_TaskFreezeStatus *GetTaskFreezeStatus(S_DWORD MSDI);
	int PrintStatusInfo(string &strInfo);
	bool ClearData(void);
};

//
//上报表计信息
//
struct S_ReportMeterInfo
{
	S_WORD m_SN; ///< 节点在路由表中的序号
	S_BYTE m_Phase; ///< 节点所在的相位[0, 'A', 'B', 'C']
	S_BYTE SignalQuality; ///< 信号品质[0-15]
	string m_strAddress; ///< 表计地址
	string m_strMeterRule; ///< 表计规约
	string m_strRelay; ///< 采集器

public:
	S_ReportMeterInfo(void){m_SN=0x00, m_Phase=0x00, SignalQuality=0x0F;}
};

struct S_ReportMeterInfos:public vector<S_ReportMeterInfo>
{
};

#endif//__BASEB_BASETYPE_H__



