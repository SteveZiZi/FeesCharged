 /** @file
 *  @brief 共享文件接口类
 *  @author Steven Wan
 *  @date 2015-08/30
 *  @version 0.1
*/
 
#ifndef _SHAREMEM_H
#define _SHAREMEM_H

#include "../inc/stdinclude.h"

typedef bool (*FILE_INIT_PROC)(FILE* fp,void* pOther);
typedef bool (*FILE_CHECK_PROC)(FILE* fp,void* pOther);

//用来校验共享文件
struct HeaderChecker
{
	unsigned char Flag;//有效标志0x5a
	unsigned char Version;//版本
	unsigned char Reserve[2];//保留

	HeaderChecker()
	{
		Init();
	}
	void Init()
	{
		memset(this,0,sizeof(*this));
		Flag=0x5a;
		Version=1;
	}
};

class ShareMem
{
public:
	inline ShareMem()
	{
		m_pMem=NULL;
		m_pFileName=NULL;
		m_iSize=0;
		#ifdef WIN32
		m_hFile=INVALID_HANDLE_VALUE;
		m_hFileMap=NULL;
		#endif
		m_bInited=false;
	}
	inline const bool IsInited()const
	{
		return m_bInited;
	}
	void Init(const char* ipFileName,unsigned int iiSize,FILE_INIT_PROC ipIniter,FILE_CHECK_PROC ipChecker=NULL,void* ipOther=NULL);
	bool OpenShareMemFile(FILE_INIT_PROC ipIniter,FILE_CHECK_PROC ipChecker,void* ipOther);
	~ShareMem();
	void* GetMem()
	{
		return m_pMem;
	}
	bool Save(void* ipData,size_t iiLen,bool onlyInvalid=false,bool async=false);
	bool Save(bool onlyInvalid=false,bool async=false);
	const unsigned int GetSize()const
	{
		return m_iSize;
	}
private:
	bool Init(FILE_INIT_PROC ipIniter,FILE_CHECK_PROC ipChecker=NULL,void* ipOther=NULL);
	bool EnsureShareFile(FILE_INIT_PROC ipIniter,FILE_CHECK_PROC ipChecker=NULL,void* ipOther=NULL);
	bool CheckShareFile(FILE_INIT_PROC ipIniter,FILE_CHECK_PROC ipChecker,void* ipOther);
private:
	void* m_pMem;
	char* m_pFileName;
	unsigned int m_iSize;
	bool m_bInited;
#ifdef WIN32
	HANDLE m_hFile;
	HANDLE m_hFileMap;
#endif
};
#endif
