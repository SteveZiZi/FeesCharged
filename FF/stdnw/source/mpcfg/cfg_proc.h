/** @file
 *  @brief ����������ļ�
 *  @author 
 *  @date 2011/10/15
 *  @version 0.1
 *  1�����̺�������
 *  2��ע�⣺�������̺�һ��Ҫ�޸ġ����������ꡱ�Ķ���
 */
#ifndef __MP_CFG_PROCESS_H__
#define __MP_CFG_PROCESS_H__

/** @brief ���̷������ƶ��� */
#define MP_PROCESS_HTTX1	"httx1"
#define MP_PROCESS_HTTX2	"httx2"
#define MP_PROCESS_HTTX3	"httx3"
#define MP_PROCESS_HTTX4	"httx4"
#define MP_PROCESS_DBCJ1		"dbcj1"
#define MP_PROCESS_DBCJ2		"dbcj2"
#define MP_PROCESS_DBCJ3		"dbcj3"
#define MP_PROCESS_DBCJ4		"dbcj4"
#define MP_PROCESS_PPP		"ppp"
#define MP_PROCESS_LCD		"lcd"
#define MP_PROCESS_JLCY		"jlcy"
#define MP_PROCESS_FHKZ		"fhkz"
#define MP_PROCESS_CTCY		"ctcy"

#define MP_PROCESS_RFTX              "rftx"   //wjp 20160729
/** @brief ����������
 *  @remarks �������̺�һ��Ҫ�޸ı���Ķ��壬
 *  ����ֵ������gMP_ProcessAll�����н��̸�����ȣ����򽫳���PLEASE BE CAREFUL.
 */
#define MP_PROCESS_CNT	(1/*daemon*/+4/*httx */+4/*dbcj */+1/*ppp*/+1/*lcd*/+1/*jlcy*/+1/*fhkz*/+1/*ctcy*/+1/*rftx*/)


#endif//__MP_CFG_PROCESS_H__

