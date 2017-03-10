/** @file
 *  @brief �澯�����ඨ��
 *  @author ������
 *  @date 2013/09/09
 *  @version 0.1
 */
#ifndef __NW_ALMEVT_BASE_H__
#define __NW_ALMEVT_BASE_H__

#include "basedef.h"
#include "syscfg.h"
#include "bitsetex.h"
#include "basetype.h"

//
//�澯����
//
#define ALARM_REGISTER_DATA_LEN_MAX		90

#define ALM_COMMID(id)	(0xE2000001+id)	//����ͨѶ��Լ����ĸ澯���ݱ�ʶ
#define EVT_COMMID(id)	(0xE2010001+id)	//����ͨѶ��Լ������¼����ݱ�ʶ

#define CONS_METER_COMFAIL_COUNT_MAX	256	//256���ֽ�BIT

/** @brief �澯��ⷽʽ */
#define ALM_DETECT_METHOD_TRIGGER		0x01 /*����ʽ*/
#define ALM_DETECT_METHOD_REALTIME		0x02 /*ʵʱ��ѯʽ*/


struct S_FBS:public C_BITSET<CONS_METER_COMFAIL_COUNT_MAX> //����ʧ�ܱ���
{
};

typedef enum
{//��Ӧ��Լ��ż�1
	ALARM_CURRENT_REVERSE_A = 12,//��������
	ALARM_CURRENT_REVERSE_B = 13,
	ALARM_CURRENT_REVERSE_C = 14,
	ALARM_LOSE_CURRENT_A=18,//ʧ��
	ALARM_LOSE_CURRENT_B=19,
	ALARM_LOSE_CURRENT_C=20,
	ALARM_LOSE_VOLTAGE_A=21,//ʧѹ
	ALARM_LOSE_VOLTAGE_B=22,
	ALARM_LOSE_VOLTAGE_C=23,
	ALARM_LACK_OF_MONERY=42, //�������
	ALARM_DEGREE_DECREASE=43,//ʾ���½�
	ALARM_METER_STOP=45,//���ͣ��
	ALARM_TERMINAL_POWEROFF=50,//�ն˵���
	ALARM_TERMINAL_POWERON=51,//�ն��ϵ�
	ALARM_METER_PROGRAM_TIME=52,//�����ʱ��
	ALARM_PERIOD_TARIFF_CHANGE=53,//ʱ�λ����
	ALARM_MONTH_FLOW_OVERRANGE=57,//������Խ��
	ALARM_RELAY_CHANGE=58,//�̵�����λ
	ALARM_LAHEZHA_FAIL=59,//����բʧ��
	ALARM_READMETER_FAIL=60,//����ʧ��
	ALARM_METER_CLOCK_ABNORMAL=61,//���ܱ�ʱ���쳣
	ALARM_LOSE_VOLTAGE_TIMES=63,//ABC��ʧѹ����
	ALARM_LOSE_CURRENT_TIMES=64,//ABC��ʧ������
	ALARM_LOSE_CURRENT_REVERSE_TIMES=65,//ABC�ೱ���������
	ALARM_METER_PROGRAM_TIMES=66,//����̴���
	ALARM_MAX
}T_emAlarmID;

typedef enum
{// ��Ӧ��Լ��ż�1
	EVENT_POWER_OFFON = 9,// �ն�ͣ�ϵ�

	EVENT_ID_MAX
}T_emEventID;

typedef enum
{
	ALARMSTATE_RESUME=0x00,
	ALARMSTATE_CREATE=0x01,
	ALARMSTATE_INVALID=0xff
}T_emAlarmStatus;

struct S_AlarmState
{
	INT8U m_AlarmId;
	INT8U m_State;
};


struct S_MpAlarmState
{
	S_AlarmState  m_AlarmStates[CONS_ALARM_TYPS_COUNT_MAX];
 	//void InitAlarmState(INT8U index, INT8U alarmId,INT8U state);
	bool SetAlarmState(INT8U alarmId,INT8U state);
	T_emAlarmStatus GetAlarmState(INT8U alarmId);
};

/** @brief �澯״̬
 */
struct S_AlarmStatus
{
	S_TIME m_tickImportantHappen;
	int m_ImportantNewCnt;
	int m_NormalNewCnt;
	int m_EventNewCnt;
	BitSetEx<256> m_bsNewAlarmID; ///< �����澯
	S_MpAlarmState m_MpAlarmStates[CONS_METER_COUNT_MAX];//��¼�������и澯�¼��ĵ�ǰ״̬
public:
	void Init(void);
	void Flush(void);
};

//
// �澯��ر�־������
//
struct S_AlmFlag
{
	bool m_fTaskFinish;
	S_MPBS m_mpbsSuccess;
};
#define ALM_FLAGS_ITEM_MAX	128
struct S_AlmFlags
{
	struct S_AlmFlag m_Current;
	struct S_AlmFlag m_itemAlm[ALM_FLAGS_ITEM_MAX];

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
	/** @note ����˳��
	 * 1 TestTask; 2 TestMeter; 3 ResetTask
	 */
	bool TestTask(S_BYTE ERC);
	bool ResetTask(S_BYTE ERC);
	bool TestMeter(S_BYTE ERC, S_WORD MP);
	bool SetTask(S_BYTE ERC, const S_MPBS &mpbsSuccess);
};

struct S_AlmData
{
	bool m_Enable;
	S_BYTE m_DataLen;
	char m_Data[ALARM_REGISTER_DATA_LEN_MAX];
};

struct S_AlmDatas
{
	struct S_AlmData m_ERC00[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC13[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC14[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC15[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC19[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC20[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC21[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC22[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC23[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC24[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC43[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC44[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC46[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC53[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC54[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC59[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC61[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC62[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC64[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC65[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC66[CONS_METER_COUNT_MAX];
	struct S_AlmData m_ERC67[CONS_METER_COUNT_MAX];


public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
	/** @return >0:�ɹ�; <=0:ʧ��*/
	int Load(S_DWORD ERC, S_WORD MP, string &strAlmData);
	/** @return >=0:�ɹ�; <0:ʧ��*/
	int Save(S_DWORD ERC, S_WORD MP, const string &strAlmData);

private:
	int _Load(const S_AlmData &AlmData, string &strAlmData);
	int _Save(S_AlmData &AlmData, const string &strAlmData);
};

// �澯�������� �������ļ��У���ҪƵ��ˢ�£��и��²�ˢ��
class C_AlmPhaseData
{
public:
	/** @return >0:�ɹ�; <=0:ʧ��*/
	int Load(S_DWORD ERC, S_WORD MP, string &strAlmData);
	/** @return >=0:�ɹ�; <0:ʧ��*/
	int Save(S_DWORD ERC, S_WORD MP, const string &strAlmData);
};

//
//
//
struct S_RegData
{
	S_DWORD m_regid;
	string m_data;
};

struct S_RegDatas:public vector<S_RegData>
{

};

class CAlarmEvtCommIdData
{
public:
	static bool GetCommRegDataFromSTS(INT16U mp,S_DWORD DI,S_RegDatas &rtSTSRegDatas);
	static bool GetCommRegDataFromCTX(INT16U mp,S_DWORD DI,S_RegDatas &rtCTXRegDatas);
public:
	static bool IsValidData(const char *pdata, INT8U len);
};



#endif

