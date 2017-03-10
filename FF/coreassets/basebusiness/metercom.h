/**
 *  @file
 *  @brief 表计通讯
 *  @author 
 *  @date 2012-05-21
 *  @version v1.0
 *  @note
 *  完成表计采集功能
 *  完成表计（现场总线）数据上报功能
 */
#ifndef __BASEB_METER_COMMUNICATION_H__
#define __BASEB_METER_COMMUNICATION_H__
#include "basedef.h"
#include "basetype.h"

enum
{
	METER_READ_ERROR_FILL_DATA = 1, // 填充数据
	METER_READ_ERROR_NO = 0, // 通讯成功
	METER_READ_ERROR_NO_FRAME = -1, // 通讯失败

	METER_READ_ERROR_OTHER = -99, // 其它错误
	METER_READ_ERROR_END = -100
};

/** @class
 *  @brief 表计通讯
 */
class C_MeterCom
{
public:
	C_MeterCom(void);
	virtual ~C_MeterCom();

public:
	char SetFillChar(char bFillChar);

	/** @fn
	 *  @brief 初始化所有配置的表计规约
	 *  @return 0:成功; -1:失败
	 */
	virtual int InitMeterRules(const char *pResourcePath);

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
	 *  @return <0:出错；>=0:成功。see[METER_READ_ERROR_]
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
	 *  @param[out] ReportDatas 表计数据输出
	 *  @param[out] idxSuccess 采集成功表计索引输出[ReportDatas的索引]
	 *  @param[in] SuccessfulMPBS 成功的测量点集
	 *  @param[in] CommRegs 通用寄存器集
	 *  @param[in] timeKeeping 上报时间;0:退出上报;0xFFFFFFFF:查询;其它:进入上报
	 *  @return <0:出错 0:结束上报 >0:上报剩余时间
	 */
	virtual int Report(S_ReportDatas &ReportDatas, S_WORD &idxSuccess, const S_MPBS &SuccessfulMPBS, const S_REGISTERS &CommRegs, S_TIME timeKeeping);
	/** @brief 广播表计时间
	 *  @param[in] Meter 表计信息
	 *  @param[in] timeExpected 期望时间
	 *  @return <0:出错；>=0:成功及延时的秒数
	 */
	virtual int Broadcast(const S_MeterBase &Meter, S_TIME timeExpected);
	/** @brief 跳合闸、报警、保电
	 *  @param[in] Meter 表计信息
	 *  @param[in] strPassword 密码 PAP1P2P3
	 *  @param[in] strOperator 操作者代码
	 *  @param[in] strParam 数据输入缓冲
	 *  @return <0:出错；>=0:成功
	 */
	virtual int Brake(const S_MeterBase &Meter, const string &strPassword, const string &strOperator, const string &strParam);
};


//
//采集对象工厂
//
class C_ComObjFact
{
public:
	/** @brief 获取表计通信组件 
	 *  @param[in] ObjType 寄存器的源[1-9] 取寄存器的高4位
	 *  @see <kge common registor manual.doc>
	 *  @note
		3*******：脉冲源的寄存器
		2*******：电压电流模拟量源的寄存器
		1*******：终端寄存器，如月流量，日GPRS信号
		其它为表计寄存器
	 */
	static C_MeterCom* GetComObj(S_BYTE ObjType);

	/** @brief 设置用户表计通信组件 */	
	static int SetComObj(S_ComObj ObjType, C_MeterCom *pMeterCom);
};

#endif //__BASEB_METER_COMMUNICATION_H__


