/** @file
 *  @brief IPC-消息底层类
 *  @author  Steven Wan
 *  @date 2015/11/07
 *  @version 0.1
 *  1、底层消息通讯类，支持LINUX系统
 *  2、用户需要配置ftok的fname
 */
#ifndef __MP_MESSAGE_H__
#define __MP_MESSAGE_H__
#include "../inc/basedef.h"
#include "strings.h"

/** @class
 *  @brief IPC消息通道
 *  @remarks 采用LINUX msgsnd msgrcv等函数
 */
class C_MESSAGE
{
public:
	enum{TEXT_BUF_MAX=2048};

	C_MESSAGE(char QFlag = 'M');
	~C_MESSAGE();

public:
	/** @brief 发送消息
	 *  @param[in] MsgT 消息类型
	 *  @param[in] msg 消息
	 *  @return <0 失败；>=0 成功
	 */
	int SendMsg(int MsgT, const string &msg);
	/** @brief 接收消息
	 *  @param[in] MsgT 消息类型
	 *  @param[out] msg 消息输出缓冲
	 *  @return <0 失败；>=0 成功
	 */
	int RecvMsg(int MsgT, string &msg);
	/** @brief 清理消息通道
	 *  @param[in] MsgT 消息类型
	 *  @return <0 失败；>=0 成功
	 *  @remarks MsgT>0 清理指定消息；MsgT=0 清理所有消息
	 */
	int ClearMsg(int MsgT);
	/** @brief 缓存消息
	 *  @param[in] MsgT 消息类型
	 *  @return <0 失败；>=0 成功
	 */
	int PutMsg(int MsgT, const string &msg);

	/** @brief 提取消息
	 *  @param[in] MsgT 消息类型
	 *  @param[out] msg 消息输出缓冲
	 *  @return <0 失败；>=0 成功
	 */
	int GetMsg(int MsgT, string &msg);
	
private:
	strings m_MsgBuffer; ///< 消息缓冲
	int m_MsgQID; ///< 消息通道描述符
};

#endif //__MPROCESS_MESSAGE_H__


