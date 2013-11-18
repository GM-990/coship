/**@defgroup CSOSG osg 统一的图形相关接口，如画布、显存地址操作及基本画图操作等
@brief 提供统一的图形相关接口，如画布、显存地址操作及基本画图操作等

@note 坐标系统:X轴从左至右延伸，Y轴从上至下延伸
@version 0.1 2008/10/17 演示版本
@{
*/
#ifndef _UDI_OSG_H_
#define _UDI_OSG_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 *                                 Header File Include                         *
 ******************************************************************************/
#include "udi_device.h"

/**@brief 返回该值表示成功*/
#define CSOSG_SUCCESS		(0)
/**@brief 返回该值表示失败*/
#define CSOSG_FAILURE		(1)

/**@brief 表示调色板类型为RGB的宏*/
#define CSGraphPalette_TYPE_RGB 0	
/**@brief 表示调色板类型为YUV的宏*/
#define CSGraphPalette_TYPE_YUV 1	

/**@brief 全局的ALPHA模式*/
#define CS_ALPHA_MODE_GLOBAL	1
/**@brief 局部ALPHA模式*/
#define CS_ALPHA_MODE_LOCAL	2

/**@brief 图像表示层类型--硬件显示*/    
#define CS_SURFACE_TYPE_HARDWARE  0		//used
/**@brief 图像表示层类型--仅存放在内存*/
#define CS_SURFACE_TYPE_MEMORY	1		//used

/**@brief 画线类型--连续线（实线），缺省时的模式*/
#define CS_CONTINUES_LINE_SOLID	 		0	
/**@brief 画线类型--由前景色和透明色间隔的虚线*/
#define CS_TRANSPARENCY_DASHED_LINE_ON_OFF_DASH 1
/**@brief 画线类型--由前景色和背景色间隔的虚线, 其虚线的具体格式由dash_format定义。*/
#define CS_BACKCOLOR_DASHED_LINE_DOUBLE_DASH 	2	

/**@brief 末端风格--方形末端，缺省时的模式 */
#define CS_CAP_STYLE_RECTBUTT_LINE_ENDED	0	
/**@brief 末端风格--圆形末端  */
#define CS_CAP_STYLE_ROUND_LINE_ENDED		1	
/**@brief 末端风格-- 工程投影线末端*/
#define CS_CAP_STYLE_PROJECTING_LINE_ENDED 	2	

/**@brief 连接处风格-- 角形连接模式，缺省时的模式 */
#define CS_ANGLE_JOIN_MODESTYLE_MITER	0	
/**@brief 连接处风格-- 圆形连接模式 */
#define CS_ROUND_JOIN_STYLEMODE_ROUND	1	
/**@brief 连接处风格-- 平形斜面连接模式*/
#define CS_FLAT_JOIN_STYLEMODE_BEVEL	2	

/*poly_fill_style：该变量用于决定多边形的填充方法。定义如下：*/
/**@brief 多边形填充类型--环绕充模式,缺省时地模式*/
#define CS_POLY_FILL_STYLE_WINDING	0	
/**@brief 多边形填充类型--奇偶填充模式*/
#define CS_POLY_FILL_STYLE_EVENODD	1	

/*arc_style：该变量定义了弧形的绘制和填充模式。定义如下：     */
/**@brief 弧形的绘制和填充模式--弦扇性形封闭区域，缺省时的模式 */
#define CS_ARC_STYLE_CENTERCHORD_ARC_MODE 	0	
/**@brief 弧形的绘制和填充模式--弧Pie形封闭区域 */
#define CS_ARC_STYLE_LINEPIESLICE_ARC_MODE 	1	

/*draw mode*/
#define	CS_MODE_COPY		0	/**< src */
#define	CS_MODE_XOR		1	/**< src ^ dst */
#define	CS_MODE_OR		2	/**< src | dst */
#define	CS_MODE_AND		3	/**< src & dst */
#define	CS_MODE_CLEAR		4	/**< 0 */
#define	CS_MODE_SET		5	/**< 11111111 */
#define	CS_MODE_EQUIV		6	/**< ~(src ^ dst) */
#define	CS_MODE_NOR		7	/**< ~(src | dst) */
#define	CS_MODE_NAND		8	/**< ~(src & dst) */
#define	CS_MODE_INVERT		9	/**< ~dst */
#define	CS_MODE_COPYINVERTED	10	/**< ~src */
#define	CS_MODE_ORINVERTED	11	/**< ~src | dst */
#define	CS_MODE_ANDINVERTED	12	/**< ~src & dst */
#define	CS_MODE_ORREVERSE	13	/**< src | ~dst */
#define	CS_MODE_ANDREVERSE	14	/**< src & ~dst */
#define	CS_MODE_NOOP		15	/**< dst */
#define	CS_MODE_ALPHA		16	/**< Alpha mode */

/*clip flag*/
#define CS_CLIPPING_NO_RECT_NO_MASK	0	/**< 无剪切无屏蔽，pixel_mode在整个区域有效 */
#define CS_CLIPPING_VALID_RECT_NO_MASK	1	/**< 有剪切无屏蔽,pixel_mode仅在剪切矩形区域内有效(包括边界) */
#define CS_CLIPPING_NO_RECT_VALID_MASK	2	/**< 无剪切有屏蔽,pixel_mode仅在剪切屏蔽区域,且该屏蔽设置为1时有效，包括屏蔽区域边界 */
#define CS_CLIPPING_VALID_RECT_AND_MASK	3	/**< 有剪切有屏蔽,pixel_mode仅在剪切区域和没有交集，则不执行任何操作。如果有一块交集，pixel_mode将应用于屏蔽设置为1的那一部分 */

/*usual error code*/
#define CS_GRAPH_NO_ERROR	0	/**< 成功*/
#define CS_SURFACE_INVALID	-1	/**< 指针所指绘图平面无效 */
#define CS_REGION_INVALID	-2	/**< 所指定的功能不支持 */
#define CS_PARAMETER_INVALID	-3	/**< 参数无效 */
#define CS_ATTRIBUTE_INVALID	-4	/**< 所设置的绘图参数无效 */
#define CS_NO_ENOUGH_MEMORY	-5	/**< 分配内存失败 */
#define CS_SIZE_INVALID		-6	/**< 区域大小无效（width，height超出系统支持的坐标范围） */
#define CS_COLOR_MODE_INVALID	-7	/**< 颜色模式(bpp)不支持或参数无效 */
#define CS_ALPHA_MODE_INVALID	-8	/**< Alpha blending模式不支持或参数无效 */
#define CS_SYSTEM_NOT_INITED	-9	/**< 图形系统未初始化 */

/**@brief 定义点的结构体*/
typedef struct _CSPoint {
	INT x;	/**< 点的x坐标*/
	INT y;	/**< 点的y坐标*/
}CSGraphPoint;

/**@brief 采用RGB分量表示的颜色结构体*/
typedef struct _CSRGBColor {
	UCHAR a;		/**< 透明因子*/
	UCHAR r;		/**< 红色分量*/
	UCHAR g;		/**< 绿色分量*/
	UCHAR b;		/**< 蓝色分量*/
} CSGraphRGBColor;

/**@brief 采用YUV分量表示的颜色结构体*/
typedef struct _CSYUVColor {
	UCHAR a;		/**< alpha值*/
	UCHAR y;		/**< 亮度分量*/
	UCHAR u;		/**< 蓝色色度分量*/
	UCHAR v;		/**< 红色色度分量*/
} CSGraphYUVColor;

/**@brief 调色板结构体*/
typedef  struct _CSPalette{
	UCHAR	type;				/**< 颜色模式CSGraphPalette_TYPE_RGB或者CSGraphPalette_TYPE_YUV*/
	USHORT	ncolors;				/**< 颜色数组长度*/
	union {
		CSGraphRGBColor	*rgb;	/**< 颜色数组相当于rgb[ncolors]对应的type为CSGraphPalette_TYPE_RGB*/
		CSGraphYUVColor	*yuv;	/**< 颜色数组相当于yuv[ncolors]对应的type为CSGraphPalette_TYPE_YUV*/
	} col;
} CSGraphPalette;


/**@brief 位图格式结构体*/
typedef struct _CSPixelFormat {
	UCHAR  BitsPerPixel;			/**< 每个像素的位数*/
	UCHAR  BytesPerPixel;		/**< 每个像素的字节数*/
	UCHAR  Rloss;				/**< red loss*/
	UCHAR  Gloss;				/**< gree loss*/
	UCHAR  Bloss;				/**< blue loss*/
	UCHAR  Aloss;				/**< alpha loss*/
	UCHAR  Rshift;				/**< red shift*/
	UCHAR  Gshift;				/**< green shift*/
	UCHAR  Bshift;				/**< blue shift*/
	UCHAR  Ashift;				/**< alpha shift*/
	UINT   Rmask;				/**< red mask*/
	UINT   Gmask;				/**< green mask*/
	UINT   Bmask;				/**< blue mask*/
	UINT   Amask;				/**< alpha mask*/
	UINT   colorkey;				/**< RGB color key information */
	UCHAR  transparence;		/**< 透明度Alpha value information (per-surface alpha) */
	UINT   alpha_blending_mode;	/**< alpha混合模式CS_ALPHA_MODE_GLOBAL or CS_ALPHA_MODE_LOCAL*/
} CSGraphPixelFormat;

/**@brief 位图结构体*/
typedef struct _CSPixmap{
	USHORT				width;		/**< image width in pixels*/
	USHORT				height;		/**< image height in pixels*/
	UCHAR				planes;		/**< image planes*/
	USHORT				pitch;		/**< bytes per line*/
	UCHAR				compression;		/**< compression algorithm*/
	CSGraphPixelFormat		pixel_format;	/**< bits per pixel(1,4 or 8,16,24,32)*/
	CSGraphPalette			*palette;		/**< 调色板地址*/
	UCHAR  		        	*imagebits;			/**< image bits (INT right aligned)*/
	UCHAR				colorkeyflag;   		/**< indicate whether to use colorkey in pixel_format */
	UCHAR				alphaflag;      		/**< indicate whether to use alpha mode in pixel_format */
	UCHAR  		        	*YUVbits;			/**< image yuv bits (INT right aligned)*/
	USHORT				YUVwidth;		/**< imagev yuv  width in pixels*/
	USHORT				YUVheight;		/**< image yuv height in pixels*/
}CSGraphPixmap;



typedef struct	_CSGC{
	INT		font_id;		/**< 字库指针*/
	INT		id;			/**< Identifier */
	UINT		background_color;	/**< 背景颜色					*/
	UINT		foreground_color;	/**< 前景颜色					*/
	UINT		pen_width;		/**< 画笔宽度					*/
	UINT		pen_style;		/**< 画笔类型					*/
	UCHAR	       *dash_format;		/**< 虚线格式					*/
	UINT		dash_length;		/**< 虚线长度					*/
	UCHAR	        cap_style;		/**< 末端类型					*/
	UCHAR	        join_style;		/**< 连接类型					*/
	UCHAR	        poly_fill_style;	/**< 多边形填充类型				*/
	UCHAR	        arc_style;		/**< 弧形模式					*/
	UCHAR		blending_factor;	/**< 混合因子（透明度）			*/
	CHAR		pixel_mode;		/**< 新的象素写入内存时对旧的象素的关系*/
	CHAR		clip_flag;		/**< 剪切标志					*/
	SHORT	        clip_area_x;		/**< 剪切矩形x坐标				*/
	SHORT	        clip_area_y;		/**< 剪切矩形y坐标				*/
	SHORT	        clip_area_width;	/**< 剪切矩形宽度				*/
	SHORT	        clip_area_height;	/**< 剪切矩形高度				*/
	SHORT	        clip_mask_x;		/**< 剪切屏蔽矩形x坐标			*/
	SHORT	        clip_mask_y;		/**< 剪切屏蔽矩形y坐标			*/
	SHORT	        clip_mask_width;	/**< 剪切屏蔽矩形宽度			*/
	SHORT	        clip_mask_height;	/**< 剪切屏蔽矩形高度			*/
	CSGraphPixmap	*pixmap_mask;	        /**< 剪切屏蔽象素图指针			*/
	UINT		plane_mask;		/**< 被绘图影响到的每个象素位掩码	*/
} CSGraphGC;


/**@brief 显示设备*/
typedef struct _CSDisplayDevice {
	CHAR		*name;			        /**< 设备名称*/
	INT			width;			        /**< 显示区域宽度,即屏幕分辨率,如果支持硬件缩放则该值仅供参考*/
	INT			height;			        /**< 显示区域宽度,即屏幕分辨率,如果支持硬件缩放则该值仅供参考*/
	INT			region_overlapable;	    /**< Region是否支持重叠:0-不支持,1-支持;即*/
	INT			anti_aliasing;		    /**< 是否支持折叠,0-不支持,1-支持*/
	CSGraphPixelFormat	*display_mode;	/**< 显示模式*/
	INT			display_mem_size;	    /**< 显存大小，以字节为单位*/
	void		*display_mem;		    /**< 显存地址*/
}CSGraphDisplayDevice;


/**@brief Surface类似于画布的意思*/
typedef struct _CSSurface {
	INT	                id;			/**< The Surface's identifier */
	struct _CSRegion   	*associated_region;	/**< 该Surface所关联的region*/
	CHAR                	type;  			/**< The type of the surface - memSurface(CS_SURFACE_TYPE_MEMORY) / hardwareSurface(CS_SURFACE_TYPE_HARDWARE)*/
	CSGraphPixelFormat  	format;			/**< 像素格式*/
	CSGraphPalette      	*palette;		/**< 该Surface对应的调色板*/
	INT 		   	width;			/**< 该Surface的宽度,横向像素数*/
	INT 		    	height;			/**< 该Surface的高度,纵向像素数*/
    	void                	*surface_mem;      	/**< 分配给surface 内存起始地址 */
    	INT                 	surface_mem_size;	/**< 该Surface的内存大小,以字节为单位*/
} CSGraphSurface;

/**@brief Region -可显示的区域*/
typedef struct _CSRegion {
	INT				id;			/**< The region's identifier */
	CSGraphPixelFormat              format;			/**< 像素格式*/
	CSGraphPalette                  *palette;		/**< 该Region对应的调色板*/
	USHORT				pitch;			/**< 存储一行像素所需要的字节数=每行的像素数*每个像素的字节数*/
	INT 				x;			/**< Region的x坐标*/
	INT 				y;			/**< Region的y坐标*/
	INT 				width;			/**< Region的宽度*/
	INT 				height;			/**< Region的高度*/
	CSGraphSurface	    		*activated_surface;	/**< Region关联的Surface*/
	INT		             	max_nb_surface;		/**< Region最多可以关联的Surface个数*/
} CSGraphRegion;

/**
@brief 打开显示设备，进行图象库的初始化工作。
@return 成功返回CSOSG_SUCCESS,失败返回CSOSG_FAILURE
@note 这是调用图象库函数之前所应调用的第一个函数。
*/
INT CSGraphDisplayDeviceOpen(void);

/**
@brief 关闭显示设备，释放相应资源。

@note 此函数用来关闭图象库。此函数调用后，其它的任何图象库函数调用均不再有效。
*/
void CSGraphDisplayDeviceClose(void);

/**
@brief 获取图形驱动设备的指针

@return 成功时返回获取的图形驱动设备的指针,失败返回空
@note 获取的指针在显示设备没有被关闭时全局有效。
*/
CSGraphDisplayDevice*  CSGraphGetDisplayDeviceInfo(void);


/**
@brief 创建一个显示区域。

@param[in] x 区域左上角X坐标
@param[in] y 区域左上角Y坐标
@param[in] width 区域宽度（以pixel为单位）
@param[in] height 区域高度（以pixel为单位）
@param[in] format 区域使用的像素格式和颜色深度等
@param[out] error_code 错误代码
@return 成功时返回一指向CSGraphRegion结构的指针,失败返回NULL
*/
CSGraphRegion*  CSGraphCreateRegion(INT x, INT y,
			INT width, INT height,
			CSGraphPixelFormat *format,
			INT *error_code);

/**
@brief 删除指定的显示区域（region）

@param[in] region 申请删除的区域的指针
@return 成功时返回0,失败时返回非0
@note 释放所有显示区域有关的内存。如果此区域正在被显示，则显示区域被置换成黑色区域（Black），其所对应的所有绘图平面均应自动被释放。 
*/
INT CSGraphDeleteRegion(CSGraphRegion *region);

/**
@brief 创建一个显示绘图平面

@param[in] region 绘图平面所对应的显示区域
@param[out] error_code 错误代码
@return 返回一指向CSGraphSurface结构的指针；否则返回NULL。
@note 图形特征与其对应的显示区域(region)相同。图形驱动最少应支持一个绘图平面。
@note 结构中的id的赋值规则由实现层决定，它应保证每个不同的Surface的id是不一样的。
*/

CSGraphSurface*  CSGraphCreateDisplaySurface(CSGraphRegion *region,int *error_code);

/**
@brief 创建一个内存绘图平面 

@param[in] region 绘图平面所对应的显示区域
@param[in] width 绘图平面宽度（以pixel为单位
@param[in] height 绘图平面高度（以pixel为单位）
@param[in] format 绘图平面使用的像素格式和颜色深度等，如果该参数为NULL，则创建与region绘图属性相同的内存平面
@param[in] palette 绘图平面使用的颜色表（只在format参数不为NULL且绘图平面为调色板模式时有效）
@param[out] error_code 错误代码
@return 返回一指向CSGraphSurface结构的指针；否则返回NULL。
@note 系统应至少支持创建与region具有项同绘图属性的内存平面，
@note 如果不能支持创建与region绘图属性不同的内存平面，则可以返回NULL，并指明错误代码。
*/
CSGraphSurface*  CSGraphCreateMemSurface(CSGraphRegion *region,
				INT width, INT height,
				CSGraphPixelFormat *format,
				CSGraphPalette 		*palette,
				INT *error_code);
 
 /**
@brief 删除指定的绘图平面（Surface），释放所有与此平面有关的内存

@param[in] Surface 申请删除的平面的指针
@return 返回一指向CSGraphSurface结构的指针；否则返回NULL。
@note 如果释放的是其显示区域，则其所对应的所有绘图平面均应自动被释放（包括内存平面和显示平面）。
*/
INT CSGraphDeleteSurface (CSGraphSurface *Surface);

/**
@brief 隐藏指定的区域。 

@param[in] region 申请隐藏的区域的指针
@return 成功返回0,失败返回非0
@note 如果该区域正在被显示，则其显示的内容消失。
*/
INT CSGraphHideRegion(CSGraphRegion *region);

/**
@brief 隐藏指定的区域。 

@param[in] region 申请隐藏的区域的指针
@return 成功返回0,失败返回非0
@note 如果该区域正在被显示，则其显示的内容消失。
*/
INT CSGraphShowRegion(CSGraphRegion *region);

/**
@brief 移动指定的区域至新坐标（x，y）

@param[in] region 申请移动的区域的指针
@param[in] x 新的坐标X
@param[in] y 新的坐标Y
@return 成功返回0,失败返回非0
@note 如果图形驱动硬件或软件实现支持，坐标（x，y）可以在显示区域之外。如果该区域正在被显示则其显示的内容也随region同时移动。
*/
INT CSGraphMoveRegion(CSGraphRegion *region, INT x, INT y);

/**
@brief 设置或更换区域的调色板Palette。 

@param[in] region region
@param[in] palette 需要更换调色板的区域指针
@param[in] firstcolor 调色板的起始位置
@param[in] ncolors 要设置的颜色总数
@return 成功返回0,失败返回非0
@note 该函数应Copy一份Palette。调用之后，用户可以将其释放掉。
@note 只有当区域为调色板模式，调色板参数时才有效。
*/
INT CSGraphSetPalette(CSGraphRegion *region, CSGraphPalette *palette,INT firstcolor,INT ncolors);

/**
@brief 获取区域的调色板Palette。 

@param[in] region 需要获取调色板的区域指针
@param[out] error_code 错误代码
@return 当调用成功时，返回指向区域Palette的指针（CSGraphPalette）；当调用失败时，返回NULL。
@note 该函数返回一CSGraphPalette指针，用户不可以将其释放掉。当错误代码不为0时，返回的CSGraphPalette指针应为NULL。
*/
CSGraphPalette* CSGraphGetPalette(CSGraphRegion *region,INT *error_code);

/**
@brief 用来设置区域的Alpha blending模式。 

@param[in] region 需要获取调色板的区域指针
@param[in] alpha_mode 需要设置的Alpha模式，它为：CSALPHA_MODE_GLOBAL 或CSALPHA_MODE_LOCAL
@return 当调用成功时，返回0;失败时返回非0
*/
INT CSGraphSetAlphaMode(CSGraphRegion *region,UCHAR alpha_mode);

/**
@brief 设置区域的透明度。

@param[in] region 指向需设置透明度的区域的指针
@param[in] transparence 该区域的透明度，以%表示：0表示完全不透明，100表示完全透明
@return 当调用成功时，返回0;失败时返回非0
*/
INT CSGraphSetTransparence(CSGraphRegion *region, UCHAR transparence);

/**
@brief 激活或取消区域的抗闪烁（anti-flickering）功能。

@param[in] region 指向需设置透明度的区域的指针
@param[in] antiflickering 指示是否激活抗闪烁（anti-flickering）功能
- 当antiflickering为1时，激活抗闪烁（anti-flickering）功能
- 当antiflickering为0时，取消抗闪烁（anti-flickering）功能
@return 当调用成功时，返回0;失败时返回非0
@note 如果平台不支持抗闪烁（anti-flickering）功能或调用失败，则返回非0值
@note 如果函数支持抗闪烁（anti-flickering）功能并且调用成功，则返回0值。
*/
INT CSGraphSetAntiFlickering(CSGraphRegion *region, INT antiflickering);

/**
@brief 设置显示区域的活动绘图平面。

@param[in] region 指向需设置活动绘图平面的显示区域的指针
@param[in] surface 活动绘图平面，该平面的类型必须是显示绘图平面，内存平面不能通过此函数激活显示
@param[in] src_x 需要显示的矩形区域的横坐标
@param[in] src_y 需要显示的矩形区域的纵坐标
@param[in] width 需要显示的矩形区域的宽
@param[in] height 需要显示的矩形区域的高
@return 当调用成功时，返回0;失败时返回非0
@note 如果该平面已绘置图形，则指定区域(src_x, src_y, width, height)的绘图立即显示，其余部分的绘图不应该显示。
*/
int CSGraphSetActivatedSurface(CSGraphRegion *region,
				CSGraphSurface *surface,
				INT src_x, 
				INT src_y,
				INT width,
				INT height);

/**
@brief 于(x,y)处绘制象素点。

@param[in] Surface 指向需绘图的绘图平面的指针
@param[in] x 绘制点的横坐标
@param[in] y 绘制点的纵坐标
@param[in] attribute 绘图所用的属性
@return 当调用成功时，返回0;失败时返回非0
@note 象素点的颜色和大小由attribute中的foreground_color和pen_width指定。
*/
int CSGraphDrawPixel(CSGraphSurface *Surface,int x,int y,CSGraphGC *attribute);

/**
@brief 读取(x, y)处的象素点颜色值。

@param[in] Surface 指向需读取象素点的区域的指针
@param[in] x 需读取象素点的横坐标
@param[in] y 需读取象素点的纵坐标
@param[in] c 所读取的颜色值
@return 当调用成功时，返回0;失败时返回非0
@note 颜色值按ARGB排列，占位关系为:Alpha(24－31) Red(16－23) green(8－15) blue(0－7)
@note 每个分量占位从分别从各自的低位开始，比如对于1555显示模式的读取结果，alpha分量占据第24位，RED分量占据16－20位，GREEN分量占据8－12位，BLUE占据0－4位
*/
int CSGraphReadPixel(CSGraphSurface *Surface,int x,int y,UINT *c);

/**
@brief 从(x1, y1)处到(x2, y2)处绘制一直线。

@param[in] Surface 指向需绘图的绘图平面的指针
@param[in] x1 起始点的横坐标
@param[in] y1 起始点的纵坐标
@param[in] x2 终结点纵坐标
@param[in] y2 终结点横坐标
@param[in] attribute 绘图所用的属性
@return 当调用成功时，返回0;失败时返回非0
@note 直线颜色为attribute->foreground_color。
*/
INT CSGraphDrawLine(CSGraphSurface * Surface, INT x1, INT y1, INT x2, INT y2, CSGraphGC * attribute);

/**
@brief 绘制一矩形区域(x1,y1)到(x2,y2)

@param[in] Surface 指向需绘图的绘图平面的指针
@param[in] x1 起始点的横坐标
@param[in] y1 起始点的纵坐标
@param[in] x2 终结点纵坐标
@param[in] y2 终结点横坐标
@param[in] attribute 绘图所用的属性
@return 当调用成功时，返回0;失败时返回非0
@note 矩形边框所用颜色为attribute->foreground_color。
*/
INT CSGraphDrawRect(CSGraphSurface * Surface, INT x1, INT y1, INT x2, INT y2, CSGraphGC * attribute);

/**
@brief 绘制一矩形区域(x1,y1)到(x2,y2)

并充填为attribute->background_color，矩形边框所用颜色为attribute->foreground_color。
@param[in] Surface 指向需绘图的绘图平面的指针
@param[in] x1 起始点的横坐标
@param[in] y1 起始点的纵坐标
@param[in] x2 终结点纵坐标
@param[in] y2 终结点横坐标
@param[in] attribute 绘图所用的属性
@return 当调用成功时，返回0;失败时返回非0
*/
INT CSGraphFillRect(CSGraphSurface * Surface, INT x1, INT y1, INT x2, INT y2, CSGraphGC * attribute);

/**
@brief 绘制一圆形区域

圆的圆心位于(x, y)，半径为radius，圆形边框所用颜色为attribute->foreground_color。
@param[in] Surface 指向需绘图的绘图平面的指针
@param[in] x 圆形圆心点的横坐标
@param[in] y 圆形圆心点的纵坐标
@param[in] radius 圆形的半径
@param[in] attribute 绘图所用的属性
@return 当调用成功时，返回0;失败时返回非0
*/
INT CSGraphDrawCircle(CSGraphSurface * Surface, INT x, INT y, INT radius, CSGraphGC * attribute);

/**
@brief 绘制一圆形区域并填色

圆形边框所用颜色为attribute->foreground_color，
圆形区域的充填颜色为attribute->foreground_color。
@param[in] Surface 指向需绘图的绘图平面的指针
@param[in] x 圆形圆心点的横坐标
@param[in] y 圆形圆心点的纵坐标
@param[in] radius 圆形的半径
@param[in] attribute 绘图所用的属性
@return 当调用成功时，返回0;失败时返回非0
*/
INT CSGraphFillCircle(CSGraphSurface * Surface, INT x, INT y, INT radius, CSGraphGC * attribute);

/**
@brief 绘制一椭圆形区域

椭圆的圆心位于(x, y)，椭圆形的短半径为a，长半径为b，
椭圆形边框所用颜色为attribute->foreground_color?
@param[in] Surface 指向需绘图的绘图平面的指针 
@param[in] x 椭圆形圆心点的横坐标
@param[in] y 椭圆形圆心点的纵坐标
@param[in] a 椭圆形的短半径
@param[in] b 椭圆形的长半径
@param[in] alpha 椭圆形长轴与水平线的夹角（以1/64度°表示）
@param[in] attribute 绘图所用的属性
@return 当调用成功时，返回0;失败时返回非0
*/
INT CSGraphDrawEllipse(CSGraphSurface * Surface, INT x, INT y, INT a, INT b, INT alpha, CSGraphGC * attribute);

/**
@brief 绘制一椭圆形区域并填色

椭圆的圆心位于(x, y)，椭圆形的短半径为a，长半径为b，
椭圆形边框所用颜色为attribute->foreground_color，充填所用颜色为attribute->foreground_color。
@param[in] Surface 指向需绘图的绘图平面的指针
@param[in] Surface 指向需绘图的绘图平面的指针 
@param[in] x 椭圆形圆心点的横坐标
@param[in] y 椭圆形圆心点的纵坐标
@param[in] a 椭圆形的短半径
@param[in] b 椭圆形的长半径
@param[in] alpha 椭圆形长轴与水平线的夹角（以1/64度°表示）
@param[in] attribute 绘图所用的属性
@return 当调用成功时，返回0;失败时返回非0
*/
INT CSGraphFillEllipse(CSGraphSurface * Surface, INT x, INT y, INT a, INT b, INT alpha, CSGraphGC * attribute);

/**
@brief绘制一弧形区域

@param[in] Surface 指向需绘图的绘图平面的指针 
@param[in] x 弧形的包围矩形中心点的横坐标
@param[in] y 弧形的包围矩形中心点的纵坐标
@param[in] width 包围弧形所在椭圆（或圆）的矩形的宽
@param[in] height 包围弧形所在椭圆（或圆）的矩形的高
@param[in] alpha 弧形的起始角（以1/64度为单位）
@param[in] beta 弧形的终止角（以1/64度为单位）
@param[in] attribute 绘图所用的属性
@return 当调用成功时，返回0;失败时返回非0
*/
INT CSGraphDrawArc(CSGraphSurface * Surface, INT x, INT y, INT width, INT height, INT alpha, INT beta, CSGraphGC * attribute);

/**
@brief绘制一弧形区域

@param[in] Surface 指向需绘图的绘图平面的指针 
@param[in] x 弧形的包围矩形中心点的横坐标
@param[in] y 弧形的包围矩形中心点的纵坐标
@param[in] width 包围弧形所在椭圆（或圆）的矩形的宽
@param[in] height 包围弧形所在椭圆（或圆）的矩形的高
@param[in] alpha 弧形的起始角（以1/64度为单位）
@param[in] beta 弧形的终止角（以1/64度为单位）
@param[in] attribute 绘图所用的属性
@return 当调用成功时，返回0;失败时返回非0
@note 弧形的包围矩形的中心位于(x, y)，弧形的包围矩形的宽为width，弧形的包围矩形的高为height，弧形边框所用颜色为
attribute->foreground_color，充填颜色为attribute->foreground_color，充填特性由attribute中相应参数指定。绘制的弧应为正椭圆的一部分（
即椭圆长短轴与x,y坐标平行）
*/
INT CSGraphFillArc(CSGraphSurface * Surface, INT x, INT y, INT width, INT height, INT alpha, INT beta, CSGraphGC * attribute);

/**
@brief绘制一多边形区域

@param[in] Surface 指向需绘图的绘图平面的指针
@param[in] points 指向要绘制坐标点集的指针
@param[in] number_points 多边形点的个数
@param[in] attribute 绘图所用的属性
@return 当调用成功时，返回0;失败时返回非0
@note 多边形共有number_points个点，每个点的X坐标在数组x中，每个点的Y坐标在数组y中。多边形不一定是封闭的；
多边形边框所用颜色为attribute->foreground_color。
*/
INT CSGraphDrawPoly(CSGraphSurface * Surface, INT number_points, CSGraphPoint * points, CSGraphGC * attribute);

/**
@brief 绘制一多边形区域并填色

@param[in] Surface 指向需绘图的绘图平面的指针 
@param[in] number_points 多边形点的个数（数组x，y中的元素数目）(图形同显示区域无交集)
@param[in] points 指向要绘制坐标点集的指针
@param[in] attribute 绘图所用的属性
@return 当调用成功时，返回0;失败时返回非0
@note 每个点的X坐标在数组x中，每个点的Y坐标在数组y中。多边形一定是封闭的：即最后一点坐标与第一点坐标一致。
多边形边框所用颜色为attribute->foreground_color，充填颜色为attribute->foreground_color，充填特性由attribute中相应参数指定
*/

INT CSGraphFillPoly(CSGraphSurface * Surface, INT number_points, CSGraphPoint * points, CSGraphGC * attribute);

/**
@brief 在区域Surface上的(x,y)处绘制图象image，绘图的大小为w*h

传给绘图函数的坐标值（x，y）都是相对于本区域而言的
@param[in] Surface 指向需绘图的绘图平面的指针
@param[in] x 起始点和终结点横坐标
@param[in] y 起始点纵坐标
@param[in] xoff 图象image绘制内容的偏移量（横坐标方向）
@param[in] yoff 图象image绘制内容的偏移量（纵坐标方向）
@param[in] w 绘制图像的宽度（如果w小于image的实际宽度，只绘制w宽度的内容，否则，按照图像的实际宽度绘制）
@param[in] h 绘制图像的高度（如果h小于image的实际高度，只绘制h高度的内容，否则，按照图像的实际高度绘制）
@param[in] attribute 拷贝的目标绘图平面（Surface）
@param[in] image 被拷贝矩形区域的新的横坐标
@return 当调用成功时，返回0;失败时返回非0
@note 传给绘图函数的坐标值（x，y）都是相对于本区域而言的 
*/
INT CSGraphDrawImage(CSGraphSurface * Surface, INT x, INT y, INT xoff, INT yoff, INT w, INT h,
                     CSGraphPixmap * image, CSGraphGC * attribute);

/**
@brief 将绘图平面src_surface的矩形区域（src_x, src_y, w, h）拷贝到绘图平面dest_surface的位置（dest_x, dest_y）上（不考虑缩放）。

@param[in] src_surface 被拷贝矩形区域所在的绘图平面（Surface）
@param[in] src_x 被拷贝矩形区域的横坐标
@param[in] src_y 被拷贝矩形区域的纵坐标
@param[in] w 被拷贝矩形区域的宽
@param[in] h 被拷贝矩形区域的高
@param[in] dest_surface 拷贝的目标绘图平面（Surface）
@param[in] dest_x 被拷贝矩形区域的新的横坐标
@param[in] dest_y 被拷贝矩形区域的新的纵坐标
@param[in] attribute attribute
@return 当调用成功时，返回0;失败时返回非0
@note 拷贝所用的规则由attribute->pixel_mode参数指明 (pixel_mode参数的定义见：2.5.10节)。这两个绘图平面应该具有相同象素格式(Pixel Format)。
拷贝时应考虑到CSGraphGC中制定的绘画模式。如果源和目的平面的绘图模式不一样，则应该自动进行转换，如果系统不支持不同模式绘图平面间的拷贝，
则返回失败。 
*/
INT CSGraphCopyArea(CSGraphSurface * src_surface, INT src_x, INT src_y, INT w, INT h,
                    CSGraphSurface * dest_surface, INT dest_x, INT dest_y, CSGraphGC * attribute);
                    

/**
@brief 将绘图平面src_surface的矩形区域（xsrc, ysrc, wsrc, hsrc）拷贝到绘图平面
 dest_surface的位置（xdst, ydst, wdst, hdst）上（考虑缩放）。

将源surface指定区域的内容，缩放拷贝到目标surface上。
拷贝的方式由attribute的pixels_mode指定，
可支持CS_MODE_COPY 或 CS_MODE_ALPHA 等模式。
同时读取attribute->blend_factor属性，以支持全局透明度。
@param[in] src_surface 源surface指针
@param[in] xsrc 源surface区域的x坐标
@param[in] ysrc 源surface区域的y坐标
@param[in] wsrc 源surface区域的宽度
@param[in] hsrc 源surface区域的高度
@param[in] dest_surface 目标surface指针
@param[in] xdst 目标surface区域的x坐标
@param[in] ydst 目标surface区域的y坐标
@param[in] wdst 目标surface区域的宽度
@param[in] hdst 目标surface区域的高度
@param[in] attribute  GC相关参数，主要用到pixels_mode及blend_factor属性
@param[out] 无
@return 成功返回0,失败返回非0值
@note 无
*/
INT CSGraphCopyAreaScaled(CSGraphSurface * src_surface, 
	INT xsrc, INT ysrc, INT wsrc, INT hsrc, 
	CSGraphSurface * dest_surface, 
	INT xdst, INT ydst, INT wdst, INT hdst, 
	CSGraphGC * attribute);

/**
@brief 获取绘图平面的数据

@param[in] nIndex  0 表示标清 1 表示高清
@param[out] psDisplaySurface  显示平面
@return 当调用成功时，返回0;失败时返回非0
@note  获取绘图平面的数据	
*/

INT CSGraphGetDisplayDeviceSurface(int nIndex, CSGraphSurface *psDisplaySurface);

/**
@brief 往显存上写数据

@param[in] surface	平面上的数据
@param[in] x  x坐标
@param[in] y  y坐标
@param[in] width  宽度
@param[in] height  高度
@return 当调用成功时，返回0;失败时返回非0
@note  往显存上写数据	
*/
INT CSGraphSetDisplayDeviceSurface(CSGraphSurface *surface,INT x, INT y, INT width, INT height);


/**
@brief 将绘图平面surface的矩形区域（x, y, w, h）生成图象，拷贝到一片内存中

@param[in] surface 被拷贝矩形区域所在的绘图平面（Surface） 
@param[in] x 被拷贝矩形区域的横坐标
@param[in] y 被拷贝矩形区域的纵坐标
@param[in] w 被拷贝矩形区域的宽
@param[in] h 被拷贝矩形区域的高
@param[in] pixmap 存放拷贝图象信息的pixmap结构指针。
@return 当调用成功时，返回0;失败时返回非0
@note 拷贝到的内存空间由上层分配，
imagebits的大小为pixmap->width*pixmap->height* pixmap-> pixel_format. BytesPerPixel,pixmap结构中的
width，height和pixel_format.BytesPerPixel的内容都应由应用填充，其它的由此函数填充，pixmap参数为输入输出参数。
该函数调用结束后，应用可以自由使用pixmap及其imagebits的内容，并可以根据需要，在任何时候释放imagebits指向的空间（
甚至pixmap结构占用的空间--如果pixmap结构也是动态分配的一块内存空间）  
*/
INT CSGraphSnapShot(CSGraphSurface * surface, INT x, INT y, INT w, INT h, CSGraphPixmap * pixmap);

	/**
	@brief  设置OSG层的显示类型
	 
	@param[in]
			int nType 设置osg的类型
			
			类型取值可以为:
			GRA_DISPLAYOPTION_COLOR  为OSG显示变灰
			GRA_DISPLAYOPTION_3D 为OSG支持3DTV
			
	@param[in]
			int nValue 设置的对应类型nType的值
			
			当nType为GRA_DISPLAYOPTION_COLOR时，
				nValue 值为1,表示设置OSG变灰
				nValue 值为0,表示设置OSG恢复正常
				
			当nType为GRA_DISPLAYOPTION_3D时，
				nValue 值为0,表示OSG正常显示
				nValue 值为1,表示将OSG一分为二左右显示
				nValue 值为2,表示将OSG一分为二上下显示	

	@return 当调用成功时，返回0;失败时返回非0
	 		
	@exception
	@note
	*/
	INT CSGraphSetDisplayOption(INT nType, INT nValue);

#ifdef __cplusplus
}
#endif

/** @} */

#endif

