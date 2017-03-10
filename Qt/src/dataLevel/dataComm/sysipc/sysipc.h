/** @file
 *  @brief IPC用户类
 *  @author  Steven Wan
 *  @date 2015/11/07
 *  @version 0.1
 *  1、各进程间的消息通信
 *  2、共享内存分为系统上下文和系统状态，
 *  系统上下文是建立在内存文件上，系统状态是建立在FLASH文件上。
 */
#ifndef __MP_SYSIPC_H__
#define __MP_SYSIPC_H__
#include "message.h"
#include "../inc/basedef.h"
//#include "traceset.h"


#define MSAFN_IPC_JLCY_MSG		0xA1
#define MSAFN_IPC_JLCY_ALT		0xA2

/** @brief 进程服务名称定义 */
/** @berif 配置守护进程的进程名 */
#define MP_PROCESS_DAEMON	"daemon"
#define MP_PROCESS_HTTX1	"httx1"
#define MP_PROCESS_HTTX2	"httx2"
#define MP_PROCESS_HTTX3	"httx3"
#define MP_PROCESS_HTTX4	"httx4"
#define MP_PROCESS_DBCJ1		"dbcj1"
#define MP_PROCESS_DBCJ2		"dbcj2"
#define MP_PROCESS_DBCJ3		"dbcj3"
#define MP_PROCESS_DBCJ4		"dbcj4"
#define MP_PROCESS_PPP		"ppp"
#define MP_PROCESS_LCD		"lcd"
#define MP_PROCESS_JLCY		"jlcy"
#define MP_PROCESS_FHKZ		"fhkz"
#define MP_PROCESS_CTCY		"ctcy"

#define MP_PROCESS_RFTX              "rftx"   //wjp 20160729
/** @brief 进程总数宏
 *  @remarks 增减进程后一定要修改本宏的定义，
 *  它的值必须与gMP_ProcessAll数组中进程个数相等，否则将出错。PLEASE BE CAREFUL.
 */
#define MP_PROCESS_CNT	(1/*daemon*/+4/*httx */+4/*dbcj */+1/*ppp*/+1/*lcd*/+1/*jlcy*/+1/*fhkz*/+1/*ctcy*/+1/*rftx*/)
enum 
{
	MSAFN_CONFIRM_DENY = 0x00,//确认/否认
	MSAFN_LINK_CHECK = 0x02,
	MSAFN_WRITE_PARAM = 0x04,
	MSAFN_AUTHENTICATE = 0x06,
	MSAFN_READ_PARAM = 0x0A,
	MSAFN_READ_REALTIME = 0x0C,
	MSAFN_READ_HISTORY = 0x0D,
	MSAFN_READ_EVENT = 0x0E,
	MSAFN_FILE_TRANSFER = 0x0F,
	MSAFN_DATA_ROUTE = 0x10,
	MSAFN_READ_TASK = 0x12,
	MSAFN_READ_ALARM = 0x13,
	MSAFN_FORWARD = 0x14,
	MSAFN_USER_DEFINE = 0x15,

//级联主终端命令
	MSAFN_FORWARD_MS = 0x2B,

//user define
	MSAFN_USER_DEFINE_BEGIN = 0x80
};

struct S_PROCESS
{
	const char *m_pName;///<进程名
	const char *m_pInfo;///<进程帮助信息
};


/* @消息通信格式[以下是以南网规范描述，国网则DI(4B)改为FN(2B)]
一基本格式：消息类型 + 消息内容
  消息类型=MSAFN_ (enum宏)
  消息内容=DA(2B) + DI(4B) [+ CONTEXT]
  注：在回复时采集失败的数据项填充无效值，或者清除本DA和DI。
  如果全部DA和DI都被清除了，则回复第一个DA和DI
二例子:实招表3正向有功总和反向有功总
  消息类型=MSAFN_READ_REALTIME
  下行消息内容: 04 01 00 00 01 00 04 01 00 00 02 00
  上行消息内容: 04 01 00 00 01 00 12 34 56 78 04 01 00 00 02 00 90 78 56 34
  上行异常1: 04 01 00 00 01 00 
  上行异常2: 04 01 00 00 01 00 FF FF FF FF 04 01 00 00 02 00 90 78 56 34
  上行异常3: 04 01 00 00 01 00 12 34 56 78 04 01 00 00 02 00 FF FF FF FF
  上行异常4: 04 01 00 00 02 00 90 78 56 34
  上行异常5: 04 01 00 00 01 00 12 34 56 78
*/

/** @brief 参与消息通信进程层次划分
高层：httx lcd
中层：fhkz abcj ppp
低层：jlcy

高层可以向低层请求消息，低层不可以向高层请求消息
同层尽量避免产生消息请求需求
消息类型尽量分细点，按进程和功能区分，不要相同
*/

/** @class
 *  @brief 系统消息通道
 *  @remarks 使用时注意区分发送进程和接收进程
 */
class C_SysMsg
{
public:
	enum{MTYPE_BASE_VAL=100, SYSMSG_TIMEOUT_MAX=60};

	/** @brief 发送消息给服务
	 *  @param[in] SerID 服务ID（0 ~ 服务总数-1）
	 *  @param[in] MsgT 消息类型
	 *  @param[in] msg 消息
	 *  @return -1 发送出错；0 发送成功
	 */
	static int Send2Server(int SerID, S_BYTE MsgT, const string &msg);
	/** @brief 发送消息给服务
	 *  @param[in] pServName 服务名称
	 *  @param[in] MsgT 消息类型
	 *  @param[in] msg 消息
	 *  @return -1 发送出错；0 发送成功
	 */
	static int Send2Server(const char *pServName, S_BYTE MsgT, const string &msg);
	/** @brief 接收服务消息
	 *  @param[in] MsgT 消息类型
	 *  @param[out] msg 消息输出缓冲
	 *  @return -1 接收失败；0 接收成功
	 */
	static int Recv4Server(S_BYTE MsgT, string &msg);
	/** @brief 等待服务消息
	 *  @param[in] MsgT 消息类型
	 *  @param[out] msg 消息输出缓冲
	 *  @param[in] WaitSecond 等待超时时间，单位秒，最大SYSMSG_TIMEOUT_MAX
	 *  @return -1 接收失败；0 接收成功
	 */
	static int Wait4Server(S_BYTE MsgT, string &msg, int WaitSecond = SYSMSG_TIMEOUT_MAX);

	/** @brief 查询服务消息
	 *  @param[out] SerID 服务ID
	 *  @param[out] MsgT 消息类型
	 *  @param[out] msg 消息缓冲
	 *  @return -1 查询失败；0 查询成功
	 */
	static int Query4Server(int &SerID, S_BYTE &MsgT, string &msg);
	/** @brief 回复消息给服务
	 *  @param[in] SerID 服务ID
	 *  @param[in] MsgT 消息类型
	 *  @param[in] msg 消息缓冲
	 *  @return -1 回复出错；0 回复成功
	 */
	static int Reply2Server(int SerID, S_BYTE MsgT, const string &msg);

private:
	static C_MESSAGE m_MsgObj; ///< 消息对象
	static S_BYTE m_ReqSeq; ///< 消息发送顺序号
	static S_BYTE m_ResSeq; ///< 消息响应顺序号
	static string m_msg;
	static string MSG;
};
#endif


