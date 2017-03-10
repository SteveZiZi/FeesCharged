/** @file
 *  @brief 测量点配置加载和保存
 *  @author 
 *  @date 2011/12/06
 *  @version 0.1
 *  定义系统测量点配置加载和保存函数
 */
#include "ini_mp.h"
#include "osrelate.h"
#include "busialgo.h"
#include "ipcSharemem.h"

/** @brief 电表参数配置结构体 */
cfg_opt_t OPT_METER_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_false, CFGF_NOCASE),
	CFG_INT("MeasureNo", 1, CFGF_NOCASE),	
	CFG_STR("PortSet.No", "VCom1", CFGF_NOCASE),
	CFG_INT("PortSet.Baud", 1200, CFGF_NOCASE),
	CFG_INT("PortSet.DataBit", 8, CFGF_NOCASE),
	CFG_INT("PortSet.StopBit", 1, CFGF_NOCASE),
	CFG_INT("PortSet.Parity", 2, CFGF_NOCASE),
	CFG_INT("TariffCnt", 1, CFGF_NOCASE),
	
	CFG_INT("WireType", 1, CFGF_NOCASE),
	CFG_STR("RatedVoltage", "2200", CFGF_NOCASE),
	CFG_STR("RatedCur", "0000", CFGF_NOCASE),
	CFG_STR("RatedContent", "0000", CFGF_NOCASE),
	CFG_STR("VTContent", "0000", CFGF_NOCASE),
	
	CFG_INT("CT", 1, CFGF_NOCASE),
	CFG_INT("PT", 1, CFGF_NOCASE),
	CFG_STR("MeterRule", "GB645_1997", CFGF_NOCASE),	
	CFG_STR("FunType", "SinglePhase", CFGF_NOCASE),
	CFG_BOOL("MainFlag", cfg_false, CFGF_NOCASE),
	CFG_BOOL("VipFlag", cfg_false, CFGF_NOCASE),
	CFG_BOOL("PowerCut", cfg_false, CFGF_NOCASE),
	CFG_STR("MeterAddr", "000000000000", CFGF_NOCASE),
	CFG_STR("RelayAddr", "000000000000", CFGF_NOCASE),
	CFG_STR("Password", "000000000000", CFGF_NOCASE),
	CFG_STR("UserName", "00000000", CFGF_NOCASE),
	CFG_END()
};
cfg_opt_t OPT_METERS_SETTINGS[] =
{
	CFG_SEC("Meter", OPT_METER_SETTINGS, CFGF_MULTI |CFGF_NOCASE),
	CFG_END()
};

/** @fn C_IniMP::LoadMeterInformation
 *  @brief 加载电表参数
 *  @param[out] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniMP::LoadMeterInformation(S_STATUS *pStatus, const char *fname)
{
	pStatus->m_MeterInfos.Init();

	cfg_t* cfg = cfg_init(OPT_METERS_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSetFile(cfg, fname) != 0)
	{
		cfg_free(cfg);
		return -1;
	}

	for (unsigned int i = 0; i < cfg_size(cfg, "Meter"); i++)
	{
		cfg_t* cfgMeter = cfg_getnsec(cfg, "Meter", i);
		S_WORD Type = MP_TYPE_METER;
		S_WORD MP = (S_WORD)cfg_getint(cfgMeter, "MeasureNo");
		S_BYTE RuleID = GetMeterRuleID(cfg_getstr(cfgMeter, "MeterRule"));
		if (RuleID == METER_RULE_AC_GATHER)
			Type = MP_TYPE_AC;
		if (pStatus->m_MpInfos.Test(MP, Type, MP) == false)
		{
			loget<<"表计["<<MP<<"]的测量点["<<MP<<"]被占用!!!"<<endl;
			continue;
		}
		pStatus->m_MpInfos.Set(MP, Type, MP);
		S_MeterInfo &meter = pStatus->m_MeterInfos.m_Item[MP];
		SetMeterInfo(meter, cfgMeter);
	}
	
	cfg_free(cfg);
	return 0;
}

/** @fn C_IniMP::SaveMeterInformation
 *  @brief 保存电表通讯参数
 *  @param[in] pStatus:配置结构体
 *  @param[in] name:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniMP::SaveMeterInformation(S_STATUS *pStatus, const char *fname)
{
	char buffer[64];	
	string strFileInfo;

	strFileInfo.append("#参数配置简述\r\n");
	strFileInfo.append("#Enable:测量点的状态 false:无效;true:有效\r\n");
	strFileInfo.append("#RuleID:通信协议类型 GB645_1997;GB645_2007;GD_97;GD_07;WEISHENG;LGB;LGD;EMDI;ABB;ACTARIS;EMAIL;ISKRA;AC\r\n");
	strFileInfo.append("#FunType:电能表类型 SinglePhase:单相电子表;MultiFun:多功能表\r\n");
	strFileInfo.append("#MainFlag:总分类型 false:用户表;true:总表\r\n");
	strFileInfo.append("#VipFlag:重点用户属性 false:普通用户;true:重点用户\r\n");
	strFileInfo.append("#PowerCut:拉闸功能 false:不带拉闸;true:带拉闸\r\n\r\n");

	S_MeterInfos &MeterInfos = pStatus->m_MeterInfos;
	for(size_t i = 0; i < CONS_METER_COUNT_MAX; i++)
	{
		S_MeterInfo &MeterInfo = MeterInfos.m_Item[i];
		if (MeterInfo.Existed() == false)
			continue;

		strFileInfo.append("Meter\r\n{");
		strFileInfo.append("\r\n\tEnable=").append(C_IniBase::GetBoolKey(MeterInfo.m_Enable));
		sprintf(buffer,"%d",MeterInfo.m_MeasureNo);
		strFileInfo.append("\r\n\tMeasureNo=").append(buffer);

		sprintf(buffer,"VCom%d",MeterInfo.m_PortSet.m_PortNO);
		strFileInfo.append("\r\n\tPortSet.No=").append(buffer);
		sprintf(buffer,"%d",MeterInfo.m_PortSet.m_Baud);
		strFileInfo.append("\r\n\tPortSet.Baud=").append(buffer);
		sprintf(buffer,"%d",MeterInfo.m_PortSet.m_DataBit);
		strFileInfo.append("\r\n\tPortSet.DataBit=").append(buffer);
		sprintf(buffer,"%d",MeterInfo.m_PortSet.m_StopBit);
		strFileInfo.append("\r\n\tPortSet.StopBit=").append(buffer);
		sprintf(buffer,"%d",MeterInfo.m_PortSet.m_Parity);
		strFileInfo.append("\r\n\tPortSet.Parity=").append(buffer);

		strFileInfo.append("\r\n\tMeterRule=").append(GetMeterRuleName(MeterInfo.m_RuleID));
		strFileInfo.append("\r\n\tFunType=").append(GetMeterFunTypeName(MeterInfo.m_FunType));
		strFileInfo.append("\r\n\tMainFlag=").append(C_IniBase::GetBoolKey(MeterInfo.m_MainFlag));
		strFileInfo.append("\r\n\tVipFlag=").append(C_IniBase::GetBoolKey(MeterInfo.m_VipFlag));
		strFileInfo.append("\r\n\tPowerCut=").append(C_IniBase::GetBoolKey(MeterInfo.m_PowerCut));

		sprintf(buffer,"%d",MeterInfo.m_WireType);
		strFileInfo.append("\r\n\tWireType=").append(buffer);
		sprintf(buffer,"%04X",MeterInfo.m_RatedVoltage);
		strFileInfo.append("\r\n\tRatedVoltage=").append(buffer);
		sprintf(buffer,"%06X",MeterInfo.m_RatedCur);
		strFileInfo.append("\r\n\tRatedCur=").append(buffer);
		sprintf(buffer,"%04X",MeterInfo.m_RatedContent);
		strFileInfo.append("\r\n\tRatedContent=").append(buffer);
		sprintf(buffer,"%04X",MeterInfo.m_VTContent);
		strFileInfo.append("\r\n\tVTContent=").append(buffer);

		sprintf(buffer,"%d",MeterInfo.m_TariffCnt);
		strFileInfo.append("\r\n\tTariffCnt=").append(buffer);
		sprintf(buffer,"%d",MeterInfo.m_CT);
		strFileInfo.append("\r\n\tCT=").append(buffer);
		sprintf(buffer,"%d",MeterInfo.m_PT);
		strFileInfo.append("\r\n\tPT=").append(buffer);

		memset(buffer, 0x00, sizeof(buffer));
		strFileInfo.append("\r\n\tMeterAddr=\"").append(MeterInfo.GetMeterAddress(buffer)).append("\"");
		memset(buffer, 0x00, sizeof(buffer));
		strFileInfo.append("\r\n\tRelayAddr=\"").append(MeterInfo.GetRelayAddress(buffer)).append("\"");
		memset(buffer, 0x00, sizeof(buffer));
		strFileInfo.append("\r\n\tPassword=\"").append(MeterInfo.GetMeterPassword(buffer)).append("\"");
		strFileInfo.append("\r\n\tUserName=\"").append(MeterInfo.m_UserName).append("\"");

		strFileInfo.append("\r\n}\r\n\r\n");
	}

	return C_IniBase::SaveSetFile(strFileInfo, fname);
}

int C_IniMP::SetMeterInfo(S_MeterInfo &Meter, cfg_t* cfgMeter)
{
	Meter.m_Enable = (cfg_getbool(cfgMeter, "Enable") == cfg_true) ? true : false; ///< 测量点的状态 0：无效；1：有效
	Meter.m_MeasureNo = (S_WORD)cfg_getint(cfgMeter, "MeasureNo"); ///< 所属测量点号
	Meter.m_RuleID = GetMeterRuleID(cfg_getstr(cfgMeter, "MeterRule")); ///< 通信协议类型 00H：DL/T645-1997规约；01H：DL/T645-2007规约；02H：其他规约
	Meter.m_FunType = GetMeterFunType(cfg_getstr(cfgMeter, "FunType")); ///< 电能表类型：01，单相电子表；02，多功能表
	Meter.m_MainFlag = (cfg_getbool(cfgMeter, "MainFlag") == cfg_true) ? true : false; ///< 总分类型：0，用户表；1，总表
	Meter.m_VipFlag = (cfg_getbool(cfgMeter, "VipFlag") == cfg_true) ? true : false; ///< 重点用户属性：0，普通用户；1，重点用户
	Meter.m_PowerCut = (cfg_getbool(cfgMeter, "PowerCut") == cfg_true) ? true : false; ///< 拉闸功能：0，不带拉闸；1，带拉闸；

	Meter.m_WireType = (S_BYTE)cfg_getint(cfgMeter, "WireType");//接线方式
	Meter.m_RatedVoltage = (S_WORD)C_IniBase::GetHex_WORD(cfg_getstr(cfgMeter, "RatedVoltage"));//额定电压
	Meter.m_RatedCur = (S_DWORD)C_IniBase::GetHex_DWORD(cfg_getstr(cfgMeter, "RatedCur"));//额定电流
	Meter.m_RatedContent = (S_WORD)C_IniBase::GetHex_WORD(cfg_getstr(cfgMeter, "RatedContent"));//报装容量
	Meter.m_VTContent = (S_WORD)C_IniBase::GetHex_WORD(cfg_getstr(cfgMeter, "VTContent"));//变压器容量
	
	Meter.m_TariffCnt = (INT8U)cfg_getint(cfgMeter, "TariffCnt"); ///< 电能费率个数
	Meter.m_CT = (INT16U)cfg_getint(cfgMeter, "CT"); ///< CT变化
	Meter.m_PT = (INT16U)cfg_getint(cfgMeter, "PT"); ///< PT变化

	int VPort = 0;
	sscanf(cfg_getstr(cfgMeter, "PortSet.No"), "VCom%d", &VPort);
	Meter.m_PortSet.m_PortNO = (INT8U)VPort; ///< 端口号测量点端口号：00H~1EH表示485表端口号；1FH表示载波通道；20H表示无线；30H表示其他。
	Meter.m_PortSet.m_Baud  = cfg_getint(cfgMeter, "PortSet.Baud");
	Meter.m_PortSet.m_DataBit = (INT8S)cfg_getint(cfgMeter, "PortSet.DataBit");
	Meter.m_PortSet.m_StopBit = (INT8S)cfg_getint(cfgMeter, "PortSet.StopBit");
	Meter.m_PortSet.m_Parity  = (INT8S)cfg_getint(cfgMeter, "PortSet.Parity");

	Meter.SetMeterAddress(cfg_getstr(cfgMeter, "MeterAddr"));
	Meter.SetRelayAddress(cfg_getstr(cfgMeter, "RelayAddr")); ///< 测量点对应采集终端地址：全为00H或全为FFH则表示不带采集器
	Meter.SetMeterPassword(cfg_getstr(cfgMeter, "Password"));
	memset(Meter.m_UserName, 0x00, sizeof(Meter.m_UserName));
	strncpy(Meter.m_UserName, cfg_getstr(cfgMeter, "UserName"), sizeof(Meter.m_UserName) - 1);

	/* QT 共享内存档案参数*/
	char meterAddr[6];	
	struct SIpcUserParm meterParam;

	meterParam.m_MeterNo = Meter.m_MeasureNo;
	memcpy(meterParam.m_MeterSerialNo,Meter.m_RelayAddr,sizeof(Meter.m_RelayAddr));
	
	IPCSHAREMEM.m_IpcTerminalInfo.SetUserParm(Meter.m_MeasureNo,meterParam);
	IPCSHAREMEM.Update();
	return 0;
}

INT8U C_IniMP::GetMeterProperty(char *pPropertyName)
{
	if (strcasecmp(pPropertyName, "RS485") == 0)
		return MP_PROPERTY_RS485;
	else if (strcasecmp(pPropertyName, "Analog") == 0)
		return MP_PROPERTY_ANALOG;
	else if (strcasecmp(pPropertyName, "Pulse") == 0)
		return MP_PROPERTY_PULSE;
	else if (strcasecmp(pPropertyName, "Sum") == 0)
		return MP_PROPERTY_SUM;
	else if (strcasecmp(pPropertyName, "AC") == 0)
		return MP_PROPERTY_AC;
	return MP_PROPERTY_UNKNOW;
}
const char *C_IniMP::GetMeterPropertyName(INT8U Property)
{
	switch (Property)
	{
	case MP_PROPERTY_RS485:///< 485表
		return "RS485";
	case MP_PROPERTY_ANALOG:///< 模拟量
		return "Analog";
	case MP_PROPERTY_PULSE:///< 脉冲量
		return "Pulse";
	case MP_PROPERTY_SUM:///< 计算值
		return "Sum";
	case MP_PROPERTY_AC: ///< 交流采样
		return "AC";
	}
	return "unknow";
}

const S_CFGITEM MeterRuleMapTab[] = 
{
	{METER_RULE_GB645_1997, "GB645_1997"},
	{METER_RULE_GB645_2007, "GB645_2007"},
	{METER_RULE_GUANGDONG_97, "GD_97"},
	{METER_RULE_GUANGDONG_07, "GD_07"},
	{METER_RULE_WEISHENG, "WEISHENG"},
	{METER_RULE_LGR_B, "LGB"},
	{METER_RULE_LGR_D, "LGD"},
	{METER_RULE_EMDI, "EMDI"},
	{METER_RULE_ABB, "ABB"},
	{METER_RULE_ACTARIS, "ACTARIS"},
	{METER_RULE_EMAIL, "EMAIL"},
	{METER_RULE_ISKRA, "ISKRA"},
	{METER_RULE_AC_GATHER, "AC"}
};

INT8U C_IniMP::GetMeterRuleID(char *pRuleName)
{
	return (INT8U)C_IniBase::MakeValue(MeterRuleMapTab, sizeof(MeterRuleMapTab)/sizeof(MeterRuleMapTab[0]), pRuleName);
}
const char *C_IniMP::GetMeterRuleName(INT8U RuleID)
{
	return C_IniBase::MakeKey(MeterRuleMapTab, sizeof(MeterRuleMapTab)/sizeof(MeterRuleMapTab[0]), RuleID);
}
INT8U C_IniMP::GetMeterFunType(char *pTypeName)
{
	if (strcasecmp(pTypeName, "SinglePhase") == 0)
		return METER_FUN_SINGLE_PHASE;
	else if (strcasecmp(pTypeName, "MultiFun") == 0)
		return METER_FUN_MULTI_FUN;
	return METER_FUN_UNKNOW;
}
const char *C_IniMP::GetMeterFunTypeName(INT8U type)
{
	switch (type)
	{
	case METER_FUN_SINGLE_PHASE:
		return "SinglePhase";
	case METER_FUN_MULTI_FUN:
		return "MultiFun";
	}
	return "unknow";
}

/** @brief 测量点限值参数配置结构 */
cfg_opt_t OPT_MPLIMIT_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_false,CFGF_NOCASE),
	CFG_INT("MPoint",0, CFGF_NOCASE),
	CFG_STR("IUBRatio","50.00", CFGF_NOCASE),
	CFG_STR("IUBResumeRatio", "40.00", CFGF_NOCASE),
	CFG_STR("Uhgsx", "1.07", CFGF_NOCASE),
	CFG_STR("Uhgxx", "0.93", CFGF_NOCASE),
	CFG_STR("IoverRatio", "1.3", CFGF_NOCASE),
	CFG_STR("FhgzRatio","1.2", CFGF_NOCASE),
	CFG_STR("FhhfRatio", "0.95", CFGF_NOCASE),
	CFG_STR("IZeroRatio", "25.0", CFGF_NOCASE),
	CFG_STR("IZeroRRatio", "15.0", CFGF_NOCASE),
	CFG_STR("IresumeRatio", "1.0", CFGF_NOCASE),
	CFG_STR("UUBRatio","0.0", CFGF_NOCASE),
	CFG_STR("UUBResumeRatio", "0.00", CFGF_NOCASE),
	CFG_STR("UOpenRatio", "78.0", CFGF_NOCASE),
	CFG_STR("UReturnRatio", "85.0", CFGF_NOCASE),
	CFG_STR("IOpenRatio", "0.5", CFGF_NOCASE),
	CFG_STR("IReturnRatio","5.0", CFGF_NOCASE),
	CFG_STR("UjbRatio", "5.0", CFGF_NOCASE),
	CFG_STR("IjbRatio", "5.0", CFGF_NOCASE),
	CFG_STR("UjbResumeRatio", "3.0", CFGF_NOCASE),
	CFG_STR("IjbResumeRatio", "3.0", CFGF_NOCASE),
	CFG_INT("PFSectionNum", 0, CFGF_NOCASE),
	CFG_STR_LIST("PowerFactorVal","{}", CFGF_NOCASE),
	CFG_INT("HarmonicNum", 21, CFGF_NOCASE),
	CFG_STR("UbcsxRatio", "0.0", CFGF_NOCASE),
	CFG_STR("UbcxxRatio", "0.0", CFGF_NOCASE),
	CFG_STR("FrebcsxRatio", "0.0", CFGF_NOCASE),
	CFG_STR("FrebcxxRatio", "0.0", CFGF_NOCASE),
	CFG_STR("ULTwinkleVal", "0.0", CFGF_NOCASE),
	CFG_STR("USTwinkleVal", "0.0", CFGF_NOCASE),
	CFG_STR("WGCompensate", "0.8", CFGF_NOCASE),
	CFG_STR("WGrCompensate", "0.9", CFGF_NOCASE),
	CFG_END()
};

cfg_opt_t OPT_MPLIMITS_SETTINGS[] =
{
	CFG_SEC("MPlimit", OPT_MPLIMIT_SETTINGS, CFGF_MULTI | CFGF_NOCASE),
	CFG_END()
};

/** @fn C_IniMPLimit::LoadMPointLimit
 *  @brief 测量点限值参数文件保存
 *  @param[in] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniMPLimit::LoadMPointLimit(S_STATUS *pStatus, const char* fname)
{
	cfg_t* cfg = cfg_init(OPT_MPLIMITS_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSetFile(cfg, fname) != 0)
	{
		cfg_free(cfg);
		return -1;
	}
	for(unsigned int i = 0; i < cfg_size(cfg, "MPlimit"); i++)
	{
		cfg_t* cfgMPlimit = cfg_getnsec(cfg, "MPlimit", i);
		if (cfg_getbool(cfgMPlimit, "Enable") == cfg_false)
			continue;

		INT16U MP = (INT16U)cfg_getint(cfgMPlimit, "MPoint");
		if (MP >= CONS_METER_COUNT_MAX)
			continue;

		S_MPointLimit& MPlimit = pStatus->m_MPointLimits.Item[MP];
		MPlimit.m_Enable = true;
		C_BusiAlgo::ChangeAscii2Bcd(cfg_getstr(cfgMPlimit, "IUBRatio"),(S_BYTE *)&MPlimit.m_IUBRatio,2,2);
		C_BusiAlgo::ChangeAscii2Bcd(cfg_getstr(cfgMPlimit, "IUBResumeRatio"),(S_BYTE *)&MPlimit.m_IUBResumeRatio,2,2);
		C_BusiAlgo::ChangeAscii2Bcd(cfg_getstr(cfgMPlimit, "Uhgsx"),(S_BYTE *)&MPlimit.m_Uhgsx,2,2);
		C_BusiAlgo::ChangeAscii2Bcd(cfg_getstr(cfgMPlimit, "Uhgxx"),(S_BYTE *)&MPlimit.m_Uhgxx,2,2);
		C_BusiAlgo::ChangeAscii2Bcd(cfg_getstr(cfgMPlimit, "IoverRatio"),(S_BYTE *)&MPlimit.m_IoverRatio,2,2);
		C_BusiAlgo::ChangeAscii2Bcd(cfg_getstr(cfgMPlimit, "FhgzRatio"),(S_BYTE *)&MPlimit.m_FhgzRatio,2,2);
		C_BusiAlgo::ChangeAscii2Bcd(cfg_getstr(cfgMPlimit, "FhhfRatio"),(S_BYTE *)&MPlimit.m_FhhfRatio,2,2);
		C_BusiAlgo::ChangeAscii2Bcd(cfg_getstr(cfgMPlimit, "IZeroRatio"),(S_BYTE *)&MPlimit.m_IZeroRatio,2,2);
		C_BusiAlgo::ChangeAscii2Bcd(cfg_getstr(cfgMPlimit, "IZeroRRatio"),(S_BYTE *)&MPlimit.m_IZeroRRatio,2,2);
		C_BusiAlgo::ChangeAscii2Bcd(cfg_getstr(cfgMPlimit, "IresumeRatio"),(S_BYTE *)&MPlimit.m_IresumeRatio,2,2);
		C_BusiAlgo::ChangeAscii2Bcd(cfg_getstr(cfgMPlimit, "UUBRatio"),(S_BYTE *)&MPlimit.m_UUBRatio,2,2);
		C_BusiAlgo::ChangeAscii2Bcd(cfg_getstr(cfgMPlimit, "UUBResumeRatio"),(S_BYTE *)&MPlimit.m_UUBResumeRatio,2,2);
		C_BusiAlgo::ChangeAscii2Bcd(cfg_getstr(cfgMPlimit, "UOpenRatio"),(S_BYTE *)&MPlimit.m_UOpenRatio,2,2);
		C_BusiAlgo::ChangeAscii2Bcd(cfg_getstr(cfgMPlimit, "UReturnRatio"),(S_BYTE *)&MPlimit.m_UReturnRatio,2,2);
		C_BusiAlgo::ChangeAscii2Bcd(cfg_getstr(cfgMPlimit, "IOpenRatio"),(S_BYTE *)&MPlimit.m_IOpenRatio,2,2);
		C_BusiAlgo::ChangeAscii2Bcd(cfg_getstr(cfgMPlimit, "IReturnRatio"),(S_BYTE *)&MPlimit.m_IReturnRatio,2,2);
		C_BusiAlgo::ChangeAscii2Bcd(cfg_getstr(cfgMPlimit, "UjbRatio"),(S_BYTE *)&MPlimit.m_UjbRatio,2,2);
		C_BusiAlgo::ChangeAscii2Bcd(cfg_getstr(cfgMPlimit, "IjbRatio"),(S_BYTE *)&MPlimit.m_IjbRatio,2,2);
		C_BusiAlgo::ChangeAscii2Bcd(cfg_getstr(cfgMPlimit, "UjbResumeRatio"),(S_BYTE *)&MPlimit.m_UjbResumeRatio,2,2);
		C_BusiAlgo::ChangeAscii2Bcd(cfg_getstr(cfgMPlimit, "IjbResumeRatio"),(S_BYTE *)&MPlimit.m_IjbResumeRatio,2,2);
		
		MPlimit.m_PFSectionNum = (S_BYTE)cfg_getint(cfgMPlimit, "PFSectionNum");
		for(int tempi=0;tempi<MPlimit.m_PFSectionNum;tempi++)
		{
			C_BusiAlgo::ChangeAscii2Bcd(cfg_getnstr(cfgMPlimit, "PowerFactorVal",tempi),(S_BYTE *)&MPlimit.m_PowerFactorVal[tempi],2,2);
		}
		MPlimit.m_HarmonicNum = (S_BYTE)cfg_getint(cfgMPlimit, "HarmonicNum");

		C_BusiAlgo::ChangeAscii2Bcd(cfg_getstr(cfgMPlimit, "UbcsxRatio"),(S_BYTE *)&MPlimit.m_UbcsxRatio,2,2);
		C_BusiAlgo::ChangeAscii2Bcd(cfg_getstr(cfgMPlimit, "UbcxxRatio"),(S_BYTE *)&MPlimit.m_UbcxxRatio,2,2);
		C_BusiAlgo::ChangeAscii2Bcd(cfg_getstr(cfgMPlimit, "FrebcsxRatio"),(S_BYTE *)&MPlimit.m_FrebcsxRatio,2,2);
		C_BusiAlgo::ChangeAscii2Bcd(cfg_getstr(cfgMPlimit, "FrebcxxRatio"),(S_BYTE *)&MPlimit.m_FrebcxxRatio,2,2);
		C_BusiAlgo::ChangeAscii2Bcd(cfg_getstr(cfgMPlimit, "ULTwinkleVal"),(S_BYTE *)&MPlimit.m_ULTwinkleVal,2,2);
		C_BusiAlgo::ChangeAscii2Bcd(cfg_getstr(cfgMPlimit, "USTwinkleVal"),(S_BYTE *)&MPlimit.m_USTwinkleVal,2,2);
		C_BusiAlgo::ChangeAscii2Bcd(cfg_getstr(cfgMPlimit, "WGCompensate"),(S_BYTE *)&MPlimit.m_WGCompensate,2,2);
		C_BusiAlgo::ChangeAscii2Bcd(cfg_getstr(cfgMPlimit, "WGrCompensate"),(S_BYTE *)&MPlimit.m_WGrCompensate,2,2);
	}
	cfg_free(cfg);
	return 0;
}

/** @fn C_IniMPLimit::SaveMPointLimit
 *  @brief 测量点限值参数文件保存
 *  @param[in] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniMPLimit::SaveMPointLimit(S_STATUS *pStatus, const char* fname)
{
	char Buffer[20];
	string strFileInfo;
	strFileInfo.reserve(sizeof(S_MPLS)+128);
	strFileInfo.append("#note:电能质量监测参数\r\n");

	S_MPLS& MPlimits = pStatus->m_MPointLimits;
	for(INT16U i = 0; i < sizeof(MPlimits.Item)/sizeof(MPlimits.Item[0]); i++)
	{
		if(MPlimits.Item[i].m_Enable == false)
			continue;

		S_MPointLimit &MPlimit = MPlimits.Item[i];

		strFileInfo.append("MPlimit\r\n{\r\n");
		strFileInfo.append("\tEnable = true\r\n");
		sprintf(Buffer, "%d", i);
		strFileInfo.append("\tMPoint = ").append(Buffer).append("\r\n");

		strFileInfo.append("\tIUBRatio=").append(C_BusiAlgo::ChangeBcd2Ascii((S_BYTE *)&MPlimit.m_IUBRatio,2,2)).append("#电流不平衡度阈值(单位%)\r\n");
		strFileInfo.append("\tIUBResumeRatio=").append(C_BusiAlgo::ChangeBcd2Ascii((S_BYTE *)&MPlimit.m_IUBResumeRatio,2,2)).append("#电流不平衡度恢复阈值(单位%)\r\n");
		strFileInfo.append("\tUhgsx=").append(C_BusiAlgo::ChangeBcd2Ascii((S_BYTE *)&MPlimit.m_Uhgsx,2,2)).append("#合格电压上限 缺省为1.07\r\n");
		strFileInfo.append("\tUhgxx=").append(C_BusiAlgo::ChangeBcd2Ascii((S_BYTE *)&MPlimit.m_Uhgxx,2,2)).append("#合格电压下限 缺省为0.93\r\n");
		strFileInfo.append("\tIoverRatio=").append(C_BusiAlgo::ChangeBcd2Ascii((S_BYTE *)&MPlimit.m_IoverRatio,2,2)).append("#判断电流过流的相对额定值的比例，缺省为1.3\r\n");
		strFileInfo.append("\tFhgzRatio=").append(C_BusiAlgo::ChangeBcd2Ascii((S_BYTE *)&MPlimit.m_FhgzRatio,2,2)).append("#判断负荷过载相对额定值的比例，缺省为1.2\r\n");
		strFileInfo.append("\tFhhfRatio=").append(C_BusiAlgo::ChangeBcd2Ascii((S_BYTE *)&MPlimit.m_FhhfRatio,2,2)).append("#判断负荷过载恢复的相对额定值的比例，缺省为0.95\r\n");
		strFileInfo.append("\tIZeroRatio=").append(C_BusiAlgo::ChangeBcd2Ascii((S_BYTE *)&MPlimit.m_IZeroRatio,2,2)).append("#零相电流报警阈值(单位%)，缺省值额定电流的25%\r\n");
		strFileInfo.append("\tIZeroRRatio=").append(C_BusiAlgo::ChangeBcd2Ascii((S_BYTE *)&MPlimit.m_IZeroRRatio,2,2)).append("#零相电流报警恢复阈值(单位%)，缺省值额定电流的15%\r\n");
		strFileInfo.append("\tIresumeRatio=").append(C_BusiAlgo::ChangeBcd2Ascii((S_BYTE *)&MPlimit.m_IresumeRatio,2,2)).append("#判断电流过负荷恢复的相对额定值的比例，缺省值为1.0\r\n");
		strFileInfo.append("\tUUBRatio=").append(C_BusiAlgo::ChangeBcd2Ascii((S_BYTE *)&MPlimit.m_UUBRatio,2,2)).append("#电压不平衡度阈值(单位%)\r\n");
		strFileInfo.append("\tUUBResumeRatio=").append(C_BusiAlgo::ChangeBcd2Ascii((S_BYTE *)&MPlimit.m_UUBResumeRatio,2,2)).append("#电压不平衡度恢复阈值(单位%)\r\n");
		strFileInfo.append("\tUOpenRatio=").append(C_BusiAlgo::ChangeBcd2Ascii((S_BYTE *)&MPlimit.m_UOpenRatio,2,2)).append("#启动电压缺省为额定电压78%\r\n");
		strFileInfo.append("\tUReturnRatio=").append(C_BusiAlgo::ChangeBcd2Ascii((S_BYTE *)&MPlimit.m_UReturnRatio,2,2)).append("#返回电压缺省为额定电压85%\r\n");
		strFileInfo.append("\tIOpenRatio=").append(C_BusiAlgo::ChangeBcd2Ascii((S_BYTE *)&MPlimit.m_IOpenRatio,2,2)).append("#启动电流缺省为额定电流0.5%\r\n");
		strFileInfo.append("\tIReturnRatio=").append(C_BusiAlgo::ChangeBcd2Ascii((S_BYTE *)&MPlimit.m_IReturnRatio,2,2)).append("#返回电流缺省为额定电流5%\r\n");
		strFileInfo.append("\tUjbRatio=").append(C_BusiAlgo::ChangeBcd2Ascii((S_BYTE *)&MPlimit.m_UjbRatio,2,2)).append("#电压畸变率阈值，缺省值5%\r\n");
		strFileInfo.append("\tIjbRatio=").append(C_BusiAlgo::ChangeBcd2Ascii((S_BYTE *)&MPlimit.m_IjbRatio,2,2)).append("#电流畸变率阈值，缺省值5%\r\n");
		strFileInfo.append("\tUjbResumeRatio=").append(C_BusiAlgo::ChangeBcd2Ascii((S_BYTE *)&MPlimit.m_UjbResumeRatio,2,2)).append("#电压畸变恢复阈值，缺省值3%\r\n");
		strFileInfo.append("\tIjbResumeRatio=").append(C_BusiAlgo::ChangeBcd2Ascii((S_BYTE *)&MPlimit.m_IjbResumeRatio,2,2)).append("#电流畸变恢复阈值，缺省值3%\r\n");

		sprintf(Buffer, "%d", MPlimit.m_PFSectionNum);
		strFileInfo.append("\tPFSectionNum=").append(Buffer).append("#功率因数区段数量\r\n");
		strFileInfo.append("\tPowerFactorVal={");
		for(int tempi=0;tempi<MPlimit.m_PFSectionNum;tempi++)
		{
			strFileInfo.append(C_BusiAlgo::ChangeBcd2Ascii((S_BYTE *)&MPlimit.m_PowerFactorVal[tempi],2,2));
			if(tempi < MPlimit.m_PFSectionNum-1)
				strFileInfo.append(",");
		}
		strFileInfo.append("}#功率因数区段定值\r\n");
		sprintf(Buffer, "%d", MPlimit.m_HarmonicNum);
		strFileInfo.append("\tHarmonicNum=").append(Buffer).append("#监测谐波次数\r\n");

		strFileInfo.append("\tUbcsxRatio=").append(C_BusiAlgo::ChangeBcd2Ascii((S_BYTE *)&MPlimit.m_UbcsxRatio,2,2)).append("#电压偏差上限(单位%)\r\n");
		strFileInfo.append("\tUbcxxRatio=").append(C_BusiAlgo::ChangeBcd2Ascii((S_BYTE *)&MPlimit.m_UbcxxRatio,2,2)).append("#电压偏差下限(单位%)\r\n");
		strFileInfo.append("\tFrebcsxRatio=").append(C_BusiAlgo::ChangeBcd2Ascii((S_BYTE *)&MPlimit.m_FrebcsxRatio,2,2)).append("#频率偏差上限(单位%)\r\n");
		strFileInfo.append("\tFrebcxxRatio=").append(C_BusiAlgo::ChangeBcd2Ascii((S_BYTE *)&MPlimit.m_FrebcxxRatio,2,2)).append("#频率偏差下限(单位%)\r\n");
		strFileInfo.append("\tULTwinkleVal=").append(C_BusiAlgo::ChangeBcd2Ascii((S_BYTE *)&MPlimit.m_ULTwinkleVal,2,2)).append("#电压长闪限值\r\n");
		strFileInfo.append("\tUSTwinkleVal=").append(C_BusiAlgo::ChangeBcd2Ascii((S_BYTE *)&MPlimit.m_USTwinkleVal,2,2)).append("#电压短闪限值\r\n");
		strFileInfo.append("\tWGCompensate=").append(C_BusiAlgo::ChangeBcd2Ascii((S_BYTE *)&MPlimit.m_WGCompensate,2,2)).append("#无功过补偿、无功欠补偿告警发生阈值总功率因数缺省0.8\r\n");
		strFileInfo.append("\tWGrCompensate=").append(C_BusiAlgo::ChangeBcd2Ascii((S_BYTE *)&MPlimit.m_WGrCompensate,2,2)).append("#无功过补偿、无功欠补偿告警恢复发生阈值总功率因数缺省0.9\r\n");

		strFileInfo.append("}\r\n\r\n");
	}

	return C_IniBase::SaveSetFile(strFileInfo, fname);
}



