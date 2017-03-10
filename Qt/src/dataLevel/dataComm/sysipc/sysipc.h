/** @file
 *  @brief IPC�û���
 *  @author  Steven Wan
 *  @date 2015/11/07
 *  @version 0.1
 *  1�������̼����Ϣͨ��
 *  2�������ڴ��Ϊϵͳ�����ĺ�ϵͳ״̬��
 *  ϵͳ�������ǽ������ڴ��ļ��ϣ�ϵͳ״̬�ǽ�����FLASH�ļ��ϡ�
 */
#ifndef __MP_SYSIPC_H__
#define __MP_SYSIPC_H__
#include "message.h"
#include "../inc/basedef.h"
//#include "traceset.h"


#define MSAFN_IPC_JLCY_MSG		0xA1
#define MSAFN_IPC_JLCY_ALT		0xA2

/** @brief ���̷������ƶ��� */
/** @berif �����ػ����̵Ľ����� */
#define MP_PROCESS_DAEMON	"daemon"
#define MP_PROCESS_HTTX1	"httx1"
#define MP_PROCESS_HTTX2	"httx2"
#define MP_PROCESS_HTTX3	"httx3"
#define MP_PROCESS_HTTX4	"httx4"
#define MP_PROCESS_DBCJ1		"dbcj1"
#define MP_PROCESS_DBCJ2		"dbcj2"
#define MP_PROCESS_DBCJ3		"dbcj3"
#define MP_PROCESS_DBCJ4		"dbcj4"
#define MP_PROCESS_PPP		"ppp"
#define MP_PROCESS_LCD		"lcd"
#define MP_PROCESS_JLCY		"jlcy"
#define MP_PROCESS_FHKZ		"fhkz"
#define MP_PROCESS_CTCY		"ctcy"

#define MP_PROCESS_RFTX              "rftx"   //wjp 20160729
/** @brief ����������
 *  @remarks �������̺�һ��Ҫ�޸ı���Ķ��壬
 *  ����ֵ������gMP_ProcessAll�����н��̸�����ȣ����򽫳���PLEASE BE CAREFUL.
 */
#define MP_PROCESS_CNT	(1/*daemon*/+4/*httx */+4/*dbcj */+1/*ppp*/+1/*lcd*/+1/*jlcy*/+1/*fhkz*/+1/*ctcy*/+1/*rftx*/)
enum 
{
	MSAFN_CONFIRM_DENY = 0x00,//ȷ��/����
	MSAFN_LINK_CHECK = 0x02,
	MSAFN_WRITE_PARAM = 0x04,
	MSAFN_AUTHENTICATE = 0x06,
	MSAFN_READ_PARAM = 0x0A,
	MSAFN_READ_REALTIME = 0x0C,
	MSAFN_READ_HISTORY = 0x0D,
	MSAFN_READ_EVENT = 0x0E,
	MSAFN_FILE_TRANSFER = 0x0F,
	MSAFN_DATA_ROUTE = 0x10,
	MSAFN_READ_TASK = 0x12,
	MSAFN_READ_ALARM = 0x13,
	MSAFN_FORWARD = 0x14,
	MSAFN_USER_DEFINE = 0x15,

//�������ն�����
	MSAFN_FORWARD_MS = 0x2B,

//user define
	MSAFN_USER_DEFINE_BEGIN = 0x80
};

struct S_PROCESS
{
	const char *m_pName;///<������
	const char *m_pInfo;///<���̰�����Ϣ
};


/* @��Ϣͨ�Ÿ�ʽ[�������������淶������������DI(4B)��ΪFN(2B)]
һ������ʽ����Ϣ���� + ��Ϣ����
  ��Ϣ����=MSAFN_ (enum��)
  ��Ϣ����=DA(2B) + DI(4B) [+ CONTEXT]
  ע���ڻظ�ʱ�ɼ�ʧ�ܵ������������Чֵ�����������DA��DI��
  ���ȫ��DA��DI��������ˣ���ظ���һ��DA��DI
������:ʵ�б�3�����й��ܺͷ����й���
  ��Ϣ����=MSAFN_READ_REALTIME
  ������Ϣ����: 04 01 00 00 01 00 04 01 00 00 02 00
  ������Ϣ����: 04 01 00 00 01 00 12 34 56 78 04 01 00 00 02 00 90 78 56 34
  �����쳣1: 04 01 00 00 01 00 
  �����쳣2: 04 01 00 00 01 00 FF FF FF FF 04 01 00 00 02 00 90 78 56 34
  �����쳣3: 04 01 00 00 01 00 12 34 56 78 04 01 00 00 02 00 FF FF FF FF
  �����쳣4: 04 01 00 00 02 00 90 78 56 34
  �����쳣5: 04 01 00 00 01 00 12 34 56 78
*/

/** @brief ������Ϣͨ�Ž��̲�λ���
�߲㣺httx lcd
�в㣺fhkz abcj ppp
�Ͳ㣺jlcy

�߲������Ͳ�������Ϣ���Ͳ㲻������߲�������Ϣ
ͬ�㾡�����������Ϣ��������
��Ϣ���;�����ϸ�㣬�����̺͹������֣���Ҫ��ͬ
*/

/** @class
 *  @brief ϵͳ��Ϣͨ��
 *  @remarks ʹ��ʱע�����ַ��ͽ��̺ͽ��ս���
 */
class C_SysMsg
{
public:
	enum{MTYPE_BASE_VAL=100, SYSMSG_TIMEOUT_MAX=60};

	/** @brief ������Ϣ������
	 *  @param[in] SerID ����ID��0 ~ ��������-1��
	 *  @param[in] MsgT ��Ϣ����
	 *  @param[in] msg ��Ϣ
	 *  @return -1 ���ͳ���0 ���ͳɹ�
	 */
	static int Send2Server(int SerID, S_BYTE MsgT, const string &msg);
	/** @brief ������Ϣ������
	 *  @param[in] pServName ��������
	 *  @param[in] MsgT ��Ϣ����
	 *  @param[in] msg ��Ϣ
	 *  @return -1 ���ͳ���0 ���ͳɹ�
	 */
	static int Send2Server(const char *pServName, S_BYTE MsgT, const string &msg);
	/** @brief ���շ�����Ϣ
	 *  @param[in] MsgT ��Ϣ����
	 *  @param[out] msg ��Ϣ�������
	 *  @return -1 ����ʧ�ܣ�0 ���ճɹ�
	 */
	static int Recv4Server(S_BYTE MsgT, string &msg);
	/** @brief �ȴ�������Ϣ
	 *  @param[in] MsgT ��Ϣ����
	 *  @param[out] msg ��Ϣ�������
	 *  @param[in] WaitSecond �ȴ���ʱʱ�䣬��λ�룬���SYSMSG_TIMEOUT_MAX
	 *  @return -1 ����ʧ�ܣ�0 ���ճɹ�
	 */
	static int Wait4Server(S_BYTE MsgT, string &msg, int WaitSecond = SYSMSG_TIMEOUT_MAX);

	/** @brief ��ѯ������Ϣ
	 *  @param[out] SerID ����ID
	 *  @param[out] MsgT ��Ϣ����
	 *  @param[out] msg ��Ϣ����
	 *  @return -1 ��ѯʧ�ܣ�0 ��ѯ�ɹ�
	 */
	static int Query4Server(int &SerID, S_BYTE &MsgT, string &msg);
	/** @brief �ظ���Ϣ������
	 *  @param[in] SerID ����ID
	 *  @param[in] MsgT ��Ϣ����
	 *  @param[in] msg ��Ϣ����
	 *  @return -1 �ظ�����0 �ظ��ɹ�
	 */
	static int Reply2Server(int SerID, S_BYTE MsgT, const string &msg);

private:
	static C_MESSAGE m_MsgObj; ///< ��Ϣ����
	static S_BYTE m_ReqSeq; ///< ��Ϣ����˳���
	static S_BYTE m_ResSeq; ///< ��Ϣ��Ӧ˳���
	static string m_msg;
	static string MSG;
};
#endif


