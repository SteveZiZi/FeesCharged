/** @file
 *  @brief 参数显示窗口定义
 *  @author  yezhengqiu
 *  @date 2012/02/21
 *  @version 0.1
 */
#include "menuwnd.h"
#include "sguiapp.h"
#include "sguictrl.h"
#include "sguidlog.h"
#include "timeop.h"
#include "status.h"
#include "paramwnd.h"
#include "nwmsrule.h"
#include "connfact.h"
#include "sysctrl.h"
#include "cfg_path.h"
#include "context.h"

//通讯通道设置
const char *g_CommunMenu[] = {"主站通道1", "主站通道2", "主站通道3", "PPP参数配置", "本地网络配置", "通讯参数设置"};
int C_CommunMenuWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	S_MenuCtrl *pData = static_cast<S_MenuCtrl *>(GetWndData(sizeof(S_MenuCtrl)));
	switch (msg)
	{
	case GM_CREATE:
		pData->m_SelectM = 0;
		pData->m_MenuCnt = (int)sizeof(g_CommunMenu)/sizeof(g_CommunMenu[0]);
		pData->m_InfoIndex = 0;
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		for (int i = 0; i < pData->m_MenuCnt; i++)
		{
			sprintf(buffer, "%s",  g_CommunMenu[i]);
			pdc->DrawString(STRINGX, i*18+8, buffer);
		}
		S_RECT rect = {STRINGX, pData->m_SelectM*18+8, STRINGW, 16};
		pdc->ReverseRect(rect);
		pdc->DrawHLine(0, 122, 160);
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
			if (pData->m_SelectM == 0) GOTOWND(C_ChannelViewWnd, 0x01);
			else if (pData->m_SelectM == 1) GOTOWND(C_ChannelViewWnd, 0x02);
			else if (pData->m_SelectM == 2) GOTOWND(C_ChannelViewWnd, 0x03);
			else if (pData->m_SelectM == 3) GOTOWND(C_PppViewWnd, 0x00);
			else if (pData->m_SelectM == 4) GOTOWND(C_LocalNetViewWnd, 0x00);			
			else if (pData->m_SelectM == 5) GOTOWND(C_ChannelsParamViewWnd, 0x00);//有待添加心跳间隔参数的设置
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

//测量点参数
int C_MeterParamViewWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	S_WndMeter *pData = static_cast<S_WndMeter *>(GetWndData(sizeof(S_WndMeter)));
	switch (msg)
	{
	case GM_CREATE:{
		memset(pData, 0x00, sizeof(*pData));
		S_RECT NumRect = {48, 0*17+2, 30, 16};
		S_RECT SetRect = {84, 0*17+2, 32, 16};
		S_RECT DetailRect = {122, 0*17+2, 32, 16};
		C_NUMEDIT *pMpEdit = C_NUMEDIT::NewCtrl();
		pMpEdit->Create("0000", NUMEDIT_STYLE_DEC, NumRect, this, 1);
		C_BUTTON *pbtnSet = C_BUTTON::NewCtrl();
		pbtnSet->Create("设置", 0x00, SetRect, this, 2);
		C_BUTTON *pbtnDetail = C_BUTTON::NewCtrl();
		pbtnDetail->Create("详细", 0x00, DetailRect, this, 3);
		SetFocusCtrl(pMpEdit);
		}break;
	case GM_RESTORE:{
		string strMP = GetWndCtrl(1)->GetWndText();
		int MP = atoi(strMP.c_str());
		S_MpInfos *pMpInfos = STATUS.GetMpInfos();
		if (pMpInfos->Test(MP, MP_TYPE_METER) == true)
			MP = pMpInfos->m_Item[MP].m_SN;
		else
			MP = 0;
		S_MeterInfos *pInfos = STATUS.GetMeterInfos();
		memset(pData, 0x00, sizeof(*pData));
		if (MP > 0 && MP < CONS_METER_COUNT_MAX)
			UpdateMeterInfo(pData, &pInfos->m_Item[MP]);
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		pdc->DrawString(STRINGX, 0*17+2, "测量点");
		sprintf(buffer, "状态  :%s", pData->m_Status);
		pdc->DrawString(STRINGX, 1*17+2, buffer);
		sprintf(buffer, "类型  :%s", pData->m_Function);
		pdc->DrawString(STRINGX, 2*17+2, buffer);
		sprintf(buffer, "地址  :%s", pData->m_Address);
		pdc->DrawString(STRINGX, 3*17+2, buffer);
		sprintf(buffer, "规约  :%s", pData->m_Rule);
		pdc->DrawString(STRINGX, 4*17+2, buffer);
		sprintf(buffer, "端口  :%s", pData->m_Port);
		pdc->DrawString(STRINGX, 5*17+2, buffer);
		sprintf(buffer, "采集器:%s", pData->m_Relay);
		pdc->DrawString(STRINGX, 6*17+2, buffer);
		pdc->DrawHLine(0, 122, 160);
		EndPaint(pdc);
		}
		break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
		{
			RETURNWND();
		}
		else if (wParam == GVK_RIGHT || wParam == GVK_OK)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			if (pCtrl->GetCtrlID() == 3)
				SetFocusCtrl(GetWndCtrl(1));
			else
				SetFocusCtrl(GetWndCtrl(pCtrl->GetCtrlID()+1));
		}
		else if (wParam == GVK_LEFT)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			if (pCtrl->GetCtrlID() == 1)
				SetFocusCtrl(GetWndCtrl(3));
			else
				SetFocusCtrl(GetWndCtrl(pCtrl->GetCtrlID()-1));
		}
		break;
	case GM_CMD:
		if (wParam == 1 && lParam == GM_CMD_CTRL_UPDATE)
		{
			string strMP = GetWndCtrl(1)->GetWndText();
			int MP = atoi(strMP.c_str());
			S_MpInfos *pMpInfos = STATUS.GetMpInfos();
			if (pMpInfos->Test(MP, MP_TYPE_METER) == true)
				MP = pMpInfos->m_Item[MP].m_SN;
			else
				MP = 0;
			S_MeterInfos *pInfos = STATUS.GetMeterInfos();
			memset(pData, 0x00, sizeof(*pData));
			if (MP > 0 && MP < CONS_METER_COUNT_MAX)
				UpdateMeterInfo(pData, &pInfos->m_Item[MP]);
			InvalidateRect(NULL);
		}
		else if (wParam == 2 && lParam == GM_CMD_BTN_PUSHED)
		{
			C_SGUIDC dc(this);
			string strMP = GetWndCtrl(1)->GetWndText();
			int MP = atoi(strMP.c_str());
			S_MpInfos *pMpInfos = STATUS.GetMpInfos();
			if (pMpInfos->Test(MP, MP_TYPE_METER, MP) == false)
			{
				dc.DrawString(20, 126, "测量点被占用  ");
			}
			else if (MP > 0 && MP < CONS_METER_COUNT_MAX)
			{
				if (C_PWD::Certification() == true)
					GOTOWND(C_MeterParamSetWnd, (S_DWORD)MP);
			}
			else
			{
				dc.DrawString(20, 126, "测量点不合要求");
			}
		}
		else if (wParam == 3 && lParam == GM_CMD_BTN_PUSHED)
		{
			string strMP = GetWndCtrl(1)->GetWndText();
			int MP = atoi(strMP.c_str());
			GOTOWND(C_MeterDetailViewWnd, (S_DWORD)MP);
		}
		break;
	case GM_DESTROY:
		ReleaseWndData();
		DeleteWndCtrl(1, 3);
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;	
}

#define METER_PORT_NAME_485I		"485I  "
#define METER_PORT_NAME_485II		"485II "
#define METER_PORT_NAME_485III	"485III"
#define METER_PORT_NAME_ZB		" 载波 "
#define METER_PORT_NAME_WX		" 无线 "
static const S_CFGITEM s_MeterPortItem[] =
{
	{0x00, METER_PORT_NAME_485I},
	{0x01, METER_PORT_NAME_485II},
	{0x02, METER_PORT_NAME_485III},
	{0x1F, METER_PORT_NAME_ZB},
	{0x20, METER_PORT_NAME_WX}
};

const char *g_MeterParam_status[] = {"无效", "有效"};
const char *g_MeterParam_function[] = {" 单相表 ", "多功能表", "其它类型"};
bool C_MeterParamViewWnd::UpdateMeterInfo(S_WndMeter *pData, const S_MeterInfo *pMeterInfo)
{
	char buffer[32];
	if (pMeterInfo->Existed() == false)
		return false;

	C_CfgItem itemPort(s_MeterPortItem, KGE_MEMBER_OF(s_MeterPortItem));
	S_MeterInfo MeterInfo = *pMeterInfo;
	S_MpInfos *pMpInfos = STATUS.GetMpInfos();
	S_MpInfo &MpInfo = pMpInfos->m_Item[MeterInfo.m_MeasureNo];

	MeterInfo.m_FunType -= 1;
	if (MeterInfo.m_FunType > 2)
		MeterInfo.m_FunType = 0;
	if (MeterInfo.m_PortSet.m_PortNO > 0)
		MeterInfo.m_PortSet.m_PortNO -= 1;

	strcpy(pData->m_Status, g_MeterParam_status[MpInfo.m_AutoGather]);
	strcpy(pData->m_Function, g_MeterParam_function[MeterInfo.m_FunType]);
	memset(buffer, 0x00, sizeof(buffer));
	strcpy(pData->m_Address, MeterInfo.GetMeterAddress(buffer));
	strcpy(pData->m_Rule, C_NwMRuleName::MakeMeterRuleName(MeterInfo.m_RuleID));
	memset(buffer, 0x00, sizeof(buffer));
	strcpy(pData->m_Relay, MeterInfo.GetRelayAddress(buffer));
	strcpy(pData->m_Port, itemPort.MakeKey(MeterInfo.m_PortSet.m_PortNO));
	return true;
}

int C_MeterDetailViewWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	S_MenuScrollCtrl *pData = static_cast<S_MenuScrollCtrl *>(GetWndData(sizeof(S_MenuScrollCtrl)));
	switch (msg)
	{
	case GM_CREATE:
		pData->m_InfoIndex = 0;
		pData->m_LineOfScreen = 7;
		pData->m_SelectM = 0;
		pData->m_StartM = 0;
		pData->m_MenuCnt = PrintMeterParamDetail((S_WORD)lParam);
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		for (int i = 0; i < pData->m_LineOfScreen; i++)
		{
			if (i + pData->m_StartM < pData->m_MenuCnt)
				pdc->DrawString(STRINGX, i*17+2, m_strListData[i + pData->m_StartM].c_str());
		}
		pdc->DrawHLine(0, 122, 160);
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F12x12));
		pdc->DrawString(20, 126, m_BottomInfo);
		EndPaint(pdc);
		}break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL || wParam == GVK_OK)
		{
			RETURNWND();
		}
		else if (wParam == GVK_DN || wParam == GVK_RIGHT)
		{
			if (pData->m_StartM + pData->m_LineOfScreen < pData->m_MenuCnt)
			{
				pData->m_StartM += pData->m_LineOfScreen;
				if (pData->m_StartM + pData->m_LineOfScreen > pData->m_MenuCnt)
					pData->m_StartM = pData->m_MenuCnt - pData->m_LineOfScreen;
				InvalidateRect(NULL);
			}
		}
		else if (wParam == GVK_UP || wParam == GVK_LEFT)
		{
			if (pData->m_StartM > 0)
			{
				pData->m_StartM -= pData->m_LineOfScreen;
				if (pData->m_StartM < 0) pData->m_StartM = 0;
				InvalidateRect(NULL);
			}
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
const char *g_MeterParam_attribute[] = {"  485表 ", " 模拟量 ", " 脉冲量 ", " 计算值 ", "交流采样"};
const char *g_MeterParam_main[] = {"分表", "总表"};
const char *g_MeterParam_vip[] = {"普通用户", "重点用户"};
const char *g_MeterParam_normal[] = {"无", "有"};
const char *g_MeterParam_stopbit[] = {"1位", "2位", "1.5位"};
const char *g_MeterParam_parity[] = {"无校验", "奇校验", "偶校验"};
S_WORD C_MeterDetailViewWnd::PrintMeterParamDetail(S_WORD MP)
{
	sprintf(m_BottomInfo, "当前测量点:%04d", MP);

	S_MpInfos *pMpInfos = STATUS.GetMpInfos();
	if (pMpInfos->Test(MP, MP_TYPE_METER) == false)
	{
		return 0;
	}
	else if (MP > CONS_METER_COUNT_MAX)
	{
		return 0;
	}
 
	char buffer[64], address[32];
	C_CfgItem itemPort(s_MeterPortItem, KGE_MEMBER_OF(s_MeterPortItem));
	S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
	S_MeterInfo MeterInfo = pMeterInfos->m_Item[pMpInfos->m_Item[MP].m_SN];
	if (MeterInfo.Existed() == false)
		return 0;

	MeterInfo.m_FunType -= 1;
	if (MeterInfo.m_FunType > 2)
		MeterInfo.m_FunType = 0;
	if (MeterInfo.m_PortSet.m_PortNO > 0)
		MeterInfo.m_PortSet.m_PortNO -= 1;

	S_MpInfo &MpInfo = pMpInfos->m_Item[MP];
	sprintf(buffer, "状态:%s", g_MeterParam_status[MpInfo.m_AutoGather]);
	m_strListData.push_back(buffer);
	sprintf(buffer, "性质:%s", g_MeterParam_attribute[MpInfo.m_Type - 1]);
	m_strListData.push_back(buffer);
	memset(address, 0x00, sizeof(address));
	sprintf(buffer, "地址:%s", MeterInfo.GetMeterAddress(address));
	m_strListData.push_back(buffer);
	sprintf(buffer, "规约:%s", C_NwMRuleName::MakeMeterRuleName(MeterInfo.m_RuleID));
	m_strListData.push_back(buffer);
	sprintf(buffer, "类型:%s", g_MeterParam_function[MeterInfo.m_FunType]);
	m_strListData.push_back(buffer);
	sprintf(buffer, "总分:%s", g_MeterParam_main[MeterInfo.m_MainFlag]);
	m_strListData.push_back(buffer);
	sprintf(buffer, "重点户:%s", g_MeterParam_vip[MeterInfo.m_VipFlag]);
	m_strListData.push_back(buffer);
	sprintf(buffer, "拉闸功能:%s", g_MeterParam_normal[MeterInfo.m_PowerCut]);
	m_strListData.push_back(buffer);
	sprintf(buffer, "费率数:%d", MeterInfo.m_TariffCnt);
	m_strListData.push_back(buffer);
	memset(address, 0x00, sizeof(address));
	sprintf(buffer, "采集器:%s", MeterInfo.GetRelayAddress(address));
	m_strListData.push_back(buffer);
	sprintf(buffer, "端口号:%s", itemPort.MakeKey(MeterInfo.m_PortSet.m_PortNO));
	m_strListData.push_back(buffer);
	sprintf(buffer, "波特率:%d", MeterInfo.m_PortSet.m_Baud);
	m_strListData.push_back(buffer);
	sprintf(buffer, "校验方式:%s", g_MeterParam_parity[MeterInfo.m_PortSet.m_Parity]);
	m_strListData.push_back(buffer);
	sprintf(buffer, "数据位:%d", MeterInfo.m_PortSet.m_DataBit);
	m_strListData.push_back(buffer);
	sprintf(buffer, "停止位:%s", g_MeterParam_stopbit[MeterInfo.m_PortSet.m_StopBit-1]);
	m_strListData.push_back(buffer);
	sprintf(buffer, "CT变比:%d", MeterInfo.m_CT);
	m_strListData.push_back(buffer);
	sprintf(buffer, "PT变比:%d", MeterInfo.m_PT);
	m_strListData.push_back(buffer);

	return (S_WORD)m_strListData.size();
}

//终端时间设置
int C_TerminalTimeViewWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	switch (msg)
	{
	case GM_CREATE:{
		C_BUTTON *pbtnSet = C_BUTTON::NewCtrl();
		S_RECT rect = BTNRECT(60,LINEXY(5));
		pbtnSet->Create("设置", 0x00, rect, this, 1);
		SetFocusCtrl(pbtnSet);
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		pdc->DrawString(STRINGX, LINEXY(0), "终端时间");
		S_DATE date;
		C_TIME::LocalTime(C_TIME::TimeNow(), date);
		sprintf(buffer, "日期: %04d-%02d-%02d", date.m_Year, date.m_Mon, date.m_Day);
		pdc->DrawString(STRINGX, LINEXY(1), buffer);
		sprintf(buffer, "时间: %02d:%02d:%02d", date.m_Hour, date.m_Min, date.m_Sec);
		pdc->DrawString(STRINGX, LINEXY(2), buffer);
		pdc->DrawHLine(0, 122, 160);
		EndPaint(pdc);
		}
		break;
	case GM_TIME:
		InvalidateRect(NULL);
		break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
			RETURNWND();
		break;
	case GM_CMD:
		if (wParam == 1 && lParam == GM_CMD_BTN_PUSHED)
		{
			if (C_PWD::Certification() == true)
				GOTOWND(C_TermTimeSetWnd, 0x00);
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

//界面密码设置
const char *g_PasswordWndInfo[] = {"               ", "密码校验错误!!!"};
int C_PasswordWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	S_MenuCtrl *pData = static_cast<S_MenuCtrl *>(GetWndData(sizeof(S_MenuCtrl)));
	switch (msg)
	{
	case GM_CREATE:{
		pData->m_InfoIndex = 0;
		S_RECT NumRect = {108, 1*20+6, 48, 16};
		C_NUMEDIT *pOldPwd = C_NUMEDIT::NewCtrl();
		pOldPwd->Create("999999", NUMEDIT_STYLE_DEC, NumRect, this, 1);
		NumRect.y += LINEHEIGHT;
		C_NUMEDIT *pNewPwd = C_NUMEDIT::NewCtrl();
		pNewPwd->Create("999999", NUMEDIT_STYLE_DEC, NumRect, this, 2);
		C_BUTTON *pbtnSet = C_BUTTON::NewCtrl();
		S_RECT SetRect = BTNRECT(60,LINEXY(5));//{0, 90, 32, 16};
		pbtnSet->Create("确定", 0x00, SetRect, this, 3);
		SetFocusCtrl(pOldPwd);
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		pdc->DrawString(STRINGX, LINEXY(0), "密码设置");
		pdc->DrawString(STRINGX, LINEXY(1), "请输入旧密码:");
		pdc->DrawString(STRINGX, LINEXY(2), "请输入新密码:");
		pdc->DrawHLine(0, 122, 160);
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F12x12));
		pdc->DrawString(20, 126, g_PasswordWndInfo[pData->m_InfoIndex]);
		EndPaint(pdc);
		}
		break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
			RETURNWND();
		else if (wParam == GVK_UP)
			SetFocusCtrl(GetWndCtrl(1));
		else if (wParam == GVK_OK)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			if (pCtrl->GetCtrlID() == 1)
				SetFocusCtrl(GetWndCtrl(2));
			else if (pCtrl->GetCtrlID() == 2)
				SetFocusCtrl(GetWndCtrl(3));
		}
		break;
	case GM_CMD:
		if (wParam == 3 && lParam == GM_CMD_BTN_PUSHED)
		{
			S_LcdInfo *pLcdInfo = STATUS.GetLcdInfo();
			string strOldPwd = GetWndCtrl(1)->GetWndText();
			int OldPwd = atoi(strOldPwd.c_str());
			if (OldPwd == pLcdInfo->m_LCDPassword)
			{
				char buffer[64];
				string strNewPwd = GetWndCtrl(2)->GetWndText();
				sprintf(buffer, "  密码设为:%s", strNewPwd.c_str());
				if (SGui_MsgBox("提示", buffer, MB_BTN_OKCANCEL) == MB_RTN_YESOK)
				{
					pLcdInfo->m_LCDPassword = atoi(strNewPwd.c_str());
					STATUS.SaveLcdInfo();
				}
				pData->m_InfoIndex = 0;
			}
			else
			{
				pData->m_InfoIndex = 1;
				SetFocusCtrl(GetWndCtrl(1));
			}
		}
		break;
	case GM_DESTROY:
		ReleaseWndData();
		DeleteWndCtrl(1, 3);
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;	
}

//终端编号设置
int C_RtuaViewWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	switch (msg)
	{
	case GM_CREATE:{
		C_BUTTON *pbtnSet = C_BUTTON::NewCtrl();
		S_RECT rect = BTNRECT(60,LINEXY(5));
		pbtnSet->Create("设置", 0x00, rect, this, 1);
		SetFocusCtrl(pbtnSet);
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		S_SYSCFG *pSystemConfig = STATUS.GetSystemConfig();
		C_RTUA rtua(pSystemConfig->m_RTUA);
		pdc->DrawString(STRINGX, LINEXY(0), "终端编号");
		sprintf(buffer, "地市区县码: %06X", rtua.GetAreaCode());
		pdc->DrawString(STRINGX, LINEXY(1), buffer);
		sprintf(buffer, "终 端地 址: %06XH", rtua.GetDevAddr());
		pdc->DrawString(STRINGX, LINEXY(2), buffer);

		pdc->DrawHLine(0, 122, 160);
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
			if (C_PWD::Certification() == true)
				GOTOWND(C_RtuaSetWnd, 0x00);
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

//级联参数查询、设置
int C_ForwardParamViewWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	switch (msg)
	{
	case GM_CREATE:
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		pdc->DrawString(STRINGX, LINEXY(1), "级联功能: 不支持");
		pdc->DrawHLine(0, 122, 160);
		EndPaint(pdc);
		}
		break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
			RETURNWND();
		break;
	case GM_DESTROY:
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;	
}

//终端限值参数查询与设置
int C_ThresholdViewWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	switch (msg)
	{
	case GM_CREATE:
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));

		S_HttxInfo *pHttxInfo=STATUS.GetHttxInfo(MP_HTTX_1);
		sprintf(buffer, "月流量限值:%dkB", pHttxInfo->m_MonTrafficLimit/1024);		
		pdc->DrawString(STRINGX, LINEXY(0), "终端");
		pdc->DrawString(STRINGX, LINEXY(1), buffer);

		S_PowerLimit *Lmit = STATUS.GetPowerLimitInfo();
		sprintf(buffer, "时钟超差限值:%d分钟", Lmit->m_MeterTimeDiffM);		
		pdc->DrawString(STRINGX, LINEXY(2), "电能表");
		pdc->DrawString(STRINGX, LINEXY(3), buffer);
		
		pdc->DrawHLine(0, 122, 160);
		EndPaint(pdc);
		}
		break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
			RETURNWND();
		break;
	case GM_DESTROY:
//		DeleteWndCtrl(1, 1);
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;	
}

// 调试菜单
const char *g_DebugMenu[] = {"485测试通道", "本地模块路由"};
int C_DebugMenuWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	S_MenuCtrl *pData = static_cast<S_MenuCtrl *>(GetWndData(sizeof(S_MenuCtrl)));
	switch (msg)
	{
	case GM_CREATE:
		pData->m_SelectM = 0;
		pData->m_MenuCnt = (int)sizeof(g_DebugMenu)/sizeof(g_DebugMenu[0]);
		pData->m_InfoIndex = 0;
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		for (int i = 0; i < pData->m_MenuCnt; i++)
		{
			sprintf(buffer, "%s",  g_DebugMenu[i]);
			pdc->DrawString(STRINGX, i*18+8, buffer);
		}
		S_RECT rect = {STRINGX, pData->m_SelectM*18+8, STRINGW, 16};
		pdc->ReverseRect(rect);
		pdc->DrawHLine(0, 122, 160);
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
			if (pData->m_SelectM == 0) GOTOWND(C_TestComViewWnd, 0x01);
			else if (pData->m_SelectM == 1) GOTOWND(C_DnModuleViewWnd, 0x02);
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

//终端测试通道设置									依据消息，含义不同，
int C_TestComViewWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	switch (msg)
	{
	case GM_CREATE:{
		m_ChannelType = 0x00; // 空闲
		C_BUTTON *pbtnSet = C_BUTTON::NewCtrl();
		S_RECT rect = BTNRECT(60,LINEXY(5));
		pbtnSet->Create("设置", 0x00, rect, this, 1);
		SetFocusCtrl(pbtnSet);
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		if(C_SYSCTRL::GetProcessEnable(MP_PROCESS_HTTX4))
			m_ChannelType = 0x01, sprintf(buffer, "测试通道:上行测试");
		else if(C_SYSCTRL::GetProcessEnable(MP_PROCESS_DBCJ3))
			m_ChannelType = 0x02, sprintf(buffer, "测试通道:下行抄表");
		else
			m_ChannelType = 0x00, sprintf(buffer, "测试通道:空闲");	
		pdc->DrawString(STRINGX, LINEXY(0), buffer);

		pdc->DrawHLine(0, 122, 160);
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
			if (C_PWD::Certification() == true)
				GOTOWND(C_TestComSetWnd, m_ChannelType);
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

//通道设置窗口
const char *g_pComType = "  空闲  ;上行测试;下行采表";
int C_TestComSetWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	switch (msg)
	{
	case GM_CREATE:{
		S_RECT BoxRect =  {80, LINEXY(0), 66, 16};

		C_SELECTBOX *pBox = C_SELECTBOX::NewCtrl();
		pBox->Create(g_pComType, 0x00, BoxRect, this, 1);
		pBox->SetDefaultItem(lParam);

		C_BUTTON *pbtnSet = C_BUTTON::NewCtrl();
		S_RECT rect = BTNRECT(60,LINEXY(5));
		pbtnSet->Create("确定", 0x00, rect, this, 2);
		SetFocusCtrl(GetWndCtrl(1));
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		pdc->DrawString(STRINGX, LINEXY(0), "测试通道:");
		pdc->DrawHLine(0, 122, 160);
		EndPaint(pdc);
		}break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
			RETURNWND();
		else if (wParam == GVK_UP)
			SetFocusCtrl(GetWndCtrl(1));
		else if (wParam == GVK_OK || wParam == GVK_RIGHT)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			int id = pCtrl->GetCtrlID() + 1;
			if (id > 2) id = 1;
			SetFocusCtrl(GetWndCtrl(id));
		}
		else if (wParam == GVK_LEFT)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			int id = pCtrl->GetCtrlID() - 1;
			if (id <= 0) id = 2;
			SetFocusCtrl(GetWndCtrl(id));
		}
		break;
	case GM_CMD:
		if (wParam == 2 && lParam == GM_CMD_BTN_PUSHED)
		{
			S_DWORD ChannelType = 0;			
			string strData;
			strings strChannelName;

			strChannelName.split(g_pComType, ';');
			strData = GetWndCtrl(1)->GetWndText();
//			string::size_type itero = strData.find(' ');
//			strData= strData.substr(0,itero);
			strings::iterator iter = find(strChannelName.begin(), strChannelName.end(), strData);
			if (iter == strChannelName.end())
				ChannelType=0;
			ChannelType = (S_DWORD)(iter - strChannelName.begin());

			if (SGui_MsgBox("提示", " 确定更改通道参数?", MB_BTN_OKCANCEL) == MB_RTN_YESOK)
			{
				SaveInfos(ChannelType);
//				RETURNWND();
			}
		}
		break;
	case GM_DESTROY:
		ReleaseWndData();
		DeleteWndCtrl(1, 2);
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;	
}

void C_TestComSetWnd::SaveInfos(S_DWORD type)
{
	if(type==0)//设置为空闲
	{
		C_SYSCTRL::EnableProcess(MP_PROCESS_HTTX4,false);
		C_SYSCTRL::EnableProcess(MP_PROCESS_DBCJ3,false);

	}
	else if(type==1)//设置为上行测试通道
	{
			C_SYSCTRL::EnableProcess(MP_PROCESS_HTTX4,true);
			C_SYSCTRL::EnableProcess(MP_PROCESS_DBCJ3,false);
	}
	else if(type==2)//设置为下行采表通道
	{
		C_SYSCTRL::EnableProcess(MP_PROCESS_HTTX4,false);
		C_SYSCTRL::EnableProcess(MP_PROCESS_DBCJ3,true);
	}

	STATUS.SaveHttxInfo();
	STATUS.SaveDbcjInfo();
}

// 本地通信模块模式
int C_DnModuleViewWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	switch (msg)
	{
	case GM_CREATE:{
		C_BUTTON *pbtnSet = C_BUTTON::NewCtrl();
		S_RECT rect = BTNRECT(60,LINEXY(5));
		pbtnSet->Create("设置", 0x00, rect, this, 1);
		SetFocusCtrl(pbtnSet);
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		S_SYSCFG *pSysCfg = STATUS.GetSystemConfig();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		sprintf(buffer, "东软路由模式: %d 代", pSysCfg->m_DrRouteMode);
		pdc->DrawString(STRINGX, LINEXY(0), buffer);
		pdc->DrawHLine(0, 122, 160);
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
			if (C_PWD::Certification() == true)
				GOTOWND(C_DnModuleSetWnd, 0x00);
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

// 本地通信模块模式
const char *g_pDrRouteMode = " 3代模式; 4代模式";
int C_DnModuleSetWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	switch (msg)
	{
	case GM_CREATE:{
		S_RECT BoxRect =  {80, LINEXY(0), 66, 16};
		S_SYSCFG *pSysCfg = STATUS.GetSystemConfig();

		C_SELECTBOX *pBox = C_SELECTBOX::NewCtrl();
		pBox->Create(g_pDrRouteMode, 0x00, BoxRect, this, 1);
		pBox->SetDefaultItem((S_DWORD)(pSysCfg->m_DrRouteMode - 3));

		C_BUTTON *pbtnSet = C_BUTTON::NewCtrl();
		S_RECT rect = BTNRECT(60,LINEXY(5));
		pbtnSet->Create("确定", 0x00, rect, this, 2);
		SetFocusCtrl(GetWndCtrl(1));
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		pdc->DrawString(STRINGX, LINEXY(0), "东软路由:");
		pdc->DrawHLine(0, 122, 160);
		EndPaint(pdc);
		}break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
			RETURNWND();
		else if (wParam == GVK_UP)
			SetFocusCtrl(GetWndCtrl(1));
		else if (wParam == GVK_OK || wParam == GVK_RIGHT)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			int id = pCtrl->GetCtrlID() + 1;
			if (id > 2) id = 1;
			SetFocusCtrl(GetWndCtrl(id));
		}
		else if (wParam == GVK_LEFT)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			int id = pCtrl->GetCtrlID() - 1;
			if (id <= 0) id = 2;
			SetFocusCtrl(GetWndCtrl(id));
		}
		break;
	case GM_CMD:
		if (wParam == 2 && lParam == GM_CMD_BTN_PUSHED)
		{
			S_DWORD DrRouteMode = 0;			
			string strData;
			strings strsDrRouteMode;

			strsDrRouteMode.split(g_pDrRouteMode, ';');
			strData = GetWndCtrl(1)->GetWndText();
			strings::iterator iter = find(strsDrRouteMode.begin(), strsDrRouteMode.end(), strData);
			if (iter == strsDrRouteMode.end())
				DrRouteMode = 0;
			DrRouteMode = (S_DWORD)(iter - strsDrRouteMode.begin()) + 3;

			if (SGui_MsgBox("提示", " 确定更改路由参数?", MB_BTN_OKCANCEL) == MB_RTN_YESOK)
			{
				S_SYSCFG *pSysCfg = STATUS.GetSystemConfig();
				pSysCfg->m_DrRouteMode = (int)DrRouteMode;
				STATUS.SaveSystemConfig();

				S_DbcjStatus *pDbcjStatus = CONTEXT.GetDbcjStatus(MP_DBCJ_4);
				pDbcjStatus->m_timeRouteError = C_TIME::TimeNow();
			}
		}
		break;
	case GM_DESTROY:
		DeleteWndCtrl(1, 2);
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;	
}

//通道显示窗口
const char *g_ChannelView_Type[] = {"Ethernet ", "GPRS/CDMA"};
int C_ChannelViewWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	S_MenuCtrl *pData = static_cast<S_MenuCtrl *>(GetWndData(sizeof(S_MenuCtrl)));
	switch (msg)
	{
	case GM_CREATE:{
		pData->m_InfoIndex = (int)lParam;
		C_BUTTON *pbtnSet = C_BUTTON::NewCtrl();
		S_RECT rect = {60, 100, 32, 16};
		pbtnSet->Create("设置", 0x00, rect, this, 1);
		SetFocusCtrl(pbtnSet);
		}break;
	case GM_PAINT:{
		S_WORD port = 0x00;
		S_DWORD IP = 0x00;
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		int ChannelType = GetHttxChannelInfo(pData->m_InfoIndex - 1, IP, port);
		sprintf(buffer, "主站通道%d信息", pData->m_InfoIndex);
		pdc->DrawString(STRINGX, 0*18+4, buffer);
		sprintf(buffer, "通道类型:%s", g_ChannelView_Type[ChannelType]);
		pdc->DrawString(STRINGX, 1*18+4, buffer);
		S_BYTE *pIP = (S_BYTE *)&IP;
		sprintf(buffer, "  %03d.%03d.%03d.%03d", *(pIP+0), *(pIP+1), *(pIP+2), *(pIP+3));
		pdc->DrawString(STRINGX, 2*18+4, "主站 IP :");
		pdc->DrawString(STRINGX, 3*18+4, buffer);
		sprintf(buffer, "主站端口: %d", port);
		pdc->DrawString(STRINGX, 4*18+4, buffer);
		pdc->DrawHLine(0, 122, 160);
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
			if (C_PWD::Certification() == true)
				GOTOWND(C_ChannelSetWnd, (S_DWORD)(pData->m_InfoIndex));
		}
		break;
	case GM_DESTROY:
		ReleaseWndData();
		DeleteWndCtrl(1, 1);
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;	
}

/** @brief 取通道信息
 *  @return 0 以太网; 1 GPRS/CDMA
 */
int C_ChannelViewWnd::GetHttxChannelInfo(int ChannelIdx, S_DWORD &IP, S_WORD &port)
{
	IP = 0, port = 0;
	S_CHANNEL *pChannel = STATUS.GetHttxChannel(MP_HTTX_1, (S_ChannelSN)ChannelIdx);
	if (pChannel != NULL && pChannel->m_Type == CHANNEL_TYPE_NET)
	{
		port = pChannel->m_PortSet.m_NetSet.m_PortNO;
		memcpy(&IP, pChannel->m_PortSet.m_NetSet.m_IP, 4);
		if (pChannel->NeedPPP() == true)
			return 1;
	}
	return 0;
}

//PPP参数显示窗口
int C_PppViewWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	switch (msg)
	{
	case GM_CREATE:{
		S_RECT rect = {110, 1*17+2, 32, 16};
		C_BUTTON *pbtnSet1 = C_BUTTON::NewCtrl();
		pbtnSet1->Create("设置", 0x00, rect, this, 1);
		rect.y += 2*17;
		C_BUTTON *pbtnSet2 = C_BUTTON::NewCtrl();
		pbtnSet2->Create("设置", 0x00, rect, this, 2);
		rect.y += 2*17;
		C_BUTTON *pbtnSet3 = C_BUTTON::NewCtrl();
		pbtnSet3->Create("设置", 0x00, rect, this, 3);
		SetFocusCtrl(pbtnSet1);
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		S_PppInfo *pPppInfo = STATUS.GetPppInfo();
		pdc->DrawString(STRINGX+0, 0*17+2, "PPP参数信息");
		pdc->DrawString(STRINGX+0, 1*17+2, "APN:");
		pdc->DrawString(STRINGX+8, 2*17+2, pPppInfo->m_DialInfo.m_Apn);
		pdc->DrawString(STRINGX+0, 3*17+2, "用户名:");
		pdc->DrawString(STRINGX+8, 4*17+2, pPppInfo->m_DialInfo.m_User);
		pdc->DrawString(STRINGX+0, 5*17+2, "密码:");
		pdc->DrawString(STRINGX+8, 6*17+2, pPppInfo->m_DialInfo.m_Pwd);
		pdc->DrawHLine(0, 122, 160);
		EndPaint(pdc);
		}
		break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
			RETURNWND();
		else if (wParam == GVK_UP)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			S_DWORD id = pCtrl->GetCtrlID() - 1;
			if (id == 0) id = 3;
			SetFocusCtrl(GetWndCtrl((int)id));
		}
		else if (wParam == GVK_DN)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			S_DWORD id = pCtrl->GetCtrlID() + 1;
			if (id == 4) id = 1;
			SetFocusCtrl(GetWndCtrl((int)id));
		}
		break;
	case GM_CMD:
		if (wParam >= 1 && wParam <= 3 && lParam == GM_CMD_BTN_PUSHED)
		{
			if (C_PWD::Certification())
				GOTOWND(C_TextEditWnd, wParam);
		}
		break;
	case GM_DESTROY:
		DeleteWndCtrl(1, 3);
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;	
}

//本地网络设置窗口
int C_LocalNetViewWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	switch (msg)
	{
	case GM_CREATE:{
		C_BUTTON *pbtnSet = C_BUTTON::NewCtrl();
		S_RECT rect = {60, 6*17+2, 32, 16};
		pbtnSet->Create("设置", 0x00, rect, this, 1);
		SetFocusCtrl(pbtnSet);
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		S_IpConfig *pIpCfg = STATUS.GetIpConfig();
		S_DWORD LocalIP = C_IfConfig::GetEth0IP();
		S_BYTE *pIp = (S_BYTE *)&LocalIP;
		sprintf(buffer, "  %03d.%03d.%03d.%03d", *(pIp+0), *(pIp+1), *(pIp+2), *(pIp+3));
		pdc->DrawString(STRINGX, 0*17+2, "IP 地址 :");
		pdc->DrawString(STRINGX, 1*17+2, buffer);
		sprintf(buffer, "  %03d.%03d.%03d.%03d", pIpCfg->m_NetMask[0], pIpCfg->m_NetMask[1],
			pIpCfg->m_NetMask[2], pIpCfg->m_NetMask[3]);
		pdc->DrawString(STRINGX, 2*17+2, "子网掩码:");
		pdc->DrawString(STRINGX, 3*17+2, buffer);
		sprintf(buffer, "  %03d.%03d.%03d.%03d", pIpCfg->m_GateWay[0], pIpCfg->m_GateWay[1],
			pIpCfg->m_GateWay[2], pIpCfg->m_GateWay[3]);
		pdc->DrawString(STRINGX, 4*17+2, "默认网关:");
		pdc->DrawString(STRINGX, 5*17+2, buffer);
		pdc->DrawHLine(0, 122, 160);
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
			if (C_PWD::Certification() == true)
				GOTOWND(C_LocalNetSetWnd, 0x00);
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

//通讯参数显示 如心跳间隔等参数
int C_ChannelsParamViewWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	switch (msg)
	{
	case GM_CREATE:{
		C_BUTTON *pbtnSet = C_BUTTON::NewCtrl();
		S_RECT rect = {60, 90, 32, 16};
		pbtnSet->Create("设置", 0x00, rect, this, 1);
		SetFocusCtrl(pbtnSet);
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));

		S_HttxInfo *pHttxInfo = STATUS.GetHttxInfo(MP_HTTX_1);
		sprintf(buffer, "心跳间隔(分): %d", pHttxInfo->m_HeartBeatPeriodM);
		pdc->DrawString(STRINGX, LINEXY(1), buffer);
		
		pdc->DrawHLine(0, 122, 160);
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
			if (C_PWD::Certification() == true)
				GOTOWND(C_ChannelsParamSetWnd, 0x00);
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

//通讯参数设置 如心跳间隔等参数
int C_ChannelsParamSetWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	S_HttxInfo *pHttxInfo = STATUS.GetHttxInfo(MP_HTTX_1);
	switch (msg)
	{
	case GM_CREATE:{
		S_RECT rect1 = {STRINGX+108, LINEXY(1), 20, 16};
		C_NUMEDIT *pEdit = C_NUMEDIT::NewCtrl();
		sprintf(buffer, "%02d", pHttxInfo->m_HeartBeatPeriodM);
		pEdit->Create(buffer, 0x00, rect1, this, 2);

		C_BUTTON *pbtnSet = C_BUTTON::NewCtrl();
		S_RECT rect2 = {60, 90, 32, 16};
		pbtnSet->Create("确定", 0x00, rect2, this, 1);
		SetFocusCtrl(pEdit);
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		pdc->DrawString(STRINGX, LINEXY(1), "心跳间隔(分):");
		pdc->DrawHLine(0, 122, 160);
		EndPaint(pdc);
		}
		break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
			RETURNWND();
		else if (wParam == GVK_OK || wParam == GVK_RIGHT)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			int id = pCtrl->GetCtrlID() + 1;
			if (id > 2) id = 1;
			SetFocusCtrl(GetWndCtrl(id));
		}

		break;
	case GM_CMD:
		if (wParam == 1 && lParam == GM_CMD_BTN_PUSHED)
		{
			string strData;
			strings strChannelName;

			strData = GetWndCtrl(2)->GetWndText();
			string::size_type itero = strData.find(' ');
			strData= strData.substr(0,itero);
			S_BYTE ti=(S_BYTE)atoi(strData.c_str());

			if (SGui_MsgBox("提示", " 确定更改通道参数?", MB_BTN_OKCANCEL) == MB_RTN_YESOK)
			{
				pHttxInfo->m_HeartBeatPeriodM=ti;
				STATUS.SaveHttxInfo();
				RETURNWND();
			}
		}

		break;
	case GM_DESTROY:
		DeleteWndCtrl(1, 2);
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;	
}


//通道设置窗口
const char *g_ChannelSet_info[] = {"            ", "参数格式错误", "参数设置成功"};
const char *g_pChannelName = "Ethernet ;GPRS/CDMA";
int C_ChannelSetWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	S_MenuCtrl *pData = static_cast<S_MenuCtrl *>(GetWndData(sizeof(S_MenuCtrl)));
	switch (msg)
	{
	case GM_CREATE:{
		S_WORD port;
		S_DWORD IP;
		pData->m_InfoIndex = 0;
		pData->m_SelectM = (int)lParam;
		int ChannelType = GetHttxChannelInfo(pData->m_SelectM - 1, IP, port);

		S_RECT BoxRect =  {74, 1*18+6, 76, 16};
		S_RECT NumRect = {16, 3*18+4, 24, 16};
		S_RECT PortRect = {74, 4*18+6, 36, 16};
		S_RECT BtnRect =  {60, 100, 32, 16};

		C_SELECTBOX *pBox = C_SELECTBOX::NewCtrl();
		pBox->Create(g_pChannelName, 0x00, BoxRect, this, 1);
		pBox->SetDefaultItem((S_DWORD)ChannelType);

		S_BYTE *pIP = (S_BYTE *)&IP;		
		NumRect.x += 0,  CreateNumEdit(*(pIP+0), NumRect, 2);
		NumRect.x += 32, CreateNumEdit(*(pIP+1), NumRect, 3);
		NumRect.x += 32, CreateNumEdit(*(pIP+2), NumRect, 4);
		NumRect.x += 32, CreateNumEdit(*(pIP+3), NumRect, 5);
		CreateNumEdit(port, PortRect, 6);

		C_BUTTON *pbtnSet = C_BUTTON::NewCtrl();
		pbtnSet->Create("确定", 0x00, BtnRect, this, 7);

		SetFocusCtrl(GetWndCtrl(1));
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		sprintf(buffer, "主站通道%d信息设置", pData->m_SelectM);
		pdc->DrawString(STRINGX, 0*18+4, buffer);
		pdc->DrawString(STRINGX, 1*18+4, "通道类型:");
		pdc->DrawString(STRINGX, 2*18+4, "主站 IP :");
		pdc->DrawString(41, 3*18+4, ".");
		pdc->DrawString(72, 3*18+4, ".");
		pdc->DrawString(105, 3*18+4, ".");
		pdc->DrawString(STRINGX, 4*18+4, "主站端口:");
		pdc->DrawHLine(0, 122, 160);
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F12x12));
		pdc->DrawString(STRINGX, 126, g_ChannelSet_info[pData->m_InfoIndex]);
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
			if (id >= 8) id = 1;
			SetFocusCtrl(GetWndCtrl(id));
		}
		else if (wParam == GVK_LEFT)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			int id = pCtrl->GetCtrlID() - 1;
			if (id <= 0) id = 7;
			SetFocusCtrl(GetWndCtrl(id));
		}
		break;
	case GM_CMD:
		if (wParam == 7 && lParam == GM_CMD_BTN_PUSHED)
		{
			int ChannelType = -1;
			 S_DWORD port = 0, IP = 0;
			if (CheckParamFormat(ChannelType, IP, port) == false)
			{
				pData->m_InfoIndex = 1;
				InvalidateRect(NULL);
			}
			else if (SGui_MsgBox("提示", " 确定更改通道参数?", MB_BTN_OKCANCEL) == MB_RTN_YESOK)
			{
				SaveHttxChannelInfo(pData->m_SelectM - 1, ChannelType, IP, (S_WORD)port);
				pData->m_InfoIndex = 2;
			}
			else
			{
				pData->m_InfoIndex = 0;
			}
		}
		break;
	case GM_DESTROY:
		ReleaseWndData();
		DeleteWndCtrl(1, 7);
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;	
}

bool C_ChannelSetWnd::CreateNumEdit(int num, S_RECT &NumRect, S_DWORD ID)
{
	char buffer[64];
	C_NUMEDIT *pEdit = C_NUMEDIT::NewCtrl();
	if (ID == 0x06) sprintf(buffer, "%05d", num);
	else sprintf(buffer, "%03d", num);
	return pEdit->Create(buffer, NUMEDIT_STYLE_DEC, NumRect, this, ID);
}

bool C_ChannelSetWnd::CheckParamFormat(int &ChannelType, S_DWORD &IP, S_DWORD &port)
{
	string strData;
	strings strChannelName;
	int ip0, ip1, ip2, ip3;

	strChannelName.split(g_pChannelName, ';');
	strData = GetWndCtrl(1)->GetWndText();

	strings::iterator iter = find(strChannelName.begin(), strChannelName.end(), strData);
	if (iter == strChannelName.end())
		return false;
	ChannelType = (int)(iter - strChannelName.begin());

	strData = GetWndCtrl(2)->GetWndText(), ip0 = atoi(strData.c_str());
	strData = GetWndCtrl(3)->GetWndText(), ip1 = atoi(strData.c_str());
	strData = GetWndCtrl(4)->GetWndText(), ip2 = atoi(strData.c_str());
	strData = GetWndCtrl(5)->GetWndText(), ip3 = atoi(strData.c_str());
	if (ip0 > 255 || ip1 > 255 || ip2 > 255 || ip3 > 255)
		return false;

	strData = GetWndCtrl(6)->GetWndText(), port = (S_DWORD)atoi(strData.c_str());
	if (port > 65535)
		return false;

	S_BYTE *pIP = (S_BYTE *)&IP;
	*(pIP+0) = (S_BYTE)ip0, *(pIP+1) = (S_BYTE)ip1, *(pIP+2) = (S_BYTE)ip2, *(pIP+3) = (S_BYTE)ip3;

	return true;
}

bool C_ChannelSetWnd::SaveHttxChannelInfo(int ChannelIdx, int ChannelType, S_DWORD IP, S_WORD port)
{
	S_CHANNEL *pChannel = STATUS.GetHttxChannel(MP_HTTX_1, (S_ChannelSN)ChannelIdx);
	if (pChannel != NULL && pChannel->m_Type == CHANNEL_TYPE_NET)
	{
		pChannel->m_PortSet.m_NetSet.m_PortNO = port;
		memcpy(pChannel->m_PortSet.m_NetSet.m_IP, &IP, 4);
		if (ChannelType == 0) //ethernet
			pChannel->m_PortSet.m_NetSet.m_NeedPPP = NET_CONNECT_PPP_NO;
		else if (ChannelType == 1)//GPRS
			pChannel->m_PortSet.m_NetSet.m_NeedPPP = NET_CONNECT_PPP_YES;

		STATUS.SaveUpChannel();
		CONTEXT.SetReconnectFlag(true);
		return true;
	}
	return false;
}

//本地网络设置窗口
const char *g_LocalNetSet_info[] = {"            ", "参数格式错误", "参数设置成功", "系统准备重启"};
int C_LocalNetSetWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[CFG_PATH_FILE_MAX_LEN];
	S_MenuCtrl *pData = static_cast<S_MenuCtrl *>(GetWndData(sizeof(S_MenuCtrl)));
	switch (msg)
	{
	case GM_CREATE:{
		pData->m_InfoIndex = 0;
		S_RECT NumRect1 =  {16, 1*17+2, 24, 16};
		S_RECT NumRect2 = {16, 3*17+2, 24, 16};
		S_RECT NumRect3 = {16, 5*17+2, 24, 16};
		S_RECT BtnRect =  {60, 6*17+2, 32, 16};

		S_DWORD LocalIP = C_IfConfig::GetEth0IP();
		S_BYTE *pIp = (S_BYTE *)&LocalIP;
		NumRect1.x += 0,  CreateNumEdit(*(pIp+0), NumRect1, 1);
		NumRect1.x += 32, CreateNumEdit(*(pIp+1), NumRect1, 2);
		NumRect1.x += 32, CreateNumEdit(*(pIp+2), NumRect1, 3);
		NumRect1.x += 32, CreateNumEdit(*(pIp+3), NumRect1, 4);

		S_IpConfig *pIpCfg = STATUS.GetIpConfig();
		NumRect2.x += 0,   CreateNumEdit(pIpCfg->m_NetMask[0], NumRect2, 5);
		NumRect2.x += 32, CreateNumEdit(pIpCfg->m_NetMask[1], NumRect2, 6);
		NumRect2.x += 32, CreateNumEdit(pIpCfg->m_NetMask[2], NumRect2, 7);
		NumRect2.x += 32, CreateNumEdit(pIpCfg->m_NetMask[3], NumRect2, 8);
		NumRect3.x += 0,   CreateNumEdit(pIpCfg->m_GateWay[0], NumRect3, 9);
		NumRect3.x += 32, CreateNumEdit(pIpCfg->m_GateWay[1], NumRect3, 10);
		NumRect3.x += 32, CreateNumEdit(pIpCfg->m_GateWay[2], NumRect3, 11);
		NumRect3.x += 32, CreateNumEdit(pIpCfg->m_GateWay[3], NumRect3, 12);

		C_BUTTON *pbtnSet = C_BUTTON::NewCtrl();
		pbtnSet->Create("确定", 0x00, BtnRect, this, 13);

		SetFocusCtrl(GetWndCtrl(1));
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		pdc->DrawString(STRINGX, 0*17+2, "IP 地址:");
		pdc->DrawString(16+26, 1*17+2, ".");
		pdc->DrawString(16+58, 1*17+2, ".");
		pdc->DrawString(16+90, 1*17+2, ".");
		pdc->DrawString(STRINGX, 2*17+2, "子网掩码:");
		pdc->DrawString(16+26, 3*17+2, ".");
		pdc->DrawString(16+58, 3*17+2, ".");
		pdc->DrawString(16+90, 3*17+2, ".");
		pdc->DrawString(STRINGX, 4*17+2, "默认网关:");
		pdc->DrawString(16+26, 5*17+2, ".");
		pdc->DrawString(16+58, 5*17+2, ".");
		pdc->DrawString(16+90, 5*17+2, ".");
		pdc->DrawHLine(0, 122, 160);
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F12x12));
		pdc->DrawString(20, 126, g_LocalNetSet_info[pData->m_InfoIndex]);
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
				memcpy(pIpCfg->m_TermIP, &LocalIP, 4);
				memcpy(pIpCfg->m_NetMask, &NetMask, 4);
				memcpy(pIpCfg->m_GateWay, &GateWay, 4);
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

bool C_LocalNetSetWnd::CreateNumEdit(int num, S_RECT &NumRect, S_DWORD ID)
{
	char buffer[64];
	C_NUMEDIT *pEdit = C_NUMEDIT::NewCtrl();
	sprintf(buffer, "%03d", num);
	return pEdit->Create(buffer, NUMEDIT_STYLE_DEC, NumRect, this, ID);
}

bool C_LocalNetSetWnd::CheckParamFormat(S_DWORD &LocalIP, S_DWORD &NetMask, S_DWORD &GateWay)
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

//电表参数设置
const char *g_pMeterParamSet_Status = "无效;有效";
const char *g_pMeterParamSet_Function = "单相表  ;多功能表;其它类型";
int C_MeterParamSetWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	S_MeterSet *pData = static_cast<S_MeterSet *>(GetWndData(sizeof(S_MeterSet)));
	switch (msg)
	{
	case GM_CREATE:{
		S_RECT Rect =  {58, 0*17+2, 32, 16};
		pData->m_MP = (S_WORD)lParam;
		S_MpInfos *pMpInfos = STATUS.GetMpInfos();
		S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
		pData->m_MpInfo = pMpInfos->m_Item[pData->m_MP];
		pData->m_MeterInfo = pMeterInfos->m_Item[pData->m_MP];
		pData->m_MeterInfo.m_MeasureNo = pData->m_MP;

		string strPortName, strRuleName;
		C_CfgItem itemPort(s_MeterPortItem, KGE_MEMBER_OF(s_MeterPortItem));
		S_MeterInfo MeterInfo = pData->m_MeterInfo;
		MeterInfo.m_FunType -= 1;
		if (MeterInfo.m_PortSet.m_PortNO > 0)
			MeterInfo.m_PortSet.m_PortNO -= 1;

		Rect.y += 0, Rect.w = 42;
		CreateSelectBox(g_pMeterParamSet_Status, pData->m_MpInfo.m_AutoGather, Rect, 1);
		Rect.y += 17, Rect.w = 66;
		CreateSelectBox(g_pMeterParamSet_Function, MeterInfo.m_FunType, Rect, 2);
		Rect.y += 17, Rect.w = 78;
		memset(buffer, 0x00, sizeof(buffer));
		CreateNumEdit(MeterInfo.GetMeterAddress(buffer), NUMEDIT_STYLE_DEC, Rect, 3);
		Rect.y += 17, Rect.w = 90;
		C_NwMRuleName::SerializeMeterRuleName(strRuleName, ' ', ';');
		CreateSelectBox(strRuleName.c_str(), C_NwMRuleName::MakeMeterRuleIndex(MeterInfo.m_RuleID), Rect, 4);
		Rect.y += 17, Rect.w = 66;
		itemPort.SerializeItemKey(strPortName, ';');
		CreateSelectBox(strPortName.c_str(), itemPort.MakeIndex(MeterInfo.m_PortSet.m_PortNO), Rect, 5);
		Rect.y += 17, Rect.w = 78;
		memset(buffer, 0x00, sizeof(buffer));
		CreateNumEdit(MeterInfo.GetRelayAddress(buffer), NUMEDIT_STYLE_DEC, Rect, 6);

		S_RECT BtnRectSet =  {43, 104, 32, 16};
		S_RECT BtnRectDetail =  {85, 104, 32, 16};
		C_BUTTON *pbtnSet = C_BUTTON::NewCtrl();
		pbtnSet->Create("设置", 0x00, BtnRectSet, this, 7);
		C_BUTTON *pbtnDetail = C_BUTTON::NewCtrl();
		pbtnDetail->Create("详细", 0x00, BtnRectDetail, this, 8);

		SetFocusCtrl(GetWndCtrl(1));
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		pdc->DrawString(STRINGX, 0*17+2, "状态  : ");
		pdc->DrawString(STRINGX, 1*17+2, "类型  : ");
		pdc->DrawString(STRINGX, 2*17+2, "地址  : ");
		pdc->DrawString(STRINGX, 3*17+2, "规约  : ");
		pdc->DrawString(STRINGX, 4*17+2, "端口  : ");
		pdc->DrawString(80, 4*17+2, "H");
		pdc->DrawString(STRINGX, 5*17+2, "采集器: ");
		pdc->DrawHLine(0, 122, 160);
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F12x12));
		sprintf(buffer, "当前测量点:%04d", pData->m_MP);
		pdc->DrawString(STRINGX, 126, buffer);
		EndPaint(pdc);
		}break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
			RETURNWND();
		else if (wParam == GVK_UP)
			SetFocusCtrl(GetWndCtrl(1));
		else if (wParam == GVK_OK || wParam == GVK_RIGHT)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			int id = pCtrl->GetCtrlID() + 1;
			if (id >= 9) id = 1;
			SetFocusCtrl(GetWndCtrl(id));
		}
		else if (wParam == GVK_LEFT)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			int id = pCtrl->GetCtrlID() - 1;
			if (id <= 0) id = 8;
			SetFocusCtrl(GetWndCtrl(id));
		}
		break;
	case GM_CMD:
		if (wParam == 7 && lParam == GM_CMD_BTN_PUSHED)
		{
			C_SGUIDC dc(this);
			S_MpInfo MpInfo = pData->m_MpInfo;
			S_MeterInfo MeterInfo = pData->m_MeterInfo;
			if (CheckParamFormat(MeterInfo, MpInfo) == false)
			{
				dc.DrawString(20, 126, "端口号配置错误!!!");
				SleepSecond(1);
				InvalidateRect(NULL);
			}
			else if (SGui_MsgBox("提示", " 确定保存电表参数?", MB_BTN_OKCANCEL) == MB_RTN_YESOK)
			{
				dc.DrawString(20, 126, "保存参数成功!!!  ");

				S_MpInfos *pMpInfos = STATUS.GetMpInfos();
				S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();
				pData->m_MpInfo = MpInfo;
				pData->m_MeterInfo = MeterInfo;
				pMpInfos->Set(pData->m_MP, MP_TYPE_METER, pData->m_MP);
				pMpInfos->m_Item[pData->m_MP].m_AutoGather = pData->m_MpInfo.m_AutoGather;
				pMeterInfos->m_Item[pData->m_MP] = pData->m_MeterInfo;
				STATUS.SaveDbcjInfo();
				STATUS.RefreshMeterInfo();
				CONTEXT.NotifyDbcj(NOTIFY_DBCJ_ADD_METER);
				SleepSecond(1);
			}
		}
		else if (wParam == 8 && lParam == GM_CMD_BTN_PUSHED)
		{
			S_MpInfo MpInfo = pData->m_MpInfo;
			S_MeterInfo MeterInfo = pData->m_MeterInfo;
			if (CheckParamFormat(MeterInfo, MpInfo) == false)
			{
				C_SGUIDC dc(this);
				dc.DrawString(20, 126, "端口号配置错误!!!");
				SleepSecond(1);
				InvalidateRect(NULL);
			}
			else
			{
				pData->m_MpInfo = MpInfo;
				pData->m_MeterInfo = MeterInfo;
				GOTOWNDEX(C_MeterDetailSetWnd, 0x00, pData);
			}
		}
		break;
	case GM_DESTROY:
		ReleaseWndData();
		DeleteWndCtrl(1, 7);
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;	
}

bool C_MeterParamSetWnd::CreateNumEdit(const char *pBuffer, S_DWORD style, S_RECT &NumRect, S_DWORD ID)
{
	C_NUMEDIT *pEdit = C_NUMEDIT::NewCtrl();
	return pEdit->Create(pBuffer, style, NumRect, this, ID);
}

bool C_MeterParamSetWnd::CreateSelectBox(const char *pBuffer, S_DWORD Idx, S_RECT &NumRect, S_DWORD ID)
{
	C_SELECTBOX *pBox = C_SELECTBOX::NewCtrl();
	if (pBox->Create(pBuffer, NUMEDIT_STYLE_DEC, NumRect, this, ID) == false)
		return false;
	return pBox->SetDefaultItem(Idx);
}

bool C_MeterParamSetWnd::CheckParamFormat(S_MeterInfo &MeterInfo, S_MpInfo &MpInfo)
{
	string strData;
	C_CfgItem itemPort(s_MeterPortItem, KGE_MEMBER_OF(s_MeterPortItem));

	strData = GetWndCtrl(5)->GetWndText();
	int port = itemPort.MakeValue(strData.c_str());
	if (port < 0 || port > 63)
		return false;

	MpInfo.m_AutoGather = (S_WORD)GetSelectBoxIdx(g_pMeterParamSet_Status, 1);

	MeterInfo.m_Enable = true;
	MeterInfo.m_FunType = (S_BYTE)(GetSelectBoxIdx(g_pMeterParamSet_Function, 2) + 1);

	strData = GetWndCtrl(3)->GetWndText();
	MeterInfo.SetMeterAddress(strData.c_str());

	strData = GetWndCtrl(4)->GetWndText();
	replace(strData.begin(), strData.end(), ' ', '\0');
	MeterInfo.m_RuleID = (S_BYTE)C_NwMRuleName::MakeMeterRuleID(strData.c_str());

	MeterInfo.m_PortSet.m_PortNO = port + 1;
	if (MeterInfo.m_PortSet.m_Baud == 0x00)
	{
		if (MeterInfo.m_RuleID == METER_RULE_GB645_1997) //DL/T645-1997
			MeterInfo.m_PortSet.m_Baud = 1200;
		else if (MeterInfo.m_RuleID == METER_RULE_GB645_2007) // DL/T645-2007
			MeterInfo.m_PortSet.m_Baud = 2400;
		MeterInfo.m_PortSet.m_DataBit = 8;
		MeterInfo.m_PortSet.m_StopBit = 1;
		MeterInfo.m_PortSet.m_Parity = 2;
	}

	strData = GetWndCtrl(6)->GetWndText();
	MeterInfo.SetRelayAddress(strData.c_str());

	return true;
}

S_DWORD C_MeterParamSetWnd::GetSelectBoxIdx(const char *pBuffer, S_DWORD ID)
{
	strings strsStatus;
	strsStatus.split(pBuffer, ';');
	string strData = GetWndCtrl(ID)->GetWndText();
	strings::iterator iter = find(strsStatus.begin(), strsStatus.end(), strData);
	if (iter == strsStatus.end())
		return 0;
	return (S_WORD)(iter - strsStatus.begin());
}

const char *g_MeterDetailSet_attribute = "  485表 ; 模拟量 ; 脉冲量 ; 计算值 ;交流采样";
const char *g_MeterDetailSet_yes = "否;是";
const char *g_MeterDetailSet_have = "无;有";
const char *g_MeterDetailSet_main = "分;总";
const char *g_MeterDetailSet_Parity = "无;奇;偶";
const char *g_MeterDetailSet_baud = "   300;   600;  1200;  2400;  4800;  9600; 19200; 38400; 57600;115200";
const char *g_MeterDetailSet_stopbit = "  1;  2;1.5";
int C_MeterDetailSetWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
#define METER_SET_2_W	34
#define METER_SET_3_W	50
#define METER_SET_6_W	124
	char buffer[64];
	S_MeterSet *pMeterSet = static_cast<S_MeterSet *>(GetFatherWndData());
	switch (msg)
	{
	case GM_CREATE:{
		S_RECT Rect =  {26, 0*17+2, 32, 16};
		pMeterSet->m_MpInfo.m_Type = MP_TYPE_METER; // 调整为表计

		Rect.x = METER_SET_2_W, Rect.w = 68;
		CreateSelectBox(g_MeterDetailSet_attribute, pMeterSet->m_MpInfo.m_Type - 1, Rect, 1);

		Rect.x = METER_SET_3_W, Rect.y += 17, Rect.w = 32;
		CreateSelectBox(g_MeterDetailSet_yes, pMeterSet->m_MeterInfo.m_VipFlag, Rect, 2);

		Rect.x = METER_SET_6_W, Rect.w = 16;
		sprintf(buffer, "%02d", pMeterSet->m_MeterInfo.m_TariffCnt);
		CreateNumEdit(buffer, NUMEDIT_STYLE_DEC, Rect, 3);

		Rect.x = METER_SET_2_W, Rect.y += 17, Rect.w = 32;
		CreateSelectBox(g_MeterDetailSet_main, pMeterSet->m_MeterInfo.m_MainFlag, Rect, 4);

		Rect.x = METER_SET_6_W, Rect.w = 32;
		CreateSelectBox(g_MeterDetailSet_have, pMeterSet->m_MeterInfo.m_PowerCut, Rect, 5);

		Rect.x = METER_SET_2_W, Rect.y += 17, Rect.w = 32;
		sprintf(buffer, "%04d", pMeterSet->m_MeterInfo.m_CT);
		CreateNumEdit(buffer, NUMEDIT_STYLE_DEC, Rect, 6);

		Rect.x = METER_SET_6_W, Rect.w = 32;
		sprintf(buffer, "%04d", pMeterSet->m_MeterInfo.m_PT);
		CreateNumEdit(buffer, NUMEDIT_STYLE_DEC, Rect, 7);

		Rect.x = METER_SET_3_W, Rect.y += 17, Rect.w = 54;
		CreateSelectBox(g_MeterDetailSet_baud, GetBaudNO(pMeterSet->m_MeterInfo.m_PortSet.m_Baud), Rect, 8);

		Rect.x = METER_SET_3_W, Rect.y += 17, Rect.w = 16;
		sprintf(buffer, "%d", pMeterSet->m_MeterInfo.m_PortSet.m_DataBit);
		CreateNumEdit(buffer, NUMEDIT_STYLE_DEC, Rect, 9);

		Rect.x = METER_SET_6_W, Rect.w = 32;
		CreateSelectBox(g_MeterDetailSet_Parity, pMeterSet->m_MeterInfo.m_PortSet.m_Parity, Rect, 10);

		S_DWORD StopBitIdx = 0;
		if (pMeterSet->m_MeterInfo.m_PortSet.m_StopBit > 0)
			StopBitIdx = pMeterSet->m_MeterInfo.m_PortSet.m_StopBit - 1;
		Rect.x = METER_SET_3_W, Rect.y += 17, Rect.w = 38;
		CreateSelectBox(g_MeterDetailSet_stopbit, StopBitIdx, Rect, 11);

		S_RECT BtnRectSet =  {108, 104, 32, 16};
		C_BUTTON *pbtnSet = C_BUTTON::NewCtrl();
		pbtnSet->Create("设置", 0x00, BtnRectSet, this, 12);

		SetFocusCtrl(GetWndCtrl(1));
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		pdc->DrawString(STRINGX, 0*17+2, "性质");
		pdc->DrawString(STRINGX, 1*17+2, "重点户");
		pdc->DrawString(90, 1*17+2, "费率");
		pdc->DrawString(STRINGX, 2*17+2, "总分");
		pdc->DrawString(90, 2*17+2, "拉闸");
		pdc->DrawString(STRINGX, 3*17+2, " CT");
		pdc->DrawString(90, 3*17+2, " PT");
		pdc->DrawString(STRINGX, 4*17+2, "波特率");
		pdc->DrawString(STRINGX, 5*17+2, "数据位");
		pdc->DrawString(90, 5*17+2, "校验");
		pdc->DrawString(STRINGX, 6*17+2, "停止位");
		pdc->DrawHLine(0, 122, 160);
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F12x12));
		sprintf(buffer, "当前测量点:%04d", pMeterSet->m_MP);
		pdc->DrawString(STRINGX, 126, buffer);
		EndPaint(pdc);
		}break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
			RETURNWND();
		else if (wParam == GVK_UP)
			SetFocusCtrl(GetWndCtrl(1));
		else if (wParam == GVK_OK || wParam == GVK_RIGHT)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			int id = pCtrl->GetCtrlID() + 1;
			if (id >= 13) id = 1;
			SetFocusCtrl(GetWndCtrl(id));
		}
		else if (wParam == GVK_LEFT)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			int id = pCtrl->GetCtrlID() - 1;
			if (id <= 0) id = 12;
			SetFocusCtrl(GetWndCtrl(id));
		}
		break;
	case GM_CMD:
		if (wParam == 12 && lParam == GM_CMD_BTN_PUSHED)
		{
			if (SGui_MsgBox("提示", " 确定保存电表参数?", MB_BTN_OKCANCEL) == MB_RTN_YESOK)
			{
				S_MpInfos *pMpInfos = STATUS.GetMpInfos();
				S_MeterInfos *pMeterInfos = STATUS.GetMeterInfos();

				CheckDetailFormat(pMeterSet->m_MeterInfo, pMeterSet->m_MpInfo);
				pMpInfos->Set(pMeterSet->m_MP, MP_TYPE_METER, pMeterSet->m_MP);
				pMpInfos->m_Item[pMeterSet->m_MP].m_AutoGather = pMeterSet->m_MpInfo.m_AutoGather;
				pMeterInfos->m_Item[pMeterSet->m_MP] = pMeterSet->m_MeterInfo;
				STATUS.SaveDbcjInfo();
				STATUS.RefreshMeterInfo();
				CONTEXT.NotifyDbcj(NOTIFY_DBCJ_ADD_METER);
				SleepSecond(1);
			}
		}
		break;
	case GM_DESTROY:
		DeleteWndCtrl(1, 12);
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;	
}

static S_DWORD g_StdandBaud[] = {300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200};
S_DWORD C_MeterDetailSetWnd::GetBaudNO(S_DWORD baud)
{
	for (S_DWORD i = 0; i < KGE_MEMBER_OF(g_StdandBaud); i++)
	{
		if (baud == g_StdandBaud[i])
			return i;
	}
	return 0x00;
}

S_DWORD C_MeterDetailSetWnd::GetBaud(S_DWORD idx)
{
	if (idx >= KGE_MEMBER_OF(g_StdandBaud))
		return 0;
	return g_StdandBaud[idx];
}

bool C_MeterDetailSetWnd::CheckDetailFormat(S_MeterInfo &MeterInfo, S_MpInfo &MpInfo)
{
	string strData;
	strings::iterator iter;
	MpInfo.m_Type = MP_TYPE_METER; // 强制为表计TODO:

	MeterInfo.m_VipFlag = GetSelectBoxIdx(g_MeterDetailSet_yes, 2)? true:false;
	strData = GetWndCtrl(3)->GetWndText();
	MeterInfo.m_TariffCnt = atoi(strData.c_str());
	MeterInfo.m_MainFlag = GetSelectBoxIdx(g_MeterDetailSet_main, 4)? true:false;
	MeterInfo.m_PowerCut = GetSelectBoxIdx(g_MeterDetailSet_have, 5)? true:false;
	strData = GetWndCtrl(6)->GetWndText();
	MeterInfo.m_CT = atoi(strData.c_str());
	strData = GetWndCtrl(7)->GetWndText();
	MeterInfo.m_PT = atoi(strData.c_str());
	MeterInfo.m_PortSet.m_Baud = GetBaud(GetSelectBoxIdx(g_MeterDetailSet_baud, 8));
	strData = GetWndCtrl(9)->GetWndText();
	MeterInfo.m_PortSet.m_DataBit = atoi(strData.c_str());
	MeterInfo.m_PortSet.m_Parity = GetSelectBoxIdx(g_MeterDetailSet_Parity, 10);
	MeterInfo.m_PortSet.m_StopBit = GetSelectBoxIdx(g_MeterDetailSet_stopbit, 11) + 1;
	return true;
}

//终端时间设置窗口
const char *g_TermTimeSet_info[] = {"            ", "时间格式错误", "时间设置出错", "时间设置成功"};
int C_TermTimeSetWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	S_MenuCtrl *pData = static_cast<S_MenuCtrl *>(GetWndData(sizeof(S_MenuCtrl)));
	switch (msg)
	{
	case GM_CREATE:{
		pData->m_InfoIndex = 0;
		S_DATE date;
		C_TIME::LocalTime(C_TIME::TimeNow(), date);
		S_RECT NumRect1 = {48, LINEXY(1), 30, 16};
		S_RECT NumRect2 = {90, LINEXY(1), 18, 16};
		S_RECT NumRect3 = {120, LINEXY(1), 18, 16};
		S_RECT NumRect4 = {48, LINEXY(2), 18, 16};
		S_RECT NumRect5 = {76, LINEXY(2), 18, 16};
		S_RECT NumRect6 = {104, LINEXY(2), 18, 16};
		CreateNumEdit(date.m_Year, NumRect1, 1);
		CreateNumEdit(date.m_Mon, NumRect2, 2);
		CreateNumEdit(date.m_Day, NumRect3, 3);
		CreateNumEdit(date.m_Hour, NumRect4, 4);
		CreateNumEdit(date.m_Min, NumRect5, 5);
		CreateNumEdit(date.m_Sec, NumRect6, 6);
		C_BUTTON *pbtnSet = C_BUTTON::NewCtrl();
		S_RECT BtnRect = BTNRECT(60,LINEXY(5));//{0, 90, 32, 16};
		pbtnSet->Create("确定", 0x00, BtnRect, this, 7);
		SetFocusCtrl(GetWndCtrl(1));
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		pdc->DrawString(STRINGX, LINEXY(0), "终端时间设置");
		pdc->DrawString(STRINGX, LINEXY(1), "日期:");
		pdc->DrawString(80, LINEXY(1), "-");
		pdc->DrawString(110, LINEXY(1), "-");
		pdc->DrawString(STRINGX, LINEXY(2), "时间:");
		pdc->DrawString(68, LINEXY(2), ":");
		pdc->DrawString(96, LINEXY(2), ":");
		pdc->DrawHLine(0, 122, 160);
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F12x12));
		pdc->DrawString(STRINGX, 126, g_TermTimeSet_info[pData->m_InfoIndex]);
		EndPaint(pdc);
		}break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
			RETURNWND();
		else if (wParam == GVK_UP)
			SetFocusCtrl(GetWndCtrl(1));
		else if (wParam == GVK_OK || wParam == GVK_RIGHT)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			int id = pCtrl->GetCtrlID() + 1;
			if (id >= 8) id = 1;
			SetFocusCtrl(GetWndCtrl(id));
		}
		else if (wParam == GVK_LEFT)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			int id = pCtrl->GetCtrlID() - 1;
			if (id <= 0) id = 7;
			SetFocusCtrl(GetWndCtrl(id));
		}
		break;
	case GM_CMD:
		if (wParam == 7 && lParam == GM_CMD_BTN_PUSHED)
		{
			S_DATE date;
			if (CheckTimeFormat(date) == false)
			{
				pData->m_InfoIndex = 1;
				InvalidateRect(NULL);
			}
			else if (SGui_MsgBox("提示", " 确定更改终端时间?", MB_BTN_OKCANCEL) == MB_RTN_YESOK)
			{
				char bcdStr7B[7];
				C_TIME::Time2BCDStr(C_TIME::MakeTime(date), bcdStr7B);
				if (C_TIME::SetHWClock(bcdStr7B) == 0)
					pData->m_InfoIndex = 3;
				else
					pData->m_InfoIndex = 2;
			}
			else
			{
				pData->m_InfoIndex = 0;
			}
		}
		break;
	case GM_DESTROY:
		ReleaseWndData();
		DeleteWndCtrl(1, 7);
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;	
}

bool C_TermTimeSetWnd::CreateNumEdit(int num, S_RECT &NumRect, S_DWORD ID)
{
	char buffer[64];
	C_NUMEDIT *pEdit = C_NUMEDIT::NewCtrl();
	if (ID == 0x01) sprintf(buffer, "%04d", num);
	else sprintf(buffer, "%02d", num);
	return pEdit->Create(buffer, NUMEDIT_STYLE_DEC, NumRect, this, ID);
}

bool C_TermTimeSetWnd::CheckTimeFormat(S_DATE &date)
{
	date.m_Year = atoi(GetWndCtrl(1)->GetWndText().c_str());
	date.m_Mon = atoi(GetWndCtrl(2)->GetWndText().c_str());
	date.m_Day = atoi(GetWndCtrl(3)->GetWndText().c_str());
	date.m_Hour = atoi(GetWndCtrl(4)->GetWndText().c_str());
	date.m_Min = atoi(GetWndCtrl(5)->GetWndText().c_str());
	date.m_Sec = atoi(GetWndCtrl(6)->GetWndText().c_str());
	if (date.m_Year < 2000 || date.m_Year > 2099) return false;
	if (date.m_Mon == 0 || date.m_Mon > 12) return false;
	if (date.m_Day == 0 || date.m_Day > 31) return false;
	if (date.m_Hour > 23) return false;
	if (date.m_Min > 59) return false;
	if (date.m_Sec > 59) return false;
	return true;
}

//终端地址设置窗口
const char *g_RtuaSetInfo[] = {"                ", "终端编号设置成功"};
int C_RtuaSetWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	S_MenuCtrl *pData = static_cast<S_MenuCtrl *>(GetWndData(sizeof(S_MenuCtrl)));
	switch (msg)
	{
	case GM_CREATE:{
		pData->m_InfoIndex = 0;
		S_SYSCFG *pSystemConfig = STATUS.GetSystemConfig();
		C_RTUA rtua(pSystemConfig->m_RTUA);
		S_RECT NumRect = {92, LINEXY(1), 48, 16};
		C_NUMEDIT *pAreaEdit = C_NUMEDIT::NewCtrl();
		sprintf(buffer, "%06X", rtua.GetAreaCode());
		pAreaEdit->Create(buffer, NUMEDIT_STYLE_DEC, NumRect, this, 1);
		NumRect.y += LINEHEIGHT;
		C_NUMEDIT *pAddrEdit = C_NUMEDIT::NewCtrl();
		sprintf(buffer, "%06X", rtua.GetDevAddr());
		pAddrEdit->Create(buffer, NUMEDIT_STYLE_HEX, NumRect, this, 2);
		
		C_BUTTON *pbtnSet = C_BUTTON::NewCtrl();
		S_RECT SetRect = BTNRECT(60,LINEXY(5));//{0, 90, 32, 16};
		pbtnSet->Create("确定", 0x00, SetRect, this, 3);
		SetFocusCtrl(pAreaEdit);
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		pdc->DrawString(STRINGX, LINEXY(0), "终端编号");
		pdc->DrawString(STRINGX, LINEXY(1), "地市区县码:");
		pdc->DrawString(STRINGX, LINEXY(2), "终 端地 址:");
		pdc->DrawString(STRINGX+142, LINEXY(2), "H");
		pdc->DrawHLine(0, 122, 160);
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F12x12));
		pdc->DrawString(STRINGX, 126, g_RtuaSetInfo[pData->m_InfoIndex]);
		EndPaint(pdc);
		}
		break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
			RETURNWND();
		else if (wParam == GVK_UP)
			SetFocusCtrl(GetWndCtrl(1));
		else if (wParam == GVK_OK)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			if (pCtrl->GetCtrlID() == 1)
				SetFocusCtrl(GetWndCtrl(2));
			else if (pCtrl->GetCtrlID() == 2)
				SetFocusCtrl(GetWndCtrl(3));
		}
		break;
	case GM_CMD:
		if (wParam == 3 && lParam == GM_CMD_BTN_PUSHED)
		{
			if (SGui_MsgBox("提示", " 确定更改终端地址?", MB_BTN_OKCANCEL) == MB_RTN_YESOK)
			{
				string strRtua = GetWndCtrl(1)->GetWndText();
				string strAddr = GetWndCtrl(2)->GetWndText();
				strRtua.append(strAddr);
				C_RTUA rtua;
				rtua.ParseAscii(strRtua.c_str());
				S_SYSCFG *pSystemConfig = STATUS.GetSystemConfig();
				pSystemConfig->m_RTUA = rtua.GetRTUA();
				STATUS.SaveSystemConfig();
				pData->m_InfoIndex = 1;
			}
			else
			{
				SetFocusCtrl(GetWndCtrl(1));
				pData->m_InfoIndex = 0;
			}
		}
		break;
	case GM_DESTROY:
		ReleaseWndData();
		DeleteWndCtrl(1, 3);
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;	
}

//文本编辑窗口 APN UserName Password
const char *g_TextEdit_Title[] = {"APN", "用户名", "密码"};
int C_TextEditWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	S_MenuCtrl *pData = static_cast<S_MenuCtrl *>(GetWndData(sizeof(S_MenuCtrl)));
	switch (msg)
	{
	case GM_CREATE:{
		string strInfo;
		pData->m_InfoIndex = 0;
		pData->m_SelectM = (int)lParam;
		S_RECT RectVk = {10, 50, 34, 16};
		S_RECT RectEd = {10, 24, 140, 16};
		C_TEXTEDIT *pEdit = C_TEXTEDIT::NewCtrl();
		GetPppInfo(pData->m_SelectM, strInfo, 21);
		pEdit->Create(strInfo.c_str(), 0x00, RectEd, this, 1);
		C_VKBOARD *pVk = C_VKBOARD::NewCtrl();
		pVk->Create(0x00, RectVk, this, 2);
		SetFocusCtrl(pEdit);
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		sprintf(buffer, "%s编辑:", g_TextEdit_Title[pData->m_SelectM-1]);
		pdc->DrawString(20, 0*18+6, buffer);
		pdc->DrawHLine(0, 122, 160);
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F12x12));
		if(pData->m_InfoIndex == 0) sprintf(buffer, "%s", "              ");
		else sprintf(buffer, "%s保存成功", g_TextEdit_Title[pData->m_SelectM-1]);
		pdc->DrawString(20, 126, buffer);
		EndPaint(pdc);
		}
		break;
	case GM_KEYUP:
		if (wParam == GVK_DN)
			SetFocusCtrl(GetWndCtrl(2));
		else if (wParam == GVK_CANCEL)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			if (pCtrl->GetCtrlID() == 1)
				RETURNWND();
			else
				SetFocusCtrl(GetWndCtrl(1));
		}
		else if (wParam == GVK_OK)
		{
			sprintf(buffer, "  确定更改%s?", g_TextEdit_Title[pData->m_SelectM-1]);
			if (SGui_MsgBox("提示", buffer, MB_BTN_OKCANCEL) == MB_RTN_YESOK)
			{
				string strInfo = GetWndCtrl(1)->GetWndText();
				SavePppInfo(pData->m_SelectM, strInfo);
				pData->m_InfoIndex = 1;
			}
			else
			{
				pData->m_InfoIndex = 0;
			}
		}
		break;
	case GM_CHAR:
		if (wParam == GVK_ENTER || wParam == GVK_ESC || wParam == GVK_F1)
			SetFocusCtrl(GetWndCtrl(1));
		else
			GetWndCtrl(1)->SendWndMsg(GM_CHAR, wParam, lParam);
		break;
	case GM_DESTROY:
		ReleaseWndData();
		DeleteWndCtrl(1, 2);
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;	
}

int C_TextEditWnd::SavePppInfo(int ItemIndex, string &strInfo)
{
	string::size_type start = strInfo.find_first_not_of(' ');
	if (start != string::npos)//去除前面多余的' '
		strInfo.erase(0, start);
	else if (strInfo[0] == ' ')//全是空格
		strInfo.clear();
	string::size_type tail = strInfo.find_last_not_of(' ');
	if (tail != string::npos)//去除后面多余的' '
		strInfo.erase(tail+1, string::npos);
	
	S_PppInfo *pPppInfo = STATUS.GetPppInfo();
	if (ItemIndex == 1)
	{
		memset(pPppInfo->m_DialInfo.m_Apn, 0x00, sizeof(pPppInfo->m_DialInfo.m_Apn));
		strcpy(pPppInfo->m_DialInfo.m_Apn, strInfo.c_str());
	}
	else if (ItemIndex == 2)
	{
		memset(pPppInfo->m_DialInfo.m_User, 0x00, sizeof(pPppInfo->m_DialInfo.m_User));
		strcpy(pPppInfo->m_DialInfo.m_User, strInfo.c_str());
	}
	else if (ItemIndex == 3)
	{
		memset(pPppInfo->m_DialInfo.m_Pwd, 0x00, sizeof(pPppInfo->m_DialInfo.m_Pwd));
		strcpy(pPppInfo->m_DialInfo.m_Pwd, strInfo.c_str());
	}
	STATUS.SavePppInfo();

	return 0;
}

int C_TextEditWnd::GetPppInfo(int ItemIndex, string &strInfo, int EditSize)
{
	S_PppInfo *pPppInfo = STATUS.GetPppInfo();
	if (ItemIndex == 1)
		strInfo.append(pPppInfo->m_DialInfo.m_Apn);
	else if (ItemIndex == 2)
		strInfo.append(pPppInfo->m_DialInfo.m_User);
	else if (ItemIndex == 3)
		strInfo.append(pPppInfo->m_DialInfo.m_Pwd);

	if (strInfo.size() > (size_t)EditSize)
		strInfo.erase(EditSize, string::npos);
	else if (strInfo.size() < (size_t)EditSize)
		strInfo.append(EditSize-strInfo.size(), ' ');

	return (int)strInfo.size();
}



