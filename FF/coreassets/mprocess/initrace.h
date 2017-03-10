/** @file
 *  @brief ���̸�����Ϣ����
 *  @author 
 *  @date 2011/10/15
 *  @version 0.1
 *  1�����̸�����Ϣ�����ļ�
 *  2��������Ϣ�����ı��ļ�����
 *  @see foundation/confuse inifileb.h
 */
#ifndef __MP_INI_TRACE_H__
#define __MP_INI_TRACE_H__
#include "inifileb.h"

/** @class
 *  @brief ���̸�����Ϣ������
 */
class C_IniTrace
{
public:
	/** @fn C_IniTrace::LoadProcessTraceConfig
	 *  @brief ���ظ�����ĸ�����Ϣ����
	 *  @param[in] fname:�ļ���	
	 *  @return 0:successful, -1:failed
	 */
	static int LoadProcessTraceConfig(const char *fname);
	/** @fn C_IniTrace::SaveProcessTraceConfig
	 *  @brief ���������ĸ�����Ϣ����
	 *  @param[in] fname:�ļ���
	 *  @return 0:successful, -1:failed
	 */
	static int SaveProcessTraceConfig(const char *fname);
	/** @fn C_IniTrace::AppCmdSetTrace
	 *  @brief Ӧ���������ý��̸���������Ϣ
	 *  @param[in] pProcess:������
	 *  @param[in] pItem:������
	 *  @param[in] pVal:ֵ
	 *  @return 0:successful, -1:failed
	 */
	static int AppCmdSetTrace(const char *pProcess, const char *pItem, const char *pVal);
	/** @fn C_IniTrace::AppCmdListTrace
	 *  @brief Ӧ�������б���̸���������Ϣ
	 *  @param[in] pProcess:������
	 *  @param[out] strTraceInfo:����ı���Ϣ����
	 *  @return 0:successful, -1:failed
	 */
	static int AppCmdListTrace(const char *pProcess, string &strTraceInfo);

private:
	/** @brief ����->���� */
	static S_BYTE MakeLevelVal(const char* pkey);
	/** @brief ����->Ŀ�� */
	static S_BYTE MakeWayVal(const char* pkey);
	/** @brief ����->���� */
	static const char* MakeLevelKey(S_BYTE val);
	/** @brief Ŀ��->���� */
	static const char* MakeWayKey(S_BYTE val);
};

#endif //__MP_INI_TRACE_H__
