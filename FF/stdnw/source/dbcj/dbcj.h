/** @file
 *  @brief 表计能量采集功能定义
 *  @author 
 *  @date 2011/11/30
 *  @version 0.1
 */
#ifndef __DBCJ_SERVER_H__
#define __DBCJ_SERVER_H__
#include "server.h"
#include "connfact.h"
#include "dnmodule.h"
#include "metercom.h"
#include "gwsched.h"
#include "sysbase.h"

//
//
//
enum
{
	DBCJ_STATUS_INIT,
	DBCJ_STATUS_SYNC_STATUS,
	DBCJ_STATUS_CREATE_TASK,
	DBCJ_STATUS_RUNNING,

	DBCJ_STATUS_MAX
};

struct S_FreezeTask
{
	S_WORD m_RecordMax; ///< 存储点数	为0表示按最大存储容量存储
	S_DWORD m_DT;
	S_MPBS m_mpbsMeter;
};
struct S_FreezeTasks:public list<S_FreezeTask>
{
public:
	int AddTask(const S_FreezeTask &FreezeTask);
};

class C_DbcjServer:public C_SERVER
{
public:
	virtual int BeforeService(void);
	virtual int AfterService(void);
	virtual int OnTickService(void);

private:
	int InitGatherInfo(void);
	int SyncGatherStatus(void);
	int CreateAllTask(void);
	int TaskSchedule(void);

	int RefreshRouterInfo(const string &strAddress, const string &strInfo);
	bool CheckRealtimeTask(void);
	int CheckRunningStatus(void);

	int CreateRouteTask(void);
	int CreateRouteExTask(void);
	int CreateBroadcastTimeTask(void);
	int CreateSeekMeterTask(void);
	int CreatePollingTask(void);
	int CreateMeterTask(void);
	int CreateFixMeterTask(void);
	int CreateTermTask(void);
	int CreateAlarmTask(void);
	int CreateGroupSumTask(void);

	int CreateMeterFreezeTask(const S_FreezeTask &FreezeTask);
	int CreateFixMeterFreezeTask(const S_FreezeTask &FreezeTask, S_DWORD ClassDataPrefix);
	int FreshAlarmTaskEnable(void);
	int EnableGatherTask(bool fEnable);
	
	int StatMeterTask(S_FreezeTasks &FreezeTasks);
	int CheckJlcyMeter(S_MPBS &MPBS, S_ALLMP *pALLMP);
	int CountMyMeter(void);

	bool BroadcastTimeTaskEnable(int channel);
	bool SeekMeterNotify(S_DbcjStatus *pDbcjStatus);
	S_WORD SeekMeterAdd(int idxFB);
	int SeekMeterInfoUpdate(void);
	S_MeterInfo DefaultUpdateMeterInfo(S_BYTE PortNO);
	int UpdateMeterInfo(const S_MeterInfo &Meter, S_WORD &BaseMP, S_WORD &BaseSN);
	int DnModuleReset(C_CONNECT *pConnZBWX);

protected:
	int m_Status;
	S_DbcjSN m_DbcjSN;
	S_WORD m_DnModuleResetCnt;

	C_CONNECT *m_pConnRS485I;
	C_CONNECT *m_pConnRS485II;
	C_CONNECT *m_pConnZBWX;
	C_CONNECT *m_pConnJLCY;
	
	C_DnModule m_DnModule;
	C_MeterCom m_MeterCom;
	C_GwTaskSchedule m_TaskSchedule;
	string msg;
	S_BYTE MsgT;
};

//
//表计能量采集服务
//
class C_Dbcj1Server:public C_DbcjServer
{
public:
	virtual int BeforeService(void);
};

class C_Dbcj2Server:public C_DbcjServer
{
public:
	virtual int BeforeService(void);
};

class C_Dbcj3Server:public C_DbcjServer
{
public:
	virtual int BeforeService(void);
};

class C_Dbcj4Server:public C_DbcjServer
{
public:
	virtual int BeforeService(void);
};



#endif//__DBCJ_SERVER_H__

