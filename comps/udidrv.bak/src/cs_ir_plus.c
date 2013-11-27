/****************************************************************************/
/*
 * Filename:        cs_ir_plus.c
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

#include "cs_ir_plus.h"

/**
@brief 遥控器初始化，用户需要指定遥控器驱动的任务优先级。

@param[in] nPriority  指定遥控器驱动的任务优先级。
@return 
- CSIR_SUCCESS：遥控器初始化成功
- CSIR_ERR_ALREADY_INIT：遥控器之前初始化过了。
- CSIR_ERR_BAD_PARAMETER：输入参数错误。
@note 优先级0-255
*/
CSIR_Error_t CSIRInitialize( int nPriority )
{
    CSIR_Error_t Retcode = CSIR_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief 遥控器初始化，用户需要指定遥控器驱动的任务优先级和遥控器线程的栈大小。

@param[in] nPriority 指定遥控器驱动的任务优先级。
@param[in] nStackSize 指定遥控器驱动的任务的栈大小。
@return
- CSIR_SUCCESS：遥控器初始化成功
- CSIR_ERR_ALREADY_INIT：遥控器之前初始化过了。
- CSIR_ERR_BAD_PARAMETER：输入参数错误。
@note 优先级0-255
*/
CSIR_Error_t CSIRInitialize_Ext( int nPriority, int nStackSize )
{
    CSIR_Error_t Retcode = CSIR_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief 增加支持的遥控器型号，需要指定遥控器的协议和用户码。
@param[in] ptIREquip 指向遥控器数据结构的指针，结构成员包括遥控器协议、用户码、按键数和扫描码与功能码的映射表。
@param[out] phIRHandle 返回遥控器的句柄。
@return
- CSIR_SUCCESS：添加遥控器成功。
- CSIR_ERR_EQUIPMENT_EXIST：设备已经存在。
- CSIR_ERR_EQUIPMENT_FULL：设备列表已满。
- CSIR_ERR_BAD_PARAMETER：输入参数有误。
@note 用户最多可以添加MAX_IR_EQUIPMENT种遥控器。 
@note 如果遥控器协议中没有定义数据(如UserId、KeyCode)的字节序则默认按little-endian方式处理，即如果内存中的数据为0x12|0x34则UserId=0x3412
@note 这种处理方式即认为数据是从0位开始依次被接收的，即16位的数据接收顺序为:D0 D1 D2...D15
@note 目前已知的存在字节序的协议有:
- NEC协议在实际应用中经常将UserId扩展到16位(放弃取反冗余)，则按此规则低位在前即原UserId字节作为低位，UserId反码字节作为高位，这与NEC协议的描述是一致的
- TopwayI协议中的UserId长4字节，按此规则低位在前，也与其定义了C0-C31的发送顺序一致
- TopwayI协议中KeyCode与UserId有相同的规则
@note 键值为遥控器固有属性，不得在其中加入其它任何内容，如UserId等。
*/
CSIR_Error_t CSIRAddEquipment( const CSIREquipList_t	* ptIREquip, HCSHANDLE* phIRHandle )
{
    CSIR_Error_t Retcode = CSIR_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}


/**
@brief 删除支持的遥控器设备，需要指定添加设备时返回的句柄。
@param[in] hIRHandle 指向添加设备时返回的遥控器的句柄。
@return
- CSIR_SUCCESS：删除遥控器设备成功。
- CSIR_ERR_BAD_PARAMETER：输入参数有误。
- CSIR_FAILURE: 操作失败
*/
CSIR_Error_t CSIRDelEquipment(HCSHANDLE hIRHandle )
{
    CSIR_Error_t Retcode = CSIR_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}


/**
@brief 设置在按键持续按下时，是否产生按键重复

@param[in] nLogicValue 要设置的按键的功能码
@param[in] bIsRepeat TRUE表示产生按键重复，FALSE表示禁止产生按键重复
*/
void CSIRSetSingleKeyRepeat( int nLogicValue, BOOL bIsRepeat )
{
    CSIR_Error_t Retcode = CSIR_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
}

/**
@brief 注册遥控器按键回调函数。

该函数供测试用，用户可以不需要加入设备就可以获取遥控器的回调信息。 
@param[in] DebugCallback 指向回调函数的函数指针，用户通过回调函数得到遥控器句柄和按键来源、状态和功能码
@return CSIR_SUCCESS：注册成功。CSIR_ERR_BAD_PARAMETER：输入参数错误。
*/
CSIR_Error_t CSIRRegisterDebugCallback( CSHDICallback DebugCallback )
{
    CSIR_Error_t Retcode = CSIR_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief注册遥控器按键回调函数。红外遥控驱动将按键扫描码通过注册的回调函数通知应用层。

@param[in] KeySendCallback 指向回调函数的函数指针，用户通过回调函数得到遥控器句柄和按键来源、状态和功能码，详见CSHDICallback定义。
@return CSIR_SUCCESS：注册成功;CSIR_ERR_BAD_PARAMETER：输入参数错误。
@note
*/
CSIR_Error_t CSIRRegisterKeySendCallback( CSHDICallback KeySendCallback )
{
    CSIR_Error_t Retcode = CSIR_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief 设置遥控器的反应速度。

@param[in] enmIRProtocolType 要设置的遥控器协议。 
@param[in] enmIRRepeatType 设置repeat的类型:从press到repeat 还是repeat到repeat。
@param[in] nRepeatTimes 设置检测到repeat消息的次数。默认值：
- Nec协议：press到repeat 2;repeat到repeat 0；
- Philip RC5协议: press到repeat 4;repeat到repeat 1；
- Philip RC6协议: press到repeat 2;repeat到repeat 1；
- Topway 协议：press到repeat 2;repeat到repeat 0；
@return CSIR_SUCCESS:设置成功；CSIR_ERR_BAD_PARAMETER:参数错误
*/
CSIR_Error_t CSIRSetRepeatSpeed(CSIRProtocol_t enmIRProtocolType, CSIRRepeatType_t enmIRRepeatType,int nRepeatTimes)
{
    CSIR_Error_t Retcode = CSIR_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief 设置驱动解码的遥控器协议

@param[in] enmProtocolIndex  指定的遥控器协议类型
@return CSIR_SUCCESS：设置成功 ；CSIR_FAILURE：设置失败
@note 该接口仅为部分不能自动识别协议的平台所设，仅能在初始化之前调用，初始化后驱动只能支持最后一次设置的协议；初始化之后该接口无效
@note 对于可以自动识别协议的平台，该接口无效
*/
CSIR_Error_t  CSIRSelectProtocol(CSIRProtocol_t  enmProtocolIndex)
{
    CSIR_Error_t Retcode = CSIR_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief get nkeyNum keys during dwTimeOut ms

@param[out] pKeyValue Logic value of key to detect
@param[in] dwTimeOut time out (ms)
@return CSIR_SUCCESS：设置成功 CSIR_FAILURE：设置失败
*/
CSIR_Error_t CSIRDetectKey(DWORD *pKeyValue, DWORD dwTimeOut)
{
    CSIR_Error_t Retcode = CSIR_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief关闭红外遥控接收功能，

调用该函数后，将不再响应遥控器按键
@return CSIR_SUCCESS：设置成功。
*/
CSIR_Error_t CSIRDisable ( void )
{
    CSIR_Error_t Retcode = CSIR_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief 打开红外遥控接收功能

只在CSIRDisable之后调用，用于恢复红外遥控接收功能。
@return CSIR_SUCCESS：设置成功。
*/
CSIR_Error_t CSIREnable( void )
{
    CSIR_Error_t Retcode = CSIR_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief 获取遥控器源码的版本号。

@param[in] pchVer 用于存放版本信息
@param[in] nSize 版本信息的长度
@return 返回版本长度
@note 版本格式为："[Module Name]-[Module Type]-[Version Number]-[Date]-[Time]\n"
*/
int CSIRGetVersion( PCHAR pchVer, int nSize )
{
    int Retcode = 0;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}


