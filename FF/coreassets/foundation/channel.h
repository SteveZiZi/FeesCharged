/** @file
 *  @brief 通道类定义
 *  @author 
 *  @date 2011/11/28
 *  @version 0.1
 */
#ifndef __FND_CHANNEL_H__
#define __FND_CHANNEL_H__

#include "basedef.h"

/** @brief 网络连接类型 */
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

/** @brief PPP拨号 */
enum
{
	NET_CONNECT_PPP_NO,
	NET_CONNECT_PPP_YES
};

/** @class
 *  @brief 网络配置
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
	/** @brief 格式化IP,如192.168.1.1
	 *  @param[out] pStrIP 输出缓冲
	 *  @return pStrIP
	 */
	S_CHAR *GetIpString(S_CHAR *pStrIP)const;
	int Parse(const char *pSetting);
	int Print(char *pSetting);
};

/** @brief 串口数据位 */
enum
{
	COMM_DATA_BIT5 = 5,
	COMM_DATA_BIT6,
	COMM_DATA_BIT7,
	COMM_DATA_BIT8
};

/** @brief 串口停止位 */
enum
{
	COMM_STOP_BIT1 = 1,
	COMM_STOP_BIT2, 
	COMM_STOP_BIT3
};

/** @brief 串口奇偶校验 */
enum
{
	COMM_PARITY_NO = 0,
	COMM_PARITY_ODD,
	COMM_PARITY_EVEN
};

/** @brief 串口CD位 */
enum
{
	COMM_CDCHECK_NO = 0,
	COMM_CDCHECK_YES
};

/** @class
 *  @brief 串口配置
 *  @note
 */
struct S_COMSET
{
	S_DWORD  m_PortNO:8;///< 端口号
	S_DWORD  m_DataBit:8;///< [COMM_DATA_] 5,6,7,8
	S_DWORD  m_StopBit:8;///< [COMM_STOP_] 1->1位,2->2位,3->1.5
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
 *  @brief PSTN口配置
 *  @note
 */
//PSTN电源方式
enum
{
	PSTN_POWER_METHOD_ALWAYS_ON=0,//常上电方式
	PSTN_POWER_METHOD_RING1_ON=1,//一声振铃上电方式
	PSTN_POWER_METHOD_RING2_ON=2,//二声振铃上电方式
};
//PSTN拨号方式
enum
{
	PSTN_DIAL_METHOD_NORMAL=0,//正常拨号方式
	PSTN_DIAL_METHOD_TWICE=1,//二次拨号方式
};

struct S_PSTNSET
{
	S_DWORD  m_PortNO:8;///< 端口号
	S_DWORD  m_DataBit:8;///< [COMM_DATA_] 5,6,7,8
	S_DWORD  m_StopBit:8;///< [COMM_STOP_] 1->1位,2->2位,3->1.5
	S_DWORD  m_Parity:4;///< [COMM_PARITY_] 0->no parity;1->odd parity;2->even parity;
	S_DWORD  m_PowerMethod:2; ///< 电源方案，参见PSTN_POWER_METHOD
	S_DWORD  m_DialMethod:2; ///< 拨号方式，参见PSTN_DIAL_METHOD
	S_DWORD  m_Baud;///< 115200, 57600, 38400, 19200, 9600, 4800, 2400, 1200, 600,300

//	S_PSTNSET(void);
	int Parse(const char *pSetting);
	int Print(char *pSetting);
};


/** @class
 *  @brief 端口配置
 *  @note
 *  共享结构体，省空间
 */
union S_PORTSET
{
	S_NETSET  m_NetSet; ///< 网络配置
	S_COMSET m_ComSet; ///< 串口配置
	S_PSTNSET m_PstnSet; ///< PSTN配置
};

bool operator==(const S_PORTSET &L, const S_PORTSET &R);

/** @brief 通道类型 */
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
 *  @brief 通道结构定义
 */
struct S_CHANNEL
{
	S_BOOL m_Enable; ///< 通道使能
	S_BYTE m_Name[CHANNEL_NAME_MAX]; ///< 通道名称
	S_BYTE m_Type; ///<通道类型，参见CHANNEL_TYPE_
	S_BYTE m_ConnectTimeoutS; ///<连接超时，单位秒
	S_WORD m_ConnectInterS; ///<连接间隔，单位秒
	S_WORD m_IdleTimeoutS; ///<空闲超时，单位秒
	S_WORD m_ReadTimeoutMS; ///<读超时，单位毫秒
	S_WORD m_WriteTimeoutMS; ///<写超时，单位毫秒
	S_WORD m_SubReadTimeoutMS; ///<低层读超时，单位毫秒
	S_WORD m_CharReadTimeoutMS; ///<字符读超时，单位毫秒
	S_WORD m_PackageLenMax; ///<报文长度
	S_PORTSET m_PortSet; ///<端口设置

public:
	S_CHANNEL(void);
	S_CHANNEL(const S_CHANNEL &r);
	~S_CHANNEL();

	S_CHANNEL & operator=(const S_CHANNEL &r);

public:
	/** @brief 判断是否需要PPP拨号
	 *  @return true 需要；false 不需要
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


