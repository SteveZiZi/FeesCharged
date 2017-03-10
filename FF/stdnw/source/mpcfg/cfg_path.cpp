/** @file
 *  @brief ϵͳ����Ŀ¼����Ҫ�ļ�������
 *  @author 
 *  @date 2011/11/07
 *  @version 0.1
 *  1�����ļ���Ŀ¼���ļ�������
 *  2��ʵ���ն�ϵͳ����ͱ�����Ŀ¼����һ��
 */
#include "basedef.h"
#include "cfg_path.h"
#include "cfg_usrp.h"

/** @brief ȡ��Ϣͨ��KEY */
const char *C_PATH::GetMessageFtok(char *fname)
{
	sprintf(fname, "%s%s", CFG_PATH_BASE, ".inited.flag");
	return fname;
}

/** @brief ȡϵͳ�����Ĺ����ڴ�ӳ���ļ��� */
const char *C_PATH::GetContextFile(char *fname)
{
	return GetRamDiskFile(CFG_FILE_CONTEXT, fname);
}

/** @brief ȡϵͳ״̬�����ڴ�ӳ���ļ��� */
const char *C_PATH::GetStatusFile(char *fname)
{
	sprintf(fname, "%s%s", CFG_PATH_DATA, CFG_FILE_STATUS);
	return fname;
}

/** @brief ȡonstart�ű� */
const char *C_PATH::GetScriptOnStart(char *fname)
{
	return GetScriptFile(CFG_FILE_SCRIPT_ON_START, fname);
}

/** @brief ȡonstop�ű� */
const char *C_PATH::GetScriptOnStop(char *fname)
{
	return GetScriptFile(CFG_FILE_SCRIPT_ON_STOP, fname);
}

/** @brief ȡDataReset�ű� */
const char *C_PATH::GetScriptDataReset(char *fname)
{
	return GetScriptFile(CFG_FILE_SCRIPT_DATA_RESET, fname);
}

/** @brief ȡFactoryReset�ű� */
const char *C_PATH::GetScriptFactoryReset(char *fname)
{
	return GetScriptFile(CFG_FILE_SCRIPT_FACT_RESET, fname);
}

/** @brief ȡAllReset�ű� */
const char *C_PATH::GetScriptAllReset(char *fname)
{
	return GetScriptFile(CFG_FILE_SCRIPT_ALL_RESET, fname);
}

/** @brief ȡParamReset�ű� */
const char *C_PATH::GetScriptParamReset(char *fname)
{
	return GetScriptFile(CFG_FILE_SCRIPT_PARAM_RESET, fname);
}

/** @brief ȡStatusReset�ű� */
const char *C_PATH::GetScriptStatusReset(char *fname)
{
	return GetScriptFile(CFG_FILE_SCRIPT_STAT_RESET, fname);
}

/** @brief ȡPppClear�ű� */
const char *C_PATH::GetScriptPppClear(char *fname)
{
	return GetScriptFile(CFG_FILE_SCRIPT_PPP_CLEAR, fname);
}

/** @brief ȡPppMuxd�ű� */
const char *C_PATH::GetScriptPppMuxd(char *fname)
{
	return GetScriptFile(CFG_FILE_SCRIPT_PPP_MUXD, fname);
}

/** @brief ȡPppGprsOn�ű� */
const char *C_PATH::GetScriptPppGprsOn(char *fname)
{
	return GetScriptFile(CFG_FILE_SCRIPT_PPP_GPRS_ON, fname);
}

/** @brief ȡPppCdmaOn�ű� */
const char *C_PATH::GetScriptPppCdmaOn(char *fname)
{
	return GetScriptFile(CFG_FILE_SCRIPT_PPP_CDMA_ON, fname);
}

/** @brief ȡPppGprsOff�ű� */
const char *C_PATH::GetScriptPppGprsOff(char *fname)
{
	return GetScriptFile(CFG_FILE_SCRIPT_PPP_GPRS_OFF, fname);
}

/** @brief ȡPppCdmaOff�ű� */
const char *C_PATH::GetScriptPppCdmaOff(char *fname)
{
	return GetScriptFile(CFG_FILE_SCRIPT_PPP_CDMA_OFF, fname);
}


/** @brief ȡ�����ļ�����·���� */
const char *C_PATH::GetProgramFile(char *fname)
{
	sprintf(fname, "%s%s", CFG_PATH_PROGRAM, CFG_FILE_PROGRAM_NAME);
	return fname;
}

/** @brief ȡ�������� */
const char *C_PATH::GetProgramName(char *fname)
{
	sprintf(fname, "%s", CFG_FILE_PROGRAM_NAME);
	return fname;
}

/** @brief ȡEXE���� */
const char *C_PATH::GetLinkExe(const char *psuffix, char *fpath)
{
	sprintf(fpath, "%s%s_%s", CFG_PATH_TMP, CFG_FILE_PROGRAM_NAME, psuffix);
	return fpath;
}

/** @brief ȡ��������⣨��·���� */
const char *C_PATH::GetZoneComponentLib(char *fname)
{
	sprintf(fname, "%s%s", CFG_PATH_PROGRAM, CFG_FILE_PROGRAM_ZONESO);
	return fname;
}

/** @brief ȡ�ű��ļ�����·���� */
const char *C_PATH::GetScriptFile(const char *fname, char *fpath)
{
	sprintf(fpath, "%s%s", CFG_PATH_SCRIPT, fname);
	return fpath;
}

/** @brief ȡϵͳ�����ļ�����·���� */
const char *C_PATH::GetSystemConfigFile(const char *fname, char *fpath)
{
	sprintf(fpath, "%s%s", CFG_PATH_SYSTEM, fname);
	return fpath;
}

/** @brief ȡ�����ļ�����·���� */
const char *C_PATH::GetSettingFile(const char *fname, char *fpath)
{
	sprintf(fpath, "%s%s", CFG_PATH_SET, fname);
	return fpath;
}

/** @brief ȡ�û������ļ�����·���� */
const char *C_PATH::GetUserConfigFile(const char *fname, char *fpath)
{
	sprintf(fpath, "%s%s", CFG_PATH_SET_USER, fname);
	return fpath;
}

/** @brief ȡȱʡ�����ļ�����·���� */
const char *C_PATH::GetDefaultConfigFile(const char *fname, char *fpath)
{
	sprintf(fpath, "%s%s", CFG_PATH_SET_DEFAULT, fname); 
	return fpath;
}

/** @brief ȡ�Ĵ��������ļ�����·���� */
const char *C_PATH::GetRegisterFile(const char *fname, char *fpath)
{
	sprintf(fpath, "%s%s", CFG_PATH_REGISTER, fname); 
	return fpath;
}

/** @brief ȡ�澯�ļ�����·���� */
const char *C_PATH::GetAlarmFile(const char *fname, char *fpath)
{
	sprintf(fpath, "%s%s", CFG_PATH_ALARM, fname);
	return fpath;
}

/** @brief ȡ�����ļ�����·���� */
const char *C_PATH::GetEnergyFile(const char *fname, char *fpath)
{
	sprintf(fpath, "%s%s", CFG_PATH_ENERGY, fname);
	return fpath;
}

/** @brief ȡRAMDISK�ļ�����·���� */
const char *C_PATH::GetRamDiskFile(const char *fname, char *fpath)
{
	sprintf(fpath, "%s%s", CFG_PATH_RAMDISK, fname);
	return fpath;
}

/** @brief ȡ�����ļ�����·���� */
const char *C_PATH::GetUpdateFile(const char *fname, char *fpath)
{
	sprintf(fpath, "%s%s", CFG_PATH_UPDATE, fname);
	return fpath;
}

/** @brief ȡ���������ļ� */
const char *C_PATH::GetFactoryTestFile(const char *fname, char *fpath)
{
	sprintf(fpath, "%s%s", CFG_PATH_FACTORY_TEST, fname);
	return fpath;
}

/** @brief ȡ���������ļ� */
const char *C_PATH::GetZoneFile(const char *fname, char *fpath)
{
	sprintf(fpath, "%s%s", CFG_PATH_ZONE, fname);
	return fpath;
}

/** @brief ȡ��Դ�ļ� */
const char *C_PATH::GetResourceFile(const char *fname, char *fpath)
{
	sprintf(fpath, "%s%s", CFG_PATH_RESOURCE, fname);
	return fpath;
}

/** @brief ȡ��־���·�� */
const char *C_PATH::GetLogBasePath(void)
{
	return CFG_PATH_LOG;
}

/** @brief ȡ�����ļ��� */
const char *C_PATH::GetTraceFileName(char *fname)
{
	strcpy(fname, CFG_FILE_INI_TRACE);
	return fname;
}

/** @brief ȡ������־�ļ�*/
const char *C_PATH::GetAutoUpdateKey(char *fname)
{
	return GetUpdateFile(CFG_FILE_AUTO_UPDATE_KEY, fname);
}


