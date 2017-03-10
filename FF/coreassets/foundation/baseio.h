/** @file
 *  @brief 基础输入输出操作
 *  @author 
 *  @date 2011/11/07
 *  @version 0.1
 *  1、驱动封装，提供友好的接口给用户。
 *  2、基础底层函数封装。
 */
#ifndef __FND_BASEIO_H__
#define __FND_BASEIO_H__

#ifndef WIN32
/** @brief 读取RTC时钟
 *  @param[out] pTime 时钟输出缓冲
 *  @return -1 失败；0 成功
 */
int io_ReadRtcTimeStamp(struct tm *pTime);
#endif

int io_WriteCmd(const char *pCmd, int state);
int io_ReadCmd(const char *pCmd);
int io_WriteData(const char *pDataID, const void *pData, unsigned int size);
int io_ReadData(const char *pDataID, void *pData, unsigned int size);

/** @brief 喂看门狗
 *  @return -1 失败；0 成功
 */
int io_Watchdog(void);

//遥信遥控
//PortNo: base 1
int io_ReomteCtrlEnable(int state);//enable:1; disable:0
int io_RemoteCtrl(unsigned char PortNo, int state);//open:1; close:0
int io_SetRemoteSmphMode(unsigned char PortNo, int mode);//smph:1; pulse:0
int io_GetRemoteSmphMode(unsigned char PortNo, int &mode);//smph:1; pulse:0
int io_RemoteSmph(int state[6]);//open:1; close:0

int io_RemoteCtrlLedLight(unsigned char PortNo, int on); //PortNo[1-4]
int io_PowerCtrlLedLight(int on);// 1:light; 0:dark
int io_EnergyCtrlLedLight(int on);// 1:light; 0:dark
int io_RunLedLight(int on);// 1:light; 0:dark
int io_AlarmLedLight(int on);// 1:light; 0:dark
int io_CtrlAlarmLedLight(int on);// 1:light; 0:dark
int io_KeepPowerLedLight(int on);// 1:light; 0:dark

/** @设置电池电量临界值*/
int io_SetBatteryCriticalValue(int minCap, int MaxCap);
/** @return -1:error 0:no battery [1-99]:cap */
int io_GetBatteryCap(void);
int io_BatteryCharge(bool bCharge);// true: charge
int io_BatteryEnable(bool fEnable); // true: 电池供电; false:电池不供电
/** @return -1:error; 0:power off; 1:power on*/
int io_GetPowerState(void);

/** @return -1:error; 0:OK*/
int io_GetCT_PhaseA(int &CT, int &increment);
int io_GetCT_PhaseB(int &CT, int &increment);
int io_GetCT_PhaseC(int &CT, int &increment);

/** MODEM 驱动
 */
 enum
{
	IO_MODEM_DRIVER_DEFAULT,
	IO_MODEM_DRIVER_EAC_5000D,
	IO_MODEM_DRIVER_CQ_6100,
	
	IO_MODEM_DRIVER_MAX
};
int io_SetModemDriverObj(int ModemDriverID);
int io_IgniteGprsModem(int PowerOnDelayS);
int io_PowerOffGprsModem(int PowerOffDelayS);

int io_BellControl(int state);
int io_EsamReset(int state);

int io_ResetPLC(int DelayS);
int io_ResetYK(int DelayS);
int io_GetStatePLC(void); // 0: 有模块; 1: 无模块
int io_GetStateYK(void); // 0: 有模块; 1: 无模块
int io_GetStateModem(void);

int io_GetProgramKey(int fd);
int io_GetCoverKey(void);

int io_InitIRDA(void); //初始化红外

/** EAC5000D watchdog driver
 */
int io_ClrWatchdog(void);
int io_SetWatchdog(void);

//
// Pstn Modem io
//
void io_SetRTS(int fd, bool fClear);
void io_SetDTR(int fd,bool fClear);
void io_SetCD(int fd,bool fClear);
bool io_HasCD(int fd);
void io_SetRing(int fd,bool fClear);
bool io_HasRing(int fd);

//
// EAC5000C 串转网模块操作
//
int io_Ser2netSetMode(int mode);
int io_Ser2netReset(void);

#endif//__FND_BASEIO_H__


