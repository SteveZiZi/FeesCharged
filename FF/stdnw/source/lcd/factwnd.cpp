/** @file
 *  @brief 生产自动化测试窗口定义
 *  @author 
 *  @date 2012/10/26
 *  @version 0.1
 */
#include "factwnd.h"
#include "sguiapp.h"
#include "sguictrl.h"
#include "factapi.h"
#include "cfg_usrp.h"
#include "osrelate.h"
#include "cfg_path.h"
#include "sysctrl.h"

static C_GwTermApi g_GwTermApiObj;
//生产测试主菜单
const char *g_FactMainMenu[] = {"自动测试", "手动测试", "重新测试", "生成报告", "重启终端"};
const char *g_FactMainWndInfo[] = {"终端测试就绪...", "终端正在测试...", "整机测试失败...", "整机测试合格...", "终端即将重启..."};
int C_FactMainWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	S_MenuCtrl *pData = static_cast<S_MenuCtrl *>(GetWndData(sizeof(S_MenuCtrl)));
	C_FactAutoTest *pTestObj = C_AutoTestFactory::GetFactoryAutoTestObj();
	switch (msg)
	{
	case GM_CREATE:
		pTestObj->InitAutoTestInfo(g_GwTermApiObj, CFG_PATH_FACTORY_TEST);
		pData->m_SelectM = 0;
		pData->m_MenuCnt = (int)sizeof(g_FactMainMenu)/sizeof(g_FactMainMenu[0]);
		pData->m_InfoIndex = GetFactoryTestState(pTestObj);
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		for (int i = 0; i < pData->m_MenuCnt; i++)
		{
			sprintf(buffer, "%d、%s", i+1, g_FactMainMenu[i]);
			pdc->DrawString(14, i*18+8, buffer);
		}
		S_RECT rect = {12, pData->m_SelectM*18+6, 138, 16};
		pdc->ReverseRect(rect);
		pdc->DrawHLine(0, 122, 160);
		pdc->DrawString(20, 126, g_FactMainWndInfo[pData->m_InfoIndex]);
		EndPaint(pdc);
		}
		break;
	case GM_KEYUP:
		if (wParam == GVK_DN)
		{
			pData->m_SelectM++;
			if (pData->m_SelectM >= pData->m_MenuCnt)
				pData->m_SelectM = 0;
			InvalidateRect(NULL);
		}
		else if (wParam == GVK_UP)
		{
			pData->m_SelectM--;
			if (pData->m_SelectM <= -1)
				pData->m_SelectM = pData->m_MenuCnt-1;
			InvalidateRect(NULL);
		}
		else if (wParam == GVK_CANCEL)
		{
//			RETURNWND();
		}
		else if (wParam == GVK_OK)
		{
			if (pData->m_SelectM == 0) GOTOWND(C_AutoTestWnd, 0x00);
			else if (pData->m_SelectM == 1) GOTOWND(C_ManualTestWnd, 0x00);
			else if (pData->m_SelectM == 2) GOTOWND(C_ResetTestWnd, 0x00);
			else if (pData->m_SelectM == 3) GOTOWND(C_NameReportWnd, 0x00);
			else if (pData->m_SelectM == 4) RebootTermWnd(pData);
		}
		break;
	case GM_RESTORE:
		if (pData->m_InfoIndex <= 3)
			pData->m_InfoIndex = GetFactoryTestState(pTestObj);
		break;
	case GM_AUTOBACK:
		return -1;
	case GM_DESTROY:
		ReleaseWndData();
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

int C_FactMainWnd::RebootTermWnd(S_MenuCtrl *pData)
{
	if (SGui_MsgBox("提示", "    确认重启终端?", MB_BTN_OKCANCEL) != MB_RTN_YESOK)
		return -1;
	pData->m_InfoIndex = 4;
	g_GwTermApiObj.SoftwareReset();
	return 0;
}

int C_FactMainWnd::GetFactoryTestState(C_FactAutoTest *pTestObj)
{
	S_TestNames AutoItem, ManualItem;
	int CntStandby = 0, CntUnpass = 0, CntPass = 0;
	pTestObj->GetAutoTestItemName(AutoItem, ManualItem);
	for (size_t m = 0; m < AutoItem.size(); m++)
	{
		if (AutoItem[m].m_Result == TEST_RESULT_UNPASS)
			CntUnpass += 1;
		else if (AutoItem[m].m_Result == TEST_RESULT_PASS)
			CntPass += 1;
		else
			CntStandby += 1;
	}
	for (size_t m = 0; m < ManualItem.size(); m++)
	{
		if (ManualItem[m].m_Result == TEST_RESULT_UNPASS)
			CntUnpass += 1;
		else if (ManualItem[m].m_Result == TEST_RESULT_PASS)
			CntPass += 1;
		else
			CntStandby += 1;
	}
	
	if (CntUnpass == 0 && CntPass == 0)
		return 0;//未开始测试
	else if (CntStandby == 0 && CntUnpass == 0)
		return 3;// 整机测试合格
	else if (CntStandby == 0 && CntUnpass != 0)
		return 2;//整机测试不合格
	return 1;//正在测试
}

//自动测试窗口
const char *g_TestResultName[] = {"未测试", "不合格", "合格"};
int C_AutoTestWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	S_MenuScrollCtrl *pData = static_cast<S_MenuScrollCtrl *>(GetWndData(sizeof(S_MenuScrollCtrl)));
	C_FactAutoTest *pTestObj = C_AutoTestFactory::GetFactoryAutoTestObj();
	switch (msg)
	{
	case GM_CREATE:{
		S_TestNames ManualItems;
		pTestObj->GetAutoTestItemName(m_AutoItems, ManualItems);
		pData->m_InfoIndex = 0;
		pData->m_LineOfScreen = 8;
		pData->m_SelectM = 0;
		pData->m_StartM = 0;
		pData->m_MenuCnt = (int)m_AutoItems.size();
		if (NeedAutoTestExec() == true)
			PostWndMsg(GM_AUTO_TEST, 0x00, 0x00);
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		for (int i = 0; i < pData->m_LineOfScreen; i++)
		{
			if (i + pData->m_StartM < (int)m_AutoItems.size())
			{
				sprintf(buffer, "%d.%s               ", i + pData->m_StartM + 1, m_AutoItems[i + pData->m_StartM].m_strName.c_str());
				strcpy(buffer+16, g_TestResultName[m_AutoItems[i + pData->m_StartM].m_Result]);
				pdc->DrawString(16, i*14+6, buffer);
			}
		}
		S_RECT rect = {12, pData->m_SelectM*14+5, 138, 14};
		pdc->ReverseRect(rect);
		pdc->DrawHLine(0, 122, 160);
		pdc->DrawString(20, 126, "                      ");
		if (m_strBottomInfo.empty() == false)
			pdc->DrawString(20, 126, m_strBottomInfo.c_str());
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
			if (SGui_MsgBox("提示", "    重测选中项?", MB_BTN_OKCANCEL) != MB_RTN_YESOK)
				break;
			PostWndMsg(GM_DISPLAY_TEST, pData->m_StartM + pData->m_SelectM, 0x00);
		}
		break;
	case GM_DISPLAY_TEST:
		sprintf(buffer, "重测第%d项......", wParam + 1);
		m_strBottomInfo.assign(buffer);
		m_AutoItems[wParam].m_Result = TEST_RESULT_STANDBY;
		PostWndMsg(GM_PAINT, 0x00, 0x00);
		PostWndMsg(GM_MANUAL_TEST, wParam, 0x00);
		break;
	case GM_AUTO_TEST:
		GOTOWND(C_AutoExecWnd, 0x00);
		break;
	case GM_MANUAL_TEST:{
		string strMsg;
		if (lParam & 0x80000000) strMsg.push_back((S_BYTE)lParam);
		switch (pTestObj->ExecuteTestItem(false, m_AutoItems[wParam], strMsg))
		{
		case TEST_EXEC_MSG:
			m_strBottomInfo = strMsg;PostWndMsg(GM_PAINT, 0x00, 0x00);
		case TEST_EXEC_CONTINUE:
			PostWndMsg(GM_MANUAL_TEST, wParam, 0x00); break;
		case TEST_EXEC_REQ_OK:
			if (SGui_MsgBox("确认", "    测试合格?", MB_BTN_OKCANCEL) == MB_RTN_YESOK) lParam = 1;
			else lParam = 0;
			PostWndMsg(GM_MANUAL_TEST, wParam, lParam | 0x80000000); break;
		case TEST_EXEC_ERROR:
		case TEST_EXEC_FINISH:
			m_strBottomInfo = "测试结束!!!";
			PostWndMsg(GM_RESTORE, 0x00, 0x00);PostWndMsg(GM_PAINT, 0x00, 0x00);break;
		default:break;
		}
		}break;
	case GM_RESTORE:{
		S_TestNames ManualItems;
		m_AutoItems.clear();
		pTestObj->GetAutoTestItemName(m_AutoItems, ManualItems);
		}break;
	case GM_AUTOBACK:
		return -1;
	case GM_DESTROY:
		ReleaseWndData();
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

bool C_AutoTestWnd::NeedAutoTestExec(void)
{
	for (size_t i = 0; i < m_AutoItems.size(); i++)
	{
		if (m_AutoItems[i].m_Result == TEST_RESULT_STANDBY)
			return true;
	}
	return false;
}

int C_AutoExecWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	string strMsg;
	S_MenuCtrl *pData = static_cast<S_MenuCtrl *>(GetWndData(sizeof(S_MenuCtrl)));
	C_FactAutoTest *pTestObj = C_AutoTestFactory::GetFactoryAutoTestObj();
	switch (msg)
	{
	case GM_CREATE:{
		S_TestNames ManualItems;
		m_fPause = false;
		pData->m_SelectM = 0;
		pData->m_MenuCnt = 0;
		pData->m_InfoIndex = 0;
		pTestObj->GetAutoTestItemName(m_AutoItems, ManualItems);
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		const char *pInfo1 = "测试中...", *pInfo2 = "按方向键暂停!";
		if (m_fPause == true)
			pInfo1 = "退出测试?", pInfo2 = "按确认键退出!";
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		pdc->DrawString(48, 40, pInfo1);
		pdc->DrawString(32, 60, pInfo2);
		pdc->DrawHLine(0, 122, 160);
		EndPaint(pdc);
		}break;
	case GM_KEYUP:
		if (m_fPause == false)
		{
			if (wParam == GVK_UP || wParam == GVK_DN || wParam == GVK_LEFT || wParam == GVK_RIGHT)
				m_fPause = true,InvalidateRect(NULL);
		}
		else
		{
			if (wParam == GVK_OK)
				RETURNWND();
			else if (wParam == GVK_CANCEL)
				m_fPause = false,SendWndMsg(GM_PAINT, 0x00, 0x00);
		}
		break;
	case GM_TIME:
		if (pData->m_SelectM == (int)m_AutoItems.size())
			RETURNWND();
		else if (m_fPause == false)
			pTestObj->ExecuteTestItem(true, m_AutoItems[pData->m_SelectM++], strMsg);
		break;
	case GM_AUTOBACK:
		return -1;
	case GM_DESTROY:
		ReleaseWndData();
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

//手动测试窗口
int C_ManualTestWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	S_MenuScrollCtrl *pData = static_cast<S_MenuScrollCtrl *>(GetWndData(sizeof(S_MenuScrollCtrl)));
	C_FactAutoTest *pTestObj = C_AutoTestFactory::GetFactoryAutoTestObj();
	switch (msg)
	{
	case GM_CREATE:{
		S_TestNames AutoItems;
		pTestObj->GetAutoTestItemName(AutoItems, m_ManualItems);
		pData->m_InfoIndex = 0;
		pData->m_LineOfScreen = 8;
		pData->m_SelectM = 0;
		pData->m_StartM = 0;
		pData->m_MenuCnt = (int)m_ManualItems.size();
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		for (int i = 0; i < pData->m_LineOfScreen; i++)
		{
			if (i + pData->m_StartM < (int)m_ManualItems.size())
			{
				sprintf(buffer, "%d.%s               ", i + pData->m_StartM + 1, m_ManualItems[i + pData->m_StartM].m_strName.c_str());
				strcpy(buffer+16, g_TestResultName[m_ManualItems[i + pData->m_StartM].m_Result]);
				pdc->DrawString(16, i*14+6, buffer);
			}
		}
		S_RECT rect = {12, pData->m_SelectM*14+5, 138, 14};
		pdc->ReverseRect(rect);
		pdc->DrawHLine(0, 122, 160);
		pdc->DrawString(20, 126, "                      ");
		if (m_strBottomInfo.empty() == false)
			pdc->DrawString(20, 126, m_strBottomInfo.c_str());
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
			sprintf(buffer, "测试第%d项......", pData->m_StartM + pData->m_SelectM + 1);
			m_strBottomInfo.assign(buffer);
			m_ManualItems[pData->m_StartM + pData->m_SelectM].m_Result = TEST_RESULT_STANDBY;
			PostWndMsg(GM_PAINT, 0x00, 0x00);
			PostWndMsg(GM_MANUAL_TEST, pData->m_StartM + pData->m_SelectM, 0x00);
		}
		break;
	case GM_MANUAL_TEST:{
		string strMsg;
		if (lParam&0x80000000) strMsg.push_back((S_BYTE)lParam);//确认结果传入
		switch (pTestObj->ExecuteTestItem(false, m_ManualItems[wParam], strMsg))
		{
		case TEST_EXEC_MSG:
			m_strBottomInfo = strMsg;PostWndMsg(GM_PAINT, 0x00, 0x00);
		case TEST_EXEC_CONTINUE:
			PostWndMsg(GM_MANUAL_TEST, wParam, 0x00); break;
		case TEST_EXEC_REQ_OK:
			if (SGui_MsgBox("确认", "    测试合格?", MB_BTN_OKCANCEL) == MB_RTN_YESOK) lParam = 1;
			else lParam = 0;
			PostWndMsg(GM_MANUAL_TEST, wParam, lParam | 0x80000000); break;
		case TEST_EXEC_ERROR:
		case TEST_EXEC_FINISH:
			m_strBottomInfo = "测试结束!!!";
			PostWndMsg(GM_RESTORE, 0x00, 0x00);PostWndMsg(GM_PAINT, 0x00, 0x00);break;
		default:break;
		}
		}break;
	case GM_RESTORE:{
		S_TestNames AutoItems;
		m_ManualItems.clear();
		pTestObj->GetAutoTestItemName(AutoItems, m_ManualItems);
		}break;
	case GM_AUTOBACK:
		return -1;
	case GM_DESTROY:
		ReleaseWndData();
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

static const char *g_ResetTestItem[] = {"重测全部项", "重测自动项", "重测手动项"};
int C_ResetTestWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	S_MenuCtrl *pData = static_cast<S_MenuCtrl *>(GetWndData(sizeof(S_MenuCtrl)));
	switch (msg)
	{
	case GM_CREATE:
		pData->m_SelectM = 0;
		pData->m_MenuCnt = (int)sizeof(g_ResetTestItem)/sizeof(g_ResetTestItem[0]);
		pData->m_InfoIndex = 0;
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		for (int i = 0; i < pData->m_MenuCnt; i++)
		{
			sprintf(buffer, "%d、%s", i+1, g_ResetTestItem[i]);
			pdc->DrawString(14, i*18+8, buffer);
		}
		S_RECT rect = {12, pData->m_SelectM*18+6, 138, 16};
		pdc->ReverseRect(rect);
		pdc->DrawHLine(0, 122, 160);
//		pdc->DrawString(20, 126, g_RebootWndInfo[pData->m_InfoIndex]);
		EndPaint(pdc);
		}
		break;
	case GM_KEYUP:
		if (wParam == GVK_DN)
		{
			pData->m_SelectM++;
			if (pData->m_SelectM >= pData->m_MenuCnt)
				pData->m_SelectM = 0;
			InvalidateRect(NULL);
		}
		else if (wParam == GVK_UP)
		{
			pData->m_SelectM--;
			if (pData->m_SelectM <= -1)
				pData->m_SelectM = pData->m_MenuCnt-1;
			InvalidateRect(NULL);
		}
		else if (wParam == GVK_CANCEL)
		{
			RETURNWND();
		}
		else if (wParam == GVK_OK)
		{
			if (SGui_MsgBox("确认", "    重测选中项?", MB_BTN_OKCANCEL) != MB_RTN_YESOK)
				break;
			C_FactAutoTest *pTestObj = C_AutoTestFactory::GetFactoryAutoTestObj();
			pTestObj->ClearTestMark((S_TestItemType)pData->m_SelectM);
		}
		break;
	case GM_AUTOBACK:
		return -1;
	case GM_DESTROY:
		ReleaseWndData();
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

int C_NameReportWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	switch (msg)
	{
	case GM_CREATE:{
		string strReportName;
		S_RECT NumRect = {50, 50, 56, 16};
		S_RECT SetRect = {60, 100, 32, 16};
		C_NUMEDIT *pMpEdit = C_NUMEDIT::NewCtrl();
		C_FactAutoTest *pTestObj = C_AutoTestFactory::GetFactoryAutoTestObj();
		pTestObj->NameAutoTestReport(strReportName);
		pMpEdit->Create(strReportName.c_str(), NUMEDIT_STYLE_HEX, NumRect, this, 1);
		C_BUTTON *pbtnSet = C_BUTTON::NewCtrl();
		pbtnSet->Create("确定", 0x00, SetRect, this, 2);
		SetFocusCtrl(pMpEdit);
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->DrawString(16, 1*16+6, "报告名称:");
		pdc->DrawHLine(0, 122, 160);
		EndPaint(pdc);
		}
		break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
			RETURNWND();
		else if (wParam == GVK_RIGHT || wParam == GVK_OK)
			SetFocusCtrl(GetWndCtrl(2));
		else if (wParam == GVK_LEFT)
			SetFocusCtrl(GetWndCtrl(1));
		break;
	case GM_CMD:
		if (wParam == 1 && lParam == GM_CMD_CTRL_UPDATE)
		{
		}
		else if (wParam == 2 && lParam == GM_CMD_BTN_PUSHED)
		{
			string strSN = GetWndCtrl(1)->GetWndText();
			C_FactAutoTest *pTestObj = C_AutoTestFactory::GetFactoryAutoTestObj();
			pTestObj->NameAutoTestReport(strSN);
			GOTOWND(C_MakeReportWnd, 0x00);
		}
		break;
	case GM_AUTOBACK:
		return -1;
	case GM_DESTROY:
		DeleteWndCtrl(1, 2);
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

static const char *g_MakeReportWndTitle[] = {"请插入U盘...", "报告拷贝结束"};
static const char *g_MakeReportWndInfo[] = {"检测U盘.....", "检测U 盘成功", "生成报告成功", "报告命名失败", "检测报告失败", "报告拷贝失败"};
int C_MakeReportWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	S_MenuCtrl *pData = static_cast<S_MenuCtrl *>(GetWndData(sizeof(S_MenuCtrl)));
	switch (msg)
	{
	case GM_CREATE:{
		pData->m_InfoIndex = 0;
		pData->m_SelectM = 0;
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		pdc->DrawString(32, 2*16+6, g_MakeReportWndTitle[pData->m_SelectM]);
		pdc->DrawHLine(0, 122, 160);
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F12x12));
		pdc->DrawString(20, 126, g_MakeReportWndInfo[pData->m_InfoIndex]);
		EndPaint(pdc);
		}
		break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
			RETURNWND();
		break;
	case GM_TIME:
		if (pData->m_InfoIndex == 0)
		{
			char FlagFile[260];
			C_USBPATH::GetUsbTestFile(TEST_USB_FLAG, FlagFile);
			if (access(FlagFile, F_OK)  != 0)//Check Usb Disk
				break;
			pData->m_InfoIndex = 1;
			InvalidateRect(NULL);
		}
		else if (pData->m_InfoIndex == 1)
		{
			string strReportName;
			C_FactAutoTest *pTestObj = C_AutoTestFactory::GetFactoryAutoTestObj();
			int ok = pTestObj->CreateAutoTestReport(strReportName, C_USBPATH::GetUsbTestPath());
			if (ok == 0) pData->m_InfoIndex = 2;
			else if (ok == -1) pData->m_InfoIndex = 3;
			else if (ok == -2) pData->m_InfoIndex = 4;
			else if (ok == -3) pData->m_InfoIndex = 5;
			pData->m_SelectM = 1;

			string strMount("umount ");
			strMount.append(C_USBPATH::GetUsbRoot());
			SystemCmd(strMount.c_str());
			InvalidateRect(NULL);
		}
		break;
	case GM_AUTOBACK:
		return -1;
	case GM_DESTROY:
		ReleaseWndData();
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

