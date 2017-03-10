/** @file
 *  @brief �ػ����̷���
 *  @author 
 *  @date 2011/11/07
 *  @version 0.1
 *  @note
 *  1��ι���Ź������ϵͳ
 *  2�������������ӹ��ܽ���
 */
#ifndef __MP_DAEMON_H__
#define __MP_DAEMON_H__
#include "basedef.h"
#include "server.h"

enum
{
	DYJC_UPDATE_BEGIN,
	DYJC_UPDATE_CHECK,
	DYJC_UPDATE_CHECK_OK,
	DYJC_UPDATE_CHECK_FAILED,
	DYJC_UPDATE_REBOOT,

	DYJC_UPDATE_MAX
};

/** @class
 *  @brief �ػ����̷�����
 */
class C_DaemonServer:public C_SERVER
{
public:
	C_DaemonServer(void);
	~C_DaemonServer();

	/** @brief ���������ڷ������ */
	virtual int BeforeService(void);
	/** @brief ����ִ�к��� */
	virtual int OnTickService(void);	

protected:
	/** @brief Alarm�жϺ��� */
	virtual int OnSigAlrm(void);
	/** @brief TERM�ź��жϺ��� */
	virtual int OnSigTerm(void);
	/** @brief ȡALARM�������Ϊ0����ALARM������ */
	virtual S_DWORD GetAlarmInterUS(void){return 1000000;}
	/** @brief ���60������ */
	virtual int CheckTask60S(void);
	/** @brief ���5������ */
	virtual int CheckTask5S(void);
	/** @brief ���2������ */
	virtual int CheckTask2S(void);
	/** @brief ���1������ */
	virtual int CheckTask1S(void);
	/** @brief ι���Ź� */
	virtual bool WatchDog(void);
	/** @������Ϣ֪ͨ */
	virtual bool UpdateNotify(int msg);

	/** @brief ���״̬ */
	bool CheckStatus(void);
	/** @brief ������״̬ */
	bool ScanServers(void);
	/** @brief ɱ�����з��� */
	bool KillServers(void);
	/** @brief �������з��� */
	bool SuspendServers(bool bSyspend);
	/** @brief ����ָ������ */
	bool StartServer(S_SERVINFO &ServInfo);
	/** @brief ɱ��ָ������ */
	bool KillServer(S_SERVINFO &ServInfo);
	/** @brief ���շ��� */
	bool WaitServer(PID_T pid=-1);
	/** @brief ����ϵͳ״̬ */
	bool SaveSystemStatus(void);
	/** @brief ִ���˳��ű� */
	int ExecStopScript(void);

private:
	/** @brief ι���Ź� */
	bool doWatchDog(void);
	/** @brief ��鸴λ��Ϣ */
	bool CheckResetInfo(void);
	/** @brief ���������Ϣ */
	bool CheckUpdate(void);
	/** @brief ͬ������ʱ�� */
	bool SyncHWClock(void);

protected:
	bool m_StopWatchDog; ///< ���Ź���־
private:
	int m_ProcessCnt; ///< ��������
	S_SystemInfo *m_pSysInfo; ///< ϵͳ��Ϣָ��
};

#endif//__MP_DAEMON_H__

