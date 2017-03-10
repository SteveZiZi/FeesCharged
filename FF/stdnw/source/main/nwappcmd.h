/** @file
 *  @brief ���������
 *  @author 
 *  @date 2011/11/30
 *  @version 0.1
 *  @see appcmd.h
 *  @note
 *  1����������̳��Զ���̿��
 *  2�����ļ�����������ɫ���������
 */
#ifndef __NW_APP_CMD_H__
#define __NW_APP_CMD_H__
#include "appcmd.h"


/** @class 
 *  @brief �����������
 *  @remarks 
 */
class C_NwAppCmd:public C_AppCmd
{
public:
	C_NwAppCmd(int argc, char *argv[]);
	virtual ~C_NwAppCmd();
	static bool SetServer(const char *pServerName, C_SERVER *pServer);

protected:
	virtual C_SERVER *GetServer(const char *pServerName);
	virtual int ListVersion(void);

	/** @brief ִ����������
	 *  @param[in] pCmdInfo ���������Ϣ
	 *  @return 0 �ɹ���-1 ʧ��
	 *  @remarks Ӧ�ó��������չ��ע��Ҫ�ص�������
	 */
	virtual int SetCmdExec(const char *pCmdInfo);
	/** @brief ��ʾ�����������
	 *  @param[in] InfoSN ������ʾ���
	 *  @return 0 �ɹ���-1 ʧ��
	 *  @remarks Ӧ�ó��������չ��ע��Ҫ�ص�������
	 */
	virtual int ShowSetHelp(int InfoSN);

	/** @brief ִ�ж�������
	 *  @param[in] pCmdInfo ���������Ϣ
	 *  @return 0 �ɹ���-1 ʧ��
	 *  @remarks Ӧ�ó��������չ��ע��Ҫ�ص�������
	 */
	virtual int ActionCmdExec(const char *pCmdInfo);
	/** @brief ��ʾ�����������
	 *  @param[in] InfoSN ������ʾ���
	 *  @return 0 �ɹ���-1 ʧ��
	 *  @remarks Ӧ�ó��������չ��ע��Ҫ�ص�������
	 */
	virtual int ShowActionHelp(int InfoSN);
	/** @brief ��ʾ�ɼ�������Ϣ
	 *  @param[in] Process �ɼ��������[1-*]
	 *  @param[out] strTaskInfo ������Ϣ
	 *  @return 0 �ɹ���-1 ʧ��
	 */
	virtual int ListDbcjTaskInfo(int Process, string &strTaskInfo);

	/** @brief  �ɼ����������Ϣ
	 *  @param[out] Cnt �������
	 *  @param[out] MinMP ��С������
	 *  @param[out] MaxMP ��������
	 *  @return 0 �ɹ���-1 ʧ��
	 */
	virtual int GetMeterCount(S_WORD &Cnt, S_WORD &MinMP, S_WORD &MaxMP);

	/** @brief ��ӡ�����ϸ��Ϣ
	 *  @param[in] MinMP ��С������
	 *  @param[in] MaxMP ��������
	 *  @param[out] strInfo ��Ϣ
	 *  @return 0 �ɹ���-1 ʧ��
	 */
	virtual int PrintMeterInfo(S_WORD MinMP, S_WORD MaxMP, string &strInfo);

	/** @brief �����ǰ����������Ϣ
	 *  @param[out] strInfo ��Ϣ
	 *  @return 0 �ɹ���-1 ʧ��
	 */
	virtual int GetCurrentZone(string &strInfo);

protected:
	int ZoneSwitch(const string &strCmdInfo, string &strInfo);
};

#endif//__NW_APP_CMD_H__


