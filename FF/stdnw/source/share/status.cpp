/** @file
 *  @brief �û���ϵͳ״̬������
 *  @author 
 *  @date 2011/12/06
 *  @version 0.1
 *  @see mprocess/sharemem.h mrpocess/sysipc.h
 *  ע����Ҫ�̳�C_StatusBase�࣬ע�����еĳ��󷽷������ء�
 */
#include "status.h"
#include "ini_sys.h"
#include "ini_serv.h"
#include "ini_mp.h"
#include "ini_fkctrl.h"
#include "timeop.h"
#include "alarm.h"
#include "alarmEventRecord.h"
#include "almfact.h"
#include "zonebase.h"

C_STATUS &C_STATUS::Instance(void)
{
	static C_STATUS s_MyStatus;
	return s_MyStatus;
}

/** @brief ���ó�Ա��Ϣ��
 *  @return true �ɹ���false ʧ��
 *  @remarks ��������ʱ���Ȼص��������������ڴ洴���ɹ�ʱ���ñ�����
 */
bool C_STATUS::SetInformation(void)
{
	if (C_StatusBase::SetInformation() == false)
		return false;
	m_pMyStatus = (S_STATUS *)m_pStatus;
	return true;
}

/** @brief ��ʼ��ϵͳ״̬
 *  @return true �ɹ���false ʧ��
 *  @remarks ��������ʱ���Ȼص�������
 */
bool C_STATUS::InitStatus(void)
{
	if (C_StatusBase::InitStatus() != true)
		return false;

	m_pMyStatus->m_TermData.Init();
	m_pMyStatus->m_TaskTemplates.Init();

	m_pMyStatus->m_SysConfig.Init();
	m_pMyStatus->m_SysRun.Init();
	m_pMyStatus->m_IpCofig.Init();
	m_pMyStatus->m_MeterInfos.Init();
	m_pMyStatus->m_ForwardInfo.Init();
	m_pMyStatus->m_AlarmInfo.Init();
	m_pMyStatus->m_PowerLimit.Init();
	m_pMyStatus->m_NormalTaskInfos.Init();
	m_pMyStatus->m_ForwardTaskInfos.Init();
	m_pMyStatus->m_PWRCtrlInfo.Init();//���ع��ʿ���
	m_pMyStatus->m_DLCtrlInfo.Init();//��������
	m_pMyStatus->m_DbcjParam.Init();// ��Ʋɼ���ز���
	m_pMyStatus->m_MPointLimits.Init();//��������ֵ����
		
	m_pMyStatus->m_UpChannels.Init(); ///< ����ͨ����Ϣ
	m_pMyStatus->m_DnChannels.Init(); ///< ����ͨ����Ϣ

	for (int i = 0; i < HTTX_PROCESS_MAX; i++)
		m_pMyStatus->m_HttxInfo[i].Init();
	for (int i = 0; i < DBCJ_PROCESS_MAX; i++)
		m_pMyStatus->m_DbcjInfo[i].Init();
	m_pMyStatus->m_PppInfo.Init();
	m_pMyStatus->m_LcdInfo.Init();
	m_pMyStatus->m_JlcyInfo.Init();
	m_pMyStatus->m_FhkzInfo.Init();

	/** ������*/
	m_pMyStatus->m_AnalogInfo.Init(); ///< ģ����
	m_pMyStatus->m_PulseInfo.Init(); ///< ������
	m_pMyStatus->m_SumInfo.Init(); ///< ������
	m_pMyStatus->m_DiffRuleInfo.Init(); ///< �����

	m_pMyStatus->m_AlarmStatus.Init(); ///< �澯״̬

	C_ZoneShm *pZoneShm = C_ZoneShm::GetZoneSharedMemoryObject();
	if (pZoneShm != NULL)
		pZoneShm->InitExtendZone_Status();

	return true;
}

/** @brief �������õ�ϵͳ״̬
 *  @return true �ɹ���false ʧ��
 *  @remarks ��������ʱ���Ȼص�������
 */
bool C_STATUS::LoadStatus(void)
{
	if (C_StatusBase::LoadStatus() != true)
		return false;

	m_pMyStatus->m_MpInfos.Init();//��MP�ǹ���ʹ�ýṹ����Ҫ�ڸ���ģ��ʹ��ǰ��ʼ��

	logwt<<"��ʼ���������ļ�..."<<endl;
	C_IniSys::LoadConfigVersion(m_pMyStatus);
	C_IniSys::LoadSystemConfig(m_pMyStatus);
	C_IniSys::LoadRunInformation(m_pMyStatus);
	C_IniSys::LoadPortMap(m_pMyStatus);
	C_IniSys::LoadCommReg(m_pMyStatus);
	C_IniSys::LoadTaskTemplateInfo(m_pMyStatus);
	C_IniSys::LoadMsdiInfo(m_pMyStatus);

	C_IniSys::LoadTermIPConfig(m_pMyStatus);
	C_IniSys::LoadForwardInfo(m_pMyStatus);
	C_IniSys::LoadAlarmInfo(m_pMyStatus);
	C_IniSys::LoadPowerLimitInfo(m_pMyStatus);
	C_IniSys::LoadAnalogInfo(m_pMyStatus);
	C_IniSys::LoadPulseInfo(m_pMyStatus);
	C_IniSys::LoadSumInfo(m_pMyStatus);
	C_IniSys::LoadDiffRuleInfo(m_pMyStatus);

	C_IniFKCtrl::LoadPWRCtrlInfo(m_pMyStatus);
	C_IniFKCtrl::LoadDLCtrlInfo(m_pMyStatus);

	C_IniServer::LoadUpChannl(m_pMyStatus);
	C_IniServer::LoadDnChannl(m_pMyStatus);
	C_IniServer::LoadHttxInfo(m_pMyStatus);
	C_IniServer::LoadPppInfo(m_pMyStatus);
	C_IniServer::LoadLcdInfo(m_pMyStatus);
	C_IniServer::LoadDbcjInfo(m_pMyStatus);
	C_IniServer::LoadJlcyInfo(m_pMyStatus);
	C_IniServer::LoadFhkzInfo(m_pMyStatus);
	C_IniServer::LoadCtcyInfo(m_pMyStatus);	

	C_IniMP::LoadMeterInformation(m_pMyStatus);
	C_IniMPLimit::LoadMPointLimit(m_pMyStatus);
	CInitTask::LoadGatherTasks(m_pMyStatus);
	CInitTask::LoadNormalTask(m_pMyStatus);
	CInitTask::LoadForwardTask(m_pMyStatus);

	logwt<<"���������ļ�����!!!"<<endl;

	C_ZoneShm *pZoneShm = C_ZoneShm::GetZoneSharedMemoryObject();
	if (pZoneShm != NULL)
		pZoneShm->LoadExtendZone_Status();

	return true;
}

/** @brief ��������
 *  @return true �ɹ���false ʧ��
 *  @remarks ��������ʱ���Ȼص�������
 */
bool C_STATUS::ParamParse(void)
{
	if (C_StatusBase::ParamParse() != true)
		return false;

	//parse meter info
	S_BYTE PortZB = 0xFF;
	S_CHANNEL *pZB = STATUS.GetDbcjZbwxChannel();
	if (pZB != NULL)
		PortZB = pZB->m_PortSet.m_ComSet.m_PortNO;
	m_pMyStatus->m_ALLMP.RefreshMeterInfo(PortZB, &m_pMyStatus->m_MeterInfos, &m_pMyStatus->m_MpInfos);

	//���帺��Ӳ��״̬������
	return true;
}

/** @brief ��������λʱ�������
 *  @return true �ɹ���false ʧ��
 *  @remarks ��������ʱ���Ȼص�������
 */
bool C_STATUS::ClearData(void)
{
	if (C_StatusBase::ClearData() != true)
		return false;

	for (int i = 0; i < DBCJ_PROCESS_MAX; i++)
		m_pMyStatus->m_TaskStatus[i].ClearData();
	return true;
}

S_CHANNEL *C_STATUS::GetHttxChannel(S_HttxSN HttxSN, S_ChannelSN ChannelSN)
{
	if (HttxSN >= HTTX_PROCESS_MAX || ChannelSN >= HTTX_CHANNL_MAX)
		return NULL;
	return m_pMyStatus->m_UpChannels.FindChannel(m_pMyStatus->m_HttxInfo[HttxSN].m_ChannlName[ChannelSN]);
}

S_CHANNEL *C_STATUS::GetForwardChannel(int ChannelSN)
{
	if (m_pMyStatus->m_ForwardInfo.m_Enable == false)
		return NULL;
	if (ChannelSN < 0 || ChannelSN >= FORWARD_CHANNL_MAX)
		return NULL;
	return m_pMyStatus->m_UpChannels.FindChannel(m_pMyStatus->m_ForwardInfo.m_ChannlName[ChannelSN]);
}

S_CHANNEL *C_STATUS::GetPppChannel(int ChannelSN)
{
	if (ChannelSN < 0 || ChannelSN >= PPP_CHANNL_MAX)
		return NULL;
	return m_pMyStatus->m_UpChannels.FindChannel(m_pMyStatus->m_PppInfo.m_ChannlName[ChannelSN]);
}

S_CHANNEL *C_STATUS::GetJlcyChannel(int ChannelSN)
{
	if (ChannelSN < 0 || ChannelSN >= JLCY_CHANNL_MAX)
		return NULL;
	return m_pMyStatus->m_DnChannels.FindChannel(m_pMyStatus->m_JlcyInfo.m_ChannlName[ChannelSN]);
}

S_CHANNEL *C_STATUS::GetDbcjChannel(S_DbcjSN DbcjSN, int ChannelSN)
{
	if (ChannelSN < 0 || ChannelSN >= DBCJ_CHANNL_MAX)
		return NULL;
	return m_pMyStatus->m_DnChannels.FindChannel(m_pMyStatus->m_DbcjInfo[DbcjSN].m_ChannlName[ChannelSN]);
}

S_CHANNEL *C_STATUS::GetDbcjZbwxChannel(void)
{
	for (int sn = MP_DBCJ_1; sn < DBCJ_PROCESS_MAX; sn++)
	{
		S_CHANNEL *pChannel = GetDbcjChannel((S_DbcjSN)sn, DBCJ_CHANNL_ZBWX);
		if (pChannel != NULL)
			return pChannel;
	}
	return NULL;
}

void C_STATUS::RefreshMeterInfo(void)
{
	S_BYTE PortZB = 0xFF;
	S_CHANNEL *pZB = STATUS.GetDbcjZbwxChannel();
	if (pZB != NULL)
		PortZB = pZB->m_PortSet.m_ComSet.m_PortNO;
	m_pMyStatus->m_ALLMP.RefreshMeterInfo(PortZB, &m_pMyStatus->m_MeterInfos, &m_pMyStatus->m_MpInfos);
	C_IniMP::SaveMeterInformation(m_pMyStatus);
}

/** @brief ��ȡ�ɼ���Ϣ
 *  @param[in] MP ������������
 *  @param[out] MeterBase �����Ϣ
 *  @return 0:�ɹ�; <0:ʧ��
 */
int C_STATUS::GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase)
{
	if (mpMeter >= CONS_MP_COUNT_MAX)
		return -1;

	S_MpInfos *pMpInfos = STATUS.GetMpInfos();
	if (pMpInfos->m_Item[mpMeter].m_Type != MP_TYPE_METER)
		return -1;

	S_WORD idxMeter = pMpInfos->m_Item[mpMeter].m_SN;
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	if (pMeterInfos->m_Item[idxMeter].m_Enable == false)
		return -1;

	pMeterInfos->m_Item[idxMeter].ExtractBaseInfo(MeterBase);
	return 0;
}

S_GatherTaskInfo* C_STATUS::GetGatherTaskInfo(INT8U taskNo)
{
	return &(m_pMyStatus->m_GatherTaskInfos.m_ItemInfo[taskNo]);
}

/** @brief ��ȡ������չ��Ϣ�洢�� */
void *C_STATUS::GetZoneExtendInfo(size_t size)
{
	if (size > sizeof(m_pMyStatus->m_ZoneExtStatus))
		return NULL;
	return (void *)m_pMyStatus->m_ZoneExtStatus;
}

/** �����������ȡ���� */
S_BYTE C_STATUS::GetReactivePowerOperator(S_WORD mp)
{
	return 0;
}
/** @brief register area */
S_REGISTER C_STATUS::FindCommonRegister(S_DWORD ID)
{
	S_REGISTER Register;

	Register.m_ID = ID;
	S_REGISTER *begin = m_pMyStatus->m_CommRegisters.m_Item;
	S_REGISTER *end = m_pMyStatus->m_CommRegisters.m_Item + m_pMyStatus->m_CommRegisters.m_Cnt;

	S_REGISTER *iter = lower_bound(begin, end, Register);
	if (iter == end || iter->m_ID != ID)
		return S_REGISTER();
	return *iter;
}

S_TaskInfo C_STATUS::FindTaskTemplateInfo(const char *pTaskName)
{
	S_TaskInfo TaskInfo;
	S_TaskTemplate *pTaskTemplate = NULL;
	for (int i = 0; i < TASK_TEMPLATE_MAX; i++)
	{
		if (strcmp(pTaskName, m_pMyStatus->m_TaskTemplates.m_Item[i].m_TaskType) == 0)
		{
			pTaskTemplate = &m_pMyStatus->m_TaskTemplates.m_Item[i];
			break;
		}
	}

	if (pTaskTemplate != NULL)
	{
		TaskInfo.m_Enable = pTaskTemplate->m_Enable;
		strcpy(TaskInfo.m_TypeName, pTaskTemplate->m_TaskType);
		TaskInfo.m_Priority = pTaskTemplate->m_Priority;///< ���ȼ���Խ���ʾ���ȼ�Խ��
		TaskInfo.m_TimeUnit = pTaskTemplate->m_TimeUnit;///< ���ڵ�λ
		TaskInfo.m_PeriodVal = pTaskTemplate->m_PeriodVal;///< ����ֵ:0��ʾ��������
		TaskInfo.m_RetryTimes = pTaskTemplate->m_RetryTimes;///< һ�����������Դ���:0��ʾ���޴���
		TaskInfo.m_MaxStartTimes = pTaskTemplate->m_MaxStartTimes;///< �����������:0��ʾ���޴���
		TaskInfo.m_StandardTime = pTaskTemplate->m_StandardTime;///< ��׼��ʼʱ��
		TaskInfo.m_ExecTimeS = pTaskTemplate->m_ExecTimeS;///< ���������ִ��ʱ�䣬��λ��,0��ʾ����ʱ��
		TaskInfo.m_ReportPeriodS = pTaskTemplate->m_ReportPeriodS;///< ���������ϱ����ڣ�����ϱ�����
	}

	return TaskInfo;
}

S_MSDI C_STATUS::FindMasterStationDataID(S_BYTE Class, S_DWORD Idx)
{
	S_MSDI msdi;
	S_MSDIF *pMSDIF = NULL;

	Idx = Idx % MSDI_ITEM_MAX; //�淶IDֵ
	if (Class == CLASSDATA_1) pMSDIF = &m_pMyStatus->m_MSDIS.m_ItemClass1[Idx];
	else if (Class == CLASSDATA_2) pMSDIF = &m_pMyStatus->m_MSDIS.m_ItemClass2[Idx];
	else if (Class == CLASSDATA_3) pMSDIF = &m_pMyStatus->m_MSDIS.m_ItemClass3[Idx];
	else if (Class == CLASSDATA_4) pMSDIF = &m_pMyStatus->m_MSDIS.m_ItemClass4[Idx];
	else if (Class == CLASSDATA_5) pMSDIF = &m_pMyStatus->m_MSDIS.m_ItemClass5[Idx];
	else if (Class == CLASSDATA_6) pMSDIF = &m_pMyStatus->m_MSDIS.m_ItemClass6[Idx];
	else if (Class == CLASSDATA_7) pMSDIF = &m_pMyStatus->m_MSDIS.m_ItemClass7[Idx];
	else if (Class == CLASSDATA_8) pMSDIF = &m_pMyStatus->m_MSDIS.m_ItemClass8[Idx];
	else if (Class == CLASSDATA_9) pMSDIF = &m_pMyStatus->m_MSDIS.m_ItemClass9[Idx];
	else return msdi;

//	if (pMSDIF->m_Enable == false)
//		return msdi;
	if (pMSDIF->m_idxRegEnd - pMSDIF->m_idxRegBegin > MSDI_COMM_REG_MAX)
		return msdi;
	
	msdi.m_Enable = pMSDIF->m_Enable;
	msdi.m_Class = Class;
	msdi.m_MSDI = (S_BYTE)Idx;
	msdi.m_Type = pMSDIF->m_Type;
	msdi.m_Freeze = pMSDIF->m_Freeze;
	msdi.m_DI = pMSDIF->m_DI;
	msdi.m_Flag = pMSDIF->m_Flag;
	msdi.m_TariffCnt = pMSDIF->m_TariffCnt;
	msdi.m_DisableCommReg = pMSDIF->m_DisableReg;
	for (S_WORD i = pMSDIF->m_idxRegBegin; i < pMSDIF->m_idxRegEnd; i++)
	{
		msdi.m_CommRegID[i - pMSDIF->m_idxRegBegin] = m_pMyStatus->m_MSDIS.m_CommRegID[i];
	}

	return msdi;
}

S_MSDI C_STATUS::FindMasterStationDataID(S_BYTE Class, S_DWORD DI, S_DWORD dummy)
{
	S_MSDI msdi;
	S_MSDIF *pMSDIF = NULL;

	if (Class == CLASSDATA_1) pMSDIF = &m_pMyStatus->m_MSDIS.m_ItemClass1[0];
	else if (Class == CLASSDATA_2) pMSDIF = &m_pMyStatus->m_MSDIS.m_ItemClass2[0];
	else if (Class == CLASSDATA_3) pMSDIF = &m_pMyStatus->m_MSDIS.m_ItemClass3[0];
	else if (Class == CLASSDATA_4) pMSDIF = &m_pMyStatus->m_MSDIS.m_ItemClass4[0];
	else if (Class == CLASSDATA_5) pMSDIF = &m_pMyStatus->m_MSDIS.m_ItemClass5[0];
	else if (Class == CLASSDATA_6) pMSDIF = &m_pMyStatus->m_MSDIS.m_ItemClass6[0];
	else if (Class == CLASSDATA_7) pMSDIF = &m_pMyStatus->m_MSDIS.m_ItemClass7[0];
	else if (Class == CLASSDATA_8) pMSDIF = &m_pMyStatus->m_MSDIS.m_ItemClass8[0];
	else if (Class == CLASSDATA_9) pMSDIF = &m_pMyStatus->m_MSDIS.m_ItemClass9[0];
	else return msdi;

	for (S_WORD idx = 0; idx < MSDI_ITEM_MAX; idx++,pMSDIF += 1)
	{
//		if (pMSDIF->m_Enable == false)
//			continue;
		if (pMSDIF->m_DI == DI)
			return FindMasterStationDataID(Class, idx);
	}

	return msdi;
}


/** @brief �ն˸�λ����*/
void C_STATUS::IncResetTimes(void)
{
	m_pMyStatus->m_TermData.m_ResetData.m_DayResetTimes += 1;
	m_pMyStatus->m_TermData.m_ResetData.m_MonResetTimes += 1;
}
S_WORD C_STATUS::GetFreezeDayResetTimes(void)
{
	S_ResetData &ResetData = m_pMyStatus->m_TermData.m_ResetData;
	S_TIME NowDay = C_TIME::DateTrim(TIME_UNIT_DAY, C_TIME::TimeNow());
	if (NowDay == ResetData.m_timeFreezeDay)
		return ResetData.m_FreezeDayResetTimes;

	ResetData.m_timeFreezeDay = NowDay;
	ResetData.m_FreezeDayResetTimes = ResetData.m_DayResetTimes;
	ResetData.m_DayResetTimes = 0x00;
	return ResetData.m_FreezeDayResetTimes;
}
S_WORD C_STATUS::GetFreezeMonResetTimes(void)
{
	S_ResetData &ResetData = m_pMyStatus->m_TermData.m_ResetData;
	S_TIME NowMon = C_TIME::DateTrim(TIME_UNIT_MONTH, C_TIME::TimeNow());
	if (NowMon == ResetData.m_timeFreezeMon)
		return ResetData.m_FreezeMonResetTimes;

	ResetData.m_timeFreezeMon = NowMon;
	ResetData.m_FreezeMonResetTimes = ResetData.m_MonResetTimes;
	ResetData.m_MonResetTimes = 0x00;
	return ResetData.m_FreezeMonResetTimes;
}

/** @brief �ն˹���ʱ�亯��*/
void C_STATUS::IncPowerSupplyTime(S_WORD PowerSupplyM)
{
	m_pMyStatus->m_TermData.m_PowerSupplyData.m_PowerSupplyM += PowerSupplyM;
}
S_WORD C_STATUS::GetDayPowerSupplyM(void)
{
	S_PowerSupplyData &PowerData = m_pMyStatus->m_TermData.m_PowerSupplyData;
	if (PowerData.m_PowerSupplyM >= PowerData.m_LastDayPowerSupplyM)
		return (S_WORD)(PowerData.m_PowerSupplyM - PowerData.m_LastDayPowerSupplyM);
	S_DWORD MaxDword = 0xFFFFFFFFUL;//�����
	return (S_WORD)(MaxDword - PowerData.m_LastDayPowerSupplyM + PowerData.m_PowerSupplyM);
}
S_WORD C_STATUS::GetMonPowerSupplyM(void)
{
	S_PowerSupplyData &PowerData = m_pMyStatus->m_TermData.m_PowerSupplyData;
	if (PowerData.m_PowerSupplyM >= PowerData.m_LastMonPowerSupplyM)
		return (S_WORD)(PowerData.m_PowerSupplyM - PowerData.m_LastMonPowerSupplyM);
	S_DWORD MaxDword = 0xFFFFFFFFUL;//�����
	return (S_WORD)(MaxDword - PowerData.m_LastMonPowerSupplyM + PowerData.m_PowerSupplyM);
}
S_WORD C_STATUS::GetFreezeDayPowerSupplyM(void)
{
	S_PowerSupplyData &PowerData = m_pMyStatus->m_TermData.m_PowerSupplyData;
	S_TIME NowDay = C_TIME::DateTrim(TIME_UNIT_DAY, C_TIME::TimeNow());
	if (NowDay == PowerData.m_timeFreezeDay)
		return PowerData.m_FreezeDayPowerSupplyM;

	PowerData.m_timeFreezeDay = NowDay;
	PowerData.m_FreezeDayPowerSupplyM = GetDayPowerSupplyM();
	PowerData.m_LastDayPowerSupplyM = PowerData.m_PowerSupplyM;
	return PowerData.m_FreezeDayPowerSupplyM;
}
S_WORD C_STATUS::GetFreezeMonPowerSupplyM(void)
{
	S_PowerSupplyData &PowerData = m_pMyStatus->m_TermData.m_PowerSupplyData;
	S_TIME NowMon = C_TIME::DateTrim(TIME_UNIT_MONTH, C_TIME::TimeNow());
	if (NowMon == PowerData.m_timeFreezeMon)
		return PowerData.m_FreezeMonPowerSupplyM;

	PowerData.m_timeFreezeMon = NowMon;
	PowerData.m_FreezeMonPowerSupplyM = GetMonPowerSupplyM();
	PowerData.m_LastMonPowerSupplyM = PowerData.m_PowerSupplyM;
	return PowerData.m_FreezeMonPowerSupplyM;
}

/** @brief �ն���������*/
S_TIME C_STATUS::GetCommunicationTime(void)
{
	return m_pMyStatus->m_TermData.m_CommunData.m_timeCommunication;
}
void C_STATUS::AddCommFlux(S_DWORD flux)
{
	m_pMyStatus->m_TermData.m_CommunData.m_CommunicationFlux += flux;
	m_pMyStatus->m_TermData.m_CommunData.m_timeCommunication = C_TIME::TimeNow();
}
S_DWORD C_STATUS::GetDayCommFlux(void)
{
	S_TermData *pTermData = &m_pMyStatus->m_TermData;
	if (pTermData->m_CommunData.m_CommunicationFlux >= pTermData->m_CommunData.m_LastDayCommunicationFlux)
		return pTermData->m_CommunData.m_CommunicationFlux - pTermData->m_CommunData.m_LastDayCommunicationFlux;
	S_DWORD MaxDword = 0xFFFFFFFFUL;//�����
	return MaxDword - pTermData->m_CommunData.m_LastDayCommunicationFlux + pTermData->m_CommunData.m_CommunicationFlux;
}
S_DWORD C_STATUS::GetMonCommFlux(void)
{
	S_TermData *pTermData = &m_pMyStatus->m_TermData;
	if (pTermData->m_CommunData.m_CommunicationFlux >= pTermData->m_CommunData.m_LastMonCommunicationFlux)
		return pTermData->m_CommunData.m_CommunicationFlux - pTermData->m_CommunData.m_LastMonCommunicationFlux;
	S_DWORD MaxDword = 0xFFFFFFFFUL;//�����
	return MaxDword - pTermData->m_CommunData.m_LastMonCommunicationFlux + pTermData->m_CommunData.m_CommunicationFlux;
}
S_DWORD C_STATUS::GetDayCommFreeze(void)
{
	S_CommunData &CommData = m_pMyStatus->m_TermData.m_CommunData;
	S_TIME NowDay = C_TIME::DateTrim(TIME_UNIT_DAY, C_TIME::TimeNow());
	if (NowDay == CommData.m_timeFreezeDay)
		return CommData.m_FreezeDayCummFlux;

	CommData.m_timeFreezeDay = NowDay;
	CommData.m_FreezeDayCummFlux = GetDayCommFlux();
	CommData.m_LastDayCommunicationFlux = CommData.m_CommunicationFlux;
	return CommData.m_FreezeDayCummFlux;
}
S_DWORD C_STATUS::GetMonCommFreeze(void)
{
	S_CommunData &CommData = m_pMyStatus->m_TermData.m_CommunData;
	S_TIME NowMon = C_TIME::DateTrim(TIME_UNIT_MONTH, C_TIME::TimeNow());
	if (NowMon == CommData.m_timeFreezeMon)
		return CommData.m_FreezeMonCummFlux;

	CommData.m_timeFreezeMon = NowMon;
	CommData.m_FreezeMonCummFlux = GetMonCommFlux();
	CommData.m_LastMonCommunicationFlux = CommData.m_CommunicationFlux;
	return CommData.m_FreezeMonCummFlux;
}


