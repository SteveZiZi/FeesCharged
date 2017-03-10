/**
 *  @file
 *  @brief �����û����Ͷ���
 *  @author 
 *  @date 2011-07-28
 *  @version v1.0
 *  @note
 *  ��������ҵ������������Ͷ���
 */
#ifndef __NWB_BASETYPE_H_
#define __NWB_BASETYPE_H_
#include "basedef.h"
//#include "channel.h"

//����֡��ַ����
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

/** @brief �ն�RTUA */
struct S_RTUA
{
	S_DWORD m_AreaCode; ///< ������,��ʽ:���ֽ�Ϊ0�������ֽ�Ϊʡ�ݡ������ֽڱ�ʾ�����룬���ֽڱ�ʾ������
	S_DWORD m_Address; ///< ��ַ��ѡַ��ΧΪ1~16777216��A2=000000HΪ��Ч��ַ��A2=FFFFFFHʱ��ʾϵͳ�㲥��ַ
};

/** @brief �������ݽṹ���� */
struct S_EnergyData
{
	S_WORD m_MeterMP; ///< ���������
	S_BYTE m_DataFlag;
	S_BYTE m_VipDataCnt; ///< ���ݵ����ݣ������ص��û������洢
	string m_Data; ///< ��������

public:
	S_EnergyData(void){m_DataFlag=0xFF;m_MeterMP=0x00;m_VipDataCnt=0x01;}
};
typedef vector<S_EnergyData> S_EnergyDatas;

/** @brief ���ݶ��ṹ*/
struct S_EnergyDataR
{
	S_EnergyData m_EnergyData;
	S_TIME m_timeData;
public:
	S_EnergyDataR(void){m_timeData=0;}
};
typedef vector<S_EnergyDataR> S_EnergyDataRs;


#endif//__NWB_BASETYPE_H_

