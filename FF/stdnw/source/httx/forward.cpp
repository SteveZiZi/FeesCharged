/** @file
 *  @brief 级联功能定义
 *  @author 
 *  @date 2011/12/27
 *  @version 0.1
 */
#include "forward.h"
#include "cmdfact.h"
#include "httxrule.h"
#include "context.h"
#include "status.h"

//
//
//
int C_Forward::m_WorkSlaveID = 0;

int C_Forward::ParseSlave(C_MSRULE *pRule, string &strCmd, string &strEnergy)
{
	INT8U AFN = 0;
	string strCmdInfo;
	if (pRule->ParseSlave(strCmd, AFN, strCmdInfo) == FRAME_PROCESS_CONTINUE)
	{
		if (AFN == MSAFN_FORWARD)
		{
			S_CmdObjs CmdObjs;
			strings strResponseInfos;
			C_AFNCMD *pMsCmd = C_CmdFact::GetAfnCmd(MSAFN_FORWARD_MS);
			if(pMsCmd->ParseCmdObj(strCmdInfo, CmdObjs) == FRAME_PROCESS_CONTINUE)
			{
				int ok = pMsCmd->Execute(CmdObjs, strResponseInfos);
				if (ok == PARSE_SLAVE_GATHER_ENERGY)
					strEnergy.append(strResponseInfos[0]);
				return ok;
			}
		}
		return PARSE_SLAVE_FORWARD_MS;
	}
	return PARSE_SLAVE_UNKNOW;
}

void C_ForwardIdle::init(void)
{
	m_strRecvBuffer.clear();
}

C_Forward *C_ForwardIdle::ForwardProcess(C_CONNECT* pForward, strings &strMsCmds, string &strCmd)
{
#if 0
	string strMyCmd;
	C_MSRULE *pRule = C_RuleFact::GetMsRule(MS_RULE_ID_SLAVE);
	pForward->RecvData(m_strRecvBuffer);
	if (pRule->ExtractCmd(m_strRecvBuffer, strMyCmd) > 0)
	{//avoid lose cmd for slave send master station
		string strEnergy;
		if (ParseSlave(pRule, strMyCmd, strEnergy) == PARSE_SLAVE_FORWARD_MS)
			strCmd.append(strMyCmd);
	}
#endif	
	if (!strMsCmds.empty())
	{
		m_WorkSlaveID = 0;
		return C_ForwardFact::GetForwardStatusObj(FS_TRANS);
	}
	S_TIME tickNow = CONTEXT.GetSystemTick();
	S_ForwardInfo *pInfo = STATUS.GetForwardInfo();
	for (int i = 0; i < NW_FORWARD_ADDR_MAX + 1; i++)
	{

		if (tickNow - m_tickPoll[i] >= pInfo->m_PollIntervalS && pInfo->GetForwardedAddr(i) != 0xFFFFFFFF)
		{
			m_WorkSlaveID = i + 1, m_tickPoll[i] = tickNow;
			if (i == NW_FORWARD_ADDR_MAX)
				return C_ForwardFact::GetForwardStatusObj(FS_GATHER);
			return C_ForwardFact::GetForwardStatusObj(FS_POLL);
		}
	}
	return this;
}

void C_ForwardPoll::init(void)
{
	m_tickSendCmd = 0;
	m_strRecvBuffer.clear();
}

C_Forward *C_ForwardPoll::ForwardProcess(C_CONNECT* pForward, strings &strMsCmds, string &strCmd)
{
	C_MSRULE *pRule = C_RuleFact::GetMsRule(MS_RULE_ID_SLAVE);
	S_TIME tickNow = CONTEXT.GetSystemTick();
	S_ForwardInfo *pInfo = STATUS.GetForwardInfo();

	if (m_tickSendCmd == 0)
	{
		string strPoll;
		pRule->MakeForwardPoll(m_WorkSlaveID-1, strPoll);
		pForward->SendData(strPoll);
		m_tickSendCmd = tickNow;
	}
	else
	{
		string strMyCmd;

		pForward->RecvData(m_strRecvBuffer);
		if (pRule->ExtractCmd(m_strRecvBuffer, strMyCmd) > 0)
		{
			string strEnergy;

			int result = ParseSlave(pRule, strMyCmd, strEnergy);
			if (result == PARSE_SLAVE_REPORT_YES)
				return C_ForwardFact::GetForwardStatusObj(FS_TRANS);
			else if (result == PARSE_SLAVE_REPORT_NO)
				return C_ForwardFact::GetForwardStatusObj(FS_IDLE);
		}

		if (tickNow - m_tickSendCmd >= pInfo->m_WaitSlaveResponseS)
			return C_ForwardFact::GetForwardStatusObj(FS_IDLE);
	}
	return this;
}

void C_ForwardTrans::init(void)
{
	m_tickSendCmd = 0;
	m_strRecvBuffer.clear();
}

C_Forward *C_ForwardTrans::ForwardProcess(C_CONNECT* pForward, strings &strMsCmds, string &strCmd)
{
	if (m_WorkSlaveID == 0)
		return ForwardProcess_Ms2Slave(pForward, strMsCmds, strCmd);
	else
		return ForwardProcess_SlaveReport(pForward, strMsCmds, strCmd);
}

C_Forward *C_ForwardTrans::ForwardProcess_Ms2Slave(C_CONNECT* pForward, strings &strMsCmds, string &strCmd)
{
	C_MSRULE *pRule = C_RuleFact::GetMsRule(MS_RULE_ID_SLAVE);
	S_TIME tickNow = CONTEXT.GetSystemTick();
	S_ForwardInfo *pInfo = STATUS.GetForwardInfo();

	if (m_tickSendCmd == 0)
	{
		if (strMsCmds.empty())
			return C_ForwardFact::GetForwardStatusObj(FS_IDLE);
		pForward->SendData(strMsCmds[0]);
		strMsCmds.erase(strMsCmds.begin());
		m_tickSendCmd = tickNow;
	}
	else
	{
		string strMyCmd;
		pForward->RecvData(m_strRecvBuffer);
		if (pRule->ExtractCmd(m_strRecvBuffer, strMyCmd) > 0)
		{
			string strEnergy;
			int status = ParseSlave(pRule, strMyCmd, strEnergy);
			if (status == PARSE_SLAVE_FORWARD_MS)
			{
				string strTransCtrlCmd;
				C_RTUA FrameAddr;
				S_ForwardInfo *pInfo = STATUS.GetForwardInfo();
				
				string strFrameAddr(m_strRecvBuffer.begin()+7, m_strRecvBuffer.begin()+7+5);
				FrameAddr.ParseString(strFrameAddr);
				S_RTUA rtua = FrameAddr.GetRTUA();

				for (int i = 0; i < NW_FORWARD_ADDR_MAX + 1; i++)
				{
					if (pInfo->GetForwardedAddr(i) == rtua.m_Address)
					{
						pRule->MakeForwardCtrl(i, strTransCtrlCmd);
						pForward->SendData(strTransCtrlCmd);
						m_tickSendCmd = tickNow;
						strCmd.append(strMyCmd);
						break;
					}
				}

			}
			else if (status == PARSE_SLAVE_REPORT_NO)
			{
				m_tickSendCmd = 0;
			}
		}
		else
		{
			if (tickNow - m_tickSendCmd >= pInfo->m_WaitSlaveResponseS)
				m_tickSendCmd = 0;
		}
	}
	return this;
}

C_Forward *C_ForwardTrans::ForwardProcess_SlaveReport(C_CONNECT* pForward, strings &strMsCmds, string &strCmd)
{
	C_MSRULE *pRule = C_RuleFact::GetMsRule(MS_RULE_ID_SLAVE);
	S_TIME tickNow = CONTEXT.GetSystemTick();
	S_ForwardInfo *pInfo = STATUS.GetForwardInfo();

	if (m_tickSendCmd == 0)
	{
		string strCtrl;
		pRule->MakeForwardCtrl(m_WorkSlaveID-1, strCtrl);
		pForward->SendData(strCtrl);
		m_tickSendCmd = tickNow;
	}
	else if (!strMsCmds.empty())
	{
		pForward->SendData(strMsCmds[0]);
		strMsCmds.erase(strMsCmds.begin());
		m_tickSendCmd = tickNow;
	}
	else
	{
		string strMyCmd;
		pForward->RecvData(m_strRecvBuffer);
		if (pRule->ExtractCmd(m_strRecvBuffer, strMyCmd) > 0)
		{
			string strEnergy;
			int status = ParseSlave(pRule, strMyCmd, strEnergy);
			if (status == PARSE_SLAVE_FORWARD_MS)
			{
				string strTransCtrl;
				pRule->MakeForwardCtrl(m_WorkSlaveID-1, strTransCtrl);
				pForward->SendData(strTransCtrl);
				m_tickSendCmd = tickNow;
				strCmd.append(strMyCmd);
			}
			else if (status == PARSE_SLAVE_REPORT_NO)
			{
				return C_ForwardFact::GetForwardStatusObj(FS_IDLE);
			}
		}
		else
		{
			if (tickNow - m_tickSendCmd >= pInfo->m_WaitSlaveResponseS)
				return C_ForwardFact::GetForwardStatusObj(FS_IDLE);
		}
	}
	return this;
}

void C_ForwardGather::init(void)
{
	m_tickSendCmd = 0;
	m_strRecvBuffer.clear();
}

C_Forward *C_ForwardGather::ForwardProcess(C_CONNECT* pForward, strings &strMsCmds, string &strCmd)
{
	C_MSRULE *pRule = C_RuleFact::GetMsRule(MS_RULE_ID_SLAVE);
	S_TIME tickNow = CONTEXT.GetSystemTick();
	S_ForwardInfo *pInfo = STATUS.GetForwardInfo();
	if (m_tickSendCmd == 0)
	{
		string strGather;
		pRule->MakeForwardGather(m_WorkSlaveID-1, strGather);
		pForward->SendData(strGather);
		m_tickSendCmd = tickNow;
	}
	else
	{
		string strMyCmd;
		pForward->RecvData(m_strRecvBuffer);
		if (pRule->ExtractCmd(m_strRecvBuffer, strMyCmd) > 0)
		{
			string strEnergy;
			if (ParseSlave(pRule, strMyCmd, strEnergy) == PARSE_SLAVE_GATHER_ENERGY)
			{
				//TODO:handle energy
				return C_ForwardFact::GetForwardStatusObj(FS_IDLE);
			}
		}

		if (tickNow - m_tickSendCmd >= pInfo->m_WaitSlaveResponseS)
			return C_ForwardFact::GetForwardStatusObj(FS_IDLE);
	}
	return this;
}

//
//
//
C_Forward *C_ForwardFact::GetForwardStatusObj(int status)
{
	static C_ForwardIdle sForwardIdle;
	static C_ForwardPoll sForwardPoll;
	static C_ForwardTrans sForwardTrans;
	static C_ForwardGather sForwardGather;
	switch (status)
	{
	case C_Forward::FS_IDLE:sForwardIdle.init();return &sForwardIdle;
	case C_Forward::FS_POLL:sForwardPoll.init();return &sForwardPoll;
	case C_Forward::FS_TRANS:sForwardTrans.init();return &sForwardTrans;
	case C_Forward::FS_GATHER:sForwardGather.init();return &sForwardGather;
	}
	return NULL;
}

int C_ForwardContext::Init(C_CONNECT* pForwardConnect)
{
	S_ForwardInfo *pInfo = STATUS.GetForwardInfo();
	m_pForwardStatus  = C_ForwardFact::GetForwardStatusObj(C_Forward::FS_IDLE);
	m_pForwardConnect = pForwardConnect;
	if (m_pForwardConnect != NULL && pInfo->AnySlaveAddress() == true)
	{
		for (int i = 0; i < 100; i++)
		{//try 100 times
			if (m_pForwardConnect->Connect() == 0)
				break;
			SleepSecond(1);
		}
	}
	return 0;
}

int C_ForwardContext::DeliverMsCmd(const string &strCmd)
{
	if (m_pForwardConnect == NULL || m_pForwardStatus == NULL)
		return -1;
	if (m_pForwardConnect->IsConnected() == false)
		return -1;

	m_strMsCmds.push_back(strCmd);
	return 0;
}

int C_ForwardContext::ForwardProcess(string &strCmd)
{
	if (m_pForwardConnect == NULL || m_pForwardStatus == NULL)
		return -1;
	if (m_pForwardConnect->IsConnected() == false)
		return -1;

	string strMyCmd;
	m_pForwardStatus = m_pForwardStatus->ForwardProcess(m_pForwardConnect, m_strMsCmds, strMyCmd);
	if (strMyCmd.empty())
		return 0;
	strCmd.append(strMyCmd);
	return (int)strMyCmd.size();
}



