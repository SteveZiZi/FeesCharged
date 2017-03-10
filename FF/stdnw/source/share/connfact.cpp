/** @file
 *  @brief 连接工厂定义
 *  @author 
 *  @date 2011/12/06
 *  @version 0.1
 */
#include "connfact.h"
#include "htrace.h"
#include "conn_com.h"
#include "conn_net.h"
#include "conn_pstn.h"
#include "sysbase.h"
#include "status.h"
#include "context.h"

/** 钩子函数 */
void Connect_ReadHOOK(S_CHANNEL *pChannel, const string &strData)
{
	if (!strData.empty())
		logvt<<pChannel->m_Name<<" recv:"<<HEX<<strData<<endl;
}
void Connect_WritHOOK(S_CHANNEL *pChannel, const string &strData)
{
	if (!strData.empty())
		logvt<<pChannel->m_Name<<" send:"<<HEX<<strData<<endl;
}
void Connect_ErrorOut(bool fWarnLevel, const string &strErrorInfo)
{
	if (!strErrorInfo.empty())
	{
		if (fWarnLevel == true) logwt<<strErrorInfo<<endl;
		else logit<<strErrorInfo<<endl;
	}
}
void Connect_ReadHOOK_COM(S_CHANNEL *pChannel, const string &strData)
{
	S_BYTE PortNo = pChannel->m_PortSet.m_ComSet.m_PortNO;
	if (!strData.empty() && PortNo < SYSTEM_SERIAL_PORT_MAX)
	{
		S_LcdStatus *pLcdStatus = CONTEXT.GetLcdStatus();
		pLcdStatus->m_timeSerialComm[PortNo] = C_TIME::TimeNow();
	}
	Connect_ReadHOOK(pChannel, strData);
}
void Connect_WritHOOK_COM(S_CHANNEL *pChannel, const string &strData)
{
	S_BYTE PortNo = pChannel->m_PortSet.m_ComSet.m_PortNO;
	if (!strData.empty() && PortNo < SYSTEM_SERIAL_PORT_MAX)
	{
		S_LcdStatus *pLcdStatus = CONTEXT.GetLcdStatus();
		pLcdStatus->m_timeSerialComm[PortNo] = C_TIME::TimeNow();
	}
	Connect_WritHOOK(pChannel, strData);
}

C_CONNECT* C_ConnFact::CreateConnection(S_CHANNEL& channel)
{
	S_SYSCFG *pSysCfg = STATUS.GetSystemConfig();
	C_COMCONN *pCom = NULL;
	C_NETCONN *pNet = NULL;
	C_PSTNCONN *pPstn = NULL;
	switch(channel.m_Type)
	{
	case CHANNEL_TYPE_COM:
		pCom = new C_COMCONN(channel);
		pCom->SetReadHook(Connect_ReadHOOK_COM);
		pCom->SetWritHook(Connect_WritHOOK_COM);
		pCom->SetErrorHook(Connect_ErrorOut);
		pCom->SetPortMap(pSysCfg->m_PortMap);
		return pCom;
	case CHANNEL_TYPE_NET:
		pNet = new C_NETCONN(channel);
		pNet->SetReadHook(Connect_ReadHOOK);
		pNet->SetWritHook(Connect_WritHOOK);
		pNet->SetErrorHook(Connect_ErrorOut);
		return pNet;
	case CHANNEL_TYPE_PSTN:
		pPstn = new C_PSTNCONN(channel);
		pPstn->SetReadHook(Connect_ReadHOOK);
		pPstn->SetWritHook(Connect_WritHOOK);
		pPstn->SetErrorHook(Connect_ErrorOut);
		pPstn->SetPortMap(pSysCfg->m_PortMap);
		return pPstn;
	default:break;
	}
	return NULL;
}

void C_ConnFact::DestroyConnection(C_CONNECT *pConnect)
{
	delete pConnect;
}




