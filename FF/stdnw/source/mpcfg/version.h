/** @file
 *  @brief ��Ʒ�汾����
 *  @author 
 *  @date 2011/11/30
 *  @version 0.1
 */
#ifndef __NW_VERSION_H__
#define __NW_VERSION_H__

/** @brief ������汾�ż��������ں궨��
 *  @note
 *  ���¸��汾�ŵ�������󲻵ó��� 9
 */
#define CONS_MAJOR_VERSION	"1"						//���汾��
#define CONS_MINOR_VERSION	"0"						//�ΰ汾��
#define CONS_FIX_VERSION	  "0"						//�����汾��
#define CONS_SW_REAL_DATE	"2016-09-18"		//�������ڣ���ʽ: YYYY-MM-DD

#define CONS_MAIN_VERSION CONS_MAJOR_VERSION"."CONS_MINOR_VERSION CONS_FIX_VERSION

/** @brief ��Լ�汾�ţ�����Լ�涨Ϊ0100
 */
#define CONS_MSRULE_VER  0x0100

/** @brief Ӳ���汾�� */
#define CONS_HARD_VERSION	"1.00"
#define CONS_HW_REAL_DATE	"2016-08-25"		//�������ڣ���ʽ: YYYY-MM-DD

/** @brief �û��汾��
 */
#define CONS_SOFT_VERSION CONS_MAIN_VERSION
#define CONS_SW_RELEASE_DATE CONS_SW_REAL_DATE

#endif//__NW_VERSION_H__


