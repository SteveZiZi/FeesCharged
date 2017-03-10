/**
 *  @file
 *  @brief ����������Ϣ������
 *  @author 
 *  @date 2013-02-25
 *  @version v1.0
 *  @note ���ɽ��̺ͱ�Ľ��̵�IPCͨ��
 */
#ifndef __BASEB_JLCY_MSG_H__
#define __BASEB_JLCY_MSG_H__
#include "basedef.h"

/** @note ��Ϣ��ʽ
 *  send: FrameLen(2B)+DI(4B)+DI(4B)+...+DI(4B)
 *  recv: FrameLen(2B)+DI(4B)+DataLen(1B)+Data(NB)+...+DI(4B)+DataLen(1B)+Data(NB)
 *  ע�����ݴ��ݰ�С�˷�ʽ
 */

/** @brief �������ݸ�ʽ */
struct S_JlcyData
{
	S_DWORD m_DI; ///< ����ID����0x05060101 ��DLT645-2007��Լ���
	string m_Data; ///< ��������
};
struct S_JlcyDatas:public vector<S_JlcyData>
{
};

/** @brief ����������Ϣ������*/
class C_JlcyMsg
{
public:
	C_JlcyMsg(void);
	~C_JlcyMsg();

public:
	/** @brief ��ȡ����֡
	 *  @param[in] strBuffer �����
	 *  @param[out] strCmd ֡�������
	 *  @return <0: ����[����ֵ�ǳ�����ֽ���]; =0: ��֡; >0: �ɹ�
	 */
	int ExtractCmd(const string &strBuffer, string &strCmd);

	/** @brief ��֯����
	 *  @param[in] JlcyDatas ����������
	 *  @param[out] strCmd ֡�������
	 *  @return <=0: ����; >0: ��������
	 *  @note �ǽ��ɽ��̵���
	 */
	int Master_MakeCmd(const S_JlcyDatas &JlcyDatas, string &strCmd);
	/** @brief ��������
	 *  @param[in] strCmd ֡�������
	 *  @param[out] JlcyDatas ����������
	 *  @return <=0: ����; >0: ���������
	 *  @note �ǽ��ɽ��̵���
	 */
	int Master_ParseCmd(const string &strCmd, S_JlcyDatas &JlcyDatas);
	/** @brief ��֯����
	 *  @param[in] JlcyDatas ����������
	 *  @param[out] strCmd ֡�������
	 *  @return <=0: ����; >0: ��������
	 *  @note ���ɽ��̵���
	 */
	int Slave_MakeCmd(const S_JlcyDatas &JlcyDatas, string &strCmd);
	/** @brief ��������
	 *  @param[in] strCmd ֡�������
	 *  @param[out] JlcyDatas ����������
	 *  @return <=0: ����; >0: ���������
	 *  @note ���ɽ��̵���
	 */
	int Slave_ParseCmd(const string &strCmd, S_JlcyDatas &JlcyDatas);
};

#endif //__BASEB_JLCY_MSG_H__

