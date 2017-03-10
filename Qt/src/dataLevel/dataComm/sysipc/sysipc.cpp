/** @file
 *  @brief IPC用户类
 *  @author  Steven Wan
 *  @date 2015/11/07
 *  @version 0.1
 *  1、各进程间的消息通信
 *  2、共享内存分为系统上下文和系统状态，
 *  系统上下文是建立在内存文件上，系统状态是建立在FLASH文件上。
 */
#include "sysipc.h"


/** @brief 消息通道全局变量定义 */
C_MESSAGE C_SysMsg::m_MsgObj('m');
S_BYTE C_SysMsg::m_ReqSeq = 0;
S_BYTE C_SysMsg::m_ResSeq = 0;
string C_SysMsg::m_msg="";
string C_SysMsg::MSG="";

//
//配置多进程
//
/** @brief 进程数组
 *  @note 增加进程的方法
 *  1、在cfg_proc.h中定义进程名宏，同时修改进程总数宏MP_PROCESS_CNT
 *  2、将进程名宏加入到gMP_ProcessAll数组中，多进程框架将自动生成指定的进程。
 *  3、继承C_SERVER类，编写进程要实现的具体功能。参见server.h。
 *  4、继承C_AppCmd类，将C_SERVER子类在GetServer()方法中实例化。参见appcmd.h。
 *  @remarks 进程名宏不能在本文件中定义。
 *  @see server.h appcmd.h cfg_proc.h
 */
S_PROCESS gMP_ProcessAll[] =
{
	//进程名，进程功能说明
	{MP_PROCESS_DAEMON, "系统守护进程，负责系统总控"},
//用户进程加在后面
	{MP_PROCESS_LCD,   "lcd进程，负责本地操作与显示"},
	{MP_PROCESS_HTTX1, "httx1进程，负责主站通讯(客户端)"},
	{MP_PROCESS_HTTX2, "httx2进程，调试软件通讯(服务器)"},
	{MP_PROCESS_HTTX3, "httx3进程，红外口通讯"},
	{MP_PROCESS_HTTX4, "httx4进程，串口通讯"},
	{MP_PROCESS_DBCJ1,  "dbcj1进程，负责能量采集及数据冻结"},
	{MP_PROCESS_DBCJ2,  "dbcj2进程，负责能量采集及数据冻结"},
	{MP_PROCESS_DBCJ3,  "dbcj3进程，负责能量采集及数据冻结"},
	{MP_PROCESS_DBCJ4,  "dbcj4进程，负责能量采集及数据冻结"},
	{MP_PROCESS_PPP,   "ppp进程，负责与主站建立拨号连接"},
	{MP_PROCESS_JLCY, "jlcy进程，负责对交采芯片数据采集"},
	{MP_PROCESS_FHKZ, "fhkz进程，负责告警管理和负控数据处理"},
	{MP_PROCESS_CTCY, "ctcy进程，负责CT采样和电池管理"},
	{MP_PROCESS_RFTX,   "RFTX进程，负责终端与射频模块通信"},//wjp 20160729

//进程定义结束
	{NULL, NULL}
};

/** @brief 获取进程序号
 *  @param[in] pName 进程名称
 *  @return 0~(MP_PROCESS_COUNT-1) 查找成功
 *  @return -1 查找失败
 *  @remarks 本函数不需要重写
 */
int MP_GetProcessSN(const char *pName)
{
	for (int i = 0; gMP_ProcessAll[i].m_pName != NULL; i++)
	{
		if (strcmp(gMP_ProcessAll[i].m_pName, pName) == 0)
			return i;
	}
	return -1;
}

/** @brief 根据服务名取服务ID */
int GetServerID(const char *ServerName)
{
	return MP_GetProcessSN(ServerName);
}
/** @brief 获取进程名称 
 *	@param[in] PSN 进程的有效序号(0~(MP_PROCESS_COUNT-1))
 *	@return NULL PSN序号无效
 *  @return !NULL 进程名指针
 *  @remarks 本函数不需要重写,用户必需保证PSN序号是有效的
 */
const char* MP_GetProcessName(int PSN)
{
	return gMP_ProcessAll[PSN].m_pName;
}
/** @brief 根据服务ID取服务名 */
const char *GetServerName(int ServerID)
{
	return MP_GetProcessName(ServerID);
}

#define MP_PROCESS_QT_ID		32
int GetCurrentServID(void)
{
	return MP_PROCESS_QT_ID;
}

/** @brief 发送消息给服务
 *  @param[in] SerID 服务ID（0 ~ 服务总数-1）
 *  @param[in] MsgT 消息类型
 *  @param[in] msg 消息
 *  @return -1 发送出错；0 发送成功
 */
int C_SysMsg::Send2Server(int SerID, S_BYTE MsgT, const string &msg)
{
	int SelfID = GetCurrentServID();
	string MSG;

	m_ReqSeq += 1;

	MSG.push_back(MsgT);
	MSG.push_back(m_ReqSeq);
	MSG.append((S_CHAR*)&SelfID, sizeof(SelfID));
	MSG.append(msg);

//	logit<<"Send "<<GetServerName(SerID)<<" msg:"<<HEX<<msg<<"."<<endl;

	return m_MsgObj.SendMsg(SerID+MTYPE_BASE_VAL, MSG);
}

/** @brief 发送消息给服务
 *  @param[in] pServName 服务名称
 *  @param[in] MsgT 消息类型
 *  @param[in] msg 消息
 *  @return -1 发送出错；0 发送成功
 */
int C_SysMsg::Send2Server(const char *pServName, S_BYTE MsgT, const string &msg)
{
	return Send2Server(GetServerID(pServName), MsgT, msg);
}

/** @brief 接收服务消息
 *  @param[in] MsgT 消息类型
 *  @param[out] msg 消息输出缓冲
 *  @return -1 接收失败；0 接收成功
 */
int C_SysMsg::Recv4Server(S_BYTE MsgT, string &msg)
{
	//string MSG;
	MSG.clear();
	
	if (m_MsgObj.RecvMsg(GetCurrentServID()+MTYPE_BASE_VAL, MSG) > 6)
	{
		printf("recv msg > 6\n");
		if ((S_BYTE)MSG[0] != MsgT)
		{
			m_MsgObj.PutMsg(GetCurrentServID()+MTYPE_BASE_VAL, MSG);
//			loget<<"Recv other msg:"<<HEX<<MSG<<"!"<<endl;
			return -1;
		}
		else if ((S_BYTE)MSG[1] != m_ReqSeq)
		{
//			loget<<"Recv not expected msg:"<<HEX<<MSG<<"!"<<endl;
			return -1;
		}
		int SerID;
		copy(MSG.begin()+2, MSG.begin()+2+sizeof(SerID), &SerID);
		msg.append(MSG.begin()+2+sizeof(SerID), MSG.end());
//		logit<<"Recv "<<GetServerName(SerID)<<" msg:"<<HEX<<msg<<"."<<endl;
		return 0;
	}
	else if (!MSG.empty())
	{
		printf("Recv exception msg:\n");
//		logwt<<"Recv exception msg:"<<HEX<<MSG<<endl;
	}
	return -1;
}

/** @brief 等待服务消息
 *  @param[in] MsgT 消息类型
 *  @param[out] msg 消息输出缓冲
 *  @param[in] WaitSecond 等待超时时间，单位秒，最大SYSMSG_TIMEOUT_MAX
 *  @return -1 接收失败；0 接收成功
 */
int C_SysMsg::Wait4Server(S_BYTE MsgT, string &msg, int WaitSecond)
{
	m_msg.clear();
	if (WaitSecond < 0 || WaitSecond > SYSMSG_TIMEOUT_MAX)
		WaitSecond = SYSMSG_TIMEOUT_MAX;
	for (int i = 0; i < WaitSecond*5; ++i)
	{
		if (Recv4Server(MsgT, m_msg) == 0)
		{
			printf("Recv4Server");
			msg = m_msg;
		}
		
#ifdef WIN32
		msleep(200);
#else
		usleep(200000);
#endif
	}			
	return 0;
}

