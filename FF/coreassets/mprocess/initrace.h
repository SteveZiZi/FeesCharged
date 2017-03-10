/** @file
 *  @brief 进程跟踪信息加载
 *  @author 
 *  @date 2011/10/15
 *  @version 0.1
 *  1、进程跟踪信息加载文件
 *  2、跟踪信息采用文本文件描述
 *  @see foundation/confuse inifileb.h
 */
#ifndef __MP_INI_TRACE_H__
#define __MP_INI_TRACE_H__
#include "inifileb.h"

/** @class
 *  @brief 进程跟踪信息配置类
 */
class C_IniTrace
{
public:
	/** @fn C_IniTrace::LoadProcessTraceConfig
	 *  @brief 加载各服务的跟踪信息配置
	 *  @param[in] fname:文件名	
	 *  @return 0:successful, -1:failed
	 */
	static int LoadProcessTraceConfig(const char *fname);
	/** @fn C_IniTrace::SaveProcessTraceConfig
	 *  @brief 保存各服务的跟踪信息配置
	 *  @param[in] fname:文件名
	 *  @return 0:successful, -1:failed
	 */
	static int SaveProcessTraceConfig(const char *fname);
	/** @fn C_IniTrace::AppCmdSetTrace
	 *  @brief 应用命令设置进程跟踪配置信息
	 *  @param[in] pProcess:进程名
	 *  @param[in] pItem:跟踪项
	 *  @param[in] pVal:值
	 *  @return 0:successful, -1:failed
	 */
	static int AppCmdSetTrace(const char *pProcess, const char *pItem, const char *pVal);
	/** @fn C_IniTrace::AppCmdListTrace
	 *  @brief 应用命令列表进程跟踪配置信息
	 *  @param[in] pProcess:进程名
	 *  @param[out] strTraceInfo:输出文本信息缓冲
	 *  @return 0:successful, -1:failed
	 */
	static int AppCmdListTrace(const char *pProcess, string &strTraceInfo);

private:
	/** @brief 名称->级别 */
	static S_BYTE MakeLevelVal(const char* pkey);
	/** @brief 名称->目标 */
	static S_BYTE MakeWayVal(const char* pkey);
	/** @brief 级别->名称 */
	static const char* MakeLevelKey(S_BYTE val);
	/** @brief 目标->名称 */
	static const char* MakeWayKey(S_BYTE val);
};

#endif //__MP_INI_TRACE_H__
