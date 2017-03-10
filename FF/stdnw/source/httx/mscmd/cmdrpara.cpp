/** @file
*  @brief 后台通讯命令读参数服务定义
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
//主站通信参数主
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
//主站通信参数备用1
bool C_CmdrE0000101::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strMS2;

	AppendDADT(strMS2, CmdObj);
	if(ReadParam(strMS2,STATUS.GetHttxChannel(MP_HTTX_1, HTTX_CHANNEL_2))==false)
		return false;

	strResponse.push_back(strMS2);
	return true;
}
//主站通信参数备用2
bool C_CmdrE0000102::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strMS3;

	AppendDADT(strMS3, CmdObj);
	if(ReadParam(strMS3,STATUS.GetHttxChannel(MP_HTTX_1, HTTX_CHANNEL_3))==false)
		return false;

	strResponse.push_back(strMS3);
	return true;
}
//网关地址或者代理服务器地址和端口，参见主站通信地址的填写(这里选的是代理服务器)
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
//APN用户名
bool C_CmdrE0000105::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPppInfo;
	S_PppInfo *pPppInfo = STATUS.GetPppInfo();

	AppendDADT(strPppInfo, CmdObj);
	strPppInfo.append((char *)&pPppInfo->m_DialInfo.m_User[0], 32);

	strResponse.push_back(strPppInfo);
	return true;
}
//APN密码
bool C_CmdrE0000106::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_PppInfo *pPppInfo = STATUS.GetPppInfo();
	string strPppInfo;

	AppendDADT(strPppInfo, CmdObj);
	strPppInfo.append((char *)&pPppInfo->m_DialInfo.m_Pwd[0], 32);

	strResponse.push_back(strPppInfo);
	return true;
}
//心跳周期
bool C_CmdrE0000107::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strHeartBeatPeriodM;
	S_HttxInfo *pHttxInfo = STATUS.GetHttxInfo(MP_HTTX_1);//取后台通信1

	AppendDADT(strHeartBeatPeriodM, CmdObj);
	strHeartBeatPeriodM.push_back(kge_cbcd(pHttxInfo->m_HeartBeatPeriodM));

	strResponse.push_back(strHeartBeatPeriodM);
	return true;
}
//掉线重拨间隔，缺省30分钟
bool C_CmdrE0000108::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPppInfo;
	S_PppInfo *pPppInfo = STATUS.GetPppInfo();

	AppendDADT(strPppInfo, CmdObj);
	strPppInfo.push_back(kge_cbcd((char)pPppInfo->m_RedialInterM));

	strResponse.push_back(strPppInfo);
	return true;
}
//掉线后重拨次数，缺省3次,重拨指定次数后，当天不再重拨
bool C_CmdrE0000109::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPpp;
	S_PppInfo *pPppInfo = STATUS.GetPppInfo();

	AppendDADT(strPpp, CmdObj);
	strPpp.push_back(kge_cbcd((char)pPppInfo->m_RedialTimesPerDay));

	strResponse.push_back(strPpp);
	return true;
}
//连接方式，0：TCP方式；1：UDP方式
bool C_CmdrE000010A::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strMS;
	S_CHANNEL *pMS = STATUS.GetHttxChannel(MP_HTTX_1, HTTX_CHANNEL_1);//暂取通道1

	AppendDADT(strMS, CmdObj);
	if((pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP_SERVER)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP_CLIENT)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP))
		strMS.push_back(0x00);
	else
		strMS.push_back(0x01);

	strResponse.push_back(strMS);
	return true;
}
//压缩开关:0：非压缩  1：压缩，BIN编码
bool C_CmdrE000010B::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strData;
	S_HttxInfo *pHttxInfo = STATUS.GetHttxInfo(MP_HTTX_1);//取后台通信1

	AppendDADT(strData, CmdObj);
	strData.push_back(pHttxInfo->m_CompressEnable);

	strResponse.push_back(strData);
	return true;
}
//以上数据集合
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
	S_HttxInfo *pHttxInfo = STATUS.GetHttxInfo(MP_HTTX_1);//取后台通信1
	S_PppInfo *pPppInfo = STATUS.GetPppInfo();

	//主站通信参数
	for (int i = HTTX_CHANNEL_1; i < HTTX_CHANNL_MAX; i++)
	{
		if(ReadParam(strAllParam, STATUS.GetHttxChannel(MP_HTTX_1, (S_ChannelSN)i)) == false)
			return false;
	}
	//网关地址或者代理服务器地址和端口，参见主站通信地址的填写(这里选的是代理服务器)
	string strIP((char *)&pIpConfig->m_ProxyIP[0], 4);
	strAllParam.append((char *)&pIpConfig->m_ProxyPort,2);
	strAllParam.append(strIP.rbegin(), strIP.rend());
	strAllParam.append((size_t)(8-6),(char)MSCMD_RESPONSE_PPPINVALID_CHAR);
	//APN
	strAllParam.append((char *)&pPppInfo->m_DialInfo.m_Apn[0], 16);
	//APN用户名
	strAllParam.append((char *)&pPppInfo->m_DialInfo.m_User[0], 32);
	//APN密码
	strAllParam.append((char *)&pPppInfo->m_DialInfo.m_Pwd[0], 32);
	//心跳周期
	strAllParam.push_back(kge_cbcd(pHttxInfo->m_HeartBeatPeriodM));
	//掉线重拨间隔，缺省30分钟
	strAllParam.push_back(kge_cbcd((S_BYTE)pPppInfo->m_RedialInterM));
	//掉线后重拨次数，缺省3次,重拨指定次数后，当天不再重拨
	strAllParam.push_back(kge_cbcd((S_BYTE)pPppInfo->m_RedialTimesPerDay));
	//连接方式，0：TCP方式；1：UDP方式
	S_CHANNEL *pMS = STATUS.GetHttxChannel(MP_HTTX_1, HTTX_CHANNEL_1);//暂取通道1
	if((pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP_SERVER)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP_CLIENT)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP))
		strAllParam.push_back(0x00);
	else
		strAllParam.push_back(0x01);
	return true;
}
//终端地市区县码，BCD码
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
//终端地址，BIN编码
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
//终端通信工作模式
bool C_CmdrE0000122::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strMS;
	S_CHANNEL *pMS = STATUS.GetHttxChannel(MP_HTTX_1, HTTX_CHANNEL_1);//暂取通道1

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
//混合模式和服务器模式下，TCP监听端口号，BIN编码
bool C_CmdrE0000123::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strMS;
	S_CHANNEL *pMS = STATUS.GetHttxChannel(MP_HTTX_1, HTTX_CHANNEL_1);//暂取通道1

	AppendDADT(strMS, CmdObj);
	if((pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_UDP)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP_SERVER))
		strMS.append((char *)&pMS->m_PortSet.m_NetSet.m_PortNO,2);
	else
		return false;

	strResponse.push_back(strMS);
	return true;
}
//终端当前通道IP地址
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
//终端当前通道子网掩码地址
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
//终端当前通道网关地址
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
//有线网络IP地址获取方式 0：手动设置IP地址 1：自动设置IP地址
bool C_CmdrE0000127::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strIpConfig;
	S_IpConfig *pIpConfig = STATUS.GetIpConfig();

	AppendDADT(strIpConfig, CmdObj);
	strIpConfig.push_back(pIpConfig->m_IpMethod);

	strResponse.push_back(strIpConfig);
	return true;
}
//以上数据合集
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
	S_CHANNEL *pMS = STATUS.GetHttxChannel(MP_HTTX_1, HTTX_CHANNEL_1);//暂取通道1
	S_IpConfig *pIpConfig = STATUS.GetIpConfig();

	//终端地市区县码，BCD码+终端地址，BIN编码
	string strRtua;
	C_RTUA rtua(pSystemConfig->m_RTUA);
	rtua.PrintString(strRtua);
	strAllParam.append(strRtua.begin(), strRtua.begin()+6);
	//终端通信工作模式
	if((pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_UDP))
		strAllParam.push_back(0x00);
	else if((pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP_SERVER)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_UDP_SERVER))
		strAllParam.push_back(0x02);
	else
		strAllParam.append(0x01, (size_t)1);
	//混合模式和服务器模式下，TCP监听端口号，BIN编码
	if((pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_UDP)||(pMS->m_PortSet.m_NetSet.m_Type == NET_CONNECT_TCP_SERVER))
		strAllParam.append((char *)&pMS->m_PortSet.m_NetSet.m_PortNO,2);
	else
		strAllParam.append((size_t)2,(char)0x00);
	//终端当前通道IP地址
	string strIpData((char*)&pIpConfig->m_TermIP[0],4);
	strAllParam.append(strIpData.rbegin(), strIpData.rend());
	//终端当前通道子网掩码地址
	string strNetMask((char*)&pIpConfig->m_NetMask[0],4);
	strAllParam.append(strNetMask.rbegin(), strNetMask.rend());
	//终端当前通道网关地址
	string strGW((char*)&pIpConfig->m_GateWay[0],4);
	strAllParam.append(strGW.rbegin(), strGW.rend());
	//有线网络IP获取方式
	strAllParam.push_back(pIpConfig->m_IpMethod);
	return true;
}

//终端时间
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
//实时校时 无读
//终端复位 无读
//告警主动上报屏蔽字
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
//告警判断屏蔽字 
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
//事件记录屏蔽字
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
//月冻结电量 DD：结算时间的日，范围1~30，缺省为每月1号 HH：结算时间的小时，范围0~23，缺省为0点
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
//日冻结电量 HH：日电能量（表码）结算时刻，范围0~23，缺省为0点
bool C_CmdrE0000161::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strDbcjInfo;
	S_DbcjInfo *pDbcjInfo = STATUS.GetDbcjInfo(MP_DBCJ_1);

	AppendDADT(strDbcjInfo, CmdObj);
	strDbcjInfo.push_back((char)pDbcjInfo->m_DayBeginTime[0]);

	strResponse.push_back(strDbcjInfo);
	return true;
}
//月电压合格率数据冻结 DD：结算时间的日，范围1~30，缺省为每月21号HH：结算时间的小时，范围0~23，缺省为0点
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
//日电压合格率数据冻结 HH：日电能量（表码）结算时刻，范围0~23，缺省为0点
bool C_CmdrE0000163::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strDbcjInfo;
	S_DbcjInfo *pDbcjInfo = STATUS.GetDbcjInfo(MP_DBCJ_1);

	AppendDADT(strDbcjInfo, CmdObj);
	strDbcjInfo.push_back((char)pDbcjInfo->m_DayVolFreezeTime[0]);

	strResponse.push_back(strDbcjInfo);
	return true;
}
//认证请求信息，BIN编码，密文
bool C_CmdrE0000171::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//认证响应信息，BIN编码，密文
bool C_CmdrE0000172::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//随机数信息，BIN编码，密文
bool C_CmdrE0000173::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//随机数响应信息，BIN编码，密文
bool C_CmdrE0000174::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//判断负荷过载、电流过负荷、过电压A、超合同容量用电、功率超定值、电流不平衡度、电压畸变、电流畸变、零序电流偏大、无功欠补偿、无功过补偿的时间缺省15分钟
bool C_CmdrE0000180::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPowerLimit;
	S_PowerLimit *pPowerLimit = STATUS.GetPowerLimitInfo();

	AppendDADT(strPowerLimit, CmdObj);
	strPowerLimit.push_back(kge_cbcd((S_BYTE)pPowerLimit->m_OverLoadM));

	strResponse.push_back(strPowerLimit);
	return true;
}
//判断负荷过载、电流过负荷、过电压超合同容量用电、功率超定值、电流不平衡度、电压畸变、电流畸变、零序电流偏大、无功欠补偿、无功过补偿恢复时间缺省15分钟
bool C_CmdrE0000181::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPowerLimit;
	S_PowerLimit *pPowerLimit = STATUS.GetPowerLimitInfo();

	AppendDADT(strPowerLimit, CmdObj);
	strPowerLimit.push_back(kge_cbcd((S_BYTE)pPowerLimit->m_OverLoadEndM));

	strResponse.push_back(strPowerLimit);
	return true;
}
//阈值参数，判断断相、失压、失流时间，缺省值1分钟
bool C_CmdrE0000182::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPowerLimit;
	S_PowerLimit *pPowerLimit = STATUS.GetPowerLimitInfo();

	AppendDADT(strPowerLimit, CmdObj);
	strPowerLimit.push_back(kge_cbcd((S_BYTE)pPowerLimit->m_MissingPhaseM));

	strResponse.push_back(strPowerLimit);
	return true;
}
//阈值参数，判断断相、失压、失流恢复时间，缺省值1分钟
bool C_CmdrE0000183::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPowerLimit;
	S_PowerLimit *pPowerLimit = STATUS.GetPowerLimitInfo();

	AppendDADT(strPowerLimit, CmdObj);
	strPowerLimit.push_back(kge_cbcd((S_BYTE)pPowerLimit->m_MissingPhaseEndM));

	strResponse.push_back(strPowerLimit);
	return true;
}
//阈值参数，判断电压逆相序时间，缺省值15分钟
bool C_CmdrE0000184::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPowerLimit;
	S_PowerLimit *pPowerLimit = STATUS.GetPowerLimitInfo();

	AppendDADT(strPowerLimit, CmdObj);
	strPowerLimit.push_back(kge_cbcd((S_BYTE)pPowerLimit->m_ReversedPhaseM));

	strResponse.push_back(strPowerLimit);
	return true;
}
//阈值参数，判断电压逆相序恢复时间，缺省值15分钟
bool C_CmdrE0000185::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPowerLimit;
	S_PowerLimit *pPowerLimit = STATUS.GetPowerLimitInfo();

	AppendDADT(strPowerLimit, CmdObj);
	strPowerLimit.push_back(kge_cbcd((S_BYTE)pPowerLimit->m_ReversedPhaseEndM));

	strResponse.push_back(strPowerLimit);
	return true;
}
//阈值参数，判断电流反极性时间，缺省值15分钟
bool C_CmdrE0000186::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPowerLimit;
	S_PowerLimit *pPowerLimit = STATUS.GetPowerLimitInfo();

	AppendDADT(strPowerLimit, CmdObj);
	strPowerLimit.push_back(kge_cbcd((S_BYTE)pPowerLimit->m_ReversedPolarityM));

	strResponse.push_back(strPowerLimit);
	return true;
}
//阈值参数，判断电流反极性恢复时间，缺省值15分钟
bool C_CmdrE0000187::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPowerLimit;
	S_PowerLimit *pPowerLimit = STATUS.GetPowerLimitInfo();

	AppendDADT(strPowerLimit, CmdObj);
	strPowerLimit.push_back(kge_cbcd((S_BYTE)pPowerLimit->m_ReversedPolarityEndM));

	strResponse.push_back(strPowerLimit);
	return true;
}
//阈值参数，电能表时钟异常阈值，缺省值10分钟
bool C_CmdrE0000188::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPowerLimit;
	S_PowerLimit *pPowerLimit = STATUS.GetPowerLimitInfo();

	AppendDADT(strPowerLimit, CmdObj);
	strPowerLimit.push_back(kge_cbcd((S_BYTE)pPowerLimit->m_MeterTimeDiffM));

	strResponse.push_back(strPowerLimit);
	return true;
}
//阈值参数，电能表飞走阈值，缺省为10倍
bool C_CmdrE0000189::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPowerLimit;
	S_PowerLimit *pPowerLimit = STATUS.GetPowerLimitInfo();

	AppendDADT(strPowerLimit, CmdObj);
	strPowerLimit.push_back(kge_cbcd((S_BYTE)pPowerLimit->m_MeterFly));

	strResponse.push_back(strPowerLimit);
	return true;
}
//阈值参数，电能表停走阈值：电量示度停止增长时按照有功功率计算应走电量值，越此值则电能表停走，缺省为0.1kWh
bool C_CmdrE000018A::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPowerLimit;
	S_PowerLimit *pPowerLimit = STATUS.GetPowerLimitInfo();

	AppendDADT(strPowerLimit, CmdObj);
	strPowerLimit.append((char *)&pPowerLimit->m_MeterStop,2);

	strResponse.push_back(strPowerLimit);
	return true;
}
//阈值参数，剩余金额不足阈值：缺省为20.00 元
bool C_CmdrE000018B::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPowerLimit;
	S_PowerLimit *pPowerLimit = STATUS.GetPowerLimitInfo();

	AppendDADT(strPowerLimit, CmdObj);
	strPowerLimit.append((char *)&pPowerLimit->m_MeterLastFee,3);

	strResponse.push_back(strPowerLimit);
	return true;
}
//阈值参数，月通信流量越限值
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
//以上数据项集合
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
//有效的抄表参数配置表总数 (只读)
bool C_CmdrE0000221::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strData;
	S_BYTE Cnt = 0x20;

	AppendDADT(strData, CmdObj);
	strData.push_back((char)Cnt);

	strResponse.push_back(strData);
	return true;
}
//具体表属性对应的抄表参数配置表
bool C_CmdrE0000222::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//现有普通任务总数 (只读)
bool C_CmdrE0000300::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strData;
	S_BYTE Cnt = 0xFE;

	AppendDADT(strData, CmdObj);
	strData.push_back((char)Cnt);

	strResponse.push_back(strData);
	return true;
}
//现有中继任务总数
bool C_CmdrE0000400::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strData;
	S_BYTE Cnt = 0xFE;

	AppendDADT(strData, CmdObj);
	strData.push_back((char)Cnt);

	strResponse.push_back(strData);
	return true;
}
//模拟量数量
bool C_CmdrE0000500::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strANALOGS;
	S_ANALOGS *pANALOGS = STATUS.GetAnalogInfos();

	AppendDADT(strANALOGS, CmdObj);
	strANALOGS.push_back((char)pANALOGS->m_AnalogCnt);

	strResponse.push_back(strANALOGS);
	return true;
}
//模拟量状态标记
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
//以上数据项集合
bool C_CmdrE00005FF::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//脉冲量数量
bool C_CmdrE0000600::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPULSES;
	S_PULSES *pPULSES = STATUS.GetPulseInfos();

	AppendDADT(strPULSES, CmdObj);
	strPULSES.push_back((char)pPULSES->m_PulseCnt);

	strResponse.push_back(strPULSES);
	return true;
}
//脉冲量状态标记
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
//以上数据集合
bool C_CmdrE00006FF::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//运算量数量
bool C_CmdrE0000700::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strSUMS;
	S_SUMS *pSUMS = STATUS.GetSumInfos();

	AppendDADT(strSUMS, CmdObj);
	strSUMS.push_back((char)pSUMS->m_SumCnt);

	strResponse.push_back(strSUMS);
	return true;
}
//运算量状态标记
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
//以上数据集合
bool C_CmdrE00007FF::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//差动规则数量
bool C_CmdrE0000800::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strDiffRuleS;
	S_DiffRuleS *pDiffRuleS = STATUS.GetDiffRuleInfos();

	AppendDADT(strDiffRuleS, CmdObj);
	strDiffRuleS.push_back((char)pDiffRuleS->m_DiffRuleCnt);

	strResponse.push_back(strDiffRuleS);
	return true;
}
//差动规则状态标记
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
//以上数据集合
bool C_CmdrE00008FF::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//终端无功电量统计标志
bool C_CmdrE0000900::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//级联从终端地址1~4
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
//主终端地址
bool C_CmdrE0000A21::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strForwardInfo;
	S_ForwardInfo *pForwardInfo = STATUS.GetForwardInfo();

	AppendDADT(strForwardInfo, CmdObj);
	strForwardInfo.append((char *)&pForwardInfo->m_MasterAddr,4);

	strResponse.push_back(strForwardInfo);
	return true;
}
//被抄终端地址
bool C_CmdrE0000A22::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strForwardInfo;
	S_ForwardInfo *pForwardInfo = STATUS.GetForwardInfo();

	AppendDADT(strForwardInfo, CmdObj);
	strForwardInfo.append((char *)&pForwardInfo->m_GatherAddr,4);

	strResponse.push_back(strForwardInfo);
	return true;
}
//主终端等待从终端应答的超时判定时间
bool C_CmdrE0000A23::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strForwardInfo;
	S_ForwardInfo *pForwardInfo = STATUS.GetForwardInfo();

	AppendDADT(strForwardInfo, CmdObj);
	strForwardInfo.push_back((char)pForwardInfo->m_WaitSlaveResponseS);

	strResponse.push_back(strForwardInfo);
	return true;
}
//从终端等待主终端级联控制命令的超时判定时间
bool C_CmdrE0000A24::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strForwardInfo;
	S_ForwardInfo *pForwardInfo = STATUS.GetForwardInfo();

	AppendDADT(strForwardInfo, CmdObj);
	strForwardInfo.push_back((char)pForwardInfo->m_WaitMasterCtrlS);

	strResponse.push_back(strForwardInfo);
	return true;
}
//级联台区终端CT倍率
bool C_CmdrE0000A25::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strForwardInfo;
	S_ForwardInfo *pForwardInfo = STATUS.GetForwardInfo();

	AppendDADT(strForwardInfo, CmdObj);
	strForwardInfo.append((char *)&pForwardInfo->m_CT,2);

	strResponse.push_back(strForwardInfo);
	return true;
}
//级联台区终端PT倍率
bool C_CmdrE0000A26::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strForwardInfo;
	S_ForwardInfo *pForwardInfo = STATUS.GetForwardInfo();

	AppendDADT(strForwardInfo, CmdObj);
	strForwardInfo.append((char *)&pForwardInfo->m_PT,2);

	strResponse.push_back(strForwardInfo);
	return true;
}
//轮询
bool C_CmdrE0000A30::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//传输控制
bool C_CmdrE0000A31::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//回复传输控制命令
bool C_CmdrE0000A32::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//抄读终端台区总电量
bool C_CmdrE0000A33::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//规约版本号
bool C_CmdrE0000B00::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	S_WORD RuleVer = CONS_MSRULE_VER;

	string strVer;
	AppendDADT(strVer, CmdObj);
	strVer.append((char *)&RuleVer, 2);

	strResponse.push_back(strVer);
	return true;
}
//终端软件版本号
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
//终端硬件版本号
bool C_CmdrE0000B02::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	const INT16U HardVer = 0x0001;

	string strVer;
	AppendDADT(strVer, CmdObj);
	strVer.append((char *)&HardVer, 2);

	strResponse.push_back(strVer);
	return true;
}
//用户自定义数据CCCC
bool C_CmdrE0002000::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}


//二个数据项区域
//现有抄表参数配置表
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

//任务参数 普通任务号
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
//任务参数 中继任务号
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
//运算量参数 模拟量号
bool C_CmdrE0000501::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//运算量参数 脉冲量号
bool C_CmdrE0000601::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//运算量参数 运算量编号
bool C_CmdrE0000701::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//运算量参数 差动规则
bool C_CmdrE0000801::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//端口参数 485端口定义
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
			return 0x01; // 485总线级联
		break;
	case 3:
		if (C_SYSCTRL::GetProcessEnable(MP_PROCESS_DBCJ3) == false)
			return 0x03; // 485上传
		break;
	default:
		break;
	}

	return 0x00; // 485 抄表
}


