/** @file
 *  @brief �ն˼Ĵ��������ʵ��
 *  @author 
 *  @date 2013/03/09
 *  @version 0.1
 *  @note �ն�����������ͳ�ƵȵȼĴ���
 */
#include "termreg.h"
#include "status.h"
#include "context.h"

//
// �ն˼Ĵ�������
//
/** @fn
 *  @brief ��ȡ�Ĵ�������
 *  @return 0:�ɹ�; -1:ʧ��
 */
int C_TermReg::GetData(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData)
{
	return -1;
}
//10010000	BIN	4	NNNNNNNN	����ͨѶ����
int C_TermReg1001::GetData(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData)
{
	S_DWORD Flux = STATUS.GetDayCommFlux();
	strData.push_back(kge_cbcd((S_BYTE)(Flux%100)));
	strData.push_back(kge_cbcd((S_BYTE)(Flux/100%100)));
	strData.push_back(kge_cbcd((S_BYTE)(Flux/10000%100)));
	strData.push_back(kge_cbcd((S_BYTE)(Flux/1000000%100)));
	return 0;
}
//10020000	BIN	4	NNNNNNNN	����ͨѶ����
int C_TermReg1002::GetData(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData)
{
	S_DWORD Flux = STATUS.GetMonCommFlux();
	strData.push_back(kge_cbcd((S_BYTE)(Flux%100)));
	strData.push_back(kge_cbcd((S_BYTE)(Flux/100%100)));
	strData.push_back(kge_cbcd((S_BYTE)(Flux/10000%100)));
	strData.push_back(kge_cbcd((S_BYTE)(Flux/1000000%100)));
	return 0;
}
//10030000	BIN	2	NNNNNNNN	����ͨѶ����
int C_TermReg1003::GetData(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData)
{
	S_DWORD Flux = (STATUS.GetDayCommFreeze() + 1023) / 1024;
	Flux = KGE_COMBWORD(kge_cbcd((S_BYTE)(Flux/100%100)), kge_cbcd((S_BYTE)(Flux%100)));
	strData.append((char *)&Flux, 2);
	return 0;
}
//10040000	BIN	3	NNNNNNNN	����ͨѶ����
int C_TermReg1004::GetData(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData)
{
	S_DWORD Flux = (STATUS.GetMonCommFreeze() + 1023) / 1024;
	strData.push_back(kge_cbcd((S_BYTE)(Flux%100)));
	strData.push_back(kge_cbcd((S_BYTE)(Flux/100%100)));
	strData.push_back(kge_cbcd((S_BYTE)(Flux/10000%100)));
	return 0;
}
//10050000	BIN	2	NNNN	�ն˵��չ���ʱ�䣨min��
int C_TermReg1005::GetData(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData)
{
	S_WORD Count = 0x00;//STATUS.GetDayPowerSupplyM();
	Count = KGE_COMBWORD(kge_cbcd(Count/100), kge_cbcd(Count%100));
	strData.append((char *)&Count, 2);
	return 0;
}
//10060000	BIN	2	NNNN	�ն˵��¹���ʱ�䣨min��
int C_TermReg1006::GetData(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData)
{
	S_WORD Count = 0x00;//STATUS.GetMonPowerSupplyM();
	Count = KGE_COMBWORD(kge_cbcd(Count/100), kge_cbcd(Count%100));
	strData.append((char *)&Count, 2);
	return 0;
}
//10070000	BIN	2	NNNN	�ն����չ���ʱ�䣨min��
int C_TermReg1007::GetData(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData)
{
	S_WORD Count = 0x00;//STATUS.GetFreezeDayPowerSupplyM();
	Count = KGE_COMBWORD(kge_cbcd(Count/100), kge_cbcd(Count%100));
	strData.append((char *)&Count, 2);
	return 0;
}
//10080000	BIN	2	NNNN	�ն����¹���ʱ�䣨min��
int C_TermReg1008::GetData(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData)
{
	S_WORD Count = 0x00;//STATUS.GetFreezeMonPowerSupplyM();
	Count = KGE_COMBWORD(kge_cbcd(Count/100), kge_cbcd(Count%100));
	strData.append((char *)&Count, 2);
	return 0;
}
//10090000	BIN	2	NNNN	�ն˵��ո�λ�ۼƴ���
int C_TermReg1009::GetData(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData)
{
	S_TermData *pTermData = STATUS.GetTermData();
	S_WORD Count = pTermData->m_ResetData.m_DayResetTimes;
	Count = KGE_COMBWORD(kge_cbcd(Count/100), kge_cbcd(Count%100));
	strData.append((char *)&Count, 2);
	return 0;
}
//100A0000	BIN	2	NNNN	�ն˵��¸�λ�ۼƴ���
int C_TermReg100A::GetData(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData)
{
	S_TermData *pTermData = STATUS.GetTermData();
	S_WORD Count = pTermData->m_ResetData.m_MonResetTimes;
	Count = KGE_COMBWORD(kge_cbcd(Count/100), kge_cbcd(Count%100));
	strData.append((char *)&Count, 2);
	return 0;
}
//100B0000	BIN	2	NNNN	�ն����ո�λ�ۼƴ���
int C_TermReg100B::GetData(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData)
{
	S_WORD Count = STATUS.GetFreezeDayResetTimes();
	Count = KGE_COMBWORD(kge_cbcd(Count/100), kge_cbcd(Count%100));
	strData.append((char *)&Count, 2);
	return 0;
}
//100C0000	BIN	2	NNNN	�ն����¸�λ�ۼƴ���
int C_TermReg100C::GetData(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData)
{
	S_WORD Count = STATUS.GetFreezeMonResetTimes();
	Count = KGE_COMBWORD(kge_cbcd(Count/100), kge_cbcd(Count%100));
	strData.append((char *)&Count, 2);
	return 0;
}



//
// �ն˼Ĵ�������
//
static C_TermReg1001 TermReg1001; //10010000	BIN	4	NNNNNNNN	����ͨѶ����
static C_TermReg1002 TermReg1002; //10020000	BIN	4	NNNNNNNN	����ͨѶ����
static C_TermReg1003 TermReg1003; //10030000	BIN	4	NNNNNNNN	����ͨѶ����
static C_TermReg1004 TermReg1004; //10040000	BIN	4	NNNNNNNN	����ͨѶ����
static C_TermReg1005 TermReg1005; //10050000	BIN	2	NNNN	�ն˵��չ���ʱ�䣨min��
static C_TermReg1006 TermReg1006; //10060000	BIN	2	NNNN	�ն˵��¹���ʱ�䣨min��
static C_TermReg1007 TermReg1007; //10070000	BIN	2	NNNN	�ն����չ���ʱ�䣨min��
static C_TermReg1008 TermReg1008; //10080000	BIN	2	NNNN	�ն����¹���ʱ�䣨min��
static C_TermReg1009 TermReg1009; //10090000	BIN	2	NNNN	�ն˵��ո�λ�ۼƴ���
static C_TermReg100A TermReg100A; //100A0000	BIN	2	NNNN	�ն˵��¸�λ�ۼƴ���
static C_TermReg100B TermReg100B; //100B0000	BIN	2	NNNN	�ն����ո�λ�ۼƴ���
static C_TermReg100C TermReg100C; //100C0000	BIN	2	NNNN	�ն����¸�λ�ۼƴ���

static S_TermRegMap sTermRegMap[] =
{
	{0x10010000, &TermReg1001}, //10010000	BIN	4	NNNNNNNN	����ͨѶ����
	{0x10020000, &TermReg1002}, //10020000	BIN	4	NNNNNNNN	����ͨѶ����
	{0x10030000, &TermReg1003}, //10030000	BIN	4	NNNNNNNN	����ͨѶ����
	{0x10040000, &TermReg1004}, //10040000	BIN	4	NNNNNNNN	����ͨѶ����
	{0x10050000, &TermReg1005}, //10050000	BIN	2	NNNN	�ն˵��չ���ʱ�䣨min��
	{0x10060000, &TermReg1006}, //10060000	BIN	2	NNNN	�ն˵��¹���ʱ�䣨min��
	{0x10070000, &TermReg1007}, //10070000	BIN	2	NNNN	�ն����չ���ʱ�䣨min��
	{0x10080000, &TermReg1008}, //10080000	BIN	2	NNNN	�ն����¹���ʱ�䣨min��
	{0x10090000, &TermReg1009}, //10090000	BIN	2	NNNN	�ն˵��ո�λ�ۼƴ���
	{0x100A0000, &TermReg100A}, //100A0000	BIN	2	NNNN	�ն˵��¸�λ�ۼƴ���
	{0x100B0000, &TermReg100B}, //100B0000	BIN	2	NNNN	�ն����ո�λ�ۼƴ���
	{0x100C0000, &TermReg100C}, //100C0000	BIN	2	NNNN	�ն����¸�λ�ۼƴ���

	{0x1FFF0000, NULL}
};
C_TermReg *C_TermRegFact::GetTermRegister(S_DWORD RegID)
{
	const S_TermRegMap CmdObjNow = {RegID, NULL};
	S_TermRegMap *pFindObj = lower_bound(sTermRegMap, sTermRegMap+KGE_MEMBER_OF(sTermRegMap), CmdObjNow);
	if (pFindObj != sTermRegMap+KGE_MEMBER_OF(sTermRegMap) && pFindObj->m_RegID == RegID)
		return pFindObj->m_pTermRegObj;

	return NULL;
}


