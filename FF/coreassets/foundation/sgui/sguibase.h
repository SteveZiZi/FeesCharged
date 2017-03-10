//---------------------------------------------------------------------------------
// Copyright  2009-2010
// E-mail:   @163.com
// Project:			SGUI(Simple GUI)
// File:			sguibase.h
// Description:		SGUI for Power user elect energy acquire system;unsupport multi_thread
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2010-01-18  Create                  Kingsea
//---------------------------------------------------------------------------------
#ifndef __SGUIBASE_H__
#define __SGUIBASE_H__

#include "sguimsg.h"

//
//SGUI driver lay
//
class C_SguiDriverBase
{
public:
	virtual int DriverID(void);
	virtual char *LcdBuffer(void);
	virtual unsigned short LcdWidth(void);
	virtual unsigned short LcdHeight(void);
	virtual unsigned short LcdBpp(void);

	virtual int fbinit(void);
	virtual int fbfresh(void);

	virtual void backlight(int on);
	virtual void SetContrast(int contrast);
	virtual void SetBrightness(int brightness);
	virtual void SetGrayness(int grayness);
	virtual void SetDirection(int direction);
	virtual void OpenScreen(void);
	virtual void CloseScreen(void);

	virtual int kbinit(void);
	virtual int ScanKeyboard(void);

protected:
	int _ScanKeyboard(void);
	int _ScanProgramkey(void);
};

class C_SguiDriverCq6100:public C_SguiDriverBase
{
public:
	virtual int DriverID(void);
	virtual unsigned short LcdWidth(void);
	virtual unsigned short LcdHeight(void);
	virtual unsigned short LcdBpp(void);

	virtual int fbinit(void);
	virtual int fbfresh(void);

	virtual void backlight(int on);
	virtual void SetContrast(int contrast);
	virtual void SetBrightness(int brightness);
	virtual void SetGrayness(int grayness);
	virtual void SetDirection(int direction);
	virtual void OpenScreen(void);
	virtual void CloseScreen(void);

	virtual int kbinit(void);
	virtual int ScanKeyboard(void);
};

class C_SguiDriverEac5000D:public C_SguiDriverBase
{
public:
	virtual int DriverID(void);
	virtual unsigned short LcdWidth(void);
	virtual unsigned short LcdHeight(void);
	virtual unsigned short LcdBpp(void);

	virtual int fbinit(void);
	virtual int fbfresh(void);

	virtual void backlight(int on);
	virtual void SetContrast(int contrast);
	virtual void SetBrightness(int brightness);
	virtual void SetGrayness(int grayness);
	virtual void SetDirection(int direction);
	virtual void OpenScreen(void);
	virtual void CloseScreen(void);

	virtual int kbinit(void);
	virtual int ScanKeyboard(void);

protected:
	int _ScanKeyboard(void);
};

class C_SguiDriverEac5000C:public C_SguiDriverBase
{
public:
	virtual int DriverID(void);
	virtual unsigned short LcdWidth(void);
	virtual unsigned short LcdHeight(void);
	virtual unsigned short LcdBpp(void);

	virtual int fbinit(void);
	virtual int fbfresh(void);

	virtual void backlight(int on);
	virtual void SetContrast(int contrast);
	virtual void SetBrightness(int brightness);
	virtual void SetGrayness(int grayness);
	virtual void SetDirection(int direction);
	virtual void OpenScreen(void);
	virtual void CloseScreen(void);

	virtual int kbinit(void);
	virtual int ScanKeyboard(void);

protected:
	int _ScanKeyboard(void);
};

//
//
//
/** @brief ����SGUI ���� */
enum
{
	SGUI_DRIVER_ID_DEFAULT,
	SGUI_DRIVER_ID_EAC_5000D,
	SGUI_DRIVER_ID_EAC_5000C,
	SGUI_DRIVER_ID_CQ_6100,

	SGUI_DRIVER_ID_USER_BEGIN, // �û�ID��ʼ

	SGUI_DRIVER_ID_MAX
};
C_SguiDriverBase *SGUI_SetDriver(C_SguiDriverBase *pSguiDriver);
int SGUI_SetDriver(int SguiDriverID);

/** @brief Lcd���� */
char *SGUI_LcdBuffer(void);
unsigned short SGUI_LcdWidth(void);
unsigned short SGUI_LcdHeight(void);
unsigned short SGUI_LcdBpp(void);
unsigned long SGUI_LcdLine(void);
unsigned long SGUI_LcdSize(void);

/** @brief LCD��ʼ��*/
int SGUI_LcdInit(void);
/** @brief ���̳�ʼ��*/
int SGUI_KeyboardInit(void);
/** @brief ��Ļˢ��*/
void SGUI_flushScreen(S_WORD x, S_WORD y, S_WORD w, S_WORD h);
/** @brief ��Ļˢ��ʹ��*/
bool SGUI_FlushScreenEn(bool enable);
/** @brief �ر���Ļ*/
void SGUI_CloseScreen(void);
/** @brief ����Ļ*/
void SGUI_OpenScreen(void);
/** @brief ����ƿ���*/
void SGUI_backlight(int on);
/** @brief ���öԱȶ�
 *  @param[in] contrast 0-99
 */
void SGUI_contrast(int contrast);
/** @brief ���ñ��������
 *  @param[in] brightness 0-99
 */
void SGUI_brightness(int brightness);
/** @brief ���ûҶ�
 *  @param[in] grayness 0-99
 */
void SGUI_grayness(int grayness);
/** @brief ������Ļ����
 *  @param[in] direction 0-3
 */
void SGUI_direction(int direction);
/** @brief ��Դ·����ʼ��*/
char *SGUI_ResourcePathInit(const char *pResourcePath);
/** @brief ��ѯ��Ϣ*/
S_GUIMSG SGUI_PollMsg(void);
/** @brief ˯�ߺ���*/
int SGUI_SleepMS(int ms);


#endif //__SGUIBASE_H__



