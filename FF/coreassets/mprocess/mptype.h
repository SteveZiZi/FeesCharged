/**
 *  @file
 *  @brief 基本用户类型定义
 *  @author 
 *  @date 2011-07-28
 *  @version v1.0
 *  @note
 *  多进程框架基本数据类型定义
 */
#ifndef __MP_BASETYPE_H__
#define __MP_BASETYPE_H__
#include "basedef.h"
#include "bitsetex.h"

/** @brief 流量控制信息 */
struct S_FlowInfo
{
	//#流控使能控制
	bool m_FlowCtrlEnable;
	//#登陆控制
	S_WORD m_LogonInterS;
	S_WORD m_LogonRetryTimes;
	S_WORD m_LogonFailedTimes;
	//#Socket控制
	S_WORD m_SocketConnectInterS;
	S_WORD m_SocketConnectRetryTimes;
	S_WORD m_SocketConnectFailedTimes;
	//#拨号控制
	S_WORD m_DialInterS;
	S_WORD m_DialRetryTimes;
	S_WORD m_DialFrequencyM;
	S_WORD m_DialFailedTimes;
	//#主站通讯日
	BitSetEx<32> m_MSConnectDate;
	BitSetEx<24> m_MSConnectTime;
	//#日月流量控制
	S_DWORD m_DayFlowMax; ///< #0表示不进行流量控制
	S_DWORD m_MonFlowMax; ///< #0表示不进行流量控制

	//心跳失败次数计数
	S_WORD m_HeartBeatFailedTimes;

public:	
	void Init(void);
	void ClearFailedInfo(void);
};

/** @brief 拨号信息 */
#define PPP_INFO_STRING_LEN_MAX	36
struct S_DialInfo
{
	S_WORD m_CheckInterM;//检测间隔（分钟）
	S_WORD m_CheckFailTimes;//检测失败阀值，如果连续此数值次检测失败，将断线
	char m_Apn[PPP_INFO_STRING_LEN_MAX];//APN
	char m_TeleNo[PPP_INFO_STRING_LEN_MAX];//电话号码，为CONS_DEFAULT_KEY时表示采用默认值
	char m_User[PPP_INFO_STRING_LEN_MAX];//拨号用户，为CONS_DEFAULT_KEY时表示采用默认值
	char m_Pwd[PPP_INFO_STRING_LEN_MAX];//拨号密码，为CONS_DEFAULT_KEY时表示采用默认值
};

/** @brief PPP报文统计 */
struct S_PppPackage
{
	S_TIME m_timeInit;
	INT64U m_LastRecvPackages;//接收包数目
	INT64U m_LastSendPackages;//发送包数目
	INT64U m_TotalRecvPackages;//总的接收包数目
	INT64U m_TotalSendPackages;//总的发送包数目
};

// 	区域配置
struct S_ZoneCfg
{
	string m_strZoneName;
	string m_strZonePackage;
};
struct S_ZoneCfgs:public vector<S_ZoneCfg>
{
};

#endif//__MP_BASETYPE_H__



