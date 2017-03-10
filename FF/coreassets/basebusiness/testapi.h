/**
 *  @file
 *  @brief 定义生产自动测试的接口
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

/** @brief 测试运行状态 */
enum
{
	TEST_EXEC_ERROR,
	TEST_EXEC_CONTINUE,
	TEST_EXEC_MSG,
	TEST_EXEC_REQ_OK,
	TEST_EXEC_FINISH,
	
	TEST_EXEC_MAX
};

/** @brief 测试结果 */
enum S_TestResult
{
	TEST_RESULT_STANDBY,
	TEST_RESULT_UNPASS,
	TEST_RESULT_PASS,
};

/** @brief 测试项*/
enum S_TestItemType
{
	TEST_ITEM_ALL,
	TEST_ITEM_AUTO,
	TEST_ITEM_MANUAL
};

/** @brief 用户接口：测试项名称 */
struct S_TestName
{
	string m_strName; ///<测试项名称
	S_TestResult m_Result; ///<测试结果
};
class S_TestNames:public vector<S_TestName>
{
};

/** @class
 *  @brief 生产自动化测试用户接口
 */
class C_FactAutoTest
{
public:
	C_FactAutoTest(void){}
	virtual ~C_FactAutoTest(void){}

public:
	/** @brief 初始化测试信息
	 *  @return 0:成功; -1:失败
	 */
	virtual int InitAutoTestInfo(C_TermApi &objTermApi, const char *pResourcePath) = 0;

	/** @brief 设置终端接口对象
	 *  @param[in] objTermApi 终端接口对象
	 *  @return 0:成功; -1:失败
	 */
	virtual int SetTermApiObj(C_TermApi &objTermApi) = 0;

	/** @brief 获取测试项目
	 *  @param[in] objTermApi 终端接口对象
	 *  @return 0:成功; -1:失败
	 */
	virtual int GetAutoTestItemName(S_TestNames &AutoItem, S_TestNames &ManualItem) = 0;

	/** @brief 清除测试记录
	 *  @return 0:成功; -1:失败
	 */
	virtual int ClearTestMark(S_TestItemType type) = 0;

	/** @brief 命名测试报告
	 *  @param[in] strReportName 测试报告名称
	 *  @return 0:成功; -1:失败
	 */
	virtual int NameAutoTestReport(string &strReportName) = 0;

	/** @brief 创建测试报告
	 *  @param[in] strReportName 测试报告名称
	 *  @return 0:成功; -1:失败
	 */
	virtual int CreateAutoTestReport(const string &strReportName, const char *pPath) = 0;

	/** @brief 测试执行
	 *  @param[in] fAutoTest true:自动测试;false:手动测试
	 *  @param[in] TestItem 测试项名称
	 *  @param[out] strMsg 测试信息输出或输入
	 *  @return TEST_EXEC_
	 */
	virtual int ExecuteTestItem(bool fAutoTest, const S_TestName &TestItem, string &strMsg) = 0;
};

class C_USBPATH
{
public:
	/** @brief 取USB设备根目录 */
	static const char *GetUsbRoot(void);
	/** @brief 取USB设备测试目录文件 */
	static const char *GetUsbTestFile(const char *fname, char *fpath);
	/** @brief 取USB设备测试目录 */
	static const char *GetUsbTestPath(void);
};

/** @class
 *  @brief 生产自动测试对象工厂
 */
class C_AutoTestFactory
{
public:
	static C_FactAutoTest *GetFactoryAutoTestObj(void);
};

#endif //__BASEB_TEST_API_H__

