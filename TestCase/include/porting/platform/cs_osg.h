/** @addtogroup CSOSG
@{
*/
#ifndef _CS_OSG_H_
#define _CS_OSG_H_

#include "cs_device.h"
#include "udi_osg.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief 定义矩形的结构体*/
typedef struct _CSOSGRect
{
  	int left; 
  	int top;
  	int Width;
  	int Height;
}CSOSGRect;

/**@brief 定义位图格式的结构体*/
typedef struct _CSOSGPixelFormat {
  	BYTE		m_ucRloss;			/**< 红色分量所占BIT位的剩余位数*/
 	BYTE		m_ucGloss;			/**< 绿色分量所占BIT位的剩余位数*/
 	BYTE		m_ucBloss;			/**< 蓝色分量所占BIT位的剩余位数*/
	BYTE		m_ucAloss;			/**< 透明因子分量所占BIT位的剩余位数*/
	
	BYTE		m_ucRshift;			/**< 红色分量距离最低位的位移*/
	BYTE		m_ucGshift;			/**< 绿色分量距离最低位的位移*/
	BYTE		m_ucBshift;			/**< 蓝色分量距离最低位的位移*/
	BYTE		m_ucAshift;			/**< 透明因子分量距离最低位的位移*/
	
	DWORD		m_dwRmask;			/**< 红色分量的掩码*/
	DWORD		m_dwGmask;			/**< 绿色分量的掩码*/
	DWORD		m_dwBmask;			/**< 蓝色分量的掩码*/
	DWORD		m_dwAmask;			/**< 透明因子分量的掩码*/
	
	DWORD		m_dwColorkey;			/**< 指示对应颜色为透明色 */	
	BYTE		m_ucBitsPerPixel;		/**< 每个象素点所使占用的比特数，可以为1，2，4，8，15，16，24，32*/
	BYTE		m_ucBytesPerPixel;		/**< 每个象素点所占用的字节数，可以为1，2，3，4*/
} CSOSGPixelFormat_S;

/**
@brief 初始化系统图形处理模块 

@param[in] enmLAYERDeviceId 设备类型，这里取值为CSLAYER_GFX1 （用于HD显示）或者 CSLAYER_GFX2（用于SD显示）
@return 成功返回CSOSG_SUCCESS,失败返回为CSOSG_FAILURE;
@note 初始化之后，会同时创建图形设备以及分配显存
*/
int CSOSGInit(CSLAYERDeviceType enmLAYERDeviceId);

/**
@brief 创建一个显示缓存窗口或者显示区域。

若bDualBuffer为TRUE，创建一个显示缓存窗口，否则只创建一个显示区域。	
@param[in] enmLAYERDeviceId 设备类型，这里取值为CSLAYER_GFX1 （用于HD显示）或者 CSLAYER_GFX2（用于SD显示）
@param[in] dwUserGraphicsIndex 要创建的内存缓存显示窗口索引，可支持零到多个显示缓存，该参数用来标识是缓存序号。
@param[in] pstRegion 指向一个坐标区域，缓冲区域的初始化需要根据该参数来决定，如果改参数为NULL，则按照{0,0,720,576}或者{0,0,1920,1080}(高清)来初始化。
@param[in] bDualBuffer  是否创建缓存，若为FALSE则认为不需要缓存。
@return 成功返回CSOSG_SUCCESS,失败返回为CSOSG_FAILURE;
@note 该接口可多次调用，不过同一个enmGraphicsDevice，仅第一次调用的时候创建显存。
*/
int CSOSGOpen(CSLAYERDeviceType enmLAYERDeviceId,DWORD dwUserGraphicsIndex,const CSOSGRect* pstRegion,BOOL bDualBuffer);

/**
@brief 获取指定缓存的内存地址

若bDualBuffer为TRUE，创建一个显示缓存窗口，否则只创建一个显示区域。	
@param[in] enmLAYERDeviceId 设备类型，这里取值为CSLAYER_GFX1 （用于HD显示）或者 CSLAYER_GFX2（用于SD显示）
@param[in] dwUserGraphicsIndex 内存缓存显示窗口索引,若不存在缓存则返回显示区域的内存地址。
@return 成功返回指定缓存的内存地址,失败返回为空指针NULL;
*/
BYTE* CSOSGGetScreenAddress(CSLAYERDeviceType enmLAYERDeviceId, DWORD dwUserGraphicsIndex);

/**
@brief 缓存数据拷贝

将输入的欲显示的缓存的指定区域拷贝到显示区域中去，用于显示图像。
@param[in] enmLAYERDeviceId 设备类型，这里取值为CSLAYER_GFX1 （用于HD显示）或者 CSLAYER_GFX2（用于SD显示）
@param[in] dwUserGraphicsIndex 内存缓存显示窗口索引。
@param[in] pstRegion  指向一个坐标区域，用来指示需要拷贝的数据区域。
@return 
@note 
*/
void  CSOSGBufferExchange(CSLAYERDeviceType enmLAYERDeviceId, DWORD dwUserGraphicsIndex,CSOSGRect* pstRegion);

/**
@brief 用指定颜色在指定的坐标点画一个点。

@param[in] enmLAYERDeviceId 设备类型，这里取值为CSLAYER_GFX1 （用于HD显示）或者 CSLAYER_GFX2（用于SD显示）
@param[in] dwUserGraphicsIndex 仅用来兼容原有接口，并无实际意义。
@param[in] bDisplay : 为TRUE表示显示enmGraphicsDevice对应的设备,为FALSE表示隐藏enmGraphicsDevice对应的设备
@return 成功返回CSOSG_SUCCESS,失败返回CSOSG_FAILURE
@note 
*/
int CSOSGShow(CSLAYERDeviceType enmLAYERDeviceId, DWORD dwUserGraphicsIndex, BOOL bDisplay);

/**
@brief 设定指定显存和缓存的像素模式

@param[in] enmLAYERDeviceId 设备类型，这里取值为CSLAYER_GFX1 （用于HD显示）或者 CSLAYER_GFX2（用于SD显示）
@param[in] stPixelFormat 要设置像素格式,由CSOSGPixelFormat_S数据结构定义的，默认为ARGB1555模式。
@return 成功返回CSOSG_SUCCESS,失败返回CSOSG_FAILURE
@note 
*/
int CSOSGSetGlobalPixelFormat(CSLAYERDeviceType enmLAYERDeviceId,CSOSGPixelFormat_S  stPixelFormat);

/**
@brief 设定系统显存像素格式模式。

@param[in] enmLAYERDeviceId 设备类型，这里取值为CSLAYER_GFX1 （用于HD显示）或者 CSLAYER_GFX2（用于SD显示）
@param[in] dwUserGraphicsIndex dwUserGraphicsIndex
@param[in] stPixelFormat 要设置像素格式,由CSOSGPixelFormat_S数据结构定义的，默认为ARGB1555模式。
@return 成功返回CSOSG_SUCCESS,失败返回CSOSG_FAILURE
@note 
*/
int CSOSGSetScreenPixelFormat(CSLAYERDeviceType enmLAYERDeviceId, DWORD dwUserGraphicsIndex,CSOSGPixelFormat_S  stPixelFormat);

#ifdef __cplusplus
}
#endif

/** @} */

#endif

