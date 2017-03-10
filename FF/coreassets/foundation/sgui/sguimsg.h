//---------------------------------------------------------------------------------
// Copyright  2009-2010
// E-mail:   @163.com
// Project:			SGUI(Simple GUI)
// File:			sguimsg.h
// Description:		SGUI for Power user elect energy acquire system;unsupport multi_thread
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2010-01-18  Create                  Kingsea
//---------------------------------------------------------------------------------
#ifndef __SGUI_MSG_H__
#define __SGUI_MSG_H__
#include "basedef.h"

enum
{
	GM_UNKNOW,
	GM_KEYDN,
	GM_KEYUP,
	GM_CHAR,
	GM_CREATE,
	GM_RESTORE,
	GM_PAINT,
	GM_SYSPAINT,
	GM_TIME,
	GM_CMD,
	GM_DESTROY,
	GM_QUIT,

	GM_SETFOCUS,
	GM_KILLFOCUS,
	GM_AUTOBACK,//return 0 enable;return !0 disable

	GM_SYSKEY,
	GM_SYSINFO,

	/** follow user msg
	 */
	GM_USER = 100,

	GM_MAX
};

enum
{
	GVK_KEYBEGIN, // begin
	GVK_UP,
	GVK_DN,
	GVK_LEFT,
	GVK_RIGHT,
	GVK_CANCEL,
	GVK_OK,
	GVK_PGUP,
	GVK_PGDN,
	GVK_KEYEND, // end

	GVK_SYSKEYBEGIN, // begin
	GVK_PROGRAMKEY,
	GVK_SYSKEYEND, // end

	GVK_KEYMAX
};

#define GVK_ENTER	0x0D
#define GVK_ESC		0x1B
#define GVK_BACKSPACE		0x08
#define GVK_TAB		0x09
#define GVK_F1		0x70

#define GM_TIME_INTERVAL_MS		1000

class C_WNDBASE;
struct S_GUIMSG
{
	C_WNDBASE *pWnd;
	S_WORD  Msg;
	S_WORD  wParam;
	S_DWORD lParam;

	S_GUIMSG(void){pWnd=NULL;Msg=GM_UNKNOW;wParam=0;lParam=0;}
};


#endif //__SGUI_OBJ_H__

