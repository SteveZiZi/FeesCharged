/** @file
 *  @brief 费控交互终端与射频模块通信协议
 *  @author 
 *  @date 2016/07/27
 *  @version 0.1
 */
#ifndef __RFTX_SERVER_H__
#define __RFTX_SERVER_H__

#include "server.h"
#include "connfact.h"
#include "dnmodule.h"
#include "metercom.h"
////#include "gwsched.h"
#include "sysbase.h"
#include "rfidMgr.h"
#include "meterFrameMgr.h"

#include <string>
#include <vector>

///////
enum
{
	RFTX_STATUS_INIT,
	RFTX_STATUS_SYNC_STATUS,
	RFTX_STATUS_RUNNING,
	RFTX_STATUS_MAX
};

class C_RFtxServer:public C_SERVER
{
public:
	C_RFtxServer(void);
	~C_RFtxServer(void);
	
	virtual int BeforeService(void);
	virtual int AfterService(void);
	virtual int OnTickService(void);
private:
	int InitRftxComm(void);
	int SyncRftxStatus(void);
	int RftxTaskRunning(void);
protected:
	int m_Status;	

public:
	int LongDistanceAccount();											// 远程开户
	int LoaclAccount();													// 本地开户
	int WalletCharge();													// 钱包充值
	int WalletInit();													// 初始化钱包
	int ParamUpdate();													// 参数更新
	int LongDistanceControl();											// 远程控制

private:
	int BackInfoProcess();												// 本地模式下 信息返写流程

private:
	static S_CHANNEL *m_pChannel;
	static C_CONNECT *m_pConnect; 

	S_CHAR				m_MeterMode;									// 电表模式
	C_RFIDMgr			m_RFIDMgr;
	C_MeterFrameMgr		m_MeterFrameMgr;
};


const char *hex2str(const S_BYTE* ab, S_WORD ab_len);
S_BYTE *hex_repr( const S_BYTE *ab, S_WORD ab_len, S_BYTE *buf, S_WORD buf_len);

#endif//__RFtx_SERVER_H__


