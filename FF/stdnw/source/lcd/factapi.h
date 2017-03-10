/** @file
 *  @brief 终端及驱动应用接口
 *  @author 
 *  @date 2012/11/07
 *  @version 0.1
 */
#ifndef __LCD_FACTORY_API_H__
#define __LCD_FACTORY_API_H__
#include "termapi.h"

class C_GwTermApi:public C_TermApi
{
public:
	virtual int EnableProcessHeartBeat(bool fEnable);

	virtual C_CONNECT *CreateConnect(int ChannelType, const S_PORTSET &PortSet);
	virtual void DestroyConnect(C_CONNECT *pConnect);

	virtual bool EnableWatchDog(bool fEnable);
	virtual bool SoftwareReset(void);
	virtual bool EnableProcess(const char *pProcess, bool fEnable);

	/** @param[in] BatteryStandTimeM  电池供电时间长度，用于切电。默认为3分钟
	  * @param[in] TermStandTimeS  掉电无电池情况下，终端切电时间
	  */
	virtual int InitPowerCutInfo(S_BYTE BatteryStandTimeM, S_BYTE TermStandTimeS);
	virtual int GetPowerCutTimes(void);
};


#endif //__LCD_FACTORY_API_H__
