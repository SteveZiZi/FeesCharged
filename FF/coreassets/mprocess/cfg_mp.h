/** @file
 *  @brief ����̿���û������ļ�
 *  @author 
 *  @date 2011/10/15
 *  @version 0.1
 *  1��������־ϵͳ
 *  2�����ö���̵Ľ�����
 *  3������IPC
 */
#ifndef __MP_CFG_MP_H__
#define __MP_CFG_MP_H__
#include "sysipc.h"

//
//���ö����
//
/** @berif �����ػ����̵Ľ����� */
#define MP_PROCESS_DAEMON	"daemon"

struct S_PROCESS
{
	const char *m_pName;///<������
	const char *m_pInfo;///<���̰�����Ϣ
};

/** @brief ��ȡ������� */
int MP_GetProcessSN(const char *pName);
/** @brief ��ȡ�������� */
const char* MP_GetProcessName(int PSN);
/** @brief ��ȡ���̰�����Ϣ */
const char* MP_GetProcessInfo(int PSN);
/** @brief ��ȡ�������� */
S_WORD MP_GetProcessCnt(void);

/** @berif ����IPC */
class C_ShmFact
{
public:
	/** @breif ��ȡϵͳ�����Ĺ����ڴ� */
	static C_ContextBase *GetContextBase(void);
	/** @breif ��ȡϵͳ״̬�����ڴ� */
	static C_StatusBase  *GetStatusBase(void);
};

/** @breif ����̿����غ��� */
char *MP_GetSoftwareVersion(void);
char *MP_GetSoftwareReleaseDate(void);
char *MP_GetHardwareVersion(void);
char *MP_GetHardwareReleaseDate(void);


#endif //__MPROCESS_CFG_MP_H__

