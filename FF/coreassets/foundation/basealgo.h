/** @file
 *  @brief 基础算法集合
 *  @author 
 *  @date 2012/02/14
 *  @version 0.1
 */
#ifndef __FND_BASEALGO_H__
#define __FND_BASEALGO_H__
#include "basedef.h"

//for_Stat 对区间进行统计或处理，结果由_Stat返回
template <class _InputIter, class _Stat>
void for_Stat(_InputIter __first, _InputIter __last, _Stat &__S) {
	for ( ; __first != __last; ++__first)
		__S(*__first);
}

/** @brief 进程退出信号描述*/
const char *LinuxProcessExitSignalDescript(int signal);
/** @brief LINUX API error info */
const char *LinuxApiErrorInfo(char *pBuffer);

/** @brief 配置项对应表 */
struct S_CFGITEM
{
	int m_val;
	const char *m_pkey;
};

/** @fn C_IniBase::MakeKey
 *  @brief 取指定名称
 *  @param[in] val 指定值
 *  @param[in] pCfgItem 索引名称数组指针
 *  @param[in] size 数组大小
 *  @return "unknow"或Key名称//NULL 无效指针；!NULL 数组项指针
 */
const char *CfgItem_MakeKey(const S_CFGITEM *pCfgItem, S_WORD size, int val);
/** @fn C_IniBase::MakeValue
 *  @brief 取索引号
 *  @param[in] pkey 指定名称
 *  @param[in] pCfgItem 索引名称数组指针
 *  @param[in] size 数组大小
 *  @return >=0 有效值 -1 无效值
 */
int CfgItem_MakeValue(const S_CFGITEM *pCfgItem, S_WORD size, const char *pkey);

/** @brief 配置项方法集
 */
class C_CfgItem
{
public:
	C_CfgItem(const S_CFGITEM *pCfgItem, S_WORD size);
	~C_CfgItem();

public:
	/** @fn MakeKey
	 *  @brief 取项目名称
	 *  @param[in] val 项目值
	 *  @return "unknow"或Key名称//NULL 无效指针；!NULL 数组项指针
	 */
	const char *MakeKey(int val);
	/** @fn MakeValue
	 *  @brief 取项目值
	 *  @param[in] pkey 项目名称
	 *  @return >=0 有效值 -1 无效值
	 */
	int MakeValue(const char *pkey);
	/** @fn MakeIndex
	 *  @brief 取项目索引
	 *  @param[in] val 项目值
	 *  @return >=0 有效值 -1 无效值
	 */
	int MakeIndex(int val);
	/** @fn MakeIndex
	 *  @brief 取项目索引
	 *  @param[in] pkey 项目名称
	 *  @return >=0 有效值 -1 无效值
	 */
	int MakeIndex(const char *pkey);
	/** @fn MakeKey
	 *  @brief 取项目名称
	 *  @param[in] idx 项目索引
	 *  @return NULL或Key名称
	 */
	const char *MakeKeyWithIdx(S_WORD idx);
	/** @fn MakeValue
	 *  @brief 取项目值
	 *  @param[in] idx 项目索引
	 *  @return >=0 有效值 -1 无效值
	 */
	int MakeValueWithIdx(S_WORD idx);
	/** @fn SerializeItemKey
	 *  @brief 串行化项目名称
	 *  @param[out] strItemKey 输出缓冲
	 *  @param[in] delimit 间隔符
	 *  @return 串行化后的数据长度
	 */
	S_WORD SerializeItemKey(string &strItemKey, char delimit);

protected:
	const S_CFGITEM *m_pCfgItem;
	S_WORD m_Size;
};

/** @brief BCD码算法
 *  @note
 *  1、最大支持8位整数，4位小数
 *  2、所有BCD缓冲中，BCD码按小端存放
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

	/** @brief 取BCD数据
	 *  @return 返回格式为前4位小数后8位整数，小端方式
	 */
	const S_BYTE *GetBcdData(void) const;
	/** @brief 取BCD数据
	 *  @param[out] Integer 16进制整数
	 *  @param[out] Float 16进制小数
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
// BCD串处理集
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

/** @brief 基础函数
 */
class C_BaseFn
{
public:
	/** @brief 取BCD数据
	 *  @param[in|out] strData 数据
	 *  @param[in] interval 间隔
	 *  @param[in] delimit 分隔符
	 *  @return 格式化后的长度
	 */
	static int FormatDataWithDelimit(string &strData, size_t interval, char delimit);
	/** @brief 检查网线是否插入
	 *  @param[in] pDevice 设备名
	 *  @return true:yes;false:no;
	 */
	static bool CheckNetLinePlugin(const char *pDevice);
	/** @brief 检查TCP模式下的路由建链
	 *  @param[in] pDevice 设备名
	 *  @param[in] pDestIP 目标IP
	 *  @return true:yes;false:no;
	 *  @note 仅在TCP连接下有效
	 */
	static bool CheckRouterOfTcpMode(const char *pDevice, const char *pDestIP);
};

#endif //__FND_BASEALGO_H__


