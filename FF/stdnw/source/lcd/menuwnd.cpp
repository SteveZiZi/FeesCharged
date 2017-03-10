/** @file
 *  @brief ���ز����˵����ڶ���
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

#define MID_COUNT	20	/**	ÿ���˵����ID��*/
#define MID_MKID(lv,off)	(lv*MID_COUNT + off)	/**	���ݲ˵�����lv�Ͳ˵�ƫ�ƻ�ȡ�˵�ID lv��0��ʼ off��0��ʼ*/
#define MID_GTLV(id)	((int)(id/MID_COUNT))		/**	���ݲ˵�ID��ȡ�˵��ļ���*/
#define MID_GTOF(id)	((int)(id%MID_COUNT))		/**	���ݲ˵�ID��ȡ�˵������Ӳ˵��е�ƫ��*/

extern S_UIMENU MainMenu;
extern S_UIMENU GathQuery;

S_UIMENU GathRealtimeData =
{
	{"�����й��ܵ���ʾֵ", "�����й��ܵ���ʾֵ", "��ѹ", "����", "�й�����", 
		"", "", "", "", "", 
		""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&GathQuery, MID_MKID(2,3),	5
};
/************************************************************************************************************************************************
**�� �� ��: 
**��    ��: ��־ǿ 
**��    ��: 2013-9-11 9:52:38
**��    ��:	�������˵�
**-----------------------------------------------------------------------------------------------------
**��    ע: 
**	����־ǿ�����ע��
**-----------------------------------------------------------------------------------------------------
**********************************************************************************************************************************************/
S_UIMENU GathHistoryData =
{
	{"�������й��ܵ���ʾֵ", "�շ����й��ܵ���ʾֵ", "�ն������", "�¶������", "", 
		"", "", "", "", "", 
		""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&GathQuery, MID_MKID(2,2),	4
};
/************************************************************************************************************************************************
**�� �� ��: 
**��    ��: ��־ǿ 
**��    ��: 2013-9-11 9:52:38
**��    ��:	�������˵�
**-----------------------------------------------------------------------------------------------------
**��    ע: 
**	����־ǿ�����ע��
**-----------------------------------------------------------------------------------------------------
**********************************************************************************************************************************************/
S_UIMENU GathQuery =
{
	{"����ͳ������", "�쳣�澯", "��ʷ������ʾ", "ʵʱ������ʾ", "", 
		"", "", "", "", "", 
		""},
	{NULL,NULL,&GathHistoryData,&GathRealtimeData,NULL,NULL,NULL,NULL},
	&MainMenu, MID_MKID(1,0),	4
};

/************************************************************************************************************************************************
**�� �� ��: 
**��    ��: ��־ǿ 
**��    ��: 2013-9-11 9:52:38
**��    ��:	�������˵�
**-----------------------------------------------------------------------------------------------------
**��    ע: 
**	��־ǿ���
**	1��ParamQuery �˵��С�2-7 ����ͨ�����á����Ӳ˵�
**-----------------------------------------------------------------------------------------------------
**********************************************************************************************************************************************/
//S_UIMENU ParamTestCom =
//{
//	{" ����ͨ������", "", "", "", "", "", "", "", "", "", ""},
//	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
//	&MainMenu, MID_MKID(2,0),	1
//};
/************************************************************************************************************************************************
**�� �� ��: 
**��    ��: ��־ǿ 
**��    ��: 2013-9-11 9:52:38
**��    ��:	�������˵�
**-----------------------------------------------------------------------------------------------------
**��    ע: 
**	1������־ǿ�����ע�� 2013-9-11 9:52:38
**	2����־ǿ��Ӳ���ͨ�����á��޸Ĳ˵�����Ϊ7
**-----------------------------------------------------------------------------------------------------
**********************************************************************************************************************************************/
S_UIMENU ParamQuery =
{
	{"ͨѶͨ����ѯ������", "�����������ѯ������", "����������ѯ������", "��ֵ������ѯ", "", "", "", "", "", "", ""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&MainMenu, MID_MKID(1,1),	4
};


/************************************************************************************************************************************************
**�� �� ��: 
**��    ��: ��־ǿ 
**��    ��: 2013-9-11 9:52:38
**��    ��:	�������˵�
**-----------------------------------------------------------------------------------------------------
**��    ע: 
**	����־ǿ�����ע��
**-----------------------------------------------------------------------------------------------------
**********************************************************************************************************************************************/
S_UIMENU TermManage =
{
	{ "�����ն�", "Һ������", "�ն˱��","�ն˰汾", "�ն�ʱ������", "������������", "����ͨ������", "�ź�ǿ�Ⱥ͵�ص���", "����ʵʱ״̬", "", ""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&MainMenu, MID_MKID(1,2),	9
};

/************************************************************************************************************************************************
**�� �� ��: 
**��    ��: ��־ǿ 
**��    ��: 2013-9-11 9:52:38
**��    ��:	һ�����˵�
**-----------------------------------------------------------------------------------------------------
**��    ע: 
**	����־ǿ�����ע��
**-----------------------------------------------------------------------------------------------------
**********************************************************************************************************************************************/
S_UIMENU MainMenu =
{
	{"���ݲ�ѯ", "������ѯ������", "�ն˹�����ά��", "", "", "", "", "", "", "", ""},
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
				//����������ʾ				
				if (pData->m_pCurMenu->m_MenuID == MID_MKID(1,0) && pData->m_ScrollCtrl.GetSelectMenuID() == 0)
					GOTOWND(C_QueryGatherInfo, 0x00);//����ʾ����ͳ����Ϣ
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(1,0) && pData->m_ScrollCtrl.GetSelectMenuID() == 1)
					GOTOWND(C_PrintAlarmWnd/*C_QueryAlarmWnd*/, 0x00);//����ʾ�쳣�澯��Ϣ
				// ��ʾʵʱ����
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(2,3) && pData->m_ScrollCtrl.GetSelectMenuID()== 0)
					GOTOWND(C_PowerZXYGWnd, ZXYGPWRIND);//��ʵʱ�����������������й��ܵ���
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(2,3) && pData->m_ScrollCtrl.GetSelectMenuID()== 1)
					GOTOWND(C_PowerZXYGWnd, FXYGPWRIND);//��ʵʱ�����������㷴���й��ܵ���
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(2,3) && pData->m_ScrollCtrl.GetSelectMenuID()== 2)
					GOTOWND(C_PowerZXYGWnd, VOLTAGE);// ��ѹ
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(2,3) && pData->m_ScrollCtrl.GetSelectMenuID()== 3)
					GOTOWND(C_PowerZXYGWnd, CURRENT);// ����
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(2,3) && pData->m_ScrollCtrl.GetSelectMenuID()== 4)
					GOTOWND(C_PowerZXYGWnd, ACTPWR);// �й�����
				//��ʾ��ʷ����
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(2,2) && pData->m_ScrollCtrl.GetSelectMenuID() == 0)
					GOTOWND(C_MeasPntDataDisplayWnd, ZXYGPWRIND);//�ն��������й���
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(2,2) && pData->m_ScrollCtrl.GetSelectMenuID() == 1)
					GOTOWND(C_MeasPntDataDisplayWnd, FXYGPWRIND);//����ʾ�����й��ܵ���
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(2,2) && pData->m_ScrollCtrl.GetSelectMenuID() == 2)
					GOTOWND(C_HistoryDayWnd, 0x00);// �ն������
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(2,2) && pData->m_ScrollCtrl.GetSelectMenuID() == 3)
					GOTOWND(C_HistoryMonWnd, 0x00);// �¶������
				//����������鿴
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(1,1) && pData->m_ScrollCtrl.GetSelectMenuID() == 0)
					GOTOWND(C_CommunMenuWnd, 0x00);//ͨѶͨ������
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(1,1) && pData->m_ScrollCtrl.GetSelectMenuID() == 1)
					GOTOWND(C_MeterParamViewWnd, 0x00);//�������������
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(1,1) && pData->m_ScrollCtrl.GetSelectMenuID() == 2)
					GOTOWND(C_ForwardParamViewWnd, 0x00);//����������ѯ������
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(1,1) && pData->m_ScrollCtrl.GetSelectMenuID() == 3)
					GOTOWND(C_ThresholdViewWnd, 0x00);//�ն���ֵ������ѯ������
				//�ն˹�����ά��
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(1,2) && pData->m_ScrollCtrl.GetSelectMenuID() == 0)
					GOTOWND(C_RebootWnd, 0x00);//�����ն�	���������ն�
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(1,2) && pData->m_ScrollCtrl.GetSelectMenuID() == 1)
					GOTOWND(C_ScreenParamViewWnd, 0x00);//Һ������	�ɵ����ն�Һ����ʾ�Աȶȡ��������ȵ�
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(1,2) && pData->m_ScrollCtrl.GetSelectMenuID() == 2)
					GOTOWND(C_RtuaViewWnd, 0x00);//�ն˱������
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(1,2) && pData->m_ScrollCtrl.GetSelectMenuID() == 3)
					GOTOWND(C_ShowVersionWnd, 0x00);//�ն˰汾	����ʾ�ն���Ӳ���汾��
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(1,2) && pData->m_ScrollCtrl.GetSelectMenuID() == 4)
					GOTOWND(C_TerminalTimeViewWnd, 0x00);//�ն�ʱ������
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(1,2) && pData->m_ScrollCtrl.GetSelectMenuID() == 5)
					GOTOWND(C_PasswordWnd, 0x00);//������������
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(1,2) && pData->m_ScrollCtrl.GetSelectMenuID() == 6)
					GOTOWND(C_DebugMenuWnd, 0x00);//����ͨ������
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(1,2) && pData->m_ScrollCtrl.GetSelectMenuID() == 7)
					GOTOWND(C_SignalAndBatteryWnd, 0x00);//�ź�ǿ�Ⱥ͵�ص���
				else if (pData->m_pCurMenu->m_MenuID == MID_MKID(1,2) && pData->m_ScrollCtrl.GetSelectMenuID() == 8)
					GOTOWND(C_WirelessWnd, 0x00);//����ʵʱ״̬	���ṩ�ն��������������ʵʱ״̬��Ϣ
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
//����Ϊdemo������Ҫ��ע��
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
		pSelectBox->Create("89;ABC;����˭", 0x00, rect3, this, 3);
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
			SGui_MsgBox("��ʾ", "  ��Ϣ�򵯳��ˣ�", 0x00);
		break;
	case GM_CMD:
		if (wParam == 3 && lParam == GM_CMD_CTRL_UPDATE)
		{
			char buffer[32];
			sprintf(buffer, "ѡ����%d", lParam);
			SGui_MsgBox("��ʾ", buffer, 0x00);
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
			SGui_MsgBox("��ʾ", "  ��Ϣ�򵯳��ˣ�", MB_BTN_OK);
		else if (wParam == GVK_DN)
			SGui_MsgBox("��ʾ", "  ��Ϣ�򵯳��ˣ�", MB_BTN_OKCANCEL);
		else if (wParam == GVK_CANCEL)
			RETURNWND();
		else if (wParam == GVK_OK)
			SGui_MsgBox("��ʾ", "  ��Ϣ�򵯳��ˣ�", 0x00);
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
		pdc->DrawString(10, 2, "������̵�\"Ent\"������Ϣ");
		pdc->DrawString(10, 20, "�ֻ���:");
		pdc->DrawString(10, 36, "��������:");
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


