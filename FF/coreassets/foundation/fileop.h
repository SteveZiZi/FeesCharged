/** @file
 *  @brief �ļ�������
 *  @author 
 *  @date 2011/11/07
 *  @version 0.1
 *  1�����ļ�������װ����
 *  2����ϵͳ�������ļ���������ͨ��������У�������չ��ά��
 */
#ifndef __FND_FILEOP_H__
#define __FND_FILEOP_H__

#include <stdio.h>
#include "strings.h"

/** @class
 *  @brief �ļ������ඨ��
 *  @remarks �벻Ҫֱ��ʹ��C����ļ��������������ñ��ļ��ṩ���ļ���
 */
class C_FILE
{
public:
	C_FILE(void);
	C_FILE(const char *ppath, const char *mode = "wb");
	~C_FILE();

	/** @brief ���ļ�
	 *  @param[in] ppth �ļ�·��
	 *  @param[in] mode ��ģʽ
	 */
	bool OpenFile(const char *ppath, const char *mode);
	/** @brief �ļ��������
	 *  @return true �ɹ���false ʧ��
	 */
	bool operator!(void);
	
	/** @brief �����Ƿ��ļ�β
	 *  @return �ļ��������ط���ֵ������Ϊ0
	 */
	int FileEof(void);

	/** @brief �ļ��ж�ȡ
	 *  @param[out] pBuffer �������
	 *  @param[in] size �����С
	 *  @return �ɹ�����pBuffer�����򷵻�NULL
	 */
	char* FileGets(char *pBuffer, int size);
	/** @brief �ļ���ȡ
	 *  @param[out] pBuffer �������
	 *  @param[in] size �����С
	 *  @return ��ȡ���ֽ���
	 */
	size_t FileRead(void *pBuffer, size_t size);
	/** @brief �ļ�д��
	 *  @param[in] pBuffer ���뻺��
	 *  @param[in] size �����С
	 *  @return д����ֽ���
	 */
	size_t FileWrite(const void *pBuffer, size_t size);
	/** @brief �ļ���λ
	 *  @param[in] offset �����seek��ƫ��
	 *  @param[in] seek SEEK_CUR | SEEK_END | SEEK_SET
	 *  @return 0 �ɹ���!0 ʧ��
	 */

	int FileSeek(long offset, int seek);
	/** @brief ��ȡ�ļ���ǰ��дλ��
	 *  @return �ļ�ָ����ļ�ͷƫ��
	 */
	long FileTell(void);

public:
	/** @brief ����Ŀ¼
	 *  @param[in] ppath Ŀ¼·��
	 *  @return 0 �ɹ���!0 ʧ��
	 */
	static int MakeDir(const char *ppath);
	/** @brief �Ƴ��ļ�
	 *  @param[in] ppath �ļ�·��
	 *  @return 0 �ɹ���!0 ʧ��
	 */
	static int RemoveFile(const char *pfile);
	/** @brief ͬ���ļ�ϵͳ
	 *  @return 0 �ɹ���!0 ʧ��
	 */
	static int SyncFileSystem(void);
	/** @brief ����ָ����׺���ļ���
	 *  @param[in] strPath ����Ŀ¼
	 *  @param[in] strSuffix �ļ���׺
	 *  @param[out] strsFileName �ļ��������
	 *  @return ���ҳɹ����ļ�����
	 */
	static S_WORD FindFileWithSuffix(const string &strPath, const string &strSuffix, strings &strsFileName);

private:
	FILE *_OpenFile(const char *ppath, const char *mode);
	int _CloseFile(FILE *pfp);

private:
	FILE *m_fp; ///< �ļ�ָ��
};

#endif //__FND_FILEOP_H__


