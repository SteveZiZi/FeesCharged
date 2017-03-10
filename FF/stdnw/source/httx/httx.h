/** @file
 *  @brief 后台通讯服务功能定义
 *  @author 
 *  @date 2011/11/30
 *  @version 0.1
 */
#ifndef __HTTX_SERVER_H__
#define __HTTX_SERVER_H__
#include "server.h"
#include "basetype.h"
#include "report.h"
#include "connect.h"
#include "sysbase.h"
#include "forward.h"

class C_HttxServer:public C_SERVER
{
public:
	C_HttxServer(void);
	virtual ~C_HttxServer();

public:
	virtual int BeforeService(void);
	virtual int AfterService(void);
	virtual int OnTickService(void);

protected:
	int ChannelServer(void);

	int CmdProcess(void);
	int ForwardProcess(void);
	int DataReport(void);
	int DataTransfer(void);
	int OffLineCheck(void);

	int GetMsCmd(string &strCmd){return m_pMsRule->ExtractCmd(m_strRecvMsBuffer, strCmd);}
	int CheckMsCmdAddr(const string &strCmd){return m_pMsRule->CheckDeviceAddr(strCmd);}
	int MsCmdHandle(const string &strCmd, strings &strResponseInfos);
	int ReportConfirm(bool fConfirm);
	bool NeedOffLine(void);
	bool LinkEthernet(void);
	int WaitPppDial(void);
	int ConnectMasterStation(void);
	int ConnectMasterStation_Pstn(void);
	int ConnectMasterStation_Normal(void);

	int MStationSend(const string &strCmd);
	int MStationRecv(string &strCmd);
	int MsCmdCount(void);
	bool CheckProgramKey(void);
	bool ResponseCmdSend(void);
	bool ReportCmdSend(void);
	bool ForwardChannelEnable(void);
	/**@brief 确认通信模式
	 *  @return true:modem;false:ethernet
	 */
	bool CheckCommunicationMode(const S_CHANNEL *pChannel);

protected:
	S_HttxSN m_HttxSN;
	S_CHANNEL *m_pChannel;
	C_CONNECT *m_pMStationConnect;
	C_CONNECT *m_pForwardConnect;
	C_MSRULE* m_pMsRule;

	S_ConnectStatus *m_pConnectStatus;
	S_HttxStatus *m_pHttxStatus;

	C_ReportHandle *m_pReportHandle;

	string m_strRecvMsBuffer;
	strings m_strResponseCmds;
	S_ReportBuffer m_ReportBuffer;
	C_ForwardContext m_ForwardContext;
};

class C_Httx1Server:public C_HttxServer
{
public:
	C_Httx1Server(void);
	virtual ~C_Httx1Server();
};

class C_Httx2Server:public C_HttxServer
{
public:
	C_Httx2Server(void);
	virtual ~C_Httx2Server();
};

class C_Httx3Server:public C_HttxServer
{
public:
	C_Httx3Server(void);
	virtual ~C_Httx3Server();
};

class C_Httx4Server:public C_HttxServer
{
public:
	C_Httx4Server(void);
	virtual ~C_Httx4Server();
};


#endif//__HTTX_SERVER_H__

