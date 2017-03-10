/** @file
 *  @brief ��̨ͨѶ�������ݶ�д������
 *  @author 
 *  @date 2011/12/22
 *  @version 0.1
 */
#include "cmddata.h"
#include "sysipc.h"
#include "cfg_proc.h"
#include "datafile.h"
#include "timeop.h"
#include "sysbase.h"
#include "context.h"
#include "nwbtype.h"
#include "status.h"
#include "sysctrl.h"
#include "../../share/ipcSharemem.h"


//
//����ǰ����
//
 //���ɼ���
bool C_CmdRealtime::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
//	string strIpcData;
	strIpcData.clear();
	const char *pPrecess = CONTEXT.GetDbcjPrecessName_MP(C_DAPN::DA2PN(CmdObj.m_DA));

	AppendDADT(strIpcData, CmdObj);
	logvt<<"pPrecess="<<hex<<pPrecess<<endl;
	if (C_SysMsg::Send2Server(pPrecess, MSAFN_READ_REALTIME, strIpcData) != 0)
	{
		logwt<<"����"<<pPrecess<<"ʵ������ʧ��!!!"<<endl;
		return false;
	}
	strIpcData.clear();
	if (C_SysMsg::Wait4Server(MSAFN_READ_REALTIME, strIpcData) != 0)
	{
		logwt<<"����"<<pPrecess<<"ʵ������ʧ��!!!"<<endl;
		return false;
	}
	logvt<<"strIpcData="<<hex<<strIpcData<<endl;
	if (strIpcData.size() <= 6)
	{
		logwt<<"����"<<pPrecess<<"ʵ������С�����ֽ�!!!"<<endl;
		strIpcData.clear();
		if (FillData(CmdObj, strIpcData) == false)
			return false;
	}
	strResponse.push_back(strIpcData);
	return true;
}

 //���ɼ���
bool C_CmdRealtime::GetCurZXYG()
{
	const char *pPrecess = "dbcj1";
	strIpcData.clear();
	strIpcData.push_back(0x01);
	strIpcData.push_back(0x01);
	strIpcData.push_back(0x00);
	strIpcData.push_back(0x00);
	strIpcData.push_back(0x01);
	strIpcData.push_back(0x00);	
	if (C_SysMsg::Send2Server(pPrecess, MSAFN_READ_REALTIME, strIpcData) != 0)
	{
		logwt<<"����"<<pPrecess<<"ʵ������ʧ��!!!"<<endl;
		return false;
	}
	strIpcData.clear();
	if (C_SysMsg::Wait4Server(MSAFN_READ_REALTIME, strIpcData) != 0)
	{
		logwt<<"����"<<pPrecess<<"ʵ������ʧ��!!!"<<endl;
		memset(IPCSHAREMEM.m_IpcDevicesRegData[1].m_Reg00010000,0xff,4);
		return false;
	}
	logvt<<"strIpcData="<<hex<<strIpcData<<endl;
	if (strIpcData.size() <= 6)
	{
		logwt<<"����"<<pPrecess<<"ʵ������С�����ֽ�!!!"<<endl;
		strIpcData.clear();
		memset(IPCSHAREMEM.m_IpcDevicesRegData[1].m_Reg00010000,0xff,4);
	}
	IPCSHAREMEM.m_IpcDevicesRegData[1].m_Reg00010000[0] = strIpcData[6];
	IPCSHAREMEM.m_IpcDevicesRegData[1].m_Reg00010000[1] = strIpcData[7];
	IPCSHAREMEM.m_IpcDevicesRegData[1].m_Reg00010000[2] = strIpcData[8];
	IPCSHAREMEM.m_IpcDevicesRegData[1].m_Reg00010000[3] = strIpcData[9];		
	

	return true;
}


bool C_CmdRealtime::FillData(const S_CmdObj &CmdObj, string &strData)
{
	C_DA da(CmdObj.m_DA);
	S_MSDI msdi = STATUS.FindMasterStationDataID(CLASSDATA_2, CmdObj.m_DI, 0x00);

	if (msdi.m_Enable == false)
		return false;
	if (da.GetMpCount() == 0xFF)//���в����㣬��֧��
		return false;

	strData.append((char *)&CmdObj.m_DA, 2);
	strData.append((char *)&CmdObj.m_DI, 4);
	if (da.GetMpCount() == 0)//�ն˲�����
	{
		msdi.FillData(strData);
	}
	else
	{
		for(S_BYTE DA1=0; DA1<8; DA1++)
		{
			if (da.TestMP(DA1) == true)
				msdi.FillData(strData);
		}
	}
	return true;
}
//�ն�״̬�֣��μ�C.5.1
bool C_CmdRealE1800001::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strData;
	S_WORD status = 0x00;//TODO:

	AppendDADT(strData, CmdObj);
	strData.append((char *)&status, 2);

	strResponse.push_back(strData);
	return true;
}
//���������·��
bool C_CmdRealE1800002::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strData;
	S_BYTE PulseMax = CONS_PULSE_COUNT_MAX;

	AppendDADT(strData, CmdObj);
	strData.push_back(kge_cbcd(PulseMax));

	strResponse.push_back(strData);
	return true;
}
//����ͨ�Ŷ˿�����
bool C_CmdRealE1800003::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strData;
	S_BYTE SerialMax = CONS_SERIAL_COUNT_MAX;

	AppendDADT(strData, CmdObj);
	strData.push_back(kge_cbcd(SerialMax));

	strResponse.push_back(strData);
	return true;
}
//֧������������
bool C_CmdRealE1800004::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strData;
	S_WORD MpMax = CONS_METER_COUNT_MAX - 1;
	MpMax = KGE_COMBWORD(kge_cbcd(MpMax/100), kge_cbcd(MpMax%100));

	AppendDADT(strData, CmdObj);
	strData.append((char *)&MpMax, 2);

	strResponse.push_back(strData);
	return true;
}
//����������,������Ч�Ĳ���������
bool C_CmdRealE1800005::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strData;
	S_ALLMP *pAllMP = STATUS.GetALLMP();
	S_WORD ValidMp = (S_WORD)pAllMP->m_MTBS.m_AllMPBS.count();
	ValidMp = KGE_COMBWORD(kge_cbcd(ValidMp/100), kge_cbcd(ValidMp%100));

	AppendDADT(strData, CmdObj);
	strData.append((char *)&ValidMp, 2);

	strResponse.push_back(strData);
	return true;
}
//�ն���ͨ�û������� 
bool C_CmdRealE1800006::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strData;
	S_ALLMP * allmp=STATUS.GetALLMP();
	S_WORD NormalUserCnt = (S_WORD)(allmp->m_MTBS.m_AllMPBS.count()-allmp->m_VipMPBS.count());
	NormalUserCnt = KGE_COMBWORD(kge_cbcd(NormalUserCnt/100), kge_cbcd(NormalUserCnt%100));

	AppendDADT(strData, CmdObj);
	strData.append((char *)&NormalUserCnt, 2);

	strResponse.push_back(strData);
	return true;
}
//�ն˶๦�ܱ�����
bool C_CmdRealE1800007::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strData;
	S_ALLMP * allmp=STATUS.GetALLMP();
	S_WORD MultiFunCnt = (S_WORD)allmp->m_MultiMPBS.count();
	MultiFunCnt = KGE_COMBWORD(kge_cbcd(MultiFunCnt/100), kge_cbcd(MultiFunCnt%100));

	AppendDADT(strData, CmdObj);
	strData.append((char *)&MultiFunCnt, 2);

	strResponse.push_back(strData);
	return true;
}
//�ն��ص㻧���ܱ�����
bool C_CmdRealE1800008::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strData;
	S_ALLMP * allmp=STATUS.GetALLMP();
	S_WORD VipUserCnt = (S_WORD)allmp->m_VipMPBS.count();
	VipUserCnt = KGE_COMBWORD(kge_cbcd(VipUserCnt/100), kge_cbcd(VipUserCnt%100));

	AppendDADT(strData, CmdObj);
	strData.append((char *)&VipUserCnt, 2);

	strResponse.push_back(strData);
	return true;
}
//�ն˳���״̬��Ϣ
bool C_CmdRealE1800009::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strData;
	S_WORD MeterTotal, MeterSuccess;
	S_TIME timeGatherBegin, timeGatherEnd;
	char bcdTime7B[8];
	if (GetTermGatherStatus(MeterTotal, MeterSuccess, timeGatherBegin, timeGatherEnd) < 0)
		return false;

	MeterTotal = KGE_COMBWORD(kge_cbcd(MeterTotal/100), kge_cbcd(MeterTotal%100));
	MeterSuccess = KGE_COMBWORD(kge_cbcd(MeterSuccess/100), kge_cbcd(MeterSuccess%100));

	AppendDADT(strData, CmdObj);
	strData.append((char *)&MeterTotal, 2);
	strData.append((char *)&MeterSuccess, 2);
	if (timeGatherBegin != 0x00)
		strData.append(C_TIME::Time2BCDStr(timeGatherBegin, bcdTime7B) , 6);
	else
		strData.append((size_t)6, (char)0xFF);
	if (timeGatherEnd != 0x00)
		strData.append(C_TIME::Time2BCDStr(timeGatherEnd, bcdTime7B) , 6);
	else
		strData.append((size_t)6, (char)0xFF);

	strResponse.push_back(strData);
	return true;
}
int C_CmdRealE1800009::GetTermGatherStatus(S_WORD &MeterTotal, S_WORD &MeterSuccess, S_TIME &timeGatherBegin, S_TIME &timeGatherEnd)
{//TODO:
	S_TermData *TD=STATUS.GetTermData();
	S_ALLMP *pAllMP = STATUS.GetALLMP();
	MeterTotal = (S_WORD)pAllMP->m_MTBS.m_AllMPBS.count();
	MeterSuccess = (S_WORD)TD->m_GatherStatInfos.m_DayInfo.m_mpbsSuccess.count();
	timeGatherBegin = TD->m_GatherStatInfos.m_DayInfo.m_timeTaskStart;
	timeGatherEnd = TD->m_GatherStatInfos.m_DayInfo.m_timeTaskFinish;
	return 0;
}

//����������
bool C_CmdRealE1800010::Action(const S_CmdObj &CmdObj, strings &strResponse)
{//TODO:
	string strData;
	S_DWORD LineLossRate = 0x00;// BCD

	AppendDADT(strData, CmdObj);
	strData.append((char *)&LineLossRate, 3);

	strResponse.push_back(strData);
	return true;
}
//(��ǰ)��ͨ������
bool C_CmdRealE1800011::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strData;
	S_WORD DayComm = (S_WORD)((STATUS.GetDayCommFlux() + 1023) / 1024); // avoid assign 0
	DayComm = KGE_COMBWORD(kge_cbcd(DayComm/100), kge_cbcd(DayComm%100));

	AppendDADT(strData, CmdObj);
	strData.append((char *)&DayComm, 2);

	strResponse.push_back(strData);
	return true;
}
//(��ǰ)��ͨ������
bool C_CmdRealE1800013::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strData;
	S_DWORD MonComm = (STATUS.GetMonCommFlux() + 1023) / 1024; // avoid assign 0

	AppendDADT(strData, CmdObj);
	strData.push_back(kge_cbcd((S_BYTE)(MonComm%100)));
	strData.push_back(kge_cbcd((S_BYTE)(MonComm/100%100)));
	strData.push_back(kge_cbcd((S_BYTE)(MonComm/10000%100)));

	strResponse.push_back(strData);
	return true;
}
//�ź�ǿ��(0~31)
bool C_CmdRealE1800015::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strData;
	S_PppStatus *pPppStatus = CONTEXT.GetPppStatus();

	AppendDADT(strData, CmdObj);
	strData.push_back(kge_cbcd(pPppStatus->m_Signal));

	strResponse.push_back(strData);
	return true;
}
//(��ǰ)���ն˸�λ����
bool C_CmdRealE1800016::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strData;
	S_ResetData &ResetData = STATUS.GetTermData()->m_ResetData;
	S_WORD MonResetTimes = ResetData.m_MonResetTimes;
	MonResetTimes = KGE_COMBWORD(kge_cbcd(MonResetTimes/100), kge_cbcd(MonResetTimes%100));
	
	AppendDADT(strData, CmdObj);
	strData.append((char *)&MonResetTimes, 2);

	strResponse.push_back(strData);
	return true;
}
//(����)���ն˸�λ����
bool C_CmdRealE1800017::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strData;
	S_ResetData &ResetData = STATUS.GetTermData()->m_ResetData;
	S_WORD FreezeMonResetTimes = ResetData.m_FreezeMonResetTimes;
	FreezeMonResetTimes = KGE_COMBWORD(kge_cbcd(FreezeMonResetTimes/100), kge_cbcd(FreezeMonResetTimes%100));

	AppendDADT(strData, CmdObj);
	strData.append((char *)&FreezeMonResetTimes, 2);

	strResponse.push_back(strData);
	return true;

}
//�ز�ģ�����汾��
bool C_CmdRealE1800018::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strData;
	S_SYSRUN * sr=STATUS.GetSysRunInfo();

	AppendDADT(strData, CmdObj);
	strData.append(sr->m_RouterType, 2);
	strData.append(sr->m_ChipCode, 2);

	int year = 0, mon = 0, day = 0;
	int version = atoi(sr->m_ChipVersion);
	sscanf(sr->m_ChipVerDate, "%d-%d-%d", &year, &mon, &day);

	strData.push_back(kge_cbcd(day));
	strData.push_back(kge_cbcd(mon));
	strData.push_back(kge_cbcd(year%100));
	strData.push_back(kge_cbcd(version/100));
	strData.push_back(kge_cbcd(version%100));

	strResponse.push_back(strData);
	return true;
}

//��������������
bool C_CmdRealE1800019::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strData;
	S_WORD DayComm = 0;
	DayComm = KGE_COMBWORD(kge_cbcd(DayComm/100), kge_cbcd(DayComm%100));

	AppendDADT(strData, CmdObj);
	strData.append((char *)&DayComm, 2);

	strResponse.push_back(strData);
	return true;
}

//�ص㻧���ܱ�������
bool C_CmdRealE1800020::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strData;
	S_WORD count=0;
	S_ALLMP * allmp=STATUS.GetALLMP();

	AppendDADT(strData, CmdObj);
	for (size_t st=allmp->m_VipMPBS.begin(); st<allmp->m_VipMPBS.end(); st++)
	{
		if(allmp->m_VipMPBS.test(st))
		{
			count+=1;
			strData.append((char*)&st,2);
			if(count>=20)
				break;
		}
	}

	if(strData.size()<(40+6))
		strData.append((size_t)(46-strData.size()),(char)0xFF);

	strResponse.push_back(strData);
	return true;

}


C_CmdReceiver *C_AFNCMD_REALTIME::GetCmdReceiver(INT32U DI)
{
	static C_CmdRealE1800001 CmdRealE1800001; //�ն�״̬�֣��μ�C.5.1
	static C_CmdRealE1800002 CmdRealE1800002; //���������·��
	static C_CmdRealE1800003 CmdRealE1800003; //����ͨ�Ŷ˿�����
	static C_CmdRealE1800004 CmdRealE1800004; //֧������������
	static C_CmdRealE1800005 CmdRealE1800005; //����������,������Ч�Ĳ���������
	static C_CmdRealE1800006 CmdRealE1800006; //�ն���ͨ�û������� 
	static C_CmdRealE1800007 CmdRealE1800007; //�ն˶๦�ܱ�����
	static C_CmdRealE1800008 CmdRealE1800008; //�ն��ص㻧���ܱ�����
	static C_CmdRealE1800009 CmdRealE1800009; //�ն˳���״̬��Ϣ
	static C_CmdRealE1800010 CmdRealE1800010; //����������
	static C_CmdRealE1800011 CmdRealE1800011; //(��ǰ)��ͨ������
	static C_CmdRealE1800013 CmdRealE1800013; //(��ǰ)��ͨ������
	static C_CmdRealE1800015 CmdRealE1800015; //�ź�ǿ��(0~31)
	static C_CmdRealE1800016 CmdRealE1800016; //(��ǰ)���ն˸�λ����
	static C_CmdRealE1800017 CmdRealE1800017; //(����)���ն˸�λ����
	static C_CmdRealE1800018 CmdRealE1800018; //�ز�ģ�����汾��
	static C_CmdRealE1800019 CmdRealE1800019; //��������������
	static C_CmdRealE1800020 CmdRealE1800020; //���ص㻧���ܱ�������
	static C_CmdRealtime CmdRealtimeDefault;
	switch (DI)
	{
	case 0xE1800001: return &CmdRealE1800001; //�ն�״̬�֣��μ�C.5.1
	case 0xE1800002: return &CmdRealE1800002; //���������·��
	case 0xE1800003: return &CmdRealE1800003; //����ͨ�Ŷ˿�����
	case 0xE1800004: return &CmdRealE1800004; //֧������������
	case 0xE1800005: return &CmdRealE1800005; //����������,������Ч�Ĳ���������
	case 0xE1800006: return &CmdRealE1800006; //�ն���ͨ�û������� 
	case 0xE1800007: return &CmdRealE1800007; //�ն˶๦�ܱ�����
	case 0xE1800008: return &CmdRealE1800008; //�ն��ص㻧���ܱ�����
	case 0xE1800009: return &CmdRealE1800009; //�ն˳���״̬��Ϣ
	case 0xE1800010: return &CmdRealE1800010; //����������
	case 0xE1800011: return &CmdRealE1800011; //(��ǰ)��ͨ������
	case 0xE1800013: return &CmdRealE1800013; //(��ǰ)��ͨ������
	case 0xE1800015: return &CmdRealE1800015; //�ź�ǿ��(0~31)
	case 0xE1800016: return &CmdRealE1800016; //(��ǰ)���ն˸�λ����
	case 0xE1800017: return &CmdRealE1800017; //�ź�ǿ��(0~31)
	case 0xE1800018: return &CmdRealE1800018; //(��ǰ)���ն˸�λ����
	case 0xE1800019: return &CmdRealE1800019; //��������������
	case 0xE1800020: return &CmdRealE1800020; //���ص㻧���ܱ�������
	default: 	return &CmdRealtimeDefault;
	}
	return NULL;
}


//
//����ʷ����
//
bool C_CmdHistory::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	if (distance(iterBegin, iterEnd) < 13)
		return false;

	CmdObj.m_TimeInfo.append(iterBegin, iterBegin+12);
	CmdObj.m_DiInfo.append(iterBegin+12, iterBegin+13);
	advance(iterBegin, 13);
	return true;
}

bool C_CmdHistory::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 1 || CmdObj.m_TimeInfo.size() != 12)
		return false;
	S_TIME timeBegin = C_TIME::BcdStr2Time6B_Big((char *)&CmdObj.m_TimeInfo[0]);
	S_TIME timeEnd = C_TIME::BcdStr2Time6B_Big((char *)&CmdObj.m_TimeInfo[6]);
	S_CHAR Density = CmdObj.m_DiInfo[0];
	if (Density <= 0)
		Density = (S_CHAR)C_Class2Data::GetDensity(CmdObj.m_DI);
	if (Density <= 0)
	{
		logwt<<"������("<<HEX<<CmdObj.m_DI<<")�Ĵ洢�ܶ�("<<Density<<")�Ƿ�!!!"<<endl;
		return false;
	}
	S_MSDI msdi = STATUS.FindMasterStationDataID(CLASSDATA_2, CmdObj.m_DI, 0x00);
//	if (msdi.m_Freeze == MSDI_FREEZE_DAY) // ���Ժ��վ���ɼ�ʱ��ȡ��
//	{
//		if (C_TIME::DateTrim(TIME_UNIT_DAY, C_TIME::TimeNow()) == C_TIME::DateTrim(TIME_UNIT_DAY, timeBegin))
//		{// �õ�ǰʱ���·�ȡ��
//			timeBegin = C_TIME::DateAdd(TIME_UNIT_DAY, -1, timeBegin);
//			timeEnd = C_TIME::DateAdd(TIME_UNIT_DAY, -1, timeEnd);
//		}
//	}
//	else if (msdi.m_Freeze == MSDI_FREEZE_MONTH)
//	{
//		if (C_TIME::DateTrim(TIME_UNIT_MONTH, C_TIME::TimeNow()) == C_TIME::DateTrim(TIME_UNIT_MONTH, timeBegin))
//		{// �õ�ǰʱ���·�ȡ��
//			timeBegin = C_TIME::DateAdd(TIME_UNIT_MONTH, -1, timeBegin);
//			timeEnd = C_TIME::DateAdd(TIME_UNIT_MONTH, -1, timeEnd);
//		}
//	}

	S_MPBS MPBS;
	if (MakeReadCmdObj(CmdObj.m_DA, MPBS) <= 0)
	{
		logwt<<"���ݵ�("<<HEX<<CmdObj.m_DA<<")�Ƿ�!!!"<<endl;
		return false;
	}

	for (S_WORD mp = (S_WORD)MPBS.begin(); mp < (S_WORD)MPBS.end(); mp++)
	{
		C_SYSCTRL::ProcessHeartBeat();
		if (MPBS.test(mp) == false)
			continue;

		string strData;
		for (S_TIME timeMyBegin = DateTrim(timeBegin, Density); timeMyBegin < timeEnd; timeMyBegin = GetNextTime(timeMyBegin, Density))
		{
			char bcdStr6B[6];
			string strMyData;

			S_TIME timeData = timeMyBegin;
			C_Class2Data::Read(CmdObj.m_DI, mp, strMyData, timeData);
			if (strMyData.empty() == true)
				msdi.FillData(strMyData);
			strData.append(strMyData).append(C_TIME::Time2BcdStr6B_Big(timeData, bcdStr6B), 6);
		}
		if (strData.empty() == false)
		{
			string strAppData;
			S_WORD DA = C_DAPN::PN2DA(mp);
			strAppData.append((char *)&DA, 2);
			strAppData.append((char *)&CmdObj.m_DI, 4);
			strAppData.append(strData);
			strResponse.push_back(strAppData);
		}
	}
	return strResponse.empty()?false:true;
}

S_TIME C_CmdHistory::GetNextTime(S_TIME timeData, S_CHAR Density)
{
	if (Density == 7)
	{// 1��
		return C_TIME::DateAdd(TIME_UNIT_MONTH, 1, timeData);
	}
	else if (Density > 0 && Density < 6)
	{// 1-60����
		return timeData + C_DBRWFACT::GetVipFreezeIntervalM((S_DataDensity)Density) * 60;
	}
//	if (Density == 6) 1��
	return timeData + 60*60*24;
}

S_TIME C_CmdHistory::DateTrim(S_TIME timeData, S_CHAR Density)
{
	if (Density > 0 && Density < 6)
	{// 1-60����
		timeData += C_DBRWFACT::GetVipFreezeIntervalM((S_DataDensity)Density) * 60 - 1; 
		if (Density == 5) // 60��
			timeData = C_TIME::DateTrim(TIME_UNIT_HOUR, timeData);
		else
			timeData = C_TIME::DateTrim(TIME_UNIT_MINUTE, (S_WORD)C_DBRWFACT::GetVipFreezeIntervalM((S_DataDensity)Density), timeData);
	}
	else if (Density == 6)
	{// ��
		S_DATE date;
		C_TIME::LocalTime(timeData, date);
		if (date.m_Hour > 0 || date.m_Min > 0 || date.m_Sec > 0)
			timeData = C_TIME::DateAdd(TIME_UNIT_DAY, 1, timeData);
		timeData = C_TIME::DateTrim(TIME_UNIT_DAY, timeData);
	}
	else if (Density == 7)
	{// ��
		S_DATE date;
		C_TIME::LocalTime(timeData, date);
		if (date.m_Day > 1 || date.m_Hour > 0 || date.m_Min > 0 || date.m_Sec > 0)
			timeData = C_TIME::DateAdd(TIME_UNIT_MONTH, 1, timeData);
		timeData = C_TIME::DateTrim(TIME_UNIT_MONTH, timeData);
	}
	return timeData;
}

int C_CmdHistory::MakeReadCmdObj(S_WORD DA, S_MPBS &MPBS)
{
	C_DA da(DA);
	if (da.GetMpCount() == 0xFF) // ���в�����
	{
		S_ALLMP *pAllMP = STATUS.GetALLMP();
		MPBS = pAllMP->m_MTBS.m_AllMPBS;
		MPBS.reset(0);
		MPBS.Stat();
	}
	else
	{
		da.PrintMPBS(MPBS);
	}

	return (int)MPBS.end();	
}

C_CmdReceiver *C_AFNCMD_HISTORY::GetCmdReceiver(INT32U DI)
{
	static C_CmdHistory CmdHistory;
	return &CmdHistory;
}


//
//����������
//
bool C_CmdTask::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	if (distance(iterBegin, iterEnd) < 13)
		return false;

	CmdObj.m_TimeInfo.append(iterBegin, iterBegin+12);
	CmdObj.m_DiInfo.append(iterBegin+12, iterBegin+13);
	advance(iterBegin, 13);
	return true;
}
bool C_CmdTask::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	const char *pBcdTime = CmdObj.m_TimeInfo.data();
	S_TIME timeBegin = C_TIME::BcdStr2Time6B_Big(pBcdTime);
	S_TIME timeEnd = C_TIME::BcdStr2Time6B_Big(pBcdTime+6);
	if (timeBegin >= timeEnd)
		return false;

	S_NormalTaskInfos *pTaskInfos = STATUS.GetNormalTaskInfos();
	S_NormalTaskInfo *pInfo = &pTaskInfos->m_ItemInfo[CmdObj.m_DI - NORMAL_TASK_ID_MIN];
	if (pInfo->m_SamplePeriodUnit > 3 || pInfo->m_SamplePeriodVal == 0)
		return false;

	S_ALLMP *pAllMP = STATUS.GetALLMP();
	S_MPBS mpbsFailed(pAllMP->m_MTBS.m_AllMPBS);

	strings strMyAppDatas;
	S_DataDensity Density = (S_DataDensity)CmdObj.m_DiInfo[0];
	for (; timeBegin < timeEnd; timeBegin = GetNextSampleTime(pInfo, timeBegin, Density))
	{
		strings strMyDatas;
		S_TIME timeTrim = TrimSampleTime(Density, timeBegin);
		if (GetNormalTaskData(pInfo, timeTrim, strMyDatas, mpbsFailed, Density) > 0)
		{
			char bcdStr6B[6];
			C_TIME::Time2BcdStr6B_Big(timeTrim, bcdStr6B);
			strMyDatas.AppendBack(string(bcdStr6B, 6));
			strMyAppDatas.append(strMyDatas);
		}
	}
	if (strMyAppDatas.empty() == true)
		return false;

	string strDadt;
	strDadt.append((char *)&CmdObj.m_DA, 2);
	strDadt.append((char *)&CmdObj.m_DI, 4);
	//��������֯����
	strings strAppDatas;
	TrimCmdInfo(strMyAppDatas, strAppDatas);
	strAppDatas.InsertFront(strDadt);
	strResponse.append(strAppDatas);
	return true;
}

const S_CHAR g_TimeUnit2Density[] = {4, 5, 6, 7};
int C_CmdTask::GetReportData(int TaskID, const S_NormalTaskInfo *pInfo, S_HistoryTask *pHistoryTask, strings &strCmdInfos)
{
	strings strMyAppDatas;
	for (S_TIME timeReportBegin = pHistoryTask->m_timeReportBegin; timeReportBegin < pHistoryTask->m_timeReportEnd; timeReportBegin = GetNextSampleTime(pInfo, timeReportBegin))
	{
		strings strMyDatas;
		S_TIME timeReport = TrimSampleTime(pInfo, timeReportBegin);

		if (GetNormalTaskData(pInfo, timeReport, strMyDatas, pHistoryTask->m_MPBS, DATA_DENSITY_MAX) > 0)
		{
			char bcdStr6B[6];
			C_TIME::Time2BcdStr6B_Big(timeReport, bcdStr6B);
			strMyDatas.AppendBack(string(bcdStr6B, 6));
			strMyAppDatas.append(strMyDatas);
		}
	}

	if (strMyAppDatas.empty() == true)
		return 0;

	string strDadt;
	S_WORD DA = 0;
	S_DWORD DI = NORMAL_TASK_ID_MIN + TaskID - 1;
	strDadt.append((char *)&DA, 2);
	strDadt.append((char *)&DI, 4);
	//��������֯����
	strings strAppDatas;
	TrimCmdInfo(strMyAppDatas, strAppDatas);
	strAppDatas.InsertFront(strDadt);
	strCmdInfos.append(strAppDatas);
	return (int)strCmdInfos.size();
}

S_TIME_UNIT g_TimeUnit[] = {TIME_UNIT_MINUTE,TIME_UNIT_HOUR,TIME_UNIT_DAY,TIME_UNIT_MONTH};
S_TIME C_CmdTask::GetNextSampleTime(const S_NormalTaskInfo *pInfo, S_TIME timeData, S_DataDensity Density)
{
	if (Density == DATA_DENSITY_TERM)
	{//���ն�ʵ�ʴ洢���ݵ�ʱ����
		S_TIME_UNIT timeUnit = g_TimeUnit[pInfo->m_SamplePeriodUnit];
		return C_TIME::DateAdd(timeUnit, pInfo->m_SamplePeriodVal, timeData);
	}
	else if (Density == DATA_DENSITY_MON01)
	{// 1��
		return C_TIME::DateAdd(TIME_UNIT_MONTH, 1, timeData);
	}
	else if (Density > DATA_DENSITY_TERM && Density < DATA_DENSITY_DAY01)
	{// 1-60����
		return timeData + C_DBRWFACT::GetVipFreezeIntervalM((S_DataDensity)Density) * 60;
	}
//	if (Density == 6) 1��
	return timeData + 60*60*24;
}

extern const long g_Seconds4TimeUnit[];
S_TIME C_CmdTask::GetNextSampleTime(const S_NormalTaskInfo *pInfo, S_TIME timeData)
{
	if (pInfo->m_SamplePeriodUnit == 3) //month
		return C_TIME::DateAdd(TIME_UNIT_MONTH, pInfo->m_SamplePeriodVal*pInfo->m_ExtractRate, timeData);
	return timeData + g_Seconds4TimeUnit[pInfo->m_SamplePeriodUnit] * (pInfo->m_SamplePeriodVal*pInfo->m_ExtractRate);
}
S_TIME C_CmdTask::TrimSampleTime(const S_NormalTaskInfo *pInfo, S_TIME timeData)
{
	switch (pInfo->m_SamplePeriodUnit)
	{
	case 0: // minute
		return C_TIME::DateTrim(TIME_UNIT_MINUTE, pInfo->m_SamplePeriodVal, timeData);
	case 1: // hour
		return C_TIME::DateTrim(TIME_UNIT_HOUR, timeData);
	case 2: // day
		return C_TIME::DateTrim(TIME_UNIT_DAY, timeData);
	case 3: // month
		return C_TIME::DateTrim(TIME_UNIT_MONTH, timeData);
	default:
		return timeData;
	}
	return timeData;
}
S_TIME C_CmdTask::TrimSampleTime(S_DataDensity Density, S_TIME timeData)
{
	switch (Density)
	{
	case DATA_DENSITY_MIN01: // 1 ����
		break;
	case DATA_DENSITY_MIN05: // 5 ����
		return C_TIME::DateTrim(TIME_UNIT_MINUTE, 5, timeData);
	case DATA_DENSITY_MIN15: // 15 ����
		return C_TIME::DateTrim(TIME_UNIT_MINUTE, 15, timeData);
	case DATA_DENSITY_MIN30: // 30 ����
		return C_TIME::DateTrim(TIME_UNIT_MINUTE, 30, timeData);
	case DATA_DENSITY_MIN60: // 60 ����
		return C_TIME::DateTrim(TIME_UNIT_HOUR, timeData);
	case DATA_DENSITY_DAY01: // 1��
		return C_TIME::DateTrim(TIME_UNIT_DAY, timeData);
	case DATA_DENSITY_MON01: // 1��
		return C_TIME::DateTrim(TIME_UNIT_MONTH, timeData);
	default:
		return timeData;
	}
	return timeData;
}
S_TIME C_CmdTask::GetDataSampleTime(const S_NormalTaskInfo *pInfo, S_TIME timeData, S_DataDensity Density, S_DWORD DI)
{
	if (Density >= DATA_DENSITY_MAX)
		return GetDataSampleTime(pInfo, timeData, DI);

	if (Density == DATA_DENSITY_TERM)
	{//���ն�ʵ�ʴ洢���ݵ�ʱ����
		S_TIME_UNIT timeUnit = g_TimeUnit[pInfo->m_SamplePeriodUnit];
		if (timeUnit == TIME_UNIT_DAY)
			Density = DATA_DENSITY_DAY01;
		else if (timeUnit == TIME_UNIT_MONTH)
			Density = DATA_DENSITY_MON01;
		else if (timeUnit == TIME_UNIT_HOUR)
			Density = DATA_DENSITY_MIN60;
		else
		{
			switch (pInfo->m_SamplePeriodVal)
			{
			case 1:Density = DATA_DENSITY_MIN01;break;// 1 ����
			case 5:Density = DATA_DENSITY_MIN05;break; // 5 ����
			case 15:Density = DATA_DENSITY_MIN15;break; // 15 ����
			case 30:Density = DATA_DENSITY_MIN30;break; // 30 ����
			case 60:Density = DATA_DENSITY_MIN60;break; // 60 ����
			default:Density = DATA_DENSITY_MIN60;break;
			}
		}
	}
	// ����������ݰ�����ȡ������ȡ���ʱ��������
	S_MSDI msdi = STATUS.FindMasterStationDataID(CLASSDATA_2, DI, 0x00);
	if (Density == DATA_DENSITY_DAY01)
	{
		if (msdi.m_Enable == true && msdi.m_Freeze == MSDI_FREEZE_HOUR)
			timeData += 60*60*24; // ȡ���ʱ���ɵ���
	}
	else if (Density == DATA_DENSITY_MON01)
	{
		if (msdi.m_Enable == true && msdi.m_Freeze == MSDI_FREEZE_HOUR)
			timeData = C_TIME::DateAdd(TIME_UNIT_MONTH, 1, timeData);// ȡ���ʱ���ɵ���
	}
	return timeData;
}
S_TIME C_CmdTask::GetDataSampleTime(const S_NormalTaskInfo *pInfo, S_TIME timeData, S_DWORD DI)
{
	S_MSDI msdi = STATUS.FindMasterStationDataID(CLASSDATA_2, DI, 0x00);
	switch (pInfo->m_SamplePeriodUnit)
	{
	case 1: //hour
		if (pInfo->m_ExtractRate > 1)
			return C_TIME::DateAdd(TIME_UNIT_HOUR, pInfo->m_SamplePeriodVal*(pInfo->m_ExtractRate-1), timeData);
		return timeData;
	case 2: //day
		if (msdi.m_Enable == true && msdi.m_Freeze == MSDI_FREEZE_HOUR) // ��½��վҪ����ȡ��������ȡ���һ����
			return timeData + 60*60*24*pInfo->m_SamplePeriodVal;
		return timeData;
	case 3: //month
		if (msdi.m_Enable == true && msdi.m_Freeze == MSDI_FREEZE_HOUR) // ��½��վҪ����ȡ��������ȡ���һ����
			return C_TIME::DateAdd(TIME_UNIT_MONTH, pInfo->m_SamplePeriodVal, timeData);
		return timeData;
	default: // minute
		if (pInfo->m_ExtractRate > 1)
			return C_TIME::DateAdd(TIME_UNIT_MINUTE, pInfo->m_SamplePeriodVal*(pInfo->m_ExtractRate-1), timeData);
		return timeData;
	}
	return timeData;
}
int C_CmdTask::GetNormalTaskData(const S_NormalTaskInfo *pInfo, S_TIME timeData, strings &strDatas, S_MPBS &mpbsFailed, S_DataDensity Density)
{
	if (pInfo->m_DataFormat == 0)//��ʾ��������ʽ��֯����
		return GetNormalTaskData_UserDefine(pInfo, timeData, strDatas, mpbsFailed, Density);
	return GetNormalTaskData_TaskDefine(pInfo, timeData, strDatas, mpbsFailed, Density);//��ʾ������������ݸ�ʽ��֯����
}
int C_CmdTask::GetNormalTaskData_UserDefine(const S_NormalTaskInfo *pInfo, S_TIME timeData, strings &strDatas, S_MPBS &mpbsFailed, S_DataDensity Density)//��ʾ��������ʽ��֯����
{
	S_MPBS MPBS;
	strings strAppDatas;

	if (MakeMeasurePointBitset(pInfo, MPBS) == false)
		return 0;

	for (S_WORD MP = (S_WORD)MPBS.begin(); MP < MPBS.end(); MP++)
	{
		C_SYSCTRL::ProcessHeartBeat();
		if (MPBS.test(MP) == false)
			continue; // ���ڵı��
		if (mpbsFailed.test(MP) == false)
			continue; // ��Ҫ�ϱ��ı��

		string strData;
		bool fReadOk = true;
		S_WORD DA = C_DAPN::PN2DA(MP);
		for (S_WORD FnIdx = 0; FnIdx < pInfo->m_DtCnt; FnIdx++)
		{
			char bcdStr6B[6];
			string strMyData;

			S_DWORD FN = pInfo->m_DT[FnIdx];
			S_TIME timeMyData = GetDataSampleTime(pInfo, timeData, Density, FN);
			if (timeMyData >= C_TIME::TimeNow()) // ������ǰʱ��
				timeMyData = C_TIME::DateTrim(TIME_UNIT_HOUR, C_TIME::TimeNow());

			strData.append((char *)&DA, 2);
			strData.append((char *)&FN, 4);
			if (C_Class2Data::Read(FN, MP, strMyData, timeMyData) >= 0)
			{
				strData.append(strMyData);
			}
			else
			{
				string strFillData;
				S_MSDI msdi = STATUS.FindMasterStationDataID(CLASSDATA_2, FN, 0x00);
				if (msdi.FillData(strFillData) <= 0)
					loget<<"��������[Class=2,DI="<<HEX<<FN<<"]���ʧ��!!!"<<endl;;
				strData.append(strFillData);

				fReadOk = false;
			}
			C_TIME::Time2BcdStr6B_Big(timeMyData, bcdStr6B);
			strData.append(bcdStr6B+1, 5);
		}
		strAppDatas.push_back(strData);
		if (fReadOk == true)
			mpbsFailed.reset(MP);
 	}

	if (strAppDatas.empty() == true)
		return 0;

	TrimCmdInfo(0, (S_WORD)MPBS.count(), pInfo->m_DtCnt, strAppDatas, strDatas);//0����������ʽ

	return (int)strDatas.size();
}

int C_CmdTask::GetNormalTaskData_TaskDefine(const S_NormalTaskInfo *pInfo, S_TIME timeData, strings &strDatas, S_MPBS &mpbsFailed, S_DataDensity Density)//��ʾ������������ݸ�ʽ��֯����
{
	S_MPBS MPBS;
	strings strAppDatas;

	if (MakeMeasurePointBitset(pInfo, MPBS) == false)
		return 0;

	for (S_WORD MP = (S_WORD)MPBS.begin(); MP < MPBS.end(); MP++)
	{
		C_SYSCTRL::ProcessHeartBeat();
		if (MPBS.test(MP) == false)
			continue; // ���ڵı��
		if (mpbsFailed.test(MP) == false)
			continue; // ��Ҫ�ϱ��ı��

		bool fReadOk = true;
		S_WORD DA = C_DAPN::PN2DA(MP);
		string strData((char *)&DA, 2);
		for (S_WORD FnIdx = 0; FnIdx < pInfo->m_DtCnt; FnIdx++)
		{
			string strMyData;
			S_DWORD FN = pInfo->m_DT[FnIdx];
			S_TIME timeMyData = GetDataSampleTime(pInfo, timeData, Density, FN);
			if (timeMyData >= C_TIME::TimeNow()) // ������ǰʱ��
				timeMyData = C_TIME::DateTrim(TIME_UNIT_HOUR, C_TIME::TimeNow());

			if (C_Class2Data::Read(FN, MP, strMyData, timeMyData) >= 0)
			{
				strData.append(strMyData);
			}
			else
			{
				string strFillData;
				S_MSDI msdi = STATUS.FindMasterStationDataID(CLASSDATA_2, FN, 0x00);
				if (msdi.FillData(strFillData) <= 0)
					loget<<"��������[Class=2,DI="<<HEX<<FN<<"]���ʧ��!!!"<<endl;;
				strData.append(strFillData);
				
				fReadOk = false;
			}
		}
		if (fReadOk == true)
			mpbsFailed.reset(MP);

		strAppDatas.push_back(strData);
	}

	if (strAppDatas.empty() == true)
		return 0;

	TrimCmdInfo(strAppDatas, strDatas);

	string strFlag;
	strFlag.push_back(0x01);// 1��������������ݣ�ֻ�ϱ���������
	strDatas.InsertFront(strFlag);
	return (int)strDatas.size();
}

bool C_CmdTask::TrimCmdInfo(S_BYTE type, S_WORD DaCnt, S_WORD DtCnt, const strings &strSrcs, strings &strDsts)
{
	if (strSrcs.empty())
		return false;

	string strFrame;
	S_WORD Cnt = 0x00;
	for (size_t i = 0;i < strSrcs.size(); i++)
	{
		Cnt += 1;
		strFrame.append(strSrcs[i]);
		if (strFrame.size() > m_PackageLength || Cnt >= 254)//�����õ����֡�����Ҳû��ϵ������֯�ɾ����ٵ�֡
		{
			string strInfo;
			strInfo.push_back(type);
			strInfo.push_back((S_BYTE)DtCnt); // DI����
			strInfo.push_back((S_BYTE)Cnt); // ����������
			strInfo.append(strFrame);
			strDsts.push_back(strInfo);
			Cnt = 0x00;
			strFrame.clear();
		}
	}
	if (!strFrame.empty())
	{
		string strInfo;
		strInfo.push_back(type);
		strInfo.push_back((S_BYTE)DtCnt); // DI����
		strInfo.push_back((S_BYTE)Cnt); // ����������
		strInfo.append(strFrame);
		strDsts.push_back(strInfo);
	}
	return true;
}

bool C_CmdTask::TrimCmdInfo(const strings &strSrcs, strings &strDsts)
{
	if (strSrcs.empty())
		return false;
	
	string strFrame;
	for (size_t i = 0;i < strSrcs.size(); i++)
	{
		strFrame.append(strSrcs[i]);
		if (strFrame.size() > m_PackageLength)//�����õ����֡�����Ҳû��ϵ������֯�ɾ����ٵ�֡
		{
			strDsts.push_back(strFrame);
			strFrame.clear();
		}
	}
	if (!strFrame.empty())
		strDsts.push_back(strFrame);
	return true;
}

bool C_CmdTask::MakeMeasurePointBitset(const S_NormalTaskInfo *pInfo, S_MPBS &MPBS)
{
	S_ALLMP *pAllMP = STATUS.GetALLMP();
	S_MPBS AllMPBS = pAllMP->m_MTBS.m_AllMPBS;

	AllMPBS.reset(0);
	for (S_WORD Idx = 0x00; Idx < pInfo->m_DaCnt; Idx++)
	{
		C_DA da(pInfo->m_DA[Idx]);
		if (da.GetMpCount() == 0xFF) // ȫ����λ��ȡ��Ƶ�λ
			MPBS |= AllMPBS;
		else
			da.PrintMPBS(MPBS);
	}

	MPBS.Stat();
	if (MPBS.any() == false)
		return false;
	return true;
}

C_CmdReceiver *C_AFNCMD_TASK::GetCmdReceiver(INT32U DI)
{
	static C_CmdTask s_CmdTask;
	if (DI >= NORMAL_TASK_ID_MIN && DI <= NORMAL_TASK_ID_MAX)
		return &s_CmdTask;
	return NULL;
}

//
//���澯����
//
bool C_CmdAlarm::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	if (distance(iterBegin, iterEnd) < 12)
		return false;

	CmdObj.m_TimeInfo.append(iterBegin, iterBegin+12);
	advance(iterBegin, 12);
	return true;
}
bool C_CmdAlarm::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_TIME timeBegin = C_TIME::BcdStr2Time6B_Big(&CmdObj.m_TimeInfo[0]);
	S_TIME timeEnd = C_TIME::BcdStr2Time6B_Big(&CmdObj.m_TimeInfo[6]);
	if (timeBegin >= timeEnd)
		return false;
	
	C_DA da(CmdObj.m_DA);
	if (da.GetMpCount() == 0xFF)
	{
		if (C_AlmEvt::Read(0xFFFF, CmdObj.m_DI, strResponse, timeBegin, timeEnd) <= 0)
			return false;
	}

	for (int i = 0; i < 8; i++)
	{
		if (da.TestMP(i) == true)
			C_AlmEvt::Read(da.GetMP(i), CmdObj.m_DI, strResponse, timeBegin, timeEnd);
	}
	if (strResponse.empty())
		return false;
	return true;
}
C_CmdReceiver *C_AFNCMD_ALARM::GetCmdReceiver(INT32U DI)
{
	static C_CmdAlarm CmdAlarmDefault;
	return &CmdAlarmDefault;
}

//
//���¼�����
//
bool C_CmdEvent::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	C_CmdAlarm CmdAlarm;
	return CmdAlarm.ParseCmdInfo(iterBegin, iterEnd, CmdObj);
}
bool C_CmdEvent::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_CmdAlarm CmdAlarm;
	return CmdAlarm.Action(CmdObj, strResponse);
}
C_CmdReceiver *C_AFNCMD_EVENT::GetCmdReceiver(INT32U DI)
{
	static C_CmdEvent CmdEventDefault;
	return &CmdEventDefault;
}

//
//�ļ�����
//
C_CmdReceiver *C_AFNCMD_FTP::GetCmdReceiver(INT32U DI)
{
	return NULL;
}

//
//����(���ն˴���)
//
// ���ն���ѯ����
bool C_CmdE0000930::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_HttxStatus *pHS = CONTEXT.GetHttxStatus(MP_HTTX_1);// default HTTX1
	if (pHS->m_fMasterTerm == true)
		return false;//���ն˲�������ѯ����

	S_ConnectStatus *pCS = CONTEXT.GetConnectStatus();
	pCS->m_Httx1Logon = true;//�յ����ն���ѯ������Ϊ��½

	string strCmd;
	strCmd.append((char *)&CmdObj.m_DA, 2);
	strCmd.append((char *)&CmdObj.m_DI, 4);
	if (pHS->m_ReportCmdCnt || pHS->m_ResponseCmdCnt)
		strCmd.push_back((char)0x55);
	else
		strCmd.push_back((char)0xFF);

	strResponse.push_back(strCmd);
	return true;
}

// ���ն˴����������
bool C_CmdE0000931::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_HttxStatus *pHS = CONTEXT.GetHttxStatus(MP_HTTX_1);// default HTTX1
	if (pHS->m_fMasterTerm == true)
		return false;//���ն˲����մ����������

	if (pHS->m_ReportCmdCnt || pHS->m_ResponseCmdCnt)
	{
		pHS->m_fForwardEnable = true;
		return false;//�������ϴ�
	}

	/** @brief ���ն˻ظ��޺���֡����*/
	S_DWORD diTransfer = 0xE0000932UL;
	string strCmd;
	strCmd.append((char *)&CmdObj.m_DA, 2);
	strCmd.append((char *)&diTransfer, 4);
	strCmd.push_back((char)0xFF);

	strResponse.push_back(strCmd);
	return true;
}

// �����ն�̨���ܵ���
bool C_CmdE0000933::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_HttxStatus *pHS = CONTEXT.GetHttxStatus(MP_HTTX_1);// default HTTX1
	if (pHS->m_fMasterTerm == true)
		return false;//���ն˲����ճ����ն�̨���ܵ�������

	S_DWORD FN = 0x05060100UL;
	string strCmd;
	strCmd.append((char *)&CmdObj.m_DA, 2);
	strCmd.append((char *)&CmdObj.m_DI, 4);
	strCmd.append((char *)&FN, 4);
//	S_TIME timeData = C_TIME::TimeNow() - 60*60*24;//����һ��
//	if (C_Class2Data::Read(FN, MeterDatas, timeData) < 0)
		strCmd.append(4, (char)0xFF);//TODO:
//	else
//		strCmd.append(MeterDatas[0].m_Data);
	return true;
}

int C_AFNCMD_FORWARD::Execute(const S_CmdObjs &CmdObjs, strings &strResponseInfos)
{
	if (CmdObjs.empty())
		return FRAME_PROCESS_RESPONSE_DENY;
	for (S_CmdObjs::const_iterator iterNow = CmdObjs.begin(); iterNow != CmdObjs.end(); advance(iterNow, 1))
	{
		C_CmdReceiver *pCmdReveiver = GetCmdReceiver(iterNow->m_DI);
		if (pCmdReveiver == NULL)
			return FRAME_PROCESS_RESPONSE_DENY;
		strings strResponse;
		if (pCmdReveiver->Prompt() != NULL)
			logit<<"�����:"<<pCmdReveiver->Prompt()<<endl;
		pCmdReveiver->SetPackageLength(m_PackageLength);
		if (pCmdReveiver->Action(*iterNow, strResponse) == false)
			return FRAME_PROCESS_STOP;
		for (size_t i = 0; i < strResponse.size(); i++)
			strResponseInfos.push_back(strResponse[i]);
	}
	if (strResponseInfos.empty())
		return FRAME_PROCESS_RESPONSE_CONFIRM;
	return FRAME_PROCESS_RESPONSE_DATA;
}

C_CmdReceiver *C_AFNCMD_FORWARD::GetCmdReceiver(INT32U DI)
{
	static C_CmdE0000930 s_CmdE0000930;
	static C_CmdE0000931 s_CmdE0000931;
	static C_CmdE0000933 s_CmdE0000933;
	switch (DI)
	{
	case 0xE0000930: return &s_CmdE0000930;
	case 0xE0000931: return &s_CmdE0000931;
//	case 0xE0000932/* �ظ������������,���ն˲��ᷢ������ID����*/
	case 0xE0000933: return &s_CmdE0000933;
	default:break;
	}
	return NULL;
}

//
//����(���ն˴���)
//
//���ն���ѯ����ظ�
bool C_CmdMsE0000930::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	if (distance(iterBegin, iterEnd) < 1)
		return false;

	CmdObj.m_DiInfo.append(iterBegin, iterBegin+1);
	advance(iterBegin, 1);
	return true;
}

bool C_CmdMsE0000930::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_BYTE flag = CmdObj.m_DiInfo[0];
	if (flag == 0x55) //��ѯ��55H�����ϱ�����
		return true;
	//FFH�����ϱ�����
	return false;
}

// ���ն˴����������ظ�
bool C_CmdMsE0000932::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	if (distance(iterBegin, iterEnd) < 1)
		return false;

	CmdObj.m_DiInfo.append(iterBegin, iterBegin+1);
	advance(iterBegin, 1);
	return true;
}

bool C_CmdMsE0000932::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
//	�ظ������������FF����ʾ�޺���֡
//	S_BYTE flag = CmdObj.m_DiInfo[0];
//	if (flag == 0xFF)
//		return false;
	return false;//�յ��������ʾ�޺���֡
}

//�ն�̨���ܵ����ظ�
bool C_CmdMsE0000933::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	if (distance(iterBegin, iterEnd) < 8)
		return false;

	CmdObj.m_DiInfo.append(iterBegin, iterBegin+8);
	advance(iterBegin, 8);
	return true;
}

bool C_CmdMsE0000933::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	strResponse.push_back(CmdObj.m_DiInfo);
	return false;//��ʾ�е���
}

int C_AFNCMD_FORWARD_MS::Execute(const S_CmdObjs &CmdObjs, strings &strResponseInfos)
{
	if (CmdObjs.empty())
		return FRAME_PROCESS_STOP;
	for (S_CmdObjs::const_iterator iterNow = CmdObjs.begin(); iterNow != CmdObjs.end(); advance(iterNow, 1))
	{
		C_CmdReceiver *pCmdReveiver = GetCmdReceiver(iterNow->m_DI);
		if (pCmdReveiver == NULL)
			return FRAME_PROCESS_STOP;
		strings strResponse;
		if (pCmdReveiver->Prompt() != NULL)
			logit<<"�����:"<<pCmdReveiver->Prompt()<<endl;
		pCmdReveiver->SetPackageLength(m_PackageLength);
		if (pCmdReveiver->Action(*iterNow, strResponse) == true)
			return PARSE_SLAVE_REPORT_YES;
		if (!strResponse.empty())
		{
			strResponseInfos.push_back(strResponse[0]);
			return PARSE_SLAVE_GATHER_ENERGY;
		}
	}
	return PARSE_SLAVE_REPORT_NO;
}

C_CmdReceiver *C_AFNCMD_FORWARD_MS::GetCmdReceiver(INT32U DI)
{
	static C_CmdMsE0000930 s_CmdMsE0000930;
	static C_CmdMsE0000932 s_CmdMsE0000932;
	static C_CmdMsE0000933 s_CmdMsE0000933;
	switch (DI)
	{
	case 0xE0000930: return &s_CmdMsE0000930;
//	case 0xE0000931: /* ������ƣ����ն˲����ϱ���������*/
	case 0xE0000932: return &s_CmdMsE0000932;
	case 0xE0000933: return &s_CmdMsE0000933;
	}
	return NULL;
}

//
//�м�ת��
//
bool C_CmdRoute::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	if (distance(iterBegin, iterEnd) < 8)
		return false;
	S_WORD FrameLen = *(iterBegin + 7);
	if (distance(iterBegin, iterEnd) < 8 + FrameLen)
		return false;

	CmdObj.m_DiInfo.append(iterBegin, iterBegin+8+FrameLen);
	advance(iterBegin, 8+FrameLen);
	return true;
}

bool C_CmdRoute::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strIpcData;
	const char *pPrecess = CONTEXT.GetDbcjPrecessName_VCom(CmdObj.m_DiInfo[1] + 1);

	AppendDADT(strIpcData, CmdObj);
	strIpcData.append(CmdObj.m_DiInfo);
	if (C_SysMsg::Send2Server(pPrecess, MSAFN_DATA_ROUTE, strIpcData) != 0)
		return false;
	strIpcData.clear();
	if (C_SysMsg::Wait4Server(MSAFN_DATA_ROUTE, strIpcData) != 0)
		return false;
	if (strIpcData.size() < 6)
		return false;
	strResponse.push_back(strIpcData);
	return true;
}

C_CmdReceiver *C_AFNCMD_ROUTE::GetCmdReceiver(INT32U DI)
{
	static C_CmdRoute CmdRoute;
	return &CmdRoute;
}


