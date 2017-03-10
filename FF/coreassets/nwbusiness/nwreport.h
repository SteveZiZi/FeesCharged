/** @file
 *  @brief ���������ϱ���ܶ���
 *  @author 
 *  @date 2011/12/22
 *  @version 0.1
 */
#ifndef __NWB_REPORT_BASE_H__
#define __NWB_REPORT_BASE_H__
#include "timeop.h"
#include "strings.h"
#include "basetype.h"
#include "nwmsrule.h"

/** @brief �ϱ����� */
struct S_ReportBuffer
{
	bool m_ConfirmEnable;
	S_WORD m_Retry;
	S_WORD m_RetryMax;
	S_WORD m_IntervalS;
	S_TIME m_tickSendCmd;
	strings m_strReportCmds;
};

/** @brief Report task ID:must order
 *  @remarks �û��ĵ�һ���ϱ�ID�������RTID_USER��������˳���1
 */
enum
{
	RTID_NONE,
	RTID_USER
};

class C_ReportHandle
{
public:
	C_ReportHandle(void){m_HttxSN=MP_HTTX_1, m_pRule=NULL, m_pNext=NULL;}
	virtual ~C_ReportHandle(){}

public:
	C_ReportHandle *SetNextHandle(C_ReportHandle *pNext){C_ReportHandle *pBackup = m_pNext;m_pNext=pNext;return pBackup;}
	int InitInfo(S_HttxSN HttxSN, C_MSRULE *pRule);
	int UpdateInfo_ReportOne(bool fok);
	int UpdateInfo_ReportAll(bool fok);
	int GetReportData(S_ReportBuffer &ReportCmds);

protected:
	virtual void init(void){}
	virtual int GetReportTaskID(void) = 0;
	virtual int ReportOneRecord(bool fok) = 0;
	virtual int ReportAllRecord(bool fok) = 0;
	virtual bool NeedReport(void) = 0;
	virtual bool BuildupReportData(S_ReportBuffer &ReportCmds) = 0;
	
protected:
	S_HttxSN m_HttxSN;
	C_MSRULE* m_pRule;

private:
	static int m_ReportTaskID;
	C_ReportHandle *m_pNext;
};

class C_ReportFactBase
{
public:
	C_ReportHandle *GetReportChain(void);

protected:
	virtual C_ReportHandle *GetReportHandle(int ReportID);
};

#endif//__NWB_REPORT_BASE_H__


