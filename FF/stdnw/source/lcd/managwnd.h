/** @file
 *  @brief �ն˹�����ά�����ڶ���
 *  @author  yezhengqiu
 *  @date 2012/02/21
 *  @version 0.1
 */
#ifndef __LCD_MANAGE_WND_H__
#define __LCD_MANAGE_WND_H__
#include "uiwnd.h"
#include "strings.h"

//�ն˰汾	����ʾ�ն���Ӳ���汾��
class C_ShowVersionWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//�����ն�	���������ն�
class C_RebootWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//�澯��ѯ	���ṩ��������100���澯��Ϣ��ѯ�������澯���롢������ʾ�¼����ݡ�����ʱ��ȣ�
class C_QueryAlarmWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//��ѯ����ͳ����Ϣ
class C_QueryGatherInfo:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

class C_PrintAlarmWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
#ifdef TEST_DEBUG
	//��Ӳ����ú���
	int TestMakeAlarmData(S_WORD MP, S_DWORD DI, strings &strReocrds,bool TypeFlag=false);
#endif
protected:
	int PrintAlarm(int Index);
	int ReadAlarmData(S_DWORD type);

protected:
	strings m_strReocrds;
	strings m_strTexts;
};
#ifdef TEST_DEBUG
//��Ӳ�����ش�������ʱ��������
//���Ը澯������
#define FILL_INVALID_DATA			0XFF
class C_TestMakeData
{
public:
	C_TestMakeData(){};
	~C_TestMakeData();
	//������
	static inline void MakeData(string &strData,const S_DWORD ENdata,const S_BYTE DataLen){strData.append((char*)&ENdata,DataLen);}
	//3���ѹ
	static inline void Make3Vol(string &strData)
	{
		MakeData(strData,0x12345678,2);//A���ѹ
		MakeData(strData,0x12345678,2);//B���ѹ
		MakeData(strData,0x12345678,2);//C���ѹ
		MakeData(strData,0x12345678,2);//AB�ߵ�ѹ
		MakeData(strData,0x12345678,2);//BC�ߵ�ѹ
		MakeData(strData,0x12345678,2);//CA�ߵ�ѹ
		MakeData(strData,0x12345678,2);//A/AB�������ѹ
		MakeData(strData,0x12345678,2);//B�������ѹ
		MakeData(strData,0x12345678,2);//C/CA�������ѹ
	}
	//3�����
	static inline void Make3Cur(string &strData)
	{
		MakeData(strData,0x12345678,3);//A�����
		MakeData(strData,0x12345678,3);//B�����
		MakeData(strData,0x12345678,3);//C�����
		MakeData(strData,0x12345678,3);//A���������
		MakeData(strData,0x12345678,3);//B���������
		MakeData(strData,0x12345678,3);//C���������
	}
	//����
	static inline void MakePower(string &strData,const S_BYTE DataLen)
	{
		MakeData(strData,0x12345678,DataLen);//��
		MakeData(strData,0x12345678,DataLen);//A
		MakeData(strData,0x12345678,DataLen);//B
		MakeData(strData,0x12345678,DataLen);//C
	}
	//TypeFlag:true���������false:��0xff
	static void FillTimeData(string &strData,int type);
	static int FillARD1Data(string &strData,bool TypeFlag=false);
	static int FillARD2Data(string &strData,bool TypeFlag=false);
	static int FillARD3Data(string &strData,bool TypeFlag=false);
	static int FillARD4Data(string &strData,bool TypeFlag=false);
	static int FillARD5Data(string &strData,bool TypeFlag=false);
	static int FillARD6Data(string &strData,bool TypeFlag=false);
	static int FillARD7Data(string &strData,bool TypeFlag=false);
	static int FillARD8Data(string &strData,bool TypeFlag=false);
	static int FillARD9Data(string &strData,bool TypeFlag=false);
	static int FillARD1018Data(string &strData,bool TypeFlag=false);
	static int FillARD11Data(string &strData,bool TypeFlag=false);
	static int FillARD12Data(string &strData,bool TypeFlag=false);
	static int FillARD13Data(string &strData,bool TypeFlag=false);
	static int FillARD14Data(string &strData,bool TypeFlag=false);
	static int FillARD151617Data(string &strData,bool TypeFlag=false);
	static int FillARD19Data(string &strData,bool TypeFlag=false);
	static int FillARD20Data(string &strData,bool TypeFlag=false);
	static int FillARD21Data(string &strData,bool TypeFlag=false);
	static int FillARD22Data(string &strData,bool TypeFlag=false);
};

//��Ӳ�����ش�����ʱ��������
#endif
//Һ������	�ɵ����ն�Һ����ʾ�Աȶȡ��������ȵ�
class C_ScreenParamViewWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//�ź�ǿ�Ⱥ͵�ص���	�ɲ�ѯ�����������źų�ǿ��ָʾ�͵�ǰ��ص������ն˵��ָ����ͣ���ϱ���أ�
class C_SignalAndBatteryWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

//����ʵʱ״̬	���ṩ�ն��������������ʵʱ״̬��Ϣ
class C_WirelessWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	const char *ModemType(int type);
	const char *GetPppBottomInfo(void);
};

//Һ������	�ɵ����ն�Һ����ʾ�Աȶȡ��������ȵ�
class C_ScreenParamSetWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};
	
#endif //__LCD_MANAGE_WND_H__


