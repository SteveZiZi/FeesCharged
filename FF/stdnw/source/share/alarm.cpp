#include "alarm.h"
#include "context.h"
#include "status.h"
#include "AlarmEventRecord.h"
#include "busialgo.h"
#include <time.h>
#include "ipcSharemem.h"


/****************************************************/
/**       class CAlarm type define                 				*/
/**            									   		*/	
/****************************************************/
/*
	brief: 抄表失败告警
	param [in] void
	return bool
	author:WanQingsong
	date: 2012-04-13
*/  
bool CAlarm_ReadMeterFail::Detect(void)
{
	bool res = false;
	S_ALLMP *pALLMP = STATUS.GetALLMP();
	S_TermData *pTermData =  STATUS.GetTermData();
	char timeBcd7[7];
	static char temp = 0;
	
	C_TIME::CurTime2BCDStr_Big(timeBcd7);
	/* 一天做一次抄表失败告警检测*/
	if (temp == timeBcd7[3])
	{
		return false;
	}
	
	temp = timeBcd7[3];//日
	m_FailMetersDbs.ClearBit();
	
	for (size_t st=pALLMP->m_MTBS.m_AllMPBS.begin(); st<pALLMP->m_MTBS.m_AllMPBS.end(); st++)
	{
		if (pALLMP->m_MTBS.m_AllMPBS.test(st))
		{
			if (pTermData->m_MeterData.m_timeMeterCommunication[st] == 0)
			{
				pTermData->m_MeterData.m_timeMeterCommunication[st] = C_TIME::TimeNow();
			}

			char buffer[64];
			S_TIME timeLast = pTermData->m_MeterData.m_timeMeterCommunication[st];
			S_TIME timeCurrent = C_TIME::TimeNow();

			/* wqs debug*/
			logvt<<"【ERC 】="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;
			logvt<<"抄表失败开始时间:"<<C_TIME::Time2Ascii14(timeLast, buffer)<<endl;
			logvt<<"当前时间:"<<C_TIME::Time2Ascii14(timeCurrent, buffer)<<endl;
			logvt<<"抄表失败天数:"<<C_TIME::DateDiff(TIME_UNIT_DAY, timeLast, timeCurrent)<<endl;
			/* end */
			
			/* 如果电能表超过三天没有抄读到数据，第四天一过0点则产生抄表失败告警 */
			if (C_TIME::DateDiff(TIME_UNIT_DAY, timeLast, timeCurrent) > 3)
			{
				m_FailMetersDbs.SetBit((int)st);
				logit<<"电表["<<(unsigned int)st<<"]产生抄表失败告警"<<endl;
				res = true;
			}
		}
	}

	if (res == true)
	{
		CAlarmRecrod_ARD21::AlarmRecord(ALM_COMMID(GetAlarmID()),&m_FailMetersDbs,ALARMSTATE_CREATE, C_TIME::TimeNow());
	}
	return res;
}

/*
	brief: A 相潮流反向告警
	param [in] void
	return bool
	author:WanQingsong
	date: 2013-09-11
*/  
bool CAlarm_CurrentReverse_A::Detect(void)
{
	bool res = false;
	S_AlmDatas *pCtxAlmDatas =  CONTEXT.GetAlarmDatas();
	S_ALLMP *pALLMP = STATUS.GetALLMP();
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_RegDatas CtxRegData;
	string CtxAlmData;
	S_AlarmStatus *pAlarmStatus = STATUS.GetAlarmStatus();
	S_AlmFlags *pAlmFlags = CONTEXT.GetAlmFlags();
	
	
	for (size_t st=pALLMP->m_MultiMPBS.begin(); st<pALLMP->m_MultiMPBS.end(); st++)
	{
		if (pALLMP->m_MultiMPBS.test(st) && pAlmFlags->TestMeter(GetAlarmID(), (S_WORD)st))
		{
			CtxRegData.clear();
			CtxAlmData.clear();

			if (pCtxAlmDatas->Load(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData) ==-1)
			{
				loget<<"CAlarm_CurrentReverse_A CONTEXT 内存告警数据加载失败!"<<endl;
				continue;
			}

			if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,ALM_COMMID(GetAlarmID()),CtxRegData) == false)
			{
				loget<<"CAlarm_CurrentReverse_A GetCommRegDataFromCTX失败!"<<endl;
				continue;
			}

			
			/* wqs debug*/
			logvt<<"【ERC 】="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;
			logvt<<"CAlarm_CurrentReverse_A CtxRegData[0].m_data: "<<HEX<<CtxRegData[0].m_data<<endl;
			/* end */
			
			/* 电科院台体采用电表状态字判断*/
			if (CtxRegData[0].m_data.at(0)&0x40)//A相潮流反向
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) != ALARMSTATE_CREATE)
				{
					if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
					{
						logvt<<" A相潮流反向告警产生"<<endl;
						pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_CREATE);
						
					}
				}
				res = true;
			}
			else
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) == ALARMSTATE_CREATE)
				{
					if ( st >= 1 && st <= 8)//总表才产生恢复告警
					{
						if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_RESUME, C_TIME::TimeNow()) == true)
						{
							logvt<<" A相潮流反向告警恢复"<<endl;
						}
					}
					res = true;
				}
				pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_RESUME);
			}
		}
		
	}
	return res;
}

/*
	brief: B 相潮流反向告警
	param [in] void
	return bool
	author:WanQingsong
	date: 2013-09-11
*/  
bool CAlarm_CurrentReverse_B::Detect(void)
{
	bool res = false;
	S_AlmDatas *pCtxAlmDatas =  CONTEXT.GetAlarmDatas();
	S_ALLMP *pALLMP = STATUS.GetALLMP();
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_RegDatas CtxRegData;
	string CtxAlmData;
	S_AlarmStatus *pAlarmStatus = STATUS.GetAlarmStatus();
	S_AlmFlags *pAlmFlags = CONTEXT.GetAlmFlags();
	
	for (size_t st=pALLMP->m_MultiMPBS.begin(); st<pALLMP->m_MultiMPBS.end(); st++)
	{
		if (pALLMP->m_MultiMPBS.test(st) && pAlmFlags->TestMeter(GetAlarmID(), (S_WORD)st))
		{
			CtxRegData.clear();
			CtxAlmData.clear();

			if (pCtxAlmDatas->Load(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData) ==-1)
			{
				loget<<"CAlarm_CurrentReverse_B CONTEXT 内存告警数据加载失败!"<<endl;
				continue;
			}
			
			if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,ALM_COMMID(GetAlarmID()),CtxRegData) == false)
			{
				loget<<"CAlarm_CurrentReverse_B GetCommRegDataFromCTX失败!"<<endl;
				continue;
			}

			/* wqs debug*/
			logvt<<"【ERC 】="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;			
			logvt<<"CAlarm_CurrentReverse_B CtxRegData[0].m_data: "<<HEX<<CtxRegData[0].m_data<<endl;
			/* end */
			
			/* 电科院台体采用电表状态字判断*/
			if (CtxRegData[0].m_data.at(0)&0x40)//B相潮流反向
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) != ALARMSTATE_CREATE)
				{
					if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
					{
						logvt<<" B相潮流反向告警产生"<<endl;
						pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_CREATE);
					}
					res = true;
				}
			}
			else
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) == ALARMSTATE_CREATE)
				{
					if ( st >= 1 && st <= 8)//总表才产生恢复告警
					{
						if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_RESUME, C_TIME::TimeNow()) == true)
						{
							logvt<<" B相潮流反向告警恢复"<<endl;
						}
					}
					res = true;
				}
				pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_RESUME);
			}

		}
		
	}
	return res;
}

/*
	brief: C 相潮流反向告警
	param [in] void
	return bool
	author:WanQingsong
	date: 2013-09-11
*/  
bool CAlarm_CurrentReverse_C::Detect(void)
{
	bool res = false;
	S_AlmDatas *pCtxAlmDatas =  CONTEXT.GetAlarmDatas();
	S_ALLMP *pALLMP = STATUS.GetALLMP();
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_RegDatas CtxRegData;
	string CtxAlmData;
	S_AlarmStatus *pAlarmStatus = STATUS.GetAlarmStatus();
	S_AlmFlags *pAlmFlags = CONTEXT.GetAlmFlags();
	
	for (size_t st=pALLMP->m_MultiMPBS.begin(); st<pALLMP->m_MultiMPBS.end(); st++)
	{
		if (pALLMP->m_MultiMPBS.test(st) && pAlmFlags->TestMeter(GetAlarmID(), (S_WORD)st))
		{
			CtxRegData.clear();
			CtxAlmData.clear();

			if (pCtxAlmDatas->Load(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData) ==-1)
			{
				loget<<"CAlarm_CurrentReverse_C CONTEXT 内存告警数据加载失败!"<<endl;
				continue;
			}
			
			if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,ALM_COMMID(GetAlarmID()),CtxRegData) == false)
			{
				loget<<"CAlarm_CurrentReverse_C GetCommRegDataFromCTX失败!"<<endl;
				continue;
			}

			/* wqs debug*/
			logvt<<"【ERC 】="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;			
			logvt<<"CAlarm_CurrentReverse_C CtxRegData[0].m_data: "<<HEX<<CtxRegData[0].m_data<<endl;
			/* end */

			/* 电科院台体采用电表状态字判断*/
			if (CtxRegData[0].m_data.at(0)&0x40)//C相潮流反向
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) != ALARMSTATE_CREATE)
				{
					if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
					{
						logvt<<" C相潮流反向告警产生"<<endl;
						pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_CREATE);
					}
					res = true;
				}	
			}
			else
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) == ALARMSTATE_CREATE)
				{
					if ( st >= 1 && st <= 8)//总表才产生恢复告警
					{
						if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_RESUME, C_TIME::TimeNow()) == true)
						{
							logvt<<" C相潮流反向告警恢复"<<endl;
						}
					}
					res = true;
				}
				pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_RESUME);
			}
		}
		
	}
	return res;
}

/*
	brief: A 相失流告警
	param [in] void
	return bool
	author:WanQingsong
	date: 2013-09-11
*/  
bool CAlarm_LoseCurrent_A::Detect(void)
{
	bool res = false;
	S_AlmDatas *pCtxAlmDatas =  CONTEXT.GetAlarmDatas();
	S_ALLMP *pALLMP = STATUS.GetALLMP();
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_RegDatas CtxRegData;
	string CtxAlmData;
	S_AlarmStatus *pAlarmStatus = STATUS.GetAlarmStatus();
	S_AlmFlags *pAlmFlags = CONTEXT.GetAlmFlags();
	
	for (size_t st=pALLMP->m_MultiMPBS.begin(); st<pALLMP->m_MultiMPBS.end(); st++)
	{
		if (pALLMP->m_MultiMPBS.test(st) && pAlmFlags->TestMeter(GetAlarmID(), (S_WORD)st))
		{
			CtxRegData.clear();
			CtxAlmData.clear();

			if (pCtxAlmDatas->Load(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData) ==-1)
			{
				loget<<"CAlarm_LoseCurrent_A CONTEXT 内存告警数据加载失败!"<<endl;
				continue;
			}
			
			if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,ALM_COMMID(GetAlarmID()),CtxRegData) == false)
			{
				loget<<"CAlarm_LoseCurrent_A GetCommRegDataFromCTX失败!"<<endl;
				continue;
			}

			/* wqs debug*/
			logvt<<"【ERC 】="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;			
			logvt<<"CAlarm_LoseCurrent_A CtxRegData[0].m_data: "<<HEX<<CtxRegData[0].m_data<<endl;
			/* end */

/*			 if (StsRegData[0].m_data != CtxRegData[0].m_data)//A相失压次数是否变化
			{
				if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
				{
					logvt<<" A相失流告警产生"<<endl;
					
				}
				res = true;
			}*/
			/* 电科院台体采用电表状态字判断*/
			if (CtxRegData[0].m_data.at(0)&0x08)//A相失流
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) != ALARMSTATE_CREATE)
				{
					if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
					{
						logvt<<" A相失流告警产生"<<endl;
						pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_CREATE);
					}
					res = true;
				}
			}
			else
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) == ALARMSTATE_CREATE)
				{
					if ( st >= 1 && st <= 8)//总表才产生恢复告警
					{
						if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_RESUME, C_TIME::TimeNow()) == true)
						{
							logvt<<" A相失流告警恢复"<<endl;
						}
					}
					res = true;
				}
				pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_RESUME);
			}
		}
		
	}
	return res;
}

/*
	brief: B 相失流告警
	param [in] void
	return bool
	author:WanQingsong
	date: 2013-09-11
*/  
bool CAlarm_LoseCurrent_B::Detect(void)
{
	bool res = false;
	S_AlmDatas *pCtxAlmDatas =  CONTEXT.GetAlarmDatas();
	S_ALLMP *pALLMP = STATUS.GetALLMP();
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_RegDatas CtxRegData;
	string CtxAlmData;
	S_AlarmStatus *pAlarmStatus = STATUS.GetAlarmStatus();
	S_AlmFlags *pAlmFlags = CONTEXT.GetAlmFlags();
	
	for (size_t st=pALLMP->m_MultiMPBS.begin(); st<pALLMP->m_MultiMPBS.end(); st++)
	{
		if (pALLMP->m_MultiMPBS.test(st) && pAlmFlags->TestMeter(GetAlarmID(), (S_WORD)st))
		{
			CtxRegData.clear();
			CtxAlmData.clear();
	
			if (pCtxAlmDatas->Load(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData) ==-1)
			{
				loget<<"CAlarm_LoseCurrent_B CONTEXT 内存告警数据加载失败!"<<endl;
				continue;
			}
			if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,ALM_COMMID(GetAlarmID()),CtxRegData) == false)
			{
				loget<<"CAlarm_LoseCurrent_B GetCommRegDataFromCTX失败!"<<endl;
				continue;
			}

			/* wqs debug*/
			logvt<<"【ERC 】="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;
			logvt<<"CAlarm_LoseCurrent_B CtxRegData[0].m_data: "<<HEX<<CtxRegData[0].m_data<<endl;
			/* end */
			
			/* 电科院台体采用电表状态字判断*/
			if (CtxRegData[0].m_data.at(0)&0x08)//B相失流
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) != ALARMSTATE_CREATE)
				{
					if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
					{
						logvt<<" B相失流告警产生"<<endl;
						pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_CREATE);
						
					}
					res = true;
				}
			}
			else
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) == ALARMSTATE_CREATE)
				{
					if ( st >= 1 && st <= 8)//总表才产生恢复告警
					{
						if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_RESUME, C_TIME::TimeNow()) == true)
						{
							logvt<<" B相失流告警恢复"<<endl;
						}
					}
					res = true;
				}
				pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_RESUME);
			}
		}
		
	}
	return res;
}

/*
	brief: C 相失流告警
	param [in] void
	return bool
	author:WanQingsong
	date: 2013-09-11
*/  
bool CAlarm_LoseCurrent_C::Detect(void)
{
	bool res = false;
	S_AlmDatas *pCtxAlmDatas =  CONTEXT.GetAlarmDatas();
	S_ALLMP *pALLMP = STATUS.GetALLMP();
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_RegDatas CtxRegData;
	string CtxAlmData;
	S_AlarmStatus *pAlarmStatus = STATUS.GetAlarmStatus();
	S_AlmFlags *pAlmFlags = CONTEXT.GetAlmFlags();
	
	for (size_t st=pALLMP->m_MultiMPBS.begin(); st<pALLMP->m_MultiMPBS.end(); st++)
	{
		if (pALLMP->m_MultiMPBS.test(st) && pAlmFlags->TestMeter(GetAlarmID(), (S_WORD)st))
		{
			CtxRegData.clear();
			CtxAlmData.clear();

			if (pCtxAlmDatas->Load(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData) ==-1)
			{
				loget<<"CAlarm_LoseCurrent_C CONTEXT 内存告警数据加载失败!"<<endl;
				continue;
			}
			
			if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,ALM_COMMID(GetAlarmID()),CtxRegData) == false)
			{
				loget<<"CAlarm_LoseCurrent_C GetCommRegDataFromCTX失败!"<<endl;
				continue;
			}

			/* wqs debug*/
			logvt<<"【ERC 】="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;			
			logvt<<"CAlarm_LoseCurrent_C CtxRegData[0].m_data: "<<HEX<<CtxRegData[0].m_data<<endl;
			/* end */
			
			/* 电科院台体采用电表状态字判断*/
			if (CtxRegData[0].m_data.at(0)&0x08)//B相失流
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) != ALARMSTATE_CREATE)
				{
					if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
					{
						logvt<<" C相失流告警产生"<<endl;
						pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_CREATE);
					}
					res = true;
				}
			}
			else
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) == ALARMSTATE_CREATE)
				{
					if ( st >= 1 && st <= 8)//总表才产生恢复告警
					{
						if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_RESUME, C_TIME::TimeNow()) == true)
						{
							logvt<<" C相失流告警恢复"<<endl;
						}
					}
					res = true;
				}
				pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_RESUME);
			}
		}
		
	}
	return res;
}



/*
	brief: A 相失压告警
	param [in] void
	return bool
	author:WanQingsong
	date: 2013-09-11
*/  
bool CAlarm_LoseVoltage_A::Detect(void)
{
	bool res = false;
	S_AlmDatas *pCtxAlmDatas =  CONTEXT.GetAlarmDatas();
	S_ALLMP *pALLMP = STATUS.GetALLMP();
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_RegDatas CtxRegData;
	string CtxAlmData;
	S_AlarmStatus *pAlarmStatus = STATUS.GetAlarmStatus();
	S_AlmFlags *pAlmFlags = CONTEXT.GetAlmFlags();
	
	for (size_t st=pALLMP->m_MultiMPBS.begin(); st<pALLMP->m_MultiMPBS.end(); st++)
	{
		if (pALLMP->m_MultiMPBS.test(st) && pAlmFlags->TestMeter(GetAlarmID(), (S_WORD)st))
		{
			CtxRegData.clear();
			CtxAlmData.clear();

			if (pCtxAlmDatas->Load(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData) ==-1)
			{
				loget<<"CAlarm_LoseVoltage_A CONTEXT 内存告警数据加载失败!"<<endl;
				continue;
			}
			
			if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,ALM_COMMID(GetAlarmID()),CtxRegData) == false)
			{
				loget<<"CAlarm_LoseVoltage_A GetCommRegDataFromCTX失败!"<<endl;
				continue;
			}

			/* wqs debug*/
			logvt<<"【ERC 】="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;			
			logvt<<"CAlarm_LoseVoltage_A CtxRegData[0].m_data: "<<HEX<<CtxRegData[0].m_data<<endl;
			/* end */
			
			/* 电科院台体采用电表状态字判断*/
			if (CtxRegData[0].m_data.at(0)&0x01)//A相失压
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) != ALARMSTATE_CREATE)
				{
					if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
					{
						logvt<<" A相失压告警产生"<<endl;
						pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_CREATE);
					}
					res = true;
				}
			}
			else
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) == ALARMSTATE_CREATE)
				{
					if ( st >= 1 && st <= 8)//总表才产生恢复告警
					{
						if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_RESUME, C_TIME::TimeNow()) == true)
						{
							logvt<<" A相失压告警恢复"<<endl;
						}
					}
					res = true;
				}
				pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_RESUME);
			}
		}
		
	}
	return res;
}

/*
	brief: B 相失压告警
	param [in] void
	return bool
	author:WanQingsong
	date: 2013-09-11
*/  
bool CAlarm_LoseVoltage_B::Detect(void)
{
	bool res = false;
	S_AlmDatas *pCtxAlmDatas =  CONTEXT.GetAlarmDatas();
	S_ALLMP *pALLMP = STATUS.GetALLMP();
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_RegDatas CtxRegData;
	string CtxAlmData;
	S_AlarmStatus *pAlarmStatus = STATUS.GetAlarmStatus();
	S_AlmFlags *pAlmFlags = CONTEXT.GetAlmFlags();
	
	for (size_t st=pALLMP->m_MultiMPBS.begin(); st<pALLMP->m_MultiMPBS.end(); st++)
	{
		if (pALLMP->m_MultiMPBS.test(st) && pAlmFlags->TestMeter(GetAlarmID(), (S_WORD)st))
		{
			CtxRegData.clear();
			CtxAlmData.clear();

			if (pCtxAlmDatas->Load(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData) ==-1)
			{
				loget<<"CAlarm_LoseVoltage_B CONTEXT 内存告警数据加载失败!"<<endl;
				continue;
			}
			
			if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,ALM_COMMID(GetAlarmID()),CtxRegData) == false)
			{
				loget<<"CAlarm_LoseVoltage_B GetCommRegDataFromCTX失败!"<<endl;
				continue;
			}

			/* wqs debug*/
			logvt<<"【ERC 】="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;			
			logvt<<"CAlarm_LoseVoltage_B CtxRegData[0].m_data: "<<HEX<<CtxRegData[0].m_data<<endl;
			/* end */
			
			/* 电科院台体采用电表状态字判断*/
			if (CtxRegData[0].m_data.at(0)&0x01)//B相失压
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) != ALARMSTATE_CREATE)
				{
					if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
					{
						logvt<<" B相失压告警产生"<<endl;
						pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_CREATE);
					}
					res = true;
				}
			}
			else
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) == ALARMSTATE_CREATE)
				{
					if ( st >= 1 && st <= 8)//总表才产生恢复告警
					{
						if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_RESUME, C_TIME::TimeNow()) == true)
						{
							logvt<<" B相失压告警恢复"<<endl;
						}
					}
					res = true;
				}
				pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_RESUME);
			}
		}
		
	}
	return res;
}

/*
	brief: C 相失压告警
	param [in] void
	return bool
	author:WanQingsong
	date: 2013-09-11
*/  
bool CAlarm_LoseVoltage_C::Detect(void)
{
	bool res = false;
	S_AlmDatas *pCtxAlmDatas =  CONTEXT.GetAlarmDatas();
	S_ALLMP *pALLMP = STATUS.GetALLMP();
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_RegDatas CtxRegData;
	string CtxAlmData;
	S_AlarmStatus *pAlarmStatus = STATUS.GetAlarmStatus();	
	S_AlmFlags *pAlmFlags = CONTEXT.GetAlmFlags();
	
	for (size_t st=pALLMP->m_MultiMPBS.begin(); st<pALLMP->m_MultiMPBS.end(); st++)
	{
		if (pALLMP->m_MultiMPBS.test(st) && pAlmFlags->TestMeter(GetAlarmID(), (S_WORD)st))
		{
			CtxRegData.clear();
			CtxAlmData.clear();

			if (pCtxAlmDatas->Load(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData) ==-1)
			{
				loget<<"CAlarm_LoseVoltage_C CONTEXT 内存告警数据加载失败!"<<endl;
				continue;
			}
			
			if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,ALM_COMMID(GetAlarmID()),CtxRegData) == false)
			{
				loget<<"CAlarm_LoseVoltage_C GetCommRegDataFromCTX失败!"<<endl;
				continue;
			}

			/* wqs debug*/
			logvt<<"【ERC 】="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;			
			logvt<<"CAlarm_LoseVoltage_C CtxRegData[0].m_data: "<<HEX<<CtxRegData[0].m_data<<endl;
			/* end */
			
			/* 电科院台体采用电表状态字判断*/
			if (CtxRegData[0].m_data.at(0)&0x01)//C相失压
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) != ALARMSTATE_CREATE)
				{
					if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
					{
						logvt<<" C相失压告警产生"<<endl;
						pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_CREATE);
					}
					res = true;
				}
			}
			else
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) == ALARMSTATE_CREATE)
				{
					if ( st >= 1 && st <= 8)//总表才产生恢复告警
					{
						if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_RESUME, C_TIME::TimeNow()) == true)
						{
							logvt<<" C相失压告警恢复"<<endl;
						}
					}
					res = true;
				}
				pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_RESUME);
			}
		}
		
	}
	return res;
}


/*
	brief: 电能表示度下降
	param [in] void
	return bool
	author:WanQingsong
	date: 2013-09-13
*/
bool CAlarm_MeterDegreeDec::Detect()
{
	bool res = false;
	C_AlmPhaseData almPhaseData;
	S_AlmDatas *pCtxAlmDatas =  CONTEXT.GetAlarmDatas();
	S_ALLMP *pALLMP = STATUS.GetALLMP();
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_AlarmStatus *pAlarmStatus = STATUS.GetAlarmStatus();
	S_AlmFlags *pAlmFlags = CONTEXT.GetAlmFlags();
	S_RegDatas StsRegData;
	S_RegDatas CtxRegData;
	string CtxAlmData;
	string StsAlmData;
	
	for (size_t st=pALLMP->m_MTBS.m_AllMPBS.begin(); st<pALLMP->m_MTBS.m_AllMPBS.end(); st++)
	{
		if (pALLMP->m_MTBS.m_AllMPBS.test(st) && pAlmFlags->TestMeter(GetAlarmID(), (S_WORD)st))
		{
			StsRegData.clear();
			CtxRegData.clear();
			CtxAlmData.clear();
			StsAlmData.clear();
			
			if (pCtxAlmDatas->Load(REGS_ARD2,(S_WORD)st,CtxAlmData) ==-1)
			{
				loget<<"CAlarm_MeterDegreeDec CONTEXT 内存告警数据加载失败!"<<endl;
				continue;
			}
			if (almPhaseData.Load(ALM_COMMID(GetAlarmID()),(S_WORD)st,StsAlmData) ==-1)
			{
				loget<<"CAlarm_MeterDegreeDec STATUS 内存告警数据加载失败!"<<endl;
				almPhaseData.Save(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData);
				continue;
			}

			if (CAlarmEvtCommIdData::GetCommRegDataFromSTS((INT16U)st,ALM_COMMID(GetAlarmID()),StsRegData) == false)
			{
				continue;
			}
			
			if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,REGS_ARD2,CtxRegData) == false)
			{
				continue;
			}
			/* 正向有功*/
			C_BCD zxyg_old_value((S_BYTE*)(StsRegData[0].m_data.data()),6,4);
			C_BCD zxyg_new_value((S_BYTE*)(CtxRegData[0].m_data.data()),6,4);
			/* wqs debug */
			S_DWORD Integer;
			S_WORD Float;
			logvt<<"【ERC 】="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;
			zxyg_old_value.GetBcdData(Integer, Float);
			logvt<<" zxyg_old_value="<<Integer<<"."<<Float<<endl;
			zxyg_new_value.GetBcdData(Integer, Float);
			logvt<<" zxyg_new_value="<<Integer<<"."<<Float<<endl;
			/* end */


			/* 反向有功*/
			C_BCD fxyg_old_value((S_BYTE*)(StsRegData[1].m_data.data()),6,4);
			C_BCD fxyg_new_value((S_BYTE*)(CtxRegData[1].m_data.data()),6,4);
			/* wqs debug */
			fxyg_old_value.GetBcdData(Integer, Float);
			logvt<<" fxyg_old_value="<<Integer<<"."<<Float<<endl;
			fxyg_new_value.GetBcdData(Integer, Float);
			logvt<<" fxyg_new_value="<<Integer<<"."<<Float<<endl;
			/* end */
					
			if (zxyg_new_value < zxyg_old_value || fxyg_new_value < fxyg_old_value )
			{
				logvt<<"电表号"<<(INT16U)st<<"示度下降"<<endl;
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) != ALARMSTATE_CREATE)
				{
					if (CAlarmRecrod_ARD3::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
					{
						res = true;	
						pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_CREATE);
					}
				}
				
			}

			if (zxyg_new_value > zxyg_old_value)
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) == ALARMSTATE_CREATE)
				{
					pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_RESUME);
				}
			}
			
			if (CtxAlmData != StsAlmData)
			{
				almPhaseData.Save(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData);
			}
		}
		
	}
	return res;
}


/*
	brief: 电表时段或费率变更
	param [in] void
	return bool
	author:WanQingsong
	date: 2013-09-13
*/  
bool CAlarm_PeriodTariffChange::Detect(void)
{
	bool res = false;
	S_AlmDatas *pCtxAlmDatas =  CONTEXT.GetAlarmDatas();
	C_AlmPhaseData almPhaseData;	
	S_ALLMP *pALLMP = STATUS.GetALLMP();
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_AlmFlags *pAlmFlags = CONTEXT.GetAlmFlags();
	S_RegDatas StsRegData;
	S_RegDatas CtxRegData;
	string CtxAlmData;
	string StsAlmData;

	SleepSecond(10);
	
	for (size_t st=pALLMP->m_MTBS.m_AllMPBS.begin(); st<pALLMP->m_MTBS.m_AllMPBS.end(); st++)
	{
		if (pALLMP->m_MTBS.m_AllMPBS.test(st) && pAlmFlags->TestMeter(GetAlarmID(), (S_WORD)st))
		{
			StsRegData.clear();
			CtxRegData.clear();
			CtxAlmData.clear();
			StsAlmData.clear();

			if (pCtxAlmDatas->Load(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData) ==-1)
			{
				loget<<"CAlarm_PeriodTariffChange CONTEXT 内存告警数据加载失败!"<<endl;
				continue;
			}
			if (almPhaseData.Load(ALM_COMMID(GetAlarmID()),(S_WORD)st,StsAlmData) ==-1)
			{
				loget<<"CAlarm_PeriodTariffChange STATUS 内存告警数据加载失败!"<<endl;
				almPhaseData.Save(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData);
				continue;
			}
			if (CAlarmEvtCommIdData::GetCommRegDataFromSTS((INT16U)st,ALM_COMMID(GetAlarmID()),StsRegData) == false)
			{
				loget<<"CAlarm_PeriodTariffChange GetCommRegDataFromSTS失败!"<<endl;
				continue;
			}
			
			if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,ALM_COMMID(GetAlarmID()),CtxRegData) == false)
			{
				loget<<"CAlarm_PeriodTariffChange GetCommRegDataFromCTX失败!"<<endl;			
				continue;
			}
			/* wqs debug*/
			logvt<<"【ERC 】="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;			
			logvt<<"CAlarm_PeriodTariffChange:"<<"CtxRegData[0].m_data: "<<HEX<<CtxRegData[0].m_data<<endl;

			/* end */
			
			if (StsRegData[0].m_data != CtxRegData[0].m_data
			    ||StsRegData[1].m_data != CtxRegData[1].m_data
		    	    ||StsRegData[2].m_data != CtxRegData[2].m_data
			    ||StsRegData[3].m_data != CtxRegData[3].m_data)       
			{
				if (CAlarmRecrod_ARD1::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
				{
					logvt<<" 表"<<(INT16U)st<<"时段费率变更告警产生"<<endl;
				}
				res = true;
			}
			if (CtxAlmData != StsAlmData)
			{
				almPhaseData.Save(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData);
			}
		}
		
	}
	return res;
}

bool CAlarm_TerminalPowerOff::Detect(void)
{
	bool res = false;
	S_MeterInfo meter;

	meter.m_MeasureNo = 0;//终端事件

	if (CAlarmRecrod_ARD2::AlarmRecord(meter,ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
	{
		logvt<<" 终端掉电告警产生"<<endl;
		res = true;
	}

	return res;
}

bool CAlarm_TerminalPowerOn::Detect(void)
{
	bool res = false;
	S_MeterInfo meter;

	meter.m_MeasureNo = 0;//终端事件

	if (CAlarmRecrod_ARD2::AlarmRecord(meter,ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
	{
		logvt<<" 终端上电告警产生"<<endl;
		res = true;
	}

	return res;
}


bool CAlarm_MonthFlowOverRange::Detect(void)
{
	bool res = false;
	S_HttxInfo *pHttxInfo=STATUS.GetHttxInfo(MP_HTTX_1);
	char timeBcd7[7];
	static char temp = 0;
	S_MeterInfo meter;
	static bool bHappen = false;

	meter.m_MeasureNo = 0;//终端事件

	/* wqs debug*/
	if (pHttxInfo->m_MonTrafficLimit != 0)
	{ // 需要告警才打印统计信息
		logvt<<"【ERC 】="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",月流量="<<STATUS.GetMonCommFlux()<<",月流量阈值="<<pHttxInfo->m_MonTrafficLimit<<endl;
	}
	/* end */

	C_TIME::CurTime2BCDStr_Big(timeBcd7);
	
	if (bHappen == true)
	{
		if ((pHttxInfo->m_MonTrafficLimit == 0) || (STATUS.GetMonCommFlux() < pHttxInfo->m_MonTrafficLimit))
		{
//			if (CAlarmRecrod_ARD10::AlarmRecord(meter,ALM_COMMID(GetAlarmID()),ALARMSTATE_RESUME, C_TIME::TimeNow(),STATUS.GetMonCommFlux()) == true)
			{
				logvt<<" 月流量越限告警恢复"<<endl;
				res = true;
				bHappen = false;
			}
		}

		/*月流量越限产生以后，在月流量越限未恢复， 一天只产生一次月流量越限告警*/
		if (temp == timeBcd7[3])
		{
			return false;
		}
	}

	temp = timeBcd7[3];//日
	
	if ((STATUS.GetMonCommFlux() > pHttxInfo->m_MonTrafficLimit) &&  (pHttxInfo->m_MonTrafficLimit > 0))	
	{
		if (CAlarmRecrod_ARD10::AlarmRecord(meter,ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow(),STATUS.GetMonCommFlux()) == true)
		{
			logvt<<" 月流量越限告警产生"<<endl;
			res = true;
			bHappen = true;
		}
	}
	
	return res;
}


/*
	brief: 继电器变位告警
	param [in] void
	return bool
	author:WanQingsong
	date: 2013-09-13
*/  
bool CAlarm_RelayChange::Detect(void)
{
	bool res = false;
	S_AlmDatas *pCtxAlmDatas =  CONTEXT.GetAlarmDatas();
	C_AlmPhaseData almPhaseData;	
	S_ALLMP *pALLMP = STATUS.GetALLMP();
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_AlmFlags *pAlmFlags = CONTEXT.GetAlmFlags();
	S_RegDatas StsRegData;
	S_RegDatas CtxRegData;
	string CtxAlmData;
	string StsAlmData;
//	S_AlarmStatus *pAlarmStatus = STATUS.GetAlarmStatus();
	
	
	for (size_t st=pALLMP->m_MTBS.m_AllMPBS.begin(); st<pALLMP->m_MTBS.m_AllMPBS.end(); st++)
	{
		if (pALLMP->m_MTBS.m_AllMPBS.test(st) && pAlmFlags->TestMeter(GetAlarmID(), (S_WORD)st))
		{
			StsRegData.clear();
			CtxRegData.clear();
			CtxAlmData.clear();
			StsAlmData.clear();
			
			
			if (pCtxAlmDatas->Load(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData) ==-1)
			{
				loget<<"CAlarm_RelayChange CONTEXT 内存告警数据加载失败!"<<endl;
				continue;
			}
			if (almPhaseData.Load(ALM_COMMID(GetAlarmID()),(S_WORD)st,StsAlmData) ==-1)
			{
				loget<<"CAlarm_RelayChange STATUS 内存告警数据加载失败!"<<endl;
				almPhaseData.Save(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData);
				continue;
			}
			if (CAlarmEvtCommIdData::GetCommRegDataFromSTS((INT16U)st,ALM_COMMID(GetAlarmID()),StsRegData) == false)
			{
				loget<<"CAlarm_RelayChange GetCommRegDataFromSTS失败!"<<endl;
				continue;
			}

			IPCSHAREMEM.m_IpcDevicesRegData[st].m_RelayState = StsRegData[0].m_data[0] & 0x10?1:0;
			
			if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,ALM_COMMID(GetAlarmID()),CtxRegData) == false)
			{
				loget<<"CAlarm_RelayChange GetCommRegDataFromCTX失败!"<<endl;
				continue;
			}
			/* wqs debug*/
			logvt<<"【ERC 】="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;
			logvt<<"CAlarm_RelayChange:"<<"CtxRegData[0].m_data: "<<HEX<<CtxRegData[0].m_data<<endl;
			/* end */
			 if ((CtxRegData[0].m_data[0] & 0x10) != (StsRegData[0].m_data[0] & 0x10))//电表运行状态字3，bit4为继电器状态位发生改变
			{
				//if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) != ALARMSTATE_CREATE)
				{
					if (CAlarmRecrod_ARD12::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
					{
						logvt<<" 表"<<(INT16U)st<<"继电器变位告警产生"<<endl;
						
						//pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_CREATE);
					}
					res = true;
				}
			}

			if (CtxAlmData != StsAlmData)
			{
				almPhaseData.Save(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData);
			}
		}
		
	}
	return res;
}

/*
	brief: A B C相失压次数告警
	param [in] void
	return bool
	author:WanQingsong
	date: 2013-09-13
*/  
bool CAlarm_LoseVoltageTimes::Detect(void)
{
	bool res = false;
#if 0	
	C_AlmPhaseData almPhaseData;
	S_AlmDatas *pCtxAlmDatas =  CONTEXT.GetAlarmDatas();
	S_ALLMP *pALLMP = STATUS.GetALLMP();
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_RegDatas StsRegData;
	S_RegDatas CtxRegData;
	string CtxAlmData;
	string StsAlmData;
	
	
	for (size_t st=pALLMP->m_MultiMPBS.begin(); st<pALLMP->m_MultiMPBS.end(); st++)
	{
		if (pALLMP->m_MultiMPBS.test(st))
		{
			StsRegData.clear();
			CtxRegData.clear();
			CtxAlmData.clear();
			StsAlmData.clear();

			if (pCtxAlmDatas->Load(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData) ==-1)
			{
				loget<<"CAlarm_LoseVoltageTimes:CONTEXT 内存告警数据加载失败!"<<endl;
				continue;
			}

			if (almPhaseData.Load(ALM_COMMID(GetAlarmID()),(S_WORD)st,StsAlmData) ==-1)
			{
				loget<<"CAlarm_LoseVoltageTimes:STATUS 内存告警数据加载失败!"<<endl;
				almPhaseData.Save(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData);
				continue;
			}
			/* wqs debug*/
			logvt<<"【ERC 】="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<endl;			
			logvt<<"CAlarm_LoseVoltageTimes:"<<"CtxAlmData: "<<HEX<<CtxAlmData<<endl;
			logvt<<"CAlarm_LoseVoltageTimes:"<<"StsAlmData: "<<HEX<<StsAlmData<<endl;
			/* end */
			if (CtxAlmData != StsAlmData)//ABC相失压次数是否变化
			{
				if (CAlarmRecrod_ARD15::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
				{
					logvt<<" ABC相失压次数告警产生"<<endl;
					
				}
				res = true;
			}
			if (CtxAlmData != StsAlmData)
			{
				almPhaseData.Save(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData);
			}
		}
		
	}
#endif	
	return res;
}


/*
	brief: A B C相失流次数告警
	param [in] void
	return bool
	author:WanQingsong
	date: 2013-09-13
*/  
bool CAlarm_LoseCurrentTimes::Detect(void)
{
	bool res = false;
#if 0	
	C_AlmPhaseData almPhaseData;
	S_AlmDatas *pCtxAlmDatas =  CONTEXT.GetAlarmDatas();
	S_ALLMP *pALLMP = STATUS.GetALLMP();
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_RegDatas StsRegData;
	S_RegDatas CtxRegData;
	string CtxAlmData;
	string StsAlmData;
	
	for (size_t st=pALLMP->m_MultiMPBS.begin(); st<pALLMP->m_MultiMPBS.end(); st++)
	{
		if (pALLMP->m_MultiMPBS.test(st))
		{
			StsRegData.clear();
			CtxRegData.clear();
			CtxAlmData.clear();
			StsAlmData.clear();

			if (pCtxAlmDatas->Load(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData) ==-1)
			{
				loget<<"CAlarm_LoseCurrentTimes CONTEXT 内存告警数据加载失败!"<<endl;
				continue;
			}
			if (almPhaseData.Load(ALM_COMMID(GetAlarmID()),(S_WORD)st,StsAlmData) ==-1)
			{
				loget<<"CAlarm_LoseCurrentTimes STATUS 内存告警数据加载失败!"<<endl;
				almPhaseData.Save(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData);
				continue;
			}
			if (CAlarmEvtCommIdData::GetCommRegDataFromSTS((INT16U)st,ALM_COMMID(GetAlarmID()),StsRegData) == false)
			{
				loget<<"CAlarm_LoseCurrentTimes GetCommRegDataFromSTS失败!"<<endl;
				continue;
			}
			
			if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,ALM_COMMID(GetAlarmID()),CtxRegData) == false)
			{
				loget<<"CAlarm_LoseCurrentTimes GetCommRegDataFromCTX失败!"<<endl;
				continue;
			}
			 if (StsRegData[0].m_data != CtxRegData[0].m_data)//ABC相失流次数是否变化
			{
				if (CAlarmRecrod_ARD16::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
				{
					logvt<<" ABC相失流次数告警产生"<<endl;
					
				}
				res = true;
			}
			if (CtxAlmData != StsAlmData)
			{
				almPhaseData.Save(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData);
			}
		}
		
	}
#endif	
	return res;
}

/*
	brief: A B C相潮流反向次数告警
	param [in] void
	return bool
	author:WanQingsong
	date: 2013-09-13
*/  
bool CAlarm_CurrentReverseTimes::Detect(void)
{
	bool res = false;
#if 0	
	C_AlmPhaseData almPhaseData;
	S_AlmDatas *pCtxAlmDatas =  CONTEXT.GetAlarmDatas();
	S_ALLMP *pALLMP = STATUS.GetALLMP();
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_RegDatas StsRegData;
	S_RegDatas CtxRegData;
	string CtxAlmData;
	string StsAlmData;
	
	for (size_t st=pALLMP->m_MultiMPBS.begin(); st<pALLMP->m_MultiMPBS.end(); st++)
	{
		if (pALLMP->m_MultiMPBS.test(st))
		{
			StsRegData.clear();
			CtxRegData.clear();
			CtxAlmData.clear();
			StsAlmData.clear();

			if (pCtxAlmDatas->Load(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData) ==-1)
			{
				loget<<"CAlarm_CurrentReverseTimes CONTEXT 内存告警数据加载失败!"<<endl;
				continue;
			}
			if (almPhaseData.Load(ALM_COMMID(GetAlarmID()),(S_WORD)st,StsAlmData) ==-1)
			{
				loget<<"CAlarm_CurrentReverseTimes STATUS 内存告警数据加载失败!"<<endl;
				almPhaseData.Save(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData);
				continue;
			}
			if (CAlarmEvtCommIdData::GetCommRegDataFromSTS((INT16U)st,ALM_COMMID(GetAlarmID()),StsRegData) == false)
			{
				loget<<"CAlarm_CurrentReverseTimes GetCommRegDataFromSTS失败!"<<endl;
				continue;
			}
			
			if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,ALM_COMMID(GetAlarmID()),CtxRegData) == false)
			{
				loget<<"CAlarm_CurrentReverseTimes GetCommRegDataFromCTX失败!"<<endl;
				continue;
			}
			 if (StsRegData[0].m_data != CtxRegData[0].m_data)//ABC相失流次数是否变化
			{
				if (CAlarmRecrod_ARD17::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
				{
					logvt<<" ABC相潮流反向次数告警产生"<<endl;
					
				}
				res = true;
			}
			if (CtxAlmData != StsAlmData)
			{
				almPhaseData.Save(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData);
			}
		}
		
	}
#endif	
	return res;
}



/*
	brief: 电表编程告警
	param [in] void
	return bool
	author:WanQingsong
	date: 2013-09-13
*/  
bool CAlarm_MeterProgramTime::Detect(void)
{
	bool res = false;
	C_AlmPhaseData almPhaseData;
	S_AlmDatas *pCtxAlmDatas =  CONTEXT.GetAlarmDatas();
	S_ALLMP *pALLMP = STATUS.GetALLMP();
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_AlmFlags *pAlmFlags = CONTEXT.GetAlmFlags();
	S_RegDatas StsRegData;
	S_RegDatas CtxRegData;
	string CtxAlmData;
	string StsAlmData;
	
	for (size_t st=pALLMP->m_MultiMPBS.begin(); st<pALLMP->m_MultiMPBS.end(); st++)
	{
		if (pALLMP->m_MultiMPBS.test(st) && pAlmFlags->TestMeter(GetAlarmID(), (S_WORD)st))
		{
			StsRegData.clear();
			CtxRegData.clear();
			CtxAlmData.clear();
			StsAlmData.clear();

			if (pCtxAlmDatas->Load(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData) ==-1)
			{
				loget<<"CAlarm_MeterProgramTime CONTEXT 内存告警数据加载失败!"<<endl;
				continue;
			}
			if (almPhaseData.Load(ALM_COMMID(GetAlarmID()),(S_WORD)st,StsAlmData) ==-1)
			{
				loget<<"CAlarm_MeterProgramTime STATUS 内存告警数据加载失败!"<<endl;
				almPhaseData.Save(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData);
				continue;
			}
			if (CAlarmEvtCommIdData::GetCommRegDataFromSTS((INT16U)st,ALM_COMMID(GetAlarmID()),StsRegData) == false)
			{
				loget<<"CAlarm_MeterProgramTime GetCommRegDataFromSTS失败!"<<endl;
				continue;
			}
			
			if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,ALM_COMMID(GetAlarmID()),CtxRegData) == false)
			{
				loget<<"CAlarm_MeterProgramTime GetCommRegDataFromCTX失败!"<<endl;
				continue;
			}
			/* wqs debug*/
			logvt<<"【ERC 】="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;			
			logvt<<"CAlarm_MeterProgramTime:"<<"StsRegData[0].m_data: "<<HEX<<StsRegData[0].m_data<<endl;
			logvt<<"CtxRegData[0].m_data: "<<HEX<<CtxRegData[0].m_data<<endl;
			/* end */
			
			 if (StsRegData[0].m_data != CtxRegData[0].m_data)//编程次数是否变化
			{
				if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
				{
					logvt<<" 电表编程时间告警产生"<<endl;
					
				}
				res = true;
			}
			if (CtxAlmData != StsAlmData)
			{
				almPhaseData.Save(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData);
			}
		}
		
	}
	return res;
}



/*
	brief: 电表停走告警
	param [in] void
	return bool
	author:WanQingsong
	date: 2013-09-16
*/  
bool CAlarm_MeterStop::Detect(void)
{
	bool res = false;
	S_AlmDatas *pCtxAlmDatas =  CONTEXT.GetAlarmDatas();
	S_ALLMP *pALLMP = STATUS.GetALLMP();
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_TermData *pTermData =  STATUS.GetTermData();
	S_PowerLimit *pMeterException = STATUS.GetPowerLimitInfo();
	S_RegDatas StsRegData;
	S_RegDatas CtxRegData;
	string CtxAlmData;
	S_AlarmStatus *pAlarmStatus = STATUS.GetAlarmStatus();
	S_AlmFlags *pAlmFlags = CONTEXT.GetAlmFlags();
	string StsAlmData;
	C_AlmPhaseData almPhaseData;
	
	for (size_t st=pALLMP->m_MTBS.m_AllMPBS.begin(); st<pALLMP->m_MTBS.m_AllMPBS.end(); st++)
	{
		if (pALLMP->m_MTBS.m_AllMPBS.test(st) && pAlmFlags->TestMeter(GetAlarmID(), (S_WORD)st))
		{
			StsRegData.clear();
			CtxRegData.clear();
			CtxAlmData.clear();
			StsAlmData.clear();

			if (pCtxAlmDatas->Load(REGS_ARD2,(S_WORD)st,CtxAlmData) ==-1)
			{
				loget<<"CAlarm_MeterStop CONTEXT 内存告警数据加载失败!"<<endl;
				continue;
			}
			if (almPhaseData.Load(ALM_COMMID(GetAlarmID()),(S_WORD)st,StsAlmData) ==-1)
			{
				loget<<"CAlarm_MeterStop STATUS 内存告警数据加载失败!"<<endl;
				almPhaseData.Save(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData);
				continue;
			}
			if (CAlarmEvtCommIdData::GetCommRegDataFromSTS((INT16U)st,ALM_COMMID(GetAlarmID()),StsRegData) == false)
			{
				loget<<"CAlarm_MeterStop GetCommRegDataFromSTS失败!"<<endl;
				continue;
			}
			
			if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,REGS_ARD2,CtxRegData) == false)
			{
				loget<<"CAlarm_MeterStop GetCommRegDataFromCTX失败!"<<endl;
				continue;
			}

			/* 以下为正向有功变更判断，及更新变更时间*/
			C_BCD old_value((S_BYTE*)(StsRegData[0].m_data.data()),6,4);
			C_BCD new_value((S_BYTE*)(CtxRegData[0].m_data.data()),6,4);

			double new_dYGpower = C_BusiAlgo::ChangeBcd2Double((S_BYTE*)CtxRegData[19].m_data.data(),2,4,true);
			/* wqs debug*/
			logvt<<"【ERC 】="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;			
			logvt<<"CAlarm_MeterStop:"<<"new_dYGpower: "<<new_dYGpower<<endl;

			S_DWORD Integer;
			S_WORD Float;

			new_value.GetBcdData(Integer, Float);
			logvt<<" zxyg_new_value="<<Integer<<"."<<Float<<endl;
			/* end */
			
			S_TIME t = C_TIME::TimeNow() - pTermData->m_MeterData.m_LastYGPowerChangeTime[st];
			if (pTermData->m_MeterData.m_LastYGPowerChangeTime[st] == 0)
			{
				t = 0;
			}
//			else
//			{
//				t = C_TIME::TimeNow() - pTermData->m_MeterData.m_LastYGPowerChangeTime[st];
//			}
			double dl = (new_dYGpower)*t/3600;//电量
			double dlLimit = C_BusiAlgo::ChangeBcd2Double((S_BYTE*)(&pMeterException->m_MeterStop),2,2,true);

			logvt<<"有功功率 计算的电量值="<<dl<<"，间隔时间="<<t<<",停走阈值="<<dlLimit<<endl;
			
			if (dl > dlLimit) //通过总功率计算出来的电量值大于电量增量阈值
			{
				if (old_value == new_value)//总功率有变化但正相有功无变化
				{
					if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) != ALARMSTATE_CREATE)
					{
						if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
						{
							logvt<<" 电表停走告警产生"<<endl;
							pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_CREATE);
						}
					}
					res = true;
				}
			}

			if (old_value == new_value && pTermData->m_MeterData.m_LastYGPowerChangeTime[st] == 0)//从正向有功无变化开始计时
			{
				S_TIME timeAlmPollCycleS = 0;
				int Precess = CONTEXT.GetDbcjPrecess_MP((S_WORD)st);
				if (Precess < 0) Precess = 0;
				S_DbcjInfo *pDbcjInfo = STATUS.GetDbcjInfo((S_DbcjSN)Precess);
				if (pDbcjInfo != NULL)
					timeAlmPollCycleS = pDbcjInfo->m_AlmPollCycleM * 60;
				pTermData->m_MeterData.m_LastYGPowerChangeTime[st] =  C_TIME::TimeNow() - timeAlmPollCycleS; // 从前一个采表周期开始计算
			}

			if  (old_value != new_value)
			{
				pTermData->m_MeterData.m_LastYGPowerChangeTime[st] = 0;		
			}
			
			if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) == ALARMSTATE_CREATE)
			{
				if (old_value != new_value)//正向有功
				{
//					if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_RESUME, C_TIME::TimeNow()) == true)
					{
						logvt<<" 电表停走告警恢复"<<endl;
						pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_RESUME);
						
					}
				}
			}

			if (StsAlmData != CtxAlmData)
			{
				almPhaseData.Save(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData);
			}
		}
	}
	return res;
}

/*
	brief: 电能表时钟超差告警
	param [in] void
	return bool
	author:WanQingsong
	date: 2013-03-14
*/
bool CAlarm_MeterClockAbnormal::Detect()
{
	S_ALLMP *pAllMP = STATUS.GetALLMP();
	bool ret =false;
	S_RegDatas CtxRegData;
	S_PowerLimit *pMeterException = STATUS.GetPowerLimitInfo();
	char mtime[7]={0};
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_AlarmStatus *pAlarmStatus = STATUS.GetAlarmStatus();
	S_AlmFlags *pAlmFlags = CONTEXT.GetAlmFlags();
	
	for (size_t st=pAllMP->m_MTBS.m_AllMPBS.begin(); st<pAllMP->m_MTBS.m_AllMPBS.end(); st++)
	{

		if (pAllMP->m_MTBS.m_AllMPBS.test(st)==false || pAlmFlags->TestMeter(GetAlarmID(), (S_WORD)st)==false)
		{
			continue;
		}
		
		CtxRegData.clear();

		if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,ALM_COMMID(GetAlarmID()),CtxRegData) == false)
		{
			return false;
		}
		if (pMeterException->m_MeterTimeDiffM== 0)
		{
			loget<<"时钟超差阈值为零，将不进行告警判断!"<<endl;
			continue;
		}
		/* 取电表时间*/
		//memcpy(mtime,PollingData.m_Time_C011,sizeof(PollingData.m_Time_C011));
		//memcpy(mtime+3,PollingData.m_Date_C010,sizeof(PollingData.m_Date_C010));
		memcpy(mtime,CtxRegData[0].m_data.data(),CtxRegData[0].m_data.size());
		memcpy(mtime+3,CtxRegData[1].m_data.data()+1,CtxRegData[1].m_data.size()-1);
		mtime[6] = 0x20;

		/* 电科院台体在测试抄表失败会更改台体模拟电表时间，导致在测抄表失败功能是
		    会产生时钟异常告警，从而导致抄表失败告警失败，为了适应台体，所以增加如果终端
		    与电表的时间不在同一天则不产生时钟异常告警*/
		char timeBcd7[7];
		C_TIME::CurTime2BCDStr_Big(timeBcd7);
		
		if (mtime[3]  != timeBcd7[3])
		{
			continue;
		}

		/* wqs debug */
		logvt<<"【ERC 】="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;		
		logvt<<"meter time = "<<HEX<<mtime[6]<<mtime[5]<<mtime[4]<<mtime[3]<<mtime[2]<<mtime[1]<<mtime[0]<<endl;
		logvt<<"time now = "<<C_TIME::TimeNow()<<endl;
		logvt<<"last time = "<<C_TIME::BcdStr2Time(mtime)<<endl;
		logvt<<"time_dif 1= "<<abs((long)(C_TIME::TimeNow()-C_TIME::BcdStr2Time(mtime)))<<endl; 
		logvt<<"m_MeterTimeDiffM = "<<pMeterException->m_MeterTimeDiffM<<endl; 
		/* end */	
		
		/* 电科院软件在测抄表失败时会改变系统时间，导致产生时钟异常告警,而导致抄表异常告警测试失败。
		   所以为了台体测试通过暂时对系统与表的时间超过3小时则不产生时钟异常告警*/
		if (abs((long)(C_TIME::TimeNow()-C_TIME::BcdStr2Time(mtime))) > 3*60*60)
		{
				continue;
		}	
		/* 终端时间与电表时间误差超过阈值*/
		if (abs((long)(C_TIME::TimeNow()-C_TIME::BcdStr2Time(mtime))) > pMeterException->m_MeterTimeDiffM*60)
		{
			if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) != ALARMSTATE_CREATE)
			{
				logit<<"电表"<<(unsigned int)st<<"时钟超差告警产生"<<endl;
				CAlarmRecrod_ARD13::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow());
				pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_CREATE);
				ret = true;
			}
		}
		else
		{
			if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) == ALARMSTATE_CREATE)
			{
//				if (CAlarmRecrod_ARD13::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_RESUME, C_TIME::TimeNow()) == true)
				{
					logvt<<"电表号"<<(INT16U)st<<"时钟超差告警恢复"<<endl;
					pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_RESUME);
				}
				ret = true;
			}
		}
	}

	return ret;	
}


/*
	brief: 剩余金额告警
	param [in] void
	return bool
	author:WanQingsong
	date: 2013-09-18
*/
bool CAlarm_LackOfMoney::Detect()
{
	S_ALLMP *pAllMP = STATUS.GetALLMP();
	bool ret =false;
	S_RegDatas CtxRegData;
	S_PowerLimit *pMeterException = STATUS.GetPowerLimitInfo();
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_AlarmStatus *pAlarmStatus = STATUS.GetAlarmStatus();
	S_AlmFlags *pAlmFlags = CONTEXT.GetAlmFlags();
	
	for (size_t st=pAllMP->m_MTBS.m_AllMPBS.begin(); st<pAllMP->m_MTBS.m_AllMPBS.end(); st++)
	{
		if (pAllMP->m_MTBS.m_AllMPBS.test(st) == false || pAlmFlags->TestMeter(GetAlarmID(), (S_WORD)st) == false)
		{
			continue;
		}
		
		CtxRegData.clear();

		if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,ALM_COMMID(GetAlarmID()),CtxRegData) == false)
		{
			continue;
		}

		double arrearageLimit = C_BusiAlgo::ChangeBcd2Double((S_BYTE*)(&pMeterException->m_MeterArrearage),4,2,false);
		double remainMoney = C_BusiAlgo::ChangeBcd2Double((S_BYTE*)(CtxRegData[4].m_data.data()),4,2,false);

		if (arrearageLimit > -0.0001 && arrearageLimit < 0.0001)//默认20
		{
			arrearageLimit = (double)20.0;
		}

		/* wqs debug*/
		logvt<<"【ERC 】="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;		
		logvt<<"CAlarm_LackOfMoney:"<<"arrearageLimit: "<<arrearageLimit<<endl;
		logvt<<"CAlarm_LackOfMoney:"<<"remainMoney: "<<remainMoney<<endl;
		/* end */

		if (remainMoney < arrearageLimit)
		{
			if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) != ALARMSTATE_CREATE)
			{
				logit<<"电表"<<(unsigned int)st<<"剩余金额不足告警产生"<<endl;
				CAlarmRecrod_ARD4::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow());
				pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_CREATE);
				ret = true;
			}
		}
		else
		{
			if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) == ALARMSTATE_CREATE)
			{
//				if (CAlarmRecrod_ARD4::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_RESUME, C_TIME::TimeNow()) == true)
				{
					logvt<<"电表号"<<(INT16U)st<<"剩余金额不足告警恢复"<<endl;
					pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_RESUME);
				}
				ret = true;
			}
		}

	}
	return ret;	
}





