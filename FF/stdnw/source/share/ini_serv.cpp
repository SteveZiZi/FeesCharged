/** @file
 *  @brief ϵͳ������غͱ���
 *  @author 
 *  @date 2011/12/06
 *  @version 0.1
 *  ����ϵͳ������غͱ��溯��
 */
#include "ini_serv.h"
#include "inifileb.h"
#include "osrelate.h"
#include "sysctrl.h"
#include "cfg_proc.h"
#include "htrace.h"
#include "strings.h"
#include "basealgo.h"
#include "ipcSharemem.h"

/** @brief ͨ�����ýṹ�� */
cfg_opt_t OPT_CHANNL_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_false,CFGF_NOCASE),
	CFG_STR("Name", "Serial", CFGF_NONE),
	CFG_STR("Type", "Com", CFGF_NONE),
	CFG_STR("Setting", "7,1200.8.1.2,Serial,NoCD", CFGF_NONE),
	CFG_INT("ReadTimeoutMS", 3000, CFGF_NONE),
	CFG_INT("WriteTimeoutMS", 3000, CFGF_NONE),
	CFG_INT("SubReadTimeoutMS", 50, CFGF_NONE),
	CFG_INT("CharReadTimeoutMS", 50, CFGF_NONE),
	CFG_INT("IdleTimeoutS", 0, CFGF_NONE),
	CFG_INT("ConnectTimeoutS", 30, CFGF_NONE),
	CFG_INT("ConnectInterS", 5, CFGF_NONE),
	CFG_INT("PackageLenMax", 500, CFGF_NONE),

	CFG_END()
};
cfg_opt_t OPT_CHANNLS_SETTINGS[] =
{
	CFG_SEC("Channel", OPT_CHANNL_SETTINGS, CFGF_MULTI | CFGF_NOCASE),
	CFG_END()
};

/** @fn C_IniServer::LoadChannel
 *  @brief ����ָ��ͨ��
 *  @param[out] pChannl:ͨ��ָ��
 *  @param[in] cfgChannel:ͨ���ִ�������Ϣ
 */
void C_IniServer::LoadChannel(cfg_t* cfgChannel, struct S_CHANNEL *pChannl)
{
	pChannl->m_Enable = true;
	memset(pChannl->m_Name, 0x00, sizeof(pChannl->m_Name));
	strncpy((char *)pChannl->m_Name, cfg_getstr(cfgChannel,"Name"),sizeof(pChannl->m_Name)-1);
	pChannl->m_Type=(S_BYTE)MakeChannelTypeVal(cfg_getstr(cfgChannel, "Type"));
	PortSettingParse(pChannl, cfg_getstr(cfgChannel,"Setting"));

	pChannl->m_ReadTimeoutMS=(S_WORD)cfg_getint(cfgChannel, "ReadTimeoutMS");
	pChannl->m_WriteTimeoutMS=(S_WORD)cfg_getint(cfgChannel, "WriteTimeoutMS");
	pChannl->m_SubReadTimeoutMS=(S_WORD)cfg_getint(cfgChannel, "SubReadTimeoutMS");
	pChannl->m_CharReadTimeoutMS=(S_WORD)cfg_getint(cfgChannel, "CharReadTimeoutMS");
	pChannl->m_ConnectTimeoutS=(S_BYTE)cfg_getint(cfgChannel, "ConnectTimeoutS");
	pChannl->m_ConnectInterS=(S_WORD)cfg_getint(cfgChannel, "ConnectInterS");
	pChannl->m_IdleTimeoutS=(S_WORD)cfg_getint(cfgChannel, "IdleTimeoutS");
	pChannl->m_PackageLenMax=(S_WORD)cfg_getint(cfgChannel, "PackageLenMax");
}

const S_CFGITEM g_ChannelTypeName[] =
{
	{CHANNEL_TYPE_NET, "Net"},
	{CHANNEL_TYPE_COM, "Com"},
	{CHANNEL_TYPE_PSTN, "Pstn"}
};
const char *C_IniServer::MakeChannelTypeKey(int val)
{
	return C_IniBase::MakeKey(g_ChannelTypeName, KGE_MEMBER_OF(g_ChannelTypeName), val);
}

int C_IniServer::MakeChannelTypeVal(char *pKey)
{
	return C_IniBase::MakeValue(g_ChannelTypeName, KGE_MEMBER_OF(g_ChannelTypeName), pKey);
}

/** @fn ServerCfg::LoadUpChannl
 *  @brief ��������ͨ����������
 *  @param[out] pStatus:���ýṹ��
 *  @param[in] fname:�ļ���
 *  @return 0:successful, -1:failed
 */
int C_IniServer::LoadUpChannl(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfg = cfg_init(OPT_CHANNLS_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSetFile(cfg, fname) != 0)
	{
		cfg_free(cfg);
		return -1;
	}

	int ChannlIdx = 0;
	pStatus->m_UpChannels.Init();
	for(S_DWORD i = 0; i < cfg_size(cfg, "Channel"); i++)
	{
		cfg_t* cfgChannel = cfg_getnsec(cfg, "Channel", i);
		if (cfg_getbool(cfgChannel, "Enable") == cfg_false)
			continue;

		struct S_CHANNEL *pChannl = pStatus->m_UpChannels.FindChannel(ChannlIdx++);
		if (pChannl == NULL)
			break;

		LoadChannel(cfgChannel, pChannl);
	}

	cfg_free(cfg);
	return 0;
}

/** @fn C_IniServer::SaveUpChannl
 *  @brief ��������ͨ����������
 *  @param[in] pStatus:���ýṹ��
 *  @param[in] fname:�ļ���
 *  @return 0:successful, -1:failed
 */
int C_IniServer::SaveUpChannl(S_STATUS *pStatus, const char *fname)
{
	char Buffer[64];
	string strFileInfo;

	strFileInfo.append("#note:��վ,PPP,����ͨ��\r\n"
		"#ChannelType:Net,Com,Pstn\r\n"
		"#NetType: TcpClient,TcpServer,UdpClient,UdpServer,Tcp,Udp\r\n"
		"#PPP: NoPPP,NeedPPP\r\n"
		"#DataBit:5,6,7,8\r\n"
		"#StopBit:1->1λ,2->2λ,3->1.5\r\n"
		"#Parity:0->no parity;1->odd parity;2->even parity\r\n"
		"#CD:NoCD,NeedCD\r\n"
		"#Pstn:VCom,19200.8.1.0,PowerMethed(Ring1On:Ring2On:AlwaysOn),DialMethed(Normal:Twice)\r\n\r\n");

	int ChannlIdx = 0;
	struct S_CHANNEL *pChannel = NULL;
	while ((pChannel = pStatus->m_UpChannels.FindChannel(ChannlIdx++))!= NULL)
	{
		if (pChannel->m_Enable == false)
			continue;

		strFileInfo.append("Channel\r\n{");
		strFileInfo.append("\r\n\tEnable=true");
		strFileInfo.append("\r\n\tName=").append((char *)pChannel->m_Name);
		strFileInfo.append("\r\n\tType=").append(MakeChannelTypeKey(pChannel->m_Type));

		PortSettingPrint(pChannel, Buffer);
		strFileInfo.append("\r\n\tSetting=\"").append(Buffer).append("\"");

		sprintf(Buffer,"%d",pChannel->m_ReadTimeoutMS);
		strFileInfo.append("\r\n\tReadTimeoutMS=").append(Buffer);
		sprintf(Buffer,"%d",pChannel->m_WriteTimeoutMS);
		strFileInfo.append("\r\n\tWriteTimeoutMS=").append(Buffer);
		sprintf(Buffer,"%d",pChannel->m_SubReadTimeoutMS);
		strFileInfo.append("\r\n\tSubReadTimeoutMS=").append(Buffer);
		sprintf(Buffer,"%d",pChannel->m_CharReadTimeoutMS);
		strFileInfo.append("\r\n\tCharReadTimeoutMS=").append(Buffer);

		sprintf(Buffer,"%d",pChannel->m_ConnectTimeoutS);
		strFileInfo.append("\r\n\tConnectTimeoutS=").append(Buffer);
		sprintf(Buffer,"%d",pChannel->m_ConnectInterS);
		strFileInfo.append("\r\n\tConnectInterS=").append(Buffer);

		sprintf(Buffer,"%d",pChannel->m_IdleTimeoutS);
		strFileInfo.append("\r\n\tIdleTimeoutS=").append(Buffer);
		sprintf(Buffer,"%d",pChannel->m_PackageLenMax);
		strFileInfo.append("\r\n\tPackageLenMax=").append(Buffer);
		strFileInfo.append("\r\n}\r\n\r\n");	
	}
	return C_IniBase::SaveSetFile(strFileInfo, fname);
}

/** @fn C_IniServer::LoadDnChannl
 *  @brief ���زɱ�ͨ����������
 *  @param[out] pStatus:���ýṹ��
 *  @param[in] fname:�ļ���
 *  @return 0:successful, -1:failed
 */
int C_IniServer::LoadDnChannl(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfg = cfg_init(OPT_CHANNLS_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSetFile(cfg, fname) != 0)
	{
		cfg_free(cfg);
		return -1;
	}

	int ChannlIdx = 0;
	pStatus->m_DnChannels.Init();
	for(unsigned int i = 0; i<cfg_size(cfg, "Channel"); i++)
	{
		cfg_t* cfgChannel = cfg_getnsec(cfg, "Channel", i);
		if (cfg_getbool(cfgChannel, "Enable") == cfg_false)
			continue;

		struct S_CHANNEL *pChannl = pStatus->m_DnChannels.FindChannel(ChannlIdx++);
		if (pChannl == NULL)
			break;

		LoadChannel(cfgChannel,pChannl);
	}

	cfg_free(cfg);
	return 0;
}

/** @fn C_IniServer::SaveDnChannl
 * @brief ����ɱ�ͨ����������
 *  @param[out] pStatus:���ýṹ��
 *  @param[in] fname:�ļ���
 *  @return 0:successful, -1:failed
 */
int C_IniServer::SaveDnChannl(S_STATUS *pStatus, const char *fname)
{
	string strFileInfo;
	char Buffer[64];

	strFileInfo.append("#note:���вɱ�ͨ��\r\n"
		"#ChannelType:Net,Com\r\n"
		"#NetType: TcpClient,TcpServer,UdpClient,UdpServer,Tcp,Udp\r\n"
		"#PPP: NoPPP,NeedPPP\r\n"
		"#DataBit:5,6,7,8\r\n"
		"#StopBit:1->1λ,2->2λ,3->1.5\r\n"
		"#Parity:0->no parity;1->odd parity;2->even parity\r\n"
		"#CD:NoCD,NeedCD\r\n\r\n");

	int ChannlIdx = 0;
	struct S_CHANNEL *pChannel = NULL;
	while ((pChannel = pStatus->m_DnChannels.FindChannel(ChannlIdx++))!= NULL)
	{
		if (pChannel->m_Enable == false)
			continue;

		strFileInfo.append("Channel\r\n{");
		strFileInfo.append("\r\n\tEnable=true");
		strFileInfo.append("\r\n\tName=").append((char *)pChannel->m_Name);
		strFileInfo.append("\r\n\tType=").append(MakeChannelTypeKey(pChannel->m_Type));

		PortSettingPrint(pChannel, Buffer);
		strFileInfo.append("\r\n\tSetting=\"").append(Buffer).append("\"");

		sprintf(Buffer,"%d",pChannel->m_ReadTimeoutMS);
		strFileInfo.append("\r\n\tReadTimeoutMS=").append(Buffer);
		sprintf(Buffer,"%d",pChannel->m_WriteTimeoutMS);
		strFileInfo.append("\r\n\tWriteTimeoutMS=").append(Buffer);
		sprintf(Buffer,"%d",pChannel->m_SubReadTimeoutMS);
		strFileInfo.append("\r\n\tSubReadTimeoutMS=").append(Buffer);
		sprintf(Buffer,"%d",pChannel->m_CharReadTimeoutMS);
		strFileInfo.append("\r\n\tCharReadTimeoutMS=").append(Buffer);

		sprintf(Buffer,"%d",pChannel->m_ConnectTimeoutS);
		strFileInfo.append("\r\n\tConnectTimeoutS=").append(Buffer);
		sprintf(Buffer,"%d",pChannel->m_ConnectInterS);
		strFileInfo.append("\r\n\tConnectInterS=").append(Buffer);

		sprintf(Buffer,"%d",pChannel->m_IdleTimeoutS);
		strFileInfo.append("\r\n\tIdleTimeoutS=").append(Buffer);
		sprintf(Buffer,"%d",pChannel->m_PackageLenMax);
		strFileInfo.append("\r\n\tPackageLenMax=").append(Buffer);
		strFileInfo.append("\r\n}\r\n\r\n");	
	}
	return C_IniBase::SaveSetFile(strFileInfo, fname);
}

/** @brief ��վͨѶ�������ýṹ�� */
cfg_opt_t OPT_HTTX_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_true,CFGF_NOCASE),
	CFG_BOOL("ReportEnable", cfg_false,CFGF_NOCASE),
	CFG_INT("ResendTimes", 3, CFGF_NONE),
	CFG_INT("WaitResponseTimeoutS", 20, CFGF_NONE),
	CFG_INT("LoginPeriodS", 0, CFGF_NONE),
	CFG_INT("HeartBeatPeriodM", 0, CFGF_NONE),
	CFG_INT("MonTrafficLimit", 0, CFGF_NONE),
	CFG_INT("MsgConfirmCode", 0, CFGF_NONE),
	CFG_STR_LIST("Channel", "{}", CFGF_NONE),

	CFG_END()
};
cfg_opt_t OPT_HTTXS_SETTINGS[] =
{
	CFG_SEC("Httx", OPT_HTTX_SETTINGS, CFGF_MULTI | CFGF_TITLE |  CFGF_NOCASE),
	CFG_END()
};

/** @fn C_IniServer::LoadHttxInfo
 *  @brief ������վͨѶ����
 *  @param[out] pStatus:���ýṹ��
 *  @param[in] fname:�ļ���
 *  @return 0:successful, -1:failed
 */
int C_IniServer::LoadHttxInfo(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfg  = cfg_init(OPT_HTTXS_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSetFile(cfg, fname)!=0)
	{
		cfg_free(cfg);
		return -1;
	}

	for(unsigned int i = 0; i < cfg_size(cfg, "Httx"); i++)
	{
		int httxID = 0;
		cfg_t* cfgHttx = cfg_getnsec(cfg, "Httx", i);
		sscanf(cfg_title(cfgHttx), "httx%d", &httxID);
		if (httxID < 1 || httxID > HTTX_PROCESS_MAX)
			continue;

		C_SYSCTRL::EnableProcess(GetHttxProcessName(httxID-1), cfg_getbool(cfgHttx, "Enable")? true:false);
		S_HttxInfo& info = pStatus->m_HttxInfo[httxID-1];

//		info.Init();
		info.m_ReportEnable = cfg_getbool(cfgHttx,"ReportEnable")? true:false;
		info.m_MsgConfirmCode = (S_BYTE)cfg_getint(cfgHttx,"MsgConfirmCode");
		info.m_HeartBeatPeriodM = (S_BYTE)cfg_getint(cfgHttx,"HeartBeatPeriodM");
		info.m_ResendTimes = (S_BYTE)cfg_getint(cfgHttx,"ResendTimes"); ///< =3 #�ط�����
		info.m_WaitResponseTimeoutS = (S_WORD)cfg_getint(cfgHttx,"WaitResponseTimeoutS"); ///< =6 #�ն˵ȴ��Ӷ�վ��Ӧ�ĳ�ʱʱ��
		info.m_LoginPeriodS = (S_WORD)cfg_getint(cfgHttx,"LoginPeriodS");
		info.m_MonTrafficLimit = (S_DWORD)cfg_getint(cfgHttx,"MonTrafficLimit");

		for (unsigned int i = 0; i < cfg_size(cfgHttx, "Channel") && i < HTTX_CHANNL_MAX; i++)
		{
			memset(info.m_ChannlName[i], 0x00, sizeof(info.m_ChannlName[i]));
			strncpy(info.m_ChannlName[i], cfg_getnstr(cfgHttx, "Channel", i), sizeof(info.m_ChannlName[i])-1);
		}
	}

	cfg_free(cfg);
	return 0;
}

/** @fn C_IniServer::SaveHttxInfo
 *  @brief ������վͨѶ����
 *  @param[in] pStatus:���ýṹ��
 *  @param[in] name:�ļ���
 *  @return 0:successful, -1:failed
 */
int C_IniServer::SaveHttxInfo(S_STATUS *pStatus, const char *fname)
{
	char Buffer[32];
	string strFileInfo;

	strFileInfo.append("#note ��̨ͨѶ�����ļ�\r\n" "#see upchannl.ini\r\n");
	strFileInfo.append("#EAC:httx[1-6]������վͨ��[1-8],httx7���ñ��ص��Կ�,httx8���ú���,�ڹ�ʽhttx4���ú���\r\n");
	strFileInfo.append("#FK|JC|PB:httx1������վͨ��,httx2���ñ��ص��Կ�,httx3���ú���,httx4���ò��Կ�\r\n\r\n");

	for(int i = 0; i < HTTX_PROCESS_MAX; i++)
	{
		S_HttxInfo *pInfo = &pStatus->m_HttxInfo[i];

		sprintf(Buffer, "%d", i+1);
		strFileInfo.append("Httx httx").append(Buffer).append("\r\n{");
		strFileInfo.append("\r\n\tEnable=").append(C_SYSCTRL::GetProcessEnable(GetHttxProcessName(i))?"true":"false");
		strFileInfo.append("\r\n\tReportEnable=").append(pInfo->m_ReportEnable==true?"true":"false");

		sprintf(Buffer,"%d", pInfo->m_ResendTimes);
		strFileInfo.append("\r\n\tResendTimes=").append(Buffer).append(" #�ط�����");
		sprintf(Buffer,"%d", pInfo->m_WaitResponseTimeoutS);
		strFileInfo.append("\r\n\tWaitResponseTimeoutS=").append(Buffer).append(" #�ն˵ȴ��Ӷ�վ��Ӧ�ĳ�ʱʱ��");
		sprintf(Buffer,"%d", pInfo->m_LoginPeriodS);
		strFileInfo.append("\r\n\tLoginPeriodS=").append(Buffer).append(" #0��ʾ��Ҫ��½");
		sprintf(Buffer,"%d", pInfo->m_HeartBeatPeriodM);
		strFileInfo.append("\r\n\tHeartBeatPeriodM=").append(Buffer).append(" #0��ʾ��Ҫ����");
		sprintf(Buffer,"%d", pInfo->m_MonTrafficLimit);
		strFileInfo.append("\r\n\tMonTrafficLimit=").append(Buffer).append(" #0��ʾ����Ҫ��������");
		sprintf(Buffer,"%d", pInfo->m_MsgConfirmCode);
		strFileInfo.append("\r\n\tMsgConfirmCode=").append(Buffer).append(" #0��ʾ��������Ϣ��֤ 1-255:��Ϣ��֤������");

		strFileInfo.append("\r\n\tChannel={");
		for (int i = 0; i < HTTX_CHANNL_MAX; i++)
		{
			if (pInfo->m_ChannlName[i][0])
				strFileInfo.append(pInfo->m_ChannlName[i]).append(",");
		}
		if (*(strFileInfo.end()-1) == ',')
			strFileInfo.erase(strFileInfo.end()-1);
		strFileInfo.append("} #��ͨ����(����), ����ͨ����1(��ѡ), ����ͨ����2(��ѡ)");
		strFileInfo.append("\r\n}\r\n\r\n");

	}
	return C_IniBase::SaveSetFile(strFileInfo, fname);
}

const char *C_IniServer::GetHttxProcessName(int SN)
{
	switch (SN)
	{
	case MP_HTTX_1: return MP_PROCESS_HTTX1;
	case MP_HTTX_2: return MP_PROCESS_HTTX2;
	case MP_HTTX_3: return MP_PROCESS_HTTX3;
	case MP_HTTX_4: return MP_PROCESS_HTTX4;
	}
	return MP_PROCESS_HTTX1;
}

int C_IniServer::PortSettingParse(struct S_CHANNEL *pChannl, const char *pSetting)
{
	if (pChannl->m_Type == CHANNEL_TYPE_NET)
	{
		int ok = pChannl->m_PortSet.m_NetSet.Parse(pSetting);
		if (ok == -1)
			loget<<"Error netsetting("<<pSetting<<") format,please use format[IP:PortNo,Mode,Property]"<<endl;
		return ok;
	}
	else if (pChannl->m_Type == CHANNEL_TYPE_COM)
	{
		int ok = pChannl->m_PortSet.m_ComSet.Parse(pSetting);
		if (ok == -1)
			loget<<"Error comsetting("<<pSetting<<") format,please use format[PortNo,Baud.DataBit.StopBit.CheckForm,PortType,CD[NoCD]]"<<endl;
		return ok;
	}
	else if (pChannl->m_Type == CHANNEL_TYPE_PSTN)
	{
		int ok = pChannl->m_PortSet.m_PstnSet.Parse(pSetting);
		if (ok == -1)
			loget<<"Error pstnsetting("<<pSetting<<") format,please use format[PortNo,Baud.DataBit.StopBit.CheckForm,PowerMethed,DialMethed]"<<endl;
		return ok;
	}
	return -1;
}

int C_IniServer::PortSettingPrint(struct S_CHANNEL *pChannl, char *pSetting)
{
	if (pChannl->m_Type == CHANNEL_TYPE_NET)
	{
		return pChannl->m_PortSet.m_NetSet.Print(pSetting);
	}
	else if (pChannl->m_Type == CHANNEL_TYPE_COM)
	{
		return pChannl->m_PortSet.m_ComSet.Print(pSetting);
	}
	else if (pChannl->m_Type == CHANNEL_TYPE_PSTN)
	{
		return pChannl->m_PortSet.m_PstnSet.Print(pSetting);
	}
	return -1;
}


/** @brief PPPͨѶ�������ýṹ�� */
cfg_opt_t OPT_PPP_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_true,CFGF_NOCASE),
	CFG_INT("ModemPowerOffDelayS", 2, CFGF_NONE),
	CFG_INT("ModemPowerOnDelayS", 4, CFGF_NONE),
	CFG_INT("RefreshSignalInterS", 300, CFGF_NONE),
	CFG_INT("RedialInterM", 30, CFGF_NONE),
	CFG_INT("RedialTimesPerDay", 3, CFGF_NONE),
	CFG_INT("CheckInterM", 0, CFGF_NONE),
	CFG_INT("CheckFailTimes", 0, CFGF_NONE),
	CFG_STR("MasterPhone", "10086", CFGF_NONE),
	CFG_STR("SmsCenterNo", "13988998899", CFGF_NONE),
	CFG_STR("Apn", "CMNET", CFGF_NONE),
	CFG_STR("TeleNo", "*99***1#", CFGF_NONE),
	CFG_STR("UserName", "", CFGF_NONE),
	CFG_STR("Password", "", CFGF_NONE),
	CFG_STR("MasterPhone", "10086", CFGF_NONE),
	CFG_STR_LIST("ApnList", "{CMNET}", CFGF_NONE),
	CFG_STR_LIST("Channel", "{}", CFGF_NONE),
	CFG_END()
};
cfg_opt_t OPT_PPPS_SETTINGS[] =
{
	CFG_SEC("Ppp", OPT_PPP_SETTINGS, CFGF_MULTI |CFGF_NOCASE),
	CFG_END()
};

/** @fn C_IniServer::LoadPppInfo
 *  @brief ������վͨѶ����
 *  @param[out] pStatus:���ýṹ��
 *  @param[in] fname:�ļ���
 *  @return 0:successful, -1:failed
 */
int C_IniServer::LoadPppInfo(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfg  = cfg_init(OPT_PPPS_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSetFile(cfg, fname)!=0)
	{
		cfg_free(cfg);
		return -1;
	}

	for(unsigned int i = 0; i < cfg_size(cfg, "Ppp"); i++)
	{
		cfg_t* cfgPpp = cfg_getnsec(cfg, "Ppp", i);
		C_SYSCTRL::EnableProcess(MP_PROCESS_PPP, cfg_getbool(cfgPpp, "Enable")? true:false);
		S_PppInfo& info = pStatus->m_PppInfo;

		info.Init();
		info.m_ModemPowerOffDelayS = cfg_getint(cfgPpp,"ModemPowerOffDelayS");
		info.m_ModemPowerOnDelayS = cfg_getint(cfgPpp,"ModemPowerOnDelayS");
		info.m_RefreshSignalInterS = (S_WORD)cfg_getint(cfgPpp,"RefreshSignalInterS"); ///=300 #ˢ�������źż����ȱʡ300��
		info.m_RedialInterM = (S_WORD)cfg_getint(cfgPpp,"RedialInterM");
		info.m_RedialTimesPerDay = (S_WORD)cfg_getint(cfgPpp,"RedialTimesPerDay");
		info.m_DialInfo.m_CheckInterM = (S_WORD)cfg_getint(cfgPpp,"CheckInterM");
		info.m_DialInfo.m_CheckFailTimes = (S_WORD)cfg_getint(cfgPpp,"CheckFailTimes");
		strncpy(info.m_DialInfo.m_Apn, cfg_getstr(cfgPpp, "Apn"), sizeof(info.m_DialInfo.m_Apn)-1);
		strncpy(info.m_DialInfo.m_TeleNo, cfg_getstr(cfgPpp, "TeleNo"), sizeof(info.m_DialInfo.m_TeleNo)-1);
		strncpy(info.m_DialInfo.m_User, cfg_getstr(cfgPpp, "UserName"), sizeof(info.m_DialInfo.m_User)-1);
		strncpy(info.m_DialInfo.m_Pwd, cfg_getstr(cfgPpp, "Password"), sizeof(info.m_DialInfo.m_Pwd)-1);
		strncpy(info.m_MasterPhone, cfg_getstr(cfgPpp, "MasterPhone"), sizeof(info.m_MasterPhone)-1);
		strncpy(info.m_SmsCenterNo, cfg_getstr(cfgPpp, "SmsCenterNo"), sizeof(info.m_SmsCenterNo)-1);

		for (unsigned int i = 0; i < cfg_size(cfgPpp, "ApnList") && i < PPP_APN_COUNT_MAX; i++)
		{
			strncpy(info.m_ApnList[i], cfg_getnstr(cfgPpp, "ApnList", i), sizeof(info.m_ApnList[i])-1);
		}
		for (unsigned int j = 0; j < cfg_size(cfgPpp, "Channel") && j < PPP_CHANNL_MAX; j++)
		{
			strncpy(info.m_ChannlName[j], cfg_getnstr(cfgPpp, "Channel", j), sizeof(info.m_ChannlName[j])-1);
		}

		/* �����QT�ڴ�ͨѶ������APN ��Ϣ*/
		struct SIpcCommParam CommParam;
		CommParam = IPCSHAREMEM.m_IpcTerminalInfo.GetCommParm();
		/* apn */
		memcpy(CommParam.m_Apn,info.m_DialInfo.m_Apn,sizeof(info.m_DialInfo.m_Apn));
		IPCSHAREMEM.m_IpcTerminalInfo.SetCommParm(CommParam);
		IPCSHAREMEM.Update();
	}

	cfg_free(cfg);
	return 0;
}

/** @fn C_IniServer::SavePppInfo
 *  @brief ������վͨѶ����
 *  @param[in] pStatus:���ýṹ��
 *  @param[in] name:�ļ���
 *  @return 0:successful, -1:failed
 */
int C_IniServer::SavePppInfo(S_STATUS *pStatus, const char *fname)
{
	char Buffer[32];
	string strFileInfo;

	strFileInfo.append("#note PPP����ͨѶ�����ļ�\r\n" "#see upchannl.ini\r\n\r\n");

	S_PppInfo *pInfo = &pStatus->m_PppInfo;
	strFileInfo.append("Ppp\r\n{");
	strFileInfo.append("\r\n\tEnable=").append(C_SYSCTRL::GetProcessEnable(MP_PROCESS_PPP)?"true":"false");

	sprintf(Buffer,"%d", pInfo->m_ModemPowerOffDelayS);
	strFileInfo.append("\r\n\tModemPowerOffDelayS=").append(Buffer);
	sprintf(Buffer,"%d", pInfo->m_ModemPowerOnDelayS);
	strFileInfo.append("\r\n\tModemPowerOnDelayS=").append(Buffer);

	sprintf(Buffer,"%d", pInfo->m_RefreshSignalInterS);
	strFileInfo.append("\r\n\tRefreshSignalInterS=").append(Buffer).append(" #ˢ�������źż����ȱʡ300��");
	sprintf(Buffer,"%d", pInfo->m_RedialInterM);
	strFileInfo.append("\r\n\tRedialInterM=").append(Buffer).append(" #�ز������ȱʡ30����");
	sprintf(Buffer,"%d", pInfo->m_RedialTimesPerDay);
	strFileInfo.append("\r\n\tRedialTimesPerDay=").append(Buffer).append(" #���ز�����,ȱʡ3��,Ϊ0����");
	sprintf(Buffer,"%d", pInfo->m_DialInfo.m_CheckInterM);
	strFileInfo.append("\r\n\tCheckInterM=").append(Buffer).append(" #pppd�������ȱʡΪ0");
	sprintf(Buffer,"%d", pInfo->m_DialInfo.m_CheckFailTimes);
	strFileInfo.append("\r\n\tCheckFailTimes=").append(Buffer).append(" #pppdʧ�ܴ�����ȱʡΪ0");

	strFileInfo.append("\r\n\tMasterPhone=\"").append(pInfo->m_MasterPhone).append("\"");
	strFileInfo.append("\r\n\tSmsCenterNo=\"").append(pInfo->m_SmsCenterNo).append("\"");
	strFileInfo.append("\r\n\tApn=\"").append(pInfo->m_DialInfo.m_Apn).append("\"");
	strFileInfo.append("\r\n\tTeleNo=\"").append(pInfo->m_DialInfo.m_TeleNo).append("\"");
	strFileInfo.append("\r\n\tUserName=\"").append(pInfo->m_DialInfo.m_User).append("\"");
	strFileInfo.append("\r\n\tPassword=\"").append(pInfo->m_DialInfo.m_Pwd).append("\"");

	strFileInfo.append("\r\n\tApnList={");
	for (int i = 0; i < PPP_APN_COUNT_MAX; i++)
	{
		if (pInfo->m_ApnList[i][0])
			strFileInfo.append(pInfo->m_ApnList[i]).append(",");
	}
	if (*(strFileInfo.end()-1) == ',')
		strFileInfo.erase(strFileInfo.end()-1);
	strFileInfo.append("} #ͨ��APN�б�");

	strFileInfo.append("\r\n\tChannel={");
	for (int j = 0; j < PPP_CHANNL_MAX; j++)
	{
		if (pInfo->m_ChannlName[j][0])
			strFileInfo.append(pInfo->m_ChannlName[j]).append(",");
	}
	if (*(strFileInfo.end()-1) == ',')
		strFileInfo.erase(strFileInfo.end()-1);
	strFileInfo.append("} #MODEMͨ��,GPRSͨ��,AT����ͨ��,SMSͨ��");
	strFileInfo.append("\r\n}\r\n\r\n");

	return C_IniBase::SaveSetFile(strFileInfo, fname);
}

/** @brief LCD�������ýṹ�� */
cfg_opt_t OPT_LCD_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_true,CFGF_NOCASE),
	CFG_STR("TermName", "***̨��������", CFGF_NONE),
	CFG_INT("LCDPassword", 0, CFGF_NONE),
	CFG_INT("LCDGreyScale", 195, CFGF_NONE),
	CFG_INT("LCDBrightness", 100, CFGF_NONE),
	CFG_INT("LCDContrast", 80, CFGF_NONE),
	CFG_INT("LCDDirection", 0, CFGF_NONE),
	CFG_INT("LCDLightTimeoutS", 60, CFGF_NONE),
	CFG_INT("BackDesktopTimeoutS", 40, CFGF_NONE),
	CFG_END()
};
cfg_opt_t OPT_LCDS_SETTINGS[] =
{
	CFG_SEC("Lcd", OPT_LCD_SETTINGS, CFGF_MULTI |CFGF_NOCASE),
	CFG_END()
};

/** @fn C_IniServer::LoadLcdInfo
 *  @brief ������վͨѶ����
 *  @param[out] pStatus:���ýṹ��
 *  @param[in] fname:�ļ���
 *  @return 0:successful, -1:failed
 */
int C_IniServer::LoadLcdInfo(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfg  = cfg_init(OPT_LCDS_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSetFile(cfg, fname)!=0)
	{
		cfg_free(cfg);
		return -1;
	}

	for(unsigned int i = 0; i < cfg_size(cfg, "Lcd"); i++)
	{
		cfg_t* cfgLcd = cfg_getnsec(cfg, "Lcd", i);
		C_SYSCTRL::EnableProcess(MP_PROCESS_LCD, cfg_getbool(cfgLcd, "Enable")? true:false);
		S_LcdInfo& info = pStatus->m_LcdInfo;

		info.Init();
		info.m_LCDPassword = cfg_getint(cfgLcd, "LCDPassword"); ///< =0 #Һ����������Ȩ������
		if (info.m_LCDPassword < 0 || info.m_LCDPassword > 999999)
			info.m_LCDPassword = 0;

		info.m_LCDGreyScale = (S_BYTE)cfg_getint(cfgLcd, "LCDGreyScale"); ///< =195 #Һ���Ҷ�ֵ
		info.m_LCDBrightness = (S_BYTE)cfg_getint(cfgLcd, "LCDBrightness"); ///=100 #Һ����������
		info.m_LCDContrast = (S_BYTE)cfg_getint(cfgLcd, "LCDContrast"); ///< =80 #Һ���Աȶ� 0-99
		info.m_LCDDirection = (S_BYTE)cfg_getint(cfgLcd, "LCDDirection"); ///< =0 #Һ����ת���� 0-3

		info.m_LCDLightTimeoutS = cfg_getint(cfgLcd, "LCDLightTimeoutS"); ///< =60 #Һ������Ƶ���ʱ��
		if (info.m_LCDLightTimeoutS < 0 || info.m_LCDLightTimeoutS > 5*60)
			info.m_LCDLightTimeoutS = 60;
		info.m_BackDesktopTimeoutS = cfg_getint(cfgLcd, "BackDesktopTimeoutS"); ///< =40 #���������ʱ��
		if (info.m_BackDesktopTimeoutS < 0 || info.m_BackDesktopTimeoutS > 5*60)
			info.m_BackDesktopTimeoutS = 40;
		strncpy(info.m_TermName, cfg_getstr(cfgLcd, "TermName"), sizeof(info.m_TermName)-1);///< =***̨��������
	}

	cfg_free(cfg);
	return 0;
}

/** @fn C_IniServer::SaveLcdInfo
 *  @brief ������վͨѶ����
 *  @param[in] pStatus:���ýṹ��
 *  @param[in] name:�ļ���
 *  @return 0:successful, -1:failed
 */
int C_IniServer::SaveLcdInfo(S_STATUS *pStatus, const char *fname)
{
	char Buffer[32];
	string strFileInfo;

	strFileInfo.append("#note LCD�����ļ�\r\n\r\n");

	S_LcdInfo *pInfo = &pStatus->m_LcdInfo;
	strFileInfo.append("Lcd\r\n{");
	strFileInfo.append("\r\n\tEnable=").append(C_SYSCTRL::GetProcessEnable(MP_PROCESS_LCD)?"true":"false");

	strFileInfo.append("\r\n\tTermName=").append(pInfo->m_TermName);
	sprintf(Buffer,"%d", pInfo->m_LCDPassword);
	strFileInfo.append("\r\n\tLCDPassword=").append(Buffer).append(" #Һ����������Ȩ������");
	sprintf(Buffer,"%d", pInfo->m_LCDGreyScale);
	strFileInfo.append("\r\n\tLCDGreyScale=").append(Buffer).append(" #Һ���Ҷ�ֵ 0-99");
	sprintf(Buffer,"%d", pInfo->m_LCDBrightness);
	strFileInfo.append("\r\n\tLCDBrightness=").append(Buffer).append(" #Һ���������� 0-99");
	sprintf(Buffer,"%d", pInfo->m_LCDContrast);
	strFileInfo.append("\r\n\tLCDContrast=").append(Buffer).append(" #Һ���Աȶ� 0-99");
	sprintf(Buffer,"%d", pInfo->m_LCDDirection);
	strFileInfo.append("\r\n\tLCDDirection=").append(Buffer).append(" #Һ����ת���� 0-3");
	sprintf(Buffer,"%d", pInfo->m_LCDLightTimeoutS);
	strFileInfo.append("\r\n\tLCDLightTimeoutS=").append(Buffer).append(" #Һ������Ƶ���ʱ��");
	sprintf(Buffer,"%d", pInfo->m_BackDesktopTimeoutS);
	strFileInfo.append("\r\n\tBackDesktopTimeoutS=").append(Buffer).append(" #���������ʱ��");

	strFileInfo.append("\r\n}\r\n\r\n");

	return C_IniBase::SaveSetFile(strFileInfo, fname);
}

/** @brief ���ɼ����ýṹ�� */
cfg_opt_t OPT_DBCJ_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_true,CFGF_NOCASE),
	CFG_STR("GatherStdDate","2009-10-14", CFGF_NOCASE),
	CFG_STR("GatherStdTime","00:00:00", CFGF_NOCASE),
	CFG_STR("GatherPeriodUnit","minute", CFGF_NOCASE),
	CFG_INT("GatherPeriodVal", 60, CFGF_NOCASE),
	CFG_INT("AlmPollCycleM", 2, CFGF_NONE),
	CFG_INT_LIST("VipBeginTime", "{}", CFGF_NONE),
	CFG_STR("DayBeginTime", "1:10", CFGF_NONE),
	CFG_STR("DayVolFreezeTime", "1:10", CFGF_NONE),
	CFG_INT("MonBeginDate", 1, CFGF_NONE),
	CFG_STR("MonBeginTime", "1:30", CFGF_NONE),
	CFG_INT("MonVolFreezeDate", 1, CFGF_NONE),
	CFG_STR("MonVolFreezeTime", "1:30", CFGF_NONE),
	CFG_STR_LIST("Channel", "{}", CFGF_NONE),
	CFG_END()
};
cfg_opt_t OPT_DBCJ_PARAM[] =
{
	CFG_INT("MeterAutoUpdate", 0, CFGF_NONE),
	CFG_STR("MeterOperator", "0", CFGF_NONE),
	CFG_STR("LineLossRate", "1.1", CFGF_NONE),
	CFG_STR_LIST("GatherMP", "{0-1200}", CFGF_NONE),
	CFG_END()
};
cfg_opt_t OPT_DBCJS_SETTINGS[] =
{
	CFG_SEC("Param", OPT_DBCJ_PARAM, CFGF_MULTI | CFGF_NOCASE),
	CFG_SEC("Dbcj", OPT_DBCJ_SETTINGS, CFGF_MULTI | CFGF_TITLE | CFGF_NOCASE),
	CFG_END()
};

/** @fn C_IniServer::LoadDbcjInfo
 *  @brief ���ص��ɼ���������
 *  @param[out] pStatus:���ýṹ��
 *  @param[in] fname:�ļ���
 *  @return 0:successful, -1:failed
 */
int C_IniServer::LoadDbcjInfo(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfg  = cfg_init(OPT_DBCJS_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSetFile(cfg, fname)!=0)
	{
		cfg_free(cfg);
		return -1;
	}

	cfg_t* cfgParam = cfg_getnsec(cfg, "Param", 0);
	if (cfgParam != NULL)
	{
		S_DbcjParam &Param = pStatus->m_DbcjParam;
		Param.m_MeterAutoUpdate = (S_BYTE)cfg_getint(cfgParam, "MeterAutoUpdate");
		sscanf(cfg_getstr(cfgParam, "MeterOperator"), "%08X", &Param.m_Operator);

		C_BCD bcdLineLossRate(cfg_getstr(cfgParam, "LineLossRate"));
		const S_BYTE *pBcd = bcdLineLossRate.GetBcdData();
		memcpy(&Param.m_LineLossRate, pBcd+1, 2); 
		Param.m_LineLossRate &= 0xFFF0; ///��ʽΪNN.N0

		S_MPBS AutoGatherMPBS;
		for (unsigned int j = 0; j < cfg_size(cfgParam, "GatherMP"); j++)
			AutoGatherMPBS.ParseString(cfg_getnstr(cfgParam, "GatherMP", j));
		S_MpInfos &MpInfos = pStatus->m_MpInfos;
		for (int k = 0; k < CONS_MP_COUNT_MAX; k++)
		{
			if (AutoGatherMPBS.test(k) == true) MpInfos.m_Item[k].m_AutoGather = 1;// "1"��ʾͶ����"0"��ʾͣ��
			else MpInfos.m_Item[k].m_AutoGather = 0;
		}
	}

	int m = 0, n = 0;
	for(unsigned int i = 0; i < cfg_size(cfg, "Dbcj"); i++)
	{
		int DbcjID = 0;
		cfg_t* cfgDbcj = cfg_getnsec(cfg, "Dbcj", i);
		sscanf(cfg_title(cfgDbcj), "dbcj%d", &DbcjID);
		if (DbcjID < 1 || DbcjID > DBCJ_PROCESS_MAX)
			continue;

		S_DbcjInfo& info = pStatus->m_DbcjInfo[DbcjID - 1];

		info.Init();
		C_SYSCTRL::EnableProcess(GetDbcjProcessName(DbcjID - 1), cfg_getbool(cfgDbcj, "Enable")? true:false);

		S_DATE date;
		sscanf(cfg_getstr(cfgDbcj,"GatherStdDate"), "%d-%d-%d", &date.m_Year, &date.m_Mon, &date.m_Day);
		sscanf(cfg_getstr(cfgDbcj,"GatherStdTime"), "%d:%d:%d", &date.m_Hour, &date.m_Min, &date.m_Sec);
		info.m_GatherStdTime = C_TIME::MakeTime(date);
		info.m_GatherPeriodUnit = C_TIME::GetTimeUnit(cfg_getstr(cfgDbcj,"GatherPeriodUnit"));
		info.m_GatherPeriodVal = (S_BYTE)cfg_getint(cfgDbcj,"GatherPeriodVal");

		info.m_AlmPollCycleM = (INT8U)cfg_getint(cfgDbcj, "AlmPollCycleM");
		info.m_VipBeginTime[0] = (INT8U)cfg_getnint(cfgDbcj, "VipBeginTime", 0);
		info.m_VipBeginTime[1] = (INT8U)cfg_getnint(cfgDbcj, "VipBeginTime", 1);

		m = 0, n = 0;
		sscanf(cfg_getstr(cfgDbcj, "DayBeginTime"), "%d:%d", &m, &n);
		info.m_DayBeginTime[0] = m, info.m_DayBeginTime[1] = n;

		m = 0, n = 0;
		sscanf(cfg_getstr(cfgDbcj, "DayVolFreezeTime"), "%d:%d", &m, &n);
		info.m_DayVolFreezeTime[0] = m, info.m_DayVolFreezeTime[1] = n;

		m = 0, n = 0;
		sscanf(cfg_getstr(cfgDbcj, "MonBeginTime"), "%d:%d", &m, &n);
		info.m_MonBeginTime[0] = m, info.m_MonBeginTime[1] = n;
		info.m_MonBeginDate = (INT8U)cfg_getint(cfgDbcj, "MonBeginDate");

		m = 0, n = 0;
		sscanf(cfg_getstr(cfgDbcj, "MonVolFreezeTime"), "%d:%d", &m, &n);
		info.m_MonVolFreezeTime[0] = m, info.m_MonVolFreezeTime[1] = n;
		info.m_MonVolFreezeDate = (INT8U)cfg_getint(cfgDbcj, "MonVolFreezeDate");

		for (unsigned int j = 0; j < cfg_size(cfgDbcj, "Channel") && j < DBCJ_CHANNL_MAX; j++)
		{
			strncpy(info.m_ChannlName[j], cfg_getnstr(cfgDbcj, "Channel", j), sizeof(info.m_ChannlName[j])-1);
		}
	}

	cfg_free(cfg);
	return 0;
}

const char *C_IniServer::GetDbcjProcessName(int SN)
{
	switch (SN)
	{
	case MP_DBCJ_1: return MP_PROCESS_DBCJ1;
	case MP_DBCJ_2: return MP_PROCESS_DBCJ2;
	case MP_DBCJ_3: return MP_PROCESS_DBCJ3;
	case MP_DBCJ_4: return MP_PROCESS_DBCJ4;
	}
	return MP_PROCESS_DBCJ1;
}

/** @fn C_IniServer::SaveDbcjInfo
 *  @brief ������ɼ���������
 *  @param[out] pStatus:���ýṹ��
 *  @param[in] fname:�ļ���
 *  @return 0:successful, -1:failed
 */
int C_IniServer::SaveDbcjInfo(S_STATUS *pStatus, const char *fname)
{
	char Buffer[32];
	string strFileInfo;

 	strFileInfo.append("#note ���вɱ����ò���\r\n" "#see dnchannl.ini\r\n");
 	strFileInfo.append("#EAC:dbcj[1-8]����bus_485[I-VIII]\r\n" "#JC|FK|PB:dbcj[1-3]����buf_485[I-III],dbcj4���ý��ɻ��ز�\r\n\r\n");

	S_DbcjParam &Param = pStatus->m_DbcjParam;
	strFileInfo.append("Param").append("\r\n{");

	sprintf(Buffer, "%d", Param.m_MeterAutoUpdate);
	strFileInfo.append("\r\n\tMeterAutoUpdate=").append(Buffer).append(" #�Զ��ѱ�");
	sprintf(Buffer, "%08X", Param.m_Operator);
	strFileInfo.append("\r\n\tMeterOperator=").append(Buffer).append(" #��Ʋ����ߴ���");

	C_BCD bcdLineLossRate((S_BYTE*)&Param.m_LineLossRate, 2, 2);
	string strBcd = bcdLineLossRate;
	strFileInfo.append("\r\n\tLineLossRate=").append(strBcd).append(" #��������Խ�ޱ�����ֵ,��ʽNN.N0");

	string strAutoGather;
	S_MPBS AutoGatherMPBS;
	S_MpInfos &MpInfos = pStatus->m_MpInfos;
	for (int k = 0; k < CONS_MP_COUNT_MAX; k++)
	{
		if (MpInfos.m_Item[k].m_AutoGather == 1)
			AutoGatherMPBS.set(k);//Ͷ������λ�����������ļ���ʾ
	}
	strFileInfo.append("\r\n\tGatherMP={");
	AutoGatherMPBS.PrintString(strAutoGather);
	strFileInfo.append(strAutoGather).append("} #Ͷ��/ͣ������");

	strFileInfo.append("\r\n}\r\n\r\n");

	for(int i = 0; i < DBCJ_PROCESS_MAX; i++)
	{
		S_DbcjInfo &info = pStatus->m_DbcjInfo[i];

		sprintf(Buffer, "%d", i+1);
		strFileInfo.append("Dbcj dbcj").append(Buffer).append("\r\n{");
		strFileInfo.append("\r\n\tEnable=").append(C_SYSCTRL::GetProcessEnable(GetDbcjProcessName(i))?"true":"false");

		S_DATE date;
		C_TIME::LocalTime(info.m_GatherStdTime, date);
		sprintf(Buffer, "%d-%d-%d", date.m_Year, date.m_Mon, date.m_Day);
		strFileInfo.append("\r\n\tGatherStdDate=").append(Buffer).append(" #�ɼ���׼��ʼ����");
		sprintf(Buffer, "%d:%d:%d", date.m_Hour, date.m_Min, date.m_Sec);
		strFileInfo.append("\r\n\tGatherStdTime=").append(Buffer).append(" #�ɼ���׼��ʼʱ��");
		strFileInfo.append("\r\n\tGatherPeriodUnit=").append(C_TIME::GetTimeUnit(info.m_GatherPeriodUnit)).append(" #�ɼ����ڵ�λ");
		sprintf(Buffer, "%d",info.m_GatherPeriodVal); 
		strFileInfo.append("\r\n\tGatherPeriodVal=").append(Buffer).append(" #�ɼ�����ֵ");

		sprintf(Buffer, "%d",info.m_AlmPollCycleM); 
		strFileInfo.append("\r\n\tAlmPollCycleM=").append(Buffer).append(" #�澯��ѯ����");
		sprintf(Buffer, "%d,%d",info.m_VipBeginTime[0],info.m_VipBeginTime[1]); 
		strFileInfo.append("\r\n\tVipBeginTime={").append(Buffer).append("} #�ص㻧����ɼ���ʼʱ�� ����");
		sprintf(Buffer, "%d:%d",info.m_DayBeginTime[0],info.m_DayBeginTime[1]); 
		strFileInfo.append("\r\n\tDayBeginTime=").append(Buffer).append(" #������ɼ���ʼʱ�� Сʱ:����");
		sprintf(Buffer, "%d:%d",info.m_DayVolFreezeTime[0],info.m_DayVolFreezeTime[1]); 
		strFileInfo.append("\r\n\tDayVolFreezeTime=").append(Buffer).append(" #�յ�ѹ�ϸ������ݶ��Ὺʼʱ�� Сʱ:����");
		sprintf(Buffer, "%d",info.m_MonBeginDate); 
		strFileInfo.append("\r\n\tMonBeginDate=").append(Buffer).append(" #������ɼ���ʼ����");
		sprintf(Buffer, "%d:%d",info.m_MonBeginTime[0],info.m_MonBeginTime[1]); 
		strFileInfo.append("\r\n\tMonBeginTime=").append(Buffer).append(" #������ɼ���ʼʱ�� Сʱ:����");
		sprintf(Buffer, "%d",info.m_MonVolFreezeDate); 
		strFileInfo.append("\r\n\tMonVolFreezeDate=").append(Buffer).append(" #�µ�ѹ�ϸ������ݶ��� �ɼ���ʼ����");
		sprintf(Buffer, "%d:%d",info.m_MonVolFreezeTime[0],info.m_MonVolFreezeTime[1]); 
		strFileInfo.append("\r\n\tMonVolFreezeTime=").append(Buffer).append(" #�µ�ѹ�ϸ������ݶ��Ὺʼʱ�� Сʱ:����");

		strFileInfo.append("\r\n\tChannel={");
		for (int i = 0; i < DBCJ_CHANNL_MAX; i++)
		{
			if (info.m_ChannlName[i][0])
				strFileInfo.append(info.m_ChannlName[i]).append(",");
		}
		if (*(strFileInfo.end()-1) == ',')
			strFileInfo.erase(strFileInfo.end()-1);
		strFileInfo.append("} #�ܱ��(����), �ֱ��(����), �ز���(����)");
		strFileInfo.append("\r\n}\r\n\r\n");
	}

	return C_IniBase::SaveSetFile(strFileInfo, fname);
}

/** @brief �����������ýṹ�� */
cfg_opt_t OPT_JLCY_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_true,CFGF_NOCASE),
	CFG_STR_LIST("Channel", "{}", CFGF_NONE),
	CFG_END()
};
cfg_opt_t OPT_JLCYS_SETTINGS[] =
{
	CFG_SEC("Jlcy", OPT_JLCY_SETTINGS, CFGF_MULTI | CFGF_NOCASE),
	CFG_END()
};

/** @fn C_IniServer::LoadDbcjInfo
 *  @brief ���ص��ɼ���������
 *  @param[out] pStatus:���ýṹ��
 *  @param[in] fname:�ļ���
 *  @return 0:successful, -1:failed
 */
int C_IniServer::LoadJlcyInfo(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfg  = cfg_init(OPT_JLCYS_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSetFile(cfg, fname)!=0)
	{
		cfg_free(cfg);
		return -1;
	}

	S_JlcyInfo& info = pStatus->m_JlcyInfo;
	info.Init();
	for(unsigned int i = 0; i < cfg_size(cfg, "Jlcy"); i++)
	{
		cfg_t* cfgJlcy = cfg_getnsec(cfg, "Jlcy", i);

		C_SYSCTRL::EnableProcess(MP_PROCESS_JLCY, cfg_getbool(cfgJlcy, "Enable")? true:false);
		
		for (unsigned int j = 0; j < cfg_size(cfgJlcy, "Channel") && j < JLCY_CHANNL_MAX; j++)
		{
			strncpy(info.m_ChannlName[j], cfg_getnstr(cfgJlcy, "Channel", j), sizeof(info.m_ChannlName[j])-1);
		}
	}

	cfg_free(cfg);
	return 0;
}

/** @fn C_IniServer::SaveJlcyInfo
 *  @brief ������ɼ���������
 *  @param[out] pStatus:���ýṹ��
 *  @param[in] fname:�ļ���
 *  @return 0:successful, -1:failed
 */
int C_IniServer::SaveJlcyInfo(S_STATUS *pStatus, const char *fname)
{
	string strFileInfo;

	S_JlcyInfo& info = pStatus->m_JlcyInfo;

 	strFileInfo.append("#note �����������ò���\r\n" "#see dnchannl.ini\r\n\r\n");

	strFileInfo.append("Jlcy").append("\r\n{");
	strFileInfo.append("\r\n\tEnable=").append(C_SYSCTRL::GetProcessEnable(MP_PROCESS_JLCY)?"true":"false");

	strFileInfo.append("\r\n\tChannel={");
	for (int i = 0; i < JLCY_CHANNL_MAX; i++)
	{
		if (info.m_ChannlName[i][0])
			strFileInfo.append(info.m_ChannlName[i]).append(",");
	}
	if (*(strFileInfo.end()-1) == ',')
		strFileInfo.erase(strFileInfo.end()-1);
	strFileInfo.append("} #����оƬ�˿�(����),���ɶ˿�(����),У��˿�(����)");
	strFileInfo.append("\r\n}\r\n\r\n");

	return C_IniBase::SaveSetFile(strFileInfo, fname);
}

/** @brief CT�������ýṹ�� */
cfg_opt_t OPT_CTCY_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_true, CFGF_NOCASE),
	CFG_END()
};
cfg_opt_t OPT_CTCYS_SETTINGS[] =
{
	CFG_SEC("Ctcy", OPT_CTCY_SETTINGS, CFGF_MULTI | CFGF_NOCASE),
	CFG_END()
};

/** @fn C_IniServer::LoadCtcyInfo
 *  @brief ����CT������������
 *  @param[out] pStatus:���ýṹ��
 *  @param[in] fname:�ļ���
 *  @return 0:successful, -1:failed
 */
int C_IniServer::LoadCtcyInfo(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfg  = cfg_init(OPT_CTCYS_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSetFile(cfg, fname)!=0)
	{
		cfg_free(cfg);
		return -1;
	}

	S_CtcyInfo& info = pStatus->m_CtcyInfo;
	info.Init();
	for(unsigned int i = 0; i < cfg_size(cfg, "Ctcy"); i++)
	{
		cfg_t* cfgCtcy = cfg_getnsec(cfg, "Ctcy", i);

		C_SYSCTRL::EnableProcess(MP_PROCESS_CTCY, cfg_getbool(cfgCtcy, "Enable")? true:false);
	}

	cfg_free(cfg);
	return 0;
}

/** @fn C_IniServer::SaveCtcyInfo
 *  @brief ����CT������������
 *  @param[out] pStatus:���ýṹ��
 *  @param[in] fname:�ļ���
 *  @return 0:successful, -1:failed
 */
int C_IniServer::SaveCtcyInfo(S_STATUS *pStatus, const char *fname)
{
	string strFileInfo;

	//S_CtcyInfo& info = pStatus->m_CtcyInfo;

 	strFileInfo.append("#note CT�������ò���\r\n" "#see dnchannl.ini\r\n\r\n");

	strFileInfo.append("Ctcy").append("\r\n{");
	strFileInfo.append("\r\n\tEnable=").append(C_SYSCTRL::GetProcessEnable(MP_PROCESS_CTCY)?"true":"false");
	strFileInfo.append("\r\n}\r\n\r\n");

	return C_IniBase::SaveSetFile(strFileInfo, fname);
}

/** @brief �������ò����ṹ�� */
cfg_opt_t OPT_FHKZ_SETTINGS[]=
{
	CFG_BOOL("Enable", cfg_true,CFGF_NOCASE),
	CFG_INT_LIST("SwitchType", "{}", CFGF_NOCASE),
	CFG_INT_LIST("SwitchInState", "{}", CFGF_NOCASE),
	CFG_INT_LIST("SwitchTurnTab", "{}", CFGF_NOCASE),
	CFG_INT("SwitchDelayTime",0000, CFGF_NOCASE),
	CFG_BOOL("BDFlag",cfg_false, CFGF_NOCASE),
	CFG_INT("TZOverLoadTimeSec",00, CFGF_NOCASE),
	CFG_INT("TZUnderLoadTimeSec",00, CFGF_NOCASE),
	CFG_INT("TZAlarmDelaySec",00, CFGF_NOCASE),
	CFG_STR("MinPower","", CFGF_NOCASE),
	CFG_END()
};
cfg_opt_t OPT_FHKZS_SETTINGS[] =
{
	CFG_SEC("Fhkz", OPT_FHKZ_SETTINGS, CFGF_MULTI | CFGF_NOCASE),
	CFG_END()
};

/** @fn ServerCfg::LoadFKCtrlInfo
 *  @brief ���ظ��ػ������ò���
 *  @author zhgx
 *  @param[out] oContext:���ýṹ��
 *  @param[in] name:�ļ���
 *  @return 0:successful, -1:failed
 */
int C_IniServer::LoadFhkzInfo(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfg = cfg_init(OPT_FHKZS_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSetFile(cfg, fname) != 0)
	{
		cfg_free(cfg);
		return -1;
	}

	cfg_t* cfgFhkz = cfg_getnsec(cfg, "Fhkz", 0);
	if (cfgFhkz == NULL)
	{
		cfg_free(cfg);
		return -1;
	}

	S_FhkzInfo& info = pStatus->m_FhkzInfo;
	info.Init();
	C_SYSCTRL::EnableProcess(MP_PROCESS_FHKZ, cfg_getbool(cfgFhkz, "Enable")? true:false);

	S_FKCtrlBaseInfo &FKCtrlBaseInfo =info.m_BaseInfo;

	//���ؽڵ�����
	for (unsigned int i = 0; i < cfg_size(cfgFhkz, "SwitchType") && i < CONS_MAX_FK_SWITCH; i++)
	{
		(cfg_getnint(cfgFhkz, "SwitchType",i)?FKCtrlBaseInfo.m_SwitchType.SetBit(i):FKCtrlBaseInfo.m_SwitchType.ClearBit(i));
	}
	//���뿪�ؽӵ�״̬
	for (unsigned int i = 0; i < cfg_size(cfgFhkz, "SwitchInState") && i < CONS_MAX_FK_SWITCH; i++)
	{
		(cfg_getnint(cfgFhkz, "SwitchInState",i)?FKCtrlBaseInfo.m_SwitchInState.SetBit(i):FKCtrlBaseInfo.m_SwitchInState.ClearBit(i));
	}
	//��������ִ�
	for (unsigned int i = 0; i < cfg_size(cfgFhkz, "SwitchTurnTab") && i < CONS_MAX_FK_SWITCH; i++)
	{
		(cfg_getnint(cfgFhkz, "SwitchTurnTab",i)?FKCtrlBaseInfo.m_SwitchTurnTab.SetBit(i):FKCtrlBaseInfo.m_SwitchTurnTab.ClearBit(i));
	}
	//ң����բʱ�� ��Լ��ʽhhmm
	FKCtrlBaseInfo.m_SwitchDelayTime =(unsigned short)cfg_getint(cfgFhkz, "SwitchDelayTime");
	//����״̬
	FKCtrlBaseInfo.m_BDFlag = (cfg_getbool(cfgFhkz, "BDFlag")==cfg_true?1:0);
	//�����趨ֵ��բ�Ĺ����ɳ���ʱ�� ��Լ��ʽhhmmss Ĭ��10����
	FKCtrlBaseInfo.m_TZOverLoadTimeSec =cfg_getint(cfgFhkz, "TZOverLoadTimeSec");
	//�����趨ֵ��բ�ָ��ĸ��ɻָ�����ʱ�� ��Լ��ʽhhmmss Ĭ��10����
	FKCtrlBaseInfo.m_TZUnderLoadTimeSec =cfg_getint(cfgFhkz, "TZUnderLoadTimeSec");
	//��բ�㾯��ʱ Ĭ��10����
	FKCtrlBaseInfo.m_TZAlarmDelaySec =(unsigned short)cfg_getint(cfgFhkz, "TZAlarmDelaySec");
	//��͸��� ��λkW  NNNNNN.NN 4�ֽ�
	C_BCD bcdMinPower(cfg_getstr(cfgFhkz, "MinPower"));
	const S_BYTE *pBcd = bcdMinPower.GetBcdData();
	memcpy(&FKCtrlBaseInfo.m_MinPower, pBcd+1, 4); 

	cfg_free(cfg);

	return 0;
}
/** @fn ServerCfg::SaveFKCtrlBaseInfo
 *  @brief ���渺�ػ������ò���
 *  @author zhgx
 *  @param[out] oContext:���ýṹ��
 *  @param[in] name:�ļ���
 *  @return 0:successful, -1:failed
 */
enum SwitchInfo
{
	SW_TYPE = 0,
	SW_INSTATE,
	SW_TURNTAB
};
int C_IniServer::SaveFhkzInfo(S_STATUS *pStatus, const char *fname)
{	
	char buffer[32];
	string strFileInfo;

	S_FhkzInfo& info = pStatus->m_FhkzInfo;
	S_FKCtrlBaseInfo &FKCtrlBaseInfo = info.m_BaseInfo;

 	strFileInfo.append("#note ���ɿ������ò���\r\n\r\n");
	strFileInfo.append("Fhkz").append("\r\n{");
	strFileInfo.append("\r\n\tEnable=").append(C_SYSCTRL::GetProcessEnable(MP_PROCESS_FHKZ)?"true":"false");

	for(INT8U i = 0;i<= SW_TURNTAB;i++)
	{
		string strTurnTab;
		switch(i)
		{
		case SW_TYPE:
			strFileInfo.append("\r\n\t#���ؽڵ�����").append("\r\n\tSwitchType={");
			FKCtrlBaseInfo.m_SwitchType.PrintString(strTurnTab, 0);
			break;
		case SW_INSTATE:
			strFileInfo.append("\r\n\t#���뿪�ؽӵ�״̬").append("\r\n\tSwitchInState={");
			FKCtrlBaseInfo.m_SwitchInState.PrintString(strTurnTab, 0);
			break;
		case SW_TURNTAB:
			strFileInfo.append("\r\n\t#��������ִ�").append("\r\n\tSwitchTurnTab={");
			FKCtrlBaseInfo.m_SwitchTurnTab.PrintString(strTurnTab, 0);
			break;
		default:
			return -1;
		}
	   if(strTurnTab.size()>CONS_MAX_FK_SWITCH)//ȥ�������
	   {
		   strTurnTab.erase((size_t)CONS_MAX_FK_SWITCH,strTurnTab.size()-CONS_MAX_FK_SWITCH);
	   }
		C_BaseFn::FormatDataWithDelimit(strTurnTab, 1, ',');
	   strFileInfo.append(strTurnTab).append("}");
	}

	strFileInfo.append("\r\n\t#ң����բʱ��");	
	sprintf(buffer, "%d", FKCtrlBaseInfo.m_SwitchDelayTime);
	strFileInfo.append("\r\n\tSwitchDelayTime=").append(buffer).append("#��λ����");

	strFileInfo.append("\r\n\t#����״̬");	
	strFileInfo.append("\r\n\tBDFlag=").append(C_IniBase::GetBoolKey(FKCtrlBaseInfo.m_BDFlag));

	strFileInfo.append("\r\n\t#�����趨ֵ��բ�Ĺ����ɳ���ʱ�� Ĭ��10����");	
	sprintf(buffer, "%d", FKCtrlBaseInfo.m_TZOverLoadTimeSec);
	strFileInfo.append("\r\n\tTZOverLoadTimeSec=").append(buffer);

	strFileInfo.append("\r\n\t#�����趨ֵ��բ�ָ��ĸ��ɻָ�����ʱ�� Ĭ��10����");	
	sprintf(buffer, "%d", FKCtrlBaseInfo.m_TZUnderLoadTimeSec);
	strFileInfo.append("\r\n\tTZUnderLoadTimeSec=").append(buffer);

	strFileInfo.append("\r\n\t#��բ�㾯��ʱ Ĭ��10����");	
	sprintf(buffer, "%d", FKCtrlBaseInfo.m_TZAlarmDelaySec);
	strFileInfo.append("\r\n\tTZAlarmDelaySec=").append(buffer);

	strFileInfo.append("\r\n\t#��͸���");	
	C_BCD bcdMinPower((S_BYTE*)&FKCtrlBaseInfo.m_MinPower, 6, 2);
	string strMinPower = bcdMinPower;
	strFileInfo.append("\r\n\tMinPower=").append(strMinPower);

	strFileInfo.append("\r\n}\r\n\r\n");

	return C_IniBase::SaveSetFile(strFileInfo, fname);
}



