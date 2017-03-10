/** @file
 *  @brief �����ļ��û���ӿ�
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
	S_DWORD m_DI; ///< ������ID
	S_BYTE m_type; ///< ���������� NW_DATA_ID_TYPE
	S_BYTE m_ItemLen; ///< ���ݿ������
};
bool operator<(const S_NwDataID &left, const S_NwDataID &right);

//
// �û��ӿڶ���
//
/**
 *  @class 
 *  @brief the interface of energy data file IO.
 */
class C_ENERGY
{
public:

	/** @brief ��������д�뺯��
	 *  @param[in] FN ���ݱ�ʶ
	 *  @param[in] MP ������
	 *  @param[in] strData ���ݼ�¼
	 *  @param[in] DataTime ʱ��
	 *  @param[in] DataDensity �����ܶ�
	 *  @return <0:ʧ��;=0:�ɹ�
	 */
	static int Write(S_DWORD FN, S_WORD MP, const string &strData, S_TIME DataTime, S_DataDensity DataDensity);
	/** @brief ��������д�뺯��
	 *  @param[in] FN ���ݱ�ʶ
	 *  @param[in] MeterDatas ���������飬�μ�S_EnergyDatas����
	 *  @param[in] DataTime ʱ��
	 *  @param[in] DataDensity �����ܶ�
	 *  @return д��ļ�¼����
	 */
	static int WriteClass2Data(INT32U FN, const S_EnergyDatas &MeterDatas, S_TIME DataTime, S_DataDensity DataDensity);
	/** @brief �������ݶ�ȡ����
	 *  @param[in] MP ���ݱ�ʶ
	 *  @param[in] FN ���ݱ�ʶ
	 *  @param[in] DataTime ʱ��
	 *  @param[out] strData ��������
	 *  @return ��ȡ�ļ�¼����
	 */
	static int ReadClass2Data(S_WORD MP, INT32U FN, S_TIME DataTime, string &strData);
	/** @brief �������ݶ�ȡ����
	 *  @param[in] FN ���ݱ�ʶ
	 *  @param[inout] MeterDatas ���������飬�μ�S_EnergyDatas����
	 *  @param[in] DataTime ʱ��
	 *  @return ��ȡ�ļ�¼����
	 */
	static int ReadClass2Data(INT32U FN, S_EnergyDatas &MeterDatas, S_TIME DataTime);

	/** @brief ��ȡ���ݴ洢�ܶȺ���
	 *  @param[in] FN ���ݱ�ʶ
	 *  @return <=0:��ȡʧ��; >0:���ݵĴ洢�ܶ�
	 */
	static int GetDensity(INT32U FN);
};

class C_Class2Data
{
public:
	/** @brief ��������д�뺯��
	 *  @param[in] pTaskInfo ������Ϣ
	 *  @param[in] pTaskCtrlStatus ���������Ϣ
	 *  @param[in] FreezeTime ���ݶ���ʱ��
	 *  @return <0:ʧ��;=0:�ɹ�
	 */
	static int Write(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus, S_TIME FreezeTime);

	/** @brief ��������д�뺯��
	 *  @param[in] pTaskInfo ������Ϣ
	 *  @param[in] pTaskCtrlStatus ���������Ϣ
	 *  @param[in] FreezeTime ���ݶ���ʱ��
	 *  @return <0:ʧ��;=0:�ɹ�
	 */
	static int WriteTermData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus, S_TIME FreezeTime);

	/** @brief �������ݶ�ȡ����
	 *  @param[in] FN ���ݱ�ʶ
	 *  @param[in] MP ������
	 *  @param[out] strData �����������
	 *  @param[in] DataTime ʱ��
	 *  @return <0:ʧ��;=0:�ɹ�
	 */
	static int Read(S_DWORD FN, S_WORD MP, string &strData, S_TIME DataTime);

	/** @brief ��ȡ���ݴ洢�ܶȺ���
	 *  @param[in] FN ���ݱ�ʶ
	 *  @return <=0:��ȡʧ��; >0:���ݵĴ洢�ܶ�
	 */
	static int GetDensity(INT32U FN);

private:
	/** @brief ���ұ���������
	 */
	static bool FindReserveDataId(S_DWORD FN, S_NwDataID &NwDataId);
	static bool _FindReserveDataId(S_DWORD FN, S_NwDataID &NwDataId);
	static bool ChangeDataFormat(S_DWORD FN, const S_NwDataID &NwDataId, const string &strRawData, string &strData);

	/** @brief ��ȡ�����ܶ�
	 *  @param[in] PeriodVal ����ֵ
	 *  @param[in] TimeUnit ���ڵ�λ
	 *  @return һ��ĵ���
	 */
	static S_DataDensity GetDataDensity(S_BYTE FreezeFlag, S_TIME_UNIT TimeUnit, S_BYTE PeriodVal);
};

/**
 *  @class 
 *  @brief �澯�¼������ӿ�
 */
class C_AlmEvt
{
public:
	/** @brief д��澯
	 *  @param[in] MP ������
	 *  @param[in] DI ���ݱ�ʶ
	 *  @param[in] strReocrd �澯��������
	 *  @return -1:����; >=0:д��ļ�¼����
	 */
	static int Write(S_WORD MP, S_DWORD DI, const string &strReocrd);
	/** @brief �����澯
	 *  @param[in] MP ������
	 *  @param[in] DI ���ݱ�ʶ
	 *  @param[out] strReocrds �澯��������(���������ʱ�����µ�����ǰ)
	 *  @param[in] timeBegin ��ʼʱ��,��Ϊ0��ȡδ�ϱ��ļ�¼
	 *  @param[in] timeEnd ����ʱ�䣬ǰ�պ�
	 *  @return -1:����; >=0:�����ļ�¼����
	 */
	static int Read(S_WORD MP, S_DWORD DI, strings &strReocrds, S_TIME timeBegin, S_TIME timeEnd);
	/** @brief ����δ�ϱ��澯
	 *  @param[in] AlmType �澯����
	 *  @param[out] strReocrds �澯��������
	 *  @return -1:����; >=0:�����ļ�¼����
	 */
	static int Report(S_AlmType AlmType, strings &strReocrds);
	/** @brief ͳ���¸澯����
	 *  @param[in] AlmType �澯����
	 *  @return -1:����; >=0:�¸澯����
	 */
	static int CountNew(S_AlmType AlmType);

	/** @brief ȡ�澯�洢����
	 *  @param[in] DI �澯������
	 *  @return ALM_TYPE_EVENT,ALM_TYPE_IMPORTANT,ALM_TYPE_NORMAL,ALM_TYPE_UNRECORD_ALARM
	 */
	static S_AlmType GetAlarmStoryType(S_DWORD DI);
	/** @brief ȡ�澯�洢����
	 *  @param[in] DI �澯������
	 *  @return ALM_TYPE_EVENT,ALM_TYPE_ALARM,ALM_TYPE_UNKNOW
	 */
	static S_AlmType GetAlarmNormalType(S_DWORD DI);

private:
	/** @brief �����澯
	 *  @param[in] MP ������
	 *  @param[in] DI ���ݱ�ʶ
	 *  @param[out] strReocrds �澯��������
	 *  @param[in] timeBegin ��ʼʱ��,��Ϊ0��ȡδ�ϱ��ļ�¼
	 *  @param[in] timeEnd ����ʱ�䣬ǰ�պ�
	 *  @return -1:����; >=0:�����ļ�¼����
	 */
	static int _Read(S_WORD MP, S_DWORD DI, strings &strReocrds, S_TIME timeBegin, S_TIME timeEnd);
	/** @brief ����δ�ϱ��澯
	 *  @param[in] AlmType �澯����
	 *  @param[out] strReocrds �澯��������
	 *  @return -1:����; >=0:�����ļ�¼����
	 */
	static int _Report(S_AlmType AlmType, strings &strReocrds);
};

#endif //__NW_DATA_FILE_H__


