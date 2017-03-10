/**
 *  @file
 *  @brief 现场总线工厂
 *  @author 
 *  @date 2012-05-10
 *  @version v1.0
 *  @note 
 */
#ifndef __BASEB_FB_FACTORY_H__
#define __BASEB_FB_FACTORY_H__
#include "fieldbus.h"

enum
{
	FB_IDX_485I = 0,
	FB_IDX_485II,
	FB_IDX_ZBWX,
	FB_IDX_JLCY,

	FB_IDX_MAX,
};

class C_FBFact
{
public:
	/** @fn
	 *  @brief 初始化所有配置的现场总线
	 *  @param[in] pRS485I 总线RS485I连接,如为NULL,则不初始化
	 *  @param[in] pRS485II 总线RS485II连接,如为NULL,则不初始化
	 *  @param[in] pZBWX 载波或无疑总线连接,如为NULL,则不初始化
	 *  @param[in] pJLCY 交流采样总线连接,如为NULL,则不初始化
	 *  @return 0:成功; <0:失败
	 *  @note 连接必需为非局部变量
	 */
	static int InitFiledBus(C_CONNECT *pRS485I, C_CONNECT *pRS485II, C_CONNECT *pZBWX, C_CONNECT *pJLCY);

	/** @fn
	 *  @brief 查找现场总线
	 *  @param[in] VCOM 总线虚拟串口号
	 *  @return !NULL:成功; NULL:失败
	 */
	static C_FieldBus* FindFieldBus(int VCOM);

	/** @fn
	 *  @brief 获取现场总线
	 *  @param[in] IDX 总线索引号
	 *  @return !NULL:成功; NULL:失败
	 */
	static C_FieldBus* GetFieldBus(int IDX);

private:
	static int InitFiledBus_RS485I(C_CONNECT *pRS485I);
	static int InitFiledBus_RS485II(C_CONNECT *pRS485II);
	static int InitFiledBus_ZBWX(C_CONNECT *pZBWX);
	static int InitFiledBus_JLCY(C_CONNECT *pJLCY);
};


#endif //__BASEB_FB_FACTORY_H__

