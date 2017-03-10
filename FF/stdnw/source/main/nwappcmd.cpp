/** @file
 *  @brief 南网命令定义
 *  @author 
 *  @date 2011/11/30
 *  @version 0.1
 *  @see appcmd.h
 *  @note
 *  1、南网命令继承自多进程框架
 *  2、本文件定义南网特色命令及服务功能
 */
#include "nwappcmd.h"
#include "cfg_proc.h"
#include "htrace.h"
#include "basebver.h"
#include "nwbver.h"
#include "nwdaemon.h"
#include "httx.h"
#include "dbcj.h"
#include "ppp.h"
#include "lcd.h"
#include "jlcy.h"
#include "fhkz.h"
#include "ctcy.h"
#include "context.h"
#include "status.h"
#include "zonebase.h"
#include "basefn.h"
#include "cfg_mp.h"
#include "cfg_path.h"

#ifndef WIN32
#include "rftx.h" //wjp 20160729
#endif
//
// 服务对象
//
static C_SERVER *pDaemonServ = NULL;
static C_SERVER *phttx1Serv = NULL;
static C_SERVER *phttx2Serv = NULL;
static C_SERVER *phttx3Serv = NULL;
static C_SERVER *phttx4Serv = NULL;
static C_SERVER *pdbcj1Serv  = NULL;
static C_SERVER *pdbcj2Serv  = NULL;
static C_SERVER *pdbcj3Serv  = NULL;
static C_SERVER *pdbcj4Serv  = NULL;
static C_SERVER *pPppServ = NULL;
static C_SERVER *pLcdServ = NULL;
static C_SERVER *pJlcyServ = NULL;
static C_SERVER *pFhkzServ = NULL;
static C_SERVER *pCtcyServ = NULL;
#ifndef WIN32
static C_SERVER *prftxServ = NULL; //wjp 20160729
#endif
C_NwAppCmd::C_NwAppCmd(int argc, char *argv[]):C_AppCmd(argc, argv)
{
}

C_NwAppCmd::~C_NwAppCmd()
{
}

bool C_NwAppCmd::SetServer(const char *pServerName, C_SERVER *pServer)
{
	if (strcasecmp(MP_PROCESS_HTTX1, pServerName) == 0)
		phttx1Serv = pServer;
	else if (strcasecmp(MP_PROCESS_HTTX2, pServerName) == 0)
		phttx2Serv = pServer;
	else if (strcasecmp(MP_PROCESS_HTTX3, pServerName) == 0)
		phttx3Serv = pServer;
	else if (strcasecmp(MP_PROCESS_HTTX4, pServerName) == 0)
		phttx4Serv = pServer;
	else if (strcasecmp(MP_PROCESS_DBCJ1, pServerName) == 0)
		pdbcj1Serv = pServer;
	else if (strcasecmp(MP_PROCESS_DBCJ2, pServerName) == 0)
		pdbcj2Serv = pServer;
	else if (strcasecmp(MP_PROCESS_DBCJ3, pServerName) == 0)
		pdbcj3Serv = pServer;
	else if (strcasecmp(MP_PROCESS_DBCJ4, pServerName) == 0)
		pdbcj4Serv = pServer;
	else if (strcasecmp(MP_PROCESS_PPP, pServerName) == 0)
		pPppServ = pServer;
	else if (strcasecmp(MP_PROCESS_LCD, pServerName) == 0)
		pLcdServ = pServer;
	else if (strcasecmp(MP_PROCESS_JLCY, pServerName) == 0)
		pJlcyServ = pServer;
	else if (strcasecmp(MP_PROCESS_FHKZ, pServerName) == 0)
		pFhkzServ = pServer;
	else if (strcasecmp(MP_PROCESS_CTCY, pServerName) == 0)
		pCtcyServ = pServer;
	else if (strcasecmp(MP_PROCESS_DAEMON, pServerName) == 0)
		pDaemonServ = pServer;
#ifndef WIN32
	else if (strcasecmp(MP_PROCESS_RFTX, pServerName) == 0)	//wjp 20160729
		prftxServ = pServer;
#endif
	return true;
}

C_SERVER *C_NwAppCmd::GetServer(const char *pServerName)
{
	if (strcasecmp(MP_PROCESS_HTTX1, pServerName) == 0)
	{
		if (phttx1Serv == NULL)
			phttx1Serv = new C_Httx1Server;
		return phttx1Serv;
	}
	if (strcasecmp(MP_PROCESS_HTTX2, pServerName) == 0)
	{
		if (phttx2Serv == NULL)
			phttx2Serv = new C_Httx2Server;
		return phttx2Serv;
	}
	if (strcasecmp(MP_PROCESS_HTTX3, pServerName) == 0)
	{
		if (phttx3Serv == NULL)
			phttx3Serv = new C_Httx3Server;
		return phttx3Serv;
	}
	if (strcasecmp(MP_PROCESS_HTTX4, pServerName) == 0)
	{
		if (phttx4Serv == NULL)
			phttx4Serv = new C_Httx4Server;
		return phttx4Serv;
	}
	if (strcasecmp(MP_PROCESS_DBCJ1, pServerName) == 0)
	{
		if (pdbcj1Serv == NULL)
			pdbcj1Serv = new C_Dbcj1Server;
		return pdbcj1Serv;
	}
	if (strcasecmp(MP_PROCESS_DBCJ2, pServerName) == 0)
	{
		if (pdbcj2Serv == NULL)
			pdbcj2Serv = new C_Dbcj2Server;
		return pdbcj2Serv;
	}
	if (strcasecmp(MP_PROCESS_DBCJ3, pServerName) == 0)
	{
		if (pdbcj3Serv == NULL)
			pdbcj3Serv = new C_Dbcj3Server;
		return pdbcj3Serv;
	}
	if (strcasecmp(MP_PROCESS_DBCJ4, pServerName) == 0)
	{
		if (pdbcj4Serv == NULL)
			pdbcj4Serv = new C_Dbcj4Server;
		return pdbcj4Serv;
	}
	if (strcasecmp(MP_PROCESS_PPP, pServerName) == 0)
	{
		if (pPppServ == NULL)
			pPppServ = new C_PppServer;
		return pPppServ;
	}
	if (strcasecmp(MP_PROCESS_LCD, pServerName) == 0)
	{
		if (pLcdServ == NULL)
			pLcdServ = new C_LcdServer;
		return pLcdServ;
	}
	if (strcasecmp(MP_PROCESS_JLCY, pServerName) == 0)
	{
		if (pJlcyServ == NULL)
			pJlcyServ = new C_JlcyServer;
		return pJlcyServ;
	}
	if (strcasecmp(MP_PROCESS_FHKZ, pServerName) == 0)
	{
		if (pFhkzServ == NULL)
			pFhkzServ = new C_FhkzServer;
		return pFhkzServ;
	}
	if (strcasecmp(MP_PROCESS_CTCY, pServerName) == 0)
	{
		if (pCtcyServ == NULL)
			pCtcyServ = new C_CtcyServer;
		return pCtcyServ;
	}
	if (strcasecmp(MP_PROCESS_DAEMON, pServerName) == 0)
	{
		if (pDaemonServ == NULL)
			pDaemonServ = new C_NwDaemonServer;
		return pDaemonServ;
	}
#ifndef WIN32
	if (strcasecmp(MP_PROCESS_RFTX, pServerName) == 0)	// wjp 20160729
	{
		if (prftxServ == NULL)
			prftxServ = new C_RFtxServer;
		return prftxServ;
	}
#endif
	return C_AppCmd::GetServer(pServerName);
}

int C_NwAppCmd::ListVersion(void)
{
	C_AppCmd::ListVersion();
	logn<<"基础业务库版本:"<<BASEB_GetVersion()<<endl;
	logn<<"基础业务库发布日期:"<<BASEB_GetReleaseDate()<<endl;
	logn<<"南网业务库版本:"<<NWB_GetVersion()<<endl;
	logn<<"南网业务库发布日期:"<<NWB_GetReleaseDate()<<endl;
	logn<<"主程序版本:"<<MP_GetSoftwareVersion()<<endl;
	logn<<"主程序发布日期:"<<MP_GetSoftwareReleaseDate()<<endl;
	logn<<"硬件版本:"<<MP_GetHardwareVersion()<<endl;
	logn<<"硬件发布日期:"<<MP_GetHardwareReleaseDate()<<endl;

	S_SYSCFG *pSysCfg = STATUS.GetSystemConfig();
	if (pSysCfg != NULL)
	{
		logn<<"配置文件版本:"<<pSysCfg->m_CfgVer<<endl;
		logn<<"配置文件发布日期:"<<pSysCfg->m_CfgDate<<endl;
	}
	C_ZoneVersion *pZoneVersion = C_ZoneVersion::GetZoneVersionObject();
	if (pZoneVersion != NULL)
	{
		logn<<"区域插件名称:"<<pZoneVersion->GetZonePluginNameE()<<endl;
		logn<<"区域插件版本:"<<pZoneVersion->GetZonePluginVersion()<<endl;
		logn<<"区域插件发布日期:"<<pZoneVersion->GetZonePluginReleaseDate()<<endl;
	}

	return 0;
}

#define GW_SETTING_ZONE	"zone"

/** @brief 执行设置命令
 *  @param[in] pCmdInfo 命令参数信息
 *  @return 0 成功；-1 失败
 *  @remarks 应用程序可以扩展，注意要回调本函数
 */
int C_NwAppCmd::SetCmdExec(const char *pCmdInfo)
{
	strings strsCmd;
	strsCmd.split(pCmdInfo, '=');
	if (strcasecmp(strsCmd[0].c_str(), GW_SETTING_ZONE) == 0)
	{
		string strInfo;
		ZoneSwitch(pCmdInfo, strInfo);
		logn<<strInfo<<endl;
	}
	else
	{
		return C_AppCmd::SetCmdExec(pCmdInfo);
	}
	return 0;
}

/** @brief 显示设置命令帮助
 *  @param[in] InfoSN 帮助显示序号
 *  @return 0 成功；-1 失败
 *  @remarks 应用程序可以扩展，注意要回调本函数
 */
int C_NwAppCmd::ShowSetHelp(int InfoSN)
{
	InfoSN = C_AppCmd::ShowSetHelp(InfoSN);

	logn<<InfoSN+0<<". "GW_SETTING_ZONE"=区域包名称(.tgz)  设置切换到指定区域"<<endl;
	return InfoSN+1;
}


#define GW_ACTION_ROUTE	"ManualRoute"

/** @brief 执行动作命令
 *  @param[in] pCmdInfo 命令参数信息
 *  @return 0 成功；-1 失败
 *  @remarks 应用程序可以扩展，注意要回调本函数
 */
int C_NwAppCmd::ActionCmdExec(const char *pCmdInfo)
{
	if (strcasecmp(pCmdInfo, GW_ACTION_ROUTE) == 0)
	{
		logn<<"路由即将重启..."<<endl;
		S_DbcjStatus *pStatus = CONTEXT.GetDbcjStatus(MP_DBCJ_4);
		pStatus->m_timeManualRoute = C_TIME::TimeNow();
	}
	else
	{
		return C_AppCmd::ActionCmdExec(pCmdInfo);
	}
	return 0;
}

const S_HelpInfo GW_ActionPara[] =
{
	{GW_ACTION_ROUTE,     "手动启动路由"},
};

/** @brief 显示动作命令帮助
 *  @param[in] InfoSN 帮助显示序号
 *  @return 0 成功；-1 失败
 *  @remarks 应用程序可以扩展，注意要回调本函数
 */
int C_NwAppCmd::ShowActionHelp(int InfoSN)
{
	InfoSN = C_AppCmd::ShowActionHelp(InfoSN);
	for (size_t i = 0; i < sizeof(GW_ActionPara)/sizeof(GW_ActionPara[0]); i++)
	{
		logn<<(int)(InfoSN+i)<<". "<<GW_ActionPara[i].m_pItem<<"  "<<GW_ActionPara[i].m_pInfo<<endl;
	}
	return InfoSN + sizeof(GW_ActionPara)/sizeof(GW_ActionPara[0]);
}

/** @brief 显示采集任务信息
 *  @param[in] Process 采集进程序号[1-*]
 *  @param[out] strTaskInfo 任务信息
 *  @return 0 成功；-1 失败
 */
int C_NwAppCmd::ListDbcjTaskInfo(int Process, string &strTaskInfo)
{
	if (Process > 0) Process -= 1;
	if (Process < DBCJ_PROCESS_MAX)
	{
		S_TaskStatus *pTaskStatus = STATUS.GetTaskStatus((S_DbcjSN)Process);
		return pTaskStatus->PrintStatusInfo(strTaskInfo);
	}
	return -1;
}

/** @brief  采集表计总数信息
 *  @param[out] Cnt 表计总数
 *  @param[out] MinMP 最小测量点
 *  @param[out] MaxMP 最大测量点
 *  @return 0 成功；-1 失败
 */
int C_NwAppCmd::GetMeterCount(S_WORD &Cnt, S_WORD &MinMP, S_WORD &MaxMP)
{
	S_ALLMP *pAllMP = STATUS.GetALLMP();
	Cnt = (S_WORD)pAllMP->m_MTBS.m_AllMPBS.count();
	MinMP = (S_WORD)pAllMP->m_MTBS.m_AllMPBS.begin();
	MaxMP = (S_WORD)pAllMP->m_MTBS.m_AllMPBS.end();
	if (MaxMP > 0)
		MaxMP -= 1;
	return 0;
}

/** @brief 打印表计详细信息
 *  @param[in] MinMP 最小测量点
 *  @param[in] MaxMP 最大测量点
 *  @param[out] strInfo 信息
 *  @return 0 成功；-1 失败
 */
int C_NwAppCmd::PrintMeterInfo(S_WORD MinMP, S_WORD MaxMP, string &strInfo)
{
	S_ALLMP *pAllMP = STATUS.GetALLMP();
	S_MpInfos *pMpInfos = STATUS.GetMpInfos();
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();

	for (; MinMP <= MaxMP; MinMP++)
	{
		if (MinMP >= pAllMP->m_MTBS.m_AllMPBS.end())
			break;
		if (pAllMP->m_MTBS.m_AllMPBS.test(MinMP) == false)
			continue;

		S_WORD SN = pMpInfos->m_Item[MinMP].m_SN;
		pMeterInfos->m_Item[SN].PrintMeterInfo(strInfo);
	}

	return 0;
}

/** @brief 输出当前区域名称信息
 *  @param[out] strInfo 信息
 *  @return 0 成功；-1 失败
 */
int C_NwAppCmd::GetCurrentZone(string &strInfo)
{
	S_SYSCFG *pSysCfg = STATUS.GetSystemConfig();
	if (pSysCfg != NULL && pSysCfg->m_ZoneName[0] != '\0')
		strInfo.append(pSysCfg->m_ZoneName);
	else
		strInfo.append("未知地区");
	return 0;
}

int C_NwAppCmd::ZoneSwitch(const string &strCmdInfo, string &strInfo)
{
	strings strsCmd;
	strsCmd.split(strCmdInfo, '=');
	if (strsCmd.size() < 2)
	{
		strings strsTgzFile;
		if (C_FILE::FindFileWithSuffix(CFG_PATH_ZONE, ".tgz", strsTgzFile) == 0)
		{
			strInfo.append("没有发现区域安装包!!!");
		}
		else
		{
			strsTgzFile.AppendBack("    ");
			strInfo.append("请选择区域安装包:\n");
			strsTgzFile.Serialize(strInfo);
		}
		return -1;
	}

	S_ZoneCfgs ZoneList;
	string strCurrentZone;
	if (C_ZoneSwh::LoadZoneInfo(strCurrentZone, ZoneList) <= 0)
	{
		strInfo.append("加载区域信息失败!!!");
		return -1;
	}

	size_t idxZone = 0;
	for (; idxZone < ZoneList.size(); idxZone++)
	{
		if (strsCmd[1].compare(ZoneList[idxZone].m_strZonePackage) == 0)
			break;
	}
	if (idxZone >= ZoneList.size())
	{
		strInfo.append("未找到匹配的区域包!!!");
		return -1;
	}

	char buffer[CFG_PATH_FILE_MAX_LEN];
	bool CurrentModeType = (strCurrentZone == ZoneList[0].m_strZoneName); // 默认第0个为缺省模式
	bool ExpectedModeType = (ZoneList[idxZone].m_strZoneName == ZoneList[0].m_strZoneName); // 默认第0个为缺省模式
	C_PATH::GetZoneFile(ZoneList[idxZone].m_strZonePackage.c_str(), buffer);
	if (access(buffer, F_OK) != 0)// 检测升级文件
	{
		strInfo.append("区域安装包不存在!!!");
		return -1;
	}
	else if (CurrentModeType == false && ExpectedModeType == false) // 区域模式之间不能切换
	{
		strInfo.append("请先切换到缺省模式（测试模式）!!!");
		return -1;
	}
	else
	{
		C_ZoneSwh::SwitchZone(buffer);
	}

	strInfo.append("正在切换到指定区域,请稍候...");
	return 0;
}


