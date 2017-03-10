/** @file
 *  @brief 文件操作集
 *  @author 
 *  @date 2011/11/07
 *  @version 0.1
 *  1、将文件操作封装成类
 *  2、本系统中所有文件操作必须通过此类进行，方便扩展和维护
 */
#ifndef __FND_FILEOP_H__
#define __FND_FILEOP_H__

#include <stdio.h>
#include "strings.h"

/** @class
 *  @brief 文件操作类定义
 *  @remarks 请不要直接使用C库的文件操作函数，采用本文件提供的文件类
 */
class C_FILE
{
public:
	C_FILE(void);
	C_FILE(const char *ppath, const char *mode = "wb");
	~C_FILE();

	/** @brief 打开文件
	 *  @param[in] ppth 文件路径
	 *  @param[in] mode 打开模式
	 */
	bool OpenFile(const char *ppath, const char *mode);
	/** @brief 文件对象就绪
	 *  @return true 成功；false 失败
	 */
	bool operator!(void);
	
	/** @brief 测试是否到文件尾
	 *  @return 文件结束返回非零值，否则为0
	 */
	int FileEof(void);

	/** @brief 文件行读取
	 *  @param[out] pBuffer 输出缓冲
	 *  @param[in] size 缓冲大小
	 *  @return 成功返回pBuffer，否则返回NULL
	 */
	char* FileGets(char *pBuffer, int size);
	/** @brief 文件读取
	 *  @param[out] pBuffer 输出缓冲
	 *  @param[in] size 缓冲大小
	 *  @return 读取的字节数
	 */
	size_t FileRead(void *pBuffer, size_t size);
	/** @brief 文件写入
	 *  @param[in] pBuffer 输入缓冲
	 *  @param[in] size 缓冲大小
	 *  @return 写入的字节数
	 */
	size_t FileWrite(const void *pBuffer, size_t size);
	/** @brief 文件定位
	 *  @param[in] offset 相对于seek的偏移
	 *  @param[in] seek SEEK_CUR | SEEK_END | SEEK_SET
	 *  @return 0 成功；!0 失败
	 */

	int FileSeek(long offset, int seek);
	/** @brief 获取文件当前读写位置
	 *  @return 文件指针距文件头偏移
	 */
	long FileTell(void);

public:
	/** @brief 创建目录
	 *  @param[in] ppath 目录路径
	 *  @return 0 成功；!0 失败
	 */
	static int MakeDir(const char *ppath);
	/** @brief 移除文件
	 *  @param[in] ppath 文件路径
	 *  @return 0 成功；!0 失败
	 */
	static int RemoveFile(const char *pfile);
	/** @brief 同步文件系统
	 *  @return 0 成功；!0 失败
	 */
	static int SyncFileSystem(void);
	/** @brief 查找指定后缀的文件集
	 *  @param[in] strPath 工作目录
	 *  @param[in] strSuffix 文件后缀
	 *  @param[out] strsFileName 文件输出缓冲
	 *  @return 查找成功的文件个数
	 */
	static S_WORD FindFileWithSuffix(const string &strPath, const string &strSuffix, strings &strsFileName);

private:
	FILE *_OpenFile(const char *ppath, const char *mode);
	int _CloseFile(FILE *pfp);

private:
	FILE *m_fp; ///< 文件指针
};

#endif //__FND_FILEOP_H__


