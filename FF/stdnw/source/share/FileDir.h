///*FileDir.h�а������е���·���ļ���صĶ���*/

#ifndef INCLUDED_FILE_DIR
#define INCLUDED_FILE_DIR


/****************************************************************
/mnt�µ�Ŀ¼�ṹ
****************************************************************/
#ifdef WIN32
#define PATH_BASE	"./mnt"
#else
#define PATH_BASE	"/mnt"
#endif

#define PATH_DYJC_BASE	PATH_BASE"/dyjc"//��־ǿ���

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

#define CONS_FILE_UPDATE_LOG PATH_BASE"/updatelog.dat"		//�����2006-10-19��ӣ�������־
#define CONS_UPDATE_FILE_NAME PATH_BASE"/update/update.tgz"	//�����2007-11-06��ӣ�Ĭ�ϵ��������ļ�
#define CONS_AUT0_UPDATE_KEY_FILE_NAME_TEMP PATH_BASE"/update/update.key_temp"		//�����2007-11-12��ӣ�Ĭ�ϵ�����֪ͨ�ļ���ʱ�ļ�
#define CONS_AUTO_UPDATE_KEY_FILE_NAME PATH_BASE"/update/auto_update.key"		//�����2007-11-06��ӣ�Ĭ�ϵ�����֪ͨ�ļ�
#define CONS_JILIAN_UPDATE_KEY_FILE_NAME PATH_BASE"/update/jilian_update.key"	//�����2007-11-12��ӣ�Ĭ�ϵļ�������֪ͨ�ļ�
#define CONS_AUTO_SAVE_LSTNODE_FILE_NAME PATH_BASE"/data/save/lst_nodes.bak"   
//*************************************will remove
#define FILE_CFG_SYS_IP	FILE_KELI_ETH0_IP
//*************************************************

#define FILE_DYJC_SYS_CONFIG PATH_BASE"/keli/dyjc_sys_config.ini"	//dyjcϵͳ��ϵͳ����//��־ǿ���
#define FILE_TERMINAL_CONFIG PATH_BASE"/keli/terminal_config.ini"	//�ն���Ϣ����
#define CFG_FILE_INI_PORTMAP	PATH_BASE"/keli/portmap.ini" //�˿������ļ�

#define FILE_KELI_ETH0_IP		PATH_BASE"/dyjc/script/eth0_ip.ini"				//ϵͳIP��ַ�����ļ�
#define FILE_KELI_ETH1_IP		PATH_BASE"/dyjc/script/eth1_ip.ini"				//ϵͳIP��ַ�����ļ�
#define CFG_LOCAL_IP 			PATH_BASE"/keli/local_ip.ini"	//����ip

#define PATH_SET_SOURCE PATH_BASE"/dyjc/set"						//��־ǿ���
#define PATH_SET_USER PATH_BASE"/user/dyjc/set"					//��־ǿ���

#define NAME_SET_METER "meter.ini"							//��������ļ���
#define NAME_SET_TASK "task.ini"							//���������ļ���
#define NAME_SET_NORTASK "nortask.ini"							//���������ļ���
#define NAME_SET_HTTX "httx.ini"							//��̨ͨѶ�����ļ���
#define NAME_SET_GPRS "gprs.ini"							//GPRSͨ����չ��������Ҫ����GPRS���������Ż���2008-2-29���
#define NAME_SET_GDDBCJ "gddbcj.ini"						//���ͨѶ�����ļ���
#define NAME_SET_GWDBCJ "gwdbcj.ini"						//���ͨѶ�����ļ���
#define NAME_SET_TRACE "trace.ini"							//���������ļ���
#define NAME_SET_PPP "ppp.ini"								//ppp�����ļ�
#define NAME_SET_HTTX_PLAN	"httx_plan.ini"					//��̨ͨѶ������
#define NAME_SET_JILIAN "jilian.ini"						//�������������ļ���2006-06-07������������
#define NAME_SET_DISTRIBUTE "JilianSlaves.ini"				//�������ն������ļ�
#define NAME_SET_FIXDATA "FixData.ini"						//�������������ļ�
#define NAME_SET_ROUTERSERVER "RouterServer.ini"			//�Զ�·�ɽ��������ļ� ����07-06-14
#define CFG_FILE_INI_IPCONFIG		"ipconfig.ini"
#define NAME_SET_RELAY "Relay.ini"                          //�ɼ��ն������ļ�
#define NAME_SET_CLRELAY "ClRelay.ini"                          //��·�ɼ��ն������ļ�
#define NAME_SET_ROUTER "Router.ini"                        //·�ɱ������ļ�
#define NAME_SET_ZBSNIFFER "ZBSnifferTask.ini"              //�ز�����������������ļ�
#define NAME_SET_DNPROTOCOL "DnProtocolType.ini"			//�Զ�·�ɽ��������ļ� ����07-06-14

#define NAME_SET_EVENT "event.ini"							//�¼������ļ���
//#define NAME_SET_EVENT_ID "event_id.ini"					//�¼�ID�����ļ���
//#define NAME_SET_DEVICE_RULE "drule.ini"					//װ�ù�Լ������
//#define NAME_SET_METER_RULE "mrule.ini"					//����Լ������
#define NAME_SET_ACTION "action.ini"						//����������

//#define NAME_SET_PPP_APN				"ppp_apn.ini"
//#define NAME_SET_PPP_TELECODE		"ppp_telecode.ini"
//#define NAME_SET_PPP_USERNAME		"ppp_username.ini"
//
#define PATH_SCRIPT_SOURCE PATH_BASE"/dyjc/script"				//��־ǿ���
#define PATH_SCRIPT_USER PATH_BASE"/user/dyjc/script"			//��־ǿ���
#define METER_OPEN_CLOSE_LOGFILE PATH_BASE"/data/meter_open_logfile.dat"	//��־ǿ��ӵ������բ��־�ļ�

#define NAME_SCRIPT_ON_START			"on_start.srt"				//ϵͳ����ʱ���ô˽ű�
#define NAME_SCRIPT_ON_STOP				"on_stop.srt"				//ϵͳֹͣʱ���ô˽ű�
#define NAME_SCRIPT_ON_MAIN_UPDATE		"on_djs_update.srt"			//���յ��������ļ�����ô˽ű�
#define NAME_SCRIPT_ON_SYS_UPDATE		"on_sys_update.srt"			//���յ�ϵͳ�����ļ�����ô˽ű�
#define NAME_SCRIPT_MEM_RESET			"on_mem_reset.srt"			//�ڴ渴λ����ʱ���ô˽ű�
#define NAME_SCRIPT_DATA_RESET			"on_data_reset.srt"			//���ݸ�λ����ʱ���ô˽ű�
#define NAME_SCRIPT_PARAM_RESET			"on_param_reset.srt"		//���ݸ�λ����ʱ���ô˽ű�
#define NAME_SCRIPT_CLEAR_LOGS			"on_clear_logs.srt"			//���ϵͳ��־ʱ���ô˽ű�
#define NAME_SCRIPT_FACTORY_RESET		"on_factory_reset.srt"		//�ָ���������ʱ���ô˽ű�
#define NAME_SCRIPT_SOFT_RESET			"on_soft_reset.srt"			//��λʱ���ô˽ű�
#define NAME_SCRIPT_SET_REPORT_DAY		"on_set_report_day.srt"		//����������ϱ���־�ű�
#define NAME_SCRIPT_SET_REPORT_MONTH	"on_set_report_month.srt"	//����������ϱ���־�ű�
#define NAME_SCRIPT_PPP_GPRS_ON			"ppp_gprs_on.srt"			//PPP�������߽ű�(GPRS)
#define NAME_SCRIPT_PPP_GPRS_OFF		"ppp_gprs_off.srt"			//PPP�������߽ű�(GPRS)
#define NAME_SCRIPT_PPP_CDMA_ON			"ppp_cdma_on.srt"			//PPP�������߽ű�(CDMA)
#define NAME_SCRIPT_PPP_CDMA_OFF		"ppp_cdma_off.srt"			//PPP�������߽ű�(CDMA)
#define NAME_SCRIPT_ETH0_IPCONFIG		"ipconfig.sh"			//eth0 set
#define NAME_SCRIPT_CAN0_IPCONFIG		"can0config.srt"			//eth0 set


//����Լ����
#define PATH_METER_LIB PATH_BASE"/dyjc/meterlib/"				//��־ǿ���

#define FILE_COMMON_CFG_LGBLIB	PATH_METER_LIB"reg_lgb.ini"
#define FILE_COMMON_CFG_LGDLIB	PATH_METER_LIB"reg_lgd.ini"
#define FILE_COMMON_CFG_MKLIB PATH_METER_LIB"reg_mk.ini"
#define FILE_COMMON_CFG_WSLIB  PATH_METER_LIB"reg_ws.ini"
#define FILE_COMMON_CFG_GB645LIB  PATH_METER_LIB"reg_gb645.ini"
#define FILE_COMMON_CFG_GB2007LIB  PATH_METER_LIB"reg_gb2007.ini"

#define FILE_DYJC_STATUS	PATH_BASE"/user/dyjc/data/status.dat"				//DYJCϵͳ��״̬�����ڴ��ļ�//��־ǿ���

//#define PATH_COMMON_RESOURCE_BMP_KELI		"/mnt/keli/resource/bmp/keli.bmp"	//Һ����ʾͼ��Ŀ¼
#define FILE_COMMON_RESOURCE_HZK_HZK12  PATH_BASE"/keli/hzk12.bin"

//===============================================================================================================================
//��־ǿ��� ���ǰ�eac�滻Ϊdyjc
#define FILE_COMMON_SCRIPT_LAST_EXEC_UPDATE_BAK		PATH_BASE"/data/common/last_exec_update.srt" //�������һ��ִ���������ļ�
//#define FILE_COMMON_CONFIG_LCDPASSW					"/mnt/data/common/lcdpassw.dat"		//Һ���û������ļ�
//#define FILE_COMMON_CONFIG_LCD_BASEINFO				"/mnt/data/common/lcdbaseinfo.dat"		//Һ��������Ϣ
//#define FILE_COMMON_DATA_LCD_MESSAGE				"/mnt/data/common/message.dat"			//����Ϣ�ļ�

#define FILE_COMMON_TASK_DATA						PATH_BASE"/data/task/task"

#define FILE_TRACE_PUBLIC_TRACE						PATH_BASE"/data/log/trace"
#define FILE_TRACE_PUBLIC_DATA						PATH_BASE"/data/log/data"
#define FILE_TRACE_PUBLIC_SCHEDULE					PATH_BASE"/data/log/sche"

#define FILE_DYJC_CONTEXT PATH_BASE"/ramdisk/dyjc/context.dat"	//DYJCϵͳ���������ڴ湲���ļ�

//GDDYJC====================================================================================
#define FILE_GDDYJC_POWEROFF_DATA PATH_BASE"/data/alarm/alarm_poweroff.dat"			//ͣ���¼���־
#define FILE_GDDYJC_WASTE_ALARM_TMP PATH_BASE"/data/tmp/waste.dat"					//���𱨾��ݴ��ļ�
#define FILE_GDDYJC_GATHER_FAIL_LOG	PATH_BASE"/data/tmp/gathfail.dat"				//����ʧ�ܼ�¼
#define FILE_GDDYJC_SYSTEM_LOG PATH_BASE"/data/log/syslog.dat"						//ϵͳ�¼���¼
#define FILE_GDDYJC_BREAK_LOG PATH_BASE"/data/tmp/breaklog.dat"						//����բ״̬����־
//GDDYJC====================================================================================

#define FILE_ENERGY_SHARE PATH_BASE"/ramdisk/data/energy.bin"	//�����Ĵ����ļ�
#define FILE_LOG_GATHERTIME PATH_BASE"/data/log/gather.dat"	//���ɼ�����־
#define FILE_LOG_ZBSNIFFERLOG PATH_BASE"/data/log/zbsnifferlog.dat"	//�ز����������־
#define FILE_EVENT_ALARM_DATA PATH_BASE"/data/alarm/alarm_tmp.dat"		//����¼�������¼�ļ�

//�շ�ʱ���ݴ��λ��	  �д��޸� ��־ǿ+06-6-24
#define FILE_COMMON_DATA_IFCONFIG			PATH_BASE"/ramdisk/ramdyjc/common/data/ifconfig.log"	//��ʱ��ѯϵͳ�����ļ�
	
#define FILE_DATA_TASK_MONTH_DATA				PATH_BASE"/data/task/monthFSdata.dat"			//�·�ʱ����
#define FILE_DATA_TASK_DAY_DATA				PATH_BASE"/data/task/dayLDdata.dat"					//����������
#define DALY_LEN 24					//�ϱ��ճ��ۺ�����ʱÿ֡���������ݵ�Ԫ��.�������.
#endif
