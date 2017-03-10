/** @file
 *  @brief �û���ϵͳ������������
 *  @author 
 *  @date 2011/12/06
 *  @version 0.1
 *  @see mprocess/sharemem.h mrpocess/sysipc.h
 *  ע����Ҫ�̳�C_ContextBase�࣬ע�����еĳ��󷽷������ء�
 */
#include "context.h"
#include "ini_sys.h"
#include "ini_serv.h"
#include "zonebase.h"
#include "sysctrl.h"

C_CONTEXT &C_CONTEXT::Instance(void)
{
	static C_CONTEXT s_MyContext;
	return s_MyContext;
}

/** @brief ���ó�Ա��Ϣ��
 *  @return true �ɹ���false ʧ��
 *  @remarks ��������ʱ���Ȼص��������������ڴ洴���ɹ�ʱ���ñ�����
 */
bool C_CONTEXT::SetInformation(void)
{
	if (C_ContextBase::SetInformation() == false)
		return false;
	m_pMyContext = (S_CONTEXT*)m_pContext;
	return true;
}

/** @brief ��ʼ��ϵͳ������
 *  @return true �ɹ���false ʧ��
 *  @remarks ��������ʱ���Ȼص�������
 */
bool C_CONTEXT::InitContext(void)
{
	if (C_ContextBase::InitContext() != true)
		return false;

	m_pMyContext->m_FlowInfo.Init();
	m_pMyContext->m_ConnectStatus.Init();
	m_pMyContext->m_PowerStatus.Init();

	for (int i = 0; i < HTTX_PROCESS_MAX; i++)
		m_pMyContext->m_HttxStatus[i].Init();

	m_pMyContext->m_PppStatus.Init(); ///< PPP״̬
	m_pMyContext->m_JlcyStatus.Init();
	m_pMyContext->m_FhkzStatus.Init();

//	m_pMyContext->m_ForwardTaskDatas.Init();

	C_ZoneShm *pZoneShm = C_ZoneShm::GetZoneSharedMemoryObject();
	if (pZoneShm != NULL)
		pZoneShm->InitExtendZone_Context();

	return true;
}

/** @brief �������õ�ϵͳ������
 *  @return true �ɹ���false ʧ��
 *  @remarks ��������ʱ���Ȼص�������
 */
bool C_CONTEXT::LoadContext(void)
{
	if (C_ContextBase::LoadContext() != true)
		return false;

	C_IniSys::LoadFlowCtrlInfo(m_pMyContext);
	
	/* FN��GatherReg����*/
//	CGatherFnRegCfg::LoadFnRegItems(m_pMyContext);

	C_ZoneShm *pZoneShm = C_ZoneShm::GetZoneSharedMemoryObject();
	if (pZoneShm != NULL)
		pZoneShm->LoadExtendZone_Context();

	return true;
}

/** @brief ��������
 *  @return true �ɹ���false ʧ��
 *  @remarks ��������ʱ���Ȼص�������
 */
bool C_CONTEXT::ParamParse(void)
{
	if (C_ContextBase::ParamParse() != true)
		return false;

	return true;
}

/** @brief ��������λʱ�������
 *  @return true �ɹ���false ʧ��
 *  @remarks ��������ʱ���Ȼص�������
 */
bool C_CONTEXT::ClearData(void)
{
	if (C_ContextBase::ClearData() != true)
		return false;

	return true;
}

bool C_CONTEXT::CheckProgramkeyState(bool fRefresh)
{
	S_LcdStatus &LcdStatus = m_pMyContext->m_LcdStatus;
	if (LcdStatus.m_ProgramKeyDelayS > 0 && fRefresh == true)
		LcdStatus.m_ProgramKeyDelayS = LCD_PROGRAM_KEY_DELAY_S;
	return LcdStatus.m_ProgramKeyDelayS > 0;
}

/** @brief ��ȡ������չ��Ϣ�洢�� */
void *C_CONTEXT::GetZoneExtendInfo(size_t size)
{
	if (size > sizeof(m_pMyContext->m_ZoneExtContext))
		return NULL;
	return (void *)m_pMyContext->m_ZoneExtContext;
}

/* return -1 ��ʾ����ʧ��, ���򷵻����*/
int C_CONTEXT::GetDbcjPrecess_MP(S_WORD MP)
{
	for (int i = 0; i < DBCJ_PROCESS_MAX; i++)
	{
		S_DbcjStatus &Status = m_pMyContext->m_DbcjStatus[i];
		if (Status.m_mpbsMyMeter.test(MP) == true)
			return i;
	}
	return -1;
}
int C_CONTEXT::GetDbcjPrecess_VCom(S_BYTE VCom)
{
	for (int i = 0; i < DBCJ_PROCESS_MAX; i++)
	{
		S_DbcjStatus &Status = m_pMyContext->m_DbcjStatus[i];
		if (Status.m_bsMyVCom.test(VCom) == true)
			return i;
	}
	return -1;
}

const char *C_CONTEXT::GetDbcjPrecessName_MP(S_WORD MP)
{
	return C_IniServer::GetDbcjProcessName(GetDbcjPrecess_MP(MP));
}

const char *C_CONTEXT::GetDbcjPrecessName_VCom(S_BYTE VCom)
{
	return C_IniServer::GetDbcjProcessName(GetDbcjPrecess_VCom(VCom));
}

bool C_CONTEXT::NotifyDbcj(int Msg)
{
	S_TIME timeCurrent = C_TIME::TimeNow();
	for (int DbcjSn = 0; DbcjSn < DBCJ_PROCESS_MAX; DbcjSn++)
	{
		S_DbcjStatus *pDbcjStatus = &m_pMyContext->m_DbcjStatus[DbcjSn];
		switch (Msg)
		{
		case NOTIFY_DBCJ_ADD_METER:pDbcjStatus->m_timeAddMeter = timeCurrent; break;
		case NOTIFY_DBCJ_ADD_TASK:pDbcjStatus->m_timeAddTask = timeCurrent; break;
		case NOTIFY_DBCJ_ADD_ALARM:pDbcjStatus->m_timeAddAlarm = timeCurrent; break;
		default:break;
		}
	}
	return true;
}

/** @brief ����LCDָʾ��
 *  @param[in] pProcessName ������
 *  @param[in] Indicator �μ�LCD_INDICATOR_
 */
bool C_CONTEXT::SetLcdBottomInfoIndicator(const char *pProcessName, S_BYTE Indicator)
{
	if (strcmp(pProcessName, MP_PROCESS_PPP) == 0)
	{
		if (Indicator >= LCD_INDICATOR_PPP_MAX)
			return false;
		m_pMyContext->m_PppStatus.m_LcdBottomInfoIndicator = Indicator;
	}
	else if (strcmp(pProcessName, MP_PROCESS_DBCJ1) == 0)
	{
		//if (Indicator >= LCD_INDICATOR_DBCJ_MAX)
		//	return false;
		//m_pMyContext-> TODO:
	}
	return true;
}

const char *C_CONTEXT::GetLcdBottomInfo(const char *pProcessName)
{
	static const char *LcdBottomInfoPpp[] = 
	{
		"MODEM�����ϵ�...",
		"���MODEM...    ",
		"��������ź�... ",
		"PPP���ڲ���...  ",
		"PPP���ӳɹ�...  ",
		"PPP����ʧ��     "
	};
	static const char *LcdBottomInfoDbcj[] = 
	{
		"��ʼ���ɼ�ϵͳ..",
		"�������        ", //SYS_TASK_IDLE,
		"����·����...   ", //SYS_TASK_ROUTE,
		"��Ӧʵʱ����... ", //SYS_TASK_REAL_TIME,
		"���ڽ��й㲥Уʱ", //SYS_TASK_BROADCAST,
		"��Ӧ��վ����... ", //SYS_TASK_ONE_OFF,
		"���ڱ���ϱ���..", //SYS_TASK_SEEK_METER,
		"����·����...   ", //USER_TASK_ROUTE_EX = SYS_TASK_MAX,
		"�ɼ���������... ", //USER_TASK_VIP,
		"�ɼ��ն�������..", //USER_TASK_DAY,
		"�ɼ��¶�������..", //USER_TASK_MON,
		"�����ն�������..", //USER_TASK_FIX_DAY,
		"�ɼ�˲ʱ����... ", //USER_TASK_POLLING,
		"�����ն���������", //USER_TASK_TERM_VIP,
		"�����ն�������..", //USER_TASK_TERM_DAY,
		"�����ն�������..", //USER_TASK_TERM_MON,
		"ͳ��������...   ", //USER_TASK_STAT_DAY,
		"ͳ��������...   ", //USER_TASK_STAT_MON,
		"�ɼ��澯����... ", //USER_TASK_POLL_ALARM,
		"�ɼ��澯����... ", //USER_TASK_DAY_ALARM,
		"���ڽ�����������", //USER_TASK_TYPE_ID_MAX,
	};
	if (strcmp(pProcessName, MP_PROCESS_PPP) == 0)
	{
		return LcdBottomInfoPpp[m_pMyContext->m_PppStatus.m_LcdBottomInfoIndicator];
	}
	else if (strcmp(pProcessName, MP_PROCESS_DBCJ1) == 0)
	{
		S_BYTE Indicator = m_pMyContext->m_DbcjStatus[MP_DBCJ_1].m_LcdBottomInfoIndicator;
		if (Indicator == 1 && C_SYSCTRL::GetProcessEnable(MP_PROCESS_DBCJ4) == true) // dbcj1������ȡdbcj4��״̬
			Indicator = m_pMyContext->m_DbcjStatus[MP_DBCJ_4].m_LcdBottomInfoIndicator;
		if (Indicator >= KGE_MEMBER_OF(LcdBottomInfoDbcj))
			Indicator = KGE_MEMBER_OF(LcdBottomInfoDbcj) - 1;
		return LcdBottomInfoDbcj[Indicator];
	}
	return NULL;
}



