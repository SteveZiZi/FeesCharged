#include <stdio.h>
#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

#include "inc/xtDebug.h"
#include "datamanager.h"
#include "datacomm/IfDatacomm.h"

#define SEVEN_DAY       7
#define SIX_MONTH       6
#define BRIGHTNESS_FILE "/sys/class/backlight/backlight/brightness"

CDataManager::CDataManager()
{
    ///////  测试用 ////////////////
    m_balance = 50.25f; 
    m_energy  = 100.54f;

    m_sSystemParam.backlight = 50;
    m_sSystemParam.loopTime = 60;
    m_sSystemParam.startTime = "06:00";
    m_sSystemParam.endTime = "23:00";
}

CDataManager::~CDataManager()
{

}


CDataManager* CDataManager::getInstance()
{
    static CDataManager mgr;

    return &mgr;
}

/************************************************************************/
/* 以下为用户卡信息                                                     */
/************************************************************************/
void CDataManager::getUserInfo(DM_UserInfo &user)
{
    user.name = QObject::tr("张三");
    user.householdNum = "3759439986";
    user.houseNum = QObject::tr("丰收路123号");
}

char bcdc(char i_cBcdData)
{
   char i;
   i=((i_cBcdData>>4)&0x0f)*10+(i_cBcdData&0x0f);
   return i;
}

float  BCD62_to_Double(char *i_cpBcdData)
{
	float x=0.0,x1=0.0;

	x1=(float)bcdc(i_cpBcdData[3]);
	x=x+x1*10000;
	x1=(float)bcdc(i_cpBcdData[2]);
	x=x+x1*100;
	x1=(float)bcdc(i_cpBcdData[1]);
	x=x+x1;
	x1=(float)bcdc(i_cpBcdData[0]);
	x=x+x1/100;
	return x;
}

void CDataManager::getUserCurrentElectro(DM_UserCurrentElectro &curElec)
{
#ifndef WIN32
    curElec.balance = (float)50.25;
    char data[4] = {0};
    CIfDatacomm::getCurZxygData(1,data);
    char str[13] = {0};
    printf("zxyg = %02x%02x%02x.%02x kWh\n",data[3],data[2],data[1],data[0]);
    
    char invaliddata[4] = {0xff};
    if (memcmp(data,invaliddata,4) == 0)
    {
	memcpy(str,"----.--",7);	    	
    }
    else
    {
    	sprintf(str,"%02x%02x%02x.%02x kWh",data[3],data[2],data[1],data[0]);	
    }
    curElec.energy = BCD62_to_Double(data);	
#else
    QDateTime tm = QDateTime::currentDateTime();
    curElec.balanceTime = tm.toString("yyyy-MM-dd hh:mm");
    curElec.balance = m_balance;
    curElec.energy = m_energy;
#endif
}
/*
 *  @Func:      userCharge
 *  @Desc:      用户充值
 *  @PaamIn:    
 *      chargeInfo:充值信息
 *  @Return:
 *      true:   充值成功
 *      false:  充值失败
 */
bool CDataManager::userCharge(const DM_UserChargeInfo &chargeInfo)
{
    m_balance += chargeInfo.money;

    return true;
}


void CDataManager::getLastWeekElectro(DM_LastestWeekElectro &elec)
{
#ifndef WIN32
    char dataarry[7][4];
    if (CIfDatacomm::geInSevenDaysData(getCardMp(),dataarry))
    {
        for (int i = 0;i < SEVEN_DAY;i++)
        {
            elec.energy[i] = BCD62_to_Double(dataarry[i]);
        }
    }
    else
    {
        elec.energy[0] = 51.2;
        elec.energy[1] = 12.64;
        elec.energy[2] = 34.94;
        elec.energy[3] = 27.65;
        elec.energy[4] = 43.68;
        elec.energy[5] = 54.68;
        elec.energy[6] = 9.35;
    }
#else
    elec.energy[0] = 51.2;
    elec.energy[1] = 12.64;
    elec.energy[2] = 34.94;
    elec.energy[3] = 27.65;
    elec.energy[4] = 43.68;
    elec.energy[5] = 54.68;
    elec.energy[6] = 9.35;
#endif
}

void CDataManager::getLastMonthElectro(DM_LastestMonthElectro &elec)
{
    elec.energy[0] = 51.2*7;
    elec.energy[1] = 12.64*7;
    elec.energy[2] = 34.94*7;
    elec.energy[3] = 27.65*7;
}

void CDataManager::getLastHalfYearElectro(DM_LastestHalfYearElectro &elec)
{
#ifndef WIN32
    char dataarry[6][4];
    if (CIfDatacomm::getInSixMonthsData(getCardMp(),dataarry))
    {
        for(int i = 0;i < SIX_MONTH;i++)
        {
            elec.energy[i] = BCD62_to_Double(dataarry[i]);
        }
    }
    else
    {
        elec.energy[0] = 51.2*30;
        elec.energy[1] = 12.64*31;
        elec.energy[2] = 34.94*30;
        elec.energy[3] = 27.65*31;
        elec.energy[4] = 43.68*31;
        elec.energy[5] = 54.68*30;
    }
#else
    elec.energy[0] = 51.2*30;
    elec.energy[1] = 12.64*31;
    elec.energy[2] = 34.94*30;
    elec.energy[3] = 27.65*31;
    elec.energy[4] = 43.68*31;
    elec.energy[5] = 54.68*30;
#endif
}

void CDataManager::getUserChargeReocrd(QList<DM_UserChargeRecord> &chargeRecord)
{
    chargeRecord.clear();

    DM_UserChargeRecord oneUserChargeRecord;
    for(int i = 0;i < 8;i++)
    {
        oneUserChargeRecord.chargeTime = QString("2016-0%1-10")
                                            .arg(QString::number(i + 1));

        oneUserChargeRecord.money = 123.3 + i * 10;
        oneUserChargeRecord.moneyBeforCharge = 50.0 + i * 20;
        oneUserChargeRecord.moneyAfterCharge = oneUserChargeRecord.money + oneUserChargeRecord.moneyBeforCharge;

        chargeRecord.push_back(oneUserChargeRecord);
    }
}

void CDataManager::getUserReplyRecord(QList<DM_UserReplyRecord> &replyRecord)
{
    replyRecord.clear();

    DM_UserReplyRecord oneUserReplyRecord;
    for(int i = 0;i < 5;i++)
    {
        oneUserReplyRecord.replyTime = QString("2016-0%1-10 %2:00:33")
                                          .arg(QString::number(i + 1))
                                          .arg(QString::number(i + 10));

        replyRecord.push_back(oneUserReplyRecord);
    }
}

void CDataManager::getUserPowerCutRecord(QList<DM_UserPowerCutRecord> &powerCutRecord)
{
    powerCutRecord.clear();

    DM_UserPowerCutRecord oneUserPowerCutRecord;
    for(int i = 0;i < 3;i++)
    {
        oneUserPowerCutRecord.powerCutTime = QString("2016-0%1-10 %2:00:33")
                                            .arg(QString::number(i + 1))
                                            .arg(QString::number(i + 10));
        oneUserPowerCutRecord.moneyOfOwe = 10 + i * 1.2;

        powerCutRecord.push_back(oneUserPowerCutRecord);
    }
}

void CDataManager::getUserNoticeInfo(DM_UserNoticeInfo &noticeInfo)
{
    noticeInfo.arrearageAccount = 20;
    noticeInfo.laClosingCondition = QObject::tr("正常");
}

void CDataManager::getUserNoticeInfos(QList<DM_UserNoticeInfoS> &noticeInfoList)
{
    noticeInfoList.clear();

    DM_UserNoticeInfoS oneUserNoticeInfo;
    for(int i = 0;i < 5;i++)
    {
        oneUserNoticeInfo.curElec.balance = -20 + i * 8;
        oneUserNoticeInfo.curElec.balanceTime = QString("2016-0%1-%2 00:00")
                                                .arg(QString::number(i + 1))
                                                .arg(QString::number(i + 10));
        oneUserNoticeInfo.curElec.energy = 13 + i * 3;

        oneUserNoticeInfo.userInfo.householdNum = QString("48613486%1").arg(QString::number(i + 1));
        oneUserNoticeInfo.userInfo.houseNum = QString("%1482166").arg(QString::number(i + 1));
        oneUserNoticeInfo.userInfo.name = QString("Name%1").arg(QString::number(i + 1));

        oneUserNoticeInfo.userNoticeInfo.arrearageAccount = 20 + 2 * i;
        oneUserNoticeInfo.userNoticeInfo.laClosingCondition = QObject::tr("正常");

        noticeInfoList.append(oneUserNoticeInfo);
    }
}

void CDataManager::getUserNoticeConnect(QList<DM_UserNoticeConnect> &noticeConnectList)
{
    noticeConnectList.clear();

    DM_UserNoticeConnect oneNoticeConnect;
    for (int i = 0;i < 3;i++)
    {
        oneNoticeConnect.beginTime = QString("2016-0%1-10 %2:00")
                                    .arg(QString::number(i + 1))
                                    .arg(QString::number(i + 10));
        oneNoticeConnect.endTime = QString("2016-0%1-10 %2:00")
                                    .arg(QString::number(i + 5))
                                    .arg(QString::number(i + 15));
        oneNoticeConnect.title = QObject::tr("公 告 信 息");
        oneNoticeConnect.connect = QObject::tr("xtsagas测试信息54646阿达按时段阿达达到按时段却无法给");

        noticeConnectList.append(oneNoticeConnect);
    }
}

/************************************************************************/
/* 以下为维护卡信息                                                     */
/************************************************************************/
void CDataManager::getTerminalInfo(DM_TerminalInfo &terminal)
{
#ifndef WIN32
    terminal.address = QString::number(CIfDatacomm::GetTerminalInfo().GetRTUA().m_Address);
    terminal.location = QString::number(CIfDatacomm::GetTerminalInfo().GetRTUA().m_AreaCode);
#else
    terminal.address = "9614861486312";
    terminal.location = QObject::tr("68741561");
#endif
}

void CDataManager::getPowerMonitorInfo(QList<DM_PowerMonitorInfo> &powerMonitorInfo)
{
    powerMonitorInfo.clear();

    DM_PowerMonitorInfo onePowerMonitorInfo;
    for (int i = 0;i < 3; i++)
    {
        onePowerMonitorInfo.houseNum = QString("651334%1").arg(QString::number(i));
        onePowerMonitorInfo.positiveActive = 225.6 + i;
        onePowerMonitorInfo.reveriseActive = 645.64 + i;
        onePowerMonitorInfo.realtimeVolgate = 397.2 + i;
        onePowerMonitorInfo.realtimeCurrent = 154.6 + i;
        onePowerMonitorInfo.realtimePower = 1567.2 + i;
        onePowerMonitorInfo.realtimePowerFactor =  1535.2 + i;

        powerMonitorInfo.push_back(onePowerMonitorInfo);
    }
}

void CDataManager::getDeviceNum(QList<DM_DeviceNum> &deviceNum)
{
    deviceNum.clear();

    DM_DeviceNum oneDeviceNum;
    for (int i = 0;i < 3; i++)
    {
        oneDeviceNum.number = QString("674563%1").arg(QString::number(i));
        oneDeviceNum.deviceNum = QString("3675463641%1").arg(QString::number(i));

        deviceNum.push_back(oneDeviceNum);
    }
    
}

void CDataManager::getTerminalBaseInfo(DM_TerminalBaseInfo &terminalBaseInfo)
{
    DM_TerminalInfo terminalInfo;
    getTerminalInfo(terminalInfo);
    terminalBaseInfo.terminalInfo.address = terminalInfo.address;
    terminalBaseInfo.terminalInfo.location = terminalInfo.location;

#ifndef WIN32
    terminalBaseInfo.softwareVersion = QString(CIfDatacomm::GetTerminalInfo().GetVersion().m_SWVersion);
    terminalBaseInfo.hardwareVersion = QString(CIfDatacomm::GetTerminalInfo().GetVersion().m_HWVersion);
    terminalBaseInfo.encryptVersion = QString(CIfDatacomm::GetTerminalInfo().GetVersion().m_ESAMKEYVersion);
#else
    terminalBaseInfo.softwareVersion = "0.1.52";
    terminalBaseInfo.hardwareVersion = "0.1.09";
    terminalBaseInfo.encryptVersion = "0.1.03";
#endif
}

void CDataManager::getUserFileInfo(QList<DM_UserFileInfo> &userFileInfoList)
{
    userFileInfoList.clear();
#ifndef WIN32
    DM_UserFileInfo oneUserFileInfo;
    for (int i = 1; i <= CONS_DEVICE_MAX_COUNT; i++)
    {
        oneUserFileInfo.num = i;
        oneUserFileInfo.residenceNum = "483611" + QString::number(i + 1);
        oneUserFileInfo.houseNum = "852133" + QString::number(i + 2);
        oneUserFileInfo.deviceNum = QString::number(CIfDatacomm::GetTerminalInfo().GetUserParm(i).m_MeterNo);
        char temp[6];
        for (int i = 0;i < 6;i++)
        {
            temp[i] = bcdc(CIfDatacomm::GetTerminalInfo().GetUserParm(i).m_MeterAddr[i]);
        }
        oneUserFileInfo.deviceAddr = QString(temp);

        userFileInfoList.push_back(oneUserFileInfo);
    }
#else
    DM_UserFileInfo oneUserFileInfo;
    for (int i = 0; i < 27; i++)
    {
        oneUserFileInfo.num = i + 1;
        oneUserFileInfo.residenceNum = "483611" + QString::number(i + 1);
        oneUserFileInfo.houseNum = "852133" + QString::number(i + 2);
        oneUserFileInfo.deviceNum = "953133" + QString::number(i + 1);
        oneUserFileInfo.deviceAddr = "48941563186" + QString::number(i + 6);

        userFileInfoList.push_back(oneUserFileInfo);
    }
#endif
}

inline QString int32uToIp(INT32U ip)
{
    QString temp = QString("%1.%2.%3.%4").
                       arg(QString::number(ip & 0x000000ff)).
                       arg(QString::number((ip >> 8) & 0x000000ff)).
                       arg(QString::number((ip >> 16) & 0x000000ff)).
                       arg(QString::number((ip >> 24) & 0x000000ff));

    return temp;
}

inline QString getCommunicateState(S_BYTE state)
{
    QString temp;
    switch(state)
    {
    case 0:
        temp = QObject::tr("差");
        break;
    case 1:
        temp = QObject::tr("良好");
        break;
    case 2:
        temp = QObject::tr("优秀");
        break;
    default:
        break;
    }

    return temp;
}

inline QString getSignalStrength(S_BYTE state)
{
    QString temp;
    switch(state)
    {
    case 0:
        temp = QObject::tr("弱");
        break;
    case 1:
        temp = QObject::tr("良");
        break;
    case 2:
        temp = QObject::tr("强");
        break;
    default:
        break;
    }

    return temp;
}

void CDataManager::getCommunicateInfo(DM_CommunicateInfo &communicateInfo)
{
#ifndef WIN32
    communicateInfo.serverAddr = "46187893110";
    communicateInfo.apn = QString(CIfDatacomm::GetTerminalInfo().GetCommParm().m_Apn);
    communicateInfo.port = QString::number(CIfDatacomm::GetTerminalInfo().GetCommParm().m_ServerPort);
    communicateInfo.serverIpAddr = int32uToIp(CIfDatacomm::GetTerminalInfo().GetCommParm().m_ServerIP);
    communicateInfo.localIpAddr = int32uToIp(CIfDatacomm::GetTerminalInfo().GetCommParm().m_LocalIP);
    communicateInfo.communicateState = getCommunicateState(CIfDatacomm::GetTerminalInfo().GetCommParm().m_NetState);
    communicateInfo.signalStrength = getSignalStrength(CIfDatacomm::GetTerminalInfo().GetCommParm().m_Singal);
    communicateInfo.heartBeat = QString::number(CIfDatacomm::GetTerminalInfo().GetCommParm().m_HeartBeat);
#else
    communicateInfo.serverAddr = "46187893110";
    communicateInfo.apn = "9999";
    communicateInfo.port = "8989";
    communicateInfo.serverIpAddr = "172.16.55.10";
    communicateInfo.localIpAddr = "192.168.1.10";
    communicateInfo.communicateState = QObject::tr("良好");
    communicateInfo.signalStrength = QObject::tr("强");
    communicateInfo.heartBeat = "50";
#endif
}

unsigned int CDataManager::getBacklight()
{
#ifndef WIN32
    QFile file(BRIGHTNESS_FILE);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        xtRelease(("open brightness fail"));
        return -1;
    }
    QTextStream in(&file);
    QString brightness = in.readAll();
    file.close();
    m_sSystemParam.backlight = (unsigned int)(brightness.toDouble() / 200 * 100);

    return m_sSystemParam.backlight;
#else
    return m_sSystemParam.backlight;
#endif
}

unsigned int CDataManager::getLoopTime() const
{
    return m_sSystemParam.loopTime;
}

QString CDataManager::getBackLightStartTime() const
{
    return m_sSystemParam.startTime;
}

QString CDataManager::getBackLightEndTime() const
{
    return m_sSystemParam.endTime;
}

bool CDataManager::setBacklight(unsigned int backlight)
{
#ifndef WIN32
    if (backlight > 100)
    {
        return false;
    }

    QFile file(BRIGHTNESS_FILE);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        xtRelease(("open brightness fail"));
        return false;
    }
    QTextStream out(&file);
    out << QString::number(backlight * 2);
    file.close();

    m_sSystemParam.backlight = backlight;
    return true;
#else
    if (backlight > 100)
    {
        return false;
    }

    m_sSystemParam.backlight = backlight;
    return true;
#endif
}

bool CDataManager::setLoopTime(unsigned int loopTime)
{
    if (loopTime > 9999)
    {
        return false;
    }

    m_sSystemParam.loopTime = loopTime;
    return true;
}

bool CDataManager::setBackLightStartTime(QString& startTime)
{
    if (startTime.isEmpty())
    {
        return false;
    }

    m_sSystemParam.startTime = startTime;
    return true;
}

bool CDataManager::setBackLightEndTime(QString& endTime)
{
    if (endTime.isEmpty())
    {
        return false;
    }

    m_sSystemParam.endTime = endTime;
    return true;
}

void CDataManager::getTestChannelInfo(QList<DM_TestChannel> &testChannel)
{
    testChannel.clear();

    DM_TestChannel oneTestChannel;
    for (int i = 0; i < 7; i++)
    {
        oneTestChannel.num = i + 1;
        oneTestChannel.deviceNum = QString("35783456%1").arg(QString::number(i + 10));
        if (i % 2)
        {
            oneTestChannel.communicateState = QObject::tr("正常");
        }
        else
        {
            oneTestChannel.communicateState = QObject::tr("异常");
        }

        testChannel.push_back(oneTestChannel);
    }
}