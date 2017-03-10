/** @file
 *  @brief ����ESAM����ȫģ��
 *  @author 
 *  @date 2012/08/16
 *  @version 0.1
 */
#ifndef __GWB_ESAM_H__
#define __GWB_ESAM_H__
#include "esambase.h"

///ESAM����ȫģ�� ����״̬������
enum S_EsamStatus
{
	REQ_OK_REPLEN = 0x6100,///��ȷִ��
	REQ_CMD_OK = 0x9000,///ָ����ȷִ��
	REQ_LEN_ERROR = 0x6700,///���ݳ��ȴ���
	REQ_NO_DATA_IN_ESAM_ERROR = 0x6F00, ///���������ݷ���
	REQ_NOT_SURPPORT_ERROR = 0x6A81,// ��֧�ִ˹��ܣ���MF��Ƭ��������
	REQ_LST_RANDOMDATA_ERROR = 0x6984,///δȡ�����
	REQ_W_PKFILE_ERROR = 0x9071,///д��Կ�ļ�����
	REQ_FRAME_ERROR = 0x9072,///ָ��ṹ����
	REQ_SM1_PWD_ERROR = 0x9073,///SM1��Կ����
	REQ_VERIRY_SIGN_NOTMATCH_ERROR = 0x9074,///��ǩ�ļ����Ͳ�ƥ��
	REQ_VERIRY_SIGN_NOTFOUND_ERROR = 0x9075,///��ǩ�ļ�δ�ҵ�
	REQ_VERIFY_RSASIGN_ERROR = 0x9086,///RSA��ǩ����
	REQ_SM1_DECIPHER_ERROR = 0x908C,///SM1�������ݴ���
	REQ_NONE = 0x0000,
	REQ_DEBUG_RET=0xFFFF,
};

/** @class
 *  @brief ȱʡ��ȫУ�����
 */
class C_EsamDefault:public C_EsamBase
{
public:
	static C_EsamDefault* GetInstance(void);

public:
	virtual bool InitEsamInfo(C_MSRULE *pMsRule, C_CONNECT *pConnect);
	virtual int Authenticate(const string &strCmd, const string &strPWD);
	virtual int GetErrorInfo(string &strErrorInfo);

private:
	C_EsamDefault(void);
};

/** @class
 *  @brief CRC����У�����
 */
class C_EsamCrc:public C_EsamBase
{
public:
	static C_EsamCrc* GetInstance(void);

public:
	bool SetSecurityParam(S_WORD CrcKey);
	virtual bool InitEsamInfo(C_MSRULE *pMsRule, C_CONNECT *pConnect);
	virtual int Authenticate(const string &strCmd, const string &strPWD);
	virtual int GetErrorInfo(string &strErrorInfo);

private:
	C_EsamCrc(void);
	S_WORD CrcPassword(S_WORD wSrcKey, const string &strData);

private:
	S_WORD m_CrcKey;
};

/** @class
 *  @brief ��ȫģ������ӿ�����ʵ��
 *  @note
 *  ����������վ����ͨѶʱ������Ҫ��ȫ���ܣ�Ӧ�Ա��ӿڽ��ж��η�װ
 */
class C_ESAM:public C_EsamBase
{
public:
	static C_ESAM* GetInstance(void);

public:
	virtual bool InitEsamInfo(C_MSRULE *pMsRule, C_CONNECT *pConnect);
	virtual int Authenticate(const string &strCmd, const string &strPWD);
	virtual int GetErrorInfo(string &strErrorInfo);

	/*���ع�Կ����ָ��*/
	int UpdateMCtrlPK(const string& strCtrlPK ,const string& strSignedData);
	
	/*��վ��Կ���ظ���ָ��*/
	int UpdateMasterPK(const string& strMasterPK ,const string& strSignedData);
	
	/*��վ��ԿԶ�̸���ָ��*/
	int RemoteUpdateMasterPK(const string& strSessionKey,const string& strMasterPk,const string& strSignedInfo);
	
	/*�ն˷ǶԳ���Կע��*/
	int RegisterDeviceAsymmetricKey(const string& strMasterRandomData,const string& strSignedData, string& strDevicePk, INT8U Tag = 1);
	 
	/*�ն˷ǶԳ���Կ����*/
	int UpdateDeviceAsymmetricKey(const string& strSessionKey,const string& strMasterRandomData, const string& strMasterSignedData, string &strDevicePk, string &strSignedData, INT8U Tag = 1);

	/*�ն˶Գ���Կ����*/
	int UpdateDeviceSymmetryKey(const string& strSessionKey, const string& strSymmetryKey, const string& strSignedData);

	/*��Կ��ָ֤��*/
	int VerifyPK(const string& strRandomData,const string& strSignedData, INT8U Tag);

	/*��ȡEsam�����*/
	int GetRandomData(string& strRandomData, bool fNew=true, INT8U expectedLen=8);
	 
	/*��ȡEsam���к�*/
	int GetSerialNo(string &strSNData, bool fReset = false, INT8U expectedLen = 8);

private:
	C_ESAM(void);

	/*�������ļ���MACֵ*/
	int GetMAC(const string &strMsg, S_BYTE AddrSN, S_BYTE Group, string &strMAC);
	/*����MAC��P1P2ֵ*/
	int GetP1P2(S_BYTE AFN, S_BYTE AddrSN, S_BYTE Group, S_BYTE &P1, S_BYTE &P2);

	int SendCmd(const string &strCmd);
	int RecvCmd(string &strData);
	int ReqResponsedData(string &strData, int expectedLen = 8);
	S_EsamStatus TranRetStat(const string& strRetCmd);

private:
	C_MSRULE *m_pMsRule;
	C_CONNECT *m_pSecurityConnect;
	string m_strSerialNo;
	string m_strRandom;
};

#endif //__GWB_ESAM_H__

