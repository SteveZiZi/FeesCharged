/** @file
 *  @brief 后台通讯命令负控写参数对象实现
 *  @author  zhongguangxiong
 *  @date 2012/02/18
 *  @version 0.1
 */
#include "cmdwctrl.h"
#include "status.h"


//遥控拉闸时间
bool C_CmdwE0000C40::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(2,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000C40::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 2)
		return false;

	S_FKCtrlBaseInfo *pFKCtrlBaseInfo = STATUS.GetFKCtrlBaseInfo();
	pFKCtrlBaseInfo->m_SwitchDelayTime = kge_bcdc(CmdObj.m_DiInfo[1])*60+kge_bcdc(CmdObj.m_DiInfo[0]);//单位暂时为分钟

	STATUS.SaveFKCtrlBaseInfo();
	return true;
}
//保电状态：01保电状态，00非保电状态
bool C_CmdwE0000C50::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000C50::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 1)
		return false;

	S_FKCtrlBaseInfo *pFKCtrlBaseInfo = STATUS.GetFKCtrlBaseInfo();
	pFKCtrlBaseInfo->m_BDFlag = (CmdObj.m_DiInfo[0]==0)? false:true;

	STATUS.SaveFKCtrlBaseInfo();
	return true;
}
//功率控制是否有效
bool C_CmdwE0000C60::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000C60::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 1)
		return false;

	S_PWRCtrlInfo *pPWRCtrlInfo = STATUS.GetPWRCtrlInfo();
	pPWRCtrlInfo->m_Enable = CmdObj.m_DiInfo[0];

	STATUS.SavePWRCtrlInfo();
	return true;
}
//月度电量控制是否有效
bool C_CmdwE0000C61::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000C61::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 1)
		return false;

	S_DLCtrlInfo *pDLCtrlInfo = STATUS.GetDLCtrlInfo();
	pDLCtrlInfo->m_MonthEnergyCtrlParam.m_Enable = CmdObj.m_DiInfo[0];

	STATUS.SaveDLCtrlInfo();
	return true;
}
//购电量控制是否有效
bool C_CmdwE0000C62::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000C62::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 1)
		return false;

	S_DLCtrlInfo *pDLCtrlInfo = STATUS.GetDLCtrlInfo();
	pDLCtrlInfo->m_BuyEnergyCtrlParam.m_Enable = CmdObj.m_DiInfo[0];

	STATUS.SaveDLCtrlInfo();
	return true;
}
//功率控制各轮次跳闸是否有效
bool C_CmdwE0000C63::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000C63::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 1)
		return false;

	S_PWRCtrlInfo *pPWRCtrlInfo = STATUS.GetPWRCtrlInfo();
	pPWRCtrlInfo->m_TurnEnable.SetBit(&CmdObj.m_DiInfo[0],1);

	STATUS.SavePWRCtrlInfo();
	return true;
}
//月电量控制跳闸轮次有效标志
bool C_CmdwE0000C64::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000C64::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 1)
		return false;

	S_DLCtrlInfo *pDLCtrlInfo = STATUS.GetDLCtrlInfo();
	pDLCtrlInfo->m_MonthEnergyCtrlParam.m_TurnEnable.SetBit(&CmdObj.m_DiInfo[0],1);

	STATUS.SavePWRCtrlInfo();
	return true;
}


//购电量控制跳闸轮次有效标志
bool C_CmdwE0000C65::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000C65::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 1)
		return false;

	S_DLCtrlInfo *pDLCtrlInfo = STATUS.GetDLCtrlInfo();
	pDLCtrlInfo->m_BuyEnergyCtrlParam.m_TurnEnable.SetBit(&CmdObj.m_DiInfo[0],1);

	STATUS.SavePWRCtrlInfo();
	return true;
}
//超过设定值跳闸的过负荷持续时间
bool C_CmdwE0000C66::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(3,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000C66::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 3)
		return false;

	S_FKCtrlBaseInfo *pFKCtrlBaseInfo = STATUS.GetFKCtrlBaseInfo();
	pFKCtrlBaseInfo->m_TZOverLoadTimeSec = kge_bcdc(CmdObj.m_DiInfo[2])*3600+kge_bcdc(CmdObj.m_DiInfo[1])*60+kge_bcdc(CmdObj.m_DiInfo[0]);//单位暂为秒

	STATUS.SaveFKCtrlBaseInfo();
	return true;
}
//超过设定值跳闸恢复的负荷恢复持续时间，缺省10分钟
bool C_CmdwE0000C67::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(3,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000C67::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 3)
		return false;

	S_FKCtrlBaseInfo *pFKCtrlBaseInfo = STATUS.GetFKCtrlBaseInfo();
	pFKCtrlBaseInfo->m_TZUnderLoadTimeSec = kge_bcdc(CmdObj.m_DiInfo[2])*3600+kge_bcdc(CmdObj.m_DiInfo[1])*60+kge_bcdc(CmdObj.m_DiInfo[0]);//单位暂为秒

	STATUS.SaveFKCtrlBaseInfo();
	return true;
}
//拉闸命令告警延时，缺省10分钟
bool C_CmdwE0000C68::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000C68::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 1)
		return false;

	S_FKCtrlBaseInfo *pFKCtrlBaseInfo = STATUS.GetFKCtrlBaseInfo();
	pFKCtrlBaseInfo->m_TZAlarmDelaySec = kge_bcdc(CmdObj.m_DiInfo[0]) * 60;//单位为分

	STATUS.SaveFKCtrlBaseInfo();
	return true;
}
//功率控制用功率累加标志
bool C_CmdwE0000C69::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(17,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000C69::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 17)
		return false;

	S_PWRCtrlInfo *pPWRCtrlInfo = STATUS.GetPWRCtrlInfo();
	pPWRCtrlInfo->m_struPwrSum.m_SumFlag.SetBit(&CmdObj.m_DiInfo[0],1);
	for(int i=0;i<8;i++)
	{
		memcpy(&pPWRCtrlInfo->m_struPwrSum.m_cTN[i],&CmdObj.m_DiInfo[2*i+1],2);
	}

	STATUS.SavePWRCtrlInfo();
	return true;
}
//电量控制用电量累加标志
bool C_CmdwE0000C6A::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(17,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000C6A::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 17)
		return false;

	S_DLCtrlInfo *pDLCtrlInfo = STATUS.GetDLCtrlInfo();
	pDLCtrlInfo->m_struEnergySum.m_SumFlag.SetBit(&CmdObj.m_DiInfo[0],1);
	for(int i=0;i<8;i++)
	{
		memcpy(&pDLCtrlInfo->m_struEnergySum.m_cTN[i],&CmdObj.m_DiInfo[2*i+1],2);
	}

	STATUS.SaveDLCtrlInfo();
	return true;
}
//功率定值浮动系数（％）缺省为0
bool C_CmdwE0000C6B::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000C6B::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 1)
		return false;

	S_PWRCtrlInfo *pPWRCtrlInfo = STATUS.GetPWRCtrlInfo();
	pPWRCtrlInfo->m_PwrCoefficient = CmdObj.m_DiInfo[0];

	STATUS.SavePWRCtrlInfo();
	return true;
}
//月电量控定值浮动系数（％）缺省为00
bool C_CmdwE0000C6C::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000C6C::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 1)
		return false;

	S_DLCtrlInfo *pDLCtrlInfo = STATUS.GetDLCtrlInfo();
	pDLCtrlInfo->m_MonthEnergyCtrlParam.m_EnergyCoefficient = CmdObj.m_DiInfo[0];

	STATUS.SaveDLCtrlInfo();
	return true;
}
//最低负荷
bool C_CmdwE0000C6D::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(4,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000C6D::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 4)
		return false;

	S_FKCtrlBaseInfo *pFKCtrlBaseInfo = STATUS.GetFKCtrlBaseInfo();

	C_BCD bcdMinPower(CmdObj.m_DiInfo);
	const S_BYTE *pBcd = bcdMinPower.GetBcdData();
	memcpy(&pFKCtrlBaseInfo->m_MinPower, pBcd+1, 4); 

	STATUS.SaveFKCtrlBaseInfo();
	return true;
}
//X临时限电控有效标识
bool C_CmdwE0000C70::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(4,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000C70::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 1)
		return false;

	S_PWRCtrlInfo *pPWRCtrlInfo = STATUS.GetPWRCtrlInfo();
	pPWRCtrlInfo->m_TPBCtrlParam.m_Enable = CmdObj.m_DiInfo[0];

	STATUS.SavePWRCtrlInfo();
	return true;
}
//临时限电的计算滑窗时间，单位分钟
bool C_CmdwE0000C71::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000C71::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 1)
		return false;

	S_PWRCtrlInfo *pPWRCtrlInfo = STATUS.GetPWRCtrlInfo();
	pPWRCtrlInfo->m_TPBCtrlParam.m_WindowCount = CmdObj.m_DiInfo[0];//单位分钟

	STATUS.SavePWRCtrlInfo();
	return true;
}
//功率控制系列1，时段数
bool C_CmdwE0000D00::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000D00::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 1)
		return false;

	S_PWRCtrlInfo *pPWRCtrlInfo = STATUS.GetPWRCtrlInfo();
	pPWRCtrlInfo->m_TimeCtrlParam[0].m_SDMaxNum = CmdObj.m_DiInfo[0];//单位分钟

	STATUS.SavePWRCtrlInfo();
	return true;
}
//功率控制系列1,执行时间
bool C_CmdwE0000D09::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(9,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000D09::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 9)
		return false;

	bool bTemp;
	S_PWRCtrlInfo *pPWRCtrlInfo = STATUS.GetPWRCtrlInfo();
	pPWRCtrlInfo->m_TimeCtrlParam[0].m_ExeTime.m_StartDate.m_A0 = CmdObj.m_DiInfo[0];//开始月
	pPWRCtrlInfo->m_TimeCtrlParam[0].m_ExeTime.m_StartDate.m_A1 = CmdObj.m_DiInfo[1];//开始日
	pPWRCtrlInfo->m_TimeCtrlParam[0].m_ExeTime.m_EndDate.m_A0 = CmdObj.m_DiInfo[2];//结束月
	pPWRCtrlInfo->m_TimeCtrlParam[0].m_ExeTime.m_EndDate.m_A1 = CmdObj.m_DiInfo[3];//结束日
	pPWRCtrlInfo->m_TimeCtrlParam[0].m_ExeTime.m_TI = CmdObj.m_DiInfo[4];//频率
	for(int i =0;i<4;i++)
	{
		for(int j=0;j<8;j++)
		{
			((CmdObj.m_DiInfo[5+i] & (1<<j))?bTemp = true:bTemp = false);
			pPWRCtrlInfo->m_TimeCtrlParam[0].m_ExeTime.m_N.set(8*i+j,bTemp);
		}
	}

	STATUS.SavePWRCtrlInfo();
	return true;
}
//功率控制系列1，以上数据项集合
bool C_CmdwE0000D0F::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(17,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000D0F::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
bool C_CmdwE0000D0F::SetAllParam(const string &strAllParam)
{
	return false;
}
//功率控制功率定值系列2，时段数
bool C_CmdwE0000D10::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000D10::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 1)
		return false;

	S_PWRCtrlInfo *pPWRCtrlInfo = STATUS.GetPWRCtrlInfo();
	pPWRCtrlInfo->m_TimeCtrlParam[1].m_SDMaxNum = CmdObj.m_DiInfo[0];//单位分钟

	STATUS.SavePWRCtrlInfo();
	return true;
}
//功率控制系列2,执行时间
bool C_CmdwE0000D19::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(9,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000D19::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 9)
		return false;

	bool bTemp;
	S_PWRCtrlInfo *pPWRCtrlInfo = STATUS.GetPWRCtrlInfo();
	pPWRCtrlInfo->m_TimeCtrlParam[1].m_ExeTime.m_StartDate.m_A0 = CmdObj.m_DiInfo[0];//开始月
	pPWRCtrlInfo->m_TimeCtrlParam[1].m_ExeTime.m_StartDate.m_A1 = CmdObj.m_DiInfo[1];//开始日
	pPWRCtrlInfo->m_TimeCtrlParam[1].m_ExeTime.m_EndDate.m_A0 = CmdObj.m_DiInfo[2];//结束月
	pPWRCtrlInfo->m_TimeCtrlParam[1].m_ExeTime.m_EndDate.m_A1 = CmdObj.m_DiInfo[3];//结束日
	pPWRCtrlInfo->m_TimeCtrlParam[1].m_ExeTime.m_TI = CmdObj.m_DiInfo[4];//频率
	for(int i =0;i<4;i++)
	{
		for(int j=0;j<8;j++)
		{
			((CmdObj.m_DiInfo[5+i] & (1<<j))?bTemp = true:bTemp = false);
			pPWRCtrlInfo->m_TimeCtrlParam[1].m_ExeTime.m_N.set(8*i+j,bTemp);
		}
	}

	STATUS.SavePWRCtrlInfo();
	return true;
}
//功率控制系列2，以上数据项集合
bool C_CmdwE0000D1F::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(17,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000D1F::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
bool C_CmdwE0000D1F::SetAllParam(const string &strAllParam)
{
	return false;
}
//功率控制功率定值系列3，时段数
bool C_CmdwE0000D20::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000D20::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 1)
		return false;

	S_PWRCtrlInfo *pPWRCtrlInfo = STATUS.GetPWRCtrlInfo();
	pPWRCtrlInfo->m_TimeCtrlParam[2].m_SDMaxNum = CmdObj.m_DiInfo[0];//单位分钟

	STATUS.SavePWRCtrlInfo();
	return true;
}
//功率控制系列3,执行时间
bool C_CmdwE0000D29::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(9,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000D29::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 9)
		return false;

	bool bTemp;
	S_PWRCtrlInfo *pPWRCtrlInfo = STATUS.GetPWRCtrlInfo();
	pPWRCtrlInfo->m_TimeCtrlParam[2].m_ExeTime.m_StartDate.m_A0 = CmdObj.m_DiInfo[0];//开始月
	pPWRCtrlInfo->m_TimeCtrlParam[2].m_ExeTime.m_StartDate.m_A1 = CmdObj.m_DiInfo[1];//开始日
	pPWRCtrlInfo->m_TimeCtrlParam[2].m_ExeTime.m_EndDate.m_A0 = CmdObj.m_DiInfo[2];//结束月
	pPWRCtrlInfo->m_TimeCtrlParam[2].m_ExeTime.m_EndDate.m_A1 = CmdObj.m_DiInfo[3];//结束日
	pPWRCtrlInfo->m_TimeCtrlParam[2].m_ExeTime.m_TI = CmdObj.m_DiInfo[4];//频率
	for(int i =0;i<4;i++)
	{
		for(int j=0;j<8;j++)
		{
			((CmdObj.m_DiInfo[5+i] & (1<<j))?bTemp = true:bTemp = false);
			pPWRCtrlInfo->m_TimeCtrlParam[2].m_ExeTime.m_N.set(8*i+j,bTemp);
		}
	}

	STATUS.SavePWRCtrlInfo();
	return true;
}
//功率控制系列3，以上数据项集合
bool C_CmdwE0000D2F::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(17,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000D2F::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
bool C_CmdwE0000D2F::SetAllParam(const string &strAllParam)
{
	return false;
}
//功率控制功率定值系列4，时段数
bool C_CmdwE0000D30::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000D30::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 1)
		return false;

	S_PWRCtrlInfo *pPWRCtrlInfo = STATUS.GetPWRCtrlInfo();
	pPWRCtrlInfo->m_TimeCtrlParam[3].m_SDMaxNum = CmdObj.m_DiInfo[0];//单位分钟

	STATUS.SavePWRCtrlInfo();
	return true;
}
//功率控制系列4,执行时间
bool C_CmdwE0000D39::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(9,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000D39::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 9)
		return false;

	bool bTemp;
	S_PWRCtrlInfo *pPWRCtrlInfo = STATUS.GetPWRCtrlInfo();
	pPWRCtrlInfo->m_TimeCtrlParam[3].m_ExeTime.m_StartDate.m_A0 = CmdObj.m_DiInfo[0];//开始月
	pPWRCtrlInfo->m_TimeCtrlParam[3].m_ExeTime.m_StartDate.m_A1 = CmdObj.m_DiInfo[1];//开始日
	pPWRCtrlInfo->m_TimeCtrlParam[3].m_ExeTime.m_EndDate.m_A0 = CmdObj.m_DiInfo[2];//结束月
	pPWRCtrlInfo->m_TimeCtrlParam[3].m_ExeTime.m_EndDate.m_A1 = CmdObj.m_DiInfo[3];//结束日
	pPWRCtrlInfo->m_TimeCtrlParam[3].m_ExeTime.m_TI = CmdObj.m_DiInfo[4];//频率
	for(int i =0;i<4;i++)
	{
		for(int j=0;j<8;j++)
		{
			((CmdObj.m_DiInfo[5+i] & (1<<j))?bTemp = true:bTemp = false);
			pPWRCtrlInfo->m_TimeCtrlParam[3].m_ExeTime.m_N.set(8*i+j,bTemp);
		}
	}

	STATUS.SavePWRCtrlInfo();
	return true;
}
//功率控制系列4，以上数据项集合
bool C_CmdwE0000D3F::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(17,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000D3F::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
bool C_CmdwE0000D3F::SetAllParam(const string &strAllParam)
{
	return false;
}
//月电量控制，月电量限额
bool C_CmdwE0000E00::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(4,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000E00::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 4)
		return false;

	S_DLCtrlInfo *pDLCtrlInfo = STATUS.GetDLCtrlInfo();
	C_BCD bcdMonthEnergyLimit(CmdObj.m_DiInfo);//这里？？
	const S_BYTE *pBcd = bcdMonthEnergyLimit.GetBcdData();
	memcpy(&pDLCtrlInfo->m_MonthEnergyCtrlParam.m_MonthEnergyLimit, pBcd+1, 4); 

	STATUS.SaveDLCtrlInfo();
	return true;
}
//月电量控制，轮次电量
bool C_CmdwE0000E01::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(4,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000E01::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 4)
		return false;

	S_DLCtrlInfo *pDLCtrlInfo = STATUS.GetDLCtrlInfo();
	C_BCD bcdTurnEnergy(CmdObj.m_DiInfo);//这里？？
	const S_BYTE *pBcd = bcdTurnEnergy.GetBcdData();
	memcpy(&pDLCtrlInfo->m_MonthEnergyCtrlParam.m_TurnEnergy, pBcd+1, 4); 

	STATUS.SaveDLCtrlInfo();
	return true;
}
//购电量控制，本次购电量
bool C_CmdwE0000E02::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(5,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000E02::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 4)
		return false;

	char buf[4];
	S_DLCtrlInfo *pDLCtrlInfo = STATUS.GetDLCtrlInfo();
	memcpy(buf,&CmdObj.m_DiInfo[0],4);
	C_BCD bcdNowBuyEnergy((const S_BYTE *)buf,4,2);//这里？？
	const S_BYTE *pBcd = bcdNowBuyEnergy.GetBcdData();
	memcpy(&pDLCtrlInfo->m_BuyEnergyCtrlParam.m_NowBuyEnergy, pBcd+1, 4); 
	pDLCtrlInfo->m_BuyEnergyCtrlParam.m_NowBuyDLNo = CmdObj.m_DiInfo[4];

	STATUS.SaveDLCtrlInfo();
	return true;
}
//购电量控制，剩余电量
bool C_CmdwE0000E03::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(5,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000E03::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//
bool C_CmdwE0000E04::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(49,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000E04::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//
bool C_CmdwE0000E05::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000E05::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}

//厂休控是否投入
bool C_CmdwE0000E10::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000E10::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 1)
		return false;

	S_PWRCtrlInfo *pPWRCtrlInfo = STATUS.GetPWRCtrlInfo();
	pPWRCtrlInfo->m_FactoryCtrlParam.m_Enable = CmdObj.m_DiInfo[0];

	STATUS.SavePWRCtrlInfo();
	return true;
}
//厂休控参数
bool C_CmdwE0000E11::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(10,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000E11::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 10)
		return false;

	S_PWRCtrlInfo *pPWRCtrlInfo = STATUS.GetPWRCtrlInfo();
	pPWRCtrlInfo->m_FactoryCtrlParam.m_Sign = CmdObj.m_DiInfo[0];
	memcpy(&pPWRCtrlInfo->m_FactoryCtrlParam.m_CtrlLimitVar,&CmdObj.m_DiInfo[1],4);
	pPWRCtrlInfo->m_FactoryCtrlParam.m_StartTime.m_A0 = CmdObj.m_DiInfo[5];
	pPWRCtrlInfo->m_FactoryCtrlParam.m_StartTime.m_A1 = CmdObj.m_DiInfo[6];
	memcpy(&pPWRCtrlInfo->m_FactoryCtrlParam.m_ContinueTime,&CmdObj.m_DiInfo[7],2);
	pPWRCtrlInfo->m_FactoryCtrlParam.m_CtrlDate.SetBit(&CmdObj.m_DiInfo[9],1);

	STATUS.SavePWRCtrlInfo();
	return true;
}
//二个数据项区域
//负控参数 开关接点类型
bool C_CmdwE0000C00::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000C00::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//负控参数 输出开关状态
bool C_CmdwE0000C20::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000C20::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//负控参数 输出开关轮次
bool C_CmdwE0000C30::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(1,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000C30::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//功率控制系列1，时段(1-8)
bool C_CmdwE0000D01::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(7,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000D01::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//功率控制系列2，时段(1-8)
bool C_CmdwE0000D11::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(7,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000D11::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//功率控制系列3，时段(1-8)
bool C_CmdwE0000D21::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(7,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000D21::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//功率控制系列4，时段(1-8)
bool C_CmdwE0000D31::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	return ExtractCmdInfo(7,  iterBegin, iterEnd, CmdObj);
}
bool C_CmdwE0000D31::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}


