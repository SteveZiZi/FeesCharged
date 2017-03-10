/** @file
 *  @brief 区域配置信息加载
 *  @author 
 *  @date 2011/10/15
 *  @version 0.1
 *  @see foundation/confuse inifileb.h
 */
#ifndef __MP_INI_ZONE_H__
#define __MP_INI_ZONE_H__
#include "inifileb.h"
#include "mptype.h"

// 区域配置
class C_IniZone
{
public:
	static int LoadZoneCfg(S_ZoneCfgs &ZoneCfgs, const char *fname = NULL);
};

#endif //__MP_INI_ZONE_H__
