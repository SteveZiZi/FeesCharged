/** @file
 *  @brief 系统服务加载和保存
 *  @author 
 *  @date 2011/12/06
 *  @version 0.1
 *  定义系统服务加载和保存函数
 */
#ifndef __NW_INI_SERVER_H__
#define __NW_INI_SERVER_H__
#include "inifileb.h"
#include "cfg_stat.h"
#include "cfg_usrp.h"

class C_IniServer
{
public:
	static int LoadUpChannl(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_UPCHANNL);
	static int SaveUpChannl(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_UPCHANNL);
	static int LoadDnChannl(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_DNCHANNL);
	static int SaveDnChannl(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_DNCHANNL);

	static int LoadHttxInfo(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_HTTX);
	static int SaveHttxInfo(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_HTTX);
	static int LoadPppInfo(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_PPP);
	static int SavePppInfo(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_PPP);
	static int LoadLcdInfo(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_LCD);
	static int SaveLcdInfo(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_LCD);
	/** @brief 加载电表采集参数配置 */
	static int LoadDbcjInfo(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_DBCJ);
	/** @brief 保存电表采集参数配置*/
	static int SaveDbcjInfo(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_DBCJ);
	static int LoadJlcyInfo(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_JLCY);
	static int SaveJlcyInfo(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_JLCY);
	static int LoadCtcyInfo(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_CTCY);
	static int SaveCtcyInfo(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_CTCY);
	static int LoadFhkzInfo(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_FHKZ);
	static int SaveFhkzInfo(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_FHKZ);

	static const char *GetHttxProcessName(int SN);
	static const char *GetDbcjProcessName(int SN);

private:
	static void LoadChannel(cfg_t* cfgChannel, struct S_CHANNEL *pChannl);
	static const char *MakeChannelTypeKey(int val);
	static int MakeChannelTypeVal(char *pKey);

	static int PortSettingParse(struct S_CHANNEL *pChannl, const char *pSetting);
	static int PortSettingPrint(struct S_CHANNEL *pChannl, char *pSetting);
};

#endif//__NW_INI_SERVER_H__

