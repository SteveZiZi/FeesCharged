/**
 *  @file
 *  @brief �ֳ����߹���
 *  @author 
 *  @date 2012-05-10
 *  @version v1.0
 *  @note 
 */
#ifndef __BASEB_FB_FACTORY_H__
#define __BASEB_FB_FACTORY_H__
#include "fieldbus.h"

enum
{
	FB_IDX_485I = 0,
	FB_IDX_485II,
	FB_IDX_ZBWX,
	FB_IDX_JLCY,

	FB_IDX_MAX,
};

class C_FBFact
{
public:
	/** @fn
	 *  @brief ��ʼ���������õ��ֳ�����
	 *  @param[in] pRS485I ����RS485I����,��ΪNULL,�򲻳�ʼ��
	 *  @param[in] pRS485II ����RS485II����,��ΪNULL,�򲻳�ʼ��
	 *  @param[in] pZBWX �ز���������������,��ΪNULL,�򲻳�ʼ��
	 *  @param[in] pJLCY ����������������,��ΪNULL,�򲻳�ʼ��
	 *  @return 0:�ɹ�; <0:ʧ��
	 *  @note ���ӱ���Ϊ�Ǿֲ�����
	 */
	static int InitFiledBus(C_CONNECT *pRS485I, C_CONNECT *pRS485II, C_CONNECT *pZBWX, C_CONNECT *pJLCY);

	/** @fn
	 *  @brief �����ֳ�����
	 *  @param[in] VCOM �������⴮�ں�
	 *  @return !NULL:�ɹ�; NULL:ʧ��
	 */
	static C_FieldBus* FindFieldBus(int VCOM);

	/** @fn
	 *  @brief ��ȡ�ֳ�����
	 *  @param[in] IDX ����������
	 *  @return !NULL:�ɹ�; NULL:ʧ��
	 */
	static C_FieldBus* GetFieldBus(int IDX);

private:
	static int InitFiledBus_RS485I(C_CONNECT *pRS485I);
	static int InitFiledBus_RS485II(C_CONNECT *pRS485II);
	static int InitFiledBus_ZBWX(C_CONNECT *pZBWX);
	static int InitFiledBus_JLCY(C_CONNECT *pJLCY);
};


#endif //__BASEB_FB_FACTORY_H__

