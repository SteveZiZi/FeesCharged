/**
 *  @file
 *  @brief 告警数据文件存取操作集
 *  @author 
 *  @date 2012-03-19
 *  @version v1.0
 *  @note
 */
#ifndef __NW_ALARM_FILE_BASE_H__
#define __NW_ALARM_FILE_BASE_H__
#include "basedef.h"
#include "strings.h"
#include "fileop.h"
#include "timeop.h"

//
//Alarm file format
//
/** @brief 数据文件格式
 * 文件头格式排列 
 * |文件标识|文件创建时间|存储容量|记录长度|版本信息|保留|校验|
 * |---16---|------8-----|---2----|---2----|---6----|-12-|--2-|
 * 告警索引及标志集(每个单位是5字节的时间标识+1字节的标志)
 * |告警1|告警2|告警3|告警4|.....|
 * |--6--|--6--|--6--|--6--|.....|
 * 告警数据区格式(共有<存储容量>个数据区)
 * |告警数据区标识|告警记录时间|测量点|数据标识|数据长度|告警数据|
 * |-------6------|------5-----|---2--|----4---|----2---|---200--|
 */

//
//
//
struct S_AlarmHeader
{//注意文件头不要超过48字节
	char m_Signature[16];
	char m_timeCreate[8];
	S_WORD m_StoryMax;
	S_WORD m_RecordLen;
	char m_Version[6];
	char m_Reserve[12];
	S_WORD m_CheckSum;

public:
	S_AlarmHeader(void){memset(this, 0x00, sizeof(*this));};
};

//
//
//
class C_AlarmDB
{
protected:
	C_AlarmDB(S_WORD RecordCntMax);
	virtual ~C_AlarmDB();

protected:
	/** @brief 加入记录
	 *  @param[in] MP 测量点
	 *  @param[in] DI 数据标识
	 *  @param[in] strReocrd 告警数据内容
	 *  @param[in] timeRecord 记录时间
	 *  @return -1:出错; >=0:写入的记录条数
	 */
	int AddRecord(S_WORD MP, S_DWORD DI, const string &strReocrd, S_TIME timeRecord);

	/** @brief 读出记录
	 *  @param[in] MP 测量点,通配符0xFFFF
	 *  @param[in] DI 数据标识,通配符0xFFFFFFFF
	 *  @param[out] strReocrds 告警数据内容(排序输出，时间最新的在最前)
	 *  @param[in] timeBegin 开始时间,如为0则取未上报的记录
	 *  @param[in] timeEnd 结束时间，前闭后开
	 *  @return -1:出错; >=0:读出的记录条数
	 */
	int GetRecord(S_WORD MP, S_DWORD DI, strings &strReocrds, S_TIME timeBegin, S_TIME timeEnd);

	/** @brief 统计新告警个数
	 *  @return -1:出错; >=0:新告警个数
	 */
	int GetUnreportCnt(void);

private:
	int InitFile(void);
	int InitAlarmHeader(void);
	int InitDataAreaSing(char *pBuffer, S_WORD i);
	int SerializeAlarmHeader(char *pBuffer);
	int UnserializeAlarmHeader(const char *pBuffer);
	int GetDataAreaSize(void);
	bool CheckFileHeader(void);
	int InsertAlarm(S_WORD MP, S_DWORD DI, const string &strReocrd, S_TIME timeRecord);
	S_WORD FindPos(const char *pIdxArea);
	int CheckUnreoprtCnt(void);
	int ExtractRecord(S_WORD MP, S_DWORD DI, strings &strReocrds, S_TIME timeBegin, S_TIME timeEnd);

	int ReadIdxArea(string &strIdxArea);
	int ReadDataArea(S_WORD pos, string &strDataArea);
	int WriteIdxArea(const string &strIdxArea);
	int WriteDataArea(S_WORD pos, const string &strDataArea);

	bool MatchMP(S_WORD SrcMP, S_WORD DstMP);
	bool MatchDI(S_DWORD SrcDI, S_DWORD DstDI);
	bool MatchTime(S_TIME SrcTime, S_TIME timeBegin, S_TIME timeEnd);

protected:
	S_WORD m_RecordCnt;
	S_CHAR m_FilePath[CFG_PATH_FILE_MAX_LEN];
	S_CHAR m_Signature[16];

private:
	C_FILE m_FileObj;
	S_AlarmHeader m_AlarmHeader;
};


//
//
//
class C_AlarmImportance:public C_AlarmDB
{
public:
	C_AlarmImportance(S_WORD RecordCntMax);
	~C_AlarmImportance();

	/** @brief 加入记录
	 *  @param[in] MP 测量点
	 *  @param[in] DI 数据标识
	 *  @param[in] strReocrd 告警数据内容
	 *  @return -1:出错; >=0:写入的记录条数
	 */
	int AddAlarm(S_WORD MP, S_DWORD DI, const string &strReocrd);

	/** @brief 读出记录
	 *  @param[in] MP 测量点,通配符0xFFFF
	 *  @param[in] DI 数据标识,通配符0xFFFFFFFF
	 *  @param[out] strReocrds 告警数据内容(排序输出，时间最新的在最前)
	 *  @param[in] timeBegin 开始时间,如为0则取未上报的记录
	 *  @param[in] timeEnd 结束时间，前闭后开
	 *  @return -1:出错; >=0:读出的记录条数
	 */
	int GetAlarm(S_WORD MP, S_DWORD DI, strings &strReocrds, S_TIME timeBegin, S_TIME timeEnd);

	/** @brief 统计新告警个数
	 *  @return -1:出错; >=0:新告警个数
	 */
	int GetNewAlarmCnt(void){return GetUnreportCnt();}
};

class C_AlarmNormal:public C_AlarmDB
{
public:
	C_AlarmNormal(S_WORD RecordCntMax);
	~C_AlarmNormal();

	/** @brief 加入记录
	 *  @param[in] MP 测量点
	 *  @param[in] DI 数据标识
	 *  @param[in] strReocrd 告警数据内容
	 *  @return -1:出错; >=0:写入的记录条数
	 */
	int AddAlarm(S_WORD MP, S_DWORD DI, const string &strReocrd);

	/** @brief 读出记录
	 *  @param[in] MP 测量点,通配符0xFFFF
	 *  @param[in] DI 数据标识,通配符0xFFFFFFFF
	 *  @param[out] strReocrds 告警数据内容(排序输出，时间最新的在最前)
	 *  @param[in] timeBegin 开始时间,如为0则取未上报的记录
	 *  @param[in] timeEnd 结束时间，前闭后开
	 *  @return -1:出错; >=0:读出的记录条数
	 */
	int GetAlarm(S_WORD MP, S_DWORD DI, strings &strReocrds, S_TIME timeBegin, S_TIME timeEnd);

	/** @brief 统计新告警个数
	 *  @return -1:出错; >=0:新告警个数
	 */
	int GetNewAlarmCnt(void){return GetUnreportCnt();}
};

class C_EVENT:public C_AlarmDB
{
public:
	C_EVENT(S_WORD RecordCntMax);
	~C_EVENT();

	/** @brief 加入记录
	 *  @param[in] MP 测量点
	 *  @param[in] DI 数据标识
	 *  @param[in] strReocrd 告警数据内容
	 *  @return -1:出错; >=0:写入的记录条数
	 */
	int AddEvent(S_WORD MP, S_DWORD DI, const string &strReocrd);

	/** @brief 读出记录
	 *  @param[in] MP 测量点,通配符0xFFFF
	 *  @param[in] DI 数据标识,通配符0xFFFFFFFF
	 *  @param[out] strReocrds 告警数据内容(排序输出，时间最新的在最前)
	 *  @param[in] timeBegin 开始时间,如为0则取未上报的记录
	 *  @param[in] timeEnd 结束时间，前闭后开
	 *  @return -1:出错; >=0:读出的记录条数
	 */
	int GetEvent(S_WORD MP, S_DWORD DI, strings &strReocrds, S_TIME timeBegin, S_TIME timeEnd);

	/** @brief 统计新告警个数
	 *  @return -1:出错; >=0:新告警个数
	 */
	int GetNewEventCnt(void){return GetUnreportCnt();}
};


#endif //__NW_ALARM_FILE_BASE_H__


