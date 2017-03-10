/** @file
 *  @brief 用户层系统状态配置类
 *  @author 
 *  @date 2011/12/06
 *  @version 0.1
 *  @see mprocess/sharemem.h mrpocess/sysipc.h
 *  注意需要继承C_StatusBase类，注意其中的抽象方法的重载。
 */
#ifndef __NW_STATUS_H__
#define __NW_STATUS_H__
#include "cfg_stat.h"
#include "sysipc.h"
#include "basetype.h"
#include "ini_serv.h"
#include "ini_mp.h"
#include "ini_sys.h"
#include "ini_task.h"
#include "ini_fkctrl.h"

class C_STATUS:public C_StatusBase
{
public:
	virtual ~C_STATUS(){m_pMyStatus = NULL;}
	static C_STATUS &Instance(void);

	/** @brief 设置成员信息等
	 *  @return true 成功；false 失败
	 *  @remarks 子类重载时请先回调本函数，共享内存创建成功时调用本函数
	 */
	virtual bool SetInformation(void);
	/** @brief 初始化系统状态
	 *  @return true 成功；false 失败
	 *  @remarks 子类重载时请先回调本函数
	 */
	virtual bool InitStatus(void);
	/** @brief 加载配置到系统状态
	 *  @return true 成功；false 失败
	 *  @remarks 子类重载时请先回调本函数
	 */
	virtual bool LoadStatus(void);
	/** @brief 参数分析
	 *  @return true 成功；false 失败
	 *  @remarks 子类重载时请先回调本函数
	 */
	virtual bool ParamParse(void);
	/** @brief 数据区复位时清除数据
	 *  @return true 成功；false 失败
	 *  @remarks 子类重载时请先回调本函数
	 */
	virtual bool ClearData(void);

public:
	/** @brief Get函数区*/
	S_MeterInfos *GetMeterInfos(void){return &m_pMyStatus->m_MeterInfos;}
	S_SYSCFG *GetSystemConfig(void){return &m_pMyStatus->m_SysConfig;}
	S_SYSRUN *GetSysRunInfo(void){return &m_pMyStatus->m_SysRun;}
	S_CHANNEL *GetHttxChannel(S_HttxSN HttxSN, S_ChannelSN ChannelSN);
	S_CHANNEL *GetForwardChannel(int ChannelSN);
	S_CHANNEL *GetPppChannel(int ChannelSN);
	S_CHANNEL *GetJlcyChannel(int ChannelSN);
	S_CHANNEL *GetDbcjChannel(S_DbcjSN DbcjSN, int ChannelSN);
	S_CHANNEL *GetDbcjZbwxChannel(void);
	S_HttxInfo *GetHttxInfo(S_HttxSN httxSN){return &m_pMyStatus->m_HttxInfo[httxSN];}
	S_JlcyInfo *GetJlcyInfo(void){return &m_pMyStatus->m_JlcyInfo;}
	S_PppInfo *GetPppInfo(void){return &m_pMyStatus->m_PppInfo;}
	S_IpConfig *GetIpConfig(void){return &m_pMyStatus->m_IpCofig;}
	S_LcdInfo *GetLcdInfo(void){return &m_pMyStatus->m_LcdInfo;}
	S_ForwardInfo *GetForwardInfo(void){return &m_pMyStatus->m_ForwardInfo;}
	S_AlarmInfo *GetAlarmInfo(void){return &m_pMyStatus->m_AlarmInfo;}
	S_NormalTaskInfos *GetNormalTaskInfos(void){return &m_pMyStatus->m_NormalTaskInfos;}
	S_DnChannel *GetDnChannelInfo(void){return &m_pMyStatus->m_DnChannels;}
	S_UpChannel *GetUpChannelInfo(void){return &m_pMyStatus->m_UpChannels;};
	S_ForwardTaskInfos *GetForwardTaskInfos(void){return &m_pMyStatus->m_ForwardTaskInfos;}
	S_PowerLimit *GetPowerLimitInfo(void){return &m_pMyStatus->m_PowerLimit;}
	S_DbcjInfo *GetDbcjInfo(S_DbcjSN DbcjSN){return &m_pMyStatus->m_DbcjInfo[DbcjSN];}
	S_MpInfos *GetMpInfos(void){return &m_pMyStatus->m_MpInfos;}
	S_TaskStatus *GetTaskStatus(S_DbcjSN DbcjSN){return &m_pMyStatus->m_TaskStatus[DbcjSN];}
	S_DbcjParam *GetDbcjParam(void){return &m_pMyStatus->m_DbcjParam;}
	S_ALLMP *GetALLMP(void){return &m_pMyStatus->m_ALLMP;}
	S_MPLS *GetMPointLimits(void){return &m_pMyStatus->m_MPointLimits;}//测量点限值参数
	/** 运算量*/
	S_ANALOGS *GetAnalogInfos(void){return &m_pMyStatus->m_AnalogInfo;}
	S_PULSES *GetPulseInfos(void){return &m_pMyStatus->m_PulseInfo;}
	S_SUMS *GetSumInfos(void){return &m_pMyStatus->m_SumInfo;}
	S_DiffRuleS *GetDiffRuleInfos(void){return &m_pMyStatus->m_DiffRuleInfo;}
	/** 负控管理参数*/
	S_FKCtrlBaseInfo *GetFKCtrlBaseInfo(void){return &m_pMyStatus->m_FhkzInfo.m_BaseInfo;}
	S_PWRCtrlInfo *GetPWRCtrlInfo(void){return &m_pMyStatus->m_PWRCtrlInfo;}
	S_DLCtrlInfo *GetDLCtrlInfo(void){return &m_pMyStatus->m_DLCtrlInfo;}
	/** 抄表任务参数获取函数 **/
	S_GatherTaskInfos *GetGatherTaskInfos(void){return &m_pMyStatus->m_GatherTaskInfos;}
	S_GatherTaskInfo *GetGatherTaskInfo(INT8U taskNo); 
	/** @brief 获取地区扩展信息存储区 */
	void *GetZoneExtendInfo(size_t size);

	/** 测量点参数获取函数 */
	S_BYTE GetReactivePowerOperator(S_WORD mp);
	/** @brief register area */
	S_REGISTER FindCommonRegister(S_DWORD ID);
	S_TaskInfo FindTaskTemplateInfo(const char *pTaskName);
	S_MSDI FindMasterStationDataID(S_BYTE Class, S_DWORD Idx);
	S_MSDI FindMasterStationDataID(S_BYTE Class, S_DWORD DI, S_DWORD dummy);

	S_TermData *GetTermData(void){return &m_pMyStatus->m_TermData;}
	S_AlarmStatus *GetAlarmStatus(void){return &m_pMyStatus->m_AlarmStatus;}

public:
	/** @brief 终端复位函数*/
	void IncResetTimes(void);
	S_WORD GetFreezeDayResetTimes(void);
	S_WORD GetFreezeMonResetTimes(void);
	/** @brief 终端供电时间函数*/
	void IncPowerSupplyTime(S_WORD PowerSupplyM);
	S_WORD GetDayPowerSupplyM(void);
	S_WORD GetMonPowerSupplyM(void);
	S_WORD GetFreezeDayPowerSupplyM(void);
	S_WORD GetFreezeMonPowerSupplyM(void);
	/** @brief 终端流量函数*/
	S_TIME GetCommunicationTime(void);
	void AddCommFlux(S_DWORD flux);
	S_DWORD GetDayCommFlux(void);
	S_DWORD GetMonCommFlux(void);
	S_DWORD GetDayCommFreeze(void);
	S_DWORD GetMonCommFreeze(void);

public:
	/** @brief ??? */
	void RefreshMeterInfo(void);
	/** @brief 获取采集信息
	 *  @param[in] MP 测量点或表计序号
	 *  @param[out] MeterBase 表计信息
	 *  @return 0:成功; <0:失败
	 */
	int GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase);

public:
	/** @brief 参数保存区 */
	int SaveUpChannel(void){return C_IniServer::SaveUpChannl(m_pMyStatus);}
	int SaveDnChannl(void){return C_IniServer::SaveDnChannl(m_pMyStatus);}
	int SaveHttxInfo(void){return C_IniServer::SaveHttxInfo(m_pMyStatus);}
	int SaveTermIPConfig(void){return C_IniSys::SaveTermIPConfig(m_pMyStatus);}
	int SavePppInfo(void){return C_IniServer::SavePppInfo(m_pMyStatus);}
	int SaveSystemConfig(void){return C_IniSys::SaveSystemConfig(m_pMyStatus);}
	int SaveSysRunInfo(void){return C_IniSys::SaveRunInformation(m_pMyStatus);}
	int SaveAlarmInfo(void){return C_IniSys::SaveAlarmInfo(m_pMyStatus);}
	int SaveLcdInfo(void){return C_IniServer::SaveLcdInfo(m_pMyStatus);}
	int SaveMeterInfo(void){return C_IniMP::SaveMeterInformation(m_pMyStatus);}
	int SavePowerLimitInfo(void){return C_IniSys::SavePowerLimitInfo(m_pMyStatus);}
	int SaveDbcjInfo(void){return C_IniServer::SaveDbcjInfo(m_pMyStatus);}
	int SaveAnalogInfo(void){return C_IniSys::SaveAnalogInfo(m_pMyStatus);}
	int SavePulseInfo(void){return C_IniSys::SavePulseInfo(m_pMyStatus);}
	int SaveSumInfo(void){return C_IniSys::SaveSumInfo(m_pMyStatus);}
	int SaveDiffRuleInfo(void){return C_IniSys::SaveDiffRuleInfo(m_pMyStatus);}
	int SaveForwardInfo(void){return C_IniSys::SaveForwardInfo(m_pMyStatus);}
	int SaveFKCtrlBaseInfo(void){return C_IniServer::SaveFhkzInfo(m_pMyStatus);}
	int SavePWRCtrlInfo(void){return C_IniFKCtrl::SavePWRCtrlInfo(m_pMyStatus);}
	int SaveDLCtrlInfo(void){return C_IniFKCtrl::SaveDLCtrlInfo(m_pMyStatus);}
	int SaveGatherTasks(void){return CInitTask::SaveGatherTasks(m_pMyStatus);}
	int SaveNormalTask(void){return CInitTask::SaveNormalTask(m_pMyStatus);}
	int SaveForwardTask(void){return CInitTask::SaveForwardTask(m_pMyStatus);}    
	int SaveMPointLimit(void){return C_IniMPLimit::SaveMPointLimit(m_pMyStatus);}

protected:
	S_STATUS *m_pMyStatus;
};

#define STATUS C_STATUS::Instance()

#endif//__NW_STATUS_H__

