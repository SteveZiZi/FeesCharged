/**
 *  @file
 *  @brief 基本用户类型定义
 *  @author 
 *  @date 2012-06-05
 *  @version v1.0
 *  @note
 *  国网基础业务基本数据类型定义
 */
#ifndef __GWB_BASETYPE_H_
#define __GWB_BASETYPE_H_
#include "basedef.h"

//国网帧地址类型-1：出错；0：本机地址；1-?：从地址?-?：组地址；
#define FRAME_ADDR_UNKNOW		-1
#define FRAME_ADDR_ME				0
#define FRAME_ADDR_SLAVE_MIN		1
#define FRAME_ADDR_SLAVE_MAX		(FRAME_ADDR_SLAVE_MIN+GW_FORWARD_ADDR_MAX-1)
#define FRAME_ADDR_BROADCAST		(FRAME_ADDR_SLAVE_MAX+1)
#define FRAME_ADDR_GROUP_MIN		(FRAME_ADDR_BROADCAST+1)
#define FRAME_ADDR_GROUP_MAX		(FRAME_ADDR_GROUP_MIN+GW_GROUP_ADDR_MAX-1)

//
#define GW_GROUP_ADDR_MAX	8
#define GW_RELAY_ADDR_MAX	16
#define GW_FORWARD_ADDR_MAX		3
struct S_TermGroup
{
	bool m_RelayEnable;///< 中继转发使能
	S_WORD m_RelayAddr[GW_RELAY_ADDR_MAX];///<中继地址
	S_WORD m_GroupAddr[GW_GROUP_ADDR_MAX];///<组地址
};



#endif//__GWB_BASETYPE_H_

