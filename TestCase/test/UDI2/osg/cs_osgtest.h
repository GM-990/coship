#ifndef _CS_OSG_TEST_H_
#define _CS_OSG_TEST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "udi2_osg.h"
#include "udi2_typedef.h"
#include "udiplus_os.h"
#include "udiplus_debug.h"

#define COLOR_EFFECT_VALUE  (3)
#define SURFACE_WIDTH_720		(720)
#define SURFACE_HEIGHT_576		(576)

//=======CSTC_OSG_IT_SetSurfaceInfo_0001~4用到这些参数=========//
#define INVALID_WIDTH   		(0x7fffffff)
#define INVALID_HEIGHT  		(0x7fffffff)
#define INVALID_ALPHAFLAG 		(0x7fffffff)
#define INVALID_ALPHA           (INVALID_ALPHAFLAG)
#define INVALID_COLORKEYFLAG	(0x7fffffff)
#define INVALID_COLORKEY  		(INVALID_COLORKEYFLAG)
#define INVALID_PIXEL_FORMAT	(EM_UDIOSG_PIXEL_FORMAT_NUM)
//============================================================//
#define INVALID_COLOR     			 (~0)

#define OSG_COLORKEY_BLACK      		 (0xff000000)  //ARGB888
#define OSG_COLORKEY_WHITE	 		 (0xffffffff)  //ARGB888
#define OSG_COLOR_BLACK_32BIT        	(0xff000000)   //black
#define OSG_COLOR_RED_32BIT           (0xffff0000)   //color1
#define OSG_COLOR_GREEN_32BIT          (0xff00ff00) ///color2
#define OSG_COLOR_TRANSPARENTRED_RED32BIT         (0x40ff0000)  //color3
#define OSG_COLOR_BLUE_32BIT          (0xff0000ff)  //color4


#define OSG_COLOR_RED_16BIT            (0xfc00)//color1
#define OSG_COLOR_GREEN_16BIT          (0x83e0)///color2
#define OSG_COLOR_BLUE_16BIT           (0x801f)//color4
#define OSG_COLOR_TRANSPARENTRED_RED16BIT    (0xfc00)//color3

#define OSGUDI2_32BIT_ALPHA_1  (0x7f)
#define OSGUDI2_32BIT_ALPHA_2  (0xff)
#define OSGUDI2_32BIT_ALPHA_3  (0x00)

#define OSGUDI2_16BIT_ALPHA_2 (0x00)
#define OSGUDI2_16BIT_ALPHA_1 (0xff)
#define OSG_FILL_COLOR_BLACK (0x0)
#define OSG_FILL_COLOR_GREY_16BIT (0x8888)
#define OSG_FILL_COLOR_GREY_32BIT (0x88888888)

#define OSG_COLOR_FORMAT_16BIT  (0x16)
#define OSG_COLOR_FORMAT_32BIT  (0x32)


/*
* 图片解码支持类型枚举
*/
typedef enum
{
	EM_OSG_DECIMAGE_IFRAME,   //支持IFRAME图片硬解码
	EM_OSG_DECIMAGE_BMP,   //支持BMP图片硬解码
	EM_OSG_DECIMAGE_JPG,   //支持JPG图片硬解码
	EM_OSG_DECIMAGE_PNG,   //支持PNG图片硬解码
	EM_OSG_DECIMAGE_MAX 
}OSGUDI2SupportDecIamgeType_E;

/*
* 绘图平面大小枚举
*/
typedef enum
{
	EM_SURFACE_1920_1080,
	EM_SURFACE_1280_720,
	EM_SURFACE_720_576
}OSGUDI2Surface_E;

/*
*各种矩形区域枚举，取值详见CSTC_OSG_IT_Blit_0000定义
*/
typedef enum
{
	EM_RECT_1,
	EM_RECT_2,
	EM_RECT_3,
	EM_RECT_4,
	EM_RECT_5,
	EM_RECT_6,
	EM_RECT_7,
	EM_RECT_8,
	EM_RECT_9,
	EM_RECT_10,
	EM_RECT_11,
	EM_RECT_12,
	EM_RECT_13,
	EM_RECT_14,
	EM_RECT_15,
	EM_RECT_16,
	EM_RECT_17,	
	EM_RECT_18,		
	EM_RECT_19,		
	EM_RECT_20,	
	EM_RECT_21,		
	EM_RECT_22,	
	EM_RECT_23,			
	EM_RECT_MAX
}OSGUDI2Rect_E;
/*
*各种图片资源名称枚举
*/
typedef enum
{
	EM_IFRAME_720_576,
	EM_IFRAME_PES_720_576,
	EM_IFRAME_1280_720,
	EM_IFRAME_1920_1080,
	EM_BMP_720_576,
	EM_BMP_1280_720,
	EM_BMP_1920_1080,
	EM_JPG_720_576,
	EM_JPG_1280_720,
	EM_JPG_1920_1080,
	EM_JPG_8192_8192,	
	EM_PNG_720_576,
	EM_PNG_1280_720,
	EM_PNG_1920_1080,
	EM_BMP_CIRCLE_576_576,
	EM_JPG_CIRCLE_576_576,
	EM_PNG_CIRCLE_576_576,
	EM_JPG_DOUBLE_SOF,//JPG图片有两个SOF,详情请参见JPG标准
	EM_JPG_SOF_Y_SAMPLE_12,//jpg图片SOF中水平垂直数据采样率为1:2,有些硬件平台可能不支持
	EM_YUV_FASTJPG_DATA,
	EM_IMAGE_MAX
}OSGUDI2ImageType_E;

//alpha混合取值公式枚举类型，详见CSTC_OSG_IT_Blit_0000说明
typedef enum
{
	EM_FORUM_1,	//dA = sMixA + dMixA - ((sMixA * dMixA)/0xff)  (coship)
	EM_FORUM_2, //dA = Asrc1 + Asrc2 * Global_Alpha (1- Asrc1)
	EM_FORUM_4, //dA = dColorA
	EM_FORUM_5, //dA = sColorA
	EM_FORUM_6, //dA = dGA
	EM_FORUM_7, //dA = sMixA + dColorA - ((sMixA * dColorA)/0xff);
	EM_FORUM_8,
	EM_FORUM_Max
}OSGUDI2AlphaForumType_E;

#define LIST_MEMBER_NUM  (EM_UDIOSG_PIXEL_FORMAT_NUM + EM_OSG_DECIMAGE_MAX +11)
typedef struct _Cfg_Info_S
{
	int 		nType;//处理config信息的方式，0:比较赋值; 1:直接赋值
	int*		pSavebuf;//存放所获得的config信息的地址
	char*	pCfgInfoKey;//获得信息的关键字
}Cfg_Info_S;

#define TEST_OSG_GOTO(expression, label, pErrorInfo)  \
{ \
    if(!(expression)) \
    {\
        CSTCPrint("[OSG]Assertion: \"%s\" failed, in file %s, line %d.\r\n",#expression, __FILE__, __LINE__); \
        CSTCPrint("[OSG]%s\r\n",pErrorInfo); \
        goto label;\
    }\
}

/**
@brief 读取配置文件信息,初始化支持的颜色模式

@return 读取成功返回CS_TK_CONFIG_SUCCESS,失败返回CS_TK_CONFIG_FAILURE
*/
int OSG_iInitCfg();

int OSG_iUnInitCfg();

CSUDI_BOOL OSG_GetSize(int cfg,int *pnWidth,int *pnHeight);
//CSUDI_BOOL OSG_GetSize1(int cfg,int  nWidth,int  nHeight);

/**
@brief 测试创建相应大小和颜色模式的绘图平面

@param[in] eFixelFormat 创建绘图平面的颜色模式，详见CSUDIOSGPixelFormat_E
@param[in] eType 创建的绘图平面的大小类型，详见OSGUDI2Surface_E说明
@return 
	- 如果平台支持创建相应的颜色模式，并且创建成功，验证通过则返回CSUDI_TRUE；平台不支持该颜色模式则返回CSUDI_TRUE
	- 其它返回CSUDI_FALSE
*/
CSUDI_BOOL OSG_CreatSurface_TestPixelFormatAndSize(CSUDIOSGPixelFormat_E ePixelFormat,OSGUDI2Surface_E eType);

/*
检测各种情况下创建Surface 的正确性
*/
CSUDI_BOOL OSG_iCreateSurface(CSUDIOSGPixelFormat_E ePixelFormat, int nWidth, int nHeight);

/**
@brief 测试源和目的矩形区域在不同大小的情况下的blit操作

测试用例CSTC_OSG_IT_Blit_Rect_0001~20调用此函数
@param[in] eFixelFormat 创建绘图平面的颜色模式，详见CSUDIOSGPixelFormat_E
@param[in] ucFillSrcColor 填充源矩形区域的颜色值(通过memset方式填充)
@param[in] eSRect 源矩形区域信息,详见OSGUDI2Rect_E说明
@param[in] eDRect 目的矩形区域信息，详见OSGUDI2Rect_E说明
@param[in] bValidRect 矩形区域是否为合法区域
@return 
	- 执行blit成功，并校验通过则返回CSUDI_TRUE
	- 其它返回CSUDI_FALSE
@note 如果bValidRect为CSUDI_TRUE，则会根据eDRect的值获取该区域中的随机像素点的颜色值，
并校验获取的颜色值是否与源矩形的颜色值一致
*/
CSUDI_BOOL OSG_Blit_TestRect(CSUDIOSGPixelFormat_E ePixelFormat,unsigned char ucFillSrcColor,OSGUDI2Rect_E eSRect,OSGUDI2Rect_E eDRect,CSUDI_BOOL bValidRect);

/**
@brief 测试缓存与显存之间的不同矩形区域大小情况下的Blit操作

测试用例CSTC_OSG_IT_Blit_Rect_0001调用此函数
@param[in] ucFillSrcColor 填充源矩形区域的颜色值(通过memset方式填充)
@param[in] eSRect 源矩形区域信息,详见OSGUDI2Rect_E说明
@param[in] eDRect 目的矩形区域信息，详见OSGUDI2Rect_E说明
@param[in] bValidRect 矩形区域是否为合法区域
@return 
	- 执行blit成功，并校验通过则返回CSUDI_TRUE
	- 其它返回CSUDI_FALSE
@note 如果bValidRect为CSUDI_TRUE，则会根据eDRect的值获取该区域中的随机像素点的颜色值，
并校验获取的颜色值是否与源矩形的颜色值一致
*/

CSUDI_BOOL OSG_Blit_TestRect_WithDisplay(unsigned char ucFillSrcColor,OSGUDI2Rect_E eSRect,OSGUDI2Rect_E eDRect,CSUDI_BOOL bValidRect);

/**
@brief 测试colorkey存在的情况下的blit操作

测试用例CSTC_OSG_IT_Blit_Ckey_XXXX调用此函数
@param[in] uColorKey 设置目的绘图平面的colorkey
@param[in] uSrcColor 源绘图平面的填充颜色,默认为32位模式
@param[in] uDstColor 目的绘图平面的填充颜色，默认为32位模式
@param[in] bExist 如果为CSUDI_TRUE，则uSrcColor应该存在目的绘图平面中，为CSUDI_FALSE,则uSrcColor不应该存在目的绘图平面中
@return 
	- 执行blit成功，并校验通过则返回CSUDI_TRUE
	- 其它返回CSUDI_FALSE
*/
CSUDI_BOOL OSG_Blit_TestCKey(CSUDIOSGPixelFormat_E ePixelFormat, unsigned int uColorKey,unsigned int uSrcColor, unsigned int uDstColor,unsigned int uCheckColor, CSUDI_BOOL bExist);

/**
@brief 测试alpha模式下的blit操作

测试用例CSTC_OSG_IT_Blit_Alpha_0001~32调用此函数
@param[in] eFixelFormat 创建绘图平面的颜色模式，详见CSUDIOSGPixelFormat_E
@param[in] uSrcAlpha 源绘图平面的全局alpha
@param[in] uDstAlpha 目的绘图平面的全局alpha
@param[in] uSrcColor 源绘图平面的填充颜色，默认为32位模式
@param[in] uDstColor 目的绘图平面的填充颜色，默认为32位模式
@return 
	- 执行blit成功，并校验通过则返回CSUDI_TRUE
	- 其它返回CSUDI_FALSE
*/
CSUDI_BOOL OSG_Blit_TestAlpha(CSUDIOSGPixelFormat_E ePixelFormat,unsigned int uSrcAlpha,unsigned int uDstAlpha,unsigned int uSrcColor, unsigned int uDstColor);
CSUDI_BOOL OSG_Blit_TestAlphaManual(CSUDIOSGPixelFormat_E ePixelFormat,unsigned int uSrcColor, unsigned int uDstColor);

/**
@brief 测试alpha模式下的blit操作

测试用例CSTC_OSG_IT_Blit_Alpha_0001~32调用此函数
@param[in] eFixelFormat 创建绘图平面的颜色模式，详见CSUDIOSGPixelFormat_E
@param[in] uSrcAlpha 源绘图平面的全局alpha
@param[in] uDstAlpha 目的绘图平面的全局alpha
@param[in] puSrcColor 源绘图平面的填充颜色，默认为32位模式
@param[in] puDstColor 目的绘图平面的填充颜色，默认为32位模式
@return 
	- 执行blit成功，并校验通过则返回CSUDI_TRUE
	- 其它返回CSUDI_FALSE
*/
CSUDI_BOOL OSG_Blit_TestAlphaWithDataColor(
					CSUDIOSGPixelFormat_E ePixelFormat,
					unsigned int uSrcAlpha,
					unsigned int uDstAlpha,
					unsigned char * puSrcColor,
					unsigned char * puDstColor, 
					int nColorLen);

/**
@brief 测试alpha模式下的blit操作

测试用例CSTC_OSG_IT_Blit_Alpha_0001~32调用此函数
@param[in] eFixelFormat 创建绘图平面的颜色模式，详见CSUDIOSGPixelFormat_E
@param[in] uSrcColor 源绘图平面的填充颜色，默认为32位模式
@param[in] uDstColor 目的绘图平面的填充颜色，默认为32位模式
@param[in] eMode blit操作的模式，详见CSUDIOSGOpMode_E说明
@return 
	- 执行blit成功，并校验通过则返回CSUDI_TRUE
	- 其它返回CSUDI_FALSE
*/
CSUDI_BOOL OSG_Blit_TestOpMode(CSUDIOSGPixelFormat_E ePixelFormat,unsigned int uSrcColor, unsigned int uDstColor,CSUDIOSGOpMode_E eMode);

/**
@brief 测试在缓存平面上进行fill操作

@param[in] eFixelFormat 创建绘图平面的颜色模式，详见CSUDIOSGPixelFormat_E
@param[in] ucFillColor 源绘图平面的填充颜色，默认为32位模式
@param[in] eRect 矩形区域信息,详见OSGUDI2Rect_E说明
@param[in] bValidRect 矩形区域是否为合法区域
@return 
	- 执行blit成功，并校验通过则返回CSUDI_TRUE
	- 其它返回CSUDI_FALSE
@note 如果bValidRect为CSUDI_TRUE，则会根据eDRect的值获取该区域中的随机像素点的颜色值，
并校验获取的颜色值是否与源矩形的颜色值一致
*/
CSUDI_BOOL OSG_Blit_TestFill(CSUDIOSGPixelFormat_E ePixelFormat,unsigned int ucFillColor,OSGUDI2Rect_E eRect,CSUDI_BOOL bValidRect);


/**
@brief 测试在缓存平面上进行fill操作

@param[in] eFixelFormat 创建绘图平面的颜色模式，详见CSUDIOSGPixelFormat_E
@param[in] ucFillColor 源绘图平面的填充颜色，默认为32位模式
@param[in] eRect 矩形区域信息,详见OSGUDI2Rect_E说明
@param[in] bValidRect 矩形区域是否为合法区域
@return 
	- 执行blit成功，并校验通过则返回CSUDI_TRUE
	- 其它返回CSUDI_FALSE
@note 如果bValidRect为CSUDI_TRUE，则会根据eDRect的值获取该区域中的随机像素点的颜色值，
并校验获取的颜色值是否与源矩形的颜色值一致
*/
CSUDI_BOOL OSG_Blit_TestFillEx(CSUDIOSGPixelFormat_E ePixelFormat,unsigned int ucFillColor,int nSurfaceWidth, int nSurfaceHeight, CSUDIOSGRect_S * psSrcRect);

/**
@brief 测试在显存空间上进行fill操作

@param[in] ucFillColor 源绘图平面的填充颜色，默认为32位模式
@param[in] eRect 矩形区域信息,详见OSGUDI2Rect_E说明
@param[in] bValidRect 矩形区域是否为合法区域
@return 
	- 执行blit成功，并校验通过则返回CSUDI_TRUE
	- 其它返回CSUDI_FALSE
@note 如果bValidRect为CSUDI_TRUE，则会根据eDRect的值获取该区域中的随机像素点的颜色值，
并校验获取的颜色值是否与源矩形的颜色值一致
*/
CSUDI_BOOL OSG_iTestFillDisplay(unsigned int ucFillColor,OSGUDI2Rect_E eRect,CSUDI_BOOL bValidRect);

/**
@brief 测试图片硬解码

@param[in] eSupportType 测试的图片类型，详见OSGUDI2SupportDecIamgeType_E说明
@param[in] eImageType 图片资源文件的标识,详见OSGUDI2ImageType_E说明
@return 
	- 执行blit成功，并校验通过则返回CSUDI_TRUE
	- 其它返回CSUDI_FALSE
*/
CSUDI_BOOL OSG_iTestDecImage(OSGUDI2SupportDecIamgeType_E eSupportType,OSGUDI2ImageType_E eImageType);

CSUDI_BOOL OSG_iTestDecImageWithScaleMode(OSGUDI2SupportDecIamgeType_E eSupportType,OSGUDI2ImageType_E eImageType,CSUDIOSGScalingMode_E eScalingMode,const CSUDIOSGRect_S * psDstRect);
CSUDI_BOOL OSG_iTestDecImageSeveralJpg();
/**
@brief 测试在同一缓存空间上进行blit操作

@param[in] eFixelFormat 创建绘图平面的颜色模式，详见CSUDIOSGPixelFormat_E
@param[in] eSRect 源矩形区域信息,详见OSGUDI2Rect_E说明
@param[in] eDRect 目的矩形区域信息，详见OSGUDI2Rect_E说明
@param[in] bValidRect 矩形区域是否为合法区域
@return 
	- 执行blit成功，并校验通过则返回CSUDI_TRUE
	- 其它返回CSUDI_FALSE
@note 如果bValidRect为CSUDI_TRUE，则会根据eDRect的值获取该区域中的随机像素点的颜色值，
并校验获取的颜色值是否与源矩形的颜色值一致
*/
CSUDI_BOOL OSG_iTestBlitWithSameSurface(CSUDIOSGPixelFormat_E ePixelFormat,OSGUDI2Rect_E eSRect,OSGUDI2Rect_E eDRect,CSUDI_BOOL bValidRect);

/**
@brief 测试在同一显存空间上进行blit操作

@param[in] eSRect 源矩形区域信息,详见OSGUDI2Rect_E说明
@param[in] eDRect 目的矩形区域信息，详见OSGUDI2Rect_E说明
@param[in] bValidRect 矩形区域是否为合法区域
@return 
	- 执行blit成功，并校验通过则返回CSUDI_TRUE
	- 其它返回CSUDI_FALSE
@note 如果bValidRect为CSUDI_TRUE，则会根据eDRect的值获取该区域中的随机像素点的颜色值，
并校验获取的颜色值是否与源矩形的颜色值一致
*/
CSUDI_BOOL OSG_iTestBlitWithSameDisplay(OSGUDI2Rect_E eSRect,OSGUDI2Rect_E eDRect,CSUDI_BOOL bValidRect);

/**
@brief 测试在缓存空间上进行blit操作的性能

@param[in] eFixelFormat 创建绘图平面的颜色模式，详见CSUDIOSGPixelFormat_E
@param[in] asSRect 源矩形区域信息数组,详见CSUDIOSGRect_S说明
@param[in] asDRect 目的矩形区域信息数组，详见CSUDIOSGRect_S说明
@param[in] nRectLen 矩形区域数组长度
@param[in] bWithAlpha blit操作是否带alpha
@return 
	- 执行blit成功，并校验通过则返回CSUDI_TRUE
	- 其它返回CSUDI_FALSE
@note 该测试只会打印出每次blit需要的时间
*/
CSUDI_BOOL OSG_iTESTBlitPerformance(CSUDIOSGPixelFormat_E ePixelFormat,CSUDIOSGRect_S asSRect[],CSUDIOSGRect_S asDRect[],int nRectLen,CSUDI_BOOL bWithAlpha);

/**
@brief 测试在显存空间上进行blit操作的性能

@param[in] asSRect 源矩形区域信息数组,详见CSUDIOSGRect_S说明
@param[in] asDRect 目的矩形区域信息数组，详见CSUDIOSGRect_S说明
@param[in] nRectLen 矩形区域数组长度
@param[in] bWithAlpha blit操作是否带alpha
@return 
	- 执行blit成功，并校验通过则返回CSUDI_TRUE
	- 其它返回CSUDI_FALSE
@note 该测试只会打印出每次blit需要的时间
*/
CSUDI_BOOL OSG_iTESTBlitPerformanceWithDisplay(CSUDIOSGRect_S asSRect[],CSUDIOSGRect_S asDRect[],int nRectLen,CSUDI_BOOL bWithAlpha);

/**
@brief 测试在缓存空间上进行fill操作的性能

@param[in] eFixelFormat 创建绘图平面的颜色模式，详见CSUDIOSGPixelFormat_E
@param[in] asSRect 填充矩形区域信息数组,详见CSUDIOSGRect_S说明
@param[in] nRectLen 矩形区域数组长度
@param[in] bWithAlpha blit操作是否带alpha
@return 
	- 执行blit成功，并校验通过则返回CSUDI_TRUE
	- 其它返回CSUDI_FALSE
@note 该测试只会打印出每次fill需要的时间
*/
CSUDI_BOOL OSG_iTESTFillPerformace(CSUDIOSGPixelFormat_E ePixelFormat,CSUDIOSGRect_S asRect[],int nRectLen);

/**
@brief 测试在显存空间上进行fill操作的性能

@param[in] asSRect 源矩形区域信息数组,详见CSUDIOSGRect_S说明
@param[in] asDRect 目的矩形区域信息数组，详见CSUDIOSGRect_S说明
@param[in] nRectLen 矩形区域数组长度
@param[in] bWithAlpha blit操作是否带alpha
@return 
	- 执行blit成功，并校验通过则返回CSUDI_TRUE
	- 其它返回CSUDI_FALSE
@note 该测试只会打印出每次fill需要的时间
*/
CSUDI_BOOL OSG_iTESTFillPerformaceWithDisplay(CSUDIOSGRect_S asRect[],int nRectLen);

/**
@brief 测试(缓存空间和显存之间)执行blit操作之后连续调用多次Sync的情况

@param[in] eFixelFormat 创建绘图平面的颜色模式，详见CSUDIOSGPixelFormat_E
@param[in] bWithAlpha blit操作是否带alpha
@param[in] 执行一次sync之后，还要测试几次，至少为1，第一次之后每次sync期望在5s之内
@return 
	- 校验通过则返回CSUDI_TRUE
	- 其它返回CSUDI_FALSE
*/
CSUDI_BOOL OSG_iTestSync(CSUDIOSGPixelFormat_E ePixelFormat,CSUDI_BOOL bWithAlpha,int nSyncTimes);
/**
@brief 测试(缓存空间之间)执行blit操作之后连续调用多次Sync的情况

@param[in] bWithAlpha blit操作是否带alpha
@param[in] 执行一次sync之后，还要测试几次，至少为1，第一次之后每次sync期望在5s之内
@return 
	- 校验通过则返回CSUDI_TRUE
	- 其它返回CSUDI_FALSE
*/
CSUDI_BOOL OSG_iTestSyncWithDisplay(CSUDI_BOOL bWithAlpha,int nSyncTimes);

CSUDI_BOOL OSG_iTESTSurfacePitch(CSUDIOSGPixelFormat_E ePixel,int nWidth ,int nHeight,unsigned char * puDstColor, int nColorLen);
/**
@brief 判断平台是否支持该颜色模式，由配置文件提供

@param[in] eFixelFormat 绘图平面的颜色模式，详见CSUDIOSGPixelFormat_E
@return 如果平台支持创建相应的颜色模式，则返回CSUDI_TRUE；平台不支持该颜色模式则返回CSUDI_FALSE
*/
CSUDI_BOOL OSG_IsSupportPixelFormat(CSUDIOSGPixelFormat_E ePixelFormat );

/**
@brief 判断平台是否支持创建1280*720的绘图平面，由配置文件提供

@return 如果平台支持创建，则返回CSUDI_TRUE；平台不支持则返回CSUDI_FALSE
*/
CSUDI_BOOL OSG_IsSupport1280_720(void);

/**
@brief 判断平台是否支持创建1920*1080的绘图平面，由配置文件提供

@return 如果平台支持创建，则返回CSUDI_TRUE；平台不支持则返回CSUDI_FALSE
*/
CSUDI_BOOL OSG_IsSupport1920_1080(void);

CSUDI_BOOL OSG_IsSupportSDHDDisplay(void);

CSUDI_BOOL OSG_iCheckSDDisplayInfo(CSUDI_HANDLE hSurface);

CSUDI_BOOL OSG_iCheckHDDisplayInfo(CSUDI_HANDLE hSurface);
CSUDI_BOOL OSG_IsSupportDecImage(OSGUDI2SupportDecIamgeType_E eType);
CSUDI_BOOL OSG_iFillSurface(CSUDI_HANDLE hSurface,unsigned int uFillColor,int nPixelMode);
CSUDI_BOOL OSG_iFillSurfaceWithFileData(CSUDI_HANDLE hSurface,unsigned char * pcFile,int nLen,int nPixelMode);
CSUDI_BOOL OSG_iFindSupportPixelFormat(CSUDIOSGPixelFormat_E *ePixelFormat);
CSUDI_BOOL OSG_iSetSurfaceInfo(CSUDI_HANDLE hSurface);

CSUDI_BOOL OSG_iCheckSetSurfaceInfo(CSUDI_HANDLE hSurface, CSUDIOSGSurfaceInfo_S sSurfaceInfo);

CSUDI_BOOL OSG_iCheckSurfaceInfo(CSUDI_HANDLE hSurface,int nExpectPixelFormat, int nExpectWidth, int nExpectHeight);

CSUDI_BOOL OSG_iCheckNotExistColor(
					CSUDI_HANDLE hSurface,
					unsigned int uCheckColor,
					const CSUDIOSGRect_S * psDRect,
					int nPixelMode
					);
CSUDI_BOOL OSG_iCheckExistColor(
					CSUDI_HANDLE hSurface,
					unsigned int uCheckColor,
					const CSUDIOSGRect_S * psDRect,
					int nPixelMode
					);

CSUDI_BOOL OSG_iCheckColorAndAlpha(
					CSUDI_HANDLE hDstSurface,
					unsigned int uSrcColor,
					unsigned int uDstColor, 
					unsigned int  uSrcAlpha,
					unsigned int  uDstAlpha,
					int nPixelMode
					);
/*从文件中获取数据来对比*/
CSUDI_BOOL OSG_iCheckColorAndAlphaWithFileColor(CSUDI_HANDLE hDstSurface,
													const unsigned char * puSrcColor,/*源平面颜色值*/
													const unsigned char * puDstColor, /*目的平面颜色值*/
													int 		  nColorLen,   /*颜色大小，目前两个颜色的大小一样*/
													unsigned int  uSrcAlpha,/*源全局alpha值*/
													unsigned int  uDstAlpha,/*目的全局alpha值*/
													int nPixelMode);
CSUDI_BOOL OSG_iCheckColorMixResult(
					CSUDI_HANDLE hSurface,
					unsigned int  uSrcColor,
					unsigned int  uDstColor,
					int nMixMode,
					int nPixelMode
					);

unsigned int OSG_iGetPhoto(const char *photoName, unsigned char **pBuff );
int OSG_iGetRandom(int min,int max);
void OSG_iFillRect(CSUDIOSGRect_S *psRect,int nX,int nY,int nWidth,int nHeight);

void OSG_iFillRectExt(CSUDIOSGRect_S * psRect,int nX,int nY,int nWidth,int nHeight,OSGUDI2Rect_E eRect);
unsigned int OSG_iColor32BitTo16Bit(unsigned int uSrcColor);
unsigned int OSG_iGetColorWithPixelMode(unsigned int uSrcColor,CSUDIOSGPixelFormat_E nPixelMode);
void OSG_iClearScreen();
CSUDI_BOOL OSG_iTestMemcpyImageToDisplay(OSGUDI2SupportDecIamgeType_E eSupportType,OSGUDI2ImageType_E eImageType);
CSUDI_BOOL OSG_iTestDecImageToDisplay(OSGUDI2SupportDecIamgeType_E eSupportType,OSGUDI2ImageType_E eImageType);
CSUDI_BOOL OSG_iTestYUVtoRGB(CSUDIOSGPixelFormat_E ePixelFormat,OSGUDI2ImageType_E eImageType);
void iFillSurfaceByBmp(unsigned char *pBmpBuffer, CSUDI_HANDLE hSurface, int w, int h);

CSUDI_BOOL OSG_iFillAndSync(CSUDI_HANDLE hSurface, CSUDIOSGRect_S *psRect, unsigned int unColor);

CSUDI_BOOL OSG_iBlitAndSync(CSUDI_HANDLE hDstSurface, CSUDIOSGRect_S *psDstRect, 
	CSUDI_HANDLE hSrcSurface, CSUDIOSGRect_S *psSrcRect, CSUDIOSGOpMode_E eOpMode);

CSUDI_BOOL OSG_iSetSurfaceBuf(CSUDI_HANDLE hSurface, unsigned char * pucImage, 
	int  nImageSize, int nImageWidth, int nImageHeight);

CSUDI_BOOL OSG_IsShareHDDisplaySurface(void);
int OSG_GetTestDisplaySurfaceIndex(void);
void iFillSurfaceByBmpWithReverseOrder(const unsigned char *pBmpBuffer, CSUDI_HANDLE hSurface, int w, int h);

/*读取显存默认的颜色模式,并返回颜色模式对应的字节数*/
CSUDI_BOOL OSG_iFindSupportPixelFormatAndBytes(CSUDIOSGPixelFormat_E *ePixelFormat, int * nBytes);


/**
  colorKey有源colorkey和目的colorkey之分，
  源colorkey需要设置源平面的colorkey属性，针对源平面的颜色进行处理；
  目的colorkey需要设置目的colorkey的属性，针对目的平面的颜色进行处理；
*/
CSUDI_BOOL OSG_Blit_TestCKeyForDst(CSUDIOSGPixelFormat_E ePixelFormat, unsigned int uColorKey,unsigned int uSrcColor, unsigned int uDstColor, unsigned int uCheckColor,CSUDI_BOOL bExist);


/*此函数用来测试alpha混合*/
CSUDI_BOOL OSG_Blit_TestAlphaWithDataColorEx(CSUDIOSGPixelFormat_E ePixelFormat,unsigned int uSrcAlpha,unsigned int uDstAlpha, const unsigned char * puSrcColor, const unsigned char * puDstColor, int nColorLen);


/*此函数用来逐点填充surface*/
CSUDI_BOOL OSG_Blit_TestFillOnePixel(CSUDIOSGPixelFormat_E ePixelFormat,unsigned int ucFillColor);


/*此函数用来测试移动特效的性能*/
CSUDI_BOOL OSG_iTESTPortalPerformance(CSUDIOSGOpMode_E eOpMode, unsigned int	unForeground_color);

/*此函数主要用来模拟跑马灯的移动*/
CSUDI_BOOL OSG_iTestMarquee();

/*此函数主要用来测试屏幕是否有偏移*/
CSUDI_BOOL OSG_iTestScreenOffset();

/*此函数主要用来测试屏幕上是否有严重的锯齿产生*/
CSUDI_BOOL OSG_iTestScreenSerrate();

/*此函数用来检测屏幕刷新是否有丢失的情况*/
CSUDI_BOOL OSG_iTestScreenlost();

/*此函数检查平台是否支持高清*/
CSUDI_BOOL OSG_iTestSupportHDDisplay();

/* 循环解码指定文件名的文件*/
CSUDI_BOOL OSG_iTestDecFolderImages();




#ifdef __cplusplus
}
#endif

#endif/*_CS_OSG_TEST_H_*/ 
