/** @file
 *  @brief 后台通讯命令测量点读参数对象实现
 *  @author  zhongguangxiong
 *  @date 2012/02/18
 *  @version 0.1
 */
#ifndef __NW_CMDRMP_H__
#define __NW_CMDRMP_H__
#include "nwmscmd.h"

/** @brief 测量点参数读操作的基类 */
class C_RMCmdReceiver:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};

/** @brief 测量点参数 */
class C_CmdrE0800000:public C_RMCmdReceiver //测量点的状态 
{
public:
	virtual char* Prompt(void){return "读测量点的状态";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE0800001:public C_RMCmdReceiver //测量点性质
{
public:
	virtual char* Prompt(void){return "读测量点性质";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE0800002:public C_RMCmdReceiver //测量点地址
{
public:
	virtual char* Prompt(void){return "读测量点地址";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE0800003:public C_RMCmdReceiver //测量点通信规约
{
public:
	virtual char* Prompt(void){return "读测量点通信规约";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE0800004:public C_RMCmdReceiver //电能表类型
{
public:
	virtual char* Prompt(void){return "读电能表类型";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE0800005:public C_RMCmdReceiver //总分类型
{
public:
	virtual char* Prompt(void){return "读测量点总分类型";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE0800006:public C_RMCmdReceiver //重点用户属性
{
public:
	virtual char* Prompt(void){return "读重点用户属性";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE0800007:public C_RMCmdReceiver //拉闸功能
{
public:
	virtual char* Prompt(void){return "读测量点拉闸功能";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE0800008:public C_RMCmdReceiver //最大费率数
{
public:
	virtual char* Prompt(void){return "读测量点最大费率数";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE0800009:public C_RMCmdReceiver //测量点对应采集终端地址
{
public:
	virtual char* Prompt(void){return "读测量点对应采集终端地址";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE080000A:public C_RMCmdReceiver //测量点端口号
{
public:
	virtual char* Prompt(void){return "读测量点端口号";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE080000B:public C_RMCmdReceiver //端口参数
{
public:
	virtual char* Prompt(void){return "读端口参数";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE080000C:public C_RMCmdReceiver //CT变比
{
public:
	virtual char* Prompt(void){return "读CT变比";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE080000D:public C_RMCmdReceiver //PT变比
{
public:
	virtual char* Prompt(void){return "读PT变比";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE080000F:public C_RMCmdReceiver //以上数据项集合
{
public:
	virtual char* Prompt(void){return "读测量点数据集合";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE0800010:public C_RMCmdReceiver //测量点接线方式
{
public:
	virtual char* Prompt(void){return "读测量点接线方式";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE0800011:public C_RMCmdReceiver //额定电压(二次侧)
{
public:
	virtual char* Prompt(void){return "读额定电压";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE0800012:public C_RMCmdReceiver //额定电流
{
public:
	virtual char* Prompt(void){return "读额定电流";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE0800013:public C_RMCmdReceiver //用户申报容量
{
public:
	virtual char* Prompt(void){return "读用户申报容量";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE0800014:public C_RMCmdReceiver //变压器容量
{
public:
	virtual char* Prompt(void){return "读变压器容量";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE080001F:public C_RMCmdReceiver //以上数据项集合
{
public:
	virtual char* Prompt(void){return "读以上数据项集合";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE0800021:public C_RMCmdReceiver //电能表通信密码
{
public:
	virtual char* Prompt(void){return "读电能表通信密码";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE0800022:public C_RMCmdReceiver //电能表通信用户名
{
public:
	virtual char* Prompt(void){return "读电能表通信用户名";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE080002F:public C_RMCmdReceiver //以上数据集合
{
public:
	virtual char* Prompt(void){return "读以上数据集合";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};

//电流不平衡度阈值
class C_CmdrE0800100:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "电流不平衡度阈值";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//电流不平衡度恢复阈值
class C_CmdrE0800101:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "电流不平衡度恢复阈值";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//合格电压上限
class C_CmdrE0800102:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "合格电压上限";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//合格电压下限
class C_CmdrE0800103:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "合格电压下限";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//判断电流过流的相对额定值的比例
class C_CmdrE0800104:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "判断电流过流的相对额定值的比例";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//判断负荷过载相对额定值的比例
class C_CmdrE0800105:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "判断负荷过载相对额定值的比例";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//判断负荷过载恢复的相对额定值的比例
class C_CmdrE0800106:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "判断负荷过载恢复的相对额定值的比例";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//零相电流报警阈值
class C_CmdrE0800107:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "零相电流报警阈值";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//零相电流报警恢复阈值
class C_CmdrE0800108:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "零相电流报警恢复阈值";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//判断电流过负荷恢复的相对额定值的比例
class C_CmdrE0800109:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "判断电流过负荷恢复的相对额定值的比例";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//电压不平衡度阈值
class C_CmdrE080010A:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "电压不平衡度阈值";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//电压不平衡度恢
class C_CmdrE080010B:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "电压不平衡度恢";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//失压或断相报警的判断阈值
class C_CmdrE080010C:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "失压或断相报警的判断阈值";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//以上数据项集合
class C_CmdrE080010F:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "以上数据项集合";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//电压不平衡度恢
class C_CmdrE0800200:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "电压不平衡度恢";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//电流畸变率阈值
class C_CmdrE0800201:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "电流畸变率阈值";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//电压畸变率报警恢复阈值
class C_CmdrE0800202:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "电压畸变率报警恢复阈值";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//电流畸变率报警恢复阈值
class C_CmdrE0800203:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "电流畸变率报警恢复阈值";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//功率因数区段数
class C_CmdrE0800204:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "功率因数区段数";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//功率因数区段
class C_CmdrE0800205:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "功率因数区段";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//监测谐波次数
class C_CmdrE0800206:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "监测谐波次数";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//电压偏差上限
class C_CmdrE0800207:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "电压偏差上限";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//电压偏差下限
class C_CmdrE0800208:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "电压偏差下限";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//频率偏差上限
class C_CmdrE0800209:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "频率偏差上限";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//频率偏差下限
class C_CmdrE080020A:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "频率偏差下限";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//电压长闪限值
class C_CmdrE080020B:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "电压长闪限值";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//电压短闪限值
class C_CmdrE080020C:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "电压短闪限值";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//无功过补偿欠补偿告警发生阈值
class C_CmdrE080020D:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "无功过补偿欠补偿告警发生阈值";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//无功过补偿欠补偿告警恢复阈值
class C_CmdrE080020E:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "无功过补偿欠补偿告警恢复阈值";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};

#endif //__NW_CMDRMP_H__


