/** @file
 *  @brief 数据文件用户层接口
 *  @author 
 *  @date 2012/03/21
 *  @version 0.1
 */
#ifndef __NW_DATA_FILE_H__
#define __NW_DATA_FILE_H__
#include "basedef.h"
#include "nwbtype.h"
#include "nwalmfil.h"
#include "nwpwrfil.h"

enum S_AlmType
{
	ALM_TYPE_UNKNOW,
	ALM_TYPE_EVENT,
	ALM_TYPE_ALARM,
	ALM_TYPE_NORMAL,
	ALM_TYPE_IMPORTANT,
	ALM_TYPE_UNRECORD_ALARM,
	ALM_TYPE_RECORD_ALARM,

	ALM_TYPE_MAX
};

#define DI_ALM_EVENT_MIN	0xE2010001UL
#define DI_ALM_EVENT_MAX	0xE2010015UL
#define DI_ALM_EVENT_ALL	0xE20100FFUL

#define DI_ALM_ALARM_MIN	0xE2000001UL
#define DI_ALM_ALARM_MAX	0xE200005AUL
#define DI_ALM_ALARM_UNREPORT	0xE20010FFUL
#define DI_ALM_ALARM_ALL	0xE200FFFFUL

enum
{
	NW_DATA_ID_TYPE_TARIFF,
	NW_DATA_ID_TYPE_SUM_Y,
	NW_DATA_ID_TYPE_SUM_N,

	NW_DATA_ID_TYPE_MAX
};
struct S_NwDataID
{
	S_DWORD m_DI; ///< 数据项ID
	S_BYTE m_type; ///< 数据项类型 NW_DATA_ID_TYPE
	S_BYTE m_ItemLen; ///< 数据块子项长度
};
bool operator<(const S_NwDataID &left, const S_NwDataID &right);

//
// 用户接口定义
//
/**
 *  @class 
 *  @brief the interface of energy data file IO.
 */
class C_ENERGY
{
public:

	/** @brief 二类数据写入函数
	 *  @param[in] FN 数据标识
	 *  @param[in] MP 测量点
	 *  @param[in] strData 数据记录
	 *  @param[in] DataTime 时标
	 *  @param[in] DataDensity 冻结密度
	 *  @return <0:失败;=0:成功
	 */
	static int Write(S_DWORD FN, S_WORD MP, const string &strData, S_TIME DataTime, S_DataDensity DataDensity);
	/** @brief 二类数据写入函数
	 *  @param[in] FN 数据标识
	 *  @param[in] MeterDatas 能量数据组，参见S_EnergyDatas定义
	 *  @param[in] DataTime 时标
	 *  @param[in] DataDensity 冻结密度
	 *  @return 写入的记录条数
	 */
	static int WriteClass2Data(INT32U FN, const S_EnergyDatas &MeterDatas, S_TIME DataTime, S_DataDensity DataDensity);
	/** @brief 二类数据读取函数
	 *  @param[in] MP 数据标识
	 *  @param[in] FN 数据标识
	 *  @param[in] DataTime 时标
	 *  @param[out] strData 能量数据
	 *  @return 读取的记录条数
	 */
	static int ReadClass2Data(S_WORD MP, INT32U FN, S_TIME DataTime, string &strData);
	/** @brief 二类数据读取函数
	 *  @param[in] FN 数据标识
	 *  @param[inout] MeterDatas 能量数据组，参见S_EnergyDatas定义
	 *  @param[in] DataTime 时标
	 *  @return 读取的记录条数
	 */
	static int ReadClass2Data(INT32U FN, S_EnergyDatas &MeterDatas, S_TIME DataTime);

	/** @brief 读取数据存储密度函数
	 *  @param[in] FN 数据标识
	 *  @return <=0:读取失败; >0:数据的存储密度
	 */
	static int GetDensity(INT32U FN);
};

class C_Class2Data
{
public:
	/** @brief 二类数据写入函数
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[in] pTaskCtrlStatus 任务控制信息
	 *  @param[in] FreezeTime 数据冻结时间
	 *  @return <0:失败;=0:成功
	 */
	static int Write(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus, S_TIME FreezeTime);

	/** @brief 二类数据写入函数
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[in] pTaskCtrlStatus 任务控制信息
	 *  @param[in] FreezeTime 数据冻结时间
	 *  @return <0:失败;=0:成功
	 */
	static int WriteTermData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus, S_TIME FreezeTime);

	/** @brief 二类数据读取函数
	 *  @param[in] FN 数据标识
	 *  @param[in] MP 测量点
	 *  @param[out] strData 数据输出缓冲
	 *  @param[in] DataTime 时标
	 *  @return <0:失败;=0:成功
	 */
	static int Read(S_DWORD FN, S_WORD MP, string &strData, S_TIME DataTime);

	/** @brief 读取数据存储密度函数
	 *  @param[in] FN 数据标识
	 *  @return <=0:读取失败; >0:数据的存储密度
	 */
	static int GetDensity(INT32U FN);

private:
	/** @brief 查找备用数据项
	 */
	static bool FindReserveDataId(S_DWORD FN, S_NwDataID &NwDataId);
	static bool _FindReserveDataId(S_DWORD FN, S_NwDataID &NwDataId);
	static bool ChangeDataFormat(S_DWORD FN, const S_NwDataID &NwDataId, const string &strRawData, string &strData);

	/** @brief 获取数据密度
	 *  @param[in] PeriodVal 周期值
	 *  @param[in] TimeUnit 周期单位
	 *  @return 一天的点数
	 */
	static S_DataDensity GetDataDensity(S_BYTE FreezeFlag, S_TIME_UNIT TimeUnit, S_BYTE PeriodVal);
};

/**
 *  @class 
 *  @brief 告警事件操作接口
 */
class C_AlmEvt
{
public:
	/** @brief 写入告警
	 *  @param[in] MP 测量点
	 *  @param[in] DI 数据标识
	 *  @param[in] strReocrd 告警数据内容
	 *  @return -1:出错; >=0:写入的记录条数
	 */
	static int Write(S_WORD MP, S_DWORD DI, const string &strReocrd);
	/** @brief 读出告警
	 *  @param[in] MP 测量点
	 *  @param[in] DI 数据标识
	 *  @param[out] strReocrds 告警数据内容(排序输出，时间最新的在最前)
	 *  @param[in] timeBegin 开始时间,如为0则取未上报的记录
	 *  @param[in] timeEnd 结束时间，前闭后开
	 *  @return -1:出错; >=0:读出的记录条数
	 */
	static int Read(S_WORD MP, S_DWORD DI, strings &strReocrds, S_TIME timeBegin, S_TIME timeEnd);
	/** @brief 读出未上报告警
	 *  @param[in] AlmType 告警类型
	 *  @param[out] strReocrds 告警数据内容
	 *  @return -1:出错; >=0:读出的记录条数
	 */
	static int Report(S_AlmType AlmType, strings &strReocrds);
	/** @brief 统计新告警个数
	 *  @param[in] AlmType 告警类型
	 *  @return -1:出错; >=0:新告警个数
	 */
	static int CountNew(S_AlmType AlmType);

	/** @brief 取告警存储类型
	 *  @param[in] DI 告警数据项
	 *  @return ALM_TYPE_EVENT,ALM_TYPE_IMPORTANT,ALM_TYPE_NORMAL,ALM_TYPE_UNRECORD_ALARM
	 */
	static S_AlmType GetAlarmStoryType(S_DWORD DI);
	/** @brief 取告警存储类型
	 *  @param[in] DI 告警数据项
	 *  @return ALM_TYPE_EVENT,ALM_TYPE_ALARM,ALM_TYPE_UNKNOW
	 */
	static S_AlmType GetAlarmNormalType(S_DWORD DI);

private:
	/** @brief 读出告警
	 *  @param[in] MP 测量点
	 *  @param[in] DI 数据标识
	 *  @param[out] strReocrds 告警数据内容
	 *  @param[in] timeBegin 开始时间,如为0则取未上报的记录
	 *  @param[in] timeEnd 结束时间，前闭后开
	 *  @return -1:出错; >=0:读出的记录条数
	 */
	static int _Read(S_WORD MP, S_DWORD DI, strings &strReocrds, S_TIME timeBegin, S_TIME timeEnd);
	/** @brief 读出未上报告警
	 *  @param[in] AlmType 告警类型
	 *  @param[out] strReocrds 告警数据内容
	 *  @return -1:出错; >=0:读出的记录条数
	 */
	static int _Report(S_AlmType AlmType, strings &strReocrds);
};

#endif //__NW_DATA_FILE_H__


