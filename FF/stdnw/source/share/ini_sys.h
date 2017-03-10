/** @file
 *  @brief 系统配置加载和保存
 *  @author 
 *  @date 2011/12/06
 *  @version 0.1
 *  定义系统配置加载和保存函数
 */
#ifndef __NW_INI_SYS_H__
#define __NW_INI_SYS_H__
#include "cfg_stat.h"
#include "cfg_cont.h"
#include "cfg_usrp.h"

class C_IniSys
{
public:
	static int LoadConfigVersion(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_CFGVER);
	static int LoadSystemConfig(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_SYSCFG);
	static int SaveSystemConfig(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_SYSCFG);
	static int _LoadSystemConfig(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_SYSCFG);
	static int _SaveSystemConfig(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_SYSCFG);
	static int LoadSystemAddress(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_ADDRESS);
	static int SaveSystemAddress(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_ADDRESS);

	static int LoadRunInformation(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_SYSRUN);
	static int SaveRunInformation(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_SYSRUN);
	static int LoadPortMap(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_PORTMAP);
	static int LoadCommReg(S_STATUS *pStatus, const char *fname = NULL);

	static int LoadTermIPConfig(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_IPCONFIG);
	static int SaveTermIPConfig(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_IPCONFIG);

	static int LoadFlowCtrlInfo(S_CONTEXT *pContext, const char *fname = CFG_FILE_INI_FLOWCTRL);
	static int SaveFlowCtrlInfo(S_CONTEXT *pContext, const char *fname = CFG_FILE_INI_FLOWCTRL);

	static int LoadAlarmInfo(S_STATUS *pContext, const char *fname = CFG_FILE_INI_ALARM);
	static int SaveAlarmInfo(S_STATUS *pContext, const char *fname = CFG_FILE_INI_ALARM);

	static int LoadForwardInfo(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_FORWARD);
	static int SaveForwardInfo(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_FORWARD);

	static int LoadTaskTemplateInfo(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_TASK_TEMP);
	static int LoadMsdiInfo(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_MSDI);

	static int LoadPowerLimitInfo(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_POWER_LIMIT);
	static int SavePowerLimitInfo(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_POWER_LIMIT);

	static int LoadAnalogInfo(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_ANALOG);
	static int SaveAnalogInfo(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_ANALOG);

	static int LoadPulseInfo(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_PULSE);
	static int SavePulseInfo(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_PULSE);

	static int LoadSumInfo(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_SUM);
	static int SaveSumInfo(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_SUM);

	static int LoadDiffRuleInfo(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_DIFF_RULE);
	static int SaveDiffRuleInfo(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_DIFF_RULE);

protected:
	static S_BYTE GetTermType(const char *pTermTypeName);
	static const char *GetTermTypeName(S_BYTE TermType);
	static S_BYTE GetMsdiTypeID(const char *pTypeName);
	static S_BYTE GetMsdiFreezeType(const char *pTypeName);
};

#endif//__NW_INI_SYS_H__

