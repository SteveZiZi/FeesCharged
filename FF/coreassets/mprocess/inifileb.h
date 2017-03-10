/** @file
 *  @brief �����ļ������ඨ��
 *  @author 
 *  @date 2011/10/15
 *  @version 0.1
 *  1���ṩ�û������ļ�������ӿ�
 *  @see foundation/confuse
 */
#ifndef __MP_INI_FILEB_H__
#define __MP_INI_FILEB_H__
#include "inifilbb.h"

/** @class
 *  @brief ��ʼ���ļ�����������
 *  @see foundation/confuse/confuse.h
 *  @remarks INI�ļ��������ô�����н���
 */
class C_IniBase:public C_IniBaseB
{
public:
	/** @fn C_IniBase::ParseSetFile
	 *  @brief �����ļ�����
	 *  @param[out] cfg:���ýṹ��
	 *  @param[in] fname:�ļ���
	 *  @return 0:successful, -1:failed
	 *  @note ����˳��:�����û��ļ�����ȱʡ�ļ�
	 */
	static int ParseSetFile(cfg_t *cfg, const char *fname);
	/** @fn C_IniBase::ParseSystemFile
	 *  @brief �����ļ�����
	 *  @param[out] cfg:���ýṹ��
	 *  @param[in] fname:�ļ���
	 *  @return 0:successful, -1:failed
	 */
	static int ParseSystemFile(cfg_t *cfg, const char *fname);
	/** @fn C_IniBase::ParseRegisterFile
	 *  @brief �����ļ�����
	 *  @param[out] cfg:���ýṹ��
	 *  @param[in] fname:�ļ���
	 *  @return 0:successful, -1:failed
	 */
	static int ParseRegisterFile(cfg_t *cfg, const char *fname);
	/** @fn C_IniBase::ParseFactoryTestFile
	 *  @brief �����ļ�����
	 *  @param[out] cfg:���ýṹ��
	 *  @param[in] fname:�ļ���
	 *  @return 0:successful, -1:failed
	 */
	static int ParseFactoryTestFile(cfg_t *cfg, const char *fname);
	/** @fn C_IniBase::ParseZoneFile
	 *  @brief �����ļ�����
	 *  @param[out] cfg:���ýṹ��
	 *  @param[in] fname:�ļ���
	 *  @return 0:successful, -1:failed
	 */
	static int ParseZoneFile(cfg_t *cfg, const char *fname);
	/** @fn C_IniBase::SaveSetFile
	 *  @brief ���������ļ�
	 *  @param[in] strBuffer ������Ϣ��
	 *  @param[in] fname �ļ���
	 *  @return 0:successful, -1:failed
	 */
	static int SaveSetFile(const string &strBuffer, const char *fname);
	/** @fn C_IniBase::SaveSystemFile
	 *  @brief ����ϵͳ�ļ�
	 *  @param[in] strBuffer ������Ϣ��
	 *  @param[in] fname �ļ���
	 *  @return 0:successful, -1:failed
	 */
	static int SaveSystemFile(const string &strBuffer, const char *fname);
	/** @fn C_IniBase::SaveRegisterFile
	 *  @brief ����ϵͳ�ļ�
	 *  @param[in] strBuffer ������Ϣ��
	 *  @param[in] fname �ļ���
	 *  @return 0:successful, -1:failed
	 */
	static int SaveRegisterFile(const string &strBuffer, const char *fname);
	/** @fn C_IniBase::SaveRegisterFile
	 *  @brief ����ϵͳ�ļ�
	 *  @param[in] strBuffer ������Ϣ��
	 *  @param[in] fname �ļ���
	 *  @return 0:successful, -1:failed
	 */
	static int SaveFactoryTestFile(const string &strBuffer, const char *fname);
};


#endif //__MP_INI_FILEB_H__

