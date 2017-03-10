/** @file
 *  @brief �������ö���
 *  @author 
 *  @date 2011/10/15
 *  @version 0.1
 *  1����־ϵͳ���ö���
 */
#ifndef __MP_TRACESET_H__
#define __MP_TRACESET_H__
#include "basedef.h"

/** @brief ������Ϣ�������� */
#define TRACE_FILE_SIZE_MAX		500000 /*�����̸����ļ����*/
#define TRACE_FILE_TOTAL_MAX	8
#define TRACE_FILE_SIZE_MIN		20000
#define TRACE_FILE_TOTAL_MIN	2 /*����2��*/

#define AVERAGE_PROCESS_TRACE_SIZE_MAX	2000000 /*ƽ��ÿ���������洢�ռ�*/
#define SINGLE_PROCESS_TRACE_SIZE_MAX	(TRACE_FILE_SIZE_MAX*TRACE_FILE_TOTAL_MAX) /*�����������洢�ռ�*/
#define TRACE_FILE_SIZE_DEFAULT	50000 /*�����̸����ļ�ȱʡֵ*/

/** @class
 *  @brief ������Ϣ����
 *  @remarks ���й������������
 */
struct S_TraceSet
{
public:
	/** @brief ��־���𣺹ر�-����-����-��Ϣ-����-DEBUG */
	enum{LEVEL_DISABLED, LEVEL_ERROR, LEVEL_WARNING, LEVEL_INFO, LEVEL_VERBOSE, LEVEL_DEBUG, LEVEL_MAX};
	/** @brief ���Ŀ�꣺��-��׼���-��־�ļ�-ttyp0-ttyp1-ttyp2-ttyp3-ttyp4-ttyp5-ttyp6 */
	enum{WAY_NONE, WAY_STDOUT, WAY_LOGFILE, WAY_TTYP0, WAY_TTYP1, WAY_TTYP2, WAY_TTYP3, WAY_TTYP4, WAY_TTYP5, WAY_TTYP6, WAY_MAX};

	/** @brief �û����캯�� */
	S_TraceSet(unsigned char *ptitle=(unsigned char *)"unknow", int level=LEVEL_INFO, int way=WAY_STDOUT);
	/** @brief �������캯�� */
	S_TraceSet(const S_TraceSet &rTraceSet);
	~S_TraceSet();
	/** @brief ���ظ�ֵ���� */
	S_TraceSet & operator=(const S_TraceSet &rTraceSet);

	/** @brief ȡ���ü��� */
	unsigned char GetLevel(void)const{return m_level;}
	/** @brief ȡ���Ŀ�� */
	unsigned char GetWay(void)const{return m_way;}
	/** @brief ȡ����ļ���С */
	unsigned int  GetFileSize(void)const{return m_FileSiz;}
	/** @brief ȡ����ļ����� */
	unsigned char GetFileTotal(void)const{return m_FileTtl;}
	/** @brief ȡ���ñ��� */
	const unsigned char *GetTitle(void)const{return (m_title[0]=='\0')?NULL:m_title;}

	/** @brief ���ü���
	 *  @param[in] level ����[LEVEL_DISABLED, LEVEL_ERROR, LEVEL_WARNING, LEVEL_INFO, LEVEL_VERBOSE, LEVEL_DEBUG, LEVEL_MAX]
	 *  @return true
	 */
	bool SetLevel(int level);
	/** @brief �������Ŀ��
	 *  @param[in] way Ŀ��[WAY_NONE, WAY_STDOUT, WAY_LOGFILE, WAY_TTYP0, WAY_TTYP1...]
	 *  @return true
	 */
	bool SetWay(int way);
	/** @brief ��������ļ���С
	 *  @param[in] FileSiz �ļ���С
	 *  @return true
	 */
	bool SetFileSize(unsigned int FileSiz);
	/** @brief �����ļ�����
	 *  @param[in] FileTtl �ļ�����
	 *  @return true
	 */
	bool SetFileTotal(unsigned char FileTtl);
	/** @brief �������ñ���
	 *  @param[in] ptitle ����
	 *  @return true
	 */
	bool SetTitle(const unsigned char *ptitle);

public:
	unsigned char m_level; ///< ������Ϣ����
	unsigned char m_way; ///< ������Ϣ���Ŀ��
	unsigned char m_FileTtl; ///< ������Ϣ�ļ�����
	unsigned char m_Reserved; ///< ����
	unsigned int  m_FileSiz; ///< ������Ϣ�ļ�������С
	unsigned char m_title[12]; ///< ������Ϣ���ʱ���⣬�硰dyjc��
};


#endif //__MPROCESS_TRACESET_H__
