/**
 *  @file
 *  @brief 下行模块管理
 *  @author 
 *  @date 2012-05-31
 *  @version v1.0
 *  @note
 */
#ifndef __BASEB_DN_MODULE_H__
#define __BASEB_DN_MODULE_H__
#include "basedef.h"
#include "basetype.h"
#include "timeop.h"
#include "connect.h"
#include "fieldbus.h"

class C_DnModule
{
public:
	/** @fn
	 *  @brief 初始化所有配置的表计规约
	 *  @param[in] pRS485I 总线RS485I连接,如为NULL,则不初始化
	 *  @param[in] pRS485II 总线RS485II连接,如为NULL,则不初始化
	 *  @param[in] pZBWX 载波或无疑总线连接,如为NULL,则不初始化
	 *  @param[in] pJLCY 交流采样总线连接,如为NULL,则不初始化
	 *  @return 0:成功; <0:失败
	 *  @note 连接必需为非局部变量
	 */
	int InitFiledBus(C_CONNECT *pRS485I, C_CONNECT *pRS485II, C_CONNECT *pZBWX, C_CONNECT *pJLCY);

	/** @brief 下行模块复位
	 *  @return 0:成功; -1:失败
	 */
	int Reset(C_CONNECT *pZBWX);
	/** @brief 获取模块名称
	 *  @return string
	 */
	const char *GetModuleName(void);
	/** @brief 设置模块参数
	 *  @param[in] FbConfig 模块参数
	 *  @return bool
	 */
	bool SetModuleConfigure(const S_FBCONFIG &FbConfig);

	/** @brief 现场总线状态同步，刚上电或进程重启需要状态同步 
	 *  @param[in] strAddress 模块地址，如没有，则不同步地址
	 *  @param[out] strFactory 输出模块工厂代码和版本号
	 *  @return 0:成功; -1:失败
	 *  @note 如同步失败，则调用C_DnModule::Reset()命令，再进行现场总线工厂初始化
	 *  @see fbfact.h
	 */
	int SyncRouterStatus(const string &strAddress, string &strFactory);
	/** @brief 现场总线路由启动、查询函数 
	 *  @param[in] timeKeeping 路由时间，0:退出路由;0xFFFFFFFF:查询路由状态;其它：进入路由
	 *  @param[in] Meter485 采集器下485表地址
	 *  @param[in] MeterZB 载波表地址
	 *  @param[in] Relay 采集器地址
	 *  @return <0:失败 0:成功，路由结束 >0:剩余路由时间,单位秒
	 */
	int Router(S_TIME timeKeeping, const S_TERMINALS &Meter485, const S_TERMINALS &MeterZB, const S_TERMINALS &Relay);
};

//
// 本地通信模块工厂
//
class C_DnModuleFact
{
public:
	/** @brief 获取本地通讯模块 */
	static C_DnModule* GetDnModule(void);
};

#endif //__BASEB_DN_MODULE_H__

