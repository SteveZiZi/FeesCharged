//---------------------------------------------------------------------------------
// Copyright  2009-2010
// E-mail:   @163.com
// Project:			SGUI(Simple GUI)
// File:			sguiobj.h
// Description:		SGUI for Power user elect energy acquire system;unsupport multi_thread
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2010-01-18  Create                  Kingsea
//---------------------------------------------------------------------------------
#ifndef __SGUI_OBJ_H__
#define __SGUI_OBJ_H__

class C_SGUIOBJ
{
public:
	enum OBJ_T {
	OBJ_T_UNKNOW,
	OBJ_T_APP,
	OBJ_T_WNDB,
	OBJ_T_WND,
	OBJ_T_DC,
	OBJ_T_FONT,
	OBJ_T_PEN,
	OBJ_T_BMP,
	OBJ_T_CTRL,
	OBJ_T_CARET,

	OBJ_T_MAX
	};
public:
	C_SGUIOBJ(OBJ_T t){m_ObjT = t; m_cntReference = -1;} // 初始为用户管理对象
	virtual ~C_SGUIOBJ(){}

	OBJ_T GetObjType(void){return m_ObjT;}
	int GetReferenceCount(void){return m_cntReference;}
	void AutoRelease(void){m_cntReference = 0;}
	void Retain(void){if (m_cntReference >= 0) m_cntReference++;}
	void Release(void){if (m_cntReference > 0) m_cntReference--;}
	
protected:
	OBJ_T m_ObjT;
	int m_cntReference; // -1表示用户自己管理对象，0-*表示引用计数，为0系统会在适当时候自己释放
};

#endif //__SGUI_OBJ_H__

