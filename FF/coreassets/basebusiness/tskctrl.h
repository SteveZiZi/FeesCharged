/**
 *  @file
 *  @brief 任务信息、状态及客户端
 *  @author 
 *  @date 2012-06-19
 *  @version v1.0
 *  @note
 */
#ifndef __BASEB_TASK_CLIENT_H__
#define __BASEB_TASK_CLIENT_H__
#include "basedef.h"
#include "basetype.h"
#include "tskctrl.h"
#include "tskexec.h"

/** @class
 *  @brief 任务控制块
 */
class C_TaskCtrl
{
public:
	C_TaskCtrl(void);
	C_TaskCtrl(const S_TaskInfo &TaskInfo, S_TaskFreezeStatus *pTaskFreezeStatus, C_TaskExec *pTaskExec);
	C_TaskCtrl(const C_TaskCtrl &Task);
	~C_TaskCtrl();

public:
	C_TaskCtrl& operator=(const C_TaskCtrl &Task);
	bool TaskEnable(void);
	bool TaskEnable(bool fEnable);
	const char *TaskName(void);
	S_DWORD TaskID(void);
	int TaskTypeID(void);

	/** @brief 优先级比较
	 *  @param[in] Task 被比较对象
	 *  @return true本身优先级高;false本身优先级低
	 */
	bool HighPriority(const C_TaskCtrl &Task);
	/** @brief 启动判断
	 *  @return true可以启动;false未到启动时间
	 */
	bool Raise(void);

	/** @brief 设定任务立即启动
	 *  @param[in] fForce true强制设定;false如已进入正常启动则不设置
	 *  @return true已设定;false未设定
	 */
	bool StartImmediately(bool fForce);

	/** @brief 阻塞态转就绪态 */
	int Block2Ready(void);
	/** @brief 就绪态转运行态 */
	int Ready2Running(void);
	/** @brief 运行态转就绪态 */
	int Running2Ready(void);
	/** @brief 运行态转阻塞态 */
	int Running2Block(void);

	/** @brief 任务结束 */
	int Exit(void);
	/** @brief 任务执行 */
	int Execute(void);

private:
	/** @brief 启动时间对齐
	 *  @param[in] TaskInfo 任务信息
	 *  @param[in] timeRefer 参考时间
	 *  @return 启动时间
	 */
	S_TIME TrimRaiseTime(const S_TaskInfo &TaskInfo, S_TIME timeRefer);

private:
	S_TaskInfo m_TaskInfo; ///< 任务信息
	S_TaskCtrlStatus m_TaskCtrlStatus; ///< 任务控制信息
	S_TaskFreezeStatus *m_pTaskFreezeStatus; ///< 任务持久化信息
	C_TaskExec *m_pTaskExec; ///< 任务执行器
};

/** @brief 任务列表 */
class C_TaskList:public list<C_TaskCtrl*>
{
};

#endif //__BASEB_TASK_CLIENT_H__

