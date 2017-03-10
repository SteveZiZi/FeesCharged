/** @file
 *  @brief 国网任务调度器
 *  @author 
 *  @date 2012/06/24
 *  @version 0.1
 */
#include "gwsched.h"
#include "gwtskexe.h"
#include "status.h"

/** @brief 获取任务执行器
 *  @param[in] pTaskTypeName 任务类型名
 *  @return NULL失败;!NULL成功
 */
C_TaskExec *C_GwTaskSchedule::GetTaskExec(const char *pTaskTypeName)
{
	static C_GwTaskBroadcast sGwTaskBroadcast;
	static C_GwTaskRoute sGwTaskRoute;
	static C_GwTaskRealtime sGwTaskRealtime;
	static C_GwTaskVip sGwTaskVip;
	static C_GwTaskDay sGwTaskDay;
	static C_GwTaskMon sGwTaskMon;
	static C_GwPollAlarm sGwFirstAlarm;
	static C_GwDayAlarm sGwNormalAlarm;
	static C_GwPolling sGwPolling;
	static C_GwStatDay sGwStatDay;
	static C_GwStatMon sGwStatMon;
	static C_GwTermDay sGwTermDay;
	static C_GwTermMon sGwTermMon;
	static C_GwTermVip sGwTermVip;
	static C_GwSeekMeter sGwSeekMeter;
	static C_GwTaskFixDay sGwTaskFixDay;
	static C_GwProxy sGwProxy;
	static C_GwRouteEx sGwRouteEx;

	if (strcmp(pTaskTypeName, sGwTaskBroadcast.GetName()) == 0)
		return &sGwTaskBroadcast;
	if (strcmp(pTaskTypeName, sGwTaskRoute.GetName()) == 0)
		return &sGwTaskRoute;
	if (strcmp(pTaskTypeName, sGwTaskRealtime.GetName()) == 0)
		return &sGwTaskRealtime;
	if (strcmp(pTaskTypeName, sGwTaskVip.GetName()) == 0)
		return &sGwTaskVip;
	if (strcmp(pTaskTypeName, sGwTaskDay.GetName()) == 0)
		return &sGwTaskDay;
	if (strcmp(pTaskTypeName, sGwTaskMon.GetName()) == 0)
		return &sGwTaskMon;
	if (strcmp(pTaskTypeName, sGwFirstAlarm.GetName()) == 0)
		return &sGwFirstAlarm;
	if (strcmp(pTaskTypeName, sGwNormalAlarm.GetName()) == 0)
		return &sGwNormalAlarm;
	if (strcmp(pTaskTypeName, sGwPolling.GetName()) == 0)
		return &sGwPolling;
	if (strcmp(pTaskTypeName, sGwStatDay.GetName()) == 0)
		return &sGwStatDay;
	if (strcmp(pTaskTypeName, sGwStatMon.GetName()) == 0)
		return &sGwStatMon;
	if (strcmp(pTaskTypeName, sGwTermDay.GetName()) == 0)
		return &sGwTermDay;
	if (strcmp(pTaskTypeName, sGwTermMon.GetName()) == 0)
		return &sGwTermMon;
	if (strcmp(pTaskTypeName, sGwTermVip.GetName()) == 0)
		return &sGwTermVip;
	if (strcmp(pTaskTypeName, sGwSeekMeter.GetName()) == 0)
		return &sGwSeekMeter;
	if (strcmp(pTaskTypeName, sGwTaskFixDay.GetName()) == 0)
		return &sGwTaskFixDay;
	if (strcmp(pTaskTypeName, sGwProxy.GetName()) == 0)
		return &sGwProxy;
	if (strcmp(pTaskTypeName, sGwRouteEx.GetName()) == 0)
		return &sGwRouteEx;

	return C_TaskSchedule::GetTaskExec(pTaskTypeName);
}

/** @brief 获取任务持久化状态
 *  @param[in] TaskInfo 任务信息
 *  @return NULL失败;!NULL成功
 */
S_TaskFreezeStatus *C_GwTaskSchedule::GetTaskFreezeStatus(const S_TaskInfo &TaskInfo)
{
	S_TaskStatus *pTaskStatus = STATUS.GetTaskStatus(m_DbcjSN);
	S_TaskFreezeStatus *pStatus = pTaskStatus->Find(TaskInfo);
	if (pStatus == NULL)
		pStatus = C_TaskSchedule::GetTaskFreezeStatus(TaskInfo);
	return pStatus;
}


