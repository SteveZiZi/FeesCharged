/** @file
 *  @brief 跟踪配置定义
 *  @author 
 *  @date 2011/10/15
 *  @version 0.1
 *  1、日志系统配置定义
 */
#ifndef __MP_TRACESET_H__
#define __MP_TRACESET_H__
#include "basedef.h"

/** @brief 跟踪信息常量定义 */
#define TRACE_FILE_SIZE_MAX		500000 /*单进程跟踪文件最大*/
#define TRACE_FILE_TOTAL_MAX	8
#define TRACE_FILE_SIZE_MIN		20000
#define TRACE_FILE_TOTAL_MIN	2 /*最少2个*/

#define AVERAGE_PROCESS_TRACE_SIZE_MAX	2000000 /*平均每个进程最大存储空间*/
#define SINGLE_PROCESS_TRACE_SIZE_MAX	(TRACE_FILE_SIZE_MAX*TRACE_FILE_TOTAL_MAX) /*单个进程最大存储空间*/
#define TRACE_FILE_SIZE_DEFAULT	50000 /*单进程跟踪文件缺省值*/

/** @class
 *  @brief 跟踪信息配置
 *  @remarks 具有国网版和南网版
 */
struct S_TraceSet
{
public:
	/** @brief 日志级别：关闭-错误-警告-信息-冗余-DEBUG */
	enum{LEVEL_DISABLED, LEVEL_ERROR, LEVEL_WARNING, LEVEL_INFO, LEVEL_VERBOSE, LEVEL_DEBUG, LEVEL_MAX};
	/** @brief 输出目标：无-标准输出-日志文件-ttyp0-ttyp1-ttyp2-ttyp3-ttyp4-ttyp5-ttyp6 */
	enum{WAY_NONE, WAY_STDOUT, WAY_LOGFILE, WAY_TTYP0, WAY_TTYP1, WAY_TTYP2, WAY_TTYP3, WAY_TTYP4, WAY_TTYP5, WAY_TTYP6, WAY_MAX};

	/** @brief 用户构造函数 */
	S_TraceSet(unsigned char *ptitle=(unsigned char *)"unknow", int level=LEVEL_INFO, int way=WAY_STDOUT);
	/** @brief 拷贝构造函数 */
	S_TraceSet(const S_TraceSet &rTraceSet);
	~S_TraceSet();
	/** @brief 重载赋值函数 */
	S_TraceSet & operator=(const S_TraceSet &rTraceSet);

	/** @brief 取配置级别 */
	unsigned char GetLevel(void)const{return m_level;}
	/** @brief 取输出目标 */
	unsigned char GetWay(void)const{return m_way;}
	/** @brief 取输出文件大小 */
	unsigned int  GetFileSize(void)const{return m_FileSiz;}
	/** @brief 取输出文件总数 */
	unsigned char GetFileTotal(void)const{return m_FileTtl;}
	/** @brief 取配置标题 */
	const unsigned char *GetTitle(void)const{return (m_title[0]=='\0')?NULL:m_title;}

	/** @brief 设置级别
	 *  @param[in] level 级别[LEVEL_DISABLED, LEVEL_ERROR, LEVEL_WARNING, LEVEL_INFO, LEVEL_VERBOSE, LEVEL_DEBUG, LEVEL_MAX]
	 *  @return true
	 */
	bool SetLevel(int level);
	/** @brief 设置输出目标
	 *  @param[in] way 目标[WAY_NONE, WAY_STDOUT, WAY_LOGFILE, WAY_TTYP0, WAY_TTYP1...]
	 *  @return true
	 */
	bool SetWay(int way);
	/** @brief 设置输出文件大小
	 *  @param[in] FileSiz 文件大小
	 *  @return true
	 */
	bool SetFileSize(unsigned int FileSiz);
	/** @brief 设置文件总数
	 *  @param[in] FileTtl 文件总数
	 *  @return true
	 */
	bool SetFileTotal(unsigned char FileTtl);
	/** @brief 设置配置标题
	 *  @param[in] ptitle 标题
	 *  @return true
	 */
	bool SetTitle(const unsigned char *ptitle);

public:
	unsigned char m_level; ///< 跟踪信息级别
	unsigned char m_way; ///< 跟踪信息输出目标
	unsigned char m_FileTtl; ///< 跟踪信息文件总数
	unsigned char m_Reserved; ///< 保留
	unsigned int  m_FileSiz; ///< 跟踪信息文件单个大小
	unsigned char m_title[12]; ///< 跟踪信息输出时标题，如“dyjc”
};


#endif //__MPROCESS_TRACESET_H__
