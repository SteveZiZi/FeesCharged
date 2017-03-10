/** @file
 *  @brief 广州区域插件
 *  @author 
 *  @date 2014/08/30
 *  @version 0.1
 *  @see appcmd.h
 */
#include "gz_version.h"
#include "gz_lcd.h"
#include "nwappcmd.h"
#include "cfg_proc.h"

/** @brief 区域插件系统调用接口 
 *  @note 请不要更改本部分代码
 */
//{{{
int _ZonePluginRegister(void);
extern "C"{int ZonePluginRegister(void){return _ZonePluginRegister();}}
//}}}


/** @brief 插件对象定义
 */
static C_GzZoneVersion g_objGzZoneVersion;
static C_GzLcdServer g_objGzLcdServer;

/** @brief 插件注册函数
 *  @return 0 成功；-1 失败
 */
int _ZonePluginRegister(void)
{
	C_GzZoneVersion::SetZoneVersionObject(&g_objGzZoneVersion);
	C_NwAppCmd::SetServer(MP_PROCESS_LCD, &g_objGzLcdServer);
	return 0;
}


