/** @file
 *  @brief 南网命令处理基类
 *  @author 
 *  @date 2011/12/16
 *  @version 0.1
 */
#ifndef __NWB_MSCMD_H__
#define __NWB_MSCMD_H__
#include "basedef.h"
#include "strings.h"
#include "bitsetex.h"
#include "basetype.h"

#define MSCMD_RESPONSE_INVALID_CHAR		0xFF
enum 
{
	MSAFN_CONFIRM_DENY = 0x00,//确认/否认
	MSAFN_LINK_CHECK = 0x02,
	MSAFN_WRITE_PARAM = 0x04,
	MSAFN_AUTHENTICATE = 0x06,
	MSAFN_READ_PARAM = 0x0A,
	MSAFN_READ_REALTIME = 0x0C,
	MSAFN_READ_HISTORY = 0x0D,
	MSAFN_READ_EVENT = 0x0E,
	MSAFN_FILE_TRANSFER = 0x0F,
	MSAFN_DATA_ROUTE = 0x10,
	MSAFN_READ_TASK = 0x12,
	MSAFN_READ_ALARM = 0x13,
	MSAFN_FORWARD = 0x14,
	MSAFN_USER_DEFINE = 0x15,

//级联主终端命令
	MSAFN_FORWARD_MS = 0x2B,

//user define
	MSAFN_USER_DEFINE_BEGIN = 0x80
};

enum
{
	FRAME_PROCESS_STOP, // 分析中止
	FRAME_PROCESS_CONTINUE, // 分析继续

	FRAME_PROCESS_CONFIRM, // 收到确认
	FRAME_PROCESS_DENY, // 收到否认

	FRAME_PROCESS_RESPONSE_CONFIRM, // 全部确认
	FRAME_PROCESS_RESPONSE_DENY, // 全部否认
	FRAME_PROCESS_RESPONSE_ITEM, // 逐项确信
	FRAME_PROCESS_RESPONSE_DATA, // 数据响应

	FRAME_PROCESS_END,
	PARSE_SLAVE_BEGIN,

	PARSE_SLAVE_UNKNOW,
	PARSE_SLAVE_REPORT_YES,
	PARSE_SLAVE_REPORT_NO,
	PARSE_SLAVE_GATHER_ENERGY,
	PARSE_SLAVE_FORWARD_MS,

	PARSE_SLAVE_END
};

//
//主站命令响应（采用命令模式）
//
struct S_CmdObj
{
	INT16U m_DA;
	INT32U m_DI;
	string m_DiInfo;
	string m_TimeInfo;
};

struct S_CmdObjs:public vector<S_CmdObj>
{
};

//C_CmdReceiver为轻量级对象
class C_CmdReceiver
{
public:
	C_CmdReceiver(void){m_PackageLength = 400;}
	void SetPackageLength(S_WORD PackageLength){m_PackageLength = PackageLength;}

	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj){return true;}
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse){return false;}
	virtual char* Prompt(void){return NULL;}

	int AppendDADT(string &strInfo, const S_CmdObj &CmdObj);

protected:
	bool ExtractCmdInfo(S_WORD size, string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);

protected:
	S_WORD m_PackageLength;
};

//C_AFNCMD为轻量级对象
class C_AFNCMD
{
public:
	C_AFNCMD(void){m_MSA = 0x00, m_PackageLength = 400;}
	void SetCmdInfo(S_BYTE MSA, S_WORD PackageLength);

	virtual bool NeedPwdCheck(void){return false;}
	virtual bool Authenticate(const string &strCmd, const string &strPWD);
	virtual int ParseCmdObj(const string &strCmdInfo, S_CmdObjs &CmdObjs);
	virtual int Execute(const S_CmdObjs &CmdObjs, strings &strResponseInfos);

protected:
	virtual C_CmdReceiver *GetCmdReceiver(INT32U DI){return NULL;}

protected:
	S_BYTE m_MSA;
	S_WORD m_PackageLength;
};

//
//命令对象参数
//
struct S_CmdObjPara
{//命令对象参数数组结构
	INT32U m_DI;
	C_CmdReceiver *m_pCmdReceiver;
};
struct S_CmdObjPara3D
{//命令对象参数数组结构
	INT32U m_diBegin;
	INT32U m_diEnd;
	C_CmdReceiver *m_pCmdReceiver;
};
inline bool operator<(const S_CmdObjPara &left, const S_CmdObjPara &right){return left.m_DI < right.m_DI;}
inline bool operator<(const S_CmdObjPara3D &left, const S_CmdObjPara3D &right){return left.m_diBegin < right.m_diBegin;}


#endif //__NWB_MSCMD_H__

