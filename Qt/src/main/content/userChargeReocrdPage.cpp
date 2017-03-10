#include "userChargeReocrdPage.h"

#include "inc/control/mTablewidget.h"
#include "inc/control/mPushButton.h"
#include "src/main/common.h"
#include "src/common_ctrl/reportWidget.h"

#include "src/dataLevel/datamanager.h"

#include <QGroupBox>
#include <QtGui/QScrollBar>
#include <QtGui/QHeaderView>


#define TABLE_L         192
#define TABLE_T         144
#define TABLE_W         305
#define TABLE_H         106

#define USER_INFO_L     0
#define USER_INFO_T     0
#define USER_INFO_W     CONTENT_SIZE_W
#define USER_INFO_H     60

#define CAPTION_L       240
#define CAPTION_T      (USER_INFO_T+USER_INFO_H+10)
#define CAPTION_W       200
#define CAPTION_H       40

#define GROUP_BOX_L     100
#define GROUP_BOX_T    (CAPTION_T+CAPTION_H+10)
#define GROUP_BOX_H    (CONTENT_SIZE_H-GROUP_BOX_T-20)

#define REPORT_ITEM_H   20
#define REPORT_ROWS    (10)


CUserInfoWidget::CUserInfoWidget(QWidget* parent/* = 0*/, Qt::WindowFlags f/* = 0*/)
: QWidget(parent, f)
{
    initUI();
}

CUserInfoWidget::~CUserInfoWidget()
{

}

void CUserInfoWidget::initUI()
{
    QLabel *labelName = new QLabel(tr("客户名称:"), this);
    QLabel *labelHousehold = new QLabel(tr("户号:"), this);
    QLabel *labelHouse = new QLabel(tr("门牌号:"), this);

    m_labelName.setParent(this);
    m_labelHouseholdNum.setParent(this);
    m_labelHouseNum.setParent(this);

    int x = 10, y = 10;
    labelName->setGeometry(x, y, 100, 40);
    x+=100;
    m_labelName.setGeometry(x, y, 120, 40);
    x+=120;

    labelHousehold->setGeometry(x, y, 60, 40);
    x+=60;
    m_labelHouseholdNum.setGeometry(x, y, 120, 40);
    x+=120;

    labelHouse->setGeometry(x, y, 80, 40);
    x+=80;
    m_labelHouseNum.setGeometry(x, y, 120, 40);
}

void CUserInfoWidget::updateUserInfo(const DM_UserInfo &userInfo)
{
    m_labelName.setText(userInfo.name);
    m_labelHouseholdNum.setText(userInfo.householdNum);
    m_labelHouseNum.setText(userInfo.houseNum);
}



CUserChargeRecordPage::CUserChargeRecordPage(QWidget* parent /*= 0*/, Qt::WindowFlags f /*= 0*/)
: CBaseContent(parent, f)
, m_userInfoReport(NULL)
, m_eReportType(REPORT_NONE)
, m_reportCharge(NULL)
, m_reportReply(NULL)
, m_reportPowerCut(NULL)
{
    m_dataMgr = CDataManager::getInstance();
    setContentCaption(tr("查询->购电记录"));
    initUI();
    updateMenuInfo();
}

CUserChargeRecordPage::~CUserChargeRecordPage()
{

}

void CUserChargeRecordPage::initUI()
{
    CBaseContent::initUI();
    initMenu();
    m_userInfoReport = new CUserInfoReport(this);
    m_userInfoReport->setGeometry(TABLE_L, TABLE_T, TABLE_W, TABLE_H);
    m_userInfoReport->setObjectName("tabelWidget_userInfo");
    m_userInfoReport->createTableWidget(CUserInfoReport::USER_INFO_ROW, 2);
    m_userInfoReport->updateCaptionColumn();
    updateUserInfo();

    m_userInfoWidget = new CUserInfoWidget(this);
    m_userInfoWidget->setGeometry(USER_INFO_L, USER_INFO_T, USER_INFO_W, USER_INFO_H);
    m_userInfoWidget->setHidden(true);
    DM_UserInfo userInfo;
    m_dataMgr->getUserInfo(userInfo);
    m_userInfoWidget->updateUserInfo(userInfo);

    m_labelCaption = new QLabel(this);
    m_labelCaption->setGeometry(CAPTION_L, CAPTION_T, CAPTION_W, CAPTION_H);
    m_labelCaption->setHidden(true);

    m_groupbox = new QGroupBox(this);
    m_groupbox->setGeometry(GROUP_BOX_L, GROUP_BOX_T, 520, GROUP_BOX_H);
    m_groupbox->setHidden(true);
}

bool CUserChargeRecordPage::OnF1Press()
{
    if (m_eReportType != REPORT_CHARGED) {
        m_eReportType = REPORT_CHARGED;
        showChargeRecord();
        setContentCaption(tr("查询->购电记录")+"->"+tr("充值记录"));
    }

    return true;
}

bool CUserChargeRecordPage::OnF2Press()
{
    if (m_eReportType != REPORT_REPLY) {
        m_eReportType = REPORT_REPLY;
        showReplyRecord();
        setContentCaption(tr("查询->购电记录")+"->"+tr("复电记录"));
    }

    return true;
}

bool CUserChargeRecordPage::OnF3Press()
{
    if (m_eReportType != REPORT_POWER_CUT) {
        m_eReportType = REPORT_POWER_CUT;
        showPowerCutRecord();
        setContentCaption(tr("查询->购电记录")+"->"+tr("欠费记录"));
    }

    return true;
}

void CUserChargeRecordPage::initMenu()
{
    setMenuCaption(BTN_RECORD_CHARGED, tr("充值记录"));
    setMenuCaption(BTN_RECORD_REPLY, tr("复电记录"));
    setMenuCaption(BTN_POWER_CUT, tr("欠费记录"));
    setMenuCaption(BTN_RETURN, tr("返回"));

    setMenuContentPage(BTN_RETURN, USER_QUERY_PAGE_ID);
}

void CUserChargeRecordPage::updateUserInfo()
{
    DM_UserInfo userInfo;
    m_dataMgr->getUserInfo(userInfo);
    m_userInfoReport->updateUserInfo(userInfo);
}


void CUserChargeRecordPage::showChargeRecord()
{
    m_userInfoReport->hide();
    m_userInfoWidget->show();
    m_labelCaption->setText(tr("充值记录")+tr("(近10条)"));
    m_labelCaption->show();
    m_groupbox->show();

    if (!m_reportCharge) {
        createCharegeRecord();
    }
    updateChargeRecord();
    m_reportCharge->show();

    if(m_reportReply) 
        m_reportReply->setHidden(true);
    if(m_reportPowerCut) 
        m_reportPowerCut->setHidden(true);
}

void CUserChargeRecordPage::showReplyRecord()
{
    m_userInfoReport->hide();
    m_userInfoWidget->show();
    m_labelCaption->setText(tr("复电记录")+tr("(近10条)"));
    m_labelCaption->show();
    m_groupbox->show();

    if (!m_reportReply) {
        createReplyRecord();
    }
    updateReplyRecord();
    m_reportReply->show();

    if(m_reportCharge) 
        m_reportCharge->setHidden(true);
    if(m_reportPowerCut) 
        m_reportPowerCut->setHidden(true);
}

void CUserChargeRecordPage::showPowerCutRecord()
{
    m_userInfoReport->hide();
    m_userInfoWidget->show();
    m_labelCaption->setText(tr("欠费记录")+tr("(近10条)"));
    m_labelCaption->show();
    m_groupbox->show();

    if (!m_reportPowerCut) {
        createPowerCutRecord();
    }
    updatePowderCutRecord();
    m_reportPowerCut->show();

    if(m_reportCharge) 
        m_reportCharge->setHidden(true);
    if(m_reportReply) 
        m_reportReply->setHidden(true);
}


void CUserChargeRecordPage::createCharegeRecord()
{
    const QString headerCaption[5] = {
        tr("序号"),
        tr("充值时间"),
        tr("充值金额"),
        tr("充值前余额"),
        tr("充值后余额")
    };

    m_reportCharge = new MTableWidget(REPORT_ROWS, 5, m_groupbox);
    m_reportCharge->setGeometry(10, 10, m_groupbox->width() - 20, m_groupbox->height() - 20);

    m_reportCharge->verticalHeader()->hide();
    m_reportCharge->horizontalScrollBar()->hide();
    m_reportCharge->verticalScrollBar()->hide();

    m_reportCharge->setShowGrid(false);
    m_reportCharge->setGridStyle(new MGridStyle(QColor(TABLE_LINE_CLR), true, true));
    m_reportCharge->setShowTopBottomLine(true, true);
    m_reportCharge->setShowLeftRightLine(true, true);
    m_reportCharge->setAlternatingRowColors(false);

    m_reportCharge->horizontalHeader()->setFixedHeight(REPORT_ITEM_H);

    for(int i = 0; i < m_reportCharge->columnCount(); i++) {
        QTableWidgetItem *headerItem = new QTableWidgetItem(headerCaption[i]);
        m_reportCharge->setHorizontalHeaderItem(i, headerItem);
        headerItem->setTextAlignment(Qt::AlignCenter);
        if(i != 0) {
            m_reportCharge->setColumnWidth(i, 110);
        }
        else {
            m_reportCharge->setColumnWidth(i, 59);
        }
    }

    for (int row = 0; row < m_reportCharge->rowCount(); ++row) {
        for (int column = 0; column < m_reportCharge->columnCount(); ++column) {
            QTableWidgetItem *newItem = new QTableWidgetItem();
            if (column == 0) {
                newItem->setFlags(Qt::ItemIsSelectable);
                newItem->setTextAlignment(Qt::AlignCenter);
            }
            else {
                newItem->setTextAlignment(Qt::AlignCenter);
            }
            m_reportCharge->setItem(row, column, newItem);
        }
        m_reportCharge->setRowHeight(row, REPORT_ITEM_H);
    }
}

void CUserChargeRecordPage::createReplyRecord()
{
    const QString headerCaption[5] = {
        tr("序号"),
        tr("复电时间")
    };

    m_reportReply = new MTableWidget(REPORT_ROWS, 2, m_groupbox);
    m_reportReply->setGeometry(100, 10, 280, m_groupbox->height()-20);

    m_reportReply->verticalHeader()->hide();
    m_reportReply->horizontalScrollBar()->hide();
    m_reportReply->verticalScrollBar()->hide();

    m_reportReply->setShowGrid(false);
    m_reportReply->setGridStyle(new MGridStyle(QColor(TABLE_LINE_CLR), true, true));
    m_reportReply->setShowTopBottomLine(true, true);
    m_reportReply->setShowLeftRightLine(true, true);
    m_reportReply->setAlternatingRowColors(false);

    m_reportReply->horizontalHeader()->setFixedHeight(REPORT_ITEM_H);

    for(int i = 0; i < m_reportReply->columnCount(); i++) {
        QTableWidgetItem *headerItem = new QTableWidgetItem(headerCaption[i]);
        m_reportReply->setHorizontalHeaderItem(i, headerItem);
        headerItem->setTextAlignment(Qt::AlignCenter);
        if(i != 0) {
            m_reportReply->setColumnWidth(i, 223);
        }
        else {
            m_reportReply->setColumnWidth(i, 56);
        }
    }

    for (int row = 0; row < m_reportReply->rowCount(); ++row) {
        for (int column = 0; column < m_reportReply->columnCount(); ++column) {
            QTableWidgetItem *newItem = new QTableWidgetItem();
            if (column == 0) {
                newItem->setFlags(Qt::ItemIsSelectable);
                newItem->setTextAlignment(Qt::AlignCenter);
            }
            else {
                newItem->setTextAlignment(Qt::AlignCenter);
            }
            m_reportReply->setItem(row, column, newItem);
        }
        m_reportReply->setRowHeight(row, REPORT_ITEM_H);
    }
}

void CUserChargeRecordPage::createPowerCutRecord()
{
    const QString headerCaption[5] = {
        tr("序号"),
        tr("停电时间"),
        tr("欠费金额")
    };

    m_reportPowerCut = new MTableWidget(REPORT_ROWS, 3, m_groupbox);
    m_reportPowerCut->setGeometry(44, 10, 400, m_groupbox->height()-20);

    m_reportPowerCut->verticalHeader()->hide();
    m_reportPowerCut->horizontalScrollBar()->hide();
    m_reportPowerCut->verticalScrollBar()->hide();

    m_reportPowerCut->setShowGrid(false);
    m_reportPowerCut->setGridStyle(new MGridStyle(QColor(TABLE_LINE_CLR), true, true));
    m_reportPowerCut->setShowTopBottomLine(true, true);
    m_reportPowerCut->setShowLeftRightLine(true, true);
    m_reportPowerCut->setAlternatingRowColors(false);

    m_reportPowerCut->horizontalHeader()->setFixedHeight(REPORT_ITEM_H);

    for(int i = 0; i < m_reportPowerCut->columnCount(); i++) {
        QTableWidgetItem *headerItem = new QTableWidgetItem(headerCaption[i]);
        m_reportPowerCut->setHorizontalHeaderItem(i, headerItem);
        headerItem->setTextAlignment(Qt::AlignCenter);
        if(i == 0) 
            m_reportPowerCut->setColumnWidth(i, 56);
        else if (i == 1) 
            m_reportPowerCut->setColumnWidth(i, 223);
        else 
            m_reportPowerCut->setColumnWidth(i, 120);
    }

    for (int row = 0; row < m_reportPowerCut->rowCount(); ++row) {
        for (int column = 0; column < m_reportPowerCut->columnCount(); ++column) {
            QTableWidgetItem *newItem = new QTableWidgetItem();
            if (column == 0) {
                newItem->setFlags(Qt::ItemIsSelectable);
                newItem->setTextAlignment(Qt::AlignCenter);
            }
            else {
                newItem->setTextAlignment(Qt::AlignCenter);
            }
            m_reportPowerCut->setItem(row, column, newItem);
        }
        m_reportPowerCut->setRowHeight(row, REPORT_ITEM_H);
    }
}

void CUserChargeRecordPage::clearChargeRecord()
{
    if (m_reportCharge)
    {
        for (int row = 0;row < m_reportCharge->rowCount();row++)
        {
            for(int column = 0; column < 5;column++)
            {
                m_reportCharge->item(row,column)->setText(" ");
            }
        }
    }
}


void CUserChargeRecordPage::clearReplyRecord()
{
    if (m_reportReply)
    {
        for (int row = 0;row < m_reportReply->rowCount();row++)
        {
            for(int column = 0; column < 2;column++)
            {
                m_reportReply->item(row,column)->setText(" ");
            }
        }
    }
}


void CUserChargeRecordPage::clearPowerCutRecord()
{
    if (m_reportPowerCut)
    {
        for (int row = 0;row < m_reportPowerCut->rowCount();row++)
        {
            for(int column = 0; column < 3;column++)
            {
                m_reportPowerCut->item(row,column)->setText(" ");
            }
        }
    }
}

void CUserChargeRecordPage::updateChargeRecord()
{
    QList<DM_UserChargeRecord> userChargeRecord;
    m_dataMgr->getUserChargeReocrd(userChargeRecord);

    clearChargeRecord();

    for(int row = 0;row < userChargeRecord.size();row++)
    {
        QString str = QString::number(row + 1);
        m_reportCharge->item(row,0)->setText(str);

        str = userChargeRecord[row].chargeTime;
        m_reportCharge->item(row,1)->setText(str);

        str = QString::number(userChargeRecord[row].money,'f',2);
        m_reportCharge->item(row,2)->setText(str);

        str = QString::number(userChargeRecord[row].moneyBeforCharge,'f',2);
        m_reportCharge->item(row,3)->setText(str);

        str = QString::number(userChargeRecord[row].moneyAfterCharge,'f',2);
        m_reportCharge->item(row,4)->setText(str);
    }
}

void CUserChargeRecordPage::updateReplyRecord()
{
    QList<DM_UserReplyRecord> userReplyRecord;
    m_dataMgr->getUserReplyRecord(userReplyRecord);

    clearReplyRecord();

    for(int row = 0;row < userReplyRecord.size();row++)
    {
        QString str = QString::number(row + 1);
        m_reportReply->item(row,0)->setText(str);

        str = userReplyRecord[row].replyTime;
        m_reportReply->item(row,1)->setText(str);
    }
}

void CUserChargeRecordPage::updatePowderCutRecord()
{
    QList<DM_UserPowerCutRecord> userPowerCutRecord;
    m_dataMgr->getUserPowerCutRecord(userPowerCutRecord);

    clearPowerCutRecord();

    for(int row = 0;row < userPowerCutRecord.size();row++)
    {
        QString str = QString::number(row + 1);
        m_reportPowerCut->item(row,0)->setText(str);

        str = userPowerCutRecord[row].powerCutTime;
        m_reportPowerCut->item(row,1)->setText(str);

        str = QString::number(userPowerCutRecord[row].moneyOfOwe,'f',2);
        m_reportPowerCut->item(row,2)->setText(str);
    }
}
