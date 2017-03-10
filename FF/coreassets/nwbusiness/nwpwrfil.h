/**
 *  @file
 *  @brief 集中器能量数据文件存取操作集
 *  @author 
 *  @date 2009-11-24
 *  @version v1.0
 *  @note
 *  1、同一个FN费率须以最大的那个为准，不足部分填充无效数据0xEE。可以依FN不同
 *  2、同一个重点户FN数据点数只能固定为某个数据值（24的倍数）。可以依FN不同
 */
#ifndef __NWB_POWER_FILE_H__
#define __NWB_POWER_FILE_H__
#include "basedef.h"
#include "basetype.h"
#include "nwbtype.h"
#include "fileop.h"

#define CLASS2_FILE_VERSION "1.00"

/** @brief 数据标识类型*/
#define DI_TYPE_VIP		"vip"
#define DI_TYPE_DAY		"day"
#define DI_TYPE_MON		"mon"
#define DI_TYPE_GTH		"gth"

 /** @brief 数据冻结密度
 */
enum S_DataDensity
{
	DATA_DENSITY_TERM = 0,//终端按终端实际存储数据的时间间隔
	DATA_DENSITY_MIN01, // 1 分钟
	DATA_DENSITY_MIN05, // 5 分钟
	DATA_DENSITY_MIN15, // 15 分钟
	DATA_DENSITY_MIN30, // 30 分钟
	DATA_DENSITY_MIN60, // 60 分钟
	DATA_DENSITY_DAY01, // 1日
	DATA_DENSITY_MON01, // 1月

	DATA_DENSITY_MAX
};

/**
 *  @class S_VIPT DataFile.h "share/DataFile.h"
 *  @brief 重点户时标定义
 *  @see 《上行规约》5.13.1.3.2 曲线类数据时标Td_c
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
 *  @brief 费率数据项结构定义
 *  @see 《上行规约》5.13.1.3.2 曲线类数据时标Td_c
 */
struct S_TariffDi
{
	INT32U m_DI;
	INT32U m_BaseLen;
};

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
#define DF_VERSION_OFF		(DF_RINTERUNT_OFF+DF_RINTERUNT_SIZE)
#define DF_VERSION_SIZE		6
#define DF_RESERVE_OFF		(DF_VERSION_OFF+DF_VERSION_SIZE)
#define DF_RESERVE_SIZE		8
#define DF_CHECKSUM_OFF		(DF_RESERVE_OFF+DF_RESERVE_SIZE)
#define DF_CHECKSUM_SIZE	2
#define DF_HEADER_SIZE		(DF_SIGNATURE_SIZE+DF_MAXMP_SIZE+DF_PIECEPM_SIZE+DF_RECORDLEN_SIZE+DF_RECORDSTD_SIZE+DF_RINTERVAL_SIZE+DF_RINTERUNT_SIZE+DF_VERSION_SIZE+DF_RESERVE_SIZE+DF_CHECKSUM_SIZE)

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
//#define DF_DAY_TIME_SIZE		DF_TIME_SIZE
//#define DF_CBR_TIME_SIZE		DF_TIME_SIZE
//#define DF_VIP_TIME_SIZE		DF_TIME_SIZE
//#define DF_MON_TIME_SIZE		3

#define DF_RECORD_FLAG_SIZE		1
#define DF_RECORD_LEN_SIZE		2


#define DF_EAREA_INFO_SIZE		(DF_EAREA_SIGNATURE_SIZE+DF_EAREA_MADDR_SIZE+DF_EAREA_QTAIL_SIZE+DF_EAREA_QHEAD_SIZE)


/** @brief 记录保存条数定义 */
#define RECORD_STORE_DAY_MAX		31
#define RECORD_STORE_MON_MAX		12
#define RECORD_STORE_VIP_MAX		10
#define RECORD_STORE_CBR_MAX		31

/** @brief 记录间隔单位 */
#define RECORD_INTER_UNIT_MIN	('m')
#define RECORD_INTER_UNIT_HOUR	('H')
#define RECORD_INTER_UNIT_DAY	('D')
#define RECORD_INTER_UNIT_MON	('M')

#define RECORD_VALID_FLAG			('R')
#define RECORD_INVALID_FLAG		('N')

#define NW_DATA_RECORD_LEN_MAX	4096

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
	INT8U  m_Version[DF_VERSION_SIZE];
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
	C_DBRW(void){m_DataDensity = DATA_DENSITY_DAY01; m_EnergyDataLen = 0;}
	virtual ~C_DBRW(){}

public:
	/** @brief 数据文件初始化 */
	virtual bool InitDataFile(C_FILE &FileObj, INT32U FN, INT16U MaxMP, S_StoreMax &StoreMax) = 0;
	/** @brief 获取一次写入数据的大小 */
	virtual INT16U GetOnceWriteDataSize(void){return GetRecordLen();}
	/** @brief 将时间按数据类型截齐 */
	virtual S_TIME DataTimeTrim(S_TIME tm) = 0;
	/** @brief 合并数据到记录 */
	virtual int CombineRecord(string &strRecord, const S_EnergyData &EnergyData, S_TIME DataTime);
	/** @brief 从数据区提取记录 */
	virtual int ExtractRecord(const string &strRecord, S_EnergyData &EnergyData, S_TIME DataTime);
	/** @brief 制作时标 */
	virtual char *MakeTimeTag(char *TimeTag, S_TIME time);
	/** @brief 记录大小核对 */
	virtual bool CompareRecordSize(INT16U size);


	/** @brief 读取一条数据记录 */
	int  Read(C_FILE &FileObj, S_EnergyData &EnergyData, S_TIME DataTime);
	/** @brief 写入一条数据记录 */
	int  Write(C_FILE &FileObj, const S_EnergyData &EnergyData, S_TIME DataTime);
	/** @brief 读取数据存储密度函数 */
	S_DataDensity GetDensity(void);
	
	/** @brief 增表后加大文件 */
	bool AppendDataFile(C_FILE &FileObj, INT16U MaxMP);
	/** @brief 获取记录长度 */
	INT16U GetRecordLen(void){return m_Header.m_RecordLen;}
	/** @brief 获取最大测量点号 */
	INT16U GetMaxMP(void){return m_Header.m_MaxMP;}
	/** @brief 写数据文件头标识 */
	void InitDataFileSignature(INT8U *pbuff, INT32U FN, char *psign){sprintf((char*)pbuff, "%s_DI_%08X", psign, (unsigned int)FN);}
	/** @brief 写能量数据区标识 */
	void InitEAreaSignature(INT8U *pbuff, INT16U MP){sprintf((char*)pbuff, "meter%04d", MP);}
	/** @brief 读取文件信息 */
	int  GetFileInfoHeader(C_FILE &FileObj);
	/** @brief 设置重点户FN一天的数据点数 */
	INT16U SetVipPoint(S_DataDensity DataDensity){m_DataDensity = DataDensity; return 0;}
	/** @brief 设置初始数据度 */
	INT16U SetEnergyDataLen(INT16U EnergyDataLen){return m_EnergyDataLen = EnergyDataLen;}
	/** @brief 获取数据记录标志 */
	INT8U GetDataRecordFlag(char Data){return (INT8U)((Data >> 4) & 0x0F);}
	/** @brief 获取数据记录长度 */
	INT16U GetDataRecordLen(char H, char L){return (((INT16U)(H & 0x0F)) << 8) | (((INT16U)L) & 0xFF);}
	/** @brief 设置记录标志和长度 */
	INT16U DataRecordLenFlag(INT8U flag, INT16U len);
	/** @brief 断言文件版本信息 */
	int AssertClass2FileVersion(const char *pExpectedVersion);
	/** @brief 获取能量记录长度*/
	int GetEnergyRecordSize(void);
	/** @brief 提取记录,返回记录号 */
	int FindReocrd(C_FILE &FileObj, string &strRecord, S_TIME DataTime);

protected:
	/** @brief 获取FN要求的记录长度 */
	INT16U GetRecordLenMax(INT32U FN);
	/** @brief reset 数据文件 */ 
	bool ResetDataFile(C_FILE &FileObj);

protected:
	S_DFHeader m_Header;
	S_DataDensity m_DataDensity;
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
	bool InitDataFile(C_FILE &FileObj, INT32U FN, INT16U MaxMP, S_StoreMax &StoreMax);
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
	bool InitDataFile(C_FILE &FileObj, INT32U FN, INT16U MaxMP, S_StoreMax &StoreMax);
	/** @brief 将时间按数据类型截齐 */
	S_TIME DataTimeTrim(S_TIME tm);
	/** @brief 制作时标 */
	char *MakeTimeTag(char *TimeTag, S_TIME time);
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
	bool InitDataFile(C_FILE &FileObj, INT32U FN, INT16U MaxMP, S_StoreMax &StoreMax);
	/** @brief 获取一次写入数据的大小 */
	INT16U GetOnceWriteDataSize(void){return (GetRecordLen()/((24*60)/m_Header.m_RecordInterVal)/*VIPDATA_POINT_PERDAY*/);}
	/** @brief 将时间按数据类型截齐 */
	S_TIME DataTimeTrim(S_TIME tm);
	/** @brief 记录大小核对 */
	bool CompareRecordSize(INT16U size);
	/** @brief 合并数据到记录 */
	int CombineRecord(string &strRecord, const S_EnergyData &EnergyData, S_TIME DataTime);
	/** @brief 从数据区提取记录 */
	int ExtractRecord(const string &strRecord, S_EnergyData &EnergyData, S_TIME DataTime);
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
	bool InitDataFile(C_FILE &FileObj, INT32U FN, INT16U MaxMP, S_StoreMax &StoreMax);
	/** @brief 将时间按数据类型截齐 */
	S_TIME DataTimeTrim(S_TIME tm);
};



/**
 *  @class C_DFRWFACT DataFile.h "share/DataFile.h"
 *  @brief 能量数据类读写实例工厂
 */
class C_DBRWFACT
{
public:
	enum{CLASS2_UNKNOW, CLASS2_VIP, CLASS2_DAY, CLASS2_MON, CLASS2_GTH};
	enum{CLASS2PN_UNKNOW, CLASS2PN_MPOINT, CLASS2PN_TGROUP, CLASS2PN_PORT, CLASS2PN_TERM};
	/** @brief 获取一个数据读写实例 */
	static C_DBRW *GetDataBlockRW(int ClassType);
	/** @brief 释放一个数据读写实例 */
	static void FreeDataBlockRW(C_DBRW *pDBRW);
	/** @brief 获取二类数据FN类型 */
	static int  GetType4Class2FN(S_DataDensity DataDensity);
	/** @brief 获取二类数据FN类型 */
	static int  GetType4Class2FN(char *pFnSign);
	/** @brief  获取FN对应的PN意义*/
	static int GetPnMean4Class2Fn(INT32U FN);
	/** @brief 获取二类数据FN（费率）基础长度 */
	static int  GetDataBaseLen4Class2FN(INT32U FN);
	/** @brief 获取VIP数据每天的点数*/
	static int GetVipPointPerDay(S_DataDensity DataDensity);
	/** @brief 获取重点户冻结时间间隔 */
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
	/** @brief 读取一条记录 */
	int  Read(S_EnergyData &EnergyData, S_TIME DataTime);
	/** @brief 写入一条记录 */
	int  Write(const S_EnergyData &EnergyData, S_TIME DataTime);
	/** @brief 读取数据存储密度函数 */
	int GetDensity(void);

	/** @brief 判断实例是否可操作 */
	bool operator!(void){return !m_File || m_pdbrw == NULL;}
	/** @brief 判断是否新建 */
	bool IsNew(void);
	/** @brief 初始化数据文件 */
	bool InitDataFile(INT16U MaxMP, S_StoreMax &StoreMax);
	/** @brief 获取文件信息头 */
	int  GetFileHeader(void);
	/** @brief 将时间按数据类型截齐 */
	S_TIME DataTimeTrim(S_TIME tm);
	/** @brief 设置重点户FN一天的数据点数 */
	INT16U SetVipPoint(S_DataDensity DataDensity){return m_pdbrw->SetVipPoint(DataDensity);}
	/** @brief 设置初始数据度 */
	INT16U SetEnergyDataLen(INT16U EnergyDataLen){return m_pdbrw->SetEnergyDataLen(EnergyDataLen);}
	/** @brief 判断是否为无效FN */
	bool IsInvalidMP(INT16U MP){return (MP>NW_MP_MAX);}
	/** @brief 记录大小核对 */
	bool CompareRecordSize(INT16U size);
	/** @备份数据文件*/
	int Backup(const char *pBackupFile);

private:
	C_DATAFILE(void);
	/** @brief 获取数据类型 */
	bool GetFileType(char *pBuf4B);
	/** @brief 打开数据文件 */
	bool Open(const char *pfile, INT8U io);
	/** @brief 关闭数据文件 */
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

	/** @brief 二类数据写入函数
	 *  @param[in] FN 国网要求的二类数据FN
	 *  @param[in] MeterDatas 能量数据组，参见S_EnergyDatas定义
	 *  @param[in] DataTime 时标
	 *  @param[in] VipPointPerDay 重点户FN每天须要的数据点数据，仅过重点户FN有效
	 *  @return 写入的记录条数
	 */
	virtual int WriteClass2Data(INT32U FN, const S_EnergyDatas &MeterDatas, S_TIME DataTime, S_DataDensity DataDensity);
	/** @brief 二类数据读取函数
	 *  @param[in] FN 国网要求的二类数据FN
	 *  @param[inout] MeterDatas 能量数据组，参见S_EnergyDatas定义
	 *  @param[in] DataTime 时标
	 *  @return 读取的记录条数
	 */
	virtual int ReadClass2Data(INT32U FN, S_EnergyDatas &MeterDatas, S_TIME DataTime);
	/** @brief 读取数据存储密度函数
	 *  @param[in] FN 数据标识
	 *  @return <=0:读取失败; >0:数据的存储密度
	 */
	virtual int GetDensity(INT32U FN);

private:
	/** Set data file path */
	char *MakeDataFilePath(INT32U FN);

private:
	char m_FilePath[CFG_PATH_FILE_MAX_LEN];///<数据文件名
	S_WORD m_TariffMax;
	S_StoreMax m_StoreMax;
};


#endif//__NWB_POWER_FILE_H__




