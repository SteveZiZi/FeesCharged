/** @file
 *  @brief �ն˼Ĵ����ɼ���
 *  @author 
 *  @date 2013/03/09
 *  @version 0.1
 *  @note �ն˼Ĵ����ɼ��ඨ���ʵ��
 */
#ifndef __TERM_COMMUNICATION_H__
#define __TERM_COMMUNICATION_H__
#include "metercom.h"

/** @class
 *  @brief �ն˼Ĵ����ɼ�
 */
class C_TermCom:public C_MeterCom
{
public:
	C_TermCom(void);
	virtual ~C_TermCom();

public:
	/** @fn
	 *  @brief ��ʼ���������õı�ƹ�Լ
	 *  @return 0:�ɹ�; -1:ʧ��
	 */
	virtual int InitMeterRules(void);

	/** @brief �������
	 *  @param[in] MeterRuleID ��ƹ�ԼID
	 *  @return true �ɹ���false ʧ��
	 *  @note ����Read�������������쳣ʱ��Ҫ�˲���
	 */
	virtual bool ClearBuffer(int MeterRuleID);
	/** @brief ���������
	 *  @param[in] Meter �����Ϣ
	 *  @param[in] CommReg ͨ�üĴ���
	 *  @param[out] strData �������
	 *  @return <0:����>=0:�ɹ�
	 */
	virtual int Read(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData);
	/** @brief д��Ʋ���
	 *  @param[in] Meter �����Ϣ
	 *  @param[in] CommReg ͨ�üĴ���
	 *  @param[in] strParam �������
	 *  @param[out] strData �������
	 *  @return <0:����>=0:�ɹ�
	 */
	virtual int Write(const S_MeterBase &Meter, const S_REGISTER &CommReg, const string &strParam, string &strData);
	/** @brief �ϱ��������
	 *  @param[in] CommReg ͨ�üĴ���
	 *  @param[out] strMeterAddress ��Ƶ�ַ���[6BYTES]
	 *  @param[out] strData �������
	 *  @param[in] timeKeeping �ϱ�ʱ��;0:�˳��ϱ�;0xFFFFFFFF:��ѯ;����:�����ϱ�
	 *  @return <0:���� 0:�����ϱ� >0:�ϱ�ʣ��ʱ��
	 */
	virtual int Report(const S_REGISTER &CommReg, string &strMeterAddress, string &strData, S_TIME timeKeeping);
	/** @brief �㲥���ʱ��
	 *  @param[in] Meter �����Ϣ
	 *  @param[in] timeExpected ����ʱ��
	 *  @return <0:����>=0:�ɹ�
	 */
	virtual int Broadcast(const S_MeterBase &Meter, S_TIME timeExpected);

protected:
	/** @brief ���������
	 *  @param[in] Meter �����Ϣ
	 *  @param[in] CommReg ͨ�üĴ���
	 *  @param[out] strData �������
	 *  @param[in] fRsvReg true���üĴ���;false���Ĵ���
	 *  @return <0:[MRULE_PARSE_ERR_]����>=0:�ɹ�
	 */
	int _Read(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData, bool fRsvReg);
};

#endif//__TERM_COMMUNICATION_H__


