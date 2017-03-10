/** @file
 *  @brief 告警中文化
 *  @author 
 *  @date 2012/03/30
 *  @version 0.1
 *  告警代码、中文显示事件内容、发生时间
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
    **函 数 名: GetValt
    **作    者: 刘志强
    **日    期: 2013-9-12 16:33:18
    **功    能: 从用户给出的strRecord字符串中，将BCD码格式的电压值转换为液晶显示格式的电压值。
    **-----------------------------------------------------------------------------------------------------
    **输入参数: strRecord   从数据文件中读取到的记录字符串
    **          strTexts    用于存放要输出到液晶的字符串集合
    **返 回 值: int类型.    <=0     失败
    **                      非零    成功
    **-----------------------------------------------------------------------------------------------------
    **修改记录: 
    **
    **-----------------------------------------------------------------------------------------------------
    **备    注: 
    **
    **-----------------------------------------------------------------------------------------------------
    **********************************************************************************************************************************************/
	int GetValt(const string &strRecord, strings &strTexts);
	/************************************************************************************************************************************************
    **函 数 名: GetCurrent
    **作    者: 刘志强
    **日    期: 2013-9-12 16:33:18
    **功    能: 从用户给出的strRecord字符串中，将BCD码格式的电流值转换为液晶显示格式的电流值。
    **-----------------------------------------------------------------------------------------------------
    **输入参数: strRecord   从数据文件中读取到的记录字符串
    **          strTexts    用于存放要输出到液晶的字符串集合
    **返 回 值: int类型.    <=0     失败
    **                      非零    成功
    **-----------------------------------------------------------------------------------------------------
    **修改记录: 
    **
    **-----------------------------------------------------------------------------------------------------
    **备    注: 
    **
    **-----------------------------------------------------------------------------------------------------
    **********************************************************************************************************************************************/
	int GetCurrent(const string &strRecord, strings &strTexts);

	/************************************************************************************************************************************************
    **函 数 名: GetActivePower
    **作    者: 刘志强
    **日    期: 2013-9-12 16:33:18
    **功    能: 从用户给出的strRecord字符串中，将BCD码格式的有功功率值或总有功功率转换为可在液晶显示的值。
    **-----------------------------------------------------------------------------------------------------
    **输入参数: strRecord   从数据文件中读取到的记录字符串
    **          strTexts    用于存放要输出到液晶的字符串集合
    **          PQ          true    处理有功功率    false 处理无功功率
    **返 回 值: int类型.    <=0     失败
    **                      非零    成功
    **-----------------------------------------------------------------------------------------------------
    **修改记录: 
    **
    **-----------------------------------------------------------------------------------------------------
    **备    注: 
    **
    **-----------------------------------------------------------------------------------------------------
    **********************************************************************************************************************************************/
	int GetActivePower(const string &strRecord, strings &strTexts,S_BOOL PQ);

/************************************************************************************************************************************************
**函 数 名: GetPF
**作    者: 刘志强
**日    期: 2013-9-12 16:33:18
**功    能: 从用户给出的strRecord字符串中，将BCD码格式的功率因素转换为可在液晶显示的值。
**-----------------------------------------------------------------------------------------------------
**输入参数: strRecord   从数据文件中读取到的记录字符串
**          strTexts    用于存放要输出到液晶的字符串集合
**返 回 值: int类型.    <=0     失败
**                      非零    成功
**-----------------------------------------------------------------------------------------------------
**修改记录: 
**
**-----------------------------------------------------------------------------------------------------
**备    注: 
**
**-----------------------------------------------------------------------------------------------------
**********************************************************************************************************************************************/
int GetPF(const string &strRecord, strings &strTexts);


/************************************************************************************************************************************************
**函 数 名: GetPowerVal
**作    者: 刘志强
**日    期: 2013-9-12 16:33:18
**功    能: 从用户给出的strRecord字符串中，将BCD码格式的正向有功总电能/反向有功总电能/组合无功1总电能/组合无功2总电能转换为可在液晶显示的值。
**-----------------------------------------------------------------------------------------------------
**输入参数: strRecord   从数据文件中读取到的记录字符串 长度16
**          strTexts    用于存放要输出到液晶的字符串集合
**返 回 值: int类型.    <=0     失败
**                      非零    成功
**-----------------------------------------------------------------------------------------------------
**修改记录: 
**
**-----------------------------------------------------------------------------------------------------
**备    注: 
**
**-----------------------------------------------------------------------------------------------------
**********************************************************************************************************************************************/
int GetPowerVal(const string &strRecord, strings &strTexts);

/************************************************************************************************************************************************
**函 数 名: GetCTPT
**作    者: 刘志强
**日    期: 2013-9-12 16:33:18
**功    能: 从用户给出的strRecord字符串中，将BCD码格式的CT/PT转换为可在液晶显示的值。
**-----------------------------------------------------------------------------------------------------
**输入参数: strRecord   从数据文件中读取到的记录字符串 长度4
**          strTexts    用于存放要输出到液晶的字符串集合
**返 回 值: int类型.    <=0     失败
**                      非零    成功
**-----------------------------------------------------------------------------------------------------
**修改记录: 
**
**-----------------------------------------------------------------------------------------------------
**备    注: 
**
**-----------------------------------------------------------------------------------------------------
**********************************************************************************************************************************************/
int GetCTPT(const string &strRecord, strings &strTexts);

/************************************************************************************************************************************************
**函 数 名: GetMeterConSet
**作    者: 刘志强
**日    期: 2013-9-12 16:33:18
**功    能: 从用户给出的strRecord字符串中，将BCD码格式的电表有功无功常数转换为可在液晶显示的值。
**-----------------------------------------------------------------------------------------------------
**输入参数: strRecord   从数据文件中读取到的记录字符串 长度6
**          strTexts    用于存放要输出到液晶的字符串集合
**返 回 值: int类型.    <=0     失败
**                      非零    成功
**-----------------------------------------------------------------------------------------------------
**修改记录: 
**
**-----------------------------------------------------------------------------------------------------
**备    注: 
**
**-----------------------------------------------------------------------------------------------------
**********************************************************************************************************************************************/
int GetMeterConSet(const string &strRecord, strings &strTexts);
/************************************************************************************************************************************************
**函 数 名: 
**作    者: zhgx
**日    期: 2013-9-12 16:33:18
**功    能: 
**-----------------------------------------------------------------------------------------------------
**输入参数: strRecord   从数据文件中读取到的记录字符串 长度6
**          strTexts    用于存放要输出到液晶的字符串集合
**返 回 值: int类型.    <=0     失败
**                      非零    成功
**-----------------------------------------------------------------------------------------------------
**修改记录: 
**
**-----------------------------------------------------------------------------------------------------
**备    注: 
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
**函 数 名: GetSubValue
**作    者: 刘志强
**日    期: 2013-9-13 11:10:55
**功    能: 用于获取报警内容的数据
**-----------------------------------------------------------------------------------------------------
**输入参数: strRecord - const string&引用类型.
**          strTexts - strings&引用类型.
**返 回 值: int类型.
**-----------------------------------------------------------------------------------------------------
**修改记录: 
**
**-----------------------------------------------------------------------------------------------------
**备    注: 
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


