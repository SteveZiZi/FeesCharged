/** @file
 *  @brief 用户层系统基础函数定义
 *  @author 
 *  @date 2011/12/06
 *  @version 0.1
 */
#include "basefn.h"
#include "sysbase.h"
#include "status.h"
#include "inizone.h"
#include "cfg_path.h"
#include "sysctrl.h"

/** @brief 区域切换方法集
 */
int C_ZoneSwh::LoadZoneInfo(string &strCurrentZone, S_ZoneCfgs &ZoneList)
{
	S_SYSCFG *pSysCfg = STATUS.GetSystemConfig();
	if (pSysCfg != NULL && pSysCfg->m_ZoneName[0] != '\0')
		strCurrentZone.append(pSysCfg->m_ZoneName);
	else
		strCurrentZone.append("未知地区");

	C_IniZone::LoadZoneCfg(ZoneList, CFG_FILE_INI_ZONE);
	return (int)ZoneList.size();
}

int C_ZoneSwh::SwitchZone(const string &strZoneFile)
{
	string strScriptCmd;
	char script[CFG_PATH_FILE_MAX_LEN];
	C_PATH::GetScriptFile(CFG_FILE_SCRIPT_ZONE_UPDATE, script);
	strScriptCmd.append(script).append(" ").append(strZoneFile);

	C_SYSCTRL::DataClear();  // 清除上一区域的残留数据
	C_SYSCTRL::UserConfigClear();  // 清除上一区域的残留配置
	C_SYSCTRL::ExecScript(strScriptCmd.c_str());

	SleepSecond(2);
	C_SYSCTRL::SystemReboot();
	return 0;
}


