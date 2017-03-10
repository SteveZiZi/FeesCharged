/** @file
 *  @brief 用户层系统上下文配置类
 *  @author 
 *  @date 2011/12/06
 *  @version 0.1
 *  @see mprocess/sharemem.h mrpocess/sysipc.h
 *  注意需要继承C_ContextBase类，注意其中的抽象方法的重载。
 */
#include "context.h"
#include "ini_sys.h"
#include "ini_serv.h"
#include "zonebase.h"
#include "sysctrl.h"

C_CONTEXT &C_CONTEXT::Instance(void)
{
	static C_CONTEXT s_MyContext;
	return s_MyContext;
}

/** @brief 设置成员信息等
 *  @return true 成功；false 失败
 *  @remarks 子类重载时请先回调本函数，共享内存创建成功时调用本函数
 */
bool C_CONTEXT::SetInformation(void)
{
	if (C_ContextBase::SetInformation() == false)
		return false;
	m_pMyContext = (S_CONTEXT*)m_pContext;
	return true;
}

/** @brief 初始化系统上下文
 *  @return true 成功；false 失败
 *  @remarks 子类重载时请先回调本函数
 */
bool C_CONTEXT::InitContext(void)
{
	if (C_ContextBase::InitContext() != true)
		return false;

	m_pMyContext->m_FlowInfo.Init();
	m_pMyContext->m_ConnectStatus.Init();
	m_pMyContext->m_PowerStatus.Init();

	for (int i = 0; i < HTTX_PROCESS_MAX; i++)
		m_pMyContext->m_HttxStatus[i].Init();

	m_pMyContext->m_PppStatus.Init(); ///< PPP状态
	m_pMyContext->m_JlcyStatus.Init();
	m_pMyContext->m_FhkzStatus.Init();

//	m_pMyContext->m_ForwardTaskDatas.Init();

	C_ZoneShm *pZoneShm = C_ZoneShm::GetZoneSharedMemoryObject();
	if (pZoneShm != NULL)
		pZoneShm->InitExtendZone_Context();

	return true;
}

/** @brief 加载配置到系统上下文
 *  @return true 成功；false 失败
 *  @remarks 子类重载时请先回调本函数
 */
bool C_CONTEXT::LoadContext(void)
{
	if (C_ContextBase::LoadContext() != true)
		return false;

	C_IniSys::LoadFlowCtrlInfo(m_pMyContext);
	
	/* FN、GatherReg加载*/
//	CGatherFnRegCfg::LoadFnRegItems(m_pMyContext);

	C_ZoneShm *pZoneShm = C_ZoneShm::GetZoneSharedMemoryObject();
	if (pZoneShm != NULL)
		pZoneShm->LoadExtendZone_Context();

	return true;
}

/** @brief 参数分析
 *  @return true 成功；false 失败
 *  @remarks 子类重载时请先回调本函数
 */
bool C_CONTEXT::ParamParse(void)
{
	if (C_ContextBase::ParamParse() != true)
		return false;

	return true;
}

/** @brief 数据区复位时清除数据
 *  @return true 成功；false 失败
 *  @remarks 子类重载时请先回调本函数
 */
bool C_CONTEXT::ClearData(void)
{
	if (C_ContextBase::ClearData() != true)
		return false;

	return true;
}

bool C_CONTEXT::CheckProgramkeyState(bool fRefresh)
{
	S_LcdStatus &LcdStatus = m_pMyContext->m_LcdStatus;
	if (LcdStatus.m_ProgramKeyDelayS > 0 && fRefresh == true)
		LcdStatus.m_ProgramKeyDelayS = LCD_PROGRAM_KEY_DELAY_S;
	return LcdStatus.m_ProgramKeyDelayS > 0;
}

/** @brief 获取地区扩展信息存储区 */
void *C_CONTEXT::GetZoneExtendInfo(size_t size)
{
	if (size > sizeof(m_pMyContext->m_ZoneExtContext))
		return NULL;
	return (void *)m_pMyContext->m_ZoneExtContext;
}

/* return -1 表示查找失败, 否则返回序号*/
int C_CONTEXT::GetDbcjPrecess_MP(S_WORD MP)
{
	for (int i = 0; i < DBCJ_PROCESS_MAX; i++)
	{
		S_DbcjStatus &Status = m_pMyContext->m_DbcjStatus[i];
		if (Status.m_mpbsMyMeter.test(MP) == true)
			return i;
	}
	return -1;
}
int C_CONTEXT::GetDbcjPrecess_VCom(S_BYTE VCom)
{
	for (int i = 0; i < DBCJ_PROCESS_MAX; i++)
	{
		S_DbcjStatus &Status = m_pMyContext->m_DbcjStatus[i];
		if (Status.m_bsMyVCom.test(VCom) == true)
			return i;
	}
	return -1;
}

const char *C_CONTEXT::GetDbcjPrecessName_MP(S_WORD MP)
{
	return C_IniServer::GetDbcjProcessName(GetDbcjPrecess_MP(MP));
}

const char *C_CONTEXT::GetDbcjPrecessName_VCom(S_BYTE VCom)
{
	return C_IniServer::GetDbcjProcessName(GetDbcjPrecess_VCom(VCom));
}

bool C_CONTEXT::NotifyDbcj(int Msg)
{
	S_TIME timeCurrent = C_TIME::TimeNow();
	for (int DbcjSn = 0; DbcjSn < DBCJ_PROCESS_MAX; DbcjSn++)
	{
		S_DbcjStatus *pDbcjStatus = &m_pMyContext->m_DbcjStatus[DbcjSn];
		switch (Msg)
		{
		case NOTIFY_DBCJ_ADD_METER:pDbcjStatus->m_timeAddMeter = timeCurrent; break;
		case NOTIFY_DBCJ_ADD_TASK:pDbcjStatus->m_timeAddTask = timeCurrent; break;
		case NOTIFY_DBCJ_ADD_ALARM:pDbcjStatus->m_timeAddAlarm = timeCurrent; break;
		default:break;
		}
	}
	return true;
}

/** @brief 设置LCD指示器
 *  @param[in] pProcessName 进程名
 *  @param[in] Indicator 参见LCD_INDICATOR_
 */
bool C_CONTEXT::SetLcdBottomInfoIndicator(const char *pProcessName, S_BYTE Indicator)
{
	if (strcmp(pProcessName, MP_PROCESS_PPP) == 0)
	{
		if (Indicator >= LCD_INDICATOR_PPP_MAX)
			return false;
		m_pMyContext->m_PppStatus.m_LcdBottomInfoIndicator = Indicator;
	}
	else if (strcmp(pProcessName, MP_PROCESS_DBCJ1) == 0)
	{
		//if (Indicator >= LCD_INDICATOR_DBCJ_MAX)
		//	return false;
		//m_pMyContext-> TODO:
	}
	return true;
}

const char *C_CONTEXT::GetLcdBottomInfo(const char *pProcessName)
{
	static const char *LcdBottomInfoPpp[] = 
	{
		"MODEM正在上电...",
		"检查MODEM...    ",
		"检查无线信号... ",
		"PPP正在拨号...  ",
		"PPP连接成功...  ",
		"PPP连接失败     "
	};
	static const char *LcdBottomInfoDbcj[] = 
	{
		"初始化采集系统..",
		"抄表结束        ", //SYS_TASK_IDLE,
		"正在路由中...   ", //SYS_TASK_ROUTE,
		"响应实时命令... ", //SYS_TASK_REAL_TIME,
		"正在进行广播校时", //SYS_TASK_BROADCAST,
		"响应主站任务... ", //SYS_TASK_ONE_OFF,
		"正在表号上报中..", //SYS_TASK_SEEK_METER,
		"正在路由中...   ", //USER_TASK_ROUTE_EX = SYS_TASK_MAX,
		"采集曲线数据... ", //USER_TASK_VIP,
		"采集日冻结数据..", //USER_TASK_DAY,
		"采集月冻结数据..", //USER_TASK_MON,
		"补采日冻结数据..", //USER_TASK_FIX_DAY,
		"采集瞬时数据... ", //USER_TASK_POLLING,
		"冻结终端曲线数据", //USER_TASK_TERM_VIP,
		"冻结终端日数据..", //USER_TASK_TERM_DAY,
		"冻结终端月数据..", //USER_TASK_TERM_MON,
		"统计日数据...   ", //USER_TASK_STAT_DAY,
		"统计月数据...   ", //USER_TASK_STAT_MON,
		"采集告警数据... ", //USER_TASK_POLL_ALARM,
		"采集告警数据... ", //USER_TASK_DAY_ALARM,
		"正在进行其它任务", //USER_TASK_TYPE_ID_MAX,
	};
	if (strcmp(pProcessName, MP_PROCESS_PPP) == 0)
	{
		return LcdBottomInfoPpp[m_pMyContext->m_PppStatus.m_LcdBottomInfoIndicator];
	}
	else if (strcmp(pProcessName, MP_PROCESS_DBCJ1) == 0)
	{
		S_BYTE Indicator = m_pMyContext->m_DbcjStatus[MP_DBCJ_1].m_LcdBottomInfoIndicator;
		if (Indicator == 1 && C_SYSCTRL::GetProcessEnable(MP_PROCESS_DBCJ4) == true) // dbcj1空闲则取dbcj4的状态
			Indicator = m_pMyContext->m_DbcjStatus[MP_DBCJ_4].m_LcdBottomInfoIndicator;
		if (Indicator >= KGE_MEMBER_OF(LcdBottomInfoDbcj))
			Indicator = KGE_MEMBER_OF(LcdBottomInfoDbcj) - 1;
		return LcdBottomInfoDbcj[Indicator];
	}
	return NULL;
}



