/** @file
 *  @brief 后台通讯规约定义
 *  @author 
 *  @date 2011/12/27
 *  @version 0.1
 */
#ifndef __HTTX_RULE_H__
#define __HTTX_RULE_H__
#include "nwmsrule.h"

#define MS_RULE_ID_MASTER 0
#define MS_RULE_ID_SLAVE	1
class C_HttxRule:public C_MSRULE
{
public:
	/** @brief 取AFN命令
	 *  @remarks 应用程序必须重载
	 */
	virtual C_AFNCMD *GetAfnCmd(S_BYTE AFN);
	/** @取告警标志
	 *  @return 1 有告警未上报;0 无告警未上报
	 */
	virtual INT8U GetAlarmReportFlag(void);
};

class C_RuleFact
{
public:
	static C_MSRULE *GetMsRule(int RuleID);
};

#endif//__HTTX_RULE_H__

