/** @file
 *  @brief 本地操作与显示功能定义
 *  @author 
 *  @date 2011/11/30
 *  @version 0.1
 */
#include "lcd.h"
#include "sysipc.h"
#include "htrace.h"
#include "sguiapp.h"
#include "sguimsg.h"
#include "sguidc.h"
#include "cfg_usrp.h"
#include "timeop.h"
#include "menuwnd.h"
#include "status.h"
#include "context.h"
#include "sysctrl.h"

C_UIMAPP  theUIMApp;

#define SYS_FLUSH_TIMEOUT	2
#define BOTTOM_INFO_FLUSH_INTERVAL_S	2
#define DESKTOP_TURNVIEW_TIMEOUT_S		90

C_UIMAPP::C_UIMAPP(void)
{
	m_CloseLight = 60;
	m_BackDesktop = 40;
	m_SysFlush   = SYS_FLUSH_TIMEOUT;
}

C_UIMAPP::~C_UIMAPP()
{
}

int C_UIMAPP::TranslateMsg(S_GUIMSG &msg)
{
	S_LcdStatus *pLcdStatus = CONTEXT.GetLcdStatus();
	S_LcdInfo *pLcdInfo = STATUS.GetLcdInfo();
	switch (msg.Msg)
	{
	case GM_SYSKEY:
		if (msg.wParam == GVK_PROGRAMKEY)
		{
			logit<<"编程键按下检测成功"<<endl;
			pLcdStatus->m_ProgramKeyDelayS = LCD_PROGRAM_KEY_DELAY_S;
		}
		break;
	case GM_KEYDN:
	case GM_KEYUP:
		m_CloseLight = pLcdInfo->m_LCDLightTimeoutS;
		m_BackDesktop = pLcdInfo->m_BackDesktopTimeoutS;
		OpenBacklight();
		break;
	case GM_TIME:
		if (pLcdStatus->m_ProgramKeyDelayS > 0)
		{
			pLcdStatus->m_ProgramKeyDelayS--;
		}
		if (m_CloseLight > 0)
		{
			m_CloseLight--;
		}
		else if (m_CloseLight == 0)
		{
			CloseBacklight();
			m_CloseLight = pLcdInfo->m_LCDLightTimeoutS;
		}
		if (m_BackDesktop > 0)
		{
			m_BackDesktop--;
		}
		else if (m_BackDesktop == 0)
		{
			if (EmptyWndStack() == 0)
				msg.pWnd = NULL, msg.Msg = GM_UNKNOW;
			m_BackDesktop = pLcdInfo->m_BackDesktopTimeoutS;
		}
		if (--m_SysFlush <= 0)
		{
			S_GUIMSG aMsg;
			aMsg.Msg = GM_SYSPAINT;
			m_MsgQ.push(aMsg);
			m_SysFlush = SYS_FLUSH_TIMEOUT;
			/** 心跳 */
			C_SYSCTRL::ProcessHeartBeat();
		}
		if (pLcdStatus->m_UpdateDlog == true)
		{
			S_GUIMSG aMsg;
			aMsg.Msg = GM_SYSINFO;
			aMsg.wParam = 1; // 升级消息框
			m_MsgQ.push(aMsg);
			pLcdStatus->m_UpdateDlog = false;
		}
		break;
	default:
		break;
	}
	return C_SGUIAPP::TranslateMsg(msg);
}

const char *WeekName[] =
{
	"星期日","星期一","星期二","星期三","星期四","星期五","星期六"
};
int C_DESKTOP::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	switch (msg)
	{
	case GM_CREATE:
		break;
	case GM_PAINT:{
		int size;
		S_DATE date;
		char strBuf[64];
		S_LcdInfo *pLcdInfo = STATUS.GetLcdInfo();
		C_SGUIDC *pdc = BeginPaint();

		C_TIME::LocalTime(C_TIME::TimeNow(), date);
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		size = pdc->GetStringExtent(pLcdInfo->m_TermName);
		pdc->DrawString((160-size)/2, 30, pLcdInfo->m_TermName);
		sprintf(strBuf, "%d-%02d-%02d", date.m_Year, date.m_Mon, date.m_Day);
		size = pdc->GetStringExtent(strBuf);
		pdc->DrawString((160-size)/2, 60, strBuf);
		size = pdc->GetStringExtent(WeekName[date.m_WeekDay]);
		pdc->DrawString((160-size)/2, 80, WeekName[date.m_WeekDay]);
		pdc->DrawHLine(0, 122, 160);
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F12x12));
		if (m_fPppInfo == true) pdc->DrawString(20, 126, GetPppBottomInfo());
		else pdc->DrawString(20, 126, GetDbcjBottomInfo());
		EndPaint(pdc);
		}break;
	case GM_TIME:
		m_DesktopTimeoutS += 1;
		if (m_DesktopTimeoutS % BOTTOM_INFO_FLUSH_INTERVAL_S == 0)
		{
			m_fPppInfo = !m_fPppInfo;
			InvalidateRect(NULL);
		}
		if (m_DesktopTimeoutS >= DESKTOP_TURNVIEW_TIMEOUT_S)
		{
			//m_DesktopTimeoutS = 0;
			//GOTOWND(C_TURNVIEW, 0);
		}
		break;
	case GM_KEYUP:
		//m_DesktopTimeoutS = 0;
		GOTOWND(C_MENUWND, 0);
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

const char *C_DESKTOP::GetDbcjBottomInfo(void)
{
	return CONTEXT.GetLcdBottomInfo(MP_PROCESS_DBCJ1);
}

const char *C_DESKTOP::GetPppBottomInfo(void)
{
	return CONTEXT.GetLcdBottomInfo(MP_PROCESS_PPP);
}

//
//
//
int C_LcdServer::BeforeService(void)
{
	if (theUIMApp.SysInit(CFG_PATH_RESOURCE) != 0)
		return -1;

	m_pDesktop = new C_DESKTOP;
	m_pLcdInfo = STATUS.GetLcdInfo();

	C_SGUIAPP::SetCurApp(&theUIMApp);
	theUIMApp.SetDesktopWnd(m_pDesktop);
	theUIMApp.CloseBacklight();
	theUIMApp.SetCloseLightDelay(m_pLcdInfo->m_LCDLightTimeoutS);
	theUIMApp.SetBackDesktopDelay(m_pLcdInfo->m_BackDesktopTimeoutS);
	theUIMApp.SetLcdContrast(m_pLcdInfo->m_LCDContrast);
	theUIMApp.SetLcdBrightness(m_pLcdInfo->m_LCDBrightness);
	theUIMApp.SetLcdGrayness(m_pLcdInfo->m_LCDGreyScale);
	theUIMApp.SetLcdDirection(m_pLcdInfo->m_LCDDirection);

	return 0;
}

int C_LcdServer::AfterService(void)
{
	delete m_pDesktop;
	return 0;
}

int C_LcdServer::OnTickService(void)
{
	S_GUIMSG aMsg;
	while (1)
	{
		aMsg = theUIMApp.GetGuiMsg();
		theUIMApp.TranslateMsg(aMsg);
		theUIMApp.DispatchGuiMsg(aMsg);
	}
	return 0;
}


