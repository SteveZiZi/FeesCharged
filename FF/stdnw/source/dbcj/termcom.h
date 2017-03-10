/** @file
 *  @brief 终端寄存器采集类
 *  @author 
 *  @date 2013/03/09
 *  @version 0.1
 *  @note 终端寄存器采集类定义和实现
 */
#ifndef __TERM_COMMUNICATION_H__
#define __TERM_COMMUNICATION_H__
#include "metercom.h"

/** @class
 *  @brief 终端寄存器采集
 */
class C_TermCom:public C_MeterCom
{
public:
	C_TermCom(void);
	virtual ~C_TermCom();

public:
	/** @fn
	 *  @brief 初始化所有配置的表计规约
	 *  @return 0:成功; -1:失败
	 */
	virtual int InitMeterRules(void);

	/** @brief 清除缓冲
	 *  @param[in] MeterRuleID 表计规约ID
	 *  @return true 成功；false 失败
	 *  @note 仅在Read操作返回数据异常时需要此操作
	 */
	virtual bool ClearBuffer(int MeterRuleID);
	/** @brief 读表计数据
	 *  @param[in] Meter 表计信息
	 *  @param[in] CommReg 通用寄存器
	 *  @param[out] strData 数据输出
	 *  @return <0:出错；>=0:成功
	 */
	virtual int Read(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData);
	/** @brief 写表计参数
	 *  @param[in] Meter 表计信息
	 *  @param[in] CommReg 通用寄存器
	 *  @param[in] strParam 输入参数
	 *  @param[out] strData 数据输出
	 *  @return <0:出错；>=0:成功
	 */
	virtual int Write(const S_MeterBase &Meter, const S_REGISTER &CommReg, const string &strParam, string &strData);
	/** @brief 上报表计数据
	 *  @param[in] CommReg 通用寄存器
	 *  @param[out] strMeterAddress 表计地址输出[6BYTES]
	 *  @param[out] strData 数据输出
	 *  @param[in] timeKeeping 上报时间;0:退出上报;0xFFFFFFFF:查询;其它:进入上报
	 *  @return <0:出错 0:结束上报 >0:上报剩余时间
	 */
	virtual int Report(const S_REGISTER &CommReg, string &strMeterAddress, string &strData, S_TIME timeKeeping);
	/** @brief 广播表计时间
	 *  @param[in] Meter 表计信息
	 *  @param[in] timeExpected 期望时间
	 *  @return <0:出错；>=0:成功
	 */
	virtual int Broadcast(const S_MeterBase &Meter, S_TIME timeExpected);

protected:
	/** @brief 读表计数据
	 *  @param[in] Meter 表计信息
	 *  @param[in] CommReg 通用寄存器
	 *  @param[out] strData 数据输出
	 *  @param[in] fRsvReg true备用寄存器;false主寄存器
	 *  @return <0:[MRULE_PARSE_ERR_]出错；>=0:成功
	 */
	int _Read(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData, bool fRsvReg);
};

#endif//__TERM_COMMUNICATION_H__


