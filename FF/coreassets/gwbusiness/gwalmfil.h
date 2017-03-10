/**
 *  @file
 *  @brief �������澯�����ļ���ȡ������
 *  @author 
 *  @date 2012-08-08
 *  @version v1.0
 *  @note
 */
#ifndef __GWB_ALARM_FILE_H__
#define __GWB_ALARM_FILE_H__
#include "basedef.h"
#include "bitsetex.h"
#include "fileop.h"
#include "timeop.h"

enum S_AlarmType
{
	ALARM_TYPE_UNKNOW,
	ALARM_TYPE_NORMAL,
	ALARM_TYPE_IMPORTANT,

	ALARM_TYPE_MAX
};

/** @brief �����ļ���ʽ
 * �ļ�ͷ��ʽ���� 
 * |�ļ���ʶ|��¼��׼ʱ��|��¼��ͷ|�ļ��洢����|
 * |---16---|------8-----|---2----|-----2------|
 * |��¼����|�汾��Ϣ|����|У��|
 * |---2----|---6----|-10-|--2-|
 * //�����ļ�ͷ��48���ֽڣ��������ʵ�������
 * �¸澯�ϱ���־��
 * |------32------|
 * �澯��������ʽ
 * |�澯��������ʶ|�澯ʱ��|���ݱ�־/��¼����|��¼����|�澯����|
 * |-------8------|----8---|---4b-------4b---|---1----|---256--|
 * //���С��ļ��洢��������������
 */
//#define CONST_ALARM_DEFAULT_CHAR   0xEE
#define CONST_ALARM_RECORD_MAX		256

#define CONST_ALARM_FLAG_LENGTH		8
#define CONST_ALARM_TIME_LENGTH		8
#define CONST_ALARM_LEN_LENGTH		2
#define CONST_ALARM_RECORD_LENGTH	256    //  һ����¼�ĳ��ȣ��ֽڣ�
#define CONST_ALARM_AREA_LENGTH		(CONST_ALARM_FLAG_LENGTH+CONST_ALARM_TIME_LENGTH+CONST_ALARM_LEN_LENGTH+CONST_ALARM_RECORD_LENGTH)

#define CONST_ALARM_FILE_NAME_OFF		0
#define CONST_ALARM_FILE_NAME_SIZE		16
#define CONST_ALARM_TIMEFLAG_OFF		(CONST_ALARM_FILE_NAME_OFF+CONST_ALARM_FILE_NAME_SIZE)
#define CONST_ALARM_TIMEFLAG_SIZE		8
#define CONST_ALARM_QHEAD_OFF			(CONST_ALARM_TIMEFLAG_OFF+CONST_ALARM_TIMEFLAG_SIZE)
#define CONST_ALARM_QHEAD_SIZE			2
#define CONST_ALARM_ALLRECORD_OFF		(CONST_ALARM_QHEAD_OFF+CONST_ALARM_QHEAD_SIZE)
#define CONST_ALARM_ALLRECORD_SIZE		2
#define CONST_ALARM_RECORDLEN_OFF		(CONST_ALARM_ALLRECORD_OFF+CONST_ALARM_ALLRECORD_SIZE)
#define CONST_ALARM_RECORDLEN_SIZE		2
#define CONST_ALARM_VERSION_OFF			(CONST_ALARM_RECORDLEN_OFF+CONST_ALARM_RECORDLEN_SIZE)
#define CONST_ALARM_VERSION_SIZE		6
#define CONST_ALARM_RESERVE_OFF			(CONST_ALARM_VERSION_OFF+CONST_ALARM_VERSION_SIZE)
#define CONST_ALARM_RESERVE_SIZE		10
#define CONST_ALARM_CHECKSUM_OFF		(CONST_ALARM_RESERVE_OFF+CONST_ALARM_RESERVE_SIZE)
#define CONST_ALARM_CHECKSUM_SIZE		2

#define CONST_ALARM_HEADER_LENGTH		(CONST_ALARM_FILE_NAME_SIZE+CONST_ALARM_VERSION_SIZE+CONST_ALARM_TIMEFLAG_SIZE+CONST_ALARM_QHEAD_SIZE+CONST_ALARM_ALLRECORD_SIZE+CONST_ALARM_RECORDLEN_SIZE+CONST_ALARM_CHECKSUM_SIZE+CONST_ALARM_RESERVE_SIZE)
#define CONST_NEW_ALARM_FLAG_MAX		((CONST_ALARM_RECORD_MAX+7)/8)


struct S_AFHeader
{
	INT8U  m_Signature[CONST_ALARM_FILE_NAME_SIZE]; ///< �����ļ���־��Ŀǰ���ļ���
	S_TIME m_RecordStdT; ///< ��¼�ο�ʱ��
	INT16U m_RecordQHead; ///< ��¼��ͷ
	INT16U m_RecordAll; ///< �ܼ�¼����
	INT16U m_RecodrLen; ///< ��¼����
	INT8U  m_Version[CONST_ALARM_VERSION_SIZE];
	INT16U m_CheckSum; ///< �ļ�ͷУ��

public:
	/** @brief ������ṹ���ֵ���ļ�ͷ�ṹ��ʽ�������� */
	int Serialization(INT8U *pBuffer, int size);
	/** @brief �ӻ����л�ȡ�ļ�ͷ����ֵ */
	int Unserialization(const INT8U *pBuffer, int size);
	/** @brief ����check sum */
	int CalculateCheckSum(const INT8U *pBuffer, int size);
};

class C_AFRW
{
public:
	C_AFRW(const char *pAlarmFileName);
	~C_AFRW();

	bool operator!(void){return !m_FileObj;}
	bool CheckHeader(void);
	bool ExtractNewFlag(void);
	bool UpdateHeader(void);
	bool UpdateNewFlag(void);

	bool UpdateRecord(const string &strAlarmInfo);
	bool ExtractRecord(string &strAlarmInfo, S_WORD Index, bool fUpdateNewFlag);

	int GetAlarmIndex(void){return m_Header.m_RecordQHead;}
	int GetAlarmRecordMax(void){return m_Header.m_RecordAll;}
	int CountNew(void){return m_NewFlag.CountBit();}
	bool AlarmNew(S_WORD index){return m_NewFlag.TestBit(index);}

private:
	S_WORD DataRecordLenFlag(S_BYTE flag, S_WORD len);
	/** @brief ��ȡ���ݼ�¼��־ */
	S_BYTE GetDataRecordFlag(char Data){return (S_BYTE)((Data >> 4) & 0x0F);}
	/** @brief ��ȡ���ݼ�¼���� */
	S_WORD GetDataRecordLen(char H, char L){return (((S_WORD)(H & 0x0F)) << 8) | (((S_WORD)L) & 0xFF);}
	int NewAlarmFile(void);
	int InitHeader(void);

private:
	char m_AlarmFileName[CONST_ALARM_FILE_NAME_SIZE];
	C_BITSET<CONST_NEW_ALARM_FLAG_MAX> m_NewFlag;
	S_AFHeader m_Header;
	C_FILE m_FileObj;
};

class C_AlarmDB
{
public:
	int AddRecord(const string &strAlarmInfo, S_AlarmType AlarmType);
	int GetRecord(string &strAlarmInfo, S_AlarmType AlarmType, S_WORD index);
	int ReportRecord(string &strAlarmInfo, S_AlarmType AlarmType, S_WORD index);
	int GetEC(S_AlarmType AlarmType);
	int CountNew(S_AlarmType AlarmType);

private:
	const char *GetAlarmFileName(S_AlarmType AlarmType);
};

#endif //__GXJCB_ALARM_FILE_H__
