/** @file
 *  @brief 系统上下文结构定义类
 *  @author 
 *  @date 2011/11/07
 *  @version 0.1
 *  1、本文件是配置文件，需要用户定义
 *  2、系统类的定义在前面，用户不需要改动。用户类的定义紧跟其后
 *  3、共享内存的使用是受控的，即用户不得随意分配过大的空间
 */
#ifndef __MP_CFG_CONTEXT_H__
#define __MP_CFG_CONTEXT_H__
#include "traceset.h"

#ifndef WIN32
#include  "sharemem.h"
#else
#include  "..\..\..\coreassets\mprocess\sharemem.h"
#endif

#include "cfg_proc.h"
#include "server.h"
#include "basetype.h"
#include "sysbase.h"
#include "almevtbase.h"

#define ZONE_EXTEND_INFO_CONTEXT_MAX	1000

/** @class
 *  @brief 系统上下文定义
 *  @remarks 本类只定义属性不定义方法
 */
struct S_CONTEXT
{
	struct S_MEMSIGN m_MemSign;///< 系统上下文标识
	struct S_SERVINFO m_ServerInfo[MP_PROCESS_CNT];///<进程信息数组
	struct S_TraceSet m_TraceSet[MP_PROCESS_CNT];///<跟踪控制信息数组
	struct S_SystemInfo m_SystemInfo;///<系统信息

//user define follow me
	struct S_FlowInfo m_FlowInfo; ///< 流量控制信息
	struct S_ConnectStatus m_ConnectStatus; ///<主站通讯状态
	struct S_PowerStatus m_PowerStatus; ///< 能源状态
	struct S_HttxStatus m_HttxStatus[HTTX_PROCESS_MAX]; ///< 后台通讯状态
	struct S_DbcjStatus m_DbcjStatus[DBCJ_PROCESS_MAX];
	struct S_PppStatus m_PppStatus; ///< PPP状态
	struct S_JlcyStatus m_JlcyStatus;
	struct S_FhkzStatus m_FhkzStatus;
	struct S_CtcyStatus m_CtcyStatus;
	struct S_LcdStatus m_LcdStatus;

// user data
	struct S_AlmDatas m_AlmDatas;
	struct S_AlmFlags m_AlmFlags;
//	struct S_ForwardTaskDatas m_ForwardTaskDatas;

//地区扩展信息
	S_DWORD m_ZoneExtContext[ZONE_EXTEND_INFO_CONTEXT_MAX];
};

#endif//__MP_CFG_CONTEXT_H__

