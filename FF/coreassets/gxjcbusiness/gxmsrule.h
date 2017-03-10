/** @file
 *  @brief 国网规约解析
 *  @author 
 *  @date 2012/05/04
 *  @version 0.1
 */
#ifndef __GXJCB_MS_RULE_H__
#define __GXJCB_MS_RULE_H__
#include "basedef.h"
#include "gxbtype.h"
#include "timeop.h"
#include "strings.h"
#include "gxmscmd.h"

#define GXJC_PROTOCOL_FLAG		1

class C_CtrlCode
{
public:
	C_CtrlCode(void){m_CtrlCode = 0x00; m_EC1 = 0x00; m_EC2 = 0x00;}
	C_CtrlCode(INT8U code){m_CtrlCode = code; m_EC1 = 0x00; m_EC2 = 0x00;}
	~C_CtrlCode(){}

public:
	INT8U GetAll(void){return m_CtrlCode;}
	INT8U GetDIR(void){return (m_CtrlCode>>7)&0x01;}
	INT8U GetPRM(void){return (m_CtrlCode>>6)&0x01;}
	INT8U GetFCB(void){return (m_CtrlCode>>5)&0x01;}
	INT8U GetFCV(void){return (m_CtrlCode>>4)&0x01;}
	INT8U GetFUN(void){return (m_CtrlCode>>0)&0x0F;}
	INT8U GetACD(void){return (m_CtrlCode>>5)&0x01;}
	INT8U GetEC1(void){return m_EC1;}
	INT8U GetEC2(void){return m_EC2;}
	INT8U GetPrimaryFun(INT8U AFN);
	INT8U GetSlaveFun(INT8U PreAFN, INT8U NowAFN);

	void SetDIR(INT8U dir){m_CtrlCode = dir==0?(m_CtrlCode&0x7F):(m_CtrlCode|0x80);}
	void SetPRM(INT8U prm){m_CtrlCode = prm==0?(m_CtrlCode&0xBF):(m_CtrlCode|0x40);}
	void SetACD(INT8U acd){m_CtrlCode = acd==0?(m_CtrlCode&0xDF):(m_CtrlCode|0x20);}
	void SetFUN(INT8U fun){m_CtrlCode = (m_CtrlCode&0xF0)|(fun&0x0F);}
	void SetEC(S_BYTE EC1, S_BYTE EC2){m_EC1 = EC1; m_EC2 = EC2;}

private:
	INT8U m_CtrlCode;
	INT8U m_EC1;
	INT8U m_EC2;
};

class C_RTUA
{
public:
	C_RTUA(void){SetAreaCode(0),SetDevAddr(0),SetMSA(0),SetGroup(0);}
	C_RTUA(S_DWORD rtua){SetRTUA(rtua),SetMSA(0),SetGroup(0);}
	C_RTUA(S_WORD AreaCode, S_WORD DevAddr, S_BYTE MSA=0, S_BYTE Group=0){SetAreaCode(AreaCode),SetDevAddr(DevAddr),SetMSA(MSA),SetGroup(Group);}

	S_DWORD GetRTUA(void)const{return KGE_COMBDWORD(m_DevAddr, m_AreaCode);}
	S_WORD GetAreaCode(void)const{return m_AreaCode;}
	S_WORD GetDevAddr(void)const{return m_DevAddr;}
	S_BYTE GetMSA(void)const{return m_MSA;}
	S_BYTE GetGroup(void)const{return m_Group;}

	void SetRTUA(S_DWORD rtua){m_AreaCode=KGE_GETLOWORD(rtua),m_DevAddr=KGE_GETHIWORD(rtua);}
	void SetAreaCode(S_WORD AreaCode){m_AreaCode = AreaCode;}
	void SetDevAddr(S_WORD DevAddr){m_DevAddr = DevAddr;}
	void SetMSA(S_BYTE MSA){m_MSA = MSA;}
	void SetGroup(S_BYTE Group){m_Group = Group;}

	bool operator== (C_RTUA &other)const{return m_AreaCode==other.m_AreaCode && m_DevAddr==other.m_DevAddr;}
	void ParseString(const string &strAddr);
	void PrintString(string &strAddr)const;
	void ParseAscii(const char *pRTUA);
	char *PrintAscii(char *pRTUA)const;

private:
	S_WORD m_AreaCode;///<行政区划码
	S_WORD m_DevAddr;///<装置地址
	S_BYTE m_MSA;///<主站地址
	S_BYTE m_Group; ///< 组标志
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
	C_FrameTP(void){m_timeFrameSend=0,m_IntervalM=0,m_PFC=0;}

	void ParseString(const string &strTP);
	void PrintString(string &strTP);
	bool Valid(void);

private:
	S_TIME m_timeFrameSend;
	INT8U m_IntervalM;
	INT8U m_PFC;
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
	virtual INT8U GetAlarmReportFlag(S_BYTE &EC1, S_BYTE &EC2){return 0;}

	void SetPackageLengthMax(S_WORD length){m_PackageLenMax = length;}
	S_WORD GetPackageLength(void){return m_PackageLenMax;}
	void SetAddress(const S_DWORD *pTermAddr, const S_DWORD *pForwardAddr, const S_WORD *pGroupAddr);

	bool TrimCmdInfo(const strings &strSrcs, strings &strDsts);
	/** 从缓冲中提取命令，检查地址，分析命令*/
	int ExtractCmd(string &strRecvBuff, string &strCmd);
	int CheckDeviceAddr(const string &strCmd);
	int Parse(const string &strCmd, INT8U &AFN, string &strCmdInfo, INT8U dir);
	int Parse(const string &strCmd, INT8U &AFN, string &strCmdInfo){return Parse(strCmd, AFN, strCmdInfo, 0x00);}

	/** 组织命令帧 */
	int MakeLoginCmd(string &strLogin);
	int MakeHeartBeatCmd(string &strHB);
	int MakeConfirmCmd(bool fConfirm, string &strConfirm);

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

	int MakeForwardPoll(INT8U ForwardAddrID, string &strPollCmd){return MakeForwardPrimary(m_pForwardAddr[ForwardAddrID], 0x00, strPollCmd);}
//	int MakeForwardCtrl(INT8U ForwardAddrID, string &strPollCmd){return MakeForwardPrimary(m_pForwardAddr[ForwardAddrID], 0x00, strPollCmd);}
//	int MakeForwardGather(INT8U ForwardAddrID, string &strPollCmd){return MakeForwardPrimary(m_pForwardAddr[ForwardAddrID], 0x00, strPollCmd);}

	int ParseSlave(const string &strCmd, INT8U &AFN, string &strCmdInfo){return Parse(strCmd, AFN, strCmdInfo, 0x01);}

protected:
	S_BYTE m_AFN;
	S_WORD m_PackageLenMax;
	const S_DWORD *m_pTermAddr;
	const S_DWORD *m_pForwardAddr;
	const S_WORD *m_pGroupAddr;

	C_RTUA m_FrameAddr;
	C_CtrlCode m_FrameCtrl;
	C_FrameSeq m_FrameSeq;
	C_FrameTP m_FrameTP;
	C_FrameSeq m_PrmFrameSeq;
};


#endif//__GXJCB_MS_RULE_H__



