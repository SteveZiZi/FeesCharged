/**
 *  @file
 *  @brief 现场总线命令过滤器
 *  @author 
 *  @date 2012-05-10
 *  @version v1.0
 *  @note 现场总线接收数据后调用
 */
#ifndef __BASEB_CMD_FILTER_H__
#define __BASEB_CMD_FILTER_H__
#include "basedef.h"
#include "basetype.h"

/** @brief 命令过滤 */
class C_CmdFilter
{
public:
	C_CmdFilter(const string &strFilterName);
	~C_CmdFilter();

public:
	/** @brief 命令过滤
	 *  @param[in] strBuffer 命令缓冲
	 *  @param[out] strCmd 帧输出缓冲
	 *  @return <0: 出错[绝对值是出错的字节数]; =0: 半帧; >0: 成功
	 */
	int operator()(const string &strBuffer, string &strCmd)const;

private:
	string m_strFilterName; //命令过滤器名称
};

/** @brief 命令访问者 */
class C_CmdVisitor
{
public:
	C_CmdVisitor(void);
	~C_CmdVisitor();

public:
	/** @brief 组规约命令
	 *  @param[in] strBuffer 命令缓冲
	 *  @param[in] CommReg 通用寄存器
	 *  @param[out] strCmd 帧输出缓冲
	 *  @return <=0: 出错; >0: 成功
	 */
	int operator()(const S_ReportData &ReportData, const S_REGISTER &CommReg, string &strCmd)const;
};

#endif //__BASEB_CMD_FILTER_H__

