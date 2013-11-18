/**@defgroup  提供Nagra规范booter接口
@brief NAGRABOOT模块对应头文件<csbl_nagra.h>，定义了Nagra规范booter所有的功能及配置接口。

@version 1.0.0 2012/2/03
@{
*/
#ifndef _CSBL_NAGRA_H_
#define _CSBL_NAGRA_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include "csbl_gconfig.h"
#include "udi2_toolset.h"
#include "udiplus_typedef.h"


/*
 结构体说明:
1， 在MMCP BL中，不论是SPI FLASH，还是NOR FLASH，APP和LOADER均要求读取到内存中进行校验
2，提供给NAGRA的JUMPCODE要求均为内存地址，而不是FLASH地址
	 
*/
typedef struct
{
    CSBLDataPos_S m_sDA2Info;
    CSBLDataPos_S m_sDa2BakInfo;
    CSBLDataPos_S m_sDa2LockInfo;
    CSBLDataPos_S m_sPkInfo;
    DWORD 		m_LoaderDdrPos;	//设计文档中标识的LOADER内存校验地址
    DWORD 		m_AppDdrPos;		//设计文档中标识的APP内存校验地址
}CSBLNagraConfig_S;


/*
函数说明:
CSBLNagraSBPCheck 用来对指定模块进行NAGRA SBP校验

输入参数:
pCheckType[in]: 校验数据类型地址信息，校验数据类型为CSUDIToolsetExecType_E
pConfigInfo [in]: NARGA相关数据信息
pPos[in]:		     校验数据地址信息
pJcAddr [in]:       保存JUMPCODE 地址信息的地址

返回值:
	FALURE:CHECK失败，SUCCESS:校验成功
*/

int CSBLNagraSBPCheck(CSUDIToolsetExecType_E* pCheckType,CSBLNagraConfig_S* pConfigInfo,CSBLDataPos_S* pPos,PDWORD pJcAddr);


#ifdef  __cplusplus
}
#endif

#endif



