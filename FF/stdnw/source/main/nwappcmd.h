/** @file
 *  @brief 南网命令定义
 *  @author 
 *  @date 2011/11/30
 *  @version 0.1
 *  @see appcmd.h
 *  @note
 *  1、南网命令继承自多进程框架
 *  2、本文件定义南网特色命令及服务功能
 */
#ifndef __NW_APP_CMD_H__
#define __NW_APP_CMD_H__
#include "appcmd.h"


/** @class 
 *  @brief 南网命令处理类
 *  @remarks 
 */
class C_NwAppCmd:public C_AppCmd
{
public:
	C_NwAppCmd(int argc, char *argv[]);
	virtual ~C_NwAppCmd();
	static bool SetServer(const char *pServerName, C_SERVER *pServer);

protected:
	virtual C_SERVER *GetServer(const char *pServerName);
	virtual int ListVersion(void);

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

protected:
	int ZoneSwitch(const string &strCmdInfo, string &strInfo);
};

#endif//__NW_APP_CMD_H__


