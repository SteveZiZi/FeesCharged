/** @file
 *  @brief ������bitset����
 *  @author 
 *  @date 2011/11/07
 *  @version 0.1
 */
#ifndef __FND_BITSET_EX_H__
#define __FND_BITSET_EX_H__
#include "basedef.h"
#include "strings.h"

/** @brief ��װsprintf,����ʹ��
 *  @param[out] pBuffer ʮ�������������
 *  @param[in] val ��ӡ��ֵ
 *  @return pBuffer
 */
char *bitset_MyPrintf(char *pBuffer, int val);
/** @brief �����ʽ��������1-3,5-8
 *  @param[out] strBitset ��������
 *  @param[in] from ��ʼ��ֵ
 *  @param[in] to ������ֵ
 *  @param[in] delimit �ֶ��м�ָ�����'-'
 *  @param[in] field �ֶηָ�����','
 *  @return void
 */
void bitset_PrintString(string &strBitset, int from, int to, char link, char field);

/** @class
 *  @brief ��չbitset����
 */
template <size_t _Bits>
class BitSetEx:public bitset<_Bits>
{
public:
	BitSetEx(void){m_FromPos = 0, m_ToPos = 0;}
	
	size_t begin(void)const{return m_FromPos;}
	size_t end(void)const{return m_ToPos;}
	bool empty(void)const{return m_ToPos == 0;}

	/** @brief ��һ����Чλ*/
	size_t firstbit(void)const{return _nextbit(0);}
	/** @brief ��һ����Чλ*/
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
	/** @brief ��ʼ����ʼ�����λ�� */
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
	/** @brief ��from-to������1 */
	void SetBit(int from, int to){for (; from <= to; from++) this->set(from);}
	/** @brief ��λ��������λ
	 *  @param[in] strBitset λ������"1-3", "11001"
	 *  @param[in] link �ָ�������'-', 0
	 */
	void SetBit(const string &strBitset, char link)
	{
		if (link == 0) SetBitNoDelimit(strBitset);
		else SetBitWithDelimit(strBitset, link);
	}
	/** @brief ����������
	 *  @param[in] strParam ��������
	 *  @param[in] delimit �ֶ��м�ָ�����'-'
	 *  @param[in] field �ֶηָ�����','
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
	/** @brief �����ʽ��������1-3,5-8
	 *  @param[out] strBitset ��������
	 *  @param[in] delimit �ֶ��м�ָ�����'-'
	 *  @param[in] field �ֶηָ�����','
	 *  @return void
	 */
	void PrintString(string &strBitset, char link = '-', char field = ',')const
	{
		if (link == 0) PrintStringNoDelimit(strBitset);
		else PrintStringWithDelimit(strBitset, link, field);
	}
	/** @brief ����������
	 *  @param[in] strParam ����������ʽ��Ϊ��1-3,5-8
	 *  @return void
	 */
	void ParseParameter(const string &strParam)
	{
		ParseString(strParam, '-', ',');
	}
	/** @brief ���������
	 *  @param[out] strParamOut ������������壬��ʽ��1-3,5-8
	 *  @return void
	 */
	void PrintParameter(string &strParamOut)const
	{
		PrintString(strParamOut, '-', ',');
	}
protected:
	/** @brief Ѱ����Чλ*/
	size_t _nextbit(size_t prev)const
	{
		for (; prev < this->size(); prev++)
		{
			if (this->test(prev) == true) return prev;
		}
		return this->size();
	}
	/** @brief ������Ϣ����λ */
	void SetBitNoDelimit(const string &strBitset)
	{
		for (size_t i = 0; i<strBitset.size() && i<this->size(); i++)
			if (strBitset[i] == '1') this->set(i);
	}
	/** @brief ������Ϣ(�����ӷ�)����λ */
	void SetBitWithDelimit(const string &strBitset, char link)
	{
		strings strField;
		if (strField.split(strBitset, link) > 0)
		{
			int from = atoi(strField[0].c_str()), to = from;
			if (strField.size() >= 2) to = atoi(strField[1].c_str());
			if (from < 0) from = 0;//�ݴ�
			if (to < 0 || to >= (int)this->size()) to = (int)this->size() - 1;//�ݴ�
			SetBit(from, to);
		}
	}
	/** @brief ���λ��Ϣ�� */
	void PrintStringNoDelimit(string &strBitset)const
	{
		for (size_t i = 0; i < this->size(); i++)
		{
			if (this->test(i)) strBitset.push_back('1');
			else strBitset.push_back('0');
		}
	}
	/** @brief ���λ��Ϣ��(�����ӷ�) */
	void PrintStringWithDelimit(string &strBitset, char link, char field)const
	{
		int from = -1, to = -1;
		for (size_t i = 0; i < this->size(); i++)
		{
			if (this->test(i))//��������λ
			{
				if (from == -1) from = (int)i;
				to = (int)i;
			}
			else if (from != -1)//λ��
			{
				bitset_PrintString(strBitset, from, to, link, field);
				from = -1, to = -1;
			}
		}
		if (from != -1 && to != -1)
            bitset_PrintString(strBitset, from, to, link, field);
	}

protected:
	size_t m_FromPos; ///< ��Ч��ʼλ��
	size_t m_ToPos; ///< ��Ч����λ��
};

/** @brief �ֽ���λ��ͳ�� */
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

/** @brief λ������
 *  @note
 *  ��STL bitset����:1,���Ͳ������ֽ���; 2,���Է���λ����; 3,���尴BYTE�洢
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

	/** @brief ȡ�ֽ��� */
	int GetByteCnt(void)const{return ByteCnt;}
	/** @brief ȡλ�� */
	int GetBitSize(void)const{return GetByteCnt()*8;}
	/** @brief ȡλ���� */
	const unsigned char *GetData(void)const{return m_data;}
	/** @brief ȡλ���� */
	unsigned char *GetData(void){return m_data;}

	/** @brief posλ��0 */
	void ClearBit(int pos){m_data[pos/8] &= (unsigned char)(~(1<<(pos%8)));}
	/** @brief ȫ��λ��0 */
	void ClearBit(void){memset(m_data, 0x00, sizeof(m_data));}
	/** @brief ����posλ�Ƿ�Ϊ1 */
	bool TestBit(int pos)const{return (m_data[pos/8] & (1<<(pos%8)))?true:false;}
	/** @brief ��pData��������λ */
	void SetBit(const unsigned char *pData, int Bytes){memcpy(m_data, pData, Bytes);}
	/** @brief posλ��1 */
	void SetBit(int pos){m_data[pos/8] |= (unsigned char)(1<<(pos%8));}
	/** @brief ȫ��λ��1 */
	void SetBit(void){memset(m_data, 0xFF, sizeof(m_data));}
	/** @brief ����from-to����Ϊ1 */
	void SetBit(int from, int to){for (; from <= to; from++) SetBit(from);}
	/** @brief ��λ����λ */
	void SetBit(const string &strBitset, char link)
	{
		if (link == 0) SetBitNoDelimit(strBitset);
		else SetBitWithDelimit(strBitset, link);
	}
	/** @brief ��תλ */
	void FlipBit(int pos){if (TestBit(pos)) ClearBit(pos); else SetBit(pos);}
	void FlipBit(void){for (int i = 0; i < ByteCnt; i++) m_data[i] = ~m_data[i];}

	/** @brief ��һ����Чλ*/
	int FirstBit(void)const{return _NextBit(0);}
	/** @brief ��һ����Чλ*/
	int NextBit(int prev)const{return _NextBit(prev+1);}

	/** @brief ����������
	 *  @param[in] strParam ��������
	 *  @param[in] delimit �ֶ��м�ָ�����'-'
	 *  @param[in] field �ֶηָ�����','
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
	/** @brief ��λ��Ϣ������� */
	void PrintString(string &strBitset, char link = '-', char field = ',')const
	{
		if (link == 0) PrintStringNoDelimit(strBitset);
		else PrintStringWithDelimit(strBitset, link, field);
	}
	/** @brief ͳ����1��λ�� */
	int CountBit(void)const
	{
		int AllBit = 0;
		for (int i = 0; i < ByteCnt; i++)
			AllBit += C_BITSET_ByteBitCnt[m_data[i]];
		return AllBit;
	}

protected:
	/** @brief Ѱ����Чλ*/
	int _NextBit(int prev)const
	{
		for (; prev < GetBitSize(); prev++)
		{
			if (TestBit(prev) == true) return prev;
		}
		return GetBitSize();
	}
	/** @brief ������Ϣ����λ */
	void SetBitNoDelimit(const string &strBitset)
	{
		for (size_t i = 0; i<strBitset.size() && i<ByteCnt*8; i++)
			if (strBitset[i] == '1') SetBit((int)i);
	}
	/** @brief ������Ϣ(�����ӷ�)����λ */
	void SetBitWithDelimit(const string &strBitset, char link)
	{
		strings strField;
		if (strField.split(strBitset, link) > 0)
		{
			int from = atoi(strField[0].c_str()), to = from;
			if (strField.size() >= 2) to = atoi(strField[1].c_str());
			if (from < 0) from = 0;//�ݴ�
			if (to < 0 || to >= ByteCnt*8) to = ByteCnt*8 - 1;//�ݴ�
			SetBit(from, to);
		}
	}
	/** @brief ���λ��Ϣ�� */
	void PrintStringNoDelimit(string &strBitset)const
	{
		for (int i = 0; i < ByteCnt*8; i++)
		{
			if (TestBit(i)) strBitset.push_back('1');
			else strBitset.push_back('0');
		}
	}
	/** @brief ���λ��Ϣ��(�����ӷ�) */
	void PrintStringWithDelimit(string &strBitset, char link, char field)const
	{
		int from = -1, to = -1;
		for (int i = 0; i < ByteCnt*8; i++)
		{
			if (TestBit(i))//��������λ
			{
				if (from == -1) from = i;
				to = i;
			}
			else if (from != -1)//λ��
			{
				bitset_PrintString(strBitset, from, to, link, field);
				from = -1, to = -1;
			}
		}
		if (from != -1 && to != -1)
			bitset_PrintString(strBitset, from, to, link, field);
	}
protected:
	unsigned char m_data[ByteCnt]; ///< λ����
};

#endif//__FND_BITSET_EX_H__

