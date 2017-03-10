/** @file
 *  @brief ��������ִ����
 *  @author 
 *  @date 2012/06/24
 *  @version 0.1
 */
#ifndef __GW_TASK_EXEC_H__
#define __GW_TASK_EXEC_H__
#include "tskexec.h"
#include "sysbase.h"

/** @class
 *  @brief �����㲥Уʱ����
 */
class C_GwTaskBroadcast:public C_TaskBroadcast
{
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
 *  @brief ����·������
 */
class C_GwTaskRoute:public C_TaskRouteBase
{
protected:
	/** @brief ��ȡ��ǰ���
	 *  @param[out] Meter485 �ɼ����µ�485��
	 *  @param[out] MeterZB �ز�������߱�
	 *  @param[out] Relay �ɼ���
	 *  @return 0:�ɹ���>0:ʧ��
	 */
	virtual int GetMeterAll(S_TERMINALS &Meter485, S_TERMINALS &MeterZB, S_TERMINALS &Relay);
	/** @brief ·��ģ�����ʧ����Ϣ */
	virtual int SendRouterErrorMessage(int ErrorInfo);
};

/** @class
 *  @brief ����ʵʱ����
 */
class C_GwTaskRealtime:public C_TaskRealtimeBase
{
protected:
	/** @brief ��ȡ�ɼ���Ϣ
	 *  @param[in] pTaskInfo ������Ϣ
	 *  @param[out] GatherInfos �ɼ���Ϣ
	 *  @return 0:�ɹ�; >0:ʧ��
	 */
	virtual int GetMeterGatherInfo(const S_TaskInfo *pTaskInfo, S_GatherInfoS &GatherInfos);
	/** @brief ����ɼ�����
	 *  @param[in] pTaskInfo ������Ϣ
	 *  @param[out] GatherInfos �ɼ���Ϣ
	 *  @return 0:�ɹ�; >0:ʧ��
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_GatherInfoS &GatherInfos);
	/** @brief ȡ���ɼ�����(ÿ����һ���Ĵ�����ѯһ��)
	 *  @param[in] pTaskInfo ������Ϣ
	 *  @param[out] GatherInfos �ɼ���Ϣ
	 *  @return true:ȡ��; false:����
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
 *  @brief �����ص㻧����
 */
class C_GwTaskVip:public C_TaskPollBase
{
public:
	/** @brief ȡ����ִ���������� */
	virtual const char *GetName(void);
	/** @brief ȡ����ִ���������ͺ� */
	virtual int GetTypeID(void);

protected:
	/** @brief ��ȡ�ɼ���Ϣ
	 *  @param[in] MP ������������
	 *  @param[out] MeterBase �����Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase);
	/** @brief ����ɼ�����
	 *  @param[in] pTaskInfo ������Ϣ
	 *  @param[out] pTaskCtrlStatus �ɼ���Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);

	/** @brief ����ɼ�ͨѶ����
	 *  @param[in] pTaskInfo ������Ϣ
	 *  @param[out] pTaskCtrlStatus �ɼ���Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int HandleMeterCommInfo(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);
};

/** @class
 *  @brief ����������
 */
class C_GwTaskDay:public C_TaskReportBase
{
public:
	/** @brief ȡ����ִ���������� */
	virtual const char *GetName(void);
	/** @brief ȡ����ִ���������ͺ� */
	virtual int GetTypeID(void);

protected:
	/** @brief ��ȡ�ɼ���Ϣ
	 *  @param[in] MP ������������
	 *  @param[out] MeterBase �����Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase);
	/** @brief ����ɼ�����
	 *  @param[in] pTaskInfo ������Ϣ
	 *  @param[out] pTaskCtrlStatus �ɼ���Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);
	/** @brief ����ɼ�ͨѶ����
	 *  @param[in] pTaskInfo ������Ϣ
	 *  @param[out] pTaskCtrlStatus �ɼ���Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int HandleMeterCommInfo(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);
};

/** @class
 *  @brief ����������
 */
class C_GwTaskMon:public C_TaskPollBase
{
public:
	/** @brief ȡ����ִ���������� */
	virtual const char *GetName(void);
	/** @brief ȡ����ִ���������ͺ� */
	virtual int GetTypeID(void);

protected:
	/** @brief ��ȡ�ɼ���Ϣ
	 *  @param[in] MP ������������
	 *  @param[out] MeterBase �����Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase);
	/** @brief ����ɼ�����
	 *  @param[in] pTaskInfo ������Ϣ
	 *  @param[out] pTaskCtrlStatus �ɼ���Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);
	/** @brief ����ɼ�ͨѶ����
	 *  @param[in] pTaskInfo ������Ϣ
	 *  @param[out] pTaskCtrlStatus �ɼ���Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int HandleMeterCommInfo(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);
};

/** @class
 *  @brief �������ȸ澯�ɼ�����
 */
class C_GwPollAlarm:public C_TaskPollBase
{
public:
	/** @brief ȡ����ִ���������� */
	virtual const char *GetName(void);
	/** @brief ȡ����ִ���������ͺ� */
	virtual int GetTypeID(void);

protected:
	/** @brief �������
	 *  @return >=0:�ɹ���<0:ʧ��
	 */
	virtual int TaskFinish(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief ��ȡ�ɼ���Ϣ
	 *  @param[in] MP ������������
	 *  @param[out] MeterBase �����Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase);
	/** @brief ����ɼ�����
	 *  @param[in] pTaskInfo ������Ϣ
	 *  @param[out] pTaskCtrlStatus �ɼ���Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);
	/** @brief ����ɼ�ͨѶ����
	 *  @param[in] pTaskInfo ������Ϣ
	 *  @param[out] pTaskCtrlStatus �ɼ���Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int HandleMeterCommInfo(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);
};

/** @class
 *  @brief ������ͨ�澯�ɼ�����
 */
class C_GwDayAlarm:public C_TaskPollBase
{
public:
	/** @brief ȡ����ִ���������� */
	virtual const char *GetName(void);
	/** @brief ȡ����ִ���������ͺ� */
	virtual int GetTypeID(void);

protected:
	/** @brief ��ȡ�ɼ���Ϣ
	 *  @param[in] MP ������������
	 *  @param[out] MeterBase �����Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase);
	/** @brief ����ɼ�����
	 *  @param[in] pTaskInfo ������Ϣ
	 *  @param[out] pTaskCtrlStatus �ɼ���Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);
};

/** @class
 *  @brief ������ѯ����
 */
class C_GwPolling:public C_TaskDirectBase
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
	/** @brief ��ȡ�ɼ���Ϣ
	 *  @param[in] MP ������������
	 *  @param[out] MeterBase �����Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase);
	/** @brief ����ɼ�����
	 *  @param[in] pTaskInfo ������Ϣ
	 *  @param[out] pTaskCtrlStatus �ɼ���Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);
};

/** @class
 *  @brief ������������ͳ��������
 */
class C_GwStatDay:public C_TaskDirectBase
{
public:
	/** @brief ȡ����ִ���������� */
	virtual const char *GetName(void);
	/** @brief ȡ����ִ���������ͺ� */
	virtual int GetTypeID(void);

protected:
	/** @brief ��ȡ�ɼ���Ϣ
	 *  @param[in] MP ������������
	 *  @param[out] MeterBase �����Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase);
	/** @brief ����ɼ�����
	 *  @param[in] pTaskInfo ������Ϣ
	 *  @param[out] pTaskCtrlStatus �ɼ���Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);
};

/** @class
 *  @brief ������������ͳ��������
 */
class C_GwStatMon:public C_GwStatDay
{
public:
	/** @brief ȡ����ִ���������� */
	virtual const char *GetName(void);
	/** @brief ȡ����ִ���������ͺ� */
	virtual int GetTypeID(void);

protected:
	/** @brief ����ɼ�����
	 *  @param[in] pTaskInfo ������Ϣ
	 *  @param[out] pTaskCtrlStatus �ɼ���Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);
};

/** @class
 *  @brief �����ն����ݲɼ�������
 */
class C_GwTermDay:public C_TaskTermBase
{
public:
	/** @brief ȡ����ִ���������� */
	virtual const char *GetName(void);
	/** @brief ȡ����ִ���������ͺ� */
	virtual int GetTypeID(void);

protected:
	/** @brief ��ȡ�ɼ���Ϣ
	 *  @param[in] MP ������������
	 *  @param[out] MeterBase �����Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase);
	/** @brief ����ɼ�����
	 *  @param[in] pTaskInfo ������Ϣ
	 *  @param[out] pTaskCtrlStatus �ɼ���Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);
	/** @brief �������
	 *  @return >=0:�ɹ���<0:ʧ��
	 */
	virtual int TaskFinish(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
};

/** @class
 *  @brief �����ն����ݲɼ�������
 */
class C_GwTermMon:public C_GwTermDay
{
public:
	/** @brief ȡ����ִ���������� */
	virtual const char *GetName(void);
	/** @brief ȡ����ִ���������ͺ� */
	virtual int GetTypeID(void);

protected:
	/** @brief ����ɼ�����
	 *  @param[in] pTaskInfo ������Ϣ
	 *  @param[out] pTaskCtrlStatus �ɼ���Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);
};

/** @class
 *  @brief �����ն����ݲɼ�VIP����
 */
class C_GwTermVip:public C_GwTermDay
{
public:
	/** @brief ȡ����ִ���������� */
	virtual const char *GetName(void);
	/** @brief ȡ����ִ���������ͺ� */
	virtual int GetTypeID(void);

protected:
	/** @brief ����ɼ�����
	 *  @param[in] pTaskInfo ������Ϣ
	 *  @param[out] pTaskCtrlStatus �ɼ���Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);
};

/** @class
 *  @brief ���������������
 */
class C_GwSeekMeter:public C_TaskSeekMeterBase
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
	/** @brief �������
	 *  @return >=0:�ɹ���<0:ʧ��
	 */
	virtual int TaskFinish(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);

protected:
	/** @brief ���������ɹ��ı��
	 *  @return <0:ʧ��; >=0:�ɹ�
	 */
	virtual int SaveMeter(int idxFB, const S_ReportMeterInfo &MeterInfo);

	bool NewMeter(const string &strMeterAddr, const string &strRelay);
	int StoreMeter(int idxFB, const S_MeterBase &Meter, const string &strRelay);
	int NewMeterAlarm(S_BYTE VCom, const S_ReportMeterInfo &MeterInfo, const string &strRelay);
};

/** @class
 *  @brief �����ղ�������
 *  @note ������������������ñ��������������Ӧ�����ݸ�ʽ����һ��
 */
class C_GwTaskFixDay:public C_TaskPollBase
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

protected:
	/** @brief ��ȡ�ɼ���Ϣ
	 *  @param[in] MP ������������
	 *  @param[out] MeterBase �����Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase);
	/** @brief ����ɼ�����
	 *  @param[in] pTaskInfo ������Ϣ
	 *  @param[out] pTaskCtrlStatus �ɼ���Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);

	int StatFixMeter(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
};

enum
{
	PROXY_METER_CTRL_ERROR = -1, // ����
	PROXY_METER_CTRL_NOTHING = 0, // ûʲô
	PROXY_METER_CTRL_SUCCESS, // �ɹ�
	PROXY_METER_CTRL_FAILED, // ʧ��

	PROXY_METER_CTRL_MAX
};

/** @class
 *  @brief ת�������ί������
 *  @note
 1��ת������
 AFN = MSAFN_DATA_ROUTE
 DADT ��ת������Ҫ��,�����й�Լ5.16������ת����AFN=10H��
 2��ί������
 AFN = MSAFN_USER_EXTEND_PROXY
 DADT�Զ���
 PN = ��Ʋ�����
 FN = 1: �����֤����
 FN = 2: �ն˶�ʱ����
 3���������բ����
 ע1��N1Ϊ�����������ͣ�N1=1AH������բ��N1=1BH�����բ����
 N1=2AH��������N1=2BH�����������N1=3AH�����磬
 N1=3BH����������N2������N3��N8����������Ч��ֹʱ�䣬
 ���ݸ�ʽΪssmmhhDDMMYY��
 */
class C_GwProxy:public C_TaskOneoff
{
protected:
	/** @brief ��������
	 */
	virtual void Handle(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	int HandleDataRoute(const string &strCmd, string &strMsg);
	int HandleProxy(const string &strCmd, string &strMsg);
	int HandleMeterCtrlCmd(const string &strCmd, string &strMsg);

	// F1��͸��ת��
	int HandleForward(const string &strCmdInfo, string &strData);
	// F9��ת����վֱ�ӶԵ��ĳ�����������
	int HandleReadData(const string &strCmdInfo, string &strData);
	// F10��ת����վֱ�ӶԵ���ң����բ/�����բ����
	int HandleControlCmd(const string &strCmdInfo, string &strData);
	// F11��ת����վֱ�ӶԵ���ң���͵�����
	int HandlePowerCmd(const string &strCmdInfo, string &strData);

	// F1����������֤
	int HandleMeterIdentification(const S_MeterInfo &Meter, const string &strCmdInfo, string &strData);
	// F2������ʱ����
	int HandleMeterTiming(const S_MeterInfo &Meter, const string &strCmdInfo, string &strData);

	// ���ܱ�ң����բ
	int HandleMeterCtrl_LaZa(S_WORD MP, const string &strCmdInfo, string &strData);
	// ���ܱ�ң�غ�բ
	int HandleMeterCtrl_HeZa(S_WORD MP, const string &strCmdInfo, string &strData);
	// �Ե��ܱ������
	int HandleMeterCtrl_BaoDian(S_WORD MP, const string &strCmdInfo, string &strData);
	// �Ե��ܱ���������
	int HandleMeterCtrl_BuBaoDian(S_WORD MP, const string &strCmdInfo, string &strData);
	// ��ƿ��ƴ���
	// return PROXY_METER_CTRL_
	int HandleMeterBrake(const S_MeterBase &MeterBase, const string &strPassword, const string &strOperator, S_BYTE Brake, S_WORD TimeoutS, string &strState, string &strYgData);
	// ��ƿ��ƴ���
	// return PROXY_METER_CTRL_
	int HandleMeterBaoDian(const S_MeterBase &MeterBase, const string &strPassword, const string &strOperator, S_BYTE Brake, S_WORD TimeoutS, string &strState);

	int HandleMeterBrake_ReadState(const S_MeterBase &MeterBase, S_DWORD CommRegID, string &strState);
	int HandleMeterBrake_ReadReg(const S_MeterBase &MeterBase, S_DWORD CommRegID, string &strData);
	int HandleMeterBrake_Action(const S_MeterBase &MeterBase, const string &strPassword, const string &strOperator, const string &strParam);
	int HandleMeterBrake_Poll(const S_MeterBase &MeterBase, S_WORD BrakeExpectedState, S_WORD TimeoutS, string &strState);

	int HandleBrakeAlarm(S_DWORD ERC, S_WORD MP, const string &strState);
};

/** @class
 *  @brief ·����չ������Ҫ���ڲ��Ի����´����е��ز���Ҫ�ֳ������,�綫������ģʽ�µĳ���
 */
class C_GwRouteEx:public C_TaskReportBase
{
public:
	/** @brief ȡ����ִ���������� */
	virtual const char *GetName(void);
	/** @brief ȡ����ִ���������ͺ� */
	virtual int GetTypeID(void);

protected:
	/** @brief ��������
	 *  @return 0:�ɹ���>0:ʧ��
	 */
	virtual int TaskStart(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief ��ȡ�ɼ���Ϣ
	 *  @param[in] MP ������������
	 *  @param[out] MeterBase �����Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int GetMeterGatherInfo(S_WORD mpMeter, S_MeterBase &MeterBase);
	/** @brief ����ɼ�����
	 *  @param[in] pTaskInfo ������Ϣ
	 *  @param[out] pTaskCtrlStatus �ɼ���Ϣ
	 *  @return 0:�ɹ�; <0:ʧ��
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus);
};

#endif //__GW_TASK_EXEC_H__


