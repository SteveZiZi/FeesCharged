/** @file
 *  @brief 系统运行目录和重要文件配置类
 *  @author 
 *  @date 2011/11/07
 *  @version 0.1
 *  1、本文件是目录和文件配置类
 *  2、实际终端系统必需和本运行目录定义一致
 */
#include "basedef.h"
#include "cfg_path.h"
#include "cfg_usrp.h"

/** @brief 取消息通道KEY */
const char *C_PATH::GetMessageFtok(char *fname)
{
	sprintf(fname, "%s%s", CFG_PATH_BASE, ".inited.flag");
	return fname;
}

/** @brief 取系统上下文共享内存映射文件名 */
const char *C_PATH::GetContextFile(char *fname)
{
	return GetRamDiskFile(CFG_FILE_CONTEXT, fname);
}

/** @brief 取系统状态共享内存映射文件名 */
const char *C_PATH::GetStatusFile(char *fname)
{
	sprintf(fname, "%s%s", CFG_PATH_DATA, CFG_FILE_STATUS);
	return fname;
}

/** @brief 取onstart脚本 */
const char *C_PATH::GetScriptOnStart(char *fname)
{
	return GetScriptFile(CFG_FILE_SCRIPT_ON_START, fname);
}

/** @brief 取onstop脚本 */
const char *C_PATH::GetScriptOnStop(char *fname)
{
	return GetScriptFile(CFG_FILE_SCRIPT_ON_STOP, fname);
}

/** @brief 取DataReset脚本 */
const char *C_PATH::GetScriptDataReset(char *fname)
{
	return GetScriptFile(CFG_FILE_SCRIPT_DATA_RESET, fname);
}

/** @brief 取FactoryReset脚本 */
const char *C_PATH::GetScriptFactoryReset(char *fname)
{
	return GetScriptFile(CFG_FILE_SCRIPT_FACT_RESET, fname);
}

/** @brief 取AllReset脚本 */
const char *C_PATH::GetScriptAllReset(char *fname)
{
	return GetScriptFile(CFG_FILE_SCRIPT_ALL_RESET, fname);
}

/** @brief 取ParamReset脚本 */
const char *C_PATH::GetScriptParamReset(char *fname)
{
	return GetScriptFile(CFG_FILE_SCRIPT_PARAM_RESET, fname);
}

/** @brief 取StatusReset脚本 */
const char *C_PATH::GetScriptStatusReset(char *fname)
{
	return GetScriptFile(CFG_FILE_SCRIPT_STAT_RESET, fname);
}

/** @brief 取PppClear脚本 */
const char *C_PATH::GetScriptPppClear(char *fname)
{
	return GetScriptFile(CFG_FILE_SCRIPT_PPP_CLEAR, fname);
}

/** @brief 取PppMuxd脚本 */
const char *C_PATH::GetScriptPppMuxd(char *fname)
{
	return GetScriptFile(CFG_FILE_SCRIPT_PPP_MUXD, fname);
}

/** @brief 取PppGprsOn脚本 */
const char *C_PATH::GetScriptPppGprsOn(char *fname)
{
	return GetScriptFile(CFG_FILE_SCRIPT_PPP_GPRS_ON, fname);
}

/** @brief 取PppCdmaOn脚本 */
const char *C_PATH::GetScriptPppCdmaOn(char *fname)
{
	return GetScriptFile(CFG_FILE_SCRIPT_PPP_CDMA_ON, fname);
}

/** @brief 取PppGprsOff脚本 */
const char *C_PATH::GetScriptPppGprsOff(char *fname)
{
	return GetScriptFile(CFG_FILE_SCRIPT_PPP_GPRS_OFF, fname);
}

/** @brief 取PppCdmaOff脚本 */
const char *C_PATH::GetScriptPppCdmaOff(char *fname)
{
	return GetScriptFile(CFG_FILE_SCRIPT_PPP_CDMA_OFF, fname);
}


/** @brief 取程序文件（带路径） */
const char *C_PATH::GetProgramFile(char *fname)
{
	sprintf(fname, "%s%s", CFG_PATH_PROGRAM, CFG_FILE_PROGRAM_NAME);
	return fname;
}

/** @brief 取程序名称 */
const char *C_PATH::GetProgramName(char *fname)
{
	sprintf(fname, "%s", CFG_FILE_PROGRAM_NAME);
	return fname;
}

/** @brief 取EXE名称 */
const char *C_PATH::GetLinkExe(const char *psuffix, char *fpath)
{
	sprintf(fpath, "%s%s_%s", CFG_PATH_TMP, CFG_FILE_PROGRAM_NAME, psuffix);
	return fpath;
}

/** @brief 取区域组件库（带路径） */
const char *C_PATH::GetZoneComponentLib(char *fname)
{
	sprintf(fname, "%s%s", CFG_PATH_PROGRAM, CFG_FILE_PROGRAM_ZONESO);
	return fname;
}

/** @brief 取脚本文件（带路径） */
const char *C_PATH::GetScriptFile(const char *fname, char *fpath)
{
	sprintf(fpath, "%s%s", CFG_PATH_SCRIPT, fname);
	return fpath;
}

/** @brief 取系统配置文件（带路径） */
const char *C_PATH::GetSystemConfigFile(const char *fname, char *fpath)
{
	sprintf(fpath, "%s%s", CFG_PATH_SYSTEM, fname);
	return fpath;
}

/** @brief 取配置文件（带路径） */
const char *C_PATH::GetSettingFile(const char *fname, char *fpath)
{
	sprintf(fpath, "%s%s", CFG_PATH_SET, fname);
	return fpath;
}

/** @brief 取用户配置文件（带路径） */
const char *C_PATH::GetUserConfigFile(const char *fname, char *fpath)
{
	sprintf(fpath, "%s%s", CFG_PATH_SET_USER, fname);
	return fpath;
}

/** @brief 取缺省配置文件（带路径） */
const char *C_PATH::GetDefaultConfigFile(const char *fname, char *fpath)
{
	sprintf(fpath, "%s%s", CFG_PATH_SET_DEFAULT, fname); 
	return fpath;
}

/** @brief 取寄存器配置文件（带路径） */
const char *C_PATH::GetRegisterFile(const char *fname, char *fpath)
{
	sprintf(fpath, "%s%s", CFG_PATH_REGISTER, fname); 
	return fpath;
}

/** @brief 取告警文件（带路径） */
const char *C_PATH::GetAlarmFile(const char *fname, char *fpath)
{
	sprintf(fpath, "%s%s", CFG_PATH_ALARM, fname);
	return fpath;
}

/** @brief 取能量文件（带路径） */
const char *C_PATH::GetEnergyFile(const char *fname, char *fpath)
{
	sprintf(fpath, "%s%s", CFG_PATH_ENERGY, fname);
	return fpath;
}

/** @brief 取RAMDISK文件（带路径） */
const char *C_PATH::GetRamDiskFile(const char *fname, char *fpath)
{
	sprintf(fpath, "%s%s", CFG_PATH_RAMDISK, fname);
	return fpath;
}

/** @brief 取升级文件（带路径） */
const char *C_PATH::GetUpdateFile(const char *fname, char *fpath)
{
	sprintf(fpath, "%s%s", CFG_PATH_UPDATE, fname);
	return fpath;
}

/** @brief 取生产测试文件 */
const char *C_PATH::GetFactoryTestFile(const char *fname, char *fpath)
{
	sprintf(fpath, "%s%s", CFG_PATH_FACTORY_TEST, fname);
	return fpath;
}

/** @brief 取区域配置文件 */
const char *C_PATH::GetZoneFile(const char *fname, char *fpath)
{
	sprintf(fpath, "%s%s", CFG_PATH_ZONE, fname);
	return fpath;
}

/** @brief 取资源文件 */
const char *C_PATH::GetResourceFile(const char *fname, char *fpath)
{
	sprintf(fpath, "%s%s", CFG_PATH_RESOURCE, fname);
	return fpath;
}

/** @brief 取日志输出路径 */
const char *C_PATH::GetLogBasePath(void)
{
	return CFG_PATH_LOG;
}

/** @brief 取配置文件名 */
const char *C_PATH::GetTraceFileName(char *fname)
{
	strcpy(fname, CFG_FILE_INI_TRACE);
	return fname;
}

/** @brief 取升级标志文件*/
const char *C_PATH::GetAutoUpdateKey(char *fname)
{
	return GetUpdateFile(CFG_FILE_AUTO_UPDATE_KEY, fname);
}


