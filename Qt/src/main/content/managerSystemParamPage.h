/*
 *  @bref:
 *      ϵͳ��������
 */
#pragma once

#include "inc/baseContent.h"

class CSystemParamReport;
class CDataManager;
class CManagerSystemParamPage : public CBaseContent
{
public:
    CManagerSystemParamPage(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~CManagerSystemParamPage(void);

    enum {
        BTN_SWITCH,
        BTN_REDUCE,
        BTN_INCREACE,
        BTN_BACK = 3
    };

    enum SELECTED_TYPE{
        BACKLIGHT,
        LOOPTIME,
        BACK_LIGHT_START,
        BACK_LIGHT_END
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
    void updateSystemParamReport();

private:
    CSystemParamReport*         m_pSystemParamReport;
    CDataManager*               m_pDataMgr;

    SELECTED_TYPE               m_eSelectedType;
};
