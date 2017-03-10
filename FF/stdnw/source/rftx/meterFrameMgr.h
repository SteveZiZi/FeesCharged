/** @file
 *  @brief ���ȫ��֤ϵ�й�Լ������
 *  @author 
 *  @date 2016/12/06
 *  @version 0.1
 *  @note ���ȫ��֤ϵ�й�Լ�����ඨ���ʵ��
 */
#ifndef __METER_FRAMEMGR_H__
#define __METER_FRAMEMGR_H__

#include <string>
#include <vector>
#include "sysbase.h"

#define METER_INDETIFY				0x070000FF							// ��������֤
#define METER_GET_MODE				0x04000501							// ��ȡ���ģʽ
#define METER_ACCOUNT				0x070101FF							// ����ؿ���
#define METER_REMOTE_ACCOUNT
#define METER_WALLET_CHARGE			0x070102FF							// ���Ǯ����ֵ
#define METER_WALLET_INIT			0x070103FF							// Ǯ����ʼ��
#define METER_UPDATE_INFO
#define METER_REMOTE_CTRL
#define METER_GET_BACK_INFO			0x078001FF							// ���ݻس�

/** @class
 *  @brief ���ȫ��֤ϵ�й�Լ������
 */
class C_MeterFrameMgr
{
public:
	C_MeterFrameMgr(void);
	virtual ~C_MeterFrameMgr();

public:
	/** @brief ����Dbcj��������ݲ����з���
	*  @param[in] strCmd ����DI
	*  @return true �ɹ���false ʧ��
	*/
	bool RequestDbcjMsg(const S_DWORD &CmdDI);
	/** @brief ����8�ֽڿ��˱�� 
	 *  @return true �ɹ���false ʧ��
	 */
	bool SetCardMeterNo(S_CHAR CardMeterNo[8]);
	/** @brief ����8�ֽڿ�������� 
	 *  @return true �ɹ���false ʧ��
	 */
	bool SetCardRandData(S_CHAR CardRandData[8]);
	/** @brief ����8�ֽڿ������� 
	 *  @return true �ɹ���false ʧ��
	 */
	bool SetCardDstData(S_CHAR CardDstData[8]);
	/** @brief ����4�ֽڿ���MAC1
	 *  @return true �ɹ���false ʧ��
	 */
	bool SetCardMac1(S_CHAR CardMac1[4]);
	/** @brief ����6�ֽڿ��˿ͻ���� 
	 *  @return true �ɹ���false ʧ��
	 */
	bool SetCardCustomerNo(S_CHAR CardCustomerNo[6]);
	/** @brief ����4�ֽڿ���MAC2 
	 *  @return true �ɹ���false ʧ��
	 */
	bool SetCardMac2(S_CHAR CardMac2[4]);
	/** @brief ��ȡ���8�ֽ�ģ��easm�����
	 *  @return true �ɹ���false ʧ��
	 */
	bool GetMeterEsamRand(S_CHAR *pMeterEsamRand,S_BYTE buflen);
	/** @brief ��ȡ���4�ֽ������
	 *  @return true �ɹ���false ʧ��
	 */
	bool GetMeterRand(S_CHAR *pMeterRand,S_BYTE buflen);
	/** @brief ��ȡ���ģʽ
	 *  @return true �ɹ���false ʧ��
	 */
	S_CHAR GetMeterMode() {return m_MeterMode;}
	/** @brief ��ȡ���MAC
	 *  @return true �ɹ���false ʧ��
	 */
	bool GetMeterMac(S_CHAR *pMac,S_BYTE buflen);
	/** @brief ��ȡ��˷�д��Ϣ
	 *  @return true �ɹ���false ʧ��
	 */
	bool GetMeterBackInfo(S_CHAR *pMeterBackInfo,S_BYTE buflen);

	/** @brief ���ò�����Ϣ
	*  @param[in] strCmd ����DI
	*  @return none
	*/
	void ResetParam();

private:
	/** @brief ��֡
	*  @param[in] strCmd ����DI
	*  @param[in] strAddress ��Ƶ�ַ
	*  @param[in] strData �������뻺��
	*  @param[out] SendFrame �����֡
	*  @return ���ݳ���
	*/
	int Make645Frame(const S_DWORD &CmdDI,string& SendFrame);
	/** @brief ��dbcj������������
	*  @param[in] SendstrFrame ��dbcj�����͵�֡
	*  @param[out] strResponse �յ�dbcj����Ļظ�֡
	*  @return ���ݳ���
	*/
	bool RequestRftxMsg2Dbcj(string &SendstrFrame, string &strResponse);
	/** @brief ����������[����������ı���]
	*  @param[in] strCmd ����
	*  @param[in] strAddress ��Ƶ�ַ
	*  @param[in] DI ͨ�üĴ���
	*  @param[out] strData �����������
	*  @return ���ݳ���
	*/
	int Parse645Frame(S_DWORD &CmdDI,string &MeterAddr,string &strData,string& ReceiveFrame);
	/** @brief ��ȡ����֡
	*  @param[in] strBuffer �����
	*  @param[out] strCmd ֡�������
	*  @return <0: ����[����ֵ�ǳ�����ֽ���]; =0: ��֡; >0: �ɹ�
	*/
	int Check645FrameFormat(string &ReceiveFrame, string &RevFrame);
	/** @brief ��ȡ������
	*  @param[in] none
	*  @return ������
	*/
	string GetOperatorCode();
	/** @brief ��ȡ���ַ
	*  @param[in] none
	*  @return ���ַ
	*/
	string GetMeterAddr();
	/** @brief ��֡����ͷ
	*  @param[in] strCmd ����
	*  @return ����ͷ
	*/
	string MakeFrameHead(const S_DWORD& CmdDI);
	/** @brief ��֡����DI
	*  @param[in] strCmd ����
	*  @return ����DI
	*/
	string MakeFrameDI(const S_DWORD& CmdDI);
	/** @brief ��֡��������
	*  @param[in] strCmd ����
	*  @return ��������
	*/
	string MakeFrameData(const S_DWORD& CmdDI);
	/** @brief ��֡����β
	*  @param[out] TempFrame ��������
	*  @return none
	*/
	void MakeFrameEnd(string& TempFrame);
	/** @brief �ж�dbcj�ظ����ĵ���ȷ��
	*  @param[in] strCmd ����
	*  @param[out] StrResponse �ظ��ı���
	*  @param[out] Flag �ظ��ı��ĵ���ȷ��
	*  @return ���ر��Ŀ�ʼ������
	*/
	S_DWORD JudgeDbcjResult(const S_DWORD& CmdDI,string &StrResponse,bool& Flag);
	/** @brief ������ȫ��֤ʱ�Ĵ�����Ϣ
	*  @param[in] Error SEER
	*  @return 
	*/
	int AnalysisErrorCode(const S_WORD &Error);
	/** @brief dbcj������ȷ���ݵ�����»�ȡ��Ӧ�Ľ������
	*  @param[in] Index ���ر��Ŀ�ʼ������
	*  @param[in] strCmd ����DI
	*  @param[out] StrResponse �ظ��ı���
	*  @return none
	*/
	void AnalysisDbcjResult(S_DWORD Index,const S_DWORD& CmdDI,string &StrResponse);

private:
	// Meter
	S_CHAR				m_MeterEsamRand[8];								// 8�ֽ�ģ��easm�����  �ɵ����
	S_CHAR				m_MeterRand[4];									// 4�ֽ������  �ɵ����
	S_CHAR				m_MeterMode;									// ���ģʽ
	S_CHAR				m_MeterAddr[8];									// ����ַ
	S_CHAR				m_MeterMAC[4];									// MAC �ɵ����
	S_CHAR				m_MeterBackInfo[50];							// ��д��Ϣ �ɵ����

	// From Card
	S_CHAR				m_CardMeterNo[8];								// ���
	S_CHAR				m_CardRandData[8];								// 8�ֽ�ģ��ESAM�����
	S_CHAR				m_CardDstData[8];								// 8�ֽ�����
	S_CHAR				m_CardMAC1[4];
	S_CHAR				m_CardCustomerNo[6];							// 6�ֽ�bcd,�ͻ����
	S_CHAR				m_CardMAC2[4];
};

#endif//__METER_FRAMEMGR_H__


