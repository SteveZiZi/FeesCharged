/** @file
 *  @brief 后台通讯命令测量点写参数对象实现
 *  @author  zhongguangxiong
 *  @date 2012/02/18
 *  @version 0.1
 */
#ifndef __NW_CMDWMP_H__
#define __NW_CMDWMP_H__
#include "nwmscmd.h"

/** @brief 写测量点参数 基类*/
#define WM_UPDATE_DBCJ_INFO				0x00000001UL
#define WM_UPDATE_METER_INFO				0x00000002UL
#define WM_UPDATE_NOTIFY_ADD_METER		0x00000004UL

class C_WMCmdReceiver:public C_CmdReceiver
{
public:
	bool UpdateInformation(void); // 统一更新下发的测量点属性信息

protected:
	bool SetUpdateFlag(S_DWORD flag); // 设置需要更新的标志

private:
	static S_DWORD m_UpdateFlag;
};

/** @brief 测量点参数 */
class C_CmdwE0800000:public C_WMCmdReceiver //测量点的状态 
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "写测量点的状态 ";}
};
class C_CmdwE0800001:public C_WMCmdReceiver //测量点性质
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "写测量点性质";}
protected:
	bool AddMeterAC(S_WORD SN);
	bool AddMeterRS485(S_WORD SN);
	bool DeleteMeasurePoint(S_WORD MP);
	bool DeleteMeter(S_WORD SN); // 表计
	bool DeleteAnalog(S_WORD SN); // 模拟量
	bool DeletePulse(S_WORD SN); // 脉冲
	bool DeleteSUM(S_WORD SN); // 计算量
	bool DeleteAC(S_WORD SN); // 交采	
};
class C_CmdwE0800002:public C_WMCmdReceiver //测量点地址
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "写测量点地址";}
};
class C_CmdwE0800003:public C_WMCmdReceiver //测量点通信规约
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "写测量点通信规约";}
};
class C_CmdwE0800004:public C_WMCmdReceiver //电能表类型
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "写电能表类型";}
};
class C_CmdwE0800005:public C_WMCmdReceiver //总分类型
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "写测量点总分类型";}
};
class C_CmdwE0800006:public C_WMCmdReceiver //重点用户属性
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "写重点用户属性";}
};
class C_CmdwE0800007:public C_WMCmdReceiver //拉闸功能
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "写测量点拉闸功能";}
};
class C_CmdwE0800008:public C_WMCmdReceiver //最大费率数
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "写测量点最大费率数";}
};
class C_CmdwE0800009:public C_WMCmdReceiver //测量点对应采集终端地址
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "写测量点对应采集终端地址";}
};
class C_CmdwE080000A:public C_WMCmdReceiver //测量点端口号
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "写测量点端口号";}
};
class C_CmdwE080000B:public C_WMCmdReceiver //端口参数
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "写测量点端口参数";}
};
class C_CmdwE080000C:public C_WMCmdReceiver //CT变比
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "写测量点CT变比";}
};
class C_CmdwE080000D:public C_WMCmdReceiver //PT变比
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "写测量点PT变比";}
};
class C_CmdwE080000F:public C_WMCmdReceiver //以上数据项集合
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "写测量点数据项集合";}
private:
	bool SetMeterParam(INT16U MP, const string &strMeterParam);
};

class C_CmdwE0800010:public C_WMCmdReceiver //测量点接线方式
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);	
	virtual char* Prompt(void){return "设置量点接线方式";}

	bool SetMeterParam(INT16U MP, const string &strMeterParam);
};
class C_CmdwE0800011:public C_WMCmdReceiver //额定电压(二次侧)
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "设额定电压";}
};
class C_CmdwE0800012:public C_WMCmdReceiver //额定电流
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "写额定电流";}
};
class C_CmdwE0800013:public C_WMCmdReceiver //用户申报容量
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "设用户申报容量";}
};
class C_CmdwE0800014:public C_WMCmdReceiver //变压器容量
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "设变压器容量";}
};
class C_CmdwE080001F:public C_WMCmdReceiver //以上数据项集合
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	bool SetMeterParam(INT16U MP, const string &strMeterParam);
	virtual char* Prompt(void){return "设以上数据项集合";}
};
class C_CmdwE0800021:public C_WMCmdReceiver //电能表通信密码
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "设电能表通信密码";}
};
class C_CmdwE0800022:public C_WMCmdReceiver //电能表通信用户名
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "设电能表通信用户名";}
};
class C_CmdwE080002F:public C_WMCmdReceiver //以上数据集合
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "设以上数据集合";}
};

//电流不平衡度阈值
class C_CmdwE0800100:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "写电流不平衡度阈值";}
};
//电流不平衡度恢复阈值
class C_CmdwE0800101:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "电流不平衡度恢复阈值";}
};
//合格电压上限
class C_CmdwE0800102:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "合格电压上限";}
};
//合格电压下限
class C_CmdwE0800103:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "合格电压下限";}
};
//判断电流过流的相对额定值的比例
class C_CmdwE0800104:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "判断电流过流的相对额定值的比例";}
};
//判断负荷过载相对额定值的比例
class C_CmdwE0800105:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "判断负荷过载相对额定值的比例";}
};
//判断负荷过载恢复的相对额定值的比例
class C_CmdwE0800106:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "判断负荷过载恢复的相对额定值的比例";}
};
//零相电流报警阈值
class C_CmdwE0800107:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "零相电流报警阈值";}
};
//零相电流报警恢复阈值
class C_CmdwE0800108:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "零相电流报警恢复阈值";}
};
//判断电流过负荷恢复的相对额定值的比例
class C_CmdwE0800109:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "判断电流过负荷恢复的相对额定值的比例";}
};
//电压不平衡度阈值
class C_CmdwE080010A:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "电压不平衡度阈值";}
};
//电压不平衡度恢
class C_CmdwE080010B:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "电压不平衡度恢";}
};
//失压失流或断相报警的判断阈值
class C_CmdwE080010C:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "失压或断相报警的判断阈值";}
};
//以上数据项集合
class C_CmdwE080010F:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "以上数据项集合";}
};
//电压不平衡度恢
class C_CmdwE0800200:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "电压不平衡度恢";}
};
//电流畸变率阈值
class C_CmdwE0800201:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "电流畸变率阈值";}
};
//电压畸变率报警恢复阈值
class C_CmdwE0800202:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "电压畸变率报警恢复阈值";}
};
//电流畸变率报警恢复阈值
class C_CmdwE0800203:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "电流畸变率报警恢复阈值";}
};
//功率因数区段数
class C_CmdwE0800204:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "功率因数区段数";}
};
//功率因数区段
class C_CmdwE0800205:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "功率因数区段";}
};
//监测谐波次数
class C_CmdwE0800206:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "监测谐波次数";}
};
//电压偏差上限
class C_CmdwE0800207:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "电压偏差上限";}
};
//电压偏差下限
class C_CmdwE0800208:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "电压偏差下限";}
};
//频率偏差上限
class C_CmdwE0800209:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "频率偏差上限";}
};
//频率偏差下限
class C_CmdwE080020A:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "频率偏差下限";}
};
//电压长闪限值
class C_CmdwE080020B:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "电压长闪限值";}
};
//电压短闪限值
class C_CmdwE080020C:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "电压短闪限值";}
};
//无功过补偿欠补偿告警发生阈值
class C_CmdwE080020D:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "无功过补偿欠补偿告警发生阈值";}
};
//无功过补偿欠补偿告警恢复阈值
class C_CmdwE080020E:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "无功过补偿欠补偿告警恢复阈值";}
};

#endif//__NW_CMDWMP_H__


