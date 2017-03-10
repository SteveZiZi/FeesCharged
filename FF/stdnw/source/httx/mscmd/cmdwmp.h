/** @file
 *  @brief ��̨ͨѶ���������д��������ʵ��
 *  @author  zhongguangxiong
 *  @date 2012/02/18
 *  @version 0.1
 */
#ifndef __NW_CMDWMP_H__
#define __NW_CMDWMP_H__
#include "nwmscmd.h"

/** @brief д��������� ����*/
#define WM_UPDATE_DBCJ_INFO				0x00000001UL
#define WM_UPDATE_METER_INFO				0x00000002UL
#define WM_UPDATE_NOTIFY_ADD_METER		0x00000004UL

class C_WMCmdReceiver:public C_CmdReceiver
{
public:
	bool UpdateInformation(void); // ͳһ�����·��Ĳ�����������Ϣ

protected:
	bool SetUpdateFlag(S_DWORD flag); // ������Ҫ���µı�־

private:
	static S_DWORD m_UpdateFlag;
};

/** @brief ��������� */
class C_CmdwE0800000:public C_WMCmdReceiver //�������״̬ 
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д�������״̬ ";}
};
class C_CmdwE0800001:public C_WMCmdReceiver //����������
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д����������";}
protected:
	bool AddMeterAC(S_WORD SN);
	bool AddMeterRS485(S_WORD SN);
	bool DeleteMeasurePoint(S_WORD MP);
	bool DeleteMeter(S_WORD SN); // ���
	bool DeleteAnalog(S_WORD SN); // ģ����
	bool DeletePulse(S_WORD SN); // ����
	bool DeleteSUM(S_WORD SN); // ������
	bool DeleteAC(S_WORD SN); // ����	
};
class C_CmdwE0800002:public C_WMCmdReceiver //�������ַ
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д�������ַ";}
};
class C_CmdwE0800003:public C_WMCmdReceiver //������ͨ�Ź�Լ
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д������ͨ�Ź�Լ";}
};
class C_CmdwE0800004:public C_WMCmdReceiver //���ܱ�����
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д���ܱ�����";}
};
class C_CmdwE0800005:public C_WMCmdReceiver //�ܷ�����
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д�������ܷ�����";}
};
class C_CmdwE0800006:public C_WMCmdReceiver //�ص��û�����
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д�ص��û�����";}
};
class C_CmdwE0800007:public C_WMCmdReceiver //��բ����
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д��������բ����";}
};
class C_CmdwE0800008:public C_WMCmdReceiver //��������
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д��������������";}
};
class C_CmdwE0800009:public C_WMCmdReceiver //�������Ӧ�ɼ��ն˵�ַ
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д�������Ӧ�ɼ��ն˵�ַ";}
};
class C_CmdwE080000A:public C_WMCmdReceiver //������˿ں�
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д������˿ں�";}
};
class C_CmdwE080000B:public C_WMCmdReceiver //�˿ڲ���
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д������˿ڲ���";}
};
class C_CmdwE080000C:public C_WMCmdReceiver //CT���
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д������CT���";}
};
class C_CmdwE080000D:public C_WMCmdReceiver //PT���
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д������PT���";}
};
class C_CmdwE080000F:public C_WMCmdReceiver //�����������
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д�������������";}
private:
	bool SetMeterParam(INT16U MP, const string &strMeterParam);
};

class C_CmdwE0800010:public C_WMCmdReceiver //��������߷�ʽ
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);	
	virtual char* Prompt(void){return "����������߷�ʽ";}

	bool SetMeterParam(INT16U MP, const string &strMeterParam);
};
class C_CmdwE0800011:public C_WMCmdReceiver //���ѹ(���β�)
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����ѹ";}
};
class C_CmdwE0800012:public C_WMCmdReceiver //�����
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д�����";}
};
class C_CmdwE0800013:public C_WMCmdReceiver //�û��걨����
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���û��걨����";}
};
class C_CmdwE0800014:public C_WMCmdReceiver //��ѹ������
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ѹ������";}
};
class C_CmdwE080001F:public C_WMCmdReceiver //�����������
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	bool SetMeterParam(INT16U MP, const string &strMeterParam);
	virtual char* Prompt(void){return "�������������";}
};
class C_CmdwE0800021:public C_WMCmdReceiver //���ܱ�ͨ������
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����ܱ�ͨ������";}
};
class C_CmdwE0800022:public C_WMCmdReceiver //���ܱ�ͨ���û���
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����ܱ�ͨ���û���";}
};
class C_CmdwE080002F:public C_WMCmdReceiver //�������ݼ���
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���������ݼ���";}
};

//������ƽ�����ֵ
class C_CmdwE0800100:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д������ƽ�����ֵ";}
};
//������ƽ��Ȼָ���ֵ
class C_CmdwE0800101:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "������ƽ��Ȼָ���ֵ";}
};
//�ϸ��ѹ����
class C_CmdwE0800102:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�ϸ��ѹ����";}
};
//�ϸ��ѹ����
class C_CmdwE0800103:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�ϸ��ѹ����";}
};
//�жϵ�����������Զֵ�ı���
class C_CmdwE0800104:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�жϵ�����������Զֵ�ı���";}
};
//�жϸ��ɹ�����Զֵ�ı���
class C_CmdwE0800105:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�жϸ��ɹ�����Զֵ�ı���";}
};
//�жϸ��ɹ��ػָ�����Զֵ�ı���
class C_CmdwE0800106:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�жϸ��ɹ��ػָ�����Զֵ�ı���";}
};
//�������������ֵ
class C_CmdwE0800107:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�������������ֵ";}
};
//������������ָ���ֵ
class C_CmdwE0800108:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "������������ָ���ֵ";}
};
//�жϵ��������ɻָ�����Զֵ�ı���
class C_CmdwE0800109:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�жϵ��������ɻָ�����Զֵ�ı���";}
};
//��ѹ��ƽ�����ֵ
class C_CmdwE080010A:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��ѹ��ƽ�����ֵ";}
};
//��ѹ��ƽ��Ȼ�
class C_CmdwE080010B:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��ѹ��ƽ��Ȼ�";}
};
//ʧѹʧ������౨�����ж���ֵ
class C_CmdwE080010C:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "ʧѹ����౨�����ж���ֵ";}
};
//�����������
class C_CmdwE080010F:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�����������";}
};
//��ѹ��ƽ��Ȼ�
class C_CmdwE0800200:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��ѹ��ƽ��Ȼ�";}
};
//������������ֵ
class C_CmdwE0800201:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "������������ֵ";}
};
//��ѹ�����ʱ����ָ���ֵ
class C_CmdwE0800202:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��ѹ�����ʱ����ָ���ֵ";}
};
//���������ʱ����ָ���ֵ
class C_CmdwE0800203:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���������ʱ����ָ���ֵ";}
};
//��������������
class C_CmdwE0800204:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��������������";}
};
//������������
class C_CmdwE0800205:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "������������";}
};
//���г������
class C_CmdwE0800206:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���г������";}
};
//��ѹƫ������
class C_CmdwE0800207:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��ѹƫ������";}
};
//��ѹƫ������
class C_CmdwE0800208:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��ѹƫ������";}
};
//Ƶ��ƫ������
class C_CmdwE0800209:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "Ƶ��ƫ������";}
};
//Ƶ��ƫ������
class C_CmdwE080020A:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "Ƶ��ƫ������";}
};
//��ѹ������ֵ
class C_CmdwE080020B:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��ѹ������ֵ";}
};
//��ѹ������ֵ
class C_CmdwE080020C:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��ѹ������ֵ";}
};
//�޹�������Ƿ�����澯������ֵ
class C_CmdwE080020D:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�޹�������Ƿ�����澯������ֵ";}
};
//�޹�������Ƿ�����澯�ָ���ֵ
class C_CmdwE080020E:public C_WMCmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�޹�������Ƿ�����澯�ָ���ֵ";}
};

#endif//__NW_CMDWMP_H__


