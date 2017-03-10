/** @file
 *  @brief ��̨ͨѶ�������ݶ�д������
 *  @author 
 *  @date 2011/12/22
 *  @version 0.1
 */
#ifndef __NW_CMDDATA_H__
#define __NW_CMDDATA_H__
#include "nwmscmd.h"
#include "nwbtype.h"
#include "timeop.h"
#include "sysbase.h"


//
//����ǰ����
//
class C_CmdRealtime:public C_CmdReceiver //���ɼ���
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����ǰ����(�ɼ���)";}
	bool GetCurZXYG();
protected:
	bool FillData(const S_CmdObj &CmdObj, string &strData);
private:
	string strIpcData;	
};
class C_CmdRealE1800001:public C_CmdReceiver //	NNNN	2	 	*		�ն�״̬�֣��μ�C.5.1
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ն�״̬��";}
};
class C_CmdRealE1800002:public C_CmdReceiver //	NN	1	 	*		���������·��
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�����������·��";}
};
class C_CmdRealE1800003:public C_CmdReceiver //	NN	1	 	*		����ͨ�Ŷ˿�����
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "������ͨ�Ŷ˿�����";}
};
class C_CmdRealE1800004:public C_CmdReceiver //	NNNN	2	 	*		֧������������
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��֧������������";}
};
class C_CmdRealE1800005:public C_CmdReceiver //	NNNN	2	 	*		����������,������Ч�Ĳ���������
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����Ч�Ĳ���������";}
};
class C_CmdRealE1800006:public C_CmdReceiver //	NNNN	2	 	*		�ն���ͨ�û������� 
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ն���ͨ�û�������";}
};
class C_CmdRealE1800007:public C_CmdReceiver //	NNNN	2	 	*		�ն˶๦�ܱ�����
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ն˶๦�ܱ�����";}
};
class C_CmdRealE1800008:public C_CmdReceiver //	NNNN	2	 	*		�ն��ص㻧���ܱ�����
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ն��ص㻧���ܱ�����";}
};
class C_CmdRealE1800009:public C_CmdReceiver //	NNNN,MMMM,YYMMDDhhmmss1,YYMMDDhhmmss2,	*		�ն˳���״̬��Ϣ
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ն˳���״̬��Ϣ";}
protected:
	int GetTermGatherStatus(S_WORD &MeterTotal, S_WORD &MeterSuccess, S_TIME &timeGatherBegin, S_TIME &timeGatherEnd);
};
class C_CmdRealE1800010:public C_CmdReceiver //	0SNN.NN	3	 	*		����������
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "������������";}
};
class C_CmdRealE1800011:public C_CmdReceiver //	NNNN	2	kB	*		(��ǰ)��ͨ������
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��(��ǰ)��ͨ������";}
};
class C_CmdRealE1800013:public C_CmdReceiver //	NNNNNN	3	kB	*		(��ǰ)��ͨ������
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��(��ǰ)��ͨ������";}
};
class C_CmdRealE1800015:public C_CmdReceiver //	NN	1		*		�ź�ǿ��(0~31)
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ź�ǿ��(0~31)";}
};
class C_CmdRealE1800016:public C_CmdReceiver //	NNNN	2		*		(��ǰ)���ն˸�λ����
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��(��ǰ)���ն˸�λ����";}
};
class C_CmdRealE1800017:public C_CmdReceiver //	NNNN	2		*		(����)���ն˸�λ����
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��(����)���ն˸�λ����";}
};
class C_CmdRealE1800018:public C_CmdReceiver //	NNNN	2		*		�ز�ģ�����汾��
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ز�ģ�����汾��";}
};

class C_CmdRealE1800019:public C_CmdReceiver //	NNNN	2		*		��������������
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����������������";}
};

class C_CmdRealE1800020:public C_CmdReceiver //	NN.....NN	2*20		*		�ص㻧���ܱ�������(ffffΪ��Ч)
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ص㻧���ܱ�������";}
};

class C_AFNCMD_REALTIME:public C_AFNCMD
{
protected:
	virtual C_CmdReceiver *GetCmdReceiver(INT32U DI);
};

//
//����ʷ����
//
class C_CmdHistory:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����ʷ����";}
protected:
	S_TIME GetNextTime(S_TIME timeData, S_CHAR Density);
	S_TIME DateTrim(S_TIME timeData, S_CHAR Density);
	int MakeReadCmdObj(S_WORD DA, S_MPBS &MPBS);
};

class C_AFNCMD_HISTORY:public C_AFNCMD
{
protected:
	virtual C_CmdReceiver *GetCmdReceiver(INT32U DI);
};

//����������
class C_CmdTask:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����������";}
	//�����ϱ�����
	int GetReportData(int TaskID, const S_NormalTaskInfo *pInfo, S_HistoryTask *pHistoryTask, strings &strCmdInfos);

protected:
	S_TIME GetNextSampleTime(const S_NormalTaskInfo *pInfo, S_TIME timeData, S_DataDensity Density);
	S_TIME GetNextSampleTime(const S_NormalTaskInfo *pInfo, S_TIME timeData);
	S_TIME TrimSampleTime(const S_NormalTaskInfo *pInfo, S_TIME timeData);
	S_TIME TrimSampleTime(S_DataDensity Density, S_TIME timeData);
	S_TIME GetDataSampleTime(const S_NormalTaskInfo *pInfo, S_TIME timeData, S_DataDensity Density, S_DWORD DI);
	S_TIME GetDataSampleTime(const S_NormalTaskInfo *pInfo, S_TIME timeData, S_DWORD DI);
	int GetNormalTaskData(const S_NormalTaskInfo *pInfo, S_TIME timeData, strings &strDatas, S_MPBS &mpbsFailed, S_DataDensity Density);
	int GetNormalTaskData_UserDefine(const S_NormalTaskInfo *pInfo, S_TIME timeData, strings &strDatas, S_MPBS &mpbsFailed, S_DataDensity Density);//��ʾ��������ʽ��֯����
	int GetNormalTaskData_TaskDefine(const S_NormalTaskInfo *pInfo, S_TIME timeData, strings &strDatas, S_MPBS &mpbsFailed, S_DataDensity Density);//��ʾ������������ݸ�ʽ��֯����
	bool TrimCmdInfo(S_BYTE type, S_WORD DaCnt, S_WORD DtCnt, const strings &strSrcs, strings &strDsts);
	bool TrimCmdInfo(const strings &strSrcs, strings &strDsts);
	bool MakeMeasurePointBitset(const S_NormalTaskInfo *pInfo, S_MPBS &MPBS);
};
class C_AFNCMD_TASK:public C_AFNCMD
{
protected:
	virtual C_CmdReceiver *GetCmdReceiver(INT32U DI);
};

//���澯����
class C_CmdAlarm:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���澯����";}
};
class C_AFNCMD_ALARM:public C_AFNCMD
{
protected:
	virtual C_CmdReceiver *GetCmdReceiver(INT32U DI);
};

//���¼�����
class C_CmdEvent:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���¼�����";}
};
class C_AFNCMD_EVENT:public C_AFNCMD
{
protected:
	virtual C_CmdReceiver *GetCmdReceiver(INT32U DI);
};

//�ļ�����
class C_AFNCMD_FTP:public C_AFNCMD
{
protected:
	virtual C_CmdReceiver *GetCmdReceiver(INT32U DI);
};

//����(���ն˴���)
class C_CmdE0000930:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ն���ѯ����";}
};
class C_CmdE0000931:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ն˴����������";}
};
class C_CmdE0000933:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�����ն�̨���ܵ���";}
};
class C_AFNCMD_FORWARD:public C_AFNCMD
{
public:
	virtual int Execute(const S_CmdObjs &CmdObjs, strings &strResponseInfos);
protected:
	virtual C_CmdReceiver *GetCmdReceiver(INT32U DI);
};

//����(���ն˴���)
class C_CmdMsE0000930:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ն���ѯ����ظ�";}
};
class C_CmdMsE0000932:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ն˴����������ظ�";}
};
class C_CmdMsE0000933:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�ն�̨���ܵ����ظ�";}
};
class C_AFNCMD_FORWARD_MS:public C_AFNCMD
{
public:
	virtual int Execute(const S_CmdObjs &CmdObjs, strings &strResponseInfos);
protected:
	virtual C_CmdReceiver *GetCmdReceiver(INT32U DI);
};

//�м�ת��
class C_CmdRoute:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�м�ת��";}
};
class C_AFNCMD_ROUTE:public C_AFNCMD
{
protected:
	virtual C_CmdReceiver *GetCmdReceiver(INT32U DI);
};


#endif //__NW_CMDDATA_H__

