/** @file
 *  @brief 后台通讯命令基础服务定义
 *  @author 
 *  @date 2011/12/22
 *  @version 0.1
 */
#include "cmdbase.h"

bool C_CR_E0000000::ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj)
{
	if (distance(iterBegin, iterEnd) < 1)
		return false;
	CmdObj.m_DiInfo.push_back(*iterBegin); advance(iterBegin, 1);
	return true;
}
bool C_CR_E0000000::Action(const S_CmdObj &CmdObj, strings &strResponse)
{
	if (CmdObj.m_DiInfo.size() != 1)
		return false;

	if (CmdObj.m_DiInfo.at(0) == 0x00)//0x00 confirm
		return true;
//	else if (CmdObj.m_DiInfo.at(0) == 0x01)//0x01 deny;
//		return false;

	return false;
}

int C_AFNCMD_CONFIRM::Execute(const S_CmdObjs &CmdObjs, strings &strResponseInfos)
{
	if (CmdObjs.empty())
		return FRAME_PROCESS_STOP;
	C_CmdReceiver *pCmdReveiver = GetCmdReceiver(CmdObjs[0].m_DI);
	if (pCmdReveiver == NULL)
		return FRAME_PROCESS_STOP;
	strings strResponse;
	if (pCmdReveiver->Action(CmdObjs[0], strResponse) == false)
		return FRAME_PROCESS_DENY;
	return FRAME_PROCESS_CONFIRM;
}

C_CmdReceiver *C_AFNCMD_CONFIRM::GetCmdReceiver(INT32U DI)
{
	static C_CR_E0000000 sCr0xE0000000;
	if (DI == 0xE0000000)
		return &sCr0xE0000000;
	return NULL;
}



