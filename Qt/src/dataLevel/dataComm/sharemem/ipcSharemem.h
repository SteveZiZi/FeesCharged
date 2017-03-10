/** @file
 *  @brief QT�������̨��ؽ��̹����ڴ�ͨѶ�ӿ���
 *  @author Steven Wan
 *  @date 2015-08/30
 *  @version 0.1
*/
 
#ifndef _IPC_SHAREMEM_H_
#define _IPC_SHAREMEM_H_


#include "FileBase.h"
#include "ShareMem.h"
#include "../inc/typedef.h"

#ifdef WIN32
#define MNT_PATH_BASE	"./mnt"
#else
#define MNT_PATH_BASE	"/mnt"
#endif


///#define FILE_IPC_SHAREMEM MNT_PATH_BASE"/ramdisk/dyjc/ipcsharemem.dat"	//DYJCϵͳ���������ڴ湲���ļ�
#define FILE_IPC_SHAREMEM "/tmp/ipcsharemem.dat"	//share memery file




/* 
brief:�����ڴ�
*/
struct SIpcShareMem
{
public:
	struct HeaderChecker Checker;
	char m_cRTUA[4];	//�ն��߼���ַ
	struct SIpcDeviceRegData m_IpcDevicesRegData[CONS_DEVICE_MAX_COUNT];//�Ĵ�������
	int m_CurCardMp;//��ǰ���������
	struct SIpcCardInfo m_SIpcCardInfo[CONS_DEVICE_MAX_COUNT];
public://�ն˲���
	struct SIpcTerminalInfo m_IpcTerminalInfo;
public:
	SIpcShareMem();
	static SIpcShareMem& Instance(void);
	bool Update(void){return m_IpcShareMem.Save(this,sizeof(*this));}
	void InitDevicesRegData(){memset(&m_IpcDevicesRegData[0],0x00,sizeof(m_IpcDevicesRegData));}

public://card ctrl
	bool SetCardType(T_emCARDTYPE cardtype);
	T_emCARDTYPE GetCardType();
	bool SetCardState(T_emCARDSTATE cardstate);
	T_emCARDSTATE GetCardState();
	bool SetCardSerialNo(S_BYTE serialno[8]);
	bool GetCardSerialNo(S_BYTE *pSerialnobuf, S_BYTE buflen);
	bool SetMeterNo(S_BYTE meterno[8]);
	bool GetMeterNo(S_BYTE *pmeternobuf, S_BYTE buflen);
	int  SetCurCardMp(S_BYTE meterno[8]);
	int  GetCurCardMp(){return m_CurCardMp;};

private:
	static bool InitFile(FILE* fp, void* ipOther);
	static bool CheckFile(FILE* fp, void* ipOther);
	int Save(bool ibForce);
	
private:
	static ShareMem m_IpcShareMem;	
};

#define IPCSHAREMEM SIpcShareMem::Instance()
#define DECLARE_IPCSHAREMEM(ipcsharemem) SIpcShareMem& ipcsharemem=IPCSHAREMEM

#endif

