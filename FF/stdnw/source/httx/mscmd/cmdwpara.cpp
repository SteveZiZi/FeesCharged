/** @file
 *  @brief ��̨ͨѶ����д����������
 *  @author 
 *  @date 2011/12/22
 *  @version 0.1
 */
#include "cmdwpara.h"
#include "channel.h"
#include "status.h"
#include "context.h"
#include "timeop.h"
#include "sysctrl.h"
#include "nwmsrule.h"
#include "cfg_path.h"

//��վͨ�Ų�����
bool C_CmdwE0000100::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(9,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000100::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 9)
		return false;

	if(WriteParam(CmdObj,STATUS.GetHttxChannel(MP_HTTX_1, HTTX_CHANNEL_1)) == false)
		return false;

	STATUS.SaveUpChannel();
	CONTEXT.SetReconnectFlag(true);
	return true;
}
bool C_CmdwE0000100::WriteParam(const S_CmdObj &CmdObj,S_CHANNEL *pMS)
{
	if (pMS != NULL && pMS->m_Type == CHANNEL_TYPE_NET)
	{
		if (CmdObj.m_DiInfo[8] == 0x02)//02��GPRS/CDMA
			pMS->m_PortSet.m_NetSet.m_NeedPPP = NET_CONNECT_PPP_YES;
		else if(CmdObj.m_DiInfo[8] == 0x04)//04��Ethernet
			pMS->m_PortSet.m_NetSet.m_NeedPPP = NET_CONNECT_PPP_NO;
		else
			return false;

		memcpy(&pMS->m_PortSet.m_NetSet.m_PortNO, &CmdObj.m_DiInfo[0], 2);
		reverse_copy(CmdObj.m_DiInfo.begin()+2, CmdObj.m_DiInfo.begin()+2+4, pMS->m_PortSet.m_NetSet.m_IP);
	}
	else
	{
		if (CmdObj.m_DiInfo[8] == 0x03)//03:PSTN
		{
		}
		else if (CmdObj.m_DiInfo[8] == 0x06)//06:RS232/RS485
		{
		}
		else if (CmdObj.m_DiInfo[8] == 0x07)//07��CSD
		{
		}
		else if (CmdObj.m_DiInfo[8] == 0x08)//08��Radio
		{
		}
		return false;
	}

	return true;
}

//��վͨ�Ų�������1
bool C_CmdwE0000101::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(9,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000101::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 9)
		return false;

	if(WriteParam(CmdObj,STATUS.GetHttxChannel(MP_HTTX_1, HTTX_CHANNEL_2)) == false)
		return false;

	STATUS.SaveUpChannel();
	CONTEXT.SetReconnectFlag(true);
	return true;
}
//��վͨ�Ų�������2
bool C_CmdwE0000102::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(9,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000102::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 9)
		return false;

	if(WriteParam(CmdObj,STATUS.GetHttxChannel(MP_HTTX_1, HTTX_CHANNEL_3)) == false)
		return false;

	STATUS.SaveUpChannel();
	CONTEXT.SetReconnectFlag(true);
	return true;
}
//���ص�ַ���ߴ����������ַ�Ͷ˿ڣ��μ���վͨ�ŵ�ַ����д(����ѡ���Ǵ��������)
bool C_CmdwE0000103::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(8,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000103::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 8)
		return false;

	S_IpConfig *pIpConfig = STATUS.GetIpConfig();
	memcpy(&pIpConfig->m_ProxyPort, &CmdObj.m_DiInfo[0], 2);
	reverse_copy(CmdObj.m_DiInfo.begin()+2, CmdObj.m_DiInfo.begin()+2+4, pIpConfig->m_ProxyIP);

	STATUS.SaveTermIPConfig();

	return true;
}
//APN
bool C_CmdwE0000104::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(16,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000104::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 16)
		return false;

	S_PppInfo *pPppInfo = STATUS.GetPppInfo();
	memcpy(&pPppInfo->m_DialInfo.m_Apn[0], &CmdObj.m_DiInfo[0], 16);
	STATUS.SavePppInfo();

	return true;
}
//APN�û���
bool C_CmdwE0000105::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(32,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000105::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 32)
		return false;

	S_PppInfo *pPppInfo = STATUS.GetPppInfo();
	memcpy(&pPppInfo->m_DialInfo.m_User[0], &CmdObj.m_DiInfo[0], 32);
	STATUS.SavePppInfo();

	return true;
}
//APN����
bool C_CmdwE0000106::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(32,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000106::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 32)
		return false;

	S_PppInfo *pPppInfo = STATUS.GetPppInfo();
	memcpy(&pPppInfo->m_DialInfo.m_Pwd[0], &CmdObj.m_DiInfo[0], 32);
	STATUS.SavePppInfo();

	return true;
}
//��������
bool C_CmdwE0000107::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000107::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 1)
		return false;

	S_HttxInfo *pHttxInfo = STATUS.GetHttxInfo(MP_HTTX_1);//ȡ��̨ͨ��1
	pHttxInfo->m_HeartBeatPeriodM = kge_bcdc(CmdObj.m_DiInfo[0]);

	STATUS.SaveHttxInfo();

	return true;
}
//�����ز����
bool C_CmdwE0000108::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000108::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_PppInfo *pPppInfo = STATUS.GetPppInfo();
	pPppInfo->m_RedialInterM = kge_bcdc(CmdObj.m_DiInfo[0]);
	STATUS.SavePppInfo();

	return true;
}
//���ߺ��ز�������ȱʡ3��,�ز�ָ�������󣬵��첻���ز�
bool C_CmdwE0000109::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000109::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_PppInfo *pPppInfo = STATUS.GetPppInfo();
	pPppInfo->m_RedialTimesPerDay = kge_bcdc(CmdObj.m_DiInfo[0]);
	STATUS.SavePppInfo();
	return true;
}
//���ӷ�ʽ 0��TCP��ʽ��1��UDP��ʽ
bool C_CmdwE000010A::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE000010A::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	for (int i = HTTX_CHANNEL_1; i < HTTX_CHANNL_MAX; i += 1)
	{
		S_CHANNEL *pMS = STATUS.GetHttxChannel(MP_HTTX_1, (S_ChannelSN)i);
		if (pMS != NULL && pMS->m_Type == CHANNEL_TYPE_NET)
		{
			if((pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP_SERVER)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_UDP_SERVER))
			{
				pMS->m_PortSet.m_NetSet.m_Type = CmdObj.m_DiInfo[0]?NET_CONNECT_UDP_SERVER:NET_CONNECT_TCP_SERVER;
			}
			else if((pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP_CLIENT)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_UDP_CLIENT))
			{
				pMS->m_PortSet.m_NetSet.m_Type = CmdObj.m_DiInfo[0]?NET_CONNECT_UDP_CLIENT:NET_CONNECT_TCP_CLIENT;			
			}
			else
			{
				pMS->m_PortSet.m_NetSet.m_Type = CmdObj.m_DiInfo[0]?NET_CONNECT_UDP:NET_CONNECT_TCP;
			}
		}
	}

	STATUS.SaveUpChannel();
	CONTEXT.SetReconnectFlag(true);
	return true;
}
//ѹ������:0����ѹ��  1��ѹ����BIN����
bool C_CmdwE000010B::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE000010B::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 1)
		return false;

	S_HttxInfo *pHttxInfo = STATUS.GetHttxInfo(MP_HTTX_1);//ȡ��̨ͨ��1
	pHttxInfo->m_CompressEnable = CmdObj.m_DiInfo[0]? true:false;

//	STATUS.SaveHttxInfo();

	return true;
}
//�������ݼ���
bool C_CmdwE000010F::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(119,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE000010F::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 119)
		return false;

	SetAllParam(CmdObj.m_DiInfo);
	return true;
}
bool C_CmdwE000010F::SetAllParam(const string &strAllParam)
{
	string::const_iterator iterBegin = strAllParam.begin();
	//��վͨ�Ų���
	for (int i = HTTX_CHANNEL_1; i < HTTX_CHANNL_MAX; i += 1)
	{
		S_CmdObj CmdObj;

		CmdObj.m_DiInfo.append(iterBegin, iterBegin+9);
		WriteParam(CmdObj, STATUS.GetHttxChannel(MP_HTTX_1, (S_ChannelSN)i));
		advance(iterBegin, 9);
	}
	//���ص�ַ���ߴ����������ַ�Ͷ˿ڣ��μ���վͨ�ŵ�ַ����д(����ѡ���Ǵ��������)
	S_IpConfig *pIpConfig = STATUS.GetIpConfig();
	pIpConfig->m_ProxyPort = KGE_COMBWORD(*(iterBegin+1), *(iterBegin+0));
	reverse_copy(iterBegin+2, iterBegin+2+4, pIpConfig->m_ProxyIP);
	advance(iterBegin, 8);
	//APN
	S_PppInfo *pPppInfo = STATUS.GetPppInfo();
	copy(iterBegin, iterBegin+16, pPppInfo->m_DialInfo.m_Apn);
	advance(iterBegin, 16);
	//APN�û���
	copy(iterBegin, iterBegin+32, pPppInfo->m_DialInfo.m_User);
	advance(iterBegin, 32);
	//APN����
	copy(iterBegin, iterBegin+32, pPppInfo->m_DialInfo.m_Pwd);
	advance(iterBegin, 32);
	//��������
	S_HttxInfo *pHttxInfo = STATUS.GetHttxInfo(MP_HTTX_1);//ȡ��̨ͨ��1
	pHttxInfo->m_HeartBeatPeriodM = kge_bcdc(*iterBegin);
	advance(iterBegin, 1);
	//�����ز����
	pPppInfo->m_RedialInterM = kge_bcdc(*iterBegin);
	advance(iterBegin, 1);
	//���ߺ��ز�������ȱʡ3��,�ز�ָ�������󣬵��첻���ز�
	pPppInfo->m_RedialTimesPerDay = kge_bcdc(*iterBegin);
	advance(iterBegin, 1);
	//���ӷ�ʽ 0��TCP��ʽ��1��UDP��ʽ
	for (int i = HTTX_CHANNEL_1; i < HTTX_CHANNL_MAX; i += 1)
	{
		S_CHANNEL *pMS = STATUS.GetHttxChannel(MP_HTTX_1, (S_ChannelSN)i);
		if (pMS != NULL && pMS->m_Type == CHANNEL_TYPE_NET)
		{
			if((pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP_SERVER)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_UDP_SERVER))
			{
				pMS->m_PortSet.m_NetSet.m_Type = *iterBegin?NET_CONNECT_UDP_SERVER:NET_CONNECT_TCP_SERVER;
			}
			else if((pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP_CLIENT)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_UDP_CLIENT))
			{
				pMS->m_PortSet.m_NetSet.m_Type = *iterBegin?NET_CONNECT_UDP_CLIENT:NET_CONNECT_TCP_CLIENT;			
			}
			else
			{
				pMS->m_PortSet.m_NetSet.m_Type = *iterBegin?NET_CONNECT_UDP:NET_CONNECT_TCP;
			}
		}
	}

	STATUS.SaveTermIPConfig();
	STATUS.SaveHttxInfo();
	STATUS.SavePppInfo();
	STATUS.SaveUpChannel();
	CONTEXT.SetReconnectFlag(true);

	return true;
}
//�ն˵���������
bool C_CmdwE0000120::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(3,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000120::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 3)
		return false;

	S_SYSCFG *pSystemConfig = STATUS.GetSystemConfig();
	C_RTUA rtua(pSystemConfig->m_RTUA);

	string strRtua;
	rtua.PrintString(strRtua);
	copy(CmdObj.m_DiInfo.begin(), CmdObj.m_DiInfo.end(), strRtua.begin());
	rtua.ParseString(strRtua);
	pSystemConfig->m_RTUA = rtua.GetRTUA();

	STATUS.SaveSystemConfig();
	return true;
}
//�ն˵�ַ
bool C_CmdwE0000121::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(3,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000121::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 3)
		return false;

	S_SYSCFG *pSystemConfig = STATUS.GetSystemConfig();
	C_RTUA rtua(pSystemConfig->m_RTUA);

	string strRtua;
	rtua.PrintString(strRtua);
	copy(CmdObj.m_DiInfo.begin(), CmdObj.m_DiInfo.end(), strRtua.begin()+3);
	rtua.ParseString(strRtua);
	pSystemConfig->m_RTUA = rtua.GetRTUA();

	STATUS.SaveSystemConfig();
	return true;
}
//�ն�ͨ�Ź���ģʽ
bool C_CmdwE0000122::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000122::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 1)
		return false;

	for (int i = HTTX_CHANNEL_1; i < HTTX_CHANNL_MAX; i += 1)
	{
		S_CHANNEL *pMS = STATUS.GetHttxChannel(MP_HTTX_1, (S_ChannelSN)i);
		if (pMS != NULL && pMS->m_Type == CHANNEL_TYPE_NET)
		{
			switch(CmdObj.m_DiInfo[0])
			{
			case 0:
				if((pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP_SERVER)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP_CLIENT)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP))
					pMS->m_PortSet.m_NetSet.m_Type = NET_CONNECT_TCP;
				else
					pMS->m_PortSet.m_NetSet.m_Type = NET_CONNECT_UDP;
				break;
			case 1:
				if((pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP_SERVER)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP_CLIENT)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP))
					pMS->m_PortSet.m_NetSet.m_Type = NET_CONNECT_TCP_CLIENT;
				else
					pMS->m_PortSet.m_NetSet.m_Type = NET_CONNECT_UDP_CLIENT;
				break;
			case 2:
				if((pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP_SERVER)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP_CLIENT)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP))
					pMS->m_PortSet.m_NetSet.m_Type = NET_CONNECT_TCP_SERVER;
				else
					pMS->m_PortSet.m_NetSet.m_Type = NET_CONNECT_UDP_SERVER;
				break;
			}		
		}
	}
	STATUS.SaveUpChannel();
	CONTEXT.SetReconnectFlag(true);
	return true;
}
//���ģʽ�ͷ�����ģʽ�£�TCP�����˿ںţ�BIN����
bool C_CmdwE0000123::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000123::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 2)
		return false;

	bool Setflag = false;
	for (int i = HTTX_CHANNEL_1; i < HTTX_CHANNL_MAX; i += 1)
	{
		S_CHANNEL *pMS = STATUS.GetHttxChannel(MP_HTTX_1, (S_ChannelSN)i);
		if (pMS != NULL && pMS->m_Type == CHANNEL_TYPE_NET)
		{
			if((pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_UDP)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP_SERVER))
			{
				memcpy(&pMS->m_PortSet.m_NetSet.m_PortNO,&CmdObj.m_DiInfo[0],2);
				Setflag = true;//��һ�����óɹ�
			}
		}
	}
	if (Setflag == true)
	{
		STATUS.SaveUpChannel();
		CONTEXT.SetReconnectFlag(true);
	}
	return Setflag;
}
//�ն˵�ǰͨ��IP��ַ
bool C_CmdwE0000124::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(4,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000124::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 4)
		return false;

	char buffer[CFG_PATH_FILE_MAX_LEN];
	S_IpConfig *pIpConfig = STATUS.GetIpConfig();
	reverse_copy(CmdObj.m_DiInfo.begin(), CmdObj.m_DiInfo.end(), pIpConfig->m_TermIP);

	STATUS.SaveTermIPConfig();
	C_SYSCTRL::ExecScript(C_PATH::GetScriptFile(CFG_FILE_SCRIPT_IP_CONFIG, buffer));

	return true;
}
//�ն˵�ǰͨ�����������ַ
bool C_CmdwE0000125::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(4,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000125::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 4)
		return false;

	char buffer[CFG_PATH_FILE_MAX_LEN];
	S_IpConfig *pIpConfig = STATUS.GetIpConfig();
	reverse_copy(CmdObj.m_DiInfo.begin(), CmdObj.m_DiInfo.end(), pIpConfig->m_NetMask);

	STATUS.SaveTermIPConfig();
	C_SYSCTRL::ExecScript(C_PATH::GetScriptFile(CFG_FILE_SCRIPT_IP_CONFIG, buffer));

	return true;
}
//�ն˵�ǰͨ�����ص�ַ
bool C_CmdwE0000126::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(4,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000126::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 4)
		return false;

	S_IpConfig *pIpConfig = STATUS.GetIpConfig();
	reverse_copy(CmdObj.m_DiInfo.begin(), CmdObj.m_DiInfo.end(), pIpConfig->m_GateWay);

	char buffer[CFG_PATH_FILE_MAX_LEN];
	STATUS.SaveTermIPConfig();
	C_SYSCTRL::ExecScript(C_PATH::GetScriptFile(CFG_FILE_SCRIPT_IP_CONFIG, buffer));

	return true;
}
//��������IP��ȡ��ʽ
bool C_CmdwE0000127::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000127::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 1)
		return false;

	S_IpConfig *pIpConfig = STATUS.GetIpConfig();
	pIpConfig->m_IpMethod = CmdObj.m_DiInfo[0];

//	char buffer[CFG_PATH_FILE_MAX_LEN];
	STATUS.SaveTermIPConfig();
//	C_SYSCTRL::ExecScript(C_PATH::GetScriptFile(CFG_FILE_SCRIPT_IP_CONFIG, buffer));

	return true;
}

//�������ݺϼ�
bool C_CmdwE000012F::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(19,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE000012F::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 22)
		return false;

	SetAllParam(CmdObj.m_DiInfo);
	return true;
}
bool C_CmdwE000012F::SetAllParam(const string &strAllParam)
{
	string::const_iterator iterBegin = strAllParam.begin();
	//�ն˵��������룬BCD�� + �ն˵�ַ��BIN����
	C_RTUA rtua;
	char buffer[CFG_PATH_FILE_MAX_LEN];
	string strRtua(iterBegin, iterBegin+6);
	S_SYSCFG *pSystemConfig = STATUS.GetSystemConfig();
	strRtua.push_back(0);
	rtua.ParseString(strRtua);
	pSystemConfig->m_RTUA = rtua.GetRTUA();
	advance(iterBegin, 6);
	//�ն�ͨ�Ź���ģʽ
	for (int i = HTTX_CHANNEL_1; i < HTTX_CHANNL_MAX; i += 1)
	{
		S_CHANNEL *pMS = STATUS.GetHttxChannel(MP_HTTX_1, (S_ChannelSN)i);
		if (pMS != NULL && pMS->m_Type == CHANNEL_TYPE_NET)
		{
			switch(*iterBegin)
			{
			case 0:
				if((pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP_SERVER)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP_CLIENT)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP))
					pMS->m_PortSet.m_NetSet.m_Type = NET_CONNECT_TCP;
				else
					pMS->m_PortSet.m_NetSet.m_Type = NET_CONNECT_UDP;
				break;
			case 1:
				if((pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP_SERVER)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP_CLIENT)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP))
					pMS->m_PortSet.m_NetSet.m_Type = NET_CONNECT_TCP_CLIENT;
				else
					pMS->m_PortSet.m_NetSet.m_Type = NET_CONNECT_UDP_CLIENT;
				break;
			case 2:
				if((pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP_SERVER)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP_CLIENT)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP))
					pMS->m_PortSet.m_NetSet.m_Type = NET_CONNECT_TCP_SERVER;
				else
					pMS->m_PortSet.m_NetSet.m_Type = NET_CONNECT_UDP_SERVER;
				break;
			}	
		    //���ģʽ�ͷ�����ģʽ�£�TCP�����˿ںţ�BIN����
		    if((pMS->m_PortSet.m_NetSet.m_Type != NET_CONNECT_TCP_CLIENT)||(pMS->m_PortSet.m_NetSet.m_Type != NET_CONNECT_UDP_CLIENT))
				pMS->m_PortSet.m_NetSet.m_PortNO = KGE_COMBWORD(*(iterBegin+2), *(iterBegin+1));
		}
	}
	advance(iterBegin, 3);

	//�ն˵�ǰͨ��IP��ַ
	S_IpConfig *pIpConfig = STATUS.GetIpConfig();
	reverse_copy(iterBegin, iterBegin+4, pIpConfig->m_TermIP);
	advance(iterBegin, 4);
	//�ն˵�ǰͨ�����������ַ
	reverse_copy(iterBegin, iterBegin+4, pIpConfig->m_NetMask);
	advance(iterBegin, 4);
	//�ն˵�ǰͨ�����ص�ַ
	reverse_copy(iterBegin, iterBegin+4, pIpConfig->m_GateWay);
	advance(iterBegin, 4);
	//��������IP��ȡ��ʽ
	pIpConfig->m_IpMethod = *iterBegin;
	advance(iterBegin, 1);

	STATUS.SaveSystemConfig();
	STATUS.SaveTermIPConfig();
	STATUS.SaveUpChannel();
	CONTEXT.SetReconnectFlag(true);
	C_SYSCTRL::ExecScript(C_PATH::GetScriptFile(CFG_FILE_SCRIPT_IP_CONFIG, buffer));

	return true;
}
//�ն�ʱ��
bool C_CmdwE0000130::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(6,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000130::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 6)
		return false;
	char bcdStr7B[7];
//	reverse_copy(CmdObj.m_DiInfo.begin(), CmdObj.m_DiInfo.end(), bcdStr7B);
	copy(CmdObj.m_DiInfo.begin(), CmdObj.m_DiInfo.end(), bcdStr7B);
	bcdStr7B[6] = 0x20;
	if (C_TIME::SetHWClock(bcdStr7B) < 0)
		return false;
	return true;
}
//ʵʱУʱ
bool C_CmdwE0000131::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(4,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000131::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 4)
		return false;

//	string strSetTimeData;
//	strSetTimeData.append((char *)&CmdObj.m_DA, 2);
//	strSetTimeData.append((char *)&CmdObj.m_DI, 4);	
//	strSetTimeData.append((char *)&CmdObj.m_DiInfo[0], 4);

	//���������ΪȫFF���Ƕ����ߵ��㲥Уʱ
	if (CmdObj.m_DiInfo[0] == 0xff && CmdObj.m_DiInfo[1] == 0xff)
	{
		logvt<<"�㲥Уʱ֪ͨ...."<<endl;
		for (int i = MP_DBCJ_1; i < DBCJ_PROCESS_MAX; i++)
		{
			S_TaskStatus* pTaskStatus = STATUS.GetTaskStatus((S_DbcjSN)i);
			if (pTaskStatus != NULL)
			{
				S_TaskFreezeStatus *pTaskFreezeStatus = pTaskStatus->GetTaskFreezeStatus((S_DWORD)MSDI_TASK_BROADCAST);
				if (pTaskFreezeStatus != NULL)
					pTaskFreezeStatus->m_timeLastRaise = 1;//�����㲥Уʱ����
			}
		}
	}

	//�жϱ��ǽ��ɱ���
//	if (C_SysMsg::Send2Server(MP_PROCESS_DBCJ1, MSAFN_WRITE_PARAM, strSetTimeData) != 0)
//		return false;
//	strSetTimeData.clear();
//	if (C_SysMsg::Wait4Server(MSAFN_WRITE_PARAM, strSetTimeData) != 0)
//		return false;

	return true;
}
//��λ���� �ն˸�λ��00  ���п�д�Ĳ����ָ��������ã�ͨ�Ų������⣩01  ��������λ�������ʷ���ݣ�02  �ն�Ӳ����λ
bool C_CmdwE0000140::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000140::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 1)
		return false;
	if(CmdObj.m_DiInfo[0] == 0x00)//���п�д�Ĳ����ָ��������ã�ͨ�Ų������⣩
		C_SYSCTRL::ParamReset();
	else if(CmdObj.m_DiInfo[0] == 0x01)//��������λ�������ʷ���ݣ�
		C_SYSCTRL::DataClear();
	else if(CmdObj.m_DiInfo[0] == 0x02)//�ն�Ӳ����λ
		C_SYSCTRL::HardwareReset();
	else
		return false;
	return true;
}
//�澯�����ϱ�������
bool C_CmdwE0000150::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(32,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000150::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 32)
		return false;

	string strAlarmInfo;
	S_AlarmInfo *pAlarmInfo = STATUS.GetAlarmInfo();
	C_BITSET<32> ReportAlarmEven;
	ReportAlarmEven.SetBit((unsigned char *)&CmdObj.m_DiInfo[0],32);
	ReportAlarmEven.PrintString(strAlarmInfo);
	pAlarmInfo->m_ReportAlarmEven.reset();
	pAlarmInfo->m_ReportAlarmEven.ParseParameter(strAlarmInfo);

	STATUS.SaveAlarmInfo();

	return true;
}
//�澯�ж�������
bool C_CmdwE0000151::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(32,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000151::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 32)
		return false;

	string strAlarmInfo;
	S_AlarmInfo *pAlarmInfo = STATUS.GetAlarmInfo();
	C_BITSET<32> AlarmEven;
	AlarmEven.SetBit((unsigned char *)&CmdObj.m_DiInfo[0],32);
	AlarmEven.PrintString(strAlarmInfo);
	pAlarmInfo->m_AlarmEven.reset();
	pAlarmInfo->m_AlarmEven.ParseParameter(strAlarmInfo);

	STATUS.SaveAlarmInfo();
	CONTEXT.NotifyDbcj(NOTIFY_DBCJ_ADD_ALARM);
	return true;
}
//�¼���¼������
bool C_CmdwE0000152::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(32,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000152::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 32)
		return false;

	string strAlarmInfo;
	S_AlarmInfo *pAlarmInfo = STATUS.GetAlarmInfo();
	C_BITSET<32> OnlyEvent;
	OnlyEvent.SetBit((unsigned char *)&CmdObj.m_DiInfo[0],32);
	OnlyEvent.PrintString(strAlarmInfo);
	pAlarmInfo->m_OnlyEvent.reset();
	pAlarmInfo->m_OnlyEvent.ParseParameter(strAlarmInfo);

	STATUS.SaveAlarmInfo();
	CONTEXT.NotifyDbcj(NOTIFY_DBCJ_ADD_ALARM);
	return true;
}
//�¶������ DD������ʱ����գ���Χ1~30��ȱʡΪÿ��1��HH������ʱ���Сʱ����Χ0~23��ȱʡΪ0��
bool C_CmdwE0000160::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000160::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_DbcjInfo *pDbcjInfo = STATUS.GetDbcjInfo(MP_DBCJ_1);
	pDbcjInfo->m_MonBeginDate = CmdObj.m_DiInfo[0];
	pDbcjInfo->m_MonBeginTime[0] = CmdObj.m_DiInfo[1];
	pDbcjInfo->m_MonBeginTime[1] = 0x00;

	STATUS.SaveDbcjInfo();

	return true;
}
//�ն������ HH���յ����������룩����ʱ�̣���Χ0~23��ȱʡΪ0��
bool C_CmdwE0000161::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000161::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_DbcjInfo *pDbcjInfo = STATUS.GetDbcjInfo(MP_DBCJ_1);
	pDbcjInfo->m_DayBeginTime[0] = CmdObj.m_DiInfo[0];
	pDbcjInfo->m_DayBeginTime[1] = 0x00;

	STATUS.SaveDbcjInfo();

	return true;
}
//�µ�ѹ�ϸ������ݶ��� DD������ʱ����գ���Χ1~30��ȱʡΪÿ��21��HH������ʱ���Сʱ����Χ0~23��ȱʡΪ0��
bool C_CmdwE0000162::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000162::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_DbcjInfo *pDbcjInfo = STATUS.GetDbcjInfo(MP_DBCJ_1);
	pDbcjInfo->m_MonVolFreezeDate = CmdObj.m_DiInfo[0];
	pDbcjInfo->m_MonVolFreezeTime[0] = CmdObj.m_DiInfo[1];
	pDbcjInfo->m_MonVolFreezeTime[1] = 0x00;

	STATUS.SaveDbcjInfo();

	return false;
}
//�յ�ѹ�ϸ������ݶ��� HH���յ����������룩����ʱ�̣���Χ0~23��ȱʡΪ0��
bool C_CmdwE0000163::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000163::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_DbcjInfo *pDbcjInfo = STATUS.GetDbcjInfo(MP_DBCJ_1);
	pDbcjInfo->m_DayVolFreezeTime[0] = CmdObj.m_DiInfo[0];
	pDbcjInfo->m_DayVolFreezeTime[1] = 0x00;

	STATUS.SaveDbcjInfo();

	return false;
}
//��Ϣ��֤���ֶ�PWУ�鷽ʽ (ֻд)
bool C_CmdwE0000170::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000170::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//
bool C_CmdwE0000171::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(16,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000171::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//
bool C_CmdwE0000172::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(16,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000172::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//
bool C_CmdwE0000173::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(16,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000173::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//
bool C_CmdwE0000174::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(16,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000174::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//�жϸ��ɹ��ء����������ɡ�����ѹA������ͬ�����õ硢���ʳ���ֵ��������ƽ��ȡ���ѹ���䡢�������䡢�������ƫ���޹�Ƿ�������޹���������ʱ��ȱʡ15����
bool C_CmdwE0000180::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000180::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_PowerLimit* info = STATUS.GetPowerLimitInfo();
	info->m_OverLoadM=kge_bcdc((S_BYTE)CmdObj.m_DiInfo[0]);
	STATUS.SavePowerLimitInfo();

	return true;
}
//�жϸ��ɹ��ء����������ɡ�����ѹ����ͬ�����õ硢���ʳ���ֵ��������ƽ��ȡ���ѹ���䡢�������䡢�������ƫ���޹�Ƿ�������޹��������ָ�ʱ��ȱʡ15����
bool C_CmdwE0000181::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000181::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_PowerLimit* info = STATUS.GetPowerLimitInfo();
	info->m_OverLoadEndM=kge_bcdc((S_BYTE)CmdObj.m_DiInfo[0]);
	STATUS.SavePowerLimitInfo();
	return true;
}
//��ֵ�������ж϶��ࡢʧѹ��ʧ��ʱ�䣬ȱʡֵ1����
bool C_CmdwE0000182::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000182::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_PowerLimit* info = STATUS.GetPowerLimitInfo();
	info->m_MissingPhaseM=kge_bcdc((S_BYTE)CmdObj.m_DiInfo[0]);
	STATUS.SavePowerLimitInfo();
	return true;
}
//��ֵ�������ж϶��ࡢʧѹ��ʧ���ָ�ʱ�䣬ȱʡֵ1����
bool C_CmdwE0000183::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000183::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_PowerLimit* info = STATUS.GetPowerLimitInfo();
	info->m_MissingPhaseEndM=kge_bcdc((S_BYTE)CmdObj.m_DiInfo[0]);
	STATUS.SavePowerLimitInfo();
	return true;
}
//��ֵ�������жϵ�ѹ������ʱ�䣬ȱʡֵ15����
bool C_CmdwE0000184::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000184::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_PowerLimit* info = STATUS.GetPowerLimitInfo();
	info->m_ReversedPhaseM=kge_bcdc((S_BYTE)CmdObj.m_DiInfo[0]);
	STATUS.SavePowerLimitInfo();
	return true;
}
//��ֵ�������жϵ�ѹ������ָ�ʱ�䣬ȱʡֵ15����
bool C_CmdwE0000185::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000185::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_PowerLimit* info = STATUS.GetPowerLimitInfo();
	info->m_ReversedPhaseEndM=kge_bcdc((S_BYTE)CmdObj.m_DiInfo[0]);
	STATUS.SavePowerLimitInfo();
	return true;
}
//��ֵ�������жϵ���������ʱ�䣬ȱʡֵ15����
bool C_CmdwE0000186::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000186::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_PowerLimit* info = STATUS.GetPowerLimitInfo();
	info->m_ReversedPolarityM=kge_bcdc((S_BYTE)CmdObj.m_DiInfo[0]);
	STATUS.SavePowerLimitInfo();
	return true;
}
//��ֵ�������жϵ��������Իָ�ʱ�䣬ȱʡֵ15����
bool C_CmdwE0000187::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000187::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_PowerLimit* info = STATUS.GetPowerLimitInfo();
	info->m_ReversedPolarityEndM=kge_bcdc((S_BYTE)CmdObj.m_DiInfo[0]);
	STATUS.SavePowerLimitInfo();
	return true;
}
//��ֵ���������ܱ�ʱ���쳣��ֵ��ȱʡֵ10����
bool C_CmdwE0000188::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000188::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_PowerLimit* info = STATUS.GetPowerLimitInfo();
	info->m_MeterTimeDiffM=kge_bcdc((S_BYTE)CmdObj.m_DiInfo[0]);
	STATUS.SavePowerLimitInfo();
	return true;
}
//��ֵ���������ܱ������ֵ��ȱʡΪ10��
bool C_CmdwE0000189::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000189::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_PowerLimit* info = STATUS.GetPowerLimitInfo();
	info->m_MeterFly=kge_bcdc((S_BYTE)CmdObj.m_DiInfo[0]);
	STATUS.SavePowerLimitInfo();
	return true;
}
//��ֵ���������ܱ�ͣ����ֵ������ʾ��ֹͣ����ʱ�����й����ʼ���Ӧ�ߵ���ֵ��Խ��ֵ����ܱ�ͣ�ߣ�ȱʡΪ0.1kWh
bool C_CmdwE000018A::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE000018A::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_PowerLimit* info = STATUS.GetPowerLimitInfo();
	memcpy((S_BYTE*)(&info->m_MeterStop),CmdObj.m_DiInfo.c_str(),2);
	STATUS.SavePowerLimitInfo();
	return true;
}
//��ֵ������ʣ�������ֵ��ȱʡΪ20.00 Ԫ
bool C_CmdwE000018B::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(3,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE000018B::Action(const S_CmdObj &CmdObj, strings &strResponse)
{//
	if (CmdObj.m_DiInfo.size() != 3)
		return false;

	S_PowerLimit* info = STATUS.GetPowerLimitInfo();
	copy(CmdObj.m_DiInfo.begin(), CmdObj.m_DiInfo.end(), (char *)&info->m_MeterLastFee);

	STATUS.SavePowerLimitInfo();
	return true;
}
//��ֵ��������ͨ������Խ��ֵ
bool C_CmdwE000018C::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(3,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE000018C::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_HttxInfo *pHttxInfo=STATUS.GetHttxInfo(MP_HTTX_1);
	pHttxInfo->m_MonTrafficLimit = C_BusiAlgo::ChangeBcd2Dword((S_BYTE *)CmdObj.m_DiInfo.data(), 3*2);
	pHttxInfo->m_MonTrafficLimit *= 1024;//�·���λΪKB ת�����ֽ�

	STATUS.SaveHttxInfo();
	return true;
}
//�����������
bool C_CmdwE000018F::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(18,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE000018F::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strParam(CmdObj.m_DiInfo.begin(), CmdObj.m_DiInfo.begin()+18);
	SetAllParam(strParam);

	return true;
}
bool C_CmdwE000018F::SetAllParam(const string &strAllParam)
{
	S_PowerLimit *pPowerLimit = STATUS.GetPowerLimitInfo();
	S_HttxInfo *pHttxInfo=STATUS.GetHttxInfo(MP_HTTX_1);

	pPowerLimit->m_OverLoadM = kge_bcdc((S_BYTE)strAllParam[0]);
	pPowerLimit->m_OverLoadEndM = kge_bcdc((S_BYTE)strAllParam[1]);
	pPowerLimit->m_MissingPhaseM = kge_bcdc((S_BYTE)strAllParam[2]);
	pPowerLimit->m_MissingPhaseEndM = kge_bcdc((S_BYTE)strAllParam[3]);
	pPowerLimit->m_ReversedPhaseM = kge_bcdc((S_BYTE)strAllParam[4]);
	pPowerLimit->m_ReversedPhaseEndM = kge_bcdc((S_BYTE)strAllParam[5]);
	pPowerLimit->m_ReversedPolarityM = kge_bcdc((S_BYTE)strAllParam[6]);
	pPowerLimit->m_ReversedPolarityEndM = kge_bcdc((S_BYTE)strAllParam[7]);
	pPowerLimit->m_MeterTimeDiffM = kge_bcdc((S_BYTE)strAllParam[8]);
	pPowerLimit->m_MeterFly = kge_bcdc((S_BYTE)strAllParam[9]);
	memcpy((S_BYTE*)&pPowerLimit->m_MeterStop,&strAllParam[10],2); // BCD��
	memcpy((S_BYTE*)&pPowerLimit->m_MeterLastFee,&strAllParam[12],3); // BCD��

	pHttxInfo->m_MonTrafficLimit = C_BusiAlgo::ChangeBcd2Dword((S_BYTE *)&strAllParam[15], 3*2);
	pHttxInfo->m_MonTrafficLimit *= 1024;//�·���λΪKB ת�����ֽ�

	STATUS.SavePowerLimitInfo();

	return true;
}
//��������Զ�Ӧ�ĳ���������ñ�
bool C_CmdwE0000222::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	if (distance(iterBegin, iterEnd) < 9)
		return false;
	for(int i=0;i<CONS_TASK_COUNT_MAX;i++)
	{
		if (distance(iterBegin, iterEnd) < 9){
			if(CmdObj.m_DiInfo.size()>11)
			{
				break;
			}
			else
			{
				return false;
			}
		}
		CmdObj.m_DiInfo.append(iterBegin, iterBegin+9);	
		advance(iterBegin, 9);

		S_LONG num=(S_LONG)CmdObj.m_DiInfo[8];
		CmdObj.m_DiInfo.append(iterBegin, iterBegin+num*4);	
		advance(iterBegin, num*4);
	}
	return true;

}

bool C_CmdwE0000222::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() <11)
		return false;

	S_GatherTaskInfos *pGatherTaskInfos = STATUS.GetGatherTaskInfos();
	S_LONG 	index=0;
	for(int i = 0; i < CONS_TASK_COUNT_MAX; i++)
	{
		if ((CmdObj.m_DiInfo.size()-index) < 9){
			if(i>0)
				break;
			else
				return false;
		}

		S_GatherTaskInfo &GatherTaskInfo = pGatherTaskInfos->m_ItemInfo[i];
		GatherTaskInfo.m_Enable=((S_BYTE)CmdObj.m_DiInfo[index++]==0X00)?false:true;
		string stemp;
		S_DWORD integer=0;
		S_WORD Float=0;
		stemp.append(CmdObj.m_DiInfo.begin()+index,CmdObj.m_DiInfo.begin()+index+2);
		index += 2;
		C_BCD bcdtemp(stemp);
		bcdtemp.GetBcdData(integer, Float);
		GatherTaskInfo.m_RecordMax=((S_WORD)integer);

		GatherTaskInfo.m_MeterType=(S_BYTE)CmdObj.m_DiInfo[index++];
		GatherTaskInfo.m_VipFlag=(S_BYTE)CmdObj.m_DiInfo[index++];
		GatherTaskInfo.m_DaCnt=(S_WORD)CmdObj.m_DiInfo[index++];

		if((int)(CmdObj.m_DiInfo.size()-index) < GatherTaskInfo.m_DaCnt*2)
			return false;
		for(S_BYTE pn=0; pn < GatherTaskInfo.m_DaCnt;pn++)
		{
			GatherTaskInfo.m_DA[pn] = KGE_COMBWORD(CmdObj.m_DiInfo[index+1], CmdObj.m_DiInfo[index]);
			index += 2;
		}

		GatherTaskInfo.m_DtCnt=(S_WORD)CmdObj.m_DiInfo[index++];
		if((int)(CmdObj.m_DiInfo.size()-index) < GatherTaskInfo.m_DtCnt*4)
			return false;
		for(S_BYTE tn = 0; tn < GatherTaskInfo.m_DtCnt; tn++)
		{
			S_WORD m = KGE_COMBWORD(CmdObj.m_DiInfo[index+1], CmdObj.m_DiInfo[index+0]);
			S_WORD n = KGE_COMBWORD(CmdObj.m_DiInfo[index+3], CmdObj.m_DiInfo[index+2]);
			GatherTaskInfo.m_DT[tn] = KGE_COMBDWORD(n, m);
			index += 4;
		}
	}	

	STATUS.SaveGatherTasks();
	CONTEXT.NotifyDbcj(NOTIFY_DBCJ_ADD_TASK);
	return true;
}

//ģ��������
bool C_CmdwE0000500::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{//ģ��������
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000500::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_ANALOGS *pANALOGS = STATUS.GetAnalogInfos();
	pANALOGS->m_AnalogCnt = CmdObj.m_DiInfo[0];

	STATUS.SaveAnalogInfo();

	return true;
}
//ģ����״̬���
bool C_CmdwE00005FE::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(32,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE00005FE::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_BITSET<32> bANALOGS;
	S_ANALOGS *pANALOGS = STATUS.GetAnalogInfos();
	bANALOGS.SetBit(&CmdObj.m_DiInfo[0],32);
	for(int i=0;i<256;i++)
	{
		pANALOGS->m_Item[i].m_Enable = bANALOGS.TestBit(i);
	}

	STATUS.SaveAnalogInfo();

	return true;
}
//�������ݼ���
bool C_CmdwE00005FF::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	if (distance(iterBegin, iterEnd) < 1)
		return false;
	CmdObj.m_DiInfo.append(iterBegin, iterBegin+1);
	advance(iterBegin, 1);
	S_BYTE num=(S_BYTE)CmdObj.m_DiInfo[0];
	if (distance(iterBegin, iterEnd) < 3*num)
		return false;
	CmdObj.m_DiInfo.append(iterBegin, iterBegin+3*num);
	advance(iterBegin, 3*num);

	if (distance(iterBegin, iterEnd) < 32)
		return false;
	CmdObj.m_DiInfo.append(iterBegin, iterBegin+32);
	advance(iterBegin, 32);
	return true;

}
bool C_CmdwE00005FF::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//����������
bool C_CmdwE0000600::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000600::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_PULSES *pPULSES = STATUS.GetPulseInfos();
	pPULSES->m_PulseCnt = CmdObj.m_DiInfo[0];

	STATUS.SavePulseInfo();

	return true;
}
//������״̬���
bool C_CmdwE00006FE::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE00006FE::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_BITSET<32> bPULSES ;
	S_PULSES *pPULSES = STATUS.GetPulseInfos();
	bPULSES.SetBit(&CmdObj.m_DiInfo[0],32);
	for(int i=0;i<256;i++)
	{
		pPULSES->m_Item[i].m_Enable = bPULSES.TestBit(i);
	}

	STATUS.SavePulseInfo();

	return true;
}
//�������ݼ���
bool C_CmdwE00006FF::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	if (distance(iterBegin, iterEnd) < 1)
		return false;
	CmdObj.m_DiInfo.append(iterBegin, iterBegin+1);
	advance(iterBegin, 1);
	
	S_BYTE num=(S_BYTE)CmdObj.m_DiInfo[0];
	if (distance(iterBegin, iterEnd) < 6*num)
		return false;
	CmdObj.m_DiInfo.append(iterBegin, iterBegin+6*num);
	advance(iterBegin, 6*num);

	if (distance(iterBegin, iterEnd) < 32)
		return false;
	CmdObj.m_DiInfo.append(iterBegin, iterBegin+32);
	advance(iterBegin, 32);
	return true;
}
bool C_CmdwE00006FF::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//����������
bool C_CmdwE0000700::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000700::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_SUMS *pSUMS = STATUS.GetSumInfos();
	pSUMS->m_SumCnt = CmdObj.m_DiInfo[0];

	STATUS.SaveSumInfo();

	return true;
}
//������״̬���
bool C_CmdwE00007FE::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(32,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE00007FE::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_BITSET<32> bSUMS;
	S_SUMS *pSUMS = STATUS.GetSumInfos();
	bSUMS.SetBit(&CmdObj.m_DiInfo[0],32);
	for(int i=0;i<256;i++)
	{
		pSUMS->m_Item[i].m_Enable = bSUMS.TestBit(i);
	}

	STATUS.SaveSumInfo();

	return true;
}
//�������ݼ���
bool C_CmdwE00007FF::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	if (distance(iterBegin, iterEnd) < 1)
		return false;
	CmdObj.m_DiInfo.append(iterBegin, iterBegin+1);
	advance(iterBegin, 1);
	
	S_BYTE num=(S_BYTE)CmdObj.m_DiInfo[0];
	if (distance(iterBegin, iterEnd) < 30*num)
		return false;
	CmdObj.m_DiInfo.append(iterBegin, iterBegin+30*num);
	advance(iterBegin, 30*num);

	if (distance(iterBegin, iterEnd) < 32)
		return false;
	CmdObj.m_DiInfo.append(iterBegin, iterBegin+32);
	advance(iterBegin, 32);
	return true;

}
bool C_CmdwE00007FF::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//���������
bool C_CmdwE0000800::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000800::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_DiffRuleS *pDiffRuleS = STATUS.GetDiffRuleInfos();
	pDiffRuleS->m_DiffRuleCnt = CmdObj.m_DiInfo[0];

	STATUS.SaveDiffRuleInfo();

	return true;
}
//�����״̬���
bool C_CmdwE00008FE::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(32,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE00008FE::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_BITSET<32> bDiffRuleS;
	S_DiffRuleS *pDiffRuleS = STATUS.GetDiffRuleInfos();
	bDiffRuleS.SetBit(&CmdObj.m_DiInfo[0],32);
	for(int i=0;i<256;i++)
	{
		pDiffRuleS->m_Item[i].m_Enable = bDiffRuleS.TestBit(i);
	}

	STATUS.SaveDiffRuleInfo();

	return true;
}
//�������ݼ���
bool C_CmdwE00008FF::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	if (distance(iterBegin, iterEnd) < 1)
		return false;
	CmdObj.m_DiInfo.append(iterBegin, iterBegin+1);
	advance(iterBegin, 1);
	
	S_BYTE num=(S_BYTE)CmdObj.m_DiInfo[0];
	if (distance(iterBegin, iterEnd) < 15*num)
		return false;
	CmdObj.m_DiInfo.append(iterBegin, iterBegin+15*num);
	advance(iterBegin, 15*num);

	if (distance(iterBegin, iterEnd) < 32)
		return false;
	CmdObj.m_DiInfo.append(iterBegin, iterBegin+32);
	advance(iterBegin, 32);
	return true;

}
bool C_CmdwE00008FF::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//�ն��޹�����ͳ�Ʊ�־
bool C_CmdwE0000900::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000900::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//�������ն˵�ַ1~4
bool C_CmdwE0000A20::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(16,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000A20::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_ForwardInfo *pForwardInfo = STATUS.GetForwardInfo();
	for(int i=0;i<4;i++)
	{
		memcpy(&pForwardInfo->m_SlaveAddr[i],&CmdObj.m_DiInfo[4*i],4);
	}
	STATUS.SaveForwardInfo();

	return true;
}
//���ն˵�ַ
bool C_CmdwE0000A21::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(4,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000A21::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_ForwardInfo *pForwardInfo = STATUS.GetForwardInfo();
	memcpy(&pForwardInfo->m_MasterAddr,&CmdObj.m_DiInfo[0],4);
	STATUS.SaveForwardInfo();

	return true;
}
//�����ն˵�ַ
bool C_CmdwE0000A22::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(4,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000A22::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_ForwardInfo *pForwardInfo = STATUS.GetForwardInfo();
	memcpy(&pForwardInfo->m_GatherAddr,&CmdObj.m_DiInfo[0],4);
	STATUS.SaveForwardInfo();

	return true;
}
//���ն˵ȴ����ն�Ӧ��ĳ�ʱ�ж�ʱ��
bool C_CmdwE0000A23::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000A23::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_ForwardInfo *pForwardInfo = STATUS.GetForwardInfo();
	pForwardInfo->m_WaitSlaveResponseS = CmdObj.m_DiInfo[0];
	STATUS.SaveForwardInfo();

	return true;
}
//���ն˵ȴ����ն˼�����������ĳ�ʱ�ж�ʱ��
bool C_CmdwE0000A24::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000A24::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_ForwardInfo *pForwardInfo = STATUS.GetForwardInfo();
	pForwardInfo->m_WaitMasterCtrlS = CmdObj.m_DiInfo[0];
	STATUS.SaveForwardInfo();

	return true;
}
//����̨���ն�CT����
bool C_CmdwE0000A25::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000A25::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_ForwardInfo *pForwardInfo = STATUS.GetForwardInfo();
	memcpy(&pForwardInfo->m_CT,&CmdObj.m_DiInfo[0],2);
	STATUS.SaveForwardInfo();

	return true;
}
//����̨���ն�PT����
bool C_CmdwE0000A26::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000A26::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_ForwardInfo *pForwardInfo = STATUS.GetForwardInfo();
	memcpy(&pForwardInfo->m_PT,&CmdObj.m_DiInfo[0],2);
	STATUS.SaveForwardInfo();

	return true;
}
//�Ե��ܱ�ң����բ
bool C_CmdwE0001100::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(15,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0001100::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_WORD PN = C_DAPN::DA2PN(CmdObj.m_DA);
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_MeterInfo &MeterInfo = pMeterInfos->m_Item[PN];
	string strAddress(CmdObj.m_DiInfo.begin(), CmdObj.m_DiInfo.begin()+6);
	if (strAddress.compare(0, 6, MeterInfo.m_Address, 6) != 0)
		return false;

	string strIpcData;
	AppendDADT(strIpcData, CmdObj);
	strIpcData.append(CmdObj.m_DiInfo);
	const char *pPrecess = CONTEXT.GetDbcjPrecessName_MP(PN);

	if (C_SysMsg::Send2Server(pPrecess, MSAFN_WRITE_PARAM, strIpcData) != 0)
		return false;
	return true;
}
//�Ե��ܱ�ң�غ�բ
bool C_CmdwE0001101::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(16,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0001101::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return C_CmdwE0001100::Action(CmdObj, strResponse);
}
//�Ե��ܱ������
bool C_CmdwE0001102::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(15,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0001102::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return C_CmdwE0001100::Action(CmdObj, strResponse);
}
//�Ե��ܱ���������
bool C_CmdwE0001103::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(15,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0001103::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return C_CmdwE0001100::Action(CmdObj, strResponse);
}
//ɾ��������NNNN��ʾ��ɾ���������
bool C_CmdwE0001104::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(8,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0001104::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_MpInfos *pMpInfos = STATUS.GetMpInfos();	

	S_WORD MP=KGE_COMBWORD(CmdObj.m_DiInfo[1],CmdObj.m_DiInfo[0]);
	S_MpInfo &MpInfo = pMpInfos->m_Item[MP];

	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_MeterInfo& MeterInfo=pMeterInfos->m_Item[MP];
	string stemp=CmdObj.m_DiInfo.substr(2,6);
	int itemp=memcmp(stemp.c_str(),MeterInfo.m_Address,6);
	if(itemp!=0)
		return false;
	
	MeterInfo.m_Enable = false;

	STATUS.RefreshMeterInfo();
	CONTEXT.NotifyDbcj(NOTIFY_DBCJ_ADD_METER);
	MpInfo.m_AutoGather = 0;
	MpInfo.m_Type = MP_TYPE_UNKNOW;
	MpInfo.m_SN = 0x00;
	return true;
}
//�û��Զ�������CCCC
bool C_CmdwE0002000::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{/// 2+L LΪ��ȷ����
	return false;
}
bool C_CmdwE0002000::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}


//��������������
//��Ч����ͨ������
bool C_CmdwE0000200::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(7,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000200::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	int Process = CONTEXT.GetDbcjPrecess_VCom((S_BYTE)((CmdObj.m_DI&0xFF)+1));
	if (Process < 0)
		return false;
	S_DbcjInfo *pDbcjInfo = STATUS.GetDbcjInfo((S_DbcjSN)Process);
	if (pDbcjInfo == NULL)
		return false;

	char bcdStr7B[7];
	copy(CmdObj.m_DiInfo.begin(), CmdObj.m_DiInfo.begin()+5, bcdStr7B+1);
	bcdStr7B[0] = 0x00;
	bcdStr7B[6] = 0x20;
	pDbcjInfo->m_GatherStdTime = C_TIME::BcdStr2Time(bcdStr7B);
	pDbcjInfo->m_GatherPeriodUnit = (S_TIME_UNIT)(TIME_UNIT_MINUTE + CmdObj.m_DiInfo[5]);
	pDbcjInfo->m_GatherPeriodVal = CmdObj.m_DiInfo[6];

	STATUS.SaveDbcjInfo();
	CONTEXT.NotifyDbcj(NOTIFY_DBCJ_ADD_TASK);
	return true;
}
bool C_CmdwE0000230::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo((S_WORD)distance(iterBegin, iterEnd),  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000230::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() < 11)
		return false;
	S_WORD NO = (S_WORD)(CmdObj.m_DI - 0xE0000230);
	if (NO >= CONS_TASK_COUNT_MAX)
		return false;
	S_GatherTaskInfos *pGatherTaskInfos = STATUS.GetGatherTaskInfos();
	if (pGatherTaskInfos == NULL)
		return false;

	S_LONG index = 0;
	S_GatherTaskInfo GatherTaskInfo;
	GatherTaskInfo.m_Enable=((S_BYTE)CmdObj.m_DiInfo[index++] == 0x00)?false:true;

	S_BYTE L = CmdObj.m_DiInfo[index++];
	S_BYTE H = CmdObj.m_DiInfo[index++];
	GatherTaskInfo.m_RecordMax = KGE_COMBWORD(kge_bcdc(H), kge_bcdc(L));

	GatherTaskInfo.m_MeterType=(S_BYTE)CmdObj.m_DiInfo[index++];
	GatherTaskInfo.m_VipFlag=(S_BYTE)CmdObj.m_DiInfo[index++];
	GatherTaskInfo.m_DaCnt=(S_WORD)CmdObj.m_DiInfo[index++];

	if((int)(CmdObj.m_DiInfo.size()-index) < GatherTaskInfo.m_DaCnt*2)
		return false;
	for(S_BYTE pn=0; pn < GatherTaskInfo.m_DaCnt; pn++)
	{
		GatherTaskInfo.m_DA[pn] = KGE_COMBWORD(CmdObj.m_DiInfo[index+1], CmdObj.m_DiInfo[index]);
		index += 2;
	}

	GatherTaskInfo.m_DtCnt=(S_WORD)CmdObj.m_DiInfo[index++];
	if((int)(CmdObj.m_DiInfo.size()-index) < GatherTaskInfo.m_DtCnt*4)
		return false;
	for(S_BYTE tn = 0; tn < GatherTaskInfo.m_DtCnt; tn++)
	{
		S_WORD m = KGE_COMBWORD(CmdObj.m_DiInfo[index+1], CmdObj.m_DiInfo[index+0]);
		S_WORD n = KGE_COMBWORD(CmdObj.m_DiInfo[index+3], CmdObj.m_DiInfo[index+2]);
		GatherTaskInfo.m_DT[tn] = KGE_COMBDWORD(n, m);
		index += 4;
	}

	pGatherTaskInfos->m_ItemInfo[NO] = GatherTaskInfo;

	STATUS.SaveGatherTasks();
	CONTEXT.NotifyDbcj(NOTIFY_DBCJ_ADD_TASK);
	return true;
}

//������� ��ͨ�����
bool C_CmdwE0000301::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	if (distance(iterBegin, iterEnd) < 20)
		return false;
	S_WORD len=20;

	S_WORD DAC=(S_WORD)(*(iterBegin+19));
	len=20+DAC*2;
	
	if (distance(iterBegin, iterEnd) < (len+1))
		return false;	
	S_WORD DIC=(S_WORD)(*(iterBegin+len));
	
	len=1+len+DIC*4;
	if (distance(iterBegin, iterEnd) < len)
		return false;
	CmdObj.m_DiInfo.append(iterBegin, iterBegin+len);
	advance(iterBegin, len);
	
	return true;
}

bool C_CmdwE0000301::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_NormalTaskInfos *pTaskInfos = STATUS.GetNormalTaskInfos();
	S_NormalTaskInfo &pInfo = pTaskInfos->m_ItemInfo[CmdObj.m_DI - NORMAL_TASK_ID_MIN];

	S_LONG index=0;
	pInfo.m_Enable = true;
	pInfo.m_Active=((S_BYTE)CmdObj.m_DiInfo[index++]==0X00)?false:true;
	
	S_CHAR bcdtime[5];
	for(int i = 4; i >= 0; i--)
		bcdtime[i]=(S_CHAR)CmdObj.m_DiInfo[index++];
	pInfo.m_ReportStdTime=C_TIME::BcdStr2Time5B_Big(bcdtime);
	pInfo.m_ReportPeriodUnit=(S_BYTE)CmdObj.m_DiInfo[index++];
	pInfo.m_ReportPeriodVal=(S_BYTE)CmdObj.m_DiInfo[index++];
	pInfo.m_DataFormat=(S_BYTE)CmdObj.m_DiInfo[index++];

	for(int i = 4; i >= 0; i--)
		bcdtime[i]=(S_CHAR)CmdObj.m_DiInfo[index++];
	pInfo.m_SampleStdTime=C_TIME::BcdStr2Time5B_Big(bcdtime);
	pInfo.m_SamplePeriodUnit=(S_BYTE)CmdObj.m_DiInfo[index++];
	pInfo.m_SamplePeriodVal=(S_BYTE)CmdObj.m_DiInfo[index++];
	pInfo.m_ExtractRate=(S_BYTE)CmdObj.m_DiInfo[index++];
	pInfo.m_MaxTimes = KGE_COMBWORD(CmdObj.m_DiInfo[index++], CmdObj.m_DiInfo[index++]);
	pInfo.m_DaCnt=(S_WORD)CmdObj.m_DiInfo[index++];
	for(S_BYTE pn=0; pn < pInfo.m_DaCnt;pn++)
	{
		pInfo.m_DA[pn] = KGE_COMBWORD(CmdObj.m_DiInfo[index+1], CmdObj.m_DiInfo[index]);
		index += 2;
	}
	pInfo.m_DtCnt=(S_WORD)CmdObj.m_DiInfo[index++];
	for(S_BYTE tn = 0; tn < pInfo.m_DtCnt; tn++)
	{
		S_WORD m = KGE_COMBWORD(CmdObj.m_DiInfo[index+1], CmdObj.m_DiInfo[index+0]);
		S_WORD n = KGE_COMBWORD(CmdObj.m_DiInfo[index+3], CmdObj.m_DiInfo[index+2]);
		pInfo.m_DT[tn] = KGE_COMBDWORD(n, m);
		index += 4;
	}	

	S_NormalTaskStatus &TaskStatus = pTaskInfos->m_ItemStatus[CmdObj.m_DI - NORMAL_TASK_ID_MIN];
	TaskStatus.m_timeLastReport = 0x00;

	STATUS.SaveNormalTask();
	return true;
}
//������� �м������
bool C_CmdwE0000401::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	if (distance(iterBegin, iterEnd) < 26)
		return false;
	S_WORD len=26;

	S_WORD DAC=(S_WORD)(*(iterBegin+25));
	len=26+DAC;
	
	if (distance(iterBegin, iterEnd) < len)
		return false;	
	
	CmdObj.m_DiInfo.append(iterBegin, iterBegin+len);
	advance(iterBegin, len);
	
	return true;
}
bool C_CmdwE0000401::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_ForwardTaskInfos *pTaskInfos = STATUS.GetForwardTaskInfos();
	S_ForwardTaskInfo  &pInfo = pTaskInfos->m_ItemInfo[CmdObj.m_DI - FORWARD_TASK_ID_MIN];

	S_LONG index=0;
	pInfo.m_Enable = true;
	pInfo.m_Active=((S_BYTE)CmdObj.m_DiInfo[index++]==0X00)?false:true;
	
	S_CHAR bcdtime[5];
	for(int i = 4; i >= 0; i--)
		bcdtime[i]=(S_CHAR)CmdObj.m_DiInfo[index++];
	pInfo.m_ReportStdTime=C_TIME::BcdStr2Time5B_Big(bcdtime);	
	pInfo.m_ReportPeriodUnit=(S_BYTE)CmdObj.m_DiInfo[index++];
	pInfo.m_ReportPeriodVal=(S_BYTE)CmdObj.m_DiInfo[index++];
	
	for(int i = 4; i >= 0; i--)
		bcdtime[i]=(S_CHAR)CmdObj.m_DiInfo[index++];
	pInfo.m_SampleStdTime=C_TIME::BcdStr2Time5B_Big(bcdtime);
	pInfo.m_SamplePeriodUnit=(S_BYTE)CmdObj.m_DiInfo[index++];
	pInfo.m_SamplePeriodVal=(S_BYTE)CmdObj.m_DiInfo[index++];
	
	pInfo.m_ExtractRate=(S_BYTE)CmdObj.m_DiInfo[index++];
	pInfo.m_ForwardType=(S_BYTE)CmdObj.m_DiInfo[index++];	
	pInfo.m_MaxTimes = KGE_COMBWORD(CmdObj.m_DiInfo[index++], CmdObj.m_DiInfo[index++]);
	
	pInfo.m_PortSet.m_PortNO=(S_DWORD)CmdObj.m_DiInfo[index++];
	pInfo.m_PortSet.m_Baud=(S_DWORD)CmdObj.m_DiInfo[index++];
	pInfo.m_PortSet.m_Parity=(S_DWORD)CmdObj.m_DiInfo[index++];
//	if(pInfo.m_PortSet.m_Parity==0)
//		pInfo.m_PortSet.m_Parity=COMM_PARITY_NO;
//	else if(pInfo.m_PortSet.m_Parity==2)
//		pInfo.m_PortSet.m_Parity=COMM_PARITY_ODD;
//	else
//		pInfo.m_PortSet.m_Parity=COMM_PARITY_EVEN;

	pInfo.m_PortSet.m_DataBit=(S_DWORD)CmdObj.m_DiInfo[index++];
	pInfo.m_PortSet.m_StopBit=(S_DWORD)CmdObj.m_DiInfo[index++];
//	if(pInfo.m_PortSet.m_Parity==1)
//		pInfo.m_PortSet.m_Parity=COMM_STOP_BIT3;
//	else if(pInfo.m_PortSet.m_Parity==2)
//		pInfo.m_PortSet.m_Parity=COMM_STOP_BIT2;
//	else
//		pInfo.m_PortSet.m_Parity=COMM_STOP_BIT1;
	
	pInfo.m_TimeoutS=(S_BYTE)CmdObj.m_DiInfo[index++];
	pInfo.m_ForwardCmdLen=(S_BYTE)CmdObj.m_DiInfo[index++];

	memcpy(pInfo.m_ForwardCmd,&CmdObj.m_DiInfo[index],pInfo.m_ForwardCmdLen);

	S_ForwardTaskStatus &TaskStatus = pTaskInfos->m_ItemStatus[CmdObj.m_DI - FORWARD_TASK_ID_MIN];
	TaskStatus.m_timeLastReport = 0x00;

	STATUS.SaveForwardTask();
	return true;
}
//���������� ģ������
bool C_CmdwE0000501::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(3,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000501::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//���������� ��������
bool C_CmdwE0000601::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(6,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000601::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//���������� ���������
bool C_CmdwE0000701::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(30,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000701::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//���������� �����
bool C_CmdwE0000801::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(15,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000801::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//
bool C_CmdwE0000A00::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(5,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000A00::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_BYTE VCom = (S_BYTE)(CmdObj.m_DI & 0xFF);
	if (VCom > 2)
		return false;

	S_CHANNEL *pChannel = NULL;
	S_BYTE fun = CmdObj.m_DiInfo[0];
	if (fun == 0x00 || fun == 0x02)
	{
		S_DnChannel *pDnChannel = STATUS.GetDnChannelInfo();
		pChannel = pDnChannel->FindChannel(VCom+1, true);
	}
	else
	{
		S_UpChannel *pUpChannel = STATUS.GetUpChannelInfo();
		pChannel = pUpChannel->FindChannel(VCom+1, true);
	}
	if (pChannel == NULL)
		return false;

	string strComSet(CmdObj.m_DiInfo.begin()+1, CmdObj.m_DiInfo.end());
	switch (VCom+1)
	{
	case 1:
		if (fun == 0x00) // 485I ֻ�ɱ�
		{
			SetChannelParam(pChannel, strComSet);
			STATUS.SaveDnChannl();
			return true;
		}
		break;
	case 2:
		if (fun == 0x00) // 485II �ɱ�
		{
			C_SYSCTRL::EnableProcess(MP_PROCESS_DBCJ2, true);
			SetChannelParam(pChannel, strComSet);
			STATUS.SaveDnChannl();
			STATUS.SaveDbcjInfo();
			CONTEXT.SetReconnectFlag(true);
			return true;
		}
		else if (fun == 0x01)// 485II ����
		{
			C_SYSCTRL::EnableProcess(MP_PROCESS_DBCJ2, false);
			SetChannelParam(pChannel, strComSet);
			STATUS.SaveUpChannel();
			STATUS.SaveDbcjInfo();
			CONTEXT.SetReconnectFlag(true);
			return true;
		}
		break;
	case 3:
		if (fun == 0x00) // 485II �ɱ�
		{
			C_SYSCTRL::EnableProcess(MP_PROCESS_DBCJ3, true);
			C_SYSCTRL::EnableProcess(MP_PROCESS_HTTX4, false);
			SetChannelParam(pChannel, strComSet);
			STATUS.SaveDnChannl();
			STATUS.SaveDbcjInfo();
			STATUS.SaveHttxInfo();
			return true;
		}
		else if (fun == 0x03) // 485II �ϴ�
		{
			C_SYSCTRL::EnableProcess(MP_PROCESS_DBCJ3, false);
			C_SYSCTRL::EnableProcess(MP_PROCESS_HTTX4, true);
			SetChannelParam(pChannel, strComSet);
			STATUS.SaveUpChannel();
			STATUS.SaveDbcjInfo();
			STATUS.SaveHttxInfo();
			return true;
		}
	default:
		break;
	}
	return false;
}

int C_CmdwE0000A00::SetChannelParam(S_CHANNEL *pChannel, const string &strComSet)
{
	//������
	pChannel->m_PortSet.m_ComSet.m_Baud = strComSet[0] * 300;
	//У�鷽ʽ
	if (strComSet[1] == 0x01) pChannel->m_PortSet.m_ComSet.m_Parity = 2;
	else if (strComSet[1] == 0x02) pChannel->m_PortSet.m_ComSet.m_Parity = 1;
	else pChannel->m_PortSet.m_ComSet.m_Parity = 0;
	//����λ
	pChannel->m_PortSet.m_ComSet.m_DataBit = strComSet[2];
	//ֹͣλ
	if (strComSet[3] == 0x01) pChannel->m_PortSet.m_ComSet.m_StopBit = 3;
	else if (strComSet[3] == 0x02) pChannel->m_PortSet.m_ComSet.m_StopBit = 2;
	else pChannel->m_PortSet.m_ComSet.m_StopBit = 1;

	return 0;
}


