/** @file
 *  @brief �����ļ��û���ӿ�
 *  @author 
 *  @date 2012/03/21
 *  @version 0.1
 */
#include "datafile.h"
#include "status.h"
#include "context.h"

//
//�������������
//
bool operator<(const S_NwDataID &left, const S_NwDataID &right)
{
	return left.m_DI < right.m_DI;
}
static S_NwDataID g_NwDataDiTable[] =
{
	{0x0000FF00, NW_DATA_ID_TYPE_TARIFF, 4}, // ��ǰ����й�
	{0x0001FF00, NW_DATA_ID_TYPE_TARIFF, 4}, // ��ǰ�����й�
	{0x0002FF00, NW_DATA_ID_TYPE_TARIFF, 4}, // ��ǰ�����й�
	{0x0003FF00, NW_DATA_ID_TYPE_TARIFF, 4}, // ��ǰ����޹�1
	{0x0004FF00, NW_DATA_ID_TYPE_TARIFF, 4}, // ��ǰ����޹�2
	{0x0005FF00, NW_DATA_ID_TYPE_TARIFF, 4}, // ��ǰһ�����޹�
	{0x0006FF00, NW_DATA_ID_TYPE_TARIFF, 4}, // ��ǰ�������޹�
	{0x0007FF00, NW_DATA_ID_TYPE_TARIFF, 4}, // ��ǰ�������޹�
	{0x0008FF00, NW_DATA_ID_TYPE_TARIFF, 4}, // ��ǰ�������޹�
	{0x0009FF00, NW_DATA_ID_TYPE_TARIFF, 4}, // ��ǰ�������ڵ���
	{0x000AFF00, NW_DATA_ID_TYPE_TARIFF, 4}, // ��ǰ��ͬ���ڵ���

	{0x0101FF00, NW_DATA_ID_TYPE_TARIFF, 8}, // ��ǰ�����й��������������ʱ��
	{0x0102FF00, NW_DATA_ID_TYPE_TARIFF, 8}, // ��ǰ�����й��������������ʱ��
	{0x0103FF00, NW_DATA_ID_TYPE_TARIFF, 8}, // ��ǰ����޹�1�������������ʱ��
	{0x0104FF00, NW_DATA_ID_TYPE_TARIFF, 8}, // ��ǰ����޹�2�������������ʱ��

	{0x0201FF00, NW_DATA_ID_TYPE_SUM_N, 2}, // ��ǰ��ѹ���ݿ�
	{0x0202FF00, NW_DATA_ID_TYPE_SUM_N, 3}, // ��ǰ�������ݿ�

	{0x0203FF00, NW_DATA_ID_TYPE_SUM_Y, 3}, // ��ǰ˲ʱ�й�����
	{0x0204FF00, NW_DATA_ID_TYPE_SUM_Y, 3}, // ��ǰ˲ʱ�޹�����
	{0x0205FF00, NW_DATA_ID_TYPE_SUM_Y, 3}, // ��ǰ˲ʱ���ڹ���
	{0x0206FF00, NW_DATA_ID_TYPE_SUM_Y, 2}, // ��ǰ��������
	
	{0x0207FF00, NW_DATA_ID_TYPE_SUM_N, 2}, // ��ǰ������ݿ�
	{0x0208FF00, NW_DATA_ID_TYPE_SUM_N, 2}, // ��ǰ��ѹ����ʧ��
	{0x0209FF00, NW_DATA_ID_TYPE_SUM_N, 2}, // ��ǰ��������ʧ��

	{0x040005FF, NW_DATA_ID_TYPE_SUM_N, 2}, // ��ǰ�������״̬��

	{0x050601FF, NW_DATA_ID_TYPE_TARIFF, 4}, // �ն��������й�
	{0x050602FF, NW_DATA_ID_TYPE_TARIFF, 4}, // �ն��ᷴ���й�
	{0x050603FF, NW_DATA_ID_TYPE_TARIFF, 4}, // �ն�������޹�1
	{0x050604FF, NW_DATA_ID_TYPE_TARIFF, 4}, // �ն�������޹�2
	{0x050605FF, NW_DATA_ID_TYPE_TARIFF, 4}, // �ն���һ�����޹�
	{0x050606FF, NW_DATA_ID_TYPE_TARIFF, 4}, // �ն���������޹�
	{0x050607FF, NW_DATA_ID_TYPE_TARIFF, 4}, // �ն����������޹�
	{0x050608FF, NW_DATA_ID_TYPE_TARIFF, 4}, // �ն����������޹�

	{0x050609FF, NW_DATA_ID_TYPE_TARIFF, 8}, // �ն��������й��������������ʱ��
	{0x05060AFF, NW_DATA_ID_TYPE_TARIFF, 8}, // �ն��ᷴ���й��������������ʱ��
	{0x05060BFF, NW_DATA_ID_TYPE_TARIFF, 8}, // �ն�������޹�1�������������ʱ��
	{0x05060CFF, NW_DATA_ID_TYPE_TARIFF, 8}, // �ն�������޹�2�������������ʱ��
	
	{0x0000FF01, NW_DATA_ID_TYPE_TARIFF, 4}, // �¶�������й�
	{0x0001FF01, NW_DATA_ID_TYPE_TARIFF, 4}, // �¶��������й�
	{0x0002FF01, NW_DATA_ID_TYPE_TARIFF, 4}, // �¶��ᷴ���й�
	{0x0003FF01, NW_DATA_ID_TYPE_TARIFF, 4}, // �¶�������޹�1
	{0x0004FF01, NW_DATA_ID_TYPE_TARIFF, 4}, // �¶�������޹�2
	{0x0005FF01, NW_DATA_ID_TYPE_TARIFF, 4}, // �¶���һ�����޹�
	{0x0006FF01, NW_DATA_ID_TYPE_TARIFF, 4}, // �¶���������޹�
	{0x0007FF01, NW_DATA_ID_TYPE_TARIFF, 4}, // �¶����������޹�
	{0x0008FF01, NW_DATA_ID_TYPE_TARIFF, 4}, // �¶����������޹�
	{0x0009FF01, NW_DATA_ID_TYPE_TARIFF, 4}, // �¶����������ڵ���
	{0x000AFF01, NW_DATA_ID_TYPE_TARIFF, 4}, // �¶��ᷴ�����ڵ���

	{0x0101FF01, NW_DATA_ID_TYPE_TARIFF, 8}, // �¶��������й��������������ʱ��
	{0x0102FF01, NW_DATA_ID_TYPE_TARIFF, 8}, // �¶��ᷴ���й��������������ʱ��
	{0x0103FF01, NW_DATA_ID_TYPE_TARIFF, 8}, // �¶�������޹�1�������������ʱ��
	{0x0104FF01, NW_DATA_ID_TYPE_TARIFF, 8}, // �¶�������޹�2�������������ʱ��
	{0x0105FF01, NW_DATA_ID_TYPE_TARIFF, 8}, // �¶���һ�����������������ʱ��
	{0x0106FF01, NW_DATA_ID_TYPE_TARIFF, 8}, // �¶���������������������ʱ��
	{0x0107FF01, NW_DATA_ID_TYPE_TARIFF, 8}, // �¶����������������������ʱ��
	{0x0108FF01, NW_DATA_ID_TYPE_TARIFF, 8}, // �¶����������������������ʱ��
	{0x0109FF01, NW_DATA_ID_TYPE_TARIFF, 8}, // �¶������������������������ʱ��
	{0x010AFF01, NW_DATA_ID_TYPE_TARIFF, 8}, // �¶��ᷴ�������������������ʱ��

	{0xFFFFFFFF, NW_DATA_ID_TYPE_MAX, 100} // ��β
};

//
//
//
/** @brief ��������д�뺯��
 *  @param[in] FN ���ݱ�ʶ
 *  @param[in] MP ������
 *  @param[in] strData ���ݼ�¼
 *  @param[in] DataTime ʱ��
 *  @param[in] DataDensity �����ܶ�
 *  @return <0:ʧ��;=0:�ɹ�
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

/** @brief ��������д�뺯��
 *  @param[in] FN ���ݱ�ʶ
 *  @param[in] MeterDatas ���������飬�μ�S_EnergyDatas����
 *  @param[in] DataTime ʱ��
 *  @param[in] VipPointPerDay �ص㻧FNÿ����Ҫ�����ݵ����ݣ������ص㻧FN��Ч
 *  @return д��ļ�¼����
 */
int C_ENERGY::WriteClass2Data(INT32U FN, const S_EnergyDatas &MeterDatas, S_TIME DataTime, S_DataDensity DataDensity)
{
	C_DFOP aDataObj(0, 0, 0, 0, 0);
	S_LcdStatus *pLcdStatus = CONTEXT.GetLcdStatus();
	pLcdStatus->m_timeFlashStorage = C_TIME::TimeNow();
	return aDataObj.WriteClass2Data(FN, MeterDatas, DataTime, DataDensity);
}

/** @brief �������ݶ�ȡ����
 *  @param[in] MP ���ݱ�ʶ
 *  @param[in] FN ���ݱ�ʶ
 *  @param[in] DataTime ʱ��
 *  @param[out] strData ��������
 *  @return ��ȡ�ļ�¼����
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

/** @brief �������ݶ�ȡ����
 *  @param[in] FN ���ݱ�ʶ
 *  @param[inout] MeterDatas ���������飬�μ�S_EnergyDatas����
 *  @param[in] DataTime ʱ��
 *  @return ��ȡ�ļ�¼����
 */
int C_ENERGY::ReadClass2Data(INT32U FN, S_EnergyDatas &MeterDatas, S_TIME DataTime)
{
	C_DFOP aDataObj(0, 0, 0, 0, 0);
	S_LcdStatus *pLcdStatus = CONTEXT.GetLcdStatus();
	pLcdStatus->m_timeFlashStorage = C_TIME::TimeNow();
	return aDataObj.ReadClass2Data(FN, MeterDatas, DataTime);
}

/** @brief ��ȡ���ݴ洢�ܶȺ���
 *  @param[in] FN ���ݱ�ʶ
 *  @return <=0:��ȡʧ��; >0:���ݵĴ洢�ܶ�
 */
int C_ENERGY::GetDensity(INT32U FN)
{
	C_DFOP aDataObj(0, 0, 0, 0, 0);
	return aDataObj.GetDensity(FN);
}

/** @brief ��������д�뺯��
 *  @param[in] pTaskInfo ������Ϣ
 *  @param[in] pTaskCtrlStatus ���������Ϣ
 *  @param[in] FreezeTime ���ݶ���ʱ��
 *  @return <0:ʧ��;=0:�ɹ�
 */
int C_Class2Data::Write(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus, S_TIME FreezeTime)
{
	char buffer[32];
	S_WORD MP = pTaskCtrlStatus->m_mpMeter;
	if (pTaskInfo->m_CommRegs.size() != pTaskCtrlStatus->m_strDatas.size())
	{
		logwt<<"������["<<MP<<"]��������Ĵ�����ƥ��"<<endl;
		return -1;//���ݲ�ƥ��
	}
	if (pTaskCtrlStatus->m_strDatas.Any() == false)
	{
		logwt<<"������["<<MP<<"]������Ϊ��"<<endl;
		return -1;//����һ����û��
	}
	S_MSDI msdi = STATUS.FindMasterStationDataID(CLASSDATA_2, pTaskInfo->m_MSDI);
	if (msdi.m_Enable == false)
		return -1;

	string strData;
	if (msdi.SerializeData(pTaskInfo->m_CommRegs, pTaskCtrlStatus->m_strDatas, strData) <= 0)
	{
		logwt<<"������["<<MP<<"]�����ݴ��л�ʧ��"<<endl;
		return -1;
	}
	S_DataDensity DataDensity = GetDataDensity(msdi.m_Freeze, pTaskInfo->m_TimeUnit, pTaskInfo->m_PeriodVal);
	if (C_ENERGY::Write(msdi.m_DI, MP, strData, FreezeTime, DataDensity) < 0)
	{
		logwt<<"������["<<MP<<"]������["<<HEX<<strData<<"]�洢ʧ��"<<endl;
		return -1;
	}
	S_TermData *pTermData = STATUS.GetTermData();
	pTermData->m_MeterData.m_timeMeterCommunication[MP] = C_TIME::TimeNow();
	C_TIME::Time2Ascii14(FreezeTime, buffer);
	logwt<<"������["<<MP<<"]������["<<HEX<<strData<<"]ʱ��["<<buffer<<"]�洢�ɹ�"<<endl;
	return 0;
}

/** @brief ��������д�뺯��
 *  @param[in] pTaskInfo ������Ϣ
 *  @param[in] pTaskCtrlStatus ���������Ϣ
 *  @param[in] FreezeTime ���ݶ���ʱ��
 *  @return <0:ʧ��;=0:�ɹ�
 */
int C_Class2Data::WriteTermData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus, S_TIME FreezeTime)
{
	char buffer[32];
	S_WORD MP = pTaskCtrlStatus->m_mpMeter;
	if (pTaskInfo->m_CommRegs.size() != pTaskCtrlStatus->m_strDatas.size())
	{
		logwt<<"������["<<MP<<"]��������Ĵ�����ƥ��"<<endl;
		return -1;//���ݲ�ƥ��
	}
	if (pTaskCtrlStatus->m_strDatas.Any() == false)
	{
		logwt<<"������["<<MP<<"]������Ϊ��"<<endl;
		return -1;//����һ����û��
	}
	S_MSDI msdi = STATUS.FindMasterStationDataID(CLASSDATA_2, pTaskInfo->m_MSDI);
	if (msdi.m_Enable == false)
		return -1;

	string strData;
	if (msdi.SerializeData(pTaskInfo->m_CommRegs, pTaskCtrlStatus->m_strDatas, strData) <= 0)
	{
		logwt<<"������["<<MP<<"]�����ݴ��л�ʧ��"<<endl;
		return -1;
	}
	S_DataDensity DataDensity = GetDataDensity(msdi.m_Freeze, pTaskInfo->m_TimeUnit, pTaskInfo->m_PeriodVal);
	if (C_ENERGY::Write(msdi.m_DI, MP, strData, FreezeTime, DataDensity) < 0)
	{
		logwt<<"������["<<MP<<"]������["<<HEX<<strData<<"]�洢ʧ��"<<endl;
		return -1;
	}
	C_TIME::Time2Ascii14(FreezeTime, buffer);
	logwt<<"������["<<MP<<"]������["<<HEX<<strData<<"]ʱ��["<<buffer<<"]�洢�ɹ�"<<endl;
	return 0;
}

/** @brief �������ݶ�ȡ����
 *  @param[in] FN ���ݱ�ʶ
 *  @param[in] MP ������
 *  @param[out] strData �����������
 *  @param[in] DataTime ʱ��
 *  @return <0:ʧ��;=0:�ɹ�
 */
int C_Class2Data::Read(S_DWORD FN, S_WORD MP, string &strData, S_TIME DataTime)
{
	char buffer[32];
	string strMyData;
	C_TIME::Time2Ascii14(DataTime, buffer);

	if (C_ENERGY::ReadClass2Data(MP, FN, DataTime, strMyData) > 0)
	{
		strData.append(strMyData);
		logwt<<"������["<<MP<<"]������["<<HEX<<FN<<"="<<strData<<"]ʱ��["<<buffer<<"]��ȡ�ɹ�"<<endl;
		return 0;
	}
	/** �����ݿ�������ȡ */
	S_NwDataID NwDataId;
	if (FindReserveDataId(FN, NwDataId) == true)
	{
		strMyData.clear();
		if (C_ENERGY::ReadClass2Data(MP, NwDataId.m_DI, DataTime, strMyData) > 0)
		{
			if (ChangeDataFormat(FN, NwDataId, strMyData, strData) == true)
			{
				logwt<<"������["<<MP<<"]������["<<HEX<<FN<<"="<<strData<<"]ʱ��["<<buffer<<"]��ȡ�ɹ�"<<endl;
				return 0;
			}
		}
	}

	logwt<<"������["<<MP<<"]������["<<HEX<<FN<<"]ʱ��["<<buffer<<"]��ȡʧ��"<<endl;
	return -1;
}

/** @brief ��ȡ���ݴ洢�ܶȺ���
 *  @param[in] FN ���ݱ�ʶ
 *  @return <=0:��ȡʧ��; >0:���ݵĴ洢�ܶ�
 */
int C_Class2Data::GetDensity(INT32U FN)
{
	return C_ENERGY::GetDensity(FN);
}

/** @brief ���ұ���������
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
		strMyData.erase(strMyData.begin()); // ȥ������
	S_DWORD TariffCnt = FN & 0x000000FF;
	if ((FN | 0x0000FF00) == NwDataId.m_DI) // �ڶ��ֽ��Ƿ���
		TariffCnt = (FN >> 8) & 0x000000FF;
	if (NwDataId.m_type == NW_DATA_ID_TYPE_SUM_N)
		TariffCnt -= 1; // û���ܵ����ݿ飬��Ŵ�1��ʼ������������ʴ�0��ʼ

	strings strMyDatas;
	if (strMyDatas.split(strMyData, (size_t)NwDataId.m_ItemLen) <= 0)
		return false;
	if ((S_DWORD)strMyDatas.size() < (TariffCnt+1))
		return false;

	strData.append(strMyDatas[TariffCnt]);
	return true;
}

/** @brief ��ȡ�����ܶ�
 *  @param[in] PeriodVal ����ֵ
 *  @param[in] TimeUnit ���ڵ�λ
 *  @return һ��ĵ���
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
			case 1:return DATA_DENSITY_MIN01; // 1 ����
			case 5:return DATA_DENSITY_MIN05; // 5 ����
			case 15:return DATA_DENSITY_MIN15; // 15 ����
			case 30:return DATA_DENSITY_MIN30; // 30 ����
			case 60:return DATA_DENSITY_MIN60; // 60 ����
			default:return DATA_DENSITY_MIN60; 
			}
		}
	}
	return DATA_DENSITY_MIN60;
}


//
//
//
/** @brief д��澯
 *  @param[in] MP ������
 *  @param[in] DI ���ݱ�ʶ
 *  @param[in] strReocrd �澯��������
 *  @return -1:����; >=0:д��ļ�¼����
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
	{// flash ��д��֪ͨ��ʾͼ����˸
		S_LcdStatus *pLcdStatus = CONTEXT.GetLcdStatus();
		pLcdStatus->m_timeFlashStorage = C_TIME::TimeNow();
	}
	return ok;
}

/** @brief �����澯
 *  @param[in] MP ������
 *  @param[in] DI ���ݱ�ʶ
 *  @param[out] strReocrds �澯��������
 *  @param[in] timeBegin ��ʼʱ��
 *  @param[in] timeEnd ����ʱ�䣬ǰ�պ�
 *  @return -1:����; >=0:�����ļ�¼����
 */
int C_AlmEvt::Read(S_WORD MP, S_DWORD DI, strings &strReocrds, S_TIME timeBegin, S_TIME timeEnd)
{
	int ok = _Read(MP, DI, strReocrds, timeBegin, timeEnd);
	if (ok > 0)
	{// flash ��д��֪ͨ��ʾͼ����˸
		S_LcdStatus *pLcdStatus = CONTEXT.GetLcdStatus();
		pLcdStatus->m_timeFlashStorage = C_TIME::TimeNow();
	}

	S_AlarmStatus *pAS = STATUS.GetAlarmStatus();
	pAS->Flush();
	return ok;
}

/** @brief ����δ�ϱ��澯
 *  @param[in] AlmType �澯����
 *  @param[out] strReocrds �澯��������
 *  @return -1:����; >=0:�����ļ�¼����
 */
int C_AlmEvt::Report(S_AlmType AlmType, strings &strReocrds)
{
	int ok = _Report(AlmType, strReocrds);
	if (ok > 0)
	{// flash ��д��֪ͨ��ʾͼ����˸
		S_LcdStatus *pLcdStatus = CONTEXT.GetLcdStatus();
		pLcdStatus->m_timeFlashStorage = C_TIME::TimeNow();
	}

	S_AlarmStatus *pAS = STATUS.GetAlarmStatus();
	pAS->Flush();
	return ok;
}

/** @brief ͳ���¸澯����
 *  @param[in] AlmType �澯����
 *  @return -1:����; >=0:�¸澯����
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

/** @brief ȡ�澯�洢����
 *  @param[in] DI �澯������
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

/** @brief ȡ�澯�洢����
 *  @param[in] DI �澯������
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

/** @brief �����澯
 *  @param[in] MP ������
 *  @param[in] DI ���ݱ�ʶ
 *  @param[out] strReocrds �澯��������
 *  @param[in] timeBegin ��ʼʱ��
 *  @param[in] timeEnd ����ʱ�䣬ǰ�պ�
 *  @return -1:����; >=0:�����ļ�¼����
 */
int C_AlmEvt::_Read(S_WORD MP, S_DWORD DI, strings &strReocrds, S_TIME timeBegin, S_TIME timeEnd)
{
	S_AlmType AlmType = GetAlarmStoryType(DI);
	if (DI == DI_ALM_EVENT_ALL) //���������¼���¼
	{
//		MP = 0xFFFF, 
		DI = 0xFFFFFFFFUL;
		AlmType = ALM_TYPE_EVENT;
	}
	else if (DI == DI_ALM_ALARM_UNREPORT) //��������������δ�ɹ��ĸ澯����
	{
//		MP = 0xFFFF, 
		DI = 0xFFFFFFFFUL;
		timeBegin = 0, timeEnd = 0;
		AlmType = ALM_TYPE_IMPORTANT;
	}
	else if (DI == DI_ALM_ALARM_ALL) //���и澯����
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

/** @brief ����δ�ϱ��澯
 *  @param[in] AlmType �澯����
 *  @param[out] strReocrds �澯��������
 *  @return -1:����; >=0:�����ļ�¼����
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


