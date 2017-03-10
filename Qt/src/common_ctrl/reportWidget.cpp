#include "reportWidget.h"
#include <QHeaderView>
#include <QScrollBar>
#include <QDateTime>

#include "src/main/common.h"


#define TABLE_ITEM_H    35

#define CAPTION_W       144
#define CONTENT_W       160


CReportWidget::CReportWidget(QWidget* parent)
: MTableWidget(parent)
, captionW(CAPTION_W)
, contentW(CONTENT_W)
, rowH(TABLE_ITEM_H)
{
}

CReportWidget::~CReportWidget()
{

}


void CReportWidget::createTableWidget(int row, int column)
{
    this->setRowCount(row);
    this->setColumnCount(column);

    this->verticalHeader()->hide();
    this->horizontalHeader()->hide();
    this->horizontalScrollBar()->hide();
    this->verticalScrollBar()->hide();

    this->setShowGrid(false);
    this->setGridStyle(new MGridStyle(QColor(TABLE_LINE_CLR), true, true));
    this->setShowTopBottomLine(true, true);
    this->setShowLeftRightLine(true, true);
    this->setAlternatingRowColors(false);


    this->setColumnWidth(0, captionW);
    this->setColumnWidth(1, contentW);
    for (int row = 0; row < this->rowCount(); ++row) {
        for (int column = 0; column < this->columnCount(); ++column) {
            QTableWidgetItem *newItem = new QTableWidgetItem();
            if (column == 0) {
                newItem->setFlags(Qt::ItemIsSelectable);
                newItem->setTextAlignment(Qt::AlignVCenter|Qt::AlignRight);
            }
            else {
                newItem->setTextAlignment(Qt::AlignVCenter|Qt::AlignLeft);
            }
            this->setItem(row, column, newItem);
        }
        this->setRowHeight(row, rowH);
    }
}


CUserInfoReport::CUserInfoReport(QWidget* parent)
: CReportWidget(parent)
{

}

CUserInfoReport::~CUserInfoReport()
{

}


void CUserInfoReport::updateCaptionColumn()
{
    Q_ASSERT(this->rowCount() >= USER_INFO_ROW);
    QString caption[USER_INFO_ROW] = {
        tr("客 户 名 称 :"),
        tr("户 号 :"),
        tr("门 牌 号 :")
    };

    for (int row = 0; row < USER_INFO_ROW; ++row) {
        this->item(row, 0)->setText(caption[row]);
    }
}

void CUserInfoReport::updateUserInfo(const DM_UserInfo &userInfo)
{
    this->item(0, 1)->setText(userInfo.name);
    this->item(1, 1)->setText(userInfo.householdNum);
    this->item(2, 1)->setText(userInfo.houseNum);
}



void CUserQueryResultReport::updateCaptionColumn()
{
    QString caption[USER_INFO_ROW] = {
        tr("结 算 时 间 :"),
        tr("当 前 余 额 :"),
        tr("当 前 总 电 量 :")
    };
    CUserInfoReport::updateCaptionColumn();

    for(int row = USER_INFO_ROW; row < QUERY_RESULT_ROW; row++) {
        this->item(row, 0)->setText(caption[row-USER_INFO_ROW]);
    }
}

void CUserQueryResultReport::updateUserResultInfo(const DM_UserCurrentElectro &userInfo)
{
    this->item(USER_INFO_ROW, 1)->setText(userInfo.balanceTime);
    this->item(USER_INFO_ROW+1, 1)->setText(QString("%1").arg(userInfo.balance, 0, 'f', 2, QChar('0')));
    float balance = userInfo.balance;
    if (balance <= 0)
        this->item(USER_INFO_ROW+1, 1)->setTextColor("red");
    else
        this->item(USER_INFO_ROW+1, 1)->setTextColor("black");
    this->item(USER_INFO_ROW+2, 1)->setText(QString("%1kWh").arg(userInfo.energy, 0, 'f', 2, QChar('0')));

}


void CUserChargeReport::updateCaptionColumn()
{
    QString caption[USER_INFO_ROW] = {
        tr("充 值 表 号 :"),
        tr("当 前 余 额 :"),
        tr("充 值 金 额 :")
    };
    CUserInfoReport::updateCaptionColumn();

    for(int row = USER_INFO_ROW; row < ROWS; row++) {
        this->item(row, 0)->setText(caption[row-USER_INFO_ROW]);
    }
}

void CUserChargeReport::updateChargedInfo(const DM_UserCurrentElectro &currentElectro,
                                          const DM_UserChargeInfo &chargeInfo)
{
    this->item(USER_INFO_ROW, 1)->setText(chargeInfo.chargeMeterNo);
    this->item(USER_INFO_ROW+1, 1)->setText(QString("%1").arg(currentElectro.balance, 0, 'f', 2, QChar('0')));
    this->item(USER_INFO_ROW+2, 1)->setText(QString("%1").arg(chargeInfo.money, 0, 'f', 2, QChar('0')));
}

void CUserChargeReport::updateChargeMemony(float money)
{
    this->item(USER_INFO_ROW+2, 1)->setText(QString("%1").arg(money, 0, 'f', 2, QChar('0')));
}

void CUserChargeReport::updateBalance(float balance)
{
    this->item(USER_INFO_ROW+1, 1)->setText(QString("%1").arg(balance, 0, 'f', 2, QChar('0')));
}

CUserNoticeReport::CUserNoticeReport(QWidget* parent /* = 0 */)
: CUserQueryResultReport(parent)
{

}

CUserNoticeReport::~CUserNoticeReport()
{

}

void CUserNoticeReport::updateCaptionColumn()
{
    QString caption[USER_NOTICE_INFO_ROW] = {
        tr("欠 费 情 况 :"),
        tr("拉 合 闸 情 况 :"),
    };

    CUserQueryResultReport::updateCaptionColumn();

    for (int row = QUERY_RESULT_ROW; row < QUERY_RESULT_ROW + USER_NOTICE_INFO_ROW; ++row) {
        this->item(row, 0)->setText(caption[row - QUERY_RESULT_ROW]);
    }
}

void CUserNoticeReport::updateUserNoticeInfo(const DM_UserNoticeInfo &userNoticeInfo)
{
    this->item(QUERY_RESULT_ROW,1)->setText(QString("%1").arg(userNoticeInfo.arrearageAccount, 0 ,'f', 2));
    if (userNoticeInfo.arrearageAccount > 0)
        this->item(QUERY_RESULT_ROW,1)->setTextColor("red");
    else
        this->item(QUERY_RESULT_ROW,1)->setTextColor("none");
    this->item(QUERY_RESULT_ROW + 1,1)->setText(userNoticeInfo.laClosingCondition);
}

/************************************************************************/
/* 以下为维护卡信息                                                     */
/************************************************************************/

CTerminalInfoReport::CTerminalInfoReport(QWidget* parent)
: CReportWidget(parent)
{

}

CTerminalInfoReport::~CTerminalInfoReport()
{

}

void CTerminalInfoReport::updateCaptionColumn()
{
    Q_ASSERT(this->rowCount() >= TERMINAL_INFO_ROW);
    QString caption[TERMINAL_INFO_ROW] = {
        tr("终 端 地 址 :"),
        tr("地 市 区 县 码 :"),
    };

    for (int row = 0; row < TERMINAL_INFO_ROW; ++row) {
        this->item(row, 0)->setText(caption[row]);
    }
}

void CTerminalInfoReport::updateTerminalInfo(const DM_TerminalInfo &terminalInfo)
{
    this->item(0, 1)->setText(terminalInfo.address);
    this->item(1, 1)->setText(terminalInfo.location);
}


CPowerMonitorReport::CPowerMonitorReport(QWidget* parent)
: CReportWidget(parent)
{

}

CPowerMonitorReport::~CPowerMonitorReport()
{

}

void CPowerMonitorReport::updateCaptionColumn()
{
    Q_ASSERT(this->rowCount() >= POWER_MONITOR_ROW);
    QString caption[POWER_MONITOR_ROW] = {
        tr("门 牌 号 :"),
        tr("正 向 有 功 总 电 能 :"),
        tr("反 向 有 功 总 电 能 :"),
        tr("实 时 电 压 :"),
        tr("实 时 电 流 :"),
        tr("实 时 总 功 率 :"),
        tr("实 时 总 功 率 因 数 :")
    };

    for (int row = 0; row < POWER_MONITOR_ROW; ++row) {
        this->item(row, 0)->setText(caption[row]);
    }
}

void CPowerMonitorReport::updatePowerMonitorInfo(const DM_PowerMonitorInfo &powerMonitorInfo)
{
    this->item(0, 1)->setText(powerMonitorInfo.houseNum);
    this->item(1, 1)->setText(QString::number(powerMonitorInfo.positiveActive,'f',2));
    this->item(2, 1)->setText(QString::number(powerMonitorInfo.reveriseActive,'f',2));
    this->item(3, 1)->setText(QString::number(powerMonitorInfo.realtimeVolgate,'f',2));
    this->item(4, 1)->setText(QString::number(powerMonitorInfo.realtimeCurrent,'f',2));
    this->item(5, 1)->setText(QString::number(powerMonitorInfo.realtimePower,'f',2));
    this->item(6, 1)->setText(QString::number(powerMonitorInfo.realtimePowerFactor,'f',2));
}

CBaseInfoReport::CBaseInfoReport(QWidget* parent)
: CReportWidget(parent)
{

}
CBaseInfoReport::~CBaseInfoReport()
{

}

void CBaseInfoReport::updateCaptionColumn()
{
    Q_ASSERT(this->rowCount() >= BASE_INFO_ROW);
    QString caption[BASE_INFO_ROW] = {
        tr("终 端 地 址 :"),
        tr("地 市 区 县 码 :"),
        tr("软 件 版 本 :"),
        tr("硬 件 版 本 :"),
        tr("密 钥 版 本 信 息 :")
    };

    for (int row = 0; row < BASE_INFO_ROW; ++row) {
        this->item(row, 0)->setText(caption[row]);
    }
}

void CBaseInfoReport::updateBaseInfo(const DM_TerminalBaseInfo &baseInfo)
{
    this->item(0, 1)->setText(baseInfo.terminalInfo.address);
    this->item(1, 1)->setText(baseInfo.terminalInfo.location);
    this->item(2, 1)->setText(baseInfo.softwareVersion);
    this->item(3, 1)->setText(baseInfo.hardwareVersion);
    this->item(4, 1)->setText(baseInfo.encryptVersion);
}

CCommunicateInfoReport::CCommunicateInfoReport(QWidget* parent)
: CReportWidget(parent)
{

}
CCommunicateInfoReport::~CCommunicateInfoReport()
{

}

void CCommunicateInfoReport::updateCaptionColumn()
{
    Q_ASSERT(this->rowCount() >= COMMUNICATE_INFO_ROW);
    QString caption[COMMUNICATE_INFO_ROW] = {
        tr("主 站 地 址 :"),
        tr("A P N :"),
        tr("端 口 号 :"),
        tr("I P 地 址 :"),
        tr("本 地 I P :"),
        tr("远 程 通 讯 状 态 :"),
        tr("信 号 强 度 :"),
        tr("心 跳 周 期 :")
    };

    for (int row = 0; row < COMMUNICATE_INFO_ROW; ++row) {
        this->item(row, 0)->setText(caption[row]);
    }
}

void CCommunicateInfoReport::updateCommunicateInfo(const DM_CommunicateInfo &communicateInfo)
{
    this->item(0, 1)->setText(communicateInfo.serverAddr);
    this->item(1, 1)->setText(communicateInfo.apn);
    this->item(2, 1)->setText(communicateInfo.port);
    this->item(3, 1)->setText(communicateInfo.serverIpAddr);
    this->item(4, 1)->setText(communicateInfo.localIpAddr);
    this->item(5, 1)->setText(communicateInfo.communicateState);
    this->item(6, 1)->setText(communicateInfo.signalStrength);
    this->item(7, 1)->setText(communicateInfo.heartBeat);
}

CSystemParamReport::CSystemParamReport(QWidget *parent /* = 0 */)
: CReportWidget(parent)
{

}

CSystemParamReport::~CSystemParamReport()
{

}

void CSystemParamReport::updateCaptionColumn()
{
    Q_ASSERT(this->rowCount() >= SYSTEM_PARAM_ROW);
    QString caption[SYSTEM_PARAM_ROW] = {
        tr("背 光 亮 度 :"),
        tr("循 环 播 放 时 间 :"),
        tr("背 光 打 开 时 间 :"),
        tr("背 光 关 闭 时 间 :")
    };

    for (int row = 0; row < SYSTEM_PARAM_ROW; ++row) {
        this->item(row, 0)->setText(caption[row]);
    }
}