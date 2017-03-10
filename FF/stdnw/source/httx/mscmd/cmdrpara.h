/** @file
 *  @brief ��̨ͨѶ���������������
 *  @author 
 *  @date 2011/12/22
 *  @version 0.1
 */
#ifndef __NW_CMDRPARA_H__
#define __NW_CMDRPARA_H__

#include "nwmscmd.h"
#include "status.h"
#include "busialgo.h"


//��վͨ�Ų�����
class C_CmdrE0000100:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����վͨ�Ų���";}
protected:
	bool ReadParam(string &strParam,S_CHANNEL *pMS);
};
//��վͨ�Ų�������1
class C_CmdrE0000101:public C_CmdrE0000100
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����վͨ�Ų�������1";}
};
//��վͨ�Ų�������2
class C_CmdrE0000102:public C_CmdrE0000100
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����վͨ�Ų�������2";}
};
//���ص�ַ
class C_CmdrE0000103:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�����ص�ַ";}
};
//APN
class C_CmdrE0000104:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��APN";}
};
//APN�û���
class C_CmdrE0000105:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��APN�û���";}
};
//APN����
class C_CmdrE0000106:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��APN����";}
};
//��������
class C_CmdrE0000107:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����������";}
};
//�����ز������ȱʡ30����
class C_CmdrE0000108:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�����ز����";}
};
//���ߺ��ز�������ȱʡ3��,�ز�ָ�������󣬵��첻���ز�
class C_CmdrE0000109:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ߺ��ز�����";}
};
//���ӷ�ʽ��0��TCP��ʽ��1��UDP��ʽ
class C_CmdrE000010A:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ӷ�ʽ��0��TCP��ʽ��1��UDP��ʽ";}
};
//ѹ������:0����ѹ��  1��ѹ����BIN����
class C_CmdrE000010B:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "ѹ������:0����ѹ��  1��ѹ����BIN����";}
};
//�������ݼ���
class C_CmdrE000010F:public C_CmdrE0000100
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�������ݼ���";}
private:
	bool GetAllParam(string &strAllParam);
};
//�ն˵���������
class C_CmdrE0000120:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�ն˵���������";}
};
//�ն˵�ַ
class C_CmdrE0000121:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�ն˵�ַ";}
};
//�ն�ͨ�Ź���ģʽ
class C_CmdrE0000122:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�ն�ͨ�Ź���ģʽ";}
};
//���ģʽ�ͷ�����ģʽ�£�TCP�����˿ںţ�BIN����
class C_CmdrE0000123:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "TCP�����˿ں�";}
};
//�ն˵�ǰͨ��IP��ַ
class C_CmdrE0000124:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�ն˵�ǰͨ��IP��ַ";}
};
//�ն˵�ǰͨ�����������ַ
class C_CmdrE0000125:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�ն˵�ǰͨ�����������ַ";}
};
//�ն˵�ǰͨ�����������ַ
class C_CmdrE0000126:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�ն˵�ǰͨ�����������ַ";}
};
//��������IP��ַ��ȡ��ʽ 0���ֶ�����IP��ַ 1���Զ�����IP��ַ
class C_CmdrE0000127:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��������IP��ַ��ȡ��ʽ";}
};
//�����������
class C_CmdrE000012F:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�����������";}
private:
	bool GetAllParam(string &strAllParam);
};
//�ն�ʱ��
class C_CmdrE0000130:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ն�ʱ��";}
};
//�澯�����ϱ�������
class C_CmdrE0000150:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���澯�����ϱ�������";}
};
//�澯�ж�������
class C_CmdrE0000151:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���澯�ж�������";}
};
//�¼���¼������
class C_CmdrE0000152:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���¼���¼������";}
};
//�¶������ DD������ʱ����գ���Χ1~30��ȱʡΪÿ��1�� HH������ʱ���Сʱ����Χ0~23��ȱʡΪ0��
class C_CmdrE0000160:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���¶����������ʱ��";}
};
//�ն������ HH���յ����������룩����ʱ�̣���Χ0~23��ȱʡΪ0��
class C_CmdrE0000161:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ն����������ʱ��";}
};
//�µ�ѹ�ϸ������ݶ��� DD������ʱ����գ���Χ1~30��ȱʡΪÿ��21��HH������ʱ���Сʱ����Χ0~23��ȱʡΪ0��
class C_CmdrE0000162:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���µ�ѹ�ϸ������ݶ���ʱ��";}
};
//�յ�ѹ�ϸ������ݶ��� HH���յ����������룩����ʱ�̣���Χ0~23��ȱʡΪ0��
class C_CmdrE0000163:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���յ�ѹ�ϸ������ݶ���ʱ��";}
};
//��Ϣ��֤���ֶ�PWУ�鷽ʽ (ֻд)
//class C_CmdrE0000170:public C_CmdReceiver
//{
//public:
//	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
//	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
//	virtual char* Prompt(void){return "��Ϣ��֤���ֶ�PWУ�鷽ʽ";}
//};
//��֤������Ϣ��BIN���룬����
class C_CmdrE0000171:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����֤������Ϣ";}
};
//��֤��Ӧ��Ϣ��BIN���룬����
class C_CmdrE0000172:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����֤��Ӧ��Ϣ";}
};
//�������Ϣ��BIN���룬����
class C_CmdrE0000173:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���������Ϣ";}
};
//�������Ӧ��Ϣ��BIN���룬����
class C_CmdrE0000174:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���������Ӧ��Ϣ";}
};
//�жϸ��ɹ��ء����������ɡ�����ѹA������ͬ�����õ硢���ʳ���ֵ��������ƽ��ȡ���ѹ���䡢�������䡢�������ƫ���޹�Ƿ�������޹���������ʱ��ȱʡ15����
class C_CmdrE0000180:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����ֵ���������޵�ʱ��";}
};
//�жϸ��ɹ��ء����������ɡ�����ѹ����ͬ�����õ硢���ʳ���ֵ��������ƽ��ȡ���ѹ���䡢�������䡢�������ƫ���޹�Ƿ�������޹��������ָ�ʱ��ȱʡ15����
class C_CmdrE0000181:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����ֵ���������޻ָ���ʱ��";}
};
//��ֵ�������ж϶��ࡢʧѹ��ʧ��ʱ�䣬ȱʡֵ1����
class C_CmdrE0000182:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����ֵ�������ж϶��ࡢʧѹ��ʧ��ʱ��";}
};
//��ֵ�������ж϶��ࡢʧѹ��ʧ���ָ�ʱ�䣬ȱʡֵ1����
class C_CmdrE0000183:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����ֵ�������ж϶��ࡢʧѹ��ʧ���ָ�ʱ��";}
};
//��ֵ�������жϵ�ѹ������ʱ�䣬ȱʡֵ15����
class C_CmdrE0000184:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����ֵ�������жϵ�ѹ������ʱ��";}
};
//��ֵ�������жϵ�ѹ������ָ�ʱ�䣬ȱʡֵ15����
class C_CmdrE0000185:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����ֵ�������жϵ�ѹ������ָ�ʱ��";}
};
//��ֵ�������жϵ���������ʱ�䣬ȱʡֵ15����
class C_CmdrE0000186:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����ֵ�������жϵ���������ʱ��";}
};
//��ֵ�������жϵ��������Իָ�ʱ�䣬ȱʡֵ15����
class C_CmdrE0000187:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����ֵ�������жϵ��������Իָ�ʱ��";}
};
//��ֵ���������ܱ�ʱ���쳣��ֵ��ȱʡֵ10����
class C_CmdrE0000188:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����ֵ���������ܱ�ʱ���쳣��ֵ";}
};
//��ֵ���������ܱ������ֵ��ȱʡΪ10��
class C_CmdrE0000189:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����ֵ���������ܱ������ֵ";}
};
//��ֵ���������ܱ�ͣ����ֵ������ʾ��ֹͣ����ʱ�����й����ʼ���Ӧ�ߵ���ֵ��Խ��ֵ����ܱ�ͣ�ߣ�ȱʡΪ0.1kWh
class C_CmdrE000018A:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����ֵ���������ܱ�ͣ����ֵ";}
};
//��ֵ������ʣ�������ֵ��ȱʡΪ20.00 Ԫ
class C_CmdrE000018B:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����ֵ������ʣ�������ֵ";}
};
//��ֵ��������ͨ������Խ��ֵ
class C_CmdrE000018C:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����ֵ��������ͨ������Խ��ֵ";}
};
//�����������
class C_CmdrE000018F:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����ֵ����,�����������";}
private:
	bool GetAllParam(string &strAllParam);
};
//��Ч�ĳ���������ñ����� (ֻ��)
class C_CmdrE0000221:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����Ч�ĳ���������ñ�����";}
};
//��������Զ�Ӧ�ĳ���������ñ�
class C_CmdrE0000222:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����������Զ�Ӧ�ĳ���������ñ�";}
};
//������ͨ�������� (ֻ��)
class C_CmdrE0000300:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��������ͨ��������";}
};
//�����м���������
class C_CmdrE0000400:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�������м���������";}
};
//ģ��������
class C_CmdrE0000500:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��ģ��������";}
};
//ģ����״̬���
class C_CmdrE00005FE:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��ģ����״̬���";}
};
//�������ݼ���
class C_CmdrE00005FF:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��ģ���� �������ݼ���";}
};
//����������
class C_CmdrE0000600:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "������������";}
};
//������״̬���
class C_CmdrE00006FE:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��������״̬���";}
};
//�������ݼ���
class C_CmdrE00006FF:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�������� �������ݼ���";}
};
//����������
class C_CmdrE0000700:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "������������";}
};
//������״̬���
class C_CmdrE00007FE:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��������״̬���";}
};
//�������ݼ���
class C_CmdrE00007FF:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�������� �������ݼ���";}
};
//���������
class C_CmdrE0000800:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�����������";}
};
//�����״̬���
class C_CmdrE00008FE:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�������״̬���";}
};
//�������ݼ���
class C_CmdrE00008FF:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��� �������ݼ���";}
};
//�ն��޹�����ͳ�Ʊ�־
class C_CmdrE0000900:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ն��޹�����ͳ�Ʊ�־";}
};
//�������ն˵�ַ1~4
class C_CmdrE0000A20:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���������ն˵�ַ";}
};
//���ն˵�ַ
class C_CmdrE0000A21:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�����ն˵�ַ";}
};
//�����ն˵�ַ
class C_CmdrE0000A22:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�������ն˵�ַ";}
};
//���ն˵ȴ����ն�Ӧ��ĳ�ʱ�ж�ʱ��
class C_CmdrE0000A23:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�����ն˵ȴ����ն�Ӧ��ĳ�ʱ�ж�ʱ��";}
};
//���ն˵ȴ����ն˼�����������ĳ�ʱ�ж�ʱ��
class C_CmdrE0000A24:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�����ն˵ȴ����ն˼�����������ĳ�ʱ�ж�ʱ��";}
};
//����̨���ն�CT����
class C_CmdrE0000A25:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "������̨���ն�CT����";}
};
//����̨���ն�PT����
class C_CmdrE0000A26:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "������̨���ն�PT����";}
};
//��ѯ
class C_CmdrE0000A30:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����ѯ";}
};
//�������
class C_CmdrE0000A31:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���������";}
};
//�ظ������������
class C_CmdrE0000A32:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ظ������������";}
};
//�����ն�̨���ܵ���
class C_CmdrE0000A33:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�������ն�̨���ܵ���";}
};
//��Լ�汾��
class C_CmdrE0000B00:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����Լ�汾��";}
};
//�ն�����汾��
class C_CmdrE0000B01:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ն�����汾��";}
};
//�ն�Ӳ���汾��
class C_CmdrE0000B02:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���ն�Ӳ���汾��";}
};
//�û��Զ�������CCCC
class C_CmdrE0002000:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���û��Զ�������CCCC";}
};

//��������������
//�������
class C_CmdrE0000200:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "��Ч����ͨ������";}
};
class C_CmdrE0000230:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "����������ñ�";}
};

//������� ��ͨ�����
class C_CmdrE0000301:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "������� ��ͨ�����";}
};
//������� �м������
class C_CmdrE0000401:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "������� �м������";}
};
//���������� ģ������
class C_CmdrE0000501:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���������� ģ������";}
};
//���������� ��������
class C_CmdrE0000601:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���������� ��������";}
};
//���������� ���������
class C_CmdrE0000701:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���������� ���������";}
};
//���������� �����
class C_CmdrE0000801:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "���������� �����";}
};
//�˿ڲ��� 485�˿ڶ���
class C_CmdrE0000A00:public C_CmdReceiver
{
public:
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
	virtual char* Prompt(void){return "�˿ڲ��� 485�˿ڶ���";}

protected:
	S_BYTE VComFunction(S_BYTE VCom);
};
/**/
#endif //__NW_CMDRPARA_H__

