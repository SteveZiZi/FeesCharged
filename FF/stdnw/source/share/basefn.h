/** @file
 *  @brief �û���ϵͳ������������
 *  @author 
 *  @date 2011/12/06
 *  @version 0.1
 */
#ifndef __NW_BASE_FUNCTION_H__
#define __NW_BASE_FUNCTION_H__
#include "basedef.h"
#include "mptype.h"

/** @brief �����л�������
 */
class C_ZoneSwh
{
public:
	static int LoadZoneInfo(string &strCurrentZone, S_ZoneCfgs &ZoneList);
	static int SwitchZone(const string &strZoneFile);
};

#endif // __NW_BASE_FUNCTION_H__


