#ifndef DATAMANAGER_H
#define DATAMANAGER_H


#include <QString>
#include "src/dataLevel/dataComm/sharemem/ipcSharemem.h"

/************************************************************************/
/* 以下为用户卡信息                                                     */
/************************************************************************/
struct DM_UserInfo {
    QString name;           //客户名称
    QString householdNum;   //户号
    QString houseNum;       //门牌号
};

struct DM_UserCurrentElectro {
    QString balanceTime;  //结算时间
    float balance;        //当前余额
    float energy;         //当前总电量
};

struct DM_UserChargeInfo {
    QString chargeMeterNo;  //充值表号
    float money;            //充值金额
};

/*
 *  @bref:
 *      最近一周的用电量
 */
struct DM_LastestWeekElectro {
    float energy[7];
};

/*
 *  @bref:
 *      最近一个月的用电量，按周统计
 */
struct DM_LastestMonthElectro {
    float energy[4];
};

/*
 *  @bref:
 *      最近半年的用电量，按月统计
 */
struct DM_LastestHalfYearElectro {
    float energy[6];          /*  6个月的用电使用情况     */
};

/*
 *  @bref:
 *      用户充值记录
 */
struct DM_UserChargeRecord {
    QString chargeTime;
    float   money;
    float   moneyBeforCharge;
    float   moneyAfterCharge;
};

/*
 *  @bref:
 *      用户复电记录
 */
struct DM_UserReplyRecord {
    QString replyTime;
};

/*
 *  @bref:
 *      用户欠费记录
 */
struct DM_UserPowerCutRecord {
    QString powerCutTime;
    float   moneyOfOwe;
};

/*
 *  @bref:
 *      用户公告信息
 */
struct DM_UserNoticeInfo {
    float arrearageAccount;
    QString   laClosingCondition;
};

/*
 *  @bref:
 *      用户公告信息集
 */
struct DM_UserNoticeInfoS {
    DM_UserInfo userInfo;
    DM_UserCurrentElectro curElec;
    DM_UserNoticeInfo userNoticeInfo;
};

/*
 *  @bref:
 *      用户公告信息集
 */
struct DM_UserNoticeConnect{
    QString beginTime;
    QString endTime;
    QString title;
    QString connect;
};

/************************************************************************/
/* 以下为维护卡信息                                                     */
/************************************************************************/
/*
 *  @bref:
 *      终端信息
 */
struct DM_TerminalInfo {
    QString address;           // 终端地址
    QString location;          // 地市区县号码
};

/*
 *  @bref:
 *      用电检测信息
 */
struct DM_PowerMonitorInfo {
    QString houseNum;           // 门牌号
    float positiveActive;     // 正向有功总电能
    float reveriseActive;     // 反向有功总电能
    float realtimeVolgate;    // 实时电压
    float realtimeCurrent;    // 实时电流
    float realtimePower;      // 实时总功率
    float realtimePowerFactor;// 实时总功率因数
};

/*
 *  @bref:
 *      表号信息
 */
struct DM_DeviceNum{
    QString number;             // 序号
    QString deviceNum;          // 表号
};

/*
 *  @bref:
 *      基本信息
 */
struct DM_TerminalBaseInfo{
    DM_TerminalInfo terminalInfo;
    QString softwareVersion;             // 软件版本
    QString hardwareVersion;             // 硬件版本号
    QString encryptVersion;              // 密钥版本信息
};

/*
 *  @bref:
 *      用户档案
 */
struct DM_UserFileInfo{
    unsigned int num;                    // 序号
    QString residenceNum;                // 户号
    QString houseNum;                    // 门牌号
    QString deviceNum;                   // 表号
    QString deviceAddr;                  // 表地址
};

/*
 *  @bref:
 *      通讯参数
 */
struct DM_CommunicateInfo{
    QString serverAddr;                  // 主站地址
    QString apn;                         // APN
    QString port;                        // 端口号
    QString serverIpAddr;                // IP地址(主站)
    QString localIpAddr;                 // 本机IP
    QString communicateState;            // 通讯状态
    QString signalStrength;              // 信号强度
    QString heartBeat;                   // 心跳周期
};

/*
 *  @bref:
 *      系统参数
 */
struct DM_SystemParam{
    unsigned int backlight;              // 背光百分比
    unsigned int loopTime;               // 循环播放时间
    QString startTime;                   // 背光开始时间
    QString endTime;                     // 背光关闭时间
};

/*
 *  @bref:
 *      测试通道
 */
struct DM_TestChannel{
    unsigned int num;                    // 序号
    QString deviceNum;                   // 表号
    QString communicateState;            // 485通讯状况
};
/************************************************************************/
/*CDataManager                                                          */
/************************************************************************/
class CDataManager
{
private:
    CDataManager();
    ~CDataManager();

public:
    static CDataManager* getInstance();

/************************************************************************/
/* 以下为用户卡信息                                                     */
/************************************************************************/
    void getUserInfo(DM_UserInfo &user);
    void getUserCurrentElectro(DM_UserCurrentElectro &curElec);
    /*
     *  @Func:      userCharge
     *  @Desc:      用户充值
     *  @PaamIn:    
     *      chargeInfo:充值信息
     *  @Return:
     *      true:   充值成功
     *      false:  充值失败
     */
    bool userCharge(const DM_UserChargeInfo &chargeInfo);

    //历史用电查询
    void getLastWeekElectro(DM_LastestWeekElectro &elec);
    void getLastMonthElectro(DM_LastestMonthElectro &elec);
    void getLastHalfYearElectro(DM_LastestHalfYearElectro &elec);

    // 用户充值记录
    void getUserChargeReocrd(QList<DM_UserChargeRecord> &chargeRecord);
    // 用户复电记录
    void getUserReplyRecord(QList<DM_UserReplyRecord> &replyRecord);
    // 用户欠费记录
    void getUserPowerCutRecord(QList<DM_UserPowerCutRecord> &powerCutRecord);
    // 用户公告信息
    void getUserNoticeInfo(DM_UserNoticeInfo &noticeInfo);
    // 所有用户公告信息
    void getUserNoticeInfos(QList<DM_UserNoticeInfoS> &noticeInfoList);
    // 获取固定格式的公告信息
    void getUserNoticeConnect(QList<DM_UserNoticeConnect> &noticeConnectList);

/************************************************************************/
/* 以下为维护卡信息                                                     */
/************************************************************************/
    // 获取终端信息
    void getTerminalInfo(DM_TerminalInfo &terminal);
    // 获取用电检测信息
    void getPowerMonitorInfo(QList<DM_PowerMonitorInfo> &powerMonitorInfo);
    // 获取设备序号信息
    void getDeviceNum(QList<DM_DeviceNum> &deviceNum);
    // 获取终端基本信息
    void getTerminalBaseInfo(DM_TerminalBaseInfo &terminalBaseInfo);
    // 获取用户档案
    void getUserFileInfo(QList<DM_UserFileInfo> &userFileInfoList);
    // 获取通讯参数
    void getCommunicateInfo(DM_CommunicateInfo &communicateInfo);
    // 获取系统参数
    unsigned int getBacklight();
    // 获取循环播放时间
    unsigned int getLoopTime() const;
    // 获取背光开始时间
    QString getBackLightStartTime() const;
    // 获取背光关闭时间
    QString getBackLightEndTime() const;
    // 设置系统参数
    bool setBacklight(unsigned int backlight);
    // 设置循环播放时间
    bool setLoopTime(unsigned int loopTime);
    // 设置背光开始时间
    bool setBackLightStartTime(QString& startTime);
    // 设置背光关闭时间
    bool setBackLightEndTime(QString& endTime);
    // 获取测试通道信息
    void getTestChannelInfo(QList<DM_TestChannel> &testChannel);

private:
    unsigned int getCardMp() const {return IPCSHAREMEM.m_CurCardMp;}

private:
    ///////  测试用 ////////////////
    float m_balance;            //当前余额
    float m_energy;
    DM_SystemParam              m_sSystemParam;
    ///////  测试用 ////////////////
};

#endif // DATAMANAGER_H
