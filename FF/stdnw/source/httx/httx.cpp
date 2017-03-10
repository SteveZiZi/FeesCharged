/** @file
 *  @brief ��̨ͨѶ�����ܶ���
 *  @author 
 *  @date 2011/11/30
 *  @version 0.1
 */
#include "httx.h"
#include "sysipc.h"
#include "htrace.h"
#include "flowctrl.h"
#include "context.h"
#include "status.h"
#include "connfact.h"
#include "httxrule.h"
#include "cmdfact.h"
#include "flowctrl.h"
#include "metercom.h"
#include "sysctrl.h"
#include "mscmd/cmddata.h"
#include "ipcSharemem.h"

C_HttxServer::C_HttxServer(void)
{
	m_HttxSN = MP_HTTX_1;
	m_pChannel = NULL;
	m_pMStationConnect = NULL;
	m_pForwardConnect = NULL;
	m_pMsRule = NULL;
	m_pReportHandle = NULL;
	
	m_pConnectStatus = NULL;
	m_pHttxStatus = NULL;
}

C_HttxServer::~C_HttxServer()
{
//	C_ConnFact::DestroyConnection(m_pMStationConnect);
//	C_ConnFact::DestroyConnection(m_pForwardConnect);
}

int C_HttxServer::BeforeService(void)
{
	m_pConnectStatus = CONTEXT.GetConnectStatus();
	m_pHttxStatus = CONTEXT.GetHttxStatus(m_HttxSN);

	S_ForwardInfo *pForwardInfo = STATUS.GetForwardInfo();
	m_pHttxStatus->m_fMasterTerm = (pForwardInfo->m_MasterAddr == 0xFFFFFFFFUL? true:false);
	if (m_pHttxStatus->m_fMasterTerm == false && m_HttxSN == MP_HTTX_1)
	{
		m_pHttxStatus->m_fForwardEnable = false;
		
	}	
	else
		m_pHttxStatus->m_fForwardEnable = true;

	C_MeterCom MeterCom;
	MeterCom.SetFillChar((char)0xFF);
	FLOWCTRL.SetFlowCtrlInfo(CONTEXT.GetFlowInfo());
	return 0;
}

int C_HttxServer::AfterService(void)
{
	return 0;
}

int C_HttxServer::OnTickService(void)
{
	for (int i = HTTX_CHANNEL_1; i < HTTX_CHANNL_MAX; i += 1)
	{
		/** @brief ��������ͨ�� */
		if (m_pHttxStatus->m_fMasterTerm == false && m_HttxSN == MP_HTTX_1)
			m_pChannel = STATUS.GetForwardChannel(FORWARD_CHANNL_SLAVE);//����
		else
		{
			m_pChannel = STATUS.GetHttxChannel(m_HttxSN, (S_ChannelSN)i);

			/* �����QT�ڴ�ͨѶ���ҵ�ǰ����Ϊ���磬���ʼ����վip��Ϣ*/
			if (m_pChannel != NULL && m_pChannel->m_Type == CHANNEL_TYPE_NET && m_HttxSN == MP_HTTX_1)
			{
				struct SIpcCommParam CommParam;
				CommParam = IPCSHAREMEM.m_IpcTerminalInfo.GetCommParm();
				/* ip */
				memcpy((char*)(&CommParam.m_ServerIP),m_pChannel->m_PortSet.m_NetSet.m_IP,sizeof(m_pChannel->m_PortSet.m_NetSet.m_IP));
				/* port */
				CommParam.m_ServerPort = m_pChannel->m_PortSet.m_NetSet.m_PortNO;
				IPCSHAREMEM.m_IpcTerminalInfo.SetCommParm(CommParam);
				IPCSHAREMEM.Update();
			}
		}
		HeartBeat();
		if (m_pChannel == NULL)
		{
			loget<<"����ͨ��("<<(int)i<<")ʧ��!!!"<<endl;
			SleepSecond(1);
			continue;
		}

		if ((m_pMStationConnect = C_ConnFact::CreateConnection(*m_pChannel)) == NULL)
		{
			loget<<"����ͨ��("<<m_pChannel->m_Name<<")ʧ��!!!"<<endl;
			SleepSecond(1);
			continue;
		}
		
		/** @brief ���Ҽ���ͨ�� */
		if (m_pHttxStatus->m_fMasterTerm == true && m_HttxSN == MP_HTTX_1 && ForwardChannelEnable() == true)
		{//����ͨ��ʧ��Ҳ������
			S_CHANNEL *pSlaveChannel = STATUS.GetForwardChannel(FORWARD_CHANNL_SLAVE);//����
			if (pSlaveChannel != NULL)
				m_pForwardConnect = C_ConnFact::CreateConnection(*pSlaveChannel);
		}
		
		ChannelServer();

		C_ConnFact::DestroyConnection(m_pMStationConnect);
		C_ConnFact::DestroyConnection(m_pForwardConnect);
		m_pMStationConnect = NULL, m_pForwardConnect = NULL;

		SleepMS(20);
	}

	return 0;
}

#define CONNECT_RETRY_TIMES_MAX	3
int C_HttxServer::ChannelServer(void)
{
	S_SYSCFG *pSysCfg = STATUS.GetSystemConfig();
	S_ForwardInfo *pForwardInfo = STATUS.GetForwardInfo();
	m_pMsRule = C_RuleFact::GetMsRule(MS_RULE_ID_MASTER);
	m_pMsRule->SetPackageLengthMax(m_pChannel->m_PackageLenMax);
	m_pMsRule->SetAddress(&pSysCfg->m_RTUA, pForwardInfo->m_SlaveAddr, &pForwardInfo->m_GatherAddr);

	C_MSRULE *pSlaveRule = C_RuleFact::GetMsRule(MS_RULE_ID_SLAVE);
//	pSlaveRule->SetPackageLengthMax(m_pChannel->m_PackageLenMax);
	pSlaveRule->SetAddress(&pSysCfg->m_RTUA, pForwardInfo->m_SlaveAddr, &pForwardInfo->m_GatherAddr);
	
	if (m_pHttxStatus->m_fMasterTerm == true && m_HttxSN == MP_HTTX_1)
	{
		if (m_pChannel->IsTcpServer())
			m_pConnectStatus->m_Httx1CommStatus = HTTX1_COMM_STATUS_LISTEN;
		else
			m_pConnectStatus->m_Httx1CommStatus = HTTX1_COMM_STATUS_NOTHING;

		m_pConnectStatus->m_Httx1Link = false,m_pConnectStatus->m_Httx1OnPpp = false;
		m_pConnectStatus->m_Httx1Logon = false,m_pConnectStatus->m_Httx1LogonTime = 0;	
	}
	m_pConnectStatus->m_HttxNeedPPP[m_HttxSN] = false;

	if (m_pChannel->NeedPPP() == true)
	{
		if (WaitPppDial() < 0)
			return -1;
	}
	if (ConnectMasterStation() < 0)
		return -1;
	if (m_HttxSN == MP_HTTX_1)
	{
		m_pConnectStatus->m_Httx1Link = true;
		if (m_pChannel->IsTcpLink() == true) // ��֤�Ƿ�Ϊ��̫������;TCP����ʱ����Ч
			m_pConnectStatus->m_Httx1OnPpp = CheckCommunicationMode(m_pChannel);

		if (m_pChannel->IsTcpServer() || m_pChannel->IsUdpServer())
			m_pConnectStatus->m_Httx1CommStatus = HTTX1_COMM_STATUS_SERVER_LINK;
		else
			m_pConnectStatus->m_Httx1CommStatus = HTTX1_COMM_STATUS_CLIENT_LINK;
	}
	logwt<<"���ӳɹ�!!!"<<endl;

	//clear cmd buffer
	m_strRecvMsBuffer.clear();
	m_strResponseCmds.clear();
	m_ReportBuffer.m_strReportCmds.clear();
	//chain of responsibility for report
	C_ReportFact ReportFact;
	m_pReportHandle = ReportFact.GetReportChain();
	m_pReportHandle->InitInfo(m_HttxSN, m_pMsRule);
	//forward function init
	m_ForwardContext.Init(m_pForwardConnect);
	//clear flowctrl cnt
	FLOWCTRL.LogonRecord(true);
	FLOWCTRL.HeartBeatRecord(true);
	m_pConnectStatus->m_Reconnect = false; ///< ��ֹ�ڱ��ز�������λ
	//init tickRecvMsData
	m_pHttxStatus->m_tickRecvMsData = CONTEXT.GetSystemTick(); ///< ��ʼ��

	while (m_pMStationConnect->IsConnected())
	{
		CmdProcess(); // ���ղ��������վ��������
		DataTransfer(); //���ݴ���
		ForwardProcess(); // ��������
		DataReport();	//�ϱ�����
		OffLineCheck();

		HeartBeat();
		SleepMS(1);
	}
	m_pMStationConnect->Disconnect();
	if (FLOWCTRL.FlowCtrlEnable() == true && m_pChannel->NeedPPP() == true)
		FLOWCTRL.SocketConnectRecord(false);//�ж����ӣ�ʧ�ܴ�����1����ֹ�����ɹ���ȡʧ�ܵ����
	if (m_pHttxStatus->m_fMasterTerm == true && m_HttxSN == MP_HTTX_1)
		m_pConnectStatus->m_Httx1Link = false,m_pConnectStatus->m_Httx1Logon = false,m_pConnectStatus->m_Httx1LogonTime = 0;	

	return 0;
}

int C_HttxServer::CmdProcess(void)
{
	string strCmd;

	C_CmdRealtime  readReal;
	S_TIME time =  C_TIME::TimeNow();
	if (time%15 == 0)
		readReal.GetCurZXYG();

	if (GetMsCmd(strCmd) <= 0)
		return 0;

	int AddrID = CheckMsCmdAddr(strCmd);
	if (AddrID == FRAME_ADDR_ME || AddrID == FRAME_ADDR_BROADCAST)
	{
		strings strResponseInfos;
		int status = MsCmdHandle(strCmd, strResponseInfos);
		if (status == FRAME_PROCESS_CONFIRM || status == FRAME_PROCESS_DENY)
		{
			ReportConfirm(status == FRAME_PROCESS_CONFIRM? true:false);
		}
		else if ((status == FRAME_PROCESS_RESPONSE_CONFIRM || status == FRAME_PROCESS_RESPONSE_DENY
			|| status == FRAME_PROCESS_RESPONSE_ITEM || status == FRAME_PROCESS_RESPONSE_DATA)
				/*&& AddrID == FRAME_ADDR_ME*/) //  ���ԺҪ��㲥������Ҫ���ر���
		{
			if (status == FRAME_PROCESS_RESPONSE_DATA)
			{
				m_pMsRule->MakeResponseCmd(strResponseInfos, m_strResponseCmds);
			}
			else if (status == FRAME_PROCESS_RESPONSE_ITEM)
			{
				string strMyCmd;
				if (m_pMsRule->MakeConfirmCmd(m_strResponseCmds, strMyCmd) >= 0)
					m_strResponseCmds.push_back(strMyCmd);
			}
			else
			{
				bool fConfirm = (status == FRAME_PROCESS_RESPONSE_CONFIRM? true:false);
				string strMyCmd;
				m_pMsRule->MakeConfirmCmd(fConfirm, strMyCmd);
				m_strResponseCmds.push_back(strMyCmd);
			}
			m_pHttxStatus->m_fForwardEnable = true;//ֻҪ�յ���վ֡��Ĭ����������
		}
	}
	if (AddrID == FRAME_ADDR_BROADCAST || (AddrID >= FRAME_ADDR_SLAVE_MIN && AddrID <= FRAME_ADDR_SLAVE_MAX))
	{
		m_ForwardContext.DeliverMsCmd(strCmd);
	}
	return 0;
}

int C_HttxServer::ForwardProcess(void)
{
	string strForwardCmd;
	if (m_ForwardContext.ForwardProcess(strForwardCmd) > 0)
		m_strResponseCmds.push_back(strForwardCmd);
	return 0;
}

int C_HttxServer::DataReport(void)
{
	if (m_pReportHandle != NULL && m_ReportBuffer.m_strReportCmds.empty())
		return m_pReportHandle->GetReportData(m_ReportBuffer);
	return 0;
}

int C_HttxServer::DataTransfer(void)
{
	/** @brief Send Data */
	ResponseCmdSend();
	ReportCmdSend();
	
	/** @brief Recv Data */
	if (MStationRecv(m_strRecvMsBuffer) > 0)
		m_pHttxStatus->m_tickRecvMsData = CONTEXT.GetSystemTick();
	return 0;
}

int C_HttxServer::OffLineCheck(void)
{
	if (NeedOffLine() == true)
		m_pMStationConnect->Disconnect();
	return 0;
}

int C_HttxServer::MsCmdHandle(const string &strCmd, strings &strResponseInfos)
{
	INT8U AFN = 0;
	string strCmdInfo;

	int status = m_pMsRule->Parse(strCmd, AFN, strCmdInfo);
	if (status != FRAME_PROCESS_CONTINUE)
	{
		logwt<<"�����ܷ�������!!!"<<endl;
		return status;
	}
	if (AFN == MSAFN_WRITE_PARAM)
	{
		if (CheckProgramKey() == false)
			return FRAME_PROCESS_STOP;
	}

	S_CmdObjs CmdObjs;
	C_AFNCMD *pMsCmd = C_CmdFact::GetAfnCmd(AFN);
	pMsCmd->SetCmdInfo(m_pMsRule->GetMSA(), m_pMsRule->GetPackageLength());
	status = pMsCmd->ParseCmdObj(strCmdInfo, CmdObjs);
	if (status != FRAME_PROCESS_CONTINUE)
	{
		logwt<<"���������������!!!"<<endl;
		return status;
	}
	return pMsCmd->Execute(CmdObjs, strResponseInfos);
}

int C_HttxServer::ReportConfirm(bool fConfirm)
{
	if (m_ReportBuffer.m_strReportCmds.empty())
		return -1;

	m_ReportBuffer.m_Retry = 0;
	m_ReportBuffer.m_tickSendCmd = 0;
	m_ReportBuffer.m_strReportCmds.erase(m_ReportBuffer.m_strReportCmds.begin());
	if (m_pReportHandle != NULL)
	{
		m_pReportHandle->UpdateInfo_ReportOne(fConfirm);
		if (m_ReportBuffer.m_strReportCmds.empty())
			m_pReportHandle->UpdateInfo_ReportAll(fConfirm);
	}
	return 0;
}

bool C_HttxServer::NeedOffLine(void)
{
#define LOGON_FAILED_TIMES_MAX 3
	if (m_HttxSN == MP_HTTX_1 && m_pConnectStatus->m_Httx1OnPpp == false)
	{
		S_LcdStatus *pLcdStatus = CONTEXT.GetLcdStatus();
		if (pLcdStatus->m_Eth0LinePlugin == false) // ���߲���
			return true;
	}
	if (m_HttxSN == MP_HTTX_1 && m_pConnectStatus->m_Reconnect == true)
	{
		logwt<<"�յ���������֪ͨ,�Ͽ�����..."<<endl;
		m_pConnectStatus->m_Reconnect = false;

		/** @brief confirm forward info */
		S_ForwardInfo *pForwardInfo = STATUS.GetForwardInfo();
		m_pHttxStatus->m_fMasterTerm = (pForwardInfo->m_MasterAddr == 0xFFFFFFFFUL? true:false);
		if (m_pHttxStatus->m_fMasterTerm == false && m_HttxSN == MP_HTTX_1)
			m_pHttxStatus->m_fForwardEnable = false;
		else
			m_pHttxStatus->m_fForwardEnable = true;

		return true;
	}
	if (m_pChannel->m_IdleTimeoutS > 0 && CONTEXT.GetSystemTick() > m_pHttxStatus->m_tickRecvMsData+m_pChannel->m_IdleTimeoutS)
	{
		logwt<<"ͨ�����г�ʱ"<<m_pChannel->m_IdleTimeoutS<<"��,�Ͽ�����..."<<endl;
		return true;
	}

	if (m_pChannel->NeedPPP() == true)
	{
		if (m_pConnectStatus->m_Httx1OnPpp == true && m_pConnectStatus->m_PppOnline == false)
		{
			logwt<<"PPP������,�Ͽ�����..."<<endl;
			return true;
		}

		if (FLOWCTRL.FlowCtrlEnable() == true && FLOWCTRL.GetLogonFailedTimes() >= FLOWCTRL.GetLogonRetryTimes())
		{
			logwt<<"FLOWCTRL:��½ʧ��["<<FLOWCTRL.GetLogonRetryTimes()<<"��],�Ͽ�����..."<<endl;
			FLOWCTRL.LogonRecord(true);
			FLOWCTRL.SocketConnectRecord(false);//��½ʧ��,����ʧ��
			return true;
		}
		
		if (FLOWCTRL.FlowCtrlEnable() == false && FLOWCTRL.GetLogonFailedTimes() > LOGON_FAILED_TIMES_MAX)
		{
			logwt<<"��½ʧ��["<<LOGON_FAILED_TIMES_MAX<<"��],�Ͽ�����..."<<endl;
			FLOWCTRL.LogonRecord(true);
			return true;
		}
	}
	else
	{// LAN
		if (FLOWCTRL.GetLogonFailedTimes() > LOGON_FAILED_TIMES_MAX)
		{
			logwt<<"��½ʧ��["<<LOGON_FAILED_TIMES_MAX<<"��],�Ͽ�����..."<<endl;
			FLOWCTRL.LogonRecord(true);
			return true;
		}
	}
	S_SYSCFG *pSysCfg = STATUS.GetSystemConfig();
	if (FLOWCTRL.GetHeartBeatFailedTimes() >= pSysCfg->m_HeartBeatRetryTimes)
	{
		logwt<<"����ʧ��["<<FLOWCTRL.GetHeartBeatFailedTimes()<<"��],�Ͽ�����..."<<endl;
		FLOWCTRL.HeartBeatRecord(true);
		return true;
	}
	return false;
}

bool C_HttxServer::LinkEthernet(void)
{
	S_LcdStatus *pLcdStatus = CONTEXT.GetLcdStatus();
	if (pLcdStatus->m_Eth0LinePlugin == false) // ����û�в���
		return false;

	S_PppStatus *pPppStatus = CONTEXT.GetPppStatus();
	if (pPppStatus->m_fGprsDialing == true) // ���ڲ��ţ�������̫������
		return false;

	if (m_pMStationConnect->Connect() == 0)
		return true;
	return false;
}

int C_HttxServer::WaitPppDial(void)
{
#define HTTX_WAIT_PPP_TIMEOUT_S		90
	m_pConnectStatus->m_HttxNeedPPP[m_HttxSN] = true;
	logit<<"�ȴ�PPP����......"<<endl;
	INT32U timeoutS = 0;
	while (m_pConnectStatus->m_PppOnline == false)
	{
		if (m_pChannel->NeedPPP() == false)///��ֹ�ڱ��ز�������Ϊno ppp
			return 0;
		if (LinkEthernet() == true)
			return 0;
		HeartBeat(), SleepSecond(1);
		if ((++timeoutS % 60) == 0)
			logit<<"�����ȴ�......"<<endl;
		if (timeoutS > HTTX_WAIT_PPP_TIMEOUT_S) //	logit<<"�л�ͨ������..."<<endl;
		{
			m_pConnectStatus->m_HttxNeedPPP[m_HttxSN] = false; // ֪ͨ�ز�
			logit<<"�ȴ�PPP ["<<HTTX_WAIT_PPP_TIMEOUT_S<<"��]ʧ��,֪ͨ�ز�..."<<endl;
			SleepSecond(1);
			return -1;
		}
	}
	if (m_HttxSN == MP_HTTX_1)
		m_pConnectStatus->m_Httx1OnPpp = true;
	logit<<"PPP ���ųɹ�!"<<endl;
	return 0;
}

int C_HttxServer::ConnectMasterStation(void)
{
	if (m_pChannel->m_Type == CHANNEL_TYPE_PSTN)
		return ConnectMasterStation_Pstn();
	return ConnectMasterStation_Normal();
}

int C_HttxServer::ConnectMasterStation_Pstn(void)
{
	logit<<"PSTNͨ������������..."<<endl;
	while (true)
	{
		if (m_pMStationConnect->Connect() == 0)
			return 0;
		HeartBeat(), SleepMS(50);
	}
	return -1;
}

int C_HttxServer::ConnectMasterStation_Normal(void)
{
	if (FLOWCTRL.FlowCtrlEnable() == true && m_pChannel->NeedPPP() == true)
	{
		while (FLOWCTRL.GetSocketConnectFailedTimes() < FLOWCTRL.GetSocketConnectRetryTimes())
		{
			logwt<<"FLOWCTRL:��"<<FLOWCTRL.GetSocketConnectFailedTimes()+1<<"������......"<<endl;
			if (m_pMStationConnect->Connect() == 0)
				return 0;
			FLOWCTRL.SocketConnectRecord(false);
			logwt<<"FLOWCTRL:������վʧ��,˯��"<<FLOWCTRL.GetSocketConnectInterSecond()<<"��......"<<endl;
			for (INT32U Second = 0; Second < FLOWCTRL.GetSocketConnectInterSecond(); Second++)
				HeartBeat(), SleepSecond(1);
		}
		if (FLOWCTRL.GetSocketConnectFailedTimes() >= FLOWCTRL.GetSocketConnectRetryTimes())
		{
			logwt<<"����ʧ�ܴ���="<<FLOWCTRL.GetSocketConnectFailedTimes()<<",֪ͨPPP���²���......"<<endl;
			m_pConnectStatus->m_HttxNeedPPP[m_HttxSN] = false;
			FLOWCTRL.SocketConnectRecord(true);
			SleepSecond(5);//make sure PPP redial
		}
	}
	else
	{
		for (int i = 0; i < CONNECT_RETRY_TIMES_MAX;)
		{
			logit<<"��"<<i+1<<"������......"<<endl;
			int ok = m_pMStationConnect->Connect();
			if (ok == 0)
			{
				return 0;
			}
			else if (ok > 0)
			{
				logit<<"δ����,��������ʱ�仹��"<<ok<<"��..."<<endl;
				for (int j = 0; j < ok; j++)
					HeartBeat(), SleepSecond(1);
			}
			else
			{
				i++;
				logit<<"����ʧ��..."<<endl;
			}
		}
		if (m_pMStationConnect->IsConnected() == false)
		{
			if (m_pChannel->NeedPPP() == true)
			{
				logwt<<"����ʧ�ܴ���="<<CONNECT_RETRY_TIMES_MAX<<",֪ͨPPP���²���......"<<endl;
				m_pConnectStatus->m_HttxNeedPPP[m_HttxSN] = false;
				SleepSecond(5);//make sure PPP redial
			}
		}
	}
	return -1;
}

int C_HttxServer::MStationSend(const string &strCmd)
{
	if (m_pHttxStatus->m_fForwardEnable == false)//��ֹ������վ����
		return 0;//�����ն˵�HTTX1�п���Ϊfalse

	int Cnt = m_pMStationConnect->SendData(strCmd);
	if (Cnt < 0)
	{
		logwt<<m_pChannel->m_Name<<":��������:"<<HEX<<strCmd<<"ʱ���ӳ���!!!"<<endl;
		m_pMStationConnect->Disconnect();
	}
	else if (Cnt > 0)
	{
		if (m_pHttxStatus->m_fMasterTerm == false && m_HttxSN == MP_HTTX_1)
			m_pHttxStatus->m_fForwardEnable = false;//���ն˵�HTTX1������һ����false,�ȼ�������֡
		if (m_HttxSN == MP_HTTX_1)
			m_pConnectStatus->m_Httx1CommTime = C_TIME::TimeNow();
		if (m_HttxSN == MP_HTTX_1 && m_pChannel->NeedPPP() == true)
			STATUS.AddCommFlux((S_DWORD)Cnt);///PPP����ͳ��
	}

	return Cnt;
}

int C_HttxServer::MStationRecv(string &strCmd)
{
	int Cnt = m_pMStationConnect->RecvData(strCmd);
	if (Cnt < 0)
	{
		logwt<<m_pChannel->m_Name<<":��������ʱ���ӳ���!!!"<<endl;
		m_pMStationConnect->Disconnect();
	}
	else if (Cnt > 0)
	{
		if (m_HttxSN == MP_HTTX_1)
			m_pConnectStatus->m_Httx1CommTime = C_TIME::TimeNow();
		if (m_HttxSN == MP_HTTX_1 && m_pChannel->NeedPPP() == true)
			STATUS.AddCommFlux((S_DWORD)Cnt);///PPP����ͳ��
	}

	return Cnt;
}

int C_HttxServer::MsCmdCount(void)
{
	if (m_pHttxStatus->m_ResponseCmdCnt != m_strResponseCmds.size())
		m_pHttxStatus->m_ResponseCmdCnt = (S_WORD)m_strResponseCmds.size();
	if (m_pHttxStatus->m_ReportCmdCnt != m_ReportBuffer.m_strReportCmds.size())
		m_pHttxStatus->m_ReportCmdCnt = (S_WORD)m_ReportBuffer.m_strReportCmds.size();
	return 0;
}

bool C_HttxServer::CheckProgramKey(void)
{
	if (m_pChannel == NULL)
		return true;
	if (m_pChannel->m_Type != CHANNEL_TYPE_COM)
		return true;

	S_SYSCFG *pSysCfg = STATUS.GetSystemConfig();
	if (pSysCfg->m_ProgramKeyPort.test(m_pChannel->m_PortSet.m_ComSet.m_PortNO) == false)
		return true;
	if (CONTEXT.CheckProgramkeyState(true) == false)
	{
		logwt<<"�밴�±�̼�..."<<endl;
		return false;
	}
	return true;
}

bool C_HttxServer::ResponseCmdSend(void)
{
	if (m_strResponseCmds.empty() == true)
		return false;

	do{
		if (MStationSend(*m_strResponseCmds.begin()) <= 0)
			break;
		SleepMS(100);
		m_strResponseCmds.erase(m_strResponseCmds.begin());
	}while (!m_strResponseCmds.empty());

	MsCmdCount();
	return true;
}

bool C_HttxServer::ReportCmdSend(void)
{
	if (m_ReportBuffer.m_strReportCmds.empty() == true)
		return false;

	if (m_ReportBuffer.m_tickSendCmd == 0)
		;//
	else if (CONTEXT.GetSystemTick() - m_ReportBuffer.m_tickSendCmd < m_ReportBuffer.m_IntervalS)
		return false;//�ȴ���վ��Ӧ

	if (m_ReportBuffer.m_Retry < m_ReportBuffer.m_RetryMax)
	{
		if (MStationSend(*m_ReportBuffer.m_strReportCmds.begin()) <= 0)
			return false;//
		if (m_ReportBuffer.m_ConfirmEnable == true)
		{
			m_ReportBuffer.m_tickSendCmd = CONTEXT.GetSystemTick();
			m_ReportBuffer.m_Retry++;
		}
		else
		{
			m_ReportBuffer.m_strReportCmds.erase(m_ReportBuffer.m_strReportCmds.begin());
		}
	}
	else
	{
		ReportConfirm(false);
	}

	MsCmdCount();
	return true;
}

bool C_HttxServer::ForwardChannelEnable(void)
{
	if (C_SYSCTRL::GetProcessEnable(MP_PROCESS_DBCJ2) == true) // ˵���ɱ������ʹ�����ͨ��
		return false;
	return true;
}

/**@brief ȷ��ͨ��ģʽ
 *  @return true:modem;false:ethernet
 */
bool C_HttxServer::CheckCommunicationMode(const S_CHANNEL *pChannel)
{
	char Buffer[32];
	pChannel->m_PortSet.m_NetSet.GetIpString(Buffer);
	if (C_BaseFn::CheckRouterOfTcpMode(NET_ETH0_DEV_NAME, Buffer) == true)
		return false;
	return true;
}

//
//
//
C_Httx1Server::C_Httx1Server(void)
{
	m_HttxSN = MP_HTTX_1;
}

C_Httx1Server::~C_Httx1Server()
{
}

C_Httx2Server::C_Httx2Server(void)
{
	m_HttxSN = MP_HTTX_2;
}

C_Httx2Server::~C_Httx2Server()
{
};

C_Httx3Server::C_Httx3Server(void)
{
	m_HttxSN = MP_HTTX_3;
}

C_Httx3Server::~C_Httx3Server()
{
};

C_Httx4Server::C_Httx4Server(void)
{
	m_HttxSN = MP_HTTX_4;
}

C_Httx4Server::~C_Httx4Server()
{
};


