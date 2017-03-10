/**
 *  @file
 *  @brief ����ҵ���㷨��
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
//��ѹ�������ݿ�
//
struct S_UI
{
	char m_VoltageABC[3][2]; ///< ABC����,��ʽ������ԼA.7
	char m_CurrentABC[4][3]; ///< ABC�������,��ʽ������ԼA.25
};

/** @class
 *  @brief ����ҵ���㷨��
 *  @note 
 */
class C_BusiAlgo
{
public:
	/**  @brief BCD��ת��DWORD
	 *  @param[in] pbcdData:BCD����
	 *  @param[in] BitSizeInteger:BCD��������λ��,���ܳ���8λ
	 *  @return DWORD
	 */
	static S_DWORD ChangeBcd2Dword(const S_BYTE *pbcdData, S_BYTE BitSizeInteger);
	/**  @brief dword��ת��BCD
	 *  @param[in] dwValue: ����[���ܳ���4�ֽ�BCD���ʾ�������]
	 *  @return BCD ���ݻ���
	 */
	static string ChangeDword2Bcd(S_DWORD dwValue);
	/**  @brief BCD��ת��ASCII�ַ���
	 *  @param[in] pbcdData:BCD����
	 *  @param[in] BitSizeInteger:BCD��������λ��,���ܳ���8λ
	 *  @param[in] BitSizeFloat:BCDС������λ��,���ܳ���4λ
	 *  @param[in] fSign: true�з���λ;false�޷���λ
	 *  @return string ASCII�봮
	 *  @note:���ֽڵĸ�λΪ����λ�����еĻ�
	 */
	static string ChangeBcd2Ascii(const S_BYTE *pbcdData, S_BYTE BitSizeInteger, S_BYTE BitSizeFloat, bool fSign = false);
	/**  @brief ASCII����ת��BCD
	 *  @param[in] strAscii: ����
	 *  @param[out] pbcdData:BCD����
	 *  @param[in] BitSizeInteger:BCD��������λ��,���ܳ���8λ
	 *  @param[in] BitSizeFloat:BCDС������λ��,���ܳ���4λ
	 *  @return BCD ���ݻ���ָ��
	 *  @note:���ֽڵĸ�λΪ����λ�����еĻ�
	 */
	static S_BYTE *ChangeAscii2Bcd(const string &strAscii, S_BYTE *pbcdData, S_BYTE BitSizeInteger, S_BYTE BitSizeFloat);
	/**  @brief BCD��ת��double
	 *  @param[in] pbcdData:BCD����
	 *  @param[in] BitSizeInteger:BCD��������λ��,���ܳ���8λ
	 *  @param[in] BitSizeFloat:BCDС������λ��,���ܳ���4λ
	 *  @param[in] fSign: true�з���λ;false�޷���λ
	 *  @return Value�������ţ�
	 *  @note:���ֽڵĸ�λΪ����λ�����еĻ�
	 */
	static double ChangeBcd2Double(const S_BYTE *pbcdData, S_BYTE BitSizeInteger, S_BYTE BitSizeFloat, bool fSign = false);
	/**  @brief double��ת��BCD
	 *  @param[in] fValue: ����
	 *  @param[out] pbcdData:BCD����
	 *  @param[in] BitSizeInteger:BCD��������λ��,���ܳ���8λ
	 *  @param[in] BitSizeFloat:BCDС������λ��,���ܳ���4λ
	 *  @return BCD ���ݻ���ָ��
	 *  @note:���ֽڵĸ�λΪ����λ�����еĻ�
	 */
	static S_BYTE *ChangeDouble2Bcd(double fValue, S_BYTE *pbcdData, S_BYTE BitSizeInteger, S_BYTE BitSizeFloat);
	/**  @brief ���ݵ�ѹ�����������ڹ���
	 *  @param[in] AbcUI:��ѹ��������
	 *  @return ApparentPower�������ţ�
	 */
	static double ComputeApparentPower(const S_UI &AbcUI);
	/**  @brief ���ݵ�ѹ�����������ڹ���
	 *  @param[in] AbcUI:��ѹ��������
	 *  @param[in] dummy:��δ����
	 *  @return ApparentPower(��ʽ������ԼA.9)
	 */
	static S_DWORD ComputeApparentPower(S_DWORD dummy, const S_UI &AbcUI);
};

#endif //__BASEB_BUSINESS_ALGO_H__


