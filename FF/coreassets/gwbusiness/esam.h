/** @file
 *  @brief 国网ESAM卡安全模块
 *  @author 
 *  @date 2012/08/16
 *  @version 0.1
 */
#ifndef __GWB_ESAM_H__
#define __GWB_ESAM_H__
#include "esambase.h"

///ESAM卡安全模块 错误状态字描述
enum S_EsamStatus
{
	REQ_OK_REPLEN = 0x6100,///正确执行
	REQ_CMD_OK = 0x9000,///指令正确执行
	REQ_LEN_ERROR = 0x6700,///数据长度错误
	REQ_NO_DATA_IN_ESAM_ERROR = 0x6F00, ///卡中无数据返回
	REQ_NOT_SURPPORT_ERROR = 0x6A81,// 不支持此功能（无MF或卡片已锁定）
	REQ_LST_RANDOMDATA_ERROR = 0x6984,///未取随机数
	REQ_W_PKFILE_ERROR = 0x9071,///写公钥文件错误
	REQ_FRAME_ERROR = 0x9072,///指令结构错误
	REQ_SM1_PWD_ERROR = 0x9073,///SM1密钥错误
	REQ_VERIRY_SIGN_NOTMATCH_ERROR = 0x9074,///验签文件类型不匹配
	REQ_VERIRY_SIGN_NOTFOUND_ERROR = 0x9075,///验签文件未找到
	REQ_VERIFY_RSASIGN_ERROR = 0x9086,///RSA验签错误
	REQ_SM1_DECIPHER_ERROR = 0x908C,///SM1解密数据错误
	REQ_NONE = 0x0000,
	REQ_DEBUG_RET=0xFFFF,
};

/** @class
 *  @brief 缺省安全校验对象
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
 *  @brief CRC密码校验对象
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
 *  @brief 安全模块基本接口命令实现
 *  @note
 *  集中器与主站进行通讯时，若需要安全功能，应对本接口进行二次封装
 */
class C_ESAM:public C_EsamBase
{
public:
	static C_ESAM* GetInstance(void);

public:
	virtual bool InitEsamInfo(C_MSRULE *pMsRule, C_CONNECT *pConnect);
	virtual int Authenticate(const string &strCmd, const string &strPWD);
	virtual int GetErrorInfo(string &strErrorInfo);

	/*主控公钥更新指令*/
	int UpdateMCtrlPK(const string& strCtrlPK ,const string& strSignedData);
	
	/*主站公钥本地更新指令*/
	int UpdateMasterPK(const string& strMasterPK ,const string& strSignedData);
	
	/*主站公钥远程更新指令*/
	int RemoteUpdateMasterPK(const string& strSessionKey,const string& strMasterPk,const string& strSignedInfo);
	
	/*终端非对称密钥注册*/
	int RegisterDeviceAsymmetricKey(const string& strMasterRandomData,const string& strSignedData, string& strDevicePk, INT8U Tag = 1);
	 
	/*终端非对称密钥更新*/
	int UpdateDeviceAsymmetricKey(const string& strSessionKey,const string& strMasterRandomData, const string& strMasterSignedData, string &strDevicePk, string &strSignedData, INT8U Tag = 1);

	/*终端对称密钥更新*/
	int UpdateDeviceSymmetryKey(const string& strSessionKey, const string& strSymmetryKey, const string& strSignedData);

	/*公钥验证指令*/
	int VerifyPK(const string& strRandomData,const string& strSignedData, INT8U Tag);

	/*获取Esam随机数*/
	int GetRandomData(string& strRandomData, bool fNew=true, INT8U expectedLen=8);
	 
	/*获取Esam序列号*/
	int GetSerialNo(string &strSNData, bool fReset = false, INT8U expectedLen = 8);

private:
	C_ESAM(void);

	/*根据明文计算MAC值*/
	int GetMAC(const string &strMsg, S_BYTE AddrSN, S_BYTE Group, string &strMAC);
	/*计算MAC的P1P2值*/
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

