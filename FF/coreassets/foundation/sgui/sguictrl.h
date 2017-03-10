//---------------------------------------------------------------------------------
// Copyright  2009-2010
// E-mail:   @163.com
// Project:			SGUI(Simple GUI)
// File:			sguictrl.h
// Description:		SGUI for Power user elect energy acquire system;unsupport multi_thread
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2010-06-06  Create                  Kingsea
//---------------------------------------------------------------------------------
#ifndef __SGUI_CTRL_H__
#define __SGUI_CTRL_H__
#include "sguiwnd.h"
#include "strings.h"

#define NUMEDIT_STYLE_DEC		0x00000000UL
#define NUMEDIT_STYLE_OCT		0x00000100UL
#define NUMEDIT_STYLE_HEX		0x00000200UL

#define GM_CMD_BTN_PUSHED	0x00000000UL
#define GM_CMD_CTRL_UPDATE	0x00000000UL
//
//�ؼ�����
//
class C_GUICTRL:public C_GUIWNDB
{
public:
	C_GUICTRL(void);
	virtual ~C_GUICTRL();

public:
	S_DWORD GetCtrlStyle(void){return m_flag;}
	S_DWORD GetCtrlID(void){return m_ID;}

protected:
	bool AddCtrl2ParentWnd(C_SGUIWND *pWnd);

	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
	virtual int DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	S_DWORD m_ID;
};

//
//���ֱ༭��֧�ְ˽��ơ�ʮ���ơ�ʮ������
//��ӦGM_KEYUP��GVK_UP,GVK_DN,GVK_LEFT,GVK_RIGHT����Ϣ
//�û�����GM_CMDҪ��
//GM_CMD��Ϣ����:wParam=�ؼ�ID,lParam=GM_CMD_CTRL_UPDATE(��ʾ�������б仯)
//
class C_NUMEDIT:public C_GUICTRL
{
public:
	C_NUMEDIT(void);
	virtual ~C_NUMEDIT();

public:
	static C_NUMEDIT *NewCtrl(void);

public:
	bool Create(const S_CHAR *pszCaption, S_DWORD dwStyle, const S_RECT& rect, C_SGUIWND* pParentWnd, S_DWORD nID); 

protected:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
	virtual int DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

	bool IsOctChar(S_CHAR ch){return ch>='0'&&ch<='7';}
	bool IsDecChar(S_CHAR ch){return ch>='0'&&ch<='9';}
	bool IsHexChar(S_CHAR ch){return IsDecChar(ch) || ch>='A'&&ch<='F';}

	S_CHAR IncreaseChar(S_CHAR ch);
	S_CHAR DecreaseChar(S_CHAR ch);

protected:
	static const char m_SideW = 2;
	static const char m_SideH = 1;
};

//
//����ASCII��༭��
//��ӦGM_KEYUP��GVK_UP,GVK_DN,GVK_LEFT,GVK_RIGHT����Ϣ
//�û�����GM_CMDҪ��
//GM_CMD��Ϣ����:wParam=�ؼ�ID,lParam=GM_CMD_CTRL_UPDATE(��ʾ�������б仯)
//
class C_ASCEDIT:public C_GUICTRL
{
public:
	C_ASCEDIT(void);
	virtual ~C_ASCEDIT();

public:
	static C_ASCEDIT *NewCtrl(void);

public:
	bool Create(const S_CHAR *pszCaption, S_DWORD dwStyle, const S_RECT& rect, C_SGUIWND* pParentWnd, S_DWORD nID); 

protected:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
	virtual int DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

	S_CHAR IncreaseChar(S_CHAR ch);
	S_CHAR DecreaseChar(S_CHAR ch);

protected:
	static const char m_SideW = 2;
	static const char m_SideH = 1;
};


struct S_VKBTN
{
	const S_CHAR *m_pName;
	const S_CHAR m_Val;
};
//
//������̿ؼ�
//��ӦGM_KEYUP��GVK_UP,GVK_DN,GVK_LEFT,GVK_RIGHT,GVK_OK����Ϣ
//
class C_VKBOARD:public C_GUICTRL
{
public:
	C_VKBOARD(void);
	virtual ~C_VKBOARD();

public:
	static C_VKBOARD *NewCtrl(void);

public:
	bool Create(S_DWORD dwStyle, const S_RECT& rect, C_SGUIWND* pParentWnd, S_DWORD nID); 

protected:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
	virtual int DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	virtual int GetHighlightRect(S_RECT &rect, const C_SGUIDC *pdc);
	virtual S_WORD GetHighlightChar(void);

protected:
	int m_RowCharCnt;
	int m_RowIdx;
	int m_ColIdx;

protected:
	static const int m_VkBoardW = 139;
	static const int m_VkBoardH = 64;
	static const S_CHAR *m_VkAsciiSet[5];
	static const S_VKBTN m_VkSpecialKey[5];
};

//
//�ı��༭��
//��ӦGM_KEYUP��GVK_LEFT,GVK_RIGHT����Ϣ
//�û�����GM_CMDҪ��
//GM_CMD��Ϣ����:wParam=�ؼ�ID,lParam=GM_CMD_CTRL_UPDATE(��ʾ�������б仯)
//
class C_TEXTEDIT:public C_GUICTRL
{
public:
	C_TEXTEDIT(void);
	virtual ~C_TEXTEDIT();

public:
	static C_TEXTEDIT *NewCtrl(void);

public:
	bool Create(const S_CHAR *pszCaption, S_DWORD dwStyle, const S_RECT& rect, C_SGUIWND* pParentWnd, S_DWORD nID); 

protected:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
	virtual int DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	int m_CaretIdx;
	
protected:
	static const char m_SideW = 2;
	static const char m_SideH = 1;
};

//
//��ť�ؼ�
//��ӦGM_KEYUP��GVK_OK����Ϣ
//�û�����GM_CMDҪ��
//GM_CMD��Ϣ����:wParam=�ؼ�ID,lParam=GM_CMD_BTN_PUSHED(��ʾ��ť������)
//
class C_BUTTON:public C_GUICTRL
{
public:
	C_BUTTON(void);
	virtual ~C_BUTTON();

public:
	static C_BUTTON *NewCtrl(void);

public:
	bool Create(const S_CHAR *pszCaption, S_DWORD dwStyle, const S_RECT& rect, C_SGUIWND* pParentWnd, S_DWORD nID); 

protected:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
	virtual int DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	S_DWORD m_state;
};

//
//ѡ���
//��ӦGM_KEYUP��GVK_UP,GVK_DN,GVK_OK����Ϣ
//ѡ���ı���;���
//�û�����GM_CMDҪ��
//GM_CMD��Ϣ����:wParam=�ؼ�ID,lParam=GM_CMD_CTRL_UPDATE(��ʾ�������б仯)
//lParam Ϊ����ֵΪ��ǰѡ���ȷ�ϵ���
//
class C_SELECTBOX:public C_GUICTRL
{
public:
	C_SELECTBOX(void);
	virtual ~C_SELECTBOX();

public:
	static C_SELECTBOX *NewCtrl(void);

public:
	bool Create(const S_CHAR *pszSelectText, S_DWORD dwStyle, const S_RECT& rect, C_SGUIWND* pParentWnd, S_DWORD nID); 
	/** @brief ����ȱʡ��
	 * @param[in] idx ����ֵ [0-(������-1)]
	 * @return true ���óɹ�; false ����ʧ��
	 */
	bool SetDefaultItem(S_DWORD idx);

protected:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
	virtual int DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

	int DrawFocusDirection(C_SGUIDC *pdc);

protected:
	S_DWORD m_Index;
	strings m_SelectText;
	static const char m_Select_SideW = 2;
	static const char m_Select_SideH = 2;
	static const char m_Direction_High = 16;//�̶��߶�Ϊ16��SYM16X16���Ÿ߶ȣ�
	static const char m_Direction_Width = 16;//�̶����Ϊ16��SYM16X16���Ÿ߶ȣ�
};

#endif //__SGUI_CTRL_H__

