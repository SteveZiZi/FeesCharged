/** @file
 *  @brief ϵͳ���ƶ���
 *  @author 
 *  @date 2011/11/07
 *  @version 0.1
 *  1��ϵͳ��������������
 *  2��ϵͳ�ű����ж���
 *  3��ϵͳ��ʼ������
 */
#ifndef __MP_CFG_SYSCTRL_H__
#define __MP_CFG_SYSCTRL_H__

/** @class
 *  @brief ϵͳ������
 */
class C_SYSCTRL
{
public:
	/** @brief �ű�����
	 *  @param[in] fScriptFile �ű��ļ�������·����
	 *  @return 0 �ɹ�
	 *  @return !0 ʧ��
	 */
	static int ExecScript(const char *fScriptFile);
	/** @brief Ӳ�����������Ź�������
	 *  @remarks ���ñ�������ϵͳ�������������
	 */
	static int HardwareReset(void);
	/** @brief Ӧ�ó������� 
	 *  @remarks ���ñ�������Ӧ�ó����������������
	 */
	static int SoftwareReset(bool fImmediately = false);
	/** @brief ϵͳ���� 
	 *  @remarks ���ñ�������ϵͳ��������
	 */
	static int SystemReboot(void);
	/** @brief ��������ʼ�� */
	static int DataReset(void);
	/** @brief ��������� */
	static int DataClear(void);
	/** @brief ������ʼ�� */
	static int FactoryReset(void);
	/** @brief ������ʼ�� */
	static int ParamReset(void);
	/** @brief ����������������ʼ�� */
	static int AllReset(void);
	/** @brief ϵͳ״̬��ʼ�� */
	static int StatusReset(void);
	/** @brief ��������û����� */
	static int UserConfigClear(void);
	/** @brief ����ʹ�ܿ��� */
	static int EnableProcess(const char *pProcessName, bool fEnable);
	/** @brief ȡ����ʹ��״̬ */
	static bool GetProcessEnable(const char *pProcessName);
	/** @brief �������� */
	static bool ProcessHeartBeat(void);
};

#endif//__MP_CFG_SYSCTRL_H__

