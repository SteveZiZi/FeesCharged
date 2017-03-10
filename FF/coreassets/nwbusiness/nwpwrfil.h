/**
 *  @file
 *  @brief ���������������ļ���ȡ������
 *  @author 
 *  @date 2009-11-24
 *  @version v1.0
 *  @note
 *  1��ͬһ��FN�������������Ǹ�Ϊ׼�����㲿�������Ч����0xEE��������FN��ͬ
 *  2��ͬһ���ص㻧FN���ݵ���ֻ�̶ܹ�Ϊĳ������ֵ��24�ı�������������FN��ͬ
 */
#ifndef __NWB_POWER_FILE_H__
#define __NWB_POWER_FILE_H__
#include "basedef.h"
#include "basetype.h"
#include "nwbtype.h"
#include "fileop.h"

#define CLASS2_FILE_VERSION "1.00"

/** @brief ���ݱ�ʶ����*/
#define DI_TYPE_VIP		"vip"
#define DI_TYPE_DAY		"day"
#define DI_TYPE_MON		"mon"
#define DI_TYPE_GTH		"gth"

 /** @brief ���ݶ����ܶ�
 */
enum S_DataDensity
{
	DATA_DENSITY_TERM = 0,//�ն˰��ն�ʵ�ʴ洢���ݵ�ʱ����
	DATA_DENSITY_MIN01, // 1 ����
	DATA_DENSITY_MIN05, // 5 ����
	DATA_DENSITY_MIN15, // 15 ����
	DATA_DENSITY_MIN30, // 30 ����
	DATA_DENSITY_MIN60, // 60 ����
	DATA_DENSITY_DAY01, // 1��
	DATA_DENSITY_MON01, // 1��

	DATA_DENSITY_MAX
};

/**
 *  @class S_VIPT DataFile.h "share/DataFile.h"
 *  @brief �ص㻧ʱ�궨��
 *  @see �����й�Լ��5.13.1.3.2 ����������ʱ��Td_c
 */
struct S_VIPT
{
	S_TIME m_time;
	INT8U  m_DataDots;
	S_DataDensity  m_DataDensity;
};
struct S_DataCond
{
	S_TIME m_timeBegin;
	S_TIME m_timeEnd;
	S_DataDensity  m_DataDensity;
};

/**
 *  @class S_DiTraffic DataFile.h "share/DataFile.h"
 *  @brief ����������ṹ����
 *  @see �����й�Լ��5.13.1.3.2 ����������ʱ��Td_c
 */
struct S_TariffDi
{
	INT32U m_DI;
	INT32U m_BaseLen;
};

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
#define DF_VERSION_OFF		(DF_RINTERUNT_OFF+DF_RINTERUNT_SIZE)
#define DF_VERSION_SIZE		6
#define DF_RESERVE_OFF		(DF_VERSION_OFF+DF_VERSION_SIZE)
#define DF_RESERVE_SIZE		8
#define DF_CHECKSUM_OFF		(DF_RESERVE_OFF+DF_RESERVE_SIZE)
#define DF_CHECKSUM_SIZE	2
#define DF_HEADER_SIZE		(DF_SIGNATURE_SIZE+DF_MAXMP_SIZE+DF_PIECEPM_SIZE+DF_RECORDLEN_SIZE+DF_RECORDSTD_SIZE+DF_RINTERVAL_SIZE+DF_RINTERUNT_SIZE+DF_VERSION_SIZE+DF_RESERVE_SIZE+DF_CHECKSUM_SIZE)

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
//#define DF_DAY_TIME_SIZE		DF_TIME_SIZE
//#define DF_CBR_TIME_SIZE		DF_TIME_SIZE
//#define DF_VIP_TIME_SIZE		DF_TIME_SIZE
//#define DF_MON_TIME_SIZE		3

#define DF_RECORD_FLAG_SIZE		1
#define DF_RECORD_LEN_SIZE		2


#define DF_EAREA_INFO_SIZE		(DF_EAREA_SIGNATURE_SIZE+DF_EAREA_MADDR_SIZE+DF_EAREA_QTAIL_SIZE+DF_EAREA_QHEAD_SIZE)


/** @brief ��¼������������ */
#define RECORD_STORE_DAY_MAX		31
#define RECORD_STORE_MON_MAX		12
#define RECORD_STORE_VIP_MAX		10
#define RECORD_STORE_CBR_MAX		31

/** @brief ��¼�����λ */
#define RECORD_INTER_UNIT_MIN	('m')
#define RECORD_INTER_UNIT_HOUR	('H')
#define RECORD_INTER_UNIT_DAY	('D')
#define RECORD_INTER_UNIT_MON	('M')

#define RECORD_VALID_FLAG			('R')
#define RECORD_INVALID_FLAG		('N')

#define NW_DATA_RECORD_LEN_MAX	4096

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
	INT8U  m_Version[DF_VERSION_SIZE];
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
	C_DBRW(void){m_DataDensity = DATA_DENSITY_DAY01; m_EnergyDataLen = 0;}
	virtual ~C_DBRW(){}

public:
	/** @brief �����ļ���ʼ�� */
	virtual bool InitDataFile(C_FILE &FileObj, INT32U FN, INT16U MaxMP, S_StoreMax &StoreMax) = 0;
	/** @brief ��ȡһ��д�����ݵĴ�С */
	virtual INT16U GetOnceWriteDataSize(void){return GetRecordLen();}
	/** @brief ��ʱ�䰴�������ͽ��� */
	virtual S_TIME DataTimeTrim(S_TIME tm) = 0;
	/** @brief �ϲ����ݵ���¼ */
	virtual int CombineRecord(string &strRecord, const S_EnergyData &EnergyData, S_TIME DataTime);
	/** @brief ����������ȡ��¼ */
	virtual int ExtractRecord(const string &strRecord, S_EnergyData &EnergyData, S_TIME DataTime);
	/** @brief ����ʱ�� */
	virtual char *MakeTimeTag(char *TimeTag, S_TIME time);
	/** @brief ��¼��С�˶� */
	virtual bool CompareRecordSize(INT16U size);


	/** @brief ��ȡһ�����ݼ�¼ */
	int  Read(C_FILE &FileObj, S_EnergyData &EnergyData, S_TIME DataTime);
	/** @brief д��һ�����ݼ�¼ */
	int  Write(C_FILE &FileObj, const S_EnergyData &EnergyData, S_TIME DataTime);
	/** @brief ��ȡ���ݴ洢�ܶȺ��� */
	S_DataDensity GetDensity(void);
	
	/** @brief �����Ӵ��ļ� */
	bool AppendDataFile(C_FILE &FileObj, INT16U MaxMP);
	/** @brief ��ȡ��¼���� */
	INT16U GetRecordLen(void){return m_Header.m_RecordLen;}
	/** @brief ��ȡ��������� */
	INT16U GetMaxMP(void){return m_Header.m_MaxMP;}
	/** @brief д�����ļ�ͷ��ʶ */
	void InitDataFileSignature(INT8U *pbuff, INT32U FN, char *psign){sprintf((char*)pbuff, "%s_DI_%08X", psign, (unsigned int)FN);}
	/** @brief д������������ʶ */
	void InitEAreaSignature(INT8U *pbuff, INT16U MP){sprintf((char*)pbuff, "meter%04d", MP);}
	/** @brief ��ȡ�ļ���Ϣ */
	int  GetFileInfoHeader(C_FILE &FileObj);
	/** @brief �����ص㻧FNһ������ݵ��� */
	INT16U SetVipPoint(S_DataDensity DataDensity){m_DataDensity = DataDensity; return 0;}
	/** @brief ���ó�ʼ���ݶ� */
	INT16U SetEnergyDataLen(INT16U EnergyDataLen){return m_EnergyDataLen = EnergyDataLen;}
	/** @brief ��ȡ���ݼ�¼��־ */
	INT8U GetDataRecordFlag(char Data){return (INT8U)((Data >> 4) & 0x0F);}
	/** @brief ��ȡ���ݼ�¼���� */
	INT16U GetDataRecordLen(char H, char L){return (((INT16U)(H & 0x0F)) << 8) | (((INT16U)L) & 0xFF);}
	/** @brief ���ü�¼��־�ͳ��� */
	INT16U DataRecordLenFlag(INT8U flag, INT16U len);
	/** @brief �����ļ��汾��Ϣ */
	int AssertClass2FileVersion(const char *pExpectedVersion);
	/** @brief ��ȡ������¼����*/
	int GetEnergyRecordSize(void);
	/** @brief ��ȡ��¼,���ؼ�¼�� */
	int FindReocrd(C_FILE &FileObj, string &strRecord, S_TIME DataTime);

protected:
	/** @brief ��ȡFNҪ��ļ�¼���� */
	INT16U GetRecordLenMax(INT32U FN);
	/** @brief reset �����ļ� */ 
	bool ResetDataFile(C_FILE &FileObj);

protected:
	S_DFHeader m_Header;
	S_DataDensity m_DataDensity;
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
	bool InitDataFile(C_FILE &FileObj, INT32U FN, INT16U MaxMP, S_StoreMax &StoreMax);
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
	bool InitDataFile(C_FILE &FileObj, INT32U FN, INT16U MaxMP, S_StoreMax &StoreMax);
	/** @brief ��ʱ�䰴�������ͽ��� */
	S_TIME DataTimeTrim(S_TIME tm);
	/** @brief ����ʱ�� */
	char *MakeTimeTag(char *TimeTag, S_TIME time);
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
	bool InitDataFile(C_FILE &FileObj, INT32U FN, INT16U MaxMP, S_StoreMax &StoreMax);
	/** @brief ��ȡһ��д�����ݵĴ�С */
	INT16U GetOnceWriteDataSize(void){return (GetRecordLen()/((24*60)/m_Header.m_RecordInterVal)/*VIPDATA_POINT_PERDAY*/);}
	/** @brief ��ʱ�䰴�������ͽ��� */
	S_TIME DataTimeTrim(S_TIME tm);
	/** @brief ��¼��С�˶� */
	bool CompareRecordSize(INT16U size);
	/** @brief �ϲ����ݵ���¼ */
	int CombineRecord(string &strRecord, const S_EnergyData &EnergyData, S_TIME DataTime);
	/** @brief ����������ȡ��¼ */
	int ExtractRecord(const string &strRecord, S_EnergyData &EnergyData, S_TIME DataTime);
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
	bool InitDataFile(C_FILE &FileObj, INT32U FN, INT16U MaxMP, S_StoreMax &StoreMax);
	/** @brief ��ʱ�䰴�������ͽ��� */
	S_TIME DataTimeTrim(S_TIME tm);
};



/**
 *  @class C_DFRWFACT DataFile.h "share/DataFile.h"
 *  @brief �����������дʵ������
 */
class C_DBRWFACT
{
public:
	enum{CLASS2_UNKNOW, CLASS2_VIP, CLASS2_DAY, CLASS2_MON, CLASS2_GTH};
	enum{CLASS2PN_UNKNOW, CLASS2PN_MPOINT, CLASS2PN_TGROUP, CLASS2PN_PORT, CLASS2PN_TERM};
	/** @brief ��ȡһ�����ݶ�дʵ�� */
	static C_DBRW *GetDataBlockRW(int ClassType);
	/** @brief �ͷ�һ�����ݶ�дʵ�� */
	static void FreeDataBlockRW(C_DBRW *pDBRW);
	/** @brief ��ȡ��������FN���� */
	static int  GetType4Class2FN(S_DataDensity DataDensity);
	/** @brief ��ȡ��������FN���� */
	static int  GetType4Class2FN(char *pFnSign);
	/** @brief  ��ȡFN��Ӧ��PN����*/
	static int GetPnMean4Class2Fn(INT32U FN);
	/** @brief ��ȡ��������FN�����ʣ��������� */
	static int  GetDataBaseLen4Class2FN(INT32U FN);
	/** @brief ��ȡVIP����ÿ��ĵ���*/
	static int GetVipPointPerDay(S_DataDensity DataDensity);
	/** @brief ��ȡ�ص㻧����ʱ���� */
	static int  GetVipFreezeIntervalM(S_DataDensity DataDensity);
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
	C_DATAFILE(const char *fPath, INT8U io, INT32U FN, S_DataDensity DataDensity);
	C_DATAFILE(const char *fPath, INT8U io, INT32U FN);
	~C_DATAFILE();

public:
	/** @brief ��ȡһ����¼ */
	int  Read(S_EnergyData &EnergyData, S_TIME DataTime);
	/** @brief д��һ����¼ */
	int  Write(const S_EnergyData &EnergyData, S_TIME DataTime);
	/** @brief ��ȡ���ݴ洢�ܶȺ��� */
	int GetDensity(void);

	/** @brief �ж�ʵ���Ƿ�ɲ��� */
	bool operator!(void){return !m_File || m_pdbrw == NULL;}
	/** @brief �ж��Ƿ��½� */
	bool IsNew(void);
	/** @brief ��ʼ�������ļ� */
	bool InitDataFile(INT16U MaxMP, S_StoreMax &StoreMax);
	/** @brief ��ȡ�ļ���Ϣͷ */
	int  GetFileHeader(void);
	/** @brief ��ʱ�䰴�������ͽ��� */
	S_TIME DataTimeTrim(S_TIME tm);
	/** @brief �����ص㻧FNһ������ݵ��� */
	INT16U SetVipPoint(S_DataDensity DataDensity){return m_pdbrw->SetVipPoint(DataDensity);}
	/** @brief ���ó�ʼ���ݶ� */
	INT16U SetEnergyDataLen(INT16U EnergyDataLen){return m_pdbrw->SetEnergyDataLen(EnergyDataLen);}
	/** @brief �ж��Ƿ�Ϊ��ЧFN */
	bool IsInvalidMP(INT16U MP){return (MP>NW_MP_MAX);}
	/** @brief ��¼��С�˶� */
	bool CompareRecordSize(INT16U size);
	/** @���������ļ�*/
	int Backup(const char *pBackupFile);

private:
	C_DATAFILE(void);
	/** @brief ��ȡ�������� */
	bool GetFileType(char *pBuf4B);
	/** @brief �������ļ� */
	bool Open(const char *pfile, INT8U io);
	/** @brief �ر������ļ� */
	bool Close(void);

private:
	INT32U m_FN;
	C_FILE m_File;
	C_DBRW *m_pdbrw;
};

class C_DFOP
{
public:
	C_DFOP(S_WORD DayDataMax, S_WORD MonDataMax, S_WORD VipDataMax, S_WORD CbrDataMax, S_WORD TariffMax);
	virtual ~C_DFOP(){}

	/** @brief ��������д�뺯��
	 *  @param[in] FN ����Ҫ��Ķ�������FN
	 *  @param[in] MeterDatas ���������飬�μ�S_EnergyDatas����
	 *  @param[in] DataTime ʱ��
	 *  @param[in] VipPointPerDay �ص㻧FNÿ����Ҫ�����ݵ����ݣ������ص㻧FN��Ч
	 *  @return д��ļ�¼����
	 */
	virtual int WriteClass2Data(INT32U FN, const S_EnergyDatas &MeterDatas, S_TIME DataTime, S_DataDensity DataDensity);
	/** @brief �������ݶ�ȡ����
	 *  @param[in] FN ����Ҫ��Ķ�������FN
	 *  @param[inout] MeterDatas ���������飬�μ�S_EnergyDatas����
	 *  @param[in] DataTime ʱ��
	 *  @return ��ȡ�ļ�¼����
	 */
	virtual int ReadClass2Data(INT32U FN, S_EnergyDatas &MeterDatas, S_TIME DataTime);
	/** @brief ��ȡ���ݴ洢�ܶȺ���
	 *  @param[in] FN ���ݱ�ʶ
	 *  @return <=0:��ȡʧ��; >0:���ݵĴ洢�ܶ�
	 */
	virtual int GetDensity(INT32U FN);

private:
	/** Set data file path */
	char *MakeDataFilePath(INT32U FN);

private:
	char m_FilePath[CFG_PATH_FILE_MAX_LEN];///<�����ļ���
	S_WORD m_TariffMax;
	S_StoreMax m_StoreMax;
};


#endif//__NWB_POWER_FILE_H__




