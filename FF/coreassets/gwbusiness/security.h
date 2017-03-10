/** @file
 *  @brief ������ȫ���Զ���
 *  @author 
 *  @date 2012/08/16
 *  @version 0.1
 *  @note ����ģʽ
 */
#ifndef __GWB_SECURITY_H__
#define __GWB_SECURITY_H__
#include "basedef.h"
#include "gwmsrule.h"
#include "connect.h"
#include "esambase.h"

#define ESAM_OBJ_MAX	256

/** @brief ��ȫ�������� */
struct S_SecurityKey
{
	S_WORD m_SecurityCode; ///< ��ȫ��
	S_WORD m_SecurityParam; ///< ��ȫ����
};

/** @class
 *  @brief��ȫģʽ�����Ķ���
 *  @note
 *  Ӧ�ò��ʼ��ʱ����˳��
 *  1�������û���ȫģ��������У�SetSecurityObject()
 *  2�����ð�ȫ���Թعؼ��֣�SetSecurityKey()
 *  3����ʼ����ȫ����InitSecurityObject()
 */
class C_SecurityContext
{
public:
	static C_SecurityContext* GetInstance(void);

public:
	/** @fn
	 *  @brief �����û���ȫ����
	 *  @param[in] idx ��ȫ����
	 *  @param[in] pEsamObj ��ȫ����ָ��[�Ǿֲ�����]
	 *  @return falseʧ�ܣ�true�ɹ�
	 */
	bool SetSecurityObject(S_WORD idx, C_EsamBase *pEsamObj);
	/** @fn
	 *  @brief ���ð�ȫ���Թؼ���
	 *  @param[in] SecurityKey ��ȫ���Թؼ���
	 *  @return falseʧ�ܣ�true�ɹ�
	 */
	bool SetSecurityKey(const S_SecurityKey &SecurityKey);
	/** @fn
	 *  @brief ��ʼ����ȫ������Ϣ
	 *  @param[in] pMsRule ��վ��Լ
	 *  @param[in] pConnect ��ȫģ��ͨ��
	 *  @return falseʧ�ܣ�true�ɹ�
	 */
	bool InitSecurityObject(C_MSRULE *pMsRule, C_CONNECT *pConnect);

	/** @fn
	 *  @brief ������֤
	 *  @param[in] strCmd ����
	 *  @param[in] strPWD ����
	 *  @return true:�ɹ��� false:ʧ��
	 */
	bool Authenticate(const string &strCmd, const string &strPWD);
	/** @fn
	 *  @brief ��ȡ������֤������Ϣ
	 *  @param[out] strErrorInfo ������֤������Ϣ
	 *  @return <0:ʧ�ܣ�>=0:�ɹ�
	 */
	int GetErrorInfo(string &strErrorInfo);
	/** @fn
	 *  @brief ��ȡ����������Ϣ
	 *  @param[out] strPassword ����������Ϣ
	 *  @return <0:ʧ�ܣ�>=0:�ɹ�
	 */
	int GetPasswordInfo(string &strPassword);

private:
	C_SecurityContext(void);

private:
//	string m_strCmd; ///< ����
	string m_strPWD; ///< ��������
	C_EsamBase *m_pEsamObj[ESAM_OBJ_MAX]; ///< ��ȫ����
	S_SecurityKey m_SecurityKey; ///< ��ȫ�ؼ���
	int m_ErrorCode; ///< ������
};

#endif //__GWB_SECURITY_H__

