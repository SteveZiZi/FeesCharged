/** @file
 *  @brief 终端寄存器定义和实现
 *  @author 
 *  @date 2013/03/09
 *  @version 0.1
 *  @note 终端流量，数据统计等等寄存器
 */
#ifndef __TERM_REGISTER_H__
#define __TERM_REGISTER_H__
#include "basetype.h"
#include "sysbase.h"

/** @class
 *  @brief 终端寄存器定义
 */
class C_TermReg
{
public:
	/** @fn
	 *  @brief 获取寄存器数据
	 *  @return 0:成功; -1:失败
	 */
	virtual int GetData(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData);
};
class C_TermReg1001:public C_TermReg //10010000	BIN	4	NNNNNNNN	当日通讯流量
{
public:
	virtual int GetData(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData);
};
class C_TermReg1002:public C_TermReg //10020000	BIN	4	NNNNNNNN	当月通讯流量
{
public:
	virtual int GetData(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData);
};
class C_TermReg1003:public C_TermReg //10030000	BIN	4	NNNNNNNN	上日通讯流量
{
public:
	virtual int GetData(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData);
};
class C_TermReg1004:public C_TermReg //10040000	BIN	4	NNNNNNNN	上月通讯流量
{
public:
	virtual int GetData(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData);
};
class C_TermReg1005:public C_TermReg //10050000	BIN	2	NNNN	终端当日供电时间（min）
{
public:
	virtual int GetData(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData);
};
class C_TermReg1006:public C_TermReg //10060000	BIN	2	NNNN	终端当月供电时间（min）
{
public:
	virtual int GetData(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData);
};
class C_TermReg1007:public C_TermReg //10070000	BIN	2	NNNN	终端上日供电时间（min）
{
public:
	virtual int GetData(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData);
};
class C_TermReg1008:public C_TermReg //10080000	BIN	2	NNNN	终端上月供电时间（min）
{
public:
	virtual int GetData(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData);
};
class C_TermReg1009:public C_TermReg //10090000	BIN	2	NNNN	终端当日复位累计次数
{
public:
	virtual int GetData(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData);
};
class C_TermReg100A:public C_TermReg //100A0000	BIN	2	NNNN	终端当月复位累计次数
{
public:
	virtual int GetData(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData);
};
class C_TermReg100B:public C_TermReg //100B0000	BIN	2	NNNN	终端上日复位累计次数
{
public:
	virtual int GetData(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData);
};
class C_TermReg100C:public C_TermReg //100C0000	BIN	2	NNNN	终端上月复位累计次数
{
public:
	virtual int GetData(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData);
};


//
//终端寄存器对象
//
struct S_TermRegMap
{
	S_DWORD m_RegID;
	C_TermReg *m_pTermRegObj;
};
inline bool operator<(const S_TermRegMap &left, const S_TermRegMap &right){return left.m_RegID < right.m_RegID;}

/** @class
 *  @brief 终端寄存器工厂
 */
class C_TermRegFact
{
public:
	static C_TermReg *GetTermRegister(S_DWORD RegID);
};

#endif//__TERM_REGISTER_H__


