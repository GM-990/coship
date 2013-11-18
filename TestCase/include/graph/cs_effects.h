/**@defgroup Effects 提供C层2D图形特效变换功能, 包括基本特效如缩放，渐变等，和组合特效如百页窗等。
@brief 提供C层2D图形特效变换功能, 包括基本特效如缩放，渐变等，和组合特效如百页窗等。

@version 0.1 2009/02/13 初始版本
@{
*/
#ifndef _CS_EFFECT_H_
#define _CS_EFFECT_H_

#include "mwtypes.h"
#include "mwapi.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**********************************************************************************************
 *										常数定义
 **********************************************************************************************/

#define MODULE_EFFECT	"CSEffect"

/**********************************************************************************************
 *									枚举/结构定义
 **********************************************************************************************/
/**@brief effect模块的返回类型，OK表示成功，FAILED表示失败*/
typedef enum
{
	EM_EFFECT_SUCCUSS = 0,	/**< success */
	EM_EFFECT_FAILED = -1		/**< failed */
}CSEFTRet_E;


/**@brief 定义特效的方向*/
typedef enum _CSEFTDirectionType_E
{
	EM_EFFECT_DIRECTION_TYPE_LEFT,            /**< 向左*/
	EM_EFFECT_DIRECTION_TYPE_TOP,              /**< 向上*/
	EM_EFFECT_DIRECTION_TYPE_RIGHT,         /**< 向右*/
	EM_EFFECT_DIRECTION_TYPE_BOTTOM,    /**< 向下*/
	EM_EFFECT_DIRECTION_TYPE_CENTER,      /**< 中间*/

	EM_EFFECT_DIRECTION_TYPE_LT,                 /**< 左上*/
	EM_EFFECT_DIRECTION_TYPE_LB,                 /**< 左下*/
	EM_EFFECT_DIRECTION_TYPE_RT,                 /**< 右上*/
	EM_EFFECT_DIRECTION_TYPE_RB,                 /**< 右下*/

	EM_EFFECT_DIRECTION_TYPE_IN,                  /**< 向内*/
	EM_EFFECT_DIRECTION_TYPE_OUT,              /**< 向外*/

	EM_EFFECT_DIRECTION_TYPE_CLOCKWISE,        			  /**< 顺时针方向*/
	EM_EFFECT_DIRECTION_TYPE_COUNTER_CLOCKWISE,      /**< 逆时针方向*/

	EM_EFFECT_DIRECTION_TYPE_HORIZON,   /**< 水平*/
	EM_EFFECT_DIRECTION_TYPE_VERTICAL,  /**< 竖直*/

	EM_EFFECT_DIRECTION_TYPE_CENTER_BOTH,  /**< 从中间向两边*/
	EM_EFFECT_DIRECTION_TYPE_BOTH_CENTER,  /**< 从两边向中间*/

	EM_EFFECT_DIRECTION_TYPE_FAR_NEAR,  /**< 由远及近*/
	EM_EFFECT_DIRECTION_TYPE_NEAR_FAR,   /**< 由近及远*/
	EM_EFFECT_DIRECTION_TYPE_MAXNUMBER   /**< 最大数目*/	
}CSEFTDirectionType_E;

/**@brief 定义特效源的类型

目前支持pixmap及image格式
pixmap用于内存内的一块汇图区域，image对应一张图片，在MicroWin中有与之对应的数据结构.
*/
typedef enum _CSEFTSourceType_E
{
	EM_EFFECT_SOURCE_TYPE_IMAGE,            /**< Image格式*/
	EM_EFFECT_SOURCE_TYPE_PIXMAP            /**< Pixmap格式*/
}CSEFTSourceType_E;

/**@brief 特效变换时传入的数据源格式

包括数据源类型：图片格式或Pixmap格式
及图片/Pixmap的ID号
*/
typedef struct _CSEFTSource_S
{
	CSEFTSourceType_E	m_eSourceType;		/**< 数据源格式*/
	int					m_nSourceID;		/**< 数据源编号*/
}CSEFTSource_S;

/**@brief 通用特效变换时，图片单元的区域变换信息

通过指定不同的起始及终止点坐标，可实现移动特效
通过指定不同的区域宽度及高度，可实现缩放特效
*/
typedef struct _CSEFTFrame_S
{
    GR_RECT     m_sClip;        /**< 原始图片的裁剪区域*/
	GR_RECT		m_sRect;		/**< 显示的目标区域*/
	BYTE		m_ucAlpha;		/**< 透明度变化相关信息*/
	BYTE		m_ucZOrder;		/**< 参与特效变化的图片所在层次*/
	WORD		m_wInterval;		/**< 运行到下一轨迹的时间*/
}CSEFTFrame_S;

/**@brief 通用特效变换时，图片单元基本信息

通过指定不同的Z序值，可实现特效的分层显示效果
Z序值越大，图片显示时越位于上层。
*/
typedef struct _CSEFTUnit_S
{
	CSEFTSource_S	m_sDataSource;		/**< 数据源*/
	CSEFTFrame_S*	m_psFrames;		/**< 特效的运动轨迹*/
	int				m_nFrameCount;		/**< 特效的运动轨迹元素个数*/
}CSEFTUnit_S;

typedef struct
{
	int nCol;								/**<列数*/
	int nRow;								/**<行数*/
	BOOL bHasReflect;						/**<是否有倒影*/
	int nVisibleCol;						/**<可见的图片列数*/
}CSFlatwallLayoutParam_S;
/**********************************************************************************************
 *									基本功能接口
 **********************************************************************************************/

/**
@brief 移动（滑动）显示图片

将指定图片在指定时间内，动态平滑地从源区域移动到目标区域（支持自动背景层维护）。
@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] dst  目标点坐标
@param[in] src  起始点坐标
@param[in] nTime 变换持续时间
@param[in] nCount 变换次数，即一次变换分为多少次完成。
@param[in] psDataSource 目标图片资源
@param[out] 无
@return 成功返回0,失败返回非0值
@note 无
*/
int CSEFTMove (int client_id, GR_DRAW_ID id, GR_GC_ID gc, GR_POINT * dst, GR_POINT * src, 
	int nTime, int nCount, CSEFTSource_S *psDataSource);


/**
@brief 移动（滑动）显示图片

将指定图片在指定时间内，动态带加速度地从源区域移动到目标区域（支持自动背景层维护）。
@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] dst  目标点坐标
@param[in] src  起始点坐标
@param[in] nTime 变换持续时间
@param[in] nCount 变换次数，即一次变换分为多少次完成。
@param[in] psDataSource 目标图片资源
@param[in] eStyle 灵活运动的方式，如加速、减速等等
@param[out] 无
@return 成功返回0,失败返回非0值
@note 无
*/
int CSEFTMoveEase (int client_id, GR_DRAW_ID id, GR_GC_ID gc, GR_POINT * dst, GR_POINT * src, 
			   int nTime, int nCount, CSEFTSource_S *psDataSource, CSANIMATIONSTYLE_E eStyle);




/**
@brief 缩放渐变显示图片

将指定图片在指定时间内，动态平滑地从开始指定大小变化为目标指定大小。
@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] psDst  图片目标变换区域
@param[in] psSrc  图片起始变换区域
@param[in] nTime 变换持续时间
@param[in] nCount 变换次数，即一次变换分为多少次完成。
@param[in] psDataSource  目标图片资源
@param[out] 无
@return 成功返回0,失败返回非0值
@note 无
*/
int CSEFTZoom (int client_id, GR_DRAW_ID id, GR_GC_ID gc, GR_RECT *psDst, GR_RECT *psSrc,
	int nTime, int nCount, CSEFTSource_S *psDataSource);
	
	
	
	
/**
@brief 缩放渐变显示图片

将指定图片在指定时间内，动态灵活地从开始指定大小变化为目标指定大小。
@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] psDst  图片目标变换区域
@param[in] psSrc  图片起始变换区域
@param[in] nTime 变换持续时间
@param[in] nCount 变换次数，即一次变换分为多少次完成。
@param[in] psDataSource  目标图片资源
@param[in] eStyle  灵活运动的方式，如加速、减速等等
@param[out] 无
@return 成功返回0,失败返回非0值
@note 无
*/
int CSEFTZoomEase (int client_id, GR_DRAW_ID id, GR_GC_ID gc, GR_RECT *psDst, GR_RECT *psSrc,
			   int nTime, int nCount, CSEFTSource_S *psDataSource, CSANIMATIONSTYLE_E eStyle);
			   
			   
/**
@brief 透明渐变显示图片

将指定图片在指定时间内，动态平滑地从起始透明度变化到目标透明度（与背景混合）。
@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] x  图片显示x坐标
@param[in] y  图片显示y坐标
@param[in] unFrom  起始Alpha值（0-255）
@param[in] unTo  目标Alpha值（0-255）
@param[in] nTime 变换持续时间
@param[in] nCount 变换次数，即一次变换分为多少次完成。
@param[in] psDataSource  目标图片资源
@param[out] 无
@return 成功返回0,失败返回非0值
@note 无
*/
int CSEFTTransparent (int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, 
	UCHAR unFrom, UCHAR unTo, int nTime, int nCount,  CSEFTSource_S *psDataSource);

/**
@brief Alpha混合显示图片

将指定的多张图片进行Alpha混合，可指定每张图片参与混合时所占的混合因子数。
混合时将第一张图片放在最底层，其它图片依次向上混合覆盖。图片最终大小以第一张图片大小为准。
@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] x  图片显示x坐标
@param[in] y  图片显示y坐标
@param[in] pnImageIDArray  需执行Alpha混合的图片的ID数组
@param[in] pucAlphaArray  参与Alpha混合的图片的混合因子数组
@param[out] 无
@return 成功返回0,失败返回非0值
@note 无
*/
int CSEFTBlend(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, GR_IMAGE_ID *pnImageIDArray, UCHAR *pucAlphaArray);

/**
@brief 画出镜像图片

画出指定图片的镜像变换后的新图片。
操作画出镜像后新图片，不改变原始图片数据。
@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] x   图片显示的x坐标
@param[in] y   图片显示的y坐标
@param[in] psDataSource  目标图片资源
@param[out] 无
@return 成功返回新的镜像部分形成的图片ID(正值),失败返回负值
@note 无
*/
int CSEFTMirror(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, CSEFTSource_S *psDataSource);


/**
@brief 获取倒影图片

获取指定图片的倒影变换后的新图片。
操作返回倒影后新图片的ID，不改变原始图片数据。
可指定新的图片相对原始图片的高度比（即竖直方向可压缩），及倒影时Alpha值的变换范围。
@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] nHeightPercent  倒影图片与原始图片的百分比（取值0-100）
@param[in] ucFromAlpha  起始Alpha值（0-255）
@param[in] ucToAlpha  目标Alpha值（0-255）
@param[in] psDataSource  目标图片资源
@param[out] 无
@return 成功返回新的倒影部分形成的图片ID(正值),失败返回负值
@note 无
*/
GR_IMAGE_ID CSEFTReflect (int client_id, GR_DRAW_ID id, GR_GC_ID gc, 
	int nHeightPercent, UCHAR ucFromAlpha, UCHAR ucToAlpha, CSEFTSource_S *psDataSource);

/**
@brief 阴影效果显示图片

将指定的图片以阴影效果显示出来。
即在显示图片的同时，在其右方及下方显示渐变灰色区域。
@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] x  图片显示x坐标
@param[in] y  图片显示y坐标
@param[in] nShadowWidth  阴影区域的宽度
@param[in] psDataSource  目标图片资源
@param[out] 无
@return 成功返回0,失败返回非0值
@note 无
*/
int CSEFTShadow (int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, int nShadowWidth, CSEFTSource_S *psDataSource);


/**
@brief 圆角显示图片

将指定的图片的四个角，按指定的半径圆角化，并显示到指定设备的指定区域上。
@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] x  图片显示x坐标
@param[in] y  图片显示y坐标
@param[in] nRoundRadius  圆角半径大小
@param[in] psDataSource  目标图片资源
@param[out] 无
@return 成功返回0,失败返回非0值
@note 无
*/
int CSEFTRound (int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, int nRoundRadius, CSEFTSource_S *psDataSource);


/**********************************************************************************************
 *								简单特效接口
 **********************************************************************************************/

/**
@brief 抖动特效

将指定的图片以抖动效果显示出来。
@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] x  图片显示x坐标
@param[in] y  图片显示y坐标
@param[in] eEftDir  抖动的方向，取以下值
	EM_EFFECT_DIRECTION_TYPE_HORIZON
	EM_EFFECT_DIRECTION_TYPE_VERTICAL
@param[in] nNum 抖动的次数
@param[in] nRange 抖动的幅度,即抖动偏移自身位置的距离
@param[in] nTime 抖动持续时间
@param[in] nCount 变换次数，即一次变换分为多少次完成。
@param[in] psDataSource 需抖动图片资源
@param[out] 无
@return 成功返回0,失败返回非0值
@note 无
*/
int CSEFTShake(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, 
	CSEFTDirectionType_E eEftDirection, int nNum, int nRange, int nTime, int nCount, CSEFTSource_S *psDataSource);

/**
@brief 滚动特效

将指定的图片以滚动效果显示出来。
滚动的特效，图像可以从四个方向像卷轴一样滚动打开
@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] x  图片显示x坐标
@param[in] y  图片显示y坐标
@param[in] eEftDir  滚动的方向，取以下值
	EM_EFFECT_DIRECTION_TYPE_LEFT,
	EM_EFFECT_DIRECTION_TYPE_TOP,
	EM_EFFECT_DIRECTION_TYPE_RIGHT,
	EM_EFFECT_DIRECTION_TYPE_BOTTOM
@param[in] nTime 持续时间
@param[in] nCount 变换次数，即一次变换分为多少次完成
@param[in] psDataSource 需变换的图片资源
@param[out] 无
@return 成功返回0,失败返回非0值
@note 与CSEFTShrink对应。
*/
int CSEFTRoll(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, CSEFTDirectionType_E eEftDir, 
	int nTime, int nCount, CSEFTSource_S *psDataSource);

/**
@brief 收缩特效

将指定的图片以收缩效果隐藏进去。
收缩的特效，图像可以从四个方向像卷轴一样收缩上去。
@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] x  图片显示x坐标
@param[in] y  图片显示y坐标
@param[in] eEftDir  收缩的方向，取以下值
	EM_EFFECT_DIRECTION_TYPE_LEFT,
	EM_EFFECT_DIRECTION_TYPE_TOP,
	EM_EFFECT_DIRECTION_TYPE_RIGHT,
	EM_EFFECT_DIRECTION_TYPE_BOTTOM
@param[in] nTime 持续时间
@param[in] nCount 变换次数，即一次变换分为多少次完成
@param[in] psDataSource 需收缩图片资源
@param[out] 无
@return 成功返回0,失败返回非0值
@note 与CSEFTRoll对应。
*/
int CSEFTShrink(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, CSEFTDirectionType_E eEftDir, 
	int nTime, int nCount, CSEFTSource_S *psDataSource);

/**
@brief 消失特效

对象透明度从大到小直至消失
@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] x  图片显示x坐标
@param[in] y  图片显示y坐标
@param[in] nTime 持续时间
@param[in] nCount 变换次数，即一次变换分为多少次完成
@param[in] psDataSource 需变换的图片资源
@param[out] 无
@return 成功返回0,失败返回非0值
@note 模拟图片从显到隐的过程，不能指定透明度变化区间。与CSEFTAppear对应。
*/
int CSEFTDisappear(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, 
	int nTime, int nCount, CSEFTSource_S *psDataSource);

/**
@brief 出现特效

对象透明度从小到大直至完全显示
@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] x  图片显示x坐标
@param[in] y  图片显示y坐标
@param[in] nTime 持续时间
@param[in] nCount 变换次数，即一次变换分为多少次完成
@param[in] psDataSource 需变换的图片资源
@param[out] 无
@return 成功返回0,失败返回非0值
@note 模拟图片从隐到显的过程，不能指定透明度变化区间。与CSEFTDisAppear对应。
*/
int CSEFTAppear(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, int nTime, int nCount, CSEFTSource_S *psDataSource);

/**
@brief 膨胀特效

对象逐渐变大，同时变淡，直至消失
@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] x  图片显示x坐标
@param[in] y  图片显示y坐标
@param[in] nTime 变换间隔时间，即每次变换持续的时间。
@param[in] nCount 变换次数，即一次变换分为多少次完成
@param[in] psDataSource 需变换的图片资源
@param[out] 无
@return 成功返回0,失败返回非0值
@note 模拟像烟花绽放后消失的过程。
*/
int CSEFTPuff(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, int nTime, int nCount, CSEFTSource_S *psDataSource);

/**
@brief 飞入特效

将指定对象从指定方向飞入到屏幕的指定位置
@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] x  图片显示x坐标
@param[in] y  图片显示y坐标
@param[in] eEftDir  飞入的起始方向，取以下值
	EM_EFFECT_DIRECTION_TYPE_LEFT,
	EM_EFFECT_DIRECTION_TYPE_TOP,
	EM_EFFECT_DIRECTION_TYPE_RIGHT,
	EM_EFFECT_DIRECTION_TYPE_BOTTOM,
	EM_EFFECT_DIRECTION_TYPE_LT,
	EM_EFFECT_DIRECTION_TYPE_LB,
	EM_EFFECT_DIRECTION_TYPE_RT,
	EM_EFFECT_DIRECTION_TYPE_RB  
@param[in] nTime 变换间隔时间，即每次变换持续的时间。
@param[in] nCount 变换次数，即一次变换分为多少次完成
@param[in] psDataSource 需变换的图片资源
@param[out] 无
@return 成功返回0,失败返回非0值
@note 与CSEFTFlyOut对应,图片的显示会超出图片原本所占区域大小。
*/
int CSEFTFlyIn(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, 
	CSEFTDirectionType_E eEftDir, int nTime, int nCount, CSEFTSource_S *psDataSource);

/**
@brief 飞出特效

对象从原先位置飞出可见范围
@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] x  图片显示x坐标
@param[in] y  图片显示y坐标
@param[in] eEftDir  飞出的方向，取以下值
	EM_EFFECT_DIRECTION_TYPE_LEFT,
	EM_EFFECT_DIRECTION_TYPE_TOP,
	EM_EFFECT_DIRECTION_TYPE_RIGHT,
	EM_EFFECT_DIRECTION_TYPE_BOTTOM,
	EM_EFFECT_DIRECTION_TYPE_LT,
	EM_EFFECT_DIRECTION_TYPE_LB,
	EM_EFFECT_DIRECTION_TYPE_RT,
	EM_EFFECT_DIRECTION_TYPE_RB  
@param[in] nTime 变换间隔时间，即每次变换持续的时间。
@param[in] nCount 变换次数，即一次变换分为多少次完成
@param[in] psDataSource 需变换的图片资源
@param[out] 无
@return 成功返回0,失败返回非0值
@note 与CSEFTFlyIn对应, 图片的显示会超出图片原本所占区域大小。
*/
int CSEFTFlyOut(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, 
	CSEFTDirectionType_E eEftDir,  int nTime, int nCount, CSEFTSource_S *psDataSource);

/**
@brief 切入特效

对象沿指定方向移动出现
图片的显示不会超出图片原本所占区域大小
@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] x  图片显示x坐标
@param[in] y  图片显示y坐标
@param[in] eEftDir  切入的方向，取以下值
	EM_EFFECT_DIRECTION_TYPE_LEFT,
	EM_EFFECT_DIRECTION_TYPE_TOP,
	EM_EFFECT_DIRECTION_TYPE_RIGHT,
	EM_EFFECT_DIRECTION_TYPE_BOTTOM,
@param[in] nTime 变换间隔时间，即每次变换持续的时间。
@param[in] nCount 变换次数，即一次变换分为多少次完成
@param[in] psDataSource 需变换的图片资源
@param[out] 无
@return 成功返回0,失败返回非0值
@note 与CSEFTCutOut对应.
*/
int CSEFTCutIn(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, 
	CSEFTDirectionType_E eEftDir,  int nTime, int nCount, CSEFTSource_S *psDataSource);




/**
@brief 切入特效

对象沿指定方向移动出现
图片的显示不会超出图片原本所占区域大小
@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] x  图片显示x坐标
@param[in] y  图片显示y坐标
@param[in] eEftDir  切入的方向，取以下值
	EM_EFFECT_DIRECTION_TYPE_LEFT,
	EM_EFFECT_DIRECTION_TYPE_TOP,
	EM_EFFECT_DIRECTION_TYPE_RIGHT,
	EM_EFFECT_DIRECTION_TYPE_BOTTOM,
@param[in] nTime 变换间隔时间，即每次变换持续的时间。
@param[in] nCount 变换次数，即一次变换分为多少次完成
@param[in] psDataSource 需变换的图片资源
@param[in] eStyle 灵活运动的方式，如加速、减速等等
@param[out] 无
@return 成功返回0,失败返回非0值
@note 与CSEFTCutOut对应.
*/
int CSEFTCutInEase(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, 
			   CSEFTDirectionType_E eEftDir,  int nTime, int nCount, CSEFTSource_S *psDataSource, CSANIMATIONSTYLE_E eStyle);


/**
@brief 切出特效

对象沿指定方向移动消失
图片的显示不会超出图片原本所占区域大小
@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] x  图片显示x坐标
@param[in] y  图片显示y坐标
@param[in] eEftDir  切入的方向，取以下值
	EM_EFFECT_DIRECTION_TYPE_LEFT,
	EM_EFFECT_DIRECTION_TYPE_TOP,
	EM_EFFECT_DIRECTION_TYPE_RIGHT,
	EM_EFFECT_DIRECTION_TYPE_BOTTOM,
@param[in] nTime 变换间隔时间，即每次变换持续的时间。
@param[in] nCount 变换次数，即一次变换分为多少次完成
@param[in] psDataSource 需变换的图片资源
@param[out] 无
@return 成功返回0,失败返回非0值
@note 与CSEFTCutIn对应.
*/
int CSEFTCutOut(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, 
	CSEFTDirectionType_E eEftDir,  int nTime, int nCount, CSEFTSource_S *psDataSource);





/**
@brief 切出特效

对象沿指定方向移动消失
图片的显示不会超出图片原本所占区域大小
@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] x  图片显示x坐标
@param[in] y  图片显示y坐标
@param[in] eEftDir  切入的方向，取以下值
	EM_EFFECT_DIRECTION_TYPE_LEFT,
	EM_EFFECT_DIRECTION_TYPE_TOP,
	EM_EFFECT_DIRECTION_TYPE_RIGHT,
	EM_EFFECT_DIRECTION_TYPE_BOTTOM,
@param[in] nTime 变换间隔时间，即每次变换持续的时间。
@param[in] nCount 变换次数，即一次变换分为多少次完成
@param[in] psDataSource 需变换的图片资源
@param[in] eStyle 灵活运动的方式，如加速、减速等等
@param[out] 无
@return 成功返回0,失败返回非0值
@note 与CSEFTCutIn对应.
*/
int CSEFTCutOutEase(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, 
				CSEFTDirectionType_E eEftDir,  int nTime, int nCount, CSEFTSource_S *psDataSource, CSANIMATIONSTYLE_E eStyle);

/**
@brief 百叶窗特效

对象纵向或横向百叶窗效果
图片的显示不会超出图片原本所占区域大小
@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] x  图片显示x坐标
@param[in] y  图片显示y坐标
@param[in] eEftDir  切入的方向，取以下值
EM_EFFECT_DIRECTION_TYPE_VERTICAL,
EM_EFFECT_DIRECTION_TYPE_HORIZON,
@param[in] nTime 变换间隔时间，即每次变换持续的时间。
@param[in] nCount 变换次数，即一次变换分为多少次完成
@@param[in] nPiece  百叶窗的页数
@param[in] psDataSource 需变换的图片资源
@param[out] 无
@return 成功返回0,失败返回非0值
@note 与CSEFTCutOut对应.
*/
int CSEFTShutter(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, 
				 CSEFTDirectionType_E eEftDir,  int nTime, int nCount,int nPiece, CSEFTSource_S *psDataSource);


/**
@brief 在图片上显示水波纹效果

。
@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] xOff  2D元素相对于屏幕(0,0)点x轴偏移
@param[in] yOff  2D元素相对于屏幕(0,0)点y轴偏移
@param[in] gc  画图上下文ID
@param[in] x  扰动中心的x坐标
@param[in] y  扰动中心的y坐标
@param[in] nRadius  扰动的半径（建议1~30）
@param[in] nStrength 扰动的强度（建议1~200）
@param[in] psDataSource  目标图片资源
@param[out] 无
@return 成功返回0,失败返回非0值
@note 无
*/
int CSEFTWaterWave(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int xOff, int yOff, int x, int y, int nRadius, int nStrength,
			   CSEFTSource_S *psDataSource);


/**
@brief 将两张图片以翻页效果展示，注意两页图片的大小必须一致

。
@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] x  图片显示的偏移横坐标
@param[in] y  图片显示的偏移纵坐标
@param[in] psDataSource1  当前页的图片
@param[in] psDataSource2 下一页的图片

@param[out] 无
@return 成功返回0,失败返回非0值
@note 无
*/
int CSEFTFlipPage(int client_id, GR_DRAW_ID id, GR_GC_ID gc,int x, int y,
				  CSEFTSource_S *psDataSource1, CSEFTSource_S *psDataSource2);





/**********************************************************************************************
 *								组合特效接口
 **********************************************************************************************/
 
/**
@brief 渐变缩放特效

对象有大小和透明度的变换
@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] psDst  图片目标变换区域
@param[in] psSrc  图片起始变换区域
@param[in] nStartOpacity 图片开始透明度，取值(0-255)
@param[in] nEndOpacity 图片结束透明度，取值(0-255)
@param[in] nTime 变换间隔时间，即每次变换持续的时间。
@param[in] nCount 变换次数，即一次变换分为多少次完成
@param[in] psDataSource 需变换的图片资源
@param[out] 无
@return 成功返回0,失败返回非0值
@note 无
*/
int CSEFTBlendZoom (int client_id, GR_DRAW_ID id, GR_GC_ID gc, GR_RECT *psDst, GR_RECT *psSrc,
	   UCHAR nStartOpacity, UCHAR nEndOpacity,
	   int nTime, int nCount, CSEFTSource_S *psDataSource);



/**
@brief 渐变缩放特效

对象有大小和透明度的变换
@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] psDst  图片目标变换区域
@param[in] psSrc  图片起始变换区域
@param[in] nStartOpacity 图片开始透明度，取值(0-255)
@param[in] nEndOpacity 图片结束透明度，取值(0-255)
@param[in] nTime 变换间隔时间，即每次变换持续的时间。
@param[in] nCount 变换次数，即一次变换分为多少次完成
@param[in] psDataSource 需变换的图片资源
@param[in] eStyle 灵活运动的方式，如加速、减速等等
@param[out] 无
@return 成功返回0,失败返回非0值
@note 无
*/
int CSEFTBlendZoomEase (int client_id, GR_DRAW_ID id, GR_GC_ID gc, GR_RECT *psDst, GR_RECT *psSrc,
					UCHAR nStartOpacity, UCHAR nEndOpacity,
					int nTime, int nCount, CSEFTSource_S *psDataSource, CSANIMATIONSTYLE_E eStyle);


/**
@brief 翻页特效

一幅图片分成两半，两半图片对象沿指定方向移动
@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] x  图片显示x坐标
@param[in] y  图片显示y坐标
@param[in] eEftDir  切入的方向，取以下值
	EM_EFFECT_DIRECTION_TYPE_LEFT,
	EM_EFFECT_DIRECTION_TYPE_TOP,
	EM_EFFECT_DIRECTION_TYPE_RIGHT,
	EM_EFFECT_DIRECTION_TYPE_BOTTOM,
@param[in] nTime 变换间隔时间，即每次变换持续的时间。
@param[in] nCount 变换次数，即一次变换分为多少次完成
@param[in] psDataSource 需变换的图片资源
@param[in] nStartOpacity  翻动的一页的起始透明度
@param[in] nEndOpacity 翻动的一页的终止透明度
@param[out] 无
@return 成功返回0,失败返回非0值
@note 
*/

int CSEFTFlip(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, 
			   CSEFTDirectionType_E eEftDir,  int nTime, int nCount, CSEFTSource_S *psDataSource,int nStartOpacity,int nEndOpacity);


/**
@brief 翻页特效

一幅图片分成两半，两半图片对象沿指定方向移动
@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] x  图片显示x坐标
@param[in] y  图片显示y坐标
@param[in] eEftDir  切入的方向，取以下值
	EM_EFFECT_DIRECTION_TYPE_LEFT,
	EM_EFFECT_DIRECTION_TYPE_TOP,
	EM_EFFECT_DIRECTION_TYPE_RIGHT,
	EM_EFFECT_DIRECTION_TYPE_BOTTOM,
@param[in] nTime 变换间隔时间，即每次变换持续的时间。
@param[in] nCount 变换次数，即一次变换分为多少次完成
@param[in] psDataSource 需变换的图片资源
@param[in] nStartOpacity  翻动的一页的起始透明度
@param[in] nEndOpacity 翻动的一页的终止透明度
@param[in] eStyle 灵活运动的方式，如加速、减速等等
@param[out] 无
@return 成功返回0,失败返回非0值
@note 
*/

int CSEFTFlipEase(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, 
			   CSEFTDirectionType_E eEftDir,  int nTime, int nCount, CSEFTSource_S *psDataSource,int nStartOpacity,int nEndOpacity,CSANIMATIONSTYLE_E eStyle);

/**
@brief 图片显示移入特效

一幅图片沿指定方向慢慢显示出来，宽度(高度)越来越大，最开始完全

@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] x  图片显示x坐标
@param[in] y  图片显示y坐标
@param[in] eEftDir  移入的方向，取以下值
	EM_EFFECT_DIRECTION_TYPE_LEFT,
	EM_EFFECT_DIRECTION_TYPE_TOP,
	EM_EFFECT_DIRECTION_TYPE_RIGHT,
	EM_EFFECT_DIRECTION_TYPE_BOTTOM,
@param[in] nTime 变换间隔时间，即每次变换持续的时间。
@param[in] nCount 变换次数，即一次变换分为多少次完成
@param[in] psDataSource 需变换的图片资源
@param[out] 无
@return 成功返回0,失败返回非0值
@note 与CSEFTShowOut对应.
*/
int CSEFTShowIn(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, 
	CSEFTDirectionType_E eEftDir,  int nTime, int nCount, CSEFTSource_S *psDataSource);




/**
@brief 图片显示移入特效

一幅图片沿指定方向慢慢显示出来，宽度(高度)越来越大，最开始完全

@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] x  图片显示x坐标
@param[in] y  图片显示y坐标
@param[in] eEftDir  移入的方向，取以下值
	EM_EFFECT_DIRECTION_TYPE_LEFT,
	EM_EFFECT_DIRECTION_TYPE_TOP,
	EM_EFFECT_DIRECTION_TYPE_RIGHT,
	EM_EFFECT_DIRECTION_TYPE_BOTTOM,
@param[in] nTime 变换间隔时间，即每次变换持续的时间。
@param[in] nCount 变换次数，即一次变换分为多少次完成
@param[in] psDataSource 需变换的图片资源
@param[in] eStyle 灵活运动的方式，如加速、减速等等
@param[out] 无
@return 成功返回0,失败返回非0值
@note 与CSEFTShowOut对应.
*/
int CSEFTShowInEase(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, 
			   CSEFTDirectionType_E eEftDir,  int nTime, int nCount, CSEFTSource_S *psDataSource, CSANIMATIONSTYLE_E eStyle);

/**
@brief 图片显示移出特效

	一幅图片沿指定方向慢慢消失不见，宽度(高度)越来越小
	
@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] x  图片显示x坐标
@param[in] y  图片显示y坐标
@param[in] eEftDir	移出的方向，取以下值
	EM_EFFECT_DIRECTION_TYPE_LEFT,
	EM_EFFECT_DIRECTION_TYPE_TOP,
	EM_EFFECT_DIRECTION_TYPE_RIGHT,
	EM_EFFECT_DIRECTION_TYPE_BOTTOM,
@param[in] nTime 变换间隔时间，即每次变换持续的时间。
@param[in] nCount 变换次数，即一次变换分为多少次完成
@param[in] psDataSource 需变换的图片资源
@param[out] 无
@return 成功返回0,失败返回非0值
@note 与CSEFTShowIn对应.
*/
int CSEFTShowOut(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, 
	CSEFTDirectionType_E eEftDir,  int nTime, int nCount, CSEFTSource_S *psDataSource);





/**
@brief 图片显示移出特效

	一幅图片沿指定方向慢慢消失不见，宽度(高度)越来越小
	
@param[in] client_id Client ID值
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] x  图片显示x坐标
@param[in] y  图片显示y坐标
@param[in] eEftDir	移出的方向，取以下值
	EM_EFFECT_DIRECTION_TYPE_LEFT,
	EM_EFFECT_DIRECTION_TYPE_TOP,
	EM_EFFECT_DIRECTION_TYPE_RIGHT,
	EM_EFFECT_DIRECTION_TYPE_BOTTOM,
@param[in] nTime 变换间隔时间，即每次变换持续的时间。
@param[in] nCount 变换次数，即一次变换分为多少次完成
@param[in] psDataSource 需变换的图片资源
@param[in] eStyle 灵活运动的方式，如加速、减速等等
@param[out] 无
@return 成功返回0,失败返回非0值
@note 与CSEFTShowIn对应.
*/
int CSEFTShowOutEase(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, 
				CSEFTDirectionType_E eEftDir,  int nTime, int nCount, CSEFTSource_S *psDataSource, CSANIMATIONSTYLE_E eStyle);



/**
@brief 通用特效接口

通用特效完成 坐标位置，区域大小，透明度变换三个层次的同时变换。
它针对的是单个图片资源的变换，支持背景自动维护。
@param[in] nClientID Client ID值
@param[in] nDrawID  窗口ID
@param[in] nGCID  画图上下文ID
@param[in] psUnit 变换轨迹（帧数据）
@param[out] 无
@return 成功返回0,失败返回非0值
@note fixme:暂时不考虑Z序
*/
CSEFTRet_E CSEFTGeneral (int nClientID, GR_DRAW_ID nDrawID, GR_GC_ID nGCID,
	CSEFTUnit_S *psUnit);

/**
@brief 开始并行特效接口

执行并行特效前，需调用此接口来进行标识
@param[in] 无
@param[out] 无
@return 成功返回0,失败返回非0值
@note:无
*/
CSEFTRet_E CSEFTBeginParallel();

/**
@brief 停止并行特效接口

如不需要进行并行特效了，调用该接口停止并行特效，此时所有特效均不执行
@param[in] pvReserve 预留扩展，必须为NULL
@param[out] 无
@return 成功返回0,失败返回非0值
@note:无
*/
CSEFTRet_E CSEFTStopParallel(void* pvReserve);

/**
@brief结束并行特效接口

结束并行特效的标志
@param[in] nClientID Client ID值
@param[in] nDrawID  窗口ID
@param[in] nGCID  画图上下文ID
@param[out] 无
@return 成功返回0,失败返回非0值
@note:无
*/
CSEFTRet_E CSEFTEndParallel(int nClientID, GR_DRAW_ID nDrawID, GR_GC_ID nGCID);

/**
@brief设置特效执行的裁剪区域

设置特效执行的裁剪区域，如果设置了区域，则特效的执行会被限制在该区域内，
超出区域的部分会被裁剪掉；没有设置区域，特效的位置和区域则不会被限制。
@param[in] psClip 特效设置的裁剪区域
@param[out] 无
@return 成功返回0,失败返回非0值
@note:无
*/
CSEFTRet_E CSEFTSetClip(GR_RECT *psClip);

/**
@brief CSCreateImageFlatWall

用户传入指定的ImageFlatWall模型参数，行、列、每张图片的宽、高，是否有倒影，屏幕的宽、高。
@param[in] client_id
@param[in] id  窗口ID
@param[in] gc  画图上下文ID
@param[in] psLayoutParam  特效的排版参数
@param[out] 无
@return 成功返回0,失败返回非0值
@note 无
*/
int CSFlatWallInit(int client_id,  GR_DRAW_ID id,GR_GC_ID gc,CSFlatwallLayoutParam_S *psLayoutParam);

/**
@brief CSFlatWallAddImage

加载图片。
@param[in] file  文件路径
@param[out] 无
@return 成功返回0,失败返回非0值
@note 无
*/
int CSFlatWallAddImage(char *file);

/**
@brief CSFlatWallShow

显示平面特效。
@param[in]无
@param[out] 无
@return 成功返回0,失败返回非0值
@note 无
*/
int CSFlatWallShow();

/**
@brief CSFlatWallOnKeyProcess

平面特效的按键响应
@param[in] type
@param[in] keyCode
@param[out] 无
@return 成功返回0,失败返回非0值
@note 无
*/

int CSFlatWallOnKeyProcess(GR_EVENT_TYPE type,int keyCode);


/**
@brief CSFlatWallUnInit

平面特效的资源释放
@param[in] 无
@param[out] 无
@return 成功返回0,失败返回非0值
@note 无
*/
int CSFlatWallUnInit();


typedef enum {
	EM_GLOBALEFFECT_NONE = 0,     ///<没有特效
	EM_GLOBALEFFECT_MOVE ,     ///<移动特效
	EM_GLOBALEFFECT_ZOOM ,         ///<缩放特效
	EM_GLOBALEFFECT_TRANSPARENT,         ///<透明特效
	EM_GLOBALEFFECT_CUTIN,         ///<切入切出特效
	EM_GLOBALEFFECT_SHUTTER,   ///<百叶窗特效
	EM_GLOBALEFFECT_WATERWAVE,   ///<百叶窗特效
	EM_GLOBALEFFECT_PAGEFLIP,   ///<百叶窗特效
	EM_GLOBALEFFECT_COUNT   ///< 不起作用，仅记录特效种类
} CSGLOABLEFFECTSTYLE_E;

/**
@brief设置全局特效的风格

设置特效执行的裁剪区域，如果设置了区域，则特效的执行会被限制在该区域内，
超出区域的部分会被裁剪掉；没有设置区域，特效的位置和区域则不会被限制。
@param[in] emEFTType 特效的类型
@param[out] 无
@return 成功返回0,失败返回非0值
@note:无
*/
CSEFTRet_E CSEFTSetGlobalStyle(CSGLOABLEFFECTSTYLE_E emEFTType);


/**
@brief设置特效的初始化

@param[in] 无
@param[out] 无
@return 无
@note:无
*/
void CSEFTInit();

#ifdef  __cplusplus
}
#endif

/** @} */

#endif


