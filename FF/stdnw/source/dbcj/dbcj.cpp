/** @file
 *  @brief 表计能量采集功能定义
 *  @author 
 *  @date 2011/11/30
 *  @version 0.1
 */
#include "dbcj.h"
#include "sysipc.h"
#include "htrace.h"
#include "status.h"
#include "nwmsrule.h"
#include "nwmscmd.h"
#include "context.h"
//#include "alarmEvent.h"
#include "termcom.h"
#include "mrfact.h"
#include "almfact.h"
#include "fbfact.h"
#include "busialgo.h"

static C_TermCom g_GW_TermCom;

//
//
//
int S_FreezeTasks::AddTask(const S_FreezeTask &FreezeTask)
{
	for (S_FreezeTasks::iterator iter = begin(); iter != end(); ++iter)
	{
		if (iter->m_DT == FreezeTask.m_DT)
		{
			iter->m_mpbsMeter |= FreezeTask.m_mpbsMeter;
			iter->m_mpbsMeter.Stat();
			return 0;
		}
	}

	push_back(FreezeTask);
	return 0;
}

//
//
//
int C_DbcjServer::BeforeService(void)
{
	m_Status = DBCJ_STATUS_INIT;
	m_pConnRS485I = NULL, m_pConnRS485II = NULL;
	m_pConnZBWX = NULL, m_pConnJLCY = NULL;
	m_DnModuleResetCnt = 0x00;
	
	S_DbcjStatus *pStatus = CONTEXT.GetDbcjStatus(m_DbcjSN);
	pStatus->m_bsMyVCom.reset();
	pStatus->m_mpbsMyMeter.reset();

	S_CHANNEL *p485I = STATUS.GetDbcjChannel(m_DbcjSN, DBCJ_CHANNL_RS485I);
	if (p485I != NULL)
	{
		m_pConnRS485I = C_ConnFact::CreateConnection(*p485I);
		pStatus->m_bsMyVCom.set(p485I->m_PortSet.m_ComSet.m_PortNO);
	}
	S_CHANNEL *p485II = STATUS.GetDbcjChannel(m_DbcjSN, DBCJ_CHANNL_RS485II);
	if (p485II != NULL)
	{
		m_pConnRS485II = C_ConnFact::CreateConnection(*p485II);
		pStatus->m_bsMyVCom.set(p485II->m_PortSet.m_ComSet.m_PortNO);
	}
	S_CHANNEL *pZbwx = STATUS.GetDbcjChannel(m_DbcjSN, DBCJ_CHANNL_ZBWX);
	if (pZbwx != NULL)
	{
		m_pConnZBWX = C_ConnFact::CreateConnection(*pZbwx);
		pStatus->m_bsMyVCom.set(pZbwx->m_PortSet.m_ComSet.m_PortNO);
	}

	pStatus->m_bsMyVCom.Stat();
	m_TaskSchedule.SetDbcjSN(m_DbcjSN);
	C_ComObjFact::SetComObj(GATHER_COM_OBJ_TERM, &g_GW_TermCom);
	return 0;
}

int C_DbcjServer::AfterService(void)
{
	return 0;
}

int C_DbcjServer::OnTickService(void)
{
	switch (m_Status)
	{
	case DBCJ_STATUS_INIT:
		if (InitGatherInfo() == 0)
			m_Status = DBCJ_STATUS_SYNC_STATUS;
		break;
	case DBCJ_STATUS_SYNC_STATUS:
		if (SyncGatherStatus() == 0)
			m_Status = DBCJ_STATUS_CREATE_TASK;
		else
			m_Status = DBCJ_STATUS_INIT;
		break;
	case DBCJ_STATUS_CREATE_TASK:
		if (CreateAllTask() == 0)
			m_Status = DBCJ_STATUS_RUNNING;
		break;
	case DBCJ_STATUS_RUNNING:
		m_Status = TaskSchedule();
		break;
	default:
		m_Status = DBCJ_STATUS_INIT;
		break;
	}
	return 0;
}

int C_DbcjServer::InitGatherInfo(void)
{
	S_DbcjStatus *pDbcjStatus = CONTEXT.GetDbcjStatus(MP_DBCJ_1);
	pDbcjStatus->m_LcdBottomInfoIndicator = 0x00; // 初始化采集系统

	memset(pDbcjStatus->m_RouterAddr, 0x00, sizeof(pDbcjStatus->m_RouterAddr));
	memset(pDbcjStatus->m_RouterChip, 0x00, sizeof(pDbcjStatus->m_RouterChip));
	memset(pDbcjStatus->m_RouterDate, 0x00, sizeof(pDbcjStatus->m_RouterDate));
	memset(pDbcjStatus->m_RouterFactory, 0x00, sizeof(pDbcjStatus->m_RouterFactory));
	memset(pDbcjStatus->m_RouterVersion, 0x00, sizeof(pDbcjStatus->m_RouterVersion));

	if (m_MeterCom.InitMeterRules(CFG_PATH_REGISTER) < 0)
	{
		loget<<"表计规约初始化失败！！！"<<endl;
		return -1;
	}
	if (m_DnModule.InitFiledBus(m_pConnRS485I, m_pConnRS485II, m_pConnZBWX, m_pConnJLCY) < 0)
	{
		if (m_pConnZBWX != NULL)
			DnModuleReset(m_pConnZBWX);
		loget<<"现场总线初始化失败！！！"<<endl;
		return -1;
	}

	S_FBCONFIG FbConfig;
	S_SYSCFG *pSysCfg = STATUS.GetSystemConfig();

	FbConfig.m_DrRouteMode = pSysCfg->m_DrRouteMode;
	m_DnModule.SetModuleConfigure(FbConfig);
	m_MeterCom.SetFillChar((char)0xFF);
	return 0;
}

int C_DbcjServer::SyncGatherStatus(void)
{
	if (m_pConnZBWX == NULL)
		return 0;
	
	string strRtua, strFactory;
	S_SYSCFG *pSysCfg = STATUS.GetSystemConfig();
	C_RTUA rtua(pSysCfg->m_RTUA);
	rtua.PrintString(strRtua);
	string strBcdAddress = C_BusiAlgo::ChangeDword2Bcd(pSysCfg->m_RTUA.m_Address);
	string strAddress(strRtua.begin(), strRtua.begin()+2);
	if (strBcdAddress.size() < 4)
		strBcdAddress.append((size_t)(4-strBcdAddress.size()), (char)0);
	strAddress.append(strBcdAddress);

	if (m_DnModule.SyncRouterStatus(strAddress, strFactory) < 0)
		return -1;
	RefreshRouterInfo(strAddress, strFactory);

	S_DbcjStatus *pDbcjStatus = CONTEXT.GetDbcjStatus(m_DbcjSN);
	pDbcjStatus->m_timeManualRoute = C_TIME::TimeNow(); // 终端一启动就进行路由,防止换模块
	pDbcjStatus->m_LcdBottomInfoIndicator = 0x00; // 初始化采集系统

	return 0;
}

int C_DbcjServer::CreateAllTask(void)
{
	S_DbcjStatus *pDbcjStatus = CONTEXT.GetDbcjStatus(m_DbcjSN);
	pDbcjStatus->m_LcdBottomInfoIndicator = 0x00; // 初始化采集系统

	S_TaskStatus *pTaskStatus = STATUS.GetTaskStatus(m_DbcjSN);
	S_ALLMP *pALLMP = STATUS.GetALLMP();
	pTaskStatus->BeforeFind(&pALLMP->m_MTBS);

	m_TaskSchedule.Init();
	CountMyMeter();
	CreateBroadcastTimeTask();
	CreateSeekMeterTask();
	if (m_pConnZBWX != NULL)
		CreateRouteTask();
	if (m_DbcjSN == MP_DBCJ_1)
		CreateTermTask();
	if (m_DbcjSN == MP_DBCJ_1)
		CreateGroupSumTask();
	CreatePollingTask();
	CreateMeterTask();
	CreateFixMeterTask();
	CreateAlarmTask();

	pTaskStatus->AfterFind();
	return 0;
}

int C_DbcjServer::TaskSchedule(void)
{
	S_DbcjStatus *pDbcjStatus = CONTEXT.GetDbcjStatus(m_DbcjSN);
///	pDbcjStatus->m_LcdBottomInfoIndicator = (S_BYTE)m_TaskSchedule.GetCurrentTaskTypeID() + 1; // 参见LcdBottomInfoDbcj[]

	m_TaskSchedule.Schedule();
	if (CheckRealtimeTask() == false)
		return CheckRunningStatus();
	return DBCJ_STATUS_RUNNING;
}

int C_DbcjServer::RefreshRouterInfo(const string &strAddress, const string &strInfo)
{
	strings strRouteInfo;
	S_SYSRUN *pSysRun = STATUS.GetSysRunInfo();
	strRouteInfo.split(strInfo, ' ');
	if (strRouteInfo.size() != 4)
		return -1;

	strncpy(pSysRun->m_RouterName, m_DnModule.GetModuleName(), sizeof(pSysRun->m_RouterName));
	if (strRouteInfo[0].compare(pSysRun->m_RouterType) != 0 || strRouteInfo[1].compare(pSysRun->m_ChipCode) != 0
		|| strRouteInfo[2].compare(pSysRun->m_ChipVerDate) != 0 || strRouteInfo[3].compare(pSysRun->m_ChipVersion) != 0)
	{
		strncpy(pSysRun->m_RouterType, strRouteInfo[0].c_str(), sizeof(pSysRun->m_RouterType));
		strncpy(pSysRun->m_ChipCode, strRouteInfo[1].c_str(), sizeof(pSysRun->m_ChipCode));
		strncpy(pSysRun->m_ChipVerDate, strRouteInfo[2].c_str(), sizeof(pSysRun->m_ChipVerDate));
		strncpy(pSysRun->m_ChipVersion, strRouteInfo[3].c_str(), sizeof(pSysRun->m_ChipVersion));
		STATUS.SaveSysRunInfo();
	}
	S_DbcjStatus *pDbcjStatus = CONTEXT.GetDbcjStatus(MP_DBCJ_1);
	copy(strAddress.begin(), strAddress.end(), pDbcjStatus->m_RouterAddr);
	strncpy(pDbcjStatus->m_RouterFactory, strRouteInfo[0].c_str(), sizeof(pDbcjStatus->m_RouterFactory));
	strncpy(pDbcjStatus->m_RouterChip, strRouteInfo[1].c_str(), sizeof(pDbcjStatus->m_RouterChip));
	int year = 0, mon = 0, day = 0;
	sscanf(strRouteInfo[2].c_str(), "%x-%x-%x", &year, &mon, &day);
	pDbcjStatus->m_RouterDate[0] = (char)day;
	pDbcjStatus->m_RouterDate[1] = (char)mon;
	pDbcjStatus->m_RouterDate[2] = (char)year;
	int ver = 0;
	sscanf(strRouteInfo[3].c_str(), "%x", &ver);
	pDbcjStatus->m_RouterVersion[0] = (char)(ver>>8);
	pDbcjStatus->m_RouterVersion[1] = (char)(ver>>0);
	return 0;
}


bool C_DbcjServer::CheckRealtimeTask(void)
{
//	S_BYTE buffer[] = {0x01,0x02,0x00,0x01,0x01,0x02,0x01,0x02,0x00,0x02,0x01,0x02,0x01,0x02,0x00,0x03,0x01,0x02};
//	string msg((char*)buffer, sizeof(buffer));

	msg.clear();
	int SerID = 0;
	//S_BYTE MsgT = MSAFN_READ_REALTIME;
	MsgT = MSAFN_READ_REALTIME;
	if (C_SysMsg::Query4Server(SerID, MsgT, msg) != 0)
	{
		return false;
	}


//MsgT = MSAFN_READ_REALTIME;

	S_TaskInfo TaskInfo;
	if (MsgT == MSAFN_READ_REALTIME)
	{
		TaskInfo.m_Enable = true;
		strcpy(TaskInfo.m_TypeName, TASK_NAME_REALTIME);
		TaskInfo.m_strPrivateData.append((char *)&SerID, sizeof(SerID));
		TaskInfo.m_strPrivateData.append((char *)&MsgT, sizeof(MsgT));
		TaskInfo.m_strPrivateData.append(msg);
		if (m_TaskSchedule.CreateTask(TaskInfo) == true)
		{
			logwt<<TASK_NAME_REALTIME<<"创建成功"<<endl;
			return true;
		}
		logwt<<TASK_NAME_REALTIME<<"创建失败"<<endl;
	}
	else if (MsgT == MSAFN_DATA_ROUTE || MsgT == MSAFN_WRITE_PARAM)
	{
		TaskInfo.m_Enable = true;
		strcpy(TaskInfo.m_TypeName, TASK_NAME_ONEOFF);
		TaskInfo.m_strPrivateData.append((char *)&SerID, sizeof(SerID));
		TaskInfo.m_strPrivateData.append((char *)&MsgT, sizeof(MsgT));
		TaskInfo.m_strPrivateData.append(msg);
		if (m_TaskSchedule.CreateTask(TaskInfo) == true)
		{
			logwt<<TASK_NAME_ONEOFF"["<<HEX<<MsgT<<"]创建成功"<<endl;
			return true;
		}
		logwt<<TASK_NAME_ONEOFF"["<<HEX<<MsgT<<"]创建失败"<<endl;
	}
	return false;
}

int C_DbcjServer::CheckRunningStatus(void)
{
	S_TIME timeCurrent = C_TIME::TimeNow();
	S_DbcjStatus *pDbcjStatus = CONTEXT.GetDbcjStatus(m_DbcjSN);
	if (pDbcjStatus->m_timeRouteError != 0x00)
	{
		logwt<<"路由出错，重新初始化..."<<endl;
		pDbcjStatus->m_timeRouteError = 0x00;
		return DBCJ_STATUS_INIT;
	}
	else if (pDbcjStatus->m_timeAddAlarm != 0x00)
	{
		logwt<<"收到告警参数命令..."<<endl;
		pDbcjStatus->m_timeAddAlarm = 0x00;
		FreshAlarmTaskEnable();
		return DBCJ_STATUS_RUNNING;
	}
	else if (pDbcjStatus->m_timeAddMeter != 0x00)
	{//diff
		if (pDbcjStatus->m_timeAddMeter > timeCurrent)
			pDbcjStatus->m_timeAddMeter = timeCurrent;
		if (timeCurrent - pDbcjStatus->m_timeAddMeter >= 60)
		{
			logwt<<"收到下表命令..."<<endl;
			pDbcjStatus->m_timeAddMeter = 0x00;
			pDbcjStatus->m_timeManualRoute = timeCurrent; //启动路由
			return DBCJ_STATUS_CREATE_TASK;
		}
	}
	else if (pDbcjStatus->m_timeAddTask != 0x00)
	{//diff
		if (pDbcjStatus->m_timeAddTask > timeCurrent)
			pDbcjStatus->m_timeAddTask = timeCurrent;
		if (timeCurrent - pDbcjStatus->m_timeAddTask >= 30)
		{
			logwt<<"收到任务命令..."<<endl;
			pDbcjStatus->m_timeAddTask = 0x00;
			return DBCJ_STATUS_CREATE_TASK;
		}
	}
	else if (pDbcjStatus->m_timeReportMeter != 0x00)
	{
		logwt<<"收到搜表命令..."<<endl;
		pDbcjStatus->m_timeReportMeter = 0x00;
		S_DbcjParam *pDbcjParam = STATUS.GetDbcjParam();
		if (pDbcjParam->m_MeterAutoUpdate == 0x00)
		{
			m_TaskSchedule.EnableTask(MSDI_TASK_SEEKMETER, false);
		}
		else
		{
			m_TaskSchedule.EnableTask(MSDI_TASK_SEEKMETER, true);
			m_TaskSchedule.TaskStartImmediately(MSDI_TASK_SEEKMETER, true);
		}
	}
	else if (pDbcjStatus->m_timeManualRoute != 0x00)
	{
		logwt<<"收到启动路由命令..."<<endl;
		pDbcjStatus->m_timeManualRoute = 0x00;
		m_TaskSchedule.TaskStartImmediately(MSDI_TASK_ROUTE, true);
		m_TaskSchedule.TaskStartImmediately(MSDI_TASK_ROUTE_EX, true);
	}
	else if (pDbcjStatus->m_ControlMeter485I != DBCJ_METER_CTRL_NOTHING)
	{
		logwt<<"收到485I端口控制命令..."<<endl;
		if (m_pConnRS485I != NULL)
			EnableGatherTask(!(pDbcjStatus->m_ControlMeter485I==DBCJ_METER_CTRL_STOP));
		pDbcjStatus->m_ControlMeter485I = DBCJ_METER_CTRL_NOTHING;
	}
	else if (pDbcjStatus->m_ControlMeter485II != DBCJ_METER_CTRL_NOTHING)
	{
		logwt<<"收到485II端口控制命令..."<<endl;
		if (m_pConnRS485II != NULL)
			EnableGatherTask(!(pDbcjStatus->m_ControlMeter485II==DBCJ_METER_CTRL_STOP));
		pDbcjStatus->m_ControlMeter485II = DBCJ_METER_CTRL_NOTHING;
	}
	else if (pDbcjStatus->m_ControlMeterZBWX != DBCJ_METER_CTRL_NOTHING)
	{
		logwt<<"收到ZBWX端口控制命令..."<<endl;
		if (m_pConnZBWX != NULL)
			EnableGatherTask(!(pDbcjStatus->m_ControlMeterZBWX==DBCJ_METER_CTRL_STOP));
		pDbcjStatus->m_ControlMeterZBWX = DBCJ_METER_CTRL_NOTHING;
	}
	//else if (SeekMeterNotify(pDbcjStatus) == true)
	//{// 搜表结束后变更表计
	//	S_WORD cntAddMeter = 0x00;
	//	if (pDbcjStatus->m_UpdateMeter485I == DBCJ_UPDATE_METER_NOTIFY)
	//		cntAddMeter += SeekMeterAdd(FB_IDX_485I);
	//	if (pDbcjStatus->m_UpdateMeter485II == DBCJ_UPDATE_METER_NOTIFY)
	//		cntAddMeter += SeekMeterAdd(FB_IDX_485II);
	//	if (pDbcjStatus->m_UpdateMeterZBWX == DBCJ_UPDATE_METER_NOTIFY)
	//		cntAddMeter += SeekMeterAdd(FB_IDX_ZBWX);
	//	if (cntAddMeter > 0)
	//		SeekMeterInfoUpdate(); // 产生告警和进行下表通知
	//	pDbcjStatus->m_UpdateMeter485I = DBCJ_UPDATE_METER_IDLE;
	//	pDbcjStatus->m_UpdateMeter485II = DBCJ_UPDATE_METER_IDLE;
	//	pDbcjStatus->m_UpdateMeterZBWX = DBCJ_UPDATE_METER_IDLE;
	//}
	return DBCJ_STATUS_RUNNING;
}

int C_DbcjServer::CreateRouteTask(void)
{
	S_ALLMP *pALLMP = STATUS.GetALLMP();
	S_TaskInfo TaskInfo = STATUS.FindTaskTemplateInfo(TASK_NAME_ROUTE);
	if (TaskInfo.m_Enable == true && pALLMP->m_MTBS.m_ZBMPBS.any() == true)
	{
		if (m_TaskSchedule.CreateTask(TaskInfo) == true)
		{
			logwt<<TASK_NAME_ROUTE<<"创建成功"<<endl;
			return CreateRouteExTask();
		}
	}
	return -1;
}

int C_DbcjServer::CreateRouteExTask(void)
{
	C_DnModule DnModule;
	if (string(DnModule.GetModuleName()) != string("DR")) // 东软模块3代模式需要先进行路由抄读
		return 0; // 这是一个特例

	S_SYSCFG *pSysCfg = STATUS.GetSystemConfig();
	if (pSysCfg->m_DrRouteMode == 4)
		return 0; // 3代才需要进行路由抄读
	
	S_TaskInfo TaskInfo = STATUS.FindTaskTemplateInfo(TASK_NAME_ROUTE_EX);
	if (TaskInfo.m_Enable == false)
	{
//		logwt<<pTaskName<<"的任务模板没有定义"<<endl;
		return 0;
	}

	S_ALLMP *pALLMP = STATUS.GetALLMP();
	TaskInfo.m_MSDI = MSDI_TASK_ROUTE_EX; //
	//置表计
	TaskInfo.m_MPBS = pALLMP->m_MTBS.m_ZBMPBS;
	TaskInfo.m_MPBS.Stat();
	if (TaskInfo.m_MPBS.begin() == TaskInfo.m_MPBS.end())
	{
		return 0;
	}

	TaskInfo.m_DisableRegs.reset();
	S_REGISTER CommReg = STATUS.FindCommonRegister(0x90100000);
	TaskInfo.m_CommRegs.push_back(CommReg);

	if (m_TaskSchedule.CreateTask(TaskInfo) == false)
		return -1;

	logwt<<TASK_NAME_ROUTE_EX<<"创建成功"<<endl;
 	return 0;
}

int C_DbcjServer::CreateBroadcastTimeTask(void)
{
	S_TaskInfo TaskInfo = STATUS.FindTaskTemplateInfo(TASK_NAME_BROADCAST);
	if (TaskInfo.m_Enable == true)
	{
		if (m_pConnRS485I != NULL && BroadcastTimeTaskEnable(FB_IDX_485I))
			TaskInfo.m_MPBS.set(FB_IDX_485I);
		if (m_pConnRS485II != NULL && BroadcastTimeTaskEnable(FB_IDX_485II))
			TaskInfo.m_MPBS.set(FB_IDX_485II);
		if (m_pConnZBWX != NULL && BroadcastTimeTaskEnable(FB_IDX_ZBWX))
			TaskInfo.m_MPBS.set(FB_IDX_ZBWX);
		TaskInfo.m_MPBS.Stat();
		if (m_TaskSchedule.CreateTask(TaskInfo) == true)
		{
			logwt<<TASK_NAME_BROADCAST<<"创建成功"<<endl;
			return 0;
		}
	}
	return -1;
}

int C_DbcjServer::CreateSeekMeterTask(void)
{
	S_DbcjParam *pDbcjParam = STATUS.GetDbcjParam();
	S_TaskInfo TaskInfo = STATUS.FindTaskTemplateInfo(TASK_NAME_SEEKMETER);
	if (TaskInfo.m_Enable == true)
	{
		if (m_pConnRS485I != NULL)
			TaskInfo.m_MPBS.set(FB_IDX_485I);
		if (m_pConnRS485II != NULL)
			TaskInfo.m_MPBS.set(FB_IDX_485II);
		if (m_pConnZBWX != NULL)
			TaskInfo.m_MPBS.set(FB_IDX_ZBWX);
		TaskInfo.m_MPBS.Stat();
		if (pDbcjParam->m_MeterAutoUpdate == 0)
			TaskInfo.m_Enable = false;
		if (m_TaskSchedule.CreateTask(TaskInfo) == true)
		{
			logwt<<TASK_NAME_SEEKMETER<<"创建成功"<<endl;
			return 0;
		}
	}
	return -1;
}

int C_DbcjServer::CreatePollingTask(void)
{
	S_DbcjStatus *pStatus = CONTEXT.GetDbcjStatus(m_DbcjSN);
	for (S_DWORD TID = 1; TID <= CONS_POLLING_TYPE_COUNT_MAX; TID++)
	{
		S_MSDI msdi = STATUS.FindMasterStationDataID(CLASSDATA_4, TID);
		if (msdi.m_Enable == false)
			continue;
		char *pTaskName = TASK_NAME_POLLING;
		S_TaskInfo TaskInfo = STATUS.FindTaskTemplateInfo(pTaskName);
		if (TaskInfo.m_Enable == false)
			continue;
		TaskInfo.m_MSDI = TID + TASK_SIGN_PREFIX_CLASS4; //+0x04000000 表示四类数据
		//采集间隔
		//置表计
		S_ALLMP *pALLMP = STATUS.GetALLMP();
		//考虑选抄
		TaskInfo.m_MPBS = pALLMP->m_MultiMPBS;
		TaskInfo.m_MPBS &= pStatus->m_mpbsMyMeter;
		TaskInfo.m_MPBS.Stat();
		if (TaskInfo.m_MPBS.begin() == TaskInfo.m_MPBS.end())
			continue;

		TaskInfo.m_DisableRegs = msdi.m_DisableCommReg;
		for (int i = 0; i < MSDI_COMM_REG_MAX; i++)
		{
			if (msdi.m_CommRegID[i] == 0)
				break;
			S_REGISTER CommReg = STATUS.FindCommonRegister(msdi.m_CommRegID[i]);
			if (CommReg.m_ID == 0)
				logwt<<"通用寄存器["<<HEX<<msdi.m_CommRegID[i]<<"]没有配置"<<endl;
			else
				TaskInfo.m_CommRegs.push_back(CommReg);
		}

		if (m_TaskSchedule.CreateTask(TaskInfo) == true)
			logwt<<pTaskName<<"[Class=4 FN="<<(TaskInfo.m_MSDI&0xFFFF)<<" DI="<<HEX<<msdi.m_DI<<"]创建成功"<<endl;
	}
	return 0;
}

int C_DbcjServer::CreateMeterTask(void)
{
	S_FreezeTasks FreezeTasks;

	StatMeterTask(FreezeTasks);
	for (S_FreezeTasks::iterator iter = FreezeTasks.begin(); iter != FreezeTasks.end(); ++iter)
	{
		CreateMeterFreezeTask(*iter);
	}

	return 0;
}

int C_DbcjServer::CreateFixMeterTask(void)
{
	S_FreezeTasks FreezeTasks;

	StatMeterTask(FreezeTasks);
	for (S_FreezeTasks::iterator iter = FreezeTasks.begin(); iter != FreezeTasks.end(); ++iter)
	{
		CreateFixMeterFreezeTask(*iter, TASK_SIGN_PREFIX_CLASS5);
		CreateFixMeterFreezeTask(*iter, TASK_SIGN_PREFIX_CLASS6);
		CreateFixMeterFreezeTask(*iter, TASK_SIGN_PREFIX_CLASS7);
		CreateFixMeterFreezeTask(*iter, TASK_SIGN_PREFIX_CLASS8);
		CreateFixMeterFreezeTask(*iter, TASK_SIGN_PREFIX_CLASS9);
	}

	return 0;
}

int C_DbcjServer::CreateTermTask(void)
{
	for (S_WORD FN = 1; FN < MSDI_ITEM_MAX; FN++)
	{
		S_MSDI msdi = STATUS.FindMasterStationDataID(CLASSDATA_2, FN);
		if (msdi.m_Enable == false)
		{
//			logwt<<"主站数据项[Class=2 FN="<<(TaskInfo.m_MSDI&0xFFFF)<<"]没有配置"<<endl;
			continue;
		}
		//(冻结)日通信流量; (冻结)月通信流量 ; (冻结)月终端复位次数
		if (!(msdi.m_DI == 0xE1800012 || msdi.m_DI == 0xE1800014 || msdi.m_DI == 0xE1800017))
			continue;

		char *pTaskName = NULL;
		if (msdi.m_DI == 0xE1800012) //(冻结)日通信流量; 
			pTaskName = TASK_NAME_TERM_DAY;
		else if (msdi.m_DI == 0xE1800014 || msdi.m_DI == 0xE1800017)// (冻结)月通信流量 ; (冻结)月终端复位次数
			pTaskName = TASK_NAME_TERM_MON;
		if (pTaskName == NULL)
		{
	//		logwt<<"Class2 FN["<<MeterDataID.m_UserData<<"]指向的任务没有定义"<<endl;
			continue;
		}
		
		S_TaskInfo TaskInfo = STATUS.FindTaskTemplateInfo(pTaskName);
		if (TaskInfo.m_Enable == false)
		{
	//		logwt<<pTaskName<<"的任务模板没有定义"<<endl;
			continue;
		}

		TaskInfo.m_MSDI = FN + TASK_SIGN_PREFIX_CLASS2; //+0x02000000 表示二类数据
		TaskInfo.m_MPBS.set(0);
		TaskInfo.m_MPBS.Stat();

		TaskInfo.m_DisableRegs = msdi.m_DisableCommReg;
		for (int i = 0; i < MSDI_COMM_REG_MAX; i++)
		{
			if (msdi.m_CommRegID[i] == 0)
				break;
			S_REGISTER CommReg = STATUS.FindCommonRegister(msdi.m_CommRegID[i]);
			if (CommReg.m_ID == 0)
				logwt<<"通用寄存器["<<HEX<<msdi.m_CommRegID[i]<<"]没有配置"<<endl;
			else
				TaskInfo.m_CommRegs.push_back(CommReg);
		}

		if (m_TaskSchedule.CreateTask(TaskInfo) == false)
			continue;

		logwt<<pTaskName<<"[Class2 FN="<<(TaskInfo.m_MSDI&0xFFFF)<<" DI="<<HEX<<msdi.m_DI<<"]创建成功"<<endl;
	}
	return 0;
}

int C_DbcjServer::CreateAlarmTask(void)
{
	S_DbcjInfo *pInfo = STATUS.GetDbcjInfo(m_DbcjSN);
	S_DbcjStatus *pStatus = CONTEXT.GetDbcjStatus(m_DbcjSN);
	for (S_DWORD ERC = 1; ERC <= CONS_ALARM_TYPE_COUNT_MAX; ERC++)
	{
		S_MSDI msdi = STATUS.FindMasterStationDataID(CLASSDATA_3, ERC);
		if (msdi.m_Enable == false)
			continue;
		char *pTaskName = TASK_NAME_POLL_ALARM;
		//if (pAlarmEvent->SupportDetectMethod(ALM_DETECT_METHOD_REALTIME) == true)
		//	pTaskName = TASK_NAME_POLL_ALARM;
		//else if (pAlarmEvent->SupportDetectMethod(ALM_DETECT_METHOD_DAY) == true)
		//	pTaskName = TASK_NAME_DAY_ALARM;
		if (pTaskName == NULL)
		{
	//		logwt<<"Class2 FN["<<MeterDataID.m_UserData<<"]指向的任务没有定义"<<endl;
			continue;
		}

		S_TaskInfo TaskInfo = STATUS.FindTaskTemplateInfo(pTaskName);
		if (TaskInfo.m_Enable == false)
			continue;
		if (pInfo->m_AlmPollCycleM > 0)
			TaskInfo.m_TimeUnit = TIME_UNIT_MINUTE, TaskInfo.m_PeriodVal = pInfo->m_AlmPollCycleM;

		TaskInfo.m_MSDI = ERC + TASK_SIGN_PREFIX_CLASS3; //+0x03000000 表示三类数据
		//采集间隔
		//置表计
		S_ALLMP *pALLMP = STATUS.GetALLMP();
		TaskInfo.m_MPBS = pALLMP->m_MTBS.m_AllMPBS;
		TaskInfo.m_MPBS &= pStatus->m_mpbsMyMeter;
		TaskInfo.m_MPBS.Stat();
		if (TaskInfo.m_MPBS.begin() == TaskInfo.m_MPBS.end())
			continue;

//		S_AlarmInfo *pAlarmInfo = STATUS.GetAlarmInfo();
//		if (pAlarmInfo->NeedRecord(msdi.m_DI) == false)
//			TaskInfo.m_Enable = false;// 任务暂不运行

		TaskInfo.m_DisableRegs = msdi.m_DisableCommReg;
		for (int i = 0; i < MSDI_COMM_REG_MAX; i++)
		{
			if (msdi.m_CommRegID[i] == 0)
				break;
			S_REGISTER CommReg = STATUS.FindCommonRegister(msdi.m_CommRegID[i]);
			if (CommReg.m_ID == 0)
				logwt<<"通用寄存器["<<HEX<<msdi.m_CommRegID[i]<<"]没有配置"<<endl;
			else
				TaskInfo.m_CommRegs.push_back(CommReg);
		}

		if (m_TaskSchedule.CreateTask(TaskInfo) == false)
			continue;

		m_TaskSchedule.TaskStartImmediately(TaskInfo.m_MSDI, false); // 第一次创建任务就启动
		logwt<<pTaskName<<"[Class=3 FN="<<(TaskInfo.m_MSDI&0xFFFF)<<" DI="<<HEX<<msdi.m_DI<<"]创建成功"<<endl;
	}
	return 0;
}

int C_DbcjServer::CreateGroupSumTask(void)
{
	for (S_WORD FN = 1; FN < MSDI_ITEM_MAX; FN++)
	{
		char *pTaskName = NULL;
		//int MeanFN = C_FnInfo::GetType4Class2FN(FN);
		//int MeanPN = C_FnInfo::GetPnMean4Class2Fn(FN);
		//if (MeanPN == GW_CLASS2PN_GROUP)
		//{
		//	if (MeanFN == GW_CLASS2_VIP)
		//		pTaskName = TASK_NAME_TERM_VIP;
		//	else if (MeanFN == GW_CLASS2_DAY)
		//		pTaskName = TASK_NAME_TERM_DAY;
		//	else if (MeanFN == GW_CLASS2_MON)
		//		pTaskName = TASK_NAME_TERM_MON;
		//}
		if (pTaskName == NULL)
		{
	//		logwt<<"Class2 FN["<<MeterDataID.m_UserData<<"]指向的任务没有定义"<<endl;
			continue;
		}
		
		S_TaskInfo TaskInfo = STATUS.FindTaskTemplateInfo(pTaskName);
		if (TaskInfo.m_Enable == false)
		{
	//		logwt<<pTaskName<<"的任务模板没有定义"<<endl;
			continue;
		}

		TaskInfo.m_MSDI = FN + TASK_SIGN_PREFIX_CLASS2; //+0x02000000 表示二类数据

		//S_GroupSums *pGroupSums = STATUS.GetGroupSums();
		//for (S_WORD GroupNO = 0; GroupNO < GW_GROUP_SUM_MAX; GroupNO++)
		//{
		//	if (pGroupSums->m_Item[GroupNO].m_Enable == true)
		//		TaskInfo.m_MPBS.set(GroupNO+1);
		//}
		TaskInfo.m_MPBS.Stat();
		if (TaskInfo.m_MPBS.begin() == TaskInfo.m_MPBS.end()) //没有总加组
			continue;

		S_MSDI msdi = STATUS.FindMasterStationDataID(CLASSDATA_2, TaskInfo.m_MSDI);
		if (msdi.m_Enable == false)
		{
//			logwt<<"主站数据项[Class=2 FN="<<(TaskInfo.m_MSDI&0xFFFF)<<"]没有配置"<<endl;
			continue;
		}
		TaskInfo.m_DisableRegs = msdi.m_DisableCommReg;
		for (int i = 0; i < MSDI_COMM_REG_MAX; i++)
		{
			if (msdi.m_CommRegID[i] == 0)
				break;
			S_REGISTER CommReg = STATUS.FindCommonRegister(msdi.m_CommRegID[i]);
			if (CommReg.m_ID == 0)
				logwt<<"通用寄存器["<<HEX<<msdi.m_CommRegID[i]<<"]没有配置"<<endl;
			else
				TaskInfo.m_CommRegs.push_back(CommReg);
		}

		if (m_TaskSchedule.CreateTask(TaskInfo) == true)
			logwt<<pTaskName<<"[Class2 FN="<<(TaskInfo.m_MSDI&0xFFFF)<<"]创建成功"<<endl;
	}

	return 0;
}

int C_DbcjServer::CreateMeterFreezeTask(const S_FreezeTask &FreezeTask)
{
	char *pTaskName = NULL;
	S_MSDI msdi = STATUS.FindMasterStationDataID(CLASSDATA_2, FreezeTask.m_DT, 0x00);
	if (msdi.m_Enable == false)
	{
//		logwt<<"主站数据项[Class=2 FN="<<(TaskInfo.m_MSDI&0xFFFF)<<"]没有配置"<<endl;
		return -1;
	}
	if (msdi.m_Freeze == MSDI_FREEZE_HOUR)
		pTaskName = TASK_NAME_VIP;
	else if (msdi.m_Freeze == MSDI_FREEZE_DAY)
		pTaskName = TASK_NAME_DAY;
	else if (msdi.m_Freeze == MSDI_FREEZE_MONTH)
		pTaskName = TASK_NAME_MON;

	if (pTaskName == NULL)
	{
//		logwt<<"Class2 FN["<<MeterDataID.m_UserData<<"]指向的任务没有定义"<<endl;
		return -1;
	}
	
	S_TaskInfo TaskInfo = STATUS.FindTaskTemplateInfo(pTaskName);
	if (TaskInfo.m_Enable == false)
	{
//		logwt<<pTaskName<<"的任务模板没有定义"<<endl;
		return -1;
	}

	TaskInfo.m_MSDI = msdi.m_MSDI + TASK_SIGN_PREFIX_CLASS2; //+0x02000000 表示二类数据
	//置表计
	TaskInfo.m_MPBS = FreezeTask.m_mpbsMeter;
	TaskInfo.m_MPBS.Stat();
	if (TaskInfo.m_MPBS.begin() == TaskInfo.m_MPBS.end())
	{
		return -1;
	}

	TaskInfo.m_DisableRegs = msdi.m_DisableCommReg;
	for (int i = 0; i < MSDI_COMM_REG_MAX; i++)
	{
		if (msdi.m_CommRegID[i] == 0)
			break;
		S_REGISTER CommReg = STATUS.FindCommonRegister(msdi.m_CommRegID[i]);
		if (CommReg.m_ID == 0)
			logwt<<"通用寄存器["<<HEX<<msdi.m_CommRegID[i]<<"]没有配置"<<endl;
		else
			TaskInfo.m_CommRegs.push_back(CommReg);
	}

	if (m_TaskSchedule.CreateTask(TaskInfo) == false)
		return -1;

	m_TaskSchedule.TaskStartImmediately(TaskInfo.m_MSDI, false); // 第一次创建任务就启动
	logwt<<pTaskName<<"[Class2 FN="<<(TaskInfo.m_MSDI&0xFFFF)<<" DI="<<HEX<<msdi.m_DI<<"]创建成功"<<endl;
	return 0;
}

int C_DbcjServer::CreateFixMeterFreezeTask(const S_FreezeTask &FreezeTask, S_DWORD ClassDataPrefix)
{
	char *pTaskName = NULL;
	S_MSDI msdi = STATUS.FindMasterStationDataID((S_BYTE)(ClassDataPrefix>>24), FreezeTask.m_DT, 0x00);
	if (msdi.m_Enable == false)
	{
//		logwt<<"主站数据项[Class=2 FN="<<(TaskInfo.m_MSDI&0xFFFF)<<"]没有配置"<<endl;
		return -1;
	}
	if (msdi.m_Freeze == MSDI_FREEZE_DAY)
		pTaskName = TASK_NAME_FIX_DAY;
//	else if (msdi.m_Freeze == MSDI_FREEZE_HOUR)
//		pTaskName = TASK_NAME_VIP;
//	else if (msdi.m_Freeze == MSDI_FREEZE_MONTH)
//		pTaskName = TASK_NAME_MON;

	if (pTaskName == NULL)
	{
//		logwt<<"Class2 FN["<<MeterDataID.m_UserData<<"]指向的任务没有定义"<<endl;
		return -1;
	}
	
	S_TaskInfo TaskInfo = STATUS.FindTaskTemplateInfo(pTaskName);
	if (TaskInfo.m_Enable == false)
	{
//		logwt<<pTaskName<<"的任务模板没有定义"<<endl;
		return -1;
	}

	TaskInfo.m_MSDI = msdi.m_MSDI + ClassDataPrefix; //+ 表示数据类
	//置表计
	TaskInfo.m_MPBS = FreezeTask.m_mpbsMeter;
	TaskInfo.m_MPBS.Stat();
	if (TaskInfo.m_MPBS.begin() == TaskInfo.m_MPBS.end())
	{
		return -1;
	}

	TaskInfo.m_DisableRegs = msdi.m_DisableCommReg;
	for (int i = 0; i < MSDI_COMM_REG_MAX; i++)
	{
		if (msdi.m_CommRegID[i] == 0)
			break;
		S_REGISTER CommReg = STATUS.FindCommonRegister(msdi.m_CommRegID[i]);
		if (CommReg.m_ID == 0)
			logwt<<"通用寄存器["<<HEX<<msdi.m_CommRegID[i]<<"]没有配置"<<endl;
		else
			TaskInfo.m_CommRegs.push_back(CommReg);
	}

	if (m_TaskSchedule.CreateTask(TaskInfo) == false)
		return -1;

	logwt<<pTaskName<<"[Class"<<(S_BYTE)(ClassDataPrefix>>24)<<" FN="<<(TaskInfo.m_MSDI&0xFFFF)<<" DI="<<HEX<<msdi.m_DI<<"]创建成功"<<endl;
	return 0;
}

int C_DbcjServer::FreshAlarmTaskEnable(void)
{
	//S_AlarmInfo *pAlmInfo = STATUS.GetAlarmInfo();
	//for (int ERC = 1; ERC <= CONS_ALARM_TYPE_COUNT_MAX; ERC++)
	//{
	//	S_MSDI msdi = STATUS.FindMasterStationDataID(CLASSDATA_3, ERC);
	//	if (msdi.m_Enable == false)
	//		continue;
	//	if (pAlmInfo->NeedRecord(msdi.m_DI) == true)
	//		m_TaskSchedule.EnableTask(TASK_SIGN_PREFIX_CLASS3+ERC, true);
	//	else
	//		m_TaskSchedule.EnableTask(TASK_SIGN_PREFIX_CLASS3+ERC, false);
	//}
	return 0;
}

int C_DbcjServer::EnableGatherTask(bool fEnable)
{
	m_TaskSchedule.EnableTask(TASK_NAME_ROUTE, fEnable);
	m_TaskSchedule.EnableTask(TASK_NAME_ROUTE_EX, fEnable);
	m_TaskSchedule.EnableTask(TASK_NAME_BROADCAST, fEnable);
	m_TaskSchedule.EnableTask(TASK_NAME_SEEKMETER, fEnable);
	m_TaskSchedule.EnableTask(TASK_NAME_VIP, fEnable);
	m_TaskSchedule.EnableTask(TASK_NAME_DAY, fEnable);
	m_TaskSchedule.EnableTask(TASK_NAME_MON, fEnable);
	m_TaskSchedule.EnableTask(TASK_NAME_FIX_DAY, fEnable);
	m_TaskSchedule.EnableTask(TASK_NAME_POLLING, fEnable);
	m_TaskSchedule.EnableTask(TASK_NAME_STAT_DAY, fEnable);
	m_TaskSchedule.EnableTask(TASK_NAME_STAT_MON, fEnable);
	m_TaskSchedule.EnableTask(TASK_NAME_POLL_ALARM, fEnable);
	m_TaskSchedule.EnableTask(TASK_NAME_DAY_ALARM, fEnable);
	return 0;
}

int C_DbcjServer::StatMeterTask(S_FreezeTasks &FreezeTasks)
{
	S_DbcjStatus *pStatus = CONTEXT.GetDbcjStatus(m_DbcjSN);
	for (S_BYTE i = 0; i < CONS_TASK_COUNT_MAX; i++)
	{
		S_GatherTaskInfo *pGatherTaskInfo = STATUS.GetGatherTaskInfo(i);
		if (pGatherTaskInfo->m_Enable == false)
			continue;

		S_FreezeTask FreezeTask;
		S_ALLMP *pALLMP = STATUS.GetALLMP();
		FreezeTask.m_RecordMax = pGatherTaskInfo->m_RecordMax;

		if (pGatherTaskInfo->m_MeterType == METER_FUN_MULTI_FUN)
			FreezeTask.m_mpbsMeter = pALLMP->m_MultiMPBS;
		else // 单相表要采的项，多功能表也采
			FreezeTask.m_mpbsMeter = pALLMP->m_MTBS.m_AllMPBS;//, FreezeTask.m_mpbsMeter -= pALLMP->m_MultiMPBS;

		/** 普通用户要采的，重点用户也采*/
		if (pGatherTaskInfo->m_VipFlag == 1)
			FreezeTask.m_mpbsMeter &= pALLMP->m_VipMPBS;
//		else // 
//			FreezeTask.m_mpbsMeter -= pALLMP->m_VipMPBS;

		FreezeTask.m_mpbsMeter &= pStatus->m_mpbsMyMeter;
		FreezeTask.m_mpbsMeter.Stat();
		if (FreezeTask.m_mpbsMeter.begin() == FreezeTask.m_mpbsMeter.end())
			continue;

		for (S_WORD j = 0; j < pGatherTaskInfo->m_DtCnt; j++)
		{
			FreezeTask.m_DT = pGatherTaskInfo->m_DT[j];
			FreezeTasks.AddTask(FreezeTask);
		}
	}
	return 0;
}

int C_DbcjServer::CheckJlcyMeter(S_MPBS &MPBS, S_ALLMP *pALLMP)
{
	S_MpInfos *pMpInfos = STATUS.GetMpInfos();
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	for (size_t MP = pALLMP->m_MTBS.m_485MPBS.begin(); MP < pALLMP->m_MTBS.m_485MPBS.end(); MP++)
	{
		if (pALLMP->m_MTBS.m_485MPBS.test(MP) == false)
			continue;
		if (pMpInfos->m_Item[MP].m_Type != MP_TYPE_METER)
			continue;
		if (pMeterInfos->m_Item[MP].m_Enable == false)
			continue;
		C_MRULE *pMRule = C_MRuleFact::GetMeterRule(pMeterInfos->m_Item[MP].m_RuleID);
		if (pMRule == NULL)
			continue;
		if (strcasecmp(pMRule->GetName(), MRULE_NAME_JLCY) == 0)
			MPBS.set(MP);
	}
	return 0;
}

int C_DbcjServer::CountMyMeter(void)
{
	S_DbcjStatus *pStatus = CONTEXT.GetDbcjStatus(m_DbcjSN);
	pStatus->m_mpbsMyMeter.reset();

	S_ALLMP *pALLMP = STATUS.GetALLMP();
	logwt<<"分析表档案开始..."<<endl;
	logwt<<"有效表共["<<(S_WORD)pALLMP->m_MTBS.m_AllMPBS.count()<<"]块"<<endl;
	logwt<<"多功能表共["<<(S_WORD)pALLMP->m_MultiMPBS.count()<<"]块"<<endl;
	logwt<<"VIP表共["<<(S_WORD)pALLMP->m_VipMPBS.count()<<"]块"<<endl;
	logwt<<"485表共["<<(S_WORD)pALLMP->m_MTBS.m_485MPBS.count()<<"]块"<<endl;
	logwt<<"载波表共["<<(S_WORD)pALLMP->m_MTBS.m_ZBMPBS.count()<<"]块"<<endl;
	logwt<<"载波VIP表共["<<(S_WORD)pALLMP->m_MTBS.m_VipZBMPBS.count()<<"]块"<<endl;
	logwt<<"分析表档案结束"<<endl;

	S_MpInfos *pMpInfos = STATUS.GetMpInfos();
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	for (size_t MP = pALLMP->m_MTBS.m_AllMPBS.begin(); MP < pALLMP->m_MTBS.m_AllMPBS.end(); MP++)
	{
		if (pALLMP->m_MTBS.m_AllMPBS.test(MP) == false)
			continue;
		if (pMpInfos->m_Item[MP].m_Type != MP_TYPE_METER)
			continue;
		S_MeterInfo &Meter = pMeterInfos->m_Item[MP];
		if (Meter.m_Enable == false)
			continue;
		if (pStatus->m_bsMyVCom.test(Meter.m_PortSet.m_PortNO) == true)
			pStatus->m_mpbsMyMeter.set(MP);
	}
	pStatus->m_mpbsMyMeter.Stat();
	int Cnt = (int)pStatus->m_mpbsMyMeter.count();
	logwt<<"统计本进程的表计共["<<Cnt<<"]块"<<endl;

	return Cnt;
}

bool C_DbcjServer::BroadcastTimeTaskEnable(int channel)
{
//	S_CHANNEL *pChannel = STATUS.GetDbcjChannel(channel);
//	if (pChannel == NULL) // default true
//		return true;
	//if (pChannel->m_PortSet.m_ComSet.m_PortNO < 1 || pChannel->m_PortSet.m_ComSet.m_PortNO > CONS_GATHER_PARAMS_CNT_MAX)
	//	return true;
	//S_GatherParams *pGatherParams = STATUS.GetGatherParams();
	//S_GatherParam &GatherParam = pGatherParams->m_Item[pChannel->m_PortSet.m_ComSet.m_PortNO - 1];
	//if (GatherParam.m_Enable == false)
	//	return true;
	//return GatherParam.BroadcastTimeEnable();
	return true;
}

bool C_DbcjServer::SeekMeterNotify(S_DbcjStatus *pDbcjStatus)
{
//	if (m_DbcjSN != MP_DBCJ_1) // dbcj1 负责更新表计到档案
//		return false;
	if (pDbcjStatus->m_UpdateMeter485I != DBCJ_UPDATE_METER_NOTIFY && pDbcjStatus->m_UpdateMeter485II != DBCJ_UPDATE_METER_NOTIFY
		&& pDbcjStatus->m_UpdateMeterZBWX != DBCJ_UPDATE_METER_NOTIFY)// 没有一个搜表任务结束
		return false;
	if (pDbcjStatus->m_UpdateMeter485I != DBCJ_UPDATE_METER_STANDBY && pDbcjStatus->m_UpdateMeter485II != DBCJ_UPDATE_METER_STANDBY
		&& pDbcjStatus->m_UpdateMeterZBWX != DBCJ_UPDATE_METER_STANDBY)// 没有一个搜表任务在启动中
		return true;
	return false;
}

S_WORD C_DbcjServer::SeekMeterAdd(int idxFB)
{
	S_WORD cntUpdateMeter = 0x00;
	//S_SeekMeterInfos *pSeekMeterInfos = STATUS.GetSeekMeterInfos();
	//S_SeekMeterInfo *pSeekMeterInfo = pSeekMeterInfos->m_Item485I;
	//if (idxFB == FB_IDX_485II)
	//{
	//	logwt<<"总线[485II]搜表完成..."<<endl;
	//	pSeekMeterInfo = pSeekMeterInfos->m_Item485II;
	//}
	//else if (idxFB == FB_IDX_ZBWX)
	//{
	//	logwt<<"总线[ZBWX]搜表完成..."<<endl;
	//	pSeekMeterInfo = pSeekMeterInfos->m_ItemZBWX;
	//}
	//else
	//{
	//	logwt<<"总线[485I]搜表完成..."<<endl;
	//}

	///** @brief 增加表计 */
	//S_MeterInfo Meter;
	//Meter.m_Enable = false;
	//S_WORD BaseMP = 1, BaseSN = 1;
	//for (S_SeekMeterInfo *SeekMeterInfoEnd = pSeekMeterInfo+CONS_SEEK_METER_MAX; pSeekMeterInfo < SeekMeterInfoEnd; pSeekMeterInfo += 1)
	//{
	//	if (pSeekMeterInfo->m_Enable == false)
	//		break;
	//	if (Meter.m_Enable == false)
	//		Meter = DefaultUpdateMeterInfo(pSeekMeterInfo->m_PortSet.m_PortNO);
	//	Meter.m_RuleID = pSeekMeterInfo->m_RuleID;
	//	Meter.m_PortSet = pSeekMeterInfo->m_PortSet;
	//	memcpy(Meter.m_Address, pSeekMeterInfo->m_Address, METER_ADDRESS_LEN_MAX);
	//	memcpy(Meter.m_RelayAddr, pSeekMeterInfo->m_RelayAddr, RELAY_ADDRESS_LEN_MAX);
	//	UpdateMeterInfo(Meter, BaseMP, BaseSN);

	//	cntUpdateMeter += 1;
	//}
	///** @brief 清除缓冲 */
	//pSeekMeterInfos->Init(idxFB);
	return cntUpdateMeter;
}

int C_DbcjServer::SeekMeterInfoUpdate(void)
{
	/** @brief 刷新表计*/
//	STATUS.SaveDbcjInfo();
//	STATUS.RefreshMeterInfo();
	/** @brief 产生参数变更告警 */
	//C_AlarmEvent *pAlarmEvent = C_AlarmFactory::GetAlarmEventObj(3);
	//if (pAlarmEvent != NULL && pAlarmEvent->NeedRecord() == true)
	//{
	//	string strPnfn;
	//	S_WORD DA = C_DAPN::PN2DA(0x00);
	//	S_WORD DT = C_DTFN::FN2DT(10);
	//	strPnfn.append((char*)&DA, 2).append((char*)&DT, 2);

	//	S_DataERC003 DataErc003;
	//	DataErc003.m_MSA = 0x00;
	//	DataErc003.m_strPnfns.push_back(strPnfn);
	//	pAlarmEvent->Action((void *)&DataErc003, 0);
	//}
	/** @brief 通知下表 */
//	S_DbcjStatus *pDbcj1Status = CONTEXT.GetDbcjStatus(MP_DBCJ_1);
//	S_DbcjStatus *pDbcj2Status = CONTEXT.GetDbcjStatus(MP_DBCJ_2);
//	pDbcj1Status->m_timeAddMeter = C_TIME::TimeNow();
//	pDbcj2Status->m_timeAddMeter = C_TIME::TimeNow();
	return 0;
}

S_MeterInfo C_DbcjServer::DefaultUpdateMeterInfo(S_BYTE PortNO)
{
	S_MeterInfo Meter;
	Meter.ClearMeterInfo();
	Meter.m_Enable = true;
	Meter.m_TariffCnt = 1;

	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	for (S_WORD sn = 1; sn < CONS_METER_COUNT_MAX; sn++)
	{
		if (pMeterInfos->m_Item[sn].m_Enable == false)
			continue;
		if (pMeterInfos->m_Item[sn].m_PortSet.m_PortNO == PortNO)
		{
			Meter = pMeterInfos->m_Item[sn];
			break;
		}
	}

	Meter.m_VipFlag = false; // clear vip flag
	return Meter;
}

int C_DbcjServer::UpdateMeterInfo(const S_MeterInfo &Meter, S_WORD &BaseMP, S_WORD &BaseSN)
{
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	for (; BaseSN < CONS_METER_COUNT_MAX; BaseSN++)
	{
		if (pMeterInfos->m_Item[BaseSN].m_Enable == false)
			break;
	}
	if (BaseSN >= CONS_METER_COUNT_MAX)
		return -1;

	S_MpInfos *pMpInfos = STATUS.GetMpInfos();
	for (; BaseMP < CONS_MP_COUNT_MAX; BaseMP++)
	{
		if (pMpInfos->Test(BaseMP, MP_TYPE_METER, BaseSN) == true)
			break;
	}
	if (BaseMP >= CONS_METER_COUNT_MAX)
		return -1;

	pMeterInfos->m_Item[BaseSN] = Meter;
	pMeterInfos->m_Item[BaseSN].m_MeasureNo = BaseMP;
	if (pMpInfos->Set(BaseMP, MP_TYPE_METER, BaseSN) == false)
		return -1;
	//S_DbcjInfo *pDbcjInfo = STATUS.GetDbcjInfo();
//	pDbcjInfo->m_mpbsAlteration.set(BaseMP);
	return 0;
}

int C_DbcjServer::DnModuleReset(C_CONNECT *pConnZBWX)
{
#define DBCJ_DNMODULE_RESET_MAX		10
	if (++m_DnModuleResetCnt > DBCJ_DNMODULE_RESET_MAX)
	{
		m_DnModuleResetCnt = 0x00;
		logwt<<"检测载波模块["<<DBCJ_DNMODULE_RESET_MAX<<"次]失败,进程退出!!!"<<endl;
		exit(0);
	}
	m_DnModule.Reset(pConnZBWX);
	return (int)m_DnModuleResetCnt;
}

//
//表计能量采集服务
//
int C_Dbcj1Server::BeforeService(void)
{
	m_DbcjSN = MP_DBCJ_1;
	return C_DbcjServer::BeforeService();
}

int C_Dbcj2Server::BeforeService(void)
{
	m_DbcjSN = MP_DBCJ_2;
	return C_DbcjServer::BeforeService();
}

int C_Dbcj3Server::BeforeService(void)
{
	m_DbcjSN = MP_DBCJ_3;
	return C_DbcjServer::BeforeService();
}

int C_Dbcj4Server::BeforeService(void)
{
	m_DbcjSN = MP_DBCJ_4;
	return C_DbcjServer::BeforeService();
}






