/** @file
 *  @brief ������strings����
 *  @author  Steven Wan
 *  @date 2015/11/07
 *  @version 0.1
 */
#include "strings.h"

/** @brief ��ָ������������ַ���
 *  @param[in] strSrc Դ�ַ���
 *  @param[in] delimit �����
 *  @return �ֿ����ִ�����
 */
int strings::split(const string &strSrc, char delimit)
{
	string::const_iterator iterFirst = strSrc.begin();
	while (iterFirst != strSrc.end())
	{
		string::const_iterator iter = find(iterFirst, strSrc.end(), delimit);
		string strTmp(iterFirst, iter);
		push_back(strTmp);
		if (iter == strSrc.end())
			break;
		iterFirst = iter + 1;
	}
	return (int)size();
}

/** @brief ��ָ����������ַ���
 *  @param[in] strSrc Դ�ַ���
 *  @param[in] distance ���
 *  @return �ֿ����ִ�����
 */
int strings::split(const string &strSrc, size_t Distance)
{
	if (Distance == 0x00) return (int)size();

	string::const_iterator iter = strSrc.begin();
	while (distance(iter, strSrc.end()) >= (int)Distance)
	{
		string strTmp(iter, iter + Distance);
		advance(iter, Distance);
		push_back(strTmp);
	}
	if (distance(iter, strSrc.end()) > 0)
	{
		string strTmp(iter, strSrc.end());
		push_back(strTmp);
	}
	return (int)size();
}

/** @brief �ַ����������
 *  @param[in] FillChar ����ַ�
 *  @return ������ַ����鳤��
 */
int strings::Trim(char FillChar)
{
	if (empty() == true)
		return 0;

	size_t MaxLen = 0;
	for (strings::const_iterator iter = begin(); iter < end(); ++iter)
	{
		if (iter->size() > MaxLen)
			MaxLen = iter->size();
	}
	if (MaxLen == 0)
		return 0;

	for (strings::iterator iter = begin(); iter < end(); ++iter)
	{
		if (iter->size() < MaxLen)
			iter->append(MaxLen - iter->size(), (char)FillChar);
	}

	return (int)MaxLen;
}

/** @brief ���Դ�
 *  @return true����һ�����ǿ�;false���д�Ϊ��
 */
bool strings::Any(void)const
{
	if (empty() == true)
		return false;

	for (strings::const_iterator iter = begin(); iter < end(); ++iter)
	{
		if (!iter->empty())
			return true;
	}
	return false;
}

/** @brief ���Դ�
 *  @return true����һ������;false���д�Ϊ�ǿ�
 */
bool strings::AnyEmpty(void)const
{
	if (empty() == true)
		return true;

	for (strings::const_iterator iter = begin(); iter < end(); ++iter)
	{
		if (iter->empty())
			return true;
	}
	return false;
}

/** @brief ͳ�ƴ��������ж��ٸ��ǿմ�
 *  @return �ǿմ�ͳ��ֵ
 */
S_WORD strings::Count(void)const
{
	if (empty() == true)
		return 0x00;

	S_WORD Cnt = 0x00;
	for (strings::const_iterator iter = begin(); iter < end(); ++iter)
	{
		if (iter->empty() == false)
			Cnt += 1;
	}
	return Cnt;
}

/** @brief ���ݴ��л�
 *  @return ���л��󳤶�
 */
S_WORD strings::Serialize(string &strData)const
{
	if (empty() == true)
		return 0x00;

	for (strings::const_iterator iter = begin(); iter < end(); ++iter)
	{
		strData.append(*iter);
	}
	return (S_WORD)strData.size();
}

/** @brief ���ݴ��л�(�Ӻ��濪ʼ)
 *  @return ���л��󳤶�
 */
S_WORD strings::ReverseSerialize(string &strData)const
{
	if (empty() == true)
		return 0x00;

	for (strings::const_reverse_iterator iter = rbegin(); iter != rend(); ++iter)
	{
		strData.append(*iter);
	}
	return (S_WORD)strData.size();
}

/** @brief ���Ӻ�׺
 *  @param[in] strSuffix ��׺��
 *  @return ����
 */
int strings::AppendBack(const string &strSuffix)
{
	if (empty() == true || strSuffix.empty() == true)
		return 0;

	for (strings::iterator iter = begin(); iter < end(); ++iter)
	{
		if (iter->empty() == false)
			iter->append(strSuffix);
	}
	return (int)size();
}

/** @brief ����ǰ׺
 *  @param[in] strPrefix ǰ׺��
 *  @return ����
 */
int strings::InsertFront(const string &strPrefix)
{
	if (empty() == true || strPrefix.empty() == true)
		return 0;

	for (strings::iterator iter = begin(); iter < end(); ++iter)
	{
		if (iter->empty() == false)
			iter->insert(0, strPrefix);
	}
	return (int)size();
}



