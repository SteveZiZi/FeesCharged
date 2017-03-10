/** @file
 *  @brief 操作系统相关定义
 *  @author 
 *  @date 2011/11/07
 *  @version 0.1
 */
#ifndef __FND_OSRELATE_H__
#define __FND_OSRELATE_H__
#include "basedef.h"

#ifdef WIN32

#include <windows.h>
#include <io.h>
#include <direct.h>

#define F_OK		0
#define access	_access
#define mkdir(path, mode)	_mkdir(path)

#else

#include <unistd.h> 
#include <dirent.h>
#include <errno.h>
#include <sys/file.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h> 
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/io.h>
#include <sys/sem.h>
#include <sys/param.h>
#include <fcntl.h>
#include <signal.h>
#include <netinet/in.h>
#include <netdb.h>
#include <termios.h>
#include <assert.h>

#endif


#ifdef WIN32
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#endif

#ifdef WIN32
#define PID_T	int
#define getpid()	0xFFFF
#define socklen_t	int
#else
#define PID_T	pid_t
#endif

/** @brief 毫秒级睡眠函数 */
inline void SleepMS(int ms)
{
#ifdef WIN32
	Sleep(ms);
#else
	usleep(ms*1000);
#endif
}

/** @brief 秒级睡眠函数 */
inline void SleepSecond(int Second)
{
#ifdef WIN32
	Sleep(Second*1000);
#else
	sleep(Second);
#endif
}

/** @brief 系统命令 */
inline int SystemCmd(const char *command)
{
#ifdef WIN32
	return 0;
#else
	int ok = system(command);
	SleepMS(1);
	return ok;
#endif
}


#endif //__FND_OSRELATE_H__

