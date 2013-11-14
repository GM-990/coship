/**@defgroup OSG OSG 定义平台绘图接口,本套接口的性能决定了中间件图形刷新的性能，建议实现时不加多线程保护
除非该平台多线程操作时，图形资源有冲突导致不能显示才加多线程保护

@brief OSG模块对应头文件<udi2_osg.h>，定义了操作OSG设备的一系列接口。模块的设计思
想是在基于Framebuffer的基础上，尽量利用硬件加速的特性。在本模块中，坐标系的延伸
方式为X轴从左至右延伸，Y轴从上至下延伸。

@par 内存Surface
模块中的Surface即可以理解为一个Framebuffer，创建时可以指定其大小、颜色模式等。其
本质就是一段内存，但根据具体平台的不同，这可能是一段特殊的，可以利用硬件加速的内
存；也可能就是一段普通的内存。Eastwin绘制图像的操作，就是把数据写到Surface的过程。\n

@par 显存Surface
除了普通的Surface，还有几个特殊的Surface，即显存，通过CSUDIOSGGetDisplaySurface接
口可以获取到这些Surface。如果将数据拷贝到显存上，则图像会立刻显示到屏幕上。如果平
台不支持这样的拷贝操作，也可把显存Surface的m_pvSurfaceMem字段设为CSUDI_NULL，这种
情况下Eastwin仅可通过Blit接口将图像显示出来。\n

@note 实际上，只要平台能够创建Surface，Eastwin都会通过Blit接口来显示图像。\n

@par Blit接口
Blit是本模块中最重要的一个接口，它通过硬件加速的方式将显示数据在Surface之间移动。
拷贝数据的Surface以及拷贝矩形之间，可能存在各种各样的差异，这些差异可以产生很多特
殊的效果，如颜色模式转换、图像缩放、扭曲等。如果硬件不支持这些特性，可以返回
CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED，但一般来说高清平台必须支持采用硬件加速方式实
现的缩放效果，因为高清平台都需要将图像显示在两个分辨率不同的屏幕上，而采用纯软件运
算实现的缩放效率极低，Eastwin根本不会进行这样的尝试。
为了充分提高绘图性能，该接口需采用异步模式实现。即Blit接口返回后，Blit操作不一定必
须完成，这样连续调用的多个Blit操作就有了合并的可能。CSUDIOSGSync会处理好需要同步操
作的情况，即所有的Blit操作都必须在CSUDIOSGSync接口返回后完成。这意味着在Blit之后
CSUDIOSGSync返回前改变相关Surface的内容，绘图效果是不确定的，但即使这样也不能死机或
产生其他不可恢复的异常。采用异步模式这样的优化时要特别注意，最终合并后的Blit效果，应
该与同步的Blit效果保持一致，应特别注意多个Blit之间的覆盖关系。即如果没有特殊的裁剪处
理，Blit的执行顺序是不能发生改变的。\n

@par 图片硬解码接口
OSG模块还包括一个将图片硬解码到一个新Surface上的接口。不过绝大多数DVB平台都是具有硬
件解码I帧的能力的，也就是说绝大多数情况下，这个接口至少会有一个功能，即解码I帧。需
要特别注意的是解码I帧一般需要用到Video解码器，在没有独立I帧解码器的情况下（大多数平
台没有）这就会与视频播放冲突，本接口应尽量使用闲置Video解码器进行解码（如果有多个
Video解码器的话），若实在找不到闲置解码器则返回CSUDIOSG_ERROR_DEVICE_BUSY而不是中断
视频播放，这种情况下Eastwin如必须要进行解码，会通知上层暂停视频播放后再次调用该接口。

@note 这是一个硬解码的接口，除非有非常充分的理由，否则不要轻易尝试以软解码的方式实现该接
口，Eastwin中已经包括了软解码的模块，如果确实以软解码方式实现该接口，则返回
CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED即可。

@note Booter及Loader版本中，本模块所有接口的实现均可从简，对于标清平台最低标准只需要实现
获取显存接口即可；而高清版本只需要实现最基本的Blit模式即可。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

@version 2.0.6 2009/9/07 增加colorkeyflag 和alphaflag
@version 2.0.5 2009/8/28 把矩形区域结构体移入public公共头文件中
@version 2.0.4 2009/8/27 增加图片硬件
@version 2.0.3 2009/8/26 修改colorKey类型为无符号数
@version 2.0.2 2009/8/24 第一次综合评审后修改的版本
@version 2.0.1 2009/07/08 经初步讨论过的版本
@{
*/

#ifndef _UDI2_OSG_H_
#define _UDI2_OSG_H_


#include "udi2_typedef.h"
#include "udi2_error.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief 返回错误代码枚举定义。*/
enum {
	CSUDIOSG_ERROR_BAD_PARAMETER = CSUDI_OSG_ERROR_BASE,                 ///< 参数异常错误 
    CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED,                                ///< 不支持此操作 
    CSUDIOSG_ERROR_UNKNOWN_ERROR,                                        ///< 未知错误
    CSUDIOSG_ERROR_NO_MEMORY,                                             ///< 内存不足错误 
    CSUDIOSG_ERROR_DEVICE_BUSY                                         ///< 设备忙
};

/**@brief 颜色模式枚举定义

仅定义常见颜色模式，只需选择需要的模式(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555 
EM_UDIOSG_PIXEL_FORMAT_ARGB_8888)支持即可，不需要全部支持。
*/
typedef enum {
    EM_UDIOSG_PIXEL_FORMAT_RGB_565,     ///< 16-bit, no per-pixel alpha 
    EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,   ///<  16-bit 
    EM_UDIOSG_PIXEL_FORMAT_RGBA_5551,   ///<  16-bit 
    EM_UDIOSG_PIXEL_FORMAT_ARGB_4444,   ///<  16-bit 
    EM_UDIOSG_PIXEL_FORMAT_RGBA_4444,   ///<  16-bit 

    EM_UDIOSG_PIXEL_FORMAT_RGB_888,     ///<  24-bit 
    EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,   ///<  32-bit 
    EM_UDIOSG_PIXEL_FORMAT_PALETTE8,    ///<  8-bit Palette index，目前没有调色板的Surface，调色板的结构预留。

    EM_UDIOSG_PIXEL_FORMAT_YCBCR888_444, ///< Y, Cb, Cr signed components with 8 bit precision in a 4:4:4 sampling model
    EM_UDIOSG_PIXEL_FORMAT_YCBCR888_422, ///< Y, Cb, Cr signed components with 8 bit precision in a 4:2:2 sampling model
	EM_UDIOSG_PIXEL_FORMAT_YCBCR888_420, ///< Y, Cb, Cr signed components with 8 bit precision in a 4:2:0 sampling model
    EM_UDIOSG_PIXEL_FORMAT_YCBCR888_411, ///< Y, Cb, Cr signed components with 8 bit precision in a 4:1:1 sampling model
    EM_UDIOSG_PIXEL_FORMAT_NUM            ///< 像素格式的总数
} CSUDIOSGPixelFormat_E;


/**@brief 混合模式枚举定义 */
typedef enum {
    EM_UDIOSG_OPMODE_COPY,               ///<  src 
    EM_UDIOSG_OPMODE_XOR,                ///<  src ^ dst 
    EM_UDIOSG_OPMODE_OR,                 ///<  src | dst 
    EM_UDIOSG_OPMODE_AND,                ///<  src & dst 
    EM_UDIOSG_OPMODE_CLEAR,             ///<  0 
    EM_UDIOSG_OPMODE_SET,                ///<  11111111 
    EM_UDIOSG_OPMODE_EQUIV,             ///<  ~(src ^ dst) 
    EM_UDIOSG_OPMODE_NOR,	               ///<  ~(src | dst) 
    EM_UDIOSG_OPMODE_NAND,               ///<  ~(src & dst) 
    EM_UDIOSG_OPMODE_INVERT,            ///<  ~dst 
    EM_UDIOSG_OPMODE_COPYINVERTED,      ///<  ~src 
    EM_UDIOSG_OPMODE_ORINVERTED,        ///<  ~src | dst 
    EM_UDIOSG_OPMODE_ANDINVERTED,          ///<  ~src & dst 
    EM_UDIOSG_OPMODE_ORREVERSE,            ///<  src | ~dst 
    EM_UDIOSG_OPMODE_ANDREVERSE,           ///<  src & ~dst 
    EM_UDIOSG_OPMODE_NOOP,                 ///<  dst 
    EM_UDIOSG_OPMODE_ALPHA                ///<  Alpha mode 
} CSUDIOSGOpMode_E;


/**@brief 绘图平面结构定义。 */
typedef struct {
	CSUDIOSGPixelFormat_E m_ePixelFormat;   ///<  该绘图平面支持颜色模式 
	int m_nWidth;                           ///<  该绘图平面的宽度  > 0
	int m_nHeight;                          ///<  该绘图平面的高度 > 0

	int m_nPitch;                            ///<  该绘图平面每行字节数,width*BytesPerPixel 
	int m_nColorKeyFlag;                    ///<  该标志为 1 时表示m_uColorKey 起作用，初始化为 0
	int m_nAlphaFlag;                       ///<  该标志为 1 时表示该平面的全局m_uAlpha起作用，初始化为 0
    unsigned int m_uColorkey;                        ///<  该绘图平面的colorKey ,AGRB888模式的像素值，指定该颜色对于该绘图平面不被显示 默认0x00000000
    unsigned int m_uAlpha;                            ///<  该绘图平面的透明度，[0x00,0xFF],0x00表示完全透明;0xFF表示不透明，默认为0xFF
    void * m_pvSurfaceMem;                    ///<  该绘图平面的内存地址 ,如果该Surface不能直接绘制（如特殊的显存Surface）,请赋CSUDI_NULL
    int m_nMemSize;                          ///<  该绘图平面的内存大小 
    void *m_pvPalette;                       ///<  m_ePixelFormat 为EM_UDIOSG_PIXEL_FORMAT_PALETTE8时指定的调色板数据区域，目前没有调色板的Surface,调色板的结构预留，故用void *
} CSUDIOSGSurfaceInfo_S;

/**@brief 矩形区域结构定义 */
typedef struct {
    int m_nX;                ///< 矩形区域左上角横坐标 
    int m_nY;                ///< 矩形区域左上角纵坐标 
	int m_nWidth;            ///< 矩形区域宽度 > 0
	int m_nHeight;           ///< 矩形区域高度 > 0
} CSUDIOSGRect_S;

/**@brief 图片缩放模式枚举定义 */
typedef enum {
    EM_UDIOSG_SCALE_FULLSIZE,   ///< 不缩放，按原始大小输出
    EM_UDIOSG_SCALE_OPTIMUM,   	///< 最优匹配，此模式下图片保持原始比例，但尽量大的占满整个输入矩形区域
    EM_UDIOSG_SCALE_FIXSIZE    ///< 不按比例缩放，即输出宽高均为指定值，但输入比例与实际数据不一致，会产生变形
} CSUDIOSGScalingMode_E;

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
CSUDI_Error_Code CSUDIOSGCreateSurface(CSUDIOSGPixelFormat_E ePixelFormat, int nWidth, int nHeight, CSUDI_HANDLE * phSurface);


/**
@brief 销毁一个绘图平面，释放该绘图平面的内存。

@param[in] hSurface CSUDIOSGCreateSurface 成功返回的句柄。
@return 成功返回 CSUDI_SUCCESS，否则返回错误代码值 
@note 
- 由CSUDIOSGCreateSurface创建的缓存区域不使用后，一定要调用此接口销毁释放。\n
- 尝试销毁一个显存空间，将返回CSUDIOSG_ERROR_BAD_PARAMETER。
*/
CSUDI_Error_Code CSUDIOSGDestroySurface(CSUDI_HANDLE hSurface);


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
CSUDI_Error_Code CSUDIOSGGetDisplaySurface(int nIndex, CSUDI_HANDLE * phSurface);

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
CSUDI_Error_Code CSUDIOSGGetSurfaceInfo(CSUDI_HANDLE hSurface, CSUDIOSGSurfaceInfo_S * psSurfaceInfo);

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
CSUDI_Error_Code CSUDIOSGSetSurfaceInfo(CSUDI_HANDLE hSurface, const CSUDIOSGSurfaceInfo_S * psSurfaceInfo);


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
                        CSUDI_HANDLE hSrcSurface, const CSUDIOSGRect_S * psSrcRect, CSUDIOSGOpMode_E eOpMode);


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
CSUDI_Error_Code CSUDIOSGFill(CSUDI_HANDLE hDstSurface, const CSUDIOSGRect_S * psDstRect, unsigned int uPixel);

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
CSUDI_Error_Code CSUDIOSGSync(void);

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
CSUDI_Error_Code CSUDIOSGDecodeImage(const char * pszImageType, const void * pvImageMem, int nMemSize,CSUDIOSGScalingMode_E eScalingMode,const CSUDIOSGRect_S * psDstRect,CSUDI_HANDLE * phSurface);

/**
@brief 获取3D显示必须的NativeDisplay句柄
@param[out] phNativeDisplay NativeDisplay句柄。
@return 返回错误类型。
@note 
-NativeDisplay句柄与具体平台中opengl es的实现相关。
-类似于win32的hdc
*/
CSUDI_Error_Code  CSUDIOSGGetNativeDisplay(CSUDI_HANDLE * phNativeDisplay);

/**
@brief 获取hSurface对应的NativeWindow句柄
@param[in]  hSurface Surface句柄
@param[out] phNativeWindow NativeWindow句柄
@return 返回错误类型。
@note 
-NativeWindow句柄与具体平台中opengl es的实现相关。
-类似于win32的window
*/
CSUDI_Error_Code CSUDIOSGGetNativeWindow (CSUDI_HANDLE hSurface, CSUDI_HANDLE *phNativeWindow);

#ifdef __cplusplus
}
#endif

/** @} */

#endif


