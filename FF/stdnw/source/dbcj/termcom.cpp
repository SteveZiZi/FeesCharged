/** @file
 *  @brief 终端寄存器采集类
 *  @author 
 *  @date 2013/03/09
 *  @version 0.1
 *  @note 终端寄存器采集类定义和实现
 */
#include "termcom.h"
#include "htrace.h"
#include "termreg.h"
#include "mrule.h"

//
// 终端寄存器采集
//
C_TermCom::C_TermCom(void)
{
}

C_TermCom::~C_TermCom()
{
}

/** @fn
 *  @brief 初始化所有配置的表计规约
 *  @return 0:成功; -1:失败
 */
int C_TermCom::InitMeterRules(void)
{
	return 0;
}

/** @brief 清除缓冲
 *  @param[in] MeterRuleID 表计规约ID
 *  @return true 成功；false 失败
 *  @note 仅在Read操作返回数据异常时需要此操作
 */
bool C_TermCom::ClearBuffer(int MeterRuleID)
{
	return true;
}

/** @brief 读表计数据
 *  @param[in] Meter 表计信息
 *  @param[in] CommReg 通用寄存器
 *  @param[out] strData 数据输出
 *  @return <0:出错；>=0:成功
 */
int C_TermCom::Read(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData)
{
	int ok = _Read(Meter, CommReg, strData, false);
	if (ok == MRULE_PARSE_ERR_CTRL)
	{
		logwt<<"采集备用寄存器["<<HEX<<CommReg.m_ID<<"]..."<<endl;
		ok = _Read(Meter, CommReg, strData, true);
	}
	else if (ok == MRULE_PARSE_ERR_UNSUPPORT)
	{
		logwt<<"填充寄存器["<<HEX<<CommReg.m_ID<<"]..."<<endl;
		ok = CommReg.FillData(strData) > 0? 0:-1;
	}
	return ok==0? 0:-1;
}

/** @brief 写表计参数
 *  @param[in] Meter 表计信息
 *  @param[in] CommReg 通用寄存器
 *  @param[in] strParam 输入参数
 *  @param[out] strData 数据输出
 *  @return <0:出错；>=0:成功
 */
int C_TermCom::Write(const S_MeterBase &Meter, const S_REGISTER &CommReg, const string &strParam, string &strData)
{
	return 0;
}

/** @brief 上报表计数据
 *  @param[in] CommReg 通用寄存器
 *  @param[out] strMeterAddress 表计地址输出[6BYTES]
 *  @param[out] strData 数据输出
 *  @param[in] timeKeeping 上报时间;0:退出上报;0xFFFFFFFF:查询;其它:进入上报
 *  @return <0:出错 0:结束上报 >0:上报剩余时间
 */
int C_TermCom::Report(const S_REGISTER &CommReg, string &strMeterAddress, string &strData, S_TIME timeKeeping)
{
	return 0;
}

/** @brief 广播表计时间
 *  @param[in] Meter 表计信息
 *  @param[in] timeExpected 期望时间
 *  @return <0:出错；>=0:成功
 */
int C_TermCom::Broadcast(const S_MeterBase &Meter, S_TIME timeExpected)
{
	return 0;
}

/** @brief 读表计数据
 *  @param[in] Meter 表计信息
 *  @param[in] CommReg 通用寄存器
 *  @param[out] strData 数据输出
 *  @param[in] fRsvReg true备用寄存器;false主寄存器
 *  @return <0:[MRULE_PARSE_ERR_]出错；>=0:成功
 */
int C_TermCom::_Read(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData, bool fRsvReg)
{
	C_TermReg *pTermReg = C_TermRegFact::GetTermRegister(CommReg.m_ID);
	if (pTermReg == NULL)
		return MRULE_PARSE_ERR_UNSUPPORT;
	if (pTermReg->GetData(Meter, CommReg, strData) != 0)
		return MRULE_PARSE_ERR_UNSUPPORT;
	return MRULE_PARSE_ERR_NO;
}


