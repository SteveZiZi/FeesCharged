/** @file
 *  @brief 后台通讯命令工厂类定义
 *  @author 
 *  @date 2011/12/22
 *  @version 0.1
 */
#include "cmdfact.h"
#include "cmdbase.h"
#include "cmddata.h"
#include "cmdrobj.h"
#include "cmdwobj.h"

C_AFNCMD *C_CmdFact::GetAfnCmd(INT8U AFN)
{
	static C_AFNCMD sDefaultAfnCmd;
	static C_AFNCMD_CONFIRM  sAfnCmd_Confirm;
	static C_AFNCMD_WPARAM   sAfnCmd_WriteParam;
	static C_AFNCMD_RPARAM   sAfnCmd_ReadParam;
	static C_AFNCMD_REALTIME sAfnCmd_Realtime;
	static C_AFNCMD_HISTORY  sAfnCmd_history;
	static C_AFNCMD_TASK sAfnCmd_Task;
	static C_AFNCMD_ALARM sAfnCmd_Alarm;
	static C_AFNCMD_EVENT sAfnCmd_Event;
	static C_AFNCMD_FTP sAfnCmd_Ftp;
	static C_AFNCMD_ROUTE sAfnCmd_Route;
	static C_AFNCMD_FORWARD sAfnCmd_Forward;
	static C_AFNCMD_FORWARD_MS sAFNCMD_FORWARD_MS;

	switch(AFN)
	{
	case MSAFN_CONFIRM_DENY:  return &sAfnCmd_Confirm;
//	case MSAFN_AUTHENTICATE:
//	case MSAFN_LINK_CHECK:
	case MSAFN_READ_PARAM:   return &sAfnCmd_ReadParam;
	case MSAFN_READ_REALTIME: return &sAfnCmd_Realtime;
	case MSAFN_READ_HISTORY: return &sAfnCmd_history;
	case MSAFN_READ_TASK: return &sAfnCmd_Task;
	case MSAFN_READ_ALARM: return &sAfnCmd_Alarm;
	case MSAFN_READ_EVENT: return &sAfnCmd_Event;
	case MSAFN_WRITE_PARAM:  return &sAfnCmd_WriteParam;
	case MSAFN_FILE_TRANSFER: return &sAfnCmd_Ftp;
	case MSAFN_FORWARD: return &sAfnCmd_Forward;
	case MSAFN_DATA_ROUTE: return &sAfnCmd_Route;
//	case MSAFN_USER_DEFINE: return &sAfnCmd_Define;
	case MSAFN_FORWARD_MS: return &sAFNCMD_FORWARD_MS;
	default:break;
	}
	return &sDefaultAfnCmd;
}

