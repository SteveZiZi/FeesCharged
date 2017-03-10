/** @file
 *  @brief �û���ϵͳ�����ඨ��
 *  @author 
 *  @date 2011/12/06
 *  @version 0.1
 */
#ifndef __NW_SYS_BASE_H__
#define __NW_SYS_BASE_H__
#include "basedef.h"
#include "syscfg.h"
#include "mptype.h"
#include "channel.h"
#include "bitsetex.h"
#include "osrelate.h"
#include "basetype.h"
#include "htrace.h"
#include "datafile.h"
#include "nwbtype.h"
#include "nwmscmd.h"

#define CONS_CHANNEL_NAME_MAX_LEN		16

enum ClassDataType
{
	CLASSDATA_1 = 1, ///< ����һ������
	CLASSDATA_2, ///< ������������
	CLASSDATA_3, ///< ������������
	CLASSDATA_4, ///< ��ѯ��������
	CLASSDATA_5, ///< �����϶���/������
	CLASSDATA_6, ///< ����������/������
	CLASSDATA_7, ///< ����������/������
	CLASSDATA_8, ///< ����������/������
	CLASSDATA_9, ///< ����������/������

	CLASSDATA_MAX
};

/*����һ���������������ʶ*/
#define TASK_SIGN_PREFIX_CLASS1	(CLASSDATA_1<<24)
#define TASK_SIGN_PREFIX_CLASS2	(CLASSDATA_2<<24)
#define TASK_SIGN_PREFIX_CLASS3	(CLASSDATA_3<<24)
#define TASK_SIGN_PREFIX_CLASS4	(CLASSDATA_4<<24)

#define TASK_SIGN_PREFIX_CLASS5	(CLASSDATA_5<<24)
#define TASK_SIGN_PREFIX_CLASS6	(CLASSDATA_6<<24)
#define TASK_SIGN_PREFIX_CLASS7	(CLASSDATA_7<<24)
#define TASK_SIGN_PREFIX_CLASS8	(CLASSDATA_8<<24)
#define TASK_SIGN_PREFIX_CLASS9	(CLASSDATA_9<<24)

/* ����λ���Ͷ���*/
struct SAlarmBitSet:public BitSetEx<CONS_MAX_ALARM_NUMBERS> //�澯����λ����
{
};
struct S_VCOMBS:public BitSetEx<SYSTEM_SERIAL_PORT_MAX> // ���⴮��
{
};

/** @class S_SYSCFG
 *  @brief ϵͳ���ýṹ��
 */
struct S_SYSCFG
{
	/** �ն˵�ַ
	 */
	S_RTUA m_RTUA; ///< 

	/** ��½��Ϣ
	 */
	char m_SysUser[20];
	char m_SysPassword[20];

	/** ������Ϣ
	 */
	bool m_InitIRDA;
	char m_ZoneName[64];
	char m_CfgVer[12]; ///< �ն˲������ð汾��
	char m_CfgDate[20]; ///< �ն˲������ð汾��������
	S_BYTE m_TermType; ///< �ն�����[CONS_TERM_TYPE_]
	INT16U m_HeartBeatRetryTimes;///< ��������  ��
	INT16U m_HeartBeatIdleTimeS; ///<  ��������ʱ��
	INT16U m_ReportAgainIntervalM;///< ��ǰ���񲹱�ʱ����
	INT16U m_BatteryStandTimeM;///< ��ع���ʱ�䳤�ȣ������е硣Ĭ��Ϊ3����
	INT16U m_TermStandTimeS; ///< �����޵������£��ն��е�ʱ��

	/** �˿�ӳ���
	 */
	S_BYTE m_PortMap[SYSTEM_SERIAL_PORT_MAX]; ///< 485�˿�ӳ���
	S_VCOMBS m_ProgramKeyPort; ///< ��̼���Ӧ�Ķ˿ں�

	/** ����ͨ��ģ��������Ϣ
	 */
	int m_DrRouteMode; ///< ����·��ģʽ3=3��;4=4��

public:
	void Init(void);
	/** @brief �����ն˵�ַ */
	void SetRTUA(const char *pRTUA);
	/** @brief ��ȡ�ն˵�ַ */
	char *GetRTUA(char *pRTUA);
};

/** @class S_SYSRUN
 *  @brief ϵͳ������Ϣ�ṹ��
 */
struct S_SYSRUN
{
	/** �汾��Ϣ
	 */
	char m_ReleaseDate[32]; ///< ��������
	char m_HWVersion[32]; ///< Ӳ���汾��
	char m_SWVersion[32]; ///< ����汾��
	char m_MPVersion[12]; ///< ������汾��

	/** �ն˵�����Ϣ
	 */
	S_TIME m_timePowerCut; ///< �ն˵���ʱ��
	S_WORD m_cntPowerCut; ///< �ն��е����,ͳ���е�����Ǹ�����������

	/** ����ģ����Ϣ */
	char m_RouterName[32]; ///< ·��ģ������
	char m_RouterType[8]; ///< XC #XC,DR,YXD,RB,SR,LME,DX
	char m_ChipCode[8];
	char m_ChipVerDate[16];
	char m_ChipVersion[8]; ///< V1 #ģ��汾��,00Ϊ��Ч

	/** MODEM��Ϣ */
	char m_ModemFactory[16]; ///< unknow
	char m_ModemModel[16]; ///< M590
	S_DWORD m_SimIP;

public:
	void Init(void);
};

/** @class S_CHANNELS
 *  @brief ͨ�����ģ�� 
 *  @note Ŀǰ��������ͨ��(12)������ͨ��(3)
 */
template <int ChannlCnt>
struct S_CHANNELS
{
	struct S_CHANNEL m_Item[ChannlCnt];

	void Init(void){memset(this, 0x00, sizeof(*this));}
	/** @brief ��ͨ��������ͨ�� */
	struct S_CHANNEL *FindChannel(const char *ChannlName)
	{
		for (int i = 0; i < ChannlCnt; ++i)
		{
			if (m_Item[i].m_Enable == true && strcasecmp((char *)m_Item[i].m_Name, ChannlName) == 0)
				return &m_Item[i];
		}
		return NULL;
	}

	/** @brief �����⴮�ںŲ���ͨ�� */
	struct S_CHANNEL *FindChannel(INT8U VcomNo, bool fVCom)
	{
		for (int i = 0; i < ChannlCnt; ++i)
		{
			if (m_Item[i].m_Enable == true && m_Item[i].m_Type == CHANNEL_TYPE_COM
				&& m_Item[i].m_PortSet.m_ComSet.m_PortNO == VcomNo)
			{
				return &m_Item[i];
			}
		}
		return NULL;
	}

	/** @brief ��ͨ����������ͨ�� */
	struct S_CHANNEL *FindChannel(int ChannlIdx)
	{
		if (ChannlIdx >= ChannlCnt || ChannlIdx < 0)
			return NULL;
		return &m_Item[ChannlIdx];
	}
};

struct S_UpChannel:public S_CHANNELS<CHANNEL_UP_COUNT>
{
};

struct S_DnChannel:public S_CHANNELS<CHANNEL_DN_COUNT>
{
};

/** @brief ��ƹ�Լ���� */
#define NW_MRULE_NAME_DLT645_1997 "DL/T645-1997"
#define NW_MRULE_NAME_DLT645_2007 "DL/T645-2007"
#define NW_MRULE_NAME_GUANGDONG_97 "GUANGDONG_97"
#define NW_MRULE_NAME_GUANGDONG_07 "GUANGDONG_07"
#define NW_MRULE_NAME_WEI_SHENG "WEI_SHENG"
#define NW_MRULE_NAME_LGR_B "LGR_B"
#define NW_MRULE_NAME_LGR_D "LGR_D"
#define NW_MRULE_NAME_EMDI "EMDI"
#define NW_MRULE_NAME_ABB "ABB"
#define NW_MRULE_NAME_ACTARIS "ACTARIS"
#define NW_MRULE_NAME_EMAIL "EMAIL"
#define NW_MRULE_NAME_ISKRA "ISKRA"

class C_NwMRuleName
{
public:
	/** @brief ת��ƹ�Լ���� */
	static const char *MakeMeterRuleName(int ID);
	/** @brief ת��ƹ�ԼID */
	static int MakeMeterRuleID(const char *pRuleName);
	/** @brief ת��ƹ�Լ���� */
	static int MakeMeterRuleIndex(int ID);
	/** @brief ת��ƹ�Լ���� */
	static int MakeMeterRuleIndex(const char *pRuleName);
	/** @brief ���л���ƹ�Լ���� */
	static S_WORD SerializeMeterRuleName(string &strRuleName, char fill, char delimit);
};

/** @brief ������Ͷ��� */
enum
{
	MP_PROPERTY_UNKNOW = 0,
	MP_PROPERTY_RS485 = 1,///< 485��
	MP_PROPERTY_ANALOG,///< ģ����
	MP_PROPERTY_PULSE,///< ������
	MP_PROPERTY_SUM,///< ����ֵ
	MP_PROPERTY_AC ///< ��������
};
enum
{
	METER_FUN_UNKNOW = 0,
	METER_FUN_SINGLE_PHASE = 1, ///< �����
	METER_FUN_MULTI_FUN ///< �๦�ܱ�
};
enum
{
	METER_TYPE_USER = 0, ///< �û���
	METER_TYPE_MAIN ///< �ܱ�
};
enum
{
	METER_USER_NORMAL = 0, ///< ��ͨ�û�
	METER_USER_VIP ///< �ص��û�
};
enum
{
	METER_POWER_CUT_NO = 0, ///< ������բ
	METER_POWER_CUT_YES ///< ����բ
};
typedef enum
{
	METER_RULE_GB645_1997 = 0, //00H��DL/T645-1997��Լ��
	METER_RULE_GB645_2007 = 1, //01H��DL/T645-2007��Լ��
	METER_RULE_GUANGDONG_97 = 2, //02H���㶫97����Լ��
	METER_RULE_GUANGDONG_07 = 3, //03H���㶫07����Լ��
	METER_RULE_WEISHENG = 4, //04H����ʤ�Զ����Լ��
	METER_RULE_LGR_B = 5, //05H��������B���Լ��
	METER_RULE_LGR_D = 6, //06H��������D���Լ��
	METER_RULE_EMDI = 7, //07H��EDMI���Լ��
	METER_RULE_ABB = 8, //08H��ABB�����Լ��
	METER_RULE_ACTARIS = 9, //09H�����������Լ��
	METER_RULE_EMAIL = 10, //0AH��EMAIL���Լ��
//	METER_RULE_OTHER = 11, //0BH��������Լ
	METER_RULE_ISKRA = 11, //0BH ��˹����

	METER_RULE_AC_GATHER = 100, //��������װ��ͨ��Э��

	METER_RULE_MAX //MAXֵ��Ӧ����ϵͳ֧�ֵı�ƹ�Լ��
}T_emMETER_RULE;

#define RELAY_ADDRESS_LEN_MAX	6
#define METER_ADDRESS_LEN_MAX	6
#define METER_PASSWORD_LEN_MAX	8
#define METER_USERNAME_LEN_MAX	8
struct S_MeterInfo
{
	bool m_Enable; ///< �������״̬ 0����Ч��1����Ч
	S_WORD m_MeasureNo; ///< �����������[ע��������ṹ�������һ��]
	S_BYTE  m_RuleID; ///< ͨ��Э������ 00H��DL/T645-1997��Լ��01H��DL/T645-2007��Լ��02H��������Լ
	S_BYTE  m_FunType; ///< ���ܱ����ͣ�01��������ӱ�02���๦�ܱ�
	bool   m_MainFlag; ///< �ܷ����ͣ�0���û���1���ܱ�
	bool   m_VipFlag; ///< �ص��û����ԣ�0����ͨ�û���1���ص��û�
	bool   m_PowerCut; ///< ��բ���ܣ�0��������բ��1������բ��
	S_BYTE  m_TariffCnt; ///< ���ܷ��ʸ���
	S_WORD m_CT; ///< CT�仯
	S_WORD m_PT; ///< PT�仯
	S_COMSET m_PortSet; ///< �˿ںŲ�����˿ںţ�00H~1EH��ʾ485��˿ںţ�1FH��ʾ�ز�ͨ����20H��ʾ���ߣ�30H��ʾ������

	S_BYTE m_WireType;///< ���߷�ʽ  01���࣬03:�������ߣ�04:��������
	S_WORD m_RatedVoltage;///< ���ѹBCD
	S_DWORD m_RatedCur;///< �����BCD
	S_WORD m_RatedContent;///< ��װ����BCD
	S_WORD m_VTContent;///< ��ѹ������BCD

	char   m_Address[METER_ADDRESS_LEN_MAX]; ///< ͨ�ŵ�ַ
	char   m_RelayAddr[RELAY_ADDRESS_LEN_MAX]; ///< �������Ӧ�ɼ��ն˵�ַ��ȫΪ00H��ȫΪFFH���ʾ�����ɼ���
	char   m_Password[METER_PASSWORD_LEN_MAX];
	char   m_UserName[METER_USERNAME_LEN_MAX+2];

public:
	S_MeterInfo(void){ClearMeterInfo();}
	bool Existed(void)const{return m_Enable == true;}
	void ClearMeterInfo(void){memset(this,0x00,sizeof(*this));}
	int ExtractBaseInfo(S_MeterBase &MeterBaseInfo)const;
	bool ValidRelayAddress(void)const;

	bool SetRelayAddress(const char *pstrAddr);
	char* GetRelayAddress(char *pstrAddr)const;
	bool SetMeterAddress(const char *pstrAddr);
	char* GetMeterAddress(char *pstrAddr)const;
	bool SetMeterPassword(const char *pstrPwd);
	char *GetMeterPassword(char *pstrPwd)const;
	const S_COMSET GetPortSet(void)const{return m_PortSet;}

	int PrintMeterInfo(string &strInfo)const;

private:
	bool SetAddress(const char *pstrAddr, char *pBcdAddr);
	char* GetAddress(const char *pBcdAddr, char *pstrAddr)const;
};

struct S_MeterInfos
{
	struct S_MeterInfo m_Item[CONS_METER_COUNT_MAX];

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
	int PrintMeterInfo(S_WORD MinMP, S_WORD MaxMP, string &strInfo)const;
};

//
//��ѯ��������
//
struct S_PollingData
{
	bool m_Enable;
	S_TIME m_timePollMeter;

	char m_Date_C010[3]; ///< ����
	char m_Time_C011[3]; ///< ʱ��
	char m_Zxyg_9010[5]; ///< �����й�
	char m_Zxwg_9110[4]; ///< �����޹�
	char m_ActivePowerSum_B630[3]; ///< �����й�������
	char m_ActivePowerA_B631[3]; ///< A���й�����  
	char m_ActivePowerB_B632[3]; ///< B���й�����  
	char m_ActivePowerC_B633[3]; ///< C���й�����  
	char m_PwFactorSum_B650[2]; ///< ����������
	char m_PwFactorA_B651[2]; ///< ��������A��
	char m_PwFactorB_B652[2]; ///< ��������B��
	char m_PwFactorC_B653[2]; ///< ��������C��
	char m_ReactivePowerSum_B640[3]; ///< �����޹�������
	char m_ReactivePowerA_B641[3]; ///< A���޹�����  
	char m_ReactivePowerB_B642[3]; ///< B���޹�����  
	char m_ReactivePowerC_B643[3]; ///< C���޹�����  

	char m_DemandSum_A010[3]; ///< ��������
	char m_DemandSum_B010[4]; ///< ��������ʱ�� 
	char m_DemandA_60A1[3]; ///< A��������  
	char m_DemandA_60B1[4]; ///< A��������ʱ��  
	char m_DemandB_60A2[3]; ///< B��������  
	char m_DemandB_60B2[4]; ///< B��������ʱ��  
	char m_DemandC_60A3[3]; ///< C��������  
	char m_DemandC_60B3[4]; ///< C��������ʱ��  

	char m_IA_B621[3]; ///< A�����
	char m_IB_B622[3]; ///< B�����
	char m_IC_B623[3]; ///< C�����
	char m_I0_6010[3]; ///< �������
	char m_UA_B611[2]; ///< A���ѹ
	char m_UB_B612[2]; ///< B���ѹ
	char m_UC_B613[2]; ///< B���ѹ
	char m_PowerSZ_6020[3]; ///< ���ڹ���
};

struct S_PollingDatas
{
	struct S_PollingData m_Item[BASEB_MP_COUNT_MAX];

public:
	void Init(void){memset(this, 0xEE, sizeof(*this));}
	/** @return >0:�ɹ�; <=0:ʧ��*/
	int Load(S_WORD MP, S_PollingData &PollData);
	/** @return >=0:�ɹ�; <0:ʧ��*/
	int Save(S_TIME timeNow, S_WORD MP, const S_REGISTERS &Registers, const strings &strDatas);

private:
	int _Save(S_PollingData &PollData, const S_REGISTERS &Registers, const strings &strDatas);
};

//********************************************************/
//*               �������������Ϣ                       */
//********************************************************/
struct S_GatherPortInfo
{
	bool m_Enable; ///< ��Ч�Ա�־
	S_TIME m_timeGatherStd; ///< �����׼ʱ��
	S_BYTE m_GatherPeriodUnit; ///< �������ڵ�λ 0-3����ʱ����
	S_BYTE m_GatherPeriod; ///< ��������
};
struct S_GatherPortInfos
{
	struct S_GatherPortInfo m_ItemInfo[CONS_TASK_COUNT_MAX];	// ����˿���Ϣ,

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
};

#define DBCJ_TASK_INFO_DADT_CNT_MAX		256
struct S_GatherTaskInfo
{
	bool m_Enable; ///< ��Ч�Ա�־
	S_WORD m_RecordMax; ///< �洢����	Ϊ0��ʾ�����洢�����洢
	S_BYTE m_MeterType; ///< �������	BIN	1	01��������ӱ�02���๦�ܱ�03���������ͣ�  ff����Ч����	//ע����Թ��䡢ר�估��վ�նˣ����ֶι̶����Ϊff
	S_BYTE m_VipFlag; ///< �ص��û�����	BIN	1	0����ͨ�û���1���ص��û�ff����Ч����//ע����Թ��䡢ר�估��վ�նˣ����ֶι̶����Ϊff
	S_WORD m_DaCnt; ///< ��Ϣ���ʶ����	BIN	1	��Ϣ���ʶ1~n������ע�� ��Լ����������ֶι̶����Ϊ0
	S_WORD m_DtCnt; ///< ���ݱ�ʶ��������	BIN	1	���ݱ�ʶ����1~m������
	S_WORD m_DA[DBCJ_TASK_INFO_DADT_CNT_MAX]; ///< DA��ʽ�����ֽ�DA2�����ֽ�DA1
	S_DWORD m_DT[DBCJ_TASK_INFO_DADT_CNT_MAX]; ///< DT��ʽ����->��,DI3 DI2 DI1 DI0

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
};

struct S_GatherTaskInfos
{
	struct S_GatherTaskInfo m_ItemInfo[CONS_TASK_COUNT_MAX];	//���������Ϣ

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
};

//********************************************************/
//*               ��ͨ���������Ϣ                       */
//********************************************************/
#define NORMAL_TASK_MAX	256
#define NORMAL_TASK_ID_MIN	0xE0000301UL
#define NORMAL_TASK_ID_MAX	0xE00003FDUL
#define REPORT_TASK_INFO_DADT_CNT_MAX	256
struct S_NormalTaskInfo
{
	bool m_Enable; ///< =false
	bool m_Active; ///< =true
	S_TIME m_ReportStdTime;///< =2009-10-14 #��-��-��00:00:00 #ʱ���֣���
	S_BYTE m_ReportPeriodUnit; ///< =hour #minute,hour,day,month
	S_BYTE m_ReportPeriodVal;///<   =1  ��ʱ�ϱ�����
	S_BYTE m_DataFormat; ///< =0 #0��ʾ��������ʽ��֯���ݣ�1��ʾ������������ݸ�ʽ��֯����
	S_TIME m_SampleStdTime; ///< 2009-10-14 #��-��-��00:00:00 #ʱ���֣���
	S_BYTE m_SamplePeriodUnit; ///< =hour #minute,hour,day,month   ��ʱ�������ڵ�λ
	S_BYTE m_SamplePeriodVal; ///< =1  ��ʱ��������
	S_BYTE m_ExtractRate; ///< =1 #���մ˱��ʳ�ȡ��������
	S_WORD m_MaxTimes;  ///< =0 #0��ʾ��Զִ�У���0��ʾ����ִ�д���
	S_WORD m_DaCnt; ///< ��Ϣ���ʶ����	BIN	1	��Ϣ���ʶ1~n������ע�� ��Լ����������ֶι̶����Ϊ0
	S_WORD m_DtCnt; ///< ���ݱ�ʶ��������	BIN	1	���ݱ�ʶ����1~m������
	S_WORD m_DA[REPORT_TASK_INFO_DADT_CNT_MAX]; ///< DA��ʽ�����ֽ�DA2�����ֽ�DA1
	S_DWORD m_DT[REPORT_TASK_INFO_DADT_CNT_MAX]; ///< DT��ʽ����->��,DI3 DI2 DI1 DI0

public:
	bool HistoryDayTask(void)const{return (m_SamplePeriodUnit == 2 && m_ReportPeriodUnit == 2);}
};

struct S_HistoryTask
{
	S_TIME m_timeReportBegin; ///< ��ʷ������ȡ����ʼʱ��
	S_TIME m_timeReportEnd; ///< ��ʷ������ȡ������ʱ��
	S_TIME m_timeLastReport; ///< �������һ������ʱ��
	S_MPBS   m_MPBS;///<��Ϣ���¼,Ŀǰ�������ϱ�ʧ�ܵı��
};
#define TASK_REPORT_HISTORY_DAY		7
struct S_HistoryTasks
{
	struct S_HistoryTask m_Item[TASK_REPORT_HISTORY_DAY];
};
struct S_NormalTaskStatus
{
	S_TIME m_timeLastReport; ///< ���һ���ϱ�ʱ��
	struct S_HistoryTasks m_HistoryTasks;
};

struct S_NormalTaskInfos
{
	struct S_NormalTaskInfo m_ItemInfo[NORMAL_TASK_MAX];
	struct S_NormalTaskStatus m_ItemStatus[NORMAL_TASK_MAX];

	void Init(void){memset(this, 0x00, sizeof(*this));}
	void InitTaskInfo(void){memset(m_ItemInfo, 0x00, sizeof(m_ItemInfo));}
};

//********************************************************/
//*               �м����������Ϣ                       */
//********************************************************/
#define FORWARD_TASK_MAX	256
#define FORWARD_TASK_ID_MIN	0xE0000401UL
#define FORWARD_TASK_ID_MAX	0xE00004FDUL
#define FORWARD_CMD_LEN_MAX	256
struct S_ForwardTaskInfo
{
	bool m_Enable;
	bool m_Active;
	S_TIME m_ReportStdTime; ///< =2009-10-14 #��-��-��0:0:0 #ʱ:��:��
	S_BYTE m_ReportPeriodUnit; ///< =hour #minute,hour,day,month
	S_BYTE m_ReportPeriodVal;
	S_TIME m_SampleStdTime; ///< =2009-10-14 #��-��-��0:0:0 #ʱ:��:��
	S_BYTE m_SamplePeriodUnit; ///<=hour #minute,hour,day,month
	S_BYTE m_SamplePeriodVal;
	S_BYTE m_ExtractRate; ///< =1 #���մ˱��ʳ�ȡ��������
	S_BYTE m_ForwardType; ///< =0 #00��ͨ�м̣�01��բ���02��բ���03����Ͷ�����04����������
	S_WORD m_MaxTimes; ///<=0 #0��ʾ��Զִ�У���0��ʾ����ִ�д���
	S_COMSET m_PortSet; ///< �˿ںŲ�����˿ںţ�00H~1EH��ʾ485��˿ںţ�1FH��ʾ�ز�ͨ����20H��ʾ���ߣ�30H��ʾ������
	S_BYTE m_TimeoutS; ///< =30
	S_BYTE m_ForwardCmdLen;
	S_CHAR m_ForwardCmd[FORWARD_CMD_LEN_MAX];
};

struct S_ForwardTaskStatus
{
	S_TIME m_timeLastReport; ///< ���һ���ϱ�ʱ��
};

struct S_ForwardTaskInfos
{
	struct S_ForwardTaskInfo m_ItemInfo[FORWARD_TASK_MAX];
	struct S_ForwardTaskStatus m_ItemStatus[FORWARD_TASK_MAX];

	void Init(void){memset(this, 0x00, sizeof(*this));}
	void InitTaskInfo(void){memset(m_ItemInfo, 0x00, sizeof(m_ItemInfo));}
};

struct S_ForwardData
{
	S_BYTE m_Datalen;
	S_CHAR m_ForwardData[FORWARD_CMD_LEN_MAX];
};
struct S_ForwardTaskDatas
{
	struct S_ForwardData m_ForwardData[FORWARD_TASK_MAX];

	void Init(void){memset(this, 0x00, sizeof(*this));}
};

#define FORWARD_CHANNL_SLAVE		0
#define FORWARD_CHANNL_GATHER	1
#define FORWARD_CHANNL_MAX		2
struct S_ForwardInfo
{
	bool m_Enable; ///< ����ʹ��
	S_DWORD m_SlaveAddr[NW_FORWARD_ADDR_MAX]; ///< �������ն˵�ַ1~4��ĳ��ַ����FFFFFFFʱ��ʾ�����ڣ�
	S_DWORD m_MasterAddr; ///< ���ն˵�ַ������FFFFFFFFʱ��ʾ�����ڣ�
	S_DWORD m_GatherAddr; ///< �����ն˵�ַ
	S_BYTE m_WaitSlaveResponseS; ///< ���ն˵ȴ����ն�Ӧ��ĳ�ʱ�ж�ʱ�䣬ȱʡֵ20��
	S_BYTE m_WaitMasterCtrlS; ///< ���ն˵ȴ����ն˼�����������ĳ�ʱ�ж�ʱ�䣬ȱʡֵ20��
	S_WORD m_CT; ///< ����̨���ն�CT����
	S_WORD m_PT; ///< ����̨���ն�PT����
	S_WORD m_PollIntervalS; ///< ��ѯ����
	char m_ChannlName[FORWARD_CHANNL_MAX][CONS_CHANNEL_NAME_MAX_LEN]; ///< #����ͨ��,����ͨ��

	void Init(void)
	{
		memset(this, 0xFF, sizeof(*this));
		m_WaitSlaveResponseS = 20, m_WaitMasterCtrlS = 20;
		m_CT = 1, m_PT = 1;
		m_PollIntervalS = 300;
	}

	void SetRTUA(const char *pName, const char *pRTUA);
	char *GetRTUA(const char *pName, char *pRTUA);
	S_DWORD GetForwardedAddr(int i);
	bool AnySlaveAddress(void);
};

struct S_PowerLimit
{
	S_BYTE m_OverLoadM;///< �� �жϹ����ɡ����������ɡ�����ͬ�����õ硢������ƽ��ȡ��޹�Ƿ�������޹���������ʱ��ȱʡ5����
	S_BYTE m_OverLoadEndM;///< �� �жϹ����ɡ����������ɡ�����ͬ�����õ硢������ƽ��ȡ��޹�Ƿ�������޹��������ָ���ʱ��ȱʡ5����
	S_BYTE m_MissingPhaseM;///< �� �ж϶��ࡢʧѹʱ�䣬ȱʡֵ15����
	S_BYTE m_MissingPhaseEndM;///< �� �ж϶��ࡢʧѹ�ָ�ʱ�䣬ȱʡֵ15����
	S_BYTE m_ReversedPhaseM;///< �� �жϵ�ѹ������ʱ�䣬ȱʡֵ15����
	S_BYTE m_ReversedPhaseEndM;///< �� �жϵ�ѹ������ָ�ʱ�䣬ȱʡֵ15����
	S_BYTE m_ReversedPolarityM;///< �� �жϵ���������ʱ�䣬ȱʡֵ15����
	S_BYTE m_ReversedPolarityEndM;///< �� �жϵ��������Իָ�ʱ�䣬ȱʡֵ15����
	S_BYTE m_MeterTimeDiffM;///< �� ���ܱ�ʱ�ӳ�����ֵ��ȱʡֵ1����
	S_BYTE m_MeterFly;///< ���ܱ������ֵ��ȱʡΪ10��
	S_WORD m_MeterStop;///< NN.NN (BCD) kWh	���ܱ�ͣ����ֵ������ʾ��ֹͣ����ʱ�����й����ʼ���Ӧ�ߵ���ֵ��Խ��ֵ����ܱ�ͣ�ߣ�ȱʡΪ0.1kWh
	S_DWORD m_MeterArrearage;///< ���Ƿ��
	S_DWORD m_MeterLastFee;///< NNNN.NN (BCD) ʣ�������ֵ��ȱʡΪ20.00 Ԫ
	void Init(void)
	{
		m_OverLoadM = m_OverLoadEndM = 5;
		m_MissingPhaseM = m_MissingPhaseEndM = 15;
		m_ReversedPhaseM = m_ReversedPhaseEndM = 15;
		m_ReversedPolarityM = m_ReversedPolarityEndM = 15;
		m_MeterTimeDiffM = 2, m_MeterFly = 10, m_MeterStop = 0x10;
		m_MeterLastFee=20;
	}
};

struct S_IpConfig
{
	INT8U m_TermIP[4];
	INT8U m_NetMask[4];
	INT8U m_GateWay[4];
	INT8U m_MAC[32];
	INT8U m_IP_eth1[4];
	INT8U m_NetMask_eth1[4];
	INT8U m_GateWay_eth1[4];
	INT8U m_MAC_eth1[32];
	
	INT8U m_ProxyIP[4];
	INT8U m_ProxyType;
	INT8U m_ProxyLink;
	INT16U m_ProxyPort;
	INT8U m_UserName[24];
	INT8U m_Password[24];
	INT8U m_IpMethod; ///< ��������IP��ַ��ȡ��ʽ 0���ֶ�����IP��ַ 1���Զ�����IP��ַ

	void Init(void){memset(this, 0x00, sizeof(*this));}
};

//
// �ն�����
//
enum
{
	RESET_METHOD_NO,
	RESET_METHOD_HW,

	RESET_METHOD_DATA,
	RESET_METHOD_PARAM,
	RESET_METHOD_ALL,
	RESET_METHOD_FACT,

	RESET_METHOD_MAX
};
struct S_CommunData
{
	S_DWORD m_CommunicationFlux; ///< ��ǰͨѶ�����ۼ�ֵ
	S_TIME m_timeCommunication; ///< ���ͨѶʱ��
//	S_TIME m_timeMeterCommunication[CONS_METER_COUNT_MAX]; ///< ������ͨѶʱ��
	S_DWORD m_LastDayCommunicationFlux; ///< ���һ���������¼�ۼ�ֵ
	S_DWORD m_LastMonCommunicationFlux; ///< ���һ�µ�������¼�ۼ�ֵ
//	S_TIME m_LastYGPowerChangeTime[CONS_METER_COUNT_MAX]; ///< ������й����ʸı�ʱ��
	S_TIME m_timeFreezeDay;
	S_TIME m_timeFreezeMon;
	S_DWORD m_FreezeDayCummFlux; ///< (����)���ն�����
	S_DWORD m_FreezeMonCummFlux;///< (����)���ն�����
};
struct S_ResetData
{
	S_WORD m_DayResetTimes; ///< (��ǰ)���ն˸�λ����
	S_WORD m_MonResetTimes;///< (��ǰ)���ն˸�λ����

	S_TIME m_timeFreezeDay;
	S_TIME m_timeFreezeMon;
	S_WORD m_FreezeDayResetTimes; ///< (����)���ն˸�λ����
	S_WORD m_FreezeMonResetTimes;///< (����)���ն˸�λ����

	S_BYTE m_ResetMethod;///< ��վ����������ʽ[RESET_METHOD_]
};
struct S_PowerSupplyData
{
	S_DWORD m_PowerSupplyM; ///< �ն˹�����ʱ�䣨��λ����)
	S_DWORD m_LastDayPowerSupplyM; ///< ����Ϊֹ������ʱ��
	S_DWORD m_LastMonPowerSupplyM; ///< ����Ϊֹ������ʱ��

	S_TIME m_timeFreezeDay;
	S_TIME m_timeFreezeMon;
	S_WORD m_FreezeDayPowerSupplyM; ///< ���ն���
	S_WORD m_FreezeMonPowerSupplyM; ///< ���¶���
};
struct S_MeterData
{
	S_TIME m_timeMeterCommunication[CONS_METER_COUNT_MAX]; ///< ������ͨѶʱ��
	S_TIME m_LastYGPowerChangeTime[CONS_METER_COUNT_MAX]; ///< ������й����ʸı�ʱ��
};
/* �ɼ���Ϣͳ��*/
struct S_GatherStatInfo
{
	S_TIME m_timeTaskStart; ///< ����ʼʱ��
	S_TIME m_timeTaskFinish; ///< �������ʱ��
	S_MPBS m_mpbsSuccess; ///< �ɹ���ƣ��ɹ���1
	S_MPBS m_mpbsFailure; ///< ʧ�ܱ�ƣ�ʧ����1

public:
	int SetInfo(S_TIME timeTaskStart, S_WORD MP, bool fSuccess);
	int SetTaskFinish(S_TIME timeTaskFinish);
};
struct S_GatherStatInfos
{
	S_GatherStatInfo m_VipInfo; ///< ����ͳ����Ϣ
	S_GatherStatInfo m_DayInfo; ///< ��ͳ����Ϣ
	S_GatherStatInfo m_MonInfo; ///< ��ͳ����Ϣ
	S_WORD m_GatherFailedTimes[CONS_MP_COUNT_MAX]; ///< ��Ʋɼ�ʧ�ܴ���

public:
	int SetInfo(int FreezeType, S_TIME timeTaskStart, S_WORD MP, bool fSuccess);
	int SetTaskFinish(int FreezeType, S_TIME timeTaskFinish);
};
struct S_TermData
{
	struct S_CommunData m_CommunData; ///< ��վͨѶ��������
	struct S_ResetData m_ResetData; ///< ��λ�������
	struct S_PowerSupplyData m_PowerSupplyData; ///< �ն˹���ʱ������
	struct S_MeterData m_MeterData;
	struct S_GatherStatInfos m_GatherStatInfos;
	
	void Init(void){memset(this, 0x00, sizeof(*this));}
};

enum
{
	HTTX1_COMM_STATUS_NOTHING,
	HTTX1_COMM_STATUS_CLIENT_LINK, // �ͻ�������
	HTTX1_COMM_STATUS_LISTEN, // �ն���������
	HTTX1_COMM_STATUS_SERVER_LINK, // ���������

	HTTX1_COMM_STATUS_MAX
};

/** @brief ��վͨѶ״̬
 */
struct S_ConnectStatus
{
	volatile bool m_HttxNeedPPP[MP_HTTX_MAX];
	volatile bool m_Reconnect;
	volatile bool m_PppOnline;
	volatile bool m_Httx1OnPpp; ///< Httx1ͨ��������PPP
	volatile bool m_Httx1Link;
	volatile bool m_Httx1Logon;
	volatile S_BYTE m_Httx1CommStatus; ///< HTTX1_COMM_STATUS_
	volatile S_TIME m_Httx1LogonTime; ///< HTTX1��½ʱ��
	volatile S_TIME m_Httx1CommTime; ///< HTTX1 ͨ��ʱ��

	void Init(void){memset(this, 0x00, sizeof(*this));}
};

/** @brief ϵͳ״̬
 */
struct S_PowerStatus
{
	bool m_fPowerOff; ///< ����״̬
	bool m_fCharge; ///< ���״̬
	S_WORD m_BatteryCap; ///< ��ص���(0-100)

	void Init(void)
	{
		m_fPowerOff = false;
		m_fCharge = false;
		m_BatteryCap = 100;
	}
};


/** @brief ����������*/
#define SUM_ALL_ITEM_MAX		256
struct S_ANALOG
{
	bool m_Enable;
	S_BYTE m_AnalogType; ///< 1 ��ѹ 2 ����  3 ��ѹ���ʱ��
	S_BYTE m_Phase; ///< 1-A��  2-B��  3-C�� 
	S_WORD m_MP; ///< ������
};
struct S_ANALOGS
{
	S_WORD m_AnalogCnt;
	S_ANALOG m_Item[SUM_ALL_ITEM_MAX];

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
};

struct S_PULSE
{
	bool m_Enable;
	S_BYTE m_PulseType; ///< �������� 01 �����й�02 �����й�03 ��������޹���I��04 ���������޹���IV��05���������޹���II��06��������޹���III��
	S_WORD m_MP; ///< �������
	S_DWORD m_PulseConst; ///< ���峣��
};

struct S_PULSES
{
	S_WORD m_PulseCnt;
	S_PULSE m_Item[SUM_ALL_ITEM_MAX];

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
};

struct S_SUM
{
	bool m_Enable;
	S_WORD m_MP; ///< �������������Ĳ������
	S_WORD m_SumMp[8]; ///< �������1~8
	S_BYTE m_SumOperator[8]; ///< 01(��), 81(��),00(��Ч)
	S_DWORD m_DI; ///< ����������NNNN��������DI3DI2DI1DI0���ڲ�����NNNN 1~8��������DI3DI2DI1DI0֮�ͻ��
};

struct S_SUMS
{
	S_WORD m_SumCnt;
	S_SUM m_Item[SUM_ALL_ITEM_MAX];

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
};

struct S_DiffRule
{
	bool m_Enable;
	S_DWORD m_DI; ///< ������
	S_WORD m_MP1; ///< �Ƚϲ������
	S_WORD m_MP2; ///< ���Ƚϲ������
	S_DWORD m_DiffMin; ///< MMMM.MM (���λС��)�����ò���ʱ�������С���Բ�ֵ
	S_WORD m_DiffAlarmLimit; ///< RR.RR(���λС��)����ʱ�����ֵ(%)
	S_WORD m_DiffEndLimit; ///< SS.SS(���λС��)����ʱ����ָ���ֵ(%)
};

struct S_DiffRuleS
{
	S_WORD m_DiffRuleCnt;
	S_DiffRule m_Item[SUM_ALL_ITEM_MAX];

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
};

struct S_AlarmInfo
{
	SAlarmBitSet m_ReportAlarmEven;///< ��Ҫ�ϱ��ĸ澯
	SAlarmBitSet m_AlarmEven;///< �澯�¼�
	SAlarmBitSet m_OnlyEvent;///< �澯�¼�

	void Init(void){m_ReportAlarmEven.reset();m_AlarmEven.reset();m_OnlyEvent.reset();}
	bool NeedRecord(S_DWORD DI);
};

//
/** @brief ���ɹ��������� */
//
/** @brief �������ۼӱ�ʶ*/
struct S_SumFlag
{
	INT16U m_cTN[8];/*��ʾ�����ۼӲ������ */
	C_BITSET<1> m_SumFlag;/*�ۼӱ�־��1��ʾ�ӣ�0��ʾ��s*/
};
/** @brief ʱ����ʼʱ�估���ʺ�E0000D*1~E0000D*8 */
struct S_TimeArray
{
	INT8U m_A1;//
	INT8U m_A0;//
};
struct S_PwrTime
{
	INT8U m_Enable;//�Ƿ�Ͷ���־
	S_TimeArray m_Time;//Сʱ ����
	S_DWORD m_PwrValve;//���ʶ�ֵKW NNNNNN.NN
};
/** @brief ִ��ʱ��E0000D*9 */
struct S_ExeTime
{
	S_TimeArray m_StartDate;//��ʼʱ�� **��**��
	S_TimeArray m_EndDate;//����ʱ�� **��**��
	INT8U m_TI;//Ƶ�ʵ�λ
	BitSetEx<32> m_N;//����
};
/** @brief ��ʱ�޵����ز��� */
struct S_TPBCtrlParam
{
	INT8U m_Enable;/* E0000C70��ʱ�޵����Ч��ʶ*/
	INT8S m_TPBCoefficient;/*E0000C70��ʱ�޵�ָ�� */
	INT16U m_MinuteCount;/*E0000C70 ��ʱ�޵��ʱ�䣬��λ����*/
	INT8U m_WindowCount;/* E0000C71��ʱ�޵�ļ��㻬��ʱ��*/
};
/** @brief ʱ�ο���ز��� *///zhgx
struct S_TimeCtrlParam
{
	INT8U m_SDMaxNum;/*E0000D00��E0000D10��E0000D20��E0000D30ʱ����*/
	S_PwrTime m_PwrTime[8];// E0000D01~E0000D08��E0000D11~E0000D18��E0000D21~E0000D28��E0000D31~E0000D38
	S_ExeTime m_ExeTime;//	 E0000D09��E0000D19��E0000D29��E0000D39
};
/** @brief ���ݿ���ز��� */
struct S_FactoryCtrlParam
{
	INT8U m_Enable;/*E0000E10���ݿ��Ƿ�Ͷ��*/
	
	S_LONG m_CtrlLimitVar;/*E0000E11���ݿض�ֵ*/
	INT8U m_Sign;/*E0000E11���ݿض�ֵ ����λ*/
	S_TimeArray m_StartTime;//E0000E11 ��ʼʱ�� **ʱ**��
	INT16U m_ContinueTime;//E0000E11 ����ʱ�� ��λ����
	C_BITSET<1> m_CtrlDate;//E0000E11 ÿ���޵��գ�D1~D7��ʾ����һ~�����գ�D0=0	
};
/** @brief �¶ȵ���������ز��� */
struct S_MonthEnergyCtrlParam
{
	INT8U m_Enable;/*E0000C61�¶ȵ��������Ƿ���Ч*/
	C_BITSET<1> m_TurnEnable;/*E0000C64�µ���������բ�ִ���Ч��־*/	
	INT8S m_EnergyCoefficient; /*E0000C6C�µ�����ֵ����ϵ��*/
	INT32U m_MonthEnergyLimit;	/*E0000E00�µ������ƣ��µ����޶һ��ֵ��*/
	S_DWORD m_TurnEnergy;		/*E0000E01�µ������ƣ��ִε�����һ��ֵ�� */
};

/** @brief ������������ز��� */
struct S_BuyEnergyCtrlParam
{
	INT8U m_Enable;/*E0000C62�����������Ƿ���Ч*/
	C_BITSET<1> m_TurnEnable;/*E0000C65������������բ�ִ���Ч��־*/
	
	INT32S m_NowBuyEnergy;	/*E0000E02���������ƣ����ι�����*/
	INT8U m_NowBuyDLSign; /*E0000E02ʣ����� ����λ��0��+��1��-*/
	INT8U m_NowBuyDLNo; /*E0000E02���ι��絥���*/
	
	INT32S m_NowLeftEnergy;	/*E0000E03���������ƣ�ʣ�����*/
	INT8U m_NowLeftDLSign; /*E0000E03ʣ����� ����λ��0��+��1��-*/
};

/** @brief ���ػ������Ʋ���*/
struct S_FKCtrlBaseInfo
{
	C_BITSET<2> m_SwitchType;/*E0000C00~E0000C0F���ؽӵ�����*/
	C_BITSET<2> m_SwitchInState;/*E0000C10~E0000C1F���뿪�ؽӵ�״̬(ֻ��)*/
	//C_BITSET<2> m_SwitchOurState;/*E0000C20~E0000C2F������ؽӵ�״̬(ֻд)*/
	C_BITSET<2> m_SwitchTurnTab;/*E0000C30~E0000C3F��������ִ�*/
	
	unsigned short m_SwitchDelayTime;/*E0000C40ң����բʱ��*/
	bool m_BDFlag; /* E0000C50����״̬,01����״̬��00�Ǳ���״̬*/
	INT32U m_TZOverLoadTimeSec;/* E0000C66�����趨ֵ��բ�Ĺ����ɳ���ʱ��,��λSec*/
	INT32U m_TZUnderLoadTimeSec; /*E0000C67�����趨ֵ��բ�ָ��ĸ��ɻָ�����ʱ��,��λSec*/
	INT16U m_TZAlarmDelaySec;/*E0000C68��բ�㾯��ʱ,��λSec*/
	S_DWORD m_MinPower;		/*E0000C6D��͸��� ��λKW*/
	
	void Init(){memset(this,0x00,sizeof(S_FKCtrlBaseInfo));}
};
/** @brief ������ز���*/
struct S_PWRCtrlInfo
{
	INT8U m_Enable;/*E0000C60���ʿ����Ƿ���Ч*/
	C_BITSET<1> m_TurnEnable;/*E0000C63���ʿ��Ƹ��ִ���բ�Ƿ���Ч*/
	INT8S m_PwrCoefficient;/*E0000C6B���ʶ�ֵ����ϵ��*/
	S_SumFlag m_struPwrSum;/*E0000C69���ʿ����ù����ۼӱ�־*/

	S_TPBCtrlParam m_TPBCtrlParam;/*��ʱ�޵����ز��� */
	S_TimeCtrlParam m_TimeCtrlParam[4];/*ʱ�ο���ز���*/
	S_FactoryCtrlParam m_FactoryCtrlParam;/*���ݿ���ز���*/
	void Init(){memset(this,0x00,sizeof(S_PWRCtrlInfo));}
};
/** @brief ������ز���*/
struct S_DLCtrlInfo
{
	S_SumFlag m_struEnergySum;/*E0000C6A���������õ����ӱ�־*/

	S_MonthEnergyCtrlParam m_MonthEnergyCtrlParam;/*�¶ȵ���������ز��� */
	S_BuyEnergyCtrlParam m_BuyEnergyCtrlParam;/*������������ز��� */

	void Init(){memset(this,0x00,sizeof(S_DLCtrlInfo));}	
};


//
//process info
//
#define HTTX_CHANNL_MAX		3
struct S_HttxInfo
{
	bool m_ReportEnable;
	bool m_CompressEnable;
	S_BYTE m_MsgConfirmCode; ///<  = 0 #0��ʾ��������Ϣ��֤,1-255:��Ϣ��֤������
	S_BYTE m_HeartBeatPeriodM; ///< =30 #0��ʾ��Ҫ����
	S_BYTE m_ResendTimes; ///< =3 #�ط�����
	S_WORD m_WaitResponseTimeoutS; ///< =6 #�ն˵ȴ��Ӷ�վ��Ӧ�ĳ�ʱʱ��
	S_WORD m_LoginPeriodS; ///< =60 #0��ʾ��Ҫ��½
	S_DWORD m_MonTrafficLimit; ///< =0 #0��ʾ����Ҫ��������
	char m_ChannlName[HTTX_CHANNL_MAX][CONS_CHANNEL_NAME_MAX_LEN];

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
	bool NeedHeartBeat(void){return m_HeartBeatPeriodM>0;}
	bool NeedLogon(void){return m_LoginPeriodS>0;}
};

struct S_HttxStatus
{
//	S_HttxSN m_HttxSN;
	bool m_fReconnect;
	bool m_fMasterTerm;
	bool m_fForwardEnable;
	S_WORD m_ResponseCmdCnt;
	S_WORD m_ReportCmdCnt;
	S_TIME m_tickRecvMsData;

	void Init(void)
	{
//		m_HttxSN = MP_HTTX_1;
		m_fReconnect = false;
		m_fMasterTerm = true;
		m_fForwardEnable = true;
		m_ResponseCmdCnt = 0;
		m_ReportCmdCnt = 0;
		m_tickRecvMsData = 0;
	}
};

#define PPP_CHANNL_MODEM	0
#define PPP_CHANNL_GPRS	1
#define PPP_CHANNL_AT		2
#define PPP_CHANNL_SMS		3
#define PPP_CHANNL_MAX	4
#define PPP_APN_LENGTH_MAX	32
#define PPP_APN_COUNT_MAX		20

struct S_PppInfo
{
	int m_ModemPowerOffDelayS;
	int m_ModemPowerOnDelayS;
	S_WORD m_RefreshSignalInterS;///< ˢ�������źż����ȱʡ300��
	S_WORD m_RedialInterM; ///< �����ز������ȱʡ30����
	S_WORD m_RedialTimesPerDay; ///< #���ز�����
	S_DialInfo m_DialInfo; ///< ������Ϣ
	char m_MasterPhone[16]; ///< ��վ�绰����
	char m_SmsCenterNo[16]; ///< ��Ϣ���ĺ���
	char m_ApnList[PPP_APN_COUNT_MAX][PPP_APN_LENGTH_MAX];
	char m_ChannlName[PPP_CHANNL_MAX][CONS_CHANNEL_NAME_MAX_LEN]; ///< #MODEMͨ��,GPRSͨ��,AT����ͨ��,SMSͨ��

	void Init(void)
	{
		memset(this, 0x00, sizeof(*this));
		m_ModemPowerOffDelayS = 2;
		m_ModemPowerOnDelayS = 4;
		m_RefreshSignalInterS = 300;
		m_RedialInterM = 30;
		m_RedialTimesPerDay = 3;
	}
};

struct S_PppStatus
{
	bool m_fGprsDialing; ///< ������
	char m_ModemFactory[32];
	char m_ModemModel[32];
	char m_ModemSim[32];
	char m_ModemVer[64];
	S_BYTE m_ModemType; ///< MODEM ����
	S_BYTE m_WirelessNetType; ///< ע���ϵ���������0-3:GPRS,CDMA,3G,4G
	S_BYTE m_LcdBottomInfoIndicator; ///< Һ����ʾPPP״ָ̬ʾ��
	S_BYTE m_Signal; ///< �ź�ǿ��(0~31)
	S_TIME m_timeSignal; ///< ȡ�ź�ʱ��
	S_DWORD m_SimIP;
	S_TIME m_timeLastOnline;
	S_PppPackage m_PppPackage;

	void Init(void){memset(this, 0x00, sizeof(*this));}
};

#define LCD_TERM_NAME_LEN_MAX	32
struct S_LcdInfo
{
	S_BYTE m_LCDGreyScale; ///< =195 #Һ���Ҷ�ֵ
	S_BYTE m_LCDBrightness; ///< Һ����������
	S_BYTE m_LCDContrast; ///< =80 #Һ���Աȶ� 0-99
	S_BYTE m_LCDDirection; ///< =0 #Һ����ת���� 0-3

	int m_LCDPassword; ///< =0 #Һ����������Ȩ������
	int m_LCDLightTimeoutS; ///< =60 #Һ������Ƶ���ʱ��
	int m_BackDesktopTimeoutS; ///< =40 #���������ʱ��

	S_DWORD m_ZXYGFn; ///< [0-255] #�����й����ն���Fnֵ
	S_DWORD m_ZXWGFn; ///< [0-255] 
	S_DWORD m_FXYGFn; ///< [0-255] 
	S_DWORD m_FXWGFn; ///< [0-255] 
	S_DWORD m_YXXWGFn; ///< [0-255] 
	S_DWORD m_EXXWGFn; ///< [0-255] 
	S_DWORD m_SXXWGFn; ///< [0-255] 
	S_DWORD m_FXXWGFn; ///< [0-255] 
	S_DWORD m_VOLTFn; ///< [0-255] 
	S_DWORD m_CURRFn; ///< [0-255] 
	S_DWORD m_ACTPWRFn; ///< [0-255] 
	S_DWORD m_WATTLESSPWRFn; ///< [0-255] 
	S_DWORD m_PWRFACTORFn; ///< [0-255] 
	S_DWORD m_ZXYGXLFn; ///< [0-255] 
	S_DWORD m_FXYGXLFn; ///< [0-255] 
	

	char m_TermName[LCD_TERM_NAME_LEN_MAX]; ///< =***̨��������

	void Init(void)
	{
		memset(this, 0x00, sizeof(*this));
		m_LCDPassword = 0;
		m_LCDGreyScale = 90;
		m_LCDBrightness = 90;
		m_LCDContrast = 90;
		m_LCDLightTimeoutS = 60;
		m_BackDesktopTimeoutS = 40;

		m_ZXYGFn = 0X05060100;
		m_ZXWGFn = 0X05060300;
		m_FXYGFn = 0X05060200;
		m_FXWGFn = 0X05060400;
		m_YXXWGFn = 1;
		m_EXXWGFn = 2;
		m_SXXWGFn = 2;
		m_FXXWGFn = 1;
		m_VOLTFn = 27;
		m_CURRFn = 29;
		m_ACTPWRFn = 81;
		m_WATTLESSPWRFn = 85;
		m_PWRFACTORFn = 105;
		m_ZXYGXLFn = 3;
		m_FXYGXLFn = 4;

		strcpy(m_TermName, "������");
	}

};
#define LCD_PROGRAM_KEY_DELAY_S		180
struct S_LcdStatus
{
	bool m_UsbDiskPlugin; ///< U�̲���
	bool m_UsbCopyFile; ///< U�̿����ļ�
	bool m_Eth0LinePlugin; ///< ���߲���
	bool m_UpdateDlog; ///< ������Ϣ����ʾ
	int m_UpdateInfo; ///< ������Ϣ�����
	int m_ProgramKeyDelayS; ///< ��̼���ʱ

	S_TIME m_timeFlashStorage; ///< FLASH����ʱ��
	S_TIME m_timeSerialComm[SYSTEM_SERIAL_PORT_MAX]; ///< ����ͨ��ʱ��

	void Init(void){memset(this, 0x00, sizeof(*this));}
};

struct S_DbcjParam
{
	S_BYTE m_MeterAutoUpdate; ///< ���ܱ�ͨ�Ų����Զ�ά������
	S_WORD m_LineLossRate; ///< ��������Խ�ޱ�����ֵ,��ʽNN.N0,���ֽ�������,���ֽ���С��
	S_DWORD m_Operator; ///< ��Ʋ�����

	void Init(void){memset(this, 0x00, sizeof(*this));}
};

#define DBCJ_CHANNL_RS485I		0
#define DBCJ_CHANNL_RS485II	1
#define DBCJ_CHANNL_ZBWX	2
#define DBCJ_CHANNL_MAX	3
struct S_DbcjInfo
{
	S_TIME m_GatherStdTime;///< �ɼ���׼��ʼʱ��
	S_TIME_UNIT m_GatherPeriodUnit;///< �ɼ����ڵ�λ
	S_BYTE m_GatherPeriodVal;///< �ɼ�����ֵ
	S_BYTE m_AlmPollCycleM; ///< �澯��ѯ����
	S_BYTE m_VipBeginTime[2]; ///< ={10,40} #�ص㻧����ɼ���ʼʱ�� ����
	S_BYTE m_DayBeginTime[2]; ///< =1:10 #������ɼ���ʼʱ�� Сʱ:����
	S_BYTE m_DayVolFreezeTime[2]; ///< =1:10 #�յ�ѹ�ϸ������ݶ��Ὺʼʱ�� Сʱ:����
	S_BYTE m_MonBeginDate; ///< =1 #������ɼ���ʼ����
	S_BYTE m_MonBeginTime[2]; ///< =1:30 #������ɼ���ʼʱ�� Сʱ:����
	S_BYTE m_MonVolFreezeDate; ///< =1 #�µ�ѹ�ϸ������ݶ��� �ɼ���ʼ����
	S_BYTE m_MonVolFreezeTime[2]; ///< =1:30 #�µ�ѹ�ϸ������ݶ��Ὺʼʱ�� Сʱ:����
	char m_ChannlName[DBCJ_CHANNL_MAX][CONS_CHANNEL_NAME_MAX_LEN];

	void Init(void){memset(this, 0x00, sizeof(*this));}
};
enum
{
	DBCJ_UPDATE_METER_IDLE = 0,
	DBCJ_UPDATE_METER_STANDBY,
	DBCJ_UPDATE_METER_NOTIFY,

	DBCJ_UPDATE_METER_MAX
};
enum
{
	DBCJ_METER_CTRL_NOTHING = 0,
	DBCJ_METER_CTRL_STOP, ///< ��ͣ����
	DBCJ_METER_CTRL_RESUME, ///< �ָ�����
	DBCJ_METER_CTRL_RESET, ///< ���³���

	DBCJ_METER_CTRL_MAX
};
struct S_DbcjStatus
{
	bool m_fReconnect;
	S_BYTE m_UpdateMeter485I; ///< 485Iͨ�����±��־ [DBCJ_UPDATE_METER_]
	S_BYTE m_UpdateMeter485II; ///< 485IIͨ�����±��־ [DBCJ_UPDATE_METER_]
	S_BYTE m_UpdateMeterZBWX; ///< ZBWXͨ�����±��־ [DBCJ_UPDATE_METER_]

	S_BYTE m_ControlMeter485I; ///< 485Iͨ��������� [DBCJ_METER_CTRL_]
	S_BYTE m_ControlMeter485II; ///< 485IIͨ���������  [DBCJ_METER_CTRL_]
	S_BYTE m_ControlMeterZBWX; ///< ZBWXͨ���������  [DBCJ_METER_CTRL_]

	S_BYTE m_LcdBottomInfoIndicator; ///< Һ����ʾDBCJ״ָ̬ʾ��

	S_TIME m_timeAddMeter; ///< ��ɾ���
	S_TIME m_timeAddTask; ///< ��ɾ����
	S_TIME m_timeAddAlarm; ///< �·��澯��Ļ
	S_TIME m_timeManualRoute; ///< �ֶ�·��
	S_TIME m_timeRouteError; ///< ·�ɳ���
	S_TIME m_timeReportMeter; ///< �·��ѱ�����

	char m_RouterAddr[8]; ///< ģ���ַ	BCD	6
	char m_RouterFactory[4]; ///< ���̴���	ASCII	2
	char m_RouterChip[4]; ///< оƬ����	ASCII	2
	char m_RouterDate[3]; ///< �汾����-������	BCD	3
	char m_RouterVersion[2];///< �汾	BCD	2

	S_MPBS m_mpbsMyMeter;
	S_VCOMBS m_bsMyVCom;

	void Init(void){memset(this, 0x00, sizeof(*this));}
};

#define JLCY_CHANNL_METER_CHIP	0
#define JLCY_CHANNL_AC_GATHER		1
#define JLCY_CHANNL_ADJUST_METER	2
#define JLCY_CHANNL_MAX	3
struct S_JlcyInfo
{
	char m_ChannlName[JLCY_CHANNL_MAX][CONS_CHANNEL_NAME_MAX_LEN];

	void Init(void){memset(this, 0x00, sizeof(*this));}
};

#define JLCY_StopupCal	0     //ֹͣУ���δ����У��̬
#define JLCY_StartupCal	90		//����У��̬������У��
struct S_JlcyStatus
{
	int m_StartupCal;

	void Init(void){memset(this, 0x00, sizeof(*this));}
};

struct S_CtcyInfo
{
	int m_dummy;

	void Init(void){memset(this, 0x00, sizeof(*this));}
};
struct S_CtcyStatus
{
	int m_dummy;

	void Init(void){memset(this, 0x00, sizeof(*this));}
};

struct S_FhkzInfo
{
	S_FKCtrlBaseInfo m_BaseInfo;

	void Init(void){memset(this, 0x00, sizeof(*this));}
};

struct S_FhkzStatus
{
	int m_Dummy;

	void Init(void){memset(this, 0x00, sizeof(*this));}
};

//system info define
//������ṹ��Ϣ
enum
{
	MP_TYPE_UNKNOW, // 
	MP_TYPE_METER, // ���
	MP_TYPE_ANALOG, // ģ����
	MP_TYPE_PULSE, // ����
	MP_TYPE_SUM, // ������
	MP_TYPE_AC, // ����

	MP_TYPE_MAX
};
struct S_MpInfo
{
	S_WORD m_AutoGather:1;//��ֵ��Χ0��1��"1"��ʾͶ����"0"��ʾͣ��
	S_WORD m_Type:3;//[MP_TYPE_] 
	S_WORD m_SN:12;//���
};
struct S_MpInfos
{
	struct S_MpInfo m_Item[CONS_MP_COUNT_MAX];

	void Init(void){memset(this, 0x00, sizeof(*this));}
	/** ����MPλ���ǲ���type���͵Ĳ����� */
	bool Test(S_WORD MP, S_WORD type) const;
	/** ����MPλ���ܲ��ܷ���type���͵Ĳ����� */
	bool Test(S_WORD MP, S_WORD type, S_WORD SN) const;
	bool Set(S_WORD MP, S_WORD type, S_WORD SN);
	bool Reset(S_WORD MP, S_WORD type, S_WORD SN);
};
//
//�����������Ϣ
//
struct S_ALLMP
{
	struct S_MTBS m_MTBS;
	struct S_MPBS m_MultiMPBS;
	struct S_MPBS m_VipMPBS;

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
	void RefreshMeterInfo(S_BYTE PortZB, S_MeterInfos *pMeterInfos, S_MpInfos *pMpInfos);
};

//
// ��������ֵ����
//��������������
#define Max_PowerFactorVal_num  256
struct S_PowerFactor
{
	S_WORD m_PowerFactorVal;//�����������ζ�ֵΪ
};
struct S_MPointLimit
{
	bool m_Enable;
	S_WORD m_IUBRatio;//������ƽ�����ֵ��ȱʡֵ��������30% �������ߣ�50%
	S_WORD m_IUBResumeRatio;//������ƽ��Ȼָ���ֵ��ȱʡֵ��������25% �������ߣ�40%
	S_WORD m_Uhgsx;//�ϸ��ѹ����(����ֵ��δ���û�0ʱ������ȱʡ�����ж�)��ȱʡֵΪ���ѹ��1.07��
	S_WORD m_Uhgxx;//�ϸ��ѹ����(����ֵ��δ���û�0ʱ������ȱʡ�����ж�)��ȱʡֵΪ���ѹ��0.93��
	S_WORD m_IoverRatio;//�жϵ�����������Զֵ�ı�����ȱʡΪ1.3
	S_WORD m_FhgzRatio;//�жϸ��ɹ�����Զֵ�ı�����ȱʡΪ1.2
	S_WORD m_FhhfRatio;//�жϸ��ɹ��ػָ�����Զֵ�ı�����ȱʡΪ0.95
	S_WORD m_IZeroRatio;//�������������ֵ��ȱʡֵ�������25%
	S_WORD m_IZeroRRatio;//������������ָ���ֵ��ȱʡֵ�������15%
	S_WORD m_IresumeRatio;//�жϵ��������ɻָ�����Զֵ�ı�����ȱʡֵΪ1.0
	S_WORD m_UUBRatio;//��ѹ��ƽ�����ֵ
	S_WORD m_UUBResumeRatio;//��ѹ��ƽ��Ȼָ���ֵ
	S_WORD m_UOpenRatio;//������ѹȱʡΪ���ѹ78%
	S_WORD m_UReturnRatio;//���ص�ѹȱʡΪ���ѹ85%
	S_WORD m_IOpenRatio;//��������ȱʡΪ�����0.5%
	S_WORD m_IReturnRatio;//���ص���ȱʡΪ�����5%
	S_WORD m_UjbRatio;//��ѹ��������ֵ��ȱʡֵ5%
	S_WORD m_IjbRatio;//������������ֵ��ȱʡֵ5%
	S_WORD m_UjbResumeRatio;//��ѹ����ָ���ֵ��ȱʡֵ3%
	S_WORD m_IjbResumeRatio;//��������ָ���ֵ��ȱʡֵ3%
	S_BYTE m_PFSectionNum;//��������������
	S_WORD m_PowerFactorVal[Max_PowerFactorVal_num];//�����������ζ�ֵ
	S_BYTE  m_HarmonicNum;//���г������
	S_WORD m_UbcsxRatio;//��ѹƫ������
	S_WORD m_UbcxxRatio;//��ѹƫ������
	S_WORD m_FrebcsxRatio;//Ƶ��ƫ������
	S_WORD m_FrebcxxRatio;//Ƶ��ƫ������
	S_WORD m_ULTwinkleVal;//��ѹ������ֵ
	S_WORD m_USTwinkleVal;//��ѹ������ֵ
	S_WORD m_WGCompensate;//�޹����������޹�Ƿ�����澯������ֵ�ܹ�������ȱʡ0.8
	S_WORD m_WGrCompensate;//�޹����������޹�Ƿ�����澯�ָ�������ֵ�ܹ�������ȱʡ0.9
};
struct S_MPLS
{
	struct S_MPointLimit Item[CONS_METER_COUNT_MAX];

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
};

/** @brief ����ģ�� */
struct S_TaskTemplate
{
	bool m_Enable;
	char m_TaskType[TASKINFO_NAME_MAX];
	S_BYTE m_Priority;///< ���ȼ���Խ���ʾ���ȼ�Խ��
	S_TIME_UNIT m_TimeUnit;///< ���ڵ�λ
	S_BYTE m_PeriodVal;///< ����ֵ:0��ʾ��������
	S_BYTE m_RetryTimes;///< һ�����������Դ���:0��ʾ���޴���
	S_BYTE m_MaxStartTimes;///< �����������:0��ʾ���޴���
	S_TIME m_StandardTime;///< ��׼��ʼʱ��
	S_DWORD m_ExecTimeS;///< ���������ִ��ʱ�䣬��λ��,0��ʾ����ʱ��
	S_DWORD m_ReportPeriodS;///< ���������ϱ����ڣ�����ϱ�����
};
#define TASK_TEMPLATE_MAX		30
struct S_TaskTemplates
{
	struct S_TaskTemplate m_Item[TASK_TEMPLATE_MAX];

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
};

//
//��վ������
//
#define MSDI_FLAG_TIME	0x01
//#define MSDI_FLAG_TARIFF	0x02
#define MSDI_FLAG_BLOCK	0x04
enum
{
	MSDI_TYPE_NORMAL,
	MSDI_TYPE_TARIFF,
	MSDI_TYPE_STATUS,
	MSDI_TYPE_HARMONIOUS,
	MSDI_TYPE_CLASS1_HOUR,

	MSDI_TYPE_MAX
};
enum
{
	MSDI_FREEZE_HOUR,
	MSDI_FREEZE_DAY,
	MSDI_FREEZE_MONTH,

	MSDI_FREEZE_MAX
};
struct S_MSDI
{
	bool m_Enable;
	S_BYTE m_Class;
	S_BYTE m_MSDI;
	S_BYTE m_Type;///Normal,Tariff,Status,Harmonious MSDI_TYPE_
	S_BYTE m_Flag; ///< MSDI_FLAG_
	S_BYTE m_Freeze; ///< #Freeze:��������,HOUR����,DAY��,MONTH��MSDI_FREEZE_
	S_BYTE m_TariffCnt;
	S_DWORD m_DI; ///< ������ԼҪ��������ID
	S_DWORD m_CommRegID[MSDI_COMM_REG_MAX];
	S_DIBS m_DisableCommReg;

public:
	S_MSDI(void){memset(this, 0x00, sizeof(*this));}
	int SerializeData(const S_REGISTERS &CommRegs, const strings &strRegDatas, string &strData)const;
	int FillData(string &strData)const;

private:
	int SerializeData_Normal(const S_REGISTERS &CommRegs, const strings &strRegDatas, string &strData)const;
	int SerializeData_Tariff(const S_REGISTERS &CommRegs, const strings &strRegDatas, string &strData)const;
	int SerializeData_Status(const S_REGISTERS &CommRegs, const strings &strRegDatas, string &strData)const;
	int SerializeData_Harmonious(const S_REGISTERS &CommRegs, const strings &strRegDatas, string &strData)const;
	int SerializeData_Class1Hour(const S_REGISTERS &CommRegs, const strings &strRegDatas, string &strData)const;

	S_BYTE FindMaxTariffCnt(const S_REGISTERS &CommRegs, const strings &strRegDatas)const;
	int FixDataTariff(const S_REGISTERS &CommRegs, strings &strRegDatas, S_BYTE MaxTariffCnt)const;
};

struct S_MSDIF
{
	bool m_Enable;
	S_BYTE m_Type:4;///< Normal,Tariff,Status,Harmonious,Class1Hour MSDI_TYPE_
	S_BYTE m_Freeze:4; ///< #Freeze:��������,HOUR����,DAY��,MONTH��MSDI_FREEZE_
	S_BYTE m_Flag; ///< MSDI_FLAG_
	S_BYTE m_TariffCnt;
	S_DWORD m_DI; ///< ������ԼҪ��������ID
	S_WORD m_idxRegBegin;
	S_WORD m_idxRegEnd;
	S_DIBS m_DisableReg;
};

#define MSDI_ITEM_MAX	256
#define MSDI_DATA_ID_MAX		2048
struct S_MSDIS
{
	S_MSDIF m_ItemClass1[MSDI_ITEM_MAX];
	S_MSDIF m_ItemClass2[MSDI_ITEM_MAX];
	S_MSDIF m_ItemClass3[MSDI_ITEM_MAX];
	S_MSDIF m_ItemClass4[MSDI_ITEM_MAX];
	S_MSDIF m_ItemClass5[MSDI_ITEM_MAX];
	S_MSDIF m_ItemClass6[MSDI_ITEM_MAX];
	S_MSDIF m_ItemClass7[MSDI_ITEM_MAX];
	S_MSDIF m_ItemClass8[MSDI_ITEM_MAX];
	S_MSDIF m_ItemClass9[MSDI_ITEM_MAX];
	S_WORD m_idxCommReg;
	S_DWORD m_CommRegID[MSDI_DATA_ID_MAX];

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
};

//
//Register area
//
#define COMMON_REGISTER_MAX		512
struct S_CommRegisters
{
	S_WORD m_Cnt;
	struct S_REGISTER m_Item[COMMON_REGISTER_MAX]; 

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
};

#endif//__NW_SYS_BASE_H__



