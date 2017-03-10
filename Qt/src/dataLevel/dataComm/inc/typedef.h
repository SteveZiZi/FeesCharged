/** @file
 *  @brief �����������Ͷ���
 *  @author Steven Wan
 *  @date 2016-02/11
 *  @version 0.1
*/

#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_
#include <string.h>
#include <stdio.h>



#ifndef WIN32
	#undef __STL_USE_EXCEPTIONS
	#undef __EXCEPTIONS
	#ifdef __THROW_BAD_ALLOC
	#undef __THROW_BAD_ALLOC
	#endif
	#ifndef __THROW_BAD_ALLOC
		#define __THROW_BAD_ALLOC throw bad_alloc()
	#endif

	//#define ASSERT(exp)     ((void)0)
	#define ASSERT(exp) {if (!(exp)) printf("***********Assert failed.file=%s,line=%d,exp=%s\n",__FILE__,__LINE__,#exp);}
	#define ASSERT_TRIP(exp,str) {if (!(exp)) printf("***********Assert failed.file=%s,line=%d,exp=%s,exp=%s\n",__FILE__,__LINE__,#exp,#str);}
#else
	#include "assert.h"
	#define ASSERT assert
#endif

#define YEAR_21CENT_BEGIN 2000
#define TM_YEAR_BEGIN     1900
#define TM_MONTH_BEGIN    1

/* �������Ϣ */
#define CONS_CARD_SERIALNO_LEN	8//�����к�8�ֽ�

#define CONS_CARD_METERNO_LEN	8//���8�ֽ�

#define CONS_DEVICE_MAX_COUNT 64+1 //�����

/* �������Ͷ���*/
typedef unsigned char			INT8U;
typedef INT8U                   		UCHAR;
typedef unsigned short			INT16U;
typedef signed short			INT16S;
typedef unsigned int			INT32U;
//typedef signed int				INT32S;
typedef unsigned long long		INT64U;
typedef signed long long		INT64S;
typedef float					FP32;
typedef double				FP64;
typedef unsigned char 			byte;

typedef void				S_VOID;
typedef char				S_CHAR;
typedef short				S_SHORT;
typedef long				S_LONG;
typedef unsigned char		S_BYTE;
typedef unsigned short		S_WORD;
typedef unsigned int		S_DWORD;

#define S_BYTE	S_BYTE
#define S_WORD	S_WORD
#define INT32U	S_DWORD
#define INT8S	S_CHAR
#define INT16S	S_SHORT
#define INT32S	S_LONG

#define S_TIME	S_DWORD
#define S_BOOL	bool
#define S_NULL	((void *)0)

typedef enum
{
	emFail = 0,
	emSucess =1	
}T_emResult;

typedef enum
{
	INVALID_CARD=1,
	PRESET_CARD=2,
	USER_CARD = 3,
	MAX_CARD
}T_emCARDTYPE;

typedef enum
{
	CARD_OUT=0,
	CARD_IN=1,
}T_emCARDSTATE;

typedef enum
{
	BRAKE_OFF=0,
	BRAKE_ON=1,
}T_emBRAKESTATE;


/* �豸�ڴ�����*/
struct SIpcDeviceRegData
{
	unsigned char m_DeviceNo;//�豸���
	unsigned char m_Reg00010000[4];//�Ĵ������ݵ�ǰ�����й���
	unsigned char m_Reg05060000[7][4];//�������ն�����
	unsigned char m_Reg00010001[6][4];//�����¶�����
};

//��բ����
struct SBrakeOnCmd
{
	T_emBRAKESTATE m_Brakestate;//����բ״̬
	char m_ActionTime7Bcd[7];//����բ����ʱ��
};

struct SElecPricePack
{
	char m_Period1Val[6][4];//��һ����ֵ1~6���ݸ�ʽXXXXXX.XX
	char m_Period1Price[7][4];//��һ���ݵ��1~7���ݸ�ʽXXXX.XXXX
	char m_Period1Accountday[6][3];//��һ���ݵ�1 ~6������
};

//������Ϣ
struct SParamInfo
{
	char m_TariffRateSwitchTime5Bcd[5];//j���׷�ʱ�����л�ʱ��
	char m_AlarmMoney1[4];//�������1 XXXXXX.XX
	char m_AlarmMoney2[4];//�������2 XXXXXX.XX
	char m_CtRatio[3];//�������б��XXXXXX
	char m_PtRatio[3];//��ѹ���б��XXXXXX
	char m_MeterNo[6];//���BCD 
	char m_CustomerNo[6];//�ͻ����BCD
};
//�׵��
struct SElecPrice
{
	char m_TariffRate[12][4]; //����12���ݸ�ʽXXXX.XXXX
	SElecPricePack m_ElecPricePack[2];//��1��2��ǰ�׵�� 
};

/* �û�����Ϣ*/
struct SUserCard
{
	struct SParamInfo m_ParamInfo;
	struct SElecPrice m_CurrentElecPrice;//��ǰ���
	struct SElecPrice m_ElecPriceBackup; //���õ��
	struct SBrakeOnCmd m_BrakeOnCmd;//��բ��������
};

/* ����Ϣ*/
struct SIpcCardInfo
{
	T_emCARDTYPE m_CardType;//������--2Ԥ�ÿ�  3�û���
	T_emCARDSTATE m_State;//�忨״̬
	S_BYTE m_CardSerialNo[8];//�����
	S_BYTE m_CardMeterNo[8];//�����
	S_BYTE m_EsamRandData[8];//8�ֽ�ģ��ESAM�����
	S_BYTE m_DstData[8];//8�ֽ�����
	S_BYTE m_RechargeVal[4];//��ֵ����ʽXXXXXX.XX
	INT32U m_RechargeTimes;//��ֵ����
	S_BYTE m_MAC1[4];
	S_BYTE m_CustomerNo[6];//6�ֽ�bcd,�ͻ����
	S_BYTE m_MAC2[4];
	S_BYTE m_ParamInfo[256];//������Ϣ
	S_WORD m_ParamInfoLen;//������Ϣ����
	S_BYTE m_MAC3[4];
};

/*�ն���Ϣ*/
/** @brief �ն�RTUA */
struct SIpcRtua
{
	S_DWORD m_AreaCode; ///< ������,��ʽ:���ֽ�Ϊ0�������ֽ�Ϊʡ�ݡ������ֽڱ�ʾ�����룬���ֽڱ�ʾ������
	S_DWORD m_Address; ///< ��ַ��ѡַ��ΧΪ1~16777216��A2=000000HΪ��Ч��ַ��A2=FFFFFFHʱ��ʾϵͳ�㲥��ַ
};

/** @brief �汾��Ϣ */
struct SIpcVersion
{
	char m_HWVersion[32]; ///< Ӳ���汾��
	char m_SWVersion[32]; ///< ����汾��
	char m_ESAMKEYVersion[32]; ///< ��Կ�汾��
};

/** @brief �û����� */
struct SIpcUserParm
{
	S_WORD m_MeterNo;//��������
	S_BYTE m_MeterSerialNo[8];//�����
	S_BYTE m_MeterAddr[6];//���ַ
	S_BYTE m_CustomerNo[6];//6�ֽ�bcd,�ͻ����
};

/** @brief ͨѶ���� */
struct SIpcCommParam
{
	char m_Apn[36];//APN
	INT32U m_ServerIP; 
	INT32U m_ServerPort;
	INT32U m_LocalIP;
	S_BYTE m_HeartBeat;
	S_BYTE m_Singal;
	S_BYTE m_NetState;
};
struct SIpcTerminalInfo
{
public://�ն˵�ַ������
	SIpcRtua GetRTUA(void){return m_Rtua;};
	void SetRTUA(SIpcRtua rtua){m_Rtua = rtua;};

public://�ն˰汾��Ϣ
	SIpcVersion GetVersion(void){return m_Version;};
	void SetVersion(SIpcVersion version){m_Version = version;};

public://�û�����
	SIpcUserParm GetUserParm(int mp){return m_UserParm[mp];}
	void SetUserParm(int mp,SIpcUserParm userparam){m_UserParm[mp] = userparam;}

public://ͨѶ����
	SIpcCommParam GetCommParm(void){return m_CommParam;}
	void SetCommParm(SIpcCommParam commparam){m_CommParam = commparam;}	
	
private:
	struct SIpcRtua m_Rtua;//�ն˵�ַ������
	struct SIpcVersion m_Version;//�ն˰汾��Ϣ
	struct SIpcUserParm m_UserParm[CONS_DEVICE_MAX_COUNT];//�û�����
	struct SIpcCommParam m_CommParam;//ͨѶ����
};

#endif
