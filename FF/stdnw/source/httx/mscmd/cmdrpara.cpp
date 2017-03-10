/** @file
*  @brief ��̨ͨѶ���������������
*  @author 
*  @date 2011/12/22
*  @version 0.1
*/
#include "cmdrpara.h"
#include "timeop.h"
#include "syscfg.h"
#include "version.h"
#include "sysbase.h"
#include "status.h"
#include "context.h"
#include "nwmsrule.h"
#include "sysctrl.h"

#define MSCMD_RESPONSE_PPPINVALID_CHAR 0xAA
//��վͨ�Ų�����
bool C_CmdrE0000100::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strMS1;

	AppendDADT(strMS1, CmdObj);
	if(ReadParam(strMS1,STATUS.GetHttxChannel(MP_HTTX_1, HTTX_CHANNEL_1))==false)
		return false;

	strResponse.push_back(strMS1);
	return true;
}
bool C_CmdrE0000100::ReadParam(string &strParam,S_CHANNEL *pMS)
{
	string strTemp;
	string strIP((char *)&pMS->m_PortSet.m_NetSet.m_IP[0], 4);
	
	strTemp.append((char *)&pMS->m_PortSet.m_NetSet.m_PortNO,2);
	strTemp.append(strIP.rbegin(), strIP.rend());
	strTemp.append((size_t)(8-6),(char)MSCMD_RESPONSE_PPPINVALID_CHAR);

	if(pMS->m_PortSet.m_NetSet.m_NeedPPP == NET_CONNECT_PPP_YES)
		strTemp.push_back(0x02);
	else if(pMS->m_PortSet.m_NetSet.m_NeedPPP == NET_CONNECT_PPP_NO)
		strTemp.push_back(0x04);
	else
		return false;

	strParam.append(strTemp);
	return true;
}
//��վͨ�Ų�������1
bool C_CmdrE0000101::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strMS2;

	AppendDADT(strMS2, CmdObj);
	if(ReadParam(strMS2,STATUS.GetHttxChannel(MP_HTTX_1, HTTX_CHANNEL_2))==false)
		return false;

	strResponse.push_back(strMS2);
	return true;
}
//��վͨ�Ų�������2
bool C_CmdrE0000102::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strMS3;

	AppendDADT(strMS3, CmdObj);
	if(ReadParam(strMS3,STATUS.GetHttxChannel(MP_HTTX_1, HTTX_CHANNEL_3))==false)
		return false;

	strResponse.push_back(strMS3);
	return true;
}
//���ص�ַ���ߴ����������ַ�Ͷ˿ڣ��μ���վͨ�ŵ�ַ����д(����ѡ���Ǵ��������)
bool C_CmdrE0000103::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strIpConfig;
	S_IpConfig *pIpConfig = STATUS.GetIpConfig();
	string strIP((char *)&pIpConfig->m_ProxyIP[0], 4);

	AppendDADT(strIpConfig, CmdObj);
	strIpConfig.append((char *)&pIpConfig->m_ProxyPort,2);
	strIpConfig.append(strIP.rbegin(), strIP.rend());
	strIpConfig.append((size_t)(8-6),(char)MSCMD_RESPONSE_PPPINVALID_CHAR);

	strResponse.push_back(strIpConfig);
	return true;
}
//APN
bool C_CmdrE0000104::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPppInfo;
	S_PppInfo *pPppInfo = STATUS.GetPppInfo();

	AppendDADT(strPppInfo, CmdObj);
	strPppInfo.append((char *)&pPppInfo->m_DialInfo.m_Apn[0], 16);

	strResponse.push_back(strPppInfo);
	return true;
}
//APN�û���
bool C_CmdrE0000105::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPppInfo;
	S_PppInfo *pPppInfo = STATUS.GetPppInfo();

	AppendDADT(strPppInfo, CmdObj);
	strPppInfo.append((char *)&pPppInfo->m_DialInfo.m_User[0], 32);

	strResponse.push_back(strPppInfo);
	return true;
}
//APN����
bool C_CmdrE0000106::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_PppInfo *pPppInfo = STATUS.GetPppInfo();
	string strPppInfo;

	AppendDADT(strPppInfo, CmdObj);
	strPppInfo.append((char *)&pPppInfo->m_DialInfo.m_Pwd[0], 32);

	strResponse.push_back(strPppInfo);
	return true;
}
//��������
bool C_CmdrE0000107::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strHeartBeatPeriodM;
	S_HttxInfo *pHttxInfo = STATUS.GetHttxInfo(MP_HTTX_1);//ȡ��̨ͨ��1

	AppendDADT(strHeartBeatPeriodM, CmdObj);
	strHeartBeatPeriodM.push_back(kge_cbcd(pHttxInfo->m_HeartBeatPeriodM));

	strResponse.push_back(strHeartBeatPeriodM);
	return true;
}
//�����ز������ȱʡ30����
bool C_CmdrE0000108::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPppInfo;
	S_PppInfo *pPppInfo = STATUS.GetPppInfo();

	AppendDADT(strPppInfo, CmdObj);
	strPppInfo.push_back(kge_cbcd((char)pPppInfo->m_RedialInterM));

	strResponse.push_back(strPppInfo);
	return true;
}
//���ߺ��ز�������ȱʡ3��,�ز�ָ�������󣬵��첻���ز�
bool C_CmdrE0000109::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPpp;
	S_PppInfo *pPppInfo = STATUS.GetPppInfo();

	AppendDADT(strPpp, CmdObj);
	strPpp.push_back(kge_cbcd((char)pPppInfo->m_RedialTimesPerDay));

	strResponse.push_back(strPpp);
	return true;
}
//���ӷ�ʽ��0��TCP��ʽ��1��UDP��ʽ
bool C_CmdrE000010A::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strMS;
	S_CHANNEL *pMS = STATUS.GetHttxChannel(MP_HTTX_1, HTTX_CHANNEL_1);//��ȡͨ��1

	AppendDADT(strMS, CmdObj);
	if((pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP_SERVER)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP_CLIENT)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP))
		strMS.push_back(0x00);
	else
		strMS.push_back(0x01);

	strResponse.push_back(strMS);
	return true;
}
//ѹ������:0����ѹ��  1��ѹ����BIN����
bool C_CmdrE000010B::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strData;
	S_HttxInfo *pHttxInfo = STATUS.GetHttxInfo(MP_HTTX_1);//ȡ��̨ͨ��1

	AppendDADT(strData, CmdObj);
	strData.push_back(pHttxInfo->m_CompressEnable);

	strResponse.push_back(strData);
	return true;
}
//�������ݼ���
bool C_CmdrE000010F::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strAllParam;

	AppendDADT(strAllParam, CmdObj);
	GetAllParam(strAllParam);

	strResponse.push_back(strAllParam);
	return true;
}
bool C_CmdrE000010F::GetAllParam(string &strAllParam)
{
	S_IpConfig *pIpConfig = STATUS.GetIpConfig();
	S_HttxInfo *pHttxInfo = STATUS.GetHttxInfo(MP_HTTX_1);//ȡ��̨ͨ��1
	S_PppInfo *pPppInfo = STATUS.GetPppInfo();

	//��վͨ�Ų���
	for (int i = HTTX_CHANNEL_1; i < HTTX_CHANNL_MAX; i++)
	{
		if(ReadParam(strAllParam, STATUS.GetHttxChannel(MP_HTTX_1, (S_ChannelSN)i)) == false)
			return false;
	}
	//���ص�ַ���ߴ����������ַ�Ͷ˿ڣ��μ���վͨ�ŵ�ַ����д(����ѡ���Ǵ��������)
	string strIP((char *)&pIpConfig->m_ProxyIP[0], 4);
	strAllParam.append((char *)&pIpConfig->m_ProxyPort,2);
	strAllParam.append(strIP.rbegin(), strIP.rend());
	strAllParam.append((size_t)(8-6),(char)MSCMD_RESPONSE_PPPINVALID_CHAR);
	//APN
	strAllParam.append((char *)&pPppInfo->m_DialInfo.m_Apn[0], 16);
	//APN�û���
	strAllParam.append((char *)&pPppInfo->m_DialInfo.m_User[0], 32);
	//APN����
	strAllParam.append((char *)&pPppInfo->m_DialInfo.m_Pwd[0], 32);
	//��������
	strAllParam.push_back(kge_cbcd(pHttxInfo->m_HeartBeatPeriodM));
	//�����ز������ȱʡ30����
	strAllParam.push_back(kge_cbcd((S_BYTE)pPppInfo->m_RedialInterM));
	//���ߺ��ز�������ȱʡ3��,�ز�ָ�������󣬵��첻���ز�
	strAllParam.push_back(kge_cbcd((S_BYTE)pPppInfo->m_RedialTimesPerDay));
	//���ӷ�ʽ��0��TCP��ʽ��1��UDP��ʽ
	S_CHANNEL *pMS = STATUS.GetHttxChannel(MP_HTTX_1, HTTX_CHANNEL_1);//��ȡͨ��1
	if((pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP_SERVER)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP_CLIENT)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP))
		strAllParam.push_back(0x00);
	else
		strAllParam.push_back(0x01);
	return true;
}
//�ն˵��������룬BCD��
bool C_CmdrE0000120::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strRtua;
	string strAreaCode;
	S_SYSCFG *pSystemConfig = STATUS.GetSystemConfig();
	C_RTUA rtua(pSystemConfig->m_RTUA);

	rtua.PrintString(strRtua);
	AppendDADT(strAreaCode, CmdObj);
	strAreaCode.append(strRtua.begin(), strRtua.begin()+3);

	strResponse.push_back(strAreaCode);
	return true;
}
//�ն˵�ַ��BIN����
bool C_CmdrE0000121::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strRtua;
	string strDevAddr;
	S_SYSCFG *pSystemConfig = STATUS.GetSystemConfig();
	C_RTUA rtua(pSystemConfig->m_RTUA);

	rtua.PrintString(strRtua);
	AppendDADT(strDevAddr, CmdObj);
	strDevAddr.append(strRtua.begin()+3, strRtua.begin()+3+3);

	strResponse.push_back(strDevAddr);
	return true;
}
//�ն�ͨ�Ź���ģʽ
bool C_CmdrE0000122::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strMS;
	S_CHANNEL *pMS = STATUS.GetHttxChannel(MP_HTTX_1, HTTX_CHANNEL_1);//��ȡͨ��1

	AppendDADT(strMS, CmdObj);
	if((pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_UDP))
		strMS.push_back(0x00);
	else if((pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP_SERVER)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_UDP_SERVER))
		strMS.push_back(0x02);
	else
		strMS.push_back(0x01);

	strResponse.push_back(strMS);
	return true;
}
//���ģʽ�ͷ�����ģʽ�£�TCP�����˿ںţ�BIN����
bool C_CmdrE0000123::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strMS;
	S_CHANNEL *pMS = STATUS.GetHttxChannel(MP_HTTX_1, HTTX_CHANNEL_1);//��ȡͨ��1

	AppendDADT(strMS, CmdObj);
	if((pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_UDP)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP_SERVER))
		strMS.append((char *)&pMS->m_PortSet.m_NetSet.m_PortNO,2);
	else
		return false;

	strResponse.push_back(strMS);
	return true;
}
//�ն˵�ǰͨ��IP��ַ
bool C_CmdrE0000124::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strIpConfig;
	S_IpConfig *pIpConfig = STATUS.GetIpConfig();
	string strIpData((char*)&pIpConfig->m_TermIP[0],4);

	AppendDADT(strIpConfig, CmdObj);
	strIpConfig.append(strIpData.rbegin(), strIpData.rend());

	strResponse.push_back(strIpConfig);
	return true;
}
//�ն˵�ǰͨ�����������ַ
bool C_CmdrE0000125::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strIpConfig;
	S_IpConfig *pIpConfig = STATUS.GetIpConfig();
	string strNetMask((char*)pIpConfig->m_NetMask, 4);

	AppendDADT(strIpConfig, CmdObj);
	strIpConfig.append(strNetMask.rbegin(), strNetMask.rend());

	strResponse.push_back(strIpConfig);
	return true;
}
//�ն˵�ǰͨ�����ص�ַ
bool C_CmdrE0000126::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strIpConfig;
	S_IpConfig *pIpConfig = STATUS.GetIpConfig();
	string strGW((char*)pIpConfig->m_GateWay, 4);

	AppendDADT(strIpConfig, CmdObj);
	strIpConfig.append(strGW.rbegin(), strGW.rend());

	strResponse.push_back(strIpConfig);
	return true;
}
//��������IP��ַ��ȡ��ʽ 0���ֶ�����IP��ַ 1���Զ�����IP��ַ
bool C_CmdrE0000127::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strIpConfig;
	S_IpConfig *pIpConfig = STATUS.GetIpConfig();

	AppendDADT(strIpConfig, CmdObj);
	strIpConfig.push_back(pIpConfig->m_IpMethod);

	strResponse.push_back(strIpConfig);
	return true;
}
//�������ݺϼ�
bool C_CmdrE000012F::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strAllParam;

	AppendDADT(strAllParam, CmdObj);
	if(GetAllParam(strAllParam)==false)
		return false;

	strResponse.push_back(strAllParam);
	return false;
}
bool C_CmdrE000012F::GetAllParam(string &strAllParam)
{
	S_SYSCFG *pSystemConfig = STATUS.GetSystemConfig();
	S_CHANNEL *pMS = STATUS.GetHttxChannel(MP_HTTX_1, HTTX_CHANNEL_1);//��ȡͨ��1
	S_IpConfig *pIpConfig = STATUS.GetIpConfig();

	//�ն˵��������룬BCD��+�ն˵�ַ��BIN����
	string strRtua;
	C_RTUA rtua(pSystemConfig->m_RTUA);
	rtua.PrintString(strRtua);
	strAllParam.append(strRtua.begin(), strRtua.begin()+6);
	//�ն�ͨ�Ź���ģʽ
	if((pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_UDP))
		strAllParam.push_back(0x00);
	else if((pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP_SERVER)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_UDP_SERVER))
		strAllParam.push_back(0x02);
	else
		strAllParam.append(0x01, (size_t)1);
	//���ģʽ�ͷ�����ģʽ�£�TCP�����˿ںţ�BIN����
	if((pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_UDP)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP_SERVER))
		strAllParam.append((char *)&pMS->m_PortSet.m_NetSet.m_PortNO,2);
	else
		strAllParam.append((size_t)2,(char)0x00);
	//�ն˵�ǰͨ��IP��ַ
	string strIpData((char*)&pIpConfig->m_TermIP[0],4);
	strAllParam.append(strIpData.rbegin(), strIpData.rend());
	//�ն˵�ǰͨ�����������ַ
	string strNetMask((char*)&pIpConfig->m_NetMask[0],4);
	strAllParam.append(strNetMask.rbegin(), strNetMask.rend());
	//�ն˵�ǰͨ�����ص�ַ
	string strGW((char*)&pIpConfig->m_GateWay[0],4);
	strAllParam.append(strGW.rbegin(), strGW.rend());
	//��������IP��ȡ��ʽ
	strAllParam.push_back(pIpConfig->m_IpMethod);
	return true;
}

//�ն�ʱ��
bool C_CmdrE0000130::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	char bcdStr7B[7];
	C_TIME::CurTime2BCDStr(bcdStr7B);

	string strBcdTime;
	AppendDADT(strBcdTime, CmdObj);
	strBcdTime.append((char *)&bcdStr7B, 6);

	strResponse.push_back(strBcdTime);
	return true;
}
//ʵʱУʱ �޶�
//�ն˸�λ �޶�
//�澯�����ϱ�������
bool C_CmdrE0000150::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strterm;
	string strAlarmInfo;

	S_AlarmInfo *pAlarmInfo = STATUS.GetAlarmInfo();
	C_BITSET<32> ReportAlarmEven;

	AppendDADT(strAlarmInfo, CmdObj);
	pAlarmInfo->m_ReportAlarmEven.PrintString(strterm);
	ReportAlarmEven.ParseString(strterm);
	strAlarmInfo.append((char *)ReportAlarmEven.GetData(),ReportAlarmEven.GetByteCnt());

	strResponse.push_back(strAlarmInfo);
	return true;
}
//�澯�ж������� 
bool C_CmdrE0000151::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strterm;
	string strAlarmInfo;

	S_AlarmInfo *pAlarmInfo = STATUS.GetAlarmInfo();
	C_BITSET<32> AlarmEven;

	AppendDADT(strAlarmInfo, CmdObj);
	pAlarmInfo->m_AlarmEven.PrintString(strterm);
	AlarmEven.ParseString(strterm);
	strAlarmInfo.append((char *)AlarmEven.GetData(),AlarmEven.GetByteCnt());

	strResponse.push_back(strAlarmInfo);
	return true;
}
//�¼���¼������
bool C_CmdrE0000152::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strterm;
	string strAlarmInfo;

	S_AlarmInfo *pAlarmInfo = STATUS.GetAlarmInfo();
	C_BITSET<32> OnlyEvent;

	AppendDADT(strAlarmInfo, CmdObj);
	pAlarmInfo->m_OnlyEvent.PrintString(strterm);
	OnlyEvent.ParseString(strterm);
	strAlarmInfo.append((char *)OnlyEvent.GetData(),OnlyEvent.GetByteCnt());

	strResponse.push_back(strAlarmInfo);
	return true;
}
//�¶������ DD������ʱ����գ���Χ1~30��ȱʡΪÿ��1�� HH������ʱ���Сʱ����Χ0~23��ȱʡΪ0��
bool C_CmdrE0000160::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strDbcjInfo;
	S_DbcjInfo *pDbcjInfo = STATUS.GetDbcjInfo(MP_DBCJ_1);

	AppendDADT(strDbcjInfo, CmdObj);
	strDbcjInfo.push_back((char)pDbcjInfo->m_MonBeginDate);
	strDbcjInfo.push_back((char)pDbcjInfo->m_MonBeginTime[0]);

	strResponse.push_back(strDbcjInfo);
	return true;
}
//�ն������ HH���յ����������룩����ʱ�̣���Χ0~23��ȱʡΪ0��
bool C_CmdrE0000161::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strDbcjInfo;
	S_DbcjInfo *pDbcjInfo = STATUS.GetDbcjInfo(MP_DBCJ_1);

	AppendDADT(strDbcjInfo, CmdObj);
	strDbcjInfo.push_back((char)pDbcjInfo->m_DayBeginTime[0]);

	strResponse.push_back(strDbcjInfo);
	return true;
}
//�µ�ѹ�ϸ������ݶ��� DD������ʱ����գ���Χ1~30��ȱʡΪÿ��21��HH������ʱ���Сʱ����Χ0~23��ȱʡΪ0��
bool C_CmdrE0000162::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strDbcjInfo;
	S_DbcjInfo *pDbcjInfo = STATUS.GetDbcjInfo(MP_DBCJ_1);

	AppendDADT(strDbcjInfo, CmdObj);
	strDbcjInfo.push_back((char)pDbcjInfo->m_MonVolFreezeDate);
	strDbcjInfo.push_back((char)pDbcjInfo->m_MonVolFreezeTime[0]);

	strResponse.push_back(strDbcjInfo);
	return true;
}
//�յ�ѹ�ϸ������ݶ��� HH���յ����������룩����ʱ�̣���Χ0~23��ȱʡΪ0��
bool C_CmdrE0000163::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strDbcjInfo;
	S_DbcjInfo *pDbcjInfo = STATUS.GetDbcjInfo(MP_DBCJ_1);

	AppendDADT(strDbcjInfo, CmdObj);
	strDbcjInfo.push_back((char)pDbcjInfo->m_DayVolFreezeTime[0]);

	strResponse.push_back(strDbcjInfo);
	return true;
}
//��֤������Ϣ��BIN���룬����
bool C_CmdrE0000171::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//��֤��Ӧ��Ϣ��BIN���룬����
bool C_CmdrE0000172::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//�������Ϣ��BIN���룬����
bool C_CmdrE0000173::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//�������Ӧ��Ϣ��BIN���룬����
bool C_CmdrE0000174::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//�жϸ��ɹ��ء����������ɡ�����ѹA������ͬ�����õ硢���ʳ���ֵ��������ƽ��ȡ���ѹ���䡢�������䡢�������ƫ���޹�Ƿ�������޹���������ʱ��ȱʡ15����
bool C_CmdrE0000180::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPowerLimit;
	S_PowerLimit *pPowerLimit = STATUS.GetPowerLimitInfo();

	AppendDADT(strPowerLimit, CmdObj);
	strPowerLimit.push_back(kge_cbcd((S_BYTE)pPowerLimit->m_OverLoadM));

	strResponse.push_back(strPowerLimit);
	return true;
}
//�жϸ��ɹ��ء����������ɡ�����ѹ����ͬ�����õ硢���ʳ���ֵ��������ƽ��ȡ���ѹ���䡢�������䡢�������ƫ���޹�Ƿ�������޹��������ָ�ʱ��ȱʡ15����
bool C_CmdrE0000181::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPowerLimit;
	S_PowerLimit *pPowerLimit = STATUS.GetPowerLimitInfo();

	AppendDADT(strPowerLimit, CmdObj);
	strPowerLimit.push_back(kge_cbcd((S_BYTE)pPowerLimit->m_OverLoadEndM));

	strResponse.push_back(strPowerLimit);
	return true;
}
//��ֵ�������ж϶��ࡢʧѹ��ʧ��ʱ�䣬ȱʡֵ1����
bool C_CmdrE0000182::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPowerLimit;
	S_PowerLimit *pPowerLimit = STATUS.GetPowerLimitInfo();

	AppendDADT(strPowerLimit, CmdObj);
	strPowerLimit.push_back(kge_cbcd((S_BYTE)pPowerLimit->m_MissingPhaseM));

	strResponse.push_back(strPowerLimit);
	return true;
}
//��ֵ�������ж϶��ࡢʧѹ��ʧ���ָ�ʱ�䣬ȱʡֵ1����
bool C_CmdrE0000183::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPowerLimit;
	S_PowerLimit *pPowerLimit = STATUS.GetPowerLimitInfo();

	AppendDADT(strPowerLimit, CmdObj);
	strPowerLimit.push_back(kge_cbcd((S_BYTE)pPowerLimit->m_MissingPhaseEndM));

	strResponse.push_back(strPowerLimit);
	return true;
}
//��ֵ�������жϵ�ѹ������ʱ�䣬ȱʡֵ15����
bool C_CmdrE0000184::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPowerLimit;
	S_PowerLimit *pPowerLimit = STATUS.GetPowerLimitInfo();

	AppendDADT(strPowerLimit, CmdObj);
	strPowerLimit.push_back(kge_cbcd((S_BYTE)pPowerLimit->m_ReversedPhaseM));

	strResponse.push_back(strPowerLimit);
	return true;
}
//��ֵ�������жϵ�ѹ������ָ�ʱ�䣬ȱʡֵ15����
bool C_CmdrE0000185::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPowerLimit;
	S_PowerLimit *pPowerLimit = STATUS.GetPowerLimitInfo();

	AppendDADT(strPowerLimit, CmdObj);
	strPowerLimit.push_back(kge_cbcd((S_BYTE)pPowerLimit->m_ReversedPhaseEndM));

	strResponse.push_back(strPowerLimit);
	return true;
}
//��ֵ�������жϵ���������ʱ�䣬ȱʡֵ15����
bool C_CmdrE0000186::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPowerLimit;
	S_PowerLimit *pPowerLimit = STATUS.GetPowerLimitInfo();

	AppendDADT(strPowerLimit, CmdObj);
	strPowerLimit.push_back(kge_cbcd((S_BYTE)pPowerLimit->m_ReversedPolarityM));

	strResponse.push_back(strPowerLimit);
	return true;
}
//��ֵ�������жϵ��������Իָ�ʱ�䣬ȱʡֵ15����
bool C_CmdrE0000187::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPowerLimit;
	S_PowerLimit *pPowerLimit = STATUS.GetPowerLimitInfo();

	AppendDADT(strPowerLimit, CmdObj);
	strPowerLimit.push_back(kge_cbcd((S_BYTE)pPowerLimit->m_ReversedPolarityEndM));

	strResponse.push_back(strPowerLimit);
	return true;
}
//��ֵ���������ܱ�ʱ���쳣��ֵ��ȱʡֵ10����
bool C_CmdrE0000188::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPowerLimit;
	S_PowerLimit *pPowerLimit = STATUS.GetPowerLimitInfo();

	AppendDADT(strPowerLimit, CmdObj);
	strPowerLimit.push_back(kge_cbcd((S_BYTE)pPowerLimit->m_MeterTimeDiffM));

	strResponse.push_back(strPowerLimit);
	return true;
}
//��ֵ���������ܱ������ֵ��ȱʡΪ10��
bool C_CmdrE0000189::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPowerLimit;
	S_PowerLimit *pPowerLimit = STATUS.GetPowerLimitInfo();

	AppendDADT(strPowerLimit, CmdObj);
	strPowerLimit.push_back(kge_cbcd((S_BYTE)pPowerLimit->m_MeterFly));

	strResponse.push_back(strPowerLimit);
	return true;
}
//��ֵ���������ܱ�ͣ����ֵ������ʾ��ֹͣ����ʱ�����й����ʼ���Ӧ�ߵ���ֵ��Խ��ֵ����ܱ�ͣ�ߣ�ȱʡΪ0.1kWh
bool C_CmdrE000018A::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPowerLimit;
	S_PowerLimit *pPowerLimit = STATUS.GetPowerLimitInfo();

	AppendDADT(strPowerLimit, CmdObj);
	strPowerLimit.append((char *)&pPowerLimit->m_MeterStop,2);

	strResponse.push_back(strPowerLimit);
	return true;
}
//��ֵ������ʣ�������ֵ��ȱʡΪ20.00 Ԫ
bool C_CmdrE000018B::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPowerLimit;
	S_PowerLimit *pPowerLimit = STATUS.GetPowerLimitInfo();

	AppendDADT(strPowerLimit, CmdObj);
	strPowerLimit.append((char *)&pPowerLimit->m_MeterLastFee,3);

	strResponse.push_back(strPowerLimit);
	return true;
}
//��ֵ��������ͨ������Խ��ֵ
bool C_CmdrE000018C::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_HttxInfo *pHttxInfo=STATUS.GetHttxInfo(MP_HTTX_1);
	string strBcdData = C_BusiAlgo::ChangeDword2Bcd(pHttxInfo->m_MonTrafficLimit/1024);

	string strPowerLimit;
	AppendDADT(strPowerLimit, CmdObj);
	strPowerLimit.append(strBcdData.data(), 3);

	strResponse.push_back(strPowerLimit);
	return true;
}
//�����������
bool C_CmdrE000018F::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strAllParam;

	AppendDADT(strAllParam, CmdObj);
	GetAllParam(strAllParam);

	strResponse.push_back(strAllParam);
	return true;
}
bool C_CmdrE000018F::GetAllParam(string &strAllParam)
{
	S_PowerLimit *pPowerLimit = STATUS.GetPowerLimitInfo();
	S_HttxInfo *pHttxInfo=STATUS.GetHttxInfo(MP_HTTX_1);

	strAllParam.push_back(kge_cbcd((S_BYTE)pPowerLimit->m_OverLoadM));
	strAllParam.push_back(kge_cbcd((S_BYTE)pPowerLimit->m_OverLoadEndM));
	strAllParam.push_back(kge_cbcd((S_BYTE)pPowerLimit->m_MissingPhaseM));
	strAllParam.push_back(kge_cbcd((S_BYTE)pPowerLimit->m_MissingPhaseEndM));
	strAllParam.push_back(kge_cbcd((S_BYTE)pPowerLimit->m_ReversedPhaseM));
	strAllParam.push_back(kge_cbcd((S_BYTE)pPowerLimit->m_ReversedPhaseEndM));
	strAllParam.push_back(kge_cbcd((S_BYTE)pPowerLimit->m_ReversedPolarityM));
	strAllParam.push_back(kge_cbcd((S_BYTE)pPowerLimit->m_ReversedPolarityEndM));
	strAllParam.push_back(kge_cbcd((S_BYTE)pPowerLimit->m_MeterTimeDiffM));
	strAllParam.push_back(kge_cbcd((S_BYTE)pPowerLimit->m_MeterFly));
	strAllParam.append((char *)&pPowerLimit->m_MeterStop,2);
	strAllParam.append((char *)&pPowerLimit->m_MeterLastFee,3);

	string strBcdData = C_BusiAlgo::ChangeDword2Bcd(pHttxInfo->m_MonTrafficLimit/1024);
	strAllParam.append(strBcdData.data(), 3);

	return true;
}
//��Ч�ĳ���������ñ����� (ֻ��)
bool C_CmdrE0000221::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strData;
	S_BYTE Cnt = 0x20;

	AppendDADT(strData, CmdObj);
	strData.push_back((char)Cnt);

	strResponse.push_back(strData);
	return true;
}
//��������Զ�Ӧ�ĳ���������ñ�
bool C_CmdrE0000222::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//������ͨ�������� (ֻ��)
bool C_CmdrE0000300::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strData;
	S_BYTE Cnt = 0xFE;

	AppendDADT(strData, CmdObj);
	strData.push_back((char)Cnt);

	strResponse.push_back(strData);
	return true;
}
//�����м���������
bool C_CmdrE0000400::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strData;
	S_BYTE Cnt = 0xFE;

	AppendDADT(strData, CmdObj);
	strData.push_back((char)Cnt);

	strResponse.push_back(strData);
	return true;
}
//ģ��������
bool C_CmdrE0000500::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strANALOGS;
	S_ANALOGS *pANALOGS = STATUS.GetAnalogInfos();

	AppendDADT(strANALOGS, CmdObj);
	strANALOGS.push_back((char)pANALOGS->m_AnalogCnt);

	strResponse.push_back(strANALOGS);
	return true;
}
//ģ����״̬���
bool C_CmdrE00005FE::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_BITSET<32> bANALOGS;
	string strANALOGS;
	S_ANALOGS *pANALOGS = STATUS.GetAnalogInfos();

	AppendDADT(strANALOGS, CmdObj);
	for(int i=0;i<256;i++)
	{
		(pANALOGS->m_Item[i].m_Enable?bANALOGS.SetBit(i):bANALOGS.ClearBit(i));
	}
	for(int i=0;i<32;i++)
	{
		strANALOGS.push_back((char)*(bANALOGS.GetData()+i));
	}
	strResponse.push_back(strANALOGS);
	return true;
}
//�����������
bool C_CmdrE00005FF::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//����������
bool C_CmdrE0000600::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPULSES;
	S_PULSES *pPULSES = STATUS.GetPulseInfos();

	AppendDADT(strPULSES, CmdObj);
	strPULSES.push_back((char)pPULSES->m_PulseCnt);

	strResponse.push_back(strPULSES);
	return true;
}
//������״̬���
bool C_CmdrE00006FE::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_BITSET<32> bPULSES;
	string strPULSES;
	S_PULSES *pPULSES = STATUS.GetPulseInfos();

	AppendDADT(strPULSES, CmdObj);
	for(int i=0;i<256;i++)
	{
		(pPULSES->m_Item[i].m_Enable?bPULSES.SetBit(i):bPULSES.ClearBit(i));
	}
	for(int i=0;i<32;i++)
	{
		strPULSES.push_back((char)*(bPULSES.GetData()+i));
	}
	strResponse.push_back(strPULSES);
	return true;
}
//�������ݼ���
bool C_CmdrE00006FF::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//����������
bool C_CmdrE0000700::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strSUMS;
	S_SUMS *pSUMS = STATUS.GetSumInfos();

	AppendDADT(strSUMS, CmdObj);
	strSUMS.push_back((char)pSUMS->m_SumCnt);

	strResponse.push_back(strSUMS);
	return true;
}
//������״̬���
bool C_CmdrE00007FE::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_BITSET<32> bSUMS;
	string strSUMS;
	S_SUMS *pSUMS = STATUS.GetSumInfos();

	AppendDADT(strSUMS, CmdObj);
	for(int i=0;i<256;i++)
	{
		(pSUMS->m_Item[i].m_Enable?bSUMS.SetBit(i):bSUMS.ClearBit(i));
	}
	for(int i=0;i<32;i++)
	{
		strSUMS.push_back((char)*(bSUMS.GetData()+i));
	}
	strResponse.push_back(strSUMS);
	return true;
}
//�������ݼ���
bool C_CmdrE00007FF::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//���������
bool C_CmdrE0000800::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strDiffRuleS;
	S_DiffRuleS *pDiffRuleS = STATUS.GetDiffRuleInfos();

	AppendDADT(strDiffRuleS, CmdObj);
	strDiffRuleS.push_back((char)pDiffRuleS->m_DiffRuleCnt);

	strResponse.push_back(strDiffRuleS);
	return true;
}
//�����״̬���
bool C_CmdrE00008FE::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_BITSET<32> bDiffRuleS;
	string strDiffRuleS;
	S_DiffRuleS *pDiffRuleS = STATUS.GetDiffRuleInfos();

	AppendDADT(strDiffRuleS, CmdObj);
	for(int i=0;i<256;i++)
	{
		(pDiffRuleS->m_Item[i].m_Enable?bDiffRuleS.SetBit(i):bDiffRuleS.ClearBit(i));
	}
	for(int i=0;i<32;i++)
	{
		strDiffRuleS.push_back((char)*(bDiffRuleS.GetData()+i));
	}
	strResponse.push_back(strDiffRuleS);
	return true;
}
//�������ݼ���
bool C_CmdrE00008FF::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//�ն��޹�����ͳ�Ʊ�־
bool C_CmdrE0000900::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//�������ն˵�ַ1~4
bool C_CmdrE0000A20::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strForwardInfo;
	S_ForwardInfo *pForwardInfo = STATUS.GetForwardInfo();

	AppendDADT(strForwardInfo, CmdObj);
	for(int i = 0; i < 4; i++)
	{
		strForwardInfo.append((char *)&pForwardInfo->m_SlaveAddr[i], 4);
	}

	strResponse.push_back(strForwardInfo);
	return true;
}
//���ն˵�ַ
bool C_CmdrE0000A21::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strForwardInfo;
	S_ForwardInfo *pForwardInfo = STATUS.GetForwardInfo();

	AppendDADT(strForwardInfo, CmdObj);
	strForwardInfo.append((char *)&pForwardInfo->m_MasterAddr,4);

	strResponse.push_back(strForwardInfo);
	return true;
}
//�����ն˵�ַ
bool C_CmdrE0000A22::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strForwardInfo;
	S_ForwardInfo *pForwardInfo = STATUS.GetForwardInfo();

	AppendDADT(strForwardInfo, CmdObj);
	strForwardInfo.append((char *)&pForwardInfo->m_GatherAddr,4);

	strResponse.push_back(strForwardInfo);
	return true;
}
//���ն˵ȴ����ն�Ӧ��ĳ�ʱ�ж�ʱ��
bool C_CmdrE0000A23::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strForwardInfo;
	S_ForwardInfo *pForwardInfo = STATUS.GetForwardInfo();

	AppendDADT(strForwardInfo, CmdObj);
	strForwardInfo.push_back((char)pForwardInfo->m_WaitSlaveResponseS);

	strResponse.push_back(strForwardInfo);
	return true;
}
//���ն˵ȴ����ն˼�����������ĳ�ʱ�ж�ʱ��
bool C_CmdrE0000A24::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strForwardInfo;
	S_ForwardInfo *pForwardInfo = STATUS.GetForwardInfo();

	AppendDADT(strForwardInfo, CmdObj);
	strForwardInfo.push_back((char)pForwardInfo->m_WaitMasterCtrlS);

	strResponse.push_back(strForwardInfo);
	return true;
}
//����̨���ն�CT����
bool C_CmdrE0000A25::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strForwardInfo;
	S_ForwardInfo *pForwardInfo = STATUS.GetForwardInfo();

	AppendDADT(strForwardInfo, CmdObj);
	strForwardInfo.append((char *)&pForwardInfo->m_CT,2);

	strResponse.push_back(strForwardInfo);
	return true;
}
//����̨���ն�PT����
bool C_CmdrE0000A26::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strForwardInfo;
	S_ForwardInfo *pForwardInfo = STATUS.GetForwardInfo();

	AppendDADT(strForwardInfo, CmdObj);
	strForwardInfo.append((char *)&pForwardInfo->m_PT,2);

	strResponse.push_back(strForwardInfo);
	return true;
}
//��ѯ
bool C_CmdrE0000A30::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//�������
bool C_CmdrE0000A31::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//�ظ������������
bool C_CmdrE0000A32::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//�����ն�̨���ܵ���
bool C_CmdrE0000A33::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//��Լ�汾��
bool C_CmdrE0000B00::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_WORD RuleVer = CONS_MSRULE_VER;

	string strVer;
	AppendDADT(strVer, CmdObj);
	strVer.append((char *)&RuleVer, 2);

	strResponse.push_back(strVer);
	return true;
}
//�ն�����汾��
bool C_CmdrE0000B01::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	const INT16U FactCode = CONS_FACTORY_CODE_BIN;

	string strVer;
	AppendDADT(strVer, CmdObj);
	strVer.append((char *)&FactCode, 2);
	strVer.push_back((char)CONS_TERM_TYPE);
	int year,mon,day;
	sscanf(CONS_SW_RELEASE_DATE, "%d-%d-%d", &year, &mon, &day);
	strVer.push_back((char)kge_cbcd(day));
	strVer.push_back((char)kge_cbcd(mon));
	strVer.push_back((char)kge_cbcd(year%100));

	strResponse.push_back(strVer);
	return true;
}
//�ն�Ӳ���汾��
bool C_CmdrE0000B02::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	const INT16U HardVer = 0x0001;

	string strVer;
	AppendDADT(strVer, CmdObj);
	strVer.append((char *)&HardVer, 2);

	strResponse.push_back(strVer);
	return true;
}
//�û��Զ�������CCCC
bool C_CmdrE0002000::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}


//��������������
//���г���������ñ�
bool C_CmdrE0000200::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	int Process = CONTEXT.GetDbcjPrecess_VCom((S_BYTE)((CmdObj.m_DI&0xFF)+1));
	if (Process < 0)
		return false;
	S_DbcjInfo *pDbcjInfo = STATUS.GetDbcjInfo((S_DbcjSN)Process);
	if (pDbcjInfo == NULL)
		return false;

	char bcdStr7B[7];
	C_TIME::Time2BCDStr(pDbcjInfo->m_GatherStdTime, bcdStr7B);

	string strInfo;
	AppendDADT(strInfo, CmdObj);
	strInfo.append(bcdStr7B+1, 5);
	strInfo.push_back(pDbcjInfo->m_GatherPeriodUnit - TIME_UNIT_MINUTE);
	strInfo.push_back(pDbcjInfo->m_GatherPeriodVal);
	strResponse.push_back(strInfo);
	return true;
}
bool C_CmdrE0000230::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_WORD NO = (S_WORD)(CmdObj.m_DI - 0xE0000230);
	if (NO >= CONS_TASK_COUNT_MAX)
		return false;
	S_GatherTaskInfos *pGatherTaskInfos = STATUS.GetGatherTaskInfos();
	if (pGatherTaskInfos == NULL)
		return false;

	string strInfo;
	AppendDADT(strInfo, CmdObj);

	S_GatherTaskInfo &GatherTaskInfo = pGatherTaskInfos->m_ItemInfo[NO];
	strInfo.push_back(GatherTaskInfo.m_Enable?0x01:0x00);
	strInfo.push_back(kge_cbcd(GatherTaskInfo.m_RecordMax%100));
	strInfo.push_back(kge_cbcd(GatherTaskInfo.m_RecordMax/100%100));
	strInfo.push_back(GatherTaskInfo.m_MeterType);
	strInfo.push_back(GatherTaskInfo.m_VipFlag);

	strInfo.push_back((S_BYTE)GatherTaskInfo.m_DaCnt);
	for(S_BYTE pn = 0; pn < GatherTaskInfo.m_DaCnt; pn++)
	{
		strInfo.append((char *)&GatherTaskInfo.m_DA[pn], 2);
//		strInfo.push_back((S_BYTE)(GatherTaskInfo.m_DA[pn]>>0));
//		strInfo.push_back((S_BYTE)(GatherTaskInfo.m_DA[pn]>>8));
	}

	strInfo.push_back((S_BYTE)GatherTaskInfo.m_DtCnt);
	for(S_BYTE tn = 0; tn < GatherTaskInfo.m_DtCnt; tn++)
	{
		strInfo.append((char *)&GatherTaskInfo.m_DT[tn], 4);
//		strInfo.push_back((S_BYTE)(GatherTaskInfo.m_DT[tn]>>0));
//		strInfo.push_back((S_BYTE)(GatherTaskInfo.m_DT[tn]>>8));
//		strInfo.push_back((S_BYTE)(GatherTaskInfo.m_DT[tn]>>16));
//		strInfo.push_back((S_BYTE)(GatherTaskInfo.m_DT[tn]>>24));
	}

	strResponse.push_back(strInfo);
	return true;
}

//������� ��ͨ�����
bool C_CmdrE0000301::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_CHAR bcdtime[5];
	S_NormalTaskInfos *pTaskInfos = STATUS.GetNormalTaskInfos();
	S_NormalTaskInfo &pInfo = pTaskInfos->m_ItemInfo[CmdObj.m_DI - NORMAL_TASK_ID_MIN];

	string strInfo;
	AppendDADT(strInfo, CmdObj);

	strInfo.push_back(pInfo.m_Active?0x01:0x00);
	strInfo.append(C_TIME::Time2BcdStr5B(pInfo.m_ReportStdTime, bcdtime), 5);

	strInfo.push_back((S_BYTE)pInfo.m_ReportPeriodUnit);
	strInfo.push_back((S_BYTE)pInfo.m_ReportPeriodVal);
	strInfo.push_back((S_BYTE)pInfo.m_DataFormat);

	strInfo.append(C_TIME::Time2BcdStr5B(pInfo.m_SampleStdTime, bcdtime), 5);
	strInfo.push_back((S_BYTE)pInfo.m_SamplePeriodUnit);
	strInfo.push_back((S_BYTE)pInfo.m_SamplePeriodVal);

	strInfo.push_back((S_BYTE)pInfo.m_ExtractRate);
	strInfo.push_back((S_BYTE)pInfo.m_MaxTimes);

	strInfo.push_back((S_BYTE)pInfo.m_DaCnt);
	S_BYTE bytemp[2]={0x00,0x00};
	for(S_BYTE pn=0; pn < pInfo.m_DaCnt;pn++)
	{
		KGE_WRITEWORD(bytemp,pInfo.m_DA[pn]);
		strInfo.append((S_CHAR*)bytemp,2);
	}

	strInfo.push_back((S_BYTE)pInfo.m_DtCnt);
	S_BYTE bytemp0[4]={0x00,0x00,0x00,0x00};
	for(S_BYTE tn = 0; tn < pInfo.m_DtCnt; tn++)
	{
		KGE_WRITEDWORD(bytemp0,pInfo.m_DT[tn]);
		strInfo.append((S_CHAR*)bytemp0,4);
	}	
	strResponse.push_back(strInfo);
	return true;
}
//������� �м������
bool C_CmdrE0000401::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_CHAR bcdtime[5];
	S_ForwardTaskInfos *pTaskInfos = STATUS.GetForwardTaskInfos();
	S_ForwardTaskInfo  &pInfo = pTaskInfos->m_ItemInfo[CmdObj.m_DI - FORWARD_TASK_ID_MIN];

	string strInfo;
	AppendDADT(strInfo, CmdObj);

	strInfo.push_back(pInfo.m_Active?0x01:0x00);
	strInfo.append(C_TIME::Time2BcdStr5B(pInfo.m_ReportStdTime, bcdtime), 5);
	strInfo.push_back((S_BYTE)pInfo.m_ReportPeriodUnit);
	strInfo.push_back((S_BYTE)pInfo.m_ReportPeriodVal);

	strInfo.append(C_TIME::Time2BcdStr5B(pInfo.m_SampleStdTime, bcdtime), 5);
	strInfo.push_back((S_BYTE)pInfo.m_SamplePeriodUnit);
	strInfo.push_back((S_BYTE)pInfo.m_SamplePeriodVal);

	strInfo.push_back((S_BYTE)pInfo.m_ExtractRate);
	strInfo.push_back((S_BYTE)pInfo.m_ForwardType);
	strInfo.push_back((S_BYTE)pInfo.m_MaxTimes);

	strInfo.push_back((S_BYTE)pInfo.m_PortSet.m_PortNO);
	strInfo.push_back((S_BYTE)(pInfo.m_PortSet.m_Baud));
//	S_BYTE bParity=0x00;
//	if(pInfo.m_PortSet.m_Parity==COMM_PARITY_NO)
//		bParity=0x00;
//	else if(pInfo.m_PortSet.m_Parity==COMM_PARITY_ODD)
//		bParity=0x02;
//	else
//		bParity=0x01;
	strInfo.push_back(pInfo.m_PortSet.m_Parity);
	strInfo.push_back((S_BYTE)pInfo.m_PortSet.m_DataBit);

//	S_BYTE bStopbit=0x00;
//	if(pInfo.m_PortSet.m_StopBit==COMM_STOP_BIT3)
//		bStopbit=0x01;
//	else if(pInfo.m_PortSet.m_Parity==COMM_STOP_BIT2)
//		bStopbit=0x02;
//	else
//		bStopbit=0x00;
	strInfo.push_back(pInfo.m_PortSet.m_StopBit);

	strInfo.push_back((S_BYTE)pInfo.m_TimeoutS);
	strInfo.push_back((S_BYTE)pInfo.m_ForwardCmdLen);
	strInfo.append(pInfo.m_ForwardCmd,pInfo.m_ForwardCmdLen);
	strResponse.push_back(strInfo);
	return true;
}
//���������� ģ������
bool C_CmdrE0000501::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//���������� ��������
bool C_CmdrE0000601::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//���������� ���������
bool C_CmdrE0000701::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//���������� �����
bool C_CmdrE0000801::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//�˿ڲ��� 485�˿ڶ���
bool C_CmdrE0000A00::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_BYTE VCom = (S_BYTE)(CmdObj.m_DI & 0xFF);
	if (VCom > 2)
		return false;

	S_CHANNEL *pChannel = NULL;
	S_BYTE fun = VComFunction(VCom + 1);
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

	string strData;
	S_COMSET PortSet = pChannel->m_PortSet.m_ComSet;

	AppendDADT(strData, CmdObj);

	strData.push_back(fun);
	strData.push_back((char)(PortSet.m_Baud/300));

	INT8U Parity = 0x00;
	if (PortSet.m_Parity == 2) Parity = 0x01;
	else if (PortSet.m_Parity == 1) Parity = 0x02;
	strData.push_back(Parity);
	strData.push_back(PortSet.m_DataBit);

	INT8U StopBit = 0x02;
	if (PortSet.m_StopBit == 1) StopBit = 0x00;
	else if (PortSet.m_StopBit == 3) StopBit = 0x01;
	strData.push_back(StopBit);

	strResponse.push_back(strData);
	return true;
}

S_BYTE C_CmdrE0000A00::VComFunction(S_BYTE VCom)
{
	switch (VCom)
	{
	case 2:
		if (C_SYSCTRL::GetProcessEnable(MP_PROCESS_DBCJ2) == false)
			return 0x01; // 485���߼���
		break;
	case 3:
		if (C_SYSCTRL::GetProcessEnable(MP_PROCESS_DBCJ3) == false)
			return 0x03; // 485�ϴ�
		break;
	default:
		break;
	}

	return 0x00; // 485 ����
}


