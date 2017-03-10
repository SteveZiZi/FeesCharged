/** @file
 *  @brief �ն˼�����Ӧ�ýӿ�
 *  @author 
 *  @date 2012/11/07
 *  @version 0.1
 */
#ifndef __LCD_FACTORY_API_H__
#define __LCD_FACTORY_API_H__
#include "termapi.h"

class C_GwTermApi:public C_TermApi
{
public:
	virtual int EnableProcessHeartBeat(bool fEnable);

	virtual C_CONNECT *CreateConnect(int ChannelType, const S_PORTSET &PortSet);
	virtual void DestroyConnect(C_CONNECT *pConnect);

	virtual bool EnableWatchDog(bool fEnable);
	virtual bool SoftwareReset(void);
	virtual bool EnableProcess(const char *pProcess, bool fEnable);

	/** @param[in] BatteryStandTimeM  ��ع���ʱ�䳤�ȣ������е硣Ĭ��Ϊ3����
	  * @param[in] TermStandTimeS  �����޵������£��ն��е�ʱ��
	  */
	virtual int InitPowerCutInfo(S_BYTE BatteryStandTimeM, S_BYTE TermStandTimeS);
	virtual int GetPowerCutTimes(void);
};


#endif //__LCD_FACTORY_API_H__
