/** @file
 *  @brief ����ESAM��ȫģ��ӿ�
 *  @author 
 *  @date 2012/08/16
 *  @version 0.1
 */
#ifndef __GWB_ESAM_BASE_H__
#define __GWB_ESAM_BASE_H__
#include "gwmsrule.h"
#include "connect.h"

enum
{
	ESAM_ERROR_CODE_OTHER = -1,
	ESAM_ERROR_CODE_OK = 0x00,
	ESAM_ERROR_CODE_SIGN, //ǩ����
	ESAM_ERROR_CODE_CRY, //���Ĵ�
	ESAM_ERROR_CODE_MAC, //MAC��֤��

	ESAM_ERROR_CODE_MAX
};


/** @class
 *  @brief ��ȫģ������ӿ�
 *  @note
 */
class C_EsamBase
{
public:
	C_EsamBase(void){}
	virtual ~C_EsamBase(){}

public:
	/** @fn
	 *  @brief ��ʼ��ESAM��Ϣ
	 *  @param[in] pMsRule ��վ��Լ
	 *  @param[in] pConnect ��ȫģ��ͨ��
	 *  @return falseʧ�ܣ�true�ɹ�
	 */
	virtual bool InitEsamInfo(C_MSRULE *pMsRule, C_CONNECT *pConnect) = 0;
	/** @fn
	 *  @brief ������֤
	 *  @param[in] strCmd ����
	 *  @param[in] strPWD ����
	 *  @return ESAM_ERROR_CODE_
	 */
	virtual int Authenticate(const string &strCmd, const string &strPWD) = 0;
	/** @fn
	 *  @brief ��ȡ������֤������Ϣ
	 *  @param[out] strErrorInfo ������֤������Ϣ
	 *  @return <0:ʧ�ܣ�>=0:�ɹ�
	 */
	virtual int GetErrorInfo(string &strErrorInfo) = 0;
};

#endif //__GWB_ESAM_BASE_H__


