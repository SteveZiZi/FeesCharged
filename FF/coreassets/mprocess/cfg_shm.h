/** @file
 *  @brief �����ڴ������
 *  @author 
 *  @date 2011/11/07
 *  @version 0.1
 *  ����ϵͳ�����ĺ�ϵͳ״̬����Ӧ�ò㣬
 *  ���ļ��ṩ�˿�ܵ���Ӧ�ò�Ľӿڡ�
 */
#ifndef __MP_CFG_SHM_H__
#define __MP_CFG_SHM_H__
#include "sharemem.h"
#include "server.h"
#include "traceset.h"


/** @class
 *  @brief �����ڴ�ת����
 *  @note
 *  ��������Ӧ�ò�Ĺ����ڴ�ָ����ýӿڵ���ʽȡ����Ӧ�ò���Ҫ��д���ӿ�ʵ��
 */
class C_SHM
{
public:
	/** ϵͳ�����Ĳ��������� */
	/** @brief ȡϵͳ�����Ĵ�С */
    static long Context_GetContextSize(void);
	/** @brief ȡϵͳ������ǩ�� */
	static S_MEMSIGN *Context_GetMemSign(void *pContext);
	/** @brief ȡϵͳ������ָ�����̵ķ�����Ϣ */
	static S_SERVINFO *Context_GetServerInfo(void *pContext, int PSN);
	/** @brief ȡϵͳ������ָ�����̵ĸ���������Ϣ */
	static S_TraceSet *Context_GetTraceSet(void *pContext, int PSN);
	/** @brief ȡϵͳ������ϵͳ��Ϣ */
	static S_SystemInfo *Context_GetSystemInfo(void *pContext);

	/** ϵͳ״̬���������� */
	/** @brief ȡϵͳ״̬��С */
	static long Status_GetStatusSize(void);
	/** @brief ȡϵͳ״̬ǩ�� */
	static S_MEMSIGN *Status_GetMemSign(void *pStatus);
};


#endif//__MP_CFG_SHM_H__

