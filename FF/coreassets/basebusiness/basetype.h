/**
 *  @file
 *  @brief �����û����Ͷ���
 *  @author 
 *  @date 2011-07-28
 *  @version v1.0
 *  @note
 *  ����ҵ������������Ͷ���
 */
#ifndef __BASEB_BASETYPE_H__
#define __BASEB_BASETYPE_H__
#include "basedef.h"
#include "bitsetex.h"
#include "timeop.h"
#include "channel.h"

/** @brief ��̨ͨѶ������� */
enum S_HttxSN
{
	MP_HTTX_1 = 0,
	MP_HTTX_2,
	MP_HTTX_3,
	MP_HTTX_4,
	MP_HTTX_5,
	MP_HTTX_6,
	MP_HTTX_7,
	MP_HTTX_8,
	MP_HTTX_9,
	MP_HTTX_10,
	MP_HTTX_11,
	MP_HTTX_12,

	MP_HTTX_MAX
};
/** @brief ���ɼ�������� */
enum S_DbcjSN
{
	MP_DBCJ_1 = 0,
	MP_DBCJ_2,
	MP_DBCJ_3,
	MP_DBCJ_4,
	MP_DBCJ_5,
	MP_DBCJ_6,
	MP_DBCJ_7,
	MP_DBCJ_8,
	MP_DBCJ_9,
	MP_DBCJ_10,
	MP_DBCJ_11,
	MP_DBCJ_12,

	MP_DBCJ_MAX
};
/** @brief ��̨ͨѶͨ����� */
enum S_ChannelSN
{
	HTTX_CHANNEL_1 = 0,
	HTTX_CHANNEL_2,
	HTTX_CHANNEL_3
};

//
//������λͼ�ṹ
//
#define BASEB_MP_COUNT_MAX	1201
struct S_MPBS:public BitSetEx<BASEB_MP_COUNT_MAX> //MP����λ����
{
};
struct S_MBS:public BitSetEx<BASEB_MP_COUNT_MAX> //Meter����λ����
{
};
struct S_MTBS
{
	S_MPBS m_AllMPBS;///< ��Ч�ĵ��,���е��
	S_MPBS m_485MPBS;///< ֱ����,�ն�ͨ��485��
	S_MPBS m_ZBMPBS;///< ��ֱ����,�ն�ͨ���ز������߳�
//	S_MBS m_Vip485MPBS;///< �ص㻧ֱ����
	S_MPBS m_VipZBMPBS;///< �ص㻧��ֱ����
};

//
//��վ������ӳ��ͨ�üĴ���
//
#define MSDI_COMM_REG_MAX	32
struct S_DIBS:public BitSetEx<MSDI_COMM_REG_MAX>
{
};


/** @brief ���ݴ洢����¼�� */
struct S_StoreMax
{
	S_WORD m_DayData; //����������¼��
	S_WORD m_MonData; //����������¼��
	S_WORD m_VipData; //�ص㻧��������¼��
	S_WORD m_CbrData; //��������������¼��
};

//
//DA����
//
class C_DA
{
public:
	C_DA(INT16U DA):m_DA1((INT8U*)&DA, 1),m_DA2(DA>>8){}
	~C_DA(){}
	
	/** @brief MPͳ��
	 *  @return 0 �ն˲�����
	 *  @return 1-8 ��ƵȲ�����
	 *  @return 0xFF ���в�����
	 */
	INT8U GetMpCount(void)const;
	void ResetMP(int pos){m_DA1.ClearBit(pos);}
	/** @brief MP���� */
	bool TestMP(int pos)const;
	INT16U GetMP(int pos)const;
	int PrintMPBS(S_MPBS &MPBS)const;

	INT8U GetDA1(void)const{const INT8U *p = m_DA1.GetData(); return *p;}
	INT8U GetDA2(void)const{return m_DA2;}
	INT16U GetDA(void)const{const INT8U *p = m_DA1.GetData(); return (m_DA2<<8)|*p;}

private:
	C_BITSET<1> m_DA1;
	INT8U m_DA2;
};

//
//DT ����
//
class C_DT
{
public:
	C_DT(S_WORD DT):m_DT1((S_BYTE*)&DT, 1),m_DT2(DT>>8){}
	~C_DT(){}
	
	/** @brief ���ܵ�ͳ��
	 *  @return 0 �޹��ܵ�
	 *  @return 1-8 ���ܵ���λ
	 */
	S_BYTE GetFnCount(void){return m_DT1.CountBit();}
	void ResetFN(int pos){m_DT1.ClearBit(pos);}
	/** @brief ���ܵ���� pos[0-7]*/
	bool TestFN(int pos){return m_DT1.TestBit(pos);}
	S_WORD GetFN(int pos){return m_DT2*8 + (pos+1);}

	S_BYTE GetDT1(void){INT8U *p = m_DT1.GetData(); return *p;}
	S_BYTE GetDT2(void){return m_DT2;}
	S_WORD GetDT(void){INT8U *p = m_DT1.GetData(); return (m_DT2<<8)|*p;}

private:
	C_BITSET<1> m_DT1;
	S_BYTE m_DT2;
};

class C_DAPN
{
public:
	/** @brief ��PNת����DA
	 * @param[in] PN �������
	 * @return ��ЧDA
	 */
	static S_WORD PN2DA(S_WORD PN);
	/** @brief ��DAת����PN
	 * @param[in] DA ��Ϣ��
	 * @return ��ЧPN
	 */
	static S_WORD DA2PN(S_WORD DA);
};

class C_DTFN
{
public:
	/** @brief ��FNת����DT
	 * @param[in] FN ��Ϣ���ܵ�
	 * @return ��ЧDT
	 */
	static S_WORD FN2DT(S_WORD FN);
	/** @brief ��DTת����FN
	 * @param[in] DT ��Ϣ��
	 * @return ��ЧFN
	 */
	static S_WORD DT2FN(S_WORD DT);
};

/** @brief ��ƹ�Լ���ƶ��弯 */
//{{{
#define MRULE_NAME_UNKNOW	"unknow"
#define MRULE_NAME_GB645_1997 "GB645-1997"
#define MRULE_NAME_GB645_2007 "GB645-2007"
#define MRULE_NAME_IEC1107 "IEC1107"

#define MRULE_NAME_ISKRA "ISKRA"
#define MRULE_NAME_JLCY	"ACGather"

//}}}

/** @brief IEC1107��Լ����Ĵ���ID���� */
enum
{
	IEC_MSG_NOTHING,

	IEC_MSG_REQUEST,
	IEC_MSG_RECOGNISE,
	IEC_MSG_CHOISE,
	IEC_MSG_DATA,
	IEC_MSG_COMPLETE,

	IEC_MSG_MAX	
};

/** @brief ���ݱ��Ľṹ */
struct S_DATAGRAM
{
	string m_strProtocol; ///< Э������
	string m_strUserData; ///< Э�鱨��
};

/** @brief �ն˵�ַ�ṹ */
#define TERM_ADDR_LEN_MAX		6
struct S_TERMINAL
{
	string m_strProtocol; ///< Э������
	string m_strAddress; ///< �ն˵�ַ
};
struct S_TERMINALS:public vector<S_TERMINAL>
{
public:
	bool InsertUnique(S_TERMINAL &terminal);
};
bool operator<(const S_TERMINAL &left, const S_TERMINAL &right);

//
//��ƼĴ�������
//
enum
{
	MR_TYPE_UNKNOW,
	MR_TYPE_BCD,
	MR_TYPE_BIN,
	MR_TYPE_BCD_BLOCK,
	MR_TYPE_BCD_TARIFF,
	MR_TYPE_BCD_TIME,
	MR_TYPE_BCD_TIME_BLOCK,
	MR_TYPE_ASCII,
	MR_TYPE_STATUS,
	MR_TYPE_TIME,
	MR_TYPE_TIME_BLOCK,
	MR_TYPE_TIME_TARIFF,
	MR_TYPE_TIME_BIN,
	MR_TYPE_TIME_BIN_BLOCK,
	MR_TYPE_COMB,

	MR_TYPE_OTHER = 0x7F
};
/** @brief ��ƼĴ������� */
struct S_REGISTER
{
	bool m_Enable;///< ��Լ֧��
	S_DWORD m_ID;///< �Ĵ���ID
	bool m_Sign;///< ����λ
	S_BYTE m_Type; ///< ֵ���� MR_TYPE_
	S_BYTE m_Len; ///< ֵ���ȣ��ֽ������䳤��0��ʾ
	S_BYTE m_Format; ///< �������,BCD��Ϊ����С��λ,��0x62

public:
	S_REGISTER(void){memset(this, 0x00, sizeof(*this));}
	const char *TypeName(void)const;
	int FillData(string &strData)const;
};
struct S_REGISTERS:public vector<S_REGISTER>
{
};
bool operator<(const S_REGISTER &left, const S_REGISTER &right);

//
//��ƻ�����Ϣ
//
enum S_ComObj
{
	GATHER_COM_OBJ_NULL,
	GATHER_COM_OBJ_TERM, //�ɱ��ؼĴ����Ķ���
	GATHER_COM_OBJ_ANALOG, //�ɵ�ѹ����ģ�����Ķ���
	GATHER_COM_OBJ_PULSE, //������Ķ���
	GATHER_COM_OBJ_METER, //�ɱ�ƵĶ���

	GATHER_COM_OBJ_MAX
};
struct S_MeterBase
{
	S_WORD m_MP; ///< ������
	int m_RuleID; ///< ��ƹ�Լ
	S_COMSET m_ComSet; ///< ͨѶ����
	string m_strAddress; ///< ��Ƶ�ַ
	string m_strRelay; ///< �ɼ�����ַ

public:
	S_MeterBase(void)
	{
		m_MP = 0x00;
		m_RuleID = 0;
		m_ComSet.m_PortNO = 0;
	}
};

//
//������������ϱ����ݽṹ
//
#define REPORT_DATA_FLAG_METER_SKIP	0x00000001UL
struct S_ReportData
{
	S_WORD m_MP; ///< ��Ʋ�����
	S_WORD m_RuleID; ///< ��ƹ�ԼID
	S_BYTE m_idxReg; ///< �Ĵ�������
	S_BYTE m_Reserve; ///< ����
	S_WORD m_Flag; ///< ��־λ
	string m_strAddress; ///< ��Ƶ�ַ
	strings m_strDatas; ///< �������

public:
	S_ReportData(void){m_MP = 0x00, m_RuleID = 0x00, m_idxReg = 0x00, m_Reserve = 0x00, m_Flag = 0x00;}
};
struct S_ReportDatas:public vector<S_ReportData>
{
};
bool operator<(const S_ReportData &left, const S_ReportData &right);


//
//�ɼ�������Ϣ
//
#define TASKINFO_NAME_MAX	32
enum
{
//ϵͳ��������������
	SYS_TASK_IDLE,
	SYS_TASK_ROUTE,
	SYS_TASK_REAL_TIME,
	SYS_TASK_BROADCAST,
	SYS_TASK_ONE_OFF,
	SYS_TASK_SEEK_METER,

	SYS_TASK_MAX,

//��������
	USER_TASK_ROUTE_EX = SYS_TASK_MAX,

//�û�����
	USER_TASK_VIP,
	USER_TASK_DAY,
	USER_TASK_MON,
	USER_TASK_FIX_DAY,
	USER_TASK_POLLING,
	USER_TASK_TERM_VIP,
	USER_TASK_TERM_DAY,
	USER_TASK_TERM_MON,
	USER_TASK_STAT_DAY,
	USER_TASK_STAT_MON,
	USER_TASK_POLL_ALARM,
	USER_TASK_DAY_ALARM,

	USER_TASK_TYPE_ID_MAX
};
//�û�����
#define USER_TASK_MAX		160
#define GATHER_TASK_MAX	(USER_TASK_MAX+SYS_TASK_MAX)
//ϵͳ����ID��ʼ
#define SYS_TASK_ID_BEGIN	0xFFFFFF00
//���ȼ�����
#define TASK_PRIORITY_USER_BEGIN	10
#define TASK_PRIORITY_USER_END		200
#define TASK_PRIORITY_IDLE	1
#define TASK_PRIORITY_REALTIME	(TASK_PRIORITY_USER_END+1)
#define TASK_PRIORITY_ONEOFF		(TASK_PRIORITY_USER_END+2)
//������
#define TASK_NAME_IDLE	"IdleTask"
#define TASK_NAME_ROUTE	"RouteTask"
#define TASK_NAME_REALTIME	"RealtimeTask"
#define TASK_NAME_BROADCAST	"BroadcastTask"
#define TASK_NAME_ONEOFF		"OneoffTask"
#define TASK_NAME_SEEKMETER		"SeekMeterTask"
//����������
#define TASK_NAME_ROUTE_EX	"RouteExTask" // ��Ҫ���ڲ��Ի����´����е��ز���Ҫ�ֳ������,�綫������ģʽ�µĳ���
//�û�������
#define TASK_NAME_VIP	"VipTask"
#define TASK_NAME_DAY	"DayTask"
#define TASK_NAME_MON	"MonTask"
#define TASK_NAME_FIX_DAY	"FixDayTask"
#define TASK_NAME_POLLING	"PollingTask" ///< ֱ���������ѯ����
#define TASK_NAME_TERM_VIP	"TermVipTask"  ///< �����ն�������GPRS�źţ�����
#define TASK_NAME_TERM_DAY	"TermDayTask"  ///< �����ն�������GPRS�źţ�����
#define TASK_NAME_TERM_MON	"TermMonTask"  ///< �����ն�������GPRS�źţ�����
#define TASK_NAME_STAT_DAY	"StatDayTask" ///< �����ն�ͳ�����ݣ����������F25-30, F33-38
#define TASK_NAME_STAT_MON	"StatMonTask" ///< �����ն�ͳ�����ݣ����������F25-30, F33-38
#define TASK_NAME_POLL_ALARM			"PollAlmTask" ///< ��ѯ��澯���ݲɼ�����
#define TASK_NAME_DAY_ALARM		"DayAlmTask" ///< �ո澯���ݲɼ�����
//ϵͳ�����ʶ
#define MSDI_TASK_IDLE			(SYS_TASK_ID_BEGIN + SYS_TASK_IDLE)
#define MSDI_TASK_ROUTE		(SYS_TASK_ID_BEGIN + SYS_TASK_ROUTE)
#define MSDI_TASK_REALTIME		(SYS_TASK_ID_BEGIN + SYS_TASK_REAL_TIME)
#define MSDI_TASK_BROADCAST	(SYS_TASK_ID_BEGIN + SYS_TASK_BROADCAST)
#define MSDI_TASK_ONEOFF		(SYS_TASK_ID_BEGIN + SYS_TASK_ONE_OFF)
#define MSDI_TASK_SEEKMETER	(SYS_TASK_ID_BEGIN + SYS_TASK_SEEK_METER)
//���������ʶ
#define MSDI_TASK_ROUTE_EX		(SYS_TASK_ID_BEGIN + SYS_TASK_MAX + 1)
// �ֶ���������ʱ���ʶ
#define TASK_MANUAL_START_TIME	0x00000001UL

struct S_TaskInfo
{
	bool m_Enable;
	char m_TypeName[TASKINFO_NAME_MAX];///< ����������
	S_DWORD m_MSDI;///< ��վ����ID������֮����ҪΨһ
	S_BYTE m_Priority;///< ���ȼ���Խ���ʾ���ȼ�Խ��
	S_BYTE m_RetryTimes;///< һ�����������Դ���:0��ʾ���޴���
	S_BYTE m_MaxStartTimes;///< �����������:0��ʾ���޴���
	S_BYTE m_PeriodVal;///< ����ֵ:0��ʾ��������
	S_TIME_UNIT m_TimeUnit;///< ���ڵ�λ
	S_TIME m_StandardTime;///< ��׼��ʼʱ��
	S_DWORD m_ExecTimeS;///< ���������ִ��ʱ�䣬��λ��,0��ʾ����ʱ��
	S_DWORD m_ReportPeriodS;///< ���������ϱ����ڣ�����ϱ�����

	S_MPBS m_MPBS; ///< �������λ
	S_REGISTERS m_CommRegs;///< ͨ�����ݼĴ���
	S_DIBS m_DisableRegs; ///< �Ĵ�������λ

	string m_strPrivateData;///< ˽������

public:
	S_TaskInfo(void);
};

enum
{
	TASK_STATUS_BLOCK = 0,
	TASK_STATUS_READY,
	TASK_STATUS_RUNNING,

	TASK_STATUS_MAX
};
enum
{
	TASK_GATHER_STATUS_INIT = 0,
	TASK_GATHER_STATUS_DIRECT,
	TASK_GATHER_STATUS_REPORT,
	TASK_GATHER_STATUS_INDIRECT,

	TASK_GATHER_STATUS_FINISH
};
enum
{
	TASK_SEEK_STATUS_INIT = 0,
	TASK_SEEK_STATUS_485I,
	TASK_SEEK_STATUS_485II,
	TASK_SEEK_STATUS_ZBWX,

	TASK_SEEK_STATUS_FINISH
};
struct S_TaskCtrlStatus
{
	int m_GatherError; ///< ���βɼ�������Ϣ[METER_READ_ERROR_]
	S_TIME m_timeStart;///< ����������ʼ��ʵ��ʱ��
	S_TIME m_timeStdStart; ///< ����������ʼ�ı�׼ʱ��
	S_TIME m_timeReportStart; ///< �����ϱ�ģʽ��ʼ��ʵ��ʱ��
	S_BYTE m_TaskStatus; ///< ����״̬[TASK_STATUS_]

	S_BYTE m_CurrTimes;///< ��ǰִ�д���,������ִ�����޸�
	S_BYTE m_GatherStatus; ///< �ɼ�����״̬[TASK_GATHER_STATUS_]
	S_BYTE m_idxReg; ///< �Ĵ�������
	S_WORD m_mpMeter; ///< ��ǰ���ڲɵĲ�����

	strings m_strDatas; ///< ��ͨ��ʽ�ɼ�����
	S_ReportDatas m_ReportDatas; ///< �ϱ���ʽ�ɼ�����

public:
	S_TaskCtrlStatus(void);
};

//
//�ɼ�����־û���Ϣ
//
struct S_TaskFreezeStatus
{
	bool m_Enable;
	char m_TypeName[TASKINFO_NAME_MAX];///< ����������
	S_DWORD m_MSDI; ///< ��վ���ݱ�ʶ��Ψһ��
	S_TIME m_timeLastRaise; ///< ���һ������ʱ��(��׼ʱ��)
	S_TIME m_timeStart;///< ��������ʼʱ��(ʵ��ʱ��)
	S_TIME m_timeFinish;///< �����������ʱ��(ʵ��ʱ��)
	S_TIME m_timeStdStart; ///< m_SuccessfulMBS��Чʱ��(��׼ʱ��)
	S_MPBS m_SuccessfulMPBS; ///< ��¼����ɹ��ı��
	S_MTBS *m_pMTBS; ///< �������ָ��

public:
	S_TaskFreezeStatus(void);
	int PrintStatusInfo(string &strInfo);
	bool ClearData(void);
};

struct S_TaskStatus
{
	struct S_TaskFreezeStatus m_Item[GATHER_TASK_MAX];

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
	void BeforeFind(S_MTBS *pMTBS);
	S_TaskFreezeStatus *Find(const S_TaskInfo &TaskInfo);
	void AfterFind(void);
	S_TaskFreezeStatus *GetTaskFreezeStatus(S_DWORD MSDI);
	int PrintStatusInfo(string &strInfo);
	bool ClearData(void);
};

//
//�ϱ������Ϣ
//
struct S_ReportMeterInfo
{
	S_WORD m_SN; ///< �ڵ���·�ɱ��е����
	S_BYTE m_Phase; ///< �ڵ����ڵ���λ[0, 'A', 'B', 'C']
	S_BYTE SignalQuality; ///< �ź�Ʒ��[0-15]
	string m_strAddress; ///< ��Ƶ�ַ
	string m_strMeterRule; ///< ��ƹ�Լ
	string m_strRelay; ///< �ɼ���

public:
	S_ReportMeterInfo(void){m_SN=0x00, m_Phase=0x00, SignalQuality=0x0F;}
};

struct S_ReportMeterInfos:public vector<S_ReportMeterInfo>
{
};

#endif//__BASEB_BASETYPE_H__



