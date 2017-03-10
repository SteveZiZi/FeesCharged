/**
 *  @file
 *  @brief 基础业务算法集
 *  @author 
 *  @date 2013-03-21
 *  @version v1.0
 *  @note
 */
#ifndef __BASEB_BUSINESS_ALGO_H__
#define __BASEB_BUSINESS_ALGO_H__
#include "basedef.h"
#include "basetype.h"

//
//电压电流数据块
//
struct S_UI
{
	char m_VoltageABC[3][2]; ///< ABC三相,格式国网规约A.7
	char m_CurrentABC[4][3]; ///< ABC零序电流,格式国网规约A.25
};

/** @class
 *  @brief 基础业务算法集
 *  @note 
 */
class C_BusiAlgo
{
public:
	/**  @brief BCD码转成DWORD
	 *  @param[in] pbcdData:BCD数据
	 *  @param[in] BitSizeInteger:BCD整数数据位数,不能超过8位
	 *  @return DWORD
	 */
	static S_DWORD ChangeBcd2Dword(const S_BYTE *pbcdData, S_BYTE BitSizeInteger);
	/**  @brief dword码转成BCD
	 *  @param[in] dwValue: 数据[不能超过4字节BCD码表示的最大数]
	 *  @return BCD 数据缓冲
	 */
	static string ChangeDword2Bcd(S_DWORD dwValue);
	/**  @brief BCD码转成ASCII字符串
	 *  @param[in] pbcdData:BCD数据
	 *  @param[in] BitSizeInteger:BCD整数数据位数,不能超过8位
	 *  @param[in] BitSizeFloat:BCD小数数据位数,不能超过4位
	 *  @param[in] fSign: true有符号位;false无符号位
	 *  @return string ASCII码串
	 *  @note:高字节的高位为符号位，如有的话
	 */
	static string ChangeBcd2Ascii(const S_BYTE *pbcdData, S_BYTE BitSizeInteger, S_BYTE BitSizeFloat, bool fSign = false);
	/**  @brief ASCII数据转成BCD
	 *  @param[in] strAscii: 数据
	 *  @param[out] pbcdData:BCD数据
	 *  @param[in] BitSizeInteger:BCD整数数据位数,不能超过8位
	 *  @param[in] BitSizeFloat:BCD小数数据位数,不能超过4位
	 *  @return BCD 数据缓冲指针
	 *  @note:高字节的高位为符号位，如有的话
	 */
	static S_BYTE *ChangeAscii2Bcd(const string &strAscii, S_BYTE *pbcdData, S_BYTE BitSizeInteger, S_BYTE BitSizeFloat);
	/**  @brief BCD码转成double
	 *  @param[in] pbcdData:BCD数据
	 *  @param[in] BitSizeInteger:BCD整数数据位数,不能超过8位
	 *  @param[in] BitSizeFloat:BCD小数数据位数,不能超过4位
	 *  @param[in] fSign: true有符号位;false无符号位
	 *  @return Value（带符号）
	 *  @note:高字节的高位为符号位，如有的话
	 */
	static double ChangeBcd2Double(const S_BYTE *pbcdData, S_BYTE BitSizeInteger, S_BYTE BitSizeFloat, bool fSign = false);
	/**  @brief double码转成BCD
	 *  @param[in] fValue: 数据
	 *  @param[out] pbcdData:BCD数据
	 *  @param[in] BitSizeInteger:BCD整数数据位数,不能超过8位
	 *  @param[in] BitSizeFloat:BCD小数数据位数,不能超过4位
	 *  @return BCD 数据缓冲指针
	 *  @note:高字节的高位为符号位，如有的话
	 */
	static S_BYTE *ChangeDouble2Bcd(double fValue, S_BYTE *pbcdData, S_BYTE BitSizeInteger, S_BYTE BitSizeFloat);
	/**  @brief 根据电压电流计算视在功率
	 *  @param[in] AbcUI:电压电流数据
	 *  @return ApparentPower（带符号）
	 */
	static double ComputeApparentPower(const S_UI &AbcUI);
	/**  @brief 根据电压电流计算视在功率
	 *  @param[in] AbcUI:电压电流数据
	 *  @param[in] dummy:暂未定义
	 *  @return ApparentPower(格式国网规约A.9)
	 */
	static S_DWORD ComputeApparentPower(S_DWORD dummy, const S_UI &AbcUI);
};

#endif //__BASEB_BUSINESS_ALGO_H__


