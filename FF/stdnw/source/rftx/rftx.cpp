/** @file
 *  @brief 费控交互终端与射频模块通信协议
 *  @author 
 *  @date 2016/07/27
 *  @version 0.1
 */
#include "rftx.h"
#include "regconv.h"
#include "sysipc.h"
#include "htrace.h"
#include "status.h"
#include "nwmsrule.h"
#include "nwmscmd.h"
#include "context.h"
//#include "alarmEvent.h"
////#include "termcom.h"
#include "mrfact.h"
#include "almfact.h"
#include "fbfact.h"
#include "busialgo.h"

#include <numeric>
#include <math.h>
#include "strings.h"
#include <stdio.h>
#include "ipcSharemem.h"




static S_BYTE* strbuf = NULL;
static S_WORD  buflen = 0;

const char *hex2str(const S_BYTE* ab, S_WORD ab_len)
{
	if (!strbuf) {
		buflen = 1028;
		strbuf = (S_BYTE*)malloc(buflen);
	}
    return (const char *)hex_repr(ab, ab_len, strbuf, buflen);
}

S_BYTE *hex_repr( const S_BYTE *ab, S_WORD ab_len, S_BYTE *buf, S_WORD buf_len)
{
    S_WORD i, j;

    // two chars for hex digit and one for space + one for null
    if ((ab_len*3+1) < buf_len)
        // only print at max buf_len chars
        buf_len = ab_len*3+1;

    for (i = 0, j = 0; i < ab_len; i++, j++) {
        buf[j] = "0123456789ABCDEF"[(ab[i]>>4)&0xF];
        j++;
        buf[j] = "0123456789ABCDEF"[ab[i]&0xF];
        j++;
        buf[j] = ' ';
        if ( buf_len - j < 4)
            break;
    }
    buf[j] = '\0'; // string terminator

    return buf;
}

char bcdc(char i_cBcdData)
{
	char i;
	i=((i_cBcdData>>4)&0x0f)*10+(i_cBcdData&0x0f);
	return i;
}

float BCD62_to_Double(char *i_cpBcdData)
{
	float x=0.0,x1=0.0;

	x1=(float)bcdc(i_cpBcdData[3]);
	x=x+x1*10000;
	x1=(float)bcdc(i_cpBcdData[2]);
	x=x+x1*100;
	x1=(float)bcdc(i_cpBcdData[1]);
	x=x+x1;
	x1=(float)bcdc(i_cpBcdData[0]);
	x=x+x1/100;
	return x;
}

int C_RFtxServer::BeforeService(void)
{
	m_Status = RFTX_STATUS_INIT;
	m_RFIDMgr.InitComParam();

	IPCSHAREMEM.setTraceType(WALLET_CHARGE);
	return 0;
}

 C_RFtxServer::C_RFtxServer(void)
{
	m_Status = RFTX_STATUS_INIT;
	m_RFIDMgr.InitComParam();
}

C_RFtxServer::~C_RFtxServer(void)
{

}

int C_RFtxServer::AfterService(void)
{
	m_RFIDMgr.Disconnect();
	return 0;
}

int C_RFtxServer::OnTickService(void)
{
	switch (m_Status)
	{
	case RFTX_STATUS_INIT:
		if (InitRftxComm() == 0)
			m_Status = RFTX_STATUS_SYNC_STATUS;
		break;
	case RFTX_STATUS_SYNC_STATUS:
		if (SyncRftxStatus() == 0)
			m_Status = RFTX_STATUS_RUNNING;
		else
			m_Status = RFTX_STATUS_SYNC_STATUS;
		break;
	case RFTX_STATUS_RUNNING:
		if (RftxTaskRunning() == 0)
			m_Status = RFTX_STATUS_RUNNING;
		else
			m_Status = RFTX_STATUS_SYNC_STATUS;
		break;
	default:
		m_Status = RFTX_STATUS_INIT;
		break;
	}
	return 0;
}

int C_RFtxServer::InitRftxComm(void)
{
	return m_RFIDMgr.InitRFIDComm();
}

int C_RFtxServer::SyncRftxStatus(void)
{
	//判断射频模块是否有刷卡动作
	int ret = -1;
	do
	{		
		ret = m_RFIDMgr.ReadInfoFromCard(READ_CARD_INFO);	
		if(ret == 0){
			printf("RFTX SyncRftxStatus Successful ret = %d",ret);
			ret = -1;
			if (0 == m_RFIDMgr.ReadInfoFromCard(IDENTIFY_AUTH)){
				S_CHAR CardDstData[8] = {0};
				m_RFIDMgr.GetDstData(CardDstData,sizeof(CardDstData));
				reverse(CardDstData,CardDstData + 8);
				m_MeterFrameMgr.SetCardDstData(CardDstData);

				S_CHAR CardRandData[8] = {0};
				m_RFIDMgr.GetEsamRandData(CardRandData,sizeof(CardRandData));
				reverse(CardRandData,CardRandData + 8);
				m_MeterFrameMgr.SetCardRandData(CardRandData);

				S_CHAR CardMeterNo[8] = {0};
				m_RFIDMgr.GetMeterNo(CardMeterNo,sizeof(CardMeterNo));
				reverse(CardMeterNo,CardMeterNo + 8);
				m_MeterFrameMgr.SetCardMeterNo(CardMeterNo);

				if (m_MeterFrameMgr.RequestDbcjMsg(METER_INDETIFY)){
					S_CHAR MeterRand[4] = {0};
					m_MeterFrameMgr.GetMeterRand(MeterRand,sizeof(MeterRand));
					reverse(MeterRand,MeterRand + 4);
					m_RFIDMgr.SetMeterRand(MeterRand);

					if (m_MeterFrameMgr.RequestDbcjMsg(METER_GET_MODE)){
						m_MeterMode = m_MeterFrameMgr.GetMeterMode();
#if 1
						if (0 == m_RFIDMgr.ReadInfoFromCard(START_CARD_HANDEL)){
							S_CHAR CardMAC1[4] = {0};
							m_RFIDMgr.GetMAC1(CardMAC1,sizeof(CardMAC1));
							reverse(CardMAC1,CardMAC1 + 4);
							m_MeterFrameMgr.SetCardMac1(CardMAC1);

							S_CHAR CardCustomerNo[6] = {0};
							m_RFIDMgr.GetCustomerNo(CardCustomerNo,sizeof(CardCustomerNo));
							reverse(CardCustomerNo,CardCustomerNo + 6);
							m_MeterFrameMgr.SetCardCustomerNo(CardCustomerNo);

							S_CHAR CardMAC2[4] = {0};
							m_RFIDMgr.GetMAC2(CardMAC2,sizeof(CardMAC2));
							reverse(CardMAC2,CardMAC2 + 4);
							m_MeterFrameMgr.SetCardMac2(CardMAC2);

							ret = 0;
							break;
						}
#else
						if (0 == m_RFIDMgr.ReadInfoFromCard(PARAM_SET)){
							ret = 0;
							break;
						}
#endif
					}
				}
			}
		}	
		else{
			printf("%s SyncRftxStatus \n","RFTX SyncRftxStatus fail");
			break;
		}
	}while(0);
	
	return ret;
}

int C_RFtxServer::RftxTaskRunning(void)
{
	int ret = -1;
	//执行相关任务(与射频卡进行数据交互)

	printf("%s RftxTaskRunning \n","RFTX RftxTaskRunning Successful");
	
	switch(IPCSHAREMEM.getTraceType())
	{
	case NONE_TRACE:
		break;
	case LONG_DISTANCE_ACCOUNT:
		ret = LongDistanceAccount();
		break;
	case LOCAL_ACCOUNT:
		ret = LoaclAccount();
		break;
	case WALLET_CHARGE:
		ret = WalletCharge();
		break;
	case WALLET_INIT:
		ret = WalletInit();
		break;
	case PARAM_UPDATE:
		ret = ParamUpdate();
		break;
	case LONG_DISTANCE_CONTROL:
		ret = LongDistanceControl();
		break;
	default:
		break;
	}

	return ret;
}

int C_RFtxServer::LongDistanceAccount()
{
	IPCSHAREMEM.setTraceType(NONE_TRACE);
	return 0;
}

int C_RFtxServer::LoaclAccount()
{
	int result = -1;

	if (m_MeterFrameMgr.RequestDbcjMsg(METER_ACCOUNT)){
		if (0 == BackInfoProcess()){
			result = 0;
		}
	}

	return result;
}

int C_RFtxServer::WalletCharge()
{
	int result = -1;

	if (m_MeterFrameMgr.RequestDbcjMsg(METER_WALLET_CHARGE)){
		if (0 == BackInfoProcess()){
			result = 0;
		}
	}

	return result;
}

int C_RFtxServer::WalletInit()
{
	int result = -1;

	if (m_MeterFrameMgr.RequestDbcjMsg(METER_WALLET_INIT)){
		if (0 == BackInfoProcess()){
			result = 0;
		}
	}

	return result;
}

int C_RFtxServer::ParamUpdate()
{
	IPCSHAREMEM.setTraceType(NONE_TRACE);
	return 1;
}

int C_RFtxServer::LongDistanceControl()
{
	IPCSHAREMEM.setTraceType(NONE_TRACE);
	return 1;
}

int C_RFtxServer::BackInfoProcess()
{
	int result = -1;
	if (m_MeterFrameMgr.RequestDbcjMsg(METER_GET_BACK_INFO)){
		S_CHAR MeterMAC[4] = {0};
		m_MeterFrameMgr.GetMeterMac(MeterMAC,sizeof(MeterMAC));
		reverse(MeterMAC,MeterMAC + 4);
		m_RFIDMgr.SetMac(MeterMAC);

		S_CHAR MeterBackInfo[50] = {0};
		m_MeterFrameMgr.GetMeterBackInfo(MeterBackInfo,sizeof(MeterBackInfo));
		reverse(MeterBackInfo,MeterBackInfo + 50);
		m_RFIDMgr.SetBackInfo(MeterBackInfo);
		if (0 == m_RFIDMgr.ReadInfoFromCard(END_CARD_HANDEL)){
			IPCSHAREMEM.setTraceType(NONE_TRACE);
			IPCSHAREMEM.setTraceResult(true);
			result = 0;
		}
	}

	return result;
}





