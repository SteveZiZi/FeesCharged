/** @file
 *  @brief 系统状态结构定义类
 *  @author 
 *  @date 2011/11/07
 *  @version 0.1
 *  1、本文件是配置文件，需要用户定义
 *  2、系统类的定义在前面，用户不需要改动。用户类的定义紧跟其后
 *  3、共享内存的使用是受控的，即用户不得随意分配过大的空间
 */
#ifndef __MP_CFG_STATUS_H__
#define __MP_CFG_STATUS_H__

#ifndef WIN32
#include  "sharemem.h"
#else
#include  "..\..\..\coreassets\mprocess\sharemem.h"
#endif

#include "sysbase.h"
#include "almevtbase.h"

#define ZONE_EXTEND_INFO_STATUS_MAX	1000

/** @class
 *  @brief 系统状态定义
 *  @remarks 本类只定义属性不定义方法
 */
struct S_STATUS
{
	struct S_MEMSIGN m_MemSign;///<系统状态标识

//user define follow me
//data area
	struct S_TermData m_TermData;
	struct S_TaskStatus m_TaskStatus[DBCJ_PROCESS_MAX];///< DBCJ任务信息

//config area
	struct S_SYSCFG m_SysConfig; ///< 系统配置
	struct S_SYSRUN m_SysRun; ///< 系统运行信息
	struct S_IpConfig m_IpCofig; ///< IP INFO
	struct S_ForwardInfo m_ForwardInfo;
	struct S_AlarmInfo m_AlarmInfo; ///< 告警配置信息
	struct S_PowerLimit m_PowerLimit; ///< 阈值参数
	struct S_DbcjParam m_DbcjParam; ///< 表计采集相关参数

	struct S_UpChannel m_UpChannels; ///< 上行通道信息
	struct S_DnChannel m_DnChannels; ///< 下行通道信息
	struct S_HttxInfo m_HttxInfo[HTTX_PROCESS_MAX]; ///< HTTX信息
	struct S_DbcjInfo m_DbcjInfo[DBCJ_PROCESS_MAX]; ///< dbcj 配置信息
	struct S_PppInfo m_PppInfo; ///< PPP信息
	struct S_LcdInfo m_LcdInfo; ///< lcd配置信息
	struct S_JlcyInfo m_JlcyInfo; ///< 交采信息
	struct S_FhkzInfo m_FhkzInfo; ///< 负控参数
	struct S_CtcyInfo m_CtcyInfo; ///< CT参数

//measure poing area
	struct S_MpInfos m_MpInfos;
	struct S_MeterInfos m_MeterInfos; ///< 表计和交采
	struct S_ANALOGS m_AnalogInfo; ///< 模拟量
	struct S_PULSES m_PulseInfo; ///< 脉冲量
	struct S_SUMS m_SumInfo; ///< 运算量
	struct S_DiffRuleS m_DiffRuleInfo; ///< 差动规则
	struct S_ALLMP m_ALLMP; // 所有测量点统计信息
	struct S_MPLS m_MPointLimits;//测量点限值参数
	
	struct S_NormalTaskInfos m_NormalTaskInfos;
	struct S_ForwardTaskInfos m_ForwardTaskInfos;
	struct S_GatherTaskInfos m_GatherTaskInfos;	//任务参数信息

//register area
	struct S_TaskTemplates m_TaskTemplates; ///< 任务模块
	struct S_CommRegisters m_CommRegisters; ///Common Register
	struct S_MSDIS m_MSDIS; ///MS Register

	struct S_PWRCtrlInfo m_PWRCtrlInfo;//功率控制
	struct S_DLCtrlInfo m_DLCtrlInfo;//电量控制

	//struct S_AlmDatas m_AlmDatas;
	struct S_AlarmStatus m_AlarmStatus; ///< 告警状态

//地区扩展信息
	S_DWORD m_ZoneExtStatus[ZONE_EXTEND_INFO_STATUS_MAX];
};

#endif//__MP_CFG_STATUS_H__


