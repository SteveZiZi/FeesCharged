/** @file
 *  @brief PSTN连接类定义
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
 *  @brief PSTN连接类定义
 *  @note
 */
class C_PSTNCONN: public C_CONNECT
{
public:
	C_PSTNCONN(const S_CHANNEL &channel);
	~C_PSTNCONN();

	/** @brief 设置读取钩子函数 */
	virtual void SetReadHook(PFN_RWHOOK pfnReadHook);
	/** @brief 设置写入钩子函数 */
	virtual void SetWritHook(PFN_RWHOOK pfnWritHook);
	/** @brief 设置错误输出钩子函数 */
	virtual void SetErrorHook(PFN_ERROUT pfnErrorOut);
	/** @brief 设置端口映射表 
	 *  @param[in] pPortMap 端口映射表
	 *  @return void
	 */
	virtual void SetPortMap(const S_BYTE *pPortMap);

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
	int m_ConnectState; ///< 连接状态
	int m_ConnectedBaud; ///< 当前PSTN连接的波特率
	int m_HardwareRingCnt; ///< 硬件响铃次数
	bool m_ModemPowerOn; ///< MODEM上电状态
	S_TIME m_timeConnectBegin; ///< 开始建链时间
	S_TIME m_timeDetectRingTimeout; ///< 检测响铃超时（空闲超时
	
	const S_BYTE *m_pPortMap; ///< 端口映射表
	C_MODEM *m_pModem; ///< PSTN MODEM
	S_CHANNEL m_ModemChannel; ///< MODEM 端口号
	C_CONNECT *m_pModemConnect; ///< MODEM 连接
};

#endif //__FND_CONN_PSTN_H__


