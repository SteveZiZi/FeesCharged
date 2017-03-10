/** @file
 *  @brief �����ڴ�������
 *  @author 
 *  @date 2011/11/07
 *  @version 0.1
 */
#include "cfg_shm.h"
#include "cfg_stat.h"
#include "cfg_cont.h"

//
//ϵͳ������
//
/** @brief ȡϵͳ�����Ĵ�С */
long C_SHM::Context_GetContextSize(void)
{
	return (long)sizeof(S_CONTEXT);
}

/** @brief ȡϵͳ������ǩ�� */
S_MEMSIGN *C_SHM::Context_GetMemSign(void *pContext)
{
	S_CONTEXT *pMyContext = (S_CONTEXT *)pContext;
	return &pMyContext->m_MemSign;
}

/** @brief ȡϵͳ������ָ�����̵ķ�����Ϣ */
S_SERVINFO *C_SHM::Context_GetServerInfo(void *pContext, int PSN)
{
	S_CONTEXT *pMyContext = (S_CONTEXT *)pContext;
	if (PSN < 0 || PSN >= MP_PROCESS_CNT)
		return NULL;
	return &pMyContext->m_ServerInfo[PSN];
}

/** @brief ȡϵͳ������ָ�����̵ĸ���������Ϣ */
S_TraceSet *C_SHM::Context_GetTraceSet(void *pContext, int PSN)
{
	S_CONTEXT *pMyContext = (S_CONTEXT *)pContext;
	if (PSN < 0 || PSN >= MP_PROCESS_CNT)
		return NULL;
	return &pMyContext->m_TraceSet[PSN];
}

/** @brief ȡϵͳ������ϵͳ��Ϣ */
S_SystemInfo *C_SHM::Context_GetSystemInfo(void *pContext)
{
	S_CONTEXT *pMyContext = (S_CONTEXT *)pContext;
	return &pMyContext->m_SystemInfo;
}

//
//ϵͳ״̬
//
/** @brief ȡϵͳ״̬��С */
long C_SHM::Status_GetStatusSize(void)
{
	return (long)sizeof(S_STATUS);
}

/** @brief ȡϵͳ״̬ǩ�� */
S_MEMSIGN *C_SHM::Status_GetMemSign(void *pStatus)
{
	S_STATUS *pMyStatus = (S_STATUS *)pStatus;
	return &pMyStatus->m_MemSign;
}


