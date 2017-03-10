/** @file
 *  @brief ����̿���û������ļ�
 *  @author 
 *  @date 2011/10/15
 *  @version 0.1
 *  1��������־ϵͳ
 *  2�����ö���̵Ľ�����
 *  3������IPC
 */
#include "cfg_mp.h"
#include "cfg_usrp.h"
#include "cfg_proc.h"
#include "context.h"
#include "status.h"

//
//���ö����
//
/** @brief ��������
 *  @note ���ӽ��̵ķ���
 *  1����cfg_proc.h�ж���������꣬ͬʱ�޸Ľ���������MP_PROCESS_CNT
 *  2��������������뵽gMP_ProcessAll�����У�����̿�ܽ��Զ�����ָ���Ľ��̡�
 *  3���̳�C_SERVER�࣬��д����Ҫʵ�ֵľ��幦�ܡ��μ�server.h��
 *  4���̳�C_AppCmd�࣬��C_SERVER������GetServer()������ʵ�������μ�appcmd.h��
 *  @remarks �������겻���ڱ��ļ��ж��塣
 *  @see server.h appcmd.h cfg_proc.h
 */
S_PROCESS gMP_ProcessAll[] =
{
	//�����������̹���˵��
	{MP_PROCESS_DAEMON, "ϵͳ�ػ����̣�����ϵͳ�ܿ�"},
//�û����̼��ں���
	{MP_PROCESS_LCD,   "lcd���̣����𱾵ز�������ʾ"},
	{MP_PROCESS_HTTX1, "httx1���̣�������վͨѶ(�ͻ���)"},
	{MP_PROCESS_HTTX2, "httx2���̣��������ͨѶ(������)"},
	{MP_PROCESS_HTTX3, "httx3���̣������ͨѶ"},
	{MP_PROCESS_HTTX4, "httx4���̣�����ͨѶ"},
	{MP_PROCESS_DBCJ1,  "dbcj1���̣����������ɼ������ݶ���"},
	{MP_PROCESS_DBCJ2,  "dbcj2���̣����������ɼ������ݶ���"},
	{MP_PROCESS_DBCJ3,  "dbcj3���̣����������ɼ������ݶ���"},
	{MP_PROCESS_DBCJ4,  "dbcj4���̣����������ɼ������ݶ���"},
	{MP_PROCESS_PPP,   "ppp���̣���������վ������������"},
	{MP_PROCESS_JLCY, "jlcy���̣�����Խ���оƬ���ݲɼ�"},
	{MP_PROCESS_FHKZ, "fhkz���̣�����澯����͸������ݴ���"},
	{MP_PROCESS_CTCY, "ctcy���̣�����CT�����͵�ع���"},
	{MP_PROCESS_RFTX,   "RFTX���̣������ն�����Ƶģ��ͨ��"},//wjp 20160729

//���̶������
	{NULL, NULL}
};

/** @brief ��ȡ�������
 *  @param[in] pName ��������
 *  @return 0~(MP_PROCESS_COUNT-1) ���ҳɹ�
 *  @return -1 ����ʧ��
 *  @remarks ����������Ҫ��д
 */
int MP_GetProcessSN(const char *pName)
{
	for (int i = 0; gMP_ProcessAll[i].m_pName != NULL; i++)
	{
		if (strcmp(gMP_ProcessAll[i].m_pName, pName) == 0)
			return i;
	}
	return -1;
}

/** @brief ��ȡ�������� 
 *	@param[in] PSN ���̵���Ч���(0~(MP_PROCESS_COUNT-1))
 *	@return NULL PSN�����Ч
 *  @return !NULL ������ָ��
 *  @remarks ����������Ҫ��д,�û����豣֤PSN�������Ч��
 */
const char* MP_GetProcessName(int PSN)
{
	return gMP_ProcessAll[PSN].m_pName;
}
/** @brief ��ȡ���̰�����Ϣ
 *	@param[in] PSN ���̵���Ч���(0~(MP_PROCESS_COUNT-1))
 *	@return NULL PSN�����Ч
 *  @return !NULL ������Ϣָ��
 *  @remarks ����������Ҫ��д,�û����豣֤PSN�������Ч��
 */
const char* MP_GetProcessInfo(int PSN)
{
	return gMP_ProcessAll[PSN].m_pInfo;
}

/** @brief ��ȡ�������� */
S_WORD MP_GetProcessCnt(void)
{
	return MP_PROCESS_CNT;
}

//
// ����IPC 
//
/** @breif ��ȡϵͳ�����Ĺ����ڴ�
 *  @remarks �����������ʵ��ָ��Ķ������ã��Ա����̿�ܵ���
 */
C_ContextBase *C_ShmFact::GetContextBase(void)
{
//	static C_ContextBase MyContextBase;
//	return &MyContextBase;
	C_CONTEXT &MyContext = C_CONTEXT::Instance();
	return &MyContext;
}

/** @breif ��ȡϵͳ״̬�����ڴ� 
 *  @remarks �����������ʵ��ָ��Ķ������ã��Ա����̿�ܵ���
 */
C_StatusBase  *C_ShmFact::GetStatusBase(void)
{
//	static C_StatusBase MyStatusBase;
//	return &MyStatusBase;
	C_STATUS &MyStatus = C_STATUS::Instance();
	return &MyStatus;
}


//
//����̿����غ���
//
#include "version.h"
/** @fn
 *  @remarks �����ʵ�����ã��Ա����̿�ܵ���
 *  @note ������汾
 */
char *MP_GetSoftwareVersion(void)
{
	return CONS_TERM_NAME CONS_MAJOR_VERSION"."CONS_MINOR_VERSION CONS_FIX_VERSION;
}
/** @fn
 *  @remarks �����ʵ�����ã��Ա����̿�ܵ���
 *  @note �����򷢲�����
 */
char *MP_GetSoftwareReleaseDate(void)
{
	return CONS_SW_REAL_DATE;
}
/** @fn
 *  @remarks �����ʵ�����ã��Ա����̿�ܵ���
 */
char *MP_GetHardwareVersion(void)
{
	return CONS_HARD_VERSION;
}
/** @fn
 *  @remarks �����ʵ�����ã��Ա����̿�ܵ���
 */
char *MP_GetHardwareReleaseDate(void)
{
	return CONS_HW_REAL_DATE;
}

