#ifndef INCLUDED_FILE_BASE
#define INCLUDED_FILE_BASE

#include "../inc/stdinclude.h"
#include "../inc/basedef.h"
 //#include<sys/file.h>

#ifdef WIN32
#define PATH_BASE	"./mnt"
#else
#define PATH_BASE	"/mnt"
#endif

#define CONS_PATH_FILE_MAX_LEN	255	//路径文件最大字符数
#define CONS_SYSTEM_MODEL_LOWER	"djs"	//系统型号（小写）
#define PATH_SCRIPT_SOURCE PATH_BASE"/dyjc/script"				
#define PATH_SCRIPT_USER PATH_BASE"/user/dyjc/script"			
#define PATH_SET_SOURCE PATH_BASE"/dyjc/set"						
#define PATH_SET_USER PATH_BASE"/user/dyjc/set"



class AutoFileFP
{
public:
	inline AutoFileFP(FILE* fp,bool lock,bool sync,bool initLock)
	{
		m_fp=fp;
		m_bLock=lock;
		m_bSync=sync;
		if (initLock==true)
		{
			Lock();
		}
	}


	inline ~AutoFileFP()
	{
		Release();
	}

	inline operator FILE*()
	{
		return m_fp;
	}
	inline void Release()
	{
		#ifndef WIN32
		if (m_bSync && m_fp!=NULL)
		{
			int fd=fileno(m_fp);
			fsync(fd);		
		}
		#endif
		Unlock();
		if (m_fp!=NULL)
		{
			fclose(m_fp);
			m_fp=NULL;
		}
	}
	inline void Assign(FILE* fp,bool lock,bool sync)
	{
		Release();
		m_fp=fp;
		m_bLock=lock;
		m_bSync=sync;
		Lock();
	}
private:
	inline void Lock()
	{
		#ifndef WIN32
		if (m_bLock && m_fp!=NULL)
		{
			int fd=fileno(m_fp);
			flock(fd,LOCK_EX);
		}
		#endif
	}
	inline void Unlock()
	{
		#ifndef WIN32
		if (m_bLock && m_fp!=NULL)
		{
			int fd=fileno(m_fp);
			flock(fd,LOCK_UN);
		}
		#endif
	}
private:
	FILE* m_fp;
	bool m_bLock;
	bool m_bSync;
};


class FileBase
{
public:
	static FILE *OpenFile( const char *filename,const char *mode,bool ibLock=true,bool ibSync=true,bool ibAutoMkDir=true);
	//static int OpenFile(const char *filename,bool ibSync=true);

	static int CreateFile(const char * ipFileName,long iiLen,unsigned char icDefaultChar=0xFF);
	static int FillFile(FILE* fp,long iiLen,unsigned char icDefaultChar=0xFF);
	static int CopyFile(char * i_cpSrcFile,char * i_cpDstFile);
	static int SetFileRead(char * i_cpSetFileName,char * i_cpSetName,char * o_cpSetting);
	static int MakePathFile(const char* ipDir,const char* ipFileName,string& oPathFile);
	inline static int DeleteFile(const char* ipFileName)
	{
		return unlink(ipFileName);
	}

	static int Close(FILE* fp,bool ibUnlock=true,bool ibSync=true);
	static int WriteCfgFile(string *i_sBuff,const char* cfgFile);
	static size_t WriteBinFile(FILE *fp,const unsigned char *ipData,const long &iOffset,const int SEEK_TYPE,const size_t &iDataLen);
	static size_t ReadBinFile(FILE *fp,unsigned char *ipData,const long &iOffset,const int SEEK_TYPE,const size_t &iDataLen);
	static int MakeDir(const char* pathFile);
	static long GetFileLen(const char *fileName);
	static int GetScriptFile(const char* name,const char* userCode,const char* planKey,char* oPathFile);
	static int GetSetFile(const char* name,const char* userCode,const char* planKey,char* oPathFile);
	static int GetDynamicFile(const char* sourcePath,const char* userPath,const char* name,const char* userCode,const char* planKey,char* oPathFile);
	static int GetUserFile(const char* userPath,const char* name,const char* userCode,const char* planKey,char* oPathFile);
};


#define KLOpenFile FileBase::OpenFile

#endif
