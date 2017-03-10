/** @file
 *  @brief 基础类bitset定义
 *  @author 
 *  @date 2011/11/07
 *  @version 0.1
 */
#ifndef __FND_BITSET_EX_H__
#define __FND_BITSET_EX_H__
#include "basedef.h"
#include "strings.h"

/** @brief 封装sprintf,方便使用
 *  @param[out] pBuffer 十进制数输出缓冲
 *  @param[in] val 打印数值
 *  @return pBuffer
 */
char *bitset_MyPrintf(char *pBuffer, int val);
/** @brief 输出格式化串：如1-3,5-8
 *  @param[out] strBitset 数出缓冲
 *  @param[in] from 开始数值
 *  @param[in] to 结束数值
 *  @param[in] delimit 字段中间分隔，如'-'
 *  @param[in] field 字段分隔，如','
 *  @return void
 */
void bitset_PrintString(string &strBitset, int from, int to, char link, char field);

/** @class
 *  @brief 扩展bitset定义
 */
template <size_t _Bits>
class BitSetEx:public bitset<_Bits>
{
public:
	BitSetEx(void){m_FromPos = 0, m_ToPos = 0;}
	
	size_t begin(void)const{return m_FromPos;}
	size_t end(void)const{return m_ToPos;}
	bool empty(void)const{return m_ToPos == 0;}

	/** @brief 第一个有效位*/
	size_t firstbit(void)const{return _nextbit(0);}
	/** @brief 下一个有效位*/
	size_t nextbit(size_t prev)const{return _nextbit(prev+1);}

	bitset<_Bits>& operator-=(const bitset<_Bits>& _Right)
	{
		return operator ^=(_Right & (*this) );
	}
	BitSetEx<_Bits>& operator=(const BitSetEx<_Bits>& _Right)
	{
		if (this != &_Right)
		{
			this->reset();
			for (size_t i = 0; i < _Right.size(); ++i)
				if (_Right.test(i)) this->set(i);
			Stat();
		}
		return *this;
	}
	/** @brief 初始化开始与结束位置 */
	void Stat(void)
	{
		m_FromPos = 0, m_ToPos = 0;
		for(size_t i = 0; i < this->size(); i++)
		{
			if (this->test(i))
			{
				if (m_ToPos == 0)
					m_FromPos = i;//first find
				m_ToPos = i + 1;
			}
		}
	}
	/** @brief 将from-to区间置1 */
	void SetBit(int from, int to){for (; from <= to; from++) this->set(from);}
	/** @brief 按位串描述置位
	 *  @param[in] strBitset 位串，如"1-3", "11001"
	 *  @param[in] link 分隔符，如'-', 0
	 */
	void SetBit(const string &strBitset, char link)
	{
		if (link == 0) SetBitNoDelimit(strBitset);
		else SetBitWithDelimit(strBitset, link);
	}
	/** @brief 分析参数串
	 *  @param[in] strParam 数出缓冲
	 *  @param[in] delimit 字段中间分隔，如'-'
	 *  @param[in] field 字段分隔，如','
	 *  @return void
	 */
	void ParseString(const string &strParam, char link = '-', char field = ',')
	{
		strings strField;
		if (strField.split(strParam, field) != 0)
		{
			for(size_t i = 0; i < strField.size(); i++)
				SetBit(strField[i], link);
		}
	}
	/** @brief 输出格式化串：如1-3,5-8
	 *  @param[out] strBitset 数出缓冲
	 *  @param[in] delimit 字段中间分隔，如'-'
	 *  @param[in] field 字段分隔，如','
	 *  @return void
	 */
	void PrintString(string &strBitset, char link = '-', char field = ',')const
	{
		if (link == 0) PrintStringNoDelimit(strBitset);
		else PrintStringWithDelimit(strBitset, link, field);
	}
	/** @brief 分析参数串
	 *  @param[in] strParam 参数串，格式需为：1-3,5-8
	 *  @return void
	 */
	void ParseParameter(const string &strParam)
	{
		ParseString(strParam, '-', ',');
	}
	/** @brief 输出参数串
	 *  @param[out] strParamOut 参数串输出缓冲，格式：1-3,5-8
	 *  @return void
	 */
	void PrintParameter(string &strParamOut)const
	{
		PrintString(strParamOut, '-', ',');
	}
protected:
	/** @brief 寻找有效位*/
	size_t _nextbit(size_t prev)const
	{
		for (; prev < this->size(); prev++)
		{
			if (this->test(prev) == true) return prev;
		}
		return this->size();
	}
	/** @brief 按串信息设置位 */
	void SetBitNoDelimit(const string &strBitset)
	{
		for (size_t i = 0; i<strBitset.size() && i<this->size(); i++)
			if (strBitset[i] == '1') this->set(i);
	}
	/** @brief 按串信息(含连接符)设置位 */
	void SetBitWithDelimit(const string &strBitset, char link)
	{
		strings strField;
		if (strField.split(strBitset, link) > 0)
		{
			int from = atoi(strField[0].c_str()), to = from;
			if (strField.size() >= 2) to = atoi(strField[1].c_str());
			if (from < 0) from = 0;//容错
			if (to < 0 || to >= (int)this->size()) to = (int)this->size() - 1;//容错
			SetBit(from, to);
		}
	}
	/** @brief 输出位信息串 */
	void PrintStringNoDelimit(string &strBitset)const
	{
		for (size_t i = 0; i < this->size(); i++)
		{
			if (this->test(i)) strBitset.push_back('1');
			else strBitset.push_back('0');
		}
	}
	/** @brief 输出位信息串(含连接符) */
	void PrintStringWithDelimit(string &strBitset, char link, char field)const
	{
		int from = -1, to = -1;
		for (size_t i = 0; i < this->size(); i++)
		{
			if (this->test(i))//测试连续位
			{
				if (from == -1) from = (int)i;
				to = (int)i;
			}
			else if (from != -1)//位段
			{
				bitset_PrintString(strBitset, from, to, link, field);
				from = -1, to = -1;
			}
		}
		if (from != -1 && to != -1)
            bitset_PrintString(strBitset, from, to, link, field);
	}

protected:
	size_t m_FromPos; ///< 有效开始位置
	size_t m_ToPos; ///< 有效结束位置
};

/** @brief 字节内位数统计 */
static char C_BITSET_ByteBitCnt[] =
{
	0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,
	1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
	1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
	2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
	1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
	2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
	2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
	3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
	1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
	2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
	2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
	3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
	2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
	3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
	3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
	4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8
};

/** @brief 位处理类
 *  @note
 *  与STL bitset区别:1,类型参数是字节数; 2,可以返回位数据; 3,缓冲按BYTE存储
 */
template <int ByteCnt>
class C_BITSET
{
public:
	C_BITSET(void){ClearBit();}
	C_BITSET(const unsigned char *pString){ClearBit();this->ParseStr(pString);}
	C_BITSET(const unsigned char *pData, int Bytes){ClearBit();SetBit(pData, Bytes);}
	C_BITSET(const C_BITSET &r){if (this != &r) SetBit(r.m_data, sizeof(r.m_data));}
	~C_BITSET(){}

	/** @brief 取字节数 */
	int GetByteCnt(void)const{return ByteCnt;}
	/** @brief 取位数 */
	int GetBitSize(void)const{return GetByteCnt()*8;}
	/** @brief 取位缓冲 */
	const unsigned char *GetData(void)const{return m_data;}
	/** @brief 取位缓冲 */
	unsigned char *GetData(void){return m_data;}

	/** @brief pos位置0 */
	void ClearBit(int pos){m_data[pos/8] &= (unsigned char)(~(1<<(pos%8)));}
	/** @brief 全部位置0 */
	void ClearBit(void){memset(m_data, 0x00, sizeof(m_data));}
	/** @brief 测试pos位是否为1 */
	bool TestBit(int pos)const{return (m_data[pos/8] & (1<<(pos%8)))?true:false;}
	/** @brief 按pData缓冲设置位 */
	void SetBit(const unsigned char *pData, int Bytes){memcpy(m_data, pData, Bytes);}
	/** @brief pos位置1 */
	void SetBit(int pos){m_data[pos/8] |= (unsigned char)(1<<(pos%8));}
	/** @brief 全部位置1 */
	void SetBit(void){memset(m_data, 0xFF, sizeof(m_data));}
	/** @brief 设置from-to区间为1 */
	void SetBit(int from, int to){for (; from <= to; from++) SetBit(from);}
	/** @brief 按位串置位 */
	void SetBit(const string &strBitset, char link)
	{
		if (link == 0) SetBitNoDelimit(strBitset);
		else SetBitWithDelimit(strBitset, link);
	}
	/** @brief 反转位 */
	void FlipBit(int pos){if (TestBit(pos)) ClearBit(pos); else SetBit(pos);}
	void FlipBit(void){for (int i = 0; i < ByteCnt; i++) m_data[i] = ~m_data[i];}

	/** @brief 第一个有效位*/
	int FirstBit(void)const{return _NextBit(0);}
	/** @brief 下一个有效位*/
	int NextBit(int prev)const{return _NextBit(prev+1);}

	/** @brief 分析参数串
	 *  @param[in] strParam 数出缓冲
	 *  @param[in] delimit 字段中间分隔，如'-'
	 *  @param[in] field 字段分隔，如','
	 *  @return void
	 */
	void ParseString(const string &strParam, char link = '-', char field = ',')
	{
		strings strField;
		if (strField.split(strParam, field) != 0)
		{
			for(size_t i = 0; i < strField.size(); i++)
				SetBit(strField[i], link);
		}
	}
	/** @brief 将位信息输出到串 */
	void PrintString(string &strBitset, char link = '-', char field = ',')const
	{
		if (link == 0) PrintStringNoDelimit(strBitset);
		else PrintStringWithDelimit(strBitset, link, field);
	}
	/** @brief 统计置1的位数 */
	int CountBit(void)const
	{
		int AllBit = 0;
		for (int i = 0; i < ByteCnt; i++)
			AllBit += C_BITSET_ByteBitCnt[m_data[i]];
		return AllBit;
	}

protected:
	/** @brief 寻找有效位*/
	int _NextBit(int prev)const
	{
		for (; prev < GetBitSize(); prev++)
		{
			if (TestBit(prev) == true) return prev;
		}
		return GetBitSize();
	}
	/** @brief 按串信息设置位 */
	void SetBitNoDelimit(const string &strBitset)
	{
		for (size_t i = 0; i<strBitset.size() && i<ByteCnt*8; i++)
			if (strBitset[i] == '1') SetBit((int)i);
	}
	/** @brief 按串信息(含连接符)设置位 */
	void SetBitWithDelimit(const string &strBitset, char link)
	{
		strings strField;
		if (strField.split(strBitset, link) > 0)
		{
			int from = atoi(strField[0].c_str()), to = from;
			if (strField.size() >= 2) to = atoi(strField[1].c_str());
			if (from < 0) from = 0;//容错
			if (to < 0 || to >= ByteCnt*8) to = ByteCnt*8 - 1;//容错
			SetBit(from, to);
		}
	}
	/** @brief 输出位信息串 */
	void PrintStringNoDelimit(string &strBitset)const
	{
		for (int i = 0; i < ByteCnt*8; i++)
		{
			if (TestBit(i)) strBitset.push_back('1');
			else strBitset.push_back('0');
		}
	}
	/** @brief 输出位信息串(含连接符) */
	void PrintStringWithDelimit(string &strBitset, char link, char field)const
	{
		int from = -1, to = -1;
		for (int i = 0; i < ByteCnt*8; i++)
		{
			if (TestBit(i))//测试连续位
			{
				if (from == -1) from = i;
				to = i;
			}
			else if (from != -1)//位段
			{
				bitset_PrintString(strBitset, from, to, link, field);
				from = -1, to = -1;
			}
		}
		if (from != -1 && to != -1)
			bitset_PrintString(strBitset, from, to, link, field);
	}
protected:
	unsigned char m_data[ByteCnt]; ///< 位缓冲
};

#endif//__FND_BITSET_EX_H__

