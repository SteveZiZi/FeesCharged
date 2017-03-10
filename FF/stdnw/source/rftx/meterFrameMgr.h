/** @file
 *  @brief 电表安全认证系列规约管理类
 *  @author 
 *  @date 2016/12/06
 *  @version 0.1
 *  @note 电表安全认证系列规约管理类定义和实现
 */
#ifndef __METER_FRAMEMGR_H__
#define __METER_FRAMEMGR_H__

#include <string>
#include <vector>
#include "sysbase.h"

#define METER_INDETIFY				0x070000FF							// 电表身份认证
#define METER_GET_MODE				0x04000501							// 获取电表模式
#define METER_ACCOUNT				0x070101FF							// 电表本地开户
#define METER_REMOTE_ACCOUNT
#define METER_WALLET_CHARGE			0x070102FF							// 电表钱包充值
#define METER_WALLET_INIT			0x070103FF							// 钱包初始化
#define METER_UPDATE_INFO
#define METER_REMOTE_CTRL
#define METER_GET_BACK_INFO			0x078001FF							// 数据回抄

/** @class
 *  @brief 电表安全认证系列规约管理类
 */
class C_MeterFrameMgr
{
public:
	C_MeterFrameMgr(void);
	virtual ~C_MeterFrameMgr();

public:
	/** @brief 请求Dbcj服务的数据并进行分析
	*  @param[in] strCmd 报文DI
	*  @return true 成功，false 失败
	*/
	bool RequestDbcjMsg(const S_DWORD &CmdDI);
	/** @brief 设置8字节卡端表号 
	 *  @return true 成功；false 失败
	 */
	bool SetCardMeterNo(S_CHAR CardMeterNo[8]);
	/** @brief 设置8字节卡端随机数 
	 *  @return true 成功；false 失败
	 */
	bool SetCardRandData(S_CHAR CardRandData[8]);
	/** @brief 设置8字节卡端密文 
	 *  @return true 成功；false 失败
	 */
	bool SetCardDstData(S_CHAR CardDstData[8]);
	/** @brief 设置4字节卡端MAC1
	 *  @return true 成功；false 失败
	 */
	bool SetCardMac1(S_CHAR CardMac1[4]);
	/** @brief 设置6字节卡端客户编号 
	 *  @return true 成功；false 失败
	 */
	bool SetCardCustomerNo(S_CHAR CardCustomerNo[6]);
	/** @brief 设置4字节卡端MAC2 
	 *  @return true 成功；false 失败
	 */
	bool SetCardMac2(S_CHAR CardMac2[4]);
	/** @brief 获取表端8字节模块easm随机数
	 *  @return true 成功；false 失败
	 */
	bool GetMeterEsamRand(S_CHAR *pMeterEsamRand,S_BYTE buflen);
	/** @brief 获取表端4字节随机数
	 *  @return true 成功；false 失败
	 */
	bool GetMeterRand(S_CHAR *pMeterRand,S_BYTE buflen);
	/** @brief 获取电表模式
	 *  @return true 成功；false 失败
	 */
	S_CHAR GetMeterMode() {return m_MeterMode;}
	/** @brief 获取表端MAC
	 *  @return true 成功；false 失败
	 */
	bool GetMeterMac(S_CHAR *pMac,S_BYTE buflen);
	/** @brief 获取表端返写信息
	 *  @return true 成功；false 失败
	 */
	bool GetMeterBackInfo(S_CHAR *pMeterBackInfo,S_BYTE buflen);

	/** @brief 重置参数信息
	*  @param[in] strCmd 报文DI
	*  @return none
	*/
	void ResetParam();

private:
	/** @brief 组帧
	*  @param[in] strCmd 报文DI
	*  @param[in] strAddress 表计地址
	*  @param[in] strData 数据输入缓冲
	*  @param[out] SendFrame 输出的帧
	*  @return 数据长度
	*/
	int Make645Frame(const S_DWORD &CmdDI,string& SendFrame);
	/** @brief 向dbcj服务请求数据
	*  @param[in] SendstrFrame 向dbcj服务发送的帧
	*  @param[out] strResponse 收到dbcj服务的回复帧
	*  @return 数据长度
	*/
	bool RequestRftxMsg2Dbcj(string &SendstrFrame, string &strResponse);
	/** @brief 分析读报文[仅分析传入的报文]
	*  @param[in] strCmd 报文
	*  @param[in] strAddress 表计地址
	*  @param[in] DI 通用寄存器
	*  @param[out] strData 数据输出缓冲
	*  @return 数据长度
	*/
	int Parse645Frame(S_DWORD &CmdDI,string &MeterAddr,string &strData,string& ReceiveFrame);
	/** @brief 提取命令帧
	*  @param[in] strBuffer 命令缓冲
	*  @param[out] strCmd 帧输出缓冲
	*  @return <0: 出错[绝对值是出错的字节数]; =0: 半帧; >0: 成功
	*/
	int Check645FrameFormat(string &ReceiveFrame, string &RevFrame);
	/** @brief 获取操作码
	*  @param[in] none
	*  @return 操作码
	*/
	string GetOperatorCode();
	/** @brief 获取表地址
	*  @param[in] none
	*  @return 表地址
	*/
	string GetMeterAddr();
	/** @brief 组帧报文头
	*  @param[in] strCmd 报文
	*  @return 报文头
	*/
	string MakeFrameHead(const S_DWORD& CmdDI);
	/** @brief 组帧报文DI
	*  @param[in] strCmd 报文
	*  @return 报文DI
	*/
	string MakeFrameDI(const S_DWORD& CmdDI);
	/** @brief 组帧报文数据
	*  @param[in] strCmd 报文
	*  @return 报文数据
	*/
	string MakeFrameData(const S_DWORD& CmdDI);
	/** @brief 组帧报文尾
	*  @param[out] TempFrame 完整报文
	*  @return none
	*/
	void MakeFrameEnd(string& TempFrame);
	/** @brief 判断dbcj回复报文的正确性
	*  @param[in] strCmd 报文
	*  @param[out] StrResponse 回复的报文
	*  @param[out] Flag 回复的报文的正确性
	*  @return 返回报文开始的索引
	*/
	S_DWORD JudgeDbcjResult(const S_DWORD& CmdDI,string &StrResponse,bool& Flag);
	/** @brief 解析安全认证时的错误信息
	*  @param[in] Error SEER
	*  @return 
	*/
	int AnalysisErrorCode(const S_WORD &Error);
	/** @brief dbcj返回正确内容的情况下获取对应的结果内容
	*  @param[in] Index 返回报文开始的索引
	*  @param[in] strCmd 报文DI
	*  @param[out] StrResponse 回复的报文
	*  @return none
	*/
	void AnalysisDbcjResult(S_DWORD Index,const S_DWORD& CmdDI,string &StrResponse);

private:
	// Meter
	S_CHAR				m_MeterEsamRand[8];								// 8字节模块easm随机数  由电表返回
	S_CHAR				m_MeterRand[4];									// 4字节随机数  由电表返回
	S_CHAR				m_MeterMode;									// 电表模式
	S_CHAR				m_MeterAddr[8];									// 电表地址
	S_CHAR				m_MeterMAC[4];									// MAC 由电表返回
	S_CHAR				m_MeterBackInfo[50];							// 返写信息 由电表返回

	// From Card
	S_CHAR				m_CardMeterNo[8];								// 表号
	S_CHAR				m_CardRandData[8];								// 8字节模块ESAM随机数
	S_CHAR				m_CardDstData[8];								// 8字节密文
	S_CHAR				m_CardMAC1[4];
	S_CHAR				m_CardCustomerNo[6];							// 6字节bcd,客户编号
	S_CHAR				m_CardMAC2[4];
};

#endif//__METER_FRAMEMGR_H__


