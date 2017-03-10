/** @file
 *  @brief 产品版本定义
 *  @author 
 *  @date 2011/11/30
 *  @version 0.1
 */
#ifndef __NW_SYS_CONFIG_H__
#define __NW_SYS_CONFIG_H__

/** @brief 后台通讯进程总数 */
#define HTTX_PROCESS_MAX	4
/** @brief 电表采集进程总数 */
#define DBCJ_PROCESS_MAX	4

/** @brief 串口总数 */
#define SYSTEM_SERIAL_PORT_MAX		64

/** @brief 上下行通道个数定义 */
#define CHANNEL_UP_COUNT	20
#define CHANNEL_DN_COUNT	16

/** @brief 产品宏定义 */
#define CONS_TERM_TYPE_CZ	1
#define CONS_TERM_TYPE_FK	2
#define CONS_TERM_TYPE_PB	3
#define CONS_TERM_TYPE_JC	4

/*** 产品类型定义***/
#define CONS_TERM_TYPE	CONS_TERM_TYPE_JC


#define CONS_FACTORY_CODE_BIN	0x1234

#define CONS_TERM_NAME		"stdnw"

/***********************/
/**  系统测量点数配置  */
/***********************/
#define CONS_MP_COUNT_MAX	BASEB_MP_COUNT_MAX
//#if (CONS_TERM_TYPE == CONS_TERM_TYPE_JC)
	#define CONS_METER_COUNT_MAX		CONS_MP_COUNT_MAX /*表计总量=1200+1;+1那个是为将序号0放弃不用,这样测量点号可以直接当序号使用*/
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
/**    系统任务数配置    */
/*************************/
#define CONS_TASK_COUNT_MAX			32	//抄表任务数
#define CONS_POLLING_TYPE_COUNT_MAX			64	//轮询类型总数
#define CONS_ALARM_TYPE_COUNT_MAX			128	//告警类型总数

/*************************/
/**    系统告警类型总数数配置    */
/*************************/
#define CONS_ALARM_TYPS_COUNT_MAX			30	//告警类型总数

#define CONS_EVENT_TYPS_COUNT_MAX			30	//事件类型总数
/***********************************/
/**            表计规约配置        */
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


/*系统中级节点数配置*/
#define CONS_RELAY_COUNT_MAX		1200//一个集中器可支持的最大采集终端数
#define CONS_ROUTER_COUNT_MAX		1200//一个集中器可支持的最大路由数


/*系统最大FN寄存器数*/
#define CONS_FN_COUNT_MAX			256

/*系统最大需要采集寄存器数*/
//#if (CONS_TERM_TYPE == CONS_TERM_TYPE_JC)
	#define CONS_FN_GATHER_REGS_COUNT_MAX	150
//#elif (CONS_TERM_TYPE == CONS_TERM_TYPE_FK)
//	#define CONS_FN_GATHER_REGS_COUNT_MAX	150
//#elif (CONS_TERM_TYPE == CONS_TERM_TYPE_EAC)
//	#define CONS_FN_GATHER_REGS_COUNT_MAX	100
//#elif (CONS_TERM_TYPE == CONS_TERM_TYPE_PB)
//	#define CONS_FN_GATHER_REGS_COUNT_MAX	150
//#endif



/*系统最大电表数据寄存器*/
#define CONS_DATA_REG_MAX_COUNT		256

/*告警最大种类数*/
#define CONS_MAX_ALARM_NUMBERS		256

/*负荷管理参数相关定义*/
/*最大开关接点数*/
#define CONS_MAX_FK_SWITCH 15  
/*功率控制最大系列数*/
#define CONS_MAX_FK_TIMECTRL_TYPE 4 
/*功率控制最大时段数*/
#define CONS_MAX_FK_TIMEL_SET 8 


/* mnt下的目录结构*/
#ifdef WIN32
#define PATH_BASE	"../../mnt"
#else
#define PATH_BASE	"/mnt"
#endif
#define MNT_PATH	PATH_BASE

#endif//__NW_SYS_CONFIG_H__

