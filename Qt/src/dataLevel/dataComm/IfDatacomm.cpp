#include "ifDatacomm.h"

bool CIfDatacomm::getCurZxygData(unsigned int mp,char *pData)
{
	memcpy(pData,IPCSHAREMEM.m_IpcDevicesRegData[mp].m_Reg00010000,sizeof(IPCSHAREMEM.m_IpcDevicesRegData[mp].m_Reg00010000));
#if 0	
	const char *pPrecess = "dbcj1";
	string strIpcData;
	
	strIpcData.clear();
	strIpcData.push_back(0x01);
	strIpcData.push_back(0x01);
	strIpcData.push_back(0x00);
	strIpcData.push_back(0x00);
	strIpcData.push_back(0x01);
	strIpcData.push_back(0x00);	

	if (C_SysMsg::Send2Server(pPrecess, MSAFN_READ_REALTIME, strIpcData) != 0)
	{
		//logwt<<"发送"<<pPrecess<<"实招数据失败!!!"<<endl;
		printf("send %s real call fail\n",pPrecess);
		return false;
	}

	strIpcData.clear();
	if (C_SysMsg::Wait4Server(MSAFN_READ_REALTIME, strIpcData) != 0)
	{
		//logwt<<"接收"<<pPrecess<<"实招数据失败!!!"<<endl;
		printf("receive %s real call fail\n",pPrecess);
		memset(pData,0xff,4);
		return false;
	}
	//logvt<<"strIpcData="<<hex<<strIpcData<<endl;

	if (strIpcData.size() <= 6)
	{
		//logwt<<"接收"<<pPrecess<<"实招数据小于六字节!!!"<<endl;
		printf("receive %s relay less 6 bytes\n",pPrecess);
		strIpcData.clear();
		memset(pData,0xff,4);
		return false;
	}
	printf("relay %x%x%x%x\n",strIpcData[6],strIpcData[7],strIpcData[8],strIpcData[9]);
	pData[0] = strIpcData[6];
	pData[1] = strIpcData[7];
	pData[2] = strIpcData[8];
	pData[3] = strIpcData[9];		
#endif		
	return true;

}
/*
	brief:获取测量点最近七天日冻结数
	param [in] unsigned int mp:测量点序号
	               char pData[7][4]:最近七天日冻结数
	returnt bool
	author:Steven Wan
	date: 2015-09-02
*/
bool CIfDatacomm::geInSevenDaysData(unsigned int mp,char dataarry[7][4])
{
	memcpy(dataarry,IPCSHAREMEM.m_IpcDevicesRegData[mp].m_Reg05060000,sizeof(IPCSHAREMEM.m_IpcDevicesRegData[mp].m_Reg05060000));
	return true;
}
/*
	brief:获取测量点最近六月冻结数
	param [in] unsigned int mp:测量点序号
	               char pData[7][4]:最近六月冻结数
	returnt bool
	author:Steven Wan
	date: 2015-09-02
*/

bool CIfDatacomm::getInSixMonthsData(unsigned int mp,char dataarry[6][4])
{
	memcpy(dataarry,IPCSHAREMEM.m_IpcDevicesRegData[mp].m_Reg00010001,sizeof(IPCSHAREMEM.m_IpcDevicesRegData[mp].m_Reg00010001));
	return true;
}
T_emCARDSTATE CIfDatacomm::getCardState()
{
	return IPCSHAREMEM.GetCardState();
}

T_emCARDTYPE CIfDatacomm::getCardType()
{
	return IPCSHAREMEM.GetCardType();
}

SIpcTerminalInfo CIfDatacomm::GetTerminalInfo()
{
	return IPCSHAREMEM.m_IpcTerminalInfo;
}