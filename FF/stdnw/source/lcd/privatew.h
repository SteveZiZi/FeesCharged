/** @file
 *  @brief ˽�д��ڶ���
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

//˽�в˵�����
class C_PrivateMenuWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	int ClearUserConfig(void);
	int EnterAdjustMeterState(void);
	int OnPressOK(S_MenuScrollCtrl *pData);
};

//�ֳ����Բ˵�����
class C_FieldDebugMenuWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	int OnPressOK(S_MenuScrollCtrl *pData);
};

// ��ѯ���ճɹ����
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

// ��ѯ����ʧ�ܱ��
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

// ����ģ����Ϣ
class C_ModuleInfoWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//�ն˹���˵�����
class C_ManMenuWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	int OnPressOK(S_MenuScrollCtrl *pData);
};

// ��ѯϵͳ�汾��Ϣ
class C_ListVersionWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	int GetSysVersionInfo(void);
	
protected:
	strings m_strsVersion;
};

// Զ��ģ����Ϣ��ѯ
class C_ModemInfoWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//USBת�������ô���
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

//MAC��ַ��ʾ����
class C_MacViewWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	char m_Eth0Mac[64];
	char m_Eth1Mac[64];
};

//MAC��ַ���ô���
class C_MacSetWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	bool CreateNumEdit(S_BYTE num, S_RECT &NumRect, S_DWORD ID);
	bool CheckParamFormat(string &strEth0Mac, string &strEth1Mac);
};

enum{WND_LIST_SET_ZB, WND_LIST_SET_APN};
//�б����ô���
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

//��־���ٲ�������
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

// 	��������
class C_ZoneCfgWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	int GetZoneListInfo(string &strCurrentZone, S_ZoneCfgs &ZoneList);

protected:
	bool m_CurrentModeType; // true Ϊȱʡģʽ��falseΪ����ģʽ
	bool m_ExpectedModeType; // true Ϊȱʡģʽ��falseΪ����ģʽ
	string m_strCurrentZone;
	S_ZoneCfgs m_listZone;
};

#endif //__LCD_PRIVATE_WND_H__


