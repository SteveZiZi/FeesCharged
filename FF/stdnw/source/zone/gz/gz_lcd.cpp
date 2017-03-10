/** @file
 *  @brief 广州显示组件：本地操作与显示功能定义
 *  @author 
 *  @date 2016/04/11
 *  @version 0.1
 */
#include "gz_lcd.h"
#include "menuwnd.h"
#include "status.h"
#include "context.h"

#define BOTTOM_INFO_FLUSH_INTERVAL_S		1
#define DESKTOP_TURNVIEW_TIMEOUT_S		90

extern C_UIMAPP  theUIMApp;
static C_GZPaintTop g_GZPaintTop;

extern const char *s_pGprsSingle;
extern const char *s_pLittleBattery;
extern const char *s_pUsbDiskPlugin;
extern const char *s_pProgramKeyState;
//sym8x16 符号
const char *s_pAntenna = " !";
const char *s_pLinkMode = " $%";
//ASCII
const char *s_pCommMode[] = {" L", " G", " C", "3G", "4G"};

//
// 广州顶层栏绘制类
//
C_GZPaintTop::C_GZPaintTop(void)
{
	m_EventID = -1;
	m_fAntennaBlink = 0;
	m_fEventBlink = 0;
}
C_GZPaintTop::~C_GZPaintTop()
{
}

int C_GZPaintTop::Paint(C_SGUIDC *pDC)
{
	S_DATE date;
	char AsciiAntenna[4];
	char AsciiSignal[4];
	char AsciiCommMode[4];
	char AsciiLinkMode[4];
	char AsciiEventFlag[4];
	char AsciiEventValue[4];
	char ASciiBattery[4];
	char AsciiTime[8];

	AsciiAntenna[0] = *(s_pAntenna + GetAntennaState()), AsciiAntenna[1] = '\0';
	AsciiSignal[0] = *(s_pGprsSingle + GetSignal()), AsciiSignal[1] = '\0';
	sprintf(AsciiCommMode, "%s", *(s_pCommMode + GetCommunicationMode()));
	AsciiLinkMode[0] = *(s_pLinkMode + GetLinkMode()), AsciiLinkMode[1] = '\0';

	int sn = GetEventIndicator();
	if (sn < 0)
		AsciiEventFlag[0] = ' ',AsciiEventFlag[1]  = '\0';
	else if (sn == 0)
		AsciiEventFlag[0] = '!',AsciiEventFlag[1]  = '\0';
	else
		sprintf(AsciiEventValue, "%02X", sn);

	ASciiBattery[0] = *(s_pLittleBattery + GetBatteryLevel());
	ASciiBattery[1] = *(s_pUsbDiskPlugin + GetUsbDiskFlag());
	ASciiBattery[2] = *(s_pProgramKeyState + GetProgramKeyFlag());
	ASciiBattery[3] = '\0';

	C_TIME::LocalTime(C_TIME::TimeNow(), date);
	sprintf(AsciiTime, "%02d:%02d", date.m_Hour, date.m_Min);

	bool en = pDC->FlushScreenEn(false);
	pDC->SelectObject(pDC->GetStockGuiObj(SGUI_OBJ_SYM08x16));
	pDC->DrawString(2+8*0, 1, AsciiAntenna);
	pDC->DrawString(2+8*5, 1, AsciiLinkMode);
	pDC->SelectObject(pDC->GetStockGuiObj(SGUI_OBJ_SYM16x16));
	pDC->DrawString(2+8*1, 1, AsciiSignal);
	if (sn <= 0)
		pDC->DrawString(2+8*6, 1, AsciiEventFlag);
	pDC->DrawString(2+8*8, 1, ASciiBattery);
	pDC->SelectObject(pDC->GetStockGuiObj(SGUI_OBJ_F16x16));
	pDC->DrawString(2+8*3, 1, AsciiCommMode);
	if (sn > 0)
		pDC->DrawString(2+8*6, 1, AsciiEventValue);
	pDC->DrawString(6+8*14, 1, AsciiTime);
	pDC->DrawHLine(0, 18, 160);
	pDC->FlushScreenEn(en);
	pDC->FlushScreen();
	return 0;
}

/** @brief 天线状态
 *  @return 0:hide;1:show 
 */
int C_GZPaintTop::GetAntennaState(void)
{
	S_ConnectStatus *pCS = CONTEXT.GetConnectStatus();
	if (pCS->m_Httx1Logon == true)
		return 1; // 登陆常亮

	// 不处理以太网登陆的情况，因为它1-2秒就完成了
	//处理MODEM登陆情况
	m_fAntennaBlink = !m_fAntennaBlink;
	S_PppStatus *pPppStatus = CONTEXT.GetPppStatus();
	if (pPppStatus->m_fGprsDialing == true)
		return (int)m_fAntennaBlink;
	return 0;
}
/** @brief 通信模式
 *  @return 0-4: L G C 3G 4G
 */
int C_GZPaintTop::GetCommunicationMode(void)
{
	S_ConnectStatus *pCS = CONTEXT.GetConnectStatus();
	if (pCS->m_Httx1OnPpp == false) // ethernet
		return 0;

	S_PppStatus *pPppStatus = CONTEXT.GetPppStatus();
	return (int)pPppStatus->m_WirelessNetType + 1;
}
/** @brief 连接模式
 *  @return 0:listen;1:Client;2:Server
 */
int C_GZPaintTop::GetLinkMode(void)
{// 查看当前通道的客户端和服务端设定
	S_ConnectStatus *pCS = CONTEXT.GetConnectStatus();
	if (pCS->m_Httx1Logon == true)
	{
		if (pCS->m_Httx1CommStatus == HTTX1_COMM_STATUS_CLIENT_LINK)
			return 1;
		if (pCS->m_Httx1CommStatus == HTTX1_COMM_STATUS_SERVER_LINK)
			return 2;
	}
	return 0;
}
/** @brief 事件状态获取
 *  @return <0 无显示
 *  @return =0 感叹号
 *  @return >0 事件编码
 */
int C_GZPaintTop::GetEventIndicator(void)
{
	S_AlarmStatus *pAS = STATUS.GetAlarmStatus();
	if (pAS->m_bsNewAlarmID.any() == false) // 没有告警
		return -1;

	m_fEventBlink = !m_fEventBlink;
	if (m_fEventBlink == 0) // 显示感叹号
		return 0;

	if (m_EventID < 0) // 第一次显示
	{
		m_EventID = (int)pAS->m_bsNewAlarmID.firstbit();
	}
	else
	{
		m_EventID = (int)pAS->m_bsNewAlarmID.nextbit((size_t)m_EventID);
		if (m_EventID == (int)pAS->m_bsNewAlarmID.size()) // 到头了
			m_EventID = (int)pAS->m_bsNewAlarmID.firstbit();
	}
	if (m_EventID == (int)pAS->m_bsNewAlarmID.size()) // 没有告警产生
		m_EventID = -1;
	return m_EventID;
}

extern const char *WeekName[];
C_GZDESKTOP::C_GZDESKTOP(void)
{
	m_DesktopTimeoutS = 0;
	m_fEthernetBlink = 0;
	m_fInfraredBlink = 0;
	m_fWirelessBlink = 0;
	m_fZBWXBlink = 0;
	m_f485IBlink = 0;
	m_f485IIBlink = 0;
	m_f485IIIBlink = 0;
	m_fUsbBlink = 0;
	m_fFlashBlink = 0;
}
int C_GZDESKTOP::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	switch (msg)
	{
	case GM_CREATE:
		break;
	case GM_PAINT:{
		int size;
		S_DATE date;
		char strBuffer[64];
		S_LcdInfo *pLcdInfo = STATUS.GetLcdInfo();
		C_SGUIDC *pdc = BeginPaint();

		C_TIME::LocalTime(C_TIME::TimeNow(), date);
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		size = pdc->GetStringExtent(pLcdInfo->m_TermName);
		pdc->DrawString((160-size)/2, 30, pLcdInfo->m_TermName);
		sprintf(strBuffer, "%d-%02d-%02d", date.m_Year, date.m_Mon, date.m_Day);
		size = pdc->GetStringExtent(strBuffer);
		pdc->DrawString((160-size)/2, 60, strBuffer);
//		size = pdc->GetStringExtent(WeekName[date.m_WeekDay]);
//		pdc->DrawString((160-size)/2, 80, WeekName[date.m_WeekDay]);
		sprintf(strBuffer, "%02d:%02d:%02d", date.m_Hour, date.m_Min, date.m_Sec);
		size = pdc->GetStringExtent(strBuffer);
		pdc->DrawString((160-size)/2, 80, strBuffer);
		pdc->DrawHLine(0, 122, 160);
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_SYM16x16));
		pdc->DrawString(8, 124, GetDesktopBottomInfo(strBuffer));
		EndPaint(pdc);
		}break;
	case GM_TIME:
		m_DesktopTimeoutS += 1;
		if (m_DesktopTimeoutS % BOTTOM_INFO_FLUSH_INTERVAL_S == 0)
		{
			InvalidateRect(NULL);
		}
		if (m_DesktopTimeoutS >= DESKTOP_TURNVIEW_TIMEOUT_S)
		{
			//m_DesktopTimeoutS = 0;
			//GOTOWND(C_TURNVIEW, 0);
		}
		break;
	case GM_KEYUP:
		if (wParam == GVK_OK)
		{
			//m_DesktopTimeoutS = 0;
			GOTOWND(C_MENUWND, 0);
		}
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

//sym16x16 符号
const char *s_pEthernetState = " WX";
const char *s_pInfraredState = " YZ";
const char *s_pWirelessState = " ]^";
const char *s_pZBWXState = " _`";
const char *s_pRS485IState= " ab";
const char *s_pRS485IIState= " cd";
const char *s_pRS485IIIState= " ef";
const char *s_pUSBState = " gh";
const char *s_pStageState = " ij";

const char *C_GZDESKTOP::GetDesktopBottomInfo(char *pstrBuffer)
{
	pstrBuffer[0] = *(s_pEthernetState + GetEthernetState());
	pstrBuffer[1] = *(s_pInfraredState + GetInfraredState());
	pstrBuffer[2] = *(s_pWirelessState + GetWirelessState());
	pstrBuffer[3] = *(s_pZBWXState + GetZBWXState());
	pstrBuffer[4] = *(s_pRS485IState + GetRS485IState());
	pstrBuffer[5] = *(s_pRS485IIState + GetRS485IIState());
	pstrBuffer[6] = *(s_pRS485IIIState + GetRS485IIIState());
	pstrBuffer[7] = *(s_pUSBState + GetUSBState());
	pstrBuffer[8] = *(s_pStageState + GetFlashState());
	pstrBuffer[9] = '\0';
	return pstrBuffer;
}

int C_GZDESKTOP::GetEthernetState(void)
{
	S_LcdStatus *pLcdStatus = CONTEXT.GetLcdStatus();
	if (pLcdStatus->m_Eth0LinePlugin == false)
		return 1;

	S_ConnectStatus *pCS = CONTEXT.GetConnectStatus();
	if (pCS->m_Httx1OnPpp == false/* && pCS->m_Httx1Logon == true*/) // 采用以太网登陆
	{
		m_fEthernetBlink = !m_fEthernetBlink;
		if (C_TIME::TimeNow() - pCS->m_Httx1CommTime <= 20)
			return (int)(m_fEthernetBlink + 1);
	}
	return 2;
}
int C_GZDESKTOP::GetInfraredState(void)
{
	m_fInfraredBlink = !m_fInfraredBlink;
	if (SerialCommDuring(20) == true)
		return (int)(m_fInfraredBlink + 1);
	return 1;
}
int C_GZDESKTOP::GetWirelessState(void)
{
	S_ConnectStatus *pCS = CONTEXT.GetConnectStatus();
	if (pCS->m_Httx1OnPpp == false)
		return 1;

//	if (pCS->m_Httx1Logon == true) // 采用MODEM登陆
	{
		m_fWirelessBlink = !m_fWirelessBlink;
		if (C_TIME::TimeNow() - pCS->m_Httx1CommTime <= 20)
			return (int)(m_fWirelessBlink + 1);
	}
	return 1;
}
int C_GZDESKTOP::GetZBWXState(void)
{
	m_fZBWXBlink = !m_fZBWXBlink;
	if (SerialCommDuring(32) == true)
		return (int)(m_fZBWXBlink + 1);
	return 1;
}
int C_GZDESKTOP::GetRS485IState(void)
{
	m_f485IBlink = !m_f485IBlink;
	if (SerialCommDuring(1) == true)
		return (int)(m_f485IBlink + 1);
	return 1;
}
int C_GZDESKTOP::GetRS485IIState(void)
{
	m_f485IIBlink = !m_f485IIBlink;
	if (SerialCommDuring(2) == true)
		return (int)(m_f485IIBlink + 1);
	return 1;
}
int C_GZDESKTOP::GetRS485IIIState(void)
{
	m_f485IIIBlink = !m_f485IIIBlink;
	if (SerialCommDuring(3) == true)
		return (int)(m_f485IIIBlink + 1);
	return 1;
}
int C_GZDESKTOP::GetUSBState(void)
{
	S_LcdStatus *pLcdStatus = CONTEXT.GetLcdStatus();
	if (pLcdStatus->m_UsbDiskPlugin == false)
		return 1;

	m_fUsbBlink = !m_fUsbBlink;
	if (pLcdStatus->m_UsbCopyFile == true)
		return (int)(m_fUsbBlink + 1);
	return 1;
}
int C_GZDESKTOP::GetFlashState(void)
{
	m_fFlashBlink = !m_fFlashBlink;
	S_LcdStatus *pLcdStatus = CONTEXT.GetLcdStatus();
	if (C_TIME::TimeNow() - pLcdStatus->m_timeFlashStorage <= 20)
		return (int)(m_fFlashBlink + 1);
	return 1;
}
bool C_GZDESKTOP::SerialCommDuring(S_BYTE VCom)
{
	S_LcdStatus *pLcdStatus = CONTEXT.GetLcdStatus();
	if (C_TIME::TimeNow() - pLcdStatus->m_timeSerialComm[VCom] <= 20)
		return true;
	return false;
}

//
//
//
int C_GzLcdServer::BeforeService(void)
{
	if (theUIMApp.SysInit(CFG_PATH_RESOURCE) != 0)
		return -1;
	C_UIMWND::SetPaintTopComponent(&g_GZPaintTop);

	m_pGzDesktop = new C_GZDESKTOP;
	m_pLcdInfo = STATUS.GetLcdInfo();

	C_SGUIAPP::SetCurApp(&theUIMApp);
	theUIMApp.SetDesktopWnd(m_pGzDesktop);
	theUIMApp.CloseBacklight();
	theUIMApp.SetCloseLightDelay(m_pLcdInfo->m_LCDLightTimeoutS);
	theUIMApp.SetBackDesktopDelay(m_pLcdInfo->m_BackDesktopTimeoutS);
	theUIMApp.SetLcdContrast(m_pLcdInfo->m_LCDContrast);
	theUIMApp.SetLcdBrightness(m_pLcdInfo->m_LCDBrightness);
	theUIMApp.SetLcdGrayness(m_pLcdInfo->m_LCDGreyScale);
	theUIMApp.SetLcdDirection(m_pLcdInfo->m_LCDDirection);

	return 0;
}

int C_GzLcdServer::AfterService(void)
{
	delete m_pGzDesktop;
	return 0;
}


