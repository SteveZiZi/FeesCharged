/** @file
 *  @brief RFID管理类
 *  @author 
 *  @date 2016/12/06
 *  @version 0.1
 *  @note RFID管理类定义和实现
 */
#include "rfidMgr.h"

#include "htrace.h"
#include "ipcSharemem.h"

//
// RFID管理
//
C_RFIDMgr::C_RFIDMgr()
: m_CurCardMp(-1)
{
	ResetCardInfo();
}

C_RFIDMgr::~C_RFIDMgr()
{

}

/** @brief 初始化RFID通道参数 
*  @return none
*/
void C_RFIDMgr::InitComParam(void)
{
	m_RFIDCom.InitComParam();
}

/** @brief 初始化RFID通道 
*  @return -1:失败 0 成功
*/
int C_RFIDMgr::InitRFIDComm(void)
{
	return m_RFIDCom.InitRFIDComm();
}

/** @brief 从卡片中获取信息 
*  @return -1 出错；0 正确
*/
int C_RFIDMgr::ReadInfoFromCard(const S_WORD& CmdParam)
{
	int result = -1;

	string SstrData;
	string RstrData;
	switch (CmdParam)
	{
	case QUERY_READER_VER:
		result = QueryReaderVerInfo(CmdParam,SstrData,RstrData);
		break;
	case BUZZER_CURL:
		result = BuzzerCtrl(CmdParam,SstrData,RstrData);
		break;
	case LED_STATUS_CTRL:
		result = LedStatusCtrl(CmdParam,SstrData,RstrData);
		break;
	case READ_CARD_INFO:
		result = ReadCardInfo(CmdParam,SstrData,RstrData);
		break;
	case IDENTIFY_AUTH:
		result = IdentifyAuth(CmdParam,SstrData,RstrData);
		break;
	case START_CARD_HANDEL:
		result = StartCardHandle(CmdParam,SstrData,RstrData);
		break;
	case END_CARD_HANDEL:
		result = EndCardHandle(CmdParam,SstrData,RstrData);
		break;
	case PARAM_SET:
		result = ParamSet(CmdParam,SstrData,RstrData);
		break;
	case REMOTE_HANDEL:
		result = RemoteHandle(CmdParam,SstrData,RstrData);
		break;
	case REMOTE_IDENTIFY_AUTH:
		result = RemoteIdentityAuth(CmdParam,SstrData,RstrData);
		break;
	case KEY_UPDATE:
		result = KeyUpdate(CmdParam,SstrData,RstrData);
		break;
	default:
		break;
	}

	return result;
}

/** @brief 获取卡序号 
*  @return true 成功；false 失败
*/
bool C_RFIDMgr::GetCardSerialNo(S_CHAR *pSerialnobuf, S_BYTE buflen)
{
	if (buflen != CONS_CARD_SERIALNO_LEN)
		return false;

	memcpy(pSerialnobuf,m_CardSerialNo,CONS_CARD_SERIALNO_LEN);
	return true;
}

/** @brief 获取卡端信息中的表号 
*  @return true 成功；false 失败
*/
bool C_RFIDMgr::GetMeterNo(S_CHAR *pMeterNo,S_BYTE buflen)
{
	if (buflen != CONS_CARD_METERNO_LEN)
		return false;

	memcpy(pMeterNo,m_CardMeterNo,CONS_CARD_METERNO_LEN);
	return true;
}

/** @brief 获取卡端8字节随机数 
*  @return true 成功；false 失败
*/
bool C_RFIDMgr::GetEsamRandData(S_CHAR *pesamRandData,S_BYTE buflen)
{
	if (buflen != CONS_CARD_EASM_RAM_LEN)
		return false;

	memcpy(pesamRandData,m_CardRandData,CONS_CARD_EASM_RAM_LEN);
	return true;
}

/** @brief 获取8字节密文 
*  @return true 成功；false 失败
*/
bool C_RFIDMgr::GetDstData(S_CHAR *pdstData,S_BYTE buflen)
{
	if (buflen != CONS_CARD_DST_DATA_LEN)
		return false;

	memcpy(pdstData,m_CardDstData,CONS_CARD_DST_DATA_LEN);
	return true;
}

/** @brief 获取4字节MAC1 
*  @return true 成功；false 失败
*/
bool C_RFIDMgr::GetMAC1(S_CHAR *pmac,S_BYTE buflen)
{
	if (buflen != CONS_CARD_MAC_LEN)
		return false;

	memcpy(pmac,m_CardMAC1,CONS_CARD_MAC_LEN);
	return true;
}

/** @brief 获取客户序列号 
*  @return true 成功；false 失败
*/
bool C_RFIDMgr::GetCustomerNo(S_CHAR *pcustomerNo,S_BYTE buflen)
{
	if (buflen != CONS_CARD_CUSTOMERNO_LEN)
		return false;

	memcpy(pcustomerNo,m_CardCustomerNo,CONS_CARD_CUSTOMERNO_LEN);
	return true;
}

/** @brief 获取4字节MAC2 
*  @return true 成功；false 失败
*/
bool C_RFIDMgr::GetMAC2(S_CHAR *pmac,S_BYTE buflen)
{
	if (buflen != CONS_CARD_MAC_LEN)
		return false;

	memcpy(pmac,m_CardMAC2,CONS_CARD_MAC_LEN);
	return true;
}

/** @brief 设置8字节模块easm随机数 
*  @return true 成功；false 失败
*/
bool C_RFIDMgr::SetMeterEsamRand(S_CHAR meterEasmRand[8])
{
	memcpy(m_MeterEsamRand,meterEasmRand,CONS_CARD_EASM_RAM_LEN);
	return true;
}

/** @brief 设置4字节电表返回随机数 
*  @return true 成功；false 失败
*/
bool C_RFIDMgr::SetMeterRand(S_CHAR meterRand[4])
{
	memcpy(m_MeterRand,meterRand,4);
	return true;
}

/** @brief 设置4字节MAC 
*  @return true 成功；false 失败
*/
bool C_RFIDMgr::SetMac(S_CHAR mac[4])
{
	memcpy(m_MeterMAC,mac,CONS_CARD_MAC_LEN);
	return true;
}

/** @brief 设置50字节返写信息 
*  @return true 成功；false 失败
*/
bool C_RFIDMgr::SetBackInfo(S_CHAR backInfo[50])
{
	memcpy(m_MeterBackInfo,backInfo,CONS_CARD_BACKINFO_LEN);
	return true;
}

/** @brief 充值卡端信息
*  @return none
*/
void C_RFIDMgr::ResetCardInfo(void)
{
	memset(m_CardSerialNo,0,sizeof(m_CardSerialNo));					// 卡序号
	memset(m_CardMeterNo,0,sizeof(m_CardMeterNo));						// 表号
	memset(m_CardRandData,0,sizeof(m_CardRandData));					// 8字节模块ESAM随机数
	memset(m_CardDstData,0,sizeof(m_CardDstData));						// 8字节密文
	memset(m_CardMAC1,0,sizeof(m_CardMAC1));
	memset(m_CardCustomerNo,0,sizeof(m_CardCustomerNo));				// 6字节bcd,客户编号
	memset(m_CardMAC2,0,sizeof(m_CardMAC2));
	memset(m_MeterEsamRand,0,sizeof(m_MeterEsamRand));					// 8字节模块easm随机数  由电表返回
	memset(m_MeterRand,0,sizeof(m_MeterRand));							// 4字节随机数   
	memset(m_MeterMAC,0,sizeof(m_MeterMAC));							// MAC 由电表返回
	memset(m_MeterBackInfo,0,sizeof(m_MeterBackInfo));					// 返写信息 由电表返回
}

/** @brief 断开连接 
*  @return true 成功；false 失败
*/
S_BOOL C_RFIDMgr::Disconnect(void)
{
	return m_RFIDCom.Disconnect();
}

/** @brief 获取读卡器版本信息 
*  @return -1 出错；0 正确
*/
int C_RFIDMgr::QueryReaderVerInfo(const S_WORD& CmdParam,string& SstrData,string& RstrData)
{
	int result = -1;

	string VerInfo;//n字节版本信息

	result = m_RFIDCom.Read(CmdParam,SstrData,RstrData);

	if(result==0)
	{
		VerInfo.append(RstrData);
		logwt<<"RFTX 查询读卡器版本信息成功!"<<endl;
		logwt<<"RFTX VerInfo = "<<HEX<<VerInfo<<"!"<<endl;
	}
	else
	{
		VerInfo.append("no verinfo");
		logwt<<"RFTX 查询读卡器版本信息失败 no verinfo!"<<endl;
	}

	return result;
}

/** @brief 控制蜂鸣器 
*  @return -1 出错；0 正确
*/
int C_RFIDMgr::BuzzerCtrl(const S_WORD& CmdParam,string& SstrData,string& RstrData)
{
	int result = -1;

	S_DWORD DelayTime  = 100; //0000H～FFFFH; 0000H-蜂鸣器长鸣
	S_CHAR Times = 2;
	SstrData.push_back((S_CHAR)DelayTime/0x100);
	SstrData.push_back((S_CHAR)DelayTime%0x100);
	SstrData.push_back(Times);

	result = m_RFIDCom.Read(CmdParam,SstrData,RstrData);
	if(result)
	{
		logwt<<"RFTX 蜂鸣器控制失败!"<<endl;
	}
	else
	{
		logwt<<"RFTX 蜂鸣器控制成功!"<<endl;
		logwt<<"RFTX BuzzerCtrl RstrData = "<<HEX<<RstrData<<"!"<<endl;
	}

	return result;
}

/** @brief 控制LED状态
*  @return -1 出错；0 正确
*/
int C_RFIDMgr::LedStatusCtrl(const S_WORD& CmdParam,string& SstrData,string& RstrData)
{
	int result = -1;

	S_CHAR Led = 0xC0; // Led  0-灭; 1-亮;  bit7-绿灯;  bit6-红灯;  bit5-bit0-预留;
	SstrData.push_back(Led);

	result = m_RFIDCom.Read(CmdParam,SstrData,RstrData);

	if(result)
	{
		logwt<<"RFTX LED 状态控制失败!"<<endl;
	}
	else
	{
		logwt<<"RFTX LED 状态控制成功!"<<endl;
		logwt<<"RFTX LedStatusCtrl RstrData = "<<HEX<<RstrData<<"!"<<endl;
	}

	return result;
}

/** @brief 读取卡片基本信息
*  @return -1 出错；0 正确
*/
int C_RFIDMgr::ReadCardInfo(const S_WORD& CmdParam,string& SstrData,string& RstrData)
{
	int result = -1;

	result = m_RFIDCom.Read(CmdParam,SstrData,RstrData);

	if(result)
	{
		logwt<<"RFTX 查询卡信息失败!"<<endl;
		IPCSHAREMEM.SetCardState(CARD_OUT);
		result = -1;
	}
	else
	{
		logwt<<"RFTX 查询卡信息成功 ReadCardInfo RstrData = "<<HEX<<RstrData<<"!"<<endl;

		S_CHAR card_type;

		card_type = RstrData.at(8);
		IPCSHAREMEM.SetCardType((T_emCARDTYPE)card_type);

		if (PRESET_CARD == card_type){
			S_CHAR meterNo[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01};
			memcpy(m_CardMeterNo,meterNo,sizeof(meterNo));
		}
		else
			memcpy(m_CardMeterNo,RstrData.data()+9,sizeof(m_CardMeterNo));

		//只有用户卡会设置电表号，预制卡没有表号内容
		m_CurCardMp = IPCSHAREMEM.SetCurCardMp((S_BYTE*)m_CardMeterNo);

		logwt<<"CardType" << card_type << endl;

		memcpy(m_CardSerialNo,RstrData.data(),sizeof(m_CardSerialNo));
		IPCSHAREMEM.SetCardState(CARD_IN);

	}
	IPCSHAREMEM.Update();

	return result;
}

/** @brief 卡片身份认证
*  @return -1 出错；0 正确
*/
int C_RFIDMgr::IdentifyAuth(const S_WORD& CmdParam,string& SstrData,string& RstrData)
{
	int result = -1;

	S_CHAR meterNo[8] = {0};	// 表号

	GetMeterNo(meterNo,sizeof(meterNo));
	SstrData.append(meterNo,sizeof(meterNo));

	result = m_RFIDCom.Read(CmdParam,SstrData,RstrData);

	if(result)
	{
		logwt<<"RFTX 身份认证失败!"<<endl;
	}
	else
	{
		logwt<<"RFTX 身份认证成功 IdentityAuth RstrData = "<<HEX<<RstrData<<"!"<<endl;

		memcpy(m_CardRandData,RstrData.data(),sizeof(m_CardRandData)); 
		memcpy(m_CardDstData,RstrData.data()+8,sizeof(m_CardDstData)); 

		string printstr;
		printstr.append(m_CardRandData,sizeof(m_CardRandData));
		logvt<<"m_CardRandData:"<<hex<<printstr<<endl;

		printstr.clear();
		printstr.append(m_CardDstData,sizeof(m_CardDstData));
		logvt<<"m_DstData:"<<hex<<printstr<<endl;
	}

	return result;
}

/** @brief 开始卡片交易
*  @return -1 出错；0 正确
*/
int C_RFIDMgr::StartCardHandle(const S_WORD& CmdParam,string& SstrData,string& RstrData)
{
	int result = -1;

	string strResponse;

	///表号
	S_CHAR meterno[8] = {0};
	GetMeterNo(meterno,sizeof(meterno));
	SstrData.append(meterno,8);
	SstrData.append(m_CardRandData,sizeof(m_CardRandData));				// 8字节模块esam随机数

	// 表端信息
 	SstrData.append(m_MeterRand,sizeof(m_MeterRand));		// 4字节表端ESAM随机数

	result = m_RFIDCom.Read(CmdParam,SstrData,RstrData);

	if(result)
	{
		logwt<<"RFTX 发起卡交易失败!"<<endl;
	}
	else
	{
		logwt<<"RFTX 发起卡交易成功 StartCardHandle RstrData = "<<HEX<<RstrData<<"!"<<endl;
		if (!GetCardInfo(RstrData))
			result = -1;
	}

	return result;
}

/** @brief 结束卡片交易 --信息返写
*  @return -1 出错；0 正确
*/
int C_RFIDMgr::EndCardHandle(const S_WORD& CmdParam,string& SstrData,string& RstrData)
{
	int result = -1;

	// 表端获取信息
	SstrData.append(m_MeterBackInfo,sizeof(m_MeterBackInfo));
	SstrData.append(m_MeterMAC,sizeof(m_MeterMAC));

	result = m_RFIDCom.Read(CmdParam,SstrData,RstrData);


	if(result)
	{
		logwt<<"RFTX 结束卡交易失败!"<<endl;
	}
	else
	{
		logwt<<"RFTX 结束卡交易成功 EndCardHandle RstrData = "<<HEX<<RstrData<<"!"<<endl;
	}

	return result;
}

/** @brief 参数设置
*  @return -1 出错；0 正确
*/
int C_RFIDMgr::ParamSet(const S_WORD& CmdParam,string& SstrData,string& RstrData)
{
	int result = -1;

	S_CHAR Rand[8] ={0xC8,0x3D,0x26,0xFE,0x9C,0x9A,0x9E,0x1E};
	S_CHAR R4[4] ={0x00,0x00,0x00,0x00};

	SstrData.append(Rand,sizeof(Rand));
	SstrData.append(R4,sizeof(R4));	

	result = m_RFIDCom.Read(CmdParam,SstrData,RstrData);


	if(result)
	{
		logwt<<"RFTX 发起参数设置失败!"<<endl;
	}
	else
	{
		logwt<<"RFTX 发起参数设置成功 ParamSet RstrData = "<<HEX<<RstrData<<"!"<<endl;
	}

	return result;
}

/** @brief 远程交易
*  @return -1 出错；0 正确
*/
int C_RFIDMgr::RemoteHandle(const S_WORD& CmdParam,string& SstrData,string& RstrData)
{
	int result = -1;
    
	S_CHAR Rand[8] ={0xC8,0x3D,0x26,0xFE,0x9C,0x9A,0x9E,0x1E};
	S_CHAR R4[4] ={0x00,0x00,0x00,0x00};

	SstrData.append(Rand,sizeof(Rand));
	SstrData.append(R4,sizeof(R4));	

	result = m_RFIDCom.Read(CmdParam,SstrData,RstrData);


	if(result)
	{
		logwt<<"RFTX 发起远程交易失败!"<<endl;
	}
	else
	{
		logwt<<"RFTX 发起远程交易成功 RemoteHandle RstrData = "<<HEX<<RstrData<<"!"<<endl;
	}

	return result;
}

/** @brief 远程身份认证
*  @return -1 出错；0 正确
*/
int C_RFIDMgr::RemoteIdentityAuth(const S_WORD& CmdParam,string& SstrData,string& RstrData)
{
	int result = -1;
    
	S_CHAR TerNum[8] ={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01};	
	S_CHAR Rand[8] ={0xC8,0x3D,0x26,0xFE,0x9C,0x9A,0x9E,0x1E};
	S_CHAR Enc1[8] ={0x00,0x00,0x00,0x00};

	SstrData.append(TerNum,sizeof(TerNum));
	SstrData.append(Rand,sizeof(Rand));
	SstrData.append(Enc1,sizeof(Enc1));	

	result = m_RFIDCom.Read(CmdParam,SstrData,RstrData);


	if(result)
	{
		logwt<<"RFTX 发起远程身份认证失败!"<<endl;
	}
	else
	{
		logwt<<"RFTX 发起远程身份认证成功 RemoteIdentityAuth RstrData = "<<HEX<<RstrData<<"!"<<endl;
	}

	return result;
}

/** @brief 密钥更新
*  @return -1 出错；0 正确
*/
int C_RFIDMgr::KeyUpdate(const S_WORD& CmdParam,string& SstrData,string& RstrData)
{
	int result = -1;
  
#if 1
	// 密钥下装
	S_CHAR Key1[32] = {0x9E,0xA8,0x42,0x25,0x6E,0x08,0xB2,0x2B,0x1E,0xDF,0x32,0x6F,0x15,0x7B,0x3E,0x26,0x75,0xC9,0x42,0x7E,0xCF,0x72,0x7D,0xC2,0xB5,0x56,0x68,0xCF,0x40,0x1A,0x47,0xB1};
	S_CHAR Key2[32] = {0x49,0x6C,0x71,0xC9,0xD6,0xB7,0x8A,0xCF,0x63,0x5B,0xA5,0xC4,0xBB,0x7E,0x37,0x77,0xDC,0x92,0xFE,0xFD,0x1B,0x3C,0x59,0x8C,0x8C,0xA0,0x3F,0x5A,0x89,0x85,0x5A,0xAB};
	S_CHAR Key3[32] = {0x3C,0x8C,0x6F,0x9E,0xF3,0x0B,0xA8,0x57,0x39,0x61,0xEB,0x21,0x7F,0xE3,0xC8,0x93,0x38,0x43,0x25,0x3A,0x23,0xEC,0x4D,0x23,0x53,0x21,0x67,0x6F,0xA0,0xCA,0xBD,0x94};
	S_CHAR Key4[32] = {0x19,0x03,0x5A,0xFB,0xB3,0xD5,0xA9,0x58,0x13,0xAF,0x4D,0x61,0x8E,0x3E,0x3D,0x9E,0xA7,0xA0,0xAF,0x28,0xC7,0x61,0xDF,0x8C,0xE6,0x19,0x76,0x1D,0xF5,0x17,0x9F,0x3C};
	S_CHAR Key5[32] = {0xB2,0x82,0xDD,0x00,0x6D,0xE1,0x3C,0xB0,0xAD,0x59,0x15,0x70,0x77,0x0B,0x13,0xB0,0xDA,0x6B,0x17,0x6A,0x56,0x8E,0x8F,0x02,0x4B,0xCA,0x42,0x3C,0x4B,0xF2,0x2A,0xA5};
#else
	// 密钥恢复
	S_CHAR Key1[32] = {0x5A,0x5D,0x6D,0x0E,0xA0,0xC8,0x78,0xFD,0x12,0x9E,0x47,0xF6,0x70,0x89,0x85,0x80,0xA2,0x25,0x5A,0xF5,0x32,0x0B,0x39,0xF4,0x9E,0x07,0x7D,0xCD,0x0B,0x2E,0x28,0x2D};
	S_CHAR Key2[32] = {0x89,0x00,0xF2,0x1B,0x8F,0x2D,0x51,0xEE,0xA8,0x01,0xDA,0x05,0xA7,0x2C,0x4F,0x31,0x04,0xA3,0xB0,0xA6,0xCA,0x93,0xE3,0xED,0x84,0xC6,0x34,0xF6,0xC5,0x1D,0xEC,0x05};
	S_CHAR Key3[32] = {0xD4,0xFE,0xF8,0xCA,0x0C,0x1A,0x0E,0x6A,0x24,0x63,0x7C,0xCA,0x62,0x31,0xBA,0xB9,0x83,0x83,0x45,0xDB,0xCE,0xF8,0xE1,0x11,0x18,0x87,0xA0,0x4B,0x18,0xC4,0x1C,0xE7};
	S_CHAR Key4[32] = {0x47,0xB7,0xEB,0xE1,0x1D,0x8E,0xCF,0x3C,0xA4,0x1B,0xC2,0x5A,0x90,0x19,0xD7,0xBB,0x9C,0x1A,0xD2,0xAC,0x34,0x4A,0xDA,0xD8,0x11,0xFC,0x3F,0x08,0x21,0x7E,0x26,0x31};
	S_CHAR Key5[32] = {0x28,0x06,0x06,0x93,0x89,0xBE,0xFA,0xEC,0x34,0xE7,0x86,0x74,0x47,0x3F,0x33,0x26,0x37,0xAE,0x66,0x1B,0xC4,0xF9,0x8A,0x5F,0x53,0x3A,0x01,0x34,0xAE,0x1E,0x1B,0xB0};
#endif

	SstrData.append(Key1,sizeof(Key1));
	SstrData.append(Key2,sizeof(Key2));
	SstrData.append(Key3,sizeof(Key3));
	SstrData.append(Key4,sizeof(Key4));
	SstrData.append(Key5,sizeof(Key5));

	result = m_RFIDCom.Read(CmdParam,SstrData,RstrData);


	if(result)
	{
		logwt<<"RFTX 密钥更新失败!"<<endl;
	}
	else
	{
		logwt<<"RFTX 密钥更新成功 KeyUpdate RstrData = "<<HEX<<RstrData<<"!"<<endl;
	}

	return result;
}

/** @brief 卡片发起交易成功后获取卡片上的信息
*  @return false 出错；true 正确
*/
bool C_RFIDMgr::GetCardInfo(string &strResponse)
{
	if (strResponse.size() <=0)
	{
		logwt<<"发起卡片交易后获取卡片信息失败!"<<endl;
		return false;
	}
	else
	{
		string printstr;
		// ChargeValue
		S_CHAR ChargeValue[4] = {0};
		memcpy(ChargeValue,strResponse.data() + 2,sizeof(ChargeValue));
		printstr.append(ChargeValue,sizeof(ChargeValue));
		logvt<<"ChargeValue: "<<hex<<printstr<<endl;
		IPCSHAREMEM.SetRechargeVal((S_BYTE*)ChargeValue);

		// ChargeTime
		S_CHAR ChargeTime[4] = {0};
		memcpy(ChargeTime,strResponse.data() + 6,sizeof(ChargeTime));
		printstr.clear();
		printstr.append(ChargeTime,sizeof(ChargeTime));
		logvt<<"ChargeTime: "<<hex<<printstr<<endl;
		IPCSHAREMEM.SetRechargeTimes((S_BYTE*)ChargeTime);

		// MAC1
		memcpy(m_CardMAC1,strResponse.data() + 10,sizeof(m_CardMAC1));
		printstr.clear();
		printstr.append(m_CardMAC1,sizeof(m_CardMAC1));
		logvt<<"m_CardMAC1: "<<hex<<printstr<<endl;

		// CustomerNo
		memcpy(m_CardCustomerNo,strResponse.data() + 14,sizeof(m_CardCustomerNo));
		printstr.clear();
		printstr.append(m_CardCustomerNo,sizeof(m_CardCustomerNo));
		logvt<<"m_CardCustomerNo: "<<hex<<printstr<<endl;

		// MAC2
		memcpy(m_CardMAC2,strResponse.data() + 20,sizeof(m_CardMAC2));
		printstr.clear();
		printstr.append(m_CardMAC2,sizeof(m_CardMAC2));
		logvt<<"m_CardMAC2: "<<hex<<printstr<<endl;
		return true;
	}
}

