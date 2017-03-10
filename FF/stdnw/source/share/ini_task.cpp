/** @file
 *  @brief ������غͱ���
 *  @author 
 *  @date 2011/12/06
 *  @version 0.1
 *  �ɱ��������վ�ϱ������ڴ˴�������غͱ��溯��
 */
#include "ini_task.h"


cfg_opt_t OPT_TASK_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_false,CFGF_NOCASE),
	CFG_INT("TaskNo",0, CFGF_NOCASE),
	CFG_INT("RecordMax",0, CFGF_NOCASE),
	CFG_INT("MeterType", 1, CFGF_NOCASE),
	CFG_INT("VipFlag", 0, CFGF_NOCASE),
	CFG_STR_LIST("DA", "{}", CFGF_NONE),
	CFG_STR_LIST("DT", "{}", CFGF_NONE),
	CFG_END()
};
cfg_opt_t OPT_GATHER_TASKS_SETTINGS[] =
{
	CFG_SEC("Task", OPT_TASK_SETTINGS, CFGF_MULTI |CFGF_NOCASE),
	CFG_END()
};

int CInitTask::LoadGatherTasks(S_STATUS *pStatus,const char* name)
{
	/* �Թ����ڴ��еĳ���������Ϣ��״̬��ʼ��*/
 	pStatus->m_GatherTaskInfos.Init();

	cfg_t* cfg = cfg_init(OPT_GATHER_TASKS_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSetFile(cfg,name)!=0)
	{
		cfg_free(cfg);
		return -1;
	}
	
	for(unsigned int i = 0; i < cfg_size(cfg, "Task"); i++)
	{
		cfg_t* cfgTask = cfg_getnsec(cfg, "Task", i);
		if (cfg_getbool(cfgTask, "Enable") == cfg_false)
			continue;
		INT8U taskNo = (INT8U)cfg_getint(cfgTask, "TaskNo");
		if (taskNo < 1 || taskNo > CONS_TASK_COUNT_MAX)
			continue;

		S_GatherTaskInfo &GatherTaskInfo = pStatus->m_GatherTaskInfos.m_ItemInfo[taskNo-1];
		GatherTaskInfo.m_Enable = true; ///< ��Ч�Ա�־
		GatherTaskInfo.m_RecordMax = (S_WORD)cfg_getint(cfgTask, "RecordMax"); ///< �洢����	Ϊ0��ʾ�����洢�����洢
		GatherTaskInfo.m_MeterType = (S_BYTE)cfg_getint(cfgTask, "MeterType"); ///< �������	BIN	1	01��������ӱ�02���๦�ܱ�03���������ͣ�  ff����Ч����	//ע����Թ��䡢ר�估��վ�նˣ����ֶι̶����Ϊff
		GatherTaskInfo.m_VipFlag = (S_BYTE)cfg_getint(cfgTask, "VipFlag"); ///< �ص��û�����	BIN	1	0����ͨ�û���1���ص��û�ff����Ч����//ע����Թ��䡢ר�估��վ�նˣ����ֶι̶����Ϊff

		S_WORD Cnt = 0;
		for(Cnt = 0; Cnt < cfg_size(cfgTask, "DA") && Cnt < DBCJ_TASK_INFO_DADT_CNT_MAX; Cnt++)
		{
			S_DWORD DA = 0;
			sscanf(cfg_getnstr(cfgTask, "DA", Cnt), "%04x", &DA);
			GatherTaskInfo.m_DA[Cnt] = KGE_SWAPWORD((S_WORD)DA);
		}
		GatherTaskInfo.m_DaCnt = Cnt;
		for(Cnt = 0; Cnt < cfg_size(cfgTask, "DT") && Cnt < DBCJ_TASK_INFO_DADT_CNT_MAX; Cnt++)
		{
			S_DWORD DT = 0;
			sscanf(cfg_getnstr(cfgTask, "DT", Cnt), "%08x", &DT);
			GatherTaskInfo.m_DT[Cnt] = DT;
		}
		GatherTaskInfo.m_DtCnt = Cnt;
	}
	cfg_free(cfg);
	return 0;
}

int CInitTask::SaveGatherTasks(S_STATUS *pStatus,const char* name)
{
	char buffer[64];
	string strFileInfo;

	strFileInfo.append("#�ɼ��������ü���\r\n" "#DA��ʽ�����ֽ�DA2�����ֽ�DA1\r\n" "#DT��ʽ����->��,DI3 DI2 DI1 DI0\r\n\r\n");
	for (S_WORD taskNo = 0; taskNo < CONS_TASK_COUNT_MAX; taskNo++)
	{
		S_GatherTaskInfo &GatherTaskInfo = pStatus->m_GatherTaskInfos.m_ItemInfo[taskNo];
		if (GatherTaskInfo.m_Enable == false)
			continue;

		strFileInfo.append("Task\r\n{");
		strFileInfo.append("\r\n\tEnable=").append(C_IniBase::GetBoolKey(GatherTaskInfo.m_Enable));
		
		sprintf(buffer,"%d", taskNo+1);
		strFileInfo.append("\r\n\tTaskNo=").append(buffer);
		sprintf(buffer,"%d", GatherTaskInfo.m_RecordMax);
		strFileInfo.append("\r\n\tRecordMax=").append(buffer).append(" #�洢����	Ϊ0��ʾ�����洢�����洢");
		sprintf(buffer,"%d", GatherTaskInfo.m_MeterType);
		strFileInfo.append("\r\n\tMeterType=").append(buffer).append(" #1��������ӱ�2���๦�ܱ�");;
		sprintf(buffer,"%d", GatherTaskInfo.m_VipFlag);
		strFileInfo.append("\r\n\tVipFlag=").append(buffer).append(" #0����ͨ�û���1���ص��û�");;

		strFileInfo.append("\r\n\tDA=\r\n\t{\r\n\t\t");
		for (S_WORD Cnt = 0x00; Cnt < GatherTaskInfo.m_DaCnt; Cnt++)
		{
			S_WORD DA = KGE_SWAPWORD(GatherTaskInfo.m_DA[Cnt]);
			sprintf(buffer, "%04X", DA);
			strFileInfo.append(buffer).append(",");
			if((Cnt+1)%12 == 0)
				strFileInfo.append("\r\n\t\t");
		}
		if (*(strFileInfo.end()-1) == ',')
			strFileInfo.erase(strFileInfo.end()-1);
		strFileInfo.append("\r\n\t} #��ʽ:DA1DA2");

		strFileInfo.append("\r\n\tDT=\r\n\t{\r\n\t\t");
		for (S_WORD Cnt = 0x00; Cnt < GatherTaskInfo.m_DtCnt; Cnt++)
		{
			sprintf(buffer, "%08X", GatherTaskInfo.m_DT[Cnt]);
			strFileInfo.append(buffer).append(",");
			if((Cnt+1)%12 == 0)
				strFileInfo.append("\r\n\t\t");
		}
		if (*(strFileInfo.end()-1) == ',')
			strFileInfo.erase(strFileInfo.end()-1);
		strFileInfo.append("\r\n\t} #��ʽ:DI3DI2DI1DI0");

		strFileInfo.append("\r\n}\r\n\r\n");
	}
	return C_IniBase::SaveSetFile(strFileInfo,name);
}


/** @brief ��ͨ����������ýṹ�� */
cfg_opt_t OPT_NORMAL_TASK_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_true,CFGF_NOCASE),
	CFG_BOOL("Active", cfg_true,CFGF_NOCASE),
	CFG_INT("TaskNo", 1, CFGF_NONE),
	CFG_STR("ReportStdDate", "2009-10-14", CFGF_NONE),
	CFG_STR("ReportStdTime", "00:00:00", CFGF_NONE),
	CFG_STR("ReportPeriodUnit", "hour", CFGF_NONE),
	CFG_INT("ReportPeriodVal", 1, CFGF_NONE),
	CFG_INT("DataFormat", 0, CFGF_NONE),
	CFG_STR("SampleStdDate", "2009-10-14", CFGF_NONE),
	CFG_STR("SampleStdTime", "00:00:00", CFGF_NONE),
	CFG_STR("SamplePeriodUnit", "hour", CFGF_NONE),
	CFG_INT("SamplePeriodVal", 1, CFGF_NONE),
	CFG_INT("ExtractRate", 1, CFGF_NONE),
	CFG_INT("MaxTimes", 0, CFGF_NONE),
	CFG_STR_LIST("TaskDA", "{}", CFGF_NONE),
	CFG_STR_LIST("TaskDI", "{}", CFGF_NONE),
	CFG_END()
};
cfg_opt_t OPT_NORMAL_TASKS_SETTINGS[] =
{
	CFG_SEC("Task", OPT_NORMAL_TASK_SETTINGS, CFGF_MULTI |CFGF_NOCASE),
	CFG_END()
};

#define NORMAL_TASK_NO_MIN	0x01
#define NORMAL_TASK_NO_MAX	0xFE
/** @fn CInitTask::LoadNormalTask
 *  @brief ���ؼ���ͨѶ����
 *  @param[out] pStatus:���ýṹ��
 *  @param[in] fname:�ļ���
 *  @return 0:successful, -1:failed
 */
 int CInitTask::LoadNormalTask(S_STATUS *pStatus, const char *fname)
{
	S_NormalTaskInfos& infos = pStatus->m_NormalTaskInfos;
	infos.InitTaskInfo();

	cfg_t* cfg  = cfg_init(OPT_NORMAL_TASKS_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSetFile(cfg, fname)!=0)
	{
		cfg_free(cfg);
		return -1;
	}

	for(unsigned int i = 0; i < cfg_size(cfg, "Task"); i++)
	{
		cfg_t* cfgTask = cfg_getnsec(cfg, "Task", i);
		if (cfg_getbool(cfgTask, "Enable") == cfg_false)
		{
			continue;
		}
		int TaskNo = cfg_getint(cfgTask, "TaskNo");
		if (TaskNo < NORMAL_TASK_NO_MIN || TaskNo > NORMAL_TASK_NO_MAX)
			continue;

		S_NormalTaskInfo& info = infos.m_ItemInfo[TaskNo-1];
		info.m_Enable = true;
		info.m_Active = cfg_getbool(cfgTask, "Active")==cfg_true?true:false;
		info.m_ReportStdTime = GetStandardTime(cfg_getstr(cfgTask,"ReportStdDate"), cfg_getstr(cfgTask,"ReportStdTime"));
		info.m_ReportPeriodUnit = (S_BYTE)GetPeriodUnitVal(cfg_getstr(cfgTask,"ReportPeriodUnit")); ///minute,hour,day,month
		info.m_ReportPeriodVal = (S_BYTE)cfg_getint(cfgTask,"ReportPeriodVal");

		info.m_DataFormat = (S_BYTE)cfg_getint(cfgTask,"DataFormat");/// #0��ʾ��������ʽ��֯���ݣ�1��ʾ������������ݸ�ʽ��֯����
		info.m_SampleStdTime = GetStandardTime(cfg_getstr(cfgTask,"SampleStdDate"), cfg_getstr(cfgTask,"SampleStdTime"));
		info.m_SamplePeriodUnit = (S_BYTE)GetPeriodUnitVal(cfg_getstr(cfgTask,"SamplePeriodUnit"));/// #minute,hour,day,month
		info.m_SamplePeriodVal = (S_BYTE)cfg_getint(cfgTask,"SamplePeriodVal");

		info.m_ExtractRate = (S_BYTE)cfg_getint(cfgTask,"ExtractRate"); // #���մ˱��ʳ�ȡ��������
		info.m_MaxTimes = (S_WORD)cfg_getint(cfgTask,"MaxTimes"); ///#0��ʾ��Զִ�У���0��ʾ����ִ�д���
		for (info.m_DaCnt = 0; info.m_DaCnt < cfg_size(cfgTask, "TaskDA") && info.m_DaCnt < REPORT_TASK_INFO_DADT_CNT_MAX; info.m_DaCnt++)
		{
			S_DWORD DA = 0x00;
			sscanf(cfg_getnstr(cfgTask, "TaskDA", info.m_DaCnt), "%04X", &DA);
			info.m_DA[info.m_DaCnt] = KGE_SWAPWORD((S_WORD)DA);
		}
		for (info.m_DtCnt = 0; info.m_DtCnt < cfg_size(cfgTask, "TaskDI") && info.m_DtCnt < REPORT_TASK_INFO_DADT_CNT_MAX; info.m_DtCnt++)
		{
			S_DWORD DT = 0x00;
			sscanf(cfg_getnstr(cfgTask, "TaskDI", info.m_DtCnt), "%08X", &DT);
			info.m_DT[info.m_DtCnt] = DT;
		}
	}

	cfg_free(cfg);
	return 0;
}

/** @fn CInitTask::SaveNormalTask
 *  @brief ���漶��ͨѶ����
 *  @param[in] pStatus:���ýṹ��
 *  @param[in] name:�ļ���
 *  @return 0:successful, -1:failed
 */
int CInitTask::SaveNormalTask(S_STATUS *pStatus, const char *fname)
{
	char buffer[64];
	string strFileInfo;

	strFileInfo.append("#��ͨ�������ü���\r\n\r\n");
	for (int TaskNo = NORMAL_TASK_NO_MIN; TaskNo <= NORMAL_TASK_NO_MAX; TaskNo++)
	{
		S_NormalTaskInfo& info = pStatus->m_NormalTaskInfos.m_ItemInfo[TaskNo - 1];
		if (info.m_Enable == false)
			continue;

		strFileInfo.append("Task\r\n{");
		strFileInfo.append("\r\n\tEnable=").append(C_IniBase::GetBoolKey(info.m_Enable));
		strFileInfo.append("\r\n\tActive=").append(C_IniBase::GetBoolKey(info.m_Active));
		
		sprintf(buffer,"%d",TaskNo);
		strFileInfo.append("\r\n\tTaskNo=").append(buffer);

		GetStandardTime(info.m_ReportStdTime, buffer, buffer+32);
		strFileInfo.append("\r\n\tReportStdDate=").append(buffer).append(" #��-��-��");
		strFileInfo.append("\r\n\tReportStdTime=").append(buffer+32).append(" #ʱ:��:��");
		strFileInfo.append("\r\n\tReportPeriodUnit=").append(GetPeriodUnitName(info.m_ReportPeriodUnit)).append(" #minute,hour,day,month");
		sprintf(buffer,"%d",info.m_ReportPeriodVal);
		strFileInfo.append("\r\n\tReportPeriodVal=").append(buffer);
		sprintf(buffer,"%d",info.m_DataFormat);
		strFileInfo.append("\r\n\tDataFormat=").append(buffer).append(" #0��ʾ��������ʽ��֯���ݣ�1��ʾ������������ݸ�ʽ��֯����");

		GetStandardTime(info.m_SampleStdTime, buffer, buffer+32);
		strFileInfo.append("\r\n\tSampleStdDate=").append(buffer).append(" #��-��-��");
		strFileInfo.append("\r\n\tSampleStdTime=").append(buffer+32).append(" #ʱ:��:��");
		strFileInfo.append("\r\n\tSamplePeriodUnit=").append(GetPeriodUnitName(info.m_SamplePeriodUnit)).append(" #minute,hour,day,month");
		sprintf(buffer,"%d",info.m_SamplePeriodVal);
		strFileInfo.append("\r\n\tSamplePeriodVal=").append(buffer);

		sprintf(buffer,"%d",info.m_ExtractRate);
		strFileInfo.append("\r\n\tExtractRate=").append(buffer).append(" #���մ˱��ʳ�ȡ��������");
		sprintf(buffer,"%d",info.m_MaxTimes);
		strFileInfo.append("\r\n\tMaxTimes=").append(buffer).append(" #0��ʾ��Զִ�У���0��ʾ����ִ�д���");

		strFileInfo.append("\r\n\tTaskDA={\r\n\t\t");
		for (S_WORD Cnt = 0x00; Cnt < info.m_DaCnt; Cnt++)
		{
			S_WORD DA = KGE_SWAPWORD(info.m_DA[Cnt]);
			sprintf(buffer, "%04X", DA);
			strFileInfo.append(buffer).append(",");
			if((Cnt+1)%12 == 0)
				strFileInfo.append("\r\n\t\t");
		}
		if (*(strFileInfo.end()-1) == ',')
			strFileInfo.erase(strFileInfo.end()-1);
		strFileInfo.append("\r\n\t} #��Ƹ�ʽ:DA1DA2");

		strFileInfo.append("\r\n\tTaskDI={\r\n\t\t");
		for (S_WORD Cnt = 0x00; Cnt < info.m_DtCnt; Cnt++)
		{
			sprintf(buffer, "%08X", info.m_DT[Cnt]);
			strFileInfo.append(buffer).append(",");
			if((Cnt+1)%12 == 0)
				strFileInfo.append("\r\n\t\t");
		}
		if (*(strFileInfo.end()-1) == ',')
			strFileInfo.erase(strFileInfo.end()-1);
		strFileInfo.append("\r\n\t} #������ ��ʽ:DI3DI2DI1DI0");

		strFileInfo.append("\r\n}\r\n\r\n");
	}

	return C_IniBase::SaveSetFile(strFileInfo, fname);
}

/** @brief �м�����������ýṹ�� */
cfg_opt_t OPT_FORWARD_TASK_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_true,CFGF_NOCASE),
	CFG_BOOL("Active", cfg_true,CFGF_NOCASE),
	CFG_INT("TaskNo", 1, CFGF_NONE),
	CFG_STR("ReportStdDate", "2009-10-14", CFGF_NONE),
	CFG_STR("ReportStdTime", "00:00:00", CFGF_NONE),
	CFG_STR("ReportPeriodUnit", "hour", CFGF_NONE),
	CFG_INT("ReportPeriodVal", 1, CFGF_NONE),
	CFG_STR("SampleStdDate", "2009-10-14", CFGF_NONE),
	CFG_STR("SampleStdTime", "00:00:00", CFGF_NONE),
	CFG_STR("SamplePeriodUnit", "hour", CFGF_NONE),
	CFG_INT("SamplePeriodVal", 1, CFGF_NONE),
	CFG_INT("ExtractRate", 1, CFGF_NONE),
	CFG_INT("ForwardType", 0, CFGF_NONE),
	CFG_INT("MaxTimes", 0, CFGF_NONE),
	CFG_STR("PortSet.No", "VCom1", CFGF_NOCASE),
	CFG_INT("PortSet.Baud", 1200, CFGF_NOCASE),
	CFG_INT("PortSet.DataBit", 8, CFGF_NOCASE),
	CFG_INT("PortSet.StopBit", 1, CFGF_NOCASE),
	CFG_INT("PortSet.Parity", 2, CFGF_NOCASE),
	CFG_INT("TimeoutS", 30, CFGF_NOCASE),
	CFG_STR("ForwardCmd", "", CFGF_NOCASE),
	CFG_END()
};
cfg_opt_t OPT_FORWARD_TASKS_SETTINGS[] =
{
	CFG_SEC("Task", OPT_FORWARD_TASK_SETTINGS, CFGF_MULTI |CFGF_NOCASE),
	CFG_END()
};

#define FORWARD_TASK_NO_MIN	0x01
#define FORWARD_TASK_NO_MAX	0xFE
/** @fn CInitTask::LoadForwardTask
 *  @brief ���ؼ���ͨѶ����
 *  @param[out] pStatus:���ýṹ��
 *  @param[in] fname:�ļ���
 *  @return 0:successful, -1:failed
 */
 int CInitTask::LoadForwardTask(S_STATUS *pStatus, const char *fname)
{
	S_ForwardTaskInfos& infos = pStatus->m_ForwardTaskInfos;
	infos.InitTaskInfo();

	cfg_t* cfg  = cfg_init(OPT_FORWARD_TASKS_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSetFile(cfg, fname)!=0)
	{
		cfg_free(cfg);
		return -1;
	}

	for(unsigned int i = 0; i < cfg_size(cfg, "Task"); i++)
	{
		cfg_t* cfgTask = cfg_getnsec(cfg, "Task", i);
		if (cfg_getbool(cfgTask, "Enable") == cfg_false)
			continue;
		int TaskNo = cfg_getint(cfgTask, "TaskNo");
		if (TaskNo < FORWARD_TASK_NO_MIN || TaskNo > FORWARD_TASK_NO_MAX)
			continue;
		S_ForwardTaskInfo& info = infos.m_ItemInfo[TaskNo];

		info.m_Enable = true;
		info.m_Active = cfg_getbool(cfgTask, "Active")==cfg_true?true:false;
		info.m_ReportStdTime = GetStandardTime(cfg_getstr(cfgTask,"ReportStdDate"), cfg_getstr(cfgTask,"ReportStdTime"));
		info.m_ReportPeriodUnit = (S_BYTE)GetPeriodUnitVal(cfg_getstr(cfgTask,"ReportPeriodUnit")); ///minute,hour,day,month
		info.m_ReportPeriodVal = (S_BYTE)cfg_getint(cfgTask,"ReportPeriodVal");
		info.m_SampleStdTime = GetStandardTime(cfg_getstr(cfgTask,"SampleStdDate"), cfg_getstr(cfgTask,"SampleStdTime"));
		info.m_SamplePeriodUnit = (S_BYTE)GetPeriodUnitVal(cfg_getstr(cfgTask,"SamplePeriodUnit"));/// #minute,hour,day,month
		info.m_SamplePeriodVal = (S_BYTE)cfg_getint(cfgTask,"SamplePeriodVal");

		info.m_ExtractRate = (S_BYTE)cfg_getint(cfgTask,"ExtractRate"); // #���մ˱��ʳ�ȡ��������
		info.m_ForwardType = (S_BYTE)cfg_getint(cfgTask,"ForwardType");
		info.m_MaxTimes = (S_WORD)cfg_getint(cfgTask,"MaxTimes"); ///#0��ʾ��Զִ�У���0��ʾ����ִ�д���

		int VPort = 0;
		sscanf(cfg_getstr(cfgTask, "PortSet.No"), "VCom%d", &VPort);
		info.m_PortSet.m_PortNO = (INT8U)VPort; ///< �˿ںŲ�����˿ںţ�00H~1EH��ʾ485��˿ںţ�1FH��ʾ�ز�ͨ����20H��ʾ���ߣ�30H��ʾ������
		info.m_PortSet.m_Baud  = cfg_getint(cfgTask, "PortSet.Baud");
		info.m_PortSet.m_DataBit = (INT8S)cfg_getint(cfgTask, "PortSet.DataBit");
		info.m_PortSet.m_StopBit = (INT8S)cfg_getint(cfgTask, "PortSet.StopBit");
		info.m_PortSet.m_Parity  = (INT8S)cfg_getint(cfgTask, "PortSet.Parity");

		info.m_TimeoutS= (S_BYTE)cfg_getint(cfgTask,"TimeoutS");
		string strHex, strAscii(cfg_getstr(cfgTask, "ForwardCmd"));
		if (EncodeAsciiCmd2HexCmd(strHex, strAscii) < sizeof(info.m_ForwardCmd))
		{
			copy(strHex.begin(), strHex.end(), info.m_ForwardCmd);
			info.m_ForwardCmdLen = (S_BYTE)strHex.size();
		}
	}

	cfg_free(cfg);
	return 0;
}

/** @fn CInitTask::SaveForwardTask
 *  @brief ���漶��ͨѶ����
 *  @param[in] pStatus:���ýṹ��
 *  @param[in] name:�ļ���
 *  @return 0:successful, -1:failed
 */
int CInitTask::SaveForwardTask(S_STATUS *pStatus, const char *fname)
{
	char buffer[64];
	string strFileInfo;

	strFileInfo.append("#�м��������ü���\r\n\r\n");
	for (int TaskNo = FORWARD_TASK_NO_MIN; TaskNo <= FORWARD_TASK_NO_MAX; TaskNo++)
	{
		S_ForwardTaskInfo& info = pStatus->m_ForwardTaskInfos.m_ItemInfo[TaskNo];
		if (info.m_Enable == false)
			continue;

		strFileInfo.append("Task\r\n{");
		strFileInfo.append("\r\n\tEnable=").append(C_IniBase::GetBoolKey(info.m_Enable));
		strFileInfo.append("\r\n\tActive=").append(C_IniBase::GetBoolKey(info.m_Active));
		
		sprintf(buffer,"%d",TaskNo);
		strFileInfo.append("\r\n\tTaskNo=").append(buffer);

		GetStandardTime(info.m_ReportStdTime, buffer, buffer+32);
		strFileInfo.append("\r\n\tReportStdDate=").append(buffer).append(" #��-��-��");
		strFileInfo.append("\r\n\tReportStdTime=").append(buffer+32).append(" #ʱ:��:��");
		strFileInfo.append("\r\n\tReportPeriodUnit=").append(GetPeriodUnitName(info.m_ReportPeriodUnit)).append(" #minute,hour,day,month");
		sprintf(buffer,"%d",info.m_ReportPeriodVal);
		strFileInfo.append("\r\n\tReportPeriodVal=").append(buffer);
		GetStandardTime(info.m_SampleStdTime, buffer, buffer+32);
		strFileInfo.append("\r\n\tSampleStdDate=").append(buffer).append(" #��-��-��");
		strFileInfo.append("\r\n\tSampleStdTime=").append(buffer+32).append(" #ʱ:��:��");
		strFileInfo.append("\r\n\tSamplePeriodUnit=").append(GetPeriodUnitName(info.m_SamplePeriodUnit)).append(" #minute,hour,day,month");
		sprintf(buffer,"%d",info.m_SamplePeriodVal);
		strFileInfo.append("\r\n\tSamplePeriodVal=").append(buffer);

		sprintf(buffer,"%d",info.m_ExtractRate);
		strFileInfo.append("\r\n\tExtractRate=").append(buffer).append(" #���մ˱��ʳ�ȡ��������");
		sprintf(buffer,"%d",info.m_ForwardType);
		strFileInfo.append("\r\n\tForwardType=").append(buffer).append(" #00��ͨ�м̣�01��բ���02��բ���03����Ͷ�����04����������");
		sprintf(buffer,"%d",info.m_MaxTimes);
		strFileInfo.append("\r\n\tMaxTimes=").append(buffer).append(" #0��ʾ��Զִ�У���0��ʾ����ִ�д���");

		sprintf(buffer,"VCom%d",info.m_PortSet.m_PortNO);
		strFileInfo.append("\r\n\tPortSet.No=").append(buffer);
		sprintf(buffer,"%d",info.m_PortSet.m_Baud);
		strFileInfo.append("\r\n\tPortSet.Baud=").append(buffer);
		sprintf(buffer,"%d",info.m_PortSet.m_DataBit);
		strFileInfo.append("\r\n\tPortSet.DataBit=").append(buffer);
		sprintf(buffer,"%d",info.m_PortSet.m_StopBit);
		strFileInfo.append("\r\n\tPortSet.StopBit=").append(buffer);
		sprintf(buffer,"%d",info.m_PortSet.m_Parity);
		strFileInfo.append("\r\n\tPortSet.Parity=").append(buffer);

		sprintf(buffer,"%d",info.m_TimeoutS);
		strFileInfo.append("\r\n\tTimeoutS=").append(buffer);

		string strAscii, strHex(info.m_ForwardCmd, info.m_ForwardCmdLen);
		DecodeHexCmd2AsciiCmd(strAscii, strHex);
		strFileInfo.append("\r\n\tForwardCmd=\"").append(strAscii).append("\"");

		strFileInfo.append("\r\n}\r\n\r\n");
	}

	return C_IniBase::SaveSetFile(strFileInfo, fname);
}


const S_CFGITEM TaskPeriodUnit[] = 
{
	{0, "minute"}, {1, "hour"}, {2, "day"}, {3, "month"}
};

int CInitTask::GetPeriodUnitVal(const char *pName)
{
	return C_IniBase::MakeValue(TaskPeriodUnit, sizeof(TaskPeriodUnit)/sizeof(TaskPeriodUnit[0]), pName);
}

const char *CInitTask::GetPeriodUnitName(int val)
{
	return C_IniBase::MakeKey(TaskPeriodUnit, sizeof(TaskPeriodUnit)/sizeof(TaskPeriodUnit[0]), val);
}

S_TIME CInitTask::GetStandardTime(const char *pdate, const char *ptime)
{
	S_DATE date;
	sscanf(pdate, "%d-%d-%d", &date.m_Year, &date.m_Mon, &date.m_Day);
	sscanf(ptime, "%d:%d:%d", &date.m_Hour, &date.m_Min, &date.m_Sec);
	return C_TIME::MakeTime(date);
}

void CInitTask::GetStandardTime(S_TIME timeVal, char *pdate, char *ptime)
{
	S_DATE date;
	C_TIME::LocalTime(timeVal, date);
	sprintf(pdate, "%d-%d-%d", date.m_Year, date.m_Mon, date.m_Day);
	sprintf(ptime, "%d:%d:%d", date.m_Hour, date.m_Min, date.m_Sec);
}

S_WORD CInitTask::EncodeAsciiCmd2HexCmd(string &strHex, const string &strAscii)
{
	for (S_WORD i = 0; i < strAscii.size(); i++)
	{
		if (!isxdigit(strAscii[i]))
			continue;
		S_BYTE H = kge_Char2Hex(strAscii[i]);
		S_BYTE L = 0;
		if (++i < strAscii.size() && isxdigit(strAscii[i]))
			L = kge_Char2Hex(strAscii[i]);
		else
			L = H, H = 0;
		strHex.push_back((H<<4)|L);
	}
	return (S_WORD)strHex.size();
}

S_WORD CInitTask::DecodeHexCmd2AsciiCmd(string &strAscii, const string &strHex)
{
	if (!strHex.empty())
	{
		strAscii.push_back(kge_Hex2Char((strHex[0]>>4)&0x0F));
		strAscii.push_back(kge_Hex2Char((strHex[0]>>0)&0x0F));
	}
	for (S_WORD i = 1; i < strHex.size(); i++)
	{
		strAscii.push_back(' ');
		strAscii.push_back(kge_Hex2Char((strHex[i]>>4)&0x0F));
		strAscii.push_back(kge_Hex2Char((strHex[i]>>0)&0x0F));
	}
	return (S_WORD)strAscii.size();
}


