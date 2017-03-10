/** @file
 *  @brief RFID管理类
 *  @author 
 *  @date 2016/12/06
 *  @version 0.1
 *  @note RFID类定义和实现
 */
#ifndef __RFID_MGR_H__
#define __RFID_MGR_H__

#include "rfidCom.h"

#define QUERY_READER_VER			0x3111								// 获取版本号
#define BUZZER_CURL					0x3113								// 控制蜂鸣器
#define LED_STATUS_CTRL				0x3114								// 控制LED
#define READ_CARD_INFO				0x4010								// 读取卡片信息
#define IDENTIFY_AUTH				0x4011								// 身份认证
#define START_CARD_HANDEL			0x4012								// 开始交易
#define END_CARD_HANDEL				0x4013								// 结束交易	
#define PARAM_SET					0x4014								// 参数设置
#define REMOTE_HANDEL				0x4015								// 远程交易
#define REMOTE_IDENTIFY_AUTH		0x4016								// 远程身份认证
#define KEY_UPDATE					0x4017								// 更新密钥

/** @class
 *  @brief RFID管理
 */
class C_RFIDMgr
{
public:
	C_RFIDMgr(void);
	virtual ~C_RFIDMgr();

public:
	/** @brief 初始化RFID通道参数 
	 *  @return none
	 */
	void InitComParam(void);
	/** @brief 初始化RFID通道 
	 *  @return -1:失败 0 成功
	 */
	int InitRFIDComm(void);
	/** @brief 从卡片中获取信息 
	*  @return -1 出错；0 正确
	*/
	int ReadInfoFromCard(const S_WORD& CmdParam);
	/** @brief 获取卡序号 
	 *  @return true 成功；false 失败
	 */
	bool GetCardSerialNo(S_CHAR *pSerialnobuf, S_BYTE buflen);
	/** @brief 获取卡端信息中的表号 
	 *  @return true 成功；false 失败
	 */
	bool GetMeterNo(S_CHAR *pMeterNo,S_BYTE buflen);
	/** @brief 获取卡端8字节随机数 
	 *  @return true 成功；false 失败
	 */
	bool GetEsamRandData(S_CHAR *pesamRandData,S_BYTE buflen);
	/** @brief 获取8字节密文 
	 *  @return true 成功；false 失败
	 */
	bool GetDstData(S_CHAR *pdstData,S_BYTE buflen);
	/** @brief 获取4字节MAC1 
	 *  @return true 成功；false 失败
	 */
	bool GetMAC1(S_CHAR *pmac,S_BYTE buflen);
	/** @brief 获取客户序列号 
	 *  @return true 成功；false 失败
	 */
	bool GetCustomerNo(S_CHAR *pcustomerNo,S_BYTE buflen);
	/** @brief 获取4字节MAC2 
	 *  @return true 成功；false 失败
	 */
	bool GetMAC2(S_CHAR *pmac,S_BYTE buflen);
	/** @brief 设置8字节模块easm随机数 
	 *  @return true 成功；false 失败
	 */
	bool SetMeterEsamRand(S_CHAR meterEasmRand[8]);
	/** @brief 设置4字节电表返回随机数 
	 *  @return true 成功；false 失败
	 */
	bool SetMeterRand(S_CHAR meterRand[4]);
	/** @brief 设置4字节MAC 
	 *  @return true 成功；false 失败
	 */
	bool SetMac(S_CHAR mac[4]);
	/** @brief 设置50字节返写信息 
	 *  @return true 成功；false 失败
	 */
	bool SetBackInfo(S_CHAR backInfo[50]);
	/** @brief 重置卡端信息
	 *  @return none
	 */
	void ResetCardInfo(void);
	/** @brief 断开连接 
	*  @return true 成功；false 失败
	*/
	S_BOOL Disconnect(void);

private:
	/** @brief 获取读卡器版本信息 
	*  @return -1 出错；0 正确
	*/
	int QueryReaderVerInfo(const S_WORD& CmdParam,string& SstrData,string& RstrData);
	/** @brief 控制蜂鸣器 
	*  @return -1 出错；0 正确
	*/
	int BuzzerCtrl(const S_WORD& CmdParam,string& SstrData,string& RstrData);
	/** @brief 控制LED状态
	*  @return -1 出错；0 正确
	*/
	int LedStatusCtrl(const S_WORD& CmdParam,string& SstrData,string& RstrData);
	/** @brief 读取卡片基本信息
	*  @return -1 出错；0 正确
	*/
	int ReadCardInfo(const S_WORD& CmdParam,string& SstrData,string& RstrData);
	/** @brief 卡片身份认证
	*  @return -1 出错；0 正确
	*/
	int IdentifyAuth(const S_WORD& CmdParam,string& SstrData,string& RstrData);
	/** @brief 开始卡片交易
	*  @return -1 出错；0 正确
	*/
	int StartCardHandle(const S_WORD& CmdParam,string& SstrData,string& RstrData);
	/** @brief 结束卡片交易 --信息返写
	*  @return -1 出错；0 正确
	*/
	int EndCardHandle(const S_WORD& CmdParam,string& SstrData,string& RstrData);
	/** @brief 参数设置
	*  @return -1 出错；0 正确
	*/
	int ParamSet(const S_WORD& CmdParam,string& SstrData,string& RstrData);
	/** @brief 远程交易
	*  @return -1 出错；0 正确
	*/
	int RemoteHandle(const S_WORD& CmdParam,string& SstrData,string& RstrData);
	/** @brief 远程身份认证
	*  @return -1 出错；0 正确
	*/
	int RemoteIdentityAuth(const S_WORD& CmdParam,string& SstrData,string& RstrData);
	/** @brief 密钥更新
	*  @return -1 出错；0 正确
	*/
	int KeyUpdate(const S_WORD& CmdParam,string& SstrData,string& RstrData);
	/** @brief 卡片发起交易成功后获取卡片上的信息
	*  @return false 出错；true 正确
	*/
	bool GetCardInfo(string &strResponse);

private:
	int					m_CurCardMp;
	C_RFIDCom			m_RFIDCom;

	// Card
	S_CHAR				m_CardSerialNo[8];								// 卡序号
	S_CHAR				m_CardMeterNo[8];								// 表号
	S_CHAR				m_CardRandData[8];								// 8字节模块ESAM随机数
	S_CHAR				m_CardDstData[8];								// 8字节密文
	S_CHAR				m_CardMAC1[4];
	S_CHAR				m_CardCustomerNo[6];							// 6字节bcd,客户编号
	S_CHAR				m_CardMAC2[4];

	// From Meter
	S_CHAR				m_MeterEsamRand[8];								// 8字节模块easm随机数  由电表返回
	S_CHAR				m_MeterRand[4];									// 4字节随机数  由电表返回
	S_CHAR				m_MeterMAC[4];									// MAC 由电表返回
	S_CHAR				m_MeterBackInfo[50];							// 返写信息 由电表返回
};

#endif//__RFID_MGR_H__


