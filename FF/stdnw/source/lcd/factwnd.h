/** @file
 *  @brief �����Զ������Դ��ڶ���
 *  @author 
 *  @date 2012/10/26
 *  @version 0.1
 */
#ifndef __LCD_FACTORY_WND_H__
#define __LCD_FACTORY_WND_H__
#include "uiwnd.h"
#include "testapi.h"

#define GM_AUTO_TEST	GM_USER
#define GM_MANUAL_TEST	(GM_USER+1)
#define GM_DISPLAY_TEST	(GM_USER+2)

//�����������˵�
class C_FactMainWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	int RebootTermWnd(S_MenuCtrl *pData);
	int GetFactoryTestState(C_FactAutoTest *pTestObj);
};

//�Զ����Դ���
class C_AutoTestWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	bool NeedAutoTestExec(void);

protected:
	S_TestNames m_AutoItems;
	string m_strBottomInfo;
};
class C_AutoExecWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	bool m_fPause;
	S_TestNames m_AutoItems;
};

//�ֶ����Դ���
class C_ManualTestWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	S_TestNames m_ManualItems;
	string m_strBottomInfo;
};

//���²���
class C_ResetTestWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//��������
class C_NameReportWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//���ɱ���
class C_MakeReportWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

#endif //__LCD_FACTORY_WND_H__
