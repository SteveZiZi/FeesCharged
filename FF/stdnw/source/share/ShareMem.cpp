#include "ShareMem.h"

#include "Define.h"
#include "FileBase.h"

#define PAGE_SIZE	(getpagesize())
#define PAGE_MASK	(~(PAGE_SIZE-1))

void ShareMem::Init(const char* ipFileName,unsigned int iiSize,FILE_INIT_PROC ipIniter,FILE_CHECK_PROC ipChecker,void* ipOther)
{
	m_pFileName=strdup(ipFileName);
	m_iSize=iiSize;
	Init(ipIniter,ipChecker,ipOther);
	m_bInited=true;
}

bool ShareMem::Init(FILE_INIT_PROC ipIniter,FILE_CHECK_PROC ipChecker,void* ipOther)
{
	if (EnsureShareFile(ipIniter,ipChecker,ipOther)==false)
	{
		printf("ShareMem:Share file %s not exist or create failed\n",m_pFileName);
		return false;
	}
#ifndef WIN32
	int fd;
	fd=open(m_pFileName,O_RDWR,00777);
	if (fd==-1)
	{
		ASSERT(false);
		printf("ShareMem:open file error %s,file name is %s\n",strerror(errno),m_pFileName);
		return false;
	}
	m_pMem=mmap(NULL,m_iSize,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	close(fd);
	if (m_pMem==(void*)-1)
	{
//		KL_ERROR("ShareMem:ShareMem shmat error %s\n",strerror(errno));
		m_pMem=NULL;
		return false;
	}
	return true;

#else
	m_hFile=CreateFile(m_pFileName,GENERIC_WRITE | GENERIC_READ,FILE_SHARE_READ | FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (m_hFile==INVALID_HANDLE_VALUE)
	{
		printf("ShareMem:File %s could not be opened\n",m_pFileName);
		return false;
	}
	m_hFileMap=CreateFileMapping(m_hFile,NULL,PAGE_READWRITE,0,m_iSize,NULL);
	if (m_hFileMap==NULL)
	{
		printf("ShareMem:File Map for %s could not be opened\n",m_pFileName);
		CloseHandle(m_hFile);
		return false;
	}

	PVOID pvFile=MapViewOfFile(m_hFileMap,FILE_MAP_WRITE,0,0,0);
	if (pvFile==NULL)
	{
		printf("ShareMem:Cound not map view of file %s\n",m_pFileName);
		CloseHandle(m_hFileMap);
		CloseHandle(m_hFile);
		return false;
	}
	m_pMem=pvFile;
	return true;
#endif
}


bool ShareMem::Save(void* ipData,size_t iiLen,bool onlyInvalid,bool async)
{
	if (ipData<m_pMem)
	{
		ASSERT(false);
		printf("ShareMem:Save to sharefile %s para ipData error\n",m_pFileName);
		return false;
	}
#ifndef WIN32
	//2006-04-10日苏本胜更改，进行优化，只保存需要保存的地方，必须以页为单位进行保存
	//int result=msync(m_pMem,m_iSize,MS_SYNC);		//|MS_INVALIDATE);	
	unsigned long pData=((unsigned long)ipData & PAGE_MASK);
	size_t len=(unsigned long)ipData+iiLen-pData;
	int flag=MS_SYNC;
	if (async==true)
	{
		flag=MS_ASYNC;
	}
	flag |=MS_INVALIDATE;
	if (onlyInvalid==true)
	{
		flag=MS_INVALIDATE;
	}
	int result=msync((void*)pData,len,flag);
	if (result!=0)
	{
		ASSERT(false);
		printf("ShareMem:msync error %s,ipData=%p,iiLen=%d\n",strerror(errno),ipData,iiLen);
		return false;
	}
	if (onlyInvalid==false)
	{
		//KL_VERBOSE("save sharemem %p succeed(data=%p,len=%d,totalsize=%d,truedata=%p,truelen=%d,pagesize=%d)\n",m_pMem,ipData,iiLen,m_iSize,pData,len,PAGE_SIZE);
	}
	return true;
#else
	FILE* fp=FileBase::OpenFile(m_pFileName,"rb+");
	if (fp==NULL)
	{
		return false;
	}
	fseek(fp,(long)((unsigned char*)ipData-(unsigned char*)m_pMem),SEEK_SET);
	bool bResult=(fwrite(ipData,iiLen,1,fp)==1);
	fclose(fp);
	return bResult;
#endif
}

bool ShareMem::Save(bool onlyInvalid,bool async)
{
	return Save(m_pMem,m_iSize,onlyInvalid,async);
}

bool ShareMem::EnsureShareFile(FILE_INIT_PROC ipIniter,FILE_CHECK_PROC ipChecker,void* ipOther)
{
	FILE *fp=FileBase::OpenFile(m_pFileName,"rb");
	if (fp!=NULL && ipChecker!=NULL)
	{
		bool bChecked=(*ipChecker)(fp,ipOther);
		fclose(fp);
		fp=NULL;
		if (bChecked==true)
		{
			return true;
		}
		printf("ShareMem:ShareMem File %s is not valid\n",m_pFileName);
	}
	if (fp==NULL)
	{
		printf("ShareMem:Will create new file %s for sharemem\n",m_pFileName);
		fp=FileBase::OpenFile(m_pFileName,"wb+");
	}
	if (fp==NULL)
	{
		printf("ShareMem: new file %s for sharemem create failed!\n",m_pFileName);
		return false;
	}
	bool bResult=true;
	if (ipIniter!=NULL)
	{
		bResult=(*ipIniter)(fp,ipOther);
		if (bResult==false)
		{
			printf("ShareMem:Init sharemem file %s failed\n",m_pFileName);
		}
	}
	fclose(fp);
	return bResult;
}

ShareMem::~ShareMem()
{
#ifndef WIN32
	if (m_pMem!=NULL)
	{
		munmap(m_pMem,m_iSize);
	}
#else
	if (m_pMem!=NULL)
	{
		UnmapViewOfFile(m_pMem);
	}
	if (m_hFile!=NULL)
	{
		CloseHandle(m_hFileMap);
	}
	if (m_hFile!=INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFile);
	}
#endif

	if (m_pFileName != NULL)
	 free(m_pFileName);
}

