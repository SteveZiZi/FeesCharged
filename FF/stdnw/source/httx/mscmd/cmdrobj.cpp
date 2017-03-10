/** @file
 *  @brief 后台通讯命令读参数对象定义及查找
 *  @author  zhongguangxiong
 *  @date 2012/02/18
 *  @version 0.1
 */
#include "cmdrobj.h"
#include "cmdrpara.h"
#include "cmdrctrl.h"
#include "cmdrmp.h"

//对象参数定义
//通信参数
static C_CmdrE0000100 CmdrE0000100; //主站参数
static C_CmdrE0000101 CmdrE0000101; //主站参数备用1
static C_CmdrE0000102 CmdrE0000102; //主站参数备用2
static C_CmdrE0000103 CmdrE0000103; //网关
static C_CmdrE0000104 CmdrE0000104; //APN
static C_CmdrE0000105 CmdrE0000105; //APN用户
static C_CmdrE0000106 CmdrE0000106; //APN密码
static C_CmdrE0000107 CmdrE0000107; //心跳周期
static C_CmdrE0000108 CmdrE0000108; //掉线重拨间隔
static C_CmdrE0000109 CmdrE0000109; //掉线后重拨次数，缺省3次,重拨指定次数后，当天不再重拨
static C_CmdrE000010A CmdrE000010A; //连接方式 0：TCP方式；1：UDP方式
static C_CmdrE000010B CmdrE000010B; //压缩开关:0：非压缩  1：压缩，BIN编?
static C_CmdrE000010F CmdrE000010F; //以上数据项集合

static C_CmdrE0000120 CmdrE0000120; //终端地市区县码
static C_CmdrE0000121 CmdrE0000121; //终端地址
static C_CmdrE0000122 CmdrE0000122; //终端通信工作模式
static C_CmdrE0000123 CmdrE0000123; //混合模式和服务器模式下，TCP监听端口号，BIN编码
static C_CmdrE0000124 CmdrE0000124; //终端当前通道IP地址
static C_CmdrE0000125 CmdrE0000125; //终端当前通道子网掩码地址
static C_CmdrE0000126 CmdrE0000126; //终端当前通道网关地址
static C_CmdrE0000127 CmdrE0000127; //有线网络IP地址获取方式 0：手动设置IP地址 1：自动设置IP地址
static C_CmdrE000012F CmdrE000012F; //以上数据项集合
//时间参数
static C_CmdrE0000130 CmdrE0000130; //终端时间
//static C_CmdrE0000131 CmdrE0000131; //实时校时 (只写)
//复位操作
//static C_CmdrE0000140 CmdrE0000140; //终端复位 (只写)
//告警屏蔽
static C_CmdrE0000150 CmdrE0000150; //告警主动上报屏蔽字
static C_CmdrE0000151 CmdrE0000151; //告警判断屏蔽字
static C_CmdrE0000152 CmdrE0000152; //事件记录屏蔽字
//结算日参数
static C_CmdrE0000160 CmdrE0000160; //结算时间的日、小时（用于月冻结电量和电压合格率冻结等） 月冻结电量
static C_CmdrE0000161 CmdrE0000161; //日电能量（表码）结算时刻（用于日电量冻结和电压合格率等） 日冻结电量
static C_CmdrE0000162 CmdrE0000162; //月电压合格率数据冻结
static C_CmdrE0000163 CmdrE0000163; //日电压合格率数据冻结
//身份认证
//static C_CmdrE0000170 CmdrE0000170; //消息认证码字段PW校验方式 (只写)
static C_CmdrE0000171 CmdrE0000171; //认证请求信息，BIN编码，密文
static C_CmdrE0000172 CmdrE0000172; //认证响应信息，BIN编码，密文
static C_CmdrE0000173 CmdrE0000173; //随机数信息，BIN编码，密文
static C_CmdrE0000174 CmdrE0000174; //随机数响应信息，BIN编码，密文
//阈值参数
static C_CmdrE0000180 CmdrE0000180;//判断负荷过载、电流过负荷、过电压、超合同容量用电、功率超定值、电流不平衡度、电压畸变、电流畸变、零序电流偏大、无功欠补偿、无功过补偿的时间、缺省15分钟
static C_CmdrE0000181 CmdrE0000181;//判断负荷过载、电流过负荷、过电压、超合同容量用电、功率超定值、电流不平衡度、电压畸变、电流畸变、零序电流偏大、无功欠补偿、无功过补偿恢复时间、缺省15分钟
static C_CmdrE0000182 CmdrE0000182;//判断断相、失压、失流时间，缺省值1分钟
static C_CmdrE0000183 CmdrE0000183;//判断断相、失压、失流恢复时间，缺省值1分钟
static C_CmdrE0000184 CmdrE0000184;//判断电压逆相序时间，缺省值15分钟
static C_CmdrE0000185 CmdrE0000185;//判断电压逆相序恢复时间，缺省值15分钟
static C_CmdrE0000186 CmdrE0000186;//判断电流反极性时间，缺省值15分钟
static C_CmdrE0000187 CmdrE0000187;//判断电流反极性恢复时间，缺省值15分钟
static C_CmdrE0000188 CmdrE0000188;//电能表时钟异常阈值，缺省值10分钟
static C_CmdrE0000189 CmdrE0000189;//电能表飞走阈值，缺省为10倍
static C_CmdrE000018A CmdrE000018A;//电能表停走阈值 缺省为0.1kWh
static C_CmdrE000018B CmdrE000018B;//剩余金额不足阈值：缺省为20.00 元
static C_CmdrE000018C CmdrE000018C;//月通信流量越限值
static C_CmdrE000018F CmdrE000018F;//以上数据集合
//抄表参数
static C_CmdrE0000221 CmdrE0000221;//有效的抄表参数配置表总数 (只读)
static C_CmdrE0000222 CmdrE0000222;//具体表属性对应的抄表参数配置表
//任务参数
static C_CmdrE0000300 CmdrE0000300;//现有普通任务总数 (只读)

static C_CmdrE0000400 CmdrE0000400;//现有中继任务总数 (只读)
//运算量参数
static C_CmdrE0000500 CmdrE0000500;//模拟量数量
static C_CmdrE00005FE CmdrE00005FE;//模拟量状态标记
static C_CmdrE00005FF CmdrE00005FF;//以上数据集合
//脉冲量参数
static C_CmdrE0000600 CmdrE0000600;//脉冲量数量
static C_CmdrE00006FE CmdrE00006FE;//脉冲量状态标记
static C_CmdrE00006FF CmdrE00006FF;//以上数据集合
//运算量参数
static C_CmdrE0000700 CmdrE0000700;//运算量数量
static C_CmdrE00007FE CmdrE00007FE;//运算量状态标记
static C_CmdrE00007FF CmdrE00007FF;//以上数据集合
//差动参数
static C_CmdrE0000800 CmdrE0000800;//差动规则数量
static C_CmdrE00008FE CmdrE00008FE;//差动规则状态标记
static C_CmdrE00008FF CmdrE00008FF;//以上数据集合

static C_CmdrE0000900 CmdrE0000900;//终端无功电量统计标志
//端口参数
//级联通信参数
static C_CmdrE0000A20 CmdrE0000A20;//级联从终端地址1~4
static C_CmdrE0000A21 CmdrE0000A21;//主终端地址
static C_CmdrE0000A22 CmdrE0000A22;//被抄终端地址
static C_CmdrE0000A23 CmdrE0000A23;//主终端等待从终端应答的超时判定时间
static C_CmdrE0000A24 CmdrE0000A24;//从终端等待主终端级联控制命令的超时判定时间
static C_CmdrE0000A25 CmdrE0000A25;//级联台区终端CT倍率
static C_CmdrE0000A26 CmdrE0000A26;//级联台区终端PT倍率
//级联控制
static C_CmdrE0000A30 CmdrE0000A30;//轮询 (只读)
static C_CmdrE0000A31 CmdrE0000A31;//传输控制 (只读)
static C_CmdrE0000A32 CmdrE0000A32;//回复传输控制命令 (只读)
static C_CmdrE0000A33 CmdrE0000A33;//抄读终端台区总电量 (只读)
//版本参数CmdrE0000B00
static C_CmdrE0000B00 CmdrE0000B00;//规约版本号，本规约规定为0100  (只读)
static C_CmdrE0000B01 CmdrE0000B01;//终端软件版本号  (只读)
static C_CmdrE0000B02 CmdrE0000B02;//终端硬件版本号  (只读)
//负控参数
static C_CmdrE0000C40 CmdrE0000C40;//遥控拉闸时间
static C_CmdrE0000C50 CmdrE0000C50;//保电状态：01保电状态，00非保电状态
static C_CmdrE0000C60 CmdrE0000C60;//功率控制是否有效
static C_CmdrE0000C61 CmdrE0000C61;//月度电量控制是否有效 
static C_CmdrE0000C62 CmdrE0000C62;//购电量控制是否有效 
static C_CmdrE0000C63 CmdrE0000C63;//功率控制各轮次跳闸是否有效，
static C_CmdrE0000C64 CmdrE0000C64;//月电量控制跳闸轮次有效标志，
static C_CmdrE0000C65 CmdrE0000C65;//购电量控制跳闸轮次有效标志，
static C_CmdrE0000C66 CmdrE0000C66;//超过设定值跳闸的过负荷持续时间，	缺省10分钟
static C_CmdrE0000C67 CmdrE0000C67;//超过设定值跳闸恢复的负荷恢复持续时间，缺省10分钟
static C_CmdrE0000C68 CmdrE0000C68;//拉闸命令告警延时，缺省10分钟
static C_CmdrE0000C69 CmdrE0000C69;//功率控制用功率累加标志
static C_CmdrE0000C6A CmdrE0000C6A;//电量控制用电量累加标志
static C_CmdrE0000C6B CmdrE0000C6B;//功率定值浮动系数（％）缺省为0
static C_CmdrE0000C6C CmdrE0000C6C;//月电量控定值浮动系数（％）缺省为00
static C_CmdrE0000C6D CmdrE0000C6D;//最低负荷
static C_CmdrE0000C70 CmdrE0000C70;//X临时限电控有效标识
static C_CmdrE0000C71 CmdrE0000C71;//临时限电的计算滑窗时间，单位分钟

static C_CmdrE0000D00 CmdrE0000D00;//功率控制系列1，时段数
static C_CmdrE0000D09 CmdrE0000D09;//功率控制系列1,执行时间
static C_CmdrE0000D0F CmdrE0000D0F;//功率控制系列1，以上数据合集
static C_CmdrE0000D10 CmdrE0000D10;//功率控制功率定值系列2,时段数
static C_CmdrE0000D19 CmdrE0000D19;//功率控制系列2,执行时间
static C_CmdrE0000D1F CmdrE0000D1F;//功率控制系列2，以上数据合集
static C_CmdrE0000D20 CmdrE0000D20;//功率控制功率定值系列3,时段数
static C_CmdrE0000D29 CmdrE0000D29;//功率控制系列3,执行时间
static C_CmdrE0000D2F CmdrE0000D2F;//功率控制系列3，以上数据合集
static C_CmdrE0000D30 CmdrE0000D30;//功率控制功率定值系列4,时段数
static C_CmdrE0000D39 CmdrE0000D39;//功率控制系列4,执行时间
static C_CmdrE0000D3F CmdrE0000D3F;//功率控制系列4，以上数据合集
static C_CmdrE0000E00 CmdrE0000E00;//月电量控制，月电量限额
static C_CmdrE0000E01 CmdrE0000E01;//月电量控制，轮次电量
static C_CmdrE0000E02 CmdrE0000E02;//购电量控制，本次购电量
static C_CmdrE0000E03 CmdrE0000E03;//购电量控制，剩余电量
static C_CmdrE0000E04 CmdrE0000E04;//终端电能量费率时段和费率数
static C_CmdrE0000E05 CmdrE0000E05;//
static C_CmdrE0000E10 CmdrE0000E10;//厂休控是否投入
static C_CmdrE0000E11 CmdrE0000E11;//厂休控参数
/* 不读不写
//链路接口数据标识
static C_CmdrE0001000 CmdrE0001000;//表示终端登录，NNNN：规约版本号 
static C_CmdrE0001001 CmdrE0001001;//表示终端心跳，无数据体
static C_CmdrE0001002 CmdrE0001002;//表示登录退出，无数据体 
*/
//测量点控制参数
//static C_CmdrE0001100 CmdrE0001100;//对电能表遥控拉闸，只有信息点标识和测量点地址相匹配才允许操作 (只写)
//static C_CmdrE0001101 CmdrE0001101;//对电能表遥控合闸，只有信息点标识和测量点地址相匹配才允许操作 (只写)
//static C_CmdrE0001102 CmdrE0001102;//对电能表保电操作，只有信息点标识和测量点地址相匹配才允许操作 (只写)
//static C_CmdrE0001103 CmdrE0001103;//对电能表保电解除操作，只有信息点标识和测量点地址相匹配才允许操作 (只写)
//static C_CmdrE0001104 CmdrE0001104;//删除测量点NNNN表示待删除测量点号 
//用户自定义数据
static C_CmdrE0002000 CmdrE0002000;//用户自定义数据CCCC：厂商编码NN……NN：厂商自定义数据
//测量点参数
static C_CmdrE0800000 CmdrE0800000; //测量点的状态 
static C_CmdrE0800001 CmdrE0800001; //测量点性质
static C_CmdrE0800002 CmdrE0800002; //测量点地址
static C_CmdrE0800003 CmdrE0800003; //测量点通信规约
static C_CmdrE0800004 CmdrE0800004; //电能表类型
static C_CmdrE0800005 CmdrE0800005; //总分类型
static C_CmdrE0800006 CmdrE0800006; //重点用户属性
static C_CmdrE0800007 CmdrE0800007; //拉闸功能
static C_CmdrE0800008 CmdrE0800008; //最大费率数
static C_CmdrE0800009 CmdrE0800009; //测量点对应采集终端地址
static C_CmdrE080000A CmdrE080000A; //测量点端口号
static C_CmdrE080000B CmdrE080000B; //端口参数
static C_CmdrE080000C CmdrE080000C; //CT变比
static C_CmdrE080000D CmdrE080000D; //PT变比
static C_CmdrE080000F CmdrE080000F; //以上数据项集合
static C_CmdrE0800010 CmdrE0800010; //测量点接线方式
static C_CmdrE0800011 CmdrE0800011; //额定电压(二次侧)
static C_CmdrE0800012 CmdrE0800012; //额定电流(根据设备额定电流折算到二次侧)
static C_CmdrE0800013 CmdrE0800013; //用户申报容量(折算到二次侧)
static C_CmdrE0800014 CmdrE0800014; //变压器容量(折算到二次侧
static C_CmdrE080001F CmdrE080001F; //以上数据项集合
static C_CmdrE0800021 CmdrE0800021; //电能表通信密码，不足8字节，低位补0
static C_CmdrE0800022 CmdrE0800022; //电能表通信用户名(ASC II码)不足8字节，低位补0
static C_CmdrE080002F CmdrE080002F; //以上数据项集合
//电能质量监测参数
static C_CmdrE0800100 CmdrE0800100; //电流不平衡度阈值 缺省值三相三线30% 三相四线：50%
static C_CmdrE0800101 CmdrE0800101; //电流不平衡度恢复阈值，缺省值三相三线25% 三相四线：40%
static C_CmdrE0800102 CmdrE0800102; //合格电压上限(二次值，未设置或0时，按照缺省规则判断)，缺省值为额定电压的1.07倍
static C_CmdrE0800103 CmdrE0800103; //合格电压下限(二次值，未设置或0时，按照缺省规则判断)，缺省值为额定电压的0.93倍
static C_CmdrE0800104 CmdrE0800104; //判断电流过流的相对额定值的比例，缺省为1.3
static C_CmdrE0800105 CmdrE0800105; //判断负荷过载相对额定值的比例，缺省为1.2
static C_CmdrE0800106 CmdrE0800106; //判断负荷过载恢复的相对额定值的比例，缺省为0.95
static C_CmdrE0800107 CmdrE0800107; //零相电流报警阈值，缺省值额定电流的25%
static C_CmdrE0800108 CmdrE0800108; //零相电流报警恢复阈值，缺省值额定电流的15%
static C_CmdrE0800109 CmdrE0800109; //判断电流过负荷恢复的相对额定值的比例，缺省值为1.0
static C_CmdrE080010A CmdrE080010A; //电压不平衡度阈值
static C_CmdrE080010B CmdrE080010B; //电压不平衡度恢
static C_CmdrE080010C CmdrE080010C; //失压或断相报警的判断阈值。低于该阈值是断相，高于该阈值是失压。LL：恢复电压，缺省为额定电压的85%MM：启动电流，缺省为额定电流的10%NN：启动电压，缺省为额定电压的78%
static C_CmdrE080010F CmdrE080010F; //以上数据项集合
static C_CmdrE0800200 CmdrE0800200; //电压不平衡度恢
static C_CmdrE0800201 CmdrE0800201; //电流畸变率阈值，缺省值5%
static C_CmdrE0800202 CmdrE0800202; //电压畸变率报警恢复阈值,缺省值3%
static C_CmdrE0800203 CmdrE0800203; //电流畸变率报警恢复阈值,缺省值3%
static C_CmdrE0800204 CmdrE0800204; //功率因数区段数
static C_CmdrE0800205 CmdrE0800205; //功率因数区段
static C_CmdrE0800206 CmdrE0800206; //监测谐波次数 
static C_CmdrE0800207 CmdrE0800207; //电压偏差上限
static C_CmdrE0800208 CmdrE0800208; //电压偏差下限
static C_CmdrE0800209 CmdrE0800209; //频率偏差上限
static C_CmdrE080020A CmdrE080020A; //频率偏差下限
static C_CmdrE080020B CmdrE080020B; //电压长闪限值
static C_CmdrE080020C CmdrE080020C; //电压短闪限值
static C_CmdrE080020D CmdrE080020D; //无功过补偿欠补偿告警发生阈值
static C_CmdrE080020E CmdrE080020E; //无功过补偿欠补偿告警恢复阈值
//二个数据项区域
static C_CmdrE0000200 CmdrE0000200;//有效抄表通道定义 00~20
static C_CmdrE0000230 CmdrE0000230;//抄表参数配置表 30~50
static C_CmdrE0000301 CmdrE0000301;//任务参数 普通任务号 01~FE
static C_CmdrE0000401 CmdrE0000401;//任务参数 中继任务号 01~FE
static C_CmdrE0000501 CmdrE0000501;//运算量参数 模拟量号 01~FD
static C_CmdrE0000601 CmdrE0000601;//运算量参数 脉冲量号 01~FD
static C_CmdrE0000701 CmdrE0000701;//运算量参数 运算量编号 01~FD
static C_CmdrE0000801 CmdrE0000801;//运算量参数 差动规则 01~FD
static C_CmdrE0000A00 CmdrE0000A00;//端口参数 485端口定义 01~1E

static C_CmdrE0000C00 CmdrE0000C00;//负控参数 开关接点类型 00~0F
static C_CmdrE0000C10 CmdrE0000C10;//(只读)负控参数 输入开关状态 10~1F
//static C_CmdrE0000C20 CmdrE0000C20;//(只写)负控参数 输出开关状态 20~2F
static C_CmdrE0000C30 CmdrE0000C30;//负控参数 输出开关轮次 30~3F
static C_CmdrE0000D01 CmdrE0000D01;//功率控制系列1，时段(1-8) 01~08
static C_CmdrE0000D11 CmdrE0000D11;//功率控制系列2，时段(1-8) 11~18
static C_CmdrE0000D21 CmdrE0000D21;//功率控制系列3，时段(1-8) 21~28
static C_CmdrE0000D31 CmdrE0000D31;//功率控制系列4，时段(1-8) 31~38

//由于参数对象太多，采用数组存储，再用二分查找法查询
static S_CmdObjPara sCmdObjParametersR[] =//一个数据项
{//按DI值从小到大排序
	{0xE0000100, &CmdrE0000100},//主站参数
	{0xE0000101, &CmdrE0000101},//主站参数备用1
	{0xE0000102, &CmdrE0000102},//主站参数备用2
	{0xE0000103, &CmdrE0000103},//网关
	{0xE0000104, &CmdrE0000104},//APN
	{0xE0000105, &CmdrE0000105},//APN用户
	{0xE0000106, &CmdrE0000106},//APN密码
	{0xE0000107, &CmdrE0000107},//心跳周期
	{0xE0000108, &CmdrE0000108},//掉线重拨间隔
	{0xE0000109, &CmdrE0000109},//掉线后重拨次数
	{0xE000010A, &CmdrE000010A},//连接方式 0：TCP方式；1：UDP方式
	{0xE000010B, &CmdrE000010B},//压缩开关:0：非压缩  1：压缩，BIN编码
	{0xE000010F, &CmdrE000010F},//以上数据项集合

	{0xE0000120, &CmdrE0000120},//终端地市区县码
	{0xE0000121, &CmdrE0000121},//终端地址
	{0xE0000122, &CmdrE0000122},//终端通信工作模式
	{0xE0000123, &CmdrE0000123},//混合模式和服务器模式下，TCP监听端口号，BIN编码
	{0xE0000124, &CmdrE0000124},//终端当前通道IP地址
	{0xE0000125, &CmdrE0000125},//终端当前通道子网掩码地址
	{0xE0000126, &CmdrE0000126},//终端当前通道网关地址
	{0xE0000127, &CmdrE0000127},//有线网络IP地址获取方式
	{0xE000012F, &CmdrE000012F},//以上数据项集合
	
	{0xE0000130, &CmdrE0000130},//终端时间
	//{0xE0000131, &CmdrE0000131},//(只写)实时校时

	//{0xE0000140, &CmdrE0000140},//(只写)终端时间

	{0xE0000150, &CmdrE0000150},//告警主动上报屏蔽字
	{0xE0000151, &CmdrE0000151},//告警判断屏蔽字
	{0xE0000152, &CmdrE0000152},//事件记录屏蔽字

	{0xE0000160, &CmdrE0000160},//用于月冻结电量和电压合格率冻结等
	{0xE0000161, &CmdrE0000161},//用于日电量冻结和电压合格率等
	{0xE0000162, &CmdrE0000162},//月电压合格率数据冻结
	{0xE0000163, &CmdrE0000163},//日电压合格率数据冻结

	//{0xE0000170, &CmdrE0000170},//(只写)消息认证码字段PW校验方式
	{0xE0000171, &CmdrE0000171},//认证请求信息，BIN编码，密文
	{0xE0000172, &CmdrE0000172},//认证响应信息，BIN编码，密文
	{0xE0000173, &CmdrE0000173},//随机数信息，BIN编码，密文
	{0xE0000174, &CmdrE0000174},//随机数响应信息，BIN编码，密文

	{0xE0000180, &CmdrE0000180},//判断负荷过载、电流过负荷、过电压、超合同容量用电、功率超定值、电流不平衡度、电压畸变、电流畸变、零序电流偏大、无功欠补偿、无功过补偿的时间、缺省15分钟
	{0xE0000181, &CmdrE0000181},//判断负荷过载、电流过负荷、过电压、超合同容量用电、功率超定值、电流不平衡度、电压畸变、电流畸变、零序电流偏大、无功欠补偿、无功过补偿恢复时间
	{0xE0000182, &CmdrE0000182},//判断断相、失压、失流时间
	{0xE0000183, &CmdrE0000183},//判断断相、失压、失流恢复时间
	{0xE0000184, &CmdrE0000184},//判断电压逆相序时间
	{0xE0000185, &CmdrE0000185},//判断电压逆相序恢复时间
	{0xE0000186, &CmdrE0000186},//判断电流反极性时间
	{0xE0000187, &CmdrE0000187},//判断电流反极性恢复时间
	{0xE0000188, &CmdrE0000188},//电能表时钟异常阈值
	{0xE0000189, &CmdrE0000189},//电能表飞走阈值
	{0xE000018A, &CmdrE000018A},//电能表停走阈值
	{0xE000018B, &CmdrE000018B},//剩余金额不足阈值
	{0xE000018C, &CmdrE000018C},//月通信流量越限值
	{0xE000018F, &CmdrE000018F},//以上数据集合

	{0xE0000221, &CmdrE0000221},//(只读)有效的抄表参数配置表总数
	{0xE0000222, &CmdrE0000222},//具体表属性对应的抄表参数配置表

	{0xE0000300, &CmdrE0000300},//(只读)现有普通任务总数

	{0xE0000400, &CmdrE0000400},//(只读)现有中继任务总数

	{0xE0000500, &CmdrE0000500},//模拟量数量
	{0xE00005FE, &CmdrE00005FE},//模拟量状态标记
	{0xE00005FF, &CmdrE00005FF},//以上数据项集合

	{0xE0000600, &CmdrE0000600},//脉冲量数量
	{0xE00006FE, &CmdrE00006FE},//脉冲量状态标记
	{0xE00006FF, &CmdrE00006FF},//以上数据项集合

	{0xE0000700, &CmdrE0000700},//运算量数量
	{0xE00007FE, &CmdrE00007FE},//运算量状态标记
	{0xE00007FF, &CmdrE00007FF},//以上数据项集合

	{0xE0000800, &CmdrE0000800},//差动规则数量
	{0xE00008FE, &CmdrE00008FE},//差动规则状态标记
	{0xE00008FF, &CmdrE00008FF},//以上数据项集合

	{0xE0000900, &CmdrE0000900},//终端无功电量统计标志

	{0xE0000A20, &CmdrE0000A20},//级联从终端地址1~4
	{0xE0000A21, &CmdrE0000A21},//主终端地址
	{0xE0000A22, &CmdrE0000A22},//被抄终端地址
	{0xE0000A23, &CmdrE0000A23},//主终端等待从终端应答的超时判定时间
	{0xE0000A24, &CmdrE0000A24},//从终端等待主终端级联控制命令的超时判定时间
	{0xE0000A25, &CmdrE0000A25},//级联台区终端CT倍率
	{0xE0000A26, &CmdrE0000A26},//级联台区终端PT倍率

	{0xE0000A30, &CmdrE0000A30},//(只读)轮询
	{0xE0000A31, &CmdrE0000A31},//(只读)传输控制
	{0xE0000A32, &CmdrE0000A32},//(只读)回复传输控制命令
	{0xE0000A33, &CmdrE0000A33},//(只读)抄读终端台区总电量

	{0xE0000B00, &CmdrE0000B00},//(只读)规约版本号
	{0xE0000B01, &CmdrE0000B01},//(只读)终端软件版本号
	{0xE0000B02, &CmdrE0000B02},//(只读)终端硬件版本号

	{0xE0000C40, &CmdrE0000C40},//遥控拉闸时间
	{0xE0000C50, &CmdrE0000C50},//保电状态：01保电状态，00非保电状态
	{0xE0000C60, &CmdrE0000C60},//功率控制是否有效
	{0xE0000C61, &CmdrE0000C61},//月度电量控制是否有效 
	{0xE0000C62, &CmdrE0000C62},//购电量控制是否有效 
	{0xE0000C63, &CmdrE0000C63},//功率控制各轮次跳闸是否有效，
	{0xE0000C64, &CmdrE0000C64},//月电量控制跳闸轮次有效标志，
	{0xE0000C65, &CmdrE0000C65},//购电量控制跳闸轮次有效标志，
	{0xE0000C66, &CmdrE0000C66},//超过设定值跳闸的过负荷持续时间，	缺省10分钟
	{0xE0000C67, &CmdrE0000C67},//超过设定值跳闸恢复的负荷恢复持续时间，缺省10分钟
	{0xE0000C68, &CmdrE0000C68},//拉闸命令告警延时，缺省10分钟
	{0xE0000C69, &CmdrE0000C69},//功率控制用功率累加标志
	{0xE0000C6A, &CmdrE0000C6A},//电量控制用电量累加标志
	{0xE0000C6B, &CmdrE0000C6B},//功率定值浮动系数（％）缺省为0
	{0xE0000C6C, &CmdrE0000C6C},//月电量控定值浮动系数（％）缺省为00
	{0xE0000C6D, &CmdrE0000C6D},//最低负荷
	{0xE0000C70, &CmdrE0000C70}, //X临时限电控有效标识
	{0xE0000C71, &CmdrE0000C71}, //临时限电的计算滑窗时间，单位分钟
	{0xE0000D00, &CmdrE0000D00}, //功率控制功率定值系列1,时段数
	{0xE0000D09, &CmdrE0000D09}, //功率控制系列1,执行时间
	{0xE0000D0F, &CmdrE0000D0F}, //功率控制系列1，以上数据项集合
	{0xE0000D10, &CmdrE0000D10}, //功率控制功率定值系列2,时段数
	{0xE0000D19, &CmdrE0000D19}, //功率控制系列2,执行时间
	{0xE0000D1F, &CmdrE0000D1F}, //功率控制系列2，以上数据项集合
	{0xE0000D20, &CmdrE0000D20}, //功率控制功率定值系列3,时段数
	{0xE0000D29, &CmdrE0000D29}, //功率控制系列3,执行时间
	{0xE0000D2F, &CmdrE0000D2F}, //功率控制系列3，以上数据项集合
	{0xE0000D30, &CmdrE0000D30}, //功率控制功率定值系列4,时段数
	{0xE0000D39, &CmdrE0000D39}, //功率控制系列4,执行时间
	{0xE0000D3F, &CmdrE0000D3F}, //功率控制系列4，以上数据项集合
	{0xE0000E00, &CmdrE0000E00}, //月电量控制，月电量限额
	{0xE0000E01, &CmdrE0000E01}, //月电量控制，轮次电量
	{0xE0000E02, &CmdrE0000E02}, //购电量控制，本次购电量
	{0xE0000E03, &CmdrE0000E03}, //购电量控制，剩余电量
	{0xE0000E04, &CmdrE0000E04}, //终端电能量费率时段和费率数
	{0xE0000E05, &CmdrE0000E05}, //
	{0xE0000E10, &CmdrE0000E10}, //厂休控是否投入
	{0xE0000E11, &CmdrE0000E11}, //厂休控参数

	//{0xE0001100, &CmdrE0001100},//(只写)对电能表遥控拉闸，只有信息点标识和测量点地址相匹配才允许操作。
	//{0xE0001101, &CmdrE0001101},//(只写)对电能表遥控合闸，只有信息点标识和测量点地址相匹配才允许操作。
	//{0xE0001102, &CmdrE0001102},//(只写)对电能表保电操作，只有信息点标识和测量点地址相匹配才允许操作
	//{0xE0001103, &CmdrE0001103},//(只写)对电能表保电解除操作，只有信息点标识和测量点地址相匹配才允许操作。
	//{0xE0001104, &CmdrE0001104},//(只写)删除测量点

	{0xE0002000, &CmdrE0002000},//用户自定义数据	

	{0xE0800000, &CmdrE0800000}, //测量点的状态 
	{0xE0800001, &CmdrE0800001}, //测量点性质
	{0xE0800002, &CmdrE0800002}, //测量点地址
	{0xE0800003, &CmdrE0800003}, //测量点通信规约
	{0xE0800004, &CmdrE0800004}, //电能表类型
	{0xE0800005, &CmdrE0800005}, //总分类型
	{0xE0800006, &CmdrE0800006}, //重点用户属性
	{0xE0800007, &CmdrE0800007}, //拉闸功能
	{0xE0800008, &CmdrE0800008}, //最大费率数
	{0xE0800009, &CmdrE0800009}, //测量点对应采集终端地址
	{0xE080000A, &CmdrE080000A}, //测量点端口号
	{0xE080000B, &CmdrE080000B}, //端口参数
	{0xE080000C, &CmdrE080000C}, //CT变比
	{0xE080000D, &CmdrE080000D}, //PT变比
	{0xE080000F, &CmdrE080000F}, //以上数据项集合
	{0xE0800010, &CmdrE0800010}, //测量点接线方式
	{0xE0800011, &CmdrE0800011}, //额定电压(二次侧)
	{0xE0800012, &CmdrE0800012}, //额定电流
	{0xE0800013, &CmdrE0800013}, //用户申报容量(折算到二次侧)
	{0xE0800014, &CmdrE0800014}, //变压器容量(折算到二次侧)
	{0xE080001F, &CmdrE080001F}, //以上数据项集合
	{0xE0800021, &CmdrE0800021}, //电能表通信密码
	{0xE0800022, &CmdrE0800022}, //电能表通信用户名
	{0xE080002F, &CmdrE080002F}, //以上数据集合

	{0xE0800100, &CmdrE0800100}, //电流不平衡度阈值
	{0xE0800101, &CmdrE0800101}, //电流不平衡度恢复阈值
	{0xE0800102, &CmdrE0800102}, //合格电压上限
	{0xE0800103, &CmdrE0800103}, //合格电压下限
	{0xE0800104, &CmdrE0800104}, //判断电流过流的相对额定值的比例
	{0xE0800105, &CmdrE0800105}, //判断负荷过载相对额定值的比例
	{0xE0800106, &CmdrE0800106}, //判断负荷过载恢复的相对额定值的比例
	{0xE0800107, &CmdrE0800107}, //零相电流报警阈值
	{0xE0800108, &CmdrE0800108}, //零相电流报警恢复阈值
	{0xE0800109, &CmdrE0800109}, //判断电流过负荷恢复的相对额定值
	{0xE080010A, &CmdrE080010A}, //电压不平衡度阈值
	{0xE080010B, &CmdrE080010B}, //电压不平衡度恢复阈值
	{0xE080010C, &CmdrE080010C}, //失压或断相报警的判断阈值
	{0xE080010F, &CmdrE080010F}, //以上数据项集合

	{0xE0800200, &CmdrE0800200}, //电压畸变率阈值，缺省值5%
	{0xE0800201, &CmdrE0800201}, //电流畸变率阈值，缺省值5%
	{0xE0800202, &CmdrE0800202}, //电压畸变率报警恢复阈值
	{0xE0800203, &CmdrE0800203}, //电流畸变率报警恢复阈值
	{0xE0800204, &CmdrE0800204}, //功率因数区段数
	{0xE0800205, &CmdrE0800205}, //功率因数区段
	{0xE0800206, &CmdrE0800206}, //监测谐波次数
	{0xE0800207, &CmdrE0800207}, //电压偏差上限
	{0xE0800208, &CmdrE0800208}, //电压偏差下限
	{0xE0800209, &CmdrE0800209}, //频率偏差上限
	{0xE080020A, &CmdrE080020A}, //频率偏差下限
	{0xE080020B, &CmdrE080020B}, //电压长闪限值
	{0xE080020C, &CmdrE080020C}, //电压短闪限值
	{0xE080020D, &CmdrE080020D},  //无功过补偿欠补偿告警发生阈值
	{0xE080020E, &CmdrE080020E},  //无功过补偿欠补偿告警恢复阈值

};

static S_CmdObjPara3D sCmdObjParametersR_3D[] =//二个数据项
{//按DI值从小到大排序
	{0xE0000200, 0xE0000220, &CmdrE0000200},//抄表参数 00~20
	{0xE0000230, 0xE0000250, &CmdrE0000230},//抄表参数配置表 30~50
	{0xE0000301, 0xE00003FE, &CmdrE0000301},//任务参数 普通任务号
	{0xE0000401, 0xE00004FE, &CmdrE0000401},//任务参数 中继任务号
	{0xE0000501, 0xE00005FD, &CmdrE0000501},//运算量参数 模拟量号
	{0xE0000601, 0xE00006FD, &CmdrE0000601},//运算量参数 脉冲量号
	{0xE0000701, 0xE00007FD, &CmdrE0000701},//运算量参数 运算量编号
	{0xE0000801, 0xE00008FD, &CmdrE0000801},//运算量参数 差动规则
	{0xE0000A00, 0xE0000A1E, &CmdrE0000A00},//端口参数 485端口定义

	{0xE0000C00, 0xE0000C0F, &CmdrE0000C00},//负控参数 开关接点类型
	{0xE0000C10, 0xE0000C1F, &CmdrE0000C10},//负控参数 输出开关状态
	//{0xE0000C20, 0xE0000C2F, &CmdrE0000C20},//(只写)负控参数 输出开关状态
	{0xE0000C30, 0xE0000C3F, &CmdrE0000C30},//负控参数 输出开关轮次
	{0xE0000D01, 0xE0000D08, &CmdrE0000D01},//功率控制系列1，时段(1-8)
	{0xE0000D11, 0xE0000D18, &CmdrE0000D11},//功率控制系列2，时段(1-8)
	{0xE0000D21, 0xE0000D28, &CmdrE0000D21},//功率控制系列3，时段(1-8)
	{0xE0000D31, 0xE0000D38, &CmdrE0000D31},//功率控制系列4，时段(1-8)

	{0xFFFFFFFF, 0xFFFFFFFF, NULL},//end
};

int C_AFNCMD_RPARAM::Execute(const S_CmdObjs &CmdObjs, strings &strResponseInfos)
{
	if (CmdObjs.empty())
		return FRAME_PROCESS_RESPONSE_DENY;
	for (S_CmdObjs::const_iterator iterNow = CmdObjs.begin(); iterNow != CmdObjs.end(); advance(iterNow, 1))
	{
		C_CmdReceiver *pCmdReveiver = GetCmdReceiver(iterNow->m_DI);
		if (pCmdReveiver == NULL)
			return FRAME_PROCESS_RESPONSE_DENY;
		strings strResponse;
		if (pCmdReveiver->Prompt() != NULL)
			logit<<"命令功能:"<<pCmdReveiver->Prompt()<<endl;
		pCmdReveiver->SetPackageLength(m_PackageLength);
		if (pCmdReveiver->Action(*iterNow, strResponse) == false)
			return FRAME_PROCESS_RESPONSE_DENY;
		for (size_t i = 0; i < strResponse.size(); i++)
			strResponseInfos.push_back(strResponse[i]);
	}
	if (strResponseInfos.empty())
		return FRAME_PROCESS_RESPONSE_CONFIRM;
	return FRAME_PROCESS_RESPONSE_DATA;
}

//参数读取命令中的具体命令接收者查找函数(采用Flyweight模式)
C_CmdReceiver *C_AFNCMD_RPARAM::GetCmdReceiver(INT32U DI)
{
	const S_CmdObjPara CmdObjNow = {DI, NULL};
	S_CmdObjPara *pFindObj = lower_bound(sCmdObjParametersR, sCmdObjParametersR+sizeof(sCmdObjParametersR)/sizeof(sCmdObjParametersR[0]), CmdObjNow);
	if (pFindObj != sCmdObjParametersR+sizeof(sCmdObjParametersR)/sizeof(sCmdObjParametersR[0])
		&& pFindObj->m_DI == DI)
		return pFindObj->m_pCmdReceiver;

	const S_CmdObjPara3D CmdObjNow3D = {DI, DI, NULL};
	S_CmdObjPara3D *pFindObj3D = lower_bound(sCmdObjParametersR_3D, sCmdObjParametersR_3D+sizeof(sCmdObjParametersR_3D)/sizeof(sCmdObjParametersR_3D[0]), CmdObjNow3D);
	if (pFindObj3D != sCmdObjParametersR_3D+sizeof(sCmdObjParametersR_3D)/sizeof(sCmdObjParametersR_3D[0]))
	{
		if (DI >= pFindObj3D->m_diBegin && DI <= pFindObj3D->m_diEnd)
			return pFindObj3D->m_pCmdReceiver;
		if (pFindObj3D != sCmdObjParametersR_3D)
			pFindObj3D -= 1;
		if (DI >= pFindObj3D->m_diBegin && DI <= pFindObj3D->m_diEnd)
			return pFindObj3D->m_pCmdReceiver;
	}
	return NULL;
}


