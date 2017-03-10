/** @file
 *  @brief 用户层系统上下文配置类
 *  @author 
 *  @date 2011/12/06
 *  @version 0.1
 *  @see mprocess/sharemem.h mrpocess/sysipc.h
 *  注意需要继承C_ContextBase类，注意其中的抽象方法的重载。
 */
#ifndef __NW_CONTEXT_H__
#define __NW_CONTEXT_H__
#include "cfg_cont.h"
#include "sysipc.h"

enum
{
	LCD_INDICATOR_PPP_MODEM_RESET, //	"MODEM正在上电...",
	LCD_INDICATOR_PPP_MODEM_CHECK, //	"检查MODEM...    ",
	LCD_INDICATOR_PPP_MODEM_SIGNAL, //	"检查无线信号... "
	LCD_INDICATOR_PPP_DIALING, //	"PPP正在拨号...  ",
	LCD_INDICATOR_PPP_CONNECTED, //	"PPP连接成功...  ",
	LCD_INDICATOR_PPP_FAILED, //	"PPP连接失败     "

	LCD_INDICATOR_PPP_MAX
};
//enum
//{
//	LCD_INDICATOR_DBCJ_GATHER_FINISH, //	"抄表结束        ",
//	LCD_INDICATOR_DBCJ_GATHER, //	"正在抄表中...   ",
//	LCD_INDICATOR_DBCJ_ROUTER, //	"正在路由中...   ",
//	LCD_INDICATOR_DBCJ_REPORT, //	"正在表号上报中..",
//	LCD_INDICATOR_DBCJ_REALCMD, //	"响应实时命令... ",
//	LCD_INDICATOR_DBCJ_RECOGNISE_FB, //	"正在识别下行模块",
//	LCD_INDICATOR_DBCJ_FB_UNKNOW, //	"未知的下行模块  ",
//	LCD_INDICATOR_DBCJ_FB_OK, //	"下行模块识别成功"

//	LCD_INDICATOR_DBCJ_MAX
//};

enum
{
	NOTIFY_DBCJ_ADD_METER, // 
	NOTIFY_DBCJ_ADD_TASK,
	NOTIFY_DBCJ_ADD_ALARM,

	NOTIFY_DBCJ_ADD_MAX
};

class C_CONTEXT:public C_ContextBase
{
public:
	virtual ~C_CONTEXT(){m_pMyContext = NULL;}
	static C_CONTEXT &Instance(void);

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

public:
	bool CheckProgramkeyState(bool fRefresh);
	void SetReconnectFlag(bool fConnect){m_pMyContext->m_ConnectStatus.m_Reconnect = fConnect;}
	bool GetReconnectFlag(void){return m_pMyContext->m_ConnectStatus.m_Reconnect;}

	S_ConnectStatus *GetConnectStatus(void){return &m_pMyContext->m_ConnectStatus;}
	S_PowerStatus *GetPowerStatus(void){return &m_pMyContext->m_PowerStatus;}
	S_HttxStatus *GetHttxStatus(S_HttxSN httxSN){return &m_pMyContext->m_HttxStatus[httxSN];}
	S_FlowInfo *GetFlowInfo(void){return &m_pMyContext->m_FlowInfo;} ///< 流量控制信息
	S_PppStatus *GetPppStatus(void){return &m_pMyContext->m_PppStatus;}
	S_DbcjStatus *GetDbcjStatus(S_DbcjSN DbcjSN){return &m_pMyContext->m_DbcjStatus[DbcjSN];}
	S_JlcyStatus *GetJlcyStatus(void){return &m_pMyContext->m_JlcyStatus;}
	S_FhkzStatus *GetFhkzStatus(void){return &m_pMyContext->m_FhkzStatus;}
	S_LcdStatus *GetLcdStatus(void){return &m_pMyContext->m_LcdStatus;}
	S_AlmDatas *GetAlarmDatas(void){return &m_pMyContext->m_AlmDatas;}
	S_AlmFlags *GetAlmFlags(void){return &m_pMyContext->m_AlmFlags;}
	S_ForwardData *GetForwardTaskData(int ForwardTaskID){return NULL;}//&m_pMyContext->m_ForwardTaskDatas.m_ForwardData[ForwardTaskID];}
	/** @brief 获取地区扩展信息存储区 */
	void *GetZoneExtendInfo(size_t size);

	/* return -1 表示查找失败, 否则返回序号*/
	int GetDbcjPrecess_MP(S_WORD MP);
	int GetDbcjPrecess_VCom(S_BYTE VCom);
	const char *GetDbcjPrecessName_MP(S_WORD MP);
	const char *GetDbcjPrecessName_VCom(S_BYTE VCom);

public:
	bool NotifyDbcj(int Msg);

	/** @brief 设置LCD指示器
	 *  @param[in] pProcessName 进程名
	 *  @param[in] Indicator 参见LCD_INDICATOR_
	 */
	bool SetLcdBottomInfoIndicator(const char *pProcessName, S_BYTE Indicator);
	const char *GetLcdBottomInfo(const char *pProcessName);
	
protected:
	C_CONTEXT(void){m_pMyContext = NULL;}

protected:
	S_CONTEXT *m_pMyContext;
};

#define CONTEXT C_CONTEXT::Instance()

#endif//__NW_CONTEXT_H__

