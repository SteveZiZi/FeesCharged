/** @file
 *  @brief IPC�û���
 *  @author  Steven Wan
 *  @date 2015/11/07
 *  @version 0.1
 *  1�������̼����Ϣͨ��
 *  2�������ڴ��Ϊϵͳ�����ĺ�ϵͳ״̬��
 *  ϵͳ�������ǽ������ڴ��ļ��ϣ�ϵͳ״̬�ǽ�����FLASH�ļ��ϡ�
 */
#include "sysipc.h"


/** @brief ��Ϣͨ��ȫ�ֱ������� */
C_MESSAGE C_SysMsg::m_MsgObj('m');
S_BYTE C_SysMsg::m_ReqSeq = 0;
S_BYTE C_SysMsg::m_ResSeq = 0;
string C_SysMsg::m_msg="";
string C_SysMsg::MSG="";

//
//���ö����
//
/** @brief ��������
 *  @note ���ӽ��̵ķ���
 *  1����cfg_proc.h�ж���������꣬ͬʱ�޸Ľ���������MP_PROCESS_CNT
 *  2��������������뵽gMP_ProcessAll�����У�����̿�ܽ��Զ�����ָ���Ľ��̡�
 *  3���̳�C_SERVER�࣬��д����Ҫʵ�ֵľ��幦�ܡ��μ�server.h��
 *  4���̳�C_AppCmd�࣬��C_SERVER������GetServer()������ʵ�������μ�appcmd.h��
 *  @remarks �������겻���ڱ��ļ��ж��塣
 *  @see server.h appcmd.h cfg_proc.h
 */
S_PROCESS gMP_ProcessAll[] =
{
	//�����������̹���˵��
	{MP_PROCESS_DAEMON, "ϵͳ�ػ����̣�����ϵͳ�ܿ�"},
//�û����̼��ں���
	{MP_PROCESS_LCD,   "lcd���̣����𱾵ز�������ʾ"},
	{MP_PROCESS_HTTX1, "httx1���̣�������վͨѶ(�ͻ���)"},
	{MP_PROCESS_HTTX2, "httx2���̣��������ͨѶ(������)"},
	{MP_PROCESS_HTTX3, "httx3���̣������ͨѶ"},
	{MP_PROCESS_HTTX4, "httx4���̣�����ͨѶ"},
	{MP_PROCESS_DBCJ1,  "dbcj1���̣����������ɼ������ݶ���"},
	{MP_PROCESS_DBCJ2,  "dbcj2���̣����������ɼ������ݶ���"},
	{MP_PROCESS_DBCJ3,  "dbcj3���̣����������ɼ������ݶ���"},
	{MP_PROCESS_DBCJ4,  "dbcj4���̣����������ɼ������ݶ���"},
	{MP_PROCESS_PPP,   "ppp���̣���������վ������������"},
	{MP_PROCESS_JLCY, "jlcy���̣�����Խ���оƬ���ݲɼ�"},
	{MP_PROCESS_FHKZ, "fhkz���̣�����澯����͸������ݴ���"},
	{MP_PROCESS_CTCY, "ctcy���̣�����CT�����͵�ع���"},
	{MP_PROCESS_RFTX,   "RFTX���̣������ն�����Ƶģ��ͨ��"},//wjp 20160729

//���̶������
	{NULL, NULL}
};

/** @brief ��ȡ�������
 *  @param[in] pName ��������
 *  @return 0~(MP_PROCESS_COUNT-1) ���ҳɹ�
 *  @return -1 ����ʧ��
 *  @remarks ����������Ҫ��д
 */
int MP_GetProcessSN(const char *pName)
{
	for (int i = 0; gMP_ProcessAll[i].m_pName != NULL; i++)
	{
		if (strcmp(gMP_ProcessAll[i].m_pName, pName) == 0)
			return i;
	}
	return -1;
}

/** @brief ���ݷ�����ȡ����ID */
int GetServerID(const char *ServerName)
{
	return MP_GetProcessSN(ServerName);
}
/** @brief ��ȡ�������� 
 *	@param[in] PSN ���̵���Ч���(0~(MP_PROCESS_COUNT-1))
 *	@return NULL PSN�����Ч
 *  @return !NULL ������ָ��
 *  @remarks ����������Ҫ��д,�û����豣֤PSN�������Ч��
 */
const char* MP_GetProcessName(int PSN)
{
	return gMP_ProcessAll[PSN].m_pName;
}
/** @brief ���ݷ���IDȡ������ */
const char *GetServerName(int ServerID)
{
	return MP_GetProcessName(ServerID);
}

#define MP_PROCESS_QT_ID		32
int GetCurrentServID(void)
{
	return MP_PROCESS_QT_ID;
}

/** @brief ������Ϣ������
 *  @param[in] SerID ����ID��0 ~ ��������-1��
 *  @param[in] MsgT ��Ϣ����
 *  @param[in] msg ��Ϣ
 *  @return -1 ���ͳ���0 ���ͳɹ�
 */
int C_SysMsg::Send2Server(int SerID, S_BYTE MsgT, const string &msg)
{
	int SelfID = GetCurrentServID();
	string MSG;

	m_ReqSeq += 1;

	MSG.push_back(MsgT);
	MSG.push_back(m_ReqSeq);
	MSG.append((S_CHAR*)&SelfID, sizeof(SelfID));
	MSG.append(msg);

//	logit<<"Send "<<GetServerName(SerID)<<" msg:"<<HEX<<msg<<"."<<endl;

	return m_MsgObj.SendMsg(SerID+MTYPE_BASE_VAL, MSG);
}

/** @brief ������Ϣ������
 *  @param[in] pServName ��������
 *  @param[in] MsgT ��Ϣ����
 *  @param[in] msg ��Ϣ
 *  @return -1 ���ͳ���0 ���ͳɹ�
 */
int C_SysMsg::Send2Server(const char *pServName, S_BYTE MsgT, const string &msg)
{
	return Send2Server(GetServerID(pServName), MsgT, msg);
}

/** @brief ���շ�����Ϣ
 *  @param[in] MsgT ��Ϣ����
 *  @param[out] msg ��Ϣ�������
 *  @return -1 ����ʧ�ܣ�0 ���ճɹ�
 */
int C_SysMsg::Recv4Server(S_BYTE MsgT, string &msg)
{
	//string MSG;
	MSG.clear();
	
	if (m_MsgObj.RecvMsg(GetCurrentServID()+MTYPE_BASE_VAL, MSG) > 6)
	{
		printf("recv msg > 6\n");
		if ((S_BYTE)MSG[0] != MsgT)
		{
			m_MsgObj.PutMsg(GetCurrentServID()+MTYPE_BASE_VAL, MSG);
//			loget<<"Recv other msg:"<<HEX<<MSG<<"!"<<endl;
			return -1;
		}
		else if ((S_BYTE)MSG[1] != m_ReqSeq)
		{
//			loget<<"Recv not expected msg:"<<HEX<<MSG<<"!"<<endl;
			return -1;
		}
		int SerID;
		copy(MSG.begin()+2, MSG.begin()+2+sizeof(SerID), &SerID);
		msg.append(MSG.begin()+2+sizeof(SerID), MSG.end());
//		logit<<"Recv "<<GetServerName(SerID)<<" msg:"<<HEX<<msg<<"."<<endl;
		return 0;
	}
	else if (!MSG.empty())
	{
		printf("Recv exception msg:\n");
//		logwt<<"Recv exception msg:"<<HEX<<MSG<<endl;
	}
	return -1;
}

/** @brief �ȴ�������Ϣ
 *  @param[in] MsgT ��Ϣ����
 *  @param[out] msg ��Ϣ�������
 *  @param[in] WaitSecond �ȴ���ʱʱ�䣬��λ�룬���SYSMSG_TIMEOUT_MAX
 *  @return -1 ����ʧ�ܣ�0 ���ճɹ�
 */
int C_SysMsg::Wait4Server(S_BYTE MsgT, string &msg, int WaitSecond)
{
	m_msg.clear();
	if (WaitSecond < 0 || WaitSecond > SYSMSG_TIMEOUT_MAX)
		WaitSecond = SYSMSG_TIMEOUT_MAX;
	for (int i = 0; i < WaitSecond*5; ++i)
	{
		if (Recv4Server(MsgT, m_msg) == 0)
		{
			printf("Recv4Server");
			msg = m_msg;
		}
		
#ifdef WIN32
		msleep(200);
#else
		usleep(200000);
#endif
	}			
	return 0;
}

