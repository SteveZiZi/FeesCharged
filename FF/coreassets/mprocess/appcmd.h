/** @file
 *  @brief APP命令定义框架
 *  @author 
 *  @date 2011/10/15
 *  @version 0.1
 *  1、定义APP命令，不能扩展
 *  2、定义各命令的参数，部分命令参数可以扩展
 */
#ifndef __MP_APP_CMD_H__
#define __MP_APP_CMD_H__
#include "server.h"

/** @brief 最大支持命令参数个数 */
#define MAIN_ARGV_MAX	3
/** @brief 参数之间的分隔 */
#define MAIN_PARAM_DELIMIT		'.'
/** @brief 赋值号 */
#define MAIN_PARAM_ASSIGN		'='

/** @brief 应用程序命令定义 */
#define APP_CMD_EXEC		"-exec"
#define APP_CMD_START		"-start"
#define APP_CMD_STOP		"-stop"
#define APP_CMD_SET			"-set"
#define APP_CMD_ACTION		"-action"
#define APP_CMD_LIST		"-list"
#define APP_CMD_HELP		"-help"

/** @brief 系统重启秒数 */
#define SYSTEM_SUSPEND_DELAY_MAX		4
#define WATCH_DOG_RESET_DELAY_MAX	16
#define HAREWARE_RESET_DELAY_MAX	(WATCH_DOG_RESET_DELAY_MAX+SYSTEM_SUSPEND_DELAY_MAX)
#define SOFTWARE_RESET_DELAY_MAX		8

/** @class S_HelpInfo appcmd.h "mprocess/appcmd.h"
 *  @brief 帮助信息结构定义
 *  所有命令和参数的帮助信息需用此结构定义，以示统一
 */
struct S_HelpInfo
{
	const char *m_pItem;///< 命令或参数项
	const char *m_pInfo;///< 命令或参数说明
};

/** @class C_AppCmd appcmd.h "mprocess/appcmd.h"
 *  @brief APP命令处理超类
 *  @remarks 用户需要实现以下两个函数，以便框架配置进程
 *  int MP_GetProcessSN(const char *pName);
 *  const char* MP_GetProcessName(int PSN);
 */
class C_AppCmd
{
public:
	C_AppCmd(int argc, char *argv[]);
	virtual ~C_AppCmd();

public:
	/** @brief 执行命令
	 *  @return 0 成功；-1 失败
	 */
	int ExecuteCmd(void);

protected:
	/** @brief 区域组件注册 */
	int ZoneComponentRegister(void);
	/** @brief exec命令 */
	int Exec(void);
	/** @brief start命令 */
	int Start(void);
	/** @brief Stop命令 */
	int Stop(void);
	/** @brief Set命令 */
	int Set(void);
	/** @brief Action命令 */
	int Action(void);
	/** @brief List命令 */
	int List(void);
	/** @brief Help命令 */
	int Help(void);

private:
	/** @brief 显示LOGO */
	int ShowLogo(void);
	/** @brief 列表命令帮助 */
	int ListCmdHelp(void);
	/** @brief 执行命令帮助 */
	int ExeCmdHelp(void);
	/** @brief 设置命令帮助 */
	int SetCmdHelp(void);
	/** @brief 动作命令帮助 */
	int ActionCmdHelp(void);
	/** @brief 子进程信息输出 */
	int SubProcessCmdHelp(char *pCmd);
	/** @brief LOGO显示判断
	 *  @return true 需要显示LOGO；false 不需要
	 */
	bool NeedShowLogo(void);
	/** @brief 进程互斥体检查
	 *  @return true 成功，false 失败
	 */
	bool MutuxCheck(const char *pMutuxName);
	/** @brief 串行化子进程名 */
	int SerializeSubProcessName(string &strPName);
	/** @brief 检查进程配置*/
	bool CheckProcessConfig(void);

	/** 进程信息比较 */
	int strProcessCmp(const char *pCmdInfo, const char *pDataInfo);
	/** 表计信息关键字比较 */
	int strMeterInfoCmp(const char *pCmdInfo, const char *pDataInfo);

protected:
	/** @brief 获取服务工厂方法
	 *  @param[in] pServerName 服务名
	 *  @return NULL 无效服务；!NULL 有效服务
	 *  @remarks 应用程序可以扩展，注意要回调本函数
	 */
	virtual C_SERVER *GetServer(const char *pServerName);
	/** @brief 命令前调用
	 *  @return 0 成功；-1 失败
	 */
	virtual int BeforeCmd(void);
	/** @brief 命令后调用
	 *  @return 0 成功；-1 失败
	 */
	virtual int AfterCmd(void);

	/** @brief 执行列表命令
	 *  @param[in] pCmdInfo 命令参数信息
	 *  @return 0 成功；-1 失败
	 *  @remarks 应用程序可以扩展，注意要回调本函数
	 */
	virtual int ListCmdExec(const char *pCmdInfo);
	/** @brief 显示列表命令帮助
	 *  @param[in] InfoSN 帮助显示序号
	 *  @return 0 成功；-1 失败
	 *  @remarks 应用程序可以扩展，注意要回调本函数
	 */
	virtual int ShowListHelp(int InfoSN);
	/** @brief 显示版本信息
	 *  @return 0 成功；-1 失败
	 *  @remarks 应用程序可以扩展，注意要回调本函数
	 */
	virtual int ListVersion(void);
	/** @brief 显示系统运行时间信息
	 *  @return 0 成功；-1 失败
	 */
	int ListRuntime(void);
	/** @brief 显示进程信息
	 *  @return 0 成功；-1 失败
	 */
	int ListProcessInfo(void);
	/** @brief 显示进程跟踪配置信息
	 *  @return 0 成功；-1 失败
	 */
	int ListProcessTrace(void);
	/** @brief 显示采集进程任务信息
	 *  @return 0 成功；-1 失败
	 */
	int ListDbcjTask(const char *pCmdInfo);
	/** @brief 显示表计总数等信息
	 *  @return 0 成功；-1 失败
	 */
	int ListMeterCount(const char *pCmdInfo);
	/** @brief 显示表计详细信息
	 *  @return 0 成功；-1 失败
	 */
	int ListMeterInfo(const char *pCmdInfo);
	/** @brief 显示当前区域信息
	 *  @return 0 成功；-1 失败
	 */
	int ListSystemCurrentZone(const char *pCmdInfo);

	/** @brief 执行设置命令
	 *  @param[in] pCmdInfo 命令参数信息
	 *  @return 0 成功；-1 失败
	 *  @remarks 应用程序可以扩展，注意要回调本函数
	 */
	virtual int SetCmdExec(const char *pCmdInfo);
	/** @brief 显示设置命令帮助
	 *  @param[in] InfoSN 帮助显示序号
	 *  @return 0 成功；-1 失败
	 *  @remarks 应用程序可以扩展，注意要回调本函数
	 */
	virtual int ShowSetHelp(int InfoSN);

	/** @brief 执行动作命令
	 *  @param[in] pCmdInfo 命令参数信息
	 *  @return 0 成功；-1 失败
	 *  @remarks 应用程序可以扩展，注意要回调本函数
	 */
	virtual int ActionCmdExec(const char *pCmdInfo);
	/** @brief 显示动作命令帮助
	 *  @param[in] InfoSN 帮助显示序号
	 *  @return 0 成功；-1 失败
	 *  @remarks 应用程序可以扩展，注意要回调本函数
	 */
	virtual int ShowActionHelp(int InfoSN);

	/** @brief 显示采集任务信息
	 *  @param[in] Process 采集进程序号[1-*]
	 *  @param[out] strTaskInfo 任务信息
	 *  @return 0 成功；-1 失败
	 */
	virtual int ListDbcjTaskInfo(int Process, string &strTaskInfo);

	/** @brief  采集表计总数信息
	 *  @param[out] Cnt 表计总数
	 *  @param[out] MinMP 最小测量点
	 *  @param[out] MaxMP 最大测量点
	 *  @return 0 成功；-1 失败
	 */
	virtual int GetMeterCount(S_WORD &Cnt, S_WORD &MinMP, S_WORD &MaxMP);

	/** @brief 打印表计详细信息
	 *  @param[in] MinMP 最小测量点
	 *  @param[in] MaxMP 最大测量点
	 *  @param[out] strInfo 信息
	 *  @return 0 成功；-1 失败
	 */
	virtual int PrintMeterInfo(S_WORD MinMP, S_WORD MaxMP, string &strInfo);

	/** @brief 输出当前区域名称信息
	 *  @param[out] strInfo 信息
	 *  @return 0 成功；-1 失败
	 */
	virtual int GetCurrentZone(string &strInfo);

private:
	int m_argc;///< 命令参数个数
	char *m_argv[MAIN_ARGV_MAX];///< 命令参数：1、应用程序名；2、命令；3、参数
};

#endif//__MPROCESS_APP_CMD_H__


