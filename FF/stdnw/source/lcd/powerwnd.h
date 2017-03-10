/** @file
 *  @brief 能量显示窗口定义
 *  @author  yezhengqiu
 *  @date 2012/02/21
 *  @version 0.1
 */
#ifndef __LCD_POWER_WND_H__
#define __LCD_POWER_WND_H__
#include "uiwnd.h"
#include "strings.h"
#include "timeop.h"
#include "Htrace.h"
#include "sysbase.h"
#include "status.h"
#include "datafile.h"

//enum for data type displayed
enum DataTypeForLCD
{
	ZXYGPWRIND = 0,   //正向有功电能示值
	ZXWGPWRIND = 1,   //正向无功电能示值
	FXYGPWRIND,		  //反向有功电能示值
	FXWGPWRIND,       //反向无功电能示值
	YXXWGPWRIND,	  //一象限无功电能示值
	EXXWGPWRIND,      //二象限无功电能示值
	SXXWGPWRIND,      //三象限无功电能示值
	FXXWGPWRIND,      //四象限无功电能示值
	VOLTAGE,          //电压
	CURRENT,          //电流
	ACTPWR,           //有功功率
	WATTLESSPWR,      //无功功率
	PWRFACTOR,        //功率因素
	ZXYGXL,           //正向有功需量
	FXYGXL,           //反向有功需量

	MAXLCDDATATYPE       //this is always the last item.	
};

struct S_PowerInfo
{
	S_DWORD m_BackWndTimeoutS;
	S_DWORD m_RecvCmdTimeoutS;

	DataTypeForLCD m_type;
	int m_InfoIndex;

	char m_PowerInfo1[32];
	char m_PowerInfo2[32];
	char m_PowerInfo3[32];
	char m_PowerInfo4[32];
};

const S_BYTE DEFAULTVAL = 0xEE;
const S_BYTE DEFAULTVALFF = 0xFF;

//可实时抄读各测量点正向有功总电量
class C_PowerZXYGWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	bool SendCmd2Dbcj(S_WORD mp, DataTypeForLCD type);
	bool RecvCmd4Dbcj(S_PowerInfo &PowerInfo);
	bool ClearPowerInfo(S_PowerInfo &PowerInfo);
	bool ParsePowerInfo(const string &strInfo, S_PowerInfo &PowerInfo);
};

//测量点数据显示窗口
class C_MeasPntDataDisplayWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	bool ClearPowerInfo(S_PowerInfo &PowerInfo);
	bool SetFnValue(DataTypeForLCD type);
	bool CreateNumEdit(int num, S_RECT &NumRect, S_DWORD ID);
	bool CheckDateFormat(S_DATE &date);
	bool readPwrData(S_PowerInfo &PowerInfo, S_TIME timeFreeze);
	bool ParseData(const string &strData, S_PowerInfo &PowerInfo);

protected:
	S_DWORD FnValue;
	S_WORD measurePoint;
	S_TIME timeFreeze;
};

//日冻结电量
class C_HistoryDayWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	bool GetDate(S_DATE &date);
	bool HandleDayData(S_WORD MP, S_DWORD FN, S_TIME DataTime, char *pBuffer);	
};

//月冻结电量
class C_HistoryMonWnd:public C_HistoryDayWnd
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	bool HandleMonData(S_WORD MP, S_DWORD FN, S_TIME DataTime, char *pBuffer);	
};


//轮显
/*
class C_TURNVIEW:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};
*/
#endif//__LCD_POWER_WND_H__

