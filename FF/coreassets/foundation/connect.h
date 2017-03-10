/** @file
 *  @brief 连接抽象类定义
 *  @author 
 *  @date 2011/11/28
 *  @version 0.1
 *  @note
 *  重点处理好两个钩子函数，一个是读写钩子，一个错误输出。参见后面“钩子函数模板”。
 *  采用钩子的目的主要是与应用层解耦。
 */
#ifndef __FND_CONNECT_H__
#define __FND_CONNECT_H__

#include "basedef.h"
#include "channel.h"

#define NET_ETH0_DEV_NAME		"eth0"
#define NET_ETH1_DEV_NAME		"eth1"
#define NET_PPP0_DEV_NAME		"ppp0"

/** @brief 读写钩子函数类型定义 */
typedef void (*PFN_RWHOOK)(S_CHANNEL *pChannel, const string &strData);
/** @brief 错误输出函数类型定义 */
typedef void (*PFN_ERROUT)(bool fWarnLevel, const string &strErrorInfo);

/** @class
 *  @brief 连接超类定义
 *  @remarks 请注意钩子函数的实现
 */
class C_CONNECT
{
public:
	C_CONNECT(const S_CHANNEL &channel);
	virtual ~C_CONNECT();

	/** @brief 连接
	 *  @return 0 成功
	 *  @return -1 失败
	 *  @return >0 距离下次开始连接的秒数
	 */
	int Connect(void);
	/** @brief 断开连接
	 *  @return true 成功
	 *  @return false 失败
	 */
	S_BOOL Disconnect(void);
	/** @brief 连接状态
	 *  @return true 处在连接中
	 *  @return false 未连接
	 */
	S_BOOL IsConnected(void);
	/** @brief 发送数据
	 *  @param[in] strData 数据缓冲
	 *  @return -1 发送出错
	 *  @return >=0 发送的字节数
	 */
	int SendData(const string &strData);
	/** @brief 接收数据
	 *  @param[out] strData 数据输出缓冲
	 *  @return -1 接收出错
	 *  @return >=0 接收的字节数
	 */
	int RecvData(string &strData);
	/** @brief 设置读取钩子函数 */
	virtual void SetReadHook(PFN_RWHOOK pfnReadHook){m_pfnReadHook = pfnReadHook;}
	/** @brief 设置写入钩子函数 */
	virtual void SetWritHook(PFN_RWHOOK pfnWritHook){m_pfnWritHook = pfnWritHook;}
	/** @brief 设置错误输出钩子函数 */
	virtual void SetErrorHook(PFN_ERROUT pfnErrorOut){m_pfnErrorOut = pfnErrorOut;}

	/** @brief 更改连接属性
	 *  @param[in] Channel 通道信息
	 *  @return true 成功；false 失败
	 *  @remarks 设置失败时有可能会导致连接中断
	 */
	bool ChangeProperty(const S_CHANNEL &Channel);

	/** @brief 设置通道信息
	 *  @param[in] Channel 通道信息
	 *  @return true 成功；false 失败
	 */
	bool SetChannelInfo(const S_CHANNEL &Channel);

	/** @brief 设置端口映射表 
	 *  @param[in] pPortMap 端口映射表
	 *  @return void
	 */
	virtual void SetPortMap(const S_BYTE *pPortMap){}

	/** @brief 取通道信息
	 *  @param[in] fVirtual true 虚拟通道;false 物理通道
	 *  @return 通道信息
	 */
	virtual S_CHANNEL GetChannelInfo(bool fVirtual){return m_Channel;}

	/** @brief 清除输入缓冲 */
	virtual S_BOOL ClearInBuffer(void){return true;}
	/** @brief 清除输出缓冲 */
	virtual S_BOOL ClearOutBuffer(void){return true;}

	/** @取连接属性工具箱
	 *  @param[in] pPropertyName 属性名，目前支持ppp0, eth0
	 *  @param[out] pBuffer 输出缓冲
	 *  @param[in] size 输出缓冲大小
	 *  @return -1 出错;0成功;>0 需要的缓冲大小
	 *  @remarks ppp0和eth0的缓冲是S_CHANNEL
	 */
	virtual int GetProperty(const char *pPropertyName, void *pBuffer, S_WORD size){return -1;}

protected:
	/** @brief 建立连接 */
	virtual S_BOOL OnConnect(void) = 0;
	/** @brief 断开连接 */
	virtual S_BOOL OnDisconnect(void) = 0;
	/** @brief 连接成功判断 */
	virtual S_BOOL ConnectSuccessful(void) = 0;
	/** @brief 阅读数据 */
	virtual int ReadData(S_BYTE *pData, int size) = 0;
	/** @brief 写入数据 */
	virtual int WriteData(const S_BYTE *pData, int size) = 0;
	/** @brief 写数据延迟 */
	virtual int WriteDataDelay(int SendByte){return 0;}

	/** @brief 错误输出
	 *  @param[in] fWarnLevel [true|false]是否按告警级别输出
	 *  @param[in] strErrorInfo 输出错误信息
	 *  @return >=0 输出的字节数
	 */
	int ErrorOut(bool fWarnLevel, const string &strErrorInfo);
	/** @brief 系统错误输出(errno)
	 *  @return >=0 输出的字节数
	 */
	int SystemError(const string &strPrompt);

protected:
	S_TIME m_timeLastConnect; ///< 最后一次连接时间
	S_CHANNEL m_Channel; ///< 连接通道
	PFN_RWHOOK m_pfnReadHook; ///< 连接读的钩子函数
	PFN_RWHOOK m_pfnWritHook; ///< 连接写的钩子函数
	PFN_ERROUT m_pfnErrorOut; ///< 错误输出函数
};


/** 钩子函数模板
void PFN_ReadHOOK(S_CHANNEL *pChannel, const string &strData)
{
	if (!strData.empty())
		logvt<<pChannel->m_Name<<" send:"<<HEX<<strData<<endl;
}
void PFN_WritHOOK(S_CHANNEL *pChannel, const string &strData)
{
	if (!strData.empty())
		logvt<<pChannel->m_Name<<" recv:"<<HEX<<strData<<endl;
}
void PFN_ErrorOut(bool fWarnLevel, const string &strErrorInfo)
{
	if (!strErrorInfo.empty())
	{
		if (fWarnLevel == true) logwt<<strErrorInof<<endl;
		else logit<<strErrorInof<<endl;
	}
}
*/

class C_IfConfig
{
public:
	static S_DWORD GetNetDeviceIP(const char *pNetDev);
	static S_DWORD GetSimIP(void){return GetNetDeviceIP(NET_PPP0_DEV_NAME);}
	static S_DWORD GetEth0IP(void){return GetNetDeviceIP(NET_ETH0_DEV_NAME);}
	static S_DWORD GetEth1IP(void){return GetNetDeviceIP(NET_ETH1_DEV_NAME);}

	static bool GetNetDeviceMac(const char *pNetDev, char *pMac, int size);
	static bool GetEth0Mac(char *pMac, int size){return GetNetDeviceMac(NET_ETH0_DEV_NAME, pMac, size);}
	static bool GetEth1Mac(char *pMac, int size){return GetNetDeviceMac(NET_ETH1_DEV_NAME, pMac, size);}
};

#endif //__FND_CONNECT_H__


