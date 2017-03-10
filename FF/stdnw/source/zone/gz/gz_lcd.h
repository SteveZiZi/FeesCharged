/** @file
 *  @brief 广州显示组件：本地操作与显示功能定义
 *  @author 
 *  @date 2016/04/11
 *  @version 0.1
 */
#ifndef __GZ_LCD_SERVER_H__
#define __GZ_LCD_SERVER_H__
#include "lcd.h"
#include "uiwnd.h"

//
// 广州顶层栏绘制类
//
class C_GZPaintTop:public C_PaintTop
{
public:
	C_GZPaintTop(void);
	virtual ~C_GZPaintTop();

	virtual int Paint(C_SGUIDC *pDC);

protected:
	/** @brief 天线状态
	 *  @return 0:hide;1:show 
	 */
	int GetAntennaState(void);
	/** @brief 通信模式
	 *  @return 0-4: L G C 3G 4G
	 */
	int GetCommunicationMode(void);
	/** @brief 连接模式
	 *  @return 0:listen;1:Client;2:Server
	 */
	int GetLinkMode(void);

	/** @brief 事件状态获取
	 *  @return <0 无显示
	 *  @return =0 感叹号
	 *  @return >0 事件编码
	 */
	virtual int GetEventIndicator(void);

protected:
	int m_EventID; ///< 事件ID
	S_BYTE m_fAntennaBlink; ///< 天线闪烁
	S_BYTE m_fEventBlink; ///< 告警标志闪烁
};

class C_GZDESKTOP:public C_UIMWND
{
public:
	C_GZDESKTOP(void);
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	const char *GetDesktopBottomInfo(char *pstrBuffer);

	int GetEthernetState(void);
	int GetInfraredState(void);
	int GetWirelessState(void);
	int GetZBWXState(void);
	int GetRS485IState(void);
	int GetRS485IIState(void);
	int GetRS485IIIState(void);
	int GetUSBState(void);
	int GetFlashState(void);
	bool SerialCommDuring(S_BYTE VCom);

protected:
	S_DWORD m_DesktopTimeoutS;
	S_BYTE m_fEthernetBlink; ///< 以太网标志闪烁
	S_BYTE m_fInfraredBlink; ///< 红外标志
	S_BYTE m_fWirelessBlink; ///< MODEM标志闪烁
	S_BYTE m_fZBWXBlink; ///< 载波标志
	S_BYTE m_f485IBlink; ///< RS485I标志
	S_BYTE m_f485IIBlink; ///< RS485II标志
	S_BYTE m_f485IIIBlink; ///< RS485III标志
	S_BYTE m_fUsbBlink; ///< USB标志闪烁
	S_BYTE m_fFlashBlink; ///< FLASH标志
};


class C_GzLcdServer:public C_LcdServer
{
public:
	C_GzLcdServer(void){m_pGzDesktop = NULL;}
	virtual ~C_GzLcdServer(){}

	virtual int BeforeService(void);
	virtual int AfterService(void);

protected:
	C_GZDESKTOP *m_pGzDesktop;
};

#endif//__GZ_LCD_SERVER_H__

