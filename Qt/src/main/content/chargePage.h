#pragma once

#include "inc/baseContent.h"

class CUserChargeReport;
class CDataManager;
class CInputMoneyWidget;
class QLabel;
class CChargePage : public CBaseContent {
public:
    CChargePage(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~CChargePage();

    enum {
        BTN_EDIT,
        BTN_CHARGE,
        BTN_TEMP,
        BTN_RETURN = 3
    };

    enum REPORT_TYPE {
        REPORT_NONE = -1,
        REPORT_CHARGE,
        REPORT_EDIT
    };

    enum{
        BTN_SWITCH,
        BTN_SELECT,
        BTN_SET,
        BTN_CANCEL = 3
    };

    enum{
        BTN_SURE,
        BTN_NONE1,
        BTN_NONE2,
        BTN_CAL = 3
    };
    // ��ֵ���
    enum CHARGE_RESULT{
        VALUE_NONE = -1,
        VALUE_AVALIABLE,
        VALUE_UNAVALIABLE,
        CHARGE_SUCCESS,
        CHARGE_FALSE
    };
    // ��ǰ��ʾҳ������
    enum CHARGE_WIDGET_TYPE {
        REPORT_WIDGET_TYPE,
        INPUT_WIDGET_TYPE,
        AFFIRM_WIDGET_TYPE
    };
    // ȷ�ϰ�ť��Դ
    enum AFFIRM_FROM {
        FROM_REPORT_WIDGET,
        FROM_INPUT_WIDGET
    };

    virtual void initUI();

protected:
    /*
     *  @Func:  OnF1Press ~ OnF4Press
     *  @Desc:  ��Ӧ��Ӧ�˵����µ��¼�
     *  
     *  @Param[in]:
     *      NONE
     *  
     *  @Return:
     *      true:   �˵������ɣ���ʹ�����˲˵���Ӧ���л�ҳ��Ҳ������ʾҳ��
     *      false:  �����˲˵���Ӧ���л�ҳ�潫����ʾ��ҳ��
     */
    virtual bool OnF1Press();
    virtual bool OnF2Press();
    virtual bool OnF3Press();
    virtual bool OnF4Press();

private:
    bool chargeResult();                                                // ��ֵ��� trueΪ�ɹ� falseΪʧ��

    void showReportWidget();                                            // ��ʾ��ֵ���ҳ����Ϣ
    void showInputWidget();                                             // ��ʾ������ҳ����Ϣ
    void showAffirmWidget();                                            // ��ʾȷ�Ϻ���ʾҳ����Ϣ

    void createReportWidget();
    void createInputWidget();
    void createAffirmWidget();

    void updateReportMenu();
    void updateInputMenu();
    void updateAffirmMenu();


private:
    CUserChargeReport       *m_userChargeReport;
    CDataManager            *m_dataMgr;
    CInputMoneyWidget       *m_pInputMoneyWidget;
    QLabel                  *m_pMessageLabel;                           // ������ʾ��ȷ����Ϣ��Label

    REPORT_TYPE             m_eReportType;
    CHARGE_WIDGET_TYPE      m_eWidgetType;
    CHARGE_RESULT           m_eChargeResult;
    AFFIRM_FROM             m_eAffirmFrom;
};