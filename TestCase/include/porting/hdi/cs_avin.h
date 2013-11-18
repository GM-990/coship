/**@defgroup CSI2C[CSI2C]同洲项目专用AVIN驱动接口
@brief 本模块主要定义了同洲项目专用AVIN驱动接口
@brief av模拟信号转换成数字信号经过mpeg2压缩，最后打包成ts流输入到主芯片
 

@version 0.2 2010/01/06 评审后的第一个版本
@version 0.1 2010/01/05演示版本
@{
*/

#ifndef _CS_AVIN_H_
#define _CS_AVIN_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "udi_hditypedef.h"


/**@brief 目前支持对路径设置或获取以下参数*/
typedef enum
{    
	CSAVIN_MUTE,			/**< 用于Mute/Unmute 某个通道的Audio */
	CSAVIN_MACROVISION	/**< allows host software to get information about the macrovision type of the current video signal. */
} CSAVINPathParam_E;


/**@brief 参数CSAVIN_MUTE对应的数据结构*/
typedef struct
{
	BOOL m_bMute;	/**< False Unmute,TRUE mute*/
}CSAVINMute_S;


/**@brief 参数CSAVIN_MACROVISION对应的数据结构*/
typedef struct
{
	UINT16 m_wMacrovisionType;	/**< 0: Not_Protected*/
								/**< 1: Level1_Protected(Copy protected; no color stripe process present, typically VCR-tapes and DVD's)*/
								/**< 2: Level2_Protected(Copy protected with color stripe burst (two line version), typically DVD's)*/
								/**< 3: Level3_Protected(Copy protected with color stripe burst type3, typically DVD's)*/
								/**< 4: Level2_Cracked(Not protected, but color stripe burst still present, typically for cracked level2 sources)*/
								/**< 5: Level3_Cracked(Not protected, but color stripe burst type3 still present, typically for cracked level3 sources)*/	
}CSAVINMacroVision_S;


/**
@brief  初始化ANIN模块并自动设置各路参数,必须首先调用

@param[in] eAVINIndex: AVIN索引号
@param[in] eI2cIndex: I2c索引号
@return: 成功返回CSHDI_SUCCESS，其它值失败，请参看枚举CSHDIComErrType所列常量及打印信息
*/
CSHDI_Error_t CSAVINInitialize(CSHDIAVINIndex_E eAVINIndex, CSHDII2CIndex eI2cIndex);


/**
@brief  设置某一路的参数

@param[in] eAVINIndex: AVIN索引号
@param[in] nPathIndex: path索引
@param[in] eParam: 参见CSAVINPathParam_E定义,指示要设置的是哪一项参数
@param[in] pvParam: 指向参数的具体值
@return: 成功返回CSHDI_SUCCESS，其它值失败，请参看枚举CSHDIComErrType所列常量及打印信息
@note:CSAVINInitialize会自动设置好各路参数,如果需要特殊化的设置才需要调用该函数
*/
CSHDI_Error_t CSAVINSetPathParam(CSHDIAVINIndex_E eAVINIndex, int nPathIndex, CSAVINPathParam_E eParam, void* pvParam);


/**
@brief  获取某一路的参数

@param[in] eAVINIndex: AVIN索引号
@param[in] nPathIndex: path索引
@param[in] eParam: 参见CSAVINPathParam_E定义,指示要获取的是哪一项参数
@param[out] pvParam: 指向参数的具体值
@return: 成功返回CSHDI_SUCCESS，其它值失败，请参看枚举CSHDIComErrType所列常量及打印信息
*/
CSHDI_Error_t CSAVINGetPathParam(CSHDIAVINIndex_E eAVINIndex, int nPathIndex, CSAVINPathParam_E eParam, void* pvParam);


/**
@brief  启动指定ANIN设备的某一通道

@param[in] eAVINIndex: AVIN索引号
@param[in] nPathIndex :path索引
@return : 成功返回CSHDI_SUCCESS，其它值失败，请参看枚举CSHDIComErrType所列常量及打印信息
*/
CSHDI_Error_t CSAVINStartPath(CSHDIAVINIndex_E eAVINIndex, int nPathIndex);


/**
@brief  停止指定ANIN设备的某一通道

@param[in] eAVINIndex: AVIN索引号
@param[in] nPathIndex :path索引
@return  : 成功返回CSHDI_SUCCESS，其它值失败，请参看枚举CSHDIComErrType所列常量及打印信息
*/
CSHDI_Error_t CSAVINStopPath (CSHDIAVINIndex_E eAVINIndex, int nPathIndex);


/**
@brief  获取AVIN程序模块的版本号

@param[in]  nSize 缓冲区长度
@param[out]  pcVer  存放版本信息的缓冲区首地址
@return  1> 如果第一个参数pchVer为空，返回版本信息的实际长度
  	     2> 如果第一个参数不为空，且函数调用成功，则返回实
  	     际获取的版本信息长度
  	     3> 如果失败，返回0	
*/
int CSAVINGetVersion(PCHAR pcVer, int nSize);


#ifdef __cplusplus
}
#endif


#endif /* !_CS_AVIN_H_ */

