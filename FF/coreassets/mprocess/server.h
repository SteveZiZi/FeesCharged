/** @file
 *  @brief 服务抽象类
 *  @author 
 *  @date 2011/11/07
 *  @version 0.1
 *  1、系统定义的各个子系统为一个具体的服务实例
 *  2、服务的选择参见C_AppCmd的工厂方法
 */
#ifndef __MP_SERVER_H__
#define __MP_SERVER_H__
#include "basedef.h"
#include "osrelate.h"

#define SERVER_HEART_BEAT_INTERS	60

/** @class
 *  @brief 系统信息
 */
struct S_SystemInfo
{
	bool m_HWReset; ///< 硬件重启标志
	bool m_SWReset; ///< 软件重启标志
	S_WORD m_HWResetDelayS; ///< 硬件重启计时
	S_WORD m_SWResetDelayS; ///< 软件重启计时
	S_TIME m_tickSystem; ///< 系统节拍
	char m_ResetScript[CFG_PATH_FILE_MAX_LEN];

public:
	S_SystemInfo(void)
	{
		memset(this, 0x00, sizeof(*this));
		m_tickSystem = 0x01; //从1开始计数
	}
};

/** @brief 服务信息宏定义 */
enum
{
	SERVER_MODE_ACTIVED,
	SERVER_MODE_INACTIVED,
	SERVER_MODE_SUSPENDED
};
enum
{
	SERVER_STATUS_STOPPED,//未运行
	SERVER_STATUS_RUNNING,//正在运行
	SERVER_STATUS_STOPPING//正在停止
};
struct S_SERVINFO
{
	bool   m_Enabled;///< 使能
	S_BYTE m_ServID;///< 服务序号
	S_BYTE m_Mode;///< 模式,参见SERVER_MODE
	S_BYTE m_Status;///< 参见SERVER_STATUS
	S_BYTE m_DeadCnt;///< 心跳停止的次数，当心跳连续停止次数超过CONS_MAX_DEADCOUNT时，将视为此进程已死
	S_WORD m_ExeCnt;///< 被执行的次数,每被fork一次就加1
	S_WORD m_HBInterS;///< 心跳间隔
	S_TIME m_tickLHB;///< 最后心跳时间（软时钟）
	PID_T  m_PID;///< 进程ID

public:
	S_SERVINFO(void);
	S_SERVINFO(S_BYTE ServID);
	~S_SERVINFO();

public:
	bool IsStartable(void) const;
	S_BYTE GetServerID(void) const;
	bool IsKillable(void) const;
	void SetActive(void);
	void SetInactive(void);
	void SetSuspend(void);
	void SetResume(void);
	bool IsRunning() const;
	/** @brief 设置停止状态 */
	void SetStoppingStatus(void);
	/** @brief 设置已停止状态 */
	void SetStopedStatus(void);
	/** @brief 设置启动状态 */
	void SetStartedStatus(PID_T pid);
	/** @brief 判断心跳超时
	 *  @param[in] tickNow 当前系统节拍
	 *  @return true 超时；false 未超时
	 */
	bool IsHBTimeout(S_TIME tickNow) const;
	/** @brief 取MODE串 */
	const char *GetModeKey(void);
	/** @brief 取STATUS串 */
	const char *GetStatusKey(void);
};
inline bool S_SERVINFO::IsStartable(void) const
{
	return (m_Enabled==true && m_Mode==SERVER_MODE_ACTIVED && m_Status==SERVER_STATUS_STOPPED);
}
inline S_BYTE S_SERVINFO::GetServerID(void) const
{
	return m_ServID;
}
inline bool S_SERVINFO::IsKillable(void) const
{
	return (m_Enabled==false || m_Mode!=SERVER_MODE_ACTIVED);
}
inline void S_SERVINFO::SetActive(void)
{
	m_Mode=SERVER_MODE_ACTIVED;
}
inline void S_SERVINFO::SetInactive(void)
{
	m_Mode=SERVER_MODE_INACTIVED;
}
inline void S_SERVINFO::SetSuspend(void)
{
	if (m_Mode==SERVER_MODE_ACTIVED)
	{
		m_Mode=SERVER_MODE_SUSPENDED;
	}
}
inline void S_SERVINFO::SetResume(void)
{
	if (m_Mode==SERVER_MODE_SUSPENDED)
	{
		m_Mode=SERVER_MODE_ACTIVED;
	}		
}
inline bool S_SERVINFO::IsRunning() const
{
	return (m_Status==SERVER_STATUS_RUNNING && m_PID>0);
}

/** @class
 *  @brief 系统服务基类
 *  @remarks 系统中的所有服务必须继承本类
 */
class C_SERVER
{
public:
	C_SERVER(void);
	virtual ~C_SERVER();

	/** @brief 根据服务ID取服务名 */
	static const char *GetServerName(int ServerID);
	/** @brief 根据服务名取服务ID */
	static int GetServerID(const char *ServerName);
	/** @brief 取当前服务ID */
	static int GetCurrentServID(void){return m_CurServID;}
	/** @brief 取当前服务名 */
	static const char *GetCurrentServerName(void){return GetServerName(GetCurrentServID());}
	/** @brief 设置当前服务ID */
	static void SetCurrentServID(int ServerID){m_CurServID = ServerID;}
	/** @brief 设置当前服务信息 */
	static void SetCurrentServer(C_SERVER *pServer){m_pCurServer = pServer;}
	/** @brief 取当前服务 */
	static C_SERVER *GetCurrentServer(void){return m_pCurServer;}
	/**  @brief 应用命令列表进程信息
	 *  @param[in] pProcess:进程名
	 *  @param[out] strProcessInfo:输出文本信息缓冲
	 *  @return 0:successful, -1:failed
	 */
	static int AppCmdListProcess(const char *pProcessName, string &strProcessInfo);

public:
	/** @brief 服务过程
	 *  @return 0 成功；-1 失败
	 */
	int Service(void);
	/** @brief 心跳
	 *  @return true 成功；false 失败
	 */
	bool HeartBeat(void);
	/** @brief 判断是否已启动 */
	bool IsStartable(void){return m_pServInfo->IsStartable();}
	/** @brief 设置服务信息 */
	void SetServerInfo(S_SERVINFO *pServInfo){m_pServInfo = pServInfo;}

	/** @brief 启动服务前调用
	 *  @return 0 成功；-1 失败
	 *  @remarks 应用程序可以重载
	 */
	virtual int BeforeService(void){return 0;}
	/** @brief 启动服务后调用
	 *  @return 0 成功；-1 失败
	 *  @remarks 应用程序可以重载
	 */
	virtual int AfterService(void){return 0;}
	/** @brief 周期服务
	 *  @return 0 成功；-1 失败
	 *  @remarks 应用程序可以重载
	 */
	virtual int OnTickService(void){return 0;}

protected:
	/** @brief 心跳判断
	 *  @return true 需要；false 否
	 */
	bool HeartBeatable(void){return m_pServInfo->m_HBInterS>0;}

	/** @brief ALARM中断函数
	 *  @return 0 成功；-1 失败
	 *  @remarks 应用程序可以重载
	 */
	virtual int OnSigAlrm(void);
	/** @brief TERM中断函数
	 *  @return 0 成功；-1 失败
	 *  @remarks 应用程序可以重载
	 */
	virtual int OnSigTerm(void);
	/** @brief 取ALARM间隔
	 *  @return 0 成功；-1 失败
	 *  @remarks 应用程序可以重载，如为0，则不启动ALARM
	 */
	virtual S_DWORD GetAlarmInterUS(void){return 0x00;}

private:
	/** @brief 安装中断 */
	void InstallSignal(void);
#ifndef WIN32
	/** @brief 中断信号处理器 */
	static void SigHandler(int signo, siginfo_t *siginfp, struct sigcontext * scp);
#endif

protected:
	S_SERVINFO *m_pServInfo; ///< 服务信息

private:
	static int m_CurServID; ///< 本进程服务ID，方便全局使用
	static C_SERVER *m_pCurServer; ///< 本进程服务信息，方便全局使用
};

#endif //__MP_SERVER_H__


