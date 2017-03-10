#include "almevtbase.h"
#include "datafile.h"
#include "htrace.h"
#include "sysbase.h"
#include "status.h"
#include "context.h"
#include "simpdata.h"


/*void S_MpAlarmState::InitAlarmState(INT8U index, INT8U alarmId,INT8U state)
{
	m_AlarmStates[index].m_AlarmId = alarmId;
	m_AlarmStates[index].m_State = state;
}*/
bool S_MpAlarmState::SetAlarmState(INT8U alarmId,INT8U state)
{
	/* 找到是否目前有此告警的记录*/
	for(int i=0; i<CONS_ALARM_TYPS_COUNT_MAX; i++)
	{
		if (m_AlarmStates[i].m_AlarmId == alarmId)
		{
			m_AlarmStates[i].m_State = state;
			return true;
		}
	}
	/* 未找到此告警记录，在新的空闲区域记录*/
	for(int i=0; i<CONS_ALARM_TYPS_COUNT_MAX; i++)
	{
		if (m_AlarmStates[i].m_AlarmId == 0x00)
		{
			m_AlarmStates[i].m_AlarmId = alarmId;
			m_AlarmStates[i].m_State = state;
			return true;
		}
	}	
	return false;
	
}
T_emAlarmStatus S_MpAlarmState::GetAlarmState(INT8U alarmId)
{
	for(int i=0; i<CONS_ALARM_TYPS_COUNT_MAX; i++)
	{
		if (m_AlarmStates[i].m_AlarmId == alarmId)
		{
			return (T_emAlarmStatus)m_AlarmStates[i].m_State;
		}
	}
	return ALARMSTATE_INVALID;
}

//
//告警状态
//
void S_AlarmStatus::Init(void)
{
	m_tickImportantHappen = 0;
	m_ImportantNewCnt = 0;
	m_NormalNewCnt = 0;
	m_EventNewCnt = 0;

	m_bsNewAlarmID.reset();
	memset(m_MpAlarmStates, 0x00, sizeof(m_MpAlarmStates));
}
void S_AlarmStatus::Flush(void)
{
	m_bsNewAlarmID.reset();

	m_ImportantNewCnt = C_AlmEvt::CountNew(ALM_TYPE_IMPORTANT);
	m_NormalNewCnt = C_AlmEvt::CountNew(ALM_TYPE_NORMAL);
	m_EventNewCnt = C_AlmEvt::CountNew(ALM_TYPE_EVENT);
	if (m_ImportantNewCnt < 0) m_ImportantNewCnt = 0;
	if (m_NormalNewCnt < 0) m_NormalNewCnt = 0;
	if (m_EventNewCnt < 0) m_EventNewCnt = 0;
}

//
//告警数据
//
bool S_AlmFlags::TestTask(S_BYTE ERC)
{
	if (ERC >= ALM_FLAGS_ITEM_MAX)
		return false;
	if (m_itemAlm[ERC].m_fTaskFinish == false)
		return false;

	m_itemAlm[ERC].m_fTaskFinish = false;
	m_Current.m_mpbsSuccess = m_itemAlm[ERC].m_mpbsSuccess;
	m_Current.m_fTaskFinish = true;
	m_itemAlm[ERC].m_mpbsSuccess.reset();
	return true;
}

bool S_AlmFlags::ResetTask(S_BYTE ERC)
{
	if (ERC >= ALM_FLAGS_ITEM_MAX)
		return false;

	m_Current.m_fTaskFinish = false;
	m_Current.m_mpbsSuccess.reset();
	return true;
}

bool S_AlmFlags::TestMeter(S_BYTE ERC, S_WORD MP)
{
	if (ERC >= ALM_FLAGS_ITEM_MAX)
		return false;
	if (m_Current.m_fTaskFinish == false)
		return false;
	if (MP >= CONS_METER_COUNT_MAX)
		return false;
	return m_Current.m_mpbsSuccess.test(MP);
}

bool S_AlmFlags::SetTask(S_BYTE ERC, const S_MPBS &mpbsSuccess)
{
	if (ERC >= ALM_FLAGS_ITEM_MAX)
		return false;
	m_itemAlm[ERC].m_mpbsSuccess |= mpbsSuccess; // 不同进程的告警任务将采集成功的标志加入
	m_itemAlm[ERC].m_fTaskFinish = true;
	return true;
}

//
int S_AlmDatas::Load(S_DWORD ERC, S_WORD MP, string &strAlmData)
{
	INT8U index = (INT8U)(ERC&0xff);
	
	if (MP >= CONS_METER_COUNT_MAX)
		return -1;

	if ((ERC&0xffff0000) == 0xE2000000)//告警
	{
		switch (index)
		{
			case 0:return   _Load(m_ERC00[MP], strAlmData);
			case 13:return _Load(m_ERC13[MP], strAlmData);
			case 14:return _Load(m_ERC14[MP], strAlmData);
			case 15:return _Load(m_ERC15[MP], strAlmData);
			case 19:return _Load(m_ERC19[MP], strAlmData);
			case 20:return _Load(m_ERC20[MP], strAlmData);
			case 21:return _Load(m_ERC21[MP], strAlmData);
			case 22:return _Load(m_ERC22[MP], strAlmData);
			case 23:return _Load(m_ERC23[MP], strAlmData);
			case 24:return _Load(m_ERC24[MP], strAlmData);
			case 43:return _Load(m_ERC43[MP], strAlmData);
			case 44:return _Load(m_ERC44[MP], strAlmData);
			case 46:return _Load(m_ERC46[MP], strAlmData);
			case 53:return _Load(m_ERC53[MP], strAlmData);
			case 54:return _Load(m_ERC54[MP], strAlmData);
			case 59:return _Load(m_ERC59[MP], strAlmData);
			case 61:return _Load(m_ERC61[MP], strAlmData);
			case 62:return _Load(m_ERC62[MP], strAlmData);					
			case 64:return _Load(m_ERC64[MP], strAlmData);
			case 65:return _Load(m_ERC65[MP], strAlmData);
			case 66:return _Load(m_ERC66[MP], strAlmData);
			case 67:return _Load(m_ERC67[MP], strAlmData);		
			default:break;
		}
	}

	if ((ERC&0xffff0000) == 0xE2010000)//事件
	{

	}
	
	return -1;
}

int S_AlmDatas::Save(S_DWORD ERC, S_WORD MP, const string &strAlmData)
{
	INT8U index = (INT8U)(ERC&0xff);
	C_AlmPhaseData almPhaseData;
	
	if (MP >= CONS_METER_COUNT_MAX)
		return -1;
	if (strAlmData.size() > ALARM_REGISTER_DATA_LEN_MAX)
	{
		logwt<<"请加大告警数据宏ALARM_REGISTER_DATA_LEN_MAX"<<endl;
		return -1;
	}

	/* wqs debug*/
	logvt<<"S_AlmDatas::Save [ERC="<<index<<"],MP="<<MP<<",strAlmData:"<<HEX<<strAlmData<<endl;
	/* end */

	if ((ERC&0xffff0000) == 0xE2000000)//告警
	{
		switch (index)
		{
			case 0:
					_Save(m_ERC00[0], strAlmData);//ARD2终端测量点也要保存一份，给终端停上电告警用，保存到内存
					almPhaseData.Save(ERC, 0, strAlmData);////ARD2终端测量点也要保存一份，给终端停上电告警用，保存到文件
					return   _Save(m_ERC00[MP], strAlmData);
			case 13:return _Save(m_ERC13[MP], strAlmData);
			case 14:return _Save(m_ERC14[MP], strAlmData);
			case 15:return _Save(m_ERC15[MP], strAlmData);
			case 19:return _Save(m_ERC19[MP], strAlmData);
			case 20:return _Save(m_ERC20[MP], strAlmData);
			case 21:return _Save(m_ERC21[MP], strAlmData);
			case 22:return _Save(m_ERC22[MP], strAlmData);
			case 23:return _Save(m_ERC23[MP], strAlmData);
			case 24:return _Save(m_ERC24[MP], strAlmData);
			case 43:return _Save(m_ERC43[MP], strAlmData);
			case 44:return _Save(m_ERC44[MP], strAlmData);
			case 46:return _Save(m_ERC46[MP], strAlmData);
			case 53:return _Save(m_ERC53[MP], strAlmData);
			case 54:return _Save(m_ERC54[MP], strAlmData);
			case 59:return _Save(m_ERC59[MP], strAlmData);
			case 61:return _Save(m_ERC61[MP], strAlmData);
			case 62:return _Save(m_ERC62[MP], strAlmData);			
			case 64:return _Save(m_ERC64[MP], strAlmData);
			case 65:return _Save(m_ERC65[MP], strAlmData);
			case 66:return _Save(m_ERC66[MP], strAlmData);
			case 67:return _Save(m_ERC67[MP], strAlmData);		
			default:break;
		}
	}

	if ((ERC&0xffff0000) == 0xE2010000)//事件
	{

	}

//	logwt<<"请增加告警["<<ERC<<"]的存储方式!!!"<<endl;
	return -1;
}

int S_AlmDatas::_Load(const S_AlmData &AlmData, string &strAlmData)
{
	if (AlmData.m_Enable == false || AlmData.m_DataLen == 0x00)
		return -1;
	string strMyData(AlmData.m_Data, AlmData.m_DataLen);
	if (strMyData.compare(0, AlmData.m_DataLen, AlmData.m_Data, AlmData.m_DataLen) != 0)//avoid data change
	{
		strMyData.assign(AlmData.m_Data, AlmData.m_DataLen);
		if (strMyData.compare(0, AlmData.m_DataLen, AlmData.m_Data, AlmData.m_DataLen) != 0)
			return -1;
	}
	strAlmData.append(strMyData);
	return (int)AlmData.m_DataLen;
}

int S_AlmDatas::_Save(S_AlmData &AlmData, const string &strAlmData)
{
	AlmData.m_Enable = false;
	AlmData.m_DataLen = (S_BYTE)strAlmData.size();
	memcpy(AlmData.m_Data, strAlmData.data(), AlmData.m_DataLen);
	AlmData.m_Enable = true;
	return (int)AlmData.m_DataLen;
}

/** @return >0:成功; <=0:失败*/
int C_AlmPhaseData::Load(S_DWORD ERC, S_WORD MP, string &strAlmData)
{
	if (MP >= CONS_METER_COUNT_MAX)
		return -1;
	C_SimpData SimpData(CFG_PATH_ALARM);
	return SimpData.Load(ERC, MP, strAlmData);
}

/** @return >=0:成功; <0:失败*/
int C_AlmPhaseData::Save(S_DWORD ERC, S_WORD MP, const string &strAlmData)
{
	if (MP >= CONS_METER_COUNT_MAX)
		return -1;
	C_SimpData SimpData(CFG_PATH_ALARM);
	if (SimpData.Save(ERC, MP, strAlmData) <= 0)
		return -1;
	return 0;
}

/*
	brief: STATUS内存获得测量点的某一个告警类型所采集的寄存器数据集
	param [in] INT16U mp:测量点号
			  INT8U alarmFn:告警类型编号
			  S_RegDatas:返回的寄存器数据集
	return bool:true:获取成功,false:获取失败
	author:WanQingsong
	date: 2013-02-22
*/ 
bool	CAlarmEvtCommIdData::GetCommRegDataFromSTS(INT16U mp,S_DWORD DI,S_RegDatas &rtSTSRegDatas)
{
	string StsAlamData;
	S_MSDI smdi = STATUS.FindMasterStationDataID(CLASSDATA_3,DI,0);
	S_REGISTER comReg;
	S_RegData StsRegData;
	C_AlmPhaseData almPhaseData;
	INT8U	pos=0;

	//传入参数测量点号超出范围返回false
	if (mp > CONS_METER_COUNT_MAX)
	{
		return false;
	}
	INT8U index = (INT8U)(DI&0xff);

	if ((DI&0xffff0000) == 0xE2000000)//告警	
	{
		switch (index)
		{
			case 0:
			case 13:
			case 14:
			case 15:
			case 19:
			case 20:
			case 21:
			case 22:
			case 23:
			case 24:		
			case 43:			
			case 44:			
			case 46:			
			case 53:			
			case 54:			
			case 59:							
			case 61:				
			case 62:				
			case 64:
			case 65:
			case 66:
			case 67:		
				if (almPhaseData.Load(DI, mp, StsAlamData) == -1)
				{
					return false;
				}
				break;
				
			default:
				break;
		}
	}

	for (int i=0; smdi.m_CommRegID[i]!=0; i++)
	{
		comReg = STATUS.FindCommonRegister(smdi.m_CommRegID[i]);	
		StsRegData.m_regid = smdi.m_CommRegID[i];
		StsRegData.m_data.assign(StsAlamData.c_str()+pos,comReg.m_Len);
		pos +=  comReg.m_Len;
		rtSTSRegDatas.push_back(StsRegData);
	}

	return true;
		
}

/*
		brief: 从CONTEXT内存中获得测量点的某一个告警类型所采集的寄存器数据集
		param [in] INT16U mp:测量点号
				  INT8U alarmFn:告警类型编号
				  S_RegDatas:返回的寄存器数据集
		return bool:true:获取成功,false:获取失败
		author:WanQingsong
		date: 2013-02-22
*/ 
bool	CAlarmEvtCommIdData::GetCommRegDataFromCTX(INT16U mp,S_DWORD DI,S_RegDatas &rtCTXRegDatas)
{
	string CtxAlamData;
	S_MSDI smdi = STATUS.FindMasterStationDataID(CLASSDATA_3,DI,0);
	S_REGISTER comReg;
	S_RegData CtxRegData;
	S_AlmDatas *pCtxAlmDatas =  CONTEXT.GetAlarmDatas();
	INT8U	pos=0;

	//传入参数测量点号超出范围返回false
	if (mp > CONS_METER_COUNT_MAX )
	{
		return false;
	}
	INT8U index = (INT8U)(DI&0xff);

	if ((DI&0xffff0000) == 0xE2000000)//告警	
	{
		switch (index)
		{
			case 0:
			case 13:
			case 14:
			case 15:
			case 19:
			case 20:
			case 21:
			case 22:
			case 23:
			case 24:		
			case 43:			
			case 44:			
			case 46:			
			case 53:			
			case 54:			
			case 59:							
			case 61:				
			case 62:				
			case 64:
			case 65:
			case 66:
			case 67:		
				if (pCtxAlmDatas->Load(DI, mp, CtxAlamData) == -1)
				{
					return false;
				}
				break;
				
			default:
				break;
		}
	}
		
	for (int i=0; smdi.m_CommRegID[i]!=0; i++)
	{
		comReg = STATUS.FindCommonRegister(smdi.m_CommRegID[i]);	
		CtxRegData.m_regid = smdi.m_CommRegID[i];
		CtxRegData.m_data.assign(CtxAlamData.c_str()+pos,comReg.m_Len);
		pos +=  comReg.m_Len;
		rtCTXRegDatas.push_back(CtxRegData);
	}

	return true;
		
}
bool CAlarmEvtCommIdData::IsValidData(const char *pdata, INT8U len)
{
	string invalidData;
	invalidData.append((size_t)len, (char)0xff);
	if (memcmp(pdata,invalidData.data(),len) == 0)
	{
		return false;
	}
	return true;
}


