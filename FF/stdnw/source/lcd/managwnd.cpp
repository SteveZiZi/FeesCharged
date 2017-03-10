/** @file
 *  @brief 终端管理与维护窗口定义
 *  @author  yezhengqiu
 *  @date 2012/02/21
 *  @version 0.1
 */
#include "sguiapp.h"
#include "managwnd.h"
#include "version.h"
#include "syscfg.h"
#include "sysctrl.h"
#include "context.h"
#include "status.h"
#include "modem.h"
#include "sguictrl.h"
#include "almshow.h"

const char *g_TermName[] ={"厂站终端","专变终端","公变终端","集中器终端"};
int C_ShowVersionWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	switch (msg)
	{
	case GM_CREATE:
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		pdc->DrawString(STRINGX, 0*18+8, "终端版本信息");
		sprintf(buffer, "厂商编码:%s", "NDKJ");
		pdc->DrawString(STRINGX, 1*18+8, buffer);
		sprintf(buffer, "终端类型:%s", g_TermName[CONS_TERM_TYPE-1]);
		pdc->DrawString(STRINGX, 2*18+8, buffer);
		sprintf(buffer, "硬件版本号:%s", CONS_HARD_VERSION);
		pdc->DrawString(STRINGX, 3*18+8, buffer);
		sprintf(buffer, "软件版本号:%s", CONS_MAIN_VERSION);
		pdc->DrawString(STRINGX, 4*18+8, buffer);
		sprintf(buffer, "发布日期:%s", CONS_SW_RELEASE_DATE);
		pdc->DrawString(STRINGX, 5*18+8, buffer);
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

const char *g_RebootMenu[] = {"参数区复位","数据区复位","终端硬件复位"};
const char *g_RebootWndInfo[] = {"               ", "系统准备重启...", "系统复位完成..."};
int C_RebootWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	S_MenuCtrl *pData = static_cast<S_MenuCtrl *>(GetWndData(sizeof(S_MenuCtrl)));
	switch (msg)
	{
	case GM_CREATE:
		pData->m_SelectM = 0;
		pData->m_MenuCnt = (int)sizeof(g_RebootMenu)/sizeof(g_RebootMenu[0]);
		pData->m_InfoIndex = 0;
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		for (int i = 0; i < pData->m_MenuCnt; i++)
		{
			sprintf(buffer, "%s", g_RebootMenu[i]);
			pdc->DrawString(STRINGX, i*18+8, buffer);
		}
		S_RECT rect = {STRINGX, pData->m_SelectM*18+8, STRINGW, 16};
		pdc->ReverseRect(rect);
		pdc->DrawHLine(0, 122, 160);
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F12x12));
		pdc->DrawString(STRINGX, 126, g_RebootWndInfo[pData->m_InfoIndex]);
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
			if (C_PWD::Certification() == false)
				break;
			sprintf(buffer, "   %s!", g_RebootMenu[pData->m_SelectM]);
			if (SGui_MsgBox("警告", buffer, MB_BTN_OKCANCEL) != MB_RTN_YESOK)
				break;
			if (pData->m_SelectM == 0) C_SYSCTRL::ParamReset();
			else if (pData->m_SelectM == 1) C_SYSCTRL::DataClear();
			else if (pData->m_SelectM == 2) C_SYSCTRL::HardwareReset();
			if (pData->m_SelectM == 1) pData->m_InfoIndex = 2;
			else pData->m_InfoIndex = 1;
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

//告警查询	可提供最新至少100条告警信息查询（包括告警代码、中文显示事件内容、发生时间等）
const char *g_QueryAlarmMenu[] = {"查询重要告警","查询普通告警","查询事件记录"};
const char *g_QueryAlarmWndInfo[] = {"新告警记录:", "新告警记录:", "新事件记录:"};
int C_QueryAlarmWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	S_MenuCtrl *pData = static_cast<S_MenuCtrl *>(GetWndData(sizeof(S_MenuCtrl)));
	switch (msg)
	{
	case GM_CREATE:
		pData->m_SelectM = 0;
		pData->m_MenuCnt = (int)sizeof(g_QueryAlarmMenu)/sizeof(g_QueryAlarmMenu[0]);
		pData->m_InfoIndex = 0;
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		for (int i = 0; i < pData->m_MenuCnt; i++)
		{
			sprintf(buffer, " %s", g_QueryAlarmMenu[i]);
			pdc->DrawString(14, i*18+8, buffer);
		}
		S_RECT rect = {12, pData->m_SelectM*18+6, 138, 16};
		pdc->ReverseRect(rect);
		pdc->DrawHLine(0, 122, 160);
		int RecordCnt = 0;
		S_AlarmStatus *pAS = STATUS.GetAlarmStatus();
		if (pData->m_SelectM == 0) RecordCnt = pAS->m_ImportantNewCnt;
		else if (pData->m_SelectM == 1) RecordCnt = pAS->m_NormalNewCnt;
		else if (pData->m_SelectM == 2) RecordCnt = pAS->m_EventNewCnt;
		sprintf(buffer, "%s %d条", g_QueryAlarmWndInfo[pData->m_SelectM], RecordCnt);
		pdc->DrawString(20, 126, buffer);
		EndPaint(pdc);
		}break;
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
			GOTOWND(C_PrintAlarmWnd, (S_DWORD)(pData->m_SelectM+1));
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

//查询抄表统计信息:
int C_QueryGatherInfo::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	switch (msg)
	{
	case GM_CREATE:
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		
		S_ALLMP *pAllMP = STATUS.GetALLMP();
		S_TermData *TD=STATUS.GetTermData();
		S_WORD MeterTotal = (S_WORD)pAllMP->m_MTBS.m_AllMPBS.count();
		S_WORD MeterDaySuccess = (S_WORD)TD->m_GatherStatInfos.m_DayInfo.m_mpbsSuccess.count();
		S_WORD MeterMonSuccess = (S_WORD)TD->m_GatherStatInfos.m_MonInfo.m_mpbsSuccess.count();
		if(MeterTotal == 0) {MeterDaySuccess = 0, MeterMonSuccess = 0, MeterTotal = 1;} //防止出错
		if (MeterDaySuccess > MeterTotal) MeterDaySuccess = MeterTotal;
		if (MeterMonSuccess > MeterTotal) MeterMonSuccess = MeterTotal;

		sprintf(buffer, "共有电表: %d 只", MeterTotal);
		pdc->DrawString(STRINGX, LINEXY(0), buffer);
		sprintf(buffer, "日抄收成功率:%0.1f%%",((FP32)MeterDaySuccess/(FP32)MeterTotal)*100.0);
		pdc->DrawString(STRINGX, LINEXY(1), buffer);
		sprintf(buffer, "月抄收成功率:%0.1f%%",((FP32)MeterMonSuccess/(FP32)MeterTotal)*100.0);
		pdc->DrawString(STRINGX, LINEXY(2), buffer);

		pdc->DrawHLine(0, 122, 160);
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F12x12));
		pdc->DrawString(20, 126, CONTEXT.GetLcdBottomInfo(MP_PROCESS_DBCJ1));
		EndPaint(pdc);
		}
		break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
			RETURNWND();
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

//告警数据显示
int C_PrintAlarmWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	S_MenuScrollCtrl *pData = static_cast<S_MenuScrollCtrl *>(GetWndData(sizeof(S_MenuScrollCtrl)));
	switch (msg)
	{
	case GM_CREATE:{
		ReadAlarmData(1);		
		pData->m_InfoIndex = 0;
		if (!m_strReocrds.empty())
			pData->m_InfoIndex = 1;
		pData->m_LineOfScreen = 7;
		pData->m_SelectM = 0;
		pData->m_StartM = 0;
		PrintAlarm(pData->m_InfoIndex);
		pData->m_MenuCnt = (int)m_strTexts.size();
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		for (int i = 0; i < pData->m_LineOfScreen; i++)
		{
			if (i + pData->m_StartM < (int)m_strTexts.size())
				pdc->DrawString(STRINGX, i*17+2, m_strTexts[i + pData->m_StartM].c_str());
		}
		pdc->DrawHLine(0, 122, 160);
		sprintf(buffer, "第%d条, 共%d条", pData->m_InfoIndex, m_strReocrds.size());
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F12x12));
		pdc->DrawString(STRINGX, 126, buffer);
		EndPaint(pdc);
		}break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
			RETURNWND();
		if (m_strReocrds.empty())
			break;
		if (wParam == GVK_DN)
		{
			if (pData->m_StartM + pData->m_LineOfScreen < pData->m_MenuCnt)
			{
				pData->m_StartM += pData->m_LineOfScreen;
				InvalidateRect(NULL);
			}
		}
		else if (wParam == GVK_UP)
		{
			if (pData->m_StartM > 0)
			{
				pData->m_StartM -= pData->m_LineOfScreen;
				InvalidateRect(NULL);
			}
		}
		else if (wParam == GVK_LEFT)
		{
			pData->m_InfoIndex--;
			if (pData->m_InfoIndex <= 0)
				pData->m_InfoIndex = (int)m_strReocrds.size();
			PrintAlarm(pData->m_InfoIndex);
			pData->m_SelectM = 0;
			pData->m_StartM = 0;
			pData->m_MenuCnt = (int)m_strTexts.size();
			InvalidateRect(NULL);
		}
		else if (wParam == GVK_RIGHT)
		{
			pData->m_InfoIndex++;
			if (pData->m_InfoIndex > (int)m_strReocrds.size())
				pData->m_InfoIndex = 1;
			PrintAlarm(pData->m_InfoIndex);
			pData->m_SelectM = 0;
			pData->m_StartM = 0;
			pData->m_MenuCnt = (int)m_strTexts.size();
			InvalidateRect(NULL);
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

int C_PrintAlarmWnd::ReadAlarmData(S_DWORD type)
{
	S_DWORD DI = 0x00;
	if(type==1)
	{
		DI=0XE200FFFF;
	}
	else
	{
		DI=0XE20100FF;
	}

	logit<<"开始读取告警..."<<endl;
#ifndef TEST_DEBUG
	// 读终端告警
	C_AlmEvt::Read((S_WORD)0, DI, m_strReocrds, /*tn-3*24*60*60*/10000, 0x7fffffff);
	// 读测量点告警
	S_ALLMP *pAllMP = STATUS.GetALLMP();
	for (size_t st=pAllMP->m_MTBS.m_AllMPBS.begin(); st<pAllMP->m_MTBS.m_AllMPBS.end(); st++)
	{
		if(!pAllMP->m_MTBS.m_AllMPBS.test(st))
			continue;
		if(C_AlmEvt::Read((S_WORD)st, DI, m_strReocrds, /*tn-3*24*60*60*/10000, 0x7fffffff)<0)
			continue;
	}
#else
	DI = 0XE2000001;
	for(int i = 0;i < 0xff;i++)
	{
		//TestMakeAlarmData(0,DI+i,m_strReocrds,false);//测量点0
		TestMakeAlarmData(0,DI+i,m_strReocrds,true);//测量点0
	}
#endif

	logit<<"共读出"<<(int)m_strReocrds.size()<<"告警"<<endl;
	return m_strReocrds.Count();
}

int C_PrintAlarmWnd::PrintAlarm(int Index)
{
	string strCurrent;
	m_strTexts.clear();
	if (Index <= 0 || Index > (int)m_strReocrds.size())
		strCurrent.append((size_t)12, (char)0);
	else if (m_strReocrds[Index-1].size() <= 6)
		strCurrent.append((size_t)12, (char)0);
	else
		strCurrent = m_strReocrds[Index-1];

	S_WORD MP = C_DAPN::DA2PN(KGE_MAKEWORD((S_BYTE *)strCurrent.data()));
	S_DWORD DI = KGE_MAKEDWORD((S_BYTE *)strCurrent.data() + 2);
	string strData(strCurrent.begin() + 6, strCurrent.end());
	C_AlmShow *pObj = C_AlmShowFact::GetAlmShowObj(DI);
	logit<<"显示告警:MP["<<MP<<"],DI["<<HEX<<DI<<"],Data["<<strData<<"]"<<endl;
	return pObj->Printf(MP, DI, strData, m_strTexts);
}
#ifdef TEST_DEBUG
//测试
int C_PrintAlarmWnd::TestMakeAlarmData(S_WORD MP, S_DWORD DI, strings &strReocrds,bool TypeFlag)
{
	string strTemp;
	strTemp.erase();
	strTemp.append((char *)&MP, 2).append((char *)&DI,4);
	S_BYTE T_DI = (S_BYTE)DI;
	if(((T_DI > 0)&&(T_DI <= 0x1f))||((T_DI >= 0x26)&&(T_DI <= 0x2a))||(T_DI == 0x2e)||((T_DI >= 0x33)&&(T_DI <= 0x35)))
		C_TestMakeData::FillARD2Data(strTemp,TypeFlag);
	else if(((T_DI >= 0x20)&&(T_DI <= 0x25))||((T_DI >= 0x44)&&(T_DI <= 0x4b)))
		C_TestMakeData::FillARD14Data(strTemp,TypeFlag);
	else if((T_DI == 0x2f)||(T_DI == 0x32)||(T_DI == 0x36)||(T_DI == 0x3f))
		C_TestMakeData::FillARD1Data(strTemp,TypeFlag);
	else if((T_DI == 0x2c)||(T_DI == 0x2d)||(T_DI == 0x30))
		C_TestMakeData::FillARD3Data(strTemp,TypeFlag);
	else if(T_DI == 0x2b)
		C_TestMakeData::FillARD4Data(strTemp,TypeFlag);
	else if(T_DI == 0x38)
		C_TestMakeData::FillARD5Data(strTemp,TypeFlag);
	else if(T_DI == 0x37)
		C_TestMakeData::FillARD6Data(strTemp,TypeFlag);
	else if(T_DI == 0x31)
		C_TestMakeData::FillARD7Data(strTemp,TypeFlag);
	else if(T_DI == 0x39)
		C_TestMakeData::FillARD9Data(strTemp,TypeFlag);
	else if((T_DI == 0x3a)||(T_DI == 0x43))
		C_TestMakeData::FillARD1018Data(strTemp,TypeFlag);
	else if((T_DI == 0x4d)||(T_DI == 0x4e))
		C_TestMakeData::FillARD11Data(strTemp,TypeFlag);
	else if((T_DI == 0x3b)||(T_DI == 0x3c))
		C_TestMakeData::FillARD12Data(strTemp,TypeFlag);
	else if(T_DI == 0x3e)
		C_TestMakeData::FillARD13Data(strTemp,TypeFlag);
	else if((T_DI == 0x40)||(T_DI == 0x41)||(T_DI == 0x42))
		C_TestMakeData::FillARD151617Data(strTemp,TypeFlag);
	else if(T_DI == 0x3d)
		C_TestMakeData::FillARD21Data(strTemp,TypeFlag);
	/*
	else if(T_DI == 0x4c)
		C_TestMakeData::FillARD22Data(strTemp);
	*/
	else
		return -1;
	strReocrds.push_back(strTemp);
	return 0;
}
//填时间type:0为YYYYMMDDHHMMSS 1为YYMMDDHHMMSS 2为YYMMDDHHMM
void C_TestMakeData::FillTimeData(string &strData,int type)
{
	char buffer[7];
	S_TIME timeCurrent = C_TIME::TimeNow();
	C_TIME::Time2BCDStr(timeCurrent, buffer);
	switch(type)
	{
	case 0:
		//YYYYMMDDHHMMSS
		strData.append(buffer,7);
		break;
	case 1:
		//YYMMDDHHMMSS
		strData.append(buffer,6);
		break;
	case 2:
		//YYMMDDHHMM
		strData.append(&buffer[1],5);
		break;
	case 3:
		//YYMMDD
		strData.append(&buffer[3],3);
		break;
	case 4:
		//HHMMSS
		strData.append(&buffer[0],3);
		break;
	default:
		strData.append(buffer,7);
		break;
	}
}
//填ARD1
int C_TestMakeData::FillARD1Data(string &strData,bool TypeFlag)
{
	strData.push_back(0x01);//告警发生为01恢复为00初始为ff 
	FillTimeData(strData,1);
	return 0;
}
//填ARD2
int C_TestMakeData::FillARD2Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	if(TypeFlag)//自造数据
	{
		MakeData(strData,0x12345678,4);//正向有功总电能	
		MakeData(strData,0x12345678,4);//反向有功总电能
		MakeData(strData,0x12345678,4);//组合无功1总电能	
		MakeData(strData,0x12345678,4);//组合无功2总电能
		Make3Vol(strData);// 3相电压
		Make3Cur(strData);// 3相电流
		MakePower(strData,3);//有功功率
		MakePower(strData,3);//无功功率
		MakePower(strData,2);//功率因素
	}
	else//填无效数据 0xff
	{
		strData.append((size_t)(16+18+18+12+12+8),(char)FILL_INVALID_DATA);
	}
	return 0;
}
//填ARD3
int C_TestMakeData::FillARD3Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	if(TypeFlag)//自造数据
	{
		//发生前
		MakeData(strData,0x12345678,4);//正向有功总电能	
		MakeData(strData,0x12345678,4);//反向有功总电能
		MakeData(strData,0x12345678,4);//组合无功1总电能	
		MakeData(strData,0x12345678,4);//组合无功2总电能
		Make3Vol(strData);// 3相电压
		Make3Cur(strData);// 3相电流
		MakePower(strData,3);//有功功率
		MakePower(strData,3);//无功功率
		//发生后
		MakeData(strData,0x12345678,4);//正向有功总电能	
		MakeData(strData,0x12345678,4);//反向有功总电能
		MakeData(strData,0x12345678,4);//组合无功1总电能	
		MakeData(strData,0x12345678,4);//组合无功2总电能
		Make3Vol(strData);// 3相电压
		Make3Cur(strData);// 3相电流
		MakePower(strData,3);//有功功率
		MakePower(strData,3);//无功功率
	}
	else//填无效数据 0xff
	{
		strData.append((size_t)(16+18+18+12+12),(char)FILL_INVALID_DATA);
	}
	return 0;
}
//填ARD4
int C_TestMakeData::FillARD4Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	if(TypeFlag)//自造数据
	{
		MakeData(strData,0x12345678,4);//正向有功总电能	
		MakeData(strData,0x12345678,4);//反向有功总电能
		MakeData(strData,0x12345678,4);//组合无功1总电能	
		MakeData(strData,0x12345678,4);//组合无功2总电能
		MakeData(strData,0x12345678,4);//剩余金额
	}
	else//填无效数据 0xff
	{
		strData.append((size_t)(16+4),(char)FILL_INVALID_DATA);
	}
	return 0;
}
//填ARD5
int C_TestMakeData::FillARD5Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	if(TypeFlag)//自造数据
	{
		MakeData(strData,0x12345678,4);//正向有功总电能	
		MakeData(strData,0x12345678,4);//反向有功总电能
		MakeData(strData,0x12345678,4);//组合无功1总电能	
		MakeData(strData,0x12345678,4);//组合无功2总电能
		//发生前
		MakeData(strData,0x12345678,2);//PT变比
		MakeData(strData,0x12345678,2);//CT变比
		//发生后
		MakeData(strData,0x12345678,2);//PT变比
		MakeData(strData,0x12345678,2);//CT变比
	}
	else//填无效数据 0xff
	{
		strData.append((size_t)(16+4+4),(char)FILL_INVALID_DATA);
	}
	return 0;
}
//填ARD6
int C_TestMakeData::FillARD6Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	if(TypeFlag)//自造数据
	{
		MakeData(strData,0x12345678,4);//正向有功总电能	
		MakeData(strData,0x12345678,4);//反向有功总电能
		MakeData(strData,0x12345678,4);//组合无功1总电能	
		MakeData(strData,0x12345678,4);//组合无功2总电能
		//发生前
		MakeData(strData,0x12345678,3);//电表常数(有功)
		MakeData(strData,0x12345678,3);//电表常数(无功)
		//发生后
		MakeData(strData,0x12345678,3);//电表常数(有功)
		MakeData(strData,0x12345678,3);//电表常数(无功)
	}
	else//填无效数据 0xff
	{
		strData.append((size_t)(16+6+6),(char)FILL_INVALID_DATA);
	}
	return 0;
}
//填ARD7
int C_TestMakeData::FillARD7Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	if(TypeFlag)//自造数据
	{
		MakeData(strData,0x12345678,4);//正向有功总电能	
		MakeData(strData,0x12345678,4);//反向有功总电能
		MakeData(strData,0x12345678,4);//组合无功1总电能	
		MakeData(strData,0x12345678,4);//组合无功2总电能
		MakeData(strData,0x12345678,3);//正向有功总最大需量
		FillTimeData(strData,2);//发生时间
		MakeData(strData,0x12345678,3);//反向有功总最大需量
		FillTimeData(strData,2);//发生时间
		MakeData(strData,0x12345678,3);//组合无功1总最大需量
		FillTimeData(strData,2);//发生时间
		MakeData(strData,0x12345678,3);//组合无功2总最大需量
		FillTimeData(strData,2);//发生时间
	}
	else//填无效数据 0xff
	{
		strData.append((size_t)(16+8*4),(char)FILL_INVALID_DATA);
	}
	return 0;
}
//填ARD8
int C_TestMakeData::FillARD8Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	if(TypeFlag)//自造数据
	{
		//发生前
		MakeData(strData,0x12345678,2);//电表状态字1
		MakeData(strData,0x12345678,2);//电表状态字2
		MakeData(strData,0x12345678,2);//电表状态字3	
		MakeData(strData,0x12345678,2);//电表状态字4
		MakeData(strData,0x12345678,2);//电表状态字5
		MakeData(strData,0x12345678,2);//电表状态字6	
		MakeData(strData,0x12345678,2);//电表状态字7
		//发生后
		MakeData(strData,0x12345678,2);//电表状态字1
		MakeData(strData,0x12345678,2);//电表状态字2
		MakeData(strData,0x12345678,2);//电表状态字3	
		MakeData(strData,0x12345678,2);//电表状态字4
		MakeData(strData,0x12345678,2);//电表状态字5
		MakeData(strData,0x12345678,2);//电表状态字6	
		MakeData(strData,0x12345678,2);//电表状态字7
	}
	else//填无效数据 0xff
	{
		strData.append((size_t)(14*2),(char)FILL_INVALID_DATA);
	}
	return 0;
}
//填ARD9
int C_TestMakeData::FillARD9Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	if(TypeFlag)//自造数据
	{
		//发生前
		MakeData(strData,0x00000000,4);
		MakeData(strData,0x00000000,4);
		MakeData(strData,0x00000000,4);
		MakeData(strData,0x00000000,3);
		//发生后
		MakeData(strData,0x01010101,4);
		MakeData(strData,0x01010101,4);
		MakeData(strData,0x01010101,4);
		MakeData(strData,0x01010101,3);
	}
	else//填无效数据 0xff
	{
		strData.append((size_t)(15*2),(char)FILL_INVALID_DATA);
	}
	return 0;
}
//填ARD10
int C_TestMakeData::FillARD1018Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	if(TypeFlag)//自造数据
	{
		MakeData(strData,0x12345678,3);
	}
	else//填无效数据 0xff
	{
		strData.append((size_t)3,(char)FILL_INVALID_DATA);
	}
	return 0;
}
//填ARD11
int C_TestMakeData::FillARD11Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	if(TypeFlag)//自造数据
	{
		MakeData(strData,0x12345678,3);//发生次数
		//发生前
		MakeData(strData,0x12345678,4);//正向有功总电能	
		MakeData(strData,0x12345678,4);//反向有功总电能
		MakeData(strData,0x12345678,4);//第1象限无功总电能	
		MakeData(strData,0x12345678,4);//第2象限无功总电能
		MakeData(strData,0x12345678,4);//第3象限无功总电能	
		MakeData(strData,0x12345678,4);//第4象限无功总电能
		//发生后
		MakeData(strData,0x12345678,4);//正向有功总电能	
		MakeData(strData,0x12345678,4);//反向有功总电能
		MakeData(strData,0x12345678,4);//第1象限无功总电能	
		MakeData(strData,0x12345678,4);//第2象限无功总电能
		MakeData(strData,0x12345678,4);//第3象限无功总电能	
		MakeData(strData,0x12345678,4);//第4象限无功总电能
	}
	else//填无效数据 0xff
	{
		strData.append((size_t)(3+24*2),(char)FILL_INVALID_DATA);
	}
	return 0;
}
//填ARD12
int C_TestMakeData::FillARD12Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	if(TypeFlag)//自造数据
	{
		MakeData(strData,0x12345678,2);//电表状态字3	
	}
	else//填无效数据 0xff
	{
		strData.append((size_t)2,(char)FILL_INVALID_DATA);
	}
	return 0;
}
//填ARD13
int C_TestMakeData::FillARD13Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	if(TypeFlag)//自造数据
	{
		FillTimeData(strData,3);//YYMMDD
		strData.push_back(0);//WW
		FillTimeData(strData,4);//HHMMSS
		FillTimeData(strData,1);//YYMMDDHHMMSS
	}
	else//填无效数据 0xff
	{
		strData.append((size_t)13,(char)FILL_INVALID_DATA);
	}
	return 0;
}
//填ARD14
int C_TestMakeData::FillARD14Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	if(TypeFlag)//自造数据
	{
		MakeData(strData,0x12345678,2);//A相电压总谐波含有量	
		MakeData(strData,0x12345678,2);//B相电压总谐波含有量	
		MakeData(strData,0x12345678,2);//C相电压总谐波含有量	
		MakeData(strData,0x12345678,2);//A相电流总谐波含有量	
		MakeData(strData,0x12345678,2);//B相电流总谐波含有量	
		MakeData(strData,0x12345678,2);//C相电流总谐波含有量	
		MakeData(strData,0x12345678,2);//A相电压偏差最大值	
		MakeData(strData,0x12345678,2);//B相电压偏差最大值	
		MakeData(strData,0x12345678,2);//C相电压偏差最大值	
		MakeData(strData,0x12345678,1);//频率偏差最大值
		MakeData(strData,0x12345678,2);//电压不平衡度最大值	
		MakeData(strData,0x12345678,1);//长时闪变最大值
		MakeData(strData,0x12345678,1);//短时闪变最大值
		MakeData(strData,0x12345678,2);//电压总畸变最大值
	}
	else//填无效数据 0xff
	{
		strData.append((size_t)25,(char)FILL_INVALID_DATA);
	}
	return 0;
}
//填ARD15
int C_TestMakeData::FillARD151617Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	if(TypeFlag)//自造数据
	{
		MakeData(strData,0x12345678,3);//A相总次数
		MakeData(strData,0x12345678,3);//总累计时间	
		MakeData(strData,0x12345678,3);//B相总次数	
		MakeData(strData,0x12345678,3);//总累计时间	
		MakeData(strData,0x12345678,3);//C相总次数	
		MakeData(strData,0x12345678,3);//总累计时间	
	}
	else//填无效数据 0xff
	{
		strData.append((size_t)(3*6),(char)FILL_INVALID_DATA);
	}
	return 0;
}
//填ARD19
int C_TestMakeData::FillARD19Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	if(TypeFlag)//自造数据
	{
		MakeData(strData,0x12345678,4);//正向有功总电能	
		MakeData(strData,0x12345678,4);//反向有功总电能
		MakeData(strData,0x12345678,4);//组合无功1总电能	
		MakeData(strData,0x12345678,4);//组合无功2总电能
	}
	else//填无效数据 0xff
	{
		strData.append((size_t)16,(char)FILL_INVALID_DATA);
	}
	return 0;
}
//填ARD20
int C_TestMakeData::FillARD20Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	if(TypeFlag)//自造数据
	{
		MakeData(strData,0x00000001,1);//
		MakeData(strData,0x12345678,4);//
	}
	else//填无效数据 0xff
	{
		strData.append((size_t)5,(char)FILL_INVALID_DATA);
	}
	return 0;
}
//填ARD21
int C_TestMakeData::FillARD21Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	//全填无效数据 0xff
		strData.append((size_t)256,(char)FILL_INVALID_DATA);
	return 0;
}
//填ARD22
int C_TestMakeData::FillARD22Data(string &strData,bool TypeFlag)
{	
	//此类型不支持
	FillARD1Data(strData);
	//填无效数据 0xff
	return 0;
}
#endif

//液晶调节	可调节终端液晶显示对比度、背光亮度等
int C_ScreenParamViewWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	switch (msg)
	{
	case GM_CREATE:{
		C_BUTTON *pbtnSet = C_BUTTON::NewCtrl();
		S_RECT rect = {60, LINEXY(5), 32, 16};
		pbtnSet->Create("设置", 0x00, rect, this, 1);
		SetFocusCtrl(pbtnSet);
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		S_LcdInfo *pInfo = STATUS.GetLcdInfo();
		sprintf(buffer, "液晶显示对比度:%d", pInfo->m_LCDGreyScale);
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
				GOTOWND(C_ScreenParamSetWnd, 0x00);
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

//信号强度和电池电量	可查询正比于无线信号场强的指示和当前电池电量（终端电池指备用停电上报电池）
int C_SignalAndBatteryWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	switch (msg)
	{
	case GM_CREATE:
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		S_PppStatus *pPppStatus = CONTEXT.GetPppStatus();
		S_PowerStatus *pPowerStatus = CONTEXT.GetPowerStatus();
		sprintf(buffer, "无线信号强度:%ddB", -113 + pPppStatus->m_Signal * 2);
		pdc->DrawString(STRINGX, 0*18+8, buffer);
		sprintf(buffer, "当前电池电量:%d%%", pPowerStatus->m_BatteryCap);
		pdc->DrawString(STRINGX, 1*18+8, buffer);
		pdc->DrawHLine(0, 122, 160);
		EndPaint(pdc);
		}
		break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
			RETURNWND();
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

//无线实时状态	可提供终端连接无线网络的实时状态信息
int C_WirelessWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	switch (msg)
	{
	case GM_CREATE:
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		S_PppStatus *pPppStatus = CONTEXT.GetPppStatus();
		sprintf(buffer, "MODEM类型:%s", ModemType(pPppStatus->m_ModemType));
		pdc->DrawString(STRINGX, (0*17+2), buffer);
		sprintf(buffer, "  %s", pPppStatus->m_ModemFactory);
		pdc->DrawString(STRINGX, (1*17+2), "MODEM厂家:");
		pdc->DrawString(STRINGX, (2*17+2), buffer);
		sprintf(buffer, "  %s", pPppStatus->m_ModemModel);
		pdc->DrawString(STRINGX, (3*17+2), "MODEM型号:");
		pdc->DrawString(STRINGX, (4*17+2), buffer);
		S_BYTE *pIP = (S_BYTE *)&pPppStatus->m_SimIP;
		sprintf(buffer, "  %d.%d.%d.%d", *(pIP+0), *(pIP+1), *(pIP+2), *(pIP+3));
		pdc->DrawString(STRINGX, (5*17+2), "Sim卡IP:");
		pdc->DrawString(STRINGX, (6*17+2), buffer);

		pdc->DrawHLine(0, 122, 160);
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F12x12));
		pdc->DrawString(20, 126, GetPppBottomInfo());
		EndPaint(pdc);
		}
		break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
			RETURNWND();
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

const char *C_WirelessWnd::ModemType(int type)
{
	if (type <= MODEM_TYPE_COMMON)
		return "";
	if (type <= MODEM_TYPE_GPRS_MAX)
		return "GPRS";
	if (type <= MODEM_TYPE_CDMA_MAX)
		return "CDMA";
	return "";
}

const char *C_WirelessWnd::GetPppBottomInfo(void)
{
	return CONTEXT.GetLcdBottomInfo(MP_PROCESS_PPP);
}

//液晶调节	可调节终端液晶显示对比度、背光亮度等
const char *ScreenParamSetInfo[] = {"                ", "显示参数设置成功"};
int C_ScreenParamSetWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	S_MenuCtrl *pData = static_cast<S_MenuCtrl *>(GetWndData(sizeof(S_MenuCtrl)));
	switch (msg)
	{
	case GM_CREATE:{
		pData->m_InfoIndex = 0;
		S_LcdInfo *pInfo = STATUS.GetLcdInfo();
		S_RECT NumRect = {96, 1*20+6, 18, 16};
		C_NUMEDIT *pContrastEdit = C_NUMEDIT::NewCtrl();
		sprintf(buffer, "%02d", pInfo->m_LCDGreyScale);
		pContrastEdit->Create(buffer, NUMEDIT_STYLE_DEC, NumRect, this, 1);
		C_BUTTON *pbtnSet = C_BUTTON::NewCtrl();
		S_RECT SetRect = {60, 90, 32, 16};
		pbtnSet->Create("确定", 0x00, SetRect, this, 2);
		SetFocusCtrl(pContrastEdit);
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		pdc->DrawString(STRINGX, 0*20+8, "液晶调节");
		pdc->DrawString(STRINGX, 1*20+8, "显示对比度:");
		pdc->DrawHLine(0, 122, 160);
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F12x12));
		pdc->DrawString(20, 126, ScreenParamSetInfo[pData->m_InfoIndex]);
		EndPaint(pdc);
		}
		break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
		{
			S_LcdInfo *pInfo = STATUS.GetLcdInfo();
			SGUI_contrast(pInfo->m_LCDGreyScale);
			RETURNWND();
		}
		else if (wParam == GVK_UP)
			SetFocusCtrl(GetWndCtrl(1));
		else if (wParam == GVK_OK)
			SetFocusCtrl(GetWndCtrl(2));
		break;
	case GM_CMD:
		if (wParam == 1 && lParam == GM_CMD_CTRL_UPDATE)
		{
			string strContrast = GetWndCtrl(1)->GetWndText();
			int contrast = atoi(strContrast.c_str());
			SGUI_contrast(contrast);
		}
		else if (wParam == 2 && lParam == GM_CMD_BTN_PUSHED)
		{
			string strContrast = GetWndCtrl(1)->GetWndText();
			int contrast = atoi(strContrast.c_str());
			sprintf(buffer, " 确定对比度改为:%d", contrast);
			if (SGui_MsgBox("提示", buffer, MB_BTN_OKCANCEL) == MB_RTN_YESOK)
			{
				S_LcdInfo *pInfo = STATUS.GetLcdInfo();
				pInfo->m_LCDGreyScale = contrast;
				STATUS.SaveLcdInfo();
				pData->m_InfoIndex = 1;
			}
			else
			{
				pData->m_InfoIndex = 0;
				SetFocusCtrl(GetWndCtrl(1));
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


