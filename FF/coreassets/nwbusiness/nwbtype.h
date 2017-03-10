/**
 *  @file
 *  @brief 基本用户类型定义
 *  @author 
 *  @date 2011-07-28
 *  @version v1.0
 *  @note
 *  南网基础业务基本数据类型定义
 */
#ifndef __NWB_BASETYPE_H_
#define __NWB_BASETYPE_H_
#include "basedef.h"
//#include "channel.h"

//南网帧地址类型
#define FRAME_ADDR_UNKNOW		-1
#define FRAME_ADDR_ME				0
#define FRAME_ADDR_BROADCAST		1
#define FRAME_ADDR_GATHERTERM	2
#define FRAME_ADDR_SLAVE_MIN		3
#define FRAME_ADDR_SLAVE_MAX		(FRAME_ADDR_SLAVE_MIN+NW_FORWARD_ADDR_MAX-1)

//
#define NW_FORWARD_ADDR_MAX		4

#define NW_TARIFF_DEFAULT_MAX		4
#define NW_MP_MAX		1200

/** @brief 终端RTUA */
struct S_RTUA
{
	S_DWORD m_AreaCode; ///< 区域码,格式:高字节为0，依次字节为省份、接着字节表示地市码，低字节表示区县码
	S_DWORD m_Address; ///< 地址：选址范围为1~16777216。A2=000000H为无效地址，A2=FFFFFFH时表示系统广播地址
};

/** @brief 能量数据结构定义 */
struct S_EnergyData
{
	S_WORD m_MeterMP; ///< 电表测量点号
	S_BYTE m_DataFlag;
	S_BYTE m_VipDataCnt; ///< 数据点数据，方便重点用户批量存储
	string m_Data; ///< 能量数据

public:
	S_EnergyData(void){m_DataFlag=0xFF;m_MeterMP=0x00;m_VipDataCnt=0x01;}
};
typedef vector<S_EnergyData> S_EnergyDatas;

/** @brief 数据读结构*/
struct S_EnergyDataR
{
	S_EnergyData m_EnergyData;
	S_TIME m_timeData;
public:
	S_EnergyDataR(void){m_timeData=0;}
};
typedef vector<S_EnergyDataR> S_EnergyDataRs;


#endif//__NWB_BASETYPE_H_

