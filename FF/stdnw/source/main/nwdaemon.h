/** @file
 *  @brief �����ػ����̷�����չ
 *  @author 
 *  @date 2011/12/26
 *  @version 0.1
 *  @see mprocess/daemon.h
 */
#ifndef __NW_DAEMON_H__
#define __NW_DAEMON_H__
#include "daemon.h"
#include "batteryb.h"

class C_BATTERY:public C_BatteryBase
{
public:
	C_BATTERY(void);
	~C_BATTERY();

protected:
	/** @brief �����ж�
	 *  @return true ϵͳ���磻false ϵͳδ����
	 */
	virtual bool GetPowerOff(void);
	/** @brief ˢ�³��״̬
	 *  @param[in] fCharge true ���ڳ��;false δ���
	 */
	virtual int SetBatteryChargeStatus(bool fCharge);
	/** @brief ˢ�µ�ǰ��ص���
	 *  @param[in] Capability ��ǰ��ص���
	 */
	virtual int SetBatteryCapability(int Capability);
	/** @brief ȡ�����С������
	 *  @return �����С������
	 */
	virtual int GetBatteryChargeLimit(void);
	/** @brief ȡ���������
	 *  @return ���������
	 */
	virtual int GetBatteryChargeFull(void);
	/** @brief ȡ��س��ʱ��
	 *  @return ��س��ʱ��
	 *  @remarks ��λ����
	 */
	virtual int GetBatteryChargeHours(void);
};


/** @class
 *  @brief �����ػ����̷�����
 */
class C_NwDaemonServer:public C_DaemonServer
{
public:
	C_NwDaemonServer(void);
	virtual ~C_NwDaemonServer();

	/** @brief ���������ڷ������ */
	virtual int BeforeService(void);

protected:
	/** @brief ���5������ */
	virtual int CheckTask5S(void);
	/** @brief ���2������ */
	virtual int CheckTask2S(void);
	/** @brief ���1������ */
	virtual int CheckTask1S(void);
	/** @������Ϣ֪ͨ */
	virtual bool UpdateNotify(int msg);

	int CheckUsbDiskPlugin(void);
	int CheckNetLinePlugin(void);
	int CheckPowerOff(void);
	int Alarm_PowerOn(S_TIME timePowerCut);
	int Alarm_PowerDown(void);
	int CheckBatteryDisable(void);
	/* �ն��ϵ��⺯��*/
	bool CheckPowerCutAlarm(void);
	bool FreshSoftVersion(void);
	bool FreshZoneInformation(void);

protected:
	S_TIME m_tickPowerCut; ///< �е翪ʼʱ��
	C_BATTERY m_Battery;
	int m_CtrlAlarmLed; ///< �澯�ƿ���
};

#endif//__NW_DAEMON_H__


