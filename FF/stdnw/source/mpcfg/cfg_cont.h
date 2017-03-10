/** @file
 *  @brief ϵͳ�����Ľṹ������
 *  @author 
 *  @date 2011/11/07
 *  @version 0.1
 *  1�����ļ��������ļ�����Ҫ�û�����
 *  2��ϵͳ��Ķ�����ǰ�棬�û�����Ҫ�Ķ����û���Ķ���������
 *  3�������ڴ��ʹ�����ܿصģ����û���������������Ŀռ�
 */
#ifndef __MP_CFG_CONTEXT_H__
#define __MP_CFG_CONTEXT_H__
#include "traceset.h"

#ifndef WIN32
#include  "sharemem.h"
#else
#include  "..\..\..\coreassets\mprocess\sharemem.h"
#endif

#include "cfg_proc.h"
#include "server.h"
#include "basetype.h"
#include "sysbase.h"
#include "almevtbase.h"

#define ZONE_EXTEND_INFO_CONTEXT_MAX	1000

/** @class
 *  @brief ϵͳ�����Ķ���
 *  @remarks ����ֻ�������Բ����巽��
 */
struct S_CONTEXT
{
	struct S_MEMSIGN m_MemSign;///< ϵͳ�����ı�ʶ
	struct S_SERVINFO m_ServerInfo[MP_PROCESS_CNT];///<������Ϣ����
	struct S_TraceSet m_TraceSet[MP_PROCESS_CNT];///<���ٿ�����Ϣ����
	struct S_SystemInfo m_SystemInfo;///<ϵͳ��Ϣ

//user define follow me
	struct S_FlowInfo m_FlowInfo; ///< ����������Ϣ
	struct S_ConnectStatus m_ConnectStatus; ///<��վͨѶ״̬
	struct S_PowerStatus m_PowerStatus; ///< ��Դ״̬
	struct S_HttxStatus m_HttxStatus[HTTX_PROCESS_MAX]; ///< ��̨ͨѶ״̬
	struct S_DbcjStatus m_DbcjStatus[DBCJ_PROCESS_MAX];
	struct S_PppStatus m_PppStatus; ///< PPP״̬
	struct S_JlcyStatus m_JlcyStatus;
	struct S_FhkzStatus m_FhkzStatus;
	struct S_CtcyStatus m_CtcyStatus;
	struct S_LcdStatus m_LcdStatus;

// user data
	struct S_AlmDatas m_AlmDatas;
	struct S_AlmFlags m_AlmFlags;
//	struct S_ForwardTaskDatas m_ForwardTaskDatas;

//������չ��Ϣ
	S_DWORD m_ZoneExtContext[ZONE_EXTEND_INFO_CONTEXT_MAX];
};

#endif//__MP_CFG_CONTEXT_H__

