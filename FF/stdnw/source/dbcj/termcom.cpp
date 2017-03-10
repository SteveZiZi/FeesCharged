/** @file
 *  @brief �ն˼Ĵ����ɼ���
 *  @author 
 *  @date 2013/03/09
 *  @version 0.1
 *  @note �ն˼Ĵ����ɼ��ඨ���ʵ��
 */
#include "termcom.h"
#include "htrace.h"
#include "termreg.h"
#include "mrule.h"

//
// �ն˼Ĵ����ɼ�
//
C_TermCom::C_TermCom(void)
{
}

C_TermCom::~C_TermCom()
{
}

/** @fn
 *  @brief ��ʼ���������õı�ƹ�Լ
 *  @return 0:�ɹ�; -1:ʧ��
 */
int C_TermCom::InitMeterRules(void)
{
	return 0;
}

/** @brief �������
 *  @param[in] MeterRuleID ��ƹ�ԼID
 *  @return true �ɹ���false ʧ��
 *  @note ����Read�������������쳣ʱ��Ҫ�˲���
 */
bool C_TermCom::ClearBuffer(int MeterRuleID)
{
	return true;
}

/** @brief ���������
 *  @param[in] Meter �����Ϣ
 *  @param[in] CommReg ͨ�üĴ���
 *  @param[out] strData �������
 *  @return <0:����>=0:�ɹ�
 */
int C_TermCom::Read(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData)
{
	int ok = _Read(Meter, CommReg, strData, false);
	if (ok == MRULE_PARSE_ERR_CTRL)
	{
		logwt<<"�ɼ����üĴ���["<<HEX<<CommReg.m_ID<<"]..."<<endl;
		ok = _Read(Meter, CommReg, strData, true);
	}
	else if (ok == MRULE_PARSE_ERR_UNSUPPORT)
	{
		logwt<<"���Ĵ���["<<HEX<<CommReg.m_ID<<"]..."<<endl;
		ok = CommReg.FillData(strData) > 0? 0:-1;
	}
	return ok==0? 0:-1;
}

/** @brief д��Ʋ���
 *  @param[in] Meter �����Ϣ
 *  @param[in] CommReg ͨ�üĴ���
 *  @param[in] strParam �������
 *  @param[out] strData �������
 *  @return <0:����>=0:�ɹ�
 */
int C_TermCom::Write(const S_MeterBase &Meter, const S_REGISTER &CommReg, const string &strParam, string &strData)
{
	return 0;
}

/** @brief �ϱ��������
 *  @param[in] CommReg ͨ�üĴ���
 *  @param[out] strMeterAddress ��Ƶ�ַ���[6BYTES]
 *  @param[out] strData �������
 *  @param[in] timeKeeping �ϱ�ʱ��;0:�˳��ϱ�;0xFFFFFFFF:��ѯ;����:�����ϱ�
 *  @return <0:���� 0:�����ϱ� >0:�ϱ�ʣ��ʱ��
 */
int C_TermCom::Report(const S_REGISTER &CommReg, string &strMeterAddress, string &strData, S_TIME timeKeeping)
{
	return 0;
}

/** @brief �㲥���ʱ��
 *  @param[in] Meter �����Ϣ
 *  @param[in] timeExpected ����ʱ��
 *  @return <0:����>=0:�ɹ�
 */
int C_TermCom::Broadcast(const S_MeterBase &Meter, S_TIME timeExpected)
{
	return 0;
}

/** @brief ���������
 *  @param[in] Meter �����Ϣ
 *  @param[in] CommReg ͨ�üĴ���
 *  @param[out] strData �������
 *  @param[in] fRsvReg true���üĴ���;false���Ĵ���
 *  @return <0:[MRULE_PARSE_ERR_]����>=0:�ɹ�
 */
int C_TermCom::_Read(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData, bool fRsvReg)
{
	C_TermReg *pTermReg = C_TermRegFact::GetTermRegister(CommReg.m_ID);
	if (pTermReg == NULL)
		return MRULE_PARSE_ERR_UNSUPPORT;
	if (pTermReg->GetData(Meter, CommReg, strData) != 0)
		return MRULE_PARSE_ERR_UNSUPPORT;
	return MRULE_PARSE_ERR_NO;
}


