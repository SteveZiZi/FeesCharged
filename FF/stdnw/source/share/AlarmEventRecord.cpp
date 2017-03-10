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
	/*告警内容保存*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
}

bool CAlarmRecrod_ARD1::AppendStateTime(string &strAlmData, T_emAlarmStatus state, S_TIME alarmTime)
{
	char buffer[7];

	/* 告警状态 */
	strAlmData.push_back((char)state);				

	/* 告警时间 */
	C_TIME::Time2BCDStr(alarmTime, buffer);
	strAlmData.append(buffer, sizeof(buffer)-1);

	return true;
}

vector<INT32U> CAlarmRecrod_ARD2::m_gatherFnIDs;
void CAlarmRecrod_ARD2::Init()
{
	//正向有功总电能
	m_gatherFnIDs.push_back(0x60700000);
	//反向有功总电能
	m_gatherFnIDs.push_back(0x60D00000);
	//组合无功1总电能
	m_gatherFnIDs.push_back(0x91100000);
	//组合无功2总电能
	m_gatherFnIDs.push_back(0x91200000);
	//三相电压
	m_gatherFnIDs.push_back(0xB6110000);
	m_gatherFnIDs.push_back(0xB6120000);
	m_gatherFnIDs.push_back(0xB6130000);
	m_gatherFnIDs.push_back(0x71001001);
	m_gatherFnIDs.push_back(0x71001002);
	m_gatherFnIDs.push_back(0x71001003);
	m_gatherFnIDs.push_back(0x71001004);
	m_gatherFnIDs.push_back(0x71001005);
	m_gatherFnIDs.push_back(0x71001006);
	
	//三相电流
	m_gatherFnIDs.push_back(0xB6210000);
	m_gatherFnIDs.push_back(0xB6220000);
	m_gatherFnIDs.push_back(0xB6230000);
	m_gatherFnIDs.push_back(0x71002001);
	m_gatherFnIDs.push_back(0x71002002);
	m_gatherFnIDs.push_back(0x71002003);
	//有功功率
	m_gatherFnIDs.push_back(0xB6300000);
	m_gatherFnIDs.push_back(0xB6310000);
	m_gatherFnIDs.push_back(0xB6320000);
	m_gatherFnIDs.push_back(0xB6330000);	
	//无功功率
	m_gatherFnIDs.push_back(0xB6400000);
	m_gatherFnIDs.push_back(0xB6410000);
	m_gatherFnIDs.push_back(0xB6420000);
	m_gatherFnIDs.push_back(0xB6430000);	
	//功率因数
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
	{// 终端告警
		InitTermData(CtxRegData);
	}
	else if (CAlarmEvtCommIdData::GetCommRegDataFromCTX(meter.m_MeasureNo,REGS_ARD2,CtxRegData) == false)
	{
		loget<<"CAlarmRecrod_ARD2:: GetCommRegDataFromCTX 失败"<<endl;
		if (CAlarmEvtCommIdData::GetCommRegDataFromSTS(meter.m_MeasureNo,REGS_ARD2,CtxRegData) == false)
		{
			loget<<"CAlarmRecrod_ARD2:: GetCommRegDataFromSTS 失败"<<endl;	
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


	/*告警内容保存*/
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
			logwt<<"通用寄存器["<<HEX<<m_gatherFnIDs[i]<<"]没有配置"<<endl;

		RegData.m_regid = CommReg.m_ID;
		CommReg.FillData(RegData.m_data);
		CtxRegData.push_back(RegData);
	}
	return true;
}


vector<INT32U> CAlarmRecrod_ARD3::m_gatherFnIDs;
	
void CAlarmRecrod_ARD3::Init()
{
	//正向有功总电能
	m_gatherFnIDs.push_back(0x60700000);
	//反向有功总电能
	m_gatherFnIDs.push_back(0x60D00000);
	//组合无功1总电能
	m_gatherFnIDs.push_back(0x91100000);
	//组合无功2总电能
	m_gatherFnIDs.push_back(0x91200000);
	//三相电压
	m_gatherFnIDs.push_back(0xB6110000);
	m_gatherFnIDs.push_back(0xB6120000);
	m_gatherFnIDs.push_back(0xB6130000);
	m_gatherFnIDs.push_back(0x71001001);
	m_gatherFnIDs.push_back(0x71001002);
	m_gatherFnIDs.push_back(0x71001003);
	m_gatherFnIDs.push_back(0x71001004);
	m_gatherFnIDs.push_back(0x71001005);
	m_gatherFnIDs.push_back(0x71001006);
	//三相电流
	m_gatherFnIDs.push_back(0xB6210000);
	m_gatherFnIDs.push_back(0xB6220000);
	m_gatherFnIDs.push_back(0xB6230000);
	m_gatherFnIDs.push_back(0x71002001);
	m_gatherFnIDs.push_back(0x71002002);
	m_gatherFnIDs.push_back(0x71002003);
	//有功功率
	m_gatherFnIDs.push_back(0xB6300000);
	m_gatherFnIDs.push_back(0xB6310000);
	m_gatherFnIDs.push_back(0xB6320000);
	m_gatherFnIDs.push_back(0xB6330000);	
	//无功功率
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

	/* 发生前数据*/
	if (CAlarmEvtCommIdData::GetCommRegDataFromSTS(meter.m_MeasureNo,alarmFn,StxRegData) == false)
	{
		loget<<"CAlarmRecrod_ARD3 GetCommRegDataFromSTS 失败"<<endl;
		return false;
	}

	/* 发生后数据*/
	if (CAlarmEvtCommIdData::GetCommRegDataFromCTX(meter.m_MeasureNo,REGS_ARD2,CtxRegData) == false)
	{
		loget<<"CAlarmRecrod_ARD3 GetCommRegDataFromCTX 失败"<<endl;
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
	/*告警内容保存*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
}

vector<INT32U> CAlarmRecrod_ARD4::m_gatherFnIDs;
void CAlarmRecrod_ARD4::Init()
{
	m_gatherFnIDs.clear();

	m_gatherFnIDs.push_back(0x60700000);//正向有功总电能
	m_gatherFnIDs.push_back(0x60D00000);//反向有功总电能
	m_gatherFnIDs.push_back(0x91100000);//组合无功1总电能
	m_gatherFnIDs.push_back(0x91200000);//组合无功2总电能
	m_gatherFnIDs.push_back(0x71003003);//剩余电量
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

	/* 发生后数据*/
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
	/*告警内容保存*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
}

vector<INT32U> CAlarmRecrod_ARD5::m_BeforeGatherFnIDs;
vector<INT32U> CAlarmRecrod_ARD5::m_AfterGatherFnIDs;

void CAlarmRecrod_ARD5::Init()
{
	m_BeforeGatherFnIDs.clear();
	m_AfterGatherFnIDs.clear();
	
	/* 告警前采集的数据项 */
	m_BeforeGatherFnIDs.push_back(0x00010000);//正向有功总电能
	m_BeforeGatherFnIDs.push_back(0x00020000);//反向有功总电能
	m_BeforeGatherFnIDs.push_back(0x00030000);//组合无功1总电能
	m_BeforeGatherFnIDs.push_back(0x00040000);//组合无功2总电能
	m_BeforeGatherFnIDs.push_back(0xE080000D);//PT变比
	m_BeforeGatherFnIDs.push_back(0xE080000C);//CT变比

	/* 告警后采集的数据项 */
	m_AfterGatherFnIDs.push_back(0xE080000D);//PT变比
	m_AfterGatherFnIDs.push_back(0xE080000C);//CT变比
	
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

	/* 发生前数据*/
	for (size_t i=0; i<m_BeforeGatherFnIDs.size(); i++)
	{
		outData.clear();
//		meter.ReadDataFileFnData(CONTEXT.GetGatherFns()->FindFn(m_BeforeGatherFnIDs[i]),outData, 0xffff);
		alarmData.append(outData);
	}

	/* 发生后数据*/
		
	for (size_t i=0; i<m_AfterGatherFnIDs.size(); i++)
	{
		outData.clear();
//		meter.ReadFnData(CONTEXT.GetGatherFns()->FindFn(m_AfterGatherFnIDs[i]), outData);
		alarmData.append(outData);
	}
	/*告警内容保存*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
}

vector<INT32U> CAlarmRecrod_ARD6::m_BeforeGatherFnIDs;
vector<INT32U> CAlarmRecrod_ARD6::m_AfterGatherFnIDs;

void CAlarmRecrod_ARD6::Init()
{
	m_BeforeGatherFnIDs.clear();
	m_AfterGatherFnIDs.clear();
	
	/* 告警前采集的数据项 */
	m_BeforeGatherFnIDs.push_back(0x00010000);//正向有功总电能
	m_BeforeGatherFnIDs.push_back(0x00020000);//反向有功总电能
	m_BeforeGatherFnIDs.push_back(0x00030000);//组合无功1总电能
	m_BeforeGatherFnIDs.push_back(0x00040000);//组合无功2总电能
	m_BeforeGatherFnIDs.push_back(0xE1000003);//电表常数(有功)
	m_BeforeGatherFnIDs.push_back(0xE1000004);//电表常数(无功)

	/* 告警后采集的数据项 */
	m_AfterGatherFnIDs.push_back(0xE1000003);//电表常数(有功)
	m_AfterGatherFnIDs.push_back(0xE1000004);//电表常数(无功)
	
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

	/* 发生前数据*/
	for (size_t i=0; i<m_BeforeGatherFnIDs.size(); i++)
	{
		outData.clear();
//		meter.ReadDataFileFnData(CONTEXT.GetGatherFns()->FindFn(m_BeforeGatherFnIDs[i]),outData, 0xffff);
		alarmData.append(outData);
	}

	/* 发生后数据*/
		
	for (size_t i=0; i<m_AfterGatherFnIDs.size(); i++)
	{
		outData.clear();
//		meter.ReadFnData(CONTEXT.GetGatherFns()->FindFn(m_AfterGatherFnIDs[i]), outData);
		alarmData.append(outData);
	}
	/*告警内容保存*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
}

vector<INT32U> CAlarmRecrod_ARD7::m_BeforeGatherFnIDs;
void CAlarmRecrod_ARD7::Init()
{
	m_BeforeGatherFnIDs.clear();
	
	/* 告警前采集的数据项 */
	m_BeforeGatherFnIDs.push_back(0x00010000);//正向有功总电能
	m_BeforeGatherFnIDs.push_back(0x00020000);//反向有功总电能
	m_BeforeGatherFnIDs.push_back(0x00030000);//组合无功1总电能
	m_BeforeGatherFnIDs.push_back(0x00040000);//组合无功2总电能
	m_BeforeGatherFnIDs.push_back(0x01010000);//正向有功总最大需量及发生时间
	m_BeforeGatherFnIDs.push_back(0x01020000);//反向有功总最大需量及发生时间
	m_BeforeGatherFnIDs.push_back(0x01030000);//组合无功1总最大需量及发生时间
	m_BeforeGatherFnIDs.push_back(0x01040000);//组合无功2总最大需量及发生时间
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

	/* 发生前数据*/
	for (size_t i=0; i<m_BeforeGatherFnIDs.size(); i++)
	{
		outData.clear();
//		meter.ReadDataFileFnData(CONTEXT.GetGatherFns()->FindFn(m_BeforeGatherFnIDs[i]),outData, 0xffff);
		alarmData.append(outData);
	}

	/*告警内容保存*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
}

vector<INT32U> CAlarmRecrod_ARD8::m_BeforeGatherFnIDs;
vector<INT32U> CAlarmRecrod_ARD8::m_AfterGatherFnIDs;
void CAlarmRecrod_ARD8::Init()
{
	m_BeforeGatherFnIDs.clear();
	m_AfterGatherFnIDs.clear();
	
	/* 告警前采集的数据项 */
	m_BeforeGatherFnIDs.push_back(0x040005FF);//电表运行状态字数据块

	/* 告警后采集的数据项 */
	m_AfterGatherFnIDs.push_back(0x040005FF);//电表运行状态字数据块
	
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

	/* 发生前数据*/
	outData.clear();
//	memcpy(buf,&(meter.m_pMeterStatus->m_MeterStatusWords),14);
	alarmData.append(buf,14);

	/* 发生后数据*/
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
			loget<<"电表状态字块读取失败"<<endl;
			return false;
		}
	}
	/*告警内容保存*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
}

vector<INT32U> CAlarmRecrod_ARD9::m_BeforeGatherFnIDs;
vector<INT32U> CAlarmRecrod_ARD9::m_AfterGatherFnIDs;

void CAlarmRecrod_ARD9::Init()
{
	m_BeforeGatherFnIDs.clear();
	m_AfterGatherFnIDs.clear();
	
	/* 告警前采集的数据项 */
	m_BeforeGatherFnIDs.push_back(0xE0000C1F);//输入开关状态数据块

	/* 告警后采集的数据项 */
	m_AfterGatherFnIDs.push_back(0xE0000C1F);//输入开关状态数据块
	
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

	/* 发生前数据*/
	for (size_t i=0; i<m_BeforeGatherFnIDs.size(); i++)
	{
		outData.clear();
//		meter.ReadDataFileFnData(CONTEXT.GetGatherFns()->FindFn(m_BeforeGatherFnIDs[i]),outData, 0xffff);
		alarmData.append(outData);
	}

	/* 发生后数据*/
		
	for (size_t i=0; i<m_AfterGatherFnIDs.size(); i++)
	{
		outData.clear();
//		meter.ReadFnData(CONTEXT.GetGatherFns()->FindFn(m_AfterGatherFnIDs[i]), outData);
		alarmData.append(outData);
	}
	/*告警内容保存*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
}

vector<INT32U> CAlarmRecrod_ARD10::m_gatherFnIDs;
void CAlarmRecrod_ARD10::Init()
{
	m_gatherFnIDs.clear();
	m_gatherFnIDs.push_back(0xE1800013);//月通信流量
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

	/*告警内容保存*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
		
}

vector<INT32U> CAlarmRecrod_ARD11::m_gatherFnIDs;
void CAlarmRecrod_ARD11::Init()
{
	m_gatherFnIDs.clear();
	m_gatherFnIDs.push_back(0xE1800010);//日线损率
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

	/*告警内容保存*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
}

vector<INT32U> CAlarmRecrod_ARD12::m_gatherFnIDs;
void CAlarmRecrod_ARD12::Init()
{
	m_gatherFnIDs.push_back(0x04000503);//电表运行状态字3
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

	/* 发生后数据*/
	if (CAlarmEvtCommIdData::GetCommRegDataFromCTX(meter.m_MeasureNo,alarmFn,CtxRegData) == false)
	{
		return false;
	}

	alarmData.append(CtxRegData[0].m_data);

	/* wqs debug*/
	logvt<<"CAlarmRecrod_ARD12: "<<HEX<<alarmData<<endl;
	/* end */
	
	/*告警内容保存*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
		
}

bool CAlarmRecrod_ARD12::AlarmRecord(S_MeterInfo& meter,INT32U alarmFn,T_emAlarmStatus state,S_TIME alarmTime, string meterStateChar3)
{
	string alarmData;

	CAlarmRecrod_ARD1::AppendStateTime(alarmData, state, alarmTime);

	/* 发生后数据*/
	alarmData.append(meterStateChar3);

	/* wqs debug*/
	logvt<<"CAlarmRecrod_ARD12: "<<HEX<<alarmData<<endl;
	/* end */
	
	/*告警内容保存*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
		
}


vector<INT32U> CAlarmRecrod_ARD13::m_gatherFnIDs;

void CAlarmRecrod_ARD13::Init()
{
	m_gatherFnIDs.clear();
	m_gatherFnIDs.push_back(0x04000102);//电能表时间
	m_gatherFnIDs.push_back(0x04000101);//电能表日期
//	m_gatherFnIDs.push_back(0xE0000130);//终端时间
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

	/* 发生后数据*/
	if (CAlarmEvtCommIdData::GetCommRegDataFromCTX(meter.m_MeasureNo,alarmFn,CtxRegData) == false)
	{
		return false;
	}

	/* 电表日期 YYMMDDWW*/
	char buf[4]={0};
	memcpy(buf,CtxRegData[1].m_data.data(),sizeof(buf));
//	reverse(buf,buf+4);
	alarmData.append(buf,4);	
	
	/* 电表时间 HHMMSS*/
	char buff[3]={0};
	memcpy(buff,CtxRegData[0].m_data.data(),sizeof(buff));
//	reverse(buff,buff+3);
	alarmData.append(buff,3);

	/* 终端时间:YYMMDDHHMMSS*/
	char time[7] = {0};
	C_TIME::Time2BCDStr(alarmTime, time);
	alarmData.append(time, 6);

	/* wqs debug*/
	logvt<<"CAlarmRecrod_ARD13: "<<HEX<<alarmData<<endl;
	/* end */
	
	/*告警内容保存*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
		
}

vector<INT32U> CAlarmRecrod_ARD14::m_gatherFnIDs;

void CAlarmRecrod_ARD14::Init()
{
	m_gatherFnIDs.clear();

	m_gatherFnIDs.push_back(0x020A0100);//A相电压总谐波含有量
	m_gatherFnIDs.push_back(0x020A0200);//B相电压总谐波含有量
	m_gatherFnIDs.push_back(0x020A0300);//C相电压总谐波含有量
	m_gatherFnIDs.push_back(0x020B0100);//A相电流总谐波含有量
	m_gatherFnIDs.push_back(0x020B0200);//B相电流总谐波含有量
	m_gatherFnIDs.push_back(0x020B0300);//C相电流总谐波含有量

	m_gatherFnIDs.push_back(0xE1000101);//A相电压偏差最大值
	m_gatherFnIDs.push_back(0xE1000102);//B相电压偏差最大值
	m_gatherFnIDs.push_back(0xE1000103);//C相电压偏差最大值
	m_gatherFnIDs.push_back(0xE1000104);//频率偏差最大值
	m_gatherFnIDs.push_back(0xE1000105);//电压不平衡度最大值
	m_gatherFnIDs.push_back(0xE100010D);//长时闪变最大值
	m_gatherFnIDs.push_back(0xE100010E);//短时闪变最大值
	m_gatherFnIDs.push_back(0xE100010F);//电压总畸变最大值

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

	/*告警内容保存*/
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
	
	/* 发生后数据*/
	if (pCtxAlmDatas->Load(alarmFn,meter.m_MeasureNo,CtxAlmData) ==-1)
	{
		loget<<"CONTEXT 内存告警数据加载失败!"<<endl;
		return false;
	}

	alarmData.append(CtxAlmData);

	/* wqs debug*/
	logvt<<"CAlarmRecrod_ARD15: "<<HEX<<alarmData<<endl;
	/* end */

	/*告警内容保存*/
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
	
	/* 发生后数据*/
	if (pCtxAlmDatas->Load(alarmFn,meter.m_MeasureNo,CtxAlmData) ==-1)
	{
		loget<<"CONTEXT 内存告警数据加载失败!"<<endl;
		return false;
	}

	alarmData.append(CtxAlmData);

	/* wqs debug*/
	logvt<<"CAlarmRecrod_ARD16: "<<HEX<<alarmData<<endl;
	/* end */

	/*告警内容保存*/
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
	
	/* 发生后数据*/
	if (pCtxAlmDatas->Load(alarmFn,meter.m_MeasureNo,CtxAlmData) ==-1)
	{
		loget<<"CONTEXT 内存告警数据加载失败!"<<endl;
		return false;
	}

	alarmData.append(CtxAlmData);

	/* wqs debug*/
	logvt<<"CAlarmRecrod_ARD17: "<<HEX<<alarmData<<endl;
	/* end */


	/*告警内容保存*/
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
	
	/*告警内容保存*/
	C_AlmEvt::Write(meter.m_MeasureNo, alarmFn, alarmData);
	return true;
		
}

vector<INT32U> CAlarmRecrod_ARD19::m_gatherFnIDs;
void CAlarmRecrod_ARD19::Init()
{
	m_gatherFnIDs.clear();

	m_gatherFnIDs.push_back(0x00010000);//正向有功总电能
	m_gatherFnIDs.push_back(0x00020000);//反向有功总电能
	m_gatherFnIDs.push_back(0x00030000);//组合无功1总电能
	m_gatherFnIDs.push_back(0x00040000);//组合无功2总电能
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

	/*告警内容保存*/
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

	/*告警内容保存*/
	C_AlmEvt::Write(0, alarmFn, alarmData);
	return true;
}


/*********************************************************/
/**               事件内容保存类						 */	
/**														 */ 	
/*********************************************************/
bool CEventRecrod_ERD1::AppendTime(string &strAlmData, S_TIME alarmTime)
{
	char buffer[7];

	/* 告警时间 */
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

	/* 编程发生时间*/
	CEventRecrod_ERD1::AppendTime(eventData, progTime);

    /* 通道类型 */
	eventData.push_back(char(comType));

	/* 编程数据项组数 */
	eventData.push_back(char(progDataNum));

	/* 编程数据项内容 */
	for (size_t i=0; i<progDataArrays.size(); i++)
	{
		/* 存放数据项ID */
		for (int k=0; k<4; k++)
		{
			eventData.push_back(char((progDataArrays[i].m_dataId>>i*8)&0xff));
		}
		eventData.append(progDataArrays[i].m_data);	
	}

	/*告警事件内容保存*/
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

	/* 编程发生时间*/
	CEventRecrod_ERD1::AppendTime(eventData, cleanUpTime);

	/*告警事件内容保存*/
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

	/* 停电时间*/
	CEventRecrod_ERD1::AppendTime(eventData, powerOffTime);

	/* 复电时间*/
	CEventRecrod_ERD1::AppendTime(eventData, powerOnsTime);

	/* wqs debug*/
	logvt<<"CEventRecrod_ERD4: "<<HEX<<eventData<<endl;
	/* end */
	
	/*告警事件内容保存*/
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

	/* 告警发生时间YYMMDDhhmmss*/
	CEventRecrod_ERD1::AppendTime(eventData, eventTime);

	eventData.push_back((INT8U)ctlType);
	eventData.append(energyYG);

	/* wqs debug*/
	logvt<<"CEventRecrod_ERD6: "<<HEX<<eventData<<endl;
	/* end */
	
	/*告警事件内容保存*/
	C_AlmEvt::Write(meter.m_MeasureNo, eventFn, eventData);
	return true;
}

bool CEventRecrod_ERD8::EventRecord(INT32U eventFn, S_TIME eventTime, const string &strParamChange)
{
	string eventData;

	/* 告警发生时间YYMMDDhhmmss*/
	CEventRecrod_ERD1::AppendTime(eventData, eventTime);

	/* 数据DI */
	eventData.append(strParamChange);

	/* wqs debug*/
	logvt<<"CEventRecrod_ERD8: "<<HEX<<eventData<<endl;
	/* end */
	
	/*告警事件内容保存*/
	C_AlmEvt::Write(0, eventFn, eventData);
	return true;
}


