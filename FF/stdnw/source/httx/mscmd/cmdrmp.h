/** @file
 *  @brief ��̨ͨѶ������������������ʵ��
 *  @author  zhongguangxiong
 *  @date 2012/02/18
 *  @version 0.1
 */
#ifndef __NW_CMDRMP_H__
#define __NW_CMDRMP_H__
#include "nwmscmd.h"

/** @brief ����������������Ļ��� */
class C_RMCmdReceiver:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};

/** @brief ��������� */
class C_CmdrE0800000:public C_RMCmdReceiver //�������״̬ 
{
public:
	virtual char* Prompt(void){return "���������״̬";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE0800001:public C_RMCmdReceiver //����������
{
public:
	virtual char* Prompt(void){return "������������";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE0800002:public C_RMCmdReceiver //�������ַ
{
public:
	virtual char* Prompt(void){return "���������ַ";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE0800003:public C_RMCmdReceiver //������ͨ�Ź�Լ
{
public:
	virtual char* Prompt(void){return "��������ͨ�Ź�Լ";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE0800004:public C_RMCmdReceiver //���ܱ�����
{
public:
	virtual char* Prompt(void){return "�����ܱ�����";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE0800005:public C_RMCmdReceiver //�ܷ�����
{
public:
	virtual char* Prompt(void){return "���������ܷ�����";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE0800006:public C_RMCmdReceiver //�ص��û�����
{
public:
	virtual char* Prompt(void){return "���ص��û�����";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE0800007:public C_RMCmdReceiver //��բ����
{
public:
	virtual char* Prompt(void){return "����������բ����";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE0800008:public C_RMCmdReceiver //��������
{
public:
	virtual char* Prompt(void){return "����������������";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE0800009:public C_RMCmdReceiver //�������Ӧ�ɼ��ն˵�ַ
{
public:
	virtual char* Prompt(void){return "���������Ӧ�ɼ��ն˵�ַ";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE080000A:public C_RMCmdReceiver //������˿ں�
{
public:
	virtual char* Prompt(void){return "��������˿ں�";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE080000B:public C_RMCmdReceiver //�˿ڲ���
{
public:
	virtual char* Prompt(void){return "���˿ڲ���";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE080000C:public C_RMCmdReceiver //CT���
{
public:
	virtual char* Prompt(void){return "��CT���";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE080000D:public C_RMCmdReceiver //PT���
{
public:
	virtual char* Prompt(void){return "��PT���";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE080000F:public C_RMCmdReceiver //�����������
{
public:
	virtual char* Prompt(void){return "�����������ݼ���";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE0800010:public C_RMCmdReceiver //��������߷�ʽ
{
public:
	virtual char* Prompt(void){return "����������߷�ʽ";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE0800011:public C_RMCmdReceiver //���ѹ(���β�)
{
public:
	virtual char* Prompt(void){return "�����ѹ";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE0800012:public C_RMCmdReceiver //�����
{
public:
	virtual char* Prompt(void){return "�������";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE0800013:public C_RMCmdReceiver //�û��걨����
{
public:
	virtual char* Prompt(void){return "���û��걨����";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE0800014:public C_RMCmdReceiver //��ѹ������
{
public:
	virtual char* Prompt(void){return "����ѹ������";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE080001F:public C_RMCmdReceiver //�����������
{
public:
	virtual char* Prompt(void){return "�������������";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE0800021:public C_RMCmdReceiver //���ܱ�ͨ������
{
public:
	virtual char* Prompt(void){return "�����ܱ�ͨ������";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE0800022:public C_RMCmdReceiver //���ܱ�ͨ���û���
{
public:
	virtual char* Prompt(void){return "�����ܱ�ͨ���û���";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
class C_CmdrE080002F:public C_RMCmdReceiver //�������ݼ���
{
public:
	virtual char* Prompt(void){return "���������ݼ���";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};

//������ƽ�����ֵ
class C_CmdrE0800100:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "������ƽ�����ֵ";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//������ƽ��Ȼָ���ֵ
class C_CmdrE0800101:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "������ƽ��Ȼָ���ֵ";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//�ϸ��ѹ����
class C_CmdrE0800102:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "�ϸ��ѹ����";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//�ϸ��ѹ����
class C_CmdrE0800103:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "�ϸ��ѹ����";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//�жϵ�����������Զֵ�ı���
class C_CmdrE0800104:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "�жϵ�����������Զֵ�ı���";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//�жϸ��ɹ�����Զֵ�ı���
class C_CmdrE0800105:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "�жϸ��ɹ�����Զֵ�ı���";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//�жϸ��ɹ��ػָ�����Զֵ�ı���
class C_CmdrE0800106:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "�жϸ��ɹ��ػָ�����Զֵ�ı���";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//�������������ֵ
class C_CmdrE0800107:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "�������������ֵ";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//������������ָ���ֵ
class C_CmdrE0800108:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "������������ָ���ֵ";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//�жϵ��������ɻָ�����Զֵ�ı���
class C_CmdrE0800109:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "�жϵ��������ɻָ�����Զֵ�ı���";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//��ѹ��ƽ�����ֵ
class C_CmdrE080010A:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "��ѹ��ƽ�����ֵ";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//��ѹ��ƽ��Ȼ�
class C_CmdrE080010B:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "��ѹ��ƽ��Ȼ�";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//ʧѹ����౨�����ж���ֵ
class C_CmdrE080010C:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "ʧѹ����౨�����ж���ֵ";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//�����������
class C_CmdrE080010F:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "�����������";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//��ѹ��ƽ��Ȼ�
class C_CmdrE0800200:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "��ѹ��ƽ��Ȼ�";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//������������ֵ
class C_CmdrE0800201:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "������������ֵ";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//��ѹ�����ʱ����ָ���ֵ
class C_CmdrE0800202:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "��ѹ�����ʱ����ָ���ֵ";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//���������ʱ����ָ���ֵ
class C_CmdrE0800203:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "���������ʱ����ָ���ֵ";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//��������������
class C_CmdrE0800204:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "��������������";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//������������
class C_CmdrE0800205:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "������������";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//���г������
class C_CmdrE0800206:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "���г������";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//��ѹƫ������
class C_CmdrE0800207:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "��ѹƫ������";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//��ѹƫ������
class C_CmdrE0800208:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "��ѹƫ������";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//Ƶ��ƫ������
class C_CmdrE0800209:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "Ƶ��ƫ������";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//Ƶ��ƫ������
class C_CmdrE080020A:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "Ƶ��ƫ������";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//��ѹ������ֵ
class C_CmdrE080020B:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "��ѹ������ֵ";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//��ѹ������ֵ
class C_CmdrE080020C:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "��ѹ������ֵ";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//�޹�������Ƿ�����澯������ֵ
class C_CmdrE080020D:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "�޹�������Ƿ�����澯������ֵ";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};
//�޹�������Ƿ�����澯�ָ���ֵ
class C_CmdrE080020E:public C_RMCmdReceiver
{
public:
	virtual char* Prompt(void){return "�޹�������Ƿ�����澯�ָ���ֵ";}

protected:
	virtual bool ReadDataByMP(S_WORD MP, string &strData);
};

#endif //__NW_CMDRMP_H__


