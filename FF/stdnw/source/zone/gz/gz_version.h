/** @file
 *  @brief ���������� �汾��Ϣ
 *  @author 
 *  @date 2014/08/30
 *  @version 0.1
 *  @see appcmd.h
 */
#ifndef __GZ_VERSION_H__
#define __GZ_VERSION_H__
#include "zonebase.h"

//
// �汾��Ϣ����
//
#define GZ_ZONE_PLUGIN_NAME_E		"gz"
#define GZ_ZONE_PLUGIN_NAME_C		"����"
#define GZ_ZONE_PLUGIN_VERSION		"1.01"
#define GZ_ZONE_PLUGIN_RELEASE_DATE	"2016-05-06"


class C_GzZoneVersion:public C_ZoneVersion
{
public:
	virtual const char *GetZonePluginNameE(void);
	virtual const char *GetZonePluginNameC(void);
	virtual const char *GetZonePluginVersion(void);
	virtual const char *GetZonePluginReleaseDate(void);
};

#endif // __GZ_VERSION_H__

