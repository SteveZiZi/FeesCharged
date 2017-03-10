/** @file
 *  @brief QT进程与后台监控进程共享内存通讯接口类
 *  @author Steven Wan
 *  @date 2015-08/30
 *  @version 0.1
*/
 
#include "ipcSharemem.h"

ShareMem SIpcShareMem::m_IpcShareMem;

SIpcShareMem::SIpcShareMem()
{
	memset(this,0,sizeof(*this));
	Checker.Init();
}
/*
	brief: 共享文件初始化
	param [in] FILE* fp：共享文件指针
	returnt true：格式正确，false：格式错误
	author:Steven Wan
	date: 2015-08-30
*/
bool SIpcShareMem::InitFile(FILE* fp, void* ipOther)
{
	ASSERT(fp);
	if (fseek(fp,0L ,SEEK_SET)!=0)
	{
		printf("fseek(fp,0L ,SEEK_SET) error in SIpcShareMem::InitFile\n");
		return false;
	}
	SIpcShareMem* pIpcShareMem=new SIpcShareMem;
	bool bResult=(fwrite(pIpcShareMem,sizeof(SIpcShareMem),1,fp)==1);
	delete pIpcShareMem;
	return bResult;
}
/*
	brief: 共享文件头校验检测
	param [in] FILE* fp：共享文件指针
	return bool：true：格式正确，false：格式错误
	author:Steven Wan
	date: 2015-08-30
*/
bool SIpcShareMem::CheckFile(FILE* fp, void* ipOther)
{
	if (fseek(fp,0L,SEEK_END)!=0)
	{
		printf("fseek(fp,0L,SEEK_END) error in SIpcShareMem::InitFile\n");
		return false;
	}
	long len=ftell(fp);
	if (len>0 && (unsigned int)len==sizeof(SIpcShareMem))
	{
		fseek(fp,0 ,SEEK_SET);
		HeaderChecker checker;
		if (fread(&checker,sizeof(HeaderChecker),1,fp)==1)
		{
			HeaderChecker initChecker;
			if (memcmp(&checker,&initChecker,sizeof(HeaderChecker))==0)
			{
				return true;
			}
		}
	}
	printf("SIpcShareMem file check return false\n");
	return false;
}

/*
	brief: 共享内存获取入口
	param [in] void
	param [out] none
	return SIpcShareMem：共享内存对象
	author:Steven Wan
	date: 2015-08-30
*/
SIpcShareMem& SIpcShareMem::Instance(void)
{	
	static SIpcShareMem* pIpcShareMem=NULL;
	printf("********** sizeof(SIpcShareMem) =%d\n",sizeof(SIpcShareMem));
	if (pIpcShareMem==NULL)
	{
		if (m_IpcShareMem.IsInited()==false)
		{
			m_IpcShareMem.Init(FILE_IPC_SHAREMEM,sizeof(SIpcShareMem),SIpcShareMem::InitFile,SIpcShareMem::CheckFile,NULL);
		}

		pIpcShareMem=(SIpcShareMem*)m_IpcShareMem.GetMem();
	}
	ASSERT(pIpcShareMem!=NULL);
	ASSERT(pIpcShareMem==m_IpcShareMem.GetMem());
	return *pIpcShareMem;
}

bool SIpcShareMem::SetCardType(T_emCARDTYPE cardtype)
{
	if (cardtype < PRESET_CARD || cardtype > USER_CARD)
		return false;
	
	m_SIpcCardInfo[m_CurCardMp].m_CardType = cardtype;
	return true;
}
T_emCARDTYPE SIpcShareMem::GetCardType()
{
	return (T_emCARDTYPE)(m_SIpcCardInfo[m_CurCardMp].m_CardType);
}

bool SIpcShareMem::SetCardState(T_emCARDSTATE cardstate)
{
	if (m_CurCardMp < 0)
		return false;
	
	m_SIpcCardInfo[m_CurCardMp].m_State = cardstate;
	return true;
}

T_emCARDSTATE SIpcShareMem::GetCardState()
{
	if (m_CurCardMp < 0)
		return CARD_OUT;
	
	return (T_emCARDSTATE)(m_SIpcCardInfo[m_CurCardMp].m_State);
}

bool SIpcShareMem::SetCardSerialNo(INT8U serialno[8])
{
	if (m_CurCardMp < 0)
		return false;
	
	memcpy(m_SIpcCardInfo[m_CurCardMp].m_CardSerialNo,serialno,8);
	return true;
}
bool SIpcShareMem::GetCardSerialNo(INT8U *pSerialnobuf, INT8U buflen)
{
	if (buflen != CONS_CARD_SERIALNO_LEN)
		return false;
	
	memcpy(pSerialnobuf,m_SIpcCardInfo[m_CurCardMp].m_CardSerialNo,CONS_CARD_SERIALNO_LEN);
	return true;
}

int  SIpcShareMem::SetCurCardMp(INT8U meterno[8])
{
	SIpcUserParm userparm;
	
	for(int i=1; i<CONS_DEVICE_MAX_COUNT; i++)
	{
		userparm = m_IpcTerminalInfo.GetUserParm(i);
		if (memcmp(userparm.m_MeterSerialNo,meterno,sizeof(userparm.m_MeterSerialNo)) == 0)
		{
			m_CurCardMp = i;
			SetMeterNo(meterno);
			return m_CurCardMp;
		}
	}
	m_CurCardMp = -1;
	return m_CurCardMp;
}
bool SIpcShareMem::SetMeterNo(INT8U meterno[8])
{
	if (m_CurCardMp < 0)
		return false;
	
	memcpy(m_SIpcCardInfo[m_CurCardMp].m_CardMeterNo,meterno,CONS_CARD_METERNO_LEN);
	return true;
}
bool SIpcShareMem::GetMeterNo(INT8U *pmeternobuf, INT8U buflen)
{
	if (buflen != CONS_CARD_METERNO_LEN)
		return false;
	
	memcpy(pmeternobuf,m_SIpcCardInfo[m_CurCardMp].m_CardMeterNo,CONS_CARD_METERNO_LEN);
	return true;
}

#if 0
bool SIpcShareMem::setTraceType(T_emTraceType traceType)
{
	m_traceType = traceType;
	return true;
}

bool SIpcShareMem::SetEsamRandData(S_BYTE esamRandData[8])
{
	if (m_CurCardMp < 0)
		return false;

	memcpy(m_SIpcCardInfo[m_CurCardMp].m_EsamRandData,esamRandData,CONS_CARD_EASM_RAM_LEN);
	return true;
}

bool SIpcShareMem::GetEsamRandData(S_BYTE *pesamRandData,S_BYTE buflen)
{
	if (buflen != CONS_CARD_EASM_RAM_LEN)
		return false;

	memcpy(pesamRandData,m_SIpcCardInfo[m_CurCardMp].m_EsamRandData,CONS_CARD_EASM_RAM_LEN);
	return true;
}

bool SIpcShareMem::SetDstData(S_BYTE dstData[8])
{
	if (m_CurCardMp < 0)
		return false;

	memcpy(m_SIpcCardInfo[m_CurCardMp].m_DstData,dstData,CONS_CARD_DST_DATA_LEN);
	return true;
}

bool SIpcShareMem::GetDstData(S_BYTE *pdstData,S_BYTE buflen)
{
	if (buflen != CONS_CARD_DST_DATA_LEN)
		return false;

	memcpy(pdstData,m_SIpcCardInfo[m_CurCardMp].m_DstData,CONS_CARD_DST_DATA_LEN);
	return true;
}

bool SIpcShareMem::SetRechargeVal(S_BYTE rechargeVal[4])
{
	if (m_CurCardMp < 0)
		return false;

	logvt<<"SetRechargeVal: +++++++++++++"<<endl;
	//memcpy(m_SIpcCardInfo[m_CurCardMp].m_RechargeVal,rechargeVal,CONS_CARD_RECHARGE_VAL_LEN);
	return true;
}

bool SIpcShareMem::GetRechargeVal(float &drechargeVal)
{
	if (m_CurCardMp < 0)
		return false;

	//drechargeVal = BCD62_to_Double((char *)m_SIpcCardInfo[m_CurCardMp].m_RechargeVal);
	return true;
}

bool SIpcShareMem::SetRechargeTimes(const INT32U rechargeTimes)
{
	if (m_CurCardMp < 0)
		return false;

	m_SIpcCardInfo[m_CurCardMp].m_RechargeTimes = rechargeTimes;
	return true;
}

INT32U SIpcShareMem::GetRechargeTimes() const
{
	return m_SIpcCardInfo[m_CurCardMp].m_RechargeTimes;
}

bool SIpcShareMem::SetMAC1(S_BYTE mac[4])
{
	if (m_CurCardMp < 0)
		return false;

	memcpy(m_SIpcCardInfo[m_CurCardMp].m_MAC1,mac,CONS_CARD_MAC_LEN);
	return true;
}

bool SIpcShareMem::GetMAC1(S_BYTE *pmac,S_BYTE buflen)
{
	if (buflen != CONS_CARD_MAC_LEN)
		return false;

	memcpy(pmac,m_SIpcCardInfo[m_CurCardMp].m_MAC1,CONS_CARD_MAC_LEN);
	return true;
}

bool SIpcShareMem::SetCustomerNo(S_BYTE customerNo[6])
{
	if (m_CurCardMp < 0)
		return false;

	memcpy(m_SIpcCardInfo[m_CurCardMp].m_CustomerNo,customerNo,CONS_CARD_CUSTOMERNO_LEN);
	return true;
}

bool SIpcShareMem::GetCustomerNo(S_BYTE *pcustomerNo,S_BYTE buflen)
{
	if (buflen != CONS_CARD_CUSTOMERNO_LEN)
		return false;

	memcpy(pcustomerNo,m_SIpcCardInfo[m_CurCardMp].m_CustomerNo,CONS_CARD_CUSTOMERNO_LEN);
	return true;
}

bool SIpcShareMem::SetMAC2(S_BYTE mac[4])
{
	if (m_CurCardMp < 0)
		return false;

	memcpy(m_SIpcCardInfo[m_CurCardMp].m_MAC2,mac,CONS_CARD_MAC_LEN);
	return true;
}

bool SIpcShareMem::GetMAC2(S_BYTE *pmac,S_BYTE buflen)
{
	if (buflen != CONS_CARD_MAC_LEN)
		return false;

	memcpy(pmac,m_SIpcCardInfo[m_CurCardMp].m_MAC2,CONS_CARD_MAC_LEN);
	return true;
}

bool SIpcShareMem::SetParamInfo(S_BYTE *pparamInfo,S_BYTE buflen)
{
	if (m_CurCardMp < 0 || buflen <= 0)
		return false;

	memcpy(m_SIpcCardInfo[m_CurCardMp].m_ParamInfo,pparamInfo,CONS_CARD_MAC_LEN);
	return true;
}

bool SIpcShareMem::GetParamInfo(S_BYTE *pparamInfo,S_BYTE buflen)
{
	if (buflen != m_SIpcCardInfo[m_CurCardMp].m_ParamInfoLen)
		return false;

	memcpy(pparamInfo,m_SIpcCardInfo[m_CurCardMp].m_MAC2,buflen);
	return true;
}

bool SIpcShareMem::SetParamInfoLen(const S_WORD paramInfoLen)
{
	if (m_CurCardMp < 0)
		return false;

	m_SIpcCardInfo[m_CurCardMp].m_ParamInfoLen = paramInfoLen;
	return true;
}

S_WORD SIpcShareMem::GetParamInfoLen() const
{
	return m_SIpcCardInfo[m_CurCardMp].m_ParamInfoLen;
}

bool SIpcShareMem::SetMAC3(S_BYTE mac[4])
{
	if (m_CurCardMp < 0)
		return false;

	memcpy(m_SIpcCardInfo[m_CurCardMp].m_MAC3,mac,CONS_CARD_MAC_LEN);
	return true;
}

bool SIpcShareMem::GetMAC3(S_BYTE *pmac,S_BYTE buflen)
{
	if (buflen != CONS_CARD_MAC_LEN)
		return false;

	memcpy(pmac,m_SIpcCardInfo[m_CurCardMp].m_MAC3,CONS_CARD_MAC_LEN);
	return true;
}

bool SIpcShareMem::SetMac(S_BYTE mac[4])
{
	if (m_CurCardMp < 0)
		return false;

	memcpy(m_SIpcCardInfo[m_CurCardMp].m_MAC,mac,CONS_CARD_MAC_LEN);
	return true;
}

bool SIpcShareMem::GetMac(S_BYTE *pmac,S_BYTE buflen)
{
	if (buflen != CONS_CARD_MAC_LEN)
		return false;

	memcpy(pmac,m_SIpcCardInfo[m_CurCardMp].m_MAC,CONS_CARD_MAC_LEN);
	return true;
}

bool SIpcShareMem::SetBackInfo(S_BYTE backInfo[50])
{
	if (m_CurCardMp < 0)
		return false;

	memcpy(m_SIpcCardInfo[m_CurCardMp].m_BackInfo,backInfo,CONS_CARD_BACKINFO_LEN);
	return true;
}

bool SIpcShareMem::GetBackInfo(S_BYTE *pbackInfo,S_BYTE buflen)
{
	if (buflen != CONS_CARD_BACKINFO_LEN)
		return false;

	memcpy(pbackInfo,m_SIpcCardInfo[m_CurCardMp].m_BackInfo,CONS_CARD_BACKINFO_LEN);
	return true;
}
#endif


