#include "userHistoryElecPage.h"
#include <QDate>

#include "inc/control/mTablewidget.h"
#include "inc/control/mPushButton.h"
#include "src/main/common.h"
#include "src/common_ctrl/reportWidget.h"

#include "src/dataLevel/datamanager.h"

#include "inc/control/diagramCtrl/BackGround.h"
#include "inc/control/diagramCtrl/Foreground.h"
#include "inc/control/diagramCtrl/Trend.h"
#include "inc/control/diagramCtrl/Description.h"

#include "rectDecorate.h"


#define TABLE_L         192
#define TABLE_T         144
#define TABLE_W         305
#define TABLE_H         106

#define REPORT_L2           20
#define REPORT_T2           20
#define REPORT_W2           221
#define REPORT_H2           76
#define REPORT_ITEM_H2      25
#define REPORT_CAPTION_W2   100
#define REPORT_CONTENT_W2   120
#define REPORT_FONT_SIZE    11

#define WEEK_HISTOGRAM_L   (REPORT_L2+40)
#define WEEK_HISTOGRAM_T   (REPORT_T2+REPORT_H2+20)
#define WEEK_HISTOGRAM_W    400
#define WEEK_HISTOGRAM_H    260

#define MONTH_HISTOGRAM_L  (REPORT_L2+100)
#define MONTH_HISTOGRAM_T  (REPORT_T2+REPORT_H2+20)
#define MONTH_HISTOGRAM_W   250
#define MONTH_HISTOGRAM_H   260

#define YEAR_HISTOGRAM_L   (REPORT_L2+60)
#define YEAR_HISTOGRAM_T   (REPORT_T2+REPORT_H2+20)
#define YEAR_HISTOGRAM_W    350
#define YEAR_HISTOGRAM_H    260


#define DESC_HISTOGRAM_L   (WEEK_HISTOGRAM_L+WEEK_HISTOGRAM_W+20)
#define DESC_HISTOGRAM_T   (WEEK_HISTOGRAM_T)
#define DESC_HISTOGRAM_W    100
#define DESC_ITEM_H         26


using namespace MDIAGRAM;

static QRgb PillarClr_OneWeek[7] = {
    qRgb(255, 0, 0),
    qRgb(0, 255, 0),
    qRgb(255, 255, 0),
    qRgb(0, 255, 255),
    qRgb(255, 0, 255),
    qRgb(128, 128, 0),
    qRgb(255, 128, 128)
};

union HistoryElectro {
    struct {
        float enery[7];
    };
    DM_LastestWeekElectro lastestWeekElec;
    DM_LastestMonthElectro lastestMonthElec;
    DM_LastestHalfYearElectro lastestHalfElec;

    HistoryElectro() {
        memset(enery, 0, sizeof(enery));
    }
};

CUserHistoryElecPage::CUserHistoryElecPage(QWidget* parent /*= 0*/, Qt::WindowFlags f /*= 0*/)
: CBaseContent(parent, f)
, m_diagram(NULL)
, m_histogram(NULL)
, m_eReportType(REPORT_NONE)
, m_descCtrl(NULL)
{
    m_dataMgr = CDataManager::getInstance();
    setContentCaption(tr("查询->历史用电"));
    initUI();
    updateMenuInfo();
}

CUserHistoryElecPage::~CUserHistoryElecPage()
{

}

void CUserHistoryElecPage::initUI()
{
    CBaseContent::initUI();
    setMenu();
    m_userInfoWidget = new CUserInfoReport(this);
    m_userInfoWidget->setGeometry(TABLE_L, TABLE_T, TABLE_W, TABLE_H);
    m_userInfoWidget->setObjectName("tabelWidget_userInfo");
    m_userInfoWidget->createTableWidget(CUserInfoReport::USER_INFO_ROW, 2);
    m_userInfoWidget->updateCaptionColumn();
    updateUserInfo();
}

bool CUserHistoryElecPage::OnF1Press()
{
    if (m_eReportType != REPORT_ONE_WEEK) {
        m_eReportType = REPORT_ONE_WEEK;
        userInfoWidgetMove();
        QRect rect(WEEK_HISTOGRAM_L, WEEK_HISTOGRAM_T, WEEK_HISTOGRAM_W, WEEK_HISTOGRAM_H);
        createHistogram(rect);
        createDescWidget();
        setContentCaption(tr("查询->历史用电")+"->"+tr("最近七天"));
    }

    return true;
}

bool CUserHistoryElecPage::OnF2Press()
{
    if (m_eReportType != REPORT_HALF_YEAR) {
        m_eReportType = REPORT_HALF_YEAR;
        userInfoWidgetMove();
        QRect rect(YEAR_HISTOGRAM_L, YEAR_HISTOGRAM_T, YEAR_HISTOGRAM_W, YEAR_HISTOGRAM_H);
        createHistogram(rect);
        createDescWidget();
        setContentCaption(tr("查询->历史用电")+"->"+tr("最近六月"));
    }
    return true;
}

bool CUserHistoryElecPage::OnF3Press()
{
#if 0
    if (m_eReportType != REPORT_ONE_MONTH) {
        m_eReportType = REPORT_ONE_MONTH;
        userInfoWidgetMove();
        QRect rect(MONTH_HISTOGRAM_L, MONTH_HISTOGRAM_T, MONTH_HISTOGRAM_W, MONTH_HISTOGRAM_H);
        createHistogram(rect);
        createDescWidget();
        setContentCaption(tr("查询->历史用电")+"->"+tr("最近30天"));
    }
#endif

    return true;
}

void CUserHistoryElecPage::setMenu()
{
    setMenuCaption(BTN_ONE_WEEK, tr("最近七天"));
    setMenuCaption(BTN_HALF_YEAR, tr("最近六月"));
    setMenuCaption(BTN_ONE_MONTH, tr(""));
    setMenuCaption(BTN_RETURN, tr("返回"));

    setMenuContentPage(BTN_RETURN, USER_QUERY_PAGE_ID);
}

void CUserHistoryElecPage::updateUserInfo()
{
    DM_UserInfo userInfo;
    m_dataMgr->getUserInfo(userInfo);
    m_userInfoWidget->updateUserInfo(userInfo);
}


void CUserHistoryElecPage::userInfoWidgetMove()
{
    m_userInfoWidget->setGeometry(REPORT_L2, REPORT_T2, REPORT_W2, REPORT_H2);

    m_userInfoWidget->setColumnWidth(0, REPORT_CAPTION_W2);
    m_userInfoWidget->setColumnWidth(1, REPORT_CONTENT_W2);
    for(int row = 0; row < m_userInfoWidget->rowCount(); row++) {
        m_userInfoWidget->setRowHeight(row, REPORT_ITEM_H2);
    }

    const QFont &font = m_userInfoWidget->font();
    m_userInfoWidget->setFont(QFont(font.family(), REPORT_FONT_SIZE));
}


void CUserHistoryElecPage::createHistogram(QRect &rect)
{
    destoryHistogram();

    HistoryElectro historyElec;
    int cnt = 0;
    int axisWidth = 0;
    if (m_eReportType == REPORT_ONE_WEEK) {
        m_dataMgr->getLastWeekElectro(historyElec.lastestWeekElec);
        cnt = 7;
        axisWidth = WEEK_HISTOGRAM_W;
    }
    else if (REPORT_ONE_MONTH == m_eReportType) {
        m_dataMgr->getLastMonthElectro(historyElec.lastestMonthElec);
        cnt = 4;
        axisWidth = MONTH_HISTOGRAM_W;
    }
    else {
        m_dataMgr->getLastHalfYearElectro(historyElec.lastestHalfElec);
        cnt = 6;
        axisWidth = YEAR_HISTOGRAM_W;
    }
    float fMaxVal = historyElec.enery[0];
    for(int i = 1; i < cnt; i++) {
        fMaxVal = qMax<float>(fMaxVal, historyElec.enery[i]);
    }

    m_diagram = new MDIAGRAM::CDiagramCtrl(NULL, NULL, this);
    m_diagram->setGeometry(rect);

    CBackGround* pBackGround = new CBackGround(BACKGROUND_ID);

    m_tScaleInfo.fScaleX = 1.0;
    m_tScaleInfo.gridCountX = cnt;
    m_tScaleInfo.intervalX = 1;
    m_tScaleInfo.divIntervalX = m_tScaleInfo.intervalX;
    m_tScaleInfo.fScaleY = 1;
    m_tScaleInfo.gridCountY = fMaxVal+1;
    m_tScaleInfo.intervalY = m_tScaleInfo.gridCountY/8;
    if (m_tScaleInfo.intervalY <= 0)
    {
        m_tScaleInfo.intervalY = 100;
    }
    m_tScaleInfo.divIntervalY = m_tScaleInfo.intervalY;
    m_tScaleInfo.magin.setMargin(40, 10, 20, 10);
    m_tScaleInfo.rectClient = m_diagram->rect();
    m_tScaleInfo.axisDirY = POSITIVE;
    m_tScaleInfo.axisDirX = POSITIVE;

    CScale* pScale = new CScale(SCALE_ID, pBackGround);
    pScale->AttachScale(&m_tScaleInfo);
    pScale->setScaleValPos(40, 10);
    pScale->SetShowScaleLine(false, true);
    pScale->SetShowDivScaleLine(false, false);
    pScale->SetShowTitle(false, false);
    pScale->setShowArrow(false, false);
    pScale->SetShowScaleVal(false, true);


    CForeground* pForeground = new CForeground(FOREGROUND_ID);

    m_histogram = new MDIAGRAM::CHistogram(TREND_ID, pForeground);
    m_histogram->AttachScale(&m_tScaleInfo);

    PillarItemInfo pillarInfo;
    pillarInfo.width = axisWidth/m_tScaleInfo.gridCountX/3;
    pillarInfo.unit = "";

    for(int i = 0; i < cnt; i++) {
        if (historyElec.enery[i] <= 0)
        {
            continue;
        }
        pillarInfo.height = historyElec.enery[i];
        pillarInfo.graphClr = PillarClr_OneWeek[i];
        pillarInfo.textClr = PillarClr_OneWeek[i];
        m_histogram->setPillarItem(i+1, pillarInfo);
    }

    m_diagram->SetComponent(pScale, m_histogram);
    m_diagram->Init();
    m_diagram->DrawForegound();
    m_diagram->show();
}

void CUserHistoryElecPage::destoryHistogram()
{
    if (m_diagram)
        m_diagram->deleteLater();
}


void CUserHistoryElecPage::createDescWidget()
{
    destoryDescWidget();

    m_descCtrl = new MDIAGRAM::CDiagramCtrl(NULL, NULL, this);
    int cnt = 0;
    if (m_eReportType == REPORT_ONE_WEEK) {
        cnt = 7;
    }
    else if (REPORT_ONE_MONTH == m_eReportType) {
        cnt = 4;
    }
    else {
        cnt = 6;
    }

    int h = DESC_ITEM_H*cnt + 10;
    QRect rect = m_diagram->geometry();
    rect.adjust(rect.width()+10, rect.height()-h, DESC_HISTOGRAM_W, 0);
    m_descCtrl->setGeometry(rect);

    CBackGround *pBackGround = new CBackGround(BACKGROUND_ID);
    CRectDecorator* rectDecorator = new CRectDecorator(USER_ID, pBackGround);
    rectDecorator->setGeometry(m_descCtrl->rect());
    rectDecorator->setBackgroundColor(qRgb(0x99, 0xCC, 0x00));

    CComponent* dateComponent = dynamic_cast<CComponent*>(rectDecorator);
    if (REPORT_ONE_WEEK == m_eReportType) {
        QDate date = QDate::currentDate();
        for(int i = 0; i < 7; i++) {
            CDescription* dateDesc = new CDescription(DESCRIPTION_ID, dateComponent);
            dateDesc->SetDescription(date.toString("M月d日"));
            dateDesc->SetDescriptionPos(QPoint(10, 8+DESC_ITEM_H*i));
            dateDesc->SetLabelColor(PillarClr_OneWeek[i]);
            dateDesc->SetDescriptionColor(qRgb(255, 255, 255));
            date = date.addDays(-1);
            dateComponent = dynamic_cast<CComponent*>(dateDesc);
        }
    }
    else if (REPORT_ONE_MONTH == m_eReportType) {
        QString desc[4] = {tr("本周"), tr("上1周"), tr("上2周"), tr("上3周")};
        for(int i = 0; i < 4; i++) {
            CDescription* dateDesc = new CDescription(DESCRIPTION_ID, dateComponent);
            dateDesc->SetDescription(desc[i]);
            dateDesc->SetDescriptionPos(QPoint(10, 8+DESC_ITEM_H*i));
            dateDesc->SetLabelColor(PillarClr_OneWeek[i]);
            dateDesc->SetDescriptionColor(qRgb(255, 255, 255));
            dateComponent = dynamic_cast<CComponent*>(dateDesc);
        }
    }
    else {
        QString desc[6] = {tr("本月"), tr("上1月"), tr("上2月"), tr("上3月"), tr("上4月"), tr("上5月")};
        for(int i = 0; i < 6; i++) {
            CDescription* dateDesc = new CDescription(DESCRIPTION_ID, dateComponent);
            dateDesc->SetDescription(desc[i]);
            dateDesc->SetDescriptionPos(QPoint(10, 8+DESC_ITEM_H*i));
            dateDesc->SetLabelColor(PillarClr_OneWeek[i]);
            dateDesc->SetDescriptionColor(qRgb(255, 255, 255));
            dateComponent = dynamic_cast<CComponent*>(dateDesc);
        }
    }

    m_descCtrl->SetComponent(dateComponent, NULL);
    m_descCtrl->Init();
    m_descCtrl->show();
}

void CUserHistoryElecPage::destoryDescWidget()
{
    if (m_descCtrl)
        m_descCtrl->deleteLater();
}