/**@defgroup CSOSG osg ͳһ��ͼ����ؽӿڣ��续�����Դ��ַ������������ͼ������
@brief �ṩͳһ��ͼ����ؽӿڣ��续�����Դ��ַ������������ͼ������

@note ����ϵͳ:X������������죬Y�������������
@version 0.1 2008/10/17 ��ʾ�汾
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

/**@brief ���ظ�ֵ��ʾ�ɹ�*/
#define CSOSG_SUCCESS		(0)
/**@brief ���ظ�ֵ��ʾʧ��*/
#define CSOSG_FAILURE		(1)

/**@brief ��ʾ��ɫ������ΪRGB�ĺ�*/
#define CSGraphPalette_TYPE_RGB 0	
/**@brief ��ʾ��ɫ������ΪYUV�ĺ�*/
#define CSGraphPalette_TYPE_YUV 1	

/**@brief ȫ�ֵ�ALPHAģʽ*/
#define CS_ALPHA_MODE_GLOBAL	1
/**@brief �ֲ�ALPHAģʽ*/
#define CS_ALPHA_MODE_LOCAL	2

/**@brief ͼ���ʾ������--Ӳ����ʾ*/    
#define CS_SURFACE_TYPE_HARDWARE  0		//used
/**@brief ͼ���ʾ������--��������ڴ�*/
#define CS_SURFACE_TYPE_MEMORY	1		//used

/**@brief ��������--�����ߣ�ʵ�ߣ���ȱʡʱ��ģʽ*/
#define CS_CONTINUES_LINE_SOLID	 		0	
/**@brief ��������--��ǰ��ɫ��͸��ɫ���������*/
#define CS_TRANSPARENCY_DASHED_LINE_ON_OFF_DASH 1
/**@brief ��������--��ǰ��ɫ�ͱ���ɫ���������, �����ߵľ����ʽ��dash_format���塣*/
#define CS_BACKCOLOR_DASHED_LINE_DOUBLE_DASH 	2	

/**@brief ĩ�˷��--����ĩ�ˣ�ȱʡʱ��ģʽ */
#define CS_CAP_STYLE_RECTBUTT_LINE_ENDED	0	
/**@brief ĩ�˷��--Բ��ĩ��  */
#define CS_CAP_STYLE_ROUND_LINE_ENDED		1	
/**@brief ĩ�˷��-- ����ͶӰ��ĩ��*/
#define CS_CAP_STYLE_PROJECTING_LINE_ENDED 	2	

/**@brief ���Ӵ����-- ��������ģʽ��ȱʡʱ��ģʽ */
#define CS_ANGLE_JOIN_MODESTYLE_MITER	0	
/**@brief ���Ӵ����-- Բ������ģʽ */
#define CS_ROUND_JOIN_STYLEMODE_ROUND	1	
/**@brief ���Ӵ����-- ƽ��б������ģʽ*/
#define CS_FLAT_JOIN_STYLEMODE_BEVEL	2	

/*poly_fill_style���ñ������ھ�������ε���䷽�����������£�*/
/**@brief ������������--���Ƴ�ģʽ,ȱʡʱ��ģʽ*/
#define CS_POLY_FILL_STYLE_WINDING	0	
/**@brief ������������--��ż���ģʽ*/
#define CS_POLY_FILL_STYLE_EVENODD	1	

/*arc_style���ñ��������˻��εĻ��ƺ����ģʽ���������£�     */
/**@brief ���εĻ��ƺ����ģʽ--�������η������ȱʡʱ��ģʽ */
#define CS_ARC_STYLE_CENTERCHORD_ARC_MODE 	0	
/**@brief ���εĻ��ƺ����ģʽ--��Pie�η������ */
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
#define CS_CLIPPING_NO_RECT_NO_MASK	0	/**< �޼��������Σ�pixel_mode������������Ч */
#define CS_CLIPPING_VALID_RECT_NO_MASK	1	/**< �м���������,pixel_mode���ڼ��о�����������Ч(�����߽�) */
#define CS_CLIPPING_NO_RECT_VALID_MASK	2	/**< �޼���������,pixel_mode���ڼ�����������,�Ҹ���������Ϊ1ʱ��Ч��������������߽� */
#define CS_CLIPPING_VALID_RECT_AND_MASK	3	/**< �м���������,pixel_mode���ڼ��������û�н�������ִ���κβ����������һ�齻����pixel_mode��Ӧ������������Ϊ1����һ���� */

/*usual error code*/
#define CS_GRAPH_NO_ERROR	0	/**< �ɹ�*/
#define CS_SURFACE_INVALID	-1	/**< ָ����ָ��ͼƽ����Ч */
#define CS_REGION_INVALID	-2	/**< ��ָ���Ĺ��ܲ�֧�� */
#define CS_PARAMETER_INVALID	-3	/**< ������Ч */
#define CS_ATTRIBUTE_INVALID	-4	/**< �����õĻ�ͼ������Ч */
#define CS_NO_ENOUGH_MEMORY	-5	/**< �����ڴ�ʧ�� */
#define CS_SIZE_INVALID		-6	/**< �����С��Ч��width��height����ϵͳ֧�ֵ����귶Χ�� */
#define CS_COLOR_MODE_INVALID	-7	/**< ��ɫģʽ(bpp)��֧�ֻ������Ч */
#define CS_ALPHA_MODE_INVALID	-8	/**< Alpha blendingģʽ��֧�ֻ������Ч */
#define CS_SYSTEM_NOT_INITED	-9	/**< ͼ��ϵͳδ��ʼ�� */

/**@brief �����Ľṹ��*/
typedef struct _CSPoint {
	INT x;	/**< ���x����*/
	INT y;	/**< ���y����*/
}CSGraphPoint;

/**@brief ����RGB������ʾ����ɫ�ṹ��*/
typedef struct _CSRGBColor {
	UCHAR a;		/**< ͸������*/
	UCHAR r;		/**< ��ɫ����*/
	UCHAR g;		/**< ��ɫ����*/
	UCHAR b;		/**< ��ɫ����*/
} CSGraphRGBColor;

/**@brief ����YUV������ʾ����ɫ�ṹ��*/
typedef struct _CSYUVColor {
	UCHAR a;		/**< alphaֵ*/
	UCHAR y;		/**< ���ȷ���*/
	UCHAR u;		/**< ��ɫɫ�ȷ���*/
	UCHAR v;		/**< ��ɫɫ�ȷ���*/
} CSGraphYUVColor;

/**@brief ��ɫ��ṹ��*/
typedef  struct _CSPalette{
	UCHAR	type;				/**< ��ɫģʽCSGraphPalette_TYPE_RGB����CSGraphPalette_TYPE_YUV*/
	USHORT	ncolors;				/**< ��ɫ���鳤��*/
	union {
		CSGraphRGBColor	*rgb;	/**< ��ɫ�����൱��rgb[ncolors]��Ӧ��typeΪCSGraphPalette_TYPE_RGB*/
		CSGraphYUVColor	*yuv;	/**< ��ɫ�����൱��yuv[ncolors]��Ӧ��typeΪCSGraphPalette_TYPE_YUV*/
	} col;
} CSGraphPalette;


/**@brief λͼ��ʽ�ṹ��*/
typedef struct _CSPixelFormat {
	UCHAR  BitsPerPixel;			/**< ÿ�����ص�λ��*/
	UCHAR  BytesPerPixel;		/**< ÿ�����ص��ֽ���*/
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
	UCHAR  transparence;		/**< ͸����Alpha value information (per-surface alpha) */
	UINT   alpha_blending_mode;	/**< alpha���ģʽCS_ALPHA_MODE_GLOBAL or CS_ALPHA_MODE_LOCAL*/
} CSGraphPixelFormat;

/**@brief λͼ�ṹ��*/
typedef struct _CSPixmap{
	USHORT				width;		/**< image width in pixels*/
	USHORT				height;		/**< image height in pixels*/
	UCHAR				planes;		/**< image planes*/
	USHORT				pitch;		/**< bytes per line*/
	UCHAR				compression;		/**< compression algorithm*/
	CSGraphPixelFormat		pixel_format;	/**< bits per pixel(1,4 or 8,16,24,32)*/
	CSGraphPalette			*palette;		/**< ��ɫ���ַ*/
	UCHAR  		        	*imagebits;			/**< image bits (INT right aligned)*/
	UCHAR				colorkeyflag;   		/**< indicate whether to use colorkey in pixel_format */
	UCHAR				alphaflag;      		/**< indicate whether to use alpha mode in pixel_format */
	UCHAR  		        	*YUVbits;			/**< image yuv bits (INT right aligned)*/
	USHORT				YUVwidth;		/**< imagev yuv  width in pixels*/
	USHORT				YUVheight;		/**< image yuv height in pixels*/
}CSGraphPixmap;



typedef struct	_CSGC{
	INT		font_id;		/**< �ֿ�ָ��*/
	INT		id;			/**< Identifier */
	UINT		background_color;	/**< ������ɫ					*/
	UINT		foreground_color;	/**< ǰ����ɫ					*/
	UINT		pen_width;		/**< ���ʿ��					*/
	UINT		pen_style;		/**< ��������					*/
	UCHAR	       *dash_format;		/**< ���߸�ʽ					*/
	UINT		dash_length;		/**< ���߳���					*/
	UCHAR	        cap_style;		/**< ĩ������					*/
	UCHAR	        join_style;		/**< ��������					*/
	UCHAR	        poly_fill_style;	/**< ������������				*/
	UCHAR	        arc_style;		/**< ����ģʽ					*/
	UCHAR		blending_factor;	/**< ������ӣ�͸���ȣ�			*/
	CHAR		pixel_mode;		/**< �µ�����д���ڴ�ʱ�Ծɵ����صĹ�ϵ*/
	CHAR		clip_flag;		/**< ���б�־					*/
	SHORT	        clip_area_x;		/**< ���о���x����				*/
	SHORT	        clip_area_y;		/**< ���о���y����				*/
	SHORT	        clip_area_width;	/**< ���о��ο��				*/
	SHORT	        clip_area_height;	/**< ���о��θ߶�				*/
	SHORT	        clip_mask_x;		/**< �������ξ���x����			*/
	SHORT	        clip_mask_y;		/**< �������ξ���y����			*/
	SHORT	        clip_mask_width;	/**< �������ξ��ο��			*/
	SHORT	        clip_mask_height;	/**< �������ξ��θ߶�			*/
	CSGraphPixmap	*pixmap_mask;	        /**< ������������ͼָ��			*/
	UINT		plane_mask;		/**< ����ͼӰ�쵽��ÿ������λ����	*/
} CSGraphGC;


/**@brief ��ʾ�豸*/
typedef struct _CSDisplayDevice {
	CHAR		*name;			        /**< �豸����*/
	INT			width;			        /**< ��ʾ������,����Ļ�ֱ���,���֧��Ӳ���������ֵ�����ο�*/
	INT			height;			        /**< ��ʾ������,����Ļ�ֱ���,���֧��Ӳ���������ֵ�����ο�*/
	INT			region_overlapable;	    /**< Region�Ƿ�֧���ص�:0-��֧��,1-֧��;��*/
	INT			anti_aliasing;		    /**< �Ƿ�֧���۵�,0-��֧��,1-֧��*/
	CSGraphPixelFormat	*display_mode;	/**< ��ʾģʽ*/
	INT			display_mem_size;	    /**< �Դ��С�����ֽ�Ϊ��λ*/
	void		*display_mem;		    /**< �Դ��ַ*/
}CSGraphDisplayDevice;


/**@brief Surface�����ڻ�������˼*/
typedef struct _CSSurface {
	INT	                id;			/**< The Surface's identifier */
	struct _CSRegion   	*associated_region;	/**< ��Surface��������region*/
	CHAR                	type;  			/**< The type of the surface - memSurface(CS_SURFACE_TYPE_MEMORY) / hardwareSurface(CS_SURFACE_TYPE_HARDWARE)*/
	CSGraphPixelFormat  	format;			/**< ���ظ�ʽ*/
	CSGraphPalette      	*palette;		/**< ��Surface��Ӧ�ĵ�ɫ��*/
	INT 		   	width;			/**< ��Surface�Ŀ��,����������*/
	INT 		    	height;			/**< ��Surface�ĸ߶�,����������*/
    	void                	*surface_mem;      	/**< �����surface �ڴ���ʼ��ַ */
    	INT                 	surface_mem_size;	/**< ��Surface���ڴ��С,���ֽ�Ϊ��λ*/
} CSGraphSurface;

/**@brief Region -����ʾ������*/
typedef struct _CSRegion {
	INT				id;			/**< The region's identifier */
	CSGraphPixelFormat              format;			/**< ���ظ�ʽ*/
	CSGraphPalette                  *palette;		/**< ��Region��Ӧ�ĵ�ɫ��*/
	USHORT				pitch;			/**< �洢һ����������Ҫ���ֽ���=ÿ�е�������*ÿ�����ص��ֽ���*/
	INT 				x;			/**< Region��x����*/
	INT 				y;			/**< Region��y����*/
	INT 				width;			/**< Region�Ŀ��*/
	INT 				height;			/**< Region�ĸ߶�*/
	CSGraphSurface	    		*activated_surface;	/**< Region������Surface*/
	INT		             	max_nb_surface;		/**< Region�����Թ�����Surface����*/
} CSGraphRegion;

/**
@brief ����ʾ�豸������ͼ���ĳ�ʼ��������
@return �ɹ�����CSOSG_SUCCESS,ʧ�ܷ���CSOSG_FAILURE
@note ���ǵ���ͼ��⺯��֮ǰ��Ӧ���õĵ�һ��������
*/
INT CSGraphDisplayDeviceOpen(void);

/**
@brief �ر���ʾ�豸���ͷ���Ӧ��Դ��

@note �˺��������ر�ͼ��⡣�˺������ú��������κ�ͼ��⺯�����þ�������Ч��
*/
void CSGraphDisplayDeviceClose(void);

/**
@brief ��ȡͼ�������豸��ָ��

@return �ɹ�ʱ���ػ�ȡ��ͼ�������豸��ָ��,ʧ�ܷ��ؿ�
@note ��ȡ��ָ������ʾ�豸û�б��ر�ʱȫ����Ч��
*/
CSGraphDisplayDevice*  CSGraphGetDisplayDeviceInfo(void);


/**
@brief ����һ����ʾ����

@param[in] x �������Ͻ�X����
@param[in] y �������Ͻ�Y����
@param[in] width �����ȣ���pixelΪ��λ��
@param[in] height ����߶ȣ���pixelΪ��λ��
@param[in] format ����ʹ�õ����ظ�ʽ����ɫ��ȵ�
@param[out] error_code �������
@return �ɹ�ʱ����һָ��CSGraphRegion�ṹ��ָ��,ʧ�ܷ���NULL
*/
CSGraphRegion*  CSGraphCreateRegion(INT x, INT y,
			INT width, INT height,
			CSGraphPixelFormat *format,
			INT *error_code);

/**
@brief ɾ��ָ������ʾ����region��

@param[in] region ����ɾ���������ָ��
@return �ɹ�ʱ����0,ʧ��ʱ���ط�0
@note �ͷ�������ʾ�����йص��ڴ档������������ڱ���ʾ������ʾ�����û��ɺ�ɫ����Black����������Ӧ�����л�ͼƽ���Ӧ�Զ����ͷš� 
*/
INT CSGraphDeleteRegion(CSGraphRegion *region);

/**
@brief ����һ����ʾ��ͼƽ��

@param[in] region ��ͼƽ������Ӧ����ʾ����
@param[out] error_code �������
@return ����һָ��CSGraphSurface�ṹ��ָ�룻���򷵻�NULL��
@note ͼ�����������Ӧ����ʾ����(region)��ͬ��ͼ����������Ӧ֧��һ����ͼƽ�档
@note �ṹ�е�id�ĸ�ֵ������ʵ�ֲ��������Ӧ��֤ÿ����ͬ��Surface��id�ǲ�һ���ġ�
*/

CSGraphSurface*  CSGraphCreateDisplaySurface(CSGraphRegion *region,int *error_code);

/**
@brief ����һ���ڴ��ͼƽ�� 

@param[in] region ��ͼƽ������Ӧ����ʾ����
@param[in] width ��ͼƽ���ȣ���pixelΪ��λ
@param[in] height ��ͼƽ��߶ȣ���pixelΪ��λ��
@param[in] format ��ͼƽ��ʹ�õ����ظ�ʽ����ɫ��ȵȣ�����ò���ΪNULL���򴴽���region��ͼ������ͬ���ڴ�ƽ��
@param[in] palette ��ͼƽ��ʹ�õ���ɫ��ֻ��format������ΪNULL�һ�ͼƽ��Ϊ��ɫ��ģʽʱ��Ч��
@param[out] error_code �������
@return ����һָ��CSGraphSurface�ṹ��ָ�룻���򷵻�NULL��
@note ϵͳӦ����֧�ִ�����region������ͬ��ͼ���Ե��ڴ�ƽ�棬
@note �������֧�ִ�����region��ͼ���Բ�ͬ���ڴ�ƽ�棬����Է���NULL����ָ��������롣
*/
CSGraphSurface*  CSGraphCreateMemSurface(CSGraphRegion *region,
				INT width, INT height,
				CSGraphPixelFormat *format,
				CSGraphPalette 		*palette,
				INT *error_code);
 
 /**
@brief ɾ��ָ���Ļ�ͼƽ�棨Surface�����ͷ��������ƽ���йص��ڴ�

@param[in] Surface ����ɾ����ƽ���ָ��
@return ����һָ��CSGraphSurface�ṹ��ָ�룻���򷵻�NULL��
@note ����ͷŵ�������ʾ������������Ӧ�����л�ͼƽ���Ӧ�Զ����ͷţ������ڴ�ƽ�����ʾƽ�棩��
*/
INT CSGraphDeleteSurface (CSGraphSurface *Surface);

/**
@brief ����ָ�������� 

@param[in] region �������ص������ָ��
@return �ɹ�����0,ʧ�ܷ��ط�0
@note ������������ڱ���ʾ��������ʾ��������ʧ��
*/
INT CSGraphHideRegion(CSGraphRegion *region);

/**
@brief ����ָ�������� 

@param[in] region �������ص������ָ��
@return �ɹ�����0,ʧ�ܷ��ط�0
@note ������������ڱ���ʾ��������ʾ��������ʧ��
*/
INT CSGraphShowRegion(CSGraphRegion *region);

/**
@brief �ƶ�ָ���������������꣨x��y��

@param[in] region �����ƶ��������ָ��
@param[in] x �µ�����X
@param[in] y �µ�����Y
@return �ɹ�����0,ʧ�ܷ��ط�0
@note ���ͼ������Ӳ�������ʵ��֧�֣����꣨x��y����������ʾ����֮�⡣������������ڱ���ʾ������ʾ������Ҳ��regionͬʱ�ƶ���
*/
INT CSGraphMoveRegion(CSGraphRegion *region, INT x, INT y);

/**
@brief ���û��������ĵ�ɫ��Palette�� 

@param[in] region region
@param[in] palette ��Ҫ������ɫ�������ָ��
@param[in] firstcolor ��ɫ�����ʼλ��
@param[in] ncolors Ҫ���õ���ɫ����
@return �ɹ�����0,ʧ�ܷ��ط�0
@note �ú���ӦCopyһ��Palette������֮���û����Խ����ͷŵ���
@note ֻ�е�����Ϊ��ɫ��ģʽ����ɫ�����ʱ����Ч��
*/
INT CSGraphSetPalette(CSGraphRegion *region, CSGraphPalette *palette,INT firstcolor,INT ncolors);

/**
@brief ��ȡ����ĵ�ɫ��Palette�� 

@param[in] region ��Ҫ��ȡ��ɫ�������ָ��
@param[out] error_code �������
@return �����óɹ�ʱ������ָ������Palette��ָ�루CSGraphPalette����������ʧ��ʱ������NULL��
@note �ú�������һCSGraphPaletteָ�룬�û������Խ����ͷŵ�����������벻Ϊ0ʱ�����ص�CSGraphPaletteָ��ӦΪNULL��
*/
CSGraphPalette* CSGraphGetPalette(CSGraphRegion *region,INT *error_code);

/**
@brief �������������Alpha blendingģʽ�� 

@param[in] region ��Ҫ��ȡ��ɫ�������ָ��
@param[in] alpha_mode ��Ҫ���õ�Alphaģʽ����Ϊ��CSALPHA_MODE_GLOBAL ��CSALPHA_MODE_LOCAL
@return �����óɹ�ʱ������0;ʧ��ʱ���ط�0
*/
INT CSGraphSetAlphaMode(CSGraphRegion *region,UCHAR alpha_mode);

/**
@brief ���������͸���ȡ�

@param[in] region ָ��������͸���ȵ������ָ��
@param[in] transparence �������͸���ȣ���%��ʾ��0��ʾ��ȫ��͸����100��ʾ��ȫ͸��
@return �����óɹ�ʱ������0;ʧ��ʱ���ط�0
*/
INT CSGraphSetTransparence(CSGraphRegion *region, UCHAR transparence);

/**
@brief �����ȡ������Ŀ���˸��anti-flickering�����ܡ�

@param[in] region ָ��������͸���ȵ������ָ��
@param[in] antiflickering ָʾ�Ƿ񼤻��˸��anti-flickering������
- ��antiflickeringΪ1ʱ�������˸��anti-flickering������
- ��antiflickeringΪ0ʱ��ȡ������˸��anti-flickering������
@return �����óɹ�ʱ������0;ʧ��ʱ���ط�0
@note ���ƽ̨��֧�ֿ���˸��anti-flickering�����ܻ����ʧ�ܣ��򷵻ط�0ֵ
@note �������֧�ֿ���˸��anti-flickering�����ܲ��ҵ��óɹ����򷵻�0ֵ��
*/
INT CSGraphSetAntiFlickering(CSGraphRegion *region, INT antiflickering);

/**
@brief ������ʾ����Ļ��ͼƽ�档

@param[in] region ָ�������û��ͼƽ�����ʾ�����ָ��
@param[in] surface ���ͼƽ�棬��ƽ������ͱ�������ʾ��ͼƽ�棬�ڴ�ƽ�治��ͨ���˺���������ʾ
@param[in] src_x ��Ҫ��ʾ�ľ�������ĺ�����
@param[in] src_y ��Ҫ��ʾ�ľ��������������
@param[in] width ��Ҫ��ʾ�ľ�������Ŀ�
@param[in] height ��Ҫ��ʾ�ľ�������ĸ�
@return �����óɹ�ʱ������0;ʧ��ʱ���ط�0
@note �����ƽ���ѻ���ͼ�Σ���ָ������(src_x, src_y, width, height)�Ļ�ͼ������ʾ�����ಿ�ֵĻ�ͼ��Ӧ����ʾ��
*/
int CSGraphSetActivatedSurface(CSGraphRegion *region,
				CSGraphSurface *surface,
				INT src_x, 
				INT src_y,
				INT width,
				INT height);

/**
@brief ��(x,y)���������ص㡣

@param[in] Surface ָ�����ͼ�Ļ�ͼƽ���ָ��
@param[in] x ���Ƶ�ĺ�����
@param[in] y ���Ƶ��������
@param[in] attribute ��ͼ���õ�����
@return �����óɹ�ʱ������0;ʧ��ʱ���ط�0
@note ���ص����ɫ�ʹ�С��attribute�е�foreground_color��pen_widthָ����
*/
int CSGraphDrawPixel(CSGraphSurface *Surface,int x,int y,CSGraphGC *attribute);

/**
@brief ��ȡ(x, y)�������ص���ɫֵ��

@param[in] Surface ָ�����ȡ���ص�������ָ��
@param[in] x ���ȡ���ص�ĺ�����
@param[in] y ���ȡ���ص��������
@param[in] c ����ȡ����ɫֵ
@return �����óɹ�ʱ������0;ʧ��ʱ���ط�0
@note ��ɫֵ��ARGB���У�ռλ��ϵΪ:Alpha(24��31) Red(16��23) green(8��15) blue(0��7)
@note ÿ������ռλ�ӷֱ�Ӹ��Եĵ�λ��ʼ���������1555��ʾģʽ�Ķ�ȡ�����alpha����ռ�ݵ�24λ��RED����ռ��16��20λ��GREEN����ռ��8��12λ��BLUEռ��0��4λ
*/
int CSGraphReadPixel(CSGraphSurface *Surface,int x,int y,UINT *c);

/**
@brief ��(x1, y1)����(x2, y2)������һֱ�ߡ�

@param[in] Surface ָ�����ͼ�Ļ�ͼƽ���ָ��
@param[in] x1 ��ʼ��ĺ�����
@param[in] y1 ��ʼ���������
@param[in] x2 �ս��������
@param[in] y2 �ս�������
@param[in] attribute ��ͼ���õ�����
@return �����óɹ�ʱ������0;ʧ��ʱ���ط�0
@note ֱ����ɫΪattribute->foreground_color��
*/
INT CSGraphDrawLine(CSGraphSurface * Surface, INT x1, INT y1, INT x2, INT y2, CSGraphGC * attribute);

/**
@brief ����һ��������(x1,y1)��(x2,y2)

@param[in] Surface ָ�����ͼ�Ļ�ͼƽ���ָ��
@param[in] x1 ��ʼ��ĺ�����
@param[in] y1 ��ʼ���������
@param[in] x2 �ս��������
@param[in] y2 �ս�������
@param[in] attribute ��ͼ���õ�����
@return �����óɹ�ʱ������0;ʧ��ʱ���ط�0
@note ���α߿�������ɫΪattribute->foreground_color��
*/
INT CSGraphDrawRect(CSGraphSurface * Surface, INT x1, INT y1, INT x2, INT y2, CSGraphGC * attribute);

/**
@brief ����һ��������(x1,y1)��(x2,y2)

������Ϊattribute->background_color�����α߿�������ɫΪattribute->foreground_color��
@param[in] Surface ָ�����ͼ�Ļ�ͼƽ���ָ��
@param[in] x1 ��ʼ��ĺ�����
@param[in] y1 ��ʼ���������
@param[in] x2 �ս��������
@param[in] y2 �ս�������
@param[in] attribute ��ͼ���õ�����
@return �����óɹ�ʱ������0;ʧ��ʱ���ط�0
*/
INT CSGraphFillRect(CSGraphSurface * Surface, INT x1, INT y1, INT x2, INT y2, CSGraphGC * attribute);

/**
@brief ����һԲ������

Բ��Բ��λ��(x, y)���뾶Ϊradius��Բ�α߿�������ɫΪattribute->foreground_color��
@param[in] Surface ָ�����ͼ�Ļ�ͼƽ���ָ��
@param[in] x Բ��Բ�ĵ�ĺ�����
@param[in] y Բ��Բ�ĵ��������
@param[in] radius Բ�εİ뾶
@param[in] attribute ��ͼ���õ�����
@return �����óɹ�ʱ������0;ʧ��ʱ���ط�0
*/
INT CSGraphDrawCircle(CSGraphSurface * Surface, INT x, INT y, INT radius, CSGraphGC * attribute);

/**
@brief ����һԲ��������ɫ

Բ�α߿�������ɫΪattribute->foreground_color��
Բ������ĳ�����ɫΪattribute->foreground_color��
@param[in] Surface ָ�����ͼ�Ļ�ͼƽ���ָ��
@param[in] x Բ��Բ�ĵ�ĺ�����
@param[in] y Բ��Բ�ĵ��������
@param[in] radius Բ�εİ뾶
@param[in] attribute ��ͼ���õ�����
@return �����óɹ�ʱ������0;ʧ��ʱ���ط�0
*/
INT CSGraphFillCircle(CSGraphSurface * Surface, INT x, INT y, INT radius, CSGraphGC * attribute);

/**
@brief ����һ��Բ������

��Բ��Բ��λ��(x, y)����Բ�εĶ̰뾶Ϊa�����뾶Ϊb��
��Բ�α߿�������ɫΪattribute->foreground_color?
@param[in] Surface ָ�����ͼ�Ļ�ͼƽ���ָ�� 
@param[in] x ��Բ��Բ�ĵ�ĺ�����
@param[in] y ��Բ��Բ�ĵ��������
@param[in] a ��Բ�εĶ̰뾶
@param[in] b ��Բ�εĳ��뾶
@param[in] alpha ��Բ�γ�����ˮƽ�ߵļнǣ���1/64�ȡ��ʾ��
@param[in] attribute ��ͼ���õ�����
@return �����óɹ�ʱ������0;ʧ��ʱ���ط�0
*/
INT CSGraphDrawEllipse(CSGraphSurface * Surface, INT x, INT y, INT a, INT b, INT alpha, CSGraphGC * attribute);

/**
@brief ����һ��Բ��������ɫ

��Բ��Բ��λ��(x, y)����Բ�εĶ̰뾶Ϊa�����뾶Ϊb��
��Բ�α߿�������ɫΪattribute->foreground_color������������ɫΪattribute->foreground_color��
@param[in] Surface ָ�����ͼ�Ļ�ͼƽ���ָ��
@param[in] Surface ָ�����ͼ�Ļ�ͼƽ���ָ�� 
@param[in] x ��Բ��Բ�ĵ�ĺ�����
@param[in] y ��Բ��Բ�ĵ��������
@param[in] a ��Բ�εĶ̰뾶
@param[in] b ��Բ�εĳ��뾶
@param[in] alpha ��Բ�γ�����ˮƽ�ߵļнǣ���1/64�ȡ��ʾ��
@param[in] attribute ��ͼ���õ�����
@return �����óɹ�ʱ������0;ʧ��ʱ���ط�0
*/
INT CSGraphFillEllipse(CSGraphSurface * Surface, INT x, INT y, INT a, INT b, INT alpha, CSGraphGC * attribute);

/**
@brief����һ��������

@param[in] Surface ָ�����ͼ�Ļ�ͼƽ���ָ�� 
@param[in] x ���εİ�Χ�������ĵ�ĺ�����
@param[in] y ���εİ�Χ�������ĵ��������
@param[in] width ��Χ����������Բ����Բ���ľ��εĿ�
@param[in] height ��Χ����������Բ����Բ���ľ��εĸ�
@param[in] alpha ���ε���ʼ�ǣ���1/64��Ϊ��λ��
@param[in] beta ���ε���ֹ�ǣ���1/64��Ϊ��λ��
@param[in] attribute ��ͼ���õ�����
@return �����óɹ�ʱ������0;ʧ��ʱ���ط�0
*/
INT CSGraphDrawArc(CSGraphSurface * Surface, INT x, INT y, INT width, INT height, INT alpha, INT beta, CSGraphGC * attribute);

/**
@brief����һ��������

@param[in] Surface ָ�����ͼ�Ļ�ͼƽ���ָ�� 
@param[in] x ���εİ�Χ�������ĵ�ĺ�����
@param[in] y ���εİ�Χ�������ĵ��������
@param[in] width ��Χ����������Բ����Բ���ľ��εĿ�
@param[in] height ��Χ����������Բ����Բ���ľ��εĸ�
@param[in] alpha ���ε���ʼ�ǣ���1/64��Ϊ��λ��
@param[in] beta ���ε���ֹ�ǣ���1/64��Ϊ��λ��
@param[in] attribute ��ͼ���õ�����
@return �����óɹ�ʱ������0;ʧ��ʱ���ط�0
@note ���εİ�Χ���ε�����λ��(x, y)�����εİ�Χ���εĿ�Ϊwidth�����εİ�Χ���εĸ�Ϊheight�����α߿�������ɫΪ
attribute->foreground_color��������ɫΪattribute->foreground_color������������attribute����Ӧ����ָ�������ƵĻ�ӦΪ����Բ��һ���֣�
����Բ��������x,y����ƽ�У�
*/
INT CSGraphFillArc(CSGraphSurface * Surface, INT x, INT y, INT width, INT height, INT alpha, INT beta, CSGraphGC * attribute);

/**
@brief����һ���������

@param[in] Surface ָ�����ͼ�Ļ�ͼƽ���ָ��
@param[in] points ָ��Ҫ��������㼯��ָ��
@param[in] number_points ����ε�ĸ���
@param[in] attribute ��ͼ���õ�����
@return �����óɹ�ʱ������0;ʧ��ʱ���ط�0
@note ����ι���number_points���㣬ÿ�����X����������x�У�ÿ�����Y����������y�С�����β�һ���Ƿ�յģ�
����α߿�������ɫΪattribute->foreground_color��
*/
INT CSGraphDrawPoly(CSGraphSurface * Surface, INT number_points, CSGraphPoint * points, CSGraphGC * attribute);

/**
@brief ����һ�����������ɫ

@param[in] Surface ָ�����ͼ�Ļ�ͼƽ���ָ�� 
@param[in] number_points ����ε�ĸ���������x��y�е�Ԫ����Ŀ��(ͼ��ͬ��ʾ�����޽���)
@param[in] points ָ��Ҫ��������㼯��ָ��
@param[in] attribute ��ͼ���õ�����
@return �����óɹ�ʱ������0;ʧ��ʱ���ط�0
@note ÿ�����X����������x�У�ÿ�����Y����������y�С������һ���Ƿ�յģ������һ���������һ������һ�¡�
����α߿�������ɫΪattribute->foreground_color��������ɫΪattribute->foreground_color������������attribute����Ӧ����ָ��
*/

INT CSGraphFillPoly(CSGraphSurface * Surface, INT number_points, CSGraphPoint * points, CSGraphGC * attribute);

/**
@brief ������Surface�ϵ�(x,y)������ͼ��image����ͼ�Ĵ�СΪw*h

������ͼ����������ֵ��x��y����������ڱ�������Ե�
@param[in] Surface ָ�����ͼ�Ļ�ͼƽ���ָ��
@param[in] x ��ʼ����ս�������
@param[in] y ��ʼ��������
@param[in] xoff ͼ��image�������ݵ�ƫ�����������귽��
@param[in] yoff ͼ��image�������ݵ�ƫ�����������귽��
@param[in] w ����ͼ��Ŀ�ȣ����wС��image��ʵ�ʿ�ȣ�ֻ����w��ȵ����ݣ����򣬰���ͼ���ʵ�ʿ�Ȼ��ƣ�
@param[in] h ����ͼ��ĸ߶ȣ����hС��image��ʵ�ʸ߶ȣ�ֻ����h�߶ȵ����ݣ����򣬰���ͼ���ʵ�ʸ߶Ȼ��ƣ�
@param[in] attribute ������Ŀ���ͼƽ�棨Surface��
@param[in] image ����������������µĺ�����
@return �����óɹ�ʱ������0;ʧ��ʱ���ط�0
@note ������ͼ����������ֵ��x��y����������ڱ�������Ե� 
*/
INT CSGraphDrawImage(CSGraphSurface * Surface, INT x, INT y, INT xoff, INT yoff, INT w, INT h,
                     CSGraphPixmap * image, CSGraphGC * attribute);

/**
@brief ����ͼƽ��src_surface�ľ�������src_x, src_y, w, h����������ͼƽ��dest_surface��λ�ã�dest_x, dest_y���ϣ����������ţ���

@param[in] src_surface �����������������ڵĻ�ͼƽ�棨Surface��
@param[in] src_x ��������������ĺ�����
@param[in] src_y ���������������������
@param[in] w ��������������Ŀ�
@param[in] h ��������������ĸ�
@param[in] dest_surface ������Ŀ���ͼƽ�棨Surface��
@param[in] dest_x ����������������µĺ�����
@param[in] dest_y ����������������µ�������
@param[in] attribute attribute
@return �����óɹ�ʱ������0;ʧ��ʱ���ط�0
@note �������õĹ�����attribute->pixel_mode����ָ�� (pixel_mode�����Ķ������2.5.10��)����������ͼƽ��Ӧ�þ�����ͬ���ظ�ʽ(Pixel Format)��
����ʱӦ���ǵ�CSGraphGC���ƶ��Ļ滭ģʽ�����Դ��Ŀ��ƽ��Ļ�ͼģʽ��һ������Ӧ���Զ�����ת�������ϵͳ��֧�ֲ�ͬģʽ��ͼƽ���Ŀ�����
�򷵻�ʧ�ܡ� 
*/
INT CSGraphCopyArea(CSGraphSurface * src_surface, INT src_x, INT src_y, INT w, INT h,
                    CSGraphSurface * dest_surface, INT dest_x, INT dest_y, CSGraphGC * attribute);
                    

/**
@brief ����ͼƽ��src_surface�ľ�������xsrc, ysrc, wsrc, hsrc����������ͼƽ��
 dest_surface��λ�ã�xdst, ydst, wdst, hdst���ϣ��������ţ���

��Դsurfaceָ����������ݣ����ſ�����Ŀ��surface�ϡ�
�����ķ�ʽ��attribute��pixels_modeָ����
��֧��CS_MODE_COPY �� CS_MODE_ALPHA ��ģʽ��
ͬʱ��ȡattribute->blend_factor���ԣ���֧��ȫ��͸���ȡ�
@param[in] src_surface Դsurfaceָ��
@param[in] xsrc Դsurface�����x����
@param[in] ysrc Դsurface�����y����
@param[in] wsrc Դsurface����Ŀ��
@param[in] hsrc Դsurface����ĸ߶�
@param[in] dest_surface Ŀ��surfaceָ��
@param[in] xdst Ŀ��surface�����x����
@param[in] ydst Ŀ��surface�����y����
@param[in] wdst Ŀ��surface����Ŀ��
@param[in] hdst Ŀ��surface����ĸ߶�
@param[in] attribute  GC��ز�������Ҫ�õ�pixels_mode��blend_factor����
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ��
*/
INT CSGraphCopyAreaScaled(CSGraphSurface * src_surface, 
	INT xsrc, INT ysrc, INT wsrc, INT hsrc, 
	CSGraphSurface * dest_surface, 
	INT xdst, INT ydst, INT wdst, INT hdst, 
	CSGraphGC * attribute);

/**
@brief ��ȡ��ͼƽ�������

@param[in] nIndex  0 ��ʾ���� 1 ��ʾ����
@param[out] psDisplaySurface  ��ʾƽ��
@return �����óɹ�ʱ������0;ʧ��ʱ���ط�0
@note  ��ȡ��ͼƽ�������	
*/

INT CSGraphGetDisplayDeviceSurface(int nIndex, CSGraphSurface *psDisplaySurface);

/**
@brief ���Դ���д����

@param[in] surface	ƽ���ϵ�����
@param[in] x  x����
@param[in] y  y����
@param[in] width  ���
@param[in] height  �߶�
@return �����óɹ�ʱ������0;ʧ��ʱ���ط�0
@note  ���Դ���д����	
*/
INT CSGraphSetDisplayDeviceSurface(CSGraphSurface *surface,INT x, INT y, INT width, INT height);


/**
@brief ����ͼƽ��surface�ľ�������x, y, w, h������ͼ�󣬿�����һƬ�ڴ���

@param[in] surface �����������������ڵĻ�ͼƽ�棨Surface�� 
@param[in] x ��������������ĺ�����
@param[in] y ���������������������
@param[in] w ��������������Ŀ�
@param[in] h ��������������ĸ�
@param[in] pixmap ��ſ���ͼ����Ϣ��pixmap�ṹָ�롣
@return �����óɹ�ʱ������0;ʧ��ʱ���ط�0
@note ���������ڴ�ռ����ϲ���䣬
imagebits�Ĵ�СΪpixmap->width*pixmap->height* pixmap-> pixel_format. BytesPerPixel,pixmap�ṹ�е�
width��height��pixel_format.BytesPerPixel�����ݶ�Ӧ��Ӧ����䣬�������ɴ˺�����䣬pixmap����Ϊ�������������
�ú������ý�����Ӧ�ÿ�������ʹ��pixmap����imagebits�����ݣ������Ը�����Ҫ�����κ�ʱ���ͷ�imagebitsָ��Ŀռ䣨
����pixmap�ṹռ�õĿռ�--���pixmap�ṹҲ�Ƕ�̬�����һ���ڴ�ռ䣩  
*/
INT CSGraphSnapShot(CSGraphSurface * surface, INT x, INT y, INT w, INT h, CSGraphPixmap * pixmap);

	/**
	@brief  ����OSG�����ʾ����
	 
	@param[in]
			int nType ����osg������
			
			����ȡֵ����Ϊ:
			GRA_DISPLAYOPTION_COLOR  ΪOSG��ʾ���
			GRA_DISPLAYOPTION_3D ΪOSG֧��3DTV
			
	@param[in]
			int nValue ���õĶ�Ӧ����nType��ֵ
			
			��nTypeΪGRA_DISPLAYOPTION_COLORʱ��
				nValue ֵΪ1,��ʾ����OSG���
				nValue ֵΪ0,��ʾ����OSG�ָ�����
				
			��nTypeΪGRA_DISPLAYOPTION_3Dʱ��
				nValue ֵΪ0,��ʾOSG������ʾ
				nValue ֵΪ1,��ʾ��OSGһ��Ϊ��������ʾ
				nValue ֵΪ2,��ʾ��OSGһ��Ϊ��������ʾ	

	@return �����óɹ�ʱ������0;ʧ��ʱ���ط�0
	 		
	@exception
	@note
	*/
	INT CSGraphSetDisplayOption(INT nType, INT nValue);

#ifdef __cplusplus
}
#endif

/** @} */

#endif

