/**
 *  @file
 *  @brief �澯�����ļ���ȡ������
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
/** @brief �����ļ���ʽ
 * �ļ�ͷ��ʽ���� 
 * |�ļ���ʶ|�ļ�����ʱ��|�洢����|��¼����|�汾��Ϣ|����|У��|
 * |---16---|------8-----|---2----|---2----|---6----|-12-|--2-|
 * �澯��������־��(ÿ����λ��5�ֽڵ�ʱ���ʶ+1�ֽڵı�־)
 * |�澯1|�澯2|�澯3|�澯4|.....|
 * |--6--|--6--|--6--|--6--|.....|
 * �澯��������ʽ(����<�洢����>��������)
 * |�澯��������ʶ|�澯��¼ʱ��|������|���ݱ�ʶ|���ݳ���|�澯����|
 * |-------6------|------5-----|---2--|----4---|----2---|---200--|
 */

//
//
//
struct S_AlarmHeader
{//ע���ļ�ͷ��Ҫ����48�ֽ�
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
	/** @brief �����¼
	 *  @param[in] MP ������
	 *  @param[in] DI ���ݱ�ʶ
	 *  @param[in] strReocrd �澯��������
	 *  @param[in] timeRecord ��¼ʱ��
	 *  @return -1:����; >=0:д��ļ�¼����
	 */
	int AddRecord(S_WORD MP, S_DWORD DI, const string &strReocrd, S_TIME timeRecord);

	/** @brief ������¼
	 *  @param[in] MP ������,ͨ���0xFFFF
	 *  @param[in] DI ���ݱ�ʶ,ͨ���0xFFFFFFFF
	 *  @param[out] strReocrds �澯��������(���������ʱ�����µ�����ǰ)
	 *  @param[in] timeBegin ��ʼʱ��,��Ϊ0��ȡδ�ϱ��ļ�¼
	 *  @param[in] timeEnd ����ʱ�䣬ǰ�պ�
	 *  @return -1:����; >=0:�����ļ�¼����
	 */
	int GetRecord(S_WORD MP, S_DWORD DI, strings &strReocrds, S_TIME timeBegin, S_TIME timeEnd);

	/** @brief ͳ���¸澯����
	 *  @return -1:����; >=0:�¸澯����
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

	/** @brief �����¼
	 *  @param[in] MP ������
	 *  @param[in] DI ���ݱ�ʶ
	 *  @param[in] strReocrd �澯��������
	 *  @return -1:����; >=0:д��ļ�¼����
	 */
	int AddAlarm(S_WORD MP, S_DWORD DI, const string &strReocrd);

	/** @brief ������¼
	 *  @param[in] MP ������,ͨ���0xFFFF
	 *  @param[in] DI ���ݱ�ʶ,ͨ���0xFFFFFFFF
	 *  @param[out] strReocrds �澯��������(���������ʱ�����µ�����ǰ)
	 *  @param[in] timeBegin ��ʼʱ��,��Ϊ0��ȡδ�ϱ��ļ�¼
	 *  @param[in] timeEnd ����ʱ�䣬ǰ�պ�
	 *  @return -1:����; >=0:�����ļ�¼����
	 */
	int GetAlarm(S_WORD MP, S_DWORD DI, strings &strReocrds, S_TIME timeBegin, S_TIME timeEnd);

	/** @brief ͳ���¸澯����
	 *  @return -1:����; >=0:�¸澯����
	 */
	int GetNewAlarmCnt(void){return GetUnreportCnt();}
};

class C_AlarmNormal:public C_AlarmDB
{
public:
	C_AlarmNormal(S_WORD RecordCntMax);
	~C_AlarmNormal();

	/** @brief �����¼
	 *  @param[in] MP ������
	 *  @param[in] DI ���ݱ�ʶ
	 *  @param[in] strReocrd �澯��������
	 *  @return -1:����; >=0:д��ļ�¼����
	 */
	int AddAlarm(S_WORD MP, S_DWORD DI, const string &strReocrd);

	/** @brief ������¼
	 *  @param[in] MP ������,ͨ���0xFFFF
	 *  @param[in] DI ���ݱ�ʶ,ͨ���0xFFFFFFFF
	 *  @param[out] strReocrds �澯��������(���������ʱ�����µ�����ǰ)
	 *  @param[in] timeBegin ��ʼʱ��,��Ϊ0��ȡδ�ϱ��ļ�¼
	 *  @param[in] timeEnd ����ʱ�䣬ǰ�պ�
	 *  @return -1:����; >=0:�����ļ�¼����
	 */
	int GetAlarm(S_WORD MP, S_DWORD DI, strings &strReocrds, S_TIME timeBegin, S_TIME timeEnd);

	/** @brief ͳ���¸澯����
	 *  @return -1:����; >=0:�¸澯����
	 */
	int GetNewAlarmCnt(void){return GetUnreportCnt();}
};

class C_EVENT:public C_AlarmDB
{
public:
	C_EVENT(S_WORD RecordCntMax);
	~C_EVENT();

	/** @brief �����¼
	 *  @param[in] MP ������
	 *  @param[in] DI ���ݱ�ʶ
	 *  @param[in] strReocrd �澯��������
	 *  @return -1:����; >=0:д��ļ�¼����
	 */
	int AddEvent(S_WORD MP, S_DWORD DI, const string &strReocrd);

	/** @brief ������¼
	 *  @param[in] MP ������,ͨ���0xFFFF
	 *  @param[in] DI ���ݱ�ʶ,ͨ���0xFFFFFFFF
	 *  @param[out] strReocrds �澯��������(���������ʱ�����µ�����ǰ)
	 *  @param[in] timeBegin ��ʼʱ��,��Ϊ0��ȡδ�ϱ��ļ�¼
	 *  @param[in] timeEnd ����ʱ�䣬ǰ�պ�
	 *  @return -1:����; >=0:�����ļ�¼����
	 */
	int GetEvent(S_WORD MP, S_DWORD DI, strings &strReocrds, S_TIME timeBegin, S_TIME timeEnd);

	/** @brief ͳ���¸澯����
	 *  @return -1:����; >=0:�¸澯����
	 */
	int GetNewEventCnt(void){return GetUnreportCnt();}
};


#endif //__NW_ALARM_FILE_BASE_H__


