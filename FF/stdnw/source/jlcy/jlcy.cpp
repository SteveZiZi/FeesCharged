/** @file
 *  @brief ����ģ��ɼ����ܶ���
 *  @author ������
 *  @date 2012/03/01
 *  @version 0.1
 */
#include "jlcy.h"
int C_JlcyServer::BeforeService(void)
{
	return 0;
}

int C_JlcyServer::AfterService(void)
{
	return 0;
}

int C_JlcyServer::OnTickService(void)
{
	HeartBeat();
	SleepMS(50);
	return 0;
}



