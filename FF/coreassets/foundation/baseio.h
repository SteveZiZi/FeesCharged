/** @file
 *  @brief ���������������
 *  @author 
 *  @date 2011/11/07
 *  @version 0.1
 *  1��������װ���ṩ�ѺõĽӿڸ��û���
 *  2�������ײ㺯����װ��
 */
#ifndef __FND_BASEIO_H__
#define __FND_BASEIO_H__

#ifndef WIN32
/** @brief ��ȡRTCʱ��
 *  @param[out] pTime ʱ���������
 *  @return -1 ʧ�ܣ�0 �ɹ�
 */
int io_ReadRtcTimeStamp(struct tm *pTime);
#endif

int io_WriteCmd(const char *pCmd, int state);
int io_ReadCmd(const char *pCmd);
int io_WriteData(const char *pDataID, const void *pData, unsigned int size);
int io_ReadData(const char *pDataID, void *pData, unsigned int size);

/** @brief ι���Ź�
 *  @return -1 ʧ�ܣ�0 �ɹ�
 */
int io_Watchdog(void);

//ң��ң��
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

/** @���õ�ص����ٽ�ֵ*/
int io_SetBatteryCriticalValue(int minCap, int MaxCap);
/** @return -1:error 0:no battery [1-99]:cap */
int io_GetBatteryCap(void);
int io_BatteryCharge(bool bCharge);// true: charge
int io_BatteryEnable(bool fEnable); // true: ��ع���; false:��ز�����
/** @return -1:error; 0:power off; 1:power on*/
int io_GetPowerState(void);

/** @return -1:error; 0:OK*/
int io_GetCT_PhaseA(int &CT, int &increment);
int io_GetCT_PhaseB(int &CT, int &increment);
int io_GetCT_PhaseC(int &CT, int &increment);

/** MODEM ����
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
int io_GetStatePLC(void); // 0: ��ģ��; 1: ��ģ��
int io_GetStateYK(void); // 0: ��ģ��; 1: ��ģ��
int io_GetStateModem(void);

int io_GetProgramKey(int fd);
int io_GetCoverKey(void);

int io_InitIRDA(void); //��ʼ������

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
// EAC5000C ��ת��ģ�����
//
int io_Ser2netSetMode(int mode);
int io_Ser2netReset(void);

#endif//__FND_BASEIO_H__


