/** @file
 *  @brief ������strings����
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
 *  @brief string������
 */
class strings:public vector<string>
{
public:
	/** @brief ��ָ������������ַ���
	 *  @param[in] strSrc Դ�ַ���
	 *  @param[in] delimit �����
	 *  @return �ֿ����ִ�����
	 */
	int split(const string &strSrc, char delimit);

	/** @brief ��ָ����������ַ���
	 *  @param[in] strSrc Դ�ַ���
	 *  @param[in] distance ���
	 *  @return �ֿ����ִ�����
	 */
	int split(const string &strSrc, size_t Distance);

	/** @brief �ַ����������
	 *  @param[in] FillChar ����ַ�
	 *  @return ������ַ����鳤��
	 */
	int Trim(char FillChar);

	/** @brief ׷�����ɸ��ִ�
	 *  @param[in] Cnt:��׷�ӵ��ִ�����
	 *  @param[in] strData:��׷�ӵ�����
	 *  @return strings&
	 */
	strings & PushBack(S_WORD Cnt, const string &strData)
	{
		for (S_WORD i = 0; i < Cnt; i++)
			push_back(strData);
		return *this;
	}

	/** @brief ׷��һ���ִ�����
	 *  @param[in] strsAppend:��׷�ӵ��ִ�����
	 *  @return strings&
	 */
	strings & append(const strings& strsAppend)
	{		
		insert(end(), strsAppend.begin(), strsAppend.end());
		return *this;
	}
	/** @brief ���Դ�
	 *  @return true����һ�����ǿ�;false���д�Ϊ��
	 */
	bool Any(void)const;
	/** @brief ���Դ�
	 *  @return true����һ������;false���д�Ϊ�ǿ�
	 */
	bool AnyEmpty(void)const;
	/** @brief ͳ�ƴ��������ж��ٸ��ǿմ�
	 *  @return �ǿմ�ͳ��ֵ
	 */
	S_WORD Count(void)const;
	/** @brief ���ݴ��л�
	 *  @return ���л��󳤶�
	 */
	S_WORD Serialize(string &strData)const;
	/** @brief ���ݴ��л�(�Ӻ��濪ʼ)
	 *  @return ���л��󳤶�
	 */
	S_WORD ReverseSerialize(string &strData)const;

	/** @brief ���Ӻ�׺
	 *  @param[in] strSuffix ��׺��
	 *  @return ����
	 */
	int AppendBack(const string &strSuffix);
	/** @brief ����ǰ׺
	 *  @param[in] strPrefix ǰ׺��
	 *  @return ����
	 */
	int InsertFront(const string &strPrefix);
};


#endif//__FND_FNDVERSION_H__

