/** @file
 *  @brief �����������ӿ�
 *  @author 
 *  @date 2014/08/30
 *  @version 0.1
 *  @see appcmd.h
 */
#ifndef __ZONE_PLUGIN_BASE_H__
#define __ZONE_PLUGIN_BASE_H__

//
//�������汾��Ϣ
//
class C_ZoneVersion
{
public:
	static int SetZoneVersionObject(C_ZoneVersion *pObj);
	static C_ZoneVersion *GetZoneVersionObject(void);

public:
	virtual const char *GetZonePluginNameE(void) = 0;
	virtual const char *GetZonePluginNameC(void) = 0;
	virtual const char *GetZonePluginVersion(void) = 0;
	virtual const char *GetZonePluginReleaseDate(void) = 0;

protected:
	static C_ZoneVersion *m_pZoneVersionObject;
};

//
// �����ڴ����
//
class C_ZoneShm
{
public:
	static int SetZoneSharedMemoryObject(C_ZoneShm *pObj);
	static C_ZoneShm *GetZoneSharedMemoryObject(void);

public:
	virtual int InitExtendZone_Context(void) = 0;
	virtual int LoadExtendZone_Context(void) = 0;
	virtual int InitExtendZone_Status(void) = 0;
	virtual int LoadExtendZone_Status(void) = 0;

protected:
	static C_ZoneShm *m_pZoneShmObject;
};

#endif // __ZONE_PLUGIN_BASE_H__

