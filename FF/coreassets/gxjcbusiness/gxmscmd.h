/** @file
 *  @brief 广西集抄命令处理基类
 *  @author 
 *  @date 2012/05/04
 *  @version 0.1
 */
#ifndef __GXJCB_MSCMD_H__
#define __GXJCB_MSCMD_H__
#include "basedef.h"
#include "strings.h"
#include "bitsetex.h"
#include "basetype.h"

//
//
//
enum
{
	GW_CLASS2_UNKNOW,
	GW_CLASS2_VIP,
	GW_CLASS2_DAY,
	GW_CLASS2_MON,
	GW_CLASS2_GTH,

	GW_CLASS2PN_UNKNOW,
	GW_CLASS2PN_MPOINT,
	GW_CLASS2PN_TGROUP,
	GW_CLASS2PN_PORT,
	GW_CLASS2PN_TERM
};
class C_FnInfo
{
public:
	/** @brief 获取二类数据FN类型 
	 *  @param[in] FN 国网要求二类数据FN
	 *  @return GW_CLASS2_UNKNOW, GW_CLASS2_VIP, GW_CLASS2_DAY, GW_CLASS2_MON, GW_CLASS2_GTH
	 */
	static int GetType4Class2FN(S_WORD FN);

	/** @brief  获取FN对应的PN意义
	 *  @param[in] FN 国网要求二类数据FN
	 *  @return GW_CLASS2PN_UNKNOW, GW_CLASS2PN_MPOINT, GW_CLASS2PN_TGROUP, GW_CLASS2PN_PORT, GW_CLASS2PN_TERM
	 */
	static int GetPnMean4Class2Fn(S_WORD FN);
};

//
//
//
#define MSCMD_RESPONSE_INVALID_CHAR		0xFF
enum 
{
	MSAFN_CONFIRM_DENY = 0x00,//确认/否认
	MSAFN_RESET = 0x01,//复位
	MSAFN_LINK_CHECK = 0x02,//链路接口检测
//	MSAFN_RELAY_CMD = 0x03,//中继站命令
	MSAFN_WRITE_PARAM_TERM = 0x04,//设置参数终端
//	MSAFN_CTRL_CMD = 0x05, //控制命令
	MSAFN_AUTHENTICATE = 0x06,//身份认证及密钥协商
//	MSAFN_FORWARD = 0x08,//请求被级联终端主动上报
//	MSAFN_REQUEST_CONFIG = 0x09, //请求终端配置及信息
	MSAFN_READ_PARAM_TERM = 0x0A,//查询参数终端
//	MSAFN_READ_TASK = 0x0B,//请求任务数据
//	MSAFN_READ_REALTIME = 0x0C,//请求1类数据
//	MSAFN_READ_HISTORY = 0x0D,//请求2类数据
//	MSAFN_READ_ALARM = 0x0E,//请求3类数据
	MSAFN_FILE_TRANSFER = 0x0F,//文件传输
	MSAFN_DATA_ROUTE = 0x10,//数据转发

	MSAFN_WRITE_PARAM_METER = 0x84,//设置参数集抄 
	MSAFN_CTRL_CMD_METER = 0x85, //控制命令
	MSAFN_READ_PARAM_METER = 0x8A,//查询参数集抄 
	MSAFN_READ_REALTIME_METER = 0x8C,//请求1类数据
	MSAFN_READ_HISTORY_METER = 0x8D,//请求2类数据
	MSAFN_READ_ALARM_METER = 0x8E,//请求3类数据

//级联主终端命令
	MSAFN_FORWARD_MS = 0xA0,

//user define
	MSAFN_USER_DEFINE_BEGIN = 0xA1
};

enum
{
	FRAME_PROCESS_STOP,
	FRAME_PROCESS_CONTINUE,

	FRAME_PROCESS_CONFIRM,
	FRAME_PROCESS_DENY,

	FRAME_PROCESS_RESPONSE_CONFIRM,
	FRAME_PROCESS_RESPONSE_DENY,
	FRAME_PROCESS_RESPONSE_DATA,

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
	S_WORD m_PN;
	S_WORD m_FN;
	string m_Info;
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

	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void);

protected:
	int AppendDADT(string &strInfo, const S_CmdObj &CmdObj);

protected:
	S_WORD m_PackageLength;
};

//C_AFNCMD为轻量级对象
class C_AFNCMD
{
public:
	C_AFNCMD(void){m_PackageLength = 400;}
	void SetPackageLength(S_WORD PackageLength){m_PackageLength = PackageLength;}

	virtual bool NeedPwdCheck(void){return false;}
	virtual bool Authenticate(const string &strCmd, const string &strPWD);
	virtual int ParseCmdObj(const string &strCmdInfo, S_CmdObjs &CmdObjs);
	virtual int Execute(const S_CmdObjs &CmdObjs, strings &strResponseInfos);

protected:
	virtual C_CmdReceiver *GetCmdReceiver(S_WORD FN){return NULL;}

protected:
	S_WORD m_PackageLength;
};

//
//命令对象参数
//
struct S_CmdObjPara
{//命令对象参数数组结构
	S_WORD m_FN;
	C_CmdReceiver *m_pCmdReceiver;
};
struct S_CmdObjPara3D
{//命令对象参数数组结构
	S_WORD m_BeginFN;
	S_WORD m_EndFN;
	C_CmdReceiver *m_pCmdReceiver;
};
inline bool operator<(const S_CmdObjPara &left, const S_CmdObjPara &right){return left.m_FN < right.m_FN;}
inline bool operator<(const S_CmdObjPara3D &left, const S_CmdObjPara3D &right){return left.m_BeginFN < right.m_BeginFN;}


#endif //__GXJCB_MSCMD_H__

