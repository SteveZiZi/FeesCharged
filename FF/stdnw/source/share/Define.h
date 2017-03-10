///*FuncDef.h�а������õĺ����Ķ��壬���ں������Ƶ�ͳһת��*/

#ifndef INCLUDED_DEFINE
#define INCLUDED_DEFINE

/*
//һ��ϵͳӲ���ܹ������У�
1��ARCH_PPC		//PowerPC
2��ARCH_ARM		//Arm7,Arm9
3��ARCH_ARM7	//Arm7
4��ARCH_ARM9	//Arm
5��WIN32		//Windows


//����ϵͳϵͳӦ�ÿ����У�
1��APPLY_EAC5000B	//����EAC5000B��װ��
2��APPLY_EAC5000C	//����EAC5000C��װ��
3��APPLY_EAC5000D	//����EAC5000D��װ��
4��APPLY_JC			//���ڼ���װ��
5��DEV_5100C		//���ڹ㶫�ĵ�ѹ�û�����װ��
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
