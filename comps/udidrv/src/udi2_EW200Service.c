/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_demux.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
 *-------------------------------------------------------------------------------


 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"


#include "udi2_EW200Service.h"
/**
@brief 获取终端信息接口

Eastwin200将通过该接口获取终端的序列号,智能卡号等信息,或获取网页中设置的信息
@param[in] eType 信息类型
@param[in] value eType类型对应的值
@param[in] length value值的长度
@return 返回实际信息的长度,出错返回-1
@note 需要设置的信息见CSUDIEW200SClientInfoType_E枚举中的[r]标识
@note 支持Coship对象、Enreach对象时需要实现该接口

@code
//参考实现(未考虑缓冲区溢出等异常)
//注意:以下两种情况，value表示的类型及含义略有不同，请按参考代码实现/使用即可。
if(eType == EM_UDIEW200S_AUDIOCHANNEL)
{
	//EM_UDIEW200S_AUDIOCHANNEL对应数据类型是int的
	//value为int的变量，其长度应该为sizeof(int)
	
	value = EM_UDIEW200S_AUD_CHANNEL_STEREO;
	CSUDIEW200SReadClientInfo(eType , (void*)&value, sizeof(int));

}
else if(eType == EM_UDIEW200S_SN)
{
	//EM_UDIEW200S_SN对应数据类型是CSUDI_UINT8数组
	//value一个指向CSUDI_UINT8数组，其长度为length
	//从value指向的buffer中获取对应内容
	value[32];
	CSUDIEW200SReadClientInfo(eType , (void*)value, sizeof(value));

}
@endcode
*/
int CSUDIEW200SReadClientInfo(CSUDIEW200SClientInfoType_E eType, void* value, int length)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_NOT_REQUIRED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}
/**
@brief 设置终端信息接口

Eastwin200将通过该接口将一些用户信息设置到终端永久存储或者应用自己存储起来
@param[in] eType 信息类型
@param[in] value eType类型对应的值
@param[in] length value值的长度
@return 返回实际存储信息的长度,出错返回-1
@note 需要设置的信息见CSUDIEW200SClientInfoType_E枚举中的[w]标识
@note 支持Coship对象、Enreach对象时需要实现该接口
@note 参考实现同CSUDIEW200SReadClientInfo
*/
int CSUDIEW200SWriteClientInfo(CSUDIEW200SClientInfoType_E eType, void* value, int length)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_NOT_REQUIRED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

/**
@brief 通过DVB3要素查询数据源信息

包括获取该3要素对应信息时所需使用的TunerId,DemuxId,及频点
@param[in] nOrNetId org network id
@param[in] nTsId ts id
@param[in] nServiceId service id
@param[out] psSourceParam 用于存储输出的数据源信息
@return 如果成功返回CSUDI_SUCCESS, 失败返回错误代码
@note EW200通过该接口查询一个Service的对应的频点及高频头信息，若平台有多个高频头，这个接口的实现要特别小心，因为对应的数据可能在不同的高频头上，否则简单查询数据管理即可
*/
CSUDI_Error_Code CSUDIEW200SGetTuneParam(int nOrNetId,int nTsId,int nServiceId,CSUDIEW200SSourceParam_S * psSourceParam)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_NOT_REQUIRED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

/**
@brief 获取IPQam域信息接口

常用语VOD模块
认证模块通过调用终端实现的该接口得到IPQam的域信息
- CITV3.x版本的VOD目前取参数指定Section的第4、5字节(即data[3],data[4])
@param[in] punDeliverList 存放信息所在频点列表数组
@param[in] nDeliverCount pDeliverList数组中的频点个数
@param[in] nPid IPQam域信息所在Pid
@param[in] ucTableId IPQam域信息所在TableId
@param[out] pIpQamDataBuffer 用于存放IPQam域信息的内存指针
@param[in] nBufferLength pIpQamDataBuffer的长度
@param[in] pvReserved 保留字段,目前必须为NULL
@return 成功获取返回CSUDI_TRUE,失败返回CSUDI_FALSE
*/
CSUDI_BOOL CSUDIEW200SGetIPQamDomainInfo(const CSUDITunerSRCDeliver_U* punDeliverList,
										int nDeliverCount,
										int nPid,
										unsigned char ucTableId,
										char* pIpQamDataBuffer,
										int nBufferLength,
										void* pvReserved
									   )
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_NOT_REQUIRED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

/**
@brief 播放节目接口

根据制定URL的格式进行播放

@param[in] pcURL 播放URL
@param[in] nPlayId 参见Eastwin200VOD中的nPlayId定义,在支持多Demux,多Tuner,Pip等功能的机器上移植层可以用该参数配合应用确定使用哪个设备，其它情况可忽略该参数
@code 播放URL的格式有以下几种，应用可以根据不同的URL格式进行播放处理：
(1)dvb://OriginalNetworkId.TS_Id.ServiceId  具体参考《数字电视中间件技术规范》第3部分附录I的1.2.1小节。
		OriginalNetworkId: original_network_id
		TS_Id: transport_stream_id
		ServiceId: service_id
		例如: dvb://1.2.5(十六进制)
(2)dvbc://Frequency.SymbolRate.Modulation.ServiceId
		Frequency:频率.单位MHz
		SymbolRate:符号率，单位是Kbps
		Modulation:调制模式
		ServiceId:节目Id
		例如: deliver://490000000:6875:64:10(ServiceId为十六进制)
(3)avpid://pcrPid.emm.videoPid.audioPid.audio_ecm_pid.video_ecm_pid(十六进制)	
@endcode
*/
void CSUDIEW200SPlayStream(char* pcURL, int nPlayId)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_NOT_REQUIRED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

/**
@brief 停止节目播放接口

@param[in] nPlayId 参见Eastwin200VOD中的nPlayId定义,在支持多Demux,多Tuner,Pip等功能的机器上移植层可以用该参数配合应用确定使用哪个设备，其它情况可忽略该参数
*/
void CSUDIEW200SStopPlay(int nPlayId)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_NOT_REQUIRED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

/**
@brief 设置视频播放窗口大小接口

@param[in] psRect 视频矩形大小,为NULL表示全屏播放
@param[in] nPlayId 参见Eastwin200VOD中的nPlayId定义,在支持多Demux,多Tuner,Pip等功能的机器上移植层可以用该参数配合应用确定使用哪个设备，其它情况可忽略该参数
*/
void CSUDIEW200SSetPlayRect(CSUDIWinRect_S* psRect,int nPlayId)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_NOT_REQUIRED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

/**
@brief 恢复暂停模式 

@param[in] nPlayId 参见Eastwin200VOD中的nPlayId定义,在支持多Demux,多Tuner,Pip等功能的机器上移植层可以用该参数配合应用确定使用哪个设备，其它情况可忽略该参数

@return 
- 成功返回CSUDI_SUCCESS；
- 失败返回其他
不支持此操作返回CSUDIEW200S_ERROR_FEATURE_NOT_SUPPORTED
@note 恢复暂停模式;与CSUDIEW200SPause成对使用。
*/
CSUDI_Error_Code CSUDIEW200SResume(int nPlayId)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_NOT_REQUIRED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

/**
@brief 暂停与hPlayer相关的一个播放器的工作

@param[in] nPlayId 参见Eastwin200VOD中的nPlayId定义,在支持多Demux,多Tuner,Pip等功能的机器上移植层可以用该参数配合应用确定使用哪个设备，其它情况可忽略该参数

@return 
- 成功返回CSUDI_SUCCESS;
- 失败返回其他
不支持此操作返回CSUDIEW200S_ERROR_FEATURE_NOT_SUPPORTED
@note 暂停与hPlayer相关的一个播放器的工作；只有正在播放某一节目（不论是只有音频还是视频）时调用才有效。
*/
CSUDI_Error_Code CSUDIEW200SPause(int nPlayId)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_NOT_REQUIRED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}
/**
@brief 进入退出TrickMode

通知驱动层后续音视频可能会按非正常速度进入解码器。
常用于VOD播放时的快退快进等，这种情况下Tuner过来的数据为非正常速度，
平台需做好准备以防止过度阶段及播放阶段的马赛克。
@param[in] nPlayId 参见Eastwin200VOD中的nPlayId定义,在支持多Demux,多Tuner,Pip等功能的机器上移植层可以用该参数配合应用确定使用哪个设备，其它情况可忽略该参数
@param[in] bTrickMode 为CSUDI_TRUE表示进入TrickMode,为CSUDI_FALSE表示退出TrickMode
@return 
- 成功返回CSUDI_SUCCESS；
不支持此操作返回CSUDIEW200S_ERROR_FEATURE_NOT_SUPPORTED
已经处于此状态返回CSUDIEW200S_ERROR_TRICKMODE_STATE_ERROR
*/
CSUDI_Error_Code CSUDIEW200STrickMode(int nPlayId,CSUDI_BOOL bTrickMode)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_NOT_REQUIRED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

/**
@brief 注册UDI层消息回调函数

注册后UDI层相关的消息会通过该接口通知到应用
@param[in] nPlayId 参见Eastwin200VOD中的nPlayId定义,在支持多Demux,多Tuner,Pip等功能的机器上移植层可以用该参数配合应用确定使用哪个设备，其它情况可忽略该参数
@param[in] pUserData 用户数据,会在回调中原样返回
@return 成功返回CSUDI_SUCCESS,失败返回错误代码见CSUDIEW200S_ERRORCODE_E定义
@note
- 回调函数的注册个数至少支持5个，如果应用注册回调个数超过所支持个数将返回CSUDIEW200S_ERROR_NO_MEMORY
- 一组nPlayId,fncallback,pUserdata共同决定一个回调，当两次注册它们完全一样时，第二次将返回CSUDIEW200S_ERROR_ALREADY_ADDED
*/
CSUDI_Error_Code CSUDIEW200SAddCallback(int nPlayId,CSUDIEW200SCallback_F fncallback,void* pUserdata)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_NOT_REQUIRED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

 /**
@brief 删除UDI层回调函数的注册

@param[in] nPlayId 参见Eastwin200VOD中的nPlayId定义,在支持多Demux,多Tuner,Pip等功能的机器上移植层可以用该参数配合应用确定使用哪个设备，其它情况可忽略该参数
@param[in] fnTunerCallback 回调函数句柄
@param[in] pUserData 同CSUDIEW200SAddCallback中的pUserData
@return成功返回CSUDI_SUCCESS 失败错误返回见CSUDIEW200S_ERRORCODE_E定义
*/
CSUDI_Error_Code CSUDIEW200SDelCallback(int nPlayId,CSUDIEW200SCallback_F fncallback,void* pUserdata)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_NOT_REQUIRED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	 
    return Retcode;
}

