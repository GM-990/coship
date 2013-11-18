/**@defgroup CSIFRAME iframe I帧显示相关接口
@brief I帧显示相关接口

@version 0.3
- 细化I帧格式的详细说明

@version 0.2
- 添加部分详细描述和部分接口函数及结构

@version 0.1
- 初始版本
@{
*/

#ifndef _MW_IFRAME_H_
#define _MW_IFRAME_H_

#include "udiplus_typedef.h"
//#include "mwscreen.h"

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef MMCP_UDI2

/**@brief I帧显示层 */
typedef enum _CSIFrameLayerType_E
{
	CSIFAME_LAYER_BACKGROUND,		/**<  Background层*/
	CSIFAME_LAYER_STILLPICTURE,		/**<  Still 层*/
	CSIFAME_LAYER_VIDEO,			/**<  Video 层*/	
	CSIFAME_LAYER_GRAPHIC,     		/**<  OSD  层*/
	CSIFAME_LAYER_CURSOR,			/**<  光标层*/
    CSIFAME_LAYER_NUM				/**<  上限 */
}CSIFrameLayerType_E;


/**@brief I帧类型 */
typedef enum _CSIFrameSourceFormat_E
{
	CSIFRAME_DATA_MPEG2_I ,				/**< MPEG2 I frame,both ES and PES is OK*/
	CSIFRAME_DATA_BITMAP_FILE ,			/**< Bitmap文件*/
	CSIFRAME_DATA_PIXMAP		   		/**< CSGraphPixmap struct像素图*/
}CSIFrameSourceFormat_E;

/**@brief I帧存储结构 */
typedef struct _CSIFrameBuf_T
{
	PBYTE		pucData;				/**< 详见nDataSourceFormat的说明*/
	int 		nDataLen;				/**< 详见nDataSourceFormat的说明*/
	int 		nDataSourceFormat;	    /**< CSIFRAME_DATA_MPEG2_I:pucData指向MPEG格式数据地址,nDataLen数据长度;
															CSIFRAME_DATA_BITMAP_RAW:pucData指向BMP图片格式数据地址,nDataLen数据长度;
															CSIFRAME_DATA_BITMAP:pucData指向CSGraphPixmap结构, nDataLen为sizeof(CSGraphPixmap)*/
	int   		nIndex;					/**< IFrame 索引值, 不能重复,CSIFrameShow，CSIFrameTerm的第一个参数要使用*/
}CSIFrameBuf_t;


/**@brief 定义矩形的结构体*/
typedef struct _MWOSGRect
{
  	int left; 
  	int top;
  	int Width;
  	int Height;
}MWOSGRect;

/**
@brief 创建I帧数据

由上层将保存IFrame Buffer的数据地址传递给模块接口，
该Buffer是由多个I_Frame 连续构成，个数由参数nCount来告知模块。
每个I_Frame的结构定义为CSIFrameBuf_t 			
@param[in] pstUserData IFrame数据数组的首地址
@param[in] nCount IFrame数目，数据数组元素的个数
@return TRUE时返回成功，FALSE时返回失败
@note CSIFrameBuf_t结构中的pucData指向的内存是否可以释放待议，但pstUserData本身可以释放
*/
BOOL MWIFrameInit( CSIFrameBuf_t* pstUserData, int nCount );

/**
@brief 按索引值设置IFrame显示的层.

@param[in] nIndex 所要显示的I帧索引
@param[in] eLayer I帧所要显示的层
@return TRUE时返回成功，FALSE时返回失败
@note 只对当前设置的nIndex有效，如果该I帧是在显示状态，即时更新生效，如果不是在显示状态，在下次显示的时候生效
*/
BOOL MWIFrameSetDisplayLayer(int nIndex, CSIFrameLayerType_E eLayer);

/**
@brief 销毁在初始化数据数组中的索引值对应的IFrame。

此函数用来擦除和释放nIndex指示的图片。
@param[in] nIndex 要销毁的iframe在初始化数据数组中的索引值
@return TRUE时返回成功，FALSE时返回失败
@note 如果nIndex的值有错（小于0）或者指示的图片不存在，则返回FALSE。
@note 如果此时I帧在显示状态，则先隐藏，再释放
*/
BOOL MWIFrameTerm(int	nIndex);

/**
@brief 显示/隐藏 按索引值存储的IFrame

@param[in] nIndex 要显示/隐藏的iframe在初始化数据数组中的索引值
@param[in] bIsShow TRUE:显示iframe;FALSE:关闭iframe
@return 成功 TRUE;失败 FALSE
@note 在同一层显示多幅I帧，后显示的覆盖先显示的，此时隐藏后显示的I帧，
@note 先显示的不会自动恢复显示
@note 由于播放视频和I帧解码都需要用到解码器资源，如果出现解码器资源冲突，
@note Cache又没有命中的情况下，该接口可能造成视频短时间内不正常的情况
@note 但I帧应仍能成功解码、显示
*/ 
BOOL MWIFrameShow( int nIndex, BOOL bIsShow );

/**
@brief 设置相应IFRAME的某个区域的透明度

@param[in] nIndex 目标iframe在初始化数据数组中的索引值
@param[in] pRegion 区域
@param[in] nAlpha 要设置的透明度值
@return 成功 TRUE;失败 FALSE
@note 区域地址为NULL时，为全屏模式。
@note 区域大小越界时，取有效部分数据。
@note nAlpha范围为0-100，透明效果与颜色模式相关（像素透明度），0为全透明，100为不透明
*/ 
BOOL MWIFrameFillRect(int nIndex, MWOSGRect * pRegion, int nAlpha);

#else

#define MWOSGRect           CSOSGRect
#define MWIFrameInit    CSIFrameInit
#define MWIFrameTerm    CSIFrameTerm
#define MWIFrameShow    CSIFrameShow
#define MWIFrameFillRect        CSIFrameFillRect
#define MWIFrameSetDisplayLayer CSIFrameSetDisplayLayer

#endif

#ifdef __cplusplus
}
#endif

/** @} */

#endif  /* _MW_IFRAME_H_ */

/* End of File */

