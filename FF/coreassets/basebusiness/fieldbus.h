/**
 *  @file
 *  @brief �����ֳ����߳���ӿ�
 *  @author 
 *  @date 2012-05-10
 *  @version v1.0
 *  @note 
 */
#ifndef __BASEB_FIELDBUS_H__
#define __BASEB_FIELDBUS_H__
#include "basedef.h"
#include "basetype.h"
#include "timeop.h"
#include "connect.h"
#include "strings.h"
#include "cmdfilt.h"

enum
{
	DR_ROUTE_MODE_DEF = 0,
	DR_ROUTE_MODE_3 = 3,
	DR_ROUTE_MODE_4 = 4,

	DR_ROUTE_MODE_MAX
};

/**
 *  @brief �ֳ���������
*/
struct S_FBCONFIG
{
	int m_DrRouteMode; // see DR_ROUTE_MODE_
};

/**
 *  @class C_FieldBus
 *  @brief �ֳ����߿�����
 *  @author 
 *  @note
 *  1���ֳ����߳���ӿڣ�
 *  2���ֳ����߸��࣬���е��ֳ����߶���̳и��࣬��485���ߣ��ز����ߣ�΢�����������ߣ�
 *  3���������ֳ����߱����ṩ�Ĺ��ܡ�
 */
class C_FieldBus
{
public:
	/** @brief �ֳ����ߵ�״̬�� */
	enum{FB_STATUS_NORMAL,FB_STATUS_REPORT_DATA,FB_STATUS_REPORT_METER,FB_STATUS_ROUTER};
	enum{/*FB_STATUS_REPORT_DATA_IDLE_TIMEOUT=1800,*/FB_STATUS_REPORT_METER_IDLE_TIMEOUT=600,FB_STATUS_ROUTER_IDLE_TIMEOUT=600};
public:
	C_FieldBus(void);
	virtual ~C_FieldBus();

public:
	const char *GetName(void);
	C_CONNECT *GetConnection(void);
	C_CONNECT *SetConnection(C_CONNECT *pConnection);
	C_CmdFilter *SetCommandFilter(C_CmdFilter *pCmdFilter);
	C_CmdVisitor *SetCommandVisitor(C_CmdVisitor *pCmdVisitor);
	S_COMSET GetRelayComSet(void);
	S_DWORD SetWaitingMS(S_DWORD MS);
	bool SetFieldBusConfigure(const S_FBCONFIG &FbConfig);
	
	/** @brief �ֳ�����Ӳ��λ��������λ���ţ� 
	 *  @return 0:�ɹ�; -1:ʧ��
	 */
	virtual int Reset(void);
	/** @brief ������������ 
	 *  @return 0:�ɹ�; -1:ʧ��
	 */
	virtual int ChangeConnectionProperty(const S_COMSET &ComSet);
	/** @brief �ֳ�����״̬ͬ�������ϵ�����������Ҫ״̬ͬ�� 
	 *  @param[in] strModuleAddr ģ���ַ����û�У���ͬ����ַ
	 *  @param[out] strFactory ���ģ�鹤������Ͱ汾��
	 *  @return 0:�ɹ�; -1:ʧ��
	 */
	virtual int SyncRouterStatus(const string &strModuleAddr, string &strFactory);
	/** @brief �ֳ�����·����������ѯ���� 
	 *  @param[in] timeKeeping ·��ʱ�䣬0:�˳�·��;0xFFFFFFFF:��ѯ·��״̬;����������·��
	 *  @param[in] Meter485 �ɼ�����485���ַ
	 *  @param[in] MeterZB �ز����ַ
	 *  @param[in] Relay �ɼ�����ַ
	 *  @return <0:ʧ�� 0:�ɹ���·�ɽ��� >0:ʣ��·��ʱ��,��λ��
	 */
	virtual int Router(S_TIME timeKeeping, const S_TERMINALS &Meter485, const S_TERMINALS &MeterZB, const S_TERMINALS &Relay);
	/** @brief �ֳ����ߵ����������ϱ����� 
	 *  @param[out] ReportDatas ����������
	 *  @param[out] DataGram ��Լ����
	 *  @param[in] SuccessfulMPBS �ɹ��Ĳ����㼯
	 *  @param[in] CommRegs ͨ�üĴ�����
	 *  @param[in] timeKeeping �ϱ�ʱ��;0:�˳��ϱ�;0xFFFFFFFF:��ѯ;����:�����ϱ�
	 *  @return <0:���� 0:�����ϱ� >0:�ϱ�ʣ��ʱ��
	 */
	virtual int ReportData(S_ReportDatas &ReportDatas, S_DATAGRAM &DataGram, const S_MPBS &SuccessfulMPBS, const S_REGISTERS &CommRegs, S_TIME timeKeeping);
	/** @brief �ֳ����ߵ��������ϱ����� 
	 *  @param[out] MeterInfo �����Ϣ
	 *  @param[in] timeKeeping �ϱ�ʱ��;0:�˳��ϱ�;0xFFFFFFFF:��ѯ;����:�����ϱ�
	 *  @return -1:�ϱ����� 0:�ϱ����� >0:ʣ���ϱ�ʱ��
	 */
	virtual int ReportMeter(S_ReportMeterInfos &MeterInfos, S_TIME timeKeeping);
	/** @brief �ֳ����ߵ����ݳ������� 
	 *  @param[in] strRelay �м̵�ַ+Ŀ���ַ��Ŀ���ַ�����ÿ����ַռ6�ֽڣ�
	 *  @param[in] DataGram �������
	 *  @param[out] strData �������
	 *  @return <0:���� >=0:�ɹ�
	 */
	virtual int RequestData(const string &strRelay, const S_DATAGRAM &DataGram, string &strData);
	/** @brief �㲥���� 
	 *  @param[in] strRelay �м̵�ַ+Ŀ���ַ��Ŀ���ַ�����ÿ����ַռ6�ֽڣ�
	 *  @param[in] DataGram �������
	 *  @return <0:���� >=0:�ɹ�����ʱ������
	 */
	virtual int Broadcast(const string &strRelay, const S_DATAGRAM &DataGram);
	/** @brief �ֳ����ߵ������������ 
	 *  @param[in] CmdID ����ID
	 *  @param[in] strRelay �м̵�ַ+Ŀ���ַ��Ŀ���ַ�����ÿ����ַռ6�ֽڣ�
	 *  @param[in] DataGram �������
	 *  @param[out] strData �������
	 *  @return <0:���� >=0:�ɹ�
	 */
	virtual int RequestCmd(int CmdID, const string &strRelay, const S_DATAGRAM &DataGram, string &strData);

	int ClearConnectionBuffer(void);
	int SendCmd(const string &strCmd);
	int RecvCmd(string &strCmd);

//	/** @brief �������������
//	 *  @param[in] void
//	 *  @return :none
//	 */
//	void ClearBufferData();

protected:
	S_DWORD m_WaitingMS; ///< �����շ���ʱ��,�˲���ҪС��ʹ��
	S_COMSET m_RelayComSet; ///< �м����Ĵ��ڲ���
	const char *m_pFieldBusName;///< �ֳ����߿���������
	C_CONNECT *m_pConnection;///< �ֳ����߿�������������
	C_CmdFilter *m_pCmdFilter; ///< ���������
	C_CmdVisitor *m_pCmdVisitor; ///< ���������:���Լ֡��
	S_FBCONFIG m_FBusConfig; ///< �ֳ���������
};

#endif//__BASEB_FIELDBUS_H__

