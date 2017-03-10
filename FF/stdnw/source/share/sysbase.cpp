/** @file
 *  @brief 用户层系统基础类定义
 *  @author 
 *  @date 2011/12/06
 *  @version 0.1
 */
#include "sysbase.h"
#include "nwmsrule.h"
#include "status.h"
#include "ipcSharemem.h"

void S_SYSCFG::Init(void)
{
	memset(this, 0x00, sizeof(*this));
	m_InitIRDA = true;
	m_TermType = CONS_TERM_TYPE_JC;
	m_HeartBeatRetryTimes = 3;
	m_HeartBeatIdleTimeS = 0;
	m_ReportAgainIntervalM = 120;
	m_BatteryStandTimeM = 3;
	m_TermStandTimeS = 3;
	m_DrRouteMode = 3; ///< 东软路由模式3=3代;4=4代
}

/** @fn S_SYSCFG::SetRTUA
 *  @brief 
 *  @param[in] pRTUA:
 *  @note 
 */
void S_SYSCFG::SetRTUA(const char *pRTUA)
{
	C_RTUA myRTUA;
	myRTUA.ParseAscii(pRTUA);
	m_RTUA = myRTUA.GetRTUA();

	struct SIpcRtua rtua;
	rtua.m_Address = m_RTUA.m_Address;
	rtua.m_AreaCode = m_RTUA.m_AreaCode;
		
	IPCSHAREMEM.m_IpcTerminalInfo.SetRTUA(rtua);
	IPCSHAREMEM.Update();
}

/** @fn S_SYSCFG::GetRTUA
 *  @brief 
 *  @param[out] pRTUA:
 *  @note 
 *  @return pRTUA
 */
char *S_SYSCFG::GetRTUA(char *pRTUA)
{
	C_RTUA myRTUA(m_RTUA);
	return myRTUA.PrintAscii(pRTUA);
}

void S_SYSRUN::Init(void)
{
	memset(this, 0x00, sizeof(*this));
	m_timePowerCut = 1255449600;///< 2009-10-14 0:0:0 
}

/** @brief 表计规约名称 */
static const S_CFGITEM s_NwMeterRuleName[] =
{
 	{0, NW_MRULE_NAME_DLT645_1997 },
 	{1, NW_MRULE_NAME_DLT645_2007 },
 	{2, NW_MRULE_NAME_GUANGDONG_97 },
 	{3, NW_MRULE_NAME_GUANGDONG_07 },
 	{4, NW_MRULE_NAME_WEI_SHENG },
 	{5, NW_MRULE_NAME_LGR_B },
 	{6, NW_MRULE_NAME_LGR_D },
 	{7, NW_MRULE_NAME_EMDI },
 	{8, NW_MRULE_NAME_ABB },
 	{9, NW_MRULE_NAME_ACTARIS },
 	{10, NW_MRULE_NAME_EMAIL },
 	{11, NW_MRULE_NAME_ISKRA }
};

/** @brief 转表计规约名称 */
const char *C_NwMRuleName::MakeMeterRuleName(int ID)
{
	C_CfgItem itemRule(s_NwMeterRuleName, KGE_MEMBER_OF(s_NwMeterRuleName));
	return itemRule.MakeKey(ID);
}

/** @brief 转表计规约ID */
int C_NwMRuleName::MakeMeterRuleID(const char *pRuleName)
{
	C_CfgItem itemRule(s_NwMeterRuleName, KGE_MEMBER_OF(s_NwMeterRuleName));
	return itemRule.MakeValue(pRuleName);
}

/** @brief 转表计规约索引 */
int C_NwMRuleName::MakeMeterRuleIndex(int ID)
{
	C_CfgItem itemRule(s_NwMeterRuleName, KGE_MEMBER_OF(s_NwMeterRuleName));
	return itemRule.MakeIndex(ID);
}

/** @brief 转表计规约索引 */
int C_NwMRuleName::MakeMeterRuleIndex(const char *pRuleName)
{
	C_CfgItem itemRule(s_NwMeterRuleName, KGE_MEMBER_OF(s_NwMeterRuleName));
	return itemRule.MakeIndex(pRuleName);
}

/** @brief 串行化表计规约名称 */
S_WORD C_NwMRuleName::SerializeMeterRuleName(string &strRuleName, char fill, char delimit)
{
	string strItemKey;
	C_CfgItem itemRule(s_NwMeterRuleName, KGE_MEMBER_OF(s_NwMeterRuleName));
	if (itemRule.SerializeItemKey(strItemKey, delimit) == 0x00)
		return 0x00;

	strings strsRuleName;
	string strSuffix((size_t)1, delimit);
	strsRuleName.split(strItemKey, delimit);
	strsRuleName.Trim(fill);
	strsRuleName.AppendBack(strSuffix);
	strsRuleName.Serialize(strRuleName);

	return (S_WORD)strRuleName.size();
}

//
//表计信息
int S_MeterInfo::ExtractBaseInfo(S_MeterBase &MeterBaseInfo)const
{
	MeterBaseInfo.m_MP = m_MeasureNo;
	MeterBaseInfo.m_RuleID = m_RuleID;
	MeterBaseInfo.m_ComSet = m_PortSet;
	MeterBaseInfo.m_strRelay.assign(m_RelayAddr, sizeof(m_RelayAddr));

	if (m_RuleID == METER_RULE_GUANGDONG_97)
		MeterBaseInfo.m_RuleID = METER_RULE_GB645_1997;
	else if (m_RuleID == METER_RULE_GUANGDONG_07)
		MeterBaseInfo.m_RuleID = METER_RULE_GB645_2007;

	if (m_RuleID == METER_RULE_AC_GATHER)
		MeterBaseInfo.m_strAddress.assign(MP_PROCESS_JLCY);
	else
		MeterBaseInfo.m_strAddress.assign(m_Address, sizeof(m_Address));
	return 0;
}

bool S_MeterInfo::ValidRelayAddress(void)const
{
	string strInvalid00((size_t)RELAY_ADDRESS_LEN_MAX, (char)0);
	string strInvalidFF((size_t)RELAY_ADDRESS_LEN_MAX, (char)0xFF);
	if (strInvalid00.compare(0, RELAY_ADDRESS_LEN_MAX, m_RelayAddr, RELAY_ADDRESS_LEN_MAX) == 0)
		return false;
	if (strInvalidFF.compare(0, RELAY_ADDRESS_LEN_MAX, m_RelayAddr, RELAY_ADDRESS_LEN_MAX) == 0)
		return false;
	return true;
}

bool S_MeterInfo::SetRelayAddress(const char *pstrAddr)
{
	return SetAddress(pstrAddr, m_RelayAddr);
}
char* S_MeterInfo::GetRelayAddress(char *pstrAddr)const
{
	return GetAddress(m_RelayAddr, pstrAddr);
}
bool S_MeterInfo::SetMeterAddress(const char *pstrAddr)
{
	return SetAddress(pstrAddr, m_Address);
}
char* S_MeterInfo::GetMeterAddress(char *pstrAddr)const
{
	return GetAddress(m_Address, pstrAddr);
}

bool S_MeterInfo::SetMeterPassword(const char *pstrPwd)
{
	if (pstrPwd == NULL)
		return false;
	size_t len = strlen(pstrPwd);
	if (len > METER_PASSWORD_LEN_MAX * 2)
		return false;

	char Pwd16B[METER_PASSWORD_LEN_MAX * 2 + 4];
	memset(Pwd16B, '0', sizeof(Pwd16B));
	memcpy(Pwd16B, pstrPwd, len);
	for(int i=0; i<METER_PASSWORD_LEN_MAX; i+=1)
	{
		INT8U h = kge_Char2Hex(*(Pwd16B+2*i+0));
		INT8U l = kge_Char2Hex(*(Pwd16B+2*i+1));
		m_Password[i] = (h<<4) | l;
	}
	return true;
}

char *S_MeterInfo::GetMeterPassword(char *pstrPwd)const
{
	if (pstrPwd == NULL)
		return NULL;

	for (int i = 0; i < METER_PASSWORD_LEN_MAX; i++)
	{	
		pstrPwd[i*2+0] = kge_Hex2Char(m_Password[i]>>4 & 0x0F);
		pstrPwd[i*2+1] = kge_Hex2Char(m_Password[i]>>0 & 0x0F);
	}
	return pstrPwd;
}

bool S_MeterInfo::SetAddress(const char *pstrAddr, char *pBcdAddr)
{
	if (pstrAddr == NULL)
		return false;
	size_t len = strlen(pstrAddr);
	if (len > METER_ADDRESS_LEN_MAX * 2)
		return false;

	char Addr12B[16];
	char ReverseAddress[METER_ADDRESS_LEN_MAX+1];
	memset(Addr12B, '0', sizeof(Addr12B));
	strcpy(Addr12B+METER_ADDRESS_LEN_MAX*2-len, pstrAddr);
	for(int i=0;i<METER_ADDRESS_LEN_MAX;i+=1)
	{
		INT8U h = kge_Char2Hex(*(Addr12B+2*i+0));
		INT8U l = kge_Char2Hex(*(Addr12B+2*i+1));
		ReverseAddress[i] = (h<<4) | l;
	}
	reverse_copy(ReverseAddress, ReverseAddress+6, pBcdAddr);
	return true;
}

char* S_MeterInfo::GetAddress(const char *pBcdAddr, char *pstrAddr)const
{
	if (pstrAddr == NULL)
		return NULL;
	char ReverseAddress[METER_ADDRESS_LEN_MAX+1];
	reverse_copy(pBcdAddr, pBcdAddr+METER_ADDRESS_LEN_MAX, ReverseAddress);
	for (int i = 0; i < METER_ADDRESS_LEN_MAX; i++)
	{	
		pstrAddr[i*2+0] = kge_Hex2Char(ReverseAddress[i]>>4 & 0x0F);
		pstrAddr[i*2+1] = kge_Hex2Char(ReverseAddress[i]>>0 & 0x0F);
	}
	return pstrAddr;
}

int S_MeterInfo::PrintMeterInfo(string &strInfo)const
{
	char buffer[64];

	sprintf(buffer, "%04d", m_MeasureNo);
	strInfo.append(buffer).append("  ");
	
	strInfo.append(C_NwMRuleName::MakeMeterRuleName(m_RuleID)).append("  ");

	if (m_VipFlag == true)
		strInfo.append("Yes").append("  ");
	else
		strInfo.append("No ").append("  ");

	sprintf(buffer, "%02X", m_PortSet.m_PortNO - 1);
	strInfo.append(buffer).append("  ");

	memset(buffer, 0x00, sizeof(buffer));
	GetMeterAddress(buffer);
	strInfo.append(buffer).append("  ");

	memset(buffer, 0x00, sizeof(buffer));
	GetRelayAddress(buffer);
	strInfo.append(buffer).append("  ");

	strInfo.append("\n");
	return (int)strInfo.size();
}

int S_MeterInfos::PrintMeterInfo(S_WORD MinMP, S_WORD MaxMP, string &strInfo)const
{
	for (; MinMP <= MaxMP; MinMP++)
	{
		if (MinMP < CONS_METER_COUNT_MAX)
			m_Item[MinMP].PrintMeterInfo(strInfo);
	}
	return 0;
}

/*********************************************************/
/**			Class S_ForwardInfo member function define   */
/**											        	 */		
/*********************************************************/
void S_ForwardInfo::SetRTUA(const char *pName, const char *pRTUA)
{
	S_DWORD Address = 0x00;
	sscanf(pRTUA, "%08X", &Address);
	if (strcmp(pName, "SlaveAddr1") == 0)
		m_SlaveAddr[0] = Address;
	else if (strcmp(pName, "SlaveAddr2") == 0)
		m_SlaveAddr[1] = Address;
	else if (strcmp(pName, "SlaveAddr3") == 0)
		m_SlaveAddr[2] = Address;
	else if (strcmp(pName, "SlaveAddr4") == 0)
		m_SlaveAddr[3] = Address;
	else if (strcmp(pName, "MasterAddr") == 0)
		m_MasterAddr = Address;
	else if (strcmp(pName, "GatherAddr") == 0)
		m_GatherAddr = Address;
}

char *S_ForwardInfo::GetRTUA(const char *pName, char *pRTUA)
{
	if (strcmp(pName, "SlaveAddr1") == 0)
		sprintf(pRTUA, "%08X", m_SlaveAddr[0]);
	else if (strcmp(pName, "SlaveAddr2") == 0)
		sprintf(pRTUA, "%08X", m_SlaveAddr[1]);
	else if (strcmp(pName, "SlaveAddr3") == 0)
		sprintf(pRTUA, "%08X", m_SlaveAddr[2]);
	else if (strcmp(pName, "SlaveAddr4") == 0)
		sprintf(pRTUA, "%08X", m_SlaveAddr[3]);
	else if (strcmp(pName, "MasterAddr") == 0)
		sprintf(pRTUA, "%08X", m_MasterAddr);
	else if (strcmp(pName, "GatherAddr") == 0)
		sprintf(pRTUA, "%08X", m_GatherAddr);
	return pRTUA;
}

S_DWORD S_ForwardInfo::GetForwardedAddr(int i)
{
	if (i >= NW_FORWARD_ADDR_MAX)
		return m_GatherAddr;
	return m_SlaveAddr[i];
}

bool S_ForwardInfo::AnySlaveAddress(void)
{
	if (m_GatherAddr != 0xFFFFFFFF && m_GatherAddr != 0x00)
		return true;
	for (int i = 0; i < NW_FORWARD_ADDR_MAX; i++)
	{
		if (m_SlaveAddr[i] != 0xFFFFFFFF && m_SlaveAddr[i] != 0x00)
			return true;
	}
	return false;
}

//
// 终端数据
//
int S_GatherStatInfo::SetInfo(S_TIME timeTaskStart, S_WORD MP, bool fSuccess)
{
	if (m_timeTaskStart != timeTaskStart)
	{
		m_timeTaskStart = timeTaskStart, m_timeTaskFinish = 0x00;
		m_mpbsSuccess.reset(), m_mpbsFailure.reset();
	}

	if (fSuccess == true)
	{
		if (m_mpbsSuccess.test(MP) == false)
			m_mpbsSuccess.set(MP);
		if (m_mpbsFailure.test(MP) == true)
			m_mpbsFailure.reset(MP);
	}
	else
	{
		if (m_mpbsSuccess.test(MP) == true)
			m_mpbsSuccess.reset(MP);
		if (m_mpbsFailure.test(MP) == false)
			m_mpbsFailure.set(MP);
	}
	return 0;
}

int S_GatherStatInfo::SetTaskFinish(S_TIME timeTaskFinish)
{
	m_timeTaskFinish = timeTaskFinish;
	return 0;
}

int S_GatherStatInfos::SetInfo(int FreezeType, S_TIME timeTaskStart, S_WORD MP, bool fSuccess)
{
	switch (FreezeType)
	{
	case MSDI_FREEZE_HOUR:m_VipInfo.SetInfo(timeTaskStart, MP, fSuccess);break;
	case MSDI_FREEZE_DAY:m_DayInfo.SetInfo(timeTaskStart, MP, fSuccess);break;
	case MSDI_FREEZE_MONTH:m_MonInfo.SetInfo(timeTaskStart, MP, fSuccess);break;
	}
	if (fSuccess == true)
	{
		if (m_GatherFailedTimes[MP] != 0x00)
			m_GatherFailedTimes[MP] = 0x00;
	}
	else
	{
		m_GatherFailedTimes[MP] += 0x01;
	}
	return 0;
}

int S_GatherStatInfos::SetTaskFinish(int FreezeType, S_TIME timeTaskFinish)
{
	switch (FreezeType)
	{
	case MSDI_FREEZE_HOUR:
		return m_VipInfo.SetTaskFinish(timeTaskFinish);
	case MSDI_FREEZE_DAY:
		return m_DayInfo.SetTaskFinish(timeTaskFinish);
	case MSDI_FREEZE_MONTH:
		return m_MonInfo.SetTaskFinish(timeTaskFinish);
	}
	return 0;
}

//
// 告警
//
bool S_AlarmInfo::NeedRecord(S_DWORD DI)
{
	S_BYTE Idx = (S_BYTE)DI - 1;
	if ((DI&0x00FF0000) == 0x00) // 告警数据项
		return m_AlarmEven.test(Idx);
	//事件数据项
	return m_OnlyEvent.test(Idx);
}

//
//测量点结构信息
//
bool S_MpInfos::Test(S_WORD MP, S_WORD type) const
{
	if (MP >= CONS_MP_COUNT_MAX)
		return false;
	if (m_Item[MP].m_Type == type)
		return true;
	return false;
}

bool S_MpInfos::Test(S_WORD MP, S_WORD type, S_WORD SN) const
{
	if (MP >= CONS_MP_COUNT_MAX)
		return false;
	const S_MpInfo &MpInfo = m_Item[MP];
	if (MpInfo.m_Type == MP_TYPE_UNKNOW)
		return true;
	if (MpInfo.m_Type == type && MpInfo.m_SN == SN)
		return true;
	return false;
}

bool S_MpInfos::Set(S_WORD MP, S_WORD type, S_WORD SN)
{
	if (Test(MP, type, SN) == false)
		return false;

	S_MpInfo &MpInfo = m_Item[MP];
	MpInfo.m_Type = type, MpInfo.m_SN = SN;
	return true;
}

bool S_MpInfos::Reset(S_WORD MP, S_WORD type, S_WORD SN)
{
	if (Test(MP, type, SN) == false)
		return false;

	S_MpInfo &MpInfo = m_Item[MP];
	MpInfo.m_Type = MP_TYPE_UNKNOW, MpInfo.m_SN = 0x00;
	return true;
}

//
//测量点分类信息
//
void S_ALLMP::RefreshMeterInfo(S_BYTE PortZB, S_MeterInfos *pMeterInfos, S_MpInfos *pMpInfos)
{
	m_MultiMPBS.reset();
	m_VipMPBS.reset();
	m_MTBS.m_AllMPBS.reset();
	m_MTBS.m_485MPBS.reset();
	m_MTBS.m_VipZBMPBS.reset();
	m_MTBS.m_ZBMPBS.reset();
	for (S_WORD i = 0; i < CONS_METER_COUNT_MAX; i++)
	{
		S_MeterInfo &Meter = pMeterInfos->m_Item[i];
		if (Meter.m_Enable == false)
			continue;
		if ((S_BYTE)(Meter.m_Address[0]) == 0xEE)
			continue;
		S_MpInfo &MpInfo = pMpInfos->m_Item[Meter.m_MeasureNo];
		if (MpInfo.m_AutoGather == 0)
			continue;

		if (Meter.m_PortSet.m_PortNO == PortZB)
		{
			m_MTBS.m_ZBMPBS.set(Meter.m_MeasureNo);
			if (Meter.m_VipFlag == true)
				m_MTBS.m_VipZBMPBS.set(Meter.m_MeasureNo);
		}
		else
		{
			m_MTBS.m_485MPBS.set(Meter.m_MeasureNo);
		}
		if (Meter.m_FunType == METER_FUN_MULTI_FUN)
		{
			m_MultiMPBS.set(Meter.m_MeasureNo);
		}
		if (Meter.m_VipFlag == true)
		{
			m_VipMPBS.set(Meter.m_MeasureNo);
		}
		m_MTBS.m_AllMPBS.set(Meter.m_MeasureNo);
	}
	m_MultiMPBS.Stat();
	m_VipMPBS.Stat();
	m_MTBS.m_485MPBS.Stat();
	m_MTBS.m_VipZBMPBS.Stat();
	m_MTBS.m_ZBMPBS.Stat();
	m_MTBS.m_AllMPBS.Stat();
}

//
//主站数据项
//
int S_MSDI::SerializeData(const S_REGISTERS &CommRegs, const strings &strRegDatas, string &strData)const
{
	if (strRegDatas.Any() == false)//无数据
		return 0;

	switch (m_Type)
	{
	case MSDI_TYPE_NORMAL:return SerializeData_Normal(CommRegs, strRegDatas, strData);
	case MSDI_TYPE_TARIFF:return SerializeData_Tariff(CommRegs, strRegDatas, strData);
	case MSDI_TYPE_STATUS:return SerializeData_Status(CommRegs, strRegDatas, strData);
	case MSDI_TYPE_HARMONIOUS:return SerializeData_Harmonious(CommRegs, strRegDatas, strData);
	case MSDI_TYPE_CLASS1_HOUR:return SerializeData_Class1Hour(CommRegs, strRegDatas, strData);
	default:break;
	}

	return 0;
}

int S_MSDI::FillData(string &strData)const
{
	strings strRegDatas;
	S_REGISTERS CommRegs;

	for (int i = 0; i < MSDI_COMM_REG_MAX; i++)
	{
		if (m_CommRegID[i] == 0)
			break;
		S_REGISTER CommReg = STATUS.FindCommonRegister(m_CommRegID[i]);
		if (CommReg.m_ID == 0)
		{
			logwt<<"通用寄存器["<<HEX<<m_CommRegID[i]<<"]没有配置"<<endl;
			continue;
		}
		string strMyData;
		CommReg.FillData(strMyData);
		strRegDatas.push_back(strMyData);
		CommRegs.push_back(CommReg);
	}
	
	return SerializeData(CommRegs, strRegDatas, strData);
}

int S_MSDI::SerializeData_Normal(const S_REGISTERS &CommRegs, const strings &strRegDatas, string &strData)const
{
	char buffer[8];
	if (strRegDatas.AnyEmpty() == true)
		return 0;

	size_t size = strData.size();
	if ((m_Flag&MSDI_FLAG_TIME) == MSDI_FLAG_TIME)
	{
		C_TIME::CurTime2BCDStr(buffer);
		strData.append(buffer+1, 5);
	}
	strRegDatas.Serialize(strData);

	return (int)(strData.size() - size);
}

int S_MSDI::SerializeData_Tariff(const S_REGISTERS &CommRegs, const strings &strRegDatas, string &strData)const
{
	char buffer[8];
	if (strRegDatas[0].empty() == true)//费率块主要数据缺失
		return 0;

	strings strMyDatas(strRegDatas);
	size_t size = strData.size();

	if ((m_Flag&MSDI_FLAG_TIME) == MSDI_FLAG_TIME)
	{
		C_TIME::CurTime2BCDStr(buffer);
		strData.append(buffer+1, 5);
	}

	if ((m_Flag&MSDI_FLAG_BLOCK) == MSDI_FLAG_BLOCK)
	{//按块采
		S_BYTE MaxBlock = (S_BYTE)(strMyDatas[0].size() / CommRegs[0].m_Len);
		if (CommRegs.size() == 1)
		{//单个项
		}
		else
		{//多个项
			if (strMyDatas.Count() != CommRegs.size()) //没有采全
				return 0;
			//比较各费率
			MaxBlock = FindMaxTariffCnt(CommRegs, strMyDatas);
			FixDataTariff(CommRegs, strMyDatas, MaxBlock);
		}
		strData.push_back(MaxBlock - 1);
	}
	else
	{//单个单个采
		if (m_TariffCnt == 0x00)
		{
			logwt<<"主站数据项["<<m_Class<<":"<<m_MSDI<<"] TariffCnt 配置错误"<<endl;
			return 0;
		}
		else if (CommRegs.size() == (size_t)(m_TariffCnt + 1))
		{//单个项
			if (strMyDatas.Count() == 1)//maybe single tariff
				strMyDatas[1].append(strMyDatas[0]);
			else if (strMyDatas.AnyEmpty() == true)
				return 0;
			strData.push_back((S_BYTE)strMyDatas.Count() - 1);
		}
		else if (CommRegs.size() > (size_t)(m_TariffCnt + 1))
		{//多个项
			if (CommRegs.size() % (m_TariffCnt+1) != 0)
			{
				logwt<<"主站数据项["<<m_Class<<":"<<m_MSDI<<"]配置错误"<<endl;
				return 0;
			}
			if (strMyDatas.AnyEmpty() == true)
				return 0;
			strData.push_back(m_TariffCnt);
		}
		else if (CommRegs.size() < (size_t)(m_TariffCnt + 1))
		{
			logwt<<"主站数据项["<<m_Class<<":"<<m_MSDI<<"]配置错误"<<endl;
			return 0;
		}
	}

	strMyDatas.Serialize(strData);

	return (int)(strData.size() - size);
}

int S_MSDI::SerializeData_Status(const S_REGISTERS &CommRegs, const strings &strRegDatas, string &strData)const
{
	char buffer[8];
	if (strRegDatas.AnyEmpty() == true)
		return 0;

	size_t size = strData.size();
	if ((m_Flag&MSDI_FLAG_TIME) == MSDI_FLAG_TIME)
	{
		C_TIME::CurTime2BCDStr(buffer);
		strData.append(buffer+1, 5);
	}

	//电表运行状态字变位标志
//	for (size_t i = 0; i < CommRegs.size(); i++)
//	{
//		strData.append((size_t)CommRegs[i].m_Len, (char)0);//暂时填0
//	}
	//电表运行状态字
	strRegDatas.Serialize(strData);

	return (int)(strData.size() - size);
}

int S_MSDI::SerializeData_Harmonious(const S_REGISTERS &CommRegs, const strings &strRegDatas, string &strData)const
{
	return 0;
}

int S_MSDI::SerializeData_Class1Hour(const S_REGISTERS &CommRegs, const strings &strRegDatas, string &strData)const
{
	char buffer[8];
	if (strRegDatas.AnyEmpty() == true)
		return 0;

	size_t size = strData.size();

	//Hour sign
	int Cnt = (int)(strRegDatas[0].size() / CommRegs[0].m_Len);
	C_TIME::Time2BCDStr(C_TIME::DateAdd(TIME_UNIT_HOUR, -Cnt, C_TIME::TimeNow()), buffer);
	strData.push_back(buffer[2]); //HourSign
	strData.push_back(3); //density

	//time
	if ((m_Flag&MSDI_FLAG_TIME) == MSDI_FLAG_TIME)
	{
		C_TIME::CurTime2BCDStr(buffer);
		strData.append(buffer+1, 5);
	}
	strRegDatas.Serialize(strData);

	return (int)(strData.size() - size);
}

S_BYTE S_MSDI::FindMaxTariffCnt(const S_REGISTERS &CommRegs, const strings &strRegDatas)const
{
	S_BYTE MaxBlock = 0x00;
	for (size_t i = 0; i < CommRegs.size(); i++)
	{
		S_BYTE CurBlock = (S_BYTE)(strRegDatas[i].size() / CommRegs[i].m_Len);
		if (MaxBlock < CurBlock)
			MaxBlock = CurBlock;
	}
	return MaxBlock;
}

int S_MSDI::FixDataTariff(const S_REGISTERS &CommRegs, strings &strRegDatas, S_BYTE MaxTariffCnt)const
{
	for (size_t i = 0; i < CommRegs.size(); i++)
	{
		S_BYTE CurBlock = (S_BYTE)(strRegDatas[i].size() / CommRegs[i].m_Len);
		if (CurBlock >= MaxTariffCnt)
			continue;
		string strData;
		if (CommRegs[i].FillData(strData) <= 0)
			continue;
		for (CurBlock = CurBlock; CurBlock < MaxTariffCnt; CurBlock++)
			strRegDatas[i].append(strData);
	}
	return 0;
}

//
//轮询任务数据
//
/** @return >0:成功; <=0:失败*/
int S_PollingDatas::Load(S_WORD MP, S_PollingData &PollData)
{
	if (MP >= sizeof(m_Item)/sizeof(m_Item[0]))
		return -1;
	S_PollingData &MyData = m_Item[MP];
	if (MyData.m_Enable == false || MyData.m_timePollMeter == 0xEEEEEEEE)
		return -1;
	PollData = MyData;
	if (memcmp(&PollData, &MyData, sizeof(S_PollingData)) != 0)
	{
		PollData = MyData;
		if (memcmp(&PollData, &MyData, sizeof(S_PollingData)) != 0)
			return -1;
	}
	return (int)sizeof(S_PollingData);
}

/** @return >=0:成功; <0:失败*/
int S_PollingDatas::Save(S_TIME timeNow, S_WORD MP, const S_REGISTERS &Registers, const strings &strDatas)
{
	if (MP >= sizeof(m_Item)/sizeof(m_Item[0]))
		return -1;
	S_PollingData &MyData = m_Item[MP];
	MyData.m_Enable = false;
	_Save(MyData, Registers, strDatas);
	MyData.m_timePollMeter = timeNow;
	MyData.m_Enable = true;
	return (int)sizeof(S_PollingData);
}

int S_PollingDatas::_Save(S_PollingData &PollData, const S_REGISTERS &Registers, const strings &strDatas)
{
	for (size_t i = 0; i < Registers.size(); i++)
	{
		const S_REGISTER &MyRegister = Registers[i];
		const string &strMyData = strDatas[i];
		switch ((MyRegister.m_ID>>16) & 0xFFFF)
		{
		case 0xC010: copy(strMyData.begin(), strMyData.end(), PollData.m_Date_C010);break; ///< 日期
		case 0xC011: copy(strMyData.begin(), strMyData.end(), PollData.m_Time_C011);break; ///< 时间
		case 0x9010: copy(strMyData.begin(), strMyData.end(), PollData.m_Zxyg_9010);break; ///< 正向有功
		case 0x9110: copy(strMyData.begin(), strMyData.end(), PollData.m_Zxwg_9110);break; ///< 正向无功
		case 0xB630: copy(strMyData.begin(), strMyData.end(), PollData.m_ActivePowerSum_B630);break; ///< 三相有功功率总
		case 0xB631: copy(strMyData.begin(), strMyData.end(), PollData.m_ActivePowerA_B631);break; ///< A相功率  
		case 0xB632: copy(strMyData.begin(), strMyData.end(), PollData.m_ActivePowerB_B632);break; ///< B相功率  
		case 0xB633: copy(strMyData.begin(), strMyData.end(), PollData.m_ActivePowerC_B633);break; ///< C相功率  
		case 0xB650: copy(strMyData.begin(), strMyData.end(), PollData.m_PwFactorSum_B650);break; ///< 功率因数总
		case 0xB651: copy(strMyData.begin(), strMyData.end(), PollData.m_PwFactorA_B651);break; ///< 功率因数A相
		case 0xB652: copy(strMyData.begin(), strMyData.end(), PollData.m_PwFactorB_B652);break; ///< 功率因数B相
		case 0xB653: copy(strMyData.begin(), strMyData.end(), PollData.m_PwFactorC_B653);break; ///< 功率因数C相
		case 0xB640: copy(strMyData.begin(), strMyData.end(), PollData.m_ReactivePowerSum_B640);break; ///< 三相无功功率总
		case 0xB641: copy(strMyData.begin(), strMyData.end(), PollData.m_ReactivePowerA_B641);break; ///< A相无功功率  
		case 0xB642: copy(strMyData.begin(), strMyData.end(), PollData.m_ReactivePowerB_B642);break; ///< B相无功功率  
		case 0xB643: copy(strMyData.begin(), strMyData.end(), PollData.m_ReactivePowerC_B643);break; ///< C相无功功率  
	
		case 0xA010: copy(strMyData.begin(), strMyData.end(), PollData.m_DemandSum_A010);break; ///< 正有需量
		case 0xB010: copy(strMyData.begin(), strMyData.end(), PollData.m_DemandSum_B010);break; ///< 正有需量时间 
		case 0x60A1: copy(strMyData.begin(), strMyData.end(), PollData.m_DemandA_60A1);break; ///< A正有需量  
		case 0x60B1: copy(strMyData.begin(), strMyData.end(), PollData.m_DemandA_60B1);break; ///< A正有需量时间  
		case 0x60A2: copy(strMyData.begin(), strMyData.end(), PollData.m_DemandB_60A2);break; ///< B正有需量  
		case 0x60B2: copy(strMyData.begin(), strMyData.end(), PollData.m_DemandB_60B2);break; ///< B正有需量时间  
		case 0x60A3: copy(strMyData.begin(), strMyData.end(), PollData.m_DemandC_60A3);break; ///< C正有需量  
		case 0x60B3: copy(strMyData.begin(), strMyData.end(), PollData.m_DemandC_60B3);break; ///< C正有需量时间  

		case 0xB621: copy(strMyData.begin(), strMyData.end(), PollData.m_IA_B621);break; ///< A相电流
		case 0xB622: copy(strMyData.begin(), strMyData.end(), PollData.m_IB_B622);break; ///< B相电流
		case 0xB623: copy(strMyData.begin(), strMyData.end(), PollData.m_IC_B623);break; ///< C相电流
		case 0x6010: copy(strMyData.begin(), strMyData.end(), PollData.m_I0_6010);break; ///< 零序电流
		case 0xB611: copy(strMyData.begin(), strMyData.end(), PollData.m_UA_B611);break; ///< A相电压
		case 0xB612: copy(strMyData.begin(), strMyData.end(), PollData.m_UB_B612);break; ///< B相电压
		case 0xB613: copy(strMyData.begin(), strMyData.end(), PollData.m_UC_B613);break; ///< B相电压
		case 0x6020: copy(strMyData.begin(), strMyData.end(), PollData.m_PowerSZ_6020);break; ///< 视在功率
		default:break;
		}
	}
	return 0;
}


