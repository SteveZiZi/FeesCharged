/** @file
 *  @brief 后台通讯命令测量点读参数对象实现
 *  @author  zhongguangxiong
 *  @date 2012/02/18
 *  @version 0.1
 */
#include "cmdrmp.h"
#include "status.h"
#include "context.h"

/** @brief 测量点参数读操作的基类 */
bool C_RMCmdReceiver::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	C_DA da(CmdObj.m_DA);
	if (da.GetMpCount() == 0xFF)
	{//完成所有测量点的组织
		return false;//TODO:
	}

	string strInfo;
	AppendDADT(strInfo, CmdObj);
	for (INT16U DA1 = 0; DA1 < 8; DA1++)
	{
		if (da.TestMP(DA1) == true)
		{
			if (da.GetMP(DA1) >= CONS_MP_COUNT_MAX)
				return false;
			if (ReadDataByMP(da.GetMP(DA1), strInfo) == false)
				return false;
		}
	}

	strResponse.push_back(strInfo);
	return true;
}
bool C_RMCmdReceiver::ReadDataByMP(S_WORD MP, string &strData)
{
	logwt<<"Please complete C_CmdrE0xxxxxx::ReadDataByMP()."<<endl;
	return false;
}
//测量点的状态 
bool C_CmdrE0800000::ReadDataByMP(S_WORD MP, string &strData)
{
	S_MpInfos *pMpInfos = STATUS.GetMpInfos();
	strData.push_back(pMpInfos->m_Item[MP].m_AutoGather);
	return true;
}
//测量点性质
bool C_CmdrE0800001::ReadDataByMP(S_WORD MP, string &strData)
{
	S_MpInfos *pMpInfos = STATUS.GetMpInfos();
	strData.push_back(pMpInfos->m_Item[MP].m_Type);
	return true;
}
//测量点地址
bool C_CmdrE0800002::ReadDataByMP(S_WORD MP, string &strData)
{
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	strData.append(pMeterInfos->m_Item[MP].m_Address, 6);
	return true;
}
//测量点通信规约
bool C_CmdrE0800003::ReadDataByMP(S_WORD MP, string &strData)
{
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	strData.push_back(pMeterInfos->m_Item[MP].m_RuleID);
	return true;
}
//电能表类型
bool C_CmdrE0800004::ReadDataByMP(S_WORD MP, string &strData)
{
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	strData.push_back(pMeterInfos->m_Item[MP].m_FunType);
	return true;
}
//总分类型
bool C_CmdrE0800005::ReadDataByMP(S_WORD MP, string &strData)
{
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	strData.push_back(pMeterInfos->m_Item[MP].m_MainFlag==true?0x01:0x00);
	return true;
}
//重点用户属性
bool C_CmdrE0800006::ReadDataByMP(S_WORD MP, string &strData)
{
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	strData.push_back(pMeterInfos->m_Item[MP].m_VipFlag==true?0x01:0x00);
	return true;
}
//拉闸功能
bool C_CmdrE0800007::ReadDataByMP(S_WORD MP, string &strData)
{
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	strData.push_back(pMeterInfos->m_Item[MP].m_PowerCut==true?0x01:0x00);
	return true;
}
//最大费率数
bool C_CmdrE0800008::ReadDataByMP(S_WORD MP, string &strData)
{
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	strData.push_back(pMeterInfos->m_Item[MP].m_TariffCnt);
	return true;
}
//测量点对应采集终端地址
bool C_CmdrE0800009::ReadDataByMP(S_WORD MP, string &strData)
{
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	strData.append((char *)pMeterInfos->m_Item[MP].m_RelayAddr, 6);
	return true;
}
//测量点端口号
bool C_CmdrE080000A::ReadDataByMP(S_WORD MP, string &strData)
{
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	strData.push_back((S_BYTE)(pMeterInfos->m_Item[MP].m_PortSet.m_PortNO - 1));
	return true;
}
//端口参数
bool C_CmdrE080000B::ReadDataByMP(S_WORD MP, string &strData)
{
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	strData.push_back((char)(pMeterInfos->m_Item[MP].m_PortSet.m_Baud/300));

	INT8U Parity = 0x00;
	if (pMeterInfos->m_Item[MP].m_PortSet.m_Parity == 2) Parity = 0x01;
	else if (pMeterInfos->m_Item[MP].m_PortSet.m_Parity == 1) Parity = 0x02;
	strData.push_back(Parity);
	strData.push_back(pMeterInfos->m_Item[MP].m_PortSet.m_DataBit);

	INT8U StopBit = 0x02;
	if (pMeterInfos->m_Item[MP].m_PortSet.m_StopBit == 1) StopBit = 0x00;
	else if (pMeterInfos->m_Item[MP].m_PortSet.m_StopBit == 3) StopBit = 0x01;
	strData.push_back(StopBit);

	return true;
}
//CT变比
bool C_CmdrE080000C::ReadDataByMP(S_WORD MP, string &strData)
{
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	strData.push_back(kge_cbcd(pMeterInfos->m_Item[MP].m_CT%100));
	strData.push_back(kge_cbcd(pMeterInfos->m_Item[MP].m_CT/100));
	return true;
}
//PT变比
bool C_CmdrE080000D::ReadDataByMP(S_WORD MP, string &strData)
{
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	strData.push_back(kge_cbcd(pMeterInfos->m_Item[MP].m_PT%100));
	strData.push_back(kge_cbcd(pMeterInfos->m_Item[MP].m_PT/100));
	return true;
}
//以上数据项集合
bool C_CmdrE080000F::ReadDataByMP(S_WORD MP, string &strData)
{
	S_MpInfos *pMpInfos = STATUS.GetMpInfos();
	if (pMpInfos->m_Item[MP].m_Type != MP_TYPE_METER)
		return false;

	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_MeterInfo &meter = pMeterInfos->m_Item[MP];
	if (meter.m_Enable == false)
		return false;

	strData.push_back(pMpInfos->m_Item[MP].m_AutoGather);
	strData.push_back(pMpInfos->m_Item[MP].m_Type);
	strData.append((char *)meter.m_Address, 6);
	strData.push_back(meter.m_RuleID);
	strData.push_back(meter.m_FunType);
	strData.push_back(meter.m_MainFlag==true?0x01:0x00);
	strData.push_back(meter.m_VipFlag==true?0x01:0x00);
	strData.push_back(meter.m_PowerCut==true?0x01:0x00);
	strData.push_back(meter.m_TariffCnt);
	strData.append((char *)meter.m_RelayAddr, 6);
	strData.push_back((S_BYTE)(meter.m_PortSet.m_PortNO - 1));
	strData.push_back((char)(meter.m_PortSet.m_Baud/300));

	INT8U Parity = 0x00;
	if (meter.m_PortSet.m_Parity == 2) Parity = 0x01;
	else if (meter.m_PortSet.m_Parity == 1) Parity = 0x02;
	strData.push_back(Parity);
	strData.push_back(meter.m_PortSet.m_DataBit);

	INT8U StopBit = 0x02;
	if (meter.m_PortSet.m_StopBit == 1) StopBit = 0x00;
	else if (meter.m_PortSet.m_StopBit == 3) StopBit = 0x01;
	strData.push_back(StopBit);

	strData.push_back(kge_cbcd(meter.m_CT%100));
	strData.push_back(kge_cbcd(meter.m_CT/100));
	strData.push_back(kge_cbcd(meter.m_PT%100));
	strData.push_back(kge_cbcd(meter.m_PT/100));

	return true;
}
//
bool C_CmdrE0800010::ReadDataByMP(S_WORD MP, string &strData)
{
	S_MpInfos *pMpInfos = STATUS.GetMpInfos();
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();

	S_MeterInfo &meter = pMeterInfos->m_Item[MP];
	if (pMpInfos->m_Item[MP].m_Type == MP_TYPE_UNKNOW)
		strData.push_back((INT8U)MSCMD_RESPONSE_INVALID_CHAR);
	else
		strData.push_back(meter.m_WireType);
	return true;
}
//
bool C_CmdrE0800011::ReadDataByMP(S_WORD MP, string &strData)
{
	S_MpInfos *pMpInfos = STATUS.GetMpInfos();
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();

	S_MeterInfo &meter = pMeterInfos->m_Item[MP];
	if (pMpInfos->m_Item[MP].m_Type == MP_TYPE_UNKNOW)
		strData.append((size_t)2,(INT8U)MSCMD_RESPONSE_INVALID_CHAR);
	else
		strData.append((char*)&meter.m_RatedVoltage,2);

	return true;
}
//
bool C_CmdrE0800012::ReadDataByMP(S_WORD MP, string &strData)
{
	S_MpInfos *pMpInfos = STATUS.GetMpInfos();
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();

	S_MeterInfo &meter = pMeterInfos->m_Item[MP];
	if (pMpInfos->m_Item[MP].m_Type == MP_TYPE_UNKNOW)
		strData.append((size_t)3, (INT8U)MSCMD_RESPONSE_INVALID_CHAR);
	else
		strData.append((char*)&meter.m_RatedCur,3);

	return true;
}
//
bool C_CmdrE0800013::ReadDataByMP(S_WORD MP, string &strData)
{
	S_MpInfos *pMpInfos = STATUS.GetMpInfos();
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();

	S_MeterInfo &meter = pMeterInfos->m_Item[MP];
	if (pMpInfos->m_Item[MP].m_Type == MP_TYPE_UNKNOW)
		strData.append((size_t)2, (INT8U)MSCMD_RESPONSE_INVALID_CHAR);
	else
		strData.append((char*)&meter.m_RatedContent,2);

	return true;
}
//
bool C_CmdrE0800014::ReadDataByMP(S_WORD MP, string &strData)
{
	S_MpInfos *pMpInfos = STATUS.GetMpInfos();
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();

	S_MeterInfo &meter = pMeterInfos->m_Item[MP];
	if (pMpInfos->m_Item[MP].m_Type == MP_TYPE_UNKNOW)
		strData.append((size_t)2, (INT8U)MSCMD_RESPONSE_INVALID_CHAR);
	else
		strData.append((char*)&meter.m_VTContent,2);

	return true;
}
//
bool C_CmdrE080001F::ReadDataByMP(S_WORD MP, string &strData)
{	
	S_MpInfos *pMpInfos = STATUS.GetMpInfos();
	if (pMpInfos->m_Item[MP].m_Type == MP_TYPE_UNKNOW)
		return false;

	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_MeterInfo &meter = pMeterInfos->m_Item[MP];

	strData.push_back(meter.m_WireType);	
	strData.append((char*)&meter.m_RatedVoltage,2);
	strData.append((char*)&meter.m_RatedCur,3);
	strData.append((char*)&meter.m_RatedContent,2);
	strData.append((char*)&meter.m_VTContent,2);
	return true;
}
bool C_CmdrE0800021::ReadDataByMP(S_WORD MP, string &strData)
{
	S_MpInfos *pMpInfos = STATUS.GetMpInfos();
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();

	S_MeterInfo &meter = pMeterInfos->m_Item[MP];
	if (pMpInfos->m_Item[MP].m_Type == MP_TYPE_UNKNOW)
		strData.append((size_t)8, (INT8U)MSCMD_RESPONSE_INVALID_CHAR);
	else
		strData.append((char*)&meter.m_Password, 8);

	return true;
}
bool C_CmdrE0800022::ReadDataByMP(S_WORD MP, string &strData)
{
	S_MpInfos *pMpInfos = STATUS.GetMpInfos();
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();

	S_MeterInfo &meter = pMeterInfos->m_Item[MP];
	if (pMpInfos->m_Item[MP].m_Type == MP_TYPE_UNKNOW)
		strData.append((size_t)8, (INT8U)MSCMD_RESPONSE_INVALID_CHAR);
	else
		strData.append((char*)&meter.m_UserName, 8);

	return true;
}
bool C_CmdrE080002F::ReadDataByMP(S_WORD MP, string &strData)
{
	S_MpInfos *pMpInfos = STATUS.GetMpInfos();
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();

	S_MeterInfo &meter = pMeterInfos->m_Item[MP];
	if (pMpInfos->m_Item[MP].m_Type == MP_TYPE_UNKNOW)
		strData.append((size_t)16, (INT8U)MSCMD_RESPONSE_INVALID_CHAR);
	else
		strData.append((char*)&meter.m_Password, 8).append((char*)&meter.m_UserName, 8);

	return true;
}

//电流不平衡度阈值
bool C_CmdrE0800100::ReadDataByMP(S_WORD MP, string &strData)
{
	return false;
}
//
bool C_CmdrE0800101::ReadDataByMP(S_WORD MP, string &strData)
{
	return false;
}
//
bool C_CmdrE0800102::ReadDataByMP(S_WORD MP, string &strData)
{
	return false;
}
//
bool C_CmdrE0800103::ReadDataByMP(S_WORD MP, string &strData)
{
	return false;
}
//
bool C_CmdrE0800104::ReadDataByMP(S_WORD MP, string &strData)
{
	return false;
}
//
bool C_CmdrE0800105::ReadDataByMP(S_WORD MP, string &strData)
{
	return false;
}
//
bool C_CmdrE0800106::ReadDataByMP(S_WORD MP, string &strData)
{
	return false;
}
//
bool C_CmdrE0800107::ReadDataByMP(S_WORD MP, string &strData)
{
	return false;
}
//
bool C_CmdrE0800108::ReadDataByMP(S_WORD MP, string &strData)
{
	return false;
}
//
bool C_CmdrE0800109::ReadDataByMP(S_WORD MP, string &strData)
{
	return false;
}
//
bool C_CmdrE080010A::ReadDataByMP(S_WORD MP, string &strData)
{
	return false;
}
//
bool C_CmdrE080010B::ReadDataByMP(S_WORD MP, string &strData)
{
	return false;
}
//
bool C_CmdrE080010C::ReadDataByMP(S_WORD MP, string &strData)
{
	return false;
}
//
bool C_CmdrE080010F::ReadDataByMP(S_WORD MP, string &strData)
{
	return false;
}
//
bool C_CmdrE0800200::ReadDataByMP(S_WORD MP, string &strData)
{
	return false;
}
//
bool C_CmdrE0800201::ReadDataByMP(S_WORD MP, string &strData)
{
	return false;
}
//
bool C_CmdrE0800202::ReadDataByMP(S_WORD MP, string &strData)
{
	return false;
}
//
bool C_CmdrE0800203::ReadDataByMP(S_WORD MP, string &strData)
{
	return false;
}
//
bool C_CmdrE0800204::ReadDataByMP(S_WORD MP, string &strData)
{
	return false;
}
//
bool C_CmdrE0800205::ReadDataByMP(S_WORD MP, string &strData)
{
	return false;
}
//
bool C_CmdrE0800206::ReadDataByMP(S_WORD MP, string &strData)
{
	return false;
}
//
bool C_CmdrE0800207::ReadDataByMP(S_WORD MP, string &strData)
{
	return false;
}
//
bool C_CmdrE0800208::ReadDataByMP(S_WORD MP, string &strData)
{
	return false;
}
//
bool C_CmdrE0800209::ReadDataByMP(S_WORD MP, string &strData)
{
	return false;
}
//
bool C_CmdrE080020A::ReadDataByMP(S_WORD MP, string &strData)
{
	return false;
}
//
bool C_CmdrE080020B::ReadDataByMP(S_WORD MP, string &strData)
{
	return false;
}
//
bool C_CmdrE080020C::ReadDataByMP(S_WORD MP, string &strData)
{
	return false;
}
//无功过补偿欠补偿告警发生阈值
bool C_CmdrE080020D::ReadDataByMP(S_WORD MP, string &strData)
{
	return false;
}
//无功过补偿欠补偿告警恢复阈值
bool C_CmdrE080020E::ReadDataByMP(S_WORD MP, string &strData)
{
	return false;
}


