/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_os.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are NOT required for Android DVB-S2 plus OTT project.
		 Because there is no EEPROM device in the STB system.
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "udi2_osg.h"
/**
@brief 创建一个绘图平面

@param[in] ePixelFormat 绘图平面的颜色模式。
@param[in] nWidth  绘图平面的宽度，必须大于 0。
@param[in] nHeight 绘图平面的高度，必须大于 0。
@param[out] phSurface 成功返回绘图平面的句柄，失败为空，详细错误参见错误代码值。
@return 成功返回 CSUDI_SUCCESS,否则返回错误代码值。 
@note 

- 该接口创建显示缓存空间，理论上可以支持无限大，支持创建无限多个直到内存不足，(建议提供16M的内存用于创建Surface)，具体需要几个Surface根据项目需求的不同而不同，目前应用用到比较多的是:\n
     1、游戏应用中的可变图片会用到此Surface。 \n
     2、Java应用中用到BufferedImage用到此Surface。\n 
     3、2D特效的实现会用到此Surface)。 \n
- 此处颜色模式可以与最终显示的颜色模式不同，在Blit时做特殊处理即可，如果平台不支持这样的处理也可在应用选择颜色模式时加以避免。\n
- 如果平台资源实在有限，也可允许不能创建任何Surface，这种情况下会直接操作显存，但必须要求界面与显存的分辨率，颜色模式等保持一致。
- 如果硬件不支持再创建Surface时，建议返回失败。这时中间件会模拟创建surface并进行操作。
*/
CSUDI_Error_Code CSUDIOSGCreateSurface(CSUDIOSGPixelFormat_E ePixelFormat, int nWidth, int nHeight, CSUDI_HANDLE * phSurface)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief 销毁一个绘图平面，释放该绘图平面的内存。

@param[in] hSurface CSUDIOSGCreateSurface 成功返回的句柄。
@return 成功返回 CSUDI_SUCCESS，否则返回错误代码值 
@note 
- 由CSUDIOSGCreateSurface创建的缓存区域不使用后，一定要调用此接口销毁释放。\n
- 尝试销毁一个显存空间，将返回CSUDIOSG_ERROR_BAD_PARAMETER。
*/
CSUDI_Error_Code CSUDIOSGDestroySurface(CSUDI_HANDLE hSurface)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	 
	return Retcode;
}


/**
@brief 得到显存surface句柄。

@param[in] nIndex 显存索引，从0开始直到没有更多的显存，返回失败为止。
@param[out] phSurface 成功返回显存Surface句柄。

@return 成功返回 CSUDI_SUCCESS,否则返回错误代码。

@note 
- 这种方式得到的Surface不能使用CSUDIOSGDestroySurface销毁。\n
- 如果高标清同时支持，nIndex 为 0 表示标清显存；nIndex 为 1 表示高清显存。\n
- 只需要返回所有用于完整OSD显示的显存,如标清显存，高清显存，不要返回特殊层显存如光标层，背景层等。\n
- 除非不能创建任何其他Surface，否则中间件一般不会直接操作显存地址进行绘制,该句柄多数情况下只会用来往上进行Blit。
*/
CSUDI_Error_Code CSUDIOSGGetDisplaySurface(int nIndex, CSUDI_HANDLE * phSurface)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 获取绘图平面或者显存空间的信息。

中间件会通过该接口获取的信息进行绘图操作。
@param[in] hSurface 绘图平面或者显存空间句柄.CSUDIOSGCreateSurface或者CSUDIOSGGetDisplaySurface的返回值。
@param[out] psSurfaceInfo 保存得到的绘图平面或者显存空间信息。
@return 成功返回 CSUDI_SUCCESS，否则返回错误代码值 。
@note
- 此接口能够得到显存或者缓存的空间地址
- Alpha超过0xFF返回CSUDIOSG_ERROR_BAD_PARAMETER
- 若不支持0xFF级透明度，需驱动层自行将0-255映射到所支持的范围内，不得仅处理支持部分。
如某平台仅支持0-127的透明度，则该值传入255应相当于127；传入127应相当于63
*/
CSUDI_Error_Code CSUDIOSGGetSurfaceInfo(CSUDI_HANDLE hSurface, CSUDIOSGSurfaceInfo_S * psSurfaceInfo)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 设置绘图平面的信息，只能设置绘图平面的m_nAlphaFlag、m_uAlpha和m_nColorKeyFlag、m_uColorkey四个成员，对其他成员操作无效。

@param[in] hSurface 绘图平面句柄.CSUDIOSGCreateSurface返回值。
@param[in] psSurfaceInfo 保存得到的绘图平面或者显存空间信息。
@return 成功返回 CSUDI_SUCCESS，否则返回错误代码值 。
@note 
- 此接口在CSUDIOSGGetSurfaceInfo后使用，只能设置绘图平面的信息，不能设置显存空间的信息。\n
- 此接口在仅仅用于改变Surface的m_uAlpha和m_uColorkey属性，并不会对显示产生变化。\n
- 当psSurfaceInfo中同时有除m_nAlphaFlag、m_uAlpha和m_nColorKeyFlag、m_uColorkey以外的成员改变时，其他成员修改操作无效，仅仅对m_nAlphaFlag、m_uAlpha和m_nColorKeyFlag、m_uColorkey生效，返回CSUDI_SUCCESS。
- m_uAlpha超过0xFF返回CSUDIOSG_ERROR_BAD_PARAMETER
- 若不支持0xFF级透明度，需驱动层自行将0-255映射到所支持的范围内，不得仅处理支持部分。
如某平台仅支持0-127的透明度，则该值传入255应相当于127；传入127应相当于63
*/
CSUDI_Error_Code CSUDIOSGSetSurfaceInfo(CSUDI_HANDLE hSurface, const CSUDIOSGSurfaceInfo_S * psSurfaceInfo)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief Blit操作接口，以利用硬件加速性能，异步操作。

@param[in] hDstSurface 目标绘图平面或者显存空间句柄.CSUDIOSGCreateSurface或者CSUDIOSGGetDisplaySurface的返回值。
@param[in] psDstRect 目标surface或显示空间hDstSurface上矩形区域,如果为空，则取hDstSurface的全部区域。
@param[in] hSrcSurface 源绘图平面或者显存空间句柄。
@param[in] psSrcRect 源surface或显示空间hSrcSurface上矩形区域,如果为空，则取hSrcSurface的全部区域。
@param[in] eOpMode 混合模式，见CSUDIOSGOpMode_E定义。
@return 成功返回 CSUDI_SUCCESS，其它返回错误代码。
@note 
- 如果是alpha混合模式，全局alpha值根据源Surface和目的Surface的alpha flag 确定，如果源和目的Surface的alpha flag全为1，则都使用.混合时的alpha值根据
      三者比例得出，例如:如果只有源的alpha flag 为1， 混合的alpha为 Sa * Pa / 255 （Sa为源surface的全局alpha，Pa为源surface的一个像素的alpha）
- 如果硬件不支持，不需要软件模拟实现，直接返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED
- 如果不支持硬件加速请返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED，即除非有更高效的实现方式或者其它充分的理由,否则不要对该操作进行软件模拟。\n
- 源和目标的区域大小psSrcRect、psDstRect可以为空，但是非空时，必须是对应surface上大小的范围内，不支持裁剪则返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED。\n
- 源和目标均可能为显存Surface或者普通Surface，若不支持请返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED。\n
- 源和目标可能为同一个Surface，若不支持请返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED。\n
- 源和目标可能为同一个Surface时，psDstRect和psSrcRect不允许重叠，否则返回CSUDIOSG_ERROR_BAD_PARAMETER。\n
- 源和目标的颜色模式可能不一致，一般不推荐这种使用，若不支持可返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED。\n
- 源和目标可能分别是YUV和RGB的颜色模式，若不支持可返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED。\n
- 源和目标矩形的大小可能不一致,若不支持可返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED,但一般来说高清平台必须支持，软件的缩放效率极低，中间件不会提供。\n
- 源和目标的矩形可能有缩放、拉伸等处理,若不支持请返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED，需支持2D特效的平台必须实现该功能。\n
- 为充分提高绘图性能，该接口可实现成异步模式，通过CSUDIOSGSync确保该操作执行完毕。\n
- 在CSUDIOSGSync返回前改变相关Surface，绘图效果是不确定的，但不能死机或产生其他不可恢复的异常。\n
- 如果平台同时支持高标清显示，要求在向标清显存blit时，一定要设置防闪(或者防锯齿)操作。\n
- 平台支持的拉伸和缩放比例至少为: 1:15。\n
- 进行blit操作时，只有源的colorkey有效，即当源绘图平面的colorkey有效时，源平面所有颜色为colorkey的像素均不会被blit到目标平面。
*/
CSUDI_Error_Code CSUDIOSGBlit(CSUDI_HANDLE hDstSurface, const CSUDIOSGRect_S * psDstRect,
                        CSUDI_HANDLE hSrcSurface, const CSUDIOSGRect_S * psSrcRect, CSUDIOSGOpMode_E eOpMode)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief 填充目标绘图平面或者显存空间上一块矩形。

@param[in] hDstSurface 绘图平面或者显存空间句柄.CSUDIOSGCreateSurface或者CSUDIOSGGetDisplaySurface的返回值。
@param[in] psDstRect 要填充在目标绘图平面上的位置和大小，为空时，表示全屏填充。
@param[in] uPixel 要填充的颜色值，ARGB8888格式的像素值。
@return 成功返回 CSUDI_SUCCESS，否则返回错误代码值。
@note 
- 为充分提高绘图性能，该接口需要实现成异步模式，通过CSUDIOSGSync确保该操作执行完毕，在CSUDIOSGSync返回前改变相关Surface，绘图效果是不确定的，但不能死机或产生其他不可恢复的异常。\n
- 必须使用硬件加速Fill,如果硬件不支持，请返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED,即除非有更高效的实现方式或者其它充分的理由,否则不要对该操作进行软件模拟。\n
- psDstRect 必须在hDstSurface的大小范围内，如果超过hDstSurface，则返回CSUDIOSG_ERROR_BAD_PARAMETER。
- 若hDstSurface不是ARGB8888格式的则需在移植层完成转换工作。
*/
CSUDI_Error_Code CSUDIOSGFill(CSUDI_HANDLE hDstSurface, const CSUDIOSGRect_S * psDstRect, unsigned int uPixel)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 同步CSUDIOSGBlit，CSUDIOSGFill等OSG操作。

@return 成功返回 CSUDI_SUCCESS，否则返回错误代码值。
@note 
- 在CSUDIOSGBlit，CSUDIOSGFill等之后调用,函数返回时，前述操作全部完成。\n
- 该接口并不是显示Surface上的内容。 \n
- 如果CSUDIOSGBlit和CSUDIOSGFill是同步的操作，建议该接口放空，直接返回成功。 \n
- 中间件利用软件操作Surface的内存时，也会用此接口同步Surface的内存，为了保证操作成功，防止数据处于cache状态，如果该芯片
- 无此cache问题，则建议放空，直接返回成功。
- 中间件在画图过程中会频繁使用此接口。
- 该接口与Blit的关系可参考C99标准fflush。
*/
CSUDI_Error_Code CSUDIOSGSync(void)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 硬件解码图片,包括I帧格式(MPEG,BMP)/JPEG格式/PNG格式等。

@param[in] pszImageType 图片的类型,使用字符串避免了定义多的枚举类型。格式与类型的定义: IFRAME - I帧格式，BMP - bmp格式, JPEG - jpeg格式, PNG - png格式
@param[in] pvImageMem  图片数据地址。
@param[in] nMemSize   图片数据大小。
@param[in] eScalingMode 解码输出模式，控制解码的缩放
@param[in] psDstRect 解码输出Surface的大小，仅宽高数据有效，根据eScalingMode的不同该值的效果有所不同
@param[out] phSurface 解码后的数据存放的surface。
@return 成功返回 CSUDI_SUCCESS,否则返回错误代码。
@note 
- 必须实现对MPEG格式的硬件解码，其他格式允许不支持，此时返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED。
- 本接口将创建一个绘图平面，通过phSurface返回给上层使用，在使用完之后，必须调用CSUDIOSGDestroySurface释放。
- 如果本接口解码图片(例如I帧)用到的设备处于忙状态，请返回CSUDIOSG_ERROR_DEVICE_BUSY。
- 解码I帧一般需要用到Video解码器，会与视频播放冲突，本接口应尽量使用闲置Video解码器进行解码，若实在找不到闲置解码器则返回CSUDIOSG_ERROR_DEVICE_BUSY，
此情况下EastWin如必须要进行解码，则应通知上层暂停视频播放后再次调用该接口。
- 本接口仅适用于可以通过硬件方式加速解码的情况，除非有非常充分的理由，否则请不要进行任何软件模拟实现。
@note eScalingMode与psDstRect的关系
- EM_UDIOSG_SCALE_FULLSIZE：psDstRect不生效，可以为CSUDI_NULL，输出的Surface宽高与原始图片保持一致
- EM_UDIOSG_SCALE_OPTIMUM：最适合缩放，图片在保持完整和比例的前提下，尽量大的放置在psDstRect区域中，但不留边。\n
即当比例不一致时输出Surface的大小会在某一个方向（宽或高）比psDstRect小。\n
如：图片原始大小为1000*500，给定的区域为100*100，则输出的Surface大小为100*50，图片按比例占满整个Surface。
- EM_UDIOSG_SCALE_FIXSIZE：拉升，psDstRect.m_nWidth，psDstRect.m_nHeight均有效，输出的Surface高度为psDstRect.m_nHeight，宽度为psDstRect.m_nWidth
*/
CSUDI_Error_Code CSUDIOSGDecodeImage(const char * pszImageType, const void * pvImageMem, int nMemSize,CSUDIOSGScalingMode_E eScalingMode,const CSUDIOSGRect_S * psDstRect,CSUDI_HANDLE * phSurface)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 获取3D显示必须的NativeDisplay句柄
@param[out] phNativeDisplay NativeDisplay句柄。
@return 返回错误类型。
@note 
-NativeDisplay句柄与具体平台中opengl es的实现相关。
-类似于win32的hdc
*/
CSUDI_Error_Code  CSUDIOSGGetNativeDisplay(CSUDI_HANDLE * phNativeDisplay)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 获取hSurface对应的NativeWindow句柄
@param[in]  hSurface Surface句柄
@param[out] phNativeWindow NativeWindow句柄
@return 返回错误类型。
@note 
-NativeWindow句柄与具体平台中opengl es的实现相关。
-类似于win32的window
*/
CSUDI_Error_Code CSUDIOSGGetNativeWindow (CSUDI_HANDLE hSurface, CSUDI_HANDLE *phNativeWindow)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

