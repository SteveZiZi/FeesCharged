/** @file
 *  @brief RFID������
 *  @author 
 *  @date 2016/12/06
 *  @version 0.1
 *  @note RFID�ඨ���ʵ��
 */
#ifndef __RFID_MGR_H__
#define __RFID_MGR_H__

#include "rfidCom.h"

#define QUERY_READER_VER			0x3111								// ��ȡ�汾��
#define BUZZER_CURL					0x3113								// ���Ʒ�����
#define LED_STATUS_CTRL				0x3114								// ����LED
#define READ_CARD_INFO				0x4010								// ��ȡ��Ƭ��Ϣ
#define IDENTIFY_AUTH				0x4011								// �����֤
#define START_CARD_HANDEL			0x4012								// ��ʼ����
#define END_CARD_HANDEL				0x4013								// ��������	
#define PARAM_SET					0x4014								// ��������
#define REMOTE_HANDEL				0x4015								// Զ�̽���
#define REMOTE_IDENTIFY_AUTH		0x4016								// Զ�������֤
#define KEY_UPDATE					0x4017								// ������Կ

/** @class
 *  @brief RFID����
 */
class C_RFIDMgr
{
public:
	C_RFIDMgr(void);
	virtual ~C_RFIDMgr();

public:
	/** @brief ��ʼ��RFIDͨ������ 
	 *  @return none
	 */
	void InitComParam(void);
	/** @brief ��ʼ��RFIDͨ�� 
	 *  @return -1:ʧ�� 0 �ɹ�
	 */
	int InitRFIDComm(void);
	/** @brief �ӿ�Ƭ�л�ȡ��Ϣ 
	*  @return -1 ����0 ��ȷ
	*/
	int ReadInfoFromCard(const S_WORD& CmdParam);
	/** @brief ��ȡ����� 
	 *  @return true �ɹ���false ʧ��
	 */
	bool GetCardSerialNo(S_CHAR *pSerialnobuf, S_BYTE buflen);
	/** @brief ��ȡ������Ϣ�еı�� 
	 *  @return true �ɹ���false ʧ��
	 */
	bool GetMeterNo(S_CHAR *pMeterNo,S_BYTE buflen);
	/** @brief ��ȡ����8�ֽ������ 
	 *  @return true �ɹ���false ʧ��
	 */
	bool GetEsamRandData(S_CHAR *pesamRandData,S_BYTE buflen);
	/** @brief ��ȡ8�ֽ����� 
	 *  @return true �ɹ���false ʧ��
	 */
	bool GetDstData(S_CHAR *pdstData,S_BYTE buflen);
	/** @brief ��ȡ4�ֽ�MAC1 
	 *  @return true �ɹ���false ʧ��
	 */
	bool GetMAC1(S_CHAR *pmac,S_BYTE buflen);
	/** @brief ��ȡ�ͻ����к� 
	 *  @return true �ɹ���false ʧ��
	 */
	bool GetCustomerNo(S_CHAR *pcustomerNo,S_BYTE buflen);
	/** @brief ��ȡ4�ֽ�MAC2 
	 *  @return true �ɹ���false ʧ��
	 */
	bool GetMAC2(S_CHAR *pmac,S_BYTE buflen);
	/** @brief ����8�ֽ�ģ��easm����� 
	 *  @return true �ɹ���false ʧ��
	 */
	bool SetMeterEsamRand(S_CHAR meterEasmRand[8]);
	/** @brief ����4�ֽڵ��������� 
	 *  @return true �ɹ���false ʧ��
	 */
	bool SetMeterRand(S_CHAR meterRand[4]);
	/** @brief ����4�ֽ�MAC 
	 *  @return true �ɹ���false ʧ��
	 */
	bool SetMac(S_CHAR mac[4]);
	/** @brief ����50�ֽڷ�д��Ϣ 
	 *  @return true �ɹ���false ʧ��
	 */
	bool SetBackInfo(S_CHAR backInfo[50]);
	/** @brief ���ÿ�����Ϣ
	 *  @return none
	 */
	void ResetCardInfo(void);
	/** @brief �Ͽ����� 
	*  @return true �ɹ���false ʧ��
	*/
	S_BOOL Disconnect(void);

private:
	/** @brief ��ȡ�������汾��Ϣ 
	*  @return -1 ����0 ��ȷ
	*/
	int QueryReaderVerInfo(const S_WORD& CmdParam,string& SstrData,string& RstrData);
	/** @brief ���Ʒ����� 
	*  @return -1 ����0 ��ȷ
	*/
	int BuzzerCtrl(const S_WORD& CmdParam,string& SstrData,string& RstrData);
	/** @brief ����LED״̬
	*  @return -1 ����0 ��ȷ
	*/
	int LedStatusCtrl(const S_WORD& CmdParam,string& SstrData,string& RstrData);
	/** @brief ��ȡ��Ƭ������Ϣ
	*  @return -1 ����0 ��ȷ
	*/
	int ReadCardInfo(const S_WORD& CmdParam,string& SstrData,string& RstrData);
	/** @brief ��Ƭ�����֤
	*  @return -1 ����0 ��ȷ
	*/
	int IdentifyAuth(const S_WORD& CmdParam,string& SstrData,string& RstrData);
	/** @brief ��ʼ��Ƭ����
	*  @return -1 ����0 ��ȷ
	*/
	int StartCardHandle(const S_WORD& CmdParam,string& SstrData,string& RstrData);
	/** @brief ������Ƭ���� --��Ϣ��д
	*  @return -1 ����0 ��ȷ
	*/
	int EndCardHandle(const S_WORD& CmdParam,string& SstrData,string& RstrData);
	/** @brief ��������
	*  @return -1 ����0 ��ȷ
	*/
	int ParamSet(const S_WORD& CmdParam,string& SstrData,string& RstrData);
	/** @brief Զ�̽���
	*  @return -1 ����0 ��ȷ
	*/
	int RemoteHandle(const S_WORD& CmdParam,string& SstrData,string& RstrData);
	/** @brief Զ�������֤
	*  @return -1 ����0 ��ȷ
	*/
	int RemoteIdentityAuth(const S_WORD& CmdParam,string& SstrData,string& RstrData);
	/** @brief ��Կ����
	*  @return -1 ����0 ��ȷ
	*/
	int KeyUpdate(const S_WORD& CmdParam,string& SstrData,string& RstrData);
	/** @brief ��Ƭ�����׳ɹ����ȡ��Ƭ�ϵ���Ϣ
	*  @return false ����true ��ȷ
	*/
	bool GetCardInfo(string &strResponse);

private:
	int					m_CurCardMp;
	C_RFIDCom			m_RFIDCom;

	// Card
	S_CHAR				m_CardSerialNo[8];								// �����
	S_CHAR				m_CardMeterNo[8];								// ���
	S_CHAR				m_CardRandData[8];								// 8�ֽ�ģ��ESAM�����
	S_CHAR				m_CardDstData[8];								// 8�ֽ�����
	S_CHAR				m_CardMAC1[4];
	S_CHAR				m_CardCustomerNo[6];							// 6�ֽ�bcd,�ͻ����
	S_CHAR				m_CardMAC2[4];

	// From Meter
	S_CHAR				m_MeterEsamRand[8];								// 8�ֽ�ģ��easm�����  �ɵ����
	S_CHAR				m_MeterRand[4];									// 4�ֽ������  �ɵ����
	S_CHAR				m_MeterMAC[4];									// MAC �ɵ����
	S_CHAR				m_MeterBackInfo[50];							// ��д��Ϣ �ɵ����
};

#endif//__RFID_MGR_H__


