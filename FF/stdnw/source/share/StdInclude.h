///*include.h中包括常用的头文件引用*/

#ifndef INCLUDED_STD_INCLUDE
#define INCLUDED_STD_INCLUDE

#ifndef WIN32
	#undef __STL_USE_EXCEPTIONS
	#undef __EXCEPTIONS
	#ifdef __THROW_BAD_ALLOC
	#undef __THROW_BAD_ALLOC
	#endif
	#ifndef __THROW_BAD_ALLOC
		#define __THROW_BAD_ALLOC throw bad_alloc()
	#endif

	//#define ASSERT(exp)     ((void)0)
	#define ASSERT(exp) {if (!(exp)) printf("***********Assert failed.file=%s,line=%d,exp=%s\n",__FILE__,__LINE__,#exp);}
#else
	#include "assert.h"
	#define ASSERT assert
#endif


#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <functional>
#include <numeric>
#include <iostream>
#include <memory>
#include <cstring>
using namespace std;


typedef unsigned char			INT8U;
typedef INT8U                   UCHAR;
//typedef signed char				INT8S;
typedef unsigned short			INT16U;
typedef signed short			INT16S;
typedef unsigned int			INT32U;
//typedef signed int				INT32S;
typedef unsigned long long		INT64U;
typedef signed long long		INT64S;
typedef float					FP32;
typedef double					FP64;

typedef unsigned char byte;

#ifdef WIN32
	#define PID_T signed int
	#define socklen_t int
	#include <winsock.h>
	#pragma comment(lib, "wsock32.lib")
#else
	#define PID_T pid_t
	#define SOCKET int
#endif

//时间印记，用于数据的更新
typedef struct _TIME_STAMP
{
	time_t CheckTime;//最后一次刷新时间，为0表示从未刷新
	time_t ChangeTime;//最后一次更改时间
public:
	inline int Check()
	{
		if (CheckTime!=ChangeTime)
		{
			CheckTime=ChangeTime;
			return 0;
		}
		return -1;
	}

}TIME_STAMP;

#ifndef WIN32
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
	#include <fcntl.h>
	#include <dirent.h> 
	#include <signal.h>
	#include <sys/wait.h>
	#include <sys/stat.h> 
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include "sys/ioctl.h"
	#include <sys/types.h>
	#include <sys/io.h>
	#include <fcntl.h>
	#include <termios.h>
	#include <sys/sem.h>
#else

	#include <io.h>
	#include <Windows.h>
	#include <direct.h>
	#include <sys/stat.h>
	#define EINPROGRESS WSAEINPROGRESS
#endif

#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>

#ifdef ARCH_PPC
#include "../arch/powerpc/bitset"
#else
#include <bitset>
#endif

#endif
