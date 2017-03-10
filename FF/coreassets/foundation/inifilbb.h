/** @file
 *  @brief 配置文件基础类定义
 *  @author 
 *  @date 2011/10/15
 *  @version 0.1
 *  1、提供用户配置文件基础类接口
 *  @see foundation/confuse
 */
#ifndef __FN_INI_FILE_BB_H__
#define __FN_INI_FILE_BB_H__
#include "confuse.h"
#include "basedef.h"
#include "basealgo.h"

/** @class
 *  @brief 初始化文件分析基础类
 *  @see foundation/confuse/confuse.h
 *  @remarks INI文件分析采用此类进行解析
 */
class C_IniBaseB
{
public:
	/** @fn C_IniBase::MakeValue
	 *  @brief 取索引号
	 *  @param[in] pkey 指定名称
	 *  @param[in] pkeys 索引名称数组指针
	 *  @param[in] size 数组大小
	 *  @return >=0 有效索引 -1 无效索引
	 */
	static int MakeValue(const char *pkey, const char **pkeys, int size);
	/** @fn C_IniBase::MakeKey
	 *  @brief 取指定名称
	 *  @param[in] idx 索引号
	 *  @param[in] pkeys 索引名称数组指针
	 *  @param[in] size 数组大小
	 *  @return NULL 无效指针；!NULL 数组项指针
	 */
	static const char *MakeKey(int idx, const char **pkeys, int size);
	/** @fn C_IniBase::GetBoolKey
	 *  @brief 取bool串：true/false
	 *  @param[in] fbool true/false
	 *  @return 指向"true|false"的指针
	 */
	static const char *GetBoolKey(bool fbool);

	/** @fn C_IniBase::MakeKey
	 *  @brief 取指定名称
	 *  @param[in] val 指定值
	 *  @param[in] pCfgItem 索引名称数组指针
	 *  @param[in] size 数组大小
	 *  @return "unknow"或Key名称//NULL 无效指针；!NULL 数组项指针
	 */
	static const char *MakeKey(const S_CFGITEM *pCfgItem, S_WORD size, int val);
	/** @fn C_IniBase::MakeValue
	 *  @brief 取索引号
	 *  @param[in] pkey 指定名称
	 *  @param[in] pCfgItem 索引名称数组指针
	 *  @param[in] size 数组大小
	 *  @return >=0 有效值 -1 无效值
	 */
	static int MakeValue(const S_CFGITEM *pCfgItem, S_WORD size, const char *pkey);

	/** @brief 取十六进制数 */
	static S_BYTE GetHex_BYTE(const char *pNumber);
	static S_WORD GetHex_WORD(const char *pNumber);
	static S_DWORD GetHex_DWORD(const char *pNumber);

	/** @fn C_IniBase::ParseIniFile
	 *  @brief 配置文件分析
	 *  @param[out] cfg:配置结构体
	 *  @param[in] iniFile:文件名(带路径)
	 *  @return 0:successful, -1:failed
	 */
	static int ParseIniFile(cfg_t* cfg, const char *iniFile);

	/** @fn C_IniBase::ParseCfgFile
	 *  @brief 配置文件分析
	 *  @param[out] cfg:配置结构体
	 *  @param[in] cfgFile:文件名(带路径)
	 *  @return 0:successful, -1:failed
	 */
	static int ParseCfgFile(cfg_t* cfg, const char *cfgFile);
	/** @fn C_IniBase::SaveCfgFile
	 *  @brief 保存配置文件
	 *  @param[in] strBuffer:配置信息串
	 *  @param[in] cfgFile:文件名(带路径)
	 *  @return 0:successful, -1:failed
	 */
	static int SaveCfgFile(const string &strBuffer, const char *cfgFile);

protected:
	static int _SaveCfgFile(const string &strBuffer, const char *cfgFile);

	/** @fn C_IniBase::CheckCfgBlock
	 *  @brief 配置文件分析
	 *  @param[out] cfg:配置结构体
	 *  @return 0:successful, -1:failed
	 */
	static int CheckCfgBlock(cfg_t* cfg);
};


#endif //__FN_INI_FILE_BB_H__

