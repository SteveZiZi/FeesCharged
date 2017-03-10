/** @file
*  @brief 负控管理参数加载和保存
 *  @author 
 *  @date 2012/2/22
 *  @version 0.1
 */
#ifndef __NW_INI_FKCTRL_H__
#define __NW_INI_FKCTRL_H__
#include "cfg_stat.h"
#include "cfg_cont.h"
#include "cfg_usrp.h"

class C_IniFKCtrl
{
public:
	static int LoadPWRCtrlInfo(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_PWR_CTRL);
	static int SavePWRCtrlInfo(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_PWR_CTRL);
	static int LoadDLCtrlInfo(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_DL_CTRL);
	static int SaveDLCtrlInfo(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_DL_CTRL);
};

#endif//__NW_INI_FKCTRL_H__

