/** @file
 *  @brief 本地操作菜单窗口定义
 *  @author 
 *  @date 2012/01/09
 *  @version 0.1
 */
#ifndef __LCD_MENUWND_H__
#define __LCD_MENUWND_H__
#include "uiwnd.h"

#define FACT_KEY_SEQ_MAX 4
class C_MENUWND:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
//	int InitMainMenu(void);
//	int GetCurrentMenuIdx(int CurrentMenuID);
	int SetFactoryKeySequence(S_WORD key);
	bool CheckFactoryTest(void);

protected:
	S_WORD m_FactKey[FACT_KEY_SEQ_MAX];
};

//
//以下为demo，不须要请注释
//
#if 0
class C_CTRLWND:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

class C_VKWND:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

class C_MSGBOXWND:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

class C_MSGSENDWND:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

class C_FONTWND:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};
#endif

#endif//__LCD_MENUWND_H__


