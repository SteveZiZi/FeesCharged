/** @file
 *  @brief 配置文件基础类定义
 *  @author 
 *  @date 2011/10/15
 *  @version 0.1
 *  1、提供用户配置文件基础类接口
 *  @see foundation/confuse
 */
#ifndef __MP_INI_FILEB_H__
#define __MP_INI_FILEB_H__
#include "inifilbb.h"

/** @class
 *  @brief 初始化文件分析基础类
 *  @see foundation/confuse/confuse.h
 *  @remarks INI文件分析采用此类进行解析
 */
class C_IniBase:public C_IniBaseB
{
public:
	/** @fn C_IniBase::ParseSetFile
	 *  @brief 配置文件分析
	 *  @param[out] cfg:配置结构体
	 *  @param[in] fname:文件名
	 *  @return 0:successful, -1:failed
	 *  @note 查找顺序:先找用户文件再找缺省文件
	 */
	static int ParseSetFile(cfg_t *cfg, const char *fname);
	/** @fn C_IniBase::ParseSystemFile
	 *  @brief 配置文件分析
	 *  @param[out] cfg:配置结构体
	 *  @param[in] fname:文件名
	 *  @return 0:successful, -1:failed
	 */
	static int ParseSystemFile(cfg_t *cfg, const char *fname);
	/** @fn C_IniBase::ParseRegisterFile
	 *  @brief 配置文件分析
	 *  @param[out] cfg:配置结构体
	 *  @param[in] fname:文件名
	 *  @return 0:successful, -1:failed
	 */
	static int ParseRegisterFile(cfg_t *cfg, const char *fname);
	/** @fn C_IniBase::ParseFactoryTestFile
	 *  @brief 配置文件分析
	 *  @param[out] cfg:配置结构体
	 *  @param[in] fname:文件名
	 *  @return 0:successful, -1:failed
	 */
	static int ParseFactoryTestFile(cfg_t *cfg, const char *fname);
	/** @fn C_IniBase::ParseZoneFile
	 *  @brief 配置文件分析
	 *  @param[out] cfg:配置结构体
	 *  @param[in] fname:文件名
	 *  @return 0:successful, -1:failed
	 */
	static int ParseZoneFile(cfg_t *cfg, const char *fname);
	/** @fn C_IniBase::SaveSetFile
	 *  @brief 保存配置文件
	 *  @param[in] strBuffer 配置信息串
	 *  @param[in] fname 文件名
	 *  @return 0:successful, -1:failed
	 */
	static int SaveSetFile(const string &strBuffer, const char *fname);
	/** @fn C_IniBase::SaveSystemFile
	 *  @brief 保存系统文件
	 *  @param[in] strBuffer 配置信息串
	 *  @param[in] fname 文件名
	 *  @return 0:successful, -1:failed
	 */
	static int SaveSystemFile(const string &strBuffer, const char *fname);
	/** @fn C_IniBase::SaveRegisterFile
	 *  @brief 保存系统文件
	 *  @param[in] strBuffer 配置信息串
	 *  @param[in] fname 文件名
	 *  @return 0:successful, -1:failed
	 */
	static int SaveRegisterFile(const string &strBuffer, const char *fname);
	/** @fn C_IniBase::SaveRegisterFile
	 *  @brief 保存系统文件
	 *  @param[in] strBuffer 配置信息串
	 *  @param[in] fname 文件名
	 *  @return 0:successful, -1:failed
	 */
	static int SaveFactoryTestFile(const string &strBuffer, const char *fname);
};


#endif //__MP_INI_FILEB_H__

