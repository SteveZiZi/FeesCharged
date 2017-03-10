/** @file
 *  @brief PPP���ŷ����ܶ���
 *  @author 
 *  @date 2011/11/30
 *  @version 0.1
 */
#include "ppp.h"
#include "sysipc.h"
#include "htrace.h"
#include "flowctrl.h"
#include "context.h"
#include "status.h"
#include "connfact.h"
#include "timeop.h"


int C_PppServer::BeforeService(void)
{
	if (C_PppBase::BeforeService() != 0)
		return -1;

	FLOWCTRL.SetFlowCtrlInfo(CONTEXT.GetFlowInfo());
	m_pPppInfo = STATUS.GetPppInfo();
	m_pPppStatus = CONTEXT.GetPppStatus();
	m_pPppPackage = &m_pPppStatus->m_PppPackage;
	m_pDialInfo = &m_pPppInfo->m_DialInfo;
	m_pPppStatus->m_ModemType = m_WirelessModemType;
	m_pPppStatus->m_WirelessNetType = 0;

	m_pModemConnect = NULL, m_pGprsConnect = NULL;
	m_pAtCmdConnect = NULL, m_pSmsConnect = NULL;

	S_CHANNEL *pModemCom = STATUS.GetPppChannel(PPP_CHANNL_MODEM);
	if (pModemCom != NULL)
		m_pModemConnect = C_ConnFact::CreateConnection(*pModemCom);
	S_CHANNEL *pGprsCom = STATUS.GetPppChannel(PPP_CHANNL_GPRS);
	if (pGprsCom != NULL)
		m_pGprsConnect = C_ConnFact::CreateConnection(*pGprsCom);
	S_CHANNEL *pAtcmdCom = STATUS.GetPppChannel(PPP_CHANNL_AT);
	if (pAtcmdCom != NULL)
		m_pAtCmdConnect = C_ConnFact::CreateConnection(*pAtcmdCom);
	m_pSmsConnect = NULL;//���Ҳ���
	m_pModem->SetModemConnection(m_pModemConnect);//default

	if (m_pModemConnect == NULL)
	{
		C_ConnFact::DestroyConnection(m_pModemConnect);
		C_ConnFact::DestroyConnection(m_pGprsConnect);
		C_ConnFact::DestroyConnection(m_pAtCmdConnect);
		m_pModemConnect = NULL, m_pGprsConnect = NULL, m_pAtCmdConnect = NULL;
		return -1;
	}
	
	return 0;
}

bool C_PppServer::PppEnable(void)
{
	return true;
}

int C_PppServer::CheckNotify(void)
{
	if (PPP_STATUS_ONLINE == m_PppStatus)
	{
		S_ConnectStatus *pConnStatus = CONTEXT.GetConnectStatus();
		if (m_OnLine == true && m_PidPPP != -1)	
			pConnStatus->m_PppOnline = true; // ˢ�£���ֹ��ĵ���httx�յȴ�
	}

	if (m_PppStatus == PPP_STATUS_RESET || m_PppStatus == PPP_STATUS_INIT
		|| m_PppStatus == PPP_STATUS_SIGNAL)
		m_pPppStatus->m_fGprsDialing = false;
	else
		m_pPppStatus->m_fGprsDialing = true;
	m_pPppStatus->m_WirelessNetType = m_WirelessNetType;

	return LcdBottomInfo();
}

int C_PppServer::CheckModemType(void)
{
	return 0;
}

int C_PppServer::AdaptModem(void)
{
	if (!m_strModemFactory.empty())
		strcpy(m_pPppStatus->m_ModemFactory, m_strModemFactory.c_str());
	if (!m_strModemModel.empty())
		strcpy(m_pPppStatus->m_ModemModel, m_strModemModel.c_str());
	if (!m_strSimInfo.empty())
		strcpy(m_pPppStatus->m_ModemSim, m_strSimInfo.c_str());
	if (!m_strGmrInfo.empty())
		strcpy(m_pPppStatus->m_ModemVer, m_strGmrInfo.c_str());
	return 0;
}

int C_PppServer::OnPPPOffLine(void)//����ʧ�ܺ���ã�ͳ�Ʋ���ʧ�ܴ���
{
	S_ConnectStatus *pConnStatus = CONTEXT.GetConnectStatus();
	pConnStatus->m_PppOnline = false;
	m_pPppStatus->m_SimIP = 0x00;
	FLOWCTRL.PppDialRecord(false);
	return 0;
}

int C_PppServer::OnPPPOnLine(void)//���ųɹ�����ã���λ����ʧ�ܴ���������
{
	S_ConnectStatus *pConnStatus = CONTEXT.GetConnectStatus();
	pConnStatus->m_PppOnline = true;
	m_pPppStatus->m_timeLastOnline = C_TIME::TimeNow();
	m_pPppStatus->m_SimIP = C_IfConfig::GetSimIP();
	if (m_pPppStatus->m_SimIP != 0)
		SavePppRunInfo();
	FLOWCTRL.PppDialRecord(true); // һ����˵Ҫ�ȵ�½�ɹ���������ȫ���ųɹ�����ʱ������Ч��
	return 0;
}

bool C_PppServer::NeedPpp(void)
{
	S_ConnectStatus *pConnStatus = CONTEXT.GetConnectStatus();
	if (pConnStatus->m_HttxNeedPPP[0] == true 
		|| pConnStatus->m_HttxNeedPPP[1] == true
		|| pConnStatus->m_HttxNeedPPP[2] == true)
	{
		logit<<"HTTX��ҪPPP����"<<endl;
		return true;
	}
	return false;
}

bool C_PppServer::NeedOnLine(void)//����ǰ���ã��ж��Ƿ��������
{
	S_DATE date;
	S_TIME CurTime = C_TIME::TimeNow();
	C_TIME::LocalTime(CurTime, date);

	if (FLOWCTRL.FlowCtrlEnable() == true)
	{
		//������������
		if (FLOWCTRL.GetDayFlowMax() && STATUS.GetDayCommFlux() >= FLOWCTRL.GetDayFlowMax())
		{
			logit<<"FLOWCTRL:��ǰ������("<<STATUS.GetDayCommFlux()<<")�����趨�����������ֵ("<<FLOWCTRL.GetDayFlowMax()<<")���������ߣ�"<<endl;
			SleepSecond(5);
			return false;
		}
		if (FLOWCTRL.GetMonFlowMax() && STATUS.GetMonCommFlux() >= FLOWCTRL.GetMonFlowMax())
		{
			logit<<"FLOWCTRL:��ǰ������("<<STATUS.GetMonCommFlux()<<")�����趨�����������ֵ("<<FLOWCTRL.GetMonFlowMax()<<")���������ߣ�"<<endl;
			SleepSecond(5);
			return false;
		}
		//��վͨѶ���ں�ʱ���ж�
		if (!FLOWCTRL.ConnectDate(date.m_Day))
		{
			logit<<"FLOWCTRL:"<<date.m_Day<<"�շ���վͨѸʱ�ڣ�������!"<<endl;
			SleepSecond(5);
			return false;
		}
		if (!FLOWCTRL.ConnectTime(date.m_Hour))
		{
			logit<<"FLOWCTRL:"<<date.m_Hour<<"�����վͨѸʱ�ڣ�������!"<<endl;
			SleepSecond(5);
			return false;
		}
		
		if (FLOWCTRL.GetDialFailedTimes() == 0)
		{//��һ�β���
			return true;
		}
		else if (FLOWCTRL.GetDialFailedTimes() >= FLOWCTRL.GetDialRetryTimes())
		{//ָ��ʧ�ܴ�������Ϣһ��ʱ��
			S_DWORD diff = abs((long)(CurTime - m_LastOffLineTime));
			if (m_LastOffLineTime && diff < FLOWCTRL.GetDialFrequencyMinite() * (S_DWORD)60)
			{
				logit<<"FLOWCTRL:PPP������Ϣ״̬�����´����߻�Ҫ: "<<(FLOWCTRL.GetDialFrequencyMinite() * 60 - diff)<<" seconds."<<endl;
				SleepSecond(5);
				return false;
			}
			m_LastOffLineTime = 0;
			FLOWCTRL.PppDialRecord(true);
 			return true;		
		}
		else
		{//��Ϣָ���ļ��
			S_DWORD diff = abs((long)(CurTime - m_LastOffLineTime));
			if (m_LastOffLineTime && diff < FLOWCTRL.GetDialInterSecond())
			{
				logit<<"FLOWCTRL:PPP��Ϣָ�����: "<<(FLOWCTRL.GetDialInterSecond() - diff)<<" seconds."<<endl;
				SleepSecond(5);
				return false;
			}
			m_LastOffLineTime = 0;
			return true;		
		}
	}
	else//���������淶��������
	{
		if (m_LastOffLineTime == 0)
			return true;
		if (m_pPppInfo->m_RedialTimesPerDay == 0 || FLOWCTRL.GetDialFailedTimes() < m_pPppInfo->m_RedialTimesPerDay)
		{
			S_DWORD diff = abs((long)(CurTime - m_LastOffLineTime));
			if (diff < m_pPppInfo->m_RedialInterM * (S_DWORD)60)
			{
				logit<<"PPP��Ϣָ�����: "<<(m_pPppInfo->m_RedialInterM * 60 - diff)<<" seconds."<<endl;
				SleepSecond(5);
				return false;
			}
			m_LastOffLineTime = 0;
			return true;	
		}

		S_DATE OfflineDate;
		C_TIME::LocalTime(m_LastOffLineTime, OfflineDate);
		if (date.m_Day == OfflineDate.m_Day)
		{
			logit<<"�����趨�Ĳ��Ŵ���������("<<m_pPppInfo->m_RedialTimesPerDay<<"),������һ�쿪ʼ����"<<endl;
			SleepSecond(5);
			return false;
		}
		else
		{
			m_LastOffLineTime = 0;
			FLOWCTRL.PppDialRecord(true);
			return true;
		}
	}

	return true;
}

bool C_PppServer::NeedOffLine(void)
{
	if (m_OnLine == false)//������
		return false;
	if (C_PppBase::NeedOffLine() == true)
		return true;
	
	S_DATE date;
	C_TIME::LocalTime(C_TIME::TimeNow(), date);

	if (FLOWCTRL.FlowCtrlEnable() == true)
	{
		if (FLOWCTRL.GetDayFlowMax() && STATUS.GetDayCommFlux() >= FLOWCTRL.GetDayFlowMax())
		{
			logit<<"FLOWCTRL:��ǰ������("<<STATUS.GetDayCommFlux()<<")�����趨�����������ֵ("<<FLOWCTRL.GetDayFlowMax()<<")���������ߣ�"<<endl;
			return true;
		}
		if (FLOWCTRL.GetMonFlowMax() && STATUS.GetMonCommFlux() >= FLOWCTRL.GetMonFlowMax())
		{
			logit<<"FLOWCTRL:��ǰ������("<<STATUS.GetMonCommFlux()<<")�����趨�����������ֵ("<<FLOWCTRL.GetMonFlowMax()<<")���������ߣ�"<<endl;
			return true;
		}
		if (!FLOWCTRL.ConnectDate(date.m_Day))
		{
			logit<<"FLOWCTRL:"<<date.m_Day<<"�շ���վͨѶʱ�ڣ��������ߣ�"<<endl;
			return true;
		}
		if (!FLOWCTRL.ConnectTime(date.m_Hour))
		{
			logit<<"FLOWCTRL:"<<date.m_Hour<<"�����վͨѶʱ�ڣ��������ߣ�"<<endl;
			return true;
		}
	}
	else //���������淶��������
	{
	}

	S_ConnectStatus *pCS = CONTEXT.GetConnectStatus();
	if (pCS->m_HttxNeedPPP[0] == false && pCS->m_HttxNeedPPP[1] == false && pCS->m_HttxNeedPPP[2] == false)
	{
		SleepMS(500);//make sure
		if (pCS->m_HttxNeedPPP[0] == false && pCS->m_HttxNeedPPP[1] == false && pCS->m_HttxNeedPPP[2] == false)
		{
			logwt<<"�յ�HTTX����֪ͨ,PPP׼������..."<<endl;
			return true;
		}
	}

	return false;
}

bool C_PppServer::InitSignalVal(void)
{
	return SetSignalVal(-113);
}

bool C_PppServer::SetSignalVal(int signal)
{
	if (signal <= -113)
		m_pPppStatus->m_Signal = 0;
	else
		m_pPppStatus->m_Signal = (signal + 113) / 2;
	if (m_pPppStatus->m_Signal > 31)
		m_pPppStatus->m_Signal = 31;
	m_pPppStatus->m_timeSignal = C_TIME::TimeNow();
	return true;
}

int C_PppServer::GetModemPowerOffDelayS(void)
{
	return m_pPppInfo->m_ModemPowerOffDelayS;
}

int C_PppServer::GetModemPowerOnDelayS(void)
{
	return m_pPppInfo->m_ModemPowerOnDelayS;
}

S_WORD C_PppServer::GetRefreshSignalInterS(void)
{
	return m_pPppInfo->m_RefreshSignalInterS;
}

int C_PppServer::LcdBottomInfo(void)
{
	switch (m_PppStatus)
	{
	case PPP_STATUS_RESET:
		CONTEXT.SetLcdBottomInfoIndicator(MP_PROCESS_PPP, LCD_INDICATOR_PPP_MODEM_RESET);
		break;
	case PPP_STATUS_INIT:
		CONTEXT.SetLcdBottomInfoIndicator(MP_PROCESS_PPP, LCD_INDICATOR_PPP_MODEM_CHECK);
		break;
	case PPP_STATUS_SIGNAL:
		CONTEXT.SetLcdBottomInfoIndicator(MP_PROCESS_PPP, LCD_INDICATOR_PPP_MODEM_SIGNAL);
		break;
	case PPP_STATUS_DIAL:
		CONTEXT.SetLcdBottomInfoIndicator(MP_PROCESS_PPP, LCD_INDICATOR_PPP_DIALING);
		break;
	case PPP_STATUS_ONLINE:
		CONTEXT.SetLcdBottomInfoIndicator(MP_PROCESS_PPP, LCD_INDICATOR_PPP_CONNECTED);
		break;
	case PPP_STATUS_OFFLINE:
		CONTEXT.SetLcdBottomInfoIndicator(MP_PROCESS_PPP, LCD_INDICATOR_PPP_FAILED);
		break;
	default:
		break;
	}
	return 0;
}

int C_PppServer::SavePppRunInfo(void)
{
	S_SYSRUN *pSysRun = STATUS.GetSysRunInfo();
	if (pSysRun->m_SimIP != m_pPppStatus->m_SimIP
		|| strcmp(pSysRun->m_ModemFactory, m_strModemFactory.c_str()) != 0
		|| strcmp(pSysRun->m_ModemModel, m_strModemModel.c_str()) != 0)
	{
		pSysRun->m_SimIP = m_pPppStatus->m_SimIP;
		strcpy(pSysRun->m_ModemFactory, m_strModemFactory.c_str());
		strcpy(pSysRun->m_ModemModel, m_strModemModel.c_str());
		STATUS.SaveSysRunInfo();
	}
	return 0;
}


