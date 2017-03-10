/**
 *  @file
 *  @brief 寄存器及值转换
 *  @author 
 *  @date 2012-05-10
 *  @version v1.0
 *  @note
 *  1、通用寄存器转换为本地寄存器
 *  2、本地寄存器数据转换为通用寄存器要求的数据
 */
#ifndef __BASEB_REGISTER_CONVERSION_H__
#define __BASEB_REGISTER_CONVERSION_H__
#include "mrule.h"

class C_Add33
{
public:
	S_BYTE operator()(S_BYTE ch){return (S_BYTE)(ch+0x33);}
};

class C_Sub33
{
public:
	S_BYTE operator()(S_BYTE ch){return (S_BYTE)(ch-0x33);}
};

class C_RegConv
{
public:
	static int FindRegisterMap(const S_RegisterMaps &RegMaps, const S_REGISTER &CommReg, S_RegisterMap &RegMap);
	static int FindLocalRegister(const S_RegisterMaps &RegMaps, const S_REGISTERS &LocalRegisters, const S_REGISTER &CommReg, S_REGISTER &LocalReg);
	static int FindLocalRegister(const S_RegisterMaps3 &RegMaps, const S_REGISTERS &LocalRegisters, const S_REGISTER &CommReg, S_REGISTER &LocalReg, S_WORD Idx);
	static int DataConversion(const S_REGISTER &LocalReg, const string &strLocalData, const S_REGISTER &CommReg, string &strData);

private:
	static bool RegisterEquare(const S_REGISTER &LeftReg, const S_REGISTER &RightReg);

	static int Conversion_BCD(const S_REGISTER &LocalReg, const string &strLocalData, const S_REGISTER &CommReg, string &strData);
	static int Conversion_BIN(const S_REGISTER &LocalReg, const string &strLocalData, const S_REGISTER &CommReg, string &strData);
	static int Conversion_BCD_BLOCK(const S_REGISTER &LocalReg, const string &strLocalData, const S_REGISTER &CommReg, string &strData);
	static int Conversion_BCD_TARIFF(const S_REGISTER &LocalReg, const string &strLocalData, const S_REGISTER &CommReg, string &strData);
	static int Conversion_BCD_TIME(const S_REGISTER &LocalReg, const string &strLocalData, const S_REGISTER &CommReg, string &strData);
	static int Conversion_BCD_TIME_BLOCK(const S_REGISTER &LocalReg, const string &strLocalData, const S_REGISTER &CommReg, string &strData);
	static int Conversion_ASCII(const S_REGISTER &LocalReg, const string &strLocalData, const S_REGISTER &CommReg, string &strData);
	static int Conversion_STATUS(const S_REGISTER &LocalReg, const string &strLocalData, const S_REGISTER &CommReg, string &strData);
	static int Conversion_TIME(const S_REGISTER &LocalReg, const string &strLocalData, const S_REGISTER &CommReg, string &strData);
	static int Conversion_TIME_BLOCK(const S_REGISTER &LocalReg, const string &strLocalData, const S_REGISTER &CommReg, string &strData);
	static int Conversion_TIME_TARIFF(const S_REGISTER &LocalReg, const string &strLocalData, const S_REGISTER &CommReg, string &strData);
	static int Conversion_TIME_BIN(const S_REGISTER &LocalReg, const string &strLocalData, const S_REGISTER &CommReg, string &strData);
	static int Conversion_TIME_BIN_BLOCK(const S_REGISTER &LocalReg, const string &strLocalData, const S_REGISTER &CommReg, string &strData);
	static int Conversion_COMB(const S_REGISTER &LocalReg, const string &strLocalData, const S_REGISTER &CommReg, string &strData);
	static int Conversion_OTHER(const S_REGISTER &LocalReg, const string &strLocalData, const S_REGISTER &CommReg, string &strData);

	static int Conversion_OTHER_GB645_1997(const S_REGISTER &LocalReg, const string &strLocalData, const S_REGISTER &CommReg, string &strData);
	static int Conversion_OTHER_GB645_2007(const S_REGISTER &LocalReg, const string &strLocalData, const S_REGISTER &CommReg, string &strData);

	static int Conversion_TIME_1(const S_REGISTER &LocalReg, const string &strLocalData, const S_REGISTER &CommReg, string &strData);
	static int Conversion_TIME_2(const S_REGISTER &LocalReg, const string &strLocalData, const S_REGISTER &CommReg, string &strData);
	static int Conversion_TIME_TARIFF_1(const S_REGISTER &LocalReg, const string &strLocalData, const S_REGISTER &CommReg, string &strData);

	static int Conversion_STATUS_8(const S_REGISTER &LocalReg, const string &strLocalData, const S_REGISTER &CommReg, string &strData);
	static int Conversion_STATUS_9(const S_REGISTER &LocalReg, const string &strLocalData, const S_REGISTER &CommReg, string &strData);
	static int Conversion_STATUS_10(const S_REGISTER &LocalReg, const string &strLocalData, const S_REGISTER &CommReg, string &strData);
	static int Conversion_STATUS_100(const S_REGISTER &LocalReg, const string &strLocalData, const S_REGISTER &CommReg, string &strData);

	static int ShiftString(const S_REGISTER &LocalReg, const string &strLocalData, const S_REGISTER &CommReg, string &strData);
	static int Conversion_BCD_SIGN(const S_REGISTER &LocalReg, const string &strLocalData, string &strData);
	static int Conversion_BCD_SIGN(int sign, string &strData);
};


#endif //__BASEB_REGISTER_CONVERSION_H__

