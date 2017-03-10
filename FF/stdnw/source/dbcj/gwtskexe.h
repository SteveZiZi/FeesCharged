/** @file
 *  @brief 国网任务执行器
 *  @author 
 *  @date 2012/06/24
 *  @version 0.1
 */
#ifndef __GW_TASK_EXEC_H__
#define __GW_TASK_EXEC_H__
#include "tskexec.h"
#include "sysbase.h"

/** @class
 *  @brief 国网广播校时任务
 */
class C_GwTaskBroadcast:public C_TaskBroadcast
{
protected:
	/** @brief 应用层任务如心跳
	 *  @return 0
	 */
	virtual int TickServer(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);

	/** @brief 广播校时最大延时时间
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[in] FBID 现场总线ID号
	 *  @return 0:无效值; >0:有效延时时间
	 *  @note 解决东软三代模式下校时延时900秒问题
	 */
 	virtual S_DWORD GetMaxDelayS(const S_TaskInfo *pTaskInfo, S_WORD FBID);
};

/** @class
 *  @brief 国网路由任务
 */
class C_GwTaskRoute:public C_TaskRouteBase
{
protected:
	/** @brief 获取当前表计
	 *  @param[out] Meter485 采集器下的485表
	 *  @param[out] MeterZB 载波表或无线表
	 *  @param[out] Relay 采集器
	 *  @return 0:成功；>0:失败
	 */
	virtual int GetMeterAll(S_TERMINALS &Meter485, S_TERMINALS &MeterZB, S_TERMINALS &Relay);
	/** @brief 路由模块操作失败消息 */
	virtual int SendRouterErrorMessage(int ErrorInfo);
};

/** @class
 *  @brief 国网实时任务
 */
class C_GwTaskRealtime:public C_TaskRealtimeBase
{
protected:
	/** @brief 获取采集信息
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[out] GatherInfos 采集信息
	 *  @return 0:成功; >0:失败
	 */
	virtual int GetMeterGatherInfo(const S_TaskInfo *pTaskInfo, S_GatherInfoS &GatherInfos);
	/** @brief 处理采集数据
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[out] GatherInfos 采集信息
	 *  @return 0:成功; >0:失败
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_GatherInfoS &GatherInfos);
	/** @brief 取消采集数据(每采完一个寄存器查询一次)
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[out] GatherInfos 采集信息
	 *  @return true:取消; false:继续
	 */
	virtual bool CancelMeterGatherData(const S_TaskInfo *pTaskInfo, const S_GatherInfoS &GatherInfos);

protected:
	int BuildupGatherInfo(S_WORD DA, S_DWORD DT, S_GatherInfoS &GatherInfos);
	int _BuildupGatherInfo(S_WORD MP, S_DWORD DT, S_GatherInfoS &GatherInfos);
	bool GetGatherInfo(S_WORD MP, S_DWORD FN, S_GatherInfo &GatherInfo);
	bool GetGatherInfo_Meter(S_WORD SN, S_DWORD FN, S_GatherInfo &GatherInfo);
	bool GetGatherInfo_Pulse(S_WORD SN, S_DWORD FN, S_GatherInfo &GatherInfo);
	bool GetGatherInfo_Analog(S_WORD SN, S_DWORD FN, S_GatherInfo &GatherInfo);
};

/** @class
 *  @brief 国网重点户任务
 */
class C_GwTaskVip:public C_TaskPollBase
{
public:
	/** @brief 取任务执行器的名称 */
	virtual const char *GetName(void);
	/** @brief 取任务执行器的类型号 */
	virtual int GetTypeID(void);

protected:
	/** @brief 获取采集信息
	 *  @param[in] MP 测量点或表计序号
	 *  @param[out] MeterBase 表计信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase);
	/** @brief 处理采集数据
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[out] pTaskCtrlStatus 采集信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);

	/** @brief 处理采集通讯错误
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[out] pTaskCtrlStatus 采集信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int HandleMeterCommInfo(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);
};

/** @class
 *  @brief 国网日任务
 */
class C_GwTaskDay:public C_TaskReportBase
{
public:
	/** @brief 取任务执行器的名称 */
	virtual const char *GetName(void);
	/** @brief 取任务执行器的类型号 */
	virtual int GetTypeID(void);

protected:
	/** @brief 获取采集信息
	 *  @param[in] MP 测量点或表计序号
	 *  @param[out] MeterBase 表计信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase);
	/** @brief 处理采集数据
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[out] pTaskCtrlStatus 采集信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);
	/** @brief 处理采集通讯错误
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[out] pTaskCtrlStatus 采集信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int HandleMeterCommInfo(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);
};

/** @class
 *  @brief 国网月任务
 */
class C_GwTaskMon:public C_TaskPollBase
{
public:
	/** @brief 取任务执行器的名称 */
	virtual const char *GetName(void);
	/** @brief 取任务执行器的类型号 */
	virtual int GetTypeID(void);

protected:
	/** @brief 获取采集信息
	 *  @param[in] MP 测量点或表计序号
	 *  @param[out] MeterBase 表计信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase);
	/** @brief 处理采集数据
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[out] pTaskCtrlStatus 采集信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);
	/** @brief 处理采集通讯错误
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[out] pTaskCtrlStatus 采集信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int HandleMeterCommInfo(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);
};

/** @class
 *  @brief 国网优先告警采集任务
 */
class C_GwPollAlarm:public C_TaskPollBase
{
public:
	/** @brief 取任务执行器的名称 */
	virtual const char *GetName(void);
	/** @brief 取任务执行器的类型号 */
	virtual int GetTypeID(void);

protected:
	/** @brief 任务结束
	 *  @return >=0:成功；<0:失败
	 */
	virtual int TaskFinish(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief 获取采集信息
	 *  @param[in] MP 测量点或表计序号
	 *  @param[out] MeterBase 表计信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase);
	/** @brief 处理采集数据
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[out] pTaskCtrlStatus 采集信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);
	/** @brief 处理采集通讯错误
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[out] pTaskCtrlStatus 采集信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int HandleMeterCommInfo(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);
};

/** @class
 *  @brief 国网普通告警采集任务
 */
class C_GwDayAlarm:public C_TaskPollBase
{
public:
	/** @brief 取任务执行器的名称 */
	virtual const char *GetName(void);
	/** @brief 取任务执行器的类型号 */
	virtual int GetTypeID(void);

protected:
	/** @brief 获取采集信息
	 *  @param[in] MP 测量点或表计序号
	 *  @param[out] MeterBase 表计信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase);
	/** @brief 处理采集数据
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[out] pTaskCtrlStatus 采集信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);
};

/** @class
 *  @brief 国网轮询任务
 */
class C_GwPolling:public C_TaskDirectBase
{
public:
	/** @brief 取任务执行器的名称 */
	virtual const char *GetName(void);
	/** @brief 取任务执行器的类型号 */
	virtual int GetTypeID(void);

	/** @brief 任务处于运行态
	 *  @return TASK_EXEC_STATUS_
	 */
	virtual int Execute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);

protected:
	/** @brief 获取采集信息
	 *  @param[in] MP 测量点或表计序号
	 *  @param[out] MeterBase 表计信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase);
	/** @brief 处理采集数据
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[out] pTaskCtrlStatus 采集信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);
};

/** @class
 *  @brief 国网能量数据统计日任务
 */
class C_GwStatDay:public C_TaskDirectBase
{
public:
	/** @brief 取任务执行器的名称 */
	virtual const char *GetName(void);
	/** @brief 取任务执行器的类型号 */
	virtual int GetTypeID(void);

protected:
	/** @brief 获取采集信息
	 *  @param[in] MP 测量点或表计序号
	 *  @param[out] MeterBase 表计信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase);
	/** @brief 处理采集数据
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[out] pTaskCtrlStatus 采集信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);
};

/** @class
 *  @brief 国网能量数据统计月任务
 */
class C_GwStatMon:public C_GwStatDay
{
public:
	/** @brief 取任务执行器的名称 */
	virtual const char *GetName(void);
	/** @brief 取任务执行器的类型号 */
	virtual int GetTypeID(void);

protected:
	/** @brief 处理采集数据
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[out] pTaskCtrlStatus 采集信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);
};

/** @class
 *  @brief 国网终端数据采集日任务
 */
class C_GwTermDay:public C_TaskTermBase
{
public:
	/** @brief 取任务执行器的名称 */
	virtual const char *GetName(void);
	/** @brief 取任务执行器的类型号 */
	virtual int GetTypeID(void);

protected:
	/** @brief 获取采集信息
	 *  @param[in] MP 测量点或表计序号
	 *  @param[out] MeterBase 表计信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase);
	/** @brief 处理采集数据
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[out] pTaskCtrlStatus 采集信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);
	/** @brief 任务结束
	 *  @return >=0:成功；<0:失败
	 */
	virtual int TaskFinish(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
};

/** @class
 *  @brief 国网终端数据采集月任务
 */
class C_GwTermMon:public C_GwTermDay
{
public:
	/** @brief 取任务执行器的名称 */
	virtual const char *GetName(void);
	/** @brief 取任务执行器的类型号 */
	virtual int GetTypeID(void);

protected:
	/** @brief 处理采集数据
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[out] pTaskCtrlStatus 采集信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);
};

/** @class
 *  @brief 国网终端数据采集VIP任务
 */
class C_GwTermVip:public C_GwTermDay
{
public:
	/** @brief 取任务执行器的名称 */
	virtual const char *GetName(void);
	/** @brief 取任务执行器的类型号 */
	virtual int GetTypeID(void);

protected:
	/** @brief 处理采集数据
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[out] pTaskCtrlStatus 采集信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);
};

/** @class
 *  @brief 国网搜索表计任务
 */
class C_GwSeekMeter:public C_TaskSeekMeterBase
{
public:
	/** @brief 取任务执行器的名称 */
	virtual const char *GetName(void);
	/** @brief 取任务执行器的类型号 */
	virtual int GetTypeID(void);

	/** @brief 任务启动
	 *  @return 0:成功；>0:失败
	 */
	virtual int TaskStart(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief 任务结束
	 *  @return >=0:成功；<0:失败
	 */
	virtual int TaskFinish(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);

protected:
	/** @brief 保存搜索成功的表计
	 *  @return <0:失败; >=0:成功
	 */
	virtual int SaveMeter(int idxFB, const S_ReportMeterInfo &MeterInfo);

	bool NewMeter(const string &strMeterAddr, const string &strRelay);
	int StoreMeter(int idxFB, const S_MeterBase &Meter, const string &strRelay);
	int NewMeterAlarm(S_BYTE VCom, const S_ReportMeterInfo &MeterInfo, const string &strRelay);
};

/** @class
 *  @brief 国网日补抄任务
 *  @note 补抄任务的数据项配置必需与二类数据相应的数据格式配置一样
 */
class C_GwTaskFixDay:public C_TaskPollBase
{
public:
	/** @brief 取任务执行器的名称 */
	virtual const char *GetName(void);
	/** @brief 取任务执行器的类型号 */
	virtual int GetTypeID(void);

	/** @brief 任务启动
	 *  @return 0:成功；>0:失败
	 */
	virtual int TaskStart(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);

protected:
	/** @brief 获取采集信息
	 *  @param[in] MP 测量点或表计序号
	 *  @param[out] MeterBase 表计信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase);
	/** @brief 处理采集数据
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[out] pTaskCtrlStatus 采集信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);

	int StatFixMeter(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
};

enum
{
	PROXY_METER_CTRL_ERROR = -1, // 错误
	PROXY_METER_CTRL_NOTHING = 0, // 没什么
	PROXY_METER_CTRL_SUCCESS, // 成功
	PROXY_METER_CTRL_FAILED, // 失败

	PROXY_METER_CTRL_MAX
};

/** @class
 *  @brief 转发任务和委托任务
 *  @note
 1、转发任务
 AFN = MSAFN_DATA_ROUTE
 DADT 按转发任务要求,见上行规约5.16　数据转发（AFN=10H）
 2、委托任务
 AFN = MSAFN_USER_EXTEND_PROXY
 DADT自定义
 PN = 表计测量点
 FN = 1: 身份认证任务
 FN = 2: 终端对时任务
 3、表计拉合闸任务
 注1：N1为控制命令类型，N1=1AH代表跳闸，N1=1BH代表合闸允许，
 N1=2AH代表报警，N1=2BH代表报警解除，N1=3AH代表保电，
 N1=3BH代表保电解除。N2保留。N3～N8代表命令有效截止时间，
 数据格式为ssmmhhDDMMYY。
 */
class C_GwProxy:public C_TaskOneoff
{
protected:
	/** @brief 任务处理器
	 */
	virtual void Handle(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	int HandleDataRoute(const string &strCmd, string &strMsg);
	int HandleProxy(const string &strCmd, string &strMsg);
	int HandleMeterCtrlCmd(const string &strCmd, string &strMsg);

	// F1：透明转发
	int HandleForward(const string &strCmdInfo, string &strData);
	// F9：转发主站直接对电表的抄读数据命令
	int HandleReadData(const string &strCmdInfo, string &strData);
	// F10：转发主站直接对电表的遥控跳闸/允许合闸命令
	int HandleControlCmd(const string &strCmdInfo, string &strData);
	// F11：转发主站直接对电表的遥控送电命令
	int HandlePowerCmd(const string &strCmdInfo, string &strData);

	// F1：电表身份认证
	int HandleMeterIdentification(const S_MeterInfo &Meter, const string &strCmdInfo, string &strData);
	// F2：电表对时任务
	int HandleMeterTiming(const S_MeterInfo &Meter, const string &strCmdInfo, string &strData);

	// 电能表遥控拉闸
	int HandleMeterCtrl_LaZa(S_WORD MP, const string &strCmdInfo, string &strData);
	// 电能表遥控合闸
	int HandleMeterCtrl_HeZa(S_WORD MP, const string &strCmdInfo, string &strData);
	// 对电能表保电操作
	int HandleMeterCtrl_BaoDian(S_WORD MP, const string &strCmdInfo, string &strData);
	// 对电能表保电解除操作
	int HandleMeterCtrl_BuBaoDian(S_WORD MP, const string &strCmdInfo, string &strData);
	// 表计控制处理
	// return PROXY_METER_CTRL_
	int HandleMeterBrake(const S_MeterBase &MeterBase, const string &strPassword, const string &strOperator, S_BYTE Brake, S_WORD TimeoutS, string &strState, string &strYgData);
	// 表计控制处理
	// return PROXY_METER_CTRL_
	int HandleMeterBaoDian(const S_MeterBase &MeterBase, const string &strPassword, const string &strOperator, S_BYTE Brake, S_WORD TimeoutS, string &strState);

	int HandleMeterBrake_ReadState(const S_MeterBase &MeterBase, S_DWORD CommRegID, string &strState);
	int HandleMeterBrake_ReadReg(const S_MeterBase &MeterBase, S_DWORD CommRegID, string &strData);
	int HandleMeterBrake_Action(const S_MeterBase &MeterBase, const string &strPassword, const string &strOperator, const string &strParam);
	int HandleMeterBrake_Poll(const S_MeterBase &MeterBase, S_WORD BrakeExpectedState, S_WORD TimeoutS, string &strState);

	int HandleBrakeAlarm(S_DWORD ERC, S_WORD MP, const string &strState);
};

/** @class
 *  @brief 路由扩展任务，主要用于测试环境下处理有的载波需要轮抄的情况,如东软三代模式下的抄表
 */
class C_GwRouteEx:public C_TaskReportBase
{
public:
	/** @brief 取任务执行器的名称 */
	virtual const char *GetName(void);
	/** @brief 取任务执行器的类型号 */
	virtual int GetTypeID(void);

protected:
	/** @brief 任务启动
	 *  @return 0:成功；>0:失败
	 */
	virtual int TaskStart(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief 获取采集信息
	 *  @param[in] MP 测量点或表计序号
	 *  @param[out] MeterBase 表计信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase);
	/** @brief 处理采集数据
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[out] pTaskCtrlStatus 采集信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);
};

#endif //__GW_TASK_EXEC_H__


