/** @file
 *  @brief 网络连接类定义
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
 *  @brief 网络连接类定义
 *  @note
 *  主要分四种情况：TCP客户端，TCP服务器，UDP客户端，UDP服务器
 */
class C_NETCONN: public C_CONNECT
{
public:
	C_NETCONN(const S_CHANNEL &channel);
	~C_NETCONN();

protected:
	/** @brief 建立连接 
	 *  @return true 成功；false 失败
	 */
	S_BOOL OnConnect(void);
	/** @brief 断开连接 
	 *  @return true 成功；false 失败
	 */
	S_BOOL OnDisconnect(void);
	/** @brief 判断连接状态 
	 *  @return true 连接；false 未连接
	 */
	S_BOOL ConnectSuccessful(void);
	/** @brief 读取数据 
	 *  @param[out] pData 输出缓冲
	 *  @param[in] size 输出缓冲大小
	 *  @return -1 出错；>=0 读取的字节数
	 */
	int ReadData(S_BYTE *pData, int size);
	/** @brief 写入数据 
	 *  @param[in] pData 输入缓冲
	 *  @param[in] size 输入缓冲大小
	 *  @return -1 出错；>=0 写入的字节数
	 */
	int WriteData(const S_BYTE *pData, int size);
	/** @brief 写数据后延时 
	 *  @param[in] SendByte 字节数
	 *  @return 0 完成延时
	 */
	int WriteDataDelay(int SendByte);

	/** @取连接属性工具箱
	 *  @param[in] pPropertyName 属性名，目前支持ppp0, eth0
	 *  @param[out] pBuffer 输出缓冲
	 *  @param[in] size 输出缓冲大小
	 *  @return -1 出错;0成功;>0 需要的缓冲大小
	 *  @remarks ppp0和eth0的缓冲是S_CHANNEL
	 */
	virtual int GetProperty(const char *pPropertyName, void *pBuffer, S_WORD size);

protected:
	/** @brief TCP模式写入数据 
	 *  @param[in] pData 输入缓冲
	 *  @param[in] size 输入缓冲大小
	 *  @return -1 出错；>=0 写入的字节数
	 */
	int TcpWriteData(const S_BYTE *pData, int size);
	/** @brief UDP模式写入数据 
	 *  @param[in] pData 输入缓冲
	 *  @param[in] size 输入缓冲大小
	 *  @return -1 出错；>=0 写入的字节数
	 */
	int UdpWriteData(const S_BYTE *pData, int size);
	/** @brief TCP模式读取数据 
	 *  @param[out] pData 输出缓冲
	 *  @param[in] size 输出缓冲大小
	 *  @return -1 出错；>=0 读取的字节数
	 */
	int TcpReadData(S_BYTE *pData, int size);
	/** @brief UDP模式读取数据 
	 *  @param[out] pData 输出缓冲
	 *  @param[in] size 输出缓冲大小
	 *  @return -1 出错；>=0 读取的字节数
	 */
	int UdpReadData(S_BYTE *pData, int size);

	/** @brief 关闭句柄 
	 *  @param[in] socket SOCKET句柄
	 *  @return void
	 */
	void CloseSocket(SOCKET socket);
	/** @brief 连接到服务器 
	 *  @param[in] pDstIP 服务器IP
	 *  @param[in] DstPort 服务器端口
	 *  @param[in] timeoutS 连接超时，单位秒
	 *  @return -1 无效连接SOCKET；>=0 有效连接SOCKET
	 */
	SOCKET SocketConnect(const S_CHAR* pDstIP, S_WORD DstPort, int timeoutS);
	/** @brief 侦听端口 
	 *  @param[in] DstPort 本地端口
	 *  @return -1 无效侦听SOCKET；>=0 有效侦听SOCKET
	 */
	SOCKET SocketListen(S_WORD DstPort);
	/** @brief 允许远程连接 
	 *  @param[in] sockfd 侦听SOCKET
	 *  @param[out] RemoteAddr 远端地址输出缓冲
	 *  @param[in] timeoutS 连接超时，单位秒
	 *  @return -1 无效连接SOCKET；>=0 有效连接SOCKET
	 */
	SOCKET SocketAccept(SOCKET sockfd, struct sockaddr_in& RemoteAddr, int timeoutS);

	/** @brief 设置网络活动状态 
	 *  @param[in] sockfd SOCKET句柄
	 *  @return -1 失败；0 成功
	 */
	int SetKeepAlive(SOCKET sockfd);

	/** @brief 取网络设备IP
	 *  @param[in] pNetDev 网络设备名 ppp0, eth0
	 *  @return 设备IP
	 */
	S_DWORD GetNetDevIP(const char *pNetDev);
	/** @brief 取网络设备MAC
	 *  @param[in] pNetDev 网络设备名 ppp0, eth0
	 *  @param[out] pMac 网络设备MAC 6字节
	 *  @return <0失败; >=0成功
	 */
	int GetNetDevMac(const char *pNetDev, char *pMac);

	/** @brief 网络链接检测
	  *  @return true 连接; false 断开
	  */
	bool SocketLinkCheck(void);

protected:
	S_DWORD m_RemoteIP; ///< 远端（客户端）IP
	SOCKET m_CurSocket; ///< SOUKET句柄
};

#endif //__FND_CONN_NET_H__


