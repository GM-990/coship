/**@NDS CA ICAM接口依赖
@brief 集成NDS CA时，对ICAM模块的封装

@version 2.0.1 2010/02/23 UDI2初稿
@{
*/

#include "udi2_typedef.h"
#include "udi2_error.h"

typedef enum
{
	CSUDIICAM_XConnectionType_TunnerPlay,    
	CSUDIICAM_XConnectionType_TunnerRecord,  
	CSUDIICAM_XConnectionType_DiskPlay,    
	CSUDIICAM_XConnectionType_DiskRecord  
}CSUDIICAMXConnectionType_E;

typedef enum
{
	CSUDIICAM_DMXOutPutType_PLAY   = 0x01,   //直播
	CSUDIICAM_DMXOutPutType_RECORC = 0x02,   //录制
	CSUDIICAM_DMXOutPutType_CAT    = 0x04,   //特殊处理
}CSUDIICAMDmxOutputType_E;

/**
@brief NDS ICAM初始化

@param:无
@return 成功返回CSUDI_SUCCESS;失败返回CSUDI_FAILURE
*/
int CSUDIICAMInit(void);

/**
@brief 建立连接，播放节目时调用该函数
@(目前在BCM平台上遇到要启用此函数，如果其它平台，直接返回CSUDI_SUCCESS)

@param[in]:
@ulXConnection     : 连接通道号
@emDmxType         : 当前是直播还是录制(预留，暂时不用)
@wDemuxId          : 目前在Boardcom平台有用到，为inputband
@ulParserBand      : 目前在Boardcom平台有用到，为parserband
@nHwModule         : 解扰通道句柄
@emXConnectionType : 枚举类型
@param[out]:无
@return 成功返回CSUDI_SUCCESS;失败返回CSUDI_FAILURE
*/
int CSUDIICAMOpenXconnection(
      unsigned long         ulXConnection,
      EM_DmxOutputType      emDmxType,
      unsigned short        wDemuxId,
      unsigned long         ulParserBand,
      int                   nHwModule,      
      EM_XConnectionType    emXConnectionType
);

/**
@brief 断开连接，停止节目播放时调用该函数
@(目前在BCM平台上遇到要启用此函数，如果其它平台，直接返回CSUDI_SUCCESS)

@param[in]:
@ulXConnection     : 连接通道号
@param[out]:无
@return 成功返回CSUDI_SUCCESS;失败返回错误代码
*/
int CSUDIICAMCloseXconnection(unsigned long  ulXConnection);

/**
@brief 获取Chip Id

@param[in/out]:
@paucChipData : 上层分配空间，空间不小于8个字节，用于存储Chip Id数据(前面4个字节为chip id)
@pnLen        : 返回实际获取到的数据长度
@return 成功返回CSUDI_SUCCESS;失败返回CSUDI_FAILURE
*/
int CSUDIGetChipId(unsigned char *paucChipData, int *pnLen);

/**
@brief 获取Device Id

@param[in/out]:
@paucDeviceData : 上层分配空间，空间不小于8个字节，用于存储Device Id数据
@pnLen        : 返回实际获取到的数据长度
@return 成功返回CSUDI_SUCCESS;失败返回CSUDI_FAILURE
*/
int CSUDIGetDeviceId(unsigned char *paucDeviceData, int *pnLen);

