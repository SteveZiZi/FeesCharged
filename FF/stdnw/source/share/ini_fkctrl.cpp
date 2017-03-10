/** @file
*  @brief ���ع���������غͱ���
 *  @author 
 *  @date 2012/2/22
 *  @version 0.1
 */
#include "ini_fkctrl.h"
#include "inifileb.h"

/** @brief ���ع��ʿ����ò����ṹ�� */
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
 *  @brief ���ظ������ò���
 *  @author zhgx
 *  @param[out] oContext:���ýṹ��
 *  @param[in] name:�ļ���
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

	//���ʿ����Ƿ���Ч
	PWRCtrlInfo.m_Enable =(cfg_getbool(cfgPWRCtrl, "PWREnable")==cfg_true?1:0);	
	//���ʿ��Ƹ��ִ���բ�Ƿ���Ч
	for(unsigned int i=0;(i<cfg_size(cfgPWRCtrl, "TurnEnable") && i<8);i++)
	{
		(cfg_getnint(cfgPWRCtrl, "TurnEnable",i)?PWRCtrlInfo.m_TurnEnable.SetBit(i):PWRCtrlInfo.m_TurnEnable.ClearBit(i));
	}
	//���ʶ�ֵ����ϵ��
	PWRCtrlInfo.m_PwrCoefficient =(INT8S)cfg_getint(cfgPWRCtrl,"PwrCoefficient");
	//���ʿ����ù����ۼӱ�־
	for(unsigned int i=0;(i<cfg_size(cfgPWRCtrl, "SumFlag") && i<8);i++)
	{
		(cfg_getnint(cfgPWRCtrl, "SumFlag",i)?PWRCtrlInfo.m_struPwrSum.m_SumFlag.SetBit(i):PWRCtrlInfo.m_struPwrSum.m_SumFlag.ClearBit(i));
	}
	for(unsigned int i=0;(i<cfg_size(cfgPWRCtrl, "TN") && i<8);i++)
	{
		PWRCtrlInfo.m_struPwrSum.m_cTN[i] = (INT16U)cfg_getnint(cfgPWRCtrl, "TN",i);
	}
	//��ʱ�޵����ز���
	cfg_t* cfgTPBCtrlParam = cfg_getsec(cfgPWRCtrl, "TPBCtrlParam");
	PWRCtrlInfo.m_TPBCtrlParam.m_Enable =(cfg_getbool(cfgTPBCtrlParam, "Enable")==cfg_true?1:0);
	PWRCtrlInfo.m_TPBCtrlParam.m_TPBCoefficient =(INT8S)cfg_getint(cfgTPBCtrlParam,"TPBCoefficient");
	PWRCtrlInfo.m_TPBCtrlParam.m_MinuteCount =(INT16U)cfg_getint(cfgTPBCtrlParam,"MinuteCount");
	PWRCtrlInfo.m_TPBCtrlParam.m_WindowCount =(INT8U)cfg_getint(cfgTPBCtrlParam,"WindowCount");
	//ʱ�ο���ز���
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
		if((PWRCtrlInfo.m_TimeCtrlParam[i].m_SDMaxNum < 1)||(PWRCtrlInfo.m_TimeCtrlParam[i].m_SDMaxNum > CONS_MAX_FK_TIMEL_SET))//����Χ����
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
		if(PWRCtrlInfo.m_TimeCtrlParam[i].m_ExeTime.m_TI == 4)//Ƶ��Ϊ��
		{
			if( cfg_size(cfgExeTime, "Ndate") > 1 )
				PWRCtrlInfo.m_TimeCtrlParam[i].m_ExeTime.m_N.set(0);
			else
				PWRCtrlInfo.m_TimeCtrlParam[i].m_ExeTime.m_N.SetBit(cfg_getstr(cfgExeTime,"Ndate"),'-');
		}
		else if(PWRCtrlInfo.m_TimeCtrlParam[i].m_ExeTime.m_TI == 6)//Ƶ��Ϊ��
		{
			for (unsigned int k = 0; k < cfg_size(cfgExeTime, "Ndate") && k < 7; k++)
			{
				PWRCtrlInfo.m_TimeCtrlParam[i].m_ExeTime.m_N.SetBit(cfg_getnstr(cfgExeTime,"Ndate",k),'-');
			}
		}
		else if(PWRCtrlInfo.m_TimeCtrlParam[i].m_ExeTime.m_TI == 5)//Ƶ��Ϊ��
		{
			for (unsigned int k = 0; k < cfg_size(cfgExeTime, "Ndate") && k < 32; k++)
			{
				PWRCtrlInfo.m_TimeCtrlParam[i].m_ExeTime.m_N.SetBit(cfg_getnstr(cfgExeTime,"Ndate",k),'-');
			}
		}
		PWRCtrlInfo.m_TimeCtrlParam[i].m_ExeTime.m_N.Stat();
	}
	//���ݿ���ز���
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
 *  @brief ���渺�ع��ʿ������ò���
 *  @author zhgx
 *  @param[out] oContext:���ýṹ��
 *  @param[in] name:�ļ���
 *  @return 0:successful, -1:failed
 */
int C_IniFKCtrl::SavePWRCtrlInfo(S_STATUS *pStatus, const char *fname)
{
	string strPWRCtrl;
	string strbuf;
	char buf[30];
	S_PWRCtrlInfo &PWRCtrlInfo = pStatus->m_PWRCtrlInfo;

	strPWRCtrl.append("#���ز�������\r\n");	

	strPWRCtrl.append("#���ʿ����Ƿ���Ч\n").append("PWREnable=");	
	(PWRCtrlInfo.m_Enable?strPWRCtrl.append("true\r\n"):strPWRCtrl.append("false\r\n"));

	strPWRCtrl.append("#���ʿ��Ƹ��ִ���բ�Ƿ���Ч\r\n").append("TurnEnable={");	
	strbuf.erase();
	PWRCtrlInfo.m_TurnEnable.PrintString(strbuf,0);
	C_BaseFn::FormatDataWithDelimit(strbuf, 1, ',');
	strPWRCtrl.append(strbuf).append("}\r\n");

	strPWRCtrl.append("#���ʶ�ֵ����ϵ��\r\n").append("PwrCoefficient=");	
	memset(buf, 0x00, sizeof(buf));
	sprintf(buf, "%d", PWRCtrlInfo.m_PwrCoefficient);
	strPWRCtrl.append(buf).append("#���ʶ�ֵ����ϵ��������ȱʡΪ0\r\n");

	strPWRCtrl.append("#���ʿ����ù����ۼӱ�־\r\n").append("SumFlag={");	
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

	strPWRCtrl.append("#��ʱ�޵����ز���\r\n").append("TPBCtrlParam\r\n{\r\n");
	strPWRCtrl.append("  Enable=");
	(PWRCtrlInfo.m_TPBCtrlParam.m_Enable?strPWRCtrl.append("true\r\n"):strPWRCtrl.append("false\r\n"));
	strPWRCtrl.append("  TPBCoefficient=");
	memset(buf, 0x00, sizeof(buf));
	sprintf(buf, "%d", PWRCtrlInfo.m_TPBCtrlParam.m_TPBCoefficient);
	strPWRCtrl.append(buf).append("#��ʱ�޵�ָ��%\r\n");
	strPWRCtrl.append("  MinuteCount=");
	memset(buf, 0x00, sizeof(buf));
	sprintf(buf, "%d", PWRCtrlInfo.m_TPBCtrlParam.m_MinuteCount);
	strPWRCtrl.append(buf).append("#��λ����\r\n");
	strPWRCtrl.append("  WindowCount=");
	memset(buf, 0x00, sizeof(buf));
	sprintf(buf, "%d", PWRCtrlInfo.m_TPBCtrlParam.m_WindowCount);
	strPWRCtrl.append(buf).append("#��λ����\r\n}\r\n");

	strPWRCtrl.append("#ʱ�ο���ز���\r\n");	
	for(int i=1;i<=CONS_MAX_FK_TIMECTRL_TYPE;i++)
	{
		memset(buf, 0x00, sizeof(buf));
		sprintf(buf, "TimeCtrlParam type%d\r\n{\r\n", i);
		strPWRCtrl.append(buf);

		strPWRCtrl.append("  SDMaxNum=");
		memset(buf, 0x00, sizeof(buf));
		sprintf(buf, "%d#ʱ���������Ϊ8\r\n", PWRCtrlInfo.m_TimeCtrlParam[i-1].m_SDMaxNum);
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
			strPWRCtrl.append(strPwrValve).append("#���ʶ�ֵ\r\n  }\r\n");
		}

		strPWRCtrl.append("  ExeTime\r\n  {\r\n");
		strPWRCtrl.append("    StartDate=");
		memset(buf, 0x00, sizeof(buf));
		sprintf(buf, "%02d.%02d#��:��\r\n", PWRCtrlInfo.m_TimeCtrlParam[i-1].m_ExeTime.m_StartDate.m_A1,PWRCtrlInfo.m_TimeCtrlParam[i-1].m_ExeTime.m_StartDate.m_A0);
		strPWRCtrl.append(buf);
		strPWRCtrl.append("    EndDate=");
		memset(buf, 0x00, sizeof(buf));
		sprintf(buf, "%02d.%02d#��:��\r\n", PWRCtrlInfo.m_TimeCtrlParam[i-1].m_ExeTime.m_EndDate.m_A1,PWRCtrlInfo.m_TimeCtrlParam[i-1].m_ExeTime.m_EndDate.m_A0);
		strPWRCtrl.append(buf);
		strPWRCtrl.append("    TI=");
		memset(buf, 0x00, sizeof(buf));
		sprintf(buf, "%02d", PWRCtrlInfo.m_TimeCtrlParam[i-1].m_ExeTime.m_TI);
		strPWRCtrl.append(buf).append("#Ƶ�ʵ�λ 04���� 05���� 06����\r\n");
		memset(buf, 0x00, sizeof(buf));
		strbuf.erase();
		PWRCtrlInfo.m_TimeCtrlParam[i-1].m_ExeTime.m_N.PrintString(strbuf);
		strPWRCtrl.append("    Ndate={").append(strbuf);
		if(PWRCtrlInfo.m_TimeCtrlParam[i-1].m_ExeTime.m_TI == 4)//��
			strPWRCtrl.append("}#��������\r\n");
		else if(PWRCtrlInfo.m_TimeCtrlParam[i-1].m_ExeTime.m_TI == 6)//��
			strPWRCtrl.append("}#ÿ��ִ�е����� Ϊ0��ʾ������\r\n");
		else if(PWRCtrlInfo.m_TimeCtrlParam[i-1].m_ExeTime.m_TI == 5)//��
			strPWRCtrl.append("}#ÿ��ִ�е�����\r\n");
		strPWRCtrl.append("    #��TI=��ʱ��N0��ʾƵ��ֵ����N0=1��ʾÿ��ִ�У�N0=2��ʾÿ��1��ִ��\r\n");
		strPWRCtrl.append("    #��TI=��ʱ����ʾִ�е����ڣ�0��ʾ����,��TI=��ʱ����ʾִ�е�����\r\n  }\r\n");

		strPWRCtrl.append("}\r\n");
	}

	strPWRCtrl.append("#���ݿ���ز���\r\n");	
	strPWRCtrl.append("FactoryCtrlParam\r\n{\r\n");
	strPWRCtrl.append("  Enable=");
	(PWRCtrlInfo.m_FactoryCtrlParam.m_Enable?strPWRCtrl.append("true\r\n"):strPWRCtrl.append("false\r\n"));
	strPWRCtrl.append("  CtrlLimitVar=");
	C_BCD bcdCtrlLimitVar((S_BYTE*)&PWRCtrlInfo.m_FactoryCtrlParam.m_CtrlLimitVar, 8, 2);
	string strCtrlLimitVar = bcdCtrlLimitVar;
	strPWRCtrl.append(strCtrlLimitVar).append("#0SNNNNNNNN.N�����ݿض�ֵ��һ�β࣬��λkW��\r\n");
	strPWRCtrl.append("  StartTime=");
	memset(buf, 0x00, sizeof(buf));
	sprintf(buf, "%02d:%02d", PWRCtrlInfo.m_FactoryCtrlParam.m_StartTime.m_A1,PWRCtrlInfo.m_FactoryCtrlParam.m_StartTime.m_A0);
	strPWRCtrl.append(buf).append("#�޵���ʼʱ�䣬ʱ:��\r\n");
	strPWRCtrl.append("  ContinueTime=");
	memset(buf, 0x00, sizeof(buf));
	sprintf(buf, "%d", PWRCtrlInfo.m_FactoryCtrlParam.m_ContinueTime);
	strPWRCtrl.append(buf).append("#MMMM���޵�����ʱ�䣬��λ��\r\n");
	strPWRCtrl.append("  CtrlDate={");
	strbuf.erase();
	PWRCtrlInfo.m_FactoryCtrlParam.m_CtrlDate.PrintString(strbuf,0);
	C_BaseFn::FormatDataWithDelimit(strbuf, 1, ',');
	strPWRCtrl.append(strbuf).append("}#��ʽD0~D7,ÿ���޵��գ�D1~D7��ʾ����һ~�����գ�D0=0\r\n").append("}\r\n");

	return C_IniBase::SaveSetFile(strPWRCtrl, fname);
}

/** @brief ���ص��������ò����ṹ�� */
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
 *  @brief ���ظ��ص����������ò���
 *  @author zhgx
 *  @param[out] oContext:���ýṹ��
 *  @param[in] name:�ļ���
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

	//�µ���������ز���
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

	//������������ز���
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
	strtemp = cfg_getstr(cfgBuyEnergyCtrlParam, "NowLeftEnergy");//���������ļ������õ��ַ����ȿ��ܻ�������⣬���ݲ�����
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
 *  @brief ��������������ò���
 *  @author zhgx
 *  @param[out] oContext:���ýṹ��
 *  @param[in] name:�ļ���
 *  @return 0:successful, -1:failed
 */
int C_IniFKCtrl::SaveDLCtrlInfo(S_STATUS *pStatus, const char *fname)
{
	string strDLCtrl;
	string strbuf;
	char buf[30];
	S_DLCtrlInfo &DLCtrlInfo = pStatus->m_DLCtrlInfo;

	strDLCtrl.append("#���ز�������\r\n").append("#�µ���������ز���\r\n");	
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

	strDLCtrl.append("#������������ز���\r\n");	
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
	strDLCtrl.append(buf).append("# ���ι����� kwh\r\n");

	strDLCtrl.append("NowBuyDLNo=");	
	memset(buf, 0x00, sizeof(buf));
	sprintf(buf, "%d", DLCtrlInfo.m_BuyEnergyCtrlParam.m_NowBuyDLNo);
	strDLCtrl.append(buf).append("#���ι��絥���\r\n");

	strDLCtrl.append("NowLeftEnergy=");	
	if(DLCtrlInfo.m_BuyEnergyCtrlParam.m_NowLeftDLSign==0)
		strDLCtrl.append("-");
	C_BCD bcdNowLeftEnergy((S_BYTE*)&DLCtrlInfo.m_BuyEnergyCtrlParam.m_NowLeftEnergy, 7, 2);
	string strNowLeftEnergy = bcdNowLeftEnergy;
	strDLCtrl.append(strNowLeftEnergy).append("#ʣ�����\r\n").append("}\r\n");

	return C_IniBase::SaveSetFile(strDLCtrl, fname);
}

