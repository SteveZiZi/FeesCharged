/** @file
 *  @brief IPC-��Ϣ�ײ���
 *  @author  Steven Wan
 *  @date 2015/11/07
 *  @version 0.1
 *  1���ײ���ϢͨѶ�֧࣬��LINUXϵͳ
 *  2���û���Ҫ����ftok��fname
 */
#include "message.h"


#ifndef WIN32
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#endif

C_MESSAGE::C_MESSAGE(char QFlag)
{
	m_MsgQID = -1;
//	char fname[CFG_PATH_FILE_MAX_LEN];
//	C_PATH::GetMessageFtok(fname);
#ifndef WIN32
	key_t key = ftok("/mnt/.inited.flag", QFlag);
	if (key == -1)
		printf("ftok error:%s", "mnt/.inited.flag");
	else
		m_MsgQID = msgget(key,IPC_CREAT|00666);
	if(m_MsgQID == -1)
		printf("Create message queue error: %s.\n", strerror(errno));
#endif
}

C_MESSAGE::~C_MESSAGE()
{
#ifndef WIN32
	if (m_MsgQID != -1)
	{//��ֹ�����˳�ʱ����Ϣͨ���Ƴ�����Ϊ��Ϣͨ����ȫ�ֹ���ġ�
//		if (msgctl(m_MsgQID,IPC_RMID, NULL) == -1)
//			printf("Unlink message queue error:%s.\n", strerror(errno));
		m_MsgQID=-1;
	}
#endif
}

#ifndef WIN32
struct S_MSGBUF
{
	int mtype;
	char mtext[C_MESSAGE::TEXT_BUF_MAX];
};
#endif

/** @brief ������Ϣ
 *  @param[in] MsgT ��Ϣ����
 *  @param[in] msg ��Ϣ
 *  @return <0 ʧ�ܣ�>=0 �ɹ�
 */
int C_MESSAGE::SendMsg(int MsgT, const string &msg)
{
	int err = -1;

#ifndef WIN32
	struct S_MSGBUF MsgBuf;

	if (m_MsgQID == -1 || msg.size() >= sizeof(MsgBuf.mtext))
	{
		if (msg.size() >= sizeof(MsgBuf.mtext))
			printf("Message is too big!!!\n");
		return err;
	}
	MsgBuf.mtype = MsgT;
	memset(MsgBuf.mtext, 0x00, sizeof(MsgBuf.mtext));
	memcpy(MsgBuf.mtext, msg.data(), msg.size());

	if ((err = msgsnd(m_MsgQID, &MsgBuf, msg.size(),IPC_NOWAIT)) == -1)
	{
		printf("Message send failed,error:%s!\n", strerror(errno));
		ClearMsg(MsgT);
		if ((err = msgsnd(m_MsgQID, &MsgBuf, msg.size(),IPC_NOWAIT)) == -1)
		{
			ClearMsg(0);
			err = msgsnd(m_MsgQID, &MsgBuf, msg.size(),IPC_NOWAIT);
		}
	}

#endif
	return err;
}

/** @brief ������Ϣ
 *  @param[in] MsgT ��Ϣ����
 *  @param[out] msg ��Ϣ�������
 *  @return <0 ʧ�ܣ�>=0 �ɹ�
 */
int C_MESSAGE::RecvMsg(int MsgT, string &msg)
{
	int err = -1;

#ifndef WIN32
	struct S_MSGBUF MsgBuf;

	if (m_MsgQID == -1)
		return err;

	MsgBuf.mtype = MsgT;
	memset(MsgBuf.mtext, 0x00, sizeof(MsgBuf.mtext));

	err = msgrcv(m_MsgQID, &MsgBuf, sizeof(MsgBuf.mtext), MsgT, IPC_NOWAIT);
	if (err == -1)
	{
		if (errno == E2BIG)
		{
			printf("Message is too big!!!\n");
			msgrcv(m_MsgQID, &MsgBuf, sizeof(MsgBuf.mtext), MsgT, IPC_NOWAIT|MSG_NOERROR);
		}
	}
	else
	{
		msg.append(MsgBuf.mtext, err);
	}
#endif

	return err;
}

/** @brief ������Ϣͨ��
 *  @param[in] MsgT ��Ϣ����
 *  @return <0 ʧ�ܣ�>=0 �ɹ�
 *  @remarks MsgT>0 ����ָ����Ϣ��MsgT=0 ����������Ϣ
 */
int C_MESSAGE::ClearMsg(int MsgT)
{
#ifndef WIN32
	if (m_MsgQID == -1)
		return -1;
	struct S_MSGBUF MsgBuf;
	while(msgrcv(m_MsgQID,&MsgBuf, sizeof(MsgBuf.mtext), MsgT, IPC_NOWAIT|MSG_NOERROR) != -1)
		;
#endif
	return 0;
}

/** @brief ������Ϣ
 *  @param[in] MsgT ��Ϣ����
 *  @param[in] msg ��Ϣ
 *  @return <0 ʧ�ܣ�>=0 �ɹ�
 */
int C_MESSAGE::PutMsg(int MsgT, const string &msg)
{
	m_MsgBuffer.push_back(msg);
	return (int)msg.size();
}

/** @brief ��ȡ��Ϣ
 *  @param[in] MsgT ��Ϣ����
 *  @param[out] msg ��Ϣ�������
 *  @return <0 ʧ�ܣ�>=0 �ɹ�
 */
int C_MESSAGE::GetMsg(int MsgT, string &msg)
{
	if (m_MsgBuffer.empty() == true)
		return -1;

	int size = (int)m_MsgBuffer.begin()->size();
	msg.append(*m_MsgBuffer.begin());
	m_MsgBuffer.erase(m_MsgBuffer.begin());
	return size;
}



