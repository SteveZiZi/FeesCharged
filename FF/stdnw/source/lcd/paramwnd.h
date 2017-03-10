/** @file
 *  @brief ������ʾ���ڶ���
 *  @author  yezhengqiu
 *  @date 2012/02/21
 *  @version 0.1
 */
#ifndef __LCD_PARAMWND_H__
#define __LCD_PARAMWND_H__
#include "uiwnd.h"



//ͨ��ͨ������
class C_CommunMenuWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

struct S_WndMeter
{
	char m_Status[8];
	char m_Function[12];
	char m_Address[16];
	char m_Rule[16];
	char m_Port[8];
	char m_Relay[16];
};
//���������
class C_MeterParamViewWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	bool UpdateMeterInfo(S_WndMeter *pData, const S_MeterInfo *pMeterInfo);	
};
class C_MeterDetailViewWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	S_WORD PrintMeterParamDetail(S_WORD MP);

protected:
	char m_BottomInfo[64];
	strings m_strListData;
};
//�ն�ʱ������
class C_TerminalTimeViewWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//������������
class C_PasswordWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//�ն˱������
class C_RtuaViewWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//����������ѯ������
class C_ForwardParamViewWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//�ն���ֵ������ѯ������
class C_ThresholdViewWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

// ���Բ˵�
class C_DebugMenuWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//�ն˲���ͨ������
class C_TestComViewWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	S_DWORD m_ChannelType;
};

//�ն˲���ͨ������
class C_TestComSetWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
private:
	void SaveInfos(S_DWORD type);
};

// ����ͨ��ģ��ģʽ
class C_DnModuleViewWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

// ����ͨ��ģ��ģʽ
class C_DnModuleSetWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//ͨ����ʾ����
class C_ChannelViewWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	/** @brief ȡͨ����Ϣ
	 *  @return 0 ��̫��; 2 GPRS/CDMA
	 */
	int GetHttxChannelInfo(int ChannelIdx, S_DWORD &IP, S_WORD &port);
};

//PPP������ʾ����
class C_PppViewWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//�����������ô���
class C_LocalNetViewWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//ͨѶ������ʾ ����������Ȳ���
class C_ChannelsParamViewWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//ͨѶ�������� ����������Ȳ���
class C_ChannelsParamSetWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//ͨ�����ô���
class C_ChannelSetWnd:public C_ChannelViewWnd
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	bool CreateNumEdit(int num, S_RECT &NumRect, S_DWORD ID);
	bool CheckParamFormat(int &ChannelType, S_DWORD &IP, S_DWORD &port);
	bool SaveHttxChannelInfo(int ChannelIdx, int ChannelType, S_DWORD IP, S_WORD port);
};

//�����������ô���
class C_LocalNetSetWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	bool CreateNumEdit(int num, S_RECT &NumRect, S_DWORD ID);
	bool CheckParamFormat(S_DWORD &LocalIP, S_DWORD &NetMask, S_DWORD &GateWay);
};

struct S_MeterSet
{
	S_WORD m_MP;
	S_MpInfo m_MpInfo;
	S_MeterInfo m_MeterInfo;
};

//����������
class C_MeterParamSetWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	bool CreateNumEdit(const char *pBuffer, S_DWORD style, S_RECT &NumRect, S_DWORD ID);
	bool CreateSelectBox(const char *pBuffer, S_DWORD Idx, S_RECT &NumRect, S_DWORD ID);
	bool CheckParamFormat(S_MeterInfo &MeterInfo, S_MpInfo &MpInfo);
	S_DWORD GetSelectBoxIdx(const char *pBuffer, S_DWORD ID);
};
class C_MeterDetailSetWnd:public C_MeterParamSetWnd
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	S_DWORD GetBaudNO(S_DWORD baud);
	S_DWORD GetBaud(S_DWORD idx);
	bool CheckDetailFormat(S_MeterInfo &MeterInfo, S_MpInfo &MpInfo);	
};

//�ն�ʱ�����ô���
class C_TermTimeSetWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	bool CreateNumEdit(int num, S_RECT &NumRect, S_DWORD ID);
	bool CheckTimeFormat(S_DATE &date);
};

//�ն˵�ַ���ô���
class C_RtuaSetWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//�ı��༭����
class C_TextEditWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	int SavePppInfo(int ItemIndex, string &strInfo);
	int GetPppInfo(int ItemIndex, string &strInfo, int EditSize);
};


#endif//__PARAMWND_H__

