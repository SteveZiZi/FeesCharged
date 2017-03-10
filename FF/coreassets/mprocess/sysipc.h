/** @file
 *  @brief IPC用户类
 *  @author 
 *  @date 2011/11/07
 *  @version 0.1
 *  1、各进程间的消息通信
 *  2、共享内存分为系统上下文和系统状态，
 *  系统上下文是建立在内存文件上，系统状态是建立在FLASH文件上。
 */
#ifndef __MP_SYSIPC_H__
#define __MP_SYSIPC_H__
#include "message.h"
#include "sharemem.h"
#include "server.h"
#include "traceset.h"

/** @brief 共享内存标识 */
#define CONTEXT_MEM_SIGN	"context.dat"
#define STATUS_MEM_SIGN		"status.dat"

#define MSAFN_IPC_JLCY_MSG		0xA1
#define MSAFN_IPC_JLCY_ALT		0xA2

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

/** @class
 *  @brief 系统上下文共享内存基类
 *  @remarks 子类重载虚函数时注意回调基类相应的函数
 */
class C_ContextBase
{
public:
	C_ContextBase(void);
	virtual ~C_ContextBase();

public:
	/** @brief 系统上下文的准备工作
	 *  @return true 就绪；false 失败
	 */
	bool Prepare(void);

	/** @brief 设置新初始化标志(守护进程使用) */
	bool SetNewFlag(bool fNew = true){m_ContextNew = fNew;return true;}
	/** @brief 判断系统上下文是否最新(守护进程使用) */
	bool ContextNewest(void){return m_ContextNew;}
	/** @brief 设置启动所有服务标志(守护进程使用) */
	bool SetStartAllServer(bool fEnable){m_StartAllServer = true; return true;}
	/** @brief 判断所有服务是否启动(守护进程使用) */
	bool AllServerNeedStart(void){return m_StartAllServer;}

	/** @brief 设置所有服务活动
	 *  @param[in] fActive [true|false]
	 *  @return true
	 */
	bool SetAllServerActive(bool fActive);
	/** @brief 系统上下文内存标志比较
	 *  @param[in] memsign 参考标志
	 *  @return true 成功；false 失败
	 */
	bool MemsignCompare(const S_MEMSIGN &memsign);

	/** @brief 取进程使能标志
	 *  @param[in] PSN 进程序号（ID）
	 *  @return true 使能；false 未使能
	 */
	bool GetProcessEnable(int PSN);
	/** @brief 使能进程
	 *  @param[in] PSN 进程序号（ID）
	 *  @param[in] fEnable [true|false]
	 *  @return true
	 */
	bool EnableProcess(int PSN, bool fEnable);

	/** @brief 取进程信息
	 *  @param[in] PSN 进程序号（ID）
	 *  @return NULL 无效；!NULL 有效信息
	 */
	S_SERVINFO *GetServerInfo(int PSN);
	/** @brief 取进程跟踪信息配置
	 *  @param[in] PSN 进程序号（ID）
	 *  @return NULL 无效；!NULL 有效信息
	 */
	S_TraceSet *GetTraceSet(int PSN);
	/** @brief 取系统信息
	 *  @return NULL 无效；!NULL 有效信息
	 */
	S_SystemInfo *GetSystemInfo(void);

	/** @brief 取系统软时钟
	 *  @return 系统节拍值
	 */
	S_TIME GetSystemTick(void);

	/** @brief 设置重启脚本
	 *  @param[in] fScript 脚本(带路径)
	 *  @note 重启脚本在看门狗复位前会调用
	 */
	bool SetResetScript(const char *fScript);

public:
	/** @brief 设置成员信息等
	 *  @return true 成功；false 失败
	 *  @remarks 子类重载时请先回调本函数，共享内存创建成功时调用本函数
	 */
	virtual bool SetInformation(void);
	/** @brief 初始化系统上下文
	 *  @return true 成功；false 失败
	 *  @remarks 子类重载时请先回调本函数
	 */
	virtual bool InitContext(void);
	/** @brief 加载配置到系统上下文
	 *  @return true 成功；false 失败
	 *  @remarks 子类重载时请先回调本函数
	 */
	virtual bool LoadContext(void);
	/** @brief 参数分析
	 *  @return true 成功；false 失败
	 *  @remarks 子类重载时请先回调本函数
	 */
	virtual bool ParamParse(void);
	/** @brief 数据区复位时清除数据
	 *  @return true 成功；false 失败
	 *  @remarks 子类重载时请先回调本函数
	 */
	virtual bool ClearData(void);
	
protected:
	bool m_ContextNew;///< 仅仅在daemon中使用
	bool m_StartAllServer;///< 仅仅在daemon中使用
	void *m_pContext; ///< 系统上下文共享内存
	C_SHAREMEM m_ShareMem; ///< 共享内存对象
};

/** @class
 *  @brief 系统状态共享内存基类
 *  @remarks 子类重载虚函数时注意回调基类相应的函数
 */
class C_StatusBase
{
public:
	C_StatusBase(void);
	virtual ~C_StatusBase();

public:
	/** @brief 系统状态的准备工作
	 *  @return true 就绪；false 失败
	 */
	bool Prepare(void);
	/** @brief 设置新初始化标志(守护进程使用) */
	bool SetNewFlag(bool fNew = true){m_StatusNew = fNew; return true;}
	/** @brief 判断系统状态是否最新(守护进程使用) */
	bool StatusNewest(void){return m_StatusNew;}

	/** @brief 保存系统状态
	 *  @return true 成功；false 失败
	 */
	bool SaveStatus(void){return m_ShareMem.Save();}
	/** @brief 系统上下文内存标志比较
	 *  @param[in] memsign 参考标志
	 *  @return true 成功；false 失败
	 */
	bool MemsignCompare(const S_MEMSIGN &memsign);

public:
	/** @brief 设置成员信息等
	 *  @return true 成功；false 失败
	 *  @remarks 子类重载时请先回调本函数，共享内存创建成功时调用本函数
	 */
	virtual bool SetInformation(void);
	/** @brief 初始化系统状态
	 *  @return true 成功；false 失败
	 *  @remarks 子类重载时请先回调本函数
	 */
	virtual bool InitStatus(void);
	/** @brief 加载配置到系统状态
	 *  @return true 成功；false 失败
	 *  @remarks 子类重载时请先回调本函数
	 */
	virtual bool LoadStatus(void);
	/** @brief 参数分析
	 *  @return true 成功；false 失败
	 *  @remarks 子类重载时请先回调本函数
	 */
	virtual bool ParamParse(void);
	/** @brief 数据区复位时清除数据
	 *  @return true 成功；false 失败
	 *  @remarks 子类重载时请先回调本函数
	 */
	virtual bool ClearData(void);

protected:
	bool m_StatusNew;///< 仅仅在daemon中使用
	void *m_pStatus; ///< 系统状态共享内存
	C_SHAREMEM m_ShareMem; ///< 共享内存对象
};


#endif //__MPROCESS_SYSIPC_H__
