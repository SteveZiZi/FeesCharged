/*
 *  @bref:
 *      ϵͳά������
 */
#pragma once

#include "inc/baseContent.h"

class QLabel;
class CManagerSystemMaintenancePage : public CBaseContent
{
public:
    CManagerSystemMaintenancePage(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~CManagerSystemMaintenancePage(void);
    enum {
        BTN_VERIFY,
        BTN_CANCEL,
        BTN_TEMP,
        BTN_BACK = 3
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

private:
    void initMenu();

private:
    QLabel*                 m_pMessageLabel;
};
