/** @file
 *  @brief 私有窗口定义
 *  @author 
 *  @date 2012/12/16
 *  @version 0.1
 */
#include "privatew.h"
#include "sguiapp.h"
#include "sysctrl.h"
#include "osrelate.h"
#include "sguictrl.h"
#include "status.h"
#include "connect.h"
#include "cfg_path.h"
#include "context.h"
#include "initrace.h"
#include "modem.h"
#include "almshow.h"
#include "busialgo.h"
#include "cfg_mp.h"
#include "basefn.h"
#include "fndver.h"
#include "mpver.h"
#include "basebver.h"
#include "nwbver.h"
#include "zonebase.h"

// 菜单往后面加
enum
{
	PRIVATE_MENU_RESET,
	PRIVATE_MENU_EXIT,

	PRIVATE_MENU_ZONE_CONFIG,
	PRIVATE_MENU_METER,
	PRIVATE_MENU_CLEAR_USER_DIR,
	PRIVATE_MENU_SWITCH_TERM,
	PRIVATE_MENU_DEBUG,
	PRIVATE_MENU_MAN,
	

	PRIVATE_MENU_MAX
};
static const char *g_PrivateMenu[] = 
{
	"系统复位",
	"退出窗口",
// 菜单往后面加
	"区域配置",
	"进入校表状态",
	"清除用户目录",
	"变换终端类型",
	"现场调试功能",
	"终端管理功能",
};
static const char *g_PrivateMenuWndInfo[] = {"            ", "系统即将复位", "设置校表成功"};
//私有菜单窗口
int C_PrivateMenuWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	S_MenuScrollCtrl *pData = static_cast<S_MenuScrollCtrl *>(GetWndData(sizeof(S_MenuScrollCtrl)));
	switch (msg)
	{
	case GM_CREATE:
		pData->m_InfoIndex = 0;
		pData->m_LineOfScreen = 8;
		pData->m_SelectM = 0;
		pData->m_StartM = 0;
		pData->m_MenuCnt = (int)KGE_MEMBER_OF(g_PrivateMenu);
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		for (int i = 0; i < pData->m_LineOfScreen; i++)
		{
			if (i + pData->m_StartM < pData->m_MenuCnt)
			{
				sprintf(buffer, "%d.%s", i + pData->m_StartM + 1, g_PrivateMenu[i + pData->m_StartM]);
				pdc->DrawString(16, i*14+6, buffer);
			}
		}
		S_RECT rect = {12, pData->m_SelectM*14+5, 138, 14};
		pdc->ReverseRect(rect);
		pdc->DrawHLine(0, 122, 160);
		pdc->DrawString(20, 126, g_PrivateMenuWndInfo[pData->m_InfoIndex]);
		EndPaint(pdc);
		}break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
		{
			RETURNWND();
		}
		else if (wParam == GVK_RIGHT || wParam == GVK_LEFT || wParam == GVK_UP || wParam == GVK_DN)
		{
			if (pData->ScrollCtrl(wParam) == true)
				InvalidateRect(NULL);
		}
		else if (wParam == GVK_OK)
		{
			OnPressOK(pData);
		}
		break;
 	case GM_DESTROY:
		ReleaseWndData();
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

int C_PrivateMenuWnd::ClearUserConfig(void)
{
	return C_SYSCTRL::UserConfigClear();
}

int C_PrivateMenuWnd::EnterAdjustMeterState(void)
{
	S_HttxStatus *pHttxStatus = CONTEXT.GetHttxStatus(MP_HTTX_3);
	S_HttxInfo *pHttxInfo = STATUS.GetHttxInfo(MP_HTTX_3);
	S_JlcyInfo *pJlcyInfo = STATUS.GetJlcyInfo();

	C_SYSCTRL::EnableProcess(MP_PROCESS_DBCJ1, false);
	C_SYSCTRL::EnableProcess(MP_PROCESS_DBCJ2, false);
	strcpy(pHttxInfo->m_ChannlName[HTTX_CHANNEL_1], pJlcyInfo->m_ChannlName[JLCY_CHANNL_ADJUST_METER]);
	pHttxStatus->m_fReconnect = true;
	return 0;
}

int C_PrivateMenuWnd::OnPressOK(S_MenuScrollCtrl *pData)
{
	switch (pData->m_StartM+pData->m_SelectM)
	{
	case PRIVATE_MENU_RESET:
		if (SGui_MsgBox("提示", "    确认重启终端?", MB_BTN_OKCANCEL) != MB_RTN_YESOK)
			return -1;
		C_SYSCTRL::HardwareReset();
		pData->m_InfoIndex = 1;
		break;
	case PRIVATE_MENU_EXIT:
		if (SGui_MsgBox("提示", "    确认退出窗口?", MB_BTN_OKCANCEL) != MB_RTN_YESOK)
			return -1;
		SleepSecond(1);
		RETURNWND();
		break;
	case PRIVATE_MENU_CLEAR_USER_DIR:
		if (SGui_MsgBox("提示", " 确认清除用户目录?", MB_BTN_OKCANCEL) != MB_RTN_YESOK)
			return -1;
		ClearUserConfig();
		C_SYSCTRL::HardwareReset();
		pData->m_InfoIndex = 1;
		break;
	case PRIVATE_MENU_SWITCH_TERM:break;
	case PRIVATE_MENU_METER:
		if (SGui_MsgBox("提示", " 确认开始校表?", MB_BTN_OKCANCEL) != MB_RTN_YESOK)
			return -1;
		EnterAdjustMeterState();
		pData->m_InfoIndex = 2;
		break;
	case PRIVATE_MENU_ZONE_CONFIG:GOTOWND(C_ZoneCfgWnd, 0x00);break;
	case PRIVATE_MENU_DEBUG:GOTOWND(C_FieldDebugMenuWnd, 0x00);break;
	case PRIVATE_MENU_MAN:GOTOWND(C_ManMenuWnd, 0x00);break;
	default:break;
	}
	return 0;
}

//现场调试菜单窗口
enum
{
	DEBUG_MENU_SET_TRACE,
	DEBUG_MENU_APN,
	DEBUG_MENU_USB2NET,
	DEBUG_METER_SUCCESS,
	DEBUG_METER_FAILED,
	DEBUG_MENU_MAC,

	DEBUG_MENU_MAX
};
static const char *g_FieldDebugMenu[] = 
{
// 菜单往后面加
	"设置日志参数",
	"选择通用APN",
	"USB转网口",
	"查询今日成功表计",
	"查询昨日失败表计",
	"设备MAC地址",
};
static const char *g_FieldDebugMenuWndInfo[] = {"            ", "系统即将复位", "设置校表成功"};
int C_FieldDebugMenuWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	S_MenuScrollCtrl *pData = static_cast<S_MenuScrollCtrl *>(GetWndData(sizeof(S_MenuScrollCtrl)));
	switch (msg)
	{
	case GM_CREATE:
		pData->m_InfoIndex = 0;
		pData->m_LineOfScreen = 8;
		pData->m_SelectM = 0;
		pData->m_StartM = 0;
		pData->m_MenuCnt = (int)KGE_MEMBER_OF(g_FieldDebugMenu);
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		for (int i = 0; i < pData->m_LineOfScreen; i++)
		{
			if (i + pData->m_StartM < pData->m_MenuCnt)
			{
				sprintf(buffer, "%d.%s", i + pData->m_StartM + 1, g_FieldDebugMenu[i + pData->m_StartM]);
				pdc->DrawString(16, i*14+6, buffer);
			}
		}
		S_RECT rect = {12, pData->m_SelectM*14+5, 138, 14};
		pdc->ReverseRect(rect);
		pdc->DrawHLine(0, 122, 160);
		pdc->DrawString(20, 126, g_FieldDebugMenuWndInfo[pData->m_InfoIndex]);
		EndPaint(pdc);
		}break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
		{
			RETURNWND();
		}
		else if (wParam == GVK_RIGHT || wParam == GVK_LEFT || wParam == GVK_UP || wParam == GVK_DN)
		{
			if (pData->ScrollCtrl(wParam) == true)
				InvalidateRect(NULL);
		}
		else if (wParam == GVK_OK)
		{
			OnPressOK(pData);
		}
		break;
 	case GM_DESTROY:
		ReleaseWndData();
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

int C_FieldDebugMenuWnd::OnPressOK(S_MenuScrollCtrl *pData)
{
	switch (pData->m_StartM+pData->m_SelectM)
	{
	case DEBUG_MENU_USB2NET:GOTOWND(C_UsbNetSetWnd, 0x00);break;
	case DEBUG_MENU_MAC:GOTOWND(C_MacViewWnd, 0x00);break;
	case DEBUG_MENU_SET_TRACE:GOTOWND(C_TraceParamSetWnd, 0x00);break;
	case DEBUG_MENU_APN:GOTOWND(C_ListSetWnd, WND_LIST_SET_APN);break;
	case DEBUG_METER_SUCCESS:GOTOWND(C_MeterSuccessWnd, 0x00);break;
	case DEBUG_METER_FAILED:GOTOWND(C_MeterFailedWnd, 0x00);break;
	default:break;
	}
	return 0;
}

// 查询今日成功表计
int C_MeterSuccessWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	S_MenuScrollCtrl *pData = static_cast<S_MenuScrollCtrl *>(GetWndData(sizeof(S_MenuScrollCtrl)));
	switch (msg)
	{
	case GM_CREATE:{
		m_MeterCnt = -1;
		S_ALLMP *pALLMP = STATUS.GetALLMP();
		m_MeterMax = (int)pALLMP->m_MTBS.m_AllMPBS.count();

		pData->m_InfoIndex = 0;
		pData->m_LineOfScreen = 8;
		pData->m_SelectM = 0;
		pData->m_StartM = 0;
		pData->m_MenuCnt = 1;
		PostWndMsg(GM_USER, 0x00, 0x00);
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		if (m_MeterCnt >= 0)
		{
			for (int i = 0; i < pData->m_LineOfScreen; i++)
			{
				if (i + pData->m_StartM < pData->m_MenuCnt)
					pdc->DrawString(6, i*14+6, m_strsMeter[i + pData->m_StartM].c_str());
			}
		}
		else
		{
			pdc->DrawString(16, 0*14+6, "请稍候......");
		}
		S_RECT rect = {4, pData->m_SelectM*14+5, 152, 14};
		pdc->ReverseRect(rect);
		pdc->DrawHLine(0, 122, 160);
		if (m_MeterCnt < 0) sprintf(buffer, "表计总数:%d", m_MeterMax);
		else sprintf(buffer, "表计总数:%d; 成功:%d", m_MeterMax, m_MeterCnt);
		pdc->DrawString(20, 126, buffer);
		EndPaint(pdc);
		}break;
	case GM_USER:
		m_MeterCnt = GetMeterSuccessInfo();
		pData->m_MenuCnt = (int)m_strsMeter.size();
		InvalidateRect(NULL);
		break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
		{
			RETURNWND();
		}
		else if (wParam == GVK_RIGHT || wParam == GVK_LEFT || wParam == GVK_UP || wParam == GVK_DN)
		{
			if (pData->ScrollCtrl(wParam) == true)
				InvalidateRect(NULL);
		}
		else if (wParam == GVK_OK)
		{
		}
		break;
 	case GM_DESTROY:
		ReleaseWndData();
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

int C_MeterSuccessWnd::GetMeterSuccessInfo(void)
{
	if (GetMeterZXYGInfo() <= 0)
		GetMeterFXYGInfo();
	
	if (m_strsMeter.empty() == true)
	{
		m_strsMeter.push_back("没有成功表计!!!");
		return 0; // 返回成功表计数
	}

	return (int)m_strsMeter.size();// 返回成功表计数
}

int C_MeterSuccessWnd::GetMeterZXYGInfo(void)
{
	S_ALLMP *pALLMP = STATUS.GetALLMP();
	S_TIME DataTime = C_TIME::TimeNow();

	for (size_t mp = pALLMP->m_MTBS.m_AllMPBS.begin(); mp < pALLMP->m_MTBS.m_AllMPBS.end(); mp++)
	{
		if (pALLMP->m_MTBS.m_AllMPBS.test(mp) == false)
			continue;

		string strData;
		if (C_Class2Data::Read(0x05060100, (S_WORD)mp, strData, DataTime) < 0)
			continue;

		char Buffer[64];
		string strInfo = C_BusiAlgo::ChangeBcd2Ascii((S_BYTE *)strData.data(), 6, 2, false);
		sprintf(Buffer, "%04d    %s kWh", mp, strInfo.c_str());
		m_strsMeter.push_back(Buffer);
	}
	
	return (int)m_strsMeter.size();// 返回成功表计数
}

int C_MeterSuccessWnd::GetMeterFXYGInfo(void)
{
	S_ALLMP *pALLMP = STATUS.GetALLMP();
	S_TIME DataTime = C_TIME::TimeNow();

	for (size_t mp = pALLMP->m_MTBS.m_AllMPBS.begin(); mp < pALLMP->m_MTBS.m_AllMPBS.end(); mp++)
	{
		if (pALLMP->m_MTBS.m_AllMPBS.test(mp) == false)
			continue;

		string strData;
		if (C_Class2Data::Read(0x05060300, (S_WORD)mp, strData, DataTime) < 0)
			continue;

		char Buffer[64];
		string strInfo = C_BusiAlgo::ChangeBcd2Ascii((S_BYTE *)strData.data(), 6, 2, false);
		sprintf(Buffer, "%04d  %s kvarh", mp, strInfo.c_str());
		m_strsMeter.push_back(Buffer);
	}
	
	return (int)m_strsMeter.size();// 返回成功表计数
}

// 查询昨日失败表计
int C_MeterFailedWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	S_MenuScrollCtrl *pData = static_cast<S_MenuScrollCtrl *>(GetWndData(sizeof(S_MenuScrollCtrl)));
	switch (msg)
	{
	case GM_CREATE:{
		m_MeterCnt = -1;
		S_ALLMP *pALLMP = STATUS.GetALLMP();
		m_MeterMax = (int)pALLMP->m_MTBS.m_AllMPBS.count();

		pData->m_InfoIndex = 0;
		pData->m_LineOfScreen = 8;
		pData->m_SelectM = 0;
		pData->m_StartM = 0;
		pData->m_MenuCnt = 1;
		PostWndMsg(GM_USER, 0x00, 0x00);
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		if (m_MeterCnt >= 0)
		{
			for (int i = 0; i < pData->m_LineOfScreen; i++)
			{
				if (i + pData->m_StartM < pData->m_MenuCnt)
					pdc->DrawString(6, i*14+6, m_strsMeter[i + pData->m_StartM].c_str());
			}
		}
		else
		{
			pdc->DrawString(16, 0*14+6, "请稍候......");
		}
		S_RECT rect = {4, pData->m_SelectM*14+5, 152, 14};
		pdc->ReverseRect(rect);
		pdc->DrawHLine(0, 122, 160);
		if (m_MeterCnt < 0) sprintf(buffer, "表计总数:%d", m_MeterMax);
		else sprintf(buffer, "表计总数:%d; 失败:%d", m_MeterMax, m_MeterCnt);
		pdc->DrawString(20, 126, buffer);
		EndPaint(pdc);
		}break;
	case GM_USER:
		m_MeterCnt = GetMeterFailedInfo();
		pData->m_MenuCnt = (int)m_strsMeter.size();
		InvalidateRect(NULL);
		break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
		{
			RETURNWND();
		}
		else if (wParam == GVK_RIGHT || wParam == GVK_LEFT || wParam == GVK_UP || wParam == GVK_DN)
		{
			if (pData->ScrollCtrl(wParam) == true)
				InvalidateRect(NULL);
		}
		else if (wParam == GVK_OK)
		{
		}
		break;
 	case GM_DESTROY:
		ReleaseWndData();
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

int C_MeterFailedWnd::GetMeterFailedInfo(void)
{
	S_ALLMP *pALLMP = STATUS.GetALLMP();
	S_MpInfos *pMpInfos = STATUS.GetMpInfos();
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_TIME DataTime = C_TIME::DateAdd(TIME_UNIT_DAY, -1, C_TIME::TimeNow());

	for (size_t mp = pALLMP->m_MTBS.m_AllMPBS.begin(); mp < pALLMP->m_MTBS.m_AllMPBS.end(); mp++)
	{
		if (pALLMP->m_MTBS.m_AllMPBS.test(mp) == false)
			continue;

		string strData;
		if (C_Class2Data::Read(0x05060100, (S_WORD)mp, strData, DataTime) >= 0)
			continue;
		if (pMpInfos->m_Item[mp].m_Type != MP_TYPE_METER)
			continue;

		char Buffer[64], Address[64];
		S_WORD SN = pMpInfos->m_Item[mp].m_SN;
		memset(Address, 0x00, sizeof(Address));
		pMeterInfos->m_Item[SN].GetMeterAddress(Address);
		sprintf(Buffer, "%04d    %s", mp, Address);
		m_strsMeter.push_back(Buffer);
	}
	
	if (m_strsMeter.empty() == true)
	{
		m_strsMeter.push_back("没有失败表计!!!");
		return 0; // 返回失败表计数
	}

	return (int)m_strsMeter.size();// 返回失败表计数
}

// 本地模块信息
int C_ModuleInfoWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	switch (msg)
	{
	case GM_CREATE:
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();

		S_SYSRUN *pSysRun = STATUS.GetSysRunInfo();
		sprintf(buffer, "模块: %s", pSysRun->m_RouterName);
		pdc->DrawString(16, 0*16+6, buffer);
		sprintf(buffer, "厂商: %s", pSysRun->m_RouterType);
		pdc->DrawString(16, 1*16+6, buffer);
		sprintf(buffer, "芯片: %s", pSysRun->m_ChipCode);
		pdc->DrawString(16, 2*16+6, buffer);
		sprintf(buffer, "日期: %s", pSysRun->m_ChipVerDate);
		pdc->DrawString(16, 3*16+6, buffer);
		sprintf(buffer, "版本: %s", pSysRun->m_ChipVersion);
		pdc->DrawString(16, 4*16+6, buffer);

		pdc->DrawHLine(0, 122, 160);
		EndPaint(pdc);
		}break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
		{
			RETURNWND();
		}
		break;
 	case GM_DESTROY:
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

//现场调试菜单窗口
enum
{
	MANAGE_MENU_LIST_VERSION,
	MANAGE_MENU_MODEM_INFO,
	MANAGE_MENU_MODULE_INFO,

	MANAGE_MENU_MAX
};
static const char *g_ManageMenu[] = 
{
// 菜单往后面加
	"终端版本信息",
	"远程模块信息",
	"本地模块信息",
};
//static const char *g_ManageMenuWndInfo[] = {"            ", "            "};
//终端管理菜单窗口
int C_ManMenuWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	S_MenuScrollCtrl *pData = static_cast<S_MenuScrollCtrl *>(GetWndData(sizeof(S_MenuScrollCtrl)));
	switch (msg)
	{
	case GM_CREATE:
		pData->m_InfoIndex = 0;
		pData->m_LineOfScreen = 8;
		pData->m_SelectM = 0;
		pData->m_StartM = 0;
		pData->m_MenuCnt = (int)KGE_MEMBER_OF(g_ManageMenu);
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		for (int i = 0; i < pData->m_LineOfScreen; i++)
		{
			if (i + pData->m_StartM < pData->m_MenuCnt)
			{
				sprintf(buffer, "%d.%s", i + pData->m_StartM + 1, g_ManageMenu[i + pData->m_StartM]);
				pdc->DrawString(16, i*14+6, buffer);
			}
		}
		S_RECT rect = {12, pData->m_SelectM*14+5, 138, 14};
		pdc->ReverseRect(rect);
		pdc->DrawHLine(0, 122, 160);
//		pdc->DrawString(20, 126, g_ManageMenuWndInfo[pData->m_InfoIndex]);
		EndPaint(pdc);
		}break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
		{
			RETURNWND();
		}
		else if (wParam == GVK_RIGHT || wParam == GVK_LEFT || wParam == GVK_UP || wParam == GVK_DN)
		{
			if (pData->ScrollCtrl(wParam) == true)
				InvalidateRect(NULL);
		}
		else if (wParam == GVK_OK)
		{
			OnPressOK(pData);
		}
		break;
 	case GM_DESTROY:
		ReleaseWndData();
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

int C_ManMenuWnd::OnPressOK(S_MenuScrollCtrl *pData)
{
	switch (pData->m_StartM+pData->m_SelectM)
	{
	case MANAGE_MENU_LIST_VERSION:GOTOWND(C_ListVersionWnd, 0x00);break;
	case MANAGE_MENU_MODEM_INFO:GOTOWND(C_ModemInfoWnd, 0x00);break;
	case MANAGE_MENU_MODULE_INFO:GOTOWND(C_ModuleInfoWnd, 0x00);break;
	default:break;
	}
	return 0;
}

// 查询系统版本信息
int C_ListVersionWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	S_MenuScrollCtrl *pData = static_cast<S_MenuScrollCtrl *>(GetWndData(sizeof(S_MenuScrollCtrl)));
	switch (msg)
	{
	case GM_CREATE:{
		pData->m_InfoIndex = 0;
		pData->m_LineOfScreen = 8;
		pData->m_SelectM = 0;
		pData->m_StartM = 0;
		pData->m_MenuCnt = GetSysVersionInfo();
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		for (int i = 0; i < pData->m_LineOfScreen; i++)
		{
			if (i + pData->m_StartM < pData->m_MenuCnt)
				pdc->DrawString(6, i*14+6, m_strsVersion[i + pData->m_StartM].c_str());
		}
		S_RECT rect = {4, pData->m_SelectM*14+5, 152, 14};
		pdc->ReverseRect(rect);
		pdc->DrawHLine(0, 122, 160);
//		pdc->DrawString(20, 126, buffer);
		EndPaint(pdc);
		}break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
		{
			RETURNWND();
		}
		else if (wParam == GVK_RIGHT || wParam == GVK_LEFT || wParam == GVK_UP || wParam == GVK_DN)
		{
			if (pData->ScrollCtrl(wParam) == true)
				InvalidateRect(NULL);
		}
		else if (wParam == GVK_OK)
		{
		}
		break;
 	case GM_DESTROY:
		ReleaseWndData();
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

int C_ListVersionWnd::GetSysVersionInfo(void)
{
	string strData;

	m_strsVersion.push_back("基础类库");
	strData.assign("版本:").append(FND_GetVersion());
	m_strsVersion.push_back(strData);
	strData.assign("日期:").append(FND_GetReleaseDate());
	m_strsVersion.push_back(strData);

	m_strsVersion.push_back("多进程框架");
	strData.assign("版本:").append(MP_GetVersion());
	m_strsVersion.push_back(strData);
	strData.assign("日期:").append(MP_GetReleaseDate());
	m_strsVersion.push_back(strData);

	m_strsVersion.push_back("基础业务库");
	strData.assign("版本:").append(BASEB_GetVersion());
	m_strsVersion.push_back(strData);
	strData.assign("日期:").append(BASEB_GetReleaseDate());
	m_strsVersion.push_back(strData);

	m_strsVersion.push_back("南网业务库");
	strData.assign("版本:").append(NWB_GetVersion());
	m_strsVersion.push_back(strData);
	strData.assign("日期:").append(NWB_GetReleaseDate());
	m_strsVersion.push_back(strData);

	m_strsVersion.push_back("主程序");
	strData.assign("版本:").append(MP_GetSoftwareVersion());
	m_strsVersion.push_back(strData);
	strData.assign("日期:").append(MP_GetSoftwareReleaseDate());
	m_strsVersion.push_back(strData);

	m_strsVersion.push_back("硬件");
	strData.assign("版本:").append(MP_GetHardwareVersion());
	m_strsVersion.push_back(strData);
	strData.assign("日期:").append(MP_GetHardwareReleaseDate());
	m_strsVersion.push_back(strData);

	S_SYSCFG *pSysCfg = STATUS.GetSystemConfig();
	if (pSysCfg != NULL)
	{
		m_strsVersion.push_back("配置文件");
		strData.assign("版本:").append(pSysCfg->m_CfgVer);
		m_strsVersion.push_back(strData);
		strData.assign("日期:").append(pSysCfg->m_CfgDate);
		m_strsVersion.push_back(strData);
	}

	C_ZoneVersion *pZoneVersion = C_ZoneVersion::GetZoneVersionObject();
	if (pZoneVersion != NULL)
	{
		m_strsVersion.push_back("区域插件");
		strData.assign("名称:").append(pZoneVersion->GetZonePluginNameE());
		m_strsVersion.push_back(strData);
		strData.assign("版本:").append(pZoneVersion->GetZonePluginVersion());
		m_strsVersion.push_back(strData);
		strData.assign("日期:").append(pZoneVersion->GetZonePluginReleaseDate());
		m_strsVersion.push_back(strData);
	}
	return (int)m_strsVersion.size();
}

// 远程模块信息查询
int C_ModemInfoWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	switch (msg)
	{
	case GM_CREATE:
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();

		S_PppStatus *pPppStatus = CONTEXT.GetPppStatus();
		sprintf(buffer, "厂商:%s", pPppStatus->m_ModemFactory);
		pdc->DrawString(2, 0*16+6, buffer);
		sprintf(buffer, "型号:%s", pPppStatus->m_ModemModel);
		pdc->DrawString(2, 1*16+6, buffer);
		sprintf(buffer, "版本:%s", pPppStatus->m_ModemVer);
		pdc->DrawString(2, 2*16+6, buffer);
		sprintf(buffer, " SIM:%s", pPppStatus->m_ModemSim);
		pdc->DrawString(2, 3*16+6, buffer);

		S_BYTE *pIP = (S_BYTE *)&pPppStatus->m_SimIP;
		sprintf(buffer, " IP :%d.%d.%d.%d", *(pIP+0), *(pIP+1), *(pIP+2), *(pIP+3));
		pdc->DrawString(2, 4*16+6, buffer);

		pdc->DrawHLine(0, 122, 160);
		if (strlen(pPppStatus->m_ModemVer) >= 20)
			pdc->DrawString(2, 126, pPppStatus->m_ModemVer);
		EndPaint(pdc);
		}break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
		{
			RETURNWND();
		}
		break;
	case GM_TIME:
		InvalidateRect(NULL);
		break;
 	case GM_DESTROY:
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

//USB转网络设置窗口
const char *g_UsbNetSetWndInfo[] = {"            ", "参数格式错误", "参数设置成功"};
int C_UsbNetSetWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[CFG_PATH_FILE_MAX_LEN];
	S_MenuCtrl *pData = static_cast<S_MenuCtrl *>(GetWndData(sizeof(S_MenuCtrl)));
	switch (msg)
	{
	case GM_CREATE:{
		S_DWORD UsbIP = C_IfConfig::GetEth1IP();
		S_BYTE *pIP = (S_BYTE *)&UsbIP;
		sprintf(m_UsbIP, "USBIP:%d.%d.%d.%d", *(pIP+0), *(pIP+1), *(pIP+2), *(pIP+3));

		pData->m_InfoIndex = 0;
		S_RECT NumRect1 =  {14, 1*16+0, 24, 16};
		S_RECT NumRect2 = {14, 3*16+0, 24, 16};
		S_RECT NumRect3 = {14, 5*16+0, 24, 16};
		S_RECT BtnRect =  {60, 102, 32, 16};

		S_IpConfig *pIpCfg = STATUS.GetIpConfig();
		NumRect1.x += 0,  CreateNumEdit(pIpCfg->m_IP_eth1[0], NumRect1, 1);
		NumRect1.x += 32, CreateNumEdit(pIpCfg->m_IP_eth1[1], NumRect1, 2);
		NumRect1.x += 32, CreateNumEdit(pIpCfg->m_IP_eth1[2], NumRect1, 3);
		NumRect1.x += 32, CreateNumEdit(pIpCfg->m_IP_eth1[3], NumRect1, 4);

		NumRect2.x += 0,  CreateNumEdit(pIpCfg->m_NetMask_eth1[0], NumRect2, 5);
		NumRect2.x += 32, CreateNumEdit(pIpCfg->m_NetMask_eth1[1], NumRect2, 6);
		NumRect2.x += 32, CreateNumEdit(pIpCfg->m_NetMask_eth1[2], NumRect2, 7);
		NumRect2.x += 32, CreateNumEdit(pIpCfg->m_NetMask_eth1[3], NumRect2, 8);
		NumRect3.x += 0,  CreateNumEdit(pIpCfg->m_GateWay_eth1[0], NumRect3, 9);
		NumRect3.x += 32, CreateNumEdit(pIpCfg->m_GateWay_eth1[1], NumRect3, 10);
		NumRect3.x += 32, CreateNumEdit(pIpCfg->m_GateWay_eth1[2], NumRect3, 11);
		NumRect3.x += 32, CreateNumEdit(pIpCfg->m_GateWay_eth1[3], NumRect3, 12);

		C_BUTTON *pbtnSet = C_BUTTON::NewCtrl();
		pbtnSet->Create("确定", 0x00, BtnRect, this, 13);

		SetFocusCtrl(GetWndCtrl(1));
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->DrawString(14, 0*16+2, " IP 地址:");
		pdc->DrawString(40,  1*16+2, ".");
		pdc->DrawString(72,  1*16+2, ".");
		pdc->DrawString(104, 1*16+2, ".");
		pdc->DrawString(14, 2*16+2, "子网掩码:");
		pdc->DrawString(40,  3*16+2, ".");
		pdc->DrawString(72,  3*16+2, ".");
		pdc->DrawString(104, 3*16+2, ".");
		pdc->DrawString(14, 4*16+2, "默认网关:");
		pdc->DrawString(40,  5*16+2, ".");
		pdc->DrawString(72,  5*16+2, ".");
		pdc->DrawString(104, 5*16+2, ".");
		pdc->DrawHLine(0, 122, 160);
		
		pdc->DrawString(20, 126, "                      ");
		if (pData->m_InfoIndex == 0)
			pdc->DrawString(20, 126, m_UsbIP);
		else
			pdc->DrawString(20, 126, g_UsbNetSetWndInfo[pData->m_InfoIndex]);
		EndPaint(pdc);
		}
		break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
			RETURNWND();
		else if (wParam == GVK_UP)
			SetFocusCtrl(GetWndCtrl(1));
		else if (wParam == GVK_OK || wParam == GVK_RIGHT)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			int id = pCtrl->GetCtrlID() + 1;
			if (id >= 14) id = 1;
			SetFocusCtrl(GetWndCtrl(id));
		}
		else if (wParam == GVK_LEFT)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			int id = pCtrl->GetCtrlID() - 1;
			if (id <= 0) id = 13;
			SetFocusCtrl(GetWndCtrl(id));
		}
		break;
	case GM_CMD:
		if (wParam == 13 && lParam == GM_CMD_BTN_PUSHED)
		{
			S_DWORD LocalIP = 0, NetMask = 0, GateWay = 0;
			if (CheckParamFormat(LocalIP, NetMask, GateWay) == false)
			{
				pData->m_InfoIndex = 1;
				InvalidateRect(NULL);
			}
			else if (SGui_MsgBox("提示", " 确定更改网络参数?", MB_BTN_OKCANCEL) == MB_RTN_YESOK)
			{
				S_IpConfig *pIpCfg = STATUS.GetIpConfig();
				memcpy(pIpCfg->m_IP_eth1, &LocalIP, 4);
				memcpy(pIpCfg->m_NetMask_eth1, &NetMask, 4);
				memcpy(pIpCfg->m_GateWay_eth1, &GateWay, 4);
				STATUS.SaveTermIPConfig();
				pData->m_InfoIndex = 2;
				PostWndMsg(GM_USER, 0x00, 0x00);
			}
			else
			{
				pData->m_InfoIndex = 0;
			}
		}
		break;
	case GM_USER:
		C_SYSCTRL::ExecScript(C_PATH::GetScriptFile(CFG_FILE_SCRIPT_IP_CONFIG, buffer));
		break;
	case GM_DESTROY:
		ReleaseWndData();
		DeleteWndCtrl(1, 13);
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

bool C_UsbNetSetWnd::CreateNumEdit(int num, S_RECT &NumRect, S_DWORD ID)
{
	char buffer[64];
	C_NUMEDIT *pEdit = C_NUMEDIT::NewCtrl();
	sprintf(buffer, "%03d", num);
	return pEdit->Create(buffer, NUMEDIT_STYLE_DEC, NumRect, this, ID);
}

bool C_UsbNetSetWnd::CheckParamFormat(S_DWORD &LocalIP, S_DWORD &NetMask, S_DWORD &GateWay)
{
	S_BYTE *pData;
	string strData;
	int ip0, ip1, ip2, ip3, mask0, mask1, mask2, mask3, way0, way1, way2, way3;

	strData = GetWndCtrl(1)->GetWndText(), ip0 = atoi(strData.c_str());
	strData = GetWndCtrl(2)->GetWndText(), ip1 = atoi(strData.c_str());
	strData = GetWndCtrl(3)->GetWndText(), ip2 = atoi(strData.c_str());
	strData = GetWndCtrl(4)->GetWndText(), ip3 = atoi(strData.c_str());

	strData = GetWndCtrl(5)->GetWndText(), mask0 = atoi(strData.c_str());
	strData = GetWndCtrl(6)->GetWndText(), mask1 = atoi(strData.c_str());
	strData = GetWndCtrl(7)->GetWndText(), mask2 = atoi(strData.c_str());
	strData = GetWndCtrl(8)->GetWndText(), mask3 = atoi(strData.c_str());

	strData = GetWndCtrl( 9)->GetWndText(), way0 = atoi(strData.c_str());
	strData = GetWndCtrl(10)->GetWndText(), way1 = atoi(strData.c_str());
	strData = GetWndCtrl(11)->GetWndText(), way2 = atoi(strData.c_str());
	strData = GetWndCtrl(12)->GetWndText(), way3 = atoi(strData.c_str());

	if (ip0 > 255 || ip1 > 255 || ip2 > 255 || ip3 > 255
		|| mask0 > 255 || mask1 > 255 || mask2 > 255 || mask3 > 255
		|| way0 > 255 || way1 > 255 || way2 > 255 || way3 > 255)
		return false;

	pData = (S_BYTE *)&LocalIP;
	*(pData+0) = ip0, *(pData+1) = ip1, *(pData+2) = ip2, *(pData+3) = ip3;
	pData = (S_BYTE *)&NetMask;
	*(pData+0) = mask0, *(pData+1) = mask1, *(pData+2) = mask2, *(pData+3) = mask3;
	pData = (S_BYTE *)&GateWay;
	*(pData+0) = way0, *(pData+1) = way1, *(pData+2) = way2, *(pData+3) = way3;

	return true;
}

//MAC地址显示窗口
int C_MacViewWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	switch (msg)
	{
	case GM_CREATE:{
		char buffer[64];
		S_RECT BtnRect =  {60, 102, 32, 16};

		memset(buffer, 0x00, sizeof(buffer));
		C_IfConfig::GetEth0Mac(buffer, 6);
		sprintf(m_Eth0Mac, "%02X:%02X:%02X:%02X:%02X:%02X", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5]);

		memset(buffer, 0x00, sizeof(buffer));
		C_IfConfig::GetEth1Mac(buffer, 6);
		sprintf(m_Eth1Mac, "%02X:%02X:%02X:%02X:%02X:%02X", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5]);

		C_BUTTON *pbtnSet = C_BUTTON::NewCtrl();
		pbtnSet->Create("设置", 0x00, BtnRect, this, 1);

		SetFocusCtrl(GetWndCtrl(1));
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->DrawString(14, 0*18+4, "本地网络MAC地址:");
		pdc->DrawString(14+16, 1*18+4, m_Eth0Mac);
		pdc->DrawString(14, 2*18+4+6, "USB转网卡MAC地址:");
		pdc->DrawString(14+16, 3*18+4+6, m_Eth1Mac);
		pdc->DrawHLine(0, 122, 160);
		pdc->DrawString(20, 126, "网络MAC地址显示");
		EndPaint(pdc);
		}
		break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
			RETURNWND();
		break;
	case GM_CMD:
		if (wParam == 1 && lParam == GM_CMD_BTN_PUSHED)
		{
			GOTOWND(C_MacSetWnd, 0x00);
		}
		break;
	case GM_DESTROY:
		DeleteWndCtrl(1, 1);
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;	
}

//MAC地址设置窗口
const char *g_MacSetWndInfo[] = {"            ", "参数格式错误", "参数设置成功"};
int C_MacSetWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[CFG_PATH_FILE_MAX_LEN];
	S_MenuCtrl *pData = static_cast<S_MenuCtrl *>(GetWndData(sizeof(S_MenuCtrl)));
	switch (msg)
	{
	case GM_CREATE:{
		pData->m_InfoIndex = 0;
		S_RECT NumRect1 =  {8, 1*18+2, 16, 16};
		S_RECT NumRect2 = {8, 3*18+8, 16, 16};
		S_RECT BtnRect =  {60, 102, 32, 16};

		memset(buffer, 0x00, sizeof(buffer));
		C_IfConfig::GetEth0Mac(buffer, 6);
		NumRect1.x += 0,  CreateNumEdit(buffer[0], NumRect1, 1);
		NumRect1.x += 24, CreateNumEdit(buffer[1], NumRect1, 2);
		NumRect1.x += 24, CreateNumEdit(buffer[2], NumRect1, 3);
		NumRect1.x += 24, CreateNumEdit(buffer[3], NumRect1, 4);
		NumRect1.x += 24, CreateNumEdit(buffer[4], NumRect1, 5);
		NumRect1.x += 24, CreateNumEdit(buffer[5], NumRect1, 6);

		memset(buffer, 0x00, sizeof(buffer));
		C_IfConfig::GetEth1Mac(buffer, 6);
		NumRect2.x += 0,  CreateNumEdit(buffer[0], NumRect2, 7);
		NumRect2.x += 24, CreateNumEdit(buffer[1], NumRect2, 8);
		NumRect2.x += 24, CreateNumEdit(buffer[2], NumRect2, 9);
		NumRect2.x += 24, CreateNumEdit(buffer[3], NumRect2, 10);
		NumRect2.x += 24, CreateNumEdit(buffer[4], NumRect2, 11);
		NumRect2.x += 24, CreateNumEdit(buffer[5], NumRect2, 12);

		C_BUTTON *pbtnSet = C_BUTTON::NewCtrl();
		pbtnSet->Create("确定", 0x00, BtnRect, this, 13);

		SetFocusCtrl(GetWndCtrl(1));
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->DrawString(14, 0*18+4, "本地网络MAC地址:");
		pdc->DrawString(14, 2*18+10, "USB转网卡MAC地址:");
		pdc->DrawHLine(0, 122, 160);
		pdc->DrawString(20, 126, g_MacSetWndInfo[pData->m_InfoIndex]);
		EndPaint(pdc);
		}
		break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
			RETURNWND();
		else if (wParam == GVK_UP)
			SetFocusCtrl(GetWndCtrl(1));
		else if (wParam == GVK_OK || wParam == GVK_RIGHT)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			int id = pCtrl->GetCtrlID() + 1;
			if (id >= 14) id = 1;
			SetFocusCtrl(GetWndCtrl(id));
		}
		else if (wParam == GVK_LEFT)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			int id = pCtrl->GetCtrlID() - 1;
			if (id <= 0) id = 13;
			SetFocusCtrl(GetWndCtrl(id));
		}
		break;
	case GM_CMD:
		if (wParam == 13 && lParam == GM_CMD_BTN_PUSHED)
		{
			string strEth0Mac, strEth1Mac;
			if (CheckParamFormat(strEth0Mac, strEth1Mac) == false)
			{
				pData->m_InfoIndex = 1;
				InvalidateRect(NULL);
			}
			else if (SGui_MsgBox("提示", " 确定更改网络参数?", MB_BTN_OKCANCEL) == MB_RTN_YESOK)
			{
				S_IpConfig *pIpCfg = STATUS.GetIpConfig();
				copy(strEth0Mac.begin(), strEth0Mac.end(), pIpCfg->m_MAC);
				copy(strEth1Mac.begin(), strEth1Mac.end(), pIpCfg->m_MAC_eth1);
				STATUS.SaveTermIPConfig();
				pData->m_InfoIndex = 2;
				PostWndMsg(GM_USER, 0x00, 0x00);
			}
			else
			{
				pData->m_InfoIndex = 0;
			}
		}
		break;
	case GM_USER:
		C_SYSCTRL::ExecScript(C_PATH::GetScriptFile(CFG_FILE_SCRIPT_IP_CONFIG, buffer));
		break;
	case GM_DESTROY:
		ReleaseWndData();
		DeleteWndCtrl(1, 13);
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;	
}

bool C_MacSetWnd::CreateNumEdit(S_BYTE num, S_RECT &NumRect, S_DWORD ID)
{
	char buffer[64];
	C_NUMEDIT *pEdit = C_NUMEDIT::NewCtrl();
	sprintf(buffer, "%02X", num);
	return pEdit->Create(buffer, NUMEDIT_STYLE_HEX, NumRect, this, ID);
}

bool C_MacSetWnd::CheckParamFormat(string &strEth0Mac, string &strEth1Mac)
{
	string strData;
	strData = GetWndCtrl(1)->GetWndText(), strEth0Mac.append(strData).append(":");
	strData = GetWndCtrl(2)->GetWndText(), strEth0Mac.append(strData).append(":");
	strData = GetWndCtrl(3)->GetWndText(), strEth0Mac.append(strData).append(":");
	strData = GetWndCtrl(4)->GetWndText(), strEth0Mac.append(strData).append(":");
	strData = GetWndCtrl(5)->GetWndText(), strEth0Mac.append(strData).append(":");
	strData = GetWndCtrl(6)->GetWndText(), strEth0Mac.append(strData);

	strData = GetWndCtrl(7)->GetWndText(), strEth1Mac.append(strData).append(":");
	strData = GetWndCtrl(8)->GetWndText(), strEth1Mac.append(strData).append(":");
	strData = GetWndCtrl(9)->GetWndText(), strEth1Mac.append(strData).append(":");
	strData = GetWndCtrl(10)->GetWndText(), strEth1Mac.append(strData).append(":");
	strData = GetWndCtrl(11)->GetWndText(), strEth1Mac.append(strData).append(":");
	strData = GetWndCtrl(12)->GetWndText(), strEth1Mac.append(strData);

	return true;
}

static const char *g_ListSetWndInfo[] = {"            ", "系统即将复位", "参数设置失败"};
int C_ListSetWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	S_MenuScrollCtrl *pData = static_cast<S_MenuScrollCtrl *>(GetWndData(sizeof(S_MenuScrollCtrl)));
	switch (msg)
	{
	case GM_CREATE:
		pData->m_InfoIndex = 0;
		pData->m_LineOfScreen = 8;
		pData->m_SelectM = 0;
		pData->m_StartM = 0;
		pData->m_MenuCnt = GetListData(lParam);
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		for (int i = 0; i < pData->m_LineOfScreen; i++)
		{
			if (i + pData->m_StartM < pData->m_MenuCnt)
			{
				sprintf(buffer, "%d.%s", i + pData->m_StartM + 1, m_strListData[i + pData->m_StartM].c_str());
				pdc->DrawString(16, i*14+6, buffer);
			}
		}
		S_RECT rect = {12, pData->m_SelectM*14+5, 138, 14};
		pdc->ReverseRect(rect);
		pdc->DrawHLine(0, 122, 160);
		if (pData->m_InfoIndex == 0)
			pdc->DrawString(20, 126, m_BottomInfo.c_str());
		else
		pdc->DrawString(20, 126, g_ListSetWndInfo[pData->m_InfoIndex]);
			EndPaint(pdc);
		}break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
		{
			RETURNWND();
		}
		else if (wParam == GVK_RIGHT || wParam == GVK_LEFT || wParam == GVK_UP || wParam == GVK_DN)
		{
			if (pData->ScrollCtrl(wParam) == true)
				InvalidateRect(NULL);
		}
		else if (wParam == GVK_OK)
		{
			if (SGui_MsgBox("提示", " 确定更改所选参数?", MB_BTN_OKCANCEL) == MB_RTN_YESOK)
				pData->m_InfoIndex = SetListData(pData->m_StartM+pData->m_SelectM);
		}
		break;
 	case GM_DESTROY:
		ReleaseWndData();
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

int C_ListSetWnd::GetListData(S_DWORD DataType)
{
	m_DataType = DataType;
	if (m_DataType == WND_LIST_SET_ZB)
		return GetListData_ZB();

	return GetListData_APN();
}

int C_ListSetWnd::GetListData_ZB(void)
{
	static const char *g_SupportZB[] = {"EASTSOFT", "XIAOCHENG", "RUBONET", "DINGXIN"};
	for (int i = 0; i < (int)KGE_MEMBER_OF(g_SupportZB); i++)
		m_strListData.push_back(g_SupportZB[i]);

	return (int)m_strListData.size();
}

int C_ListSetWnd::GetListData_APN(void)
{
	S_PppInfo *pPppInfo = STATUS.GetPppInfo();
	m_BottomInfo.assign("APN:").append(pPppInfo->m_DialInfo.m_Apn);

	for (int i = 0; i < PPP_APN_COUNT_MAX; i++)
	{
		string strData(pPppInfo->m_ApnList[i]);
		if (strData.empty() == true)
			break;
		m_strListData.push_back(strData);
	}

	if (m_strListData.empty() == true)
		m_strListData.push_back("CMNET");
	
	return (int)m_strListData.size();
}

int C_ListSetWnd::SetListData(int idxListData)
{
	if (m_DataType == WND_LIST_SET_ZB)
		return SetListData_ZB(idxListData);

	return SetListData_APN(idxListData);
}

int C_ListSetWnd::SetListData_ZB(int idxListData)
{
//	DataApi_SetTypeZB(idxListData);
//	DataApi_SystemReset(2);
	return 1; //系统重启
}

int C_ListSetWnd::SetListData_APN(int idxListData)
{
	S_PppInfo *pPppInfo = STATUS.GetPppInfo();
	strcpy(pPppInfo->m_DialInfo.m_Apn, m_strListData[idxListData].c_str());
	m_BottomInfo.assign("APN:").append(pPppInfo->m_DialInfo.m_Apn);
	
	STATUS.SavePppInfo();
	return 0; 
}

//日志跟踪参数设置
static const char *g_TraceParamSet_Level = "disabled;error   ;warn    ;info    ;verbose ;debug   ";
static const char *g_TraceParamSet_Way = "none   ;stdout ;logfile;ttyp0  ;ttyp1  ;ttyp2  ;ttyp3  ;ttyp4  ;ttyp5  ;ttyp6  ";
static const char *g_TraceParamSetWndInfo[] = {"            ", "参数设置失败", "参数设置成功"};
int C_TraceParamSetWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	S_MenuCtrl *pData = static_cast<S_MenuCtrl *>(GetWndData(sizeof(S_MenuCtrl)));
	switch (msg)
	{
	case GM_CREATE:{
		string strAllProcess;
		strings strsAllProcess;
		S_TraceSet TraceInfo;
		S_RECT Rect =  {50, 1*20+8, 90, 20};

		pData->m_InfoIndex = 0;
		AllProcessName(strsAllProcess);
		LoadTraceInfo(strsAllProcess[0].c_str(), TraceInfo);
		strsAllProcess.Trim(' '), strsAllProcess.AppendBack(";");
		strsAllProcess.Serialize(strAllProcess);

		Rect.y += 0, Rect.w = 90;
		CreateSelectBox(strAllProcess.c_str(), 0, Rect, 1);
		Rect.y += 20, Rect.w = 90;
		CreateSelectBox(g_TraceParamSet_Level, TraceInfo.m_level, Rect, 2);
		Rect.y += 20, Rect.w = 90;
		CreateSelectBox(g_TraceParamSet_Way, TraceInfo.m_way, Rect, 3);

		S_RECT BtnRect =  {60, 94, 32, 16};
		C_BUTTON *pbtnSet = C_BUTTON::NewCtrl();
		pbtnSet->Create("确定", 0x00, BtnRect, this, 4);

		SetFocusCtrl(GetWndCtrl(1));
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->DrawString(14, 0*20+8, "日志输出参数设置:");
		pdc->DrawString(14, 1*20+10, "进程:");
		pdc->DrawString(14, 2*20+10, "级别:");
		pdc->DrawString(14, 3*20+10, "输出:");
		pdc->DrawHLine(0, 122, 160);
		pdc->DrawString(20, 126, g_TraceParamSetWndInfo[pData->m_InfoIndex]);
		EndPaint(pdc);
		}
		break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
			RETURNWND();
		else if (wParam == GVK_UP)
			SetFocusCtrl(GetWndCtrl(1));
		else if (wParam == GVK_OK || wParam == GVK_RIGHT)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			int id = pCtrl->GetCtrlID() + 1;
			if (id >= 5) id = 1;
			SetFocusCtrl(GetWndCtrl(id));
		}
		else if (wParam == GVK_LEFT)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			int id = pCtrl->GetCtrlID() - 1;
			if (id <= 0) id = 4;
			SetFocusCtrl(GetWndCtrl(id));
		}
		break;
	case GM_CMD:
		if (wParam == 1 && lParam == GM_CMD_BTN_PUSHED)
		{
			S_TraceSet TraceInfo;
			string strData = GetWndCtrl(1)->GetWndText();
			replace(strData.begin(), strData.end(), ' ', '\0');
			LoadTraceInfo(strData.c_str(), TraceInfo);
			GetWndCtrl(2)->SendWndMsg(GM_CMD, 0x00, TraceInfo.m_level);
			GetWndCtrl(3)->SendWndMsg(GM_CMD, 0x00, TraceInfo.m_way);
			InvalidateRect(NULL);
		}
		else if (wParam == 4 && lParam == GM_CMD_BTN_PUSHED)
		{
			if (SGui_MsgBox("提示", " 确定更改日志参数?", MB_BTN_OKCANCEL) == MB_RTN_YESOK)
			{
				string strProcess = GetWndCtrl(1)->GetWndText();
				string strLevel = GetWndCtrl(2)->GetWndText();
				string strWay = GetWndCtrl(3)->GetWndText();
				replace(strProcess.begin(), strProcess.end(), ' ', '\0');
				replace(strLevel.begin(), strLevel.end(), ' ', '\0');
				replace(strWay.begin(), strWay.end(), ' ', '\0');
				if (SaveTraceInfo(strProcess.c_str(), strLevel.c_str(), strWay.c_str()) == 0)
					pData->m_InfoIndex = 2;
				else
					pData->m_InfoIndex = 1;
			}
		}
		break;
	case GM_DESTROY:
		ReleaseWndData();
		DeleteWndCtrl(1, 4);
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

bool C_TraceParamSetWnd::CreateSelectBox(const char *pBuffer, S_DWORD Idx, S_RECT &NumRect, S_DWORD ID)
{
	C_SELECTBOX *pBox = C_SELECTBOX::NewCtrl();
	if (pBox->Create(pBuffer, NUMEDIT_STYLE_DEC, NumRect, this, ID) == false)
		return false;
	return pBox->SetDefaultItem(Idx);
}

int C_TraceParamSetWnd::LoadTraceInfo(const char *pProcess, S_TraceSet &TraceInfo)
{
	int PSN = C_SERVER::GetServerID(pProcess);
	S_TraceSet *pServerTrace = CONTEXT.GetTraceSet(PSN);
	if (pServerTrace == NULL)
		return -1;

	TraceInfo = *pServerTrace;
	return 0;
}

int C_TraceParamSetWnd::SaveTraceInfo(const char *pProcess, const char *pLevel, const char *pWay)
{
	char fname[CFG_PATH_FILE_MAX_LEN];
	C_IniTrace::AppCmdSetTrace(pProcess, "Level", pLevel);
	C_IniTrace::AppCmdSetTrace(pProcess, "Way", pWay);
	return C_IniTrace::SaveProcessTraceConfig(C_PATH::GetTraceFileName(fname));
}

int C_TraceParamSetWnd::AllProcessName(strings &strsProcessName)
{
	const char *pName = NULL;
	for (int i = 0; ; i++)
	{
		if ((pName = MP_GetProcessName(i)) == NULL)
			break;
		string strProcess(pName);
		strsProcessName.push_back(strProcess);
	}
	return (int)strsProcessName.size();	
}

static const char *g_ZoneCfgWndInfo[] = {"            ", "系统即将复位          ", "未发现区域包          ", "请先切换测试版本      "};
int C_ZoneCfgWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[CFG_PATH_FILE_MAX_LEN];
	S_MenuScrollCtrl *pData = static_cast<S_MenuScrollCtrl *>(GetWndData(sizeof(S_MenuScrollCtrl)));
	switch (msg)
	{
	case GM_CREATE:
		pData->m_InfoIndex = 0;
		pData->m_LineOfScreen = 8;
		pData->m_SelectM = 0;
		pData->m_StartM = 0;
		pData->m_MenuCnt = GetZoneListInfo(m_strCurrentZone, m_listZone);
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		for (int i = 0; i < pData->m_LineOfScreen; i++)
		{
			if (i + pData->m_StartM < pData->m_MenuCnt)
			{
				sprintf(buffer, "%d.%s", i + pData->m_StartM + 1, m_listZone[i + pData->m_StartM].m_strZoneName.c_str());
				pdc->DrawString(16, i*14+6, buffer);
			}
		}
		S_RECT rect = {12, pData->m_SelectM*14+5, 138, 14};
		if (pData->m_MenuCnt != 0)
			pdc->ReverseRect(rect);
		else
			pdc->DrawString(16, 0*14+6, "没有区域配置包");
		pdc->DrawHLine(0, 122, 160);

		string strBottomInfo("当前区域:");
		strBottomInfo.append(m_strCurrentZone);
		if (pData->m_InfoIndex == 0)
			pdc->DrawString(20, 126, strBottomInfo.c_str());
		else
			pdc->DrawString(20, 126, g_ZoneCfgWndInfo[pData->m_InfoIndex]);
		EndPaint(pdc);
		}break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
		{
			RETURNWND();
		}
		else if (wParam == GVK_RIGHT || wParam == GVK_LEFT || wParam == GVK_UP || wParam == GVK_DN)
		{
			if (pData->ScrollCtrl(wParam) == true)
				InvalidateRect(NULL);
		}
		else if (wParam == GVK_OK && pData->m_MenuCnt != 0)
		{
			if (SGui_MsgBox("提示", " 确定更改区域配置?", MB_BTN_OKCANCEL) == MB_RTN_YESOK)
				pData->m_InfoIndex = 1, PostWndMsg(GM_USER, 0x00, 0x00);
		}
		break;
	case GM_USER:
		m_CurrentModeType = (m_strCurrentZone == m_listZone[0].m_strZoneName); // 默认第0个为缺省模式
		m_ExpectedModeType = (m_listZone[pData->m_SelectM + pData->m_StartM].m_strZoneName == m_listZone[0].m_strZoneName); // 默认第0个为缺省模式
		C_PATH::GetZoneFile(m_listZone[pData->m_SelectM + pData->m_StartM].m_strZonePackage.c_str(), buffer);
		if (access(buffer, F_OK) != 0)// 检测升级文件
		{
			pData->m_InfoIndex = 2;
			PostWndMsg(GM_PAINT, 0x00, 0x00);
		}
		else if (m_CurrentModeType == false && m_ExpectedModeType == false) // 区域模式之间不能切换
		{
			pData->m_InfoIndex = 3;
			PostWndMsg(GM_PAINT, 0x00, 0x00);
		}
		else
		{
			C_ZoneSwh::SwitchZone(buffer);
			while (true) SleepMS(10); // 等待复位
		}
		break;
 	case GM_DESTROY:
		ReleaseWndData();
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

int C_ZoneCfgWnd::GetZoneListInfo(string &strCurrentZone, S_ZoneCfgs &ZoneList)
{
	return C_ZoneSwh::LoadZoneInfo(strCurrentZone, ZoneList);
}


