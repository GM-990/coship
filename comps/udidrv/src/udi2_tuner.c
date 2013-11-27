/****************************************************************************/
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_tuner.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are required for Android DVB-S2 plus OTT project.
		 Because there is no EEPROM device in the STB system.
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "udi2_tuner.h"
#include "generic_include.h"

#define TUNER_TYPE  1 //1: CABLE, 2:SATALLITE, 3:TERRISIAL

extern CS_TM_PIPE_OBJECTS gTmPipeObject;

/**
@brief 根据指定的频点进行锁频操作.

@param[in] dwTunerId 用于锁频的Tuner设备的设备号。
@param[in] punDeliver 用户存放频点信息的数据块的地址。
@return 
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回错误代码。\n
-- CSUDITUNER_ERROR_BAD_PARAMETER：参数错误。\n
-- CSUDITUNER_ERROR_INVALID_DEVICE_ID：非法ID。\n
-- CSUDITUNER_ERROR_I2C_ERR：I2C总线错误。\n
@note 本接口异步执行，返回成功仅表示命令已接收，不表示命令执行结果。
@note 本接口返回后，如果本次锁的频点与上次不同，则必须马上停止掉上个频点的demux数据接收

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDITunerConnect(CSUDI_UINT32 dwTunerId, const CSUDITunerSRCDeliver_U* punDeliver)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 

	PIPE_TUNING_PARAMETERS PipeTuneParams;
	PIPE_STATUS       PipeStatus = CNXT_STATUS_OK;
#if (TUNER_TYPE == 1) //cable
	PipeTuneParams.bAsync = TRUE;
	PipeTuneParams.Type = PIPE_CABLE_TUNER_TYPE;
	PipeTuneParams.TuningParameters.CableTuningParameters.uFrequencyKHz = punDeliver->m_sCableDeliver.m_uFrequency;
	
	switch (punDeliver->m_sCableDeliver.m_uModulation_8)
	{
	case 1:		
		PipeTuneParams.TuningParameters.CableTuningParameters.Modulation = PIPE_TUNER_MOD_TYPE_16QAM;
		break;
		
	case 2: 	
		PipeTuneParams.TuningParameters.CableTuningParameters.Modulation = PIPE_TUNER_MOD_TYPE_32QAM;
		break;

	case 3:		
		PipeTuneParams.TuningParameters.CableTuningParameters.Modulation = PIPE_TUNER_MOD_TYPE_64QAM;
		break;

	case 4:		
		PipeTuneParams.TuningParameters.CableTuningParameters.Modulation = PIPE_TUNER_MOD_TYPE_128QAM;
		break;		

	case 5:		
		PipeTuneParams.TuningParameters.CableTuningParameters.Modulation = PIPE_TUNER_MOD_TYPE_256QAM;
		break;		
		
	}
	
	
	PipeTuneParams.TuningParameters.CableTuningParameters.uSymbolRateSs = punDeliver->m_sCableDeliver.m_uSymbolRate_24*1000;
	PipeTuneParams.TuningParameters.CableTuningParameters.Annex=PIPE_TUNER_CABLE_ANNEX_A;
	PipeTuneParams.TuningParameters.CableTuningParameters.bAutoSpectrum=TRUE;
	PipeTuneParams.TuningParameters.CableTuningParameters.bInvertedSpectrum = FALSE;
	PipeTuneParams.TuningParameters.CableTuningParameters.uSDPMax =255;
    PipeTuneParams.TuningParameters.CableTuningParameters.uSDPMin =0;

	UDIDRV_LOGI("frank.zhou m_uFrequency =%d\n", PipeTuneParams.TuningParameters.CableTuningParameters.uFrequencyKHz);
	UDIDRV_LOGI("frank.zhou m_uModulation_8 =%d\n", PipeTuneParams.TuningParameters.CableTuningParameters.Modulation);
	UDIDRV_LOGI("frank.zhou m_uSymbolRate_24 =%d\n", PipeTuneParams.TuningParameters.CableTuningParameters.uSymbolRateSs);

	PipeStatus = gTmPipeObject.hTunerObj[0]->tune(gTmPipeObject.hTunerObj[0], &PipeTuneParams);
	if(PipeStatus != PIPE_STATUS_OK)
	{
		UDIDRV_LOGI("frank.zhou tune fail!\n");
	}
	else
	{
		UDIDRV_LOGI("frank.zhou tune success!\n");
	}
#else if (TUNER_TYPE == 2) //sat

#endif
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	 
	return Retcode;
}

 /**
@brief 注册Tuner锁频信息回调

@param[in] dwTunerId tuner设备号。
@param[in] fnTunerCallback 回调函数句柄。
@param[in] pvUserData 用户传入的自定义指针,用来通知驱动返回给用户的数据信息。
@return 如果成功返回CSUDI_SUCCESS, 失败返回错误代码
@note
- 回调函数的注册个数至少支持8个，如果应用注册回调个数超过所支持个数将返回CSUDITUNER_ERROR_NO_MEMORY
- 一组tuner id,callback,userdata共同决定一个Tuner锁频信息回调，当两次注册它们完全一样时，第二次将返回CSUDITUNER_ERROR_ALREADY_ADDED

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDITunerAddCallback(CSUDI_UINT32 dwTunerId,CSUDITunerCallback  fnTunerCallback, void *pvUserData )
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	int *p = pvUserData;
	*p = 1;
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	 
	return Retcode;
}

 /**
@brief 删除指定的锁频回调函数的注册

@param[in] dwTunerId tuner设备号。
@param[in] fnTunerCallback 回调函数句柄。
@param[in] pvUserData 同CSUDITunerAddCallback的pvUserData
@return成功返回CSUDI_SUCCESS 如果该回调函数根本就没有注册，将返回CSUDI_FAILURE
@note 对于可移动的Tuner，当其被拔出后，仍需要主动删除其对应的回调注册，否则仍会占用部分资源（主要是内存）

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDITunerDelCallback(CSUDI_UINT32 dwTunerId,CSUDITunerCallback  fnTunerCallback, void *pvUserData)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	 
	return Retcode;
}

 /**
@brief 取得当前正在锁定的信号信息

@param[in] dwTunerId 用于锁频的Tuner设备的设备号
@param[in] u32SignalInfoMask 需获取的信号信息类型,取值为CSUDITunerSignalInfoType_E中的枚举值或枚举中任意值相或，对应位为1则返回该信息；为0则不返回，此时对应结构的值是不确定的。
@param[out] psSignalInfo 用来保存获取的信号信息。
@return 
- 成功返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE
- 如果失败返回如下错误代码:\n
-- CSUDITUNER_ERROR_BAD_PARAMETER ：参数错误。\n
-- CSUDITUNER_ERROR_INVALID_DEVICE_ID：非法设备ID。\n 
-- CSUDITUNER_ERROR_I2C_ERROR：I2C总线错误。\n 
@note 获取所有这些信息可能会涉及一些不同的Tuner操作，为提高效率，请尽量仅获取需关注的信息，而不是全部。因为大多数情况下，需要的信息越多，该接口需要的时间越长
@note 该接口一定要迅速返回，不必获取非常精确的信息。不建议在该接口中直接调用驱动阻塞接口查询。建议在udi层定时查询，将所需的信息先取出来，调用本接口时直接获取即可。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDITunerGetSignalInfo( CSUDI_UINT32 dwTunerId, CSUDI_UINT32 u32SignalInfoMask, CSUDITunerSignalInfo_S *psSignalInfo)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	 
	return Retcode;
}
 /**
@brief 取得Tuner设备的能力信息

@param[in] dwTunerId 用于标记Tuner设备的设备号
@param[out] psTunerInfo 用来保存获取的Tuner能力信息
@return 成功获取返回CSUDI_SUCCESS，失败则返回错误代码值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDITunerGetDeviceInfo( CSUDI_UINT32 dwTunerId, CSUDITunerInfo_S *psTunerInfo)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	 
	return Retcode;
}

/**
@brief 获取所有设备的Id

包括可移动的Tuner设备和固定的设备
@param[out] pdwIds 用于存储所有存在的设备的Id
@param[in] nMaxIdCnt pdwIds数组的大小,即最多能返回多少设备Id
@param[out] pnActIdCnt 实际的设备ID数
@return 成功返回CSUDI_SUCCESS，失败返回错误代码值
@note
- 为保持兼容，所有非可移动设备的设备号仍从0开始索引
- 允许pdwIds为CSUDI_NULL，这种情况下表示应用仅关注有多少个tuner设备，但不关心它们具体的Id

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDITunerGetAllDeviceId(CSUDI_UINT32 * pdwIds,int nMaxIdCnt, int * pnActIdCnt)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 增加Tuner硬过滤Pid

- 对于m_bPIDFilterSupport为CSUDI_TRUE的Tuner，必须通过该接口增加硬过滤Pid后，方可获取数据。
- 对于m_bPIDFilterSupport为CSUDI_FALSE的Tuner，该接口返回CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED，并且不需要调用该接口即可获取所有数据
@param[in] dwTunerId Tuner设备号
@param[in] nPID 需添加过滤的数据Pid，添加后该Pid的数据可通过进入对应的后处理模块（一般为Demux）
@return 成功返回CSUDI_SUCCESS，失败返回错误代码值
@note 初始状态下，Tuner不通过任何Pid的数据

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDITunerAddPID (CSUDI_UINT32 dwTunerId ,int nPID)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 删除Tuner硬过滤Pid

- 对于m_bPIDFilterSupport为CSUDI_TRUE的Tuner，可以通过该接口删除CSUDITunerAddPID接口增加的硬过滤Pid，阻止获取对应PID的数据。
- 对于m_bPIDFilterSupport为CSUDI_FALSE的Tuner，该接口返回CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED，并且不需要调用该接口即可获取所有数据
@param[in] dwTunerId Tuner设备号
@param[in] nPID 需删除过滤的数据Pid，删除后该Pid的数据不可通过进入对应的后处理模块（一般为Demux）
@return 成功返回CSUDI_SUCCESS，失败返回错误代码值
@note nPID为~0表示删除该Tuner下的所有pid过滤，即此后无法通过该Tuner获取任何数据（除非再次调用CSUDITunerAddPID）
@note 初始状态下，Tuner不通过任何Pid的数据
@see CSUDITunerAddPID

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDITunerRemovePID (CSUDI_UINT32 dwTunerId ,int nPID)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/*-----------------以下接口定义可移动Tuner的相关接口---------------------*/
/**
@brief 添加设备通知回调函数

详见RDI模块CSUDIRDIAddCallback_F接口定义，仅用于处理“TUNER”类型的可移动设备
@param[in] fnCallback 回调函数,当设备状态发生变化时调用该函数
@param[in] pvUserData 用户数据,将作为回调函数的pvUserData参数返回
@return 成功添加返回CSUDI_SUCCESS,失败返回错误代码值
@note 添加回调前产生的设备变化信息,不会补充通知.即若开机时这些设备就存在,调用本函数时不会得到通知.
@note 若平台不需要支持任何可移动Tuner，该接口返回CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDITunerRDIAddCallback(CSUDIRDICallback_F fnCallback,const void * pvUserData)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 删除设备通知回调函数

详见RDI模块CSUDIRDIRemoveCallback_F接口定义，仅用于处理“TUNER”类型的可移动设备
@param[in] fnCallback 回调函数,之前注册的函数
@param[in] pvUserData 用户数据,必须与注册时的一致
@return 成功删除返回CSUDI_SUCCESS,失败返回错误代码值
@note fnCallback和pUserData一起唯一确定一个回调函数
@note 若平台不需要支持任何可移动Tuner，该接口返回CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDITunerRDIRemoveCallback(CSUDIRDICallback_F fnCallback,const void * pvUserData)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 安全移除可移动Tuner设备

详见RDI模块CSUDIRDIRemoveCallback_F接口定义，仅用于处理“TUNER”类型的可移动设备
@param[in] dwTunerId 目标设备ID
@return 成功则返回CSUDI_SUCCESS,失败返回错误代码值
@note
- 只能删除可移动设备，试图删除其它设备类型则返回CSUDIFS_ERROR_FEATURE_NOT_SUPPORTED，通过设备的设备Id可判断设备类型，删除设备后不可再通过该设备Id获取设备信息。用户调用该接口会触发EM_UDIRDI_PLUGOUT_SAFE类型的PLUGOUT事件。 
- 若平台不需要支持任何可移动Tuner，该接口返回CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDITunerRDIRemoveDev(CSUDI_UINT32 dwTunerId)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 从可移动Tuner读取数据

成功锁频并设置过滤PID(如果需要)后，可通过该接口读取到TS流数据
@param[in] dwTunerId 目标设备Id
@param[in] pvData 指向读入数据的Buffer
@param[in] nBuffLength pvData的长度
@param[out] pnReadLength 实际读到的数据长度
@return 成功则返回CSUDI_SUCCESS,失败返回错误代码值
@note 接口应立刻返回，若不能读到任何数据，则*pnReadLength==0，但仍返回成功
@note 详见RDI模块中CSUDIRDIIORead_F原形
@note 若平台不需要支持任何可移动Tuner，该接口返回CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDITunerIORead(CSUDI_UINT32 dwTunerId,void* pvData,int nBuffLength,int * pnReadLength)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 获取一组PlpID,PlpID为0~255的整数

@param[in] dwTunerId 用于锁频的Tuner设备的设备号。
@param[out] pbPlpIds 用于返PLP ID的数组，数组大小应该至少256，否则会由于可用的PLP ID数量超过数组大小而返回错误
@param[in] nPlpIdsCount pbPlpIds指向的数组大小
@param[out] pnRetIdCount 用于返回实际保存到pbPlpIds中的PlpID个数
@return 
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回错误代码。\n
-- CSUDITUNER_ERROR_BAD_PARAMETER：参数错误。\n
-- CSUDITUNER_ERROR_INVALID_DEVICE_ID：非法ID。\n
-- CSUDITUNER_ERROR_I2C_ERR：I2C总线错误。\n
@note 本接口为DVB-T2专用。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDITunerT2GetPlpInfo(CSUDI_UINT32 dwTunerId, CSUDI_UINT8* pbPlpIds, int nPlpIdsCount, int* pnRetIdCount)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}

/**
@brief 获取当前高频头的锁定状态

@param[in] dwTunerId 用于锁频的Tuner设备的设备号。
@param[out] pnLockStatus 用于返回当前的锁定状态，*pnLockStatus == 0 表示未锁定，*pnLockStatus == 1表示锁定。
@return 
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回错误代码。\n
-- CSUDITUNER_ERROR_BAD_PARAMETER：参数错误。\n
-- CSUDITUNER_ERROR_INVALID_DEVICE_ID：非法ID。\n
-- CSUDITUNER_ERROR_I2C_ERR：I2C总线错误。\n

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDITunerGetLockStatus(CSUDI_UINT32 dwTunerId, int *pnLockStatus)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

