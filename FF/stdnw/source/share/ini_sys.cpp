/** @file
 *  @brief 系统配置加载和保存
 *  @author 
 *  @date 2011/12/06
 *  @version 0.1
 *  定义系统配置加载和保存函数
 */
#include "ini_sys.h"
#include "inifileb.h"
#include "timeop.h"
#include "version.h"
#include "flowctrl.h"
#include "status.h"
#include "mrfact.h"
#include "ipcSharemem.h"

/** @brief 系统配置版本结构体 */
cfg_opt_t OPT_CFGVER_SETTINGS[] =
{	//参数名 ,参数默认值  ,标志	configuration file is case insensitive
	CFG_STR("Version", "0.00", CFGF_NOCASE),
	CFG_STR("Date", "XXXX-XX-XX", CFGF_NOCASE),
	CFG_END()
};

/** @fn C_IniSys::LoadConfigVersion
 *  @brief 加载系统配置版本
 *  @param[out] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::LoadConfigVersion(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfg = cfg_init(OPT_CFGVER_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSystemFile(cfg, fname) != 0)
	{
		cfg_free(cfg);
		return -1;
	}
	S_SYSCFG &SysConfig = pStatus->m_SysConfig;

	strncpy(SysConfig.m_CfgVer, cfg_getstr(cfg, "Version"), sizeof(SysConfig.m_CfgVer)-2);
	strncpy(SysConfig.m_CfgDate, cfg_getstr(cfg, "Date"), sizeof(SysConfig.m_CfgDate)-2);

	cfg_free(cfg);
	return 0;
}

/** @fn C_IniSys::LoadSystemConfig
 *  @brief 加载系统配置参数配置
 *  @param[out] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::LoadSystemConfig(S_STATUS *pStatus, const char *fname)
{
	if (_LoadSystemConfig(pStatus) != 0)
		return -1;
	return LoadSystemAddress(pStatus);
}

/** @fn C_IniSys::SaveSystemConfig
 *  @brief 保存系统配置参数配置
 *  @param[in] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::SaveSystemConfig(S_STATUS *pStatus, const char *fname)
{
	if (_SaveSystemConfig(pStatus) != 0)
		return -1;
	return SaveSystemAddress(pStatus);
}

/** @brief 系统配置参数结构体 */
cfg_opt_t OPT_SYSCFG_SETTINGS[] =
{	//参数名 ,参数默认值  ,标志	configuration file is case insensitive
	CFG_STR("RTUA", "500100", CFGF_NOCASE),
	CFG_STR("SysUser", "root", CFGF_NOCASE),
	CFG_STR("SysPassword", "123456", CFGF_NOCASE),
	CFG_BOOL("InitIRDA", cfg_false, CFGF_NOCASE),
	CFG_STR("TermType", "jc", CFGF_NOCASE),
	CFG_STR("ZoneName", "", CFGF_NOCASE),
	CFG_INT("HeartBeatRetryTimes", 3, CFGF_NOCASE),
	CFG_INT("HeartBeatIdleTimeS", 60, CFGF_NOCASE),
	CFG_INT("ReportAgainIntervalM", 120, CFGF_NOCASE),
	CFG_INT("BatteryStandTimeM", 3, CFGF_NOCASE),
	CFG_INT("TermStandTimeS", 3, CFGF_NOCASE),
	CFG_INT("DrRouteMode", 3, CFGF_NOCASE),
	CFG_STR_LIST("ProgramKeyPort", "{20}", CFGF_NONE),
	CFG_END()
};

/** @fn C_IniSys::_LoadSystemConfig
 *  @brief 加载系统配置参数配置
 *  @param[out] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::_LoadSystemConfig(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfg = cfg_init(OPT_SYSCFG_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSystemFile(cfg, fname) != 0)
	{
		cfg_free(cfg);
		return -1;
	}
	S_SYSCFG &SysConfig = pStatus->m_SysConfig;
	/** 终端区县码*/
	string strArea(cfg_getstr(cfg, "RTUA"));
	if (strArea.size() < 12)
		strArea.append((size_t)12, '0'); // 不做精确处理，多一点也没有关系
	SysConfig.SetRTUA(strArea.c_str());
	/** 登陆信息*/
	strncpy(SysConfig.m_SysUser, cfg_getstr(cfg, "SysUser"), sizeof(SysConfig.m_SysUser)-4);
	strncpy(SysConfig.m_SysPassword, cfg_getstr(cfg, "SysPassword"), sizeof(SysConfig.m_SysPassword)-4);
	/** 配置信息*/
	SysConfig.m_InitIRDA = cfg_getbool(cfg, "InitIRDA") == cfg_true? true:false;
	SysConfig.m_TermType = GetTermType(cfg_getstr(cfg, "TermType"));
	SysConfig.m_HeartBeatRetryTimes = (INT16U)cfg_getint(cfg, "HeartBeatRetryTimes");
	SysConfig.m_HeartBeatIdleTimeS = (INT16U)cfg_getint(cfg, "HeartBeatIdleTimeS");
	SysConfig.m_ReportAgainIntervalM = (INT16U)cfg_getint(cfg, "ReportAgainIntervalM");
	SysConfig.m_BatteryStandTimeM = (INT16U)cfg_getint(cfg, "BatteryStandTimeM");
	SysConfig.m_TermStandTimeS = (INT16U)cfg_getint(cfg, "TermStandTimeS");
	SysConfig.m_DrRouteMode = cfg_getint(cfg, "DrRouteMode"); ///< 东软路由模式3=3代;4=4代

	for (unsigned int i = 0; i < cfg_size(cfg, "ProgramKeyPort"); i++)
	{
		SysConfig.m_ProgramKeyPort.SetBit(cfg_getnstr(cfg, "ProgramKeyPort", i), '-');
	}
	SysConfig.m_ProgramKeyPort.Stat();
	strncpy(SysConfig.m_ZoneName, cfg_getstr(cfg, "ZoneName"), sizeof(SysConfig.m_ZoneName)-4);

	if (SysConfig.m_HeartBeatRetryTimes == 0)//确认不等于0
		SysConfig.m_HeartBeatRetryTimes = 3;
	if (SysConfig.m_BatteryStandTimeM < 1 || SysConfig.m_BatteryStandTimeM > 10)
		SysConfig.m_BatteryStandTimeM = 3;

	cfg_free(cfg);
	return 0;
}

/** @fn C_IniSys::_SaveSystemConfig
 *  @brief 保存系统配置参数配置
 *  @param[in] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::_SaveSystemConfig(S_STATUS *pStatus, const char *fname)
{
	char Buffer[32];
	string strInfo, strPort;

	S_SYSCFG &SysConfig = pStatus->m_SysConfig;

	strInfo.append("#系统配置信息\r\n\r\n#终端区县码\r\n");
	memset(Buffer, 0x00, sizeof(Buffer));
	SysConfig.GetRTUA(Buffer);
	Buffer[6] = '\0'; // 截断地址
	strInfo.append("RTUA=").append(Buffer).append(" #格式 BBBBBB, B=BCD\r\n\r\n");
	strInfo.append("#登陆信息\r\n");
	strInfo.append("SysUser=\"").append(SysConfig.m_SysUser).append("\"\r\n");
	strInfo.append("SysPassword=\"").append(SysConfig.m_SysPassword).append("\"\r\n\r\n");
	
	strInfo.append("#配置信息\r\n");
	strInfo.append("InitIRDA=").append(C_IniBase::GetBoolKey(SysConfig.m_InitIRDA)).append(" #初始化红外\r\n");
	strInfo.append("TermType=").append(GetTermTypeName(SysConfig.m_TermType)).append(" #终端类型[jc|pb|fk|eac]\r\n");
	sprintf(Buffer, "%d", SysConfig.m_HeartBeatRetryTimes);
	strInfo.append("HeartBeatRetryTimes=").append(Buffer).append(" #心跳失败重试次数\r\n");
	sprintf(Buffer, "%d", SysConfig.m_HeartBeatIdleTimeS);
	strInfo.append("HeartBeatIdleTimeS=").append(Buffer).append(" #空闲心跳时间\r\n");
	sprintf(Buffer, "%d", SysConfig.m_ReportAgainIntervalM);
	strInfo.append("ReportAgainIntervalM=").append(Buffer).append(" #当前上报任务补报间隔\r\n");
	sprintf(Buffer, "%d", SysConfig.m_DrRouteMode);
	strInfo.append("DrRouteMode=").append(Buffer).append(" #东软路由模式3=3代;4=4代\r\n");;

	sprintf(Buffer, "%d", SysConfig.m_BatteryStandTimeM);
	strInfo.append("BatteryStandTimeM=").append(Buffer).append(" #电池供电时间长度，用于切电。默认为3分钟\r\n");
	sprintf(Buffer, "%d", SysConfig.m_TermStandTimeS);
	strInfo.append("TermStandTimeS=").append(Buffer).append(" #掉电无电池情况下，终端切电时间\r\n");

	strInfo.append("ProgramKeyPort={");
	SysConfig.m_ProgramKeyPort.PrintParameter(strPort);
	strInfo.append(strPort).append("} #编程键控制的端口\r\n");
	strInfo.append("ZoneName=\"").append(SysConfig.m_ZoneName).append("\"\r\n");

	return C_IniBase::SaveSystemFile(strInfo, fname);
}

/** @brief 系统配置参数结构体 */
cfg_opt_t OPT_SYSCFG_ADDRESS[] =
{	//参数名 ,参数默认值  ,标志	configuration file is case insensitive
	CFG_STR("Address", "000000", CFGF_NOCASE),

	CFG_END()
};

/** @fn C_IniSys::LoadSystemAddress
 *  @brief 加载系统配置参数配置
 *  @param[out] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::LoadSystemAddress(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfg = cfg_init(OPT_SYSCFG_ADDRESS, CFGF_NONE);
	if (C_IniBase::ParseSystemFile(cfg, fname) != 0)
	{
		cfg_free(cfg);
		return -1;
	}

	char Buffer[32];
	S_SYSCFG &SysConfig = pStatus->m_SysConfig;

	memset(Buffer, 0x00, sizeof(Buffer));
	SysConfig.GetRTUA(Buffer);

	strncpy(Buffer+6, cfg_getstr(cfg, "Address"), 6); // 合并地址到后6字节
	SysConfig.SetRTUA(Buffer);

	cfg_free(cfg);
	return 0;
}

/** @fn C_IniSys::SaveSystemConfig
 *  @brief 保存系统配置参数配置
 *  @param[in] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::SaveSystemAddress(S_STATUS *pStatus, const char *fname)
{
	char Buffer[32];
	string strInfo, strPort;

	S_SYSCFG &SysConfig = pStatus->m_SysConfig;

	strInfo.append("#终端地址\r\n");
	memset(Buffer, 0x00, sizeof(Buffer));
	strInfo.append("Address=").append(SysConfig.GetRTUA(Buffer)+6).append(" #格式 HHHHHH, H=HEX\r\n\r\n");

	return C_IniBase::SaveSystemFile(strInfo, fname);
}

/** @brief 系统运行信息参数结构体 */
cfg_opt_t OPT_RUNNING_SETTINGS[] =
{	//参数名 ,参数默认值  ,标志	configuration file is case insensitive
	CFG_STR("ReleaseDate", "2009-09-09", CFGF_NOCASE),
	CFG_STR("HWVersion", "GZNF-CQ01-DJGZ33-6100", CFGF_NOCASE),
	CFG_STR("SWVersion", "GZNF-CQ01-DJGZ33-6100", CFGF_NOCASE),
	CFG_STR("MPVersion", "0.00", CFGF_NOCASE),
	CFG_STR("PowerCutDate", "2010-6-6", CFGF_NOCASE),
	CFG_STR("PowerCutTime", "10:30:30", CFGF_NOCASE),
	CFG_STR("RouterType", "XC", CFGF_NOCASE),
	CFG_STR("ChipCode", "01", CFGF_NOCASE),
	CFG_STR("ChipVerDate", "700101", CFGF_NOCASE),
	CFG_STR("ChipVersion", "V1",CFGF_NOCASE),
	CFG_STR("ModemFactory", "unknow",CFGF_NOCASE),
	CFG_STR("ModemModel", "unknow",CFGF_NOCASE),
	CFG_STR("SimIP", "0.0.0.0", CFGF_NOCASE),
	CFG_END()
};

/** @fn C_IniSys::LoadRunInformation
 *  @brief 加载系统版本备份
 *  @param[out] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::LoadRunInformation(S_STATUS *pStatus, const char *fname)
{
	S_SYSRUN &SysRun = pStatus->m_SysRun;
	cfg_t* cfg = cfg_init(OPT_RUNNING_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSystemFile(cfg, fname) != 0)
	{
		SysRun.Init();
		cfg_free(cfg);
		return -1;
	}

	strncpy(SysRun.m_ReleaseDate, cfg_getstr(cfg, "ReleaseDate"), sizeof(SysRun.m_ReleaseDate)-1);
	strncpy(SysRun.m_HWVersion, cfg_getstr(cfg, "HWVersion"), sizeof(SysRun.m_HWVersion)-1);
	strncpy(SysRun.m_SWVersion, cfg_getstr(cfg, "SWVersion"), sizeof(SysRun.m_SWVersion)-1);
	strncpy(SysRun.m_MPVersion, cfg_getstr(cfg, "MPVersion"), sizeof(SysRun.m_MPVersion)-1);

	/* qt  共享内存通讯，版本信息*/
	struct SIpcVersion version;
	memcpy(version.m_HWVersion,SysRun.m_HWVersion,sizeof(SysRun.m_HWVersion));
	memcpy(version.m_SWVersion,SysRun.m_SWVersion,sizeof(SysRun.m_SWVersion));
	IPCSHAREMEM.m_IpcTerminalInfo.SetVersion(version);
	IPCSHAREMEM.Update();

	int year=1900,month=1,day=1,hour=0,minute=0,second=0;
	sscanf(cfg_getstr(cfg,"PowerCutDate"), "%d-%d-%d", &year, &month, &day);
	sscanf(cfg_getstr(cfg,"PowerCutTime"), "%d:%d:%d", &hour, &minute, &second);
	S_DATE date;
	date.m_Year = year;
	date.m_Mon  = month;
	date.m_Day = day;
	date.m_Hour = hour;
	date.m_Min  = minute;
	date.m_Sec  = second;
	SysRun.m_timePowerCut = C_TIME::MakeTime(date);

	strncpy(SysRun.m_RouterType, cfg_getstr(cfg, "RouterType"), sizeof(SysRun.m_RouterType)-1);
	strncpy(SysRun.m_ChipCode, cfg_getstr(cfg, "ChipCode"), sizeof(SysRun.m_ChipCode)-1);
	strncpy(SysRun.m_ChipVerDate, cfg_getstr(cfg, "ChipVerDate"), sizeof(SysRun.m_ChipVerDate)-1);
	strncpy(SysRun.m_ChipVersion, cfg_getstr(cfg, "ChipVersion"), sizeof(SysRun.m_ChipVersion)-1);
	strncpy(SysRun.m_ModemFactory, cfg_getstr(cfg, "ModemFactory"), sizeof(SysRun.m_ModemFactory)-1);
	strncpy(SysRun.m_ModemModel, cfg_getstr(cfg, "ModemModel"), sizeof(SysRun.m_ModemModel)-1);
	
	int ip0 = 0, ip1 = 0, ip2 = 0, ip3 = 0;
	sscanf(cfg_getstr(cfg,"SimIP"), "%d.%d.%d.%d", &ip0, &ip1, &ip2, &ip3);
	INT8U *IP = (INT8U*)&SysRun.m_SimIP;
	IP[0] = (INT8U)ip0, IP[1] = (INT8U)ip1, IP[2] = (INT8U)ip2, IP[3] = (INT8U)ip3;

	cfg_free(cfg);
	return 0;
}

/** @fn C_IniSys::SaveRunInformation
 *  @brief 保存系统版本备份
 *  @param[in] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::SaveRunInformation(S_STATUS *pStatus, const char *fname)
{
	string strInfo;
	char Buffer[32];

	S_SYSRUN &SysRun = pStatus->m_SysRun;

	strInfo.append("#注意：本文件由系统自动维护，不得人为更改！！！\r\n\r\n");
	strInfo.append("#版本信息\r\n");
	strInfo.append("ReleaseDate=").append(CONS_SW_REAL_DATE).append(" #版本发布日期\r\n");
	strInfo.append("HWVersion=").append(CONS_HARD_VERSION).append(" #硬件系统版本号\r\n");
	memset(Buffer, 0x00, sizeof(Buffer));
	strInfo.append("SWVersion=").append(CONS_SOFT_VERSION).append(" #软件系统版本号\r\n");
	strInfo.append("MPVersion=").append(CONS_MAIN_VERSION).append(" #主程序版本号\r\n\r\n");

	strInfo.append("#系统前一次掉电时间\r\n");
	S_DATE date;
	C_TIME::LocalTime(SysRun.m_timePowerCut, date);
	sprintf(Buffer, "%d-%d-%d", date.m_Year, date.m_Mon, date.m_Day);
	strInfo.append("PowerCutDate=").append(Buffer).append(" #年-月-日\r\n");
	sprintf(Buffer, "%d:%d:%d", date.m_Hour, date.m_Min, date.m_Sec);
	strInfo.append("PowerCutTime=").append(Buffer).append(" #时:分:秒\r\n\r\n");

	strInfo.append("#下行模块信息\r\n");
	strInfo.append("RouterType=\"").append(SysRun.m_RouterType).append("\" #XC,DR,YXD,RB,SR,LME,DX\r\n");
	strInfo.append("ChipCode=\"").append(SysRun.m_ChipCode).append("\" #芯片代码\r\n");
	strInfo.append("ChipVerDate=\"").append(SysRun.m_ChipVerDate).append("\" #芯片版本日期\r\n");
	strInfo.append("ChipVersion=\"").append(SysRun.m_ChipVersion).append("\" #模块版本号,00为无效\r\n\r\n");

	strInfo.append("#MODEM信息\r\n");
	strInfo.append("ModemFactory=\"").append(SysRun.m_ModemFactory).append("\" #MODEM生产厂家\r\n");
	strInfo.append("ModemModel=\"").append(SysRun.m_ModemModel).append("\" #MODEM型号\r\n");
	INT8U *IP = (INT8U*)&SysRun.m_SimIP;
	sprintf(Buffer, "%d.%d.%d.%d", IP[0], IP[1], IP[2], IP[3]);
	strInfo.append("SimIP=\"").append(Buffer).append("\" #Sim卡IP\r\n\r\n");
	
	return C_IniBase::SaveSystemFile(strInfo, fname);
}

/** @brief 系统配置参数结构体 */
cfg_opt_t OPT_PORTMAP_SETTINGS[] =
{	//参数名 ,参数默认值  ,标志	configuration file is case insensitive
	CFG_INT("VCom0",  0, CFGF_NOCASE),
	CFG_INT("VCom1",  0, CFGF_NOCASE),
	CFG_INT("VCom2",  0, CFGF_NOCASE),
	CFG_INT("VCom3",  0, CFGF_NOCASE),
	CFG_INT("VCom4",  0, CFGF_NOCASE),
	CFG_INT("VCom5",  0, CFGF_NOCASE),
	CFG_INT("VCom6",  0, CFGF_NOCASE),
	CFG_INT("VCom7",  0, CFGF_NOCASE),
	CFG_INT("VCom8",  0, CFGF_NOCASE),
	CFG_INT("VCom9",  0, CFGF_NOCASE),
	CFG_INT("VCom10", 0, CFGF_NOCASE),
	CFG_INT("VCom11", 0, CFGF_NOCASE),
	CFG_INT("VCom12", 0, CFGF_NOCASE),
	CFG_INT("VCom13", 0, CFGF_NOCASE),
	CFG_INT("VCom14", 0, CFGF_NOCASE),
	CFG_INT("VCom15", 0, CFGF_NOCASE),
	CFG_INT("VCom16", 0, CFGF_NOCASE),
	CFG_INT("VCom17", 0, CFGF_NOCASE),
	CFG_INT("VCom18", 0, CFGF_NOCASE),
	CFG_INT("VCom19", 0, CFGF_NOCASE),
	CFG_INT("VCom20", 0, CFGF_NOCASE),
	CFG_INT("VCom21", 0, CFGF_NOCASE),
	CFG_INT("VCom22", 0, CFGF_NOCASE),
	CFG_INT("VCom23", 0, CFGF_NOCASE),
	CFG_INT("VCom24", 0, CFGF_NOCASE),
	CFG_INT("VCom25", 0, CFGF_NOCASE),
	CFG_INT("VCom26", 0, CFGF_NOCASE),
	CFG_INT("VCom27", 0, CFGF_NOCASE),
	CFG_INT("VCom28", 0, CFGF_NOCASE),
	CFG_INT("VCom29", 0, CFGF_NOCASE),
	CFG_INT("VCom30", 0, CFGF_NOCASE),
	CFG_INT("VCom31", 0, CFGF_NOCASE),
	CFG_INT("VCom32",  0, CFGF_NOCASE),
	CFG_INT("VCom33",  0, CFGF_NOCASE),
	CFG_INT("VCom34",  0, CFGF_NOCASE),
	CFG_INT("VCom35",  0, CFGF_NOCASE),
	CFG_INT("VCom36",  0, CFGF_NOCASE),
	CFG_INT("VCom37",  0, CFGF_NOCASE),
	CFG_INT("VCom38",  0, CFGF_NOCASE),
	CFG_INT("VCom39",  0, CFGF_NOCASE),
	CFG_INT("VCom40", 0, CFGF_NOCASE),
	CFG_INT("VCom41", 0, CFGF_NOCASE),
	CFG_INT("VCom42", 0, CFGF_NOCASE),
	CFG_INT("VCom43", 0, CFGF_NOCASE),
	CFG_INT("VCom44", 0, CFGF_NOCASE),
	CFG_INT("VCom45", 0, CFGF_NOCASE),
	CFG_INT("VCom46", 0, CFGF_NOCASE),
	CFG_INT("VCom47", 0, CFGF_NOCASE),
	CFG_INT("VCom48", 0, CFGF_NOCASE),
	CFG_INT("VCom49", 0, CFGF_NOCASE),
	CFG_INT("VCom50", 0, CFGF_NOCASE),
	CFG_INT("VCom51", 0, CFGF_NOCASE),
	CFG_INT("VCom52", 0, CFGF_NOCASE),
	CFG_INT("VCom53", 0, CFGF_NOCASE),
	CFG_INT("VCom54", 0, CFGF_NOCASE),
	CFG_INT("VCom55", 0, CFGF_NOCASE),
	CFG_INT("VCom56", 0, CFGF_NOCASE),
	CFG_INT("VCom57", 0, CFGF_NOCASE),
	CFG_INT("VCom58", 0, CFGF_NOCASE),
	CFG_INT("VCom59", 0, CFGF_NOCASE),
	CFG_INT("VCom60", 0, CFGF_NOCASE),
	CFG_INT("VCom61", 0, CFGF_NOCASE),
	CFG_INT("VCom62", 0, CFGF_NOCASE),
	CFG_INT("VCom63", 0, CFGF_NOCASE),
	CFG_END()
};

/** @fn C_IniSys::LoadPortMap
 *  @brief 加载系统配置参数配置
 *  @param[out] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::LoadPortMap(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfg = cfg_init(OPT_PORTMAP_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSystemFile(cfg, fname) != 0)
	{
		cfg_free(cfg);
		return -1;
	}

	S_SYSCFG &SysConfig = pStatus->m_SysConfig;

	for (size_t i = 0; i < SYSTEM_SERIAL_PORT_MAX; i++)
	{
		char CfgItem[12];
		sprintf(CfgItem, "VCom%d", i);
		SysConfig.m_PortMap[i] = (S_BYTE)cfg_getint(cfg, CfgItem);
	}

	cfg_free(cfg);
	return 0;
}

int C_IniSys::LoadCommReg(S_STATUS *pStatus, const char *fname)
{
	S_REGISTERS Registers;
	if (C_CommReg::LoadCommonRegister(Registers, CFG_PATH_REGISTER) < 0)
		return -1;
	if (Registers.empty())
		return -1;
	sort(Registers.begin(), Registers.end());
	pStatus->m_CommRegisters.Init();
	pStatus->m_CommRegisters.m_Cnt = (S_WORD)Registers.size();
	if (pStatus->m_CommRegisters.m_Cnt > COMMON_REGISTER_MAX)
		logwt<<"*** 通用寄存器缓存太小，请调整COMMON_REGISTER_MAX ***"<<endl;;
	for (S_WORD i = 0; i < pStatus->m_CommRegisters.m_Cnt; i++)
		pStatus->m_CommRegisters.m_Item[i] = Registers[i];

	return 0;
}

/** @brief IP控制数据块*/
cfg_opt_t OPT_IPINFO_INFO[] =
{
	//参数名 ,参数默认值  ,标志	configuration file is case insensitive
	CFG_STR("IP_eth0", "192.168.0.100", CFGF_NOCASE),
	CFG_STR("NETMASK_eth0", "255.255.255.0", CFGF_NOCASE),
	CFG_STR("GW_eth0", "192.168.0.1", CFGF_NOCASE),
	CFG_STR("MAC_eth0", "00:01:02:B4:56:58", CFGF_NOCASE),
	CFG_STR("IP_eth1", "192.168.1.100", CFGF_NOCASE),
	CFG_STR("NETMASK_eth1", "255.255.255.0", CFGF_NOCASE),
	CFG_STR("GW_eth1", "192.168.1.1", CFGF_NOCASE),
	CFG_STR("MAC_eth1", "00:01:02:B4:56:59", CFGF_NOCASE),
	CFG_INT("ProxyType", 0, CFGF_NOCASE),
	CFG_STR("ProxyIP", "192.168.9.1", CFGF_NOCASE),//徐冠+将GPRS的设置集中写
	CFG_INT("ProxyPort", 10000, CFGF_NOCASE),
	CFG_INT("ProxyLink", 0, CFGF_NOCASE),
	CFG_STR("UserName", "", CFGF_NOCASE),
	CFG_STR("PassWord", "", CFGF_NOCASE),
	CFG_END()
};

/** @fn C_IniSys::LoadTermIPConfig
 *  @brief 加载IP信息配置
 *  @param[out] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::LoadTermIPConfig(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfg;
	int ip0,ip1,ip2,ip3;

	cfg = cfg_init(OPT_IPINFO_INFO, CFGF_NONE);
	if (C_IniBase::ParseSetFile(cfg, fname) != 0)
	{
		cfg_free(cfg);
		return -1;
	}
	struct S_IpConfig &IpInfo = pStatus->m_IpCofig;

	sscanf(cfg_getstr(cfg,"IP_eth0"), "%d.%d.%d.%d", &ip0, &ip1, &ip2, &ip3);
	IpInfo.m_TermIP[0] = ip0; IpInfo.m_TermIP[1] = ip1; IpInfo.m_TermIP[2] = ip2; IpInfo.m_TermIP[3] = ip3; 
	sscanf(cfg_getstr(cfg,"NETMASK_eth0"), "%d.%d.%d.%d", &ip0, &ip1, &ip2, &ip3);
	IpInfo.m_NetMask[0] = ip0; IpInfo.m_NetMask[1] = ip1; IpInfo.m_NetMask[2] = ip2; IpInfo.m_NetMask[3] = ip3; 
	sscanf(cfg_getstr(cfg,"GW_eth0"), "%d.%d.%d.%d", &ip0, &ip1, &ip2, &ip3);
	IpInfo.m_GateWay[0] = ip0; IpInfo.m_GateWay[1] = ip1; IpInfo.m_GateWay[2] = ip2; IpInfo.m_GateWay[3] = ip3;
	strcpy((char *)IpInfo.m_MAC, cfg_getstr(cfg, "MAC_eth0"));

	sscanf(cfg_getstr(cfg,"IP_eth1"), "%d.%d.%d.%d", &ip0, &ip1, &ip2, &ip3);
	IpInfo.m_IP_eth1[0] = ip0; IpInfo.m_IP_eth1[1] = ip1; IpInfo.m_IP_eth1[2] = ip2; IpInfo.m_IP_eth1[3] = ip3; 
	sscanf(cfg_getstr(cfg,"NETMASK_eth1"), "%d.%d.%d.%d", &ip0, &ip1, &ip2, &ip3);
	IpInfo.m_NetMask_eth1[0] = ip0; IpInfo.m_NetMask_eth1[1] = ip1; IpInfo.m_NetMask_eth1[2] = ip2; IpInfo.m_NetMask_eth1[3] = ip3; 
	sscanf(cfg_getstr(cfg,"GW_eth1"), "%d.%d.%d.%d", &ip0, &ip1, &ip2, &ip3);
	IpInfo.m_GateWay_eth1[0] = ip0; IpInfo.m_GateWay_eth1[1] = ip1; IpInfo.m_GateWay_eth1[2] = ip2; IpInfo.m_GateWay_eth1[3] = ip3;
	strcpy((char *)IpInfo.m_MAC_eth1, cfg_getstr(cfg, "MAC_eth1"));

	IpInfo.m_ProxyType = (INT8U)cfg_getint(cfg, "ProxyType");
	sscanf(cfg_getstr(cfg,"ProxyIP"), "%d.%d.%d.%d", &ip0, &ip1, &ip2, &ip3);
	IpInfo.m_ProxyIP[0] = ip0; IpInfo.m_ProxyIP[1] = ip1; IpInfo.m_ProxyIP[2] = ip2; IpInfo.m_ProxyIP[3] = ip3; 
	IpInfo.m_ProxyPort = (INT16U)cfg_getint(cfg, "ProxyPort");
	IpInfo.m_ProxyLink = (INT8U)cfg_getint(cfg, "ProxyLink");
	memset(IpInfo.m_UserName, 0x00, sizeof(IpInfo.m_UserName));
	strncpy((char *)IpInfo.m_UserName, cfg_getstr(cfg, "UserName"), 20);
	memset(IpInfo.m_Password, 0x00, sizeof(IpInfo.m_Password));
	strncpy((char *)IpInfo.m_Password, cfg_getstr(cfg, "PassWord"), 20);

	cfg_free(cfg);
	return 0;
}

/** @fn C_IniSys::SaveTermIPConfig
 *  @brief 保存IP信息配置
 *  @param[out] pStatus:配置结构体
 *  @param[in] cfgFile:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::SaveTermIPConfig(S_STATUS *pStatus, const char *fname)
{
	char buffer[64];
	string strFileInfo;

	struct S_IpConfig &IpInfo = pStatus->m_IpCofig;

	strFileInfo.append("#ipconfig.ini\r\n");
	strFileInfo.append("#function: ip config\r\n");

	sprintf(buffer, "%d.%d.%d.%d", IpInfo.m_TermIP[0], IpInfo.m_TermIP[1], IpInfo.m_TermIP[2], IpInfo.m_TermIP[3]);
	strFileInfo.append("\r\nIP_eth0 = ").append(buffer).append(" #集中器本地网口IP");
	sprintf(buffer, "%d.%d.%d.%d", IpInfo.m_NetMask[0], IpInfo.m_NetMask[1], IpInfo.m_NetMask[2], IpInfo.m_NetMask[3]);
	strFileInfo.append("\r\nNETMASK_eth0 = ").append(buffer).append(" #子网掩码");
	sprintf(buffer, "%d.%d.%d.%d", IpInfo.m_GateWay[0], IpInfo.m_GateWay[1], IpInfo.m_GateWay[2], IpInfo.m_GateWay[3]);
	strFileInfo.append("\r\nGW_eth0 = ").append(buffer).append(" #网关");
	strFileInfo.append("\r\nMAC_eth0 = ").append((char *)IpInfo.m_MAC).append(" #MAC地址");

	sprintf(buffer, "%d.%d.%d.%d", IpInfo.m_IP_eth1[0], IpInfo.m_IP_eth1[1], IpInfo.m_IP_eth1[2], IpInfo.m_IP_eth1[3]);
	strFileInfo.append("\r\nIP_eth1 = ").append(buffer).append(" #USB转网络IP");
	sprintf(buffer, "%d.%d.%d.%d", IpInfo.m_NetMask_eth1[0], IpInfo.m_NetMask_eth1[1], IpInfo.m_NetMask_eth1[2], IpInfo.m_NetMask_eth1[3]);
	strFileInfo.append("\r\nNETMASK_eth1 = ").append(buffer).append(" #USB转网络子网掩码");
	sprintf(buffer, "%d.%d.%d.%d", IpInfo.m_GateWay_eth1[0], IpInfo.m_GateWay_eth1[1], IpInfo.m_GateWay_eth1[2], IpInfo.m_GateWay_eth1[3]);
	strFileInfo.append("\r\nGW_eth1 = ").append(buffer).append(" #USB转网络网关");
	strFileInfo.append("\r\nMAC_eth1 = ").append((char *)IpInfo.m_MAC_eth1).append(" #USB转网络MAC地址");

	sprintf(buffer, "%d", IpInfo.m_ProxyType);
	strFileInfo.append("\r\nProxyType = ").append(buffer).append(" #0～3依次表示:不使用代理,http connect代理,socks4代理,socks5代理");
	sprintf(buffer, "%d.%d.%d.%d", IpInfo.m_ProxyIP[0], IpInfo.m_ProxyIP[1], IpInfo.m_ProxyIP[2], IpInfo.m_ProxyIP[3]);
	strFileInfo.append("\r\nProxyIP = ").append(buffer).append(" #代理服务器IP");
	sprintf(buffer, "%d", IpInfo.m_ProxyPort);
	strFileInfo.append("\r\nProxyPort = ").append(buffer);
	sprintf(buffer, "%d", IpInfo.m_ProxyLink);
	strFileInfo.append("\r\nProxyLink = ").append(buffer).append(" #0～1依次表示：无需验证、需要用户名/密码");
	strFileInfo.append("\r\nUserName = \"").append((char *)IpInfo.m_UserName).append("\"");
	strFileInfo.append("\r\nPassWord = \"").append((char *)IpInfo.m_Password).append("\"");

	strFileInfo.append("\r\n\r\n");

	return C_IniBase::SaveSetFile(strFileInfo, fname);
}

/** @fn C_IniSys::LoadFlowCtrlInfo
 *  @brief 加载流量控制配置
 *  @param[out] pContext:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::LoadFlowCtrlInfo(S_CONTEXT *pContext, const char *fname)
{
	return C_IniFlow::LoadFlowCtrlInfo(pContext->m_FlowInfo, fname);
}

/** @fn C_IniSys::SaveFlowCtrlInfo
 *  @brief 保存流量控制配置
 *  @param[out] pStatus:配置结构体
 *  @param[in] cfgFile:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::SaveFlowCtrlInfo(S_CONTEXT *pContext, const char *fname)
{
	return C_IniFlow::SaveFlowCtrlInfo(pContext->m_FlowInfo, fname);
}

/** @brief 告警配置结构 */
cfg_opt_t OPT_ALARM_SETTINGS[] =
{
	CFG_STR_LIST("ReportAlarmEven", "{}", CFGF_NONE),
	CFG_STR_LIST("AlarmEven", "{}", CFGF_NONE),
	CFG_STR_LIST("OnlyEvent", "{}", CFGF_NONE),
	CFG_END()
};
/** @fn C_IniSys::LoadAlarmInfo
 *  @brief 加载告警配置
 *  @param[out] pContext:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::LoadAlarmInfo(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfgAlarm;
	cfgAlarm = cfg_init(OPT_ALARM_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSetFile(cfgAlarm, fname) != 0)
	{
		cfg_free(cfgAlarm);
		return -1;
	}

	S_AlarmInfo &AlarmInfo = pStatus->m_AlarmInfo;
	AlarmInfo.Init();
	for (unsigned int i = 0; i < cfg_size(cfgAlarm, "ReportAlarmEven") && i < CONS_MAX_ALARM_NUMBERS; i++)
	{
		AlarmInfo.m_ReportAlarmEven.SetBit(cfg_getnstr(cfgAlarm,"ReportAlarmEven",i),'-');
	}
	AlarmInfo.m_ReportAlarmEven.Stat();

	for (unsigned int i = 0; i < cfg_size(cfgAlarm, "AlarmEven") && i < CONS_MAX_ALARM_NUMBERS; i++)
	{
		AlarmInfo.m_AlarmEven.SetBit(cfg_getnstr(cfgAlarm,"AlarmEven",i),'-');
	}
	AlarmInfo.m_AlarmEven.Stat();

	for (unsigned int i = 0; i < cfg_size(cfgAlarm, "OnlyEvent") && i < CONS_MAX_ALARM_NUMBERS; i++)
	{
		AlarmInfo.m_OnlyEvent.SetBit(cfg_getnstr(cfgAlarm,"OnlyEvent",i),'-');
	}
	AlarmInfo.m_OnlyEvent.Stat();

	cfg_free(cfgAlarm);
	return 0;
}

/** @fn C_IniSys::SaveFlowCtrlInfo
 *  @brief 保存告警配置
 *  @param[out] pStatus:配置结构体
 *  @param[in] cfgFile:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::SaveAlarmInfo(S_STATUS *pStatus, const char *fname)
{
	string strAlarmEven;
	string buf;
	S_AlarmInfo &AlarmInfo = pStatus->m_AlarmInfo;

	strAlarmEven.append("#1:计量装置门打开\r\n"
						"#2:窃电\r\n"
						"#3:电流回路逆相序\r\n"
						"#4:电压回路逆相序\r\n"
						"#5:电流不平衡\r\n"
						"#6:零序电流偏大\r\n"
						"#7:A相CT二次侧短路\r\n"
						"#8:B相CT二次侧短路\r\n"
						"#9:C相CT二次侧短路\r\n"
						"#10:A相CT二次侧开路\r\n"
						"#11:B相CT二次侧开路\r\n"
						"#12:C相CT二次侧开路\r\n"
						"#13:A相潮流反向\r\n"
						"#14:B相潮流反向\r\n"
						"#15:C相潮流反向\r\n"
						"#16:A相电流过流\r\n"
						"#17:B相电流过流\r\n"
						"#18:C相电流过流\r\n"
						"#19:A相电流失流\r\n"
						"#20:B相电流失流\r\n"
						"#21:C相电流失流\r\n"
						"#22:A相电压失压\r\n"
						"#23:B相电压失压\r\n"
						"#24:C相电压失压\r\n"
						"#25:全失压\r\n"
						"#26:A相电压过压\r\n"
						"#27:B相电压过压\r\n"
						"#28:C相电压过压\r\n"
						"#29:A相电压断相\r\n"
						"#30:B相电压断相\r\n"
						"#31:C相电压断相r\n"
						"#32:A相电压畸变率超标\r\n"
						"#33:B相电压畸变率超标\r\n"
						"#34:C相电压畸变率超标\r\n"
						"#35:A相电流畸变率超标\r\n"
						"#36:B相电流畸变率超标\r\n"
						"#37:C相电流畸变率超标\r\n"
						"#38:无功过补偿\r\n"
						"#39:无功欠补偿\r\n"
						"#40:功率超定值\r\n"
						"#41:负荷过载\r\n"
						"#42:超合同容量用电\r\n"
						"#43:剩余购电量不足\r\n"
						"#44:示度下降\r\n"
						"#45:电能表飞走\r\n"
						"#46:电能表停走\r\n"
						"#47:电能表通信失败\r\n"
						"#48:差动报警\r\n"
						"#49:最大需量结算日自动复零\r\n"
						"#50:最大需量手动复零\r\n"
						"#51:时钟电池电压过低\r\n"
						"#52:充电电池电压过低\r\n"
						"#53:电能表状态字变化\r\n"
						"#54:终端停电\r\n"
						"#55:电能表编程时间更改\r\n"
						"#56:电能表时段或费率更改\r\n"
						"#57:电能表脉冲常数更改\r\n"
						"#58:电能表的互感器倍率更改\r\n"
						"#59:遥信变位\r\n"
						"#60:月通信流量越限\r\n"
						"#61:日线损率越限\r\n"
						"#62:继电器变位\r\n"
						"#63:电能表拉合闸失败\r\n"
						"#64:抄表失败\r\n"
						"#65:电能表时钟超差\r\n"
						"#66:电能表校时失败\r\n"
						"#67:第1轮跳闸\r\n"
						"#68:第2轮跳闸\r\n"
						"#69:第3轮跳闸\r\n"
						"#70:第4轮跳闸\r\n"
						"#71:第5轮跳闸\r\n"
						"#72:第6轮跳闸\r\n"
						"#73:第7轮跳闸\r\n"
						"#74:第8轮跳闸\r\n"
						"#75:电能表A、B、C相失压总次数\r\n"
						"#76:电能表A、B、C相失流总次数\r\n"
						"#77:电能表A、B、C相潮流反向总次数\r\n"
						"#78:电能表编程总次数\r\n"
						"#79:电能表停电\r\n"
						"#80:A相电压暂态事件\r\n"
						"#81:B相电压暂态事件\r\n"
						"#82:C相电压暂态事件\r\n"
						"#83:A相电压偏差越限\r\n"
						"#84:B相电压偏差越限\r\n"
						"#85:C相电压偏差越限\r\n"
						"#86:频率偏差越限\r\n"
						"#87:A相闪变越限\r\n"
						"#88:B相闪变越限\r\n"
						"#89:C相闪变越限\r\n"
						"#90:电压不平衡越限\r\n"
						"#91:读所有主动上送未成功的告警数据\r\n"
						"#92:所有告警类型\r\n"
						"\r\n");
	strAlarmEven.append("ReportAlarmEven=\r\n{");
	AlarmInfo.m_ReportAlarmEven.PrintParameter(buf);
	strAlarmEven.append(buf).append("}\r\n");
	buf.erase();
	strAlarmEven.append("AlarmEven=\r\n{");
	AlarmInfo.m_AlarmEven.PrintParameter(buf);
	strAlarmEven.append(buf).append("}\r\n");
	buf.erase();
	strAlarmEven.append("OnlyEvent=\r\n{");
	AlarmInfo.m_OnlyEvent.PrintParameter(buf);
	strAlarmEven.append(buf).append("}\r\n");

	return C_IniBase::SaveSetFile(strAlarmEven, fname);
}


/** @brief 级联通讯参数配置结构体 */
cfg_opt_t OPT_FORWARD_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_true,CFGF_NOCASE),
	CFG_STR("SlaveAddr1", "FFFFFFFF", CFGF_NONE),
	CFG_STR("SlaveAddr2", "FFFFFFFF", CFGF_NONE),
	CFG_STR("SlaveAddr3", "FFFFFFFF", CFGF_NONE),
	CFG_STR("SlaveAddr4", "FFFFFFFF", CFGF_NONE),
	CFG_STR("MasterAddr", "FFFFFFFF", CFGF_NONE),
	CFG_STR("GatherAddr", "FFFFFFFF", CFGF_NONE),
	CFG_INT("WaitSlaveResponseS", 20, CFGF_NONE),
	CFG_INT("WaitMasterCtrlS", 20, CFGF_NONE),
	CFG_INT("CT", 1, CFGF_NONE),
	CFG_INT("PT", 1, CFGF_NONE),
	CFG_INT("PollIntervalS", 300, CFGF_NONE),
	CFG_STR_LIST("Channel", "{}", CFGF_NONE),
	CFG_END()
};
cfg_opt_t OPT_FORWARDS_SETTINGS[] =
{
	CFG_SEC("Forward", OPT_FORWARD_SETTINGS, CFGF_MULTI |CFGF_NOCASE),
	CFG_END()
};
/** @fn C_IniSys::LoadForwardInfo
 *  @brief 加载级联通讯参数
 *  @param[out] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
 int C_IniSys::LoadForwardInfo(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfg  = cfg_init(OPT_FORWARDS_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSetFile(cfg, fname)!=0)
	{
		cfg_free(cfg);
		return -1;
	}

	for(unsigned int i = 0; i < cfg_size(cfg, "Forward"); i++)
	{
		cfg_t* cfgForward = cfg_getnsec(cfg, "Forward", i);
//		if (cfg_getbool(cfgForward, "Enable") == cfg_false)
//			continue;
		S_ForwardInfo& info = pStatus->m_ForwardInfo;

		info.Init();
		info.SetRTUA("SlaveAddr1", cfg_getstr(cfgForward, "SlaveAddr1"));
		info.SetRTUA("SlaveAddr2", cfg_getstr(cfgForward, "SlaveAddr2"));
		info.SetRTUA("SlaveAddr3", cfg_getstr(cfgForward, "SlaveAddr3"));
		info.SetRTUA("SlaveAddr4", cfg_getstr(cfgForward, "SlaveAddr4"));
		info.SetRTUA("MasterAddr", cfg_getstr(cfgForward, "MasterAddr"));
		info.SetRTUA("GatherAddr", cfg_getstr(cfgForward, "GatherAddr"));

		info.m_Enable = (cfg_getbool(cfgForward, "Enable") == cfg_true)? true:false;
		info.m_WaitSlaveResponseS = (S_BYTE)cfg_getint(cfgForward,"WaitSlaveResponseS");
		info.m_WaitMasterCtrlS = (S_BYTE)cfg_getint(cfgForward,"WaitMasterCtrlS");
		info.m_CT = (S_WORD)cfg_getint(cfgForward,"CT"); ///=300 #刷新无线信号间隔，缺省300秒
		info.m_PT = (S_WORD)cfg_getint(cfgForward,"PT");
		info.m_PollIntervalS = (S_WORD)cfg_getint(cfgForward,"PollIntervalS");

		for (unsigned int i = 0; i < cfg_size(cfgForward, "Channel") && i < FORWARD_CHANNL_MAX; i++)
		{
			strncpy(info.m_ChannlName[i], cfg_getnstr(cfgForward, "Channel", i), sizeof(info.m_ChannlName[i])-1);
		}
	}

	cfg_free(cfg);
	return 0;
}

/** @fn C_IniSys::SaveForwardInfo
 *  @brief 保存级联通讯参数
 *  @param[in] pStatus:配置结构体
 *  @param[in] name:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::SaveForwardInfo(S_STATUS *pStatus, const char *fname)
{
	char Buffer[32];
	string strFileInfo;
	const char *pPrompt = " #级联从终端地址（等于FFFFFFF时表示不存在）";
	strFileInfo.append("#note 级联通讯配置文件\r\n" "#see upchannl.ini\r\n\r\n");

	S_ForwardInfo *pInfo = &pStatus->m_ForwardInfo;
	strFileInfo.append("Forward\r\n{");
	strFileInfo.append("\r\n\tEnable=").append(C_IniBase::GetBoolKey(pInfo->m_Enable));

	memset(Buffer, 0x00, sizeof(Buffer));
	strFileInfo.append("\r\n\tSlaveAddr1=").append(pInfo->GetRTUA("SlaveAddr1", Buffer)).append(pPrompt);
	memset(Buffer, 0x00, sizeof(Buffer));
	strFileInfo.append("\r\n\tSlaveAddr2=").append(pInfo->GetRTUA("SlaveAddr2", Buffer)).append(pPrompt);
	memset(Buffer, 0x00, sizeof(Buffer));
	strFileInfo.append("\r\n\tSlaveAddr3=").append(pInfo->GetRTUA("SlaveAddr3", Buffer)).append(pPrompt);
	memset(Buffer, 0x00, sizeof(Buffer));
	strFileInfo.append("\r\n\tSlaveAddr4=").append(pInfo->GetRTUA("SlaveAddr4", Buffer)).append(pPrompt);
	memset(Buffer, 0x00, sizeof(Buffer));
	strFileInfo.append("\r\n\tGatherAddr=").append(pInfo->GetRTUA("GatherAddr", Buffer)).append(" #主终端地址（等于FFFFFFFF时表示不存在）");
	memset(Buffer, 0x00, sizeof(Buffer));
	strFileInfo.append("\r\n\tGatherAddr=").append(pInfo->GetRTUA("GatherAddr", Buffer)).append(" #被抄终端地址");

	sprintf(Buffer,"%d", pInfo->m_WaitSlaveResponseS);
	strFileInfo.append("\r\n\tWaitSlaveResponseS=").append(Buffer).append(" #主终端等待从终端应答的超时判定时间，缺省值20秒");
	sprintf(Buffer,"%d", pInfo->m_WaitMasterCtrlS);
	strFileInfo.append("\r\n\tWaitMasterCtrlS=").append(Buffer).append(" #从终端等待主终端级联控制命令的超时判定时间，缺省值20秒");
	sprintf(Buffer,"%d", pInfo->m_CT);
	strFileInfo.append("\r\n\tCT=").append(Buffer).append(" #级联台区终端CT倍率");
	sprintf(Buffer,"%d", pInfo->m_PT);
	strFileInfo.append("\r\n\tPT=").append(Buffer).append(" #级联台区终端PT倍率");
	sprintf(Buffer,"%d", pInfo->m_PollIntervalS);
	strFileInfo.append("\r\n\tPollIntervalS=").append(Buffer).append(" #轮询周期");

	strFileInfo.append("\r\n\tChannel={");
	for (int i = 0; i < FORWARD_CHANNL_MAX; i++)
	{
		if (pInfo->m_ChannlName[i][0])
			strFileInfo.append(pInfo->m_ChannlName[i]).append(",");
	}
	if (*(strFileInfo.end()-1) == ',')
		strFileInfo.erase(strFileInfo.end()-1);
	strFileInfo.append("} ##级联通道，被抄通道");

	strFileInfo.append("\r\n}\r\n\r\n");

	return C_IniBase::SaveSetFile(strFileInfo, fname);
}

/** @breif 任务模板配置 */
cfg_opt_t OPT_TASKTEMP_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_true,CFGF_NOCASE),
	CFG_STR("TaskType","unknow", CFGF_NOCASE),
	CFG_INT("Priority", 1, CFGF_NOCASE),
	CFG_STR("StdDate","2009-10-14", CFGF_NOCASE),
	CFG_STR("StdTime","00:10:00", CFGF_NOCASE),
	CFG_STR("PeriodUnit","hour", CFGF_NOCASE),
	CFG_INT("PeriodVal", 6, CFGF_NOCASE),
	CFG_INT("RetryTimes", 1, CFGF_NOCASE),
	CFG_INT("ExecTimeS", 7200, CFGF_NOCASE),
	CFG_INT("ReportPeriodS", 0, CFGF_NOCASE),
	CFG_INT("MaxStartTimes", 0, CFGF_NOCASE),
	CFG_END()
};

cfg_opt_t OPT_TASKTEMPS_SETTINGS[] =
{
	CFG_SEC("Task", OPT_TASKTEMP_SETTINGS, CFGF_MULTI | CFGF_NOCASE),
	CFG_END()
};

/** @fn C_IniSys::LoadTaskTemplateInfo
 *  @brief 加载任务模板配置
 *  @param[out] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::LoadTaskTemplateInfo(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfg = cfg_init(OPT_TASKTEMPS_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSetFile(cfg, fname) != 0)
	{
		cfg_free(cfg);
		return -1;
	}

	S_TaskTemplates *pTaskTemplates = &pStatus->m_TaskTemplates; ///< 任务模块
	pTaskTemplates->Init();
	for(unsigned int i = 0; i < cfg_size(cfg, "Task") && i < TASK_TEMPLATE_MAX; i++)
	{
		cfg_t* cfgTaskCfg = cfg_getnsec(cfg, "Task", i);
		S_TaskTemplate &Task = pTaskTemplates->m_Item[i];

		Task.m_Enable = (cfg_getbool(cfgTaskCfg, "Enable") == cfg_true)? true:false;
		strcpy(Task.m_TaskType, cfg_getstr(cfgTaskCfg, "TaskType"));
		Task.m_Priority = (S_BYTE)cfg_getint(cfgTaskCfg, "Priority");

		S_DATE date;
		sscanf(cfg_getstr(cfgTaskCfg,"StdDate"), "%d-%d-%d", &date.m_Year, &date.m_Mon, &date.m_Day);
		sscanf(cfg_getstr(cfgTaskCfg,"StdTime"), "%d:%d:%d", &date.m_Hour, &date.m_Min, &date.m_Sec);
		Task.m_StandardTime = C_TIME::MakeTime(date);

		Task.m_TimeUnit = C_TIME::GetTimeUnit(cfg_getstr(cfgTaskCfg,"PeriodUnit"));
		Task.m_PeriodVal = (S_BYTE)cfg_getint(cfgTaskCfg,"PeriodVal");
		Task.m_RetryTimes = (S_BYTE)cfg_getint(cfgTaskCfg,"RetryTimes");
		Task.m_ExecTimeS = (S_DWORD)cfg_getint(cfgTaskCfg,"ExecTimeS");
		Task.m_ReportPeriodS = (S_DWORD)cfg_getint(cfgTaskCfg,"ReportPeriodS");
		Task.m_MaxStartTimes = (S_BYTE)cfg_getint(cfgTaskCfg,"MaxStartTimes");
	}

	cfg_free(cfg);
	return 0;
}

/** @breif 国网主站数据项配置 */
cfg_opt_t OPT_MSDI_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_false, CFGF_NOCASE),
	CFG_INT("Class", 1, CFGF_NOCASE),
	CFG_INT("FN", 1, CFGF_NOCASE),
	CFG_STR("DI", "00000000", CFGF_NOCASE),
	CFG_STR("Freeze", "HOUR", CFGF_NOCASE),
	CFG_STR("Type", "Normal", CFGF_NOCASE),
	CFG_BOOL("Time", cfg_false, CFGF_NOCASE),
	CFG_BOOL("TariffBlock", cfg_false, CFGF_NOCASE),
	CFG_INT("TariffCnt", 1, CFGF_NOCASE),
	CFG_STR_LIST("MapCommID","{}", CFGF_NOCASE),
	CFG_STR_LIST("DisableID","{}", CFGF_NOCASE),
	CFG_END()
};

cfg_opt_t OPT_MSDIS_SETTINGS[] =
{
	CFG_SEC("Register", OPT_MSDI_SETTINGS, CFGF_MULTI | CFGF_NOCASE),
	CFG_END()
};
/** @fn C_IniSys::LoadMsdiInfo
 *  @brief 加载主站数据项配置
 *  @param[out] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::LoadMsdiInfo(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfg = cfg_init(OPT_MSDIS_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseRegisterFile(cfg, fname) != 0)
	{
		cfg_free(cfg);
		return -1;
	}

	S_MSDIS *pMSDIS = &pStatus->m_MSDIS; 
	pMSDIS->Init();
	for(unsigned int i = 0; i < cfg_size(cfg, "Register"); i++)
	{
		cfg_t* cfgMsdi = cfg_getnsec(cfg, "Register", i);
//		if (cfg_getbool(cfgMsdi, "Enable") == cfg_false)
//			continue;
		S_WORD Class = (S_WORD)cfg_getint(cfgMsdi, "Class");
		S_WORD FN = (S_WORD)cfg_getint(cfgMsdi, "FN");
		if (FN >= MSDI_ITEM_MAX)
			continue;

		S_MSDIF *pMsdif = NULL;
		if (Class == CLASSDATA_1) pMsdif = &pMSDIS->m_ItemClass1[FN];
		else if (Class == CLASSDATA_2) pMsdif = &pMSDIS->m_ItemClass2[FN];
		else if (Class == CLASSDATA_3) pMsdif = &pMSDIS->m_ItemClass3[FN];
		else if (Class == CLASSDATA_4) pMsdif = &pMSDIS->m_ItemClass4[FN];
		else if (Class == CLASSDATA_5) pMsdif = &pMSDIS->m_ItemClass5[FN];
		else if (Class == CLASSDATA_6) pMsdif = &pMSDIS->m_ItemClass6[FN];
		else if (Class == CLASSDATA_7) pMsdif = &pMSDIS->m_ItemClass7[FN];
		else if (Class == CLASSDATA_8) pMsdif = &pMSDIS->m_ItemClass8[FN];
		else if (Class == CLASSDATA_9) pMsdif = &pMSDIS->m_ItemClass9[FN];
		else continue;

		//pMsdif->m_Enable = true;
		pMsdif->m_Enable = cfg_getbool(cfgMsdi, "Enable") == cfg_true? true:false;
		pMsdif->m_Type = GetMsdiTypeID(cfg_getstr(cfgMsdi, "Type"));
		pMsdif->m_Freeze = GetMsdiFreezeType(cfg_getstr(cfgMsdi, "Freeze"));
		if (cfg_getbool(cfgMsdi, "Time") == cfg_true) pMsdif->m_Flag |= MSDI_FLAG_TIME;
//		if (cfg_getbool(cfgMsdi, "Tariff") == cfg_true) pMsdif->m_Flag |= MSDI_FLAG_TARIFF;
		if (cfg_getbool(cfgMsdi, "TariffBlock") == cfg_true) pMsdif->m_Flag |= MSDI_FLAG_BLOCK;
		pMsdif->m_TariffCnt = (S_BYTE)cfg_getint(cfgMsdi, "TariffCnt");
		sscanf(cfg_getstr(cfgMsdi, "DI"), "%08X", &pMsdif->m_DI);

		for (INT32U n = 0; n < cfg_size(cfgMsdi, "DisableID"); n++)
		{
			pMsdif->m_DisableReg.ParseParameter(cfg_getnstr(cfgMsdi, "DisableID", n));
		}

		pMsdif->m_idxRegBegin = pMSDIS->m_idxCommReg;
		for (INT32U n = 0; n < cfg_size(cfgMsdi, "MapCommID"); n++)
		{
			S_DWORD di = 0;
			sscanf(cfg_getnstr(cfgMsdi, "MapCommID", n), "%x", &di);
			pMSDIS->m_CommRegID[pMSDIS->m_idxCommReg] = di;
			pMSDIS->m_idxCommReg += 1;
		}
		pMsdif->m_idxRegEnd = pMSDIS->m_idxCommReg;
		if (pMSDIS->m_idxCommReg > MSDI_DATA_ID_MAX)
			loget<<"主站数据项映射通用寄存器太多,请调整MSDI_DATA_ID_MAX的大小"<<endl;
	}

	cfg_free(cfg);
	return 0;
}

/** @brief 级联通讯参数配置结构体 */
cfg_opt_t OPT_LIMIT_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_true,CFGF_NOCASE),
	CFG_INT("OverLoadM", 5, CFGF_NONE),
	CFG_INT("OverLoadEndM", 5, CFGF_NONE),
	CFG_INT("MissingPhaseM", 15, CFGF_NONE),
	CFG_INT("MissingPhaseEndM", 15, CFGF_NONE),
	CFG_INT("ReversedPhaseM", 15, CFGF_NONE),
	CFG_INT("ReversedPhaseEndM", 15, CFGF_NONE),
	CFG_INT("ReversedPolarityM", 15, CFGF_NONE),
	CFG_INT("ReversedPolarityEndM", 15, CFGF_NONE),
	CFG_INT("MeterTimeDiffM", 1, CFGF_NONE),
	CFG_INT("MeterFly", 10, CFGF_NONE),
	CFG_STR("MeterStop", "0.1", CFGF_NONE),
	CFG_INT("MeterLastFee", 8192, CFGF_NONE),
	CFG_END()
};
cfg_opt_t OPT_LIMITS_SETTINGS[] =
{
	CFG_SEC("Limit", OPT_LIMIT_SETTINGS, CFGF_MULTI |CFGF_NOCASE),
	CFG_END()
};

/** @fn C_IniSys::LoadPowerLimitInfo
 *  @brief 加载级联通讯参数
 *  @param[out] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::LoadPowerLimitInfo(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfg  = cfg_init(OPT_LIMITS_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSetFile(cfg, fname)!=0)
	{
		cfg_free(cfg);
		return -1;
	}

	for(unsigned int i = 0; i < cfg_size(cfg, "Limit"); i++)
	{
		cfg_t* cfgLimit = cfg_getnsec(cfg, "Limit", i);
		if (cfg_getbool(cfgLimit, "Enable") == cfg_false)
			continue;
		S_PowerLimit& info = pStatus->m_PowerLimit;

		info.m_OverLoadM = (S_BYTE)cfg_getint(cfgLimit, "OverLoadM");
		info.m_OverLoadEndM = (S_BYTE)cfg_getint(cfgLimit, "OverLoadEndM");
		info.m_MissingPhaseM = (S_BYTE)cfg_getint(cfgLimit, "MissingPhaseM");
		info.m_MissingPhaseEndM = (S_BYTE)cfg_getint(cfgLimit, "MissingPhaseEndM");
		info.m_ReversedPhaseM = (S_BYTE)cfg_getint(cfgLimit, "ReversedPhaseM");
		info.m_ReversedPhaseEndM = (S_BYTE)cfg_getint(cfgLimit, "ReversedPhaseEndM");
		info.m_ReversedPolarityM = (S_BYTE)cfg_getint(cfgLimit, "ReversedPolarityM");
		info.m_ReversedPolarityEndM = (S_BYTE)cfg_getint(cfgLimit, "ReversedPolarityEndM");
		info.m_MeterTimeDiffM = (S_BYTE)cfg_getint(cfgLimit, "MeterTimeDiffM");
		info.m_MeterFly = (S_BYTE)cfg_getint(cfgLimit, "MeterFly");

		C_BCD bcdStop(cfg_getstr(cfgLimit, "MeterStop"));
		const S_BYTE *pBcd = bcdStop.GetBcdData();

		info.m_MeterLastFee = (S_DWORD)cfg_getint(cfgLimit, "MeterLastFee");
		memcpy(&info.m_MeterStop, pBcd+1, 2); 
	}

	cfg_free(cfg);
	return 0;
}

/** @fn C_IniSys::SavePowerLimitInfo
 *  @brief 保存级联通讯参数
 *  @param[in] pStatus:配置结构体
 *  @param[in] name:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::SavePowerLimitInfo(S_STATUS *pStatus, const char *fname)
{
	char Buffer[32];
	string strFileInfo;
	strFileInfo.append("#note 阈值参数\r\n\r\n");

	S_PowerLimit& info = pStatus->m_PowerLimit;
	strFileInfo.append("Limit\r\n{");
	strFileInfo.append("\r\n\tEnable=").append("true");

	sprintf(Buffer,"%d", info.m_OverLoadM);
	strFileInfo.append("\r\n\tOverLoadM=").append(Buffer).append(" #分 判断过负荷、电流过负荷、超合同容量用电、电流不平衡度、无功欠补偿、无功过补偿的时间缺省5分钟");
	sprintf(Buffer,"%d", info.m_OverLoadEndM);
	strFileInfo.append("\r\n\tOverLoadEndM=").append(Buffer).append(" #分 判断过负荷、电流过负荷、超合同容量用电、电流不平衡度、无功欠补偿、无功过补偿恢复的时间缺省5分钟");
	sprintf(Buffer,"%d", info.m_MissingPhaseM);
	strFileInfo.append("\r\n\tMissingPhaseM=").append(Buffer).append(" #分 判断断相、失压时间，缺省值15分钟");
	sprintf(Buffer,"%d", info.m_MissingPhaseEndM);
	strFileInfo.append("\r\n\tMissingPhaseEndM=").append(Buffer).append(" #分 判断断相、失压恢复时间，缺省值15分钟");
	sprintf(Buffer,"%d", info.m_ReversedPhaseM);
	strFileInfo.append("\r\n\tReversedPhaseM=").append(Buffer).append(" #分 判断电压逆相序时间，缺省值15分钟");
	sprintf(Buffer,"%d", info.m_ReversedPhaseEndM);
	strFileInfo.append("\r\n\tReversedPhaseEndM=").append(Buffer).append(" #分 判断电压逆相序恢复时间，缺省值15分钟");
	sprintf(Buffer,"%d", info.m_ReversedPolarityM);
	strFileInfo.append("\r\n\tReversedPolarityM=").append(Buffer).append(" #分 判断电流反极性时间，缺省值15分钟");
	sprintf(Buffer,"%d", info.m_ReversedPolarityEndM);
	strFileInfo.append("\r\n\tReversedPolarityEndM=").append(Buffer).append(" #分 判断电流反极性恢复时间，缺省值15分钟");
	sprintf(Buffer,"%d", info.m_MeterTimeDiffM);
	strFileInfo.append("\r\n\tMeterTimeDiffM=").append(Buffer).append(" #分 电能表时钟超差阈值，缺省值1分钟");
	sprintf(Buffer,"%d", info.m_MeterFly);
	strFileInfo.append("\r\n\tMeterFly=").append(Buffer).append(" #电能表飞走阈值，缺省为10倍");


	C_BCD bcdStop((S_BYTE*)&info.m_MeterStop, 2, 2);
	string strBcd = bcdStop;
	strFileInfo.append("\r\n\tMeterStop=").append(strBcd).append(" #NN.NN (BCD) kWh	电能表停走阈值：电量示度停止增长时按照有功功率计算应走电量值，越此值则电能表停走，缺省为0.1kWh");

	sprintf(Buffer,"%d", info.m_MeterLastFee);
	strFileInfo.append("\r\n\tMeterLastFee=").append(Buffer).append(" #剩余金额不足阈值：缺省为20.00 元");
	

	strFileInfo.append("\r\n}\r\n\r\n");

	return C_IniBase::SaveSetFile(strFileInfo, fname);
}


/** @brief 模拟量参数配置结构体 */
cfg_opt_t OPT_ANALOG_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_true,CFGF_NOCASE),
	CFG_INT("SN", 1, CFGF_NONE),
	CFG_STR("AnalogType", "voltage", CFGF_NONE),
	CFG_STR("Phase", "A", CFGF_NONE),
	CFG_INT("MP", 1, CFGF_NONE),
	CFG_END()
};
cfg_opt_t OPT_ANALOGS_SETTINGS[] =
{
	CFG_INT("AnalogCnt", 1, CFGF_NONE),
	CFG_SEC("Analog", OPT_ANALOG_SETTINGS, CFGF_MULTI |CFGF_NOCASE),
	CFG_END()
};

const S_CFGITEM g_AnalogTypeItem[] =
{
	{1, "voltage"},{2, "current"},{3, "time"}
};
const S_CFGITEM g_AnalogPhaseItem[] =
{
	{1, "A"},{2, "B"},{3, "C"}
};

/** @fn C_IniSys::LoadAnalogInfo
 *  @brief 加载模拟量参数
 *  @param[out] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::LoadAnalogInfo(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfg  = cfg_init(OPT_ANALOGS_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSetFile(cfg, fname)!=0)
	{
		cfg_free(cfg);
		return -1;
	}

	S_ANALOGS *pAnalogs = STATUS.GetAnalogInfos();
	pAnalogs->Init();
	pAnalogs->m_AnalogCnt = (S_WORD)cfg_getint(cfg, "AnalogCnt");
	if (pAnalogs->m_AnalogCnt >= SUM_ALL_ITEM_MAX)
		pAnalogs->m_AnalogCnt = SUM_ALL_ITEM_MAX - 1;

	for(unsigned int i = 0; i < cfg_size(cfg, "Analog"); i++)
	{
		cfg_t* cfgAnalog = cfg_getnsec(cfg, "Analog", i);
		S_WORD sn = (S_WORD)cfg_getint(cfgAnalog, "SN");
		if (sn > pAnalogs->m_AnalogCnt)
			continue;

		S_ANALOG *pInfo = &pAnalogs->m_Item[sn];
		pInfo->m_Enable = (cfg_getbool(cfgAnalog, "Enable") == cfg_false)? false:true;
		pInfo->m_AnalogType = C_IniBase::MakeValue(g_AnalogTypeItem, sizeof(g_AnalogTypeItem)/sizeof(g_AnalogTypeItem[0]), cfg_getstr(cfgAnalog, "AnalogType"));
		pInfo->m_Phase = C_IniBase::MakeValue(g_AnalogPhaseItem, sizeof(g_AnalogPhaseItem)/sizeof(g_AnalogPhaseItem[0]), cfg_getstr(cfgAnalog, "Phase"));
		pInfo->m_MP = (S_WORD)cfg_getint(cfgAnalog, "MP");
	}

	cfg_free(cfg);
	return 0;
}

/** @fn C_IniSys::SaveAnalogInfo
 *  @brief 保存模拟量参数
 *  @param[in] pStatus:配置结构体
 *  @param[in] name:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::SaveAnalogInfo(S_STATUS *pStatus, const char *fname)
{
	char Buffer[32];
	string strFileInfo;
	strFileInfo.append("#note 模拟量配置文件\r\n");
	strFileInfo.append("#remark SN=序号 范围[1 - 253]\r\n");
	strFileInfo.append("#remark AnalogType=模拟量类型 voltage电压,current电流,time电压监测时间\r\n");
	strFileInfo.append("#remark Phase=相位 A相B相C相\r\n");

	S_ANALOGS *pAnalogs = STATUS.GetAnalogInfos();

	sprintf(Buffer,"%d", pAnalogs->m_AnalogCnt);
	strFileInfo.append("\r\nAnalogCnt=").append(Buffer);

	for (S_WORD i = 1; i <= pAnalogs->m_AnalogCnt; i++)
	{
		S_ANALOG *pInfo = &pAnalogs->m_Item[i];

		strFileInfo.append("\r\nAnalog\r\n{");
		strFileInfo.append("\r\n\tEnable=").append(C_IniBase::GetBoolKey(pInfo->m_Enable));

		sprintf(Buffer,"%d", i);
		strFileInfo.append("\r\n\tSN=").append(Buffer);
		strFileInfo.append("\r\n\tAnalogType=").append(C_IniBase::MakeKey(g_AnalogTypeItem,
			sizeof(g_AnalogTypeItem)/sizeof(g_AnalogTypeItem[0]), pInfo->m_AnalogType));
		strFileInfo.append("\r\n\tPhase=").append(C_IniBase::MakeKey(g_AnalogPhaseItem,
			sizeof(g_AnalogPhaseItem)/sizeof(g_AnalogPhaseItem[0]), pInfo->m_Phase));
		sprintf(Buffer,"%d", pInfo->m_MP);
		strFileInfo.append("\r\n\tMP=").append(Buffer);

		strFileInfo.append("\r\n}\r\n\r\n");
	}

	return C_IniBase::SaveSetFile(strFileInfo, fname);
}


/** @brief 模拟量参数配置结构体 */
cfg_opt_t OPT_PULSE_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_true,CFGF_NOCASE),
	CFG_INT("SN", 1, CFGF_NONE),
	CFG_INT("MP", 1, CFGF_NONE),
	CFG_INT("PulseType", 1, CFGF_NONE),
	CFG_INT("PulseConst", 1, CFGF_NONE),
	CFG_END()
};
cfg_opt_t OPT_PULSES_SETTINGS[] =
{
	CFG_INT("PulseCnt", 1, CFGF_NONE),
	CFG_SEC("Pulse", OPT_PULSE_SETTINGS, CFGF_MULTI |CFGF_NOCASE),
	CFG_END()
};

/** @fn C_IniSys::LoadPulseInfo
 *  @brief 加载脉冲量参数
 *  @param[out] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::LoadPulseInfo(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfg  = cfg_init(OPT_PULSES_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSetFile(cfg, fname)!=0)
	{
		cfg_free(cfg);
		return -1;
	}

	S_PULSES *pPulses = STATUS.GetPulseInfos();
	pPulses->Init();
	pPulses->m_PulseCnt = (S_WORD)cfg_getint(cfg, "PulseCnt");
	if (pPulses->m_PulseCnt >= SUM_ALL_ITEM_MAX)
		pPulses->m_PulseCnt = SUM_ALL_ITEM_MAX - 1;

	for(unsigned int i = 0; i < cfg_size(cfg, "Pulse"); i++)
	{
		cfg_t* cfgPulse = cfg_getnsec(cfg, "Pulse", i);
		S_WORD sn = (S_WORD)cfg_getint(cfgPulse, "SN");
		if (sn > pPulses->m_PulseCnt)
			continue;

		S_PULSE *pInfo = &pPulses->m_Item[sn];
		pInfo->m_Enable = (cfg_getbool(cfgPulse, "Enable") == cfg_false)? false:true;
		pInfo->m_MP = (S_WORD)cfg_getint(cfgPulse, "MP");
		pInfo->m_PulseType = (S_BYTE)cfg_getint(cfgPulse, "PulseType");
		pInfo->m_PulseConst = (S_DWORD)cfg_getint(cfgPulse, "PulseConst");
	}

	cfg_free(cfg);
	return 0;
}

/** @fn C_IniSys::SavePulseInfo
 *  @brief 保存脉冲量参数
 *  @param[in] pStatus:配置结构体
 *  @param[in] name:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::SavePulseInfo(S_STATUS *pStatus, const char *fname)
{
	char Buffer[32];
	string strFileInfo;
	strFileInfo.append("#note 脉冲量配置文件\r\n");
	strFileInfo.append("#remark SN=序号 范围[1 - 253]\r\n");
	strFileInfo.append("#remark PulseType=脉冲类型 01:正向有功 02:反向有功 03:正向感性无功（I）04:正向容性无功（IV）05:反向容性无功（II）06:反向感性无功（III）\r\n");
	strFileInfo.append("#remark PulseConst=脉冲常数\r\n");

	S_PULSES *pPulses = STATUS.GetPulseInfos();

	sprintf(Buffer,"%d", pPulses->m_PulseCnt);
	strFileInfo.append("\r\nPulseCnt=").append(Buffer);

	for (S_WORD i = 1; i <= pPulses->m_PulseCnt; i++)
	{
		S_PULSE *pInfo = &pPulses->m_Item[i];

		strFileInfo.append("\r\nPulse\r\n{");
		strFileInfo.append("\r\n\tEnable=").append(C_IniBase::GetBoolKey(pInfo->m_Enable));

		sprintf(Buffer,"%d", i);
		strFileInfo.append("\r\n\tSN=").append(Buffer);
		sprintf(Buffer,"%d", pInfo->m_MP);
		strFileInfo.append("\r\n\tMP=").append(Buffer);

		sprintf(Buffer,"%d", pInfo->m_PulseType);
		strFileInfo.append("\r\n\tPulseType=").append(Buffer);
		sprintf(Buffer,"%d", pInfo->m_PulseConst);
		strFileInfo.append("\r\n\tPulseConst=").append(Buffer);
		
		strFileInfo.append("\r\n}\r\n\r\n");
	}

	return C_IniBase::SaveSetFile(strFileInfo, fname);
}


/** @brief 运算量参数配置结构体 */
cfg_opt_t OPT_SUM_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_true,CFGF_NOCASE),
	CFG_INT("SN", 1, CFGF_NONE),
	CFG_INT("MP", 1, CFGF_NONE),
	CFG_INT_LIST("SumMP", "{}", CFGF_NONE),
	CFG_INT_LIST("SumOP", "{}", CFGF_NONE),
	CFG_STR("DI", "00000000", CFGF_NONE),
	CFG_END()
};
cfg_opt_t OPT_SUMS_SETTINGS[] =
{
	CFG_INT("SumCnt", 1, CFGF_NONE),
	CFG_SEC("Sum", OPT_SUM_SETTINGS, CFGF_MULTI |CFGF_NOCASE),
	CFG_END()
};

/** @fn C_IniSys::LoadSumInfo
 *  @brief 加载运算量参数
 *  @param[out] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::LoadSumInfo(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfg  = cfg_init(OPT_SUMS_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSetFile(cfg, fname)!=0)
	{
		cfg_free(cfg);
		return -1;
	}

	S_SUMS *pSums = STATUS.GetSumInfos();
	pSums->Init();
	pSums->m_SumCnt = (S_WORD)cfg_getint(cfg, "SumCnt");
	if (pSums->m_SumCnt >= SUM_ALL_ITEM_MAX)
		pSums->m_SumCnt = SUM_ALL_ITEM_MAX - 1;

	for(unsigned int i = 0; i < cfg_size(cfg, "Sum"); i++)
	{
		cfg_t* cfgSum = cfg_getnsec(cfg, "Sum", i);
		S_WORD sn = (S_WORD)cfg_getint(cfgSum, "SN");
		if (sn > pSums->m_SumCnt)
			continue;

		S_SUM *pInfo = &pSums->m_Item[sn];
		pInfo->m_Enable = (cfg_getbool(cfgSum, "Enable") == cfg_false)? false:true;
		pInfo->m_MP = (S_WORD)cfg_getint(cfgSum, "MP");
		for (S_WORD m = 0; m < cfg_size(cfgSum, "SumMP") && m < 8; m++)
		{
			pInfo->m_SumMp[m] = (S_WORD)cfg_getnint(cfgSum, "SumMP", m);
		}
		for (S_WORD n = 0; n < cfg_size(cfgSum, "SumOP") && n < 8; n++)
		{
			int sum = cfg_getnint(cfgSum, "SumOP", n);
			switch (sum)
			{
			case 1:pInfo->m_SumOperator[n] = 0x01;break;
			case 2:pInfo->m_SumOperator[n] = 0x81;break;
			default:pInfo->m_SumOperator[n] = 0x00;break;
			}
		}
		sscanf(cfg_getstr(cfgSum, "DI"), "%X", &pInfo->m_DI);
	}

	cfg_free(cfg);
	return 0;
}

/** @fn C_IniSys::SaveSumInfo
 *  @brief 保存运算量参数
 *  @param[in] pStatus:配置结构体
 *  @param[in] name:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::SaveSumInfo(S_STATUS *pStatus, const char *fname)
{
	char Buffer[32];
	string strFileInfo;
	strFileInfo.append("#note 运算量配置文件\r\n");
	strFileInfo.append("#remark SN=序号 范围[1 - 253]\r\n");
	strFileInfo.append("#remark SumMP=参与运算测量点，最多8个\r\n");
	strFileInfo.append("#remark SumOP=运算规则 01(加)02(减)00(无效)\r\n");

	S_SUMS *pSums = STATUS.GetSumInfos();

	sprintf(Buffer,"%d", pSums->m_SumCnt);
	strFileInfo.append("\r\nSumCnt=").append(Buffer);

	for (S_WORD i = 1; i <= pSums->m_SumCnt; i++)
	{
		S_SUM *pInfo = &pSums->m_Item[i];

		strFileInfo.append("\r\nSum\r\n{");
		strFileInfo.append("\r\n\tEnable=").append(C_IniBase::GetBoolKey(pInfo->m_Enable));

		sprintf(Buffer,"%d", i);
		strFileInfo.append("\r\n\tSN=").append(Buffer);
		sprintf(Buffer,"%d", pInfo->m_MP);
		strFileInfo.append("\r\n\tMP=").append(Buffer);

		sprintf(Buffer,"%d", pInfo->m_SumMp[0]);
		strFileInfo.append("\r\n\tSumMP={").append(Buffer);
		for (int m = 1; m < 8; m++)
		{
			sprintf(Buffer,",%d", pInfo->m_SumMp[m]);
			strFileInfo.append(Buffer);
		}
		strFileInfo.append("}");

		sprintf(Buffer,"%d", ((pInfo->m_SumOperator[0]==0x81)? 0x02:pInfo->m_SumOperator[0]));
		strFileInfo.append("\r\n\tSumOP={").append(Buffer);
		for (int n = 1; n < 8; n++)
		{
			sprintf(Buffer,",%d", ((pInfo->m_SumOperator[n]==0x81)? 0x02:pInfo->m_SumOperator[n]));
			strFileInfo.append(Buffer);
		}
		strFileInfo.append("}");

		sprintf(Buffer,"%08X", pInfo->m_DI);
		strFileInfo.append("\r\n\tDI=").append(Buffer);

		strFileInfo.append("\r\n}\r\n\r\n");
	}

	return C_IniBase::SaveSetFile(strFileInfo, fname);
}


/** @brief 差动规则参数配置结构体 */
cfg_opt_t OPT_DIFFRULE_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_true,CFGF_NOCASE),
	CFG_INT("SN", 1, CFGF_NONE),
	CFG_STR("DI", "00000000", CFGF_NONE),
	CFG_INT("MP1", 1, CFGF_NONE),
	CFG_INT("MP2", 1, CFGF_NONE),
	CFG_STR("DiffMin", "0.1", CFGF_NONE),
	CFG_STR("DiffAlarmLimit", "0.1", CFGF_NONE),
	CFG_STR("DiffEndLimit", "0.1", CFGF_NONE),
	CFG_END()
};
cfg_opt_t OPT_DIFFRULES_SETTINGS[] =
{
	CFG_INT("DiffRuleCnt", 1, CFGF_NONE),
	CFG_SEC("Diff", OPT_DIFFRULE_SETTINGS, CFGF_MULTI |CFGF_NOCASE),
	CFG_END()
};
/** @fn C_IniSys::LoadDiffRuleInfo
 *  @brief 加载差动规则参数
 *  @param[out] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::LoadDiffRuleInfo(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfg  = cfg_init(OPT_DIFFRULES_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSetFile(cfg, fname)!=0)
	{
		cfg_free(cfg);
		return -1;
	}

	S_DiffRuleS *pDiffRules = STATUS.GetDiffRuleInfos();
	pDiffRules->Init();
	pDiffRules->m_DiffRuleCnt = (S_WORD)cfg_getint(cfg, "DiffRuleCnt");
	if (pDiffRules->m_DiffRuleCnt >= SUM_ALL_ITEM_MAX)
		pDiffRules->m_DiffRuleCnt = SUM_ALL_ITEM_MAX - 1;

	for(unsigned int i = 0; i < cfg_size(cfg, "Diff"); i++)
	{
		cfg_t* cfgDiffRule = cfg_getnsec(cfg, "Diff", i);
		S_WORD sn = (S_WORD)cfg_getint(cfgDiffRule, "SN");
		if (sn > pDiffRules->m_DiffRuleCnt)
			continue;

		S_DiffRule *pInfo = &pDiffRules->m_Item[sn];
		pInfo->m_Enable = (cfg_getbool(cfgDiffRule, "Enable") == cfg_false)? false:true;
		sscanf(cfg_getstr(cfgDiffRule, "DI"), "%X", &pInfo->m_DI);
		pInfo->m_MP1 = (S_WORD)cfg_getint(cfgDiffRule, "MP1");
		pInfo->m_MP2 = (S_WORD)cfg_getint(cfgDiffRule, "MP2");

		C_BCD bcdDiffMin(cfg_getstr(cfgDiffRule, "DiffMin"));
		const S_BYTE *pDiffMin = bcdDiffMin.GetBcdData();
		memcpy(&pInfo->m_DiffMin, pDiffMin+1, 3);

		C_BCD bcdDiffAlarm(cfg_getstr(cfgDiffRule, "DiffAlarmLimit"));
		const S_BYTE *pDiffAlarm = bcdDiffAlarm.GetBcdData();
		memcpy(&pInfo->m_DiffAlarmLimit, pDiffAlarm+1, 2);

		C_BCD bcdDiffEnd(cfg_getstr(cfgDiffRule, "DiffEndLimit"));
		const S_BYTE *pDiffEnd = bcdDiffEnd.GetBcdData();
		memcpy(&pInfo->m_DiffEndLimit, pDiffEnd+1, 2);
	}

	cfg_free(cfg);
	return 0;
}

/** @fn C_IniSys::SaveDiffRuleInfo
 *  @brief 保存差动规则参数
 *  @param[in] pStatus:配置结构体
 *  @param[in] name:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::SaveDiffRuleInfo(S_STATUS *pStatus, const char *fname)
{
	char Buffer[32];
	string strFileInfo;
	strFileInfo.append("#note 差动规则配置文件\r\n");
	strFileInfo.append("#remark SN=序号 范围[1 - 253]\r\n");
	strFileInfo.append("#remark DI=数据项, MP1=比较测量点号, MP2=被比较测量点号\r\n");
	strFileInfo.append("#remark DiffMin=MMMM.MM 采用差动比率报警的最小绝对差值\r\n");
	strFileInfo.append("#remark DiffAlarmLimit=RR.RR 差动比率报警阈值(%)\r\n");
	strFileInfo.append("#remark DiffEndLimit=SS.SS 差动比率报警恢复阈值(%)\r\n");

	S_DiffRuleS *pDiffRules = STATUS.GetDiffRuleInfos();

	sprintf(Buffer,"%d", pDiffRules->m_DiffRuleCnt);
	strFileInfo.append("\r\nDiffRuleCnt=").append(Buffer);

	for (S_WORD i = 1; i <= pDiffRules->m_DiffRuleCnt; i++)
	{
		S_DiffRule *pInfo = &pDiffRules->m_Item[i];

		strFileInfo.append("\r\nDiff\r\n{");
		strFileInfo.append("\r\n\tEnable=").append(C_IniBase::GetBoolKey(pInfo->m_Enable));

		sprintf(Buffer,"%d", i);
		strFileInfo.append("\r\n\tSN=").append(Buffer);
		sprintf(Buffer,"%08X", pInfo->m_DI);
		strFileInfo.append("\r\n\tDI=").append(Buffer);
		sprintf(Buffer,"%d", pInfo->m_MP1);
		strFileInfo.append("\r\n\tMP1=").append(Buffer);
		sprintf(Buffer,"%d", pInfo->m_MP2);
		strFileInfo.append("\r\n\tMP2=").append(Buffer);

		C_BCD bcdDiffMin((S_BYTE*)&pInfo->m_DiffMin, 4, 2);
		string strDiffMin = bcdDiffMin;
		strFileInfo.append("\r\n\tDiffMin=").append(strDiffMin);

		C_BCD bcdDiffAlarm((S_BYTE*)&pInfo->m_DiffAlarmLimit, 2, 2);
		string strDiffAlarm = bcdDiffAlarm;
		strFileInfo.append("\r\n\tDiffAlarmLimit=").append(strDiffAlarm);

		C_BCD bcdDiffEnd((S_BYTE*)&pInfo->m_DiffEndLimit, 2, 2);
		string strDiffEnd = bcdDiffEnd;
		strFileInfo.append("\r\n\tDiffEndLimit=").append(strDiffEnd);
		
		strFileInfo.append("\r\n}\r\n\r\n");
	}

	return C_IniBase::SaveSetFile(strFileInfo, fname);
}

static S_CFGITEM sTermTypeMap[] =
{
	{CONS_TERM_TYPE_FK, "fk"},
	{CONS_TERM_TYPE_PB, "pb"},
	{CONS_TERM_TYPE_JC, "jc"},
	{CONS_TERM_TYPE_CZ, "eac"}
};
S_BYTE C_IniSys::GetTermType(const char *pTermTypeName)
{
	int type = (S_BYTE)C_IniBase::MakeValue(sTermTypeMap, sizeof(sTermTypeMap)/sizeof(sTermTypeMap[0]), pTermTypeName);
	if (type < 0)
		return CONS_TERM_TYPE_JC;
	return (S_BYTE)type;
}

const char *C_IniSys::GetTermTypeName(S_BYTE TermType)
{
	return C_IniBase::MakeKey(sTermTypeMap, sizeof(sTermTypeMap)/sizeof(sTermTypeMap[0]), TermType);
}

static S_CFGITEM sMsdiTypeMap[] =
{
	{MSDI_TYPE_NORMAL, "Normal"},
	{MSDI_TYPE_TARIFF, "Tariff"},
	{MSDI_TYPE_STATUS, "Status"},
	{MSDI_TYPE_HARMONIOUS, "Harmonious"},
	{MSDI_TYPE_CLASS1_HOUR, "Class1Hour"}
};
static S_CFGITEM sMsdiFreezeMap[] =
{
	{MSDI_FREEZE_HOUR, "HOUR"},
	{MSDI_FREEZE_DAY, "DAY"},
	{MSDI_FREEZE_MONTH, "MONTH"}
};
S_BYTE C_IniSys::GetMsdiTypeID(const char *pTypeName)
{
	return (S_BYTE)C_IniBase::MakeValue(sMsdiTypeMap, sizeof(sMsdiTypeMap)/sizeof(sMsdiTypeMap[0]), pTypeName);
}
S_BYTE C_IniSys::GetMsdiFreezeType(const char *pTypeName)
{
	return (S_BYTE)C_IniBase::MakeValue(sMsdiFreezeMap, sizeof(sMsdiFreezeMap)/sizeof(sMsdiFreezeMap[0]), pTypeName);
}


