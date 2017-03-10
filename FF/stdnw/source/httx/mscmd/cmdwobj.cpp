/** @file
 *  @brief ��̨ͨѶ����д���������弰����
 *  @author  zhongguangxiong
 *  @date 2012/02/18
 *  @version 0.1
 */
#include "cmdwobj.h"
#include "cmdwpara.h"
#include "cmdwctrl.h"
#include "cmdwmp.h"
#include "AlarmEventRecord.h"

//����������
//ͨ�Ų���
static C_CmdwE0000100 CmdwE0000100; //��վ����
static C_CmdwE0000101 CmdwE0000101; //��վ��������1
static C_CmdwE0000102 CmdwE0000102; //��վ��������2
static C_CmdwE0000103 CmdwE0000103; //����
static C_CmdwE0000104 CmdwE0000104; //APN
static C_CmdwE0000105 CmdwE0000105; //APN�û�
static C_CmdwE0000106 CmdwE0000106; //APN����
static C_CmdwE0000107 CmdwE0000107; //��������
static C_CmdwE0000108 CmdwE0000108; //�����ز����
static C_CmdwE0000109 CmdwE0000109; //���ߺ��ز�������ȱʡ3��,�ز�ָ�������󣬵��첻���ز�
static C_CmdwE000010A CmdwE000010A; //���ӷ�ʽ 0��TCP��ʽ��1��UDP��ʽ
static C_CmdwE000010B CmdwE000010B; //ѹ������:0����ѹ��  1��ѹ����BIN����
static C_CmdwE000010F CmdwE000010F; //�����������

static C_CmdwE0000120 CmdwE0000120; //�ն˵���������
static C_CmdwE0000121 CmdwE0000121; //�ն˵�ַ
static C_CmdwE0000122 CmdwE0000122; //�ն�ͨ�Ź���ģʽ
static C_CmdwE0000123 CmdwE0000123; //���ģʽ�ͷ�����ģʽ�£�TCP�����˿ںţ�BIN����
static C_CmdwE0000124 CmdwE0000124; //�ն˵�ǰͨ��IP��ַ
static C_CmdwE0000125 CmdwE0000125; //�ն˵�ǰͨ�����������ַ
static C_CmdwE0000126 CmdwE0000126; //�ն˵�ǰͨ�����ص�ַ
static C_CmdwE0000127 CmdwE0000127; //��������IP��ַ��ȡ��ʽ 0���ֶ�����IP��ַ 1���Զ�����IP��ַ
static C_CmdwE000012F CmdwE000012F; //�����������
//ʱ�����
static C_CmdwE0000130 CmdwE0000130; //�ն�ʱ��
static C_CmdwE0000131 CmdwE0000131; //ʵʱУʱ (ֻд)
//��λ����
static C_CmdwE0000140 CmdwE0000140; //�ն˸�λ (ֻд)
//�澯����
static C_CmdwE0000150 CmdwE0000150; //�澯�����ϱ�������
static C_CmdwE0000151 CmdwE0000151; //�澯�ж�������
static C_CmdwE0000152 CmdwE0000152; //�¼���¼������
//�����ղ���
static C_CmdwE0000160 CmdwE0000160; //����ʱ����ա�Сʱ�������¶�������͵�ѹ�ϸ��ʶ���ȣ� �¶������
static C_CmdwE0000161 CmdwE0000161; //�յ����������룩����ʱ�̣������յ�������͵�ѹ�ϸ��ʵȣ� �ն������
static C_CmdwE0000162 CmdwE0000162; //�µ�ѹ�ϸ������ݶ���
static C_CmdwE0000163 CmdwE0000163; //�յ�ѹ�ϸ������ݶ���
//�����֤
static C_CmdwE0000170 CmdwE0000170; //��Ϣ��֤���ֶ�PWУ�鷽ʽ (ֻд)
static C_CmdwE0000171 CmdwE0000171; //��֤������Ϣ��BIN���룬����
static C_CmdwE0000172 CmdwE0000172; //��֤��Ӧ��Ϣ��BIN���룬����
static C_CmdwE0000173 CmdwE0000173; //�������Ϣ��BIN���룬����
static C_CmdwE0000174 CmdwE0000174; //�������Ӧ��Ϣ��BIN���룬����
//��ֵ����
static C_CmdwE0000180 CmdwE0000180;//�жϸ��ɹ��ء����������ɡ�����ѹ������ͬ�����õ硢���ʳ���ֵ��������ƽ��ȡ���ѹ���䡢�������䡢�������ƫ���޹�Ƿ�������޹���������ʱ�䡢ȱʡ15����
static C_CmdwE0000181 CmdwE0000181;//�жϸ��ɹ��ء����������ɡ�����ѹ������ͬ�����õ硢���ʳ���ֵ��������ƽ��ȡ���ѹ���䡢�������䡢�������ƫ���޹�Ƿ�������޹��������ָ�ʱ�䡢ȱʡ15����
static C_CmdwE0000182 CmdwE0000182;//�ж϶��ࡢʧѹ��ʧ��ʱ�䣬ȱʡֵ1����
static C_CmdwE0000183 CmdwE0000183;//�ж϶��ࡢʧѹ��ʧ���ָ�ʱ�䣬ȱʡֵ1����
static C_CmdwE0000184 CmdwE0000184;//�жϵ�ѹ������ʱ�䣬ȱʡֵ15����
static C_CmdwE0000185 CmdwE0000185;//�жϵ�ѹ������ָ�ʱ�䣬ȱʡֵ15����
static C_CmdwE0000186 CmdwE0000186;//�жϵ���������ʱ�䣬ȱʡֵ15����
static C_CmdwE0000187 CmdwE0000187;//�жϵ��������Իָ�ʱ�䣬ȱʡֵ15����
static C_CmdwE0000188 CmdwE0000188;//���ܱ�ʱ���쳣��ֵ��ȱʡֵ10����
static C_CmdwE0000189 CmdwE0000189;//���ܱ������ֵ��ȱʡΪ10��
static C_CmdwE000018A CmdwE000018A;//���ܱ�ͣ����ֵ ȱʡΪ0.1kWh
static C_CmdwE000018B CmdwE000018B;//ʣ�������ֵ��ȱʡΪ20.00 Ԫ
static C_CmdwE000018C CmdwE000018C;//��ͨ������Խ��ֵ
static C_CmdwE000018F CmdwE000018F;//�������ݼ���
//�������
//static C_CmdwE0000221 CmdwE0000221;//��Ч�ĳ���������ñ����� (ֻ��)
static C_CmdwE0000222 CmdwE0000222;//��������Զ�Ӧ�ĳ���������ñ�
//�������
//static C_CmdwE0000300 CmdwE0000300;//������ͨ�������� (ֻ��)

//static C_CmdwE0000400 CmdwE0000400;//�����м��������� (ֻ��)
//����������
static C_CmdwE0000500 CmdwE0000500;//ģ��������
static C_CmdwE00005FE CmdwE00005FE;//ģ����״̬���
static C_CmdwE00005FF CmdwE00005FF;//�������ݼ���
//����������
static C_CmdwE0000600 CmdwE0000600;//����������
static C_CmdwE00006FE CmdwE00006FE;//������״̬���
static C_CmdwE00006FF CmdwE00006FF;//�������ݼ���
//����������
static C_CmdwE0000700 CmdwE0000700;//����������
static C_CmdwE00007FE CmdwE00007FE;//������״̬���
static C_CmdwE00007FF CmdwE00007FF;//�������ݼ���
//�����
static C_CmdwE0000800 CmdwE0000800;//���������
static C_CmdwE00008FE CmdwE00008FE;//�����״̬���
static C_CmdwE00008FF CmdwE00008FF;//�������ݼ���

static C_CmdwE0000900 CmdwE0000900;//�ն��޹�����ͳ�Ʊ�־
//�˿ڲ���
//����ͨ�Ų���
static C_CmdwE0000A20 CmdwE0000A20;//�������ն˵�ַ1~4
static C_CmdwE0000A21 CmdwE0000A21;//���ն˵�ַ
static C_CmdwE0000A22 CmdwE0000A22;//�����ն˵�ַ
static C_CmdwE0000A23 CmdwE0000A23;//���ն˵ȴ����ն�Ӧ��ĳ�ʱ�ж�ʱ��
static C_CmdwE0000A24 CmdwE0000A24;//���ն˵ȴ����ն˼�����������ĳ�ʱ�ж�ʱ��
static C_CmdwE0000A25 CmdwE0000A25;//����̨���ն�CT����
static C_CmdwE0000A26 CmdwE0000A26;//����̨���ն�PT����
//��������
//static C_CmdwE0000A30 CmdwE0000A30;//��ѯ (ֻ��)
//static C_CmdwE0000A31 CmdwE0000A31;//������� (ֻ��)
//static C_CmdwE0000A32 CmdwE0000A32;//�ظ������������ (ֻ��)
//static C_CmdwE0000A33 CmdwE0000A33;//�����ն�̨���ܵ��� (ֻ��)
//�汾����
//static C_CmdwE0000B01 CmdwE0000B01;//��Լ�汾�ţ�����Լ�涨Ϊ0100  (ֻ��)
//static C_CmdwE0000B02 CmdwE0000B02;//�ն�����汾��  (ֻ��)
//static C_CmdwE0000B03 CmdwE0000B03;//�ն�Ӳ���汾��  (ֻ��)
//���ز���
static C_CmdwE0000C40 CmdwE0000C40;//ң����բʱ��
static C_CmdwE0000C50 CmdwE0000C50;//����״̬��01����״̬��00�Ǳ���״̬
static C_CmdwE0000C60 CmdwE0000C60;//���ʿ����Ƿ���Ч
static C_CmdwE0000C61 CmdwE0000C61;//�¶ȵ��������Ƿ���Ч 
static C_CmdwE0000C62 CmdwE0000C62;//�����������Ƿ���Ч 
static C_CmdwE0000C63 CmdwE0000C63;//���ʿ��Ƹ��ִ���բ�Ƿ���Ч��
static C_CmdwE0000C64 CmdwE0000C64;//�µ���������բ�ִ���Ч��־��
static C_CmdwE0000C65 CmdwE0000C65;//������������բ�ִ���Ч��־��
static C_CmdwE0000C66 CmdwE0000C66;//�����趨ֵ��բ�Ĺ����ɳ���ʱ�䣬	ȱʡ10����
static C_CmdwE0000C67 CmdwE0000C67;//�����趨ֵ��բ�ָ��ĸ��ɻָ�����ʱ�䣬ȱʡ10����
static C_CmdwE0000C68 CmdwE0000C68;//��բ����澯��ʱ��ȱʡ10����
static C_CmdwE0000C69 CmdwE0000C69;//���ʿ����ù����ۼӱ�־
static C_CmdwE0000C6A CmdwE0000C6A;//���������õ����ۼӱ�־
static C_CmdwE0000C6B CmdwE0000C6B;//���ʶ�ֵ����ϵ��������ȱʡΪ0
static C_CmdwE0000C6C CmdwE0000C6C;//�µ����ض�ֵ����ϵ��������ȱʡΪ00
static C_CmdwE0000C6D CmdwE0000C6D;//��͸���
static C_CmdwE0000C70 CmdwE0000C70;//X��ʱ�޵����Ч��ʶ
static C_CmdwE0000C71 CmdwE0000C71;//��ʱ�޵�ļ��㻬��ʱ�䣬��λ����

static C_CmdwE0000D00 CmdwE0000D00;//���ʿ���ϵ��1��ʱ����
static C_CmdwE0000D09 CmdwE0000D09;//���ʿ���ϵ��1,ִ��ʱ��
static C_CmdwE0000D0F CmdwE0000D0F;//���ʿ���ϵ��1���������ݺϼ�
static C_CmdwE0000D10 CmdwE0000D10;//���ʿ��ƹ��ʶ�ֵϵ��2,ʱ����
static C_CmdwE0000D19 CmdwE0000D19;//���ʿ���ϵ��2,ִ��ʱ��
static C_CmdwE0000D1F CmdwE0000D1F;//���ʿ���ϵ��2���������ݺϼ�
static C_CmdwE0000D20 CmdwE0000D20;//���ʿ��ƹ��ʶ�ֵϵ��3,ʱ����
static C_CmdwE0000D29 CmdwE0000D29;//���ʿ���ϵ��3,ִ��ʱ��
static C_CmdwE0000D2F CmdwE0000D2F;//���ʿ���ϵ��3���������ݺϼ�
static C_CmdwE0000D30 CmdwE0000D30;//���ʿ��ƹ��ʶ�ֵϵ��4,ʱ����
static C_CmdwE0000D39 CmdwE0000D39;//���ʿ���ϵ��4,ִ��ʱ��
static C_CmdwE0000D3F CmdwE0000D3F;//���ʿ���ϵ��4���������ݺϼ�
static C_CmdwE0000E00 CmdwE0000E00;//�µ������ƣ��µ����޶�
static C_CmdwE0000E01 CmdwE0000E01;//�µ������ƣ��ִε���
static C_CmdwE0000E02 CmdwE0000E02;//���������ƣ����ι�����
static C_CmdwE0000E03 CmdwE0000E03;//���������ƣ�ʣ�����
static C_CmdwE0000E04 CmdwE0000E04;//�ն˵���������ʱ�κͷ�����
static C_CmdwE0000E05 CmdwE0000E05;//
static C_CmdwE0000E10 CmdwE0000E10;//���ݿ��Ƿ�Ͷ��
static C_CmdwE0000E11 CmdwE0000E11;//���ݿز���
/* ������д
//��·�ӿ����ݱ�ʶ
static C_CmdwE0001000 CmdwE0001000;//��ʾ�ն˵�¼��NNNN����Լ�汾�� 
static C_CmdwE0001001 CmdwE0001001;//��ʾ�ն���������������
static C_CmdwE0001002 CmdwE0001002;//��ʾ��¼�˳����������� 
*/
//��������Ʋ���
static C_CmdwE0001100 CmdwE0001100;//�Ե��ܱ�ң����բ��ֻ����Ϣ���ʶ�Ͳ������ַ��ƥ���������� (ֻд)
static C_CmdwE0001101 CmdwE0001101;//�Ե��ܱ�ң�غ�բ��ֻ����Ϣ���ʶ�Ͳ������ַ��ƥ���������� (ֻд)
static C_CmdwE0001102 CmdwE0001102;//�Ե��ܱ��������ֻ����Ϣ���ʶ�Ͳ������ַ��ƥ���������� (ֻд)
static C_CmdwE0001103 CmdwE0001103;//�Ե��ܱ�����������ֻ����Ϣ���ʶ�Ͳ������ַ��ƥ���������� (ֻд)
static C_CmdwE0001104 CmdwE0001104;//ɾ��������NNNN��ʾ��ɾ��������� 
//�û��Զ�������
static C_CmdwE0002000 CmdwE0002000;//�û��Զ�������CCCC�����̱���NN����NN�������Զ�������
//���������
static C_CmdwE0800000 CmdwE0800000; //�������״̬ 
static C_CmdwE0800001 CmdwE0800001; //����������
static C_CmdwE0800002 CmdwE0800002; //�������ַ
static C_CmdwE0800003 CmdwE0800003; //������ͨ�Ź�Լ
static C_CmdwE0800004 CmdwE0800004; //���ܱ�����
static C_CmdwE0800005 CmdwE0800005; //�ܷ�����
static C_CmdwE0800006 CmdwE0800006; //�ص��û�����
static C_CmdwE0800007 CmdwE0800007; //��բ����
static C_CmdwE0800008 CmdwE0800008; //��������
static C_CmdwE0800009 CmdwE0800009; //�������Ӧ�ɼ��ն˵�ַ
static C_CmdwE080000A CmdwE080000A; //������˿ں�
static C_CmdwE080000B CmdwE080000B; //�˿ڲ���
static C_CmdwE080000C CmdwE080000C; //CT���
static C_CmdwE080000D CmdwE080000D; //PT���
static C_CmdwE080000F CmdwE080000F; //�����������
static C_CmdwE0800010 CmdwE0800010; //��������߷�ʽ
static C_CmdwE0800011 CmdwE0800011; //���ѹ(���β�)
static C_CmdwE0800012 CmdwE0800012; //�����(�����豸��������㵽���β�)
static C_CmdwE0800013 CmdwE0800013; //�û��걨����(���㵽���β�)
static C_CmdwE0800014 CmdwE0800014; //��ѹ������(���㵽���β�
static C_CmdwE080001F CmdwE080001F; //�����������
static C_CmdwE0800021 CmdwE0800021; //���ܱ�ͨ�����룬����8�ֽڣ���λ��0
static C_CmdwE0800022 CmdwE0800022; //���ܱ�ͨ���û���(ASC II��)����8�ֽڣ���λ��0
static C_CmdwE080002F CmdwE080002F; //�����������
//��������������
static C_CmdwE0800100 CmdwE0800100; //������ƽ�����ֵ ȱʡֵ��������30% �������ߣ�50%
static C_CmdwE0800101 CmdwE0800101; //������ƽ��Ȼָ���ֵ��ȱʡֵ��������25% �������ߣ�40%
static C_CmdwE0800102 CmdwE0800102; //�ϸ��ѹ����(����ֵ��δ���û�0ʱ������ȱʡ�����ж�)��ȱʡֵΪ���ѹ��1.07��
static C_CmdwE0800103 CmdwE0800103; //�ϸ��ѹ����(����ֵ��δ���û�0ʱ������ȱʡ�����ж�)��ȱʡֵΪ���ѹ��0.93��
static C_CmdwE0800104 CmdwE0800104; //�жϵ�����������Զֵ�ı�����ȱʡΪ1.3
static C_CmdwE0800105 CmdwE0800105; //�жϸ��ɹ�����Զֵ�ı�����ȱʡΪ1.2
static C_CmdwE0800106 CmdwE0800106; //�жϸ��ɹ��ػָ�����Զֵ�ı�����ȱʡΪ0.95
static C_CmdwE0800107 CmdwE0800107; //�������������ֵ��ȱʡֵ�������25%
static C_CmdwE0800108 CmdwE0800108; //������������ָ���ֵ��ȱʡֵ�������15%
static C_CmdwE0800109 CmdwE0800109; //�жϵ��������ɻָ�����Զֵ�ı�����ȱʡֵΪ1.0
static C_CmdwE080010A CmdwE080010A; //��ѹ��ƽ�����ֵ
static C_CmdwE080010B CmdwE080010B; //��ѹ��ƽ��Ȼ�
static C_CmdwE080010C CmdwE080010C; //ʧѹ����౨�����ж���ֵ�����ڸ���ֵ�Ƕ��࣬���ڸ���ֵ��ʧѹ��LL���ָ���ѹ��ȱʡΪ���ѹ��85%MM������������ȱʡΪ�������10%NN��������ѹ��ȱʡΪ���ѹ��78%
static C_CmdwE080010F CmdwE080010F; //�����������
static C_CmdwE0800200 CmdwE0800200; //��ѹ��ƽ��Ȼ�
static C_CmdwE0800201 CmdwE0800201; //������������ֵ��ȱʡֵ5%
static C_CmdwE0800202 CmdwE0800202; //��ѹ�����ʱ����ָ���ֵȱʡֵ3%
static C_CmdwE0800203 CmdwE0800203; //���������ʱ����ָ���ֵȱʡֵ3%
static C_CmdwE0800204 CmdwE0800204; //��������������
static C_CmdwE0800205 CmdwE0800205; //������������
static C_CmdwE0800206 CmdwE0800206; //���г������ 
static C_CmdwE0800207 CmdwE0800207; //��ѹƫ������
static C_CmdwE0800208 CmdwE0800208; //��ѹƫ������
static C_CmdwE0800209 CmdwE0800209; //Ƶ��ƫ������
static C_CmdwE080020A CmdwE080020A; //Ƶ��ƫ������
static C_CmdwE080020B CmdwE080020B; //��ѹ������ֵ
static C_CmdwE080020C CmdwE080020C; //��ѹ������ֵ
static C_CmdwE080020D CmdwE080020D; //�޹�������Ƿ�����澯������ֵ
static C_CmdwE080020E CmdwE080020E; //�޹�������Ƿ�����澯�ָ���ֵ
//��������������
static C_CmdwE0000200 CmdwE0000200;//��Ч����ͨ������ 00~20
static C_CmdwE0000230 CmdwE0000230;//����������ñ� 30~50
static C_CmdwE0000301 CmdwE0000301;//������� ��ͨ����� 01~FE
static C_CmdwE0000401 CmdwE0000401;//������� �м������ 01~FE
static C_CmdwE0000501 CmdwE0000501;//���������� ģ������ 01~FD
static C_CmdwE0000601 CmdwE0000601;//���������� �������� 01~FD
static C_CmdwE0000701 CmdwE0000701;//���������� ��������� 01~FD
static C_CmdwE0000801 CmdwE0000801;//���������� ����� 01~FD
static C_CmdwE0000A00 CmdwE0000A00;//�˿ڲ��� 485�˿ڶ��� 01~1E

static C_CmdwE0000C00 CmdwE0000C00;//���ز��� ���ؽӵ����� 00~0F
//static C_CmdwE0000C10 CmdwE0000C10;//(ֻ��)���ز��� ���뿪��״̬ 10~1F
static C_CmdwE0000C20 CmdwE0000C20;//(ֻд)���ز��� �������״̬ 20~2F
static C_CmdwE0000C30 CmdwE0000C30;//���ز��� ��������ִ� 30~3F
static C_CmdwE0000D01 CmdwE0000D01;//���ʿ���ϵ��1��ʱ��(1-8) 01~08
static C_CmdwE0000D11 CmdwE0000D11;//���ʿ���ϵ��2��ʱ��(1-8) 11~18
static C_CmdwE0000D21 CmdwE0000D21;//���ʿ���ϵ��3��ʱ��(1-8) 21~28
static C_CmdwE0000D31 CmdwE0000D31;//���ʿ���ϵ��4��ʱ��(1-8) 31~38

//���ڲ�������̫�࣬��������洢�����ö��ֲ��ҷ���ѯ
static S_CmdObjPara sCmdObjParametersW[] =
{//��DIֵ��С��������
	{0xE0000100, &CmdwE0000100},//��վ����
	{0xE0000101, &CmdwE0000101},//��վ��������1
	{0xE0000102, &CmdwE0000102},//��վ��������2
	{0xE0000103, &CmdwE0000103},//����
	{0xE0000104, &CmdwE0000104},//APN
	{0xE0000105, &CmdwE0000105},//APN�û�
	{0xE0000106, &CmdwE0000106},//APN����
	{0xE0000107, &CmdwE0000107},//��������
	{0xE0000108, &CmdwE0000108},//�����ز����
	{0xE0000109, &CmdwE0000109},//���ߺ��ز�����
	{0xE000010A, &CmdwE000010A},//���ӷ�ʽ 0��TCP��ʽ��1��UDP��ʽ
	{0xE000010B, &CmdwE000010B},//ѹ������:0����ѹ��  1��ѹ����BIN����
	{0xE000010F, &CmdwE000010F},//�����������

	{0xE0000120, &CmdwE0000120},//�ն˵���������
	{0xE0000121, &CmdwE0000121},//�ն˵�ַ
	{0xE0000122, &CmdwE0000122},//�ն�ͨ�Ź���ģʽ
	{0xE0000123, &CmdwE0000123},//���ģʽ�ͷ�����ģʽ�£�TCP�����˿ںţ�BIN����
	{0xE0000124, &CmdwE0000124},//�ն˵�ǰͨ��IP��ַ
	{0xE0000125, &CmdwE0000125},//�ն˵�ǰͨ�����������ַ
	{0xE0000126, &CmdwE0000126},//�ն˵�ǰͨ�����ص�ַ
	{0xE0000127, &CmdwE0000127},//��������IP��ַ��ȡ��ʽ
	{0xE000012F, &CmdwE000012F},//�����������
	
	{0xE0000130, &CmdwE0000130},//�ն�ʱ��
	{0xE0000131, &CmdwE0000131},//(ֻд)ʵʱУʱ

	{0xE0000140, &CmdwE0000140},//(ֻд)�ն�ʱ��

	{0xE0000150, &CmdwE0000150},//�澯�����ϱ�������
	{0xE0000151, &CmdwE0000151},//�澯�ж�������
	{0xE0000152, &CmdwE0000152},//�¼���¼������

	{0xE0000160, &CmdwE0000160},//�����¶�������͵�ѹ�ϸ��ʶ����
	{0xE0000161, &CmdwE0000161},//�����յ�������͵�ѹ�ϸ��ʵ�
	{0xE0000162, &CmdwE0000162},//�µ�ѹ�ϸ������ݶ���
	{0xE0000163, &CmdwE0000163},//�յ�ѹ�ϸ������ݶ���

	{0xE0000170, &CmdwE0000170},//(ֻд)��Ϣ��֤���ֶ�PWУ�鷽ʽ
	{0xE0000171, &CmdwE0000171},//��֤������Ϣ��BIN���룬����
	{0xE0000172, &CmdwE0000172},//��֤��Ӧ��Ϣ��BIN���룬����
	{0xE0000173, &CmdwE0000173},//�������Ϣ��BIN���룬����
	{0xE0000174, &CmdwE0000174},//�������Ӧ��Ϣ��BIN���룬����

	{0xE0000180, &CmdwE0000180},//�жϸ��ɹ��ء����������ɡ�����ѹ������ͬ�����õ硢���ʳ���ֵ��������ƽ��ȡ���ѹ���䡢�������䡢�������ƫ���޹�Ƿ�������޹���������ʱ�䡢ȱʡ15����
	{0xE0000181, &CmdwE0000181},//�жϸ��ɹ��ء����������ɡ�����ѹ������ͬ�����õ硢���ʳ���ֵ��������ƽ��ȡ���ѹ���䡢�������䡢�������ƫ���޹�Ƿ�������޹��������ָ�ʱ��
	{0xE0000182, &CmdwE0000182},//�ж϶��ࡢʧѹ��ʧ��ʱ��
	{0xE0000183, &CmdwE0000183},//�ж϶��ࡢʧѹ��ʧ���ָ�ʱ��
	{0xE0000184, &CmdwE0000184},//�жϵ�ѹ������ʱ��
	{0xE0000185, &CmdwE0000185},//�жϵ�ѹ������ָ�ʱ��
	{0xE0000186, &CmdwE0000186},//�жϵ���������ʱ��
	{0xE0000187, &CmdwE0000187},//�жϵ��������Իָ�ʱ��
	{0xE0000188, &CmdwE0000188},//���ܱ�ʱ���쳣��ֵ
	{0xE0000189, &CmdwE0000189},//���ܱ������ֵ
	{0xE000018A, &CmdwE000018A},//���ܱ�ͣ����ֵ
	{0xE000018B, &CmdwE000018B},//ʣ�������ֵ
	{0xE000018C, &CmdwE000018C},//��ͨ������Խ��ֵ
	{0xE000018F, &CmdwE000018F},//�������ݼ���

	//{0xE0000221, &CmdwE0000221},//(ֻ��)��Ч�ĳ���������ñ�����
	{0xE0000222, &CmdwE0000222},//��������Զ�Ӧ�ĳ���������ñ�

	//{0xE0000300, &CmdwE0000300},//(ֻ��)������ͨ��������

	//{0xE0000400, &CmdwE0000400},//(ֻ��)�����м���������

	{0xE0000500, &CmdwE0000500},//ģ��������
	{0xE00005FE, &CmdwE00005FE},//ģ����״̬���
	{0xE00005FF, &CmdwE00005FF},//�����������

	{0xE0000600, &CmdwE0000600},//����������
	{0xE00006FE, &CmdwE00006FE},//������״̬���
	{0xE00006FF, &CmdwE00006FF},//�����������

	{0xE0000700, &CmdwE0000700},//����������
	{0xE00007FE, &CmdwE00007FE},//������״̬���
	{0xE00007FF, &CmdwE00007FF},//�����������

	{0xE0000800, &CmdwE0000800},//���������
	{0xE00008FE, &CmdwE00008FE},//�����״̬���
	{0xE00008FF, &CmdwE00008FF},//�����������

	{0xE0000900, &CmdwE0000900},//�ն��޹�����ͳ�Ʊ�־

	{0xE0000A20, &CmdwE0000A20},//�������ն˵�ַ1~4
	{0xE0000A21, &CmdwE0000A21},//���ն˵�ַ
	{0xE0000A22, &CmdwE0000A22},//�����ն˵�ַ
	{0xE0000A23, &CmdwE0000A23},//���ն˵ȴ����ն�Ӧ��ĳ�ʱ�ж�ʱ��
	{0xE0000A24, &CmdwE0000A24},//���ն˵ȴ����ն˼�����������ĳ�ʱ�ж�ʱ��
	{0xE0000A25, &CmdwE0000A25},//����̨���ն�CT����
	{0xE0000A26, &CmdwE0000A26},//����̨���ն�PT����

	//{0xE0000A30, &CmdwE0000A30},//(ֻ��)��ѯ
	//{0xE0000A31, &CmdwE0000A31},//(ֻ��)�������
	//{0xE0000A32, &CmdwE0000A32},//(ֻ��)�ظ������������
	//{0xE0000A33, &CmdwE0000A33},//(ֻ��)�����ն�̨���ܵ���

	//{0xE0000B00, &CmdwE0000B00},//(ֻ��)��Լ�汾��
	//{0xE0000B01, &CmdwE0000B01},//(ֻ��)�ն�����汾��
	//{0xE0000B02, &CmdwE0000B02},//(ֻ��)�ն�Ӳ���汾��

	{0xE0000C40, &CmdwE0000C40},//ң����բʱ��
	{0xE0000C50, &CmdwE0000C50},//����״̬��01����״̬��00�Ǳ���״̬
	{0xE0000C60, &CmdwE0000C60},//���ʿ����Ƿ���Ч
	{0xE0000C61, &CmdwE0000C61},//�¶ȵ��������Ƿ���Ч 
	{0xE0000C62, &CmdwE0000C62},//�����������Ƿ���Ч 
	{0xE0000C63, &CmdwE0000C63},//���ʿ��Ƹ��ִ���բ�Ƿ���Ч��
	{0xE0000C64, &CmdwE0000C64},//�µ���������բ�ִ���Ч��־��
	{0xE0000C65, &CmdwE0000C65},//������������բ�ִ���Ч��־��
	{0xE0000C66, &CmdwE0000C66},//�����趨ֵ��բ�Ĺ����ɳ���ʱ�䣬	ȱʡ10����
	{0xE0000C67, &CmdwE0000C67},//�����趨ֵ��բ�ָ��ĸ��ɻָ�����ʱ�䣬ȱʡ10����
	{0xE0000C68, &CmdwE0000C68},//��բ����澯��ʱ��ȱʡ10����
	{0xE0000C69, &CmdwE0000C69},//���ʿ����ù����ۼӱ�־
	{0xE0000C6A, &CmdwE0000C6A},//���������õ����ۼӱ�־
	{0xE0000C6B, &CmdwE0000C6B},//���ʶ�ֵ����ϵ��������ȱʡΪ0
	{0xE0000C6C, &CmdwE0000C6C},//�µ����ض�ֵ����ϵ��������ȱʡΪ00
	{0xE0000C6D, &CmdwE0000C6D},//��͸���
	{0xE0000C70, &CmdwE0000C70}, //X��ʱ�޵����Ч��ʶ
	{0xE0000C71, &CmdwE0000C71}, //��ʱ�޵�ļ��㻬��ʱ�䣬��λ����
	{0xE0000D00, &CmdwE0000D00}, //���ʿ��ƹ��ʶ�ֵϵ��1,ʱ����
	{0xE0000D09, &CmdwE0000D09}, //���ʿ���ϵ��1,ִ��ʱ��
	{0xE0000D0F, &CmdwE0000D0F}, //���ʿ���ϵ��1�������������
	{0xE0000D10, &CmdwE0000D10}, //���ʿ��ƹ��ʶ�ֵϵ��2,ʱ����
	{0xE0000D19, &CmdwE0000D19}, //���ʿ���ϵ��2,ִ��ʱ��
	{0xE0000D1F, &CmdwE0000D1F}, //���ʿ���ϵ��2�������������
	{0xE0000D20, &CmdwE0000D20}, //���ʿ��ƹ��ʶ�ֵϵ��3,ʱ����
	{0xE0000D29, &CmdwE0000D29}, //���ʿ���ϵ��3,ִ��ʱ��
	{0xE0000D2F, &CmdwE0000D2F}, //���ʿ���ϵ��3�������������
	{0xE0000D30, &CmdwE0000D30}, //���ʿ��ƹ��ʶ�ֵϵ��4,ʱ����
	{0xE0000D39, &CmdwE0000D39}, //���ʿ���ϵ��4,ִ��ʱ��
	{0xE0000D3F, &CmdwE0000D3F}, //���ʿ���ϵ��4�������������
	{0xE0000E00, &CmdwE0000E00}, //�µ������ƣ��µ����޶�
	{0xE0000E01, &CmdwE0000E01}, //�µ������ƣ��ִε���
	{0xE0000E02, &CmdwE0000E02}, //���������ƣ����ι�����
	{0xE0000E03, &CmdwE0000E03}, //���������ƣ�ʣ�����
	{0xE0000E04, &CmdwE0000E04}, //�ն˵���������ʱ�κͷ�����
	{0xE0000E05, &CmdwE0000E05}, //
	{0xE0000E10, &CmdwE0000E10}, //���ݿ��Ƿ�Ͷ��
	{0xE0000E11, &CmdwE0000E11}, //���ݿز���

	{0xE0001100, &CmdwE0001100},//(ֻд)�Ե��ܱ�ң����բ��ֻ����Ϣ���ʶ�Ͳ������ַ��ƥ������������
	{0xE0001101, &CmdwE0001101},//(ֻд)�Ե��ܱ�ң�غ�բ��ֻ����Ϣ���ʶ�Ͳ������ַ��ƥ������������
	{0xE0001102, &CmdwE0001102},//(ֻд)�Ե��ܱ��������ֻ����Ϣ���ʶ�Ͳ������ַ��ƥ����������
	{0xE0001103, &CmdwE0001103},//(ֻд)�Ե��ܱ�����������ֻ����Ϣ���ʶ�Ͳ������ַ��ƥ������������
	{0xE0001104, &CmdwE0001104},//(ֻд)ɾ��������

	{0xE0002000, &CmdwE0002000},//�û��Զ�������	

	{0xE0800000, &CmdwE0800000}, //�������״̬ 
	{0xE0800001, &CmdwE0800001}, //����������
	{0xE0800002, &CmdwE0800002}, //�������ַ
	{0xE0800003, &CmdwE0800003}, //������ͨ�Ź�Լ
	{0xE0800004, &CmdwE0800004}, //���ܱ�����
	{0xE0800005, &CmdwE0800005}, //�ܷ�����
	{0xE0800006, &CmdwE0800006}, //�ص��û�����
	{0xE0800007, &CmdwE0800007}, //��բ����
	{0xE0800008, &CmdwE0800008}, //��������
	{0xE0800009, &CmdwE0800009}, //�������Ӧ�ɼ��ն˵�ַ
	{0xE080000A, &CmdwE080000A}, //������˿ں�
	{0xE080000B, &CmdwE080000B}, //�˿ڲ���
	{0xE080000C, &CmdwE080000C}, //CT���
	{0xE080000D, &CmdwE080000D}, //PT���
	{0xE080000F, &CmdwE080000F}, //�����������
	{0xE0800010, &CmdwE0800010}, //��������߷�ʽ
	{0xE0800011, &CmdwE0800011}, //���ѹ(���β�)
	{0xE0800012, &CmdwE0800012}, //�����
	{0xE0800013, &CmdwE0800013}, //�û��걨����(���㵽���β�)
	{0xE0800014, &CmdwE0800014}, //��ѹ������(���㵽���β�)
	{0xE080001F, &CmdwE080001F}, //�����������
	{0xE0800021, &CmdwE0800021}, //���ܱ�ͨ������
	{0xE0800022, &CmdwE0800022}, //���ܱ�ͨ���û���
	{0xE080002F, &CmdwE080002F}, //�������ݼ���

	{0xE0800100, &CmdwE0800100}, //������ƽ�����ֵ
	{0xE0800101, &CmdwE0800101}, //������ƽ��Ȼָ���ֵ
	{0xE0800102, &CmdwE0800102}, //�ϸ��ѹ����
	{0xE0800103, &CmdwE0800103}, //�ϸ��ѹ����
	{0xE0800104, &CmdwE0800104}, //�жϵ�����������Զֵ�ı���
	{0xE0800105, &CmdwE0800105}, //�жϸ��ɹ�����Զֵ�ı���
	{0xE0800106, &CmdwE0800106}, //�жϸ��ɹ��ػָ�����Զֵ�ı���
	{0xE0800107, &CmdwE0800107}, //�������������ֵ
	{0xE0800108, &CmdwE0800108}, //������������ָ���ֵ
	{0xE0800109, &CmdwE0800109}, //�жϵ��������ɻָ�����Զֵ
	{0xE080010A, &CmdwE080010A}, //��ѹ��ƽ�����ֵ
	{0xE080010B, &CmdwE080010B}, //��ѹ��ƽ��Ȼָ���ֵ
	{0xE080010C, &CmdwE080010C}, //ʧѹ����౨�����ж���ֵ
	{0xE080010F, &CmdwE080010F}, //�����������

	{0xE0800200, &CmdwE0800200}, //��ѹ��������ֵ��ȱʡֵ5%
	{0xE0800201, &CmdwE0800201}, //������������ֵ��ȱʡֵ5%
	{0xE0800202, &CmdwE0800202}, //��ѹ�����ʱ����ָ���ֵ
	{0xE0800203, &CmdwE0800203}, //���������ʱ����ָ���ֵ
	{0xE0800204, &CmdwE0800204}, //��������������
	{0xE0800205, &CmdwE0800205}, //������������
	{0xE0800206, &CmdwE0800206}, //���г������
	{0xE0800207, &CmdwE0800207}, //��ѹƫ������
	{0xE0800208, &CmdwE0800208}, //��ѹƫ������
	{0xE0800209, &CmdwE0800209}, //Ƶ��ƫ������
	{0xE080020A, &CmdwE080020A}, //Ƶ��ƫ������
	{0xE080020B, &CmdwE080020B}, //��ѹ������ֵ
	{0xE080020C, &CmdwE080020C}, //��ѹ������ֵ
	{0xE080020D, &CmdwE080020D},  //�޹�������Ƿ�����澯������ֵ
	{0xE080020E, &CmdwE080020E},  //�޹�������Ƿ�����澯�ָ���ֵ

};

static S_CmdObjPara3D sCmdObjParametersW_3D[] =//����������
{//��DIֵ��С��������
	{0xE0000200, 0xE0000220, &CmdwE0000200},//������� 00~20
	{0xE0000230, 0xE0000250, &CmdwE0000230},//����������ñ� 30~50
	{0xE0000301, 0xE00003FE, &CmdwE0000301},//������� ��ͨ�����
	{0xE0000401, 0xE00004FE, &CmdwE0000401},//������� �м������
	{0xE0000501, 0xE00005FD, &CmdwE0000501},//���������� ģ������
	{0xE0000601, 0xE00006FD, &CmdwE0000601},//���������� ��������
	{0xE0000701, 0xE00007FD, &CmdwE0000701},//���������� ���������
	{0xE0000801, 0xE00008FD, &CmdwE0000801},//���������� �����
	{0xE0000A00, 0xE0000A1E, &CmdwE0000A00},//�˿ڲ��� 485�˿ڶ���

	{0xE0000C00, 0xE0000C0F, &CmdwE0000C00},//���ز��� ���ؽӵ�����
	//{0xE0000C10, 0xE0000C1F, &CmdwE0000C10},//(ֻ��)���ز��� �������״̬
	{0xE0000C20, 0xE0000C2F, &CmdwE0000C20},//(ֻд)���ز��� �������״̬
	{0xE0000C30, 0xE0000C3F, &CmdwE0000C30},//���ز��� ��������ִ�
	{0xE0000D01, 0xE0000D08, &CmdwE0000D01},//���ʿ���ϵ��1��ʱ��(1-8)
	{0xE0000D11, 0xE0000D18, &CmdwE0000D11},//���ʿ���ϵ��2��ʱ��(1-8)
	{0xE0000D21, 0xE0000D28, &CmdwE0000D21},//���ʿ���ϵ��3��ʱ��(1-8)
	{0xE0000D31, 0xE0000D38, &CmdwE0000D31},//���ʿ���ϵ��4��ʱ��(1-8)

	{0xFFFFFFFF, 0xFFFFFFFF, NULL},//end
};

int C_AFNCMD_WPARAM::Execute(const S_CmdObjs &CmdObjs, strings &strResponseInfos)
{
	if (CmdObjs.empty())
		return FRAME_PROCESS_RESPONSE_DENY;

	string strParamChange;
	for (S_CmdObjs::const_iterator iterNow = CmdObjs.begin(); iterNow != CmdObjs.end(); advance(iterNow, 1))
	{
		C_CmdReceiver *pCmdReveiver = GetCmdReceiver(iterNow->m_DI);
		if (pCmdReveiver == NULL)
			return FRAME_PROCESS_RESPONSE_DENY;

		string strItem;
		strings strResponse;
		if (pCmdReveiver->Prompt() != NULL)
			logit<<"�����:"<<pCmdReveiver->Prompt()<<endl;
		pCmdReveiver->SetPackageLength(m_PackageLength);
		pCmdReveiver->AppendDADT(strItem, *iterNow);
		if (pCmdReveiver->Action(*iterNow, strResponse) == false)
		{
			strItem.push_back(0x02); // err �����֧��
		}
		else
		{
			strItem.push_back(0x00); // err ��ȷ
			if (iterNow->m_DA == 0x00) // ��¼�ն˲������
				strParamChange.append((char *)&iterNow->m_DI, 4);
		}

		strResponseInfos.push_back(strItem);
		// �ն˲�������¼�
		if (strParamChange.size() >= 200) // ���ļ�
		{
			CEventRecrod_ERD8::EventRecord(0xE2010014, C_TIME::TimeNow(), strParamChange);
			strParamChange.clear();
		}
	}

	// ˢ�²��������
	UpdateMeterInfo();
	if (strParamChange.empty() == false)
		CEventRecrod_ERD8::EventRecord(0xE2010014, C_TIME::TimeNow(), strParamChange);
	return FRAME_PROCESS_RESPONSE_DATA;
}

C_CmdReceiver *C_AFNCMD_WPARAM::GetCmdReceiver(INT32U DI)
{
	const S_CmdObjPara CmdObjNow = {DI, NULL};
	S_CmdObjPara *pFindObj = lower_bound(sCmdObjParametersW,
		sCmdObjParametersW+sizeof(sCmdObjParametersW)/sizeof(sCmdObjParametersW[0]), CmdObjNow);
	if (pFindObj != sCmdObjParametersW+sizeof(sCmdObjParametersW)/sizeof(sCmdObjParametersW[0])
		&& pFindObj->m_DI == DI)
		return pFindObj->m_pCmdReceiver;

	const S_CmdObjPara3D CmdObjNow3D = {DI, DI, NULL};
	S_CmdObjPara3D *pFindObj3D = lower_bound(sCmdObjParametersW_3D,
		sCmdObjParametersW_3D+sizeof(sCmdObjParametersW_3D)/sizeof(sCmdObjParametersW_3D[0]), CmdObjNow3D);
	if (pFindObj3D != sCmdObjParametersW_3D+sizeof(sCmdObjParametersW_3D)/sizeof(sCmdObjParametersW_3D[0]))
	{
		if (DI >= pFindObj3D->m_diBegin && DI <= pFindObj3D->m_diEnd)
			return pFindObj3D->m_pCmdReceiver;
		if (pFindObj3D != sCmdObjParametersW_3D)
			pFindObj3D -= 1;
		if (DI >= pFindObj3D->m_diBegin && DI <= pFindObj3D->m_diEnd)
			return pFindObj3D->m_pCmdReceiver;
	}
	return NULL;
}

bool C_AFNCMD_WPARAM::UpdateMeterInfo(void)
{
	C_WMCmdReceiver WMCmdReceiver;
	return WMCmdReceiver.UpdateInformation();
}



