/** @file
 *  @brief 后台通讯命令测量点写参数对象实现
 *  @author  zhongguangxiong
 *  @date 2012/02/18
 *  @version 0.1
 */
#include "cmdwmp.h"
#include "status.h"
#include "context.h"
#include "nwmsrule.h"

/** @brief 写测量点参数 基类*/
S_DWORD C_WMCmdReceiver::m_UpdateFlag = 0x00;

bool C_WMCmdReceiver::SetUpdateFlag(S_DWORD flag)
{
	m_UpdateFlag |= flag;
	return true;
}

bool C_WMCmdReceiver::UpdateInformation(void)
{
	if (m_UpdateFlag & WM_UPDATE_DBCJ_INFO)
		STATUS.SaveDbcjInfo();
	if (m_UpdateFlag & WM_UPDATE_METER_INFO)
		STATUS.RefreshMeterInfo();
	if (m_UpdateFlag & WM_UPDATE_NOTIFY_ADD_METER)
		CONTEXT.NotifyDbcj(NOTIFY_DBCJ_ADD_METER);

	m_UpdateFlag = 0x00;
	return true;
}

//测量点的状态 
bool C_CmdwE0800000::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800000::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_DA da(CmdObj.m_DA);
	if (da.GetMpCount() == 0xFF)//所有测量点，下表不支持
		return false;

	S_MpInfos *pMpInfos = STATUS.GetMpInfos();
	string::const_iterator iter = CmdObj.m_DiInfo.begin();
	for(INT8U DA1=0;DA1<8;DA1++)
	{
		if (da.TestMP(DA1) == true)
		{
			string strParam(iter, iter+1);
			pMpInfos->m_Item[da.GetMP(DA1)].m_AutoGather = strParam[0];
			advance(iter, 1);
		}
	}

	SetUpdateFlag(WM_UPDATE_DBCJ_INFO | WM_UPDATE_METER_INFO | WM_UPDATE_NOTIFY_ADD_METER);
	return true;
}
//测量点性质
bool C_CmdwE0800001::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800001::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_DA da(CmdObj.m_DA);
	if (da.GetMpCount() == 0xFF)//所有测量点，下表不支持
		return false;

	S_MpInfos *pMpInfos = STATUS.GetMpInfos();
	string::const_iterator iter = CmdObj.m_DiInfo.begin();
	for(INT8U DA1=0;DA1<8;DA1++)
	{
		if (da.TestMP(DA1) == true)
		{
			string strParam(iter, iter+1);
			pMpInfos->m_Item[da.GetMP(DA1)].m_Type = strParam[0];
			pMpInfos->m_Item[da.GetMP(DA1)].m_SN = da.GetMP(DA1);
			if (pMpInfos->m_Item[da.GetMP(DA1)].m_Type == MP_TYPE_UNKNOW)
				DeleteMeasurePoint(da.GetMP(DA1));
			else if (pMpInfos->m_Item[da.GetMP(DA1)].m_Type == MP_TYPE_AC)
				AddMeterAC(da.GetMP(DA1));
			else if (pMpInfos->m_Item[da.GetMP(DA1)].m_Type == MP_TYPE_METER)
				AddMeterRS485(da.GetMP(DA1));
			advance(iter, 1);
		}
	}

	return true;
}
bool C_CmdwE0800001::AddMeterAC(S_WORD SN)
{
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	pMeterInfos->m_Item[SN].ClearMeterInfo();
	pMeterInfos->m_Item[SN].m_Enable = true;
	pMeterInfos->m_Item[SN].m_MeasureNo = SN;
	pMeterInfos->m_Item[SN].m_RuleID = METER_RULE_AC_GATHER;
	pMeterInfos->m_Item[SN].m_FunType = METER_FUN_MULTI_FUN;

	S_CHANNEL *pChannel = STATUS.GetJlcyChannel(JLCY_CHANNL_AC_GATHER);
	if (pChannel != NULL)
		pMeterInfos->m_Item[SN].m_PortSet.m_PortNO = pChannel->m_PortSet.m_ComSet.m_PortNO;

	string strAddress;
	S_SYSCFG *pSysCfg = STATUS.GetSystemConfig();
	C_RTUA rtua(pSysCfg->m_RTUA);
	rtua.PrintString(strAddress);
	reverse_copy(strAddress.begin(), strAddress.begin()+6, pMeterInfos->m_Item[SN].m_Address);

	SetUpdateFlag(WM_UPDATE_METER_INFO | WM_UPDATE_NOTIFY_ADD_METER);
	return true;
}
bool C_CmdwE0800001::AddMeterRS485(S_WORD SN)
{
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
//	pMeterInfos->m_Item[SN].ClearMeterInfo();
	pMeterInfos->m_Item[SN].m_Enable = true;
	pMeterInfos->m_Item[SN].m_MeasureNo = SN;
	if (pMeterInfos->m_Item[SN].m_PortSet.m_PortNO == 0)
		pMeterInfos->m_Item[SN].m_PortSet.m_PortNO = 1; // default RS485I

	SetUpdateFlag(WM_UPDATE_METER_INFO | WM_UPDATE_NOTIFY_ADD_METER);
	return true;
}
bool C_CmdwE0800001::DeleteMeasurePoint(S_WORD MP)
{
	S_MpInfos *pMpInfos = STATUS.GetMpInfos();
	S_MpInfo &MpInfo = pMpInfos->m_Item[MP];
	switch (MpInfo.m_Type)
	{
	case MP_TYPE_METER: DeleteMeter(MpInfo.m_SN); break; // 表计
	case MP_TYPE_ANALOG: DeleteAnalog(MpInfo.m_SN); break; // 模拟量
	case MP_TYPE_PULSE:  DeletePulse(MpInfo.m_SN); break; // 脉冲
	case MP_TYPE_SUM:  DeleteSUM(MpInfo.m_SN); break; // 计算量
	case MP_TYPE_AC:  DeleteAC(MpInfo.m_SN); break; // 交采
	default:break;
	}
	MpInfo.m_AutoGather = 0;
	MpInfo.m_Type = MP_TYPE_UNKNOW;
	MpInfo.m_SN = 0x00;
	return true;
}
bool C_CmdwE0800001::DeleteMeter(S_WORD SN) // 表计
{
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	pMeterInfos->m_Item[SN].m_Enable = false;

	SetUpdateFlag(WM_UPDATE_METER_INFO | WM_UPDATE_NOTIFY_ADD_METER);
	return true;
}
bool C_CmdwE0800001::DeleteAnalog(S_WORD SN) // 模拟量
{
	return true;
}
bool C_CmdwE0800001::DeletePulse(S_WORD SN) // 脉冲
{
	return true;
}
bool C_CmdwE0800001::DeleteSUM(S_WORD SN) // 计算量
{
	return true;
}
bool C_CmdwE0800001::DeleteAC(S_WORD SN) // 交采
{
	return true;
}
//测量点地址
bool C_CmdwE0800002::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(6,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800002::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_DA da(CmdObj.m_DA);
	if (da.GetMpCount() == 0xFF)//所有测量点，下表不支持
		return false;

	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	string::const_iterator iter = CmdObj.m_DiInfo.begin();
	for(INT8U DA1=0;DA1<8;DA1++)
	{
		if (da.TestMP(DA1) == true)
		{
			string strParam(iter, iter+6);
			memcpy(pMeterInfos->m_Item[da.GetMP(DA1)].m_Address,&strParam[0],6);
			advance(iter, 6);
		}
	}

	SetUpdateFlag(WM_UPDATE_METER_INFO | WM_UPDATE_NOTIFY_ADD_METER);
	return true;
}
//测量点通信规约
bool C_CmdwE0800003::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800003::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_DA da(CmdObj.m_DA);
	if (da.GetMpCount() == 0xFF)//所有测量点，下表不支持
		return false;

	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	string::const_iterator iter = CmdObj.m_DiInfo.begin();
	for(INT8U DA1=0;DA1<8;DA1++)
	{
		if (da.TestMP(DA1) == true)
		{
			string strParam(iter, iter+1);
			pMeterInfos->m_Item[da.GetMP(DA1)].m_RuleID = strParam[0];
			advance(iter, 1);
		}
	}

	SetUpdateFlag(WM_UPDATE_METER_INFO | WM_UPDATE_NOTIFY_ADD_METER);
	return true;
}
//电能表类型
bool C_CmdwE0800004::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800004::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_DA da(CmdObj.m_DA);
	if (da.GetMpCount() == 0xFF)//所有测量点，下表不支持
		return false;

	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	string::const_iterator iter = CmdObj.m_DiInfo.begin();
	for(INT8U DA1=0;DA1<8;DA1++)
	{
		if (da.TestMP(DA1) == true)
		{
			string strParam(iter, iter+1);
			pMeterInfos->m_Item[da.GetMP(DA1)].m_FunType = strParam[0];
			advance(iter, 1);
		}
	}

	SetUpdateFlag(WM_UPDATE_METER_INFO | WM_UPDATE_NOTIFY_ADD_METER);
	return true;
}
//总分类型
bool C_CmdwE0800005::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800005::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_DA da(CmdObj.m_DA);
	if (da.GetMpCount() == 0xFF)//所有测量点，下表不支持
		return false;

	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	string::const_iterator iter = CmdObj.m_DiInfo.begin();
	for(INT8U DA1=0;DA1<8;DA1++)
	{
		if (da.TestMP(DA1) == true)
		{
			string strParam(iter, iter+1);
			pMeterInfos->m_Item[da.GetMP(DA1)].m_MainFlag = strParam[0]?true:false;
			advance(iter, 1);
		}
	}

	SetUpdateFlag(WM_UPDATE_METER_INFO);
	return true;
}
//重点用户属性
bool C_CmdwE0800006::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800006::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_DA da(CmdObj.m_DA);
	if (da.GetMpCount() == 0xFF)//所有测量点，下表不支持
		return false;

	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	string::const_iterator iter = CmdObj.m_DiInfo.begin();
	for(INT8U DA1=0;DA1<8;DA1++)
	{
		if (da.TestMP(DA1) == true)
		{
			string strParam(iter, iter+1);
			pMeterInfos->m_Item[da.GetMP(DA1)].m_VipFlag = strParam[0]?true:false;
			advance(iter, 1);
		}
	}

	SetUpdateFlag(WM_UPDATE_METER_INFO | WM_UPDATE_NOTIFY_ADD_METER);
	return true;
}
//拉闸功能
bool C_CmdwE0800007::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800007::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_DA da(CmdObj.m_DA);
	if (da.GetMpCount() == 0xFF)//所有测量点，下表不支持
		return false;

	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	string::const_iterator iter = CmdObj.m_DiInfo.begin();
	for(INT8U DA1=0;DA1<8;DA1++)
	{
		if (da.TestMP(DA1) == true)
		{
			string strParam(iter, iter+1);
			pMeterInfos->m_Item[da.GetMP(DA1)].m_PowerCut = strParam[0]?true:false;
			advance(iter, 1);
		}
	}

	SetUpdateFlag(WM_UPDATE_METER_INFO);
	return true;
}
//最大费率数
bool C_CmdwE0800008::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800008::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_DA da(CmdObj.m_DA);
	if (da.GetMpCount() == 0xFF)//所有测量点，下表不支持
		return false;

	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	string::const_iterator iter = CmdObj.m_DiInfo.begin();
	for(INT8U DA1=0;DA1<8;DA1++)
	{
		if (da.TestMP(DA1) == true)
		{
			string strParam(iter, iter+1);
			pMeterInfos->m_Item[da.GetMP(DA1)].m_TariffCnt = strParam[0];
			advance(iter, 1);
		}
	}

	SetUpdateFlag(WM_UPDATE_METER_INFO);
	return true;
}
//测量点对应采集终端地址
bool C_CmdwE0800009::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(6,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800009::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_DA da(CmdObj.m_DA);
	if (da.GetMpCount() == 0xFF)//所有测量点，下表不支持
		return false;

	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	string::const_iterator iter = CmdObj.m_DiInfo.begin();
	for(INT8U DA1=0;DA1<8;DA1++)
	{
		if (da.TestMP(DA1) == true)
		{
			string strParam(iter, iter+6);
			memcpy(pMeterInfos->m_Item[da.GetMP(DA1)].m_RelayAddr,&strParam[0],6);
			advance(iter, 6);
		}
	}

	SetUpdateFlag(WM_UPDATE_METER_INFO | WM_UPDATE_NOTIFY_ADD_METER);
	return true;
}
//测量点端口号
bool C_CmdwE080000A::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE080000A::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_DA da(CmdObj.m_DA);
	if (da.GetMpCount() == 0xFF)//所有测量点，下表不支持
		return false;

	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	string::const_iterator iter = CmdObj.m_DiInfo.begin();
	for(INT8U DA1=0;DA1<8;DA1++)
	{
		if (da.TestMP(DA1) == true)
		{
			string strParam(iter, iter+1);
			pMeterInfos->m_Item[da.GetMP(DA1)].m_PortSet.m_PortNO = strParam[0] + 1;
			advance(iter, 1);
		}
	}

	SetUpdateFlag(WM_UPDATE_METER_INFO | WM_UPDATE_NOTIFY_ADD_METER);
	return true;
}
//端口参数
bool C_CmdwE080000B::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(4,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE080000B::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_DA da(CmdObj.m_DA);
	if (da.GetMpCount() == 0xFF)//所有测量点，下表不支持
		return false;

	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	string::const_iterator iter = CmdObj.m_DiInfo.begin();
	for(INT8U DA1=0;DA1<8;DA1++)
	{
		if (da.TestMP(DA1) == true)
		{
			string strParam(iter, iter+4);
			//波特率
			pMeterInfos->m_Item[da.GetMP(DA1)].m_PortSet.m_Baud = strParam[0] * 300;
			//校验方式
			if (strParam[1] == 0x01) pMeterInfos->m_Item[da.GetMP(DA1)].m_PortSet.m_Parity = 2;
			else if (strParam[1] == 0x02) pMeterInfos->m_Item[da.GetMP(DA1)].m_PortSet.m_Parity = 1;
			else pMeterInfos->m_Item[da.GetMP(DA1)].m_PortSet.m_Parity = 0;
			//数据位
			pMeterInfos->m_Item[da.GetMP(DA1)].m_PortSet.m_DataBit = strParam[2];
			//停止位
			if (strParam[3] == 0x01) pMeterInfos->m_Item[da.GetMP(DA1)].m_PortSet.m_StopBit = 3;
			else if (strParam[3] == 0x02) pMeterInfos->m_Item[da.GetMP(DA1)].m_PortSet.m_StopBit = 2;
			else pMeterInfos->m_Item[da.GetMP(DA1)].m_PortSet.m_StopBit = 1;

			advance(iter, 4);
		}
	}

	SetUpdateFlag(WM_UPDATE_METER_INFO | WM_UPDATE_NOTIFY_ADD_METER);
	return true;
}
//CT变比
bool C_CmdwE080000C::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE080000C::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_DA da(CmdObj.m_DA);
	if (da.GetMpCount() == 0xFF)//所有测量点，下表不支持
		return false;

	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	string::const_iterator iter = CmdObj.m_DiInfo.begin();
	for(INT8U DA1=0;DA1<8;DA1++)
	{
		if (da.TestMP(DA1) == true)
		{
			string strParam(iter, iter+2);
			pMeterInfos->m_Item[da.GetMP(DA1)].m_CT = (INT16U)kge_bcdc(strParam[0]) + (INT16U)kge_bcdc(strParam[1])*100;
			advance(iter, 2);
		}
	}

	SetUpdateFlag(WM_UPDATE_METER_INFO);
	return true;
}
//PT变比
bool C_CmdwE080000D::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE080000D::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_DA da(CmdObj.m_DA);
	if (da.GetMpCount() == 0xFF)//所有测量点，下表不支持
		return false;

	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	string::const_iterator iter = CmdObj.m_DiInfo.begin();
	for(INT8U DA1=0;DA1<8;DA1++)
	{
		if (da.TestMP(DA1) == true)
		{
			string strParam(iter, iter+2);
			pMeterInfos->m_Item[da.GetMP(DA1)].m_PT = (INT16U)kge_bcdc(strParam[0]) + (INT16U)kge_bcdc(strParam[1])*100;
			advance(iter, 2);
		}
	}

	SetUpdateFlag(WM_UPDATE_METER_INFO);
	return true;
}
//以上数据项集合
bool C_CmdwE080000F::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	C_DA da(CmdObj.m_DA);
	if (da.GetMpCount() == 0xFF)//所有测量点，下表不支持
		return false;

	INT16U cnt = (INT16U)da.GetMpCount();
	if (cnt == 0) cnt = 1;

	if (distance(iterBegin, iterEnd) < 29*cnt)
		return false;

	CmdObj.m_DiInfo.append(iterBegin, iterBegin+29*cnt);
	advance(iterBegin, 29*cnt);
	return true;
}
bool C_CmdwE080000F::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_DA da(CmdObj.m_DA);
	if (da.GetMpCount() == 0xFF)//所有测量点，下表不支持
		return false;

	string::const_iterator iter = CmdObj.m_DiInfo.begin();
	for (INT16U i = 0; i < 8; i++)
	{
		if (da.TestMP(i) == true)
		{
			string strParam(iter, iter+29);
			SetMeterParam(da.GetMP(i), strParam);
			advance(iter, 29);
		}
	}

	SetUpdateFlag(WM_UPDATE_DBCJ_INFO | WM_UPDATE_METER_INFO | WM_UPDATE_NOTIFY_ADD_METER);
	return true;
}
bool C_CmdwE080000F::SetMeterParam(INT16U MP, const string &strMeterParam)
{
	if (MP >= CONS_METER_COUNT_MAX)
		return false;

	S_MpInfos *pMpInfos = STATUS.GetMpInfos();
	pMpInfos->m_Item[MP].m_AutoGather = strMeterParam[0];
	pMpInfos->m_Item[MP].m_Type = strMeterParam[1];
	pMpInfos->m_Item[MP].m_SN = MP;
	
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_MeterInfo &meter = pMeterInfos->m_Item[MP];

	meter.m_Enable = true;
	meter.m_MeasureNo = MP;
	copy(&strMeterParam[2], &strMeterParam[8], meter.m_Address);
	meter.m_RuleID = strMeterParam[8];
	meter.m_FunType = strMeterParam[9];
	meter.m_MainFlag = strMeterParam[10]?true:false;
	meter.m_VipFlag = strMeterParam[11]?true:false;
	meter.m_PowerCut = strMeterParam[12]?true:false;
	meter.m_TariffCnt = strMeterParam[13];
	copy(&strMeterParam[14], &strMeterParam[20], meter.m_RelayAddr);
	meter.m_PortSet.m_PortNO = strMeterParam[20] + 1;
	meter.m_PortSet.m_Baud = strMeterParam[21] * 300;

	if (strMeterParam[22] == 0x01) meter.m_PortSet.m_Parity = 2;
	else if (strMeterParam[22] == 0x02) meter.m_PortSet.m_Parity = 1;
	else meter.m_PortSet.m_Parity = 0;

	meter.m_PortSet.m_DataBit = strMeterParam[23];

	if (strMeterParam[24] == 0x00) meter.m_PortSet.m_StopBit = 1;
	else if (strMeterParam[24] == 0x01) meter.m_PortSet.m_StopBit = 3;
	else meter.m_PortSet.m_StopBit = 2;

	meter.m_CT = (INT16U)kge_bcdc(strMeterParam[25]) + (INT16U)kge_bcdc(strMeterParam[26])*100;
	meter.m_PT = (INT16U)kge_bcdc(strMeterParam[27]) + (INT16U)kge_bcdc(strMeterParam[28])*100;

	return true;
}
//
bool C_CmdwE0800010::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800010::Action(const S_CmdObj &CmdObj, strings &strResponse)
{	
	C_DA da(CmdObj.m_DA);
	if (da.GetMpCount() == 0xFF)//所有测量点，下表不支持
		return false;

	string::const_iterator iter = CmdObj.m_DiInfo.begin();
	for (INT16U i = 0; i < 8; i++)
	{
		if (da.TestMP(i) == true)
		{
			string strParam(iter, iter+1);
			SetMeterParam(da.GetMP(i), strParam);
			advance(iter, 1);
		}
	}

	SetUpdateFlag(WM_UPDATE_DBCJ_INFO | WM_UPDATE_METER_INFO | WM_UPDATE_NOTIFY_ADD_METER);
	return true;
}

bool C_CmdwE0800010::SetMeterParam(INT16U MP, const string &strMeterParam)
{
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_MeterInfo &meter = pMeterInfos->m_Item[MP];
	meter.m_WireType=(S_BYTE)strMeterParam[0];
	return true;
}

//
bool C_CmdwE0800011::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800011::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_DA da(CmdObj.m_DA);
	if (da.GetMpCount() == 0xFF)//所有测量点，下表不支持
		return false;
	
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	string::const_iterator iter = CmdObj.m_DiInfo.begin();
	for (INT16U i = 0; i < 8; i++)
	{
		if (da.TestMP(i) == true)
		{
			string strParam(iter, iter+2);
			S_MeterInfo &meter = pMeterInfos->m_Item[da.GetMP(i)];
			memcpy((S_BYTE*)&meter.m_RatedVoltage,strParam.c_str(),2);
			advance(iter, 2);
		}
	}

	SetUpdateFlag(WM_UPDATE_DBCJ_INFO | WM_UPDATE_METER_INFO | WM_UPDATE_NOTIFY_ADD_METER);
	return true;
}
//
bool C_CmdwE0800012::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(3,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800012::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_DA da(CmdObj.m_DA);
	if (da.GetMpCount() == 0xFF)//所有测量点，下表不支持
		return false;
	
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	string::const_iterator iter = CmdObj.m_DiInfo.begin();
	for (INT16U i = 0; i < 8; i++)
	{
		if (da.TestMP(i) == true)
		{
			string strParam(iter, iter+3);
			S_MeterInfo &meter = pMeterInfos->m_Item[da.GetMP(i)];
			memcpy((S_BYTE*)&meter.m_RatedCur,strParam.c_str(),3);
			advance(iter, 3);
		}
	}

	SetUpdateFlag(WM_UPDATE_DBCJ_INFO | WM_UPDATE_METER_INFO | WM_UPDATE_NOTIFY_ADD_METER);
	return true;
}
//
bool C_CmdwE0800013::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800013::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_DA da(CmdObj.m_DA);
	if (da.GetMpCount() == 0xFF)//所有测量点，下表不支持
		return false;
	
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	string::const_iterator iter = CmdObj.m_DiInfo.begin();
	for (INT16U i = 0; i < 8; i++)
	{
		if (da.TestMP(i) == true)
		{
			string strParam(iter, iter+2);
			S_MeterInfo &meter = pMeterInfos->m_Item[da.GetMP(i)];
			memcpy((S_BYTE*)&meter.m_RatedContent,strParam.c_str(),2);
			advance(iter, 2);
		}
	}

	SetUpdateFlag(WM_UPDATE_DBCJ_INFO | WM_UPDATE_METER_INFO | WM_UPDATE_NOTIFY_ADD_METER);
	return true;
}
//
bool C_CmdwE0800014::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800014::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_DA da(CmdObj.m_DA);
	if (da.GetMpCount() == 0xFF)//所有测量点，下表不支持
		return false;
	
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	string::const_iterator iter = CmdObj.m_DiInfo.begin();
	for (INT16U i = 0; i < 8; i++)
	{
		if (da.TestMP(i) == true)
		{
			string strParam(iter, iter+2);
			S_MeterInfo &meter = pMeterInfos->m_Item[da.GetMP(i)];
			memcpy((S_BYTE*)&meter.m_VTContent,strParam.c_str(),2);
			advance(iter, 2);
		}
	}

	SetUpdateFlag(WM_UPDATE_DBCJ_INFO | WM_UPDATE_METER_INFO | WM_UPDATE_NOTIFY_ADD_METER);
	return true;
}
//
bool C_CmdwE080001F::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(10,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE080001F::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_DA da(CmdObj.m_DA);
	if (da.GetMpCount() == 0xFF)//所有测量点，下表不支持
		return false;
	
	string::const_iterator iter = CmdObj.m_DiInfo.begin();
	for (INT16U i = 0; i < 8; i++)
	{
		if (da.TestMP(i) == true)
		{
			string strParam(iter, iter+10);
			SetMeterParam(da.GetMP(i), strParam);
			advance(iter, 10);
		}
	}

	SetUpdateFlag(WM_UPDATE_DBCJ_INFO | WM_UPDATE_METER_INFO | WM_UPDATE_NOTIFY_ADD_METER);
	return true;
}

bool C_CmdwE080001F::SetMeterParam(INT16U MP, const string &strMeterParam)
{
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_MeterInfo &meter = pMeterInfos->m_Item[MP];
	string::const_iterator iter = strMeterParam.begin();

	meter.m_WireType = (S_BYTE)*iter; advance(iter, 1);
	meter.m_RatedVoltage = KGE_COMBWORD(*(iter+1), *(iter+0)); advance(iter, 2);
	meter.m_RatedCur = KGE_COMBDWORD(KGE_COMBWORD(0, *(iter+2)), KGE_COMBWORD(*(iter+1), *(iter+0))); advance(iter, 3);
	meter.m_RatedContent = KGE_COMBWORD(*(iter+1), *(iter+0)); advance(iter, 2);
	meter.m_VTContent = KGE_COMBWORD(*(iter+1), *(iter+0)); advance(iter, 2);

	return true;
}

//
bool C_CmdwE0800021::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(8,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800021::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_DA da(CmdObj.m_DA);
	if (da.GetMpCount() == 0xFF)//所有测量点，下表不支持
		return false;
	
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	string::const_iterator iter = CmdObj.m_DiInfo.begin();
	for (INT16U i = 0; i < 8; i++)
	{
		if (da.TestMP(i) == true)
		{
			string strParam(iter, iter+8);
			S_MeterInfo &meter = pMeterInfos->m_Item[da.GetMP(i)];
			memcpy((S_BYTE*)&meter.m_Password,strParam.c_str(),8);
			advance(iter, 8);
		}
	}

	SetUpdateFlag(WM_UPDATE_DBCJ_INFO | WM_UPDATE_METER_INFO | WM_UPDATE_NOTIFY_ADD_METER);
	return true;
}
//
bool C_CmdwE0800022::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(8,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800022::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_DA da(CmdObj.m_DA);
	if (da.GetMpCount() == 0xFF)//所有测量点，下表不支持
		return false;
	
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	string::const_iterator iter = CmdObj.m_DiInfo.begin();
	for (INT16U i = 0; i < 8; i++)
	{
		if (da.TestMP(i) == true)
		{
			string strParam(iter, iter+8);
			S_MeterInfo &meter = pMeterInfos->m_Item[da.GetMP(i)];
			memset(&meter.m_UserName, 0x00, sizeof(meter.m_UserName));
			memcpy((S_BYTE*)&meter.m_UserName,strParam.c_str(),8);
			advance(iter, 8);
		}
	}

	SetUpdateFlag(WM_UPDATE_DBCJ_INFO | WM_UPDATE_METER_INFO | WM_UPDATE_NOTIFY_ADD_METER);
	return true;
}
//
bool C_CmdwE080002F::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(16,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE080002F::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_DA da(CmdObj.m_DA);
	if (da.GetMpCount() == 0xFF)//所有测量点，下表不支持
		return false;
	
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	string::const_iterator iter = CmdObj.m_DiInfo.begin();
	for (INT16U i = 0; i < 8; i++)
	{
		if (da.TestMP(i) == true)
		{
			string strParam(iter, iter+16);
			S_MeterInfo &meter = pMeterInfos->m_Item[da.GetMP(i)];
			memcpy((S_BYTE*)&meter.m_Password,strParam.c_str(),8);
			memset(&meter.m_UserName, 0x00, sizeof(meter.m_UserName));
			memcpy((S_BYTE*)&meter.m_UserName,strParam.c_str()+8,8);
			advance(iter, 16);
		}
	}

	SetUpdateFlag(WM_UPDATE_DBCJ_INFO | WM_UPDATE_METER_INFO | WM_UPDATE_NOTIFY_ADD_METER);
	return true;
}

//电流不平衡度阈值
bool C_CmdwE0800100::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800100::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strParam;
	S_MPLS *pMPointLimits = STATUS.GetMPointLimits();
	S_MPointLimit *pMPointLimit = &pMPointLimits->Item[C_DAPN::DA2PN(CmdObj.m_DA)];
	AppendDADT(strParam, CmdObj);
	if(pMPointLimit->m_Enable == false)
		return false;
	/*
	strParam.append((char *)&pMPointLimit->Uhgsx,2);
	strParam.append((char *)&pMPointLimit->Uhgxx,2);
	strParam.append((char *)&pMPointLimit->Udxmx,2);
	
	strParam.append((char *)&pMPointLimit->Us_sx,2);
	strParam.push_back((char)pMPointLimit->Us_yxcx);
	strParam.append((char *)&pMPointLimit->Us_yxhf,2);

	strParam.append((char *)&pMPointLimit->Ux_xx,2);
	strParam.push_back((char)pMPointLimit->Ux_yxcx);
	strParam.append((char *)&pMPointLimit->Ux_yxhf,2);

	strParam.append((char *)&pMPointLimit->PIss_sx,3);
	strParam.push_back((char)pMPointLimit->PIss_yxcx);
	strParam.append((char *)&pMPointLimit->PIss_yxhf,2);

	strParam.append((char *)&pMPointLimit->PIs_sx,3);
	strParam.push_back((char)pMPointLimit->PIs_yxcx);
	strParam.append((char *)&pMPointLimit->PIs_yxhf,2);
	
	strParam.append((char *)&pMPointLimit->I0_sx,3);
	strParam.push_back((char)pMPointLimit->I0_yxcx);
	strParam.append((char *)&pMPointLimit->I0_yxhf,2);

	strParam.append((char *)&pMPointLimit->APss_sx,3);
	strParam.push_back((char)pMPointLimit->APss_yxcx);
	strParam.append((char *)&pMPointLimit->APss_yxhf,2);

	strParam.append((char *)&pMPointLimit->APs_sx,3);
	strParam.push_back((char)pMPointLimit->APs_yxcx);
	strParam.append((char *)&pMPointLimit->APss_yxhf,2);

	
	strParam.append((char *)&pMPointLimit->P3U_bph,2);
	strParam.push_back((char)pMPointLimit->P3U_yxcx);
	strParam.append((char *)&pMPointLimit->P3U_yxhf,2);

	strParam.append((char *)&pMPointLimit->P3I_bph,2);
	strParam.push_back((char)pMPointLimit->P3I_yxcx);
	strParam.append((char *)&pMPointLimit->P3I_yxhf,2);

	strParam.push_back((char)pMPointLimit->lxsy_timelimit);
	*/
	strResponse.push_back(strParam);

	STATUS.SaveMPointLimit();
	return true;
}//
bool C_CmdwE0800101::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800101::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}//
bool C_CmdwE0800102::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800102::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}//
bool C_CmdwE0800103::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800103::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//
bool C_CmdwE0800104::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800104::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}//
bool C_CmdwE0800105::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800105::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}//
bool C_CmdwE0800106::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800106::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}//
bool C_CmdwE0800107::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800107::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}//
bool C_CmdwE0800108::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800108::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}//
bool C_CmdwE0800109::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800109::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}//
bool C_CmdwE080010A::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE080010A::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}//
bool C_CmdwE080010B::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE080010B::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}//
bool C_CmdwE080010C::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(8,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE080010C::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}//
bool C_CmdwE080010F::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(25,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE080010F::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
/*	string strParam;
	S_MPLS *pMPointLimits = STATUS.GetMPointLimits();
	S_MPointLimit *pMPointLimit = &pMPointLimits->Item[C_DAPN::DA2PN(CmdObj.m_DA)];
	AppendDADT(strParam, CmdObj);
	if(pMPointLimit->Enable == false)
		return false;
	strParam.append((char *)&pMPointLimit->Uhgsx,2);
	strParam.append((char *)&pMPointLimit->Uhgxx,2);
	strParam.append((char *)&pMPointLimit->Udxmx,2);
	
	strParam.append((char *)&pMPointLimit->Us_sx,2);
	strParam.push_back((char)pMPointLimit->Us_yxcx);
	strParam.append((char *)&pMPointLimit->Us_yxhf,2);

	strParam.append((char *)&pMPointLimit->Ux_xx,2);
	strParam.push_back((char)pMPointLimit->Ux_yxcx);
	strParam.append((char *)&pMPointLimit->Ux_yxhf,2);

	strParam.append((char *)&pMPointLimit->PIss_sx,3);
	strParam.push_back((char)pMPointLimit->PIss_yxcx);
	strParam.append((char *)&pMPointLimit->PIss_yxhf,2);

	strParam.append((char *)&pMPointLimit->PIs_sx,3);
	strParam.push_back((char)pMPointLimit->PIs_yxcx);
	strParam.append((char *)&pMPointLimit->PIs_yxhf,2);
	
	strParam.append((char *)&pMPointLimit->I0_sx,3);
	strParam.push_back((char)pMPointLimit->I0_yxcx);
	strParam.append((char *)&pMPointLimit->I0_yxhf,2);

	strParam.append((char *)&pMPointLimit->APss_sx,3);
	strParam.push_back((char)pMPointLimit->APss_yxcx);
	strParam.append((char *)&pMPointLimit->APss_yxhf,2);

	strParam.append((char *)&pMPointLimit->APs_sx,3);
	strParam.push_back((char)pMPointLimit->APs_yxcx);
	strParam.append((char *)&pMPointLimit->APss_yxhf,2);

	
	strParam.append((char *)&pMPointLimit->P3U_bph,2);
	strParam.push_back((char)pMPointLimit->P3U_yxcx);
	strParam.append((char *)&pMPointLimit->P3U_yxhf,2);

	strParam.append((char *)&pMPointLimit->P3I_bph,2);
	strParam.push_back((char)pMPointLimit->P3I_yxcx);
	strParam.append((char *)&pMPointLimit->P3I_yxhf,2);

	strParam.push_back((char)pMPointLimit->lxsy_timelimit);

	strResponse.push_back(strParam);

	STATUS.SaveMPointLimit();
	return true;*/
	return false;
}//
bool C_CmdwE0800200::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800200::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}//
bool C_CmdwE0800201::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800201::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}//
bool C_CmdwE0800202::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800202::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}//
bool C_CmdwE0800203::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800203::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}//
bool C_CmdwE0800204::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800204::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}//
bool C_CmdwE0800205::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo((S_WORD)(distance(iterBegin, iterEnd)),  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800205::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}//
bool C_CmdwE0800206::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800206::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}//
bool C_CmdwE0800207::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800207::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}//
bool C_CmdwE0800208::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800208::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}//
bool C_CmdwE0800209::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0800209::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}//
bool C_CmdwE080020A::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE080020A::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}//
bool C_CmdwE080020B::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE080020B::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//
bool C_CmdwE080020C::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(8,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE080020C::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//无功过补偿欠补偿告警发生阈值
bool C_CmdwE080020D::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE080020D::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//无功过补偿欠补偿告警恢复阈值
bool C_CmdwE080020E::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE080020E::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}

