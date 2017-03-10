/** @file
 *  @brief ϵͳ����Ŀ¼����Ҫ�ļ�������
 *  @author 
 *  @date 2011/11/07
 *  @version 0.1
 *  1�����ļ���Ŀ¼���ļ�������
 *  2��ʵ���ն�ϵͳ����ͱ�����Ŀ¼����һ��
 */
#ifndef __MP_CFG_PATH_H__
#define __MP_CFG_PATH_H__

/** @class
 *  @brief ·��ת����
 *  @note
 *  ��������Ӧ�ò��·�����ýӿڵ���ʽȡ����Ӧ�ò���Ҫ��д���ӿ�ʵ��
 */
class C_PATH
{
public:
	/** @brief ȡ��Ϣͨ��KEY */
	static const char *GetMessageFtok(char *fname);
	/** @brief ȡϵͳ�����Ĺ����ڴ�ӳ���ļ��� */
	static const char *GetContextFile(char *fname);
	/** @brief ȡϵͳ״̬�����ڴ�ӳ���ļ��� */
	static const char *GetStatusFile(char *fname);

	/** @brief ȡonstart�ű� */
	static const char *GetScriptOnStart(char *fname);
	/** @brief ȡonstop�ű� */
	static const char *GetScriptOnStop(char *fname);
	/** @brief ȡDataReset�ű� */
	static const char *GetScriptDataReset(char *fname);
	/** @brief ȡFactoryReset�ű� */
	static const char *GetScriptFactoryReset(char *fname);
	/** @brief ȡAllReset�ű� */
	static const char *GetScriptAllReset(char *fname);
	/** @brief ȡParamReset�ű� */
	static const char *GetScriptParamReset(char *fname);
	/** @brief ȡStatusReset�ű� */
	static const char *GetScriptStatusReset(char *fname);
	/** @brief ȡPppClear�ű� */
	static const char *GetScriptPppClear(char *fname);
	/** @brief ȡPppMuxd�ű� */
	static const char *GetScriptPppMuxd(char *fname);
	/** @brief ȡPppGprsOn�ű� */
	static const char *GetScriptPppGprsOn(char *fname);
	/** @brief ȡPppCdmaOn�ű� */
	static const char *GetScriptPppCdmaOn(char *fname);
	/** @brief ȡPppGprsOff�ű� */
	static const char *GetScriptPppGprsOff(char *fname);
	/** @brief ȡPppCdmaOff�ű� */
	static const char *GetScriptPppCdmaOff(char *fname);

	/** @brief ȡ�����ļ�����·���� */
	static const char *GetProgramFile(char *fname);
	/** @brief ȡ�������� */
	static const char *GetProgramName(char *fname);
	/** @brief ȡEXE���� */
	static const char *GetLinkExe(const char *psuffix, char *fpath);
	/** @brief ȡ��������⣨��·���� */
	static const char *GetZoneComponentLib(char *fname);

	/** @brief ȡ�ű��ļ�����·���� */
	static const char *GetScriptFile(const char *fname, char *fpath);
	/** @brief ȡϵͳ�����ļ�����·���� */
	static const char *GetSystemConfigFile(const char *fname, char *fpath);
	/** @brief ȡ�����ļ�����·���� */
	static const char *GetSettingFile(const char *fname, char *fpath);
	/** @brief ȡ�û������ļ�����·���� */
	static const char *GetUserConfigFile(const char *fname, char *fpath);
	/** @brief ȡȱʡ�����ļ�����·���� */
	static const char *GetDefaultConfigFile(const char *fname, char *fpath);
	/** @brief ȡ�Ĵ��������ļ�����·���� */
	static const char *GetRegisterFile(const char *fname, char *fpath);
	/** @brief ȡ�澯�ļ�����·���� */
	static const char *GetAlarmFile(const char *fname, char *fpath);
	/** @brief ȡ�����ļ�����·���� */
	static const char *GetEnergyFile(const char *fname, char *fpath);
	/** @brief ȡRAMDISK�ļ�����·���� */
	static const char *GetRamDiskFile(const char *fname, char *fpath);
	/** @brief ȡ�����ļ�����·���� */
	static const char *GetUpdateFile(const char *fname, char *fpath);
	/** @brief ȡ���������ļ� */
	static const char *GetFactoryTestFile(const char *fname, char *fpath);
	/** @brief ȡ���������ļ� */
	static const char *GetZoneFile(const char *fname, char *fpath);
	/** @brief ȡ��Դ�ļ� */
	static const char *GetResourceFile(const char *fname, char *fpath);
	/** @brief ȡ��־���·�� */
	static const char *GetLogBasePath(void);

	/** @brief ȡ�����ļ��� */
	static const char *GetTraceFileName(char *fname);
	/** @brief ȡ������־�ļ�*/
	static const char *GetAutoUpdateKey(char *fname);
	/** @brief ȡ�ն�������ʱ�ļ�*/
	static const char *GetTerminalUpdateFile(const char *fname, char *fpath);
};

#endif//__MP_CFG_PATH_H__

