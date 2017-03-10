/** @file
 *  @brief 本地操作与显示功能定义
 *  @author 
 *  @date 2011/11/30
 *  @version 0.1
 */
#ifndef __LCD_SERVER_H__
#define __LCD_SERVER_H__
#include "server.h"
#include "sguiapp.h"
#include "sguibase.h"
#include "uiwnd.h"
#include "sysbase.h"

class C_UIMAPP:public C_SGUIAPP
{
public:
	C_UIMAPP(void);
	virtual ~C_UIMAPP();

	virtual int TranslateMsg(S_GUIMSG &msg);

	void OpenBacklight(void){SGUI_backlight(1);}
	void CloseBacklight(void){SGUI_backlight(0);}
	void SetCloseLightDelay(int DelayS){m_CloseLight = DelayS;}
	void SetBackDesktopDelay(int DelayS){m_BackDesktop = DelayS;}
	void SetLcdContrast(int contrast){SGUI_contrast(contrast);}
	void SetLcdBrightness(int brightness){SGUI_brightness(brightness);}
	void SetLcdGrayness(int grayness){SGUI_grayness(grayness);}
	void SetLcdDirection(int direction){SGUI_direction(direction);}

protected:
	int m_CloseLight;
	int m_BackDesktop;
	int m_SysFlush;
};

class C_DESKTOP:public C_UIMWND
{
public:
	C_DESKTOP(void){m_DesktopTimeoutS = 0, m_fPppInfo = true;}

	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	const char *GetDbcjBottomInfo(void);
	const char *GetPppBottomInfo(void);

protected:
	S_DWORD m_DesktopTimeoutS;
	bool m_fPppInfo;
};


class C_LcdServer:public C_SERVER
{
public:
	C_LcdServer(void){m_pDesktop = NULL, m_pLcdInfo = NULL;}
	virtual ~C_LcdServer(){}

	virtual int BeforeService(void);
	virtual int AfterService(void);
	virtual int OnTickService(void);

protected:
	C_DESKTOP *m_pDesktop;
	S_LcdInfo *m_pLcdInfo;
};

#endif//__LCD_SERVER_H__

