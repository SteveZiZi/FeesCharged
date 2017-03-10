#ifndef DATAMANAGER_H
#define DATAMANAGER_H


#include <QString>
#include "src/dataLevel/dataComm/sharemem/ipcSharemem.h"

/************************************************************************/
/* ����Ϊ�û�����Ϣ                                                     */
/************************************************************************/
struct DM_UserInfo {
    QString name;           //�ͻ�����
    QString householdNum;   //����
    QString houseNum;       //���ƺ�
};

struct DM_UserCurrentElectro {
    QString balanceTime;  //����ʱ��
    float balance;        //��ǰ���
    float energy;         //��ǰ�ܵ���
};

struct DM_UserChargeInfo {
    QString chargeMeterNo;  //��ֵ���
    float money;            //��ֵ���
};

/*
 *  @bref:
 *      ���һ�ܵ��õ���
 */
struct DM_LastestWeekElectro {
    float energy[7];
};

/*
 *  @bref:
 *      ���һ���µ��õ���������ͳ��
 */
struct DM_LastestMonthElectro {
    float energy[4];
};

/*
 *  @bref:
 *      ���������õ���������ͳ��
 */
struct DM_LastestHalfYearElectro {
    float energy[6];          /*  6���µ��õ�ʹ�����     */
};

/*
 *  @bref:
 *      �û���ֵ��¼
 */
struct DM_UserChargeRecord {
    QString chargeTime;
    float   money;
    float   moneyBeforCharge;
    float   moneyAfterCharge;
};

/*
 *  @bref:
 *      �û������¼
 */
struct DM_UserReplyRecord {
    QString replyTime;
};

/*
 *  @bref:
 *      �û�Ƿ�Ѽ�¼
 */
struct DM_UserPowerCutRecord {
    QString powerCutTime;
    float   moneyOfOwe;
};

/*
 *  @bref:
 *      �û�������Ϣ
 */
struct DM_UserNoticeInfo {
    float arrearageAccount;
    QString   laClosingCondition;
};

/*
 *  @bref:
 *      �û�������Ϣ��
 */
struct DM_UserNoticeInfoS {
    DM_UserInfo userInfo;
    DM_UserCurrentElectro curElec;
    DM_UserNoticeInfo userNoticeInfo;
};

/*
 *  @bref:
 *      �û�������Ϣ��
 */
struct DM_UserNoticeConnect{
    QString beginTime;
    QString endTime;
    QString title;
    QString connect;
};

/************************************************************************/
/* ����Ϊά������Ϣ                                                     */
/************************************************************************/
/*
 *  @bref:
 *      �ն���Ϣ
 */
struct DM_TerminalInfo {
    QString address;           // �ն˵�ַ
    QString location;          // �������غ���
};

/*
 *  @bref:
 *      �õ�����Ϣ
 */
struct DM_PowerMonitorInfo {
    QString houseNum;           // ���ƺ�
    float positiveActive;     // �����й��ܵ���
    float reveriseActive;     // �����й��ܵ���
    float realtimeVolgate;    // ʵʱ��ѹ
    float realtimeCurrent;    // ʵʱ����
    float realtimePower;      // ʵʱ�ܹ���
    float realtimePowerFactor;// ʵʱ�ܹ�������
};

/*
 *  @bref:
 *      �����Ϣ
 */
struct DM_DeviceNum{
    QString number;             // ���
    QString deviceNum;          // ���
};

/*
 *  @bref:
 *      ������Ϣ
 */
struct DM_TerminalBaseInfo{
    DM_TerminalInfo terminalInfo;
    QString softwareVersion;             // ����汾
    QString hardwareVersion;             // Ӳ���汾��
    QString encryptVersion;              // ��Կ�汾��Ϣ
};

/*
 *  @bref:
 *      �û�����
 */
struct DM_UserFileInfo{
    unsigned int num;                    // ���
    QString residenceNum;                // ����
    QString houseNum;                    // ���ƺ�
    QString deviceNum;                   // ���
    QString deviceAddr;                  // ���ַ
};

/*
 *  @bref:
 *      ͨѶ����
 */
struct DM_CommunicateInfo{
    QString serverAddr;                  // ��վ��ַ
    QString apn;                         // APN
    QString port;                        // �˿ں�
    QString serverIpAddr;                // IP��ַ(��վ)
    QString localIpAddr;                 // ����IP
    QString communicateState;            // ͨѶ״̬
    QString signalStrength;              // �ź�ǿ��
    QString heartBeat;                   // ��������
};

/*
 *  @bref:
 *      ϵͳ����
 */
struct DM_SystemParam{
    unsigned int backlight;              // ����ٷֱ�
    unsigned int loopTime;               // ѭ������ʱ��
    QString startTime;                   // ���⿪ʼʱ��
    QString endTime;                     // ����ر�ʱ��
};

/*
 *  @bref:
 *      ����ͨ��
 */
struct DM_TestChannel{
    unsigned int num;                    // ���
    QString deviceNum;                   // ���
    QString communicateState;            // 485ͨѶ״��
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
/* ����Ϊ�û�����Ϣ                                                     */
/************************************************************************/
    void getUserInfo(DM_UserInfo &user);
    void getUserCurrentElectro(DM_UserCurrentElectro &curElec);
    /*
     *  @Func:      userCharge
     *  @Desc:      �û���ֵ
     *  @PaamIn:    
     *      chargeInfo:��ֵ��Ϣ
     *  @Return:
     *      true:   ��ֵ�ɹ�
     *      false:  ��ֵʧ��
     */
    bool userCharge(const DM_UserChargeInfo &chargeInfo);

    //��ʷ�õ��ѯ
    void getLastWeekElectro(DM_LastestWeekElectro &elec);
    void getLastMonthElectro(DM_LastestMonthElectro &elec);
    void getLastHalfYearElectro(DM_LastestHalfYearElectro &elec);

    // �û���ֵ��¼
    void getUserChargeReocrd(QList<DM_UserChargeRecord> &chargeRecord);
    // �û������¼
    void getUserReplyRecord(QList<DM_UserReplyRecord> &replyRecord);
    // �û�Ƿ�Ѽ�¼
    void getUserPowerCutRecord(QList<DM_UserPowerCutRecord> &powerCutRecord);
    // �û�������Ϣ
    void getUserNoticeInfo(DM_UserNoticeInfo &noticeInfo);
    // �����û�������Ϣ
    void getUserNoticeInfos(QList<DM_UserNoticeInfoS> &noticeInfoList);
    // ��ȡ�̶���ʽ�Ĺ�����Ϣ
    void getUserNoticeConnect(QList<DM_UserNoticeConnect> &noticeConnectList);

/************************************************************************/
/* ����Ϊά������Ϣ                                                     */
/************************************************************************/
    // ��ȡ�ն���Ϣ
    void getTerminalInfo(DM_TerminalInfo &terminal);
    // ��ȡ�õ�����Ϣ
    void getPowerMonitorInfo(QList<DM_PowerMonitorInfo> &powerMonitorInfo);
    // ��ȡ�豸�����Ϣ
    void getDeviceNum(QList<DM_DeviceNum> &deviceNum);
    // ��ȡ�ն˻�����Ϣ
    void getTerminalBaseInfo(DM_TerminalBaseInfo &terminalBaseInfo);
    // ��ȡ�û�����
    void getUserFileInfo(QList<DM_UserFileInfo> &userFileInfoList);
    // ��ȡͨѶ����
    void getCommunicateInfo(DM_CommunicateInfo &communicateInfo);
    // ��ȡϵͳ����
    unsigned int getBacklight();
    // ��ȡѭ������ʱ��
    unsigned int getLoopTime() const;
    // ��ȡ���⿪ʼʱ��
    QString getBackLightStartTime() const;
    // ��ȡ����ر�ʱ��
    QString getBackLightEndTime() const;
    // ����ϵͳ����
    bool setBacklight(unsigned int backlight);
    // ����ѭ������ʱ��
    bool setLoopTime(unsigned int loopTime);
    // ���ñ��⿪ʼʱ��
    bool setBackLightStartTime(QString& startTime);
    // ���ñ���ر�ʱ��
    bool setBackLightEndTime(QString& endTime);
    // ��ȡ����ͨ����Ϣ
    void getTestChannelInfo(QList<DM_TestChannel> &testChannel);

private:
    unsigned int getCardMp() const {return IPCSHAREMEM.m_CurCardMp;}

private:
    ///////  ������ ////////////////
    float m_balance;            //��ǰ���
    float m_energy;
    DM_SystemParam              m_sSystemParam;
    ///////  ������ ////////////////
};

#endif // DATAMANAGER_H
