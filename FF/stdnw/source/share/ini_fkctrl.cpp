/** @file
*  @brief 负控管理参数加载和保存
 *  @author 
 *  @date 2012/2/22
 *  @version 0.1
 */
#include "ini_fkctrl.h"
#include "inifileb.h"

/** @brief 负控功率控配置参数结构体 */
cfg_opt_t OPT_FK_TPBCTRL_SETTING[] =
{
	CFG_BOOL("Enable", cfg_true,CFGF_NOCASE),
	CFG_INT("TPBCoefficient", 0,CFGF_NOCASE),
	CFG_INT("MinuteCount",00, CFGF_NOCASE),
	CFG_INT("WindowCount",00, CFGF_NOCASE),
	CFG_END()
};
cfg_opt_t OPT_FK_TIMECTRL_PWRTIME_SETTING[] =
{
	CFG_BOOL("Enable", cfg_true,CFGF_NOCASE),
	CFG_STR("Time","", CFGF_NOCASE),
	CFG_STR("PwrValve", "",CFGF_NOCASE),
	CFG_END()
};
cfg_opt_t OPT_FK_TIMECTRL_EXETIME_SETTING[] =
{
	CFG_STR("StartDate","", CFGF_NOCASE),
	CFG_STR("EndDate","", CFGF_NOCASE),
	CFG_INT("TI", 0,CFGF_NOCASE),
	CFG_STR_LIST("Ndate","{}", CFGF_NOCASE),
	CFG_END()
};

cfg_opt_t OPT_FK_TIMECTRL_SETTING[] =
{
	CFG_INT("SDMaxNum", 0,CFGF_NOCASE),
	CFG_SEC("PwrTime", OPT_FK_TIMECTRL_PWRTIME_SETTING, CFGF_MULTI | CFGF_TITLE |  CFGF_NOCASE),
	CFG_SEC("ExeTime", OPT_FK_TIMECTRL_EXETIME_SETTING, CFGF_MULTI | CFGF_NOCASE),
	CFG_END()
};

cfg_opt_t OPT_FK_FACTORYCTRL_SETTING[] =
{
	CFG_BOOL("Enable", cfg_true,CFGF_NOCASE),
	CFG_STR("CtrlLimitVar", "",CFGF_NOCASE),
	CFG_STR("StartTime", "",CFGF_NOCASE),
	CFG_INT("ContinueTime",00, CFGF_NOCASE),
	CFG_INT_LIST("CtrlDate","{}", CFGF_NOCASE),
	CFG_END()
};
cfg_opt_t OPT_FK_PWR_SETTINGS[] =
{
	CFG_BOOL("PWREnable", cfg_true,CFGF_NOCASE),
	CFG_INT_LIST("TurnEnable","{}", CFGF_NOCASE),
	CFG_INT("PwrCoefficient", 0,CFGF_NOCASE),
	CFG_INT_LIST("SumFlag","{}", CFGF_NOCASE),
	CFG_INT_LIST("TN","{}", CFGF_NOCASE),
	CFG_SEC("TPBCtrlParam", OPT_FK_TPBCTRL_SETTING, CFGF_MULTI | CFGF_NOCASE),
	CFG_SEC("TimeCtrlParam", OPT_FK_TIMECTRL_SETTING, CFGF_MULTI | CFGF_TITLE |  CFGF_NOCASE),
	CFG_SEC("FactoryCtrlParam", OPT_FK_FACTORYCTRL_SETTING, CFGF_MULTI | CFGF_NOCASE),
	CFG_END()
};
/** @fn ServerCfg::LoadPWRCtrlInfo
 *  @brief 加载负控配置参数
 *  @author zhgx
 *  @param[out] oContext:配置结构体
 *  @param[in] name:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniFKCtrl::LoadPWRCtrlInfo(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfgPWRCtrl = cfg_init(OPT_FK_PWR_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSetFile(cfgPWRCtrl, fname) != 0)
	{
		cfg_free(cfgPWRCtrl);
		return -1;
	}
	S_PWRCtrlInfo &PWRCtrlInfo = pStatus->m_PWRCtrlInfo;

	//功率控制是否有效
	PWRCtrlInfo.m_Enable =(cfg_getbool(cfgPWRCtrl, "PWREnable")==cfg_true?1:0);	
	//功率控制各轮次跳闸是否有效
	for(unsigned int i=0;(i<cfg_size(cfgPWRCtrl, "TurnEnable") && i<8);i++)
	{
		(cfg_getnint(cfgPWRCtrl, "TurnEnable",i)?PWRCtrlInfo.m_TurnEnable.SetBit(i):PWRCtrlInfo.m_TurnEnable.ClearBit(i));
	}
	//功率定值浮动系数
	PWRCtrlInfo.m_PwrCoefficient =(INT8S)cfg_getint(cfgPWRCtrl,"PwrCoefficient");
	//功率控制用功率累加标志
	for(unsigned int i=0;(i<cfg_size(cfgPWRCtrl, "SumFlag") && i<8);i++)
	{
		(cfg_getnint(cfgPWRCtrl, "SumFlag",i)?PWRCtrlInfo.m_struPwrSum.m_SumFlag.SetBit(i):PWRCtrlInfo.m_struPwrSum.m_SumFlag.ClearBit(i));
	}
	for(unsigned int i=0;(i<cfg_size(cfgPWRCtrl, "TN") && i<8);i++)
	{
		PWRCtrlInfo.m_struPwrSum.m_cTN[i] = (INT16U)cfg_getnint(cfgPWRCtrl, "TN",i);
	}
	//临时限电控相关参数
	cfg_t* cfgTPBCtrlParam = cfg_getsec(cfgPWRCtrl, "TPBCtrlParam");
	PWRCtrlInfo.m_TPBCtrlParam.m_Enable =(cfg_getbool(cfgTPBCtrlParam, "Enable")==cfg_true?1:0);
	PWRCtrlInfo.m_TPBCtrlParam.m_TPBCoefficient =(INT8S)cfg_getint(cfgTPBCtrlParam,"TPBCoefficient");
	PWRCtrlInfo.m_TPBCtrlParam.m_MinuteCount =(INT16U)cfg_getint(cfgTPBCtrlParam,"MinuteCount");
	PWRCtrlInfo.m_TPBCtrlParam.m_WindowCount =(INT8U)cfg_getint(cfgTPBCtrlParam,"WindowCount");
	//时段控相关参数
	cfg_t* cfgTimeCtrlParam;
	int Ndata[4];
	for(unsigned int i = 0; i < cfg_size(cfgPWRCtrl, "TimeCtrlParam"); i++)
	{
		int TypeID=0;
		cfgTimeCtrlParam = cfg_getnsec(cfgPWRCtrl, "TimeCtrlParam", i);
		sscanf(cfg_title(cfgTimeCtrlParam), "type%d", &TypeID);
		if (TypeID < 1 || TypeID > CONS_MAX_FK_TIMECTRL_TYPE)
			continue;
		PWRCtrlInfo.m_TimeCtrlParam[i].m_SDMaxNum =(INT8U)cfg_getint(cfgTimeCtrlParam,"SDMaxNum");
		if((PWRCtrlInfo.m_TimeCtrlParam[i].m_SDMaxNum < 1)||(PWRCtrlInfo.m_TimeCtrlParam[i].m_SDMaxNum > CONS_MAX_FK_TIMEL_SET))//超范围处理
			PWRCtrlInfo.m_TimeCtrlParam[i].m_SDMaxNum = 0;
		cfg_t* cfgPwrTime;
		for(unsigned int j = 0; j < cfg_size(cfgTimeCtrlParam, "PwrTime"); j++)
		{
			int PwrTypeID =0;
			cfgPwrTime = cfg_getnsec(cfgTimeCtrlParam,"PwrTime",j);
			sscanf(cfg_title(cfgPwrTime),"Time%d",&PwrTypeID);
			if(PwrTypeID<1 || PwrTypeID>CONS_MAX_FK_TIMEL_SET)
				continue;
			PWRCtrlInfo.m_TimeCtrlParam[i].m_PwrTime[j].m_Enable =(cfg_getbool(cfgPwrTime, "Enable")==cfg_true?1:0);
			sscanf(cfg_getstr(cfgPwrTime,"Time"),"%d:%d",&Ndata[1],&Ndata[0]);
			PWRCtrlInfo.m_TimeCtrlParam[i].m_PwrTime[j].m_Time.m_A1 = (INT8U)Ndata[1];
			PWRCtrlInfo.m_TimeCtrlParam[i].m_PwrTime[j].m_Time.m_A0 = (INT8U)Ndata[0];

			C_BCD bcdPwrValve(cfg_getstr(cfgPwrTime, "PwrValve"));
			const S_BYTE *pBcd = bcdPwrValve.GetBcdData();
			memcpy(&PWRCtrlInfo.m_TimeCtrlParam[i].m_PwrTime[j].m_PwrValve, pBcd+1, 4); 
		}
		cfg_t* cfgExeTime = cfg_getsec(cfgTimeCtrlParam,"ExeTime");
		memset(Ndata,0x00,4);
		sscanf(cfg_getstr(cfgExeTime,"StartDate"),"%d.%d",&Ndata[3],&Ndata[2]);
		sscanf(cfg_getstr(cfgExeTime,"EndDate"),"%d.%d",&Ndata[1],&Ndata[0]);
		PWRCtrlInfo.m_TimeCtrlParam[i].m_ExeTime.m_StartDate.m_A1 = (INT8U)Ndata[3];
		PWRCtrlInfo.m_TimeCtrlParam[i].m_ExeTime.m_StartDate.m_A0 = (INT8U)Ndata[2];
		PWRCtrlInfo.m_TimeCtrlParam[i].m_ExeTime.m_EndDate.m_A1 = (INT8U)Ndata[1];
		PWRCtrlInfo.m_TimeCtrlParam[i].m_ExeTime.m_EndDate.m_A0 = (INT8U)Ndata[0];
		PWRCtrlInfo.m_TimeCtrlParam[i].m_ExeTime.m_TI = (INT8U)cfg_getint(cfgExeTime,"TI");

		PWRCtrlInfo.m_TimeCtrlParam[i].m_ExeTime.m_N.Stat();
		if(PWRCtrlInfo.m_TimeCtrlParam[i].m_ExeTime.m_TI == 4)//频率为日
		{
			if( cfg_size(cfgExeTime, "Ndate") > 1 )
				PWRCtrlInfo.m_TimeCtrlParam[i].m_ExeTime.m_N.set(0);
			else
				PWRCtrlInfo.m_TimeCtrlParam[i].m_ExeTime.m_N.SetBit(cfg_getstr(cfgExeTime,"Ndate"),'-');
		}
		else if(PWRCtrlInfo.m_TimeCtrlParam[i].m_ExeTime.m_TI == 6)//频率为周
		{
			for (unsigned int k = 0; k < cfg_size(cfgExeTime, "Ndate") && k < 7; k++)
			{
				PWRCtrlInfo.m_TimeCtrlParam[i].m_ExeTime.m_N.SetBit(cfg_getnstr(cfgExeTime,"Ndate",k),'-');
			}
		}
		else if(PWRCtrlInfo.m_TimeCtrlParam[i].m_ExeTime.m_TI == 5)//频率为月
		{
			for (unsigned int k = 0; k < cfg_size(cfgExeTime, "Ndate") && k < 32; k++)
			{
				PWRCtrlInfo.m_TimeCtrlParam[i].m_ExeTime.m_N.SetBit(cfg_getnstr(cfgExeTime,"Ndate",k),'-');
			}
		}
		PWRCtrlInfo.m_TimeCtrlParam[i].m_ExeTime.m_N.Stat();
	}
	//厂休控相关参数
	cfg_t* cfgFactoryCtrlParam = cfg_getsec(cfgPWRCtrl, "FactoryCtrlParam");
	PWRCtrlInfo.m_TPBCtrlParam.m_Enable =(cfg_getbool(cfgFactoryCtrlParam, "Enable")==cfg_true?1:0);
	C_BCD bcdCtrlLimitVar(cfg_getstr(cfgFactoryCtrlParam, "CtrlLimitVar"));
	const S_BYTE *pBcd = bcdCtrlLimitVar.GetBcdData();
	memcpy(&PWRCtrlInfo.m_FactoryCtrlParam.m_CtrlLimitVar, pBcd+1, 4); 
	sscanf(cfg_getstr(cfgFactoryCtrlParam,"StartTime"),"%d:%d",&PWRCtrlInfo.m_FactoryCtrlParam.m_StartTime.m_A1,&PWRCtrlInfo.m_FactoryCtrlParam.m_StartTime.m_A0);
	PWRCtrlInfo.m_FactoryCtrlParam.m_ContinueTime = (S_WORD)cfg_getint(cfgFactoryCtrlParam,"ContinueTime");
	for (unsigned int i = 0; i < cfg_size(cfgFactoryCtrlParam, "CtrlDate") && i < 8; i++)
	{
		(cfg_getnint(cfgFactoryCtrlParam, "CtrlDate",i)?PWRCtrlInfo.m_FactoryCtrlParam.m_CtrlDate.SetBit(i):PWRCtrlInfo.m_FactoryCtrlParam.m_CtrlDate.ClearBit(i));
	}

	cfg_free(cfgPWRCtrl);

	return 0;
}
/** @fn ServerCfg::SavePWRCtrlInfo
 *  @brief 保存负控功率控制配置参数
 *  @author zhgx
 *  @param[out] oContext:配置结构体
 *  @param[in] name:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniFKCtrl::SavePWRCtrlInfo(S_STATUS *pStatus, const char *fname)
{
	string strPWRCtrl;
	string strbuf;
	char buf[30];
	S_PWRCtrlInfo &PWRCtrlInfo = pStatus->m_PWRCtrlInfo;

	strPWRCtrl.append("#负控参数配置\r\n");	

	strPWRCtrl.append("#功率控制是否有效\n").append("PWREnable=");	
	(PWRCtrlInfo.m_Enable?strPWRCtrl.append("true\r\n"):strPWRCtrl.append("false\r\n"));

	strPWRCtrl.append("#功率控制各轮次跳闸是否有效\r\n").append("TurnEnable={");	
	strbuf.erase();
	PWRCtrlInfo.m_TurnEnable.PrintString(strbuf,0);
	C_BaseFn::FormatDataWithDelimit(strbuf, 1, ',');
	strPWRCtrl.append(strbuf).append("}\r\n");

	strPWRCtrl.append("#功率定值浮动系数\r\n").append("PwrCoefficient=");	
	memset(buf, 0x00, sizeof(buf));
	sprintf(buf, "%d", PWRCtrlInfo.m_PwrCoefficient);
	strPWRCtrl.append(buf).append("#功率定值浮动系数（％）缺省为0\r\n");

	strPWRCtrl.append("#功率控制用功率累加标志\r\n").append("SumFlag={");	
	strbuf.erase();
	PWRCtrlInfo.m_struPwrSum.m_SumFlag.PrintString(strbuf,0);
	C_BaseFn::FormatDataWithDelimit(strbuf, 1, ',');
	strPWRCtrl.append(strbuf).append("}\r\n");
	memset(buf, 0x00, sizeof(buf));
	strPWRCtrl.append("TN={");	
	for(int i=0;i<8;i++)
	{
		sprintf(&buf[strlen(buf)],"%d,",PWRCtrlInfo.m_struPwrSum.m_cTN[i]);
	}
	strPWRCtrl.append(buf);
	strPWRCtrl.erase(strPWRCtrl.end()-1);
	strPWRCtrl.append("}\r\n");

	strPWRCtrl.append("#临时限电控相关参数\r\n").append("TPBCtrlParam\r\n{\r\n");
	strPWRCtrl.append("  Enable=");
	(PWRCtrlInfo.m_TPBCtrlParam.m_Enable?strPWRCtrl.append("true\r\n"):strPWRCtrl.append("false\r\n"));
	strPWRCtrl.append("  TPBCoefficient=");
	memset(buf, 0x00, sizeof(buf));
	sprintf(buf, "%d", PWRCtrlInfo.m_TPBCtrlParam.m_TPBCoefficient);
	strPWRCtrl.append(buf).append("#临时限电指标%\r\n");
	strPWRCtrl.append("  MinuteCount=");
	memset(buf, 0x00, sizeof(buf));
	sprintf(buf, "%d", PWRCtrlInfo.m_TPBCtrlParam.m_MinuteCount);
	strPWRCtrl.append(buf).append("#单位分钟\r\n");
	strPWRCtrl.append("  WindowCount=");
	memset(buf, 0x00, sizeof(buf));
	sprintf(buf, "%d", PWRCtrlInfo.m_TPBCtrlParam.m_WindowCount);
	strPWRCtrl.append(buf).append("#单位分钟\r\n}\r\n");

	strPWRCtrl.append("#时段控相关参数\r\n");	
	for(int i=1;i<=CONS_MAX_FK_TIMECTRL_TYPE;i++)
	{
		memset(buf, 0x00, sizeof(buf));
		sprintf(buf, "TimeCtrlParam type%d\r\n{\r\n", i);
		strPWRCtrl.append(buf);

		strPWRCtrl.append("  SDMaxNum=");
		memset(buf, 0x00, sizeof(buf));
		sprintf(buf, "%d#时段数，最大为8\r\n", PWRCtrlInfo.m_TimeCtrlParam[i-1].m_SDMaxNum);
		strPWRCtrl.append(buf);

		for(int j=1;j<=CONS_MAX_FK_TIMEL_SET;j++)
		{
			memset(buf, 0x00, sizeof(buf));
			sprintf(buf, "  PwrTime Time%d\r\n  {\r\n", j);
			strPWRCtrl.append(buf);

            strPWRCtrl.append("    Enable=");
			(PWRCtrlInfo.m_TimeCtrlParam[i-1].m_PwrTime[j-1].m_Enable?strPWRCtrl.append("true\r\n"):strPWRCtrl.append("false\r\n"));
			strPWRCtrl.append("    Time=");
			memset(buf, 0x00, sizeof(buf));
			sprintf(buf, "%02d:%02d\r\n", PWRCtrlInfo.m_TimeCtrlParam[i-1].m_PwrTime[j-1].m_Time.m_A1,PWRCtrlInfo.m_TimeCtrlParam[i-1].m_PwrTime[j-1].m_Time.m_A0);
			strPWRCtrl.append(buf);

			C_BCD bcdPwrValve((S_BYTE*)&PWRCtrlInfo.m_TimeCtrlParam[i-1].m_PwrTime[j-1].m_PwrValve, 6, 2);
			string strPwrValve = bcdPwrValve;
			strPWRCtrl.append("    PwrValve=");
			strPWRCtrl.append(strPwrValve).append("#功率定值\r\n  }\r\n");
		}

		strPWRCtrl.append("  ExeTime\r\n  {\r\n");
		strPWRCtrl.append("    StartDate=");
		memset(buf, 0x00, sizeof(buf));
		sprintf(buf, "%02d.%02d#月:日\r\n", PWRCtrlInfo.m_TimeCtrlParam[i-1].m_ExeTime.m_StartDate.m_A1,PWRCtrlInfo.m_TimeCtrlParam[i-1].m_ExeTime.m_StartDate.m_A0);
		strPWRCtrl.append(buf);
		strPWRCtrl.append("    EndDate=");
		memset(buf, 0x00, sizeof(buf));
		sprintf(buf, "%02d.%02d#月:日\r\n", PWRCtrlInfo.m_TimeCtrlParam[i-1].m_ExeTime.m_EndDate.m_A1,PWRCtrlInfo.m_TimeCtrlParam[i-1].m_ExeTime.m_EndDate.m_A0);
		strPWRCtrl.append(buf);
		strPWRCtrl.append("    TI=");
		memset(buf, 0x00, sizeof(buf));
		sprintf(buf, "%02d", PWRCtrlInfo.m_TimeCtrlParam[i-1].m_ExeTime.m_TI);
		strPWRCtrl.append(buf).append("#频率单位 04：日 05：月 06：周\r\n");
		memset(buf, 0x00, sizeof(buf));
		strbuf.erase();
		PWRCtrlInfo.m_TimeCtrlParam[i-1].m_ExeTime.m_N.PrintString(strbuf);
		strPWRCtrl.append("    Ndate={").append(strbuf);
		if(PWRCtrlInfo.m_TimeCtrlParam[i-1].m_ExeTime.m_TI == 4)//日
			strPWRCtrl.append("}#隔的天数\r\n");
		else if(PWRCtrlInfo.m_TimeCtrlParam[i-1].m_ExeTime.m_TI == 6)//周
			strPWRCtrl.append("}#每周执行的日子 为0表示星期日\r\n");
		else if(PWRCtrlInfo.m_TimeCtrlParam[i-1].m_ExeTime.m_TI == 5)//月
			strPWRCtrl.append("}#每月执行的日子\r\n");
		strPWRCtrl.append("    #当TI=日时，N0表示频率值，如N0=1表示每日执行，N0=2表示每隔1天执行\r\n");
		strPWRCtrl.append("    #当TI=周时，表示执行的日期，0表示周日,当TI=月时，表示执行的日期\r\n  }\r\n");

		strPWRCtrl.append("}\r\n");
	}

	strPWRCtrl.append("#厂休控相关参数\r\n");	
	strPWRCtrl.append("FactoryCtrlParam\r\n{\r\n");
	strPWRCtrl.append("  Enable=");
	(PWRCtrlInfo.m_FactoryCtrlParam.m_Enable?strPWRCtrl.append("true\r\n"):strPWRCtrl.append("false\r\n"));
	strPWRCtrl.append("  CtrlLimitVar=");
	C_BCD bcdCtrlLimitVar((S_BYTE*)&PWRCtrlInfo.m_FactoryCtrlParam.m_CtrlLimitVar, 8, 2);
	string strCtrlLimitVar = bcdCtrlLimitVar;
	strPWRCtrl.append(strCtrlLimitVar).append("#0SNNNNNNNN.N：厂休控定值（一次侧，单位kW）\r\n");
	strPWRCtrl.append("  StartTime=");
	memset(buf, 0x00, sizeof(buf));
	sprintf(buf, "%02d:%02d", PWRCtrlInfo.m_FactoryCtrlParam.m_StartTime.m_A1,PWRCtrlInfo.m_FactoryCtrlParam.m_StartTime.m_A0);
	strPWRCtrl.append(buf).append("#限电起始时间，时:分\r\n");
	strPWRCtrl.append("  ContinueTime=");
	memset(buf, 0x00, sizeof(buf));
	sprintf(buf, "%d", PWRCtrlInfo.m_FactoryCtrlParam.m_ContinueTime);
	strPWRCtrl.append(buf).append("#MMMM：限电延续时间，单位分\r\n");
	strPWRCtrl.append("  CtrlDate={");
	strbuf.erase();
	PWRCtrlInfo.m_FactoryCtrlParam.m_CtrlDate.PrintString(strbuf,0);
	C_BaseFn::FormatDataWithDelimit(strbuf, 1, ',');
	strPWRCtrl.append(strbuf).append("}#格式D0~D7,每周限电日：D1~D7表示星期一~星期日，D0=0\r\n").append("}\r\n");

	return C_IniBase::SaveSetFile(strPWRCtrl, fname);
}

/** @brief 负控电量控配置参数结构体 */
cfg_opt_t OPT_FK_MONTHENERGYCTRL_SETTING[] =
{
	CFG_BOOL("Enable", cfg_true,CFGF_NOCASE),
	CFG_INT_LIST("TurnEnable","{}", CFGF_NOCASE),
	CFG_INT_LIST("SumFlag","{}", CFGF_NOCASE),
	CFG_INT_LIST("TN","{}", CFGF_NOCASE),
	CFG_INT("EnergyCoefficient", 0,CFGF_NOCASE),
	CFG_INT("MonthEnergyLimit",00, CFGF_NOCASE),
	CFG_STR("TurnEnergy", "",CFGF_NOCASE),
	CFG_END()
};
cfg_opt_t OPT_FK_BUYENERGYCTRL_SETTING[] =
{
	CFG_BOOL("Enable", cfg_true,CFGF_NOCASE),
	CFG_INT_LIST("TurnEnable","{}", CFGF_NOCASE),
	CFG_INT_LIST("SumFlag","{}", CFGF_NOCASE),
	CFG_INT_LIST("TN","{}", CFGF_NOCASE),
	CFG_INT("NowBuyEnergy", 00,CFGF_NOCASE),
	CFG_INT("NowBuyDLNo",00, CFGF_NOCASE),
	CFG_STR("NowLeftEnergy", 0,CFGF_NOCASE),
	CFG_END()
};
cfg_opt_t OPT_FK_DL_SETTINGS[] =
{
	CFG_SEC("MonthEnergyCtrlParam", OPT_FK_MONTHENERGYCTRL_SETTING, CFGF_MULTI | CFGF_NOCASE),
	CFG_SEC("BuyEnergyCtrlParam", OPT_FK_BUYENERGYCTRL_SETTING, CFGF_MULTI | CFGF_NOCASE),
	CFG_END()
};
/** @fn ServerCfg::LoadFKCtrlInfo
 *  @brief 加载负控电量控制配置参数
 *  @author zhgx
 *  @param[out] oContext:配置结构体
 *  @param[in] name:文件名
 *  @return 0:successful, -1:failed
 */
 int C_IniFKCtrl::LoadDLCtrlInfo(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfgDLCtrl;
	cfgDLCtrl = cfg_init(OPT_FK_DL_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSetFile(cfgDLCtrl, fname) != 0)
	{
		cfg_free(cfgDLCtrl);
		return -1;
	}
	S_DLCtrlInfo &DLCtrlInfo = pStatus->m_DLCtrlInfo;

	//月电量控制相关参数
	cfg_t* cfgMonthEnergyCtrlParam =cfg_getsec(cfgDLCtrl, "MonthEnergyCtrlParam");
	DLCtrlInfo.m_MonthEnergyCtrlParam.m_Enable =(cfg_getbool(cfgMonthEnergyCtrlParam, "Enable")==cfg_true?1:0);
	for(unsigned int i=0;(i<cfg_size(cfgMonthEnergyCtrlParam, "TurnEnable") && i<8);i++)
	{
		(cfg_getnint(cfgMonthEnergyCtrlParam, "TurnEnable",i)?DLCtrlInfo.m_MonthEnergyCtrlParam.m_TurnEnable.SetBit(i):DLCtrlInfo.m_MonthEnergyCtrlParam.m_TurnEnable.ClearBit(i));
	}
	for(unsigned int i=0;(i<cfg_size(cfgMonthEnergyCtrlParam, "SumFlag") && i<8);i++)
	{
		(cfg_getnint(cfgMonthEnergyCtrlParam, "SumFlag",i)?DLCtrlInfo.m_struEnergySum.m_SumFlag.SetBit(i):DLCtrlInfo.m_struEnergySum.m_SumFlag.ClearBit(i));
	}
	for(unsigned int i=0;(i<cfg_size(cfgMonthEnergyCtrlParam, "TN") && i<8);i++)
	{
		DLCtrlInfo.m_struEnergySum.m_cTN[i] = (INT16U)cfg_getnint(cfgMonthEnergyCtrlParam, "TN",i);
	}

	DLCtrlInfo.m_MonthEnergyCtrlParam.m_EnergyCoefficient =(INT8S)cfg_getint(cfgMonthEnergyCtrlParam,"EnergyCoefficient");
	DLCtrlInfo.m_MonthEnergyCtrlParam.m_MonthEnergyLimit =cfg_getint(cfgMonthEnergyCtrlParam,"MonthEnergyLimit");
	C_BCD bcdTurnEnergy(cfg_getstr(cfgMonthEnergyCtrlParam, "TurnEnergy"));
	const S_BYTE *pBcd = bcdTurnEnergy.GetBcdData();
	memcpy(&DLCtrlInfo.m_MonthEnergyCtrlParam.m_TurnEnergy, pBcd+1, 4); 

	//购电量控制相关参数
	cfg_t* cfgBuyEnergyCtrlParam =cfg_getsec(cfgDLCtrl, "BuyEnergyCtrlParam");
	DLCtrlInfo.m_BuyEnergyCtrlParam.m_Enable =(cfg_getbool(cfgBuyEnergyCtrlParam, "Enable")==cfg_true?1:0);
	for(unsigned int i=0;(i<cfg_size(cfgBuyEnergyCtrlParam, "TurnEnable") && i<8);i++)
	{
		(cfg_getnint(cfgBuyEnergyCtrlParam, "TurnEnable",i)?DLCtrlInfo.m_BuyEnergyCtrlParam.m_TurnEnable.SetBit(i):DLCtrlInfo.m_BuyEnergyCtrlParam.m_TurnEnable.ClearBit(i));
	}
	for(unsigned int i=0;(i<cfg_size(cfgBuyEnergyCtrlParam, "SumFlag") && i<8);i++)
	{
		(cfg_getnint(cfgBuyEnergyCtrlParam, "SumFlag",i)?DLCtrlInfo.m_struEnergySum.m_SumFlag.SetBit(i):DLCtrlInfo.m_struEnergySum.m_SumFlag.ClearBit(i));
	}
	for(unsigned int i=0;(i<cfg_size(cfgBuyEnergyCtrlParam, "TN") && i<8);i++)
	{
		DLCtrlInfo.m_struEnergySum.m_cTN[i] = (INT16U)cfg_getnint(cfgBuyEnergyCtrlParam, "TN",i);
	}
	DLCtrlInfo.m_BuyEnergyCtrlParam.m_NowBuyEnergy =cfg_getint(cfgBuyEnergyCtrlParam,"NowBuyEnergy");
	DLCtrlInfo.m_BuyEnergyCtrlParam.m_NowBuyDLNo =(unsigned char)cfg_getint(cfgBuyEnergyCtrlParam,"NowBuyDLNo");
	string strtemp;
	strtemp = cfg_getstr(cfgBuyEnergyCtrlParam, "NowLeftEnergy");//这里配置文件里设置的字符长度可能会产生问题，但暂不考虑
	if(strtemp[0] == '-')
	{
		DLCtrlInfo.m_BuyEnergyCtrlParam.m_NowLeftDLSign = 0;
		strtemp.erase(strtemp.begin());
	}
	C_BCD bcdNowLeftEnergy(strtemp);
	pBcd = bcdNowLeftEnergy.GetBcdData();
	memcpy(&DLCtrlInfo.m_BuyEnergyCtrlParam.m_NowLeftEnergy, pBcd+1, 4); 

	cfg_free(cfgDLCtrl);

	return 0;
}
/** @fn ServerCfg::SaveDLCtrlInfo
 *  @brief 保存电量控制配置参数
 *  @author zhgx
 *  @param[out] oContext:配置结构体
 *  @param[in] name:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniFKCtrl::SaveDLCtrlInfo(S_STATUS *pStatus, const char *fname)
{
	string strDLCtrl;
	string strbuf;
	char buf[30];
	S_DLCtrlInfo &DLCtrlInfo = pStatus->m_DLCtrlInfo;

	strDLCtrl.append("#负控参数配置\r\n").append("#月电量控制相关参数\r\n");	
	strDLCtrl.append("MonthEnergyCtrlParam\r\n{\r\nEnable=");	
    (DLCtrlInfo.m_MonthEnergyCtrlParam.m_Enable?strDLCtrl.append("true\r\n"):strDLCtrl.append("false\r\n"));

	strbuf.erase();
	strDLCtrl.append("TurnEnable={");	
	DLCtrlInfo.m_MonthEnergyCtrlParam.m_TurnEnable.PrintString(strbuf,0);
	C_BaseFn::FormatDataWithDelimit(strbuf, 1, ',');
	strDLCtrl.append(strbuf).append("}\r\n");

	strbuf.erase();
	strDLCtrl.append("SumFlag={");	
	DLCtrlInfo.m_struEnergySum.m_SumFlag.PrintString(strbuf,0);
	C_BaseFn::FormatDataWithDelimit(strbuf, 1, ',');
	strDLCtrl.append(strbuf).append("}\r\n");

	strDLCtrl.append("TN={");
	memset(buf, 0x00, sizeof(buf));
	for(int i=0;i<8;i++)
	{
		sprintf(&buf[strlen(buf)],"%d,",DLCtrlInfo.m_struEnergySum.m_cTN[i]);
	}
	strDLCtrl.append(buf);
	strDLCtrl.erase(strDLCtrl.end()-1);
	strDLCtrl.append("}\r\n");

	strDLCtrl.append("EnergyCoefficient=");	
	memset(buf, 0x00, sizeof(buf));
	sprintf(buf, "%d", DLCtrlInfo.m_MonthEnergyCtrlParam.m_EnergyCoefficient);
	strDLCtrl.append(buf).append("# %\r\n");

	strDLCtrl.append("MonthEnergyLimit=");	
	memset(buf, 0x00, sizeof(buf));
	sprintf(buf, "%d", DLCtrlInfo.m_MonthEnergyCtrlParam.m_MonthEnergyLimit);
	strDLCtrl.append(buf).append("# kwh\r\n");

	C_BCD bcdTurnEnerg((S_BYTE*)&DLCtrlInfo.m_MonthEnergyCtrlParam.m_TurnEnergy, 7, 2);
	string strTurnEnerg = bcdTurnEnerg;
	strDLCtrl.append("TurnEnergy=");	
	strDLCtrl.append(strTurnEnerg).append("# kwh\r\n").append("}\r\n");

	strDLCtrl.append("#购电量控制相关参数\r\n");	
	strDLCtrl.append("BuyEnergyCtrlParam\r\n{\r\nEnable=");	
	(DLCtrlInfo.m_BuyEnergyCtrlParam.m_Enable?strDLCtrl.append("true\r\n"):strDLCtrl.append("false\r\n"));

	strbuf.erase();
	strDLCtrl.append("TurnEnable={");	
	DLCtrlInfo.m_BuyEnergyCtrlParam.m_TurnEnable.PrintString(strbuf,0);
	C_BaseFn::FormatDataWithDelimit(strbuf, 1, ',');
	strDLCtrl.append(strbuf).append("}\r\n");

	strbuf.erase();
	strDLCtrl.append("SumFlag={");	
	DLCtrlInfo.m_struEnergySum.m_SumFlag.PrintString(strbuf,0);
	C_BaseFn::FormatDataWithDelimit(strbuf, 1, ',');
	strDLCtrl.append(strbuf).append("}\r\n");

	strDLCtrl.append("TN={");
	memset(buf, 0x00, sizeof(buf));
	for(int i=0;i<8;i++)
	{
		sprintf(&buf[strlen(buf)],"%d,",DLCtrlInfo.m_struEnergySum.m_cTN[i]);
	}
	strDLCtrl.append(buf);
	strDLCtrl.erase(strDLCtrl.end()-1);
	strDLCtrl.append("}\r\n");

	strDLCtrl.append("NowBuyEnergy=");	
	memset(buf, 0x00, sizeof(buf));
	sprintf(buf, "%d", DLCtrlInfo.m_BuyEnergyCtrlParam.m_NowBuyEnergy);
	strDLCtrl.append(buf).append("# 本次购电量 kwh\r\n");

	strDLCtrl.append("NowBuyDLNo=");	
	memset(buf, 0x00, sizeof(buf));
	sprintf(buf, "%d", DLCtrlInfo.m_BuyEnergyCtrlParam.m_NowBuyDLNo);
	strDLCtrl.append(buf).append("#本次购电单序号\r\n");

	strDLCtrl.append("NowLeftEnergy=");	
	if(DLCtrlInfo.m_BuyEnergyCtrlParam.m_NowLeftDLSign==0)
		strDLCtrl.append("-");
	C_BCD bcdNowLeftEnergy((S_BYTE*)&DLCtrlInfo.m_BuyEnergyCtrlParam.m_NowLeftEnergy, 7, 2);
	string strNowLeftEnergy = bcdNowLeftEnergy;
	strDLCtrl.append(strNowLeftEnergy).append("#剩余电量\r\n").append("}\r\n");

	return C_IniBase::SaveSetFile(strDLCtrl, fname);
}

