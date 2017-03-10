/**
 *  @file
 *  @brief �ɼ����������
 *  @author 
 *  @date 2012-06-19
 *  @version v1.0
 *  @note
 */
#ifndef __BASEB_TASK_SCHEDULE_H__
#define __BASEB_TASK_SCHEDULE_H__
#include "tskctrl.h"
#include "tskexec.h"
#include "basetype.h"

/** @class
 *  @brief ���������
 *  @note 
 *  ʹ��˳�� 1:����init(); 2:�����û�����CreateTaskClient; 3:����ʼ����Schedule
 *  �����п�����������,��ʵʱ����
 *  �û���Ҫ��дGetTaskExec��GetTaskFreezeStatus
 */
class C_TaskSchedule
{
public:
	C_TaskSchedule(void);
	virtual ~C_TaskSchedule();

public:
	/** @brief �����������ʼ�� */
	bool Init(void);
	/** @brief �����û�����
	 *  @param[in] TaskInfo ������Ϣ
	 *  @return true�ɹ�;falseʧ��
	 */
	bool CreateTask(const S_TaskInfo &TaskInfo);
	/** @brief ʹ������
	 *  @param[in] pTaskName ����������
	 *  @param[in] fEnable true|false
	 *  @return true|false
	 */
	bool EnableTask(const char *pTaskName, bool fEnable);
	/** @brief ʹ������
	 *  @param[in] TaskID ����ID��
	 *  @param[in] fEnable true|false
	 *  @return true|false
	 */
	bool EnableTask(S_DWORD TaskID, bool fEnable);
	/** @brief ������� 
	 *  @return �ݷ���0
	 */
	int Schedule(void);
	/** @brief �趨������������
	 *  @param[in] TaskID ����ID��
	 *  @param[in] fForce trueǿ���趨;false���ѽ�����������������
	 *  @return true���趨;falseδ�趨
	 *  @note ���¶��������һ�δ���ʱ��Ҫ��������֮��ᰴ������������
	 ��fForce=false;·��������Ҫ��������������fForce=true;
	 */
	bool TaskStartImmediately(S_DWORD TaskID, bool fForce);

	/** @brief ��ȡ��ǰ�������� */
	const char *GetCurrentTaskName(void);
	/** @brief ��ȡ��ǰ�������ͺ� */
	int GetCurrentTaskTypeID(void);

protected:
	/** @brief ɾ���������� */
	bool DeleteTaskClient(void);
	/** @brief �����û��������û������б�
	 *  @param[in] pTaskClient �û�����
	 *  @return true�ɹ�;falseʧ��
	 */
	bool AddTask(C_TaskCtrl *pTaskClient);
	/** @brief ��������
	 *  @param[in] TaskID ����ID��
	 *  @return NULL����ʧ��;!NULL���ҳɹ�
	 */
	C_TaskCtrl *FindTask(S_DWORD TaskID);

	/** @brief ��ʼ���������� */
	int InitIdleTask(void);
	/** @brief �������������� */
	int CheckBlockList(void);
	bool _CheckBlockList(void);
	/** @brief ������������� */
	int CheckReadyList(void);
	/** @brief �������� */
	int TaskRunning(void);

	/** @brief ��������
	 *  @param[in] TaskInfo ������Ϣ
	 *  @return NULL����ʧ�� ��NULL�ɹ�
	 */
	C_TaskCtrl *NewTask(const S_TaskInfo &TaskInfo);
	/** @brief ɾ������ */
	void DeleteTask(C_TaskCtrl *pTaskClient);

	/** @brief ��ȡ����ִ����
	 *  @param[in] pTaskTypeName ����������
	 *  @return NULLʧ��;!NULL�ɹ�
	 */
	virtual C_TaskExec *GetTaskExec(const char *pTaskTypeName);
	/** @brief ��ȡ����־û�״̬
	 *  @param[in] TaskInfo ������Ϣ
	 *  @return NULLʧ��;!NULL�ɹ�
	 */
	virtual S_TaskFreezeStatus *GetTaskFreezeStatus(const S_TaskInfo &TaskInfo);

protected:
	C_TaskCtrl m_SysTask[SYS_TASK_MAX];///< ϵͳ����
	C_TaskCtrl *m_pUserTask[USER_TASK_MAX]; ///< �û�����

	C_TaskList m_BlockList;///< �����������
	C_TaskList m_ReadyList;///< �����������
	C_TaskCtrl *m_pRunTask;///< ��������
};

#endif //__BASEB_TASK_SCHEDULE_H__


