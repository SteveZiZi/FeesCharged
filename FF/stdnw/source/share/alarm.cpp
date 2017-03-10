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
	brief: ����ʧ�ܸ澯
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
	/* һ����һ�γ���ʧ�ܸ澯���*/
	if (temp == timeBcd7[3])
	{
		return false;
	}
	
	temp = timeBcd7[3];//��
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
			logvt<<"��ERC ��="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;
			logvt<<"����ʧ�ܿ�ʼʱ��:"<<C_TIME::Time2Ascii14(timeLast, buffer)<<endl;
			logvt<<"��ǰʱ��:"<<C_TIME::Time2Ascii14(timeCurrent, buffer)<<endl;
			logvt<<"����ʧ������:"<<C_TIME::DateDiff(TIME_UNIT_DAY, timeLast, timeCurrent)<<endl;
			/* end */
			
			/* ������ܱ�������û�г��������ݣ�������һ��0�����������ʧ�ܸ澯 */
			if (C_TIME::DateDiff(TIME_UNIT_DAY, timeLast, timeCurrent) > 3)
			{
				m_FailMetersDbs.SetBit((int)st);
				logit<<"���["<<(unsigned int)st<<"]��������ʧ�ܸ澯"<<endl;
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
	brief: A �ೱ������澯
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
				loget<<"CAlarm_CurrentReverse_A CONTEXT �ڴ�澯���ݼ���ʧ��!"<<endl;
				continue;
			}

			if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,ALM_COMMID(GetAlarmID()),CtxRegData) == false)
			{
				loget<<"CAlarm_CurrentReverse_A GetCommRegDataFromCTXʧ��!"<<endl;
				continue;
			}

			
			/* wqs debug*/
			logvt<<"��ERC ��="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;
			logvt<<"CAlarm_CurrentReverse_A CtxRegData[0].m_data: "<<HEX<<CtxRegData[0].m_data<<endl;
			/* end */
			
			/* ���Ժ̨����õ��״̬���ж�*/
			if (CtxRegData[0].m_data.at(0)&0x40)//A�ೱ������
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) != ALARMSTATE_CREATE)
				{
					if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
					{
						logvt<<" A�ೱ������澯����"<<endl;
						pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_CREATE);
						
					}
				}
				res = true;
			}
			else
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) == ALARMSTATE_CREATE)
				{
					if ( st >= 1 && st <= 8)//�ܱ�Ų����ָ��澯
					{
						if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_RESUME, C_TIME::TimeNow()) == true)
						{
							logvt<<" A�ೱ������澯�ָ�"<<endl;
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
	brief: B �ೱ������澯
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
				loget<<"CAlarm_CurrentReverse_B CONTEXT �ڴ�澯���ݼ���ʧ��!"<<endl;
				continue;
			}
			
			if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,ALM_COMMID(GetAlarmID()),CtxRegData) == false)
			{
				loget<<"CAlarm_CurrentReverse_B GetCommRegDataFromCTXʧ��!"<<endl;
				continue;
			}

			/* wqs debug*/
			logvt<<"��ERC ��="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;			
			logvt<<"CAlarm_CurrentReverse_B CtxRegData[0].m_data: "<<HEX<<CtxRegData[0].m_data<<endl;
			/* end */
			
			/* ���Ժ̨����õ��״̬���ж�*/
			if (CtxRegData[0].m_data.at(0)&0x40)//B�ೱ������
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) != ALARMSTATE_CREATE)
				{
					if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
					{
						logvt<<" B�ೱ������澯����"<<endl;
						pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_CREATE);
					}
					res = true;
				}
			}
			else
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) == ALARMSTATE_CREATE)
				{
					if ( st >= 1 && st <= 8)//�ܱ�Ų����ָ��澯
					{
						if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_RESUME, C_TIME::TimeNow()) == true)
						{
							logvt<<" B�ೱ������澯�ָ�"<<endl;
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
	brief: C �ೱ������澯
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
				loget<<"CAlarm_CurrentReverse_C CONTEXT �ڴ�澯���ݼ���ʧ��!"<<endl;
				continue;
			}
			
			if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,ALM_COMMID(GetAlarmID()),CtxRegData) == false)
			{
				loget<<"CAlarm_CurrentReverse_C GetCommRegDataFromCTXʧ��!"<<endl;
				continue;
			}

			/* wqs debug*/
			logvt<<"��ERC ��="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;			
			logvt<<"CAlarm_CurrentReverse_C CtxRegData[0].m_data: "<<HEX<<CtxRegData[0].m_data<<endl;
			/* end */

			/* ���Ժ̨����õ��״̬���ж�*/
			if (CtxRegData[0].m_data.at(0)&0x40)//C�ೱ������
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) != ALARMSTATE_CREATE)
				{
					if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
					{
						logvt<<" C�ೱ������澯����"<<endl;
						pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_CREATE);
					}
					res = true;
				}	
			}
			else
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) == ALARMSTATE_CREATE)
				{
					if ( st >= 1 && st <= 8)//�ܱ�Ų����ָ��澯
					{
						if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_RESUME, C_TIME::TimeNow()) == true)
						{
							logvt<<" C�ೱ������澯�ָ�"<<endl;
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
	brief: A ��ʧ���澯
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
				loget<<"CAlarm_LoseCurrent_A CONTEXT �ڴ�澯���ݼ���ʧ��!"<<endl;
				continue;
			}
			
			if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,ALM_COMMID(GetAlarmID()),CtxRegData) == false)
			{
				loget<<"CAlarm_LoseCurrent_A GetCommRegDataFromCTXʧ��!"<<endl;
				continue;
			}

			/* wqs debug*/
			logvt<<"��ERC ��="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;			
			logvt<<"CAlarm_LoseCurrent_A CtxRegData[0].m_data: "<<HEX<<CtxRegData[0].m_data<<endl;
			/* end */

/*			 if (StsRegData[0].m_data != CtxRegData[0].m_data)//A��ʧѹ�����Ƿ�仯
			{
				if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
				{
					logvt<<" A��ʧ���澯����"<<endl;
					
				}
				res = true;
			}*/
			/* ���Ժ̨����õ��״̬���ж�*/
			if (CtxRegData[0].m_data.at(0)&0x08)//A��ʧ��
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) != ALARMSTATE_CREATE)
				{
					if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
					{
						logvt<<" A��ʧ���澯����"<<endl;
						pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_CREATE);
					}
					res = true;
				}
			}
			else
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) == ALARMSTATE_CREATE)
				{
					if ( st >= 1 && st <= 8)//�ܱ�Ų����ָ��澯
					{
						if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_RESUME, C_TIME::TimeNow()) == true)
						{
							logvt<<" A��ʧ���澯�ָ�"<<endl;
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
	brief: B ��ʧ���澯
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
				loget<<"CAlarm_LoseCurrent_B CONTEXT �ڴ�澯���ݼ���ʧ��!"<<endl;
				continue;
			}
			if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,ALM_COMMID(GetAlarmID()),CtxRegData) == false)
			{
				loget<<"CAlarm_LoseCurrent_B GetCommRegDataFromCTXʧ��!"<<endl;
				continue;
			}

			/* wqs debug*/
			logvt<<"��ERC ��="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;
			logvt<<"CAlarm_LoseCurrent_B CtxRegData[0].m_data: "<<HEX<<CtxRegData[0].m_data<<endl;
			/* end */
			
			/* ���Ժ̨����õ��״̬���ж�*/
			if (CtxRegData[0].m_data.at(0)&0x08)//B��ʧ��
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) != ALARMSTATE_CREATE)
				{
					if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
					{
						logvt<<" B��ʧ���澯����"<<endl;
						pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_CREATE);
						
					}
					res = true;
				}
			}
			else
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) == ALARMSTATE_CREATE)
				{
					if ( st >= 1 && st <= 8)//�ܱ�Ų����ָ��澯
					{
						if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_RESUME, C_TIME::TimeNow()) == true)
						{
							logvt<<" B��ʧ���澯�ָ�"<<endl;
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
	brief: C ��ʧ���澯
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
				loget<<"CAlarm_LoseCurrent_C CONTEXT �ڴ�澯���ݼ���ʧ��!"<<endl;
				continue;
			}
			
			if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,ALM_COMMID(GetAlarmID()),CtxRegData) == false)
			{
				loget<<"CAlarm_LoseCurrent_C GetCommRegDataFromCTXʧ��!"<<endl;
				continue;
			}

			/* wqs debug*/
			logvt<<"��ERC ��="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;			
			logvt<<"CAlarm_LoseCurrent_C CtxRegData[0].m_data: "<<HEX<<CtxRegData[0].m_data<<endl;
			/* end */
			
			/* ���Ժ̨����õ��״̬���ж�*/
			if (CtxRegData[0].m_data.at(0)&0x08)//B��ʧ��
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) != ALARMSTATE_CREATE)
				{
					if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
					{
						logvt<<" C��ʧ���澯����"<<endl;
						pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_CREATE);
					}
					res = true;
				}
			}
			else
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) == ALARMSTATE_CREATE)
				{
					if ( st >= 1 && st <= 8)//�ܱ�Ų����ָ��澯
					{
						if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_RESUME, C_TIME::TimeNow()) == true)
						{
							logvt<<" C��ʧ���澯�ָ�"<<endl;
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
	brief: A ��ʧѹ�澯
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
				loget<<"CAlarm_LoseVoltage_A CONTEXT �ڴ�澯���ݼ���ʧ��!"<<endl;
				continue;
			}
			
			if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,ALM_COMMID(GetAlarmID()),CtxRegData) == false)
			{
				loget<<"CAlarm_LoseVoltage_A GetCommRegDataFromCTXʧ��!"<<endl;
				continue;
			}

			/* wqs debug*/
			logvt<<"��ERC ��="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;			
			logvt<<"CAlarm_LoseVoltage_A CtxRegData[0].m_data: "<<HEX<<CtxRegData[0].m_data<<endl;
			/* end */
			
			/* ���Ժ̨����õ��״̬���ж�*/
			if (CtxRegData[0].m_data.at(0)&0x01)//A��ʧѹ
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) != ALARMSTATE_CREATE)
				{
					if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
					{
						logvt<<" A��ʧѹ�澯����"<<endl;
						pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_CREATE);
					}
					res = true;
				}
			}
			else
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) == ALARMSTATE_CREATE)
				{
					if ( st >= 1 && st <= 8)//�ܱ�Ų����ָ��澯
					{
						if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_RESUME, C_TIME::TimeNow()) == true)
						{
							logvt<<" A��ʧѹ�澯�ָ�"<<endl;
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
	brief: B ��ʧѹ�澯
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
				loget<<"CAlarm_LoseVoltage_B CONTEXT �ڴ�澯���ݼ���ʧ��!"<<endl;
				continue;
			}
			
			if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,ALM_COMMID(GetAlarmID()),CtxRegData) == false)
			{
				loget<<"CAlarm_LoseVoltage_B GetCommRegDataFromCTXʧ��!"<<endl;
				continue;
			}

			/* wqs debug*/
			logvt<<"��ERC ��="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;			
			logvt<<"CAlarm_LoseVoltage_B CtxRegData[0].m_data: "<<HEX<<CtxRegData[0].m_data<<endl;
			/* end */
			
			/* ���Ժ̨����õ��״̬���ж�*/
			if (CtxRegData[0].m_data.at(0)&0x01)//B��ʧѹ
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) != ALARMSTATE_CREATE)
				{
					if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
					{
						logvt<<" B��ʧѹ�澯����"<<endl;
						pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_CREATE);
					}
					res = true;
				}
			}
			else
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) == ALARMSTATE_CREATE)
				{
					if ( st >= 1 && st <= 8)//�ܱ�Ų����ָ��澯
					{
						if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_RESUME, C_TIME::TimeNow()) == true)
						{
							logvt<<" B��ʧѹ�澯�ָ�"<<endl;
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
	brief: C ��ʧѹ�澯
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
				loget<<"CAlarm_LoseVoltage_C CONTEXT �ڴ�澯���ݼ���ʧ��!"<<endl;
				continue;
			}
			
			if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,ALM_COMMID(GetAlarmID()),CtxRegData) == false)
			{
				loget<<"CAlarm_LoseVoltage_C GetCommRegDataFromCTXʧ��!"<<endl;
				continue;
			}

			/* wqs debug*/
			logvt<<"��ERC ��="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;			
			logvt<<"CAlarm_LoseVoltage_C CtxRegData[0].m_data: "<<HEX<<CtxRegData[0].m_data<<endl;
			/* end */
			
			/* ���Ժ̨����õ��״̬���ж�*/
			if (CtxRegData[0].m_data.at(0)&0x01)//C��ʧѹ
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) != ALARMSTATE_CREATE)
				{
					if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
					{
						logvt<<" C��ʧѹ�澯����"<<endl;
						pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_CREATE);
					}
					res = true;
				}
			}
			else
			{
				if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) == ALARMSTATE_CREATE)
				{
					if ( st >= 1 && st <= 8)//�ܱ�Ų����ָ��澯
					{
						if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_RESUME, C_TIME::TimeNow()) == true)
						{
							logvt<<" C��ʧѹ�澯�ָ�"<<endl;
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
	brief: ���ܱ�ʾ���½�
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
				loget<<"CAlarm_MeterDegreeDec CONTEXT �ڴ�澯���ݼ���ʧ��!"<<endl;
				continue;
			}
			if (almPhaseData.Load(ALM_COMMID(GetAlarmID()),(S_WORD)st,StsAlmData) ==-1)
			{
				loget<<"CAlarm_MeterDegreeDec STATUS �ڴ�澯���ݼ���ʧ��!"<<endl;
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
			/* �����й�*/
			C_BCD zxyg_old_value((S_BYTE*)(StsRegData[0].m_data.data()),6,4);
			C_BCD zxyg_new_value((S_BYTE*)(CtxRegData[0].m_data.data()),6,4);
			/* wqs debug */
			S_DWORD Integer;
			S_WORD Float;
			logvt<<"��ERC ��="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;
			zxyg_old_value.GetBcdData(Integer, Float);
			logvt<<" zxyg_old_value="<<Integer<<"."<<Float<<endl;
			zxyg_new_value.GetBcdData(Integer, Float);
			logvt<<" zxyg_new_value="<<Integer<<"."<<Float<<endl;
			/* end */


			/* �����й�*/
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
				logvt<<"����"<<(INT16U)st<<"ʾ���½�"<<endl;
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
	brief: ���ʱ�λ���ʱ��
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
				loget<<"CAlarm_PeriodTariffChange CONTEXT �ڴ�澯���ݼ���ʧ��!"<<endl;
				continue;
			}
			if (almPhaseData.Load(ALM_COMMID(GetAlarmID()),(S_WORD)st,StsAlmData) ==-1)
			{
				loget<<"CAlarm_PeriodTariffChange STATUS �ڴ�澯���ݼ���ʧ��!"<<endl;
				almPhaseData.Save(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData);
				continue;
			}
			if (CAlarmEvtCommIdData::GetCommRegDataFromSTS((INT16U)st,ALM_COMMID(GetAlarmID()),StsRegData) == false)
			{
				loget<<"CAlarm_PeriodTariffChange GetCommRegDataFromSTSʧ��!"<<endl;
				continue;
			}
			
			if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,ALM_COMMID(GetAlarmID()),CtxRegData) == false)
			{
				loget<<"CAlarm_PeriodTariffChange GetCommRegDataFromCTXʧ��!"<<endl;			
				continue;
			}
			/* wqs debug*/
			logvt<<"��ERC ��="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;			
			logvt<<"CAlarm_PeriodTariffChange:"<<"CtxRegData[0].m_data: "<<HEX<<CtxRegData[0].m_data<<endl;

			/* end */
			
			if (StsRegData[0].m_data != CtxRegData[0].m_data
			    ||StsRegData[1].m_data != CtxRegData[1].m_data
		    	    ||StsRegData[2].m_data != CtxRegData[2].m_data
			    ||StsRegData[3].m_data != CtxRegData[3].m_data)       
			{
				if (CAlarmRecrod_ARD1::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
				{
					logvt<<" ��"<<(INT16U)st<<"ʱ�η��ʱ���澯����"<<endl;
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

	meter.m_MeasureNo = 0;//�ն��¼�

	if (CAlarmRecrod_ARD2::AlarmRecord(meter,ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
	{
		logvt<<" �ն˵���澯����"<<endl;
		res = true;
	}

	return res;
}

bool CAlarm_TerminalPowerOn::Detect(void)
{
	bool res = false;
	S_MeterInfo meter;

	meter.m_MeasureNo = 0;//�ն��¼�

	if (CAlarmRecrod_ARD2::AlarmRecord(meter,ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
	{
		logvt<<" �ն��ϵ�澯����"<<endl;
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

	meter.m_MeasureNo = 0;//�ն��¼�

	/* wqs debug*/
	if (pHttxInfo->m_MonTrafficLimit != 0)
	{ // ��Ҫ�澯�Ŵ�ӡͳ����Ϣ
		logvt<<"��ERC ��="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",������="<<STATUS.GetMonCommFlux()<<",��������ֵ="<<pHttxInfo->m_MonTrafficLimit<<endl;
	}
	/* end */

	C_TIME::CurTime2BCDStr_Big(timeBcd7);
	
	if (bHappen == true)
	{
		if ((pHttxInfo->m_MonTrafficLimit == 0) || (STATUS.GetMonCommFlux() < pHttxInfo->m_MonTrafficLimit))
		{
//			if (CAlarmRecrod_ARD10::AlarmRecord(meter,ALM_COMMID(GetAlarmID()),ALARMSTATE_RESUME, C_TIME::TimeNow(),STATUS.GetMonCommFlux()) == true)
			{
				logvt<<" ������Խ�޸澯�ָ�"<<endl;
				res = true;
				bHappen = false;
			}
		}

		/*������Խ�޲����Ժ���������Խ��δ�ָ��� һ��ֻ����һ��������Խ�޸澯*/
		if (temp == timeBcd7[3])
		{
			return false;
		}
	}

	temp = timeBcd7[3];//��
	
	if ((STATUS.GetMonCommFlux() > pHttxInfo->m_MonTrafficLimit) &&  (pHttxInfo->m_MonTrafficLimit > 0))	
	{
		if (CAlarmRecrod_ARD10::AlarmRecord(meter,ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow(),STATUS.GetMonCommFlux()) == true)
		{
			logvt<<" ������Խ�޸澯����"<<endl;
			res = true;
			bHappen = true;
		}
	}
	
	return res;
}


/*
	brief: �̵�����λ�澯
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
				loget<<"CAlarm_RelayChange CONTEXT �ڴ�澯���ݼ���ʧ��!"<<endl;
				continue;
			}
			if (almPhaseData.Load(ALM_COMMID(GetAlarmID()),(S_WORD)st,StsAlmData) ==-1)
			{
				loget<<"CAlarm_RelayChange STATUS �ڴ�澯���ݼ���ʧ��!"<<endl;
				almPhaseData.Save(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData);
				continue;
			}
			if (CAlarmEvtCommIdData::GetCommRegDataFromSTS((INT16U)st,ALM_COMMID(GetAlarmID()),StsRegData) == false)
			{
				loget<<"CAlarm_RelayChange GetCommRegDataFromSTSʧ��!"<<endl;
				continue;
			}

			IPCSHAREMEM.m_IpcDevicesRegData[st].m_RelayState = StsRegData[0].m_data[0] & 0x10?1:0;
			
			if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,ALM_COMMID(GetAlarmID()),CtxRegData) == false)
			{
				loget<<"CAlarm_RelayChange GetCommRegDataFromCTXʧ��!"<<endl;
				continue;
			}
			/* wqs debug*/
			logvt<<"��ERC ��="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;
			logvt<<"CAlarm_RelayChange:"<<"CtxRegData[0].m_data: "<<HEX<<CtxRegData[0].m_data<<endl;
			/* end */
			 if ((CtxRegData[0].m_data[0] & 0x10) != (StsRegData[0].m_data[0] & 0x10))//�������״̬��3��bit4Ϊ�̵���״̬λ�����ı�
			{
				//if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) != ALARMSTATE_CREATE)
				{
					if (CAlarmRecrod_ARD12::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
					{
						logvt<<" ��"<<(INT16U)st<<"�̵�����λ�澯����"<<endl;
						
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
	brief: A B C��ʧѹ�����澯
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
				loget<<"CAlarm_LoseVoltageTimes:CONTEXT �ڴ�澯���ݼ���ʧ��!"<<endl;
				continue;
			}

			if (almPhaseData.Load(ALM_COMMID(GetAlarmID()),(S_WORD)st,StsAlmData) ==-1)
			{
				loget<<"CAlarm_LoseVoltageTimes:STATUS �ڴ�澯���ݼ���ʧ��!"<<endl;
				almPhaseData.Save(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData);
				continue;
			}
			/* wqs debug*/
			logvt<<"��ERC ��="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<endl;			
			logvt<<"CAlarm_LoseVoltageTimes:"<<"CtxAlmData: "<<HEX<<CtxAlmData<<endl;
			logvt<<"CAlarm_LoseVoltageTimes:"<<"StsAlmData: "<<HEX<<StsAlmData<<endl;
			/* end */
			if (CtxAlmData != StsAlmData)//ABC��ʧѹ�����Ƿ�仯
			{
				if (CAlarmRecrod_ARD15::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
				{
					logvt<<" ABC��ʧѹ�����澯����"<<endl;
					
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
	brief: A B C��ʧ�������澯
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
				loget<<"CAlarm_LoseCurrentTimes CONTEXT �ڴ�澯���ݼ���ʧ��!"<<endl;
				continue;
			}
			if (almPhaseData.Load(ALM_COMMID(GetAlarmID()),(S_WORD)st,StsAlmData) ==-1)
			{
				loget<<"CAlarm_LoseCurrentTimes STATUS �ڴ�澯���ݼ���ʧ��!"<<endl;
				almPhaseData.Save(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData);
				continue;
			}
			if (CAlarmEvtCommIdData::GetCommRegDataFromSTS((INT16U)st,ALM_COMMID(GetAlarmID()),StsRegData) == false)
			{
				loget<<"CAlarm_LoseCurrentTimes GetCommRegDataFromSTSʧ��!"<<endl;
				continue;
			}
			
			if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,ALM_COMMID(GetAlarmID()),CtxRegData) == false)
			{
				loget<<"CAlarm_LoseCurrentTimes GetCommRegDataFromCTXʧ��!"<<endl;
				continue;
			}
			 if (StsRegData[0].m_data != CtxRegData[0].m_data)//ABC��ʧ�������Ƿ�仯
			{
				if (CAlarmRecrod_ARD16::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
				{
					logvt<<" ABC��ʧ�������澯����"<<endl;
					
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
	brief: A B C�ೱ����������澯
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
				loget<<"CAlarm_CurrentReverseTimes CONTEXT �ڴ�澯���ݼ���ʧ��!"<<endl;
				continue;
			}
			if (almPhaseData.Load(ALM_COMMID(GetAlarmID()),(S_WORD)st,StsAlmData) ==-1)
			{
				loget<<"CAlarm_CurrentReverseTimes STATUS �ڴ�澯���ݼ���ʧ��!"<<endl;
				almPhaseData.Save(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData);
				continue;
			}
			if (CAlarmEvtCommIdData::GetCommRegDataFromSTS((INT16U)st,ALM_COMMID(GetAlarmID()),StsRegData) == false)
			{
				loget<<"CAlarm_CurrentReverseTimes GetCommRegDataFromSTSʧ��!"<<endl;
				continue;
			}
			
			if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,ALM_COMMID(GetAlarmID()),CtxRegData) == false)
			{
				loget<<"CAlarm_CurrentReverseTimes GetCommRegDataFromCTXʧ��!"<<endl;
				continue;
			}
			 if (StsRegData[0].m_data != CtxRegData[0].m_data)//ABC��ʧ�������Ƿ�仯
			{
				if (CAlarmRecrod_ARD17::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
				{
					logvt<<" ABC�ೱ����������澯����"<<endl;
					
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
	brief: ����̸澯
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
				loget<<"CAlarm_MeterProgramTime CONTEXT �ڴ�澯���ݼ���ʧ��!"<<endl;
				continue;
			}
			if (almPhaseData.Load(ALM_COMMID(GetAlarmID()),(S_WORD)st,StsAlmData) ==-1)
			{
				loget<<"CAlarm_MeterProgramTime STATUS �ڴ�澯���ݼ���ʧ��!"<<endl;
				almPhaseData.Save(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData);
				continue;
			}
			if (CAlarmEvtCommIdData::GetCommRegDataFromSTS((INT16U)st,ALM_COMMID(GetAlarmID()),StsRegData) == false)
			{
				loget<<"CAlarm_MeterProgramTime GetCommRegDataFromSTSʧ��!"<<endl;
				continue;
			}
			
			if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,ALM_COMMID(GetAlarmID()),CtxRegData) == false)
			{
				loget<<"CAlarm_MeterProgramTime GetCommRegDataFromCTXʧ��!"<<endl;
				continue;
			}
			/* wqs debug*/
			logvt<<"��ERC ��="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;			
			logvt<<"CAlarm_MeterProgramTime:"<<"StsRegData[0].m_data: "<<HEX<<StsRegData[0].m_data<<endl;
			logvt<<"CtxRegData[0].m_data: "<<HEX<<CtxRegData[0].m_data<<endl;
			/* end */
			
			 if (StsRegData[0].m_data != CtxRegData[0].m_data)//��̴����Ƿ�仯
			{
				if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
				{
					logvt<<" �����ʱ��澯����"<<endl;
					
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
	brief: ���ͣ�߸澯
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
				loget<<"CAlarm_MeterStop CONTEXT �ڴ�澯���ݼ���ʧ��!"<<endl;
				continue;
			}
			if (almPhaseData.Load(ALM_COMMID(GetAlarmID()),(S_WORD)st,StsAlmData) ==-1)
			{
				loget<<"CAlarm_MeterStop STATUS �ڴ�澯���ݼ���ʧ��!"<<endl;
				almPhaseData.Save(ALM_COMMID(GetAlarmID()),(S_WORD)st,CtxAlmData);
				continue;
			}
			if (CAlarmEvtCommIdData::GetCommRegDataFromSTS((INT16U)st,ALM_COMMID(GetAlarmID()),StsRegData) == false)
			{
				loget<<"CAlarm_MeterStop GetCommRegDataFromSTSʧ��!"<<endl;
				continue;
			}
			
			if (CAlarmEvtCommIdData::GetCommRegDataFromCTX((INT16U)st,REGS_ARD2,CtxRegData) == false)
			{
				loget<<"CAlarm_MeterStop GetCommRegDataFromCTXʧ��!"<<endl;
				continue;
			}

			/* ����Ϊ�����й�����жϣ������±��ʱ��*/
			C_BCD old_value((S_BYTE*)(StsRegData[0].m_data.data()),6,4);
			C_BCD new_value((S_BYTE*)(CtxRegData[0].m_data.data()),6,4);

			double new_dYGpower = C_BusiAlgo::ChangeBcd2Double((S_BYTE*)CtxRegData[19].m_data.data(),2,4,true);
			/* wqs debug*/
			logvt<<"��ERC ��="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;			
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
			double dl = (new_dYGpower)*t/3600;//����
			double dlLimit = C_BusiAlgo::ChangeBcd2Double((S_BYTE*)(&pMeterException->m_MeterStop),2,2,true);

			logvt<<"�й����� ����ĵ���ֵ="<<dl<<"�����ʱ��="<<t<<",ͣ����ֵ="<<dlLimit<<endl;
			
			if (dl > dlLimit) //ͨ���ܹ��ʼ�������ĵ���ֵ���ڵ���������ֵ
			{
				if (old_value == new_value)//�ܹ����б仯�������й��ޱ仯
				{
					if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) != ALARMSTATE_CREATE)
					{
						if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_CREATE, C_TIME::TimeNow()) == true)
						{
							logvt<<" ���ͣ�߸澯����"<<endl;
							pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_CREATE);
						}
					}
					res = true;
				}
			}

			if (old_value == new_value && pTermData->m_MeterData.m_LastYGPowerChangeTime[st] == 0)//�������й��ޱ仯��ʼ��ʱ
			{
				S_TIME timeAlmPollCycleS = 0;
				int Precess = CONTEXT.GetDbcjPrecess_MP((S_WORD)st);
				if (Precess < 0) Precess = 0;
				S_DbcjInfo *pDbcjInfo = STATUS.GetDbcjInfo((S_DbcjSN)Precess);
				if (pDbcjInfo != NULL)
					timeAlmPollCycleS = pDbcjInfo->m_AlmPollCycleM * 60;
				pTermData->m_MeterData.m_LastYGPowerChangeTime[st] =  C_TIME::TimeNow() - timeAlmPollCycleS; // ��ǰһ���ɱ����ڿ�ʼ����
			}

			if  (old_value != new_value)
			{
				pTermData->m_MeterData.m_LastYGPowerChangeTime[st] = 0;		
			}
			
			if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) == ALARMSTATE_CREATE)
			{
				if (old_value != new_value)//�����й�
				{
//					if (CAlarmRecrod_ARD2::AlarmRecord(pMeterInfos->m_Item[st],ALM_COMMID(GetAlarmID()),ALARMSTATE_RESUME, C_TIME::TimeNow()) == true)
					{
						logvt<<" ���ͣ�߸澯�ָ�"<<endl;
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
	brief: ���ܱ�ʱ�ӳ���澯
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
			loget<<"ʱ�ӳ�����ֵΪ�㣬�������и澯�ж�!"<<endl;
			continue;
		}
		/* ȡ���ʱ��*/
		//memcpy(mtime,PollingData.m_Time_C011,sizeof(PollingData.m_Time_C011));
		//memcpy(mtime+3,PollingData.m_Date_C010,sizeof(PollingData.m_Date_C010));
		memcpy(mtime,CtxRegData[0].m_data.data(),CtxRegData[0].m_data.size());
		memcpy(mtime+3,CtxRegData[1].m_data.data()+1,CtxRegData[1].m_data.size()-1);
		mtime[6] = 0x20;

		/* ���Ժ̨���ڲ��Գ���ʧ�ܻ����̨��ģ����ʱ�䣬�����ڲⳭ��ʧ�ܹ�����
		    �����ʱ���쳣�澯���Ӷ����³���ʧ�ܸ澯ʧ�ܣ�Ϊ����Ӧ̨�壬������������ն�
		    �����ʱ�䲻��ͬһ���򲻲���ʱ���쳣�澯*/
		char timeBcd7[7];
		C_TIME::CurTime2BCDStr_Big(timeBcd7);
		
		if (mtime[3]  != timeBcd7[3])
		{
			continue;
		}

		/* wqs debug */
		logvt<<"��ERC ��="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;		
		logvt<<"meter time = "<<HEX<<mtime[6]<<mtime[5]<<mtime[4]<<mtime[3]<<mtime[2]<<mtime[1]<<mtime[0]<<endl;
		logvt<<"time now = "<<C_TIME::TimeNow()<<endl;
		logvt<<"last time = "<<C_TIME::BcdStr2Time(mtime)<<endl;
		logvt<<"time_dif 1= "<<abs((long)(C_TIME::TimeNow()-C_TIME::BcdStr2Time(mtime)))<<endl; 
		logvt<<"m_MeterTimeDiffM = "<<pMeterException->m_MeterTimeDiffM<<endl; 
		/* end */	
		
		/* ���Ժ����ڲⳭ��ʧ��ʱ��ı�ϵͳʱ�䣬���²���ʱ���쳣�澯,�����³����쳣�澯����ʧ�ܡ�
		   ����Ϊ��̨�����ͨ����ʱ��ϵͳ����ʱ�䳬��3Сʱ�򲻲���ʱ���쳣�澯*/
		if (abs((long)(C_TIME::TimeNow()-C_TIME::BcdStr2Time(mtime))) > 3*60*60)
		{
				continue;
		}	
		/* �ն�ʱ������ʱ��������ֵ*/
		if (abs((long)(C_TIME::TimeNow()-C_TIME::BcdStr2Time(mtime))) > pMeterException->m_MeterTimeDiffM*60)
		{
			if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) != ALARMSTATE_CREATE)
			{
				logit<<"���"<<(unsigned int)st<<"ʱ�ӳ���澯����"<<endl;
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
					logvt<<"����"<<(INT16U)st<<"ʱ�ӳ���澯�ָ�"<<endl;
					pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_RESUME);
				}
				ret = true;
			}
		}
	}

	return ret;	
}


/*
	brief: ʣ����澯
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

		if (arrearageLimit > -0.0001 && arrearageLimit < 0.0001)//Ĭ��20
		{
			arrearageLimit = (double)20.0;
		}

		/* wqs debug*/
		logvt<<"��ERC ��="<<HEX<<ALM_COMMID(GetAlarmID())<<",INDEX = "<<dec<<GetAlarmID()<<",MP = "<<(S_WORD)st<<endl;		
		logvt<<"CAlarm_LackOfMoney:"<<"arrearageLimit: "<<arrearageLimit<<endl;
		logvt<<"CAlarm_LackOfMoney:"<<"remainMoney: "<<remainMoney<<endl;
		/* end */

		if (remainMoney < arrearageLimit)
		{
			if (pAlarmStatus->m_MpAlarmStates[st].GetAlarmState(GetAlarmID()) != ALARMSTATE_CREATE)
			{
				logit<<"���"<<(unsigned int)st<<"ʣ�����澯����"<<endl;
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
					logvt<<"����"<<(INT16U)st<<"ʣ�����澯�ָ�"<<endl;
					pAlarmStatus->m_MpAlarmStates[st].SetAlarmState(GetAlarmID(),(INT8U)ALARMSTATE_RESUME);
				}
				ret = true;
			}
		}

	}
	return ret;	
}





