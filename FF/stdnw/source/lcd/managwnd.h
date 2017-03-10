/** @file
 *  @brief 终端管理与维护窗口定义
 *  @author  yezhengqiu
 *  @date 2012/02/21
 *  @version 0.1
 */
#ifndef __LCD_MANAGE_WND_H__
#define __LCD_MANAGE_WND_H__
#include "uiwnd.h"
#include "strings.h"

//终端版本	可显示终端软、硬件版本等
class C_ShowVersionWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//重启终端	重新启动终端
class C_RebootWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//告警查询	可提供最新至少100条告警信息查询（包括告警代码、中文显示事件内容、发生时间等）
class C_QueryAlarmWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//查询抄表统计信息
class C_QueryGatherInfo:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

class C_PrintAlarmWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
#ifdef TEST_DEBUG
	//添加测试用函数
	int TestMakeAlarmData(S_WORD MP, S_DWORD DI, strings &strReocrds,bool TypeFlag=false);
#endif
protected:
	int PrintAlarm(int Index);
	int ReadAlarmData(S_DWORD type);

protected:
	strings m_strReocrds;
	strings m_strTexts;
};
#ifdef TEST_DEBUG
//添加测试相关处理类暂时放在这里
//测试告警造数据
#define FILL_INVALID_DATA			0XFF
class C_TestMakeData
{
public:
	C_TestMakeData(){};
	~C_TestMakeData();
	//组数据
	static inline void MakeData(string &strData,const S_DWORD ENdata,const S_BYTE DataLen){strData.append((char*)&ENdata,DataLen);}
	//3相电压
	static inline void Make3Vol(string &strData)
	{
		MakeData(strData,0x12345678,2);//A相电压
		MakeData(strData,0x12345678,2);//B相电压
		MakeData(strData,0x12345678,2);//C相电压
		MakeData(strData,0x12345678,2);//AB线电压
		MakeData(strData,0x12345678,2);//BC线电压
		MakeData(strData,0x12345678,2);//CA线电压
		MakeData(strData,0x12345678,2);//A/AB相基波电压
		MakeData(strData,0x12345678,2);//B相基波电压
		MakeData(strData,0x12345678,2);//C/CA相基波电压
	}
	//3相电流
	static inline void Make3Cur(string &strData)
	{
		MakeData(strData,0x12345678,3);//A相电流
		MakeData(strData,0x12345678,3);//B相电流
		MakeData(strData,0x12345678,3);//C相电流
		MakeData(strData,0x12345678,3);//A相基波电流
		MakeData(strData,0x12345678,3);//B相基波电流
		MakeData(strData,0x12345678,3);//C相基波电流
	}
	//功率
	static inline void MakePower(string &strData,const S_BYTE DataLen)
	{
		MakeData(strData,0x12345678,DataLen);//总
		MakeData(strData,0x12345678,DataLen);//A
		MakeData(strData,0x12345678,DataLen);//B
		MakeData(strData,0x12345678,DataLen);//C
	}
	//TypeFlag:true填具体数据false:填0xff
	static void FillTimeData(string &strData,int type);
	static int FillARD1Data(string &strData,bool TypeFlag=false);
	static int FillARD2Data(string &strData,bool TypeFlag=false);
	static int FillARD3Data(string &strData,bool TypeFlag=false);
	static int FillARD4Data(string &strData,bool TypeFlag=false);
	static int FillARD5Data(string &strData,bool TypeFlag=false);
	static int FillARD6Data(string &strData,bool TypeFlag=false);
	static int FillARD7Data(string &strData,bool TypeFlag=false);
	static int FillARD8Data(string &strData,bool TypeFlag=false);
	static int FillARD9Data(string &strData,bool TypeFlag=false);
	static int FillARD1018Data(string &strData,bool TypeFlag=false);
	static int FillARD11Data(string &strData,bool TypeFlag=false);
	static int FillARD12Data(string &strData,bool TypeFlag=false);
	static int FillARD13Data(string &strData,bool TypeFlag=false);
	static int FillARD14Data(string &strData,bool TypeFlag=false);
	static int FillARD151617Data(string &strData,bool TypeFlag=false);
	static int FillARD19Data(string &strData,bool TypeFlag=false);
	static int FillARD20Data(string &strData,bool TypeFlag=false);
	static int FillARD21Data(string &strData,bool TypeFlag=false);
	static int FillARD22Data(string &strData,bool TypeFlag=false);
};

//添加测试相关处理暂时放在这里
#endif
//液晶调节	可调节终端液晶显示对比度、背光亮度等
class C_ScreenParamViewWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//信号强度和电池电量	可查询正比于无线信号场强的指示和当前电池电量（终端电池指备用停电上报电池）
class C_SignalAndBatteryWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//无线实时状态	可提供终端连接无线网络的实时状态信息
class C_WirelessWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	const char *ModemType(int type);
	const char *GetPppBottomInfo(void);
};

//液晶调节	可调节终端液晶显示对比度、背光亮度等
class C_ScreenParamSetWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};
	
#endif //__LCD_MANAGE_WND_H__


