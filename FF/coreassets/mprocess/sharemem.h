/** @file
 *  @brief IPC-�����ڴ���
 *  @author 
 *  @date 2011/11/07
 *  @version 0.1
 *  1���ײ㹲���ڴ�ͨѶ�֧࣬��LINUXϵͳ
 *  2�������WINDOWS�µ����õĹ����ڴ�
 */
#ifndef __MP_SHOREMEM_H__
#define __MP_SHOREMEM_H__
#include "basedef.h"

/** @class
 *  @brief �ڴ�ǩ����
 *  @remarks �ڹ����ڴ��н��бȽϣ�ȷ�Ϲ����ڴ��Ƿ���Ч
 */
struct S_MEMSIGN
{
public:
	S_MEMSIGN(const char *pstr = NULL);
	S_MEMSIGN(S_MEMSIGN &r);
	~S_MEMSIGN();

	/** @brief ǩ���Ƚ�
	 *  @param[in] r �ο��Ƚ���
	 *  @return true ��ͬ; false ��ͬ
	 */
	bool operator==(const S_MEMSIGN &r);

public:
	char m_MemSign[20]; ///< �ڴ�ǩ��
};

//
//share memory
//
#ifdef WIN32
#include <windows.h>
#endif

/** @class
 *  @brief �����ڴ���
 *  @remarks ���û���POSIX��׼���ڴ�ӳ�䷽ʽʵ��
 */
class C_SHAREMEM
{
public:
	C_SHAREMEM(void);
	C_SHAREMEM(const char *pFile, long size);
	~C_SHAREMEM();

	/** @brief ���������ڴ�
	 *  @param[in] pFile ӳ���ڴ���ļ�
	 *  @param[in] size �ڴ��С
	 *  @return true �ɹ�; false ʧ��
	 */
	bool Create(const char *pFile, long size);
	/** @brief ���ٹ����ڴ�
	 *  @return true �ɹ�; false ʧ��
	 */
	bool Destroy(void);
	/** @brief �����ڴ�ˢ�µ��ļ�
	 *  @param[in] pstart �ڴ濪ʼλ��
	 *  @param[in] size ��Ҫˢ�µ��ڴ��С
	 *  @return true �ɹ�; false ʧ��
	 */
	bool Save(void *pstart, long size);
	/** @brief �����ڴ�ˢ�µ��ļ�
	 *  @return true �ɹ�; false ʧ��
	 */
	bool Save(void);

	/** @brief ȡ�����ڴ�ָ��
	 *  @return NULL ��Чָ�룻!NULL ��Чָ��
	 */
	void *GetShareMem(void){return m_pmem;}

private:
	/** @brief ȷ��ӳ�乲���ڴ���ļ���Ч
	 *  @param[in] pFile ӳ���ڴ���ļ�
	 *  @param[in] size �ڴ��С
	 *  @return true �ɹ�; false ʧ��
	 *  @remarks ����ļ������ڣ��򴴽�
	 */
	bool EnsureFile(const char *pFile, long size);

private:
	long  m_size; ///< �����ڴ��С
	void *m_pmem; ///< �����ڴ�ָ��
#ifdef WIN32
	HANDLE m_hFile;
	HANDLE m_hFileMap;
#endif
};

#endif //__MPROCESS_SHOREMEM_H__


