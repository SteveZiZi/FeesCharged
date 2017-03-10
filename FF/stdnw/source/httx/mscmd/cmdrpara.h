/** @file
 *  @brief 后台通讯命令读参数服务定义
 *  @author 
 *  @date 2011/12/22
 *  @version 0.1
 */
#ifndef __NW_CMDRPARA_H__
#define __NW_CMDRPARA_H__

#include "nwmscmd.h"
#include "status.h"
#include "busialgo.h"


//主站通信参数主
class C_CmdrE0000100:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读主站通信参数";}
protected:
	bool ReadParam(string &strParam,S_CHANNEL *pMS);
};
//主站通信参数备用1
class C_CmdrE0000101:public C_CmdrE0000100
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读主站通信参数备用1";}
};
//主站通信参数备用2
class C_CmdrE0000102:public C_CmdrE0000100
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读主站通信参数备用2";}
};
//网关地址
class C_CmdrE0000103:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读网关地址";}
};
//APN
class C_CmdrE0000104:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读APN";}
};
//APN用户名
class C_CmdrE0000105:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读APN用户名";}
};
//APN密码
class C_CmdrE0000106:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读APN密码";}
};
//心跳周期
class C_CmdrE0000107:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读心跳周期";}
};
//掉线重拨间隔，缺省30分钟
class C_CmdrE0000108:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "掉线重拨间隔";}
};
//掉线后重拨次数，缺省3次,重拨指定次数后，当天不再重拨
class C_CmdrE0000109:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "掉线后重拨次数";}
};
//连接方式，0：TCP方式；1：UDP方式
class C_CmdrE000010A:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "连接方式，0：TCP方式；1：UDP方式";}
};
//压缩开关:0：非压缩  1：压缩，BIN编码
class C_CmdrE000010B:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "压缩开关:0：非压缩  1：压缩，BIN编码";}
};
//以上数据集合
class C_CmdrE000010F:public C_CmdrE0000100
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "以上数据集合";}
private:
	bool GetAllParam(string &strAllParam);
};
//终端地市区县码
class C_CmdrE0000120:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "终端地市区县码";}
};
//终端地址
class C_CmdrE0000121:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "终端地址";}
};
//终端通信工作模式
class C_CmdrE0000122:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "终端通信工作模式";}
};
//混合模式和服务器模式下，TCP监听端口号，BIN编码
class C_CmdrE0000123:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "TCP监听端口号";}
};
//终端当前通道IP地址
class C_CmdrE0000124:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "终端当前通道IP地址";}
};
//终端当前通道子网掩码地址
class C_CmdrE0000125:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "终端当前通道子网掩码地址";}
};
//终端当前通道子网掩码地址
class C_CmdrE0000126:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "终端当前通道子网掩码地址";}
};
//有线网络IP地址获取方式 0：手动设置IP地址 1：自动设置IP地址
class C_CmdrE0000127:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "有线网络IP地址获取方式";}
};
//以上数据项集合
class C_CmdrE000012F:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "以上数据项集合";}
private:
	bool GetAllParam(string &strAllParam);
};
//终端时间
class C_CmdrE0000130:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读终端时间";}
};
//告警主动上报屏蔽字
class C_CmdrE0000150:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读告警主动上报屏蔽字";}
};
//告警判断屏蔽字
class C_CmdrE0000151:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读告警判断屏蔽字";}
};
//事件记录屏蔽字
class C_CmdrE0000152:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读事件记录屏蔽字";}
};
//月冻结电量 DD：结算时间的日，范围1~30，缺省为每月1号 HH：结算时间的小时，范围0~23，缺省为0点
class C_CmdrE0000160:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读月冻结电量冻结时间";}
};
//日冻结电量 HH：日电能量（表码）结算时刻，范围0~23，缺省为0点
class C_CmdrE0000161:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读日冻结电量冻结时间";}
};
//月电压合格率数据冻结 DD：结算时间的日，范围1~30，缺省为每月21号HH：结算时间的小时，范围0~23，缺省为0点
class C_CmdrE0000162:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读月电压合格率数据冻结时间";}
};
//日电压合格率数据冻结 HH：日电能量（表码）结算时刻，范围0~23，缺省为0点
class C_CmdrE0000163:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读日电压合格率数据冻结时间";}
};
//消息认证码字段PW校验方式 (只写)
//class C_CmdrE0000170:public C_CmdReceiver
//{
//public:
//	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
//	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
//	virtual char* Prompt(void){return "消息认证码字段PW校验方式";}
//};
//认证请求信息，BIN编码，密文
class C_CmdrE0000171:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读认证请求信息";}
};
//认证响应信息，BIN编码，密文
class C_CmdrE0000172:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读认证响应信息";}
};
//随机数信息，BIN编码，密文
class C_CmdrE0000173:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读随机数信息";}
};
//随机数响应信息，BIN编码，密文
class C_CmdrE0000174:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读随机数响应信息";}
};
//判断负荷过载、电流过负荷、过电压A、超合同容量用电、功率超定值、电流不平衡度、电压畸变、电流畸变、零序电流偏大、无功欠补偿、无功过补偿的时间缺省15分钟
class C_CmdrE0000180:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读阈值参数，超限的时间";}
};
//判断负荷过载、电流过负荷、过电压超合同容量用电、功率超定值、电流不平衡度、电压畸变、电流畸变、零序电流偏大、无功欠补偿、无功过补偿恢复时间缺省15分钟
class C_CmdrE0000181:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读阈值参数，超限恢复的时间";}
};
//阈值参数，判断断相、失压、失流时间，缺省值1分钟
class C_CmdrE0000182:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读阈值参数，判断断相、失压、失流时间";}
};
//阈值参数，判断断相、失压、失流恢复时间，缺省值1分钟
class C_CmdrE0000183:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读阈值参数，判断断相、失压、失流恢复时间";}
};
//阈值参数，判断电压逆相序时间，缺省值15分钟
class C_CmdrE0000184:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读阈值参数，判断电压逆相序时间";}
};
//阈值参数，判断电压逆相序恢复时间，缺省值15分钟
class C_CmdrE0000185:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读阈值参数，判断电压逆相序恢复时间";}
};
//阈值参数，判断电流反极性时间，缺省值15分钟
class C_CmdrE0000186:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读阈值参数，判断电流反极性时间";}
};
//阈值参数，判断电流反极性恢复时间，缺省值15分钟
class C_CmdrE0000187:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读阈值参数，判断电流反极性恢复时间";}
};
//阈值参数，电能表时钟异常阈值，缺省值10分钟
class C_CmdrE0000188:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读阈值参数，电能表时钟异常阈值";}
};
//阈值参数，电能表飞走阈值，缺省为10倍
class C_CmdrE0000189:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读阈值参数，电能表飞走阈值";}
};
//阈值参数，电能表停走阈值：电量示度停止增长时按照有功功率计算应走电量值，越此值则电能表停走，缺省为0.1kWh
class C_CmdrE000018A:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读阈值参数，电能表停走阈值";}
};
//阈值参数，剩余金额不足阈值：缺省为20.00 元
class C_CmdrE000018B:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读阈值参数，剩余金额不足阈值";}
};
//阈值参数，月通信流量越限值
class C_CmdrE000018C:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读阈值参数，月通信流量越限值";}
};
//以上数据项集合
class C_CmdrE000018F:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读阈值参数,以上数据项集合";}
private:
	bool GetAllParam(string &strAllParam);
};
//有效的抄表参数配置表总数 (只读)
class C_CmdrE0000221:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读有效的抄表参数配置表总数";}
};
//具体表属性对应的抄表参数配置表
class C_CmdrE0000222:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读具体表属性对应的抄表参数配置表";}
};
//现有普通任务总数 (只读)
class C_CmdrE0000300:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读现有普通任务总数";}
};
//现有中继任务总数
class C_CmdrE0000400:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读现有中继任务总数";}
};
//模拟量数量
class C_CmdrE0000500:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读模拟量数量";}
};
//模拟量状态标记
class C_CmdrE00005FE:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读模拟量状态标记";}
};
//以上数据集合
class C_CmdrE00005FF:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读模拟量 以上数据集合";}
};
//脉冲量数量
class C_CmdrE0000600:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读脉冲量数量";}
};
//脉冲量状态标记
class C_CmdrE00006FE:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读脉冲量状态标记";}
};
//以上数据集合
class C_CmdrE00006FF:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读脉冲量 以上数据集合";}
};
//运算量数量
class C_CmdrE0000700:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读运算量数量";}
};
//运算量状态标记
class C_CmdrE00007FE:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读运算量状态标记";}
};
//以上数据集合
class C_CmdrE00007FF:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读运算量 以上数据集合";}
};
//差动规则数量
class C_CmdrE0000800:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读差动规则数量";}
};
//差动规则状态标记
class C_CmdrE00008FE:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读差动规则状态标记";}
};
//以上数据集合
class C_CmdrE00008FF:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读差动 以上数据集合";}
};
//终端无功电量统计标志
class C_CmdrE0000900:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读终端无功电量统计标志";}
};
//级联从终端地址1~4
class C_CmdrE0000A20:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读级联从终端地址";}
};
//主终端地址
class C_CmdrE0000A21:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读主终端地址";}
};
//被抄终端地址
class C_CmdrE0000A22:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读被抄终端地址";}
};
//主终端等待从终端应答的超时判定时间
class C_CmdrE0000A23:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读主终端等待从终端应答的超时判定时间";}
};
//从终端等待主终端级联控制命令的超时判定时间
class C_CmdrE0000A24:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读从终端等待主终端级联控制命令的超时判定时间";}
};
//级联台区终端CT倍率
class C_CmdrE0000A25:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读级联台区终端CT倍率";}
};
//级联台区终端PT倍率
class C_CmdrE0000A26:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读级联台区终端PT倍率";}
};
//轮询
class C_CmdrE0000A30:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读轮询";}
};
//传输控制
class C_CmdrE0000A31:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读传输控制";}
};
//回复传输控制命令
class C_CmdrE0000A32:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读回复传输控制命令";}
};
//抄读终端台区总电量
class C_CmdrE0000A33:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读抄读终端台区总电量";}
};
//规约版本号
class C_CmdrE0000B00:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读规约版本号";}
};
//终端软件版本号
class C_CmdrE0000B01:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读终端软件版本号";}
};
//终端硬件版本号
class C_CmdrE0000B02:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读终端硬件版本号";}
};
//用户自定义数据CCCC
class C_CmdrE0002000:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "读用户自定义数据CCCC";}
};

//二个数据项区域
//抄表参数
class C_CmdrE0000200:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "有效抄表通道定义";}
};
class C_CmdrE0000230:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "抄表参数配置表";}
};

//任务参数 普通任务号
class C_CmdrE0000301:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "任务参数 普通任务号";}
};
//任务参数 中继任务号
class C_CmdrE0000401:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "任务参数 中继任务号";}
};
//运算量参数 模拟量号
class C_CmdrE0000501:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "运算量参数 模拟量号";}
};
//运算量参数 脉冲量号
class C_CmdrE0000601:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "运算量参数 脉冲量号";}
};
//运算量参数 运算量编号
class C_CmdrE0000701:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "运算量参数 运算量编号";}
};
//运算量参数 差动规则
class C_CmdrE0000801:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "运算量参数 差动规则";}
};
//端口参数 485端口定义
class C_CmdrE0000A00:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "端口参数 485端口定义";}

protected:
	S_BYTE VComFunction(S_BYTE VCom);
};
/**/
#endif //__NW_CMDRPARA_H__

