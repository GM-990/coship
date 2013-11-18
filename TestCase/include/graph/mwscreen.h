
/**@defgroup OSG OSG ����ƽ̨��ͼ�ӿ�,���׽ӿڲ�������̵߳���

@note ����ģ�������ϵͳΪ:X������������죬Y�������������

@version 2.0.6 2009/9/07 ����colorkeyflag ��alphaflag
@version 2.0.5 2009/8/28 �Ѿ�������ṹ������public����ͷ�ļ���
@version 2.0.4 2009/8/27 ����ͼƬӲ��
@version 2.0.3 2009/8/26 �޸�colorKey����Ϊ�޷�����
@version 2.0.2 2009/8/24 ��һ���ۺ�������޸ĵİ汾
@version 2.0.1 2009/07/08 ���������۹��İ汾
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

	/**@brief ȫ�ֵ�ALPHAģʽ*/
#define CS_ALPHA_MODE_GLOBAL	1
	/**@brief �ֲ�ALPHAģʽ*/
#define CS_ALPHA_MODE_LOCAL	2

	/**@brief ͼ���ʾ������--Ӳ����ʾ*/    
#define CS_SURFACE_TYPE_HARDWARE  0		//used
	/**@brief ͼ���ʾ������--��������ڴ�*/
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

	/*arc_style���ñ��������˻��εĻ��ƺ����ģʽ���������£�     */
	/**@brief ���εĻ��ƺ����ģʽ--�������η������ȱʡʱ��ģʽ */
#define CS_ARC_STYLE_CENTERCHORD_ARC_MODE 	0	
	/**@brief ���εĻ��ƺ����ģʽ--��Pie�η������ */
#define CS_ARC_STYLE_LINEPIESLICE_ARC_MODE 	1	

/*clip flag*/
#define CS_CLIPPING_NO_RECT_NO_MASK	0	/**< �޼��������Σ�pixel_mode������������Ч */
#define CS_CLIPPING_VALID_RECT_NO_MASK	1	/**< �м���������,pixel_mode���ڼ��о�����������Ч(�����߽�) */
#define CS_CLIPPING_NO_RECT_VALID_MASK	2	/**< �޼���������,pixel_mode���ڼ�����������,�Ҹ���������Ϊ1ʱ��Ч��������������߽� */
#define CS_CLIPPING_VALID_RECT_AND_MASK	3	/**< �м���������,pixel_mode���ڼ��������û�н�������ִ���κβ����������һ�齻����pixel_mode��Ӧ������������Ϊ1����һ���� */


#if 1
	/**@brief �����Ľṹ��*/
	typedef struct _CSPoint {
		int x;	/**< ���x����*/
		int y;	/**< ���y����*/
	}CSGraphPoint;

	/**@brief ����RGB������ʾ����ɫ�ṹ��*/
	typedef struct _CSRGBColor {
		unsigned char a;		/**< ͸������*/
		unsigned char r;		/**< ��ɫ����*/
		unsigned char g;		/**< ��ɫ����*/
		unsigned char b;		/**< ��ɫ����*/
	} CSGraphRGBColor;

	/**@brief ����YUV������ʾ����ɫ�ṹ��*/
	typedef struct _CSYUVColor {
		unsigned char a;		/**< alphaֵ*/
		unsigned char y;		/**< ���ȷ���*/
		unsigned char u;		/**< ��ɫɫ�ȷ���*/
		unsigned char v;		/**< ��ɫɫ�ȷ���*/
	} CSGraphYUVColor;

	/**@brief ��ɫ��ṹ��*/
	typedef  struct _CSPalette{
		unsigned char	type;				/**< ��ɫģʽCSGraphPalette_TYPE_RGB����CSGraphPalette_TYPE_YUV*/
		unsigned short	ncolors;				/**< ��ɫ���鳤��*/
		union {
			CSGraphRGBColor	*rgb;	/**< ��ɫ�����൱��rgb[ncolors]��Ӧ��typeΪCSGraphPalette_TYPE_RGB*/
			CSGraphYUVColor	*yuv;	/**< ��ɫ�����൱��yuv[ncolors]��Ӧ��typeΪCSGraphPalette_TYPE_YUV*/
		} col;
	} CSGraphPalette;


	/**@brief λͼ��ʽ�ṹ��*/
	typedef struct _CSPixelFormat {
		unsigned char  BitsPerPixel;			/**< ÿ�����ص�λ��*/
		unsigned char  BytesPerPixel;		/**< ÿ�����ص��ֽ���*/
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
		unsigned char  transparence;		/**< ͸����Alpha value information (per-surface alpha) */
		unsigned int   alpha_blending_mode;	/**< alpha���ģʽCS_ALPHA_MODE_GLOBAL or CS_ALPHA_MODE_LOCAL*/
	} CSGraphPixelFormat;

	/**@brief λͼ�ṹ��*/
	typedef struct _CSPixmap{
		unsigned short				width;		/**< image width in pixels*/
		unsigned short				height;		/**< image height in pixels*/
		unsigned char				planes;		/**< image planes*/
		unsigned short				pitch;		/**< bytes per line*/
		unsigned char				compression;		/**< compression algorithm*/
		CSGraphPixelFormat		pixel_format;	/**< bits per pixel(1,4 or 8,16,24,32)*/
		CSGraphPalette			*palette;		/**< ��ɫ���ַ*/
		unsigned char  		        	*imagebits;			/**< image bits (int right aligned)*/
		unsigned char				colorkeyflag;   		/**< indicate whether to use colorkey in pixel_format */
		unsigned char				alphaflag;      		/**< indicate whether to use alpha mode in pixel_format */
		unsigned char  		        	*YUVbits;			/**< image yuv bits (int right aligned)*/
		unsigned short				YUVwidth;		/**< imagev yuv  width in pixels*/
		unsigned short				YUVheight;		/**< image yuv height in pixels*/
	}CSGraphPixmap;



	typedef struct	_CSGC{
		int		font_id;		/**< �ֿ�ָ��*/
		int		id;			/**< Identifier */
		unsigned int		background_color;	/**< ������ɫ					*/
		unsigned int		foreground_color;	/**< ǰ����ɫ					*/
		unsigned int		pen_width;		/**< ���ʿ��					*/
		unsigned int		pen_style;		/**< ��������					*/
		unsigned char	       *dash_format;		/**< ���߸�ʽ					*/
		unsigned int		dash_length;		/**< ���߳���					*/
		unsigned char	        cap_style;		/**< ĩ������					*/
		unsigned char	        join_style;		/**< ��������					*/
		unsigned char	        poly_fill_style;	/**< ������������				*/
		unsigned char	        arc_style;		/**< ����ģʽ					*/
		unsigned char		blending_factor;	/**< ������ӣ�͸���ȣ�			*/
		char		pixel_mode;		/**< �µ�����д���ڴ�ʱ�Ծɵ����صĹ�ϵ*/
		char		clip_flag;		/**< ���б�־					*/
		short	        clip_area_x;		/**< ���о���x����				*/
		short	        clip_area_y;		/**< ���о���y����				*/
		short	        clip_area_width;	/**< ���о��ο��				*/
		short	        clip_area_height;	/**< ���о��θ߶�				*/
		short	        clip_mask_x;		/**< �������ξ���x����			*/
		short	        clip_mask_y;		/**< �������ξ���y����			*/
		short	        clip_mask_width;	/**< �������ξ��ο��			*/
		short	        clip_mask_height;	/**< �������ξ��θ߶�			*/
		CSGraphPixmap	*pixmap_mask;	        /**< ������������ͼָ��			*/
		unsigned int		plane_mask;		/**< ����ͼӰ�쵽��ÿ������λ����	*/
	} CSGraphGC;


	/**@brief ��ʾ�豸*/
	typedef struct _CSDisplayDevice {
		char		*name;			        /**< �豸����*/
		int			width;			        /**< ��ʾ������,����Ļ�ֱ���,���֧��Ӳ���������ֵ�����ο�*/
		int			height;			        /**< ��ʾ������,����Ļ�ֱ���,���֧��Ӳ���������ֵ�����ο�*/
		int			region_overlapable;	    /**< Region�Ƿ�֧���ص�:0-��֧��,1-֧��;��*/
		int			anti_aliasing;		    /**< �Ƿ�֧���۵�,0-��֧��,1-֧��*/
		CSGraphPixelFormat	*display_mode;	/**< ��ʾģʽ*/
		int			display_mem_size;	    /**< �Դ��С�����ֽ�Ϊ��λ*/
		void		*display_mem;		    /**< �Դ��ַ*/
	}CSGraphDisplayDevice;


	/**@brief Surface�����ڻ�������˼*/
	typedef struct _CSSurface {
		int	                id;			/**< The Surface's identifier */
		struct _CSRegion   	*associated_region;	/**< ��Surface��������region*/
		char                	type;  			/**< The type of the surface - memSurface(CS_SURFACE_TYPE_MEMORY) / hardwareSurface(CS_SURFACE_TYPE_HARDWARE)*/
		CSGraphPixelFormat  	format;			/**< ���ظ�ʽ*/
		CSGraphPalette      	*palette;		/**< ��Surface��Ӧ�ĵ�ɫ��*/
		int 		   	width;			/**< ��Surface�Ŀ��,����������*/
		int 		    	height;			/**< ��Surface�ĸ߶�,����������*/
		void                	*surface_mem;      	/**< �����surface �ڴ���ʼ��ַ */
		int                 	surface_mem_size;	/**< ��Surface���ڴ��С,���ֽ�Ϊ��λ*/
	} CSGraphSurface;

	/**@brief Region -����ʾ������*/
	typedef struct _CSRegion {
		int				id;			/**< The region's identifier */
		CSGraphPixelFormat              format;			/**< ���ظ�ʽ*/
		CSGraphPalette                  *palette;		/**< ��Region��Ӧ�ĵ�ɫ��*/
		unsigned short				pitch;			/**< �洢һ����������Ҫ���ֽ���=ÿ�е�������*ÿ�����ص��ֽ���*/
		int 				x;			/**< Region��x����*/
		int 				y;			/**< Region��y����*/
		int 				width;			/**< Region�Ŀ��*/
		int 				height;			/**< Region�ĸ߶�*/
		CSGraphSurface	    		*activated_surface;	/**< Region������Surface*/
		int		             	max_nb_surface;		/**< Region�����Թ�����Surface����*/
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
	@brief ����ʾ�豸������ͼ���ĳ�ʼ��������
	@return �ɹ�����CSOSG_SUCCESS,ʧ�ܷ���CSOSG_FAILURE
	@note ���ǵ���ͼ��⺯��֮ǰ��Ӧ���õĵ�һ��������
	*/
	int MWScreenDisplayDeveiceOpen(void);

	/**
	@brief �ر���ʾ�豸���ͷ���Ӧ��Դ��

	@note �˺��������ر�ͼ��⡣�˺������ú��������κ�ͼ��⺯�����þ�������Ч��
	*/
	void MWScreenDisplayDeviceClose(void);

	/**
	@brief ��ȡͼ�������豸��ָ��

	@return �ɹ�ʱ���ػ�ȡ��ͼ�������豸��ָ��,ʧ�ܷ��ؿ�
	@note ��ȡ��ָ������ʾ�豸û�б��ر�ʱȫ����Ч��
	*/
	CSGraphDisplayDevice* MWScreenGetDisplayDeviceInfo(void);


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
	CSGraphRegion*  MWScreenCreateRegion(int x, int y,
		int width, int height,
		CSGraphPixelFormat *format,
		int *error_code);

	/**
	@brief ɾ��ָ������ʾ����region��

	@param[in] region ����ɾ���������ָ��
	@return �ɹ�ʱ����0,ʧ��ʱ���ط�0
	@note �ͷ�������ʾ�����йص��ڴ档������������ڱ���ʾ������ʾ�����û��ɺ�ɫ����Black����������Ӧ�����л�ͼƽ���Ӧ�Զ����ͷš� 
	*/
	int MWScreenDeleteRegion(CSGraphRegion *region);

	/**
	@brief ����һ����ʾ��ͼƽ��

	@param[in] region ��ͼƽ������Ӧ����ʾ����
	@param[out] error_code �������
	@return ����һָ��CSGraphSurface�ṹ��ָ�룻���򷵻�NULL��
	@note ͼ�����������Ӧ����ʾ����(region)��ͬ��ͼ����������Ӧ֧��һ����ͼƽ�档
	@note �ṹ�е�id�ĸ�ֵ������ʵ�ֲ��������Ӧ��֤ÿ����ͬ��Surface��id�ǲ�һ���ġ�
	*/

	CSGraphSurface*  MWScreenCreateDisplaySurface(CSGraphRegion *region,int *error_code);

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
	CSGraphSurface*  MWScreenCreateMemSurface(CSGraphRegion *region,
		int width, int height,
		CSGraphPixelFormat *format,
		CSGraphPalette 		*palette,
		int *error_code);

	/**
	@brief ɾ��ָ���Ļ�ͼƽ�棨Surface�����ͷ��������ƽ���йص��ڴ�

	@param[in] Surface ����ɾ����ƽ���ָ��
	@return ����һָ��CSGraphSurface�ṹ��ָ�룻���򷵻�NULL��
	@note ����ͷŵ�������ʾ������������Ӧ�����л�ͼƽ���Ӧ�Զ����ͷţ������ڴ�ƽ�����ʾƽ�棩��
	*/
	int MWScreenDeleteSurface (CSGraphSurface *Surface);

	/**
	@brief ����ָ�������� 

	@param[in] region �������ص������ָ��
	@return �ɹ�����0,ʧ�ܷ��ط�0
	@note ������������ڱ���ʾ��������ʾ��������ʧ��
	*/
	int MWScreenHideRegion(CSGraphRegion *region);

	/**
	@brief ����ָ�������� 

	@param[in] region �������ص������ָ��
	@return �ɹ�����0,ʧ�ܷ��ط�0
	@note ������������ڱ���ʾ��������ʾ��������ʧ��
	*/
	int MWScreenShowRegion(CSGraphRegion *region);

	/**
	@brief �ƶ�ָ���������������꣨x��y��

	@param[in] region �����ƶ��������ָ��
	@param[in] x �µ�����X
	@param[in] y �µ�����Y
	@return �ɹ�����0,ʧ�ܷ��ط�0
	@note ���ͼ������Ӳ�������ʵ��֧�֣����꣨x��y����������ʾ����֮�⡣������������ڱ���ʾ������ʾ������Ҳ��regionͬʱ�ƶ���
	*/
	int MWScreenMoveRegion(CSGraphRegion *region, int x, int y);

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
	int MWScreenSetPalette(CSGraphRegion *region, CSGraphPalette *palette,int firstcolor,int ncolors);

	/**
	@brief ��ȡ����ĵ�ɫ��Palette�� 

	@param[in] region ��Ҫ��ȡ��ɫ�������ָ��
	@param[out] error_code �������
	@return �����óɹ�ʱ������ָ������Palette��ָ�루CSGraphPalette����������ʧ��ʱ������NULL��
	@note �ú�������һCSGraphPaletteָ�룬�û������Խ����ͷŵ�����������벻Ϊ0ʱ�����ص�CSGraphPaletteָ��ӦΪNULL��
	*/
	CSGraphPalette* MWScreenGetPalette(CSGraphRegion *region,int *error_code);

	/**
	@brief �������������Alpha blendingģʽ�� 

	@param[in] region ��Ҫ��ȡ��ɫ�������ָ��
	@param[in] alpha_mode ��Ҫ���õ�Alphaģʽ����Ϊ��CSALPHA_MODE_GLOBAL ��CSALPHA_MODE_LOCAL
	@return �����óɹ�ʱ������0;ʧ��ʱ���ط�0
	*/
	int MWScreenSetAlphaMode(CSGraphRegion *region,unsigned char alpha_mode);

	/**
	@brief ���������͸���ȡ�

	@param[in] region ָ��������͸���ȵ������ָ��
	@param[in] transparence �������͸���ȣ���%��ʾ��0��ʾ��ȫ��͸����100��ʾ��ȫ͸��
	@return �����óɹ�ʱ������0;ʧ��ʱ���ط�0
	*/
	int MWScreenSetTransparence(CSGraphRegion *region, unsigned char transparence);

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
	int MWScreenSetAntiFlickering(CSGraphRegion *region, int antiflickering);

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
	int MWScreenSetActivatedSurface(CSGraphRegion *region,
		CSGraphSurface *surface,
		int src_x, 
		int src_y,
		int width,
		int height);

	/**
	@brief ��(x,y)���������ص㡣

	@param[in] Surface ָ�����ͼ�Ļ�ͼƽ���ָ��
	@param[in] x ���Ƶ�ĺ�����
	@param[in] y ���Ƶ��������
	@param[in] attribute ��ͼ���õ�����
	@return �����óɹ�ʱ������0;ʧ��ʱ���ط�0
	@note ���ص����ɫ�ʹ�С��attribute�е�foreground_color��pen_widthָ����
	*/
	int MWScreenDrawPixel(CSGraphSurface *Surface,int x,int y,CSGraphGC *attribute);

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
	int MWScreenReadPixel(CSGraphSurface *Surface,int x,int y,unsigned int *c);

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
	int MWScreenDrawLine(CSGraphSurface * Surface, int x1, int y1, int x2, int y2, CSGraphGC * attribute);

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
	int MWScreenDrawRect(CSGraphSurface * Surface, int x1, int y1, int x2, int y2, CSGraphGC * attribute);

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
	int MWScreenFillRect(CSGraphSurface * Surface, int x1, int y1, int x2, int y2, CSGraphGC * attribute);

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
	int MWScreenDrawCircle(CSGraphSurface * Surface, int x, int y, int radius, CSGraphGC * attribute);

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
	int MWScreenFillCircle(CSGraphSurface * Surface, int x, int y, int radius, CSGraphGC * attribute);

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
	int MWScreenDrawEllipse(CSGraphSurface * Surface, int x, int y, int a, int b, int alpha, CSGraphGC * attribute);

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
	int MWScreenFillEllipse(CSGraphSurface * Surface, int x, int y, int a, int b, int alpha, CSGraphGC * attribute);

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
	int MWScreenDrawArc(CSGraphSurface * Surface, int x, int y, int width, int height, int alpha, int beta, CSGraphGC * attribute);

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
	int MWScreenFillArc(CSGraphSurface * Surface, int x, int y, int width, int height, int alpha, int beta, CSGraphGC * attribute);

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
	int MWScreenDrawPoly(CSGraphSurface * Surface, int number_points, CSGraphPoint * points, CSGraphGC * attribute);

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

	int MWScreenFillPoly(CSGraphSurface * Surface, int number_points, CSGraphPoint * points, CSGraphGC * attribute);

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
	int MWScreenDrawImage(CSGraphSurface * Surface, int x, int y, int xoff, int yoff, int w, int h,
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
	int MWScreenCopyArea(CSGraphSurface * src_surface, int src_x, int src_y, int w, int h,
		CSGraphSurface * dest_surface, int dest_x, int dest_y, CSGraphGC * attribute);


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
	int MWScreenCopyAreaScaled(CSGraphSurface * src_surface, 
		int xsrc, int ysrc, int wsrc, int hsrc, 
		CSGraphSurface * dest_surface, 
		int xdst, int ydst, int wdst, int hdst, 
		CSGraphGC * attribute);

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
	INT MWScreenSnapShot(CSGraphSurface * surface, INT x, INT y, INT w, INT h, CSGraphPixmap * pixmap);

	/**
	@brief ��ȡ��ͼƽ�������

	@param[in] nIndex  0 ��ʾ���� 1 ��ʾ����
	@param[out] psDisplaySurface  ��ʾƽ��
	@return �����óɹ�ʱ������0;ʧ��ʱ���ط�0
	@note  ��ȡ��ͼƽ�������	
	*/

	INT MWScreenGetDisplayDeviceSurface(int nIndex, CSGraphSurface *psDisplaySurface);

	/**
	@brief ���Դ���д����

	@param[in] surface  ƽ���ϵ�����
	@param[in] x  x����
	@param[in] y  y����
	@param[in] width  ���
	@param[in] height  �߶�
	@return �����óɹ�ʱ������0;ʧ��ʱ���ط�0
	@note  ���Դ���д����	
	*/
	INT MWScreenSetDisplayDeviceSurface(CSGraphSurface *surface,INT x, INT y, INT width, INT height);


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
	INT MWScreenSetDisplayOption(INT nType, INT nValue);

#ifdef MMCP_QT
#include "qtadapter.h"
#endif


//������handle
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
@brief ����һ����ͼƽ��

@param[in] ePixelFormat ��ͼƽ�����ɫģʽ
@param[in] nWidth  ��ͼƽ��Ŀ�ȣ�������� 0
@param[in] nHeight ��ͼƽ��ĸ߶ȣ�������� 0
@param[out] phSurface �ɹ����ػ�ͼƽ��ľ����ʧ��Ϊ�գ���ϸ����μ��������ֵ
@return �ɹ����� CSUDI_SUCCESS,���򷵻ش������ֵ 
@note �ýӿڴ�����ʾ����ռ䣬�����Ͽ���֧�����޴�֧�ִ������޶��ֱ���ڴ治�㣬(�����ṩ16M���ڴ����ڴ���Surface)
      ������Ҫ����Surface������Ŀ����Ĳ�ͬ����ͬ(ĿǰӦ���õ��Ƚ϶����:1.��ϷӦ���еĿɱ�ͼƬ���õ���Surface 2.JavaӦ����
      �õ�BufferedImage�õ���Surface 3.2D��Ч��ʵ�ֻ��õ���Surface)
@note �˴���ɫģʽ������������ʾ����ɫģʽ��ͬ����Blitʱ�����⴦���ɣ����ƽ̨��֧�������Ĵ���Ҳ����Ӧ��ѡ����ɫģʽʱ���Ա���
@note ���ƽ̨��Դʵ�����ޣ�Ҳ�������ܴ����κ�Surface����������»�ֱ�Ӳ����Դ棬������Ҫ��������Դ�ķֱ��ʣ���ɫģʽ�ȱ���һ��
@note ���ȵ���CSUDIOSGCreateSurface ����surface���������CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED�������Լ�����ͨ�ڴ��д���surface��
*/
CSUDI_Error_Code UDIOSGAdapterCreateSurface(CSGraphPixelFormat *psGraphPixelFormat, int nWidth, int nHeight, UDIAdapterSurface_S **ppsSurface);


/**
@brief ����һ����ͼƽ�棬�ͷŸû�ͼƽ����ڴ�

@param[in] hSurface CSUDIOSGCreateSurface �ɹ����صľ��
@return �ɹ����� CSUDI_SUCCESS�����򷵻ش������ֵ 
@note ��CSUDIOSGCreateSurface�����Ļ�������ʹ�ú�һ��Ҫ���ô˽ӿ������ͷ�
@note ��������һ���Դ�ռ䣬������CSUDIOSG_ERROR_BAD_PARAMETER
@note ���ȵ���CSUDIOSGDestroySurface ����surface���������CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED�����Լ��ͷ�surface��
*/
CSUDI_Error_Code UDIOSGAdapterDestroySurface(UDIAdapterSurface_S  *psSurface);


/**
@brief �õ��Դ�surface���

@param[in] nIndex �Դ���������0��ʼֱ��û�и�����Դ棬����ʧ��Ϊֹ
@param[out] phSurface �ɹ������Դ�Surface���

@return �ɹ����� CSUDI_SUCCESS,���򷵻ش������

@note ���ַ�ʽ�õ���Surface����ʹ��CSUDIOSGDestroySurface����
@note ����߱���ͬʱ֧�֣�nIndex Ϊ 0 ��ʾ�����Դ棻nIndex Ϊ 1 ��ʾ�����Դ�
@note ֻ��Ҫ����������������OSD��ʾ���Դ�,������Դ棬�����Դ棬��Ҫ����������Դ�����㣬������ȡ�
@note ���ǲ��ܴ����κ�����Surface�������м��һ�㲻��ֱ�Ӳ����Դ��ַ���л���,�þ�����������ֻ���������Ͻ���Blit
@note ���ȵ���CSUDIOSGGetDisplaySurface, UDI�����֧�ִ˽ӿڣ������޷���ʾͼ�����ݵ���Ļ�ϡ�

*/
CSUDI_Error_Code UDIOSGAdapterGetDisplaySurface(int nIndex, UDIAdapterSurface_S *psSurface);


/**
@brief ��ȡ��ͼƽ������Դ�ռ����Ϣ

�м����ͨ���ýӿڻ�ȡ����Ϣ���л�ͼ����
@param[in] hSurface ��ͼƽ������Դ�ռ���.CSUDIOSGCreateSurface����CSUDIOSGGetDisplaySurface�ķ���ֵ
@param[out] psSurfaceInfo ����õ��Ļ�ͼƽ������Դ�ռ���Ϣ
@return �ɹ����� CSUDI_SUCCESS�����򷵻ش������ֵ 
@note �˽ӿ��ܹ��õ��Դ���߻���Ŀռ��ַ
@note ���ȵ���CSUDIOSGGetSurfaceInfo���������CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED������Ҫ�������ʵ�ִ˺�����
*/
CSUDI_Error_Code UDIOSGAdapterGetSurfaceInfo(UDIAdapterSurface_S *psSurface, CSUDIOSGSurfaceInfo_S * psSurfaceInfo);

/**
@brief ���û�ͼƽ�����Ϣ��ֻ�����û�ͼƽ���m_nAlphaFlag��m_uAlpha��m_nColorKeyFlag��m_uColorkey�ĸ���Ա��
       ��������Ա������Ч

@param[in] hSurface ��ͼƽ����.CSUDIOSGCreateSurface����ֵ
@param[in] psSurfaceInfo ����õ��Ļ�ͼƽ������Դ�ռ���Ϣ
@return �ɹ����� CSUDI_SUCCESS�����򷵻ش������ֵ 
@note �˽ӿ���CSUDIOSGGetSurfaceInfo��ʹ�ã�ֻ�����û�ͼƽ�����Ϣ�����������Դ�ռ����Ϣ
@note �˽ӿ��ڽ������ڸı�Surface��m_uAlpha��m_uColorkey���ԣ����������ʾ�����仯
@note ��psSurfaceInfo��ͬʱ�г�m_nAlphaFlag��m_uAlpha��m_nColorKeyFlag��m_uColorkey����ĳ�Ա�ı�ʱ��
      ������Ա�޸Ĳ�����Ч��������m_nAlphaFlag��m_uAlpha��m_nColorKeyFlag��m_uColorkey��Ч������CSUDI_SUCCESS
@note ���ȵ���CSUDIOSGSetSurfaceInfo���������CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED������Ҫ�������ʵ�ִ˺�����
*/
CSUDI_Error_Code UDIOSGAdapterSetSurfaceInfo(UDIAdapterSurface_S *psSurface, const CSUDIOSGSurfaceInfo_S * psSurfaceInfo);


/**
@brief Blit�����ӿڣ�������Ӳ���������ܣ��첽����

@param[in] hDstSurface Ŀ���ͼƽ������Դ�ռ���.CSUDIOSGCreateSurface����CSUDIOSGGetDisplaySurface�ķ���ֵ
@param[in] psDstRect Ŀ��surface����ʾ�ռ�hDstSurface�Ͼ�������,���Ϊ�գ���ȡhDstSurface��ȫ������
@param[in] hSrcSurface Դ��ͼƽ������Դ�ռ���
@param[in] psSrcRect Դsurface����ʾ�ռ�hSrcSurface�Ͼ�������,���Ϊ�գ���ȡhSrcSurface��ȫ������
@param[in] eOpMode ���ģʽ����CSUDIOSGOpMode_E����
@param[in] bEnableFilter �Ƿ����alphaΪ0�����أ�TRUE ��ʾ���ˣ�FALSE ��ʾ�����ˡ�
@return �ɹ����� CSUDI_SUCCESS���������ش�����롣
@note ʹ��hSrcSurface�е�alphaֵ�����Ҫ���ã�ʹ��CSUDIOSGSetSurfaceInfo;
@note �����֧��Ӳ�������뷵��CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED���������и���Ч��ʵ�ַ�ʽ����������ֵ�����,��Ҫ�Ըò����������ģ��.
@note Դ��Ŀ��������СpsSrcRect��psDstRect����Ϊ�գ����Ƿǿ�ʱ�������Ƕ�Ӧsurface�ϴ�С�ķ�Χ�ڣ���֧�ֲü��򷵻�CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED
@note Դ��Ŀ�������Ϊ�Դ�Surface������ͨSurface,����֧���뷵��CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED
@note Դ��Ŀ�����Ϊͬһ��Surface,����֧���뷵��CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED
@note Դ��Ŀ�����Ϊͬһ��Surfaceʱ��psDstRect��psSrcRect�������ص������򷵻�CSUDIOSG_ERROR_BAD_PARAMETER
@note Դ��Ŀ�����ɫģʽ���ܲ�һ��,һ�㲻�Ƽ�����ʹ�ã�����֧�ֿɷ���CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED
@note Դ��Ŀ��Ŀ��ֱܷ���YUV��RGB����ɫģʽ,����֧�ֿɷ���CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED
@note Դ��Ŀ����εĴ�С���ܲ�һ��,����֧�ֿɷ���CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED,��һ����˵����ƽ̨����֧��,���������Ч�ʼ���,�м�������ṩ
@note Դ��Ŀ��ľ��ο��������š������ȴ���,����һ���뷵��CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED,��֧��2D��Ч��ƽ̨����ʵ�ָù���
@note Ϊ�����߻�ͼ���ܣ��ýӿڿ�ʵ�ֳ��첽ģʽ��ͨ��CSUDIOSGSyncȷ���ò���ִ�����,
@note ��CSUDIOSGSync����ǰ�ı����Surface,��ͼЧ���ǲ�ȷ����,����������������������ɻָ����쳣
@note ���ƽ̨ͬʱ֧�ָ߱�����ʾ��Ҫ����������Դ�blitʱ��һ��Ҫ���÷���(���߷����)����
@note ƽ̨֧�ֵ���������ű�������Ϊ: 1:15
@note ���ȵ���CSUDIOSGBlit���������CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED������Ҫ�������ʵ�ִ˺�����
*/
CSUDI_Error_Code UDIOSGAdapterBlit(UDIAdapterSurface_S *psDstSurface, const CSUDIOSGRect_S * psDstRect,
                       UDIAdapterSurface_S *psSrcSurface, const CSUDIOSGRect_S * psSrcRect, CSUDIOSGOpMode_E eOpMode, CSGraphPixelFormat *psSrcPixelFormat, CSGraphPixelFormat *psDstPixelFormat, BOOL bEnableFilter);


/**
@brief ���Ŀ���ͼƽ������Դ�ռ���һ�����

@param[in] hDstSurface ��ͼƽ������Դ�ռ���.CSUDIOSGCreateSurface����CSUDIOSGGetDisplaySurface�ķ���ֵ
@param[in] psDstRect Ҫ�����Ŀ���ͼƽ���ϵ�λ�úʹ�С��Ϊ��ʱ����ʾȫ�����
@param[in] uPixel Ҫ������ɫֵ��ARGB8888��ʽ������ֵ
@return �ɹ����� CSUDI_SUCCESS�����򷵻ش������ֵ 
@note Ϊ�����߻�ͼ���ܣ��ýӿ���Ҫʵ�ֳ��첽ģʽ��ͨ��CSUDIOSGSyncȷ���ò���ִ�����,
��CSUDIOSGSync����ǰ�ı����Surface,��ͼЧ���ǲ�ȷ����,����������������������ɻָ����쳣
@note ����ʹ��Ӳ������Fill,���Ӳ����֧�֣��뷵��CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED,�������и���Ч��ʵ�ַ�ʽ����������ֵ�����,��Ҫ�Ըò����������ģ��.
@note psDstRect ������hDstSurface�Ĵ�С��Χ�ڣ��������hDstSurface���򷵻�CSUDIOSG_ERROR_BAD_PARAMETER
@note ���ȵ���CSUDIOSGFill���������CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED������Ҫ�������ʵ�ִ˺�����
*/
CSUDI_Error_Code UDIOSGAdapterFill(UDIAdapterSurface_S *psDstSurface, const CSUDIOSGRect_S * psDstRect, unsigned int unPixel,CSGraphPixelFormat *psPixelFormat);

/**
@brief ͬ��OSG����

@return �ɹ����� CSUDI_SUCCESS�����򷵻ش������ֵ 
@note ��CSUDIOSGBlit��CSUDIOSGFill��֮�����,��������ʱ��ǰ������ȫ�����
@note �ýӿ���Blit�Ĺ�ϵ�ɲο�C99��׼fflush
@note ���ȵ���CSUDIOSGSync���������CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED�����ʾȫ��ʹ��ͬ����ʽ��

*/
CSUDI_Error_Code UDIOSGAdapterSync(void);

/**
@brief Ӳ������ͼƬ,����I֡��ʽ(MPEG,BMP)/JPEG��ʽ/PNG��ʽ��

@param[in] pszImageType ͼƬ������,ʹ���ַ��������˶�����ö�����͡���ʽ�����͵Ķ���: IFRAME - I֡��ʽ��BMP - bmp��ʽ, JPEG - jpeg��ʽ, PNG - png��ʽ
@param[in] pvImageMem  ͼƬ���ݵ�ַ
@param[in] nMemSize   ͼƬ���ݴ�С
@param[out] phSurface ���������ݴ�ŵ�surface
@return �ɹ����� CSUDI_SUCCESS,���򷵻ش������

@note ����֧��Ӳ�����룬��ʱ����CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED
@note ʹ�ñ��ӿڵõ��Ļ�ͼƽ����ʹ�ú󣬱������CSUDIOSGDestroySurface�ͷ�
@note ������ӿڽ���ͼƬ(����I֡)�õ����豸����æ״̬���뷵��CSUDIOSG_ERROR_DEVICE_BUSY
@note ����I֡һ����Ҫ�õ�Video��������������Ƶ���ų�ͻ�����ӿ�Ӧ����ʹ������Video���������н��룬��ʵ���Ҳ������ý������򷵻�CSUDIOSG_ERROR_DEVICE_BUSY��
�������EastWin�����Ҫ���н��룬��Ӧ֪ͨ�ϲ���ͣ��Ƶ���ź��ٴε��øýӿ�
@note ���ӿڽ������ڿ���ͨ��Ӳ����ʽ���ٽ��������������зǳ���ֵ����ɣ������벻Ҫ�����κ����ģ��ʵ�֡�
*/
CSUDI_Error_Code UDIOSGAdapterDecodeImage(const char * pszImageType, const void * pvImageMem, int nMemSize, UDIAdapterSurface_S **ppsSurface);

/**
@brief ��ȡUDIAdapterSurface_S �ṹ���m_hSurface���
@param[in] int psSurface   -----��Ӧudi1��CSGraphSurface ��id��udi2��UDIAdapterSurface_S * psSurface
@param[out] CSUDI_HANDLE * hSurface��
@return ���ش������͡�
@note Ӧ�ó���:1. ����newһ��pixmap 2.����pixmap��surface id��SurfaceId,Ȼ��õ�SurfaceId��Ӧ��m_hSurface�����3 ֻ����Ӳ��������surface��Ӧ�ľ����
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
