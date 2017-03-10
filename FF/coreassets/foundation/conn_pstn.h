/** @file
 *  @brief PSTN�����ඨ��
 *  @author 
 *  @date 2011/11/28
 *  @version 0.1
 *  @note
 */
#ifndef __FND_CONN_PSTN_H__
#define __FND_CONN_PSTN_H__

#include "basedef.h"
#include "connect.h"
#include "modem.h"

enum
{
	PSTN_CONNECT_STATE_INIT,
	PSTN_CONNECT_STATE_CHECK,
	PSTN_CONNECT_STATE_RING,
	PSTN_CONNECT_STATE_FINISH,

	PSTN_CONNECT_STATE_MAX
};

/** @class
 *  @brief PSTN�����ඨ��
 *  @note
 */
class C_PSTNCONN: public C_CONNECT
{
public:
	C_PSTNCONN(const S_CHANNEL &channel);
	~C_PSTNCONN();

	/** @brief ���ö�ȡ���Ӻ��� */
	virtual void SetReadHook(PFN_RWHOOK pfnReadHook);
	/** @brief ����д�빳�Ӻ��� */
	virtual void SetWritHook(PFN_RWHOOK pfnWritHook);
	/** @brief ���ô���������Ӻ��� */
	virtual void SetErrorHook(PFN_ERROUT pfnErrorOut);
	/** @brief ���ö˿�ӳ��� 
	 *  @param[in] pPortMap �˿�ӳ���
	 *  @return void
	 */
	virtual void SetPortMap(const S_BYTE *pPortMap);

protected:
	/** @brief �������� 
	 *  @return true �ɹ���false ʧ��
	 */
	S_BOOL OnConnect(void);
	/** @brief �Ͽ����� 
	 *  @return true �ɹ���false ʧ��
	 */
	S_BOOL OnDisconnect(void);
	/** @brief �ж�����״̬ 
	 *  @return true ���ӣ�false δ����
	 */
	S_BOOL ConnectSuccessful(void);
	/** @brief ��ȡ���� 
	 *  @param[out] pData �������
	 *  @param[in] size ��������С
	 *  @return -1 ����>=0 ��ȡ���ֽ���
	 */
	int ReadData(S_BYTE *pData, int size);
	/** @brief д������ 
	 *  @param[in] pData ���뻺��
	 *  @param[in] size ���뻺���С
	 *  @return -1 ����>=0 д����ֽ���
	 */
	int WriteData(const S_BYTE *pData, int size);
	/** @brief д���ݺ���ʱ 
	 *  @param[in] SendByte �ֽ���
	 *  @return 0 �����ʱ
	 */
	int WriteDataDelay(int SendByte);

	/** @ȡ�������Թ�����
	 *  @param[in] pPropertyName ��������Ŀǰ֧��ppp0, eth0
	 *  @param[out] pBuffer �������
	 *  @param[in] size ��������С
	 *  @return -1 ����;0�ɹ�;>0 ��Ҫ�Ļ����С
	 *  @remarks ppp0��eth0�Ļ�����S_CHANNEL
	 */
	virtual int GetProperty(const char *pPropertyName, void *pBuffer, S_WORD size);

protected:
	bool PstnInit(void);
	bool PstnCheckEx(void);
	bool PstnCheck(void);
	bool PstnDetectRing(void);
	bool PstnDetectRingTimeout(void);
	bool PstnFinish(void);

	bool CheckModem(void);
	bool PstnDetectRing_ModemPowerOn(void);
	bool PstnDetectRing_ModemPowerOff(void);

	bool CheckHardwareRing(void);
	bool WakeupModem(void);
	int CheckConnection(void);

protected:
	int m_ConnectState; ///< ����״̬
	int m_ConnectedBaud; ///< ��ǰPSTN���ӵĲ�����
	int m_HardwareRingCnt; ///< Ӳ���������
	bool m_ModemPowerOn; ///< MODEM�ϵ�״̬
	S_TIME m_timeConnectBegin; ///< ��ʼ����ʱ��
	S_TIME m_timeDetectRingTimeout; ///< ������峬ʱ�����г�ʱ
	
	const S_BYTE *m_pPortMap; ///< �˿�ӳ���
	C_MODEM *m_pModem; ///< PSTN MODEM
	S_CHANNEL m_ModemChannel; ///< MODEM �˿ں�
	C_CONNECT *m_pModemConnect; ///< MODEM ����
};

#endif //__FND_CONN_PSTN_H__


