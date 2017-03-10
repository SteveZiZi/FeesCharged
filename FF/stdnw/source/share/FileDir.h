///*FileDir.h中包括所有的与路径文件相关的定义*/

#ifndef INCLUDED_FILE_DIR
#define INCLUDED_FILE_DIR


/****************************************************************
/mnt下的目录结构
****************************************************************/
#ifdef WIN32
#define PATH_BASE	"./mnt"
#else
#define PATH_BASE	"/mnt"
#endif

#define PATH_DYJC_BASE	PATH_BASE"/dyjc"//刘志强添加

#ifdef ARCH_PPC
#define DOC_PATH "/"
#else
#define DOC_PATH "/mnt"
#endif

#define FILE_COMMON_TASK_DATA_POSTFIX	"dat"
#define FILE_TRACE_FILE_POSTFIX		"log"
#define TRACE_FILE_BACKUP_COMMAND "tar -czf"
#define TRACE_FILE_BACKUP_FILE_COUNT 4
#define FILE_TRACE_FILE_BACKUP_EXTEND_NAME ".tgz"

//#define FILE_TRACE_TTYP		"/dev/ttyp"
#define FILE_TRACE_TTYP			"/dev/pts/"

#define CONS_FILE_UPDATE_LOG PATH_BASE"/updatelog.dat"		//苏煦烽2006-10-19添加，升级日志
#define CONS_UPDATE_FILE_NAME PATH_BASE"/update/update.tgz"	//苏煦烽2007-11-06添加，默认的升级包文件
#define CONS_AUT0_UPDATE_KEY_FILE_NAME_TEMP PATH_BASE"/update/update.key_temp"		//苏煦烽2007-11-12添加，默认的升级通知文件临时文件
#define CONS_AUTO_UPDATE_KEY_FILE_NAME PATH_BASE"/update/auto_update.key"		//苏煦烽2007-11-06添加，默认的升级通知文件
#define CONS_JILIAN_UPDATE_KEY_FILE_NAME PATH_BASE"/update/jilian_update.key"	//苏煦烽2007-11-12添加，默认的级联升级通知文件
#define CONS_AUTO_SAVE_LSTNODE_FILE_NAME PATH_BASE"/data/save/lst_nodes.bak"   
//*************************************will remove
#define FILE_CFG_SYS_IP	FILE_KELI_ETH0_IP
//*************************************************

#define FILE_DYJC_SYS_CONFIG PATH_BASE"/keli/dyjc_sys_config.ini"	//dyjc系统的系统配置//刘志强添加
#define FILE_TERMINAL_CONFIG PATH_BASE"/keli/terminal_config.ini"	//终端信息配置
#define CFG_FILE_INI_PORTMAP	PATH_BASE"/keli/portmap.ini" //端口配置文件

#define FILE_KELI_ETH0_IP		PATH_BASE"/dyjc/script/eth0_ip.ini"				//系统IP地址配置文件
#define FILE_KELI_ETH1_IP		PATH_BASE"/dyjc/script/eth1_ip.ini"				//系统IP地址配置文件
#define CFG_LOCAL_IP 			PATH_BASE"/keli/local_ip.ini"	//本地ip

#define PATH_SET_SOURCE PATH_BASE"/dyjc/set"						//刘志强添加
#define PATH_SET_USER PATH_BASE"/user/dyjc/set"					//刘志强添加

#define NAME_SET_METER "meter.ini"							//电表配置文件名
#define NAME_SET_TASK "task.ini"							//任务配置文件名
#define NAME_SET_NORTASK "nortask.ini"							//任务配置文件名
#define NAME_SET_HTTX "httx.ini"							//后台通讯配置文件名
#define NAME_SET_GPRS "gprs.ini"							//GPRS通信扩展参数，主要用于GPRS流量限制优化，2008-2-29添加
#define NAME_SET_GDDBCJ "gddbcj.ini"						//电表通讯配置文件名
#define NAME_SET_GWDBCJ "gwdbcj.ini"						//电表通讯配置文件名
#define NAME_SET_TRACE "trace.ini"							//跟踪配置文件名
#define NAME_SET_PPP "ppp.ini"								//ppp配置文件
#define NAME_SET_HTTX_PLAN	"httx_plan.ini"					//后台通讯方案名
#define NAME_SET_JILIAN "jilian.ini"						//级联进程配置文件，2006-06-07，由苏煦烽添加
#define NAME_SET_DISTRIBUTE "JilianSlaves.ini"				//级联从终端配置文件
#define NAME_SET_FIXDATA "FixData.ini"						//补数方案配置文件
#define NAME_SET_ROUTERSERVER "RouterServer.ini"			//自动路由进程配置文件 胡，07-06-14
#define CFG_FILE_INI_IPCONFIG		"ipconfig.ini"
#define NAME_SET_RELAY "Relay.ini"                          //采集终端配置文件
#define NAME_SET_CLRELAY "ClRelay.ini"                          //回路采集终端配置文件
#define NAME_SET_ROUTER "Router.ini"                        //路由表配置文件
#define NAME_SET_ZBSNIFFER "ZBSnifferTask.ini"              //载波质量侦测任务配置文件
#define NAME_SET_DNPROTOCOL "DnProtocolType.ini"			//自动路由进程配置文件 胡，07-06-14

#define NAME_SET_EVENT "event.ini"							//事件配置文件名
//#define NAME_SET_EVENT_ID "event_id.ini"					//事件ID配置文件名
//#define NAME_SET_DEVICE_RULE "drule.ini"					//装置规约配置名
//#define NAME_SET_METER_RULE "mrule.ini"					//电表规约配置名
#define NAME_SET_ACTION "action.ini"						//操作配置名

//#define NAME_SET_PPP_APN				"ppp_apn.ini"
//#define NAME_SET_PPP_TELECODE		"ppp_telecode.ini"
//#define NAME_SET_PPP_USERNAME		"ppp_username.ini"
//
#define PATH_SCRIPT_SOURCE PATH_BASE"/dyjc/script"				//刘志强添加
#define PATH_SCRIPT_USER PATH_BASE"/user/dyjc/script"			//刘志强添加
#define METER_OPEN_CLOSE_LOGFILE PATH_BASE"/data/meter_open_logfile.dat"	//刘志强添加电表拉合闸日志文件

#define NAME_SCRIPT_ON_START			"on_start.srt"				//系统启动时调用此脚本
#define NAME_SCRIPT_ON_STOP				"on_stop.srt"				//系统停止时调用此脚本
#define NAME_SCRIPT_ON_MAIN_UPDATE		"on_djs_update.srt"			//接收到主升级文件后调用此脚本
#define NAME_SCRIPT_ON_SYS_UPDATE		"on_sys_update.srt"			//接收到系统升级文件后调用此脚本
#define NAME_SCRIPT_MEM_RESET			"on_mem_reset.srt"			//内存复位操作时调用此脚本
#define NAME_SCRIPT_DATA_RESET			"on_data_reset.srt"			//数据复位操作时调用此脚本
#define NAME_SCRIPT_PARAM_RESET			"on_param_reset.srt"		//数据复位操作时调用此脚本
#define NAME_SCRIPT_CLEAR_LOGS			"on_clear_logs.srt"			//清除系统日志时调用此脚本
#define NAME_SCRIPT_FACTORY_RESET		"on_factory_reset.srt"		//恢复出厂设置时调用此脚本
#define NAME_SCRIPT_SOFT_RESET			"on_soft_reset.srt"			//软复位时调用此脚本
#define NAME_SCRIPT_SET_REPORT_DAY		"on_set_report_day.srt"		//清除日数据上报标志脚本
#define NAME_SCRIPT_SET_REPORT_MONTH	"on_set_report_month.srt"	//清除月数据上报标志脚本
#define NAME_SCRIPT_PPP_GPRS_ON			"ppp_gprs_on.srt"			//PPP拨号上线脚本(GPRS)
#define NAME_SCRIPT_PPP_GPRS_OFF		"ppp_gprs_off.srt"			//PPP拨号下线脚本(GPRS)
#define NAME_SCRIPT_PPP_CDMA_ON			"ppp_cdma_on.srt"			//PPP拨号上线脚本(CDMA)
#define NAME_SCRIPT_PPP_CDMA_OFF		"ppp_cdma_off.srt"			//PPP拨号下线脚本(CDMA)
#define NAME_SCRIPT_ETH0_IPCONFIG		"ipconfig.sh"			//eth0 set
#define NAME_SCRIPT_CAN0_IPCONFIG		"can0config.srt"			//eth0 set


//电表规约配置
#define PATH_METER_LIB PATH_BASE"/dyjc/meterlib/"				//刘志强添加

#define FILE_COMMON_CFG_LGBLIB	PATH_METER_LIB"reg_lgb.ini"
#define FILE_COMMON_CFG_LGDLIB	PATH_METER_LIB"reg_lgd.ini"
#define FILE_COMMON_CFG_MKLIB PATH_METER_LIB"reg_mk.ini"
#define FILE_COMMON_CFG_WSLIB  PATH_METER_LIB"reg_ws.ini"
#define FILE_COMMON_CFG_GB645LIB  PATH_METER_LIB"reg_gb645.ini"
#define FILE_COMMON_CFG_GB2007LIB  PATH_METER_LIB"reg_gb2007.ini"

#define FILE_DYJC_STATUS	PATH_BASE"/user/dyjc/data/status.dat"				//DYJC系统的状态共享内存文件//刘志强添加

//#define PATH_COMMON_RESOURCE_BMP_KELI		"/mnt/keli/resource/bmp/keli.bmp"	//液晶显示图标目录
#define FILE_COMMON_RESOURCE_HZK_HZK12  PATH_BASE"/keli/hzk12.bin"

//===============================================================================================================================
//刘志强添加 仅是把eac替换为dyjc
#define FILE_COMMON_SCRIPT_LAST_EXEC_UPDATE_BAK		PATH_BASE"/data/common/last_exec_update.srt" //保存最后一次执行性升级文件
//#define FILE_COMMON_CONFIG_LCDPASSW					"/mnt/data/common/lcdpassw.dat"		//液晶用户密码文件
//#define FILE_COMMON_CONFIG_LCD_BASEINFO				"/mnt/data/common/lcdbaseinfo.dat"		//液晶基本信息
//#define FILE_COMMON_DATA_LCD_MESSAGE				"/mnt/data/common/message.dat"			//短消息文件

#define FILE_COMMON_TASK_DATA						PATH_BASE"/data/task/task"

#define FILE_TRACE_PUBLIC_TRACE						PATH_BASE"/data/log/trace"
#define FILE_TRACE_PUBLIC_DATA						PATH_BASE"/data/log/data"
#define FILE_TRACE_PUBLIC_SCHEDULE					PATH_BASE"/data/log/sche"

#define FILE_DYJC_CONTEXT PATH_BASE"/ramdisk/dyjc/context.dat"	//DYJC系统的上下文内存共享文件

//GDDYJC====================================================================================
#define FILE_GDDYJC_POWEROFF_DATA PATH_BASE"/data/alarm/alarm_poweroff.dat"			//停电事件日志
#define FILE_GDDYJC_WASTE_ALARM_TMP PATH_BASE"/data/tmp/waste.dat"					//线损报警暂存文件
#define FILE_GDDYJC_GATHER_FAIL_LOG	PATH_BASE"/data/tmp/gathfail.dat"				//抄表失败记录
#define FILE_GDDYJC_SYSTEM_LOG PATH_BASE"/data/log/syslog.dat"						//系统事件记录
#define FILE_GDDYJC_BREAK_LOG PATH_BASE"/data/tmp/breaklog.dat"						//拉合闸状态字日志
//GDDYJC====================================================================================

#define FILE_ENERGY_SHARE PATH_BASE"/ramdisk/data/energy.bin"	//能量寄存器文件
#define FILE_LOG_GATHERTIME PATH_BASE"/data/log/gather.dat"	//电表采集的日志
#define FILE_LOG_ZBSNIFFERLOG PATH_BASE"/data/log/zbsnifferlog.dat"	//载波质量侦测日志
#define FILE_EVENT_ALARM_DATA PATH_BASE"/data/alarm/alarm_tmp.dat"		//电表事件报警记录文件

//日分时数据存放位置	  有待修改 刘志强+06-6-24
#define FILE_COMMON_DATA_IFCONFIG			PATH_BASE"/ramdisk/ramdyjc/common/data/ifconfig.log"	//临时查询系统配置文件
	
#define FILE_DATA_TASK_MONTH_DATA				PATH_BASE"/data/task/monthFSdata.dat"			//月分时数据
#define FILE_DATA_TASK_DAY_DATA				PATH_BASE"/data/task/dayLDdata.dat"					//电表零点数据
#define DALY_LEN 24					//上报日常综合数据时每帧包含的数据单元数.即电表数.
#endif
