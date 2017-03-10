/** @file
 *  @brief 后台通讯规约定义
 *  @author 
 *  @date 2011/12/27
 *  @version 0.1
 */
#include "httxrule.h"
#include "cmdfact.h"
#include "status.h"

/** @brief 取AFN命令
 *  @remarks 应用程序必须重载
 */
C_AFNCMD *C_HttxRule::GetAfnCmd(S_BYTE AFN)
{
	C_AFNCMD *pCmd = C_CmdFact::GetAfnCmd(AFN);
	if (pCmd != NULL)
		return pCmd;
	return C_MSRULE::GetAfnCmd(AFN);
}

/** @取告警标志
 *  @return 1 有告警未上报;0 无告警未上报
 */
INT8U C_HttxRule::GetAlarmReportFlag(void)
{
	S_AlarmStatus *pAS = STATUS.GetAlarmStatus();
	if (pAS->m_ImportantNewCnt > 0)
		return 1;
	return 0;
}

C_MSRULE *C_RuleFact::GetMsRule(int RuleID)
{
	static C_HttxRule s_DefaultHttxRule;
	static C_HttxRule s_SlaveHttxRule;
	switch (RuleID)
	{
	case MS_RULE_ID_MASTER:return &s_DefaultHttxRule;
	case MS_RULE_ID_SLAVE:return &s_SlaveHttxRule;
	}
	return &s_DefaultHttxRule;
}


