/** @file
 *  @brief ϵͳ״̬�ṹ������
 *  @author 
 *  @date 2011/11/07
 *  @version 0.1
 *  1�����ļ��������ļ�����Ҫ�û�����
 *  2��ϵͳ��Ķ�����ǰ�棬�û�����Ҫ�Ķ����û���Ķ���������
 *  3�������ڴ��ʹ�����ܿصģ����û���������������Ŀռ�
 */
#ifndef __MP_CFG_STATUS_H__
#define __MP_CFG_STATUS_H__

#ifndef WIN32
#include  "sharemem.h"
#else
#include  "..\..\..\coreassets\mprocess\sharemem.h"
#endif

#include "sysbase.h"
#include "almevtbase.h"

#define ZONE_EXTEND_INFO_STATUS_MAX	1000

/** @class
 *  @brief ϵͳ״̬����
 *  @remarks ����ֻ�������Բ����巽��
 */
struct S_STATUS
{
	struct S_MEMSIGN m_MemSign;///<ϵͳ״̬��ʶ

//user define follow me
//data area
	struct S_TermData m_TermData;
	struct S_TaskStatus m_TaskStatus[DBCJ_PROCESS_MAX];///< DBCJ������Ϣ

//config area
	struct S_SYSCFG m_SysConfig; ///< ϵͳ����
	struct S_SYSRUN m_SysRun; ///< ϵͳ������Ϣ
	struct S_IpConfig m_IpCofig; ///< IP INFO
	struct S_ForwardInfo m_ForwardInfo;
	struct S_AlarmInfo m_AlarmInfo; ///< �澯������Ϣ
	struct S_PowerLimit m_PowerLimit; ///< ��ֵ����
	struct S_DbcjParam m_DbcjParam; ///< ��Ʋɼ���ز���

	struct S_UpChannel m_UpChannels; ///< ����ͨ����Ϣ
	struct S_DnChannel m_DnChannels; ///< ����ͨ����Ϣ
	struct S_HttxInfo m_HttxInfo[HTTX_PROCESS_MAX]; ///< HTTX��Ϣ
	struct S_DbcjInfo m_DbcjInfo[DBCJ_PROCESS_MAX]; ///< dbcj ������Ϣ
	struct S_PppInfo m_PppInfo; ///< PPP��Ϣ
	struct S_LcdInfo m_LcdInfo; ///< lcd������Ϣ
	struct S_JlcyInfo m_JlcyInfo; ///< ������Ϣ
	struct S_FhkzInfo m_FhkzInfo; ///< ���ز���
	struct S_CtcyInfo m_CtcyInfo; ///< CT����

//measure poing area
	struct S_MpInfos m_MpInfos;
	struct S_MeterInfos m_MeterInfos; ///< ��ƺͽ���
	struct S_ANALOGS m_AnalogInfo; ///< ģ����
	struct S_PULSES m_PulseInfo; ///< ������
	struct S_SUMS m_SumInfo; ///< ������
	struct S_DiffRuleS m_DiffRuleInfo; ///< �����
	struct S_ALLMP m_ALLMP; // ���в�����ͳ����Ϣ
	struct S_MPLS m_MPointLimits;//��������ֵ����
	
	struct S_NormalTaskInfos m_NormalTaskInfos;
	struct S_ForwardTaskInfos m_ForwardTaskInfos;
	struct S_GatherTaskInfos m_GatherTaskInfos;	//���������Ϣ

//register area
	struct S_TaskTemplates m_TaskTemplates; ///< ����ģ��
	struct S_CommRegisters m_CommRegisters; ///Common Register
	struct S_MSDIS m_MSDIS; ///MS Register

	struct S_PWRCtrlInfo m_PWRCtrlInfo;//���ʿ���
	struct S_DLCtrlInfo m_DLCtrlInfo;//��������

	//struct S_AlmDatas m_AlmDatas;
	struct S_AlarmStatus m_AlarmStatus; ///< �澯״̬

//������չ��Ϣ
	S_DWORD m_ZoneExtStatus[ZONE_EXTEND_INFO_STATUS_MAX];
};

#endif//__MP_CFG_STATUS_H__


