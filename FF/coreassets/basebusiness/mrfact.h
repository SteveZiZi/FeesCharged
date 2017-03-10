/**
 *  @file
 *  @brief 表计规约工厂类
 *  @author 
 *  @date 2012-05-10
 *  @version v1.0
 *  @note
 */
#ifndef __BASEB_MR_FACTORY_H__
#define __BASEB_MR_FACTORY_H__
#include "mrule.h"

/** @brief 表计规约名称映射 */
struct S_RuleNameMap
{
	int m_RuleID;
	char m_RuleName[32];

public:
	S_RuleNameMap(void){memset(this, 0x00, sizeof(*this));}
};
struct S_RuleNameMaps:public vector<S_RuleNameMap>
{
};

class C_MRuleFact
{
public:
	/** @fn
	 *  @brief 初始化所有配置的表计规约
	 *  @return 0:成功; -1:失败
	 *  @see /mnt/dyjc/registor/mrule.ini
	 *  @note 在mrule.ini中配置表计规约
	 */
	static int InitMeterRules(const char *pResourcePath);
	/** @fn
	 *  @brief 取表规约
	 *  @param[in] pRuleName 规约名称
	 *  @return NULL:没有相应的表计规约; !NULL:合法表计规约
	 */
	static C_MRULE *GetMeterRule(const char *pRuleName);
	/** @fn
	 *  @brief 取表规约
	 *  @param[in] RuleID 上行要求规约ID
	 *  @return NULL:没有相应的表计规约; !NULL:合法表计规约
	 */
	static C_MRULE *GetMeterRule(int RuleID);
	/** @fn
	 *  @brief 取表规约ID
	 *  @param[in] pRuleName 规约名称
	 *  @return 表计规约ID; -1:非法表计规约ID
	 */
	static int GetMeterRuleID(const char *pRuleName);
	/** @fn
	 *  @brief 取表规约名称
	 *  @param[in] RuleID 规约ID
	 *  @return 表计规约名称; unknow:非法表计规约名称
	 */
	static const char *GetMeterRuleName(int RuleID);
};

class C_CommReg
{
public:
	static int LoadCommonRegister(S_REGISTERS &Registers, const char *pResourcePath);
};

class C_MeterRuleMap
{
public:
	static int LoadMeterRuleMap(S_RuleNameMaps &NameMaps, const char *pResourcePath);

protected:
	static S_RuleNameMaps m_NameMaps;
};

#endif //__BASEB_MR_FACTORY_H__

