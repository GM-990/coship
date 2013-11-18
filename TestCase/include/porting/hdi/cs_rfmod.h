/**@defgroup CSRFMOD [CSRFMOD]同洲项目专用RFMOD接口
@brief 本模块主要定义了同洲项目专用平台无关RFMOD驱动接口
@brief  RFMOD是射频调制模块。基于udi的MMCP中间层不会用到这个模块，但是在具体的基于MMCP的项目的配置文件中应该会用到。
 
@version 0.3 2008/11/5  第一次细化版本
@version 0.2 2008/10/17 评审后的第一个版本
@version 0.1 2008/10/16 演示版本
@{
*/

#ifndef _CS_RFMOD_H_
#define _CS_RFMOD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include  "CSHDI_typedef.h"

/**@brief 伴音制式*/
typedef enum
{
	NTSC_M,							/**< 需要添加说明 */
	PAL_BG,							/**< 需要添加说明 */
	PAL_I,							/**< 需要添加说明 */
	PAL_DK							/**< 需要添加说明 */
}AUDIOMODE ;

/**@brief 调制器类型*/
typedef enum 
{
	RFMOD_2170,					/**< 需要添加说明 */
	RFMOD_74055,				/**< 需要添加说明 */
	RFMOD_2270,					/**< 需要添加说明 */
	RFMOD_MAXTYPE				/**< 需要添加说明 */
}RFMODEType_t;

/**@brief 调制器标准*/
typedef enum
{
	RF_STANDARD_EUROPE,      /*欧洲标准*/
	RF_STANDARD_AUSTRALIA    /*澳洲标准*/
}RFSTANDARD_t;

/**
@brief 设置标准

*设置调制器使用的标准
@param[in] enmStandardType 调制器使用的标准，可选值
@return 如果成功返回CSHDI_SUCCESS;如果失败返回CSHDI_FAILURE
*/
CSHDI_Error_t CSRFVSetStandard(RFSTANDARD_t  enmStandardType);

/**
@brief 射频调制器初始化

*射频调制器初始化(伴音制式默认为DK制，初始频道为CH25)
@param[in] nIndex 关联的I2C的索引，可选值
@param[in] bRFModType 调制器类型
@param[in] bControlPinIndex 控制脚
@return 如果成功返回CSHDI_SUCCESS;如果失败返回CSHDI_FAILURE
*/
CSHDI_Error_t CSRFVInit( CSHDII2CIndex nIndex, RFMODEType_t bRFModType, BYTE bControlPinIndex);

/**
@brief 设置调谐频道

@param[in] nCHNumber 频道序号 (21-69)
@return 如果成功返回CSHDI_SUCCESS;如果失败返回CSHDI_FAILURE
*/
CSHDI_Error_t CSRFVSetChannel ( int nCHNumber ) ;

/**
@brief 设置调谐器视频载波频率

@param[in] dwFrequency 视频载波频率(以KHz为单位)
@return 如果成功返回CSHDI_SUCCESS;如果失败返回CSHDI_FAILURE
*/
CSHDI_Error_t CSRFVSetFrequency(DWORD dwFrequency) ;

/**
@brief 设置射频调制器伴音制式

@param[in] bAudioMode 伴音制式(NTSC_M, PAL_BG, PAL_I, PAL_DK)	
@return 如果成功返回CSHDI_SUCCESS;如果失败返回CSHDI_FAILURE
*/
CSHDI_Error_t CSRFVSetAudioMode(AUDIOMODE bAudioMode) ;

/**
@brief 设置射频调制器伴音制式

@param[in] bPowerState  状态(非零: 正常工作； 0: STANDBY模式)
@return 如果成功返回CSHDI_SUCCESS;如果失败返回CSHDI_FAILURE
*/
CSHDI_Error_t CSRFSetPowerState(BYTE bPowerState);

/**
@brief 获取RFMOD驱动的版本号

@param[in] nSize 要获取版本号的长度
@param[out] pchVer 要获取的版本号存放位置
@return 如果pchVer为空或者调用失败则返回0，如果pchVer为非空返回版本号信息长度
*/
int CSRFMODGetVersion( PCHAR pchVer, int nSize );

#if 0 /*满足对一个平台存在多个rf调制器的接口*/
typedef struct _CSRFModInitParam_S
{
	DWORD	dwReserved0;
	CSHDII2CIndex nIndex;					/*i2c控制索引*/
	RFMODEType_t eRFModType;			/*调谐器类型*/
	BYTE ucControlPinIndex;				/*控制索引*/
	DWORD	dwReserved1;
	DWORD	dwReserved2;
} CSRFModInitParam_S;

/***********************************************************
*  Function:	射频调制器初始化(伴音制式默认为DK制，初始频道为CH25)
*  Input:		pInitParam：初始化参数
*  Output:	需要返回调谐器的句柄
*  Return:	CSHDI_Error_t
************************************************************/
CSHDI_Error_t CSRFModInit( CSRFModInitParam_S* pInitParam,HANDLE* phRFModHndl);
/***********************************************************
*  Function:	设置调谐器视频载波频率
*  Input:		hRFModHndl:调谐器句柄
*			dwFrequency:视频载波频率(以KHz为单位)
*  Output:	无			
*  Return:	CSHDI_Error_t
************************************************************/
CSHDI_Error_t CSRFModSetFrequency(HANDLE hRFModHndl,DWORD dwFrequency) ;

/***********************************************************
*  Function:	设置伴音制式
*  Input:		hRFModHndl:调谐器句柄
*			bAudioMode:伴音制式(NTSC_M, PAL_BG, PAL_I, PAL_DK)	
*  Output:	无			
*  Return:	CSHDI_Error_t
************************************************************/
CSHDI_Error_t CSRFModSetAudioMode(HANDLE hRFModHndl,AUDIOMODE bAudioMode) ;

/***********************************************************
*  Function:	设置电源状态
*  Input:		hRFModHndl:调谐器句柄
*			bPowerState:电源状态
			-非0 正常工作
			-	0  待机
*  Output:	无			
*  Return:	CSHDI_Error_t
************************************************************/
CSHDI_Error_t CSRFModSetPowerState(HANDLE hRFModHndl,BYTE bPowerState);

#endif
#ifdef __cplusplus
}
#endif
/** @} */

#endif
