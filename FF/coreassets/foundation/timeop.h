/** @file
 *  @brief ʱ�������
 *  @author 
 *  @date 2011/11/07
 *  @version 0.1
 *  1��ʱ�������-���Ϲ�����Լ��ʽssmmhhDDMMYYYY
 *  2��ʱ�������-����������Լ��ʽYYYYMMDDhhmmss
 */
#ifndef __FND_TIMEOP_H__
#define __FND_TIMEOP_H__
#include "basedef.h"

struct S_DATE
{
	int m_Year; ///< ��ݣ�ȡֵ����Ϊ[0,9999]
	int m_Mon; ///< �·ݣ���һ�¿�ʼ��1����һ�£� - ȡֵ����Ϊ[1,12] 
	int m_Day; ///< һ�����е����� - ȡֵ����Ϊ[1,31] 
	int m_Hour; ///< ʱ - ȡֵ����Ϊ[0,23] 
	int m_Min; ///< �� - ȡֵ����Ϊ[0,59] 
	int m_Sec; ///< ��Cȡֵ����Ϊ[0,59] 
	int m_WeekDay; ///< ���ڨCȡֵ����Ϊ[0,6]������0���������죬1��������һ���Դ�����
	int m_YearDay; ///< ��ÿ���1��1�տ�ʼ�������Cȡֵ����Ϊ[1,366]������1����1��1�գ�2����1��2�գ��Դ�����

	S_DATE(void){memset(this, 0x00, sizeof(*this));}
};

struct S_TIMEX
{
	S_TIME m_Second;
	S_TIME m_MS;
};

/** ʱ�䵥λ */
enum S_TIME_UNIT{
	TIME_UNIT_UNKNOW,
	TIME_UNIT_SECOND,
	TIME_UNIT_MINUTE,
	TIME_UNIT_HOUR,
	TIME_UNIT_DAY,
	TIME_UNIT_MONTH,
	TIME_UNIT_YEAR
};

#define TIME_UNIT_NAME_UNKNOW	"unknow"
#define TIME_UNIT_NAME_SECOND	"second"
#define TIME_UNIT_NAME_MINUTE		"minute"
#define TIME_UNIT_NAME_HOUR		"hour"
#define TIME_UNIT_NAME_DAY		"day"
#define TIME_UNIT_NAME_MONTH		"month"
#define TIME_UNIT_NAME_YEAR		"year"

/** @class
 *  @brief ʱ���������
 *  @remarks ���й������������
 */
class C_TIME
{
public:
	static S_TIME_UNIT GetTimeUnit(const char *pUnitName);
	static const char* GetTimeUnit(S_TIME_UNIT TimeUnit);

	/** ȡ��ǰʱ��(��װtime(NULL)) */
	static S_TIME TimeNow(void);
	static void TimeNow(S_TIMEX &timeX);
	static S_TIME MakeTime(const S_DATE &date);
	static S_DATE LocalTime(S_TIME mytime);
	static S_DATE *LocalTime(S_TIME mytime, S_DATE &date);

	/** ����BCD���ĸ�ʽΪ:ssmmhhDDMMYYYY ���Ϲ�����Լ*/
	static char *CurTime2BCDStr(char *bcdStr7B){return Time2BCDStr(TimeNow(), bcdStr7B);}
	static char *Time2BCDStr(S_TIME time, char *bcdStr7B);
	static S_TIME BcdStr2Time(const char *bcdStr7B);
	static S_TIME BcddateAdd(S_TIME_UNIT interUnit, long offset, const char *srcBcd7B, char *dstBcd7B);
	static int SetHWClock(const char *bcdStr7B);
	static int SyncHWClock(void);
	//bcdStr5B:mmhhDDMMYY
	static S_TIME BcdStr2Time5B(const char *bcdStr5B);
	static char* Time2BcdStr5B(S_TIME time, char *bcdStr5B);

	/** ����BCD���ĸ�ʽΪ:YYYYMMDDhhmmss ����������Լ*/
	static char *CurTime2BCDStr_Big(char *bcdStr7B){return Time2BCDStr_Big(TimeNow(), bcdStr7B);}
	static char *Time2BCDStr_Big(S_TIME time, char *bcdStr7B);
	static S_TIME BcdStr2Time_Big(const char *bcdStr7B);
	static int SetHWClock_Big(const char *bcdStr7B);
	//bcdStr5B:YYMMDDHHMM
	static S_TIME BcdStr2Time5B_Big(const char *bcdStr5B);
	static char* Time2BcdStr5B_Big(S_TIME time, char *bcdStr5B);
	//bcdStr6B:YYYYMMDDHHMM
	static S_TIME BcdStr2Time6B_Big(const char *bcdStr6B);
	static char* Time2BcdStr6B_Big(S_TIME time, char *bcdStr6B);
	static S_TIME BcddateAdd_Big(S_TIME_UNIT interUnit, long offset, const char *srcBcd7B, char *dstBcd7B);


	/** @brief ���ڶ���
	 *  @param[in] interUnit ʱ�䵥λ
	 *  @param[in] date ����
	 *  @return �����������
	 */
	static S_TIME DateTrim(S_TIME_UNIT interUnit, S_TIME date);
	/** @brief ���ڶ���
	 *  @param[in] interUnit ʱ�䵥λ
	 *  @param[in] offset ��������ڣ���5��10
	 *  @param[in] date ����
	 *  @return �����������
	 */
	static S_TIME DateTrim(S_TIME_UNIT interUnit, S_WORD offset, S_TIME timeDate);
	/** @breif ���ڵ�λʱ������
	 *  @param[in] interUnit ���ڵ�λ
	 *  @param[in] offset ʱ��ƫ��
	 *  @param[in] date ����
	 *  @return ����
	 */
	static S_TIME DateAdd(S_TIME_UNIT interUnit, long offset, S_TIME date);
	/** @brief �������ڼ�ĵ�λ��
	 *  @param[in] interUnit ʱ�䵥λ
	 *  @param[in] begin ��ʼʱ��
	 *  @param[in] end ����ʱ��
	 *  @return >= 0 (end>=begin)
	 *  @return <0 (end<begin)
	 */
	static long DateDiff(S_TIME_UNIT interUnit, S_TIME begin, S_TIME end);

	/** @brief ȡʱ�䴮����ʽ:YYYY-MM-DD hh:mm:ss
	 *  @param[out] strAscii20B ��ʽ���������
	 *  @param[in] time ʱ��
	 *  @return strAscii20B
	 */
	static char *Time2YMDhms(S_TIME time, char *strAscii20B);
	/** @brief ȡʱ�䴮����ʽ:MM-DD hh:mm:ss
	 *  @param[out] strAscii14B ��ʽ���������
	 *  @return strAscii14B
	 */
	static char *Now2Stamp(char *strAscii14B);
	/** @brief ȡʱ�䴮����ʽ:YYYYMMDDhhmm
	 *  @param[out] strAscii12B ��ʽ���������
	 *  @return strAscii12B
	 */
	static char *Now2Ascii12(char *strAscii12B);

	/** @brief ȡʱ�䴮����ʽ:YYYYMMDDhhmmss
	 *  @param[out] strAscii16B ��ʽ���������
	 *  @param[in] time ʱ��
	 *  @return strAscii16B
	 */
	static char* Time2Ascii14(S_TIME time, char *strAscii16B);

	/** @brief ���ʱ���ʽ�Ƿ����������ʽ����13���ǲ��Ե�
	 *  @param[in] date ʱ�����
	 *  @return true ����; false �쳣
	 */
	static bool CheckTime(const S_DATE &date);
};

/** @class
 *  @brief ��ʱ������
 */
class C_MSTIMOUT
{
public:
	C_MSTIMOUT(S_DWORD ms);
	~C_MSTIMOUT();

	/** @brief ��ʱ�ж�
	 *  @return true ��ʱ��false δ��ʱ
	 */
	S_BOOL timeout(void);

private:
	S_TIME m_timeStart; ///< ��ʼʱ��
	S_DWORD m_timout; ///< ��ʱ�������λ�룩
};

/** @class
 *  @brief ʱ�ӽ��Ķ���
 */
class C_TimeTick
{
public:
	C_TimeTick(S_WORD span);
	C_TimeTick(void);

	bool TickNow(void);

private:
	S_WORD m_Span;
	S_TIME m_timePrev;
};

#endif //__FND_TIMEOP_H__


