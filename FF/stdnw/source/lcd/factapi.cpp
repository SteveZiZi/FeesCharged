/** @file
 *  @brief �ն˼�����Ӧ�ýӿ�
 *  @author 
 *  @date 2012/11/07
 *  @version 0.1
 */
#include "factapi.h"
#include "connfact.h"
#include "sysctrl.h"
#include "context.h"
#include "cfg_path.h"
#include "cfg_usrp.h"
#include "status.h"

int C_GwTermApi::EnableProcessHeartBeat(bool fEnable)
{
	S_SERVINFO *pServInfo = CONTEXT.GetServerInfo(C_SERVER::GetCurrentServID());
	if (fEnable == false)
		pServInfo->m_HBInterS = 0;
	else
		pServInfo->m_HBInterS = SERVER_HEART_BEAT_INTERS;
	return 0;
}

C_CONNECT *C_GwTermApi::CreateConnect(int ChannelType, const S_PORTSET &PortSet)
{
	S_CHANNEL channel;
	channel.m_Enable = true; ///< ͨ��ʹ��
	sprintf((char *)channel.m_Name, "VCom%d", PortSet.m_ComSet.m_PortNO); ///< ͨ������
	channel.m_Type = ChannelType; ///<ͨ�����ͣ��μ�CHANNEL_TYPE_
	channel.m_ConnectTimeoutS = 10; ///<���ӳ�ʱ����λ��
	channel.m_ConnectInterS = 0; ///<���Ӽ������λ��
	channel.m_IdleTimeoutS = 0; ///<���г�ʱ����λ��
	channel.m_ReadTimeoutMS = 2000; ///<����ʱ����λ����
	channel.m_WriteTimeoutMS = 1000; ///<д��ʱ����λ����
	channel.m_SubReadTimeoutMS = 100; ///<�Ͳ����ʱ����λ����
	channel.m_CharReadTimeoutMS = 100; ///<�ַ�����ʱ����λ����
	channel.m_PackageLenMax = 400; ///<���ĳ���
	channel.m_PortSet = PortSet; ///<�˿�����

	return C_ConnFact::CreateConnection(channel);
}

void C_GwTermApi::DestroyConnect(C_CONNECT *pConnect)
{
	C_ConnFact::DestroyConnection(pConnect);
}

bool C_GwTermApi::EnableWatchDog(bool fEnable)
{
	C_SYSCTRL::HardwareReset();
	return true;
}

bool C_GwTermApi::SoftwareReset(void)
{
	C_SYSCTRL::SoftwareReset(true);
	return true;
}

bool C_GwTermApi::EnableProcess(const char *pProcess, bool fEnable)
{
	char fname[CFG_PATH_FILE_MAX_LEN];
	if (strcasecmp(pProcess, CFG_FILE_PROGRAM_PPPD) == 0)
		C_SYSCTRL::ExecScript(C_PATH::GetScriptPppClear(fname));
	else
		C_SYSCTRL::EnableProcess(pProcess, fEnable);
	return true;
}

/** @param[in] BatteryStandTimeM  ��ع���ʱ�䳤�ȣ������е硣Ĭ��Ϊ3����
  * @param[in] TermStandTimeS  �����޵������£��ն��е�ʱ��
  */
int C_GwTermApi::InitPowerCutInfo(S_BYTE BatteryStandTimeM, S_BYTE TermStandTimeS)
{
	S_SYSRUN *pSysRun = STATUS.GetSysRunInfo();
	S_SYSCFG *pSysCfg = STATUS.GetSystemConfig();

	pSysRun->m_cntPowerCut = 0;
	pSysCfg->m_BatteryStandTimeM = BatteryStandTimeM;
	pSysCfg->m_TermStandTimeS = TermStandTimeS;
	return 0;
}

int C_GwTermApi::GetPowerCutTimes(void)
{
	S_SYSRUN *pSysRun = STATUS.GetSysRunInfo();
	return pSysRun->m_cntPowerCut;
}


