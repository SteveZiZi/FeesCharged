/**
 *  @file
 *  @brief �ֳ��������������
 *  @author 
 *  @date 2012-05-10
 *  @version v1.0
 *  @note �ֳ����߽������ݺ����
 */
#ifndef __BASEB_CMD_FILTER_H__
#define __BASEB_CMD_FILTER_H__
#include "basedef.h"
#include "basetype.h"

/** @brief ������� */
class C_CmdFilter
{
public:
	C_CmdFilter(const string &strFilterName);
	~C_CmdFilter();

public:
	/** @brief �������
	 *  @param[in] strBuffer �����
	 *  @param[out] strCmd ֡�������
	 *  @return <0: ����[����ֵ�ǳ�����ֽ���]; =0: ��֡; >0: �ɹ�
	 */
	int operator()(const string &strBuffer, string &strCmd)const;

private:
	string m_strFilterName; //�������������
};

/** @brief ��������� */
class C_CmdVisitor
{
public:
	C_CmdVisitor(void);
	~C_CmdVisitor();

public:
	/** @brief ���Լ����
	 *  @param[in] strBuffer �����
	 *  @param[in] CommReg ͨ�üĴ���
	 *  @param[out] strCmd ֡�������
	 *  @return <=0: ����; >0: �ɹ�
	 */
	int operator()(const S_ReportData &ReportData, const S_REGISTER &CommReg, string &strCmd)const;
};

#endif //__BASEB_CMD_FILTER_H__

