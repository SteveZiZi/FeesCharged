/** @file
 *  @brief 共享内存操作类
 *  @author 
 *  @date 2011/11/07
 *  @version 0.1
 *  由于系统上下文和系统状态属于应用层，
 *  本文件提供了框架调用应用层的接口。
 */
#ifndef __MP_CFG_SHM_H__
#define __MP_CFG_SHM_H__
#include "sharemem.h"
#include "server.h"
#include "traceset.h"


/** @class
 *  @brief 共享内存转换类
 *  @note
 *  本函数将应用层的共享内存指针采用接口的形式取出，应用层需要重写本接口实现
 */
class C_SHM
{
public:
	/** 系统上下文操作方法集 */
	/** @brief 取系统上下文大小 */
    static long Context_GetContextSize(void);
	/** @brief 取系统上下文签名 */
	static S_MEMSIGN *Context_GetMemSign(void *pContext);
	/** @brief 取系统上下文指定进程的服务信息 */
	static S_SERVINFO *Context_GetServerInfo(void *pContext, int PSN);
	/** @brief 取系统上下文指定进程的跟踪配置信息 */
	static S_TraceSet *Context_GetTraceSet(void *pContext, int PSN);
	/** @brief 取系统上下文系统信息 */
	static S_SystemInfo *Context_GetSystemInfo(void *pContext);

	/** 系统状态操作方法集 */
	/** @brief 取系统状态大小 */
	static long Status_GetStatusSize(void);
	/** @brief 取系统状态签名 */
	static S_MEMSIGN *Status_GetMemSign(void *pStatus);
};


#endif//__MP_CFG_SHM_H__

