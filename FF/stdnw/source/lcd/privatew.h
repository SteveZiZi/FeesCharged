/** @file
 *  @brief 私有窗口定义
 *  @author 
 *  @date 2012/12/16
 *  @version 0.1
 */
#ifndef __LCD_PRIVATE_WND_H__
#define __LCD_PRIVATE_WND_H__
#include "uiwnd.h"
#include "strings.h"
#include "traceset.h"
#include "datafile.h"
#include "basetype.h"
#include "inizone.h"

//私有菜单窗口
class C_PrivateMenuWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	int ClearUserConfig(void);
	int EnterAdjustMeterState(void);
	int OnPressOK(S_MenuScrollCtrl *pData);
};

//现场调试菜单窗口
class C_FieldDebugMenuWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	int OnPressOK(S_MenuScrollCtrl *pData);
};

// 查询今日成功表计
class C_MeterSuccessWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	int GetMeterSuccessInfo(void);
	int GetMeterZXYGInfo(void);
	int GetMeterFXYGInfo(void);
	
protected:
	int m_MeterMax;
	int m_MeterCnt;
	strings m_strsMeter;
};

// 查询昨日失败表计
class C_MeterFailedWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	int GetMeterFailedInfo(void);
	
protected:
	int m_MeterMax;
	int m_MeterCnt;
	strings m_strsMeter;
};

// 本地模块信息
class C_ModuleInfoWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//终端管理菜单窗口
class C_ManMenuWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	int OnPressOK(S_MenuScrollCtrl *pData);
};

// 查询系统版本信息
class C_ListVersionWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	int GetSysVersionInfo(void);
	
protected:
	strings m_strsVersion;
};

// 远程模块信息查询
class C_ModemInfoWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//USB转网络设置窗口
class C_UsbNetSetWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	bool CreateNumEdit(int num, S_RECT &NumRect, S_DWORD ID);
	bool CheckParamFormat(S_DWORD &LocalIP, S_DWORD &NetMask, S_DWORD &GateWay);

protected:
	char m_UsbIP[32];
};

//MAC地址显示窗口
class C_MacViewWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	char m_Eth0Mac[64];
	char m_Eth1Mac[64];
};

//MAC地址设置窗口
class C_MacSetWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	bool CreateNumEdit(S_BYTE num, S_RECT &NumRect, S_DWORD ID);
	bool CheckParamFormat(string &strEth0Mac, string &strEth1Mac);
};

enum{WND_LIST_SET_ZB, WND_LIST_SET_APN};
//列表设置窗口
class C_ListSetWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	int GetListData(S_DWORD DataType);
	int SetListData(int idxListData);
	int GetListData_ZB(void);
	int GetListData_APN(void);
	int SetListData_ZB(int idxListData);
	int SetListData_APN(int idxListData);

protected:
	string m_BottomInfo;
	strings m_strListData;
	S_DWORD m_DataType;
};

//日志跟踪参数设置
class C_TraceParamSetWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	bool CreateSelectBox(const char *pBuffer, S_DWORD Idx, S_RECT &NumRect, S_DWORD ID);
	int SaveTraceInfo(const char *pProcess, const char *pLevel, const char *pWay);
	int LoadTraceInfo(const char *pProcess, S_TraceSet &TraceInfo);
	int AllProcessName(strings &strsProcessName);
};

// 	区域配置
class C_ZoneCfgWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	int GetZoneListInfo(string &strCurrentZone, S_ZoneCfgs &ZoneList);

protected:
	bool m_CurrentModeType; // true 为缺省模式；false为区域模式
	bool m_ExpectedModeType; // true 为缺省模式；false为区域模式
	string m_strCurrentZone;
	S_ZoneCfgs m_listZone;
};

#endif //__LCD_PRIVATE_WND_H__


