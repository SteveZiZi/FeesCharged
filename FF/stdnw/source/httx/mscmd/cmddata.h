/** @file
 *  @brief 后台通讯命令数据读写服务定义
 *  @author 
 *  @date 2011/12/22
 *  @version 0.1
 */
#ifndef __NW_CMDDATA_H__
#define __NW_CMDDATA_H__
#include "nwmscmd.h"
#include "nwbtype.h"
#include "timeop.h"
#include "sysbase.h"


//
//读当前数据
//
class C_CmdRealtime:public C_CmdReceiver //读采集量
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读当前数据(采集量)";}
	bool GetCurZXYG();
protected:
	bool FillData(const S_CmdObj &CmdObj, string &strData);
private:
	string strIpcData;	
};
class C_CmdRealE1800001:public C_CmdReceiver //	NNNN	2	 	*		终端状态字，参见C.5.1
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读终端状态字";}
};
class C_CmdRealE1800002:public C_CmdReceiver //	NN	1	 	*		最大脉冲量路数
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读最大脉冲量路数";}
};
class C_CmdRealE1800003:public C_CmdReceiver //	NN	1	 	*		串行通信端口数量
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读串行通信端口数量";}
};
class C_CmdRealE1800004:public C_CmdReceiver //	NNNN	2	 	*		支持最大测量点数
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读支持最大测量点数";}
};
class C_CmdRealE1800005:public C_CmdReceiver //	NNNN	2	 	*		测量点总数,所有有效的测量点总数
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读有效的测量点总数";}
};
class C_CmdRealE1800006:public C_CmdReceiver //	NNNN	2	 	*		终端普通用户表总数 
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读终端普通用户表总数";}
};
class C_CmdRealE1800007:public C_CmdReceiver //	NNNN	2	 	*		终端多功能表总数
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读终端多功能表总数";}
};
class C_CmdRealE1800008:public C_CmdReceiver //	NNNN	2	 	*		终端重点户电能表总数
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读终端重点户电能表总数";}
};
class C_CmdRealE1800009:public C_CmdReceiver //	NNNN,MMMM,YYMMDDhhmmss1,YYMMDDhhmmss2,	*		终端抄表状态信息
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读终端抄表状态信息";}
protected:
	int GetTermGatherStatus(S_WORD &MeterTotal, S_WORD &MeterSuccess, S_TIME &timeGatherBegin, S_TIME &timeGatherEnd);
};
class C_CmdRealE1800010:public C_CmdReceiver //	0SNN.NN	3	 	*		本日线损率
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读本日线损率";}
};
class C_CmdRealE1800011:public C_CmdReceiver //	NNNN	2	kB	*		(当前)日通信流量
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读(当前)日通信流量";}
};
class C_CmdRealE1800013:public C_CmdReceiver //	NNNNNN	3	kB	*		(当前)月通信流量
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读(当前)月通信流量";}
};
class C_CmdRealE1800015:public C_CmdReceiver //	NN	1		*		信号强度(0~31)
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读信号强度(0~31)";}
};
class C_CmdRealE1800016:public C_CmdReceiver //	NNNN	2		*		(当前)月终端复位次数
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读(当前)月终端复位次数";}
};
class C_CmdRealE1800017:public C_CmdReceiver //	NNNN	2		*		(冻结)月终端复位次数
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读(冻结)月终端复位次数";}
};
class C_CmdRealE1800018:public C_CmdReceiver //	NNNN	2		*		载波模块程序版本号
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读载波模块程序版本号";}
};

class C_CmdRealE1800019:public C_CmdReceiver //	NNNN	2		*		最大需量清零次数
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读最大需量清零次数";}
};

class C_CmdRealE1800020:public C_CmdReceiver //	NN.....NN	2*20		*		重点户电能表测量点号(ffff为无效)
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读重点户电能表测量点号";}
};

class C_AFNCMD_REALTIME:public C_AFNCMD
{
protected:
	virtual C_CmdReceiver *GetCmdReceiver(INT32U DI);
};

//
//读历史数据
//
class C_CmdHistory:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读历史数据";}
protected:
	S_TIME GetNextTime(S_TIME timeData, S_CHAR Density);
	S_TIME DateTrim(S_TIME timeData, S_CHAR Density);
	int MakeReadCmdObj(S_WORD DA, S_MPBS &MPBS);
};

class C_AFNCMD_HISTORY:public C_AFNCMD
{
protected:
	virtual C_CmdReceiver *GetCmdReceiver(INT32U DI);
};

//读任务数据
class C_CmdTask:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读任务数据";}
	//主动上报任务
	int GetReportData(int TaskID, const S_NormalTaskInfo *pInfo, S_HistoryTask *pHistoryTask, strings &strCmdInfos);

protected:
	S_TIME GetNextSampleTime(const S_NormalTaskInfo *pInfo, S_TIME timeData, S_DataDensity Density);
	S_TIME GetNextSampleTime(const S_NormalTaskInfo *pInfo, S_TIME timeData);
	S_TIME TrimSampleTime(const S_NormalTaskInfo *pInfo, S_TIME timeData);
	S_TIME TrimSampleTime(S_DataDensity Density, S_TIME timeData);
	S_TIME GetDataSampleTime(const S_NormalTaskInfo *pInfo, S_TIME timeData, S_DataDensity Density, S_DWORD DI);
	S_TIME GetDataSampleTime(const S_NormalTaskInfo *pInfo, S_TIME timeData, S_DWORD DI);
	int GetNormalTaskData(const S_NormalTaskInfo *pInfo, S_TIME timeData, strings &strDatas, S_MPBS &mpbsFailed, S_DataDensity Density);
	int GetNormalTaskData_UserDefine(const S_NormalTaskInfo *pInfo, S_TIME timeData, strings &strDatas, S_MPBS &mpbsFailed, S_DataDensity Density);//表示自描述格式组织数据
	int GetNormalTaskData_TaskDefine(const S_NormalTaskInfo *pInfo, S_TIME timeData, strings &strDatas, S_MPBS &mpbsFailed, S_DataDensity Density);//表示按任务定义的数据格式组织数据
	bool TrimCmdInfo(S_BYTE type, S_WORD DaCnt, S_WORD DtCnt, const strings &strSrcs, strings &strDsts);
	bool TrimCmdInfo(const strings &strSrcs, strings &strDsts);
	bool MakeMeasurePointBitset(const S_NormalTaskInfo *pInfo, S_MPBS &MPBS);
};
class C_AFNCMD_TASK:public C_AFNCMD
{
protected:
	virtual C_CmdReceiver *GetCmdReceiver(INT32U DI);
};

//读告警数据
class C_CmdAlarm:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读告警数据";}
};
class C_AFNCMD_ALARM:public C_AFNCMD
{
protected:
	virtual C_CmdReceiver *GetCmdReceiver(INT32U DI);
};

//读事件数据
class C_CmdEvent:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读事件数据";}
};
class C_AFNCMD_EVENT:public C_AFNCMD
{
protected:
	virtual C_CmdReceiver *GetCmdReceiver(INT32U DI);
};

//文件传输
class C_AFNCMD_FTP:public C_AFNCMD
{
protected:
	virtual C_CmdReceiver *GetCmdReceiver(INT32U DI);
};

//级联(从终端处理)
class C_CmdE0000930:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "主终端轮询命令";}
};
class C_CmdE0000931:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "主终端传输控制命令";}
};
class C_CmdE0000933:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "抄读终端台区总电量";}
};
class C_AFNCMD_FORWARD:public C_AFNCMD
{
public:
	virtual int Execute(const S_CmdObjs &CmdObjs, strings &strResponseInfos);
protected:
	virtual C_CmdReceiver *GetCmdReceiver(INT32U DI);
};

//级联(主终端处理)
class C_CmdMsE0000930:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "从终端轮询命令回复";}
};
class C_CmdMsE0000932:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "从终端传输控制命令回复";}
};
class C_CmdMsE0000933:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "终端台区总电量回复";}
};
class C_AFNCMD_FORWARD_MS:public C_AFNCMD
{
public:
	virtual int Execute(const S_CmdObjs &CmdObjs, strings &strResponseInfos);
protected:
	virtual C_CmdReceiver *GetCmdReceiver(INT32U DI);
};

//中继转发
class C_CmdRoute:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "中继转发";}
};
class C_AFNCMD_ROUTE:public C_AFNCMD
{
protected:
	virtual C_CmdReceiver *GetCmdReceiver(INT32U DI);
};


#endif //__NW_CMDDATA_H__

