/** @file
 *  @brief 多进程框架用户配置文件
 *  @author 
 *  @date 2011/10/15
 *  @version 0.1
 *  1、配置日志系统
 *  2、配置多进程的进程名
 *  3、配置IPC
 */
#ifndef __MP_CFG_MP_H__
#define __MP_CFG_MP_H__
#include "sysipc.h"

//
//配置多进程
//
/** @berif 配置守护进程的进程名 */
#define MP_PROCESS_DAEMON	"daemon"

struct S_PROCESS
{
	const char *m_pName;///<进程名
	const char *m_pInfo;///<进程帮助信息
};

/** @brief 获取进程序号 */
int MP_GetProcessSN(const char *pName);
/** @brief 获取进程名称 */
const char* MP_GetProcessName(int PSN);
/** @brief 获取进程帮助信息 */
const char* MP_GetProcessInfo(int PSN);
/** @brief 获取进程总数 */
S_WORD MP_GetProcessCnt(void);

/** @berif 配置IPC */
class C_ShmFact
{
public:
	/** @breif 获取系统上下文共享内存 */
	static C_ContextBase *GetContextBase(void);
	/** @breif 获取系统状态共享内存 */
	static C_StatusBase  *GetStatusBase(void);
};

/** @breif 多进程框架相关函数 */
char *MP_GetSoftwareVersion(void);
char *MP_GetSoftwareReleaseDate(void);
char *MP_GetHardwareVersion(void);
char *MP_GetHardwareReleaseDate(void);


#endif //__MPROCESS_CFG_MP_H__

