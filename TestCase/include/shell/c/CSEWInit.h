/**@defgroup EW200Init EW200Init EW200初始化接口

@brief 本模块为EW200中间件的初始化配置接口，一般来说集成方直接使用MMCPInit即可，无需关注本模块。
@note 整个EW200依赖UDI,部分未标注申明,请参见UDI文档
@{
*/
#ifndef _CS_EW_INIT_H_
#define _CS_EW_INIT_H_

#include "udi2_typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
@brief 定义OC URL的编码方式
一个OC的URL形如:dvb://11.12.13/index.html
用本结构指定这里的11是10进制(表示11)还是16进制(表示17)
@see CSEWInitOc
*/
typedef enum
{
    EW_OC_TYPE_HEX,      ///< 十六进制
    EW_OC_TYPE_DEC       ///< 十进制
}
EWOcType_E;

/**
@brief 定义VOD接入模式
@see CSEWVODInitCITV3
*/
typedef enum
{
    EM_EW_ITV_MODE_IPTV,		///< iptv
    EM_EW_ITV_MODE_IPQAM,		///< 双向
    EM_EW_ITV_MODE_DVBIP,		///< 双模
    EM_EW_ITV_MODE_MAX
} CSEWMODE_E;

/**
@brief 定义服务器版本号
@see CSEWVODInitCITV3
*/
typedef enum
{
    EM_EW_THREE_POINT_ZERO,		///< 视讯3.0
    EM_EW_THREE_POINT_ONE     ///< 视讯3.1
} CSEWVODServerVersion_E;

/**
@brief EW200浏览器初始化参数
@see CSEWInitGtools
*/
typedef struct
{
    int                  nStructSize;            ///< 本结构体大小,必须等于sizeof(CSEWArgList_S)
    int                 nMemorySize;      ///< 浏览器的内存大小。至少10M。将分次从CSMalloc申请,用CSFree释放，但总大小不会超过本值
    int                 nScreenWidth;	  ///< 显示区域分辨率
    int                 nScreenHeight;    ///< 显示区域分辨率
    int		    nCacheBufferSize; ///< 浏览器缓存大小.缓存的大小是包含在nMemorySize里面
    int 	            nX;			      ///< 初始化窗口的X位置(相对显示设备)
    int 	            nY;			      ///< 初始化窗口的Y位置(相对显示设备)
    int 	            nWidth;           ///< 初始化窗口的宽度  ,标清项目传浏览器窗口大小,高清项目传入固定的640,526,
    int 	            nHeight;	      ///< 初始化窗口的高度
} CSEWArgList_S;

/**
@brief 初始化接口

EW200浏览器初始化接口,需所有接口调用前调用.
@param[in] psArgList 初始化参数,详见结构定义
@return 成功返回CSUDI_TRUE,失败返回CSUDI_FALSE
@note 接口将对结构进行浅拷贝,函数返回后pArgList结构本身不再需要
*/
CSUDI_BOOL CSEWInitGtools(CSEWArgList_S *psArgList);

/**
@brief 用于百汇200功能选项
OC初始化接口.可选,如果需要OC，需要调用此接口。
@param[in] nOcbufferSize 分配给OC的内存大小。注意:单位为K .最小2K。这个值的大小根据系统总内存以及前端OC的复杂度来分配，推荐值:128M 的分配5K;64M分配2M或者2K以上
@param[in] eCode OC URL的编码方式。
@return CSUDI_TRUE :表示成功;CSUDI_FALSE:表示失败
*/
CSUDI_BOOL CSEWInitOc(const int nOcbufferSize , EWOcType_E  eCode);

/**
@brief 用于EW200功能选项
视讯VOD初始化接口。可选，如果需要citv3，需要调用此接口
@param[in] eMode vod模式，参见CSEWMODE_E定义
@param[in] eServerVer 前端服务器版本号 ，参见CSEWVODServerVersion_E定义
@return CSUDI_TRUE:表示成功;CSUDI_FALSE:表示失败
*/
CSUDI_BOOL CSEWVODInitCITV3(CSEWMODE_E eMode , CSEWVODServerVersion_E eServerVer);

/**
@brief 用于EW200功能选项
视讯VOD初始化接口。可选，如果使用第三方浏览器，同时集成citv3，需要调用此接口
@note 如果使用第三方浏览器，需要自己去识别协议头及相关JS，详情请参考集成文档。
@param[in] eMode vod模式，参见CSEWMODE_E定义
@param[in] eServerVer 前端服务器版本号 ，参见CSEWVODServerVersion_E定义
@return CSUDI_TRUE:表示成功;CSUDI_FALSE:表示失败
*/
CSUDI_BOOL CSEWVODInitCITV3NoGT(CSEWMODE_E eMode , CSEWVODServerVersion_E eServerVer);

/**
@brief 用于EW200功能选项
视讯VOD初始化接口。可选，如果只需要集成视讯vod的sa认证过程
@param[in] eMode vod模式，参见CSEWMODE_E定义
@param[in] eServerVer 前端服务器版本号 ，参见CSEWVODServerVersion_E定义
@return CSUDI_TRUE:表示成功;CSUDI_FALSE:表示失败*/
CSUDI_BOOL CSEWCITVInitSA(CSEWMODE_E eMode ,CSEWVODServerVersion_E eServerVer);

/**
@brief 用于EW200功能选项
视讯音秀初始化接口。
@param[in] eMode vod模式，参见CSEWMODE_E定义
@param[in] eServerVer 前端服务器版本号 ，参见CSEWVODServerVersion_E定义
@return CSUDI_TRUE:表示成功;CSUDI_FALSE:表示失败*/
CSUDI_BOOL CSEWInitInShowMusic();

#ifdef __cplusplus
}

#endif
/** @} */
#endif//_CS_EW_INIT_H_
