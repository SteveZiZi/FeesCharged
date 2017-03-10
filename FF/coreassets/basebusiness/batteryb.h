/**
 *  @file
 *  @brief ������������
 *  @author 
 *  @date 2011-12-26
 *  @version v1.0
 */
#ifndef __BASEB_BATTERY_BASE_H__
#define __BASEB_BATTERY_BASE_H__
#include "basedef.h"
#include "timeop.h"

/** @class
 *  @brief ��س�����
 *  @remarks ����BatteryCharge()֮ǰ�ȵ���InitDevice()
 */
class C_BatteryBase
{
public:
	enum{BATTERY_CHARGE_LIMIT=90, BATTERY_CHARGE_FULL=99};// base-on 0
	enum{CHARGE_MODE_DAY, CHARGE_MODE_LOWER};// ÿ���һ�Σ��͵������
	enum{BATTERY_CHARGE_INTER_S=86400, BATTERY_CHARGE_TIME=72000, BATTERY_CHARGE_TIME_AFTER_RESET=30};

public:
	C_BatteryBase(void);
	virtual ~C_BatteryBase();

	/** @brief ���ó��ģʽ*/
	int SetBatteryChargeMode(int ChargeMode);
	/** @���õ�ص����ٽ�ֵ*/
	int SetBatteryCriticalValue(int minCap, int MaxCap);
	/** @brief ����豸��ʼ�� */
	int InitDevice(void);
	/** @brief ������ */
	int BatteryCharge(void);

protected:
	/** @brief ������ */
	int _BatteryCharge(void);
	/** @brief �ն�һ�ϵ������ */
	int _BatteryChargeAfterReset(void);
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

protected:
	int _BatteryCharge_LowerMode(int BatteryLevel);
	int _BatteryCharge_DayMode(int BatteryLevel);

	int BatteryChargeProcess(int CurBatteryLevel);
	int BatteryNormalProcess(int CurBatteryLevel);

	int BatteryChargeProcess(void);
	int BatteryNormalProcess(void);

	int GetBatteryCap(void);
	int _GetBatteryCap(void);
	int EnableBatteryCharge(void);
	int DisableBatteryCharge(void);

protected:
	int m_BatteryChargeMode;
	bool m_BatteryChargeState;///< ���״̬:true ���;false����
	S_TIME m_Charge100Time;///< ��س䵽100��ʱ�䣬�Ӵ�ʱ���ٳ�AfterCap100ChargeHoursʱ�䣬����
	int m_CurBatteryCap;///< ��ǰ��ص���
	int m_PreBatteryCap; ///< ǰһ�εĵ���
	S_TIME m_timeChargeBegin;///< ��ʼ���ʱ��
	S_TIME m_timeChargeAfterReset;///< �ն��ϵ���ʱ��

	static const int AfterCap100ChargeHours = 1*60*60;///< �䵽100%���ٳ�೤ʱ��
};

#endif//__BASEB_BATTERY_BASE_H__

