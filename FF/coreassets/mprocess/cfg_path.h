/** @file
 *  @brief 系统运行目录和重要文件配置类
 *  @author 
 *  @date 2011/11/07
 *  @version 0.1
 *  1、本文件是目录和文件配置类
 *  2、实际终端系统必需和本运行目录定义一致
 */
#ifndef __MP_CFG_PATH_H__
#define __MP_CFG_PATH_H__

/** @class
 *  @brief 路径转换类
 *  @note
 *  本函数将应用层的路径采用接口的形式取出，应用层需要重写本接口实现
 */
class C_PATH
{
public:
	/** @brief 取消息通道KEY */
	static const char *GetMessageFtok(char *fname);
	/** @brief 取系统上下文共享内存映射文件名 */
	static const char *GetContextFile(char *fname);
	/** @brief 取系统状态共享内存映射文件名 */
	static const char *GetStatusFile(char *fname);

	/** @brief 取onstart脚本 */
	static const char *GetScriptOnStart(char *fname);
	/** @brief 取onstop脚本 */
	static const char *GetScriptOnStop(char *fname);
	/** @brief 取DataReset脚本 */
	static const char *GetScriptDataReset(char *fname);
	/** @brief 取FactoryReset脚本 */
	static const char *GetScriptFactoryReset(char *fname);
	/** @brief 取AllReset脚本 */
	static const char *GetScriptAllReset(char *fname);
	/** @brief 取ParamReset脚本 */
	static const char *GetScriptParamReset(char *fname);
	/** @brief 取StatusReset脚本 */
	static const char *GetScriptStatusReset(char *fname);
	/** @brief 取PppClear脚本 */
	static const char *GetScriptPppClear(char *fname);
	/** @brief 取PppMuxd脚本 */
	static const char *GetScriptPppMuxd(char *fname);
	/** @brief 取PppGprsOn脚本 */
	static const char *GetScriptPppGprsOn(char *fname);
	/** @brief 取PppCdmaOn脚本 */
	static const char *GetScriptPppCdmaOn(char *fname);
	/** @brief 取PppGprsOff脚本 */
	static const char *GetScriptPppGprsOff(char *fname);
	/** @brief 取PppCdmaOff脚本 */
	static const char *GetScriptPppCdmaOff(char *fname);

	/** @brief 取程序文件（带路径） */
	static const char *GetProgramFile(char *fname);
	/** @brief 取程序名称 */
	static const char *GetProgramName(char *fname);
	/** @brief 取EXE名称 */
	static const char *GetLinkExe(const char *psuffix, char *fpath);
	/** @brief 取区域组件库（带路径） */
	static const char *GetZoneComponentLib(char *fname);

	/** @brief 取脚本文件（带路径） */
	static const char *GetScriptFile(const char *fname, char *fpath);
	/** @brief 取系统配置文件（带路径） */
	static const char *GetSystemConfigFile(const char *fname, char *fpath);
	/** @brief 取配置文件（带路径） */
	static const char *GetSettingFile(const char *fname, char *fpath);
	/** @brief 取用户配置文件（带路径） */
	static const char *GetUserConfigFile(const char *fname, char *fpath);
	/** @brief 取缺省配置文件（带路径） */
	static const char *GetDefaultConfigFile(const char *fname, char *fpath);
	/** @brief 取寄存器配置文件（带路径） */
	static const char *GetRegisterFile(const char *fname, char *fpath);
	/** @brief 取告警文件（带路径） */
	static const char *GetAlarmFile(const char *fname, char *fpath);
	/** @brief 取能量文件（带路径） */
	static const char *GetEnergyFile(const char *fname, char *fpath);
	/** @brief 取RAMDISK文件（带路径） */
	static const char *GetRamDiskFile(const char *fname, char *fpath);
	/** @brief 取升级文件（带路径） */
	static const char *GetUpdateFile(const char *fname, char *fpath);
	/** @brief 取生产测试文件 */
	static const char *GetFactoryTestFile(const char *fname, char *fpath);
	/** @brief 取区域配置文件 */
	static const char *GetZoneFile(const char *fname, char *fpath);
	/** @brief 取资源文件 */
	static const char *GetResourceFile(const char *fname, char *fpath);
	/** @brief 取日志输出路径 */
	static const char *GetLogBasePath(void);

	/** @brief 取配置文件名 */
	static const char *GetTraceFileName(char *fname);
	/** @brief 取升级标志文件*/
	static const char *GetAutoUpdateKey(char *fname);
	/** @brief 取终端升级临时文件*/
	static const char *GetTerminalUpdateFile(const char *fname, char *fpath);
};

#endif//__MP_CFG_PATH_H__

