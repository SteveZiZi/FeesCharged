/** @file
 *  @brief ���ز��������ڶ���
 *  @author 
 *  @date 2012/01/09
 *  @version 0.1
 *  �û����ڱ���̳�C_UIMWND
 */
#include "uiwnd.h"
#include "timeop.h"
#include "context.h"
#include "status.h"
#include "modem.h"
#include "sguictrl.h"

//sym16x16 ����
const char *s_pProgramKeyState = " p";
const char *s_pUsbDiskPlugin = " [";
const char *s_pGprsSingle = "#$%&'";
const char *s_pGprsSingle_G = "()*+,";
const char *s_pGprsSingle_C = "-./01";
const char *s_pGprsSingle_L = "23456";
const char *s_pCommStatus_T = "?@ABCDEFG";
const char *s_pCommStatus = "HI";
const char *s_pLittleBattery = "JKLMNO";
const char *s_pBitBattery = "7>8>9>:;:<:=";
//sym8x16 ����
const char *s_pLeftPaneDigit = "&'()*+,-./";
const char *s_pRightPaneDigit = "0123456789";
const char *s_pLeftBlackPaneDigit = ":;<=>?@ABC";
const char *s_pRightBlackPaneDigit = "DEFGHIJKLM";

static C_PaintTop g_UIMPaintTop;
C_PaintTop *C_UIMWND::m_pPaintTop = &g_UIMPaintTop;

//
// ������������
//
int C_PaintTop::Paint(C_SGUIDC *pDC)
{
	int sn;
	S_DATE date;
	char AsciiBuffer[32];
	char AsciiEvent[12];
	char AsciiMP[12];

	C_TIME::LocalTime(C_TIME::TimeNow(), date);
	sn = GetSignal(), AsciiBuffer[0] = *(s_pGprsSingle + sn);
	sn = GetCommunicationStatus(), AsciiBuffer[1] = *(s_pCommStatus_T + sn);
	sn = GetEventIndicator();
	if (sn < 0) AsciiBuffer[2] = '!';
	else AsciiBuffer[2] = ' ';
	if (sn > 0) AsciiEvent[0] = *(s_pLeftBlackPaneDigit+sn/10%10), AsciiEvent[1] = *(s_pRightBlackPaneDigit+sn%10);
	else AsciiEvent[0] = ' ', AsciiEvent[1] = ' ';
	AsciiEvent[2] = '\0';

	sn = GetBatteryLevel(), AsciiBuffer[3] = *(s_pLittleBattery + sn);
	if ((sn = GetUsbDiskFlag()) != 0)
		AsciiBuffer[4] = *(s_pUsbDiskPlugin + sn);
	else
		sn = GetProgramKeyFlag(), AsciiBuffer[4] = *(s_pProgramKeyState + sn);
//		AsciiBuffer[3] = ' ', AsciiBuffer[4] = ' ';
	AsciiBuffer[5] = '\0';

	sn = GetDisplayedMP();
	if (sn > 0) AsciiMP[0] = *(s_pLeftPaneDigit+sn/10%10), AsciiMP[1] = *(s_pRightPaneDigit + sn%10);
	else AsciiMP[0] = ' ', AsciiMP[1] = ' ';
	AsciiMP[2] = '\0';

	bool en = pDC->FlushScreenEn(false);
	pDC->SelectObject(pDC->GetStockGuiObj(SGUI_OBJ_SYM16x16));
	pDC->DrawString(2, 1, AsciiBuffer);
	pDC->SelectObject(pDC->GetStockGuiObj(SGUI_OBJ_SYM08x16));
	if (AsciiEvent[0] != ' ')
		pDC->DrawString(2+16*2, 1, AsciiEvent);
	pDC->DrawString(2+16*5, 1, AsciiMP);
	sprintf(AsciiBuffer, "%02d:%02d", date.m_Hour, date.m_Min);
	pDC->SelectObject(pDC->GetStockGuiObj(SGUI_OBJ_F16x16));
	pDC->DrawString(116, 1, AsciiBuffer);
	pDC->DrawHLine(0, 18, 160);
	pDC->FlushScreenEn(en);
	pDC->FlushScreen();
	return 0;
}

/** @brief �����źŻ�ȡ
 *  @return 0-4 
 */
int C_PaintTop::GetSignal(void)
{
	S_PppStatus *pPppStatus = CONTEXT.GetPppStatus();
	if (pPppStatus->m_Signal == 0)
		return 0;
	S_BYTE level = pPppStatus->m_Signal / 8 + 1;
	if (level > 4) level = 4;
	return level;
}

/** @brief ͨѶ״̬��ȡ
 *  @return 0-2 GprsStatus 0δ��½1����2ͨѶ
 *  @return 3-5 CdmaStatus 3δ��½4����5ͨѶ
 *  @return 6-7 LanStatus 6δ��½7����8ͨѶ
 */
int C_PaintTop::GetCommunicationStatus(void)
{
	int status;
	S_PppStatus *pPppStatus = CONTEXT.GetPppStatus();
	S_ConnectStatus *pCS = CONTEXT.GetConnectStatus();
	if (pCS->m_Httx1OnPpp == false) // ethernet
		status = 6;
	else if (pPppStatus->m_ModemType < MODEM_TYPE_GPRS_MAX)
		status = 0;
	else if (pPppStatus->m_ModemType < MODEM_TYPE_CDMA_MAX)
		status = 3;
	else
		status = 0;//default GPRS
	if (pCS->m_Httx1Logon == true)
	{
		status += 1;
		if (C_TIME::TimeNow() - pCS->m_Httx1CommTime <= 30)
			status += 1;
	}

	return status;
}

/** @brief �¼�״̬��ȡ
 *  @return -1 ��ʾ��ð��
 *  @return 0 ����ʾ
 *  @return >0 �¼�����
 */
int C_PaintTop::GetEventIndicator(void)
{
	S_AlarmStatus *pAS = STATUS.GetAlarmStatus();
	if (pAS->m_EventNewCnt > 0 || pAS->m_ImportantNewCnt > 0 || pAS->m_NormalNewCnt > 0)
		return -1;
	return 0;
}

/** @brief ��ؼ����ȡ
 *  @return 0-5
 */
int C_PaintTop::GetBatteryLevel(void)
{
	static S_BYTE LastLevel = 5;
	S_PowerStatus *pPowerStatus = CONTEXT.GetPowerStatus();
	if (pPowerStatus->m_BatteryCap == 0)
		return 0;// û������ʾ��
	if (pPowerStatus->m_fCharge == true && LastLevel < 5)//����У�������ʾ
		return ++LastLevel;

	S_BYTE level = 1;
	if (pPowerStatus->m_BatteryCap > 10)
	{//ȡʵ�ʵ�������
		level = pPowerStatus->m_BatteryCap / 20 + 1;
		if (level > 5) level = 5;
	}

	if (pPowerStatus->m_fCharge == true)
	{//��紦��
		if (level > 1) level -= 1;//��һ���Ա㶯����ʾ
		LastLevel = level;
	}

	return level;
}

/** @brief ȡU�̲���״̬
 *  @return 0 ��U�̲���
 *  @return 1 U�̲���
 */
int C_PaintTop::GetUsbDiskFlag(void)
{
	S_LcdStatus *pLcdStatus = CONTEXT.GetLcdStatus();
	if (pLcdStatus->m_UsbDiskPlugin == true)
		return 1;
	return 0;
}

/** @brief ȡ��̼�״̬
 *  @return 0 ��̼�δ����
 *  @return 1 ��̼��Ѱ���
 */
int C_PaintTop::GetProgramKeyFlag(void)
{
	S_LcdStatus *pLcdStatus = CONTEXT.GetLcdStatus();
	if (pLcdStatus->m_ProgramKeyDelayS > 0)
		return 1;
	return 0;
}

/** @brief ȡ��ǰ��ʾ��Ϣ�Ĳ������
 *  @return 0 ��
 *  @return >0 ����������
 */
int C_PaintTop::GetDisplayedMP(void)
{
	return 0;
}

//
// Ӧ�ò㴰�ڸ���
//
int C_UIMWND::SetPaintTopComponent(C_PaintTop *pPaintTop)
{
	m_pPaintTop = pPaintTop;
	return 0;
}

int C_UIMWND::DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	switch (msg)
	{
	case GM_SYSPAINT:
		if (m_pPaintTop != NULL)
		{
			C_SGUIDC DC;
			m_pPaintTop->Paint(&DC);
		}
		break;
	case GM_SYSINFO:
		if (wParam == 1) // ������Ϣ
		{
			C_UpdateDlog UpdateDlog;
			UpdateDlog.DoModel();
		}
		break;
	default:
		return C_SGUIWND::DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

/** @brief delete �ؼ�
 *  @param[in] idMin ��С�ؼ�ID��
 *  @param[in] idMax ���ؼ�ID��
 *  @return 0
 *  @note �ؼ���������new������
 */
int C_UIMWND::DeleteWndCtrl(int idMin, int idMax)
{
	for (; idMin <= idMax; idMin++)
	{
		delete RemoveControl(idMin);
	}
	return 0;
}

/** @brief ������֤��
 */
 #define PWDDLAG_CTRLID_NUMEDIT		1
 #define PWDDLAG_CTRLID_BUTTON_OK	2
 #define PWDDLAG_CTRLID_BUTTON_CAN	3
 #define PWDDLAG_DELAY_MAXMS		(30*GM_TIME_INTERVAL_MS)

C_PWDDLOG::C_PWDDLOG(int pwd)
{
	m_pwd = pwd;
	m_DelayMS = 0;
}

C_PWDDLOG::~C_PWDDLOG()
{
}

int C_PWDDLOG::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	switch (msg)
	{
	case GM_CREATE:
		m_WndRect.x = 20;
		m_WndRect.w = 120;
		m_WndRect.y = 50, m_WndRect.h = 80;
		CreateCtrl();
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		S_RECT WndRect = {0, 0, m_WndRect.w, m_WndRect.h};
		pdc->DrawRect(WndRect);
		pdc->DrawHLine(1, 16, m_WndRect.w-2);
		pdc->DrawString(2, 2, "����������");
		EndPaint(pdc);
		}	
		break;
	case GM_KEYUP:
		m_DelayMS = 0; ///�˳���ʱ���¼���
		if (wParam == GVK_OK || wParam == GVK_DN)
			SetFocusCtrl(GetWndCtrl(PWDDLAG_CTRLID_BUTTON_OK));
		else if (wParam == GVK_UP)
			SetFocusCtrl(GetWndCtrl(PWDDLAG_CTRLID_NUMEDIT));
		else if (wParam == GVK_LEFT)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			if (pCtrl->GetCtrlID() == PWDDLAG_CTRLID_BUTTON_CAN)
				SetFocusCtrl(GetWndCtrl(PWDDLAG_CTRLID_BUTTON_OK));
		}
		else if (wParam == GVK_RIGHT)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			if (pCtrl->GetCtrlID() == PWDDLAG_CTRLID_BUTTON_OK)
				SetFocusCtrl(GetWndCtrl(PWDDLAG_CTRLID_BUTTON_CAN));
		}
		break;
	case GM_CMD:
		m_DelayMS = 0; ///�˳���ʱ���¼���
		if (wParam == PWDDLAG_CTRLID_BUTTON_OK && lParam == GM_CMD_BTN_PUSHED)//OK
		{
			C_GUICTRL *pNum = GetWndCtrl(PWDDLAG_CTRLID_NUMEDIT);
			string strNum = pNum->GetWndText();
			int CurPwd = atoi(strNum.c_str());
			if (CurPwd == m_pwd)
			{
				EndDlg();
				return MB_RTN_YESOK;
			}
			C_SGUIDC dc(this);
			dc.DrawString(16, 44, "������֤ʧ��!!!");
		}
		else if (wParam == PWDDLAG_CTRLID_BUTTON_CAN && lParam == GM_CMD_BTN_PUSHED)//Cancel
		{
			EndDlg();
			return MB_RTN_CANCEL;
		}
		break;
	case GM_TIME:
		m_DelayMS += GM_TIME_INTERVAL_MS;
		if (m_DelayMS > PWDDLAG_DELAY_MAXMS)
		{
			EndDlg();
			return MB_RTN_NULL;
		}
		break;
	case GM_DESTROY:
		DestroyCtrl();
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

int C_PWDDLOG::DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	return C_SGUIDLG::DefWndProcess(msg, wParam, lParam);
}

int C_PWDDLOG::CreateCtrl(void)
{
	S_RECT NumRect = {35, 22, 50, 16};
	C_NUMEDIT *pNumEdit = C_NUMEDIT::NewCtrl();
	pNumEdit->Create("999999", NUMEDIT_STYLE_DEC, NumRect, this, PWDDLAG_CTRLID_NUMEDIT);
	
	C_BUTTON *pbtnOK = C_BUTTON::NewCtrl();
	C_BUTTON *pbtnCancel = C_BUTTON::NewCtrl();
	S_RECT rect;
	rect.w = 32; rect.h = 16;
	rect.x = (m_WndRect.w - (rect.w + rect.w + 10)) / 2;
	rect.y = 60;
	pbtnOK->Create("ȷ��", 0x00, rect, this, PWDDLAG_CTRLID_BUTTON_OK);
	rect.x += rect.w + 10;
	pbtnCancel->Create("ȡ��", 0x00, rect, this, PWDDLAG_CTRLID_BUTTON_CAN);

	SetFocusCtrl(pNumEdit);
	return 0;
}

int C_PWDDLOG::DestroyCtrl(void)
{
	delete (RemoveControl(PWDDLAG_CTRLID_NUMEDIT));
	delete (RemoveControl(PWDDLAG_CTRLID_BUTTON_OK));
	delete (RemoveControl(PWDDLAG_CTRLID_BUTTON_CAN));
	return 0;
}


/** @brief ������֤
 */
bool C_PWD::m_fCertification = false;

/** @brief ������֤
 *  @return true ��֤ͨ��;false δͨ��
 *  @remarks ��Ϣ��ͶԻ����ܵ��ñ�����
 */
bool C_PWD::Certification(void)
{
	if (m_fCertification == true)
		return true;
	S_LcdInfo *pLcdInfo = STATUS.GetLcdInfo();
	C_PWDDLOG PwdDlog(pLcdInfo->m_LCDPassword);
	if (PwdDlog.DoModel() == MB_RTN_YESOK)
	{
		m_fCertification = true;
		return true;
	}
	return false;
}

/** @brief ����������֤��־
 */
void C_PWD::SetCertification(bool fOk)
{
	m_fCertification = fOk;
}

/** @brief ������Ϣ��ʾ��
 */
C_UpdateDlog::C_UpdateDlog(void)
{
}

C_UpdateDlog::~C_UpdateDlog()
{
}

static const char *g_UpdateDlogInfo[] = {"����������...", "У��������...", "У��ɹ�.....", "У��ʧ��.....", "ϵͳ������..."};
int C_UpdateDlog::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	switch (msg)
	{
	case GM_CREATE:
		m_DelayS = 0;
		m_WndRect.x = 20, m_WndRect.w = 120;
		m_WndRect.y = 50, m_WndRect.h = 80;
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		S_LcdStatus *pLcdStatus = CONTEXT.GetLcdStatus();
		S_RECT WndRect = {0, 0, m_WndRect.w, m_WndRect.h};
		S_WORD idxInfo = pLcdStatus->m_UpdateInfo;
		if (idxInfo >= KGE_MEMBER_OF(g_UpdateDlogInfo))
			idxInfo = 0x00;
		pdc->DrawRect(WndRect);
		pdc->DrawHLine(1, 16, m_WndRect.w-2);
		pdc->DrawString(2, 2, "������ʾ:");
		pdc->DrawString(16, 40, g_UpdateDlogInfo[idxInfo]);
		EndPaint(pdc);
		}
		break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
		{
			EndDlg();
			return MB_RTN_CANCEL;
		}
		break;
	case GM_TIME:
		m_DelayS += 1;
		if (m_DelayS > 10)
		{
			EndDlg();
			return MB_RTN_NULL;
		}
		else
		{
			InvalidateRect(NULL);
		}
		break;
	case GM_DESTROY:
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

int C_UpdateDlog::DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	return C_SGUIDLG::DefWndProcess(msg, wParam, lParam);
}


