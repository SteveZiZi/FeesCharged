/**
 *  @file
 *  @brief 任务执行者
 *  @author 
 *  @date 2012-06-19
 *  @version v1.0
 *  @note
 */
#ifndef __BASEB_TASK_EXEC_H__
#define __BASEB_TASK_EXEC_H__
#include "basetype.h"
#include "dnmodule.h"
#include "metercom.h"

//
//任务执行器
//
enum
{
	TASK_EXEC_STATUS_CONTINUE,
	TASK_EXEC_STATUS_BLOCK,
	TASK_EXEC_STATUS_EXIT,

	TASK_EXEC_STATUS_MAX
};

/** @class
 *  @brief 任务执行器
 *  @note 轻量级对象，不得有属性
 */
class C_TaskExec
{
public:
	C_TaskExec(void);
	virtual ~C_TaskExec();

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
	/** @brief 任务运行态前准备
	 *  @return >=0:成功；<0:失败
	 */
	virtual int BeforeExecute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief 任务运行态后处理
	 *  @return >=0:成功；<0:失败
	 */
	virtual int AfterExecute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief 任务处于运行态
	 *  @return TASK_EXEC_STATUS_
	 */
	virtual int Execute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	
	void CommLedFlashPrompt();
protected:
	/** @brief 获取表读通信组件 
	 *  @param[in] ObjType 寄存器的源[1-9] 取寄存器的高4位
	 *  @see <kge common registor manual.doc>
	 *  @note
		3*******：脉冲源的寄存器
		2*******：电压电流模拟量源的寄存器
		1*******：终端寄存器，如月流量，日GPRS信号
		其它为表计寄存器
	 */
	virtual C_MeterCom* GetMeterCom(S_BYTE ObjType);
	/** @brief 判断是否有填充的数据
	 *  @param[in] CommRegs 通用寄存器组
	 *  @param[in] strDatas 数据
	 */
	bool AnyFilledRegister(const S_REGISTERS &CommRegs, const strings &strDatas);
	/** @brief 判断是否有采集成功的数据
	 *  @param[in] CommRegs 通用寄存器组
	 *  @param[in] strDatas 数据
	 */
	bool AnySuccessfulRegister(const S_REGISTERS &CommRegs, const strings &strDatas);

};

/** @class
 *  @brief 空闲任务
 */
class C_TaskIdle:public C_TaskExec
{
};

/** @class
 *  @brief 广播校表任务
 *  GetMeterCom依具体情况决定是否重写
 */
class C_TaskBroadcast:public C_TaskExec
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
 *  @brief 一次性任务
 *  请不要重写Execute，任务处理通过重写Handle实现
 */
class C_TaskOneoff:public C_TaskExec
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
	/** @brief 任务处理器
	 */
	virtual void Handle(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus) = 0;
};

/** @class
 *  @brief 路由任务基类
 *  @note GetMeterAll需要重写；GetDnModule依具体情况决定是否重写
 */
class C_TaskRouteBase:public C_TaskExec
{
public:
	/** @brief 取任务执行器的名称 */
	virtual const char *GetName(void);
	/** @brief 取任务执行器的类型号 */
	virtual int GetTypeID(void);
	/** @brief 任务结束
	 *  @return >=0:成功；<0:失败
	 */
	virtual int TaskFinish(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief 任务运行态后处理
	 *  @return >=0:成功；<0:失败
	 */
	virtual int AfterExecute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief 任务处于运行态
	 *  @return TASK_EXEC_STATUS_
	 */
	virtual int Execute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);

protected:
	/** @brief 获取下行模块管理组件 */
	virtual C_DnModule* GetDnModule(void);
	/** @brief 获取当前表计
	 *  @param[out] Meter485 采集器下的485表
	 *  @param[out] MeterZB 载波表或无线表
	 *  @param[out] Relay 采集器
	 *  @return 0:成功；>0:失败
	 */
	virtual int GetMeterAll(S_TERMINALS &Meter485, S_TERMINALS &MeterZB, S_TERMINALS &Relay) = 0;
	/** @brief 路由模块操作失败消息 */
	virtual int SendRouterErrorMessage(int ErrorInfo) = 0;
};

struct S_GatherInfo
{
	S_DWORD m_MP; ///< 测量点
	S_DWORD m_MSDI; ///< 主站数据标识
	S_MeterBase m_MeterBase; ///< 表计信息
	S_REGISTERS m_CommRegs; ///< 通用寄存器信息
	S_DIBS m_DisableReg;
	strings m_strDatas; ///< 采集数据信息
};
struct S_GatherInfoS:public vector<S_GatherInfo>
{
};

/** @class
 *  @brief 实时任务基类
 *  @note GetMeterGatherInfo和SaveMeterGatherData需要重写；
 *  GetMeterCom依具体情况决定是否重写
 */
class C_TaskRealtimeBase:public C_TaskExec
{
public:
	/** @brief 取任务执行器的名称 */
	virtual const char *GetName(void);
	/** @brief 取任务执行器的类型号 */
	virtual int GetTypeID(void);
	/** @brief 任务处于运行态
	 *  @return TASK_EXEC_STATUS_DESTROY
	 */
	virtual int Execute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);

protected:
	/** @brief 获取采集信息
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[out] GatherInfos 采集信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int GetMeterGatherInfo(const S_TaskInfo *pTaskInfo, S_GatherInfoS &GatherInfos) = 0;
	/** @brief 处理采集数据
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[out] GatherInfos 采集信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_GatherInfoS &GatherInfos) = 0;
	/** @brief 取消采集数据(每采完一个寄存器查询一次)
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[out] GatherInfos 采集信息
	 *  @return true:取消; false:继续
	 */
	virtual bool CancelMeterGatherData(const S_TaskInfo *pTaskInfo, const S_GatherInfoS &GatherInfos) = 0;
};

/** @class
 *  @brief 轮询式采表基类
 *  @note GetMeterGatherInfo和HandleMeterGatherData需要重写；
 *  GetMeterCom依具体情况决定是否重写
 */
class C_TaskPollBase:public C_TaskExec
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
	virtual int GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase) = 0;
	/** @brief 处理采集数据
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[out] pTaskCtrlStatus 采集信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus) = 0;

	/** @brief 处理采集通讯错误
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[out] pTaskCtrlStatus 采集信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int HandleMeterCommInfo(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);

	/** @brief 任务处于运行态
	 *  @return 0:采集结束；<0:继续采集
	 */
	int DirectMeterGather(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief 任务处于运行态
	 *  @return 0:采集结束；<0:继续采集
	 */
	int IndirectMeterGather(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);

	/** @brief 判断测量点是否需要采集
	 *  @return true:需要；false:不需要
	 */
	bool NeedGather(S_WORD mpMeter, const S_TaskInfo *pTaskInfo, const S_TaskFreezeStatus *pTaskFreezeStatus);

	bool InitNextMeterInfo_485(S_TaskCtrlStatus *pTaskCtrlStatus, const S_TaskFreezeStatus *pTaskFreezeStatus);
	bool InitNextMeterInfo_ZBWX(S_TaskCtrlStatus *pTaskCtrlStatus, const S_TaskFreezeStatus *pTaskFreezeStatus);
};

/** @class
 *  @brief 上报式采表基类
 *  @note GetMeterIndex需要重写；
 */
class C_TaskReportBase:public C_TaskPollBase
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
	/** @brief 任务运行态后处理
	 *  @return >=0:成功；<0:失败
	 */
	virtual int AfterExecute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief 任务处于运行态
	 *  @return TASK_EXEC_STATUS_
	 */
	virtual int Execute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);

protected:
	/** @brief 任务处于运行态
	 *  @return 0:采集结束；<0:继续采集
	 */
	int ReportMeterGather(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief 组织须主动上报的表计集 */
	int MakeReportMeterSet(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief 数据上报状态下的超时时间（秒） */
	S_DWORD GetReportDataTimeoutS(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);
};

/** @class
 *  @brief 直抄表采表基类
 */
class C_TaskDirectBase:public C_TaskPollBase
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
};

/** @class
 *  @brief 终端数据冻结基类
 *  @note 
 *  1、GetMeterGatherInfo、GetMeterCom和HandleMeterGatherData需要重写；
 *  2、用于终端数据如流量、总加组数据的冻结
 */
class C_TaskTermBase:public C_TaskExec
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
	virtual int GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase) = 0;
	/** @brief 处理采集数据
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[out] pTaskCtrlStatus 采集信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus) = 0;

	/** @brief 处理采集通讯错误
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[out] pTaskCtrlStatus 采集信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int HandleMeterCommInfo(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);

	/** @brief 任务处于运行态
	 *  @return 0:采集结束；<0:继续采集
	 */
	int TermDataGather(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);

	/** @brief 判断测量点是否需要采集
	 *  @return true:需要；false:不需要
	 */
	bool NeedGather(S_WORD mpMeter, const S_TaskInfo *pTaskInfo, const S_TaskFreezeStatus *pTaskFreezeStatus);

	bool InitNextMeterInfo(S_TaskCtrlStatus *pTaskCtrlStatus, const S_TaskInfo *pTaskInfo);
};

/** @class
 *  @brief 搜索表计任务基类
 *  @note SaveMeter需要重写；
 */
class C_TaskSeekMeterBase:public C_TaskExec
{
public:
	/** @brief 取任务执行器的名称 */
	virtual const char *GetName(void);
	/** @brief 取任务执行器的类型号 */
	virtual int GetTypeID(void);
	/** @brief 任务运行态后处理
	 *  @return >=0:成功；<0:失败
	 */
	virtual int AfterExecute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief 任务处于运行态
	 *  @return TASK_EXEC_STATUS_
	 */
	virtual int Execute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);

protected:
	int SeekMeter_485I(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	int SeekMeter_485II(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	int SeekMeter_ZBWX(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	int SeekMeter(int idxFB, const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);

	/** @brief 保存搜索成功的表计
	 *  @return <0:失败; >=0:成功
	 */
	virtual int SaveMeter(int idxFB, const S_ReportMeterInfo &MeterInfo) = 0;
};

#endif //__BASEB_TASK_EXEC_H__



