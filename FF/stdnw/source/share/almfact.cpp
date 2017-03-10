/** @file
 *  @brief 告警对象工厂
 *  @author 
 *  @date 2013/04/12
 *  @version 0.1
 */
#include "context.h"
#include "status.h"
#include "almfact.h"
#include "almevtbase.h"
#include "datafile.h"


static CAlarm_CurrentReverse_A sCurrentReverse_A; 
static CAlarm_CurrentReverse_B sCurrentReverse_B; 
static CAlarm_CurrentReverse_C sCurrentReverse_C; 
static CAlarm_LoseCurrent_A sLoseCurrent_A;
static CAlarm_LoseCurrent_B sLoseCurrent_B;
static CAlarm_LoseCurrent_C sLoseCurrent_C;
static CAlarm_LoseVoltage_A sAlarm_LoseVoltage_A;
static CAlarm_LoseVoltage_B sAlarm_LoseVoltage_B;
static CAlarm_LoseVoltage_C sAlarm_LoseVoltage_C;
static CAlarm_LackOfMoney sLackOfMoney;
static CAlarm_MeterDegreeDec sMeterDegreeDec;
static CAlarm_MeterStop sMeterStop;
static CAlarm_PeriodTariffChange sPeriodTariffChange;
static CAlarm_TerminalPowerOn sAlarm_TerminalPowerOn;
static CAlarm_TerminalPowerOff	sAlarm_TerminalPowerOff;
static CAlarm_RelayChange sRelayChange;
static CAlarm_ReadMeterFail sReadMeterFailAlarm;
static CAlarm_MeterClockAbnormal sMeterClockAbnormal;
static CAlarm_LoseVoltageTimes sLoseVoltageTimes;
static CAlarm_LoseCurrentTimes sLoseCurrentTimes;
static CAlarm_MonthFlowOverRange sMonthFlowOverRange;
static CAlarm_CurrentReverseTimes sCurrentReverseTimes;
static CAlarm_MeterProgramTime sMeterProgramTime;
	

static CAlarm *g_AlarmEventObject[] = 
{
	NULL, //ERC1：
	NULL, //ERC2：
	NULL, //ERC3：
	NULL, //ERC4：
	NULL, //ERC5：
	NULL, //ERC6：
	NULL, //ERC7：
	NULL, //ERC8：
	NULL, //ERC9：
	NULL, //ERC10：

	NULL, //ERC11：
	NULL, //ERC12：
	&sCurrentReverse_A, //ERC13：
	&sCurrentReverse_B, //ERC14：
	&sCurrentReverse_C, //ERC15：
	NULL, //ERC16：
	NULL, //ERC17：
	NULL, //ERC18：
	&sLoseCurrent_A, //ERC19：
	&sLoseCurrent_B, //ERC20：
	&sLoseCurrent_C, //ERC21：
	&sAlarm_LoseVoltage_A, //ERC22：
	&sAlarm_LoseVoltage_B, //ERC23：
	&sAlarm_LoseVoltage_C, //ERC24：
	NULL, //ERC25：
	NULL, //ERC26：
	NULL, //ERC27：
	NULL, //ERC28：
	NULL, //ERC29：
	NULL, //ERC30：

	NULL, //ERC31：
	NULL, //ERC32：
	NULL, //ERC33：
	NULL, //ERC34：
	NULL, //ERC35：
	NULL, //ERC36：
	NULL, //ERC37：
	NULL, //ERC38：
	NULL, //ERC39：
	NULL, //ERC40：
	NULL, //ERC41：
	NULL, //ERC42：
	&sLackOfMoney, //ERC43：
	&sMeterDegreeDec, //ERC44：
	NULL, //ERC45：
	&sMeterStop, //ERC46：
	NULL, //ERC47：
	NULL, //ERC48：
	NULL, //ERC49：
	NULL, //ERC50：
	&sAlarm_TerminalPowerOff, //ERC51：
	&sAlarm_TerminalPowerOn, //ERC52：
	&sMeterProgramTime, //ERC53：
	&sPeriodTariffChange, //ERC54：
	NULL, //ERC55：
	NULL, //ERC56：
	NULL, //ERC57：
	&sMonthFlowOverRange, //ERC58：
	&sRelayChange, //ERC59：
	NULL, //ERC60：
	&sReadMeterFailAlarm, //ERC61：
	&sMeterClockAbnormal, //ERC62：
	NULL, //ERC63：
	&sLoseVoltageTimes, //ERC64：
	&sLoseCurrentTimes, //ERC65：
	&sCurrentReverseTimes, //ERC66：
	NULL//ERC67：
};

static const S_BYTE g_AlmDetectMethodMask[] =
{
	ALM_DETECT_METHOD_TRIGGER, //ERC1：
	ALM_DETECT_METHOD_TRIGGER, //ERC2：
	ALM_DETECT_METHOD_TRIGGER, //ERC3：
	ALM_DETECT_METHOD_TRIGGER, //ERC4：
	ALM_DETECT_METHOD_TRIGGER, //ERC5：
	ALM_DETECT_METHOD_TRIGGER, //ERC6：
	ALM_DETECT_METHOD_TRIGGER, //ERC7：
	ALM_DETECT_METHOD_TRIGGER, //ERC8：
	ALM_DETECT_METHOD_TRIGGER, //ERC9：
	ALM_DETECT_METHOD_TRIGGER, //ERC10：
	ALM_DETECT_METHOD_TRIGGER, //ERC11：
	ALM_DETECT_METHOD_TRIGGER, //ERC12：
	ALM_DETECT_METHOD_TRIGGER|ALM_DETECT_METHOD_REALTIME, //ERC13：
	ALM_DETECT_METHOD_TRIGGER|ALM_DETECT_METHOD_REALTIME, //ERC14：
	ALM_DETECT_METHOD_TRIGGER|ALM_DETECT_METHOD_REALTIME, //ERC15：
	ALM_DETECT_METHOD_TRIGGER, //ERC16：
	ALM_DETECT_METHOD_TRIGGER, //ERC17：
	ALM_DETECT_METHOD_TRIGGER, //ERC18：
	ALM_DETECT_METHOD_TRIGGER|ALM_DETECT_METHOD_REALTIME, //ERC19：
	ALM_DETECT_METHOD_TRIGGER|ALM_DETECT_METHOD_REALTIME, //ERC20：
	ALM_DETECT_METHOD_TRIGGER|ALM_DETECT_METHOD_REALTIME, //ERC21：
	ALM_DETECT_METHOD_TRIGGER|ALM_DETECT_METHOD_REALTIME, //ERC22：
	ALM_DETECT_METHOD_TRIGGER|ALM_DETECT_METHOD_REALTIME, //ERC23：
	ALM_DETECT_METHOD_TRIGGER|ALM_DETECT_METHOD_REALTIME, //ERC24：
	ALM_DETECT_METHOD_TRIGGER, //ERC25：
	ALM_DETECT_METHOD_TRIGGER, //ERC26：
	ALM_DETECT_METHOD_TRIGGER, //ERC27：
	ALM_DETECT_METHOD_TRIGGER, //ERC28：
	ALM_DETECT_METHOD_TRIGGER, //ERC29：
	ALM_DETECT_METHOD_TRIGGER, //ERC30：
	ALM_DETECT_METHOD_TRIGGER, //ERC31：
	ALM_DETECT_METHOD_TRIGGER, //ERC32：
	ALM_DETECT_METHOD_TRIGGER, //ERC33：
	ALM_DETECT_METHOD_TRIGGER, //ERC34：
	ALM_DETECT_METHOD_TRIGGER, //ERC35：
	ALM_DETECT_METHOD_TRIGGER, //ERC36：
	ALM_DETECT_METHOD_TRIGGER, //ERC37：
	ALM_DETECT_METHOD_TRIGGER, //ERC38：
	ALM_DETECT_METHOD_TRIGGER, //ERC39：
	ALM_DETECT_METHOD_TRIGGER, //ERC40：
	ALM_DETECT_METHOD_TRIGGER, //ERC41：
	ALM_DETECT_METHOD_TRIGGER, //ERC42：
	ALM_DETECT_METHOD_TRIGGER|ALM_DETECT_METHOD_REALTIME, //ERC43：
	ALM_DETECT_METHOD_TRIGGER|ALM_DETECT_METHOD_REALTIME, //ERC44：
	ALM_DETECT_METHOD_TRIGGER, //ERC45：
	ALM_DETECT_METHOD_TRIGGER|ALM_DETECT_METHOD_REALTIME, //ERC46：
	ALM_DETECT_METHOD_TRIGGER, //ERC47：
	ALM_DETECT_METHOD_TRIGGER, //ERC48：
	ALM_DETECT_METHOD_TRIGGER, //ERC49：
	ALM_DETECT_METHOD_TRIGGER, //ERC50：
	ALM_DETECT_METHOD_TRIGGER, //ERC51：
	ALM_DETECT_METHOD_TRIGGER, //ERC52：
	ALM_DETECT_METHOD_TRIGGER|ALM_DETECT_METHOD_REALTIME, //ERC53：
	ALM_DETECT_METHOD_TRIGGER|ALM_DETECT_METHOD_REALTIME, //ERC54：
	ALM_DETECT_METHOD_TRIGGER, //ERC55：
	ALM_DETECT_METHOD_TRIGGER, //ERC56：
	ALM_DETECT_METHOD_TRIGGER, //ERC57：
	ALM_DETECT_METHOD_TRIGGER|ALM_DETECT_METHOD_REALTIME, //ERC58：
	ALM_DETECT_METHOD_TRIGGER|ALM_DETECT_METHOD_REALTIME, //ERC59：
	ALM_DETECT_METHOD_TRIGGER, //ERC60：
	ALM_DETECT_METHOD_TRIGGER|ALM_DETECT_METHOD_REALTIME, //ERC61：
	ALM_DETECT_METHOD_TRIGGER|ALM_DETECT_METHOD_REALTIME, //ERC62：
	ALM_DETECT_METHOD_TRIGGER, //ERC63：
	ALM_DETECT_METHOD_TRIGGER|ALM_DETECT_METHOD_REALTIME, //ERC64：
	ALM_DETECT_METHOD_TRIGGER|ALM_DETECT_METHOD_REALTIME, //ERC65：
	ALM_DETECT_METHOD_TRIGGER|ALM_DETECT_METHOD_REALTIME //ERC66：
};

int C_AlarmFactory::InitAlarmStatus(void)
{
	S_AlarmStatus *pAS = STATUS.GetAlarmStatus();
	pAS->m_NormalNewCnt = C_AlmEvt::CountNew(ALM_TYPE_NORMAL);
	pAS->m_ImportantNewCnt = C_AlmEvt::CountNew(ALM_TYPE_IMPORTANT);
	pAS->m_EventNewCnt = C_AlmEvt::CountNew(ALM_TYPE_EVENT);

	return 0;
}

/** @fn
 *  @brief 获取告警对象
 *  @param[in] ERC 告警ID [1-64]
 *  @return 告警对象指针
 */
CAlarm* C_AlarmFactory::GetAlarmEventObj(S_BYTE ERC)
{
	if (ERC >= KGE_MEMBER_OF(g_AlarmEventObject))
		return NULL;
	return g_AlarmEventObject[ERC];
}

bool C_AlarmFactory::PrintStatus(C_BITSET<8> &ParamStatus)
{
	for (int i = 0; i < (int)KGE_MEMBER_OF(g_AlarmEventObject); i++)
	{
		if (g_AlarmEventObject[i] != NULL && i < ParamStatus.GetBitSize())
			ParamStatus.SetBit(i);
	}
	return true;
}


bool C_AlarmDetect::SupportDetectMethod(S_BYTE ERC,S_BYTE Methed)
{
	if (ERC >= KGE_MEMBER_OF(g_AlmDetectMethodMask))
		return false;
	return (g_AlmDetectMethodMask[ERC] & Methed) ? true:false;
}


