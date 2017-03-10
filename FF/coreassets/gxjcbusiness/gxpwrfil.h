/**
 *  @file DataFile.h 
 *  @brief 集中器能量数据文件存取操作集
 *  @author 
 *  @date 2009-11-24
 *  @version v1.0
 *  @note
 *  1、同一个FN费率须以最大的那个为准，不足部分填充无效数据0xEE。可以依FN不同
 *  2、同一个重点户FN数据点数只能固定为某个数据值（24的倍数）。可以依FN不同
 */
#ifndef __GXJCB_POWER_FILE_H__
#define __GXJCB_POWER_FILE_H__
#include "basedef.h"
#include "basetype.h"
#include "timeop.h"
#include "fileop.h"


#define GW_MP_MAX	BASEB_MP_COUNT_MAX
#define GW_TARIFF_MAX	12

/** @brief 数据文件格式
 * 文件头格式排列 
 * |文件标识|最大测量点号|电表保存记录数|记录长度|
 * |---16---|------2-----|-------2------|---2----|
 * |记录标准时间|记录间隔时间|记录间隔单位|保留|校验|
 * |-----8------|------1-----|------1-----|-14-|--2-|
 * //数据文件头共48个字节，考虑了适当的冗余
 *
 * 能量数据区格式
 * |能量数据区标识|电能表地址|队尾|队头|
 * |-------10-----|-----6----|--1-|--1-|
 * |记录时标|开始标志|数据标志/记录长度|记录长度|记录1能量数据|
 * |--3or4--|----1---|---4b-------4b---|---1----|------N------|
 * |记录时标|开始标志|数据标志/记录长度|记录长度|记录2能量数据|
 * |--3or4--|----1---|---4b-------4b---|---1----|------N------|
 * |记录时标|开始标志|数据标志/记录长度|记录长度|记录3能量数据|
 * |--3or4--|----1---|---4b-------4b---|---1----|------N------|
 * //共有“最大测量点号”个能量数据区
 */

/** @brief 文件头偏移定义 */
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

/** @brief 能量数据区偏移 */
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

/** @brief 记录保存条数定义 */
#define RECORD_STORE_DAY_MAX		31
#define RECORD_STORE_MON_MAX		12
#define RECORD_STORE_VIP_MAX		10
#define RECORD_STORE_CBR_MAX		30

/** @brief 记录间隔单位 */
#define RECORD_INTER_UNIT_MIN	('m')
#define RECORD_INTER_UNIT_HOUR	('H')
#define RECORD_INTER_UNIT_DAY	('D')
#define RECORD_INTER_UNIT_MON	('M')

#define RECORD_VALID_FLAG			('R')
#define RECORD_INVALID_FLAG		('N')

/** @brief 通用的数据文件头结构定义 */
struct S_DFHeader
{
	INT8U  m_Signature[DF_SIGNATURE_SIZE]; ///< 数据文件标志，目前存文件名
	INT16U m_MaxMP; ///< 当前文件最大电能表测量点号
	INT16U m_PiecePerMeter; ///< 本文件中每块电表保留的记录数
	INT16U m_RecordLen; ///< 记录长度
	INT8U  m_RecordInterVal; ///< 记录时间间隔,重点户中指点数据的间隔
	INT8U  m_RecordInterUnt; ///< 记录时间间隔单位,重点户中指点数据间隔单位
	S_TIME m_RecordStdT; ///< 记录参考时间
	INT16U m_CheckSum; ///< 文件头校验

public:
	/** @brief 输出本结构体的值按文件头结构形式到缓冲中 */
	int OutputBuffer(INT8U *pBuffer, int size);
	/** @brief 从缓冲中获取文件头的数值 */
	int GetFromBuffer(const INT8U *pBuffer, int size);
	/** @brief 计算check sum */
	int CalculateCheckSum(const INT8U *pBuffer, int size);
};

/** @brief 通用的能量数据区信息结构定义 */
struct S_EAreaInfo
{
	INT8U Sign[DF_EAREA_SIGNATURE_SIZE];
	INT8U MAddr[DF_EAREA_MADDR_SIZE];
	INT8U QTail;
	INT8U QHead;

	/** @brief 从缓冲中获取数据 */
	bool GetFromBuffer(const char *pbuf);
	/** @brief 输出数据到缓冲 */
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
	/** @brief 数据文件初始化 */
	virtual bool InitDataFile(C_FILE &FileObj, S_WORD FN, S_WORD MaxMP, const S_StoreMax &StoreMax) = 0;
	/** @brief 获取一次写入数据的大小 */
	virtual INT16U GetOnceWriteDataSize(void){return GetRecordLen();}
	/** @brief 将时间按数据类型截齐 */
	virtual S_TIME DataTimeTrim(S_TIME tm) = 0;
	/** @brief 合并数据到记录 */
	virtual int CombineRecord(string &strRecord, const string &strData, S_TIME DataTime);
	/** @brief 从数据区提取记录 */
	virtual int ExtractRecord(const string &strRecord, string &strData, S_TIME DataTime);
	/** @brief 制作时标 */
	virtual char *MakeTimeTag(char *TimeTag, S_TIME time);
	/** @brief 记录大小核对 */
	virtual bool CompareRecordSize(INT16U size);

	/** @brief 写入一条数据记录 */
	int  Write(C_FILE &FileObj, S_WORD MP, const string &strData, S_TIME DataTime);
	/** @brief 读取一条数据记录 */
	int  Read(C_FILE &FileObj, S_WORD MP, string &strData, S_TIME DataTime);
	/** @brief 增表后加大文件 */
	bool AppendDataFile(C_FILE &FileObj, INT16U MaxMP);
	/** @brief 获取记录长度 */
	INT16U GetRecordLen(void){return m_Header.m_RecordLen;}
	/** @brief 获取最大测量点号 */
	INT16U GetMaxMP(void){return m_Header.m_MaxMP;}
	/** @brief 写数据文件头标识 */
	void InitDataFileSignature(INT8U *pbuff, INT16U FN, char *psign){sprintf((char*)pbuff, "class2f%d_%s", FN, psign);}
	/** @brief 写能量数据区标识 */
	void InitEAreaSignature(INT8U *pbuff, INT16U FN){sprintf((char*)pbuff, "meter%04d", FN);}
	/** @brief 读取文件信息 */
	int  GetFileInfoHeader(C_FILE &FileObj);
	/** @brief 设置重点户FN一天的数据点数 */
	INT16U SetVipPoint(INT16U VipPointPerDay){m_VipPointPerDay = (VipPointPerDay?VipPointPerDay:m_VipPointPerDay);return m_VipPointPerDay;}
	/** @brief 设置初始数据度 */
	INT16U SetEnergyDataLen(INT16U EnergyDataLen){return m_EnergyDataLen = EnergyDataLen;}
	/** @brief 获取数据记录标志 */
	INT8U GetDataRecordFlag(char Data){return (INT8U)((Data >> 4) & 0x0F);}
	/** @brief 获取数据记录长度 */
	INT16U GetDataRecordLen(char H, char L){return (((INT16U)(H & 0x0F)) << 8) | (((INT16U)L) & 0xFF);}
	/** @brief 设置记录标志和长度 */
	INT16U DataRecordLenFlag(INT8U flag, INT16U len);
	/** @brief 获取能量记录长度*/
	int GetEnergyRecordSize(void);
	/** @brief 提取记录,返回记录号 */
	int FindReocrd(C_FILE &FileObj, string &strRecord, S_TIME DataTime);

protected:
	/** @brief 获取FN要求的记录长度 */
	INT16U GetRecordLenMax(INT16U FN);
	/** @brief reset 数据文件 */ 
	bool ResetDataFile(C_FILE &FileObj);

protected:
	S_DFHeader m_Header;
	INT16U m_VipPointPerDay;
	INT16U m_EnergyDataLen;
};

/**
 *  @class C_DataFileRW_D DataFile.h "share/DataFile.h"
 *  @brief 日数据文件操作IO; 
 */
class C_DBRW_D:public C_DBRW
{
public:
	C_DBRW_D(void){}
	~C_DBRW_D(){}

public:
	/** @brief 数据文件初始化 */
	bool InitDataFile(C_FILE &FileObj, S_WORD FN, S_WORD MaxMP, const S_StoreMax &StoreMax);
	/** @brief 将时间按数据类型截齐 */
	S_TIME DataTimeTrim(S_TIME tm);
};

/**
 *  @class C_DataFileRW_M DataFile.h "share/DataFile.h"
 *  @brief 月数据文件操作IO; 
 */
class C_DBRW_M:public C_DBRW
{
public:
	C_DBRW_M(void){}
	~C_DBRW_M(){}

public:
	/** @brief 数据文件初始化 */
	bool InitDataFile(C_FILE &FileObj, S_WORD FN, S_WORD MaxMP, const S_StoreMax &StoreMax);
	/** @brief 将时间按数据类型截齐 */
	S_TIME DataTimeTrim(S_TIME tm);
	/** @brief 制作时标 */
	virtual char *MakeTimeTag(char *TimeTag, S_TIME time);
};

/**
 *  @class C_DataFileRW_V DataFile.h "share/DataFile.h"
 *  @brief 重点户数据文件操作IO; 
 */
class C_DBRW_V:public C_DBRW
{
public:
	C_DBRW_V(void){}
	~C_DBRW_V(){}

public:
	/** @brief 数据文件初始化 */
	bool InitDataFile(C_FILE &FileObj, S_WORD FN, S_WORD MaxMP, const S_StoreMax &StoreMax);
	/** @brief 获取一次写入数据的大小 */
	INT16U GetOnceWriteDataSize(void){return (GetRecordLen()/((24*60)/m_Header.m_RecordInterVal)/*VIPDATA_POINT_PERDAY*/);}
	/** @brief 将时间按数据类型截齐 */
	S_TIME DataTimeTrim(S_TIME tm);
	/** @brief 记录大小核对 */
	bool CompareRecordSize(INT16U size);
	/** @brief 合并数据到记录 */
	int CombineRecord(string &strRecord, const string &strData, S_TIME DataTime);
	/** @brief 从数据区提取记录 */
	int ExtractRecord(const string &strRecord, string &strData, S_TIME DataTime);
};

/**
 *  @class C_DataFileRW_G DataFile.h "share/DataFile.h"
 *  @brief 抄表日数据文件操作IO; 
 */
class C_DBRW_G:public C_DBRW
{
public:
	C_DBRW_G(void){}
	~C_DBRW_G(){}

public:
	/** @brief 数据文件初始化 */
	bool InitDataFile(C_FILE &FileObj, S_WORD FN, S_WORD MaxMP, const S_StoreMax &StoreMax);
	/** @brief 将时间按数据类型截齐 */
	S_TIME DataTimeTrim(S_TIME tm);
};

/**
 *  @class C_DBRWFACT DataFile.h "share/DataFile.h"
 *  @brief 能量数据类读写实例工厂
 */
class C_DBRWFACT
{
public:
	/** @brief 获取一个数据读写实例 */
	static C_DBRW *GetDataBlockRW(INT16U FN);
	/** @brief 释放一个数据读写实例 */
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
	/** @brief 读取一条记录 */
	int  Read(S_WORD MP, string &strData, S_TIME DataTime);
	/** @brief 写入一条记录 */
	int  Write(S_WORD MP, const string &strData, S_TIME DataTime);
	/** @brief 判断实例是否可操作 */
	bool operator!(void){return (!m_File || m_pdbrw==NULL);}
	/** @brief 判断是否新建 */
	bool IsNew(void);
	/** @brief 初始化数据文件 */
	bool InitDataFile(S_WORD MaxMP, const S_StoreMax &StoreMax);
	/** @brief 获取文件信息头 */
	int  GetFileHeader(void);
	/** @brief 将时间按数据类型截齐 */
	S_TIME DataTimeTrim(S_TIME tm);
	/** @brief 设置重点户FN一天的数据点数 */
	INT16U SetVipPoint(INT16U VipPointPerDay){return m_pdbrw->SetVipPoint(VipPointPerDay);}
	/** @brief 设置初始数据度 */
	INT16U SetEnergyDataLen(INT16U EnergyDataLen){return m_pdbrw->SetEnergyDataLen(EnergyDataLen);}
	/** @brief 判断是否为无效FN */
	bool IsInvalidMP(INT16U MP){return (MP>=GW_MP_MAX);}
	/** @brief 获取重点户冻结时间间隔 */
	int  GetVipFreezeIntervalM(INT8U FreezeCode);
	/** @brief 记录大小核对 */
	bool CompareRecordSize(INT16U size);
	/** @备份数据文件*/
	int Backup(const char *pBackupFile);

private:
	C_DATAFILE(void);
	/** @brief 打开数据文件 */
	bool Open(const char *pfile, INT8U io);
	/** @brief 关闭数据文件 */
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

	/** @brief 二类数据写入函数
	 *  @param[in] FN 国网要求的二类数据FN
	 *  @param[in] MP 测量点
	 *  @param[in] strData 数据
	 *  @param[in] DataTime 时标
	 *  @param[in] StorePeriod 存储周期
	 *  @return <0:失败;=0:成功
	 */
	int Write(S_WORD FN, S_WORD MP, const string &strData, S_TIME DataTime, S_WORD VipPointPerDay = 24);
	/** @brief 二类数据读取函数
	 *  @param[in] FN 国网要求的二类数据FN
	 *  @param[in] MP 测量点
	 *  @param[out] strData 数据
	 *  @param[in] DataTime 时标
	 *  @return <0:失败;=0:成功
	 */
	int Read(S_WORD FN, S_WORD MP, string &strData, S_TIME DataTime);

	/** @设置二类数据记录长度表
	 *  @param[in] pClass2FnLen 表指针
	 *  @return true
	 *  @note 如用户不设置则采用默认表
	 */
	bool SetRecordLengthTable(const short *pClass2FnLen);

private:
	/** Set data file path */
	char *MakeDataFilePath(INT16U FN);

private:
	char m_FilePath[CFG_PATH_FILE_MAX_LEN];///<数据文件名
	const short *m_pClass2FnLen;
	S_StoreMax m_StoreMax;
};

#endif //__GXJCB_POWER_FILE_H__



