/**
 *  @file
 *  @brief MODEM控制类
 *  @author 
 *  @date 2012-02-06
 *  @version v1.0
 *  @note
 */
#ifndef __BASEB_MODEM_H__
#define __BASEB_MODEM_H__
#include "basedef.h"
#include "connect.h"
#include "baseio.h"

enum MODEM_TYPE
{
	MODEM_TYPE_COMMON,
	MODEM_TYPE_GPRS_COMMON,

	MODEM_TYPE_GPRS_MAX,

	MODEM_TYPE_CDMA_COMMON,

	MODEM_TYPE_CDMA_MAX,

	MODEM_TYPE_PSTN_COMMON,

	MODEM_TYPE_PSTN_MAX,

	MODEM_TYPE_MAX
};


class C_MODEM
{
public:
	C_MODEM(MODEM_TYPE type, const char* name);
	virtual ~C_MODEM(){}

	MODEM_TYPE GetType() const{return m_Type;}
	const char* GetName() const{return m_Name.c_str();}
	/** @brief 如果MODEM驱动不用缺省，则要设置: IO_MODEM_DRIVER_ */
	int SetModemDriver(int ModemDriver){return io_SetModemDriverObj(ModemDriver);}
	void SetModemConnection(C_CONNECT *pModemConnect){m_pModemConnect = pModemConnect;}
	C_CONNECT *GetModemConnection(){return m_pModemConnect;}

	virtual int PowerOn(int PowerOnDelayS);
	virtual int PowerOff(int PowerOffDelayS);
	//检查CD状态，返回-1表示未知，返回0表示有CD，返回1表示无CD
	virtual int CheckCD(void);
	/** @brief AT COMMAND */
	virtual int GetIprCommand(string& strCmd, int baud);//用于设置Modem波特率
	//初始化命令可能需要分步进行，step表示步骤，从0开始，返回0表示没有下一步骤，返回1表示还有下一步骤，返回-1表示不支持
	virtual int GetInitCommand(string& strCmd,int step);
	virtual int GetTestCommand(string& strCmd);
	virtual int GetSignalCommand(string& strCmd);
	virtual int GetAutoConnectCommand(string& strCmd);
	virtual int GetFactoryCommand(string& strCmd);
	virtual int GetModelCommand(string& strCmd);
	virtual int GetSimCimiCommand(string& strCmd);
	virtual int GetRegisterCommand(string& strCmd);
	virtual int GetRegisterCommand(string& strCmd, int urc);
	virtual int GetGmrCommand(string& strCmd);
	virtual int GetFunCommand(string& strCmd, int fun, int rst);

	virtual int ParseSignal(const string& strAnswer);
	virtual bool HasRing(const string& strAnswer);
	virtual bool ParseCarrierLost(const string& strAnswer);
	virtual int ParseConnectBaud(const string& strAnswer);
	virtual int ParseRegisterCmd(const string &strAnswer, int &mode, int &stat);
	virtual int ParseRegisterCmd(const string &strAnswer, int &mode, int &stat, string &strLac, string &strCi, int &act);
	virtual int ParseGmrCmd(const string &strAnswer, string &strGmrInfo);

public:
	virtual int TestModem(int times = 3);
	virtual int InitModem(const char* initCmd = NULL);
	virtual int SoftReset(int PowerOnDelayS);
	virtual int GetSignal(void);
	virtual int SyncModemBaud(int baud = 0);
	virtual int GetModemInfo(string &strModemFact, string &strModemModel);
	virtual int GetSimInfo(string &strSimInfo);
	virtual int SetRegisterParam(int urc);
	virtual int GetRegisterInfo(int &mode, int &stat);
	virtual int GetRegisterInfo(int &mode, int &stat, string &strLac, string &strCi, int &act);
	virtual int GetGmrInfo(string &strGmrInfo);

	virtual int Switch2Cmd(void); // 切换命令模式
	virtual int Hangup(void); // 挂机
	virtual int handon(void); // 摘机
	virtual int AnswerCall(void); // 摘机应答
	virtual int CheckRing(void); // 检测振铃
	virtual int CheckConnection(void); // 检测振铃|载波|连接波特率

	virtual bool GetSignalExt(int &Signal);

protected:
	int ModemStdComm(const string& strCmd, string& strAnswer);
	int ModemSendRecv(const string& strCmd, string& strAnswer);
	int ExtractModemInfo(const string &strAtCmd, const string &strAnswer, string &strOut);
	int ExtractModemInfo(const string &strAnswer, string &strOut);

protected:
	MODEM_TYPE m_Type;
	string m_Name;
	C_CONNECT *m_pModemConnect;
};

class C_GprsModem:public C_MODEM
{
public:
	C_GprsModem(MODEM_TYPE type, const char* name):C_MODEM(type,name){}

	//初始化命令可能需要分步进行，step表示步骤，从0开始，返回0表示没有下一步骤，返回1表示还有下一步骤，返回-1表示不支持
	virtual int GetInitCommand(string& strCmd,int step);
};

class C_PstnModem:public C_MODEM
{
public:
	C_PstnModem(MODEM_TYPE type, const char* name):C_MODEM(type,name){}

	virtual int PowerOn(int PowerOnDelayS);
	virtual int PowerOff(int PowerOffDelayS);
	//初始化命令可能需要分步进行，step表示步骤，从0开始，返回0表示没有下一步骤，返回1表示还有下一步骤，返回-1表示不支持
	virtual int GetInitCommand(string& strCmd,int step);
};

class C_ModemFactory
{
public:
	static C_MODEM* CreateModem(INT8U modemType);
	static void DestroyModem(C_MODEM* pModem);
};

#endif//__BASEB_MODEM_H__

