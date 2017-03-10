/** @file
 *  @brief PPP拨号服务功能定义
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
	m_pSmsConnect = NULL;//暂且不用
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
			pConnStatus->m_PppOnline = true; // 刷新，防止误改导致httx空等待
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

int C_PppServer::OnPPPOffLine(void)//拨号失败后调用，统计拨号失败次数
{
	S_ConnectStatus *pConnStatus = CONTEXT.GetConnectStatus();
	pConnStatus->m_PppOnline = false;
	m_pPppStatus->m_SimIP = 0x00;
	FLOWCTRL.PppDialRecord(false);
	return 0;
}

int C_PppServer::OnPPPOnLine(void)//拨号成功后调用，复位拨号失败次数计数器
{
	S_ConnectStatus *pConnStatus = CONTEXT.GetConnectStatus();
	pConnStatus->m_PppOnline = true;
	m_pPppStatus->m_timeLastOnline = C_TIME::TimeNow();
	m_pPppStatus->m_SimIP = C_IfConfig::GetSimIP();
	if (m_pPppStatus->m_SimIP != 0)
		SavePppRunInfo();
	FLOWCTRL.PppDialRecord(true); // 一般来说要等登陆成功才能算完全拨号成功，暂时这样看效果
	return 0;
}

bool C_PppServer::NeedPpp(void)
{
	S_ConnectStatus *pConnStatus = CONTEXT.GetConnectStatus();
	if (pConnStatus->m_HttxNeedPPP[0] == true 
		|| pConnStatus->m_HttxNeedPPP[1] == true
		|| pConnStatus->m_HttxNeedPPP[2] == true)
	{
		logit<<"HTTX需要PPP拨号"<<endl;
		return true;
	}
	return false;
}

bool C_PppServer::NeedOnLine(void)//拨号前调用，判断是否进行连接
{
	S_DATE date;
	S_TIME CurTime = C_TIME::TimeNow();
	C_TIME::LocalTime(CurTime, date);

	if (FLOWCTRL.FlowCtrlEnable() == true)
	{
		//日月流量控制
		if (FLOWCTRL.GetDayFlowMax() && STATUS.GetDayCommFlux() >= FLOWCTRL.GetDayFlowMax())
		{
			logit<<"FLOWCTRL:当前日流量("<<STATUS.GetDayCommFlux()<<")大于设定的日流量最大值("<<FLOWCTRL.GetDayFlowMax()<<")！不能上线！"<<endl;
			SleepSecond(5);
			return false;
		}
		if (FLOWCTRL.GetMonFlowMax() && STATUS.GetMonCommFlux() >= FLOWCTRL.GetMonFlowMax())
		{
			logit<<"FLOWCTRL:当前月流量("<<STATUS.GetMonCommFlux()<<")大于设定的月流量最大值("<<FLOWCTRL.GetMonFlowMax()<<")！不能上线！"<<endl;
			SleepSecond(5);
			return false;
		}
		//主站通讯日期和时间判断
		if (!FLOWCTRL.ConnectDate(date.m_Day))
		{
			logit<<"FLOWCTRL:"<<date.m_Day<<"日非主站通迅时期，不上线!"<<endl;
			SleepSecond(5);
			return false;
		}
		if (!FLOWCTRL.ConnectTime(date.m_Hour))
		{
			logit<<"FLOWCTRL:"<<date.m_Hour<<"点非主站通迅时期，不上线!"<<endl;
			SleepSecond(5);
			return false;
		}
		
		if (FLOWCTRL.GetDialFailedTimes() == 0)
		{//第一次拨号
			return true;
		}
		else if (FLOWCTRL.GetDialFailedTimes() >= FLOWCTRL.GetDialRetryTimes())
		{//指定失败次数后，休息一段时间
			S_DWORD diff = abs((long)(CurTime - m_LastOffLineTime));
			if (m_LastOffLineTime && diff < FLOWCTRL.GetDialFrequencyMinite() * (S_DWORD)60)
			{
				logit<<"FLOWCTRL:PPP进入休息状态，离下次上线还要: "<<(FLOWCTRL.GetDialFrequencyMinite() * 60 - diff)<<" seconds."<<endl;
				SleepSecond(5);
				return false;
			}
			m_LastOffLineTime = 0;
			FLOWCTRL.PppDialRecord(true);
 			return true;		
		}
		else
		{//休息指定的间隔
			S_DWORD diff = abs((long)(CurTime - m_LastOffLineTime));
			if (m_LastOffLineTime && diff < FLOWCTRL.GetDialInterSecond())
			{
				logit<<"FLOWCTRL:PPP休息指定间隔: "<<(FLOWCTRL.GetDialInterSecond() - diff)<<" seconds."<<endl;
				SleepSecond(5);
				return false;
			}
			m_LastOffLineTime = 0;
			return true;		
		}
	}
	else//采用南网规范流量控制
	{
		if (m_LastOffLineTime == 0)
			return true;
		if (m_pPppInfo->m_RedialTimesPerDay == 0 || FLOWCTRL.GetDialFailedTimes() < m_pPppInfo->m_RedialTimesPerDay)
		{
			S_DWORD diff = abs((long)(CurTime - m_LastOffLineTime));
			if (diff < m_pPppInfo->m_RedialInterM * (S_DWORD)60)
			{
				logit<<"PPP休息指定间隔: "<<(m_pPppInfo->m_RedialInterM * 60 - diff)<<" seconds."<<endl;
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
			logit<<"今天设定的拨号次数已用完("<<m_pPppInfo->m_RedialTimesPerDay<<"),将在下一天开始拨号"<<endl;
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
	if (m_OnLine == false)//不在线
		return false;
	if (C_PppBase::NeedOffLine() == true)
		return true;
	
	S_DATE date;
	C_TIME::LocalTime(C_TIME::TimeNow(), date);

	if (FLOWCTRL.FlowCtrlEnable() == true)
	{
		if (FLOWCTRL.GetDayFlowMax() && STATUS.GetDayCommFlux() >= FLOWCTRL.GetDayFlowMax())
		{
			logit<<"FLOWCTRL:当前日流量("<<STATUS.GetDayCommFlux()<<")大于设定的日流量最大值("<<FLOWCTRL.GetDayFlowMax()<<")！必需下线！"<<endl;
			return true;
		}
		if (FLOWCTRL.GetMonFlowMax() && STATUS.GetMonCommFlux() >= FLOWCTRL.GetMonFlowMax())
		{
			logit<<"FLOWCTRL:当前月流量("<<STATUS.GetMonCommFlux()<<")大于设定的月流量最大值("<<FLOWCTRL.GetMonFlowMax()<<")！必需下线！"<<endl;
			return true;
		}
		if (!FLOWCTRL.ConnectDate(date.m_Day))
		{
			logit<<"FLOWCTRL:"<<date.m_Day<<"日非主站通讯时期，必需下线！"<<endl;
			return true;
		}
		if (!FLOWCTRL.ConnectTime(date.m_Hour))
		{
			logit<<"FLOWCTRL:"<<date.m_Hour<<"点非主站通讯时期，必需下线！"<<endl;
			return true;
		}
	}
	else //采用南网规范流量控制
	{
	}

	S_ConnectStatus *pCS = CONTEXT.GetConnectStatus();
	if (pCS->m_HttxNeedPPP[0] == false && pCS->m_HttxNeedPPP[1] == false && pCS->m_HttxNeedPPP[2] == false)
	{
		SleepMS(500);//make sure
		if (pCS->m_HttxNeedPPP[0] == false && pCS->m_HttxNeedPPP[1] == false && pCS->m_HttxNeedPPP[2] == false)
		{
			logwt<<"收到HTTX下线通知,PPP准备下线..."<<endl;
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


