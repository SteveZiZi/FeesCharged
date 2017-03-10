/**
 *  @file
 *  @brief 交流采样消息分析器
 *  @author 
 *  @date 2013-02-25
 *  @version v1.0
 *  @note 交采进程和别的进程的IPC通道
 */
#ifndef __BASEB_JLCY_MSG_H__
#define __BASEB_JLCY_MSG_H__
#include "basedef.h"

/** @note 消息格式
 *  send: FrameLen(2B)+DI(4B)+DI(4B)+...+DI(4B)
 *  recv: FrameLen(2B)+DI(4B)+DataLen(1B)+Data(NB)+...+DI(4B)+DataLen(1B)+Data(NB)
 *  注：数据传递按小端方式
 */

/** @brief 交采数据格式 */
struct S_JlcyData
{
	S_DWORD m_DI; ///< 数据ID，如0x05060101 与DLT645-2007规约相仿
	string m_Data; ///< 数据内容
};
struct S_JlcyDatas:public vector<S_JlcyData>
{
};

/** @brief 交流采样消息分析类*/
class C_JlcyMsg
{
public:
	C_JlcyMsg(void);
	~C_JlcyMsg();

public:
	/** @brief 提取命令帧
	 *  @param[in] strBuffer 命令缓冲
	 *  @param[out] strCmd 帧输出缓冲
	 *  @return <0: 出错[绝对值是出错的字节数]; =0: 半帧; >0: 成功
	 */
	int ExtractCmd(const string &strBuffer, string &strCmd);

	/** @brief 组织命令
	 *  @param[in] JlcyDatas 数据项数组
	 *  @param[out] strCmd 帧输出缓冲
	 *  @return <=0: 出错; >0: 输出命令长度
	 *  @note 非交采进程调用
	 */
	int Master_MakeCmd(const S_JlcyDatas &JlcyDatas, string &strCmd);
	/** @brief 分析命令
	 *  @param[in] strCmd 帧输出缓冲
	 *  @param[out] JlcyDatas 数据项数组
	 *  @return <=0: 出错; >0: 数据项个数
	 *  @note 非交采进程调用
	 */
	int Master_ParseCmd(const string &strCmd, S_JlcyDatas &JlcyDatas);
	/** @brief 组织命令
	 *  @param[in] JlcyDatas 数据项数组
	 *  @param[out] strCmd 帧输出缓冲
	 *  @return <=0: 出错; >0: 输出命令长度
	 *  @note 交采进程调用
	 */
	int Slave_MakeCmd(const S_JlcyDatas &JlcyDatas, string &strCmd);
	/** @brief 分析命令
	 *  @param[in] strCmd 帧输出缓冲
	 *  @param[out] JlcyDatas 数据项数组
	 *  @return <=0: 出错; >0: 数据项个数
	 *  @note 交采进程调用
	 */
	int Slave_ParseCmd(const string &strCmd, S_JlcyDatas &JlcyDatas);
};

#endif //__BASEB_JLCY_MSG_H__

