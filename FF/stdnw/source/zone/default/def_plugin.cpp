/** @file
 *  @brief ȱʡ������
 *  @author 
 *  @date 2014/08/30
 *  @version 0.1
 *  @see appcmd.h
 */
#include "def_version.h"

/** @brief ������ϵͳ���ýӿ� 
 *  @note �벻Ҫ���ı����ִ���
 */
//{{{
int _ZonePluginRegister(void);
extern "C"{int ZonePluginRegister(void){return _ZonePluginRegister();}}
//}}}


/** @brief ���������
 */
static C_DefZoneVersion g_objDefZoneVersion;

/** @brief ���ע�ắ��
 *  @return 0 �ɹ���-1 ʧ��
 */
int _ZonePluginRegister(void)
{
	C_DefZoneVersion::SetZoneVersionObject(&g_objDefZoneVersion);
	return 0;
}

