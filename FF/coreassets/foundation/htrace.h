/** @file
 *  @brief 跟踪系统定义
 *  @author 
 *  @date 2011/10/15
 *  @version 0.1
 *  1、定义日志系统跟踪类
 *  2、日志系统配置定义
 */
#ifndef __MP_HTRACE_H__
#define __MP_HTRACE_H__

/** 错误信息级别定义：
 * 1.error:严重错误，可能导致程序不能正常运行。
 * 2.warning:与正常运行结果相反的，但不影响程序正常运行。
 * 3.information:比较重要的程序运行信息，通过本信息级别可以基本定位问题出现的模块。
 * 4.verbose:一般的程序运行信息，如报文输出
 * 5.debug:程序调试信息。程序调试完后，需要注释掉，以免影响程序运行效率。
 */
/** 日志输出的要求：
 *  1、日志用中文显示；
 *  2、日志的可理解性；
 *  3、日志输出格式化(美观)；
 *  4、日志不同级别输出量；
 *  5、日志输出级别符合性；
 */
/** @code
 *  logit<<HEX<<string<<endl;
 *  @endcode
 */

#include <string>
using std::string;

/** @breif 日志信息配置文件包含 */
#include "traceset.h"
#include "basealgo.h"
#include "timeop.h"

//
//
#define 	TRACEOP_DEC  0x00000000UL
#define		TRACEOP_HEX  0x00000100UL
#define		TRACEOP_hex  0x00000200UL
#define		TRACEOP_OCT  0x00000300UL
#define		TRACEOP_BIN  0x00000400UL
#define		TRACEOP_JZMSK  0x00000700UL

class C_HTRACE;
/** @brief 跟踪操作函数定义 */
typedef C_HTRACE & TRACEOP(C_HTRACE & v);

#if 1
extern C_HTRACE & tag(C_HTRACE & v);
/** @brief 行输出算子 */
extern C_HTRACE & endl(C_HTRACE & v);
/** @brief 时钟输出算子 */
extern C_HTRACE & time(C_HTRACE & v);
/** @brief 标题输出算子 */
extern C_HTRACE & title(C_HTRACE & v);
/** @brief 标签（title+time）输出算子 */
//	friend C_HTRACE & tag(C_HTRACE & v);
/** @brief 十六进制（大写）输出算子 */
extern C_HTRACE & HEX(C_HTRACE & v);
/** @brief 十六进制（小写）输出算子 */
extern C_HTRACE & hex(C_HTRACE & v);
/** @brief 十进制输出算子 */
extern C_HTRACE & dec(C_HTRACE & v);
/** @brief 八进制输出算子 */
extern C_HTRACE & oct(C_HTRACE & v);
/** @brief 二进制输出算子 */
extern C_HTRACE & bin(C_HTRACE & v);
#endif

/** @class
 *  @brief 进程跟踪信息类
 *  @remarks 注意操作函数的使用
 */
class C_HTRACE
{
public:
	C_HTRACE(unsigned char *ptitle = NULL, int level = S_TraceSet::LEVEL_INFO, int way = S_TraceSet::WAY_STDOUT);
	~C_HTRACE();

public:
	/** @brief 设置系统跟踪日志输出路径 */
	static void SetLogBasePath(const char *pLogBasePath);
	/** @brief 获取系统跟踪日志输出路径 */
	static const char *GetLogBasePath(void);
	/** @brief 设置系统跟踪配置 */
	static void SetSystemTraceSetting(S_TraceSet *psysset);
	/** @brief 获取系统跟踪配置 */
	static S_TraceSet* GetSystemTraceSetting(void);

public:
	/** @brief <<操作符重载 */
	C_HTRACE & operator<<(TRACEOP *pfnop);
	C_HTRACE & operator<<(const char *pstr);
	C_HTRACE & operator<<(const unsigned char *pstr);
	C_HTRACE & operator<<(char *pstr);
	C_HTRACE & operator<<(unsigned char *pstr);
	C_HTRACE & operator<<(float val);
	C_HTRACE & operator<<(double val);
	C_HTRACE & operator<<(const string &rstr);
	C_HTRACE & operator<<(const C_BCD &rBcd);
	C_HTRACE & operator<<(const S_DATE &rDate);
	C_HTRACE & operator<<(const S_TIMEX &rTimex);
	template <class baseT> C_HTRACE & operator<<(baseT val);

	/** @brief 重载函数运算符() */
	void operator()(const char *pszformat, ...);

public:
	/** @brief 行输出算子 */
#if 1
	friend C_HTRACE & endl(C_HTRACE & v);
	/** @brief 时钟输出算子 */
	friend C_HTRACE & time(C_HTRACE & v);
	/** @brief 标题输出算子 */
	friend C_HTRACE & title(C_HTRACE & v);
	/** @brief 标签（title+time）输出算子 */
	friend C_HTRACE & tag(C_HTRACE & v);
	/** @brief 十六进制（大写）输出算子 */
	friend C_HTRACE & HEX(C_HTRACE & v);
	/** @brief 十六进制（小写）输出算子 */
	friend C_HTRACE & hex(C_HTRACE & v);
	/** @brief 十进制输出算子 */
	friend C_HTRACE & dec(C_HTRACE & v);
	/** @brief 八进制输出算子 */
	friend C_HTRACE & oct(C_HTRACE & v);
	/** @brief 二进制输出算子 */
	friend C_HTRACE & bin(C_HTRACE & v);
#endif
private:
	/** @brief 十六进制输出重载 */
	void _ToHexString(unsigned char val, string &strDigit, bool upcase);
	void ToHexString(unsigned char val, string &strDigit, bool upcase);
	void ToHexString(char val, string &strDigit, bool upcase);
	void ToHexString(unsigned short val, string &strDigit, bool upcase);
	void ToHexString(short val, string &strDigit, bool upcase);
	void ToHexString(unsigned long val, string &strDigit, bool upcase);
	void ToHexString(long val, string &strDigit, bool upcase);
	void ToHexString(unsigned int val, string &strDigit, bool upcase);
	void ToHexString(int val, string &strDigit, bool upcase);

	/** @brief 十进制输出重载 */
	void ToDecString(long val, string &strDigit);
	void ToDecString(unsigned long val, string &strDigit);
	void ToDecString(char val, string &strDigit);
	void ToDecString(unsigned char val, string &strDigit);
	void ToDecString(short val, string &strDigit);
	void ToDecString(unsigned short val, string &strDigit);
	void ToDecString(int val, string &strDigit);
	void ToDecString(unsigned int val, string &strDigit);
public:
	/** @brief 八进制输出重载 */
	void ToOctString(int val, string &strDigit);

	/** @brief 取输出文件名
	 *  @param[in] rSet 跟踪配置
	 *  @param[out] Outfile 输出文件名
	 *  @return 输出文件名指针
	 */
	char *GetOutfileName(const S_TraceSet &rSet, char *Outfile);
	/** @brief 取备份文件名
	 *  @param[in] rSet 跟踪配置
	 *  @param[out] OutDir 输出文件名
	 *  @return 输出文件名指针
	 */
	char *GetBackfileName(const S_TraceSet &rSet, char *OutDir);

	/** @brief 文件改名
	 *  @param[in] SrcFile 源文件名
	 *  @param[in] DstFile 目的文件名
	 *  @return true
	 */
	bool RenameFile(const char *SrcFile, const char *DstFile);
	/** @brief 删除文件
	 *  @param[in] rSet 跟踪配置
	 *  @return true
	 */
	bool DelLogFile(const S_TraceSet &rSet);
	/** @brief 删除文件
	 *  @param[in] rSet 跟踪配置
	 *  @return true
	 */
	bool DelOneLogFile(const S_TraceSet &rSet);

public:
	unsigned long m_operate; ///< 操作符
	string m_context; ///< 输出信息缓冲
	S_TraceSet m_curset; ///< 当前配置
	static const char *m_pLogBasePath; ///< 日志输出径
	static S_TraceSet *m_pSystemSet; ///< 系统配置
};

template <class baseT>
C_HTRACE & C_HTRACE::operator<<(baseT val)
{
	unsigned long op = m_operate & TRACEOP_JZMSK;
	string strDigit;

	if (op == TRACEOP_HEX)
		ToHexString(val, strDigit, true);
	else if (op == TRACEOP_hex)
		ToHexString(val, strDigit, false);
	else
		ToDecString(val, strDigit);

	m_context.append(strDigit);

	return *this;
}

/** @brief 使用方法
 *  应用程序用户注意：hai_logn日志输出用在非-exec命令产生的应用程序中，
 *  hai_logn不会产生信息级别标识。
 */
extern C_HTRACE hai_logn;
extern C_HTRACE hai_loge;
extern C_HTRACE hai_logw;
extern C_HTRACE hai_logi;
extern C_HTRACE hai_logv;
extern C_HTRACE hai_logd;

#define logn hai_logn
#define loge hai_loge
#define logw hai_logw
#define logi hai_logi
#define logv hai_logv
#define logd hai_logd

/** 以下用宏的形式给出加了tag的标识符，方便程序中自动添加标题和时间。
 *  请在使用时采用以下形式。
 */
#define loget hai_loge<<tag
#define logwt hai_logw<<tag
#define logit hai_logi<<tag
#define logvt hai_logv<<tag
#define logdt hai_logd<<tag

#endif //__MPROCESS_HTRACE_H__

