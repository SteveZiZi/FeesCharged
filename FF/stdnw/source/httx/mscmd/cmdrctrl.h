/** @file
 *  @brief ��̨ͨѶ�����д��������ʵ��
 *  @author  zhongguangxiong
 *  @date 2012/02/18
 *  @version 0.1
 */
#ifndef __NW_CMDRCTRL_H__
#define __NW_CMDRCTRL_H__
#include "nwmscmd.h"

//ң����բʱ��
class C_CmdrE0000C40:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "ң����բʱ��";}
};
//����״̬��01����״̬��00�Ǳ���״̬
class C_CmdrE0000C50:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����״̬";}
};
//���ʿ����Ƿ���Ч
class C_CmdrE0000C60:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ʿ����Ƿ���Ч";}
};
//�¶ȵ��������Ƿ���Ч
class C_CmdrE0000C61:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�¶ȵ��������Ƿ���Ч";}
};
//�����������Ƿ���Ч
class C_CmdrE0000C62:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�����������Ƿ���Ч";}
};
//���ʿ��Ƹ��ִ���բ�Ƿ���Ч
class C_CmdrE0000C63:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ʿ��Ƹ��ִ���բ�Ƿ���Ч";}
};
//�µ���������բ�ִ���Ч��־
class C_CmdrE0000C64:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�µ���������բ�ִ���Ч��־";}
};
//������������բ�ִ���Ч��־
class C_CmdrE0000C65:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "������������բ�ִ���Ч��־";}
};
//�����趨ֵ��բ�Ĺ����ɳ���ʱ��
class C_CmdrE0000C66:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�����趨ֵ��բ�Ĺ����ɳ���ʱ��";}
};
//�����趨ֵ��բ�ָ��ĸ��ɻָ�����ʱ�䣬ȱʡ10����
class C_CmdrE0000C67:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�����趨ֵ��բ�ָ��ĸ��ɻָ�����ʱ��";}
};
//��բ����澯��ʱ��ȱʡ10����
class C_CmdrE0000C68:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��բ����澯��ʱ";}
};
//���ʿ����ù����ۼӱ�־
class C_CmdrE0000C69:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ʿ����ù����ۼӱ�־";}
};
//���������õ����ۼӱ�־
class C_CmdrE0000C6A:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���������õ����ۼӱ�־";}
};
//���ʶ�ֵ����ϵ��������ȱʡΪ0
class C_CmdrE0000C6B:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ʶ�ֵ����ϵ��������ȱʡΪ0";}
};
//�µ����ض�ֵ����ϵ��������ȱʡΪ00
class C_CmdrE0000C6C:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�µ����ض�ֵ����ϵ��������ȱʡΪ00";}
};
//��͸���
class C_CmdrE0000C6D:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��͸���";}
};
//X��ʱ�޵����Ч��ʶ
class C_CmdrE0000C70:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "X��ʱ�޵����Ч��ʶ";}
};
//��ʱ�޵�ļ��㻬��ʱ�䣬��λ����
class C_CmdrE0000C71:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��ʱ�޵�ļ��㻬��ʱ�䣬��λ����";}
};
//���ʿ���ϵ��1��ʱ����
class C_CmdrE0000D00:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ʿ���ϵ��1��ʱ����";}
};
//���ʿ���ϵ��1,ִ��ʱ��
class C_CmdrE0000D09:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ʿ���ϵ��1,ִ��ʱ��";}
};
//���ʿ���ϵ��1�������������
class C_CmdrE0000D0F:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ʿ���ϵ��1�������������";}
private:
	bool SetAllParam(const string &strAllParam);
};
//���ʿ��ƹ��ʶ�ֵϵ��2��ʱ����
class C_CmdrE0000D10:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ʿ��ƹ��ʶ�ֵϵ��2��ʱ����";}
};
//���ʿ���ϵ��2,ִ��ʱ��
class C_CmdrE0000D19:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ʿ���ϵ��2,ִ��ʱ��";}
};
//���ʿ���ϵ��2�������������
class C_CmdrE0000D1F:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ʿ���ϵ��2�������������";}
private:
	bool SetAllParam(const string &strAllParam);
};
//���ʿ��ƹ��ʶ�ֵϵ��3��ʱ����
class C_CmdrE0000D20:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ʿ��ƹ��ʶ�ֵϵ��3��ʱ����";}
};
//���ʿ���ϵ��3,ִ��ʱ��
class C_CmdrE0000D29:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ʿ���ϵ��3,ִ��ʱ��";}
};
//���ʿ���ϵ��3�������������
class C_CmdrE0000D2F:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ʿ���ϵ��3�������������";}
private:
	bool SetAllParam(const string &strAllParam);
};
//���ʿ��ƹ��ʶ�ֵϵ��4��ʱ����
class C_CmdrE0000D30:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ʿ��ƹ��ʶ�ֵϵ��3��ʱ����";}
};
//���ʿ���ϵ��4,ִ��ʱ��
class C_CmdrE0000D39:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ʿ���ϵ��3,ִ��ʱ��";}
};
//���ʿ���ϵ��4�������������
class C_CmdrE0000D3F:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ʿ���ϵ��3�������������";}
private:
	bool SetAllParam(const string &strAllParam);
};
//�µ������ƣ��µ����޶�
class C_CmdrE0000E00:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�µ������ƣ��µ����޶�";}
};
//�µ������ƣ��ִε���
class C_CmdrE0000E01:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�µ������ƣ��ִε���";}
};
//���������ƣ����ι�����
class C_CmdrE0000E02:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�µ������ƣ��µ����޶�";}
};
//���������ƣ�ʣ�����
class C_CmdrE0000E03:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�µ������ƣ��ִε���";}
};
//�ն˵���������ʱ�κͷ�����
class C_CmdrE0000E04:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�µ������ƣ��ִε���";}
};
//
class C_CmdrE0000E05:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�µ������ƣ��ִε���";}
};
//���ݿ��Ƿ�Ͷ��
class C_CmdrE0000E10:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ݿ��Ƿ�Ͷ��";}
};
//���ݿز���
class C_CmdrE0000E11:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ݿز���";}
};


//��������������
//���ز��� ���ؽӵ�����
class C_CmdrE0000C00:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ؽӵ�����";}
};
//���뿪��״̬(ֻ��)
class C_CmdrE0000C10:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���뿪��״̬";}
};
//�������״̬(ֻд)
//class C_CmdrE0000C20:public C_CmdReceiver
//{
//public:
//	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
//	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
//	virtual char* Prompt(void){return "�������״̬";}
//};
//��������ִ�
class C_CmdrE0000C30:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��������ִ�";}
};
//���ʿ���ϵ��1��ʱ��(1-8)
class C_CmdrE0000D01:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ʿ���ϵ��1��ʱ��(1-8)";}
};
//���ʿ���ϵ��2��ʱ��(1-8)
class C_CmdrE0000D11:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ʿ���ϵ��2��ʱ��(1-8)";}
};
//���ʿ���ϵ��3��ʱ��(1-8)
class C_CmdrE0000D21:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ʿ���ϵ��3��ʱ��(1-8)";}
};
//���ʿ���ϵ��4��ʱ��(1-8)
class C_CmdrE0000D31:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ʿ���ϵ��4��ʱ��(1-8)";}
};

#endif//__NW_CMDRCTRL_H__

