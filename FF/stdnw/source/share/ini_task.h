/** @file
 *  @brief 任务加载和保存
 *  @author 
 *  @date 2011/12/06
 *  @version 0.1
 *  采表任务和主站上报任务都在此处定义加载和保存函数
 */
#ifndef __NW_INI_TASK_H__
#define __NW_INI_TASK_H__

#include "sysbase.h"
#include "inifileb.h"
#include "cfg_stat.h"
#include "confuse.h"
#include "cfg_usrp.h"

//#define GATHER_TASK_NAME	"抄表任务"
//#define NORMAL_TASK_NAME	"普通任务"

class CInitTask
{
public:
	static int LoadGatherTasks(S_STATUS *pStatus,const char* name = CFG_FILE_INI_TASK);
	static int SaveGatherTasks(S_STATUS *pStatus,const char* name = CFG_FILE_INI_TASK);
	
	static int LoadNormalTask(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_NORMAL_TASK);
	static int SaveNormalTask(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_NORMAL_TASK);

	static int LoadForwardTask(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_FORWARD_TASK);
	static int SaveForwardTask(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_FORWARD_TASK);

private:
	static int GetPeriodUnitVal(const char *pName);
	static const char *GetPeriodUnitName(int val);
	static S_TIME GetStandardTime(const char *pdate, const char *ptime);
	static void GetStandardTime(S_TIME timeVal, char *pdate, char *ptime);
	static S_WORD EncodeAsciiCmd2HexCmd(string &strHex, const string &strAscii);
	static S_WORD DecodeHexCmd2AsciiCmd(string &strAscii, const string &strHex);
};

#endif//__NW_INI_TASK_H__

