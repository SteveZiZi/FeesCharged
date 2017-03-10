/**
 *  @file
 *  @brief ���������Զ����ԵĽӿ�
 *  @author 
 *  @date 2012-10-28
 *  @version v1.0
 *  @note 
 */
#ifndef __BASEB_TEST_API_H__
#define __BASEB_TEST_API_H__
#include "basedef.h"
#include "termapi.h"

#define TEST_USB_FLAG	"testusb.flg"

/** @brief ��������״̬ */
enum
{
	TEST_EXEC_ERROR,
	TEST_EXEC_CONTINUE,
	TEST_EXEC_MSG,
	TEST_EXEC_REQ_OK,
	TEST_EXEC_FINISH,
	
	TEST_EXEC_MAX
};

/** @brief ���Խ�� */
enum S_TestResult
{
	TEST_RESULT_STANDBY,
	TEST_RESULT_UNPASS,
	TEST_RESULT_PASS,
};

/** @brief ������*/
enum S_TestItemType
{
	TEST_ITEM_ALL,
	TEST_ITEM_AUTO,
	TEST_ITEM_MANUAL
};

/** @brief �û��ӿڣ����������� */
struct S_TestName
{
	string m_strName; ///<����������
	S_TestResult m_Result; ///<���Խ��
};
class S_TestNames:public vector<S_TestName>
{
};

/** @class
 *  @brief �����Զ��������û��ӿ�
 */
class C_FactAutoTest
{
public:
	C_FactAutoTest(void){}
	virtual ~C_FactAutoTest(void){}

public:
	/** @brief ��ʼ��������Ϣ
	 *  @return 0:�ɹ�; -1:ʧ��
	 */
	virtual int InitAutoTestInfo(C_TermApi &objTermApi, const char *pResourcePath) = 0;

	/** @brief �����ն˽ӿڶ���
	 *  @param[in] objTermApi �ն˽ӿڶ���
	 *  @return 0:�ɹ�; -1:ʧ��
	 */
	virtual int SetTermApiObj(C_TermApi &objTermApi) = 0;

	/** @brief ��ȡ������Ŀ
	 *  @param[in] objTermApi �ն˽ӿڶ���
	 *  @return 0:�ɹ�; -1:ʧ��
	 */
	virtual int GetAutoTestItemName(S_TestNames &AutoItem, S_TestNames &ManualItem) = 0;

	/** @brief ������Լ�¼
	 *  @return 0:�ɹ�; -1:ʧ��
	 */
	virtual int ClearTestMark(S_TestItemType type) = 0;

	/** @brief �������Ա���
	 *  @param[in] strReportName ���Ա�������
	 *  @return 0:�ɹ�; -1:ʧ��
	 */
	virtual int NameAutoTestReport(string &strReportName) = 0;

	/** @brief �������Ա���
	 *  @param[in] strReportName ���Ա�������
	 *  @return 0:�ɹ�; -1:ʧ��
	 */
	virtual int CreateAutoTestReport(const string &strReportName, const char *pPath) = 0;

	/** @brief ����ִ��
	 *  @param[in] fAutoTest true:�Զ�����;false:�ֶ�����
	 *  @param[in] TestItem ����������
	 *  @param[out] strMsg ������Ϣ���������
	 *  @return TEST_EXEC_
	 */
	virtual int ExecuteTestItem(bool fAutoTest, const S_TestName &TestItem, string &strMsg) = 0;
};

class C_USBPATH
{
public:
	/** @brief ȡUSB�豸��Ŀ¼ */
	static const char *GetUsbRoot(void);
	/** @brief ȡUSB�豸����Ŀ¼�ļ� */
	static const char *GetUsbTestFile(const char *fname, char *fpath);
	/** @brief ȡUSB�豸����Ŀ¼ */
	static const char *GetUsbTestPath(void);
};

/** @class
 *  @brief �����Զ����Զ��󹤳�
 */
class C_AutoTestFactory
{
public:
	static C_FactAutoTest *GetFactoryAutoTestObj(void);
};

#endif //__BASEB_TEST_API_H__

