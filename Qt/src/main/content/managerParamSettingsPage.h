/*
 *  @bref:
 *      �������ý��� + ����ͨ��
 */
#pragma once

#include "inc/baseContent.h"

class CTerminalInfoReport;
class CDataManager;
class MTableWidget;
class CManagerParamSettingsPage : public CBaseContent
{
public:
    CManagerParamSettingsPage(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~CManagerParamSettingsPage(void);
    enum {
        BTN_SYSTEM_PARAM,
        BTN_TEST_CHANNEL,
        BTN_SYSTEM_MAINTAIN,
        BTN_BACK = 3
    };

    enum REPORT_TYPE{
        TERMINAL_INFO_REPORT,
        TEST_CHANNEL_REPORT
    };

    enum {
        BTN_START_TEST,
        BTN_STOP_TEST,
        BTN_TEMP,
        BTN_RETURN = 3
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
    void initMenu();

    void createTerminalInfoReport();
    void createTestChannelReport();

    void showTerminalInfoReport();
    void showTestChannelReport();

    void updateTerminalInfoMenu();
    void updateTestChannelMenu();

    void updateTestChannelReport();

    void clearTestChannelReport();

private:
    CTerminalInfoReport*        m_pTerminalInfoWidget;
    CDataManager*               m_pDataMgr;
    MTableWidget*               m_pTestChannelReport;

    REPORT_TYPE                 m_eReportType;
};
