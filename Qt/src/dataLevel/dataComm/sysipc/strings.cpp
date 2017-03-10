/** @file
 *  @brief 基础类strings定义
 *  @author  Steven Wan
 *  @date 2015/11/07
 *  @version 0.1
 */
#include "strings.h"

/** @brief 按指定间隔符分离字符串
 *  @param[in] strSrc 源字符串
 *  @param[in] delimit 间隔符
 *  @return 分开的字串个数
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

/** @brief 按指定间隔分离字符串
 *  @param[in] strSrc 源字符串
 *  @param[in] distance 间隔
 *  @return 分开的字串个数
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

/** @brief 字符串数组对齐
 *  @param[in] FillChar 填充字符
 *  @return 填充后的字符数组长度
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

/** @brief 测试串
 *  @return true任意一个串非空;false所有串为空
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

/** @brief 测试串
 *  @return true任意一个串空;false所有串为非空
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

/** @brief 统计串数组中有多少个非空串
 *  @return 非空串统计值
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

/** @brief 数据串行化
 *  @return 串行化后长度
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

/** @brief 数据串行化(从后面开始)
 *  @return 串行化后长度
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

/** @brief 附加后缀
 *  @param[in] strSuffix 后缀串
 *  @return 个数
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

/** @brief 插入前缀
 *  @param[in] strPrefix 前缀串
 *  @return 个数
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



