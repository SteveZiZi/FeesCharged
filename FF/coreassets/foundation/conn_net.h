/** @file
 *  @brief ���������ඨ��
 *  @author 
 *  @date 2011/11/28
 *  @version 0.1
 *  @note
 */
#ifndef __FND_CONN_NET_H__
#define __FND_CONN_NET_H__

#include "basedef.h"
#include "connect.h"

#ifdef WIN32
	#include <winsock.h>
	#pragma comment(lib, "wsock32.lib")
#else
	#define SOCKET int
#endif

/** @class
 *  @brief ���������ඨ��
 *  @note
 *  ��Ҫ�����������TCP�ͻ��ˣ�TCP��������UDP�ͻ��ˣ�UDP������
 */
class C_NETCONN: public C_CONNECT
{
public:
	C_NETCONN(const S_CHANNEL &channel);
	~C_NETCONN();

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
	/** @brief TCPģʽд������ 
	 *  @param[in] pData ���뻺��
	 *  @param[in] size ���뻺���С
	 *  @return -1 ����>=0 д����ֽ���
	 */
	int TcpWriteData(const S_BYTE *pData, int size);
	/** @brief UDPģʽд������ 
	 *  @param[in] pData ���뻺��
	 *  @param[in] size ���뻺���С
	 *  @return -1 ����>=0 д����ֽ���
	 */
	int UdpWriteData(const S_BYTE *pData, int size);
	/** @brief TCPģʽ��ȡ���� 
	 *  @param[out] pData �������
	 *  @param[in] size ��������С
	 *  @return -1 ����>=0 ��ȡ���ֽ���
	 */
	int TcpReadData(S_BYTE *pData, int size);
	/** @brief UDPģʽ��ȡ���� 
	 *  @param[out] pData �������
	 *  @param[in] size ��������С
	 *  @return -1 ����>=0 ��ȡ���ֽ���
	 */
	int UdpReadData(S_BYTE *pData, int size);

	/** @brief �رվ�� 
	 *  @param[in] socket SOCKET���
	 *  @return void
	 */
	void CloseSocket(SOCKET socket);
	/** @brief ���ӵ������� 
	 *  @param[in] pDstIP ������IP
	 *  @param[in] DstPort �������˿�
	 *  @param[in] timeoutS ���ӳ�ʱ����λ��
	 *  @return -1 ��Ч����SOCKET��>=0 ��Ч����SOCKET
	 */
	SOCKET SocketConnect(const S_CHAR* pDstIP, S_WORD DstPort, int timeoutS);
	/** @brief �����˿� 
	 *  @param[in] DstPort ���ض˿�
	 *  @return -1 ��Ч����SOCKET��>=0 ��Ч����SOCKET
	 */
	SOCKET SocketListen(S_WORD DstPort);
	/** @brief ����Զ������ 
	 *  @param[in] sockfd ����SOCKET
	 *  @param[out] RemoteAddr Զ�˵�ַ�������
	 *  @param[in] timeoutS ���ӳ�ʱ����λ��
	 *  @return -1 ��Ч����SOCKET��>=0 ��Ч����SOCKET
	 */
	SOCKET SocketAccept(SOCKET sockfd, struct sockaddr_in& RemoteAddr, int timeoutS);

	/** @brief ��������״̬ 
	 *  @param[in] sockfd SOCKET���
	 *  @return -1 ʧ�ܣ�0 �ɹ�
	 */
	int SetKeepAlive(SOCKET sockfd);

	/** @brief ȡ�����豸IP
	 *  @param[in] pNetDev �����豸�� ppp0, eth0
	 *  @return �豸IP
	 */
	S_DWORD GetNetDevIP(const char *pNetDev);
	/** @brief ȡ�����豸MAC
	 *  @param[in] pNetDev �����豸�� ppp0, eth0
	 *  @param[out] pMac �����豸MAC 6�ֽ�
	 *  @return <0ʧ��; >=0�ɹ�
	 */
	int GetNetDevMac(const char *pNetDev, char *pMac);

	/** @brief �������Ӽ��
	  *  @return true ����; false �Ͽ�
	  */
	bool SocketLinkCheck(void);

protected:
	S_DWORD m_RemoteIP; ///< Զ�ˣ��ͻ��ˣ�IP
	SOCKET m_CurSocket; ///< SOUKET���
};

#endif //__FND_CONN_NET_H__


