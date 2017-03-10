/** @file
 *  @brief �������ܶ���
 *  @author 
 *  @date 2011/12/27
 *  @version 0.1
 */
#ifndef __HTTX_FORWARD_H__
#define __HTTX_FORWARD_H__
#include "basedef.h"
#include "connect.h"
#include "strings.h"
#include "nwmsrule.h"

//
//������������
//
class C_Forward
{
public:
	enum{FS_IDLE, FS_POLL, FS_TRANS, FS_GATHER};

public:
	C_Forward(void){}
	virtual ~C_Forward(){}

public:
	virtual void init(void) = 0;
	virtual C_Forward *ForwardProcess(C_CONNECT* pForward, strings &strMsCmds, string &strCmd) = 0;

protected:
	int ParseSlave(C_MSRULE *pRule, string &strCmd, string &strEnergy);

protected:
	static int m_WorkSlaveID;
};

class C_ForwardIdle:public C_Forward
{
public:
	C_ForwardIdle(void){memset(m_tickPoll, 0x00, sizeof(m_tickPoll));}

public:
	virtual void init(void);
	virtual C_Forward *ForwardProcess(C_CONNECT* pForward, strings &strMsCmds, string &strCmd);

protected:
	S_TIME m_tickPoll[NW_FORWARD_ADDR_MAX+1];
	string m_strRecvBuffer;
};

class C_ForwardPoll:public C_Forward
{
public:
	virtual void init(void);
	virtual C_Forward *ForwardProcess(C_CONNECT* pForward, strings &strMsCmds, string &strCmd);

protected:
	S_TIME m_tickSendCmd;
	string m_strRecvBuffer;
};

class C_ForwardTrans:public C_Forward
{
public:
	virtual void init(void);
	virtual C_Forward *ForwardProcess(C_CONNECT* pForward, strings &strMsCmds, string &strCmd);

protected:
	C_Forward *ForwardProcess_Ms2Slave(C_CONNECT* pForward, strings &strMsCmds, string &strCmd);
	C_Forward *ForwardProcess_SlaveReport(C_CONNECT* pForward, strings &strMsCmds, string &strCmd);

protected:
	S_TIME m_tickSendCmd;
	string m_strRecvBuffer;
};

class C_ForwardGather:public C_Forward
{
public:
	virtual void init(void);
	virtual C_Forward *ForwardProcess(C_CONNECT* pForward, strings &strMsCmds, string &strCmd);

protected:
	S_TIME m_tickSendCmd;
	string m_strRecvBuffer;
};

class C_ForwardFact
{
public:
	static C_Forward *GetForwardStatusObj(int status);
};

class C_ForwardContext
{
public:
	C_ForwardContext(void){m_pForwardStatus=NULL, m_pForwardConnect=NULL;}

public:
	/** @brief ����ģ���ʼ��
	 *  @param[in] pForwardConnect ͨ������
	 *  @return <0 ����; >=0 �ɹ�
	 */
	int Init(C_CONNECT* pForwardConnect);
	/** @brief ת����վ��Ϣ
	 *  @param[in] strCmd ��վ��Ϣ���������й�Լ��
	 *  @return <0 ����; >=0 �ɹ�
	 */
	int DeliverMsCmd(const string &strCmd);
	/** @brief ��������
	 *  @param[out] strCmd ���ն��ϴ�����Ϣ
	 *  @return <0 ����; =0 û����Ϣ; >0 ���ϴ���Ϣ
	 */
	int ForwardProcess(string &strCmd);

protected:
	C_Forward *m_pForwardStatus;
	C_CONNECT* m_pForwardConnect;
	strings m_strMsCmds;
};

#endif//__HTTX_FORWARD_H__

