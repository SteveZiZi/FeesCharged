/** @file
 *  @brief ���������
 *  @author 
 *  @date 2011/11/30
 *  @version 0.1
 *  @see appcmd.h
 *  @note
 *  1����������̳��Զ���̿��
 *  2�����ļ�����������ɫ���������
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
// �������
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
	logn<<"����ҵ���汾:"<<BASEB_GetVersion()<<endl;
	logn<<"����ҵ��ⷢ������:"<<BASEB_GetReleaseDate()<<endl;
	logn<<"����ҵ���汾:"<<NWB_GetVersion()<<endl;
	logn<<"����ҵ��ⷢ������:"<<NWB_GetReleaseDate()<<endl;
	logn<<"������汾:"<<MP_GetSoftwareVersion()<<endl;
	logn<<"�����򷢲�����:"<<MP_GetSoftwareReleaseDate()<<endl;
	logn<<"Ӳ���汾:"<<MP_GetHardwareVersion()<<endl;
	logn<<"Ӳ����������:"<<MP_GetHardwareReleaseDate()<<endl;

	S_SYSCFG *pSysCfg = STATUS.GetSystemConfig();
	if (pSysCfg != NULL)
	{
		logn<<"�����ļ��汾:"<<pSysCfg->m_CfgVer<<endl;
		logn<<"�����ļ���������:"<<pSysCfg->m_CfgDate<<endl;
	}
	C_ZoneVersion *pZoneVersion = C_ZoneVersion::GetZoneVersionObject();
	if (pZoneVersion != NULL)
	{
		logn<<"����������:"<<pZoneVersion->GetZonePluginNameE()<<endl;
		logn<<"�������汾:"<<pZoneVersion->GetZonePluginVersion()<<endl;
		logn<<"��������������:"<<pZoneVersion->GetZonePluginReleaseDate()<<endl;
	}

	return 0;
}

#define GW_SETTING_ZONE	"zone"

/** @brief ִ����������
 *  @param[in] pCmdInfo ���������Ϣ
 *  @return 0 �ɹ���-1 ʧ��
 *  @remarks Ӧ�ó��������չ��ע��Ҫ�ص�������
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

/** @brief ��ʾ�����������
 *  @param[in] InfoSN ������ʾ���
 *  @return 0 �ɹ���-1 ʧ��
 *  @remarks Ӧ�ó��������չ��ע��Ҫ�ص�������
 */
int C_NwAppCmd::ShowSetHelp(int InfoSN)
{
	InfoSN = C_AppCmd::ShowSetHelp(InfoSN);

	logn<<InfoSN+0<<". "GW_SETTING_ZONE"=���������(.tgz)  �����л���ָ������"<<endl;
	return InfoSN+1;
}


#define GW_ACTION_ROUTE	"ManualRoute"

/** @brief ִ�ж�������
 *  @param[in] pCmdInfo ���������Ϣ
 *  @return 0 �ɹ���-1 ʧ��
 *  @remarks Ӧ�ó��������չ��ע��Ҫ�ص�������
 */
int C_NwAppCmd::ActionCmdExec(const char *pCmdInfo)
{
	if (strcasecmp(pCmdInfo, GW_ACTION_ROUTE) == 0)
	{
		logn<<"·�ɼ�������..."<<endl;
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
	{GW_ACTION_ROUTE,     "�ֶ�����·��"},
};

/** @brief ��ʾ�����������
 *  @param[in] InfoSN ������ʾ���
 *  @return 0 �ɹ���-1 ʧ��
 *  @remarks Ӧ�ó��������չ��ע��Ҫ�ص�������
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

/** @brief ��ʾ�ɼ�������Ϣ
 *  @param[in] Process �ɼ��������[1-*]
 *  @param[out] strTaskInfo ������Ϣ
 *  @return 0 �ɹ���-1 ʧ��
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

/** @brief  �ɼ����������Ϣ
 *  @param[out] Cnt �������
 *  @param[out] MinMP ��С������
 *  @param[out] MaxMP ��������
 *  @return 0 �ɹ���-1 ʧ��
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

/** @brief ��ӡ�����ϸ��Ϣ
 *  @param[in] MinMP ��С������
 *  @param[in] MaxMP ��������
 *  @param[out] strInfo ��Ϣ
 *  @return 0 �ɹ���-1 ʧ��
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

/** @brief �����ǰ����������Ϣ
 *  @param[out] strInfo ��Ϣ
 *  @return 0 �ɹ���-1 ʧ��
 */
int C_NwAppCmd::GetCurrentZone(string &strInfo)
{
	S_SYSCFG *pSysCfg = STATUS.GetSystemConfig();
	if (pSysCfg != NULL && pSysCfg->m_ZoneName[0] != '\0')
		strInfo.append(pSysCfg->m_ZoneName);
	else
		strInfo.append("δ֪����");
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
			strInfo.append("û�з�������װ��!!!");
		}
		else
		{
			strsTgzFile.AppendBack("    ");
			strInfo.append("��ѡ������װ��:\n");
			strsTgzFile.Serialize(strInfo);
		}
		return -1;
	}

	S_ZoneCfgs ZoneList;
	string strCurrentZone;
	if (C_ZoneSwh::LoadZoneInfo(strCurrentZone, ZoneList) <= 0)
	{
		strInfo.append("����������Ϣʧ��!!!");
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
		strInfo.append("δ�ҵ�ƥ��������!!!");
		return -1;
	}

	char buffer[CFG_PATH_FILE_MAX_LEN];
	bool CurrentModeType = (strCurrentZone == ZoneList[0].m_strZoneName); // Ĭ�ϵ�0��Ϊȱʡģʽ
	bool ExpectedModeType = (ZoneList[idxZone].m_strZoneName == ZoneList[0].m_strZoneName); // Ĭ�ϵ�0��Ϊȱʡģʽ
	C_PATH::GetZoneFile(ZoneList[idxZone].m_strZonePackage.c_str(), buffer);
	if (access(buffer, F_OK) != 0)// ��������ļ�
	{
		strInfo.append("����װ��������!!!");
		return -1;
	}
	else if (CurrentModeType == false && ExpectedModeType == false) // ����ģʽ֮�䲻���л�
	{
		strInfo.append("�����л���ȱʡģʽ������ģʽ��!!!");
		return -1;
	}
	else
	{
		C_ZoneSwh::SwitchZone(buffer);
	}

	strInfo.append("�����л���ָ������,���Ժ�...");
	return 0;
}


