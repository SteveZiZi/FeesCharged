/** @file
 *  @brief �澯���Ļ�
 *  @author 
 *  @date 2012/03/30
 *  @version 0.1
 *  �澯���롢������ʾ�¼����ݡ�����ʱ��
 */
#include "almshow.h"
#include "datafile.h"
#include "sysbase.h"

const char *g_AlarmTitle[] =
{
	"����װ���ſ���",
	"����������",
	"��ѹ������",
	"������ƽ��",
	"��ѹ��ƽ��",
	"�������ƫ��",
	"A ��CT ���β��·",
	"B ��CT ���β��·",
	"C ��CT ���β��·",
	"A ��CT ���β࿪·",
	"B ��CT ���β࿪·",
	"C ��CT ���β࿪·",
	"A �ೱ������",
	"B �ೱ������",
	"C �ೱ������",
	"A ���������",
	"B ���������",
	"C ���������",
	"A �����ʧ��",
	"B �����ʧ��",
	"C �����ʧ��",
	"A ���ѹʧѹ",
	"B ���ѹʧѹ",
	"C ���ѹʧѹ",
	"ȫʧѹ",
	"A ���ѹ��ѹ",
	"B ���ѹ��ѹ",
	"C ���ѹ��ѹ",
	"A ���ѹ����",
	"B ���ѹ����",
	"C ���ѹ����",
	"A ���ѹ����",
	"B ���ѹ����",
	"C ���ѹ����",
	"A ���������",
	"B ���������",
	"C ���������",
	"�޹�������",
	"�޹�Ƿ����",
	"���ʳ���ֵ",
	"���ɹ���",
	"����ͬ�����õ�",
	"ʣ���Ѳ���",
	"ʾ���½�",
	"���ܱ����",
	"���ܱ�ͣ��",
	"���ܱ�ͨѶʧ��",
	"��澯",
	"��������ֶ�����",
	"ʱ�ӵ�ص�ѹ����",
	"�ն˵���",
	"�ն��ϵ�",
	"���ܱ���ʱ�����",
	"���ܱ�ʱ�λ���ʸ���",
	"���ܱ����峣������",
	"���ܱ��������ʸ���",
	"ң�ű�λ",
	"��ͨ������Խ��",
	"�̵�����λ",
	"���ܱ�����բʧ��",
	"����ʧ��",
	"���ܱ�ʱ���쳣",
	"���ܱ�Уʱʧ��",
	"���ܱ�ABC ��ʧѹ����",
	"���ܱ�ABC ��ʧ������",
	"���ܱ�ABC �ೱ������",
	"���ܱ����ܴ���",
	"A ���ѹƫ��Խ��",
	"B ���ѹƫ��Խ��",
	"C ���ѹƫ��Խ��",
	"Ƶ��ƫ��Խ��",
	"A ������Խ��",
	"B ������Խ��",
	"C ������Խ��",
	"��ѹ��ƽ��Խ��",
	"����δ֪���",
	"���ť�п����澯",
	"��ǿ����澯"
};
const char *g_EventTitle[] =
{
	"��̼�¼",
	"��������Զ�����",
	"��������ֶ�����",
	"A ��ʧѹ��¼",
	"B ��ʧѹ��¼",
	"C ��ʧѹ��¼",
	"A ��ʧ����¼",
	"B ��ʧ����¼",
	"C ��ʧ����¼",
	"�ն�ͣ���¼",
	"A ������¼",
	"B ������¼",
	"C ������¼",
	"�����¼���¼",
	"���������¼",
	"Уʱ��¼",
	"���ť�п�����¼",
	"��ǿ�����¼",
	"��Ϣ��֤�����¼",
	"���������¼",
	"�������俪�ռ�¼"
};

int C_AlmShow::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{
	char bufferMP[64], bufferDI[64];
	strings strTitles;

	S_AlmType AlmType = C_AlmEvt::GetAlarmNormalType(DI);
	sprintf(bufferMP, "������: %d", MP);
	if (AlmType == ALM_TYPE_ALARM)
	{
		sprintf(bufferDI, "�澯����: %08X", DI);
		strTitles.push_back(g_AlarmTitle[DI - DI_ALM_ALARM_MIN]);
		strTitles.push_back(bufferMP);
		strTitles.push_back(bufferDI);
		AlarmGeneralPrint(strRecord, strTitles);
	}
	else if (AlmType == ALM_TYPE_EVENT)
	{
		sprintf(bufferDI, "�¼�����: %08X", DI);
		strTitles.push_back(g_EventTitle[DI - DI_ALM_EVENT_MIN]);
		strTitles.push_back(bufferMP);
		strTitles.push_back(bufferDI);
	}
	else
	{
		strTitles.push_back("û�и澯����");
	}
	strTexts.append(strTitles);
	return (int)strTexts.size();
}

static char *g_AlarmStatus[] = {"�ָ�", "����", "    "};
int C_AlmShow::AlarmGeneralPrint(const string &strRecord, strings &strTexts)
{
	if (strRecord.size() < 7)
		return 0;
	char buffer[64];
	S_BYTE status = (S_BYTE)strRecord[0];
	if (status > 2) status = 2;
		
	sprintf(buffer, "�澯״̬: %s", g_AlarmStatus[status]);
	strTexts.push_back(buffer);
	strTexts.push_back("ʱ��: ");
	sprintf(buffer, " %02X-%02X-%02X %02X:%02X:%02X", strRecord[6], strRecord[5], strRecord[4], strRecord[3], strRecord[2], strRecord[1]);
	strTexts.push_back(buffer);
	return (int)strTexts.size();
}

int C_AlmShow::GetValt(const string &strRecord, strings &strTexts)
{
	S_CHAR	buffer[64];
	S_LONG index=-1;
	S_BYTE btemp[10];
	for(int i=0;i<3;i++)
	{
		btemp[0]=strRecord[++index];
		btemp[1]=strRecord[++index];
		string strAscii=C_BusiAlgo::ChangeBcd2Ascii(btemp,3,1,false); 	
		sprintf(buffer, "%C���ѹ:",'A'+i);
		strTexts.push_back(buffer);
		sprintf(buffer, " %s V",strAscii.c_str());
		strTexts.push_back(buffer);
	}
	return 1;
}

int C_AlmShow::GetCurrent(const string &strRecord, strings &strTexts)
{
	S_CHAR	buffer[64];
	S_LONG index=-1;
	S_BYTE btemp[10];
	for(int i=0;i<3;i++)
	{
		btemp[0]=strRecord[++index];
		btemp[1]=strRecord[++index];
		btemp[2]=strRecord[++index];
		string strAscii=C_BusiAlgo::ChangeBcd2Ascii(btemp,3,3,false);		
		sprintf(buffer, "%C�����:",'A'+i);
		strTexts.push_back(buffer);
		sprintf(buffer, " %s A",strAscii.c_str());
		strTexts.push_back(buffer);
	}
	return 1;
}

int C_AlmShow::GetActivePower(const string &strRecord, strings &strTexts,S_BOOL PQ)
{
	S_CHAR	buffer[64];
	for(int i=0;i<4;i++)
	{
		string strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(0+i*3).c_str(),2,4,true);		
		if(i>0)
			sprintf(buffer, "%C��%s������:", 'A'+i-1,PQ?"��":"��");
		else
			sprintf(buffer, "��%s������:", PQ?"��":"��");
		strTexts.push_back(buffer);
		sprintf(buffer, " %s %s",strAscii.c_str(),PQ?"kw":"kvar");
		strTexts.push_back(buffer);
	}
	return 1;
}

int C_AlmShow::GetPF(const string &strRecord, strings &strTexts)
{
	S_CHAR	buffer[64];
	for(int i=0;i<4;i++)
	{
		string strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(0+i*2).c_str(),1,3,false);	
		if(i>0)
			sprintf(buffer, "%C�๦������:",'A'+i-1);
		else
			sprintf(buffer, "�ܹ�������:");
		strTexts.push_back(buffer);
		sprintf(buffer, " %s",strAscii.c_str());
		strTexts.push_back(buffer);
	}
	return 1;
}


int C_AlmShow::GetPowerVal(const string &strRecord, strings &strTexts)
{
	S_CHAR	buffer[64];
	string strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(0,4).c_str(),6,2,false);
	strTexts.push_back("�����й���: ");
	sprintf(buffer, " %s kWh",strAscii.c_str());
	strTexts.push_back(buffer);

	strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(4,4).c_str(),6,2,false);
	strTexts.push_back("�����й���: ");
	sprintf(buffer, " %s kWh",strAscii.c_str());
	strTexts.push_back(buffer);

	strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(8,4).c_str(),6,2,false);
	strTexts.push_back("����޹�1: ");
	sprintf(buffer, " %s kWh",strAscii.c_str());
	strTexts.push_back(buffer);

	strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(12,4).c_str(),6,2,false);
	strTexts.push_back("����޹�2: ");
	sprintf(buffer, " %s kWh",strAscii.c_str());
	strTexts.push_back(buffer);

	return 1;
}

int C_AlmShow::GetCTPT(const string &strRecord, strings &strTexts)
{
	S_CHAR	buffer[64];
	for(int i=0;i<2;i++)
	{
		string strAscii = C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(0+i*2,2).c_str(),4,0,false);

		if(i==0)
			strTexts.push_back("PT���: ");
		else 
			strTexts.push_back("CT���: ");

		sprintf(buffer, " %s", strAscii.c_str());
		strTexts.push_back(buffer);
	}
	return 1;
}

int C_AlmShow::GetMeterConSet(const string &strRecord, strings &strTexts)
{
	S_CHAR	buffer[64];
	S_LONG index=-1;
	for(int i=0;i<2;i++)
	{
		buffer[2]=strRecord[++index];
		buffer[1]=strRecord[++index];
		buffer[0]=strRecord[++index];
		sprintf(buffer, "���%s����:",(i==0)?"�й�":"�޹�");
		strTexts.push_back(buffer);
		string strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)buffer,6,0,false);
		sprintf(buffer, " %s %s",strAscii.c_str(),(i==0)?"p/(kWh)":"p/(kVarh)");
		strTexts.push_back(buffer);
	}
	return 1;
}
string C_AlmShow::GetDwordData(const string &strRecord)
{
	string strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.c_str(),6,0,false);
	string::iterator iter = find(strAscii.begin(), strAscii.end(), '.');
	string strDes(strAscii.begin(),iter);
	return strDes;
}

int C_AlmShow_ARD01::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	C_AlmShow::Printf(MP, DI, strRecord, strTexts);
	return (int)strTexts.size();
}
int C_AlmShow_ARD02::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	C_AlmShow::Printf(MP, DI, strRecord, strTexts);
	int offset = 7;
	string::const_iterator iterBegin = strRecord.begin() + offset; // skip time string
	string::const_iterator iterEnd = strRecord.end();

	offset = 16;
	if (distance(iterBegin, iterEnd) >= offset)
	{
		GetPowerVal(string(iterBegin,iterBegin+offset),strTexts);
		advance(iterBegin, offset);
	}

	offset = 18;
	if (distance(iterBegin, iterEnd) >= offset)
	{
		GetValt(string(iterBegin,iterBegin+offset),strTexts);
		advance(iterBegin, offset);
	}

	offset = 18;
	if (distance(iterBegin, iterEnd) >= offset)
	{
		GetCurrent(string(iterBegin,iterBegin+offset),strTexts);
		advance(iterBegin, offset);
	}
	
	offset = 12;
	if (distance(iterBegin, iterEnd) >= offset)
	{
		GetActivePower(string(iterBegin,iterBegin+offset),strTexts,true);
		advance(iterBegin, offset);
	}

	offset = 12;
	if (distance(iterBegin, iterEnd) >= offset)
	{
		GetActivePower(string(iterBegin,iterBegin+offset),strTexts,false);
		advance(iterBegin, offset);
	}

	offset = 8;
	if (distance(iterBegin, iterEnd) >= offset)
	{
		GetPF(string(iterBegin,iterBegin+offset),strTexts);	
		advance(iterBegin, offset);
	}
	return (int)strTexts.size();
}
int C_AlmShow_ARD03::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	C_AlmShow::Printf(MP, DI, strRecord, strTexts);
	int offset = 7;
	string::const_iterator iterBegin = strRecord.begin() + offset; // skip time string
	string::const_iterator iterEnd = strRecord.end();

	offset = 16;
	if (distance(iterBegin, iterEnd) >= offset)
	{
		GetPowerVal(string(iterBegin,iterBegin+offset),strTexts);
		advance(iterBegin, offset);
	}

	offset = 18;
	if (distance(iterBegin, iterEnd) >= offset)
	{
		GetValt(string(iterBegin,iterBegin+offset),strTexts);
		advance(iterBegin, offset);
	}

	offset = 18;
	if (distance(iterBegin, iterEnd) >= offset)
	{
		GetCurrent(string(iterBegin,iterBegin+offset),strTexts);
		advance(iterBegin, offset);
	}
	
	offset = 12;
	if (distance(iterBegin, iterEnd) >= offset)
	{
		GetActivePower(string(iterBegin,iterBegin+offset),strTexts,true);
		advance(iterBegin, offset);
	}

	offset = 12;
	if (distance(iterBegin, iterEnd) >= offset)
	{
		GetActivePower(string(iterBegin,iterBegin+offset),strTexts,false);
		advance(iterBegin, offset);
	}

	offset = 16;
	if (distance(iterBegin, iterEnd) >= offset)
	{
		GetPowerVal(string(iterBegin,iterBegin+offset),strTexts);
		advance(iterBegin, offset);
	}

	offset = 18;
	if (distance(iterBegin, iterEnd) >= offset)
	{
		GetValt(string(iterBegin,iterBegin+offset),strTexts);
		advance(iterBegin, offset);
	}

	offset = 18;
	if (distance(iterBegin, iterEnd) >= offset)
	{
		GetCurrent(string(iterBegin,iterBegin+offset),strTexts);
		advance(iterBegin, offset);
	}
	
	offset = 12;
	if (distance(iterBegin, iterEnd) >= offset)
	{
		GetActivePower(string(iterBegin,iterBegin+offset),strTexts,true);
		advance(iterBegin, offset);
	}

	offset = 12;
	if (distance(iterBegin, iterEnd) >= offset)
	{
		GetActivePower(string(iterBegin,iterBegin+offset),strTexts,false);
		advance(iterBegin, offset);
	}
	return (int)strTexts.size();
}
int C_AlmShow_ARD04::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	C_AlmShow::Printf(MP, DI, strRecord, strTexts);
	int offset = 7;
	string::const_iterator iterBegin = strRecord.begin() + offset; // skip time string
	string::const_iterator iterEnd = strRecord.end();

	offset = 16;
	if (distance(iterBegin, iterEnd) >= offset)
	{
		GetPowerVal(string(iterBegin,iterBegin+offset),strTexts);
		advance(iterBegin, offset);
	}

	offset = 4;
	if (distance(iterBegin, iterEnd) >= offset)
	{
		S_CHAR buffer[64];	
		string strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)&(*iterBegin),8,0,false);
		strTexts.push_back("ʣ����: ");
		sprintf(buffer, " %s Ԫ",strAscii.c_str());
		strTexts.push_back(buffer);
	}
	return (int)strTexts.size();
}
int C_AlmShow_ARD05::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	C_AlmShow::Printf(MP, DI, strRecord, strTexts);
	int offset = 7;
	string::const_iterator iterBegin = strRecord.begin() + offset; // skip time string
	string::const_iterator iterEnd = strRecord.end();

	offset = 16;
	if (distance(iterBegin, iterEnd) >= offset)
	{
		GetPowerVal(string(iterBegin,iterBegin+offset),strTexts);	
		advance(iterBegin, offset);
	}

	offset = 4;
	if (distance(iterBegin, iterEnd) >= offset)
	{
		GetCTPT(string(iterBegin,iterBegin+offset),strTexts);
		advance(iterBegin, offset);
	}

	offset = 4;
	if (distance(iterBegin, iterEnd) >= offset)
	{
		GetCTPT(string(iterBegin,iterBegin+offset),strTexts);
		advance(iterBegin, offset);
	}
	return (int)strTexts.size();
}
int C_AlmShow_ARD06::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	C_AlmShow::Printf(MP, DI, strRecord, strTexts);
	int offset = 7;
	string::const_iterator iterBegin = strRecord.begin() + offset; // skip time string
	string::const_iterator iterEnd = strRecord.end();

	offset = 16;
	if (distance(iterBegin, iterEnd) >= offset)
	{
		GetPowerVal(string(iterBegin,iterBegin+offset),strTexts);	
		advance(iterBegin, offset);
	}

	offset = 6;
	if (distance(iterBegin, iterEnd) >= offset)
	{
		GetMeterConSet(string(iterBegin,iterBegin+offset),strTexts);
		advance(iterBegin, offset);
	}

	offset = 6;
	if (distance(iterBegin, iterEnd) >= offset)
	{
		GetMeterConSet(string(iterBegin,iterBegin+offset),strTexts);
		advance(iterBegin, offset);
	}
	return (int)strTexts.size();
}
int C_AlmShow_ARD07::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	C_AlmShow::Printf(MP, DI, strRecord, strTexts);
	S_LONG index=6;
	S_CHAR buffer[64];	

	GetPowerVal(strRecord.substr(++index,16),strTexts);	

	strTexts.push_back("�����й���: ");
	strTexts.push_back("�������: ");
	string strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(23,3).c_str(),2,4,false);
	sprintf(buffer, " %s kW",strAscii.c_str());
	strTexts.push_back(buffer);
	index=26;
	strTexts.push_back("����ʱ��: ");
	sprintf(buffer, " %02X-%02X-%02X %02X:%02X", strRecord[index+4], strRecord[index+3], strRecord[2+index], strRecord[1+index], strRecord[0+index]);
	strTexts.push_back(buffer);
		
	strTexts.push_back("�����й���: ");
	strTexts.push_back("�������: ");
	strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(31,3).c_str(),2,4,false);
	sprintf(buffer, " %s kW",strAscii.c_str());
	strTexts.push_back(buffer);
	index=34;
	strTexts.push_back("����ʱ��: ");
	sprintf(buffer, " %02X-%02X-%02X %02X:%02X", strRecord[4+index], strRecord[5+index], strRecord[2+index], strRecord[1+index], strRecord[0+index]);
	strTexts.push_back(buffer);
	
	strTexts.push_back("����޹�1��: ");
	strTexts.push_back("�������: ");
	strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(39,3).c_str(),2,4,false);
	sprintf(buffer, " %s kW",strAscii.c_str());
	strTexts.push_back(buffer);
	index=42;	
	strTexts.push_back("����ʱ��: ");
	sprintf(buffer, " %02X-%02X-%02X %02X:%02X", strRecord[4+index], strRecord[3+index], strRecord[2+index], strRecord[1+index], strRecord[0+index]);
	strTexts.push_back(buffer);

	
	strTexts.push_back("����޹�2��: ");
	strTexts.push_back("�������: ");
	strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(47,3).c_str(),2,4,false);
	sprintf(buffer, " %s kW",strAscii.c_str());
	strTexts.push_back(buffer);
	index=50;	
	strTexts.push_back("����ʱ��: ");
	sprintf(buffer, " %02X-%02X-%02X %02X:%02X", strRecord[4+index], strRecord[3+index], strRecord[2+index], strRecord[1+index], strRecord[0+index]);
	strTexts.push_back(buffer);
	return (int)strTexts.size();
}
int C_AlmShow_ARD08::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	C_AlmShow::Printf(MP, DI, strRecord, strTexts);
	return (int)strTexts.size();
}
int C_AlmShow_ARD09::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	C_AlmShow::Printf(MP, DI, strRecord, strTexts);
	return (int)strTexts.size();
}
int C_AlmShow_ARD10::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	C_AlmShow::Printf(MP, DI, strRecord, strTexts);
	int offset = 7;
	string::const_iterator iterBegin = strRecord.begin() + offset; // skip time string
	string::const_iterator iterEnd = strRecord.end();

	offset = 3;
	if (distance(iterBegin, iterEnd) >= offset)
	{
		S_CHAR buffer[64];	
		strTexts.push_back("��ͨ������: ");
		string strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)&(*iterBegin),6,0,false);
		sprintf(buffer, " %s kB",strAscii.c_str());
		strTexts.push_back(buffer);
	}
	return (int)strTexts.size();
}
int C_AlmShow_ARD11::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	C_AlmShow::Printf(MP, DI, strRecord, strTexts);
	int offset = 7;
	string::const_iterator iterBegin = strRecord.begin() + offset; // skip time string
	string::const_iterator iterEnd = strRecord.end();

	offset = 3;
	if (distance(iterBegin, iterEnd) >= offset)
	{
		S_CHAR buffer[64];	
		string strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)&(*iterBegin),6,0,false);
		sprintf(buffer, "��������:%s", strAscii.c_str());
		strTexts.push_back(buffer);
		advance(iterBegin, offset);
	}

	offset = 24;
	if (distance(iterBegin, iterEnd) >= offset)
	{
		GetSubValue(string(iterBegin,iterBegin+offset),strTexts);
		advance(iterBegin, offset);
	}

	offset = 24;
	if (distance(iterBegin, iterEnd) >= offset)
	{
		GetSubValue(string(iterBegin,iterBegin+offset),strTexts);
		advance(iterBegin, offset);
	}
	return (int)strTexts.size();
}

int C_AlmShow_ARD11::GetSubValue(const string &strRecord, strings &strTexts)
{
	S_CHAR	buffer[64];
	S_LONG index=-1;
	string strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(0,4).c_str(),6,2,false);
	strTexts.push_back("�����й���: ");
	sprintf(buffer, " %s kWh",strAscii.c_str());
	strTexts.push_back(buffer);
	index=3;
	strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(4,4).c_str(),6,2,false);
	strTexts.push_back("�����й���: ");
	sprintf(buffer, " %s kWh",strAscii.c_str());
	strTexts.push_back(buffer);

	index=7;
	for(int i=0;i<4;i++)
	{
		strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(8+i*4,4).c_str(),6,2,false);
		sprintf(buffer, "��%d�����޹��ܵ���:",(i+1));
		strTexts.push_back(buffer);
		sprintf(buffer, " %s kvarh",strAscii.c_str());
		strTexts.push_back(buffer);
	}
	return 1;
}

int C_AlmShow_ARD12::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	C_AlmShow::Printf(MP, DI, strRecord, strTexts);
	int offset = 7;
	S_CHAR buffer[128];	
	string::const_iterator iterBegin = strRecord.begin() + offset; // skip time string
	string::const_iterator iterEnd = strRecord.end();

	offset = 2;
	if (distance(iterBegin, iterEnd) >= offset)
	{
		const char *pState = "��բ";
		if (*iterBegin & 0x10)
			pState = "��բ";
		sprintf(buffer, "״̬: %s", pState);
		strTexts.push_back(buffer);
//		advance(iterBegin, offset);
	}
	return (int)strTexts.size();
}
int C_AlmShow_ARD13::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	C_AlmShow::Printf(MP, DI, strRecord, strTexts);
	int offset = 7;
	S_CHAR buffer[128];	
	string::const_iterator iterBegin = strRecord.begin() + offset; // skip time string
	string::const_iterator iterEnd = strRecord.end();

	offset = 7;
	if (distance(iterBegin, iterEnd) >= offset)
	{
		strTexts.push_back("�������: ");
		sprintf(buffer, " %02X-%02X-%02X %02X:%02X:%02X", *(iterBegin+3), *(iterBegin+2), *(iterBegin+1), *(iterBegin+6), *(iterBegin+5), *(iterBegin+4));
		strTexts.push_back(buffer);
		advance(iterBegin, offset);
	}

	offset = 6;
	if (distance(iterBegin, iterEnd) >= offset)
	{
		strTexts.push_back("�ն�ʱ��: ");
		sprintf(buffer, " %02X-%02X-%02X %02X:%02X:%02X", *(iterBegin+5), *(iterBegin+4), *(iterBegin+3), *(iterBegin+2), *(iterBegin+1), *(iterBegin+0));
		strTexts.push_back(buffer);
		advance(iterBegin, offset);
	}
	return (int)strTexts.size();
}
int C_AlmShow_ARD14::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	C_AlmShow::Printf(MP, DI, strRecord, strTexts);
	S_LONG index=6;
	S_CHAR buffer[64];	

	index=6;		
	for(int i=0;i<6;i++)
	{
		string strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(7+i*2,2).c_str(),2,2,false);
		sprintf(buffer, "%C���%S��г��������: ",'A'+i%3,(i<=2)?"ѹ":"��");
		strTexts.push_back(buffer);
		sprintf(buffer, " %s %%",strAscii.c_str());
		strTexts.push_back(buffer);
	}

	index=18;
	for(int i=0;i<3;i++)
	{
		string strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(19+i*2,2).c_str(),4,0,false);
		sprintf(buffer, "%C���%Sƫ�����ֵ: ", 'A'+i,"ѹ");
		strTexts.push_back(buffer);
		sprintf(buffer, " %s %%",strAscii.c_str());
		strTexts.push_back(buffer);
	}
	
	index=24;
	strTexts.push_back("Ƶ��ƫ�����ֵ: ");
	string strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(++index,1).c_str(),2,0,false);
	sprintf(buffer, " %s %%", strAscii.c_str());
	strTexts.push_back(buffer);

	index=25;
	strTexts.push_back("��ѹ��ƽ������ֵ: ");
	strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(++index,2).c_str(),4,0,false);
	sprintf(buffer, " %s %%",strAscii.c_str());
	strTexts.push_back(buffer);
	
	index=27;
	strTexts.push_back("��ʱ�������ֵ: ");
	strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(++index,1).c_str(),2,0,false);
	sprintf(buffer, " %s %%", strAscii.c_str());
	strTexts.push_back(buffer);
	
	index=28;
	strTexts.push_back("��ʱ�������ֵ: ");
	strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(++index,1).c_str(),2,0,false);
	sprintf(buffer, " %s %%", strAscii.c_str());
	strTexts.push_back(buffer);

	index=29;
	strTexts.push_back("��ѹ�ܻ������ֵ: ");
	strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(++index,2).c_str(),2,2,false);
	sprintf(buffer, " %s %%",strAscii.c_str());
	strTexts.push_back(buffer);
	return (int)strTexts.size();
}
int C_AlmShow_ARD15::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	C_AlmShow::Printf(MP, DI, strRecord, strTexts);
	S_LONG index=6;
	S_CHAR buffer[64];	

	index=6;
		
	for(int i=0;i<3;i++)
	{
		sprintf(buffer, "%C��ʧѹ�ܴ���:",'A'+i);
		strTexts.push_back(buffer);
		string strAscii=GetDwordData(strRecord.substr(7+i*6,3));
		sprintf(buffer, " %s ��",strAscii.c_str());
		strTexts.push_back(buffer);

		sprintf(buffer, "%C��ʧѹ��ʱ��:", 'A'+i);
		strTexts.push_back(buffer);
		strAscii=GetDwordData(strRecord.substr(7+i*6+3,3));
		sprintf(buffer, " %s ��",strAscii.c_str());
		strTexts.push_back(buffer);
	}
	return (int)strTexts.size();
}
int C_AlmShow_ARD16::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	C_AlmShow::Printf(MP, DI, strRecord, strTexts);
	S_LONG index=6;
	S_CHAR buffer[64];	

	index=6;
		
	for(int i=0;i<3;i++)
	{
		sprintf(buffer, "%C��ʧ���ܴ���:",'A'+i);
		strTexts.push_back(buffer);
		string strAscii=GetDwordData(strRecord.substr(7+i*6,3));
		sprintf(buffer, " %s ��",strAscii.c_str());
		strTexts.push_back(buffer);

		sprintf(buffer, "%C��ʧ����ʱ��:",'A'+i);
		strTexts.push_back(buffer);
		strAscii=GetDwordData(strRecord.substr(7+i*6+3,3));
		sprintf(buffer, " %s ��",strAscii.c_str());
		strTexts.push_back(buffer);
	}
	return (int)strTexts.size();
}
int C_AlmShow_ARD17::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	C_AlmShow::Printf(MP, DI, strRecord, strTexts);
	S_LONG index=6;
	S_CHAR buffer[64];	

	index=6;
		
	for(int i=0;i<3;i++)
	{
		sprintf(buffer, "%C�ೱ�������ܴ���:", 'A'+i);
		strTexts.push_back(buffer);
		string strAscii=GetDwordData(strRecord.substr(7+i*6,3));
		sprintf(buffer, " %s ��",strAscii.c_str());
		strTexts.push_back(buffer);

		sprintf(buffer, "%C�ೱ��������ʱ��:",'A'+i);
		strTexts.push_back(buffer);
		strAscii=GetDwordData(strRecord.substr(7+i*6+3,3));
		sprintf(buffer, " %s ��",strAscii.c_str());
		strTexts.push_back(buffer);
	}
	return (int)strTexts.size();
}
int C_AlmShow_ARD18::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	C_AlmShow::Printf(MP, DI, strRecord, strTexts);
	S_CHAR buffer[64];	

	string strAscii=GetDwordData(strRecord.substr(7,3));
	strTexts.push_back("����ܴ���: ");
	sprintf(buffer, " %s ��", strAscii.c_str());
	strTexts.push_back(buffer);

	return (int)strTexts.size();
}
int C_AlmShow_ARD19::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	C_AlmShow::Printf(MP, DI, strRecord, strTexts);
	S_LONG index=6;

	GetPowerVal(strRecord.substr(++index,16),strTexts); 	
	return (int)strTexts.size();
}
int C_AlmShow_ARD20::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	C_AlmShow::Printf(MP, DI, strRecord, strTexts);
	S_LONG index=6;
	S_CHAR buffer[64];	

	++index;
	if(strRecord[index]==0x01)
		strTexts.push_back("�ݽ�");
	else if(strRecord[index]==0x02)
		strTexts.push_back("����");
	else if(strRecord[index]==0x03)
		strTexts.push_back("��ʱ�ж�");


	index=7;
	strTexts.push_back("����ʱ��: ");
	string strAscii=GetDwordData(strRecord.substr(++index,2));
	sprintf(buffer, " %s ����", strAscii.c_str());
	strTexts.push_back(buffer);

	index=9;
	strTexts.push_back("��ؼ�ֵ: ");
	strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(++index,2).c_str(),3,1,false);	
	sprintf(buffer, " %s ����", strAscii.c_str());
	strTexts.push_back(buffer);
	return (int)strTexts.size();
}

int C_AlmShow_ARD21::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{
	C_AlmShow::Printf(MP, DI, strRecord, strTexts);
	S_CHAR buffer[64];	

	C_BITSET<256> bitset((S_BYTE*)strRecord.data() + 7, 256);
	for(int i=0;i<bitset.GetBitSize();i++)
	{
		if(bitset.TestBit(i))
		{
			sprintf(buffer, " %d �ű���ʧ��",i);
			strTexts.push_back(buffer);
		}
	}
	return (int)strTexts.size();
}

int C_AlmShow_ARD22::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{
	C_AlmShow::Printf(MP, DI, strRecord, strTexts);
	S_LONG index=7;
	S_CHAR buffer[64];	

	S_LONG count=(S_LONG)strRecord[++index];
	sprintf(buffer,"�����ֵ�� %d ��",count);
	strTexts.push_back(buffer);
	index=8;
	for(int i=0;i<count;i++)
	{		
		buffer[5]=strRecord[++index];
		buffer[4]=strRecord[++index];
		buffer[3]=strRecord[++index];
		buffer[2]=strRecord[++index];
		buffer[1]=strRecord[++index];
		buffer[0]=strRecord[++index];
		strTexts.push_back("����ַ: ");
		sprintf(buffer, " %02X%02X%02X%02X%02X%02X",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5]);
		strTexts.push_back(buffer);
		
		S_BYTE btemp=strRecord[++index];
		if(btemp&0x80)
			strTexts.push_back("����:A��");
		else if(btemp&0x40)
			strTexts.push_back("����:B��");
		else if(btemp&0x20)
			strTexts.push_back("����:C��");

		count=(S_LONG)(btemp&0x0f);
		sprintf(buffer,"�ز��ź�ǿ��: %d",count);
		strTexts.push_back(buffer);

		btemp=(strRecord[++index]&0x03);
		strTexts.push_back("����Լ: ");
		if(btemp==0x00)
			strTexts.push_back(" "NW_MRULE_NAME_DLT645_1997);
		else if(btemp==0x01)			
			strTexts.push_back(" "NW_MRULE_NAME_DLT645_2007);
		else
			strTexts.push_back(" ����");		
	}
	return (int)strTexts.size();
}


int C_AlmShow_ERD01::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	if (strRecord.size() < 6)
		return 0;
	char buffer[64];

	strTexts.push_back("���ʱ��: ");		
	sprintf(buffer, " %02X-%02X-%02X %02X:%02X:%02X", strRecord[0], strRecord[1], strRecord[2], strRecord[3], strRecord[4], strRecord[5]);
	strTexts.push_back(buffer);	
	return (int)strTexts.size();
}
int C_AlmShow_ERD02::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	if (strRecord.size() < 6)
		return 0;
	char buffer[64];

	strTexts.push_back("�����������ʱ��: ");		
	sprintf(buffer, " %02X-%02X-%02X %02X:%02X:%02X", strRecord[0], strRecord[1], strRecord[2], strRecord[3], strRecord[4], strRecord[5]);
	strTexts.push_back(buffer);	
	return (int)strTexts.size();
}
int C_AlmShow_ERD03::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	if (strRecord.size() < 12)
		return 0;
	char buffer[64];

	strTexts.push_back("��ʼʱ��: ");		
	sprintf(buffer, " %02X-%02X-%02X %02X:%02X:%02X", strRecord[0], strRecord[1], strRecord[2], strRecord[3], strRecord[4], strRecord[5]);
	strTexts.push_back(buffer);	
	strTexts.push_back("����ʱ��: ");		
	sprintf(buffer, " %02X-%02X-%02X %02X:%02X:%02X", strRecord[6], strRecord[7], strRecord[8], strRecord[9], strRecord[10], strRecord[11]);
	strTexts.push_back(buffer);	

	return (int)strTexts.size();
}
int C_AlmShow_ERD04::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	if (strRecord.size() < 12)
		return 0;
	char buffer[64];

	strTexts.push_back("ͣ��ʱ��: ");		
	sprintf(buffer, " %02X-%02X-%02X %02X:%02X:%02X", strRecord[0], strRecord[1], strRecord[2], strRecord[3], strRecord[4], strRecord[5]);
	strTexts.push_back(buffer);	
	strTexts.push_back("����ʱ��: ");		
	sprintf(buffer, " %02X-%02X-%02X %02X:%02X:%02X", strRecord[6], strRecord[7], strRecord[8], strRecord[9], strRecord[10], strRecord[11]);
	strTexts.push_back(buffer);	
	return (int)strTexts.size();
}
int C_AlmShow_ERD05::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	if (strRecord.size() < 12)
		return 0;
	char buffer[64];

	strTexts.push_back("��ʼʱ��: ");		
	sprintf(buffer, " %02X-%02X-%02X %02X:%02X:%02X", strRecord[0], strRecord[1], strRecord[2], strRecord[3], strRecord[4], strRecord[5]);
	strTexts.push_back(buffer);	
	strTexts.push_back("����ʱ��: ");		
	sprintf(buffer, " %02X-%02X-%02X %02X:%02X:%02X", strRecord[6], strRecord[7], strRecord[8], strRecord[9], strRecord[10], strRecord[11]);
	strTexts.push_back(buffer);	
	return (int)strTexts.size();
}
int C_AlmShow_ERD06::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	if (strRecord.size() < 7)
		return 0;
	char buffer[64];

	strTexts.push_back("�����¼�����ʱ��: ");		
	sprintf(buffer, " %02X-%02X-%02X %02X:%02X:%02X", strRecord[0], strRecord[1], strRecord[2], strRecord[3], strRecord[4], strRecord[5]);
	strTexts.push_back(buffer);	
	if((S_BYTE)strRecord[6]==(S_BYTE)0x01)
	{
		sprintf(buffer, "��������:%s", "��բ");
	}
	else if((S_BYTE)strRecord[0]==(S_BYTE)0x01)
	{
		sprintf(buffer, "��������:%s", "��բ");
	}
	else if((S_BYTE)strRecord[0]==(S_BYTE)0x01)
	{
		sprintf(buffer, "��������:%s", "����");
	}
	else if((S_BYTE)strRecord[0]==(S_BYTE)0x01)
	{
		sprintf(buffer, "��������:%s", "������");
	}
	
	strTexts.push_back(buffer);	
	return (int)strTexts.size();
}
int C_AlmShow_ERD07::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	if (strRecord.size() < 6)
		return 0;
	char buffer[64];

	strTexts.push_back("��Ϣ��֤������ʱ��: ");
	sprintf(buffer, " %02X-%02X-%02X %02X:%02X:%02X", strRecord[0], strRecord[1], strRecord[2], strRecord[3], strRecord[4], strRecord[5]);
	strTexts.push_back(buffer);	
	return (int)strTexts.size();
}
int C_AlmShow_ERD08::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	if (strRecord.size() < 6)
		return 0;
	char buffer[64];

	strTexts.push_back("�����������ʱ��: ");
	sprintf(buffer, " %02X-%02X-%02X %02X:%02X:%02X", strRecord[0], strRecord[1], strRecord[2], strRecord[3], strRecord[4], strRecord[5]);
	strTexts.push_back(buffer);	
	return (int)strTexts.size();
}
int C_AlmShow_ERD09::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	if (strRecord.size() < 6)
		return 0;
	char buffer[64];

	strTexts.push_back("Уʱ����ʱ��: ");
	sprintf(buffer, " %02X-%02X-%02X %02X:%02X:%02X", strRecord[0], strRecord[1], strRecord[2], strRecord[3], strRecord[4], strRecord[5]);
	strTexts.push_back(buffer);	
	return (int)strTexts.size();
}

//
//
//
static C_AlmShow_ARD01 AlmShow_ARD01;
static C_AlmShow_ARD02 AlmShow_ARD02;
static C_AlmShow_ARD03 AlmShow_ARD03;
static C_AlmShow_ARD04 AlmShow_ARD04;
static C_AlmShow_ARD05 AlmShow_ARD05;
static C_AlmShow_ARD06 AlmShow_ARD06;
static C_AlmShow_ARD07 AlmShow_ARD07;
static C_AlmShow_ARD08 AlmShow_ARD08;
static C_AlmShow_ARD09 AlmShow_ARD09;
static C_AlmShow_ARD10 AlmShow_ARD10;
static C_AlmShow_ARD11 AlmShow_ARD11;
static C_AlmShow_ARD12 AlmShow_ARD12;
static C_AlmShow_ARD13 AlmShow_ARD13;
static C_AlmShow_ARD14 AlmShow_ARD14;
static C_AlmShow_ARD15 AlmShow_ARD15;
static C_AlmShow_ARD16 AlmShow_ARD16;
static C_AlmShow_ARD17 AlmShow_ARD17;
static C_AlmShow_ARD18 AlmShow_ARD18;
static C_AlmShow_ARD19 AlmShow_ARD19;
static C_AlmShow_ARD20 AlmShow_ARD20;
static C_AlmShow_ARD21 AlmShow_ARD21;
static C_AlmShow_ARD22 AlmShow_ARD22;

static C_AlmShow_ERD01 AlmShow_ERD01;
static C_AlmShow_ERD02 AlmShow_ERD02;
static C_AlmShow_ERD03 AlmShow_ERD03;
static C_AlmShow_ERD04 AlmShow_ERD04;
static C_AlmShow_ERD05 AlmShow_ERD05;
static C_AlmShow_ERD06 AlmShow_ERD06;
static C_AlmShow_ERD07 AlmShow_ERD07;
static C_AlmShow_ERD08 AlmShow_ERD08;
static C_AlmShow_ERD09 AlmShow_ERD09;

//C_AlmShow *g_AlmShowObj[] =
//{
//	&AlmShow_ARD02, // 1"����װ���Ŵ�",
//	&AlmShow_ARD01, // 2"�Ե�",
//	&AlmShow_ARD02, // 3"������·������",
//	&AlmShow_ARD02, // 4"��ѹ��·������",
//	&AlmShow_ARD02, // 5"������ƽ��",
//	&AlmShow_ARD02, // 6"�������ƫ��",
//	&AlmShow_ARD02, // 7"A��CT���β��·",
//	&AlmShow_ARD02, // 8"B��CT���β��·",
//	&AlmShow_ARD02, // 9"C��CT���β��·",
//	&AlmShow_ARD02, //10"A��CT���β࿪·",
//	&AlmShow_ARD02, //11"B��CT���β࿪·",
//	&AlmShow_ARD02, //12"C��CT���β࿪·",
//	&AlmShow_ARD02, //13"A�ೱ������",
//	&AlmShow_ARD02, //14"B�ೱ������",
//	&AlmShow_ARD02, //15"C�ೱ������",
//	&AlmShow_ARD02, //16"A���������",
//	&AlmShow_ARD02, //17"B���������",
//	&AlmShow_ARD02, //18"C���������",
//	&AlmShow_ARD02, //19"A�����ʧ��",
//	&AlmShow_ARD02, //20"B�����ʧ��",
//	&AlmShow_ARD02, //21"C�����ʧ��",
//	&AlmShow_ARD02, //22"A���ѹʧѹ",
//	&AlmShow_ARD02, //23"B���ѹʧѹ",
//	&AlmShow_ARD02, //24"C���ѹʧѹ",
//	&AlmShow_ARD02, //25"ȫʧѹ",
//	&AlmShow_ARD02, //26"A���ѹ��ѹ",
//	&AlmShow_ARD02, //27"B���ѹ��ѹ",
//	&AlmShow_ARD02, //28"C���ѹ��ѹ",
//	&AlmShow_ARD02, //29"A���ѹ����",
//	&AlmShow_ARD02, //30"B���ѹ����",
//	&AlmShow_ARD02, //31"C���ѹ����",
//	&AlmShow_ARD14, //32"A���ѹ�����ʳ���",
//	&AlmShow_ARD14, //33"B���ѹ�����ʳ���",
//	&AlmShow_ARD14, //34"C���ѹ�����ʳ���",
//	&AlmShow_ARD14, //35"A����������ʳ���",
//	&AlmShow_ARD14, //36"B����������ʳ���",
//	&AlmShow_ARD14, //37"C����������ʳ���",
//	&AlmShow_ARD02, //38"�޹�������",
//	&AlmShow_ARD02, //39"�޹�Ƿ����",
//	&AlmShow_ARD02, //40"���ʳ���ֵ",
//	&AlmShow_ARD02, //41"���ɹ���",
//	&AlmShow_ARD02, //42"����ͬ�����õ�",
//	&AlmShow_ARD04, //43"ʣ�๺��������",
//	&AlmShow_ARD03, //44"ʾ���½�",
//	&AlmShow_ARD03, //45"���ܱ����",
//	&AlmShow_ARD02, //46"���ܱ�ͣ��",
//	&AlmShow_ARD01, //47"���ܱ�ͨ��ʧ��",
//	&AlmShow_ARD03, //48"�����",
//	&AlmShow_ARD07, //49"��������������Զ�����",
//	&AlmShow_ARD07, //50"��������ֶ�����",
//	&AlmShow_ARD01, //51"ʱ�ӵ�ص�ѹ����",
//	&AlmShow_ARD01, //52"����ص�ѹ����",
//	&AlmShow_ARD08, //53"���ܱ�״̬�ֱ仯",
//	&AlmShow_ARD02, //54"�ն�ͣ��",
//	&AlmShow_ARD02, //55"���ܱ���ʱ�����",
//	&AlmShow_ARD01, //56"���ܱ�ʱ�λ���ʸ���",
//	&AlmShow_ARD06, //57"���ܱ����峣������",
//	&AlmShow_ARD05, //58"���ܱ�Ļ��������ʸ���",
//	&AlmShow_ARD09, //59"ң�ű�λ",
//	&AlmShow_ARD10, //60"��ͨ������Խ��",
//	&AlmShow_ARD11, //61"��������Խ��",
//	&AlmShow_ARD12, //62"�̵�����λ",
//	&AlmShow_ARD12, //63"���ܱ�����բʧ��",
//	&AlmShow_ARD01, //64"����ʧ��",
//	&AlmShow_ARD13, //65"���ܱ�ʱ�ӳ���",
//	&AlmShow_ARD01, //66"���ܱ�Уʱʧ��",
//	&AlmShow_ARD02, //67"��һ����բ",
//	&AlmShow_ARD02, //68"�ڶ�����բ",
//	&AlmShow_ARD02, //69"��������բ",
//	&AlmShow_ARD02, //70"��������բ",
//	&AlmShow_ARD02, //71"��������բ",
//	&AlmShow_ARD02, //72"��������բ",
//	&AlmShow_ARD02, //73"��������բ",
//	&AlmShow_ARD02, //74"�ڰ�����բ",
//	&AlmShow_ARD15, //75"���ܱ�A��B��C��ʧѹ�ܴ���",
//	&AlmShow_ARD16, //76"���ܱ�A��B��C��ʧ���ܴ���",
//	&AlmShow_ARD17, //77"���ܱ�A��B��C�ೱ�������ܴ���",
//	&AlmShow_ARD18, //78"���ܱ����ܴ���",
//	&AlmShow_ARD19, //79"���ܱ�ͣ��",
//	&AlmShow_ARD20, //80"A���ѹ��̬�¼�",
//	&AlmShow_ARD20, //81"B���ѹ��̬�¼�",
//	&AlmShow_ARD20, //82"C���ѹ��̬�¼�",
//	&AlmShow_ARD14, //83"A���ѹƫ��Խ��",
//	&AlmShow_ARD14, //84"B���ѹƫ��Խ��",
//	&AlmShow_ARD14, //85"C���ѹƫ��Խ��",
//	&AlmShow_ARD14, //86"Ƶ��ƫ��Խ��",
//	&AlmShow_ARD14, //87"A������Խ��",
//	&AlmShow_ARD14, //88"B������Խ��",
//	&AlmShow_ARD14, //89"C������Խ��",
//	&AlmShow_ARD14  //90"��ѹ��ƽ��Խ��",
//};

C_AlmShow *g_AlmShowObj[] =
{
	&AlmShow_ARD02, //  01 ����װ���ſ��� ARD2
	&AlmShow_ARD02, //  02 ���������� ARD2
	&AlmShow_ARD02, //	03 ��ѹ������ ARD2
	&AlmShow_ARD02, //	04 ������ƽ�� ARD2
	&AlmShow_ARD02, //	05 ��ѹ��ƽ�� ARD2
	&AlmShow_ARD02, //	06 �������ƫ�� ARD2
	&AlmShow_ARD02, //	07 A ��CT ���β��· ARD2
	&AlmShow_ARD02, //	08 B ��CT ���β��· ARD2
	&AlmShow_ARD02, //	09 C ��CT ���β��· ARD2
	&AlmShow_ARD02, //	10 A ��CT ���β࿪· ARD2
	&AlmShow_ARD02, //	11 B ��CT ���β࿪· ARD2
	&AlmShow_ARD02, //	12 C ��CT ���β࿪· ARD2
	&AlmShow_ARD02, //	13 A �ೱ������ ARD2
	&AlmShow_ARD02, //	14 B �ೱ������ ARD2
	&AlmShow_ARD02, //	15 C �ೱ������ ARD2
	&AlmShow_ARD02, //	16 A ��������� ARD2
	&AlmShow_ARD02, //	17 B ��������� ARD2
	&AlmShow_ARD02, //	18 C ��������� ARD2
	&AlmShow_ARD02, //	19 A �����ʧ�� ARD2
	&AlmShow_ARD02, //	20 B �����ʧ�� ARD2
	&AlmShow_ARD02, //	21 C �����ʧ�� ARD2
	&AlmShow_ARD02, //	22 A ���ѹʧѹ ARD2
	&AlmShow_ARD02, //	23 B ���ѹʧѹ ARD2
	&AlmShow_ARD02, //	24 C ���ѹʧѹ ARD2
	&AlmShow_ARD02, //	25 ȫʧѹ ARD2
	&AlmShow_ARD02, //	26 A ���ѹ��ѹ ARD2
	&AlmShow_ARD02, //	27 B ���ѹ��ѹ ARD2
	&AlmShow_ARD02, //	28 C ���ѹ��ѹ ARD2
	&AlmShow_ARD02, //	29 A ���ѹ���� ARD2
	&AlmShow_ARD02, //	30 B ���ѹ���� ARD2
	&AlmShow_ARD02, //	31 C ���ѹ���� ARD2
	&AlmShow_ARD14, //	32 A ���ѹ���� ARD14
	&AlmShow_ARD14, //	33 B ���ѹ���� ARD14
	&AlmShow_ARD14, //	34 C ���ѹ���� ARD14
	&AlmShow_ARD14, //	35 A ��������� ARD14
	&AlmShow_ARD14, //	36 B ��������� ARD14
	&AlmShow_ARD14, //	37 C ��������� ARD14
	&AlmShow_ARD02, //	38 �޹������� ARD2
	&AlmShow_ARD02, //	39 �޹�Ƿ���� ARD2
	&AlmShow_ARD02, //	40 ���ʳ���ֵ ARD2
	&AlmShow_ARD02, //	41 ���ɹ��� ARD2
	&AlmShow_ARD02, //	42 ����ͬ�����õ� ARD2
	&AlmShow_ARD04, //	43 ʣ���Ѳ��� ARD4
	&AlmShow_ARD03, //	44 ʾ���½� ARD3
	&AlmShow_ARD03, //	45 ���ܱ���� ARD3
	&AlmShow_ARD02, //	46 ���ܱ�ͣ�� ARD2
	&AlmShow_ARD01, //	47 ���ܱ�ͨѶʧ�� ARD1
	&AlmShow_ARD03, //	48 ��澯 ARD3
	&AlmShow_ARD07, //	49 ��������ֶ����� ARD7
	&AlmShow_ARD01, //	50 ʱ�ӵ�ص�ѹ���� ARD1
	&AlmShow_ARD02, //	51 �ն˵��� ARD2
	&AlmShow_ARD02, //	52 �ն��ϵ� ARD2
	&AlmShow_ARD02, //	53 ���ܱ���ʱ����� ARD2
	&AlmShow_ARD01, //	54 ���ܱ�ʱ�λ���ʸ��� ARD1
	&AlmShow_ARD06, //	55 ���ܱ����峣������ ARD6
	&AlmShow_ARD05, //	56 ���ܱ�Ļ��������ʸ��� ARD5
	&AlmShow_ARD09, //	57 ң�ű�λ ARD9
	&AlmShow_ARD10, //	58 ��ͨ������Խ�� ARD10
	&AlmShow_ARD12, //	59 �̵�����λ ARD12
	&AlmShow_ARD12, //	60 ���ܱ�����բʧ�� ARD12
	&AlmShow_ARD21, //	61 ����ʧ�� ARD21
	&AlmShow_ARD13, //	62 ���ܱ�ʱ���쳣 ARD13
	&AlmShow_ARD01, //	63 ���ܱ�Уʱʧ�� ARD1
	&AlmShow_ARD15, //	64 ���ܱ�A��B��C ��ʧѹ�ܴ��� ARD15
	&AlmShow_ARD16, //	65 ���ܱ�A��B��C ��ʧ���ܴ��� ARD16
	&AlmShow_ARD17, //	66 ���ܱ�A��B��C �ೱ�������ܴ���ARD17
	&AlmShow_ARD18, //	67 ���ܱ����ܴ��� ARD18
	&AlmShow_ARD14, //	68 A ���ѹƫ��Խ�� ARD14
	&AlmShow_ARD14, //	69 B ���ѹƫ��Խ�� ARD14
	&AlmShow_ARD14, //	70 C ���ѹƫ��Խ�� ARD14
	&AlmShow_ARD14, //	71 Ƶ��ƫ��Խ�� ARD14
	&AlmShow_ARD14, //	72 A ������Խ�� ARD14
	&AlmShow_ARD14, //	73 B ������Խ�� ARD14
	&AlmShow_ARD14, //	74 C ������Խ�� ARD14
	&AlmShow_ARD14, //	75 ��ѹ��ƽ��Խ�� ARD14
	&AlmShow_ARD22, //	76 ����δ֪��� ARD22
	&AlmShow_ARD11, //	77 ���ť�п����澯 ARD11
	&AlmShow_ARD11 //	78 ��ǿ����澯 ARD11
};

C_AlmShow *g_EvtShowObj[] =
{
	//&AlmShow_ERD01, // 1"�����־",
	//&AlmShow_ERD02, // 2"��������������Զ�����",
	//&AlmShow_ERD02, // 3"��������ֶ�����",
	//&AlmShow_ERD03, // 4"A��ʧѹ��¼",
	//&AlmShow_ERD03, // 5"B��ʧѹ��¼",
	//&AlmShow_ERD03, // 6"C��ʧѹ��¼",
	//&AlmShow_ERD03, // 7"A��ʧ����¼",
	//&AlmShow_ERD03, // 8"B��ʧ����¼",
	//&AlmShow_ERD03, // 9"C����ѹ��¼",
	//&AlmShow_ERD04, //10"�ն�ͣ���¼",
	//&AlmShow_ERD05, //11"A���ѹƫ���¼",
	//&AlmShow_ERD05, //12"B���ѹƫ���¼",
	//&AlmShow_ERD05, //13"C���ѹƫ���¼",
	//&AlmShow_ERD05, //14"Ƶ��ƫ���¼",
	//&AlmShow_ERD05, //15"��ѹ��ƽ��ȼ�¼",
	//&AlmShow_ERD05, //16"A�������¼",
	//&AlmShow_ERD05, //17"B�������¼",
	//&AlmShow_ERD05, //18"C�������¼",
	//&AlmShow_ERD05, //19"A���ѹ�����¼",
	//&AlmShow_ERD05, //20"B���ѹ�����¼",
	//&AlmShow_ERD05  //21"C���ѹ�����¼",

	&AlmShow_ERD01, //01 ��̼�¼ ERD1        
	&AlmShow_ERD02, //02 ��������������Զ���ERD2
	&AlmShow_ERD02, //03 ��������ֶ����� ERD2
	&AlmShow_ERD03, //04 A ��ʧѹ��¼ ERD3    
	&AlmShow_ERD03, //05 B ��ʧѹ��¼ ERD3    
	&AlmShow_ERD03, //06 C ��ʧѹ��¼ ERD3    
	&AlmShow_ERD03, //07 A ��ʧ����¼ ERD3    
	&AlmShow_ERD03, //08 B ��ʧ����¼ ERD3    
	&AlmShow_ERD03, //09 C ��ʧ����¼ ERD3    
	&AlmShow_ERD04, //10 �ն�ͣ���¼ ERD4    
	&AlmShow_ERD03, //11 A ������¼ ERD3    
	&AlmShow_ERD03, //12 B ������¼ ERD3    
	&AlmShow_ERD03, //13 C ������¼ ERD3    
	&AlmShow_ERD06, //14 �����¼���¼ ERD6    
	&AlmShow_ERD03, //15 ���������¼ ERD3    
	&AlmShow_ERD09, //16 Уʱ��¼ ERD9        
	&AlmShow_ERD03, //17 ���ť�п�����¼ ERD3
	&AlmShow_ERD03, //18 ��ǿ�����¼ ERD3    
	&AlmShow_ERD07, //19 ��Ϣ��֤�����¼ ERD7
	&AlmShow_ERD08, //20 ���������¼ ERD8    
	&AlmShow_ERD03, //21 �������俪�ռ�¼ ERD3

};
C_AlmShow *C_AlmShowFact::GetAlmShowObj(S_DWORD DI)
{
	static C_AlmShow s_DefaultAlmShow;
	C_AlmShow *pAlmObj = NULL;
	S_AlmType AlmType = C_AlmEvt::GetAlarmNormalType(DI);
	if (AlmType == ALM_TYPE_ALARM)
		pAlmObj = g_AlmShowObj[DI - DI_ALM_ALARM_MIN];
	else if (AlmType == ALM_TYPE_EVENT)
		pAlmObj = g_EvtShowObj[DI - DI_ALM_EVENT_MIN];
	if (pAlmObj != NULL)
		return pAlmObj;
	return &s_DefaultAlmShow;
}


