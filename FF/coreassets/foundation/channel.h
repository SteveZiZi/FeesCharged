/** @file
 *  @brief ͨ���ඨ��
 *  @author 
 *  @date 2011/11/28
 *  @version 0.1
 */
#ifndef __FND_CHANNEL_H__
#define __FND_CHANNEL_H__

#include "basedef.h"

/** @brief ������������ */
enum
{
	NET_CONNECT_TCP,
	NET_CONNECT_UDP,
	NET_CONNECT_TCP_SERVER,
	NET_CONNECT_TCP_CLIENT,
	NET_CONNECT_UDP_SERVER,
	NET_CONNECT_UDP_CLIENT,

	NET_CONNECT_TYPE_MAX
};

/** @brief PPP���� */
enum
{
	NET_CONNECT_PPP_NO,
	NET_CONNECT_PPP_YES
};

/** @class
 *  @brief ��������
 *  @note
 */
struct S_NETSET
{
	S_BYTE m_IP[4]; ///< 192.168.1.1 IP[0]=192
	S_WORD m_PortNO;
	S_BYTE m_Type;///< NET_CONNECT_
	S_BYTE m_NeedPPP;///< NET_CONNECT_NEEDPPP_

public:

//	S_NETSET(void);
	/** @brief ��ʽ��IP,��192.168.1.1
	 *  @param[out] pStrIP �������
	 *  @return pStrIP
	 */
	S_CHAR *GetIpString(S_CHAR *pStrIP)const;
	int Parse(const char *pSetting);
	int Print(char *pSetting);
};

/** @brief ��������λ */
enum
{
	COMM_DATA_BIT5 = 5,
	COMM_DATA_BIT6,
	COMM_DATA_BIT7,
	COMM_DATA_BIT8
};

/** @brief ����ֹͣλ */
enum
{
	COMM_STOP_BIT1 = 1,
	COMM_STOP_BIT2, 
	COMM_STOP_BIT3
};

/** @brief ������żУ�� */
enum
{
	COMM_PARITY_NO = 0,
	COMM_PARITY_ODD,
	COMM_PARITY_EVEN
};

/** @brief ����CDλ */
enum
{
	COMM_CDCHECK_NO = 0,
	COMM_CDCHECK_YES
};

/** @class
 *  @brief ��������
 *  @note
 */
struct S_COMSET
{
	S_DWORD  m_PortNO:8;///< �˿ں�
	S_DWORD  m_DataBit:8;///< [COMM_DATA_] 5,6,7,8
	S_DWORD  m_StopBit:8;///< [COMM_STOP_] 1->1λ,2->2λ,3->1.5
	S_DWORD  m_Parity:4;///< [COMM_PARITY_] 0->no parity;1->odd parity;2->even parity;
	S_DWORD  m_CDCheck:4; ///< [COMM_CDCHECK_]
	S_DWORD  m_Baud;///< 115200, 57600, 38400, 19200, 9600, 4800, 2400, 1200, 600,300

//	S_COMSET(void);
	int Parse(const char *pSetting);
	int Print(char *pSetting);
	int Parse(S_BYTE Setting);
	S_BYTE Print(void);
};

/** @class
 *  @brief PSTN������
 *  @note
 */
//PSTN��Դ��ʽ
enum
{
	PSTN_POWER_METHOD_ALWAYS_ON=0,//���ϵ緽ʽ
	PSTN_POWER_METHOD_RING1_ON=1,//һ�������ϵ緽ʽ
	PSTN_POWER_METHOD_RING2_ON=2,//���������ϵ緽ʽ
};
//PSTN���ŷ�ʽ
enum
{
	PSTN_DIAL_METHOD_NORMAL=0,//�������ŷ�ʽ
	PSTN_DIAL_METHOD_TWICE=1,//���β��ŷ�ʽ
};

struct S_PSTNSET
{
	S_DWORD  m_PortNO:8;///< �˿ں�
	S_DWORD  m_DataBit:8;///< [COMM_DATA_] 5,6,7,8
	S_DWORD  m_StopBit:8;///< [COMM_STOP_] 1->1λ,2->2λ,3->1.5
	S_DWORD  m_Parity:4;///< [COMM_PARITY_] 0->no parity;1->odd parity;2->even parity;
	S_DWORD  m_PowerMethod:2; ///< ��Դ�������μ�PSTN_POWER_METHOD
	S_DWORD  m_DialMethod:2; ///< ���ŷ�ʽ���μ�PSTN_DIAL_METHOD
	S_DWORD  m_Baud;///< 115200, 57600, 38400, 19200, 9600, 4800, 2400, 1200, 600,300

//	S_PSTNSET(void);
	int Parse(const char *pSetting);
	int Print(char *pSetting);
};


/** @class
 *  @brief �˿�����
 *  @note
 *  ����ṹ�壬ʡ�ռ�
 */
union S_PORTSET
{
	S_NETSET  m_NetSet; ///< ��������
	S_COMSET m_ComSet; ///< ��������
	S_PSTNSET m_PstnSet; ///< PSTN����
};

bool operator==(const S_PORTSET &L, const S_PORTSET &R);

/** @brief ͨ������ */
typedef enum
{
	CHANNEL_TYPE_UNKNOW,
	CHANNEL_TYPE_NET,
	CHANNEL_TYPE_COM,
	CHANNEL_TYPE_PSTN,

	CHANNEL_TYPE_MAX
}T_emChannelType;

#define CHANNEL_NAME_MAX	16


/** @class
 *  @brief ͨ���ṹ����
 */
struct S_CHANNEL
{
	S_BOOL m_Enable; ///< ͨ��ʹ��
	S_BYTE m_Name[CHANNEL_NAME_MAX]; ///< ͨ������
	S_BYTE m_Type; ///<ͨ�����ͣ��μ�CHANNEL_TYPE_
	S_BYTE m_ConnectTimeoutS; ///<���ӳ�ʱ����λ��
	S_WORD m_ConnectInterS; ///<���Ӽ������λ��
	S_WORD m_IdleTimeoutS; ///<���г�ʱ����λ��
	S_WORD m_ReadTimeoutMS; ///<����ʱ����λ����
	S_WORD m_WriteTimeoutMS; ///<д��ʱ����λ����
	S_WORD m_SubReadTimeoutMS; ///<�Ͳ����ʱ����λ����
	S_WORD m_CharReadTimeoutMS; ///<�ַ�����ʱ����λ����
	S_WORD m_PackageLenMax; ///<���ĳ���
	S_PORTSET m_PortSet; ///<�˿�����

public:
	S_CHANNEL(void);
	S_CHANNEL(const S_CHANNEL &r);
	~S_CHANNEL();

	S_CHANNEL & operator=(const S_CHANNEL &r);

public:
	/** @brief �ж��Ƿ���ҪPPP����
	 *  @return true ��Ҫ��false ����Ҫ
	 */
	S_BOOL NeedPPP(void);
	S_BOOL IsTcpClient(void);
	S_BOOL IsTcpServer(void);
	S_BOOL IsTcpLink(void);
	S_BOOL IsUdpClient(void);
	S_BOOL IsUdpServer(void);
	S_BOOL IsUdpLink(void);
};


#endif //__FND_CHANNEL_H__


