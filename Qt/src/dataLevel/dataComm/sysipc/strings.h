/** @file
 *  @brief 基础类strings定义
 *  @author  Steven Wan
 *  @date 2015/11/07
 *  @version 0.1
 */
#ifndef __FND_STRINGS_H__
#define __FND_STRINGS_H__
#include <cstring>
#include <iostream>
#include "../inc/basedef.h"

/** @class
 *  @brief string串数组
 */
class strings:public vector<string>
{
public:
	/** @brief 按指定间隔符分离字符串
	 *  @param[in] strSrc 源字符串
	 *  @param[in] delimit 间隔符
	 *  @return 分开的字串个数
	 */
	int split(const string &strSrc, char delimit);

	/** @brief 按指定间隔分离字符串
	 *  @param[in] strSrc 源字符串
	 *  @param[in] distance 间隔
	 *  @return 分开的字串个数
	 */
	int split(const string &strSrc, size_t Distance);

	/** @brief 字符串数组对齐
	 *  @param[in] FillChar 填充字符
	 *  @return 填充后的字符数组长度
	 */
	int Trim(char FillChar);

	/** @brief 追加若干个字串
	 *  @param[in] Cnt:待追加的字串数组
	 *  @param[in] strData:待追加的数据
	 *  @return strings&
	 */
	strings & PushBack(S_WORD Cnt, const string &strData)
	{
		for (S_WORD i = 0; i < Cnt; i++)
			push_back(strData);
		return *this;
	}

	/** @brief 追加一个字串数组
	 *  @param[in] strsAppend:待追加的字串数组
	 *  @return strings&
	 */
	strings & append(const strings& strsAppend)
	{		
		insert(end(), strsAppend.begin(), strsAppend.end());
		return *this;
	}
	/** @brief 测试串
	 *  @return true任意一个串非空;false所有串为空
	 */
	bool Any(void)const;
	/** @brief 测试串
	 *  @return true任意一个串空;false所有串为非空
	 */
	bool AnyEmpty(void)const;
	/** @brief 统计串数组中有多少个非空串
	 *  @return 非空串统计值
	 */
	S_WORD Count(void)const;
	/** @brief 数据串行化
	 *  @return 串行化后长度
	 */
	S_WORD Serialize(string &strData)const;
	/** @brief 数据串行化(从后面开始)
	 *  @return 串行化后长度
	 */
	S_WORD ReverseSerialize(string &strData)const;

	/** @brief 附加后缀
	 *  @param[in] strSuffix 后缀串
	 *  @return 个数
	 */
	int AppendBack(const string &strSuffix);
	/** @brief 插入前缀
	 *  @param[in] strPrefix 前缀串
	 *  @return 个数
	 */
	int InsertFront(const string &strPrefix);
};


#endif//__FND_FNDVERSION_H__

