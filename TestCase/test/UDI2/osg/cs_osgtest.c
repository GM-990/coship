/**
   *@version 1.0.3 2009/10/09 �����µĲ��Կ�ܽ����ع�
   *@version 1.0.2 2009/09/04 �޸����ò���ΪOSG
   *@version 1.0.1 2009/07/27 ����
**/
/*Include head files*/

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cs_testkit.h"
#include "cs_testkit_porting.h"
#include "cs_osgtest.h"
#include "udi2_error.h"

static CSUDIOSGPixelFormat_E  g_eDisplayPixelFormat = EM_UDIOSG_PIXEL_FORMAT_NUM;  //�Դ����ɫģʽ
static CSUDI_BOOL g_bPixelFormatCfg[EM_UDIOSG_PIXEL_FORMAT_NUM] = {CSUDI_FALSE};    //֧�ֵ���ɫģʽ
static CSUDI_BOOL g_bSupportDecImageType[EM_OSG_DECIMAGE_MAX]   = {CSUDI_FALSE};   //֧�ֵ�ͼƬӲ��������
static CSUDI_BOOL g_bSupportSurface_1280_720 = CSUDI_FALSE;  //֧�ִ����Ļ����С��ʽ1280_720 1920_1080
static CSUDI_BOOL g_bSupportSurface_1920_1080 = CSUDI_FALSE;
static CSUDI_BOOL g_bSupportHdSdDisplay = CSUDI_FALSE; //�Ƿ�֧�ָ���ͱ����Դ�ͬʱ����
static  CSUDI_BOOL g_bShareHdDispalySurface = CSUDI_FALSE; //?�߱����Ƿ�ͬԴ��������߱��干��һ�������Դ�
/*Hi3716H�Ǹ߱���ͬԴ�ģ�ʵ����ֻ��һ�������Դ�*/
static int g_nTestDisplaySurfaceIndex = 0;

unsigned int g_uSurfaceMaxWidth = 0;   //ƽ̨֧�ִ�������󻺴�ռ��С
unsigned int g_uSurfaceMaxHeight = 0;

static int g_nSDSurfaceWidth  = 0;    //�����Դ�Ĵ�С
static int g_nSDSurfaceHeight = 0;

static int g_nHDSurfaceWidth  = 0;   //�����Դ�Ĵ�С
static int g_nHDSurfaceHeight = 0;

//ͼƬӲ����ͼƬ��Դ����
char * g_aImageFile[EM_IMAGE_MAX+1] = {
	"UDI2OSG_720_576.mpg",
	"UDI2OSG_PES_720_576.mpg",
	"UDI2OSG_1280_720.mpg",
	"UDI2OSG_1920_1080.mpg",
	"UDI2OSG_720_576.bmp",
	"UDI2OSG_1280_720.bmp",
	"UDI2OSG_1920_1080.bmp",
	"UDI2OSG_720_576.jpg",
	"UDI2OSG_1280_720.jpg",
	"UDI2OSG_1920_1080.jpg",
	"UDI2OSG_8192_8192.jpg",	
	"UDI2OSG_720_576.png",
	"UDI2OSG_1280_720.png",
	"UDI2OSG_1920_1080.png",
	"UDI2OSG_Circle_576_576.bmp",
	"UDI2OSG_Circle_576_576.jpg",
	"UDI2OSG_Circle_576_576.png",
	"UDI2OSG_DOUBLE_SOF.jpg",
	"UDI2OSG_Y_SAMPLE_12.jpg",
	"fastjpgYUVDataTestImage.jpg",
	""
};


//��Ҫ��ȡ�������ļ� �и�����Ϣ���б�
static Cfg_Info_S s_aCfgInfoList[LIST_MEMBER_NUM] = 
{
	{0, &g_bPixelFormatCfg[0], "CS_OSGUDI2_SURPORT_RGB_565"},
	{0, &g_bPixelFormatCfg[1], "CS_OSGUDI2_SURPORT_ARGB_1555"},
	{0, &g_bPixelFormatCfg[2], "CS_OSGUDI2_SURPORT_RGBA_5551"},
	{0, &g_bPixelFormatCfg[3], "CS_OSGUDI2_SURPORT_ARGB_4444"},
	{0, &g_bPixelFormatCfg[4], "CS_OSGUDI2_SURPORT_RGBA_4444"},
	{0, &g_bPixelFormatCfg[5], "CS_OSGUDI2_SURPORT_RGB_888"},
	{0, &g_bPixelFormatCfg[6], "CS_OSGUDI2_SURPORT_ARGB_8888"},
	{0, &g_bPixelFormatCfg[7], "CS_OSGUDI2_SURPORT_PALETTE8"},
	{0, &g_bPixelFormatCfg[8], "CS_OSGUDI2_SURPORT_YCBCR888_444"},
	{0, &g_bPixelFormatCfg[9], "CS_OSGUDI2_SURPORT_YCBCR888_422"},
	{0, &g_bPixelFormatCfg[10], "CS_OSGUDI2_SURPORT_YCBCR888_420"},
	{0, &g_bPixelFormatCfg[11], "CS_OSGUDI2_SURPORT_YCBCR888_411"},
	{0, &g_bSupportDecImageType[0], "CS_UDI2OSG_DECODE_IFRAME"},
	{0, &g_bSupportDecImageType[1], "CS_UDI2OSG_DECODE_BMP"},
	{0, &g_bSupportDecImageType[2], "CS_UDI2OSG_DECODE_JPG"},
	{0, &g_bSupportDecImageType[3], "CS_UDI2OSG_DECODE_PNG"},
	{0, &g_bSupportSurface_1280_720,"CS_OSGUDI2_SURPORT_SURFACE_1280_720"},
	{0, &g_bSupportSurface_1920_1080,"CS_OSGUDI2_SURPORT_SURFACE_1920_1080"},
	{0, &g_bSupportHdSdDisplay,"CS_OSGUDI2_SURPORT_HD_SD_DISPLAY"},
	{1, (int*)&g_uSurfaceMaxWidth, "CS_OSGUDI2_SURFACE_MAX_WIDTH"},
	{1, (int*)&g_uSurfaceMaxHeight, "CS_OSGUDI2_SURFACE_MAX_HEIGHT"},
	{1, &g_nSDSurfaceWidth, "CS_UDI2OSG_SD_DISPLAY_WIDTH"},
	{1, &g_nSDSurfaceHeight, "CS_UDI2OSG_SD_DISPLAY_HEIGHT"},
	{1, &g_nHDSurfaceWidth, "CS_UDI2OSG_HD_DISPLAY_WIDTH"},
	{1, &g_nHDSurfaceHeight, "CS_UDI2OSG_HD_DISPLAY_HEIGHT"},
	{1, (int*)&g_eDisplayPixelFormat, "CS_UDI2OSG_DISPLAY_PIXEL_FORMAT"},
	{0, &g_bShareHdDispalySurface, "CS_OSGUDI2_SHARE_HD_DISPLAY_SERFACE"} 
};

//��Ҫ��ȡ

//��Ҫ��ȡ�������ļ��й����Դ�������Ϣ���б�

int OSG_iUnInitCfg()
{
	int nCfgInfoIndex = 0;
	
	for(; nCfgInfoIndex < LIST_MEMBER_NUM; nCfgInfoIndex++)
	{
		if(0 == s_aCfgInfoList[nCfgInfoIndex].nType)
		{
			*s_aCfgInfoList[nCfgInfoIndex].pSavebuf = CSUDI_FALSE;
		}
		else
		{
			*s_aCfgInfoList[nCfgInfoIndex].pSavebuf = 0;
		}
	}
	
	return CS_TK_CONFIG_SUCCESS;
}
/**
@brief ��ȡ�����ļ���Ϣ,��ʼ��֧�ֵ���ɫģʽ

@return ��ȡ�ɹ�����CS_TK_CONFIG_SUCCESS,ʧ�ܷ���CS_TK_CONFIG_FAILURE
*/
int OSG_iInitCfg()
{
	char cBuf[32] = {0};
	int nCfgInfoIndex = 0;
	int nResult = 0;

	for(; nCfgInfoIndex < LIST_MEMBER_NUM; nCfgInfoIndex++)
	{
		//�����Ϣ
		memset(cBuf, '\0', sizeof(cBuf));
		nResult =  CSTKGetConfigInfo("OSG", s_aCfgInfoList[nCfgInfoIndex].pCfgInfoKey, cBuf, sizeof(cBuf));
		if(CS_TK_CONFIG_SUCCESS != nResult)
		{
			return nResult;
		}

		//������Ϣ
		if(0 == s_aCfgInfoList[nCfgInfoIndex].nType)
		{
			if(cBuf[0] == '1')
			{
				*s_aCfgInfoList[nCfgInfoIndex].pSavebuf = CSUDI_TRUE;
			}
		}
		else
		{
			*s_aCfgInfoList[nCfgInfoIndex].pSavebuf = CSTKGetIntFromStr(cBuf,10);
		}
	}

	if(g_bShareHdDispalySurface)
	{
		g_nTestDisplaySurfaceIndex = 1; //ʹ�ø����Դ����
	}
	
	return CS_TK_CONFIG_SUCCESS;
}

/**
@brief �߱����ʶ��ȡ��Ӧ��surface�ߴ�.
@param[in] cfg�߱����ʶֵ0��1��0��ʾ���壬1��ʾ����
@param[out] ��¼surface_widthָ��
@param[out] surface_heightָ��
**/
CSUDI_BOOL OSG_GetSize(int cfg,int  *pnWidth,int *pnHeight)
{
	int    nResult = -1;
	char cBuf[32] = {0};

	if(cfg == 0)
	{ 
		nResult = CSTKGetConfigInfo("OSG","CS_UDI2OSG_SD_DISPLAY_WIDTH",cBuf,sizeof(cBuf));
		if(CS_TK_CONFIG_SUCCESS != nResult)
		{
			return nResult;
		}
		*pnWidth = CSTKGetIntFromStr(cBuf,10);
		nResult = CSTKGetConfigInfo("OSG","CS_UDI2OSG_SD_DISPLAY_HEIGHT",cBuf,sizeof(cBuf));
		if(CS_TK_CONFIG_SUCCESS != nResult)
		{
			return nResult;
		}
		*pnHeight = CSTKGetIntFromStr(cBuf,10);//nSHei;
		if( *pnWidth <= 0||*pnHeight <= 0) 
		return CSUDI_FALSE;
	}
	if(cfg ==1)
	{
		nResult = CSTKGetConfigInfo("OSG","CS_UDI2OSG_HD_DISPLAY_WIDTH",cBuf,sizeof(cBuf));
		if(CS_TK_CONFIG_SUCCESS != nResult)
		{
			return nResult;
		}
		*pnWidth = CSTKGetIntFromStr(cBuf,10);
		nResult = CSTKGetConfigInfo("OSG","CS_UDI2OSG_HD_DISPLAY_HEIGHT",cBuf,sizeof(cBuf));
		if(CS_TK_CONFIG_SUCCESS != nResult)
		{
			return nResult;
		}
		*pnHeight = CSTKGetIntFromStr(cBuf,10);//nSHei;
		
		if( *pnWidth <= 0||*pnHeight <= 0) 
		return CSUDI_FALSE;
	}

	return CSUDI_TRUE;
}

typedef struct _Width_Height_S
{
	int 		nWidth;//��Ҫ����surface ���
	int		nHeight;//��Ҫ����surface �߶�
}Width_Height_S;

/**
@brief ���Դ�����Ӧ��С����ɫģʽ�Ļ�ͼƽ��

��������CSTC_OSG_IT_CreateSurface_0001 ~ CSTC_OSG_IT_CreateSurface_0035���ô˺���
@param[in] eFixelFormat ������ͼƽ�����ɫģʽ�����CSUDIOSGPixelFormat_E
@param[in] eType �����Ļ�ͼƽ��Ĵ�С���ͣ����OSGUDI2Surface_E˵��
@return 
	- ���ƽ̨֧�ִ�����Ӧ����ɫģʽ�����Ҵ����ɹ�����֤ͨ���򷵻�CSUDI_TRUE��ƽ̨��֧�ָ���ɫģʽ�򷵻�CSUDI_TRUE
	- ��������CSUDI_FALSE
*/
CSUDI_BOOL OSG_CreatSurface_TestPixelFormatAndSize(CSUDIOSGPixelFormat_E ePixelFormat,OSGUDI2Surface_E eType)
{
	Width_Height_S aTestData[4];
	int	i = 0;

	//���в����жϣ������쳣��
	//����eFixelFormat�����Ϊ�����ͻ�����g_bPixelFormatCfg[eFixelFormat] ��ȡ����
	if((ePixelFormat < 0) ||(ePixelFormat >= EM_UDIOSG_PIXEL_FORMAT_NUM) || (eType < 0) ||(eType > EM_SURFACE_720_576))
	{
		CSTCPrint("[%s, %d] param error, ePixelFormat=%d, eType=%d\n", __FUNCTION__, __LINE__, ePixelFormat, eType);
		return CSUDI_FALSE;
	}

	//���в���
	if(OSG_IsSupportPixelFormat(ePixelFormat))
	{	
		switch(eType)
		{
			case EM_SURFACE_720_576:
			{
				//���ֱ߽�ֵ֮�����ϣ��������б߽�ֵ
				aTestData[0].nWidth = 720;
				aTestData[0].nHeight = 576;
				aTestData[1].nWidth = 1;
				aTestData[1].nHeight = 1;
				aTestData[2].nWidth = 1;
				aTestData[2].nHeight = 576;
				aTestData[3].nWidth = 720;
				aTestData[3].nHeight = 1;
			
				for(i = 0; i < sizeof(aTestData)/sizeof(aTestData[0]); i++)
				{
					if(!OSG_iCreateSurface(ePixelFormat, aTestData[i].nWidth, aTestData[i].nHeight))
					{
						CSTCPrint("[%s, %d] call OSG_iCreateSurface failed, pixelformat=%d, whidth=%d, hight=%d\n",\
							__FUNCTION__, __LINE__, ePixelFormat, aTestData[i].nWidth, aTestData[i].nHeight);
						return CSUDI_FALSE;
					}
				}
				break;
			}
			case EM_SURFACE_1280_720:
			{
				aTestData[0].nWidth = 1280;
				aTestData[0].nHeight = 720;
				aTestData[1].nWidth = 1;
				aTestData[1].nHeight = 1;
				aTestData[2].nWidth = 1;
				aTestData[2].nHeight = 720;
				aTestData[3].nWidth = 1280;
				aTestData[3].nHeight = 1;

				for(i = 0; i < sizeof(aTestData)/sizeof(aTestData[0]); i++)
				{
					if(!OSG_iCreateSurface(ePixelFormat, aTestData[i].nWidth, aTestData[i].nHeight))
					{
						CSTCPrint("[%s, %d] call OSG_iCreateSurface failed, pixelformat=%d, whidth=%d, hight=%d\n",\
							__FUNCTION__, __LINE__, aTestData[i].nWidth, aTestData[i].nHeight);
						return CSUDI_FALSE;
					}
				}
				break;
			}
			case EM_SURFACE_1920_1080:
			{
				aTestData[0].nWidth = 1920;
				aTestData[0].nHeight = 1080;
				aTestData[1].nWidth = 1;
				aTestData[1].nHeight = 1;
				aTestData[2].nWidth = 1;
				aTestData[2].nHeight = 1080;
				aTestData[3].nWidth = 1920;
				aTestData[3].nHeight = 1;

				for(i = 0; i < sizeof(aTestData)/sizeof(aTestData[0]); i++)
				{
					if(!OSG_iCreateSurface(ePixelFormat, aTestData[i].nWidth, aTestData[i].nHeight))
					{
						CSTCPrint("[%s, %d] call OSG_iCreateSurface failed, pixelformat=%d, whidth=%d, hight=%d\n",\
							__FUNCTION__, __LINE__, aTestData[i].nWidth, aTestData[i].nHeight);
						return CSUDI_FALSE;
					}
				}
				break;
			}
			default:
				break;
		}
	}

	return CSUDI_TRUE;	
}

/*
����������´���Surface ����ȷ��
*/
CSUDI_BOOL OSG_iCreateSurface(CSUDIOSGPixelFormat_E ePixelFormat, int nWidth, int nHeight)
{
	CSUDI_HANDLE	hSurface =  CSUDI_NULL;
	CSUDI_BOOL		bResult = CSUDI_FALSE;
	
	//����Surfase
	if(CSUDI_SUCCESS != CSUDIOSGCreateSurface(ePixelFormat, nWidth, nHeight, &hSurface))
	{
		CSTCPrint("[ePixelFormat:%d , nWidth:%d, nHeight:%d]\n",ePixelFormat, nWidth, nHeight);
		TEST_OSG_GOTO(CSUDI_FALSE,OSG_EXIT_LABEL,"����Surface ʧ��!!!");
	}
	if(CSUDI_NULL == hSurface)
	{
		CSTCPrint("[ePixelFormat:%d , nWidth:%d, nHeight:%d]\n",ePixelFormat, nWidth, nHeight);
		TEST_OSG_GOTO(CSUDI_FALSE,OSG_EXIT_LABEL,"�����ɹ�handle ȴΪ��!!!");
	}

	//���Surface
	if(!OSG_iCheckSurfaceInfo(hSurface,ePixelFormat,nWidth,nHeight))
	{
		CSTCPrint("[ePixelFormat:%d , nWidth:%d, nHeight:%d]\n",ePixelFormat, nWidth, nHeight);
		TEST_OSG_GOTO(CSUDI_FALSE,OSG_EXIT_LABEL,"��ȡ�Ļ�ͼƽ�����Ϣ������ʱ�������Ϣ��һ��!!!");		
	}

	//����Surface�����������õ���ȷ��
	if(!OSG_iSetSurfaceInfo(hSurface))
	{
		CSTCPrint("[ePixelFormat:%d , nWidth:%d, nHeight:%d]\n",ePixelFormat, nWidth, nHeight);
		TEST_OSG_GOTO(CSUDI_FALSE,OSG_EXIT_LABEL,"����SetSurfaceInfo ʧ��");		
	}
	
	//����Surface���ڴ�һ�����DestorySurface�Ĳ��ԣ����Ա�֤���Ե�ȫ����
	TEST_OSG_GOTO((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface)),OSG_EXIT_LABEL,"����Surface ʧ��");
	hSurface = CSUDI_NULL;
	
	//���ˣ����Գɹ�
	bResult = CSUDI_TRUE;
		
OSG_EXIT_LABEL:
	if(hSurface != CSUDI_NULL)
	{
		TEST_OSG_GOTO((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface)),OSG_EXIT_LABEL,"����Surface ʧ��");
		hSurface = CSUDI_NULL;
	}

	return bResult;
}

/**
@brief ����Դ��Ŀ�ľ��������ڲ�ͬ��С������µ�blit����

��������CSTC_OSG_IT_Blit_Rect_0001���ô˺���
@param[in] eFixelFormat ������ͼƽ�����ɫģʽ�����CSUDIOSGPixelFormat_E
@param[in] ucFillSrcColor ���Դ�����������ɫֵ(ͨ��memset��ʽ���)
@param[in] psSRect Դ����������Ϣ
@param[in] psDRect Ŀ�ľ���������Ϣ
@param[in] bValidRect ���������Ƿ�Ϊ�Ϸ�����
@return 
	- ִ��blit�ɹ�����У��ͨ���򷵻�CSUDI_TRUE
	- ��������CSUDI_FALSE
@note ���bValidRectΪCSUDI_TRUE��������psDRect��ֵ��ȡ�������е�������ص����ɫֵ��
��У���ȡ����ɫֵ�Ƿ���Դ���ε���ɫֵһ��
*/
CSUDI_BOOL OSG_Blit_TestRect(CSUDIOSGPixelFormat_E ePixelFormat,unsigned char ucFillSrcColor,OSGUDI2Rect_E eSRect,OSGUDI2Rect_E eDRect,CSUDI_BOOL bValidRect)
{
	if(OSG_IsSupportPixelFormat(ePixelFormat))
	{
		CSUDI_HANDLE hSurface_S = CSUDI_NULL;
		CSUDI_HANDLE hSurface_D = CSUDI_NULL;
		CSUDI_HANDLE hDisplay = CSUDI_NULL;
		CSUDI_Error_Code nRet = CSUDI_FAILURE;
		CSUDI_BOOL bRet = CSUDI_FALSE;
		CSUDIOSGSurfaceInfo_S sSurfaceInfo;
		CSUDIOSGRect_S sSRect,sDRect;
		CSUDIOSGRect_S * psSRect = &sSRect;
		CSUDIOSGRect_S * psDRect = &sDRect;
		unsigned int uExpectedColor = 0;
		
		switch(ePixelFormat) //�����ɫΪ0x88����������ɫֵ����ɫģʽ�й�
		{
			case EM_UDIOSG_PIXEL_FORMAT_ARGB_8888:
				{
					uExpectedColor = OSG_FILL_COLOR_GREY_32BIT;
				}
				break;
			case EM_UDIOSG_PIXEL_FORMAT_ARGB_1555:
				{
					uExpectedColor = OSG_FILL_COLOR_GREY_16BIT;
				}
				break;
			case EM_UDIOSG_PIXEL_FORMAT_RGBA_5551:
			case EM_UDIOSG_PIXEL_FORMAT_ARGB_4444:
			case EM_UDIOSG_PIXEL_FORMAT_RGB_565:
			default:
				{
					CSTCPrint("[%s, %d] Not support, pixelformat = %d!!", __FUNCTION__, __LINE__, ePixelFormat);
				}
				return CSUDI_FALSE;
		}
		nRet = CSUDIOSGCreateSurface(ePixelFormat,SURFACE_WIDTH_720,SURFACE_HEIGHT_576,&hSurface_S);
		CSTK_ASSERT_TRUE_FATAL(
					(nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),
					"Step 1 Create P1_1 Surface Failure !"
				);
			
		nRet = CSUDIOSGCreateSurface(ePixelFormat,SURFACE_WIDTH_720,SURFACE_HEIGHT_576,&hSurface_D);
		CSTK_ASSERT_TRUE_FATAL(
					(nRet == CSUDI_SUCCESS && hSurface_D!= CSUDI_NULL),
					"Step 1 Create P2_1 Surface Failure !"
				);
				
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_S, &sSurfaceInfo)),
					"Get P1_1 Surface info failure !"
				);	
		CSTK_ASSERT_TRUE_FATAL(
					(sSurfaceInfo.m_pvSurfaceMem != CSUDI_NULL),
					"Get P1_1 Surface info failure !"
				);
		
		memset(sSurfaceInfo.m_pvSurfaceMem, ucFillSrcColor, sSurfaceInfo.m_nMemSize);
		//*****************************************************************//
		if(eSRect == EM_RECT_1)
		{
			psSRect = CSUDI_NULL;
		}
		OSG_iFillRectExt(psSRect,0,0,SURFACE_WIDTH_720,SURFACE_HEIGHT_576,eSRect);
		//*****************************************************************//
		//*****************************************************************//
		if(eDRect == EM_RECT_1)
		{
			psDRect = CSUDI_NULL;
		}
		OSG_iFillRectExt(psDRect,0,0,SURFACE_WIDTH_720,SURFACE_HEIGHT_576,eDRect);
		//*****************************************************************//
		nRet = CSUDIOSGBlit(hSurface_D, psDRect, hSurface_S, psSRect, EM_UDIOSG_OPMODE_COPY);
		if(bValidRect)
		{
			CSTK_ASSERT_TRUE_FATAL(
						((nRet == CSUDI_SUCCESS)||(nRet == CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED)),/*�Ŵ�blitʱ���ܲ�֧��*/
						"Step 3 Blit should be success !"
					);
			CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_SUCCESS == CSUDIOSGSync()),
						"Step 4 Sync Failure !"
					);

			bRet =  OSG_iCheckExistColor(hSurface_D,uExpectedColor,psDRect,ePixelFormat);
			CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_TRUE == bRet),
						"Check Surface color Failure !"
					);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(g_nTestDisplaySurfaceIndex,&hDisplay), "Get DisplaySurface fail.");
			nRet = CSUDIOSGBlit(hDisplay, CSUDI_NULL, hSurface_D, CSUDI_NULL, EM_UDIOSG_OPMODE_COPY); //blit���Դ�
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==nRet, "Blit DisplaySurface fail.");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGSync(), "CSUDIOSGSync fail.");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(
						(nRet == CSUDIOSG_ERROR_BAD_PARAMETER),
						"Blit should be failure !"
					);
		}
		
		CSTK_FATAL_POINT
		if(hSurface_S != CSUDI_NULL)
		{
			if(CSUDIOSGDestroySurface(hSurface_S) != CSUDI_SUCCESS)
				 CSTCPrint("Destroy P1_1 surface failure !\n");
			hSurface_S = CSUDI_NULL;
		}
		
		if(hSurface_D != CSUDI_NULL)
		{
			if(CSUDIOSGDestroySurface(hSurface_D) != CSUDI_SUCCESS)
				 CSTCPrint("Destroy P2_1 surface failure !");
			hSurface_D = CSUDI_NULL;
		}
 		return CSUDI_TRUE;
	}
	else 
	{
		CSTCPrint("[%s, %d] not Supported, PixelFormat=%d !!!\n", __FUNCTION__, __LINE__, ePixelFormat);
	  	return CSUDI_TRUE;
	}	
}

CSUDI_BOOL OSG_Blit_TestRect_WithDisplay(unsigned char ucFillSrcColor,OSGUDI2Rect_E eSRect,OSGUDI2Rect_E eDRect,CSUDI_BOOL bValidRect)
{
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hSurface_D = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;
	CSUDIOSGSurfaceInfo_S sDisplayInfo;
	CSUDIOSGRect_S sSRect,sDRect;
	CSUDIOSGRect_S * psSRect = &sSRect;
	CSUDIOSGRect_S * psDRect = &sDRect;
	unsigned int uExpectedColor = 0;
	
	nRet = CSUDIOSGGetDisplaySurface(g_nTestDisplaySurfaceIndex,&hSurface_D);
	CSTK_ASSERT_TRUE_FATAL(
				(nRet == CSUDI_SUCCESS && hSurface_D != CSUDI_NULL),
				"Step 1 get display surface Failure !"
			);

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_D, &sDisplayInfo)),
				"step 2 Get display info failure !"
			);	
	switch(sDisplayInfo.m_ePixelFormat)
	{
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_8888:
			{
				uExpectedColor = OSG_FILL_COLOR_GREY_32BIT;
			}
			break;
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_1555:
			{
				uExpectedColor = OSG_FILL_COLOR_GREY_16BIT;
			}
			break;
		case EM_UDIOSG_PIXEL_FORMAT_RGBA_5551:
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_4444:
		case EM_UDIOSG_PIXEL_FORMAT_RGB_565:
		default:
			{
				CSTCPrint("[%s, %d] Not support,  pixelformat=%d !!", __FUNCTION__, __LINE__, sDisplayInfo.m_ePixelFormat);
			}
			return CSUDI_FALSE;
	}
	nRet = CSUDIOSGCreateSurface(sDisplayInfo.m_ePixelFormat,sDisplayInfo.m_nWidth,sDisplayInfo.m_nHeight,&hSurface_S);
	CSTK_ASSERT_TRUE_FATAL(
				(nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),
				"Step 3 Create P1_1 Surface Failure !"
			);
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_S, &sSurfaceInfo)),
				"Get P1_1 Surface info failure !"
			);	
	CSTK_ASSERT_TRUE_FATAL(
				(sSurfaceInfo.m_pvSurfaceMem != CSUDI_NULL),
				"Get P1_1 Surface info failure !"
			);
	
	memset(sSurfaceInfo.m_pvSurfaceMem, ucFillSrcColor, sSurfaceInfo.m_nMemSize);

	//*****************************************************************//
	if(eSRect == EM_RECT_1)
	{
		psSRect = CSUDI_NULL;
	}
	OSG_iFillRectExt(psSRect,0,0,sSurfaceInfo.m_nWidth,sSurfaceInfo.m_nHeight,eSRect);
	//*****************************************************************//
	//*****************************************************************//
	if(eDRect == EM_RECT_1)
	{
		psDRect = CSUDI_NULL;
	}
	OSG_iFillRectExt(psDRect,0,0,sSurfaceInfo.m_nWidth,sSurfaceInfo.m_nHeight,eDRect);
	//*****************************************************************//
	nRet = CSUDIOSGBlit(hSurface_D, psDRect, hSurface_S, psSRect, EM_UDIOSG_OPMODE_COPY);
	if(bValidRect)
	{
		CSTK_ASSERT_TRUE_FATAL(
					((nRet == CSUDI_SUCCESS)||(nRet == CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED)),/*�Ŵ�blitʱ���ܲ�֧��*/
					"Step 3 Blit should be success !"
				);
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGSync()),
					"Step 4 Sync Failure !"
				);
		//ƽ�����ɫģʽӦ����sDisplayInfo.m_ePixelFormat����
		bRet =  OSG_iCheckExistColor(hSurface_D,uExpectedColor,psDRect,sDisplayInfo.m_ePixelFormat);
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == bRet),
					"Check Surface color Failure !"
				);
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(
					(nRet == CSUDIOSG_ERROR_BAD_PARAMETER),
					"Blit should be failure !"
				);
	}
	
	CSTK_FATAL_POINT
	if(hSurface_S != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface_S)),
			"Destroy P1_1 surface failure !"
			);
		hSurface_S = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

/**
  colorKey��Դcolorkey��Ŀ��colorkey֮�֣�
  Դcolorkey��Ҫ����Դƽ���colorkey���ԣ����Դƽ�����ɫ���д���
  Ŀ��colorkey��Ҫ����Ŀ��colorkey�����ԣ����Ŀ��ƽ�����ɫ���д���
*/
CSUDI_BOOL OSG_Blit_TestCKey(CSUDIOSGPixelFormat_E ePixelFormat, unsigned int uColorKey, unsigned int unSrcColor, unsigned int unDstColor, unsigned int unCheckColor, CSUDI_BOOL bExist)
{
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hSurface_D = CSUDI_NULL;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;	
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDI_HANDLE hDisplay = CSUDI_NULL;
	
	unsigned int unSrcColorTmp = unSrcColor;
	unsigned int unDstColorTmp = unDstColor;
	unsigned int unChkColorTmp = unCheckColor;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);

	if (ePixelFormat != EM_UDIOSG_PIXEL_FORMAT_ARGB_8888)
	{
		unSrcColorTmp = OSG_iGetColorWithPixelMode(unSrcColor, ePixelFormat);
		unDstColorTmp = OSG_iGetColorWithPixelMode(unDstColor, ePixelFormat);
		unChkColorTmp = OSG_iGetColorWithPixelMode(unCheckColor, ePixelFormat);
	}

	CSTCPrint("SrcColor=0x%08x, DstColor=0x%08x, ChkColor=0x%08x\n", unSrcColorTmp, unDstColorTmp, unChkColorTmp);
		
	nRet = CSUDIOSGCreateSurface(ePixelFormat,
									SURFACE_WIDTH_720,
									SURFACE_HEIGHT_576,
									&hSurface_S
									);

	CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),"Step 1 P1_1 Create Surface Failure !");
	
	nRet = CSUDIOSGCreateSurface(ePixelFormat,
									SURFACE_WIDTH_720,
									SURFACE_HEIGHT_576,
									&hSurface_D
									);
	CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hSurface_D != CSUDI_NULL),"Step 1 P2_1 Create Surface Failure !");
	
	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_S, &sSurfaceInfo)),//����Ӧ����hSurface_S��
				"Step 2 get surface info failure"										//��Ȼ�ڴ��ַ֮�ڵĲ�����Ϊ
			);																	//set�ı䣬������ò�Ҫ��������
	
	sSurfaceInfo.m_uColorkey = uColorKey;
	sSurfaceInfo.m_nColorKeyFlag = 1;
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGSetSurfaceInfo(hSurface_S, &sSurfaceInfo)),//2009-10-30�޸ĳ�ԴcolorkeyģʽhSurface_S
				"Step 2 set surface info failure"
			);	

	//��ʼ��Դ��ͼƽ����ɫ
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFillSurface(hSurface_S, unSrcColorTmp, ePixelFormat)),
				"Step 4 Fill src surface failure"
			);	
	
	//��ʼ��Ŀ�Ļ�ͼƽ����ɫ 
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFillSurface(hSurface_D, unDstColorTmp, ePixelFormat)),
				"Step 5 Fill dist surface failure"
			);
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGBlit(hSurface_D, CSUDI_NULL, hSurface_S, CSUDI_NULL, EM_UDIOSG_OPMODE_COPY)),
				"Step 6 blit  failure"
			);	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGSync()),
				"Step 7 Sync  failure"
			);
	if(bExist)
	{
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iCheckExistColor(hSurface_D, unChkColorTmp, CSUDI_NULL, ePixelFormat)),
					"Step 8 check color failure"
				);	
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iCheckNotExistColor(hSurface_D, unChkColorTmp, CSUDI_NULL, ePixelFormat)),
					"Step 8 check color failure"
				);	
	}
	CSUDIOSGGetDisplaySurface(g_nTestDisplaySurfaceIndex, &hDisplay);
	CSUDIOSGBlit(hDisplay, CSUDI_NULL, hSurface_D, CSUDI_NULL, EM_UDIOSG_OPMODE_COPY); //blit���Դ�
	CSUDIOSGSync();

	CSTK_FATAL_POINT
		
	if(CSUDI_NULL != hSurface_S)
	{
		if(CSUDIOSGDestroySurface(hSurface_S) != CSUDI_SUCCESS)
			 CSTCPrint("Step 9 destroy P1_1 surface failure\n");
		hSurface_S = CSUDI_NULL;
	}
	if(CSUDI_NULL != hSurface_D)
	{
		if(CSUDIOSGDestroySurface(hSurface_D) != CSUDI_SUCCESS)
			 CSTCPrint("Step 9 destroy P2_1 surface failure");
	}
	hSurface_D = CSUDI_NULL;
	
	return CSUDI_TRUE;
}

/**
  colorKey��Դcolorkey��Ŀ��colorkey֮�֣�
  Դcolorkey��Ҫ����Դƽ���colorkey���ԣ����Դƽ�����ɫ���д���
  Ŀ��colorkey��Ҫ����Ŀ��colorkey�����ԣ����Ŀ��ƽ�����ɫ���д���
*/
CSUDI_BOOL OSG_Blit_TestCKeyForDst(CSUDIOSGPixelFormat_E ePixelFormat, unsigned int uColorKey,unsigned int unSrcColor, unsigned int unDstColor, unsigned int unCheckColor, CSUDI_BOOL bExist)
{
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hSurface_D = CSUDI_NULL;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;	
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDI_HANDLE hDisplay = CSUDI_NULL;
	
	unsigned int unSrcColorTmp = unSrcColor;
	unsigned int unDstColorTmp = unDstColor;
	unsigned int unChkColorTmp = unCheckColor;
	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);

	if (ePixelFormat != EM_UDIOSG_PIXEL_FORMAT_ARGB_8888)
	{
		unSrcColorTmp = OSG_iGetColorWithPixelMode(unSrcColor, ePixelFormat);
		unDstColorTmp = OSG_iGetColorWithPixelMode(unDstColor, ePixelFormat);
		unChkColorTmp = OSG_iGetColorWithPixelMode(unCheckColor, ePixelFormat);
	}
			
	nRet = CSUDIOSGCreateSurface(ePixelFormat,
									SURFACE_WIDTH_720,
									SURFACE_HEIGHT_576,
									&hSurface_S
									);

	CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),"Step 1 P1_1 Create Surface Failure !");
	
	nRet = CSUDIOSGCreateSurface(ePixelFormat,
									SURFACE_WIDTH_720,
									SURFACE_HEIGHT_576,
									&hSurface_D
									);
	CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hSurface_D != CSUDI_NULL),"Step 1 P2_1 Create Surface Failure !");
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_D, &sSurfaceInfo)),
				"Step 2 get surface info failure");																	
	
	sSurfaceInfo.m_uColorkey = uColorKey;
	sSurfaceInfo.m_nColorKeyFlag = 1;
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSetSurfaceInfo(hSurface_D, &sSurfaceInfo)),
				"Step 3 set surface info failure");	

	//��ʼ��Դ��ͼƽ����ɫ
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == OSG_iFillSurface(hSurface_S,unSrcColorTmp,ePixelFormat)),"Step 4 Fill src surface failure");	
	
	//��ʼ��Ŀ�Ļ�ͼƽ����ɫ 
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == OSG_iFillSurface(hSurface_D,unDstColorTmp,ePixelFormat)),"Step 5 Fill dist surface failure");

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGBlit(hSurface_D, CSUDI_NULL, hSurface_S, CSUDI_NULL, EM_UDIOSG_OPMODE_COPY)),"Step 6 blit  failure"	);	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Step 7 Sync  failure");

	if(bExist)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == OSG_iCheckExistColor(hSurface_D,unChkColorTmp,CSUDI_NULL,ePixelFormat)),"Step 8 check color failure");	
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == OSG_iCheckNotExistColor(hSurface_D,unChkColorTmp,CSUDI_NULL,ePixelFormat)),"Step 8 check color failure");	
	}

	CSUDIOSGGetDisplaySurface(g_nTestDisplaySurfaceIndex,&hDisplay);
	CSUDIOSGBlit(hDisplay, CSUDI_NULL, hSurface_D, CSUDI_NULL, EM_UDIOSG_OPMODE_COPY); //blit���Դ�
	CSUDIOSGSync();

	CSTK_FATAL_POINT
		
	if(CSUDI_NULL != hSurface_S)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface_S)),"Step 9 destroy P1_1 surface failure");
		hSurface_S = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hSurface_D)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface_D)),"Step 9 destroy P2_1 surface failure");
		hSurface_D = CSUDI_NULL;
	}		

	hSurface_D = CSUDI_NULL;
	
	return CSUDI_TRUE;
}

CSUDI_BOOL OSG_Blit_TestAlphaManual(CSUDIOSGPixelFormat_E ePixelFormat,unsigned int uSrcColor, unsigned int uDstColor)
{
	if(OSG_IsSupportPixelFormat(ePixelFormat))
	{
		CSUDI_HANDLE hSurface_S = CSUDI_NULL;
		CSUDI_HANDLE hSurface_D = CSUDI_NULL;
		CSUDIOSGSurfaceInfo_S sSurfaceInfo;	
		CSUDI_Error_Code nRet = CSUDI_FAILURE;
		CSUDI_HANDLE hDisplay = CSUDI_NULL;
		unsigned char uAlpha = 0xff;
		CSUDIOSGRect_S sSRect;
		CSUDIOSGRect_S * psSRect = &sSRect;
		nRet = CSUDIOSGCreateSurface(ePixelFormat,
										SURFACE_WIDTH_720,
										SURFACE_HEIGHT_576,
										&hSurface_S
										);

		CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),"Step 1 P1_1 Create Surface Failure !");
		
		nRet = CSUDIOSGCreateSurface(ePixelFormat,
										SURFACE_WIDTH_720,
										SURFACE_HEIGHT_576,
										&hSurface_D
										);
		CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hSurface_D != CSUDI_NULL),"Step 1 P2_1 Create Surface Failure !");
		CSUDIOSGGetDisplaySurface(g_nTestDisplaySurfaceIndex,&hDisplay);

		//uAlpha��5�Ĳ����𽥼�С��������break
		for(uAlpha = 0xff; ; uAlpha -= 5)
		{
			CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_S, &sSurfaceInfo)),
						"Step 2 get surface info failure"
					);	
			
			sSurfaceInfo.m_uAlpha = uAlpha;
			sSurfaceInfo.m_nAlphaFlag = 1;
			
			CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_SUCCESS == CSUDIOSGSetSurfaceInfo(hSurface_S, &sSurfaceInfo)),
						"Step 3 set surface info failure"
					);

			//��ʼ��Դ��ͼƽ����ɫ
			CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_TRUE == OSG_iFillSurface(hSurface_S, uSrcColor, ePixelFormat)),
						"Step 5 Fill src surface failure"
					);	
			
			//��ʼ��Ŀ�Ļ�ͼƽ����ɫ 
			CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_TRUE == OSG_iFillSurface(hSurface_D, uDstColor, ePixelFormat)), 
						"Step 6 Fill dist surface failure"
					);
			
			OSG_iFillRectExt(psSRect,0,0,SURFACE_WIDTH_720,SURFACE_HEIGHT_576,EM_RECT_3); ///����ƽ��
			CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_SUCCESS == CSUDIOSGBlit(hSurface_D, psSRect, hSurface_S, psSRect, EM_UDIOSG_OPMODE_ALPHA)),
						"Step 7 blit  failure"
					);	
			CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_SUCCESS == CSUDIOSGSync()),
						"Step 8 Sync  failure"
					);	

			nRet = CSUDIOSGBlit(hDisplay, CSUDI_NULL, hSurface_D, CSUDI_NULL, EM_UDIOSG_OPMODE_COPY); //blit���Դ�
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == nRet, "blit���Դ�ʧ��");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGSync(), "CSUDIOSGSync ʧ��");

			if (uAlpha == 0)
			{
				break;
			}
		}

		CSTK_FATAL_POINT
		if(CSUDI_NULL != hSurface_S)
		{
			if(CSUDIOSGDestroySurface(hSurface_S) != CSUDI_SUCCESS)
				 CSTCPrint("Step 9 destroy P1_1 surface failure\n");
			hSurface_S = CSUDI_NULL;
		}
		if(CSUDI_NULL != hSurface_D)
		{
			if(CSUDIOSGDestroySurface(hSurface_D) != CSUDI_SUCCESS)
				 CSTCPrint("Step 9 destroy P2_1 surface failure");
		}
		hSurface_D = CSUDI_NULL;
		return CSUDI_TRUE;
	}
	else 
	{
		CSTCPrint("[%s, %d] Not Supported, PixelFormat=%d !!!\n", __FUNCTION__, __LINE__, ePixelFormat);
	  	return CSUDI_TRUE;	
	}
}
CSUDI_BOOL OSG_Blit_TestAlpha(CSUDIOSGPixelFormat_E ePixelFormat,unsigned int uSrcAlpha,unsigned int uDstAlpha,unsigned int uSrcColor, unsigned int uDstColor)
{
	if(OSG_IsSupportPixelFormat(ePixelFormat))
	{
		CSUDI_HANDLE hSurface_S = CSUDI_NULL;
		CSUDI_HANDLE hSurface_D = CSUDI_NULL;
		CSUDIOSGSurfaceInfo_S sSurfaceInfo;	
		CSUDI_Error_Code nRet = CSUDI_FAILURE;
		
		nRet = CSUDIOSGCreateSurface(ePixelFormat,
										SURFACE_WIDTH_720,
										SURFACE_HEIGHT_576,
										&hSurface_S
										);

		CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),"Step 1 P1_1 Create Surface Failure !");
		
		nRet = CSUDIOSGCreateSurface(ePixelFormat,
										SURFACE_WIDTH_720,
										SURFACE_HEIGHT_576,
										&hSurface_D
										);
		CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hSurface_D != CSUDI_NULL),"Step 1 P2_1 Create Surface Failure !");
		
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_S, &sSurfaceInfo)),
					"Step 2 get surface info failure"
				);			
		sSurfaceInfo.m_uAlpha = uSrcAlpha;
		sSurfaceInfo.m_nAlphaFlag = 1;
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGSetSurfaceInfo(hSurface_S, &sSurfaceInfo)),
					"Step 3 set surface info failure"
				);
		
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_D, &sSurfaceInfo)),
					"Step 2 get surface info failure"
				);			
		sSurfaceInfo.m_uAlpha = uDstAlpha;
		sSurfaceInfo.m_nAlphaFlag = 1;
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGSetSurfaceInfo(hSurface_D, &sSurfaceInfo)),
					"Step 4 set surface info failure"
				);	

		//��ʼ��Դ��ͼƽ����ɫ
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iFillSurface(hSurface_S,uSrcColor,ePixelFormat)),
					"Step 5 Fill src surface failure"
				);	
		
		//��ʼ��Ŀ�Ļ�ͼƽ����ɫ 
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iFillSurface(hSurface_D,uDstColor,ePixelFormat)),
					"Step 6 Fill dist surface failure"
				);
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGBlit(hSurface_D, CSUDI_NULL, hSurface_S, CSUDI_NULL, EM_UDIOSG_OPMODE_ALPHA)),
					"Step 7 blit  failure"
				);	
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGSync()),
					"Step 8 Sync  failure"
				);	
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iCheckColorAndAlpha(hSurface_D,
									uSrcColor,
									uDstColor,
									uSrcAlpha,
									uDstAlpha,
									ePixelFormat)),
					"Step 10 check alpha and color failure"
				);
		
		CSTK_FATAL_POINT
		if(CSUDI_NULL != hSurface_S)
		{
			if(CSUDIOSGDestroySurface(hSurface_S) != CSUDI_SUCCESS)
				 CSTCPrint("Step 9 destroy P1_1 surface failure\n");
			hSurface_S = CSUDI_NULL;
		}
		if(CSUDI_NULL != hSurface_D)
		{
			if(CSUDIOSGDestroySurface(hSurface_D) != CSUDI_SUCCESS)
				 CSTCPrint("Step 9 destroy P2_1 surface failure");
		}
		hSurface_D = CSUDI_NULL;
		return CSUDI_TRUE;
	}
	else 
	{
		CSTCPrint("[%s, %d] Not Supported, PixelFormat=%d !!!\n", __FUNCTION__, __LINE__, ePixelFormat);
	  	return CSUDI_TRUE;	
	}
}

CSUDI_BOOL OSG_Blit_TestAlphaWithDataColor(CSUDIOSGPixelFormat_E ePixelFormat,unsigned int uSrcAlpha,unsigned int uDstAlpha,unsigned char * puSrcColor, unsigned char * puDstColor, int nColorLen)
{
	if(OSG_IsSupportPixelFormat(ePixelFormat))
	{
		CSUDI_HANDLE hSurface_S = CSUDI_NULL;
		CSUDI_HANDLE hSurface_D = CSUDI_NULL;
		CSUDIOSGSurfaceInfo_S sSurfaceInfo;	
		CSUDI_Error_Code nRet = CSUDI_FAILURE;
		CSUDI_HANDLE hDisplay = CSUDI_NULL;
		
		nRet = CSUDIOSGCreateSurface(ePixelFormat,
										SURFACE_WIDTH_720,
										SURFACE_HEIGHT_576,
										&hSurface_S
										);

		CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),"Step 1 P1_1 Create Surface Failure !");
		
		nRet = CSUDIOSGCreateSurface(ePixelFormat,
										SURFACE_WIDTH_720,
										SURFACE_HEIGHT_576,
										&hSurface_D
										);
		CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hSurface_D != CSUDI_NULL),"Step 1 P2_1 Create Surface Failure !");
		
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_S, &sSurfaceInfo)),
					"Step 2 get surface info failure"
				);			
		sSurfaceInfo.m_uAlpha = uSrcAlpha;
		sSurfaceInfo.m_nAlphaFlag = 1;
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGSetSurfaceInfo(hSurface_S, &sSurfaceInfo)),
					"Step 3 set surface info failure"
				);
		
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_D, &sSurfaceInfo)),
					"Step 2 get surface info failure"
				);			
		sSurfaceInfo.m_uAlpha = uDstAlpha;
		sSurfaceInfo.m_nAlphaFlag = 1;
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGSetSurfaceInfo(hSurface_D, &sSurfaceInfo)),
					"Step 4 set surface info failure"
				);	

		//��ʼ��Դ��ͼƽ����ɫ
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iFillSurfaceWithFileData(hSurface_S,puSrcColor,nColorLen,ePixelFormat)),
					"Step 5 Fill src surface failure"
				);	
		
		//��ʼ��Ŀ�Ļ�ͼƽ����ɫ 
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iFillSurfaceWithFileData(hSurface_D,puDstColor,nColorLen,ePixelFormat)),
					"Step 6 Fill dist surface failure"
				);
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGBlit(hSurface_D, CSUDI_NULL, hSurface_S, CSUDI_NULL, EM_UDIOSG_OPMODE_ALPHA)),
					"Step 7 blit  failure"
				);	
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGSync()),
					"Step 8 Sync  failure"
				);	

		CSUDIOSGGetDisplaySurface(g_nTestDisplaySurfaceIndex,&hDisplay);
		
		CSUDIOSGBlit(hDisplay, CSUDI_NULL, hSurface_D, CSUDI_NULL, EM_UDIOSG_OPMODE_ALPHA); //blit���Դ�
		CSTCPrint("[OSG_TEST] nColorLen =====: %d\n",nColorLen);
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iCheckColorAndAlphaWithFileColor(hSurface_D,
									puSrcColor,
									puDstColor,
									nColorLen,
									uSrcAlpha,
									uDstAlpha,
									ePixelFormat)),
					"Step 10 check alpha and color failure"
				);
		
		CSTK_FATAL_POINT
		if(CSUDI_NULL != hSurface_S)
		{
			if(CSUDIOSGDestroySurface(hSurface_S) != CSUDI_SUCCESS)
				 CSTCPrint("Step 9 destroy P1_1 surface failure\n");
			hSurface_S = CSUDI_NULL;
		}
		if(CSUDI_NULL != hSurface_D)
		{
			if(CSUDIOSGDestroySurface(hSurface_D) != CSUDI_SUCCESS)
				 CSTCPrint("Step 9 destroy P2_1 surface failure");
			hSurface_D = CSUDI_NULL;
		}
		return CSUDI_TRUE;
	}
	else 
	{
		CSTCPrint("\tNot Supported, PixelFormat=%d !!!\n", ePixelFormat);
	  	return CSUDI_TRUE;	
	}
}

//�˺�����������alpha���
CSUDI_BOOL OSG_Blit_TestAlphaWithDataColorEx(CSUDIOSGPixelFormat_E ePixelFormat,unsigned int uSrcAlpha,unsigned int uDstAlpha, const unsigned char * puSrcColor, const unsigned char * puDstColor, int nColorLen)
{
	if(OSG_IsSupportPixelFormat(ePixelFormat))
	{
		CSUDI_HANDLE hSurface_S = CSUDI_NULL;
		CSUDI_HANDLE hSurface_D = CSUDI_NULL;
		CSUDIOSGSurfaceInfo_S sSurfaceInfo;	
		CSUDI_Error_Code nRet = CSUDI_FAILURE;
		unsigned char * pSurfaceMem1 = CSUDI_NULL;
		unsigned char * pSurfaceMem2 = CSUDI_NULL;
		CSUDIOSGRect_S  sRect;
		
		sRect.m_nX = sRect.m_nY = 0;
		sRect.m_nWidth = 400;
		sRect.m_nHeight = 100;	

		/*********����P1_1 ��P2_1 ����surface********/
		nRet = CSUDIOSGCreateSurface(ePixelFormat,
										400,
										100,
										&hSurface_S
										);

		CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),"Step 1 P1_1 Create Surface Failure !");
		
		nRet = CSUDIOSGCreateSurface(ePixelFormat,
										400,
										100,
										&hSurface_D
										);
		CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hSurface_D != CSUDI_NULL),"Step 1 P2_1 Create Surface Failure !");

		/*********���SA��Ϊ255��������P1_1��ȫ��alpha********/
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_S, &sSurfaceInfo)),
					"get surface info failure");	

		pSurfaceMem1 = sSurfaceInfo.m_pvSurfaceMem;
		if(uSrcAlpha != 255)
		{
			sSurfaceInfo.m_uAlpha = uSrcAlpha;
			sSurfaceInfo.m_nAlphaFlag = 1;
			
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSetSurfaceInfo(hSurface_S, &sSurfaceInfo)),
						"set surface info failure");	
		}

		/*********���DA��Ϊ255��������P1_2��ȫ��alpha********/
		memset(&sSurfaceInfo, 0, sizeof(CSUDIOSGSurfaceInfo_S));		
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_D, &sSurfaceInfo)),
					"get surface info failure");
		
		pSurfaceMem2 = sSurfaceInfo.m_pvSurfaceMem;
		if(uDstAlpha != 255)
		{
			sSurfaceInfo.m_uAlpha = uDstAlpha;
			sSurfaceInfo.m_nAlphaFlag = 1;
			
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSetSurfaceInfo(hSurface_D, &sSurfaceInfo)),
						"set surface info failure");	
		}

		//��ʼ��Դ��ͼƽ����ɫ
		iFillSurfaceByBmpWithReverseOrder(puSrcColor, hSurface_S, 400, 100);		
		//��ʼ��Ŀ�Ļ�ͼƽ����ɫ 
		iFillSurfaceByBmpWithReverseOrder(puDstColor, hSurface_D, 400, 100);		

		/*********��P1_1��P1_2����alpha���********/	
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGBlit(hSurface_D, CSUDI_NULL, hSurface_S, CSUDI_NULL, EM_UDIOSG_OPMODE_ALPHA)),
					"blit  failure");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),
					"Sync  failure");	

		/*********��������ֵ��Ӳ�����ֵ���м���********/	
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iCheckColorAndAlphaWithFileColor(hSurface_D,
									puSrcColor,
									puDstColor,
									nColorLen,
									uSrcAlpha,
									uDstAlpha,
									ePixelFormat)),
					"check alpha and color failure"
				);		
		
		CSTK_FATAL_POINT
			
		if(CSUDI_NULL != hSurface_S)
		{
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface_S)),"destroy  hSurface_S failure");
			hSurface_S = CSUDI_NULL;
		}
		
		if(CSUDI_NULL != hSurface_D)
		{
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface_D)),"destroy  hSurface_D failure");
			hSurface_D = CSUDI_NULL;
		}
		
		return CSUDI_TRUE;
	}
	else 
	{
		CSTCPrint("[%s, %d] Not Supported, PixelFormat=%d !!!\n", __FUNCTION__, __LINE__, ePixelFormat);
	  	return CSUDI_TRUE;	
	}
}

CSUDI_BOOL OSG_Blit_TestOpMode(CSUDIOSGPixelFormat_E ePixelFormat, unsigned int unSrcColor, unsigned int unDstColor, CSUDIOSGOpMode_E eMode)
{
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hSurface_D = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;

	unsigned int unSrcColorTmp = unSrcColor;
	unsigned int unDstColorTmp = unDstColor;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);

	if (ePixelFormat != EM_UDIOSG_PIXEL_FORMAT_ARGB_8888)
	{
		unSrcColorTmp = OSG_iGetColorWithPixelMode(unSrcColor, ePixelFormat);
		unDstColorTmp = OSG_iGetColorWithPixelMode(unDstColor, ePixelFormat);
	}

	CSTCPrint("SrcColor=%08x, DstColor=%08x\n", unSrcColorTmp, unDstColorTmp);
	
	nRet = CSUDIOSGCreateSurface(ePixelFormat, SURFACE_WIDTH_720,SURFACE_HEIGHT_576, &hSurface_S);
	CSTK_ASSERT_TRUE_FATAL(
				(nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),
				"Step 1 Create P1_1 Surface Failure !"
			);
		
	nRet = CSUDIOSGCreateSurface(ePixelFormat, SURFACE_WIDTH_720, SURFACE_HEIGHT_576, &hSurface_D);
	CSTK_ASSERT_TRUE_FATAL(
				(nRet == CSUDI_SUCCESS && hSurface_D != CSUDI_NULL),
				"Step 1 Create P2_1 Surface Failure !"
			);
			
	//��ʼ��Դ��ͼƽ����ɫ
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFillSurface(hSurface_S, unSrcColorTmp, ePixelFormat)),
				"Step 2 Fill src surface failure"
			);	
	
	//��ʼ��Ŀ�Ļ�ͼƽ����ɫ 
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFillSurface(hSurface_D, unDstColorTmp, ePixelFormat)),
				"Step 3 Fill dist surface failure"
			);
	nRet = CSUDIOSGBlit(hSurface_D, CSUDI_NULL, hSurface_S, CSUDI_NULL, eMode);
	
	CSTK_ASSERT_TRUE_FATAL(
				((CSUDI_SUCCESS == nRet) ||(CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED == nRet)),
				"Step 4 blit  failure"
			);	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGSync()),
				"Sync failure"
			);	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iCheckColorMixResult(hSurface_D,
								unSrcColorTmp,
								unDstColorTmp,
								eMode,
								ePixelFormat)),
				"Step 6 check Mixed color failure"
			);

	CSTK_FATAL_POINT
		
	if(hSurface_S != CSUDI_NULL)
	{
		if(CSUDIOSGDestroySurface(hSurface_S) != CSUDI_SUCCESS)
		{
			 CSTCPrint("Destroy P1_1 surface failure !\n");
		}
		
		hSurface_S = CSUDI_NULL;
	}
	
	if(hSurface_D != CSUDI_NULL)
	{
		if(CSUDIOSGDestroySurface(hSurface_D) != CSUDI_SUCCESS)
		{
			 CSTCPrint("Destroy P2_1 surface failure !");
		}
		
		hSurface_D = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;	
}

CSUDI_BOOL OSG_Blit_TestFill(CSUDIOSGPixelFormat_E ePixelFormat,unsigned int unFillColor,OSGUDI2Rect_E eRect,CSUDI_BOOL bValidRect)
{
	CSUDI_HANDLE hSurface = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;
	CSUDIOSGRect_S sRect;
	CSUDIOSGRect_S * psRect = &sRect;
	unsigned int unTmpColor = unFillColor;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);

	nRet = CSUDIOSGCreateSurface(ePixelFormat, SURFACE_WIDTH_720, SURFACE_HEIGHT_576, &hSurface);
	CSTK_ASSERT_TRUE_FATAL(
				(nRet == CSUDI_SUCCESS && hSurface != CSUDI_NULL),
				"Step 1 Create P1_1 Surface Failure !"
			);
			
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo)),
				"Get P1_1 Surface info failure !"
			);	
	CSTK_ASSERT_TRUE_FATAL(
				((sSurfaceInfo.m_pvSurfaceMem != CSUDI_NULL)&&(sSurfaceInfo.m_nMemSize>0)),
				"Get P1_1 Surface info failure !"
			);
	
	memset(sSurfaceInfo.m_pvSurfaceMem, 0x88, sSurfaceInfo.m_nMemSize);
	
	//*****************************************************************//
	if(eRect == EM_RECT_1)
	{
		psRect = CSUDI_NULL;
	}
	OSG_iFillRectExt(psRect, 0, 0, SURFACE_WIDTH_720, SURFACE_HEIGHT_576, eRect);
	
	if (ePixelFormat != EM_UDIOSG_PIXEL_FORMAT_ARGB_8888)
	{
		unTmpColor = OSG_iGetColorWithPixelMode(unFillColor, ePixelFormat);
	}
	
	nRet = CSUDIOSGFill(hSurface, psRect, unFillColor);
	if(bValidRect)
	{
		CSTK_ASSERT_TRUE_FATAL(
					((nRet == CSUDI_SUCCESS)),/*�Ŵ�blitʱ���ܲ�֧��*/
					"Step 3 fill should be success !"
				);
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGSync()),
					"Step 4 Sync Failure !"
				);
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iCheckExistColor(hSurface, unTmpColor, psRect, ePixelFormat)),
					"Check Surface color Failure !"
				);
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(
					(nRet == CSUDIOSG_ERROR_BAD_PARAMETER),
					"Blit should be failure !"
				);
	}
	
	CSTK_FATAL_POINT
		
	if(hSurface != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface)),
				"Destroy P1_1 surface failure !"
			);
		
		hSurface = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;	
}

//�˺�������������surface
CSUDI_BOOL OSG_Blit_TestFillOnePixel(CSUDIOSGPixelFormat_E ePixelFormat,unsigned int unFillColor)
{
	CSUDI_HANDLE hSurface = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;
	CSUDIOSGRect_S sRect;
	unsigned int unTmpColor = unFillColor;
	int i,j;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);

	sRect.m_nX = sRect.m_nY = 0;
	sRect.m_nWidth = 1;
	sRect.m_nHeight = 1;
	
	nRet = CSUDIOSGCreateSurface(ePixelFormat,200,200,&hSurface);
	CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hSurface != CSUDI_NULL),	"Step 1 Create  Surface Failure !");
			
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo)),"Get  Surface info failure !"	);	
	CSTK_ASSERT_TRUE_FATAL(((sSurfaceInfo.m_pvSurfaceMem != CSUDI_NULL)&&(sSurfaceInfo.m_nMemSize>0)),"Get  Surface info failure !");
	
	memset(sSurfaceInfo.m_pvSurfaceMem, 0x88, sSurfaceInfo.m_nMemSize);
	
	//*****************************************************************//
	unTmpColor = OSG_iGetColorWithPixelMode(unFillColor, ePixelFormat);

	CSTCPrint("��������ʱ�Ͼã������ĵȴ�������\n");
	
	for(i = 0; i< 200; i++)
	{			
		for(j = 0; j< 200; j++)
		{
			sRect.m_nX = j;
			sRect.m_nY = i;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGFill(hSurface, &sRect, unFillColor), "Step 2 fill should be success ");//ucFillColor	
		}
	}		

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync())," Sync Failure !");

	sRect.m_nX = sRect.m_nY = 0;
	sRect.m_nWidth  = 200;
	sRect.m_nHeight = 200;
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == OSG_iCheckExistColor(hSurface, unTmpColor, &sRect, ePixelFormat)), "step 3 Check Surface color Failure !"	);
	
	CSTK_FATAL_POINT;
	
	if(hSurface != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface)),	"step 4 Destroy surface failure !");
		hSurface = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
}

//�˺�����OSG_Blit_TestFill�Ĳ�ͬ����������ͬsurface�Ŀ�ȡ��߶Ȳ�ͬ
CSUDI_BOOL OSG_Blit_TestFillEx(CSUDIOSGPixelFormat_E ePixelFormat, unsigned int unFillColor, int nSurfaceWidth, int nSurfaceHeight, CSUDIOSGRect_S * psSrcRect)
{
	CSUDI_HANDLE hSurface = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;
	unsigned int unTmpColor = unFillColor;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);		
		
	nRet = CSUDIOSGCreateSurface(ePixelFormat,nSurfaceWidth,nSurfaceHeight,&hSurface);
	CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hSurface != CSUDI_NULL),	"Create  Surface Failure !");

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo)),"Get  Surface info failure !");	
	CSTK_ASSERT_TRUE_FATAL(((sSurfaceInfo.m_pvSurfaceMem != CSUDI_NULL)&&(sSurfaceInfo.m_nMemSize>0)),"Get  Surface info failure !");

	if(sSurfaceInfo.m_pvSurfaceMem != CSUDI_NULL && sSurfaceInfo.m_nMemSize > nSurfaceWidth *  nSurfaceHeight)
	{
		memset(sSurfaceInfo.m_pvSurfaceMem, 0x88, sSurfaceInfo.m_nMemSize);		
	}	
	
	if (ePixelFormat != EM_UDIOSG_PIXEL_FORMAT_ARGB_8888)
	{
		unTmpColor = OSG_iGetColorWithPixelMode(unFillColor, ePixelFormat);
	}
	
	CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS == CSUDIOSGFill(hSurface, psSrcRect, unFillColor))),	"fill surface failed !");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()), "Sync Failure !");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == OSG_iCheckExistColor(hSurface, unTmpColor, psSrcRect, ePixelFormat)),"Check Surface color Failure !");
	
	CSTK_FATAL_POINT;
	if(hSurface != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface)),"Destroy  surface failure !");
		hSurface = CSUDI_NULL;
	}
	
		return CSUDI_TRUE;
}

CSUDI_BOOL OSG_iTestFillDisplay(unsigned int ucFillColor,OSGUDI2Rect_E eRect,CSUDI_BOOL bValidRect)
{
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;
	CSUDIOSGRect_S sRect;
	CSUDIOSGRect_S * psRect = &sRect;
	unsigned int ucTmpColor = 0;
	nRet = CSUDIOSGGetDisplaySurface(g_nTestDisplaySurfaceIndex,&hSurface_S);
	CSTK_ASSERT_TRUE_FATAL(
				(nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),
				"Step 1 Get display Surface Failure !"
			);
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_S, &sSurfaceInfo)),
				"get display info Failure !"
			);
	
	//*****************************************************************//
	if(eRect == EM_RECT_1)
	{
		psRect = CSUDI_NULL;
	}
	OSG_iFillRectExt(psRect,0,0,sSurfaceInfo.m_nWidth,sSurfaceInfo.m_nHeight,eRect);
	//*****************************************************************//
 	ucTmpColor = OSG_iGetColorWithPixelMode(ucFillColor,sSurfaceInfo.m_ePixelFormat);
	nRet = CSUDIOSGFill(hSurface_S, psRect, ucFillColor);
	
	if(bValidRect)
	{
		CSTK_ASSERT_TRUE_FATAL(
					((nRet == CSUDI_SUCCESS)),
					"Step 3 fill should be success !"
				);
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGSync()),
					"Step 4 Sync Failure !"
				);
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iCheckExistColor(hSurface_S,ucTmpColor,psRect,sSurfaceInfo.m_ePixelFormat)),
					"Check Surface color Failure !"
				);
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(
					(nRet == CSUDIOSG_ERROR_BAD_PARAMETER),
					"Blit should be failure !"
				);
	}

	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

CSUDI_BOOL OSG_iCheckDecodeImgSurfaceInfo(OSGUDI2ImageType_E eImageType, CSUDIOSGScalingMode_E eScalingMode, const CSUDIOSGRect_S * psDstRect, CSUDIOSGSurfaceInfo_S* psSurfaceInfo)
{
	CSUDI_BOOL bRet = CSUDI_FALSE;
	int nSrWidth = 0;
	int nSrHeight = 0;

	switch (eImageType)
	{
		case EM_IFRAME_720_576:
		case EM_IFRAME_PES_720_576:
		case EM_BMP_720_576:
		case EM_JPG_720_576:
		case EM_PNG_720_576:
		{
			nSrWidth = 720;
			nSrHeight = 576;
		}
		break;
		
		case EM_IFRAME_1280_720:
		case EM_BMP_1280_720:
		case EM_JPG_1280_720:
		case EM_PNG_1280_720:
		{
			nSrWidth = 1280;
			nSrHeight = 720;
		}
		break;
			
		case EM_IFRAME_1920_1080:
		case EM_BMP_1920_1080:
		case EM_JPG_1920_1080:
		case EM_PNG_1920_1080:
		{
			nSrWidth = 1920;
			nSrHeight = 1080;
		}
		break;
			
		case EM_JPG_8192_8192:	
		{
			nSrWidth = 8192;
			nSrHeight = 8192;
		}
		break;
							
		case EM_BMP_CIRCLE_576_576:
		case EM_JPG_CIRCLE_576_576:
		case EM_PNG_CIRCLE_576_576:
		{
			nSrWidth = 576;
			nSrHeight = 576;
		}
		break;

		default:
			CSTCPrint("[%s, %d] Error ImageType:%d\n", __FUNCTION__, __LINE__, eImageType);
			break;
	}

	if ((nSrWidth != 0) && (nSrHeight != 0))
	{
		if (EM_UDIOSG_SCALE_FULLSIZE == eScalingMode)
		{
			if ((nSrWidth == psSurfaceInfo->m_nWidth) && (nSrHeight == psSurfaceInfo->m_nHeight))
			{
				bRet = CSUDI_TRUE;
			}
			else
			{
				psSurfaceInfo->m_nWidth = nSrWidth;
				psSurfaceInfo->m_nHeight = nSrHeight;
			}
		}

		if (psDstRect != CSUDI_NULL)
		{
			if (EM_UDIOSG_SCALE_FIXSIZE == eScalingMode)
			{
				if ((psDstRect->m_nWidth == psSurfaceInfo->m_nWidth) 
					&& (psDstRect->m_nHeight == psSurfaceInfo->m_nHeight))
				{
					bRet = CSUDI_TRUE;
				}
				else
				{
					psSurfaceInfo->m_nWidth = psDstRect->m_nWidth;
					psSurfaceInfo->m_nHeight = psDstRect->m_nHeight;
				}
			}
			else if (EM_UDIOSG_SCALE_OPTIMUM== eScalingMode)
			{
				if (psDstRect->m_nWidth*nSrHeight > nSrWidth*psDstRect->m_nHeight)
				{
					if ((psDstRect->m_nHeight*nSrWidth/nSrHeight == psSurfaceInfo->m_nWidth) 
						&& (psDstRect->m_nHeight == psSurfaceInfo->m_nHeight))
					{
						bRet = CSUDI_TRUE;
					}
					else
					{
						psSurfaceInfo->m_nWidth = psDstRect->m_nHeight*nSrWidth/nSrHeight;
						psSurfaceInfo->m_nHeight = psDstRect->m_nHeight;
					}
					
				}
				else
				{
					if ((psDstRect->m_nWidth == psSurfaceInfo->m_nWidth) 
						&& (psDstRect->m_nWidth*nSrHeight/nSrWidth == psSurfaceInfo->m_nHeight))
					{
						bRet = CSUDI_TRUE;
					}
					else
					{
						psSurfaceInfo->m_nWidth = psDstRect->m_nWidth;
						psSurfaceInfo->m_nHeight = psDstRect->m_nWidth*nSrHeight/nSrWidth;
					}
				}
			}			
		}
	}
	
	return bRet;
}
CSUDI_BOOL OSG_iTestDecImage(OSGUDI2SupportDecIamgeType_E eSupportType,OSGUDI2ImageType_E eImageType)
{
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hSurface_D = CSUDI_NULL;
	unsigned char * pcBuff = CSUDI_NULL;
	unsigned int nImageLen = 0;
	char cType[32];
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;
	
	switch(eSupportType)
	{
		case EM_OSG_DECIMAGE_IFRAME:
			strcpy(cType,"IFRAME");
			break;
		case EM_OSG_DECIMAGE_BMP:
			strcpy(cType,"BMP");
			break;
		case EM_OSG_DECIMAGE_JPG:
			strcpy(cType,"JPEG");
			break;
		case EM_OSG_DECIMAGE_PNG:
			strcpy(cType,"PNG");
			break;
		default:
			CSTCPrint("[%s, %d] Error ImageType: %d\n", __FUNCTION__, __LINE__, eSupportType);
			return CSUDI_FALSE;
	}

	if ((eSupportType == EM_OSG_DECIMAGE_BMP) || (eSupportType == EM_OSG_DECIMAGE_PNG))
	{
		CSTCPrint("���м��δʹ�ã��ݲ�����%s����ͼƬӲ����\n", cType);
		return CSUDI_TRUE;
	}
	else if (eSupportType == EM_OSG_DECIMAGE_IFRAME)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_IsSupportDecImage(eSupportType)), 
				"ƽ̨��֧��IFRAME����ͼƬӲ����");
	}
	else if (eSupportType == EM_OSG_DECIMAGE_JPG)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_IsSupportDecImage(eSupportType)), 
				"ƽ̨��֧��JPEG����ͼƬӲ����");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_FALSE), "�����ͼƬ����");
	}
	
	nImageLen = OSG_iGetPhoto(g_aImageFile[eImageType],&pcBuff);
	CSTK_ASSERT_TRUE_FATAL(
				((pcBuff != CSUDI_NULL) && nImageLen != 0),
				"Step 1 get photo data failure"
			);
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGDecodeImage(cType,pcBuff,nImageLen,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,&hSurface_S) && (hSurface_S != CSUDI_NULL)),
				"Step 2 decode image failure"
			);

	memset(&sSurfaceInfo, 0, sizeof(sSurfaceInfo));
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_S, &sSurfaceInfo)),
				"��ȡӲ���������surface��Ϣʧ��"
			);

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iCheckDecodeImgSurfaceInfo(eImageType, EM_UDIOSG_SCALE_FULLSIZE, CSUDI_NULL, &sSurfaceInfo)),
				"���Ӳ���������surface��Ϣ����"
			);
	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(g_nTestDisplaySurfaceIndex,&hSurface_D)),
				"Step 3 get display surface failure"
			);
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGBlit(hSurface_D,CSUDI_NULL,hSurface_S,CSUDI_NULL,EM_UDIOSG_OPMODE_COPY)),
				"Step 4 blit image failure"
			);
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGSync()),
				"Step 5 snyc failure"
			);	
	CSTCPrint("Yes,��Ļ����ʾ %s ����...\r\n",g_aImageFile[eImageType]);
	CSTCPrint("No, ��Ļ��û�� %s ����...\r\n",g_aImageFile[eImageType]);
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == CSTKWaitYes()),
				"Step 6 Check failure"
			);	

	CSTK_FATAL_POINT
	if(hSurface_S != CSUDI_NULL)
	{
		if(CSUDIOSGDestroySurface(hSurface_S) != CSUDI_SUCCESS)
			 CSTCPrint("Step 7 destroy decode surface failure\n");
		hSurface_S = CSUDI_NULL;
	}
	if(pcBuff != CSUDI_NULL)
	{
		if(CSUDIOSFree(pcBuff) != CSUDI_SUCCESS)
			 CSTCPrint("Step 4 free photo data failure");
		pcBuff = CSUDI_NULL;
		CSTCPrint("[UDI2OSGEST]CSUDIOSFree success !! \r\n");
	}

	OSG_iClearScreen();
	
	return CSUDI_TRUE;
}

//�˺���ͨ���ļ��ĺ�׺�����ж����ݵ�����
OSGUDI2SupportDecIamgeType_E OSG_iGetImageType(char *pcName)
{
	OSGUDI2SupportDecIamgeType_E eSupportType ;

	if(strcasestr(pcName, ".bmp")!= NULL)
	{
		eSupportType = EM_OSG_DECIMAGE_BMP;
	}
	else if(strcasestr(pcName, ".jpg")!= NULL)
	{
		eSupportType = EM_OSG_DECIMAGE_JPG;
	}	
	else if(strcasestr(pcName, ".png")!= NULL)
	{
		eSupportType = EM_OSG_DECIMAGE_PNG;
	}
	else if(strcasestr(pcName, ".mpg")!= NULL)
	{
		eSupportType = EM_OSG_DECIMAGE_IFRAME;
	}
	else
	{
		eSupportType = EM_OSG_DECIMAGE_MAX + 1;
	}

	return eSupportType;
}

//����ָ��·����ͼƬ
CSUDI_BOOL OSG_iTestDecImageByPath(char *pcName)
{
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hSurface_D = CSUDI_NULL;
	unsigned char * pcBuff = CSUDI_NULL;
	unsigned int nImageLen = 0;
	char cType[32] = {0};
	OSGUDI2SupportDecIamgeType_E eSupportType;
	unsigned int dwStartTime = 0;
	unsigned int dwEndTime   = 0;	

	eSupportType = OSG_iGetImageType(pcName);
	
	switch(eSupportType)
	{
		case EM_OSG_DECIMAGE_IFRAME:
			strcpy(cType,"IFRAME");
			break;
		case EM_OSG_DECIMAGE_BMP:
			strcpy(cType,"BMP");
			break;
		case EM_OSG_DECIMAGE_JPG:
			strcpy(cType,"JPEG");
			break;
		case EM_OSG_DECIMAGE_PNG:
			strcpy(cType,"PNG");
			break;
		default:
			CSTCPrint("[%s, %d] Error ImageType: %d\n", __FUNCTION__, __LINE__, eSupportType);
			return CSUDI_FALSE;
	}

	if ((eSupportType == EM_OSG_DECIMAGE_BMP) || (eSupportType == EM_OSG_DECIMAGE_PNG))
	{
		CSTCPrint("���м��δʹ�ã��ݲ�����%s����ͼƬӲ����\n", cType);
		return CSUDI_TRUE;
	}
	else if (eSupportType == EM_OSG_DECIMAGE_IFRAME)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_IsSupportDecImage(eSupportType)), 
				"ƽ̨��֧��IFRAME����ͼƬӲ����");
	}
	else if (eSupportType == EM_OSG_DECIMAGE_JPG)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_IsSupportDecImage(eSupportType)), 
				"ƽ̨��֧��JPEG����ͼƬӲ����");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_FALSE), "�����ͼƬ����");
	}
	
	nImageLen = OSG_iGetPhoto(pcName,&pcBuff);
	CSTK_ASSERT_TRUE_FATAL(((pcBuff != CSUDI_NULL) && nImageLen != 0),
				"Step 1 get photo data failure");

	dwStartTime = CSUDIPlusOSGetTickCount();	
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDecodeImage(cType,pcBuff,nImageLen,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,&hSurface_S) && (hSurface_S != CSUDI_NULL)),
				"Step 2 decode image failure");	
	
	dwEndTime = CSUDIPlusOSGetTickCount();
	
	CSTCPrint("[UDI2OSGEST]CSUDIOSGDecodeImage %s use time :%d ms !! \r\n", pcName, dwEndTime - dwStartTime);
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(g_nTestDisplaySurfaceIndex,&hSurface_D)),
				"Step 3 get display surface failure");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGBlit(hSurface_D,CSUDI_NULL,hSurface_S,CSUDI_NULL,EM_UDIOSG_OPMODE_COPY)),
				"Step 4 blit image failure");
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),
				"Step 5 snyc failure");	

	CSTK_FATAL_POINT;

	OSG_iClearScreen();
	
	if(hSurface_S != CSUDI_NULL)
	{
		if(CSUDIOSGDestroySurface(hSurface_S) != CSUDI_SUCCESS)
		{
			CSTCPrint("Step 7 destroy decode surface failure\n");
		}
		hSurface_S = CSUDI_NULL;
	}
	
	if(pcBuff != CSUDI_NULL)
	{
		if(CSUDIOSFree(pcBuff) != CSUDI_SUCCESS)
		{
			CSTCPrint("Step 8 free photo data failure");
		}

		pcBuff = CSUDI_NULL;
		CSTCPrint("[UDI2OSGEST]CSUDIOSFree success !! \r\n");
	}
	
	return CSUDI_TRUE;
}

//ѭ������ָ���ļ������ļ�
CSUDI_BOOL OSG_iTestDecFolderImages()
{
	int nMax = EM_IMAGE_MAX;
	int i;
	char *pcName;
	CSUDI_BOOL bRet = CSUDI_TRUE;
	
	for(i = 0; i< nMax ; i++)
	{
		if ((i != EM_JPG_8192_8192) && (i != EM_JPG_SOF_Y_SAMPLE_12))
		{
			pcName = g_aImageFile[i];
			bRet &= OSG_iTestDecImageByPath(pcName);
		}
		else
		{
			CSTCPrint("���м��δʹ�ã��ݲ�����ͼƬ%s��Ӳ����\n", g_aImageFile[i]);
		}
	}

	return bRet;
}

CSUDI_BOOL OSG_iTestDecImageWithScaleMode(OSGUDI2SupportDecIamgeType_E eSupportType,OSGUDI2ImageType_E eImageType,CSUDIOSGScalingMode_E eScalingMode,const CSUDIOSGRect_S * psDstRect)
{
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hSurface_D = CSUDI_NULL;
	unsigned char * pcBuff = CSUDI_NULL;
	unsigned int nImageLen = 0;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;
	CSUDIOSGRect_S sOutRect;
	char cType[32];

	switch(eSupportType)
	{
		case EM_OSG_DECIMAGE_IFRAME:
			strcpy(cType,"IFRAME");
			break;
		case EM_OSG_DECIMAGE_BMP:
			strcpy(cType,"BMP");
			break;
		case EM_OSG_DECIMAGE_JPG:
			strcpy(cType,"JPEG");
			break;
		case EM_OSG_DECIMAGE_PNG:
			strcpy(cType,"PNG");
			break;
		default:
			CSTCPrint("[%s, %d] Error ImageType: %d\n", __FUNCTION__, __LINE__, eSupportType);
			return CSUDI_FALSE;
	}

	if ((eSupportType == EM_OSG_DECIMAGE_BMP) || (eSupportType == EM_OSG_DECIMAGE_PNG))
	{
		CSTCPrint("���м��δʹ�ã��ݲ�����%s����ͼƬӲ����\n", cType);
		return CSUDI_TRUE;
	}
	else if (eSupportType == EM_OSG_DECIMAGE_IFRAME)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_IsSupportDecImage(eSupportType)), 
				"ƽ̨��֧��IFRAME����ͼƬӲ����");
	}
	else if (eSupportType == EM_OSG_DECIMAGE_JPG)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_IsSupportDecImage(eSupportType)), 
				"ƽ̨��֧��JPEG����ͼƬӲ����");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_FALSE), "�����ͼƬ����");
	}
		
	nImageLen = OSG_iGetPhoto(g_aImageFile[eImageType],&pcBuff);
	CSTK_ASSERT_TRUE_FATAL(
				((pcBuff != CSUDI_NULL) && nImageLen != 0),
				"Step 1 get photo data failure"
			);
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGDecodeImage(cType,pcBuff,nImageLen,eScalingMode,psDstRect,&hSurface_S) && (hSurface_S != CSUDI_NULL)),
				"Step 2 decode image failure"
			);

	memset(&sSurfaceInfo,0,sizeof(CSUDIOSGSurfaceInfo_S));
	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_S, &sSurfaceInfo)),
				"Step 3 get surfaceinfo failure"
			);

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iCheckDecodeImgSurfaceInfo(eImageType, eScalingMode, psDstRect, &sSurfaceInfo)),
				"���Ӳ���������surface��Ϣ����"
			);
	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(g_nTestDisplaySurfaceIndex,&hSurface_D)),
				"Step 3 get display surface failure"
			);

	sOutRect.m_nHeight = sSurfaceInfo.m_nHeight;
	sOutRect.m_nWidth = sSurfaceInfo.m_nWidth;
	
	memset(&sSurfaceInfo, 0, sizeof(sSurfaceInfo));

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_D, &sSurfaceInfo)),
				"Get display surface failed"
			);

	sOutRect.m_nX = (sSurfaceInfo.m_nWidth/2) - (sOutRect.m_nWidth/2);
	sOutRect.m_nY = (sSurfaceInfo.m_nHeight/2) - (sOutRect.m_nHeight/2);
	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGBlit(hSurface_D,&sOutRect,hSurface_S,CSUDI_NULL,EM_UDIOSG_OPMODE_COPY)),
				"Step 4 blit image failure"
			);
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGSync()),
				"Step 5 snyc failure"
			);

    	switch(eScalingMode)
    	{
        		case EM_UDIOSG_SCALE_FULLSIZE:
            		CSTCPrint("����ͼƬ\"%s\"����״Ϊ�����ţ�ԭʼ��С\n", g_aImageFile[eImageType]);
            	break;
        		case EM_UDIOSG_SCALE_FIXSIZE:
            		CSTCPrint("����ͼƬ\"%s\"����״Ϊ�����������ţ������߾�Ϊָ��ֵ\n", g_aImageFile[eImageType]);
            	break;
        		case EM_UDIOSG_SCALE_OPTIMUM:
            		CSTCPrint("����ͼƬ\"%s\"����״Ϊ���������ţ��������ռ�����������������\n", g_aImageFile[eImageType]);
            	break;
        		default:
            		CSTCPrint("ͼƬ��ʾ���ʹ���Ч��δ֪\n", g_aImageFile[eImageType]);
            	break;
    	}

	CSTCPrint("��ʼsurface��СΪ{w:%d, h:%d}��ʵ�����ӦΪ{x:%d, y:%d, w:%d, h:%d} \n", psDstRect->m_nWidth, psDstRect->m_nHeight, sOutRect.m_nX, sOutRect.m_nY, sOutRect.m_nWidth, sOutRect.m_nHeight);
		
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == CSTKWaitYes()),
				"Step 6 Check failure"
			);	
	
	CSTK_FATAL_POINT;
		
	if(hSurface_S != CSUDI_NULL)
	{
		if(CSUDIOSGDestroySurface(hSurface_S) != CSUDI_SUCCESS)
			 CSTCPrint("Step 7 destroy decode surface failure\n");
		hSurface_S = CSUDI_NULL;
		CSUDIOSGFill(hSurface_D, CSUDI_NULL, 0);
		CSUDIOSGSync();
	}
	if(pcBuff != CSUDI_NULL)
	{
		if(CSUDIOSFree(pcBuff) != CSUDI_SUCCESS)
			 CSTCPrint("Step 4 free photo data failure");
		pcBuff = CSUDI_NULL;
		CSTCPrint("[UDI2OSGEST]CSUDIOSFree success !! \r\n");
	}

	OSG_iClearScreen();
	
	return CSUDI_TRUE;
}

CSUDI_BOOL OSG_iTestDecImageSeveralJpg()
{
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hSurface_D = CSUDI_NULL;
	unsigned char * pcBuff = CSUDI_NULL;
	unsigned int nImageLen = 0;
	char cType[32];
	int i=0;
	CSUDI_Error_Code enRet;
	OSGUDI2ImageType_E enJpg[]={EM_JPG_DOUBLE_SOF,EM_JPG_SOF_Y_SAMPLE_12,EM_JPG_CIRCLE_576_576};

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == OSG_IsSupportDecImage(EM_OSG_DECIMAGE_JPG)), 
			"ƽ̨��֧��JPEG����ͼƬӲ����");

	strcpy(cType,"JPG");
	for(i=0;i<sizeof(enJpg)/sizeof(OSGUDI2ImageType_E);i++)
	{
		nImageLen = OSG_iGetPhoto(g_aImageFile[enJpg[i]],&pcBuff);
		CSTK_ASSERT_TRUE_FATAL(
					((pcBuff != CSUDI_NULL) && nImageLen != 0),
					"Step 1 get photo data failure"
				);
		
		enRet=CSUDIOSGDecodeImage(cType,pcBuff,nImageLen,EM_UDIOSG_SCALE_FULLSIZE,NULL,&hSurface_S);
		/*���Ӳ��ƽ̨֧�ָ�ͼƬ������Ҫ����ʾ����*/
		if(enRet == CSUDI_SUCCESS)
		{
			CSTK_ASSERT_TRUE_FATAL((hSurface_S!=CSUDI_NULL),"step 4 CSUDIOSGDecodeImage return success but surface is null");
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(g_nTestDisplaySurfaceIndex,&hSurface_D)),
					"Step 3 get display surface failure"
				);
			CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_SUCCESS == CSUDIOSGBlit(hSurface_D,CSUDI_NULL,hSurface_S,CSUDI_NULL,EM_UDIOSG_OPMODE_COPY)),
						"Step 4 blit image failure"
					);
			CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_SUCCESS == CSUDIOSGSync()),
						"Step 5 snyc failure"
					);
		
			CSTCPrint("\r��Ļ���Ƿ�������ʾͼ����%s??\n",g_aImageFile[enJpg[i]]);

			CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_TRUE == CSTKWaitYes()),
						"Step 6 Check failure"
					);	
		}
		else 
		{
			CSTK_ASSERT_TRUE_FATAL((enJpg[i]!=EM_JPG_CIRCLE_576_576),"UDI2OSG_Circle_576_576.jpg should be decoded normal");
		}
		if (hSurface_S != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface_S)),
					"Step 7 destroy decode surface failure"
				);
			hSurface_S = CSUDI_NULL;

			CSUDIOSGFill(hSurface_D, CSUDI_NULL, 0);
			CSUDIOSGSync();
		}
		if(pcBuff != CSUDI_NULL)
		{
			CSUDIOSFree(pcBuff);
			pcBuff = CSUDI_NULL;
		}
	}

	CSTK_FATAL_POINT
	if(hSurface_S != CSUDI_NULL)
	{
		if(CSUDIOSGDestroySurface(hSurface_S) != CSUDI_SUCCESS)
			 CSTCPrint("Destroy  surface failure !\n");
		hSurface_S = CSUDI_NULL;
		CSUDIOSGFill(hSurface_D, CSUDI_NULL, 0);
		CSUDIOSGSync();
	}
	if(pcBuff != CSUDI_NULL)
	{
		if(CSUDIOSFree(pcBuff) != CSUDI_SUCCESS)
			 CSTCPrint("Step 4 free photo data failure");
		pcBuff = CSUDI_NULL;
		CSTCPrint("[UDI2OSGEST]CSUDIOSFree success !! \r\n");
	}

	OSG_iClearScreen();
	
	return CSUDI_TRUE;	
}

#define TEST_SYNC_TIMES 10  //�ظ����Դ���
#define SECOND_SYNC_TIME  5000  //�ڶ���sync������5��֮��
CSUDI_BOOL OSG_iTestSync(CSUDIOSGPixelFormat_E ePixelFormat,CSUDI_BOOL bWithAlpha,int nSyncTimes)
{
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hSurface_D = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;	
	int ii = 0,jj =0;
	unsigned int uSrcFillColor = 0;
	unsigned int uDstFillColor = 0;
	unsigned int uBlueColor = 0;
	unsigned int uRedColor = 0;
	unsigned int uAlpha = 0;
	unsigned int dwStartTime = 0;
	unsigned int dwEndTime   = 0;
	unsigned int dwSyncTime = 0;
	CSUDIOSGOpMode_E eTmpMode = bWithAlpha ? EM_UDIOSG_OPMODE_ALPHA : EM_UDIOSG_OPMODE_COPY;
	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);
		
	nRet = CSUDIOSGCreateSurface(ePixelFormat,SURFACE_WIDTH_720,SURFACE_HEIGHT_576,&hSurface_S);
	CSTK_ASSERT_TRUE_FATAL(
				(nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),
				"Step 1 Create P1_1 Surface Failure !"
			);

	nRet = CSUDIOSGCreateSurface(ePixelFormat,SURFACE_WIDTH_720,SURFACE_HEIGHT_576,&hSurface_D);
	CSTK_ASSERT_TRUE_FATAL(
				(nRet == CSUDI_SUCCESS && hSurface_D != CSUDI_NULL),
				"Step 1 Create P2_1 Surface Failure !"
			);
	
	switch(ePixelFormat)
	{
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_8888:
				uSrcFillColor = OSG_COLOR_RED_32BIT;
				uDstFillColor = OSG_COLOR_BLUE_32BIT;
				uBlueColor = OSG_COLOR_BLUE_32BIT;
				uRedColor = OSG_COLOR_RED_32BIT;
				uAlpha = OSGUDI2_32BIT_ALPHA_1;
				break;
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_1555:
		case EM_UDIOSG_PIXEL_FORMAT_RGBA_5551:
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_4444:
		case EM_UDIOSG_PIXEL_FORMAT_RGB_565:
				uSrcFillColor = OSG_COLOR_RED_16BIT;
				uDstFillColor = OSG_COLOR_BLUE_16BIT;
				uBlueColor = OSG_COLOR_BLUE_16BIT;
				uRedColor = OSG_COLOR_RED_16BIT;
				uAlpha = OSGUDI2_16BIT_ALPHA_1;
				break;
		break;
		default:
			break;
	}
	for (ii = 0; ii < TEST_SYNC_TIMES; ii++ )
	{
		if(bWithAlpha)
		{
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_S, &sSurfaceInfo)),
					"get surface info Failure !"
				);
			sSurfaceInfo.m_uAlpha = uAlpha;
			sSurfaceInfo.m_nAlphaFlag = 1;
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGSetSurfaceInfo(hSurface_S,&sSurfaceInfo)),
					"set surface info Failure !"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_D, &sSurfaceInfo)),
					"get surface info Failure !"
				);
			sSurfaceInfo.m_uAlpha = uAlpha;
			sSurfaceInfo.m_nAlphaFlag = 1;
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGSetSurfaceInfo(hSurface_D,&sSurfaceInfo)),
					"set surface info Failure !"
				);
		}
		
		CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_SUCCESS == CSUDIOSGFill(hSurface_D, CSUDI_NULL, OSG_COLOR_BLUE_32BIT)),
					"Fill Display Failure !"
				);	
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
		
		CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_SUCCESS == CSUDIOSGFill(hSurface_S, CSUDI_NULL, OSG_COLOR_RED_32BIT)),
					"Fill Src surface Failure !"
				);	
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
		
		nRet = CSUDIOSGBlit(hSurface_D, CSUDI_NULL, hSurface_S,CSUDI_NULL, eTmpMode);
		CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS),"Blit Failure !");
		
		//FISRT SYNC
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
		
		if(bWithAlpha)
		{
			CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_TRUE == OSG_iCheckColorAndAlpha(hSurface_D,
										uSrcFillColor,
										uDstFillColor,
										uAlpha,
										uAlpha,
										ePixelFormat)),
						"check alpha and color failure"
					);
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_TRUE == OSG_iCheckExistColor(hSurface_D,uSrcFillColor,CSUDI_NULL,ePixelFormat)),
						"First time Check Surface color Failure !"
					);
		}
		dwStartTime = 0;
		dwEndTime = 0;
		for(jj = 0; jj <nSyncTimes; jj++)
		{
			dwStartTime = CSUDIPlusOSGetTickCount();
			//SECOND SYNC
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
			
			dwEndTime = CSUDIPlusOSGetTickCount();
			
			dwSyncTime = (dwEndTime - dwStartTime);
			CSTK_ASSERT_TRUE_FATAL((dwSyncTime <= SECOND_SYNC_TIME),"The Second Sync should be less 5s!");
			if(bWithAlpha)
			{
				CSTK_ASSERT_TRUE_FATAL(
							(CSUDI_TRUE == OSG_iCheckColorAndAlpha(hSurface_D,
											uRedColor,
											uBlueColor,
											uAlpha,
											uAlpha,
											ePixelFormat)),
							"check alpha and color failure"
						);
			}
			else
			{
				CSTK_ASSERT_TRUE_FATAL(
							(CSUDI_TRUE == OSG_iCheckExistColor(hSurface_D,uRedColor,CSUDI_NULL,ePixelFormat)),
							"Second time Check Surface color Failure !"
						);
			}
			
		}
	}
	
	CSTK_FATAL_POINT
		
	if(hSurface_S != CSUDI_NULL)
	{
		if(CSUDIOSGDestroySurface(hSurface_S) != CSUDI_SUCCESS)
		{
			 CSTCPrint("Destroy  surface failure !\n");
		}
		hSurface_S = CSUDI_NULL;
	}
	if(hSurface_D != CSUDI_NULL)
	{
		if(CSUDIOSGDestroySurface(hSurface_D) != CSUDI_SUCCESS)
		{
			 CSTCPrint("Destroy  surface failure !");
		}
		hSurface_D = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
}
CSUDI_BOOL OSG_iTestSyncWithDisplay(CSUDI_BOOL bWithAlpha,int nSyncTimes)
{
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hSurface_D = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;	
	int ii = 0,jj;
	unsigned int dwStartTime = 0;
	unsigned int dwEndTime   = 0;
	unsigned int dwSyncTime = 0;
	CSUDIOSGPixelFormat_E ePixelFormat;
	CSUDIOSGOpMode_E eTmpMode = bWithAlpha ? EM_UDIOSG_OPMODE_ALPHA : EM_UDIOSG_OPMODE_COPY;
	
	unsigned int uSrcFillColor = 0;
	unsigned int uDstFillColor = 0;
	
	nRet = CSUDIOSGGetDisplaySurface(g_nTestDisplaySurfaceIndex,&hSurface_D);
	CSTK_ASSERT_TRUE_FATAL(
				(nRet == CSUDI_SUCCESS && hSurface_D != CSUDI_NULL),
				"step1 Get display Surface Failure !"
			);

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_D, &sSurfaceInfo)),
				"step 2 Get display Surface info failure !"
			);	
	ePixelFormat = sSurfaceInfo.m_ePixelFormat;
	
	switch(ePixelFormat)
	{
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_8888:
				uSrcFillColor = OSG_COLOR_RED_32BIT;
				uDstFillColor = OSG_COLOR_BLUE_32BIT;
				break;
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_1555:
		case EM_UDIOSG_PIXEL_FORMAT_RGBA_5551:
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_4444:
		case EM_UDIOSG_PIXEL_FORMAT_RGB_565:
				uSrcFillColor = OSG_COLOR_RED_16BIT;
				uDstFillColor = OSG_COLOR_BLUE_16BIT;
				break;
		break;
		default:
			break;
	}
	nRet = CSUDIOSGCreateSurface(sSurfaceInfo.m_ePixelFormat,
									sSurfaceInfo.m_nWidth,
									sSurfaceInfo.m_nHeight,
									&hSurface_S
									);
	CSTK_ASSERT_TRUE_FATAL(
				(nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),
				"create  Surface Failure !"
			);
	for (ii = 0; ii < TEST_SYNC_TIMES; ii++ )
	{
		if(bWithAlpha)
		{
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_S, &sSurfaceInfo)),
					"get surface info Failure !"
				);
			sSurfaceInfo.m_uAlpha = OSGUDI2_32BIT_ALPHA_1;
			sSurfaceInfo.m_nAlphaFlag = 1;
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGSetSurfaceInfo(hSurface_S,&sSurfaceInfo)),
					"set surface info Failure !"
				);
		}
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGFill(hSurface_D, CSUDI_NULL, OSG_COLOR_BLUE_32BIT)),
					"Fill Display Failure !"
				);	
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
		
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGFill(hSurface_S, CSUDI_NULL, OSG_COLOR_RED_32BIT)),
					"Fill Src surface Failure !"
				);	
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
		
		nRet = CSUDIOSGBlit(hSurface_D, CSUDI_NULL, hSurface_S,CSUDI_NULL, eTmpMode);
		CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS),"Blit Failure !");
		//FISRT SYNC
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
		
		if(bWithAlpha)
		{
			CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_TRUE == OSG_iCheckColorAndAlpha(hSurface_D,
										uSrcFillColor,
										uDstFillColor,
										OSGUDI2_32BIT_ALPHA_1,
										OSGUDI2_32BIT_ALPHA_1,
										ePixelFormat)),
						"check alpha and color failure"
					);
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_TRUE == OSG_iCheckExistColor(hSurface_D,uSrcFillColor,CSUDI_NULL,ePixelFormat)),
						"Check Surface color Failure !"
					);
		}

		for(jj = 0; jj <nSyncTimes; jj++)
		{
		
			dwStartTime = 0;
			dwEndTime = 0;
			
			dwStartTime = CSUDIPlusOSGetTickCount();
			//SECOND SYNC
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
			
			dwEndTime = CSUDIPlusOSGetTickCount();
			
			dwSyncTime = ((dwEndTime - dwStartTime)/1000);
			CSTK_ASSERT_TRUE_FATAL((dwSyncTime <= SECOND_SYNC_TIME),"The Second Sync should be less 5s!");
			if(bWithAlpha)
			{
				CSTK_ASSERT_TRUE_FATAL(
							(CSUDI_TRUE == OSG_iCheckColorAndAlpha(hSurface_D,
											uSrcFillColor,
											uDstFillColor,
											OSGUDI2_32BIT_ALPHA_1,
											OSGUDI2_32BIT_ALPHA_1,
											ePixelFormat)),
							"check alpha and color failure"
						);
			}
			else
			{
				CSTK_ASSERT_TRUE_FATAL(
							(CSUDI_TRUE == OSG_iCheckExistColor(hSurface_D,uSrcFillColor,CSUDI_NULL,ePixelFormat)),
							"Check Surface color Failure !"
						);
			}
			
		}
	}
	
	CSTK_FATAL_POINT
	if(hSurface_S != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface_S)),
			"Destroy  surface failure !"
			);
		hSurface_S = CSUDI_NULL;
	}	
	return CSUDI_TRUE;
}
CSUDI_BOOL OSG_iTestBlitWithSameSurface(CSUDIOSGPixelFormat_E ePixelFormat,OSGUDI2Rect_E eSRect,OSGUDI2Rect_E eDRect,CSUDI_BOOL bValidRect)
{
	CSUDI_HANDLE hSurface = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDIOSGRect_S sDRect;
	CSUDIOSGRect_S sSRect;
	unsigned int unColor = OSG_COLOR_RED_32BIT;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);

	if (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)
	{
		unColor = OSG_COLOR_RED_16BIT;
	}
		
	nRet = CSUDIOSGCreateSurface(ePixelFormat,SURFACE_WIDTH_720,SURFACE_HEIGHT_576,&hSurface);
	CSTK_ASSERT_TRUE_FATAL(
				(nRet == CSUDI_SUCCESS && hSurface != CSUDI_NULL),
				"Step 1 Create P1_1 Surface Failure !"
			);
	
	//*****************************************************************//
	OSG_iFillRectExt(&sSRect,0,0,SURFACE_WIDTH_720,SURFACE_HEIGHT_576,eSRect);
	OSG_iFillRectExt(&sDRect,0,0,SURFACE_WIDTH_720,SURFACE_HEIGHT_576,eDRect);
	//*****************************************************************//
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGFill(hSurface, CSUDI_NULL, 0)),
				"Fill Failure !"
			);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGFill(hSurface, &sSRect, OSG_COLOR_RED_32BIT)),
				"Fill Failure !"
			);
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGSync()),
				"Step 2 Sync Failure !"
			);
	
	nRet = CSUDIOSGBlit(hSurface, &sDRect, hSurface, &sSRect, EM_UDIOSG_OPMODE_COPY);
	do{
		if(bValidRect)
		{
			if(CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED == nRet) break;
			
			CSTK_ASSERT_TRUE_FATAL(
						((nRet == CSUDI_SUCCESS)),
						"Blit Failure !"
					);
			CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_SUCCESS == CSUDIOSGSync()),
						"Sync Failure !"
					);
			CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_TRUE == OSG_iCheckExistColor(hSurface, unColor, &sDRect, ePixelFormat)),
						"Check Surface color Failure !"
					);
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(
						(nRet == CSUDIOSG_ERROR_BAD_PARAMETER || CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED == nRet),
						"Blit should be failure !"
					);
		}
	}while(0);
	
	CSTK_FATAL_POINT
		
	if(hSurface != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface)),
				"Destroy P1_1 surface failure !"
			);
		hSurface = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;		
}
CSUDI_BOOL OSG_iTestBlitWithSameDisplay(OSGUDI2Rect_E eSRect,OSGUDI2Rect_E eDRect,CSUDI_BOOL bValidRect)
{
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDIOSGRect_S sDRect;
	CSUDIOSGRect_S sSRect;
	CSUDI_Error_Code nRet;
	CSUDIOSGSurfaceInfo_S sSurInfo;
	unsigned int uExceptColor = 0;
	
	nRet = CSUDIOSGGetDisplaySurface(g_nTestDisplaySurfaceIndex,&hSurface_S);
	CSTK_ASSERT_TRUE_FATAL(
				(nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),
				"Step 1 get display surface Failure !"
			);

	memset(&sSurInfo, 0, sizeof(sSurInfo));
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_S, &sSurInfo)),
			"get surfaceInfo failure !");
		
	OSG_iFillRectExt(&sSRect,0,0,0,0,eSRect);
	OSG_iFillRectExt(&sDRect,0,0,0,0,eDRect);
	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGFill(hSurface_S, CSUDI_NULL, 0)),
				"Step 2 fill surface Failure !"
			);
	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGSync()),
				"Step 3 Sync Failure !"
			);
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGFill(hSurface_S, &sSRect, OSG_COLOR_RED_32BIT)),
				"Fill Failure !"
			);
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGSync()),
				"Sync Failure !"
			);
	
	nRet = CSUDIOSGBlit(hSurface_S, &sDRect, hSurface_S, &sSRect, EM_UDIOSG_OPMODE_COPY);
	do{
		if(bValidRect)
		{
			if(CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED == nRet) break;
			
			CSTK_ASSERT_TRUE_FATAL(
						((nRet == CSUDI_SUCCESS)),
						"Blit Failure !"
					);
			CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_SUCCESS == CSUDIOSGSync()),
						"Sync Failure !"
					);

			if (sSurInfo.m_ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)
			{
				uExceptColor = OSG_iColor32BitTo16Bit(OSG_COLOR_RED_32BIT);
			}
			else
			{
				uExceptColor = OSG_COLOR_RED_32BIT;
			}
			
			CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_TRUE == OSG_iCheckExistColor(hSurface_S,uExceptColor,&sDRect,sSurInfo.m_ePixelFormat)),
						"Check Surface color Failure !"
					);
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(
						(nRet == CSUDIOSG_ERROR_BAD_PARAMETER  || CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED == nRet),
						"Blit should be failure !"
					);
		}
	}while(0);
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

CSUDI_BOOL OSG_iTESTBlitPerformance(CSUDIOSGPixelFormat_E ePixelFormat,CSUDIOSGRect_S asSRect[],CSUDIOSGRect_S asDRect[],int nRectLen,CSUDI_BOOL bWithAlpha)
{
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hSurface_D = CSUDI_NULL;
	CSUDI_Error_Code nRet;
	int ii = 0;
	unsigned int dwStartTime = 0;
	unsigned int dwEndTime   = 0;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;
	
	CSUDIOSGOpMode_E eTmpMode = bWithAlpha ? EM_UDIOSG_OPMODE_ALPHA : EM_UDIOSG_OPMODE_COPY;
	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);
	
	nRet = CSUDIOSGCreateSurface(ePixelFormat,
									SURFACE_WIDTH_720,
									SURFACE_HEIGHT_576,
									&hSurface_S
									);
	CSTK_ASSERT_TRUE(
				(nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),
				"Create P1_1 Surface Failure !"
			);	
	
	nRet = CSUDIOSGCreateSurface(ePixelFormat,
									SURFACE_WIDTH_720,
									SURFACE_HEIGHT_576,
									&hSurface_D
									);
	CSTK_ASSERT_TRUE(
			(nRet == CSUDI_SUCCESS && hSurface_D != CSUDI_NULL),
			"Create P2_1 Surface Failure !"
		);	
	
	if(bWithAlpha)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_S, &sSurfaceInfo)),
				"get surface info Failure !"
			);
		sSurfaceInfo.m_uAlpha = 0x7f;
		sSurfaceInfo.m_nAlphaFlag = 1;
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGSetSurfaceInfo(hSurface_S,&sSurfaceInfo)),
				"set surface info Failure !"
			);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_D, &sSurfaceInfo)),
				"get surface info Failure !"
			);
		sSurfaceInfo.m_uAlpha = 0x7f;
		sSurfaceInfo.m_nAlphaFlag = 1;
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGSetSurfaceInfo(hSurface_D,&sSurfaceInfo)),
				"set surface info Failure !"
			);
	}
	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGFill(hSurface_D, CSUDI_NULL, 0)),
				"Fill Failure !"
			);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
	
	//���Դ��ͼƽ��ΪCOLOR_RED_32BIT��ɫ
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGFill(hSurface_S, CSUDI_NULL, OSG_COLOR_RED_32BIT)),
				"Fill Failure !"
			);
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");

	CSTCPrint("\n Begin Test blit performance between cache surface \n");
	for (ii = 0; ii < nRectLen; ii++ )
	{
		dwStartTime = CSUDIPlusOSGetTickCount();
		nRet = CSUDIOSGBlit(hSurface_D, &asDRect[ii], hSurface_S, &asSRect[ii], eTmpMode);
		CSTK_ASSERT_TRUE((nRet == CSUDI_SUCCESS),"Blit Failure !");
		
		CSTK_ASSERT_TRUE((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
		
		dwEndTime = CSUDIPlusOSGetTickCount();
		
		CSTCPrint("\t PerformaceTest blit %s , [%d*%d] ,-->, [%d*%d] , %d (ms)\n",
					bWithAlpha?"Alpha":"Copy", asSRect[ii].m_nWidth, asSRect[ii].m_nHeight,asDRect[ii].m_nWidth, asDRect[ii].m_nHeight,((dwEndTime - dwStartTime)));
	}
	CSTCPrint("\n End Test blit performance between cache surface\n");
	
	CSTK_FATAL_POINT
		
	if(hSurface_S != CSUDI_NULL)
	{
		if(CSUDIOSGDestroySurface(hSurface_S) != CSUDI_SUCCESS)
		{
			CSTCPrint("Destroy P1_1 Surface Failure !\n");	
		}
		
		hSurface_S = CSUDI_NULL;
	}	
	if(hSurface_D != CSUDI_NULL)
	{
		if(CSUDIOSGDestroySurface(hSurface_D) != CSUDI_SUCCESS)
		{
			CSTCPrint("Destroy P2_1 Surface Failure !");	
		}
		
		hSurface_D = CSUDI_NULL;
	}	

	return CSUDI_TRUE;
}

CSUDI_BOOL OSG_iTESTBlitPerformanceWithDisplay(CSUDIOSGRect_S asSRect[],CSUDIOSGRect_S asDRect[],int nRectLen,CSUDI_BOOL bWithAlpha)
{
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hSurface_D = CSUDI_NULL;
	CSUDI_Error_Code nRet;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;	
	int ii = 0;
	unsigned int dwStartTime = 0;
	unsigned int dwEndTime = 0;
	CSUDIOSGOpMode_E eTmpMode = bWithAlpha ? EM_UDIOSG_OPMODE_ALPHA : EM_UDIOSG_OPMODE_COPY;
	
	nRet = CSUDIOSGGetDisplaySurface(g_nTestDisplaySurfaceIndex,&hSurface_D);
	CSTK_ASSERT_TRUE_FATAL(
				(nRet == CSUDI_SUCCESS && hSurface_D != CSUDI_NULL),
				"Get display Surface Failure !"
			);

	memset(&sSurfaceInfo, 0, sizeof(sSurfaceInfo));
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_D, &sSurfaceInfo)),
				"Get display Surface info failure !"
			);	

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGFill(hSurface_D, CSUDI_NULL, 0)),
				"Fill Failure !"
			);	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
	nRet = CSUDIOSGCreateSurface(sSurfaceInfo.m_ePixelFormat,
									sSurfaceInfo.m_nWidth,
									sSurfaceInfo.m_nHeight,
									&hSurface_S
									);
	CSTK_ASSERT_TRUE_FATAL(
				(nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),
				"create  Surface Failure !"
			);
	if(bWithAlpha)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_S, &sSurfaceInfo)),
				"get surface info Failure !"
			);
		sSurfaceInfo.m_uAlpha = 0x7f;
		sSurfaceInfo.m_nAlphaFlag = 1;
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGSetSurfaceInfo(hSurface_S,&sSurfaceInfo)),
				"set surface info Failure !"
			);
	}
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGFill(hSurface_S, CSUDI_NULL, OSG_COLOR_RED_32BIT)),
				"Fill Failure !"
			);	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
	
	CSTCPrint("\n Begin Test blit performance between cache surface and display surface  \n");
	for (ii = 0; ii < nRectLen; ii++ )
	{
		dwStartTime = CSUDIPlusOSGetTickCount();
		nRet = CSUDIOSGBlit(hSurface_D, &asDRect[ii], hSurface_S, &asSRect[ii], eTmpMode);
		CSTK_ASSERT_TRUE((nRet == CSUDI_SUCCESS),"Blit Failure !");
		
		CSTK_ASSERT_TRUE((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
		
		dwEndTime = CSUDIPlusOSGetTickCount();
		CSTCPrint("\t PerformaceTest blit %s , [%d*%d] ,-->, [%d*%d] , %d (ms)\n",
						bWithAlpha?"Alpha":"Copy", asSRect[ii].m_nWidth, asSRect[ii].m_nHeight,asDRect[ii].m_nWidth, asDRect[ii].m_nHeight,((dwEndTime - dwStartTime)));
	}
	CSTCPrint("\n End Test blit performance between cache surface and display surface \n");
	
	CSTK_FATAL_POINT
		
	if(hSurface_S != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface_S)),
				"Destroy  surface failure !"
			);
		
		hSurface_S = CSUDI_NULL;
	}	
	
	return CSUDI_TRUE;
}

//�˺������������ƶ���Ч������
CSUDI_BOOL OSG_iTESTPortalPerformance(CSUDIOSGOpMode_E eOpMode, unsigned int	unForeground_color)
{

	int nWidth, nHeight;
	int nBgWidth, nBgHeight;
	int	i;

	CSUDI_HANDLE hBgSurface, hForeSurface, hDisplaySurface;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;	

	CSUDIOSGRect_S sDstRect, sSrcRect,sBgRect;
	CSUDIOSGSurfaceInfo_S  sSurfaceInfo;
	int nStep = 2;//move�Ĳ���
	unsigned int dwStartTime = 0;
	unsigned int dwEndTime   = 0;		
	
	nWidth = 100;
	nHeight = 100;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(g_nTestDisplaySurfaceIndex, &hDisplaySurface),
		"step 1 ��ȡ��ʾsurfaceʧ��!\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hDisplaySurface, &sSurfaceInfo),
		"��ȡ��ʾsurface��Ϣʧ��!\n");
	
	nBgWidth = sSurfaceInfo.m_nWidth;
	nBgHeight = sSurfaceInfo.m_nHeight;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGCreateSurface(ePixelFormat,nBgWidth, nBgHeight, &hBgSurface),
		"step 2 ����hBgSurfaceʧ��!\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGCreateSurface(ePixelFormat,nWidth, nHeight, &hForeSurface),
		"step 3 ����hForeSurfaceʧ��!\n");

	//����Դ�Ϊ��ɫ

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGFill( hDisplaySurface, NULL, 0xFFFFFFFF),
		"����Դ�ʧ��!\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGFill( hForeSurface, NULL, unForeground_color),
		"step 4 ���hForeSurfaceʧ��!\n");

	sSrcRect.m_nX = 0;
	sSrcRect.m_nY = 0;
	sSrcRect.m_nWidth = nWidth;
	sSrcRect.m_nHeight=	nHeight;
	
	sDstRect.m_nX = 0;
	sDstRect.m_nY = 0;
	sDstRect.m_nWidth = nWidth;
	sDstRect.m_nHeight=	nHeight;

	sBgRect.m_nX = 0;
	sBgRect.m_nY = 0;
	sBgRect.m_nWidth = nBgWidth;
	sBgRect.m_nHeight=	nBgHeight;
	
	//���Դ�����򱣴���hBgSurface,ʵ�������������򲻻ᱣ��ȫ���ģ������ü�
	//��Ϊ��������������ȫ������	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGBlit(hBgSurface , &sBgRect, hDisplaySurface , &sBgRect,  eOpMode),
		"step 5 blitʧ��!\n");	

	CSUDIOSGSync();

	dwStartTime = CSUDIPlusOSGetTickCount();	
	
	for(i = 0; i< 200; i++)
	{
		//��hForeSurface�����ݻ��Ƶ��Դ�
		sSrcRect.m_nX = sSrcRect.m_nY = 0;
		sDstRect.m_nX = sDstRect.m_nY = i * nStep;
		sSrcRect.m_nWidth  = sDstRect.m_nWidth= nWidth;
		sSrcRect.m_nHeight = sDstRect.m_nHeight = nHeight;
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGBlit(hDisplaySurface , &sDstRect, hForeSurface , &sSrcRect,  eOpMode),
			"step 6 blit ʧ��!\n");

		//�ָ����������ݵ��Դ�

		sSrcRect.m_nX = sDstRect.m_nX = i * nStep;
		sSrcRect.m_nY = sDstRect.m_nY = i * nStep;
		
		sSrcRect.m_nWidth  = sDstRect.m_nWidth= nWidth;
		sSrcRect.m_nHeight = sDstRect.m_nHeight = nStep;
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGBlit(hDisplaySurface , &sDstRect, hBgSurface , &sSrcRect,  EM_UDIOSG_OPMODE_COPY),
			"step 7 blitʧ��!\n");

		sSrcRect.m_nX = sDstRect.m_nX = i * nStep;
		sSrcRect.m_nY = sDstRect.m_nY = i * nStep + nStep;
		
		sSrcRect.m_nWidth  = sDstRect.m_nWidth= nStep;
		sSrcRect.m_nHeight = sDstRect.m_nHeight = nHeight - nStep;
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGBlit(hDisplaySurface , &sDstRect, hBgSurface , &sSrcRect,  EM_UDIOSG_OPMODE_COPY), 
			"step 8 blit ʧ��!\n");
		
		CSUDIOSGSync();
	}
	
	dwEndTime = CSUDIPlusOSGetTickCount();
	
	CSTCPrint("\n \tInfo:   The time of portal  is  %d (ms)\n", (dwEndTime - dwStartTime));

	CSTK_FATAL_POINT;

	if (hForeSurface != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hForeSurface)),"step 12 Destroy hForeSurface failure!");
		hForeSurface = CSUDI_NULL;
	}

	if (hBgSurface != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hBgSurface)),"step 12 Destroy hBgSurface failure!");
		hBgSurface = CSUDI_NULL;
	}
	//����
 	OSG_iClearScreen();	
	return CSUDI_TRUE;
}

CSUDI_BOOL OSG_iTESTFillPerformace(CSUDIOSGPixelFormat_E ePixelFormat,CSUDIOSGRect_S asRect[],int nRectLen)
{
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	int ii = 0;
	unsigned int dwStartTime = 0;
	unsigned int dwEndTime   = 0;
	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);
		
	nRet = CSUDIOSGCreateSurface(ePixelFormat,
									SURFACE_WIDTH_720,
									SURFACE_HEIGHT_576,
									&hSurface_S
									);
	CSTK_ASSERT_TRUE_FATAL(
				(nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),
				"create  Surface Failure !"
			);
	
	CSTCPrint("\n Begin Test fill performance on cache surface \n");
	for (ii = 0; ii < nRectLen; ii++ )
	{
		dwStartTime = CSUDIPlusOSGetTickCount();			
		CSTK_ASSERT_TRUE(
					(CSUDI_SUCCESS == CSUDIOSGFill(hSurface_S, &asRect[ii], OSG_COLOR_RED_32BIT)),
					"Fill Failure !"
				);
		CSTK_ASSERT_TRUE((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
		
		dwEndTime = CSUDIPlusOSGetTickCount();

		CSTCPrint("\n \tInfo:   The time of fill area [%d * %d]  is  %d (ms)\n",
			asRect[ii].m_nWidth, asRect[ii].m_nHeight, (dwEndTime - dwStartTime));
	}
	CSTCPrint("\n End Test fill performance on cache surface \n");

	CSTK_FATAL_POINT
		
	if(hSurface_S != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface_S)),
				"Destroy P1_1 surface failure !"
			);
		
		hSurface_S = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
}
CSUDI_BOOL OSG_iTESTFillPerformaceWithDisplay(CSUDIOSGRect_S asRect[],int nRectLen)
{
	CSUDI_HANDLE hSurface_D = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE; 
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;
	int ii = 0;
	unsigned int dwStartTime = 0;
	unsigned int dwEndTime   = 0;

	nRet = CSUDIOSGGetDisplaySurface(g_nTestDisplaySurfaceIndex,&hSurface_D);
	CSTK_ASSERT_TRUE_FATAL(
				(nRet == CSUDI_SUCCESS && hSurface_D != CSUDI_NULL),
				"Get display Surface Failure !"
			);

	memset(&sSurfaceInfo, 0, sizeof(sSurfaceInfo));
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_D, &sSurfaceInfo)),
				"Get display Surface info failure !"
			);	

	CSTCPrint("\n Begin Test fill performance on display surface \n");
	for (ii = 0; ii < nRectLen; ii++ )
	{
		dwStartTime = CSUDIPlusOSGetTickCount();
		
		CSTK_ASSERT_TRUE(
					(CSUDI_SUCCESS == CSUDIOSGFill(hSurface_D, &asRect[ii], OSG_COLOR_RED_32BIT)),
					"Fill Failure !"
				);
		CSTK_ASSERT_TRUE((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");

		dwEndTime = CSUDIPlusOSGetTickCount();
		
		CSTCPrint("\n\tInfo:   The time of fill area [%d * %d]  is  %d (ms)\n",
			asRect[ii].m_nWidth, asRect[ii].m_nHeight, (dwEndTime - dwStartTime));
	}
	CSTCPrint("\n End Test fill performance on display surface \n");	
	
	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

/**
@brief �ж�ƽ̨�Ƿ�֧�ָ���ɫģʽ���������ļ��ṩ

@param[in] eFixelFormat ��ͼƽ�����ɫģʽ�����CSUDIOSGPixelFormat_E

@return ���ƽ̨֧�ִ�����Ӧ����ɫģʽ���򷵻�CSUDI_TRUE��ƽ̨��֧�ָ���ɫģʽ�򷵻�CSUDI_FALSE
*/
CSUDI_BOOL OSG_IsSupportPixelFormat(CSUDIOSGPixelFormat_E ePixelFormat )
{
	return g_bPixelFormatCfg[ePixelFormat];
}

/**
@brief �ж�ƽ̨�Ƿ�֧�ִ���1280*720�Ļ�ͼƽ��

@return ���ƽ̨֧�ִ������򷵻�CSUDI_TRUE��ƽ̨��֧���򷵻�CSUDI_FALSE
*/
CSUDI_BOOL OSG_IsSupport1280_720(void)
{
	return g_bSupportSurface_1280_720;
}

/**
@brief �ж�ƽ̨�Ƿ�֧�ִ���1920*1080�Ļ�ͼƽ��

@return ���ƽ̨֧�ִ������򷵻�CSUDI_TRUE��ƽ̨��֧���򷵻�CSUDI_FALSE
*/
CSUDI_BOOL OSG_IsSupport1920_1080(void)
{
	return g_bSupportSurface_1920_1080;
}

/**
@brief �ж�ƽ̨�Ƿ�֧�ָ߱����Դ�ͬʱ����

@return ���ƽ̨֧�֣��򷵻�CSUDI_TRUE��ƽ̨��֧���򷵻�CSUDI_FALSE
*/
CSUDI_BOOL OSG_IsSupportSDHDDisplay(void)
{
	return g_bSupportHdSdDisplay;
}

/**
@brief �ж�ƽ̨�Ƿ�֧�ָ߱����Դ�ͬʱ����

@return ���ƽ̨֧�֣��򷵻�CSUDI_TRUE��ƽ̨��֧���򷵻�CSUDI_FALSE
*/
CSUDI_BOOL OSG_IsSupportDecImage(OSGUDI2SupportDecIamgeType_E eType)
{
	return g_bSupportDecImageType[eType];
}

/**
@brief �жϸ߱����Ƿ�ͬԴ��������߱��干��һ�������Դ�

@return ���ƽ̨֧�֣��򷵻�CSUDI_TRUE��ƽ̨��֧���򷵻�CSUDI_FALSE
*/
CSUDI_BOOL OSG_IsShareHDDisplaySurface(void)
{
	return g_bShareHdDispalySurface;
}

/**
@brief ��ȡ��ͼ����ʹ�õ��Դ�����

@return ��ͼ����ʹ�õ��Դ�����
*/
int OSG_GetTestDisplaySurfaceIndex(void)
{
	return g_nTestDisplaySurfaceIndex;
}

/**
 ������
*/
void OSG_iFillRect(CSUDIOSGRect_S * psRect,int nX,int nY,int nWidth,int nHeight)
{
	if(psRect == CSUDI_NULL) return ;
	psRect->m_nX = nX;
	psRect->m_nY = nY;
	psRect->m_nWidth = nWidth;
	psRect->m_nHeight = nHeight;
	return;
}
/**
 ����eRect�����������Σ�eRectö��ֵ����Ӧ��ֵ��ο�CSTC_OSG_IT_Blit_0000˵��
*/
void OSG_iFillRectExt(CSUDIOSGRect_S * psRect,int nX,int nY,int nWidth,int nHeight,OSGUDI2Rect_E eRect)
{
	if(psRect == CSUDI_NULL) return ;
	
	switch(eRect)
	{
		case EM_RECT_1:
			return;
		case EM_RECT_2:
			nX = 0;
			nY = 0;
			nWidth = nWidth;
			nHeight = nHeight;
			break;
		case EM_RECT_3:	
			nX = nWidth/2-200;
			nY = nHeight/2-200;     
			nWidth = nWidth/2+100;  	
			nHeight = nHeight/2+100;		
			break;
		case EM_RECT_4:
			nX = 0;
			nY = 0;  
			nWidth = 0;  	
			nHeight = 0;
			break;
		case EM_RECT_5:
			nX = 0;
			nY = 0;    
			nWidth = -1; 	
			nHeight = -1;
			break;
		case EM_RECT_6:
			nX = 0;
			nY = 0;   
			nWidth = 1; 	
			nHeight = 1;
			break;
		case EM_RECT_7:
			nX = 0;
			nY = 0;     
			nWidth = 15;  	
			nHeight = 15;
			break;
		case EM_RECT_8:
			nX = 0;
			nY = nHeight-100;     
			nWidth = 100;  	
			nHeight = nHeight+100;
			break;
		case EM_RECT_9:
			nX = nWidth/2;
			nY = nHeight-100;     
			nWidth = nWidth/2+100;  	
			nHeight = nHeight+100;
			break;
		case EM_RECT_10:
			nX = nWidth-100;
			nY = nHeight-100;     
			nWidth = nWidth+100;  	
			nHeight = nHeight+100;
			break;
		case EM_RECT_11:
			nX = nWidth-100;
			nY = nHeight/2-100;     
			nWidth = nWidth+100;  	
			nHeight = nHeight/2+100;
			break;
		case EM_RECT_12:
			nX = nWidth-100;
			nY = 0;     
			nWidth = nWidth+100;  	
			nHeight = 100;
			break;
		case EM_RECT_13:
			nX = 2;
			nY = 2;     
			nWidth = 17;  	
			nHeight = 17;
			break;
		case EM_RECT_14:
			nX = -11;
			nY = -11;     
			nWidth = 100;  	
			nHeight = 100;
			break;
		case EM_RECT_15:
			nX = -11;
			nY = nHeight/2-100;     
			nWidth = 100;  	
			nHeight = nHeight/2+100;
			break;
		case EM_RECT_16:
			nX = nWidth/2-100;
			nY = -11;     
			nWidth = nWidth/2+100;  	
			nHeight = 100;
			break;	
		case EM_RECT_17:
			nX = nWidth>>2;
			nY = nWidth>>2;     
			nWidth = nWidth>>1;  	
			nHeight = nHeight>>1;
			break;	
		case EM_RECT_18:
			nX = (nWidth>>2) + 10;
			nY = (nWidth>>2) + 10;     
			nWidth = nWidth>>1;  	
			nHeight = nHeight>>1;
			break;	
		case EM_RECT_19:
			nX = nWidth>>2;
			nY = nWidth>>2;     
			nWidth = (nWidth>>1) + 1;  	
			nHeight = (nHeight>>1) + 1;
			break;	
		case EM_RECT_20:
			nX = 10;
			nY = 10;     
			nWidth = 50;  	
			nHeight = 4000;
			break;	
		case EM_RECT_21:
			nX = 10;
			nY = 10;     
			nWidth = 4000;  	
			nHeight = 50;
			break;	
		case EM_RECT_22:
			nX = 10;
			nY = 10;     
			nWidth = 4000;  	
			nHeight = 50;
			break;	
		case EM_RECT_23:
			nX = (nWidth>>2) + 10;
			nY = (nWidth>>2) + 10;     
			nWidth = (nWidth>>1) + 1;  	
			nHeight = (nHeight>>1) + 1;
			break;			
			
		default:
			break;
	}

	psRect->m_nX = nX;
	psRect->m_nY = nY;
	psRect->m_nWidth = nWidth;
	psRect->m_nHeight = nHeight;
	return;
}

CSUDI_BOOL OSG_iCheckSDDisplayInfo(CSUDI_HANDLE hSurface)
{
	return OSG_iCheckSurfaceInfo(hSurface,
				g_eDisplayPixelFormat,   //�Դ���ɫģʽ
				g_nSDSurfaceWidth,     //�Դ��С
				g_nSDSurfaceHeight);   //�Դ��С
}

CSUDI_BOOL OSG_iCheckHDDisplayInfo(CSUDI_HANDLE hSurface)
{
	return OSG_iCheckSurfaceInfo(hSurface,
				g_eDisplayPixelFormat,   //�Դ���ɫģʽ
				g_nHDSurfaceWidth,     //�Դ��С
				g_nHDSurfaceHeight);   //�Դ��С
}

typedef struct _Set_SurfaceInfo_S
{
	int m_nAlphaFlag; 
	unsigned int m_uAlpha; 
	int m_nColorKeyFlag; 
   	unsigned int m_uColorkey;
}Set_SurfaceInfo_S;

/*
���û�ͼƽ�����Ϣ
*/
CSUDI_BOOL OSG_iSetSurfaceInfo(CSUDI_HANDLE hSurface)
{
	Set_SurfaceInfo_S s_aSurfaceInfo[] = {
		{0, 0, 0, 0},
		{0, 255, 0, 0},
		{0, 0, 0, 0xffffffff},
		{0, 255, 0, 0xffffffff},
		{1, 0, 0, 0},
		{1, 255, 0, 0,},
		{1, 0, 0, 0xffffffff},
		{1, 255, 0, 0xffffffff},
		{0, 0, 0, 1},
		{0, 255, 1, 0},
		{0, 0, 1, 0xffffffff},
		{0, 255, 1, 0xffffffff},
		{1, 0, 0, 1},
		{1, 255, 1, 0},
		{1, 0, 1, 0xffffffff},
		{1, 255, 1, 0xffffffff}
	};
	
	CSUDI_Error_Code			nResult = CSUDI_FAILURE;
	CSUDIOSGSurfaceInfo_S 		sSurfaceInfo;
	int						i = 0;

	nResult = CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo);
	if(CSUDI_SUCCESS != nResult)
	{
		return CSUDI_FALSE;
	}

	for(i = 0; i < sizeof(s_aSurfaceInfo)/sizeof(s_aSurfaceInfo[0]); i++)
	{
		// �������õ����������
		sSurfaceInfo.m_nAlphaFlag = s_aSurfaceInfo[i].m_nAlphaFlag;
		sSurfaceInfo.m_uAlpha = s_aSurfaceInfo[i].m_uAlpha;
		sSurfaceInfo.m_nColorKeyFlag = s_aSurfaceInfo[i].m_nColorKeyFlag;
		sSurfaceInfo.m_uColorkey = s_aSurfaceInfo[i].m_uColorkey;

		//�Բ������õ���������ã�ϣ�����ò�������
		sSurfaceInfo.m_nHeight = 0x11;
		sSurfaceInfo.m_nWidth = 0x11;
		sSurfaceInfo.m_pvSurfaceMem = (void*)0x11;
		sSurfaceInfo.m_nMemSize = 0x11;
		
		//���SetSurfaceInfo ��������µ����е�������
		CSTCPrint("m_nAlphaFlag:%u, m_uAlpha:%u, m_nColorKeyFlag:%u, m_uColorkey:%u\n",
				sSurfaceInfo.m_nAlphaFlag,sSurfaceInfo.m_uAlpha,
				sSurfaceInfo.m_nColorKeyFlag,sSurfaceInfo.m_uColorkey);
		if(!OSG_iCheckSetSurfaceInfo(hSurface, sSurfaceInfo))
		{
			return CSUDI_FALSE;
		}
	}

	return CSUDI_TRUE;
}

/**
@brief ���SetSurfaceInfo �����Ƿ�����
@return ������Ӧ����ֵ
*/
CSUDI_BOOL OSG_iCheckSetSurfaceInfo(CSUDI_HANDLE hSurface, CSUDIOSGSurfaceInfo_S sSurfaceInfo)
{
	CSUDI_Error_Code			nResult = CSUDI_FAILURE;
	CSUDIOSGSurfaceInfo_S 		sSurfaceInfo_get;

	//����surfaceInfo
	nResult = CSUDIOSGSetSurfaceInfo(hSurface, &sSurfaceInfo);
	
	//�ж�����Ҫ���óɹ���������Ƿ����óɹ�
	nResult = CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo_get);
		
	if(CSUDI_SUCCESS == nResult)
	{	
		if((sSurfaceInfo_get.m_nAlphaFlag != sSurfaceInfo.m_nAlphaFlag)||
			(sSurfaceInfo_get.m_uAlpha != sSurfaceInfo.m_uAlpha)||
			(sSurfaceInfo_get.m_nColorKeyFlag != sSurfaceInfo.m_nColorKeyFlag)||
			(sSurfaceInfo_get.m_uColorkey != sSurfaceInfo.m_uColorkey))
		{	
			CSTCPrint("���õ�surface ��Ԥ�ڲ���\n");
			return CSUDI_FALSE;
		}
		if((sSurfaceInfo_get.m_nHeight == 0x11)||
			(sSurfaceInfo_get.m_nWidth == 0x11)||
			(sSurfaceInfo_get.m_pvSurfaceMem == (void*)0x11)||
			(sSurfaceInfo_get.m_nMemSize == 0x11))
		{
			CSTCPrint("�������õ�surface �������\n");
			return CSUDI_FALSE;
		}
	}
	else
	{
		CSTCPrint("��ȡsurface��Ϣʧ��, nResult=0x%x\n", nResult);
		return CSUDI_FALSE;
	}

	return CSUDI_TRUE;
}

/*
*��֤��ͼƽ�����Ϣ����������Ϣ�Ƿ�һ��
*�˴�����ȫ����֤CSUDIOSGGetSurfaceInfo����������µ��������
*/
CSUDI_BOOL OSG_iCheckSurfaceInfo(CSUDI_HANDLE hSurface,int nExpectPixelFormat, int nExpectWidth, int nExpectHeight)
{
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	
	nRet = CSUDIOSGGetSurfaceInfo(hSurface,&sSurfaceInfo);
	if(CSUDI_SUCCESS != nRet)
	{
		CSTCPrint("[%s, %d] nRet=0x%x\n", __FUNCTION__, __LINE__, nRet);
		return CSUDI_FALSE;
	}
	
	if(nExpectPixelFormat != sSurfaceInfo.m_ePixelFormat) 
	{
		CSTCPrint("[%s, %d] ExpectPixelFormat=%d, ActualPixelFormat=%d\n", \
			__FUNCTION__, __LINE__, nExpectPixelFormat, sSurfaceInfo.m_ePixelFormat);
		return CSUDI_FALSE;
	}
	if(nExpectWidth != sSurfaceInfo.m_nWidth ) 
	{
		CSTCPrint("[%s, %d] ExpectWidth=%d, ActualWidth=%d\n", \
			__FUNCTION__, __LINE__, nExpectWidth, sSurfaceInfo.m_nWidth);
		return CSUDI_FALSE;
	}
	
	if(nExpectHeight != sSurfaceInfo.m_nHeight) 
	{
		CSTCPrint("[%s, %d] ExpectHeight=%d, ActualHeight=%d\n", \
			__FUNCTION__, __LINE__, nExpectHeight, sSurfaceInfo.m_nHeight);
		return CSUDI_FALSE;
	}
	
	if(CSUDI_NULL == sSurfaceInfo.m_pvSurfaceMem) 
	{
		CSTCPrint("[%s, %d] sSurfaceInfo.m_pvSurfaceMem is null\n", __FUNCTION__, __LINE__);
		return CSUDI_FALSE;
	}
	
	if(sSurfaceInfo.m_nMemSize <= 0) 
	{
		CSTCPrint("[%s, %d] sSurfaceInfo.m_nMemSize=%d\n", __FUNCTION__, __LINE__, sSurfaceInfo.m_nMemSize);
		return CSUDI_FALSE;
	}
	
	return CSUDI_TRUE;
}

/*
Ѱ��һ��ƽ̨֧�ֵ�ʹ����ΪePixelFormat ����������ֵ�����ڲ���
����ΪEM_UDIOSG_PIXEL_FORMAT_ARGB_8888�����ƽ̨��֧����ΪEM_UDIOSG_PIXEL_FORMAT_ARGB_1555
*/
CSUDI_BOOL OSG_iFindSupportPixelFormat(CSUDIOSGPixelFormat_E *ePixelFormat)
{
	CSUDI_BOOL bRet = CSUDI_FALSE;
	int i = 0;

	*ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_NUM;
	
	for(i = 0; i < EM_UDIOSG_PIXEL_FORMAT_NUM; i++)
	{
		if(g_bPixelFormatCfg[i] && (i==EM_UDIOSG_PIXEL_FORMAT_ARGB_1555 || i==EM_UDIOSG_PIXEL_FORMAT_ARGB_8888))
		{
			*ePixelFormat = i;
		}

		if (i == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888)
		{
			break;
		}
	}

	if (*ePixelFormat==EM_UDIOSG_PIXEL_FORMAT_ARGB_1555 || *ePixelFormat==EM_UDIOSG_PIXEL_FORMAT_ARGB_8888)
	{
		bRet = CSUDI_TRUE;
	}

	CSTCPrint("Find Pixelformat : %d (%d: ARGB1555, %d: ARGB8888)\n", 
		*ePixelFormat, EM_UDIOSG_PIXEL_FORMAT_ARGB_1555, EM_UDIOSG_PIXEL_FORMAT_ARGB_8888);

	return bRet;
}

/****************************************************************************
��ȡƽ̨֧�ֵ���ɫģʽ,��������ɫģʽ��Ӧ���ֽ���
*******************************************************************************/
CSUDI_BOOL OSG_iFindSupportPixelFormatAndBytes(CSUDIOSGPixelFormat_E *ePixelFormat, int * nBytes)
{
	int i = 0;
	for(i = 0; i < EM_UDIOSG_PIXEL_FORMAT_NUM; i++)
	{
		if(g_bPixelFormatCfg[i])
		{
			*ePixelFormat = i;
			switch (i)
			{
				case EM_UDIOSG_PIXEL_FORMAT_RGB_565:
				case EM_UDIOSG_PIXEL_FORMAT_ARGB_1555:
				case EM_UDIOSG_PIXEL_FORMAT_RGBA_5551:
				case EM_UDIOSG_PIXEL_FORMAT_ARGB_4444:
				case EM_UDIOSG_PIXEL_FORMAT_RGBA_4444:
						*nBytes = 2; //����16λ����ɫģʽ
						break;
				case EM_UDIOSG_PIXEL_FORMAT_RGB_888:
						*nBytes = 3;
						break;
				case EM_UDIOSG_PIXEL_FORMAT_ARGB_8888:
						*nBytes = 4;
						break;
				case EM_UDIOSG_PIXEL_FORMAT_PALETTE8:
						*nBytes = 1; 
				case EM_UDIOSG_PIXEL_FORMAT_YCBCR888_444:
				case EM_UDIOSG_PIXEL_FORMAT_YCBCR888_422:
				case EM_UDIOSG_PIXEL_FORMAT_YCBCR888_420:
				case EM_UDIOSG_PIXEL_FORMAT_YCBCR888_411:	
						*nBytes = 3;
						break;
				default:
						CSTCPrint("the i is wrong;\n");
						break;
			}
			return CSUDI_TRUE;
		}
	}

	return CSUDI_FALSE;
}

/*
��ͼƬ��Դ����ɫ������ͼƽ��,��ʱ֧��32ģʽ
*/
CSUDI_BOOL OSG_iFillSurfaceWithFileData(CSUDI_HANDLE hSurface,unsigned char * pcFile,int nLen,int nPixelMode)
{
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	int nCopyLen = 0;
	int nSurfaceSize = 0;
	unsigned char * pcColor = pcFile;
	sSurfaceInfo.m_pvSurfaceMem = CSUDI_NULL;
	
	if(pcColor == CSUDI_NULL) return CSUDI_FALSE;
	
	nRet = CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo);	
	
	if(CSUDI_SUCCESS != nRet || sSurfaceInfo.m_pvSurfaceMem == CSUDI_NULL) 
	{
		CSASSERT(CSUDI_SUCCESS == nRet && sSurfaceInfo.m_pvSurfaceMem != CSUDI_NULL);
		return CSUDI_FALSE;	
	}
	
	memset(sSurfaceInfo.m_pvSurfaceMem, 0, sSurfaceInfo.m_nMemSize);
	
	nSurfaceSize = sSurfaceInfo.m_nMemSize;//sSurfaceInfo.m_nHeight*sSurfaceInfo.m_nWidth*4;
	
	nCopyLen = (nSurfaceSize > nLen) ? nLen : nSurfaceSize;

	memcpy(sSurfaceInfo.m_pvSurfaceMem,pcColor,nCopyLen);
	
	return CSUDI_TRUE;
}

//ARGB8888��ɫת����ARGB1555
unsigned int OSG_iColor32BitTo16Bit(unsigned int uSrcColor)
{
	unsigned char sA = 0; //Դ��ɫֵ��alphaֵ
	unsigned char sB = 0;
	unsigned char sG = 0;
	unsigned char sR = 0;
	unsigned int uResult = 0;
	
	sB = uSrcColor & 0xff;
	sG = (uSrcColor>>8) & 0xff;
	sR = (uSrcColor>>16) & 0xff;
	sA = (uSrcColor>>24) & 0xff;
	if(sA == 0)
	{
		uResult =  0x0000|(sR>>3<<10)|(sG>>3<<5)|(sB>>3);
	}
	else
	{
		uResult =  0x8000|(sR>>3<<10)|(sG>>3<<5)|(sB>>3);
	}
	return uResult;
}
//uSrcColorΪ32λARGB8888
unsigned int OSG_iGetColorWithPixelMode(unsigned int uSrcColor,CSUDIOSGPixelFormat_E nPixelMode)
{
	unsigned int uTmpColor = 0;
	switch(nPixelMode)
	{
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_8888:
		case OSG_COLOR_FORMAT_32BIT:
		{
			uTmpColor = uSrcColor;
		}
		break;
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_1555:
		case OSG_COLOR_FORMAT_16BIT:
		{
			uTmpColor =  OSG_iColor32BitTo16Bit(uSrcColor);
		}
		break;
		case EM_UDIOSG_PIXEL_FORMAT_RGBA_5551:
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_4444:
		case EM_UDIOSG_PIXEL_FORMAT_RGB_565:
		default:
			CSTCPrint("[%s, %d] Error PixelMode: %d\n", __FUNCTION__, __LINE__, nPixelMode);
			return 0;
	}
	return uTmpColor;
}
/*
��uFillColor��ɫ������ͼƽ��
*/
CSUDI_BOOL OSG_iFillSurface(CSUDI_HANDLE hSurface,unsigned int uFillColor,int nPixelMode)
{		
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;
	int ii=0;
	int jj=0;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	sSurfaceInfo.m_pvSurfaceMem = CSUDI_NULL;
	
	nRet = CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo);	
	
	if(CSUDI_SUCCESS != nRet || sSurfaceInfo.m_pvSurfaceMem == CSUDI_NULL) 
	{
		CSASSERT(CSUDI_SUCCESS == nRet && sSurfaceInfo.m_pvSurfaceMem != CSUDI_NULL);
		return CSUDI_FALSE;	
	}
	memset(sSurfaceInfo.m_pvSurfaceMem, 0, sSurfaceInfo.m_nMemSize);

	switch(nPixelMode)
	{
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_8888:
		case OSG_COLOR_FORMAT_32BIT:
		{
			unsigned int *pSurTemp = (unsigned int*)sSurfaceInfo.m_pvSurfaceMem;
			
			for(ii=0; ii < sSurfaceInfo.m_nHeight;ii++ )
			{	
				//ARGB8888ģʽÿ������Ϊ4���ֽڣ�
				//һ���н��������ɫ
				for(jj=0; jj < sSurfaceInfo.m_nWidth; jj ++)
				{
					*pSurTemp = uFillColor;  //32BIT
					pSurTemp++;
				}
			}
			return CSUDI_TRUE;
		}
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_1555:
		case OSG_COLOR_FORMAT_16BIT:
		{
			unsigned short *pSurTemp = (unsigned short*)sSurfaceInfo.m_pvSurfaceMem;

			for(ii=0; ii < sSurfaceInfo.m_nHeight;ii++ )
			{	
				//ARGB_1555ģʽÿ������Ϊ2���ֽڣ�
				//һ���н��������ɫ
				for(jj=0; jj < sSurfaceInfo.m_nWidth; jj ++)
				{
					*pSurTemp = uFillColor & 0xffff; //16BIT
					pSurTemp++;
				}
			}
			return CSUDI_TRUE;
		}
		case EM_UDIOSG_PIXEL_FORMAT_RGBA_5551:
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_4444:
		case EM_UDIOSG_PIXEL_FORMAT_RGB_565:
		default:
			CSTCPrint("[%s, %d] Error PixelMode: %d\n", __FUNCTION__, __LINE__, nPixelMode);
			return CSUDI_FALSE;
	}
	return CSUDI_FALSE;
}


unsigned int OSG_iGetRandomColor(CSUDI_HANDLE hSurface,const CSUDIOSGRect_S * psCheckRect,int nPixelMode)
{
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;
	int nRandX = 0;
	int nRandY = 0;
	unsigned int uTempColor = 0;
	unsigned char  *pSurTemp = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	sSurfaceInfo.m_pvSurfaceMem = CSUDI_NULL;
	
	nRet = CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo);				
	if(CSUDI_SUCCESS != nRet || sSurfaceInfo.m_pvSurfaceMem == CSUDI_NULL) 
	{
		CSASSERT(CSUDI_SUCCESS == nRet && sSurfaceInfo.m_pvSurfaceMem != CSUDI_NULL);
		return INVALID_COLOR;	
	}
	
	srand((unsigned int)time(0));
	if(CSUDI_NULL == psCheckRect)//ȫƽ��
	{
		nRandX = OSG_iGetRandom(0,sSurfaceInfo.m_nWidth);
		nRandY = OSG_iGetRandom(0,sSurfaceInfo.m_nHeight);
	}
	else
	{
		nRandX = OSG_iGetRandom(psCheckRect->m_nX,psCheckRect->m_nWidth);
		nRandY = OSG_iGetRandom(psCheckRect->m_nY,psCheckRect->m_nHeight);
	}
	CSTCPrint("[OSG_TEST] nRandX = %d , nRandY = %d \r\n",nRandX,nRandY);

	pSurTemp = (unsigned char *)sSurfaceInfo.m_pvSurfaceMem;
	//���ȡ����ͼƽ����ĳ�������ɫֵ
	switch(nPixelMode)
	{
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_8888:
		case OSG_COLOR_FORMAT_32BIT:
			{
				pSurTemp = pSurTemp + nRandY*sSurfaceInfo.m_nPitch + nRandX*4;  //ARGB8888ģʽÿ������Ϊ4���ֽ�
				uTempColor = *(unsigned int *)pSurTemp;
			}
			break;
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_1555:
		case EM_UDIOSG_PIXEL_FORMAT_RGBA_5551:
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_4444:
		case EM_UDIOSG_PIXEL_FORMAT_RGB_565:
		case OSG_COLOR_FORMAT_16BIT:
			{
				pSurTemp = pSurTemp + nRandY*sSurfaceInfo.m_nPitch + nRandX*2;  //ARGB_1555ģʽÿ������Ϊ2���ֽ�
				uTempColor =*(unsigned short*) pSurTemp;	
			}
			break;
		default:
			{
				pSurTemp = pSurTemp + nRandY*sSurfaceInfo.m_nPitch + nRandX*4;  //ARGB8888ģʽÿ������Ϊ4���ֽ�
				uTempColor =  *(unsigned int *)pSurTemp;
			}
			break;
	}
	CSTCPrint("[OSG_TEST] RandomColor=== %x \r\n",uTempColor);
	return uTempColor;
}
CSUDI_BOOL OSG_iCheckColors(CSUDI_HANDLE hSurface,unsigned int uCheckColor,const CSUDIOSGRect_S * psCheckRect,int nPixelMode,CSUDI_BOOL bExist)
{
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;
	unsigned char  *pSurTemp = CSUDI_NULL;
	unsigned char  *pTemp = CSUDI_NULL;
	
	int nXX = 0;
	int nX = -1;
	int nY = -1;
	int nWidth = -1;
	int nHeight = -1;
	int nXMax = -1;
	int nYMax = -1;
	unsigned int uTempcolor = 0;
	/*Hi3716H �ڽ���blitʱ��alphaֵ����΢С����ʧ,���㹫ʽΪ
	out_a = (((in_a + 1)>>1)<<1) - 1 */
	unsigned char ucAlpha = 0;
	unsigned int uCkeckColorWithAlphaLoss = 0;
	
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	sSurfaceInfo.m_pvSurfaceMem = CSUDI_NULL;
	
	nRet = CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo);				
	if(CSUDI_SUCCESS != nRet || sSurfaceInfo.m_pvSurfaceMem == CSUDI_NULL) 
	{
		CSASSERT(CSUDI_SUCCESS == nRet && sSurfaceInfo.m_pvSurfaceMem != CSUDI_NULL);
		return CSUDI_FALSE;	
	}
	
	if(CSUDI_NULL == psCheckRect)//ȫƽ��
	{
		nX = 0+1;//�����0��ʼ +1 2009-10-31��Ϊblit����ʱƽ���Ե���򲻽��д������+1
		nY = 0+1;
		nWidth = sSurfaceInfo.m_nWidth;
		nHeight = sSurfaceInfo.m_nHeight;
		nXMax = sSurfaceInfo.m_nWidth;
		nYMax = sSurfaceInfo.m_nHeight;
	}
	else if(psCheckRect->m_nX ==0 && psCheckRect->m_nY ==0)
	{
		nX = 0+1;//�����0��ʼ +1 2009-10-31��Ϊblit����ʱƽ���Ե���򲻽��д������+1
		nY = 0+1;
		nWidth = psCheckRect->m_nWidth;
		nHeight = psCheckRect->m_nHeight;  //for testcase CSTC_OSG_IT_0005��Rect��(0,0)��ʼ�Ĳ�������
		nXMax = psCheckRect->m_nWidth;
		nYMax = psCheckRect->m_nHeight;
	}
	else
	{
		nX = psCheckRect->m_nX;
		nY = psCheckRect->m_nY;
		nWidth = psCheckRect->m_nWidth;
		nHeight = psCheckRect->m_nHeight;
		nXMax = psCheckRect->m_nX+psCheckRect->m_nWidth;
		nYMax = psCheckRect->m_nY+psCheckRect->m_nHeight;
	}
	
	CSTCPrint("[OSG_TEST] The check area is {%d*%d,%d*%d} \r\n",nX,nY,nWidth,nHeight);
	pSurTemp = (unsigned char *)sSurfaceInfo.m_pvSurfaceMem;

	//����{nX*nY,Width*Height}1����
	switch(nPixelMode)
	{
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_8888:
		case OSG_COLOR_FORMAT_32BIT:
			{
				for(;nY < nYMax; nY++)
				{
					for(nXX = nX;nXX < nXMax; nXX++)
					{
						pTemp = pSurTemp + nY*sSurfaceInfo.m_nPitch + nXX*4;  //ARGB8888ģʽÿ������Ϊ4���ֽ�
						
						if(bExist) //��֤��ɫ���ڣ�����������򷵻�ʧ��
						{
							uTempcolor  = *(unsigned int *)pTemp; 

							ucAlpha = (uCheckColor>>24)&0xff;
							ucAlpha = (((ucAlpha + 1)>>1)<<1) - 1;

							uCkeckColorWithAlphaLoss = (ucAlpha<<24) | (uCheckColor&0xffffff);

							 if(uCheckColor != uTempcolor && uCkeckColorWithAlphaLoss != uTempcolor) 
							 {
							 	CSTCPrint("nX==%d,nY==%d\n",nXX,nY);
							 	CSTCPrint("[OSG_TEST] Line %d ActualColor is: 0x%x   ExpectedColor is: 0x%x \r\n",__LINE__,uTempcolor,uCheckColor);
							 	return CSUDI_FALSE;  //���ƽ����ɫ����������ֵ���򷵻�false
							 }
						}
						else //��֤��ɫ������,��������򷵻�ʧ��
						{
							 uTempcolor  = *(unsigned int *)pTemp; 
							 
							 if(uCheckColor == uTempcolor) 
							 {
							 	CSTCPrint("[OSG_TEST] NotExpectedColor is: %x   checkColor is: %x \r\n",uTempcolor,uCheckColor);
								return CSUDI_FALSE;  //���ƽ����ɫ��������ֵ���򷵻�false
							 }
						}
					}
				}
			}
			break;
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_1555:
		case OSG_COLOR_FORMAT_16BIT:
			{
				for(;nY < nYMax; nY++)
				{
					for(nXX = nX;nXX < nXMax; nXX++)
					{
						pTemp = pSurTemp + nY*sSurfaceInfo.m_nPitch + nXX*2;  //ARGB_1555ģʽÿ������Ϊ2���ֽ�
						
						if(bExist) //��֤��ɫ����
						{	
							 uTempcolor  = *(unsigned short *)pTemp;
							 
							 if(uCheckColor != uTempcolor) 
							 {
							 	CSTCPrint("[OSG_TEST] Line %d ActualColor is: 0x%x   ExpectedColor is: 0x%x \r\n",__LINE__,uTempcolor,uCheckColor);
							 	return CSUDI_FALSE;  //���ƽ����ɫ����������ֵ���򷵻�false
							 }
						}
						else //��֤��ɫ������
						{
							 uTempcolor = *(unsigned short *)pTemp;
							 if(uCheckColor == uTempcolor) 
							 {
							 	CSTCPrint("[OSG_TEST] NotExpectedColor is: %x   checkColor is: %x \r\n",uTempcolor,uCheckColor);
						 	
							 	return CSUDI_FALSE;  //���ƽ����ɫ��������ֵ���򷵻�false
							 }
						}
					}
				}
			}
			break;
		case EM_UDIOSG_PIXEL_FORMAT_RGBA_5551:
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_4444:
		case EM_UDIOSG_PIXEL_FORMAT_RGB_565:
		default:
			CSTCPrint("[%s, %d] Error PixelMode: %d\n", __FUNCTION__, __LINE__, nPixelMode);
			return CSUDI_FALSE;
	}
	return CSUDI_TRUE;
}
/*
* �����ɫcolorֵ�Ƿ��ڻ�ͼƽ������Դ�ռ���
*/
CSUDI_BOOL OSG_iCheckNotExistColor(CSUDI_HANDLE hSurface,unsigned int uCheckColor,const CSUDIOSGRect_S * psCheckRect,int nPixelMode)
{
	return OSG_iCheckColors(hSurface,uCheckColor,psCheckRect,nPixelMode,CSUDI_FALSE);
}
/*
* �����ɫcolorֵ�Ƿ��ڻ�ͼƽ������Դ�ռ���
*/
CSUDI_BOOL OSG_iCheckExistColor(CSUDI_HANDLE hSurface,unsigned int uCheckColor,const CSUDIOSGRect_S * psCheckRect,int nPixelMode)
{
	return OSG_iCheckColors(hSurface,uCheckColor,psCheckRect,nPixelMode,CSUDI_TRUE);
}

CSUDI_BOOL OSG_iGetColorByAlphaForumType(
												int nPixelMode, /*��ɫģʽ*/
												unsigned int uSrcColor,/*Դƽ����ɫֵ*/
												unsigned int uDstColor, /*Ŀ��ƽ����ɫֵ*/
												unsigned int  uSrcGAlpha,/*Դȫ��alphaֵ*/
												unsigned int  uDstGAlpha,/*Ŀ��ȫ��alphaֵ*/
												OSGUDI2AlphaForumType_E eType, /*��ʽ����*/
												unsigned int * puExpectColor /*���ݹ�ʽ��õ�alphaֵ*/
												)
{
	unsigned char sA = 0; //Դ��ɫֵ��alphaֵ
	unsigned char sB = 0;
	unsigned char sG = 0;
	unsigned char sR = 0;
	unsigned char sMixA = 0;
	
	unsigned char dB = 0;
	unsigned char dG = 0;
	unsigned char dR = 0;
	unsigned char dA = 0; //Ŀ����ɫֵ��alphaֵ
	unsigned char dMixA = 0;
	
	unsigned char uAout = 0;  //��Ͻ��alphaֵ
	unsigned char uRout = 0; //��Ͻ��R����ֵ
	unsigned char uGout = 0;
	unsigned char uBout = 0;
	CSUDI_BOOL bIs32Bit = CSUDI_FALSE;
	
	switch(nPixelMode)
	{
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_8888:
		case OSG_COLOR_FORMAT_32BIT:
			{
				sB = uSrcColor & 0xff;
				sG = (uSrcColor>>8) & 0xff;
				sR = (uSrcColor>>16) & 0xff;
				sA = (uSrcColor>>24) & 0xff;
				sMixA = sA * uSrcGAlpha/0xff;
				
				dB = uDstColor & 0xff;
				dG = (uDstColor>>8) & 0xff;
				dR = (uDstColor>>16) & 0xff;
				dA = (uDstColor>>24) & 0xff;
				dMixA = dA * uDstGAlpha/0xff;
				bIs32Bit = CSUDI_TRUE;
			}
			break;
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_1555:
		case OSG_COLOR_FORMAT_16BIT:
			{
				sB = uSrcColor & 0x1F;
				sG = (uSrcColor>>5) & 0x1f;
				sR = (uSrcColor>>10) & 0x1f;
				sA = (uSrcColor>>15) & 0x1f;
				sMixA = sA*uSrcGAlpha/0xff;
				
				dB = uDstColor & 0x1f;
				dG = (uDstColor>>5) & 0x1f;
				dR = (uDstColor>>10) & 0x1f;
				dA = (uDstColor>>15) & 0x1f;
				dMixA = dA * uDstGAlpha/0xff;
				bIs32Bit = CSUDI_FALSE;
			}
			break;
		case EM_UDIOSG_PIXEL_FORMAT_RGBA_5551:
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_4444:
		case EM_UDIOSG_PIXEL_FORMAT_RGB_565:
		default:
			CSTCPrint("[%s, %d] Error PixelMode: %d\n", __FUNCTION__, __LINE__, nPixelMode);
			return CSUDI_FALSE;
	}
	if(!bIs32Bit)
	{
		uAout = sMixA + dMixA - ((sMixA * dMixA));
	}
	else
	{
		switch(eType)
		{	
			case EM_FORUM_1:
				uAout = sMixA + dMixA - ((sMixA * dMixA)/0xff);
				break;
			case EM_FORUM_2:
				uAout = sA + dA*(1)*(1-sA);  //Alphaout=Asrc1 + Asrc2 * Global_Alpha (1- Asrc1)
				break;
			case EM_FORUM_4:
				uAout = dA;
				break;
			case EM_FORUM_5:
				uAout = sA; 
				break;
			case EM_FORUM_6:
				uAout = uDstGAlpha;
				break;
			case EM_FORUM_7:
				uAout = sMixA + dA - ((sMixA * dA)/0xff);
				break;
			case EM_FORUM_8:
				uAout = sMixA + dA *(0xff- sMixA)/0xff;
				uRout =dR + (sR - dR) * sMixA/0xff;
				uGout = dG + (sG - dG) * sMixA/0xff;
				uBout = dB + (sB - dB) * sMixA/0xff;

				break;
			default:
				CSASSERT(0);
				return CSUDI_FALSE;;
		}
	}
	if(uAout == 0)
	{
		*puExpectColor = uDstColor;
		return CSUDI_TRUE;
	}
	if(eType  != EM_FORUM_8 )
	{
		uRout = dR + (sR - dR) * sMixA /uAout;
		uGout = dG + (sG - dG) * sMixA /uAout;
		uBout = dB + (sB - dB) * sMixA /uAout;
	}
	if(bIs32Bit)
	{
		*puExpectColor = ((unsigned int)uAout<<24)|((unsigned int)uRout<<16)|((unsigned short)uGout<<8)|uBout;
	}
	else
	{
		*puExpectColor = ((unsigned short)uAout<<15)|((unsigned short)uRout<<10)|((unsigned char)uGout<<5)|uBout;
	}
	return CSUDI_TRUE;
}
//������ɫ�Ƚ�ʱ����ƫ��,���ÿһ����ƫ�����
 CSUDI_BOOL OSG_iCheckColorWithEffect(int nPixelMode,unsigned int uCheckColor, unsigned int nActualColor, int nEffectValue)
{

	unsigned char sA = 0; //Դ��ɫֵ��alphaֵ
	unsigned char sB = 0;
	unsigned char sG = 0;
	unsigned char sR = 0;
	unsigned char dB = 0;
	unsigned char dG = 0;
	unsigned char dR = 0;
	unsigned char dA = 0; //Ŀ����ɫֵ��alphaֵ
	
	switch(nPixelMode)
	{
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_8888:
		case OSG_COLOR_FORMAT_32BIT:
			{
				sB = uCheckColor & 0xff;
				sG = (uCheckColor>>8) & 0xff;
				sR = (uCheckColor>>16) & 0xff;
				sA = (uCheckColor>>24) & 0xff;
				
				dB = nActualColor & 0xff;
				dG = (nActualColor>>8) & 0xff;
				dR = (nActualColor>>16) & 0xff;
				dA = (nActualColor>>24) & 0xff;
			}
			break;
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_1555:
		case OSG_COLOR_FORMAT_16BIT:
			{
				sB = uCheckColor & 0x1F;
				sG = (uCheckColor>>5) & 0x1f;
				sR = (uCheckColor>>10) & 0x1f;
				sA = (uCheckColor>>15) & 0x1f;
				
				dB = nActualColor & 0x1f;
				dG = (nActualColor>>5) & 0x1f;
				dR = (nActualColor>>10) & 0x1f;
				dA = (nActualColor>>15) & 0x1f;
			}
			break;
		case EM_UDIOSG_PIXEL_FORMAT_RGBA_5551:
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_4444:
		case EM_UDIOSG_PIXEL_FORMAT_RGB_565:
		default:
			CSTCPrint("[%s, %d] Error PixelMode: %d\n", __FUNCTION__, __LINE__, nPixelMode);
			return CSUDI_FALSE;
	}

	return ((sA-nEffectValue)<= dA && dA <= (sA+nEffectValue))
			&& ((sR-nEffectValue)<= dR && dR <= (sR+nEffectValue))
			&& ((sG-nEffectValue)<= dG && dG <= (sG+nEffectValue))
			&& ((sB-nEffectValue)<= dB && dB <= (sB+nEffectValue));
}
/*���ļ��л�ȡ�������Ա�,�˴�����λ32λģʽ*/
CSUDI_BOOL OSG_iCheckColorAndAlphaWithFileColor(CSUDI_HANDLE hDstSurface,
													const unsigned char * puSrcColor,/*Դƽ����ɫֵ*/
													const unsigned char * puDstColor, /*Ŀ��ƽ����ɫֵ*/
													int 		  nColorLen,   /*��ɫ��С��Ŀǰ������ɫ�Ĵ�Сһ��*/
													unsigned int  uSrcAlpha,/*Դȫ��alphaֵ*/
													unsigned int  uDstAlpha,/*Ŀ��ȫ��alphaֵ*/
													int nPixelMode)
{
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;	
	CSUDI_BOOL bResult = CSUDI_FALSE;
	unsigned int uExpectColor = 0;
	unsigned int nActualColor = 0;
	int nCompareLen = 0;
	int nSurfaceSize = 0;

	int nPixelIndex = 0;
	int nForumIndex = 0;

	unsigned int * pSurface = CSUDI_NULL;
	unsigned int * pSurTemp = CSUDI_NULL;
	
	unsigned int * pSrcColor = (unsigned int *)puSrcColor;
	unsigned int * pSrcTmp = CSUDI_NULL;
	
	unsigned int * pDstcColor = (unsigned int *)puDstColor;
	unsigned int * pDstTmp = CSUDI_NULL;
	
	if(hDstSurface == CSUDI_NULL) 
	{
		CSASSERT(hDstSurface != CSUDI_NULL);
		return CSUDI_FALSE;	
	}
	
	if(puSrcColor == CSUDI_NULL || puDstColor == CSUDI_NULL) return CSUDI_FALSE;
	
	sSurfaceInfo.m_pvSurfaceMem = CSUDI_NULL;
	nRet = CSUDIOSGGetSurfaceInfo(hDstSurface, &sSurfaceInfo);	
	
	if(CSUDI_SUCCESS != nRet || sSurfaceInfo.m_pvSurfaceMem == CSUDI_NULL) 
	{
		CSASSERT(CSUDI_SUCCESS == nRet && sSurfaceInfo.m_pvSurfaceMem != CSUDI_NULL);
		return CSUDI_FALSE;	
	}
	
	nSurfaceSize = sSurfaceInfo.m_nHeight*sSurfaceInfo.m_nWidth*4;

	nCompareLen = (nSurfaceSize > nColorLen) ? nColorLen : nSurfaceSize; //�ֽ�
	
	CSTCPrint("[OSG_TEST] nCompareLen =====: %d\n",nCompareLen);
	
	pSurface = (unsigned int *)sSurfaceInfo.m_pvSurfaceMem; //ARGB8888ģʽÿ������Ϊ4���ֽ�


	for(nPixelIndex = 1; nPixelIndex < (nCompareLen/4-1); nPixelIndex++) //������رȽ�,��Ե���򲻱Ƚ�
	{
		pSurTemp = pSurface + nPixelIndex; 
		pSrcTmp = pSrcColor +  nPixelIndex;
		pDstTmp = pDstcColor +  nPixelIndex;
		
		bResult = CSUDI_FALSE;
		nActualColor = *pSurTemp;
		
		for(nForumIndex= EM_FORUM_1; nForumIndex < EM_FORUM_Max; nForumIndex++) //�����ʽ���бȽ�
		{

			if(!OSG_iGetColorByAlphaForumType(nPixelMode,*pSrcTmp,*pDstTmp,uSrcAlpha,uDstAlpha,nForumIndex,&uExpectColor)) 
			{
				CSASSERT(0);
				return CSUDI_FALSE;
			}
		
			if((nActualColor == uExpectColor) || OSG_iCheckColorWithEffect(nPixelMode,uExpectColor,nActualColor,COLOR_EFFECT_VALUE)) //�����������ƫ��1
			{
				bResult = CSUDI_TRUE; //ֻҪ��һ����ʽ�����������ɫ����Ҫ��ͷ��سɹ�
				break;
			}
		}
		
		if(nForumIndex == EM_FORUM_Max) //��ƥ�����ӡ
		{
			CSTCPrint("[OSG_TEST] nPixelIndex =====: %d\n",nPixelIndex);
			CSTCPrint("[OSG_TEST] Actual color is: 0x%x ,ExpectedColor from forum (%d) is: 0x%x \r\n",
						nActualColor,(nForumIndex),uExpectColor);
			bResult = CSUDI_FALSE;
		}
		if(!bResult) //�������й�ʽ��û����ȣ���Ƚ�ʧ��
			break;
	}

	return bResult;
}
//������ɫ�Ƚ�ʱ����ƫ��,���ÿһ����ƫ�����
CSUDI_BOOL OSG_iCheckSurfaceColorsWithEffect(CSUDI_HANDLE hSurface,unsigned int uCheckColor,const CSUDIOSGRect_S * psCheckRect,int nPixelMode,int nEffectValue)
{
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;
	unsigned char  *pSurTemp = CSUDI_NULL;
	unsigned char  *pTemp = CSUDI_NULL;

	int nXX = 0;
	
	int nX = -1;
	int nY = -1;
	int nWidth = -1;
	int nHeight = -1;
	int nXMax = -1;
	int nYMax = -1;
	unsigned int uTempcolor = 0;
	
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	sSurfaceInfo.m_pvSurfaceMem = CSUDI_NULL;
	
	nRet = CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo);				
	if(CSUDI_SUCCESS != nRet || sSurfaceInfo.m_pvSurfaceMem == CSUDI_NULL) 
	{
		CSASSERT(CSUDI_SUCCESS == nRet && sSurfaceInfo.m_pvSurfaceMem != CSUDI_NULL);
		return CSUDI_FALSE;	
	}
	
	if(CSUDI_NULL == psCheckRect)//ȫƽ��
	{
		nX = 0+1;//�����0��ʼ +1 2009-10-31��Ϊblit����ʱƽ���Ե���򲻽��д������+1
		nY = 0+1;
		nWidth = sSurfaceInfo.m_nWidth;
		nHeight = sSurfaceInfo.m_nHeight;
		nXMax = sSurfaceInfo.m_nWidth;
		nYMax = sSurfaceInfo.m_nHeight;
	}
	else
	{
		nX = psCheckRect->m_nX;
		nY = psCheckRect->m_nY;
		nWidth = psCheckRect->m_nWidth;
		nHeight = psCheckRect->m_nHeight;
		nXMax = psCheckRect->m_nX+psCheckRect->m_nWidth;
		nYMax = psCheckRect->m_nY+psCheckRect->m_nHeight;
	}
	
	CSTCPrint("[OSG_TEST] The check area is {%d*%d,%d*%d} \r\n",nX,nY,nWidth,nHeight);
	pSurTemp = (unsigned char *)sSurfaceInfo.m_pvSurfaceMem;

	//����{nX*nY,Width*Height}1����
	switch(nPixelMode)
	{
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_8888:
		case OSG_COLOR_FORMAT_32BIT:
			{
				for(;nY < nYMax; nY++)
				{
					for(nXX = nX; nXX < nXMax; nXX++)
					{
						pTemp = pSurTemp + nY*sSurfaceInfo.m_nPitch + nXX*4;  //ARGB8888ģʽÿ������Ϊ4���ֽ�
						//printf("nXX = %d,nY == %d\n",nXX,nY);
						
						uTempcolor = *(unsigned int *)pTemp; 
						if(!OSG_iCheckColorWithEffect(nPixelMode,uCheckColor,uTempcolor,nEffectValue)) //���������ƫ�Χ֮���򷵻�ʧ��
						{
							CSTCPrint("nX==%d,nY==%d\n",nXX,nY);
							CSTCPrint("[OSG_TEST] Line %d ActualColor is: 0x%x   ExpectedColor is: 0x%x \r\n",__LINE__,uTempcolor,uCheckColor);
							return CSUDI_FALSE;  //���ƽ����ɫ����������ֵ���򷵻�false
						}
					}
				}
			}
			break;
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_1555:
		case OSG_COLOR_FORMAT_16BIT:
			{
				for(;nY < nYMax; nY++)
				{
					for(nXX = nX; nXX < nXMax; nXX++)
					{
						pTemp = pSurTemp + nY*sSurfaceInfo.m_nPitch + nXX*2;  //ARGB_1555ģʽÿ������Ϊ2���ֽ�
						
						uTempcolor = *(unsigned short *)pTemp;

						if(uCheckColor != uTempcolor) 
						{
							CSTCPrint("[OSG_TEST] Line %d ActualColor is: 0x%x   ExpectedColor is: 0x%x \r\n",__LINE__,uTempcolor,uCheckColor);
							return CSUDI_FALSE;  //���ƽ����ɫ����������ֵ���򷵻�false
						}
					}
				}
			}
			break;
		case EM_UDIOSG_PIXEL_FORMAT_RGBA_5551:
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_4444:
		case EM_UDIOSG_PIXEL_FORMAT_RGB_565:
		default:
			CSTCPrint("[%s, %d] Error PixelMode: %d\n", __FUNCTION__, __LINE__, nPixelMode);
			return CSUDI_FALSE;
	}
	return CSUDI_TRUE;
}
/*
*��֤����alpha��Ϻ��͸����ֵ����ɫֵ(ARGB888ģʽ)
*/
CSUDI_BOOL OSG_iCheckColorAndAlpha(CSUDI_HANDLE hDstSurface,
													unsigned int uSrcColor,/*Դƽ����ɫֵ*/
													unsigned int uDstColor, /*Ŀ��ƽ����ɫֵ*/
													unsigned int  uSrcAlpha,/*Դȫ��alphaֵ*/
													unsigned int  uDstAlpha,/*Ŀ��ȫ��alphaֵ*/
													int nPixelMode)
{
	unsigned int uExpectColor = 0;
	int ii = 0;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	
	if(hDstSurface == CSUDI_NULL) 
	{
		CSASSERT(hDstSurface != CSUDI_NULL);
		return CSUDI_FALSE;	
	}
	
	for(ii= EM_FORUM_1; ii < EM_FORUM_Max; ii++)
	{
		if(!OSG_iGetColorByAlphaForumType(nPixelMode,uSrcColor,uDstColor,uSrcAlpha,uDstAlpha,ii,&uExpectColor)) 
		{
			CSASSERT(0);
			return CSUDI_FALSE;
		}
		CSTCPrint("[OSG_TEST] ExpectedColor from forum (%d) is: 0x%x \r\n",(ii+1),uExpectColor);
		if(OSG_iCheckSurfaceColorsWithEffect(hDstSurface,uExpectColor,CSUDI_NULL,nPixelMode,COLOR_EFFECT_VALUE))
		{
			bResult = CSUDI_TRUE; //ֻҪ��һ����ʽ�����������ɫ����Ҫ��ͷ��سɹ�
			break;
		}
	}
	
	return bResult;
}

CSUDI_BOOL OSG_iCheckColorMixResult(CSUDI_HANDLE hSurface,unsigned int  uSrcColor,unsigned int  uDstColor,int nMixMode,int nPixelMode)
{
	unsigned int uCheckColor = 0;
	if(hSurface == CSUDI_NULL) 
	{
		CSASSERT(hSurface != CSUDI_NULL);
		return CSUDI_FALSE;	
	}
	
	if(EM_UDIOSG_OPMODE_COPY == nMixMode)
	{
		uCheckColor = uSrcColor;
	}
	else if(EM_UDIOSG_OPMODE_XOR == nMixMode)
	{
		uCheckColor = (uSrcColor ^ uDstColor);
	}
	else if(EM_UDIOSG_OPMODE_OR == nMixMode)
	{
		uCheckColor = (uSrcColor | uDstColor);
	}
	else if(EM_UDIOSG_OPMODE_AND == nMixMode)
	{
		uCheckColor = (uSrcColor & uDstColor);
	}
	else if(EM_UDIOSG_OPMODE_CLEAR == nMixMode)
	{
		uCheckColor = 0 ;
	}
	else if(EM_UDIOSG_OPMODE_SET == nMixMode)
	{
		uCheckColor = (~0);
	}
	else if(EM_UDIOSG_OPMODE_EQUIV == nMixMode)
	{
		uCheckColor = ~(uSrcColor ^ uDstColor);
	}
	else if(EM_UDIOSG_OPMODE_NOR == nMixMode)
	{
		uCheckColor = ~(uSrcColor | uDstColor);
	}
	else if(EM_UDIOSG_OPMODE_NAND == nMixMode)
	{
		uCheckColor = ~(uSrcColor & uDstColor);
	}
	else if(EM_UDIOSG_OPMODE_INVERT == nMixMode)
	{
		uCheckColor = ~( uDstColor);
	}
	else if(EM_UDIOSG_OPMODE_COPYINVERTED == nMixMode)
	{
		uCheckColor = ~(uSrcColor);
	}
	else if(EM_UDIOSG_OPMODE_ORINVERTED == nMixMode)
	{
		uCheckColor =( (~uSrcColor) | uDstColor);
	}
	else if(EM_UDIOSG_OPMODE_ANDINVERTED == nMixMode)
	{
		uCheckColor = ((~uSrcColor) & uDstColor);
	}
	else if(EM_UDIOSG_OPMODE_ORREVERSE == nMixMode)
	{
		uCheckColor = (uSrcColor | (~uDstColor));
	}
	else if(EM_UDIOSG_OPMODE_ANDREVERSE == nMixMode)
	{
		uCheckColor = (uSrcColor & (~uDstColor));
	}
	else if(EM_UDIOSG_OPMODE_NOOP == nMixMode)
	{
		uCheckColor = (uDstColor);
	}
	else
	{
		CSTCPrint("[%s, %d] Error MixMode: %d\n", __FUNCTION__, __LINE__, nMixMode);
		return CSUDI_FALSE;
	}
	
	return OSG_iCheckColors(hSurface,uCheckColor,CSUDI_NULL,nPixelMode,CSUDI_TRUE);
}

/*
*��ȡͼƬ����
*/
unsigned int  OSG_iGetPhoto(const char *photoName, unsigned char **pBuff )
{
	CSUDI_HANDLE fp = CSUDI_NULL;
    unsigned int nIframeLen = 0;
	CSUDI_BOOL bOpen = CSUDI_FALSE;

	if(CSUDI_NULL != (fp = CSTKP_FOpen(photoName, "rb")))
	{
		CSTCPrint("ͼƬ%s�򿪳ɹ�\n ", photoName);
		bOpen = CSUDI_TRUE;
	}

	else
	{
		CSTCPrint("ͼƬ""%s ""��ʧ��\n", photoName);
		bOpen = CSUDI_FALSE;
	}

	if(bOpen)
	{
		if(0 == CSTKP_FSeek(fp, 0, CSTK_SEEK_END))
		{
			nIframeLen = CSTKP_FTell(fp);
			CSTCPrint("iframeLen = 0x%x\n", nIframeLen);
			
			if(nIframeLen > 0 && nIframeLen < 4*1920*1080 )       //ͼƬ��С�Ϸ����ж�1920*1080�ֱ���ARGB8888 
			{
				*pBuff = CSUDIOSMalloc(nIframeLen); 
				if(*pBuff == CSUDI_NULL)
				{
					CSTCPrint("����ռ�ʧ��!\n");
					if(fp != CSUDI_NULL)
					{
						CSTKP_FClose(fp);
					}
					return 0;
				}
				memset(*pBuff, 0,nIframeLen);

		   		if(0 != CSTKP_FSeek(fp,  0, CSTK_SEEK_SET))
	         	{
					CSTCPrint("������ʼָ��ʧ��!\n");
					bOpen = CSUDI_FALSE;
		   		}
	
				if(nIframeLen != (CSTKP_FRead(*pBuff, sizeof(unsigned char),nIframeLen, fp)))
				{
					CSTCPrint("��%s�ļ�ʧ��!\n", photoName);
					bOpen = CSUDI_FALSE;
				}
			}
			else
			{
				CSTCPrint("ͼƬ��С������ȷ��Χ��ʧ��!\n");
				bOpen = CSUDI_FALSE;
			}
		}
		else
		{
			CSTCPrint("Ѱ��ͼƬ%s ĩָ��ʧ��!\n", photoName);
			bOpen = CSUDI_FALSE;
		}
	}
	
	if(fp != CSUDI_NULL)
	{
		CSTKP_FClose(fp);
	}

	if (!bOpen)
	{
		nIframeLen = 0;

		if (*pBuff != CSUDI_NULL)
		{
			CSUDIOSFree(*pBuff);
			*pBuff = CSUDI_NULL;
		}
	}
	
	return nIframeLen;

}
//����(min)��(max-1)֮��������
int OSG_iGetRandom(int min, int max)
{ 
	int nRet = ((rand()%((max) - (min))) + (min)); 
	return ( nRet == max ? (nRet-1) : nRet); 
}

CSUDI_BOOL OSG_iTESTSurfacePitch(CSUDIOSGPixelFormat_E ePixel,int nWidth ,int nHeight,unsigned char * puDstColor, int nColorLen)
{
	if(OSG_IsSupportPixelFormat(ePixel))
	{
		CSUDI_HANDLE hSurface_S = CSUDI_NULL;
		CSUDI_HANDLE hBGSurface = CSUDI_NULL; //��Ϊ����ɫ
		CSUDIOSGSurfaceInfo_S sSurfaceInfo;	
		CSUDI_Error_Code nRet = CSUDI_FAILURE;
		CSUDI_HANDLE hDisplay = CSUDI_NULL;
		int ii;
		unsigned char * pColorBuf = CSUDI_NULL;
		unsigned char * pSurTemp = CSUDI_NULL;
		CSUDIOSGRect_S sDRect;
		int nSurfaceSize,nCopyLen;
		
		nRet = CSUDIOSGCreateSurface(ePixel,
										nWidth,
										nHeight,
										&hSurface_S
										);

		CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),"Step 1 P1_1 Create Surface Failure !");
		
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(g_nTestDisplaySurfaceIndex,&hDisplay)),
					"Step 2 get display failure"
				);	
		
		memset(&sSurfaceInfo,0,sizeof(CSUDIOSGSurfaceInfo_S));
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_S, &sSurfaceInfo)),
					"Step 3 get surface info failure"
				);	
		memset(sSurfaceInfo.m_pvSurfaceMem, 0, sSurfaceInfo.m_nMemSize);
		
		pSurTemp = (unsigned char*)sSurfaceInfo.m_pvSurfaceMem;
		pColorBuf = puDstColor;

		nSurfaceSize = sSurfaceInfo.m_nHeight*sSurfaceInfo.m_nWidth*4;
		nCopyLen = (nSurfaceSize > nColorLen) ? nColorLen : nSurfaceSize;
		
		for(ii=0; ii < nHeight && pColorBuf <(puDstColor + nCopyLen);ii++)
		{
			memcpy(pSurTemp,pColorBuf,nWidth*4);
			
			pSurTemp += sSurfaceInfo.m_nPitch;
			pColorBuf += nWidth*4;
		}
		
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hDisplay, &sSurfaceInfo)),
					"Step 5 get display info failure"
				);	
		
		sDRect.m_nX = sSurfaceInfo.m_nWidth/2-200;
		sDRect.m_nY = sSurfaceInfo.m_nHeight/2-200;  
		sDRect.m_nWidth = nWidth;
		sDRect.m_nHeight = nHeight;

		nRet = CSUDIOSGCreateSurface(sSurfaceInfo.m_ePixelFormat,
										sSurfaceInfo.m_nWidth,
										sSurfaceInfo.m_nHeight,
										&hBGSurface
										);

		CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hBGSurface != CSUDI_NULL),"Step 6  Create bg Surface Failure !");

		OSG_iFillSurface(hBGSurface,0xffffffff,sSurfaceInfo.m_ePixelFormat);
		//��ʼ��������ɫ
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGBlit(hDisplay, CSUDI_NULL, hBGSurface, CSUDI_NULL, EM_UDIOSG_OPMODE_COPY)),
					"Step 7 blit  failure"
				);
		
		//��ʼ��Դ��ͼƽ����ɫ
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGBlit(hDisplay, &sDRect, hSurface_S, CSUDI_NULL, EM_UDIOSG_OPMODE_COPY)),
					"Step 8 blit  failure"
				);	
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGSync()),
					"Step 9 Sync  failure"
				);
		CSTCPrint("ͼ���Ƿ�������ʾ(������κ�ɫ���ڲ�������ɫ)?\n");
		
		CSTK_ASSERT_TRUE_FATAL(
					(CSTKWaitYes()),
					"Step 10 ͼ����ʾ������"
				);
		CSTK_FATAL_POINT
		if(CSUDI_NULL != hSurface_S)
		{
			if(CSUDIOSGDestroySurface(hSurface_S) != CSUDI_SUCCESS)
				CSTCPrint("Step 11 destroy P1_1 surface failure\n");
			hSurface_S = CSUDI_NULL;
		}
		if(CSUDI_NULL != hBGSurface)
		{
			if(CSUDIOSGDestroySurface(hBGSurface) !=  CSUDI_SUCCESS)
				CSTCPrint("Step 12 destroy P1_1 surface failure");
			hBGSurface = CSUDI_NULL;
		}
		//��������
		CSUDIOSGFill(hDisplay, CSUDI_NULL, 0);
		CSUDIOSGSync();
		
		return CSUDI_TRUE;
	}
	else 
	{
		CSTCPrint("[%s, %d] Not Supported, PixelFormat=%d !!!\n", __FUNCTION__, __LINE__, ePixel);
	  	return CSUDI_TRUE;	
	}
}

void OSG_iClearScreen()
{
	CSUDI_HANDLE hDisplay = CSUDI_NULL;
	CSUDI_HANDLE hDisplay_HD = CSUDI_NULL;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;
	CSUDI_BOOL bSupportHD = OSG_iTestSupportHDDisplay();
    
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(g_nTestDisplaySurfaceIndex, &hDisplay) && (CSUDI_NULL != hDisplay)),"��ȡ�Դ�ʧ��");

	memset(&sSurfaceInfo, 0, sizeof(sSurfaceInfo));
	CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hDisplay, &sSurfaceInfo)),
					"Get Surface info failure !"
				);

	CSTK_ASSERT_TRUE_FATAL(
                    (CSUDI_SUCCESS == CSUDIOSGFill(hDisplay, CSUDI_NULL, 0x00000000)),
                    "Fill Display Failure !"
                );  

	if(bSupportHD == CSUDI_TRUE)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(1,&hDisplay_HD)),
			"��ȡ�����Դ�ʧ��!");

		memset(&sSurfaceInfo, 0, sizeof(sSurfaceInfo));
		CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hDisplay_HD, &sSurfaceInfo)),
						"Get Surface info failure !"
					);
	
		CSTK_ASSERT_TRUE_FATAL(
                        (CSUDI_SUCCESS == CSUDIOSGFill(hDisplay_HD, CSUDI_NULL, 0x00000000)),
                        "Fill Display Failure !"
                    );  
    	} 

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"snyc failure");

	CSTK_FATAL_POINT

 	return ;	
}
CSUDI_BOOL OSG_iTestDecImageToDisplay(OSGUDI2SupportDecIamgeType_E eSupportType,OSGUDI2ImageType_E eImageType)
{
	if(OSG_IsSupportDecImage(eSupportType))
	{
		CSUDI_HANDLE hSurface_S = CSUDI_NULL;
		CSUDI_HANDLE hSurface_D = CSUDI_NULL;
		unsigned char * pcBuff = CSUDI_NULL;
		unsigned int nImageLen = 0;
		char cType[32];
		int ii = 0;
		
		switch(eSupportType)
		{
			case EM_OSG_DECIMAGE_IFRAME:
				strcpy(cType,"IFRAME");
				break;
			case EM_OSG_DECIMAGE_BMP:
				strcpy(cType,"BMP");
				break;
			case EM_OSG_DECIMAGE_JPG:
				strcpy(cType,"JPEG");
				break;
			case EM_OSG_DECIMAGE_PNG:
				strcpy(cType,"PNG");
				break;
			default:
				CSTK_ASSERT_TRUE_FATAL(0,"OSG_iTestDecImage ����δ֪\n");
				break;
		}

		if ((eSupportType == EM_OSG_DECIMAGE_BMP) || (eSupportType == EM_OSG_DECIMAGE_PNG))
		{
			CSTCPrint("���м��δʹ�ã��ݲ�����%s����ͼƬӲ����\n", cType);
			return CSUDI_TRUE;
		}
		
		nImageLen = OSG_iGetPhoto(g_aImageFile[eImageType],&pcBuff);
		CSTK_ASSERT_TRUE_FATAL(
					((pcBuff != CSUDI_NULL) && nImageLen != 0),
					"Step 1 get photo data failure"
				);
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGDecodeImage(cType,pcBuff,nImageLen,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,&hSurface_S) && (hSurface_S != CSUDI_NULL)),
					"Step 2 decode image failure"
				);
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(g_nTestDisplaySurfaceIndex,&hSurface_D)),
					"Step 3 get display surface failure"
				);
		for(ii = 0;ii < 100;ii ++)
		{
                    OSG_iClearScreen();
                    CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_SUCCESS == CSUDIOSGBlit(hSurface_D,CSUDI_NULL,hSurface_S,CSUDI_NULL,EM_UDIOSG_OPMODE_COPY)),
						"Step 4 blit image failure"
				);
			CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_SUCCESS == CSUDIOSGSync()),
						"Step 5 snyc failure"
					);
		}
		CSTCPrint("��ʾ:�ղ���Ļ�Ͽ��ܻ���������˸��������\n");
		CSTCPrint("Answer 0:Yes,��Ļ����ʾ %s ����...\r\n",g_aImageFile[eImageType]);
		CSTCPrint("Answer 1:No, ��Ļ��û�� %s ����...\r\n",g_aImageFile[eImageType]);
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == CSTKWaitYes()),
					"Step 6 Check failure"
				);	

		CSTK_FATAL_POINT
		if(hSurface_S != CSUDI_NULL)
		{
			if(CSUDIOSGDestroySurface(hSurface_S)!=CSUDI_SUCCESS)
				CSTCPrint("Step 6 destroy decode surface failure\n");
			hSurface_S = CSUDI_NULL;
		}
		if(pcBuff != CSUDI_NULL)
		{
			if(CSUDIOSFree(pcBuff)!=CSUDI_SUCCESS)
				CSTCPrint("Step 4 free photo data failure");
			pcBuff = CSUDI_NULL;
			CSTCPrint("[UDI2OSGEST]CSUDIOSFree success !! \r\n");
		}
		return CSUDI_TRUE;
	}
	else 
	{
		CSTCPrint("[%s, %d] Not Supported , ImageType=%d!!!\n", __FUNCTION__, __LINE__, eSupportType);
	  	return CSUDI_TRUE;
	}
}

//�˺�������checkָ����surface��ÿ�����ص�alphaֵ�Ƿ�Ϊָ��ֵ
CSUDI_BOOL OSG_iTestCheckSurfaceAlpha(CSUDI_HANDLE hSurface, unsigned char ucAlpha)
{
	unsigned char ucPixels;
	int i,j;
	CSUDIOSGSurfaceInfo_S SSurfaceInfo;
	unsigned char ucAlpha1555;

	int nBytesPerPixel = -1;	
	
	CSUDIOSGGetSurfaceInfo(hSurface, &SSurfaceInfo);

	if(SSurfaceInfo.m_ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)
	{
		nBytesPerPixel = 2;
		if(ucAlpha > 0)
		{
			ucAlpha1555 = 1;
		}
		else
		{
			ucAlpha1555 = 0;
		}
		
		for(i = 0; i < SSurfaceInfo.m_nHeight; i++)
		{
			for(j = 0; j < SSurfaceInfo.m_nWidth; j++)
			{
				ucPixels = *((unsigned char*)SSurfaceInfo.m_pvSurfaceMem + i * SSurfaceInfo.m_nPitch + j * nBytesPerPixel + 1);
				if(ucPixels >>15 != ucAlpha1555)
				{
					break;
				}
			}
		}
		return CSUDI_SUCCESS;
	}
	else if(SSurfaceInfo.m_ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888)
	{
		nBytesPerPixel = 4;
		for(i = 0; i < SSurfaceInfo.m_nHeight; i++)
		{
			for(j = 0; j < SSurfaceInfo.m_nWidth; j++)
			{
				ucPixels = *((unsigned char*)SSurfaceInfo.m_pvSurfaceMem + i * SSurfaceInfo.m_nPitch + j * nBytesPerPixel + 3);
				if(ucPixels != ucAlpha)
				{
					break;
				}
			}
		}
		return CSUDI_SUCCESS;
	}

	return CSUDI_FAILURE;
}

CSUDI_BOOL OSG_iTestMemcpyImageToDisplay(OSGUDI2SupportDecIamgeType_E eSupportType,OSGUDI2ImageType_E eImageType)
{
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hDisplaySurface = CSUDI_NULL;
	unsigned char * pcBuff = CSUDI_NULL;
	unsigned int nImageLen = 0;
	char cType[32];
	int i = 0;
	CSUDIOSGSurfaceInfo_S stSSurfaceInfo = {0};
	CSUDIOSGSurfaceInfo_S stDSurfaceInfo = {0};
	
	switch(eSupportType)
	{
		case EM_OSG_DECIMAGE_IFRAME:
			strcpy(cType,"IFRAME");
			break;
		case EM_OSG_DECIMAGE_BMP:
			strcpy(cType,"BMP");
			break;
		case EM_OSG_DECIMAGE_JPG:
			strcpy(cType,"JPEG");
			break;
		case EM_OSG_DECIMAGE_PNG:
			strcpy(cType,"PNG");
			break;
		default:
			CSTK_ASSERT_TRUE_FATAL(0,"OSG_iTestDecImage ����δ֪\n");
			break;
	}

	if ((eSupportType == EM_OSG_DECIMAGE_BMP) || (eSupportType == EM_OSG_DECIMAGE_PNG))
	{
		CSTCPrint("���м��δʹ�ã��ݲ�����%s����ͼƬӲ����\n", cType);
		return CSUDI_TRUE;
	}
	else if (eSupportType == EM_OSG_DECIMAGE_IFRAME)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_IsSupportDecImage(eSupportType)), 
				"ƽ̨��֧��IFRAME����ͼƬӲ����");
	}
	else if (eSupportType == EM_OSG_DECIMAGE_JPG)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_IsSupportDecImage(eSupportType)), 
				"ƽ̨��֧��JPEG����ͼƬӲ����");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_FALSE), "�����ͼƬ����");
	}
				
 	OSG_iClearScreen();

	//��ȡͼƬ
	nImageLen = OSG_iGetPhoto(g_aImageFile[eImageType],&pcBuff);
	CSTK_ASSERT_TRUE_FATAL(
				((pcBuff != CSUDI_NULL) && nImageLen != 0),
				"Step 1 get photo data failure"
			);
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGDecodeImage(cType,pcBuff,nImageLen,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,&hSurface_S) && (hSurface_S != CSUDI_NULL)),
				"Step 2 decode image failure"
			);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_S, &stSSurfaceInfo) && stSSurfaceInfo.m_pvSurfaceMem != CSUDI_NULL),
			"Get Iframe surface info failure !"
		);
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(g_nTestDisplaySurfaceIndex,&hDisplaySurface)),
				"Step 3 get display surface failure"
			);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hDisplaySurface, &stDSurfaceInfo) && stDSurfaceInfo.m_pvSurfaceMem != CSUDI_NULL),
			"Get Iframe surface info failure !"
		);

	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_SUCCESS == OSG_iTestCheckSurfaceAlpha(hSurface_S, 0xFF)),
		"����:ͼƬ�����Alphaֵ��Ϊ0xff"
	);

	CSTCPrint("stSSurfaceInfo = %d  %d  %d\n",stSSurfaceInfo.m_nPitch,stDSurfaceInfo.m_nPitch,stDSurfaceInfo.m_nMemSize);

	//����
	for(i  = 0;i < stSSurfaceInfo.m_nHeight;i ++)
	{
		if( (i * stDSurfaceInfo.m_nPitch + stDSurfaceInfo.m_nPitch) <= stDSurfaceInfo.m_nMemSize
			&& (i * stSSurfaceInfo.m_nPitch + stSSurfaceInfo.m_nPitch) <= stSSurfaceInfo.m_nMemSize)
		{
			memcpy((CSUDI_INT8 *)stDSurfaceInfo.m_pvSurfaceMem + i * stDSurfaceInfo.m_nPitch ,(CSUDI_INT8 *)stSSurfaceInfo.m_pvSurfaceMem + i * stSSurfaceInfo.m_nPitch ,stSSurfaceInfo.m_nPitch);
		}
		else
		{
			break;
		}
	}

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGSync()),
				"Step 4 snyc failure"
			);		

	CSTCPrint("������������Ļ����%sͼƬ\n",g_aImageFile[eImageType]);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()),"Step 5 Check failure"); 		
	
	CSTK_FATAL_POINT;

	OSG_iClearScreen();
	
	if(hSurface_S != CSUDI_NULL)
	{
		if(CSUDIOSGDestroySurface(hSurface_S)!=CSUDI_SUCCESS)
			CSTCPrint("Step 6 destroy decode surface failure\n");
		hSurface_S = CSUDI_NULL;
	}
	
	if(pcBuff != CSUDI_NULL)
	{
		if(CSUDIOSFree(pcBuff)!=CSUDI_SUCCESS)
			CSTCPrint("Step 7 free photo data failure");
		pcBuff = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
}

CSUDI_BOOL OSG_iTestYUVtoRGB(CSUDIOSGPixelFormat_E ePixelFormat,OSGUDI2ImageType_E eImageType)
{
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hSurface_D = CSUDI_NULL;
	CSUDI_HANDLE hDisplaySurface = CSUDI_NULL;
	unsigned char * pcBuff = CSUDI_NULL;
	unsigned int nImageLen = 0;
	CSUDI_Error_Code nRet;
	CSUDIOSGSurfaceInfo_S stSSurfaceInfo = {0};

	//����surface�ͻ�ȡ�Դ�surface
	nRet = CSUDIOSGCreateSurface(ePixelFormat,
									816,
									612,
									&hSurface_S
									);
	CSTK_ASSERT_TRUE_FATAL(
				(nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),
				"Step 2 Create Surface Failure !"
			);	

	nRet = CSUDIOSGCreateSurface(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,
									816,
									612,
									&hSurface_D
									);
	CSTK_ASSERT_TRUE_FATAL(
				(nRet == CSUDI_SUCCESS && hSurface_D != CSUDI_NULL),
				"Step 3 Create Surface Failure !"
			);		
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(g_nTestDisplaySurfaceIndex,&hDisplaySurface)),
				"Step 4 get display surface failure"
			);
	
	//��ȡͼƬ
	nImageLen = OSG_iGetPhoto(g_aImageFile[eImageType],&pcBuff);
	CSTK_ASSERT_TRUE_FATAL(
				((pcBuff != CSUDI_NULL) && nImageLen != 0),
				"Step 1 get photo data failure"
			);


	//YUVת��RGB	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_S, &stSSurfaceInfo) && stSSurfaceInfo.m_pvSurfaceMem != CSUDI_NULL 
				&& stSSurfaceInfo.m_nMemSize >= nImageLen),
				"Step 5 Get surface info failure !"
			);
	memcpy(stSSurfaceInfo.m_pvSurfaceMem,pcBuff,nImageLen);
		
	CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGBlit(hSurface_D,CSUDI_NULL,hSurface_S,CSUDI_NULL,EM_UDIOSG_OPMODE_COPY)),
					"Step 6 blit image failure"
			);
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGBlit(hDisplaySurface,CSUDI_NULL,hSurface_D,CSUDI_NULL,EM_UDIOSG_OPMODE_COPY)),
				"Step 7blit image failure"
			);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Step 8 snyc failure");


	CSTCPrint("������������Ļ����%sͼƬ,��ͼƬ��%sTestImage.jpgһ��,��СΪ816*612\n",g_aImageFile[eImageType],g_aImageFile[eImageType]);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()),"Step 9 Check failure"); 	
		
	CSTK_FATAL_POINT;

	OSG_iClearScreen();
	
	if(hSurface_S != CSUDI_NULL)
	{
		if(CSUDIOSGDestroySurface(hSurface_S)!=SUCCESS)
		        CSTCPrint("Step 10 destroy decode surface failure\n");	
		hSurface_S = CSUDI_NULL;
	}
	if(hSurface_D != CSUDI_NULL)
	{
		if(CSUDIOSGDestroySurface(hSurface_D)!=SUCCESS)
		       CSTCPrint("Step 11 destroy decode surface failure\n");	
		hSurface_D = CSUDI_NULL;
	}		
	if(pcBuff != CSUDI_NULL)
	{
		if(CSUDIOSFree(pcBuff)!=CSUDI_SUCCESS)
			CSTCPrint("Step 12 free photo data failure");
		pcBuff = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//�˺�����ָ����bmp���������surface
void iFillSurfaceByBmp(unsigned char *pBmpBuffer, CSUDI_HANDLE hSurface, int w, int h)
{
	int i,j;
	unsigned char *pSrc, *pDst;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;

	if(	(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo) && sSurfaceInfo.m_pvSurfaceMem != CSUDI_NULL));
	{
		pSrc = (unsigned char *)(pBmpBuffer);
        pDst = (unsigned char *)sSurfaceInfo.m_pvSurfaceMem;
        
		for(i = h - 1; i >= 0; i--)
		{
			for(j = 0; j< w; j++)
			{
                  pDst[0]= *(pSrc+ i * w * 4 + 4 * j);
                  pDst[1]= *(pSrc+ i * w * 4 + 4 * j+1);
                  pDst[2]= *(pSrc+ i * w * 4 + 4 * j+2);
                  pDst[3]= *(pSrc+ i * w * 4 + 4 * j+3);
                  pDst += 4;
			}
		}
  	}
}
    
//bmp���ݰ��յ������surface
void iFillSurfaceByBmpWithReverseOrder(const unsigned char *pBmpBuffer, CSUDI_HANDLE hSurface, int w, int h)
{
	int i,j;
	unsigned int *pSrc, *pDst;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;

	if(	(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo) && sSurfaceInfo.m_pvSurfaceMem != CSUDI_NULL));
	{
		pSrc = (unsigned int *)pBmpBuffer;
		pDst = (unsigned int *)sSurfaceInfo.m_pvSurfaceMem;
		for(i = 0; i < h; i++)
		{
			for(j = 0; j< w; j++)
			{
				*(pDst++)= *(pSrc+ i * w + j);
			}
		}
	}

}

//�˺�����Ҫ��������һ���������εľ��η���
CSUDI_BOOL OSG_iTestDrawRectDisplay(int nIndex)
{
	CSUDI_HANDLE hSurface;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;
	int w, h;
	int i;

	if(nIndex == 0)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(nIndex, &hSurface)), "��ȡ�����Դ�ʧ��!");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo) && sSurfaceInfo.m_pvSurfaceMem != CSUDI_NULL),"��ȡ�����Դ���Ϣʧ��!");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(nIndex, &hSurface)), "��ȡ�����Դ�ʧ��!");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo) && sSurfaceInfo.m_pvSurfaceMem != CSUDI_NULL),"��ȡ�����Դ���Ϣʧ��!");
	}	

	w	= sSurfaceInfo.m_nWidth;
	h 	= sSurfaceInfo.m_nHeight;

	for(i = 0; i< h/2; i++)
	{
		CSUDIOSGRect_S sDstRect;
		sDstRect.m_nX = sDstRect.m_nY = i;
		sDstRect.m_nWidth = w - i * 2;
		sDstRect.m_nHeight= h - i * 2;
		
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGFill(hSurface,&sDstRect, 0xFFFFFFFF)), "CSUDIOSGFill ʧ��!");
		i++;
		sDstRect.m_nX = sDstRect.m_nY = i;
		sDstRect.m_nWidth = w - i * 2;
		sDstRect.m_nHeight= h - i * 2;
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGFill(hSurface,&sDstRect, 0xFFFF0000)), "CSUDIOSGFill ʧ��!");
	}

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync())," Sync Failure !");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//�˺������ƽ̨�Ƿ�֧�ָ���
CSUDI_BOOL OSG_iTestSupportHDDisplay()
{
	char cBuf[32] = {0};
	CSTKCfgError_E nResult;
	int nValue;
	
	memset(cBuf, '\0', sizeof(cBuf));
	
	nResult = CSTKGetConfigInfo("OSG","CS_OSGUDI2_SURPORT_HD_SD_DISPLAY",cBuf,sizeof(cBuf));

	if(CS_TK_CONFIG_SUCCESS != nResult)
	{
		CSTCPrint("��ȡCS_OSGUDI2_SURPORT_HD_SD_DISPLAY������ʧ��\n");
		return nResult;
	}
	
	nValue = CSTKGetIntFromStr(cBuf,10);

	if(nValue != 1)
	{
		return CSUDI_FALSE;
	}

	return CSUDI_TRUE;
}

//�˺�����Ҫ����������Ļ�Ƿ���ƫ��
CSUDI_BOOL OSG_iTestScreenOffset()
{
	CSUDI_BOOL bSupportHD = OSG_iTestSupportHDDisplay();

	if (!OSG_IsShareHDDisplaySurface())
	{
		OSG_iTestDrawRectDisplay(0);//���Ա���
	}

	if(bSupportHD == CSUDI_TRUE)
	{
		OSG_iTestDrawRectDisplay(1);//���Ը���
	}
	CSTCPrint("�����εĿ��Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()),
				"��Ļ����ʼ������ƫ��!");
	
	CSTK_FATAL_POINT;
	//����
 	OSG_iClearScreen();		
	return CSUDI_TRUE;	
}

//�˺�����Ҫ����ģ������Ƶ��ƶ�
CSUDI_BOOL OSG_iTestMarquee()
{
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hfontsurface = CSUDI_NULL;	
	CSUDI_HANDLE hSurface_D = CSUDI_NULL;
	
	unsigned char * pcBuff = CSUDI_NULL;
	unsigned int nImageLen = 0;
	char *pcName = "UDI2OSG_Marquee.bmp";	

	int i;
	int nCycles = 0;
	int nWidth, nHeight;
	nWidth = 720;
	nHeight = 40;

	nImageLen = OSG_iGetPhoto(pcName,&pcBuff);
	
	CSTK_ASSERT_TRUE_FATAL(((pcBuff != CSUDI_NULL) && nImageLen != 0),
				"Step 1 get photo data failure");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIOSGCreateSurface(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,
									768,
									49,
									&hfontsurface
									), "step 2 CSUDIOSGCreateSurface failed \n");

	iFillSurfaceByBmp(pcBuff + 54, hfontsurface, 768, 49);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIOSGCreateSurface(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,
									4000,
									50,
									&hSurface_S
									), "step 3 CSUDIOSGCreateSurface failed \n");	

	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(g_nTestDisplaySurfaceIndex,&hSurface_D)),"Step 6 get display surface failure");

	CSTCPrint("��ע����Ļ���Ƿ����������Ļ������5��\n");
	CSTCPrint("��ע��۲��������ʾЧ���Ƿ���ȷ: \n");
	CSTCPrint("������ɫΪ��ɫ������������ʼ����Ϊ(0,200)�����Ϊ(720,40)��\n");
	CSTCPrint("������ɫΪ��ɫ����������������ң�����Ϊ\"ĺȻ�䣬���������ߣ���ӽ��������������ڵ���һ���Ҳ��������\"��\n");
	CSTKWaitAnyKey();
	
	for(i = 0; i< 4000; i++)
	{
		CSUDIOSGRect_S sRect1, sRect2, sDstRect;
		sRect1.m_nX = sRect1.m_nY = 0;
		sRect1.m_nWidth = nWidth;
		sRect1.m_nHeight = nHeight;	
		sRect2.m_nX = i;
		sRect2.m_nY = 0;
		
		sRect2.m_nWidth = nWidth;
		sRect2.m_nHeight = nHeight;	
		sDstRect.m_nWidth = nWidth;
		sDstRect.m_nHeight = nHeight;
		sDstRect.m_nX = 0;
		sDstRect.m_nY = 200;
		
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGFill(hSurface_S,CSUDI_NULL,0xFFFFFFFF)),
					"Step 4 CSUDIOSGFill failure");	

		if(sRect2.m_nX + sRect2.m_nWidth > nWidth)
		{
			sRect1.m_nWidth = sRect2.m_nWidth = nWidth - sRect2.m_nX;
		}	
		
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGBlit(hSurface_S,&sRect2,hfontsurface,&sRect1,EM_UDIOSG_OPMODE_ALPHA)),
					"Step 5 blit hfontsurface failure");			

		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGBlit(hSurface_D,&sDstRect,hSurface_S,&sRect1,EM_UDIOSG_OPMODE_COPY)),
					"Step 7 blit hSurface_S failure");
		
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Step 8 snyc failure");

		CSUDIOSThreadSleep(5);

		if(i > 400)
		{
			i = 0;
			nCycles++;
			CSTCPrint("-----������Ѿ�������%d��----\n", nCycles);
			
			if(nCycles >= 5)//�ܹ������
				break;
		}		
		
	}

	CSTCPrint("���Ƿ񿴵����������Ļ�Ϲ�����5��?\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()),"Step 9 Check failure");	

	CSTCPrint("�������ʾЧ���Ƿ���ȷ?\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "�������ʾЧ������ȷ");
	
	CSTK_FATAL_POINT;
	if(hSurface_S != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface_S)),"Step 10 destroy hSurface_S surface failure!");
		hSurface_S = CSUDI_NULL;
	}
	
	if(hfontsurface != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hfontsurface)),"Step 10 destroy hfontsurface surface failure!");
		hfontsurface = CSUDI_NULL;
	}
	
	if(pcBuff != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSFree(pcBuff)),"Step 10 free photo data failure!");
		pcBuff = CSUDI_NULL;
	}

	//����
	OSG_iClearScreen();
	return CSUDI_TRUE;
}


//�˺�����Ҫ����������Ļ���Ƿ������صľ�ݲ���
CSUDI_BOOL OSG_iTestScreenSerrate()
{
	CSUDI_HANDLE hSurface_S1 = CSUDI_NULL;
	CSUDI_HANDLE hSurface_S2 = CSUDI_NULL;
	CSUDI_HANDLE hSurface_SD = CSUDI_NULL;
	CSUDI_HANDLE hSurface_HD = CSUDI_NULL;
	unsigned char * pcBuff1 = CSUDI_NULL;
	unsigned char * pcBuff2 = CSUDI_NULL;
	unsigned int nImageLen = 0;
	char *pcName1 = "UDI2OSG_search.bmp";	
	char *pcName2 = "UDI2OSG_avsetting.bmp";
	CSUDIOSGRect_S sRect1;
	int i;
	CSUDI_BOOL bSupportHD = OSG_iTestSupportHDDisplay();
	int nRepeatTime = 10;

	sRect1.m_nX = sRect1.m_nY = 0;
	sRect1.m_nWidth = 1280;
	sRect1.m_nHeight = 600;	//�˴�С��720������Ϊ�������ʱ�򣬾�ݸ�����

	nImageLen = OSG_iGetPhoto(pcName1,&pcBuff1);
	CSTK_ASSERT_TRUE_FATAL(((pcBuff1 != CSUDI_NULL) && nImageLen != 0),
				"get search.bmp data failure");	
	
	nImageLen = OSG_iGetPhoto(pcName2,&pcBuff2);
	CSTK_ASSERT_TRUE_FATAL(((pcBuff2 != CSUDI_NULL) && nImageLen != 0),
				"get avsetting.bmp data failure");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIOSGCreateSurface(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,
									1280,
									720,
									&hSurface_S1
									), "CSUDIOSGCreateSurface failed \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIOSGCreateSurface(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,
									1280,
									720,
									&hSurface_S2
									), "CSUDIOSGCreateSurface failed \n");	
	
	iFillSurfaceByBmp(pcBuff1 + 54, hSurface_S1, 1280, 720);
	iFillSurfaceByBmp(pcBuff2 + 54, hSurface_S2, 1280, 720);

	if(!OSG_IsShareHDDisplaySurface())
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(0,&hSurface_SD)),
					"��ȡ�����Դ�ʧ��!");
	}

	if(bSupportHD == CSUDI_TRUE)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(1,&hSurface_HD)),
					"��ȡ�����Դ�ʧ��!");
	}

	if(!OSG_IsShareHDDisplaySurface())
	{
		CSTCPrint("��ע��۲�������ͼƬˢ�¹������Ƿ������ؾ��\n");
		CSTKWaitAnyKey();

		for (i=0; i<nRepeatTime; i++)
		{
			//����ƽ̨
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGBlit(hSurface_SD,NULL,hSurface_S1,&sRect1,EM_UDIOSG_OPMODE_COPY)),
						"blit hSurface_S1 to SD failure");
			
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"snyc failure");	

			CSUDIOSThreadSleep(1000);

			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGBlit(hSurface_SD,NULL,hSurface_S2,&sRect1,EM_UDIOSG_OPMODE_COPY)),
						"blit hSurface_S2 to SD failure");	

			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"snyc failure");	

			CSUDIOSThreadSleep(1000);
		}

		CSTCPrint("�����������û�����صľ������!! \r\n");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "������Ļˢ�µ�ʱ���ݺ�����!");	
	}
	else
	{
		CSTCPrint("�߱���ͬԴ���۲�����������!! \r\n");
	}

	//����ƽ̨
	if(bSupportHD == CSUDI_TRUE)
	{
		CSTCPrint("��ע��۲�������ͼƬˢ�¹������Ƿ������ؾ��\n");
		CSTKWaitAnyKey();

		for (i=0; i<nRepeatTime; i++)
		{
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGBlit(hSurface_HD,NULL,hSurface_S1,&sRect1,EM_UDIOSG_OPMODE_COPY)),
						"blit hSurface_S1 to HD failure");			
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"snyc failure");

			CSUDIOSThreadSleep(1000);
			
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGBlit(hSurface_HD,NULL,hSurface_S2,&sRect1,EM_UDIOSG_OPMODE_COPY)),
						"blit hSurface_S2 to HD failure");
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"snyc failure");

			CSUDIOSThreadSleep(1000);
		}

		CSTCPrint("�����������û�����صľ������!! \r\n");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "������Ļˢ�µ�ʱ���ݺ�����!");	
	}
	
	CSTK_FATAL_POINT;
	//����surface��Դ
	if(hSurface_S1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDIOSGDestroySurface(hSurface_S1) == CSUDI_SUCCESS),"destroy hSurface_S1  failure\n");
		hSurface_S1 = CSUDI_NULL;
	}	
	
	if(hSurface_S2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDIOSGDestroySurface(hSurface_S2) == CSUDI_SUCCESS),"destroy hSurface_S2  failure\n");
		hSurface_S2 = CSUDI_NULL;
	}
	//����ͼƬ��Դ
	if(pcBuff1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDIOSFree(pcBuff1) == CSUDI_SUCCESS),"free photo data1 failure\n");
		pcBuff1 = CSUDI_NULL;
	}
	
	if(pcBuff2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDIOSFree(pcBuff2) == CSUDI_SUCCESS),"free photo data2 failure\n");
		pcBuff2 = CSUDI_NULL;
	}
	
	//����
	OSG_iClearScreen();
	return CSUDI_TRUE;
}

//�˺������������Ļˢ���Ƿ��ж�ʧ�����
CSUDI_BOOL OSG_iTestScreenlost()
{
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hSurface_SD = CSUDI_NULL;
	CSUDI_HANDLE hSurface_HD = CSUDI_NULL;
	unsigned char * pcBuff = CSUDI_NULL;
	unsigned int nImageLen = 0;
	char *pcName = "UDI2OSG_search.bmp";	
	CSUDIOSGRect_S sRect;

	CSUDIOSGSurfaceInfo_S sSurfaceInfoSD;
	CSUDIOSGSurfaceInfo_S sSurfaceInfoHD;
	CSUDI_BOOL bSupportHD = CSUDI_FALSE;
	int w, h;
	int i, j;
	int nOffset = 54;//bmp��Ϣͷ�ĳ��ȣ������׵������bmp��ʽͼƬ��׼
	
	w = 1280;
	h = 720;	
	bSupportHD = OSG_iTestSupportHDDisplay();

	sRect.m_nX = sRect.m_nY = 0;
	sRect.m_nWidth  = 16;
	sRect.m_nHeight = 16;	//����16*16��С���񽻻�����	
	
	/************����ͼƬ��ָ����surface***************/
	nImageLen = OSG_iGetPhoto(pcName,&pcBuff);	
	CSTK_ASSERT_TRUE_FATAL(((pcBuff != CSUDI_NULL) && nImageLen != 0),
				"Step 2 get photo data failure");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIOSGCreateSurface(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,
									w,
									h,
									&hSurface_S
									), "CSUDIOSGCreateSurface failed \n");
	
	iFillSurfaceByBmp(pcBuff + nOffset, hSurface_S, w, h);

	/************��ȡ�߱������Ϣ***************/
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(g_nTestDisplaySurfaceIndex,&hSurface_SD)),
				"��ȡ�����Դ�ʧ��!");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_SD, &sSurfaceInfoSD) && sSurfaceInfoSD.m_pvSurfaceMem != CSUDI_NULL),"CSUDIOSGGetSurfaceInfo  SD failed !! \r\n")
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGFill(hSurface_SD, NULL, 0xFFFF0000)," fill �����Դ�ʧ��");

	if(bSupportHD == CSUDI_TRUE)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(1,&hSurface_HD)),
					"��ȡ�����Դ�ʧ��!");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_HD, &sSurfaceInfoHD) && sSurfaceInfoHD.m_pvSurfaceMem != CSUDI_NULL),"CSUDIOSGGetSurfaceInfo  HD failed !! \r\n")
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGFill(hSurface_HD, NULL, 0xFFFF0000)," fill �����Դ�ʧ��");
	}	

	/************����16*16�ķ��񽻻����ݵ����������***************/	
	for(i = 0; i< h; i++)
	{
		for(j = 0; j< w; j++)
		{
			sRect.m_nX  = j;
			sRect.m_nY  = i;

			j +=15;

			if(sRect.m_nX + sRect.m_nWidth <= sSurfaceInfoSD.m_nWidth && sRect.m_nY + sRect.m_nHeight <= sSurfaceInfoSD.m_nHeight)
			{
				//����ƽ̨
				CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGBlit(hSurface_SD,&sRect,hSurface_S,&sRect,EM_UDIOSG_OPMODE_COPY)),
							"blit���ݵ������Դ�ʧ��!");
				CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"snyc failure");	
			}

			if(bSupportHD == CSUDI_TRUE)//֧�ָ���ƽ̨
			{
				if(sRect.m_nX + sRect.m_nWidth <= sSurfaceInfoHD.m_nWidth && sRect.m_nY + sRect.m_nHeight <= sSurfaceInfoHD.m_nHeight)
				{
					CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGBlit(hSurface_HD,&sRect,hSurface_S,&sRect,EM_UDIOSG_OPMODE_COPY)),
								"blit���ݵ������Դ�ʧ��!");
					CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"snyc failure");	
				}
			}			
			
		}		
		i += 15;
	}

	CSTCPrint("��Ļ��û�к�ɫ�Ĳ����ĵ� !! \r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()),"����Ļ�Ϸ����˺�ɫ�Ĳ����ĵ�!");	
	
	CSTK_FATAL_POINT;
	//������Դ
	if(hSurface_S != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDIOSGDestroySurface(hSurface_S) == CSUDI_SUCCESS),"destroy hSurface_S  failure\n");
		hSurface_S = CSUDI_NULL;
	}

	//�ͷ�ͼƬ����
	if(pcBuff != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDIOSFree(pcBuff) == CSUDI_SUCCESS), "free photo data failure");
		pcBuff = CSUDI_NULL;
	}
	
	//����
	OSG_iClearScreen();
	return CSUDI_TRUE;
}

CSUDI_BOOL OSG_iFillAndSync(CSUDI_HANDLE hSurface, CSUDIOSGRect_S *psRect, unsigned int unColor)
{
	CSUDI_BOOL bRet = CSUDI_FALSE;

	if (hSurface == CSUDI_NULL)
	{
		return bRet;
	}
	
	if(CSUDI_SUCCESS == CSUDIOSGFill(hSurface, psRect, unColor))
	{
		if(CSUDI_SUCCESS == CSUDIOSGSync())
		{
			bRet = CSUDI_TRUE;
		}
		else
		{
			CSTCPrint("ͬ�� surfaceʧ��!\n");
		}
	}
	else
	{
		CSTCPrint("Fill surfaceʧ��!\n");
	}
	
	return bRet;
}

CSUDI_BOOL OSG_iBlitAndSync(CSUDI_HANDLE hDstSurface, CSUDIOSGRect_S *psDstRect, 
	CSUDI_HANDLE hSrcSurface, CSUDIOSGRect_S *psSrcRect, CSUDIOSGOpMode_E eOpMode)
{
	CSUDI_BOOL bRet = CSUDI_FALSE;

	if (hDstSurface == CSUDI_NULL || hSrcSurface == CSUDI_NULL
		|| (eOpMode < EM_UDIOSG_OPMODE_COPY || eOpMode > EM_UDIOSG_OPMODE_ALPHA))
	{
		return bRet;
	}
	
	if(CSUDI_SUCCESS == CSUDIOSGBlit(hDstSurface, psDstRect, hSrcSurface, psSrcRect, eOpMode))
	{
		if(CSUDI_SUCCESS == CSUDIOSGSync())
		{
			bRet = CSUDI_TRUE;
		}
		else
		{
			CSTCPrint("ͬ�� surfaceʧ��!\n");
		}
	}
	else
	{
		CSTCPrint("Blit surfaceʧ��!\n");
	}
	
	return bRet;
}

int OSG_iGetPixelDepthByPixelFormat(CSUDIOSGPixelFormat_E ePixelFmt)
{
	int nRet = 0;
	switch(ePixelFmt)
	{
	case EM_UDIOSG_PIXEL_FORMAT_PALETTE8:
		nRet = 8;
		break;		
	case EM_UDIOSG_PIXEL_FORMAT_RGB_565:     ///< 16-bit, no per-pixel alpha 
	case EM_UDIOSG_PIXEL_FORMAT_ARGB_1555:   ///<  16-bit 
	case EM_UDIOSG_PIXEL_FORMAT_RGBA_5551:   ///<  16-bit 
	case EM_UDIOSG_PIXEL_FORMAT_ARGB_4444:  ///<  16-bit 
	case EM_UDIOSG_PIXEL_FORMAT_RGBA_4444:  ///<  16-bit 
	case EM_UDIOSG_PIXEL_FORMAT_YCBCR888_444:///< Y, Cb, Cr signed components with 8 bit precision in a 4:4:4 sampling model
	case EM_UDIOSG_PIXEL_FORMAT_YCBCR888_422: ///< Y, Cb, Cr signed components with 8 bit precision in a 4:2:2 sampling model
	case EM_UDIOSG_PIXEL_FORMAT_YCBCR888_420: ///< Y, Cb, Cr signed components with 8 bit precision in a 4:2:0 sampling model
	case EM_UDIOSG_PIXEL_FORMAT_YCBCR888_411: ///< Y, Cb, Cr signed components with 8 bit precision in a 4:1:1 sampling model
		nRet = 16;
		break;
	case EM_UDIOSG_PIXEL_FORMAT_RGB_888:
		nRet = 24;
		break;
	case EM_UDIOSG_PIXEL_FORMAT_ARGB_8888:
		nRet = 32;
		break;
	default:
		CSTCPrint("��ʽ ����δ֪����!\n");
		break;
	}

	return nRet;
}

CSUDI_BOOL OSG_iSetSurfaceBuf(CSUDI_HANDLE hSurface, unsigned char * pucImage, 
	int  nImageSize, int nImageWidth, int nImageHeight)
{
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;
	int nPitch;
	unsigned char* pSurfaceMem = NULL;
	unsigned char* pColorBuf = NULL;
	int nCopyLen = 0;
	int ii;
	int nSurfacePixelDepth = 0;
	int nImageColorDepth;

	CSTK_ASSERT_TRUE_FATAL(hSurface != CSUDI_NULL, 
		"�����������!");

	CSTK_ASSERT_TRUE_FATAL(pucImage != CSUDI_NULL, 
		"�����������!");

	CSTK_ASSERT_TRUE_FATAL((nImageSize > 0) && (nImageWidth > 0) && (nImageHeight > 0), 
		"�����������!");

	nImageColorDepth = (*(pucImage+0x1D)<<8) |*(pucImage+0x1C);
	
	nPitch = nImageWidth * (nImageColorDepth>>3);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo), 
		"��ȡsurface��Ϣʧ��!");

	pSurfaceMem = (unsigned char*)sSurfaceInfo.m_pvSurfaceMem;
	CSTK_ASSERT_TRUE_FATAL(NULL != pSurfaceMem, "surface ������Ϊ��!");
	
	nSurfacePixelDepth = OSG_iGetPixelDepthByPixelFormat(sSurfaceInfo.m_ePixelFormat);
	if(nSurfacePixelDepth == nImageColorDepth)
	{
		nCopyLen = (sSurfaceInfo.m_nHeight*sSurfaceInfo.m_nPitch > nPitch* nImageHeight) ?  nPitch* nImageHeight : sSurfaceInfo.m_nHeight*sSurfaceInfo.m_nPitch;
		pColorBuf = pucImage + 54 + nCopyLen;	
		for(ii=0; ii < sSurfaceInfo.m_nHeight && pColorBuf > (pucImage + 54 - nCopyLen); ii++)
		{
			pColorBuf -= nPitch;
			memcpy(pSurfaceMem, pColorBuf, nPitch);			
			pSurfaceMem += sSurfaceInfo.m_nPitch;		
		}
	}
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

