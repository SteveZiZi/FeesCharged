/** @file
 *  @brief 国网安全策略定义
 *  @author 
 *  @date 2012/08/16
 *  @version 0.1
 *  @note 策略模式
 */
#ifndef __GWB_SECURITY_H__
#define __GWB_SECURITY_H__
#include "basedef.h"
#include "gwmsrule.h"
#include "connect.h"
#include "esambase.h"

#define ESAM_OBJ_MAX	256

/** @brief 安全方案定义 */
struct S_SecurityKey
{
	S_WORD m_SecurityCode; ///< 安全码
	S_WORD m_SecurityParam; ///< 安全参数
};

/** @class
 *  @brief安全模式上下文定义
 *  @note
 *  应用层初始化时调用顺序：
 *  1、设置用户安全模块对象，如有，SetSecurityObject()
 *  2、设置安全策略关关键字，SetSecurityKey()
 *  3、初始化安全对象，InitSecurityObject()
 */
class C_SecurityContext
{
public:
	static C_SecurityContext* GetInstance(void);

public:
	/** @fn
	 *  @brief 设置用户安全对象
	 *  @param[in] idx 安全索引
	 *  @param[in] pEsamObj 安全对象指针[非局部变量]
	 *  @return false失败；true成功
	 */
	bool SetSecurityObject(S_WORD idx, C_EsamBase *pEsamObj);
	/** @fn
	 *  @brief 设置安全策略关键字
	 *  @param[in] SecurityKey 安全策略关键字
	 *  @return false失败；true成功
	 */
	bool SetSecurityKey(const S_SecurityKey &SecurityKey);
	/** @fn
	 *  @brief 初始化安全对象信息
	 *  @param[in] pMsRule 主站规约
	 *  @param[in] pConnect 安全模块通道
	 *  @return false失败；true成功
	 */
	bool InitSecurityObject(C_MSRULE *pMsRule, C_CONNECT *pConnect);

	/** @fn
	 *  @brief 报文验证
	 *  @param[in] strCmd 报文
	 *  @param[in] strPWD 密码
	 *  @return true:成功； false:失败
	 */
	bool Authenticate(const string &strCmd, const string &strPWD);
	/** @fn
	 *  @brief 提取报文验证错误信息
	 *  @param[out] strErrorInfo 报文验证错误信息
	 *  @return <0:失败；>=0:成功
	 */
	int GetErrorInfo(string &strErrorInfo);
	/** @fn
	 *  @brief 提取报文密码信息
	 *  @param[out] strPassword 报文密码信息
	 *  @return <0:失败；>=0:成功
	 */
	int GetPasswordInfo(string &strPassword);

private:
	C_SecurityContext(void);

private:
//	string m_strCmd; ///< 报文
	string m_strPWD; ///< 报文密码
	C_EsamBase *m_pEsamObj[ESAM_OBJ_MAX]; ///< 安全对象
	S_SecurityKey m_SecurityKey; ///< 安全关键字
	int m_ErrorCode; ///< 错误码
};

#endif //__GWB_SECURITY_H__

