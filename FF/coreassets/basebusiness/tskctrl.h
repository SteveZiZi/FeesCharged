/**
 *  @file
 *  @brief ������Ϣ��״̬���ͻ���
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
 *  @brief ������ƿ�
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

	/** @brief ���ȼ��Ƚ�
	 *  @param[in] Task ���Ƚ϶���
	 *  @return true�������ȼ���;false�������ȼ���
	 */
	bool HighPriority(const C_TaskCtrl &Task);
	/** @brief �����ж�
	 *  @return true��������;falseδ������ʱ��
	 */
	bool Raise(void);

	/** @brief �趨������������
	 *  @param[in] fForce trueǿ���趨;false���ѽ�����������������
	 *  @return true���趨;falseδ�趨
	 */
	bool StartImmediately(bool fForce);

	/** @brief ����̬ת����̬ */
	int Block2Ready(void);
	/** @brief ����̬ת����̬ */
	int Ready2Running(void);
	/** @brief ����̬ת����̬ */
	int Running2Ready(void);
	/** @brief ����̬ת����̬ */
	int Running2Block(void);

	/** @brief ������� */
	int Exit(void);
	/** @brief ����ִ�� */
	int Execute(void);

private:
	/** @brief ����ʱ�����
	 *  @param[in] TaskInfo ������Ϣ
	 *  @param[in] timeRefer �ο�ʱ��
	 *  @return ����ʱ��
	 */
	S_TIME TrimRaiseTime(const S_TaskInfo &TaskInfo, S_TIME timeRefer);

private:
	S_TaskInfo m_TaskInfo; ///< ������Ϣ
	S_TaskCtrlStatus m_TaskCtrlStatus; ///< ���������Ϣ
	S_TaskFreezeStatus *m_pTaskFreezeStatus; ///< ����־û���Ϣ
	C_TaskExec *m_pTaskExec; ///< ����ִ����
};

/** @brief �����б� */
class C_TaskList:public list<C_TaskCtrl*>
{
};

#endif //__BASEB_TASK_CLIENT_H__

