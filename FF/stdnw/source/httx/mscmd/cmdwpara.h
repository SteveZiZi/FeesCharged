/** @file
 *  @brief ��̨ͨѶ����д����������
 *  @author 
 *  @date 2011/12/22
 *  @version 0.1
 */
#ifndef __NW_CMDWPARA_H__
#define __NW_CMDWPARA_H__

#include "nwmscmd.h"
#include "sysbase.h"
#include "busialgo.h"
#include "status.h"

//��վͨ�Ų�����
class C_CmdwE0000100:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д��վͨ�Ų���";}
protected:
	bool WriteParam(const S_CmdObj &CmdObj,S_CHANNEL *pMS);

};
//��վͨ�Ų�������1
class C_CmdwE0000101:public C_CmdwE0000100
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д��վͨ�Ų�������1";}
};
//��վͨ�Ų�������2
class C_CmdwE0000102:public C_CmdwE0000100
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д��վͨ�Ų�������2";}
};
//���ص�ַ
class C_CmdwE0000103:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д���ص�ַ";}
};
//APN
class C_CmdwE0000104:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "дAPN";}
};
//APN�û���
class C_CmdwE0000105:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "дAPN�û���";}
};
//APN����
class C_CmdwE0000106:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "дAPN����";}
};
//��������
class C_CmdwE0000107:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д��������";}
};
//�����ز����
class C_CmdwE0000108:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д�����ز����";}
};
//���ߺ��ز�������ȱʡ3��,�ز�ָ�������󣬵��첻���ز�
class C_CmdwE0000109:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д���ߺ��ز�����";}
};
//���ӷ�ʽ
class C_CmdwE000010A:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д���ӷ�ʽ,0��TCP��ʽ��1��UDP��ʽ";}
};
//ѹ������:0����ѹ��  1��ѹ����BIN����
class C_CmdwE000010B:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "дѹ������:0����ѹ��  1��ѹ����BIN����";}
};
//�������ݼ���
class C_CmdwE000010F:public C_CmdwE0000100
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "дͨ�Ų������������ݼ���";}
private:
	bool SetAllParam(const string &strAllParam);
};
//�ն˵���������
class C_CmdwE0000120:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�ն˵���������";}
};
//�ն˵�ַ
class C_CmdwE0000121:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�ն˵�ַ";}
};
//�ն�ͨ�Ź���ģʽ
class C_CmdwE0000122:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�ն�ͨ�Ź���ģʽ";}
};
//���ģʽ�ͷ�����ģʽ�£�TCP�����˿ںţ�BIN����
class C_CmdwE0000123:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "TCP�����˿ں�";}
};
//�ն˵�ǰͨ��IP��ַ
class C_CmdwE0000124:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�ն˵�ǰͨ��IP��ַ";}
};
//�ն˵�ǰͨ�����������ַ
class C_CmdwE0000125:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�ն˵�ǰͨ�����������ַ";}
};
//�ն˵�ǰͨ�����������ַ
class C_CmdwE0000126:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�ն˵�ǰͨ�����������ַ";}
};
//��������IP��ַ��ȡ��ʽ
class C_CmdwE0000127:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��������IP��ַ��ȡ��ʽ";}
};
//�����������
class C_CmdwE000012F:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "������Ϣ�������������";}
private:
	bool SetAllParam(const string &strAllParam);
};
//�ն�ʱ��
class C_CmdwE0000130:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д�ն�ʱ��";}
};
//ʵʱУʱ
class C_CmdwE0000131:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "ʵʱУʱ";}
};
//�ն˸�λ
class C_CmdwE0000140:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�ն˸�λ����";}
};
//�澯�����ϱ�������
class C_CmdwE0000150:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д�澯�����ϱ�������";}
};
//�澯�ж�������
class C_CmdwE0000151:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д�澯�ж�������";}
};
//�¼���¼������
class C_CmdwE0000152:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д�¼���¼������";}
};
//�¶������ DD������ʱ����գ���Χ1~30��ȱʡΪÿ��1�� HH������ʱ���Сʱ����Χ0~23��ȱʡΪ0��
class C_CmdwE0000160:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д�¶������";}
};
//�ն������ HH���յ����������룩����ʱ�̣���Χ0~23��ȱʡΪ0��
class C_CmdwE0000161:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д�ն������";}
};
//�µ�ѹ�ϸ������ݶ��� DD������ʱ����գ���Χ1~30��ȱʡΪÿ��21��HH������ʱ���Сʱ����Χ0~23��ȱʡΪ0��
class C_CmdwE0000162:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д�µ�ѹ�ϸ������ݶ���";}
};
//�յ�ѹ�ϸ������ݶ��� HH���յ����������룩����ʱ�̣���Χ0~23��ȱʡΪ0��
class C_CmdwE0000163:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д�յ�ѹ�ϸ������ݶ���";}
};
//��Ϣ��֤���ֶ�PWУ�鷽ʽ (ֻд)
class C_CmdwE0000170:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д��Ϣ��֤���ֶ�PWУ�鷽ʽ";}
};
//��֤������Ϣ��BIN���룬����
class C_CmdwE0000171:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��֤������Ϣ";}
};
//��֤��Ӧ��Ϣ��BIN���룬����
class C_CmdwE0000172:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��֤��Ӧ��Ϣ";}
};
//�������Ϣ��BIN���룬����
class C_CmdwE0000173:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�������Ϣ";}
};
//�������Ӧ��Ϣ��BIN���룬����
class C_CmdwE0000174:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�������Ӧ��Ϣ";}
};
//�жϸ��ɹ��ء����������ɡ�����ѹA������ͬ�����õ硢���ʳ���ֵ��������ƽ��ȡ���ѹ���䡢�������䡢�������ƫ���޹�Ƿ�������޹���������ʱ��ȱʡ15����
class C_CmdwE0000180:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д��ֵ���������޵�ʱ��";}
};
//�жϸ��ɹ��ء����������ɡ�����ѹ����ͬ�����õ硢���ʳ���ֵ��������ƽ��ȡ���ѹ���䡢�������䡢�������ƫ���޹�Ƿ�������޹��������ָ�ʱ��ȱʡ15����
class C_CmdwE0000181:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д��ֵ���������޻ָ���ʱ��";}
};
//��ֵ�������ж϶��ࡢʧѹ��ʧ��ʱ�䣬ȱʡֵ1����
class C_CmdwE0000182:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д��ֵ�������ж϶��ࡢʧѹ��ʧ��ʱ��";}
};
//��ֵ�������ж϶��ࡢʧѹ��ʧ���ָ�ʱ�䣬ȱʡֵ1����
class C_CmdwE0000183:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д��ֵ�������ж϶��ࡢʧѹ��ʧ���ָ�ʱ��";}
};
//��ֵ�������жϵ�ѹ������ʱ�䣬ȱʡֵ15����
class C_CmdwE0000184:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д��ֵ�������жϵ�ѹ������ʱ��";}
};
//��ֵ�������жϵ�ѹ������ָ�ʱ�䣬ȱʡֵ15����
class C_CmdwE0000185:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д��ֵ�������жϵ�ѹ������ָ�ʱ��";}
};
//��ֵ�������жϵ���������ʱ�䣬ȱʡֵ15����
class C_CmdwE0000186:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д��ֵ�������жϵ���������ʱ��";}
};
//��ֵ�������жϵ��������Իָ�ʱ�䣬ȱʡֵ15����
class C_CmdwE0000187:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д��ֵ�������жϵ��������Իָ�ʱ��";}
};
//��ֵ���������ܱ�ʱ���쳣��ֵ��ȱʡֵ10����
class C_CmdwE0000188:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д��ֵ���������ܱ�ʱ���쳣��ֵ";}
};
//��ֵ���������ܱ������ֵ��ȱʡΪ10��
class C_CmdwE0000189:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д��ֵ���������ܱ������ֵ";}
};
//��ֵ���������ܱ�ͣ����ֵ������ʾ��ֹͣ����ʱ�����й����ʼ���Ӧ�ߵ���ֵ��Խ��ֵ����ܱ�ͣ�ߣ�ȱʡΪ0.1kWh
class C_CmdwE000018A:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д��ֵ���������ܱ�ͣ����ֵ";}
};
//��ֵ������ʣ�������ֵ��ȱʡΪ20.00 Ԫ
class C_CmdwE000018B:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д��ֵ������ʣ�������ֵ";}
};
//��ֵ��������ͨ������Խ��ֵ
class C_CmdwE000018C:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д��ֵ��������ͨ������Խ��ֵ";}
};
//�����������
class C_CmdwE000018F:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д��ֵ����,�����������";}
private:
	bool SetAllParam(const string &strAllParam);
};
//��Ч�ĳ���������ñ����� (ֻ��)
//class C_CmdwE0000221:public C_CmdReceiver
//{
//public:
//	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
//	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
//	virtual char* Prompt(void){return "��Ч�ĳ���������ñ�����";}
//};
//��������Զ�Ӧ�ĳ���������ñ�
class C_CmdwE0000222:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д��������Զ�Ӧ�ĳ���������ñ�";}
};
//������ͨ�������� (ֻ��)
//class C_CmdwE0000300:public C_CmdReceiver
//{
//public:
//	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
//	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
//	virtual char* Prompt(void){return "������ͨ��������";}
//};
//�����м��������� (ֻ��)
//class C_CmdwE0000400:public C_CmdReceiver
//{
//public:
//	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
//	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
//	virtual char* Prompt(void){return "�����м���������";}
//};
//ģ��������
class C_CmdwE0000500:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "дģ��������";}
};
//ģ����״̬���
class C_CmdwE00005FE:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "дģ����״̬���";}
};
//�������ݼ���
class C_CmdwE00005FF:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "дģ���� �������ݼ���";}
};
//����������
class C_CmdwE0000600:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д����������";}
};
//������״̬���
class C_CmdwE00006FE:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д������״̬���";}
};
//�������ݼ���
class C_CmdwE00006FF:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д������ �������ݼ���";}
};
//����������
class C_CmdwE0000700:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д����������";}
};
//������״̬���
class C_CmdwE00007FE:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д������״̬���";}
};
//�������ݼ���
class C_CmdwE00007FF:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д������ �������ݼ���";}
};
//���������
class C_CmdwE0000800:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д���������";}
};
//�����״̬���
class C_CmdwE00008FE:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д�����״̬���";}
};
//�������ݼ���
class C_CmdwE00008FF:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д� �������ݼ���";}
};
//�ն��޹�����ͳ�Ʊ�־
class C_CmdwE0000900:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д�ն��޹�����ͳ�Ʊ�־";}
};
//�������ն˵�ַ1~4
class C_CmdwE0000A20:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д�������ն˵�ַ";}
};
//���ն˵�ַ
class C_CmdwE0000A21:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д���ն˵�ַ";}
};
//�����ն˵�ַ
class C_CmdwE0000A22:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д�����ն˵�ַ";}
};
//���ն˵ȴ����ն�Ӧ��ĳ�ʱ�ж�ʱ��
class C_CmdwE0000A23:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д���ն˵ȴ����ն�Ӧ��ĳ�ʱ�ж�ʱ��";}
};
//���ն˵ȴ����ն˼�����������ĳ�ʱ�ж�ʱ��
class C_CmdwE0000A24:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д���ն˵ȴ����ն˼�����������ĳ�ʱ�ж�ʱ��";}
};
//����̨���ն�CT����
class C_CmdwE0000A25:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д����̨���ն�CT����";}
};
//����̨���ն�PT����
class C_CmdwE0000A26:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д����̨���ն�PT����";}
};
//�Ե��ܱ�ң����բ
class C_CmdwE0001100:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д�Ե��ܱ�ң����բ";}
};
//�Ե��ܱ�ң�غ�բ
class C_CmdwE0001101:public C_CmdwE0001100
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д�Ե��ܱ�ң�غ�բ";}
};
//�Ե��ܱ������
class C_CmdwE0001102:public C_CmdwE0001100
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д�Ե��ܱ������";}
};
//�Ե��ܱ���������
class C_CmdwE0001103:public C_CmdwE0001100
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д�Ե��ܱ���������";}
};
//ɾ��������NNNN��ʾ��ɾ���������
class C_CmdwE0001104:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "дɾ��������";}
};
//�û��Զ�������CCCC
class C_CmdwE0002000:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "д�û��Զ�������CCCC";}
};


//��������������
//�������
class C_CmdwE0000200:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��Ч����ͨ������";}
};
class C_CmdwE0000230:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����������ñ�";}
};
//������� ��ͨ�����
class C_CmdwE0000301:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "������� ��ͨ�����";}
};
//������� �м������
class C_CmdwE0000401:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "������� �м������";}
};
//���������� ģ������
class C_CmdwE0000501:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���������� ģ������";}
};
//���������� ��������
class C_CmdwE0000601:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���������� ��������";}
};
//���������� ���������
class C_CmdwE0000701:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���������� ���������";}
};
//���������� �����
class C_CmdwE0000801:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���������� �����";}
};
//�˿ڲ��� 485�˿ڶ���
class C_CmdwE0000A00:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�˿ڲ��� 485�˿ڶ���";}

protected:
	int SetChannelParam(S_CHANNEL *pChannel, const string &strComSet);
};


#endif //__NW_CMDWPARA_H__

