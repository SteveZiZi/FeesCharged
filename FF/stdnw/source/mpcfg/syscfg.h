/** @file
 *  @brief ��Ʒ�汾����
 *  @author 
 *  @date 2011/11/30
 *  @version 0.1
 */
#ifndef __NW_SYS_CONFIG_H__
#define __NW_SYS_CONFIG_H__

/** @brief ��̨ͨѶ�������� */
#define HTTX_PROCESS_MAX	4
/** @brief ���ɼ��������� */
#define DBCJ_PROCESS_MAX	4

/** @brief �������� */
#define SYSTEM_SERIAL_PORT_MAX		64

/** @brief ������ͨ���������� */
#define CHANNEL_UP_COUNT	20
#define CHANNEL_DN_COUNT	16

/** @brief ��Ʒ�궨�� */
#define CONS_TERM_TYPE_CZ	1
#define CONS_TERM_TYPE_FK	2
#define CONS_TERM_TYPE_PB	3
#define CONS_TERM_TYPE_JC	4

/*** ��Ʒ���Ͷ���***/
#define CONS_TERM_TYPE	CONS_TERM_TYPE_JC


#define CONS_FACTORY_CODE_BIN	0x1234

#define CONS_TERM_NAME		"stdnw"

/***********************/
/**  ϵͳ������������  */
/***********************/
#define CONS_MP_COUNT_MAX	BASEB_MP_COUNT_MAX
//#if (CONS_TERM_TYPE == CONS_TERM_TYPE_JC)
	#define CONS_METER_COUNT_MAX		CONS_MP_COUNT_MAX /*�������=1200+1;+1�Ǹ���Ϊ�����0��������,����������ſ���ֱ�ӵ����ʹ��*/
	#define CONS_SERIAL_COUNT_MAX	4
	#define CONS_PULSE_COUNT_MAX		2
//#elif (CONS_TERM_TYPE == CONS_TERM_TYPE_FK)
//	#define CONS_METER_COUNT_MAX		4
//	#define CONS_SERIAL_COUNT_MAX	2
//	#define CONS_PULSE_COUNT_MAX		2
//#elif (CONS_TERM_TYPE == CONS_TERM_TYPE_EAC)
//	#define CONS_METER_COUNT_MAX		128
//	#define CONS_SERIAL_COUNT_MAX	3
//	#define CONS_PULSE_COUNT_MAX		2
//#elif (CONS_TERM_TYPE == CONS_TERM_TYPE_PB)
//	#define CONS_METER_COUNT_MAX		4
//	#define CONS_SERIAL_COUNT_MAX	3
//	#define CONS_PULSE_COUNT_MAX		2
//#endif
/*************************/
/**    ϵͳ����������    */
/*************************/
#define CONS_TASK_COUNT_MAX			32	//����������
#define CONS_POLLING_TYPE_COUNT_MAX			64	//��ѯ��������
#define CONS_ALARM_TYPE_COUNT_MAX			128	//�澯��������

/*************************/
/**    ϵͳ�澯��������������    */
/*************************/
#define CONS_ALARM_TYPS_COUNT_MAX			30	//�澯��������

#define CONS_EVENT_TYPS_COUNT_MAX			30	//�¼���������
/***********************************/
/**            ��ƹ�Լ����        */
/***********************************/
//#if (CONS_TERM_TYPE == CONS_TERM_TYPE_JC)
//	#define MRULE_GB645
//	#define MRULE_GB2007
//#elif (CONS_TERM_TYPE == CONS_TERM_TYPE_FK)
//	#define MRULE_GB645
//	#define MRULE_GB2007
//#elif (CONS_TERM_TYPE == CONS_TERM_TYPE_EAC)
//	#define MRULE_GB645
//	#define MRULE_GB2007
//#elif (CONS_TERM_TYPE == CONS_TERM_TYPE_PB)
//	#define MRULE_GB645
//	#define MRULE_GB2007
//#endif


/*ϵͳ�м��ڵ�������*/
#define CONS_RELAY_COUNT_MAX		1200//һ����������֧�ֵ����ɼ��ն���
#define CONS_ROUTER_COUNT_MAX		1200//һ����������֧�ֵ����·����


/*ϵͳ���FN�Ĵ�����*/
#define CONS_FN_COUNT_MAX			256

/*ϵͳ�����Ҫ�ɼ��Ĵ�����*/
//#if (CONS_TERM_TYPE == CONS_TERM_TYPE_JC)
	#define CONS_FN_GATHER_REGS_COUNT_MAX	150
//#elif (CONS_TERM_TYPE == CONS_TERM_TYPE_FK)
//	#define CONS_FN_GATHER_REGS_COUNT_MAX	150
//#elif (CONS_TERM_TYPE == CONS_TERM_TYPE_EAC)
//	#define CONS_FN_GATHER_REGS_COUNT_MAX	100
//#elif (CONS_TERM_TYPE == CONS_TERM_TYPE_PB)
//	#define CONS_FN_GATHER_REGS_COUNT_MAX	150
//#endif



/*ϵͳ��������ݼĴ���*/
#define CONS_DATA_REG_MAX_COUNT		256

/*�澯���������*/
#define CONS_MAX_ALARM_NUMBERS		256

/*���ɹ��������ض���*/
/*��󿪹ؽӵ���*/
#define CONS_MAX_FK_SWITCH 15  
/*���ʿ������ϵ����*/
#define CONS_MAX_FK_TIMECTRL_TYPE 4 
/*���ʿ������ʱ����*/
#define CONS_MAX_FK_TIMEL_SET 8 


/* mnt�µ�Ŀ¼�ṹ*/
#ifdef WIN32
#define PATH_BASE	"../../mnt"
#else
#define PATH_BASE	"/mnt"
#endif
#define MNT_PATH	PATH_BASE

#endif//__NW_SYS_CONFIG_H__

