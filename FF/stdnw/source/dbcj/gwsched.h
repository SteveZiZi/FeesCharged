/** @file
 *  @brief �������������
 *  @author 
 *  @date 2012/06/24
 *  @version 0.1
 */
#ifndef __GW_SCHEDULE_H__
#define __GW_SCHEDULE_H__
#include "tsksched.h"

/** @class
 *  @brief ���������
 *  @note 
 *  ʹ��˳�� 1:����init(); 2:�����û�����CreateTaskClient; 3:����ʼ����Schedule
 *  �����п�����������,��ʵʱ����
 */
class C_GwTaskSchedule:public C_TaskSchedule
{
public:
	C_GwTaskSchedule(void){m_DbcjSN = MP_DBCJ_1;}
	void SetDbcjSN(S_DbcjSN DbcjSN){m_DbcjSN = DbcjSN;}

protected:
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
	S_DbcjSN m_DbcjSN;
};

#endif //__GW_SCHEDULE_H__


