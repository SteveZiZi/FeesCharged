/** @file
 *  @brief RFIDͨѶ��
 *  @author 
 *  @date 2016/12/06
 *  @version 0.1
 *  @note RFIDͨѶ��ض����ʵ��
 */
#ifndef __RFID_COM_H__
#define __RFID_COM_H__

#include <string>
#include <vector>
#include "sysbase.h"


/** @class
 *  @brief RFIDͨ��
 */
class C_RFIDCom
{
public:
	C_RFIDCom(void);
	virtual ~C_RFIDCom();

public:
	/** @brief ��ʼ��RFIDͨ������ 
	 *  @return none
	 */
	void InitComParam(void);
	/** @brief ��ʼ��RFIDͨ�� 
	 *  @return -1:ʧ�� 0 �ɹ�
	 */
	int InitRFIDComm(void);
	/** @brief ��ȡ����
	*  @return -1 ����0 ��ȡ���ݳɹ�
	*/
	int Read(S_WORD CmdID,string &SstrData,string &RstrData);
	/** @brief �Ͽ����� 
	*  @return true �ɹ���false ʧ��
	*/
	S_BOOL Disconnect(void);
	
private:
	/** @brief �������� 
	 *  @return true �ɹ���false ʧ��
	 */
	S_BOOL OnConnect(void);
	/** @brief �Ͽ����� 
	*  @return true �ɹ���false ʧ��
	*/
	S_BOOL OnDisconnect(void);
	/** @brief ������뻺�� 
	*  @return true �ɹ���false ʧ��
	*/
	S_BOOL ClearInBuffer(void);
	/** @brief ���������� 
	*  @return true �ɹ���false ʧ��
	*/
	S_BOOL ClearOutBuffer(void);
	/** @brief �ж�����״̬ 
	*  @return true ���ӣ�false δ����
	*/
	S_BOOL ConnectSuccessful(void);
	/** @brief ���ô��� 
	*  @return true �ɹ���false ʧ��
	*/
	S_BOOL SetCom(void);
	/** @brief ��ʼ������ 
	*  @return -1 ����>=0 ��Ч����������
	*/
	int InitCom(void);
	/** @brief ��ȡ���� 
	*  @param[out] pData �������
	*  @param[in] size ��������С
	*  @return -1 ����>=0 ��ȡ���ֽ���
	*/
	int ReadData(S_BYTE *pData, int size);								
	/** @brief д������ ������(�ײ�)
	*  @param[in] pData ���뻺��
	*  @param[in] size ���뻺���С
	*  @return -1 ����>=0 д����ֽ���
	*/
	int WriteData(const S_BYTE *pData, int size);						
	/** @brief ��ȡ���������� д����(�ײ�)
	*  @param[in] Baud ��������ֵ
	*  @return B115200,B57600,B38400, B19200, B9600, B4800, B2400, B1200,B600, B300
	*/
	int GetBaudAttr(int Baud);
	int ErrorOut(S_BOOL fWarnLevel, const string &strErrorInfo);

	/** @brief �������� 
	*  @return -1 ����>=0 ���͵����ݵĳ���
	*/
	int SendData(string &rfSendData);									
	/** @brief �������� 
	*  @return -1 ����>=0 ���յ����ݵĳ���
	*/
	int ReceiveData(string &rfReceiveData);	
	/** @brief ���ͽ��� 
	*  @return -1 ����>=0 ���ͽ��յ����ݵĳ���
	*/
	int RequestData(string &rfSendData, string &rfRevData);
	/** @brief ��֡
	*  @return -1 ����0 ��֡�ĳ���
	*/
	int MakeReadFrame(S_WORD CmdID, string &strData,  string &strFrame);
	/** @brief ��ȡ����֡
	*  @param[in] strBuffer �����
	*  @param[out] strCmd ֡�������
	*  @return <0: ����[����ֵ�ǳ�����ֽ���]; =0: ��֡; >0: �ɹ�
	*/
	int ExtractCmd(string &strBuffer, string &strRecvCmd);
	/** @brief ��ȡ����֡
	*  @param[in] strBuffer �����
	*  @param[out] strCmd ֡�������
	*  @return <0: ����[����ֵ�ǳ�����ֽ���]; =0: ��֡; >0: �ɹ�
	*/
	int _ExtractCmd(string &strBuffer, string &strCmd);
	/** @brief �����������֡(�ڲ�ʹ��)
	*  @param[in] strCmd ��������֡,��Ϊ�����������
	*  @param[in] CommReg ͨ�üĴ���
	*  @param[out] strData �����������
	*  @return <0: ����[����ֵ�ǳ�����ֽ���]; =0: ��֡; >0: �ɹ�
	*/
	int ParseReadFrame(string &strCmd, string &strData);
	/** @brief ���ò�����
	*  @return -1 ����0 �ɹ�
	*/
	int CommParamSet(void);

private:
	int				m_Comfd;											// < ���ھ��
	S_COMSET		m_ComSet;	
	double			m_EnValue;
};

#endif//__RFID_COM_H__


