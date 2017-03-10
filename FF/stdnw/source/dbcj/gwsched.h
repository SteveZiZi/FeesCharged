/** @file
 *  @brief 国网任务调度器
 *  @author 
 *  @date 2012/06/24
 *  @version 0.1
 */
#ifndef __GW_SCHEDULE_H__
#define __GW_SCHEDULE_H__
#include "tsksched.h"

/** @class
 *  @brief 任务调度器
 *  @note 
 *  使用顺序 1:调用init(); 2:创建用户任务CreateTaskClient; 3:任务开始调度Schedule
 *  运行中可以增加任务,如实时任务
 */
class C_GwTaskSchedule:public C_TaskSchedule
{
public:
	C_GwTaskSchedule(void){m_DbcjSN = MP_DBCJ_1;}
	void SetDbcjSN(S_DbcjSN DbcjSN){m_DbcjSN = DbcjSN;}

protected:
	/** @brief 获取任务执行器
	 *  @param[in] pTaskTypeName 任务类型名
	 *  @return NULL失败;!NULL成功
	 */
	virtual C_TaskExec *GetTaskExec(const char *pTaskTypeName);
	/** @brief 获取任务持久化状态
	 *  @param[in] TaskInfo 任务信息
	 *  @return NULL失败;!NULL成功
	 */
	virtual S_TaskFreezeStatus *GetTaskFreezeStatus(const S_TaskInfo &TaskInfo);

protected:
	S_DbcjSN m_DbcjSN;
};

#endif //__GW_SCHEDULE_H__


