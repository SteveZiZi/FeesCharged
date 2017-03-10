/** @file
 *  @brief ���������ඨ��
 *  @author 
 *  @date 2011/11/28
 *  @version 0.1
 *  @note
 */
#ifndef __FND_CONN_COM_H__
#define __FND_CONN_COM_H__

#include "basedef.h"
#include "connect.h"

/** @class
 *  @brief ���������ඨ��
 *  @note
 */
class C_COMCONN:public C_CONNECT
{
public:
	C_COMCONN(const S_CHANNEL &Channel);
	~C_COMCONN();

	/** @brief ���ö˿�ӳ��� 
	 *  @param[in] pPortMap �˿�ӳ���
	 *  @return void
	 */
	virtual void SetPortMap(const S_BYTE *pPortMap){m_pPortMap = pPortMap;}

	/** @brief ȡͨ����Ϣ
	 *  @param[in] fVirtual true ����ͨ��;false ����ͨ��
	 *  @return ͨ����Ϣ
	 */
	virtual S_CHANNEL GetChannelInfo(bool fVirtual);
	/** @brief ������뻺�� 
	 *  @return true �ɹ���false ʧ��
	 */
	virtual S_BOOL ClearInBuffer(void);
	/** @brief ���������� 
	 *  @return true �ɹ���false ʧ��
	 */
	virtual S_BOOL ClearOutBuffer(void);

	/** @ȡ�������Թ�����
	 *  @param[in] pPropertyName ��������Ŀǰ֧��ppp0, eth0
	 *  @param[out] pBuffer �������
	 *  @param[in] size ��������С
	 *  @return -1 ����;0�ɹ�;>0 ��Ҫ�Ļ����С
	 *  @remarks ppp0��eth0�Ļ�����S_CHANNEL
	 */
	virtual int GetProperty(const char *pPropertyName, void *pBuffer, S_WORD size);

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

protected:
	/** @brief ��ʼ������ 
	 *  @return -1 ����>=0 ��Ч����������
	 */
	int InitCom(void);
	/** @brief ���ô��� 
	 *  @return true �ɹ���false ʧ��
	 */
	S_BOOL SetCom(void);
	/** @brief ��ȡ���������� 
	 *  @param[in] Baud ��������ֵ
	 *  @return B115200,B57600,B38400, B19200, B9600, B4800, B2400, B1200,B600, B300
	 */
	int GetBaudAttr(int Baud);

protected:
	int m_Comfd; ///< ���ھ��
	const S_BYTE *m_pPortMap; ///< �˿�ӳ���
};


#endif //__FND_CONN_COM_H__


