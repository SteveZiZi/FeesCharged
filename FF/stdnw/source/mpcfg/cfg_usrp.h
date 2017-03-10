/** @file
 *  @brief 系统运行目录和重要文件定义
 *  @author 
 *  @date 2011/11/07
 *  @version 0.1
 *  1、本文件是配置文件，需要用户定义
 *  2、实际终端系统必需和本运行目录定义一致
 */
#ifndef __MP_CFG_USRPATH_H__
#define __MP_CFG_USRPATH_H__

#include "confuse.h"
/** base path */
#ifdef WIN32
#define CFG_PATH_BASE	"../../mnt/"
#else
#define CFG_PATH_BASE	"/mnt/"
#endif

/** dir define */
#define CFG_PATH_TMP		"/tmp/"
#define CFG_PATH_SYSTEM		CFG_PATH_BASE"system/"
#define CFG_PATH_RESOURCE	CFG_PATH_BASE"resource/"
#define CFG_PATH_ZONE		CFG_PATH_BASE"zone/"

#define CFG_PATH_DATA		CFG_PATH_BASE"data/"
#define CFG_PATH_LOG		CFG_PATH_DATA"log/"
#define CFG_PATH_ALARM		CFG_PATH_DATA"alarm/"
#define CFG_PATH_ENERGY		CFG_PATH_DATA"energy/"

#define CFG_PATH_DYJC		CFG_PATH_BASE"dyjc/"
#define CFG_PATH_PROGRAM	CFG_PATH_DYJC"program/"
#define CFG_PATH_SCRIPT		CFG_PATH_DYJC"script/"
#define CFG_PATH_REGISTER		CFG_PATH_DYJC"register/"
#define CFG_PATH_SET		CFG_PATH_DYJC"set/"
#define CFG_PATH_SET_DEFAULT	CFG_PATH_SET"default/"
#define CFG_PATH_SET_USER		CFG_PATH_SET"user/"
#define CFG_PATH_FACTORY_TEST	CFG_PATH_SET"factest/"

#define CFG_PATH_RAMDISK		CFG_PATH_BASE"ramdisk/"
#define CFG_PATH_UPDATE		CFG_PATH_BASE"update/"


/** system file define */
#define CFG_FILE_PROGRAM_NAME	"dyjc"
#define CFG_FILE_PROGRAM_PPPD		"pppd"
#define CFG_FILE_PROGRAM_ZONESO		"libzonex.so"

#define CFG_FILE_CONTEXT	"context.dat"
#define CFG_FILE_STATUS		"status.dat"
#define CFG_FILE_AUTO_UPDATE_KEY	"update.key"

/** script file define */
#define CFG_FILE_SCRIPT_ON_START	"on_start.srt"
#define CFG_FILE_SCRIPT_ON_STOP		"on_stop.srt"
#define CFG_FILE_SCRIPT_DATA_RESET	"on_data_reset.srt"
#define CFG_FILE_SCRIPT_FACT_RESET	"on_factory_reset.srt"
#define CFG_FILE_SCRIPT_PARAM_RESET	"on_param_reset.srt"
#define CFG_FILE_SCRIPT_ALL_RESET		"on_all_reset.srt"
#define CFG_FILE_SCRIPT_STAT_RESET	"on_status_reset.srt"
#define CFG_FILE_SCRIPT_PPP_GPRS_ON			"ppp_gprs_on.srt"			//PPP拨号上线脚本(GPRS)
#define CFG_FILE_SCRIPT_PPP_GPRS_OFF		"ppp_gprs_off.srt"			//PPP拨号下线脚本(GPRS)
#define CFG_FILE_SCRIPT_PPP_CDMA_ON			"ppp_cdma_on.srt"			//PPP拨号上线脚本(CDMA)
#define CFG_FILE_SCRIPT_PPP_CDMA_OFF		"ppp_cdma_off.srt"			//PPP拨号下线脚本(CDMA)
#define CFG_FILE_SCRIPT_PPP_MUXD			"ppp_muxd.srt"		//ppp串口复用
#define CFG_FILE_SCRIPT_PPP_CLEAR			"ppp_clear.srt"		//ppp信息清除
#define CFG_FILE_SCRIPT_IP_CONFIG			"ipconfig.sh"			//ifconfig
#define CFG_FILE_SCRIPT_ZONE_UPDATE		"zoneupdt.srt"			//区域升级脚本


/** user ini file define */
//register file define
#define CFG_FILE_INI_MSDI		"reg_ms.ini"
//system dir
#define CFG_FILE_INI_SYSCFG		"syscfg.ini"
#define CFG_FILE_INI_PORTMAP	"portmap.ini"
#define CFG_FILE_INI_SYSRUN		"sysrun.inf"
#define CFG_FILE_INI_CFGVER		"cfgver.ini"
//set dir
#define CFG_FILE_INI_TRACE			"trace.ini"
#define CFG_FILE_INI_UPCHANNL		"upchannl.ini"					//上行通道配置文件
#define CFG_FILE_INI_DNCHANNL		"dnchannl.ini"					//下行通道配置文件
#define CFG_FILE_INI_HTTX			"httx.ini"
#define CFG_FILE_INI_METER			"meter.ini"
#define CFG_FILE_INI_IPCONFIG		"ipconfig.ini"
#define CFG_FILE_INI_FLOWCTRL		"flowctrl.ini"
#define CFG_FILE_INI_TASK		 	"gathert.ini"
#define CFG_FILE_INI_PPP			"ppp.ini"
#define CFG_FILE_INI_LCD			"lcd.ini"
#define CFG_FILE_INI_ALARM			"alarm.ini"
#define CFG_FILE_INI_DBCJ			"dbcj.ini"
#define CFG_FILE_INI_FORWARD		"forward.ini"
#define CFG_FILE_INI_NORMAL_TASK	"normalt.ini"
#define CFG_FILE_INI_FORWARD_TASK		"forwardt.ini"
//#define CFG_FILE_INI_GATHERFNRGE	"gather_fnreg.ini"
#define CFG_FILE_INI_POWER_LIMIT	"powlimit.ini"
#define CFG_FILE_INI_PWR_CTRL		"pwrctrl.ini"
#define CFG_FILE_INI_DL_CTRL		"dlctrl.ini"
#define CFG_FILE_INI_ANALOG		"analog.ini"
#define CFG_FILE_INI_PULSE			"pulse.ini"
#define CFG_FILE_INI_SUM			"sum.ini"
#define CFG_FILE_INI_DIFF_RULE		"diffrule.ini"
#define CFG_FILE_INI_JLCY			"jlcy.ini"
#define CFG_FILE_INI_FHKZ			"fhkz.ini"
#define CFG_FILE_INI_TASK_TEMP		"tasktemp.ini"
#define CFG_FILE_INI_CTCY			"ctcy.ini"
#define CFG_FILE_INI_ZONE			"zone.ini"
#define CFG_FILE_INI_MPLIMIT		"mplimit.ini"
#define CFG_FILE_INI_ADDRESS		"address.ini"

#endif//__MP_CFG_USRPATH_H__

