/** @file
 *  @brief ����������
 *  @author 
 *  @date 2014/08/30
 *  @version 0.1
 *  @see appcmd.h
 */
#include "gz_version.h"
#include "gz_lcd.h"
#include "nwappcmd.h"
#include "cfg_proc.h"

/** @brief ������ϵͳ���ýӿ� 
 *  @note �벻Ҫ���ı����ִ���
 */
//{{{
int _ZonePluginRegister(void);
extern "C"{int ZonePluginRegister(void){return _ZonePluginRegister();}}
//}}}


/** @brief ���������
 */
static C_GzZoneVersion g_objGzZoneVersion;
static C_GzLcdServer g_objGzLcdServer;

/** @brief ���ע�ắ��
 *  @return 0 �ɹ���-1 ʧ��
 */
int _ZonePluginRegister(void)
{
	C_GzZoneVersion::SetZoneVersionObject(&g_objGzZoneVersion);
	C_NwAppCmd::SetServer(MP_PROCESS_LCD, &g_objGzLcdServer);
	return 0;
}


