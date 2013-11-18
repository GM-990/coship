
/**@defgroup OSG OSG 定义平台绘图接口,本套接口不允许多线程调用

@note 整个模块的坐标系统为:X轴从左至右延伸，Y轴从上至下延伸

@version 2.0.6 2009/9/07 增加colorkeyflag 和alphaflag
@version 2.0.5 2009/8/28 把矩形区域结构体移入public公共头文件中
@version 2.0.4 2009/8/27 增加图片硬件
@version 2.0.3 2009/8/26 修改colorKey类型为无符号数
@version 2.0.2 2009/8/24 第一次综合评审后修改的版本
@version 2.0.1 2009/07/08 经初步讨论过的版本
@{
*/
#ifndef MWSCREEN_H
#define MWSCREEN_H

#include "mmcp_typedef.h"
#include "mmcp_osg.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MMCP_UDI2

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

	/**@brief 全局的ALPHA模式*/
#define CS_ALPHA_MODE_GLOBAL	1
	/**@brief 局部ALPHA模式*/
#define CS_ALPHA_MODE_LOCAL	2

	/**@brief 图像表示层类型--硬件显示*/    
#define CS_SURFACE_TYPE_HARDWARE  0		//used
	/**@brief 图像表示层类型--仅存放在内存*/
#define CS_SURFACE_TYPE_MEMORY	1		//used

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

	/*arc_style：该变量定义了弧形的绘制和填充模式。定义如下：     */
	/**@brief 弧形的绘制和填充模式--弦扇性形封闭区域，缺省时的模式 */
#define CS_ARC_STYLE_CENTERCHORD_ARC_MODE 	0	
	/**@brief 弧形的绘制和填充模式--弧Pie形封闭区域 */
#define CS_ARC_STYLE_LINEPIESLICE_ARC_MODE 	1	

/*clip flag*/
#define CS_CLIPPING_NO_RECT_NO_MASK	0	/**< 无剪切无屏蔽，pixel_mode在整个区域有效 */
#define CS_CLIPPING_VALID_RECT_NO_MASK	1	/**< 有剪切无屏蔽,pixel_mode仅在剪切矩形区域内有效(包括边界) */
#define CS_CLIPPING_NO_RECT_VALID_MASK	2	/**< 无剪切有屏蔽,pixel_mode仅在剪切屏蔽区域,且该屏蔽设置为1时有效，包括屏蔽区域边界 */
#define CS_CLIPPING_VALID_RECT_AND_MASK	3	/**< 有剪切有屏蔽,pixel_mode仅在剪切区域和没有交集，则不执行任何操作。如果有一块交集，pixel_mode将应用于屏蔽设置为1的那一部分 */


#if 1
	/**@brief 定义点的结构体*/
	typedef struct _CSPoint {
		int x;	/**< 点的x坐标*/
		int y;	/**< 点的y坐标*/
	}CSGraphPoint;

	/**@brief 采用RGB分量表示的颜色结构体*/
	typedef struct _CSRGBColor {
		unsigned char a;		/**< 透明因子*/
		unsigned char r;		/**< 红色分量*/
		unsigned char g;		/**< 绿色分量*/
		unsigned char b;		/**< 蓝色分量*/
	} CSGraphRGBColor;

	/**@brief 采用YUV分量表示的颜色结构体*/
	typedef struct _CSYUVColor {
		unsigned char a;		/**< alpha值*/
		unsigned char y;		/**< 亮度分量*/
		unsigned char u;		/**< 蓝色色度分量*/
		unsigned char v;		/**< 红色色度分量*/
	} CSGraphYUVColor;

	/**@brief 调色板结构体*/
	typedef  struct _CSPalette{
		unsigned char	type;				/**< 颜色模式CSGraphPalette_TYPE_RGB或者CSGraphPalette_TYPE_YUV*/
		unsigned short	ncolors;				/**< 颜色数组长度*/
		union {
			CSGraphRGBColor	*rgb;	/**< 颜色数组相当于rgb[ncolors]对应的type为CSGraphPalette_TYPE_RGB*/
			CSGraphYUVColor	*yuv;	/**< 颜色数组相当于yuv[ncolors]对应的type为CSGraphPalette_TYPE_YUV*/
		} col;
	} CSGraphPalette;


	/**@brief 位图格式结构体*/
	typedef struct _CSPixelFormat {
		unsigned char  BitsPerPixel;			/**< 每个像素的位数*/
		unsigned char  BytesPerPixel;		/**< 每个像素的字节数*/
		unsigned char  Rloss;				/**< red loss*/
		unsigned char  Gloss;				/**< gree loss*/
		unsigned char  Bloss;				/**< blue loss*/
		unsigned char  Aloss;				/**< alpha loss*/
		unsigned char  Rshift;				/**< red shift*/
		unsigned char  Gshift;				/**< green shift*/
		unsigned char  Bshift;				/**< blue shift*/
		unsigned char  Ashift;				/**< alpha shift*/
		unsigned int   Rmask;				/**< red mask*/
		unsigned int   Gmask;				/**< green mask*/
		unsigned int   Bmask;				/**< blue mask*/
		unsigned int   Amask;				/**< alpha mask*/
		unsigned int   colorkey;				/**< RGB color key information */
		unsigned char  transparence;		/**< 透明度Alpha value information (per-surface alpha) */
		unsigned int   alpha_blending_mode;	/**< alpha混合模式CS_ALPHA_MODE_GLOBAL or CS_ALPHA_MODE_LOCAL*/
	} CSGraphPixelFormat;

	/**@brief 位图结构体*/
	typedef struct _CSPixmap{
		unsigned short				width;		/**< image width in pixels*/
		unsigned short				height;		/**< image height in pixels*/
		unsigned char				planes;		/**< image planes*/
		unsigned short				pitch;		/**< bytes per line*/
		unsigned char				compression;		/**< compression algorithm*/
		CSGraphPixelFormat		pixel_format;	/**< bits per pixel(1,4 or 8,16,24,32)*/
		CSGraphPalette			*palette;		/**< 调色板地址*/
		unsigned char  		        	*imagebits;			/**< image bits (int right aligned)*/
		unsigned char				colorkeyflag;   		/**< indicate whether to use colorkey in pixel_format */
		unsigned char				alphaflag;      		/**< indicate whether to use alpha mode in pixel_format */
		unsigned char  		        	*YUVbits;			/**< image yuv bits (int right aligned)*/
		unsigned short				YUVwidth;		/**< imagev yuv  width in pixels*/
		unsigned short				YUVheight;		/**< image yuv height in pixels*/
	}CSGraphPixmap;



	typedef struct	_CSGC{
		int		font_id;		/**< 字库指针*/
		int		id;			/**< Identifier */
		unsigned int		background_color;	/**< 背景颜色					*/
		unsigned int		foreground_color;	/**< 前景颜色					*/
		unsigned int		pen_width;		/**< 画笔宽度					*/
		unsigned int		pen_style;		/**< 画笔类型					*/
		unsigned char	       *dash_format;		/**< 虚线格式					*/
		unsigned int		dash_length;		/**< 虚线长度					*/
		unsigned char	        cap_style;		/**< 末端类型					*/
		unsigned char	        join_style;		/**< 连接类型					*/
		unsigned char	        poly_fill_style;	/**< 多边形填充类型				*/
		unsigned char	        arc_style;		/**< 弧形模式					*/
		unsigned char		blending_factor;	/**< 混合因子（透明度）			*/
		char		pixel_mode;		/**< 新的象素写入内存时对旧的象素的关系*/
		char		clip_flag;		/**< 剪切标志					*/
		short	        clip_area_x;		/**< 剪切矩形x坐标				*/
		short	        clip_area_y;		/**< 剪切矩形y坐标				*/
		short	        clip_area_width;	/**< 剪切矩形宽度				*/
		short	        clip_area_height;	/**< 剪切矩形高度				*/
		short	        clip_mask_x;		/**< 剪切屏蔽矩形x坐标			*/
		short	        clip_mask_y;		/**< 剪切屏蔽矩形y坐标			*/
		short	        clip_mask_width;	/**< 剪切屏蔽矩形宽度			*/
		short	        clip_mask_height;	/**< 剪切屏蔽矩形高度			*/
		CSGraphPixmap	*pixmap_mask;	        /**< 剪切屏蔽象素图指针			*/
		unsigned int		plane_mask;		/**< 被绘图影响到的每个象素位掩码	*/
	} CSGraphGC;


	/**@brief 显示设备*/
	typedef struct _CSDisplayDevice {
		char		*name;			        /**< 设备名称*/
		int			width;			        /**< 显示区域宽度,即屏幕分辨率,如果支持硬件缩放则该值仅供参考*/
		int			height;			        /**< 显示区域宽度,即屏幕分辨率,如果支持硬件缩放则该值仅供参考*/
		int			region_overlapable;	    /**< Region是否支持重叠:0-不支持,1-支持;即*/
		int			anti_aliasing;		    /**< 是否支持折叠,0-不支持,1-支持*/
		CSGraphPixelFormat	*display_mode;	/**< 显示模式*/
		int			display_mem_size;	    /**< 显存大小，以字节为单位*/
		void		*display_mem;		    /**< 显存地址*/
	}CSGraphDisplayDevice;


	/**@brief Surface类似于画布的意思*/
	typedef struct _CSSurface {
		int	                id;			/**< The Surface's identifier */
		struct _CSRegion   	*associated_region;	/**< 该Surface所关联的region*/
		char                	type;  			/**< The type of the surface - memSurface(CS_SURFACE_TYPE_MEMORY) / hardwareSurface(CS_SURFACE_TYPE_HARDWARE)*/
		CSGraphPixelFormat  	format;			/**< 像素格式*/
		CSGraphPalette      	*palette;		/**< 该Surface对应的调色板*/
		int 		   	width;			/**< 该Surface的宽度,横向像素数*/
		int 		    	height;			/**< 该Surface的高度,纵向像素数*/
		void                	*surface_mem;      	/**< 分配给surface 内存起始地址 */
		int                 	surface_mem_size;	/**< 该Surface的内存大小,以字节为单位*/
	} CSGraphSurface;

	/**@brief Region -可显示的区域*/
	typedef struct _CSRegion {
		int				id;			/**< The region's identifier */
		CSGraphPixelFormat              format;			/**< 像素格式*/
		CSGraphPalette                  *palette;		/**< 该Region对应的调色板*/
		unsigned short				pitch;			/**< 存储一行像素所需要的字节数=每行的像素数*每个像素的字节数*/
		int 				x;			/**< Region的x坐标*/
		int 				y;			/**< Region的y坐标*/
		int 				width;			/**< Region的宽度*/
		int 				height;			/**< Region的高度*/
		CSGraphSurface	    		*activated_surface;	/**< Region关联的Surface*/
		int		             	max_nb_surface;		/**< Region最多可以关联的Surface个数*/
	} CSGraphRegion;


#endif 

	typedef struct _GraphLine{
		int minx;
		int maxx;
		/*int y;*/
	} GraphLine;



	typedef struct {
		int     x1, y1, x2, y2;
		int     cx, fn, mn, d;
	} edge_t;

	/**
	@brief 打开显示设备，进行图象库的初始化工作。
	@return 成功返回CSOSG_SUCCESS,失败返回CSOSG_FAILURE
	@note 这是调用图象库函数之前所应调用的第一个函数。
	*/
	int MWScreenDisplayDeveiceOpen(void);

	/**
	@brief 关闭显示设备，释放相应资源。

	@note 此函数用来关闭图象库。此函数调用后，其它的任何图象库函数调用均不再有效。
	*/
	void MWScreenDisplayDeviceClose(void);

	/**
	@brief 获取图形驱动设备的指针

	@return 成功时返回获取的图形驱动设备的指针,失败返回空
	@note 获取的指针在显示设备没有被关闭时全局有效。
	*/
	CSGraphDisplayDevice* MWScreenGetDisplayDeviceInfo(void);


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
	CSGraphRegion*  MWScreenCreateRegion(int x, int y,
		int width, int height,
		CSGraphPixelFormat *format,
		int *error_code);

	/**
	@brief 删除指定的显示区域（region）

	@param[in] region 申请删除的区域的指针
	@return 成功时返回0,失败时返回非0
	@note 释放所有显示区域有关的内存。如果此区域正在被显示，则显示区域被置换成黑色区域（Black），其所对应的所有绘图平面均应自动被释放。 
	*/
	int MWScreenDeleteRegion(CSGraphRegion *region);

	/**
	@brief 创建一个显示绘图平面

	@param[in] region 绘图平面所对应的显示区域
	@param[out] error_code 错误代码
	@return 返回一指向CSGraphSurface结构的指针；否则返回NULL。
	@note 图形特征与其对应的显示区域(region)相同。图形驱动最少应支持一个绘图平面。
	@note 结构中的id的赋值规则由实现层决定，它应保证每个不同的Surface的id是不一样的。
	*/

	CSGraphSurface*  MWScreenCreateDisplaySurface(CSGraphRegion *region,int *error_code);

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
	CSGraphSurface*  MWScreenCreateMemSurface(CSGraphRegion *region,
		int width, int height,
		CSGraphPixelFormat *format,
		CSGraphPalette 		*palette,
		int *error_code);

	/**
	@brief 删除指定的绘图平面（Surface），释放所有与此平面有关的内存

	@param[in] Surface 申请删除的平面的指针
	@return 返回一指向CSGraphSurface结构的指针；否则返回NULL。
	@note 如果释放的是其显示区域，则其所对应的所有绘图平面均应自动被释放（包括内存平面和显示平面）。
	*/
	int MWScreenDeleteSurface (CSGraphSurface *Surface);

	/**
	@brief 隐藏指定的区域。 

	@param[in] region 申请隐藏的区域的指针
	@return 成功返回0,失败返回非0
	@note 如果该区域正在被显示，则其显示的内容消失。
	*/
	int MWScreenHideRegion(CSGraphRegion *region);

	/**
	@brief 隐藏指定的区域。 

	@param[in] region 申请隐藏的区域的指针
	@return 成功返回0,失败返回非0
	@note 如果该区域正在被显示，则其显示的内容消失。
	*/
	int MWScreenShowRegion(CSGraphRegion *region);

	/**
	@brief 移动指定的区域至新坐标（x，y）

	@param[in] region 申请移动的区域的指针
	@param[in] x 新的坐标X
	@param[in] y 新的坐标Y
	@return 成功返回0,失败返回非0
	@note 如果图形驱动硬件或软件实现支持，坐标（x，y）可以在显示区域之外。如果该区域正在被显示则其显示的内容也随region同时移动。
	*/
	int MWScreenMoveRegion(CSGraphRegion *region, int x, int y);

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
	int MWScreenSetPalette(CSGraphRegion *region, CSGraphPalette *palette,int firstcolor,int ncolors);

	/**
	@brief 获取区域的调色板Palette。 

	@param[in] region 需要获取调色板的区域指针
	@param[out] error_code 错误代码
	@return 当调用成功时，返回指向区域Palette的指针（CSGraphPalette）；当调用失败时，返回NULL。
	@note 该函数返回一CSGraphPalette指针，用户不可以将其释放掉。当错误代码不为0时，返回的CSGraphPalette指针应为NULL。
	*/
	CSGraphPalette* MWScreenGetPalette(CSGraphRegion *region,int *error_code);

	/**
	@brief 用来设置区域的Alpha blending模式。 

	@param[in] region 需要获取调色板的区域指针
	@param[in] alpha_mode 需要设置的Alpha模式，它为：CSALPHA_MODE_GLOBAL 或CSALPHA_MODE_LOCAL
	@return 当调用成功时，返回0;失败时返回非0
	*/
	int MWScreenSetAlphaMode(CSGraphRegion *region,unsigned char alpha_mode);

	/**
	@brief 设置区域的透明度。

	@param[in] region 指向需设置透明度的区域的指针
	@param[in] transparence 该区域的透明度，以%表示：0表示完全不透明，100表示完全透明
	@return 当调用成功时，返回0;失败时返回非0
	*/
	int MWScreenSetTransparence(CSGraphRegion *region, unsigned char transparence);

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
	int MWScreenSetAntiFlickering(CSGraphRegion *region, int antiflickering);

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
	int MWScreenSetActivatedSurface(CSGraphRegion *region,
		CSGraphSurface *surface,
		int src_x, 
		int src_y,
		int width,
		int height);

	/**
	@brief 于(x,y)处绘制象素点。

	@param[in] Surface 指向需绘图的绘图平面的指针
	@param[in] x 绘制点的横坐标
	@param[in] y 绘制点的纵坐标
	@param[in] attribute 绘图所用的属性
	@return 当调用成功时，返回0;失败时返回非0
	@note 象素点的颜色和大小由attribute中的foreground_color和pen_width指定。
	*/
	int MWScreenDrawPixel(CSGraphSurface *Surface,int x,int y,CSGraphGC *attribute);

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
	int MWScreenReadPixel(CSGraphSurface *Surface,int x,int y,unsigned int *c);

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
	int MWScreenDrawLine(CSGraphSurface * Surface, int x1, int y1, int x2, int y2, CSGraphGC * attribute);

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
	int MWScreenDrawRect(CSGraphSurface * Surface, int x1, int y1, int x2, int y2, CSGraphGC * attribute);

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
	int MWScreenFillRect(CSGraphSurface * Surface, int x1, int y1, int x2, int y2, CSGraphGC * attribute);

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
	int MWScreenDrawCircle(CSGraphSurface * Surface, int x, int y, int radius, CSGraphGC * attribute);

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
	int MWScreenFillCircle(CSGraphSurface * Surface, int x, int y, int radius, CSGraphGC * attribute);

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
	int MWScreenDrawEllipse(CSGraphSurface * Surface, int x, int y, int a, int b, int alpha, CSGraphGC * attribute);

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
	int MWScreenFillEllipse(CSGraphSurface * Surface, int x, int y, int a, int b, int alpha, CSGraphGC * attribute);

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
	int MWScreenDrawArc(CSGraphSurface * Surface, int x, int y, int width, int height, int alpha, int beta, CSGraphGC * attribute);

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
	int MWScreenFillArc(CSGraphSurface * Surface, int x, int y, int width, int height, int alpha, int beta, CSGraphGC * attribute);

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
	int MWScreenDrawPoly(CSGraphSurface * Surface, int number_points, CSGraphPoint * points, CSGraphGC * attribute);

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

	int MWScreenFillPoly(CSGraphSurface * Surface, int number_points, CSGraphPoint * points, CSGraphGC * attribute);

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
	int MWScreenDrawImage(CSGraphSurface * Surface, int x, int y, int xoff, int yoff, int w, int h,
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
	int MWScreenCopyArea(CSGraphSurface * src_surface, int src_x, int src_y, int w, int h,
		CSGraphSurface * dest_surface, int dest_x, int dest_y, CSGraphGC * attribute);


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
	int MWScreenCopyAreaScaled(CSGraphSurface * src_surface, 
		int xsrc, int ysrc, int wsrc, int hsrc, 
		CSGraphSurface * dest_surface, 
		int xdst, int ydst, int wdst, int hdst, 
		CSGraphGC * attribute);

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
	INT MWScreenSnapShot(CSGraphSurface * surface, INT x, INT y, INT w, INT h, CSGraphPixmap * pixmap);

	/**
	@brief 获取绘图平面的数据

	@param[in] nIndex  0 表示标清 1 表示高清
	@param[out] psDisplaySurface  显示平面
	@return 当调用成功时，返回0;失败时返回非0
	@note  获取绘图平面的数据	
	*/

	INT MWScreenGetDisplayDeviceSurface(int nIndex, CSGraphSurface *psDisplaySurface);

	/**
	@brief 往显存上写数据

	@param[in] surface  平面上的数据
	@param[in] x  x坐标
	@param[in] y  y坐标
	@param[in] width  宽度
	@param[in] height  高度
	@return 当调用成功时，返回0;失败时返回非0
	@note  往显存上写数据	
	*/
	INT MWScreenSetDisplayDeviceSurface(CSGraphSurface *surface,INT x, INT y, INT width, INT height);


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
	INT MWScreenSetDisplayOption(INT nType, INT nValue);

#ifdef MMCP_QT
#include "qtadapter.h"
#endif


//适配层的handle
#ifdef MMCP_QT

	typedef struct
	{
		CSQT_HANDLE	 m_hSurface;
		int			 m_nSurfaceCreateType;
	}UDIAdapterSurface_S;

#else

	typedef struct
	{
		CSUDI_HANDLE 		 m_hSurface;
		int					 m_nSurfaceCreateType;
	}UDIAdapterSurface_S;
#endif




/**
@brief 创建一个绘图平面

@param[in] ePixelFormat 绘图平面的颜色模式
@param[in] nWidth  绘图平面的宽度，必须大于 0
@param[in] nHeight 绘图平面的高度，必须大于 0
@param[out] phSurface 成功返回绘图平面的句柄，失败为空，详细错误参见错误代码值
@return 成功返回 CSUDI_SUCCESS,否则返回错误代码值 
@note 该接口创建显示缓存空间，理论上可以支持无限大，支持创建无限多个直到内存不足，(建议提供16M的内存用于创建Surface)
      具体需要几个Surface根据项目需求的不同而不同(目前应用用到比较多的是:1.游戏应用中的可变图片会用到此Surface 2.Java应用中
      用到BufferedImage用到此Surface 3.2D特效的实现会用到此Surface)
@note 此处颜色模式可以与最终显示的颜色模式不同，在Blit时做特殊处理即可，如果平台不支持这样的处理也可在应用选择颜色模式时加以避免
@note 如果平台资源实在有限，也可允许不能创建任何Surface，这种情况下会直接操作显存，但必须要求界面与显存的分辨率，颜色模式等保持一致
@note 首先调用CSUDIOSGCreateSurface 创建surface，如果返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED，则尝试自己从普通内存中创建surface。
*/
CSUDI_Error_Code UDIOSGAdapterCreateSurface(CSGraphPixelFormat *psGraphPixelFormat, int nWidth, int nHeight, UDIAdapterSurface_S **ppsSurface);


/**
@brief 销毁一个绘图平面，释放该绘图平面的内存

@param[in] hSurface CSUDIOSGCreateSurface 成功返回的句柄
@return 成功返回 CSUDI_SUCCESS，否则返回错误代码值 
@note 由CSUDIOSGCreateSurface创建的缓存区域不使用后，一定要调用此接口销毁释放
@note 尝试销毁一个显存空间，将返回CSUDIOSG_ERROR_BAD_PARAMETER
@note 首先调用CSUDIOSGDestroySurface 销毁surface，如果返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED，则自己释放surface。
*/
CSUDI_Error_Code UDIOSGAdapterDestroySurface(UDIAdapterSurface_S  *psSurface);


/**
@brief 得到显存surface句柄

@param[in] nIndex 显存索引，从0开始直到没有更多的显存，返回失败为止
@param[out] phSurface 成功返回显存Surface句柄

@return 成功返回 CSUDI_SUCCESS,否则返回错误代码

@note 这种方式得到的Surface不能使用CSUDIOSGDestroySurface销毁
@note 如果高标清同时支持，nIndex 为 0 表示标清显存；nIndex 为 1 表示高清显存
@note 只需要返回所有用于完整OSD显示的显存,如标清显存，高清显存，不要返回特殊层显存如光标层，背景层等。
@note 除非不能创建任何其他Surface，否则中间件一般不会直接操作显存地址进行绘制,该句柄多数情况下只会用来往上进行Blit
@note 首先调用CSUDIOSGGetDisplaySurface, UDI层必须支持此接口，否则无法显示图像数据到屏幕上。

*/
CSUDI_Error_Code UDIOSGAdapterGetDisplaySurface(int nIndex, UDIAdapterSurface_S *psSurface);


/**
@brief 获取绘图平面或者显存空间的信息

中间件会通过该接口获取的信息进行绘图操作
@param[in] hSurface 绘图平面或者显存空间句柄.CSUDIOSGCreateSurface或者CSUDIOSGGetDisplaySurface的返回值
@param[out] psSurfaceInfo 保存得到的绘图平面或者显存空间信息
@return 成功返回 CSUDI_SUCCESS，否则返回错误代码值 
@note 此接口能够得到显存或者缓存的空间地址
@note 首先调用CSUDIOSGGetSurfaceInfo，如果返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED，则需要在适配层实现此函数。
*/
CSUDI_Error_Code UDIOSGAdapterGetSurfaceInfo(UDIAdapterSurface_S *psSurface, CSUDIOSGSurfaceInfo_S * psSurfaceInfo);

/**
@brief 设置绘图平面的信息，只能设置绘图平面的m_nAlphaFlag、m_uAlpha和m_nColorKeyFlag、m_uColorkey四个成员，
       对其他成员操作无效

@param[in] hSurface 绘图平面句柄.CSUDIOSGCreateSurface返回值
@param[in] psSurfaceInfo 保存得到的绘图平面或者显存空间信息
@return 成功返回 CSUDI_SUCCESS，否则返回错误代码值 
@note 此接口在CSUDIOSGGetSurfaceInfo后使用，只能设置绘图平面的信息，不能设置显存空间的信息
@note 此接口在仅仅用于改变Surface的m_uAlpha和m_uColorkey属性，并不会对显示产生变化
@note 当psSurfaceInfo中同时有除m_nAlphaFlag、m_uAlpha和m_nColorKeyFlag、m_uColorkey以外的成员改变时，
      其他成员修改操作无效，仅仅对m_nAlphaFlag、m_uAlpha和m_nColorKeyFlag、m_uColorkey生效，返回CSUDI_SUCCESS
@note 首先调用CSUDIOSGSetSurfaceInfo，如果返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED，则需要在适配层实现此函数。
*/
CSUDI_Error_Code UDIOSGAdapterSetSurfaceInfo(UDIAdapterSurface_S *psSurface, const CSUDIOSGSurfaceInfo_S * psSurfaceInfo);


/**
@brief Blit操作接口，以利用硬件加速性能，异步操作

@param[in] hDstSurface 目标绘图平面或者显存空间句柄.CSUDIOSGCreateSurface或者CSUDIOSGGetDisplaySurface的返回值
@param[in] psDstRect 目标surface或显示空间hDstSurface上矩形区域,如果为空，则取hDstSurface的全部区域
@param[in] hSrcSurface 源绘图平面或者显存空间句柄
@param[in] psSrcRect 源surface或显示空间hSrcSurface上矩形区域,如果为空，则取hSrcSurface的全部区域
@param[in] eOpMode 混合模式，见CSUDIOSGOpMode_E定义
@param[in] bEnableFilter 是否过滤alpha为0的像素，TRUE 表示过滤，FALSE 表示不过滤。
@return 成功返回 CSUDI_SUCCESS，其它返回错误代码。
@note 使用hSrcSurface中的alpha值，如果要设置，使用CSUDIOSGSetSurfaceInfo;
@note 如果不支持硬件加速请返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED，即除非有更高效的实现方式或者其它充分的理由,不要对该操作进行软件模拟.
@note 源和目标的区域大小psSrcRect、psDstRect可以为空，但是非空时，必须是对应surface上大小的范围内，不支持裁剪则返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED
@note 源和目标均可能为显存Surface或者普通Surface,若不支持请返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED
@note 源和目标可能为同一个Surface,若不支持请返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED
@note 源和目标可能为同一个Surface时，psDstRect和psSrcRect不允许重叠，否则返回CSUDIOSG_ERROR_BAD_PARAMETER
@note 源和目标的颜色模式可能不一致,一般不推荐这种使用，若不支持可返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED
@note 源和目标的可能分别是YUV和RGB的颜色模式,若不支持可返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED
@note 源和目标矩形的大小可能不一致,若不支持可返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED,但一般来说高清平台必须支持,软件的缩放效率极低,中间件不会提供
@note 源和目标的矩形可能有缩放、拉升等处理,若不一致请返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED,需支持2D特效的平台必须实现该功能
@note 为充分提高绘图性能，该接口可实现成异步模式，通过CSUDIOSGSync确保该操作执行完毕,
@note 在CSUDIOSGSync返回前改变相关Surface,绘图效果是不确定的,但不能死机或产生其他不可恢复的异常
@note 如果平台同时支持高标清显示，要求在向标清显存blit时，一定要设置防闪(或者防锯齿)操作
@note 平台支持的拉伸和缩放比例至少为: 1:15
@note 首先调用CSUDIOSGBlit，如果返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED，则需要在适配层实现此函数。
*/
CSUDI_Error_Code UDIOSGAdapterBlit(UDIAdapterSurface_S *psDstSurface, const CSUDIOSGRect_S * psDstRect,
                       UDIAdapterSurface_S *psSrcSurface, const CSUDIOSGRect_S * psSrcRect, CSUDIOSGOpMode_E eOpMode, CSGraphPixelFormat *psSrcPixelFormat, CSGraphPixelFormat *psDstPixelFormat, BOOL bEnableFilter);


/**
@brief 填充目标绘图平面或者显存空间上一块矩形

@param[in] hDstSurface 绘图平面或者显存空间句柄.CSUDIOSGCreateSurface或者CSUDIOSGGetDisplaySurface的返回值
@param[in] psDstRect 要填充在目标绘图平面上的位置和大小，为空时，表示全屏填充
@param[in] uPixel 要填充的颜色值，ARGB8888格式的像素值
@return 成功返回 CSUDI_SUCCESS，否则返回错误代码值 
@note 为充分提高绘图性能，该接口需要实现成异步模式，通过CSUDIOSGSync确保该操作执行完毕,
在CSUDIOSGSync返回前改变相关Surface,绘图效果是不确定的,但不能死机或产生其他不可恢复的异常
@note 必须使用硬件加速Fill,如果硬件不支持，请返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED,即除非有更高效的实现方式或者其它充分的理由,不要对该操作进行软件模拟.
@note psDstRect 必须在hDstSurface的大小范围内，如果超过hDstSurface，则返回CSUDIOSG_ERROR_BAD_PARAMETER
@note 首先调用CSUDIOSGFill，如果返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED，则需要在适配层实现此函数。
*/
CSUDI_Error_Code UDIOSGAdapterFill(UDIAdapterSurface_S *psDstSurface, const CSUDIOSGRect_S * psDstRect, unsigned int unPixel,CSGraphPixelFormat *psPixelFormat);

/**
@brief 同步OSG操作

@return 成功返回 CSUDI_SUCCESS，否则返回错误代码值 
@note 在CSUDIOSGBlit，CSUDIOSGFill等之后调用,函数返回时，前述操作全部完成
@note 该接口与Blit的关系可参考C99标准fflush
@note 首先调用CSUDIOSGSync，如果返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED，则表示全部使用同步方式。

*/
CSUDI_Error_Code UDIOSGAdapterSync(void);

/**
@brief 硬件解码图片,包括I帧格式(MPEG,BMP)/JPEG格式/PNG格式等

@param[in] pszImageType 图片的类型,使用字符串避免了定义多的枚举类型。格式与类型的定义: IFRAME - I帧格式，BMP - bmp格式, JPEG - jpeg格式, PNG - png格式
@param[in] pvImageMem  图片数据地址
@param[in] nMemSize   图片数据大小
@param[out] phSurface 解码后的数据存放的surface
@return 成功返回 CSUDI_SUCCESS,否则返回错误代码

@note 允许不支持硬件解码，此时返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED
@note 使用本接口得到的绘图平面在使用后，必须调用CSUDIOSGDestroySurface释放
@note 如果本接口解码图片(例如I帧)用到的设备处于忙状态，请返回CSUDIOSG_ERROR_DEVICE_BUSY
@note 解码I帧一般需要用到Video解码器，会与视频播放冲突，本接口应尽量使用闲置Video解码器进行解码，若实在找不到闲置解码器则返回CSUDIOSG_ERROR_DEVICE_BUSY，
此情况下EastWin如必须要进行解码，则应通知上层暂停视频播放后再次调用该接口
@note 本接口仅适用于可以通过硬件方式加速解码的情况，除非有非常充分的理由，否则请不要进行任何软件模拟实现。
*/
CSUDI_Error_Code UDIOSGAdapterDecodeImage(const char * pszImageType, const void * pvImageMem, int nMemSize, UDIAdapterSurface_S **ppsSurface);

/**
@brief 获取UDIAdapterSurface_S 结构体的m_hSurface句柄
@param[in] int psSurface   -----对应udi1中CSGraphSurface 的id，udi2中UDIAdapterSurface_S * psSurface
@param[out] CSUDI_HANDLE * hSurface。
@return 返回错误类型。
@note 应用场景:1. 首先new一个pixmap 2.传入pixmap的surface id给SurfaceId,然后得到SurfaceId对应的m_hSurface句柄。3 只返回硬件创建的surface对应的句柄。
*/
CSUDI_Error_Code UDIOSGAdapterGetUDISurfaceHandle(int SurfaceId,CSUDI_HANDLE *hSurface);

#else
#define MWScreenDisplayDeveiceOpen    CSGraphDisplayDeviceOpen
#define MWScreenDisplayDeviceClose    CSGraphDisplayDeviceClose
#define MWScreenGetDisplayDeviceInfo   CSGraphGetDisplayDeviceInfo
#define MWScreenCreateRegion    CSGraphCreateRegion
#define MWScreenDeleteRegion    CSGraphDeleteRegion
#define MWScreenCreateDisplaySurface    CSGraphCreateDisplaySurface
#define MWScreenCreateMemSurface    CSGraphCreateMemSurface
#define MWScreenDeleteSurface    CSGraphDeleteSurface
#define MWScreenHideRegion    CSGraphHideRegion
#define MWScreenShowRegion  CSGraphShowRegion
#define MWScreenMoveRegion    CSGraphMoveRegion
#define MWScreenSetPalette    CSGraphSetPalette
#define MWScreenGetPalette    CSGraphGetPalette
#define MWScreenSetAlphaMode    CSGraphSetAlphaMode
#define MWScreenSetTransparence   CSGraphSetTransparence
#define MWScreenSetAntiFlickering    CSGraphSetAntiFlickering
#define MWScreenSetActivatedSurface    CSGraphSetActivatedSurface
#define MWScreenDrawPixel    CSGraphDrawPixel
#define MWScreenReadPixel    CSGraphReadPixel
#define MWScreenDrawLine    CSGraphDrawLine
#define MWScreenDrawRect    CSGraphDrawRect
#define MWScreenFillRect    CSGraphFillRect
#define MWScreenDrawCircle    CSGraphDrawCircle
#define MWScreenFillCircle    CSGraphFillCircle
#define MWScreenDrawEllipse    CSGraphDrawEllipse
#define MWScreenFillEllipse    CSGraphFillEllipse
#define MWScreenDrawArc    CSGraphDrawArc
#define MWScreenFillArc    CSGraphFillArc
#define MWScreenDrawPoly    CSGraphDrawPoly
#define MWScreenFillPoly    CSGraphFillPoly
#define MWScreenDrawImage    CSGraphDrawImage
#define MWScreenCopyArea    CSGraphCopyArea
#define MWScreenCopyAreaScaled   CSGraphCopyAreaScaled
#define MWScreenSnapShot    CSGraphSnapShot
#define MWScreenGetDisplayDeviceSurface CSGraphGetDisplayDeviceSurface
#define MWScreenSetDisplayDeviceSurface CSGraphSetDisplayDeviceSurface
#define MWScreenSetDisplayOption CSGraphSetDisplayOption
#endif

#ifdef __cplusplus
}
#endif

#endif  /* MWSCREEN_H */
