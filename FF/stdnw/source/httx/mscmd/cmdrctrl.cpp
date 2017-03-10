/** @file
 *  @brief ��̨ͨѶ�����д��������ʵ��
 *  @author  zhongguangxiong
 *  @date 2012/02/18
 *  @version 0.1
 */
#include "cmdrctrl.h"
#include "status.h"

//ң����բʱ��
bool C_CmdrE0000C40::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strFKCtrlBaseInfo;
	AppendDADT(strFKCtrlBaseInfo, CmdObj);

	S_FKCtrlBaseInfo *pFKCtrlBaseInfo = STATUS.GetFKCtrlBaseInfo();
	S_BYTE hh = kge_cbcd(pFKCtrlBaseInfo->m_SwitchDelayTime/60);
	S_BYTE mm = kge_cbcd(pFKCtrlBaseInfo->m_SwitchDelayTime%60);
	strFKCtrlBaseInfo.push_back(mm);
	strFKCtrlBaseInfo.push_back(hh);

	strResponse.push_back(strFKCtrlBaseInfo);
	return true;
}
//����״̬��01����״̬��00�Ǳ���״̬
bool C_CmdrE0000C50::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strFKCtrlBaseInfo;
	S_FKCtrlBaseInfo *pFKCtrlBaseInfo = STATUS.GetFKCtrlBaseInfo();

	AppendDADT(strFKCtrlBaseInfo, CmdObj);
	strFKCtrlBaseInfo.push_back(pFKCtrlBaseInfo->m_BDFlag);

	strResponse.push_back(strFKCtrlBaseInfo);
	return true;
}
//���ʿ����Ƿ���Ч
bool C_CmdrE0000C60::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPWRCtrlInfo;
	S_PWRCtrlInfo *pPWRCtrlInfo = STATUS.GetPWRCtrlInfo();

	AppendDADT(strPWRCtrlInfo, CmdObj);
	strPWRCtrlInfo.push_back(pPWRCtrlInfo->m_Enable);

	strResponse.push_back(strPWRCtrlInfo);
	return true;
}
//�¶ȵ��������Ƿ���Ч
bool C_CmdrE0000C61::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strDLCtrlInfo;
	S_DLCtrlInfo *pDLCtrlInfo = STATUS.GetDLCtrlInfo();

	AppendDADT(strDLCtrlInfo, CmdObj);
	strDLCtrlInfo.push_back(pDLCtrlInfo->m_MonthEnergyCtrlParam.m_Enable);

	strResponse.push_back(strDLCtrlInfo);
	return true;
}
//�����������Ƿ���Ч
bool C_CmdrE0000C62::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strDLCtrlInfo;
	S_DLCtrlInfo *pDLCtrlInfo = STATUS.GetDLCtrlInfo();

	AppendDADT(strDLCtrlInfo, CmdObj);
	strDLCtrlInfo.push_back(pDLCtrlInfo->m_BuyEnergyCtrlParam.m_Enable);

	strResponse.push_back(strDLCtrlInfo);
	return true;
}
//���ʿ��Ƹ��ִ���բ�Ƿ���Ч
bool C_CmdrE0000C63::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPWRCtrlInfo;
	S_PWRCtrlInfo *pPWRCtrlInfo = STATUS.GetPWRCtrlInfo();

	AppendDADT(strPWRCtrlInfo, CmdObj);
	strPWRCtrlInfo.push_back(*pPWRCtrlInfo->m_TurnEnable.GetData());

	strResponse.push_back(strPWRCtrlInfo);
	return true;
}
//�µ���������բ�ִ���Ч��־
bool C_CmdrE0000C64::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strDLCtrlInfo;
	S_DLCtrlInfo *pDLCtrlInfo = STATUS.GetDLCtrlInfo();

	AppendDADT(strDLCtrlInfo, CmdObj);
	strDLCtrlInfo.push_back(*pDLCtrlInfo->m_MonthEnergyCtrlParam.m_TurnEnable.GetData());

	strResponse.push_back(strDLCtrlInfo);
	return true;
}
//������������բ�ִ���Ч��־
bool C_CmdrE0000C65::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strDLCtrlInfo;
	S_DLCtrlInfo *pDLCtrlInfo = STATUS.GetDLCtrlInfo();

	AppendDADT(strDLCtrlInfo, CmdObj);
	strDLCtrlInfo.push_back(*pDLCtrlInfo->m_BuyEnergyCtrlParam.m_TurnEnable.GetData());

	strResponse.push_back(strDLCtrlInfo);
	return true;
}
//�����趨ֵ��բ�Ĺ����ɳ���ʱ��
bool C_CmdrE0000C66::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strFKCtrlBaseInfo;
	AppendDADT(strFKCtrlBaseInfo, CmdObj);

	S_FKCtrlBaseInfo *pFKCtrlBaseInfo = STATUS.GetFKCtrlBaseInfo();
	S_BYTE hh = kge_cbcd((S_BYTE)(pFKCtrlBaseInfo->m_TZOverLoadTimeSec/3600));
	S_BYTE mm = kge_cbcd((S_BYTE)(pFKCtrlBaseInfo->m_TZOverLoadTimeSec%3600/60));
	S_BYTE ss = kge_cbcd((S_BYTE)(pFKCtrlBaseInfo->m_TZOverLoadTimeSec%60));
	strFKCtrlBaseInfo.push_back(ss);
	strFKCtrlBaseInfo.push_back(mm);
	strFKCtrlBaseInfo.push_back(hh);

	strResponse.push_back(strFKCtrlBaseInfo);
	return true;
}
//�����趨ֵ��բ�ָ��ĸ��ɻָ�����ʱ�䣬ȱʡ10����
bool C_CmdrE0000C67::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strFKCtrlBaseInfo;
	AppendDADT(strFKCtrlBaseInfo, CmdObj);

	S_FKCtrlBaseInfo *pFKCtrlBaseInfo = STATUS.GetFKCtrlBaseInfo();
	S_BYTE hh = kge_cbcd((S_BYTE)(pFKCtrlBaseInfo->m_TZUnderLoadTimeSec/3600));
	S_BYTE mm = kge_cbcd((S_BYTE)(pFKCtrlBaseInfo->m_TZUnderLoadTimeSec%3600/60));
	S_BYTE ss = kge_cbcd((S_BYTE)(pFKCtrlBaseInfo->m_TZUnderLoadTimeSec%60));
	strFKCtrlBaseInfo.push_back(ss);
	strFKCtrlBaseInfo.push_back(mm);
	strFKCtrlBaseInfo.push_back(hh);

	strResponse.push_back(strFKCtrlBaseInfo);
	return true;
}
//��բ����澯��ʱ��ȱʡ10����
bool C_CmdrE0000C68::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strFKCtrlBaseInfo;
	AppendDADT(strFKCtrlBaseInfo, CmdObj);

	S_FKCtrlBaseInfo *pFKCtrlBaseInfo = STATUS.GetFKCtrlBaseInfo();
	S_BYTE mm = kge_cbcd(pFKCtrlBaseInfo->m_TZAlarmDelaySec/60);
	strFKCtrlBaseInfo.push_back(mm);

	strResponse.push_back(strFKCtrlBaseInfo);
	return true;
}
//���ʿ����ù����ۼӱ�־
bool C_CmdrE0000C69::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPWRCtrlInfo;
	AppendDADT(strPWRCtrlInfo, CmdObj);

	S_PWRCtrlInfo *pPWRCtrlInfo = STATUS.GetPWRCtrlInfo();
	strPWRCtrlInfo.push_back(*pPWRCtrlInfo->m_struPwrSum.m_SumFlag.GetData());
	strPWRCtrlInfo.append((char *)pPWRCtrlInfo->m_struPwrSum.m_cTN, 16);

	strResponse.push_back(strPWRCtrlInfo);
	return true;
}
//���������õ����ۼӱ�־
bool C_CmdrE0000C6A::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strDLCtrlInfo;
	AppendDADT(strDLCtrlInfo, CmdObj);

	S_DLCtrlInfo *pDLCtrlInfo = STATUS.GetDLCtrlInfo();
	strDLCtrlInfo.push_back(*pDLCtrlInfo->m_struEnergySum.m_SumFlag.GetData());
	strDLCtrlInfo.append((char *)pDLCtrlInfo->m_struEnergySum.m_cTN, 16);

	strResponse.push_back(strDLCtrlInfo);
	return true;
}
//���ʶ�ֵ����ϵ��������ȱʡΪ0
bool C_CmdrE0000C6B::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPWRCtrlInfo;
	S_PWRCtrlInfo *pPWRCtrlInfo = STATUS.GetPWRCtrlInfo();

	AppendDADT(strPWRCtrlInfo, CmdObj);
	strPWRCtrlInfo.push_back(pPWRCtrlInfo->m_PwrCoefficient);

	strResponse.push_back(strPWRCtrlInfo);
	return true;
}
//�µ����ض�ֵ����ϵ��������ȱʡΪ00
bool C_CmdrE0000C6C::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strDLCtrlInfo;
	S_DLCtrlInfo *pDLCtrlInfo = STATUS.GetDLCtrlInfo();

	AppendDADT(strDLCtrlInfo, CmdObj);
	strDLCtrlInfo.push_back(pDLCtrlInfo->m_MonthEnergyCtrlParam.m_EnergyCoefficient);

	strResponse.push_back(strDLCtrlInfo);
	return true;
}
//��͸���
bool C_CmdrE0000C6D::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strFKCtrlBaseInfo;
	S_FKCtrlBaseInfo *pFKCtrlBaseInfo = STATUS.GetFKCtrlBaseInfo();

	AppendDADT(strFKCtrlBaseInfo, CmdObj);
	strFKCtrlBaseInfo.append((char *)&pFKCtrlBaseInfo->m_MinPower, 4);

	strResponse.push_back(strFKCtrlBaseInfo);
	return true;
}
//X��ʱ�޵����Ч��ʶ
bool C_CmdrE0000C70::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPWRCtrlInfo;
	S_PWRCtrlInfo *pPWRCtrlInfo = STATUS.GetPWRCtrlInfo();

	AppendDADT(strPWRCtrlInfo, CmdObj);
	strPWRCtrlInfo.push_back(pPWRCtrlInfo->m_TPBCtrlParam.m_Enable);

	strResponse.push_back(strPWRCtrlInfo);
	return true;
}
//��ʱ�޵�ļ��㻬��ʱ�䣬��λ����
bool C_CmdrE0000C71::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPWRCtrlInfo;
	S_PWRCtrlInfo *pPWRCtrlInfo = STATUS.GetPWRCtrlInfo();

	AppendDADT(strPWRCtrlInfo, CmdObj);
	strPWRCtrlInfo.push_back(pPWRCtrlInfo->m_TPBCtrlParam.m_WindowCount);

	strResponse.push_back(strPWRCtrlInfo);
	return true;
}
//���ʿ���ϵ��1��ʱ����
bool C_CmdrE0000D00::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPWRCtrlInfo;
	S_PWRCtrlInfo *pPWRCtrlInfo = STATUS.GetPWRCtrlInfo();

	AppendDADT(strPWRCtrlInfo, CmdObj);
	strPWRCtrlInfo.push_back(pPWRCtrlInfo->m_TimeCtrlParam[0].m_SDMaxNum);

	strResponse.push_back(strPWRCtrlInfo);
	return true;
}
//���ʿ���ϵ��1,ִ��ʱ��
bool C_CmdrE0000D09::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPWRCtrlInfo;
	S_PWRCtrlInfo *pPWRCtrlInfo = STATUS.GetPWRCtrlInfo();

	AppendDADT(strPWRCtrlInfo, CmdObj);
	strPWRCtrlInfo.push_back(pPWRCtrlInfo->m_TimeCtrlParam[0].m_ExeTime.m_StartDate.m_A0);
	strPWRCtrlInfo.push_back(pPWRCtrlInfo->m_TimeCtrlParam[0].m_ExeTime.m_StartDate.m_A1);
	strPWRCtrlInfo.push_back(pPWRCtrlInfo->m_TimeCtrlParam[0].m_ExeTime.m_EndDate.m_A0);
	strPWRCtrlInfo.push_back(pPWRCtrlInfo->m_TimeCtrlParam[0].m_ExeTime.m_EndDate.m_A1);
	strPWRCtrlInfo.push_back(pPWRCtrlInfo->m_TimeCtrlParam[0].m_ExeTime.m_TI);

	char buf[4]; /// ???
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<8;j++)
		{
			if(pPWRCtrlInfo->m_TimeCtrlParam[0].m_ExeTime.m_N.test(8*i+j))
				buf[i] |=(1<<j);
		}
	}

	strResponse.push_back(strPWRCtrlInfo);
	return true;
}
//���ʿ���ϵ��1�������������
bool C_CmdrE0000D0F::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
bool C_CmdrE0000D0F::SetAllParam(const string &strAllParam)
{
	return false;
}
//���ʿ��ƹ��ʶ�ֵϵ��2��ʱ����
bool C_CmdrE0000D10::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPWRCtrlInfo;
	S_PWRCtrlInfo *pPWRCtrlInfo = STATUS.GetPWRCtrlInfo();

	AppendDADT(strPWRCtrlInfo, CmdObj);
	strPWRCtrlInfo.push_back(pPWRCtrlInfo->m_TimeCtrlParam[1].m_SDMaxNum);

	strResponse.push_back(strPWRCtrlInfo);
	return true;
}
//���ʿ���ϵ��2,ִ��ʱ��
bool C_CmdrE0000D19::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPWRCtrlInfo;
	S_PWRCtrlInfo *pPWRCtrlInfo = STATUS.GetPWRCtrlInfo();

	AppendDADT(strPWRCtrlInfo, CmdObj);
	strPWRCtrlInfo.push_back(pPWRCtrlInfo->m_TimeCtrlParam[1].m_ExeTime.m_StartDate.m_A0);
	strPWRCtrlInfo.push_back(pPWRCtrlInfo->m_TimeCtrlParam[1].m_ExeTime.m_StartDate.m_A1);
	strPWRCtrlInfo.push_back(pPWRCtrlInfo->m_TimeCtrlParam[1].m_ExeTime.m_EndDate.m_A0);
	strPWRCtrlInfo.push_back(pPWRCtrlInfo->m_TimeCtrlParam[1].m_ExeTime.m_EndDate.m_A1);
	strPWRCtrlInfo.push_back(pPWRCtrlInfo->m_TimeCtrlParam[1].m_ExeTime.m_TI);

	char buf[4];
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<8;j++)
		{
			if(pPWRCtrlInfo->m_TimeCtrlParam[1].m_ExeTime.m_N.test(8*i+j))
				buf[i] |=(1<<j);
		}
	}

	strResponse.push_back(strPWRCtrlInfo);
	return true;
}
//���ʿ���ϵ��2�������������
bool C_CmdrE0000D1F::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
bool C_CmdrE0000D1F::SetAllParam(const string &strAllParam)
{
	return false;
}
//���ʿ��ƹ��ʶ�ֵϵ��3��ʱ����
bool C_CmdrE0000D20::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPWRCtrlInfo;
	S_PWRCtrlInfo *pPWRCtrlInfo = STATUS.GetPWRCtrlInfo();

	AppendDADT(strPWRCtrlInfo, CmdObj);
	strPWRCtrlInfo.push_back(pPWRCtrlInfo->m_TimeCtrlParam[2].m_SDMaxNum);

	strResponse.push_back(strPWRCtrlInfo);
	return true;
}
//���ʿ���ϵ��3,ִ��ʱ��
bool C_CmdrE0000D29::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPWRCtrlInfo;
	S_PWRCtrlInfo *pPWRCtrlInfo = STATUS.GetPWRCtrlInfo();

	AppendDADT(strPWRCtrlInfo, CmdObj);
	strPWRCtrlInfo.push_back(pPWRCtrlInfo->m_TimeCtrlParam[2].m_ExeTime.m_StartDate.m_A0);
	strPWRCtrlInfo.push_back(pPWRCtrlInfo->m_TimeCtrlParam[2].m_ExeTime.m_StartDate.m_A1);
	strPWRCtrlInfo.push_back(pPWRCtrlInfo->m_TimeCtrlParam[2].m_ExeTime.m_EndDate.m_A0);
	strPWRCtrlInfo.push_back(pPWRCtrlInfo->m_TimeCtrlParam[2].m_ExeTime.m_EndDate.m_A1);
	strPWRCtrlInfo.push_back(pPWRCtrlInfo->m_TimeCtrlParam[2].m_ExeTime.m_TI);

	char buf[4];
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<8;j++)
		{
			if(pPWRCtrlInfo->m_TimeCtrlParam[2].m_ExeTime.m_N.test(8*i+j))
				buf[i] |=(1<<j);
		}
	}

	strResponse.push_back(strPWRCtrlInfo);
	return true;
}
//���ʿ���ϵ��3�������������
bool C_CmdrE0000D2F::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
bool C_CmdrE0000D2F::SetAllParam(const string &strAllParam)
{
	return false;
}
//���ʿ��ƹ��ʶ�ֵϵ��4��ʱ����
bool C_CmdrE0000D30::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPWRCtrlInfo;
	S_PWRCtrlInfo *pPWRCtrlInfo = STATUS.GetPWRCtrlInfo();

	AppendDADT(strPWRCtrlInfo, CmdObj);
	strPWRCtrlInfo.push_back(pPWRCtrlInfo->m_TimeCtrlParam[3].m_SDMaxNum);

	strResponse.push_back(strPWRCtrlInfo);
	return true;
}
//���ʿ���ϵ��4,ִ��ʱ��
bool C_CmdrE0000D39::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPWRCtrlInfo;
	S_PWRCtrlInfo *pPWRCtrlInfo = STATUS.GetPWRCtrlInfo();

	AppendDADT(strPWRCtrlInfo, CmdObj);
	strPWRCtrlInfo.push_back(pPWRCtrlInfo->m_TimeCtrlParam[3].m_ExeTime.m_StartDate.m_A0);
	strPWRCtrlInfo.push_back(pPWRCtrlInfo->m_TimeCtrlParam[3].m_ExeTime.m_StartDate.m_A1);
	strPWRCtrlInfo.push_back(pPWRCtrlInfo->m_TimeCtrlParam[3].m_ExeTime.m_EndDate.m_A0);
	strPWRCtrlInfo.push_back(pPWRCtrlInfo->m_TimeCtrlParam[3].m_ExeTime.m_EndDate.m_A1);
	strPWRCtrlInfo.push_back(pPWRCtrlInfo->m_TimeCtrlParam[3].m_ExeTime.m_TI);

	char buf[4] = {0};
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<8;j++)
		{
			if(pPWRCtrlInfo->m_TimeCtrlParam[3].m_ExeTime.m_N.test(8*i+j))
				buf[i] |=(1<<j);
		}
	}

	strResponse.push_back(strPWRCtrlInfo);
	return true;
}
//���ʿ���ϵ��4�������������
bool C_CmdrE0000D3F::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
bool C_CmdrE0000D3F::SetAllParam(const string &strAllParam)
{
	return false;
}
//�µ������ƣ��µ����޶�
bool C_CmdrE0000E00::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strDLCtrlInfo;
	S_DLCtrlInfo *pDLCtrlInfo = STATUS.GetDLCtrlInfo();

	AppendDADT(strDLCtrlInfo, CmdObj);
	strDLCtrlInfo.append((char *)&pDLCtrlInfo->m_MonthEnergyCtrlParam.m_MonthEnergyLimit,4);

	strResponse.push_back(strDLCtrlInfo);
	return true;
}
//�µ������ƣ��ִε���
bool C_CmdrE0000E01::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strDLCtrlInfo;
	S_DLCtrlInfo *pDLCtrlInfo = STATUS.GetDLCtrlInfo();

	AppendDADT(strDLCtrlInfo, CmdObj);
	strDLCtrlInfo.append((char *)&pDLCtrlInfo->m_MonthEnergyCtrlParam.m_TurnEnergy,4);

	strResponse.push_back(strDLCtrlInfo);
	return true;
}
//���������ƣ����ι�����
bool C_CmdrE0000E02::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strDLCtrlInfo;
	S_DLCtrlInfo *pDLCtrlInfo = STATUS.GetDLCtrlInfo();

	AppendDADT(strDLCtrlInfo, CmdObj);
	strDLCtrlInfo.append((char *)&pDLCtrlInfo->m_BuyEnergyCtrlParam.m_NowBuyEnergy,4);
	strDLCtrlInfo.push_back(pDLCtrlInfo->m_BuyEnergyCtrlParam.m_NowBuyDLNo);

	strResponse.push_back(strDLCtrlInfo);
	return true;
}
//���������ƣ�ʣ�����
bool C_CmdrE0000E03::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strDLCtrlInfo;
	S_DLCtrlInfo *pDLCtrlInfo = STATUS.GetDLCtrlInfo();

	AppendDADT(strDLCtrlInfo, CmdObj);
	strDLCtrlInfo.push_back(pDLCtrlInfo->m_BuyEnergyCtrlParam.m_NowLeftDLSign);
	strDLCtrlInfo.append((char *)&pDLCtrlInfo->m_BuyEnergyCtrlParam.m_NowLeftEnergy,4);

	strResponse.push_back(strDLCtrlInfo);
	return true;
}
//
bool C_CmdrE0000E04::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}//
bool C_CmdrE0000E05::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}//
//���ݿ��Ƿ�Ͷ��
bool C_CmdrE0000E10::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPWRCtrlInfo;
	S_PWRCtrlInfo *pPWRCtrlInfo = STATUS.GetPWRCtrlInfo();

	AppendDADT(strPWRCtrlInfo, CmdObj);
	strPWRCtrlInfo.push_back(pPWRCtrlInfo->m_FactoryCtrlParam.m_Enable);

	strResponse.push_back(strPWRCtrlInfo);
	return true;
}
//���ݿز���
bool C_CmdrE0000E11::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	string strPWRCtrlInfo;
	S_PWRCtrlInfo *pPWRCtrlInfo = STATUS.GetPWRCtrlInfo();

	AppendDADT(strPWRCtrlInfo, CmdObj);
	strPWRCtrlInfo.push_back(pPWRCtrlInfo->m_FactoryCtrlParam.m_Sign);
	strPWRCtrlInfo.append((char *)&pPWRCtrlInfo->m_FactoryCtrlParam.m_CtrlLimitVar,4);
	strPWRCtrlInfo.push_back(pPWRCtrlInfo->m_FactoryCtrlParam.m_StartTime.m_A0);
	strPWRCtrlInfo.push_back(pPWRCtrlInfo->m_FactoryCtrlParam.m_StartTime.m_A1);
	strPWRCtrlInfo.append((char *)&pPWRCtrlInfo->m_FactoryCtrlParam.m_ContinueTime,2);
	strPWRCtrlInfo.push_back(*pPWRCtrlInfo->m_FactoryCtrlParam.m_CtrlDate.GetData());

	strResponse.push_back(strPWRCtrlInfo);
	return true;
}
//��������������
//���ز��� ���ؽӵ�����
bool C_CmdrE0000C00::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//���ز��� �������״̬
bool C_CmdrE0000C10::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//���ز��� ��������ִ�
bool C_CmdrE0000C30::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//���ʿ���ϵ��1��ʱ��(1-8)
bool C_CmdrE0000D01::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//���ʿ���ϵ��2��ʱ��(1-8)
bool C_CmdrE0000D11::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//���ʿ���ϵ��3��ʱ��(1-8)
bool C_CmdrE0000D21::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}
//���ʿ���ϵ��4��ʱ��(1-8)
bool C_CmdrE0000D31::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	return false;
}


