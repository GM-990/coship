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

//=======CSTC_OSG_IT_SetSurfaceInfo_0001~4�õ���Щ����=========//
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
* ͼƬ����֧������ö��
*/
typedef enum
{
	EM_OSG_DECIMAGE_IFRAME,   //֧��IFRAMEͼƬӲ����
	EM_OSG_DECIMAGE_BMP,   //֧��BMPͼƬӲ����
	EM_OSG_DECIMAGE_JPG,   //֧��JPGͼƬӲ����
	EM_OSG_DECIMAGE_PNG,   //֧��PNGͼƬӲ����
	EM_OSG_DECIMAGE_MAX 
}OSGUDI2SupportDecIamgeType_E;

/*
* ��ͼƽ���Сö��
*/
typedef enum
{
	EM_SURFACE_1920_1080,
	EM_SURFACE_1280_720,
	EM_SURFACE_720_576
}OSGUDI2Surface_E;

/*
*���־�������ö�٣�ȡֵ���CSTC_OSG_IT_Blit_0000����
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
*����ͼƬ��Դ����ö��
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
	EM_JPG_DOUBLE_SOF,//JPGͼƬ������SOF,������μ�JPG��׼
	EM_JPG_SOF_Y_SAMPLE_12,//jpgͼƬSOF��ˮƽ��ֱ���ݲ�����Ϊ1:2,��ЩӲ��ƽ̨���ܲ�֧��
	EM_YUV_FASTJPG_DATA,
	EM_IMAGE_MAX
}OSGUDI2ImageType_E;

//alpha���ȡֵ��ʽö�����ͣ����CSTC_OSG_IT_Blit_0000˵��
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
	int 		nType;//����config��Ϣ�ķ�ʽ��0:�Ƚϸ�ֵ; 1:ֱ�Ӹ�ֵ
	int*		pSavebuf;//�������õ�config��Ϣ�ĵ�ַ
	char*	pCfgInfoKey;//�����Ϣ�Ĺؼ���
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
@brief ��ȡ�����ļ���Ϣ,��ʼ��֧�ֵ���ɫģʽ

@return ��ȡ�ɹ�����CS_TK_CONFIG_SUCCESS,ʧ�ܷ���CS_TK_CONFIG_FAILURE
*/
int OSG_iInitCfg();

int OSG_iUnInitCfg();

CSUDI_BOOL OSG_GetSize(int cfg,int *pnWidth,int *pnHeight);
//CSUDI_BOOL OSG_GetSize1(int cfg,int  nWidth,int  nHeight);

/**
@brief ���Դ�����Ӧ��С����ɫģʽ�Ļ�ͼƽ��

@param[in] eFixelFormat ������ͼƽ�����ɫģʽ�����CSUDIOSGPixelFormat_E
@param[in] eType �����Ļ�ͼƽ��Ĵ�С���ͣ����OSGUDI2Surface_E˵��
@return 
	- ���ƽ̨֧�ִ�����Ӧ����ɫģʽ�����Ҵ����ɹ�����֤ͨ���򷵻�CSUDI_TRUE��ƽ̨��֧�ָ���ɫģʽ�򷵻�CSUDI_TRUE
	- ��������CSUDI_FALSE
*/
CSUDI_BOOL OSG_CreatSurface_TestPixelFormatAndSize(CSUDIOSGPixelFormat_E ePixelFormat,OSGUDI2Surface_E eType);

/*
����������´���Surface ����ȷ��
*/
CSUDI_BOOL OSG_iCreateSurface(CSUDIOSGPixelFormat_E ePixelFormat, int nWidth, int nHeight);

/**
@brief ����Դ��Ŀ�ľ��������ڲ�ͬ��С������µ�blit����

��������CSTC_OSG_IT_Blit_Rect_0001~20���ô˺���
@param[in] eFixelFormat ������ͼƽ�����ɫģʽ�����CSUDIOSGPixelFormat_E
@param[in] ucFillSrcColor ���Դ�����������ɫֵ(ͨ��memset��ʽ���)
@param[in] eSRect Դ����������Ϣ,���OSGUDI2Rect_E˵��
@param[in] eDRect Ŀ�ľ���������Ϣ�����OSGUDI2Rect_E˵��
@param[in] bValidRect ���������Ƿ�Ϊ�Ϸ�����
@return 
	- ִ��blit�ɹ�����У��ͨ���򷵻�CSUDI_TRUE
	- ��������CSUDI_FALSE
@note ���bValidRectΪCSUDI_TRUE��������eDRect��ֵ��ȡ�������е�������ص����ɫֵ��
��У���ȡ����ɫֵ�Ƿ���Դ���ε���ɫֵһ��
*/
CSUDI_BOOL OSG_Blit_TestRect(CSUDIOSGPixelFormat_E ePixelFormat,unsigned char ucFillSrcColor,OSGUDI2Rect_E eSRect,OSGUDI2Rect_E eDRect,CSUDI_BOOL bValidRect);

/**
@brief ���Ի������Դ�֮��Ĳ�ͬ���������С����µ�Blit����

��������CSTC_OSG_IT_Blit_Rect_0001���ô˺���
@param[in] ucFillSrcColor ���Դ�����������ɫֵ(ͨ��memset��ʽ���)
@param[in] eSRect Դ����������Ϣ,���OSGUDI2Rect_E˵��
@param[in] eDRect Ŀ�ľ���������Ϣ�����OSGUDI2Rect_E˵��
@param[in] bValidRect ���������Ƿ�Ϊ�Ϸ�����
@return 
	- ִ��blit�ɹ�����У��ͨ���򷵻�CSUDI_TRUE
	- ��������CSUDI_FALSE
@note ���bValidRectΪCSUDI_TRUE��������eDRect��ֵ��ȡ�������е�������ص����ɫֵ��
��У���ȡ����ɫֵ�Ƿ���Դ���ε���ɫֵһ��
*/

CSUDI_BOOL OSG_Blit_TestRect_WithDisplay(unsigned char ucFillSrcColor,OSGUDI2Rect_E eSRect,OSGUDI2Rect_E eDRect,CSUDI_BOOL bValidRect);

/**
@brief ����colorkey���ڵ�����µ�blit����

��������CSTC_OSG_IT_Blit_Ckey_XXXX���ô˺���
@param[in] uColorKey ����Ŀ�Ļ�ͼƽ���colorkey
@param[in] uSrcColor Դ��ͼƽ��������ɫ,Ĭ��Ϊ32λģʽ
@param[in] uDstColor Ŀ�Ļ�ͼƽ��������ɫ��Ĭ��Ϊ32λģʽ
@param[in] bExist ���ΪCSUDI_TRUE����uSrcColorӦ�ô���Ŀ�Ļ�ͼƽ���У�ΪCSUDI_FALSE,��uSrcColor��Ӧ�ô���Ŀ�Ļ�ͼƽ����
@return 
	- ִ��blit�ɹ�����У��ͨ���򷵻�CSUDI_TRUE
	- ��������CSUDI_FALSE
*/
CSUDI_BOOL OSG_Blit_TestCKey(CSUDIOSGPixelFormat_E ePixelFormat, unsigned int uColorKey,unsigned int uSrcColor, unsigned int uDstColor,unsigned int uCheckColor, CSUDI_BOOL bExist);

/**
@brief ����alphaģʽ�µ�blit����

��������CSTC_OSG_IT_Blit_Alpha_0001~32���ô˺���
@param[in] eFixelFormat ������ͼƽ�����ɫģʽ�����CSUDIOSGPixelFormat_E
@param[in] uSrcAlpha Դ��ͼƽ���ȫ��alpha
@param[in] uDstAlpha Ŀ�Ļ�ͼƽ���ȫ��alpha
@param[in] uSrcColor Դ��ͼƽ��������ɫ��Ĭ��Ϊ32λģʽ
@param[in] uDstColor Ŀ�Ļ�ͼƽ��������ɫ��Ĭ��Ϊ32λģʽ
@return 
	- ִ��blit�ɹ�����У��ͨ���򷵻�CSUDI_TRUE
	- ��������CSUDI_FALSE
*/
CSUDI_BOOL OSG_Blit_TestAlpha(CSUDIOSGPixelFormat_E ePixelFormat,unsigned int uSrcAlpha,unsigned int uDstAlpha,unsigned int uSrcColor, unsigned int uDstColor);
CSUDI_BOOL OSG_Blit_TestAlphaManual(CSUDIOSGPixelFormat_E ePixelFormat,unsigned int uSrcColor, unsigned int uDstColor);

/**
@brief ����alphaģʽ�µ�blit����

��������CSTC_OSG_IT_Blit_Alpha_0001~32���ô˺���
@param[in] eFixelFormat ������ͼƽ�����ɫģʽ�����CSUDIOSGPixelFormat_E
@param[in] uSrcAlpha Դ��ͼƽ���ȫ��alpha
@param[in] uDstAlpha Ŀ�Ļ�ͼƽ���ȫ��alpha
@param[in] puSrcColor Դ��ͼƽ��������ɫ��Ĭ��Ϊ32λģʽ
@param[in] puDstColor Ŀ�Ļ�ͼƽ��������ɫ��Ĭ��Ϊ32λģʽ
@return 
	- ִ��blit�ɹ�����У��ͨ���򷵻�CSUDI_TRUE
	- ��������CSUDI_FALSE
*/
CSUDI_BOOL OSG_Blit_TestAlphaWithDataColor(
					CSUDIOSGPixelFormat_E ePixelFormat,
					unsigned int uSrcAlpha,
					unsigned int uDstAlpha,
					unsigned char * puSrcColor,
					unsigned char * puDstColor, 
					int nColorLen);

/**
@brief ����alphaģʽ�µ�blit����

��������CSTC_OSG_IT_Blit_Alpha_0001~32���ô˺���
@param[in] eFixelFormat ������ͼƽ�����ɫģʽ�����CSUDIOSGPixelFormat_E
@param[in] uSrcColor Դ��ͼƽ��������ɫ��Ĭ��Ϊ32λģʽ
@param[in] uDstColor Ŀ�Ļ�ͼƽ��������ɫ��Ĭ��Ϊ32λģʽ
@param[in] eMode blit������ģʽ�����CSUDIOSGOpMode_E˵��
@return 
	- ִ��blit�ɹ�����У��ͨ���򷵻�CSUDI_TRUE
	- ��������CSUDI_FALSE
*/
CSUDI_BOOL OSG_Blit_TestOpMode(CSUDIOSGPixelFormat_E ePixelFormat,unsigned int uSrcColor, unsigned int uDstColor,CSUDIOSGOpMode_E eMode);

/**
@brief �����ڻ���ƽ���Ͻ���fill����

@param[in] eFixelFormat ������ͼƽ�����ɫģʽ�����CSUDIOSGPixelFormat_E
@param[in] ucFillColor Դ��ͼƽ��������ɫ��Ĭ��Ϊ32λģʽ
@param[in] eRect ����������Ϣ,���OSGUDI2Rect_E˵��
@param[in] bValidRect ���������Ƿ�Ϊ�Ϸ�����
@return 
	- ִ��blit�ɹ�����У��ͨ���򷵻�CSUDI_TRUE
	- ��������CSUDI_FALSE
@note ���bValidRectΪCSUDI_TRUE��������eDRect��ֵ��ȡ�������е�������ص����ɫֵ��
��У���ȡ����ɫֵ�Ƿ���Դ���ε���ɫֵһ��
*/
CSUDI_BOOL OSG_Blit_TestFill(CSUDIOSGPixelFormat_E ePixelFormat,unsigned int ucFillColor,OSGUDI2Rect_E eRect,CSUDI_BOOL bValidRect);


/**
@brief �����ڻ���ƽ���Ͻ���fill����

@param[in] eFixelFormat ������ͼƽ�����ɫģʽ�����CSUDIOSGPixelFormat_E
@param[in] ucFillColor Դ��ͼƽ��������ɫ��Ĭ��Ϊ32λģʽ
@param[in] eRect ����������Ϣ,���OSGUDI2Rect_E˵��
@param[in] bValidRect ���������Ƿ�Ϊ�Ϸ�����
@return 
	- ִ��blit�ɹ�����У��ͨ���򷵻�CSUDI_TRUE
	- ��������CSUDI_FALSE
@note ���bValidRectΪCSUDI_TRUE��������eDRect��ֵ��ȡ�������е�������ص����ɫֵ��
��У���ȡ����ɫֵ�Ƿ���Դ���ε���ɫֵһ��
*/
CSUDI_BOOL OSG_Blit_TestFillEx(CSUDIOSGPixelFormat_E ePixelFormat,unsigned int ucFillColor,int nSurfaceWidth, int nSurfaceHeight, CSUDIOSGRect_S * psSrcRect);

/**
@brief �������Դ�ռ��Ͻ���fill����

@param[in] ucFillColor Դ��ͼƽ��������ɫ��Ĭ��Ϊ32λģʽ
@param[in] eRect ����������Ϣ,���OSGUDI2Rect_E˵��
@param[in] bValidRect ���������Ƿ�Ϊ�Ϸ�����
@return 
	- ִ��blit�ɹ�����У��ͨ���򷵻�CSUDI_TRUE
	- ��������CSUDI_FALSE
@note ���bValidRectΪCSUDI_TRUE��������eDRect��ֵ��ȡ�������е�������ص����ɫֵ��
��У���ȡ����ɫֵ�Ƿ���Դ���ε���ɫֵһ��
*/
CSUDI_BOOL OSG_iTestFillDisplay(unsigned int ucFillColor,OSGUDI2Rect_E eRect,CSUDI_BOOL bValidRect);

/**
@brief ����ͼƬӲ����

@param[in] eSupportType ���Ե�ͼƬ���ͣ����OSGUDI2SupportDecIamgeType_E˵��
@param[in] eImageType ͼƬ��Դ�ļ��ı�ʶ,���OSGUDI2ImageType_E˵��
@return 
	- ִ��blit�ɹ�����У��ͨ���򷵻�CSUDI_TRUE
	- ��������CSUDI_FALSE
*/
CSUDI_BOOL OSG_iTestDecImage(OSGUDI2SupportDecIamgeType_E eSupportType,OSGUDI2ImageType_E eImageType);

CSUDI_BOOL OSG_iTestDecImageWithScaleMode(OSGUDI2SupportDecIamgeType_E eSupportType,OSGUDI2ImageType_E eImageType,CSUDIOSGScalingMode_E eScalingMode,const CSUDIOSGRect_S * psDstRect);
CSUDI_BOOL OSG_iTestDecImageSeveralJpg();
/**
@brief ������ͬһ����ռ��Ͻ���blit����

@param[in] eFixelFormat ������ͼƽ�����ɫģʽ�����CSUDIOSGPixelFormat_E
@param[in] eSRect Դ����������Ϣ,���OSGUDI2Rect_E˵��
@param[in] eDRect Ŀ�ľ���������Ϣ�����OSGUDI2Rect_E˵��
@param[in] bValidRect ���������Ƿ�Ϊ�Ϸ�����
@return 
	- ִ��blit�ɹ�����У��ͨ���򷵻�CSUDI_TRUE
	- ��������CSUDI_FALSE
@note ���bValidRectΪCSUDI_TRUE��������eDRect��ֵ��ȡ�������е�������ص����ɫֵ��
��У���ȡ����ɫֵ�Ƿ���Դ���ε���ɫֵһ��
*/
CSUDI_BOOL OSG_iTestBlitWithSameSurface(CSUDIOSGPixelFormat_E ePixelFormat,OSGUDI2Rect_E eSRect,OSGUDI2Rect_E eDRect,CSUDI_BOOL bValidRect);

/**
@brief ������ͬһ�Դ�ռ��Ͻ���blit����

@param[in] eSRect Դ����������Ϣ,���OSGUDI2Rect_E˵��
@param[in] eDRect Ŀ�ľ���������Ϣ�����OSGUDI2Rect_E˵��
@param[in] bValidRect ���������Ƿ�Ϊ�Ϸ�����
@return 
	- ִ��blit�ɹ�����У��ͨ���򷵻�CSUDI_TRUE
	- ��������CSUDI_FALSE
@note ���bValidRectΪCSUDI_TRUE��������eDRect��ֵ��ȡ�������е�������ص����ɫֵ��
��У���ȡ����ɫֵ�Ƿ���Դ���ε���ɫֵһ��
*/
CSUDI_BOOL OSG_iTestBlitWithSameDisplay(OSGUDI2Rect_E eSRect,OSGUDI2Rect_E eDRect,CSUDI_BOOL bValidRect);

/**
@brief �����ڻ���ռ��Ͻ���blit����������

@param[in] eFixelFormat ������ͼƽ�����ɫģʽ�����CSUDIOSGPixelFormat_E
@param[in] asSRect Դ����������Ϣ����,���CSUDIOSGRect_S˵��
@param[in] asDRect Ŀ�ľ���������Ϣ���飬���CSUDIOSGRect_S˵��
@param[in] nRectLen �����������鳤��
@param[in] bWithAlpha blit�����Ƿ��alpha
@return 
	- ִ��blit�ɹ�����У��ͨ���򷵻�CSUDI_TRUE
	- ��������CSUDI_FALSE
@note �ò���ֻ���ӡ��ÿ��blit��Ҫ��ʱ��
*/
CSUDI_BOOL OSG_iTESTBlitPerformance(CSUDIOSGPixelFormat_E ePixelFormat,CSUDIOSGRect_S asSRect[],CSUDIOSGRect_S asDRect[],int nRectLen,CSUDI_BOOL bWithAlpha);

/**
@brief �������Դ�ռ��Ͻ���blit����������

@param[in] asSRect Դ����������Ϣ����,���CSUDIOSGRect_S˵��
@param[in] asDRect Ŀ�ľ���������Ϣ���飬���CSUDIOSGRect_S˵��
@param[in] nRectLen �����������鳤��
@param[in] bWithAlpha blit�����Ƿ��alpha
@return 
	- ִ��blit�ɹ�����У��ͨ���򷵻�CSUDI_TRUE
	- ��������CSUDI_FALSE
@note �ò���ֻ���ӡ��ÿ��blit��Ҫ��ʱ��
*/
CSUDI_BOOL OSG_iTESTBlitPerformanceWithDisplay(CSUDIOSGRect_S asSRect[],CSUDIOSGRect_S asDRect[],int nRectLen,CSUDI_BOOL bWithAlpha);

/**
@brief �����ڻ���ռ��Ͻ���fill����������

@param[in] eFixelFormat ������ͼƽ�����ɫģʽ�����CSUDIOSGPixelFormat_E
@param[in] asSRect ������������Ϣ����,���CSUDIOSGRect_S˵��
@param[in] nRectLen �����������鳤��
@param[in] bWithAlpha blit�����Ƿ��alpha
@return 
	- ִ��blit�ɹ�����У��ͨ���򷵻�CSUDI_TRUE
	- ��������CSUDI_FALSE
@note �ò���ֻ���ӡ��ÿ��fill��Ҫ��ʱ��
*/
CSUDI_BOOL OSG_iTESTFillPerformace(CSUDIOSGPixelFormat_E ePixelFormat,CSUDIOSGRect_S asRect[],int nRectLen);

/**
@brief �������Դ�ռ��Ͻ���fill����������

@param[in] asSRect Դ����������Ϣ����,���CSUDIOSGRect_S˵��
@param[in] asDRect Ŀ�ľ���������Ϣ���飬���CSUDIOSGRect_S˵��
@param[in] nRectLen �����������鳤��
@param[in] bWithAlpha blit�����Ƿ��alpha
@return 
	- ִ��blit�ɹ�����У��ͨ���򷵻�CSUDI_TRUE
	- ��������CSUDI_FALSE
@note �ò���ֻ���ӡ��ÿ��fill��Ҫ��ʱ��
*/
CSUDI_BOOL OSG_iTESTFillPerformaceWithDisplay(CSUDIOSGRect_S asRect[],int nRectLen);

/**
@brief ����(����ռ���Դ�֮��)ִ��blit����֮���������ö��Sync�����

@param[in] eFixelFormat ������ͼƽ�����ɫģʽ�����CSUDIOSGPixelFormat_E
@param[in] bWithAlpha blit�����Ƿ��alpha
@param[in] ִ��һ��sync֮�󣬻�Ҫ���Լ��Σ�����Ϊ1����һ��֮��ÿ��sync������5s֮��
@return 
	- У��ͨ���򷵻�CSUDI_TRUE
	- ��������CSUDI_FALSE
*/
CSUDI_BOOL OSG_iTestSync(CSUDIOSGPixelFormat_E ePixelFormat,CSUDI_BOOL bWithAlpha,int nSyncTimes);
/**
@brief ����(����ռ�֮��)ִ��blit����֮���������ö��Sync�����

@param[in] bWithAlpha blit�����Ƿ��alpha
@param[in] ִ��һ��sync֮�󣬻�Ҫ���Լ��Σ�����Ϊ1����һ��֮��ÿ��sync������5s֮��
@return 
	- У��ͨ���򷵻�CSUDI_TRUE
	- ��������CSUDI_FALSE
*/
CSUDI_BOOL OSG_iTestSyncWithDisplay(CSUDI_BOOL bWithAlpha,int nSyncTimes);

CSUDI_BOOL OSG_iTESTSurfacePitch(CSUDIOSGPixelFormat_E ePixel,int nWidth ,int nHeight,unsigned char * puDstColor, int nColorLen);
/**
@brief �ж�ƽ̨�Ƿ�֧�ָ���ɫģʽ���������ļ��ṩ

@param[in] eFixelFormat ��ͼƽ�����ɫģʽ�����CSUDIOSGPixelFormat_E
@return ���ƽ̨֧�ִ�����Ӧ����ɫģʽ���򷵻�CSUDI_TRUE��ƽ̨��֧�ָ���ɫģʽ�򷵻�CSUDI_FALSE
*/
CSUDI_BOOL OSG_IsSupportPixelFormat(CSUDIOSGPixelFormat_E ePixelFormat );

/**
@brief �ж�ƽ̨�Ƿ�֧�ִ���1280*720�Ļ�ͼƽ�棬�������ļ��ṩ

@return ���ƽ̨֧�ִ������򷵻�CSUDI_TRUE��ƽ̨��֧���򷵻�CSUDI_FALSE
*/
CSUDI_BOOL OSG_IsSupport1280_720(void);

/**
@brief �ж�ƽ̨�Ƿ�֧�ִ���1920*1080�Ļ�ͼƽ�棬�������ļ��ṩ

@return ���ƽ̨֧�ִ������򷵻�CSUDI_TRUE��ƽ̨��֧���򷵻�CSUDI_FALSE
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
/*���ļ��л�ȡ�������Ա�*/
CSUDI_BOOL OSG_iCheckColorAndAlphaWithFileColor(CSUDI_HANDLE hDstSurface,
													const unsigned char * puSrcColor,/*Դƽ����ɫֵ*/
													const unsigned char * puDstColor, /*Ŀ��ƽ����ɫֵ*/
													int 		  nColorLen,   /*��ɫ��С��Ŀǰ������ɫ�Ĵ�Сһ��*/
													unsigned int  uSrcAlpha,/*Դȫ��alphaֵ*/
													unsigned int  uDstAlpha,/*Ŀ��ȫ��alphaֵ*/
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

/*��ȡ�Դ�Ĭ�ϵ���ɫģʽ,��������ɫģʽ��Ӧ���ֽ���*/
CSUDI_BOOL OSG_iFindSupportPixelFormatAndBytes(CSUDIOSGPixelFormat_E *ePixelFormat, int * nBytes);


/**
  colorKey��Դcolorkey��Ŀ��colorkey֮�֣�
  Դcolorkey��Ҫ����Դƽ���colorkey���ԣ����Դƽ�����ɫ���д���
  Ŀ��colorkey��Ҫ����Ŀ��colorkey�����ԣ����Ŀ��ƽ�����ɫ���д���
*/
CSUDI_BOOL OSG_Blit_TestCKeyForDst(CSUDIOSGPixelFormat_E ePixelFormat, unsigned int uColorKey,unsigned int uSrcColor, unsigned int uDstColor, unsigned int uCheckColor,CSUDI_BOOL bExist);


/*�˺�����������alpha���*/
CSUDI_BOOL OSG_Blit_TestAlphaWithDataColorEx(CSUDIOSGPixelFormat_E ePixelFormat,unsigned int uSrcAlpha,unsigned int uDstAlpha, const unsigned char * puSrcColor, const unsigned char * puDstColor, int nColorLen);


/*�˺�������������surface*/
CSUDI_BOOL OSG_Blit_TestFillOnePixel(CSUDIOSGPixelFormat_E ePixelFormat,unsigned int ucFillColor);


/*�˺������������ƶ���Ч������*/
CSUDI_BOOL OSG_iTESTPortalPerformance(CSUDIOSGOpMode_E eOpMode, unsigned int	unForeground_color);

/*�˺�����Ҫ����ģ������Ƶ��ƶ�*/
CSUDI_BOOL OSG_iTestMarquee();

/*�˺�����Ҫ����������Ļ�Ƿ���ƫ��*/
CSUDI_BOOL OSG_iTestScreenOffset();

/*�˺�����Ҫ����������Ļ���Ƿ������صľ�ݲ���*/
CSUDI_BOOL OSG_iTestScreenSerrate();

/*�˺������������Ļˢ���Ƿ��ж�ʧ�����*/
CSUDI_BOOL OSG_iTestScreenlost();

/*�˺������ƽ̨�Ƿ�֧�ָ���*/
CSUDI_BOOL OSG_iTestSupportHDDisplay();

/* ѭ������ָ���ļ������ļ�*/
CSUDI_BOOL OSG_iTestDecFolderImages();




#ifdef __cplusplus
}
#endif

#endif/*_CS_OSG_TEST_H_*/ 
