#include "managerSystemParamPage.h"

#include "src/main/common.h"
#include "src/common_ctrl/reportWidget.h"

#include "src/dataLevel/datamanager.h"

#include <QTime>

#define TABLE_L         160
#define TABLE_T         121
#define TABLE_W         288
#define TABLE_H         152
#define TABLE_ITEM_H    38
#define TABLE_CAPTION_W 200
#define TABLE_CONTENT_W 88

#define PER_BACKLIGHT   10
#define PER_LOOPTIME    5
#define PER_TIME        15

CManagerSystemParamPage::CManagerSystemParamPage(QWidget* parent, Qt::WindowFlags f)
: CBaseContent(parent,f)
, m_eSelectedType(BACKLIGHT)
{
    m_pDataMgr = CDataManager::getInstance();
    setContentCaption(tr("管理维护->参数设置->系统参数"));
    initUI();
    initMenu();
}

CManagerSystemParamPage::~CManagerSystemParamPage(void)
{

}

void CManagerSystemParamPage::initUI()
{
    CBaseContent::initUI();
    m_pSystemParamReport = new CSystemParamReport(this);
    m_pSystemParamReport->setGeometry(TABLE_L, TABLE_T, TABLE_W, TABLE_H);
    m_pSystemParamReport->setCommonRowHeight(TABLE_ITEM_H);
    m_pSystemParamReport->setCaptionWidth(TABLE_CAPTION_W);
    m_pSystemParamReport->setContentWidth(TABLE_CONTENT_W);
    m_pSystemParamReport->setObjectName("tabelWidget_systemParamInfo");
    m_pSystemParamReport->createTableWidget(CSystemParamReport::SYSTEM_PARAM_ROW, 2);
    m_pSystemParamReport->updateCaptionColumn();
    m_pSystemParamReport->item(0,1)->setSelected(true);
    updateSystemParamReport();
}

bool CManagerSystemParamPage::OnF1Press()
{
    switch(m_eSelectedType)
    {
    case BACKLIGHT:
        m_pSystemParamReport->item(0,1)->setSelected(false);
        m_pSystemParamReport->item(1,1)->setSelected(true);
        m_pSystemParamReport->item(2,1)->setSelected(false);
        m_pSystemParamReport->item(3,1)->setSelected(false);

        m_eSelectedType = LOOPTIME;
        break;
    case LOOPTIME:
        m_pSystemParamReport->item(0,1)->setSelected(false);
        m_pSystemParamReport->item(1,1)->setSelected(false);
        m_pSystemParamReport->item(2,1)->setSelected(true);
        m_pSystemParamReport->item(3,1)->setSelected(false);

        m_eSelectedType = BACK_LIGHT_START;
        break;
    case BACK_LIGHT_START:
        m_pSystemParamReport->item(0,1)->setSelected(false);
        m_pSystemParamReport->item(1,1)->setSelected(false);
        m_pSystemParamReport->item(2,1)->setSelected(false);
        m_pSystemParamReport->item(3,1)->setSelected(true);

        m_eSelectedType = BACK_LIGHT_END;
        break;
    case BACK_LIGHT_END:
        m_pSystemParamReport->item(0,1)->setSelected(true);
        m_pSystemParamReport->item(1,1)->setSelected(false);
        m_pSystemParamReport->item(2,1)->setSelected(false);
        m_pSystemParamReport->item(3,1)->setSelected(false);

        m_eSelectedType = BACKLIGHT;
        break;
    default:
        break;
    }

    return true;
}

bool CManagerSystemParamPage::OnF2Press()
{
    switch(m_eSelectedType)
    {
    case BACKLIGHT:
        {
            unsigned int backlight = m_pSystemParamReport->item(0,1)->text().remove("  %").toUInt();
            if (backlight >= 0 && backlight <= 100)
            {
                backlight = backlight - PER_BACKLIGHT;
                if (backlight <= 0)
                {
                    backlight = 0;
                }
                m_pDataMgr->setBacklight(backlight);
            }
            break;
        }
    case LOOPTIME:
        {
            unsigned int looptime = m_pSystemParamReport->item(1,1)->text().remove("  S").toUInt();
            if (looptime >= 0 && looptime <= 100)
            {
                looptime = looptime - PER_LOOPTIME;
                if (looptime <= 5)
                {
                    looptime = 5;
                }
                m_pDataMgr->setLoopTime(looptime);
            }
            break;
        }
    case BACK_LIGHT_START:
        {
            QTime time = QTime::fromString(m_pSystemParamReport->item(2,1)->text(),"hh:mm");
            if (time.isValid())
            {
                time = time.addSecs(-PER_TIME * 60);
            }
            m_pDataMgr->setBackLightStartTime(time.toString("hh:mm"));
            break;
        }
    case BACK_LIGHT_END:
        {
            QTime time = QTime::fromString(m_pSystemParamReport->item(3,1)->text(),"hh:mm");
            if (time.isValid())
            {
                time = time.addSecs(-PER_TIME * 60);
            }
            m_pDataMgr->setBackLightEndTime(time.toString("hh:mm"));
            break;
        }
    default:
        break;
    }

    updateSystemParamReport();
    return true;
}

bool CManagerSystemParamPage::OnF3Press()
{
    switch(m_eSelectedType)
    {
    case BACKLIGHT:
        {
            unsigned int backlight = m_pSystemParamReport->item(0,1)->text().remove("  %").toUInt();
            if (backlight >= 0 && backlight <= 100)
            {
                backlight = backlight + PER_BACKLIGHT;
                if (backlight >= 100)
                {
                    backlight = 100;
                }
                m_pDataMgr->setBacklight(backlight);
            }
            break;
        }
    case LOOPTIME:
        {
            unsigned int looptime = m_pSystemParamReport->item(1,1)->text().remove("  S").toUInt();
            if (looptime >= 0 && looptime <= 100)
            {
                looptime = looptime + PER_LOOPTIME;
                if (looptime >= 60)
                {
                    looptime = 60;
                }
                m_pDataMgr->setLoopTime(looptime);
            }
            break;
        }
    case BACK_LIGHT_START:
        {
            QTime time = QTime::fromString(m_pSystemParamReport->item(2,1)->text(),"hh:mm");
            if (time.isValid())
            {
                time = time.addSecs(PER_TIME * 60);
            }
            m_pDataMgr->setBackLightStartTime(time.toString("hh:mm"));
            break;
        }
    case BACK_LIGHT_END:
        {
            QTime time = QTime::fromString(m_pSystemParamReport->item(3,1)->text(),"hh:mm");
            if (time.isValid())
            {
                time = time.addSecs(PER_TIME * 60);
            }
            m_pDataMgr->setBackLightEndTime(time.toString("hh:mm"));
            break;
        }
    default:
        break;
    }

    updateSystemParamReport();
    return true;
}

void CManagerSystemParamPage::initMenu()
{
    setMenuCaption(BTN_SWITCH, tr("切换"));
    setMenuCaption(BTN_REDUCE, tr("减小"));
    setMenuCaption(BTN_INCREACE, tr("增大"));
    setMenuCaption(BTN_BACK, tr("返回"));

    setMenuContentPage(BTN_BACK,MANAGER_PARAM_SETTING_ID);
}

void CManagerSystemParamPage::updateSystemParamReport()
{
    m_pSystemParamReport->item(0,1)->setText(QString("%1  %").
                                         arg(QString::number(m_pDataMgr->getBacklight())));

    m_pSystemParamReport->item(1,1)->setText(QString("%1  S").
                                         arg(QString::number(m_pDataMgr->getLoopTime())));

    m_pSystemParamReport->item(2,1)->setText(m_pDataMgr->getBackLightStartTime());

    m_pSystemParamReport->item(3,1)->setText(m_pDataMgr->getBackLightEndTime());
}