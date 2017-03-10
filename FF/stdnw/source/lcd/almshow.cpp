/** @file
 *  @brief 告警中文化
 *  @author 
 *  @date 2012/03/30
 *  @version 0.1
 *  告警代码、中文显示事件内容、发生时间
 */
#include "almshow.h"
#include "datafile.h"
#include "sysbase.h"

const char *g_AlarmTitle[] =
{
	"计量装置门开闭",
	"电流反极性",
	"电压逆相序",
	"电流不平衡",
	"电压不平衡",
	"零序电流偏大",
	"A 相CT 二次侧短路",
	"B 相CT 二次侧短路",
	"C 相CT 二次侧短路",
	"A 相CT 二次侧开路",
	"B 相CT 二次侧开路",
	"C 相CT 二次侧开路",
	"A 相潮流反向",
	"B 相潮流反向",
	"C 相潮流反向",
	"A 相电流过流",
	"B 相电流过流",
	"C 相电流过流",
	"A 相电流失流",
	"B 相电流失流",
	"C 相电流失流",
	"A 相电压失压",
	"B 相电压失压",
	"C 相电压失压",
	"全失压",
	"A 相电压过压",
	"B 相电压过压",
	"C 相电压过压",
	"A 相电压断相",
	"B 相电压断相",
	"C 相电压断相",
	"A 相电压畸变",
	"B 相电压畸变",
	"C 相电压畸变",
	"A 相电流畸变",
	"B 相电流畸变",
	"C 相电流畸变",
	"无功过补偿",
	"无功欠补偿",
	"功率超定值",
	"负荷过载",
	"超合同容量用电",
	"剩余电费不足",
	"示度下降",
	"电能表飞走",
	"电能表停走",
	"电能表通讯失败",
	"差动告警",
	"最大需量手动复零",
	"时钟电池电压过低",
	"终端掉电",
	"终端上电",
	"电能表编程时间更改",
	"电能表时段或费率更改",
	"电能表脉冲常数更改",
	"电能表互感器倍率更改",
	"遥信变位",
	"月通信流量越限",
	"继电器变位",
	"电能表拉合闸失败",
	"抄表失败",
	"电能表时钟异常",
	"电能表校时失败",
	"电能表ABC 相失压总数",
	"电能表ABC 相失流总数",
	"电能表ABC 相潮流反向",
	"电能表编程总次数",
	"A 相电压偏差越限",
	"B 相电压偏差越限",
	"C 相电压偏差越限",
	"频率偏差越限",
	"A 相闪变越限",
	"B 相闪变越限",
	"C 相闪变越限",
	"电压不平衡越限",
	"发现未知电表",
	"表端钮盒开启告警",
	"表盖开启告警"
};
const char *g_EventTitle[] =
{
	"编程记录",
	"最大需量自动复零",
	"最大需量手动复零",
	"A 相失压记录",
	"B 相失压记录",
	"C 相失压记录",
	"A 相失流记录",
	"B 相失流记录",
	"C 相失流记录",
	"终端停电记录",
	"A 相断相记录",
	"B 相断相记录",
	"C 相断相记录",
	"控制事件记录",
	"电量清零记录",
	"校时记录",
	"表端钮盒开启记录",
	"表盖开启记录",
	"消息认证错误记录",
	"参数变更记录",
	"计量门箱开闭记录"
};

int C_AlmShow::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{
	char bufferMP[64], bufferDI[64];
	strings strTitles;

	S_AlmType AlmType = C_AlmEvt::GetAlarmNormalType(DI);
	sprintf(bufferMP, "测量点: %d", MP);
	if (AlmType == ALM_TYPE_ALARM)
	{
		sprintf(bufferDI, "告警编码: %08X", DI);
		strTitles.push_back(g_AlarmTitle[DI - DI_ALM_ALARM_MIN]);
		strTitles.push_back(bufferMP);
		strTitles.push_back(bufferDI);
		AlarmGeneralPrint(strRecord, strTitles);
	}
	else if (AlmType == ALM_TYPE_EVENT)
	{
		sprintf(bufferDI, "事件编码: %08X", DI);
		strTitles.push_back(g_EventTitle[DI - DI_ALM_EVENT_MIN]);
		strTitles.push_back(bufferMP);
		strTitles.push_back(bufferDI);
	}
	else
	{
		strTitles.push_back("没有告警数据");
	}
	strTexts.append(strTitles);
	return (int)strTexts.size();
}

static char *g_AlarmStatus[] = {"恢复", "发生", "    "};
int C_AlmShow::AlarmGeneralPrint(const string &strRecord, strings &strTexts)
{
	if (strRecord.size() < 7)
		return 0;
	char buffer[64];
	S_BYTE status = (S_BYTE)strRecord[0];
	if (status > 2) status = 2;
		
	sprintf(buffer, "告警状态: %s", g_AlarmStatus[status]);
	strTexts.push_back(buffer);
	strTexts.push_back("时间: ");
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
		sprintf(buffer, "%C相电压:",'A'+i);
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
		sprintf(buffer, "%C相电流:",'A'+i);
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
			sprintf(buffer, "%C相%s功功率:", 'A'+i-1,PQ?"有":"无");
		else
			sprintf(buffer, "总%s功功率:", PQ?"有":"无");
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
			sprintf(buffer, "%C相功率因数:",'A'+i-1);
		else
			sprintf(buffer, "总功率因数:");
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
	strTexts.push_back("正向有功总: ");
	sprintf(buffer, " %s kWh",strAscii.c_str());
	strTexts.push_back(buffer);

	strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(4,4).c_str(),6,2,false);
	strTexts.push_back("反向有功总: ");
	sprintf(buffer, " %s kWh",strAscii.c_str());
	strTexts.push_back(buffer);

	strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(8,4).c_str(),6,2,false);
	strTexts.push_back("组合无功1: ");
	sprintf(buffer, " %s kWh",strAscii.c_str());
	strTexts.push_back(buffer);

	strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(12,4).c_str(),6,2,false);
	strTexts.push_back("组合无功2: ");
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
			strTexts.push_back("PT变比: ");
		else 
			strTexts.push_back("CT变比: ");

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
		sprintf(buffer, "电表%s常数:",(i==0)?"有功":"无功");
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
		strTexts.push_back("剩余金额: ");
		sprintf(buffer, " %s 元",strAscii.c_str());
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

	strTexts.push_back("正向有功总: ");
	strTexts.push_back("最大需量: ");
	string strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(23,3).c_str(),2,4,false);
	sprintf(buffer, " %s kW",strAscii.c_str());
	strTexts.push_back(buffer);
	index=26;
	strTexts.push_back("发生时间: ");
	sprintf(buffer, " %02X-%02X-%02X %02X:%02X", strRecord[index+4], strRecord[index+3], strRecord[2+index], strRecord[1+index], strRecord[0+index]);
	strTexts.push_back(buffer);
		
	strTexts.push_back("反向有功总: ");
	strTexts.push_back("最大需量: ");
	strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(31,3).c_str(),2,4,false);
	sprintf(buffer, " %s kW",strAscii.c_str());
	strTexts.push_back(buffer);
	index=34;
	strTexts.push_back("发生时间: ");
	sprintf(buffer, " %02X-%02X-%02X %02X:%02X", strRecord[4+index], strRecord[5+index], strRecord[2+index], strRecord[1+index], strRecord[0+index]);
	strTexts.push_back(buffer);
	
	strTexts.push_back("组合无功1总: ");
	strTexts.push_back("最大需量: ");
	strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(39,3).c_str(),2,4,false);
	sprintf(buffer, " %s kW",strAscii.c_str());
	strTexts.push_back(buffer);
	index=42;	
	strTexts.push_back("发生时间: ");
	sprintf(buffer, " %02X-%02X-%02X %02X:%02X", strRecord[4+index], strRecord[3+index], strRecord[2+index], strRecord[1+index], strRecord[0+index]);
	strTexts.push_back(buffer);

	
	strTexts.push_back("组合无功2总: ");
	strTexts.push_back("最大需量: ");
	strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(47,3).c_str(),2,4,false);
	sprintf(buffer, " %s kW",strAscii.c_str());
	strTexts.push_back(buffer);
	index=50;	
	strTexts.push_back("发生时间: ");
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
		strTexts.push_back("月通信流量: ");
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
		sprintf(buffer, "发生次数:%s", strAscii.c_str());
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
	strTexts.push_back("正向有功总: ");
	sprintf(buffer, " %s kWh",strAscii.c_str());
	strTexts.push_back(buffer);
	index=3;
	strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(4,4).c_str(),6,2,false);
	strTexts.push_back("反向有功总: ");
	sprintf(buffer, " %s kWh",strAscii.c_str());
	strTexts.push_back(buffer);

	index=7;
	for(int i=0;i<4;i++)
	{
		strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(8+i*4,4).c_str(),6,2,false);
		sprintf(buffer, "第%d象限无功总电能:",(i+1));
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
		const char *pState = "合闸";
		if (*iterBegin & 0x10)
			pState = "拉闸";
		sprintf(buffer, "状态: %s", pState);
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
		strTexts.push_back("电表日期: ");
		sprintf(buffer, " %02X-%02X-%02X %02X:%02X:%02X", *(iterBegin+3), *(iterBegin+2), *(iterBegin+1), *(iterBegin+6), *(iterBegin+5), *(iterBegin+4));
		strTexts.push_back(buffer);
		advance(iterBegin, offset);
	}

	offset = 6;
	if (distance(iterBegin, iterEnd) >= offset)
	{
		strTexts.push_back("终端时间: ");
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
		sprintf(buffer, "%C相电%S总谐波含有量: ",'A'+i%3,(i<=2)?"压":"流");
		strTexts.push_back(buffer);
		sprintf(buffer, " %s %%",strAscii.c_str());
		strTexts.push_back(buffer);
	}

	index=18;
	for(int i=0;i<3;i++)
	{
		string strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(19+i*2,2).c_str(),4,0,false);
		sprintf(buffer, "%C相电%S偏差最大值: ", 'A'+i,"压");
		strTexts.push_back(buffer);
		sprintf(buffer, " %s %%",strAscii.c_str());
		strTexts.push_back(buffer);
	}
	
	index=24;
	strTexts.push_back("频率偏差最大值: ");
	string strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(++index,1).c_str(),2,0,false);
	sprintf(buffer, " %s %%", strAscii.c_str());
	strTexts.push_back(buffer);

	index=25;
	strTexts.push_back("电压不平衡度最大值: ");
	strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(++index,2).c_str(),4,0,false);
	sprintf(buffer, " %s %%",strAscii.c_str());
	strTexts.push_back(buffer);
	
	index=27;
	strTexts.push_back("长时闪变最大值: ");
	strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(++index,1).c_str(),2,0,false);
	sprintf(buffer, " %s %%", strAscii.c_str());
	strTexts.push_back(buffer);
	
	index=28;
	strTexts.push_back("短时闪变最大值: ");
	strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(++index,1).c_str(),2,0,false);
	sprintf(buffer, " %s %%", strAscii.c_str());
	strTexts.push_back(buffer);

	index=29;
	strTexts.push_back("电压总畸变最大值: ");
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
		sprintf(buffer, "%C相失压总次数:",'A'+i);
		strTexts.push_back(buffer);
		string strAscii=GetDwordData(strRecord.substr(7+i*6,3));
		sprintf(buffer, " %s 次",strAscii.c_str());
		strTexts.push_back(buffer);

		sprintf(buffer, "%C相失压总时间:", 'A'+i);
		strTexts.push_back(buffer);
		strAscii=GetDwordData(strRecord.substr(7+i*6+3,3));
		sprintf(buffer, " %s 分",strAscii.c_str());
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
		sprintf(buffer, "%C相失流总次数:",'A'+i);
		strTexts.push_back(buffer);
		string strAscii=GetDwordData(strRecord.substr(7+i*6,3));
		sprintf(buffer, " %s 次",strAscii.c_str());
		strTexts.push_back(buffer);

		sprintf(buffer, "%C相失流总时间:",'A'+i);
		strTexts.push_back(buffer);
		strAscii=GetDwordData(strRecord.substr(7+i*6+3,3));
		sprintf(buffer, " %s 分",strAscii.c_str());
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
		sprintf(buffer, "%C相潮流反向总次数:", 'A'+i);
		strTexts.push_back(buffer);
		string strAscii=GetDwordData(strRecord.substr(7+i*6,3));
		sprintf(buffer, " %s 次",strAscii.c_str());
		strTexts.push_back(buffer);

		sprintf(buffer, "%C相潮流反向总时间:",'A'+i);
		strTexts.push_back(buffer);
		strAscii=GetDwordData(strRecord.substr(7+i*6+3,3));
		sprintf(buffer, " %s 分",strAscii.c_str());
		strTexts.push_back(buffer);
	}
	return (int)strTexts.size();
}
int C_AlmShow_ARD18::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	C_AlmShow::Printf(MP, DI, strRecord, strTexts);
	S_CHAR buffer[64];	

	string strAscii=GetDwordData(strRecord.substr(7,3));
	strTexts.push_back("编程总次数: ");
	sprintf(buffer, " %s 次", strAscii.c_str());
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
		strTexts.push_back("暂降");
	else if(strRecord[index]==0x02)
		strTexts.push_back("暂升");
	else if(strRecord[index]==0x03)
		strTexts.push_back("短时中断");


	index=7;
	strTexts.push_back("持续时间: ");
	string strAscii=GetDwordData(strRecord.substr(++index,2));
	sprintf(buffer, " %s 毫秒", strAscii.c_str());
	strTexts.push_back(buffer);

	index=9;
	strTexts.push_back("电池极值: ");
	strAscii=C_BusiAlgo::ChangeBcd2Ascii((S_BYTE*)strRecord.substr(++index,2).c_str(),3,1,false);	
	sprintf(buffer, " %s 毫秒", strAscii.c_str());
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
			sprintf(buffer, " %d 号表抄收失败",i);
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
	sprintf(buffer,"共发现电表 %d 块",count);
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
		strTexts.push_back("电表地址: ");
		sprintf(buffer, " %02X%02X%02X%02X%02X%02X",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5]);
		strTexts.push_back(buffer);
		
		S_BYTE btemp=strRecord[++index];
		if(btemp&0x80)
			strTexts.push_back("相序:A相");
		else if(btemp&0x40)
			strTexts.push_back("相序:B相");
		else if(btemp&0x20)
			strTexts.push_back("相序:C相");

		count=(S_LONG)(btemp&0x0f);
		sprintf(buffer,"载波信号强度: %d",count);
		strTexts.push_back(buffer);

		btemp=(strRecord[++index]&0x03);
		strTexts.push_back("电表规约: ");
		if(btemp==0x00)
			strTexts.push_back(" "NW_MRULE_NAME_DLT645_1997);
		else if(btemp==0x01)			
			strTexts.push_back(" "NW_MRULE_NAME_DLT645_2007);
		else
			strTexts.push_back(" 其他");		
	}
	return (int)strTexts.size();
}


int C_AlmShow_ERD01::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	if (strRecord.size() < 6)
		return 0;
	char buffer[64];

	strTexts.push_back("编程时间: ");		
	sprintf(buffer, " %02X-%02X-%02X %02X:%02X:%02X", strRecord[0], strRecord[1], strRecord[2], strRecord[3], strRecord[4], strRecord[5]);
	strTexts.push_back(buffer);	
	return (int)strTexts.size();
}
int C_AlmShow_ERD02::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	if (strRecord.size() < 6)
		return 0;
	char buffer[64];

	strTexts.push_back("最大需量清零时间: ");		
	sprintf(buffer, " %02X-%02X-%02X %02X:%02X:%02X", strRecord[0], strRecord[1], strRecord[2], strRecord[3], strRecord[4], strRecord[5]);
	strTexts.push_back(buffer);	
	return (int)strTexts.size();
}
int C_AlmShow_ERD03::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	if (strRecord.size() < 12)
		return 0;
	char buffer[64];

	strTexts.push_back("开始时间: ");		
	sprintf(buffer, " %02X-%02X-%02X %02X:%02X:%02X", strRecord[0], strRecord[1], strRecord[2], strRecord[3], strRecord[4], strRecord[5]);
	strTexts.push_back(buffer);	
	strTexts.push_back("结束时间: ");		
	sprintf(buffer, " %02X-%02X-%02X %02X:%02X:%02X", strRecord[6], strRecord[7], strRecord[8], strRecord[9], strRecord[10], strRecord[11]);
	strTexts.push_back(buffer);	

	return (int)strTexts.size();
}
int C_AlmShow_ERD04::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	if (strRecord.size() < 12)
		return 0;
	char buffer[64];

	strTexts.push_back("停电时刻: ");		
	sprintf(buffer, " %02X-%02X-%02X %02X:%02X:%02X", strRecord[0], strRecord[1], strRecord[2], strRecord[3], strRecord[4], strRecord[5]);
	strTexts.push_back(buffer);	
	strTexts.push_back("复电时刻: ");		
	sprintf(buffer, " %02X-%02X-%02X %02X:%02X:%02X", strRecord[6], strRecord[7], strRecord[8], strRecord[9], strRecord[10], strRecord[11]);
	strTexts.push_back(buffer);	
	return (int)strTexts.size();
}
int C_AlmShow_ERD05::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	if (strRecord.size() < 12)
		return 0;
	char buffer[64];

	strTexts.push_back("开始时间: ");		
	sprintf(buffer, " %02X-%02X-%02X %02X:%02X:%02X", strRecord[0], strRecord[1], strRecord[2], strRecord[3], strRecord[4], strRecord[5]);
	strTexts.push_back(buffer);	
	strTexts.push_back("结束时间: ");		
	sprintf(buffer, " %02X-%02X-%02X %02X:%02X:%02X", strRecord[6], strRecord[7], strRecord[8], strRecord[9], strRecord[10], strRecord[11]);
	strTexts.push_back(buffer);	
	return (int)strTexts.size();
}
int C_AlmShow_ERD06::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	if (strRecord.size() < 7)
		return 0;
	char buffer[64];

	strTexts.push_back("控制事件发生时间: ");		
	sprintf(buffer, " %02X-%02X-%02X %02X:%02X:%02X", strRecord[0], strRecord[1], strRecord[2], strRecord[3], strRecord[4], strRecord[5]);
	strTexts.push_back(buffer);	
	if((S_BYTE)strRecord[6]==(S_BYTE)0x01)
	{
		sprintf(buffer, "控制类型:%s", "拉闸");
	}
	else if((S_BYTE)strRecord[0]==(S_BYTE)0x01)
	{
		sprintf(buffer, "控制类型:%s", "合闸");
	}
	else if((S_BYTE)strRecord[0]==(S_BYTE)0x01)
	{
		sprintf(buffer, "控制类型:%s", "保电");
	}
	else if((S_BYTE)strRecord[0]==(S_BYTE)0x01)
	{
		sprintf(buffer, "控制类型:%s", "保电解除");
	}
	
	strTexts.push_back(buffer);	
	return (int)strTexts.size();
}
int C_AlmShow_ERD07::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	if (strRecord.size() < 6)
		return 0;
	char buffer[64];

	strTexts.push_back("消息认证错误发生时间: ");
	sprintf(buffer, " %02X-%02X-%02X %02X:%02X:%02X", strRecord[0], strRecord[1], strRecord[2], strRecord[3], strRecord[4], strRecord[5]);
	strTexts.push_back(buffer);	
	return (int)strTexts.size();
}
int C_AlmShow_ERD08::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	if (strRecord.size() < 6)
		return 0;
	char buffer[64];

	strTexts.push_back("参数变更发生时间: ");
	sprintf(buffer, " %02X-%02X-%02X %02X:%02X:%02X", strRecord[0], strRecord[1], strRecord[2], strRecord[3], strRecord[4], strRecord[5]);
	strTexts.push_back(buffer);	
	return (int)strTexts.size();
}
int C_AlmShow_ERD09::Printf(S_WORD MP, S_DWORD DI, const string &strRecord, strings &strTexts)
{//TODO:
	if (strRecord.size() < 6)
		return 0;
	char buffer[64];

	strTexts.push_back("校时发生时间: ");
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
//	&AlmShow_ARD02, // 1"计量装置门打开",
//	&AlmShow_ARD01, // 2"窃电",
//	&AlmShow_ARD02, // 3"电流回路逆相序",
//	&AlmShow_ARD02, // 4"电压回路逆相序",
//	&AlmShow_ARD02, // 5"电流不平衡",
//	&AlmShow_ARD02, // 6"零序电流偏大",
//	&AlmShow_ARD02, // 7"A相CT二次侧短路",
//	&AlmShow_ARD02, // 8"B相CT二次侧短路",
//	&AlmShow_ARD02, // 9"C相CT二次侧短路",
//	&AlmShow_ARD02, //10"A相CT二次侧开路",
//	&AlmShow_ARD02, //11"B相CT二次侧开路",
//	&AlmShow_ARD02, //12"C相CT二次侧开路",
//	&AlmShow_ARD02, //13"A相潮流反向",
//	&AlmShow_ARD02, //14"B相潮流反向",
//	&AlmShow_ARD02, //15"C相潮流反向",
//	&AlmShow_ARD02, //16"A相电流过流",
//	&AlmShow_ARD02, //17"B相电流过流",
//	&AlmShow_ARD02, //18"C相电流过流",
//	&AlmShow_ARD02, //19"A相电流失流",
//	&AlmShow_ARD02, //20"B相电流失流",
//	&AlmShow_ARD02, //21"C相电流失流",
//	&AlmShow_ARD02, //22"A相电压失压",
//	&AlmShow_ARD02, //23"B相电压失压",
//	&AlmShow_ARD02, //24"C相电压失压",
//	&AlmShow_ARD02, //25"全失压",
//	&AlmShow_ARD02, //26"A相电压过压",
//	&AlmShow_ARD02, //27"B相电压过压",
//	&AlmShow_ARD02, //28"C相电压过压",
//	&AlmShow_ARD02, //29"A相电压断相",
//	&AlmShow_ARD02, //30"B相电压断相",
//	&AlmShow_ARD02, //31"C相电压断相",
//	&AlmShow_ARD14, //32"A相电压畸变率超标",
//	&AlmShow_ARD14, //33"B相电压畸变率超标",
//	&AlmShow_ARD14, //34"C相电压畸变率超标",
//	&AlmShow_ARD14, //35"A相电流畸变率超标",
//	&AlmShow_ARD14, //36"B相电流畸变率超标",
//	&AlmShow_ARD14, //37"C相电流畸变率超标",
//	&AlmShow_ARD02, //38"无功过补偿",
//	&AlmShow_ARD02, //39"无功欠补偿",
//	&AlmShow_ARD02, //40"功率超定值",
//	&AlmShow_ARD02, //41"负荷过载",
//	&AlmShow_ARD02, //42"超合同容量用电",
//	&AlmShow_ARD04, //43"剩余购电量不足",
//	&AlmShow_ARD03, //44"示度下降",
//	&AlmShow_ARD03, //45"电能表飞走",
//	&AlmShow_ARD02, //46"电能表停走",
//	&AlmShow_ARD01, //47"电能表通信失败",
//	&AlmShow_ARD03, //48"差动报警",
//	&AlmShow_ARD07, //49"最大需量结算日自动复零",
//	&AlmShow_ARD07, //50"最大需量手动复零",
//	&AlmShow_ARD01, //51"时钟电池电压过低",
//	&AlmShow_ARD01, //52"充电电池电压过低",
//	&AlmShow_ARD08, //53"电能表状态字变化",
//	&AlmShow_ARD02, //54"终端停电",
//	&AlmShow_ARD02, //55"电能表编程时间更改",
//	&AlmShow_ARD01, //56"电能表时段或费率更改",
//	&AlmShow_ARD06, //57"电能表脉冲常数更改",
//	&AlmShow_ARD05, //58"电能表的互感器倍率更改",
//	&AlmShow_ARD09, //59"遥信变位",
//	&AlmShow_ARD10, //60"月通信流量越限",
//	&AlmShow_ARD11, //61"日线损率越限",
//	&AlmShow_ARD12, //62"继电器变位",
//	&AlmShow_ARD12, //63"电能表拉合闸失败",
//	&AlmShow_ARD01, //64"抄表失败",
//	&AlmShow_ARD13, //65"电能表时钟超差",
//	&AlmShow_ARD01, //66"电能表校时失败",
//	&AlmShow_ARD02, //67"第一轮跳闸",
//	&AlmShow_ARD02, //68"第二轮跳闸",
//	&AlmShow_ARD02, //69"第三轮跳闸",
//	&AlmShow_ARD02, //70"第四轮跳闸",
//	&AlmShow_ARD02, //71"第五轮跳闸",
//	&AlmShow_ARD02, //72"第六轮跳闸",
//	&AlmShow_ARD02, //73"第七轮跳闸",
//	&AlmShow_ARD02, //74"第八轮跳闸",
//	&AlmShow_ARD15, //75"电能表A、B、C相失压总次数",
//	&AlmShow_ARD16, //76"电能表A、B、C相失流总次数",
//	&AlmShow_ARD17, //77"电能表A、B、C相潮流反向总次数",
//	&AlmShow_ARD18, //78"电能表编程总次数",
//	&AlmShow_ARD19, //79"电能表停电",
//	&AlmShow_ARD20, //80"A相电压暂态事件",
//	&AlmShow_ARD20, //81"B相电压暂态事件",
//	&AlmShow_ARD20, //82"C相电压暂态事件",
//	&AlmShow_ARD14, //83"A相电压偏差越限",
//	&AlmShow_ARD14, //84"B相电压偏差越限",
//	&AlmShow_ARD14, //85"C相电压偏差越限",
//	&AlmShow_ARD14, //86"频率偏差越限",
//	&AlmShow_ARD14, //87"A相闪变越限",
//	&AlmShow_ARD14, //88"B相闪变越限",
//	&AlmShow_ARD14, //89"C相闪变越限",
//	&AlmShow_ARD14  //90"电压不平衡越限",
//};

C_AlmShow *g_AlmShowObj[] =
{
	&AlmShow_ARD02, //  01 计量装置门开闭 ARD2
	&AlmShow_ARD02, //  02 电流反极性 ARD2
	&AlmShow_ARD02, //	03 电压逆相序 ARD2
	&AlmShow_ARD02, //	04 电流不平衡 ARD2
	&AlmShow_ARD02, //	05 电压不平衡 ARD2
	&AlmShow_ARD02, //	06 零序电流偏大 ARD2
	&AlmShow_ARD02, //	07 A 相CT 二次侧短路 ARD2
	&AlmShow_ARD02, //	08 B 相CT 二次侧短路 ARD2
	&AlmShow_ARD02, //	09 C 相CT 二次侧短路 ARD2
	&AlmShow_ARD02, //	10 A 相CT 二次侧开路 ARD2
	&AlmShow_ARD02, //	11 B 相CT 二次侧开路 ARD2
	&AlmShow_ARD02, //	12 C 相CT 二次侧开路 ARD2
	&AlmShow_ARD02, //	13 A 相潮流反向 ARD2
	&AlmShow_ARD02, //	14 B 相潮流反向 ARD2
	&AlmShow_ARD02, //	15 C 相潮流反向 ARD2
	&AlmShow_ARD02, //	16 A 相电流过流 ARD2
	&AlmShow_ARD02, //	17 B 相电流过流 ARD2
	&AlmShow_ARD02, //	18 C 相电流过流 ARD2
	&AlmShow_ARD02, //	19 A 相电流失流 ARD2
	&AlmShow_ARD02, //	20 B 相电流失流 ARD2
	&AlmShow_ARD02, //	21 C 相电流失流 ARD2
	&AlmShow_ARD02, //	22 A 相电压失压 ARD2
	&AlmShow_ARD02, //	23 B 相电压失压 ARD2
	&AlmShow_ARD02, //	24 C 相电压失压 ARD2
	&AlmShow_ARD02, //	25 全失压 ARD2
	&AlmShow_ARD02, //	26 A 相电压过压 ARD2
	&AlmShow_ARD02, //	27 B 相电压过压 ARD2
	&AlmShow_ARD02, //	28 C 相电压过压 ARD2
	&AlmShow_ARD02, //	29 A 相电压断相 ARD2
	&AlmShow_ARD02, //	30 B 相电压断相 ARD2
	&AlmShow_ARD02, //	31 C 相电压断相 ARD2
	&AlmShow_ARD14, //	32 A 相电压畸变 ARD14
	&AlmShow_ARD14, //	33 B 相电压畸变 ARD14
	&AlmShow_ARD14, //	34 C 相电压畸变 ARD14
	&AlmShow_ARD14, //	35 A 相电流畸变 ARD14
	&AlmShow_ARD14, //	36 B 相电流畸变 ARD14
	&AlmShow_ARD14, //	37 C 相电流畸变 ARD14
	&AlmShow_ARD02, //	38 无功过补偿 ARD2
	&AlmShow_ARD02, //	39 无功欠补偿 ARD2
	&AlmShow_ARD02, //	40 功率超定值 ARD2
	&AlmShow_ARD02, //	41 负荷过载 ARD2
	&AlmShow_ARD02, //	42 超合同容量用电 ARD2
	&AlmShow_ARD04, //	43 剩余电费不足 ARD4
	&AlmShow_ARD03, //	44 示度下降 ARD3
	&AlmShow_ARD03, //	45 电能表飞走 ARD3
	&AlmShow_ARD02, //	46 电能表停走 ARD2
	&AlmShow_ARD01, //	47 电能表通讯失败 ARD1
	&AlmShow_ARD03, //	48 差动告警 ARD3
	&AlmShow_ARD07, //	49 最大需量手动复零 ARD7
	&AlmShow_ARD01, //	50 时钟电池电压过低 ARD1
	&AlmShow_ARD02, //	51 终端掉电 ARD2
	&AlmShow_ARD02, //	52 终端上电 ARD2
	&AlmShow_ARD02, //	53 电能表编程时间更改 ARD2
	&AlmShow_ARD01, //	54 电能表时段或费率更改 ARD1
	&AlmShow_ARD06, //	55 电能表脉冲常数更改 ARD6
	&AlmShow_ARD05, //	56 电能表的互感器倍率更改 ARD5
	&AlmShow_ARD09, //	57 遥信变位 ARD9
	&AlmShow_ARD10, //	58 月通信流量越限 ARD10
	&AlmShow_ARD12, //	59 继电器变位 ARD12
	&AlmShow_ARD12, //	60 电能表拉合闸失败 ARD12
	&AlmShow_ARD21, //	61 抄表失败 ARD21
	&AlmShow_ARD13, //	62 电能表时钟异常 ARD13
	&AlmShow_ARD01, //	63 电能表校时失败 ARD1
	&AlmShow_ARD15, //	64 电能表A、B、C 相失压总次数 ARD15
	&AlmShow_ARD16, //	65 电能表A、B、C 相失流总次数 ARD16
	&AlmShow_ARD17, //	66 电能表A、B、C 相潮流反向总次数ARD17
	&AlmShow_ARD18, //	67 电能表编程总次数 ARD18
	&AlmShow_ARD14, //	68 A 相电压偏差越限 ARD14
	&AlmShow_ARD14, //	69 B 相电压偏差越限 ARD14
	&AlmShow_ARD14, //	70 C 相电压偏差越限 ARD14
	&AlmShow_ARD14, //	71 频率偏差越限 ARD14
	&AlmShow_ARD14, //	72 A 相闪变越限 ARD14
	&AlmShow_ARD14, //	73 B 相闪变越限 ARD14
	&AlmShow_ARD14, //	74 C 相闪变越限 ARD14
	&AlmShow_ARD14, //	75 电压不平衡越限 ARD14
	&AlmShow_ARD22, //	76 发现未知电表 ARD22
	&AlmShow_ARD11, //	77 表端钮盒开启告警 ARD11
	&AlmShow_ARD11 //	78 表盖开启告警 ARD11
};

C_AlmShow *g_EvtShowObj[] =
{
	//&AlmShow_ERD01, // 1"编程日志",
	//&AlmShow_ERD02, // 2"最大需量结算日自动复零",
	//&AlmShow_ERD02, // 3"最大需量手动复零",
	//&AlmShow_ERD03, // 4"A相失压记录",
	//&AlmShow_ERD03, // 5"B相失压记录",
	//&AlmShow_ERD03, // 6"C相失压记录",
	//&AlmShow_ERD03, // 7"A相失流记录",
	//&AlmShow_ERD03, // 8"B相失流记录",
	//&AlmShow_ERD03, // 9"C相流压记录",
	//&AlmShow_ERD04, //10"终端停电记录",
	//&AlmShow_ERD05, //11"A相电压偏差记录",
	//&AlmShow_ERD05, //12"B相电压偏差记录",
	//&AlmShow_ERD05, //13"C相电压偏差记录",
	//&AlmShow_ERD05, //14"频率偏差记录",
	//&AlmShow_ERD05, //15"电压不平衡度记录",
	//&AlmShow_ERD05, //16"A相闪变记录",
	//&AlmShow_ERD05, //17"B相闪变记录",
	//&AlmShow_ERD05, //18"C相闪变记录",
	//&AlmShow_ERD05, //19"A相电压畸变记录",
	//&AlmShow_ERD05, //20"B相电压畸变记录",
	//&AlmShow_ERD05  //21"C相电压畸变记录",

	&AlmShow_ERD01, //01 编程记录 ERD1        
	&AlmShow_ERD02, //02 最大需量结算日自动复ERD2
	&AlmShow_ERD02, //03 最大需量手动复零 ERD2
	&AlmShow_ERD03, //04 A 相失压记录 ERD3    
	&AlmShow_ERD03, //05 B 相失压记录 ERD3    
	&AlmShow_ERD03, //06 C 相失压记录 ERD3    
	&AlmShow_ERD03, //07 A 相失流记录 ERD3    
	&AlmShow_ERD03, //08 B 相失流记录 ERD3    
	&AlmShow_ERD03, //09 C 相失流记录 ERD3    
	&AlmShow_ERD04, //10 终端停电记录 ERD4    
	&AlmShow_ERD03, //11 A 相断相记录 ERD3    
	&AlmShow_ERD03, //12 B 相断相记录 ERD3    
	&AlmShow_ERD03, //13 C 相断相记录 ERD3    
	&AlmShow_ERD06, //14 控制事件记录 ERD6    
	&AlmShow_ERD03, //15 电量清零记录 ERD3    
	&AlmShow_ERD09, //16 校时记录 ERD9        
	&AlmShow_ERD03, //17 表端钮盒开启记录 ERD3
	&AlmShow_ERD03, //18 表盖开启记录 ERD3    
	&AlmShow_ERD07, //19 消息认证错误记录 ERD7
	&AlmShow_ERD08, //20 参数变更记录 ERD8    
	&AlmShow_ERD03, //21 计量门箱开闭记录 ERD3

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


