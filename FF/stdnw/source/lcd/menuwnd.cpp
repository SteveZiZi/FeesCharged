/** @file
 *  @brief 本地操作菜单窗口定义
 *  @author 
 *  @date 2012/01/09
 *  @version 0.1
 */
#include "menuwnd.h"
#include "sguiapp.h"
#include "sguictrl.h"
#include "sguidlog.h"
#include "timeop.h"
#include "status.h"
#include "paramwnd.h" //temp yzq add
#include "powerwnd.h"
#include "managwnd.h"
#include "privatew.h"
#include "factwnd.h"
#include "cfg_path.h"

#define MENUCOUNTNUM	11
struct S_UIMENU
{
	const S_CHAR *m_pMenu[MENUCOUNTNUM];
	S_UIMENU *m_pSubMenu[MENUCOUNTNUM];
	S_UIMENU *m_pPMenu;
	S_BYTE m_MenuID;
	S_BYTE m_MenuCount;
};

#define MID_COUNT	20	/**	每级菜单最大ID数*/
#define MID_MKID(lv,off)	(lv*MID_COUNT + off)	/**	根据菜单级数lv和菜单偏移获取菜单ID lv从0开始 off从0开始*/
#define MID_GTLV(id)	((int)(id/MID_COUNT))		/**	根据菜单ID获取菜单的级别*/
#define MID_GTOF(id)	((int)(id%MID_COUNT))		/**	根据菜单ID获取菜单的在子菜单中的偏移*/

extern S_UIMENU MainMenu;
extern S_UIMENU GathQuery;

S_UIMENU GathRealtimeData =
{
	{"正向有功总电能示值", "反向有功总电能示值", "电压", "电流", "有功功率", 
		"", "", "", "", "", 
		""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&GathQuery, MID_MKID(2,3),	5
};
/************************************************************************************************************************************************
**函 数 名: 
**作    者: 刘志强 
**日    期: 2013-9-11 9:52:38
**功    能:	三级主菜单
**-----------------------------------------------------------------------------------------------------
**备    注: 
**	，刘志强仅添加注释
**-----------------------------------------------------------------------------------------------------
**********************************************************************************************************************************************/
S_UIMENU GathHistoryData =
{
	{"日正向有功总电能示值", "日反向有功总电能示值", "日冻结电量", "月冻结电量", "", 
		"", "", "", "", "", 
		""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&GathQuery, MID_MKID(2,2),	4
};
/************************************************************************************************************************************************
**函 数 名: 
**作    者: 刘志强 
**日    期: 2013-9-11 9:52:38
**功    能:	二级主菜单
**-----------------------------------------------------------------------------------------------------
**备    注: 
**	，刘志强仅添加注释
**-----------------------------------------------------------------------------------------------------
**********************************************************************************************************************************************/
S_UIMENU GathQuery =
{
	{"抄表统计数据", "异常告警", "历史数据显示", "实时数据显示", "", 
		"", "", "", "", "", 
		""},
	{NULL,NULL,&GathHistoryData,&GathRealtimeData,NULL,NULL,NULL,NULL},
	&MainMenu, MID_MKID(1,0),	4
};

/************************************************************************************************************************************************
**函 数 名: 
**作    者: 刘志强 
**日    期: 2013-9-11 9:52:38
**功    能:	三级主菜单
**-----------------------------------------------------------------------------------------------------
**备    注: 
**	刘志强添加
**	1、ParamQuery 菜单中“2-7 测试通道设置”的子菜单
**-----------------------------------------------------------------------------------------------------
**********************************************************************************************************************************************/
//S_UIMENU ParamTestCom =
//{
//	{" 测试通道设置", "", "", "", "", "", "", "", "", "", ""},
//	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
//	&MainMenu, MID_MKID(2,0),	1
//};
/************************************************************************************************************************************************
**函 数 名: 
**作    者: 刘志强 
**日    期: 2013-9-11 9:52:38
**功    能:	二级主菜单
**-----------------------------------------------------------------------------------------------------
**备    注: 
**	1、，刘志强仅添加注释 2013-9-11 9:52:38
**	2、刘志强添加测试通道设置、修改菜单数量为7
**-----------------------------------------------------------------------------------------------------
**********************************************************************************************************************************************/
S_UIMENU ParamQuery =
{
	{"通讯通道查询与设置", "测量点参数查询与设置", "级联参数查询与设置", "限值参数查询", "", "", "", "", "", "", ""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&MainMenu, MID_MKID(1,1),	4
};


/************************************************************************************************************************************************
**函 数 名: 
**作    者: 刘志强 
**日    期: 2013-9-11 9:52:38
**功    能:	二级主菜单
**-----------------------------------------------------------------------------------------------------
**备    注: 
**	，刘志强仅添加注释
**-----------------------------------------------------------------------------------------------------
**********************************************************************************************************************************************/
S_UIMENU TermManage =
{
	{ "重启终端", "液晶调节", "终端编号","终端版本", "终端时间设置", "界面密码设置", "测试通道设置", "信号强度和电池电量", "无线实时状态", "", ""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&MainMenu, MID_MKID(1,2),	9
};

/************************************************************************************************************************************************
**函 数 名: 
**作    者: 刘志强 
**日    期: 2013-9-11 9:52:38
**功    能:	一级主菜单
**-----------------------------------------------------------------------------------------------------
**备    注: 
**	，刘志强仅添加注释
**-----------------------------------------------------------------------------------------------------
**********************************************************************************************************************************************/
S_UIMENU MainMenu =
{
	{"数据查询", "参数查询与设置", "终端管理与维护", "", "", "", "", "", "", "", ""},
	{&GathQuery,&ParamQuery,&TermManage,NULL,NULL,NULL,NULL,NULL},
	NULL, MID_MKID(0,0),	3
};

struct S_MENUWDATA
{
	S_UIMENU *m_pCurMenu;
	S_MenuScrollCtrl m_ScrollCtrl;
};

const S_WORD g_FactKeySeq[FACT_KEY_SEQ_MAX] = {GVK_LEFT, GVK_LEFT, GVK_RIGHT, GVK_RIGHT};
int C_MENUWND::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	S_MENUWDATA *pData = static_cast<S_MENUWDATA *>(GetWndData(sizeof(S_MENUWDATA)));

	switch (msg)
	{
	case GM_CREATE:
		pData->m_pCurMenu = &MainMenu;
		pData->m_ScrollCtrl.m_SelectM=0;
		pData->m_ScrollCtrl.m_MenuCnt=MainMenu.m_MenuCount;
		pData->m_ScrollCtrl.m_LineOfScreen = LINEOFSCRT;
		pData->m_ScrollCtrl.m_StartM = 0;
		SetFactoryKeySequence(GVK_CANCEL);
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		S_MenuScrollCtrl &ScrollCtrl = pData->m_ScrollCtrl;
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		for(int i = 0; i < ScrollCtrl.m_LineOfScreen; i++)
		{
			if (i + ScrollCtrl.m_StartM < ScrollCtrl.m_MenuCnt)
				pdc->DrawString(STRINGX, LINEXY(i), pData->m_pCurMenu->m_pMenu[i + ScrollCtrl.m_StartM]);
		}
		
		S_RECT rect = {STRINGX, ScrollCtrl.m_SelectM*18+8, STRINGW, 16};
		pdc->ReverseRect(rect);
		pdc->DrawHLine(0, 122, 160);
		EndPaint(pdc);
		}
		break;
	case GM_KEYUP:
		SetFactoryKeySequence(wParam);
		if (wParam == GVK_DN || wParam == GVK_UP || wParam == GVK_LEFT || wParam == GVK_RIGHT)
		{
			if (pData->m_ScrollCtrl.ScrollCtrl(wParam) == true)
				InvalidateRect(NULL);
		}
		else if (wParam == GVK_CANCEL)
		{
			if (pData->m_pCurMenu->m_pPMenu)
			{
				pData->m_ScrollCtrl.SetMenuHighlight(pData->m_pCurMenu->m_pPMenu->m_MenuCount, MID_GTOF(pData->m_pCurMenu->m_MenuID));
				pData->m_pCurMenu = pData->m_pCurMenu->m_pPMenu;
				InvalidateRect(NULL);
			}
			else
			{
				RETURNWND();
				pData->m_ScrollCtrl.m_SelectM = 0;
				pData->m_ScrollCtrl.m_StartM  = 0;
			}
		}
		else if (wParam == GVK_OK)
		{
			if (pData->m_pCurMenu->m_pSubMenu[pData->m_ScrollCtrl.GetSelectMenuID()] != NULL)
			{

				if (pData->m_ScrollCtrl.GetSelectMenuID() == 0 /*&& memcmp(m_FactKey, g_FactKeySeq, sizeof(m_FactKey)) == 0x00*/
					&& CheckFactoryTest() == true)
				{
					SetFactoryKeySequence(GVK_CANCEL);
					GOTOWND(C_FactMainWnd, 0x00);
				}
				else if (pData->m_ScrollCtrl.GetSelectMenuID() == 1 && memcmp(m_FactKey, g_FactKeySeq, sizeof(m_FactKey)) == 0x00)
				{
					SetFactoryKeySequence(GVK_CANCEL);
					GOTOWND(C_PrivateMenuWnd, 0x00);
				}
				else
				{
					pData->m_pCurMenu = pData->m_pCurMenu->m_pSubMenu[pData->m_ScrollCtrl.GetSelectMenuID()];
					pData->m_ScrollCtrl.m_SelectM = 0;
					pData->m_ScrollCtrl.m_StartM = 0;
					pData->m_ScrollCtrl.m_MenuCnt=pData->m_pCurMenu->m_MenuCount;
					InvalidateRect(NULL);
				}
			}
			else
			{
				//抄表数据显示				
				if (pData->m_pCurMenu->m_MenuID == MID_MKID(1,0) && pData->m_ScrollCtrl.GetSelectMenuID() == 0)
					GOTOWND(C_QueryGatherInfo, 0x00);//可显示抄表统计信息
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(1,0) && pData->m_ScrollCtrl.GetSelectMenuID() == 1)
					GOTOWND(C_PrintAlarmWnd/*C_QueryAlarmWnd*/, 0x00);//可显示异常告警信息
				// 显示实时数据
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(2,3) && pData->m_ScrollCtrl.GetSelectMenuID()== 0)
					GOTOWND(C_PowerZXYGWnd, ZXYGPWRIND);//可实时抄读各测量点正向有功总电量
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(2,3) && pData->m_ScrollCtrl.GetSelectMenuID()== 1)
					GOTOWND(C_PowerZXYGWnd, FXYGPWRIND);//可实时抄读各测量点反向有功总电量
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(2,3) && pData->m_ScrollCtrl.GetSelectMenuID()== 2)
					GOTOWND(C_PowerZXYGWnd, VOLTAGE);// 电压
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(2,3) && pData->m_ScrollCtrl.GetSelectMenuID()== 3)
					GOTOWND(C_PowerZXYGWnd, CURRENT);// 电流
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(2,3) && pData->m_ScrollCtrl.GetSelectMenuID()== 4)
					GOTOWND(C_PowerZXYGWnd, ACTPWR);// 有功功率
				//显示历史数据
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(2,2) && pData->m_ScrollCtrl.GetSelectMenuID() == 0)
					GOTOWND(C_MeasPntDataDisplayWnd, ZXYGPWRIND);//日冻结正向有功总
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(2,2) && pData->m_ScrollCtrl.GetSelectMenuID() == 1)
					GOTOWND(C_MeasPntDataDisplayWnd, FXYGPWRIND);//可显示反向有功总电能
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(2,2) && pData->m_ScrollCtrl.GetSelectMenuID() == 2)
					GOTOWND(C_HistoryDayWnd, 0x00);// 日冻结电量
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(2,2) && pData->m_ScrollCtrl.GetSelectMenuID() == 3)
					GOTOWND(C_HistoryMonWnd, 0x00);// 月冻结电量
				//参数设置与查看
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(1,1) && pData->m_ScrollCtrl.GetSelectMenuID() == 0)
					GOTOWND(C_CommunMenuWnd, 0x00);//通讯通道设置
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(1,1) && pData->m_ScrollCtrl.GetSelectMenuID() == 1)
					GOTOWND(C_MeterParamViewWnd, 0x00);//测量点参数设置
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(1,1) && pData->m_ScrollCtrl.GetSelectMenuID() == 2)
					GOTOWND(C_ForwardParamViewWnd, 0x00);//级联参数查询、设置
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(1,1) && pData->m_ScrollCtrl.GetSelectMenuID() == 3)
					GOTOWND(C_ThresholdViewWnd, 0x00);//终端限值参数查询、设置
				//终端管理与维护
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(1,2) && pData->m_ScrollCtrl.GetSelectMenuID() == 0)
					GOTOWND(C_RebootWnd, 0x00);//重启终端	重新启动终端
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(1,2) && pData->m_ScrollCtrl.GetSelectMenuID() == 1)
					GOTOWND(C_ScreenParamViewWnd, 0x00);//液晶调节	可调节终端液晶显示对比度、背光亮度等
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(1,2) && pData->m_ScrollCtrl.GetSelectMenuID() == 2)
					GOTOWND(C_RtuaViewWnd, 0x00);//终端编号设置
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(1,2) && pData->m_ScrollCtrl.GetSelectMenuID() == 3)
					GOTOWND(C_ShowVersionWnd, 0x00);//终端版本	可显示终端软、硬件版本等
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(1,2) && pData->m_ScrollCtrl.GetSelectMenuID() == 4)
					GOTOWND(C_TerminalTimeViewWnd, 0x00);//终端时间设置
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(1,2) && pData->m_ScrollCtrl.GetSelectMenuID() == 5)
					GOTOWND(C_PasswordWnd, 0x00);//界面密码设置
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(1,2) && pData->m_ScrollCtrl.GetSelectMenuID() == 6)
					GOTOWND(C_DebugMenuWnd, 0x00);//测试通道设置
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(1,2) && pData->m_ScrollCtrl.GetSelectMenuID() == 7)
					GOTOWND(C_SignalAndBatteryWnd, 0x00);//信号强度和电池电量
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(1,2) && pData->m_ScrollCtrl.GetSelectMenuID() == 8)
					GOTOWND(C_WirelessWnd, 0x00);//无线实时状态	可提供终端连接无线网络的实时状态信息
			}
		}
		break;
	case GM_DESTROY:
		ReleaseWndData();
		C_PWD::SetCertification(false);
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

int C_MENUWND::SetFactoryKeySequence(S_WORD key)
{
	if (key == GVK_LEFT || key == GVK_RIGHT)
	{
		for (S_WORD i = 0; i < sizeof(m_FactKey)/sizeof(m_FactKey[0]); i++)
		{
			if (m_FactKey[i] == 0x00)
			{
				m_FactKey[i] = key;
				break;
			}
		}
	}
	else if (key != GVK_OK)
	{
		memset(m_FactKey, 0x00, sizeof(m_FactKey));
	}
	return 0;
}

bool C_MENUWND::CheckFactoryTest(void)
{
#ifdef WIN32
	if (memcmp(m_FactKey, g_FactKeySeq, sizeof(m_FactKey)) == 0x00)
		return  true;
#else
	char FlagFile[260];
	C_USBPATH::GetUsbTestFile(TEST_USB_FLAG, FlagFile);

	if (access(FlagFile, F_OK)  == 0)
		return true;
#endif
	return false;
}


//
//以下为demo，不须要请注释
//
#if 0
int C_CTRLWND::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	switch (msg)
	{
	case GM_CREATE:{
		S_RECT rect1 = {10, 20, 34, 16};
//		S_RECT rect2 = {50, 20, 34, 16};
		S_RECT rect3 = {90, 20, 54, 18};
		C_NUMEDIT *pNumEdit1 = new C_NUMEDIT;
//		C_NUMEDIT *pNumEdit2 = new C_NUMEDIT;
		C_SELECTBOX *pSelectBox = new C_SELECTBOX;
		pNumEdit1->Create("89ABC", NUMEDIT_STYLE_HEX, rect1, this, 1);
//		pNumEdit2->Create("89ABC", 0x00, rect2, this, 2);
		pSelectBox->Create("89;ABC;我是谁", 0x00, rect3, this, 3);
		SetFocusCtrl(pNumEdit1);
		}
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		EndPaint(pdc);
		}
		break;
	case GM_DESTROY:
		DeleteWndCtrl(1, 3);
		break;
	case GM_KEYUP:
		if (wParam == GVK_LEFT)
			SetFocusCtrl(GetWndCtrl(1));
		else if (wParam == GVK_RIGHT)
			SetFocusCtrl(GetWndCtrl(3));
		else if (wParam == GVK_CANCEL)
			RETURNWND();
		else if (wParam == GVK_OK)
			SGui_MsgBox("提示", "  消息框弹出了！", 0x00);
		break;
	case GM_CMD:
		if (wParam == 3 && lParam == GM_CMD_CTRL_UPDATE)
		{
			char buffer[32];
			sprintf(buffer, "选择了%d", lParam);
			SGui_MsgBox("提示", buffer, 0x00);
		}
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

int C_MSGBOXWND::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	switch (msg)
	{
	case GM_CREATE:
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		EndPaint(pdc);
		}
		break;
	case GM_DESTROY:
		break;
	case GM_KEYUP:
		if (wParam == GVK_UP)
			SGui_MsgBox("提示", "  消息框弹出了！", MB_BTN_OK);
		else if (wParam == GVK_DN)
			SGui_MsgBox("提示", "  消息框弹出了！", MB_BTN_OKCANCEL);
		else if (wParam == GVK_CANCEL)
			RETURNWND();
		else if (wParam == GVK_OK)
			SGui_MsgBox("提示", "  消息框弹出了！", 0x00);
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

int C_MSGSENDWND::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	switch (msg)
	{
	case GM_CREATE:{
		S_RECT rect1 = {60, 18, 70, 16};
		S_RECT rect2 = {10, 70, 34, 16};
		S_RECT rect3 = {10, 50, 140, 16};
		C_NUMEDIT *pNumEdit1 = new C_NUMEDIT;
		C_VKBOARD *pVk = new C_VKBOARD;
		C_TEXTEDIT *pEdit = new C_TEXTEDIT;
		pNumEdit1->Create("13560303396", NUMEDIT_STYLE_DEC, rect1, this, 1);
		pVk->Create(0x00, rect2, this, 2);
		pEdit->Create("                     ", 0x00, rect3, this, 3);
		SetFocusCtrl(pNumEdit1);
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->DrawString(10, 2, "虚拟键盘的\"Ent\"发送消息");
		pdc->DrawString(10, 20, "手机号:");
		pdc->DrawString(10, 36, "发送内容:");
		EndPaint(pdc);
		}
		break;
	case GM_DESTROY:
		DeleteWndCtrl(1, 3);
		break;
	case GM_KEYUP:
		if (wParam == GVK_OK)
			SetFocusCtrl(GetWndCtrl(2));
		break;
	case GM_CHAR:
		if (wParam == GVK_ENTER)
		{
			string msg;
			C_GUICTRL *pCtrl = GetWndCtrl(1);
			msg.append(pCtrl->GetWndText());
			pCtrl = GetWndCtrl(3);
			msg.append(pCtrl->GetWndText());
//			C_SYSIPC::Send2Server(HDJS_SERID_PPP, C_SYSIPC::SYSIPC_TYPE_MSG_SEND, msg);
		}
		else if (wParam == GVK_ESC)
			SetFocusCtrl(GetWndCtrl(1));
		else if (wParam == GVK_F1)
			RETURNWND();
		else
		{
			C_GUICTRL *pCtrl = GetWndCtrl(3);
			pCtrl->SendWndMsg(GM_CHAR, wParam, lParam);
		}
		break;
	case GM_TIME:{
//		int SerID;S_BYTE MsgT;
		string msg;
//		if ((C_SYSIPC::Query4Server(SerID, MsgT, msg) >= 0) && MsgT == C_SYSIPC::SYSIPC_TYPE_MSG_SEND)
		{
			string strDA, strUD;
//			strDA.append(msg.begin(), msg.begin()+14);
//			strUD.assign(msg.begin()+14, msg.end());
//			SGui_MsgBox(strDA.c_str(), strUD.c_str(), MB_BTN_OK);
		}
		}break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

int C_VKWND::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	switch (msg)
	{
	case GM_CREATE:{
		S_RECT rect1 = {10, 70, 34, 16};
		S_RECT rect2 = {12, 10, 134, 16};
		C_VKBOARD *pVk = new C_VKBOARD;
		C_TEXTEDIT *pEdit = new C_TEXTEDIT;
		pVk->Create(0x00, rect1, this, 1);
		pEdit->Create("          ", 0x00, rect2, this, 2);
		SetFocusCtrl(pVk);
		}
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		EndPaint(pdc);
		}
		break;
	case GM_DESTROY:
		DeleteWndCtrl(1, 2);
		break;
	case GM_KEYUP:
		if (wParam == GVK_OK)
		{
			SetFocusCtrl(GetWndCtrl(1));
		}
		else if (wParam == GVK_CANCEL)
		{
			RETURNWND();
		}
		break;
	case GM_CHAR:
		if (wParam == GVK_ENTER)
		{
		}
		else if (wParam == GVK_ESC)
		{
			SetFocusCtrl(GetWndCtrl(2));
		}
		else if (wParam == GVK_F1)
		{
		}
		else
		{
			C_GUICTRL *pCtrl = GetWndCtrl(2);
			pCtrl->SendWndMsg(GM_CHAR, wParam, lParam);
		}
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

int C_FONTWND::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	static char FontShow[2] = " ";
	switch (msg)
	{
	case GM_CREATE:{
		}
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->DrawString(10, 10, FontShow);
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_SYM16x16));
		pdc->DrawString(30, 10, FontShow);
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_SYM08x16));
		pdc->DrawString(50, 10, FontShow);
		EndPaint(pdc);
		}
		break;
	case GM_DESTROY:
		break;
	case GM_KEYUP:
		if (wParam == GVK_OK)
		{
			FontShow[0] += 1;
			if (FontShow[0] > '~')
				FontShow[0] = ' ';
			InvalidateRect(NULL);
		}
		else if (wParam == GVK_CANCEL)
		{
			RETURNWND();
		}
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}
#endif


