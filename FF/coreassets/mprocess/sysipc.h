/** @file
 *  @brief IPC�û���
 *  @author 
 *  @date 2011/11/07
 *  @version 0.1
 *  1�������̼����Ϣͨ��
 *  2�������ڴ��Ϊϵͳ�����ĺ�ϵͳ״̬��
 *  ϵͳ�������ǽ������ڴ��ļ��ϣ�ϵͳ״̬�ǽ�����FLASH�ļ��ϡ�
 */
#ifndef __MP_SYSIPC_H__
#define __MP_SYSIPC_H__
#include "message.h"
#include "sharemem.h"
#include "server.h"
#include "traceset.h"

/** @brief �����ڴ��ʶ */
#define CONTEXT_MEM_SIGN	"context.dat"
#define STATUS_MEM_SIGN		"status.dat"

#define MSAFN_IPC_JLCY_MSG		0xA1
#define MSAFN_IPC_JLCY_ALT		0xA2

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

/** @class
 *  @brief ϵͳ�����Ĺ����ڴ����
 *  @remarks ���������麯��ʱע��ص�������Ӧ�ĺ���
 */
class C_ContextBase
{
public:
	C_ContextBase(void);
	virtual ~C_ContextBase();

public:
	/** @brief ϵͳ�����ĵ�׼������
	 *  @return true ������false ʧ��
	 */
	bool Prepare(void);

	/** @brief �����³�ʼ����־(�ػ�����ʹ��) */
	bool SetNewFlag(bool fNew = true){m_ContextNew = fNew;return true;}
	/** @brief �ж�ϵͳ�������Ƿ�����(�ػ�����ʹ��) */
	bool ContextNewest(void){return m_ContextNew;}
	/** @brief �����������з����־(�ػ�����ʹ��) */
	bool SetStartAllServer(bool fEnable){m_StartAllServer = true; return true;}
	/** @brief �ж����з����Ƿ�����(�ػ�����ʹ��) */
	bool AllServerNeedStart(void){return m_StartAllServer;}

	/** @brief �������з���
	 *  @param[in] fActive [true|false]
	 *  @return true
	 */
	bool SetAllServerActive(bool fActive);
	/** @brief ϵͳ�������ڴ��־�Ƚ�
	 *  @param[in] memsign �ο���־
	 *  @return true �ɹ���false ʧ��
	 */
	bool MemsignCompare(const S_MEMSIGN &memsign);

	/** @brief ȡ����ʹ�ܱ�־
	 *  @param[in] PSN ������ţ�ID��
	 *  @return true ʹ�ܣ�false δʹ��
	 */
	bool GetProcessEnable(int PSN);
	/** @brief ʹ�ܽ���
	 *  @param[in] PSN ������ţ�ID��
	 *  @param[in] fEnable [true|false]
	 *  @return true
	 */
	bool EnableProcess(int PSN, bool fEnable);

	/** @brief ȡ������Ϣ
	 *  @param[in] PSN ������ţ�ID��
	 *  @return NULL ��Ч��!NULL ��Ч��Ϣ
	 */
	S_SERVINFO *GetServerInfo(int PSN);
	/** @brief ȡ���̸�����Ϣ����
	 *  @param[in] PSN ������ţ�ID��
	 *  @return NULL ��Ч��!NULL ��Ч��Ϣ
	 */
	S_TraceSet *GetTraceSet(int PSN);
	/** @brief ȡϵͳ��Ϣ
	 *  @return NULL ��Ч��!NULL ��Ч��Ϣ
	 */
	S_SystemInfo *GetSystemInfo(void);

	/** @brief ȡϵͳ��ʱ��
	 *  @return ϵͳ����ֵ
	 */
	S_TIME GetSystemTick(void);

	/** @brief ���������ű�
	 *  @param[in] fScript �ű�(��·��)
	 *  @note �����ű��ڿ��Ź���λǰ�����
	 */
	bool SetResetScript(const char *fScript);

public:
	/** @brief ���ó�Ա��Ϣ��
	 *  @return true �ɹ���false ʧ��
	 *  @remarks ��������ʱ���Ȼص��������������ڴ洴���ɹ�ʱ���ñ�����
	 */
	virtual bool SetInformation(void);
	/** @brief ��ʼ��ϵͳ������
	 *  @return true �ɹ���false ʧ��
	 *  @remarks ��������ʱ���Ȼص�������
	 */
	virtual bool InitContext(void);
	/** @brief �������õ�ϵͳ������
	 *  @return true �ɹ���false ʧ��
	 *  @remarks ��������ʱ���Ȼص�������
	 */
	virtual bool LoadContext(void);
	/** @brief ��������
	 *  @return true �ɹ���false ʧ��
	 *  @remarks ��������ʱ���Ȼص�������
	 */
	virtual bool ParamParse(void);
	/** @brief ��������λʱ�������
	 *  @return true �ɹ���false ʧ��
	 *  @remarks ��������ʱ���Ȼص�������
	 */
	virtual bool ClearData(void);
	
protected:
	bool m_ContextNew;///< ������daemon��ʹ��
	bool m_StartAllServer;///< ������daemon��ʹ��
	void *m_pContext; ///< ϵͳ�����Ĺ����ڴ�
	C_SHAREMEM m_ShareMem; ///< �����ڴ����
};

/** @class
 *  @brief ϵͳ״̬�����ڴ����
 *  @remarks ���������麯��ʱע��ص�������Ӧ�ĺ���
 */
class C_StatusBase
{
public:
	C_StatusBase(void);
	virtual ~C_StatusBase();

public:
	/** @brief ϵͳ״̬��׼������
	 *  @return true ������false ʧ��
	 */
	bool Prepare(void);
	/** @brief �����³�ʼ����־(�ػ�����ʹ��) */
	bool SetNewFlag(bool fNew = true){m_StatusNew = fNew; return true;}
	/** @brief �ж�ϵͳ״̬�Ƿ�����(�ػ�����ʹ��) */
	bool StatusNewest(void){return m_StatusNew;}

	/** @brief ����ϵͳ״̬
	 *  @return true �ɹ���false ʧ��
	 */
	bool SaveStatus(void){return m_ShareMem.Save();}
	/** @brief ϵͳ�������ڴ��־�Ƚ�
	 *  @param[in] memsign �ο���־
	 *  @return true �ɹ���false ʧ��
	 */
	bool MemsignCompare(const S_MEMSIGN &memsign);

public:
	/** @brief ���ó�Ա��Ϣ��
	 *  @return true �ɹ���false ʧ��
	 *  @remarks ��������ʱ���Ȼص��������������ڴ洴���ɹ�ʱ���ñ�����
	 */
	virtual bool SetInformation(void);
	/** @brief ��ʼ��ϵͳ״̬
	 *  @return true �ɹ���false ʧ��
	 *  @remarks ��������ʱ���Ȼص�������
	 */
	virtual bool InitStatus(void);
	/** @brief �������õ�ϵͳ״̬
	 *  @return true �ɹ���false ʧ��
	 *  @remarks ��������ʱ���Ȼص�������
	 */
	virtual bool LoadStatus(void);
	/** @brief ��������
	 *  @return true �ɹ���false ʧ��
	 *  @remarks ��������ʱ���Ȼص�������
	 */
	virtual bool ParamParse(void);
	/** @brief ��������λʱ�������
	 *  @return true �ɹ���false ʧ��
	 *  @remarks ��������ʱ���Ȼص�������
	 */
	virtual bool ClearData(void);

protected:
	bool m_StatusNew;///< ������daemon��ʹ��
	void *m_pStatus; ///< ϵͳ״̬�����ڴ�
	C_SHAREMEM m_ShareMem; ///< �����ڴ����
};


#endif //__MPROCESS_SYSIPC_H__
