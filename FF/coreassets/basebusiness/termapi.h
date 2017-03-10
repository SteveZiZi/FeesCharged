/**
 *  @file
 *  @brief 终端及驱动接口定义和实现
 *  @author 
 *  @date 2012-10-30
 *  @version v1.0
 *  @note 
 */
#ifndef __BASEB_TERM_API_H__
#define __BASEB_TERM_API_H__
#include "connect.h"

enum
{
	REMOTE_SMPH_STATE_ERROR = -1,
	REMOTE_SMPH_STATE_CLOSE = 0,
	REMOTE_SMPH_STATE_OPEN = 1
};
enum
{
	REMOTE_SMPH_MODE_PULSE = 0,
	REMOTE_SMPH_MODE_SMPH = 1
};

class C_TermApi
{
public:
	C_TermApi(void);
	virtual ~C_TermApi(void);

public:
	virtual int EnableProcessHeartBeat(bool fEnable); 

	virtual C_CONNECT *CreateConnect(int ChannelType, const S_PORTSET &PortSet);
	virtual void DestroyConnect(C_CONNECT *pConnect);

	/** @return -1:error; 0:power off; 1:power on*/
	virtual int PowerState(void);
	virtual int EnableBattery(bool fEnable);

	virtual int EnableRemoteCtrl(bool fEnable);
	virtual int SetRemoteCtrl(INT8U PortNO, bool fClose);
	virtual int RemoteSemaphore(INT8U PortNO);
	virtual int SetRemoteSmphMode(INT8U PortNO, int mode);

	virtual bool EnableProcess(const char *pProcess, bool fEnable);
	virtual bool CheckSystemBattery(int &Capability);
	virtual bool EnableWatchDog(bool fEnable);
	virtual bool SoftwareReset(void);

	virtual int ClearScreen(int color);
	virtual int DrawRect(int x, int y, int w, int h, int color);
	virtual int ScanKeyboard(bool fWaiting);
	virtual int BackLight(bool fOn);

	virtual int AlarmLedLight(bool fOn);
	/** @return -1:error 0:ok 2:finish
	 */
	virtual int RemoteCtrlLedLight(unsigned char PortNo, bool fOn);
	/** @return -1:error 0:ok 2:finish
	 */
	virtual int PowerCtrlLedLight(unsigned char PortNo, bool fOn);

	/** @param[in] BatteryStandTimeM  电池供电时间长度，用于切电。默认为3分钟
	  * @param[in] TermStandTimeS  掉电无电池情况下，终端切电时间
	  */
	virtual int InitPowerCutInfo(S_BYTE BatteryStandTimeM, S_BYTE TermStandTimeS);
	virtual int GetPowerCutTimes(void);

protected:
	int DrawPixel(int x, int y, int color);	
};

#endif //__BASEB_TERM_API_H__

