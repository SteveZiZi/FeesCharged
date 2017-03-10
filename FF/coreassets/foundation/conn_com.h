/** @file
 *  @brief 串口连接类定义
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
 *  @brief 串口连接类定义
 *  @note
 */
class C_COMCONN:public C_CONNECT
{
public:
	C_COMCONN(const S_CHANNEL &Channel);
	~C_COMCONN();

	/** @brief 设置端口映射表 
	 *  @param[in] pPortMap 端口映射表
	 *  @return void
	 */
	virtual void SetPortMap(const S_BYTE *pPortMap){m_pPortMap = pPortMap;}

	/** @brief 取通道信息
	 *  @param[in] fVirtual true 虚拟通道;false 物理通道
	 *  @return 通道信息
	 */
	virtual S_CHANNEL GetChannelInfo(bool fVirtual);
	/** @brief 清除输入缓冲 
	 *  @return true 成功；false 失败
	 */
	virtual S_BOOL ClearInBuffer(void);
	/** @brief 清除输出缓冲 
	 *  @return true 成功；false 失败
	 */
	virtual S_BOOL ClearOutBuffer(void);

	/** @取连接属性工具箱
	 *  @param[in] pPropertyName 属性名，目前支持ppp0, eth0
	 *  @param[out] pBuffer 输出缓冲
	 *  @param[in] size 输出缓冲大小
	 *  @return -1 出错;0成功;>0 需要的缓冲大小
	 *  @remarks ppp0和eth0的缓冲是S_CHANNEL
	 */
	virtual int GetProperty(const char *pPropertyName, void *pBuffer, S_WORD size);

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

protected:
	/** @brief 初始化串口 
	 *  @return -1 出错；>=0 有效串口描述符
	 */
	int InitCom(void);
	/** @brief 设置串口 
	 *  @return true 成功；false 失败
	 */
	S_BOOL SetCom(void);
	/** @brief 获取波特率属性 
	 *  @param[in] Baud 波特率数值
	 *  @return B115200,B57600,B38400, B19200, B9600, B4800, B2400, B1200,B600, B300
	 */
	int GetBaudAttr(int Baud);

protected:
	int m_Comfd; ///< 串口句柄
	const S_BYTE *m_pPortMap; ///< 端口映射表
};


#endif //__FND_CONN_COM_H__


