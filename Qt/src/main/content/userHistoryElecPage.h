#pragma once

#include "inc/baseContent.h"

#include "inc/control/diagramCtrl/diagramCommon.h"
#include "inc/control/diagramCtrl/diagramCtrl.h"
#include "inc/control/diagramCtrl/Scale.h"
#include "inc/control/diagramCtrl/Histogram.h"

class CUserInfoReport;
class CDataManager;
class CUserHistoryElecPage : public CBaseContent {
public:
    CUserHistoryElecPage(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~CUserHistoryElecPage();

    enum {
        BTN_ONE_WEEK,
        BTN_HALF_YEAR,
        BTN_ONE_MONTH,
        BTN_RETURN
    };

    enum REPORT_TYPE {
        REPORT_NONE = -1,
        REPORT_ONE_WEEK,
        REPORT_ONE_MONTH,
        REPORT_HALF_YEAR
    };
    virtual void initUI();

protected:
    /*
     *  @Func:  OnF1Press ~ OnF4Press
     *  @Desc:  响应对应菜单按下的事件
     *  
     *  @Param[in]:
     *      NONE
     *  
     *  @Return:
     *      true:   菜单项处理完成，即使设置了菜单对应的切换页面也不会显示页面
     *      false:  设置了菜单对应的切换页面将会显示该页面
     */
    virtual bool OnF1Press();
    virtual bool OnF2Press();
    virtual bool OnF3Press();

private:
    virtual void setMenu();
    void updateUserInfo();
    void userInfoWidgetMove();

    void createHistogram(QRect &rect);
    void destoryHistogram();
    void createDescWidget();
    void destoryDescWidget();

private:
    CUserInfoReport* m_userInfoWidget;
    CDataManager *m_dataMgr;

    MDIAGRAM::CDiagramCtrl* m_diagram;
    MDIAGRAM::ScaleInfo   m_tScaleInfo;
    MDIAGRAM::CHistogram *m_histogram;

    MDIAGRAM::CDiagramCtrl* m_descCtrl;

    REPORT_TYPE m_eReportType;
};