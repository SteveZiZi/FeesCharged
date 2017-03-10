/** @file
 *  @brief APP�������
 *  @author 
 *  @date 2011/10/15
 *  @version 0.1
 *  1������APP���������չ
 *  2�����������Ĳ����������������������չ
 */
#ifndef __MP_APP_CMD_H__
#define __MP_APP_CMD_H__
#include "server.h"

/** @brief ���֧������������� */
#define MAIN_ARGV_MAX	3
/** @brief ����֮��ķָ� */
#define MAIN_PARAM_DELIMIT		'.'
/** @brief ��ֵ�� */
#define MAIN_PARAM_ASSIGN		'='

/** @brief Ӧ�ó�������� */
#define APP_CMD_EXEC		"-exec"
#define APP_CMD_START		"-start"
#define APP_CMD_STOP		"-stop"
#define APP_CMD_SET			"-set"
#define APP_CMD_ACTION		"-action"
#define APP_CMD_LIST		"-list"
#define APP_CMD_HELP		"-help"

/** @brief ϵͳ�������� */
#define SYSTEM_SUSPEND_DELAY_MAX		4
#define WATCH_DOG_RESET_DELAY_MAX	16
#define HAREWARE_RESET_DELAY_MAX	(WATCH_DOG_RESET_DELAY_MAX+SYSTEM_SUSPEND_DELAY_MAX)
#define SOFTWARE_RESET_DELAY_MAX		8

/** @class S_HelpInfo appcmd.h "mprocess/appcmd.h"
 *  @brief ������Ϣ�ṹ����
 *  ��������Ͳ����İ�����Ϣ���ô˽ṹ���壬��ʾͳһ
 */
struct S_HelpInfo
{
	const char *m_pItem;///< ����������
	const char *m_pInfo;///< ��������˵��
};

/** @class C_AppCmd appcmd.h "mprocess/appcmd.h"
 *  @brief APP�������
 *  @remarks �û���Ҫʵ�����������������Ա������ý���
 *  int MP_GetProcessSN(const char *pName);
 *  const char* MP_GetProcessName(int PSN);
 */
class C_AppCmd
{
public:
	C_AppCmd(int argc, char *argv[]);
	virtual ~C_AppCmd();

public:
	/** @brief ִ������
	 *  @return 0 �ɹ���-1 ʧ��
	 */
	int ExecuteCmd(void);

protected:
	/** @brief �������ע�� */
	int ZoneComponentRegister(void);
	/** @brief exec���� */
	int Exec(void);
	/** @brief start���� */
	int Start(void);
	/** @brief Stop���� */
	int Stop(void);
	/** @brief Set���� */
	int Set(void);
	/** @brief Action���� */
	int Action(void);
	/** @brief List���� */
	int List(void);
	/** @brief Help���� */
	int Help(void);

private:
	/** @brief ��ʾLOGO */
	int ShowLogo(void);
	/** @brief �б�������� */
	int ListCmdHelp(void);
	/** @brief ִ��������� */
	int ExeCmdHelp(void);
	/** @brief ����������� */
	int SetCmdHelp(void);
	/** @brief ����������� */
	int ActionCmdHelp(void);
	/** @brief �ӽ�����Ϣ��� */
	int SubProcessCmdHelp(char *pCmd);
	/** @brief LOGO��ʾ�ж�
	 *  @return true ��Ҫ��ʾLOGO��false ����Ҫ
	 */
	bool NeedShowLogo(void);
	/** @brief ���̻�������
	 *  @return true �ɹ���false ʧ��
	 */
	bool MutuxCheck(const char *pMutuxName);
	/** @brief ���л��ӽ����� */
	int SerializeSubProcessName(string &strPName);
	/** @brief ����������*/
	bool CheckProcessConfig(void);

	/** ������Ϣ�Ƚ� */
	int strProcessCmp(const char *pCmdInfo, const char *pDataInfo);
	/** �����Ϣ�ؼ��ֱȽ� */
	int strMeterInfoCmp(const char *pCmdInfo, const char *pDataInfo);

protected:
	/** @brief ��ȡ���񹤳�����
	 *  @param[in] pServerName ������
	 *  @return NULL ��Ч����!NULL ��Ч����
	 *  @remarks Ӧ�ó��������չ��ע��Ҫ�ص�������
	 */
	virtual C_SERVER *GetServer(const char *pServerName);
	/** @brief ����ǰ����
	 *  @return 0 �ɹ���-1 ʧ��
	 */
	virtual int BeforeCmd(void);
	/** @brief ��������
	 *  @return 0 �ɹ���-1 ʧ��
	 */
	virtual int AfterCmd(void);

	/** @brief ִ���б�����
	 *  @param[in] pCmdInfo ���������Ϣ
	 *  @return 0 �ɹ���-1 ʧ��
	 *  @remarks Ӧ�ó��������չ��ע��Ҫ�ص�������
	 */
	virtual int ListCmdExec(const char *pCmdInfo);
	/** @brief ��ʾ�б��������
	 *  @param[in] InfoSN ������ʾ���
	 *  @return 0 �ɹ���-1 ʧ��
	 *  @remarks Ӧ�ó��������չ��ע��Ҫ�ص�������
	 */
	virtual int ShowListHelp(int InfoSN);
	/** @brief ��ʾ�汾��Ϣ
	 *  @return 0 �ɹ���-1 ʧ��
	 *  @remarks Ӧ�ó��������չ��ע��Ҫ�ص�������
	 */
	virtual int ListVersion(void);
	/** @brief ��ʾϵͳ����ʱ����Ϣ
	 *  @return 0 �ɹ���-1 ʧ��
	 */
	int ListRuntime(void);
	/** @brief ��ʾ������Ϣ
	 *  @return 0 �ɹ���-1 ʧ��
	 */
	int ListProcessInfo(void);
	/** @brief ��ʾ���̸���������Ϣ
	 *  @return 0 �ɹ���-1 ʧ��
	 */
	int ListProcessTrace(void);
	/** @brief ��ʾ�ɼ�����������Ϣ
	 *  @return 0 �ɹ���-1 ʧ��
	 */
	int ListDbcjTask(const char *pCmdInfo);
	/** @brief ��ʾ�����������Ϣ
	 *  @return 0 �ɹ���-1 ʧ��
	 */
	int ListMeterCount(const char *pCmdInfo);
	/** @brief ��ʾ�����ϸ��Ϣ
	 *  @return 0 �ɹ���-1 ʧ��
	 */
	int ListMeterInfo(const char *pCmdInfo);
	/** @brief ��ʾ��ǰ������Ϣ
	 *  @return 0 �ɹ���-1 ʧ��
	 */
	int ListSystemCurrentZone(const char *pCmdInfo);

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

private:
	int m_argc;///< �����������
	char *m_argv[MAIN_ARGV_MAX];///< ���������1��Ӧ�ó�������2�����3������
};

#endif//__MPROCESS_APP_CMD_H__


