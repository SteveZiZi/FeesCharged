/**
 *  @file
 *  @brief ����ִ����
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
//����ִ����
//
enum
{
	TASK_EXEC_STATUS_CONTINUE,
	TASK_EXEC_STATUS_BLOCK,
	TASK_EXEC_STATUS_EXIT,

	TASK_EXEC_STATUS_MAX
};

/** @class
 *  @brief ����ִ����
 *  @note ���������󣬲���������
 */
class C_TaskExec
{
public:
	C_TaskExec(void);
	virtual ~C_TaskExec();

public:
	/** @brief ȡ����ִ���������� */
	virtual const char *GetName(void);
	/** @brief ȡ����ִ���������ͺ� */
	virtual int GetTypeID(void);
	/** @brief ��������
	 *  @return 0:�ɹ���>0:ʧ��
	 */
	virtual int TaskStart(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief �������
	 *  @return >=0:�ɹ���<0:ʧ��
	 */
	virtual int TaskFinish(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief ��������̬ǰ׼��
	 *  @return >=0:�ɹ���<0:ʧ��
	 */
	virtual int BeforeExecute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief ��������̬����
	 *  @return >=0:�ɹ���<0:ʧ��
	 */
	virtual int AfterExecute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief ����������̬
	 *  @return TASK_EXEC_STATUS_
	 */
	virtual int Execute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	
	void CommLedFlashPrompt();
protected:
	/** @brief ��ȡ���ͨ����� 
	 *  @param[in] ObjType �Ĵ�����Դ[1-9] ȡ�Ĵ����ĸ�4λ
	 *  @see <kge common registor manual.doc>
	 *  @note
		3*******������Դ�ļĴ���
		2*******����ѹ����ģ����Դ�ļĴ���
		1*******���ն˼Ĵ�����������������GPRS�ź�
		����Ϊ��ƼĴ���
	 */
	virtual C_MeterCom* GetMeterCom(S_BYTE ObjType);
	/** @brief �ж��Ƿ�����������
	 *  @param[in] CommRegs ͨ�üĴ�����
	 *  @param[in] strDatas ����
	 */
	bool AnyFilledRegister(const S_REGISTERS &CommRegs, const strings &strDatas);
	/** @brief �ж��Ƿ��вɼ��ɹ�������
	 *  @param[in] CommRegs ͨ�üĴ�����
	 *  @param[in] strDatas ����
	 */
	bool AnySuccessfulRegister(const S_REGISTERS &CommRegs, const strings &strDatas);

};

/** @class
 *  @brief ��������
 */
class C_TaskIdle:public C_TaskExec
{
};

/** @class
 *  @brief �㲥У������
 *  GetMeterCom��������������Ƿ���д
 */
class C_TaskBroadcast:public C_TaskExec
{
public:
	/** @brief ȡ����ִ���������� */
	virtual const char *GetName(void);
	/** @brief ȡ����ִ���������ͺ� */
	virtual int GetTypeID(void);
	/** @brief ����������̬
	 *  @return TASK_EXEC_STATUS_
	 */
	virtual int Execute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);

protected:
	/** @brief Ӧ�ò�����������
	 *  @return 0
	 */
	virtual int TickServer(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);

	/** @brief �㲥Уʱ�����ʱʱ��
	 *  @param[in] pTaskInfo ������Ϣ
	 *  @param[in] FBID �ֳ�����ID��
	 *  @return 0:��Чֵ; >0:��Ч��ʱʱ��
	 *  @note �����������ģʽ��Уʱ��ʱ900������
	 */
 	virtual S_DWORD GetMaxDelayS(const S_TaskInfo *pTaskInfo, S_WORD FBID);
};

/** @class
 *  @brief һ��������
 *  �벻Ҫ��дExecute��������ͨ����дHandleʵ��
 */
class C_TaskOneoff:public C_TaskExec
{
public:
	/** @brief ȡ����ִ���������� */
	virtual const char *GetName(void);
	/** @brief ȡ����ִ���������ͺ� */
	virtual int GetTypeID(void);
	/** @brief ����������̬
	 *  @return TASK_EXEC_STATUS_
	 */
	virtual int Execute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
protected:
	/** @brief ��������
	 */
	virtual void Handle(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus) = 0;
};

/** @class
 *  @brief ·���������
 *  @note GetMeterAll��Ҫ��д��GetDnModule��������������Ƿ���д
 */
class C_TaskRouteBase:public C_TaskExec
{
public:
	/** @brief ȡ����ִ���������� */
	virtual const char *GetName(void);
	/** @brief ȡ����ִ���������ͺ� */
	virtual int GetTypeID(void);
	/** @brief �������
	 *  @return >=0:�ɹ���<0:ʧ��
	 */
	virtual int TaskFinish(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief ��������̬����
	 *  @return >=0:�ɹ���<0:ʧ��
	 */
	virtual int AfterExecute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief ����������̬
	 *  @return TASK_EXEC_STATUS_
	 */
	virtual int Execute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);

protected:
	/** @brief ��ȡ����ģ�������� */
	virtual C_DnModule* GetDnModule(void);
	/** @brief ��ȡ��ǰ���
	 *  @param[out] Meter485 �ɼ����µ�485��
	 *  @param[out] MeterZB �ز�������߱�
	 *  @param[out] Relay �ɼ���
	 *  @return 0:�ɹ���>0:ʧ��
	 */
	virtual int GetMeterAll(S_TERMINALS &Meter485, S_TERMINALS &MeterZB, S_TERMINALS &Relay) = 0;
	/** @brief ·��ģ�����ʧ����Ϣ */
	virtual int SendRouterErrorMessage(int ErrorInfo) = 0;
};

struct S_GatherInfo
{
	S_DWORD m_MP; ///< ������
	S_DWORD m_MSDI; ///< ��վ���ݱ�ʶ
	S_MeterBase m_MeterBase; ///< �����Ϣ
	S_REGISTERS m_CommRegs; ///< ͨ�üĴ�����Ϣ
	S_DIBS m_DisableReg;
	strings m_strDatas; ///< �ɼ�������Ϣ
};
struct S_GatherInfoS:public vector<S_GatherInfo>
{
};

/** @class
 *  @brief ʵʱ�������
 *  @note GetMeterGatherInfo��SaveMeterGatherData��Ҫ��д��
 *  GetMeterCom��������������Ƿ���д
 */
class C_TaskRealtimeBase:public C_TaskExec
{
public:
	/** @brief ȡ����ִ���������� */
	virtual const char *GetName(void);
	/** @brief ȡ����ִ���������ͺ� */
	virtual int GetTypeID(void);
	/** @brief ����������̬
	 *  @return TASK_EXEC_STATUS_DESTROY
	 */
	virtual int Execute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);

protected:
	/** @brief ��ȡ�ɼ���Ϣ
	 *  @param[in] pTaskInfo ������Ϣ
	 *  @param[out] GatherInfos �ɼ���Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int GetMeterGatherInfo(const S_TaskInfo *pTaskInfo, S_GatherInfoS &GatherInfos) = 0;
	/** @brief ����ɼ�����
	 *  @param[in] pTaskInfo ������Ϣ
	 *  @param[out] GatherInfos �ɼ���Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_GatherInfoS &GatherInfos) = 0;
	/** @brief ȡ���ɼ�����(ÿ����һ���Ĵ�����ѯһ��)
	 *  @param[in] pTaskInfo ������Ϣ
	 *  @param[out] GatherInfos �ɼ���Ϣ
	 *  @return true:ȡ��; false:����
	 */
	virtual bool CancelMeterGatherData(const S_TaskInfo *pTaskInfo, const S_GatherInfoS &GatherInfos) = 0;
};

/** @class
 *  @brief ��ѯʽ�ɱ����
 *  @note GetMeterGatherInfo��HandleMeterGatherData��Ҫ��д��
 *  GetMeterCom��������������Ƿ���д
 */
class C_TaskPollBase:public C_TaskExec
{
public:
	/** @brief ȡ����ִ���������� */
	virtual const char *GetName(void);
	/** @brief ȡ����ִ���������ͺ� */
	virtual int GetTypeID(void);
	/** @brief ��������
	 *  @return 0:�ɹ���>0:ʧ��
	 */
	virtual int TaskStart(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief ����������̬
	 *  @return TASK_EXEC_STATUS_
	 */
	virtual int Execute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);

protected:
	/** @brief ��ȡ�ɼ���Ϣ
	 *  @param[in] MP ������������
	 *  @param[out] MeterBase �����Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase) = 0;
	/** @brief ����ɼ�����
	 *  @param[in] pTaskInfo ������Ϣ
	 *  @param[out] pTaskCtrlStatus �ɼ���Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus) = 0;

	/** @brief ����ɼ�ͨѶ����
	 *  @param[in] pTaskInfo ������Ϣ
	 *  @param[out] pTaskCtrlStatus �ɼ���Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int HandleMeterCommInfo(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);

	/** @brief ����������̬
	 *  @return 0:�ɼ�������<0:�����ɼ�
	 */
	int DirectMeterGather(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief ����������̬
	 *  @return 0:�ɼ�������<0:�����ɼ�
	 */
	int IndirectMeterGather(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);

	/** @brief �жϲ������Ƿ���Ҫ�ɼ�
	 *  @return true:��Ҫ��false:����Ҫ
	 */
	bool NeedGather(S_WORD mpMeter, const S_TaskInfo *pTaskInfo, const S_TaskFreezeStatus *pTaskFreezeStatus);

	bool InitNextMeterInfo_485(S_TaskCtrlStatus *pTaskCtrlStatus, const S_TaskFreezeStatus *pTaskFreezeStatus);
	bool InitNextMeterInfo_ZBWX(S_TaskCtrlStatus *pTaskCtrlStatus, const S_TaskFreezeStatus *pTaskFreezeStatus);
};

/** @class
 *  @brief �ϱ�ʽ�ɱ����
 *  @note GetMeterIndex��Ҫ��д��
 */
class C_TaskReportBase:public C_TaskPollBase
{
public:
	/** @brief ȡ����ִ���������� */
	virtual const char *GetName(void);
	/** @brief ȡ����ִ���������ͺ� */
	virtual int GetTypeID(void);
	/** @brief ��������
	 *  @return 0:�ɹ���>0:ʧ��
	 */
	virtual int TaskStart(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief ��������̬����
	 *  @return >=0:�ɹ���<0:ʧ��
	 */
	virtual int AfterExecute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief ����������̬
	 *  @return TASK_EXEC_STATUS_
	 */
	virtual int Execute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);

protected:
	/** @brief ����������̬
	 *  @return 0:�ɼ�������<0:�����ɼ�
	 */
	int ReportMeterGather(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief ��֯�������ϱ��ı�Ƽ� */
	int MakeReportMeterSet(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief �����ϱ�״̬�µĳ�ʱʱ�䣨�룩 */
	S_DWORD GetReportDataTimeoutS(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);
};

/** @class
 *  @brief ֱ����ɱ����
 */
class C_TaskDirectBase:public C_TaskPollBase
{
public:
	/** @brief ȡ����ִ���������� */
	virtual const char *GetName(void);
	/** @brief ȡ����ִ���������ͺ� */
	virtual int GetTypeID(void);
	/** @brief ����������̬
	 *  @return TASK_EXEC_STATUS_
	 */
	virtual int Execute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
};

/** @class
 *  @brief �ն����ݶ������
 *  @note 
 *  1��GetMeterGatherInfo��GetMeterCom��HandleMeterGatherData��Ҫ��д��
 *  2�������ն��������������ܼ������ݵĶ���
 */
class C_TaskTermBase:public C_TaskExec
{
public:
	/** @brief ȡ����ִ���������� */
	virtual const char *GetName(void);
	/** @brief ȡ����ִ���������ͺ� */
	virtual int GetTypeID(void);
	/** @brief ��������
	 *  @return 0:�ɹ���>0:ʧ��
	 */
	virtual int TaskStart(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief ����������̬
	 *  @return TASK_EXEC_STATUS_
	 */
	virtual int Execute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);

protected:
	/** @brief ��ȡ�ɼ���Ϣ
	 *  @param[in] MP ������������
	 *  @param[out] MeterBase �����Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase) = 0;
	/** @brief ����ɼ�����
	 *  @param[in] pTaskInfo ������Ϣ
	 *  @param[out] pTaskCtrlStatus �ɼ���Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus) = 0;

	/** @brief ����ɼ�ͨѶ����
	 *  @param[in] pTaskInfo ������Ϣ
	 *  @param[out] pTaskCtrlStatus �ɼ���Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int HandleMeterCommInfo(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);

	/** @brief ����������̬
	 *  @return 0:�ɼ�������<0:�����ɼ�
	 */
	int TermDataGather(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);

	/** @brief �жϲ������Ƿ���Ҫ�ɼ�
	 *  @return true:��Ҫ��false:����Ҫ
	 */
	bool NeedGather(S_WORD mpMeter, const S_TaskInfo *pTaskInfo, const S_TaskFreezeStatus *pTaskFreezeStatus);

	bool InitNextMeterInfo(S_TaskCtrlStatus *pTaskCtrlStatus, const S_TaskInfo *pTaskInfo);
};

/** @class
 *  @brief ��������������
 *  @note SaveMeter��Ҫ��д��
 */
class C_TaskSeekMeterBase:public C_TaskExec
{
public:
	/** @brief ȡ����ִ���������� */
	virtual const char *GetName(void);
	/** @brief ȡ����ִ���������ͺ� */
	virtual int GetTypeID(void);
	/** @brief ��������̬����
	 *  @return >=0:�ɹ���<0:ʧ��
	 */
	virtual int AfterExecute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief ����������̬
	 *  @return TASK_EXEC_STATUS_
	 */
	virtual int Execute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);

protected:
	int SeekMeter_485I(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	int SeekMeter_485II(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	int SeekMeter_ZBWX(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	int SeekMeter(int idxFB, const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);

	/** @brief ���������ɹ��ı��
	 *  @return <0:ʧ��; >=0:�ɹ�
	 */
	virtual int SaveMeter(int idxFB, const S_ReportMeterInfo &MeterInfo) = 0;
};

#endif //__BASEB_TASK_EXEC_H__



