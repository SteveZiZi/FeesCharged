/** @file
 *  @brief 能量显示窗口定义
 *  @author  yezhengqiu
 *  @date 2012/02/21
 *  @version 0.1
 */
#include "powerwnd.h"
#include "sguiapp.h"
#include "sguictrl.h"
#include "osrelate.h"
#include "syscfg.h"
#include "nwmscmd.h"
#include "sysipc.h"
#include "cfg_proc.h"
#include "basealgo.h"
#include "context.h"
#include "busialgo.h"


#define POWER_ZXYG_BACKWND_TIMEOUT_S	180
#define POWER_ZXYG_RECVCMD_TIMEOUT_S	120
//可实时抄读各测量点正向有功总电量
const char *g_PowerZXYG_info[] = {"请选择测量点...", "正在读取数据...", "发送抄读命令...",
"数据抄读结束...", "数据抄读失败...", "测量点不支持...", "日期格式出错..."};

const char *g_PowerZXYG_title[] =
{
	"正向有功电能示值",
	"正向无功电能示值",
	"反向有功电能示值",
	"反向无功电能示值",
	"一象限无功电能示值",
	"二象限无功电能示值",
	"三象限无功电能示值",
	"四象限无功电能示值",
	"电压",
	"电流",
	"有功功率",
	"无功功率",
	"功率因素",
	"正向有功需量",
	"反向有功需量",

	"最后一个"
};

int C_PowerZXYGWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	S_PowerInfo *pData = static_cast<S_PowerInfo *>(GetWndData(sizeof(S_PowerInfo)));
	switch (msg)
	{
	case GM_CREATE:{
		memset(pData, 0x00, sizeof(*pData));
		pData->m_type = (DataTypeForLCD)lParam; 
		pData->m_BackWndTimeoutS = 0;
		pData->m_RecvCmdTimeoutS = 0;
		pData->m_InfoIndex = 0;
		S_RECT NumRect = {60, 1*18+8, 30, 16};
		C_NUMEDIT *pEdit = C_NUMEDIT::NewCtrl();
		pEdit->Create("0001", NUMEDIT_STYLE_DEC, NumRect, this, 1);
		SetFocusCtrl(pEdit);
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		pdc->DrawString(STRINGX, 0*18+8, g_PowerZXYG_title[pData->m_type]);
		pdc->DrawString(STRINGX, 1*18+8, "测量点:");
		pdc->DrawString(STRINGX, 2*18+8, pData->m_PowerInfo1);
		pdc->DrawString(STRINGX, 3*18+8, pData->m_PowerInfo2);
		pdc->DrawString(STRINGX, 4*18+8, pData->m_PowerInfo3);
		pdc->DrawString(STRINGX, 5*18+8, pData->m_PowerInfo4);
		pdc->DrawHLine(0, 122, 160);
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F12x12));
		pdc->DrawString(STRINGX, 126, g_PowerZXYG_info[pData->m_InfoIndex]);
		EndPaint(pdc);
		}
		break;
	case GM_KEYUP:
		pData->m_BackWndTimeoutS = 0;
		if (wParam == GVK_CANCEL)
		{
			if (pData->m_InfoIndex != 1)//等待数据返回...
				RETURNWND();
			else if (SGui_MsgBox("提示", "  确定退出吗?", MB_BTN_OKCANCEL) == MB_RTN_YESOK)
				RETURNWND();
		}
		else if (wParam == GVK_OK)
		{
			if (pData->m_InfoIndex == 1)//等待数据返回...
				break;
			string strMP = GetWndCtrl(1)->GetWndText();
			int mp = atoi(strMP.c_str());
			if (mp <= 0 || mp >= CONS_METER_COUNT_MAX)
			{
				pData->m_InfoIndex = 5;
			}
			else
			{
				C_SGUIDC dc(this);
				dc.DrawString(STRINGX, 126, g_PowerZXYG_info[2]);
				SendCmd2Dbcj(mp, pData->m_type);
				SleepSecond(1);
				pData->m_InfoIndex = 1;
				pData->m_RecvCmdTimeoutS = 0;
				ClearPowerInfo(*pData);
			}
			InvalidateRect(NULL);
		}
		break;
	case GM_TIME:
		pData->m_BackWndTimeoutS += 1;
		if (pData->m_InfoIndex != 1)//等待数据返回...
			break;
		pData->m_RecvCmdTimeoutS += 1;
		if (RecvCmd4Dbcj(*pData) == true)
		{
			pData->m_InfoIndex = 3;
			InvalidateRect(NULL);
		}
		else if (pData->m_RecvCmdTimeoutS > POWER_ZXYG_RECVCMD_TIMEOUT_S)
		{
			pData->m_RecvCmdTimeoutS = 0;
			ParsePowerInfo(string(""), *pData);
			pData->m_InfoIndex = 4;
			InvalidateRect(NULL);
		}
		break;
	case GM_CMD:
		pData->m_BackWndTimeoutS = 0;
		break;
	case GM_AUTOBACK:
		if (pData->m_BackWndTimeoutS < POWER_ZXYG_BACKWND_TIMEOUT_S)
			return -1;
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

bool C_PowerZXYGWnd::SendCmd2Dbcj(S_WORD mp, DataTypeForLCD type)
{
	string strIpcData;
	S_DWORD DI = 0x00;
	S_WORD DA = C_DAPN::PN2DA(mp);
	const char *pPrecess = CONTEXT.GetDbcjPrecessName_MP(mp);

	switch (type)
	{
	case ZXYGPWRIND:
	case FXYGPWRIND:
		if (type == ZXYGPWRIND) DI = 0x00010000;
		else DI = 0x00020000;
		strIpcData.append((char *)&DA, 2).append((char *)&DI, 4);
		break;
	case VOLTAGE:          //电压
		DI = 0x02010100;strIpcData.append((char *)&DA, 2).append((char *)&DI, 4);
		DI = 0x02010200;strIpcData.append((char *)&DA, 2).append((char *)&DI, 4);
		DI = 0x02010300;strIpcData.append((char *)&DA, 2).append((char *)&DI, 4);
		break;
	case CURRENT:          //电流
		DI = 0x02020100;strIpcData.append((char *)&DA, 2).append((char *)&DI, 4);
		DI = 0x02020200;strIpcData.append((char *)&DA, 2).append((char *)&DI, 4);
		DI = 0x02020300;strIpcData.append((char *)&DA, 2).append((char *)&DI, 4);
		break;
	case ACTPWR:           //有功功率
		DI = 0x02030000;strIpcData.append((char *)&DA, 2).append((char *)&DI, 4);
		DI = 0x02030100;strIpcData.append((char *)&DA, 2).append((char *)&DI, 4);
		DI = 0x02030200;strIpcData.append((char *)&DA, 2).append((char *)&DI, 4);
		DI = 0x02030300;strIpcData.append((char *)&DA, 2).append((char *)&DI, 4);
		break;
	default:
		return false;
	}
	//判断表计是交采吗
	if (C_SysMsg::Send2Server(pPrecess, MSAFN_READ_REALTIME, strIpcData) != 0)
		return false;
	return true;
}

bool C_PowerZXYGWnd::RecvCmd4Dbcj(S_PowerInfo &PowerInfo)
{
	string strMsg;
	if (C_SysMsg::Recv4Server(MSAFN_READ_REALTIME, strMsg) != 0)
		return false;

	return ParsePowerInfo(strMsg, PowerInfo);
}

bool C_PowerZXYGWnd::ClearPowerInfo(S_PowerInfo &PowerInfo)
{
	memset(PowerInfo.m_PowerInfo1, 0x00, sizeof(PowerInfo.m_PowerInfo1));
	memset(PowerInfo.m_PowerInfo2, 0x00, sizeof(PowerInfo.m_PowerInfo2));
	memset(PowerInfo.m_PowerInfo3, 0x00, sizeof(PowerInfo.m_PowerInfo3));
	memset(PowerInfo.m_PowerInfo4, 0x00, sizeof(PowerInfo.m_PowerInfo4));
	return true;
}

bool C_PowerZXYGWnd::ParsePowerInfo(const string &strInfo, S_PowerInfo &PowerInfo)
{
	string strData, strData1, strData2, strData3;
	string strMyInfo(strInfo);

	switch (PowerInfo.m_type)
	{
	case ZXYGPWRIND:
	case FXYGPWRIND:
		if (strMyInfo.size() != 10)// 2(DA)+4(DI)+4(DATA)
			strMyInfo.assign((size_t)10, (char)0xEE);
		strData = C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strMyInfo.data()+6, 6, 2, false);
		if (PowerInfo.m_type == ZXYGPWRIND) strcpy(PowerInfo.m_PowerInfo2, "正有:");
		else strcpy(PowerInfo.m_PowerInfo2, "反有:");
		strcat(PowerInfo.m_PowerInfo2, strData.c_str());
		strcat(PowerInfo.m_PowerInfo2, "kWh");
		break;
	case VOLTAGE:          //电压
		if (strMyInfo.size() != (3*8))// 2(DA)+4(DI)+2(DATA)
			strMyInfo.assign((size_t)(3*8), (char)0xEE);
		strData1 = C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strMyInfo.data()+0+6, 3, 1, false);
		strData2 = C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strMyInfo.data()+8+6, 3, 1, false);
		strData3 = C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strMyInfo.data()+16+6, 3, 1, false);

		strcpy(PowerInfo.m_PowerInfo1, " A相:");
		strcpy(PowerInfo.m_PowerInfo2, " B相:");
		strcpy(PowerInfo.m_PowerInfo3, " C相:");

		strcat(PowerInfo.m_PowerInfo1, strData1.c_str());
		strcat(PowerInfo.m_PowerInfo2, strData2.c_str());
		strcat(PowerInfo.m_PowerInfo3, strData3.c_str());

		strcat(PowerInfo.m_PowerInfo1, "V");
		strcat(PowerInfo.m_PowerInfo2, "V");
		strcat(PowerInfo.m_PowerInfo3, "V");
		break;
	case CURRENT:          //电流
		if (strMyInfo.size() != (3*9))// 2(DA)+4(DI)+3(DATA)
			strMyInfo.assign((size_t)(3*9), (char)0xEE);
		strData1 = C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strMyInfo.data()+0+6, 3, 3, true);
		strData2 = C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strMyInfo.data()+9+6, 3, 3, true);
		strData3 = C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strMyInfo.data()+18+6, 3, 3, true);

		strcpy(PowerInfo.m_PowerInfo1, " A相:");
		strcpy(PowerInfo.m_PowerInfo2, " B相:");
		strcpy(PowerInfo.m_PowerInfo3, " C相:");

		strcat(PowerInfo.m_PowerInfo1, strData1.c_str());
		strcat(PowerInfo.m_PowerInfo2, strData2.c_str());
		strcat(PowerInfo.m_PowerInfo3, strData3.c_str());

		strcat(PowerInfo.m_PowerInfo1, "A");
		strcat(PowerInfo.m_PowerInfo2, "A");
		strcat(PowerInfo.m_PowerInfo3, "A");
		break;
	case ACTPWR:           //有功功率
		if (strMyInfo.size() != (4*9))// 2(DA)+4(DI)+3(DATA)
			strMyInfo.assign((size_t)(4*9), (char)0xEE);
		strData = C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strMyInfo.data()+0+6, 2, 4, true);
		strData1 = C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strMyInfo.data()+9+6, 2, 4, true);
		strData2 = C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strMyInfo.data()+18+6, 2, 4, true);
		strData3 = C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strMyInfo.data()+27+6, 2, 4, true);

		strcpy(PowerInfo.m_PowerInfo1, "  总:");
		strcpy(PowerInfo.m_PowerInfo2, " A相:");
		strcpy(PowerInfo.m_PowerInfo3, " B相:");
		strcpy(PowerInfo.m_PowerInfo4, " C相:");

		strcat(PowerInfo.m_PowerInfo1, strData.c_str());
		strcat(PowerInfo.m_PowerInfo2, strData1.c_str());
		strcat(PowerInfo.m_PowerInfo3, strData2.c_str());
		strcat(PowerInfo.m_PowerInfo4, strData3.c_str());

		strcat(PowerInfo.m_PowerInfo1, "kW");
		strcat(PowerInfo.m_PowerInfo2, "kW");
		strcat(PowerInfo.m_PowerInfo3, "kW");
		strcat(PowerInfo.m_PowerInfo4, "kW");
		break;
	default:
		return false;
	}

	return true;
}

//轮显
/*
int C_TURNVIEW::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	switch (msg)
	{
	case GM_CREATE:
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->DrawHLine(0, 122, 160);
		EndPaint(pdc);
		}
		break;
	case GM_KEYUP:
		RETURNWND();
		break;
	case GM_DESTROY:
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;	
}
*/

static const char *g_MeasPntDataDisplayWndTitle[] =
{
	"日冻结正向有功总",
	"日冻结正向无功总",
	"日冻结反向有功总",
	"日冻结反向无功总",
	"日冻结一象限无功",
	"日冻结二象限无功",
	"日冻结三象限无功",
	"日冻结四象限无功",
	"日冻结日电压(V)",
	"日冻结日电流(A)",
	"日冻结有功功率",
	"日冻结无功功率",
	"日冻结功率因数",
	"日冻结正向有功需量",
	"日冻结反向有功需量",

	"MAX"
};
//测量点数据显示
int C_MeasPntDataDisplayWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	S_PowerInfo *pData = static_cast<S_PowerInfo *>(GetWndData(sizeof(S_PowerInfo)));
	switch (msg)
	{
	case GM_CREATE:{
		//initiliaze members.
		memset(pData, 0x00, sizeof(*pData));
		pData->m_type = (DataTypeForLCD)lParam;
		pData->m_BackWndTimeoutS = 0;
		pData->m_RecvCmdTimeoutS = 0;
		pData->m_InfoIndex = 0;

		SetFnValue(pData->m_type);
		//get date, we need the date value of yesterda;
		timeFreeze = C_TIME::DateAdd(TIME_UNIT_DAY, -1, C_TIME::TimeNow());

		//create edit rect
		S_DATE date;
		C_TIME::LocalTime(timeFreeze, date);

		S_RECT Rect = {58, 1*18+2, 18, 16};   //x,y,w,h
		CreateNumEdit(date.m_Year, Rect, 1);
		Rect.x += 34;
		CreateNumEdit(date.m_Mon, Rect, 2);
		Rect.x += 34;
		CreateNumEdit(date.m_Day, Rect, 3);

		S_RECT MpRect = {60, 2*18+2, 30, 16};
		C_NUMEDIT *pMpEdit = C_NUMEDIT::NewCtrl();
		pMpEdit->Create("0001", NUMEDIT_STYLE_DEC, MpRect, this, 4);

		S_RECT BtnRect =  {94, 2*18+2, 32, 16};
		C_BUTTON *pbtnSet = C_BUTTON::NewCtrl();
		pbtnSet->Create("确定", 0x00, BtnRect, this, 5);

		SetFocusCtrl(pMpEdit);
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		pdc->DrawString(STRINGX, 0*18+2, g_MeasPntDataDisplayWndTitle[pData->m_type]);
		pdc->DrawString(STRINGX, 1*18+2, "时标:");
		pdc->DrawString(STRINGX+40, 1*18+2, "20");
		pdc->DrawString(STRINGX+78, 1*18+2, "-");
		pdc->DrawString(STRINGX+112, 1*18+2, "-");
		pdc->DrawString(STRINGX, 2*18+2, "测量点:");
		pdc->DrawString(STRINGX, 3*18+2, pData->m_PowerInfo1);
		pdc->DrawString(STRINGX, 4*18+2, pData->m_PowerInfo2);
		pdc->DrawHLine(0, 122, 160);
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F12x12));
		pdc->DrawString(STRINGX, 126, g_PowerZXYG_info[pData->m_InfoIndex]);
		EndPaint(pdc);
		}
		break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
		{
			if (pData->m_InfoIndex != 1)//等待数据返回...
				RETURNWND();
			else if (SGui_MsgBox("提示", "  确定退出吗?", MB_BTN_OKCANCEL) == MB_RTN_YESOK)
				RETURNWND();
		}
		else if (wParam == GVK_OK || wParam == GVK_RIGHT || wParam == GVK_DN)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			int id = pCtrl->GetCtrlID() + 1;
			if (id >= 6) id = 1;
			SetFocusCtrl(GetWndCtrl(id));
		}
		else if (wParam == GVK_LEFT || wParam == GVK_UP)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			int id = pCtrl->GetCtrlID() - 1;
			if (id <= 0) id = 5;
			SetFocusCtrl(GetWndCtrl(id));
		}
		break;
	case GM_CMD:
		pData->m_BackWndTimeoutS = 0;
		if (wParam == 5 && lParam == GM_CMD_BTN_PUSHED)
		{
			S_DATE date;
			if (CheckDateFormat(date) == false)
			{
				pData->m_InfoIndex = 6;
			}
			else
			{
				//set the time
				timeFreeze = C_TIME::MakeTime(date);
				if (pData->m_InfoIndex == 1)//等待数据返回...
					break;
				string strMP = GetWndCtrl(4)->GetWndText();
				measurePoint = atoi(strMP.c_str());
				if (measurePoint <= 0 || measurePoint >= CONS_METER_COUNT_MAX)
				{
					pData->m_InfoIndex = 5;
				}
				else
				{
					C_SGUIDC dc(this);
					dc.DrawString(STRINGX, 126, g_PowerZXYG_info[2]);
					pData->m_InfoIndex = 1;
					pData->m_RecvCmdTimeoutS = 0;
					ClearPowerInfo(*pData);
				}
			}
			InvalidateRect(NULL);
		}
		break;
	case GM_TIME:
		pData->m_BackWndTimeoutS += 1;
		if (pData->m_InfoIndex != 1)//等待数据返回...
			break;
		pData->m_RecvCmdTimeoutS += 1;
	
		if (readPwrData(*pData, timeFreeze) == true)
		{
			pData->m_InfoIndex = 3;
			InvalidateRect(NULL);
		}
		else if (pData->m_RecvCmdTimeoutS >= 1)//POWER_ZXYG_RECVCMD_TIMEOUT_S)
		{
			// for common handling, 
			pData->m_RecvCmdTimeoutS = 0;
			ParseData(string(""), *pData);
			pData->m_InfoIndex = 4;
			InvalidateRect(NULL);
		}
		break;
	case GM_AUTOBACK:
		if (pData->m_BackWndTimeoutS < POWER_ZXYG_BACKWND_TIMEOUT_S)
			return -1;
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

bool C_MeasPntDataDisplayWnd::ClearPowerInfo(S_PowerInfo &PowerInfo)
{
	memset(PowerInfo.m_PowerInfo1, 0x00, sizeof(PowerInfo.m_PowerInfo1));
	memset(PowerInfo.m_PowerInfo2, 0x00, sizeof(PowerInfo.m_PowerInfo2));
	memset(PowerInfo.m_PowerInfo3, 0x00, sizeof(PowerInfo.m_PowerInfo3));
	memset(PowerInfo.m_PowerInfo4, 0x00, sizeof(PowerInfo.m_PowerInfo4));
	return true;
}

bool C_MeasPntDataDisplayWnd::SetFnValue(DataTypeForLCD type)
{
	S_LcdInfo *pLcdInfo = STATUS.GetLcdInfo();
	switch (type)
	{
		case ZXYGPWRIND:FnValue = pLcdInfo->m_ZXYGFn;break;
		case ZXWGPWRIND:FnValue = pLcdInfo->m_ZXWGFn;break;
		case FXYGPWRIND:FnValue = pLcdInfo->m_FXYGFn;break;
		case FXWGPWRIND:FnValue = pLcdInfo->m_FXWGFn;break;
		case YXXWGPWRIND:FnValue = pLcdInfo->m_YXXWGFn;break;
		case EXXWGPWRIND:FnValue = pLcdInfo->m_EXXWGFn;break;
		case SXXWGPWRIND:FnValue = pLcdInfo->m_SXXWGFn;break;
		case FXXWGPWRIND:FnValue = pLcdInfo->m_FXXWGFn;break;
		case VOLTAGE:FnValue = pLcdInfo->m_VOLTFn;break;
		case CURRENT:FnValue = pLcdInfo->m_CURRFn;break;
		case ACTPWR:FnValue = pLcdInfo->m_ACTPWRFn;break;
		case WATTLESSPWR:FnValue = pLcdInfo->m_WATTLESSPWRFn;break;
		case PWRFACTOR:FnValue = pLcdInfo->m_PWRFACTORFn;break;
		case ZXYGXL:FnValue = pLcdInfo->m_ZXYGXLFn;break;
		case FXYGXL:FnValue = pLcdInfo->m_FXYGXLFn;break;
		default:
			return false;
	}
	return true;
}

bool C_MeasPntDataDisplayWnd::CreateNumEdit(int num, S_RECT &NumRect, S_DWORD ID)
{
	char buffer[64];
	C_NUMEDIT *pEdit = C_NUMEDIT::NewCtrl();
	num %= 100;
	sprintf(buffer, "%02d", num);
	return pEdit->Create(buffer, NUMEDIT_STYLE_DEC, NumRect, this, ID);
}

bool C_MeasPntDataDisplayWnd::CheckDateFormat(S_DATE &date)
{
	string strData;
	strData = GetWndCtrl(1)->GetWndText(), date.m_Year = atoi(strData.c_str()) + 2000;
	strData = GetWndCtrl(2)->GetWndText(), date.m_Mon = atoi(strData.c_str());
	strData = GetWndCtrl(3)->GetWndText(), date.m_Day = atoi(strData.c_str());

	S_DATE CheckDate;
	C_TIME::LocalTime(C_TIME::MakeTime(date), CheckDate);
	if (CheckDate.m_Year == date.m_Year && CheckDate.m_Mon == date.m_Mon && CheckDate.m_Day == date.m_Day)
		return true;
	return false;
}

//the code will be restructured in the future
bool C_MeasPntDataDisplayWnd::readPwrData(S_PowerInfo &PowerInfo, S_TIME timeFreeze)
{
	//sanity check here???
	string strData;	
	if (C_Class2Data::Read(FnValue, measurePoint, strData, timeFreeze) != 0)
	{
		//read file error
		loget<<"read file error, FN = "<<FnValue<<",measurePoint = "<<measurePoint<<endl;
		return false;
	}
	if (strData.empty() == true)
	{
		//software error
		loget<<"no data found for FN= "<<FnValue<<",measurePoint = "<<measurePoint<<endl;
		return false;
	}

	//for debug
	logdt<<"read file correctly, FN= "<<FnValue<<",measurePoint = "<<measurePoint
		<<",strData:"<<HEX<<strData<<endl;

	return ParseData(strData, PowerInfo);
}

bool C_MeasPntDataDisplayWnd::ParseData(const string &strData, S_PowerInfo &PowerInfo)
{
	string strMyData;
	string strMyInfo(strData);

	switch (PowerInfo.m_type)
	{
	case ZXYGPWRIND:
	case FXYGPWRIND:
		if (strMyInfo.size() != 4)// 4(DATA)
			strMyInfo.assign((size_t)4, (char)0xEE);
		strMyData = C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strMyInfo.data(), 6, 2, false);
		if (PowerInfo.m_type == ZXYGPWRIND) strcpy(PowerInfo.m_PowerInfo2, "正有:");
		else strcpy(PowerInfo.m_PowerInfo2, "反有:");
		strcat(PowerInfo.m_PowerInfo2, strMyData.c_str());
		strcat(PowerInfo.m_PowerInfo2, "kWh");
		break;
	default:
		return false;
	}

	return true;
}

struct S_HistoryPower
{
	int m_idx;
	char m_PowerInfo[64];
};
//日冻结电量
const char *g_HistoryDayWndInfo[] = {"            ", "时间格式错误"};
int C_HistoryDayWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	S_HistoryPower *pData = static_cast<S_HistoryPower *>(GetWndData(sizeof(S_HistoryPower)));
	switch (msg)
	{
	case GM_CREATE:{
		S_DATE date;
		memset(pData, 0x00, sizeof(S_HistoryPower));
		S_TIME mytime = C_TIME::DateAdd(TIME_UNIT_DAY, -1, C_TIME::TimeNow());
		C_TIME::LocalTime(mytime, date);

		S_RECT NumRect = {66, 1*17+6, 18, 16};
		S_RECT MpRect = {64, 2*17+6, 30, 16};

		C_NUMEDIT *pNum1Edit = C_NUMEDIT::NewCtrl();
		sprintf(buffer, "%02d", date.m_Year % 100);
		pNum1Edit->Create(buffer, NUMEDIT_STYLE_DEC, NumRect, this, 1);
		C_NUMEDIT *pNum2Edit = C_NUMEDIT::NewCtrl();
		NumRect.x += 32;
		sprintf(buffer, "%02d", date.m_Mon);
		pNum2Edit->Create(buffer, NUMEDIT_STYLE_DEC, NumRect, this, 2);
		C_NUMEDIT *pNum3Edit = C_NUMEDIT::NewCtrl();
		NumRect.x += 32;
		sprintf(buffer, "%02d", date.m_Day);
		pNum3Edit->Create(buffer, NUMEDIT_STYLE_DEC, NumRect, this, 3);

		C_NUMEDIT *pMpEdit = C_NUMEDIT::NewCtrl();
		pMpEdit->Create("0000", NUMEDIT_STYLE_DEC, MpRect, this, 4);
		SetFocusCtrl(pMpEdit);
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		pdc->DrawString(STRINGX, 0*17+6, "日正向有功电量");
		pdc->DrawString(STRINGX, 1*17+6, "时间: 20");
		pdc->DrawString(STRINGX+86, 1*17+6, "-");
		pdc->DrawString(STRINGX+118, 1*17+6, "-");
		pdc->DrawString(STRINGX, 2*17+6, "测量点:");
		pdc->DrawString(STRINGX, 4*17+6, pData->m_PowerInfo);
		pdc->DrawHLine(0, 122, 160);
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F12x12));
		pdc->DrawString(20, 126, g_HistoryDayWndInfo[pData->m_idx]);
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
			if (pCtrl->GetCtrlID() == 4)
				SetFocusCtrl(GetWndCtrl(1));
			else
				SetFocusCtrl(GetWndCtrl(pCtrl->GetCtrlID()+1));
		}
		else if (wParam == GVK_LEFT)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			if (pCtrl->GetCtrlID() == 1)
				SetFocusCtrl(GetWndCtrl(4));
			else
				SetFocusCtrl(GetWndCtrl(pCtrl->GetCtrlID()-1));
		}
		break;
	case GM_CMD:
		if (wParam == 4 && lParam == GM_CMD_CTRL_UPDATE)
		{
			S_DATE date;
			string strMP = GetWndCtrl(4)->GetWndText();
			int MP = atoi(strMP.c_str());
			if (GetDate(date) == false)
			{
				pData->m_idx = 1;
				memset(pData->m_PowerInfo, 0x00, sizeof(pData->m_PowerInfo));
			}
			else if (MP > 0 && MP < CONS_MP_COUNT_MAX)
			{
				pData->m_idx = 0;
				HandleDayData(MP, 0x05060100, C_TIME::MakeTime(date), pData->m_PowerInfo);
			}
			else
			{
				pData->m_idx = 0;
				memset(pData->m_PowerInfo, 0x00, sizeof(pData->m_PowerInfo));
			}
			InvalidateRect(NULL);
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

bool C_HistoryDayWnd::GetDate(S_DATE &date)
{
	string strData;
	int year = 0, mon = 0, day = 0;

	strData = GetWndCtrl(1)->GetWndText(), year = atoi(strData.c_str());
	strData = GetWndCtrl(2)->GetWndText(), mon = atoi(strData.c_str());
	strData = GetWndCtrl(3)->GetWndText(), day = atoi(strData.c_str());

	date.m_Year = 2000 + year;
	date.m_Mon = mon;
	date.m_Day = day;
	date.m_Hour = 0;
	date.m_Min = 0;
	date.m_Sec = 0;

	return C_TIME::CheckTime(date);
}

bool C_HistoryDayWnd::HandleDayData(S_WORD MP, S_DWORD FN, S_TIME DataTime, char *pBuffer)
{
	string strData1, strData2;
	C_Class2Data::Read(FN, MP, strData1, DataTime);
	C_Class2Data::Read(FN, MP, strData2, C_TIME::DateAdd(TIME_UNIT_DAY, -1, DataTime));
	if (strData1.empty() == true || strData2.empty() == true)
	{
		strcpy(pBuffer, "电量: 无数据");
		return false;
	}

	C_BCD bcdData1((S_BYTE *)strData1.data(), 6, 2);
	C_BCD bcdData2((S_BYTE *)strData2.data(), 6, 2);
	if (!bcdData1 || !bcdData2)
	{
		strcpy(pBuffer, "电量: ------.--kWh");
		return false;
	}
	if (bcdData1 < bcdData2)
	{
		strcpy(pBuffer, "电量: ------.-- kWh");
		return false;
	}

	C_BCD bcdData = bcdData1 - bcdData2;
	string strData = bcdData;
	sprintf(pBuffer, "电量: %s kWh", strData.c_str());
	return true;
}

//月冻结电量
int C_HistoryMonWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	S_HistoryPower *pData = static_cast<S_HistoryPower *>(GetWndData(sizeof(S_HistoryPower)));
	switch (msg)
	{
	case GM_CREATE:{
		S_DATE date;
		memset(pData, 0x00, sizeof(S_HistoryPower));
		S_TIME mytime = C_TIME::DateAdd(TIME_UNIT_MONTH, -1, C_TIME::TimeNow());
		C_TIME::LocalTime(mytime, date);

		S_RECT NumRect = {66, 1*17+6, 18, 16};
		S_RECT MpRect = {64, 2*17+6, 30, 16};

		C_NUMEDIT *pNum1Edit = C_NUMEDIT::NewCtrl();
		sprintf(buffer, "%02d", date.m_Year % 100);
		pNum1Edit->Create(buffer, NUMEDIT_STYLE_DEC, NumRect, this, 1);
		C_NUMEDIT *pNum2Edit = C_NUMEDIT::NewCtrl();
		NumRect.x += 32;
		sprintf(buffer, "%02d", date.m_Mon);
		pNum2Edit->Create(buffer, NUMEDIT_STYLE_DEC, NumRect, this, 2);
		C_NUMEDIT *pNum3Edit = C_NUMEDIT::NewCtrl();
		NumRect.x += 32;
		sprintf(buffer, "%02d", date.m_Day);
		pNum3Edit->Create(buffer, NUMEDIT_STYLE_DEC, NumRect, this, 3);

		C_NUMEDIT *pMpEdit = C_NUMEDIT::NewCtrl();
		pMpEdit->Create("0000", NUMEDIT_STYLE_DEC, MpRect, this, 4);
		SetFocusCtrl(pMpEdit);
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		pdc->DrawString(STRINGX, 0*17+6, "月正向有功电量");
		pdc->DrawString(STRINGX, 1*17+6, "时间: 20");
		pdc->DrawString(STRINGX+86, 1*17+6, "-");
		pdc->DrawString(STRINGX+118, 1*17+6, "-");
		pdc->DrawString(STRINGX, 2*17+6, "测量点:");
		pdc->DrawString(STRINGX, 4*17+6, pData->m_PowerInfo);
		pdc->DrawHLine(0, 122, 160);
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F12x12));
		pdc->DrawString(20, 126, g_HistoryDayWndInfo[pData->m_idx]);
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
			if (pCtrl->GetCtrlID() == 4)
				SetFocusCtrl(GetWndCtrl(1));
			else
				SetFocusCtrl(GetWndCtrl(pCtrl->GetCtrlID()+1));
		}
		else if (wParam == GVK_LEFT)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			if (pCtrl->GetCtrlID() == 1)
				SetFocusCtrl(GetWndCtrl(4));
			else
				SetFocusCtrl(GetWndCtrl(pCtrl->GetCtrlID()-1));
		}
		break;
	case GM_CMD:
		if (wParam == 4 && lParam == GM_CMD_CTRL_UPDATE)
		{
			S_DATE date;
			string strMP = GetWndCtrl(4)->GetWndText();
			int MP = atoi(strMP.c_str());
			if (GetDate(date) == false)
			{
				pData->m_idx = 1;
				memset(pData->m_PowerInfo, 0x00, sizeof(pData->m_PowerInfo));
			}
			else if (MP > 0 && MP < CONS_MP_COUNT_MAX)
			{
				pData->m_idx = 0;
				HandleMonData(MP, 0x00010001, C_TIME::MakeTime(date), pData->m_PowerInfo);
			}
			else
			{
				pData->m_idx = 0;
				memset(pData->m_PowerInfo, 0x00, sizeof(pData->m_PowerInfo));
			}
			InvalidateRect(NULL);
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

bool C_HistoryMonWnd::HandleMonData(S_WORD MP, S_DWORD FN, S_TIME DataTime, char *pBuffer)
{
	string strData1, strData2;
	C_Class2Data::Read(FN, MP, strData1, DataTime);
	C_Class2Data::Read(FN, MP, strData2, C_TIME::DateAdd(TIME_UNIT_MONTH, -1, DataTime));
	if (strData1.empty() == true || strData2.empty() == true)
	{
		strcpy(pBuffer, "电量: 无数据");
		return false;
	}

	C_BCD bcdData1((S_BYTE *)strData1.data(), 6, 2);
	C_BCD bcdData2((S_BYTE *)strData2.data(), 6, 2);
	if (!bcdData1 || !bcdData2)
	{
		strcpy(pBuffer, "电量: ------.--kWh");
		return false;
	}
	if (bcdData1 < bcdData2)
	{
		strcpy(pBuffer, "电量: ------.-- kWh");
		return false;
	}

	C_BCD bcdData = bcdData1 - bcdData2;
	string strData = bcdData;
	sprintf(pBuffer, "电量: %s kWh", strData.c_str());
	return true;
}

//===========================================================================




