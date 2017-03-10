/**
 *  @file
 *  @brief ���ͨѶ
 *  @author 
 *  @date 2012-05-21
 *  @version v1.0
 *  @note
 *  ��ɱ�Ʋɼ�����
 *  ��ɱ�ƣ��ֳ����ߣ������ϱ�����
 */
#ifndef __BASEB_METER_COMMUNICATION_H__
#define __BASEB_METER_COMMUNICATION_H__
#include "basedef.h"
#include "basetype.h"

enum
{
	METER_READ_ERROR_FILL_DATA = 1, // �������
	METER_READ_ERROR_NO = 0, // ͨѶ�ɹ�
	METER_READ_ERROR_NO_FRAME = -1, // ͨѶʧ��

	METER_READ_ERROR_OTHER = -99, // ��������
	METER_READ_ERROR_END = -100
};

/** @class
 *  @brief ���ͨѶ
 */
class C_MeterCom
{
public:
	C_MeterCom(void);
	virtual ~C_MeterCom();

public:
	char SetFillChar(char bFillChar);

	/** @fn
	 *  @brief ��ʼ���������õı�ƹ�Լ
	 *  @return 0:�ɹ�; -1:ʧ��
	 */
	virtual int InitMeterRules(const char *pResourcePath);

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
	 *  @return <0:����>=0:�ɹ���see[METER_READ_ERROR_]
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
	 *  @param[out] ReportDatas ����������
	 *  @param[out] idxSuccess �ɼ��ɹ�����������[ReportDatas������]
	 *  @param[in] SuccessfulMPBS �ɹ��Ĳ����㼯
	 *  @param[in] CommRegs ͨ�üĴ�����
	 *  @param[in] timeKeeping �ϱ�ʱ��;0:�˳��ϱ�;0xFFFFFFFF:��ѯ;����:�����ϱ�
	 *  @return <0:���� 0:�����ϱ� >0:�ϱ�ʣ��ʱ��
	 */
	virtual int Report(S_ReportDatas &ReportDatas, S_WORD &idxSuccess, const S_MPBS &SuccessfulMPBS, const S_REGISTERS &CommRegs, S_TIME timeKeeping);
	/** @brief �㲥���ʱ��
	 *  @param[in] Meter �����Ϣ
	 *  @param[in] timeExpected ����ʱ��
	 *  @return <0:����>=0:�ɹ�����ʱ������
	 */
	virtual int Broadcast(const S_MeterBase &Meter, S_TIME timeExpected);
	/** @brief ����բ������������
	 *  @param[in] Meter �����Ϣ
	 *  @param[in] strPassword ���� PAP1P2P3
	 *  @param[in] strOperator �����ߴ���
	 *  @param[in] strParam �������뻺��
	 *  @return <0:����>=0:�ɹ�
	 */
	virtual int Brake(const S_MeterBase &Meter, const string &strPassword, const string &strOperator, const string &strParam);
};


//
//�ɼ����󹤳�
//
class C_ComObjFact
{
public:
	/** @brief ��ȡ���ͨ����� 
	 *  @param[in] ObjType �Ĵ�����Դ[1-9] ȡ�Ĵ����ĸ�4λ
	 *  @see <kge common registor manual.doc>
	 *  @note
		3*******������Դ�ļĴ���
		2*******����ѹ����ģ����Դ�ļĴ���
		1*******���ն˼Ĵ�����������������GPRS�ź�
		����Ϊ��ƼĴ���
	 */
	static C_MeterCom* GetComObj(S_BYTE ObjType);

	/** @brief �����û����ͨ����� */	
	static int SetComObj(S_ComObj ObjType, C_MeterCom *pMeterCom);
};

#endif //__BASEB_METER_COMMUNICATION_H__


