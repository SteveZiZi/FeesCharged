/**
 *  @file
 *  @brief ��ƹ�Լ������
 *  @author 
 *  @date 2012-05-10
 *  @version v1.0
 *  @note
 */
#ifndef __BASEB_MRULE_H__
#define __BASEB_MRULE_H__
#include "basedef.h"
#include "basetype.h"

enum
{
	MRULE_PARSE_ERR_NO = 0, //�ɹ�
	MRULE_PARSE_ERR_ADDR = -1, //��ַ��
	MRULE_PARSE_ERR_DI = -2, //�������
	MRULE_PARSE_ERR_CTRL = -3, //�������
	MRULE_PARSE_ERR_REG = -4, //�Ĵ�����
	MRULE_PARSE_ERR_UNSUPPORT = -5, //��Լ��֧��
	MRULE_PARSE_ERR_RETURN_CODE = -6, // ���ش�����
	MRULE_PARSE_ERR_CHECKSUM = -7, // У�������
	MRULE_PARSE_ERR_CONTENT = -8, // �������ݳ���
	MRULE_PARSE_ERR_NO_DATA = -9, // ����������
	MRULE_PARSE_ERR_NO_FRAME = -10, // û���յ�����
	MRULE_PARSE_ERR_BUFFER_INVALID = -11, // ���ݻ�����Ч

	MRULE_PARSE_ERR_OTHER = -100 //������
};

/** @brief ��ƼĴ�����ͨ�üĴ���ӳ�� */
struct S_RegisterMap
{
	S_DWORD m_MainID;///< ���Ĵ���ID
	S_DWORD m_MapID; ///< ӳ��Ĵ���ID
	string m_MapName;///< ӳ��Ĵ����ַ�������

public:
	S_RegisterMap(void)
	{
		m_MainID = 0x0;
		m_MapID = 0x0;
	}
};
struct S_RegisterMaps:public vector<S_RegisterMap>
{
};
bool operator<(const S_RegisterMap &left, const S_RegisterMap &right);

/** @brief ��ƼĴ�����ͨ�üĴ���ӳ�� (һ�Զ�)*/
struct S_RegisterMap3
{
	S_DWORD m_MainID;///< ���Ĵ���ID
	vector<S_DWORD> m_MapID; ///< ӳ��Ĵ���ID

public:
	S_RegisterMap3(void)
	{
		m_MainID = 0x0;
	}
};
struct S_RegisterMaps3:public vector<S_RegisterMap3>
{
};
bool operator<(const S_RegisterMap3 &left, const S_RegisterMap3 &right);


/** @brief ��ƹ�Լ����ӿڶ��� */
class C_MRULE
{
public:
	C_MRULE(void);
	virtual ~C_MRULE();

public:
	S_COMSET GetComSet(void);
	/** @brief ��ȡ��ƹ�Լ���� */
	virtual char *GetName(void);
	/** @brief �����Լ����*/
	virtual bool ClearBuffer(void);
	/** @brief ���ұ��ؼĴ��� */
	virtual bool FindLocalRegister(const S_REGISTER &CommReg, S_REGISTER &LocalReg);

	/** @brief ��ȡ����֡
	 *  @param[in] strBuffer �����
	 *  @param[out] strCmd ֡�������
	 *  @return <0: ����[����ֵ�ǳ�����ֽ���]; =0: ��֡; >0: �ɹ�
	 */
	virtual int ExtractCmd(const string &strBuffer, string &strCmd);

	/** @brief �������֡
	 *  @param[in] strAddress ��Ƶ�ַ
	 *  @param[in] CommReg ͨ�üĴ���
	 *  @param[out] DataGram ֡�������
	 *  @param[in] fRsvReg true���üĴ���;false���Ĵ���
	 *  @return <0: ������޴�Ҫ��֡; =0: ������֡��ֱ�ӷ�������; >0: ��֡�ɹ�
	 */
	virtual int MakeReadCmd(const string &strAddress, const S_REGISTER &CommReg, S_DATAGRAM &DataGram, bool fRsvReg);
	/** @brief �����������֡
	 *  @param[in] strCmd ��������֡,��Ϊ�����������
	 *  @param[in] CommReg ͨ�üĴ���
	 *  @param[out] strData �����������
	 *  @return <0:����[�μ�MRULE_PARSE_ERR_];=0:�ɹ�
	 */
	virtual int ParseReadCmd(const string &strCmd, const S_REGISTER &CommReg, string &strData);

	/** @brief ��д����֡
	 *  @param[in] strAddress ��Ƶ�ַ
	 *  @param[in] CommReg ͨ�üĴ���
	 *  @param[in] strPassword ���� PAP1P2P3
	 *  @param[in] strOperator �����ߴ���
	 *  @param[in] strData �������뻺��
	 *  @param[out] DataGram ֡�������
	 *  @return <=0: ������޴�Ҫ��֡; >0: ��֡�ɹ�
	 */
	virtual int MakeWriteCmd(const string &strAddress, const S_REGISTER &CommReg, const string &strPassword, const string &strOperator, const string &strData, S_DATAGRAM &DataGram);
	/** @brief ����д�����֡
	 *  @param[in] strCmd ��������֡
	 *  @param[in] strAddress ��Ƶ�ַ
	 *  @param[in] CommReg ͨ�üĴ���
	 *  @param[out] strData �����������
	 *  @return <0:����[�μ�MRULE_PARSE_ERR_];=0:�ɹ�
	 */
	virtual int ParseWriteCmd(const string &strCmd, const string &strAddress, const S_REGISTER &CommReg, string &strData);

	/** @brief �鰲ȫ��֤����֡
	 *  @param[in] strAddress ��Ƶ�ַ
	 *  @param[in] CommReg ͨ�üĴ���
	 *  @param[in] strOperator �����ߴ���
	 *  @param[in] strData �������뻺��
	 *  @param[out] DataGram ֡�������
	 *  @return <=0: ������޴�Ҫ��֡; >0: ��֡�ɹ�
	 */
	virtual int MakeSecurityCmd(const string &strAddress, const S_REGISTER &CommReg, const string &strOperator, const string &strData, S_DATAGRAM &DataGram);
	/** @brief ������ȫ��֤�����֡
	 *  @param[in] strCmd ��������֡
	 *  @param[in] strAddress ��Ƶ�ַ
	 *  @param[in] CommReg ͨ�üĴ���
	 *  @param[out] strData �����������
	 *  @return <0:����[�μ�MRULE_PARSE_ERR_];>=0:�ɹ�
	 */
	virtual int ParseSecurityCmd(const string &strCmd, const string &strAddress, const S_REGISTER &CommReg, string &strData);

	/** @brief ��㲥Уʱ֡
	 *  @param[in] strAddress ��Ƶ�ַ
	 *  @param[in] CommReg ͨ�üĴ���
	 *  @param[in] timeExpected ����ʱ��
	 *  @param[out] DataGram ֡�������
	 *  @return <0: ������޴�Ҫ��֡; =0: ������֡��ֱ�ӷ�������; >0: ��֡�ɹ�
	 */
	virtual int MakeBroadcastCmd(const string &strAddress, const S_REGISTER &CommReg, S_TIME timeExpected, S_DATAGRAM &DataGram);

	/** @brief �������[�����ݴ�����Ϣ�鱨��]
	 *  @param[in] strAddress ��Ƶ�ַ
	 *  @param[in] DI ���ؼĴ���
	 *  @param[out] DataGram ֡�������
	 *  @return ֡����
	 */
	virtual int MakeReadFrame(const string &strAddress, S_DWORD DI, S_DATAGRAM &DataGram);

	/** @brief ����������[����������ı���]
	 *  @param[in] strCmd ����
	 *  @param[in] strAddress ��Ƶ�ַ
	 *  @param[in] DI ͨ�üĴ���
	 *  @param[out] strData �����������
	 *  @return ���ݳ���
	 */
	virtual int ParseReadFrame(const string &strCmd, string &strAddress, S_DWORD &DI, string &strData);

	/** @brief ������բ������������֡
	 *  @param[in] strAddress ��Ƶ�ַ
	 *  @param[in] strPassword ���� PAP1P2P3
	 *  @param[in] strOperator �����ߴ���
	 *  @param[in] strData �������뻺��
	 *  @param[out] DataGram ֡�������
	 *  @return <=0: ������޴�Ҫ��֡; >0: ��֡�ɹ�
	 */
	virtual int MakeBrakeCmd(const string &strAddress, const string &strPassword, const string &strOperator, const string &strData, S_DATAGRAM &DataGram);
	/** @brief ��������բ�����������������֡
	 *  @param[in] strCmd ��������֡
	 *  @param[in] strAddress ��Ƶ�ַ
	 *  @param[out] strData �����������
	 *  @return <0:����[�μ�MRULE_PARSE_ERR_];=0:�ɹ�
	 */
	virtual int ParseBrakeCmd(const string &strCmd, const string &strAddress, string &strData);

protected:
	S_COMSET m_ComSet;
};

#endif //__BASEB_MRULE_H__

