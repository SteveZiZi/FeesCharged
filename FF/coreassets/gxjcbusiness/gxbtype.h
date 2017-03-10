/**
 *  @file
 *  @brief �����û����Ͷ���
 *  @author 
 *  @date 2012-06-05
 *  @version v1.0
 *  @note
 *  ����ҵ������������Ͷ���
 */
#ifndef __GXJCB_BASETYPE_H_
#define __GXJCB_BASETYPE_H_
#include "basedef.h"

//֡��ַ����-1������0��������ַ��1-?���ӵ�ַ?-?�����ַ��
#define FRAME_ADDR_UNKNOW		-1
#define FRAME_ADDR_ME				0
#define FRAME_ADDR_SLAVE_MIN		1
#define FRAME_ADDR_SLAVE_MAX		(FRAME_ADDR_SLAVE_MIN+GX_FORWARD_ADDR_MAX-1)
#define FRAME_ADDR_BROADCAST		(FRAME_ADDR_SLAVE_MAX+1)
#define FRAME_ADDR_GROUP_MIN		(FRAME_ADDR_BROADCAST+1)
#define FRAME_ADDR_GROUP_MAX		(FRAME_ADDR_GROUP_MIN+GX_GROUP_ADDR_MAX-1)

//
#define GX_GROUP_ADDR_MAX	8
#define GX_RELAY_ADDR_MAX	16
#define GX_FORWARD_ADDR_MAX		4
struct S_TermGroup
{
	bool m_RelayEnable;///< �м�ת��ʹ��
	S_WORD m_RelayAddr[GX_RELAY_ADDR_MAX];///<�м̵�ַ
	S_WORD m_GroupAddr[GX_GROUP_ADDR_MAX];///<���ַ
};



#endif//__GXJCB_BASETYPE_H_

