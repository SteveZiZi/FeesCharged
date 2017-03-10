/** @file
 *  @brief 测量点配置加载和保存
 *  @author 
 *  @date 2011/12/06
 *  @version 0.1
 *  定义系统测量点配置加载和保存函数
 */
#ifndef __NW_INI_MP_H__
#define __NW_INI_MP_H__
#include "basedef.h"
#include "cfg_stat.h"
#include "inifileb.h"
#include "cfg_usrp.h"

class C_IniMP
{
public:
	static int LoadMeterInformation(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_METER);
	static int SaveMeterInformation(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_METER);
	static INT8U GetMeterRuleID(char *pRuleName);
private:
	static int SetMeterInfo(S_MeterInfo &Meter, cfg_t* cfgMeter);
	static INT8U GetMeterProperty(char *pPropertyName);
	static const char *GetMeterPropertyName(INT8U Property);
	
	static const char *GetMeterRuleName(INT8U RuleID);
	static INT8U GetMeterFunType(char *pTypeName);
	static const char *GetMeterFunTypeName(INT8U type);
};

/** @class C_IniMPLimit
 *  @brief 测量点限值参数文件加载与保存
 *  @note 测量点限值参数件名:mplimit.ini
 */
class C_IniMPLimit
{
public:
	static int LoadMPointLimit(S_STATUS *pStatus, const char* fname=CFG_FILE_INI_MPLIMIT);
	static int SaveMPointLimit(S_STATUS *pStatus, const char* fname=CFG_FILE_INI_MPLIMIT);
};

#endif//__NW_INI_MP_H__

