#include "FileBase.h"
#ifndef WIN32
#include <unistd.h>
#endif 

#ifdef WIN32
#include <errno.h>
#endif

#include <errno.h>
#define FILE_FD_WARN_VAL 8

FILE *FileBase::OpenFile( const char *filename,const char *mode,bool ibLock,bool ibSync,bool ibAutoMkDir)
{
	FILE* fp=fopen(filename,mode);

	if (fp)
	{
		int fd=fileno(fp);
		#ifndef WIN32
		if(ibLock)
		{
			flock(fd,LOCK_EX);
		}
		if (ibSync)
		{
			fcntl(fd,O_FSYNC);
		}
		#endif

		if (fd>FILE_FD_WARN_VAL)
		{
			printf("fd for file %s is %d\n",filename,fd);
		}
		ASSERT(fd<64);
	}
	else if(errno==ENFILE)
	{
		printf("****达到最大打开文件数*******\n");
		ASSERT(false);
	}
	else if(errno==ENOENT && ibAutoMkDir==true)
	{
		if (MakeDir(filename)!=0)
		{
			ASSERT(false);
			printf("MakeDir for %s failed\n",filename);
			return NULL;
		}
		//printf("****创建目录*******\n");
		return OpenFile(filename,mode,ibLock,ibSync,false);
	}
	return fp;
}

int FileBase::CreateFile(const char * ipFileName,long iiLen,unsigned char icDefaultChar)
{
	//printf("ipFileName=%s\n",ipFileName);
	//printf("iiLen=%ld\n",iiLen);
	if (ipFileName==NULL || iiLen<=0) return -1;
	FILE *fp=FileBase::OpenFile(ipFileName,"wb+");
	if (fp==NULL) return -1;
	int result=FillFile(fp,iiLen,icDefaultChar);
	Close(fp);
	return result;
}

int FileBase::FillFile(FILE* fp,long iiLen,unsigned char icDefaultChar)
{
	size_t t;
	if (iiLen<=1024*3)
	{
		unsigned char* buff=(unsigned char*)malloc(iiLen);
		memset(buff,icDefaultChar,iiLen);
		t=fwrite(buff,iiLen,1,fp);
		free(buff);
	}
	else
	{
		int iBlock=2048;
		unsigned char cTemp[2048];
		long i;
		memset(cTemp,icDefaultChar,iBlock);
		for (i=iBlock;i<iiLen;i=i+iBlock)
		{
			t=fwrite(cTemp,iBlock,1,fp);
		}
		t=fwrite(cTemp,iiLen-i+iBlock,1,fp);
	}
	return (t==1)?0:-1;

}

int FileBase::CopyFile(char * i_cpSrcFile,char * i_cpDstFile)
{
	FILE *fp,*fp1;
	int flag;
	long m,n;
	char Public_Buf1[2100];
	int Len_PublicBuf1=2048;

	if (i_cpSrcFile==NULL || i_cpDstFile==NULL)
	{
		return -1;
	}
	fp=FileBase::OpenFile(i_cpSrcFile,"rb");
	if (fp==NULL) return -1;
	remove(i_cpDstFile);
	fp1=FileBase::OpenFile(i_cpDstFile,"wb+");
	if (fp1==NULL){
		fclose(fp);
		return -1;
	}
	flag=0x5a;
	while(1){
		m=(long)fread(Public_Buf1,1,Len_PublicBuf1,fp);
		if (m<=0){
			break;
		}
		n=(long)fwrite(Public_Buf1,1,m,fp1);
		if (n!=m){
			flag=0;
			break;
		}
		if (m!=Len_PublicBuf1) break;
	}
	fclose(fp);
	fclose(fp1);
	if (flag!=0x5a){
		return -1;
	}else{
		return 0;
	}
}
//============================== String Deal ===================================//
//Function:Separate a string to two parts according to the special char
//In:
//	i_cpLineStr: pointer of the source string to be cut
//	i_cCutChar:the specail char(separator)
//Out:
//	o_cpLeftStr:pointer of the left string
//	o_cpRightStr:pointer of the right string
//Return:
//	-2:failuer,the source string does't include the special char
//	0:success
int String_CutLine(char *o_cpLeftStr,char *o_cpRightStr,char *i_cpLineStr,char i_cCutChar)
{
	char * s_mid;

	s_mid=strchr(i_cpLineStr,i_cCutChar);
	if (s_mid==NULL){
		strcpy(o_cpLeftStr,"");
		strcpy(o_cpRightStr,"");
		return -2;
	}
	*s_mid=0;
	strcpy(o_cpLeftStr,i_cpLineStr);
	*s_mid=i_cCutChar;
	s_mid++;
	strcpy(o_cpRightStr,s_mid);

	return 0;
}

int FileBase::SetFileRead(char * i_cpSetFileName,char * i_cpSetName,char * o_cpSetting)
{
	FILE *fp;
	char get_buf0[520],get_name[520],get_data[520];
	int flag,len,i;
	char c='=';

	fp=FileBase::OpenFile(i_cpSetFileName,"rt");
	if (fp==NULL){
           strcpy(o_cpSetting,"");
           return -1;
        }
	flag=0;
	while (fgets(get_buf0,512,fp)!=NULL){
		len=(int)strlen(get_buf0);
		if (len<=3) continue;
		for(i=0;i<len;i++){
			if((get_buf0[i]==0x0a)||(get_buf0[i]==0x0d)){
				get_buf0[i]=0;
				break;
			}
		}
		String_CutLine(get_name,get_data,get_buf0,c);
		if (strncmp(get_name,i_cpSetName,512)==0){
			flag=0x5a;
			strcpy(o_cpSetting,get_data);
			break;
		}
	}
	Close(fp);
	if (flag==0x5a){
		return 0;
	}else{
		strcpy(o_cpSetting,"");
		return -1;
	}
}

int FileBase::MakePathFile(const char* ipDir,const char* ipFileName,string& oPathFile)
{
#ifndef WIN32
	char sepChar='/';
#else
	char sepChar='\\';
#endif
	size_t dirLen=strlen(ipDir);
	oPathFile.assign(ipDir);
	if (ipDir[dirLen-1]!=sepChar)
	{
		oPathFile.push_back(sepChar);
	}
	oPathFile.append(ipFileName);
	return 0;
}

int FileBase::Close(FILE* fp,bool ibUnlock,bool ibSync)
{
	#ifndef WIN32
	if(fp)
	{
		int fd=fileno(fp);
		if (ibSync) fsync(fd);
		if (ibUnlock)	flock(fd,LOCK_UN);
	}
	#endif
	return fclose(fp);
}

int FileBase::WriteCfgFile(string *i_sBuff,const char* cfgFile)
{
	FILE *fp =OpenFile(cfgFile, "w");
	if (fp==NULL)
	{
		return -1;
	}
	fprintf(fp,i_sBuff->c_str());
	Close(fp);
	return 0;

}

int FileBase::MakeDir(const char* pathFile)
{
	char path[CONS_PATH_FILE_MAX_LEN];
	strcpy(path,pathFile);
	int len=(int)strlen(path);
	for(int i=1;i<len;i++)//跳过path[0]字符的判定
	{
		char c=path[i];
		if (c=='\\' || c=='/')
		{
			path[i]=0;
			if (access(path,0)!=0)
			{
				if (mkdir(path,00777)!=0)
				{
					return -1;
				}
			}
			path[i]=c;
		}
	}
	return 0;
}

int FileBase::GetUserFile(const char* userPath,const char* name,const char* userCode,const char* planKey,char* oPathFile)
{
	const char* code=(userCode && userCode[0]!=0) ?userCode:"default";
	const char* key=(planKey && planKey[0]!=0) ?planKey:"default";
	sprintf(oPathFile,"%s/%s/%s/%s/%s",userPath,CONS_SYSTEM_MODEL_LOWER,code,key,name);
	return 0;
}

int FileBase::GetScriptFile(const char* name,const char* userCode,const char* planKey,char* oPathFile)
{
	return 	GetDynamicFile(PATH_SCRIPT_SOURCE,PATH_SCRIPT_USER,name,userCode,planKey,oPathFile);
}

int FileBase::GetSetFile(const char* name,const char* userCode,const char* planKey,char* oPathFile)
{
	return 	GetDynamicFile(PATH_SET_SOURCE,PATH_SET_USER,name,userCode,planKey,oPathFile);
}

int FileBase::GetDynamicFile(const char* sourcePath,const char* userPath,const char* name,const char* userCode,const char* planKey,char* oPathFile)
{
	GetUserFile(userPath,name,userCode,planKey,oPathFile);
	if (access(oPathFile,0)==0)
	{
		return 0;
	}
	const char* code=(userCode && userCode[0]!=0) ?userCode:"default";
	const char* key=(planKey && planKey[0]!=0) ?planKey:"default";
	sprintf(oPathFile,"%s/%s/%s/%s/%s",sourcePath,CONS_SYSTEM_MODEL_LOWER,code,key,name);//such as "/mnt/eac/set/eac5000d/guangdong/default/meter.ini"
	if (access(oPathFile,0)==0)
	{
		return 0;
	}
	sprintf(oPathFile,"%s/%s/%s/%s",sourcePath,CONS_SYSTEM_MODEL_LOWER,code,name);//such as "/mnt/eac/set/eac5000d/guangdong/meter.ini"
	if (access(oPathFile,0)==0)
	{
		return 0;
	}
	sprintf(oPathFile,"%s/%s/%s",sourcePath,CONS_SYSTEM_MODEL_LOWER,name);//such as "/mnt/eac/set/eac5000d/meter.ini"
	if (access(oPathFile,0)==0)
	{
		return 0;
	}
	sprintf(oPathFile,"%s/%s",sourcePath,name);//such as "/mnt/eac/set/meter.ini"
	if (access(oPathFile,0)==0)
	{
		return 0;
	}
	return -1;
}

size_t FileBase::WriteBinFile(FILE *fp,const unsigned char *ipData,const long &iOffset,const int SEEK_TYPE,const size_t &iDataLen)
{
	size_t iRet=0;
	if (fp==NULL)
	{
		return iRet;
	}
	iRet=0;
	fseek(fp,iOffset,SEEK_TYPE);
	if (iDataLen>2048)//如果要写入的数据比较大，那就分块写入，避免占过大的资源。
	{
		size_t xhcs=iDataLen/2048+1;
		for (size_t i=0;i<xhcs;i++)
		{
			iRet+=fwrite(ipData + (i * 2048),2048,1,fp);
		}
	}
	else
	{
		iRet+=fwrite(ipData,iDataLen,1,fp);
	}
	return iRet;
}

size_t FileBase::ReadBinFile(FILE *fp,unsigned char *ipData,const long &iOffset,const int SEEK_TYPE,const size_t &iDataLen)
{
	size_t iRet=0;
	if (fp==NULL)
	{
		return iRet;
	}
	fseek(fp,iOffset,SEEK_TYPE);
	iRet=fread(ipData,iDataLen,1,fp);
	return iRet;
}

long FileBase::GetFileLen(const char *fileName)
{
	FILE *fp = OpenFile(fileName,"rb");
	if (fp == NULL)
	{
		return 0;
	}
	fseek(fp,0L,SEEK_END);
	long so_size = ftell(fp);
	Close(fp);
	return so_size;
}
