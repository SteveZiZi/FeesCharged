/** @file
 *  @brief 后台通讯服务任务上报功能定义
 *  @author 
 *  @date 2011/11/30
 *  @version 0.1
 */
#include "report.h"
#include "sysbase.h"
#include "context.h"
#include "status.h"
#include "htrace.h"
#include "flowctrl.h"
#include "cmddata.h"

//
//
//
int C_ReportLogin::GetReportTaskID(void)
{
	return RTID_LOGIN;
}

int C_ReportLogin::ReportOneRecord(bool fok)
{
	return 0;
}

int C_ReportLogin::ReportAllRecord(bool fok)
{
	if (fok == true)
	{
		S_ConnectStatus *pCS = CONTEXT.GetConnectStatus();
		pCS->m_Httx1Logon = true;
		pCS->m_Httx1LogonTime = C_TIME::TimeNow();	
		FLOWCTRL.ClearFlowCtrlInfo();
		logit<<"登陆成功!"<<endl;
	}
	else
	{
		FLOWCTRL.LogonRecord(false);
		logwt<<"登陆失败（未收到主站确认）!!!"<<endl;
	}
	m_fPrompt = true;
	m_tickLastSendLoginCmd = CONTEXT.GetSystemTick();
	return 0;
}

bool C_ReportLogin::NeedReport(void)
{
	S_ConnectStatus *pCS = CONTEXT.GetConnectStatus();
	S_HttxStatus *pHS = CONTEXT.GetHttxStatus(m_HttxSN);
	S_HttxInfo *pHI = STATUS.GetHttxInfo(m_HttxSN);

	if (pCS->m_Httx1Logon == false && pHS->m_fMasterTerm == true && pHI->NeedLogon() == true)
	{
		if (m_tickLastSendLoginCmd == 0)
			return true;
		S_WORD LoginPeriodS = pHI->m_LoginPeriodS;
		if (FLOWCTRL.FlowCtrlEnable() == true)
			LoginPeriodS = FLOWCTRL.GetLogonInterSecond();
		if (CONTEXT.GetSystemTick() - m_tickLastSendLoginCmd >= LoginPeriodS)
			return true;
		SleepSecond(1);
		if (m_fPrompt == true)
		{
			m_fPrompt = false;
			logwt<<"FLOWCTRL:约在"<<FLOWCTRL.GetLogonInterSecond()<<"秒后登陆......"<<endl;
		}
	}
	return false;
}

bool C_ReportLogin::BuildupReportData(S_ReportBuffer &ReportCmds)
{
	string strCmd;
	S_HttxInfo *pHI = STATUS.GetHttxInfo(m_HttxSN);

	m_pRule->MakeLoginCmd(strCmd);
	ReportCmds.m_strReportCmds.push_back(strCmd);
	ReportCmds.m_ConfirmEnable = true;
	ReportCmds.m_IntervalS = pHI->m_WaitResponseTimeoutS;
	ReportCmds.m_Retry = 0;
	ReportCmds.m_RetryMax = pHI->m_ResendTimes + 1;
	ReportCmds.m_tickSendCmd = 0;
	return true;
}

//
//
//
int C_ReportHB::GetReportTaskID(void)
{
	return RTID_HB;
}

int C_ReportHB::ReportOneRecord(bool fok)
{
	return 0;
}

int C_ReportHB::ReportAllRecord(bool fok)
{
	if (fok == true)
	{
		m_tickLastHeartBeatTime = CONTEXT.GetSystemTick();
		FLOWCTRL.HeartBeatRecord(true);
		logit<<"心跳成功"<<endl;
	}
	else
	{
		FLOWCTRL.HeartBeatRecord(false);
		logwt<<"心跳失败（未收到主站确认）"<<endl;
	}
	return 0;
}

bool C_ReportHB::NeedReport(void) 
{
	S_ConnectStatus *pCS = CONTEXT.GetConnectStatus();
	S_HttxStatus *pHS = CONTEXT.GetHttxStatus(m_HttxSN);
	S_HttxInfo *pHI = STATUS.GetHttxInfo(m_HttxSN);
	S_SYSCFG *pSysCfg = STATUS.GetSystemConfig();

	if (pHS->m_fMasterTerm == true && pCS->m_Httx1Logon == true && pHI->NeedHeartBeat() == true)
	{
		if (m_tickLastHeartBeatTime == 0)
			return true;
		S_TIME tickSystem = CONTEXT.GetSystemTick();
		if ((tickSystem - pHS->m_tickRecvMsData >= pSysCfg->m_HeartBeatIdleTimeS) // 系统空闲时发心跳，收发报文太多有时会干扰主站
			&& (tickSystem - m_tickLastHeartBeatTime >= (S_DWORD)pHI->m_HeartBeatPeriodM*60)) // 心跳时间到了
			return true;
	}
	return false;
}

bool C_ReportHB::BuildupReportData(S_ReportBuffer &ReportCmds)
{
	string strCmd;
	S_HttxInfo *pHI = STATUS.GetHttxInfo(m_HttxSN);

	m_pRule->MakeHeartBeatCmd(strCmd);
	ReportCmds.m_strReportCmds.push_back(strCmd);
	ReportCmds.m_ConfirmEnable = true;
	ReportCmds.m_IntervalS = pHI->m_WaitResponseTimeoutS;
	ReportCmds.m_Retry = 0;
	ReportCmds.m_RetryMax = 1;
	ReportCmds.m_tickSendCmd = 0;
	return true;
}

//
//
//
int C_ReportAlarmT::GetReportTaskID(void)
{
	return RTID_ALARM;
}
int C_ReportAlarmT::ReportOneRecord(bool fok)
{
	return 0;
}
int C_ReportAlarmT::ReportAllRecord(bool fok)
{
	if (fok == true)
		logit<<"重要告警上报成功!"<<endl;
	else
		logit<<"重要告警上报失败!"<<endl;
	return 0;
}
bool C_ReportAlarmT::NeedReport(void)
{
	S_ConnectStatus *pCS = CONTEXT.GetConnectStatus();
	S_HttxInfo *pHI = STATUS.GetHttxInfo(m_HttxSN);
	if (pCS->m_Httx1Logon == false || pHI->m_ReportEnable == false)
		return false;
	S_AlarmStatus *pAS = STATUS.GetAlarmStatus();
	if (pAS->m_tickImportantHappen == 0)
		return false;
	if (CONTEXT.GetSystemTick() - pAS->m_tickImportantHappen < 10)//延时一会,等确定后再上报
		return false;
	pAS->m_tickImportantHappen = 0;
	return true;
}
bool C_ReportAlarmT::BuildupReportData(S_ReportBuffer &ReportCmds)
{
	strings strCmdInfos;
	S_HttxInfo *pHI = STATUS.GetHttxInfo(m_HttxSN);

	if (C_AlmEvt::Report(ALM_TYPE_IMPORTANT, strCmdInfos) <= 0)
	{
		logwt<<"告警上报:重要告警读取失败!!!"<<endl;
		return false;
	}

	m_pRule->MakeReportCmd(MSAFN_READ_ALARM, strCmdInfos, ReportCmds.m_strReportCmds);
	ReportCmds.m_ConfirmEnable = true;
	ReportCmds.m_IntervalS = pHI->m_WaitResponseTimeoutS;
	ReportCmds.m_Retry = 0;
	ReportCmds.m_RetryMax = pHI->m_ResendTimes + 1;
	ReportCmds.m_tickSendCmd = 0;
	logit<<"重要告警准备上报......"<<endl;
	return true;
}

//
//
//
int C_ReportNormalT::GetReportTaskID(void)
{
	return RTID_NORMAL;
}
int C_ReportNormalT::ReportOneRecord(bool fok)
{
	return 0;
}
int C_ReportNormalT::ReportAllRecord(bool fok)
{
	S_NormalTaskInfos *pTaskInfos = STATUS.GetNormalTaskInfos();
	S_NormalTaskInfo *pTaskInfo = &pTaskInfos->m_ItemInfo[m_NormalTaskID];
	if (fok == true)
	{
		//日任务检测
		if (HistoryDayTask(pTaskInfo) == true)
		{
			InsetHistoryTask(m_NormalTaskID, m_HistoryTask);
		}
		logwt<<"普通任务("<<(m_NormalTaskID+1)<<")上报成功!"<<endl;
	}
	else
	{
		//日任务检测
		if (HistoryDayTask(pTaskInfo) == true)
		{
			InsetHistoryTask(m_NormalTaskID, m_HistoryTask);
		}
		logwt<<"普通任务("<<(m_NormalTaskID+1)<<")上报失败!"<<endl;
	}

	UpdateReportTime();
	m_NormalTaskID = -1;
	return 0;
}
bool C_ReportNormalT::NeedReport(void)
{
	S_ConnectStatus *pCS = CONTEXT.GetConnectStatus();
	S_HttxInfo *pHI = STATUS.GetHttxInfo(m_HttxSN);
	if (pCS->m_Httx1Logon == false || pHI->m_ReportEnable == false)
		return false;
	return NormalTaskRaise();
}
bool C_ReportNormalT::BuildupReportData(S_ReportBuffer &ReportCmds)
{
	strings strCmdInfos;
	C_CmdTask CmdTask;
	S_HttxInfo *pHI = STATUS.GetHttxInfo(m_HttxSN);
	S_NormalTaskInfos *pTaskInfos = STATUS.GetNormalTaskInfos();
	S_NormalTaskStatus *pTaskStatus = &pTaskInfos->m_ItemStatus[m_NormalTaskID];
	S_NormalTaskInfo *pTaskInfo = &pTaskInfos->m_ItemInfo[m_NormalTaskID];

	/**@brief 数据采样时间从上报时间往前推若干个周期m_ReportPeriodVal个m_ReportPeriodUnit*/
	S_ALLMP *pAllMP = STATUS.GetALLMP();
	m_HistoryTask.m_timeReportBegin = pTaskStatus->m_timeLastReport;
	m_HistoryTask.m_timeReportEnd = m_HistoryTask.m_timeLastReport = TrimReportTime(pTaskInfo, C_TIME::TimeNow());
	m_HistoryTask.m_MPBS = pAllMP->m_MTBS.m_AllMPBS;

	logwt<<"普通任务("<<(m_NormalTaskID+1)<<")取数据开始......"<<endl;
	CmdTask.SetPackageLength(m_pRule->GetPackageLength());
	if (CmdTask.GetReportData(m_NormalTaskID+1, pTaskInfo, &m_HistoryTask, strCmdInfos) <= 0)
	{//取数失败
		logwt<<"普通任务("<<(m_NormalTaskID+1)<<")取数据失败!"<<endl;
		if (HistoryDayTask(pTaskInfo) == true)
		{
			InsetHistoryTask(m_NormalTaskID, m_HistoryTask);
		}
		UpdateReportTime();
		m_NormalTaskID = -1;
		return false;
	}

	m_HistoryTask.m_MPBS.Stat();
	m_pRule->MakeReportCmd(MSAFN_READ_TASK, strCmdInfos, ReportCmds.m_strReportCmds);
	ReportCmds.m_ConfirmEnable = true;
	ReportCmds.m_IntervalS = pHI->m_WaitResponseTimeoutS;
	ReportCmds.m_Retry = 0;
	ReportCmds.m_RetryMax = pHI->m_ResendTimes + 1;
	ReportCmds.m_tickSendCmd = 0;
	logwt<<"普通任务("<<(m_NormalTaskID+1)<<")准备上报......"<<endl;
	return true;
}

bool C_ReportNormalT::NormalTaskRaise(void)
{
	S_TIME timeRefer = C_TIME::TimeNow();
	S_NormalTaskInfos *pTaskInfos = STATUS.GetNormalTaskInfos();
	for (int i = 0; i < NORMAL_TASK_MAX; i++)
	{
		if (NormalTaskRaise(&pTaskInfos->m_ItemInfo[i], &pTaskInfos->m_ItemStatus[i], timeRefer) == true)
		{
			m_NormalTaskID = i;
			return true;
		}
	}
	return false;
}

extern const long g_Seconds4TimeUnit[] = {60, 60*60, 60*60*24};
bool C_ReportNormalT::NormalTaskRaise(const S_NormalTaskInfo *pTaskInfo, S_NormalTaskStatus *pTaskStatus, S_TIME timeRefer)
{
	if (pTaskInfo->m_Enable == false || pTaskInfo->m_Active == false)
		return false;//invalid task
	if (pTaskInfo->m_ReportPeriodVal == 0 || pTaskInfo->m_ReportPeriodUnit > 3)
		return false;//wrong param
	if (pTaskInfo->m_ReportStdTime > timeRefer)
		return false;

	if (pTaskStatus->m_timeLastReport == 0) //first report after reboot
	{// 防止主站设置一个太大的超过当前的时间
		S_DATE date;
		S_NormalTaskInfo TaskInfo = *pTaskInfo;
		C_TIME::LocalTime(TaskInfo.m_ReportStdTime, date);
		date.m_Year = 2000; // 初始为2000年
		TaskInfo.m_ReportStdTime = C_TIME::MakeTime(date);
		pTaskStatus->m_timeLastReport = TrimReportTime(&TaskInfo, timeRefer - 60*2);
	}
	if (pTaskStatus->m_timeLastReport > timeRefer)//fix report time after change system time
		pTaskStatus->m_timeLastReport = TrimReportTime(pTaskInfo, timeRefer);

	if (pTaskInfo->m_ReportPeriodUnit == 3)//month
	{
		S_TIME NextReport = C_TIME::DateAdd(TIME_UNIT_MONTH, pTaskInfo->m_ReportPeriodVal, pTaskStatus->m_timeLastReport);
		if (timeRefer >= NextReport)
			return true;
	}
	else//minute, hour, day
	{
		long Interval = pTaskInfo->m_ReportPeriodVal * g_Seconds4TimeUnit[pTaskInfo->m_ReportPeriodUnit];
		if (timeRefer >= pTaskStatus->m_timeLastReport + Interval)
			return true;
	}
	return false;
}

S_TIME C_ReportNormalT::TrimReportTime(const S_NormalTaskInfo *pTaskInfo, S_TIME timeRefer)
{
	if (timeRefer < pTaskInfo->m_ReportStdTime)
		return 0;

	if (pTaskInfo->m_ReportPeriodUnit == 3)//month
	{
		long MonCnt = C_TIME::DateDiff(TIME_UNIT_MONTH, pTaskInfo->m_ReportStdTime, timeRefer);
		long MonDiff = MonCnt - (MonCnt % pTaskInfo->m_ReportPeriodVal);
		S_TIME ReporTime = C_TIME::DateAdd(TIME_UNIT_MONTH, MonDiff, pTaskInfo->m_ReportStdTime);
		if (ReporTime > timeRefer)
			ReporTime = C_TIME::DateAdd(TIME_UNIT_MONTH, -(pTaskInfo->m_ReportPeriodVal), ReporTime);
		return ReporTime;
	}
	//minute, hour, day
	long Interval = pTaskInfo->m_ReportPeriodVal * g_Seconds4TimeUnit[pTaskInfo->m_ReportPeriodUnit];
	S_TIME diff = (timeRefer - pTaskInfo->m_ReportStdTime) % Interval;
	return timeRefer - diff;
}

void C_ReportNormalT::UpdateReportTime(void)
{
	S_NormalTaskInfos *pTaskInfos = STATUS.GetNormalTaskInfos();
	pTaskInfos->m_ItemStatus[m_NormalTaskID].m_timeLastReport = TrimReportTime(&pTaskInfos->m_ItemInfo[m_NormalTaskID], C_TIME::TimeNow());
}

bool C_ReportNormalT::HistoryDayTask(const S_NormalTaskInfo *pTaskInfo)
{
	S_SYSCFG *pSysCfg = STATUS.GetSystemConfig();
	if (pSysCfg->m_ReportAgainIntervalM == 0x00)
		return false;
	return pTaskInfo->HistoryDayTask();//采样周期为日
}

bool C_ReportNormalT::InsetHistoryTask(int NormalTaskID, S_HistoryTask &HistoryTask)
{
	S_NormalTaskInfos *pTaskInfos = STATUS.GetNormalTaskInfos();
	S_NormalTaskStatus *pTaskStatus = &pTaskInfos->m_ItemStatus[NormalTaskID];
	S_HistoryTasks *pHistoryTasks = &pTaskStatus->m_HistoryTasks;
	S_HistoryTask *pMinTask = GetHistoryTaskMin(*pHistoryTasks);
	*pMinTask = HistoryTask;
	return true;
}

S_HistoryTask *C_ReportNormalT::GetHistoryTaskMin(S_HistoryTasks &HistoryTasks)
{
	S_HistoryTask *pCtrl = &HistoryTasks.m_Item[0];
	for (int i = 1; i < TASK_REPORT_HISTORY_DAY; i++)
	{
		if (pCtrl->m_timeReportBegin > HistoryTasks.m_Item[i].m_timeReportBegin)
			pCtrl = &HistoryTasks.m_Item[i];
	}
	return pCtrl;
}

//
//
//
int C_ReportDataRouteT::GetReportTaskID(void)
{
	return RTID_DATAROUTE;
}

int C_ReportDataRouteT::ReportOneRecord(bool fok)
{
	return 0;
}

int C_ReportDataRouteT::ReportAllRecord(bool fok)
{
	//S_ForwardTaskInfos *pTaskInfos = STATUS.GetForwardTaskInfos();
	//S_ForwardTaskInfo *pTaskInfo = &pTaskInfos->m_ItemInfo[m_ForwardTaskID];
	if (fok == true)
	{
		logwt<<"中继任务("<<m_ForwardTaskID<<")上报成功!"<<endl;
	}
	else
	{
		logwt<<"中继任务("<<m_ForwardTaskID<<")上报失败!"<<endl;
	}

	UpdateReportTime();
	m_ForwardTaskID = -1;
	return 0;
}

bool C_ReportDataRouteT::NeedReport(void)
{
	S_ConnectStatus *pCS = CONTEXT.GetConnectStatus();
	S_HttxInfo *pHI = STATUS.GetHttxInfo(m_HttxSN);
	if (pCS->m_Httx1Logon == false || pHI->m_ReportEnable == false)
		return false;
	return ForwardTaskRaise();
}

bool C_ReportDataRouteT::BuildupReportData(S_ReportBuffer &ReportCmds)
{
	strings strCmdInfos;
	C_CmdTask CmdTask;
	S_HttxInfo *pHI = STATUS.GetHttxInfo(m_HttxSN);
	S_ForwardTaskInfos *pTaskInfos = STATUS.GetForwardTaskInfos();
	S_ForwardTaskInfo *pTaskInfo = &pTaskInfos->m_ItemInfo[m_ForwardTaskID];

	S_ForwardData *pForwardData =CONTEXT.GetForwardTaskData(m_ForwardTaskID);
	CmdTask.SetPackageLength(m_pRule->GetPackageLength());
	if (!GetForwardTaskData(m_ForwardTaskID, pTaskInfo, pForwardData, strCmdInfos))
	{//取数失败
		logwt<<"中继任务("<<m_ForwardTaskID<<")取数据失败......"<<endl;
		UpdateReportTime();
		m_ForwardTaskID = -1;
		return false;
	}
	
	m_pRule->MakeReportCmd(MSAFN_READ_TASK, strCmdInfos, ReportCmds.m_strReportCmds);
	ReportCmds.m_ConfirmEnable = true;
	ReportCmds.m_IntervalS = pHI->m_WaitResponseTimeoutS;
	ReportCmds.m_Retry = 0;
	ReportCmds.m_RetryMax = pHI->m_ResendTimes + 1;
	ReportCmds.m_tickSendCmd = 0;
	logwt<<"中继任务("<<m_ForwardTaskID<<")准备上报......"<<endl;
	return true;
}

bool C_ReportDataRouteT::GetForwardTaskData(int ForwardTaskID,const S_ForwardTaskInfo *pTaskInfo,S_ForwardData *pForwardData,strings &strCmdInfos)
{
	S_WORD DA = 0x00;
	S_DWORD DI = 0xE3020001; // ：中继转发
	string strIpcData;
	strIpcData.append((char *)&DA, 2);
	strIpcData.append((char *)&DI, 4);
	strIpcData.push_back(pTaskInfo->m_ForwardType);
	strIpcData.push_back((S_BYTE)(pTaskInfo->m_PortSet.m_PortNO));
	strIpcData.push_back((S_BYTE)(pTaskInfo->m_PortSet.m_Baud));
	strIpcData.push_back((S_BYTE)(pTaskInfo->m_PortSet.m_Parity));
	strIpcData.push_back((S_BYTE)(pTaskInfo->m_PortSet.m_DataBit));
	strIpcData.push_back((S_BYTE)(pTaskInfo->m_PortSet.m_StopBit));
	strIpcData.push_back((S_BYTE)(pTaskInfo->m_ForwardCmdLen));
	strIpcData.append(pTaskInfo->m_ForwardCmd, pTaskInfo->m_ForwardCmdLen);

	if (C_SysMsg::Send2Server(CONTEXT.GetDbcjPrecessName_VCom((S_BYTE)(pTaskInfo->m_PortSet.m_PortNO+1)), MSAFN_DATA_ROUTE, strIpcData) != 0)
		return false;
	strIpcData.clear();
	if (C_SysMsg::Wait4Server(MSAFN_DATA_ROUTE, strIpcData) != 0)
		return false;
	if (strIpcData.size() <= 6)
		return false;

	string strForwardData;
	S_DWORD ForwardDI = FORWARD_TASK_ID_MIN + ForwardTaskID;
	strForwardData.append((char *)&DA, 2);
	strForwardData.append((char *)&ForwardDI, 4);
	strForwardData.push_back(pTaskInfo->m_ForwardType);
	strForwardData.push_back((S_BYTE)(strIpcData.size() - 6));
	strForwardData.append(strIpcData.begin()+6, strIpcData.end());

	strCmdInfos.push_back(strForwardData);
	return true;
}

bool C_ReportDataRouteT::ForwardTaskRaise(void)
{
	S_TIME timeRefer = C_TIME::TimeNow();
	S_ForwardTaskInfos *pTaskInfos = STATUS.GetForwardTaskInfos();
	for (int i = 0; i < NORMAL_TASK_MAX; i++)
	{
		if (ForwardTaskRaise(&pTaskInfos->m_ItemInfo[i], &pTaskInfos->m_ItemStatus[i], timeRefer) == true)
		{
			m_ForwardTaskID = i;
			return true;
		}
	}
	return false;
}

bool C_ReportDataRouteT::ForwardTaskRaise(const S_ForwardTaskInfo *pTaskInfo, S_ForwardTaskStatus *pTaskStatus, S_TIME timeRefer)
{
	if (pTaskInfo->m_Enable == false || pTaskInfo->m_Active == false)
		return false;//invalid task
	if (pTaskInfo->m_ReportPeriodVal == 0 || pTaskInfo->m_ReportPeriodUnit > 3)
		return false;//wrong param
	if (pTaskInfo->m_ReportStdTime > timeRefer)
		return false;

	if (pTaskStatus->m_timeLastReport == 0) //first report after reboot
	{// 防止主站设置一个太大的超过当前的时间
		S_DATE date;
		S_ForwardTaskInfo TaskInfo = *pTaskInfo;
		C_TIME::LocalTime(TaskInfo.m_ReportStdTime, date);
		date.m_Year = 2000; // 初始为2000年
		TaskInfo.m_ReportStdTime = C_TIME::MakeTime(date);
		pTaskStatus->m_timeLastReport = TrimReportTime(&TaskInfo, timeRefer - 60*2);
	}
	if (pTaskStatus->m_timeLastReport > timeRefer)//fix report time after change system time
		pTaskStatus->m_timeLastReport = TrimReportTime(pTaskInfo, timeRefer);

	if (pTaskInfo->m_ReportPeriodUnit == 3)//month
	{
		S_TIME NextReport = C_TIME::DateAdd(TIME_UNIT_MONTH, pTaskInfo->m_ReportPeriodVal, pTaskStatus->m_timeLastReport);
		if (timeRefer >= NextReport)
			return true;
	}
	else//minute, hour, day
	{
		long Interval = pTaskInfo->m_ReportPeriodVal * g_Seconds4TimeUnit[pTaskInfo->m_ReportPeriodUnit];
		if (timeRefer >= pTaskStatus->m_timeLastReport + Interval)
			return true;
	}
	return false;
}

S_TIME C_ReportDataRouteT::TrimReportTime(const S_ForwardTaskInfo *pTaskInfo, S_TIME timeRefer)
{
	if (timeRefer < pTaskInfo->m_ReportStdTime)
		return 0;

	if (pTaskInfo->m_ReportPeriodUnit == 3)//month
	{
		long MonCnt = C_TIME::DateDiff(TIME_UNIT_MONTH, pTaskInfo->m_ReportStdTime, timeRefer);
		long MonDiff = MonCnt - (MonCnt % pTaskInfo->m_ReportPeriodVal);
		S_TIME ReporTime = C_TIME::DateAdd(TIME_UNIT_MONTH, MonDiff, pTaskInfo->m_ReportStdTime);
		if (ReporTime > timeRefer)
			ReporTime = C_TIME::DateAdd(TIME_UNIT_MONTH, -(pTaskInfo->m_ReportPeriodVal), ReporTime);
		return ReporTime;
	}
	//minute, hour, day
	long Interval = pTaskInfo->m_ReportPeriodVal * g_Seconds4TimeUnit[pTaskInfo->m_ReportPeriodUnit];
	S_TIME diff = (timeRefer - pTaskInfo->m_ReportStdTime) % Interval;
	return timeRefer - diff;
}

void C_ReportDataRouteT::UpdateReportTime(void)
{
	S_ForwardTaskInfos *pTaskInfos = STATUS.GetForwardTaskInfos();
	pTaskInfos->m_ItemStatus[m_ForwardTaskID].m_timeLastReport = TrimReportTime(&pTaskInfos->m_ItemInfo[m_ForwardTaskID], C_TIME::TimeNow());
}

//
//
//
int C_ReportAgainT::GetReportTaskID(void)
{
	return RTID_AGAIN;
}
int C_ReportAgainT::ReportOneRecord(bool fok)
{
	return 0;
}
int C_ReportAgainT::ReportAllRecord(bool fok)
{
	char buffer[20];
	S_NormalTaskInfos *pTaskInfos = STATUS.GetNormalTaskInfos();
	S_NormalTaskStatus *pTaskStatus = &pTaskInfos->m_ItemStatus[m_NormalTaskID];
	S_HistoryTask *pHistoryTask = &pTaskStatus->m_HistoryTasks.m_Item[m_HistoryTaskID];
	memset(buffer, 0x00, sizeof(buffer));

	if (fok == true)
	{
		pHistoryTask->m_MPBS = m_HistoryTask.m_MPBS;//更新表计信息
		logwt<<"当日补报任务(任务号:"<<(m_NormalTaskID+1)<<",时间:"<<C_TIME::Time2Ascii14(pHistoryTask->m_timeReportBegin, buffer)<<")上报成功!"<<endl;
	}
	else
	{
		logwt<<"当日补报任务(任务号:"<<(m_NormalTaskID+1)<<",时间:"<<C_TIME::Time2Ascii14(pHistoryTask->m_timeReportBegin, buffer)<<")上报失败!"<<endl;
	}

	S_SYSCFG *pSysCfg = STATUS.GetSystemConfig();
	m_timeLastReport[m_NormalTaskID] += 60 * pSysCfg->m_ReportAgainIntervalM;
	m_NormalTaskID = -1, m_HistoryTaskID = -1;
	return 0;
}
bool C_ReportAgainT::NeedReport(void)
{
	S_ConnectStatus *pCS = CONTEXT.GetConnectStatus();
	S_HttxInfo *pHI = STATUS.GetHttxInfo(m_HttxSN);
	S_SYSCFG *pSysCfg = STATUS.GetSystemConfig();
	if (pCS->m_Httx1Logon == false || pHI->m_ReportEnable == false)
		return false;
	if (pSysCfg->m_ReportAgainIntervalM == 0x00)
		return false;
	return HistoryTaskRaise();
}
bool C_ReportAgainT::BuildupReportData(S_ReportBuffer &ReportCmds)
{
	char buffer[20];
	strings strCmdInfos;
	C_CmdTask CmdTask;
	S_HttxInfo *pHI = STATUS.GetHttxInfo(m_HttxSN);
	S_SYSCFG *pSysCfg = STATUS.GetSystemConfig();
	S_NormalTaskInfos *pTaskInfos = STATUS.GetNormalTaskInfos();
	S_NormalTaskStatus *pTaskStatus = &pTaskInfos->m_ItemStatus[m_NormalTaskID];
	S_NormalTaskInfo TaskInfo = pTaskInfos->m_ItemInfo[m_NormalTaskID];

	memset(buffer, 0x00, sizeof(buffer));
	m_HistoryTask = pTaskStatus->m_HistoryTasks.m_Item[m_HistoryTaskID];
	TaskInfo.m_DataFormat = 0; // 自描述方式组织数据

	logwt<<"当日补报任务(任务号:"<<(m_NormalTaskID+1)<<",时间:"<<C_TIME::Time2Ascii14(m_HistoryTask.m_timeReportBegin, buffer)<<")取数开始..."<<endl;
	CmdTask.SetPackageLength(m_pRule->GetPackageLength());
	if (CmdTask.GetReportData(m_NormalTaskID+1, &TaskInfo, &m_HistoryTask, strCmdInfos) <= 0)
	{//取数失败
		logwt<<"当日补报任务(任务号:"<<(m_NormalTaskID+1)<<",时间:"<<C_TIME::Time2Ascii14(m_HistoryTask.m_timeReportBegin, buffer)<<")取数失败!"<<endl;
		m_timeLastReport[m_NormalTaskID] += pSysCfg->m_ReportAgainIntervalM * 60;//更新上报时间
		m_NormalTaskID = -1, m_HistoryTaskID = -1;
		return false;
	}

	m_HistoryTask.m_MPBS.Stat();
	m_pRule->MakeReportCmd(MSAFN_READ_TASK, strCmdInfos, ReportCmds.m_strReportCmds);
	ReportCmds.m_ConfirmEnable = true;
	ReportCmds.m_IntervalS = pHI->m_WaitResponseTimeoutS;
	ReportCmds.m_Retry = 0;
	ReportCmds.m_RetryMax = pHI->m_ResendTimes + 1;
	ReportCmds.m_tickSendCmd = 0;
	logwt<<"当日补报任务(任务号:"<<(m_NormalTaskID+1)<<",时间:"<<C_TIME::Time2Ascii14(m_HistoryTask.m_timeReportBegin, buffer)<<")准备上报..."<<endl;
	return true;
}

bool C_ReportAgainT::HistoryTaskRaise(void)
{
	S_TIME timeRefer = C_TIME::TimeNow();
	S_NormalTaskInfos *pTaskInfos = STATUS.GetNormalTaskInfos();
	for (int i = 0; i < NORMAL_TASK_MAX; i++)
	{
		S_HistoryTasks *pHistoryTasks = &pTaskInfos->m_ItemStatus[i].m_HistoryTasks;
		int idxMax = GetHistoryTaskMax(*pHistoryTasks);
		if (HistoryTaskRaise(&pTaskInfos->m_ItemInfo[i], &pHistoryTasks->m_Item[idxMax], timeRefer) == true
			&& HistoryTaskRaise(i, timeRefer) == true)
		{
			m_NormalTaskID = i, m_HistoryTaskID = idxMax;
			return true;
		}
	}
	return false;
}

bool C_ReportAgainT::HistoryTaskRaise(const S_NormalTaskInfo *pTaskInfo, S_HistoryTask *pHistoryTask, S_TIME timeRefer)
{
	if (pHistoryTask->m_timeReportBegin == 0)
		return false;//历史任务不存在
	if (pHistoryTask->m_timeLastReport > timeRefer)
		return false;//时间不对

	S_DATE HistoryDate, CurrentDate;
	C_TIME::LocalTime(pHistoryTask->m_timeReportEnd, HistoryDate);
	C_TIME::LocalTime(timeRefer, CurrentDate);
	if (HistoryDate.m_Year != CurrentDate.m_Year || HistoryDate.m_Mon != CurrentDate.m_Mon || HistoryDate.m_Day != CurrentDate.m_Day)
		return false;//不是当天的任务，本任务只报当天的任务

	if (pTaskInfo->m_Enable == false || pTaskInfo->m_Active == false)
		return false;//invalid task
	if (pTaskInfo->m_ReportPeriodVal == 0 || pTaskInfo->m_ReportPeriodUnit > 3)
		return false;//wrong param
	if (pTaskInfo->m_ReportStdTime > timeRefer)
		return false;
	if (HistoryDayTask(pTaskInfo) == false)
		return false;

	return true;
}

bool C_ReportAgainT::HistoryTaskRaise(int NormalTaskID, S_TIME timeRefer)
{
	S_SYSCFG *pSysCfg = STATUS.GetSystemConfig();
	if (m_timeLastReport[NormalTaskID] == 0x00)
	{
		S_TIME interval = pSysCfg->m_ReportAgainIntervalM * 60;
		S_TIME diff = timeRefer % interval;
		m_timeLastReport[NormalTaskID] = timeRefer - diff;
	}
	if (m_timeLastReport[NormalTaskID] > timeRefer)
		return false;//时间不对
	if (timeRefer - m_timeLastReport[NormalTaskID] <= (S_DWORD)pSysCfg->m_ReportAgainIntervalM * 60)
		return false;
	return true;
}

bool C_ReportAgainT::HistoryDayTask(const S_NormalTaskInfo *pTaskInfo)
{
	return pTaskInfo->HistoryDayTask();//采样周期为日
}

int C_ReportAgainT::GetHistoryTaskMax(S_HistoryTasks &HistoryTasks)
{
	int idxMax = 0;
	for (int i = 1; i < TASK_REPORT_HISTORY_DAY; i++)
	{
		if (HistoryTasks.m_Item[idxMax].m_timeReportBegin < HistoryTasks.m_Item[i].m_timeReportBegin)
			idxMax = i;
	}
	return idxMax;
}


//
//
//
int C_ReportHistoryT::GetReportTaskID(void)
{
	return RTID_HISTORY;
}
int C_ReportHistoryT::ReportOneRecord(bool fok)
{
	return 0;
}
int C_ReportHistoryT::ReportAllRecord(bool fok)
{
	char buffer[20];
	S_NormalTaskInfos *pTaskInfos = STATUS.GetNormalTaskInfos();
	S_NormalTaskStatus *pTaskStatus = &pTaskInfos->m_ItemStatus[m_NormalTaskID];
	S_HistoryTask *pHistoryTask = &pTaskStatus->m_HistoryTasks.m_Item[m_HistoryTaskID];
	memset(buffer, 0x00, sizeof(buffer));

	if (fok == true)
	{
		pHistoryTask->m_MPBS = m_HistoryTask.m_MPBS;//更新表计信息
		logwt<<"历史日补报任务(任务号:"<<(m_NormalTaskID+1)<<",时间:"<<C_TIME::Time2Ascii14(pHistoryTask->m_timeReportBegin, buffer)<<")上报成功!"<<endl;
	}
	else
	{
		logwt<<"历史日补报任务(任务号:"<<(m_NormalTaskID+1)<<",时间:"<<C_TIME::Time2Ascii14(pHistoryTask->m_timeReportBegin, buffer)<<")上报失败!"<<endl;
	}

	pHistoryTask->m_timeLastReport += 24*60*60;
	m_NormalTaskID = -1, m_HistoryTaskID = -1;
	return 0;
}
bool C_ReportHistoryT::NeedReport(void)
{
	S_ConnectStatus *pCS = CONTEXT.GetConnectStatus();
	S_HttxInfo *pHI = STATUS.GetHttxInfo(m_HttxSN);
	S_SYSCFG *pSysCfg = STATUS.GetSystemConfig();
	if (pCS->m_Httx1Logon == false || pHI->m_ReportEnable == false)
		return false;
	if (pSysCfg->m_ReportAgainIntervalM == 0x00)
		return false;
	return HistoryTaskRaise();
}
bool C_ReportHistoryT::BuildupReportData(S_ReportBuffer &ReportCmds)
{
	char buffer[20];
	strings strCmdInfos;
	C_CmdTask CmdTask;
	S_HttxInfo *pHI = STATUS.GetHttxInfo(m_HttxSN);
	S_NormalTaskInfos *pTaskInfos = STATUS.GetNormalTaskInfos();
	S_NormalTaskStatus *pTaskStatus = &pTaskInfos->m_ItemStatus[m_NormalTaskID];
	S_NormalTaskInfo TaskInfo = pTaskInfos->m_ItemInfo[m_NormalTaskID];

	memset(buffer, 0x00, sizeof(buffer));
	m_HistoryTask = pTaskStatus->m_HistoryTasks.m_Item[m_HistoryTaskID];
	TaskInfo.m_DataFormat = 0; // 自描述方式组织数据

	logwt<<"历史日补报任务(任务号:"<<(m_NormalTaskID+1)<<",时间:"<<C_TIME::Time2Ascii14(m_HistoryTask.m_timeReportBegin, buffer)<<")取数开始..."<<endl;
	CmdTask.SetPackageLength(m_pRule->GetPackageLength());
	if (CmdTask.GetReportData(m_NormalTaskID+1, &TaskInfo, &m_HistoryTask, strCmdInfos) <= 0)
	{//取数失败
		logwt<<"历史日补报任务(任务号:"<<(m_NormalTaskID+1)<<",时间:"<<C_TIME::Time2Ascii14(m_HistoryTask.m_timeReportBegin, buffer)<<")取数失败!"<<endl;
		pTaskStatus->m_HistoryTasks.m_Item[m_HistoryTaskID].m_timeLastReport += 24*60*60;//更新上报时间
		m_NormalTaskID = -1, m_HistoryTaskID = -1;
		return false;
	}

	m_HistoryTask.m_MPBS.Stat();
	m_pRule->MakeReportCmd(MSAFN_READ_TASK, strCmdInfos, ReportCmds.m_strReportCmds);
	ReportCmds.m_ConfirmEnable = true;
	ReportCmds.m_IntervalS = pHI->m_WaitResponseTimeoutS;
	ReportCmds.m_Retry = 0;
	ReportCmds.m_RetryMax = pHI->m_ResendTimes + 1;
	ReportCmds.m_tickSendCmd = 0;
	logwt<<"历史日补报任务(任务号:"<<(m_NormalTaskID+1)<<",时间:"<<C_TIME::Time2Ascii14(m_HistoryTask.m_timeReportBegin, buffer)<<")准备上报..."<<endl;
	return true;
}

bool C_ReportHistoryT::HistoryTaskRaise(void)
{
	S_TIME timeRefer = C_TIME::TimeNow();
	S_NormalTaskInfos *pTaskInfos = STATUS.GetNormalTaskInfos();
	for (int i = 0; i < NORMAL_TASK_MAX; i++)
	{
		S_HistoryTasks *pHistoryTasks = &pTaskInfos->m_ItemStatus[i].m_HistoryTasks;
		for (int j = 0; j < TASK_REPORT_HISTORY_DAY; j++)
		{
			if (HistoryTaskRaise(&pTaskInfos->m_ItemInfo[i], &pHistoryTasks->m_Item[j], timeRefer) == true)
			{
				m_NormalTaskID = i, m_HistoryTaskID = j;
				return true;
			}
		}
	}
	return false;
}

bool C_ReportHistoryT::HistoryTaskRaise(const S_NormalTaskInfo *pTaskInfo, S_HistoryTask *pHistoryTask, S_TIME timeRefer)
{
	if (pHistoryTask->m_timeReportBegin == 0)
		return false;//历史任务不存在
	if (pHistoryTask->m_timeLastReport > timeRefer)
		return false;//时间不对
	if (timeRefer - pHistoryTask->m_timeLastReport < 24*60*60)
		return false;//时间未到
	if (timeRefer - pHistoryTask->m_timeLastReport > 24*60*60*TASK_REPORT_HISTORY_DAY)
	{
		memset(pHistoryTask, 0x00, sizeof(*pHistoryTask));
		return false;//任务太老了
	}

	if (pTaskInfo->m_Enable == false || pTaskInfo->m_Active == false)
		return false;//invalid task
	if (pTaskInfo->m_ReportPeriodVal == 0 || pTaskInfo->m_ReportPeriodUnit > 3)
		return false;//wrong param
	if (pTaskInfo->m_ReportStdTime > timeRefer)
		return false;
	if (HistoryDayTask(pTaskInfo) == false)
		return false;

	return true;
}

bool C_ReportHistoryT::HistoryDayTask(const S_NormalTaskInfo *pTaskInfo)
{
	return pTaskInfo->HistoryDayTask();//采样周期为日
}

//
//
//
C_ReportHandle *C_ReportFact::GetReportHandle(int ReportID)
{
	static C_ReportLogin sDefReportLogin;
	static C_ReportHB sDefReportHB;
	static C_ReportAlarmT sDefReportAlarmT;
	static C_ReportNormalT sDefReportNormalT;
	static C_ReportDataRouteT sDefReportDataRouteT;
	static C_ReportAgainT sDefReportAgainT;
	static C_ReportHistoryT sDefReportHistoryT;
	switch(ReportID)
	{
	case RTID_LOGIN:return &sDefReportLogin;
	case RTID_HB:return &sDefReportHB;
	case RTID_ALARM:return &sDefReportAlarmT;
	case RTID_NORMAL:return &sDefReportNormalT;
	case RTID_DATAROUTE:return &sDefReportDataRouteT;
	case RTID_AGAIN:return &sDefReportAgainT;
	case RTID_HISTORY:return &sDefReportHistoryT;
	}
	return C_ReportFactBase::GetReportHandle(ReportID);
};



