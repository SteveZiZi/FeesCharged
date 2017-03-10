/** @file
 *  @brief 缺省区域插件
 *  @author 
 *  @date 2014/08/30
 *  @version 0.1
 *  @see appcmd.h
 */
#include "def_version.h"

/** @brief 区域插件系统调用接口 
 *  @note 请不要更改本部分代码
 */
//{{{
int _ZonePluginRegister(void);
extern "C"{int ZonePluginRegister(void){return _ZonePluginRegister();}}
//}}}


/** @brief 插件对象定义
 */
static C_DefZoneVersion g_objDefZoneVersion;

/** @brief 插件注册函数
 *  @return 0 成功；-1 失败
 */
int _ZonePluginRegister(void)
{
	C_DefZoneVersion::SetZoneVersionObject(&g_objDefZoneVersion);
	return 0;
}

