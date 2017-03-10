///*FuncDef.h中包括常用的函数的定义，用于函数名称的统一转换*/

#ifndef INCLUDED_DEFINE
#define INCLUDED_DEFINE

/*
//一、系统硬件架构开关有：
1、ARCH_PPC		//PowerPC
2、ARCH_ARM		//Arm7,Arm9
3、ARCH_ARM7	//Arm7
4、ARCH_ARM9	//Arm
5、WIN32		//Windows


//二、系统系统应用开关有：
1、APPLY_EAC5000B	//用于EAC5000B型装置
2、APPLY_EAC5000C	//用于EAC5000C型装置
3、APPLY_EAC5000D	//用于EAC5000D型装置
4、APPLY_JC			//用于集抄装置
5、DEV_5100C		//用于广东的低压用户集抄装置
*/

#ifdef WIN32
#define strdup _strdup
#define unlink _unlink
#define sleep(seconds) Sleep(seconds*1000)
#define msleep(milliseconds) Sleep(milliseconds)
#define getpid GetCurrentProcessId
#define getppid() 0xFFFF
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#define access _access
#define fileno _fileno
#define chdir _chdir
#define getcwd _getcwd
#define strlower _strlwr
#define mkdir(path,mode) _mkdir(path)
#else
#define msleep(milliseconds) usleep(milliseconds*1000)
#endif

#endif
