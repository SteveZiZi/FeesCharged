/** @file
 *  @brief �ն˹�����ά�����ڶ���
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

const char *g_TermName[] ={"��վ�ն�","ר���ն�","�����ն�","�������ն�"};
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
		pdc->DrawString(STRINGX, 0*18+8, "�ն˰汾��Ϣ");
		sprintf(buffer, "���̱���:%s", "NDKJ");
		pdc->DrawString(STRINGX, 1*18+8, buffer);
		sprintf(buffer, "�ն�����:%s", g_TermName[CONS_TERM_TYPE-1]);
		pdc->DrawString(STRINGX, 2*18+8, buffer);
		sprintf(buffer, "Ӳ���汾��:%s", CONS_HARD_VERSION);
		pdc->DrawString(STRINGX, 3*18+8, buffer);
		sprintf(buffer, "����汾��:%s", CONS_MAIN_VERSION);
		pdc->DrawString(STRINGX, 4*18+8, buffer);
		sprintf(buffer, "��������:%s", CONS_SW_RELEASE_DATE);
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

const char *g_RebootMenu[] = {"��������λ","��������λ","�ն�Ӳ����λ"};
const char *g_RebootWndInfo[] = {"               ", "ϵͳ׼������...", "ϵͳ��λ���..."};
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
			if (SGui_MsgBox("����", buffer, MB_BTN_OKCANCEL) != MB_RTN_YESOK)
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

//�澯��ѯ	���ṩ��������100���澯��Ϣ��ѯ�������澯���롢������ʾ�¼����ݡ�����ʱ��ȣ�
const char *g_QueryAlarmMenu[] = {"��ѯ��Ҫ�澯","��ѯ��ͨ�澯","��ѯ�¼���¼"};
const char *g_QueryAlarmWndInfo[] = {"�¸澯��¼:", "�¸澯��¼:", "���¼���¼:"};
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
		sprintf(buffer, "%s %d��", g_QueryAlarmWndInfo[pData->m_SelectM], RecordCnt);
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

//��ѯ����ͳ����Ϣ:
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
		if(MeterTotal == 0) {MeterDaySuccess = 0, MeterMonSuccess = 0, MeterTotal = 1;} //��ֹ����
		if (MeterDaySuccess > MeterTotal) MeterDaySuccess = MeterTotal;
		if (MeterMonSuccess > MeterTotal) MeterMonSuccess = MeterTotal;

		sprintf(buffer, "���е��: %d ֻ", MeterTotal);
		pdc->DrawString(STRINGX, LINEXY(0), buffer);
		sprintf(buffer, "�ճ��ճɹ���:%0.1f%%",((FP32)MeterDaySuccess/(FP32)MeterTotal)*100.0);
		pdc->DrawString(STRINGX, LINEXY(1), buffer);
		sprintf(buffer, "�³��ճɹ���:%0.1f%%",((FP32)MeterMonSuccess/(FP32)MeterTotal)*100.0);
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

//�澯������ʾ
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
		sprintf(buffer, "��%d��, ��%d��", pData->m_InfoIndex, m_strReocrds.size());
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

	logit<<"��ʼ��ȡ�澯..."<<endl;
#ifndef TEST_DEBUG
	// ���ն˸澯
	C_AlmEvt::Read((S_WORD)0, DI, m_strReocrds, /*tn-3*24*60*60*/10000, 0x7fffffff);
	// ��������澯
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
		//TestMakeAlarmData(0,DI+i,m_strReocrds,false);//������0
		TestMakeAlarmData(0,DI+i,m_strReocrds,true);//������0
	}
#endif

	logit<<"������"<<(int)m_strReocrds.size()<<"�澯"<<endl;
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
	logit<<"��ʾ�澯:MP["<<MP<<"],DI["<<HEX<<DI<<"],Data["<<strData<<"]"<<endl;
	return pObj->Printf(MP, DI, strData, m_strTexts);
}
#ifdef TEST_DEBUG
//����
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
//��ʱ��type:0ΪYYYYMMDDHHMMSS 1ΪYYMMDDHHMMSS 2ΪYYMMDDHHMM
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
//��ARD1
int C_TestMakeData::FillARD1Data(string &strData,bool TypeFlag)
{
	strData.push_back(0x01);//�澯����Ϊ01�ָ�Ϊ00��ʼΪff 
	FillTimeData(strData,1);
	return 0;
}
//��ARD2
int C_TestMakeData::FillARD2Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	if(TypeFlag)//��������
	{
		MakeData(strData,0x12345678,4);//�����й��ܵ���	
		MakeData(strData,0x12345678,4);//�����й��ܵ���
		MakeData(strData,0x12345678,4);//����޹�1�ܵ���	
		MakeData(strData,0x12345678,4);//����޹�2�ܵ���
		Make3Vol(strData);// 3���ѹ
		Make3Cur(strData);// 3�����
		MakePower(strData,3);//�й�����
		MakePower(strData,3);//�޹�����
		MakePower(strData,2);//��������
	}
	else//����Ч���� 0xff
	{
		strData.append((size_t)(16+18+18+12+12+8),(char)FILL_INVALID_DATA);
	}
	return 0;
}
//��ARD3
int C_TestMakeData::FillARD3Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	if(TypeFlag)//��������
	{
		//����ǰ
		MakeData(strData,0x12345678,4);//�����й��ܵ���	
		MakeData(strData,0x12345678,4);//�����й��ܵ���
		MakeData(strData,0x12345678,4);//����޹�1�ܵ���	
		MakeData(strData,0x12345678,4);//����޹�2�ܵ���
		Make3Vol(strData);// 3���ѹ
		Make3Cur(strData);// 3�����
		MakePower(strData,3);//�й�����
		MakePower(strData,3);//�޹�����
		//������
		MakeData(strData,0x12345678,4);//�����й��ܵ���	
		MakeData(strData,0x12345678,4);//�����й��ܵ���
		MakeData(strData,0x12345678,4);//����޹�1�ܵ���	
		MakeData(strData,0x12345678,4);//����޹�2�ܵ���
		Make3Vol(strData);// 3���ѹ
		Make3Cur(strData);// 3�����
		MakePower(strData,3);//�й�����
		MakePower(strData,3);//�޹�����
	}
	else//����Ч���� 0xff
	{
		strData.append((size_t)(16+18+18+12+12),(char)FILL_INVALID_DATA);
	}
	return 0;
}
//��ARD4
int C_TestMakeData::FillARD4Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	if(TypeFlag)//��������
	{
		MakeData(strData,0x12345678,4);//�����й��ܵ���	
		MakeData(strData,0x12345678,4);//�����й��ܵ���
		MakeData(strData,0x12345678,4);//����޹�1�ܵ���	
		MakeData(strData,0x12345678,4);//����޹�2�ܵ���
		MakeData(strData,0x12345678,4);//ʣ����
	}
	else//����Ч���� 0xff
	{
		strData.append((size_t)(16+4),(char)FILL_INVALID_DATA);
	}
	return 0;
}
//��ARD5
int C_TestMakeData::FillARD5Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	if(TypeFlag)//��������
	{
		MakeData(strData,0x12345678,4);//�����й��ܵ���	
		MakeData(strData,0x12345678,4);//�����й��ܵ���
		MakeData(strData,0x12345678,4);//����޹�1�ܵ���	
		MakeData(strData,0x12345678,4);//����޹�2�ܵ���
		//����ǰ
		MakeData(strData,0x12345678,2);//PT���
		MakeData(strData,0x12345678,2);//CT���
		//������
		MakeData(strData,0x12345678,2);//PT���
		MakeData(strData,0x12345678,2);//CT���
	}
	else//����Ч���� 0xff
	{
		strData.append((size_t)(16+4+4),(char)FILL_INVALID_DATA);
	}
	return 0;
}
//��ARD6
int C_TestMakeData::FillARD6Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	if(TypeFlag)//��������
	{
		MakeData(strData,0x12345678,4);//�����й��ܵ���	
		MakeData(strData,0x12345678,4);//�����й��ܵ���
		MakeData(strData,0x12345678,4);//����޹�1�ܵ���	
		MakeData(strData,0x12345678,4);//����޹�2�ܵ���
		//����ǰ
		MakeData(strData,0x12345678,3);//�����(�й�)
		MakeData(strData,0x12345678,3);//�����(�޹�)
		//������
		MakeData(strData,0x12345678,3);//�����(�й�)
		MakeData(strData,0x12345678,3);//�����(�޹�)
	}
	else//����Ч���� 0xff
	{
		strData.append((size_t)(16+6+6),(char)FILL_INVALID_DATA);
	}
	return 0;
}
//��ARD7
int C_TestMakeData::FillARD7Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	if(TypeFlag)//��������
	{
		MakeData(strData,0x12345678,4);//�����й��ܵ���	
		MakeData(strData,0x12345678,4);//�����й��ܵ���
		MakeData(strData,0x12345678,4);//����޹�1�ܵ���	
		MakeData(strData,0x12345678,4);//����޹�2�ܵ���
		MakeData(strData,0x12345678,3);//�����й����������
		FillTimeData(strData,2);//����ʱ��
		MakeData(strData,0x12345678,3);//�����й����������
		FillTimeData(strData,2);//����ʱ��
		MakeData(strData,0x12345678,3);//����޹�1���������
		FillTimeData(strData,2);//����ʱ��
		MakeData(strData,0x12345678,3);//����޹�2���������
		FillTimeData(strData,2);//����ʱ��
	}
	else//����Ч���� 0xff
	{
		strData.append((size_t)(16+8*4),(char)FILL_INVALID_DATA);
	}
	return 0;
}
//��ARD8
int C_TestMakeData::FillARD8Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	if(TypeFlag)//��������
	{
		//����ǰ
		MakeData(strData,0x12345678,2);//���״̬��1
		MakeData(strData,0x12345678,2);//���״̬��2
		MakeData(strData,0x12345678,2);//���״̬��3	
		MakeData(strData,0x12345678,2);//���״̬��4
		MakeData(strData,0x12345678,2);//���״̬��5
		MakeData(strData,0x12345678,2);//���״̬��6	
		MakeData(strData,0x12345678,2);//���״̬��7
		//������
		MakeData(strData,0x12345678,2);//���״̬��1
		MakeData(strData,0x12345678,2);//���״̬��2
		MakeData(strData,0x12345678,2);//���״̬��3	
		MakeData(strData,0x12345678,2);//���״̬��4
		MakeData(strData,0x12345678,2);//���״̬��5
		MakeData(strData,0x12345678,2);//���״̬��6	
		MakeData(strData,0x12345678,2);//���״̬��7
	}
	else//����Ч���� 0xff
	{
		strData.append((size_t)(14*2),(char)FILL_INVALID_DATA);
	}
	return 0;
}
//��ARD9
int C_TestMakeData::FillARD9Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	if(TypeFlag)//��������
	{
		//����ǰ
		MakeData(strData,0x00000000,4);
		MakeData(strData,0x00000000,4);
		MakeData(strData,0x00000000,4);
		MakeData(strData,0x00000000,3);
		//������
		MakeData(strData,0x01010101,4);
		MakeData(strData,0x01010101,4);
		MakeData(strData,0x01010101,4);
		MakeData(strData,0x01010101,3);
	}
	else//����Ч���� 0xff
	{
		strData.append((size_t)(15*2),(char)FILL_INVALID_DATA);
	}
	return 0;
}
//��ARD10
int C_TestMakeData::FillARD1018Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	if(TypeFlag)//��������
	{
		MakeData(strData,0x12345678,3);
	}
	else//����Ч���� 0xff
	{
		strData.append((size_t)3,(char)FILL_INVALID_DATA);
	}
	return 0;
}
//��ARD11
int C_TestMakeData::FillARD11Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	if(TypeFlag)//��������
	{
		MakeData(strData,0x12345678,3);//��������
		//����ǰ
		MakeData(strData,0x12345678,4);//�����й��ܵ���	
		MakeData(strData,0x12345678,4);//�����й��ܵ���
		MakeData(strData,0x12345678,4);//��1�����޹��ܵ���	
		MakeData(strData,0x12345678,4);//��2�����޹��ܵ���
		MakeData(strData,0x12345678,4);//��3�����޹��ܵ���	
		MakeData(strData,0x12345678,4);//��4�����޹��ܵ���
		//������
		MakeData(strData,0x12345678,4);//�����й��ܵ���	
		MakeData(strData,0x12345678,4);//�����й��ܵ���
		MakeData(strData,0x12345678,4);//��1�����޹��ܵ���	
		MakeData(strData,0x12345678,4);//��2�����޹��ܵ���
		MakeData(strData,0x12345678,4);//��3�����޹��ܵ���	
		MakeData(strData,0x12345678,4);//��4�����޹��ܵ���
	}
	else//����Ч���� 0xff
	{
		strData.append((size_t)(3+24*2),(char)FILL_INVALID_DATA);
	}
	return 0;
}
//��ARD12
int C_TestMakeData::FillARD12Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	if(TypeFlag)//��������
	{
		MakeData(strData,0x12345678,2);//���״̬��3	
	}
	else//����Ч���� 0xff
	{
		strData.append((size_t)2,(char)FILL_INVALID_DATA);
	}
	return 0;
}
//��ARD13
int C_TestMakeData::FillARD13Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	if(TypeFlag)//��������
	{
		FillTimeData(strData,3);//YYMMDD
		strData.push_back(0);//WW
		FillTimeData(strData,4);//HHMMSS
		FillTimeData(strData,1);//YYMMDDHHMMSS
	}
	else//����Ч���� 0xff
	{
		strData.append((size_t)13,(char)FILL_INVALID_DATA);
	}
	return 0;
}
//��ARD14
int C_TestMakeData::FillARD14Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	if(TypeFlag)//��������
	{
		MakeData(strData,0x12345678,2);//A���ѹ��г��������	
		MakeData(strData,0x12345678,2);//B���ѹ��г��������	
		MakeData(strData,0x12345678,2);//C���ѹ��г��������	
		MakeData(strData,0x12345678,2);//A�������г��������	
		MakeData(strData,0x12345678,2);//B�������г��������	
		MakeData(strData,0x12345678,2);//C�������г��������	
		MakeData(strData,0x12345678,2);//A���ѹƫ�����ֵ	
		MakeData(strData,0x12345678,2);//B���ѹƫ�����ֵ	
		MakeData(strData,0x12345678,2);//C���ѹƫ�����ֵ	
		MakeData(strData,0x12345678,1);//Ƶ��ƫ�����ֵ
		MakeData(strData,0x12345678,2);//��ѹ��ƽ������ֵ	
		MakeData(strData,0x12345678,1);//��ʱ�������ֵ
		MakeData(strData,0x12345678,1);//��ʱ�������ֵ
		MakeData(strData,0x12345678,2);//��ѹ�ܻ������ֵ
	}
	else//����Ч���� 0xff
	{
		strData.append((size_t)25,(char)FILL_INVALID_DATA);
	}
	return 0;
}
//��ARD15
int C_TestMakeData::FillARD151617Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	if(TypeFlag)//��������
	{
		MakeData(strData,0x12345678,3);//A���ܴ���
		MakeData(strData,0x12345678,3);//���ۼ�ʱ��	
		MakeData(strData,0x12345678,3);//B���ܴ���	
		MakeData(strData,0x12345678,3);//���ۼ�ʱ��	
		MakeData(strData,0x12345678,3);//C���ܴ���	
		MakeData(strData,0x12345678,3);//���ۼ�ʱ��	
	}
	else//����Ч���� 0xff
	{
		strData.append((size_t)(3*6),(char)FILL_INVALID_DATA);
	}
	return 0;
}
//��ARD19
int C_TestMakeData::FillARD19Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	if(TypeFlag)//��������
	{
		MakeData(strData,0x12345678,4);//�����й��ܵ���	
		MakeData(strData,0x12345678,4);//�����й��ܵ���
		MakeData(strData,0x12345678,4);//����޹�1�ܵ���	
		MakeData(strData,0x12345678,4);//����޹�2�ܵ���
	}
	else//����Ч���� 0xff
	{
		strData.append((size_t)16,(char)FILL_INVALID_DATA);
	}
	return 0;
}
//��ARD20
int C_TestMakeData::FillARD20Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	if(TypeFlag)//��������
	{
		MakeData(strData,0x00000001,1);//
		MakeData(strData,0x12345678,4);//
	}
	else//����Ч���� 0xff
	{
		strData.append((size_t)5,(char)FILL_INVALID_DATA);
	}
	return 0;
}
//��ARD21
int C_TestMakeData::FillARD21Data(string &strData,bool TypeFlag)
{
	FillARD1Data(strData);
	//ȫ����Ч���� 0xff
		strData.append((size_t)256,(char)FILL_INVALID_DATA);
	return 0;
}
//��ARD22
int C_TestMakeData::FillARD22Data(string &strData,bool TypeFlag)
{	
	//�����Ͳ�֧��
	FillARD1Data(strData);
	//����Ч���� 0xff
	return 0;
}
#endif

//Һ������	�ɵ����ն�Һ����ʾ�Աȶȡ��������ȵ�
int C_ScreenParamViewWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	switch (msg)
	{
	case GM_CREATE:{
		C_BUTTON *pbtnSet = C_BUTTON::NewCtrl();
		S_RECT rect = {60, LINEXY(5), 32, 16};
		pbtnSet->Create("����", 0x00, rect, this, 1);
		SetFocusCtrl(pbtnSet);
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		S_LcdInfo *pInfo = STATUS.GetLcdInfo();
		sprintf(buffer, "Һ����ʾ�Աȶ�:%d", pInfo->m_LCDGreyScale);
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

//�ź�ǿ�Ⱥ͵�ص���	�ɲ�ѯ�����������źų�ǿ��ָʾ�͵�ǰ��ص������ն˵��ָ����ͣ���ϱ���أ�
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
		sprintf(buffer, "�����ź�ǿ��:%ddB", -113 + pPppStatus->m_Signal * 2);
		pdc->DrawString(STRINGX, 0*18+8, buffer);
		sprintf(buffer, "��ǰ��ص���:%d%%", pPowerStatus->m_BatteryCap);
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

//����ʵʱ״̬	���ṩ�ն��������������ʵʱ״̬��Ϣ
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
		sprintf(buffer, "MODEM����:%s", ModemType(pPppStatus->m_ModemType));
		pdc->DrawString(STRINGX, (0*17+2), buffer);
		sprintf(buffer, "  %s", pPppStatus->m_ModemFactory);
		pdc->DrawString(STRINGX, (1*17+2), "MODEM����:");
		pdc->DrawString(STRINGX, (2*17+2), buffer);
		sprintf(buffer, "  %s", pPppStatus->m_ModemModel);
		pdc->DrawString(STRINGX, (3*17+2), "MODEM�ͺ�:");
		pdc->DrawString(STRINGX, (4*17+2), buffer);
		S_BYTE *pIP = (S_BYTE *)&pPppStatus->m_SimIP;
		sprintf(buffer, "  %d.%d.%d.%d", *(pIP+0), *(pIP+1), *(pIP+2), *(pIP+3));
		pdc->DrawString(STRINGX, (5*17+2), "Sim��IP:");
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

//Һ������	�ɵ����ն�Һ����ʾ�Աȶȡ��������ȵ�
const char *ScreenParamSetInfo[] = {"                ", "��ʾ�������óɹ�"};
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
		pbtnSet->Create("ȷ��", 0x00, SetRect, this, 2);
		SetFocusCtrl(pContrastEdit);
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		pdc->DrawString(STRINGX, 0*20+8, "Һ������");
		pdc->DrawString(STRINGX, 1*20+8, "��ʾ�Աȶ�:");
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
			sprintf(buffer, " ȷ���Աȶȸ�Ϊ:%d", contrast);
			if (SGui_MsgBox("��ʾ", buffer, MB_BTN_OKCANCEL) == MB_RTN_YESOK)
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


