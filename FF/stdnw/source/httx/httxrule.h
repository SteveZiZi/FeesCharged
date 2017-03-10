/** @file
 *  @brief ��̨ͨѶ��Լ����
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
	/** @brief ȡAFN����
	 *  @remarks Ӧ�ó����������
	 */
	virtual C_AFNCMD *GetAfnCmd(S_BYTE AFN);
	/** @ȡ�澯��־
	 *  @return 1 �и澯δ�ϱ�;0 �޸澯δ�ϱ�
	 */
	virtual INT8U GetAlarmReportFlag(void);
};

class C_RuleFact
{
public:
	static C_MSRULE *GetMsRule(int RuleID);
};

#endif//__HTTX_RULE_H__

