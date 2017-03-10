/** @file
 *  @brief RFID通讯类
 *  @author 
 *  @date 2016/12/06
 *  @version 0.1
 *  @note RFID通讯相关定义和实现
 */
#ifndef __RFID_COM_H__
#define __RFID_COM_H__

#include <string>
#include <vector>
#include "sysbase.h"


/** @class
 *  @brief RFID通信
 */
class C_RFIDCom
{
public:
	C_RFIDCom(void);
	virtual ~C_RFIDCom();

public:
	/** @brief 初始化RFID通道参数 
	 *  @return none
	 */
	void InitComParam(void);
	/** @brief 初始化RFID通道 
	 *  @return -1:失败 0 成功
	 */
	int InitRFIDComm(void);
	/** @brief 读取命令
	*  @return -1 出错；0 读取数据成功
	*/
	int Read(S_WORD CmdID,string &SstrData,string &RstrData);
	/** @brief 断开连接 
	*  @return true 成功；false 失败
	*/
	S_BOOL Disconnect(void);
	
private:
	/** @brief 建立连接 
	 *  @return true 成功；false 失败
	 */
	S_BOOL OnConnect(void);
	/** @brief 断开连接 
	*  @return true 成功；false 失败
	*/
	S_BOOL OnDisconnect(void);
	/** @brief 清除输入缓冲 
	*  @return true 成功；false 失败
	*/
	S_BOOL ClearInBuffer(void);
	/** @brief 清除输出缓冲 
	*  @return true 成功；false 失败
	*/
	S_BOOL ClearOutBuffer(void);
	/** @brief 判断连接状态 
	*  @return true 连接；false 未连接
	*/
	S_BOOL ConnectSuccessful(void);
	/** @brief 设置串口 
	*  @return true 成功；false 失败
	*/
	S_BOOL SetCom(void);
	/** @brief 初始化串口 
	*  @return -1 出错；>=0 有效串口描述符
	*/
	int InitCom(void);
	/** @brief 读取数据 
	*  @param[out] pData 输出缓冲
	*  @param[in] size 输出缓冲大小
	*  @return -1 出错；>=0 读取的字节数
	*/
	int ReadData(S_BYTE *pData, int size);								
	/** @brief 写入数据 读数据(底层)
	*  @param[in] pData 输入缓冲
	*  @param[in] size 输入缓冲大小
	*  @return -1 出错；>=0 写入的字节数
	*/
	int WriteData(const S_BYTE *pData, int size);						
	/** @brief 获取波特率属性 写数据(底层)
	*  @param[in] Baud 波特率数值
	*  @return B115200,B57600,B38400, B19200, B9600, B4800, B2400, B1200,B600, B300
	*/
	int GetBaudAttr(int Baud);
	int ErrorOut(S_BOOL fWarnLevel, const string &strErrorInfo);

	/** @brief 发送数据 
	*  @return -1 出错；>=0 发送的数据的长度
	*/
	int SendData(string &rfSendData);									
	/** @brief 接收数据 
	*  @return -1 出错；>=0 接收的数据的长度
	*/
	int ReceiveData(string &rfReceiveData);	
	/** @brief 发送接收 
	*  @return -1 出错；>=0 发送接收的数据的长度
	*/
	int RequestData(string &rfSendData, string &rfRevData);
	/** @brief 组帧
	*  @return -1 出错；0 组帧的长度
	*/
	int MakeReadFrame(S_WORD CmdID, string &strData,  string &strFrame);
	/** @brief 提取命令帧
	*  @param[in] strBuffer 命令缓冲
	*  @param[out] strCmd 帧输出缓冲
	*  @return <0: 出错[绝对值是出错的字节数]; =0: 半帧; >0: 成功
	*/
	int ExtractCmd(string &strBuffer, string &strRecvCmd);
	/** @brief 提取命令帧
	*  @param[in] strBuffer 命令缓冲
	*  @param[out] strCmd 帧输出缓冲
	*  @return <0: 出错[绝对值是出错的字节数]; =0: 半帧; >0: 成功
	*/
	int _ExtractCmd(string &strBuffer, string &strCmd);
	/** @brief 分析读命令返回帧(内部使用)
	*  @param[in] strCmd 返回命令帧,如为空则分析缓冲
	*  @param[in] CommReg 通用寄存器
	*  @param[out] strData 数据输出缓冲
	*  @return <0: 出错[绝对值是出错的字节数]; =0: 半帧; >0: 成功
	*/
	int ParseReadFrame(string &strCmd, string &strData);
	/** @brief 设置波特率
	*  @return -1 出错；0 成功
	*/
	int CommParamSet(void);

private:
	int				m_Comfd;											// < 串口句柄
	S_COMSET		m_ComSet;	
	double			m_EnValue;
};

#endif//__RFID_COM_H__


