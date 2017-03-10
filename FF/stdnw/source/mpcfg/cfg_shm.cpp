/** @file
 *  @brief 共享内存配置类
 *  @author 
 *  @date 2011/11/07
 *  @version 0.1
 */
#include "cfg_shm.h"
#include "cfg_stat.h"
#include "cfg_cont.h"

//
//系统上下文
//
/** @brief 取系统上下文大小 */
long C_SHM::Context_GetContextSize(void)
{
	return (long)sizeof(S_CONTEXT);
}

/** @brief 取系统上下文签名 */
S_MEMSIGN *C_SHM::Context_GetMemSign(void *pContext)
{
	S_CONTEXT *pMyContext = (S_CONTEXT *)pContext;
	return &pMyContext->m_MemSign;
}

/** @brief 取系统上下文指定进程的服务信息 */
S_SERVINFO *C_SHM::Context_GetServerInfo(void *pContext, int PSN)
{
	S_CONTEXT *pMyContext = (S_CONTEXT *)pContext;
	if (PSN < 0 || PSN >= MP_PROCESS_CNT)
		return NULL;
	return &pMyContext->m_ServerInfo[PSN];
}

/** @brief 取系统上下文指定进程的跟踪配置信息 */
S_TraceSet *C_SHM::Context_GetTraceSet(void *pContext, int PSN)
{
	S_CONTEXT *pMyContext = (S_CONTEXT *)pContext;
	if (PSN < 0 || PSN >= MP_PROCESS_CNT)
		return NULL;
	return &pMyContext->m_TraceSet[PSN];
}

/** @brief 取系统上下文系统信息 */
S_SystemInfo *C_SHM::Context_GetSystemInfo(void *pContext)
{
	S_CONTEXT *pMyContext = (S_CONTEXT *)pContext;
	return &pMyContext->m_SystemInfo;
}

//
//系统状态
//
/** @brief 取系统状态大小 */
long C_SHM::Status_GetStatusSize(void)
{
	return (long)sizeof(S_STATUS);
}

/** @brief 取系统状态签名 */
S_MEMSIGN *C_SHM::Status_GetMemSign(void *pStatus)
{
	S_STATUS *pMyStatus = (S_STATUS *)pStatus;
	return &pMyStatus->m_MemSign;
}


