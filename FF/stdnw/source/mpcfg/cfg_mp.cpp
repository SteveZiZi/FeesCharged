/** @file
 *  @brief 多进程框架用户配置文件
 *  @author 
 *  @date 2011/10/15
 *  @version 0.1
 *  1、配置日志系统
 *  2、配置多进程的进程名
 *  3、配置IPC
 */
#include "cfg_mp.h"
#include "cfg_usrp.h"
#include "cfg_proc.h"
#include "context.h"
#include "status.h"

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
/** @brief 获取进程帮助信息
 *	@param[in] PSN 进程的有效序号(0~(MP_PROCESS_COUNT-1))
 *	@return NULL PSN序号无效
 *  @return !NULL 进程信息指针
 *  @remarks 本函数不需要重写,用户必需保证PSN序号是有效的
 */
const char* MP_GetProcessInfo(int PSN)
{
	return gMP_ProcessAll[PSN].m_pInfo;
}

/** @brief 获取进程总数 */
S_WORD MP_GetProcessCnt(void)
{
	return MP_PROCESS_CNT;
}

//
// 配置IPC 
//
/** @breif 获取系统上下文共享内存
 *  @remarks 本函数请根据实际指向的对象配置，以便多进程框架调用
 */
C_ContextBase *C_ShmFact::GetContextBase(void)
{
//	static C_ContextBase MyContextBase;
//	return &MyContextBase;
	C_CONTEXT &MyContext = C_CONTEXT::Instance();
	return &MyContext;
}

/** @breif 获取系统状态共享内存 
 *  @remarks 本函数请根据实际指向的对象配置，以便多进程框架调用
 */
C_StatusBase  *C_ShmFact::GetStatusBase(void)
{
//	static C_StatusBase MyStatusBase;
//	return &MyStatusBase;
	C_STATUS &MyStatus = C_STATUS::Instance();
	return &MyStatus;
}


//
//多进程框架相关函数
//
#include "version.h"
/** @fn
 *  @remarks 请根据实际配置，以便多进程框架调用
 *  @note 主程序版本
 */
char *MP_GetSoftwareVersion(void)
{
	return CONS_TERM_NAME CONS_MAJOR_VERSION"."CONS_MINOR_VERSION CONS_FIX_VERSION;
}
/** @fn
 *  @remarks 请根据实际配置，以便多进程框架调用
 *  @note 主程序发布日期
 */
char *MP_GetSoftwareReleaseDate(void)
{
	return CONS_SW_REAL_DATE;
}
/** @fn
 *  @remarks 请根据实际配置，以便多进程框架调用
 */
char *MP_GetHardwareVersion(void)
{
	return CONS_HARD_VERSION;
}
/** @fn
 *  @remarks 请根据实际配置，以便多进程框架调用
 */
char *MP_GetHardwareReleaseDate(void)
{
	return CONS_HW_REAL_DATE;
}

