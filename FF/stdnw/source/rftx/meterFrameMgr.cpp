/** @file
 *  @brief ���ȫ��֤ϵ�й�Լ������
 *  @author 
 *  @date 2016/12/06
 *  @version 0.1
 *  @note ���ȫ��֤ϵ�й�Լ�����ඨ���ʵ��
 */
#include "meterFrameMgr.h"

#include "htrace.h"
#include "regconv.h"
#include "sysipc.h"
#include "status.h"
#include "nwmsrule.h"
#include "nwmscmd.h"
#include "context.h"
#include "mrfact.h"
#include "almfact.h"
#include "fbfact.h"
#include "busialgo.h"
#include "ipcSharemem.h"

//
//���ȫ��֤ϵ�й�Լ������
//
C_MeterFrameMgr::C_MeterFrameMgr()
{
	ResetParam();
}

C_MeterFrameMgr::~C_MeterFrameMgr()
{
}

/** @brief ����Dbcj��������ݲ����з���
*  @param[in] strCmd ����DI
*  @return true �ɹ���false ʧ��
*/
bool C_MeterFrameMgr::RequestDbcjMsg(const S_DWORD &CmdDI)
{
	bool ret = false;
	string SendFrame;
	string ResponseFrame;
	if (Make645Frame(CmdDI,SendFrame)){
		if (RequestRftxMsg2Dbcj(SendFrame,ResponseFrame)){
			bool Flag = false;
			S_DWORD Index = JudgeDbcjResult(CmdDI,ResponseFrame,Flag);
			if (Flag){
				AnalysisDbcjResult(Index,CmdDI,ResponseFrame);
				ret = true;
			}
		}
	}
	
	return ret;
}

/** @brief ����8�ֽڿ��˱�� 
*  @return true �ɹ���false ʧ��
*/
bool C_MeterFrameMgr::SetCardMeterNo(S_CHAR CardMeterNo[8])
{
	memcpy(m_CardMeterNo,CardMeterNo,CONS_CARD_METERNO_LEN);
	return true;
}

/** @brief ����8�ֽڿ�������� 
*  @return true �ɹ���false ʧ��
*/
bool C_MeterFrameMgr::SetCardRandData(S_CHAR CardRandData[8])
{
	memcpy(m_CardRandData,CardRandData,CONS_CARD_EASM_RAM_LEN);
	return true;
}

/** @brief ����8�ֽڿ������� 
*  @return true �ɹ���false ʧ��
*/
bool C_MeterFrameMgr::SetCardDstData(S_CHAR CardDstData[8])
{
	memcpy(m_CardDstData,CardDstData,CONS_CARD_DST_DATA_LEN);
	return true;
}

/** @brief ����4�ֽڿ���MAC1
*  @return true �ɹ���false ʧ��
*/
bool C_MeterFrameMgr::SetCardMac1(S_CHAR CardMac1[4])
{
	memcpy(m_CardMAC1,CardMac1,CONS_CARD_MAC_LEN);
	return true;
}

/** @brief ����6�ֽڿ��˿ͻ���� 
*  @return true �ɹ���false ʧ��
*/
bool C_MeterFrameMgr::SetCardCustomerNo(S_CHAR CardCustomerNo[6])
{
	memcpy(m_CardCustomerNo,CardCustomerNo,CONS_CARD_CUSTOMERNO_LEN);
	return true;
}

/** @brief ����4�ֽڿ���MAC2 
*  @return true �ɹ���false ʧ��
*/
bool C_MeterFrameMgr::SetCardMac2(S_CHAR CardMac2[4])
{
	memcpy(m_CardMAC2,CardMac2,CONS_CARD_MAC_LEN);
	return true;
}

/** @brief ��ȡ���8�ֽ�ģ��easm�����
*  @return true �ɹ���false ʧ��
*/
bool C_MeterFrameMgr::GetMeterEsamRand(S_CHAR *pMeterEsamRand,S_BYTE buflen)
{
	if (buflen != CONS_METER_ESAM_RAND_LEN)
		return false;

	memcpy(pMeterEsamRand,m_MeterEsamRand,CONS_METER_ESAM_RAND_LEN);
	return true;
}

/** @brief ��ȡ���4�ֽ������
*  @return true �ɹ���false ʧ��
*/
bool C_MeterFrameMgr::GetMeterRand(S_CHAR *pMeterRand,S_BYTE buflen)
{
	if (buflen != CONS_METER_RAND_LEN)
		return false;

	memcpy(pMeterRand,m_MeterRand,CONS_METER_RAND_LEN);
	return true;
}

/** @brief ��ȡ���MAC
*  @return true �ɹ���false ʧ��
*/
bool C_MeterFrameMgr::GetMeterMac(S_CHAR *pMac,S_BYTE buflen)
{
	if (buflen != CONS_CARD_MAC_LEN)
		return false;

	memcpy(pMac,m_MeterMAC,CONS_CARD_MAC_LEN);
	return true;
}

/** @brief ��ȡ��˷�д��Ϣ
*  @return true �ɹ���false ʧ��
*/
bool C_MeterFrameMgr::GetMeterBackInfo(S_CHAR *pMeterBackInfo,S_BYTE buflen)
{
	if (buflen != CONS_CARD_BACKINFO_LEN)
		return false;

	memcpy(pMeterBackInfo,m_MeterBackInfo,CONS_CARD_BACKINFO_LEN);
	return true;
}

/** @brief ���ò�����Ϣ
*  @param[in] strCmd ����DI
*  @return none
*/
void C_MeterFrameMgr::ResetParam()
{
	memset(m_MeterEsamRand,0,sizeof(m_MeterEsamRand));					// 8�ֽ�ģ��easm�����  �ɵ����
	memset(m_MeterRand,0,sizeof(m_MeterRand));							// 4�ֽ������  �ɵ����
	m_MeterMode = 0;													// ���ģʽ
	memset(m_MeterAddr,0,sizeof(m_MeterAddr));							// ����ַ
	memset(m_MeterMAC,0,sizeof(m_MeterMAC));							// MAC �ɵ����
	memset(m_MeterBackInfo,0,sizeof(m_MeterBackInfo));					// ��д��Ϣ �ɵ����

	// From Card
	memset(m_CardMeterNo,0,sizeof(m_CardMeterNo));						// ���
	memset(m_CardRandData,0,sizeof(m_CardRandData));					// 8�ֽ�ģ��ESAM�����
	memset(m_CardDstData,0,sizeof(m_CardDstData));						// 8�ֽ�����
	memset(m_CardMAC1,0,sizeof(m_CardMAC1));
	memset(m_CardCustomerNo,0,sizeof(m_CardCustomerNo));				// 6�ֽ�bcd,�ͻ����
	memset(m_CardMAC2,0,sizeof(m_CardMAC2));
}

/** @brief ��֡
*  @param[in] strCmd ����DI
*  @param[in] strAddress ��Ƶ�ַ
*  @param[in] strData �������뻺��
*  @param[out] SendFrame �����֡
*  @return ���ݳ���
*/
int C_MeterFrameMgr::Make645Frame(const S_DWORD &CmdDI,string& SendFrame)
{
	// Head
	string Head = MakeFrameHead(CmdDI);
	SendFrame.append(Head.begin(),Head.end());
	// DI
	string DI = MakeFrameDI(CmdDI);
	SendFrame.append(DI.begin(),DI.end());
	// Data
	if (METER_GET_MODE != CmdDI)
	{
		string OperatorCode = GetOperatorCode();
		string FrameData = MakeFrameData(CmdDI);
		SendFrame.append(OperatorCode.begin(),OperatorCode.end());
		SendFrame.append(FrameData.begin(),FrameData.end());
	}

	// end
	MakeFrameEnd(SendFrame);

	return SendFrame.size();
}

/** @brief ��dbcj������������
*  @param[in] SendstrFrame ��dbcj�����͵�֡
*  @param[out] strResponse �յ�dbcj����Ļظ�֡
*  @return ���ݳ���
*/
bool C_MeterFrameMgr::RequestRftxMsg2Dbcj(string &SendstrFrame, string &strResponse)
{
	string strIpcData;
	const char *pPrecess = "dbcj1";//dbcj1
	S_WORD DA = 0x0000;
	S_DWORD DI = 0xE3020001;
	S_CHAR RelayType = 0x00;
	S_BYTE MessageLen = 0x00;

	strIpcData.append((char*)&DA,2);
	strIpcData.append((char*)&DI,4);
	strIpcData.push_back(0x00);											//�м�����
	strIpcData.push_back(0x00);											//�˿ں�
	strIpcData.push_back(0x08);											//������08=2400/300
	strIpcData.push_back(0x01);											//У�鷽ʽ
	strIpcData.push_back(0x08);											//����λ
	strIpcData.push_back(0x00);											//ֹͣλ
	strIpcData.push_back(0x05);											//��ʱʱ��
	strIpcData.push_back((char)SendstrFrame.size());					//�м̱��ĳ���(645���ĳ���)
	strIpcData.append(SendstrFrame);

	logwt<<"RFTX RequestRftxMsg2Dbcj SendstrFrame = "<<HEX<<SendstrFrame<<endl;
	logwt<<"RFTX RequestRftxMsg2Dbcj strIpcData = "<<HEX<<strIpcData<<endl;

	if (C_SysMsg::Send2Server(pPrecess, MSAFN_DATA_ROUTE, strIpcData) != 0)
		return false;
	strIpcData.clear();
	if (C_SysMsg::Wait4Server(MSAFN_DATA_ROUTE, strIpcData) != 0)
		return false;
	if (strIpcData.size() < 6)
		return false;

	RelayType = strIpcData.at(0);
	MessageLen = (S_BYTE)strIpcData.at(1);	
	strResponse.append(strIpcData.begin()+2,strIpcData.end());

	logwt<<"RFTX RequestRftxMsg2Dbcj after strIpcData = "<<HEX<<strIpcData<<endl;

	return true;
}

/** @brief ����������[����������ı���]
 *  @param[in] strCmd ����
 *  @param[in] strAddress ��Ƶ�ַ
 *  @param[in] DI ͨ�üĴ���
 *  @param[out] strData �����������
 *  @return ���ݳ���
 */
int C_MeterFrameMgr::Parse645Frame(S_DWORD &CmdDI,string &MeterAddr, string &strData,string &ReceiveFrame)
{
	string strMyCmd;
	if (Check645FrameFormat(ReceiveFrame, strMyCmd) <= 0)
		return 0;

	if (MeterAddr.empty() == false)
	{
		if (strMyCmd.compare(1, 6, MeterAddr) != 0)
			return 0;
	}
	else
	{
		MeterAddr.append(strMyCmd.begin()+1, strMyCmd.begin()+1+6);
	}

	string::iterator iter = ReceiveFrame.begin();
	advance(iter, 8);	//compare ctrl code
	S_BYTE Ctrl = (S_BYTE)*iter; advance(iter, 1);
	S_BYTE Len = (S_BYTE)*iter; advance(iter, 1);
	if (Len == 0 || Ctrl != 0x83)
		return 0;
	if(Ctrl == 0xC3)
	{
		return -1;//�쳣֡
	}

	string strMyData(iter, iter+Len);
	if (strMyData.size() <= 4)
		return 0;

	string strDiData(strMyData.begin(), strMyData.end());
	transform(strMyData.begin(), strMyData.end(), strDiData.begin(), C_Sub33());
	if (CmdDI != 0x00)
	{
		if (strDiData.compare(0, 4, (char *)&CmdDI, 4) != 0)
			return 0;
	}
	else
	{
		copy(strDiData.begin(), strDiData.begin()+4, (char *)&CmdDI);
	}
	strData.append(strDiData.begin() + 4, strDiData.end());
	return (int)(strMyData.size() - 4);
}

/** @brief ��ȡ����֡
 *  @param[in] strBuffer �����
 *  @param[out] strCmd ֡�������
 *  @return <0: ����[����ֵ�ǳ�����ֽ���]; =0: ��֡; >0: �ɹ�
 */
int C_MeterFrameMgr::Check645FrameFormat(string &ReceiveFrame, string &RevFrame)
{
	if (ReceiveFrame.empty()) return 0;
	string::iterator iter = find(ReceiveFrame.begin(), ReceiveFrame.end(), 0x68);
	if (iter != ReceiveFrame.begin())
	{
		int diff = (int)distance(ReceiveFrame.begin(), iter);
		return (-diff);
	}
	if (distance(iter, ReceiveFrame.end()) < 12)
		return 0;
	S_BYTE Start1 = (S_BYTE)*iter; advance(iter, 1+6);
	S_BYTE Start2 = (S_BYTE)*iter; advance(iter, 1+1);
	if (Start1 != 0x68 || Start2 != 0x68)
	{
		logwt<<"��ƹ�Լ�յ��쳣֡��ʼ��,Start1="<<HEX<<Start1<<",Start2="<<Start2<<endl;
		return -1;
	}
	
	S_BYTE Len = (S_BYTE)*iter;

	if ((int)ReceiveFrame.size() < 12 + Len)
		return 0;
	advance(iter, 1 + Len);
	S_BYTE RealCS = (S_BYTE)accumulate(ReceiveFrame.begin(), iter, 0);
	S_BYTE StoreCS = (S_BYTE)*iter; advance(iter, 1);
	if (StoreCS != RealCS)
	{
		logwt<<"��ƹ�ԼУ��ʧ��,����CS["<<HEX<<RealCS<<"],ԴCS["<<StoreCS<<"]!"<<endl;
		return -1;
	}
	S_BYTE Stop = (S_BYTE)*iter; advance(iter, 1);
	if (Stop != 0x16)
	{
		logwt<<"��ƹ�Լ֡����������["<<HEX<<Stop<<"]!"<<endl;
		return -1;
	}

	RevFrame.append(ReceiveFrame.begin(), iter);
	return (int)distance(ReceiveFrame.begin(), iter);
}

/** @brief ��ȡ������
*  @param[in] none
*  @return ������
*/
string C_MeterFrameMgr::GetOperatorCode()
{
	string OperatorCode;
	OperatorCode.push_back(0x00);
	OperatorCode.push_back(0x00);
	OperatorCode.push_back(0x00);
	OperatorCode.push_back(0x00);

	string StrFrameOperatorCode(OperatorCode);
	transform(OperatorCode.begin(), OperatorCode.end(), StrFrameOperatorCode.begin(), C_Add33());

	return StrFrameOperatorCode;
}

/** @brief ��ȡ���ַ
*  @param[in] none
*  @return ���ַ
*/
string C_MeterFrameMgr::GetMeterAddr()
{
	string MeterAddr;
	S_CHAR cMeterAddr[6] = {0x11,0x11,0x11,0x11,0x11,0x11};
	MeterAddr.append(cMeterAddr,sizeof(cMeterAddr));

	return MeterAddr;
}

/** @brief ��֡����ͷ
*  @param[in] strCmd ����
*  @return ����ͷ
*/
string C_MeterFrameMgr::MakeFrameHead(const S_DWORD &CmdDI)
{
	string FrameHead;
	string MetterAddr = GetMeterAddr();
	FrameHead.push_back(0x68);
	FrameHead.append(MetterAddr.begin(),MetterAddr.end());
	FrameHead.push_back(0x68);
	S_CHAR CtrlCode = 0x03;
	S_CHAR Length = 0;
	switch(CmdDI)
	{
	case METER_INDETIFY:
		Length = 0x20;
		break;
	case METER_GET_MODE:
		CtrlCode = 0x11;
		Length = 0x04;
		break;
	case METER_ACCOUNT:
		Length = 0x1E;
		break;
// 	case METER_REMOTE_ACCOUNT:
// 		Length = 0x1E;
// 		break;
	case METER_WALLET_CHARGE:
		Length = 0x1E;
		break;
	case METER_WALLET_INIT:
		Length = 0x1E;
		break;
// 	case METER_UPDATE_INFO:
// 		Length = 0x1E;
// 		break;
// 	case METER_REMOTE_CTRL:
// 		Length = 0x1E;
// 		break;
	case METER_GET_BACK_INFO:
		Length = 0x10;
		break;
	default:
		break;
	}

	FrameHead.push_back(CtrlCode);
	FrameHead.push_back(Length);

	return FrameHead;
}

/** @brief ��֡����DI
*  @param[in] strCmd ����
*  @return ����DI
*/
string C_MeterFrameMgr::MakeFrameDI(const S_DWORD& CmdDI)
{
	string StrDI((char*)&CmdDI, 4);
	string StrFrameDI(StrDI);
	transform(StrDI.begin(), StrDI.end(), StrFrameDI.begin(), C_Add33());

	return StrFrameDI;
}

/** @brief ��֡��������
*  @param[in] strCmd ����
*  @return ��������
*/
string C_MeterFrameMgr::MakeFrameData(const S_DWORD &CmdDI)
{
	string StrData;
	switch(CmdDI)
	{
	case METER_INDETIFY:
		StrData.append(m_CardDstData,sizeof(m_CardDstData));
		StrData.append(m_CardRandData,sizeof(m_CardRandData));
		StrData.append(m_CardMeterNo,sizeof(m_CardMeterNo));
		logvt<<"��Կ�������ɢ����:"<<hex<<StrData<<endl;
		break;
	case METER_GET_MODE:
		logwt<<"���޲���!"<<endl;
		break;
	case METER_ACCOUNT:
		{
			S_BYTE ChargeValue[4] = {0};
			S_BYTE ChargeTimes[4] = {0};

			IPCSHAREMEM.GetRechargeVal(ChargeValue,sizeof(ChargeValue));
			reverse(ChargeValue,ChargeValue+4);
			StrData.append((const char*)ChargeValue,sizeof(ChargeValue));

			IPCSHAREMEM.GetRechargeTimes(ChargeTimes,sizeof(ChargeTimes));
			reverse(ChargeTimes,ChargeTimes+4);
			StrData.append((const char*)ChargeTimes,sizeof(ChargeTimes));

			StrData.append(m_CardMAC1,sizeof(m_CardMAC1));
			StrData.append(m_CardCustomerNo,sizeof(m_CardCustomerNo));
			StrData.append(m_CardMAC2,sizeof(m_CardMAC2));
			logvt<<"��������:"<<hex<<StrData<<endl;
			break;
		}
// 	case METER_REMOTE_ACCOUNT:
// 		logwt<<"���޲���!"<<endl;
// 		break;
	case METER_WALLET_CHARGE:
		{
			S_BYTE ChargeValue[4] = {0};
			S_BYTE ChargeTimes[4] = {0};

			IPCSHAREMEM.GetRechargeVal(ChargeValue,sizeof(ChargeValue));
			reverse(ChargeValue,ChargeValue+4);
			StrData.append((const char*)ChargeValue,sizeof(ChargeValue));

			IPCSHAREMEM.GetRechargeTimes(ChargeTimes,sizeof(ChargeTimes));
			reverse(ChargeTimes,ChargeTimes+4);
			StrData.append((const char*)ChargeTimes,sizeof(ChargeTimes));

			StrData.append(m_CardMAC1,sizeof(m_CardMAC1));
			StrData.append(m_CardCustomerNo,sizeof(m_CardCustomerNo));
			StrData.append(m_CardMAC2,sizeof(m_CardMAC2));
			logvt<<"��ֵ����:"<<hex<<StrData<<endl;
			break;
		}
	case METER_WALLET_INIT:
		{
			S_BYTE ChargeValue[4] = {0};
			S_BYTE ChargeTimes[4] = {0};

			IPCSHAREMEM.GetRechargeVal(ChargeValue,sizeof(ChargeValue));
			reverse(ChargeValue,ChargeValue+4);
			StrData.append((const char*)ChargeValue,sizeof(ChargeValue));

			IPCSHAREMEM.GetRechargeTimes(ChargeTimes,sizeof(ChargeTimes));
			reverse(ChargeTimes,ChargeTimes+4);
			StrData.append((const char*)ChargeTimes,sizeof(ChargeTimes));

			StrData.append(m_CardMAC1,sizeof(m_CardMAC1));
			StrData.append(m_CardCustomerNo,sizeof(m_CardCustomerNo));
			StrData.append(m_CardMAC2,sizeof(m_CardMAC2));
			logvt<<"Ǯ����ʼ������:"<<hex<<StrData<<endl;
			break;
		}
// 	case METER_UPDATE_INFO:
// 		logwt<<"���޲���!"<<endl;
// 		break;
// 	case METER_REMOTE_CTRL:
// 		logwt<<"���޲���!"<<endl;
// 		break;
	case METER_GET_BACK_INFO:
		{
			S_CHAR DataBack[8] = {0x32,0x00,0x00,0x00,0x06,0x00,0x01,0xdf}; // ���ݻس���ʶ �̶�
			StrData.append(DataBack,sizeof(DataBack));
			logvt<<"���ݻس���ʶ:"<<hex<<StrData<<endl;
			break;
		}
	default:
		break;
	}


	string StrFrameData(StrData);
	transform(StrData.begin(), StrData.end(), StrFrameData.begin(), C_Add33());

	return StrFrameData;
}

/** @brief ��֡����β
*  @param[out] TempFrame ��������
*  @return none
*/
void C_MeterFrameMgr::MakeFrameEnd(string& TempFrame)
{
	int CheckSum = accumulate(TempFrame.begin(), TempFrame.end(), 0);
	TempFrame.push_back((S_CHAR)CheckSum);								// CS
	TempFrame.push_back(0x16);											// end
}

/** @brief �ж�dbcj�ظ����ĵ���ȷ��
*  @param[in] strCmd ����
*  @param[out] StrResponse �ظ��ı���
*  @return ���ر��Ŀ�ʼ������
*/
S_DWORD C_MeterFrameMgr::JudgeDbcjResult(const S_DWORD& CmdDI,string &StrResponse,bool& Flag)
{													// ���ȫ��֤�Ƿ�ɹ�
	S_DWORD Index = 0;

	if (METER_GET_MODE == CmdDI){
		for (Index = 0;Index < StrResponse.size();Index++){
			if (StrResponse.at(Index) == 0x68 && Index < StrResponse.size() - 1){
				if (StrResponse.at(Index + 1) == 0x91){
					Flag = true;
					break;
				}
			}
		}
	}
	else
	{
		for (Index = 0;Index < StrResponse.size();Index++){
			if (StrResponse.at(Index) == 0x68 && Index < StrResponse.size() - 1){
				if (StrResponse.at(Index + 1) == 0x83){
					Flag = true;
					break;
				}
			}
		}
		
		if (!Flag)
		{
			for (Index = 0;Index < StrResponse.size();Index++){
				if (StrResponse.at(Index) == 0x68 && Index < StrResponse.size() - 1){
					if (StrResponse.at(Index + 1) == 0xC3){
						break;
					}
				}
			}
			string::iterator iter = StrResponse.begin() + Index + 3;
			string StrErrCode(iter,iter + 2);
			transform(StrResponse.begin() + Index + 3, StrResponse.begin() + Index + 5, StrErrCode.begin(), C_Sub33());
			
			S_WORD Error = ((S_WORD)StrErrCode.at(1)) << 8 | StrErrCode.at(0);
			AnalysisErrorCode(Error);
		}

	}

	return Index;
}

/** @brief ������ȫ��֤ʱ�Ĵ�����Ϣ
*  @param[in] Error SEER
*  @return 
*/
int C_MeterFrameMgr::AnalysisErrorCode(const S_WORD &error)
{
	logwt<<"���ȫ��֤���󣬴�������Ϊ:  ";
	switch(error)
	{
	case 0x0001:
		logwt<<"��������!"<<endl;
		break;
	case 0x0002:
		logwt<<"�ظ���ֵ!"<<endl;
		break;
	case 0x0004:
		logwt<<"ESAM��֤ʧ��!"<<endl;
		break;
	case 0x0008:
		logwt<<"�����֤ʧ��!"<<endl;
		break;
	case 0x0010:
		logwt<<"�ͻ���Ų�ƥ��!"<<endl;
		break;
	case 0x0020:
		logwt<<"��ֵ��������!"<<endl;
		break;
	case 0x0040:
		logwt<<"���糬�ڻ�!"<<endl;
		break;
	case 0x0080:
		logwt<<"��ַ�쳣!"<<endl;
		break;
	case 0x0100:
		logwt<<"������!"<<endl;
		break;
	case 0x0200:
		logwt<<"��բ����ʧ��!"<<endl;
		break;
	default:
		logwt<<"������������!"<<endl;
		break;
	}

	return 0;
}

/** @brief dbcj������ȷ���ݵ�����»�ȡ��Ӧ�Ľ������
*  @param[in] Index ���ر��Ŀ�ʼ������
*  @param[in] strCmd ����DI
*  @param[out] StrResponse �ظ��ı���
*  @return none
*/
void C_MeterFrameMgr::AnalysisDbcjResult(S_DWORD Index,const S_DWORD& CmdDI,string &StrResponse)
{
	string::iterator iter = StrResponse.begin() + Index + 7;
	
	switch(CmdDI)
	{
	case METER_INDETIFY:
		{
			string StrDiData(iter,iter + 12);
			transform(StrResponse.begin() + Index + 7, StrResponse.begin() + Index + 19, StrDiData.begin(), C_Sub33());

			memcpy(m_MeterRand,StrDiData.data(),sizeof(m_MeterRand)); 

			memcpy(m_MeterEsamRand,StrDiData.data()+ 4,sizeof(m_MeterEsamRand)); 

			logwt<<"��������֤�ɹ�!"<<endl;
			break;
		}
	case METER_GET_MODE:
		{
			string StrDiData(iter,iter + 2);
			transform(StrResponse.begin() + Index + 7, StrResponse.begin() + Index + 9, StrDiData.begin(), C_Sub33());

			m_MeterMode = StrResponse.at(0) & 0x40;

			string printstr;
			logvt<<"��ȡ���ģʽ�ɹ� m_MeterMode = " << hex << m_MeterMode <<endl;
			break;
		}
	case METER_ACCOUNT:
		logwt<<"������ɹ�!"<<endl;
		break;
// 	case METER_REMOTE_ACCOUNT:
// 		break;
	case METER_WALLET_CHARGE:
		logwt<<"����ֵ�ɹ�!"<<endl;
		break;
	case METER_WALLET_INIT:
		logwt<<"���Ǯ����ʼ���ɹ�!"<<endl;
		break;
// 	case METER_UPDATE_INFO:
// 		break;
// 	case METER_REMOTE_CTRL:
// 		break;
	case METER_GET_BACK_INFO:
		{
			logwt<<"����ȡ��д���ݳɹ�!"<<endl;

			string strDiData(iter,iter + 62);
			transform(StrResponse.begin() + Index + 7, StrResponse.begin() + Index + 69, strDiData.begin(), C_Sub33());

			memcpy(m_MeterBackInfo,strDiData.data()+8,sizeof(m_MeterBackInfo));

			string printstr;
			printstr.append(m_MeterBackInfo,sizeof(m_MeterBackInfo));
			logvt<<"m_MeterBackInfo:"<<hex<<printstr<<endl;

			memcpy(m_MeterMAC,strDiData.data() + 58,sizeof(m_MeterMAC)); 

			printstr.clear();
			printstr.append(m_MeterMAC,sizeof(m_MeterMAC));
			logvt<<"m_MeterMAC:"<<hex<<printstr<<endl;
			break;
		}
	default:
		break;
	}
}

