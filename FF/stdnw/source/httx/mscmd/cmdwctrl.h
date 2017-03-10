/** @file
 *  @brief 后台通讯命令负控写参数对象实现
 *  @author  zhongguangxiong
 *  @date 2012/02/18
 *  @version 0.1
 */
#ifndef __NW_CMDWCTRL_H__
#define __NW_CMDWCTRL_H__
#include "nwmscmd.h"

//遥控拉闸时间
class C_CmdwE0000C40:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "遥控拉闸时间";}
};
//保电状态：01保电状态，00非保电状态
class C_CmdwE0000C50:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "保电状态";}
};
//功率控制是否有效
class C_CmdwE0000C60:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "功率控制是否有效";}
};
//月度电量控制是否有效
class C_CmdwE0000C61:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "月度电量控制是否有效";}
};

//购电量控制是否有效
class C_CmdwE0000C62:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "购电量控制是否有效";}
};
//功率控制各轮次跳闸是否有效
class C_CmdwE0000C63:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "功率控制各轮次跳闸是否有效";}
};
//月电量控制跳闸轮次有效标志
class C_CmdwE0000C64:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "月电量控制跳闸轮次有效标志";}
};
//购电量控制跳闸轮次有效标志
class C_CmdwE0000C65:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "购电量控制跳闸轮次有效标志";}
};
//超过设定值跳闸的过负荷持续时间
class C_CmdwE0000C66:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "超过设定值跳闸的过负荷持续时间";}
};
//超过设定值跳闸恢复的负荷恢复持续时间，缺省10分钟
class C_CmdwE0000C67:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "超过设定值跳闸恢复的负荷恢复持续时间";}
};
//拉闸命令告警延时，缺省10分钟
class C_CmdwE0000C68:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "拉闸命令告警延时";}
};
//功率控制用功率累加标志
class C_CmdwE0000C69:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "功率控制用功率累加标志";}
};
//电量控制用电量累加标志
class C_CmdwE0000C6A:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "电量控制用电量累加标志";}
};
//功率定值浮动系数（％）缺省为0
class C_CmdwE0000C6B:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "功率定值浮动系数（％）缺省为0";}
};
//月电量控定值浮动系数（％）缺省为00
class C_CmdwE0000C6C:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "月电量控定值浮动系数（％）缺省为00";}
};
//最低负荷
class C_CmdwE0000C6D:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "最低负荷";}
};
//X临时限电控有效标识
class C_CmdwE0000C70:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "X临时限电控有效标识";}
};
//临时限电的计算滑窗时间，单位分钟
class C_CmdwE0000C71:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "临时限电的计算滑窗时间，单位分钟";}
};
//功率控制系列1，时段数
class C_CmdwE0000D00:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "功率控制系列1，时段数";}
};
//功率控制系列1,执行时间
class C_CmdwE0000D09:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "功率控制系列1,执行时间";}
};
//功率控制系列1，以上数据项集合
class C_CmdwE0000D0F:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "功率控制系列1，以上数据项集合";}
private:
	bool SetAllParam(const string &strAllParam);
};
//功率控制功率定值系列2，时段数
class C_CmdwE0000D10:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "功率控制功率定值系列2，时段数";}
};
//功率控制系列2,执行时间
class C_CmdwE0000D19:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "功率控制系列2,执行时间";}
};
//功率控制系列2，以上数据项集合
class C_CmdwE0000D1F:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "功率控制系列2，以上数据项集合";}
private:
	bool SetAllParam(const string &strAllParam);
};
//功率控制功率定值系列3，时段数
class C_CmdwE0000D20:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "功率控制功率定值系列3，时段数";}
};
//功率控制系列3,执行时间
class C_CmdwE0000D29:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "功率控制系列3,执行时间";}
};
//功率控制系列3，以上数据项集合
class C_CmdwE0000D2F:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "功率控制系列3，以上数据项集合";}
private:
	bool SetAllParam(const string &strAllParam);
};
//功率控制功率定值系列4，时段数
class C_CmdwE0000D30:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "功率控制功率定值系列3，时段数";}
};
//功率控制系列4,执行时间
class C_CmdwE0000D39:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "功率控制系列3,执行时间";}
};
//功率控制系列4，以上数据项集合
class C_CmdwE0000D3F:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "功率控制系列3，以上数据项集合";}
private:
	bool SetAllParam(const string &strAllParam);
};
//月电量控制，月电量限额
class C_CmdwE0000E00:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "月电量控制，月电量限额";}
};
//月电量控制，轮次电量
class C_CmdwE0000E01:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "月电量控制，轮次电量";}
};
//购电量控制，本次购电量
class C_CmdwE0000E02:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "月电量控制，月电量限额";}
};
//购电量控制，剩余电量
class C_CmdwE0000E03:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "月电量控制，轮次电量";}
};
//终端电能量费率时段和费率数
class C_CmdwE0000E04:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "终端电能量费率时段和费率数";}
};
//
class C_CmdwE0000E05:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "";}
};
//厂休控是否投入
class C_CmdwE0000E10:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "厂休控是否投入";}
};
//厂休控参数
class C_CmdwE0000E11:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "厂休控参数";}
};


//二个数据项区域
//负控参数 开关接点类型
class C_CmdwE0000C00:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "开关接点类型";}
};
//输出开关状态(只写)
class C_CmdwE0000C20:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "输出开关状态";}
};
//输出开关轮次
class C_CmdwE0000C30:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "输出开关轮次";}
};
//功率控制系列1，时段(1-8)
class C_CmdwE0000D01:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "功率控制系列1，时段(1-8)";}
};
//功率控制系列2，时段(1-8)
class C_CmdwE0000D11:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "功率控制系列2，时段(1-8)";}
};
//功率控制系列3，时段(1-8)
class C_CmdwE0000D21:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "功率控制系列3，时段(1-8)";}
};
//功率控制系列4，时段(1-8)
class C_CmdwE0000D31:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "功率控制系列4，时段(1-8)";}
};

#endif//__NW_CMDWCTRL_H__


