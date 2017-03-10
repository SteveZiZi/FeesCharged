/** @file
 *  @brief 系统控制定义
 *  @author 
 *  @date 2011/11/07
 *  @version 0.1
 *  1、系统启动和重启定义
 *  2、系统脚本运行定义
 *  3、系统初始化定义
 */
#ifndef __MP_CFG_SYSCTRL_H__
#define __MP_CFG_SYSCTRL_H__

/** @class
 *  @brief 系统控制类
 */
class C_SYSCTRL
{
public:
	/** @brief 脚本运行
	 *  @param[in] fScriptFile 脚本文件名（含路径）
	 *  @return 0 成功
	 *  @return !0 失败
	 */
	static int ExecScript(const char *fScriptFile);
	/** @brief 硬件重启（看门狗重启）
	 *  @remarks 调用本操作后系统在若干秒后将重启
	 */
	static int HardwareReset(void);
	/** @brief 应用程序重启 
	 *  @remarks 调用本操作后应用程序在若干秒后将重启
	 */
	static int SoftwareReset(bool fImmediately = false);
	/** @brief 系统重启 
	 *  @remarks 调用本操作后系统立即重启
	 */
	static int SystemReboot(void);
	/** @brief 数据区初始化 */
	static int DataReset(void);
	/** @brief 数据区清除 */
	static int DataClear(void);
	/** @brief 出厂初始化 */
	static int FactoryReset(void);
	/** @brief 参数初始化 */
	static int ParamReset(void);
	/** @brief 参数区和数据区初始化 */
	static int AllReset(void);
	/** @brief 系统状态初始化 */
	static int StatusReset(void);
	/** @brief 清除所有用户配置 */
	static int UserConfigClear(void);
	/** @brief 进程使能控制 */
	static int EnableProcess(const char *pProcessName, bool fEnable);
	/** @brief 取进程使能状态 */
	static bool GetProcessEnable(const char *pProcessName);
	/** @brief 进程心跳 */
	static bool ProcessHeartBeat(void);
};

#endif//__MP_CFG_SYSCTRL_H__

