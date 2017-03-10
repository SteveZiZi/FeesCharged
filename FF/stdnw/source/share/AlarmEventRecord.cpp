#include "AlarmEventRecord.h"
#include "datafile.h"
/*
	brief: save alram data to alarm file ,as a record.
	param [in] CMeter meter:the class of meter operation
			   INT32U alarmFn:alarm fn id
			   T_emAlarmStatus state:alarm state
			   S_TIME alarmTime: alarm record saved time
	return bool 
	author:WanQingsong
	date: 2012-04-09
*/  
bool CAlarmRecrod_ARD1::AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime)
{
	string alarmData;

	AppendStateTime(alarmData, state, alarmTime);
	/*�澯���ݱ���*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
}

bool CAlarmRecrod_ARD1::AppendStateTime(string &strAlmData, T_emAlarmStatus state, S_TIME alarmTime)
{
	char buffer[7];

	/* �澯״̬ */
	strAlmData.push_back((char)state);				

	/* �澯ʱ�� */
	C_TIME::Time2BCDStr(alarmTime, buffer);
	strAlmData.append(buffer, sizeof(buffer)-1);

	return true;
}

vector<INT32U> CAlarmRecrod_ARD2::m_gatherFnIDs;
void CAlarmRecrod_ARD2::Init()
{
	//�����й��ܵ���
	m_gatherFnIDs.push_back(0x60700000);
	//�����й��ܵ���
	m_gatherFnIDs.push_back(0x60D00000);
	//����޹�1�ܵ���
	m_gatherFnIDs.push_back(0x91100000);
	//����޹�2�ܵ���
	m_gatherFnIDs.push_back(0x91200000);
	//�����ѹ
	m_gatherFnIDs.push_back(0xB6110000);
	m_gatherFnIDs.push_back(0xB6120000);
	m_gatherFnIDs.push_back(0xB6130000);
	m_gatherFnIDs.push_back(0x71001001);
	m_gatherFnIDs.push_back(0x71001002);
	m_gatherFnIDs.push_back(0x71001003);
	m_gatherFnIDs.push_back(0x71001004);
	m_gatherFnIDs.push_back(0x71001005);
	m_gatherFnIDs.push_back(0x71001006);
	
	//�������
	m_gatherFnIDs.push_back(0xB6210000);
	m_gatherFnIDs.push_back(0xB6220000);
	m_gatherFnIDs.push_back(0xB6230000);
	m_gatherFnIDs.push_back(0x71002001);
	m_gatherFnIDs.push_back(0x71002002);
	m_gatherFnIDs.push_back(0x71002003);
	//�й�����
	m_gatherFnIDs.push_back(0xB6300000);
	m_gatherFnIDs.push_back(0xB6310000);
	m_gatherFnIDs.push_back(0xB6320000);
	m_gatherFnIDs.push_back(0xB6330000);	
	//�޹�����
	m_gatherFnIDs.push_back(0xB6400000);
	m_gatherFnIDs.push_back(0xB6410000);
	m_gatherFnIDs.push_back(0xB6420000);
	m_gatherFnIDs.push_back(0xB6430000);	
	//��������
	m_gatherFnIDs.push_back(0xB6500000);
	m_gatherFnIDs.push_back(0xB6510000);
	m_gatherFnIDs.push_back(0xB6520000);
	m_gatherFnIDs.push_back(0xB6530000);	


}
static CAlarmRecrod_ARD2 s_AlarmRecrod_ARD2; 
/*
	brief: save alram data to alarm file ,as a record.
	param [in] CMeter meter:the class of meter operation
			   INT32U alarmFn:alarm fn id
			   T_emAlarmStatus state:alarm state
			   S_TIME alarmTime: alarm record saved time
	return bool 
	author:WanQingsong
	date: 2012-04-09
*/  
bool CAlarmRecrod_ARD2::AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime)
{
	string alarmData;
	S_RegDatas CtxRegData;

	CAlarmRecrod_ARD1::AppendStateTime(alarmData, state, alarmTime);

	if (meter.m_MeasureNo == 0x00)
	{// �ն˸澯
		InitTermData(CtxRegData);
	}
	else if (CAlarmEvtCommIdData::GetCommRegDataFromCTX(meter.m_MeasureNo,REGS_ARD2,CtxRegData) == false)
	{
		loget<<"CAlarmRecrod_ARD2:: GetCommRegDataFromCTX ʧ��"<<endl;
		if (CAlarmEvtCommIdData::GetCommRegDataFromSTS(meter.m_MeasureNo,REGS_ARD2,CtxRegData) == false)
		{
			loget<<"CAlarmRecrod_ARD2:: GetCommRegDataFromSTS ʧ��"<<endl;	
			return false;
		}
	}

	int index = 0;	
	for (int i=0; i<(int)CtxRegData.size(); i++)
	{
		if (CtxRegData[i].m_regid == m_gatherFnIDs[index])
		{
			index++;
			alarmData.append(CtxRegData[i].m_data);
		}
	}

	/* wqs debug*/
	logvt<<"CAlarmRecrod_ARD2: "<<HEX<<alarmData<<endl;
	/* end */


	/*�澯���ݱ���*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
		
}

bool CAlarmRecrod_ARD2::InitTermData(S_RegDatas &CtxRegData)
{
	for (size_t i = 0; i < m_gatherFnIDs.size(); i++)
	{
		S_RegData RegData;
		S_REGISTER CommReg = STATUS.FindCommonRegister(m_gatherFnIDs[i]);
		if (CommReg.m_ID == 0)
			logwt<<"ͨ�üĴ���["<<HEX<<m_gatherFnIDs[i]<<"]û������"<<endl;

		RegData.m_regid = CommReg.m_ID;
		CommReg.FillData(RegData.m_data);
		CtxRegData.push_back(RegData);
	}
	return true;
}


vector<INT32U> CAlarmRecrod_ARD3::m_gatherFnIDs;
	
void CAlarmRecrod_ARD3::Init()
{
	//�����й��ܵ���
	m_gatherFnIDs.push_back(0x60700000);
	//�����й��ܵ���
	m_gatherFnIDs.push_back(0x60D00000);
	//����޹�1�ܵ���
	m_gatherFnIDs.push_back(0x91100000);
	//����޹�2�ܵ���
	m_gatherFnIDs.push_back(0x91200000);
	//�����ѹ
	m_gatherFnIDs.push_back(0xB6110000);
	m_gatherFnIDs.push_back(0xB6120000);
	m_gatherFnIDs.push_back(0xB6130000);
	m_gatherFnIDs.push_back(0x71001001);
	m_gatherFnIDs.push_back(0x71001002);
	m_gatherFnIDs.push_back(0x71001003);
	m_gatherFnIDs.push_back(0x71001004);
	m_gatherFnIDs.push_back(0x71001005);
	m_gatherFnIDs.push_back(0x71001006);
	//�������
	m_gatherFnIDs.push_back(0xB6210000);
	m_gatherFnIDs.push_back(0xB6220000);
	m_gatherFnIDs.push_back(0xB6230000);
	m_gatherFnIDs.push_back(0x71002001);
	m_gatherFnIDs.push_back(0x71002002);
	m_gatherFnIDs.push_back(0x71002003);
	//�й�����
	m_gatherFnIDs.push_back(0xB6300000);
	m_gatherFnIDs.push_back(0xB6310000);
	m_gatherFnIDs.push_back(0xB6320000);
	m_gatherFnIDs.push_back(0xB6330000);	
	//�޹�����
	m_gatherFnIDs.push_back(0xB6400000);
	m_gatherFnIDs.push_back(0xB6410000);
	m_gatherFnIDs.push_back(0xB6420000);
	m_gatherFnIDs.push_back(0xB6430000);	
}
static CAlarmRecrod_ARD3 s_AlarmRecrod_ARD3; 
/*
	brief: save alram data to alarm file ,as a record.
	param [in] CMeter meter:the class of meter operation
			   INT32U alarmFn:alarm fn id
			   T_emAlarmStatus state:alarm state
			   S_TIME alarmTime: alarm record saved time
	return bool 
	author:WanQingsong
	date: 2012-04-09
*/  
bool CAlarmRecrod_ARD3::AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime)
{
	string alarmData;
	S_RegDatas CtxRegData;
	S_RegDatas StxRegData;

	CAlarmRecrod_ARD1::AppendStateTime(alarmData, state, alarmTime);

	/* ����ǰ����*/
	if (CAlarmEvtCommIdData::GetCommRegDataFromSTS(meter.m_MeasureNo,alarmFn,StxRegData) == false)
	{
		loget<<"CAlarmRecrod_ARD3 GetCommRegDataFromSTS ʧ��"<<endl;
		return false;
	}

	/* ����������*/
	if (CAlarmEvtCommIdData::GetCommRegDataFromCTX(meter.m_MeasureNo,REGS_ARD2,CtxRegData) == false)
	{
		loget<<"CAlarmRecrod_ARD3 GetCommRegDataFromCTX ʧ��"<<endl;
		return false;
	}

	int index = 0;
	for (int i=0; i<(int)StxRegData.size(); i++)
	{
		if (StxRegData[i].m_regid == m_gatherFnIDs[index])
		{
			index++;
			alarmData.append(StxRegData[i].m_data);
		}
	}

	index = 0;
	for (int i=0; i<(int)CtxRegData.size(); i++)
	{
		if (CtxRegData[i].m_regid == m_gatherFnIDs[index])
		{
			index++;
			alarmData.append(CtxRegData[i].m_data);
		}
	}

	/* wqs debug*/
	logvt<<"CAlarmRecrod_ARD3: "<<HEX<<alarmData<<endl;
	/* end */
	/*�澯���ݱ���*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
}

vector<INT32U> CAlarmRecrod_ARD4::m_gatherFnIDs;
void CAlarmRecrod_ARD4::Init()
{
	m_gatherFnIDs.clear();

	m_gatherFnIDs.push_back(0x60700000);//�����й��ܵ���
	m_gatherFnIDs.push_back(0x60D00000);//�����й��ܵ���
	m_gatherFnIDs.push_back(0x91100000);//����޹�1�ܵ���
	m_gatherFnIDs.push_back(0x91200000);//����޹�2�ܵ���
	m_gatherFnIDs.push_back(0x71003003);//ʣ�����
}
static CAlarmRecrod_ARD4 s_AlarmRecrod_ARD4;
/*
	brief: save alram data to alarm file ,as a record.
	param [in] CMeter meter:the class of meter operation
			   INT32U alarmFn:alarm fn id
			   T_emAlarmStatus state:alarm state
			   S_TIME alarmTime: alarm record saved time
	return bool 
	author:WanQingsong
	date: 2012-04-09
*/  
bool CAlarmRecrod_ARD4::AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime)
{
	string alarmData;
	S_RegDatas CtxRegData;

	CAlarmRecrod_ARD1::AppendStateTime(alarmData, state, alarmTime);

	/* ����������*/
	if (CAlarmEvtCommIdData::GetCommRegDataFromCTX(meter.m_MeasureNo,alarmFn,CtxRegData) == false)
	{
		return false;
	}

	int index=0;
	for (int i=0; i<(int)CtxRegData.size(); i++)
	{
		if (CtxRegData[i].m_regid == m_gatherFnIDs[index])
		{
			index++;
			alarmData.append(CtxRegData[i].m_data);
		}
	}
	
	/* wqs debug*/
	logvt<<"CAlarmRecrod_ARD4: "<<HEX<<alarmData<<endl;
	/* end */
	/*�澯���ݱ���*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
}

vector<INT32U> CAlarmRecrod_ARD5::m_BeforeGatherFnIDs;
vector<INT32U> CAlarmRecrod_ARD5::m_AfterGatherFnIDs;

void CAlarmRecrod_ARD5::Init()
{
	m_BeforeGatherFnIDs.clear();
	m_AfterGatherFnIDs.clear();
	
	/* �澯ǰ�ɼ��������� */
	m_BeforeGatherFnIDs.push_back(0x00010000);//�����й��ܵ���
	m_BeforeGatherFnIDs.push_back(0x00020000);//�����й��ܵ���
	m_BeforeGatherFnIDs.push_back(0x00030000);//����޹�1�ܵ���
	m_BeforeGatherFnIDs.push_back(0x00040000);//����޹�2�ܵ���
	m_BeforeGatherFnIDs.push_back(0xE080000D);//PT���
	m_BeforeGatherFnIDs.push_back(0xE080000C);//CT���

	/* �澯��ɼ��������� */
	m_AfterGatherFnIDs.push_back(0xE080000D);//PT���
	m_AfterGatherFnIDs.push_back(0xE080000C);//CT���
	
}
static CAlarmRecrod_ARD5 s_AlarmRecrod_ARD5;
/*
	brief: save alram data to alarm file ,as a record.
	param [in] CMeter meter:the class of meter operation
			   INT32U alarmFn:alarm fn id
			   T_emAlarmStatus state:alarm state
			   S_TIME alarmTime: alarm record saved time
	return bool 
	author:WanQingsong
	date: 2012-04-09
*/  
bool CAlarmRecrod_ARD5::AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime)
{
	string alarmData;
	string outData;

	CAlarmRecrod_ARD1::AppendStateTime(alarmData, state, alarmTime);

	/* ����ǰ����*/
	for (size_t i=0; i<m_BeforeGatherFnIDs.size(); i++)
	{
		outData.clear();
//		meter.ReadDataFileFnData(CONTEXT.GetGatherFns()->FindFn(m_BeforeGatherFnIDs[i]),outData, 0xffff);
		alarmData.append(outData);
	}

	/* ����������*/
		
	for (size_t i=0; i<m_AfterGatherFnIDs.size(); i++)
	{
		outData.clear();
//		meter.ReadFnData(CONTEXT.GetGatherFns()->FindFn(m_AfterGatherFnIDs[i]), outData);
		alarmData.append(outData);
	}
	/*�澯���ݱ���*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
}

vector<INT32U> CAlarmRecrod_ARD6::m_BeforeGatherFnIDs;
vector<INT32U> CAlarmRecrod_ARD6::m_AfterGatherFnIDs;

void CAlarmRecrod_ARD6::Init()
{
	m_BeforeGatherFnIDs.clear();
	m_AfterGatherFnIDs.clear();
	
	/* �澯ǰ�ɼ��������� */
	m_BeforeGatherFnIDs.push_back(0x00010000);//�����й��ܵ���
	m_BeforeGatherFnIDs.push_back(0x00020000);//�����й��ܵ���
	m_BeforeGatherFnIDs.push_back(0x00030000);//����޹�1�ܵ���
	m_BeforeGatherFnIDs.push_back(0x00040000);//����޹�2�ܵ���
	m_BeforeGatherFnIDs.push_back(0xE1000003);//�����(�й�)
	m_BeforeGatherFnIDs.push_back(0xE1000004);//�����(�޹�)

	/* �澯��ɼ��������� */
	m_AfterGatherFnIDs.push_back(0xE1000003);//�����(�й�)
	m_AfterGatherFnIDs.push_back(0xE1000004);//�����(�޹�)
	
}

static CAlarmRecrod_ARD6 s_AlarmRecrod_ARD6;
/*
	brief: save alram data to alarm file ,as a record.
	param [in] CMeter meter:the class of meter operation
			   INT32U alarmFn:alarm fn id
			   T_emAlarmStatus state:alarm state
			   S_TIME alarmTime: alarm record saved time
	return bool 
	author:WanQingsong
	date: 2012-04-09
*/  
bool CAlarmRecrod_ARD6::AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime)
{
	string alarmData;
	string outData;

	CAlarmRecrod_ARD1::AppendStateTime(alarmData, state, alarmTime);

	/* ����ǰ����*/
	for (size_t i=0; i<m_BeforeGatherFnIDs.size(); i++)
	{
		outData.clear();
//		meter.ReadDataFileFnData(CONTEXT.GetGatherFns()->FindFn(m_BeforeGatherFnIDs[i]),outData, 0xffff);
		alarmData.append(outData);
	}

	/* ����������*/
		
	for (size_t i=0; i<m_AfterGatherFnIDs.size(); i++)
	{
		outData.clear();
//		meter.ReadFnData(CONTEXT.GetGatherFns()->FindFn(m_AfterGatherFnIDs[i]), outData);
		alarmData.append(outData);
	}
	/*�澯���ݱ���*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
}

vector<INT32U> CAlarmRecrod_ARD7::m_BeforeGatherFnIDs;
void CAlarmRecrod_ARD7::Init()
{
	m_BeforeGatherFnIDs.clear();
	
	/* �澯ǰ�ɼ��������� */
	m_BeforeGatherFnIDs.push_back(0x00010000);//�����й��ܵ���
	m_BeforeGatherFnIDs.push_back(0x00020000);//�����й��ܵ���
	m_BeforeGatherFnIDs.push_back(0x00030000);//����޹�1�ܵ���
	m_BeforeGatherFnIDs.push_back(0x00040000);//����޹�2�ܵ���
	m_BeforeGatherFnIDs.push_back(0x01010000);//�����й����������������ʱ��
	m_BeforeGatherFnIDs.push_back(0x01020000);//�����й����������������ʱ��
	m_BeforeGatherFnIDs.push_back(0x01030000);//����޹�1���������������ʱ��
	m_BeforeGatherFnIDs.push_back(0x01040000);//����޹�2���������������ʱ��
}
static CAlarmRecrod_ARD7 s_AlarmRecrod_ARD7;
/*
	brief: save alram data to alarm file ,as a record.
	param [in] CMeter meter:the class of meter operation
			   INT32U alarmFn:alarm fn id
			   T_emAlarmStatus state:alarm state
			   S_TIME alarmTime: alarm record saved time
	return bool 
	author:WanQingsong
	date: 2012-04-09
*/  
bool CAlarmRecrod_ARD7::AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime)
{
	string alarmData;
	string outData;

	CAlarmRecrod_ARD1::AppendStateTime(alarmData, state, alarmTime);

	/* ����ǰ����*/
	for (size_t i=0; i<m_BeforeGatherFnIDs.size(); i++)
	{
		outData.clear();
//		meter.ReadDataFileFnData(CONTEXT.GetGatherFns()->FindFn(m_BeforeGatherFnIDs[i]),outData, 0xffff);
		alarmData.append(outData);
	}

	/*�澯���ݱ���*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
}

vector<INT32U> CAlarmRecrod_ARD8::m_BeforeGatherFnIDs;
vector<INT32U> CAlarmRecrod_ARD8::m_AfterGatherFnIDs;
void CAlarmRecrod_ARD8::Init()
{
	m_BeforeGatherFnIDs.clear();
	m_AfterGatherFnIDs.clear();
	
	/* �澯ǰ�ɼ��������� */
	m_BeforeGatherFnIDs.push_back(0x040005FF);//�������״̬�����ݿ�

	/* �澯��ɼ��������� */
	m_AfterGatherFnIDs.push_back(0x040005FF);//�������״̬�����ݿ�
	
}
static CAlarmRecrod_ARD8 s_AlarmRecrod_ARD8; 
/*
	brief: save alram data to alarm file ,as a record.
	param [in] CMeter meter:the class of meter operation
			   INT32U alarmFn:alarm fn id
			   T_emAlarmStatus state:alarm state
			   S_TIME alarmTime: alarm record saved time
	return bool 
	author:WanQingsong
	date: 2012-04-09
*/  
bool CAlarmRecrod_ARD8::AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime)
{
	string alarmData;
	string outData;
	char buf[14]={0};

	CAlarmRecrod_ARD1::AppendStateTime(alarmData, state, alarmTime);

	/* ����ǰ����*/
	outData.clear();
//	memcpy(buf,&(meter.m_pMeterStatus->m_MeterStatusWords),14);
	alarmData.append(buf,14);

	/* ����������*/
	for (size_t i=0; i<m_AfterGatherFnIDs.size(); i++)
	{
		outData.clear();
//		meter.ReadFnData(CONTEXT.GetGatherFns()->FindFn(m_AfterGatherFnIDs[i]),outData);
		if (outData.size() == 14)
		{
			alarmData.append(outData);
		}
		else
		{
			loget<<"���״̬�ֿ��ȡʧ��"<<endl;
			return false;
		}
	}
	/*�澯���ݱ���*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
}

vector<INT32U> CAlarmRecrod_ARD9::m_BeforeGatherFnIDs;
vector<INT32U> CAlarmRecrod_ARD9::m_AfterGatherFnIDs;

void CAlarmRecrod_ARD9::Init()
{
	m_BeforeGatherFnIDs.clear();
	m_AfterGatherFnIDs.clear();
	
	/* �澯ǰ�ɼ��������� */
	m_BeforeGatherFnIDs.push_back(0xE0000C1F);//���뿪��״̬���ݿ�

	/* �澯��ɼ��������� */
	m_AfterGatherFnIDs.push_back(0xE0000C1F);//���뿪��״̬���ݿ�
	
}
static CAlarmRecrod_ARD9 s_AlarmRecrod_ARD9;
/*
	brief: save alram data to alarm file ,as a record.
	param [in] CMeter meter:the class of meter operation
			   INT32U alarmFn:alarm fn id
			   T_emAlarmStatus state:alarm state
			   S_TIME alarmTime: alarm record saved time
	return bool 
	author:WanQingsong
	date: 2012-04-09
*/  
bool CAlarmRecrod_ARD9::AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime)
{
	string alarmData;
	string outData;

	CAlarmRecrod_ARD1::AppendStateTime(alarmData, state, alarmTime);

	/* ����ǰ����*/
	for (size_t i=0; i<m_BeforeGatherFnIDs.size(); i++)
	{
		outData.clear();
//		meter.ReadDataFileFnData(CONTEXT.GetGatherFns()->FindFn(m_BeforeGatherFnIDs[i]),outData, 0xffff);
		alarmData.append(outData);
	}

	/* ����������*/
		
	for (size_t i=0; i<m_AfterGatherFnIDs.size(); i++)
	{
		outData.clear();
//		meter.ReadFnData(CONTEXT.GetGatherFns()->FindFn(m_AfterGatherFnIDs[i]), outData);
		alarmData.append(outData);
	}
	/*�澯���ݱ���*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
}

vector<INT32U> CAlarmRecrod_ARD10::m_gatherFnIDs;
void CAlarmRecrod_ARD10::Init()
{
	m_gatherFnIDs.clear();
	m_gatherFnIDs.push_back(0xE1800013);//��ͨ������
}
static CAlarmRecrod_ARD10 s_AlarmRecrod_ARD10;
/*
	brief: save alram data to alarm file ,as a record.
	param [in] CMeter meter:the class of meter operation
			   INT32U alarmFn:alarm fn id
			   T_emAlarmStatus state:alarm state
			   S_TIME alarmTime: alarm record saved time
	return bool 
	author:WanQingsong
	date: 2012-04-09
*/  
bool CAlarmRecrod_ARD10::AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime,S_DWORD monthFlow)
{
	string alarmData;

	CAlarmRecrod_ARD1::AppendStateTime(alarmData, state, alarmTime);

	S_DWORD temp = monthFlow/1024;
	alarmData.append((char *)&(temp), 3);	

	/* wqs debug*/
	logvt<<"CAlarmRecrod_ARD10: "<<HEX<<alarmData<<endl;
	/* end */

	/*�澯���ݱ���*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
		
}

vector<INT32U> CAlarmRecrod_ARD11::m_gatherFnIDs;
void CAlarmRecrod_ARD11::Init()
{
	m_gatherFnIDs.clear();
	m_gatherFnIDs.push_back(0xE1800010);//��������
}
static CAlarmRecrod_ARD11 s_AlarmRecrod_ARD11;
/*
	brief: save alram data to alarm file ,as a record.
	param [in] CMeter meter:the class of meter operation
			   INT32U alarmFn:alarm fn id
			   T_emAlarmStatus state:alarm state
			   S_TIME alarmTime: alarm record saved time
	return bool 
	author:WanQingsong
	date: 2012-04-09
*/  
bool CAlarmRecrod_ARD11::AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime)
{
	string alarmData;
	string outData;

	CAlarmRecrod_ARD1::AppendStateTime(alarmData, state, alarmTime);

	for (size_t i=0; i<m_gatherFnIDs.size(); i++)
	{
		outData.clear();
//		meter.ReadMemoryFnData(CONTEXT.GetGatherFns()->FindFn(m_gatherFnIDs[i]),outData, 0);
		alarmData.append(outData);	
	}

	/*�澯���ݱ���*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
}

vector<INT32U> CAlarmRecrod_ARD12::m_gatherFnIDs;
void CAlarmRecrod_ARD12::Init()
{
	m_gatherFnIDs.push_back(0x04000503);//�������״̬��3
}
static CAlarmRecrod_ARD12 s_AlarmRecrod_ARD12;
/*
	brief: save alram data to alarm file ,as a record.
	param [in] CMeter meter:the class of meter operation
			   INT32U alarmFn:alarm fn id
			   T_emAlarmStatus state:alarm state
			   S_TIME alarmTime: alarm record saved time
	return bool 
	author:WanQingsong
	date: 2012-04-09
*/  
bool CAlarmRecrod_ARD12::AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime)
{
	string alarmData;
	S_RegDatas CtxRegData;

	CAlarmRecrod_ARD1::AppendStateTime(alarmData, state, alarmTime);

	/* ����������*/
	if (CAlarmEvtCommIdData::GetCommRegDataFromCTX(meter.m_MeasureNo,alarmFn,CtxRegData) == false)
	{
		return false;
	}

	alarmData.append(CtxRegData[0].m_data);

	/* wqs debug*/
	logvt<<"CAlarmRecrod_ARD12: "<<HEX<<alarmData<<endl;
	/* end */
	
	/*�澯���ݱ���*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
		
}

bool CAlarmRecrod_ARD12::AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime, string meterStateChar3)
{
	string alarmData;

	CAlarmRecrod_ARD1::AppendStateTime(alarmData, state, alarmTime);

	/* ����������*/
	alarmData.append(meterStateChar3);

	/* wqs debug*/
	logvt<<"CAlarmRecrod_ARD12: "<<HEX<<alarmData<<endl;
	/* end */
	
	/*�澯���ݱ���*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
		
}


vector<INT32U> CAlarmRecrod_ARD13::m_gatherFnIDs;

void CAlarmRecrod_ARD13::Init()
{
	m_gatherFnIDs.clear();
	m_gatherFnIDs.push_back(0x04000102);//���ܱ�ʱ��
	m_gatherFnIDs.push_back(0x04000101);//���ܱ�����
//	m_gatherFnIDs.push_back(0xE0000130);//�ն�ʱ��
}
static CAlarmRecrod_ARD13 s_AlarmRecrod_ARD13;
/*
	brief: save alram data to alarm file ,as a record.
	param [in] CMeter meter:the class of meter operation
			   INT32U alarmFn:alarm fn id
			   T_emAlarmStatus state:alarm state
			   S_TIME alarmTime: alarm record saved time
	return bool 
	author:WanQingsong
	date: 2012-04-09
*/  
bool CAlarmRecrod_ARD13::AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime)
{
	string alarmData;
	S_RegDatas CtxRegData;

	CAlarmRecrod_ARD1::AppendStateTime(alarmData, state, alarmTime);

	/* ����������*/
	if (CAlarmEvtCommIdData::GetCommRegDataFromCTX(meter.m_MeasureNo,alarmFn,CtxRegData) == false)
	{
		return false;
	}

	/* ������� YYMMDDWW*/
	char buf[4]={0};
	memcpy(buf,CtxRegData[1].m_data.data(),sizeof(buf));
//	reverse(buf,buf+4);
	alarmData.append(buf,4);	
	
	/* ���ʱ�� HHMMSS*/
	char buff[3]={0};
	memcpy(buff,CtxRegData[0].m_data.data(),sizeof(buff));
//	reverse(buff,buff+3);
	alarmData.append(buff,3);

	/* �ն�ʱ��:YYMMDDHHMMSS*/
	char time[7] = {0};
	C_TIME::Time2BCDStr(alarmTime, time);
	alarmData.append(time, 6);

	/* wqs debug*/
	logvt<<"CAlarmRecrod_ARD13: "<<HEX<<alarmData<<endl;
	/* end */
	
	/*�澯���ݱ���*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
		
}

vector<INT32U> CAlarmRecrod_ARD14::m_gatherFnIDs;

void CAlarmRecrod_ARD14::Init()
{
	m_gatherFnIDs.clear();

	m_gatherFnIDs.push_back(0x020A0100);//A���ѹ��г��������
	m_gatherFnIDs.push_back(0x020A0200);//B���ѹ��г��������
	m_gatherFnIDs.push_back(0x020A0300);//C���ѹ��г��������
	m_gatherFnIDs.push_back(0x020B0100);//A�������г��������
	m_gatherFnIDs.push_back(0x020B0200);//B�������г��������
	m_gatherFnIDs.push_back(0x020B0300);//C�������г��������

	m_gatherFnIDs.push_back(0xE1000101);//A���ѹƫ�����ֵ
	m_gatherFnIDs.push_back(0xE1000102);//B���ѹƫ�����ֵ
	m_gatherFnIDs.push_back(0xE1000103);//C���ѹƫ�����ֵ
	m_gatherFnIDs.push_back(0xE1000104);//Ƶ��ƫ�����ֵ
	m_gatherFnIDs.push_back(0xE1000105);//��ѹ��ƽ������ֵ
	m_gatherFnIDs.push_back(0xE100010D);//��ʱ�������ֵ
	m_gatherFnIDs.push_back(0xE100010E);//��ʱ�������ֵ
	m_gatherFnIDs.push_back(0xE100010F);//��ѹ�ܻ������ֵ

}
static CAlarmRecrod_ARD14 s_AlarmRecrod_ARD14;
/*
	brief: save alram data to alarm file ,as a record.
	param [in] CMeter meter:the class of meter operation
			   INT32U alarmFn:alarm fn id
			   T_emAlarmStatus state:alarm state
			   S_TIME alarmTime: alarm record saved time
	return bool 
	author:WanQingsong
	date: 2012-04-09
*/  
bool CAlarmRecrod_ARD14::AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime)
{
	string alarmData;
	string outData;

	CAlarmRecrod_ARD1::AppendStateTime(alarmData, state, alarmTime);

	for (size_t i=0; i<m_gatherFnIDs.size(); i++)
	{
		outData.clear();
//		meter.ReadMemoryFnData(CONTEXT.GetGatherFns()->FindFn(m_gatherFnIDs[i]),outData, 0);
		alarmData.append(outData);	
	}

	/*�澯���ݱ���*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
		
}

vector<INT32U> CAlarmRecrod_ARD15::m_gatherFnIDs;

void CAlarmRecrod_ARD15::Init()
{
	
}

/*
	brief: save alram data to alarm file ,as a record.
	param [in] CMeter meter:the class of meter operation
			   INT32U alarmFn:alarm fn id
			   T_emAlarmStatus state:alarm state
			   S_TIME alarmTime: alarm record saved time
	return bool 
	author:WanQingsong
	date: 2012-04-09
*/  
bool CAlarmRecrod_ARD15::AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime)
{
	string alarmData;
	S_AlmDatas *pCtxAlmDatas =  CONTEXT.GetAlarmDatas();
	string CtxAlmData;

	CAlarmRecrod_ARD1::AppendStateTime(alarmData, state, alarmTime);
	
	/* ����������*/
	if (pCtxAlmDatas->Load(alarmFn,meter.m_MeasureNo,CtxAlmData) ==-1)
	{
		loget<<"CONTEXT �ڴ�澯���ݼ���ʧ��!"<<endl;
		return false;
	}

	alarmData.append(CtxAlmData);

	/* wqs debug*/
	logvt<<"CAlarmRecrod_ARD15: "<<HEX<<alarmData<<endl;
	/* end */

	/*�澯���ݱ���*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
		
}

vector<INT32U> CAlarmRecrod_ARD16::m_gatherFnIDs;

void CAlarmRecrod_ARD16::Init()
{
}
/*
	brief: save alram data to alarm file ,as a record.
	param [in] CMeter meter:the class of meter operation
			   INT32U alarmFn:alarm fn id
			   T_emAlarmStatus state:alarm state
			   S_TIME alarmTime: alarm record saved time
	return bool 
	author:WanQingsong
	date: 2012-04-09
*/  
bool CAlarmRecrod_ARD16::AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime)
{
	string alarmData;
	S_AlmDatas *pCtxAlmDatas =  CONTEXT.GetAlarmDatas();
	string CtxAlmData;

	CAlarmRecrod_ARD1::AppendStateTime(alarmData, state, alarmTime);
	
	/* ����������*/
	if (pCtxAlmDatas->Load(alarmFn,meter.m_MeasureNo,CtxAlmData) ==-1)
	{
		loget<<"CONTEXT �ڴ�澯���ݼ���ʧ��!"<<endl;
		return false;
	}

	alarmData.append(CtxAlmData);

	/* wqs debug*/
	logvt<<"CAlarmRecrod_ARD16: "<<HEX<<alarmData<<endl;
	/* end */

	/*�澯���ݱ���*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
		
}



void CAlarmRecrod_ARD17::Init()
{
}

/*
	brief: save alram data to alarm file ,as a record.
	param [in] CMeter meter:the class of meter operation
			   INT32U alarmFn:alarm fn id
			   T_emAlarmStatus state:alarm state
			   S_TIME alarmTime: alarm record saved time
	return bool 
	author:WanQingsong
	date: 2012-04-09
*/  
bool CAlarmRecrod_ARD17::AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime)
{
	string alarmData;
	S_AlmDatas *pCtxAlmDatas =  CONTEXT.GetAlarmDatas();
	string CtxAlmData;

	CAlarmRecrod_ARD1::AppendStateTime(alarmData, state, alarmTime);
	
	/* ����������*/
	if (pCtxAlmDatas->Load(alarmFn,meter.m_MeasureNo,CtxAlmData) ==-1)
	{
		loget<<"CONTEXT �ڴ�澯���ݼ���ʧ��!"<<endl;
		return false;
	}

	alarmData.append(CtxAlmData);

	/* wqs debug*/
	logvt<<"CAlarmRecrod_ARD17: "<<HEX<<alarmData<<endl;
	/* end */


	/*�澯���ݱ���*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
		
}

void CAlarmRecrod_ARD18::Init()
{

}

/*
	brief: save alram data to alarm file ,as a record.
	param [in] CMeter meter:the class of meter operation
			   INT32U alarmFn:alarm fn id
			   T_emAlarmStatus state:alarm state
			   S_TIME alarmTime: alarm record saved time
	return bool 
	author:WanQingsong
	date: 2012-04-09
*/  
bool CAlarmRecrod_ARD18::AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime)
{
	string alarmData;
	S_RegDatas CtxRegData;

	CAlarmRecrod_ARD1::AppendStateTime(alarmData, state, alarmTime);

	if (CAlarmEvtCommIdData::GetCommRegDataFromCTX(meter.m_MeasureNo,alarmFn,CtxRegData) == false)
	{
		return false;
	}
	
	alarmData.append(CtxRegData[0].m_data);

	
	/* wqs debug*/
	logvt<<"CAlarmRecrod_ARD18: "<<HEX<<alarmData<<endl;
	/* end */
	
	/*�澯���ݱ���*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
		
}

vector<INT32U> CAlarmRecrod_ARD19::m_gatherFnIDs;
void CAlarmRecrod_ARD19::Init()
{
	m_gatherFnIDs.clear();

	m_gatherFnIDs.push_back(0x00010000);//�����й��ܵ���
	m_gatherFnIDs.push_back(0x00020000);//�����й��ܵ���
	m_gatherFnIDs.push_back(0x00030000);//����޹�1�ܵ���
	m_gatherFnIDs.push_back(0x00040000);//����޹�2�ܵ���
}
static CAlarmRecrod_ARD19 s_AlarmRecrod_ARD19;
/*
	brief: save alram data to alarm file ,as a record.
	param [in] CMeter meter:the class of meter operation
			   INT32U alarmFn:alarm fn id
			   T_emAlarmStatus state:alarm state
			   S_TIME alarmTime: alarm record saved time
	return bool 
	author:WanQingsong
	date: 2012-04-09
*/  
bool CAlarmRecrod_ARD19::AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime)
{
	string alarmData;
	string outData;

	CAlarmRecrod_ARD1::AppendStateTime(alarmData, state, alarmTime);

	for (size_t i=0; i<m_gatherFnIDs.size(); i++)
	{
		outData.clear();
//		meter.ReadMemoryFnData(CONTEXT.GetGatherFns()->FindFn(m_gatherFnIDs[i]),outData, 0);
		alarmData.append(outData);	
	}

	/*�澯���ݱ���*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
}
/*
	brief: save alram data to alarm file ,as a record.
	param [in] CMeter meter:the class of meter operation
			   INT32U alarmFn:alarm fn id
			   T_emAlarmStatus state:alarm state
			   S_TIME alarmTime: alarm record saved time
	return bool 
	author:WanQingsong
	date: 2012-04-09
*/  
bool CAlarmRecrod_ARD21::AlarmRecord(INT32U alarmFn,S_FBS *pFBS,T_emAlarmStatus state,S_TIME alarmTime)
{
	string alarmData;

	CAlarmRecrod_ARD1::AppendStateTime(alarmData, state, alarmTime);
	alarmData.append((char*)pFBS->GetData(), 256);

	/*�澯���ݱ���*/
	C_AlmEvt::Write(0, alarmFn, alarmData);
	return true;
}


/*********************************************************/
/**               �¼����ݱ�����						 */	
/**														 */ 	
/*********************************************************/
bool CEventRecrod_ERD1::AppendTime(string &strAlmData, S_TIME alarmTime)
{
	char buffer[7];

	/* �澯ʱ�� */
	C_TIME::Time2BCDStr(alarmTime, buffer);
	strAlmData.append(buffer, sizeof(buffer)-1);

	return true;
}

/*
	brief: save terminal program event record
	param [in] CMeter meter:the class of meter operation
			   INT32U eventFn:event fn id
			   S_TIME progTime:the terminal program time 
			   T_emChannelType comType: the channel type
			   int progDataNum: the program data id number 
			   SProgDataArrays: the program data arrays
	return bool 
	author:WanQingsong
	date: 2012-04-09
*/  
bool CEventRecrod_ERD1::EventRecord(S_MeterInfo& meter,INT32U eventFn,S_TIME progTime,T_emChannelType comType,INT8U progDataNum,SProgDataArrays progDataArrays)
{
	string eventData;

	/* ��̷���ʱ��*/
	CEventRecrod_ERD1::AppendTime(eventData, progTime);

    /* ͨ������ */
	eventData.push_back(char(comType));

	/* ������������� */
	eventData.push_back(char(progDataNum));

	/* ������������� */
	for (size_t i=0; i<progDataArrays.size(); i++)
	{
		/* ���������ID */
		for (int k=0; k<4; k++)
		{
			eventData.push_back(char((progDataArrays[i].m_dataId>>i*8)&0xff));
		}
		eventData.append(progDataArrays[i].m_data);	
	}

	/*�澯�¼����ݱ���*/
	C_AlmEvt::Write(meter.m_MeasureNo, eventFn, eventData);
	return true;
}

/*
	brief: save terminal program event record
	param [in] CMeter meter:the class of meter operation
			   INT32U eventFn:event fn id
			   S_TIME cleanUpTime:the max demand value clean time
	return bool 
	author:WanQingsong
	date: 2012-04-09
*/  
bool CEventRecrod_ERD2::EventRecord(S_MeterInfo& meter,INT32U eventFn,S_TIME cleanUpTime)
{
	string eventData;

	/* ��̷���ʱ��*/
	CEventRecrod_ERD1::AppendTime(eventData, cleanUpTime);

	/*�澯�¼����ݱ���*/
	C_AlmEvt::Write(meter.m_MeasureNo, eventFn, eventData);
	return true;
}

/*
	brief: save terminal power On/OFF event record
	param [in] CMeter meter:the class of meter operation
			   INT32U eventFn:event fn id
			   S_TIME cleanUpTime:the max demand value clean time
	return bool 
	author:WanQingsong
	date: 2012-04-09
*/  
bool CEventRecrod_ERD4::EventRecord(INT32U eventFn,S_TIME powerOffTime,S_TIME powerOnsTime)
{
	string eventData;

	/* ͣ��ʱ��*/
	CEventRecrod_ERD1::AppendTime(eventData, powerOffTime);

	/* ����ʱ��*/
	CEventRecrod_ERD1::AppendTime(eventData, powerOnsTime);

	/* wqs debug*/
	logvt<<"CEventRecrod_ERD4: "<<HEX<<eventData<<endl;
	/* end */
	
	/*�澯�¼����ݱ���*/
	C_AlmEvt::Write(0, eventFn, eventData);
	return true;
}

/*
	brief: save terminal program event record
	param [in] CMeter meter:the class of meter operation
			   INT32U eventFn:event fn id
			   S_TIME cleanUpTime:the max demand value clean time
	return bool 
	author:WanQingsong
	date: 2013-10-09
*/  
bool CEventRecrod_ERD6::EventRecord(S_MeterInfo& meter,INT32U eventFn,T_emControlType ctlType,string energyYG,S_TIME eventTime)
{
	string eventData;

	/* �澯����ʱ��YYMMDDhhmmss*/
	CEventRecrod_ERD1::AppendTime(eventData, eventTime);

	eventData.push_back((INT8U)ctlType);
	eventData.append(energyYG);

	/* wqs debug*/
	logvt<<"CEventRecrod_ERD6: "<<HEX<<eventData<<endl;
	/* end */
	
	/*�澯�¼����ݱ���*/
	C_AlmEvt::Write(meter.m_MeasureNo, eventFn, eventData);
	return true;
}

bool CEventRecrod_ERD8::EventRecord(INT32U eventFn, S_TIME eventTime, const string &strParamChange)
{
	string eventData;

	/* �澯����ʱ��YYMMDDhhmmss*/
	CEventRecrod_ERD1::AppendTime(eventData, eventTime);

	/* ����DI */
	eventData.append(strParamChange);

	/* wqs debug*/
	logvt<<"CEventRecrod_ERD8: "<<HEX<<eventData<<endl;
	/* end */
	
	/*�澯�¼����ݱ���*/
	C_AlmEvt::Write(0, eventFn, eventData);
	return true;
}


