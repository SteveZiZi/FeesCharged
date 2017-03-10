/** @file
 *  @brief 国网任务执行器
 *  @author 
 *  @date 2012/06/24
 *  @version 0.1
 */
#include "gwtskexe.h"
#include "status.h"
#include "mrfact.h"
#include "context.h"
#include "datafile.h"
#include "sysctrl.h"
#include "fbfact.h"
#include "AlarmEventRecord.h"

//
// 国网广播校时任务
//
int C_GwTaskBroadcast::TickServer(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus)
{
	C_SYSCTRL::ProcessHeartBeat();
	return 0;
}

/** @brief 广播校时最大延时时间
 *  @param[in] pTaskInfo 任务信息
 *  @param[in] FBID 现场总线ID号
 *  @return 0:无效值; >0:有效延时时间
 *  @note 解决东软三代模式下校时延时900秒问题
 */
S_DWORD C_GwTaskBroadcast::GetMaxDelayS(const S_TaskInfo *pTaskInfo, S_WORD FBID)
{
	if (FB_IDX_ZBWX != FBID)
		return 0;

	// 载波无线方式下的广播校时最大延时
	S_ALLMP *pAllMP = STATUS.GetALLMP();
	S_DWORD DelayS = (S_DWORD)(pAllMP->m_MTBS.m_ZBMPBS.count() * 10);

	if (DelayS == 0) DelayS = 10; // 最少10秒
//	else if (DelayS > 100) DelayS = 100; // 暂时定为最大100秒

	return DelayS;
}

//
//国网路由任务
//
/** @brief 获取当前表计
 *  @param[out] Meter485 采集器下的485表
 *  @param[out] MeterZB 载波表或无线表
 *  @param[out] Relay 采集器
 *  @return 0:成功；>0:失败
 */
int C_GwTaskRoute::GetMeterAll(S_TERMINALS &Meter485, S_TERMINALS &MeterZB, S_TERMINALS &Relay)
{
	S_ALLMP *pAllMP = STATUS.GetALLMP();
	S_MpInfos *pMpInfos = STATUS.GetMpInfos();
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	for (size_t MP = pAllMP->m_MTBS.m_ZBMPBS.begin(); MP < pAllMP->m_MTBS.m_ZBMPBS.end(); MP++)
	{
		if (pAllMP->m_MTBS.m_ZBMPBS.test(MP) == false)
			continue;
		if (pMpInfos->m_Item[MP].m_Type != MP_TYPE_METER)
			continue;
		S_TERMINAL Meter, MyRelay;
		S_MeterBase MeterBaseInfo;
		S_WORD SN = pMpInfos->m_Item[MP].m_SN;
		pMeterInfos->m_Item[SN].ExtractBaseInfo(MeterBaseInfo);

		Meter.m_strAddress.append(MeterBaseInfo.m_strAddress);
		C_MRULE *pRule = C_MRuleFact::GetMeterRule(MeterBaseInfo.m_RuleID);
		if (pRule == NULL)
			Meter.m_strProtocol.append("unknow");
		else
			Meter.m_strProtocol.append(pRule->GetName());

		MyRelay.m_strAddress.append(MeterBaseInfo.m_strRelay);
		MyRelay.m_strProtocol.append("unknow");

		if (pMeterInfos->m_Item[SN].ValidRelayAddress() == true)
		{
			Meter485.push_back(Meter);
			Relay.InsertUnique(MyRelay);
		}
		else
		{
			MeterZB.push_back(Meter);
		}
	}

	logwt<<"统计载波表共["<<(S_WORD)MeterZB.size()<<"]块"<<endl;
	logwt<<"统计载波485表共["<<(S_WORD)Meter485.size()<<"]块"<<endl;
	logwt<<"统计采集器共["<<(S_WORD)Relay.size()<<"]块"<<endl;
	return 0;
}

/** @brief 路由模块操作失败消息 */
int C_GwTaskRoute::SendRouterErrorMessage(int ErrorInfo)
{
	S_DbcjStatus *pStatus = CONTEXT.GetDbcjStatus(MP_DBCJ_4);
	pStatus->m_timeRouteError = C_TIME::TimeNow();
	return 0;
}


//
//国网实时任务
//
/** @brief 获取采集信息
 *  @param[in] pTaskInfo 任务信息
 *  @param[out] GatherInfos 采集信息
 *  @return 0:成功; >0:失败
 */
int C_GwTaskRealtime::GetMeterGatherInfo(const S_TaskInfo *pTaskInfo, S_GatherInfoS &GatherInfos)
{
	string strMsg(pTaskInfo->m_strPrivateData.begin() + sizeof(int) + sizeof(S_BYTE), pTaskInfo->m_strPrivateData.end());
	if (strMsg.size() < 6)
		return -1;

	for (string::iterator iter = strMsg.begin(); distance(iter, strMsg.end()) >= 6; advance(iter, 6))
	{
		S_WORD DA = KGE_COMBWORD(*(iter+1), *(iter+0));
		S_DWORD DT = KGE_COMBDWORD(KGE_COMBWORD(*(iter+5), *(iter+4)), KGE_COMBWORD(*(iter+3), *(iter+2)));
		BuildupGatherInfo(DA, DT, GatherInfos);
	}
	return 0;
}

/** @brief 处理采集数据
 *  @param[in] pTaskInfo 任务信息
 *  @param[out] GatherInfos 采集信息
 *  @return 0:成功; >0:失败
 */
int C_GwTaskRealtime::HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_GatherInfoS &GatherInfos)
{
	int SerID;
	string strMsg;
	S_BYTE MsgT = pTaskInfo->m_strPrivateData[sizeof(int)];
	copy(pTaskInfo->m_strPrivateData.begin(), pTaskInfo->m_strPrivateData.begin()+sizeof(int), (char*)&SerID);

	for (size_t i = 0; i < GatherInfos.size(); i++)
	{
		const S_GatherInfo &GatherInfo = GatherInfos[i];
		S_MSDI MSDI = STATUS.FindMasterStationDataID(CLASSDATA_1, GatherInfo.m_MSDI, 0x00);
		if (MSDI.m_Enable == false)
			continue;

		string strData;
		if (MSDI.SerializeData(GatherInfo.m_CommRegs, GatherInfo.m_strDatas, strData) <= 0)
			MSDI.FillData(strData); // fill

		S_WORD DA = C_DAPN::PN2DA((S_WORD)GatherInfo.m_MP);
		S_DWORD DT = GatherInfo.m_MSDI;
		strMsg.append((char *)&DA, 2);
		strMsg.append((char *)&DT, 4);
		strMsg.append(strData);
	}
	
	if (strMsg.empty())
		strMsg.append((size_t)6, 0);

	if (C_SysMsg::Reply2Server(SerID, MsgT, strMsg) == 0)
		return 0;
	return -1;
}

/** @brief 取消采集数据(每采完一个寄存器查询一次)
 *  @param[in] pTaskInfo 任务信息
 *  @param[out] GatherInfos 采集信息
 *  @return true:取消; false:继续
 */
bool C_GwTaskRealtime::CancelMeterGatherData(const S_TaskInfo *pTaskInfo, const S_GatherInfoS &GatherInfos)
{
	C_SYSCTRL::ProcessHeartBeat();
	return false;
}

int C_GwTaskRealtime::BuildupGatherInfo(S_WORD DA, S_DWORD DT, S_GatherInfoS &GatherInfos)
{
	C_DA MyDA(DA);
	for (int i = 0; i < 8; i++)
	{
		if (MyDA.TestMP(i) == false)
			continue;
		S_WORD MP = MyDA.GetMP(i);
		_BuildupGatherInfo(MP, DT, GatherInfos);
	}
	return 0;
}

int C_GwTaskRealtime::_BuildupGatherInfo(S_WORD MP, S_DWORD DT, S_GatherInfoS &GatherInfos)
{
	S_GatherInfo GatherInfo;
	GatherInfo.m_MP = MP;
	GatherInfo.m_MSDI = DT;

	if (GetGatherInfo(MP, DT, GatherInfo) == false)
		return -1;

	S_MSDI MSDI = STATUS.FindMasterStationDataID(CLASSDATA_1, GatherInfo.m_MSDI, 0x00);
	if (MSDI.m_Enable == false)
	{
		logwt<<"主站数据项[Class=1 FN="<<HEX<<GatherInfo.m_MSDI<<"]未配置"<<endl;
		return -1;
	}
	GatherInfo.m_DisableReg = MSDI.m_DisableCommReg;
	for (int j = 0; j < MSDI_COMM_REG_MAX; j++)
	{
		S_REGISTER CommReg = STATUS.FindCommonRegister(MSDI.m_CommRegID[j]);
		if (CommReg.m_ID == 0x00)
			break;
		GatherInfo.m_CommRegs.push_back(CommReg);
	}
	GatherInfos.push_back(GatherInfo);
	return 0;
}

bool C_GwTaskRealtime::GetGatherInfo(S_WORD MP, S_DWORD FN, S_GatherInfo &GatherInfo)
{
	S_MpInfos *pMpInfos = STATUS.GetMpInfos();
	if (MP < CONS_MP_COUNT_MAX)
	{
		S_MpInfo &MpInfo = pMpInfos->m_Item[MP];
		if (MpInfo.m_AutoGather == 0)//停抄
		{
			logwt<<"测量点["<<MP<<"]属于停抄状态"<<endl;
			return false;
		}
		if (MpInfo.m_Type == MP_TYPE_METER)
			return GetGatherInfo_Meter(MpInfo.m_SN, FN, GatherInfo);
		else if (MpInfo.m_Type == MP_TYPE_PULSE)
			return GetGatherInfo_Pulse(MpInfo.m_SN, FN, GatherInfo);
		else if (MpInfo.m_Type == MP_TYPE_ANALOG)
			return GetGatherInfo_Analog(MpInfo.m_SN, FN, GatherInfo);
	}
	return false;
}

bool C_GwTaskRealtime::GetGatherInfo_Meter(S_WORD SN, S_DWORD FN, S_GatherInfo &GatherInfo)
{
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_MeterInfo &Meter = pMeterInfos->m_Item[SN];
	if (Meter.m_Enable == false)
	{
		logwt<<"测量点["<<GatherInfo.m_MP<<"]指向的表计无效"<<endl;
		return false;
	}
	Meter.ExtractBaseInfo(GatherInfo.m_MeterBase);
	return true;
}

bool C_GwTaskRealtime::GetGatherInfo_Pulse(S_WORD SN, S_DWORD FN, S_GatherInfo &GatherInfo)
{
	//S_PULSES *pPULSES = STATUS.GetPulses();
	//if (SN >= CONS_PULSE_COUNT_MAX)
	//	return false;
	//S_PULSE *pPulse = &pPULSES->m_Item[SN];
	//if (pPulse->m_MP == 0x00 || pPulse->m_PortNO == 0x00)
	//	return false;
	//GatherInfo.m_MeterBase.m_MP = (S_BYTE)GatherInfo.m_MP;
	//GatherInfo.m_MeterBase.m_ComSet.m_PortNO = pPulse->m_PortNO; // DOTO:map port
	return false;
}

bool C_GwTaskRealtime::GetGatherInfo_Analog(S_WORD SN, S_DWORD FN, S_GatherInfo &GatherInfo)
{
	return false;
}

//
// 国网重点户任务
//
/** @brief 取任务执行器的名称 */
const char *C_GwTaskVip::GetName(void)
{
	return TASK_NAME_VIP;
}
/** @brief 取任务执行器的类型号 */
int C_GwTaskVip::GetTypeID(void)
{
	return USER_TASK_VIP;
}

/** @brief 获取采集信息
 *  @param[in] MP 测量点或表计序号
 *  @param[out] MeterBase 表计信息
 *  @return 0:成功; <0:失败
 */
int C_GwTaskVip::GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase)
{
	return STATUS.GetMeterGatherInfo(mpMeter, MeterBase);
}

/** @brief 处理采集数据
 *  @param[in] pTaskInfo 任务信息
 *  @param[out] pTaskCtrlStatus 采集信息
 *  @return 0:成功; <0:失败
 */
int C_GwTaskVip::HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus)
{
	S_TIME FreezeTime = pTaskCtrlStatus->m_timeStdStart;
	int ok = C_Class2Data::Write(pTaskInfo, pTaskCtrlStatus, FreezeTime);

	S_TermData *pTermData = STATUS.GetTermData();
	pTermData->m_GatherStatInfos.SetInfo(MSDI_FREEZE_HOUR, pTaskCtrlStatus->m_timeStdStart, pTaskCtrlStatus->m_mpMeter, ok>=0);
	if (pTaskCtrlStatus->m_mpMeter >= pTaskInfo->m_MPBS.end())
		pTermData->m_GatherStatInfos.SetTaskFinish(MSDI_FREEZE_HOUR, C_TIME::TimeNow());
	return ok;
}

/** @brief 处理采集通讯错误
 *  @param[in] pTaskInfo 任务信息
 *  @param[out] pTaskCtrlStatus 采集信息
 *  @return 0:成功; <0:失败
 */
int C_GwTaskVip::HandleMeterCommInfo(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus)
{
//	S_TermData *pTermData = STATUS.GetTermData();
//	if (pTaskCtrlStatus->m_GatherError == METER_READ_ERROR_NO_FRAME)
//		pTermData->m_MeterData.m_MeterCommFailTimes[pTaskCtrlStatus->m_mpMeter] += 1;
//	else if (pTaskCtrlStatus->m_GatherError == METER_READ_ERROR_NO)
//		pTermData->m_MeterData.m_MeterCommFailTimes[pTaskCtrlStatus->m_mpMeter] = 0;
	return 0;
}

//
// 国网日任务
//
/** @brief 取任务执行器的名称 */
const char *C_GwTaskDay::GetName(void)
{
	return TASK_NAME_DAY;
}
/** @brief 取任务执行器的类型号 */
int C_GwTaskDay::GetTypeID(void)
{
	return USER_TASK_DAY;
}

/** @brief 获取采集信息
 *  @param[in] MP 测量点或表计序号
 *  @param[out] MeterBase 表计信息
 *  @return 0:成功; <0:失败
 */
int C_GwTaskDay::GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase)
{
	return STATUS.GetMeterGatherInfo(mpMeter, MeterBase);
}

/** @brief 处理采集数据
 *  @param[in] pTaskInfo 任务信息
 *  @param[out] pTaskCtrlStatus 采集信息
 *  @return 0:成功; <0:失败
 */
int C_GwTaskDay::HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus)
{
	S_TIME FreezeTime = pTaskCtrlStatus->m_timeStdStart/* - 24*60*60*/; // 按采集时间存储
	int ok = C_Class2Data::Write(pTaskInfo, pTaskCtrlStatus, FreezeTime);

	S_TermData *pTermData = STATUS.GetTermData();
	pTermData->m_GatherStatInfos.SetInfo(MSDI_FREEZE_DAY, pTaskCtrlStatus->m_timeStdStart, pTaskCtrlStatus->m_mpMeter, ok>=0);
	if (pTaskCtrlStatus->m_mpMeter >= pTaskInfo->m_MPBS.end())
		pTermData->m_GatherStatInfos.SetTaskFinish(MSDI_FREEZE_DAY, C_TIME::TimeNow());
	return ok;
}

/** @brief 处理采集通讯错误
 *  @param[in] pTaskInfo 任务信息
 *  @param[out] pTaskCtrlStatus 采集信息
 *  @return 0:成功; <0:失败
 */
int C_GwTaskDay::HandleMeterCommInfo(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus)
{
//	S_TermData *pTermData = STATUS.GetTermData();
//	if (pTaskCtrlStatus->m_GatherError == METER_READ_ERROR_NO_FRAME)
//		pTermData->m_MeterData.m_MeterCommFailTimes[pTaskCtrlStatus->m_mpMeter] += 1;
//	else if (pTaskCtrlStatus->m_GatherError == METER_READ_ERROR_NO)
//		pTermData->m_MeterData.m_MeterCommFailTimes[pTaskCtrlStatus->m_mpMeter] = 0;
	return 0;
}

//
// 国网月任务
//
/** @brief 取任务执行器的名称 */
const char *C_GwTaskMon::GetName(void)
{
	return TASK_NAME_MON;
}
/** @brief 取任务执行器的类型号 */
int C_GwTaskMon::GetTypeID(void)
{
	return USER_TASK_MON;
}

/** @brief 获取采集信息
 *  @param[in] MP 测量点或表计序号
 *  @param[out] MeterBase 表计信息
 *  @return 0:成功; <0:失败
 */
int C_GwTaskMon::GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase)
{
	return STATUS.GetMeterGatherInfo(mpMeter, MeterBase);
}

/** @brief 处理采集数据
 *  @param[in] pTaskInfo 任务信息
 *  @param[out] pTaskCtrlStatus 采集信息
 *  @return 0:成功; <0:失败
 */
int C_GwTaskMon::HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus)
{
	S_TIME FreezeTime = C_TIME::DateAdd(TIME_UNIT_MONTH, /*-1*/0, pTaskCtrlStatus->m_timeStdStart); // 按采集时间存储
	int ok = C_Class2Data::Write(pTaskInfo, pTaskCtrlStatus, FreezeTime);

	S_TermData *pTermData = STATUS.GetTermData();
	pTermData->m_GatherStatInfos.SetInfo(MSDI_FREEZE_MONTH, pTaskCtrlStatus->m_timeStdStart, pTaskCtrlStatus->m_mpMeter, ok>=0);
	if (pTaskCtrlStatus->m_mpMeter >= pTaskInfo->m_MPBS.end())
		pTermData->m_GatherStatInfos.SetTaskFinish(MSDI_FREEZE_MONTH, C_TIME::TimeNow());
	return ok;
}

/** @brief 处理采集通讯错误
 *  @param[in] pTaskInfo 任务信息
 *  @param[out] pTaskCtrlStatus 采集信息
 *  @return 0:成功; <0:失败
 */
int C_GwTaskMon::HandleMeterCommInfo(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus)
{
//	S_TermData *pTermData = STATUS.GetTermData();
//	if (pTaskCtrlStatus->m_GatherError == METER_READ_ERROR_NO_FRAME)
//		pTermData->m_MeterData.m_MeterCommFailTimes[pTaskCtrlStatus->m_mpMeter] += 1;
//	else if (pTaskCtrlStatus->m_GatherError == METER_READ_ERROR_NO)
//		pTermData->m_MeterData.m_MeterCommFailTimes[pTaskCtrlStatus->m_mpMeter] = 0;
	return 0;
}

//
//国网优先告警采集任务
//
/** @brief 取任务执行器的名称 */
const char *C_GwPollAlarm::GetName(void)
{
	return TASK_NAME_POLL_ALARM;
}
/** @brief 取任务执行器的类型号 */
int C_GwPollAlarm::GetTypeID(void)
{
	return USER_TASK_POLL_ALARM;
}

/** @brief 任务结束
 *  @return >=0:成功；<0:失败
 */
int C_GwPollAlarm::TaskFinish(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus)
{
	S_AlmFlags *pAlmFlags = CONTEXT.GetAlmFlags();
	if (pTaskFreezeStatus != NULL)
		pAlmFlags->SetTask((S_BYTE)pTaskFreezeStatus->m_MSDI, pTaskFreezeStatus->m_SuccessfulMPBS);
	return 0;
}

/** @brief 获取采集信息
 *  @param[in] MP 测量点或表计序号
 *  @param[out] MeterBase 表计信息
 *  @return 0:成功; <0:失败
 */
int C_GwPollAlarm::GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase)
{
	return STATUS.GetMeterGatherInfo(mpMeter, MeterBase);
}

/** @brief 处理采集数据
 *  @param[in] pTaskInfo 任务信息
 *  @param[out] pTaskCtrlStatus 采集信息
 *  @return 0:成功; <0:失败
 */
int C_GwPollAlarm::HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus)
{
//	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_WORD MP = pTaskCtrlStatus->m_mpMeter;
	if (pTaskInfo->m_CommRegs.size() != pTaskCtrlStatus->m_strDatas.size())
	{
		logwt<<"测量点["<<MP<<"]的数据与寄存器不匹配"<<endl;
		return -1;//数据不匹配
	}
	if (pTaskCtrlStatus->m_strDatas.Any() == false)
	{
		logwt<<"测量点["<<MP<<"]的数据为空"<<endl;
		return -1;//数据一个都没有
	}
	S_MSDI msdi = STATUS.FindMasterStationDataID(CLASSDATA_3, pTaskInfo->m_MSDI);
	if (msdi.m_Enable == false)
		return -1;

	string strData;
	if (msdi.SerializeData(pTaskInfo->m_CommRegs, pTaskCtrlStatus->m_strDatas, strData) <= 0)
	{
		logwt<<"测量点["<<MP<<"]的数据串行化失败"<<endl;
		return -1;
	}
	S_AlmDatas *pAlmDatas = CONTEXT.GetAlarmDatas();
	if (pAlmDatas->Save(msdi.m_DI, MP, strData)< 0)
	{
		logwt<<"测量点["<<MP<<"]的数据["<<HEX<<strData<<"]存储失败"<<endl;
		return -1;
	}
	S_TermData *pTermData = STATUS.GetTermData();
	pTermData->m_MeterData.m_timeMeterCommunication[MP] = C_TIME::TimeNow();
	logwt<<"测量点["<<MP<<"]的数据["<<HEX<<strData<<"]存储成功"<<endl;
	return 0;
}

/** @brief 处理采集通讯错误
 *  @param[in] pTaskInfo 任务信息
 *  @param[out] pTaskCtrlStatus 采集信息
 *  @return 0:成功; <0:失败
 */
int C_GwPollAlarm::HandleMeterCommInfo(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus)
{
//	S_TermData *pTermData = STATUS.GetTermData();
//	if (pTaskCtrlStatus->m_GatherError == METER_READ_ERROR_NO_FRAME)
//		pTermData->m_MeterData.m_MeterCommFailTimes[pTaskCtrlStatus->m_mpMeter] += 1;
//	else if (pTaskCtrlStatus->m_GatherError == METER_READ_ERROR_NO)
//		pTermData->m_MeterData.m_MeterCommFailTimes[pTaskCtrlStatus->m_mpMeter] = 0;
	return 0;
}

//
//国网普通告警采集任务
//
/** @brief 取任务执行器的名称 */
const char *C_GwDayAlarm::GetName(void)
{
	return TASK_NAME_DAY_ALARM;
}
/** @brief 取任务执行器的类型号 */
int C_GwDayAlarm::GetTypeID(void)
{
	return USER_TASK_DAY_ALARM;
}

/** @brief 获取采集信息
 *  @param[in] MP 测量点或表计序号
 *  @param[out] MeterBase 表计信息
 *  @return 0:成功; <0:失败
 */
int C_GwDayAlarm::GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase)
{
	return STATUS.GetMeterGatherInfo(mpMeter, MeterBase);
}

/** @brief 处理采集数据
 *  @param[in] pTaskInfo 任务信息
 *  @param[out] pTaskCtrlStatus 采集信息
 *  @return 0:成功; <0:失败
 */
int C_GwDayAlarm::HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus)
{
//	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_WORD MP = pTaskCtrlStatus->m_mpMeter;
	if (pTaskInfo->m_CommRegs.size() != pTaskCtrlStatus->m_strDatas.size())
	{
		logwt<<"测量点["<<MP<<"]的数据与寄存器不匹配"<<endl;
		return -1;//数据不匹配
	}
	if (pTaskCtrlStatus->m_strDatas.Any() == false)
	{
		logwt<<"测量点["<<MP<<"]的数据为空"<<endl;
		return -1;//数据一个都没有
	}
	S_MSDI msdi = STATUS.FindMasterStationDataID(CLASSDATA_4, pTaskInfo->m_MSDI);
	if (msdi.m_Enable == false)
		return -1;

//
//TODO:
//

	string strData;
	if (msdi.SerializeData(pTaskInfo->m_CommRegs, pTaskCtrlStatus->m_strDatas, strData) <= 0)
	{
		logwt<<"测量点["<<MP<<"]的数据串行化失败"<<endl;
		return -1;
	}

	logwt<<"测量点["<<MP<<"]的数据["<<HEX<<strData<<"]存储成功"<<endl;
	return 0;
}

//
//国网轮询任务
//
/** @brief 取任务执行器的名称 */
const char *C_GwPolling::GetName(void)
{
	return TASK_NAME_POLLING;
}
/** @brief 取任务执行器的类型号 */
int C_GwPolling::GetTypeID(void)
{
	return USER_TASK_POLLING;
}

/** @brief 任务处于运行态
 *  @return TASK_EXEC_STATUS_
 */
int C_GwPolling::Execute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus)
{
	//C_SumData SumData;
	//int ok = C_TaskDirectBase::Execute(pTaskInfo, pTaskCtrlStatus, pTaskFreezeStatus);
	//if (ok == TASK_EXEC_STATUS_BLOCK)
	//	SumData.Stat(C_TIME::TimeNow(), pTaskInfo->m_PeriodVal, pTaskInfo->m_CommRegs, pTaskCtrlStatus->m_strDatas);
	//return ok;
	return 0;
}

/** @brief 获取采集信息
 *  @param[in] MP 测量点或表计序号
 *  @param[out] MeterBase 表计信息
 *  @return 0:成功; <0:失败
 */
int C_GwPolling::GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase)
{
	return STATUS.GetMeterGatherInfo(mpMeter, MeterBase);
}

/** @brief 处理采集数据
 *  @param[in] pTaskInfo 任务信息
 *  @param[out] pTaskCtrlStatus 采集信息
 *  @return 0:成功; <0:失败
 */
int C_GwPolling::HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus)
{
//	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_WORD MP = pTaskCtrlStatus->m_mpMeter;
	if (pTaskInfo->m_CommRegs.size() != pTaskCtrlStatus->m_strDatas.size())
	{
		logwt<<"测量点["<<MP<<"]的数据与寄存器不匹配"<<endl;
		return -1;//数据不匹配
	}
	if (pTaskCtrlStatus->m_strDatas.Any() == false)
	{
		logwt<<"测量点["<<MP<<"]的数据为空"<<endl;
		return -1;//数据一个都没有
	}
	S_MSDI msdi = STATUS.FindMasterStationDataID(CLASSDATA_4, pTaskInfo->m_MSDI);
	if (msdi.m_Enable == false)
		return -1;

	string strData;
	if (msdi.SerializeData(pTaskInfo->m_CommRegs, pTaskCtrlStatus->m_strDatas, strData) <= 0)
	{
		logwt<<"测量点["<<MP<<"]的数据串行化失败"<<endl;
		return -1;
	}
	////统计数据
	//C_StatData StatData;
	//StatData.Stat(C_TIME::TimeNow(), MP, pTaskInfo->m_PeriodVal, pTaskInfo->m_CommRegs, pTaskCtrlStatus->m_strDatas);
	////缓存数据
	//S_PollingDatas *pPollDatas = CONTEXT.GetPollingDatas();
	//if (pPollDatas->Save(C_TIME::TimeNow(), MP, pTaskInfo->m_CommRegs, pTaskCtrlStatus->m_strDatas)< 0)
	//{
	//	logwt<<"测量点["<<MP<<"]的数据["<<HEX<<strData<<"]存储失败"<<endl;
	//	return -1;
	//}
	logwt<<"测量点["<<MP<<"]的数据["<<HEX<<strData<<"]存储成功"<<endl;
	return 0;
}

//
// 国网能量数据统计日任务
//
/** @brief 取任务执行器的名称 */
const char *C_GwStatDay::GetName(void)
{
	return TASK_NAME_STAT_DAY;
}
/** @brief 取任务执行器的类型号 */
int C_GwStatDay::GetTypeID(void)
{
	return USER_TASK_STAT_DAY;
}

/** @brief 获取采集信息
 *  @param[in] MP 测量点或表计序号
 *  @param[out] MeterBase 表计信息
 *  @return 0:成功; <0:失败
 */
int C_GwStatDay::GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase)
{
	if (STATUS.GetMeterGatherInfo(mpMeter, MeterBase) < 0)
		return -1;

	return 0;
}

/** @brief 处理采集数据
 *  @param[in] pTaskInfo 任务信息
 *  @param[out] pTaskCtrlStatus 采集信息
 *  @return 0:成功; <0:失败
 */
int C_GwStatDay::HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus)
{
	S_TIME FreezeTime = pTaskCtrlStatus->m_timeStdStart/* - 24*60*60*/; // 按采集时间存储
	return C_Class2Data::Write(pTaskInfo, pTaskCtrlStatus, FreezeTime);
}

//
// 国网能量数据统计月任务
//
/** @brief 取任务执行器的名称 */
const char *C_GwStatMon::GetName(void)
{
	return TASK_NAME_STAT_MON;
}
/** @brief 取任务执行器的类型号 */
int C_GwStatMon::GetTypeID(void)
{
	return USER_TASK_STAT_MON;
}

/** @brief 处理采集数据
 *  @param[in] pTaskInfo 任务信息
 *  @param[out] pTaskCtrlStatus 采集信息
 *  @return 0:成功; <0:失败
 */
int C_GwStatMon::HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus)
{
	S_TIME FreezeTime = C_TIME::DateAdd(TIME_UNIT_MONTH, /*-1*/0, pTaskCtrlStatus->m_timeStdStart); // 按采集时间存储
	return C_Class2Data::Write(pTaskInfo, pTaskCtrlStatus, FreezeTime);
}

//
// 国网终端数据采集日任务
//
/** @brief 取任务执行器的名称 */
const char *C_GwTermDay::GetName(void)
{
	return TASK_NAME_TERM_DAY;
}
/** @brief 取任务执行器的类型号 */
int C_GwTermDay::GetTypeID(void)
{
	return USER_TASK_TERM_DAY;
}

/** @brief 获取采集信息
 *  @param[in] MP 测量点或表计序号
 *  @param[out] MeterBase 表计信息
 *  @return 0:成功; <0:失败
 */
int C_GwTermDay::GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase)
{
	if (mpMeter >= CONS_MP_COUNT_MAX)
		return -1;
	MeterBase.m_MP = mpMeter;
	MeterBase.m_RuleID = 0;
	MeterBase.m_ComSet.m_PortNO = 0;
	return 0;
}

/** @brief 处理采集数据
 *  @param[in] pTaskInfo 任务信息
 *  @param[out] pTaskCtrlStatus 采集信息
 *  @return 0:成功; <0:失败
 */
int C_GwTermDay::HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus)
{
	S_TIME FreezeTime = pTaskCtrlStatus->m_timeStdStart/* - 24*60*60*/; // 按采集时间存储
	return C_Class2Data::WriteTermData(pTaskInfo, pTaskCtrlStatus, FreezeTime);
}

/** @brief 任务结束
 *  @return >=0:成功；<0:失败
 */
int C_GwTermDay::TaskFinish(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus)
{
	//if ((pTaskInfo->m_MSDI & 0xFF) == 53)
	//{// TODO : 将月流量超限告警与F53 终端与主站日通信流量绑定，每日上报一次
	//	S_DataERC032 DataErc032;
	//	S_HttxInfo *pHttxInfo = STATUS.GetHttxInfo(MP_HTTX_1);
	//	DataErc032.m_CurMonFlow = STATUS.GetMonCommFlux();
	//	DataErc032.m_MonFlowLimit = pHttxInfo->m_MonTrafficLimit;
	//	if (DataErc032.m_MonFlowLimit == 0x00 || DataErc032.m_MonFlowLimit > DataErc032.m_CurMonFlow)
	//		return 0;
	//	C_AlarmEvent *pAlarmEvent = C_AlarmFactory::GetAlarmEventObj(32);
	//	if (pAlarmEvent != NULL && pAlarmEvent->NeedRecord() == true)
	//		pAlarmEvent->Action((void *)&DataErc032, 0);
	//}
	return 0;
}

//
// 国网终端数据采集月任务
//
/** @brief 取任务执行器的名称 */
const char *C_GwTermMon::GetName(void)
{
	return TASK_NAME_TERM_MON;
}
/** @brief 取任务执行器的类型号 */
int C_GwTermMon::GetTypeID(void)
{
	return USER_TASK_TERM_MON;
}

/** @brief 处理采集数据
 *  @param[in] pTaskInfo 任务信息
 *  @param[out] pTaskCtrlStatus 采集信息
 *  @return 0:成功; <0:失败
 */
int C_GwTermMon::HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus)
{
	S_TIME FreezeTime = C_TIME::DateAdd(TIME_UNIT_MONTH, /*-1*/0, pTaskCtrlStatus->m_timeStdStart); // 按采集时间存储
	return C_Class2Data::WriteTermData(pTaskInfo, pTaskCtrlStatus, FreezeTime);
}

//
//国网终端数据采集VIP任务
//
/** @brief 取任务执行器的名称 */
const char *C_GwTermVip::GetName(void)
{
	return TASK_NAME_TERM_VIP;
}
/** @brief 取任务执行器的类型号 */
int C_GwTermVip::GetTypeID(void)
{
	return USER_TASK_TERM_VIP;
}

/** @brief 处理采集数据
*  @param[in] pTaskInfo 任务信息
*  @param[out] pTaskCtrlStatus 采集信息
*  @return 0:成功; <0:失败
*/
 int C_GwTermVip::HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus)
{
	S_TIME FreezeTime = pTaskCtrlStatus->m_timeStdStart;
	return C_Class2Data::Write(pTaskInfo, pTaskCtrlStatus, FreezeTime);
}
 
//
//国网搜索表计任务
//
/** @brief 取任务执行器的名称 */
const char *C_GwSeekMeter::GetName(void)
{
	return TASK_NAME_SEEKMETER;
}
/** @brief 取任务执行器的类型号 */
int C_GwSeekMeter::GetTypeID(void)
{
	return SYS_TASK_SEEK_METER;
}

/** @brief 任务启动
 *  @return 0:成功；>0:失败
 */
int C_GwSeekMeter::TaskStart(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus)
{
	//S_SeekMeterInfos *pSeekMeterInfos = STATUS.GetSeekMeterInfos();
	//if (pTaskInfo->m_MPBS.test(FB_IDX_485I) == true)
	//	pSeekMeterInfos->Init(FB_IDX_485I);
	//if (pTaskInfo->m_MPBS.test(FB_IDX_485II) == true)
	//	pSeekMeterInfos->Init(FB_IDX_485II);
	//if (pTaskInfo->m_MPBS.test(FB_IDX_ZBWX) == true)
	//	pSeekMeterInfos->Init(FB_IDX_ZBWX);

	//S_DbcjInfo *pDbcjInfo = STATUS.GetDbcjInfo();
	//if (pDbcjInfo->m_MeterAutoUpdate == 0x01)
	//{//由dbcj1进行数据处理
	//	S_DbcjStatus *pDbcjStatus = CONTEXT.GetDbcjStatus(MP_DBCJ_1);
	//	if (pTaskInfo->m_MPBS.test(FB_IDX_485I) == true)
	//		pDbcjStatus->m_UpdateMeter485I = DBCJ_UPDATE_METER_STANDBY;
	//	if (pTaskInfo->m_MPBS.test(FB_IDX_485II) == true)
	//		pDbcjStatus->m_UpdateMeter485II = DBCJ_UPDATE_METER_STANDBY;
	//	if (pTaskInfo->m_MPBS.test(FB_IDX_ZBWX) == true)
	//		pDbcjStatus->m_UpdateMeterZBWX = DBCJ_UPDATE_METER_STANDBY;
	//}
	return 0;
}

/** @brief 任务结束
 *  @return >=0:成功；<0:失败
 */
int C_GwSeekMeter::TaskFinish(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus)
{
	S_DbcjStatus *pDbcjStatus = CONTEXT.GetDbcjStatus(MP_DBCJ_1);
	if (pDbcjStatus->m_UpdateMeter485I == DBCJ_UPDATE_METER_STANDBY)
		pDbcjStatus->m_UpdateMeter485I = DBCJ_UPDATE_METER_NOTIFY;
	if (pDbcjStatus->m_UpdateMeter485II == DBCJ_UPDATE_METER_STANDBY)
		pDbcjStatus->m_UpdateMeter485II = DBCJ_UPDATE_METER_NOTIFY;
	if (pDbcjStatus->m_UpdateMeterZBWX == DBCJ_UPDATE_METER_STANDBY)
		pDbcjStatus->m_UpdateMeterZBWX = DBCJ_UPDATE_METER_NOTIFY;
	return 0;
}

/** @brief 保存搜索成功的表计
 *  @return <0:失败; >=0:成功
 */
int C_GwSeekMeter::SaveMeter(int idxFB, const S_ReportMeterInfo &MeterInfo)
{
	string strMyRelay(MeterInfo.m_strRelay);
	if (strMyRelay.empty() == true)
		strMyRelay.append((size_t)6, (char)0);

	C_FieldBus* pBus = C_FBFact::GetFieldBus(idxFB);
	C_CONNECT *pConnect = pBus->GetConnection();
	S_CHANNEL Channel = pConnect->GetChannelInfo(true);
	logvt<<"总线["<<Channel.m_PortSet.m_ComSet.m_PortNO<<"]搜表:地址["<<HEX<<MeterInfo.m_strAddress<<"],采集器["<<strMyRelay<<"]"<<endl;
	if (NewMeter(MeterInfo.m_strAddress, strMyRelay) == false)
		return 0;

	S_MeterBase Meter;
	Meter.m_RuleID = C_MRuleFact::GetMeterRuleID(MeterInfo.m_strMeterRule.c_str()); ///< 表计规约
	if (Meter.m_RuleID == -1) //TODO  default 
		Meter.m_RuleID = METER_RULE_GB645_2007;
	Meter.m_ComSet = Channel.m_PortSet.m_ComSet; ///< 通讯配置
	Meter.m_strAddress.append(MeterInfo.m_strAddress); ///< 表计地址
	StoreMeter(idxFB, Meter, strMyRelay);
	NewMeterAlarm(Channel.m_PortSet.m_ComSet.m_PortNO, MeterInfo, strMyRelay);
	logvt<<"新表保存结束"<<endl;
	return 0;
}

bool C_GwSeekMeter::NewMeter(const string &strMeterAddr, const string &strRelay)
{
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	for (S_WORD sn = 1; sn < CONS_METER_COUNT_MAX; sn++)
	{
		S_MeterInfo &Meter = pMeterInfos->m_Item[sn];
		if (Meter.m_Enable == false)
			continue;
		if (strMeterAddr.compare(0, strMeterAddr.size(), Meter.m_Address, METER_ADDRESS_LEN_MAX) == 0)
		{
			if ((S_BYTE)strMeterAddr[0] != 0xEE)//真实表计
				return false;
			else if (strRelay.compare(0, strRelay.size(), Meter.m_RelayAddr, RELAY_ADDRESS_LEN_MAX) == 0)//地址为EE的虚拟表,比采集器
				return false;
		}
	}
	return true;
}

int C_GwSeekMeter::StoreMeter(int idxFB, const S_MeterBase &Meter, const string &strRelay)
{
	//S_SeekMeterInfos *pSeekMeterInfos = STATUS.GetSeekMeterInfos();
	//S_SeekMeterInfo *pSeekMeterInfo = pSeekMeterInfos->m_Item485I;
	//if (idxFB == FB_IDX_485II)
	//	pSeekMeterInfo = pSeekMeterInfos->m_Item485II;
	//else if (idxFB == FB_IDX_ZBWX)
	//	pSeekMeterInfo = pSeekMeterInfos->m_ItemZBWX;

	//for (S_WORD sn = 0; sn < CONS_SEEK_METER_MAX; sn++)
	//{
	//	if (pSeekMeterInfo->m_Enable == false)
	//	{
	//		pSeekMeterInfo->m_Enable = true;
	//		pSeekMeterInfo->m_RuleID = Meter.m_MeterRuleID;
	//		pSeekMeterInfo->m_PortSet = Meter.m_ComSet;
	//		copy(Meter.m_strMeterAddr.begin(), Meter.m_strMeterAddr.end(), pSeekMeterInfo->m_Address);
	//		copy(strRelay.begin(), strRelay.end(), pSeekMeterInfo->m_RelayAddr);
	//		return 0;
	//	}
	//	pSeekMeterInfo += 1;
	//}
	return -1;
}

int C_GwSeekMeter::NewMeterAlarm(S_BYTE VCom, const S_ReportMeterInfo &MeterInfo, const string &strRelay)
{
	//S_DataERC035 DataERC035;
	//DataERC035.m_PortNo = VCom;
	//DataERC035.m_phase = MeterInfo.m_Phase;
	//DataERC035.m_SignalQuality = MeterInfo.SignalQuality;
	//DataERC035.m_MeterRule = 1; // default DL/T645-2007
	//if (C_MRuleFact::GetMeterRuleID(MeterInfo.m_strMeterRule.c_str()) == METER_RULE_GB645_1997) // 参见设置参数F10规约
	//	DataERC035.m_MeterRule = 0;
	//DataERC035.m_strMeterAddr.append(MeterInfo.m_strAddress);

	//C_AlarmEvent *pAlarmEvent = C_AlarmFactory::GetAlarmEventObj(35);
	//if (pAlarmEvent != NULL && pAlarmEvent->NeedRecord() == true)
	//	pAlarmEvent->Action((void *)&DataERC035, 0);
	return 0;
}

//
// 国网日补抄任务
//
/** @brief 取任务执行器的名称 */
const char *C_GwTaskFixDay::GetName(void)
{
	return TASK_NAME_FIX_DAY;
}
/** @brief 取任务执行器的类型号 */
int C_GwTaskFixDay::GetTypeID(void)
{
	return USER_TASK_FIX_DAY;
}

/** @brief 任务启动
 *  @return 0:成功；>0:失败
 */
int C_GwTaskFixDay::TaskStart(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus)
{
	if (pTaskFreezeStatus->m_timeStdStart != pTaskCtrlStatus->m_timeStdStart)
	{
		pTaskFreezeStatus->m_timeStdStart = pTaskCtrlStatus->m_timeStdStart;
		pTaskFreezeStatus->m_SuccessfulMPBS.reset();
		return StatFixMeter(pTaskInfo, pTaskCtrlStatus, pTaskFreezeStatus);
	}
	return 0;
}

/** @brief 获取采集信息
 *  @param[in] MP 测量点或表计序号
 *  @param[out] MeterBase 表计信息
 *  @return 0:成功; <0:失败
 */
int C_GwTaskFixDay::GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase)
{
	return STATUS.GetMeterGatherInfo(mpMeter, MeterBase);
}

/** @brief 处理采集数据
 *  @param[in] pTaskInfo 任务信息
 *  @param[out] pTaskCtrlStatus 采集信息
 *  @return 0:成功; <0:失败
 */
int C_GwTaskFixDay::HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus)
{
	S_DWORD ClassData = pTaskInfo->m_MSDI >> 24;
	if (ClassData < CLASSDATA_5 || ClassData > CLASSDATA_9)
		return -1;

	S_DWORD DayCnt = ClassData - CLASSDATA_5 + 1; 
	S_TIME FreezeTime = pTaskCtrlStatus->m_timeStdStart/* - (24*60*60)*/ - DayCnt * (24*60*60); // 按采集时间存储
	return C_Class2Data::Write(pTaskInfo, pTaskCtrlStatus, FreezeTime);
}

int C_GwTaskFixDay::StatFixMeter(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus)
{
	char buffer[32];
	S_DWORD ClassData = pTaskInfo->m_MSDI >> 24;
	if (ClassData < CLASSDATA_5 || ClassData > CLASSDATA_9)
		return -1;
	S_MSDI msdi = STATUS.FindMasterStationDataID((S_BYTE)ClassData, pTaskInfo->m_MSDI);
	if (msdi.m_Enable == false)
		return -1;

	S_DWORD DayCnt = ClassData - CLASSDATA_5 + 1; 
	S_TIME FreezeTime = pTaskCtrlStatus->m_timeStdStart/* - (24*60*60)*/ - DayCnt * (24*60*60); // 按采集时间存储
	C_TIME::Time2Ascii14(FreezeTime, buffer);
	logwt<<"补抄任务:统计数据项["<<HEX<<msdi.m_DI<<"]时标["<<buffer<<"]的数据冻结情况..."<<endl;

	for (size_t MP = pTaskInfo->m_MPBS.begin(); MP < pTaskInfo->m_MPBS.end(); MP++)
	{
		if (pTaskInfo->m_MPBS.test(MP) == false)
			continue;
		string strData;
		if (C_Class2Data::Read(msdi.m_DI, (S_WORD)MP, strData, FreezeTime) >= 0)
			pTaskFreezeStatus->m_SuccessfulMPBS.set(MP);
	}

	logwt<<"补抄任务:统计结束!!!"<<endl;
	return 0;
}

//
// 转发任务和委托任务
//
void C_GwProxy::Handle(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus)
{
	int SerID;
	string strMsg;
	S_BYTE MsgT = pTaskInfo->m_strPrivateData[sizeof(int)];
	string strCmd(pTaskInfo->m_strPrivateData.begin()+sizeof(int)+sizeof(S_BYTE), pTaskInfo->m_strPrivateData.end());
	copy(pTaskInfo->m_strPrivateData.begin(), pTaskInfo->m_strPrivateData.begin()+sizeof(int), (char*)&SerID);

	if (strCmd.empty() == false)
	{
		if (MsgT == MSAFN_DATA_ROUTE)
			HandleDataRoute(strCmd, strMsg);
		//else if (MsgT == MSAFN_USER_EXTEND_PROXY)
		//	HandleProxy(strCmd, strMsg);
		else if (MsgT == MSAFN_WRITE_PARAM)
			HandleMeterCtrlCmd(strCmd, strMsg);
	}

	if (strMsg.empty() == false)
		C_SysMsg::Reply2Server(SerID, MsgT, strMsg);
}

int C_GwProxy::HandleDataRoute(const string &strCmd, string &strMsg)
{
	if (strCmd.size() < 6 + 8)
		return -1;

	string::const_iterator iter = strCmd.begin();
	S_WORD DA = KGE_COMBWORD(*(iter+1), *(iter+0));
	S_DWORD DT = KGE_COMBDWORD(KGE_COMBWORD(*(iter+5), *(iter+4)), KGE_COMBWORD(*(iter+3), *(iter+2)));
	string strData;
	string strCmdInfo(iter+6, strCmd.end());

	switch (DT)
	{
	case 0xE3020001:// ：透明转发
		HandleForward(strCmdInfo, strData);
		break;
	case 9:// F9：转发主站直接对电表的抄读数据命令
		HandleReadData(strCmdInfo, strData);
		break;
	case 10:// F10：转发主站直接对电表的遥控跳闸/允许合闸命令
		HandleControlCmd(strCmdInfo, strData);
		break;
	case 11:// F11：转发主站直接对电表的遥控送电命令
		HandlePowerCmd(strCmdInfo, strData);
		break;
	default:
		break;
	}

	strMsg.append((char *)&DA, 2);
	strMsg.append((char *)&DT, 4);
	strMsg.append(strData);
	return 0;
}

int C_GwProxy::HandleProxy(const string &strCmd, string &strMsg)
{
	if (strCmd.size() < 5)
		return -1;

	string::const_iterator iter = strCmd.begin();
	S_WORD DA = KGE_COMBWORD(*(iter+1), *(iter+0));
	S_WORD DT = KGE_COMBWORD(*(iter+3), *(iter+2));
	string strData;
	string strCmdInfo(iter+4, strCmd.end());
	S_MpInfos *pMpInfos = STATUS.GetMpInfos();
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();

	S_WORD MP = C_DAPN::DA2PN(DA);
	if (pMpInfos->Test(MP, MP_TYPE_METER) == false)
		return -1;
	S_MeterInfo &Meter = pMeterInfos->m_Item[pMpInfos->m_Item[MP].m_SN];
	if (Meter.m_Enable == false)
		return -1;

	switch (C_DTFN::DT2FN(DT))
	{
	case 1:// F1：电表身份认证
		HandleMeterIdentification(Meter, strCmdInfo, strData);
		break;
	case 2:// F2：电表对时任务
		HandleMeterTiming(Meter, strCmdInfo, strData);
		break;
	default:
		break;
	}

	strMsg.append((char *)&DA, 2);
	strMsg.append((char *)&DT, 2);
	strMsg.append(strData);
	return 0;
}

int C_GwProxy::HandleMeterCtrlCmd(const string &strCmd, string &strMsg)
{
	if (strCmd.size() < 6)
		return -1;

	string::const_iterator iter = strCmd.begin();
	S_WORD MP = C_DAPN::DA2PN(KGE_COMBWORD(*(iter+1), *(iter+0)));
	S_DWORD DT = KGE_COMBDWORD(KGE_COMBWORD(*(iter+5), *(iter+4)), KGE_COMBWORD(*(iter+3), *(iter+2)));
	string strData;
	string strCmdInfo(iter+6, strCmd.end());

	switch (DT)
	{
	case 0xE0001100:// 电能表遥控拉闸
		HandleMeterCtrl_LaZa(MP, strCmdInfo, strData);
		break;
	case 0xE0001101:// 电能表遥控合闸
		HandleMeterCtrl_HeZa(MP, strCmdInfo, strData);
		break;
	case 0xE0001102:// 对电能表保电操作
		HandleMeterCtrl_BaoDian(MP, strCmdInfo, strData);
		break;
	case 0xE0001103:// 对电能表保电解除操作
		HandleMeterCtrl_BuBaoDian(MP, strCmdInfo, strData);
		break;
	default:
		break;
	}

	return 0;
}

// ：透明转发
int C_GwProxy::HandleForward(const string &strCmdInfo, string &strData)
{
	if (strCmdInfo.size() < 8)
		return -1;
	logit<<"开始处理透明转发任务"<<endl;
	S_COMSET ComSet;
	S_BYTE type = strCmdInfo[0];
	ComSet.m_CDCheck = 0x00;
	//端口号
	ComSet.m_PortNO = strCmdInfo[1] + 1;
	//波特率
	ComSet.m_Baud = strCmdInfo[2] * 300;
	//校验方式
	if (strCmdInfo[3] == 0x01) ComSet.m_Parity = 2;
	else if (strCmdInfo[3] == 0x02) ComSet.m_Parity = 1;
	else ComSet.m_Parity = 0;
	//数据位
	ComSet.m_DataBit = strCmdInfo[4];
	//停止位
	if (strCmdInfo[5] == 0x01) ComSet.m_StopBit = 3;
	else if (strCmdInfo[5] == 0x02) ComSet.m_StopBit = 2;
	else ComSet.m_StopBit = 1;
//	S_BYTE FrameTimeout = strCmdInfo[6];
	S_WORD Cnt = strCmdInfo[7];
	if ((int)strCmdInfo.size() < Cnt + 8)
		return -1;
	C_FieldBus *pBus = C_FBFact::FindFieldBus(ComSet.m_PortNO);
	if (pBus == NULL)
		return -1;

	string strRelay;
	string strBuffer;
	S_DATAGRAM DataGram;
	DataGram.m_strProtocol.append(MRULE_NAME_UNKNOW);
	DataGram.m_strUserData.append(strCmdInfo.begin()+8, strCmdInfo.begin()+8+Cnt);
	if (Cnt >= 8) // as dlt645-****
		strRelay.append(DataGram.m_strUserData.begin()+1, DataGram.m_strUserData.begin()+1+6);
	else
		strRelay.append((size_t)6, (char)0x99);//broadcast address

	pBus->ChangeConnectionProperty(ComSet);
	if (pBus->RequestData(strRelay, DataGram, strBuffer) < 0)
		return -1;
	if (strBuffer.empty() == true)
		return -1;

	S_WORD DataLen = (S_WORD)strBuffer.size();
	strData.push_back(type);
	strData.push_back((S_BYTE)DataLen);
	strData.append(strBuffer);
	return 0;
}
// F9：转发主站直接对电表的抄读数据命令
int C_GwProxy::HandleReadData(const string &strCmdInfo, string &strData)
{
	if (strCmdInfo.size() < 13)
		return -1;
	logit<<"开始处理主站直接抄读表计数据任务"<<endl;
	S_BYTE PortNO = strCmdInfo[0];
	S_BYTE RelayCnt = strCmdInfo[1];
	if (RelayCnt == 0xFF)
		RelayCnt = 0x00;
	if ((int)strCmdInfo.size() < RelayCnt*6 + 13)
		return -1;

	string strRelay;
	string strAddress;
	S_BYTE flag = 0x00;
	S_DWORD DI = 0x00;
	string::const_iterator iter = strCmdInfo.begin()+2;

	strRelay.append(iter, iter+RelayCnt*6); advance(iter, RelayCnt*6);
	strAddress.append(iter, iter+6); advance(iter, 6);
	flag = *iter; advance(iter, 1);
	copy(iter, iter+4, (char *)&DI);

	S_BYTE Result = 0x00;
	string strMyData;
	const char *pRuleName = MRULE_NAME_GB645_1997;
	if ((flag&0x03) == 0x01)
		pRuleName = MRULE_NAME_GB645_2007;
	C_FieldBus *pBus = C_FBFact::FindFieldBus(PortNO);
	C_MRULE *pMRule = C_MRuleFact::GetMeterRule(pRuleName);
	if (pBus == NULL || pMRule == NULL)
	{
		if (pMRule == NULL)
			logwt<<"数据转发F9:表计规约获取失败,规约名="<<pRuleName<<endl;
		if (pBus == NULL)
			logwt<<"数据转发F9:现场总线获取失败,总线端口="<<PortNO<<endl;
		Result = 0x00; //不能执行转发
	}
	else
	{
		string strBuffer;
		S_DATAGRAM DataGram;
		S_COMSET ComSet = pMRule->GetComSet();
		C_CmdFilter CmdFilter(pMRule->GetName());
		C_CmdFilter *pOldFilter = pBus->SetCommandFilter(&CmdFilter);

		ComSet.m_PortNO = PortNO;
		pBus->ChangeConnectionProperty(ComSet);
		if (pMRule->MakeReadFrame(strAddress, DI, DataGram) <= 0)
			Result = 0; // 不能执行转发
		else if (pBus->RequestData(strRelay, DataGram, strBuffer) < 0)
			Result = 2; // 转发接收错误
		else if (pMRule->ParseReadFrame(strBuffer, strAddress, DI, strMyData) > 0)
			Result = 5; // 转发接收数据
		else
			Result = 4; // 转发接收否认
		pBus->SetCommandFilter(pOldFilter);
	}

	strData.push_back(PortNO); strData.append(strAddress);
	strData.push_back(Result); strData.push_back((S_BYTE)(strMyData.size() + 4));
	strData.append((char *)&DI, 4);
	strData.append(strMyData);
	return 0;
}
// F10：转发主站直接对电表的遥控跳闸/允许合闸命令
int C_GwProxy::HandleControlCmd(const string &strCmdInfo, string &strData)
{
	return -1;
}
// F11：转发主站直接对电表的遥控送电命令
int C_GwProxy::HandlePowerCmd(const string &strCmdInfo, string &strData)
{
	return -1;
}

// F1：电表身份认证
int C_GwProxy::HandleMeterIdentification(const S_MeterInfo &Meter, const string &strCmdInfo, string &strData)
{
	if (strCmdInfo.empty() == true)
		return -1;
	logit<<"处理表计["<<Meter.m_MeasureNo<<"]身份认证"<<endl;
	S_DbcjParam *pDbcjParam = STATUS.GetDbcjParam();

	string strRelay;
	string strAddress(Meter.m_Address, METER_ADDRESS_LEN_MAX);
	string strOperator((char *)&pDbcjParam->m_Operator, 4);

	int ok = 0;
	string strMyData;
	C_FieldBus *pBus = C_FBFact::FindFieldBus(Meter.m_PortSet.m_PortNO);
	C_MRULE *pMRule = C_MRuleFact::GetMeterRule(Meter.m_RuleID);
	if (pBus == NULL || pMRule == NULL)
	{
		if (pMRule == NULL)
			logwt<<"表计规约获取失败,规约ID ="<<Meter.m_RuleID<<endl;
		if (pBus == NULL)
			logwt<<"现场总线获取失败,总线端口="<<Meter.m_PortSet.m_PortNO<<endl;
		ok = -1;
	}
	else
	{
		string strBuffer;
		S_REGISTER CommReg;
		S_DATAGRAM DataGram;
		S_COMSET ComSet = Meter.m_PortSet;
		C_CmdFilter CmdFilter(pMRule->GetName());
		C_CmdFilter *pOldFilter = pBus->SetCommandFilter(&CmdFilter);

		CommReg.m_ID = 0x070000FF;
		if (ComSet.m_Baud == 0x00)
			ComSet = pMRule->GetComSet();
		ComSet.m_PortNO = Meter.m_PortSet.m_PortNO;
		pBus->ChangeConnectionProperty(ComSet);
		if (pMRule->MakeSecurityCmd(strAddress, CommReg, strOperator, strCmdInfo, DataGram) <= 0)
			ok = -2;
		else if (pBus->RequestData(strRelay, DataGram, strBuffer) < 0)
			ok = -3;
		else if (pMRule->ParseSecurityCmd(strBuffer, strAddress, CommReg, strMyData) < 0)
			ok = -4;
		pBus->SetCommandFilter(pOldFilter);
	}

	strData.append(strMyData);
	return ok;
}

// F2：电表对时任务
int C_GwProxy::HandleMeterTiming(const S_MeterInfo &Meter, const string &strCmdInfo, string &strData)
{
	S_BYTE Password[] = {0x98, 0x00, 0x00, 0x00};
	if (strCmdInfo.empty() == true)
		return -1;
	logit<<"处理表计["<<Meter.m_MeasureNo<<"]对时任务..."<<endl;
	S_DbcjParam *pDbcjParam = STATUS.GetDbcjParam();

	string strRelay;
	string strAddress(Meter.m_Address, METER_ADDRESS_LEN_MAX);
	string strPassword((char *)Password, 4);
	string strOperator((char *)&pDbcjParam->m_Operator, 4);

	int ok = 0;
	string strMyData;
	C_FieldBus *pBus = C_FBFact::FindFieldBus(Meter.m_PortSet.m_PortNO);
	C_MRULE *pMRule = C_MRuleFact::GetMeterRule(Meter.m_RuleID);
	if (pBus == NULL || pMRule == NULL)
	{
		if (pMRule == NULL)
			logwt<<"表计规约获取失败,规约ID ="<<Meter.m_RuleID<<endl;
		if (pBus == NULL)
			logwt<<"现场总线获取失败,总线端口="<<Meter.m_PortSet.m_PortNO<<endl;
		ok = -1;
	}
	else
	{
		string strBuffer;
		S_REGISTER CommReg;
		S_DATAGRAM DataGram;
		S_COMSET ComSet = Meter.m_PortSet;
		C_CmdFilter CmdFilter(pMRule->GetName());
		C_CmdFilter *pOldFilter = pBus->SetCommandFilter(&CmdFilter);

		CommReg.m_ID = 0x0400010C;
		if (ComSet.m_Baud == 0x00)
			ComSet = pMRule->GetComSet();
		ComSet.m_PortNO = Meter.m_PortSet.m_PortNO;
		pBus->ChangeConnectionProperty(ComSet);
		if (pMRule->MakeWriteCmd(strAddress, CommReg, strPassword, strOperator, strCmdInfo, DataGram) <= 0)
			ok = -2;
		else if (pBus->RequestData(strRelay, DataGram, strBuffer) < 0)
			ok = -3;
		else if (pMRule->ParseWriteCmd(strBuffer, strAddress, CommReg, strMyData) < 0)
			ok = -4;
		pBus->SetCommandFilter(pOldFilter);
	}

	strData.append(strMyData);
	return ok;
}

// 电能表遥控拉闸
int C_GwProxy::HandleMeterCtrl_LaZa(S_WORD MP, const string &strCmdInfo, string &strData)
{
	string strAddress(strCmdInfo.begin(), strCmdInfo.begin()+6);
	string strPassword(strCmdInfo.begin()+6, strCmdInfo.begin()+6+4);
	string strOperator(strCmdInfo.begin()+6+4, strCmdInfo.begin()+6+4+4);
	S_WORD TimeoutS = (S_WORD)kge_bcdc(strCmdInfo[14]) * 60;

	string strState, strYgData;
	S_MeterBase MeterBase;
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_MeterInfo &MeterInfo = pMeterInfos->m_Item[MP];
	MeterInfo.ExtractBaseInfo(MeterBase);
	int ok = HandleMeterBrake(MeterBase, strPassword, strOperator, 0x1A, TimeoutS, strState, strYgData); // 1AH代表跳闸
	if (ok == PROXY_METER_CTRL_SUCCESS)
	{
		logvt<<" 产生拉合闸控制事件"<<endl;	
		CEventRecrod_ERD6::EventRecord(MeterInfo,0xE201000E, CTL_RELAY_OPEN, strYgData, C_TIME::TimeNow());
		HandleBrakeAlarm(ALM_COMMID(ALARM_RELAY_CHANGE), MP, strState);
	}
	else if (ok == PROXY_METER_CTRL_FAILED)
	{
		logvt<<"合闸失败，产生拉合闸失败告警"<<endl;
		CAlarmRecrod_ARD12::AlarmRecord(MeterInfo,0xE200003C,ALARMSTATE_CREATE,C_TIME::TimeNow( ), strState);

		logvt<<" 产生拉合闸控制事件"<<endl;	
		CEventRecrod_ERD6::EventRecord(MeterInfo,0xE201000E, CTL_RELAY_OPEN, strYgData, C_TIME::TimeNow());
	}
	return ok;
}

// 电能表遥控合闸
int C_GwProxy::HandleMeterCtrl_HeZa(S_WORD MP, const string &strCmdInfo, string &strData)
{
	string strAddress(strCmdInfo.begin(), strCmdInfo.begin()+6);
	string strPassword(strCmdInfo.begin()+6, strCmdInfo.begin()+6+4);
	string strOperator(strCmdInfo.begin()+6+4, strCmdInfo.begin()+6+4+4);
	S_WORD TimeoutS = (S_WORD)kge_bcdc(strCmdInfo[14]) * 60;
	S_BYTE Brake = 0x1B; // 代表合闸允许
	if (strCmdInfo[15] == 1)
		Brake = 0x1C; // 直接合闸

	string strState, strYgData;
	S_MeterBase MeterBase;
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_MeterInfo &MeterInfo = pMeterInfos->m_Item[MP];
	MeterInfo.ExtractBaseInfo(MeterBase);
	int ok = HandleMeterBrake(MeterBase, strPassword, strOperator, Brake, TimeoutS, strState, strYgData);
	if (ok == PROXY_METER_CTRL_SUCCESS)
	{
		logvt<<" 产生拉合闸控制事件"<<endl;	
		CEventRecrod_ERD6::EventRecord(MeterInfo,0xE201000E, CTL_RELAY_CLOSE, strYgData, C_TIME::TimeNow());
		HandleBrakeAlarm(ALM_COMMID(ALARM_RELAY_CHANGE), MP, strState);
	}
	else if (ok == PROXY_METER_CTRL_FAILED)
	{
		logvt<<"合闸失败，产生拉合闸失败告警"<<endl;
		CAlarmRecrod_ARD12::AlarmRecord(MeterInfo,0xE200003C,ALARMSTATE_CREATE,C_TIME::TimeNow( ), strState);

		logvt<<" 产生拉合闸控制事件"<<endl;	
		CEventRecrod_ERD6::EventRecord(MeterInfo,0xE201000E, CTL_RELAY_CLOSE, strYgData, C_TIME::TimeNow());
	}
	return ok;
}

// 对电能表保电操作
int C_GwProxy::HandleMeterCtrl_BaoDian(S_WORD MP, const string &strCmdInfo, string &strData)
{
	string strAddress(strCmdInfo.begin(), strCmdInfo.begin()+6);
	string strPassword(strCmdInfo.begin()+6, strCmdInfo.begin()+6+4);
	string strOperator(strCmdInfo.begin()+6+4, strCmdInfo.begin()+6+4+4);
	S_WORD TimeoutS = (S_WORD)kge_bcdc(strCmdInfo[14]) * 60;

	string strState;
	S_MeterBase MeterBase;
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_MeterInfo &MeterInfo = pMeterInfos->m_Item[MP];
	MeterInfo.ExtractBaseInfo(MeterBase);
	int ok = HandleMeterBaoDian(MeterBase, strPassword, strOperator, 0x3A, TimeoutS, strState);
	if (ok == PROXY_METER_CTRL_SUCCESS)
	{
	}
	else if (ok == PROXY_METER_CTRL_FAILED)
	{
	}
	return ok;
}

// 对电能表保电解除操作
int C_GwProxy::HandleMeterCtrl_BuBaoDian(S_WORD MP, const string &strCmdInfo, string &strData)
{
	string strAddress(strCmdInfo.begin(), strCmdInfo.begin()+6);
	string strPassword(strCmdInfo.begin()+6, strCmdInfo.begin()+6+4);
	string strOperator(strCmdInfo.begin()+6+4, strCmdInfo.begin()+6+4+4);
	S_WORD TimeoutS = (S_WORD)kge_bcdc(strCmdInfo[14]) * 60;

	string strState;
	S_MeterBase MeterBase;
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_MeterInfo &MeterInfo = pMeterInfos->m_Item[MP];
	MeterInfo.ExtractBaseInfo(MeterBase);
	int ok = HandleMeterBaoDian(MeterBase, strPassword, strOperator, 0x3B, TimeoutS, strState);
	if (ok == PROXY_METER_CTRL_SUCCESS)
	{
	}
	else if (ok == PROXY_METER_CTRL_FAILED)
	{
	}
	return ok;
}

// 表计控制处理
// return PROXY_METER_CTRL_
int C_GwProxy::HandleMeterBrake(const S_MeterBase &MeterBase, const string &strPassword, const string &strOperator, S_BYTE Brake, S_WORD TimeoutS, string &strState, string &strYgData)
{
	S_WORD BrakeExpectedState = 0x10; // 表计继电器的期望值
	const char *pTaskName = "拉闸";
	if (Brake == 0x1B) BrakeExpectedState = 0x00, pTaskName = "允许合闸";
	else if (Brake == 0x1C) BrakeExpectedState = 0x00, pTaskName = "直接合闸";
	logit<<"开始处理["<<pTaskName<<"]任务"<<endl;

	char Buffer[8];
	string strParam((char *)&Brake, 1); // N1为控制命令类型
	S_TIME timeNow = C_TIME::TimeNow() + TimeoutS;
	strParam.push_back(0x00); // N2保留
	strParam.append(C_TIME::Time2BCDStr(timeNow, Buffer), 6); // N3～N8代表命令有效截止时间，数据格式为ssmmhhDDMMYY

	if (HandleMeterBrake_Action(MeterBase, strPassword, strOperator, strParam) != 0)
		TimeoutS = 20; // 下发拉合闸命令失败,快速读取状态字
	int ok = HandleMeterBrake_Poll(MeterBase, BrakeExpectedState, TimeoutS, strState);

	HandleMeterBrake_ReadReg(MeterBase, 0x60700000, strYgData); // 读正向有功
	HandleMeterBrake_ReadReg(MeterBase, 0x60D00000, strYgData); // 读反向有功

	if (ok == PROXY_METER_CTRL_SUCCESS)
		logit<<"处理["<<pTaskName<<"]任务成功"<<endl;
	else
		logit<<"处理["<<pTaskName<<"]任务失败"<<endl;
	return ok;
}

// 表计控制处理
// return PROXY_METER_BRAKE_
int C_GwProxy::HandleMeterBaoDian(const S_MeterBase &MeterBase, const string &strPassword, const string &strOperator, S_BYTE Brake, S_WORD TimeoutS, string &strState)
{
	return PROXY_METER_CTRL_NOTHING;
}

int C_GwProxy::HandleMeterBrake_ReadState(const S_MeterBase &MeterBase, S_DWORD CommRegID, string &strState)
{
	S_REGISTER CommReg = STATUS.FindCommonRegister(CommRegID);
	if (CommReg.m_ID == 0x00)
		return -1;

	string strData;
	C_MeterCom* pMeterCom = GetMeterCom((S_BYTE)(CommReg.m_ID>>28));
	pMeterCom->ClearBuffer(MeterBase.m_RuleID);
	if (pMeterCom->Read(MeterBase, CommReg, strData) < 0)
	{
		SleepSecond(1), strData.clear();
		pMeterCom->ClearBuffer(MeterBase.m_RuleID);
		pMeterCom->Read(MeterBase, CommReg, strData);
	}
	strState.append(strData);
	if (strData.empty() == false)
		return 0;
	return -1;
}

int C_GwProxy::HandleMeterBrake_ReadReg(const S_MeterBase &MeterBase, S_DWORD CommRegID, string &strData)
{
	S_REGISTER CommReg = STATUS.FindCommonRegister(CommRegID);
	if (CommReg.m_ID == 0x00)
		return -1;

	string strMyData;
	C_MeterCom* pMeterCom = GetMeterCom((S_BYTE)(CommReg.m_ID>>28));
	if (pMeterCom->Read(MeterBase, CommReg, strMyData) < 0)
	{
		SleepSecond(1), strMyData.clear();
		pMeterCom->Read(MeterBase, CommReg, strMyData);
	}
	if (strMyData.empty() == true)
		CommReg.FillData(strMyData);
	strData.append(strMyData);
	return (int)strMyData.size();
}

int C_GwProxy::HandleMeterBrake_Action(const S_MeterBase &MeterBase, const string &strPassword, const string &strOperator, const string &strParam)
{
	C_MeterCom* pMeterCom = GetMeterCom(GATHER_COM_OBJ_METER);
	if (pMeterCom->Brake(MeterBase, strPassword, strOperator, strParam) < 0)
	{
		SleepSecond(1);
		if (pMeterCom->Brake(MeterBase, strPassword, strOperator, strParam) < 0)
			return -1;
	}
	return 0;
}

int C_GwProxy::HandleMeterBrake_Poll(const S_MeterBase &MeterBase, S_WORD BrakeExpectedState, S_WORD TimeoutS, string &strState)
{
	string strExpectedState;
	C_MSTIMOUT ReadState(TimeoutS * 1000);

	do
	{
		C_SYSCTRL::ProcessHeartBeat(); // 加个心跳,防止进程被杀
		strExpectedState.clear();
		if (HandleMeterBrake_ReadState(MeterBase, 0x60630000, strExpectedState) == 0)
		{
			S_WORD ExpectedState3 = KGE_COMBWORD(strExpectedState[1], strExpectedState[0]);
			if (BrakeExpectedState == (ExpectedState3 & 0x10)) // 表继电器与期望值一致
			{
				strState.append(strExpectedState);
				return PROXY_METER_CTRL_SUCCESS;
			}
		}
		SleepSecond(5);
	}while (ReadState.timeout() == false);

	if (strExpectedState.empty() == false) strState.append(strExpectedState);
	else strState.append((size_t)2, (char)0);
	return PROXY_METER_CTRL_FAILED;
}

int C_GwProxy::HandleBrakeAlarm(S_DWORD ERC, S_WORD MP, const string &strState)
{
	S_AlmDatas *pAlmDatas = CONTEXT.GetAlarmDatas();
	pAlmDatas->Save(ERC, MP, strState);

	string strAlmData;
	C_AlmPhaseData almPhaseData;	
	if (almPhaseData.Load(ERC, MP, strAlmData) == -1)
	{
		strAlmData.assign(strState);
		if (strState[0] & 0x10) strAlmData[0] &= ~0x10;
		else strAlmData[0] |= 0x10;
		almPhaseData.Save(ERC, MP, strAlmData);
	}

	S_MPBS mpbsSuccess;
	mpbsSuccess.set(MP);

	S_AlmFlags *pAlmFlags = CONTEXT.GetAlmFlags();
	pAlmFlags->SetTask((S_BYTE)(ERC-1), mpbsSuccess); // 告警任务序号从0开始

	return 0;
}

//
//路由扩展任务，主要用于测试环境下处理有的载波需要轮抄的情况,如东软三代模式下的抄表
//
/** @brief 取任务执行器的名称 */
const char *C_GwRouteEx::GetName(void)
{
	return TASK_NAME_ROUTE_EX;
}
/** @brief 取任务执行器的类型号 */
int C_GwRouteEx::GetTypeID(void)
{
	return USER_TASK_ROUTE_EX;
}

/** @brief 任务启动
 *  @return 0:成功；>0:失败
 */
int C_GwRouteEx::TaskStart(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus)
{
	if (pTaskFreezeStatus->m_timeStdStart != pTaskCtrlStatus->m_timeStdStart)
	{
		pTaskFreezeStatus->m_timeStdStart = pTaskCtrlStatus->m_timeStdStart;
		pTaskFreezeStatus->m_SuccessfulMPBS.reset();
	}
	else if (pTaskFreezeStatus->m_timeLastRaise == TASK_MANUAL_START_TIME)
	{
		pTaskFreezeStatus->m_timeStdStart = pTaskCtrlStatus->m_timeStdStart;
		pTaskFreezeStatus->m_SuccessfulMPBS.reset();
	}
	return 0;
}

/** @brief 获取采集信息
 *  @param[in] MP 测量点或表计序号
 *  @param[out] MeterBase 表计信息
 *  @return 0:成功; <0:失败
 */
int C_GwRouteEx::GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase)
{
	return STATUS.GetMeterGatherInfo(mpMeter, MeterBase);
}

/** @brief 处理采集数据
 *  @param[in] pTaskInfo 任务信息
 *  @param[out] pTaskCtrlStatus 采集信息
 *  @return 0:成功; <0:失败
 */
int C_GwRouteEx::HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus)
{
	return 0;
}


