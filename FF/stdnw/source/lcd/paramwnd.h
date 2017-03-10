/** @file
 *  @brief 参数显示窗口定义
 *  @author  yezhengqiu
 *  @date 2012/02/21
 *  @version 0.1
 */
#ifndef __LCD_PARAMWND_H__
#define __LCD_PARAMWND_H__
#include "uiwnd.h"



//通信通道设置
class C_CommunMenuWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

struct S_WndMeter
{
	char m_Status[8];
	char m_Function[12];
	char m_Address[16];
	char m_Rule[16];
	char m_Port[8];
	char m_Relay[16];
};
//测量点参数
class C_MeterParamViewWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	bool UpdateMeterInfo(S_WndMeter *pData, const S_MeterInfo *pMeterInfo);	
};
class C_MeterDetailViewWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	S_WORD PrintMeterParamDetail(S_WORD MP);

protected:
	char m_BottomInfo[64];
	strings m_strListData;
};
//终端时间设置
class C_TerminalTimeViewWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//界面密码设置
class C_PasswordWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//终端编号设置
class C_RtuaViewWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//级联参数查询、设置
class C_ForwardParamViewWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//终端限值参数查询与设置
class C_ThresholdViewWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

// 调试菜单
class C_DebugMenuWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//终端测试通道设置
class C_TestComViewWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	S_DWORD m_ChannelType;
};

//终端测试通道设置
class C_TestComSetWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
private:
	void SaveInfos(S_DWORD type);
};

// 本地通信模块模式
class C_DnModuleViewWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

// 本地通信模块模式
class C_DnModuleSetWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//通道显示窗口
class C_ChannelViewWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	/** @brief 取通道信息
	 *  @return 0 以太网; 2 GPRS/CDMA
	 */
	int GetHttxChannelInfo(int ChannelIdx, S_DWORD &IP, S_WORD &port);
};

//PPP参数显示窗口
class C_PppViewWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//本地网络设置窗口
class C_LocalNetViewWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//通讯参数显示 如心跳间隔等参数
class C_ChannelsParamViewWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//通讯参数设置 如心跳间隔等参数
class C_ChannelsParamSetWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//通道设置窗口
class C_ChannelSetWnd:public C_ChannelViewWnd
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	bool CreateNumEdit(int num, S_RECT &NumRect, S_DWORD ID);
	bool CheckParamFormat(int &ChannelType, S_DWORD &IP, S_DWORD &port);
	bool SaveHttxChannelInfo(int ChannelIdx, int ChannelType, S_DWORD IP, S_WORD port);
};

//本地网络设置窗口
class C_LocalNetSetWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	bool CreateNumEdit(int num, S_RECT &NumRect, S_DWORD ID);
	bool CheckParamFormat(S_DWORD &LocalIP, S_DWORD &NetMask, S_DWORD &GateWay);
};

struct S_MeterSet
{
	S_WORD m_MP;
	S_MpInfo m_MpInfo;
	S_MeterInfo m_MeterInfo;
};

//电表参数设置
class C_MeterParamSetWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	bool CreateNumEdit(const char *pBuffer, S_DWORD style, S_RECT &NumRect, S_DWORD ID);
	bool CreateSelectBox(const char *pBuffer, S_DWORD Idx, S_RECT &NumRect, S_DWORD ID);
	bool CheckParamFormat(S_MeterInfo &MeterInfo, S_MpInfo &MpInfo);
	S_DWORD GetSelectBoxIdx(const char *pBuffer, S_DWORD ID);
};
class C_MeterDetailSetWnd:public C_MeterParamSetWnd
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	S_DWORD GetBaudNO(S_DWORD baud);
	S_DWORD GetBaud(S_DWORD idx);
	bool CheckDetailFormat(S_MeterInfo &MeterInfo, S_MpInfo &MpInfo);	
};

//终端时间设置窗口
class C_TermTimeSetWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	bool CreateNumEdit(int num, S_RECT &NumRect, S_DWORD ID);
	bool CheckTimeFormat(S_DATE &date);
};

//终端地址设置窗口
class C_RtuaSetWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//文本编辑窗口
class C_TextEditWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	int SavePppInfo(int ItemIndex, string &strInfo);
	int GetPppInfo(int ItemIndex, string &strInfo, int EditSize);
};


#endif//__PARAMWND_H__

