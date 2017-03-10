/** @file
 *  @brief IPC-��Ϣ�ײ���
 *  @author  Steven Wan
 *  @date 2015/11/07
 *  @version 0.1
 *  1���ײ���ϢͨѶ�֧࣬��LINUXϵͳ
 *  2���û���Ҫ����ftok��fname
 */
#ifndef __MP_MESSAGE_H__
#define __MP_MESSAGE_H__
#include "../inc/basedef.h"
#include "strings.h"

/** @class
 *  @brief IPC��Ϣͨ��
 *  @remarks ����LINUX msgsnd msgrcv�Ⱥ���
 */
class C_MESSAGE
{
public:
	enum{TEXT_BUF_MAX=2048};

	C_MESSAGE(char QFlag = 'M');
	~C_MESSAGE();

public:
	/** @brief ������Ϣ
	 *  @param[in] MsgT ��Ϣ����
	 *  @param[in] msg ��Ϣ
	 *  @return <0 ʧ�ܣ�>=0 �ɹ�
	 */
	int SendMsg(int MsgT, const string &msg);
	/** @brief ������Ϣ
	 *  @param[in] MsgT ��Ϣ����
	 *  @param[out] msg ��Ϣ�������
	 *  @return <0 ʧ�ܣ�>=0 �ɹ�
	 */
	int RecvMsg(int MsgT, string &msg);
	/** @brief ������Ϣͨ��
	 *  @param[in] MsgT ��Ϣ����
	 *  @return <0 ʧ�ܣ�>=0 �ɹ�
	 *  @remarks MsgT>0 ����ָ����Ϣ��MsgT=0 ����������Ϣ
	 */
	int ClearMsg(int MsgT);
	/** @brief ������Ϣ
	 *  @param[in] MsgT ��Ϣ����
	 *  @return <0 ʧ�ܣ�>=0 �ɹ�
	 */
	int PutMsg(int MsgT, const string &msg);

	/** @brief ��ȡ��Ϣ
	 *  @param[in] MsgT ��Ϣ����
	 *  @param[out] msg ��Ϣ�������
	 *  @return <0 ʧ�ܣ�>=0 �ɹ�
	 */
	int GetMsg(int MsgT, string &msg);
	
private:
	strings m_MsgBuffer; ///< ��Ϣ����
	int m_MsgQID; ///< ��Ϣͨ��������
};

#endif //__MPROCESS_MESSAGE_H__


