/** @file
 *  @brief �����㷨����
 *  @author 
 *  @date 2012/02/14
 *  @version 0.1
 */
#ifndef __FND_BASEALGO_H__
#define __FND_BASEALGO_H__
#include "basedef.h"

//for_Stat ���������ͳ�ƻ��������_Stat����
template <class _InputIter, class _Stat>
void for_Stat(_InputIter __first, _InputIter __last, _Stat &__S) {
	for ( ; __first != __last; ++__first)
		__S(*__first);
}

/** @brief �����˳��ź�����*/
const char *LinuxProcessExitSignalDescript(int signal);
/** @brief LINUX API error info */
const char *LinuxApiErrorInfo(char *pBuffer);

/** @brief �������Ӧ�� */
struct S_CFGITEM
{
	int m_val;
	const char *m_pkey;
};

/** @fn C_IniBase::MakeKey
 *  @brief ȡָ������
 *  @param[in] val ָ��ֵ
 *  @param[in] pCfgItem ������������ָ��
 *  @param[in] size �����С
 *  @return "unknow"��Key����//NULL ��Чָ�룻!NULL ������ָ��
 */
const char *CfgItem_MakeKey(const S_CFGITEM *pCfgItem, S_WORD size, int val);
/** @fn C_IniBase::MakeValue
 *  @brief ȡ������
 *  @param[in] pkey ָ������
 *  @param[in] pCfgItem ������������ָ��
 *  @param[in] size �����С
 *  @return >=0 ��Чֵ -1 ��Чֵ
 */
int CfgItem_MakeValue(const S_CFGITEM *pCfgItem, S_WORD size, const char *pkey);

/** @brief ���������
 */
class C_CfgItem
{
public:
	C_CfgItem(const S_CFGITEM *pCfgItem, S_WORD size);
	~C_CfgItem();

public:
	/** @fn MakeKey
	 *  @brief ȡ��Ŀ����
	 *  @param[in] val ��Ŀֵ
	 *  @return "unknow"��Key����//NULL ��Чָ�룻!NULL ������ָ��
	 */
	const char *MakeKey(int val);
	/** @fn MakeValue
	 *  @brief ȡ��Ŀֵ
	 *  @param[in] pkey ��Ŀ����
	 *  @return >=0 ��Чֵ -1 ��Чֵ
	 */
	int MakeValue(const char *pkey);
	/** @fn MakeIndex
	 *  @brief ȡ��Ŀ����
	 *  @param[in] val ��Ŀֵ
	 *  @return >=0 ��Чֵ -1 ��Чֵ
	 */
	int MakeIndex(int val);
	/** @fn MakeIndex
	 *  @brief ȡ��Ŀ����
	 *  @param[in] pkey ��Ŀ����
	 *  @return >=0 ��Чֵ -1 ��Чֵ
	 */
	int MakeIndex(const char *pkey);
	/** @fn MakeKey
	 *  @brief ȡ��Ŀ����
	 *  @param[in] idx ��Ŀ����
	 *  @return NULL��Key����
	 */
	const char *MakeKeyWithIdx(S_WORD idx);
	/** @fn MakeValue
	 *  @brief ȡ��Ŀֵ
	 *  @param[in] idx ��Ŀ����
	 *  @return >=0 ��Чֵ -1 ��Чֵ
	 */
	int MakeValueWithIdx(S_WORD idx);
	/** @fn SerializeItemKey
	 *  @brief ���л���Ŀ����
	 *  @param[out] strItemKey �������
	 *  @param[in] delimit �����
	 *  @return ���л�������ݳ���
	 */
	S_WORD SerializeItemKey(string &strItemKey, char delimit);

protected:
	const S_CFGITEM *m_pCfgItem;
	S_WORD m_Size;
};

/** @brief BCD���㷨
 *  @note
 *  1�����֧��8λ������4λС��
 *  2������BCD�����У�BCD�밴С�˴��
 */
class C_BCD
{
public:
	C_BCD(void);
	C_BCD(double val);
	C_BCD(S_DWORD Integer, S_WORD Float = 0);
	C_BCD(const string &strBcdAscii);
	C_BCD(const S_BYTE *pbcdInteger4B, const S_BYTE *pbcdFloat2B);
	C_BCD(const S_BYTE *pbcdBuffer, S_BYTE BitSizeInteger, S_BYTE BitSizeFloat);
	C_BCD(const C_BCD &r);
	~C_BCD();

	/** @brief ȡBCD����
	 *  @return ���ظ�ʽΪǰ4λС����8λ������С�˷�ʽ
	 */
	const S_BYTE *GetBcdData(void) const;
	/** @brief ȡBCD����
	 *  @param[out] Integer 16��������
	 *  @param[out] Float 16����С��
	 *  @return void
	 */
	void GetBcdData(S_DWORD &Integer, S_WORD &Float) const;

	bool operator!(void);
	C_BCD &operator=(const C_BCD &r);
	C_BCD &operator+=(const C_BCD &r);
	C_BCD &operator+=(S_DWORD val);
	C_BCD &operator-=(const C_BCD &r);
	C_BCD &operator-=(S_DWORD val);
	C_BCD &operator*=(const C_BCD &r);
	C_BCD &operator*=(S_DWORD val);
	C_BCD &operator/=(const C_BCD &r);
	C_BCD &operator/=(S_DWORD val);

	friend bool operator>(const C_BCD &left, const C_BCD &right);
	friend bool operator>=(const C_BCD &left, const C_BCD &right);
	friend bool operator<(const C_BCD &left, const C_BCD &right);
	friend bool operator<=(const C_BCD &left, const C_BCD &right);
	friend bool operator==(const C_BCD &left, const C_BCD &right);
	friend bool operator!=(const C_BCD &left, const C_BCD &right);

	operator double()const;
	operator string()const;

	bool Format(S_DWORD Integerbit, S_WORD Floatbit,string& formatDataStr);
	const S_BYTE *ConvAsciiToBCD(const string& strdata, string& bcdData);

private:
	bool ValidBcd(const S_BYTE *pbcdBuffer, S_WORD ByteSize);
	bool ValidBcd(const string &strBcdAscii);
	S_DWORD Convert(S_DWORD dwBCD);
	S_DWORD Convert(const string &strBcdAscii);
	void Correct(S_DWORD Integer, S_DWORD Float);
	bool PrintAscii(string &strBcdAscii) const;

private:
	static const S_DWORD POWER[8];// = {1,10,100,1000,10000,100000,1000000,10000000};
	S_DWORD m_Integer;
	S_WORD m_Float;
	bool m_ValidBcd;
	S_BYTE m_BitSizeInteger;
	S_BYTE m_BitSizeFloat;
	mutable S_BYTE m_bcdBuffer[6];
};

//nonmember
C_BCD operator+(const C_BCD &left, const C_BCD &right);
C_BCD operator+(const C_BCD &left, S_DWORD val);
C_BCD operator-(const C_BCD &left, const C_BCD &right);
C_BCD operator-(const C_BCD &left, S_DWORD val);
C_BCD operator*(const C_BCD &left, const C_BCD &right);
C_BCD operator*(const C_BCD &left, S_DWORD val);
C_BCD operator/(const C_BCD &left, const C_BCD &right);
C_BCD operator/(const C_BCD &left, S_DWORD val);

bool operator>(const C_BCD &left, const C_BCD &right);
bool operator>=(const C_BCD &left, const C_BCD &right);
bool operator<(const C_BCD &left, const C_BCD &right);
bool operator<=(const C_BCD &left, const C_BCD &right);
bool operator==(const C_BCD &left, const C_BCD &right);
bool operator!=(const C_BCD &left, const C_BCD &right);


//
// BCD������
//
class C_BcdString:public string
{
public:
	C_BcdString(const string &strBcd);
	C_BcdString(const string &strBcd, bool fVerifyBcd);
	C_BcdString(S_DWORD val);
	C_BcdString(INT64U val);

public:
	C_BcdString &operator<<(S_WORD BcdBit);
	C_BcdString &operator>>(S_WORD BcdBit);

	S_DWORD transform(S_DWORD val);
	INT64U transform(INT64U val);

	S_WORD PrintAscii(string &strAscii);
	S_WORD PrintAscii(string &strAscii, char delimit);
	string PrintAscii(S_BYTE BitSizeInteger, S_BYTE BitSizeFloat, bool fSign);

protected:
	bool VerifyBcd(const string &strData);
};

/** @brief ��������
 */
class C_BaseFn
{
public:
	/** @brief ȡBCD����
	 *  @param[in|out] strData ����
	 *  @param[in] interval ���
	 *  @param[in] delimit �ָ���
	 *  @return ��ʽ����ĳ���
	 */
	static int FormatDataWithDelimit(string &strData, size_t interval, char delimit);
	/** @brief ��������Ƿ����
	 *  @param[in] pDevice �豸��
	 *  @return true:yes;false:no;
	 */
	static bool CheckNetLinePlugin(const char *pDevice);
	/** @brief ���TCPģʽ�µ�·�ɽ���
	 *  @param[in] pDevice �豸��
	 *  @param[in] pDestIP Ŀ��IP
	 *  @return true:yes;false:no;
	 *  @note ����TCP��������Ч
	 */
	static bool CheckRouterOfTcpMode(const char *pDevice, const char *pDestIP);
};

#endif //__FND_BASEALGO_H__


