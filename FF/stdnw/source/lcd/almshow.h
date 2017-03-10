/** @file
 *  @brief �澯���Ļ�
 *  @author 
 *  @date 2012/03/30
 *  @version 0.1
 *  �澯���롢������ʾ�¼����ݡ�����ʱ��
 */
#ifndef __ALARM_SHOW_H__
#define __ALARM_SHOW_H__
#include "basedef.h"
#include "strings.h"

#include "Busialgo.h"

//
//
//
class C_AlmShow
{
public:
	C_AlmShow(void){}
	virtual ~C_AlmShow(){}

public:
	virtual int Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts);

protected:
	int AlarmGeneralPrint(const string &strRecord, strings &strTexts);
    /************************************************************************************************************************************************
    **�� �� ��: GetValt
    **��    ��: ��־ǿ
    **��    ��: 2013-9-12 16:33:18
    **��    ��: ���û�������strRecord�ַ����У���BCD���ʽ�ĵ�ѹֵת��ΪҺ����ʾ��ʽ�ĵ�ѹֵ��
    **-----------------------------------------------------------------------------------------------------
    **�������: strRecord   �������ļ��ж�ȡ���ļ�¼�ַ���
    **          strTexts    ���ڴ��Ҫ�����Һ�����ַ�������
    **�� �� ֵ: int����.    <=0     ʧ��
    **                      ����    �ɹ�
    **-----------------------------------------------------------------------------------------------------
    **�޸ļ�¼: 
    **
    **-----------------------------------------------------------------------------------------------------
    **��    ע: 
    **
    **-----------------------------------------------------------------------------------------------------
    **********************************************************************************************************************************************/
	int GetValt(const string &strRecord, strings &strTexts);
	/************************************************************************************************************************************************
    **�� �� ��: GetCurrent
    **��    ��: ��־ǿ
    **��    ��: 2013-9-12 16:33:18
    **��    ��: ���û�������strRecord�ַ����У���BCD���ʽ�ĵ���ֵת��ΪҺ����ʾ��ʽ�ĵ���ֵ��
    **-----------------------------------------------------------------------------------------------------
    **�������: strRecord   �������ļ��ж�ȡ���ļ�¼�ַ���
    **          strTexts    ���ڴ��Ҫ�����Һ�����ַ�������
    **�� �� ֵ: int����.    <=0     ʧ��
    **                      ����    �ɹ�
    **-----------------------------------------------------------------------------------------------------
    **�޸ļ�¼: 
    **
    **-----------------------------------------------------------------------------------------------------
    **��    ע: 
    **
    **-----------------------------------------------------------------------------------------------------
    **********************************************************************************************************************************************/
	int GetCurrent(const string &strRecord, strings &strTexts);

	/************************************************************************************************************************************************
    **�� �� ��: GetActivePower
    **��    ��: ��־ǿ
    **��    ��: 2013-9-12 16:33:18
    **��    ��: ���û�������strRecord�ַ����У���BCD���ʽ���й�����ֵ�����й�����ת��Ϊ����Һ����ʾ��ֵ��
    **-----------------------------------------------------------------------------------------------------
    **�������: strRecord   �������ļ��ж�ȡ���ļ�¼�ַ���
    **          strTexts    ���ڴ��Ҫ�����Һ�����ַ�������
    **          PQ          true    �����й�����    false �����޹�����
    **�� �� ֵ: int����.    <=0     ʧ��
    **                      ����    �ɹ�
    **-----------------------------------------------------------------------------------------------------
    **�޸ļ�¼: 
    **
    **-----------------------------------------------------------------------------------------------------
    **��    ע: 
    **
    **-----------------------------------------------------------------------------------------------------
    **********************************************************************************************************************************************/
	int GetActivePower(const string &strRecord, strings &strTexts,S_BOOL PQ);

/************************************************************************************************************************************************
**�� �� ��: GetPF
**��    ��: ��־ǿ
**��    ��: 2013-9-12 16:33:18
**��    ��: ���û�������strRecord�ַ����У���BCD���ʽ�Ĺ�������ת��Ϊ����Һ����ʾ��ֵ��
**-----------------------------------------------------------------------------------------------------
**�������: strRecord   �������ļ��ж�ȡ���ļ�¼�ַ���
**          strTexts    ���ڴ��Ҫ�����Һ�����ַ�������
**�� �� ֵ: int����.    <=0     ʧ��
**                      ����    �ɹ�
**-----------------------------------------------------------------------------------------------------
**�޸ļ�¼: 
**
**-----------------------------------------------------------------------------------------------------
**��    ע: 
**
**-----------------------------------------------------------------------------------------------------
**********************************************************************************************************************************************/
int GetPF(const string &strRecord, strings &strTexts);


/************************************************************************************************************************************************
**�� �� ��: GetPowerVal
**��    ��: ��־ǿ
**��    ��: 2013-9-12 16:33:18
**��    ��: ���û�������strRecord�ַ����У���BCD���ʽ�������й��ܵ���/�����й��ܵ���/����޹�1�ܵ���/����޹�2�ܵ���ת��Ϊ����Һ����ʾ��ֵ��
**-----------------------------------------------------------------------------------------------------
**�������: strRecord   �������ļ��ж�ȡ���ļ�¼�ַ��� ����16
**          strTexts    ���ڴ��Ҫ�����Һ�����ַ�������
**�� �� ֵ: int����.    <=0     ʧ��
**                      ����    �ɹ�
**-----------------------------------------------------------------------------------------------------
**�޸ļ�¼: 
**
**-----------------------------------------------------------------------------------------------------
**��    ע: 
**
**-----------------------------------------------------------------------------------------------------
**********************************************************************************************************************************************/
int GetPowerVal(const string &strRecord, strings &strTexts);

/************************************************************************************************************************************************
**�� �� ��: GetCTPT
**��    ��: ��־ǿ
**��    ��: 2013-9-12 16:33:18
**��    ��: ���û�������strRecord�ַ����У���BCD���ʽ��CT/PTת��Ϊ����Һ����ʾ��ֵ��
**-----------------------------------------------------------------------------------------------------
**�������: strRecord   �������ļ��ж�ȡ���ļ�¼�ַ��� ����4
**          strTexts    ���ڴ��Ҫ�����Һ�����ַ�������
**�� �� ֵ: int����.    <=0     ʧ��
**                      ����    �ɹ�
**-----------------------------------------------------------------------------------------------------
**�޸ļ�¼: 
**
**-----------------------------------------------------------------------------------------------------
**��    ע: 
**
**-----------------------------------------------------------------------------------------------------
**********************************************************************************************************************************************/
int GetCTPT(const string &strRecord, strings &strTexts);

/************************************************************************************************************************************************
**�� �� ��: GetMeterConSet
**��    ��: ��־ǿ
**��    ��: 2013-9-12 16:33:18
**��    ��: ���û�������strRecord�ַ����У���BCD���ʽ�ĵ���й��޹�����ת��Ϊ����Һ����ʾ��ֵ��
**-----------------------------------------------------------------------------------------------------
**�������: strRecord   �������ļ��ж�ȡ���ļ�¼�ַ��� ����6
**          strTexts    ���ڴ��Ҫ�����Һ�����ַ�������
**�� �� ֵ: int����.    <=0     ʧ��
**                      ����    �ɹ�
**-----------------------------------------------------------------------------------------------------
**�޸ļ�¼: 
**
**-----------------------------------------------------------------------------------------------------
**��    ע: 
**
**-----------------------------------------------------------------------------------------------------
**********************************************************************************************************************************************/
int GetMeterConSet(const string &strRecord, strings &strTexts);
/************************************************************************************************************************************************
**�� �� ��: 
**��    ��: zhgx
**��    ��: 2013-9-12 16:33:18
**��    ��: 
**-----------------------------------------------------------------------------------------------------
**�������: strRecord   �������ļ��ж�ȡ���ļ�¼�ַ��� ����6
**          strTexts    ���ڴ��Ҫ�����Һ�����ַ�������
**�� �� ֵ: int����.    <=0     ʧ��
**                      ����    �ɹ�
**-----------------------------------------------------------------------------------------------------
**�޸ļ�¼: 
**
**-----------------------------------------------------------------------------------------------------
**��    ע: 
**
**-----------------------------------------------------------------------------------------------------
**********************************************************************************************************************************************/
string GetDwordData(const string &strRecord);

};


//
//
//
class C_AlmShow_ARD01: public C_AlmShow
{
public:
	virtual int Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts);
};
class C_AlmShow_ARD02: public C_AlmShow
{
public:
	virtual int Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts);
};
class C_AlmShow_ARD03: public C_AlmShow
{
public:
	virtual int Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts);
};
class C_AlmShow_ARD04: public C_AlmShow
{
public:
	virtual int Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts);
};
class C_AlmShow_ARD05: public C_AlmShow
{
public:
	virtual int Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts);
};
class C_AlmShow_ARD06: public C_AlmShow
{
public:
	virtual int Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts);
};
class C_AlmShow_ARD07: public C_AlmShow
{
public:
	virtual int Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts);
};
class C_AlmShow_ARD08: public C_AlmShow
{
public:
	virtual int Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts);
};
class C_AlmShow_ARD09: public C_AlmShow
{
public:
	virtual int Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts);
};
class C_AlmShow_ARD10: public C_AlmShow
{
public:
	virtual int Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts);
};
class C_AlmShow_ARD11: public C_AlmShow
{
public:
	virtual int Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts);
private:
/************************************************************************************************************************************************
**�� �� ��: GetSubValue
**��    ��: ��־ǿ
**��    ��: 2013-9-13 11:10:55
**��    ��: ���ڻ�ȡ�������ݵ�����
**-----------------------------------------------------------------------------------------------------
**�������: strRecord - const string&��������.
**          strTexts - strings&��������.
**�� �� ֵ: int����.
**-----------------------------------------------------------------------------------------------------
**�޸ļ�¼: 
**
**-----------------------------------------------------------------------------------------------------
**��    ע: 
**
**-----------------------------------------------------------------------------------------------------
**********************************************************************************************************************************************/
	int GetSubValue(const string &strRecord, strings &strTexts);
};
class C_AlmShow_ARD12: public C_AlmShow
{
public:
	virtual int Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts);
};
class C_AlmShow_ARD13: public C_AlmShow
{
public:
	virtual int Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts);
};
class C_AlmShow_ARD14: public C_AlmShow
{
public:
	virtual int Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts);
};
class C_AlmShow_ARD15: public C_AlmShow
{
public:
	virtual int Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts);
};
class C_AlmShow_ARD16: public C_AlmShow
{
public:
	virtual int Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts);
};
class C_AlmShow_ARD17: public C_AlmShow
{
public:
	virtual int Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts);
};
class C_AlmShow_ARD18: public C_AlmShow
{
public:
	virtual int Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts);
};
class C_AlmShow_ARD19: public C_AlmShow
{
public:
	virtual int Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts);
};
class C_AlmShow_ARD20: public C_AlmShow
{
public:
	virtual int Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts);
};
class C_AlmShow_ARD21: public C_AlmShow
{
public:
	virtual int Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts);
};
class C_AlmShow_ARD22: public C_AlmShow
{
public:
	virtual int Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts);
};



class C_AlmShow_ERD01: public C_AlmShow
{
public:
	virtual int Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts);
};
class C_AlmShow_ERD02: public C_AlmShow
{
public:
	virtual int Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts);
};
class C_AlmShow_ERD03: public C_AlmShow
{
public:
	virtual int Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts);
};
class C_AlmShow_ERD04: public C_AlmShow
{
public:
	virtual int Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts);
};
class C_AlmShow_ERD05: public C_AlmShow
{
public:
	virtual int Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts);
};
class C_AlmShow_ERD06: public C_AlmShow
{
public:
	virtual int Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts);
};

class C_AlmShow_ERD07: public C_AlmShow
{
public:
	virtual int Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts);
};
class C_AlmShow_ERD08: public C_AlmShow
{
public:
	virtual int Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts);
};
class C_AlmShow_ERD09: public C_AlmShow
{
public:
	virtual int Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts);
};

//
//
//
class C_AlmShowFact
{
public:
	static C_AlmShow *GetAlmShowObj(S_DWORD DI);
};

#endif //__ALARM_SHOW_H__


