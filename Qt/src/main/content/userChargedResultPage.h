#pragma once

#include "inc/baseContent.h"

class CUserChargeReport;
class CDataManager;
class CChargePage : public CBaseContent {
public:
    CChargePage(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~CChargePage();

    enum {
        BTN_CHARGE,
        BTN_EDIT,

        BTN_RETURN = 3
    };

    enum REPORT_TYPE {
        REPORT_NONE = -1,
        REPORT_CHARGE,
        REPORT_EDIT
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

private:
    void initMenu();

private:
    CUserChargeReport* m_userChargeReport;
    CDataManager *m_dataMgr;
};