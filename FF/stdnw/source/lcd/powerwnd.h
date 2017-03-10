/** @file
 *  @brief ������ʾ���ڶ���
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
	ZXYGPWRIND = 0,   //�����й�����ʾֵ
	ZXWGPWRIND = 1,   //�����޹�����ʾֵ
	FXYGPWRIND,		  //�����й�����ʾֵ
	FXWGPWRIND,       //�����޹�����ʾֵ
	YXXWGPWRIND,	  //һ�����޹�����ʾֵ
	EXXWGPWRIND,      //�������޹�����ʾֵ
	SXXWGPWRIND,      //�������޹�����ʾֵ
	FXXWGPWRIND,      //�������޹�����ʾֵ
	VOLTAGE,          //��ѹ
	CURRENT,          //����
	ACTPWR,           //�й�����
	WATTLESSPWR,      //�޹�����
	PWRFACTOR,        //��������
	ZXYGXL,           //�����й�����
	FXYGXL,           //�����й�����

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

//��ʵʱ�����������������й��ܵ���
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

//������������ʾ����
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

//�ն������
class C_HistoryDayWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	bool GetDate(S_DATE &date);
	bool HandleDayData(S_WORD MP, S_DWORD FN, S_TIME DataTime, char *pBuffer);	
};

//�¶������
class C_HistoryMonWnd:public C_HistoryDayWnd
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	bool HandleMonData(S_WORD MP, S_DWORD FN, S_TIME DataTime, char *pBuffer);	
};


//����
/*
class C_TURNVIEW:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};
*/
#endif//__LCD_POWER_WND_H__

