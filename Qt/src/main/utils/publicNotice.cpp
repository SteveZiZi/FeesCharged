#include "publicNotice.h"

#include "src/main/common.h"
#include "inc/BottomBarMgr.h"
#include "inc/xtDebug.h"
#include "src/common_ctrl/reportWidget.h"
#include "src/dataLevel/datamanager.h"

#include <QEvent>
#include <QLabel>
#include <QDir>
#include <QTimer>

#define TOPBAR_L        0
#define TOPBAR_T        0
#define TOPBAR_W        800
#define TOPBAR_H        67

#define TITLE_L         0
#define TITLE_T         68
#define TITLE_W         800
#define TITLE_H         38

#define CONNECT_L       0
#define CONNECT_T       105
#define CONNECT_W       800
#define CONNECT_H       336

#define TABLE_L         248
#define TABLE_T         125
#define TABLE_W         305
#define TABLE_H         281

#define IMAGE_PATH      ".//images//"
#define UPDATE_TIME     3000
#define TEMP_UPDATE_TIME 60000

static QList<DM_UserNoticeConnect> s_noticeConnectList;
static QList<DM_UserNoticeInfoS> s_noticeReportInfoList;

CPublicNotice::CPublicNotice(QWidget *parent, Qt::WFlags flags)
    : QDialog(parent, flags)
    , m_noticeType(NONE_TYPE)
    , m_noticeIndex(0)
{
    m_dataMgr = CDataManager::getInstance();
    initUI();

    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(onTimerOut()));
}

CPublicNotice::~CPublicNotice()
{

}

void CPublicNotice::initUI()
{
    setWindowFlags(Qt::FramelessWindowHint);
    this->resize(SCREEN_SIZE_W, SCREEN_SIZE_H);
    this->setObjectName("PublicNotice");

    m_pTimer = new QTimer(this);

    createNotice();

    createNoticeReport();
}

void CPublicNotice::createNotice()
{
    m_topBar = new QLabel(this);
    m_topBar->setGeometry(TOPBAR_L, TOPBAR_T, TOPBAR_W, TOPBAR_H);
    m_topBar->setObjectName("topBar");
    //m_topBar->setPixmap(QPixmap(":/images/topTitle.png"));

    m_titleLabel = new QLabel(this);
    m_titleLabel->setGeometry(TITLE_L,TITLE_T,TITLE_W,TITLE_H);
    m_titleLabel->setObjectName("titleLabel");
    m_titleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    m_contentLabel = new QLabel(this);
    m_contentLabel->setObjectName("contentLabel");
    m_contentLabel->setGeometry(CONNECT_L,CONNECT_T,CONNECT_W,CONNECT_H);
    m_contentLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    m_bottomBar = new CBottomBarMgr(this);
    m_bottomBar->showStatus(tr("运行状态"), -1);
}

bool CPublicNotice::getNoticeConnects()
{
    bool rc = false;

    s_noticeConnectList.clear();
    m_dataMgr->getUserNoticeConnect(s_noticeConnectList);

    if (s_noticeConnectList.size())
        rc = true;

    return rc;
}

void CPublicNotice::showNotice(int index)
{
    Q_ASSERT(index >=0 && index < s_noticeConnectList.size());

    if (!m_topBar->isVisible())
        m_topBar->show();
    if (!m_titleLabel->isVisible())
        m_titleLabel->show();
    if (!m_contentLabel->isVisible())
        m_contentLabel->show();
    if (!m_bottomBar->isVisible())
        m_bottomBar->show();


    m_titleLabel->setText(s_noticeConnectList[index].title);
    m_contentLabel->setText(QString(tr("开 始 时 间 : %1\n\n\n结 束 时 间 : %2\n\n\n内 容 : %3"))
                            .arg(s_noticeConnectList[index].beginTime)
                            .arg(s_noticeConnectList[index].endTime)
                            .arg(s_noticeConnectList[index].connect));
}

void CPublicNotice::clearNotice()
{
    if (m_topBar->isVisible())
        m_topBar->hide();
    if (m_titleLabel->isVisible())
        m_titleLabel->hide();
    if (m_contentLabel->isVisible())
        m_contentLabel->hide();
    if (m_bottomBar->isVisible())
        m_bottomBar->hide();
}

void CPublicNotice::createNoticeReport()
{
    m_pUserNoticeReport = new CUserNoticeReport(this);
    m_pUserNoticeReport->setGeometry(TABLE_L, TABLE_T, TABLE_W, TABLE_H);
    m_pUserNoticeReport->setObjectName("tabelWidget_userNoticeReport");
    m_pUserNoticeReport->createTableWidget(CUserQueryResultReport::QUERY_RESULT_ROW + CUserNoticeReport::USER_NOTICE_INFO_ROW, 2);
    m_pUserNoticeReport->updateCaptionColumn();
}

bool CPublicNotice::getNoticeReportInfos()
{
    bool rc = false;

    s_noticeReportInfoList.clear();
    m_dataMgr->getUserNoticeInfos(s_noticeReportInfoList);

    if (s_noticeReportInfoList.size())
        rc = true;

    return rc;
}

void CPublicNotice::showNoticeReport(int index)
{
    Q_ASSERT(index >=0 && index < s_noticeReportInfoList.size());

    if (!m_pUserNoticeReport->isVisible())
        m_pUserNoticeReport->show();
    if (!m_topBar->isVisible())
        m_topBar->show();
    if (!m_titleLabel->isVisible()){
        m_titleLabel->show();
        m_titleLabel->setText(tr("用 户 信 息"));
    }
    if (!m_bottomBar->isVisible())
        m_bottomBar->show();

    m_pUserNoticeReport->updateUserInfo(s_noticeReportInfoList[index].userInfo);

    m_pUserNoticeReport->updateUserResultInfo(s_noticeReportInfoList[index].curElec);

    m_pUserNoticeReport->updateUserNoticeInfo(s_noticeReportInfoList[index].userNoticeInfo);
}

void CPublicNotice::clearNoticeReport()
{
    if (m_pUserNoticeReport->isVisible())
        m_pUserNoticeReport->hide();
}

bool CPublicNotice::getImagesResource()
{
    bool rc = false;

    m_imageInfoList.clear();
    QDir dir(IMAGE_PATH);
    QFileInfoList entryInfo;
    entryInfo = dir.entryInfoList();
    for(int i = 0;i < entryInfo.size();i++)
    {
        if (entryInfo[i].isFile()){
            if (entryInfo[i].suffix().contains("png",Qt::CaseInsensitive) ||
                entryInfo[i].suffix().contains("jpg",Qt::CaseInsensitive) ||
                entryInfo[i].suffix().contains("bmp",Qt::CaseInsensitive)){
                m_imageInfoList.append(entryInfo[i]);
            }
        }
    }

    if (m_imageInfoList.size())
        rc = true;

    return rc;
}

void CPublicNotice::showImage(int index)
{
    Q_ASSERT(index >=0 && index < m_imageInfoList.size());

    QString style("background-image: url(");
    style += IMAGE_PATH;
    style += m_imageInfoList[index].fileName();
    style += ");";
    this->setStyleSheet(style);
//     QPalette pal = this->palette();
//     pal.setBrush(QPixmap(IMAGE_PATH + m_imageInfoList[i].fileName()));
//     this->setPalette(pal);
}

void CPublicNotice::clearImage()
{
    QString style("background-image:none;");
    this->setStyleSheet(style);
}

void CPublicNotice::getNoticeResource()
{
    if (getNoticeConnects())
        m_noticeType = NOTCIE_CONNECT_TYPE;
    if (getNoticeReportInfos())
        m_noticeType = USER_INFO_TYPE;
    if (getImagesResource())
        m_noticeType = IMAGE_TYPE;
    else
        m_noticeType = NONE_TYPE;
}

void CPublicNotice::shouDefaultStyle()
{
    if (!m_topBar->isVisible())
        m_topBar->show();
    if (!m_titleLabel->isVisible()){
        m_titleLabel->show();
        m_titleLabel->setText(tr("公 告 信 息"));
    }
    if (!m_contentLabel->isVisible()){
        m_contentLabel->show();
        m_contentLabel->setText(tr("无 公 告 内 容"));
    }
    if (!m_bottomBar->isVisible())
        m_bottomBar->show();
}

CPublicNotice* CPublicNotice::getInstance()
{
    static CPublicNotice publicNotice;

    return &publicNotice;
}


bool CPublicNotice::event ( QEvent * event )
{
    QEvent::Type type = event->type();
    if (type == QEvent::MouseButtonPress ||
        type == QEvent::KeyPress ||
        type == QEvent::MouseMove)
    {
        m_pTimer->start(TEMP_UPDATE_TIME);
    }
    else {
        return QDialog::event(event);
    }

    return true;
}

void CPublicNotice::onTimerOut()
{
    m_pTimer->start(UPDATE_TIME);
    switch(m_noticeType)
    {
    case IMAGE_TYPE:
        clearNotice();
        clearNoticeReport();
        showImage(m_noticeIndex);
        if (m_noticeIndex == m_imageInfoList.size() - 1){
            if (s_noticeConnectList.size())
                m_noticeType = NOTCIE_CONNECT_TYPE;
            else if (s_noticeReportInfoList.size())
                m_noticeType = USER_INFO_TYPE;
            else if (m_imageInfoList.size())
                m_noticeType = IMAGE_TYPE;
            else 
                m_noticeType = NONE_TYPE;
            m_noticeIndex = -1;
        }
        break;
    case NOTCIE_CONNECT_TYPE:
        clearImage();
        clearNoticeReport();
        showNotice(m_noticeIndex);
        if (m_noticeIndex == s_noticeConnectList.size() - 1){
            if (s_noticeReportInfoList.size())
                m_noticeType = USER_INFO_TYPE;
            else if (m_imageInfoList.size())
                m_noticeType = IMAGE_TYPE;
            else if (s_noticeConnectList.size())
                m_noticeType = NOTCIE_CONNECT_TYPE;
            else
                m_noticeType = NONE_TYPE;
            m_noticeIndex = -1;
        }
        break;
    case USER_INFO_TYPE:
        clearImage();
        clearNotice();
        showNoticeReport(m_noticeIndex);
        if (m_noticeIndex == s_noticeReportInfoList.size() - 1){
            if (m_imageInfoList.size())
                m_noticeType = IMAGE_TYPE;
            else if (s_noticeConnectList.size())
                m_noticeType = NOTCIE_CONNECT_TYPE;
            else if (s_noticeReportInfoList.size())
                m_noticeType = USER_INFO_TYPE;
            else
                m_noticeType = NONE_TYPE;
            m_noticeIndex = -1;
        }
        break;
    case NONE_TYPE:
        clearImage();
        clearNotice();
        clearNoticeReport();
        shouDefaultStyle();
        if (m_imageInfoList.size())
            m_noticeType = IMAGE_TYPE;
        else if (s_noticeConnectList.size())
            m_noticeType = NOTCIE_CONNECT_TYPE;
        else if (s_noticeReportInfoList.size())
            m_noticeType = USER_INFO_TYPE;
        else
            m_noticeType = NONE_TYPE;
        m_noticeIndex = -1;
        break;
    default:
        break;
    }
    m_noticeIndex++;
}

void CPublicNotice::splash()
{
    if (!this->isVisible()) {
        this->show();
        getNoticeResource();                                            // 更新公告内容
        onTimerOut();
    }
}

void CPublicNotice::stopSplash()
{
    m_pTimer->stop();
    this->hide();
    m_noticeIndex = 0;
}