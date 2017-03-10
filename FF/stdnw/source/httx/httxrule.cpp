/** @file
 *  @brief ��̨ͨѶ��Լ����
 *  @author 
 *  @date 2011/12/27
 *  @version 0.1
 */
#include "httxrule.h"
#include "cmdfact.h"
#include "status.h"

/** @brief ȡAFN����
 *  @remarks Ӧ�ó����������
 */
C_AFNCMD *C_HttxRule::GetAfnCmd(S_BYTE AFN)
{
	C_AFNCMD *pCmd = C_CmdFact::GetAfnCmd(AFN);
	if (pCmd != NULL)
		return pCmd;
	return C_MSRULE::GetAfnCmd(AFN);
}

/** @ȡ�澯��־
 *  @return 1 �и澯δ�ϱ�;0 �޸澯δ�ϱ�
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


