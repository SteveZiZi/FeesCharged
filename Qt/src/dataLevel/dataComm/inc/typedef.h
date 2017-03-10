/** @file
 *  @brief 基础数据类型定义
 *  @author Steven Wan
 *  @date 2016-02/11
 *  @version 0.1
*/

#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_
#include <string.h>
#include <stdio.h>



#ifndef WIN32
	#undef __STL_USE_EXCEPTIONS
	#undef __EXCEPTIONS
	#ifdef __THROW_BAD_ALLOC
	#undef __THROW_BAD_ALLOC
	#endif
	#ifndef __THROW_BAD_ALLOC
		#define __THROW_BAD_ALLOC throw bad_alloc()
	#endif

	//#define ASSERT(exp)     ((void)0)
	#define ASSERT(exp) {if (!(exp)) printf("***********Assert failed.file=%s,line=%d,exp=%s\n",__FILE__,__LINE__,#exp);}
	#define ASSERT_TRIP(exp,str) {if (!(exp)) printf("***********Assert failed.file=%s,line=%d,exp=%s,exp=%s\n",__FILE__,__LINE__,#exp,#str);}
#else
	#include "assert.h"
	#define ASSERT assert
#endif

#define YEAR_21CENT_BEGIN 2000
#define TM_YEAR_BEGIN     1900
#define TM_MONTH_BEGIN    1

/* 卡相关信息 */
#define CONS_CARD_SERIALNO_LEN	8//卡序列号8字节

#define CONS_CARD_METERNO_LEN	8//表号8字节

#define CONS_DEVICE_MAX_COUNT 64+1 //电表数

/* 数据类型定义*/
typedef unsigned char			INT8U;
typedef INT8U                   		UCHAR;
typedef unsigned short			INT16U;
typedef signed short			INT16S;
typedef unsigned int			INT32U;
//typedef signed int				INT32S;
typedef unsigned long long		INT64U;
typedef signed long long		INT64S;
typedef float					FP32;
typedef double				FP64;
typedef unsigned char 			byte;

typedef void				S_VOID;
typedef char				S_CHAR;
typedef short				S_SHORT;
typedef long				S_LONG;
typedef unsigned char		S_BYTE;
typedef unsigned short		S_WORD;
typedef unsigned int		S_DWORD;

#define S_BYTE	S_BYTE
#define S_WORD	S_WORD
#define INT32U	S_DWORD
#define INT8S	S_CHAR
#define INT16S	S_SHORT
#define INT32S	S_LONG

#define S_TIME	S_DWORD
#define S_BOOL	bool
#define S_NULL	((void *)0)

typedef enum
{
	emFail = 0,
	emSucess =1	
}T_emResult;

typedef enum
{
	INVALID_CARD=1,
	PRESET_CARD=2,
	USER_CARD = 3,
	MAX_CARD
}T_emCARDTYPE;

typedef enum
{
	CARD_OUT=0,
	CARD_IN=1,
}T_emCARDSTATE;

typedef enum
{
	BRAKE_OFF=0,
	BRAKE_ON=1,
}T_emBRAKESTATE;


/* 设备内存数据*/
struct SIpcDeviceRegData
{
	unsigned char m_DeviceNo;//设备序号
	unsigned char m_Reg00010000[4];//寄存器数据当前正常有功总
	unsigned char m_Reg05060000[7][4];//近七日日冻结数
	unsigned char m_Reg00010001[6][4];//近六月冻结数
};

//合闸复电
struct SBrakeOnCmd
{
	T_emBRAKESTATE m_Brakestate;//拉合闸状态
	char m_ActionTime7Bcd[7];//拉合闸发生时间
};

struct SElecPricePack
{
	char m_Period1Val[6][4];//第一阶梯值1~6数据格式XXXXXX.XX
	char m_Period1Price[7][4];//第一阶梯电价1~7数据格式XXXX.XXXX
	char m_Period1Accountday[6][3];//第一阶梯第1 ~6结算日
};

//参数信息
struct SParamInfo
{
	char m_TariffRateSwitchTime5Bcd[5];//j两套分时费率切换时间
	char m_AlarmMoney1[4];//报警金额1 XXXXXX.XX
	char m_AlarmMoney2[4];//报警金额2 XXXXXX.XX
	char m_CtRatio[3];//电流互感变比XXXXXX
	char m_PtRatio[3];//电压互感变比XXXXXX
	char m_MeterNo[6];//表号BCD 
	char m_CustomerNo[6];//客户编号BCD
};
//套电价
struct SElecPrice
{
	char m_TariffRate[12][4]; //费率12数据格式XXXX.XXXX
	SElecPricePack m_ElecPricePack[2];//第1、2当前套电价 
};

/* 用户卡信息*/
struct SUserCard
{
	struct SParamInfo m_ParamInfo;
	struct SElecPrice m_CurrentElecPrice;//当前电价
	struct SElecPrice m_ElecPriceBackup; //备用电价
	struct SBrakeOnCmd m_BrakeOnCmd;//合闸复电命令
};

/* 卡信息*/
struct SIpcCardInfo
{
	T_emCARDTYPE m_CardType;//卡类型--2预置卡  3用户卡
	T_emCARDSTATE m_State;//插卡状态
	S_BYTE m_CardSerialNo[8];//卡序号
	S_BYTE m_CardMeterNo[8];//表序号
	S_BYTE m_EsamRandData[8];//8字节模块ESAM随机数
	S_BYTE m_DstData[8];//8字节密文
	S_BYTE m_RechargeVal[4];//充值金额，格式XXXXXX.XX
	INT32U m_RechargeTimes;//充值次数
	S_BYTE m_MAC1[4];
	S_BYTE m_CustomerNo[6];//6字节bcd,客户编号
	S_BYTE m_MAC2[4];
	S_BYTE m_ParamInfo[256];//参数信息
	S_WORD m_ParamInfoLen;//参数信息长度
	S_BYTE m_MAC3[4];
};

/*终端信息*/
/** @brief 终端RTUA */
struct SIpcRtua
{
	S_DWORD m_AreaCode; ///< 区域码,格式:高字节为0，依次字节为省份、接着字节表示地市码，低字节表示区县码
	S_DWORD m_Address; ///< 地址：选址范围为1~16777216。A2=000000H为无效地址，A2=FFFFFFH时表示系统广播地址
};

/** @brief 版本信息 */
struct SIpcVersion
{
	char m_HWVersion[32]; ///< 硬件版本号
	char m_SWVersion[32]; ///< 软件版本号
	char m_ESAMKEYVersion[32]; ///< 密钥版本号
};

/** @brief 用户档案 */
struct SIpcUserParm
{
	S_WORD m_MeterNo;//表测量点号
	S_BYTE m_MeterSerialNo[8];//表序号
	S_BYTE m_MeterAddr[6];//表地址
	S_BYTE m_CustomerNo[6];//6字节bcd,客户编号
};

/** @brief 通讯参数 */
struct SIpcCommParam
{
	char m_Apn[36];//APN
	INT32U m_ServerIP; 
	INT32U m_ServerPort;
	INT32U m_LocalIP;
	S_BYTE m_HeartBeat;
	S_BYTE m_Singal;
	S_BYTE m_NetState;
};
struct SIpcTerminalInfo
{
public://终端地址地市码
	SIpcRtua GetRTUA(void){return m_Rtua;};
	void SetRTUA(SIpcRtua rtua){m_Rtua = rtua;};

public://终端版本信息
	SIpcVersion GetVersion(void){return m_Version;};
	void SetVersion(SIpcVersion version){m_Version = version;};

public://用户档案
	SIpcUserParm GetUserParm(int mp){return m_UserParm[mp];}
	void SetUserParm(int mp,SIpcUserParm userparam){m_UserParm[mp] = userparam;}

public://通讯参数
	SIpcCommParam GetCommParm(void){return m_CommParam;}
	void SetCommParm(SIpcCommParam commparam){m_CommParam = commparam;}	
	
private:
	struct SIpcRtua m_Rtua;//终端地址地市码
	struct SIpcVersion m_Version;//终端版本信息
	struct SIpcUserParm m_UserParm[CONS_DEVICE_MAX_COUNT];//用户档案
	struct SIpcCommParam m_CommParam;//通讯参数
};

#endif
