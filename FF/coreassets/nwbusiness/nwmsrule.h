/** @file
 *  @brief 南网规约解析
 *  @author 
 *  @date 2011/12/16
 *  @version 0.1
 */

#ifndef __NW_MS_RULE_H__
#define __NW_MS_RULE_H__
#include "basedef.h"
#include "timeop.h"
#include "strings.h"
#include "nwbtype.h"
#include "nwmscmd.h"

class C_CtrlCode
{
public:
	C_CtrlCode(void){m_CtrlCode = 0x00;}
	C_CtrlCode(INT8U code){m_CtrlCode = code;}
	~C_CtrlCode(){}

public:
	INT8U GetAll(void){return m_CtrlCode;}
	INT8U GetDIR(void){return (m_CtrlCode>>7)&0x01;}
	INT8U GetPRM(void){return (m_CtrlCode>>6)&0x01;}
	INT8U GetFCB(void){return (m_CtrlCode>>5)&0x01;}
	INT8U GetFCV(void){return (m_CtrlCode>>4)&0x01;}
	INT8U GetFUN(void){return (m_CtrlCode>>0)&0x0F;}
	INT8U GetPrimaryFun(INT8U AFN);
	INT8U GetSlaveFun(INT8U PreAFN, INT8U NowAFN);

	void SetDIR(INT8U dir){m_CtrlCode = dir==0?(m_CtrlCode&0x7F):(m_CtrlCode|0x80);}
	void SetPRM(INT8U prm){m_CtrlCode = prm==0?(m_CtrlCode&0xBF):(m_CtrlCode|0x40);}
	void SetACD(INT8U acd){m_CtrlCode = acd==0?(m_CtrlCode&0xDF):(m_CtrlCode|0x20);}
	void SetFUN(INT8U fun){m_CtrlCode = (m_CtrlCode&0xF0)|(fun&0x0F);}

private:
	INT8U m_CtrlCode;
};

class C_RTUA
{
public:
	C_RTUA(void){SetAreaCode(0),SetDevAddr(0),SetMSA(0);}
	C_RTUA(const S_RTUA &rtua){SetRTUA(rtua),m_MSA=0;}
	C_RTUA(S_DWORD AreaCode, S_DWORD DevAddr, S_BYTE MSA=0){SetAreaCode(AreaCode),SetDevAddr(DevAddr),SetMSA(MSA);}

	S_RTUA GetRTUA(void)const{S_RTUA rtua; rtua.m_AreaCode=m_AreaCode, rtua.m_Address=m_DevAddr; return rtua;}
	S_DWORD GetAreaCode(void)const{return m_AreaCode;}
	S_DWORD GetDevAddr(void)const{return m_DevAddr;}
	S_BYTE GetMSA(void)const{return m_MSA;}

	void SetRTUA(const S_RTUA &rtua){m_AreaCode=rtua.m_AreaCode,m_DevAddr=rtua.m_Address;}
	void SetAreaCode(S_DWORD AreaCode){m_AreaCode = AreaCode;}
	void SetDevAddr(S_DWORD DevAddr){m_DevAddr = DevAddr;}
	void SetMSA(S_BYTE MSA){m_MSA = MSA;}

	bool operator== (C_RTUA &other)const{return m_AreaCode==other.m_AreaCode && m_DevAddr==other.m_DevAddr;}
	void ParseString(const string &strAddr);
	void PrintString(string &strAddr)const;
	void ParseAscii(const char *pRTUA);
	char *PrintAscii(char *pRTUA)const;

private:
	S_DWORD m_AreaCode;///<行政区划码
	S_DWORD m_DevAddr;///<装置地址
	S_BYTE m_MSA;///<主站地址
};

class C_FrameSeq
{
public:
	C_FrameSeq(void){m_SEQ=0x00;}
	C_FrameSeq(INT8U seq){m_SEQ=seq;}

public:
	INT8U GetAll(void){return m_SEQ;}
	INT8U GetTpV(void){return (m_SEQ>>7)&0x01;}
	INT8U GetFIR(void){return (m_SEQ>>6)&0x01;}
	INT8U GetFIN(void){return (m_SEQ>>5)&0x01;}
	INT8U GetCON(void){return (m_SEQ>>4)&0x01;}
	INT8U GetRSEQ(void){return (m_SEQ>>0)&0x0F;}

	void SetTpV(INT8U tpv){m_SEQ = tpv==0?(m_SEQ&0x7F):(m_SEQ|0x80);}
	void SetFIR(INT8U fir){m_SEQ = fir==0?(m_SEQ&0xBF):(m_SEQ|0x40);}
	void SetFIN(INT8U fin){m_SEQ = fin==0?(m_SEQ&0xDF):(m_SEQ|0x20);}
	void SetCON(INT8U con){m_SEQ = con==0?(m_SEQ&0xEF):(m_SEQ|0x10);}
	void SetRSEQ(INT8U seq){m_SEQ = (m_SEQ&0xF0)|(seq&0x0F);}

private:
	INT8U m_SEQ;
};

class C_FrameTP
{
public:
	C_FrameTP(void){m_timeFrameSend=0,m_IntervalM=0;}

	void ParseString(const string &strTP);
	void PrintString(string &strTP);
	bool Valid(void);

private:
	S_TIME m_timeFrameSend;
	INT8U m_IntervalM;
};

class C_MSRULE
{
public:
	C_MSRULE(void){m_AFN = MSAFN_CONFIRM_DENY;m_PackageLenMax = 512;SetAddress(NULL,NULL,NULL);}
	virtual ~C_MSRULE(){}

public:
	/** @brief 取AFN命令
	 *  @remarks 应用程序必须重载
	 */
	virtual C_AFNCMD *GetAfnCmd(S_BYTE AFN);
	/** @取告警标志
	 *  @return 1 有告警未上报;0 无告警未上报
	 */
	virtual INT8U GetAlarmReportFlag(void){return 0;}

	void SetPackageLengthMax(S_WORD length){m_PackageLenMax = length;}
	S_WORD GetPackageLength(void){return m_PackageLenMax;}
	S_BYTE GetMSA(void){return m_FrameAddr.GetMSA();}
	void SetAddress(const S_RTUA *pTermAddr, const S_DWORD *pForwardAddr, const S_DWORD *pGatherAddr);

	bool TrimCmdInfo(const strings &strSrcs, strings &strDsts);
	/** 从缓冲中提取命令，检查地址，分析命令*/
	int ExtractCmd(string &strRecvBuff, string &strCmd);
	int CheckDeviceAddr(const string &strCmd);
	int Parse(const string &strCmd, INT8U &AFN, string &strCmdInfo, INT8U dir);
	int Parse(const string &strCmd, INT8U &AFN, string &strCmdInfo){return Parse(strCmd, AFN, strCmdInfo, 0x00);}

	bool CheckPrivateAddress(const C_RTUA &FrameAddr);

	/** 组织命令帧 */
	int MakeLoginCmd(string &strLogin);
	int MakeHeartBeatCmd(string &strHB);
	int MakeConfirmCmd(bool fConfirm, string &strConfirm);
	int MakeConfirmCmd(const strings &strItems, string &strConfirm);

	int Make(INT8U AFN, C_CtrlCode &CtrlCode, C_RTUA &FrameAddr, C_FrameSeq &FrameSeq, C_FrameTP &FrameTP, string &strCmdInfo, string &strCmd);
	int MakePrimary(INT8U AFN, C_FrameSeq &FrameSeq, C_FrameTP &FrameTP, string &strCmdInfo, string &strCmd);
	int MakeSlave(INT8U AFN, C_FrameSeq &FrameSeq, C_FrameTP &FrameTP, string &strCmdInfo, string &strCmd);
	int MakeResponseCmd(INT8U AFN, const strings &strCmdInfos, strings &strResponseCmds);
	int MakeReportCmd(INT8U AFN, const strings &strCmdInfos, strings &strReportCmds);
	int MakeResponseCmd(const strings &strCmdInfos, strings &strResponseCmds)
	{
		return MakeResponseCmd(m_AFN, strCmdInfos, strResponseCmds);
	}

	/* 级联 */
	int MakeForwardPrimary(INT32U ForwardAddr, string &strCmdInfo, string &strCmd);
	int MakeForwardPrimary(INT32U ForwardAddr, INT32U DI, string &strCmd);
	int MakeForwardSlave(void);

	int MakeForwardPoll(INT8U ForwardAddrID, string &strPollCmd){return MakeForwardPrimary(m_pForwardAddr[ForwardAddrID], 0xE0000930, strPollCmd);}
	int MakeForwardCtrl(INT8U ForwardAddrID, string &strPollCmd){return MakeForwardPrimary(m_pForwardAddr[ForwardAddrID], 0xE0000931, strPollCmd);}
	int MakeForwardGather(INT8U ForwardAddrID, string &strPollCmd){return MakeForwardPrimary(m_pForwardAddr[ForwardAddrID], 0xE0000933, strPollCmd);}

	int ParseSlave(const string &strCmd, INT8U &AFN, string &strCmdInfo){return Parse(strCmd, AFN, strCmdInfo, 0x01);}

protected:
	S_BYTE m_AFN;
	S_WORD m_PackageLenMax;
	const S_RTUA *m_pTermAddr;
	const S_DWORD *m_pForwardAddr;
	const S_DWORD *m_pGatherAddr;

	C_RTUA m_FrameAddr;
	C_CtrlCode m_FrameCtrl;
	C_FrameSeq m_FrameSeq;
	C_FrameTP m_FrameTP;
	C_FrameSeq m_PrmFrameSeq;
};


#endif//__NW_MS_RULE_H__



