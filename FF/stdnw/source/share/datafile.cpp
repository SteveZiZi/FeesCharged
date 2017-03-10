/** @file
 *  @brief 数据文件用户层接口
 *  @author 
 *  @date 2012/03/21
 *  @version 0.1
 */
#include "datafile.h"
#include "status.h"
#include "context.h"

//
//南网块数据项表
//
bool operator<(const S_NwDataID &left, const S_NwDataID &right)
{
	return left.m_DI < right.m_DI;
}
static S_NwDataID g_NwDataDiTable[] =
{
	{0x0000FF00, NW_DATA_ID_TYPE_TARIFF, 4}, // 当前组合有功
	{0x0001FF00, NW_DATA_ID_TYPE_TARIFF, 4}, // 当前正向有功
	{0x0002FF00, NW_DATA_ID_TYPE_TARIFF, 4}, // 当前反向有功
	{0x0003FF00, NW_DATA_ID_TYPE_TARIFF, 4}, // 当前组合无功1
	{0x0004FF00, NW_DATA_ID_TYPE_TARIFF, 4}, // 当前组合无功2
	{0x0005FF00, NW_DATA_ID_TYPE_TARIFF, 4}, // 当前一象限无功
	{0x0006FF00, NW_DATA_ID_TYPE_TARIFF, 4}, // 当前二象限无功
	{0x0007FF00, NW_DATA_ID_TYPE_TARIFF, 4}, // 当前三象限无功
	{0x0008FF00, NW_DATA_ID_TYPE_TARIFF, 4}, // 当前四象限无功
	{0x0009FF00, NW_DATA_ID_TYPE_TARIFF, 4}, // 当前正向视在电能
	{0x000AFF00, NW_DATA_ID_TYPE_TARIFF, 4}, // 当前反同视在电能

	{0x0101FF00, NW_DATA_ID_TYPE_TARIFF, 8}, // 当前正向有功最大需量及发生时间
	{0x0102FF00, NW_DATA_ID_TYPE_TARIFF, 8}, // 当前反向有功最大需量及发生时间
	{0x0103FF00, NW_DATA_ID_TYPE_TARIFF, 8}, // 当前组合无功1最大需量及发生时间
	{0x0104FF00, NW_DATA_ID_TYPE_TARIFF, 8}, // 当前组合无功2最大需量及发生时间

	{0x0201FF00, NW_DATA_ID_TYPE_SUM_N, 2}, // 当前电压数据块
	{0x0202FF00, NW_DATA_ID_TYPE_SUM_N, 3}, // 当前电流数据块

	{0x0203FF00, NW_DATA_ID_TYPE_SUM_Y, 3}, // 当前瞬时有功功率
	{0x0204FF00, NW_DATA_ID_TYPE_SUM_Y, 3}, // 当前瞬时无功功率
	{0x0205FF00, NW_DATA_ID_TYPE_SUM_Y, 3}, // 当前瞬时视在功率
	{0x0206FF00, NW_DATA_ID_TYPE_SUM_Y, 2}, // 当前功率因数
	
	{0x0207FF00, NW_DATA_ID_TYPE_SUM_N, 2}, // 当前相角数据块
	{0x0208FF00, NW_DATA_ID_TYPE_SUM_N, 2}, // 当前电压波形失真
	{0x0209FF00, NW_DATA_ID_TYPE_SUM_N, 2}, // 当前电流波形失真

	{0x040005FF, NW_DATA_ID_TYPE_SUM_N, 2}, // 当前电表运行状态字

	{0x050601FF, NW_DATA_ID_TYPE_TARIFF, 4}, // 日冻结正向有功
	{0x050602FF, NW_DATA_ID_TYPE_TARIFF, 4}, // 日冻结反向有功
	{0x050603FF, NW_DATA_ID_TYPE_TARIFF, 4}, // 日冻结组合无功1
	{0x050604FF, NW_DATA_ID_TYPE_TARIFF, 4}, // 日冻结组合无功2
	{0x050605FF, NW_DATA_ID_TYPE_TARIFF, 4}, // 日冻结一象限无功
	{0x050606FF, NW_DATA_ID_TYPE_TARIFF, 4}, // 日冻结二象限无功
	{0x050607FF, NW_DATA_ID_TYPE_TARIFF, 4}, // 日冻结三象限无功
	{0x050608FF, NW_DATA_ID_TYPE_TARIFF, 4}, // 日冻结四象限无功

	{0x050609FF, NW_DATA_ID_TYPE_TARIFF, 8}, // 日冻结正向有功最大需量及发生时间
	{0x05060AFF, NW_DATA_ID_TYPE_TARIFF, 8}, // 日冻结反向有功最大需量及发生时间
	{0x05060BFF, NW_DATA_ID_TYPE_TARIFF, 8}, // 日冻结组合无功1最大需量及发生时间
	{0x05060CFF, NW_DATA_ID_TYPE_TARIFF, 8}, // 日冻结组合无功2最大需量及发生时间
	
	{0x0000FF01, NW_DATA_ID_TYPE_TARIFF, 4}, // 月冻结组合有功
	{0x0001FF01, NW_DATA_ID_TYPE_TARIFF, 4}, // 月冻结正向有功
	{0x0002FF01, NW_DATA_ID_TYPE_TARIFF, 4}, // 月冻结反向有功
	{0x0003FF01, NW_DATA_ID_TYPE_TARIFF, 4}, // 月冻结组合无功1
	{0x0004FF01, NW_DATA_ID_TYPE_TARIFF, 4}, // 月冻结组合无功2
	{0x0005FF01, NW_DATA_ID_TYPE_TARIFF, 4}, // 月冻结一象限无功
	{0x0006FF01, NW_DATA_ID_TYPE_TARIFF, 4}, // 月冻结二象限无功
	{0x0007FF01, NW_DATA_ID_TYPE_TARIFF, 4}, // 月冻结三象限无功
	{0x0008FF01, NW_DATA_ID_TYPE_TARIFF, 4}, // 月冻结四象限无功
	{0x0009FF01, NW_DATA_ID_TYPE_TARIFF, 4}, // 月冻结正向视在电能
	{0x000AFF01, NW_DATA_ID_TYPE_TARIFF, 4}, // 月冻结反向视在电能

	{0x0101FF01, NW_DATA_ID_TYPE_TARIFF, 8}, // 月冻结正向有功最大需量及发生时间
	{0x0102FF01, NW_DATA_ID_TYPE_TARIFF, 8}, // 月冻结反向有功最大需量及发生时间
	{0x0103FF01, NW_DATA_ID_TYPE_TARIFF, 8}, // 月冻结组合无功1最大需量及发生时间
	{0x0104FF01, NW_DATA_ID_TYPE_TARIFF, 8}, // 月冻结组合无功2最大需量及发生时间
	{0x0105FF01, NW_DATA_ID_TYPE_TARIFF, 8}, // 月冻结一象限最大需量及发生时间
	{0x0106FF01, NW_DATA_ID_TYPE_TARIFF, 8}, // 月冻结二象限最大需量及发生时间
	{0x0107FF01, NW_DATA_ID_TYPE_TARIFF, 8}, // 月冻结三象限最大需量及发生时间
	{0x0108FF01, NW_DATA_ID_TYPE_TARIFF, 8}, // 月冻结四象限最大需量及发生时间
	{0x0109FF01, NW_DATA_ID_TYPE_TARIFF, 8}, // 月冻结正向视在最大需量及发生时间
	{0x010AFF01, NW_DATA_ID_TYPE_TARIFF, 8}, // 月冻结反向视在最大需量及发生时间

	{0xFFFFFFFF, NW_DATA_ID_TYPE_MAX, 100} // 结尾
};

//
//
//
/** @brief 二类数据写入函数
 *  @param[in] FN 数据标识
 *  @param[in] MP 测量点
 *  @param[in] strData 数据记录
 *  @param[in] DataTime 时标
 *  @param[in] DataDensity 冻结密度
 *  @return <0:失败;=0:成功
 */
int C_ENERGY::Write(S_DWORD FN, S_WORD MP, const string &strData, S_TIME DataTime, S_DataDensity DataDensity)
{
	S_EnergyData MeterData;
	S_EnergyDatas MeterDatas;
	MeterData.m_MeterMP = MP;
	MeterData.m_Data.append(strData);
	MeterDatas.push_back(MeterData);
	return WriteClass2Data(FN, MeterDatas, DataTime, DataDensity);
}

/** @brief 二类数据写入函数
 *  @param[in] FN 数据标识
 *  @param[in] MeterDatas 能量数据组，参见S_EnergyDatas定义
 *  @param[in] DataTime 时标
 *  @param[in] VipPointPerDay 重点户FN每天须要的数据点数据，仅过重点户FN有效
 *  @return 写入的记录条数
 */
int C_ENERGY::WriteClass2Data(INT32U FN, const S_EnergyDatas &MeterDatas, S_TIME DataTime, S_DataDensity DataDensity)
{
	C_DFOP aDataObj(0, 0, 0, 0, 0);
	S_LcdStatus *pLcdStatus = CONTEXT.GetLcdStatus();
	pLcdStatus->m_timeFlashStorage = C_TIME::TimeNow();
	return aDataObj.WriteClass2Data(FN, MeterDatas, DataTime, DataDensity);
}

/** @brief 二类数据读取函数
 *  @param[in] MP 数据标识
 *  @param[in] FN 数据标识
 *  @param[in] DataTime 时标
 *  @param[out] strData 能量数据
 *  @return 读取的记录条数
 */
int C_ENERGY::ReadClass2Data(S_WORD MP, INT32U FN, S_TIME DataTime, string &strData)
{
	S_EnergyData MeterData;
	S_EnergyDatas MeterDatas;
	MeterData.m_MeterMP = MP;
	MeterDatas.push_back(MeterData);

	int Cnt = ReadClass2Data(FN, MeterDatas, DataTime);
	strData.append(MeterDatas[0].m_Data);
	return Cnt;
}

/** @brief 二类数据读取函数
 *  @param[in] FN 数据标识
 *  @param[inout] MeterDatas 能量数据组，参见S_EnergyDatas定义
 *  @param[in] DataTime 时标
 *  @return 读取的记录条数
 */
int C_ENERGY::ReadClass2Data(INT32U FN, S_EnergyDatas &MeterDatas, S_TIME DataTime)
{
	C_DFOP aDataObj(0, 0, 0, 0, 0);
	S_LcdStatus *pLcdStatus = CONTEXT.GetLcdStatus();
	pLcdStatus->m_timeFlashStorage = C_TIME::TimeNow();
	return aDataObj.ReadClass2Data(FN, MeterDatas, DataTime);
}

/** @brief 读取数据存储密度函数
 *  @param[in] FN 数据标识
 *  @return <=0:读取失败; >0:数据的存储密度
 */
int C_ENERGY::GetDensity(INT32U FN)
{
	C_DFOP aDataObj(0, 0, 0, 0, 0);
	return aDataObj.GetDensity(FN);
}

/** @brief 二类数据写入函数
 *  @param[in] pTaskInfo 任务信息
 *  @param[in] pTaskCtrlStatus 任务控制信息
 *  @param[in] FreezeTime 数据冻结时间
 *  @return <0:失败;=0:成功
 */
int C_Class2Data::Write(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus, S_TIME FreezeTime)
{
	char buffer[32];
	S_WORD MP = pTaskCtrlStatus->m_mpMeter;
	if (pTaskInfo->m_CommRegs.size() != pTaskCtrlStatus->m_strDatas.size())
	{
		logwt<<"测量点["<<MP<<"]的数据与寄存器不匹配"<<endl;
		return -1;//数据不匹配
	}
	if (pTaskCtrlStatus->m_strDatas.Any() == false)
	{
		logwt<<"测量点["<<MP<<"]的数据为空"<<endl;
		return -1;//数据一个都没有
	}
	S_MSDI msdi = STATUS.FindMasterStationDataID(CLASSDATA_2, pTaskInfo->m_MSDI);
	if (msdi.m_Enable == false)
		return -1;

	string strData;
	if (msdi.SerializeData(pTaskInfo->m_CommRegs, pTaskCtrlStatus->m_strDatas, strData) <= 0)
	{
		logwt<<"测量点["<<MP<<"]的数据串行化失败"<<endl;
		return -1;
	}
	S_DataDensity DataDensity = GetDataDensity(msdi.m_Freeze, pTaskInfo->m_TimeUnit, pTaskInfo->m_PeriodVal);
	if (C_ENERGY::Write(msdi.m_DI, MP, strData, FreezeTime, DataDensity) < 0)
	{
		logwt<<"测量点["<<MP<<"]的数据["<<HEX<<strData<<"]存储失败"<<endl;
		return -1;
	}
	S_TermData *pTermData = STATUS.GetTermData();
	pTermData->m_MeterData.m_timeMeterCommunication[MP] = C_TIME::TimeNow();
	C_TIME::Time2Ascii14(FreezeTime, buffer);
	logwt<<"测量点["<<MP<<"]的数据["<<HEX<<strData<<"]时标["<<buffer<<"]存储成功"<<endl;
	return 0;
}

/** @brief 二类数据写入函数
 *  @param[in] pTaskInfo 任务信息
 *  @param[in] pTaskCtrlStatus 任务控制信息
 *  @param[in] FreezeTime 数据冻结时间
 *  @return <0:失败;=0:成功
 */
int C_Class2Data::WriteTermData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus, S_TIME FreezeTime)
{
	char buffer[32];
	S_WORD MP = pTaskCtrlStatus->m_mpMeter;
	if (pTaskInfo->m_CommRegs.size() != pTaskCtrlStatus->m_strDatas.size())
	{
		logwt<<"测量点["<<MP<<"]的数据与寄存器不匹配"<<endl;
		return -1;//数据不匹配
	}
	if (pTaskCtrlStatus->m_strDatas.Any() == false)
	{
		logwt<<"测量点["<<MP<<"]的数据为空"<<endl;
		return -1;//数据一个都没有
	}
	S_MSDI msdi = STATUS.FindMasterStationDataID(CLASSDATA_2, pTaskInfo->m_MSDI);
	if (msdi.m_Enable == false)
		return -1;

	string strData;
	if (msdi.SerializeData(pTaskInfo->m_CommRegs, pTaskCtrlStatus->m_strDatas, strData) <= 0)
	{
		logwt<<"测量点["<<MP<<"]的数据串行化失败"<<endl;
		return -1;
	}
	S_DataDensity DataDensity = GetDataDensity(msdi.m_Freeze, pTaskInfo->m_TimeUnit, pTaskInfo->m_PeriodVal);
	if (C_ENERGY::Write(msdi.m_DI, MP, strData, FreezeTime, DataDensity) < 0)
	{
		logwt<<"测量点["<<MP<<"]的数据["<<HEX<<strData<<"]存储失败"<<endl;
		return -1;
	}
	C_TIME::Time2Ascii14(FreezeTime, buffer);
	logwt<<"测量点["<<MP<<"]的数据["<<HEX<<strData<<"]时标["<<buffer<<"]存储成功"<<endl;
	return 0;
}

/** @brief 二类数据读取函数
 *  @param[in] FN 数据标识
 *  @param[in] MP 测量点
 *  @param[out] strData 数据输出缓冲
 *  @param[in] DataTime 时标
 *  @return <0:失败;=0:成功
 */
int C_Class2Data::Read(S_DWORD FN, S_WORD MP, string &strData, S_TIME DataTime)
{
	char buffer[32];
	string strMyData;
	C_TIME::Time2Ascii14(DataTime, buffer);

	if (C_ENERGY::ReadClass2Data(MP, FN, DataTime, strMyData) > 0)
	{
		strData.append(strMyData);
		logwt<<"测量点["<<MP<<"]的数据["<<HEX<<FN<<"="<<strData<<"]时标["<<buffer<<"]读取成功"<<endl;
		return 0;
	}
	/** 从数据块里面提取 */
	S_NwDataID NwDataId;
	if (FindReserveDataId(FN, NwDataId) == true)
	{
		strMyData.clear();
		if (C_ENERGY::ReadClass2Data(MP, NwDataId.m_DI, DataTime, strMyData) > 0)
		{
			if (ChangeDataFormat(FN, NwDataId, strMyData, strData) == true)
			{
				logwt<<"测量点["<<MP<<"]的数据["<<HEX<<FN<<"="<<strData<<"]时标["<<buffer<<"]读取成功"<<endl;
				return 0;
			}
		}
	}

	logwt<<"测量点["<<MP<<"]的数据["<<HEX<<FN<<"]时标["<<buffer<<"]读取失败"<<endl;
	return -1;
}

/** @brief 读取数据存储密度函数
 *  @param[in] FN 数据标识
 *  @return <=0:读取失败; >0:数据的存储密度
 */
int C_Class2Data::GetDensity(INT32U FN)
{
	return C_ENERGY::GetDensity(FN);
}

/** @brief 查找备用数据项
 */
bool C_Class2Data::FindReserveDataId(S_DWORD FN, S_NwDataID &NwDataId)
{
	static bool fSort = false;
	if (fSort == false)
	{
		sort(g_NwDataDiTable, g_NwDataDiTable+KGE_MEMBER_OF(g_NwDataDiTable));
		fSort = true;
	}

	if (_FindReserveDataId(FN|0x000000FF, NwDataId) == true)
		return true;
	if (_FindReserveDataId(FN|0x0000FF00, NwDataId) == true)
		return true;
	return false;
}
bool C_Class2Data::_FindReserveDataId(S_DWORD FN, S_NwDataID &NwDataId)
{
	const S_NwDataID NwDataID = {FN, 0, 0};
	const S_NwDataID *pFindObj = lower_bound(g_NwDataDiTable, g_NwDataDiTable+KGE_MEMBER_OF(g_NwDataDiTable), NwDataID);
	if (pFindObj != g_NwDataDiTable+KGE_MEMBER_OF(g_NwDataDiTable) && pFindObj->m_DI == FN)
	{
		NwDataId = *pFindObj;
		return true;
	}
	return false;
}

bool C_Class2Data::ChangeDataFormat(S_DWORD FN, const S_NwDataID &NwDataId, const string &strRawData, string &strData)
{
	if (strRawData.size() < NwDataId.m_ItemLen)
		return false;

	string strMyData(strRawData);
	if (NwDataId.m_type == NW_DATA_ID_TYPE_TARIFF)
		strMyData.erase(strMyData.begin()); // 去费率数
	S_DWORD TariffCnt = FN & 0x000000FF;
	if ((FN | 0x0000FF00) == NwDataId.m_DI) // 第二字节是费率
		TariffCnt = (FN >> 8) & 0x000000FF;
	if (NwDataId.m_type == NW_DATA_ID_TYPE_SUM_N)
		TariffCnt -= 1; // 没有总的数据块，序号从1开始，其它，如费率从0开始

	strings strMyDatas;
	if (strMyDatas.split(strMyData, (size_t)NwDataId.m_ItemLen) <= 0)
		return false;
	if ((S_DWORD)strMyDatas.size() < (TariffCnt+1))
		return false;

	strData.append(strMyDatas[TariffCnt]);
	return true;
}

/** @brief 获取数据密度
 *  @param[in] PeriodVal 周期值
 *  @param[in] TimeUnit 周期单位
 *  @return 一天的点数
 */
S_DataDensity C_Class2Data::GetDataDensity(S_BYTE FreezeFlag, S_TIME_UNIT TimeUnit, S_BYTE PeriodVal)
{
	if (FreezeFlag == MSDI_FREEZE_DAY)
		return DATA_DENSITY_DAY01;
	else if (FreezeFlag == MSDI_FREEZE_MONTH)
		return DATA_DENSITY_MON01;
	else
	{
		if (TimeUnit == TIME_UNIT_MINUTE)
		{
			switch (PeriodVal)
			{
			case 1:return DATA_DENSITY_MIN01; // 1 分钟
			case 5:return DATA_DENSITY_MIN05; // 5 分钟
			case 15:return DATA_DENSITY_MIN15; // 15 分钟
			case 30:return DATA_DENSITY_MIN30; // 30 分钟
			case 60:return DATA_DENSITY_MIN60; // 60 分钟
			default:return DATA_DENSITY_MIN60; 
			}
		}
	}
	return DATA_DENSITY_MIN60;
}


//
//
//
/** @brief 写入告警
 *  @param[in] MP 测量点
 *  @param[in] DI 数据标识
 *  @param[in] strReocrd 告警数据内容
 *  @return -1:出错; >=0:写入的记录条数
 */
int C_AlmEvt::Write(S_WORD MP, S_DWORD DI, const string &strReocrd)
{
	int ok = 0;
	S_AlarmStatus *pAS = STATUS.GetAlarmStatus();
	S_AlmType AlmType = GetAlarmStoryType(DI);
	if (AlmType == ALM_TYPE_NORMAL)
	{
		C_AlarmNormal AlmNormal(0);
		ok = AlmNormal.AddAlarm(MP, DI, strReocrd);
		pAS->m_NormalNewCnt += 1;
	}
	else if (AlmType == ALM_TYPE_IMPORTANT)
	{
		C_AlarmImportance AlmImportance(0);
		ok = AlmImportance.AddAlarm(MP, DI, strReocrd);
		pAS->m_ImportantNewCnt += 1;
		pAS->m_bsNewAlarmID.set((int)(DI&0xFF));
		if (pAS->m_tickImportantHappen == 0)
			pAS->m_tickImportantHappen = CONTEXT.GetSystemTick();
	}
	else if (AlmType == ALM_TYPE_EVENT)
	{
		C_EVENT event(0);
		ok = event.AddEvent(MP, DI, strReocrd);
		pAS->m_EventNewCnt += 1;
	}
	if (ok > 0)
	{// flash 读写，通知显示图标闪烁
		S_LcdStatus *pLcdStatus = CONTEXT.GetLcdStatus();
		pLcdStatus->m_timeFlashStorage = C_TIME::TimeNow();
	}
	return ok;
}

/** @brief 读出告警
 *  @param[in] MP 测量点
 *  @param[in] DI 数据标识
 *  @param[out] strReocrds 告警数据内容
 *  @param[in] timeBegin 开始时间
 *  @param[in] timeEnd 结束时间，前闭后开
 *  @return -1:出错; >=0:读出的记录条数
 */
int C_AlmEvt::Read(S_WORD MP, S_DWORD DI, strings &strReocrds, S_TIME timeBegin, S_TIME timeEnd)
{
	int ok = _Read(MP, DI, strReocrds, timeBegin, timeEnd);
	if (ok > 0)
	{// flash 读写，通知显示图标闪烁
		S_LcdStatus *pLcdStatus = CONTEXT.GetLcdStatus();
		pLcdStatus->m_timeFlashStorage = C_TIME::TimeNow();
	}

	S_AlarmStatus *pAS = STATUS.GetAlarmStatus();
	pAS->Flush();
	return ok;
}

/** @brief 读出未上报告警
 *  @param[in] AlmType 告警类型
 *  @param[out] strReocrds 告警数据内容
 *  @return -1:出错; >=0:读出的记录条数
 */
int C_AlmEvt::Report(S_AlmType AlmType, strings &strReocrds)
{
	int ok = _Report(AlmType, strReocrds);
	if (ok > 0)
	{// flash 读写，通知显示图标闪烁
		S_LcdStatus *pLcdStatus = CONTEXT.GetLcdStatus();
		pLcdStatus->m_timeFlashStorage = C_TIME::TimeNow();
	}

	S_AlarmStatus *pAS = STATUS.GetAlarmStatus();
	pAS->Flush();
	return ok;
}

/** @brief 统计新告警个数
 *  @param[in] AlmType 告警类型
 *  @return -1:出错; >=0:新告警个数
 */
int C_AlmEvt::CountNew(S_AlmType AlmType)
{
	if (AlmType == ALM_TYPE_NORMAL)
	{
		C_AlarmNormal AlmNormal(0);
		return AlmNormal.GetNewAlarmCnt();
	}
	else if (AlmType == ALM_TYPE_IMPORTANT)
	{
		C_AlarmImportance AlmImportance(0);
		return AlmImportance.GetNewAlarmCnt();
	}
	else if (AlmType == ALM_TYPE_EVENT)
	{
		C_EVENT event(0);
		return event.GetNewEventCnt();
	}
	return 0;
}

/** @brief 取告警存储类型
 *  @param[in] DI 告警数据项
 *  @return ALM_TYPE_EVENT,ALM_TYPE_IMPORTANT,ALM_TYPE_NORMAL,ALM_TYPE_UNRECORD_ALARM
 */
S_AlmType C_AlmEvt::GetAlarmStoryType(S_DWORD DI)
{
	S_AlarmInfo *pInfo = STATUS.GetAlarmInfo();
	if (DI >= DI_ALM_EVENT_MIN && DI <= DI_ALM_EVENT_MAX)
	{
		S_DWORD idx = DI - DI_ALM_EVENT_MIN;
		if (pInfo->m_OnlyEvent.test(idx) == true)
			return ALM_TYPE_EVENT;
	}
	if (DI >= DI_ALM_ALARM_MIN && DI <= DI_ALM_ALARM_MAX)
	{
		S_DWORD idx = DI - DI_ALM_ALARM_MIN;
		if (pInfo->m_AlarmEven.test(idx) == true)
		{
			if (pInfo->m_ReportAlarmEven.test(idx) == true)
				return ALM_TYPE_IMPORTANT;
			return ALM_TYPE_NORMAL;
		}
	}
	return ALM_TYPE_UNRECORD_ALARM;
}

/** @brief 取告警存储类型
 *  @param[in] DI 告警数据项
 *  @return ALM_TYPE_EVENT,ALM_TYPE_ALARM,ALM_TYPE_UNKNOW
 */
S_AlmType C_AlmEvt::GetAlarmNormalType(S_DWORD DI)
{
	if (DI >= DI_ALM_EVENT_MIN && DI <= DI_ALM_EVENT_MAX)
		return ALM_TYPE_EVENT;
	if (DI >= DI_ALM_ALARM_MIN && DI <= DI_ALM_ALARM_MAX)
		return ALM_TYPE_ALARM;
	return ALM_TYPE_UNKNOW;
}

/** @brief 读出告警
 *  @param[in] MP 测量点
 *  @param[in] DI 数据标识
 *  @param[out] strReocrds 告警数据内容
 *  @param[in] timeBegin 开始时间
 *  @param[in] timeEnd 结束时间，前闭后开
 *  @return -1:出错; >=0:读出的记录条数
 */
int C_AlmEvt::_Read(S_WORD MP, S_DWORD DI, strings &strReocrds, S_TIME timeBegin, S_TIME timeEnd)
{
	S_AlmType AlmType = GetAlarmStoryType(DI);
	if (DI == DI_ALM_EVENT_ALL) //所有类型事件记录
	{
//		MP = 0xFFFF, 
		DI = 0xFFFFFFFFUL;
		AlmType = ALM_TYPE_EVENT;
	}
	else if (DI == DI_ALM_ALARM_UNREPORT) //读所有主动上送未成功的告警数据
	{
//		MP = 0xFFFF, 
		DI = 0xFFFFFFFFUL;
		timeBegin = 0, timeEnd = 0;
		AlmType = ALM_TYPE_IMPORTANT;
	}
	else if (DI == DI_ALM_ALARM_ALL) //所有告警类型
	{
//		MP = 0xFFFF, 
		DI = 0xFFFFFFFFUL;
		AlmType = ALM_TYPE_RECORD_ALARM;
	}

	if (AlmType == ALM_TYPE_NORMAL)
	{
		C_AlarmNormal AlmNormal(0);
		return AlmNormal.GetAlarm(MP, DI, strReocrds, timeBegin, timeEnd);
	}
	else if (AlmType == ALM_TYPE_IMPORTANT)
	{
		C_AlarmImportance AlmImportance(0);
		return AlmImportance.GetAlarm(MP, DI, strReocrds, timeBegin, timeEnd);
	}
	else if (AlmType == ALM_TYPE_EVENT)
	{
		C_EVENT event(0);
		return event.GetEvent(MP, DI, strReocrds, timeBegin, timeEnd);
	}
	else if (AlmType == ALM_TYPE_RECORD_ALARM)
	{
		C_AlarmNormal aAlmNormal(0);
		C_AlarmImportance bAlmImportance(0);
		aAlmNormal.GetAlarm(MP, DI, strReocrds, timeBegin, timeEnd);
		return bAlmImportance.GetAlarm(MP, DI, strReocrds, timeBegin, timeEnd);
	}
	return 0;
}

/** @brief 读出未上报告警
 *  @param[in] AlmType 告警类型
 *  @param[out] strReocrds 告警数据内容
 *  @return -1:出错; >=0:读出的记录条数
 */
int C_AlmEvt::_Report(S_AlmType AlmType, strings &strReocrds)
{
	if (AlmType == ALM_TYPE_NORMAL)
	{
		C_AlarmNormal AlmNormal(0);
		return AlmNormal.GetAlarm(0xFFFF, 0xFFFFFFFFUL, strReocrds, 0, 0);
	}
	else if (AlmType == ALM_TYPE_IMPORTANT)
	{
		C_AlarmImportance AlmImportance(0);
		return AlmImportance.GetAlarm(0xFFFF, 0xFFFFFFFFUL, strReocrds, 0, 0);
	}
	else if (AlmType == ALM_TYPE_EVENT)
	{
		C_EVENT event(0);
		return event.GetEvent(0xFFFF, 0xFFFFFFFFUL, strReocrds, 0, 0);
	}
	return 0;
}


