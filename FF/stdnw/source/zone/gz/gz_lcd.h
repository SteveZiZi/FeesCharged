/** @file
 *  @brief ������ʾ��������ز�������ʾ���ܶ���
 *  @author 
 *  @date 2016/04/11
 *  @version 0.1
 */
#ifndef __GZ_LCD_SERVER_H__
#define __GZ_LCD_SERVER_H__
#include "lcd.h"
#include "uiwnd.h"

//
// ���ݶ�����������
//
class C_GZPaintTop:public C_PaintTop
{
public:
	C_GZPaintTop(void);
	virtual ~C_GZPaintTop();

	virtual int Paint(C_SGUIDC *pDC);

protected:
	/** @brief ����״̬
	 *  @return 0:hide;1:show 
	 */
	int GetAntennaState(void);
	/** @brief ͨ��ģʽ
	 *  @return 0-4: L G C 3G 4G
	 */
	int GetCommunicationMode(void);
	/** @brief ����ģʽ
	 *  @return 0:listen;1:Client;2:Server
	 */
	int GetLinkMode(void);

	/** @brief �¼�״̬��ȡ
	 *  @return <0 ����ʾ
	 *  @return =0 ��̾��
	 *  @return >0 �¼�����
	 */
	virtual int GetEventIndicator(void);

protected:
	int m_EventID; ///< �¼�ID
	S_BYTE m_fAntennaBlink; ///< ������˸
	S_BYTE m_fEventBlink; ///< �澯��־��˸
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
	S_BYTE m_fEthernetBlink; ///< ��̫����־��˸
	S_BYTE m_fInfraredBlink; ///< �����־
	S_BYTE m_fWirelessBlink; ///< MODEM��־��˸
	S_BYTE m_fZBWXBlink; ///< �ز���־
	S_BYTE m_f485IBlink; ///< RS485I��־
	S_BYTE m_f485IIBlink; ///< RS485II��־
	S_BYTE m_f485IIIBlink; ///< RS485III��־
	S_BYTE m_fUsbBlink; ///< USB��־��˸
	S_BYTE m_fFlashBlink; ///< FLASH��־
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

