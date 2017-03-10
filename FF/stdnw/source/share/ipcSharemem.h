/** @file
 *  @brief QT进程与后台监控进程共享内存通讯接口类
 *  @author Steven Wan
 *  @date 2015-08/30
 *  @version 0.1
*/
 
#ifndef _IPC_SHAREMEM_H_
#define _IPC_SHAREMEM_H_


#include "FileBase.h"
#include "ShareMem.h"
#include "typedef.h"
#include "htrace.h"

#ifdef WIN32
#define MNT_PATH_BASE	"./mnt"
#else
#define MNT_PATH_BASE	"/mnt"
#endif


///#define FILE_IPC_SHAREMEM MNT_PATH_BASE"/ramdisk/dyjc/ipcsharemem.dat"	//DYJC系统的上下文内存共享文件
#define FILE_IPC_SHAREMEM "/tmp/ipcsharemem.dat"	//share memery file


/* 
brief:共享内存
*/
struct SIpcShareMem
{
public:
	struct HeaderChecker Checker;
	char m_cRTUA[4];	//终端逻辑地址
	struct SIpcDeviceRegData m_IpcDevicesRegData[CONS_DEVICE_MAX_COUNT];//寄存器数据
	int m_CurCardMp;//当前卡测量点号
	struct SIpcCardInfo m_SIpcCardInfo[CONS_DEVICE_MAX_COUNT];
	T_emTraceType m_traceType;
	bool m_traceResult; // 交易结果
public://终端参数
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
	int  GetCurCardMp(){return m_CurCardMp;}

	T_emTraceType getTraceType() const {return m_traceType;}
	bool setTraceType(T_emTraceType traceType)
	{
		m_traceType = traceType;
		m_traceResult = false;
		return true;
	}
	bool getTraceResult() {return m_traceResult;}
	void setTraceResult(bool result) {m_traceResult = result;}
// 	bool SetEsamRandData(S_BYTE esamRandData[8])
// 	{
// 		if (m_CurCardMp < 0)
// 			return false;
// 
// 		memcpy(m_SIpcCardInfo[m_CurCardMp].m_EsamRandData,esamRandData,CONS_CARD_EASM_RAM_LEN);
// 		return true;
// 	}
// 	bool GetEsamRandData(S_BYTE *pesamRandData,S_BYTE buflen)
// 	{
// 		if (buflen != CONS_CARD_EASM_RAM_LEN)
// 			return false;
// 
// 		memcpy(pesamRandData,m_SIpcCardInfo[m_CurCardMp].m_EsamRandData,CONS_CARD_EASM_RAM_LEN);
// 		return true;
// 	}
// 	bool SetDstData(S_BYTE dstData[8])
// 	{
// 		if (m_CurCardMp < 0)
// 			return false;
// 
// 		memcpy(m_SIpcCardInfo[m_CurCardMp].m_DstData,dstData,CONS_CARD_DST_DATA_LEN);
// 		return true;
// 	}
// 	bool GetDstData(S_BYTE *pdstData,S_BYTE buflen)
// 	{
// 		if (buflen != CONS_CARD_DST_DATA_LEN)
// 			return false;
// 
// 		memcpy(pdstData,m_SIpcCardInfo[m_CurCardMp].m_DstData,CONS_CARD_DST_DATA_LEN);
// 		return true;
// 	}
	bool SetRechargeVal(S_BYTE rechargeVal[4])
	{
		if (m_CurCardMp < 0)
			return false;

		memcpy(m_SIpcCardInfo[m_CurCardMp].m_RechargeVal,rechargeVal,CONS_CARD_RECHARGE_VAL_LEN);
		return true;
	}
	bool GetRechargeVal(S_BYTE *prechargeVal,S_BYTE buflen)
	{
		if (buflen != CONS_CARD_RECHARGE_VAL_LEN)
			return false;

		memcpy(prechargeVal,m_SIpcCardInfo[m_CurCardMp].m_RechargeVal,CONS_CARD_RECHARGE_VAL_LEN);
		return true;
	}
	bool SetRechargeTimes(S_BYTE rechargeTimes[4])
	{
		if (m_CurCardMp < 0)
			return false;

		memcpy(m_SIpcCardInfo[m_CurCardMp].m_RechargeTimes,rechargeTimes,CONS_CARD_RECHARGE_VAL_LEN);
		return true;
	}
	bool GetRechargeTimes(S_BYTE *prechargeTimes,S_BYTE buflen) 
	{
		if (buflen != CONS_CARD_RECHARGE_VAL_LEN)
			return false;

		memcpy(prechargeTimes,m_SIpcCardInfo[m_CurCardMp].m_RechargeTimes,CONS_CARD_RECHARGE_VAL_LEN);
		return true;
	}
// 	bool SetMAC1(S_BYTE mac[4])
// 	{
// 		if (m_CurCardMp < 0)
// 			return false;
// 
// 		memcpy(m_SIpcCardInfo[m_CurCardMp].m_MAC1,mac,CONS_CARD_MAC_LEN);
// 		return true;
// 	}
// 	bool GetMAC1(S_BYTE *pmac,S_BYTE buflen)
// 	{
// 		if (buflen != CONS_CARD_MAC_LEN)
// 			return false;
// 
// 		memcpy(pmac,m_SIpcCardInfo[m_CurCardMp].m_MAC1,CONS_CARD_MAC_LEN);
// 		return true;
// 	}
// 	bool SetCustomerNo(S_BYTE customerNo[6])
// 	{
// 		if (m_CurCardMp < 0)
// 			return false;
// 
// 		memcpy(m_SIpcCardInfo[m_CurCardMp].m_CustomerNo,customerNo,CONS_CARD_CUSTOMERNO_LEN);
// 		return true;
// 	}
// 	bool GetCustomerNo(S_BYTE *pcustomerNo,S_BYTE buflen)
// 	{
// 		if (buflen != CONS_CARD_CUSTOMERNO_LEN)
// 			return false;
// 
// 		memcpy(pcustomerNo,m_SIpcCardInfo[m_CurCardMp].m_CustomerNo,CONS_CARD_CUSTOMERNO_LEN);
// 		return true;
// 	}
// 	bool SetMAC2(S_BYTE mac[4])
// 	{
// 		if (m_CurCardMp < 0)
// 			return false;
// 
// 		memcpy(m_SIpcCardInfo[m_CurCardMp].m_MAC2,mac,CONS_CARD_MAC_LEN);
// 		return true;
// 	}
// 	bool GetMAC2(S_BYTE *pmac,S_BYTE buflen)
// 	{
// 		if (buflen != CONS_CARD_MAC_LEN)
// 			return false;
// 
// 		memcpy(pmac,m_SIpcCardInfo[m_CurCardMp].m_MAC2,CONS_CARD_MAC_LEN);
// 		return true;
// 	}
// 	bool SetParamInfo(S_BYTE *pparamInfo,S_BYTE buflen)
// 	{
// 		if (m_CurCardMp < 0 || buflen <= 0)
// 			return false;
// 
// 		memcpy(m_SIpcCardInfo[m_CurCardMp].m_ParamInfo,pparamInfo,CONS_CARD_MAC_LEN);
// 		return true;
// 	}
// 	bool GetParamInfo(S_BYTE *pparamInfo,S_BYTE buflen)
// 	{
// 		if (buflen != m_SIpcCardInfo[m_CurCardMp].m_ParamInfoLen)
// 			return false;
// 
// 		memcpy(pparamInfo,m_SIpcCardInfo[m_CurCardMp].m_MAC2,buflen);
// 		return true;
// 	}
// 	bool SetParamInfoLen(const S_WORD paramInfoLen)
// 	{
// 		if (m_CurCardMp < 0)
// 			return false;
// 
// 		m_SIpcCardInfo[m_CurCardMp].m_ParamInfoLen = paramInfoLen;
// 		return true;
// 	}
// 	S_WORD GetParamInfoLen() const {return m_SIpcCardInfo[m_CurCardMp].m_ParamInfoLen;}
// 	bool SetMAC3(S_BYTE mac[4])
// 	{
// 		if (m_CurCardMp < 0)
// 			return false;
// 
// 		memcpy(m_SIpcCardInfo[m_CurCardMp].m_MAC3,mac,CONS_CARD_MAC_LEN);
// 		return true;
// 	}
// 	bool GetMAC3(S_BYTE *pmac,S_BYTE buflen)
// 	{
// 		if (buflen != CONS_CARD_MAC_LEN)
// 			return false;
// 
// 		memcpy(pmac,m_SIpcCardInfo[m_CurCardMp].m_MAC3,CONS_CARD_MAC_LEN);
// 		return true;
// 	}
// 	bool SetMac(S_BYTE mac[4])
// 	{
// 		if (m_CurCardMp < 0)
// 			return false;
// 
// 		memcpy(m_SIpcCardInfo[m_CurCardMp].m_MAC,mac,CONS_CARD_MAC_LEN);
// 		return true;
// 	}
// 	bool GetMac(S_BYTE *pmac,S_BYTE buflen)
// 	{
// 		if (buflen != CONS_CARD_MAC_LEN)
// 			return false;
// 
// 		memcpy(pmac,m_SIpcCardInfo[m_CurCardMp].m_MAC,CONS_CARD_MAC_LEN);
// 		return true;
// 	}
// 	bool SetBackInfo(S_BYTE backInfo[50])
// 	{
// 		if (m_CurCardMp < 0)
// 			return false;
// 
// 		memcpy(m_SIpcCardInfo[m_CurCardMp].m_BackInfo,backInfo,CONS_CARD_BACKINFO_LEN);
// 		return true;
// 	}
// 	bool GetBackInfo(S_BYTE *pbackInfo,S_BYTE buflen)
// 	{
// 		if (buflen != CONS_CARD_BACKINFO_LEN)
// 			return false;
// 
// 		memcpy(pbackInfo,m_SIpcCardInfo[m_CurCardMp].m_BackInfo,CONS_CARD_BACKINFO_LEN);
// 		return true;
// 	}


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

