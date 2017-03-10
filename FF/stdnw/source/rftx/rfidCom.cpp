/** @file
 *  @brief RFIDͨѶ��
 *  @author 
 *  @date 2016/12/06
 *  @version 0.1
 *  @note RFIDͨѶ��ض����ʵ��
 */
#include "rfidCom.h"

#include <unistd.h> 
#include <dirent.h>
#include <errno.h>
#include <sys/file.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h> 
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/io.h>
#include <sys/sem.h>
#include <sys/param.h>
#include <fcntl.h>
#include <signal.h>
#include <netinet/in.h>
#include <netdb.h>
#include <termios.h>
#include <assert.h>

#include <numeric>
#include <math.h>
#include "strings.h"
#include <stdio.h>

#include "htrace.h"

//�ն�����Ƶģ��ͨ��Э���ʽ����
/*
 ֡��ʽ:  STX   LEN   CMD   CMDPARAM   DATA   BCC   ETX 
 �ֽ���:   1       2       1             1              N        1       1
*/

#define FRAME_STX						0x02							//֡ͷ
#define FRAME_ETX						0x03							//֡β
#define FRAME_MIN_LEN					0x07							//֡��С����
#define FRAME_BCC_ERR					0xF4

#define FRAME_VAILD            			0x00							//֡��Ч
#define	FRAME_AUTH_FAIL 				0x01							//��֤ʧ��
#define	FRAME_METER_ERR					0x02							//���ַ��һ��
#define	FRAME_PARAM_ERR					0xF0							//֡��������
#define	FRAME_LEN_ERR					0xF1							//֡���ȴ���
#define	FRAME_BCC_ERR					0xF4 							//֡У�����
#define	FRAME_ERR                       0xF5							//��������  
#define	FRAME_INVAILD					0xFE							//֡��Ч

/** @class
 *  @brief RFIDͨ��
 */
C_RFIDCom::C_RFIDCom()
: m_Comfd(-1) 
{
	
}

C_RFIDCom::~C_RFIDCom()
{

}

/** @brief ��ʼ��RFIDͨ������ 
*  @return none
*/
void C_RFIDCom::InitComParam(void)
{
	m_Comfd = -1;
	m_ComSet.m_PortNO = 3; //���ں�
	m_ComSet.m_Baud = 115200;//Ĭ�ϲ�����//115200
	m_ComSet.m_CDCheck = 0;
	m_ComSet.m_DataBit = 8;// ����λ
	m_ComSet.m_Parity = 0;//У��λ 0-��У��; 1-��У��; 2-żУ��
	m_ComSet.m_StopBit = 1;	// ֹͣλ
	m_EnValue = 0.0;
}

/** @brief ��ʼ��RFIDͨ�� 
*  @return -1:ʧ�� 0 �ɹ�
*/
int C_RFIDCom::InitRFIDComm(void)
{
	S_BOOL Success = false;
	int result = -1;
	printf("%s InitRftxComm \n","start...");

	while(1)
	{
		int DelayTime = 10;
		int k = 0x00;
		int ret = -1;
		for(k = 0x00;k < 1000;k++)
		{
			Success = OnConnect();
			if(Success)
			{
				if(ConnectSuccessful())
				{
					ret = 0x01;
					break;
					printf("%s Started \n","VCOM2 RFTX Open Successful");
				}
			}
			printf("%s\n","VCOM2 RFTX Open Fail");
			sleep(DelayTime*k);
		}
		if(ret == 0x01)
		{
			result = 0;
			break;
		}
		else
		{
			result = -1;
			break;			
		}
	}
	ClearInBuffer();//��մ��ڻ�����
	ClearOutBuffer();//��մ��ڻ�����
	printf("Clean VCom2/RFTX Buffer ******\n");

	sleep(1);
	return result;
}

/** @brief ��ȡ����
*  @return -1 ����0 ��ȡ���ݳɹ�
*/
int C_RFIDCom::Read(S_WORD CmdID,string &SstrData,string &RstrData)
{
	int ok = -1;

	string SstrFrame,RstrFrame,strRecvCmd;

	for(int i =0; i<3; i++)
	{
		if (MakeReadFrame(CmdID, SstrData, SstrFrame) <= 0)
		{
			ok=  -1;
			return ok;
		}


		if (RequestData(SstrFrame, RstrFrame) <= 0)
		{
			logwt<<"���ͽ�������ʧ��......"<<endl;

			ok = -1;

			return ok;
		}
		if (ExtractCmd(RstrFrame, strRecvCmd) <= 0)
		{
			logwt<<"���������ȡ��Լ֡����"<<endl;
			ok = -1;
			return ok;
		}

		ok = ParseReadFrame(strRecvCmd,  RstrData);

		if(ok == FRAME_AUTH_FAIL)
		{
			if(CmdID == 0x3001)
			{
				logwt<<"��������֧�ָò�����"<<endl;
			}
			else if(CmdID == 0x4010)
			{
				logwt<<"Ѱ��ʧ��"<<endl;			
			}
			else if(CmdID == 0x4011)
			{
				logwt<<"ESAM�ϵ�ʧ��"<<endl;			
			}
			else if(CmdID == 0x4012 || CmdID == 0x4014 || CmdID == 0x4015)
			{
				logwt<<"�����ն˺�CPU�������֤ʧ��"<<endl;
			}
		}

		if(ok == FRAME_METER_ERR)
		{
			logwt<<"�û����ڱ���뽻���ն˱�Ų�һ��"<<endl;
		}

		if(ok != FRAME_BCC_ERR)
		{
			break;
		}
	}

	return ok;
}

/** @brief �Ͽ����� 
*  @return true �ɹ���false ʧ��
*/
S_BOOL C_RFIDCom::Disconnect(void)
{
	if (OnDisconnect())
		return true;

	return false;
}

/** @brief �������� 
 *  @return true �ɹ���false ʧ��
 */
S_BOOL C_RFIDCom::OnConnect(void)
{
	if((m_Comfd = InitCom()) < 0)
	{return false;}

	SetCom();
	return true;
}

/** @brief �Ͽ����� 
 *  @return true �ɹ���false ʧ��
 */
S_BOOL C_RFIDCom::OnDisconnect(void)
{
	if (m_Comfd != -1)
	{close(m_Comfd);}
	m_Comfd = -1;
	return true;
}

/** @brief ������뻺�� 
 *  @return true �ɹ���false ʧ��
 */
S_BOOL C_RFIDCom::ClearInBuffer(void)
{
	if(ConnectSuccessful() == true)
	{
		if(tcflush(m_Comfd, TCIFLUSH) == 0)
		{return true;}
	}
	return false;
}

/** @brief ���������� 
 *  @return true �ɹ���false ʧ��
 */
S_BOOL C_RFIDCom::ClearOutBuffer(void)
{
	if(ConnectSuccessful() == true)
	{
		if(tcflush(m_Comfd, TCOFLUSH) == 0)
		{return true;}
	}
	return false;
}

/** @brief �ж�����״̬ 
 *  @return true ���ӣ�false δ����
 */
S_BOOL C_RFIDCom::ConnectSuccessful(void)
{
	return m_Comfd != -1;
}

/** @brief ���ô��� 
 *  @return true �ɹ���false ʧ��
 */
S_BOOL C_RFIDCom::SetCom(void)
{
	struct termios ioCom;

	if (m_Comfd < 0)
		return false;

	S_COMSET &ComSet = m_ComSet;

	bzero(&ioCom, sizeof(ioCom));
	ioCom.c_cflag = 0;
	ioCom.c_oflag = 0;		
	ioCom.c_lflag = 0;
	ioCom.c_line = 0;
	ioCom.c_iflag = 0;

	if (ComSet.m_Parity == COMM_PARITY_ODD)
		ioCom.c_cflag |= PARENB|PARODD;
	else if (ComSet.m_Parity == COMM_PARITY_EVEN)
		ioCom.c_cflag |= PARENB;
	
	if (ComSet.m_StopBit == COMM_STOP_BIT2)
		ioCom.c_cflag |= CSTOPB;

	if (ComSet.m_DataBit == COMM_DATA_BIT7)
		ioCom.c_cflag |= CS7;
	else
		ioCom.c_cflag |= CS8;

	ioCom.c_cflag |= CREAD|HUPCL|CLOCAL;
	ioCom.c_cc[VMIN] = 1;
	ioCom.c_cc[VTIME] = 0;
	
	int BaudAttr = GetBaudAttr(ComSet.m_Baud);
	cfsetispeed(&ioCom, BaudAttr);
	cfsetospeed(&ioCom, BaudAttr);

	tcsetattr(m_Comfd, TCSANOW, &ioCom);

	char ComCfgInfo[64];
	sprintf(ComCfgInfo, "%d,%d,%d,%d,%d", ComSet.m_PortNO, ComSet.m_Baud , ComSet.m_DataBit, ComSet.m_StopBit, ComSet.m_Parity);
	string strComCfg;
	strComCfg.append(":�������ã�").append(ComCfgInfo);
	ErrorOut(true, strComCfg);
	return true;
}


/** @brief ��ʼ������ 
 *  @return -1 ����>=0 ��Ч����������
 */
int C_RFIDCom::InitCom(void)
{
	S_BYTE PortNO = m_ComSet.m_PortNO;

//	printf("%s PortNO = VCom%d\n","RFTX InitCom",PortNO);
	logvt<<"hardware portno ="<<hex<<PortNO<<endl;
	if(PortNO == 0)
	{
		string strPortErr;
		strPortErr.append(":RFTX ���ں�Ϊ0,����˿������ļ�!!!");
		ErrorOut(true, strPortErr);
		return -1;
	}

	char cDevName[15];
	sprintf(cDevName, "/dev/ttyS%d", PortNO-1);

	string strInitInfo;
	strInitInfo.append(":��ʼ������").append(cDevName).append("...");
	ErrorOut(false, strInitInfo);

	int fd = -1;
	if((fd = open(cDevName, O_RDWR|O_NONBLOCK, 0666)) < 0)
	{
		string strErrorInfo(strInitInfo);
		strErrorInfo.append("��ʧ��!");
		loget<<"���ڴ�ʧ��\n"<<endl;
		ErrorOut(true, strErrorInfo);
		return -1;
	}

	if (fcntl(fd, F_SETFL, FNDELAY) < 0)
	{
		close(fd);
		return -1;
	}
	if (flock(fd, LOCK_EX|LOCK_NB) != 0)
	{
		string strErrorInfo(strInitInfo);
		strErrorInfo.append("����ʧ��!");
		loget<<"��������ʧ��\n"<<endl;
		ErrorOut(true, strErrorInfo);
		close(fd);
		return -1;
	}
	return fd;
}

/** @brief ��ȡ���� 
 *  @param[out] pData �������
 *  @param[in] size ��������С
 *  @return -1 ����>=0 ��ȡ���ֽ���
 */
int C_RFIDCom::ReadData(S_BYTE *pData,int size)
{
	if(m_Comfd == -1)
	{return -1;}
	
	struct timeval ReadTimeout;
	int ReadTimeoutMS = 2000;
	ReadTimeout.tv_sec = ReadTimeoutMS / 1000;
	ReadTimeout.tv_usec = (ReadTimeoutMS % 1000)*1000;
	fd_set fs_read;
	FD_ZERO(&fs_read);
	FD_SET(m_Comfd, &fs_read);
	if (select(m_Comfd + 1, &fs_read, NULL, NULL, &ReadTimeout) <= 0)
	{return 0;}	
	int ReadBytes = read(m_Comfd, pData, size);
	return ReadBytes;	
}

/** @brief д������ 
 *  @param[in] pData ���뻺��
 *  @param[in] size ���뻺���С
 *  @return -1 ����>=0 д����ֽ���
 */
int C_RFIDCom::WriteData(const S_BYTE *pData, int size)
{
	ClearInBuffer();
	ClearOutBuffer();
	
	if(m_Comfd == -1)
	{return -1;}

	int WriteBytes = write(m_Comfd, (const char*)pData, size);
	if(WriteBytes > 0)
	{tcdrain(m_Comfd);}

	return WriteBytes;
}


/** @brief ��ȡ���������� 
 *  @param[in] Baud ��������ֵ
 *  @return B115200,B57600,B38400, B19200, B9600, B4800, B2400, B1200,B600, B300
 */
int C_RFIDCom::GetBaudAttr(int Baud)
{
	int BaudAttr[] = {B115200,B57600,B38400, B19200, B9600, B4800, B2400, B1200,B600, B300};
	int BaudSpeed[] = {115200, 57600, 38400, 19200, 9600, 4800, 2400, 1200, 600, 300};
	for (size_t i= 0;  i < (sizeof(BaudSpeed)/sizeof(BaudSpeed[0]));  i++)
	{
		if  (Baud == BaudSpeed[i])
			return BaudAttr[i];
	}
	return B19200;
}

int C_RFIDCom::ErrorOut(S_BOOL fWarnLevel, const string &strErrorInfo)
{
	if (!strErrorInfo.empty())
	{
		if (!strErrorInfo.empty())
		{
			if (fWarnLevel == true) 
			{
			}
			else 
			{
			}
		}
	}
	return (int)strErrorInfo.size();
}

/** @brief �������� 
*  @return -1 ����>=0 �������ݵĳ���
*/
int C_RFIDCom::SendData(string &rfSendData)
{
    int result = -1;

    S_BYTE* send_data;
    S_DWORD dataLen;

     send_data = (S_BYTE*)rfSendData.c_str();
     dataLen = ( S_DWORD)rfSendData.size();
     logvt<<"rftx send:"<<hex<<rfSendData<<endl;	
     if( dataLen > 0 && WriteData(send_data,dataLen) != -1)
      {
	   
            result = (int)rfSendData.size();
	     logvt<<"send bytes"<<dec<<result<<endl;		
       }

      return result;
}

int C_RFIDCom::ReceiveData(string &rfReceiveData)
{
    S_DWORD data_Len = 0;
    S_BYTE tempBuf[500];
    int  ret = -1;
	
    memset(tempBuf,0x00,sizeof(tempBuf));
	
    // �ɼ�����
    data_Len = ReadData(tempBuf, sizeof(tempBuf));

    if( 0 >= data_Len)
    {
	 return ret;
    }
    else if( data_Len > 0)
    {
	ret = data_Len;
    }

    rfReceiveData.append((char*)tempBuf, data_Len);
    logvt<<"rftx recv:"<<hex<<rfReceiveData<<endl;		
	
    ret = (int)rfReceiveData.size();
    return ret;
}

int C_RFIDCom::RequestData(string &rfSendData, string &rfRevData)
{
	int RepeatNum = 2;
	do{
		string TempRevData;

		if (SendData(rfSendData) <= 0)
		{
			return -1;
		}
		
		SleepMS(10);

		C_MSTIMOUT WaitComm(500);
		SleepMS(500);

		
		do{
			if (ReceiveData(TempRevData) <= 0)
			{
				continue;
			}
			else
			{
				rfRevData.append(TempRevData);
				return (int)rfRevData.size();
			}
		}while (WaitComm.timeout() == false);
		sleep(1);		
	}while(RepeatNum--);

	return -1;
}

/** @brief ��֡
*  @return -1 ����0 ��֡�ĳ���
*/
int C_RFIDCom::MakeReadFrame(S_WORD CmdID, string &strData,  string &strFrame)
{
	S_WORD AppDataLen = 0x00;

	AppDataLen = 2 + strData.size();// 2�ֽ�����+ N�ֽ�����

	//STX
	strFrame.push_back(FRAME_STX);//ͷ02
	//LEN
	strFrame.push_back(AppDataLen/0x100%0x100);// 2�ֽڳ���
	strFrame.push_back(AppDataLen%0x100);
	//CMD PARAM
	strFrame.push_back(CmdID/0x100%0x100);// 2�ֽ�����	
	strFrame.push_back(CmdID%0x100);// 2�ֽ�����	
	//DATA
	if(strData.empty())//N�ֽ�����
	{

	}
	else
	{
		strFrame.append(strData.begin(), strData.end());
	}
	//BCC
	char CheckSum = strFrame.at(3);//��3�ֽڿ�ʼУ��(Ӧ�ò�����)
	for(S_WORD i =1;i< AppDataLen;i++)
	{
		CheckSum ^= strFrame.at(3+i);
	}

	strFrame.push_back((char)CheckSum);//���У��
	//ETX
	strFrame.push_back(FRAME_ETX);//β 03

	return (int)strFrame.size();
}

/** @brief ��ȡ����֡
 *  @param[in] strBuffer �����
 *  @param[out] strCmd ֡�������
 *  @return <0: ����[����ֵ�ǳ�����ֽ���]; =0: ��֡; >0: �ɹ�
 */
int C_RFIDCom::ExtractCmd(string &strBuffer, string &strRecvCmd)
{
	while (true)
	{
		string strMyData;
		int cnt = _ExtractCmd(strBuffer, strMyData);
		if (cnt > 0)
		{
			strRecvCmd.append(strMyData);
			return (int)strMyData.size();
		}
		else
		{
			break;
		}
	}
	return 0;
}

/** @brief ��ȡ����֡
 *  @param[in] strBuffer �����
 *  @param[out] strCmd ֡�������
 *  @return <0: ����[����ֵ�ǳ�����ֽ���]; =0: ��֡; >0: �ɹ�
 */
int C_RFIDCom::_ExtractCmd(string &strBuffer, string &strCmd)
{
	if (strBuffer.empty()) return 0;
	string::iterator iter = find(strBuffer.begin(), strBuffer.end(), (char)FRAME_STX);
	if (iter != strBuffer.begin())
	{
		int diff = (int)distance(strBuffer.begin(), iter);
		return (-diff);
	}
	if (distance(iter, strBuffer.end()) < FRAME_MIN_LEN)
		return 0;
	S_BYTE Start = (S_BYTE)*iter; advance(iter, 1);
	S_BYTE Len_H = (S_BYTE)*iter; advance(iter, 1);
	S_BYTE Len_L = (S_BYTE)*iter; advance(iter, 1);
	S_WORD DataLen = Len_H*0x100+Len_L;
	if (Start != FRAME_STX || DataLen < 2)
	{
		logwt<<"RFTX Э��֡ͷ�쳣 Start ="<<HEX<<Start<<",DataLen ="<<DataLen<<endl;
		return -1;
	}
	
	if(DataLen != strBuffer.size()-5)
	{
		return -1;
	}

	S_BYTE RealBCC = *iter;
	
//	RealBCC ^= Start;
//	RealBCC ^= Len_H;
//	RealBCC ^= Len_L;
	
	for(int i = 1; i< DataLen; i++)
	{
		RealBCC ^= *(iter+i);
	}

	advance(iter, DataLen);
	
	S_BYTE StoreBCC = (S_BYTE)*iter; advance(iter, 1);
	if (StoreBCC != RealBCC)
	{
		logwt<<"RFTX Э��֡У���쳣,����CS["<<HEX<<RealBCC<<"],ԴCS["<<StoreBCC<<"]!"<<endl;
		return -1;
	}
	S_BYTE Stop = (S_BYTE)*iter; advance(iter, 1);
	
	if (Stop != (S_BYTE)FRAME_ETX)
	{
		logwt<<"RFTX Э��֡����������["<<HEX<<Stop<<"]!"<<endl;
		return -1;
	}

	strCmd.append(strBuffer.begin(), iter);
	return (int)distance(strBuffer.begin(), iter);
}


/** @brief �����������֡(�ڲ�ʹ��)
 *  @param[in] strCmd ��������֡,��Ϊ�����������
 *  @param[in] CommReg ͨ�üĴ���
 *  @param[out] strData �����������
 *  @return <0: ����[����ֵ�ǳ�����ֽ���]; =0: ��֡; >0: �ɹ�
 */
int C_RFIDCom::ParseReadFrame(string &strCmd, string &strData)
{
	string::iterator iter = strCmd.begin();
	advance(iter, 1);	//head
	S_CHAR Len_H = (S_BYTE)*iter; advance(iter, 1);//���ȸ��ֽ�
	S_CHAR Len_L = (S_BYTE)*iter; advance(iter, 1);//���ȵ��ֽ�
	int Len = Len_H*0x100+Len_L;//�ܳ���
	S_CHAR Status_H = (S_BYTE)*iter; advance(iter, 1);//״̬���ֽ�
	S_CHAR Status_L = (S_BYTE)*iter; advance(iter, 1);//״̬���ֽ�
	if (Len == 0)
	{
		logwt<<"RFTX ͨ�Ź�Լ"<<HEX<<Len<<"�յ����ݳ���Ϊ0��֡!"<<endl;
		return -1;
	}
	
	if (Status_H == 0x00 && Status_L == 0xF0)//������(Ӧ���͵ı����ڴ�ģ��У�鲻��)
	{
		logwt<<"֡��������"<<endl;
		return FRAME_PARAM_ERR;
	}
	else if(Status_H == 0x00 && Status_L == 0xF1)
	{
		logwt<<"֡���ȴ���"<<endl;
		return FRAME_LEN_ERR;
	}
	else if(Status_H == 0x00 && Status_L == 0xF4)
	{
		logwt<<"֡У�����"<<endl;
		return FRAME_BCC_ERR;
	}
	else if(Status_H == 0x00 && Status_L == 0xF5)
	{
		logwt<<"��������"<<endl;
		return FRAME_ERR;
	}
	else if(Status_H == 0x00 && Status_L == 0x01)
	{
		return FRAME_AUTH_FAIL;
	}
	else if(Status_H == 0x00 && Status_L == 0x02)
	{
		return FRAME_METER_ERR;
	}
	else if(Status_H == 0x00 && Status_L == 0x00)
	{
		strData.append(iter,strCmd.end()-2);
		
		logwt<<"RFTX ���ص�Ӧ�ò����� strData = "<<HEX<<strData<<"Len = "<<strData.size()<<" !"<<endl;
	}
	else
	{
		return FRAME_INVAILD;
	}

	return FRAME_VAILD;
}

/** @brief ���ò�����
*  @return -1 ����0 �ɹ�
*/
int C_RFIDCom::CommParamSet(void)
{	//����������Ҫ�Ļ�����ͨ�������ļ���������
	int result = -1;
	S_WORD CmdParam = 0x3001;
	string SstrData;
	string RstrData;
	S_CHAR BaudRate = 0x04; //0x00-9600; 0x01-19200; 0x02-38400; 0x03-57600; 0x04-115200;
	
	SstrData.push_back(BaudRate);

	result = Read(CmdParam,SstrData,RstrData);

	if(result == 0)
	{	
		S_DWORD BR = 0x115200;
		switch(BaudRate)
		{
			case 0x00:BR = 0x9600;break;
			case 0x01:BR = 19200;break;
			case 0x02:BR = 38400;break;
			case 0x03:BR = 57600;break;
			case 0x04:BR = 0x115200;break;
			default:  BR = 0x115200;break;
		}
		//�����ʲ������óɹ�
		logwt<<"RFTX ͨ�Ų������óɹ� BaudRate = "<<HEX<<BR<<"!"<<endl;
	}
	
	if(result == 0xf0)
	{
		//���ݲ�������
		logwt<<"RFTX ͨ�����ݲ�������!"<<endl;
	}
	
	if(result == 0x01)
	{
		//��֧�ָò�����
		logwt<<"RFTX ��֧�ָò�����!"<<endl;
	}

	return result;
}


