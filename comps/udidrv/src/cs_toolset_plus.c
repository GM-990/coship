/****************************************************************************/
/*
 * Filename:        cs_toolset_plus.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are required for Android DVB-S2 plus OTT project. 
 *-------------------------------------------------------------------------------

 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "cs_toolset_plus.h"


/**
@brief 移植层配置接口，调用时机由各eCfgType决定,具体见CSSYSCfg_E定义

该接口可被调用多次，同一类型的配置以最后一次的调用为准
@param[in] eCfgType 配置类型,具体见CSSYSCfg_E定义;
@param[in] nDeviceIndex 所要配置的设备index,如果该eCfgType无对应的设备时,传入-1;
@param[in] unValue1	配置参数1,具体含义由eCfgType决定;
@param[in] unValue2 配置参数2,具体含义由eCfgType决定;
@return 配置成功返回CSUDI_SUCCESS ，配置失败返回错误值。
*/
CSUDI_Error_Code CSUDISYSSetCfg(CSUDISYSCfg_E eCfgType, int nDeviceIndex, CSUDI_UINT32 unValue1, CSUDI_UINT32 unValue2)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

/**
@brief 移植层系统初始化接口，实现系统的分区划分，并启动操作系统，系统启动时第一个被调用

@return 调用成功返回CSUDI_SUCCESS ，调用失败返回错误值。
*/
CSUDI_Error_Code CSUDISYSInit(void)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

/**
@brief 移植层系统初始化接口

该接口只可被调用一次，必须在CSUDISYSInit后调用
@return 调用成功返回CSUDI_SUCCESS ，调用失败返回错误值。
@note 
- 初始化机顶盒主芯片相关设备驱动，如AV decoder，Demux等等，并初始化相应的移植层模块，
	如Player,Inject,Audio,Video,Screen,Demux,Section等，初始化之后调用.用户不需要关心各设备的初始化顺序。
	初始化的配置参数使用CSUDISYSSetCfg设置，如果没有设置将按默认值初始化。
*/
CSUDI_Error_Code CSUDIPlatformInit(void)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

/**
@brief 设置真待机唤醒参数

@param[in] pstStandbyParams 见CSUDISYSStandbyParams_S
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDISYSSetWakeupParams(const CSUDISYSWakeupParams_S* pstWakeupParams)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

/**
@brief 芯片待机的钩子函数定义
@param[in] bStandbyStatus 	TRUE:进入待机状态;FALSE:退出待机状态
@param[in] nStandbyTime  待机时间
@param[out] pbSkipDefault 是否忽略默认操作，TRUE表示只执行Hook函数定义的操作，FALSE表示同时执行CSUDISYSChipStandby原有操作，默认为TRUE
@return CSUDI_SUCCESS 表示钩子函数执行成功，其它为返回的错误代码值。
*/
//typedef CSUDI_Error_Code (*CSUDIChipStandbyHookFunc_F)(CSUDI_BOOL bStandby,int nStandbyTime, CSUDI_BOOL *pbSkipDefault);

/**
@brief 设置芯片待机的钩子函数

@param[in] fnHook 钩子函数，见CSUDIChipStandbyHookFunc_F定义
@return 如果调用成功，返回调用CSUDI_SUCCESS；如果调用失败则返回错误代码值。
@note fnHook可以为NULL，表示取消钩子函数
*/
CSUDI_Error_Code CSUDISYSSetChipStandbyHook(CSUDIChipStandbyHookFunc_F fnHook)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
} 

/**
@brief 设置引导信息

根据引导类型设置额外的引导信息，用于在CSUDIToolsetExec时使用相同的目标代码完成不同的工作。
如loader和app可以使用相同Linux内核的两个拷贝，但根据本接口设置的引导参数加载不同的rootfs。
@param[in] eType 引导信息
@param[in] pExecParam 引导参数，具体意义由CSUDIToolsetExec自行决定，并告知初始化模块设置，调用者必须保证所指向内存一直有效。
@return 调用成功返回CSUDI_SUCCESS ，调用失败返回错误代码
@see CSUDIToolsetExec
*/
CSUDI_Error_Code CSUDIToolsetSetExecInfo(CSUDIToolsetExecType_E eType,void* pExecParam)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

/**
@brief 设置目标机器的序列号和Key，与UDI接口CSUDIGetSerialKey对应

@param[in] psSerialKey 指向存放获取到的序列号和Key的结构体指针，序列号和Key均是以0结尾的字符串，请参见CSUDISerialKey_S。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 由于在coship内部，CSUDIGetSerialKey接口驱动部门实现，但是驱动并不知道如何获取序列号和Key，所以需要有一个set接口，供应用在初始化时调用
*/
//CSUDI_Error_Code CSUDISetSerialKey(const CSUDISerialKey_S * psSerialKey);


