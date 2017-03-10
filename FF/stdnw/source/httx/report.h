/** @file
 *  @brief 后台通讯服务任务上报功能定义
 *  @author 
 *  @date 2011/11/30
 *  @version 0.1
 */
#ifndef __HTTX_REPORT_H__
#define __HTTX_REPORT_H__
#include "nwreport.h"
#include "timeop.h"
#include "sysbase.h"

enum
{
	RTID_LOGIN = RTID_USER,
	RTID_HB,
	RTID_ALARM,
	RTID_NORMAL,
	RTID_DATAROUTE,
	RTID_AGAIN,
	RTID_HISTORY,


	RTID_MAX
};
class C_ReportLogin:public C_ReportHandle
{
public:
	C_ReportLogin(void){m_tickLastSendLoginCmd = 0;}
	
protected:
	virtual void init(void){m_fPrompt = true;m_tickLastSendLoginCmd = 0;}
	virtual int GetReportTaskID(void);
	virtual int ReportOneRecord(bool fok);
	virtual int ReportAllRecord(bool fok);
	virtual bool NeedReport(void);
	virtual bool BuildupReportData(S_ReportBuffer &ReportCmds);

protected:
	bool m_fPrompt;
	S_TIME m_tickLastSendLoginCmd;
};

class C_ReportHB:public C_ReportHandle
{
public:
	C_ReportHB(void){m_tickLastHeartBeatTime = 0;}
	
protected:
	virtual void init(void){m_tickLastHeartBeatTime = 0;}
	virtual int GetReportTaskID(void);
	virtual int ReportOneRecord(bool fok);
	virtual int ReportAllRecord(bool fok);
	virtual bool NeedReport(void);
	virtual bool BuildupReportData(S_ReportBuffer &ReportCmds);

protected:
	S_TIME m_tickLastHeartBeatTime;
};

class C_ReportAlarmT:public C_ReportHandle
{
public:
	C_ReportAlarmT(void){}
	
protected:
	virtual void init(void){}
	virtual int GetReportTaskID(void);
	virtual int ReportOneRecord(bool fok);
	virtual int ReportAllRecord(bool fok);
	virtual bool NeedReport(void);
	virtual bool BuildupReportData(S_ReportBuffer &ReportCmds);
};

class C_ReportNormalT:public C_ReportHandle
{
public:
	C_ReportNormalT(void){}
	
protected:
	virtual void init(void){m_NormalTaskID = -1;}
	virtual int GetReportTaskID(void);
	virtual int ReportOneRecord(bool fok);
	virtual int ReportAllRecord(bool fok);
	virtual bool NeedReport(void);
	virtual bool BuildupReportData(S_ReportBuffer &ReportCmds);

protected:
	bool NormalTaskRaise(void);
	bool NormalTaskRaise(const S_NormalTaskInfo *pTaskInfo, S_NormalTaskStatus *pTaskStatus, S_TIME timeRefer);
	S_TIME TrimReportTime(const S_NormalTaskInfo *pTaskInfo, S_TIME timeRefer);
	void UpdateReportTime(void);
	bool HistoryDayTask(const S_NormalTaskInfo *pTaskInfo);
	bool InsetHistoryTask(int NormalTaskID, S_HistoryTask &HistoryTask);
	S_HistoryTask *GetHistoryTaskMin(S_HistoryTasks &HistoryTasks);

protected:
	int m_NormalTaskID;
	S_HistoryTask m_HistoryTask;///<信息点记录,目前仅记忆上报失败的表计
};

class C_ReportDataRouteT:public C_ReportHandle
{
public:
	C_ReportDataRouteT(void){}
	
protected:
	virtual void init(void){m_ForwardTaskID = -1;}
	virtual int GetReportTaskID(void);
	virtual int ReportOneRecord(bool fok);
	virtual int ReportAllRecord(bool fok);
	virtual bool NeedReport(void);
	virtual bool BuildupReportData(S_ReportBuffer &ReportCmds);

protected:
	bool ForwardTaskRaise(void);
	bool ForwardTaskRaise(const S_ForwardTaskInfo *pTaskInfo, S_ForwardTaskStatus *pTaskStatus, S_TIME timeRefer);
	S_TIME TrimReportTime(const S_ForwardTaskInfo *pTaskInfo, S_TIME timeRefer);
	void UpdateReportTime(void);
	bool GetForwardTaskData(int ForwardTaskID,const S_ForwardTaskInfo *pTaskInfo,S_ForwardData *pForwardData,strings &strCmdInfos);

protected:
	int m_ForwardTaskID;
};

class C_ReportAgainT:public C_ReportHandle
{
public:
	C_ReportAgainT(void){}
	
protected:
	virtual void init(void){memset(m_timeLastReport, 0x00, sizeof(m_timeLastReport));}
	virtual int GetReportTaskID(void);
	virtual int ReportOneRecord(bool fok);
	virtual int ReportAllRecord(bool fok);
	virtual bool NeedReport(void);
	virtual bool BuildupReportData(S_ReportBuffer &ReportCmds);

protected:
	bool HistoryTaskRaise(void);
	bool HistoryTaskRaise(const S_NormalTaskInfo *pTaskInfo, S_HistoryTask *pHistoryTask, S_TIME timeRefer);
	bool HistoryTaskRaise(int NormalTaskID, S_TIME timeRefer);
	bool HistoryDayTask(const S_NormalTaskInfo *pTaskInfo);
	int GetHistoryTaskMax(S_HistoryTasks &HistoryTasks);

protected:
	int m_NormalTaskID;
	int m_HistoryTaskID;
	S_TIME m_timeLastReport[NORMAL_TASK_MAX];
	S_HistoryTask m_HistoryTask;///<信息点记录,目前仅记忆上报失败的表计
};

class C_ReportHistoryT:public C_ReportHandle
{
public:
	C_ReportHistoryT(void){}
	
protected:
	virtual void init(void){}
	virtual int GetReportTaskID(void);
	virtual int ReportOneRecord(bool fok);
	virtual int ReportAllRecord(bool fok);
	virtual bool NeedReport(void);
	virtual bool BuildupReportData(S_ReportBuffer &ReportCmds);

protected:
	bool HistoryTaskRaise(void);
	bool HistoryTaskRaise(const S_NormalTaskInfo *pTaskInfo, S_HistoryTask *pHistoryTask, S_TIME timeRefer);
	bool HistoryDayTask(const S_NormalTaskInfo *pTaskInfo);

protected:
	int m_NormalTaskID;
	int m_HistoryTaskID;
	S_HistoryTask m_HistoryTask;///<信息点记录,目前仅记忆上报失败的表计
};


//
//
//
class C_ReportFact:public C_ReportFactBase
{
private:
	virtual C_ReportHandle *GetReportHandle(int ReportID);
};


#endif//__HTTX_REPORT_H__

