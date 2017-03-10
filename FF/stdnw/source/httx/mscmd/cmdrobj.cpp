/** @file
 *  @brief ��̨ͨѶ��������������弰����
 *  @author  zhongguangxiong
 *  @date 2012/02/18
 *  @version 0.1
 */
#include "cmdrobj.h"
#include "cmdrpara.h"
#include "cmdrctrl.h"
#include "cmdrmp.h"

//�����������
//ͨ�Ų���
static C_CmdrE0000100 CmdrE0000100; //��վ����
static C_CmdrE0000101 CmdrE0000101; //��վ��������1
static C_CmdrE0000102 CmdrE0000102; //��վ��������2
static C_CmdrE0000103 CmdrE0000103; //����
static C_CmdrE0000104 CmdrE0000104; //APN
static C_CmdrE0000105 CmdrE0000105; //APN�û�
static C_CmdrE0000106 CmdrE0000106; //APN����
static C_CmdrE0000107 CmdrE0000107; //��������
static C_CmdrE0000108 CmdrE0000108; //�����ز����
static C_CmdrE0000109 CmdrE0000109; //���ߺ��ز�������ȱʡ3��,�ز�ָ�������󣬵��첻���ز�
static C_CmdrE000010A CmdrE000010A; //���ӷ�ʽ 0��TCP��ʽ��1��UDP��ʽ
static C_CmdrE000010B CmdrE000010B; //ѹ������:0����ѹ��  1��ѹ����BIN��?
static C_CmdrE000010F CmdrE000010F; //�����������

static C_CmdrE0000120 CmdrE0000120; //�ն˵���������
static C_CmdrE0000121 CmdrE0000121; //�ն˵�ַ
static C_CmdrE0000122 CmdrE0000122; //�ն�ͨ�Ź���ģʽ
static C_CmdrE0000123 CmdrE0000123; //���ģʽ�ͷ�����ģʽ�£�TCP�����˿ںţ�BIN����
static C_CmdrE0000124 CmdrE0000124; //�ն˵�ǰͨ��IP��ַ
static C_CmdrE0000125 CmdrE0000125; //�ն˵�ǰͨ�����������ַ
static C_CmdrE0000126 CmdrE0000126; //�ն˵�ǰͨ�����ص�ַ
static C_CmdrE0000127 CmdrE0000127; //��������IP��ַ��ȡ��ʽ 0���ֶ�����IP��ַ 1���Զ�����IP��ַ
static C_CmdrE000012F CmdrE000012F; //�����������
//ʱ�����
static C_CmdrE0000130 CmdrE0000130; //�ն�ʱ��
//static C_CmdrE0000131 CmdrE0000131; //ʵʱУʱ (ֻд)
//��λ����
//static C_CmdrE0000140 CmdrE0000140; //�ն˸�λ (ֻд)
//�澯����
static C_CmdrE0000150 CmdrE0000150; //�澯�����ϱ�������
static C_CmdrE0000151 CmdrE0000151; //�澯�ж�������
static C_CmdrE0000152 CmdrE0000152; //�¼���¼������
//�����ղ���
static C_CmdrE0000160 CmdrE0000160; //����ʱ����ա�Сʱ�������¶�������͵�ѹ�ϸ��ʶ���ȣ� �¶������
static C_CmdrE0000161 CmdrE0000161; //�յ����������룩����ʱ�̣������յ�������͵�ѹ�ϸ��ʵȣ� �ն������
static C_CmdrE0000162 CmdrE0000162; //�µ�ѹ�ϸ������ݶ���
static C_CmdrE0000163 CmdrE0000163; //�յ�ѹ�ϸ������ݶ���
//�����֤
//static C_CmdrE0000170 CmdrE0000170; //��Ϣ��֤���ֶ�PWУ�鷽ʽ (ֻд)
static C_CmdrE0000171 CmdrE0000171; //��֤������Ϣ��BIN���룬����
static C_CmdrE0000172 CmdrE0000172; //��֤��Ӧ��Ϣ��BIN���룬����
static C_CmdrE0000173 CmdrE0000173; //�������Ϣ��BIN���룬����
static C_CmdrE0000174 CmdrE0000174; //�������Ӧ��Ϣ��BIN���룬����
//��ֵ����
static C_CmdrE0000180 CmdrE0000180;//�жϸ��ɹ��ء����������ɡ�����ѹ������ͬ�����õ硢���ʳ���ֵ��������ƽ��ȡ���ѹ���䡢�������䡢�������ƫ���޹�Ƿ�������޹���������ʱ�䡢ȱʡ15����
static C_CmdrE0000181 CmdrE0000181;//�жϸ��ɹ��ء����������ɡ�����ѹ������ͬ�����õ硢���ʳ���ֵ��������ƽ��ȡ���ѹ���䡢�������䡢�������ƫ���޹�Ƿ�������޹��������ָ�ʱ�䡢ȱʡ15����
static C_CmdrE0000182 CmdrE0000182;//�ж϶��ࡢʧѹ��ʧ��ʱ�䣬ȱʡֵ1����
static C_CmdrE0000183 CmdrE0000183;//�ж϶��ࡢʧѹ��ʧ���ָ�ʱ�䣬ȱʡֵ1����
static C_CmdrE0000184 CmdrE0000184;//�жϵ�ѹ������ʱ�䣬ȱʡֵ15����
static C_CmdrE0000185 CmdrE0000185;//�жϵ�ѹ������ָ�ʱ�䣬ȱʡֵ15����
static C_CmdrE0000186 CmdrE0000186;//�жϵ���������ʱ�䣬ȱʡֵ15����
static C_CmdrE0000187 CmdrE0000187;//�жϵ��������Իָ�ʱ�䣬ȱʡֵ15����
static C_CmdrE0000188 CmdrE0000188;//���ܱ�ʱ���쳣��ֵ��ȱʡֵ10����
static C_CmdrE0000189 CmdrE0000189;//���ܱ������ֵ��ȱʡΪ10��
static C_CmdrE000018A CmdrE000018A;//���ܱ�ͣ����ֵ ȱʡΪ0.1kWh
static C_CmdrE000018B CmdrE000018B;//ʣ�������ֵ��ȱʡΪ20.00 Ԫ
static C_CmdrE000018C CmdrE000018C;//��ͨ������Խ��ֵ
static C_CmdrE000018F CmdrE000018F;//�������ݼ���
//�������
static C_CmdrE0000221 CmdrE0000221;//��Ч�ĳ���������ñ����� (ֻ��)
static C_CmdrE0000222 CmdrE0000222;//��������Զ�Ӧ�ĳ���������ñ�
//�������
static C_CmdrE0000300 CmdrE0000300;//������ͨ�������� (ֻ��)

static C_CmdrE0000400 CmdrE0000400;//�����м��������� (ֻ��)
//����������
static C_CmdrE0000500 CmdrE0000500;//ģ��������
static C_CmdrE00005FE CmdrE00005FE;//ģ����״̬���
static C_CmdrE00005FF CmdrE00005FF;//�������ݼ���
//����������
static C_CmdrE0000600 CmdrE0000600;//����������
static C_CmdrE00006FE CmdrE00006FE;//������״̬���
static C_CmdrE00006FF CmdrE00006FF;//�������ݼ���
//����������
static C_CmdrE0000700 CmdrE0000700;//����������
static C_CmdrE00007FE CmdrE00007FE;//������״̬���
static C_CmdrE00007FF CmdrE00007FF;//�������ݼ���
//�����
static C_CmdrE0000800 CmdrE0000800;//���������
static C_CmdrE00008FE CmdrE00008FE;//�����״̬���
static C_CmdrE00008FF CmdrE00008FF;//�������ݼ���

static C_CmdrE0000900 CmdrE0000900;//�ն��޹�����ͳ�Ʊ�־
//�˿ڲ���
//����ͨ�Ų���
static C_CmdrE0000A20 CmdrE0000A20;//�������ն˵�ַ1~4
static C_CmdrE0000A21 CmdrE0000A21;//���ն˵�ַ
static C_CmdrE0000A22 CmdrE0000A22;//�����ն˵�ַ
static C_CmdrE0000A23 CmdrE0000A23;//���ն˵ȴ����ն�Ӧ��ĳ�ʱ�ж�ʱ��
static C_CmdrE0000A24 CmdrE0000A24;//���ն˵ȴ����ն˼�����������ĳ�ʱ�ж�ʱ��
static C_CmdrE0000A25 CmdrE0000A25;//����̨���ն�CT����
static C_CmdrE0000A26 CmdrE0000A26;//����̨���ն�PT����
//��������
static C_CmdrE0000A30 CmdrE0000A30;//��ѯ (ֻ��)
static C_CmdrE0000A31 CmdrE0000A31;//������� (ֻ��)
static C_CmdrE0000A32 CmdrE0000A32;//�ظ������������ (ֻ��)
static C_CmdrE0000A33 CmdrE0000A33;//�����ն�̨���ܵ��� (ֻ��)
//�汾����CmdrE0000B00
static C_CmdrE0000B00 CmdrE0000B00;//��Լ�汾�ţ�����Լ�涨Ϊ0100  (ֻ��)
static C_CmdrE0000B01 CmdrE0000B01;//�ն�����汾��  (ֻ��)
static C_CmdrE0000B02 CmdrE0000B02;//�ն�Ӳ���汾��  (ֻ��)
//���ز���
static C_CmdrE0000C40 CmdrE0000C40;//ң����բʱ��
static C_CmdrE0000C50 CmdrE0000C50;//����״̬��01����״̬��00�Ǳ���״̬
static C_CmdrE0000C60 CmdrE0000C60;//���ʿ����Ƿ���Ч
static C_CmdrE0000C61 CmdrE0000C61;//�¶ȵ��������Ƿ���Ч 
static C_CmdrE0000C62 CmdrE0000C62;//�����������Ƿ���Ч 
static C_CmdrE0000C63 CmdrE0000C63;//���ʿ��Ƹ��ִ���բ�Ƿ���Ч��
static C_CmdrE0000C64 CmdrE0000C64;//�µ���������բ�ִ���Ч��־��
static C_CmdrE0000C65 CmdrE0000C65;//������������բ�ִ���Ч��־��
static C_CmdrE0000C66 CmdrE0000C66;//�����趨ֵ��բ�Ĺ����ɳ���ʱ�䣬	ȱʡ10����
static C_CmdrE0000C67 CmdrE0000C67;//�����趨ֵ��բ�ָ��ĸ��ɻָ�����ʱ�䣬ȱʡ10����
static C_CmdrE0000C68 CmdrE0000C68;//��բ����澯��ʱ��ȱʡ10����
static C_CmdrE0000C69 CmdrE0000C69;//���ʿ����ù����ۼӱ�־
static C_CmdrE0000C6A CmdrE0000C6A;//���������õ����ۼӱ�־
static C_CmdrE0000C6B CmdrE0000C6B;//���ʶ�ֵ����ϵ��������ȱʡΪ0
static C_CmdrE0000C6C CmdrE0000C6C;//�µ����ض�ֵ����ϵ��������ȱʡΪ00
static C_CmdrE0000C6D CmdrE0000C6D;//��͸���
static C_CmdrE0000C70 CmdrE0000C70;//X��ʱ�޵����Ч��ʶ
static C_CmdrE0000C71 CmdrE0000C71;//��ʱ�޵�ļ��㻬��ʱ�䣬��λ����

static C_CmdrE0000D00 CmdrE0000D00;//���ʿ���ϵ��1��ʱ����
static C_CmdrE0000D09 CmdrE0000D09;//���ʿ���ϵ��1,ִ��ʱ��
static C_CmdrE0000D0F CmdrE0000D0F;//���ʿ���ϵ��1���������ݺϼ�
static C_CmdrE0000D10 CmdrE0000D10;//���ʿ��ƹ��ʶ�ֵϵ��2,ʱ����
static C_CmdrE0000D19 CmdrE0000D19;//���ʿ���ϵ��2,ִ��ʱ��
static C_CmdrE0000D1F CmdrE0000D1F;//���ʿ���ϵ��2���������ݺϼ�
static C_CmdrE0000D20 CmdrE0000D20;//���ʿ��ƹ��ʶ�ֵϵ��3,ʱ����
static C_CmdrE0000D29 CmdrE0000D29;//���ʿ���ϵ��3,ִ��ʱ��
static C_CmdrE0000D2F CmdrE0000D2F;//���ʿ���ϵ��3���������ݺϼ�
static C_CmdrE0000D30 CmdrE0000D30;//���ʿ��ƹ��ʶ�ֵϵ��4,ʱ����
static C_CmdrE0000D39 CmdrE0000D39;//���ʿ���ϵ��4,ִ��ʱ��
static C_CmdrE0000D3F CmdrE0000D3F;//���ʿ���ϵ��4���������ݺϼ�
static C_CmdrE0000E00 CmdrE0000E00;//�µ������ƣ��µ����޶�
static C_CmdrE0000E01 CmdrE0000E01;//�µ������ƣ��ִε���
static C_CmdrE0000E02 CmdrE0000E02;//���������ƣ����ι�����
static C_CmdrE0000E03 CmdrE0000E03;//���������ƣ�ʣ�����
static C_CmdrE0000E04 CmdrE0000E04;//�ն˵���������ʱ�κͷ�����
static C_CmdrE0000E05 CmdrE0000E05;//
static C_CmdrE0000E10 CmdrE0000E10;//���ݿ��Ƿ�Ͷ��
static C_CmdrE0000E11 CmdrE0000E11;//���ݿز���
/* ������д
//��·�ӿ����ݱ�ʶ
static C_CmdrE0001000 CmdrE0001000;//��ʾ�ն˵�¼��NNNN����Լ�汾�� 
static C_CmdrE0001001 CmdrE0001001;//��ʾ�ն���������������
static C_CmdrE0001002 CmdrE0001002;//��ʾ��¼�˳����������� 
*/
//��������Ʋ���
//static C_CmdrE0001100 CmdrE0001100;//�Ե��ܱ�ң����բ��ֻ����Ϣ���ʶ�Ͳ������ַ��ƥ���������� (ֻд)
//static C_CmdrE0001101 CmdrE0001101;//�Ե��ܱ�ң�غ�բ��ֻ����Ϣ���ʶ�Ͳ������ַ��ƥ���������� (ֻд)
//static C_CmdrE0001102 CmdrE0001102;//�Ե��ܱ��������ֻ����Ϣ���ʶ�Ͳ������ַ��ƥ���������� (ֻд)
//static C_CmdrE0001103 CmdrE0001103;//�Ե��ܱ�����������ֻ����Ϣ���ʶ�Ͳ������ַ��ƥ���������� (ֻд)
//static C_CmdrE0001104 CmdrE0001104;//ɾ��������NNNN��ʾ��ɾ��������� 
//�û��Զ�������
static C_CmdrE0002000 CmdrE0002000;//�û��Զ�������CCCC�����̱���NN����NN�������Զ�������
//���������
static C_CmdrE0800000 CmdrE0800000; //�������״̬ 
static C_CmdrE0800001 CmdrE0800001; //����������
static C_CmdrE0800002 CmdrE0800002; //�������ַ
static C_CmdrE0800003 CmdrE0800003; //������ͨ�Ź�Լ
static C_CmdrE0800004 CmdrE0800004; //���ܱ�����
static C_CmdrE0800005 CmdrE0800005; //�ܷ�����
static C_CmdrE0800006 CmdrE0800006; //�ص��û�����
static C_CmdrE0800007 CmdrE0800007; //��բ����
static C_CmdrE0800008 CmdrE0800008; //��������
static C_CmdrE0800009 CmdrE0800009; //�������Ӧ�ɼ��ն˵�ַ
static C_CmdrE080000A CmdrE080000A; //������˿ں�
static C_CmdrE080000B CmdrE080000B; //�˿ڲ���
static C_CmdrE080000C CmdrE080000C; //CT���
static C_CmdrE080000D CmdrE080000D; //PT���
static C_CmdrE080000F CmdrE080000F; //�����������
static C_CmdrE0800010 CmdrE0800010; //��������߷�ʽ
static C_CmdrE0800011 CmdrE0800011; //���ѹ(���β�)
static C_CmdrE0800012 CmdrE0800012; //�����(�����豸��������㵽���β�)
static C_CmdrE0800013 CmdrE0800013; //�û��걨����(���㵽���β�)
static C_CmdrE0800014 CmdrE0800014; //��ѹ������(���㵽���β�
static C_CmdrE080001F CmdrE080001F; //�����������
static C_CmdrE0800021 CmdrE0800021; //���ܱ�ͨ�����룬����8�ֽڣ���λ��0
static C_CmdrE0800022 CmdrE0800022; //���ܱ�ͨ���û���(ASC II��)����8�ֽڣ���λ��0
static C_CmdrE080002F CmdrE080002F; //�����������
//��������������
static C_CmdrE0800100 CmdrE0800100; //������ƽ�����ֵ ȱʡֵ��������30% �������ߣ�50%
static C_CmdrE0800101 CmdrE0800101; //������ƽ��Ȼָ���ֵ��ȱʡֵ��������25% �������ߣ�40%
static C_CmdrE0800102 CmdrE0800102; //�ϸ��ѹ����(����ֵ��δ���û�0ʱ������ȱʡ�����ж�)��ȱʡֵΪ���ѹ��1.07��
static C_CmdrE0800103 CmdrE0800103; //�ϸ��ѹ����(����ֵ��δ���û�0ʱ������ȱʡ�����ж�)��ȱʡֵΪ���ѹ��0.93��
static C_CmdrE0800104 CmdrE0800104; //�жϵ�����������Զֵ�ı�����ȱʡΪ1.3
static C_CmdrE0800105 CmdrE0800105; //�жϸ��ɹ�����Զֵ�ı�����ȱʡΪ1.2
static C_CmdrE0800106 CmdrE0800106; //�жϸ��ɹ��ػָ�����Զֵ�ı�����ȱʡΪ0.95
static C_CmdrE0800107 CmdrE0800107; //�������������ֵ��ȱʡֵ�������25%
static C_CmdrE0800108 CmdrE0800108; //������������ָ���ֵ��ȱʡֵ�������15%
static C_CmdrE0800109 CmdrE0800109; //�жϵ��������ɻָ�����Զֵ�ı�����ȱʡֵΪ1.0
static C_CmdrE080010A CmdrE080010A; //��ѹ��ƽ�����ֵ
static C_CmdrE080010B CmdrE080010B; //��ѹ��ƽ��Ȼ�
static C_CmdrE080010C CmdrE080010C; //ʧѹ����౨�����ж���ֵ�����ڸ���ֵ�Ƕ��࣬���ڸ���ֵ��ʧѹ��LL���ָ���ѹ��ȱʡΪ���ѹ��85%MM������������ȱʡΪ�������10%NN��������ѹ��ȱʡΪ���ѹ��78%
static C_CmdrE080010F CmdrE080010F; //�����������
static C_CmdrE0800200 CmdrE0800200; //��ѹ��ƽ��Ȼ�
static C_CmdrE0800201 CmdrE0800201; //������������ֵ��ȱʡֵ5%
static C_CmdrE0800202 CmdrE0800202; //��ѹ�����ʱ����ָ���ֵ,ȱʡֵ3%
static C_CmdrE0800203 CmdrE0800203; //���������ʱ����ָ���ֵ,ȱʡֵ3%
static C_CmdrE0800204 CmdrE0800204; //��������������
static C_CmdrE0800205 CmdrE0800205; //������������
static C_CmdrE0800206 CmdrE0800206; //���г������ 
static C_CmdrE0800207 CmdrE0800207; //��ѹƫ������
static C_CmdrE0800208 CmdrE0800208; //��ѹƫ������
static C_CmdrE0800209 CmdrE0800209; //Ƶ��ƫ������
static C_CmdrE080020A CmdrE080020A; //Ƶ��ƫ������
static C_CmdrE080020B CmdrE080020B; //��ѹ������ֵ
static C_CmdrE080020C CmdrE080020C; //��ѹ������ֵ
static C_CmdrE080020D CmdrE080020D; //�޹�������Ƿ�����澯������ֵ
static C_CmdrE080020E CmdrE080020E; //�޹�������Ƿ�����澯�ָ���ֵ
//��������������
static C_CmdrE0000200 CmdrE0000200;//��Ч����ͨ������ 00~20
static C_CmdrE0000230 CmdrE0000230;//����������ñ� 30~50
static C_CmdrE0000301 CmdrE0000301;//������� ��ͨ����� 01~FE
static C_CmdrE0000401 CmdrE0000401;//������� �м������ 01~FE
static C_CmdrE0000501 CmdrE0000501;//���������� ģ������ 01~FD
static C_CmdrE0000601 CmdrE0000601;//���������� �������� 01~FD
static C_CmdrE0000701 CmdrE0000701;//���������� ��������� 01~FD
static C_CmdrE0000801 CmdrE0000801;//���������� ����� 01~FD
static C_CmdrE0000A00 CmdrE0000A00;//�˿ڲ��� 485�˿ڶ��� 01~1E

static C_CmdrE0000C00 CmdrE0000C00;//���ز��� ���ؽӵ����� 00~0F
static C_CmdrE0000C10 CmdrE0000C10;//(ֻ��)���ز��� ���뿪��״̬ 10~1F
//static C_CmdrE0000C20 CmdrE0000C20;//(ֻд)���ز��� �������״̬ 20~2F
static C_CmdrE0000C30 CmdrE0000C30;//���ز��� ��������ִ� 30~3F
static C_CmdrE0000D01 CmdrE0000D01;//���ʿ���ϵ��1��ʱ��(1-8) 01~08
static C_CmdrE0000D11 CmdrE0000D11;//���ʿ���ϵ��2��ʱ��(1-8) 11~18
static C_CmdrE0000D21 CmdrE0000D21;//���ʿ���ϵ��3��ʱ��(1-8) 21~28
static C_CmdrE0000D31 CmdrE0000D31;//���ʿ���ϵ��4��ʱ��(1-8) 31~38

//���ڲ�������̫�࣬��������洢�����ö��ֲ��ҷ���ѯ
static S_CmdObjPara sCmdObjParametersR[] =//һ��������
{//��DIֵ��С��������
	{0xE0000100, &CmdrE0000100},//��վ����
	{0xE0000101, &CmdrE0000101},//��վ��������1
	{0xE0000102, &CmdrE0000102},//��վ��������2
	{0xE0000103, &CmdrE0000103},//����
	{0xE0000104, &CmdrE0000104},//APN
	{0xE0000105, &CmdrE0000105},//APN�û�
	{0xE0000106, &CmdrE0000106},//APN����
	{0xE0000107, &CmdrE0000107},//��������
	{0xE0000108, &CmdrE0000108},//�����ز����
	{0xE0000109, &CmdrE0000109},//���ߺ��ز�����
	{0xE000010A, &CmdrE000010A},//���ӷ�ʽ 0��TCP��ʽ��1��UDP��ʽ
	{0xE000010B, &CmdrE000010B},//ѹ������:0����ѹ��  1��ѹ����BIN����
	{0xE000010F, &CmdrE000010F},//�����������

	{0xE0000120, &CmdrE0000120},//�ն˵���������
	{0xE0000121, &CmdrE0000121},//�ն˵�ַ
	{0xE0000122, &CmdrE0000122},//�ն�ͨ�Ź���ģʽ
	{0xE0000123, &CmdrE0000123},//���ģʽ�ͷ�����ģʽ�£�TCP�����˿ںţ�BIN����
	{0xE0000124, &CmdrE0000124},//�ն˵�ǰͨ��IP��ַ
	{0xE0000125, &CmdrE0000125},//�ն˵�ǰͨ�����������ַ
	{0xE0000126, &CmdrE0000126},//�ն˵�ǰͨ�����ص�ַ
	{0xE0000127, &CmdrE0000127},//��������IP��ַ��ȡ��ʽ
	{0xE000012F, &CmdrE000012F},//�����������
	
	{0xE0000130, &CmdrE0000130},//�ն�ʱ��
	//{0xE0000131, &CmdrE0000131},//(ֻд)ʵʱУʱ

	//{0xE0000140, &CmdrE0000140},//(ֻд)�ն�ʱ��

	{0xE0000150, &CmdrE0000150},//�澯�����ϱ�������
	{0xE0000151, &CmdrE0000151},//�澯�ж�������
	{0xE0000152, &CmdrE0000152},//�¼���¼������

	{0xE0000160, &CmdrE0000160},//�����¶�������͵�ѹ�ϸ��ʶ����
	{0xE0000161, &CmdrE0000161},//�����յ�������͵�ѹ�ϸ��ʵ�
	{0xE0000162, &CmdrE0000162},//�µ�ѹ�ϸ������ݶ���
	{0xE0000163, &CmdrE0000163},//�յ�ѹ�ϸ������ݶ���

	//{0xE0000170, &CmdrE0000170},//(ֻд)��Ϣ��֤���ֶ�PWУ�鷽ʽ
	{0xE0000171, &CmdrE0000171},//��֤������Ϣ��BIN���룬����
	{0xE0000172, &CmdrE0000172},//��֤��Ӧ��Ϣ��BIN���룬����
	{0xE0000173, &CmdrE0000173},//�������Ϣ��BIN���룬����
	{0xE0000174, &CmdrE0000174},//�������Ӧ��Ϣ��BIN���룬����

	{0xE0000180, &CmdrE0000180},//�жϸ��ɹ��ء����������ɡ�����ѹ������ͬ�����õ硢���ʳ���ֵ��������ƽ��ȡ���ѹ���䡢�������䡢�������ƫ���޹�Ƿ�������޹���������ʱ�䡢ȱʡ15����
	{0xE0000181, &CmdrE0000181},//�жϸ��ɹ��ء����������ɡ�����ѹ������ͬ�����õ硢���ʳ���ֵ��������ƽ��ȡ���ѹ���䡢�������䡢�������ƫ���޹�Ƿ�������޹��������ָ�ʱ��
	{0xE0000182, &CmdrE0000182},//�ж϶��ࡢʧѹ��ʧ��ʱ��
	{0xE0000183, &CmdrE0000183},//�ж϶��ࡢʧѹ��ʧ���ָ�ʱ��
	{0xE0000184, &CmdrE0000184},//�жϵ�ѹ������ʱ��
	{0xE0000185, &CmdrE0000185},//�жϵ�ѹ������ָ�ʱ��
	{0xE0000186, &CmdrE0000186},//�жϵ���������ʱ��
	{0xE0000187, &CmdrE0000187},//�жϵ��������Իָ�ʱ��
	{0xE0000188, &CmdrE0000188},//���ܱ�ʱ���쳣��ֵ
	{0xE0000189, &CmdrE0000189},//���ܱ������ֵ
	{0xE000018A, &CmdrE000018A},//���ܱ�ͣ����ֵ
	{0xE000018B, &CmdrE000018B},//ʣ�������ֵ
	{0xE000018C, &CmdrE000018C},//��ͨ������Խ��ֵ
	{0xE000018F, &CmdrE000018F},//�������ݼ���

	{0xE0000221, &CmdrE0000221},//(ֻ��)��Ч�ĳ���������ñ�����
	{0xE0000222, &CmdrE0000222},//��������Զ�Ӧ�ĳ���������ñ�

	{0xE0000300, &CmdrE0000300},//(ֻ��)������ͨ��������

	{0xE0000400, &CmdrE0000400},//(ֻ��)�����м���������

	{0xE0000500, &CmdrE0000500},//ģ��������
	{0xE00005FE, &CmdrE00005FE},//ģ����״̬���
	{0xE00005FF, &CmdrE00005FF},//�����������

	{0xE0000600, &CmdrE0000600},//����������
	{0xE00006FE, &CmdrE00006FE},//������״̬���
	{0xE00006FF, &CmdrE00006FF},//�����������

	{0xE0000700, &CmdrE0000700},//����������
	{0xE00007FE, &CmdrE00007FE},//������״̬���
	{0xE00007FF, &CmdrE00007FF},//�����������

	{0xE0000800, &CmdrE0000800},//���������
	{0xE00008FE, &CmdrE00008FE},//�����״̬���
	{0xE00008FF, &CmdrE00008FF},//�����������

	{0xE0000900, &CmdrE0000900},//�ն��޹�����ͳ�Ʊ�־

	{0xE0000A20, &CmdrE0000A20},//�������ն˵�ַ1~4
	{0xE0000A21, &CmdrE0000A21},//���ն˵�ַ
	{0xE0000A22, &CmdrE0000A22},//�����ն˵�ַ
	{0xE0000A23, &CmdrE0000A23},//���ն˵ȴ����ն�Ӧ��ĳ�ʱ�ж�ʱ��
	{0xE0000A24, &CmdrE0000A24},//���ն˵ȴ����ն˼�����������ĳ�ʱ�ж�ʱ��
	{0xE0000A25, &CmdrE0000A25},//����̨���ն�CT����
	{0xE0000A26, &CmdrE0000A26},//����̨���ն�PT����

	{0xE0000A30, &CmdrE0000A30},//(ֻ��)��ѯ
	{0xE0000A31, &CmdrE0000A31},//(ֻ��)�������
	{0xE0000A32, &CmdrE0000A32},//(ֻ��)�ظ������������
	{0xE0000A33, &CmdrE0000A33},//(ֻ��)�����ն�̨���ܵ���

	{0xE0000B00, &CmdrE0000B00},//(ֻ��)��Լ�汾��
	{0xE0000B01, &CmdrE0000B01},//(ֻ��)�ն�����汾��
	{0xE0000B02, &CmdrE0000B02},//(ֻ��)�ն�Ӳ���汾��

	{0xE0000C40, &CmdrE0000C40},//ң����բʱ��
	{0xE0000C50, &CmdrE0000C50},//����״̬��01����״̬��00�Ǳ���״̬
	{0xE0000C60, &CmdrE0000C60},//���ʿ����Ƿ���Ч
	{0xE0000C61, &CmdrE0000C61},//�¶ȵ��������Ƿ���Ч 
	{0xE0000C62, &CmdrE0000C62},//�����������Ƿ���Ч 
	{0xE0000C63, &CmdrE0000C63},//���ʿ��Ƹ��ִ���բ�Ƿ���Ч��
	{0xE0000C64, &CmdrE0000C64},//�µ���������բ�ִ���Ч��־��
	{0xE0000C65, &CmdrE0000C65},//������������բ�ִ���Ч��־��
	{0xE0000C66, &CmdrE0000C66},//�����趨ֵ��բ�Ĺ����ɳ���ʱ�䣬	ȱʡ10����
	{0xE0000C67, &CmdrE0000C67},//�����趨ֵ��բ�ָ��ĸ��ɻָ�����ʱ�䣬ȱʡ10����
	{0xE0000C68, &CmdrE0000C68},//��բ����澯��ʱ��ȱʡ10����
	{0xE0000C69, &CmdrE0000C69},//���ʿ����ù����ۼӱ�־
	{0xE0000C6A, &CmdrE0000C6A},//���������õ����ۼӱ�־
	{0xE0000C6B, &CmdrE0000C6B},//���ʶ�ֵ����ϵ��������ȱʡΪ0
	{0xE0000C6C, &CmdrE0000C6C},//�µ����ض�ֵ����ϵ��������ȱʡΪ00
	{0xE0000C6D, &CmdrE0000C6D},//��͸���
	{0xE0000C70, &CmdrE0000C70}, //X��ʱ�޵����Ч��ʶ
	{0xE0000C71, &CmdrE0000C71}, //��ʱ�޵�ļ��㻬��ʱ�䣬��λ����
	{0xE0000D00, &CmdrE0000D00}, //���ʿ��ƹ��ʶ�ֵϵ��1,ʱ����
	{0xE0000D09, &CmdrE0000D09}, //���ʿ���ϵ��1,ִ��ʱ��
	{0xE0000D0F, &CmdrE0000D0F}, //���ʿ���ϵ��1�������������
	{0xE0000D10, &CmdrE0000D10}, //���ʿ��ƹ��ʶ�ֵϵ��2,ʱ����
	{0xE0000D19, &CmdrE0000D19}, //���ʿ���ϵ��2,ִ��ʱ��
	{0xE0000D1F, &CmdrE0000D1F}, //���ʿ���ϵ��2�������������
	{0xE0000D20, &CmdrE0000D20}, //���ʿ��ƹ��ʶ�ֵϵ��3,ʱ����
	{0xE0000D29, &CmdrE0000D29}, //���ʿ���ϵ��3,ִ��ʱ��
	{0xE0000D2F, &CmdrE0000D2F}, //���ʿ���ϵ��3�������������
	{0xE0000D30, &CmdrE0000D30}, //���ʿ��ƹ��ʶ�ֵϵ��4,ʱ����
	{0xE0000D39, &CmdrE0000D39}, //���ʿ���ϵ��4,ִ��ʱ��
	{0xE0000D3F, &CmdrE0000D3F}, //���ʿ���ϵ��4�������������
	{0xE0000E00, &CmdrE0000E00}, //�µ������ƣ��µ����޶�
	{0xE0000E01, &CmdrE0000E01}, //�µ������ƣ��ִε���
	{0xE0000E02, &CmdrE0000E02}, //���������ƣ����ι�����
	{0xE0000E03, &CmdrE0000E03}, //���������ƣ�ʣ�����
	{0xE0000E04, &CmdrE0000E04}, //�ն˵���������ʱ�κͷ�����
	{0xE0000E05, &CmdrE0000E05}, //
	{0xE0000E10, &CmdrE0000E10}, //���ݿ��Ƿ�Ͷ��
	{0xE0000E11, &CmdrE0000E11}, //���ݿز���

	//{0xE0001100, &CmdrE0001100},//(ֻд)�Ե��ܱ�ң����բ��ֻ����Ϣ���ʶ�Ͳ������ַ��ƥ������������
	//{0xE0001101, &CmdrE0001101},//(ֻд)�Ե��ܱ�ң�غ�բ��ֻ����Ϣ���ʶ�Ͳ������ַ��ƥ������������
	//{0xE0001102, &CmdrE0001102},//(ֻд)�Ե��ܱ��������ֻ����Ϣ���ʶ�Ͳ������ַ��ƥ����������
	//{0xE0001103, &CmdrE0001103},//(ֻд)�Ե��ܱ�����������ֻ����Ϣ���ʶ�Ͳ������ַ��ƥ������������
	//{0xE0001104, &CmdrE0001104},//(ֻд)ɾ��������

	{0xE0002000, &CmdrE0002000},//�û��Զ�������	

	{0xE0800000, &CmdrE0800000}, //�������״̬ 
	{0xE0800001, &CmdrE0800001}, //����������
	{0xE0800002, &CmdrE0800002}, //�������ַ
	{0xE0800003, &CmdrE0800003}, //������ͨ�Ź�Լ
	{0xE0800004, &CmdrE0800004}, //���ܱ�����
	{0xE0800005, &CmdrE0800005}, //�ܷ�����
	{0xE0800006, &CmdrE0800006}, //�ص��û�����
	{0xE0800007, &CmdrE0800007}, //��բ����
	{0xE0800008, &CmdrE0800008}, //��������
	{0xE0800009, &CmdrE0800009}, //�������Ӧ�ɼ��ն˵�ַ
	{0xE080000A, &CmdrE080000A}, //������˿ں�
	{0xE080000B, &CmdrE080000B}, //�˿ڲ���
	{0xE080000C, &CmdrE080000C}, //CT���
	{0xE080000D, &CmdrE080000D}, //PT���
	{0xE080000F, &CmdrE080000F}, //�����������
	{0xE0800010, &CmdrE0800010}, //��������߷�ʽ
	{0xE0800011, &CmdrE0800011}, //���ѹ(���β�)
	{0xE0800012, &CmdrE0800012}, //�����
	{0xE0800013, &CmdrE0800013}, //�û��걨����(���㵽���β�)
	{0xE0800014, &CmdrE0800014}, //��ѹ������(���㵽���β�)
	{0xE080001F, &CmdrE080001F}, //�����������
	{0xE0800021, &CmdrE0800021}, //���ܱ�ͨ������
	{0xE0800022, &CmdrE0800022}, //���ܱ�ͨ���û���
	{0xE080002F, &CmdrE080002F}, //�������ݼ���

	{0xE0800100, &CmdrE0800100}, //������ƽ�����ֵ
	{0xE0800101, &CmdrE0800101}, //������ƽ��Ȼָ���ֵ
	{0xE0800102, &CmdrE0800102}, //�ϸ��ѹ����
	{0xE0800103, &CmdrE0800103}, //�ϸ��ѹ����
	{0xE0800104, &CmdrE0800104}, //�жϵ�����������Զֵ�ı���
	{0xE0800105, &CmdrE0800105}, //�жϸ��ɹ�����Զֵ�ı���
	{0xE0800106, &CmdrE0800106}, //�жϸ��ɹ��ػָ�����Զֵ�ı���
	{0xE0800107, &CmdrE0800107}, //�������������ֵ
	{0xE0800108, &CmdrE0800108}, //������������ָ���ֵ
	{0xE0800109, &CmdrE0800109}, //�жϵ��������ɻָ�����Զֵ
	{0xE080010A, &CmdrE080010A}, //��ѹ��ƽ�����ֵ
	{0xE080010B, &CmdrE080010B}, //��ѹ��ƽ��Ȼָ���ֵ
	{0xE080010C, &CmdrE080010C}, //ʧѹ����౨�����ж���ֵ
	{0xE080010F, &CmdrE080010F}, //�����������

	{0xE0800200, &CmdrE0800200}, //��ѹ��������ֵ��ȱʡֵ5%
	{0xE0800201, &CmdrE0800201}, //������������ֵ��ȱʡֵ5%
	{0xE0800202, &CmdrE0800202}, //��ѹ�����ʱ����ָ���ֵ
	{0xE0800203, &CmdrE0800203}, //���������ʱ����ָ���ֵ
	{0xE0800204, &CmdrE0800204}, //��������������
	{0xE0800205, &CmdrE0800205}, //������������
	{0xE0800206, &CmdrE0800206}, //���г������
	{0xE0800207, &CmdrE0800207}, //��ѹƫ������
	{0xE0800208, &CmdrE0800208}, //��ѹƫ������
	{0xE0800209, &CmdrE0800209}, //Ƶ��ƫ������
	{0xE080020A, &CmdrE080020A}, //Ƶ��ƫ������
	{0xE080020B, &CmdrE080020B}, //��ѹ������ֵ
	{0xE080020C, &CmdrE080020C}, //��ѹ������ֵ
	{0xE080020D, &CmdrE080020D},  //�޹�������Ƿ�����澯������ֵ
	{0xE080020E, &CmdrE080020E},  //�޹�������Ƿ�����澯�ָ���ֵ

};

static S_CmdObjPara3D sCmdObjParametersR_3D[] =//����������
{//��DIֵ��С��������
	{0xE0000200, 0xE0000220, &CmdrE0000200},//������� 00~20
	{0xE0000230, 0xE0000250, &CmdrE0000230},//����������ñ� 30~50
	{0xE0000301, 0xE00003FE, &CmdrE0000301},//������� ��ͨ�����
	{0xE0000401, 0xE00004FE, &CmdrE0000401},//������� �м������
	{0xE0000501, 0xE00005FD, &CmdrE0000501},//���������� ģ������
	{0xE0000601, 0xE00006FD, &CmdrE0000601},//���������� ��������
	{0xE0000701, 0xE00007FD, &CmdrE0000701},//���������� ���������
	{0xE0000801, 0xE00008FD, &CmdrE0000801},//���������� �����
	{0xE0000A00, 0xE0000A1E, &CmdrE0000A00},//�˿ڲ��� 485�˿ڶ���

	{0xE0000C00, 0xE0000C0F, &CmdrE0000C00},//���ز��� ���ؽӵ�����
	{0xE0000C10, 0xE0000C1F, &CmdrE0000C10},//���ز��� �������״̬
	//{0xE0000C20, 0xE0000C2F, &CmdrE0000C20},//(ֻд)���ز��� �������״̬
	{0xE0000C30, 0xE0000C3F, &CmdrE0000C30},//���ز��� ��������ִ�
	{0xE0000D01, 0xE0000D08, &CmdrE0000D01},//���ʿ���ϵ��1��ʱ��(1-8)
	{0xE0000D11, 0xE0000D18, &CmdrE0000D11},//���ʿ���ϵ��2��ʱ��(1-8)
	{0xE0000D21, 0xE0000D28, &CmdrE0000D21},//���ʿ���ϵ��3��ʱ��(1-8)
	{0xE0000D31, 0xE0000D38, &CmdrE0000D31},//���ʿ���ϵ��4��ʱ��(1-8)

	{0xFFFFFFFF, 0xFFFFFFFF, NULL},//end
};

int C_AFNCMD_RPARAM::Execute(const S_CmdObjs &CmdObjs, strings &strResponseInfos)
{
	if (CmdObjs.empty())
		return FRAME_PROCESS_RESPONSE_DENY;
	for (S_CmdObjs::const_iterator iterNow = CmdObjs.begin(); iterNow != CmdObjs.end(); advance(iterNow, 1))
	{
		C_CmdReceiver *pCmdReveiver = GetCmdReceiver(iterNow->m_DI);
		if (pCmdReveiver == NULL)
			return FRAME_PROCESS_RESPONSE_DENY;
		strings strResponse;
		if (pCmdReveiver->Prompt() != NULL)
			logit<<"�����:"<<pCmdReveiver->Prompt()<<endl;
		pCmdReveiver->SetPackageLength(m_PackageLength);
		if (pCmdReveiver->Action(*iterNow, strResponse) == false)
			return FRAME_PROCESS_RESPONSE_DENY;
		for (size_t i = 0; i < strResponse.size(); i++)
			strResponseInfos.push_back(strResponse[i]);
	}
	if (strResponseInfos.empty())
		return FRAME_PROCESS_RESPONSE_CONFIRM;
	return FRAME_PROCESS_RESPONSE_DATA;
}

//������ȡ�����еľ�����������߲��Һ���(����Flyweightģʽ)
C_CmdReceiver *C_AFNCMD_RPARAM::GetCmdReceiver(INT32U DI)
{
	const S_CmdObjPara CmdObjNow = {DI, NULL};
	S_CmdObjPara *pFindObj = lower_bound(sCmdObjParametersR, sCmdObjParametersR+sizeof(sCmdObjParametersR)/sizeof(sCmdObjParametersR[0]), CmdObjNow);
	if (pFindObj != sCmdObjParametersR+sizeof(sCmdObjParametersR)/sizeof(sCmdObjParametersR[0])
		&& pFindObj->m_DI == DI)
		return pFindObj->m_pCmdReceiver;

	const S_CmdObjPara3D CmdObjNow3D = {DI, DI, NULL};
	S_CmdObjPara3D *pFindObj3D = lower_bound(sCmdObjParametersR_3D, sCmdObjParametersR_3D+sizeof(sCmdObjParametersR_3D)/sizeof(sCmdObjParametersR_3D[0]), CmdObjNow3D);
	if (pFindObj3D != sCmdObjParametersR_3D+sizeof(sCmdObjParametersR_3D)/sizeof(sCmdObjParametersR_3D[0]))
	{
		if (DI >= pFindObj3D->m_diBegin && DI <= pFindObj3D->m_diEnd)
			return pFindObj3D->m_pCmdReceiver;
		if (pFindObj3D != sCmdObjParametersR_3D)
			pFindObj3D -= 1;
		if (DI >= pFindObj3D->m_diBegin && DI <= pFindObj3D->m_diEnd)
			return pFindObj3D->m_pCmdReceiver;
	}
	return NULL;
}


