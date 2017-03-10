/**
 *  @file
 *  @brief �������ݴ洢�ļ�
 *  @author 
 *  @date 2013-09-27
 *  @version v1.0
 *  @note
 */
#ifndef __BB_SIMPLE_DATA_FILE_H__
#define __BB_SIMPLE_DATA_FILE_H__
#include "basedef.h"
#include "fileop.h"

/** @brief �����ļ���ʽ
 * ��������ʽ
 * |��������ʶ|ʱ��|��¼����|����|
 * |---------8------|---8--|---2---------|---N --|
 */

#define CONST_DATA_LENGTH_MAX		128
#define CONST_AREA_LENGTH_MAX		8
#define CONST_TIME_LENGTH_MAX		8
#define CONST_RECORD_LENGTH_MAX		(8+8+2+CONST_DATA_LENGTH_MAX)

class C_SimpData
{
public:
	C_SimpData(void);
	C_SimpData(const char *pPath);
	~C_SimpData();

	/** @return >=0:��ȡ���ֽ���; <0:ʧ��*/
	int Load(S_DWORD DI, S_WORD MP, string &strData);
	/** @return >=0:д����ֽ���; <:ʧ��*/
	int Save(S_DWORD DI, S_WORD MP, const string &strData);

private:
	bool operator!(void){return !m_FileObj;}
	int NewFile(const char *pPath);
	const char *MakeFileName(S_DWORD DI, char *pPathBuffer);
	int MakeDataBuffer(S_WORD MP, const string &strData, string &strRecord);

private:
	char m_Path[CFG_PATH_FILE_MAX_LEN];
	C_FILE m_FileObj;
};


#endif //__BB_SIMPLE_DATA_FILE_H__


