/** @file
 *  @brief ����ϵͳ����
 *  @author 
 *  @date 2011/10/15
 *  @version 0.1
 *  1��������־ϵͳ������
 *  2����־ϵͳ���ö���
 */
#ifndef __MP_HTRACE_H__
#define __MP_HTRACE_H__

/** ������Ϣ�����壺
 * 1.error:���ش��󣬿��ܵ��³������������С�
 * 2.warning:���������н���෴�ģ�����Ӱ������������С�
 * 3.information:�Ƚ���Ҫ�ĳ���������Ϣ��ͨ������Ϣ������Ի�����λ������ֵ�ģ�顣
 * 4.verbose:һ��ĳ���������Ϣ���籨�����
 * 5.debug:���������Ϣ��������������Ҫע�͵�������Ӱ���������Ч�ʡ�
 */
/** ��־�����Ҫ��
 *  1����־��������ʾ��
 *  2����־�Ŀ�����ԣ�
 *  3����־�����ʽ��(����)��
 *  4����־��ͬ�����������
 *  5����־�����������ԣ�
 */
/** @code
 *  logit<<HEX<<string<<endl;
 *  @endcode
 */

#include <string>
using std::string;

/** @breif ��־��Ϣ�����ļ����� */
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
/** @brief ���ٲ����������� */
typedef C_HTRACE & TRACEOP(C_HTRACE & v);

#if 1
extern C_HTRACE & tag(C_HTRACE & v);
/** @brief ��������� */
extern C_HTRACE & endl(C_HTRACE & v);
/** @brief ʱ��������� */
extern C_HTRACE & time(C_HTRACE & v);
/** @brief ����������� */
extern C_HTRACE & title(C_HTRACE & v);
/** @brief ��ǩ��title+time��������� */
//	friend C_HTRACE & tag(C_HTRACE & v);
/** @brief ʮ�����ƣ���д��������� */
extern C_HTRACE & HEX(C_HTRACE & v);
/** @brief ʮ�����ƣ�Сд��������� */
extern C_HTRACE & hex(C_HTRACE & v);
/** @brief ʮ����������� */
extern C_HTRACE & dec(C_HTRACE & v);
/** @brief �˽���������� */
extern C_HTRACE & oct(C_HTRACE & v);
/** @brief ������������� */
extern C_HTRACE & bin(C_HTRACE & v);
#endif

/** @class
 *  @brief ���̸�����Ϣ��
 *  @remarks ע�����������ʹ��
 */
class C_HTRACE
{
public:
	C_HTRACE(unsigned char *ptitle = NULL, int level = S_TraceSet::LEVEL_INFO, int way = S_TraceSet::WAY_STDOUT);
	~C_HTRACE();

public:
	/** @brief ����ϵͳ������־���·�� */
	static void SetLogBasePath(const char *pLogBasePath);
	/** @brief ��ȡϵͳ������־���·�� */
	static const char *GetLogBasePath(void);
	/** @brief ����ϵͳ�������� */
	static void SetSystemTraceSetting(S_TraceSet *psysset);
	/** @brief ��ȡϵͳ�������� */
	static S_TraceSet* GetSystemTraceSetting(void);

public:
	/** @brief <<���������� */
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

	/** @brief ���غ��������() */
	void operator()(const char *pszformat, ...);

public:
	/** @brief ��������� */
#if 1
	friend C_HTRACE & endl(C_HTRACE & v);
	/** @brief ʱ��������� */
	friend C_HTRACE & time(C_HTRACE & v);
	/** @brief ����������� */
	friend C_HTRACE & title(C_HTRACE & v);
	/** @brief ��ǩ��title+time��������� */
	friend C_HTRACE & tag(C_HTRACE & v);
	/** @brief ʮ�����ƣ���д��������� */
	friend C_HTRACE & HEX(C_HTRACE & v);
	/** @brief ʮ�����ƣ�Сд��������� */
	friend C_HTRACE & hex(C_HTRACE & v);
	/** @brief ʮ����������� */
	friend C_HTRACE & dec(C_HTRACE & v);
	/** @brief �˽���������� */
	friend C_HTRACE & oct(C_HTRACE & v);
	/** @brief ������������� */
	friend C_HTRACE & bin(C_HTRACE & v);
#endif
private:
	/** @brief ʮ������������� */
	void _ToHexString(unsigned char val, string &strDigit, bool upcase);
	void ToHexString(unsigned char val, string &strDigit, bool upcase);
	void ToHexString(char val, string &strDigit, bool upcase);
	void ToHexString(unsigned short val, string &strDigit, bool upcase);
	void ToHexString(short val, string &strDigit, bool upcase);
	void ToHexString(unsigned long val, string &strDigit, bool upcase);
	void ToHexString(long val, string &strDigit, bool upcase);
	void ToHexString(unsigned int val, string &strDigit, bool upcase);
	void ToHexString(int val, string &strDigit, bool upcase);

	/** @brief ʮ����������� */
	void ToDecString(long val, string &strDigit);
	void ToDecString(unsigned long val, string &strDigit);
	void ToDecString(char val, string &strDigit);
	void ToDecString(unsigned char val, string &strDigit);
	void ToDecString(short val, string &strDigit);
	void ToDecString(unsigned short val, string &strDigit);
	void ToDecString(int val, string &strDigit);
	void ToDecString(unsigned int val, string &strDigit);
public:
	/** @brief �˽���������� */
	void ToOctString(int val, string &strDigit);

	/** @brief ȡ����ļ���
	 *  @param[in] rSet ��������
	 *  @param[out] Outfile ����ļ���
	 *  @return ����ļ���ָ��
	 */
	char *GetOutfileName(const S_TraceSet &rSet, char *Outfile);
	/** @brief ȡ�����ļ���
	 *  @param[in] rSet ��������
	 *  @param[out] OutDir ����ļ���
	 *  @return ����ļ���ָ��
	 */
	char *GetBackfileName(const S_TraceSet &rSet, char *OutDir);

	/** @brief �ļ�����
	 *  @param[in] SrcFile Դ�ļ���
	 *  @param[in] DstFile Ŀ���ļ���
	 *  @return true
	 */
	bool RenameFile(const char *SrcFile, const char *DstFile);
	/** @brief ɾ���ļ�
	 *  @param[in] rSet ��������
	 *  @return true
	 */
	bool DelLogFile(const S_TraceSet &rSet);
	/** @brief ɾ���ļ�
	 *  @param[in] rSet ��������
	 *  @return true
	 */
	bool DelOneLogFile(const S_TraceSet &rSet);

public:
	unsigned long m_operate; ///< ������
	string m_context; ///< �����Ϣ����
	S_TraceSet m_curset; ///< ��ǰ����
	static const char *m_pLogBasePath; ///< ��־�����
	static S_TraceSet *m_pSystemSet; ///< ϵͳ����
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

/** @brief ʹ�÷���
 *  Ӧ�ó����û�ע�⣺hai_logn��־������ڷ�-exec���������Ӧ�ó����У�
 *  hai_logn���������Ϣ�����ʶ��
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

/** �����ú����ʽ��������tag�ı�ʶ��������������Զ���ӱ����ʱ�䡣
 *  ����ʹ��ʱ����������ʽ��
 */
#define loget hai_loge<<tag
#define logwt hai_logw<<tag
#define logit hai_logi<<tag
#define logvt hai_logv<<tag
#define logdt hai_logd<<tag

#endif //__MPROCESS_HTRACE_H__

