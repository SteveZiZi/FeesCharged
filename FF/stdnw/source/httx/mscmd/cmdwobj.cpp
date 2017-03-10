/** @file
 *  @brief 后台通讯命令写参数对象定义及查找
 *  @author  zhongguangxiong
 *  @date 2012/02/18
 *  @version 0.1
 */
#include "cmdwobj.h"
#include "cmdwpara.h"
#include "cmdwctrl.h"
#include "cmdwmp.h"
#include "AlarmEventRecord.h"

//参数对象定义
//通信参数
static C_CmdwE0000100 CmdwE0000100; //主站参数
static C_CmdwE0000101 CmdwE0000101; //主站参数备用1
static C_CmdwE0000102 CmdwE0000102; //主站参数备用2
static C_CmdwE0000103 CmdwE0000103; //网关
static C_CmdwE0000104 CmdwE0000104; //APN
static C_CmdwE0000105 CmdwE0000105; //APN用户
static C_CmdwE0000106 CmdwE0000106; //APN密码
static C_CmdwE0000107 CmdwE0000107; //心跳周期
static C_CmdwE0000108 CmdwE0000108; //掉线重拨间隔
static C_CmdwE0000109 CmdwE0000109; //掉线后重拨次数，缺省3次,重拨指定次数后，当天不再重拨
static C_CmdwE000010A CmdwE000010A; //连接方式 0：TCP方式；1：UDP方式
static C_CmdwE000010B CmdwE000010B; //压缩开关:0：非压缩  1：压缩，BIN编码
static C_CmdwE000010F CmdwE000010F; //以上数据项集合

static C_CmdwE0000120 CmdwE0000120; //终端地市区县码
static C_CmdwE0000121 CmdwE0000121; //终端地址
static C_CmdwE0000122 CmdwE0000122; //终端通信工作模式
static C_CmdwE0000123 CmdwE0000123; //混合模式和服务器模式下，TCP监听端口号，BIN编码
static C_CmdwE0000124 CmdwE0000124; //终端当前通道IP地址
static C_CmdwE0000125 CmdwE0000125; //终端当前通道子网掩码地址
static C_CmdwE0000126 CmdwE0000126; //终端当前通道网关地址
static C_CmdwE0000127 CmdwE0000127; //有线网络IP地址获取方式 0：手动设置IP地址 1：自动设置IP地址
static C_CmdwE000012F CmdwE000012F; //以上数据项集合
//时间参数
static C_CmdwE0000130 CmdwE0000130; //终端时间
static C_CmdwE0000131 CmdwE0000131; //实时校时 (只写)
//复位操作
static C_CmdwE0000140 CmdwE0000140; //终端复位 (只写)
//告警屏蔽
static C_CmdwE0000150 CmdwE0000150; //告警主动上报屏蔽字
static C_CmdwE0000151 CmdwE0000151; //告警判断屏蔽字
static C_CmdwE0000152 CmdwE0000152; //事件记录屏蔽字
//结算日参数
static C_CmdwE0000160 CmdwE0000160; //结算时间的日、小时（用于月冻结电量和电压合格率冻结等） 月冻结电量
static C_CmdwE0000161 CmdwE0000161; //日电能量（表码）结算时刻（用于日电量冻结和电压合格率等） 日冻结电量
static C_CmdwE0000162 CmdwE0000162; //月电压合格率数据冻结
static C_CmdwE0000163 CmdwE0000163; //日电压合格率数据冻结
//身份认证
static C_CmdwE0000170 CmdwE0000170; //消息认证码字段PW校验方式 (只写)
static C_CmdwE0000171 CmdwE0000171; //认证请求信息，BIN编码，密文
static C_CmdwE0000172 CmdwE0000172; //认证响应信息，BIN编码，密文
static C_CmdwE0000173 CmdwE0000173; //随机数信息，BIN编码，密文
static C_CmdwE0000174 CmdwE0000174; //随机数响应信息，BIN编码，密文
//阈值参数
static C_CmdwE0000180 CmdwE0000180;//判断负荷过载、电流过负荷、过电压、超合同容量用电、功率超定值、电流不平衡度、电压畸变、电流畸变、零序电流偏大、无功欠补偿、无功过补偿的时间、缺省15分钟
static C_CmdwE0000181 CmdwE0000181;//判断负荷过载、电流过负荷、过电压、超合同容量用电、功率超定值、电流不平衡度、电压畸变、电流畸变、零序电流偏大、无功欠补偿、无功过补偿恢复时间、缺省15分钟
static C_CmdwE0000182 CmdwE0000182;//判断断相、失压、失流时间，缺省值1分钟
static C_CmdwE0000183 CmdwE0000183;//判断断相、失压、失流恢复时间，缺省值1分钟
static C_CmdwE0000184 CmdwE0000184;//判断电压逆相序时间，缺省值15分钟
static C_CmdwE0000185 CmdwE0000185;//判断电压逆相序恢复时间，缺省值15分钟
static C_CmdwE0000186 CmdwE0000186;//判断电流反极性时间，缺省值15分钟
static C_CmdwE0000187 CmdwE0000187;//判断电流反极性恢复时间，缺省值15分钟
static C_CmdwE0000188 CmdwE0000188;//电能表时钟异常阈值，缺省值10分钟
static C_CmdwE0000189 CmdwE0000189;//电能表飞走阈值，缺省为10倍
static C_CmdwE000018A CmdwE000018A;//电能表停走阈值 缺省为0.1kWh
static C_CmdwE000018B CmdwE000018B;//剩余金额不足阈值：缺省为20.00 元
static C_CmdwE000018C CmdwE000018C;//月通信流量越限值
static C_CmdwE000018F CmdwE000018F;//以上数据集合
//抄表参数
//static C_CmdwE0000221 CmdwE0000221;//有效的抄表参数配置表总数 (只读)
static C_CmdwE0000222 CmdwE0000222;//具体表属性对应的抄表参数配置表
//任务参数
//static C_CmdwE0000300 CmdwE0000300;//现有普通任务总数 (只读)

//static C_CmdwE0000400 CmdwE0000400;//现有中继任务总数 (只读)
//运算量参数
static C_CmdwE0000500 CmdwE0000500;//模拟量数量
static C_CmdwE00005FE CmdwE00005FE;//模拟量状态标记
static C_CmdwE00005FF CmdwE00005FF;//以上数据集合
//脉冲量参数
static C_CmdwE0000600 CmdwE0000600;//脉冲量数量
static C_CmdwE00006FE CmdwE00006FE;//脉冲量状态标记
static C_CmdwE00006FF CmdwE00006FF;//以上数据集合
//运算量参数
static C_CmdwE0000700 CmdwE0000700;//运算量数量
static C_CmdwE00007FE CmdwE00007FE;//运算量状态标记
static C_CmdwE00007FF CmdwE00007FF;//以上数据集合
//差动参数
static C_CmdwE0000800 CmdwE0000800;//差动规则数量
static C_CmdwE00008FE CmdwE00008FE;//差动规则状态标记
static C_CmdwE00008FF CmdwE00008FF;//以上数据集合

static C_CmdwE0000900 CmdwE0000900;//终端无功电量统计标志
//端口参数
//级联通信参数
static C_CmdwE0000A20 CmdwE0000A20;//级联从终端地址1~4
static C_CmdwE0000A21 CmdwE0000A21;//主终端地址
static C_CmdwE0000A22 CmdwE0000A22;//被抄终端地址
static C_CmdwE0000A23 CmdwE0000A23;//主终端等待从终端应答的超时判定时间
static C_CmdwE0000A24 CmdwE0000A24;//从终端等待主终端级联控制命令的超时判定时间
static C_CmdwE0000A25 CmdwE0000A25;//级联台区终端CT倍率
static C_CmdwE0000A26 CmdwE0000A26;//级联台区终端PT倍率
//级联控制
//static C_CmdwE0000A30 CmdwE0000A30;//轮询 (只读)
//static C_CmdwE0000A31 CmdwE0000A31;//传输控制 (只读)
//static C_CmdwE0000A32 CmdwE0000A32;//回复传输控制命令 (只读)
//static C_CmdwE0000A33 CmdwE0000A33;//抄读终端台区总电量 (只读)
//版本参数
//static C_CmdwE0000B01 CmdwE0000B01;//规约版本号，本规约规定为0100  (只读)
//static C_CmdwE0000B02 CmdwE0000B02;//终端软件版本号  (只读)
//static C_CmdwE0000B03 CmdwE0000B03;//终端硬件版本号  (只读)
//负控参数
static C_CmdwE0000C40 CmdwE0000C40;//遥控拉闸时间
static C_CmdwE0000C50 CmdwE0000C50;//保电状态：01保电状态，00非保电状态
static C_CmdwE0000C60 CmdwE0000C60;//功率控制是否有效
static C_CmdwE0000C61 CmdwE0000C61;//月度电量控制是否有效 
static C_CmdwE0000C62 CmdwE0000C62;//购电量控制是否有效 
static C_CmdwE0000C63 CmdwE0000C63;//功率控制各轮次跳闸是否有效，
static C_CmdwE0000C64 CmdwE0000C64;//月电量控制跳闸轮次有效标志，
static C_CmdwE0000C65 CmdwE0000C65;//购电量控制跳闸轮次有效标志，
static C_CmdwE0000C66 CmdwE0000C66;//超过设定值跳闸的过负荷持续时间，	缺省10分钟
static C_CmdwE0000C67 CmdwE0000C67;//超过设定值跳闸恢复的负荷恢复持续时间，缺省10分钟
static C_CmdwE0000C68 CmdwE0000C68;//拉闸命令告警延时，缺省10分钟
static C_CmdwE0000C69 CmdwE0000C69;//功率控制用功率累加标志
static C_CmdwE0000C6A CmdwE0000C6A;//电量控制用电量累加标志
static C_CmdwE0000C6B CmdwE0000C6B;//功率定值浮动系数（％）缺省为0
static C_CmdwE0000C6C CmdwE0000C6C;//月电量控定值浮动系数（％）缺省为00
static C_CmdwE0000C6D CmdwE0000C6D;//最低负荷
static C_CmdwE0000C70 CmdwE0000C70;//X临时限电控有效标识
static C_CmdwE0000C71 CmdwE0000C71;//临时限电的计算滑窗时间，单位分钟

static C_CmdwE0000D00 CmdwE0000D00;//功率控制系列1，时段数
static C_CmdwE0000D09 CmdwE0000D09;//功率控制系列1,执行时间
static C_CmdwE0000D0F CmdwE0000D0F;//功率控制系列1，以上数据合集
static C_CmdwE0000D10 CmdwE0000D10;//功率控制功率定值系列2,时段数
static C_CmdwE0000D19 CmdwE0000D19;//功率控制系列2,执行时间
static C_CmdwE0000D1F CmdwE0000D1F;//功率控制系列2，以上数据合集
static C_CmdwE0000D20 CmdwE0000D20;//功率控制功率定值系列3,时段数
static C_CmdwE0000D29 CmdwE0000D29;//功率控制系列3,执行时间
static C_CmdwE0000D2F CmdwE0000D2F;//功率控制系列3，以上数据合集
static C_CmdwE0000D30 CmdwE0000D30;//功率控制功率定值系列4,时段数
static C_CmdwE0000D39 CmdwE0000D39;//功率控制系列4,执行时间
static C_CmdwE0000D3F CmdwE0000D3F;//功率控制系列4，以上数据合集
static C_CmdwE0000E00 CmdwE0000E00;//月电量控制，月电量限额
static C_CmdwE0000E01 CmdwE0000E01;//月电量控制，轮次电量
static C_CmdwE0000E02 CmdwE0000E02;//购电量控制，本次购电量
static C_CmdwE0000E03 CmdwE0000E03;//购电量控制，剩余电量
static C_CmdwE0000E04 CmdwE0000E04;//终端电能量费率时段和费率数
static C_CmdwE0000E05 CmdwE0000E05;//
static C_CmdwE0000E10 CmdwE0000E10;//厂休控是否投入
static C_CmdwE0000E11 CmdwE0000E11;//厂休控参数
/* 不读不写
//链路接口数据标识
static C_CmdwE0001000 CmdwE0001000;//表示终端登录，NNNN：规约版本号 
static C_CmdwE0001001 CmdwE0001001;//表示终端心跳，无数据体
static C_CmdwE0001002 CmdwE0001002;//表示登录退出，无数据体 
*/
//测量点控制参数
static C_CmdwE0001100 CmdwE0001100;//对电能表遥控拉闸，只有信息点标识和测量点地址相匹配才允许操作 (只写)
static C_CmdwE0001101 CmdwE0001101;//对电能表遥控合闸，只有信息点标识和测量点地址相匹配才允许操作 (只写)
static C_CmdwE0001102 CmdwE0001102;//对电能表保电操作，只有信息点标识和测量点地址相匹配才允许操作 (只写)
static C_CmdwE0001103 CmdwE0001103;//对电能表保电解除操作，只有信息点标识和测量点地址相匹配才允许操作 (只写)
static C_CmdwE0001104 CmdwE0001104;//删除测量点NNNN表示待删除测量点号 
//用户自定义数据
static C_CmdwE0002000 CmdwE0002000;//用户自定义数据CCCC：厂商编码NN……NN：厂商自定义数据
//测量点参数
static C_CmdwE0800000 CmdwE0800000; //测量点的状态 
static C_CmdwE0800001 CmdwE0800001; //测量点性质
static C_CmdwE0800002 CmdwE0800002; //测量点地址
static C_CmdwE0800003 CmdwE0800003; //测量点通信规约
static C_CmdwE0800004 CmdwE0800004; //电能表类型
static C_CmdwE0800005 CmdwE0800005; //总分类型
static C_CmdwE0800006 CmdwE0800006; //重点用户属性
static C_CmdwE0800007 CmdwE0800007; //拉闸功能
static C_CmdwE0800008 CmdwE0800008; //最大费率数
static C_CmdwE0800009 CmdwE0800009; //测量点对应采集终端地址
static C_CmdwE080000A CmdwE080000A; //测量点端口号
static C_CmdwE080000B CmdwE080000B; //端口参数
static C_CmdwE080000C CmdwE080000C; //CT变比
static C_CmdwE080000D CmdwE080000D; //PT变比
static C_CmdwE080000F CmdwE080000F; //以上数据项集合
static C_CmdwE0800010 CmdwE0800010; //测量点接线方式
static C_CmdwE0800011 CmdwE0800011; //额定电压(二次侧)
static C_CmdwE0800012 CmdwE0800012; //额定电流(根据设备额定电流折算到二次侧)
static C_CmdwE0800013 CmdwE0800013; //用户申报容量(折算到二次侧)
static C_CmdwE0800014 CmdwE0800014; //变压器容量(折算到二次侧
static C_CmdwE080001F CmdwE080001F; //以上数据项集合
static C_CmdwE0800021 CmdwE0800021; //电能表通信密码，不足8字节，低位补0
static C_CmdwE0800022 CmdwE0800022; //电能表通信用户名(ASC II码)不足8字节，低位补0
static C_CmdwE080002F CmdwE080002F; //以上数据项集合
//电能质量监测参数
static C_CmdwE0800100 CmdwE0800100; //电流不平衡度阈值 缺省值三相三线30% 三相四线：50%
static C_CmdwE0800101 CmdwE0800101; //电流不平衡度恢复阈值，缺省值三相三线25% 三相四线：40%
static C_CmdwE0800102 CmdwE0800102; //合格电压上限(二次值，未设置或0时，按照缺省规则判断)，缺省值为额定电压的1.07倍
static C_CmdwE0800103 CmdwE0800103; //合格电压下限(二次值，未设置或0时，按照缺省规则判断)，缺省值为额定电压的0.93倍
static C_CmdwE0800104 CmdwE0800104; //判断电流过流的相对额定值的比例，缺省为1.3
static C_CmdwE0800105 CmdwE0800105; //判断负荷过载相对额定值的比例，缺省为1.2
static C_CmdwE0800106 CmdwE0800106; //判断负荷过载恢复的相对额定值的比例，缺省为0.95
static C_CmdwE0800107 CmdwE0800107; //零相电流报警阈值，缺省值额定电流的25%
static C_CmdwE0800108 CmdwE0800108; //零相电流报警恢复阈值，缺省值额定电流的15%
static C_CmdwE0800109 CmdwE0800109; //判断电流过负荷恢复的相对额定值的比例，缺省值为1.0
static C_CmdwE080010A CmdwE080010A; //电压不平衡度阈值
static C_CmdwE080010B CmdwE080010B; //电压不平衡度恢
static C_CmdwE080010C CmdwE080010C; //失压或断相报警的判断阈值。低于该阈值是断相，高于该阈值是失压。LL：恢复电压，缺省为额定电压的85%MM：启动电流，缺省为额定电流的10%NN：启动电压，缺省为额定电压的78%
static C_CmdwE080010F CmdwE080010F; //以上数据项集合
static C_CmdwE0800200 CmdwE0800200; //电压不平衡度恢
static C_CmdwE0800201 CmdwE0800201; //电流畸变率阈值，缺省值5%
static C_CmdwE0800202 CmdwE0800202; //电压畸变率报警恢复阈值缺省值3%
static C_CmdwE0800203 CmdwE0800203; //电流畸变率报警恢复阈值缺省值3%
static C_CmdwE0800204 CmdwE0800204; //功率因数区段数
static C_CmdwE0800205 CmdwE0800205; //功率因数区段
static C_CmdwE0800206 CmdwE0800206; //监测谐波次数 
static C_CmdwE0800207 CmdwE0800207; //电压偏差上限
static C_CmdwE0800208 CmdwE0800208; //电压偏差下限
static C_CmdwE0800209 CmdwE0800209; //频率偏差上限
static C_CmdwE080020A CmdwE080020A; //频率偏差下限
static C_CmdwE080020B CmdwE080020B; //电压长闪限值
static C_CmdwE080020C CmdwE080020C; //电压短闪限值
static C_CmdwE080020D CmdwE080020D; //无功过补偿欠补偿告警发生阈值
static C_CmdwE080020E CmdwE080020E; //无功过补偿欠补偿告警恢复阈值
//二个数据项区域
static C_CmdwE0000200 CmdwE0000200;//有效抄表通道定义 00~20
static C_CmdwE0000230 CmdwE0000230;//抄表参数配置表 30~50
static C_CmdwE0000301 CmdwE0000301;//任务参数 普通任务号 01~FE
static C_CmdwE0000401 CmdwE0000401;//任务参数 中继任务号 01~FE
static C_CmdwE0000501 CmdwE0000501;//运算量参数 模拟量号 01~FD
static C_CmdwE0000601 CmdwE0000601;//运算量参数 脉冲量号 01~FD
static C_CmdwE0000701 CmdwE0000701;//运算量参数 运算量编号 01~FD
static C_CmdwE0000801 CmdwE0000801;//运算量参数 差动规则 01~FD
static C_CmdwE0000A00 CmdwE0000A00;//端口参数 485端口定义 01~1E

static C_CmdwE0000C00 CmdwE0000C00;//负控参数 开关接点类型 00~0F
//static C_CmdwE0000C10 CmdwE0000C10;//(只读)负控参数 输入开关状态 10~1F
static C_CmdwE0000C20 CmdwE0000C20;//(只写)负控参数 输出开关状态 20~2F
static C_CmdwE0000C30 CmdwE0000C30;//负控参数 输出开关轮次 30~3F
static C_CmdwE0000D01 CmdwE0000D01;//功率控制系列1，时段(1-8) 01~08
static C_CmdwE0000D11 CmdwE0000D11;//功率控制系列2，时段(1-8) 11~18
static C_CmdwE0000D21 CmdwE0000D21;//功率控制系列3，时段(1-8) 21~28
static C_CmdwE0000D31 CmdwE0000D31;//功率控制系列4，时段(1-8) 31~38

//由于参数对象太多，采用数组存储，再用二分查找法查询
static S_CmdObjPara sCmdObjParametersW[] =
{//按DI值从小到大排序
	{0xE0000100, &CmdwE0000100},//主站参数
	{0xE0000101, &CmdwE0000101},//主站参数备用1
	{0xE0000102, &CmdwE0000102},//主站参数备用2
	{0xE0000103, &CmdwE0000103},//网关
	{0xE0000104, &CmdwE0000104},//APN
	{0xE0000105, &CmdwE0000105},//APN用户
	{0xE0000106, &CmdwE0000106},//APN密码
	{0xE0000107, &CmdwE0000107},//心跳周期
	{0xE0000108, &CmdwE0000108},//掉线重拨间隔
	{0xE0000109, &CmdwE0000109},//掉线后重拨次数
	{0xE000010A, &CmdwE000010A},//连接方式 0：TCP方式；1：UDP方式
	{0xE000010B, &CmdwE000010B},//压缩开关:0：非压缩  1：压缩，BIN编码
	{0xE000010F, &CmdwE000010F},//以上数据项集合

	{0xE0000120, &CmdwE0000120},//终端地市区县码
	{0xE0000121, &CmdwE0000121},//终端地址
	{0xE0000122, &CmdwE0000122},//终端通信工作模式
	{0xE0000123, &CmdwE0000123},//混合模式和服务器模式下，TCP监听端口号，BIN编码
	{0xE0000124, &CmdwE0000124},//终端当前通道IP地址
	{0xE0000125, &CmdwE0000125},//终端当前通道子网掩码地址
	{0xE0000126, &CmdwE0000126},//终端当前通道网关地址
	{0xE0000127, &CmdwE0000127},//有线网络IP地址获取方式
	{0xE000012F, &CmdwE000012F},//以上数据项集合
	
	{0xE0000130, &CmdwE0000130},//终端时间
	{0xE0000131, &CmdwE0000131},//(只写)实时校时

	{0xE0000140, &CmdwE0000140},//(只写)终端时间

	{0xE0000150, &CmdwE0000150},//告警主动上报屏蔽字
	{0xE0000151, &CmdwE0000151},//告警判断屏蔽字
	{0xE0000152, &CmdwE0000152},//事件记录屏蔽字

	{0xE0000160, &CmdwE0000160},//用于月冻结电量和电压合格率冻结等
	{0xE0000161, &CmdwE0000161},//用于日电量冻结和电压合格率等
	{0xE0000162, &CmdwE0000162},//月电压合格率数据冻结
	{0xE0000163, &CmdwE0000163},//日电压合格率数据冻结

	{0xE0000170, &CmdwE0000170},//(只写)消息认证码字段PW校验方式
	{0xE0000171, &CmdwE0000171},//认证请求信息，BIN编码，密文
	{0xE0000172, &CmdwE0000172},//认证响应信息，BIN编码，密文
	{0xE0000173, &CmdwE0000173},//随机数信息，BIN编码，密文
	{0xE0000174, &CmdwE0000174},//随机数响应信息，BIN编码，密文

	{0xE0000180, &CmdwE0000180},//判断负荷过载、电流过负荷、过电压、超合同容量用电、功率超定值、电流不平衡度、电压畸变、电流畸变、零序电流偏大、无功欠补偿、无功过补偿的时间、缺省15分钟
	{0xE0000181, &CmdwE0000181},//判断负荷过载、电流过负荷、过电压、超合同容量用电、功率超定值、电流不平衡度、电压畸变、电流畸变、零序电流偏大、无功欠补偿、无功过补偿恢复时间
	{0xE0000182, &CmdwE0000182},//判断断相、失压、失流时间
	{0xE0000183, &CmdwE0000183},//判断断相、失压、失流恢复时间
	{0xE0000184, &CmdwE0000184},//判断电压逆相序时间
	{0xE0000185, &CmdwE0000185},//判断电压逆相序恢复时间
	{0xE0000186, &CmdwE0000186},//判断电流反极性时间
	{0xE0000187, &CmdwE0000187},//判断电流反极性恢复时间
	{0xE0000188, &CmdwE0000188},//电能表时钟异常阈值
	{0xE0000189, &CmdwE0000189},//电能表飞走阈值
	{0xE000018A, &CmdwE000018A},//电能表停走阈值
	{0xE000018B, &CmdwE000018B},//剩余金额不足阈值
	{0xE000018C, &CmdwE000018C},//月通信流量越限值
	{0xE000018F, &CmdwE000018F},//以上数据集合

	//{0xE0000221, &CmdwE0000221},//(只读)有效的抄表参数配置表总数
	{0xE0000222, &CmdwE0000222},//具体表属性对应的抄表参数配置表

	//{0xE0000300, &CmdwE0000300},//(只读)现有普通任务总数

	//{0xE0000400, &CmdwE0000400},//(只读)现有中继任务总数

	{0xE0000500, &CmdwE0000500},//模拟量数量
	{0xE00005FE, &CmdwE00005FE},//模拟量状态标记
	{0xE00005FF, &CmdwE00005FF},//以上数据项集合

	{0xE0000600, &CmdwE0000600},//脉冲量数量
	{0xE00006FE, &CmdwE00006FE},//脉冲量状态标记
	{0xE00006FF, &CmdwE00006FF},//以上数据项集合

	{0xE0000700, &CmdwE0000700},//运算量数量
	{0xE00007FE, &CmdwE00007FE},//运算量状态标记
	{0xE00007FF, &CmdwE00007FF},//以上数据项集合

	{0xE0000800, &CmdwE0000800},//差动规则数量
	{0xE00008FE, &CmdwE00008FE},//差动规则状态标记
	{0xE00008FF, &CmdwE00008FF},//以上数据项集合

	{0xE0000900, &CmdwE0000900},//终端无功电量统计标志

	{0xE0000A20, &CmdwE0000A20},//级联从终端地址1~4
	{0xE0000A21, &CmdwE0000A21},//主终端地址
	{0xE0000A22, &CmdwE0000A22},//被抄终端地址
	{0xE0000A23, &CmdwE0000A23},//主终端等待从终端应答的超时判定时间
	{0xE0000A24, &CmdwE0000A24},//从终端等待主终端级联控制命令的超时判定时间
	{0xE0000A25, &CmdwE0000A25},//级联台区终端CT倍率
	{0xE0000A26, &CmdwE0000A26},//级联台区终端PT倍率

	//{0xE0000A30, &CmdwE0000A30},//(只读)轮询
	//{0xE0000A31, &CmdwE0000A31},//(只读)传输控制
	//{0xE0000A32, &CmdwE0000A32},//(只读)回复传输控制命令
	//{0xE0000A33, &CmdwE0000A33},//(只读)抄读终端台区总电量

	//{0xE0000B00, &CmdwE0000B00},//(只读)规约版本号
	//{0xE0000B01, &CmdwE0000B01},//(只读)终端软件版本号
	//{0xE0000B02, &CmdwE0000B02},//(只读)终端硬件版本号

	{0xE0000C40, &CmdwE0000C40},//遥控拉闸时间
	{0xE0000C50, &CmdwE0000C50},//保电状态：01保电状态，00非保电状态
	{0xE0000C60, &CmdwE0000C60},//功率控制是否有效
	{0xE0000C61, &CmdwE0000C61},//月度电量控制是否有效 
	{0xE0000C62, &CmdwE0000C62},//购电量控制是否有效 
	{0xE0000C63, &CmdwE0000C63},//功率控制各轮次跳闸是否有效，
	{0xE0000C64, &CmdwE0000C64},//月电量控制跳闸轮次有效标志，
	{0xE0000C65, &CmdwE0000C65},//购电量控制跳闸轮次有效标志，
	{0xE0000C66, &CmdwE0000C66},//超过设定值跳闸的过负荷持续时间，	缺省10分钟
	{0xE0000C67, &CmdwE0000C67},//超过设定值跳闸恢复的负荷恢复持续时间，缺省10分钟
	{0xE0000C68, &CmdwE0000C68},//拉闸命令告警延时，缺省10分钟
	{0xE0000C69, &CmdwE0000C69},//功率控制用功率累加标志
	{0xE0000C6A, &CmdwE0000C6A},//电量控制用电量累加标志
	{0xE0000C6B, &CmdwE0000C6B},//功率定值浮动系数（％）缺省为0
	{0xE0000C6C, &CmdwE0000C6C},//月电量控定值浮动系数（％）缺省为00
	{0xE0000C6D, &CmdwE0000C6D},//最低负荷
	{0xE0000C70, &CmdwE0000C70}, //X临时限电控有效标识
	{0xE0000C71, &CmdwE0000C71}, //临时限电的计算滑窗时间，单位分钟
	{0xE0000D00, &CmdwE0000D00}, //功率控制功率定值系列1,时段数
	{0xE0000D09, &CmdwE0000D09}, //功率控制系列1,执行时间
	{0xE0000D0F, &CmdwE0000D0F}, //功率控制系列1，以上数据项集合
	{0xE0000D10, &CmdwE0000D10}, //功率控制功率定值系列2,时段数
	{0xE0000D19, &CmdwE0000D19}, //功率控制系列2,执行时间
	{0xE0000D1F, &CmdwE0000D1F}, //功率控制系列2，以上数据项集合
	{0xE0000D20, &CmdwE0000D20}, //功率控制功率定值系列3,时段数
	{0xE0000D29, &CmdwE0000D29}, //功率控制系列3,执行时间
	{0xE0000D2F, &CmdwE0000D2F}, //功率控制系列3，以上数据项集合
	{0xE0000D30, &CmdwE0000D30}, //功率控制功率定值系列4,时段数
	{0xE0000D39, &CmdwE0000D39}, //功率控制系列4,执行时间
	{0xE0000D3F, &CmdwE0000D3F}, //功率控制系列4，以上数据项集合
	{0xE0000E00, &CmdwE0000E00}, //月电量控制，月电量限额
	{0xE0000E01, &CmdwE0000E01}, //月电量控制，轮次电量
	{0xE0000E02, &CmdwE0000E02}, //购电量控制，本次购电量
	{0xE0000E03, &CmdwE0000E03}, //购电量控制，剩余电量
	{0xE0000E04, &CmdwE0000E04}, //终端电能量费率时段和费率数
	{0xE0000E05, &CmdwE0000E05}, //
	{0xE0000E10, &CmdwE0000E10}, //厂休控是否投入
	{0xE0000E11, &CmdwE0000E11}, //厂休控参数

	{0xE0001100, &CmdwE0001100},//(只写)对电能表遥控拉闸，只有信息点标识和测量点地址相匹配才允许操作。
	{0xE0001101, &CmdwE0001101},//(只写)对电能表遥控合闸，只有信息点标识和测量点地址相匹配才允许操作。
	{0xE0001102, &CmdwE0001102},//(只写)对电能表保电操作，只有信息点标识和测量点地址相匹配才允许操作
	{0xE0001103, &CmdwE0001103},//(只写)对电能表保电解除操作，只有信息点标识和测量点地址相匹配才允许操作。
	{0xE0001104, &CmdwE0001104},//(只写)删除测量点

	{0xE0002000, &CmdwE0002000},//用户自定义数据	

	{0xE0800000, &CmdwE0800000}, //测量点的状态 
	{0xE0800001, &CmdwE0800001}, //测量点性质
	{0xE0800002, &CmdwE0800002}, //测量点地址
	{0xE0800003, &CmdwE0800003}, //测量点通信规约
	{0xE0800004, &CmdwE0800004}, //电能表类型
	{0xE0800005, &CmdwE0800005}, //总分类型
	{0xE0800006, &CmdwE0800006}, //重点用户属性
	{0xE0800007, &CmdwE0800007}, //拉闸功能
	{0xE0800008, &CmdwE0800008}, //最大费率数
	{0xE0800009, &CmdwE0800009}, //测量点对应采集终端地址
	{0xE080000A, &CmdwE080000A}, //测量点端口号
	{0xE080000B, &CmdwE080000B}, //端口参数
	{0xE080000C, &CmdwE080000C}, //CT变比
	{0xE080000D, &CmdwE080000D}, //PT变比
	{0xE080000F, &CmdwE080000F}, //以上数据项集合
	{0xE0800010, &CmdwE0800010}, //测量点接线方式
	{0xE0800011, &CmdwE0800011}, //额定电压(二次侧)
	{0xE0800012, &CmdwE0800012}, //额定电流
	{0xE0800013, &CmdwE0800013}, //用户申报容量(折算到二次侧)
	{0xE0800014, &CmdwE0800014}, //变压器容量(折算到二次侧)
	{0xE080001F, &CmdwE080001F}, //以上数据项集合
	{0xE0800021, &CmdwE0800021}, //电能表通信密码
	{0xE0800022, &CmdwE0800022}, //电能表通信用户名
	{0xE080002F, &CmdwE080002F}, //以上数据集合

	{0xE0800100, &CmdwE0800100}, //电流不平衡度阈值
	{0xE0800101, &CmdwE0800101}, //电流不平衡度恢复阈值
	{0xE0800102, &CmdwE0800102}, //合格电压上限
	{0xE0800103, &CmdwE0800103}, //合格电压下限
	{0xE0800104, &CmdwE0800104}, //判断电流过流的相对额定值的比例
	{0xE0800105, &CmdwE0800105}, //判断负荷过载相对额定值的比例
	{0xE0800106, &CmdwE0800106}, //判断负荷过载恢复的相对额定值的比例
	{0xE0800107, &CmdwE0800107}, //零相电流报警阈值
	{0xE0800108, &CmdwE0800108}, //零相电流报警恢复阈值
	{0xE0800109, &CmdwE0800109}, //判断电流过负荷恢复的相对额定值
	{0xE080010A, &CmdwE080010A}, //电压不平衡度阈值
	{0xE080010B, &CmdwE080010B}, //电压不平衡度恢复阈值
	{0xE080010C, &CmdwE080010C}, //失压或断相报警的判断阈值
	{0xE080010F, &CmdwE080010F}, //以上数据项集合

	{0xE0800200, &CmdwE0800200}, //电压畸变率阈值，缺省值5%
	{0xE0800201, &CmdwE0800201}, //电流畸变率阈值，缺省值5%
	{0xE0800202, &CmdwE0800202}, //电压畸变率报警恢复阈值
	{0xE0800203, &CmdwE0800203}, //电流畸变率报警恢复阈值
	{0xE0800204, &CmdwE0800204}, //功率因数区段数
	{0xE0800205, &CmdwE0800205}, //功率因数区段
	{0xE0800206, &CmdwE0800206}, //监测谐波次数
	{0xE0800207, &CmdwE0800207}, //电压偏差上限
	{0xE0800208, &CmdwE0800208}, //电压偏差下限
	{0xE0800209, &CmdwE0800209}, //频率偏差上限
	{0xE080020A, &CmdwE080020A}, //频率偏差下限
	{0xE080020B, &CmdwE080020B}, //电压长闪限值
	{0xE080020C, &CmdwE080020C}, //电压短闪限值
	{0xE080020D, &CmdwE080020D},  //无功过补偿欠补偿告警发生阈值
	{0xE080020E, &CmdwE080020E},  //无功过补偿欠补偿告警恢复阈值

};

static S_CmdObjPara3D sCmdObjParametersW_3D[] =//二个数据项
{//按DI值从小到大排序
	{0xE0000200, 0xE0000220, &CmdwE0000200},//抄表参数 00~20
	{0xE0000230, 0xE0000250, &CmdwE0000230},//抄表参数配置表 30~50
	{0xE0000301, 0xE00003FE, &CmdwE0000301},//任务参数 普通任务号
	{0xE0000401, 0xE00004FE, &CmdwE0000401},//任务参数 中继任务号
	{0xE0000501, 0xE00005FD, &CmdwE0000501},//运算量参数 模拟量号
	{0xE0000601, 0xE00006FD, &CmdwE0000601},//运算量参数 脉冲量号
	{0xE0000701, 0xE00007FD, &CmdwE0000701},//运算量参数 运算量编号
	{0xE0000801, 0xE00008FD, &CmdwE0000801},//运算量参数 差动规则
	{0xE0000A00, 0xE0000A1E, &CmdwE0000A00},//端口参数 485端口定义

	{0xE0000C00, 0xE0000C0F, &CmdwE0000C00},//负控参数 开关接点类型
	//{0xE0000C10, 0xE0000C1F, &CmdwE0000C10},//(只读)负控参数 输出开关状态
	{0xE0000C20, 0xE0000C2F, &CmdwE0000C20},//(只写)负控参数 输出开关状态
	{0xE0000C30, 0xE0000C3F, &CmdwE0000C30},//负控参数 输出开关轮次
	{0xE0000D01, 0xE0000D08, &CmdwE0000D01},//功率控制系列1，时段(1-8)
	{0xE0000D11, 0xE0000D18, &CmdwE0000D11},//功率控制系列2，时段(1-8)
	{0xE0000D21, 0xE0000D28, &CmdwE0000D21},//功率控制系列3，时段(1-8)
	{0xE0000D31, 0xE0000D38, &CmdwE0000D31},//功率控制系列4，时段(1-8)

	{0xFFFFFFFF, 0xFFFFFFFF, NULL},//end
};

int C_AFNCMD_WPARAM::Execute(const S_CmdObjs &CmdObjs, strings &strResponseInfos)
{
	if (CmdObjs.empty())
		return FRAME_PROCESS_RESPONSE_DENY;

	string strParamChange;
	for (S_CmdObjs::const_iterator iterNow = CmdObjs.begin(); iterNow != CmdObjs.end(); advance(iterNow, 1))
	{
		C_CmdReceiver *pCmdReveiver = GetCmdReceiver(iterNow->m_DI);
		if (pCmdReveiver == NULL)
			return FRAME_PROCESS_RESPONSE_DENY;

		string strItem;
		strings strResponse;
		if (pCmdReveiver->Prompt() != NULL)
			logit<<"命令功能:"<<pCmdReveiver->Prompt()<<endl;
		pCmdReveiver->SetPackageLength(m_PackageLength);
		pCmdReveiver->AppendDADT(strItem, *iterNow);
		if (pCmdReveiver->Action(*iterNow, strResponse) == false)
		{
			strItem.push_back(0x02); // err 数据项不支持
		}
		else
		{
			strItem.push_back(0x00); // err 正确
			if (iterNow->m_DA == 0x00) // 记录终端参数变更
				strParamChange.append((char *)&iterNow->m_DI, 4);
		}

		strResponseInfos.push_back(strItem);
		// 终端参数变更事件
		if (strParamChange.size() >= 200) // 存文件
		{
			CEventRecrod_ERD8::EventRecord(0xE2010014, C_TIME::TimeNow(), strParamChange);
			strParamChange.clear();
		}
	}

	// 刷新测量点参数
	UpdateMeterInfo();
	if (strParamChange.empty() == false)
		CEventRecrod_ERD8::EventRecord(0xE2010014, C_TIME::TimeNow(), strParamChange);
	return FRAME_PROCESS_RESPONSE_DATA;
}

C_CmdReceiver *C_AFNCMD_WPARAM::GetCmdReceiver(INT32U DI)
{
	const S_CmdObjPara CmdObjNow = {DI, NULL};
	S_CmdObjPara *pFindObj = lower_bound(sCmdObjParametersW,
		sCmdObjParametersW+sizeof(sCmdObjParametersW)/sizeof(sCmdObjParametersW[0]), CmdObjNow);
	if (pFindObj != sCmdObjParametersW+sizeof(sCmdObjParametersW)/sizeof(sCmdObjParametersW[0])
		&& pFindObj->m_DI == DI)
		return pFindObj->m_pCmdReceiver;

	const S_CmdObjPara3D CmdObjNow3D = {DI, DI, NULL};
	S_CmdObjPara3D *pFindObj3D = lower_bound(sCmdObjParametersW_3D,
		sCmdObjParametersW_3D+sizeof(sCmdObjParametersW_3D)/sizeof(sCmdObjParametersW_3D[0]), CmdObjNow3D);
	if (pFindObj3D != sCmdObjParametersW_3D+sizeof(sCmdObjParametersW_3D)/sizeof(sCmdObjParametersW_3D[0]))
	{
		if (DI >= pFindObj3D->m_diBegin && DI <= pFindObj3D->m_diEnd)
			return pFindObj3D->m_pCmdReceiver;
		if (pFindObj3D != sCmdObjParametersW_3D)
			pFindObj3D -= 1;
		if (DI >= pFindObj3D->m_diBegin && DI <= pFindObj3D->m_diEnd)
			return pFindObj3D->m_pCmdReceiver;
	}
	return NULL;
}

bool C_AFNCMD_WPARAM::UpdateMeterInfo(void)
{
	C_WMCmdReceiver WMCmdReceiver;
	return WMCmdReceiver.UpdateInformation();
}



