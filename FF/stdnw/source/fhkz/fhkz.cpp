/** @file
 *  @brief 负荷控制功能定义
 *  @author 万青松
 *  @date 2012/03/01
 *  @version 0.1
 */
#include "fhkz.h"
#include "status.h"
#include "context.h"
#include "almfact.h"
#include "alarm.h"
#include "context.h"
#include "AlarmEventRecord.h"
#include "metercom.h"
#include "cfg_mp.h"
#include "ipcSharemem.h"

int C_FhkzServer::BeforeService(void)
{
	C_MeterCom MeterCom;
	MeterCom.SetFillChar((char)0xFF);
	return 0;
}

int C_FhkzServer::AfterService(void)
{
	return 0;
}

int C_FhkzServer::OnTickService(void)
{
	static S_TIME tickMinute30S = 0;
	static S_TIME tickMinute10S = 0;	
	static S_TIME tickMinute5S = 0;	
	static S_TIME tickMinute2S = 0;	
	C_ContextBase *pContextBase = C_ShmFact::GetContextBase();
	S_TIME tickNow = pContextBase->GetSystemTick();

	if(tickMinute2S == 0 || tickNow/2 != tickMinute2S)
	{
		CheckTask2S(), tickMinute2S = tickNow/2;
	}
	if(tickMinute5S == 0 || tickNow/5 != tickMinute5S)
	{
		CheckTask5S(), tickMinute5S = tickNow/5;
	}
	if(tickMinute10S == 0 || tickNow/10 != tickMinute10S)
	{
		CheckTask10S(), tickMinute10S = tickNow/10;
	}
	if(tickMinute30S == 0 || tickNow/30 != tickMinute30S)
	{
		CheckTask30S(), tickMinute30S = tickNow/30;
	}
	return 0;
}

/** @brief 检查2秒任务 */
int C_FhkzServer::CheckTask2S(void)
{
	return 0;
}

/** @brief 检查5秒任务 */
int C_FhkzServer::CheckTask5S(void)
{
	return 0;
}

/** @brief 检查10秒任务 */
int C_FhkzServer::CheckTask10S(void)
{
	AlmEvtHandle();
	return 0;
}

/** @brief 检查30秒任务 */
int C_FhkzServer::CheckTask30S(void)
{
	HistoryDayDataFill();
	HistoryMonthDataFill();
	return 0;
}

int C_FhkzServer::AlmEvtHandle(void)
{
	S_AlarmInfo *pAlarmInfo = STATUS.GetAlarmInfo();
	S_AlmFlags *pAlmFlags = CONTEXT.GetAlmFlags();
	
	VirtualAlmTaskFinish();
	for (unsigned int i=0; i< pAlarmInfo->m_AlarmEven.size(); i++)
	{
		if (pAlarmInfo->m_AlarmEven.test(i) == false 
			|| C_AlarmDetect::SupportDetectMethod((INT8U)i,ALM_DETECT_METHOD_REALTIME) == false
			)
		{
			continue;
		}

		CAlarm *pAlarm = C_AlarmFactory::GetAlarmEventObj((INT8U)i);
		if (pAlarm == NULL)
			continue;

		//除抄表失败和月流量越线告警外，其他告警都要先判断相应告警是否采集完成再进行告警检测
		if (i != ALARM_READMETER_FAIL 
			&& i !=ALARM_MONTH_FLOW_OVERRANGE
			&& pAlmFlags->TestTask((INT8U)i) == false)
		{
			continue;
		}

		pAlarm->Detect();

		pAlmFlags->ResetTask((INT8U)i);
		
	}
	
	return 0;
}

int C_FhkzServer::VirtualAlmTaskFinish(void)
{
	S_MSDI msdi = STATUS.FindMasterStationDataID(CLASSDATA_3, REGS_ARD2, 0x00);
	if (msdi.m_Enable == false)
		return 0;
	if (msdi.m_MSDI >= ALM_FLAGS_ITEM_MAX)
		return 0;

	S_AlmFlags *pAlmFlags = CONTEXT.GetAlmFlags();
	S_AlmFlag &AlmFlag = pAlmFlags->m_itemAlm[msdi.m_MSDI];
	if (AlmFlag.m_fTaskFinish == false)
		return 0;

	AlmFlag.m_fTaskFinish = false;
	pAlmFlags->SetTask(ALARM_DEGREE_DECREASE, AlmFlag.m_mpbsSuccess); // 电表示度下降告警
	pAlmFlags->SetTask(ALARM_METER_STOP, AlmFlag.m_mpbsSuccess); // 电表停走告警
	AlmFlag.m_mpbsSuccess.reset();
	return 0;
}

bool C_FhkzServer::FillDayData(S_WORD MP, S_DWORD FN, S_TIME DataTime, char *pBuffer)
{
	string strData1, strData2;

	C_Class2Data::Read(FN, MP, strData1, DataTime);
	memcpy(pBuffer,strData1.data(),strData1.size());
	return true;
}

void C_FhkzServer::HistoryDayDataFill()
{
	S_TIME mytime;
	char data[4];
	S_ALLMP *pALLMP = STATUS.GetALLMP();
	
	for (int i=-1,j=0; i>=-7; i--)
	{
	 	mytime = C_TIME::DateAdd(TIME_UNIT_DAY, i, C_TIME::TimeNow());
		for (size_t st=pALLMP->m_MTBS.m_AllMPBS.begin(); st<pALLMP->m_MTBS.m_AllMPBS.end(); st++)
		{
			if (pALLMP->m_MTBS.m_AllMPBS.test(st))
			{
				logvt <<"day data mp="<<dec<<st<<",date="<<mytime<<endl;
				if (FillDayData(st, 0x05060100, mytime, data) == true)
				{
					memcpy(&(IPCSHAREMEM.m_IpcDevicesRegData[st].m_Reg05060000[j][0]),data,sizeof(data));
				}
				else
				{
					memset(&(IPCSHAREMEM.m_IpcDevicesRegData[st].m_Reg05060000[j][0]),0xff,sizeof(data));
				}
			}
		}	
	}
		
}

bool C_FhkzServer::FillMonthData(S_WORD MP, S_DWORD FN, S_TIME DataTime, char *pBuffer)
{
	string strData1, strData2;

	C_Class2Data::Read(FN, MP, strData1, DataTime);
	memcpy(pBuffer,strData1.data(),strData1.size());
	return true;
}

void C_FhkzServer::HistoryMonthDataFill()
{
	S_TIME mytime;
	char data[4];
	S_ALLMP *pALLMP = STATUS.GetALLMP();
	
	for (int i=-1,j=0; i>=-6; i--)
	{
	 	mytime = C_TIME::DateAdd(TIME_UNIT_MONTH, i, C_TIME::TimeNow());
		
		for (size_t st=pALLMP->m_MTBS.m_AllMPBS.begin(); st<pALLMP->m_MTBS.m_AllMPBS.end(); st++)
		{
			if (pALLMP->m_MTBS.m_AllMPBS.test(st))
			{
				logvt <<"month data mp="<<dec<<st<<",date="<<mytime<<endl;
				if (FillMonthData(st, 0x00010001, mytime, data) == true)
				{
					memcpy(&(IPCSHAREMEM.m_IpcDevicesRegData[st].m_Reg00010001[j][0]),data,sizeof(data));
				}
				else
				{
					memset(&(IPCSHAREMEM.m_IpcDevicesRegData[st].m_Reg00010001[j][0]),0xff,sizeof(data));
				}
			}
		}	
	}
		
}


