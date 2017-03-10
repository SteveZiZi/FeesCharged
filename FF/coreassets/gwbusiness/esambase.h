/** @file
 *  @brief 国网ESAM安全模块接口
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
	ESAM_ERROR_CODE_SIGN, //签名错
	ESAM_ERROR_CODE_CRY, //密文错
	ESAM_ERROR_CODE_MAC, //MAC验证错

	ESAM_ERROR_CODE_MAX
};


/** @class
 *  @brief 安全模块基本接口
 *  @note
 */
class C_EsamBase
{
public:
	C_EsamBase(void){}
	virtual ~C_EsamBase(){}

public:
	/** @fn
	 *  @brief 初始化ESAM信息
	 *  @param[in] pMsRule 主站规约
	 *  @param[in] pConnect 安全模块通道
	 *  @return false失败；true成功
	 */
	virtual bool InitEsamInfo(C_MSRULE *pMsRule, C_CONNECT *pConnect) = 0;
	/** @fn
	 *  @brief 报文验证
	 *  @param[in] strCmd 报文
	 *  @param[in] strPWD 密码
	 *  @return ESAM_ERROR_CODE_
	 */
	virtual int Authenticate(const string &strCmd, const string &strPWD) = 0;
	/** @fn
	 *  @brief 提取报文验证错误信息
	 *  @param[out] strErrorInfo 报文验证错误信息
	 *  @return <0:失败；>=0:成功
	 */
	virtual int GetErrorInfo(string &strErrorInfo) = 0;
};

#endif //__GWB_ESAM_BASE_H__


