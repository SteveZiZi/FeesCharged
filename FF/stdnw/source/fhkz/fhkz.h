/** @file
 *  @brief 交流采集进程数据采集类
 *  @author 万青松
 *  @date 2012/03/01
 *  @version 0.1
 */
#ifndef __FHKZ_SERVER_H__
#define __FHKZ_SERVER_H__

#include "server.h"

class C_FhkzServer:public C_SERVER
{
public:
	virtual int BeforeService(void);
	virtual int AfterService(void);
	virtual int OnTickService(void);
	
private:
	/** @brief 检查2秒任务 */
	int CheckTask2S(void);
	/** @brief 检查5秒任务 */
	int CheckTask5S(void);
	/** @brief 检查10秒任务 */
	int CheckTask10S(void);
	/** @brief 检查30秒任务 */
	int CheckTask30S(void);

private:
	void HistoryDayDataFill();
	bool FillDayData(S_WORD MP, S_DWORD FN, S_TIME DataTime, char *pBuffer);
	void HistoryMonthDataFill();
	bool FillMonthData(S_WORD MP, S_DWORD FN, S_TIME DataTime, char *pBuffer);

private:
	int AlmEvtHandle(void);
	int VirtualAlmTaskFinish(void);
};

#endif//__FHKZ_SERVER_H__

