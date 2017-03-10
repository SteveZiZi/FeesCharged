/** @file
 *  @brief ���������
 *  @author 
 *  @date 2011/11/07
 *  @version 0.1
 *  1��ϵͳ����ĸ�����ϵͳΪһ������ķ���ʵ��
 *  2�������ѡ��μ�C_AppCmd�Ĺ�������
 */
#ifndef __MP_SERVER_H__
#define __MP_SERVER_H__
#include "basedef.h"
#include "osrelate.h"

#define SERVER_HEART_BEAT_INTERS	60

/** @class
 *  @brief ϵͳ��Ϣ
 */
struct S_SystemInfo
{
	bool m_HWReset; ///< Ӳ��������־
	bool m_SWReset; ///< ���������־
	S_WORD m_HWResetDelayS; ///< Ӳ��������ʱ
	S_WORD m_SWResetDelayS; ///< ���������ʱ
	S_TIME m_tickSystem; ///< ϵͳ����
	char m_ResetScript[CFG_PATH_FILE_MAX_LEN];

public:
	S_SystemInfo(void)
	{
		memset(this, 0x00, sizeof(*this));
		m_tickSystem = 0x01; //��1��ʼ����
	}
};

/** @brief ������Ϣ�궨�� */
enum
{
	SERVER_MODE_ACTIVED,
	SERVER_MODE_INACTIVED,
	SERVER_MODE_SUSPENDED
};
enum
{
	SERVER_STATUS_STOPPED,//δ����
	SERVER_STATUS_RUNNING,//��������
	SERVER_STATUS_STOPPING//����ֹͣ
};
struct S_SERVINFO
{
	bool   m_Enabled;///< ʹ��
	S_BYTE m_ServID;///< �������
	S_BYTE m_Mode;///< ģʽ,�μ�SERVER_MODE
	S_BYTE m_Status;///< �μ�SERVER_STATUS
	S_BYTE m_DeadCnt;///< ����ֹͣ�Ĵ���������������ֹͣ��������CONS_MAX_DEADCOUNTʱ������Ϊ�˽�������
	S_WORD m_ExeCnt;///< ��ִ�еĴ���,ÿ��forkһ�ξͼ�1
	S_WORD m_HBInterS;///< �������
	S_TIME m_tickLHB;///< �������ʱ�䣨��ʱ�ӣ�
	PID_T  m_PID;///< ����ID

public:
	S_SERVINFO(void);
	S_SERVINFO(S_BYTE ServID);
	~S_SERVINFO();

public:
	bool IsStartable(void) const;
	S_BYTE GetServerID(void) const;
	bool IsKillable(void) const;
	void SetActive(void);
	void SetInactive(void);
	void SetSuspend(void);
	void SetResume(void);
	bool IsRunning() const;
	/** @brief ����ֹͣ״̬ */
	void SetStoppingStatus(void);
	/** @brief ������ֹͣ״̬ */
	void SetStopedStatus(void);
	/** @brief ��������״̬ */
	void SetStartedStatus(PID_T pid);
	/** @brief �ж�������ʱ
	 *  @param[in] tickNow ��ǰϵͳ����
	 *  @return true ��ʱ��false δ��ʱ
	 */
	bool IsHBTimeout(S_TIME tickNow) const;
	/** @brief ȡMODE�� */
	const char *GetModeKey(void);
	/** @brief ȡSTATUS�� */
	const char *GetStatusKey(void);
};
inline bool S_SERVINFO::IsStartable(void) const
{
	return (m_Enabled==true && m_Mode==SERVER_MODE_ACTIVED && m_Status==SERVER_STATUS_STOPPED);
}
inline S_BYTE S_SERVINFO::GetServerID(void) const
{
	return m_ServID;
}
inline bool S_SERVINFO::IsKillable(void) const
{
	return (m_Enabled==false || m_Mode!=SERVER_MODE_ACTIVED);
}
inline void S_SERVINFO::SetActive(void)
{
	m_Mode=SERVER_MODE_ACTIVED;
}
inline void S_SERVINFO::SetInactive(void)
{
	m_Mode=SERVER_MODE_INACTIVED;
}
inline void S_SERVINFO::SetSuspend(void)
{
	if (m_Mode==SERVER_MODE_ACTIVED)
	{
		m_Mode=SERVER_MODE_SUSPENDED;
	}
}
inline void S_SERVINFO::SetResume(void)
{
	if (m_Mode==SERVER_MODE_SUSPENDED)
	{
		m_Mode=SERVER_MODE_ACTIVED;
	}		
}
inline bool S_SERVINFO::IsRunning() const
{
	return (m_Status==SERVER_STATUS_RUNNING && m_PID>0);
}

/** @class
 *  @brief ϵͳ�������
 *  @remarks ϵͳ�е����з������̳б���
 */
class C_SERVER
{
public:
	C_SERVER(void);
	virtual ~C_SERVER();

	/** @brief ���ݷ���IDȡ������ */
	static const char *GetServerName(int ServerID);
	/** @brief ���ݷ�����ȡ����ID */
	static int GetServerID(const char *ServerName);
	/** @brief ȡ��ǰ����ID */
	static int GetCurrentServID(void){return m_CurServID;}
	/** @brief ȡ��ǰ������ */
	static const char *GetCurrentServerName(void){return GetServerName(GetCurrentServID());}
	/** @brief ���õ�ǰ����ID */
	static void SetCurrentServID(int ServerID){m_CurServID = ServerID;}
	/** @brief ���õ�ǰ������Ϣ */
	static void SetCurrentServer(C_SERVER *pServer){m_pCurServer = pServer;}
	/** @brief ȡ��ǰ���� */
	static C_SERVER *GetCurrentServer(void){return m_pCurServer;}
	/**  @brief Ӧ�������б������Ϣ
	 *  @param[in] pProcess:������
	 *  @param[out] strProcessInfo:����ı���Ϣ����
	 *  @return 0:successful, -1:failed
	 */
	static int AppCmdListProcess(const char *pProcessName, string &strProcessInfo);

public:
	/** @brief �������
	 *  @return 0 �ɹ���-1 ʧ��
	 */
	int Service(void);
	/** @brief ����
	 *  @return true �ɹ���false ʧ��
	 */
	bool HeartBeat(void);
	/** @brief �ж��Ƿ������� */
	bool IsStartable(void){return m_pServInfo->IsStartable();}
	/** @brief ���÷�����Ϣ */
	void SetServerInfo(S_SERVINFO *pServInfo){m_pServInfo = pServInfo;}

	/** @brief ��������ǰ����
	 *  @return 0 �ɹ���-1 ʧ��
	 *  @remarks Ӧ�ó����������
	 */
	virtual int BeforeService(void){return 0;}
	/** @brief ������������
	 *  @return 0 �ɹ���-1 ʧ��
	 *  @remarks Ӧ�ó����������
	 */
	virtual int AfterService(void){return 0;}
	/** @brief ���ڷ���
	 *  @return 0 �ɹ���-1 ʧ��
	 *  @remarks Ӧ�ó����������
	 */
	virtual int OnTickService(void){return 0;}

protected:
	/** @brief �����ж�
	 *  @return true ��Ҫ��false ��
	 */
	bool HeartBeatable(void){return m_pServInfo->m_HBInterS>0;}

	/** @brief ALARM�жϺ���
	 *  @return 0 �ɹ���-1 ʧ��
	 *  @remarks Ӧ�ó����������
	 */
	virtual int OnSigAlrm(void);
	/** @brief TERM�жϺ���
	 *  @return 0 �ɹ���-1 ʧ��
	 *  @remarks Ӧ�ó����������
	 */
	virtual int OnSigTerm(void);
	/** @brief ȡALARM���
	 *  @return 0 �ɹ���-1 ʧ��
	 *  @remarks Ӧ�ó���������أ���Ϊ0��������ALARM
	 */
	virtual S_DWORD GetAlarmInterUS(void){return 0x00;}

private:
	/** @brief ��װ�ж� */
	void InstallSignal(void);
#ifndef WIN32
	/** @brief �ж��źŴ����� */
	static void SigHandler(int signo, siginfo_t *siginfp, struct sigcontext * scp);
#endif

protected:
	S_SERVINFO *m_pServInfo; ///< ������Ϣ

private:
	static int m_CurServID; ///< �����̷���ID������ȫ��ʹ��
	static C_SERVER *m_pCurServer; ///< �����̷�����Ϣ������ȫ��ʹ��
};

#endif //__MP_SERVER_H__


