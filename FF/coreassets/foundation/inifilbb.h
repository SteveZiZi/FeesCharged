/** @file
 *  @brief �����ļ������ඨ��
 *  @author 
 *  @date 2011/10/15
 *  @version 0.1
 *  1���ṩ�û������ļ�������ӿ�
 *  @see foundation/confuse
 */
#ifndef __FN_INI_FILE_BB_H__
#define __FN_INI_FILE_BB_H__
#include "confuse.h"
#include "basedef.h"
#include "basealgo.h"

/** @class
 *  @brief ��ʼ���ļ�����������
 *  @see foundation/confuse/confuse.h
 *  @remarks INI�ļ��������ô�����н���
 */
class C_IniBaseB
{
public:
	/** @fn C_IniBase::MakeValue
	 *  @brief ȡ������
	 *  @param[in] pkey ָ������
	 *  @param[in] pkeys ������������ָ��
	 *  @param[in] size �����С
	 *  @return >=0 ��Ч���� -1 ��Ч����
	 */
	static int MakeValue(const char *pkey, const char **pkeys, int size);
	/** @fn C_IniBase::MakeKey
	 *  @brief ȡָ������
	 *  @param[in] idx ������
	 *  @param[in] pkeys ������������ָ��
	 *  @param[in] size �����С
	 *  @return NULL ��Чָ�룻!NULL ������ָ��
	 */
	static const char *MakeKey(int idx, const char **pkeys, int size);
	/** @fn C_IniBase::GetBoolKey
	 *  @brief ȡbool����true/false
	 *  @param[in] fbool true/false
	 *  @return ָ��"true|false"��ָ��
	 */
	static const char *GetBoolKey(bool fbool);

	/** @fn C_IniBase::MakeKey
	 *  @brief ȡָ������
	 *  @param[in] val ָ��ֵ
	 *  @param[in] pCfgItem ������������ָ��
	 *  @param[in] size �����С
	 *  @return "unknow"��Key����//NULL ��Чָ�룻!NULL ������ָ��
	 */
	static const char *MakeKey(const S_CFGITEM *pCfgItem, S_WORD size, int val);
	/** @fn C_IniBase::MakeValue
	 *  @brief ȡ������
	 *  @param[in] pkey ָ������
	 *  @param[in] pCfgItem ������������ָ��
	 *  @param[in] size �����С
	 *  @return >=0 ��Чֵ -1 ��Чֵ
	 */
	static int MakeValue(const S_CFGITEM *pCfgItem, S_WORD size, const char *pkey);

	/** @brief ȡʮ�������� */
	static S_BYTE GetHex_BYTE(const char *pNumber);
	static S_WORD GetHex_WORD(const char *pNumber);
	static S_DWORD GetHex_DWORD(const char *pNumber);

	/** @fn C_IniBase::ParseIniFile
	 *  @brief �����ļ�����
	 *  @param[out] cfg:���ýṹ��
	 *  @param[in] iniFile:�ļ���(��·��)
	 *  @return 0:successful, -1:failed
	 */
	static int ParseIniFile(cfg_t* cfg, const char *iniFile);

	/** @fn C_IniBase::ParseCfgFile
	 *  @brief �����ļ�����
	 *  @param[out] cfg:���ýṹ��
	 *  @param[in] cfgFile:�ļ���(��·��)
	 *  @return 0:successful, -1:failed
	 */
	static int ParseCfgFile(cfg_t* cfg, const char *cfgFile);
	/** @fn C_IniBase::SaveCfgFile
	 *  @brief ���������ļ�
	 *  @param[in] strBuffer:������Ϣ��
	 *  @param[in] cfgFile:�ļ���(��·��)
	 *  @return 0:successful, -1:failed
	 */
	static int SaveCfgFile(const string &strBuffer, const char *cfgFile);

protected:
	static int _SaveCfgFile(const string &strBuffer, const char *cfgFile);

	/** @fn C_IniBase::CheckCfgBlock
	 *  @brief �����ļ�����
	 *  @param[out] cfg:���ýṹ��
	 *  @return 0:successful, -1:failed
	 */
	static int CheckCfgBlock(cfg_t* cfg);
};


#endif //__FN_INI_FILE_BB_H__

