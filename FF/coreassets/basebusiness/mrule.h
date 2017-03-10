/**
 *  @file
 *  @brief 表计规约基础类
 *  @author 
 *  @date 2012-05-10
 *  @version v1.0
 *  @note
 */
#ifndef __BASEB_MRULE_H__
#define __BASEB_MRULE_H__
#include "basedef.h"
#include "basetype.h"

enum
{
	MRULE_PARSE_ERR_NO = 0, //成功
	MRULE_PARSE_ERR_ADDR = -1, //地址错
	MRULE_PARSE_ERR_DI = -2, //数据项错
	MRULE_PARSE_ERR_CTRL = -3, //控制码错
	MRULE_PARSE_ERR_REG = -4, //寄存器错
	MRULE_PARSE_ERR_UNSUPPORT = -5, //规约不支持
	MRULE_PARSE_ERR_RETURN_CODE = -6, // 返回错误码
	MRULE_PARSE_ERR_CHECKSUM = -7, // 校验码错误
	MRULE_PARSE_ERR_CONTENT = -8, // 报文内容出错
	MRULE_PARSE_ERR_NO_DATA = -9, // 无数据内容
	MRULE_PARSE_ERR_NO_FRAME = -10, // 没有收到报文
	MRULE_PARSE_ERR_BUFFER_INVALID = -11, // 数据缓冲无效

	MRULE_PARSE_ERR_OTHER = -100 //其它错
};

/** @brief 表计寄存器与通用寄存器映射 */
struct S_RegisterMap
{
	S_DWORD m_MainID;///< 主寄存器ID
	S_DWORD m_MapID; ///< 映射寄存器ID
	string m_MapName;///< 映射寄存器字符串名称

public:
	S_RegisterMap(void)
	{
		m_MainID = 0x0;
		m_MapID = 0x0;
	}
};
struct S_RegisterMaps:public vector<S_RegisterMap>
{
};
bool operator<(const S_RegisterMap &left, const S_RegisterMap &right);

/** @brief 表计寄存器与通用寄存器映射 (一对多)*/
struct S_RegisterMap3
{
	S_DWORD m_MainID;///< 主寄存器ID
	vector<S_DWORD> m_MapID; ///< 映射寄存器ID

public:
	S_RegisterMap3(void)
	{
		m_MainID = 0x0;
	}
};
struct S_RegisterMaps3:public vector<S_RegisterMap3>
{
};
bool operator<(const S_RegisterMap3 &left, const S_RegisterMap3 &right);


/** @brief 表计规约抽象接口定义 */
class C_MRULE
{
public:
	C_MRULE(void);
	virtual ~C_MRULE();

public:
	S_COMSET GetComSet(void);
	/** @brief 获取表计规约名称 */
	virtual char *GetName(void);
	/** @brief 清除规约缓冲*/
	virtual bool ClearBuffer(void);
	/** @brief 查找本地寄存器 */
	virtual bool FindLocalRegister(const S_REGISTER &CommReg, S_REGISTER &LocalReg);

	/** @brief 提取命令帧
	 *  @param[in] strBuffer 命令缓冲
	 *  @param[out] strCmd 帧输出缓冲
	 *  @return <0: 出错[绝对值是出错的字节数]; =0: 半帧; >0: 成功
	 */
	virtual int ExtractCmd(const string &strBuffer, string &strCmd);

	/** @brief 组读命令帧
	 *  @param[in] strAddress 表计地址
	 *  @param[in] CommReg 通用寄存器
	 *  @param[out] DataGram 帧输出缓冲
	 *  @param[in] fRsvReg true备用寄存器;false主寄存器
	 *  @return <0: 出错或无此要求帧; =0: 无需组帧，直接分析缓冲; >0: 组帧成功
	 */
	virtual int MakeReadCmd(const string &strAddress, const S_REGISTER &CommReg, S_DATAGRAM &DataGram, bool fRsvReg);
	/** @brief 分析读命令返回帧
	 *  @param[in] strCmd 返回命令帧,如为空则分析缓冲
	 *  @param[in] CommReg 通用寄存器
	 *  @param[out] strData 数据输出缓冲
	 *  @return <0:出错[参见MRULE_PARSE_ERR_];=0:成功
	 */
	virtual int ParseReadCmd(const string &strCmd, const S_REGISTER &CommReg, string &strData);

	/** @brief 组写命令帧
	 *  @param[in] strAddress 表计地址
	 *  @param[in] CommReg 通用寄存器
	 *  @param[in] strPassword 密码 PAP1P2P3
	 *  @param[in] strOperator 操作者代码
	 *  @param[in] strData 数据输入缓冲
	 *  @param[out] DataGram 帧输出缓冲
	 *  @return <=0: 出错或无此要求帧; >0: 组帧成功
	 */
	virtual int MakeWriteCmd(const string &strAddress, const S_REGISTER &CommReg, const string &strPassword, const string &strOperator, const string &strData, S_DATAGRAM &DataGram);
	/** @brief 分析写命令返回帧
	 *  @param[in] strCmd 返回命令帧
	 *  @param[in] strAddress 表计地址
	 *  @param[in] CommReg 通用寄存器
	 *  @param[out] strData 数据输出缓冲
	 *  @return <0:出错[参见MRULE_PARSE_ERR_];=0:成功
	 */
	virtual int ParseWriteCmd(const string &strCmd, const string &strAddress, const S_REGISTER &CommReg, string &strData);

	/** @brief 组安全认证命令帧
	 *  @param[in] strAddress 表计地址
	 *  @param[in] CommReg 通用寄存器
	 *  @param[in] strOperator 操作者代码
	 *  @param[in] strData 数据输入缓冲
	 *  @param[out] DataGram 帧输出缓冲
	 *  @return <=0: 出错或无此要求帧; >0: 组帧成功
	 */
	virtual int MakeSecurityCmd(const string &strAddress, const S_REGISTER &CommReg, const string &strOperator, const string &strData, S_DATAGRAM &DataGram);
	/** @brief 分析安全认证命令返回帧
	 *  @param[in] strCmd 返回命令帧
	 *  @param[in] strAddress 表计地址
	 *  @param[in] CommReg 通用寄存器
	 *  @param[out] strData 数据输出缓冲
	 *  @return <0:出错[参见MRULE_PARSE_ERR_];>=0:成功
	 */
	virtual int ParseSecurityCmd(const string &strCmd, const string &strAddress, const S_REGISTER &CommReg, string &strData);

	/** @brief 组广播校时帧
	 *  @param[in] strAddress 表计地址
	 *  @param[in] CommReg 通用寄存器
	 *  @param[in] timeExpected 期望时间
	 *  @param[out] DataGram 帧输出缓冲
	 *  @return <0: 出错或无此要求帧; =0: 无需组帧，直接分析缓冲; >0: 组帧成功
	 */
	virtual int MakeBroadcastCmd(const string &strAddress, const S_REGISTER &CommReg, S_TIME timeExpected, S_DATAGRAM &DataGram);

	/** @brief 组读报文[仅根据传入信息组报文]
	 *  @param[in] strAddress 表计地址
	 *  @param[in] DI 本地寄存器
	 *  @param[out] DataGram 帧输出缓冲
	 *  @return 帧长度
	 */
	virtual int MakeReadFrame(const string &strAddress, S_DWORD DI, S_DATAGRAM &DataGram);

	/** @brief 分析读报文[仅分析传入的报文]
	 *  @param[in] strCmd 报文
	 *  @param[in] strAddress 表计地址
	 *  @param[in] DI 通用寄存器
	 *  @param[out] strData 数据输出缓冲
	 *  @return 数据长度
	 */
	virtual int ParseReadFrame(const string &strCmd, string &strAddress, S_DWORD &DI, string &strData);

	/** @brief 组跳合闸、报警、保电帧
	 *  @param[in] strAddress 表计地址
	 *  @param[in] strPassword 密码 PAP1P2P3
	 *  @param[in] strOperator 操作者代码
	 *  @param[in] strData 数据输入缓冲
	 *  @param[out] DataGram 帧输出缓冲
	 *  @return <=0: 出错或无此要求帧; >0: 组帧成功
	 */
	virtual int MakeBrakeCmd(const string &strAddress, const string &strPassword, const string &strOperator, const string &strData, S_DATAGRAM &DataGram);
	/** @brief 分析跳合闸、报警、保电命令返回帧
	 *  @param[in] strCmd 返回命令帧
	 *  @param[in] strAddress 表计地址
	 *  @param[out] strData 数据输出缓冲
	 *  @return <0:出错[参见MRULE_PARSE_ERR_];=0:成功
	 */
	virtual int ParseBrakeCmd(const string &strCmd, const string &strAddress, string &strData);

protected:
	S_COMSET m_ComSet;
};

#endif //__BASEB_MRULE_H__

