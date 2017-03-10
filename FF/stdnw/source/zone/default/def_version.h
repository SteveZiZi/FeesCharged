/** @file
 *  @brief ȱʡ������ �汾��Ϣ
 *  @author 
 *  @date 2014/08/30
 *  @version 0.1
 *  @see appcmd.h
 */
#ifndef __DEFAULT_VERSION_H__
#define __DEFAULT_VERSION_H__
#include "zonebase.h"

//
// �汾��Ϣ����
//
#define DEFAULT_ZONE_PLUGIN_NAME_E	"default"
#define DEFAULT_ZONE_PLUGIN_NAME_C	"����"
#define DEFAULT_ZONE_PLUGIN_VERSION		"1.01"
#define DEFAULT_ZONE_PLUGIN_RELEASE_DATE	"2016-05-06"


class C_DefZoneVersion:public C_ZoneVersion
{
public:
	virtual const char *GetZonePluginNameE(void);
	virtual const char *GetZonePluginNameC(void);
	virtual const char *GetZonePluginVersion(void);
	virtual const char *GetZonePluginReleaseDate(void);
};

#endif // __DEFAULT_VERSION_H__

