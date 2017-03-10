/**
 *  @file DataFile.h 
 *  @brief ���������������ļ���ȡ������
 *  @author 
 *  @date 2009-11-24
 *  @version v1.0
 *  @note
 *  1��ͬһ��FN�������������Ǹ�Ϊ׼�����㲿�������Ч����0xEE��������FN��ͬ
 *  2��ͬһ���ص㻧FN���ݵ���ֻ�̶ܹ�Ϊĳ������ֵ��24�ı�������������FN��ͬ
 */
#ifndef __GXJCB_POWER_FILE_H__
#define __GXJCB_POWER_FILE_H__
#include "basedef.h"
#include "basetype.h"
#include "timeop.h"
#include "fileop.h"


#define GW_MP_MAX	BASEB_MP_COUNT_MAX
#define GW_TARIFF_MAX	12

/** @brief �����ļ���ʽ
 * �ļ�ͷ��ʽ���� 
 * |�ļ���ʶ|���������|������¼��|��¼����|
 * |---16---|------2-----|-------2------|---2----|
 * |��¼��׼ʱ��|��¼���ʱ��|��¼�����λ|����|У��|
 * |-----8------|------1-----|------1-----|-14-|--2-|
 * //�����ļ�ͷ��48���ֽڣ��������ʵ�������
 *
 * ������������ʽ
 * |������������ʶ|���ܱ��ַ|��β|��ͷ|
 * |-------10-----|-----6----|--1-|--1-|
 * |��¼ʱ��|��ʼ��־|���ݱ�־/��¼����|��¼����|��¼1��������|
 * |--3or4--|----1---|---4b-------4b---|---1----|------N------|
 * |��¼ʱ��|��ʼ��־|���ݱ�־/��¼����|��¼����|��¼2��������|
 * |--3or4--|----1---|---4b-------4b---|---1----|------N------|
 * |��¼ʱ��|��ʼ��־|���ݱ�־/��¼����|��¼����|��¼3��������|
 * |--3or4--|----1---|---4b-------4b---|---1----|------N------|
 * //���С���������š�������������
 */

/** @brief �ļ�ͷƫ�ƶ��� */
#define DF_SIGNATURE_OFF	0
#define DF_SIGNATURE_SIZE	16
#define DF_MAXMP_OFF		(DF_SIGNATURE_OFF+DF_SIGNATURE_SIZE)
#define DF_MAXMP_SIZE		2
#define DF_PIECEPM_OFF		(DF_MAXMP_OFF+DF_MAXMP_SIZE)
#define DF_PIECEPM_SIZE		2
#define DF_RECORDLEN_OFF	(DF_PIECEPM_OFF+DF_PIECEPM_SIZE)
#define DF_RECORDLEN_SIZE	2
#define DF_RECORDSTD_OFF	(DF_RECORDLEN_OFF+DF_RECORDLEN_SIZE)
#define DF_RECORDSTD_SIZE	8
#define DF_RINTERVAL_OFF	(DF_RECORDSTD_OFF+DF_RECORDSTD_SIZE)
#define DF_RINTERVAL_SIZE	1
#define DF_RINTERUNT_OFF	(DF_RINTERVAL_OFF+DF_RINTERVAL_SIZE)
#define DF_RINTERUNT_SIZE	1
#define DF_RESERVE_OFF		(DF_RINTERUNT_OFF+DF_RINTERUNT_SIZE)
#define DF_RESERVE_SIZE		14
#define DF_CHECKSUM_OFF		(DF_RESERVE_OFF+DF_RESERVE_SIZE)
#define DF_CHECKSUM_SIZE	2
#define DF_HEADER_SIZE		(DF_SIGNATURE_SIZE+DF_MAXMP_SIZE+DF_PIECEPM_SIZE+DF_RECORDLEN_SIZE+DF_RECORDSTD_SIZE+DF_RINTERVAL_SIZE+DF_RINTERUNT_SIZE+DF_RESERVE_SIZE+DF_CHECKSUM_SIZE)

/** @brief ����������ƫ�� */
#define DF_EAREA_SIGNATURE_OFF		0
#define DF_EAREA_SIGNATURE_SIZE		10
#define DF_EAREA_MADDR_OFF			(DF_EAREA_SIGNATURE_OFF+DF_EAREA_SIGNATURE_SIZE)
#define DF_EAREA_MADDR_SIZE			6
#define DF_EAREA_QTAIL_OFF			(DF_EAREA_MADDR_OFF+DF_EAREA_MADDR_SIZE)
#define DF_EAREA_QTAIL_SIZE			1
#define DF_EAREA_QHEAD_OFF			(DF_EAREA_QTAIL_OFF+DF_EAREA_QTAIL_SIZE)
#define DF_EAREA_QHEAD_SIZE			1

#define DF_TIME_SIZE			4
//#define DF_DAY_TIME_SIZE		4
//#define DF_CBR_TIME_SIZE		DF_DAY_TIME_SIZE
//#define DF_VIP_TIME_SIZE		DF_DAY_TIME_SIZE
//#define DF_MON_TIME_SIZE		3

#define DF_RECORD_FLAG_SIZE		1
#define DF_RECORD_LEN_SIZE		2

#define DF_EAREA_INFO_SIZE		(DF_EAREA_SIGNATURE_SIZE+DF_EAREA_MADDR_SIZE+DF_EAREA_QTAIL_SIZE+DF_EAREA_QHEAD_SIZE)

/** @brief ��¼������������ */
#define RECORD_STORE_DAY_MAX		31
#define RECORD_STORE_MON_MAX		12
#define RECORD_STORE_VIP_MAX		10
#define RECORD_STORE_CBR_MAX		30

/** @brief ��¼�����λ */
#define RECORD_INTER_UNIT_MIN	('m')
#define RECORD_INTER_UNIT_HOUR	('H')
#define RECORD_INTER_UNIT_DAY	('D')
#define RECORD_INTER_UNIT_MON	('M')

#define RECORD_VALID_FLAG			('R')
#define RECORD_INVALID_FLAG		('N')

/** @brief ͨ�õ������ļ�ͷ�ṹ���� */
struct S_DFHeader
{
	INT8U  m_Signature[DF_SIGNATURE_SIZE]; ///< �����ļ���־��Ŀǰ���ļ���
	INT16U m_MaxMP; ///< ��ǰ�ļ������ܱ�������
	INT16U m_PiecePerMeter; ///< ���ļ���ÿ�������ļ�¼��
	INT16U m_RecordLen; ///< ��¼����
	INT8U  m_RecordInterVal; ///< ��¼ʱ����,�ص㻧��ָ�����ݵļ��
	INT8U  m_RecordInterUnt; ///< ��¼ʱ������λ,�ص㻧��ָ�����ݼ����λ
	S_TIME m_RecordStdT; ///< ��¼�ο�ʱ��
	INT16U m_CheckSum; ///< �ļ�ͷУ��

public:
	/** @brief ������ṹ���ֵ���ļ�ͷ�ṹ��ʽ�������� */
	int OutputBuffer(INT8U *pBuffer, int size);
	/** @brief �ӻ����л�ȡ�ļ�ͷ����ֵ */
	int GetFromBuffer(const INT8U *pBuffer, int size);
	/** @brief ����check sum */
	int CalculateCheckSum(const INT8U *pBuffer, int size);
};

/** @brief ͨ�õ�������������Ϣ�ṹ���� */
struct S_EAreaInfo
{
	INT8U Sign[DF_EAREA_SIGNATURE_SIZE];
	INT8U MAddr[DF_EAREA_MADDR_SIZE];
	INT8U QTail;
	INT8U QHead;

	/** @brief �ӻ����л�ȡ���� */
	bool GetFromBuffer(const char *pbuf);
	/** @brief ������ݵ����� */
	bool OutputBuffer(char *pbuf);
};

/**
 *  @class C_DataFileRW DataFile.h "share/DataFile.h"
 *  @brief the father of data file IO; 
 */
class C_DBRW
{
public:
	enum {DATA_RECORD_LEN_MAX=4096};

public:
	C_DBRW(void){m_VipPointPerDay = 24; m_EnergyDataLen = 0;}
	virtual ~C_DBRW(){}

public:
	/** @brief �����ļ���ʼ�� */
	virtual bool InitDataFile(C_FILE &FileObj, S_WORD FN, S_WORD MaxMP, const S_StoreMax &StoreMax) = 0;
	/** @brief ��ȡһ��д�����ݵĴ�С */
	virtual INT16U GetOnceWriteDataSize(void){return GetRecordLen();}
	/** @brief ��ʱ�䰴�������ͽ��� */
	virtual S_TIME DataTimeTrim(S_TIME tm) = 0;
	/** @brief �ϲ����ݵ���¼ */
	virtual int CombineRecord(string &strRecord, const string &strData, S_TIME DataTime);
	/** @brief ����������ȡ��¼ */
	virtual int ExtractRecord(const string &strRecord, string &strData, S_TIME DataTime);
	/** @brief ����ʱ�� */
	virtual char *MakeTimeTag(char *TimeTag, S_TIME time);
	/** @brief ��¼��С�˶� */
	virtual bool CompareRecordSize(INT16U size);

	/** @brief д��һ�����ݼ�¼ */
	int  Write(C_FILE &FileObj, S_WORD MP, const string &strData, S_TIME DataTime);
	/** @brief ��ȡһ�����ݼ�¼ */
	int  Read(C_FILE &FileObj, S_WORD MP, string &strData, S_TIME DataTime);
	/** @brief �����Ӵ��ļ� */
	bool AppendDataFile(C_FILE &FileObj, INT16U MaxMP);
	/** @brief ��ȡ��¼���� */
	INT16U GetRecordLen(void){return m_Header.m_RecordLen;}
	/** @brief ��ȡ��������� */
	INT16U GetMaxMP(void){return m_Header.m_MaxMP;}
	/** @brief д�����ļ�ͷ��ʶ */
	void InitDataFileSignature(INT8U *pbuff, INT16U FN, char *psign){sprintf((char*)pbuff, "class2f%d_%s", FN, psign);}
	/** @brief д������������ʶ */
	void InitEAreaSignature(INT8U *pbuff, INT16U FN){sprintf((char*)pbuff, "meter%04d", FN);}
	/** @brief ��ȡ�ļ���Ϣ */
	int  GetFileInfoHeader(C_FILE &FileObj);
	/** @brief �����ص㻧FNһ������ݵ��� */
	INT16U SetVipPoint(INT16U VipPointPerDay){m_VipPointPerDay = (VipPointPerDay?VipPointPerDay:m_VipPointPerDay);return m_VipPointPerDay;}
	/** @brief ���ó�ʼ���ݶ� */
	INT16U SetEnergyDataLen(INT16U EnergyDataLen){return m_EnergyDataLen = EnergyDataLen;}
	/** @brief ��ȡ���ݼ�¼��־ */
	INT8U GetDataRecordFlag(char Data){return (INT8U)((Data >> 4) & 0x0F);}
	/** @brief ��ȡ���ݼ�¼���� */
	INT16U GetDataRecordLen(char H, char L){return (((INT16U)(H & 0x0F)) << 8) | (((INT16U)L) & 0xFF);}
	/** @brief ���ü�¼��־�ͳ��� */
	INT16U DataRecordLenFlag(INT8U flag, INT16U len);
	/** @brief ��ȡ������¼����*/
	int GetEnergyRecordSize(void);
	/** @brief ��ȡ��¼,���ؼ�¼�� */
	int FindReocrd(C_FILE &FileObj, string &strRecord, S_TIME DataTime);

protected:
	/** @brief ��ȡFNҪ��ļ�¼���� */
	INT16U GetRecordLenMax(INT16U FN);
	/** @brief reset �����ļ� */ 
	bool ResetDataFile(C_FILE &FileObj);

protected:
	S_DFHeader m_Header;
	INT16U m_VipPointPerDay;
	INT16U m_EnergyDataLen;
};

/**
 *  @class C_DataFileRW_D DataFile.h "share/DataFile.h"
 *  @brief �������ļ�����IO; 
 */
class C_DBRW_D:public C_DBRW
{
public:
	C_DBRW_D(void){}
	~C_DBRW_D(){}

public:
	/** @brief �����ļ���ʼ�� */
	bool InitDataFile(C_FILE &FileObj, S_WORD FN, S_WORD MaxMP, const S_StoreMax &StoreMax);
	/** @brief ��ʱ�䰴�������ͽ��� */
	S_TIME DataTimeTrim(S_TIME tm);
};

/**
 *  @class C_DataFileRW_M DataFile.h "share/DataFile.h"
 *  @brief �������ļ�����IO; 
 */
class C_DBRW_M:public C_DBRW
{
public:
	C_DBRW_M(void){}
	~C_DBRW_M(){}

public:
	/** @brief �����ļ���ʼ�� */
	bool InitDataFile(C_FILE &FileObj, S_WORD FN, S_WORD MaxMP, const S_StoreMax &StoreMax);
	/** @brief ��ʱ�䰴�������ͽ��� */
	S_TIME DataTimeTrim(S_TIME tm);
	/** @brief ����ʱ�� */
	virtual char *MakeTimeTag(char *TimeTag, S_TIME time);
};

/**
 *  @class C_DataFileRW_V DataFile.h "share/DataFile.h"
 *  @brief �ص㻧�����ļ�����IO; 
 */
class C_DBRW_V:public C_DBRW
{
public:
	C_DBRW_V(void){}
	~C_DBRW_V(){}

public:
	/** @brief �����ļ���ʼ�� */
	bool InitDataFile(C_FILE &FileObj, S_WORD FN, S_WORD MaxMP, const S_StoreMax &StoreMax);
	/** @brief ��ȡһ��д�����ݵĴ�С */
	INT16U GetOnceWriteDataSize(void){return (GetRecordLen()/((24*60)/m_Header.m_RecordInterVal)/*VIPDATA_POINT_PERDAY*/);}
	/** @brief ��ʱ�䰴�������ͽ��� */
	S_TIME DataTimeTrim(S_TIME tm);
	/** @brief ��¼��С�˶� */
	bool CompareRecordSize(INT16U size);
	/** @brief �ϲ����ݵ���¼ */
	int CombineRecord(string &strRecord, const string &strData, S_TIME DataTime);
	/** @brief ����������ȡ��¼ */
	int ExtractRecord(const string &strRecord, string &strData, S_TIME DataTime);
};

/**
 *  @class C_DataFileRW_G DataFile.h "share/DataFile.h"
 *  @brief �����������ļ�����IO; 
 */
class C_DBRW_G:public C_DBRW
{
public:
	C_DBRW_G(void){}
	~C_DBRW_G(){}

public:
	/** @brief �����ļ���ʼ�� */
	bool InitDataFile(C_FILE &FileObj, S_WORD FN, S_WORD MaxMP, const S_StoreMax &StoreMax);
	/** @brief ��ʱ�䰴�������ͽ��� */
	S_TIME DataTimeTrim(S_TIME tm);
};

/**
 *  @class C_DBRWFACT DataFile.h "share/DataFile.h"
 *  @brief �����������дʵ������
 */
class C_DBRWFACT
{
public:
	/** @brief ��ȡһ�����ݶ�дʵ�� */
	static C_DBRW *GetDataBlockRW(INT16U FN);
	/** @brief �ͷ�һ�����ݶ�дʵ�� */
	static void FreeDataBlockRW(C_DBRW *pDBRW);
};

/**
 *  @class C_DATAFILE DataFile.h "share/DataFile.h"
 *  @brief General data file OP.
 */
class C_DATAFILE
{
public:
	enum{OPEN_READ, OPEN_WRITE};

public:
	C_DATAFILE(const char *fPath, INT8U io, INT16U FN);
	~C_DATAFILE();

public:
	/** @brief ��ȡһ����¼ */
	int  Read(S_WORD MP, string &strData, S_TIME DataTime);
	/** @brief д��һ����¼ */
	int  Write(S_WORD MP, const string &strData, S_TIME DataTime);
	/** @brief �ж�ʵ���Ƿ�ɲ��� */
	bool operator!(void){return (!m_File || m_pdbrw==NULL);}
	/** @brief �ж��Ƿ��½� */
	bool IsNew(void);
	/** @brief ��ʼ�������ļ� */
	bool InitDataFile(S_WORD MaxMP, const S_StoreMax &StoreMax);
	/** @brief ��ȡ�ļ���Ϣͷ */
	int  GetFileHeader(void);
	/** @brief ��ʱ�䰴�������ͽ��� */
	S_TIME DataTimeTrim(S_TIME tm);
	/** @brief �����ص㻧FNһ������ݵ��� */
	INT16U SetVipPoint(INT16U VipPointPerDay){return m_pdbrw->SetVipPoint(VipPointPerDay);}
	/** @brief ���ó�ʼ���ݶ� */
	INT16U SetEnergyDataLen(INT16U EnergyDataLen){return m_pdbrw->SetEnergyDataLen(EnergyDataLen);}
	/** @brief �ж��Ƿ�Ϊ��ЧFN */
	bool IsInvalidMP(INT16U MP){return (MP>=GW_MP_MAX);}
	/** @brief ��ȡ�ص㻧����ʱ���� */
	int  GetVipFreezeIntervalM(INT8U FreezeCode);
	/** @brief ��¼��С�˶� */
	bool CompareRecordSize(INT16U size);
	/** @���������ļ�*/
	int Backup(const char *pBackupFile);

private:
	C_DATAFILE(void);
	/** @brief �������ļ� */
	bool Open(const char *pfile, INT8U io);
	/** @brief �ر������ļ� */
	bool Close(void);

private:
	INT16U m_FN;
	C_FILE m_File;
	C_DBRW *m_pdbrw;
};

class C_DFOP
{
public:
	C_DFOP(S_WORD DayDataMax, S_WORD MonDataMax, S_WORD VipDataMax, S_WORD CbrDataMax);
	~C_DFOP(){}

	/** @brief ��������д�뺯��
	 *  @param[in] FN ����Ҫ��Ķ�������FN
	 *  @param[in] MP ������
	 *  @param[in] strData ����
	 *  @param[in] DataTime ʱ��
	 *  @param[in] StorePeriod �洢����
	 *  @return <0:ʧ��;=0:�ɹ�
	 */
	int Write(S_WORD FN, S_WORD MP, const string &strData, S_TIME DataTime, S_WORD VipPointPerDay = 24);
	/** @brief �������ݶ�ȡ����
	 *  @param[in] FN ����Ҫ��Ķ�������FN
	 *  @param[in] MP ������
	 *  @param[out] strData ����
	 *  @param[in] DataTime ʱ��
	 *  @return <0:ʧ��;=0:�ɹ�
	 */
	int Read(S_WORD FN, S_WORD MP, string &strData, S_TIME DataTime);

	/** @���ö������ݼ�¼���ȱ�
	 *  @param[in] pClass2FnLen ��ָ��
	 *  @return true
	 *  @note ���û������������Ĭ�ϱ�
	 */
	bool SetRecordLengthTable(const short *pClass2FnLen);

private:
	/** Set data file path */
	char *MakeDataFilePath(INT16U FN);

private:
	char m_FilePath[CFG_PATH_FILE_MAX_LEN];///<�����ļ���
	const short *m_pClass2FnLen;
	S_StoreMax m_StoreMax;
};

#endif //__GXJCB_POWER_FILE_H__



