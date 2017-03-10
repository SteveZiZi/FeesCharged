/** @file
 *  @brief IPC-共享内存类
 *  @author 
 *  @date 2011/11/07
 *  @version 0.1
 *  1、底层共享内存通讯类，支持LINUX系统
 *  2、完成了WINDOWS下调试用的共享内存
 */
#ifndef __MP_SHOREMEM_H__
#define __MP_SHOREMEM_H__
#include "basedef.h"

/** @class
 *  @brief 内存签名类
 *  @remarks 在共享内存中进行比较，确认共享内存是否有效
 */
struct S_MEMSIGN
{
public:
	S_MEMSIGN(const char *pstr = NULL);
	S_MEMSIGN(S_MEMSIGN &r);
	~S_MEMSIGN();

	/** @brief 签名比较
	 *  @param[in] r 参考比较者
	 *  @return true 相同; false 不同
	 */
	bool operator==(const S_MEMSIGN &r);

public:
	char m_MemSign[20]; ///< 内存签名
};

//
//share memory
//
#ifdef WIN32
#include <windows.h>
#endif

/** @class
 *  @brief 共享内存类
 *  @remarks 采用基于POSIX标准的内存映射方式实现
 */
class C_SHAREMEM
{
public:
	C_SHAREMEM(void);
	C_SHAREMEM(const char *pFile, long size);
	~C_SHAREMEM();

	/** @brief 创建共享内存
	 *  @param[in] pFile 映射内存的文件
	 *  @param[in] size 内存大小
	 *  @return true 成功; false 失败
	 */
	bool Create(const char *pFile, long size);
	/** @brief 销毁共享内存
	 *  @return true 成功; false 失败
	 */
	bool Destroy(void);
	/** @brief 共享内存刷新到文件
	 *  @param[in] pstart 内存开始位置
	 *  @param[in] size 需要刷新的内存大小
	 *  @return true 成功; false 失败
	 */
	bool Save(void *pstart, long size);
	/** @brief 共享内存刷新到文件
	 *  @return true 成功; false 失败
	 */
	bool Save(void);

	/** @brief 取共享内存指针
	 *  @return NULL 无效指针；!NULL 有效指针
	 */
	void *GetShareMem(void){return m_pmem;}

private:
	/** @brief 确认映射共享内存的文件有效
	 *  @param[in] pFile 映射内存的文件
	 *  @param[in] size 内存大小
	 *  @return true 成功; false 失败
	 *  @remarks 如果文件不存在，则创建
	 */
	bool EnsureFile(const char *pFile, long size);

private:
	long  m_size; ///< 共享内存大小
	void *m_pmem; ///< 共享内存指针
#ifdef WIN32
	HANDLE m_hFile;
	HANDLE m_hFileMap;
#endif
};

#endif //__MPROCESS_SHOREMEM_H__


