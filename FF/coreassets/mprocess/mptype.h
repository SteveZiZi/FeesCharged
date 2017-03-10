/**
 *  @file
 *  @brief �����û����Ͷ���
 *  @author 
 *  @date 2011-07-28
 *  @version v1.0
 *  @note
 *  ����̿�ܻ����������Ͷ���
 */
#ifndef __MP_BASETYPE_H__
#define __MP_BASETYPE_H__
#include "basedef.h"
#include "bitsetex.h"

/** @brief ����������Ϣ */
struct S_FlowInfo
{
	//#����ʹ�ܿ���
	bool m_FlowCtrlEnable;
	//#��½����
	S_WORD m_LogonInterS;
	S_WORD m_LogonRetryTimes;
	S_WORD m_LogonFailedTimes;
	//#Socket����
	S_WORD m_SocketConnectInterS;
	S_WORD m_SocketConnectRetryTimes;
	S_WORD m_SocketConnectFailedTimes;
	//#���ſ���
	S_WORD m_DialInterS;
	S_WORD m_DialRetryTimes;
	S_WORD m_DialFrequencyM;
	S_WORD m_DialFailedTimes;
	//#��վͨѶ��
	BitSetEx<32> m_MSConnectDate;
	BitSetEx<24> m_MSConnectTime;
	//#������������
	S_DWORD m_DayFlowMax; ///< #0��ʾ��������������
	S_DWORD m_MonFlowMax; ///< #0��ʾ��������������

	//����ʧ�ܴ�������
	S_WORD m_HeartBeatFailedTimes;

public:	
	void Init(void);
	void ClearFailedInfo(void);
};

/** @brief ������Ϣ */
#define PPP_INFO_STRING_LEN_MAX	36
struct S_DialInfo
{
	S_WORD m_CheckInterM;//����������ӣ�
	S_WORD m_CheckFailTimes;//���ʧ�ܷ�ֵ�������������ֵ�μ��ʧ�ܣ�������
	char m_Apn[PPP_INFO_STRING_LEN_MAX];//APN
	char m_TeleNo[PPP_INFO_STRING_LEN_MAX];//�绰���룬ΪCONS_DEFAULT_KEYʱ��ʾ����Ĭ��ֵ
	char m_User[PPP_INFO_STRING_LEN_MAX];//�����û���ΪCONS_DEFAULT_KEYʱ��ʾ����Ĭ��ֵ
	char m_Pwd[PPP_INFO_STRING_LEN_MAX];//�������룬ΪCONS_DEFAULT_KEYʱ��ʾ����Ĭ��ֵ
};

/** @brief PPP����ͳ�� */
struct S_PppPackage
{
	S_TIME m_timeInit;
	INT64U m_LastRecvPackages;//���հ���Ŀ
	INT64U m_LastSendPackages;//���Ͱ���Ŀ
	INT64U m_TotalRecvPackages;//�ܵĽ��հ���Ŀ
	INT64U m_TotalSendPackages;//�ܵķ��Ͱ���Ŀ
};

// 	��������
struct S_ZoneCfg
{
	string m_strZoneName;
	string m_strZonePackage;
};
struct S_ZoneCfgs:public vector<S_ZoneCfg>
{
};

#endif//__MP_BASETYPE_H__



