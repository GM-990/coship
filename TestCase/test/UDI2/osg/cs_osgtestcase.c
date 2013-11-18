/**
 *@version 1.0.8 2009/10/09 �����µĲ��Կ�ܽ����ع�
 *@version 1.0.7 2009/09/30 �ع�Ϊ��֧���µĿ��
 *@version 1.0.6 2009/09/15 ȥ����ȫ����������Ļblit���������޸�alphaΪ0ʱ������������
 *@version 1.0.5 2009/09/03 �����߶����޸�������ƽ̨�ϱ��벻���ı�������λ������
 *@version 1.0.4 2009/08/29 �ӿڸ��ģ���������ͬ��
 *@version 1.0.3 2009/07/17 ��ɳ�ʼ�汾
 *@version 1.0.2 2009/07/18 ����Ԥ��������
 *@version 1.0.1 2009/07/27 ��������������
 */
/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
-----------------------------------------------------------------------*/
#include "cs_osgtestcase.h"
#include "cs_osgtest.h"
#include <time.h>
#include "udi2_player.h"
#include "barportal.h"
#include "coshipportal.h"
#include "udi2_screen.h"


//blit�����ظ�ִ�еĴ���
#define TEST_MAX_BLIT_TIME  (2<<10)

extern unsigned int g_uSurfaceMaxWidth;
extern unsigned int g_uSurfaceMaxHeight;
extern unsigned char bmp32data_h[1658882];


//@TESTCASEID: CSTC_OSG_TEST_Init
//@CASEGROUP: 
//@DESCRIPTION:��ʼ�����Ի���,��ȡ�����ļ���Ϣ��
//@DESCRIPTION:�����ļ���Ϣ����ƽ̨֧�ֵ���ɫģʽ��ƽ̨�Ƿ�֧�ָ߱����Դ�ͬʱ���ڣ�ƽ̨֧�ִ�������󻺴�ռ��С��
//@REMARK:�ýӿڱ�����ϵͳ��ʼ��ʱ���á�
CSUDI_BOOL CSTC_OSG_Init(void)
{
	//�ڱ�����������ִ��ǰ����
	return (CS_TK_CONFIG_SUCCESS == OSG_iInitCfg());
}

//@TESTCASEID:CSTC_OSG_TEST_UnInit
//@CASEGROUP:
//@DESCRIPTION:ȥ��ʼ�����Ի���
CSUDI_BOOL CSTC_OSG_UnInit(void)
{
	//�ڱ�����������ִ�к����
	return (CS_TK_CONFIG_SUCCESS == OSG_iUnInitCfg());
}

//@TESTCASEID:CSTC_OSG_IT_CreateSurface_0001
//@CASEGROUP:CSUDIOSGCreateSurface
//@DESCRIPTION:���Դ�����ͼƽ��ʱ����Ƿ��Ĳ���
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@EXPECTATION:  ������ͼƽ��ʧ�ܣ�����ΪCSUDIOSG_ERROR_BAD_PARAMETER 
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface�����Ƿ���ͼ����������ƽ��ʧ��
CSUDI_BOOL CSTC_OSG_IT_CreateSurface_0001(void)
{   
	CSUDI_HANDLE				hSurface = CSUDI_NULL;
	CSUDIOSGPixelFormat_E 		eErrorPixelFormat[] = {-1, EM_UDIOSG_PIXEL_FORMAT_NUM};
	int 						nErrorWidth[] = {-1, 0};
	int 						nErrorHeight[] = {-1, 0};
	int							i = 0;
	CSUDI_Error_Code			nResult = CSUDI_FAILURE;
	CSUDIOSGPixelFormat_E		ePixelFormat = -1;

	//Ѱ��һ��֧�ֵ�ePixelFormat
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);
	
	//��phSurface Ϊ�յļ��
	for(i = 0; i < 1; i++)
	{
		nResult = CSUDIOSGCreateSurface(ePixelFormat, 1, 1, CSUDI_NULL);
		CSTK_ASSERT_TRUE_FATAL((CSUDIOSG_ERROR_BAD_PARAMETER == nResult), "��phSurface �����жϴ���");
	}
	//�Դ���ePixelFormat �ļ��
	for(i = 0; i < sizeof(eErrorPixelFormat)/sizeof(eErrorPixelFormat[0]); i++)
	{
		nResult = CSUDIOSGCreateSurface(eErrorPixelFormat[i], 1, 1, &hSurface);
		CSTK_ASSERT_TRUE_FATAL((CSUDIOSG_ERROR_BAD_PARAMETER == nResult)&&(CSUDI_NULL == hSurface), "��ePixelFormat �����жϴ���");
	}
	//�Դ���nWidth �ļ��
	for(i = 0; i < sizeof(nErrorWidth)/sizeof(nErrorWidth[0]); i++)
	{
		nResult = CSUDIOSGCreateSurface(ePixelFormat, nErrorWidth[i], 1, &hSurface);
		CSTK_ASSERT_TRUE_FATAL((CSUDIOSG_ERROR_BAD_PARAMETER == nResult)&&(CSUDI_NULL == hSurface), "��nWidth �����жϴ���");
        if((CSUDIOSG_ERROR_BAD_PARAMETER == nResult)&&(CSUDI_NULL == hSurface))
		{
			CSTCPrint("����Ĳ���nWidth =%d\n",nErrorWidth[i]);
        }
    }
	//�Դ���Height �ļ��
	for(i = 0; i < sizeof(nErrorHeight)/sizeof(nErrorHeight[0]); i++)
	{
		nResult = CSUDIOSGCreateSurface(ePixelFormat, 1, nErrorHeight[i], &hSurface);
		CSTK_ASSERT_TRUE_FATAL((CSUDIOSG_ERROR_BAD_PARAMETER == nResult)&&(CSUDI_NULL == hSurface), "��nHeight �����жϴ���");
        if((CSUDIOSG_ERROR_BAD_PARAMETER == nResult)&&(CSUDI_NULL == hSurface))
        {
		  	CSTCPrint("����Ĳ���nHeight =%d\n",nErrorHeight[i]);
        }
	}

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_CreateSurface_0002
//@CASEGROUP:CSUDIOSGCreateSurface
//@DESCRIPTION:ȷ�Ͽ�����100��10*10��surface.ʹ�������ļ��е�CS_OSGUDI2_SURPORT_HD_SD_DISPLAY�ֶ����ָ߱������.�ɸ��ֶ����ִ���ȫ��surface�Ĵ�С.ȷ�Ͽ���������ȫ��surface�ĸ���
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@EXPECTATION:  ������ͼƽ��ɹ�
//@EXECUTIONFLOW:1�����û�ȡ�����ļ��ӿڣ���CS_OSGUDI2_SURPORT_HD_SD_DISPLAY�ֶ����ָ߱�����棬ȷ�������С
//@EXECUTIONFLOW:2������CSUDIOSGCreateSurface������ͼ����ɫģʽ��EM_UDIOSG_PIXEL_FORMAT_ARGB_8888��EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,��С��10 * 10,��������ƽ��ɹ�
//@EXECUTIONFLOW:3��ѭ�����ò���1��100��.
//@EXECUTIONFLOW:4��ѭ������CSUDIOSGDestroySurface,���ٲ���1-2���ɵ�surface
//@EXECUTIONFLOW:5������CSUDIOSGCreateSurface������ͼ����ɫģʽ��EM_UDIOSG_PIXEL_FORMAT_ARGB_8888��EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,��С��ȫ��,��������ƽ��ɹ�
//@EXECUTIONFLOW:6��ѭ�����ò���4,�����ò���4ʧ��ʱ��ӡ���ò���4�Ĵ���.
//@EXECUTIONFLOW:7��ѭ������CSUDIOSGDestroySurface,���ٲ���4���ɵ�surface
CSUDI_BOOL CSTC_OSG_IT_CreateSurface_0002(void)
{
	int i = 0;
	CSUDI_Error_Code nResult = -1;
	CSUDI_HANDLE hSurface[100];
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	int nWidth = 0;
	int nHeight = 0;
	int nValue = -1;
	char cBuf[32] = {0};
	
	CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
					"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
				);
	
	for(i = 0; i < 100; i++)
	{
		hSurface[i] = CSUDI_NULL;
	}
	memset(cBuf, '\0', sizeof(cBuf));
	nResult = CSTKGetConfigInfo("OSG","CS_OSGUDI2_SURPORT_HD_SD_DISPLAY",cBuf,sizeof(cBuf));//OSG_GetValueofCfg("CS_OSGUDI2_SURPORT_HD_SD_DISPLAY");
	if(CS_TK_CONFIG_SUCCESS != nResult)
	{
		return CSUDI_FALSE;
	}
	nValue = CSTKGetIntFromStr(cBuf,10);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == OSG_GetSize(nValue, &nWidth,&nHeight)), "�жϸ߱������!!!!!");
	CSTCPrint("�߱����ʶ=%d , surface Width =%d , Height =%d\n",nValue,nWidth,nHeight);
	
	for(i = 0; i < 100; i++)
	{
		nResult = CSUDIOSGCreateSurface(ePixelFormat, 10, 10, &hSurface[i]);
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult), "surface����ʧ��");
	}
	for(i = 0; i < 100; i++)
	{
		if( hSurface[i] != CSUDI_NULL)
		{
			nResult = CSUDIOSGDestroySurface( hSurface[i]);
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult), "surface����ʧ��");
			hSurface[i] = CSUDI_NULL;
		}
	}
	for(i = 0; i < 100; i++)
	{
		nResult = CSUDIOSGCreateSurface(ePixelFormat, nWidth, nHeight, &hSurface[i]);
		if(CSUDI_SUCCESS != nResult)
		{
			break;
		}
	}
	CSTCPrint("��������%d��ȫ��surface(����ƽ̨Ҫ�������ܴ���5��������ƽ̨10��)\n",i);
	
	for(i = 0; i < 100; i++)
	{
		if( hSurface[i] != CSUDI_NULL)
		{
			nResult = CSUDIOSGDestroySurface( hSurface[i]);
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult), "surface����ʧ��");
			hSurface[i] = CSUDI_NULL;
		}
	}	

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_CreateSurface_0003
//@CASEGROUP:CSUDIOSGCreateSurface
//@DESCRIPTION:����ƽ̨�ɴ���ȫ��surface����������ƽ̨Ҫ�������ܹ�����10��ȫ��surface������ƽ̨Ҫ�������ܹ����� 5��ȫ����Surface
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@EXPECTATION:  ������ͼƽ��ɹ�
//@EXECUTIONFLOW:1�����û�ȡ�����ļ��ӿڣ���CS_OSGUDI2_SURPORT_HD_SD_DISPLAY�ֶ����ָ߱�����棬ȷ�������С
//@EXECUTIONFLOW:2������Ǹ���ƽ̨������CSUDIOSGCreateSurface����������ͼƽ��100������ɫģʽ��ARGB8888��ARGB1555����С��ȫ��
//@EXECUTIONFLOW:3���жϳɹ������ĸ��������С��4���򷵻�ʧ��
//@EXECUTIONFLOW:4��ѭ������CSUDIOSGDestroySurface�����ٲ���2���ɵ�surface
//@EXECUTIONFLOW:5������Ǳ���ƽ̨������CSUDIOSGCreateSurface����������ɫģʽ��EARGB8888��ARGB1555����С��ȫ���Ļ�ͼƽ��100������С��720*24�Ļ�ͼƽ��100��
//@EXECUTIONFLOW:6���жϲ���5�ɹ�������ͼƽ��ĸ��������С��2���򷵻�ʧ��
//@EXECUTIONFLOW:7��ѭ������CSUDIOSGDestroySurface�����ٲ���5���ɵ�surface
CSUDI_BOOL CSTC_OSG_IT_CreateSurface_0003(void)
{
	int i = 0;
	CSUDI_Error_Code nResult = -1;
	CSUDI_HANDLE hSurface[100];
	CSUDI_HANDLE hSurfaceD[100];//720*24
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	int nWidth = 0;
	int nHeight = 0;
	int nValue = -1;
	char cBuf[32] = {0};
	
	CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
					"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
				);

	for(i = 0; i < 100; i++)
	{
		hSurface[i] = CSUDI_NULL;
		hSurfaceD[i] = CSUDI_NULL;
	}
	
	memset(cBuf, '\0', sizeof(cBuf));
	nResult = CSTKGetConfigInfo("OSG","CS_OSGUDI2_SURPORT_HD_SD_DISPLAY",cBuf,sizeof(cBuf));//OSG_GetValueofCfg("CS_OSGUDI2_SURPORT_HD_SD_DISPLAY");
	if(CS_TK_CONFIG_SUCCESS != nResult)
	{
		return CSUDI_FALSE;
	}
	
	nValue = CSTKGetIntFromStr(cBuf,10);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == OSG_GetSize(nValue, &nWidth,&nHeight)), "�жϸ߱������!!!!!");

	if(nValue ==1)
	{
		nResult = -1;
		for(i = 0; i < 100; i++)
		{
			nResult = CSUDIOSGCreateSurface(ePixelFormat, nWidth, nHeight, &hSurface[i]);
			if(CSUDI_SUCCESS != nResult &&hSurface[i]==CSUDI_NULL)
			{
				break;
			}
		}
		CSTK_ASSERT_TRUE_FATAL(i >=4,"����ȫ������surface����������Ҫ��");	
	}
	else if(nValue ==0)
	{
		for(i = 0; i < 100; i++)
		{
			nResult = CSUDIOSGCreateSurface(ePixelFormat, nWidth, nHeight, &hSurface[i]);
			if(CSUDI_SUCCESS != nResult&&hSurface[i]==CSUDI_NULL)
			{
				break;
			}
			nResult = CSUDIOSGCreateSurface(ePixelFormat, 720, 24, &hSurfaceD[i]);
			if(CSUDI_SUCCESS != nResult&&hSurfaceD[i]==CSUDI_NULL)
			{
				break;
			}
		}
		CSTK_ASSERT_TRUE_FATAL(i >=2,"����surface����������Ҫ��");
	}

	CSTK_FATAL_POINT;
		
	for(i = 0; i < 100; i++)
	{
		if( hSurface[i] != CSUDI_NULL)
		{
			nResult = CSUDIOSGDestroySurface( hSurface[i]);
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult), "surface����ʧ��");
			hSurface[i] = CSUDI_NULL;
				
		}
		if( hSurfaceD[i] != CSUDI_NULL)
		{
			nResult = CSUDIOSGDestroySurface( hSurfaceD[i]);
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult), "surface����ʧ��");
			hSurfaceD[i] = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_CreateSurface_0004
//@CASEGROUP:CSUDIOSGCreateSurface
//@DESCRIPTION:����ƽ̨����֧��ARGB1555����ARGB8888��ɫģʽ�е�һ��
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@EXPECTATION:  ����ARGB1555����ARGB8888��surface�ɹ�
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface����720*576��ARGB1555 surface
//@EXECUTIONFLOW:2������CSUDIOSGCreateSurface����720*576��ARGB8888 surface
//@EXECUTIONFLOW:3���жϳɹ�����1�Ͳ���2������һ���ǳɹ���,���ҷ��صľ����Ϊ��
//@EXECUTIONFLOW:4������CSUDIOSGDestroySurface���ٴ����ɹ���surface
CSUDI_BOOL CSTC_OSG_IT_CreateSurface_0004(void)
{
	CSUDI_HANDLE	hSurface1555 =  CSUDI_NULL;
	CSUDI_HANDLE	hSurface8888 =  CSUDI_NULL;
	CSUDI_Error_Code nResult = -1;
	CSUDI_BOOL 		bLastResult;
	CSUDIOSGPixelFormat_E ePixelFormat1555 = EM_UDIOSG_PIXEL_FORMAT_ARGB_1555;
	CSUDIOSGPixelFormat_E ePixelFormat8888 = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;	
	int nWidth 		= 720;
	int nHeight 	= 576;

	nResult = CSUDIOSGCreateSurface(ePixelFormat1555, nWidth, nHeight, &hSurface1555);
	if(nResult == CSUDI_SUCCESS)
	{
		bLastResult = CSUDI_TRUE;
	}
	
	if(hSurface1555 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface1555)), "step 4 ����1555��ɫģʽsurfaceʧ��!");
	}
	
	nResult = CSUDIOSGCreateSurface(ePixelFormat8888, nWidth, nHeight, &hSurface8888);
	if(nResult == CSUDI_SUCCESS)
	{
		bLastResult = CSUDI_TRUE;
	}

	if(hSurface8888 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface8888)), "step 4 ����8888��ɫģʽsurfaceʧ��!");
	}	

	CSTK_FATAL_POINT;
	
	return bLastResult;	
}

//@TESTCASEID:CSTC_OSG_IT_CreateSurface_0005
//@CASEGROUP:CSUDIOSGCreateSurface
//@DESCRIPTION:1.����ƽ̨֧�ִ��������Դ��С��surface
//@DESCRIPTION:2.����ƽ̨֧�ִ���ϸ����3000*50��ϸ��50*3000��surface
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@EXPECTATION:  ����surface �ɹ�
//@EXECUTIONFLOW:1����ȡƽ̨���Դ�߿�,����ΪDis_Width,Dis_Weight,ע������Ǹ���ƽ̨Ӧ���Ը����ִ���ΪDis_Width,Dis_Weight
//@EXECUTIONFLOW:2������CSUDIOSGCreateSurface����(Dis_Width+100,Dis_Weight+100) ��surface,�����ɹ�
//@EXECUTIONFLOW:3������CSUDIOSGDestroySurface���ٴ����ɹ���surface
//@EXECUTIONFLOW:4������CSUDIOSGCreateSurface����3000*50��surface,�����ɹ�
//@EXECUTIONFLOW:5������CSUDIOSGDestroySurface���ٴ����ɹ���surface
//@EXECUTIONFLOW:6������CSUDIOSGCreateSurface����50*3000��surface,�����ɹ�
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٴ����ɹ���surface
CSUDI_BOOL CSTC_OSG_IT_CreateSurface_0005(void)
{
	CSUDI_Error_Code			nResult = CSUDI_FAILURE;
	CSUDIOSGPixelFormat_E 		ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	CSUDI_HANDLE hSurface = CSUDI_NULL;
	int nValue = -1;
	char acConfigBuf[32];
	int nDisplayWidth, nDisplayHeight;
	int nWidth, nHeight;

	CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
					"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
				);

	memset(acConfigBuf, 0, sizeof(acConfigBuf));
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("OSG","CS_OSGUDI2_SURPORT_HD_SD_DISPLAY",acConfigBuf,sizeof(acConfigBuf)),"��ȡ�����ļ�ʧ��!!!\n");
	
	nValue = CSTKGetIntFromStr(acConfigBuf,10);
	//��ȡ�Դ�Ĵ�С
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == OSG_GetSize(nValue, &nDisplayWidth,&nDisplayHeight)), "step 1 ��ȡ�Դ���Ϣʧ��!!!!!");

	//�����Դ��С�ߴ�surface�ɹ�
	nWidth  = nDisplayWidth + 100;
	nHeight = nDisplayHeight+ 100;	
	
	nResult = CSUDIOSGCreateSurface(ePixelFormat, nWidth, nHeight, &hSurface);
	CSTK_ASSERT_TRUE_FATAL((nResult == CSUDI_SUCCESS && hSurface != CSUDI_NULL), "step 2���������Դ��СΪ��surfaceʧ��!");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface)), "step 3���ٳ����Դ��Сsurfaceʧ��! ");

	//����3000 * 50��С�ߴ�surface�ɹ�
	nWidth  = 3000;
	nHeight = 50;
	nResult = CSUDIOSGCreateSurface(ePixelFormat, nWidth, nHeight, &hSurface);
	CSTK_ASSERT_TRUE_FATAL((nResult == CSUDI_SUCCESS && hSurface != CSUDI_NULL), "step 4������СΪ(3000  *  50 )��С��surfaceʧ��!");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface)), "step 5 ����surface(3000  *  50 )ʧ��! ");

	//����50 * 3000��С�ߴ�surface�ɹ�
	nWidth  = 50;
	nHeight = 3000;
	nResult = CSUDIOSGCreateSurface(ePixelFormat, nWidth, nHeight, &hSurface);
	CSTK_ASSERT_TRUE_FATAL((nResult == CSUDI_SUCCESS && hSurface != CSUDI_NULL), "step 6 ������СΪ(50  *  3000 )��С��surfaceʧ��!");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface)), "step 7 ����surface(50  *  3000 )ʧ��! ");

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_CreateSurface_0006
//@CASEGROUP:CSUDIOSGCreateSurface
//@DESCRIPTION:1.����ƽ̨��������������surface����Դй¶
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@EXPECTATION:  �ɹ���������surface 2000��
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface����surface�����ɹ�
//@EXECUTIONFLOW:2������CSUDIOSGDestroySurface���ٴ�����surface
//@EXECUTIONFLOW:3���ظ�����1��2 һ��2000��,����ÿ�γɹ�
CSUDI_BOOL CSTC_OSG_IT_CreateSurface_0006(void)
{
	CSUDI_Error_Code			nResult = CSUDI_FAILURE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	CSUDI_HANDLE hSurface = CSUDI_NULL;
	int nValue = -1;
	char acConfigBuf[32] = {0};
	int i = 0;
	int nDisplayWidth, nDisplayHeight;

	memset(acConfigBuf, 0, sizeof(acConfigBuf));
	
	CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
					"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
				);

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("OSG","CS_OSGUDI2_SURPORT_HD_SD_DISPLAY",acConfigBuf,sizeof(acConfigBuf)),"��ȡ�����ļ�ʧ��!!!\n");
	
	nValue = CSTKGetIntFromStr(acConfigBuf,10);
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == OSG_GetSize(nValue, &nDisplayWidth,&nDisplayHeight)), "��ȡ�Դ���Ϣʧ��!!!!!");

	CSTCPrint("��������Ҫ�ϳ�ʱ�䣬�����ĵȴ�\n");
	
	//ѭ���������ͷ�2000��
	for(i = 0; i< 2000; i++)
	{
		nResult = CSUDIOSGCreateSurface(ePixelFormat, nDisplayWidth, nDisplayHeight, &hSurface);
		CSTK_ASSERT_TRUE_FATAL((nResult == CSUDI_SUCCESS && hSurface != CSUDI_NULL), "step 1 �����Դ��С��surfaceʧ��!");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface)), "step 2 �����Դ�surfaceʧ��! ");

		if(i%20 == 0)
		{
			CSTCPrint("%% %d\n", i/20);
		}
	}

	CSTK_FATAL_POINT;
	if(i != 2000)
	{
		CSTCPrint("��ǰ��������%d��", i);
	}

	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_DestroySurface_0001
//@CASEGROUP:CSUDIOSGDestroySurface
//@DESCRIPTION:��������һ����ͼƽ�棬���ԷǷ�ֵ
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: CSUDI_NULL
//@EXPECTATION:  ����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDIOSGDestroySurface�������������CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_DestroySurface_0001(void)
{   
	CSTK_ASSERT_TRUE_FATAL((CSUDIOSG_ERROR_BAD_PARAMETER == CSUDIOSGDestroySurface(CSUDI_NULL)),"Step 1 Destroy an invalid surface should be failure !");
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_DestroySurface_0002
//@CASEGROUP:CSUDIOSGDestroySurface
//@DESCRIPTION: ��������һ���Դ�ռ�(���壬����)
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT:  ��CSUDIOSGGetDisplaySurface��ȡ�ľ��
//@EXPECTATION:  �����������Դ�ռ䣬����ʧ�ܣ���������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���
//@EXECUTIONFLOW:2������CSUDIOSGDestroySurface���������Դ�ռ䣬�������CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_DestroySurface_0002(void)
{   
	CSUDI_HANDLE hSurface= CSUDI_NULL;
	
	if(!OSG_IsShareHDDisplaySurface())
	{
		//��ȡ�����Դ�
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(0, &hSurface) && (CSUDI_NULL != hSurface)),"��ȡ�����Դ�ʧ��");
		//����ɾ���Դ�ռ�
		CSTK_ASSERT_TRUE_FATAL((CSUDIOSG_ERROR_BAD_PARAMETER == CSUDIOSGDestroySurface(hSurface)),"����ɾ�������Դ�ռ����");
	}
	//��ƽ̨֧�ֵ�����»�ȡ�����Դ�
	if(OSG_IsSupportSDHDDisplay())
	{
		hSurface= CSUDI_NULL;
		
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(1, &hSurface) && (CSUDI_NULL != hSurface)),"��ȡ�����Դ�ʧ��");
		CSTK_ASSERT_TRUE_FATAL((CSUDIOSG_ERROR_BAD_PARAMETER == CSUDIOSGDestroySurface(hSurface)),"����ɾ�������Դ�ռ����");
	}
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_GetDisplaySurface_0001
//@CASEGROUP:CSUDIOSGGetDisplaySurface
//@DESCRIPTION:����ƽ̨display surface �Ƿ���Ч
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT:�Դ�����Ϊ0,1(���֧��)
//@EXPECTATION:  �ɹ���ȡ��ΪCSUDI_NULL �ľ������fill����ɫ��ʾ����Ļ��
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ���Դ�ռ����Ϣ������ֵΪCSUDI_SUCCESS
//@EXECUTIONFLOW:3���жϻ�ȡ����Surface��ʾģʽ���Ƿ�ΪEM_UDIOSG_PIXEL_FORMAT_ARGB_8888��EM_UDIOSG_PIXEL_FORMAT_ARGB_1555
//@EXECUTIONFLOW:4�����display surface memory Ϊȫ�� ����ʾ����Ļ��
//@EXECUTIONFLOW:4�����display surface memory Ϊȫ�� ����ʾ����Ļ��
//@EXECUTIONFLOW:4�����display surface memory Ϊȫ������ʾ����Ļ��
//@EXECUTIONFLOW:4�����display surface memory Ϊȫ�� ����ʾ����Ļ��
//@EXECUTIONFLOW:4�����display surface memory Ϊȫ�� ����ʾ����Ļ��
CSUDI_BOOL CSTC_OSG_IT_GetDisplaySurface_0001(void)
{  
	CSUDI_Error_Code errCode = CSUDI_FAILURE;
	CSUDI_HANDLE hSurface = CSUDI_NULL;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;

	errCode = CSUDIOSGGetDisplaySurface(OSG_GetTestDisplaySurfaceIndex(), &hSurface);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS== errCode), "��ȡ�Դ�ʧ��!!!!!");

	memset(&sSurfaceInfo, 0, sizeof(sSurfaceInfo));

	errCode = CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == errCode), "��ȡ�Դ���Ϣʧ��!!!!!");

	CSTCPrint("��ע����Ļ��ɫ�仯\n");
	CSTKWaitAnyKey();

	//fill surface memory with red 
	CSTCPrint("����Ļ��������ʾ��ɫ\n");
	errCode = CSUDIOSGFill(hSurface, CSUDI_NULL, OSG_COLOR_RED_32BIT);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == errCode), "���surfaceʧ��!!!!!");
	/*<added by lengwenhua at 2010-12-01, after write dispaly buffer, we should use Sync  to display it>*/   
	errCode = CSUDIOSGSync();
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == errCode), "ͬ����Ϣʧ��!!!!!");
	CSUDIOSThreadSleep(2000);

	//fill surface memory with white
	CSTCPrint("����Ļ��������ʾ��ɫ\n");
	errCode = CSUDIOSGFill(hSurface, CSUDI_NULL, 0xffffffff);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == errCode), "���surfaceʧ��!!!!!");
	errCode = CSUDIOSGSync();
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == errCode), "ͬ����Ϣʧ��!!!!!");
	CSUDIOSThreadSleep(2000);

	//fill surface memory with blue
	CSTCPrint("����Ļ��������ʾ��ɫ\n");
	errCode = CSUDIOSGFill(hSurface, CSUDI_NULL, OSG_COLOR_BLUE_32BIT);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == errCode), "���surfaceʧ��!!!!!");
	errCode = CSUDIOSGSync();
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == errCode), "ͬ����Ϣʧ��!!!!!");
	CSUDIOSThreadSleep(2000);

	//fill surface memory with black
	CSTCPrint("����Ļ��������ʾ��ɫ\n");
	errCode = CSUDIOSGFill(hSurface, CSUDI_NULL, 0xff000000);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == errCode), "���surfaceʧ��!!!!!");
	errCode = CSUDIOSGSync();
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == errCode), "ͬ����Ϣʧ��!!!!!");
	CSUDIOSThreadSleep(2000);

	//fill surface memory with green
	CSTCPrint("����Ļ��������ʾ��ɫ\n");
	errCode = CSUDIOSGFill(hSurface, CSUDI_NULL, OSG_COLOR_GREEN_32BIT);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == errCode), "���surfaceʧ��!!!!!");
	errCode = CSUDIOSGSync();
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == errCode), "ͬ����Ϣʧ��!!!!!");
	CSUDIOSThreadSleep(2000);

	CSTCPrint("��ȷ����Ļ��ɫ������ʾΪ���������\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "��Ļ��ɫ��ʾ����ȷ!!!!!");

	CSTK_FATAL_POINT;

	OSG_iClearScreen();

	return TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_GetDisplaySurfaceInfo_0001
//@CASEGROUP:CSUDIOSGGetDisplaySurface
//@DESCRIPTION:����ƽ̨��֧�ֵ������Դ����͵Ĵ�����������������
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT:�Դ�����Ϊ0,1(���֧��)
//@EXPECTATION:  �ɹ���ȡ��ΪCSUDI_NULL �ľ��
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ���Դ�ռ����Ϣ������ֵΪCSUDI_SUCCESS
//@EXECUTIONFLOW:3��ʹ��OSG_iCheckSD(HD)DisplayInfo ����Դ�ĸ�������
CSUDI_BOOL CSTC_OSG_IT_GetDisplaySurfaceInfo_0001(void)
{   
	CSUDI_HANDLE hSurface = CSUDI_NULL;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;

	if(!OSG_IsShareHDDisplaySurface())
	{
		//���������Դ�
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(0, &hSurface) && (CSUDI_NULL != hSurface)),"���������Դ�ʧ��");
		//��ȡ�Դ���Ϣ
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo)), "��ȡ�����Դ���Ϣʧ��");
		//����Դ���Ϣ
		CSTK_ASSERT_TRUE_FATAL(OSG_iCheckSDDisplayInfo(hSurface), "���������Դ���Ϣ��Ԥ�ڲ���");
	}
	
	//��֧�ָ��������²��Ը����Դ�
	if(OSG_IsSupportSDHDDisplay())
	{
		hSurface = CSUDI_NULL;
		memset(&sSurfaceInfo, 0, sizeof(sSurfaceInfo));
		
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(1, &hSurface) && (CSUDI_NULL != hSurface)),"���������Դ�ʧ��");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo)), "��ȡ�����Դ���Ϣʧ��");
		CSTK_ASSERT_TRUE_FATAL(OSG_iCheckHDDisplayInfo(hSurface), "���������Դ���Ϣ��Ԥ�ڲ���");
	}

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_GetDisplaySurfaceInfo_0002
//@CASEGROUP:CSUDIOSGGetDisplaySurface
//@DESCRIPTION:�����Դ�����Ϊ�Ƿ�ֵʱ��ȡ�Դ���ʧ��
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT:�Դ�����Ϊ5
//@EXPECTATION: ��ȡ�Դ���ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���
//@EXECUTIONFLOW:2����ȡ�Դ���ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_GetDisplaySurfaceInfo_0002(void)
{   
	CSUDI_HANDLE hSurface = CSUDI_NULL;
	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDIOSG_ERROR_BAD_PARAMETER==CSUDIOSGGetDisplaySurface(-1, &hSurface)) && (hSurface == CSUDI_NULL),
				"��nIndex ����������ʧ��");
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDIOSG_ERROR_BAD_PARAMETER==CSUDIOSGGetDisplaySurface(2, &hSurface)) && (hSurface == CSUDI_NULL),
				"��nIndex ����������ʧ��");
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDIOSG_ERROR_BAD_PARAMETER==CSUDIOSGGetDisplaySurface(0, CSUDI_NULL)) && (hSurface == CSUDI_NULL),
				"��phSurface ����������ʧ��");
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGGetSurfaceInfo
//@DESCRIPTION:���Ի�ȡ��ͼƽ����Ϣʱ����Ƿ�����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT:1������hSurface���ΪCSUDI_NULL
//@INPUT:2������Ƿ�sSurfaceInfo
//@EXPECTATION: ����CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDIOSGGetSurfaceInfo���������CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_GetSurfaceInfo_0001(void)
{   
	CSUDIOSGPixelFormat_E		ePixelFormat = -1;
	CSUDIOSGSurfaceInfo_S 		sSurfaceInfo;
	CSUDI_HANDLE 			hSurface = CSUDI_NULL;
	CSUDI_Error_Code			nResult = CSUDI_FAILURE;
	
	//Ѱ��һ��֧�ֵ�ePixelFormat
	CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
					"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
				);
		
	//����һ��surface
	nResult = CSUDIOSGCreateSurface(ePixelFormat,SURFACE_WIDTH_720,SURFACE_HEIGHT_576,&hSurface);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult) && (CSUDI_NULL != hSurface), "����surface ʧ��");

	//�ֱ���Դ��������ִ��GetSurfaceInfo
	CSTK_ASSERT_TRUE_FATAL((CSUDIOSG_ERROR_BAD_PARAMETER == CSUDIOSGGetSurfaceInfo(CSUDI_NULL, &sSurfaceInfo)),
			"��hSurface ����������ʧ��");
	CSTK_ASSERT_TRUE_FATAL((CSUDIOSG_ERROR_BAD_PARAMETER == CSUDIOSGGetSurfaceInfo(hSurface, CSUDI_NULL)),
			"��psSurfaceInfo ����������ʧ��");
	
	CSTK_FATAL_POINT
	{
		if(hSurface != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface)),"����Surface ʧ��");
			hSurface = CSUDI_NULL;
		}
	}
	
	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_GetSurfaceInfo_0002
//@CASEGROUP:CSUDIOSGGetSurfaceInfo
//@DESCRIPTION:���Ի�ȡ�Ѿ����ٵĻ�ͼƽ�����Ϣ
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT:1�������hSurface����Ѿ�������
//@INPUT:2������Ϸ���psSurfaceInfo
//@EXPECTATION: ����CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface������ͼƽ��
//@EXECUTIONFLOW:2������CSUDIOSGDestroySurface���ٻ�ͼƽ�棬�ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIOSGGetSurfaceInfo���������CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_GetSurfaceInfo_0002(void)
{   
	CSUDIOSGPixelFormat_E		ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	CSUDIOSGSurfaceInfo_S 		sSurfaceInfo;
	CSUDI_HANDLE 			hSurface = CSUDI_NULL;	
	CSUDI_Error_Code			nResult = CSUDI_FAILURE;

	//Ѱ��һ��֧�ֵ�ePixelFormat
	CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
					"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
				);
		
	//����һ��surface
	nResult = CSUDIOSGCreateSurface(ePixelFormat,SURFACE_WIDTH_720,SURFACE_HEIGHT_576,&hSurface);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult) && (CSUDI_NULL != hSurface), "����surface ʧ��");

	//���ٴ�����surface
	//ע�⣬��ʱ���ٲ���hSurface ���գ�����ͳ�Ϊ��������ļ����
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface)),"����surface ʧ��");

	//�߼�����ȡһ���Ѿ����ٵ�surface
	CSTK_ASSERT_TRUE_FATAL((CSUDIOSG_ERROR_BAD_PARAMETER == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo)),
							"���Ѿ����ٵ�surafce ִ��CSUDIOSGGetSurfaceInfo ���ʧ��");
	hSurface = CSUDI_NULL;
	CSTK_FATAL_POINT
	{
		if(hSurface != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface)),"����Surface ʧ��");
			hSurface = CSUDI_NULL;
		}
	}
		
	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_GetSurfaceInfo_0003
//@CASEGROUP:CSUDIOSGGetSurfaceInfo
//@DESCRIPTION:���Կ���ͨ��CSUDIOSGGetSurfaceInfo��ȡ���Դ����Ϣ
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@EXPECTATION: 1.�ɹ���ȡ����Ϣ
//@EXPECTATION: 2.��ȡ����surface information��ʵ�����һ��
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�����Դ���,�����ɹ�
//@EXECUTIONFLOW:2�����ƽ̨֧�ָ���,����CSUDIOSGGetDisplaySurface��ȡ�����Դ���,�����ɹ�
//@EXECUTIONFLOW:3������CSUDIOSGGetSurfaceInfo����ȡ�����Դ�info,�����ɹ�
//@EXECUTIONFLOW:4�����m_pvSurfaceMem��Ա��Ϊ�գ�m_nMemSize����0,�ҿ��Խ�m_pvSurfaceMemָ����ڴ�m_nMemSize��С����memset
//@EXECUTIONFLOW:5�����m_nWidth��m_nHeightӦ�ú�ʵ�ʵ��Դ�߿�һ���Ҵ���0
//@EXECUTIONFLOW:6������ȡ������ɫģʽ�Ƿ����Դ���ɫģʽһ��
//@EXECUTIONFLOW:7������ȡ�����Դ�pitchӦ�ô��ڵ����Դ�����ÿ��������ռ���ֽ���
//@EXECUTIONFLOW:8������Ǹ���ƽ̨�����CSUDIOSGGetDisplaySurface��ȡ�����Դ�surface���
//@EXECUTIONFLOW:9������CSUDIOSGGetSurfaceInfo����ȡ�����Դ�info,�����ɹ�
//@EXECUTIONFLOW:10���ظ�����4~7
CSUDI_BOOL CSTC_OSG_IT_GetSurfaceInfo_0003(void)
{
	CSUDIOSGPixelFormat_E ePixelFormat;
	CSUDI_HANDLE hSurface = CSUDI_NULL;
	int nValue = -1;
	char acConfigBuf[32] = {0};
	int i;
	int nExpectWidth, nExpectHeight;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;

	memset(acConfigBuf, 0, sizeof(acConfigBuf));
			
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("OSG","CS_OSGUDI2_SURPORT_HD_SD_DISPLAY",acConfigBuf,sizeof(acConfigBuf)),"��ȡ�����ļ�ʧ��!!!\n");
	
	nValue = CSTKGetIntFromStr(acConfigBuf,10);

	for(i = 0; i<= nValue; i++)
	{
		if(i == 0)
		{
			if(OSG_IsShareHDDisplaySurface())
			{
				continue;
			}
			
			CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("OSG","CS_UDI2OSG_SD_DISPLAY_WIDTH",acConfigBuf,sizeof(acConfigBuf)),"��ȡ�����ļ�ʧ��!!!\n");
			nExpectWidth = CSTKGetIntFromStr(acConfigBuf,10);
			CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("OSG","CS_UDI2OSG_SD_DISPLAY_HEIGHT",acConfigBuf,sizeof(acConfigBuf)),"��ȡ�����ļ�ʧ��!!!\n");
			nExpectHeight = CSTKGetIntFromStr(acConfigBuf,10);
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("OSG","CS_UDI2OSG_HD_DISPLAY_WIDTH",acConfigBuf,sizeof(acConfigBuf)),"��ȡ�����ļ�ʧ��!!!\n");
			nExpectWidth = CSTKGetIntFromStr(acConfigBuf,10);
			CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("OSG","CS_UDI2OSG_HD_DISPLAY_HEIGHT",acConfigBuf,sizeof(acConfigBuf)),"��ȡ�����ļ�ʧ��!!!\n");
			nExpectHeight = CSTKGetIntFromStr(acConfigBuf,10);
		}
		
		CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("OSG","CS_UDI2OSG_DISPLAY_PIXEL_FORMAT",acConfigBuf,sizeof(acConfigBuf)),"��ȡ�����ļ�ʧ��!!!\n");
		ePixelFormat = CSTKGetIntFromStr(acConfigBuf,10);

		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(i, &hSurface)),"step 2 ��ȡ�Դ�ʧ��!!!!!\n");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo)), "step 3 ��ȡ�Դ���Ϣʧ��!!!!!");

		//���surface����Ϣ
		CSTK_ASSERT_TRUE_FATAL((sSurfaceInfo.m_nWidth == nExpectWidth), "step 5 ʧ�ܣ�surface info width is error!");
		CSTK_ASSERT_TRUE_FATAL((sSurfaceInfo.m_nHeight == nExpectHeight), "step 5 ʧ�ܣ�surface info height is error!");
		CSTK_ASSERT_TRUE_FATAL((sSurfaceInfo.m_pvSurfaceMem != NULL), "step 4 ʧ�ܣ�surface �ڴ治Ϊ��!");
		CSTK_ASSERT_TRUE_FATAL((sSurfaceInfo.m_nMemSize > 0), "step 4 ʧ�ܣ�surface ��memsize is error!");

		if (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888)
		{
			CSTK_ASSERT_TRUE_FATAL((sSurfaceInfo.m_nPitch >= sSurfaceInfo.m_nWidth * 4), "step 7 ʧ�ܣ�surface��pitch����ȷ!");
		}
		else if(ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)
		{
			CSTK_ASSERT_TRUE_FATAL((sSurfaceInfo.m_nPitch >= sSurfaceInfo.m_nWidth * 2), "step 7 ʧ�ܣ�surface��pitch����ȷ!");
		}
		
		CSTK_ASSERT_TRUE_FATAL((sSurfaceInfo.m_ePixelFormat == ePixelFormat), "step 6 ʧ�ܣ�surface����ɫģʽ����ȷ!");

		memset(sSurfaceInfo.m_pvSurfaceMem, 0, sSurfaceInfo.m_nMemSize);
	}

	CSTK_FATAL_POINT;

	return CSUDI_FAILURE;	
}

//@TESTCASEID:CSTC_OSG_IT_GetSurfaceInfo_0004
//@CASEGROUP:CSUDIOSGGetSurfaceInfo
//@DESCRIPTION:���Կ���ͨ��CSUDIOSGGetSurfaceInfo��ȡ���û�������surface����Ϣ
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@EXPECTATION: 1.�ɹ���ȡ����Ϣ
//@EXPECTATION: 2.��ȡ����surface information��ʵ�����һ��
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������surface
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo����ȡ����surface��info,�����ɹ�
//@EXECUTIONFLOW:4�����m_pvSurfaceMem��Ա��Ϊ�գ�m_nMemSize����0,�ҿ��Խ�m_pvSurfaceMemָ����ڴ�m_nMemSize��С����memset
//@EXECUTIONFLOW:5�����m_nWidth��m_nHeightӦ�úʹ���ʱ����ĸ߿�һ��
//@EXECUTIONFLOW:6������ȡ������ɫģʽ�Ƿ��봴��ָ������ɫģʽһ��
//@EXECUTIONFLOW:7������ȡ�����Դ�pitchӦ�ô��ڵ���surface�����ÿ��������ռ���ֽ���
//@EXECUTIONFLOW:8������CSUDIOSGDestroySurface���ٴ�����surface
CSUDI_BOOL CSTC_OSG_IT_GetSurfaceInfo_0004(void)
{
	CSUDI_Error_Code			nResult = CSUDI_FAILURE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	CSUDI_HANDLE hSurface = CSUDI_NULL;
	int nWidth = 720;
	int nHeight= 576;
		
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;

	int nBytes = 0;
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == OSG_iFindSupportPixelFormatAndBytes(&ePixelFormat, &nBytes)), "û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat");

	nResult = CSUDIOSGCreateSurface(ePixelFormat, nWidth, nHeight, &hSurface);
	CSTK_ASSERT_TRUE_FATAL((nResult == CSUDI_SUCCESS && hSurface != CSUDI_NULL), "step 1 �����Դ��С��surfaceʧ��!");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS== CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo)), "step 2��ȡsurface��Ϣʧ��!!!!!");

	//���surface����Ϣ
	CSTK_ASSERT_TRUE_FATAL((sSurfaceInfo.m_nWidth == nWidth), "step 5 ʧ�ܣ�surface info width is error!");
	CSTK_ASSERT_TRUE_FATAL((sSurfaceInfo.m_nHeight == nHeight), "step 5 ʧ�ܣ�surface info height is error!");
	CSTK_ASSERT_TRUE_FATAL((sSurfaceInfo.m_pvSurfaceMem != NULL), "step 4 ʧ�ܣ�surface �ڴ治Ϊ��!");
	CSTK_ASSERT_TRUE_FATAL((sSurfaceInfo.m_nMemSize > 0), "step 4 ʧ�ܣ�surface ��memsize is error!");
	CSTK_ASSERT_TRUE_FATAL((sSurfaceInfo.m_nPitch >= sSurfaceInfo.m_nWidth * nBytes), "step 7 ʧ�ܣ�surface��pitch����ȷ!");
	CSTK_ASSERT_TRUE_FATAL((sSurfaceInfo.m_ePixelFormat == ePixelFormat), "step 6 ʧ�ܣ�surface����ɫģʽ����ȷ!");

	CSTK_FATAL_POINT;	
	if(hSurface != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface)),"step 8 �����Դ�surfaceʧ��! ");
		hSurface = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

typedef struct _Error_SurfaceInfo_S
{
	int m_nAlphaFlag; 
	unsigned int m_uAlpha; 
	int m_nColorKeyFlag; 
}Error_SurfaceInfo_S;

//@TESTCASEID:CSTC_OSG_IT_SetSurfaceInfo_0001
//@CASEGROUP:CSUDIOSGSetSurfaceInfo
//@DESCRIPTION:���Ի�ͼƽ����Ϣ�Ƿ�ֵ�����
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT:1����CSUDIOSGCreateSurface�����ľ��hSurface
//@INPUT:2����ͼƽ����Ϣ�ṹ��sSurfaceInfo��ģʽΪARGB8888��ARGB1555��ƽ����ΪnWidth = 720, g_nHeight = 576
//@INPUT:3���������
//@EXPECTATION: ÿһ������Ԥ�ڵ�ֵ
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface������ͼƽ�棬Ҫ�󷵻�CSUDI_SUCCESS�Ҿ����Ϊ��
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ��ͼƽ����Ϣ��Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIOSGSetSurfaceInfo�ӿ����ô���Ļ�ͼƽ����Ϣ����������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4������CSUDIOSGDestroySurface�������ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_SetSurfaceInfo_0001(void)
{
	CSUDIOSGPixelFormat_E		ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	CSUDI_HANDLE 				hSurface = CSUDI_NULL;
	CSUDIOSGSurfaceInfo_S	 	sSurfaceInfo;
	CSUDI_Error_Code			nResult = CSUDI_FAILURE;
	int							i = 0;
	Error_SurfaceInfo_S			aErrorPram[] = {
											{-1, 0, 0},
											{0, 0, -1},
											{2, 0, 0},
											{0, 256, 0},
											{0, 0, 2}
										};

	//Ѱ��һ��֧�ֵ�ePixelFormat
	CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
					"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
				);
		
	//����һ��surface
	nResult = CSUDIOSGCreateSurface(ePixelFormat,SURFACE_WIDTH_720,SURFACE_HEIGHT_576,&hSurface);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult) && (CSUDI_NULL != hSurface), "����surface ʧ��");

	//�������surface ����Ϣ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo),"���surface ��Ϣʧ��");
		
	//���Դ���ֵ
	for(i = 0; i<sizeof(aErrorPram)/sizeof(aErrorPram[0]); i++)
	{
		sSurfaceInfo.m_nAlphaFlag = aErrorPram[i].m_nAlphaFlag;
		sSurfaceInfo.m_uAlpha = aErrorPram[i].m_uAlpha;
		sSurfaceInfo.m_nColorKeyFlag = aErrorPram[i].m_nColorKeyFlag;
		
		nResult = CSUDIOSGSetSurfaceInfo(hSurface, &sSurfaceInfo);
		CSTK_ASSERT_TRUE_FATAL((CSUDIOSG_ERROR_BAD_PARAMETER == nResult), "���������ʧ��");
	}
	
	CSTK_FATAL_POINT
	{
		if(hSurface != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface)),"����surface ʧ��");
			hSurface = CSUDI_NULL;
		}
	}
	
	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_SetSurfaceInfo_0002
//@CASEGROUP:CSUDIOSGSetSurfaceInfo
//@DESCRIPTION:���ԶԲ������õ��Դ��������
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT:1����GetDisplaySurfaceInfo��ȡ�����Դ�surface���hSurface
//@INPUT:2����ͼƽ����Ϣ�ṹ��sSurfaceInfo
//@EXPECTATION: ÿһ������Ԥ�ڵ�ֵ
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�surface����������CSUDI_SUCCESS�Ҿ����Ϊ��
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3���޸�SurfaceInfo������CSUDIOSGSetSurfaceInfo����������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4������CSUDIOSGSetSurfaceInfo�ӿ����û�ͼƽ����Ϣ����������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��������CSUDI_SUCCESS
//@EXECUTIONFLOW:6���Ƚϲ���2�Ͳ���5�ֱ��ȡ����SurfaceInfo��Ҫ����ͬ
CSUDI_BOOL CSTC_OSG_IT_SetSurfaceInfo_0002(void)
{
	CSUDI_HANDLE				hSurface = CSUDI_NULL;
	CSUDIOSGSurfaceInfo_S		sSurfaceInfo;
	CSUDI_Error_Code			nResult = CSUDI_FAILURE;
	CSUDIOSGSurfaceInfo_S		sSurfaceInfoBake;

	//����һ��surface
	nResult = CSUDIOSGGetDisplaySurface(OSG_GetTestDisplaySurfaceIndex(), &hSurface);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult) && (CSUDI_NULL != hSurface), "����surface ʧ��");

	//�������surface ����Ϣ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo),"���surface ��Ϣʧ��");

	memcpy(&sSurfaceInfoBake, &sSurfaceInfo, sizeof(CSUDIOSGSurfaceInfo_S));
	//���������Դ�ռ�
	sSurfaceInfo.m_uAlpha = 0x11;	
	sSurfaceInfo.m_nAlphaFlag= 1;
	nResult = CSUDIOSGSetSurfaceInfo(hSurface, &sSurfaceInfo);
	CSTK_ASSERT_TRUE_FATAL((CSUDIOSG_ERROR_BAD_PARAMETER == nResult), "���������Դ�ռ�");

	//��������CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED ����CSUDIOSG_ERROR_BAD_PARAMETER
	//�ٴλ���Դ��surface ����Ϣ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo),"���surface ��Ϣʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(0 == memcmp(&sSurfaceInfoBake, &sSurfaceInfo, sizeof(CSUDIOSGSurfaceInfo_S)), "����2�Ͳ���5�ֱ��ȡ����SurfaceInfo��һ��");
	
	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_SetSurfaceInfo_0003
//@CASEGROUP:CSUDIOSGSetSurfaceInfo
//@DESCRIPTION:���Զ�surface����colorkey��alpha����
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@EXPECTATION: 1.�´�����surface��colorkey��alphaӦ��Ĭ��δ����
//@EXPECTATION: 2.����CSUDIOSGSetSurfaceInfo�ܹ��ɹ���colorkey��alpha��������
//@EXPECTATION: 3.��surface����colorkey��alpha�����ò�Ӱ��surface����������
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface������ͼƽ��
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ�´���surface����Ϣ������ΪstInfoA
//@EXECUTIONFLOW:3������ȡ������ϢsSurfaceInfoA������sSurfaceInfoA.m_nColorKeyFlag=0,sSurfaceInfoA.m_nAlphaFlag=0,sSurfaceInfoA.m_uColorkey=0,sSurfaceInfoA.m_uAlpha=0xFF
//@EXECUTIONFLOW:4������CSUDIOSGSetSurfaceInfo�ӿ����û�ͼƽ����Ϣ��m_nColorKeyFlag = 1,m_nAlphaFlag=1,m_uColorkey=0xFFFF,m_uAlpha=0x55,�������سɹ�
//@EXECUTIONFLOW:5���ٴε���CSUDIOSGGetSurfaceInfo��ȡsurface����Ϣ����ΪsSurfaceInfoB,�������سɹ�
//@EXECUTIONFLOW:6�����sSurfaceInfoB������sSurfaceInfoB.m_nColorKeyFlag = 1,sSurfaceInfoB.m_nAlphaFlag=1,sSurfaceInfoB.m_uColorkey=0xFFFF,sSurfaceInfoB.m_uAlpha=0x55
//@EXECUTIONFLOW:7����������������õĳ�Ա�Ƿ����仯,����sSurfaceInfoA.m_ePixelFormat = sSurfaceInfoB.m_ePixelFormat;sSurfaceInfoA.m_nWidth = sSurfaceInfoB.m_nWidth;
//@EXECUTIONFLOW:7��sSurfaceInfoA.m_nHeight = stInfoB.m_nHeight;sSurfaceInfoA.m_nPitch = sSurfaceInfoB.m_nPitch;sSurfaceInfoA.m_pvSurfaceMem = sSurfaceInfoB.m_pvSurfaceMem;sSurfaceInfoA.m_nMemSize = sSurfaceInfoB.m_nMemSize
//@EXECUTIONFLOW:8������CSUDIOSGDestroySurface����surface
CSUDI_BOOL CSTC_OSG_IT_SetSurfaceInfo_0003(void)
{
	CSUDIOSGPixelFormat_E		ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	CSUDI_HANDLE				hSurface = CSUDI_NULL;
	CSUDIOSGSurfaceInfo_S		sSurfaceInfoA;
	CSUDIOSGSurfaceInfo_S		sSurfaceInfoB;	
	CSUDI_Error_Code			nResult = CSUDI_FAILURE;

	memset(&sSurfaceInfoA,0, sizeof(CSUDIOSGSurfaceInfo_S));
	memset(&sSurfaceInfoB,0, sizeof(CSUDIOSGSurfaceInfo_S));	

	//Ѱ��һ��֧�ֵ�ePixelFormat
	CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
					"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
				);
		
	//����һ��surface
	nResult = CSUDIOSGCreateSurface(ePixelFormat, 720, 576, &hSurface);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult) && (CSUDI_NULL != hSurface), "����surface ʧ��");

	//�������surface ����Ϣ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfoA),"���surface ��Ϣʧ��");

	//����sSurfaceInfoA ����Ϣ��sSurfaceInfoB
	memcpy(&sSurfaceInfoB, &sSurfaceInfoA, sizeof(CSUDIOSGSurfaceInfo_S));	
	
	//���surface����Ϣ
	CSTK_ASSERT_TRUE_FATAL((sSurfaceInfoB.m_nColorKeyFlag == 0), "step 3 ʧ�ܣ�colorkey flag error!");
	CSTK_ASSERT_TRUE_FATAL((sSurfaceInfoB.m_nAlphaFlag == 0), "step 3 ʧ�ܣ�alpha flag error!");
	CSTK_ASSERT_TRUE_FATAL((sSurfaceInfoB.m_uColorkey == 0), "step 3 ʧ�ܣ�colorkey  error!");
	CSTK_ASSERT_TRUE_FATAL((sSurfaceInfoB.m_uAlpha == 0xff), "step 3 ʧ�ܣ�alpha  error!");
	
	//����surface����Ϣ
	sSurfaceInfoB.m_nColorKeyFlag = 1 ;
	sSurfaceInfoB.m_nAlphaFlag = 1 ;
	sSurfaceInfoB.m_uColorkey = 0xFFFF ;
	sSurfaceInfoB.m_uAlpha = 0x55 ;
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS== CSUDIOSGSetSurfaceInfo(hSurface, &sSurfaceInfoB)), "step 4 failed !��������surface�Ŀռ�");

	memset(&sSurfaceInfoB, 0, sizeof(sSurfaceInfoB));

	//��ȡ����surface ����Ϣ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfoB),"step 5 failed !���surface ��Ϣʧ��");
	CSTK_ASSERT_TRUE_FATAL((sSurfaceInfoB.m_nColorKeyFlag == 1), "step 4 colorkey flag error!");
	CSTK_ASSERT_TRUE_FATAL((sSurfaceInfoB.m_nAlphaFlag == 1), "step 4 alpha flag error!");
	CSTK_ASSERT_TRUE_FATAL((sSurfaceInfoB.m_uColorkey == 0xFFFF), "step 4 colorkey  error!");
	CSTK_ASSERT_TRUE_FATAL((sSurfaceInfoB.m_uAlpha == 0x55), "step 4 alpha  error!");

	sSurfaceInfoB.m_nColorKeyFlag = 0 ;
	sSurfaceInfoB.m_nAlphaFlag = 0 ;
	sSurfaceInfoB.m_uColorkey = 0 ;
	sSurfaceInfoB.m_uAlpha = 0xff ;

	//����ǰ����Ϣ��Ƚ�֮ǰ�����surfaceinfoB����Ϣ�Ƚ�
	CSTK_ASSERT_TRUE_FATAL((0== memcmp(&sSurfaceInfoA, &sSurfaceInfoB, sizeof(CSUDIOSGSurfaceInfo_S))), "step 7 У��surface��Ϣʧ��!");

	CSTK_FATAL_POINT;
	
	if(hSurface != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface)),"Step 8 destroy hSurface failure");
		hSurface = CSUDI_NULL;
	}	
	
	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_CreateDestorySurface_0001
//@CASEGROUP:CSUDIOSGCreateSurface
//@DESCRIPTION:���Դ�����ͼƽ��ʱ֧�ִ�������ɫģʽ(��������CSUDIOSGPixelFormat_Eö���ж����ֵ)
//@DESCRIPTION:����ͬʱ��ÿ���������������������ɾ��
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT:��ɫģʽΪCSUDIOSGPixelFormat_Eö���ж����ֵ������ƽ̨֧�ָ���ɫģʽ
//@EXPECTATION: �ɹ�������ͼƽ��
//@REMARK:	�˲�������ʧ�ܣ���֮��Ĳ����������Խ��������
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface������ͼƽ��,ƽ����Ϊ720,ƽ��߶�Ϊ576
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ��ͼƽ�����Ϣ
//@EXECUTIONFLOW:3��������ȡ�Ļ�ͼƽ�����Ϣ������ʱ�������Ϣһ��
//@EXECUTIONFLOW:4������CSUDIOSGDestroySurface�������ٻ�ͼƽ��
//@EXECUTIONFLOW:5������CSUDIOSGCreateSurface������ͼƽ��,ƽ����Ϊ1280,ƽ��߶�Ϊ720
//@EXECUTIONFLOW:6������CSUDIOSGGetSurfaceInfo��ȡ��ͼƽ�����Ϣ
//@EXECUTIONFLOW:7��������ȡ�Ļ�ͼƽ�����Ϣ������ʱ�������Ϣһ��
//@EXECUTIONFLOW:8������CSUDIOSGDestroySurface�������ٻ�ͼƽ��
//@EXECUTIONFLOW:9������CSUDIOSGCreateSurface������ͼƽ��,ƽ����Ϊ1920,ƽ��߶�Ϊ1080
//@EXECUTIONFLOW:10������CSUDIOSGGetSurfaceInfo��ȡ��ͼƽ�����Ϣ
//@EXECUTIONFLOW:11��������ȡ�Ļ�ͼƽ�����Ϣ������ʱ�������Ϣһ��
//@EXECUTIONFLOW:12������CSUDIOSGDestroySurface�������ٻ�ͼƽ��
//@EXECUTIONFLOW:ѭ��CSUDIOSGPixelFormat_Eö���ж����ֵ��ִ�в���1������12,����ÿһ�����ɹ�
CSUDI_BOOL CSTC_OSG_IT_CreateDestorySetGetSurface_0001(void)
{  
	CSUDI_BOOL bRet = CSUDI_TRUE;
	int nIndex = EM_UDIOSG_PIXEL_FORMAT_RGB_565;
	
	for(; nIndex < EM_UDIOSG_PIXEL_FORMAT_NUM; nIndex++)
	{
		bRet &= OSG_CreatSurface_TestPixelFormatAndSize(nIndex,EM_SURFACE_720_576);
		CSTK_ASSERT_TRUE_FATAL(bRet,"CreateDestorySetGetSurface EM_SURFACE_720_576 ʧ��!!!");
		
		if(OSG_IsSupport1280_720())
		{
			bRet &= OSG_CreatSurface_TestPixelFormatAndSize(nIndex,EM_SURFACE_1280_720);
			CSTK_ASSERT_TRUE_FATAL(bRet,"CreateDestorySetGetSurface EM_SURFACE_1280_720 ʧ��!!!");
		}
		
		if(OSG_IsSupport1920_1080())
		{
			bRet &= OSG_CreatSurface_TestPixelFormatAndSize(nIndex,EM_SURFACE_1920_1080);
            		CSTK_ASSERT_TRUE_FATAL(bRet,"CreateDestorySetGetSurface EM_SURFACE_1920_1080 ʧ��!!!");
		}
	}
    
	CSTK_FATAL_POINT

	return bRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_0000
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:<font color=red>ע�������ֻ������˵��Blit������Ҫ�ĸ�����������û��ʵ������</font>
//@PRECONDITION: �����ǲ���blit����ʱ���ֲ�����˵��
//@INPUT:  **********************************************************************************		      			      		     		    
//@INPUT:  ��ʶ��:  P1 
//@INPUT:  ��  ��:  Դ����ռ��С       
//@INPUT:  ȡֵ��Χ:(0���ޣ�
//@INPUT:
//@INPUT:  ����ȡֵ:[720,576]	
//@INPUT:  �����:  P1_1
//@INPUT:  ===================
//@INPUT:  ����ȡֵ:[1280,720]	
//@INPUT:  �����:  P1_2
//@INPUT:  ------------------------------------------------------------------------------
//@INPUT:  ��ʶ��:  P2 
//@INPUT:  ��  ��:  Դ����ռ��С       
//@INPUT:  ȡֵ��Χ:(0���ޣ�
//@INPUT:
//@INPUT:  ����ȡֵ:[720,576]	
//@INPUT:  �����:  P2_1
//@INPUT:  ===================
//@INPUT:  ����ȡֵ:[1280,720]	
//@INPUT:  �����:  P2_2
//@INPUT:  ------------------------------------------------------------------------------
//@INPUT:  ��ʶ��:  D
//@INPUT:  ��  ��:  �Դ�ռ��С       
//@INPUT:  ȡֵ��Χ:(0���ޣ�
//@INPUT:
//@INPUT:  ����ȡֵ:ͨ��CSUDIOSGGetSurfaceInfo��ȡ	
//@INPUT:  �����:  D
//@INPUT:  ------------------------------------------------------------------------------
//@INPUT:  ��ʶ��:  C 
//@INPUT:  ��  ��:  Colorkey       
//@INPUT:  ȡֵ��Χ:[0x00000000,0xffffffff]
//@INPUT:
//@INPUT:  ����ȡֵ:0xff000000 	
//@INPUT:  �����:  C1
//@INPUT:  ===================
//@INPUT:  ����ȡֵ:0xffffffff
//@INPUT:  �����:  C2
//@INPUT:  ------------------------------------------------------------------------------
//@INPUT:  ��ʶ��:  A 
//@INPUT:  ��  ��:  Alpha       
//@INPUT:  ȡֵ��Χ:[0x00,0xff]
//@INPUT:
//@INPUT:  ����ȡֵ:0x40 	
//@INPUT:  �����:  A1
//@INPUT:  ===================
//@INPUT:  ����ȡֵ:0xff
//@INPUT:  �����:  A2	
//@INPUT:  ------------------------------------------------------------------------------
//@INPUT:  ��ʶ��:  Cr 
//@INPUT:  ��  ��:  ��ɫֵ       
//@INPUT:  ȡֵ��Χ:[0x00000000,0xffffffff]
//@INPUT:
//@INPUT:  ����ȡֵ:0xffff0000 	
//@INPUT:  �����:  Cr1
//@INPUT:  ===================
//@INPUT:  ����ȡֵ:0xff00ff00
//@INPUT:  �����:  Cr2	
//@INPUT:  ===================
//@INPUT:  ����ȡֵ:0x40ff0000
//@INPUT:  �����:  Cr3
//@INPUT:  ===================
//@INPUT:  ����ȡֵ:0xff0000ff
//@INPUT:  �����:  Cr4
//@INPUT:  ===================
//@INPUT:  ����ȡֵ:��ɫֵΪcs_osgcolordata.c�ļ��е�ֵ
//@INPUT:  �����:  Cr5
//@INPUT:  ------------------------------------------------------------------------------
//@INPUT:  ��ʶ��:  R 
//@INPUT:  ��  ��:  ��������(����Ŀ���Դ��       
//@INPUT:  ȡֵ��Χ:�Ϸ����Ƿ����߽�ֵ(x,y)�ʣ�0��+��)|��w,h)�ʣ�0��+�ޣ�{x,y,w,h}
//@INPUT:  ȡֵ��Χ:���Դ�ռ佻��ʱ��ͨ��CSUDIOSGSurfaceInfo_S��ȡ��ͼ���Դ棩�ռ�Ĵ�С��w,h)
//@INPUT:
//@INPUT:  ����ȡֵ:CSUDI_NULL 	
//@INPUT:  �����:  R1
//@INPUT:  ===================
//@INPUT:  ����ȡֵ:{0��0��w��h}��ȫ����
//@INPUT:  �����:  R2	
//@INPUT:  ===================
//@INPUT:  ����ȡֵ:{w/2-100��h/2-100��w/2+100��h/2+100}������λ��)
//@INPUT:  �����:  R3
//@INPUT:  ===================
//@INPUT:  ����ȡֵ:{0��0��0��0}���Ƿ�ֵ��
//@INPUT:  �����:  R4
//@INPUT:  ===================
//@INPUT:  ����ȡֵ:{0��0��-1��-1}���Ƿ�ֵ��
//@INPUT:  �����:  R5
//@INPUT:  ===================
//@INPUT:  ����ȡֵ:{0��0��1��1}��һ������ֵ��
//@INPUT:  �����:  R6
//@INPUT:  ===================
//@INPUT:  ����ȡֵ:{0��0��15��15}����r6��ϲ�������15������ͬ��ͼ�ռ�������
//@INPUT:  �����:  R7
//@INPUT:  ===================
//@INPUT:  ����ȡֵ:{0,h-100,100,h+100}�����½�Խ�磩
//@INPUT:  �����:  R8
//@INPUT:  ===================
//@INPUT:  ����ȡֵ:{w/2,h-100,w/2+100,h+100}�����б�Խ�磩
//@INPUT:  �����:  R9
//@INPUT:  ===================
//@INPUT:  ����ȡֵ:{w-100��h-100��w+100��h+100}�����½�Խ�磩
//@INPUT:  �����:  R10
//@INPUT:  ===================
//@INPUT:  ����ȡֵ:{w-100,h/2-100,w+100,h/2+100}�����б�Խ�磩
//@INPUT:  �����:  R11
//@INPUT:  ===================
//@INPUT:  ����ȡֵ:{w-100,0,w+100,100}�����Ͻ�Խ�磩
//@INPUT:  �����:  R12
//@INPUT:  ===================
//@INPUT:  ����ȡֵ:{1��1��16��16}����r6��ϲ�������15���������ص��������
//@INPUT:  �����:  R13
//@INPUT:  ===================
//@INPUT:  ����ȡֵ:{-11,-11,100,100}�����Ͻ�Խ�磩
//@INPUT:  �����:  R14
//@INPUT:  ===================
//@INPUT:  ����ȡֵ:{-11,h/2-100,100,h+100}�����б�Խ�磩
//@INPUT:  �����:  R15
//@INPUT:  ===================
//@INPUT:  ����ȡֵ:{w/2,-11,w/2+100,h+100}�����б�Խ�磩
//@INPUT:  �����:  R16
//@INPUT:  -------------------------------------------------------------------------------
//@INPUT:  ��ʶ��:  M
//@INPUT:  ��  ��:  ���ģʽ ��       
//@INPUT:  ȡֵ��Χ:17��CSUDIOSGOpMode_Eģʽ
//@INPUT:
//@INPUT:  ����ȡֵ:
//@INPUT:  EM_UDIOSG_OPMODE_COPY         == M1
//@INPUT:  EM_UDIOSG_OPMODE_XOR          == M2
//@INPUT:  EM_UDIOSG_OPMODE_OR           == M3
//@INPUT:  EM_UDIOSG_OPMODE_AND          == M4
//@INPUT:  EM_UDIOSG_OPMODE_CLEAR        == M5
//@INPUT:  EM_UDIOSG_OPMODE_SET          == M6
//@INPUT:  EM_UDIOSG_OPMODE_EQUIV        == M7
//@INPUT:  EM_UDIOSG_OPMODE_NOR          == M8
//@INPUT:  EM_UDIOSG_OPMODE_NAND         == M9
//@INPUT:  EM_UDIOSG_OPMODE_INVERT       == M10
//@INPUT:  EM_UDIOSG_OPMODE_COPYINVERTED == M11
//@INPUT:  EM_UDIOSG_OPMODE_ORINVERTED   == M12
//@INPUT:  EM_UDIOSG_OPMODE_ANDINVERTED  == M13
//@INPUT:  EM_UDIOSG_OPMODE_ORREVERSE    == M14
//@INPUT:  EM_UDIOSG_OPMODE_ANDREVERSE   == M15
//@INPUT:  EM_UDIOSG_OPMODE_NOOP         == M16
//@INPUT:  EM_UDIOSG_OPMODE_ALPHA        == M17
//@INPUT: --------------------------------------------------------------------------------
//@INPUT:  ��ʶ��:  X
//@INPUT:  ��  ��:  ��ɫģʽ       
//@INPUT:  ȡֵ��Χ:12��CSUDIOSGPixelFormat_E
//@INPUT:
//@INPUT:  ����ȡֵ:
//@INPUT:  EM_UDIOSG_PIXEL_FORMAT_ARGB_1555    == X1
//@INPUT:  EM_UDIOSG_PIXEL_FORMAT_ARGB_8888    == X2
//@INPUT:  EM_UDIOSG_PIXEL_FORMAT_RGB_565      == X3
//@INPUT:  EM_UDIOSG_PIXEL_FORMAT_RGBA_5551    == X4
//@INPUT:  EM_UDIOSG_PIXEL_FORMAT_ARGB_4444    == X5
//@INPUT:  EM_UDIOSG_PIXEL_FORMAT_RGBA_4444    == X6
//@INPUT:  EM_UDIOSG_PIXEL_FORMAT_RGB_888      == X7
//@INPUT:  EM_UDIOSG_PIXEL_FORMAT_YCBCR888_444 == X8
//@INPUT:  EM_UDIOSG_PIXEL_FORMAT_YCBCR888_422 == X9
//@INPUT:  EM_UDIOSG_PIXEL_FORMAT_YCBCR888_420 == X10
//@INPUT:  EM_UDIOSG_PIXEL_FORMAT_YCBCR888_411 == X11
//@INPUT:  EM_UDIOSG_PIXEL_FORMAT_PALETTE8     == X12
//@INPUT:  ���Ը��ݸ�����ȡ������ɫģʽֵ
//@INPUT:
//@INPUT:*******************************************************************
//@INPUT:*************����������͸���Ȼ��Ч���ļ��㹫ʽ********************
//@INPUT:*******************************************************************
//@INPUT: sColorA: Դƽ����ɫalphaֵ
//@INPUT: dColorA: Ŀ��ƽ����ɫalphaֵ
//@INPUT: sGA: Դƽ���ȫ��alphaֵ
//@INPUT: dGA: Ŀ��ƽ���ȫ��alphaֵ
//@INPUT: dA: ���ջ�Ͻ����alphaֵ
//@INPUT: dR: ���ջ�Ͻ����R����ֵ 
//@INPUT: dG: ���ջ�Ͻ����G����ֵ
//@INPUT: dB: ���ջ�Ͻ����B����ֵ
//@INPUT: sMixA = sColorA * sGA/0xff
//@INPUT: dMixA = dColorA * dGA/0xff
//@INPUT: Forum1: dA = sMixA + dMixA - ((sMixA * dMixA)/0xff)
//@INPUT: Forum2: dA = dGA
//@INPUT: Forum3: dA = sMixA + dColorA - ((sMixA * dColorA)/0xff)
//@INPUT: Forum4: dA = dColorA
//@INPUT: Forum5: dA = sColorA
//@INPUT: Forum6: dA = sColorA + dColorA * Global_Alpha (1- sColorA)
//@INPUT:��1��dA = �����Ϲ�ʽ(ForumX)�õ�
//@INPUT:��2��dR = dR + (sR - dR) * sMixA /dA
//@INPUT:��3��dG = dG + (sG - dG) * sMixA /dA
//@INPUT:��4��dB = dB + (sB - dB) * sMixA /dA
//@INPUT:
//@EXPECTATION: �ṩ���²��������õ��ĸ��������
//@REMARK:  <b>�������ϸ�������������ϳ�ִ��Blit�����ĸ��ֲ���
//@REMARK:  �����������Ϊ <font color= blue>P1_1+P2_1+X1+SR1+DR1+M1</font>
//@REMARK:  ��ʾ�������ݣ�
//@REMARK:	1��Դ��ͼƽ���СP1_1=[720,576]
//@REMARK:	2��Ŀ�Ļ�ͼƽ���СP2_1=[720,576]
//@REMARK:	3����ǰ��ɫģʽX1=EM_UDIOSG_PIXEL_FORMAT_ARGB_1555
//@REMARK:	4��Դ��������SR1=CSUDI_NULL
//@REMARK:  5��Ŀ�ľ�������DR1=CSUDI_NULL
//@REMARK:  6�����ģʽM1=EM_UDIOSG_OPMODE_COPY</b>
//@EXECUTIONFLOW: 
CSUDI_BOOL CSTC_OSG_IT_Blit_0000(void)
{   
	return CSUDI_TRUE;
}

//**********************************************************************************//
//******************************Begin Test  Rect Blit******************************//
//**********************************************************************************//
//************************Ŀ���ͼ�ռ�Ϊ����ռ�************************************//

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0001
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��Ŀ�ľ�������ΪCSUDI_NULLʱ��blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX1��X2
//@INPUT: P1_1+P2_1+X1+SR1+DR1+M1
//@EXPECTATION: �ɹ�ִ��blit������CSUDI_SUCCESS
//@REMARK: ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ   
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:3������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M1��
//@EXECUTIONFLOW:4������CSUDISync���blit����
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡP2_1��ͼƽ����ϢP2_1_Info
//@EXECUTIONFLOW:6������Ŀ�Ļ�ͼƽ���ڵ��������ɫֵΪOSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
//@EXECUTIONFLOW:8��������ͼƽ�����ɫģʽΪX2,�ظ����ϲ���
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0001(void)
{  
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_1,EM_RECT_1,CSUDI_TRUE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_1,EM_RECT_1,CSUDI_TRUE);
	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0002
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������ΪCSUDI_NULL��Ŀ�ľ�������Ϊȫ����Сʱ��blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX1
//@INPUT: P1_1+P2_1+X1+SR1+DR2+M1
//@EXPECTATION: �ɹ�ִ��blit������CSUDI_SUCCESS
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ�����0x88
//@EXECUTIONFLOW:3������CSUDIOSGBlit��P2_1,DR2,P1_1,SR1,M1��
//@EXECUTIONFLOW:4������CSUDISync���blit����
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡP2_1��ͼƽ����ϢP2_1_Info
//@EXECUTIONFLOW:6������Ŀ�Ļ�ͼƽ���ڵ��������ɫֵΪOSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
//@EXECUTIONFLOW:8��������ͼƽ�����ɫģʽΪX2,�ظ����ϲ���
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0002(void)
{   	
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_1,EM_RECT_2,CSUDI_TRUE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_1,EM_RECT_2,CSUDI_TRUE);
	return nRet;
}

//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������ΪCSUDI_NULL��Ŀ�ľ��������СΪ�Ƿ�ֵ��0��0��ʱ��blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX1
//@INPUT: P1_1+P2_1+X1+SR1+DR4+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:3������CSUDIOSGBlit��P2_1,DR4,P1_1,SR1,M1��������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4������CSUDIOSGDestroySurface���ٻ�ͼƽ��
//@EXECUTIONFLOW:5��������ͼƽ�����ɫģʽΪX2,�ظ����ϲ���
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0003(void)
{   
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_1,EM_RECT_4,CSUDI_FALSE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_1,EM_RECT_4,CSUDI_FALSE);
	return nRet;
}

//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������ΪCSUDI_NULL��Ŀ�ľ��������СΪ�Ƿ�ֵ��-1��-1��ʱ��blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX1
//@INPUT: P1_1+P2_1+X1+SR1+DR5+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:3������CSUDIOSGBlit��P2_1,DR5,P1_1,SR1,M1��������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4������CSUDIOSGDestroySurface���ٻ�ͼƽ��
//@EXECUTIONFLOW:5��������ͼƽ�����ɫģʽΪX2,�ظ����ϲ���
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0004(void)
{   
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_1,EM_RECT_5,CSUDI_FALSE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_1,EM_RECT_5,CSUDI_FALSE);
	return nRet;
}

//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������Ϊȫ����С��Ŀ�ľ�������CSUDI_NULLʱ��blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX1
//@INPUT: P1_1+P2_1+X1+SR2+DR1+M1
//@EXPECTATION: ִ��blit�ɹ�������CSUDI_SUCCESS
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ�����0x88
//@EXECUTIONFLOW:3������CSUDIOSGBlit��P2_1,DR1,P1_1,SR2,M1��
//@EXECUTIONFLOW:4������CSUDISync���blit����
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡP2_1��ͼƽ����ϢP2_1_Info
//@EXECUTIONFLOW:6������Ŀ�Ļ�ͼƽ���ڵ��������ɫֵΪOSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
//@EXECUTIONFLOW:8��������ͼƽ�����ɫģʽΪX2,�ظ����ϲ���
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0005(void)
{   
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_2,EM_RECT_1,CSUDI_TRUE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_2,EM_RECT_1,CSUDI_TRUE);
	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0006
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������Ϊȫ����С��Ŀ�ľ�������Ϊȫ����Сʱ��blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX1
//@INPUT: P1_1+P2_1+X1+SR2+DR2+M1
//@EXPECTATION: �ɹ�ִ��blit������CSUDI_SUCCESS
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ�����0x88
//@EXECUTIONFLOW:3������CSUDIOSGBlit��P2_1,DR2,P1_1,SR2,M1��
//@EXECUTIONFLOW:4������CSUDISync���blit����
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡP2_1��ͼƽ����ϢP2_1_Info
//@EXECUTIONFLOW:6��������ͼƽ���ڵ��������ɫֵΪOSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
//@EXECUTIONFLOW:8��������ͼƽ�����ɫģʽΪX2,�ظ����ϲ���
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0006(void)
{   
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_2,EM_RECT_2,CSUDI_TRUE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_2,EM_RECT_2,CSUDI_TRUE);
	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0007
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������Ϊȫ����С��Ŀ�ľ�������Ϊ�Ƿ�ֵ��-1��-1��ʱ��blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX1
//@INPUT: P1_1+P2_1+X1+SR2+DR5+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ�����
//@EXECUTIONFLOW:3������CSUDIOSGBlit��P2_1,DR5,P1_1,SR2,M1��������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4������CSUDIOSGDestroySurface���ٻ�ͼƽ��
//@EXECUTIONFLOW:5��������ͼƽ�����ɫģʽΪX2,�ظ����ϲ���
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0007(void)
{   
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_2,EM_RECT_5,CSUDI_FALSE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_2,EM_RECT_5,CSUDI_FALSE);
	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0008
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��Ŀ�ľ�������Ϊ��Ļ����λ�ã�200*200��ʱ��blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX1
//@INPUT: P1_1+P2_1+X1+SR3+DR3+M1
//@EXPECTATION: �ɹ�ִ��blit������CSUDI_SUCCESS
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ   
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:3������CSUDIOSGBlit��P2_1,DR3,P1_1,SR3,M1��������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDISync���blit����
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡP2_1��ͼƽ����ϢP2_1_Info
//@EXECUTIONFLOW:6������Ŀ�Ļ�ͼƽ��DR3�����ڵ��������ɫֵΪOSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
//@EXECUTIONFLOW:8��������ͼƽ�����ɫģʽΪX2,�ظ����ϲ���
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0008(void)
{   
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_3,EM_RECT_3,CSUDI_TRUE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_3,EM_RECT_3,CSUDI_TRUE);
	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0009
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������Ϊȫ����С��Ŀ�ľ�����������Ļ���½�Խ���blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX1
//@INPUT: P1_1+P2_1+X1+SR3+DR8+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:3������CSUDIOSGBlit��P2_1,DR8,P1_1,SR3,M1��������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4������CSUDIOSGDestroySurface���ٻ�ͼƽ��
//@EXECUTIONFLOW:5��������ͼƽ�����ɫģʽΪX2,�ظ����ϲ���
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0009(void)
{   
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_3,EM_RECT_8,CSUDI_FALSE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_3,EM_RECT_8,CSUDI_FALSE);
	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0010
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������Ϊȫ����С��Ŀ�ľ�����������Ļ���б�Խ���blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX1
//@INPUT: P1_1+P2_1+X1+SR3+DR9+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:3������CSUDIOSGBlit��P2_1,DR9,P1_1,SR3,M1��������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4������CSUDIOSGDestroySurface���ٻ�ͼƽ��
//@EXECUTIONFLOW:5��������ͼƽ�����ɫģʽΪX2,�ظ����ϲ���
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0010(void)
{   
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_3,EM_RECT_9,CSUDI_FALSE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_3,EM_RECT_9,CSUDI_FALSE);
	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0011
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������Ϊȫ����С��Ŀ�ľ�����������Ļ���½�Խ���blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX1
//@INPUT: P1_1+P2_1+X1+SR3+DR10+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:3������CSUDIOSGBlit��P2_1,DR10,P1_1,SR3,M1��������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4������CSUDIOSGDestroySurface���ٻ�ͼƽ��
//@EXECUTIONFLOW:5��������ͼƽ�����ɫģʽΪX2,�ظ����ϲ���
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0011(void)
{   
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_3,EM_RECT_10,CSUDI_FALSE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_3,EM_RECT_10,CSUDI_FALSE);
	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0012
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������Ϊȫ����С��Ŀ�ľ�����������Ļ���б�Խ���blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX1
//@INPUT: P1_1+P2_1+X1+SR3+DR11+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:3������CSUDIOSGBlit��P2_1,DR11,P1_1,SR3,M1��������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4������CSUDIOSGDestroySurface���ٻ�ͼƽ��
//@EXECUTIONFLOW:5��������ͼƽ�����ɫģʽΪX2,�ظ����ϲ���
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0012(void)
{   	
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_3,EM_RECT_11,CSUDI_FALSE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_3,EM_RECT_11,CSUDI_FALSE);
	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0013
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������Ϊȫ����С��Ŀ�ľ�����������Ļ���Ͻ�Խ���blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX1
//@INPUT: P1_1+P2_1+X1+SR3+DR12+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ   
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:3������CSUDIOSGBlit��P2_1,DR12,P1_1,SR3,M1��������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4������CSUDIOSGDestroySurface���ٻ�ͼƽ��
//@EXECUTIONFLOW:5��������ͼƽ�����ɫģʽΪX2,�ظ����ϲ���
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0013(void)
{   		
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_3,EM_RECT_12,CSUDI_FALSE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_3,EM_RECT_12,CSUDI_FALSE);
	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0014
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��Ŀ�ľ�������Ϊ�Ƿ�ֵ��0��0��ʱ��blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX1
//@INPUT: P1_1+P2_1+X1+SR4+DR4+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:3������CSUDIOSGBlit��P2_1,DR4,P1_1,SR4,M1��������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4������CSUDIOSGDestroySurface���ٻ�ͼƽ��
//@EXECUTIONFLOW:5��������ͼƽ�����ɫģʽΪX2,�ظ����ϲ���
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0014(void)
{   
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_4,EM_RECT_4,CSUDI_FALSE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_4,EM_RECT_4,CSUDI_FALSE);
	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0015
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������Ϊ�Ƿ�ֵ��0��0����Ŀ�ľ�������Ϊһ�����أ�1��1��ʱ��blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX1
//@INPUT: P1_1+P2_1+X1+SR4+DR6+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:3������CSUDIOSGBlit��P2_1,DR6,P1_1,SR4,M1��
//@EXECUTIONFLOW:4������CSUDISync���blit����
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡP2_1��ͼƽ����ϢP2_1_Info
//@EXECUTIONFLOW:6������Ŀ�Ļ�ͼƽ��DR6�����ڵ��������ɫֵΪOSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
//@EXECUTIONFLOW:8��������ͼƽ�����ɫģʽΪX2,�ظ����ϲ���
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0015(void)
{   	
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_4,EM_RECT_6,CSUDI_FALSE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_4,EM_RECT_6,CSUDI_FALSE);
	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0016
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������Ϊ�Ƿ�ֵ��-1��-1��ʱ��blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX1
//@INPUT: P1_1+P2_1+X1+SR5+DR5+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:3������CSUDIOSGBlit��P2_1,SR5,P1_1,SR5,M1��������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4������CSUDIOSGDestroySurface���ٻ�ͼƽ��
//@EXECUTIONFLOW:5��������ͼƽ�����ɫģʽΪX2,�ظ����ϲ���
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0016(void)
{   
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_5,EM_RECT_5,CSUDI_FALSE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_5,EM_RECT_5,CSUDI_FALSE);
	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0017
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������Ϊһ�����أ�1��1��ʱ��blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX1
//@INPUT: P1_1+P2_1+X1+SR6+DR6+M1
//@EXPECTATION: �ɹ�ִ��blit������CSUDI_SUCCESS
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:3������CSUDIOSGBlit��P2_1,SR6,P1_1,DR6,M1��
//@EXECUTIONFLOW:4������CSUDISync���blit����
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡP2_1��ͼƽ����ϢP2_1_Info
//@EXECUTIONFLOW:6������Ŀ�Ļ�ͼƽ��DR6�����ڵ��������ɫֵΪOSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
//@EXECUTIONFLOW:8��������ͼƽ�����ɫģʽΪX2,�ظ����ϲ���
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0017(void)
{     
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_6,EM_RECT_6,CSUDI_TRUE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_6,EM_RECT_6,CSUDI_TRUE);
	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0018
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������Ϊһ�����أ�1��1����Ŀ�ľ�������Ϊ�Ƿ�ֵ��-1��-1��ʱ��blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX1
//@INPUT: P1_1+P2_1+X1+SR6+DR5+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ�����
//@EXECUTIONFLOW:3������CSUDIOSGBlit��P2_1,SR6,P1_1,DR5,M1��������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4������CSUDIOSGDestroySurface���ٻ�ͼƽ��
//@EXECUTIONFLOW:5��������ͼƽ�����ɫģʽΪX2,�ظ����ϲ���
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0018(void)
{   
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_6,EM_RECT_5,CSUDI_FALSE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_6,EM_RECT_5,CSUDI_FALSE);
	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0019
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ�Ŵ�15������blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX1
//@INPUT: P1_1+P2_1+X1+SR6+DR7+M1
//@EXPECTATION: �ɹ�ִ��blit������CSUDI_SUCCESS
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ�����
//@EXECUTIONFLOW:3������CSUDIOSGBlit��P2_1,SR6,P1_1,DR7,M1��
//@EXECUTIONFLOW:4������CSUDISync���blit����
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡP2_1��ͼƽ����ϢP2_1_Info
//@EXECUTIONFLOW:6������Ŀ�Ļ�ͼƽ��DR7�����ڵ��������ɫֵΪOSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
//@EXECUTIONFLOW:8��������ͼƽ�����ɫģʽΪX2,�ظ����ϲ���
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0019(void)
{   	
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_6,EM_RECT_7,CSUDI_TRUE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_6,EM_RECT_7,CSUDI_TRUE);
	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0020
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��С15������blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX1
//@INPUT: P1_1+P2_1+X1+SR7+DR6+M1
//@EXPECTATION: �ɹ�ִ��blit������CSUDI_SUCCESS
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ�����
//@EXECUTIONFLOW:3������CSUDIOSGBlit��P2_1,SR7,P1_1,DR6,M1��
//@EXECUTIONFLOW:4������CSUDISync���blit����
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡP2_1��ͼƽ����ϢP2_1_Info
//@EXECUTIONFLOW:6������Ŀ�Ļ�ͼƽ��DR6�����ڵ��������ɫֵΪOSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
//@EXECUTIONFLOW:8��������ͼƽ�����ɫģʽΪX2,�ظ����ϲ���
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0020(void)
{   
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_7,EM_RECT_6,CSUDI_TRUE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_7,EM_RECT_6,CSUDI_TRUE);
	return nRet;
}

//***************************Ŀ���ͼ�ռ�Ϊ�Դ�ռ�************************//

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0021
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��Ŀ�ľ�������ΪCSUDI_NULLʱ��blit����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: P1_1+D+SR1+DR1+M1
//@EXPECTATION: �ɹ�ִ��blit������CSUDI_SUCCESS
//@REMARK: ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ   
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���D�������ɹ�
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ�Դ�ռ�D����Ϣ����С����ɫģʽ
//@EXECUTIONFLOW:3�����ݲ���(2)��ȡ����Ϣ����CSUDIOSGCreateSurface��������ռ�P1_1
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:5������CSUDIOSGBlit��D,DR1,P1_1,SR1,M1��
//@EXECUTIONFLOW:6������CSUDISync���blit����
//@EXECUTIONFLOW:7������CSUDIOSGGetSurfaceInfo��ȡD�Դ�ռ����Ϣ
//@EXECUTIONFLOW:8�������Դ�ռ��ڵ��������ɫֵΪOSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:9������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0021(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_1,EM_RECT_1,CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0022
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������ΪCSUDI_NULL��Ŀ�ľ�������Ϊȫ����Сʱ��blit����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: P1_1+D+SR1+DR2+M1
//@EXPECTATION: �ɹ�ִ��blit������CSUDI_SUCCESS
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ 
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���D�������ɹ�
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ�Դ�ռ�D����Ϣ����С����ɫģʽ
//@EXECUTIONFLOW:3�����ݲ���(2)��ȡ����Ϣ����CSUDIOSGCreateSurface��������ռ�P1_1
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:5������CSUDIOSGBlit��D,DR2,P1_1,SR1,M1��
//@EXECUTIONFLOW:6������CSUDISync���blit����
//@EXECUTIONFLOW:7������CSUDIOSGGetSurfaceInfo��ȡD�Դ�ռ����Ϣ
//@EXECUTIONFLOW:8�������Դ�ռ��ڵ��������ɫֵΪOSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:9������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0022(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_1,EM_RECT_2,CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0023
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������ΪCSUDI_NULL��Ŀ�ľ��������СΪ�Ƿ�ֵ��0��0��ʱ��blit����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: P1_1+D+SR1+DR4+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���D�������ɹ�
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ�Դ�ռ�D����Ϣ����С����ɫģʽ
//@EXECUTIONFLOW:3�����ݲ���(2)��ȡ����Ϣ����CSUDIOSGCreateSurface��������ռ�P1_1
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:5������CSUDIOSGBlit��D,DR4,P1_1,SR1,M1��������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0023(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_1,EM_RECT_4,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0024
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������ΪCSUDI_NULL��Ŀ�ľ��������СΪ�Ƿ�ֵ��-1��-1��ʱ��blit����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: P1_1+D+SR1+DR5+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���D�������ɹ�
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ�Դ�ռ�D����Ϣ����С����ɫģʽ
//@EXECUTIONFLOW:3�����ݲ���(2)��ȡ����Ϣ����CSUDIOSGCreateSurface��������ռ�P1_1
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:3������CSUDIOSGBlit��D,DR5,P1_1,SR1,M1��������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0024(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_1,EM_RECT_5,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0025
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������Ϊȫ����С��Ŀ�ľ�������CSUDI_NULLʱ��blit����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: P1_1+D+SR2+DR1+M1
//@EXPECTATION: �ɹ�ִ��blit������CSUDI_SUCCESS
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���D�������ɹ�
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ�Դ�ռ�D����Ϣ����С����ɫģʽ
//@EXECUTIONFLOW:3�����ݲ���(2)��ȡ����Ϣ����CSUDIOSGCreateSurface��������ռ�P1_1
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:5������CSUDIOSGBlit��D,DR1,P1_1,SR2,M1��
//@EXECUTIONFLOW:6������CSUDISync���blit����
//@EXECUTIONFLOW:7������CSUDIOSGGetSurfaceInfo��ȡD�Դ�ռ����Ϣ
//@EXECUTIONFLOW:8�������Դ�ռ��ڵ��������ɫֵΪOSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:4������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0025(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_2,EM_RECT_1,CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0026
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������Ϊȫ����С��Ŀ�ľ�������Ϊȫ����Сʱ��blit����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: P1_1+D+SR2+DR2+M1
//@EXPECTATION: �ɹ�ִ��blit������CSUDI_SUCCESS
//@REMARK: ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ   
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���D�������ɹ�
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ�Դ�ռ�D����Ϣ����С����ɫģʽ
//@EXECUTIONFLOW:3�����ݲ���(2)��ȡ����Ϣ����CSUDIOSGCreateSurface��������ռ�P1_1
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:5������CSUDIOSGBlit��D,DR2,P1_1,SR2,M1��
//@EXECUTIONFLOW:6������CSUDISync���blit����
//@EXECUTIONFLOW:7������CSUDIOSGGetSurfaceInfo��ȡD�Դ�ռ����Ϣ
//@EXECUTIONFLOW:8�������Դ�ռ��ڵ��������ɫֵΪOSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:4������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0026(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_2,EM_RECT_2,CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0027
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������Ϊȫ����С��Ŀ�ľ�������Ϊ��Ļ����λ�ã�200*200��ʱ��blit����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: P1_1+D+SR2+DR3+M1
//@EXPECTATION: �ɹ�ִ��blit������CSUDI_SUCCESS
//@REMARK: ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ   
//@REMARK: ����������Ч
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���D�������ɹ�
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ�Դ�ռ�D����Ϣ����С����ɫģʽ
//@EXECUTIONFLOW:3�����ݲ���(2)��ȡ����Ϣ����CSUDIOSGCreateSurface��������ռ�P1_1
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:5������CSUDIOSGBlit��D,DR3,P1_1,SR2,M1��
//@EXECUTIONFLOW:6������CSUDISync���blit����
//@EXECUTIONFLOW:7������CSUDIOSGGetSurfaceInfo��ȡD�Դ�ռ����Ϣ
//@EXECUTIONFLOW:8�������Դ�ռ��ڵ��������ɫֵΪOSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:9������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0027(void)
{   		
	CSTCPrint("Not realized yet !!!\n");
	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0028
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������Ϊȫ����С��Ŀ�ľ�������Ϊ�Ƿ�ֵ��-1��-1��ʱ��blit����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: P1_1+D+SR2+DR5+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���D�������ɹ�
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ�Դ�ռ�D����Ϣ����С����ɫģʽ
//@EXECUTIONFLOW:3�����ݲ���(2)��ȡ����Ϣ����CSUDIOSGCreateSurface��������ռ�P1_1
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:5������CSUDIOSGBlit��D,DR5,P1_1,SR2,M1��������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0028(void)
{   	
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_2,EM_RECT_5,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0029
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ�;�������Ϊ��Ļ����λ�ã�200*200��ʱ��blit����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: P1_1+D+SR3+DR3+M1
//@EXPECTATION: �ɹ�ִ��blit������CSUDI_SUCCESS
//@REMARK: ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ   
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���D�������ɹ�
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ�Դ�ռ�D����Ϣ����С����ɫģʽ
//@EXECUTIONFLOW:3�����ݲ���(2)��ȡ����Ϣ����CSUDIOSGCreateSurface��������ռ�P1_1
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:5������CSUDIOSGBlit��D,DR3,P1_1,SR3,M1��
//@EXECUTIONFLOW:6������CSUDISync���blit����
//@EXECUTIONFLOW:7������CSUDIOSGGetSurfaceInfo��ȡD�Դ�ռ����Ϣ
//@EXECUTIONFLOW:8�������Դ�ռ��ڵ��������ɫֵΪOSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:9������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0029(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_3,EM_RECT_3,CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0030
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������Ϊȫ����С��Ŀ�ľ�����������Ļ���½�Խ���blit����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: P1_1+D+SR2+DR8+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���D�������ɹ�
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ�Դ�ռ�D����Ϣ����С����ɫģʽ
//@EXECUTIONFLOW:3�����ݲ���(2)��ȡ����Ϣ����CSUDIOSGCreateSurface��������ռ�P1_1
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:5������CSUDIOSGBlit��D,DR8,P1_1,SR2,M1��������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0030(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_2,EM_RECT_8,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0031
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������Ϊȫ����С��Ŀ�ľ�����������Ļ���б�Խ���blit����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: P1_1+D+SR2+DR9+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���D�������ɹ�
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ�Դ�ռ�D����Ϣ����С����ɫģʽ
//@EXECUTIONFLOW:3�����ݲ���(2)��ȡ����Ϣ����CSUDIOSGCreateSurface��������ռ�P1_1
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:5������CSUDIOSGBlit��D,DR9,P1_1,SR2,M1��������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0031(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_2,EM_RECT_9,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0032
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������Ϊȫ����С��Ŀ�ľ�����������Ļ���½�Խ���blit����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: P1_1+D+SR2+DR10+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���D�������ɹ�
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ�Դ�ռ�D����Ϣ����С����ɫģʽ
//@EXECUTIONFLOW:3�����ݲ���(2)��ȡ����Ϣ����CSUDIOSGCreateSurface��������ռ�P1_1
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:5������CSUDIOSGBlit��D,DR10,P1_1,SR2,M1��������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0032(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_2,EM_RECT_10,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0033
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������Ϊȫ����С��Ŀ�ľ�����������Ļ���б�Խ���blit����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: P1_1+D+SR2+DR11+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���D�������ɹ�
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ�Դ�ռ�D����Ϣ����С����ɫģʽ
//@EXECUTIONFLOW:3�����ݲ���(2)��ȡ����Ϣ����CSUDIOSGCreateSurface��������ռ�P1_1
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:5������CSUDIOSGBlit��D,DR11,P1_1,SR2,M1��������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0033(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_2,EM_RECT_11,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0034
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������Ϊȫ����С��Ŀ�ľ�����������Ļ���Ͻ�Խ���blit����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: P1_1+D+SR2+DR12+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���D�������ɹ�
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ�Դ�ռ�D����Ϣ����С����ɫģʽ
//@EXECUTIONFLOW:3�����ݲ���(2)��ȡ����Ϣ����CSUDIOSGCreateSurface��������ռ�P1_1
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:5������CSUDIOSGBlit��D,DR12,P1_1,SR2,M1��������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0034(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_2,EM_RECT_12,CSUDI_FALSE);	
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0035
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������Ϊ�Ƿ�ֵ��0��0��ʱ��blit����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: P1_1+D+SR4+DR4+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���D�������ɹ�
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ�Դ�ռ�D����Ϣ����С����ɫģʽ
//@EXECUTIONFLOW:3�����ݲ���(2)��ȡ����Ϣ����CSUDIOSGCreateSurface��������ռ�P1_1
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:5������CSUDIOSGBlit��D,DR4,P1_1,SR4,M1��������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0035(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_4,EM_RECT_4,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0036
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������Ϊ�Ƿ�ֵ��0��0����Ŀ�ľ�������Ϊһ�����أ�1��1��ʱ��blit����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: P1_1+D+SR4+DR6+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���D�������ɹ�
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ�Դ�ռ�D����Ϣ����С����ɫģʽ
//@EXECUTIONFLOW:3�����ݲ���(2)��ȡ����Ϣ����CSUDIOSGCreateSurface��������ռ�P1_1
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:5������CSUDIOSGBlit��D,DR6,P1_1,SR4,M1��������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0036(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_4,EM_RECT_6,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0037
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������Ϊ�Ƿ�ֵ��-1��-1��ʱ��blit����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: P1_1+D+SR5+DR5+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���D�������ɹ�
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ�Դ�ռ�D����Ϣ����С����ɫģʽ
//@EXECUTIONFLOW:3�����ݲ���(2)��ȡ����Ϣ����CSUDIOSGCreateSurface��������ռ�P1_1
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:5������CSUDIOSGBlit��D,DR5,P1_1,SR5,M1��������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0037(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_5,EM_RECT_5,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0038
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��Ŀ�ľ�������Ϊһ�����أ�1��1��ʱ��blit����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: P1_1+D+SR6+DR6+M1
//@EXPECTATION: �ɹ�ִ��blit������CSUDI_SUCCESS
//@REMARK: ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ   
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���D�������ɹ�
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ�Դ�ռ�D����Ϣ����С����ɫģʽ
//@EXECUTIONFLOW:3�����ݲ���(2)��ȡ����Ϣ����CSUDIOSGCreateSurface��������ռ�P1_1
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:5������CSUDIOSGBlit��D,DR6,P1_1,SR6,M1��
//@EXECUTIONFLOW:6������CSUDISync���blit����
//@EXECUTIONFLOW:7������CSUDIOSGGetSurfaceInfo��ȡD�Դ�ռ����Ϣ
//@EXECUTIONFLOW:8�������Դ�ռ��ڵ��������ɫֵΪOSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:9������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0038(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_6,EM_RECT_6,CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0039
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������Ϊһ�����أ�1��1����Ŀ�ľ�������Ϊ�Ƿ�ֵ��-1��-1��ʱ��blit����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: P1_1+D+SR6+DR5+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���D�������ɹ�
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ�Դ�ռ�D����Ϣ����С����ɫģʽ
//@EXECUTIONFLOW:3�����ݲ���(2)��ȡ����Ϣ����CSUDIOSGCreateSurface��������ռ�P1_1
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:5������CSUDIOSGBlit��D,DR5,P1_1,SR6,M1��������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0039(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_6,EM_RECT_5,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0040
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ�Ŵ�15������blit����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: P1_1+D+SR7+DR6+M1
//@EXPECTATION: �ɹ�ִ��blit������CSUDI_SUCCESS
//@REMARK: ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ   
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���D�������ɹ�
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ�Դ�ռ�D����Ϣ����С����ɫģʽ
//@EXECUTIONFLOW:3�����ݲ���(2)��ȡ����Ϣ����CSUDIOSGCreateSurface��������ռ�P1_1
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:5������CSUDIOSGBlit��D,DR7,P1_1,SR6,M1��
//@EXECUTIONFLOW:6������CSUDISync���blit����
//@EXECUTIONFLOW:7������CSUDIOSGGetSurfaceInfo��ȡD�Դ�ռ����Ϣ
//@EXECUTIONFLOW:8�������Դ�ռ��ڵ��������ɫֵΪOSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:9������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0040(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_7,EM_RECT_6,CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0041
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��С15������blit����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: P1_1+D+SR6+DR7+M1
//@EXPECTATION: �ɹ�ִ��blit������CSUDI_SUCCESS
//@REMARK: ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ   
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���D�������ɹ�
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ�Դ�ռ�D����Ϣ����С����ɫģʽ
//@EXECUTIONFLOW:3�����ݲ���(2)��ȡ����Ϣ����CSUDIOSGCreateSurface��������ռ�P1_1
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:5������CSUDIOSGBlit��D,DR6,P1_1,SR7,M1��
//@EXECUTIONFLOW:6������CSUDISync���blit����
//@EXECUTIONFLOW:7������CSUDIOSGGetSurfaceInfo��ȡD�Դ�ռ����Ϣ
//@EXECUTIONFLOW:8�������Դ�ռ��ڵ��������ɫֵΪOSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:9������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0041(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_6,EM_RECT_7,CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0042
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������Ϊȫ����С��Ŀ�ľ�����������Ļ���Ͻ�Խ���blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX1
//@INPUT: P1_1+P2_1+X1+SR2+DR14+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ   
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ�����
//@EXECUTIONFLOW:3������CSUDIOSGBlit��P2_1,DR14,P1_1,SR2,M1��������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0042(void)
{   	
	return OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_2,EM_RECT_14,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0043
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������Ϊȫ����С��Ŀ�ľ�����������Ļ���б�Խ���blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX1
//@INPUT: P1_1+P2_1+X1+SR2+DR15+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ   
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1����ɫģʽΪX1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ�����
//@EXECUTIONFLOW:3������CSUDIOSGBlit��P2_1,DR15,P1_1,SR2,M1��������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0043(void)
{   		
	return OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_2,EM_RECT_15,CSUDI_FALSE);	
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0044
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������Ϊȫ����С��Ŀ�ľ�����������Ļ���б�Խ���blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX1
//@INPUT: P1_1+P2_1+X1+SR2+DR16+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ   
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1����ɫģʽΪX1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ�����
//@EXECUTIONFLOW:3������CSUDIOSGBlit��P2_1,DR16,P1_1,SR2,M1��������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0044(void)
{   		
	return OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_2,EM_RECT_16,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0045
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������Ϊȫ����С��Ŀ�ľ�����������Ļ���Ͻ�Խ���blit����(Ŀ��Ϊ�Դ�ռ�)
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: P1_1+D+SR2+DR14+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���D�������ɹ�
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ�Դ�ռ�D����Ϣ����С����ɫģʽ
//@EXECUTIONFLOW:3�����ݲ���(2)��ȡ����Ϣ����CSUDIOSGCreateSurface��������ռ�P1_1
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:5������CSUDIOSGBlit��D,DR14,P1_1,SR2,M1��������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0045(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_2,EM_RECT_14,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0046
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������Ϊȫ����С��Ŀ�ľ�����������Ļ���б�Խ���blit����(Ŀ��Ϊ�Դ�ռ�)
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: P1_1+D+SR2+DR15+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���D�������ɹ�
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ�Դ�ռ�D����Ϣ����С����ɫģʽ
//@EXECUTIONFLOW:3�����ݲ���(2)��ȡ����Ϣ����CSUDIOSGCreateSurface��������ռ�P1_1
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:5������CSUDIOSGBlit��D,DR15,P1_1,SR2,M1��������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0046(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_2,EM_RECT_15,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0047
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��������Ϊȫ����С��Ŀ�ľ�����������Ļ���б�Խ���blit����(Ŀ��Ϊ�Դ�ռ�)
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: P1_1+D+SR2+DR16+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���D�������ɹ�
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ�Դ�ռ�D����Ϣ����С����ɫģʽ
//@EXECUTIONFLOW:3�����ݲ���(2)��ȡ����Ϣ����CSUDIOSGCreateSurface��������ռ�P1_1
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:5������CSUDIOSGBlit��D,DR16,P1_1,SR2,M1��������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0047(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_2,EM_RECT_16,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0048
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������С��blit����(��Ŀ�������С��Դ����ҪС)
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪ��ǰ֧�ֵ���ɫģʽX2��X1
//@EXPECTATION: �ɹ�ִ��blit������CSUDI_SUCCESS
//@REMARK: ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ   
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�SurfaceSrc��SurfaceDst����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:3������CSUDIOSGBlit,��SurfaceSrc������blit��SurfaceDst�ϣ�Դ����Ϊ(x,y,w,h):(0, 0, 720, 576),Ŀ������Ϊ(x,y,w,h):(180,144,360,288)
//@EXECUTIONFLOW:4������CSUDISync���blit����
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡSurfaceDst��ͼƽ����ϢP2_1_Info
//@EXECUTIONFLOW:6������Ŀ�Ļ�ͼƽ���ڵ�(x,y,w,h):(180,144,360,288)����ɫֵΪ0x88
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0048(void)
{   
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	
	//Ѱ��һ��֧�ֵ�ePixelFormat
	CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
					"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
				);
		
	bRet = OSG_Blit_TestRect(ePixelFormat,0x88,EM_RECT_1,EM_RECT_17,CSUDI_TRUE);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == bRet),"blit ��������ʧ��!");
	
	CSTK_FATAL_POINT;
	
	return bRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0049
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������С��blit����(��Ŀ�������С��Դ����Ҫ��)
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪ��ǰ֧�ֵ���ɫģʽX2��X1
//@EXPECTATION: �ɹ�ִ��blit������CSUDI_SUCCESS
//@REMARK: ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ   
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�SurfaceSrc��SurfaceDst����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:3������CSUDIOSGBlit,��SurfaceSrc������blit��SurfaceDst�ϣ�Դ����Ϊ(x,y,w,h):(180,144,360,288),Ŀ������Ϊ(x,y,w,h):(0, 0, 720, 576)
//@EXECUTIONFLOW:4������CSUDISync���blit����
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡSurfaceDst��ͼƽ����ϢP2_1_Info
//@EXECUTIONFLOW:6������Ŀ�Ļ�ͼƽ���ڵ�(x,y,w,h):(0, 0, 720, 576)����ɫֵΪ0x88
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0049(void)
{   
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	
	//Ѱ��һ��֧�ֵ�ePixelFormat
	CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
					"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
				);
		
	bRet = OSG_Blit_TestRect(ePixelFormat,0x88,EM_RECT_17,EM_RECT_1,CSUDI_TRUE);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == bRet),"blit �Ŵ����ʧ��!");
	
	CSTK_FATAL_POINT;
	
	return bRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0050
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��Ŀ�ľ��������С��ͬ����ƫ�Ʋ�ͬ�ĶԵȵ�blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪ��ǰ֧�ֵ���ɫģʽX2��X1
//@EXPECTATION: �ɹ�ִ��blit������CSUDI_SUCCESS
//@REMARK: ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ   
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�SurfaceSrc��SurfaceDst����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:3������CSUDIOSGBlit,��SurfaceSrc������blit��SurfaceDst�ϣ�Դ����Ϊ(x,y,w,h):(180,144,360,288),Ŀ������Ϊ(x,y,w,h):(190, 154, 360, 288)
//@EXECUTIONFLOW:4������CSUDISync���blit����
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡSurfaceDst��ͼƽ����ϢP2_1_Info
//@EXECUTIONFLOW:6������Ŀ�Ļ�ͼƽ���ڵ�(x,y,w,h):(190, 154, 360, 288)����ɫֵΪ0x88
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0050(void)
{   
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	
	//Ѱ��һ��֧�ֵ�ePixelFormat
	CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
					"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
				);
	
	bRet = OSG_Blit_TestRect(ePixelFormat,0x88,EM_RECT_17,EM_RECT_18,CSUDI_TRUE);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == bRet),"blit ���Ե����򿽱�ʧ��!");
	
	CSTK_FATAL_POINT;
	
	return bRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0051
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��Ŀ�ľ��������С��ͬ����ƫ�Ʋ�ͬ�ĶԵȵ�blit����,����������߶�Ϊ����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪ��ǰ֧�ֵ���ɫģʽX2��X1
//@EXPECTATION: �ɹ�ִ��blit������CSUDI_SUCCESS
//@REMARK: ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ   
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�SurfaceSrc��SurfaceDst����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:3������CSUDIOSGBlit,��SurfaceSrc������blit��SurfaceDst�ϣ�Դ����Ϊ(x,y,w,h):(180,144,361,289),Ŀ������Ϊ(x,y,w,h):(190, 154, 361, 289)
//@EXECUTIONFLOW:4������CSUDISync���blit����
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡSurfaceDst��ͼƽ����ϢP2_1_Info
//@EXECUTIONFLOW:6������Ŀ�Ļ�ͼƽ���ڵ�(x,y,w,h):(190, 154, 361, 289)����ɫֵΪOSG_FILL_COLOR_GREY_32BIT
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0051(void)
{   
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	
	//Ѱ��һ��֧�ֵ�ePixelFormat
	CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
					"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
				);
		
	bRet = OSG_Blit_TestRect(ePixelFormat,0x88,EM_RECT_19,EM_RECT_23,CSUDI_TRUE);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == bRet),"blit ���Ե����򿽱����Ҹ߿�Ϊ����ʧ��!");

	CSTK_FATAL_POINT;
	
	return bRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0052
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��Ŀ�ľ��������С��ͬ(4000*100)����ƫ�Ʋ�ͬ�ĶԵȵ�blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: 
//@EXPECTATION: �ɹ�ִ��blit������CSUDI_SUCCESS
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ   
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�sSurfaceSrc��sSurfaceDst����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:3������CSUDIOSGBlit,��SurfaceSrc������blit��SurfaceDst�ϣ�Դ����Ϊ(x,y,w,h):(10,10,3000,50),Ŀ������Ϊ(x,y,w,h):(20, 10, 3000, 50)
//@EXECUTIONFLOW:4������CSUDISync���blit����
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡSurfaceDst��ͼƽ����ϢP2_1_Info
//@EXECUTIONFLOW:6������Ŀ�Ļ�ͼƽ�������ڵ��������ɫֵΪ������ɫOSG_FILL_COLOR_GREY_32BIT
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0052(void)
{   
	CSUDI_BOOL bRet = CSUDI_FALSE;
	
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;

	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hSurface_D = CSUDI_NULL;

	CSUDI_Error_Code nRet = CSUDI_FAILURE;

	CSUDIOSGSurfaceInfo_S sSurfaceInfo;
	CSUDIOSGRect_S sSRect ;
	CSUDIOSGRect_S sDRect ;
	unsigned int uExpectedColor = 0;			

	CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
					"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
				);

	/*Modified by lwh at 2010-12-01, check color rect is (10, 10, 3000, 50)and (20, 20, 3000, 50), the Height should be more than (20+50)*/
	nRet = CSUDIOSGCreateSurface(ePixelFormat,4000,100,&hSurface_S);
	CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),"Step 1 Create Surfacesrc Failure !");
		
	nRet = CSUDIOSGCreateSurface(ePixelFormat,4000,100,&hSurface_D);
	CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hSurface_D!= CSUDI_NULL),"Step 1 Create  Surfacedst Failure !");
			
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_S, &sSurfaceInfo)),"Get  Surfacesrc info failure !");	
	CSTK_ASSERT_TRUE_FATAL((sSurfaceInfo.m_pvSurfaceMem != CSUDI_NULL),"Get  Surfacesrc info failure !");

	if(sSurfaceInfo.m_pvSurfaceMem!= CSUDI_NULL && hSurface_S != CSUDI_NULL)
	{
		memset(sSurfaceInfo.m_pvSurfaceMem, 0x88, sSurfaceInfo.m_nMemSize);
	}

	sSRect.m_nX = 10;
	sSRect.m_nY = 10;
	sSRect.m_nWidth = 3000;
	sSRect.m_nHeight = 50;
	
	sDRect.m_nX = 20;
	sDRect.m_nY = 20;
	sDRect.m_nWidth = 3000;
	sDRect.m_nHeight = 50;

	switch(ePixelFormat)
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
				CSTK_ASSERT_TRUE_FATAL(0,"Not support pixelformat !!");
			}
			break;
	}
	
	nRet = CSUDIOSGBlit(hSurface_D, &sDRect, hSurface_S, &sSRect, EM_UDIOSG_OPMODE_COPY);

	CSTK_ASSERT_TRUE_FATAL(((nRet == CSUDI_SUCCESS)||(nRet == CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED)),/*�Ŵ�blitʱ���ܲ�֧��*/	"Step 4 Blit should be success !");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync())," Sync Failure !"	);

	bRet =  OSG_iCheckExistColor(hSurface_D,uExpectedColor,&sDRect,ePixelFormat);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == bRet),"step 6 Check Surface color Failure !");

	CSTK_FATAL_POINT;
	
	if(hSurface_S != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface_S)),"step 7 Destroy  surfacesrc failure !");
		hSurface_S = CSUDI_NULL;
	}
	
	if(hSurface_D != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface_D)),"step 7 Destroy  surfacedst failure !");
		hSurface_D = CSUDI_NULL;
	}		

	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0053
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��Ŀ�ľ��������С��ͬ(100*4000)����ƫ�Ʋ�ͬ�ĶԵȵ�blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: 
//@EXPECTATION: �ɹ�ִ��blit������CSUDI_SUCCESS
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ   
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�sSurfaceSrc��sSurfaceDst����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ�����Ϊ0x88
//@EXECUTIONFLOW:3������CSUDIOSGBlit,��SurfaceSrc������blit��SurfaceDst�ϣ�Դ����Ϊ(x,y,w,h):(10,10,50,3000),Ŀ������Ϊ(x,y,w,h):(10, 20, 50, 3000)
//@EXECUTIONFLOW:4������CSUDISync���blit����
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡSurfaceDst��ͼƽ����ϢP2_1_Info
//@EXECUTIONFLOW:6������Ŀ�Ļ�ͼƽ�������ڵ��������ɫֵΪ������ɫOSG_FILL_COLOR_GREY_32BIT
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0053(void)
{   
	CSUDI_BOOL bRet = CSUDI_FALSE;
	
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;

	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hSurface_D = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;

	CSUDIOSGSurfaceInfo_S sSurfaceInfo;
	CSUDIOSGRect_S sSRect ;
	CSUDIOSGRect_S sDRect ;
	unsigned int uExpectedColor = 0;			

	//Ѱ��һ��֧�ֵ�ePixelFormat
	CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
					"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
				);
	
	nRet = CSUDIOSGCreateSurface(ePixelFormat,100,4000,&hSurface_S);
	CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),"Step 1 Create Surfacesrc Failure !");
		
	nRet = CSUDIOSGCreateSurface(ePixelFormat,100,4000,&hSurface_D);
	CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hSurface_D!= CSUDI_NULL),"Step 1 Create  Surfacedst Failure !");
			
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_S, &sSurfaceInfo)),"Get  Surfacesrc info failure !");	
	CSTK_ASSERT_TRUE_FATAL((sSurfaceInfo.m_pvSurfaceMem != CSUDI_NULL),"Get  Surfacesrc info failure !");

	if(sSurfaceInfo.m_pvSurfaceMem!= CSUDI_NULL && hSurface_S != CSUDI_NULL)
	{
		memset(sSurfaceInfo.m_pvSurfaceMem, 0x88, sSurfaceInfo.m_nMemSize);
	}			

	sSRect.m_nX = 10;
	sSRect.m_nY = 10;
	sSRect.m_nWidth = 50;
	sSRect.m_nHeight = 3000;
	
	sDRect.m_nX = 10;
	sDRect.m_nY = 20;
	sDRect.m_nWidth = 50;
	sDRect.m_nHeight = 3000;

	switch(ePixelFormat)
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
				CSTK_ASSERT_TRUE_FATAL(0,"Not support pixelformat !!");
			}
			break;
	}
	
	nRet = CSUDIOSGBlit(hSurface_D, &sDRect, hSurface_S, &sSRect, EM_UDIOSG_OPMODE_COPY);

	CSTK_ASSERT_TRUE_FATAL(((nRet == CSUDI_SUCCESS)||(nRet == CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED)),/*�Ŵ�blitʱ���ܲ�֧��*/	"Step 4 Blit should be success !");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync())," Sync Failure !"	);

	bRet =  OSG_iCheckExistColor(hSurface_D,uExpectedColor,&sDRect,ePixelFormat);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == bRet),"step 6 Check Surface color Failure !");

	CSTK_FATAL_POINT;
	
	if(hSurface_S != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface_S)),"step 7 Destroy  surfacesrc failure !");
		hSurface_S = CSUDI_NULL;
	}
	
	if(hSurface_D != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface_D)),"step 7 Destroy  surfacedst failure !");
		hSurface_D = CSUDI_NULL;
	}		

	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0054
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ�Ŵ����15������blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: 1��P1_1+P2_1+X1+SR13+DR2+M1
//@INPUT: 1��P1_1+P2_1+X2+SR13+DR2+M1
//@EXPECTATION: �ɹ�ִ��blit������CSUDI_SUCCESS
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ�����
//@EXECUTIONFLOW:3������CSUDIOSGBlit��P2_1,SR13,P1_1,DR2,M1��
//@EXECUTIONFLOW:4������CSUDISync���blit����
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡP2_1��ͼƽ����ϢP2_1_Info
//@EXECUTIONFLOW:6������Ŀ�Ļ�ͼƽ��DR2�����ڵ��������ɫֵΪOSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
//@EXECUTIONFLOW:8��������ͼƽ�����ɫģʽΪX2,�ظ����ϲ���
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0054(void)
{   	
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_13,EM_RECT_2,CSUDI_TRUE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_13,EM_RECT_2,CSUDI_TRUE);

	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0055
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��С����15������blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: 1��P1_1+P2_1+X1+SR2+DR13+M1
//@INPUT: 2��P1_1+P2_1+X2+SR2+DR13+M1
//@EXPECTATION: �ɹ�ִ��blit������CSUDI_SUCCESS
//@REMARK:  ���Ծ��������Сʱ����û��ȥ��ע��ɫģʽ��colorkey��alpha����ɫֵ�ͻ��ģʽ    
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ�����
//@EXECUTIONFLOW:3������CSUDIOSGBlit��P2_1,SR2,P1_1,DR13,M1��
//@EXECUTIONFLOW:4������CSUDISync���blit����
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡP2_1��ͼƽ����ϢP2_1_Info
//@EXECUTIONFLOW:6������Ŀ�Ļ�ͼƽ��DR2�����ڵ��������ɫֵΪOSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
//@EXECUTIONFLOW:8��������ͼƽ�����ɫģʽΪX2,�ظ����ϲ���
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0055(void)
{   	
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_2,EM_RECT_13,CSUDI_TRUE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_2,EM_RECT_13,CSUDI_TRUE);

	return nRet;
}

//**********************************************************************************//
//******************************Begin Test colorkey Blit**************************//
//**********************************************************************************//

//@TESTCASEID:CSTC_OSG_IT_Blit_Ckey_0001
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��ͼƽ��ColorKeyΪC1ʱ��blit����,Դ��ͼƽ�����C1��ɫ�����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+C1+SR1+DR1+SCr1+DCr4+M1
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2������SCr1��ɫ�е�C1��ɫֵ������blit����
//@REMARK: 1������colorkeyʱ��Ҫ����ɫ����ֵ����û��ȥ��ע���������С����ɫģʽ��alpha�ͻ��ģʽ   
//@REMARK: 2��ֻ��X2��X1��ɫģʽ���в��ԣ���ʱ����������ɫģʽ���в��ԣ����Ը��ݲ��Ը����ʲ���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡԴ����ռ�P1_1����Ϣ
//@EXECUTIONFLOW:3������CSUDIOSGSetSurfaceInfo��Դ����ռ�P1_1��ColorKey����ΪC1
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ��������ɫֵΪSCr1
//@EXECUTIONFLOW:5����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr4
//@EXECUTIONFLOW:6������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M1��
//@EXECUTIONFLOW:7������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:8����P2_1_SInfo��Ϣ�л�ȡ����һ�����ص����ɫֵ������SCr1��ɫֵ
//@EXECUTIONFLOW:9������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Ckey_0001(void)
{   
	return OSG_Blit_TestCKey(
					EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,/*pixel format*/
					OSG_COLORKEY_BLACK,  /*colorkey*/
					OSG_COLOR_BLACK_32BIT,/*src color*/
					OSG_COLOR_BLUE_32BIT, /*dst color*/
					OSG_COLOR_BLACK_32BIT, /*check color*/
					CSUDI_FALSE);  /*check exist*/
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Ckey_0002
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��ͼƽ��ColorKeyΪC1ʱ��blit����,Ŀ���ͼƽ�����C1��ɫ�����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+C1+SR1+DR1+SCr1+DCr4+M1
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2������SCr1��ɫ�е�C1��ɫֵ������blit����
//@REMARK: 1������colorkeyʱ��Ҫ����ɫ����ֵ����û��ȥ��ע���������С����ɫģʽ��alpha�ͻ��ģʽ   
//@REMARK: 2��ֻ��X2��X1��ɫģʽ���в��ԣ���ʱ����������ɫģʽ���в��ԣ����Ը��ݲ��Ը����ʲ���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡԴ����ռ�P1_1����Ϣ
//@EXECUTIONFLOW:3������CSUDIOSGSetSurfaceInfo��Դ����ռ�P1_1��ColorKey����ΪC1
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ��������ɫֵΪSCr4
//@EXECUTIONFLOW:5����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr1
//@EXECUTIONFLOW:6������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M1��
//@EXECUTIONFLOW:7������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:8����P2_1_SInfo��Ϣ�л�ȡ����һ�����ص����ɫֵӦ�ô���SCr4��ɫֵ
//@EXECUTIONFLOW:9������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Ckey_0002(void)
{   
	return OSG_Blit_TestCKey(
					EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,/*pixel format*/
					OSG_COLORKEY_BLACK,  /*colorkey*/
					OSG_COLOR_BLUE_32BIT,/*src color*/
					OSG_COLOR_BLACK_32BIT, /*dst color*/
					OSG_COLOR_BLUE_32BIT, /*check color*/
					CSUDI_TRUE);  /*check exist*/
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Ckey_0003
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��ͼƽ��ColorKeyΪC1ʱ��blit����,����ƽ�涼������C1��ɫֵ�����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2+C1+SR1+DR1+SCr3+DCr4+M1
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2������SCr3��ɫ�е�C1��ɫֵ������blit����
//@REMARK: 1������colorkeyʱ��Ҫ����ɫ����ֵ����û��ȥ��ע���������С����ɫģʽ��alpha�ͻ��ģʽ   
//@REMARK: 2��ֻ��X2��X1��ɫģʽ���в��ԣ���ʱ����������ɫģʽ���в��ԣ����Ը��ݲ��Ը����ʲ���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡԴ����ռ�P1_1����Ϣ
//@EXECUTIONFLOW:3������CSUDIOSGSetSurfaceInfo��Դ����ռ�P1_1��ColorKey����ΪC1
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ��������ɫֵΪSCr2
//@EXECUTIONFLOW:5����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr4
//@EXECUTIONFLOW:6������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M1��������colorkey����SCr3������
//@EXECUTIONFLOW:7������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:8����P2_1_SInfo��Ϣ�л�ȡ����һ�����ص����ɫֵӦ�ô���SCr2��ɫֵ
//@EXECUTIONFLOW:9������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Ckey_0003(void)
{   
	return OSG_Blit_TestCKey(
					EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,/*pixel format*/
					OSG_COLORKEY_BLACK,  /*colorkey*/
					OSG_COLOR_GREEN_32BIT,/*src color*/
					OSG_COLOR_BLUE_32BIT, /*dst color*/
					OSG_COLOR_GREEN_32BIT, /*check color*/
					CSUDI_TRUE);  /*check exist*/
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Ckey_0004
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��ͼƽ��ColorKeyΪC2ʱ��blit����,����ƽ�涼������C2��ɫֵ�����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2+C2+SR1+DR1+SCr1+DCr4+M1
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2������SCr1��ɫ�е�C2��ɫֵ������blit����
//@REMARK: 1������colorkeyʱ��Ҫ����ɫ����ֵ����û��ȥ��ע���������С����ɫģʽ��alpha�ͻ��ģʽ   
//@REMARK: 2��ֻ��X2��X1��ɫģʽ���в��ԣ���ʱ����������ɫģʽ���в��ԣ����Ը��ݲ��Ը����ʲ���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡԴ����ռ�P1_1����Ϣ
//@EXECUTIONFLOW:3������CSUDIOSGSetSurfaceInfo��Դ����ռ�P1_1��ColorKey����ΪC2
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ��������ɫֵΪSCr1
//@EXECUTIONFLOW:5����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr4
//@EXECUTIONFLOW:6������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M1��
//@EXECUTIONFLOW:7������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:8����P2_1_SInfo��Ϣ�л�ȡ����һ�����ص����ɫֵӦ�ô���SCr1��ɫֵ
//@EXECUTIONFLOW:9������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Ckey_0004(void)
{   
	return OSG_Blit_TestCKey(
					EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,/*pixel format*/
					OSG_COLORKEY_WHITE,  /*colorkey*/
					OSG_COLOR_RED_32BIT,/*src color*/
					OSG_COLOR_BLUE_32BIT, /*dst color*/
					OSG_COLOR_RED_32BIT, /*check color*/
					CSUDI_TRUE);  /*check exist*/
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Ckey_0005
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Ŀ�Ļ�ͼƽ��ColorKeyΪC1ʱ��blit����,Դ��ͼƽ�����C1��ɫֵ�����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��Ŀ���ͼƽ̨����SCr1��ɫֵ
//@REMARK: 1������colorkeyʱ��Ҫ����ɫ����ֵ����û��ȥ��ע���������С����ɫģʽ��alpha�ͻ��ģʽ   
//@REMARK: 2����ʱֻ��1555����8888��ɫģʽ����
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�surface1��surface1����ɫģʽΪARGB8888��ARGB1555
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�surface2����Ϣ
//@EXECUTIONFLOW:3������CSUDIOSGSetSurfaceInfo��Ŀ�Ļ���ռ��ColorKey����ΪC1
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ��������ɫֵΪSCr1
//@EXECUTIONFLOW:5����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr4
//@EXECUTIONFLOW:6������CSUDIOSGBlit����ɫ����ɫ�Ͽ���
//@EXECUTIONFLOW:7������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�surface2����ϢSInfo
//@EXECUTIONFLOW:8����SInfo��Ϣ�л�ȡ����һ�����ص����ɫֵ����SCr1��ɫֵ
//@EXECUTIONFLOW:9������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Ckey_0005(void)
{   
	return OSG_Blit_TestCKeyForDst(
					EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,/*pixel format*/
					OSG_COLORKEY_BLACK,  /*colorkey*/
					OSG_COLOR_BLACK_32BIT,/*src color*/
					OSG_COLOR_BLUE_32BIT, /*dst color*/
					OSG_COLOR_BLACK_32BIT, /*check color*/
					CSUDI_TRUE);  /*check exist*/
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Ckey_0006
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Ŀ�Ļ�ͼƽ��ColorKeyΪ��ɫʱ��blit����,Ŀ���ͼƽ�����C1��ɫֵ�����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��Ŀ���ͼƽ̨������C1��ɫֵ
//@REMARK: 1������colorkeyʱ��Ҫ����ɫ����ֵ����û��ȥ��ע���������С����ɫģʽ��alpha�ͻ��ģʽ   
//@REMARK: 2����ʱֻ��1555����8888��ɫģʽ����
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�surface1��surface1����ɫģʽΪARGB8888��ARGB1555
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�surface2����Ϣ
//@EXECUTIONFLOW:3������CSUDIOSGSetSurfaceInfo��Ŀ�Ļ���ռ��ColorKey����ΪC1
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ��������ɫֵΪSCr4
//@EXECUTIONFLOW:5����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr1
//@EXECUTIONFLOW:6������CSUDIOSGBlit����ɫ����ɫ�Ͽ���
//@EXECUTIONFLOW:7������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�surface2����ϢSInfo
//@EXECUTIONFLOW:8����SInfo��Ϣ�л�ȡ����һ�����ص����ɫֵ������DCr1��ɫֵ
//@EXECUTIONFLOW:9������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Ckey_0006(void)
{   
	return OSG_Blit_TestCKeyForDst(
					EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,/*pixel format*/
					OSG_COLORKEY_BLACK,  /*colorkey*/
					OSG_COLOR_BLUE_32BIT,/*src color*/
					OSG_COLOR_BLACK_32BIT, /*dst color*/
					OSG_COLOR_BLACK_32BIT, /*check color*/
					CSUDI_FALSE);  /*check exist*/
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Ckey_0007
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Ŀ���ͼƽ��ColorKeyΪC1ʱ��blit����,����ƽ�涼������C1��ɫֵ�����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2+C1+SR1+DR1+SCr3+DCr4+M1
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��Ŀ���ͼƽ�����SCr2��ɫֵ
//@REMARK: 1������colorkeyʱ��Ҫ����ɫ����ֵ����û��ȥ��ע���������С����ɫģʽ��alpha�ͻ��ģʽ   
//@REMARK: 2��ֻ��X2��X1��ɫģʽ���в��ԣ���ʱ����������ɫģʽ���в��ԣ����Ը��ݲ��Ը����ʲ���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡԴ����ռ�P1_1����Ϣ
//@EXECUTIONFLOW:3������CSUDIOSGSetSurfaceInfo��Դ����ռ�P1_1��ColorKey����ΪC1
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ��������ɫֵΪSCr2
//@EXECUTIONFLOW:5����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr4
//@EXECUTIONFLOW:6������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M1��������colorkey����SCr3������
//@EXECUTIONFLOW:7������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:8����P2_1_SInfo��Ϣ�л�ȡ����һ�����ص����ɫֵӦ�ô���SCr2��ɫֵ
//@EXECUTIONFLOW:9������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Ckey_0007(void)
{   
	return OSG_Blit_TestCKeyForDst(
					EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,/*pixel format*/
					OSG_COLORKEY_BLACK,  /*colorkey*/
					OSG_COLOR_GREEN_32BIT,/*src color*/
					OSG_COLOR_BLUE_32BIT, /*dst color*/
					OSG_COLOR_GREEN_32BIT, /*check color*/
					CSUDI_TRUE);  /*check exist*/
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Ckey_0008
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����Դ��ͼƽ��ColorKeyΪC2ʱ��blit����,����ƽ�涼������C2��ɫֵ�����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2+C2+SR1+DR1+SCr1+DCr4+M1
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��Ŀ���ͼƽ�����SCr1��ɫֵ
//@REMARK: 1������colorkeyʱ��Ҫ����ɫ����ֵ����û��ȥ��ע���������С����ɫģʽ��alpha�ͻ��ģʽ   
//@REMARK: 2��ֻ��X2��X1��ɫģʽ���в��ԣ���ʱ����������ɫģʽ���в��ԣ����Ը��ݲ��Ը����ʲ���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡԴ����ռ�P1_1����Ϣ
//@EXECUTIONFLOW:3������CSUDIOSGSetSurfaceInfo��Դ����ռ�P1_1��ColorKey����ΪC2
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ��������ɫֵΪSCr1
//@EXECUTIONFLOW:5����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr4
//@EXECUTIONFLOW:6������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M1��
//@EXECUTIONFLOW:7������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:8����P2_1_SInfo��Ϣ�л�ȡ����һ�����ص����ɫֵӦ�ô���SCr1��ɫֵ
//@EXECUTIONFLOW:9������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Ckey_0008(void)
{   
	return OSG_Blit_TestCKeyForDst(
					EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,/*pixel format*/
					OSG_COLORKEY_WHITE,  /*colorkey*/
					OSG_COLOR_RED_32BIT,/*src color*/
					OSG_COLOR_BLUE_32BIT, /*dst color*/
					OSG_COLOR_RED_32BIT, /*check color*/
					CSUDI_TRUE);  /*check exist*/
}

//**********************************************************************************//
//******************************Begin Test Alpha Blit**************************//
//**********************************************************************************//
//**********Ŀ���ͼ�ռ�Ϊ����ռ䣬 ��ɫģʽΪX2��X1**************//

//@TESTCASEID:CSTC_OSG_IT_Blit_Alpha_0001
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:��������Դƽ��Ϊȫ��alpha��Ч��Ŀ�Ļ�ͼƽ��ȫ��alpha��Ч����µ�blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+SA1+DA1+SR3+DR3+SCr1+DCr2+M17
//@EXPECTATION:ÿһ�����õ�����ֵ
//@REMARK: ����Դȫ��alpha��Ч�����
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡԴP1_1
//@EXECUTIONFLOW:3������CSUDIOSGSetSurfaceInfo��Դ����ռ�P1_1��Alphaֵ����Ϊ0xff(uAlpha��5�Ĳ����𽥼�Сֱ��0)
//@EXECUTIONFLOW:4����ʼ������䣩Դ����ռ��������ɫֵΪ��ɫ
//@EXECUTIONFLOW:5����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪ��ɫ
//@EXECUTIONFLOW:6������CSUDIOSGBlit��P2_1,DR3,P1_1,SR3,M17����Դ��ͼƽ�����������blit��Ŀ�Ļ�ͼƽ��
//@EXECUTIONFLOW:7������CSUDIOSGSync���blit����
//@EXECUTIONFLOW:8������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���
//@EXECUTIONFLOW:9������CSUDIOSGBlit��P2_1��ͼƽ�����ɫblit���Դ�ռ�
//@EXECUTIONFLOW:10������CSUDIOSGSync���blit����
//@EXECUTIONFLOW:11��ѭ��ִ�в���3������10���۲�ƽ�����ɫ������ƽ������ĺ�ɫ�𽥱��͸��ɫ
//@EXECUTIONFLOW:12������CSUDIOSGDestroySurface���ٻ�ͼƽ��
//@EXECUTIONFLOW:13������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:14������CSUDIOSGGetSurfaceInfo��ȡԴP1_1
//@EXECUTIONFLOW:15������CSUDIOSGSetSurfaceInfo��Դ����ռ�P1_1��Alphaֵ����Ϊ0xff(uAlpha��5�Ĳ����𽥼�Сֱ��0)
//@EXECUTIONFLOW:16����ʼ������䣩Դ����ռ��������ɫֵΪ��ɫ
//@EXECUTIONFLOW:17����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪ��ɫ
//@EXECUTIONFLOW:18������CSUDIOSGBlit��P2_1,DR3,P1_1,SR3,M17����Դ��ͼƽ�����������blit��Ŀ�Ļ�ͼƽ��
//@EXECUTIONFLOW:19������CSUDIOSGSync���blit����
//@EXECUTIONFLOW:20������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���
//@EXECUTIONFLOW:21������CSUDIOSGBlit��P2_1��ͼƽ�����ɫblit���Դ�ռ�
//@EXECUTIONFLOW:22������CSUDIOSGSync���blit����
//@EXECUTIONFLOW:23��ѭ��ִ�в���15������22���۲�ƽ�����ɫ������ƽ������ĺ�ɫ�𽥱��͸��ɫ
//@EXECUTIONFLOW:24������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Alpha_0001(void)
{   	
	CSUDI_BOOL bRet = CSUDI_TRUE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	unsigned int uSrcColorRed = OSG_COLOR_RED_32BIT;
	unsigned int uDstColorGreen = OSG_COLOR_GREEN_32BIT;
	unsigned int uDstColorBlue = OSG_COLOR_BLUE_32BIT;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);

	if(ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)
	{
		uSrcColorRed = OSG_COLOR_RED_16BIT;
		uDstColorGreen = OSG_COLOR_GREEN_16BIT;
		uDstColorBlue = OSG_COLOR_BLUE_16BIT;
	}
	
	CSTCPrint("ע����Ļ����ĺ�ɫ�𽥱��͸��ɫ!!\r\n");
	CSTKWaitAnyKey();	
	bRet &= OSG_Blit_TestAlphaManual(ePixelFormat, uSrcColorRed, uDstColorGreen);
	CSTCPrint("���������Ƿ��������ֵ!!\r\n");
	CSTCPrint("Yes,������������ͬ .\r\n");
	CSTCPrint("No,����������ͬ.\r\n");
	bRet &= CSTKWaitYes();

	CSTCPrint("ע����Ļ����ĺ�ɫ�𽥱��͸��ɫ!!\r\n");
	CSTKWaitAnyKey();
	bRet = OSG_Blit_TestAlphaManual(ePixelFormat, uSrcColorRed, uDstColorBlue);
	CSTCPrint("���������Ƿ��������ֵ!!\r\n");
	CSTCPrint("Yes,������������ͬ .\r\n");
	CSTCPrint("No,����������ͬ.\r\n");
	bRet &= CSTKWaitYes();

	CSTCPrint("ע����Ļ�������ɫ�𽥱��͸��ɫ!!\r\n");
	CSTKWaitAnyKey();
	bRet &= OSG_Blit_TestAlphaManual(ePixelFormat, uDstColorBlue, uDstColorGreen);
	CSTCPrint("���������Ƿ��������ֵ!!\r\n");
	CSTCPrint("Yes,������������ͬ .\r\n");
	CSTCPrint("No,����������ͬ.\r\n");
	bRet &= CSTKWaitYes();
	
	CSTCPrint("ע����Ļ�������ɫ�𽥱��͸��ɫ!!\r\n");
	CSTKWaitAnyKey();
	bRet &= OSG_Blit_TestAlphaManual(ePixelFormat, uDstColorBlue, uSrcColorRed);
	CSTCPrint("���������Ƿ��������ֵ!!\r\n");
	CSTCPrint("Yes,������������ͬ .\r\n");
	CSTCPrint("No,����������ͬ.\r\n");
	bRet &= CSTKWaitYes();
	
	CSTCPrint("ע����Ļ�������ɫ�𽥱��͸��ɫ!!\r\n");
	CSTKWaitAnyKey();
	bRet &= OSG_Blit_TestAlphaManual(ePixelFormat, uDstColorGreen, uSrcColorRed);
	CSTCPrint("���������Ƿ��������ֵ!!\r\n");
	CSTCPrint("Yes,������������ͬ .\r\n");
	CSTCPrint("No,����������ͬ.\r\n");
	bRet &= CSTKWaitYes();	

	CSTCPrint("ע����Ļ�������ɫ�𽥱��͸��ɫ!!\r\n");
	CSTKWaitAnyKey();
	bRet &= OSG_Blit_TestAlphaManual(ePixelFormat, uDstColorGreen, uDstColorBlue);
	CSTCPrint("���������Ƿ��������ֵ!!\r\n");
	CSTCPrint("Yes,������������ͬ .\r\n");
	CSTCPrint("No,����������ͬ.\r\n");
	bRet &= CSTKWaitYes();	

	CSTK_FATAL_POINT;
	
	return bRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Alpha_0002
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������alphaģʽ�µ�blit����,��ɫֵ�ֱ�ΪSCr1��DCr2��alphaֵ�ֱ�ΪSA1��DA1
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+SA1+DA1+SR1+DR1+SCr1+DCr2+M17
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��alphaֵ��������CSTC_OSG_IT_Blit_0000�еĹ�ʽ(1)(2)(3)(4)
//@REMARK: 1������alphaʱ��Ҫ��ɫ����ֵ����û�й�ע��ɫģʽ�;�������   
//@REMARK: 2��ֻ��X2��X1��ɫģʽ���в��ԣ���ʱ����������ɫģʽ���в��ԣ����Ը��ݲ��Ը����ʲ���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡԴP1_1��Ŀ�껺��ռ�P2_1����Ϣ
//@EXECUTIONFLOW:3������CSUDIOSGSetSurfaceInfo��Դ����ռ�P1_1��Alphaֵ����ΪSA1
//@EXECUTIONFLOW:4������CSUDIOSGSetSurfaceInfo��Ŀ�껺��ռ�P2_1��Alphaֵ����ΪDA1
//@EXECUTIONFLOW:5����ʼ������䣩Դ����ռ��������ɫֵΪSCr1
//@EXECUTIONFLOW:6����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr2
//@EXECUTIONFLOW:7������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M17��
//@EXECUTIONFLOW:8������CSUDIOSGSync���blit����
//@EXECUTIONFLOW:9������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:10����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ�����Ϲ�ʽ(1)(2)(3)(4)
//@EXECUTIONFLOW:11������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Alpha_0002(void)
{   
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	unsigned int uSrcColor = OSG_COLOR_RED_32BIT;
	unsigned int uDstColor = OSG_COLOR_GREEN_32BIT;
	unsigned int uSrcAlpha = OSGUDI2_32BIT_ALPHA_1;
	unsigned int uDstAlpha = OSGUDI2_32BIT_ALPHA_1;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);

	if(ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)
	{
		uSrcColor = OSG_COLOR_RED_16BIT;
		uDstColor = OSG_COLOR_GREEN_16BIT;
		uSrcAlpha = OSGUDI2_16BIT_ALPHA_1;
		uDstAlpha = OSGUDI2_16BIT_ALPHA_1;
	}
	
	bRet = OSG_Blit_TestAlpha(
							 	ePixelFormat,/*pixel format*/
							 	uSrcAlpha,/*Src alpha*/
							 	uDstAlpha,/*dst alpha*/
							 	uSrcColor,/*src color*/
							 	uDstColor/*dst color*/
							  );

	CSTK_FATAL_POINT;

	return bRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Alpha_0003
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������alphaģʽ�µ�blit����,��ɫֵ�ֱ�ΪSCr2��DCr1��alphaֵ�ֱ�ΪSA1��DA1
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+SA1+DA1+SR1+DR1+SCr2+DCr1+M17
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��alphaֵ��������CSTC_OSG_IT_Blit_0000�еĹ�ʽ(1)(2)(3)(4)
//@REMARK: 1������alphaʱ��Ҫ��ɫ����ֵ����û�й�ע��ɫģʽ�;�������   
//@REMARK: 2��ֻ��X2��ɫģʽ���в��ԣ���ʱ����������ɫģʽ���в��ԣ����Ը��ݲ��Ը����ʲ���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡԴP1_1��Ŀ�껺��ռ�P2_1����Ϣ
//@EXECUTIONFLOW:3������CSUDIOSGSetSurfaceInfo��Դ����ռ�P1_1��Alphaֵ����ΪSA1
//@EXECUTIONFLOW:4������CSUDIOSGSetSurfaceInfo��Ŀ�껺��ռ�P2_1��Alphaֵ����ΪDA1
//@EXECUTIONFLOW:5����ʼ������䣩Դ����ռ��������ɫֵΪSCr2
//@EXECUTIONFLOW:6����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr1
//@EXECUTIONFLOW:7������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M17��
//@EXECUTIONFLOW:8������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:9����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ�����Ϲ�ʽ(1)(2)(3)(4)
//@EXECUTIONFLOW:10������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Alpha_0003(void)
{   
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	unsigned int uSrcColor = OSG_COLOR_GREEN_32BIT;
	unsigned int uDstColor = OSG_COLOR_RED_32BIT;
	unsigned int uSrcAlpha = OSGUDI2_32BIT_ALPHA_1;
	unsigned int uDstAlpha = OSGUDI2_32BIT_ALPHA_1;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);

	if(ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)
	{
		uSrcColor = OSG_COLOR_GREEN_16BIT;
		uDstColor = OSG_COLOR_RED_16BIT;
		uSrcAlpha = OSGUDI2_16BIT_ALPHA_1;
		uDstAlpha = OSGUDI2_16BIT_ALPHA_1;
	}
	
	bRet = OSG_Blit_TestAlpha(
							 	ePixelFormat,/*pixel format*/
							 	uSrcAlpha,/*Src alpha*/
							 	uDstAlpha,/*dst alpha*/
							 	uSrcColor,/*src color*/
							 	uDstColor/*dst color*/
							  );

	CSTK_FATAL_POINT;

	return bRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Alpha_0004
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������alphaģʽ�µ�blit����,��ɫֵ�ֱ�ΪSCr2��DCr2��alphaֵ�ֱ�ΪSA1��DA1
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+SA1+DA1+SR1+DR1+SCr2+DCr2+M17
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��alphaֵ��������CSTC_OSG_IT_Blit_0000�еĹ�ʽ(1)(2)(3)(4)
//@REMARK: 1������alphaʱ��Ҫ��ɫ����ֵ����û�й�ע��ɫģʽ�;�������   
//@REMARK: 2��ֻ��X2��X1��ɫģʽ���в��ԣ���ʱ����������ɫģʽ���в��ԣ����Ը��ݲ��Ը����ʲ���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡԴP1_1��Ŀ�껺��ռ�P2_1����Ϣ
//@EXECUTIONFLOW:3������CSUDIOSGSetSurfaceInfo��Դ����ռ�P1_1��Alphaֵ����ΪSA1
//@EXECUTIONFLOW:4������CSUDIOSGSetSurfaceInfo��Ŀ�껺��ռ�P2_1��Alphaֵ����ΪDA1
//@EXECUTIONFLOW:5����ʼ������䣩Դ����ռ��������ɫֵΪSCr2
//@EXECUTIONFLOW:6����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr2
//@EXECUTIONFLOW:7������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M17��
//@EXECUTIONFLOW:8������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:9����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ�����Ϲ�ʽ(1)(2)(3)(4)
//@EXECUTIONFLOW:10������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Alpha_0004(void)
{   
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	unsigned int uSrcColor = OSG_COLOR_GREEN_32BIT;
	unsigned int uDstColor = OSG_COLOR_GREEN_32BIT;
	unsigned int uSrcAlpha = OSGUDI2_32BIT_ALPHA_1;
	unsigned int uDstAlpha = OSGUDI2_32BIT_ALPHA_1;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);

	if(ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)
	{
		uSrcColor = OSG_COLOR_GREEN_16BIT;
		uDstColor = OSG_COLOR_GREEN_16BIT;
		uSrcAlpha = OSGUDI2_16BIT_ALPHA_1;
		uDstAlpha = OSGUDI2_16BIT_ALPHA_1;
	}
	
	bRet = OSG_Blit_TestAlpha(
							 	ePixelFormat,/*pixel format*/
							 	uSrcAlpha,/*Src alpha*/
							 	uDstAlpha,/*dst alpha*/
							 	uSrcColor,/*src color*/
							 	uDstColor/*dst color*/
							  );

	CSTK_FATAL_POINT;

	return bRet;	
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Alpha_0005
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������alphaģʽ�µ�blit����,��ɫֵ�ֱ�ΪSCr1��DCr1��alphaֵ�ֱ�ΪSA1��DA2
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+SA1+DA2+SR1+DR1+SCr1+DCr1+M17
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��alphaֵ��������CSTC_OSG_IT_Blit_0000�еĹ�ʽ(1)(2)(3)(4)
//@REMARK: 1������alphaʱ��Ҫ��ɫ����ֵ����û�й�ע��ɫģʽ�;�������   
//@REMARK: 2��ֻ��X2��X1��ɫģʽ���в��ԣ���ʱ����������ɫģʽ���в��ԣ����Ը��ݲ��Ը����ʲ���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡԴP1_1��Ŀ�껺��ռ�P2_1����Ϣ
//@EXECUTIONFLOW:3������CSUDIOSGSetSurfaceInfo��Դ����ռ�P1_1��Alphaֵ����ΪSA1
//@EXECUTIONFLOW:4������CSUDIOSGSetSurfaceInfo��Ŀ�껺��ռ�P2_1��Alphaֵ����ΪDA2
//@EXECUTIONFLOW:5����ʼ������䣩Դ����ռ��������ɫֵΪSCr1
//@EXECUTIONFLOW:6����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr1
//@EXECUTIONFLOW:7������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M17��
//@EXECUTIONFLOW:8������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:9����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ�����Ϲ�ʽ(1)(2)(3)(4)
//@EXECUTIONFLOW:10������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Alpha_0005(void)
{   
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	unsigned int uSrcColor = OSG_COLOR_RED_32BIT;
	unsigned int uDstColor = OSG_COLOR_RED_32BIT;
	unsigned int uSrcAlpha = OSGUDI2_32BIT_ALPHA_1;
	unsigned int uDstAlpha = OSGUDI2_32BIT_ALPHA_2;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);

	if(ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)
	{
		uSrcColor = OSG_COLOR_RED_16BIT;
		uDstColor = OSG_COLOR_RED_16BIT;
		uSrcAlpha = OSGUDI2_16BIT_ALPHA_1;
		uDstAlpha = OSGUDI2_16BIT_ALPHA_2;
	}
	
	bRet = OSG_Blit_TestAlpha(
							 	ePixelFormat,/*pixel format*/
							 	uSrcAlpha,/*Src alpha*/
							 	uDstAlpha,/*dst alpha*/
							 	uSrcColor,/*src color*/
							 	uDstColor/*dst color*/
							  );

	CSTK_FATAL_POINT;

	return bRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Alpha_0006
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������alphaģʽ�µ�blit����,��ɫֵ�ֱ�ΪSCr1��DCr2��alphaֵ�ֱ�ΪSA1��DA2
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+SA1+DA2+SR1+DR1+SCr1+DCr2+M17
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��alphaֵ��������CSTC_OSG_IT_Blit_0000�еĹ�ʽ(1)(2)(3)(4)
//@REMARK: 1������alphaʱ��Ҫ��ɫ����ֵ����û�й�ע��ɫģʽ�;�������   
//@REMARK: 2��ֻ��X2��X1��ɫģʽ���в��ԣ���ʱ����������ɫģʽ���в��ԣ����Ը��ݲ��Ը����ʲ���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡԴP1_1��Ŀ�껺��ռ�P2_1����Ϣ
//@EXECUTIONFLOW:3������CSUDIOSGSetSurfaceInfo��Դ����ռ�P1_1��Alphaֵ����ΪSA1
//@EXECUTIONFLOW:4������CSUDIOSGSetSurfaceInfo��Ŀ�껺��ռ�P2_1��Alphaֵ����ΪDA2
//@EXECUTIONFLOW:5����ʼ������䣩Դ����ռ��������ɫֵΪSCr1
//@EXECUTIONFLOW:6����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr2
//@EXECUTIONFLOW:7������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M17��
//@EXECUTIONFLOW:8������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:9����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ�����Ϲ�ʽ(1)(2)(3)(4)
//@EXECUTIONFLOW:10������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Alpha_0006(void)
{   
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	unsigned int uSrcColor = OSG_COLOR_RED_32BIT;
	unsigned int uDstColor = OSG_COLOR_GREEN_32BIT;
	unsigned int uSrcAlpha = OSGUDI2_32BIT_ALPHA_1;
	unsigned int uDstAlpha = OSGUDI2_32BIT_ALPHA_2;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);

	if(ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)
	{
		uSrcColor = OSG_COLOR_RED_16BIT;
		uDstColor = OSG_COLOR_GREEN_16BIT;
		uSrcAlpha = OSGUDI2_16BIT_ALPHA_1;
		uDstAlpha = OSGUDI2_16BIT_ALPHA_2;
	}
	
	bRet = OSG_Blit_TestAlpha(
							 	ePixelFormat,/*pixel format*/
							 	uSrcAlpha,/*Src alpha*/
							 	uDstAlpha,/*dst alpha*/
							 	uSrcColor,/*src color*/
							 	uDstColor/*dst color*/
							  );

	CSTK_FATAL_POINT;

	return bRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Alpha_0007
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������alphaģʽ�µ�blit����,��ɫֵ�ֱ�ΪSCr2��DCr1��alphaֵ�ֱ�ΪSA1��DA2
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+SA1+DA2+SR1+DR1+SCr2+DCr1+M17
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��alphaֵ��������CSTC_OSG_IT_Blit_0000�еĹ�ʽ(1)(2)(3)(4)
//@REMARK: 1������alphaʱ��Ҫ��ɫ����ֵ����û�й�ע��ɫģʽ�;�������   
//@REMARK: 2��ֻ��X2��X1��ɫģʽ���в��ԣ���ʱ����������ɫģʽ���в��ԣ����Ը��ݲ��Ը����ʲ���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡԴP1_1��Ŀ�껺��ռ�P2_1����Ϣ
//@EXECUTIONFLOW:3������CSUDIOSGSetSurfaceInfo��Դ����ռ�P1_1��Alphaֵ����ΪSA1
//@EXECUTIONFLOW:4������CSUDIOSGSetSurfaceInfo��Ŀ�껺��ռ�P2_1��Alphaֵ����ΪDA2
//@EXECUTIONFLOW:5����ʼ������䣩Դ����ռ��������ɫֵΪSCr2
//@EXECUTIONFLOW:6����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr1
//@EXECUTIONFLOW:7������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M17��
//@EXECUTIONFLOW:8������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:9����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ�����Ϲ�ʽ(1)(2)(3)(4)
//@EXECUTIONFLOW:10������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Alpha_0007(void)
{   
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	unsigned int uSrcColor = OSG_COLOR_GREEN_32BIT;
	unsigned int uDstColor = OSG_COLOR_RED_32BIT;
	unsigned int uSrcAlpha = OSGUDI2_32BIT_ALPHA_1;
	unsigned int uDstAlpha = OSGUDI2_32BIT_ALPHA_2;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);

	if(ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)
	{
		uSrcColor = OSG_COLOR_GREEN_16BIT;
		uDstColor = OSG_COLOR_RED_16BIT;
		uSrcAlpha = OSGUDI2_16BIT_ALPHA_1;
		uDstAlpha = OSGUDI2_16BIT_ALPHA_2;
	}
	
	bRet = OSG_Blit_TestAlpha(
							 	ePixelFormat,/*pixel format*/
							 	uSrcAlpha,/*Src alpha*/
							 	uDstAlpha,/*dst alpha*/
							 	uSrcColor,/*src color*/
							 	uDstColor/*dst color*/
							  );

	CSTK_FATAL_POINT;

	return bRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Alpha_0008
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������alphaģʽ�µ�blit����,��ɫֵ�ֱ�ΪSCr2��DCr2��alphaֵ�ֱ�ΪSA1��DA2
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+SA1+DA2+SR1+DR1+SCr2+DCr2+M17
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��alphaֵ��������CSTC_OSG_IT_Blit_0000�еĹ�ʽ(1)(2)(3)(4)
//@REMARK: 1������alphaʱ��Ҫ��ɫ����ֵ����û�й�ע��ɫģʽ�;�������   
//@REMARK: 2��ֻ��X2��X1��ɫģʽ���в��ԣ���ʱ����������ɫģʽ���в��ԣ����Ը��ݲ��Ը����ʲ���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡԴP1_1��Ŀ�껺��ռ�P2_1����Ϣ
//@EXECUTIONFLOW:3������CSUDIOSGSetSurfaceInfo��Դ����ռ�P1_1��Alphaֵ����ΪSA1
//@EXECUTIONFLOW:4������CSUDIOSGSetSurfaceInfo��Ŀ�껺��ռ�P2_1��Alphaֵ����ΪDA2
//@EXECUTIONFLOW:5����ʼ������䣩Դ����ռ��������ɫֵΪSCr2
//@EXECUTIONFLOW:6����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr2
//@EXECUTIONFLOW:7������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M17��
//@EXECUTIONFLOW:8������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:9����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ�����Ϲ�ʽ(1)(2)(3)(4)
//@EXECUTIONFLOW:10������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Alpha_0008(void)
{   
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	unsigned int uSrcColor = OSG_COLOR_GREEN_32BIT;
	unsigned int uDstColor = OSG_COLOR_GREEN_32BIT;
	unsigned int uSrcAlpha = OSGUDI2_32BIT_ALPHA_1;
	unsigned int uDstAlpha = OSGUDI2_32BIT_ALPHA_2;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);

	if(ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)
	{
		uSrcColor = OSG_COLOR_GREEN_16BIT;
		uDstColor = OSG_COLOR_GREEN_16BIT;
		uSrcAlpha = OSGUDI2_16BIT_ALPHA_1;
		uDstAlpha = OSGUDI2_16BIT_ALPHA_2;
	}
	
	bRet = OSG_Blit_TestAlpha(
							 	ePixelFormat,/*pixel format*/
							 	uSrcAlpha,/*Src alpha*/
							 	uDstAlpha,/*dst alpha*/
							 	uSrcColor,/*src color*/
							 	uDstColor/*dst color*/
							  );

	CSTK_FATAL_POINT;

	return bRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Alpha_0009
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������alphaģʽ�µ�blit����,��ɫֵ�ֱ�ΪSCr1��DCr1��alphaֵ�ֱ�ΪSA2��DA1
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+SA2+DA1+SR1+DR1+SCr1+DCr1+M17
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��alphaֵ��������CSTC_OSG_IT_Blit_0000�еĹ�ʽ(1)(2)(3)(4)
//@REMARK: 1������alphaʱ��Ҫ��ɫ����ֵ����û�й�ע��ɫģʽ�;�������   
//@REMARK: 2��ֻ��X2��X1��ɫģʽ���в��ԣ���ʱ����������ɫģʽ���в��ԣ����Ը��ݲ��Ը����ʲ���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡԴP1_1��Ŀ�껺��ռ�P2_1����Ϣ
//@EXECUTIONFLOW:3������CSUDIOSGSetSurfaceInfo��Դ����ռ�P1_1��Alphaֵ����ΪSA2
//@EXECUTIONFLOW:4������CSUDIOSGSetSurfaceInfo��Ŀ�껺��ռ�P2_1��Alphaֵ����ΪDA1
//@EXECUTIONFLOW:5����ʼ������䣩Դ����ռ��������ɫֵΪSCr1
//@EXECUTIONFLOW:6����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr1
//@EXECUTIONFLOW:7������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M17��
//@EXECUTIONFLOW:8������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:9����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ�����Ϲ�ʽ(1)(2)(3)(4)
//@EXECUTIONFLOW:10����P2_1_SInfo��Ϣ�л�ȡƽ���alphaֵ�����Ϲ�ʽ(1)
//@EXECUTIONFLOW:11������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Alpha_0009(void)
{   
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	unsigned int uSrcColor = OSG_COLOR_RED_32BIT;
	unsigned int uDstColor = OSG_COLOR_RED_32BIT;
	unsigned int uSrcAlpha = OSGUDI2_32BIT_ALPHA_2;
	unsigned int uDstAlpha = OSGUDI2_32BIT_ALPHA_1;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);

	if(ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)
	{
		uSrcColor = OSG_COLOR_RED_16BIT;
		uDstColor = OSG_COLOR_RED_16BIT;
		uSrcAlpha = OSGUDI2_16BIT_ALPHA_2;
		uDstAlpha = OSGUDI2_16BIT_ALPHA_1;
	}
	
	bRet = OSG_Blit_TestAlpha(
							 	ePixelFormat,/*pixel format*/
							 	uSrcAlpha,/*Src alpha*/
							 	uDstAlpha,/*dst alpha*/
							 	uSrcColor,/*src color*/
							 	uDstColor/*dst color*/
							  );

	CSTK_FATAL_POINT;

	return bRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Alpha_0010
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������alphaģʽ�µ�blit����,��ɫֵ�ֱ�ΪSCr1��DCr2��alphaֵ�ֱ�ΪSA2��DA1
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+SA2+DA1+SR1+DR1+SCr1+DCr2+M17
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��alphaֵ��������CSTC_OSG_IT_Blit_0000�еĹ�ʽ(1)(2)(3)(4)
//@REMARK: 1������alphaʱ��Ҫ��ɫ����ֵ����û�й�ע��ɫģʽ�;�������   
//@REMARK: 2��ֻ��X2��X1��ɫģʽ���в��ԣ���ʱ����������ɫģʽ���в��ԣ����Ը��ݲ��Ը����ʲ���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡԴP1_1��Ŀ�껺��ռ�P2_1����Ϣ
//@EXECUTIONFLOW:3������CSUDIOSGSetSurfaceInfo��Դ����ռ�P1_1��Alphaֵ����ΪSA2
//@EXECUTIONFLOW:4������CSUDIOSGSetSurfaceInfo��Ŀ�껺��ռ�P2_1��Alphaֵ����ΪDA1
//@EXECUTIONFLOW:5����ʼ������䣩Դ����ռ��������ɫֵΪSCr1
//@EXECUTIONFLOW:6����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr2
//@EXECUTIONFLOW:7������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M17��
//@EXECUTIONFLOW:8������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:9����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ�����Ϲ�ʽ(1)(2)(3)(4)
//@EXECUTIONFLOW:10������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Alpha_0010(void)
{   
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	unsigned int uSrcColor = OSG_COLOR_RED_32BIT;
	unsigned int uDstColor = OSG_COLOR_GREEN_32BIT;
	unsigned int uSrcAlpha = OSGUDI2_32BIT_ALPHA_2;
	unsigned int uDstAlpha = OSGUDI2_32BIT_ALPHA_1;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);

	if(ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)
	{
		uSrcColor = OSG_COLOR_RED_16BIT;
		uDstColor = OSG_COLOR_GREEN_16BIT;
		uSrcAlpha = OSGUDI2_16BIT_ALPHA_2;
		uDstAlpha = OSGUDI2_16BIT_ALPHA_1;
	}
	
	bRet = OSG_Blit_TestAlpha(
							 	ePixelFormat,/*pixel format*/
							 	uSrcAlpha,/*Src alpha*/
							 	uDstAlpha,/*dst alpha*/
							 	uSrcColor,/*src color*/
							 	uDstColor/*dst color*/
							  );

	CSTK_FATAL_POINT;

	return bRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Alpha_0011
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������alphaģʽ�µ�blit����,��ɫֵ�ֱ�ΪSCr2��DCr1��alphaֵ�ֱ�ΪSA2��DA1
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+SA2+DA1+SR1+DR1+SCr2+DCr1+M17
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��alphaֵ��������CSTC_OSG_IT_Blit_0000�еĹ�ʽ(1)(2)(3)(4)
//@REMARK: 1������alphaʱ��Ҫ��ɫ����ֵ����û�й�ע��ɫģʽ�;�������   
//@REMARK: 2��ֻ��X2��X1��ɫģʽ���в��ԣ���ʱ����������ɫģʽ���в��ԣ����Ը��ݲ��Ը����ʲ���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡԴP1_1��Ŀ�껺��ռ�P2_1����Ϣ
//@EXECUTIONFLOW:3������CSUDIOSGSetSurfaceInfo��Դ����ռ�P1_1��Alphaֵ����ΪSA2
//@EXECUTIONFLOW:4������CSUDIOSGSetSurfaceInfo��Ŀ�껺��ռ�P2_1��Alphaֵ����ΪDA1
//@EXECUTIONFLOW:5����ʼ������䣩Դ����ռ��������ɫֵΪSCr2
//@EXECUTIONFLOW:6����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr1
//@EXECUTIONFLOW:7������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M17��
//@EXECUTIONFLOW:8������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:9����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ�����Ϲ�ʽ(1)(2)(3)(4)
//@EXECUTIONFLOW:10������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Alpha_0011(void)
{   
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	unsigned int uSrcColor = OSG_COLOR_GREEN_32BIT;
	unsigned int uDstColor = OSG_COLOR_RED_32BIT;
	unsigned int uSrcAlpha = OSGUDI2_32BIT_ALPHA_2;
	unsigned int uDstAlpha = OSGUDI2_32BIT_ALPHA_1;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);

	if(ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)
	{
		uSrcColor = OSG_COLOR_GREEN_16BIT;
		uDstColor = OSG_COLOR_RED_16BIT;
		uSrcAlpha = OSGUDI2_16BIT_ALPHA_2;
		uDstAlpha = OSGUDI2_16BIT_ALPHA_1;
	}
	
	bRet = OSG_Blit_TestAlpha(
							 	ePixelFormat,/*pixel format*/
							 	uSrcAlpha,/*Src alpha*/
							 	uDstAlpha,/*dst alpha*/
							 	uSrcColor,/*src color*/
							 	uDstColor/*dst color*/
							  );

	CSTK_FATAL_POINT;

	return bRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Alpha_0012
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������alphaģʽ�µ�blit����,��ɫֵ�ֱ�ΪSCr2��DCr2��alphaֵ�ֱ�ΪSA2��DA1
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+SA2+DA1+SR1+DR1+SCr2+DCr2+M17
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��alphaֵ��������CSTC_OSG_IT_Blit_0000�еĹ�ʽ(1)(2)(3)(4)
//@REMARK: 1������alphaʱ��Ҫ��ɫ����ֵ����û�й�ע��ɫģʽ�;�������   
//@REMARK: 2��ֻ��X2��X1��ɫģʽ���в��ԣ���ʱ����������ɫģʽ���в��ԣ����Ը��ݲ��Ը����ʲ���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡԴP1_1��Ŀ�껺��ռ�P2_1����Ϣ
//@EXECUTIONFLOW:3������CSUDIOSGSetSurfaceInfo��Դ����ռ�P1_1��Alphaֵ����ΪSA2
//@EXECUTIONFLOW:4������CSUDIOSGSetSurfaceInfo��Ŀ�껺��ռ�P2_1��Alphaֵ����ΪDA1
//@EXECUTIONFLOW:5����ʼ������䣩Դ����ռ��������ɫֵΪSCr2
//@EXECUTIONFLOW:6����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr2
//@EXECUTIONFLOW:7������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M17��
//@EXECUTIONFLOW:8������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:9����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ�����Ϲ�ʽ(1)(2)(3)(4)
//@EXECUTIONFLOW:10������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Alpha_0012(void)
{   
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	unsigned int uSrcColor = OSG_COLOR_GREEN_32BIT;
	unsigned int uDstColor = OSG_COLOR_GREEN_32BIT;
	unsigned int uSrcAlpha = OSGUDI2_32BIT_ALPHA_2;
	unsigned int uDstAlpha = OSGUDI2_32BIT_ALPHA_1;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);

	if(ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)
	{
		uSrcColor = OSG_COLOR_GREEN_16BIT;
		uDstColor = OSG_COLOR_GREEN_16BIT;
		uSrcAlpha = OSGUDI2_16BIT_ALPHA_2;
		uDstAlpha = OSGUDI2_16BIT_ALPHA_1;
	}
	
	bRet = OSG_Blit_TestAlpha(
							 	ePixelFormat,/*pixel format*/
							 	uSrcAlpha,/*Src alpha*/
							 	uDstAlpha,/*dst alpha*/
							 	uSrcColor,/*src color*/
							 	uDstColor/*dst color*/
							  );

	CSTK_FATAL_POINT;

	return bRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Alpha_0013
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������alphaģʽ�µ�blit����,��ɫֵ�ֱ�ΪSCr1��DCr1��alphaֵ�ֱ�ΪSA2��DA2
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+SA2+DA2+SR1+DR1+SCr1+DCr1+M17
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��alphaֵ��������CSTC_OSG_IT_Blit_0000�еĹ�ʽ(1)(2)(3)(4)
//@REMARK: 1������alphaʱ��Ҫ��ɫ����ֵ����û�й�ע��ɫģʽ�;�������   
//@REMARK: 2��ֻ��X2��X1��ɫģʽ���в��ԣ���ʱ����������ɫģʽ���в��ԣ����Ը��ݲ��Ը����ʲ���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡԴP1_1��Ŀ�껺��ռ�P2_1����Ϣ
//@EXECUTIONFLOW:3������CSUDIOSGSetSurfaceInfo��Դ����ռ�P1_1��Alphaֵ����ΪSA2
//@EXECUTIONFLOW:4������CSUDIOSGSetSurfaceInfo��Ŀ�껺��ռ�P2_1��Alphaֵ����ΪDA2
//@EXECUTIONFLOW:5����ʼ������䣩Դ����ռ��������ɫֵΪSCr1
//@EXECUTIONFLOW:6����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr1
//@EXECUTIONFLOW:7������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M17��
//@EXECUTIONFLOW:8������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:9����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ�����Ϲ�ʽ(1)(2)(3)(4)
//@EXECUTIONFLOW:10������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Alpha_0013(void)
{   
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	unsigned int uSrcColor = OSG_COLOR_RED_32BIT;
	unsigned int uDstColor = OSG_COLOR_RED_32BIT;
	unsigned int uSrcAlpha = OSGUDI2_32BIT_ALPHA_2;
	unsigned int uDstAlpha = OSGUDI2_32BIT_ALPHA_2;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);

	if(ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)
	{
		uSrcColor = OSG_COLOR_RED_16BIT;
		uDstColor = OSG_COLOR_RED_16BIT;
		uSrcAlpha = OSGUDI2_16BIT_ALPHA_2;
		uDstAlpha = OSGUDI2_16BIT_ALPHA_2;
	}
	
	bRet = OSG_Blit_TestAlpha(
							 	ePixelFormat,/*pixel format*/
							 	uSrcAlpha,/*Src alpha*/
							 	uDstAlpha,/*dst alpha*/
							 	uSrcColor,/*src color*/
							 	uDstColor/*dst color*/
							  );

	CSTK_FATAL_POINT;

	return bRet;	
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Alpha_0014
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������alphaģʽ�µ�blit����,��ɫֵ�ֱ�ΪSCr1��DCr2��alphaֵ�ֱ�ΪSA2��DA2
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+SA2+DA2+SR1+DR1+SCr1+DCr2+M17
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��alphaֵ��������CSTC_OSG_IT_Blit_0000�еĹ�ʽ(1)(2)(3)(4)
//@REMARK: 1������alphaʱ��Ҫ��ɫ����ֵ����û�й�ע��ɫģʽ�;�������   
//@REMARK: 2��ֻ��X2��X1��ɫģʽ���в��ԣ���ʱ����������ɫģʽ���в��ԣ����Ը��ݲ��Ը����ʲ���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡԴP1_1��Ŀ�껺��ռ�P2_1����Ϣ
//@EXECUTIONFLOW:3������CSUDIOSGSetSurfaceInfo��Դ����ռ�P1_1��Alphaֵ����ΪSA2
//@EXECUTIONFLOW:4������CSUDIOSGSetSurfaceInfo��Ŀ�껺��ռ�P2_1��Alphaֵ����ΪDA2
//@EXECUTIONFLOW:5����ʼ������䣩Դ����ռ��������ɫֵΪSCr1
//@EXECUTIONFLOW:6����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr2
//@EXECUTIONFLOW:7������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M17��
//@EXECUTIONFLOW:8������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:9����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ�����Ϲ�ʽ(1)(2)(3)(4)
//@EXECUTIONFLOW:10������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Alpha_0014(void)
{   
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	unsigned int uSrcColor = OSG_COLOR_RED_32BIT;
	unsigned int uDstColor = OSG_COLOR_GREEN_32BIT;
	unsigned int uSrcAlpha = OSGUDI2_32BIT_ALPHA_2;
	unsigned int uDstAlpha = OSGUDI2_32BIT_ALPHA_2;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);

	if(ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)
	{
		uSrcColor = OSG_COLOR_RED_16BIT;
		uDstColor = OSG_COLOR_GREEN_16BIT;
		uSrcAlpha = OSGUDI2_16BIT_ALPHA_2;
		uDstAlpha = OSGUDI2_16BIT_ALPHA_2;
	}
	
	bRet = OSG_Blit_TestAlpha(
							 	ePixelFormat,/*pixel format*/
							 	uSrcAlpha,/*Src alpha*/
							 	uDstAlpha,/*dst alpha*/
							 	uSrcColor,/*src color*/
							 	uDstColor/*dst color*/
							  );

	CSTK_FATAL_POINT;

	return bRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Alpha_0015
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������alphaģʽ�µ�blit����,��ɫֵ�ֱ�ΪSCr2��DCr1��alphaֵ�ֱ�ΪSA2��DA2
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+SA2+DA2+SR1+DR1+SCr2+DCr1+M17
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��alphaֵ��������CSTC_OSG_IT_Blit_0000�еĹ�ʽ(1)(2)(3)(4)
//@REMARK: 1������alphaʱ��Ҫ��ɫ����ֵ����û�й�ע��ɫģʽ�;�������   
//@REMARK: 2��ֻ��X2��X1��ɫģʽ���в��ԣ���ʱ����������ɫģʽ���в��ԣ����Ը��ݲ��Ը����ʲ���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡԴP1_1��Ŀ�껺��ռ�P2_1����Ϣ
//@EXECUTIONFLOW:3������CSUDIOSGSetSurfaceInfo��Դ����ռ�P1_1��Alphaֵ����ΪSA2
//@EXECUTIONFLOW:4������CSUDIOSGSetSurfaceInfo��Ŀ�껺��ռ�P2_1��Alphaֵ����ΪDA2
//@EXECUTIONFLOW:5����ʼ������䣩Դ����ռ��������ɫֵΪSCr2
//@EXECUTIONFLOW:6����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr1
//@EXECUTIONFLOW:7������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M17��
//@EXECUTIONFLOW:8������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:9����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ�����Ϲ�ʽ(1)(2)(3)(4)
//@EXECUTIONFLOW:10����P2_1_SInfo��Ϣ�л�ȡƽ���alphaֵ�����Ϲ�ʽ(1)
//@EXECUTIONFLOW:11������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Alpha_0015(void)
{   
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	unsigned int uSrcColor = OSG_COLOR_GREEN_32BIT;
	unsigned int uDstColor = OSG_COLOR_RED_32BIT;
	unsigned int uSrcAlpha = OSGUDI2_32BIT_ALPHA_2;
	unsigned int uDstAlpha = OSGUDI2_32BIT_ALPHA_2;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);

	if(ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)
	{
		uSrcColor = OSG_COLOR_GREEN_16BIT;
		uDstColor = OSG_COLOR_RED_16BIT;
		uSrcAlpha = OSGUDI2_16BIT_ALPHA_2;
		uDstAlpha = OSGUDI2_16BIT_ALPHA_2;
	}
	
	bRet = OSG_Blit_TestAlpha(
							 	ePixelFormat,/*pixel format*/
							 	uSrcAlpha,/*Src alpha*/
							 	uDstAlpha,/*dst alpha*/
							 	uSrcColor,/*src color*/
							 	uDstColor/*dst color*/
							  );

	CSTK_FATAL_POINT;

	return bRet;	
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Alpha_0016
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������alphaģʽ�µ�blit����,��ɫֵ�ֱ�ΪSCr2��DCr2��alphaֵ�ֱ�ΪSA2��DA2
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+SA2+DA2+SR1+DR1+SCr2+DCr2+M17
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��alphaֵ��������CSTC_OSG_IT_Blit_0000�еĹ�ʽ(1)(2)(3)(4)
//@REMARK: 1������alphaʱ��Ҫ��ɫ����ֵ����û�й�ע��ɫģʽ�;�������   
//@REMARK: 2��ֻ��X2��X1��ɫģʽ���в��ԣ���ʱ����������ɫģʽ���в��ԣ����Ը��ݲ��Ը����ʲ���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡԴP1_1��Ŀ�껺��ռ�P2_1����Ϣ
//@EXECUTIONFLOW:3������CSUDIOSGSetSurfaceInfo��Դ����ռ�P1_1��Alphaֵ����ΪSA2
//@EXECUTIONFLOW:4������CSUDIOSGSetSurfaceInfo��Ŀ�껺��ռ�P2_1��Alphaֵ����ΪDA2
//@EXECUTIONFLOW:5����ʼ������䣩Դ����ռ��������ɫֵΪSCr2
//@EXECUTIONFLOW:6����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr2
//@EXECUTIONFLOW:7������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M17��
//@EXECUTIONFLOW:8������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:9����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ�����Ϲ�ʽ(1)(2)(3)(4)
//@EXECUTIONFLOW:10������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Alpha_0016(void)
{   
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	unsigned int uSrcColor = OSG_COLOR_GREEN_32BIT;
	unsigned int uDstColor = OSG_COLOR_GREEN_32BIT;
	unsigned int uSrcAlpha = OSGUDI2_32BIT_ALPHA_2;
	unsigned int uDstAlpha = OSGUDI2_32BIT_ALPHA_1;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);

	if(ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)
	{
		uSrcColor = OSG_COLOR_GREEN_16BIT;
		uDstColor = OSG_COLOR_GREEN_16BIT;
		uSrcAlpha = OSGUDI2_16BIT_ALPHA_2;
		uDstAlpha = OSGUDI2_16BIT_ALPHA_1;
	}
	
	bRet = OSG_Blit_TestAlpha(
							 	ePixelFormat,/*pixel format*/
							 	uSrcAlpha,/*Src alpha*/
							 	uDstAlpha,/*dst alpha*/
							 	uSrcColor,/*src color*/
							 	uDstColor/*dst color*/
							  );

	CSTK_FATAL_POINT;

	return bRet;
}

//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������alphaģʽ�µ�blit����,��ɫֵΪcs_osgcolordata.c�ļ��е�ֵ��alphaֵ�ֱ�ΪSA1��DA1
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2
//@INPUT: P1_1+P2_1+X2/X1+SA1+DA1+SR1+DR1+SCr5+DCr5+M17
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��alphaֵ��������CSTC_OSG_IT_Blit_0000�еĹ�ʽ(1)(2)(3)(4)
//@REMARK: 1������alphaʱ��Ҫ��ɫ����ֵ����û�й�ע��ɫģʽ�;�������   
//@REMARK: 2��ֻ��X2��X1��ɫģʽ���в��ԣ���ʱ����������ɫģʽ���в��ԣ����Ը��ݲ��Ը����ʲ���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡԴP1_1��Ŀ�껺��ռ�P2_1����Ϣ
//@EXECUTIONFLOW:3������CSUDIOSGSetSurfaceInfo��Դ����ռ�P1_1��Alphaֵ����ΪSA1
//@EXECUTIONFLOW:4������CSUDIOSGSetSurfaceInfo��Ŀ�껺��ռ�P2_1��Alphaֵ����ΪDA1
//@EXECUTIONFLOW:5����ʼ������䣩Դ����ռ��������ɫֵΪSCr5
//@EXECUTIONFLOW:6����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr5
//@EXECUTIONFLOW:7������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M17��
//@EXECUTIONFLOW:8������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:9����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ�����Ϲ�ʽ(1)(2)(3)(4)
//@EXECUTIONFLOW:10������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Alpha_0017(void)
{   
	return OSG_Blit_TestAlphaWithDataColor(
							 	EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,/*pixel format*/
							 	OSGUDI2_32BIT_ALPHA_1,/*Src alpha*/
							 	OSGUDI2_32BIT_ALPHA_1,/*dst alpha*/
							 	bmp32data_h,/*src color*/
							 	bmp32data_h,/*dst color*/
							 	sizeof(bmp32data_h)
							  );
}


//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������alphaģʽ�µ�blit����,Դ����Ϊ��coship������͸��Ч��ͼƬ��Ŀ����Ϊ͸��Ч��ͼƬ(û��ȫ��͸����)
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2
//@INPUT: P1_1+P2_1+X2/X1+SA1+DA1+SR1+DR1
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��alphaֵ��������CSTC_OSG_IT_Blit_0000�еĹ�ʽ(1)(2)(3)(4)
//@REMARK: 1������alphaʱ��Ҫ��ɫ����ֵ����û�й�ע��ɫģʽ�;�������   
//@REMARK: 2��ֻ��X2��X1��ɫģʽ���в��ԣ���ʱ����������ɫģʽ���в��ԣ����Ը��ݲ��Ը����ʲ���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡԴP1_1��Ŀ�껺��ռ�P2_1����Ϣ
//@EXECUTIONFLOW:3������CSUDIOSGSetSurfaceInfo��Դ����ռ�P1_1��Alphaֵ����ΪSA1
//@EXECUTIONFLOW:4������CSUDIOSGSetSurfaceInfo��Ŀ�껺��ռ�P2_1��Alphaֵ����ΪDA1
//@EXECUTIONFLOW:5����ʼ������䣩Դ����ռ����������Ϊcoship��������
//@EXECUTIONFLOW:6����ʼ������䣩Ŀ�껺��ռ����������Ϊ��ɫ����ͼƬ��
//@EXECUTIONFLOW:7������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1��
//@EXECUTIONFLOW:8������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:9����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ�����Ϲ�ʽ(1)(2)(3)(4)
//@EXECUTIONFLOW:10������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Alpha_0018(void)
{  
	return OSG_Blit_TestAlphaWithDataColorEx(
							 	EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,/*pixel format*/
							 	OSGUDI2_32BIT_ALPHA_2,/*Src alpha*/
							 	OSGUDI2_32BIT_ALPHA_2,/*dst alpha*/
							 	coshipportal_h,/*src color*/
							 	barportal_h,/*dst color*/
							 	sizeof(coshipportal_h)
							  );
	
}

//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������alphaģʽ�µ�blit����,Դ����Ϊ��coship������͸��Ч��ͼƬ��Ŀ����Ϊ͸��Ч��ͼƬ(Դ��ȫ��͸����)
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2
//@INPUT: P1_1+P2_1+X2+SA1+DA1+SR1+DR1
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��alphaֵ��������CSTC_OSG_IT_Blit_0000�еĹ�ʽ(1)(2)(3)(4)
//@REMARK: 1������alphaʱ��Ҫ��ɫ����ֵ����û�й�ע��ɫģʽ�;�������   
//@REMARK: 2��ֻ��X2��ɫģʽ���в��ԣ���ʱ����������ɫģʽ���в��ԣ����Ը��ݲ��Ը����ʲ���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡԴP1_1��Ŀ�껺��ռ�P2_1����Ϣ
//@EXECUTIONFLOW:3������CSUDIOSGSetSurfaceInfo��Դ����ռ�P1_1��Alphaֵ����ΪSA1
//@EXECUTIONFLOW:4������CSUDIOSGSetSurfaceInfo��Ŀ�껺��ռ�P2_1��Alphaֵ����ΪDA1
//@EXECUTIONFLOW:5����ʼ������䣩Դ����ռ����������Ϊcoship��������
//@EXECUTIONFLOW:6����ʼ������䣩Ŀ�껺��ռ����������Ϊ��ɫ����ͼƬ��
//@EXECUTIONFLOW:7������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1��
//@EXECUTIONFLOW:8������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:9����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ�����Ϲ�ʽ(1)(2)(3)(4)
//@EXECUTIONFLOW:10������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Alpha_0019(void)
{  
	return OSG_Blit_TestAlphaWithDataColorEx(
							 	EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,/*pixel format*/
							 	OSGUDI2_32BIT_ALPHA_1,/*Src alpha*/
							 	OSGUDI2_32BIT_ALPHA_2,/*dst alpha*/
							 	coshipportal_h,/*src color*/
							 	barportal_h,/*dst color*/
							 	sizeof(coshipportal_h)
							  );
}

//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������alphaģʽ�µ�blit����,Դ����Ϊ��coship������͸��Ч��ͼƬ��Ŀ����Ϊ͸��Ч��ͼƬ(Ŀ����ȫ��͸����)
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2
//@INPUT: P1_1+P2_1+X2/X1+SA1+DA1+SR1+DR1
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��alphaֵ��������CSTC_OSG_IT_Blit_0000�еĹ�ʽ(1)(2)(3)(4)
//@REMARK: 1������alphaʱ��Ҫ��ɫ����ֵ����û�й�ע��ɫģʽ�;�������   
//@REMARK: 2��ֻ��X2��X1��ɫģʽ���в��ԣ���ʱ����������ɫģʽ���в��ԣ����Ը��ݲ��Ը����ʲ���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡԴP1_1��Ŀ�껺��ռ�P2_1����Ϣ
//@EXECUTIONFLOW:3������CSUDIOSGSetSurfaceInfo��Դ����ռ�P1_1��Alphaֵ����ΪSA1
//@EXECUTIONFLOW:4������CSUDIOSGSetSurfaceInfo��Ŀ�껺��ռ�P2_1��Alphaֵ����ΪDA1
//@EXECUTIONFLOW:5����ʼ������䣩Դ����ռ����������Ϊcoship��������
//@EXECUTIONFLOW:6����ʼ������䣩Ŀ�껺��ռ����������Ϊ��ɫ����ͼƬ��
//@EXECUTIONFLOW:7������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1��
//@EXECUTIONFLOW:8������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:9����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ�����Ϲ�ʽ(1)(2)(3)(4)
//@EXECUTIONFLOW:10������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Alpha_0020(void)
{  
	return OSG_Blit_TestAlphaWithDataColorEx(
							 	EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,/*pixel format*/
							 	OSGUDI2_32BIT_ALPHA_2,/*Src alpha*/
							 	OSGUDI2_32BIT_ALPHA_1,/*dst alpha*/
							 	coshipportal_h,/*src color*/
							 	barportal_h,/*dst color*/
							 	sizeof(coshipportal_h)
							  );
}

//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������alphaģʽ�µ�blit����,Դ����Ϊ��coship������͸��Ч��ͼƬ��Ŀ����Ϊ͸��Ч��ͼƬ(Դ��Ŀ�ľ���ȫ��͸����)
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2
//@INPUT: P1_1+P2_1+X2/X1+SA1+DA1+SR1+DR1
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��alphaֵ��������CSTC_OSG_IT_Blit_0000�еĹ�ʽ(1)(2)(3)(4)
//@REMARK: 1������alphaʱ��Ҫ��ɫ����ֵ����û�й�ע��ɫģʽ�;�������   
//@REMARK: 2��ֻ��X2��X1��ɫģʽ���в��ԣ���ʱ����������ɫģʽ���в��ԣ����Ը��ݲ��Ը����ʲ���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡԴP1_1��Ŀ�껺��ռ�P2_1����Ϣ
//@EXECUTIONFLOW:3������CSUDIOSGSetSurfaceInfo��Դ����ռ�P1_1��Alphaֵ����ΪSA1
//@EXECUTIONFLOW:4������CSUDIOSGSetSurfaceInfo��Ŀ�껺��ռ�P2_1��Alphaֵ����ΪDA1
//@EXECUTIONFLOW:5����ʼ������䣩Դ����ռ����������Ϊcoship��������
//@EXECUTIONFLOW:6����ʼ������䣩Ŀ�껺��ռ����������Ϊ��ɫ����ͼƬ��
//@EXECUTIONFLOW:7������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1��
//@EXECUTIONFLOW:8������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:9����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ�����Ϲ�ʽ(1)(2)(3)(4)
//@EXECUTIONFLOW:10������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Alpha_0021(void)
{  
	return OSG_Blit_TestAlphaWithDataColorEx(
							 	EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,/*pixel format*/
							 	0x6F,/*Src alpha*/
							 	OSGUDI2_32BIT_ALPHA_1,/*dst alpha*/
							 	coshipportal_h,/*src color*/
							 	barportal_h,/*dst color*/
							 	sizeof(coshipportal_h)
							  );
	
}

//**********************************************************************************//
//******************************Begin Test OpMode Blit*****************************//
//**********************************************************************************//
//**************************Ӧ����Ըò�����������ɫֵ*******************************//

//@TESTCASEID:CSTC_OSG_IT_Blit_OpMode_0001
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������EM_UDIOSG_OPMODE_COPY���ģʽ�µ�blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+SR1+DR1+SCr1+DCr4+M1
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��Դ��ͼƽ�����ɫֵһ��
//@REMARK: 1��û������������������в���   
//@REMARK: 2��û����Բ�ͬ��ɫģʽ���в���
//@REMARK: 3���˴����ٶ�alphaģʽ���в���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ��������ɫֵΪSCr1
//@EXECUTIONFLOW:3����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr4
//@EXECUTIONFLOW:4������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M1��
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:6����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ��Ӧ��ΪSCr1
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_OpMode_0001(void)
{   
	return OSG_Blit_TestOpMode(
								EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,
								OSG_COLOR_RED_32BIT,
								OSG_COLOR_BLUE_32BIT,
								EM_UDIOSG_OPMODE_COPY);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_OpMode_0002
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������EM_UDIOSG_OPMODE_XOR���ģʽ�µ�blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+SR1+DR1+SCr1+DCr4+M2
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��Դ��ͼƽ�����ɫֵ����XOR���
//@REMARK: 1��û������������������в���   
//@REMARK: 2��û����Բ�ͬ��ɫģʽ���в���
//@REMARK: 3���˴����ٶ�alphaģʽ���в���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ��������ɫֵΪSCr1
//@EXECUTIONFLOW:3����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr4
//@EXECUTIONFLOW:4������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M2��
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:6����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ��Ӧ��ΪSCr1^DCr4
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_OpMode_0002(void)
{   
	return OSG_Blit_TestOpMode(
								EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,
								OSG_COLOR_RED_32BIT,
								OSG_COLOR_BLUE_32BIT,
								EM_UDIOSG_OPMODE_XOR);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_OpMode_0003
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������EM_UDIOSG_OPMODE_OR���ģʽ�µ�blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+SR1+DR1+SCr1+DCr4+M3
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��Դ��ͼƽ�����ɫֵ����OR���
//@REMARK: 1��û������������������в���   
//@REMARK: 2��û����Բ�ͬ��ɫģʽ���в���
//@REMARK: 3���˴����ٶ�alphaģʽ���в���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ��������ɫֵΪSCr1
//@EXECUTIONFLOW:3����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr4
//@EXECUTIONFLOW:4������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M3��
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:6����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ��Ӧ��ΪSCr1|DCr4
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_OpMode_0003(void)
{   
	return OSG_Blit_TestOpMode(
								EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,
								OSG_COLOR_RED_32BIT,
								OSG_COLOR_BLUE_32BIT,
								EM_UDIOSG_OPMODE_OR);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_OpMode_0004
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������EM_UDIOSG_OPMODE_AND���ģʽ�µ�blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+SR1+DR1+SCr1+DCr4+M4
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��Դ��ͼƽ�����ɫֵ����AND���
//@REMARK: 1��û������������������в���   
//@REMARK: 2��û����Բ�ͬ��ɫģʽ���в���
//@REMARK: 3���˴����ٶ�alphaģʽ���в���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ��������ɫֵΪSCr1
//@EXECUTIONFLOW:3����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr4
//@EXECUTIONFLOW:4������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M4��
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:6����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ��Ӧ��ΪSCr1&DCr4
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_OpMode_0004(void)
{   
	return OSG_Blit_TestOpMode(
								EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,
								OSG_COLOR_RED_32BIT,
								OSG_COLOR_BLUE_32BIT,
								EM_UDIOSG_OPMODE_AND);
}
 
//@TESTCASEID:CSTC_OSG_IT_Blit_OpMode_0005
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������EM_UDIOSG_OPMODE_CLEAR���ģʽ�µ�blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+SR1+DR1+SCr1+DCr4+M5
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��Դ��ͼƽ�����ɫֵ����CLEAR���
//@REMARK: 1��û������������������в���   
//@REMARK: 2��û����Բ�ͬ��ɫģʽ���в���
//@REMARK: 3���˴����ٶ�alphaģʽ���в���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ��������ɫֵΪSCr1
//@EXECUTIONFLOW:3����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr4
//@EXECUTIONFLOW:4������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M4��
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:6����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ��Ӧ��Ϊ0
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_OpMode_0005(void)
{   
	return OSG_Blit_TestOpMode(
								EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,
								OSG_COLOR_RED_32BIT,
								OSG_COLOR_BLUE_32BIT,
								EM_UDIOSG_OPMODE_CLEAR);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_OpMode_0006
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������EM_UDIOSG_OPMODE_SET���ģʽ�µ�blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+SR1+DR1+SCr1+DCr4+M6
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��Դ��ͼƽ�����ɫֵ����SET���
//@REMARK: 1��û������������������в���   
//@REMARK: 2��û����Բ�ͬ��ɫģʽ���в���
//@REMARK: 3���˴����ٶ�alphaģʽ���в���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ��������ɫֵΪSCr1
//@EXECUTIONFLOW:3����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr4
//@EXECUTIONFLOW:4������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M6��
//@EXECUTIONFLOW:5������CSUDIOSGSync���blit����
//@EXECUTIONFLOW:6������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:7����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ��Ӧ��Ϊ0xffffffff
//@EXECUTIONFLOW:8������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_OpMode_0006(void)
{   
	return OSG_Blit_TestOpMode(
								EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,
								OSG_COLOR_RED_32BIT,
								OSG_COLOR_BLUE_32BIT,
								EM_UDIOSG_OPMODE_SET);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_OpMode_0007
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������EM_UDIOSG_OPMODE_EQUIV���ģʽ�µ�blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+SR1+DR1+SCr1+DCr4+M7
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��Դ��ͼƽ�����ɫֵ����EQUIV���
//@REMARK: 1��û������������������в���   
//@REMARK: 2��û����Բ�ͬ��ɫģʽ���в���
//@REMARK: 3���˴����ٶ�alphaģʽ���в���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ��������ɫֵΪSCr1
//@EXECUTIONFLOW:3����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr4
//@EXECUTIONFLOW:4������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M7��
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:6����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ��Ӧ��Ϊ~��SCrr^DCr4)
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_OpMode_0007(void)
{   
	return OSG_Blit_TestOpMode(
								EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,
								OSG_COLOR_RED_32BIT,
								OSG_COLOR_BLUE_32BIT,
								EM_UDIOSG_OPMODE_EQUIV);	
}

//@TESTCASEID:CSTC_OSG_IT_Blit_OpMode_0008
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������EM_UDIOSG_OPMODE_NOR���ģʽ�µ�blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+SR1+DR1+SCr1+DCr4+M8
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��Դ��ͼƽ�����ɫֵ����NOR���
//@REMARK: 1��û������������������в���   
//@REMARK: 2��û����Բ�ͬ��ɫģʽ���в���
//@REMARK: 3���˴����ٶ�alphaģʽ���в���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ��������ɫֵΪSCr1
//@EXECUTIONFLOW:3����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr4
//@EXECUTIONFLOW:4������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M8��
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:6����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ��Ӧ��Ϊ~��SCr1|DCr4��
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_OpMode_0008(void)
{   
	return OSG_Blit_TestOpMode(
								EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,
								OSG_COLOR_RED_32BIT,
								OSG_COLOR_BLUE_32BIT,
								EM_UDIOSG_OPMODE_NOR);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_OpMode_0009
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������EM_UDIOSG_OPMODE_NAND���ģʽ�µ�blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+SR1+DR1+SCr1+DCr4+M9
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��Դ��ͼƽ�����ɫֵ����NAND���
//@REMARK: 1��û������������������в���   
//@REMARK: 2��û����Բ�ͬ��ɫģʽ���в���
//@REMARK: 3���˴����ٶ�alphaģʽ���в���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ��������ɫֵΪSCr1
//@EXECUTIONFLOW:3����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr4
//@EXECUTIONFLOW:4������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M9��
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:6����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ��Ӧ��Ϊ~��SCr1&DCr4��
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_OpMode_0009(void)
{   
	return OSG_Blit_TestOpMode(
								EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,
								OSG_COLOR_RED_32BIT,
								OSG_COLOR_BLUE_32BIT,
								EM_UDIOSG_OPMODE_NAND);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_OpMode_0010
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������EM_UDIOSG_OPMODE_INVERT���ģʽ�µ�blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+SR1+DR1+SCr1+DCr4+M10
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��Դ��ͼƽ�����ɫֵ����INVERT���
//@REMARK: 1��û������������������в���   
//@REMARK: 2��û����Բ�ͬ��ɫģʽ���в���
//@REMARK: 3���˴����ٶ�alphaģʽ���в���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ��������ɫֵΪSCr1
//@EXECUTIONFLOW:3����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr4
//@EXECUTIONFLOW:4������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M10��
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:6����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ��Ӧ��Ϊ~DCr4
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_OpMode_0010(void)
{   
	return OSG_Blit_TestOpMode(
								EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,
								OSG_COLOR_RED_32BIT,
								OSG_COLOR_BLUE_32BIT,
								EM_UDIOSG_OPMODE_INVERT);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_OpMode_0011
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������EM_UDIOSG_OPMODE_COPYINVERTED���ģʽ�µ�blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+SR1+DR1+SCr1+DCr4+M11
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��Դ��ͼƽ�����ɫֵ����COPYINVERTED���
//@REMARK: 1��û������������������в���   
//@REMARK: 2��û����Բ�ͬ��ɫģʽ���в���
//@REMARK: 3���˴����ٶ�alphaģʽ���в���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ��������ɫֵΪSCr1
//@EXECUTIONFLOW:3����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr4
//@EXECUTIONFLOW:4������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M11��
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:6����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ��Ӧ��Ϊ~SCr1
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_OpMode_0011(void)
{   
	return OSG_Blit_TestOpMode(
								EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,
								OSG_COLOR_RED_32BIT,
								OSG_COLOR_BLUE_32BIT,
								EM_UDIOSG_OPMODE_COPYINVERTED);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_OpMode_0012
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������EM_UDIOSG_OPMODE_ORINVERTED���ģʽ�µ�blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+SR1+DR1+SCr1+DCr4+M12
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��Դ��ͼƽ�����ɫֵ����ORINVERTED���
//@REMARK: 1��û������������������в���   
//@REMARK: 2��û����Բ�ͬ��ɫģʽ���в���
//@REMARK: 3���˴����ٶ�alphaģʽ���в���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ��������ɫֵΪSCr1
//@EXECUTIONFLOW:3����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr4
//@EXECUTIONFLOW:4������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M12��
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:6����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ��Ӧ��Ϊ~SCr1|DCr4
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_OpMode_0012(void)
{   
	return OSG_Blit_TestOpMode(
								EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,
								OSG_COLOR_RED_32BIT,
								OSG_COLOR_BLUE_32BIT,
								EM_UDIOSG_OPMODE_ORINVERTED);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_OpMode_0013
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������EM_UDIOSG_OPMODE_ANDINVERTED���ģʽ�µ�blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+SR1+DR1+SCr1+DCr4+M13
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��Դ��ͼƽ�����ɫֵ����ANDINVERTED���
//@REMARK: 1��û������������������в���   
//@REMARK: 2��û����Բ�ͬ��ɫģʽ���в���
//@REMARK: 3���˴����ٶ�alphaģʽ���в���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ��������ɫֵΪSCr1
//@EXECUTIONFLOW:3����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr4
//@EXECUTIONFLOW:4������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M13��
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:6����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ��Ӧ��Ϊ~SCr1&DCr4
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_OpMode_0013(void)
{   
	return OSG_Blit_TestOpMode(
								EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,
								OSG_COLOR_RED_32BIT,
								OSG_COLOR_BLUE_32BIT,
								EM_UDIOSG_OPMODE_ANDINVERTED);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_OpMode_0014
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������EM_UDIOSG_OPMODE_ORREVERSE���ģʽ�µ�blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+SR1+DR1+SCr1+DCr4+M14
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��Դ��ͼƽ�����ɫֵ����ORREVERSE���
//@REMARK: 1��û������������������в���   
//@REMARK: 2��û����Բ�ͬ��ɫģʽ���в���
//@REMARK: 3���˴����ٶ�alphaģʽ���в���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ��������ɫֵΪSCr1
//@EXECUTIONFLOW:3����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr4
//@EXECUTIONFLOW:4������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M14��
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:6����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ��Ӧ��ΪSCr1| ~DCr4
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_OpMode_0014(void)
{   
	return OSG_Blit_TestOpMode(
								EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,
								OSG_COLOR_RED_32BIT,
								OSG_COLOR_BLUE_32BIT,
								EM_UDIOSG_OPMODE_ORREVERSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_OpMode_0015
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������EM_UDIOSG_OPMODE_ANDREVERSE���ģʽ�µ�blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+SR1+DR1+SCr1+DCr4+M15
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ��Դ��ͼƽ�����ɫֵ����ANDREVERSE���
//@REMARK: 1��û������������������в���   
//@REMARK: 2��û����Բ�ͬ��ɫģʽ���в���
//@REMARK: 3���˴����ٶ�alphaģʽ���в���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ��������ɫֵΪSCr1
//@EXECUTIONFLOW:3����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr4
//@EXECUTIONFLOW:4������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M15��
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:6����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ��Ӧ��ΪSCr1 &~DCr4
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_OpMode_0015(void)
{   
	return OSG_Blit_TestOpMode(
								EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,
								OSG_COLOR_RED_32BIT,
								OSG_COLOR_BLUE_32BIT,
								EM_UDIOSG_OPMODE_ANDREVERSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_OpMode_0016
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������EM_UDIOSG_OPMODE_NOOP���ģʽ�µ�blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_1+X2/X1+SR1+DR1+SCr1+DCr4+M16
//@EXPECTATION:1���ɹ�ִ��blit������CSUDI_SUCCESS
//@EXPECTATION:2��blit�õ�����ɫֵ����
//@REMARK: 1��û������������������в���   
//@REMARK: 2��û����Բ�ͬ��ɫģʽ���в���
//@REMARK: 3���˴����ٶ�alphaģʽ���в���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1��P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ��������ɫֵΪSCr1
//@EXECUTIONFLOW:3����ʼ������䣩Ŀ�껺��ռ��������ɫֵΪDCr4
//@EXECUTIONFLOW:4������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M16��
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:6����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ��Ӧ��ΪDCr4
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_OpMode_0016(void)
{   
	return OSG_Blit_TestOpMode(
								EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,
								OSG_COLOR_RED_32BIT,
								OSG_COLOR_BLUE_32BIT,
								EM_UDIOSG_OPMODE_NOOP);
}

//**********************************************************************************//
//******************************Begin Test Blit surface******************************//
//**********************************************************************************//

//@TESTCASEID:CSTC_OSG_IT_Blit_Surface_0001
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:���Դ���Դ��ͼƽ����ΪCSUDI_NULL�µ�blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_3+P2_1+SR1+DR1+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: ��   
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGBlit��P2_1,DR1,P1_3,SR1,M1��
//@EXECUTIONFLOW:3��blit����ʧ�ܲ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Surface_0001(void)
{   
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hSurface_D = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);
	
	nRet = CSUDIOSGCreateSurface(ePixelFormat,
									SURFACE_WIDTH_720,
									SURFACE_HEIGHT_576,
									&hSurface_D
									);
	CSTK_ASSERT_TRUE_FATAL(
				(nRet == CSUDI_SUCCESS && hSurface_D != CSUDI_NULL),
				"Step 1 Create P2_1 Surface Failure !"
			);
	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDIOSG_ERROR_BAD_PARAMETER == CSUDIOSGBlit(hSurface_D, CSUDI_NULL, hSurface_S, CSUDI_NULL, EM_UDIOSG_OPMODE_COPY)),
				"Step 2 blit  failure"
			);	
	
	CSTK_FATAL_POINT
		
	if(hSurface_D != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface_D)),
				"Destroy P2_1 surface failure !"
			);
		
		hSurface_D = CSUDI_NULL;
	}	
	
	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Surface_0002
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:���Դ���Ŀ�Ļ�ͼƽ����ΪCSUDI_NULL�µ�blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+P2_3+X2/X1+SR1+DR1+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: ��   
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGBlit��P2_3,DR1,P1_1,SR1,M1��
//@EXECUTIONFLOW:3��blit����ʧ�ܲ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Blit_Surface_0002(void)
{   
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hSurface_D = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	
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
				"Step 1 Create P1_1 Surface Failure !"
			);
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDIOSG_ERROR_BAD_PARAMETER == CSUDIOSGBlit(hSurface_D, CSUDI_NULL, hSurface_S, CSUDI_NULL, EM_UDIOSG_OPMODE_COPY)),
				"Step 2 blit  failure"
			);	
	
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

//@TESTCASEID:CSTC_OSG_IT_Blit_Surface_0003
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:���Դ���eOpMode �Ƿ���blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_3+P2_3+SR1+DR1+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: ��   
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_3����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGCreateSurface��������ռ�P2_3����ɫģʽΪX2��X1
//@EXECUTIONFLOW:3������CSUDIOSGBlit(P2_3,DR1,P1_3,SR1,M1+1)������ʧ�ܲ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4������CSUDIOSGBlit(P2_3,DR1,P1_3,SR1,M17-1)������ʧ�ܲ�����CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_Blit_Surface_0003(void)
{   
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hSurface_D = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);
	
	nRet = CSUDIOSGCreateSurface(ePixelFormat, SURFACE_WIDTH_720, SURFACE_HEIGHT_576, &hSurface_S);
	CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),
							"Step 1 Create P1_1 Surface Failure !");

	nRet = CSUDIOSGCreateSurface(ePixelFormat, SURFACE_WIDTH_720, SURFACE_HEIGHT_576, &hSurface_D);
	CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hSurface_D != CSUDI_NULL),
							"Step 1 Create P1_1 Surface Failure !");
					
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDIOSG_ERROR_BAD_PARAMETER == CSUDIOSGBlit(hSurface_D, CSUDI_NULL, hSurface_S, CSUDI_NULL, EM_UDIOSG_OPMODE_ALPHA+1)),
				"eOpMode ����������ʧ��"
			);	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDIOSG_ERROR_BAD_PARAMETER == CSUDIOSGBlit(hSurface_D, CSUDI_NULL, hSurface_S, CSUDI_NULL, EM_UDIOSG_OPMODE_COPY-1)),
				"eOpMode ����������ʧ��"
			);
	
	CSTK_FATAL_POINT
	{
		if(hSurface_S != CSUDI_NULL)
		{
		      if(CSUDIOSGDestroySurface(hSurface_S) != CSUDI_SUCCESS)
		      	{
				 CSTCPrint("����Surface ʧ��\n");
		      	}
			  
			hSurface_S = CSUDI_NULL;
		}
		if(hSurface_D != CSUDI_NULL)
		{
			 if(CSUDIOSGDestroySurface(hSurface_D) != CSUDI_SUCCESS)
			 {
				 CSTCPrint("����Surface ʧ��");
			 }
			 
			hSurface_D = CSUDI_NULL;
		}
	}
	
	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Surface_0004
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:���blitǰԴsurface������cache�Ѿ�ͬ�����ڴ�
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@EXPECTATION: surface B���������ص㱻�仯��Cr1
//@REMARK: ��   
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface������СΪ100 * 80��С��surfaceA, surfaceB.
//@EXECUTIONFLOW:2��ֱ�Ӷ�surfaceA���ڴ渳ֵΪcr1
//@EXECUTIONFLOW:3������CSUDIOSGBlit��surfaceA����blit��surfaceB��
//@EXECUTIONFLOW:4�����surfaceB�е������Ƿ�Ϊcr1
//@EXECUTIONFLOW:5���ظ�����3~4����
CSUDI_BOOL CSTC_OSG_IT_Blit_Surface_0004(void)
{   
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hSurface_D = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;
	CSUDIOSGPixelFormat_E ePixelFormat =EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	unsigned int uExpectedColor = 0;
	int i;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);
	
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
				CSTK_ASSERT_TRUE_FATAL(0,"Not support pixelformat !!");
			}
			break;
	}
		
	nRet = CSUDIOSGCreateSurface(ePixelFormat,100,80,&hSurface_S);
	CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),
							"Step 1 Create  SurfaceA Failure !");

	nRet = CSUDIOSGCreateSurface(ePixelFormat,100,80,&hSurface_D);
	CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hSurface_D != CSUDI_NULL),
							"Step 1 Create  SurfaceB Failure !");

	nRet = CSUDIOSGGetSurfaceInfo(hSurface_S,&sSurfaceInfo);
	CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && sSurfaceInfo.m_pvSurfaceMem !=CSUDI_NULL),
							"get surfaceinfo  SurfaceA Failure !");	

	for(i = 0; i< 3; i++)
	{
		memset(sSurfaceInfo.m_pvSurfaceMem, 0x88, sSurfaceInfo.m_nMemSize);	
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGBlit(hSurface_D, CSUDI_NULL, hSurface_S, CSUDI_NULL, EM_UDIOSG_OPMODE_COPY)),"Step 3 blit failure!"	);
		
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync failure");	

		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == OSG_iCheckExistColor(hSurface_D, uExpectedColor,CSUDI_NULL,ePixelFormat)),"Step 4 check color failure");	
	}
	
	CSTK_FATAL_POINT
	
	if(hSurface_S != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface_S)),"����hSurface_S ʧ��");
		hSurface_S = CSUDI_NULL;
	}
	
	if(hSurface_D != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface_D)),"����hSurface_D ʧ��");
		hSurface_D = CSUDI_NULL;
	}	
	
	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Surface_0005
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:���blit��Ŀ��surface������cache�Ƿ���ڴ�ͬ��
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@EXPECTATION: surface B���������ص㱻�仯��Cr1
//@REMARK: ��   
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface������СΪ100 * 80��С��surfaceA, surfaceB.
//@EXECUTIONFLOW:2������CSUDIOSGFill�����SurfaceAΪcr1 : 0x88888888
//@EXECUTIONFLOW:3��ֱ�Ӷ�surfaceB���ڴ渳ֵΪ0xff
//@EXECUTIONFLOW:4������CSUDIOSGBlit��surfaceA����blit��surfaceB��
//@EXECUTIONFLOW:5�����surfaceB�е������Ƿ�Ϊcr1
//@EXECUTIONFLOW:6���ظ�2~5����
CSUDI_BOOL CSTC_OSG_IT_Blit_Surface_0005(void)
{   
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hSurface_D = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;
	CSUDIOSGPixelFormat_E ePixelFormat =EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	unsigned int uExpectedColor = 0;
	int i;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);
	
	switch(ePixelFormat)
	{
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_8888:
			{
				uExpectedColor = OSG_FILL_COLOR_GREY_32BIT;
			}
			break;
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_1555:
			{
				uExpectedColor = OSG_iColor32BitTo16Bit(OSG_FILL_COLOR_GREY_32BIT);
			}
			break;
		case EM_UDIOSG_PIXEL_FORMAT_RGBA_5551:
		case EM_UDIOSG_PIXEL_FORMAT_ARGB_4444:
		case EM_UDIOSG_PIXEL_FORMAT_RGB_565:
		default:
			{
				CSTK_ASSERT_TRUE_FATAL(0,"Not support pixelformat !!");
			}
			break;
	}
		
	nRet = CSUDIOSGCreateSurface(ePixelFormat,100,80,&hSurface_S);
	CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),"Step 1 Create Surface Failure !");

	nRet = CSUDIOSGCreateSurface(ePixelFormat,100,80,&hSurface_D);
	CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hSurface_D != CSUDI_NULL),"Step 1 Create Surface Failure !");

	for(i = 0; i< 3; i++)
	{
		nRet = CSUDIOSGFill(hSurface_S, CSUDI_NULL, OSG_FILL_COLOR_GREY_32BIT);
		CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS ),"Step 2 fill  Surface_S Failure !");	

		nRet = CSUDIOSGGetSurfaceInfo(hSurface_D,&sSurfaceInfo);
		CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && sSurfaceInfo.m_pvSurfaceMem != CSUDI_NULL), " getsurfaceinfo Surface_S Failure !");	
		
		memset(sSurfaceInfo.m_pvSurfaceMem, 0xFF, sSurfaceInfo.m_nMemSize);				
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGBlit(hSurface_D, CSUDI_NULL, hSurface_S, CSUDI_NULL, EM_UDIOSG_OPMODE_COPY)), "Step 4 blit failure!");

		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync failure");	
		
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == OSG_iCheckExistColor(hSurface_D, uExpectedColor, CSUDI_NULL, ePixelFormat)), "Step 5 check color failure");	
	}

	CSTK_FATAL_POINT
	
	if(hSurface_S != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface_S)),"����hSurface_S ʧ��");
		hSurface_S = CSUDI_NULL;
	}
	
	if(hSurface_D != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface_D)),"����hSurface_D ʧ��");
		hSurface_D = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
}

//**********************************************************************************//
//******************************Begin Test Fill************************************//
//**********************************************************************************//
//**************************Ӧ����Ըò����������ɫֵ*******************************//

//@TESTCASEID:CSTC_OSG_IT_Fill_0001
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:���Դ�������������ΪCSUDI_NULL�µ�fill����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P2_1+X2/X1+DR1+SCr1
//@EXPECTATION: �ɹ�ִ��fill�������������Ϊȫ��������CSUDI_SUCCESS
//@REMARK: 1��ֻ��һ����ɫֵ����fill���������Ը��ݲ��Ը����ʽ��в���������ɫֵ   
//@REMARK: 2��ֻ��һ����ɫģʽ����fill���������Ը��ݲ��Ը����ʲ���������ɫģʽ�µ�fill����
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ��������ɫֵΪ0x88
//@EXECUTIONFLOW:3������CSUDIOSGFill��P2_1,DR1,SCr1��
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:6����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ��Ӧ��ΪSCr1
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Fill_0001(void)
{   
	return OSG_Blit_TestFill(
							EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,/*surface pixel format*/
							OSG_COLOR_RED_32BIT, /*fill color*/
							EM_RECT_1,			/*fill rect area*/
							CSUDI_TRUE       /*rect is valid or not*/
							);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0002
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:���Դ�������������Ϊ������ͼƽ���fill����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P2_1+X2/X1+DR2+SCr1
//@EXPECTATION: �ɹ�ִ��fill�������������Ϊȫ��������CSUDI_SUCCESS
//@REMARK: 1��ֻ��һ����ɫֵ����fill���������Ը��ݲ��Ը����ʽ��в���������ɫֵ   
//@REMARK: 2��ֻ��һ����ɫģʽ����fill���������Ը��ݲ��Ը����ʲ���������ɫģʽ�µ�fill����
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ��������ɫֵΪ0x88
//@EXECUTIONFLOW:3������CSUDIOSGFill��P2_1,DR2,SCr1��
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:6����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ��Ӧ��ΪSCr1
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Fill_0002(void)
{   
	return OSG_Blit_TestFill(
							EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,/*surface pixel format*/
							OSG_COLOR_RED_32BIT, /*fill color*/
							EM_RECT_2,			/*fill rect area*/
							CSUDI_TRUE       /*rect is valid or not*/
							);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0003
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:���Դ�������������Ϊ��ͼƽ�������fill����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P2_1+X2/X1+DR3+SCr1
//@EXPECTATION: �ɹ�ִ��fill����������CSUDI_SUCCESS
//@REMARK: 1��ֻ��һ����ɫֵ����fill���������Ը��ݲ��Ը����ʽ��в���������ɫֵ   
//@REMARK: 2��ֻ��һ����ɫģʽ����fill���������Ը��ݲ��Ը����ʲ���������ɫģʽ�µ�fill����
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ��������ɫֵΪ0x88
//@EXECUTIONFLOW:3������CSUDIOSGFill��P2_1,DR3,SCr1��
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:6����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ��Ӧ��ΪSCr1
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Fill_0003(void)
{   
	return OSG_Blit_TestFill(
							EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,/*surface pixel format*/
							OSG_COLOR_RED_32BIT, /*fill color*/
							EM_RECT_3,			/*fill rect area*/
							CSUDI_TRUE       /*rect is valid or not*/
							);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0004
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:���Դ�������������Ϊ�Ƿ�ֵ��0��0����fill����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P2_1+X2/X1+DR4+SCr1
//@EXPECTATION: ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1��ֻ��һ����ɫֵ����fill���������Ը��ݲ��Ը����ʽ��в���������ɫֵ   
//@REMARK: 2��ֻ��һ����ɫģʽ����fill���������Ը��ݲ��Ը����ʲ���������ɫģʽ�µ�fill����
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ��������ɫֵΪ0x88
//@EXECUTIONFLOW:3������CSUDIOSGFill��P2_1,DR4,SCr1��
//@EXECUTIONFLOW:4��ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Fill_0004(void)
{   
	return OSG_Blit_TestFill(
							EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,/*surface pixel format*/
							OSG_COLOR_RED_32BIT, /*fill color*/
							EM_RECT_4,			/*fill rect area*/
							CSUDI_FALSE       /*rect is valid or not*/
							);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0005
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:���Դ�������������Ϊ�Ƿ�ֵ��-1��-1����fill����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P2_1+X2/X1+DR5+SCr1
//@EXPECTATION: ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1��ֻ��һ����ɫֵ����fill���������Ը��ݲ��Ը����ʽ��в���������ɫֵ   
//@REMARK: 2��ֻ��һ����ɫģʽ����fill���������Ը��ݲ��Ը����ʲ���������ɫģʽ�µ�fill����
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ��������ɫֵΪ0x88
//@EXECUTIONFLOW:3������CSUDIOSGFill��P2_1,DR5,SCr1��
//@EXECUTIONFLOW:4��ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Fill_0005(void)
{   
	return OSG_Blit_TestFill(
							EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,/*surface pixel format*/
							OSG_COLOR_RED_32BIT, /*fill color*/
							EM_RECT_5,			/*fill rect area*/
							CSUDI_FALSE       /*rect is valid or not*/
							);
}


//@TESTCASEID:CSTC_OSG_IT_Fill_0006
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:���Դ�������������Ϊһ�����ص�fill����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P2_1+X2/X1+DR6+SCr1
//@EXPECTATION: �ɹ�ִ��fill����������CSUDI_SUCCESS
//@REMARK: 1��ֻ��һ����ɫֵ����fill���������Ը��ݲ��Ը����ʽ��в���������ɫֵ   
//@REMARK: 2��ֻ��һ����ɫģʽ����fill���������Ը��ݲ��Ը����ʲ���������ɫģʽ�µ�fill����
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ��������ɫֵΪ0x88
//@EXECUTIONFLOW:3������CSUDIOSGFill��P2_1,DR6,SCr1��
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:6����P2_1_SInfo��Ϣ�л�ȡ��������DR6�������ɫֵ��Ӧ��ΪSCr1
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Fill_0006(void)
{   
	return OSG_Blit_TestFill(
							EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,/*surface pixel format*/
							OSG_COLOR_RED_32BIT, /*fill color*/
							EM_RECT_6,			/*fill rect area*/
							CSUDI_TRUE/*rect is valid or not*/
							);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0007
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:���Դ�������������Ϊ���½�Խ���fill����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P2_1+X2/X1+DR8+SCr1
//@EXPECTATION: ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1��ֻ��һ����ɫֵ����fill���������Ը��ݲ��Ը����ʽ��в���������ɫֵ   
//@REMARK: 2��ֻ��һ����ɫģʽ����fill���������Ը��ݲ��Ը����ʲ���������ɫģʽ�µ�fill����
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ��������ɫֵΪ0x88
//@EXECUTIONFLOW:3������CSUDIOSGFill��P2_1,DR8,SCr1��
//@EXECUTIONFLOW:4��ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Fill_0007(void)
{   
	return OSG_Blit_TestFill(
							EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,/*surface pixel format*/
							OSG_COLOR_RED_32BIT, /*fill color*/
							EM_RECT_8,			/*fill rect area*/
							CSUDI_FALSE       /*rect is valid or not*/
							);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0008
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:���Դ�������������Ϊ���б�Խ���fill����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P2_1+X2/X1+DR9+SCr1
//@EXPECTATION: ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1��ֻ��һ����ɫֵ����fill���������Ը��ݲ��Ը����ʽ��в���������ɫֵ   
//@REMARK: 2��ֻ��һ����ɫģʽ����fill���������Ը��ݲ��Ը����ʲ���������ɫģʽ�µ�fill����
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ��������ɫֵΪ0x88
//@EXECUTIONFLOW:3������CSUDIOSGFill��P2_1,DR9,SCr1��
//@EXECUTIONFLOW:4��ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Fill_0008(void)
{   
	return OSG_Blit_TestFill(
							EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,/*surface pixel format*/
							OSG_COLOR_RED_32BIT, /*fill color*/
							EM_RECT_9,			/*fill rect area*/
							CSUDI_FALSE       /*rect is valid or not*/
							);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0009
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:���Դ�������������Ϊ���½�Խ���fill����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P2_1+X2/X1+DR10+SCr1
//@EXPECTATION: ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1��ֻ��һ����ɫֵ����fill���������Ը��ݲ��Ը����ʽ��в���������ɫֵ   
//@REMARK: 2��ֻ��һ����ɫģʽ����fill���������Ը��ݲ��Ը����ʲ���������ɫģʽ�µ�fill����
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ��������ɫֵΪ0x88
//@EXECUTIONFLOW:3������CSUDIOSGFill��P2_1,DR10,SCr1��
//@EXECUTIONFLOW:4��ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Fill_0009(void)
{   
	return OSG_Blit_TestFill(
							EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,/*surface pixel format*/
							OSG_COLOR_RED_32BIT, /*fill color*/
							EM_RECT_10,			/*fill rect area*/
							CSUDI_FALSE       /*rect is valid or not*/
							);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0010
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:���Դ�������������Ϊ���б�Խ���fill����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P2_1+X2/X1+DR11+SCr1
//@EXPECTATION: ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1��ֻ��һ����ɫֵ����fill���������Ը��ݲ��Ը����ʽ��в���������ɫֵ   
//@REMARK: 2��ֻ��һ����ɫģʽ����fill���������Ը��ݲ��Ը����ʲ���������ɫģʽ�µ�fill����
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ��������ɫֵΪ0x88
//@EXECUTIONFLOW:3������CSUDIOSGFill��P2_1,DR11,SCr1��
//@EXECUTIONFLOW:4��ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Fill_0010(void)
{   
	return OSG_Blit_TestFill(
							EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,/*surface pixel format*/
							OSG_COLOR_RED_32BIT, /*fill color*/
							EM_RECT_11,			/*fill rect area*/
							CSUDI_FALSE       /*rect is valid or not*/
							);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0011
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:���Դ�������������Ϊ���Ͻ�Խ���fill����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P2_1+X2/X1+DR12+SCr1
//@EXPECTATION: ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1��ֻ��һ����ɫֵ����fill���������Ը��ݲ��Ը����ʽ��в���������ɫֵ   
//@REMARK: 2��ֻ��һ����ɫģʽ����fill���������Ը��ݲ��Ը����ʲ���������ɫģʽ�µ�fill����
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ��������ɫֵΪ0x88
//@EXECUTIONFLOW:3������CSUDIOSGFill��P2_1,DR12,SCr1��
//@EXECUTIONFLOW:4��ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Fill_0011(void)
{   
	return OSG_Blit_TestFill(
							EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,/*surface pixel format*/
							OSG_COLOR_RED_32BIT, /*fill color*/
							EM_RECT_12,			/*fill rect area*/
							CSUDI_FALSE       /*rect is valid or not*/
							);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0012
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:�����Դ�ռ䴫������������ΪCSUDI_NULL�µ�fill����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: D+DR1+SCr1
//@EXPECTATION: �ɹ�ִ��fill�������������Ϊȫ��������CSUDI_SUCCESS
//@REMARK: 1��ֻ��һ����ɫֵ����fill���������Ը��ݲ��Ը����ʽ��в���������ɫֵ   
//@REMARK: 2��ֻ��һ����ɫģʽ����fill���������Ը��ݲ��Ը����ʲ���������ɫģʽ�µ�fill����
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���
//@EXECUTIONFLOW:2������CSUDIOSGFill��D,DR1,SCr1��
//@EXECUTIONFLOW:3������CSUDIOSGGetSurfaceInfo��ȡ�Դ�ռ�D����ϢD_SInfo
//@EXECUTIONFLOW:4����D_SInfo��Ϣ�л�ȡ����һ�������ɫֵ��Ӧ��ΪSCr1
CSUDI_BOOL CSTC_OSG_IT_Fill_0012(void)
{   
	return  OSG_iTestFillDisplay(OSG_COLOR_RED_32BIT,EM_RECT_1,CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0013
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:�����Դ�ռ䴫������������Ϊȫ�������fill����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2����ȡ�Դ�ռ��С��DR2���г�ʼ��
//@INPUT: D+DR2+SCr1
//@EXPECTATION: �ɹ�ִ��fill�������������Ϊȫ��������CSUDI_SUCCESS
//@REMARK: 1��ֻ��һ����ɫֵ����fill���������Ը��ݲ��Ը����ʽ��в���������ɫֵ   
//@REMARK: 2��ֻ��һ����ɫģʽ����fill���������Ը��ݲ��Ը����ʲ���������ɫģʽ�µ�fill����
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���
//@EXECUTIONFLOW:2������CSUDIOSGFill��D,DR2,SCr1��
//@EXECUTIONFLOW:3������CSUDIOSGGetSurfaceInfo��ȡ�Դ�ռ�D����ϢD_SInfo
//@EXECUTIONFLOW:4����D_SInfo��Ϣ�л�ȡ����һ�������ɫֵ��Ӧ��ΪSCr1
CSUDI_BOOL CSTC_OSG_IT_Fill_0013(void)
{   
	return  OSG_iTestFillDisplay(OSG_COLOR_RED_32BIT,EM_RECT_2,CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0014
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:�����Դ�ռ䴫������������Ϊ��ͼƽ�������fill����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2����ȡ�Դ�ռ��С��DR3���г�ʼ��
//@INPUT: D+DR3+SCr1
//@EXPECTATION: �ɹ�ִ��fill�������������Ϊȫ��������CSUDI_SUCCESS
//@REMARK: 1��ֻ��һ����ɫֵ����fill���������Ը��ݲ��Ը����ʽ��в���������ɫֵ   
//@REMARK: 2��ֻ��һ����ɫģʽ����fill���������Ը��ݲ��Ը����ʲ���������ɫģʽ�µ�fill����
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���
//@EXECUTIONFLOW:2����ʼ������䣩�Դ��ռ��������ɫֵΪ0
//@EXECUTIONFLOW:3������CSUDIOSGFill��D,DR3,SCr1��
//@EXECUTIONFLOW:4������CSUDIOSGGetSurfaceInfo��ȡ�Դ�ռ�D����ϢD_SInfo
//@EXECUTIONFLOW:5����D_SInfo��Ϣ�л�ȡ����һ�������ɫֵ��Ӧ��ΪSCr1
CSUDI_BOOL CSTC_OSG_IT_Fill_0014(void)
{   
	return  OSG_iTestFillDisplay(OSG_COLOR_RED_32BIT,EM_RECT_3,CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0015
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:�����Դ�ռ䴫������������Ϊ�Ƿ�ֵ��0��0����fill����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: D+DR4+SCr1
//@EXPECTATION: ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1��ֻ��һ����ɫֵ����fill���������Ը��ݲ��Ը����ʽ��в���������ɫֵ   
//@REMARK: 2��ֻ��һ����ɫģʽ����fill���������Ը��ݲ��Ը����ʲ���������ɫģʽ�µ�fill����
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���
//@EXECUTIONFLOW:2������CSUDIOSGFill��D,DR4,SCr1��
//@EXECUTIONFLOW:3��ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_Fill_0015(void)
{   
	return  OSG_iTestFillDisplay(OSG_COLOR_RED_32BIT,EM_RECT_4,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0016
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:�����Դ�ռ䴫������������Ϊ�Ƿ�ֵ��-1��-1����fill����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: D+DR5+SCr1
//@EXPECTATION: ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1��ֻ��һ����ɫֵ����fill���������Ը��ݲ��Ը����ʽ��в���������ɫֵ   
//@REMARK: 2��ֻ��һ����ɫģʽ����fill���������Ը��ݲ��Ը����ʲ���������ɫģʽ�µ�fill����
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���
//@EXECUTIONFLOW:3������CSUDIOSGFill��D,DR4,SCr1��
//@EXECUTIONFLOW:4��ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_Fill_0016(void)
{   
	return  OSG_iTestFillDisplay(OSG_COLOR_RED_32BIT,EM_RECT_5,CSUDI_FALSE);
}


//@TESTCASEID:CSTC_OSG_IT_Fill_0017
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:�����Դ�ռ䴫������������Ϊһ�����ص�fill����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: D+DR6+SCr1
//@EXPECTATION: �ɹ�ִ��fill����������CSUDI_SUCCESS
//@REMARK: 1��ֻ��һ����ɫֵ����fill���������Ը��ݲ��Ը����ʽ��в���������ɫֵ   
//@REMARK: 2��ֻ��һ����ɫģʽ����fill���������Ը��ݲ��Ը����ʲ���������ɫģʽ�µ�fill����
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���
//@EXECUTIONFLOW:2����ʼ������䣩�Դ��ռ��������ɫֵΪ0
//@EXECUTIONFLOW:3������CSUDIOSGFill��D,DR3,SCr1��
//@EXECUTIONFLOW:4������CSUDIOSGGetSurfaceInfo��ȡ�Դ�ռ�D����ϢD_SInfo
//@EXECUTIONFLOW:5����D_SInfo��Ϣ�л�ȡ����DR6����ɫֵ��Ӧ��ΪSCr1
CSUDI_BOOL CSTC_OSG_IT_Fill_0017(void)
{   
	
	  return OSG_iTestFillDisplay(OSG_COLOR_RED_32BIT,EM_RECT_6,CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0018
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:�����Դ�ռ䴫������������Ϊ���½�Խ���fill����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2����ȡ�Դ�ռ��С��DR8���г�ʼ��
//@INPUT: D+DR8+SCr1
//@EXPECTATION: ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1��ֻ��һ����ɫֵ����fill���������Ը��ݲ��Ը����ʽ��в���������ɫֵ   
//@REMARK: 2��ֻ��һ����ɫģʽ����fill���������Ը��ݲ��Ը����ʲ���������ɫģʽ�µ�fill����
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���
//@EXECUTIONFLOW:2������CSUDIOSGFill��D,DR8,SCr1��
//@EXECUTIONFLOW:3��ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_Fill_0018(void)
{   
	return  OSG_iTestFillDisplay(OSG_COLOR_RED_32BIT,EM_RECT_8,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0019
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:�����Դ�ռ䴫������������Ϊ���б�Խ���fill����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2����ȡ�Դ�ռ��С��DR9���г�ʼ��
//@INPUT: D+DR9+SCr1
//@EXPECTATION: ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1��ֻ��һ����ɫֵ����fill���������Ը��ݲ��Ը����ʽ��в���������ɫֵ   
//@REMARK: 2��ֻ��һ����ɫģʽ����fill���������Ը��ݲ��Ը����ʲ���������ɫģʽ�µ�fill����
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���
//@EXECUTIONFLOW:2������CSUDIOSGFill��D,DR9,SCr1��
//@EXECUTIONFLOW:3��ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_Fill_0019(void)
{   
	return  OSG_iTestFillDisplay(OSG_COLOR_RED_32BIT,EM_RECT_9,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0020
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:�����Դ�ռ䴫������������Ϊ���½�Խ���fill����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2����ȡ�Դ�ռ��С��DR10���г�ʼ��
//@INPUT: D+DR10+SCr1
//@EXPECTATION: ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1��ֻ��һ����ɫֵ����fill���������Ը��ݲ��Ը����ʽ��в���������ɫֵ   
//@REMARK: 2��ֻ��һ����ɫģʽ����fill���������Ը��ݲ��Ը����ʲ���������ɫģʽ�µ�fill����
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���
//@EXECUTIONFLOW:2������CSUDIOSGFill��D,DR10,SCr1��
//@EXECUTIONFLOW:3��ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_Fill_0020(void)
{   
	return  OSG_iTestFillDisplay(OSG_COLOR_RED_32BIT,EM_RECT_10,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0021
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:�����Դ�ռ䴫������������Ϊ���б�Խ���fill����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2����ȡ�Դ�ռ��С��DR11���г�ʼ��
//@INPUT: D+DR11+SCr1
//@EXPECTATION: ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1��ֻ��һ����ɫֵ����fill���������Ը��ݲ��Ը����ʽ��в���������ɫֵ   
//@REMARK: 2��ֻ��һ����ɫģʽ����fill���������Ը��ݲ��Ը����ʲ���������ɫģʽ�µ�fill����
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���
//@EXECUTIONFLOW:2������CSUDIOSGFill��D,DR11,SCr1��
//@EXECUTIONFLOW:3��ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_Fill_0021(void)
{   
	return  OSG_iTestFillDisplay(OSG_COLOR_RED_32BIT,EM_RECT_11,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0022
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:�����Դ�ռ䴫������������Ϊ���Ͻ�Խ���fill����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2����ȡ�Դ�ռ��С��DR12���г�ʼ��
//@INPUT: D+DR12+SCr1+SCr1
//@EXPECTATION: ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1��ֻ��һ����ɫֵ����fill���������Ը��ݲ��Ը����ʽ��в���������ɫֵ   
//@REMARK: 2��ֻ��һ����ɫģʽ����fill���������Ը��ݲ��Ը����ʲ���������ɫģʽ�µ�fill����
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���
//@EXECUTIONFLOW:2������CSUDIOSGFill��D,DR12,SCr1��
//@EXECUTIONFLOW:3��ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_Fill_0022(void)
{   
	return  OSG_iTestFillDisplay(OSG_COLOR_RED_32BIT,EM_RECT_12,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0023
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:���Դ���Դ��ͼƽ������ƽ�涼ΪCSUDI_NULL��fill����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: P2_3+DR1+SCr1
//@EXPECTATION: ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1��ֻ��һ����ɫֵ����fill���������Ը��ݲ��Ը����ʽ��в���������ɫֵ   
//@REMARK: 2��ֻ��һ����ɫģʽ����fill���������Ը��ݲ��Ը����ʲ���������ɫģʽ�µ�fill����
//@EXECUTIONFLOW:1������CSUDIOSGFill��P2_3,DR1,SCr1��
//@EXECUTIONFLOW:2��ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_Fill_0023(void)
{   
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDIOSG_ERROR_BAD_PARAMETER == CSUDIOSGFill(CSUDI_NULL, CSUDI_NULL, OSG_COLOR_RED_32BIT)),
				"Step 1 failure"
			);
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0024
//@CASEGROUP:  CSUDIOSGFill
//@DESCRIPTION:  ���Դ�������������Ϊȫ����fill����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P2_1+X2/X1+DR1+SCr1
//@EXPECTATION: ÿ��fill����������CSUDI_SUCCESS����Ļ����ʾ��ȷ��ɫ
//@REMARK: ֻ��һ����ɫģʽ����fill���������Ը��ݲ��Ը����ʲ���������ARGB8888ģʽ����ɫģʽ
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ��������ɫֵΪ0
//@EXECUTIONFLOW:3������CSUDIOSGFill��P2_1,DR1,SCr1��
//@EXECUTIONFLOW:4������CSUDIOSGGetSurfaceInfo��ȡĿ�Ļ���ռ�P2_1����ϢP2_1_SInfo
//@EXECUTIONFLOW:5����P2_1_SInfo��Ϣ�л�ȡ����һ�������ɫֵ��Ӧ��ΪSCr1
//@EXECUTIONFLOW:6������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Fill_0024(void)
{   
	return OSG_Blit_TestFill(
							EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,/*surface pixel format*/
							OSG_COLOR_RED_32BIT, /*fill color*/
							EM_RECT_1,			/*fill rect area*/
							CSUDI_TRUE       /*rect is valid or not*/
							);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0025
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:���Դ�������������Ϊ���Ͻ�Խ���fill����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P2_1+X2��X1+DR14+SCr1
//@EXPECTATION: ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1��ֻ��һ����ɫֵ����fill���������Ը��ݲ��Ը����ʽ��в���������ɫֵ   
//@REMARK: 2��ֻ��һ����ɫģʽ����fill���������Ը��ݲ��Ը����ʲ���������ɫģʽ�µ�fill����
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ��������ɫֵΪ0
//@EXECUTIONFLOW:3������CSUDIOSGFill��P2_1,DR14,SCr1��
//@EXECUTIONFLOW:4��ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Fill_0025(void)
{   
	return OSG_Blit_TestFill(
							EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,/*surface pixel format*/
							OSG_COLOR_RED_32BIT, /*fill color*/
							EM_RECT_14,			/*fill rect area*/
							CSUDI_FALSE       /*rect is valid or not*/
							);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0026
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:���Դ�������������Ϊ���б�Խ���fill����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P2_1+X2/X1+DR15+SCr1
//@EXPECTATION: ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1��ֻ��һ����ɫֵ����fill���������Ը��ݲ��Ը����ʽ��в���������ɫֵ   
//@REMARK: 2��ֻ��һ����ɫģʽ����fill���������Ը��ݲ��Ը����ʲ���������ɫģʽ�µ�fill����
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2����ʼ������䣩Դ����ռ��������ɫֵΪ0
//@EXECUTIONFLOW:3������CSUDIOSGFill��P2_1,DR15,SCr1��
//@EXECUTIONFLOW:4��ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Fill_0026(void)
{   
	return OSG_Blit_TestFill(
							EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,/*surface pixel format*/
							OSG_COLOR_RED_32BIT, /*fill color*/
							EM_RECT_15,			/*fill rect area*/
							CSUDI_FALSE       /*rect is valid or not*/
							);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0027
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:���Դ�������������Ϊ���б�Խ���fill����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P2_1+X2/X1+DR16+SCr1
//@EXPECTATION: ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1��ֻ��һ����ɫֵ����fill���������Ը��ݲ��Ը����ʽ��в���������ɫֵ   
//@REMARK: 2��ֻ��һ����ɫģʽ����fill���������Ը��ݲ��Ը����ʲ���������ɫģʽ�µ�fill����
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P2_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGFill��P2_1,DR16,SCr1��
//@EXECUTIONFLOW:3��ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Fill_0027(void)
{   
	return OSG_Blit_TestFill(
							EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,/*surface pixel format*/
							OSG_COLOR_RED_32BIT, /*fill color*/
							EM_RECT_16,			/*fill rect area*/
							CSUDI_FALSE       /*rect is valid or not*/
							);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0028
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:�����Դ�ռ䴫������������Ϊ���Ͻ�Խ���fill����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: D+DR14+SCr1
//@EXPECTATION: ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1��ֻ��һ����ɫֵ����fill���������Ը��ݲ��Ը����ʽ��в���������ɫֵ   
//@REMARK: 2��ֻ��һ����ɫģʽ����fill���������Ը��ݲ��Ը����ʲ���������ɫģʽ�µ�fill����
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���
//@EXECUTIONFLOW:2������CSUDIOSGFill��D,DR14,SCr1��
//@EXECUTIONFLOW:3��ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_Fill_0028(void)
{   
	return  OSG_iTestFillDisplay(OSG_COLOR_RED_32BIT,EM_RECT_14,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0029
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:�����Դ�ռ䴫������������Ϊ���б�Խ���fill����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: D+DR15+SCr1
//@EXPECTATION: ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1��ֻ��һ����ɫֵ����fill���������Ը��ݲ��Ը����ʽ��в���������ɫֵ   
//@REMARK: 2��ֻ��һ����ɫģʽ����fill���������Ը��ݲ��Ը����ʲ���������ɫģʽ�µ�fill����
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���
//@EXECUTIONFLOW:2������CSUDIOSGFill��D,DR15,SCr1��
//@EXECUTIONFLOW:3��ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_Fill_0029(void)
{   
	return  OSG_iTestFillDisplay(OSG_COLOR_RED_32BIT,EM_RECT_15,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0030
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:�����Դ�ռ䴫������������Ϊ���б�Խ���fill����
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: D+DR16+SCr1
//@EXPECTATION: ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1��ֻ��һ����ɫֵ����fill���������Ը��ݲ��Ը����ʽ��в���������ɫֵ   
//@REMARK: 2��ֻ��һ����ɫģʽ����fill���������Ը��ݲ��Ը����ʲ���������ɫģʽ�µ�fill����
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���
//@EXECUTIONFLOW:2������CSUDIOSGFill��D,DR16,SCr1��
//@EXECUTIONFLOW:3��ִ��fill����ʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_Fill_0030(void)
{   
	return  OSG_iTestFillDisplay(OSG_COLOR_RED_32BIT,EM_RECT_16,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0031
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:���Զ�ϸ������ϸ��������ɫ���
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: 4000*50���ο�,50*4000���ο�
//@EXPECTATION: ִ��fill�ɹ�����ɫ�����ȷ
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface����4000*50 ����ռ䣬�������سɹ�
//@EXECUTIONFLOW:2������CSUDIOSGFill���{0,0,4000,50}����Ϊ��ɫ��ɫ���������سɹ�
//@EXECUTIONFLOW:3��У��{0,0,4000,50}������ɫ����Ϊ��ɫ
//@EXECUTIONFLOW:4������CSUDIOSGCreateSurface����50*4000 ����ռ䣬�������سɹ�
//@EXECUTIONFLOW:5������CSUDIOSGFill���{0,0,50,4000}Ϊ��ɫ��ɫ���������سɹ�
//@EXECUTIONFLOW:6��У��{0,0,4000,50}������ɫ����Ϊ��ɫ
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٴ�����surface
CSUDI_BOOL CSTC_OSG_IT_Fill_0031(void)
{
	CSUDIOSGRect_S sRect;
	CSUDI_BOOL bRet = CSUDI_FALSE;

	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;

	sRect.m_nX = sRect.m_nY = 0;
	sRect.m_nWidth  = 4000; 
	sRect.m_nHeight = 50;
	
	bRet  =  OSG_Blit_TestFillEx(ePixelFormat, OSG_COLOR_RED_32BIT, 4000, 50, &sRect);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == bRet),"����4000*50�ľ���ʧ��!");
	
	sRect.m_nWidth  = 50; 
	sRect.m_nHeight = 4000;	
	
	bRet =  OSG_Blit_TestFillEx(ePixelFormat, OSG_COLOR_RED_32BIT, 50, 4000, &sRect);	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == bRet),"����50*4000�ľ���ʧ��!");
	
	CSTK_FATAL_POINT;

	return bRet;
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0032
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:��������������䣬����������н���
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@EXPECTATION:���������ɺ󣬽����������ɫӦ��Ϊ���һ��������ɫ
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface����720*576 ����ռ䣬�������سɹ�
//@EXECUTIONFLOW:2������CSUDIOSGFill���{0,0,720,576}����Ϊ��ɫSCr1���������سɹ�
//@EXECUTIONFLOW:3��У�������������ɫ,��������{0,0,720,576}ΪSCr1
//@EXECUTIONFLOW:4������CSUDIOSGFill���{720/4,576/4,720/2,576/2}Ϊ��ɫSCr2���������سɹ�
//@EXECUTIONFLOW:5��У�������������ɫ,��������{720/4,576/4,720/2,576/2}ΪSCr2
//@EXECUTIONFLOW:6������CSUDIOSGDestroySurface���ٴ�����surface
CSUDI_BOOL CSTC_OSG_IT_Fill_0032(void)
{
	CSUDIOSGPixelFormat_E ePixelFormat =EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;	

	CSUDI_HANDLE hSurface = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;

	CSUDIOSGRect_S sRect ;
	unsigned int ucTmpColor = 0;
	unsigned int ucFillColorRed = OSG_COLOR_RED_32BIT;
	unsigned int ucFillColorBlue = OSG_COLOR_BLUE_32BIT;	
	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);

	nRet = CSUDIOSGCreateSurface(ePixelFormat,720,576,&hSurface);
	CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hSurface != CSUDI_NULL),	"Step 1 Create Surface Failure !");
			
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo)),	"Get Surface info failure !");	
	CSTK_ASSERT_TRUE_FATAL(((sSurfaceInfo.m_pvSurfaceMem != CSUDI_NULL)&&(sSurfaceInfo.m_nMemSize>0)),"Get Surface info failure !");
	
	memset(sSurfaceInfo.m_pvSurfaceMem, 0x88, sSurfaceInfo.m_nMemSize);

	//************************���ȫ�������ɫ*****************************************//
	ucTmpColor = OSG_iGetColorWithPixelMode(ucFillColorRed,ePixelFormat);

	nRet = CSUDIOSGFill(hSurface, NULL, ucFillColorRed);

	CSTK_ASSERT_TRUE_FATAL(((nRet == CSUDI_SUCCESS)),	"Step 2 fill should be success !");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync())," Sync Failure !"	);	

	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == OSG_iCheckExistColor(hSurface,ucTmpColor,NULL,ePixelFormat)),	"step 3 Check Surface color Failure !");

	//*************************���1/2����������ɫ****************************************//
	ucTmpColor = OSG_iGetColorWithPixelMode(ucFillColorBlue,ePixelFormat);
	sRect.m_nX = 720>>2;
	sRect.m_nY = 576>>2;
	sRect.m_nWidth = 720>>1;
	sRect.m_nHeight = 576>>1;
	
	nRet = CSUDIOSGFill(hSurface, &sRect, ucFillColorBlue);

	CSTK_ASSERT_TRUE_FATAL(((nRet == CSUDI_SUCCESS)),	"Step 4 fill should be success !");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == OSG_iCheckExistColor(hSurface,ucTmpColor,&sRect,ePixelFormat)),"step 5 Check Surface color Failure !"	);
	
	CSTK_FATAL_POINT;
	
	if(hSurface != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface)),"step 6 Destroy  surface failure !");
		hSurface = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0033
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:����CSUDIOSGFill��cache��ͬ��
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@EXPECTATION:fill����surface�Ļ�ͼ����Ӧ�ñ����ڴ��cache��һ����
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ䣬�������سɹ�
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡsurface����Ϣ
//@EXECUTIONFLOW:3����surface�����������ڴ�ռ�memset��0x88
//@EXECUTIONFLOW:4������CSUDIOSGFill�����������ռ�Ϊ��ɫ0xFFFFFFFF���������سɹ�
//@EXECUTIONFLOW:5��У��ÿ�����ص�ֵ,������Ϊ0xFFFFFFFF
//@EXECUTIONFLOW:6���ظ�����3��5һ�ٴ�
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٴ�����surface
CSUDI_BOOL CSTC_OSG_IT_Fill_0033(void)
{
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;	

	CSUDI_HANDLE hSurface = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;

	unsigned int ucTmpColor = 0;
	unsigned int ucFillColorWhite = 0xFFFFFFFF;
	int i;	

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);
	
	nRet = CSUDIOSGCreateSurface(ePixelFormat,720,576,&hSurface);
	CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hSurface != CSUDI_NULL),	"Step 1 Create Surface Failure !");
			
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo)),	"step 2 Get Surface info failure !");	
	CSTK_ASSERT_TRUE_FATAL(((sSurfaceInfo.m_pvSurfaceMem != CSUDI_NULL)&&(sSurfaceInfo.m_nMemSize>0)),"step 2 Get Surface mem info failure !");


	//************************���ȫ�������ɫ*****************************************//
	for(i = 0 ; i< 100; i++)
	{
		ucFillColorWhite -= 0x7FFF;
		ucTmpColor = OSG_iGetColorWithPixelMode(ucFillColorWhite,ePixelFormat);
		memset(sSurfaceInfo.m_pvSurfaceMem, 0x88, sSurfaceInfo.m_nMemSize);	
		nRet = CSUDIOSGFill(hSurface, NULL, ucFillColorWhite);

		CSTK_ASSERT_TRUE_FATAL(((nRet == CSUDI_SUCCESS)),	"Step 4 fill should be success !");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync())," Sync Failure !");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == OSG_iCheckExistColor(hSurface,ucTmpColor,NULL,ePixelFormat)),	"step 5 Check Surface color Failure !");	
	}
	
	CSTK_FATAL_POINT;
	if(hSurface != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS== CSUDIOSGDestroySurface(hSurface)),"step 7 Destroy  surface failure !");
		hSurface = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
	
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0034
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:����CSUDIOSGFill�Ը߶�Ϊ1�����Ϊ1�ľ��ε����
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@EXPECTATION:ִ��fill�ɹ�����ɫ�����ȷ
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface����1*4000 ����ռ䣬�������سɹ�
//@EXECUTIONFLOW:2������CSUDIOSGFill���{0,0,4000,1}����Ϊ��ɫSCr1���������سɹ�
//@EXECUTIONFLOW:3��У��{0,0,4000,1}������ɫ����ΪSCr1
//@EXECUTIONFLOW:4������CSUDIOSGCreateSurface����1*4000 ����ռ䣬�������سɹ�
//@EXECUTIONFLOW:5������CSUDIOSGFill���{0,0,1,4000}����Ϊ��ɫSCr2���������سɹ�
//@EXECUTIONFLOW:6��У��{0,0,4000,1}������ɫ����ΪSCr2
//@EXECUTIONFLOW:7������CSUDIOSGDestroySurface���ٴ�����surface
CSUDI_BOOL CSTC_OSG_IT_Fill_0034(void)
{
	CSUDIOSGRect_S sRect;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;

	sRect.m_nX = sRect.m_nY = 0;
	
	sRect.m_nWidth  = 1; 
	sRect.m_nHeight = 4000;
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == OSG_Blit_TestFillEx(ePixelFormat,OSG_COLOR_RED_32BIT,1, 4000, &sRect)),"����1* 4000�ľ���ʧ��!");
	
	sRect.m_nWidth  = 4000; 
	sRect.m_nHeight = 1;
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == OSG_Blit_TestFillEx(ePixelFormat,OSG_COLOR_BLUE_32BIT,4000, 1, &sRect)),"����4000* 1�ľ���ʧ��!");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
	
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0035
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:����colorkey alpha�����Ƿ��CSUDIOSGFill��������Ӱ��
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@EXPECTATION:ִ��fill�ɹ�����ɫ�����ȷ
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface����720*576 ����ռ䣬�������سɹ�
//@EXECUTIONFLOW:2������CSUDIOSGSetSurfaceInfo�ӿ����û�ͼƽ����Ϣ��m_nColorKeyFlag = 1,m_nAlphaFlag=1,m_uColorkey=0xFFFF0000,m_uAlpha=0x55,�������سɹ�
//@EXECUTIONFLOW:3������CSUDIOSGFill���{0,0,720,576}����Ϊ��ɫ0xFFFF0000���������سɹ�
//@EXECUTIONFLOW:4��У��{0,0,720,576}������ɫ����Ϊ0xFFFF0000
//@EXECUTIONFLOW:5������CSUDIOSGDestroySurface���ٴ�����surface
CSUDI_BOOL CSTC_OSG_IT_Fill_0035(void)
{
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;	

	CSUDI_HANDLE hSurface = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;

	unsigned int ucTmpColor = 0;
	unsigned int ucFillColorRed = 0xFFFF0000;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);
	
	nRet = CSUDIOSGCreateSurface(ePixelFormat,720,576,&hSurface);
	CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hSurface != CSUDI_NULL),	"Step 1 Create Surface Failure !");
			
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo)),	"Get Surface info failure !");	
	CSTK_ASSERT_TRUE_FATAL(((sSurfaceInfo.m_pvSurfaceMem != CSUDI_NULL)&&(sSurfaceInfo.m_nMemSize>0)),"Get Surface info failure !");

	sSurfaceInfo.m_nAlphaFlag 		= 1;
	sSurfaceInfo.m_nColorKeyFlag 	= 1;
	sSurfaceInfo.m_uAlpha 			= 0x55;
	sSurfaceInfo.m_uColorkey 		= 0xFFFF0000;
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSetSurfaceInfo(hSurface, &sSurfaceInfo)),	"step 2 set Surface info failure !");	

	//************************���ȫ�������ɫ*****************************************//
	ucTmpColor = OSG_iGetColorWithPixelMode(ucFillColorRed,ePixelFormat);

	memset(sSurfaceInfo.m_pvSurfaceMem, 0x88, sSurfaceInfo.m_nMemSize);	
	
	nRet = CSUDIOSGFill(hSurface, NULL, ucFillColorRed);
	CSTK_ASSERT_TRUE_FATAL(((nRet == CSUDI_SUCCESS)),	"Step 3 fill should be success !");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync())," Sync Failure !"	);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == OSG_iCheckExistColor(hSurface,ucTmpColor,NULL,ePixelFormat)),	"step 4 Check Surface color Failure !");	
	
	CSTK_FATAL_POINT;	
	if(hSurface != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS== CSUDIOSGDestroySurface(hSurface)),"step 5 Destroy  surface failure !");
		hSurface = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0036
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:����CSUDIOSGFill�Ը߶�Ϊ1�����Ϊ1�ľ��ε����
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@EXPECTATION:ִ��fill�ɹ�����ɫ�����ȷ
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface����200*200 ����ռ䣬��ɫģʽΪEM_UDIOSG_PIXEL_FORMAT_ARGB_8888�������سɹ�
//@EXECUTIONFLOW:2������CSUDIOSGFill��ÿ�����һ�����أ�ָ����ɫΪ��ɫ��ѭ�����ֱ��{0,0,200,200}���������ϣ��������سɹ�
//@EXECUTIONFLOW:3��У��{0,0,200,200}������ɫ����Ϊ��ɫ
//@EXECUTIONFLOW:4������CSUDIOSGDestroySurface���ٴ�����surface
CSUDI_BOOL CSTC_OSG_IT_Fill_0036(void)
{
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;

	bRet = OSG_Blit_TestFillOnePixel(ePixelFormat, OSG_COLOR_RED_32BIT);
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == bRet),"����һ�����ص����ʧ��!");

	CSTK_FATAL_POINT;	
	
	return bRet;
}

//@CASEGROUP: CSUDIOSGSync
//@DESCRIPTION:����(����ռ���Դ�֮��)ִ��blit����֮���������ö��Sync�����(����alphaģʽ)
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT:  P1_1+D+X2+SR1+DR1+M1+SCr1+DCr4
//@EXPECTATION: 1�����λ��߶��sync����֮�󣬲���ı�blit��Ч��
//@EXPECTATION: 2���ڶ��λ���֮���syncʱ��Ӧ����5��֮��
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���D�������ɹ�
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ�Դ�ռ�D����Ϣ����С����ɫģʽ
//@EXECUTIONFLOW:3�����ݲ���(2)��ȡ����Ϣ����CSUDIOSGCreateSurface��������ռ�P1_1
//@EXECUTIONFLOW:4������CSUDIOSGFill����Դ�ռ�D��ɫֵΪDCr4�������ɹ�
//@EXECUTIONFLOW:5������CSUDISync��ɲ���
//@EXECUTIONFLOW:6������CSUDIOSGFill����ͼƽ��P1_1��ɫֵΪSCr1�������ɹ�
//@EXECUTIONFLOW:7������CSUDISync��ɲ���
//@EXECUTIONFLOW:8������CSUDIOSGBlit��D,DR1,P1_1,SR1,M1��
//@EXECUTIONFLOW:9������CSUDISync���blit����
//@EXECUTIONFLOW:10������CSUDIOSGGetSurfaceInfo��ȡD�Դ�ռ����Ϣ
//@EXECUTIONFLOW:11�������Դ�ռ��ڵ��������ɫֵΪSCr1
//@EXECUTIONFLOW:12���ٴε���CSUDISync������Syncʱ����5��֮��
//@EXECUTIONFLOW:13������CSUDIOSGGetSurfaceInfo��ȡD�Դ�ռ����Ϣ
//@EXECUTIONFLOW:14�������Դ�ռ��ڵ��������ɫֵ��ΪSCr1
//@EXECUTIONFLOW:16���ظ�����4��14 10�Σ�����ÿ�ζ��ɹ���
//@EXECUTIONFLOW:15������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Sync_0001(void)
{   
	return  OSG_iTestSyncWithDisplay(CSUDI_FALSE,1);
}

//@CASEGROUP: CSUDIOSGSync
//@DESCRIPTION:����(����ռ�֮��)ִ��blit����֮���������ö��Sync�����(����alphaģʽ)
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION:ϵͳ֧����ɫģʽX2
//@INPUT:  P1_1+P2_1+X2+SR1+DR1+M1+SCr1+DCr4
//@EXPECTATION: 1�����λ��߶��sync����֮�󣬲���ı�blit��Ч��
//@EXPECTATION: 2���ڶ��λ���֮���syncʱ��Ӧ����5��֮��
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1,��ɫģʽΪX2��X1�������ɹ�
//@EXECUTIONFLOW:2������CSUDIOSGCreateSurface��������ռ�P2_1,��ɫģʽΪX2��X1�������ɹ�
//@EXECUTIONFLOW:3������CSUDIOSGFill����ͼ��ĻP2_1��ɫֵΪDCr4�������ɹ�
//@EXECUTIONFLOW:4������CSUDISync��ɲ���
//@EXECUTIONFLOW:5������CSUDIOSGFill����ͼƽ��P1_1��ɫֵΪSCr1�������ɹ�
//@EXECUTIONFLOW:6������CSUDISync��ɲ���
//@EXECUTIONFLOW:7������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M1��
//@EXECUTIONFLOW:8������CSUDISync��ɲ���
//@EXECUTIONFLOW:9������CSUDIOSGGetSurfaceInfo��ȡP2_1�ռ����Ϣ
//@EXECUTIONFLOW:10������P2_1�ռ��ڵ��������ɫֵΪSCr1
//@EXECUTIONFLOW:11���ٴε���CSUDISync������Syncʱ����5��֮��
//@EXECUTIONFLOW:12������CSUDIOSGGetSurfaceInfo��ȡP2_1�ռ����Ϣ
//@EXECUTIONFLOW:13������P2_1�ռ��ڵ��������ɫֵ��ΪSCr1
//@EXECUTIONFLOW:14���ظ�����4��14 10�Σ�����ÿ�ζ��ɹ���
//@EXECUTIONFLOW:16������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Sync_0002(void)
{   
	return  OSG_iTestSync(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,CSUDI_FALSE,1);
}
//@CASEGROUP: CSUDIOSGSync
//@DESCRIPTION:����(����ռ�֮��)ִ��blit����֮���������ö��Sync�����(��alphaģʽ)
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT:  P1_1+P2_1+X2+SR1+DR1+M17+SCr1+DCr4+SA1+DA1
//@EXPECTATION: 1�����λ��߶��sync����֮�󣬲���ı�blit��Ч��
//@EXPECTATION: 2���ڶ��λ���֮���syncʱ��Ӧ����5��֮��
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1,��ɫģʽΪX2��X1�������ɹ�
//@EXECUTIONFLOW:2������CSUDIOSGCreateSurface��������ռ�P2_1,��ɫģʽΪX2��X1�������ɹ�
//@EXECUTIONFLOW:3�����û���ռ�P1_1��ȫ��alphaֵΪSA1,P2_1��ȫ��alphaֵΪDA1
//@EXECUTIONFLOW:4������CSUDIOSGFill���P2_1��ɫֵΪDCr4�������ɹ�
//@EXECUTIONFLOW:5������CSUDISync��ɲ���
//@EXECUTIONFLOW:6������CSUDIOSGFill����ͼƽ��P1_1��ɫֵΪSCr1�������ɹ�
//@EXECUTIONFLOW:7������CSUDISync��ɲ���
//@EXECUTIONFLOW:8������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M17��
//@EXECUTIONFLOW:9������CSUDISync���blit����
//@EXECUTIONFLOW:10������CSUDIOSGGetSurfaceInfo��ȡP2_1�ռ����Ϣ
//@EXECUTIONFLOW:11�������ռ�P2_1�л�ȡ����һ�������ɫֵ�����Ϲ�ʽ(1)(2)(3)(4)
//@EXECUTIONFLOW:12���ٴε���CSUDISync������Syncʱ����5��֮��
//@EXECUTIONFLOW:13������CSUDIOSGGetSurfaceInfo��ȡD�Դ�ռ����Ϣ
//@EXECUTIONFLOW:14�������ռ�P2_1�л�ȡ����һ�������ɫֵ���Է��Ϲ�ʽ(1)(2)(3)(4)
//@EXECUTIONFLOW:16���ظ�����4��14 10�Σ�����ÿ�ζ��ɹ���
//@EXECUTIONFLOW:15������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_Sync_0003(void)
{   
	return  OSG_iTestSync(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,CSUDI_TRUE,1);
}
//**********************************************************************************//
//******************************Begin DecodeImage Test *****************************//
//**********************************************************************************//
//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: ���Դ���ӿڲ���Ϊ�Ƿ�ֵ
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@INPUT:1������ͼƬ�������ַ���ΪCSUDI_NULL
//@INPUT:2��ͼƬ�����ڴ��ַΪCSUDI_NULL
//@INPUT:3��ͼƬ���ݴ�СΪ0
//@EXPECTATION:����ÿ���Ƿ�ֵ������CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 
//@EXECUTIONFLOW:1������CSUDIOSGDecodeImage(CSUDI_NULL,CSUDI_NULL,0,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,CSUDI_NULL)���������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage(CSUDI_NULL,CSUDI_NULL,0,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,�Ϸ�ָ��),�������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:3������CSUDIOSGDecodeImage(CSUDI_NULL,CSUDI_NULL,10�EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,�Ϸ�ָ��,�������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4������CSUDIOSGDecodeImage(CSUDI_NULL,�Ϸ�ͼƬ���ݵ�ַ,ͼƬ���ݳ���,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,�Ϸ�ָ��),�������CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5������CSUDIOSGDecodeImage("IFRAME",CSUDI_NULL,10,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,�Ϸ�ָ��),�������CSUDIOSG_ERROR_BAD_PARAMETE
//@EXECUTIONFLOW:6������CSUDIOSGDecodeImage("BMP",CSUDI_NULL,10,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,�Ϸ�ָ��),�������CSUDIOSG_ERROR_BAD_PARAMETE
//@EXECUTIONFLOW:7������CSUDIOSGDecodeImage("JPG",CSUDI_NULL,10,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,�Ϸ�ָ��),�������CSUDIOSG_ERROR_BAD_PARAMETE
//@EXECUTIONFLOW:8������CSUDIOSGDecodeImage("PNG",CSUDI_NULL,10,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,�Ϸ�ָ��),�������CSUDIOSG_ERROR_BAD_PARAMETE
//@EXECUTIONFLOW:9������CSUDIOSGDecodeImage("PNG",�Ϸ�ͼƬ���ݵ�ַ,ͼƬ���ݳ���,EM_UDIOSG_SCALE_OPTIMUM,CSUDI_NULL,�Ϸ�ָ��),�������CSUDIOSG_ERROR_BAD_PARAMETE
//@EXECUTIONFLOW:10������CSUDIOSGDecodeImage("PNG",�Ϸ�ͼƬ���ݵ�ַ,ͼƬ���ݳ���,EM_UDIOSG_SCALE_FIXSIZE,CSUDI_NULL,�Ϸ�ָ��),�������CSUDIOSG_ERROR_BAD_PARAMETE
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0001(void)
{  
	CSUDI_HANDLE hSurface = CSUDI_NULL;
	char * pcBuff = "Image";

	CSTK_ASSERT_TRUE(
					CSUDIOSG_ERROR_BAD_PARAMETER == CSUDIOSGDecodeImage(CSUDI_NULL,CSUDI_NULL,0,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,CSUDI_NULL),
					"Step 1 failure"
				);
	CSTK_ASSERT_TRUE(
					CSUDIOSG_ERROR_BAD_PARAMETER == CSUDIOSGDecodeImage(CSUDI_NULL,CSUDI_NULL,0,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,&hSurface),
					"Step 2 failure"
				);

	CSTK_ASSERT_TRUE(
					CSUDIOSG_ERROR_BAD_PARAMETER == CSUDIOSGDecodeImage(CSUDI_NULL,CSUDI_NULL,10,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,&hSurface),
					"Step 3 failure"
				);
	CSTK_ASSERT_TRUE(
					CSUDIOSG_ERROR_BAD_PARAMETER == CSUDIOSGDecodeImage(CSUDI_NULL,pcBuff,10,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,&hSurface),
					"Step 4 failure"
				);
	CSTK_ASSERT_TRUE(
					CSUDIOSG_ERROR_BAD_PARAMETER == CSUDIOSGDecodeImage("IFRAME",CSUDI_NULL,10,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,&hSurface),
					"Step 5 failure"
				);
	CSTK_ASSERT_TRUE(
					CSUDIOSG_ERROR_BAD_PARAMETER == CSUDIOSGDecodeImage("BMP",CSUDI_NULL,10,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,&hSurface),
					"Step 6 failure"
				);
	CSTK_ASSERT_TRUE(
					CSUDIOSG_ERROR_BAD_PARAMETER == CSUDIOSGDecodeImage("JPG",CSUDI_NULL,10,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,&hSurface),
					"Step 7 failure"
				);
	CSTK_ASSERT_TRUE(
					CSUDIOSG_ERROR_BAD_PARAMETER == CSUDIOSGDecodeImage("PNG",CSUDI_NULL,10,EM_UDIOSG_SCALE_OPTIMUM,CSUDI_NULL,&hSurface),
					"Step 8 failure"
				);
	CSTK_ASSERT_TRUE(
					CSUDIOSG_ERROR_BAD_PARAMETER == CSUDIOSGDecodeImage("PNG",pcBuff,10,EM_UDIOSG_SCALE_OPTIMUM,CSUDI_NULL,&hSurface),
					"Step 9 failure"
				);
	CSTK_ASSERT_TRUE(
					CSUDIOSG_ERROR_BAD_PARAMETER == CSUDIOSGDecodeImage("PNG",pcBuff,10,EM_UDIOSG_SCALE_FIXSIZE,CSUDI_NULL,&hSurface),
					"Step 10 failure"
				);
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: ���Խ���ͼƬ����ΪI֡720*576��ʽʱ�Ľ���,���ģʽΪEM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2���п��е���Ƶ������
//@PRECONDITION: 3��ƽ̨֧��I֡���͵�ͼƬ����
//@INPUT:1������ͼƬ�������ַ���Ϊ"IFRAME"
//@INPUT:2��ͼƬUDI2OSG_720_576.mpg�����ݵ�ַ
//@INPUT:3��ͼƬUDI2OSG_720_576.mpg�ĳ���
//@EXPECTATION: 1���ܹ��ɹ���ͼƬ���н��룬����CSUDI_SUCCESS��
//@EXPECTATION: 2������õ���surface�����ܹ��ɹ�blit���Դ������
//@REMARK: 
//@EXECUTIONFLOW:1����ȡUDI2OSG_720_576.mpgͼƬ���ݵ��ڴ��ַ�ͳ���
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDI_SUCCESS������õ���phSurfaceӦ�ò�ΪCSUDI_NULL
//@EXECUTIONFLOW:3������CSUDIOSGGetDisplaySurface�ӿڻ�ȡ�Դ�ռ�hDisSurface���
//@EXECUTIONFLOW:4������CSUDIOSGBlit�ӿڽ�����õ���ͼƬ��Ϣblit���Դ�ռ�hDisSurface�ϣ�blit�ľ�������ΪCSUDI_NULL��
//@EXECUTIONFLOW:5������CSUDIOSGSync���ͼƬ��blit�������ɹ�����CSUDI_SUCCESS��
//@EXECUTIONFLOW:6����5�����֮��Ӧ������Ļ�Ͽ���UDI2OSG_720_576.mpgͼƬ��Ч������������������������ʧ�ܡ�
//@EXECUTIONFLOW:7���ͷŵ�UDI2OSG_720_576.mpgռ�õ��ڴ档
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0002(void)
{  
	return OSG_iTestDecImage(EM_OSG_DECIMAGE_IFRAME,EM_IFRAME_720_576);
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: ���Խ���ͼƬ����ΪI֡1280*720��ʽʱ�Ľ���,���ģʽΪEM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2���п��е���Ƶ������
//@PRECONDITION: 3��ƽ̨֧��I֡���͵�ͼƬ����
//@INPUT:1������ͼƬ�������ַ���Ϊ"IFRAME"
//@INPUT:2��ͼƬUDI2OSG_1280_720.mpg�����ݵ�ַ
//@INPUT:3��ͼƬUDI2OSG_1280_720.mpg�ĳ���
//@EXPECTATION: 1���ܹ��ɹ���ͼƬ���н��룬����CSUDI_SUCCESS��
//@EXPECTATION: 2������õ���surface�����ܹ��ɹ�blit���Դ������
//@REMARK: 
//@EXECUTIONFLOW:1����ȡUDI2OSG_1280_720.mpgͼƬ���ݵ��ڴ��ַ�ͳ���
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDI_SUCCESS������õ���phSurfaceӦ�ò�ΪCSUDI_NULL
//@EXECUTIONFLOW:3������CSUDIOSGGetDisplaySurface�ӿڻ�ȡ�Դ�ռ�hDisSurface���
//@EXECUTIONFLOW:4������CSUDIOSGBlit�ӿڽ�����õ���ͼƬ��Ϣblit���Դ�ռ�hDisSurface�ϣ�bilt�ľ�������ΪCSUDI_NULL��
//@EXECUTIONFLOW:5������CSUDIOSGSync���ͼƬ��blit�������ɹ�����CSUDI_SUCCESS��
//@EXECUTIONFLOW:6����5�����֮��Ӧ������Ļ�Ͽ���UDI2OSG_1280_720.mpgͼƬ��Ч������������������������ʧ�ܡ�
//@EXECUTIONFLOW:7���ͷŵ�UDI2OSG_1280_720.mpgռ�õ��ڴ档
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0003(void)
{  
	return OSG_iTestDecImage(EM_OSG_DECIMAGE_IFRAME,EM_IFRAME_1280_720);
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: ���Խ���ͼƬ����ΪI֡1920*1080��ʽʱ�Ľ���,���ģʽΪEM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2���п��е���Ƶ������
//@PRECONDITION: 3��ƽ̨֧��I֡���͵�ͼƬ����
//@INPUT:1������ͼƬ�������ַ���Ϊ"IFRAME"
//@INPUT:2��ͼƬUDI2OSG_1920_1080.mpg�����ݵ�ַ
//@INPUT:3��ͼƬUDI2OSG_1920_1080.mpg�ĳ���
//@EXPECTATION: 1���ܹ��ɹ���ͼƬ���н��룬����CSUDI_SUCCESS��
//@EXPECTATION: 2������õ���surface�����ܹ��ɹ�blit���Դ������
//@REMARK: 
//@EXECUTIONFLOW:1����ȡUDI2OSG_1920_1080.mpgͼƬ���ݵ��ڴ��ַ�ͳ���
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDI_SUCCESS������õ���phSurfaceӦ�ò�ΪCSUDI_NULL
//@EXECUTIONFLOW:3������CSUDIOSGGetDisplaySurface�ӿڻ�ȡ�Դ�ռ�hDisSurface���
//@EXECUTIONFLOW:4������CSUDIOSGBlit�ӿڽ�����õ���ͼƬ��Ϣblit���Դ�ռ�hDisSurface�ϣ�bilt�ľ�������ΪCSUDI_NULL��
//@EXECUTIONFLOW:5������CSUDIOSGSync���ͼƬ��blit�������ɹ�����CSUDI_SUCCESS��
//@EXECUTIONFLOW:6����5�����֮��Ӧ������Ļ�Ͽ���UDI2OSG_1920_1080.mpgͼƬ��Ч������������������������ʧ�ܡ�
//@EXECUTIONFLOW:7���ͷŵ�UDI2OSG_1920_1080.mpgռ�õ��ڴ档
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0004(void)
{  
	return OSG_iTestDecImage(EM_OSG_DECIMAGE_IFRAME,EM_IFRAME_1920_1080);
}


//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: ���Խ���ͼƬ����Ϊbmp720*576��ʽʱ�Ľ���,���ģʽΪEM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2���п��е���Ƶ������
//@PRECONDITION: 3��ƽ̨֧��bmp���͵�ͼƬ����
//@INPUT:1������ͼƬ�������ַ���Ϊ"BMP"
//@INPUT:2��ͼƬUDI2OSG_720_576.bmp�����ݵ�ַ
//@INPUT:3��ͼƬUDI2OSG_720_576.bmp�ĳ���
//@EXPECTATION: 1���ܹ��ɹ���ͼƬ���н��룬����CSUDI_SUCCESS��
//@EXPECTATION: 2������õ���surface�����ܹ��ɹ�blit���Դ������
//@REMARK: 
//@EXECUTIONFLOW:1����ȡUDI2OSG_720_576.bmpͼƬ���ݵ��ڴ��ַ�ͳ���
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDI_SUCCESS������õ���phSurfaceӦ�ò�ΪCSUDI_NULL
//@EXECUTIONFLOW:3������CSUDIOSGGetDisplaySurface�ӿڻ�ȡ�Դ�ռ�hDisSurface���
//@EXECUTIONFLOW:4������CSUDIOSGBlit�ӿڽ�����õ���ͼƬ��Ϣblit���Դ�ռ�hDisSurface�ϣ�bilt�ľ�������ΪCSUDI_NULL��
//@EXECUTIONFLOW:5������CSUDIOSGSync���ͼƬ��blit�������ɹ�����CSUDI_SUCCESS��
//@EXECUTIONFLOW:6����5�����֮��Ӧ������Ļ�Ͽ���UDI2OSG_720_576.bmpͼƬ��Ч������������������������ʧ�ܡ�
//@EXECUTIONFLOW:7���ͷŵ�UDI2OSG_720_576.bmpռ�õ��ڴ档
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0005(void)
{  
	return OSG_iTestDecImage(EM_OSG_DECIMAGE_BMP,EM_BMP_720_576);
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: ���Խ���ͼƬ����Ϊbmp1280*720��ʽʱ�Ľ���,���ģʽΪEM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2���п��е���Ƶ������
//@PRECONDITION: 3��ƽ̨֧��bmp���͵�ͼƬ����
//@INPUT:1������ͼƬ�������ַ���Ϊ"BMP"
//@INPUT:2��ͼƬUDI2OSG_1280_720.bmp�����ݵ�ַ
//@INPUT:3��ͼƬUDI2OSG_1280_720.bmp�ĳ���
//@EXPECTATION: 1���ܹ��ɹ���ͼƬ���н��룬����CSUDI_SUCCESS��
//@EXPECTATION: 2������õ���surface�����ܹ��ɹ�blit���Դ������
//@REMARK: 
//@EXECUTIONFLOW:1����ȡUDI2OSG_1280_720.bmpͼƬ���ݵ��ڴ��ַ�ͳ���
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDI_SUCCESS������õ���phSurfaceӦ�ò�ΪCSUDI_NULL
//@EXECUTIONFLOW:3������CSUDIOSGGetDisplaySurface�ӿڻ�ȡ�Դ�ռ�hDisSurface���
//@EXECUTIONFLOW:4������CSUDIOSGBlit�ӿڽ�����õ���ͼƬ��Ϣblit���Դ�ռ�hDisSurface�ϣ�bilt�ľ�������ΪCSUDI_NULL��
//@EXECUTIONFLOW:5������CSUDIOSGSync���ͼƬ��blit�������ɹ�����CSUDI_SUCCESS��
//@EXECUTIONFLOW:6����5�����֮��Ӧ������Ļ�Ͽ���UDI2OSG_1280_720.bmpͼƬ��Ч������������������������ʧ�ܡ�
//@EXECUTIONFLOW:7���ͷŵ�UDI2OSG_1280_720.bmpռ�õ��ڴ档
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0006(void)
{ 
	return OSG_iTestDecImage(EM_OSG_DECIMAGE_BMP,EM_BMP_1280_720);
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: ���Խ���ͼƬ����Ϊbmp1920*1080��ʽʱ�Ľ���,���ģʽΪEM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2���п��е���Ƶ������
//@PRECONDITION: 3��ƽ̨֧��bmp���͵�ͼƬ����
//@INPUT:1������ͼƬ�������ַ���Ϊ"BMP"
//@INPUT:2��ͼƬUDI2OSG_1920_1080.bmp�����ݵ�ַ
//@INPUT:3��ͼƬUDI2OSG_1920_1080.bmp�ĳ���
//@EXPECTATION: 1���ܹ��ɹ���ͼƬ���н��룬����CSUDI_SUCCESS��
//@EXPECTATION: 2������õ���surface�����ܹ��ɹ�blit���Դ������
//@REMARK: 
//@EXECUTIONFLOW:1����ȡUDI2OSG_1920_1080.bmpͼƬ���ݵ��ڴ��ַ�ͳ���
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDI_SUCCESS������õ���phSurfaceӦ�ò�ΪCSUDI_NULL
//@EXECUTIONFLOW:3������CSUDIOSGGetDisplaySurface�ӿڻ�ȡ�Դ�ռ�hDisSurface���
//@EXECUTIONFLOW:4������CSUDIOSGBlit�ӿڽ�����õ���ͼƬ��Ϣblit���Դ�ռ�hDisSurface�ϣ�bilt�ľ�������ΪCSUDI_NULL��
//@EXECUTIONFLOW:5������CSUDIOSGSync���ͼƬ��blit�������ɹ�����CSUDI_SUCCESS��
//@EXECUTIONFLOW:6����5�����֮��Ӧ������Ļ�Ͽ���UDI2OSG_1920_1080.bmpͼƬ��Ч������������������������ʧ�ܡ�
//@EXECUTIONFLOW:7���ͷŵ�UDI2OSG_1920_1080.bmpռ�õ��ڴ档
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0007(void)
{  
	return OSG_iTestDecImage(EM_OSG_DECIMAGE_BMP,EM_BMP_1920_1080);
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: ���Խ���ͼƬ����Ϊjpg720*576��ʽʱ�Ľ���,���ģʽΪEM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2���п��е���Ƶ������
//@PRECONDITION: 3��ƽ̨֧��jpg���͵�ͼƬ����
//@INPUT:1������ͼƬ�������ַ���Ϊ"JPG"
//@INPUT:2��ͼƬUDI2OSG_720_576.jpg�����ݵ�ַ
//@INPUT:3��ͼƬUDI2OSG_720_576.jpg�ĳ���
//@EXPECTATION: 1���ܹ��ɹ���ͼƬ���н��룬����CSUDI_SUCCESS��
//@EXPECTATION: 2������õ���surface�����ܹ��ɹ�blit���Դ������
//@REMARK: 
//@EXECUTIONFLOW:1����ȡUDI2OSG_720_576.jpgͼƬ���ݵ��ڴ��ַ�ͳ���
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDI_SUCCESS������õ���phSurfaceӦ�ò�ΪCSUDI_NULL
//@EXECUTIONFLOW:3������CSUDIOSGGetDisplaySurface�ӿڻ�ȡ�Դ�ռ�hDisSurface���
//@EXECUTIONFLOW:4������CSUDIOSGBlit�ӿڽ�����õ���ͼƬ��Ϣblit���Դ�ռ�hDisSurface�ϣ�bilt�ľ�������ΪCSUDI_NULL��
//@EXECUTIONFLOW:5������CSUDIOSGSync���ͼƬ��blit�������ɹ�����CSUDI_SUCCESS��
//@EXECUTIONFLOW:6����5�����֮��Ӧ������Ļ�Ͽ���UDI2OSG_720_576.jpgͼƬ��Ч������������������������ʧ�ܡ�
//@EXECUTIONFLOW:7���ͷŵ�UDI2OSG_720_576.jpgռ�õ��ڴ档
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0008(void)
{  
	return OSG_iTestDecImage(EM_OSG_DECIMAGE_JPG,EM_JPG_720_576);
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: ���Խ���ͼƬ����Ϊjpg1280*720��ʽʱ�Ľ���,���ģʽΪEM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2���п��е���Ƶ������
//@PRECONDITION: 3��ƽ̨֧��jpg���͵�ͼƬ����
//@INPUT:1������ͼƬ�������ַ���Ϊ"JPG"
//@INPUT:2��ͼƬUDI2OSG_1280_720.jpg�����ݵ�ַ
//@INPUT:3��ͼƬUDI2OSG_1280_720.jpg�ĳ���
//@EXPECTATION: 1���ܹ��ɹ���ͼƬ���н��룬����CSUDI_SUCCESS��
//@EXPECTATION: 2������õ���surface�����ܹ��ɹ�blit���Դ������
//@REMARK: 
//@EXECUTIONFLOW:1����ȡUDI2OSG_1280_720.jpgͼƬ���ݵ��ڴ��ַ�ͳ���
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDI_SUCCESS������õ���phSurfaceӦ�ò�ΪCSUDI_NULL
//@EXECUTIONFLOW:3������CSUDIOSGGetDisplaySurface�ӿڻ�ȡ�Դ�ռ�hDisSurface���
//@EXECUTIONFLOW:4������CSUDIOSGBlit�ӿڽ�����õ���ͼƬ��Ϣblit���Դ�ռ�hDisSurface�ϣ�bilt�ľ�������ΪCSUDI_NULL��
//@EXECUTIONFLOW:5������CSUDIOSGSync���ͼƬ��blit�������ɹ�����CSUDI_SUCCESS��
//@EXECUTIONFLOW:6����5�����֮��Ӧ������Ļ�Ͽ���UDI2OSG_1280_720.jpgͼƬ��Ч������������������������ʧ�ܡ�
//@EXECUTIONFLOW:7���ͷŵ�UDI2OSG_1280_720.jpgռ�õ��ڴ档
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0009(void)
{  
	return OSG_iTestDecImage(EM_OSG_DECIMAGE_JPG,EM_JPG_1280_720);
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: ���Խ���ͼƬ����Ϊjpg1920*1080��ʽʱ�Ľ���,���ģʽΪEM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2���п��е���Ƶ������
//@PRECONDITION: 3��ƽ̨֧��jpg���͵�ͼƬ����
//@INPUT:1������ͼƬ�������ַ���Ϊ"JPG"
//@INPUT:2��ͼƬUDI2OSG_1920_1080.jpg�����ݵ�ַ
//@INPUT:3��ͼƬUDI2OSG_1920_1080.jpg�ĳ���
//@EXPECTATION: 1���ܹ��ɹ���ͼƬ���н��룬����CSUDI_SUCCESS��
//@EXPECTATION: 2������õ���surface�����ܹ��ɹ�blit���Դ������
//@REMARK: 
//@EXECUTIONFLOW:1����ȡUDI2OSG_1920_1080.jpgͼƬ���ݵ��ڴ��ַ�ͳ���
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDI_SUCCESS������õ���phSurfaceӦ�ò�ΪCSUDI_NULL
//@EXECUTIONFLOW:3������CSUDIOSGGetDisplaySurface�ӿڻ�ȡ�Դ�ռ�hDisSurface���
//@EXECUTIONFLOW:4������CSUDIOSGBlit�ӿڽ�����õ���ͼƬ��Ϣblit���Դ�ռ�hDisSurface�ϣ�bilt�ľ�������ΪCSUDI_NULL��
//@EXECUTIONFLOW:5������CSUDIOSGSync���ͼƬ��blit�������ɹ�����CSUDI_SUCCESS��
//@EXECUTIONFLOW:6����5�����֮��Ӧ������Ļ�Ͽ���UDI2OSG_1920_1080.jpgͼƬ��Ч������������������������ʧ�ܡ�
//@EXECUTIONFLOW:7���ͷŵ�UDI2OSG_1920_1080.jpgռ�õ��ڴ档
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0010(void)
{  
	return OSG_iTestDecImage(EM_OSG_DECIMAGE_JPG,EM_JPG_1920_1080);
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: ���Խ���ͼƬ����Ϊpng720*576��ʽʱ�Ľ���,���ģʽΪEM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2���п��е���Ƶ������
//@PRECONDITION: 3��ƽ̨֧��png���͵�ͼƬ����
//@INPUT:1������ͼƬ�������ַ���Ϊ"PNG"
//@INPUT:2��ͼƬUDI2OSG_720_576.png�����ݵ�ַ
//@INPUT:3��ͼƬUDI2OSG_720_576.png�ĳ���
//@EXPECTATION: 1���ܹ��ɹ���ͼƬ���н��룬����CSUDI_SUCCESS��
//@EXPECTATION: 2������õ���surface�����ܹ��ɹ�blit���Դ������
//@REMARK: 
//@EXECUTIONFLOW:1����ȡUDI2OSG_720_576.pngͼƬ���ݵ��ڴ��ַ�ͳ���
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDI_SUCCESS������õ���phSurfaceӦ�ò�ΪCSUDI_NULL
//@EXECUTIONFLOW:3������CSUDIOSGGetDisplaySurface�ӿڻ�ȡ�Դ�ռ�hDisSurface���
//@EXECUTIONFLOW:4������CSUDIOSGBlit�ӿڽ�����õ���ͼƬ��Ϣblit���Դ�ռ�hDisSurface�ϣ�bilt�ľ�������ΪCSUDI_NULL��
//@EXECUTIONFLOW:5������CSUDIOSGSync���ͼƬ��blit�������ɹ�����CSUDI_SUCCESS��
//@EXECUTIONFLOW:6����5�����֮��Ӧ������Ļ�Ͽ���UDI2OSG_720_576.pngͼƬ��Ч������������������������ʧ�ܡ�
//@EXECUTIONFLOW:7���ͷŵ�UDI2OSG_720_576.pngռ�õ��ڴ档
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0011(void)
{  
	return OSG_iTestDecImage(EM_OSG_DECIMAGE_PNG,EM_PNG_720_576);
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: ���Խ���ͼƬ����Ϊpng1280*720��ʽʱ�Ľ���,���ģʽΪEM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2���п��е���Ƶ������
//@PRECONDITION: 3��ƽ̨֧��png���͵�ͼƬ����
//@INPUT:1������ͼƬ�������ַ���Ϊ"PNG"
//@INPUT:2��ͼƬUDI2OSG_1280_720.png�����ݵ�ַ
//@INPUT:3��ͼƬUDI2OSG_1280_720.png�ĳ���
//@EXPECTATION: 1���ܹ��ɹ���ͼƬ���н��룬����CSUDI_SUCCESS��
//@EXPECTATION: 2������õ���surface�����ܹ��ɹ�blit���Դ������
//@REMARK: 
//@EXECUTIONFLOW:1����ȡUDI2OSG_1280_720.pngͼƬ���ݵ��ڴ��ַ�ͳ���
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDI_SUCCESS������õ���phSurfaceӦ�ò�ΪCSUDI_NULL
//@EXECUTIONFLOW:3������CSUDIOSGGetDisplaySurface�ӿڻ�ȡ�Դ�ռ�hDisSurface���
//@EXECUTIONFLOW:4������CSUDIOSGBlit�ӿڽ�����õ���ͼƬ��Ϣblit���Դ�ռ�hDisSurface�ϣ�bilt�ľ�������ΪCSUDI_NULL��
//@EXECUTIONFLOW:5������CSUDIOSGSync���ͼƬ��blit�������ɹ�����CSUDI_SUCCESS��
//@EXECUTIONFLOW:6����5�����֮��Ӧ������Ļ�Ͽ���UDI2OSG_1280_720.pngͼƬ��Ч������������������������ʧ�ܡ�
//@EXECUTIONFLOW:7���ͷŵ�UDI2OSG_1280_720.pngռ�õ��ڴ档
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0012(void)
{  
	return OSG_iTestDecImage(EM_OSG_DECIMAGE_PNG,EM_PNG_1280_720);
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: ���Խ���ͼƬ����Ϊpng1920*1080��ʽʱ�Ľ���,���ģʽΪEM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2���п��е���Ƶ������
//@PRECONDITION: 3��ƽ̨֧��png���͵�ͼƬ����
//@INPUT:1������ͼƬ�������ַ���Ϊ"PNG"
//@INPUT:2��ͼƬUDI2OSG_1920_1080.png�����ݵ�ַ
//@INPUT:3��ͼƬUDI2OSG_1920_1080.png�ĳ���
//@EXPECTATION: 1���ܹ��ɹ���ͼƬ���н��룬����CSUDI_SUCCESS��
//@EXPECTATION: 2������õ���surface�����ܹ��ɹ�blit���Դ������
//@REMARK: 
//@EXECUTIONFLOW:1����ȡUDI2OSG_1920_1080.pngͼƬ���ݵ��ڴ��ַ�ͳ���
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDI_SUCCESS������õ���phSurfaceӦ�ò�ΪCSUDI_NULL
//@EXECUTIONFLOW:3������CSUDIOSGGetDisplaySurface�ӿڻ�ȡ�Դ�ռ�hDisSurface���
//@EXECUTIONFLOW:4������CSUDIOSGBlit�ӿڽ�����õ���ͼƬ��Ϣblit���Դ�ռ�hDisSurface�ϣ�bilt�ľ�������ΪCSUDI_NULL��
//@EXECUTIONFLOW:5������CSUDIOSGSync���ͼƬ��blit�������ɹ�����CSUDI_SUCCESS��
//@EXECUTIONFLOW:6����5�����֮��Ӧ������Ļ�Ͽ���UDI2OSG_1920_1080.pngͼƬ��Ч������������������������ʧ�ܡ�
//@EXECUTIONFLOW:7���ͷŵ�UDI2OSG_1920_1080.pngռ�õ��ڴ档
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0013(void)
{  
	return OSG_iTestDecImage(EM_OSG_DECIMAGE_PNG,EM_PNG_1920_1080);
}


//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: ���Խ��������Ϊ�Ƿ���I֡ͼƬ����
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2���п��е���Ƶ������
//@PRECONDITION: 3��ƽ̨֧��I֡���͵�ͼƬ����
//@INPUT:1������ͼƬ�������ַ���Ϊ"IFRAME"
//@INPUT:2��ͼƬUDI2OSG_720_576_Error.mpg�����ݵ�ַ
//@INPUT:3��ͼƬUDI2OSG_720_576_Error.mpg�ĳ���
//@EXPECTATION: ͼƬ����ʧ�ܣ�����CSUDIOSG_ERROR_UNKNOWN_ERROR
//@REMARK: 
//@EXECUTIONFLOW:1����ȡUDI2OSG_720_576_Error.mpgͼƬ���ݵ��ڴ��ַ�ͳ���
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDIOSG_ERROR_UNKNOWN_ERROR
//@EXECUTIONFLOW:4���ͷŵ�UDI2OSG_720_576_Error.mpgռ�õ��ڴ档
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0014(void)
{  
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	unsigned char* pcBuff = CSUDI_NULL;
	unsigned int nImageLen = 0;
	
	nImageLen = OSG_iGetPhoto("UDI2OSG_720_576_Error.mpg",&pcBuff);
	CSTK_ASSERT_TRUE_FATAL(
				((pcBuff != CSUDI_NULL) && nImageLen != 0),
				"Step 1 get photo data failure"
			);
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS != CSUDIOSGDecodeImage("IFRAME",pcBuff,nImageLen,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,&hSurface_S)),
				"Step 2 decode image failure"
			);
	CSTK_FATAL_POINT
	if(hSurface_S != CSUDI_NULL)
	{
		if(CSUDIOSGDestroySurface(hSurface_S) != CSUDI_SUCCESS)
			CSTCPrint("Step 4 destroy decode surface failure\n");
		hSurface_S = CSUDI_NULL;
	}
	if(pcBuff != CSUDI_NULL)
	{
		if(CSUDIOSFree(pcBuff) != CSUDI_SUCCESS)
			CSTCPrint("Step 4 free photo data failure");
		CSTCPrint("[UDI2OSGEST]CSUDIOSFree success !! \r\n");
	}
	
	return CSUDI_TRUE;
}
//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: ���Խ��������Ϊ�Ƿ���JPEGͼƬ����
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2���п��е���Ƶ������
//@PRECONDITION: 3��ƽ̨֧��I֡���͵�ͼƬ����
//@INPUT:1������ͼƬ�������ַ���Ϊ"JPEG"
//@INPUT:2��ͼƬUDI2OSG_Circle_576_576_Error.jpg�����ݵ�ַ
//@INPUT:3��ͼƬUDI2OSG_Circle_576_576_Error.jpg�ĳ���
//@EXPECTATION: ͼƬ����ʧ�ܣ�����CSUDIOSG_ERROR_UNKNOWN_ERROR
//@REMARK: 
//@EXECUTIONFLOW:1����ȡUDI2OSG_Circle_576_576_Error.jpgͼƬ���ݵ��ڴ��ַ�ͳ���
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDIOSG_ERROR_UNKNOWN_ERROR
//@EXECUTIONFLOW:4���ͷŵ�UDI2OSG_Circle_576_576_Error.jpgռ�õ��ڴ�
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0015(void)
{  
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hSurface_D = CSUDI_NULL;
	unsigned char* pcBuff = CSUDI_NULL;
	unsigned int nImageLen = 0;
	
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == OSG_IsSupportDecImage(EM_OSG_DECIMAGE_JPG)), 
		"ƽ̨��֧��JPEG����ͼƬӲ����");
	
	nImageLen = OSG_iGetPhoto("UDI2OSG_Circle_576_576_Error.jpg",&pcBuff);
	CSTK_ASSERT_TRUE_FATAL(
				((pcBuff != CSUDI_NULL) && nImageLen != 0),
				"Step 1 get photo data failure"
			);
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS != CSUDIOSGDecodeImage("JPEG",pcBuff,nImageLen,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,&hSurface_S)),
				"Step 2 decode image failure"
			);
	if(hSurface_S != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(OSG_GetTestDisplaySurfaceIndex(),&hSurface_D)),
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
	}
	CSTK_FATAL_POINT
	if(hSurface_S != CSUDI_NULL)
	{
		if(CSUDIOSGDestroySurface(hSurface_S)!= CSUDI_SUCCESS)
			CSTCPrint("Step 4 destroy decode surface failure\n");
		hSurface_S = CSUDI_NULL;
	}
	if(pcBuff != CSUDI_NULL)
	{
		if(CSUDIOSFree(pcBuff) != CSUDI_SUCCESS)
			CSTCPrint("Step 4 free photo data failure");
		CSTCPrint("[UDI2OSGEST]CSUDIOSFree success !! \r\n");
	}

	OSG_iClearScreen();
	
	return CSUDI_TRUE;
}
//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: ���Խ���ͼƬ����Ϊbmp��jpg��png576*576��ʽʱ�Ľ���,���ģʽΪEM_UDIOSG_SCALE_OPTIMUM
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2���п��е���Ƶ������
//@PRECONDITION: 3��ƽ̨֧�ָ����͵�ͼƬ����
//@INPUT:1������ͼƬ�������ַ���Ϊ��Ӧ��ͼƬ����:"BMP"��"JPG"��"PNG"
//@INPUT:2��ͼƬUDI2OSG_Circle_576_576�����ݵ�ַ
//@INPUT:3��ͼƬUDI2OSG_Circle_576_576�ĳ���
//@INPUT:4���������ģʽΪEM_UDIOSG_SCALE_OPTIMUM
//@INPUT:5���������Surface�Ĵ�СΪ200*200
//@EXPECTATION: 1���ܹ��ɹ���ͼƬ���н��룬����CSUDI_SUCCESS��
//@EXPECTATION: 2������õ���surface��СӦ��Ϊ576:576�ı���
//@EXPECTATION: 3������õ���surface�����ܹ��ɹ�blit���Դ������ͼƬ����״Ӧ��Ϊ�����κ�Բ��
//@REMARK: 
//@EXECUTIONFLOW:1����ȡUDI2OSG_Circle_576_576ͼƬ���ݵ��ڴ��ַ�ͳ���
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDI_SUCCESS������õ���phSurfaceӦ�ò�ΪCSUDI_NULL
//@EXECUTIONFLOW:3������CSUDIOSGGetSurfaceInfo��ȡsurface��Ϣ����������õ���surface��СӦ��Ϊ576:576�ı���
//@EXECUTIONFLOW:4������CSUDIOSGGetDisplaySurface�ӿڻ�ȡ�Դ�ռ�hDisSurface���
//@EXECUTIONFLOW:5������CSUDIOSGBlit�ӿڽ�����õ���ͼƬ��Ϣblit���Դ�ռ�hDisSurface�ϣ�blit�ľ�������ΪCSUDI_NULL��
//@EXECUTIONFLOW:6������CSUDIOSGSync���ͼƬ��blit�������ɹ�����CSUDI_SUCCESS��
//@EXECUTIONFLOW:7����5�����֮��Ӧ������Ļ�Ͽ���UDI2OSG_Circle_576_576ͼƬ��Ч����ͼƬ����״Ӧ��Ϊ�����κ�Բ�Ρ�
//@EXECUTIONFLOW:8���ͷŵ�UDI2OSG_Circle_576_576ռ�õ��ڴ档
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0016(void)
{  
	CSUDIOSGRect_S sDstRect;
	CSUDI_BOOL bRet = CSUDI_TRUE;
	sDstRect.m_nWidth = 200;
	sDstRect.m_nHeight = 200;
	
	bRet &= OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_BMP,EM_BMP_CIRCLE_576_576,EM_UDIOSG_SCALE_OPTIMUM,&sDstRect);
	bRet &= OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_JPG,EM_JPG_CIRCLE_576_576,EM_UDIOSG_SCALE_OPTIMUM,&sDstRect);
	bRet &= OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_PNG,EM_PNG_CIRCLE_576_576,EM_UDIOSG_SCALE_OPTIMUM,&sDstRect);
	return bRet;
}
//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION:  ���Խ���ͼƬ����Ϊbmp��jpg��png576*576��ʽʱ�Ľ���,���ģʽΪEM_UDIOSG_SCALE_FIXSIZE
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2���п��е���Ƶ������
//@PRECONDITION: 3��ƽ̨֧�ָ����͵�ͼƬ����
//@INPUT:1������ͼƬ�������ַ���Ϊ��Ӧ��ͼƬ����:"BMP"��"JPG"��"PNG"
//@INPUT:2��ͼƬUDI2OSG_Circle_576_576�����ݵ�ַ
//@INPUT:3��ͼƬUDI2OSG_Circle_576_576�ĳ���
//@INPUT:4���������ģʽΪEM_UDIOSG_SCALE_OPTIMUM
//@INPUT:5���������Surface�Ĵ�СΪ350*100
//@EXPECTATION: 1���ܹ��ɹ���ͼƬ���н��룬����CSUDI_SUCCESS��
//@EXPECTATION: 2������õ���surface�����ܹ��ɹ�blit���Դ������ͼƬ����״Ӧ��Ϊ�����κ���Բ
//@REMARK: 
//@EXECUTIONFLOW:1����ȡUDI2OSG_Circle_576_576ͼƬ���ݵ��ڴ��ַ�ͳ���
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDI_SUCCESS������õ���phSurfaceӦ�ò�ΪCSUDI_NULL
//@EXECUTIONFLOW:3������CSUDIOSGGetSurfaceInfo��ȡsurface��Ϣ����������õ���surface��СӦ��Ϊ350*100
//@EXECUTIONFLOW:4������CSUDIOSGGetDisplaySurface�ӿڻ�ȡ�Դ�ռ�hDisSurface���
//@EXECUTIONFLOW:5������CSUDIOSGBlit�ӿڽ�����õ���ͼƬ��Ϣblit���Դ�ռ�hDisSurface�ϣ�blit�ľ�������ΪCSUDI_NULL��
//@EXECUTIONFLOW:6������CSUDIOSGSync���ͼƬ��blit�������ɹ�����CSUDI_SUCCESS��
//@EXECUTIONFLOW:7����5�����֮��Ӧ������Ļ�Ͽ���UDI2OSG_Circle_576_576ͼƬ��Ч����ͼƬ����״Ӧ��Ϊ�����κ���Բ
//@EXECUTIONFLOW:8���ͷŵ�UDI2OSG_Circle_576_576ռ�õ��ڴ档
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0017(void)
{  
	CSUDIOSGRect_S sDstRect;
	CSUDI_BOOL bRet = CSUDI_TRUE;
	sDstRect.m_nWidth = 350;
	sDstRect.m_nHeight = 100;

	bRet &= OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_BMP,EM_BMP_CIRCLE_576_576,EM_UDIOSG_SCALE_FIXSIZE,&sDstRect);
	bRet &= OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_JPG,EM_JPG_CIRCLE_576_576,EM_UDIOSG_SCALE_FIXSIZE,&sDstRect);
	bRet &= OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_PNG,EM_PNG_CIRCLE_576_576,EM_UDIOSG_SCALE_FIXSIZE,&sDstRect);
	return bRet;
}
//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION:  ���Խ���ͼƬ����Ϊbmp��jpg��png576*576��ʽʱ�Ľ���,���ģʽΪEM_UDIOSG_SCALE_OPTIMUM
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2���п��е���Ƶ������
//@PRECONDITION: 3��ƽ̨֧�ָ����͵�ͼƬ����
//@INPUT:1������ͼƬ�������ַ���Ϊ��Ӧ��ͼƬ����:"BMP"��"JPG"��"PNG"
//@INPUT:2��ͼƬUDI2OSG_Circle_576_576�����ݵ�ַ
//@INPUT:3��ͼƬUDI2OSG_Circle_576_576�ĳ���
//@INPUT:4���������ģʽΪEM_UDIOSG_SCALE_OPTIMUM
//@INPUT:5���������Surface�Ĵ�СΪ350*100
//@EXPECTATION: 1���ܹ��ɹ���ͼƬ���н��룬����CSUDI_SUCCESS��
//@EXPECTATION: 2������õ���surface�����ܹ��ɹ�blit���Դ������ͼƬ����״Ӧ��Ϊ�����κ���Բ
//@REMARK: 
//@EXECUTIONFLOW:1����ȡUDI2OSG_Circle_576_576ͼƬ���ݵ��ڴ��ַ�ͳ���
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDI_SUCCESS������õ���phSurfaceӦ�ò�ΪCSUDI_NULL
//@EXECUTIONFLOW:3������CSUDIOSGGetSurfaceInfo��ȡsurface��Ϣ����������õ���surface��СӦ��Ϊ350*100
//@EXECUTIONFLOW:4������CSUDIOSGGetDisplaySurface�ӿڻ�ȡ�Դ�ռ�hDisSurface���
//@EXECUTIONFLOW:5������CSUDIOSGBlit�ӿڽ�����õ���ͼƬ��Ϣblit���Դ�ռ�hDisSurface�ϣ�blit�ľ�������ΪCSUDI_NULL��
//@EXECUTIONFLOW:6������CSUDIOSGSync���ͼƬ��blit�������ɹ�����CSUDI_SUCCESS��
//@EXECUTIONFLOW:7����5�����֮��Ӧ������Ļ�Ͽ���UDI2OSG_Circle_576_576ͼƬ��Ч����ͼƬ����״Ӧ��Ϊ�����κ���Բ,���С��350*100�ľ�����
//@EXECUTIONFLOW:8���ͷŵ�UDI2OSG_Circle_576_576ռ�õ��ڴ档
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0018(void)
{  
	CSUDIOSGRect_S sDstRect;
	CSUDI_BOOL bRet = CSUDI_TRUE;
	sDstRect.m_nWidth = 350;
	sDstRect.m_nHeight = 100;

	bRet &= OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_BMP,EM_BMP_CIRCLE_576_576,EM_UDIOSG_SCALE_OPTIMUM,&sDstRect);
	bRet &= OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_JPG,EM_JPG_CIRCLE_576_576,EM_UDIOSG_SCALE_OPTIMUM,&sDstRect);
	bRet &= OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_PNG,EM_PNG_CIRCLE_576_576,EM_UDIOSG_SCALE_OPTIMUM,&sDstRect);
	return bRet;
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION:  ���Խ���ͼƬ����Ϊjpg,��jpg������SOF������SOF��ˮƽ�ʹ�ֱ���ݲ�����Ϊ1:2������³�������
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��ƽ̨֧�ָ����͵�ͼƬ����
//@INPUT:1������ͼƬ�������ַ���Ϊ��Ӧ��ͼƬ����:"JPG"
//@INPUT:2��ͼƬEM_JPG_DOUBLE_SOF,EM_JPG_SOF_Y_SAMPLE_12,EM_PNG_CIRCLE_576_576 �����ݵ�ַ
//@INPUT:3��ͼƬEM_JPG_DOUBLE_SOF,EM_JPG_SOF_Y_SAMPLE_12,EM_PNG_CIRCLE_576_576�ĳ���
//@INPUT:4���������ģʽΪEM_UDIOSG_SCALE_FULLSIZE
//@EXPECTATION: 1�����֧��EM_JPG_DOUBLE_SOF��EM_JPG_SOF_Y_SAMPLE_12����ͼƬ�Ľ�����ͼƬӦ����ʾ����
//@EXPECTATION: 2�������֧��EM_JPG_DOUBLE_SOF��EM_JPG_SOF_Y_SAMPLE_12����ͼƬ�Ľ����򲻳���Ӧ������,Ҳ����Ӱ�����ͼƬ���������
//@REMARK: 
//@EXECUTIONFLOW:1����ȡEM_JPG_DOUBLE_SOF��ͼƬ����
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage����ͼƬ,����ӿڷ���success ������blit����������ϣ�������ʾ
//@EXECUTIONFLOW:3���������ɹ������CSUDIOSGDestroySurface�ͷ�surface
//@EXECUTIONFLOW:4����ȡEM_JPG_SOF_Y_SAMPLE_12��ͼƬ����
//@EXECUTIONFLOW:5������CSUDIOSGDecodeImage����ͼƬ,����ӿڷ���success ������blit����������ϣ�������ʾ
//@EXECUTIONFLOW:6���������ɹ������CSUDIOSGDestroySurface�ͷ�surface
//@EXECUTIONFLOW:7����ȡEM_PNG_CIRCLE_576_576��ͼƬ����
//@EXECUTIONFLOW:8������CSUDIOSGDecodeImage����ͼƬ��������CSUDI_SUCCESS,����ͼƬ��ʾ����
//@EXECUTIONFLOW:9���������ɹ������CSUDIOSGDestroySurface�ͷ�surface
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0019(void)
{
	return OSG_iTestDecImageSeveralJpg();
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION:  ���Խ���ͼƬ����Ϊbmp��jpg��png576*576��ʽʱ�Ľ���,���ģʽΪEM_UDIOSG_SCALE_OPTIMUM
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2���п��е���Ƶ������
//@PRECONDITION: 3��ƽ̨֧�ָ����͵�ͼƬ����
//@INPUT:1������ͼƬ�������ַ���Ϊ��Ӧ��ͼƬ����:"BMP"��"JPG"��"PNG"
//@INPUT:2��ͼƬUDI2OSG_Circle_576_576�����ݵ�ַ
//@INPUT:3��ͼƬUDI2OSG_Circle_576_576�ĳ���
//@INPUT:4���������ģʽΪEM_UDIOSG_SCALE_OPTIMUM
//@INPUT:5���������Surface�Ĵ�СΪ500*500
//@EXPECTATION: 1���ܹ��ɹ���ͼƬ���н��룬����CSUDI_SUCCESS��
//@EXPECTATION: 2������õ���surface�����ܹ��ɹ�blit���Դ������ͼƬ����״Ӧ��Ϊ�����κ���Բ
//@REMARK: 
//@EXECUTIONFLOW:1����ȡUDI2OSG_Circle_576_576ͼƬ���ݵ��ڴ��ַ�ͳ���
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDI_SUCCESS������õ���phSurfaceӦ�ò�ΪCSUDI_NULL
//@EXECUTIONFLOW:3������CSUDIOSGGetSurfaceInfo��ȡsurface��Ϣ����������õ���surface��СӦ��Ϊ500*500
//@EXECUTIONFLOW:4������CSUDIOSGGetDisplaySurface�ӿڻ�ȡ�Դ�ռ�hDisSurface���
//@EXECUTIONFLOW:5������CSUDIOSGBlit�ӿڽ�����õ���ͼƬ��Ϣblit���Դ�ռ�hDisSurface�ϣ�blit�ľ�������ΪCSUDI_NULL��
//@EXECUTIONFLOW:6������CSUDIOSGSync���ͼƬ��blit�������ɹ�����CSUDI_SUCCESS��
//@EXECUTIONFLOW:7����5�����֮��Ӧ������Ļ�Ͽ���UDI2OSG_Circle_576_576ͼƬ��Ч����ͼƬ����״Ӧ��Ϊ�����κ���Բ,���С��500*500�ľ�����
//@EXECUTIONFLOW:8���ͷŵ�UDI2OSG_Circle_576_576ռ�õ��ڴ档
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0020(void)
{  
	CSUDIOSGRect_S sDstRect;
	CSUDI_BOOL bRet = CSUDI_TRUE;
	
	sDstRect.m_nWidth = 500;
	sDstRect.m_nHeight = 500;
	
	bRet &= OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_BMP,EM_BMP_CIRCLE_576_576,EM_UDIOSG_SCALE_OPTIMUM,&sDstRect);
	bRet &= OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_JPG,EM_JPG_CIRCLE_576_576,EM_UDIOSG_SCALE_OPTIMUM,&sDstRect);
	bRet &= OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_PNG,EM_PNG_CIRCLE_576_576,EM_UDIOSG_SCALE_OPTIMUM,&sDstRect);

	return bRet;
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION:  ���Խ���ͼƬ����Ϊjpg 8192*8192��ʽʱ�Ľ���,���ģʽΪEM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2���п��е���Ƶ������
//@PRECONDITION: 3��ƽ̨֧�ָ����͵�ͼƬ����
//@INPUT:1������ͼƬ�������ַ���Ϊ��Ӧ��ͼƬ����:"JPG"
//@INPUT:2��ͼƬEM_JPG_8192_8192�����ݵ�ַ
//@INPUT:3��ͼƬEM_JPG_8192_8192�ĳ���
//@INPUT:4���������ģʽΪEM_UDIOSG_SCALE_FULLSIZE
//@INPUT:5���������Surface�Ĵ�СΪ8192*8192
//@EXPECTATION: 1���ܹ��ɹ���ͼƬ���н��룬����CSUDI_SUCCESS��
//@EXPECTATION: 2������õ���surface�����ܹ��ɹ�blit���Դ������ͼƬ����״Ӧ��Ϊ�����κ���Բ
//@REMARK: 
//@EXECUTIONFLOW:1����ȡEM_JPG_8192_8192ͼƬ���ݵ��ڴ��ַ�ͳ���
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDI_SUCCESS������õ���phSurfaceӦ�ò�ΪCSUDI_NULL
//@EXECUTIONFLOW:3������CSUDIOSGGetSurfaceInfo��ȡsurface��Ϣ����������õ���surface��СӦ��Ϊ8192*8192
//@EXECUTIONFLOW:4������CSUDIOSGGetDisplaySurface�ӿڻ�ȡ�Դ�ռ�hDisSurface���
//@EXECUTIONFLOW:5������CSUDIOSGBlit�ӿڽ�����õ���ͼƬ��Ϣblit���Դ�ռ�hDisSurface�ϣ�blit�ľ�������ΪCSUDI_NULL��
//@EXECUTIONFLOW:6������CSUDIOSGSync���ͼƬ��blit�������ɹ�����CSUDI_SUCCESS��
//@EXECUTIONFLOW:7����5�����֮��Ӧ������Ļ�Ͽ���EM_JPG_8192_8192ͼƬ��Ч����ͼƬ����״Ӧ��Ϊ�����κ���Բ,���С��8192*8192�ľ�����
//@EXECUTIONFLOW:8���ͷŵ�EM_JPG_8192_8192ռ�õ��ڴ档
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0021(void)
{  
	CSUDIOSGRect_S sDstRect;
	CSUDI_BOOL bRet = CSUDI_TRUE;
	sDstRect.m_nWidth = 100;
	sDstRect.m_nHeight = 100;
	
	bRet &= OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_JPG,EM_JPG_8192_8192,EM_UDIOSG_SCALE_FULLSIZE,&sDstRect);

	return bRet;
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: ���Խ���ͼƬ����ΪIFRAME��JPG 720*576��ʽʱ�Ľ���,���ģʽΪEM_UDIOSG_SCALE_OPTIMUM
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2���п��е���Ƶ������
//@PRECONDITION: 3��ƽ̨֧�ָ����͵�ͼƬ����
//@INPUT:1������ͼƬ�������ַ���Ϊ��Ӧ��ͼƬ����:"IFRAME"
//@INPUT:2��ͼƬEM_IFRAME_720_576�����ݵ�ַ
//@INPUT:3��ͼƬEM_IFRAME_720_576�ĳ���
//@INPUT:4���������ģʽΪEM_UDIOSG_SCALE_OPTIMUM
//@INPUT:5���������Surface�Ĵ�С��ʼֵΪ{600��300}
//@EXPECTATION: 1���ܹ��ɹ���ͼƬ���н��룬����CSUDI_SUCCESS��
//@EXPECTATION: 2������õ���surface�ĸ߿�Ӧ��Ϊ{720*300/576��300}
//@EXPECTATION: 3������õ���surface�����ܹ��ɹ�blit���Դ����
//@REMARK: 
//@EXECUTIONFLOW:1����ȡEM_IFRAME_720_576ͼƬ���ݵ��ڴ��ַ�ͳ���
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDI_SUCCESS������õ���phSurfaceӦ�ò�ΪCSUDI_NULL
//@EXECUTIONFLOW:3������CSUDIOSGGetSurfaceInfo��ȡsurface��Ϣ����������õ���surface��СӦ��Ϊ{375��300}�ı���
//@EXECUTIONFLOW:4������CSUDIOSGGetDisplaySurface�ӿڻ�ȡ�Դ�ռ�hDisSurface���
//@EXECUTIONFLOW:5������CSUDIOSGBlit�ӿڽ�����õ���ͼƬ��Ϣblit���Դ�ռ�hDisSurface�ϣ�blit�ľ�������ΪCSUDI_NULL��
//@EXECUTIONFLOW:6������CSUDIOSGSync���ͼƬ��blit�������ɹ�����CSUDI_SUCCESS��
//@EXECUTIONFLOW:7����5�����֮��Ӧ������Ļ�Ͽ���EM_IFRAME_720_576ͼƬ��Ч����
//@EXECUTIONFLOW:8���ͷŵ�EM_IFRAME_720_576ռ�õ��ڴ档
//@EXECUTIONFLOW:9����EM_JPG_720_576ͼƬ�ظ����ϲ��衣
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0022(void)
{  
	CSUDIOSGRect_S sDstRect;
	CSUDI_BOOL bRet = CSUDI_TRUE;

	sDstRect.m_nWidth = 600;
	sDstRect.m_nHeight = 300;
	bRet = OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_IFRAME,EM_IFRAME_720_576,EM_UDIOSG_SCALE_OPTIMUM,&sDstRect);
	CSTK_ASSERT_TRUE_FATAL((bRet == CSUDI_TRUE),"����MPEGʧ��!");
		
	bRet = OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_JPG,EM_JPG_720_576,EM_UDIOSG_SCALE_OPTIMUM,&sDstRect);
	CSTK_ASSERT_TRUE_FATAL((bRet == CSUDI_TRUE),"����JPGʧ��!");
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: ���Խ���ͼƬ����ΪIFRAME��JPG 720*576��ʽʱ�Ľ���,���ģʽΪEM_UDIOSG_SCALE_OPTIMUM
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2���п��е���Ƶ������
//@PRECONDITION: 3��ƽ̨֧�ָ����͵�ͼƬ����
//@INPUT:1������ͼƬ�������ַ���Ϊ��Ӧ��ͼƬ����:"IFRAME"
//@INPUT:2��ͼƬEM_IFRAME_720_576�����ݵ�ַ
//@INPUT:3��ͼƬEM_IFRAME_720_576�ĳ���
//@INPUT:4���������ģʽΪEM_UDIOSG_SCALE_OPTIMUM
//@INPUT:5���������Surface�Ĵ�С��ʼֵΪ{500��500}
//@EXPECTATION: 1���ܹ��ɹ���ͼƬ���н��룬����CSUDI_SUCCESS��
//@EXPECTATION: 2������õ���surface�ĸ߿�Ӧ��Ϊ{400*576/720��400}
//@EXPECTATION: 3������õ���surface�����ܹ��ɹ�blit���Դ����
//@REMARK: 
//@EXECUTIONFLOW:1����ȡEM_IFRAME_720_576ͼƬ���ݵ��ڴ��ַ�ͳ���
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDI_SUCCESS������õ���phSurfaceӦ�ò�ΪCSUDI_NULL
//@EXECUTIONFLOW:3������CSUDIOSGGetSurfaceInfo��ȡsurface��Ϣ����������õ���surface��СӦ��Ϊ{500��400}�ı���
//@EXECUTIONFLOW:4������CSUDIOSGGetDisplaySurface�ӿڻ�ȡ�Դ�ռ�hDisSurface���
//@EXECUTIONFLOW:5������CSUDIOSGBlit�ӿڽ�����õ���ͼƬ��Ϣblit���Դ�ռ�hDisSurface�ϣ�blit�ľ�������ΪCSUDI_NULL��
//@EXECUTIONFLOW:6������CSUDIOSGSync���ͼƬ��blit�������ɹ�����CSUDI_SUCCESS��
//@EXECUTIONFLOW:7����5�����֮��Ӧ������Ļ�Ͽ���EM_IFRAME_720_576ͼƬ��Ч����
//@EXECUTIONFLOW:8���ͷŵ�EM_IFRAME_720_576ռ�õ��ڴ档
//@EXECUTIONFLOW:9����EM_JPG_720_576ͼƬ�ظ����ϲ��衣
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0023(void)
{  
	CSUDIOSGRect_S sDstRect;
	CSUDI_BOOL bRet = CSUDI_TRUE;

	sDstRect.m_nWidth = 500;
	sDstRect.m_nHeight = 500;
	
	bRet = OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_IFRAME,EM_IFRAME_720_576,EM_UDIOSG_SCALE_OPTIMUM,&sDstRect);
	CSTK_ASSERT_TRUE_FATAL((bRet == CSUDI_TRUE),"����MPEGʧ��!");

	bRet = OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_JPG,EM_JPG_720_576,EM_UDIOSG_SCALE_OPTIMUM,&sDstRect);
	CSTK_ASSERT_TRUE_FATAL((bRet == CSUDI_TRUE),"����JPGʧ��!");
	
	CSTK_FATAL_POINT;	
	return bRet;
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: ���Խ���ͼƬ����ΪIFRAME 720*576��ʽʱ�Ľ���,���ģʽΪEM_UDIOSG_SCALE_OPTIMUM
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2���п��е���Ƶ������
//@PRECONDITION: 3��ƽ̨֧�ָ����͵�ͼƬ����
//@INPUT:1������ͼƬ�������ַ���Ϊ��Ӧ��ͼƬ����:"IFRAME"
//@INPUT:2��ͼƬEM_IFRAME_720_576�����ݵ�ַ
//@INPUT:3��ͼƬEM_IFRAME_720_576�ĳ���
//@INPUT:4���������ģʽΪEM_UDIOSG_SCALE_OPTIMUM
//@INPUT:5���������Surface�Ĵ�С��ʼֵΪ625*500
//@EXPECTATION: 1���ܹ��ɹ���ͼƬ���н��룬����CSUDI_SUCCESS��
//@EXPECTATION: 2������õ���surface�ĸ߿�Ӧ��Ϊ{625��500}
//@EXPECTATION: 3������õ���surface�����ܹ��ɹ�blit���Դ����
//@REMARK: 
//@EXECUTIONFLOW:1����ȡEM_IFRAME_720_576ͼƬ���ݵ��ڴ��ַ�ͳ���
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDI_SUCCESS������õ���phSurfaceӦ�ò�ΪCSUDI_NULL
//@EXECUTIONFLOW:3������CSUDIOSGGetSurfaceInfo��ȡsurface��Ϣ����������õ���surface��СӦ��Ϊ{625��500}�ı���
//@EXECUTIONFLOW:4������CSUDIOSGGetDisplaySurface�ӿڻ�ȡ�Դ�ռ�hDisSurface���
//@EXECUTIONFLOW:5������CSUDIOSGBlit�ӿڽ�����õ���ͼƬ��Ϣblit���Դ�ռ�hDisSurface�ϣ�blit�ľ�������ΪCSUDI_NULL��
//@EXECUTIONFLOW:6������CSUDIOSGSync���ͼƬ��blit�������ɹ�����CSUDI_SUCCESS��
//@EXECUTIONFLOW:7����5�����֮��Ӧ������Ļ�Ͽ���EM_IFRAME_720_576ͼƬ��Ч����
//@EXECUTIONFLOW:8���ͷŵ�EM_IFRAME_720_576ռ�õ��ڴ档
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0024(void)
{  
	CSUDIOSGRect_S sDstRect;
	CSUDI_BOOL bRet = CSUDI_TRUE;

	sDstRect.m_nWidth = 625;
	sDstRect.m_nHeight = 500;
	bRet = OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_IFRAME,EM_IFRAME_720_576,EM_UDIOSG_SCALE_OPTIMUM,&sDstRect);
	CSTK_ASSERT_TRUE_FATAL((bRet == CSUDI_TRUE),"����MPEGʧ��!");
	
	CSTK_FATAL_POINT;
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: ���Խ���ͼƬ����ΪIFRAME 720*576��ʽʱ�Ľ���,���ģʽΪEM_UDIOSG_SCALE_FIXSIZE
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2���п��е���Ƶ������
//@PRECONDITION: 3��ƽ̨֧�ָ����͵�ͼƬ����
//@INPUT:1������ͼƬ�������ַ���Ϊ��Ӧ��ͼƬ����:"IFRAME"
//@INPUT:2��ͼƬEM_IFRAME_720_576�����ݵ�ַ
//@INPUT:3��ͼƬEM_IFRAME_720_576�ĳ���
//@INPUT:4���������ģʽΪEM_UDIOSG_SCALE_FIXSIZE
//@INPUT:5���������Surface�Ĵ�С��ʼֵΪ625*500
//@EXPECTATION: 1���ܹ��ɹ���ͼƬ���н��룬����CSUDI_SUCCESS��
//@EXPECTATION: 2������õ���surface�ĸ߿�Ӧ��Ϊ{625��500}
//@EXPECTATION: 3������õ���surface�����ܹ��ɹ�blit���Դ����
//@REMARK: 
//@EXECUTIONFLOW:1����ȡEM_IFRAME_720_576ͼƬ���ݵ��ڴ��ַ�ͳ���
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDI_SUCCESS������õ���phSurfaceӦ�ò�ΪCSUDI_NULL
//@EXECUTIONFLOW:3������CSUDIOSGGetSurfaceInfo��ȡsurface��Ϣ����������õ���surface��СӦ��Ϊ{625��500}�ı���
//@EXECUTIONFLOW:4������CSUDIOSGGetDisplaySurface�ӿڻ�ȡ�Դ�ռ�hDisSurface���
//@EXECUTIONFLOW:5������CSUDIOSGBlit�ӿڽ�����õ���ͼƬ��Ϣblit���Դ�ռ�hDisSurface�ϣ�blit�ľ�������ΪCSUDI_NULL��
//@EXECUTIONFLOW:6������CSUDIOSGSync���ͼƬ��blit�������ɹ�����CSUDI_SUCCESS��
//@EXECUTIONFLOW:7����5�����֮��Ӧ������Ļ�Ͽ���EM_IFRAME_720_576ͼƬ��Ч����
//@EXECUTIONFLOW:8���ͷŵ�EM_IFRAME_720_576ռ�õ��ڴ档
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0025(void)
{  
	CSUDIOSGRect_S sDstRect;
	CSUDI_BOOL bRet = CSUDI_TRUE;

	sDstRect.m_nWidth = 625;
	sDstRect.m_nHeight = 500;
	bRet = OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_IFRAME,EM_IFRAME_720_576,EM_UDIOSG_SCALE_FIXSIZE,&sDstRect);

	CSTK_ASSERT_TRUE_FATAL((bRet == CSUDI_TRUE),"����MPEGʧ��!");
	
	CSTK_FATAL_POINT;	
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: ���Խ���ͼƬ����ΪIFRAME 720*576��ʽʱ�Ľ���,���ģʽΪEM_UDIOSG_SCALE_FIXSIZE
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2���п��е���Ƶ������
//@PRECONDITION: 3��ƽ̨֧�ָ����͵�ͼƬ����
//@INPUT:1������ͼƬ�������ַ���Ϊ��Ӧ��ͼƬ����:"IFRAME"
//@INPUT:2��ͼƬEM_IFRAME_720_576�����ݵ�ַ
//@INPUT:3��ͼƬEM_IFRAME_720_576�ĳ���
//@INPUT:4���������ģʽΪEM_UDIOSG_SCALE_FIXSIZE
//@INPUT:5���������Surface�Ĵ�С��ʼֵΪ500*500
//@EXPECTATION: 1���ܹ��ɹ���ͼƬ���н��룬����CSUDI_SUCCESS��
//@EXPECTATION: 2������õ���surface�ĸ߿�Ӧ��Ϊ{500��500}
//@EXPECTATION: 3������õ���surface�����ܹ��ɹ�blit���Դ����
//@REMARK: 
//@EXECUTIONFLOW:1����ȡEM_IFRAME_720_576ͼƬ���ݵ��ڴ��ַ�ͳ���
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDI_SUCCESS������õ���phSurfaceӦ�ò�ΪCSUDI_NULL
//@EXECUTIONFLOW:3������CSUDIOSGGetSurfaceInfo��ȡsurface��Ϣ����������õ���surface��СӦ��Ϊ{500��500}�ı���
//@EXECUTIONFLOW:4������CSUDIOSGGetDisplaySurface�ӿڻ�ȡ�Դ�ռ�hDisSurface���
//@EXECUTIONFLOW:5������CSUDIOSGBlit�ӿڽ�����õ���ͼƬ��Ϣblit���Դ�ռ�hDisSurface�ϣ�blit�ľ�������ΪCSUDI_NULL��
//@EXECUTIONFLOW:6������CSUDIOSGSync���ͼƬ��blit�������ɹ�����CSUDI_SUCCESS��
//@EXECUTIONFLOW:7����5�����֮��Ӧ������Ļ�Ͽ���EM_IFRAME_720_576ͼƬ��Ч����
//@EXECUTIONFLOW:8���ͷŵ�EM_IFRAME_720_576ռ�õ��ڴ档
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0026(void)
{  
	CSUDIOSGRect_S sDstRect;
	CSUDI_BOOL bRet = CSUDI_TRUE;

	sDstRect.m_nWidth = 500;
	sDstRect.m_nHeight = 500;
	bRet = OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_IFRAME,EM_IFRAME_720_576,EM_UDIOSG_SCALE_FIXSIZE,&sDstRect);

	CSTK_ASSERT_TRUE_FATAL((bRet == CSUDI_TRUE),"����MPEGʧ��!");
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: ���Խ���ͼƬ����ΪIFRAME 720*576��ʽʱ�Ľ���,���ģʽΪEM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2���п��е���Ƶ������
//@PRECONDITION: 3��ƽ̨֧�ָ����͵�ͼƬ����
//@INPUT:1������ͼƬ�������ַ���Ϊ��Ӧ��ͼƬ����:"IFRAME"
//@INPUT:2��ͼƬEM_IFRAME_720_576�����ݵ�ַ
//@INPUT:3��ͼƬEM_IFRAME_720_576�ĳ���
//@INPUT:4���������ģʽΪEM_UDIOSG_SCALE_FULLSIZE
//@INPUT:5���������Surface�Ĵ�С��ʼֵΪ500*500
//@EXPECTATION: 1���ܹ��ɹ���ͼƬ���н��룬����CSUDI_SUCCESS��
//@EXPECTATION: 2������õ���surface�ĸ߿�Ӧ��Ϊ{720��576}
//@EXPECTATION: 3������õ���surface�����ܹ��ɹ�blit���Դ����
//@REMARK: 
//@EXECUTIONFLOW:1����ȡEM_IFRAME_720_576ͼƬ���ݵ��ڴ��ַ�ͳ���
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDI_SUCCESS������õ���phSurfaceӦ�ò�ΪCSUDI_NULL
//@EXECUTIONFLOW:3������CSUDIOSGGetSurfaceInfo��ȡsurface��Ϣ����������õ���surface��СӦ��Ϊ{720��576}�ı���
//@EXECUTIONFLOW:4������CSUDIOSGGetDisplaySurface�ӿڻ�ȡ�Դ�ռ�hDisSurface���
//@EXECUTIONFLOW:5������CSUDIOSGBlit�ӿڽ�����õ���ͼƬ��Ϣblit���Դ�ռ�hDisSurface�ϣ�blit�ľ�������ΪCSUDI_NULL��
//@EXECUTIONFLOW:6������CSUDIOSGSync���ͼƬ��blit�������ɹ�����CSUDI_SUCCESS��
//@EXECUTIONFLOW:7����5�����֮��Ӧ������Ļ�Ͽ���EM_IFRAME_720_576ͼƬ��Ч��.
//@EXECUTIONFLOW:8���ͷŵ�EM_IFRAME_720_576ռ�õ��ڴ档
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0027(void)
{  
	CSUDIOSGRect_S sDstRect;
	CSUDI_BOOL bRet = CSUDI_TRUE;

	sDstRect.m_nWidth = 500;
	sDstRect.m_nHeight = 500;
	bRet = OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_IFRAME,EM_IFRAME_720_576,EM_UDIOSG_SCALE_FULLSIZE,&sDstRect);
	
	CSTK_ASSERT_TRUE_FATAL((bRet == CSUDI_TRUE),"����MPEGʧ��!");
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: ���Խ���ͼƬ����ΪIFRAME 720*576��ʽʱ�Ľ���,���ģʽΪEM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2���п��е���Ƶ������
//@PRECONDITION: 3��ƽ̨֧�ָ����͵�ͼƬ����
//@INPUT:1������ͼƬ�������ַ���Ϊ��Ӧ��ͼƬ����:"IFRAME"
//@INPUT:2��ͼƬEM_IFRAME_PES_720_576�����ݵ�ַ
//@INPUT:3��ͼƬEM_IFRAME_PES_720_576�ĳ���
//@INPUT:4���������ģʽΪEM_UDIOSG_SCALE_FULLSIZE
//@INPUT:5���������Surface�Ĵ�С��ʼֵΪ500*500
//@EXPECTATION: 1���ܹ��ɹ���ͼƬ���н��룬����CSUDI_SUCCESS��
//@EXPECTATION: 2������õ���surface�ĸ߿�Ӧ��Ϊ{720��576}
//@EXPECTATION: 3������õ���surface�����ܹ��ɹ�blit���Դ����
//@REMARK: 
//@EXECUTIONFLOW:1����ȡEM_IFRAME_PES_720_576ͼƬ���ݵ��ڴ��ַ�ͳ���
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDI_SUCCESS������õ���phSurfaceӦ�ò�ΪCSUDI_NULL
//@EXECUTIONFLOW:3������CSUDIOSGGetSurfaceInfo��ȡsurface��Ϣ����������õ���surface��СӦ��Ϊ{720��576}�ı���
//@EXECUTIONFLOW:4������CSUDIOSGGetDisplaySurface�ӿڻ�ȡ�Դ�ռ�hDisSurface���
//@EXECUTIONFLOW:5������CSUDIOSGBlit�ӿڽ�����õ���ͼƬ��Ϣblit���Դ�ռ�hDisSurface�ϣ�blit�ľ�������ΪCSUDI_NULL��
//@EXECUTIONFLOW:6������CSUDIOSGSync���ͼƬ��blit�������ɹ�����CSUDI_SUCCESS��
//@EXECUTIONFLOW:7����5�����֮��Ӧ������Ļ�Ͽ���EM_IFRAME_PES_720_576ͼƬ��Ч����
//@EXECUTIONFLOW:8���ͷŵ�EM_IFRAME_PES_720_576ռ�õ��ڴ档
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0028(void)
{  
	CSUDIOSGRect_S sDstRect;
	CSUDI_BOOL bRet = CSUDI_TRUE;

	sDstRect.m_nX 		= sDstRect.m_nY = 0;
	sDstRect.m_nWidth 	= 500;
	sDstRect.m_nHeight 	= 500;
	bRet = OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_IFRAME, EM_IFRAME_PES_720_576,EM_UDIOSG_SCALE_FULLSIZE,&sDstRect);
	
	CSTK_ASSERT_TRUE_FATAL((bRet == CSUDI_TRUE),"����MPEGʧ��!");
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: ���Խ���ͼƬ����ΪIFRAME PES �� ES ��ʽʱ����������,��СΪ720*576���ģʽΪEM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2���п��е���Ƶ������
//@PRECONDITION: 3��ƽ̨֧�ָ����͵�ͼƬ����
//@INPUT:1������ͼƬ�������ַ���Ϊ��Ӧ��ͼƬ����:"IFRAME"
//@INPUT:2��ͼƬEM_IFRAME_PES_720_576�����ݵ�ַ
//@INPUT:3��ͼƬEM_IFRAME_PES_720_576�ĳ���
//@INPUT:4���������ģʽΪEM_UDIOSG_SCALE_FULLSIZE
//@INPUT:5���������Surface�Ĵ�С��ʼֵΪ500*500
//@INPUT:6��ͼƬEM_IFRAME_ES_720_576�����ݵ�ַ
//@INPUT:7��ͼƬEM_IFRAME_ES_720_576�ĳ���
//@INPUT:8���������ģʽΪEM_UDIOSG_SCALE_FULLSIZE
//@INPUT:9���������Surface�Ĵ�СΪ720*576
//@EXPECTATION: 1���ܹ��ɹ���ͼƬ���н��룬����CSUDI_SUCCESS��
//@EXPECTATION: 2������õ���surface�ĸ߿�Ӧ��Ϊ{720*576}
//@EXPECTATION: 3������õ���surface�����ܹ��ɹ�blit���Դ����
//@REMARK: 
//@EXECUTIONFLOW:1����ȡEM_IFRAME_PES_720_576ͼƬ���ݵ��ڴ��ַ�ͳ���
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDI_SUCCESS������õ���phSurfaceӦ�ò�ΪCSUDI_NULL
//@EXECUTIONFLOW:3������CSUDIOSGGetSurfaceInfo��ȡsurface��Ϣ����������õ���surface��СӦ��Ϊ{720��576}�ı���
//@EXECUTIONFLOW:4������CSUDIOSGGetDisplaySurface�ӿڻ�ȡ�Դ�ռ�hDisSurface���
//@EXECUTIONFLOW:5������CSUDIOSGBlit�ӿڽ�����õ���ͼƬ��Ϣblit���Դ�ռ�hDisSurface�ϣ�blit�ľ�������ΪCSUDI_NULL��
//@EXECUTIONFLOW:6������CSUDIOSGSync���ͼƬ��blit�������ɹ�����CSUDI_SUCCESS��
//@EXECUTIONFLOW:7���ͷŵ�EM_IFRAME_PES_720_576ռ�õ��ڴ档
//@EXECUTIONFLOW:8����ȡEM_IFRAME_720_576ͼƬ���ݵ��ڴ��ַ�ͳ���
//@EXECUTIONFLOW:9������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDI_SUCCESS������õ���phSurfaceӦ�ò�ΪCSUDI_NULL
//@EXECUTIONFLOW:10������CSUDIOSGGetSurfaceInfo��ȡsurface��Ϣ����������õ���surface��СӦ��Ϊ{720��576}�ı���
//@EXECUTIONFLOW:11������CSUDIOSGGetDisplaySurface�ӿڻ�ȡ�Դ�ռ�hDisSurface���
//@EXECUTIONFLOW:12������CSUDIOSGBlit�ӿڽ�����õ���ͼƬ��Ϣblit���Դ�ռ�hDisSurface�ϣ�blit�ľ�������ΪCSUDI_NULL��
//@EXECUTIONFLOW:13������CSUDIOSGSync���ͼƬ��blit�������ɹ�����CSUDI_SUCCESS��
//@EXECUTIONFLOW:14���ͷŵ�EM_IFRAME_720_576ռ�õ��ڴ档
//@EXECUTIONFLOW:15���ظ����ϲ���100�Σ���Ļ�Ͽ��Կ�������IFRAME��ͼƬ���л���
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0029(void)
{  
	CSUDIOSGRect_S sDstRect;
	CSUDI_BOOL bRet = CSUDI_TRUE;
	int i = 100;

	sDstRect.m_nWidth = 500;
	sDstRect.m_nHeight = 500;
	for(i = 0; i< 3; i++)
	{
		bRet = OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_IFRAME, EM_IFRAME_PES_720_576,EM_UDIOSG_SCALE_FULLSIZE,&sDstRect);
		CSTK_ASSERT_TRUE_FATAL((bRet == CSUDI_TRUE),"����MPEGʧ��!");
		
		bRet = OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_IFRAME, EM_IFRAME_720_576,EM_UDIOSG_SCALE_FULLSIZE,&sDstRect);
		CSTK_ASSERT_TRUE_FATAL((bRet == CSUDI_TRUE),"����MPEGʧ��!");
	}
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: ģ�����������ʾָ��·����ͼƬ,���ģʽΪEM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2���п��еĽ�����
//@PRECONDITION: 3��ƽ̨֧��ͼƬ����
//@INPUT:1������testdataĿ¼�µ�ͼƬ·��
//@INPUT:2��ͼƬ·�������е����ݵ�ַ
//@INPUT:3��ͼƬ·�������е����ݳ���
//@EXPECTATION: 1���ܹ��ɹ���ͼƬ���н��룬����CSUDI_SUCCESS��
//@EXPECTATION: 2������õ���surface�����ܹ��ɹ�blit���Դ������
//@REMARK: 
//@EXECUTIONFLOW:1����ȡtestdat·���µ�ͼƬ�ļ���·����
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDI_SUCCESS������õ���phSurfaceӦ�ò�ΪCSUDI_NULL
//@EXECUTIONFLOW:3������CSUDIOSGGetDisplaySurface�ӿڻ�ȡ�Դ�ռ�hDisSurface���
//@EXECUTIONFLOW:4������CSUDIOSGBlit�ӿڽ�����õ���ͼƬ��Ϣblit���Դ�ռ�hDisSurface�ϣ�bilt�ľ�������ΪCSUDI_NULL��
//@EXECUTIONFLOW:5������CSUDIOSGSync���ͼƬ��blit�������ɹ�����CSUDI_SUCCESS��
//@EXECUTIONFLOW:6����5�����֮��Ӧ������Ļ�Ͽ���ͼƬ��Ч������������������������ʧ�ܡ�
//@EXECUTIONFLOW:7���ͷŵ�����ռ�õ��ڴ档
//@EXECUTIONFLOW:8������ͼƬ���ݡ�
//@EXECUTIONFLOW:9����ȡ��һ��ͼƬ��·�����ظ�����2~7��
//@EXECUTIONFLOW:10��ģ������ļ���ʮ�顣
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0030(void)
{  
	int i;
	CSUDI_BOOL bRet;
	bRet = CSUDI_FALSE;

	for(i = 0; i< 10; i++)
	{
		bRet = OSG_iTestDecFolderImages();
		CSTK_ASSERT_TRUE_FATAL((bRet == CSUDI_TRUE),"����ͼƬʧ��!");
	}
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: ��ָ��·����ͼƬȫ������һ�飬ͳ�����õ�ʱ��,���ģʽΪEM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2���п��еĽ�����
//@PRECONDITION: 3��ƽ̨֧��ͼƬ����
//@INPUT:1������testdataĿ¼�µ�ͼƬ·��
//@INPUT:2��ͼƬ·�������е����ݵ�ַ
//@INPUT:3��ͼƬ·�������е����ݳ���
//@EXPECTATION: 1���ܹ��ɹ���ͼƬ���н��룬����CSUDI_SUCCESS��
//@EXPECTATION: 2������õ���surface�����ܹ��ɹ�blit���Դ����
//@REMARK: 
//@EXECUTIONFLOW:1����ȡtestdat·���µ�ͼƬ�ļ���·��
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDI_SUCCESS������õ���phSurfaceӦ�ò�ΪCSUDI_NULL
//@EXECUTIONFLOW:3�����CSUDIOSGDecodeImage����ͼƬ���õ�ʱ�䡣
//@EXECUTIONFLOW:4������CSUDIOSGGetDisplaySurface�ӿڻ�ȡ�Դ�ռ�hDisSurface���
//@EXECUTIONFLOW:5������CSUDIOSGBlit�ӿڽ�����õ���ͼƬ��Ϣblit���Դ�ռ�hDisSurface�ϣ�bilt�ľ�������ΪCSUDI_NULL
//@EXECUTIONFLOW:6������CSUDIOSGSync���ͼƬ��blit�������ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:7����5�����֮��Ӧ������Ļ�Ͽ���ͼƬ��Ч������������������������ʧ��
//@EXECUTIONFLOW:8���ͷŵ�ռ�õ��ڴ�
//@EXECUTIONFLOW:9����ȡ��һ��ͼƬ��·�����ظ�����2~7
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0031(void)
{  
	CSUDI_BOOL bRet;
	bRet = CSUDI_TRUE;

	bRet = OSG_iTestDecFolderImages();

	return bRet;
}

//@CASEGROUP: CSUDIOSGDecodeImage32
//@DESCRIPTION: ���Խ��������������128kb��I֡ͼƬ����
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2���п��е���Ƶ������
//@PRECONDITION: 3��ƽ̨֧��I֡���͵�ͼƬ����
//@INPUT:1������ͼƬ�������ַ���Ϊ"IFRAME"
//@INPUT:2��ͼƬUDI2OSG_bigiframe.mpg�����ݵ�ַ
//@INPUT:3��ͼƬUDI2OSG_bigiframe.mpg�ĳ���
//@EXPECTATION: ͼƬ����ɹ�
//@REMARK: 
//@EXECUTIONFLOW:1����ȡUDI2OSG_bigiframe.mpgͼƬ���ݵ��ڴ��ַ�ͳ���
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDI_SUCCESS
//@EXECUTIONFLOW:3����ʾ������ͼƬ��ȷ�Ͻ����
//@EXECUTIONFLOW:4�����ٽ����surface��
//@EXECUTIONFLOW:5���ͷŵ�UDI2OSG_bigiframe.mpgռ�õ��ڴ档
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0032(void)
{  
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	unsigned char* pcBuff = CSUDI_NULL;
	unsigned int nImageLen = 0;
	CSUDI_HANDLE  hSurface_D = CSUDI_NULL;
	
	nImageLen = OSG_iGetPhoto("UDI2OSG_bigiframe.mpg",&pcBuff);
	CSTK_ASSERT_TRUE_FATAL(
				((pcBuff != CSUDI_NULL) && nImageLen != 0),
				"Step 1 get photo data failure"
			);
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGDecodeImage("IFRAME",pcBuff,nImageLen,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,&hSurface_S)),
				"Step 2 decode image failure"
			);

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(OSG_GetTestDisplaySurfaceIndex(),&hSurface_D)),
				"get display surface failure"
			);
	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGBlit(hSurface_D,CSUDI_NULL,hSurface_S,CSUDI_NULL,EM_UDIOSG_OPMODE_COPY)),
				"blit image  to displaysurface failure"
			);
	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGSync()),
				"snyc failure"
			);		

	CSTCPrint("\r��Ļ����ͼƬ��ʾ��?\n");		
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes())," Check failure");
	
	CSTK_FATAL_POINT
	if(hSurface_S != CSUDI_NULL)
	{
		if(CSUDIOSGDestroySurface(hSurface_S) != CSUDI_SUCCESS)
			CSTCPrint("destroy decode surface failure\n");
		hSurface_S = CSUDI_NULL;
	}
	
	if(pcBuff != CSUDI_NULL)
	{
		if(CSUDIOSFree(pcBuff) != CSUDI_SUCCESS)
			CSTCPrint("Step 4 free photo data failure");
		CSTCPrint("[UDI2OSGEST]CSUDIOSFree success !! \r\n");
	}

	//����
	OSG_iClearScreen();
	
	return CSUDI_TRUE;
}

//**********************************************************************************//
//******************************End DecodeImage Test *******************************//
//**********************************************************************************//

//**********************************************************************************//
//******************************Begin IT Test ************************************//
//**********************************************************************************//

//@TESTCASEID:CSTC_OSG_IT_0001
//@CASEGROUP: CSUDIOSGCreateSurface
//@CASEGROUP: CSUDIOSGGetSurfaceInfo 
//@DESCRIPTION:  ����CSUDIOSGSurfaceInfo_S�л�ͼƽ����ڴ��С����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT:P1_1+X2|X1
//@EXPECTATION: �ӻ�ͼƽ����Ϣ�л�ȡ��Ϊm_nMemSize = m_nHeight*m_nPitch
//@REMARK:
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface������ͼƽ��P1_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ��ͼƽ��P1_1����ϢP1_1_Info
//@EXECUTIONFLOW:3���ڴ��СֵP1_1_Info.m_nMemSize = P1_1_Info.m_nHeight*P1_1_Info.m_nPitch
//@EXECUTIONFLOW:4������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_0001(void)
{   
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);
	
	nRet = CSUDIOSGCreateSurface(ePixelFormat,SURFACE_WIDTH_720,SURFACE_HEIGHT_576,&hSurface_S);
	CSTK_ASSERT_TRUE_FATAL(
				(nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),
				"Step 1 Create P1_1 Surface Failure !"
			);

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface_S, &sSurfaceInfo)),
				"step 2 Get P1_1 Surface info failure !"
			);
	CSTK_ASSERT_TRUE_FATAL(
				(sSurfaceInfo.m_pvSurfaceMem != CSUDI_NULL),
				"step 2  Get P1_1 Surface info failure !"
			);
	CSTK_ASSERT_TRUE_FATAL(
				(sSurfaceInfo.m_nMemSize == SURFACE_HEIGHT_576*sSurfaceInfo.m_nPitch),
				"step 3 check memsize failure !"
			);

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

//@TESTCASEID:CSTC_OSG_IT_0002
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION: ����Դ��Ŀ��surface��Ϊ�Դ�ռ䲢�Ҿ��������ص�ʱ��blit����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: D+SR6+DR7+M1
//@EXPECTATION: ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���
//@EXECUTIONFLOW:2������CSUDIOSGFill�Դ�ռ�����D����ɫֵΪ0
//@EXECUTIONFLOW:3������CSUDIOSGBlit��D,DR7,D,SR6,M1��
//@EXECUTIONFLOW:4��ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_0002(void)
{   
	return OSG_iTestBlitWithSameDisplay(EM_RECT_6,EM_RECT_7,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_0003
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION: ����Դ��Ŀ��surface��Ϊ��ͬ�Ļ���ռ䲢�Ҿ��������ص�ʱ��blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+X2/X1+SR6+DR7+M1
//@EXPECTATION: ����CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface������ͼƽ��P1_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGFill����ռ�P1_1����ɫֵΪ0
//@EXECUTIONFLOW:3������CSUDIOSGBlit��P1_1,DR7,P1_1,SR6,M1��
//@EXECUTIONFLOW:4��ִ��blitʧ�ܣ�����CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_0003(void)
{   
	return OSG_iTestBlitWithSameSurface(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,EM_RECT_6,EM_RECT_7,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_0004
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:  ����Դ��Ŀ��surface��Ϊ��ͬ�Ļ���ռ䲢�Ҿ��������зŴ�ʱ��blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+X2/X1+SR6+DR13+M1+Cr1
//@EXPECTATION:1���ɹ��򷵻�CSUDI_SUCCESS����Ŀ�������ܹ����ݾ��δ�С��������
//@EXPECTATION:2�������֧��Ӧ�÷���CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED
//@REMARK: DR13����ȡֵΪ����SR6�ص������ⱶ��(ƽ̨֧�ֵı���)
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface������ͼƽ��P1_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGFill��ͼƽ�����ɫֵΪ 0
//@EXECUTIONFLOW:3������CSUDIOSGFill��ͼƽ������SR6����ɫֵΪCr1
//@EXECUTIONFLOW:4������CSUDIOSGBlit��P1_1,DR13,P1_1,SR6,M1��
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡP1_1��ͼƽ����ϢP1_1_Info
//@EXECUTIONFLOW:6����ȡ��ͼƽ��DR13�����ڵ���ɫֵӦ��ΪCr1
//@EXECUTIONFLOW:7���������4����CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED������4��5����ʡ��
//@EXECUTIONFLOW:8������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_0004(void)
{   
	return OSG_iTestBlitWithSameSurface(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,EM_RECT_6,EM_RECT_13,CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_0005
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:  ����Դ��Ŀ��surface��Ϊ��ͬ�Ļ���ռ䲢�Ҿ�����������Сʱ��blit����
//@PRECONDITION: 1��ƽ̨UDI�Ѿ��ɹ���ʼ��
//@PRECONDITION: 2��������ͼƽ�����ɫģʽΪX2��X1
//@INPUT: P1_1+X2/X1+SR13+DR6+M1+Cr1
//@EXPECTATION:1���ɹ��򷵻�CSUDI_SUCCESS����Ŀ�������ܹ����ݾ��δ�С��������
//@EXPECTATION:2�������֧��Ӧ�÷���CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED
//@REMARK: DR13����ȡֵΪ����SR6�ص������ⱶ��
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface������ͼƽ��P1_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGFill��ͼƽ�����ɫֵΪ0
//@EXECUTIONFLOW:3������CSUDIOSGFill��ͼƽ������SR13����ɫֵΪCr1
//@EXECUTIONFLOW:4������CSUDIOSGBlit��P1_1,DR6,P1_1,SR13,M1��
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡP1_1��ͼƽ����ϢP1_1_Info
//@EXECUTIONFLOW:6����ȡ��ͼƽ��DR6�����ڵ���ɫֵӦ��ΪCr1
//@EXECUTIONFLOW:7���������CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED������4��5����ʡ��
//@EXECUTIONFLOW:8������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_0005(void)
{   
	return OSG_iTestBlitWithSameSurface(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,EM_RECT_13,EM_RECT_6,CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_0006
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:  ����Դ��Ŀ��surface��Ϊ��ͬ���Դ�ռ䲢�Ҿ��������зŴ�ʱ��blit����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: D+SR6+DR13+M1+Cr1
//@EXPECTATION:1���ɹ��򷵻�CSUDI_SUCCESS����Ŀ�������ܹ����ݾ��δ�С��������
//@EXPECTATION:2�������֧��Ӧ�÷���CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED
//@REMARK: DR13����ȡֵΪ����SR6�ص������ⱶ��
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���D
//@EXECUTIONFLOW:2������CSUDIOSGFill�Դ�ռ��������ɫֵΪ0
//@EXECUTIONFLOW:3������CSUDIOSGFill�Դ�ռ�����SR6����ɫֵΪCr1
//@EXECUTIONFLOW:4������CSUDIOSGBlit��D,DR13,D,SR6,M1��
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡD�Դ�ռ���ϢD_Info
//@EXECUTIONFLOW:6����ȡ��ͼƽ��DR13�����ڵ���ɫֵӦ��ΪCr1
//@EXECUTIONFLOW:7���������CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED������4��5����ʡ��
CSUDI_BOOL CSTC_OSG_IT_0006(void)
{   
	return OSG_iTestBlitWithSameDisplay(EM_RECT_6,EM_RECT_13,CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_0007
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:  ����Դ��Ŀ��surface��Ϊ��ͬ���Դ�ռ䲢�Ҿ�����������Сʱ��blit����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: D+SR13+DR6+M1+Cr1
//@EXPECTATION:1���ɹ��򷵻�CSUDI_SUCCESS����Ŀ�������ܹ����ݾ��δ�С��������
//@EXPECTATION:2�������֧��Ӧ�÷���CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED
//@REMARK: DR13����ȡֵΪ����SR6�ص������ⱶ��
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���D
//@EXECUTIONFLOW:2������CSUDIOSGFill�Դ�ռ��������ɫֵΪ0
//@EXECUTIONFLOW:3������CSUDIOSGFill�Դ�ռ�����SR13����ɫֵΪCr1
//@EXECUTIONFLOW:4������CSUDIOSGBlit��D,DR6,D,SR13,M1��
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡP1_1��ͼƽ����ϢD_Info
//@EXECUTIONFLOW:6����ȡ��ͼƽ��DR13�����ڵ���ɫֵӦ��ΪCr1
//@EXECUTIONFLOW:7���������CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED������4��5����ʡ��
CSUDI_BOOL CSTC_OSG_IT_0007(void)
{   
	return OSG_iTestBlitWithSameDisplay(EM_RECT_13,EM_RECT_6,CSUDI_TRUE);	
}

//@TESTCASEID:CSTC_OSG_IT_0008
//@CASEGROUP: CSUDIOSGBlit
//@CASEGROUP: CSUDIOSGCreateSurface
//@CASEGROUP: CSUDIOSGSync
//@DESCRIPTION:  ��ִ��Sync����֮ǰ�����ж�ε�blit���� 
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: P1_1+D+SR1+DR1+M1+X2/X1 
//@EXPECTATION: ԭ���Ͽ���ִ��������blit�Ĳ������ٽ���Sync�������������������ܲ������ɻָ����쳣
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface������ͼƽ��P1_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���D
//@EXECUTIONFLOW:3������CSUDIOSGFill����ռ�P1_1����ɫֵΪCr1
//@EXECUTIONFLOW:4������CSUDIOSGBlit��D,DR1,P1_1,SR1,M1��
//@EXECUTIONFLOW:5��ѭ������N�β���3�Ͳ���4������ÿ�θı�Cr1��ֵ
//@EXECUTIONFLOW:6������CSUDIOSGSync
//@EXECUTIONFLOW:7������CSUDIOSGGetSurfaceInfo��ȡ����ռ�D����ϢD_Info
//@EXECUTIONFLOW:8����ȡ��ͼƽ��D�����ڵ���ɫֵӦ��Ϊ���һ��Fill��Cr1
//@EXECUTIONFLOW:9������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_0008(void)
{   
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hSurface_D = CSUDI_NULL;
	CSUDI_Error_Code nRet;
	unsigned int ii;
	unsigned int unColor = OSG_COLOR_RED_32BIT;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);

	if (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)
	{
		unColor = OSG_COLOR_RED_16BIT;
	}
	
	nRet = CSUDIOSGCreateSurface(ePixelFormat,
									SURFACE_WIDTH_720,
									SURFACE_HEIGHT_576,
									&hSurface_S
									);
	CSTK_ASSERT_TRUE_FATAL(
				(nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),
				"Step 1 Create P1_1 Surface Failure !"
			);

	nRet = CSUDIOSGGetDisplaySurface(OSG_GetTestDisplaySurfaceIndex(),&hSurface_D);
	CSTK_ASSERT_TRUE_FATAL(
				(nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),
				"Step 1 get display surface Failure !"
			);

	CSTCPrint("��������ʱ�Ͼã������ĵȴ�������\n");
	
	for(ii=0;ii < TEST_MAX_BLIT_TIME; ii ++)
	{
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGFill(hSurface_S, CSUDI_NULL, OSG_COLOR_RED_32BIT)),
					"Fill Failure !"
				);
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()), "Sync Failure !");
		
		nRet = CSUDIOSGBlit(hSurface_D, CSUDI_NULL, hSurface_S, CSUDI_NULL, EM_UDIOSG_OPMODE_COPY);
		CSTK_ASSERT_TRUE_FATAL(
					((nRet == CSUDI_SUCCESS)),
					"Blit Failure !"
				);
	}
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iCheckExistColor(hSurface_D, unColor, CSUDI_NULL, ePixelFormat)),
				"Check Surface color Failure !"
			);
	
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

//@TESTCASEID:CSTC_OSG_IT_0009
//@CASEGROUP: 1��CSUDIOSGBlit
//@CASEGROUP: 2��CSUDIOSGDestroySurface
//@CASEGROUP: 3��CSUDIOSGCreateSurface
//@CASEGROUP: 4��CSUDIOSGSync
//@DESCRIPTION:  û��ִ��Sync֮ǰ����blit�Ĺ�����ɾ��Դ��ͼƽ�� 
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: P1_1+D+SR1+DR1+M1+X2/X1  
//@EXPECTATION: ���յĻ�ͼЧ����ȷ�������������������ܲ������ɻָ����쳣
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface������ͼƽ��P1_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���D
//@EXECUTIONFLOW:3������CSUDIOSGFill����ռ�P1_1����ɫֵΪCr1
//@EXECUTIONFLOW:4������CSUDIOSGFill�Դ�ռ��������ɫֵΪ0
//@EXECUTIONFLOW:5������CSUDIOSGBlit��D,DR1,P1_1,SR1,M1��
//@EXECUTIONFLOW:6������CSUDIOSGDestroySurfaceԴ��ͼƽ��P1_1
//@EXECUTIONFLOW:7������CSUDIOSGSync
//@EXECUTIONFLOW:8������CSUDIOSGGetSurfaceInfo��ȡ����ռ�D����ϢD_Info
//@EXECUTIONFLOW:9����ȡ��ͼƽ��D�����ڵ���ɫֵ��ȷ��
//@EXECUTIONFLOW:10������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_0009(void)
{   
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hSurface_D = CSUDI_NULL;
	CSUDI_Error_Code nRet;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;

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
				"Step 1 Create P1_1 Surface Failure !"
			);
	
	nRet = CSUDIOSGGetDisplaySurface(OSG_GetTestDisplaySurfaceIndex(),&hSurface_D);
	CSTK_ASSERT_TRUE_FATAL(
				(nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),
				"Step 1 get display surface Failure !"
			);

	CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGFill(hSurface_S, CSUDI_NULL, OSG_COLOR_RED_32BIT)),
				"Fill Failure !"
			);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
	
	nRet = CSUDIOSGBlit(hSurface_D, CSUDI_NULL, hSurface_S, CSUDI_NULL, EM_UDIOSG_OPMODE_COPY);
	CSTK_ASSERT_TRUE_FATAL(
				((nRet == CSUDI_SUCCESS)),
				"Blit Failure !"
			);
	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface_S)),
				"Destroy P1_1 surface failure !"
			);
	hSurface_S = CSUDI_NULL;
	
	//ִ�д˲��費Ӧ�ó�������������
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
	
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

//@TESTCASEID:CSTC_OSG_IT_0010
//@CASEGROUP: 1��CSUDIOSGBlit
//@CASEGROUP: 2��CSUDIOSGDestroySurface
//@CASEGROUP: 3��CSUDIOSGCreateSurface
//@CASEGROUP: 4��CSUDIOSGSync
//@CASEGROUP: 5��CSUDIOSGFill
//@DESCRIPTION:  û��ִ��Sync֮ǰ����blit�Ĺ����У��ٶ�Դ��ͼƽ��ȥִ��fill����
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: P1_1+D+SR1+DR1+M1+X2/X1  
//@EXPECTATION: �����Դ����ɫֵΪCr2
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface������ͼƽ��P1_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���D
//@EXECUTIONFLOW:3������CSUDIOSGFill����ռ�P1_1����ɫֵΪCr1
//@EXECUTIONFLOW:4������CSUDIOSGFill�Դ�ռ��������ɫֵΪ0
//@EXECUTIONFLOW:5������CSUDIOSGBlit��D,DR1,P1_1,SR1,M1��
//@EXECUTIONFLOW:6������CSUDIOSGFill����ռ�P1_1����ɫֵΪCr2
//@EXECUTIONFLOW:7������CSUDIOSGSync
//@EXECUTIONFLOW:8������CSUDIOSGGetSurfaceInfo��ȡ����ռ�D����ϢD_Info
//@EXECUTIONFLOW:9����ȡ��ͼƽ��D�����ڵ���ɫֵӦΪCr2
//@EXECUTIONFLOW:10������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_0010(void)
{   
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hSurface_D = CSUDI_NULL;
	CSUDI_Error_Code nRet;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;

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
				"Step 1 Create P1_1 Surface Failure !"
			);
	
	nRet = CSUDIOSGGetDisplaySurface(OSG_GetTestDisplaySurfaceIndex(),&hSurface_D);
	CSTK_ASSERT_TRUE_FATAL(
				(nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),
				"Step 1 get display surface Failure !"
			);

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGFill(hSurface_S, CSUDI_NULL, OSG_COLOR_RED_32BIT)),
				"Fill Failure !"
			);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
	
	nRet = CSUDIOSGBlit(hSurface_D, CSUDI_NULL, hSurface_S, CSUDI_NULL, EM_UDIOSG_OPMODE_COPY);
	CSTK_ASSERT_TRUE_FATAL(
				((nRet == CSUDI_SUCCESS)),
				"Blit Failure !"
			);
	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGFill(hSurface_S, CSUDI_NULL, OSG_COLOR_RED_32BIT)),
				"Fill Failure !"
			);
	
	//ִ�д˲��費Ӧ�ó�������������
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
	
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

//@TESTCASEID:CSTC_OSG_IT_0011
//@CASEGROUP: 1��CSUDIOSGCreateSurface
//@CASEGROUP: 2��CSUDIOSGDestroySurface
//@DESCRIPTION:��������������ɾ����ͼƽ��N��κ󣬻��ܹ�������ͼƽ��
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT:	P1_1+X2/X1
//@EXPECTATION: ÿ�δ�����ͼƽ�涼�ɹ�����������ڴ���Ƭ�����Լ���κ��ܹ�����������ͼƽ�棩�������������
//@REMARK: ���д˲��������Ứ�Ѻܳ�ʱ��
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface������ͼƽ��P1_1����ɫģʽΪX2��X1
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ��ͼƽ��P1_1����Ϣ
//@EXECUTIONFLOW:3����ȡ�Ļ�ͼƽ�����ϢӦ�ø�����ʱ�������Ϣһ��
//@EXECUTIONFLOW:4������CSUDIOSGDestroySurface���ٻ�ͼƽ��P1_1
//@EXECUTIONFLOW:5��ѭ��ִ�в���1������4 N�Σ�N>=10000)
CSUDI_BOOL CSTC_OSG_IT_0011(void)
{   
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_Error_Code nRet;
	int jj;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);
	
	for ( jj=0; jj < TEST_MAX_BLIT_TIME; jj ++)
	{
		nRet = CSUDIOSGCreateSurface(ePixelFormat,
										SURFACE_WIDTH_720,
										SURFACE_HEIGHT_576,
										&hSurface_S
										);
		CSTK_ASSERT_TRUE(
					(nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),
					"Step 1 Create P1_1 Surface Failure !"
				);
		CSTK_ASSERT_TRUE(
					(CSUDI_TRUE == OSG_iCheckSurfaceInfo(
						hSurface_S,
						ePixelFormat,
						SURFACE_WIDTH_720,
						SURFACE_HEIGHT_576
						)),
					"Step 3 check surface info Failure !"
				);
		
		if(hSurface_S != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface_S)),
						"Destroy P1_1 surface failure !"
					);
			
			hSurface_S = CSUDI_NULL;
		}	
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

//@TESTCASEID:CSTC_OSG_IT_0012
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����ԴΪARGB��Ŀ��ΪYUV��ɫģʽ֮���blit����
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: P1_1+P2_1+X1+X9+SR1+DR1+M1
//@EXPECTATION: ֧��blit�����򷵻�CSUDI_SUCCESS����֧���򷵻�CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED�� 
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1����ɫģʽΪX1
//@EXECUTIONFLOW:2������CSUDIOSGCreateSurface��������ռ�P2_1����ɫģʽΪX9
//@EXECUTIONFLOW:3������CSUDIOSGFill��仺��ռ�P1_1����ɫֵΪCr1
//@EXECUTIONFLOW:4������CSUDIOSGSyn������
//@EXECUTIONFLOW:5������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M1������ɴ�ARGB��YUVģʽ��blit����
//@EXECUTIONFLOW:6������CSUDIOSGSync���blit��������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIOSGFill���Դ����ռ�P1_1����ɫֵΪ0
//@EXECUTIONFLOW:8������CSUDIOSGSyn������
//@EXECUTIONFLOW:9������CSUDIOSGBlit��P1_1,DR1,P2_1,SR1,M1������ɴ�YUV��ARGBģʽ��blit����
//@EXECUTIONFLOW:10������CSUDIOSGSync���blit��������CSUDI_SUCCESS
//@EXECUTIONFLOW:11������CSUDIOSGGetSurfaceInfo��ȡ����ռ�P1_1����ϢD_Info
//@EXECUTIONFLOW:12����ȡ��ͼƽ��D�����ڵ���ɫֵӦΪCr1
//@EXECUTIONFLOW:13������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_0012(void)
{   
	if(OSG_IsSupportPixelFormat(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888) &&
		OSG_IsSupportPixelFormat(EM_UDIOSG_PIXEL_FORMAT_YCBCR888_422))
	{

		CSUDI_HANDLE hSurface_S = CSUDI_NULL;
		CSUDI_HANDLE hSurface_D = CSUDI_NULL;
		CSUDI_Error_Code nRet;
		nRet = CSUDIOSGCreateSurface(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,
										SURFACE_WIDTH_720,
										SURFACE_HEIGHT_576,
										&hSurface_S
										);
		CSTK_ASSERT_TRUE(
					(nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),
					"Step 1 Create P1_1 Surface Failure !"
				);
		
		nRet = CSUDIOSGCreateSurface(EM_UDIOSG_PIXEL_FORMAT_YCBCR888_422,
										SURFACE_WIDTH_720,
										SURFACE_HEIGHT_576,
										&hSurface_D
										);
		CSTK_ASSERT_TRUE(
					(nRet == CSUDI_SUCCESS && hSurface_D != CSUDI_NULL),
					"Step 2 Create P2_1 Surface Failure !"
				);
		
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGFill(hSurface_S, CSUDI_NULL, OSG_COLOR_RED_32BIT)),
					"Fill Failure !"
				);
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
		
		nRet = CSUDIOSGBlit(hSurface_D, CSUDI_NULL, hSurface_S, CSUDI_NULL, EM_UDIOSG_OPMODE_COPY);
		CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_SUCCESS == nRet || CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED == nRet),
						"��ARGB��YUVģʽ��blit����ʧ��"
				);
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");

		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGFill(hSurface_S, CSUDI_NULL, 0)),
					"Fill Failure !"
				);
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");

		nRet = CSUDIOSGBlit(hSurface_S, CSUDI_NULL, hSurface_D, CSUDI_NULL, EM_UDIOSG_OPMODE_COPY);
		CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_SUCCESS == nRet || CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED == nRet),
						"��YUV��ARGBģʽ��blit����ʧ��"
				);
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
		
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iCheckExistColor(hSurface_S,OSG_COLOR_RED_32BIT,CSUDI_NULL,OSG_COLOR_FORMAT_32BIT)),
					"Check Surface color Failure !"
				);		
	
		CSTK_FATAL_POINT
		if(hSurface_S != CSUDI_NULL)
		{
			if(CSUDIOSGDestroySurface(hSurface_S)!=CSUDI_SUCCESS)
				CSTCPrint("Destroy P1_1 surface failure !\n");
			hSurface_S = CSUDI_NULL;
		}	
		if(hSurface_D != CSUDI_NULL)
		{
			if(CSUDIOSGDestroySurface(hSurface_D)!=CSUDI_SUCCESS)
				CSTCPrint("Destroy P2_1 surface failure !");
			hSurface_D = CSUDI_NULL;
		}	
 		return CSUDI_TRUE;
	}
	else 
	{
		CSTCPrint("\tNot Supported !!!\n");
	  	return CSUDI_TRUE;
	}
}

//@TESTCASEID:CSTC_OSG_IT_0013
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:����16λ��32λ��ɫģʽ֮���blit����
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: P1_1+P2_1+X1+X2+SR1+DR1+M1
//@EXPECTATION: blit�����ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������ռ�P1_1����ɫģʽΪX2��32λģʽ
//@EXECUTIONFLOW:2������CSUDIOSGCreateSurface��������ռ�P2_1����ɫģʽΪX1��16λģʽ
//@EXECUTIONFLOW:3������CSUDIOSGFill��仺��ռ�P1_1����ɫֵΪCr1
//@EXECUTIONFLOW:4������CSUDIOSGSyn������
//@EXECUTIONFLOW:5������CSUDIOSGBlit��P2_1,DR1,P1_1,SR1,M1��,��ɴ�32λ��16ģʽ��blit����
//@EXECUTIONFLOW:6������CSUDIOSGSync���blit��������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIOSGGetSurfaceInfo��ȡ����ռ�P2_1����ϢD_Info
//@EXECUTIONFLOW:8����ȡ��ͼƽ��P2_1�����ڵ���ɫֵӦΪCr1��16λģʽ
//@EXECUTIONFLOW:9������CSUDIOSGFill���Դ����ռ�P1_1����ɫֵΪ0
//@EXECUTIONFLOW:10������CSUDIOSGSyn������
//@EXECUTIONFLOW:11������CSUDIOSGBlit��P1_1,DR1,P2_1,SR1,M1������ɴ�16��32λģʽ��blit����
//@EXECUTIONFLOW:12������CSUDIOSGSync���blit��������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIOSGGetSurfaceInfo��ȡ����ռ�P1_1����ϢD_Info
//@EXECUTIONFLOW:14����ȡ��ͼƽ��P1_1�����ڵ���ɫֵӦΪCr1��32λģʽ
//@EXECUTIONFLOW:15������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_0013(void)
{   
	if(OSG_IsSupportPixelFormat(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888) &&
		OSG_IsSupportPixelFormat(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555))
	{

		CSUDI_HANDLE hSurface_S = CSUDI_NULL;
		CSUDI_HANDLE hSurface_D = CSUDI_NULL;
		CSUDI_Error_Code nRet;
		nRet = CSUDIOSGCreateSurface(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,
										SURFACE_WIDTH_720,
										SURFACE_HEIGHT_576,
										&hSurface_S
										);
		CSTK_ASSERT_TRUE(
					(nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),
					"Step 1 Create P1_1 Surface Failure !"
				);
		
		nRet = CSUDIOSGCreateSurface(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,
										SURFACE_WIDTH_720,
										SURFACE_HEIGHT_576,
										&hSurface_D
										);
		CSTK_ASSERT_TRUE(
					(nRet == CSUDI_SUCCESS && hSurface_D!= CSUDI_NULL),
					"Step 2 Create P2_1 Surface Failure !"
				);
		
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGFill(hSurface_S, CSUDI_NULL, OSG_COLOR_RED_32BIT)),
					"Fill Failure !"
				);
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
		
		nRet = CSUDIOSGBlit(hSurface_D, CSUDI_NULL, hSurface_S, CSUDI_NULL, EM_UDIOSG_OPMODE_COPY);
		CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_SUCCESS == nRet || CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED == nRet),
						"��32λ��16ģʽ��blit����ʧ��"
				);
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
		
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iCheckExistColor(hSurface_D,OSG_COLOR_RED_16BIT,CSUDI_NULL,OSG_COLOR_FORMAT_16BIT)),
					"Check Surface color Failure !"
				);		
		
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGFill(hSurface_S, CSUDI_NULL, 0)),
					"Fill Failure !"
				);
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");

		nRet = CSUDIOSGBlit(hSurface_S, CSUDI_NULL, hSurface_D, CSUDI_NULL, EM_UDIOSG_OPMODE_COPY);
		CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_SUCCESS == nRet || CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED == nRet),
						"��16��32λģʽ��blit����ʧ��"
				);
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
		
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iCheckExistColor(hSurface_S,OSG_COLOR_RED_32BIT,CSUDI_NULL,OSG_COLOR_FORMAT_32BIT)),
					"Check Surface color Failure !"
				);		
	
		CSTK_FATAL_POINT
		if(hSurface_S != CSUDI_NULL)
		{
			if(CSUDIOSGDestroySurface(hSurface_S)!=CSUDI_SUCCESS)
				CSTCPrint("Destroy P1_1 surface failure !\n");
			hSurface_S = CSUDI_NULL;
		}	
		if(hSurface_D != CSUDI_NULL)
		{
			if(CSUDIOSGDestroySurface(hSurface_D)!=CSUDI_SUCCESS)
				CSTCPrint("Destroy P2_1 surface failure !");
			hSurface_D = CSUDI_NULL;
		}	
 		return CSUDI_TRUE;
	}
	else 
	{
		CSTCPrint("\tNot Supported !!!\n");
	  	return CSUDI_TRUE;
	}
}

//@TESTCASEID:CSTC_OSG_IT_0014
//@CASEGROUP: 1��CSUDIOSGCreateSurface
//@CASEGROUP: 2��CSUDIOSGDestroySurface
//@DESCRIPTION:�����ܹ�����ƽ̨�ṩ������ռ���ڴ�������С
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: 1����ɫģʽX2��X1
//@INPUT: 2�����= ����ƽ̨�ṩ���ڴ���������
//@INPUT: 3���߶�= ����ƽ̨�ṩ���ڴ���������
//@EXPECTATION: �ܹ��������ô�С�Ļ���ռ�
//@REMARK: ���д˲��������ᵼ���������������޷����гɹ���
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface������ͼƽ��P1_1
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ��ͼƽ��P1_1����Ϣ
//@EXECUTIONFLOW:3����ȡ�Ļ�ͼƽ�����ϢӦ�ø�����ʱ�������Ϣһ��
CSUDI_BOOL CSTC_OSG_IT_0014(void)
{   
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_Error_Code nRet;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);
	
	nRet = CSUDIOSGCreateSurface(ePixelFormat,
									g_uSurfaceMaxWidth,
									g_uSurfaceMaxHeight,
									&hSurface_S
									);
	CSTK_ASSERT_TRUE(
				(nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),
				"Step 1 Create P1_1 Surface Failure !"
			);	
	
	CSTK_ASSERT_TRUE(
				(CSUDI_TRUE == OSG_iCheckSurfaceInfo(
					hSurface_S,
					ePixelFormat,
					g_uSurfaceMaxWidth,
					g_uSurfaceMaxHeight
					)),
				"Step 2 3 check surface info Failure !"
			);
	
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

//@TESTCASEID:CSTC_OSG_IT_0015
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������������֮���blit����������
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT:					{0,0,10,10},     //10*10
//@INPUT:					{0,0,20,20},     //20*20
//@INPUT:					{0,0,30,30},     //30*30
//@INPUT:					{0,0,40,40},     //40*40
//@INPUT:					{0,0,80,80},     //80*80
//@INPUT:					{0,0,100,100},  //100*100
//@INPUT:					{0,0,200,200},  //200*200
//@INPUT:					{0,0,300,300},  //300*300
//@INPUT:					{0,0,400,400},  //100*100
//@INPUT:					{0,0,SURFACE_WIDTH_720,SURFACE_HEIGHT_576},  
//@INPUT:					{0,0,10,SURFACE_HEIGHT_576},  //10*SURFACE_HEIGHT_576
//@INPUT:					{0,0,40,SURFACE_HEIGHT_576},  //40*SURFACE_HEIGHT_576
//@INPUT:					{0,0,80,SURFACE_HEIGHT_576},  //80*SURFACE_HEIGHT_576
//@INPUT:					{0,0,SURFACE_WIDTH_720,10},  //SURFACE_WIDTH_720*10
//@INPUT:					{0,0,SURFACE_WIDTH_720,40},  //SURFACE_WIDTH_720*40
//@INPUT:					{0,0,SURFACE_WIDTH_720,80}   //SURFACE_WIDTH_720*80
//@EXPECTATION: ��ӡ����������ֵ��blitʱ��
//@EXECUTIONFLOW:��
CSUDI_BOOL CSTC_OSG_IT_0015(void)
{  
	CSUDIOSGRect_S sRect[] = {
		{0,0,10,10},     //10*10
		{0,0,20,20},     //20*20
		{0,0,30,30},     //30*30
		{0,0,40,40},     //40*40
		{0,0,80,80},     //80*80
		{0,0,100,100},  //100*100
		{0,0,200,200},  //200*200
		{0,0,300,300},  //300*300
		{0,0,400,400},  //100*100
		{0,0,SURFACE_WIDTH_720,SURFACE_HEIGHT_576},  
		{0,0,10,SURFACE_HEIGHT_576},  //10*SURFACE_HEIGHT_576
		{0,0,40,SURFACE_HEIGHT_576},  //40*SURFACE_HEIGHT_576
		{0,0,80,SURFACE_HEIGHT_576},  //80*SURFACE_HEIGHT_576
		{0,0,SURFACE_WIDTH_720,10},  //SURFACE_WIDTH_720*10
		{0,0,SURFACE_WIDTH_720,40},  //SURFACE_WIDTH_720*40
		{0,0,SURFACE_WIDTH_720,80}   //SURFACE_WIDTH_720*80
	};  
	return OSG_iTESTBlitPerformance(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,sRect,sRect,sizeof(sRect)/sizeof(sRect[0]),CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_0016
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:���Ի�����Դ�֮���blit����������
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT:					{0,0,10,10},     //10*10
//@INPUT:					{0,0,20,20},     //20*20
//@INPUT:					{0,0,30,30},     //30*30
//@INPUT:					{0,0,40,40},     //40*40
//@INPUT:					{0,0,80,80},     //80*80
//@INPUT:					{0,0,100,100},  //100*100
//@INPUT:					{0,0,200,200},  //200*200
//@INPUT:					{0,0,300,300},  //300*300
//@INPUT:					{0,0,400,400},  //100*100
//@INPUT:					{0,0,SURFACE_WIDTH_720,SURFACE_HEIGHT_576},  
//@INPUT:					{0,0,10,SURFACE_HEIGHT_576},  //10*SURFACE_HEIGHT_576
//@INPUT:					{0,0,40,SURFACE_HEIGHT_576},  //40*SURFACE_HEIGHT_576
//@INPUT:					{0,0,80,SURFACE_HEIGHT_576},  //80*SURFACE_HEIGHT_576
//@INPUT:					{0,0,SURFACE_WIDTH_720,10},  //SURFACE_WIDTH_720*10
//@INPUT:					{0,0,SURFACE_WIDTH_720,40},  //SURFACE_WIDTH_720*40
//@INPUT:					{0,0,SURFACE_WIDTH_720,80}   //SURFACE_WIDTH_720*80
//@EXPECTATION: ��ӡ����������ֵ��blitʱ��
//@EXECUTIONFLOW:��
CSUDI_BOOL CSTC_OSG_IT_0016(void)
{  	
	CSUDIOSGRect_S sRect[] = {
		{0,0,10,10},     //10*10
		{0,0,20,20},     //20*20
		{0,0,30,30},     //30*30
		{0,0,40,40},     //40*40
		{0,0,80,80},     //80*80
		{0,0,100,100},  //100*100
		{0,0,200,200},  //200*200
		{0,0,300,300},  //300*300
		{0,0,400,400},  //100*100
		{0,0,SURFACE_WIDTH_720,SURFACE_HEIGHT_576},  
		{0,0,10,SURFACE_HEIGHT_576},  //10*SURFACE_HEIGHT_576
		{0,0,40,SURFACE_HEIGHT_576},  //40*SURFACE_HEIGHT_576
		{0,0,80,SURFACE_HEIGHT_576},  //80*SURFACE_HEIGHT_576
		{0,0,SURFACE_WIDTH_720,10},  //SURFACE_WIDTH_720*10
		{0,0,SURFACE_WIDTH_720,40},  //SURFACE_WIDTH_720*40
		{0,0,SURFACE_WIDTH_720,80}   //SURFACE_WIDTH_720*80
	}; 
	return OSG_iTESTBlitPerformanceWithDisplay(sRect,sRect,sizeof(sRect)/sizeof(sRect[0]),CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_0017
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:���Ի����Fill����������
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT:					{0,0,10,10},     //10*10
//@INPUT:					{0,0,20,20},     //20*20
//@INPUT:					{0,0,30,30},     //30*30
//@INPUT:					{0,0,40,40},     //40*40
//@INPUT:					{0,0,80,80},     //80*80
//@INPUT:					{0,0,100,100},  //100*100
//@INPUT:					{0,0,200,200},  //200*200
//@INPUT:					{0,0,300,300},  //300*300
//@INPUT:					{0,0,400,400},  //100*100
//@INPUT:					{0,0,SURFACE_WIDTH_720,SURFACE_HEIGHT_576},  //720*576
//@INPUT:					{0,0,10,SURFACE_HEIGHT_576},  //10*SURFACE_HEIGHT_576
//@INPUT:					{0,0,40,SURFACE_HEIGHT_576},  //40*SURFACE_HEIGHT_576
//@INPUT:					{0,0,80,SURFACE_HEIGHT_576},  //80*SURFACE_HEIGHT_576
//@INPUT:					{0,0,SURFACE_WIDTH_720,10},  //SURFACE_WIDTH_720*10
//@INPUT:					{0,0,SURFACE_WIDTH_720,40},  //SURFACE_WIDTH_720*40
//@INPUT:					{0,0,SURFACE_WIDTH_720,80}   //SURFACE_WIDTH_720*80
//@EXPECTATION: ��ӡ����������ֵ��Fillʱ��
//@EXECUTIONFLOW:��
CSUDI_BOOL CSTC_OSG_IT_0017(void)
{   
	CSUDIOSGRect_S sRect[] = {
				{0,0,10,10},     //10*10
				{0,0,20,20},     //20*20
				{0,0,30,30},     //30*30
				{0,0,40,40},     //40*40
				{0,0,80,80},     //80*80
				{0,0,100,100},  //100*100
				{0,0,200,200},  //200*200
				{0,0,300,300},  //300*300
				{0,0,400,400},  //100*100
				{0,0,SURFACE_WIDTH_720,SURFACE_HEIGHT_576},  
				{0,0,10,SURFACE_HEIGHT_576},  //10*SURFACE_HEIGHT_576
				{0,0,40,SURFACE_HEIGHT_576},  //40*SURFACE_HEIGHT_576
				{0,0,80,SURFACE_HEIGHT_576},  //80*SURFACE_HEIGHT_576
				{0,0,SURFACE_WIDTH_720,10},  //SURFACE_WIDTH_720*10
				{0,0,SURFACE_WIDTH_720,40},  //SURFACE_WIDTH_720*40
				{0,0,SURFACE_WIDTH_720,80}   //SURFACE_WIDTH_720*80
			};
	return OSG_iTESTFillPerformace(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,sRect,sizeof(sRect)/sizeof(sRect[0]));
}

//@TESTCASEID:CSTC_OSG_IT_0018
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:�����Դ��Fill����������
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT:					{0,0,10,10},     //10*10
//@INPUT:					{0,0,20,20},     //20*20
//@INPUT:					{0,0,30,30},     //30*30
//@INPUT:					{0,0,40,40},     //40*40
//@INPUT:					{0,0,80,80},     //80*80
//@INPUT:					{0,0,100,100},  //100*100
//@INPUT:					{0,0,200,200},  //200*200
//@INPUT:					{0,0,300,300},  //300*300
//@INPUT:					{0,0,400,400},  //100*100
//@INPUT:					{0,0,SURFACE_WIDTH_720,SURFACE_HEIGHT_576},  
//@INPUT:					{0,0,10,SURFACE_HEIGHT_576},  //10*SURFACE_HEIGHT_576
//@INPUT:					{0,0,40,SURFACE_HEIGHT_576},  //40*SURFACE_HEIGHT_576
//@INPUT:					{0,0,80,SURFACE_HEIGHT_576},  //80*SURFACE_HEIGHT_576
//@INPUT:					{0,0,SURFACE_WIDTH_720,10},  //SURFACE_WIDTH_720*10
//@INPUT:					{0,0,SURFACE_WIDTH_720,40},  //SURFACE_WIDTH_720*40
//@INPUT:					{0,0,SURFACE_WIDTH_720,80}   //SURFACE_WIDTH_720*80
//@EXPECTATION: ��ӡ����������ֵ��Fillʱ��
//@EXECUTIONFLOW:��
CSUDI_BOOL CSTC_OSG_IT_0018(void)
{   
	CSUDIOSGRect_S sRect[] = {
				{0,0,10,10},     //10*10
				{0,0,20,20},     //20*20
				{0,0,30,30},     //30*30
				{0,0,40,40},     //40*40
				{0,0,80,80},     //80*80
				{0,0,100,100},  //100*100
				{0,0,200,200},  //200*200
				{0,0,300,300},  //300*300
				{0,0,400,400},  //100*100
				{0,0,SURFACE_WIDTH_720,SURFACE_HEIGHT_576},  
				{0,0,10,SURFACE_HEIGHT_576},  //10*SURFACE_HEIGHT_576
				{0,0,40,SURFACE_HEIGHT_576},  //40*SURFACE_HEIGHT_576
				{0,0,80,SURFACE_HEIGHT_576},  //80*SURFACE_HEIGHT_576
				{0,0,SURFACE_WIDTH_720,10},  //SURFACE_WIDTH_720*10
				{0,0,SURFACE_WIDTH_720,40},  //SURFACE_WIDTH_720*40
				{0,0,SURFACE_WIDTH_720,80}   //SURFACE_WIDTH_720*80
			};
	return OSG_iTESTFillPerformaceWithDisplay(sRect,sizeof(sRect)/sizeof(sRect[0]));
}

//@TESTCASEID:CSTC_OSG_IT_0019
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������������֮���alphaģʽ��blit����������
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT:					{0,0,10,10},     //10*10
//@INPUT:					{0,0,20,20},     //20*20
//@INPUT:					{0,0,30,30},     //30*30
//@INPUT:					{0,0,40,40},     //40*40
//@INPUT:					{0,0,80,80},     //80*80
//@INPUT:					{0,0,100,100},  //100*100
//@INPUT:					{0,0,200,200},  //200*200
//@INPUT:					{0,0,300,300},  //300*300
//@INPUT:					{0,0,400,400},  //100*100
//@INPUT:					{0,0,SURFACE_WIDTH_720,SURFACE_HEIGHT_576},  
//@INPUT:					{0,0,10,SURFACE_HEIGHT_576},  //10*SURFACE_HEIGHT_576
//@INPUT:					{0,0,40,SURFACE_HEIGHT_576},  //40*SURFACE_HEIGHT_576
//@INPUT:					{0,0,80,SURFACE_HEIGHT_576},  //80*SURFACE_HEIGHT_576
//@INPUT:					{0,0,SURFACE_WIDTH_720,10},  //SURFACE_WIDTH_720*10
//@INPUT:					{0,0,SURFACE_WIDTH_720,40},  //SURFACE_WIDTH_720*40
//@INPUT:					{0,0,SURFACE_WIDTH_720,80}   //SURFACE_WIDTH_720*80
//@EXPECTATION: ��ӡ����������ֵ��blitʱ��
//@EXECUTIONFLOW:��Դ��Ŀ�Ļ�ͼƽ��alphaֵΪ0x7f
CSUDI_BOOL CSTC_OSG_IT_0019(void)
{  

	CSUDIOSGRect_S sRect[] = {
		{0,0,10,10},     //10*10
		{0,0,20,20},     //20*20
		{0,0,30,30},     //30*30
		{0,0,40,40},     //40*40
		{0,0,80,80},     //80*80
		{0,0,100,100},  //100*100
		{0,0,200,200},  //200*200
		{0,0,300,300},  //300*300
		{0,0,400,400},  //100*100
		{0,0,SURFACE_WIDTH_720,SURFACE_HEIGHT_576},  
		{0,0,10,SURFACE_HEIGHT_576},  //10*SURFACE_HEIGHT_576
		{0,0,40,SURFACE_HEIGHT_576},  //40*SURFACE_HEIGHT_576
		{0,0,80,SURFACE_HEIGHT_576},  //80*SURFACE_HEIGHT_576
		{0,0,SURFACE_WIDTH_720,10},  //SURFACE_WIDTH_720*10
		{0,0,SURFACE_WIDTH_720,40},  //SURFACE_WIDTH_720*40
		{0,0,SURFACE_WIDTH_720,80}   //SURFACE_WIDTH_720*80
	};  
	return OSG_iTESTBlitPerformance(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,sRect,sRect,sizeof(sRect)/sizeof(sRect[0]),CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_0020
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:���Ի�����Դ�֮���alphaģʽ��blit����������
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT:					{0,0,10,10},     //10*10
//@INPUT:					{0,0,20,20},     //20*20
//@INPUT:					{0,0,30,30},     //30*30
//@INPUT:					{0,0,40,40},     //40*40
//@INPUT:					{0,0,80,80},     //80*80
//@INPUT:					{0,0,100,100},  //100*100
//@INPUT:					{0,0,200,200},  //200*200
//@INPUT:					{0,0,300,300},  //300*300
//@INPUT:					{0,0,400,400},  //100*100
//@INPUT:					{0,0,SURFACE_WIDTH_720,SURFACE_HEIGHT_576},  
//@INPUT:					{0,0,10,SURFACE_HEIGHT_576},  //10*SURFACE_HEIGHT_576
//@INPUT:					{0,0,40,SURFACE_HEIGHT_576},  //40*SURFACE_HEIGHT_576
//@INPUT:					{0,0,80,SURFACE_HEIGHT_576},  //80*SURFACE_HEIGHT_576
//@INPUT:					{0,0,SURFACE_WIDTH_720,10},  //SURFACE_WIDTH_720*10
//@INPUT:					{0,0,SURFACE_WIDTH_720,40},  //SURFACE_WIDTH_720*40
//@INPUT:					{0,0,SURFACE_WIDTH_720,80}   //SURFACE_WIDTH_720*80
//@EXPECTATION: ��ӡ����������ֵ��blitʱ��
//@EXECUTIONFLOW:��Դ��ͼƽ��alphaֵΪ0x7f
CSUDI_BOOL CSTC_OSG_IT_0020(void)
{  
	CSUDIOSGRect_S sRect[] = {
		{0,0,10,10},     //10*10
		{0,0,20,20},     //20*20
		{0,0,30,30},     //30*30
		{0,0,40,40},     //40*40
		{0,0,80,80},     //80*80
		{0,0,100,100},  //100*100
		{0,0,200,200},  //200*200
		{0,0,300,300},  //300*300
		{0,0,400,400},  //100*100
		{0,0,SURFACE_WIDTH_720,SURFACE_HEIGHT_576},  
		{0,0,10,SURFACE_HEIGHT_576},  //10*SURFACE_HEIGHT_576
		{0,0,40,SURFACE_HEIGHT_576},  //40*SURFACE_HEIGHT_576
		{0,0,80,SURFACE_HEIGHT_576},  //80*SURFACE_HEIGHT_576
		{0,0,SURFACE_WIDTH_720,10},  //SURFACE_WIDTH_720*10
		{0,0,SURFACE_WIDTH_720,40},  //SURFACE_WIDTH_720*40
		{0,0,SURFACE_WIDTH_720,80}   //SURFACE_WIDTH_720*80
	}; 
	return OSG_iTESTBlitPerformanceWithDisplay(sRect,sRect,sizeof(sRect)/sizeof(sRect[0]),CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_0021
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������������֮���зŴ�ʽ��blit����������
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT:CSUDIOSGRect_S sSRect[] = {
//@INPUT:					{0,0,10,10},     //10*10
//@INPUT:					{0,0,20,20},     //20*20
//@INPUT:					{0,0,30,30},     //30*30
//@INPUT:					{0,0,40,40},     //40*40
//@INPUT:					{0,0,80,80},     //80*80
//@INPUT:					{0,0,100,100},  //100*100
//@INPUT:					{0,0,200,200},  //200*200
//@INPUT:					{0,0,300,300},  //300*300
//@INPUT:					{0,0,400,400},  //100*100
//@INPUT:					{0,0,SURFACE_WIDTH_720-15,SURFACE_HEIGHT_576-15},  
//@INPUT:					{0,0,10,SURFACE_HEIGHT_576-15},  
//@INPUT:					{0,0,40,SURFACE_HEIGHT_576-15},   
//@INPUT:					{0,0,80,SURFACE_HEIGHT_576-15},   
//@INPUT:					{0,0,SURFACE_WIDTH_720-15,10},  
//@INPUT:					{0,0,SURFACE_WIDTH_720-15,40},   
//@INPUT:					{0,0,SURFACE_WIDTH_720-15,80}    
//@INPUT:					};
//@INPUT:CSUDIOSGRect_S sDRect[] = {
//@INPUT:					{0,0,25,25},     //25*25
//@INPUT:					{0,0,35,35},     //35*35
//@INPUT:					{0,0,45,45},     //45*45
//@INPUT:					{0,0,55,55},     //55*55
//@INPUT:					{0,0,95,95},    //95*95
//@INPUT:					{0,0,115,115},  //115*115
//@INPUT:					{0,0,215,215},  //215*215
//@INPUT:					{0,0,315,315},  //315*315
//@INPUT:					{0,0,415,415},  //415*415
//@INPUT:					{0,0,SURFACE_WIDTH_720,SURFACE_HEIGHT_576},  
//@INPUT:					{0,0,25,SURFACE_HEIGHT_576},  
//@INPUT:					{0,0,55,SURFACE_HEIGHT_576},  
//@INPUT:					{0,0,95,SURFACE_HEIGHT_576},   
//@INPUT:					{0,0,SURFACE_WIDTH_720,15},   
//@INPUT:					{0,0,SURFACE_WIDTH_720,55},   
//@INPUT:					{0,0,SURFACE_WIDTH_720,95}   
//@INPUT:					};
//@EXPECTATION: ��ӡ����������ֵ��blitʱ��
//@EXECUTIONFLOW:��
CSUDI_BOOL CSTC_OSG_IT_0021(void)
{  
	CSUDIOSGRect_S sSRect[] = {
				{0,0,10,10},     //10*10
				{0,0,20,20},     //20*20
				{0,0,30,30},     //30*30
				{0,0,40,40},     //40*40
				{0,0,80,80},     //80*80
				{0,0,100,100},  //100*100
				{0,0,200,200},  //200*200
				{0,0,300,300},  //300*300
				{0,0,400,400},  //100*100
				{0,0,SURFACE_WIDTH_720-15,SURFACE_HEIGHT_576-15},  
				{0,0,10,SURFACE_HEIGHT_576-15},  //10*SURFACE_HEIGHT_576-15
				{0,0,40,SURFACE_HEIGHT_576-15},  //40*SURFACE_HEIGHT_576-15
				{0,0,80,SURFACE_HEIGHT_576-15},  //80*SURFACE_HEIGHT_576-15
				{0,0,SURFACE_WIDTH_720-15,10},  //SURFACE_WIDTH_720-15
				{0,0,SURFACE_WIDTH_720-15,40},  //SURFACE_WIDTH_720-15
				{0,0,SURFACE_WIDTH_720-15,80}   //SURFACE_WIDTH_720-15
			};  
	CSUDIOSGRect_S sDRect[] = {
				{0,0,25,25},     //25*25
				{0,0,35,35},     //35*35
				{0,0,45,45},     //45*45
				{0,0,55,55},     //55*55
				{0,0,95,95},    //95*95
				{0,0,115,115},  //115*115
				{0,0,215,215},  //215*215
				{0,0,315,315},  //315*315
				{0,0,415,415},  //415*415
				{0,0,SURFACE_WIDTH_720,SURFACE_HEIGHT_576},  
				{0,0,25,SURFACE_HEIGHT_576},  //10*SURFACE_HEIGHT_576
				{0,0,55,SURFACE_HEIGHT_576},  //40*SURFACE_HEIGHT_576
				{0,0,95,SURFACE_HEIGHT_576},  //80*SURFACE_HEIGHT_576
				{0,0,SURFACE_WIDTH_720,15},  //SURFACE_WIDTH_720*10
				{0,0,SURFACE_WIDTH_720,55},  //SURFACE_WIDTH_720*40
				{0,0,SURFACE_WIDTH_720,95}   //SURFACE_WIDTH_720*80
			};    
	return OSG_iTESTBlitPerformance(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,sSRect,sDRect,sizeof(sSRect)/sizeof(sSRect[0]),CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_0022
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������������֮������С��blit����������
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT:CSUDIOSGRect_S sDRect[] = {
//@INPUT:					{0,0,10,10},     //10*10
//@INPUT:					{0,0,20,20},     //20*20
//@INPUT:					{0,0,30,30},     //30*30
//@INPUT:					{0,0,40,40},     //40*40
//@INPUT:					{0,0,80,80},     //80*80
//@INPUT:					{0,0,100,100},  //100*100
//@INPUT:					{0,0,200,200},  //200*200
//@INPUT:					{0,0,300,300},  //300*300
//@INPUT:					{0,0,400,400},  //100*100
//@INPUT:					{0,0,SURFACE_WIDTH_720-15,SURFACE_HEIGHT_576-15},  
//@INPUT:					{0,0,10,SURFACE_HEIGHT_576-15},  
//@INPUT:					{0,0,40,SURFACE_HEIGHT_576-15},   
//@INPUT:					{0,0,80,SURFACE_HEIGHT_576-15},   
//@INPUT:					{0,0,SURFACE_WIDTH_720-15,10},  
//@INPUT:					{0,0,SURFACE_WIDTH_720-15,40},   
//@INPUT:					{0,0,SURFACE_WIDTH_720-15,80}    
//@INPUT:					};
//@INPUT:CSUDIOSGRect_S sSRect[] = {
//@INPUT:					{0,0,25,25},     //25*25
//@INPUT:					{0,0,35,35},     //35*35
//@INPUT:					{0,0,45,45},     //45*45
//@INPUT:					{0,0,55,55},     //55*55
//@INPUT:					{0,0,95,95},    //95*95
//@INPUT:					{0,0,115,115},  //115*115
//@INPUT:					{0,0,215,215},  //215*215
//@INPUT:					{0,0,315,315},  //315*315
//@INPUT:					{0,0,415,415},  //415*415
//@INPUT:					{0,0,SURFACE_WIDTH_720,SURFACE_HEIGHT_576},  
//@INPUT:					{0,0,25,SURFACE_HEIGHT_576},  
//@INPUT:					{0,0,55,SURFACE_HEIGHT_576},  
//@INPUT:					{0,0,95,SURFACE_HEIGHT_576},   
//@INPUT:					{0,0,SURFACE_WIDTH_720,15},   
//@INPUT:					{0,0,SURFACE_WIDTH_720,55},   
//@INPUT:					{0,0,SURFACE_WIDTH_720,95}   
//@INPUT:					};
//@EXPECTATION: ��ӡ����������ֵ��blitʱ��
//@EXECUTIONFLOW:��
CSUDI_BOOL CSTC_OSG_IT_0022(void)
{   
	CSUDIOSGRect_S sSRect[] = {
				{0,0,25,25},     //25*25
				{0,0,35,35},     //35*35
				{0,0,45,45},     //45*45
				{0,0,55,55},     //55*55
				{0,0,95,95},    //95*95
				{0,0,115,115},  //115*115
				{0,0,215,215},  //215*215
				{0,0,315,315},  //315*315
				{0,0,415,415},  //415*415
				{0,0,SURFACE_WIDTH_720,SURFACE_HEIGHT_576},  
				{0,0,25,SURFACE_HEIGHT_576},  //10*SURFACE_HEIGHT_576
				{0,0,55,SURFACE_HEIGHT_576},  //40*SURFACE_HEIGHT_576
				{0,0,95,SURFACE_HEIGHT_576},  //80*SURFACE_HEIGHT_576
				{0,0,SURFACE_WIDTH_720,15},  //SURFACE_WIDTH_720*10
				{0,0,SURFACE_WIDTH_720,55},  //SURFACE_WIDTH_720*40
				{0,0,SURFACE_WIDTH_720,95}   //SURFACE_WIDTH_720*80
			};    
	CSUDIOSGRect_S sDRect[] = {
				{0,0,10,10},     //10*10
				{0,0,20,20},     //20*20
				{0,0,30,30},     //30*30
				{0,0,40,40},     //40*40
				{0,0,80,80},     //80*80
				{0,0,100,100},  //100*100
				{0,0,200,200},  //200*200
				{0,0,300,300},  //300*300
				{0,0,400,400},  //100*100
				{0,0,SURFACE_WIDTH_720-15,SURFACE_HEIGHT_576-15},  
				{0,0,10,SURFACE_HEIGHT_576-15},  //10*SURFACE_HEIGHT_576-15
				{0,0,40,SURFACE_HEIGHT_576-15},  //40*SURFACE_HEIGHT_576-15
				{0,0,80,SURFACE_HEIGHT_576-15},  //80*SURFACE_HEIGHT_576-15
				{0,0,SURFACE_WIDTH_720-15,10},  //SURFACE_WIDTH_720-15
				{0,0,SURFACE_WIDTH_720-15,40},  //SURFACE_WIDTH_720-15
				{0,0,SURFACE_WIDTH_720-15,80}   //SURFACE_WIDTH_720-15
			}; 
	return OSG_iTESTBlitPerformance(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,sSRect,sDRect,sizeof(sSRect)/sizeof(sSRect[0]),CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_0023
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������������֮���зŴ��alpha��blit����������
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT:CSUDIOSGRect_S sSRect[] = {
//@INPUT:					{0,0,10,10},     //10*10
//@INPUT:					{0,0,20,20},     //20*20
//@INPUT:					{0,0,30,30},     //30*30
//@INPUT:					{0,0,40,40},     //40*40
//@INPUT:					{0,0,80,80},     //80*80
//@INPUT:					{0,0,100,100},  //100*100
//@INPUT:					{0,0,200,200},  //200*200
//@INPUT:					{0,0,300,300},  //300*300
//@INPUT:					{0,0,400,400},  //100*100
//@INPUT:					{0,0,SURFACE_WIDTH_720-15,SURFACE_HEIGHT_576-15},  
//@INPUT:					{0,0,10,SURFACE_HEIGHT_576-15},  
//@INPUT:					{0,0,40,SURFACE_HEIGHT_576-15},   
//@INPUT:					{0,0,80,SURFACE_HEIGHT_576-15},   
//@INPUT:					{0,0,SURFACE_WIDTH_720-15,10},  
//@INPUT:					{0,0,SURFACE_WIDTH_720-15,40},   
//@INPUT:					{0,0,SURFACE_WIDTH_720-15,80}    
//@INPUT:					};
//@INPUT:CSUDIOSGRect_S sDRect[] = {
//@INPUT:					{0,0,25,25},     //25*25
//@INPUT:					{0,0,35,35},     //35*35
//@INPUT:					{0,0,45,45},     //45*45
//@INPUT:					{0,0,55,55},     //55*55
//@INPUT:					{0,0,95,95},    //95*95
//@INPUT:					{0,0,115,115},  //115*115
//@INPUT:					{0,0,215,215},  //215*215
//@INPUT:					{0,0,315,315},  //315*315
//@INPUT:					{0,0,415,415},  //415*415
//@INPUT:					{0,0,SURFACE_WIDTH_720,SURFACE_HEIGHT_576},  
//@INPUT:					{0,0,25,SURFACE_HEIGHT_576},  
//@INPUT:					{0,0,55,SURFACE_HEIGHT_576},  
//@INPUT:					{0,0,95,SURFACE_HEIGHT_576},   
//@INPUT:					{0,0,SURFACE_WIDTH_720,15},   
//@INPUT:					{0,0,SURFACE_WIDTH_720,55},   
//@INPUT:					{0,0,SURFACE_WIDTH_720,95}   
//@INPUT:					};
//@EXPECTATION: ��ӡ����������ֵ��blitʱ��
//@EXECUTIONFLOW:��Դ��ͼƽ��alphaֵΪ0x7f
CSUDI_BOOL CSTC_OSG_IT_0023(void)
{  
	CSUDIOSGRect_S sSRect[] = {
				{0,0,10,10},     //10*10
				{0,0,20,20},     //20*20
				{0,0,30,30},     //30*30
				{0,0,40,40},     //40*40
				{0,0,80,80},     //80*80
				{0,0,100,100},  //100*100
				{0,0,200,200},  //200*200
				{0,0,300,300},  //300*300
				{0,0,400,400},  //100*100
				{0,0,SURFACE_WIDTH_720-15,SURFACE_HEIGHT_576-15},  
				{0,0,10,SURFACE_HEIGHT_576-15},  //10*SURFACE_HEIGHT_576
				{0,0,40,SURFACE_HEIGHT_576-15},  //40*SURFACE_HEIGHT_576
				{0,0,80,SURFACE_HEIGHT_576-15},  //80*SURFACE_HEIGHT_576
				{0,0,SURFACE_WIDTH_720-15,10},  //SURFACE_WIDTH_720*10
				{0,0,SURFACE_WIDTH_720-15,40},  //SURFACE_WIDTH_720*40
				{0,0,SURFACE_WIDTH_720-15,80}   //SURFACE_WIDTH_720*80
			};  
	CSUDIOSGRect_S sDRect[] = {
				{0,0,25,25},     //25*25
				{0,0,35,35},     //35*35
				{0,0,45,45},     //45*45
				{0,0,55,55},     //55*55
				{0,0,95,95},    //95*95
				{0,0,115,115},  //115*115
				{0,0,215,215},  //215*215
				{0,0,315,315},  //315*315
				{0,0,415,415},  //415*415
				{0,0,SURFACE_WIDTH_720,SURFACE_HEIGHT_576},  
				{0,0,25,SURFACE_HEIGHT_576},  //10*SURFACE_HEIGHT_576
				{0,0,55,SURFACE_HEIGHT_576},  //40*SURFACE_HEIGHT_576
				{0,0,95,SURFACE_HEIGHT_576},  //80*SURFACE_HEIGHT_576
				{0,0,SURFACE_WIDTH_720,15},  //SURFACE_WIDTH_720*10
				{0,0,SURFACE_WIDTH_720,55},  //SURFACE_WIDTH_720*40
				{0,0,SURFACE_WIDTH_720,95}   //SURFACE_WIDTH_720*80
			};  
	return OSG_iTESTBlitPerformance(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,sSRect,sDRect,sizeof(sSRect)/sizeof(sSRect[0]),CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_0024
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:������������֮������С�Ĵ�alpha��blit����������
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT:CSUDIOSGRect_S sDRect[] = {
//@INPUT:					{0,0,10,10},     //10*10
//@INPUT:					{0,0,20,20},     //20*20
//@INPUT:					{0,0,30,30},     //30*30
//@INPUT:					{0,0,40,40},     //40*40
//@INPUT:					{0,0,80,80},     //80*80
//@INPUT:					{0,0,100,100},  //100*100
//@INPUT:					{0,0,200,200},  //200*200
//@INPUT:					{0,0,300,300},  //300*300
//@INPUT:					{0,0,400,400},  //100*100
//@INPUT:					{0,0,SURFACE_WIDTH_720-15,SURFACE_HEIGHT_576-15},  
//@INPUT:					{0,0,10,SURFACE_HEIGHT_576-15},  
//@INPUT:					{0,0,40,SURFACE_HEIGHT_576-15},   
//@INPUT:					{0,0,80,SURFACE_HEIGHT_576-15},   
//@INPUT:					{0,0,SURFACE_WIDTH_720-15,10},  
//@INPUT:					{0,0,SURFACE_WIDTH_720-15,40},   
//@INPUT:					{0,0,SURFACE_WIDTH_720-15,80}    
//@INPUT:					};
//@INPUT:CSUDIOSGRect_S sSRect[] = {
//@INPUT:					{0,0,25,25},     //25*25
//@INPUT:					{0,0,35,35},     //35*35
//@INPUT:					{0,0,45,45},     //45*45
//@INPUT:					{0,0,55,55},     //55*55
//@INPUT:					{0,0,95,95},    //95*95
//@INPUT:					{0,0,115,115},  //115*115
//@INPUT:					{0,0,215,215},  //215*215
//@INPUT:					{0,0,315,315},  //315*315
//@INPUT:					{0,0,415,415},  //415*415
//@INPUT:					{0,0,SURFACE_WIDTH_720,SURFACE_HEIGHT_576},  
//@INPUT:					{0,0,25,SURFACE_HEIGHT_576},  
//@INPUT:					{0,0,55,SURFACE_HEIGHT_576},  
//@INPUT:					{0,0,95,SURFACE_HEIGHT_576},   
//@INPUT:					{0,0,SURFACE_WIDTH_720,15},   
//@INPUT:					{0,0,SURFACE_WIDTH_720,55},   
//@INPUT:					{0,0,SURFACE_WIDTH_720,95}   
//@INPUT:					};
//@EXPECTATION: ��ӡ����������ֵ��blitʱ��
//@EXECUTIONFLOW:��Դ��ͼƽ��alphaֵΪ0x7f
CSUDI_BOOL CSTC_OSG_IT_0024(void)
{  
	CSUDIOSGRect_S sSRect[] = {
				{0,0,25,25},     //25*25
				{0,0,35,35},     //35*35
				{0,0,45,45},     //45*45
				{0,0,55,55},     //55*55
				{0,0,95,95},    //95*95
				{0,0,115,115},  //115*115
				{0,0,215,215},  //215*215
				{0,0,315,315},  //315*315
				{0,0,415,415},  //415*415
				{0,0,SURFACE_WIDTH_720,SURFACE_HEIGHT_576},  
				{0,0,25,SURFACE_HEIGHT_576},  //10*SURFACE_HEIGHT_576
				{0,0,55,SURFACE_HEIGHT_576},  //40*SURFACE_HEIGHT_576
				{0,0,95,SURFACE_HEIGHT_576},  //80*SURFACE_HEIGHT_576
				{0,0,SURFACE_WIDTH_720,15},  //SURFACE_WIDTH_720*10
				{0,0,SURFACE_WIDTH_720,55},  //SURFACE_WIDTH_720*40
				{0,0,SURFACE_WIDTH_720,95}   //SURFACE_WIDTH_720*80
			};    
	CSUDIOSGRect_S sDRect[] = {
				{0,0,10,10},     //10*10
				{0,0,20,20},     //20*20
				{0,0,30,30},     //30*30
				{0,0,40,40},     //40*40
				{0,0,80,80},     //80*80
				{0,0,100,100},  //100*100
				{0,0,200,200},  //200*200
				{0,0,300,300},  //300*300
				{0,0,400,400},  //100*100
				{0,0,SURFACE_WIDTH_720-15,SURFACE_HEIGHT_576-15},  
				{0,0,10,SURFACE_HEIGHT_576-15},  //10*SURFACE_HEIGHT_576-15
				{0,0,40,SURFACE_HEIGHT_576-15},  //40*SURFACE_HEIGHT_576-15
				{0,0,80,SURFACE_HEIGHT_576-15},  //80*SURFACE_HEIGHT_576-15
				{0,0,SURFACE_WIDTH_720-15,10},  //SURFACE_WIDTH_720-15
				{0,0,SURFACE_WIDTH_720-15,40},  //SURFACE_WIDTH_720-15
				{0,0,SURFACE_WIDTH_720-15,80}   //SURFACE_WIDTH_720-15
			}; 
	return OSG_iTESTBlitPerformance(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,sSRect,sDRect,sizeof(sSRect)/sizeof(sSRect[0]),CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_0025
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:���Ի�����Դ�֮����С�Ĵ�alpha��blit����������
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT:CSUDIOSGRect_S sDRect[] = {
//@INPUT:					{0,0,10,10},     //10*10
//@INPUT:					{0,0,20,20},     //20*20
//@INPUT:					{0,0,30,30},     //30*30
//@INPUT:					{0,0,40,40},     //40*40
//@INPUT:					{0,0,80,80},     //80*80
//@INPUT:					{0,0,100,100},  //100*100
//@INPUT:					{0,0,200,200},  //200*200
//@INPUT:					{0,0,300,300},  //300*300
//@INPUT:					{0,0,400,400},  //100*100
//@INPUT:					{0,0,SURFACE_WIDTH_720-15,SURFACE_HEIGHT_576-15},  
//@INPUT:					{0,0,10,SURFACE_HEIGHT_576-15},  
//@INPUT:					{0,0,40,SURFACE_HEIGHT_576-15},   
//@INPUT:					{0,0,80,SURFACE_HEIGHT_576-15},   
//@INPUT:					{0,0,SURFACE_WIDTH_720-15,10},  
//@INPUT:					{0,0,SURFACE_WIDTH_720-15,40},   
//@INPUT:					{0,0,SURFACE_WIDTH_720-15,80}    
//@INPUT:					};
//@INPUT:CSUDIOSGRect_S sSRect[] = {
//@INPUT:					{0,0,25,25},     //25*25
//@INPUT:					{0,0,35,35},     //35*35
//@INPUT:					{0,0,45,45},     //45*45
//@INPUT:					{0,0,55,55},     //55*55
//@INPUT:					{0,0,95,95},    //95*95
//@INPUT:					{0,0,115,115},  //115*115
//@INPUT:					{0,0,215,215},  //215*215
//@INPUT:					{0,0,315,315},  //315*315
//@INPUT:					{0,0,415,415},  //415*415
//@INPUT:					{0,0,SURFACE_WIDTH_720,SURFACE_HEIGHT_576},  
//@INPUT:					{0,0,25,SURFACE_HEIGHT_576},  
//@INPUT:					{0,0,55,SURFACE_HEIGHT_576},  
//@INPUT:					{0,0,95,SURFACE_HEIGHT_576},   
//@INPUT:					{0,0,SURFACE_WIDTH_720,15},   
//@INPUT:					{0,0,SURFACE_WIDTH_720,55},   
//@INPUT:					{0,0,SURFACE_WIDTH_720,95}   
//@INPUT:					};
//@EXPECTATION: ��ӡ����������ֵ��blitʱ��
CSUDI_BOOL CSTC_OSG_IT_0025(void)
{  
	CSUDIOSGRect_S sSRect[] = {
			{0,0,25,25},     //25*25
			{0,0,35,35},     //35*35
			{0,0,45,45},     //45*45
			{0,0,55,55},     //55*55
			{0,0,95,95},    //95*95
			{0,0,115,115},  //115*115
			{0,0,215,215},  //215*215
			{0,0,315,315},  //315*315
			{0,0,415,415},  //415*415
			{0,0,SURFACE_WIDTH_720,SURFACE_HEIGHT_576},  
			{0,0,25,SURFACE_HEIGHT_576},  //10*SURFACE_HEIGHT_576
			{0,0,55,SURFACE_HEIGHT_576},  //40*SURFACE_HEIGHT_576
			{0,0,95,SURFACE_HEIGHT_576},  //80*SURFACE_HEIGHT_576
			{0,0,SURFACE_WIDTH_720,15},  //SURFACE_WIDTH_720*10
			{0,0,SURFACE_WIDTH_720,55},  //SURFACE_WIDTH_720*40
			{0,0,SURFACE_WIDTH_720,95}   //SURFACE_WIDTH_720*80
	};
	CSUDIOSGRect_S sDRect[] = {
			{0,0,10,10},     //10*10
			{0,0,20,20},     //20*20
			{0,0,30,30},     //30*30
			{0,0,40,40},     //40*40
			{0,0,80,80},     //80*80
			{0,0,100,100},  //100*100
			{0,0,200,200},  //200*200
			{0,0,300,300},  //300*300
			{0,0,400,400},  //100*100
			{0,0,SURFACE_WIDTH_720-15,SURFACE_HEIGHT_576-15},  
			{0,0,10,SURFACE_HEIGHT_576-15},  //10*SURFACE_HEIGHT_576-15
			{0,0,40,SURFACE_HEIGHT_576-15},  //40*SURFACE_HEIGHT_576-15
			{0,0,80,SURFACE_HEIGHT_576-15},  //80*SURFACE_HEIGHT_576-15
			{0,0,SURFACE_WIDTH_720-15,10},  //SURFACE_WIDTH_720-15*10
			{0,0,SURFACE_WIDTH_720-15,40},  //SURFACE_WIDTH_720-15*40
			{0,0,SURFACE_WIDTH_720-15,80}   //SURFACE_WIDTH_720-15*80
		};
	return OSG_iTESTBlitPerformanceWithDisplay(sSRect,sDRect,sizeof(sSRect)/sizeof(sSRect[0]),CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_0026
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:���Ի�����Դ�֮��Ŵ�Ĵ�alpha��blit����������
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT:CSUDIOSGRect_S sSRect[] = {
//@INPUT:					{0,0,10,10},     //10*10
//@INPUT:					{0,0,20,20},     //20*20
//@INPUT:					{0,0,30,30},     //30*30
//@INPUT:					{0,0,40,40},     //40*40
//@INPUT:					{0,0,80,80},     //80*80
//@INPUT:					{0,0,100,100},  //100*100
//@INPUT:					{0,0,200,200},  //200*200
//@INPUT:					{0,0,300,300},  //300*300
//@INPUT:					{0,0,400,400},  //100*100
//@INPUT:					{0,0,SURFACE_WIDTH_720-15,SURFACE_HEIGHT_576-15},  
//@INPUT:					{0,0,10,SURFACE_HEIGHT_576-15},  
//@INPUT:					{0,0,40,SURFACE_HEIGHT_576-15},   
//@INPUT:					{0,0,80,SURFACE_HEIGHT_576-15},   
//@INPUT:					{0,0,SURFACE_WIDTH_720-15,10},  
//@INPUT:					{0,0,SURFACE_WIDTH_720-15,40},   
//@INPUT:					{0,0,SURFACE_WIDTH_720-15,80}    
//@INPUT:					};
//@INPUT:CSUDIOSGRect_S sDRect[] = {
//@INPUT:					{0,0,25,25},     //25*25
//@INPUT:					{0,0,35,35},     //35*35
//@INPUT:					{0,0,45,45},     //45*45
//@INPUT:					{0,0,55,55},     //55*55
//@INPUT:					{0,0,95,95},    //95*95
//@INPUT:					{0,0,115,115},  //115*115
//@INPUT:					{0,0,215,215},  //215*215
//@INPUT:					{0,0,315,315},  //315*315
//@INPUT:					{0,0,415,415},  //415*415
//@INPUT:					{0,0,SURFACE_WIDTH_720,SURFACE_HEIGHT_576},  
//@INPUT:					{0,0,25,SURFACE_HEIGHT_576},  
//@INPUT:					{0,0,55,SURFACE_HEIGHT_576},  
//@INPUT:					{0,0,95,SURFACE_HEIGHT_576},   
//@INPUT:					{0,0,SURFACE_WIDTH_720,15},   
//@INPUT:					{0,0,SURFACE_WIDTH_720,55},   
//@INPUT:					{0,0,SURFACE_WIDTH_720,95}   
//@INPUT:					};
//@EXPECTATION: ��ӡ����������ֵ��blitʱ��
CSUDI_BOOL CSTC_OSG_IT_0026(void)
{  
	CSUDIOSGRect_S sSRect[] = {
			{0,0,10,10},     //10*10
			{0,0,20,20},     //20*20
			{0,0,30,30},     //30*30
			{0,0,40,40},     //40*40
			{0,0,80,80},     //80*80
			{0,0,100,100},  //100*100
			{0,0,200,200},  //200*200
			{0,0,300,300},  //300*300
			{0,0,400,400},  //100*100
			{0,0,SURFACE_WIDTH_720-15,SURFACE_HEIGHT_576-15},  
			{0,0,10,SURFACE_HEIGHT_576-15},  //10*SURFACE_HEIGHT_576
			{0,0,40,SURFACE_HEIGHT_576-15},  //40*SURFACE_HEIGHT_576
			{0,0,80,SURFACE_HEIGHT_576-15},  //80*SURFACE_HEIGHT_576
			{0,0,SURFACE_WIDTH_720-15,10},  //SURFACE_WIDTH_720*10
			{0,0,SURFACE_WIDTH_720-15,40},  //SURFACE_WIDTH_720*40
			{0,0,SURFACE_WIDTH_720-15,80}   //SURFACE_WIDTH_720*80
		};
	CSUDIOSGRect_S sDRect[] = {
			{0,0,25,25},     //25*25
			{0,0,35,35},     //35*35
			{0,0,45,45},     //45*45
			{0,0,55,55},     //55*55
			{0,0,95,95},    //95*95
			{0,0,115,115},  //115*115
			{0,0,215,215},  //215*215
			{0,0,315,315},  //315*315
			{0,0,415,415},  //415*415
			{0,0,SURFACE_WIDTH_720,SURFACE_HEIGHT_576},  
			{0,0,25,SURFACE_HEIGHT_576},  //10*SURFACE_HEIGHT_576
			{0,0,55,SURFACE_HEIGHT_576},  //40*SURFACE_HEIGHT_576
			{0,0,95,SURFACE_HEIGHT_576},  //80*SURFACE_HEIGHT_576
			{0,0,SURFACE_WIDTH_720,15},  //SURFACE_WIDTH_720*10
			{0,0,SURFACE_WIDTH_720,55},  //SURFACE_WIDTH_720*40
			{0,0,SURFACE_WIDTH_720,95}   //SURFACE_WIDTH_720*80
	};
	return OSG_iTESTBlitPerformanceWithDisplay(sSRect,sDRect,sizeof(sSRect)/sizeof(sSRect[0]),CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_0027
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:���Ի�ͼƽ���pitch�Ƿ���ȷʵ��
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT: ��ͼƽ��Ŀ��=255,254,253���߶�Ϊ255
//@EXPECTATION: ������Ļ�ܹ�������ʾͼƬ���������쳣���
//@EXECUTIONFLOW:1���ֱ𴴽���ͼƽ��Ŀ��Ϊ255��254��253���߶�Ϊ255�Ļ�ͼƽ��
//@EXECUTIONFLOW:2��������Ļ������ɫΪ��ɫ
//@EXECUTIONFLOW:3��Ȼ�����Դ��Ͻ���blit����������blit�ɹ�������ͼƬ��ʾû���쳣
CSUDI_BOOL CSTC_OSG_IT_0027(void)
{
	extern unsigned char Picture_255_255_h[260100];
	extern unsigned char Picture_254_255_h[259080];
	extern unsigned char Picture_253_255_h[258060];
	
	CSUDI_BOOL bRet = CSUDI_TRUE;
	bRet &= OSG_iTESTSurfacePitch(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,255,255,Picture_255_255_h,sizeof(Picture_255_255_h));
	bRet &= OSG_iTESTSurfacePitch(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,254,255,Picture_254_255_h,sizeof(Picture_254_255_h));
	bRet &= OSG_iTESTSurfacePitch(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,253,255,Picture_253_255_h,sizeof(Picture_253_255_h));
	return bRet;
}

//@TESTCASEID:CSTC_OSG_IT_0028
//@CASEGROUP: CSUDIOSGBlit
//@CASEGROUP: CSUDIOSGCreateSurface
//@CASEGROUP: CSUDIOSGSync
//@DESCRIPTION:  �ú���������ɫ��������ˢ���Դ�,�������ᶪʧˢ�²���
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@EXPECTATION: �ú���������ɫ�ľ���������ˢ���Դ�,�����ˢ�¹����г����������ֻ����ϵĺ�ɫ����ɫ����������,�����ʧ��
//@INPUT:CSUDIOSGRect_S sSRect[] = {
//@INPUT:					{0,0,720,40},   
//@INPUT:					{0,40,720,40},   
//@INPUT:					{0,80,720,40},   
//@INPUT:					{0,120,720,40},   
//@INPUT:					{0,160,720,40},   
//@INPUT:					{0,200,720,40},   
//@INPUT:					{0,240,720,40},   
//@INPUT:					{0,280,720,40},   
//@INPUT:					{0,320,720,40},   
//@INPUT:					{0,360,720,40},   
//@INPUT:					{0,400,720,40},   
//@INPUT:					{0,440,720,40}
//@INPUT:                                 {0,480,720,40},
//@INPUT:				       {0,520,720,40},
//@INPUT:					};
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���D
//@EXECUTIONFLOW:3������CSUDIOSGCreateSurface������ͼƽ��P1_1����ɫģʽΪX2��X1,��720,����50
//@EXECUTIONFLOW:4������CSUDIOSGFill����ͼƽ������ɫ
//@EXECUTIONFLOW:5������CSUDIOSGCreateSurface������ͼƽ��P1_2����ɫģʽΪX2��X1,��s720,����50
//@EXECUTIONFLOW:6������CSUDIOSGFill����ͼƽ�������ɫ
//@EXECUTIONFLOW:7����ѭ��ֵiΪ0
//@EXECUTIONFLOW:8������CSUDIOSGBlit��D,sSRect[i],P1_1,sSRect[i],EM_UDIOSG_OPMODE_COPY��,�ѻ�ͼƽ��P1_1�������Դ���
//@EXECUTIONFLOW:9����ѭ��ֵi��1
//@EXECUTIONFLOW:10    ����CSUDIOSGBlit��D,sSRect[i],P1_2,sSRect[i],EM_UDIOSG_OPMODE_COPY��,�ѻ�ͼƽ��P1_2�������Դ���
//@EXECUTIONFLOW:11    ��ѭ��ֵi��1
//@EXECUTIONFLOW:12    �ظ����ò���8~����11��6��
//@EXECUTIONFLOW:13    ����CSUDIOSGSync����ͬ��
//@EXECUTIONFLOW:14    �ظ����ò���7~����13��50��,ÿ�ε���ʱ�Ѳ���8�Ͳ���10˳��Ե�
//@EXECUTIONFLOW:15    ����,����CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_0028(void)
{
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	CSUDI_HANDLE  phSurface1;
	CSUDI_HANDLE  phSurface2;

	CSUDI_HANDLE hDisplay = CSUDI_NULL;
	CSUDIOSGSurfaceInfo_S hSurfaceInfo;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo1;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo2;
	int i = 0;
	int n = 0;
	int t = 0;
	CSUDIOSGRect_S sSRect[] = {
	{0,0,720,40},   
	{0,40,720,40},   
	{0,80,720,40},   
	{0,120,720,40},   
	{0,160,720,40},   
	{0,200,720,40},   
	{0,240,720,40},   
	{0,280,720,40},   
	{0,320,720,40},   
	{0,360,720,40},   
	{0,400,720,40},   
	{0,440,720,40},
	{0,480,720,40},
	{0,520,720,40},
	};
	int nRectLen = 7;
	unsigned int nColor1 = OSG_COLOR_GREEN_32BIT;
	unsigned int nColor2 = OSG_COLOR_RED_32BIT;
	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);

	CSTK_ASSERT_TRUE_FATAL(
		                     (CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(OSG_GetTestDisplaySurfaceIndex(), &hDisplay) && (CSUDI_NULL != hDisplay)),
		                     "�����Դ�ʧ��"
		                  );
	CSTK_ASSERT_TRUE_FATAL(
		                    (CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hDisplay, &hSurfaceInfo)), 
		                     "��ȡ����Ϣʧ��"
		                  );
	CSTK_ASSERT_TRUE_FATAL(
		                    (CSUDI_SUCCESS == CSUDIOSGCreateSurface( ePixelFormat, 720, 30, &phSurface1)),
		                    "����ƽ��ʧ��"
		                   );
	CSTK_ASSERT_TRUE_FATAL(
		                    (CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(phSurface1, &sSurfaceInfo1)), 
		                    "��ȡ����Ϣʧ��"
		                   );
	CSTK_ASSERT_TRUE_FATAL(
		                     (CSUDI_SUCCESS == CSUDIOSGCreateSurface( ePixelFormat, 720, 30, &phSurface2)),
		                     "����ƽ��ʧ��"
		                    );
	CSTK_ASSERT_TRUE_FATAL(
		                     (CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(phSurface2, &sSurfaceInfo2)), 
		                     "��ȡ����Ϣʧ��"
		                    );
       CSTK_ASSERT_TRUE_FATAL(
		   	                     (CSUDI_SUCCESS == CSUDIOSGFill(phSurface1, CSUDI_NULL, nColor1)),
	   	                     "Fill Failure !"
	   	                    );
	CSTK_ASSERT_TRUE_FATAL(
			                     (CSUDI_SUCCESS == CSUDIOSGFill(phSurface2, CSUDI_NULL, nColor2)),
		                     "Fill Failure !"
		                    );

	CSTCPrint("��ע����Ļ��ˢ�¹������Ƿ������������������ϵĺ�ɫ(����ɫ)����������,ÿ����������СΪ %d*%d\n", 
		sSRect[0].m_nWidth, sSRect[0].m_nHeight);
	CSTKWaitAnyKey();
	
	for(n = 0;n < 50;n++)
	{
		t = 0;
	       i = 0;
		CSUDIOSThreadSleep(100);
		for(t = 0;t < nRectLen;t ++)
		{
			switch (n%2)
			{
				case 0:
				{
					CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGBlit(hDisplay,&sSRect[i],phSurface1,CSUDI_NULL,EM_UDIOSG_OPMODE_COPY)),
						"������ͼƽ��phSurface1���Դ�ʧ��" );
					i++;
					CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGBlit(hDisplay,&sSRect[i],phSurface2,CSUDI_NULL,EM_UDIOSG_OPMODE_COPY)),
						"������ͼƽ��phSurface2���Դ�ʧ��");
					i++; 
					break;
				}
		            case 1:
				{
					CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGBlit(hDisplay,&sSRect[i],phSurface2,CSUDI_NULL,EM_UDIOSG_OPMODE_COPY)),
						"������ͼƽ��phSurface2���Դ�ʧ��" );
					i++;
					CSTK_ASSERT_TRUE_FATAL( (CSUDI_SUCCESS == CSUDIOSGBlit(hDisplay,&sSRect[i],phSurface1,CSUDI_NULL,EM_UDIOSG_OPMODE_COPY)),
                                         "������ͼƽ��phSurface1���Դ�ʧ��" );
					i++;
					break;
				}
				default:
					break;
			}
				 
		}
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
	}
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
	
	CSTCPrint(" ����û����ˢ�¹����г����������������ϵĺ�ɫ(����ɫ)����������\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), " result failure");
	
	CSTK_FATAL_POINT
		
	if(phSurface1 != CSUDI_NULL)
	{
		if(CSUDIOSGDestroySurface(phSurface1)!=CSUDI_SUCCESS)
		{
			CSTCPrint("���ٻ�ͼƽ��1ʧ��\n"); 	
		}
		
		phSurface1 = CSUDI_NULL;
	}
	if(phSurface2 != CSUDI_NULL)
	{
		if(CSUDIOSGDestroySurface(phSurface2)!=CSUDI_SUCCESS)
		{
			CSTCPrint("���ٻ�ͼƽ��2ʧ��"); 	
		}
		
		phSurface2 = CSUDI_NULL;
	}
	
	OSG_iClearScreen();
	
	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_0029
//@CASEGROUP: CSUDIOSGBlit
//@CASEGROUP: CSUDIOSGCreateSurface
//@CASEGROUP: CSUDIOSGSync
//@DESCRIPTION:  ʹ��ͼƬ���ˢ���Դ�,�۲�����Ƿ񶶶�
//@PRECONDITION: ƽ̨UDI�Ѿ��ɹ���ʼ��
//@EXPECTATION: ������ֱ��blit���Դ棬�۲�����Ƿ񶶶���
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�Դ�ռ���D
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage�����������ͼƬ����
//@EXECUTIONFLOW:3������CSUDIOSGBlit��ͼƬBlit���Դ���
//@EXECUTIONFLOW:4����ʾ�û�ȷ�ϣ�ͼƬ�Ƿ���ʾ����
//@EXECUTIONFLOW:4������CSUDIOSGDestroySurface����ͼƬ��ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_0029(void)
{
	OSG_iTestDecImageToDisplay(EM_OSG_DECIMAGE_JPG,EM_JPG_720_576);
	OSG_iClearScreen();
    return CSUDI_TRUE;
}

//@CASEGROUP: CSTC_OSG_IT_0030
//@DESCRIPTION: JPGͼƬ������ALPHAֵӦΪ0xFF,ͼƬ����ֱ��ͨ��memcpy��ʽ���Դ�
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��ƽ̨֧��JPG���͵�ͼƬ����
//@INPUT:1��ͼƬUDI2OSG_720_576.jpg�����ݵ�ַ
//@INPUT:2��ͼƬUDI2OSG_720_576.jpg�ĳ���
//@EXPECTATION: 1���ܹ��ɹ���ͼƬ���н��룬����CSUDI_SUCCESS��
//@EXPECTATION: 2������õ������ݿ���ͨ��memcpy�ķ�ʽ�������Դ���,������ȷ��ʾ
//@REMARK: 
//@EXECUTIONFLOW:1����ȡUDI2OSG_720_576.jpgͼƬ���ݵ��ڴ��ַ�ͳ���
//@EXECUTIONFLOW:2������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDI_SUCCESS������õ���phSurfaceӦ�ò�ΪCSUDI_NULL
//@EXECUTIONFLOW:3������CSUDIOSGGetSurfaceInfo��ȡ������ͼƬ�ڴ������
//@EXECUTIONFLOW:4������CSUDIOSGGetDisplaySurface�ӿڻ�ȡ�Դ�ռ�hDisSurface���
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡ�Դ��ַ������
//@EXECUTIONFLOW:6������memcpy�ķ�ʽ,��ͼƬ�������Դ���
//@EXECUTIONFLOW:7��Ӧ������Ļ�Ͽ���UDI2OSG_720_576.jpgͼƬ��Ч������������������������ʧ�ܡ�
//@EXECUTIONFLOW:8������,����surface,�ͷŵ�UDI2OSG_720_576.jpgռ�õ��ڴ档
CSUDI_BOOL CSTC_OSG_IT_0030(void)
{
	CSUDI_BOOL bRet = CSUDI_TRUE;
	
	bRet &= OSG_iTestMemcpyImageToDisplay(EM_OSG_DECIMAGE_IFRAME,EM_IFRAME_720_576);
	
	bRet &= OSG_iTestMemcpyImageToDisplay(EM_OSG_DECIMAGE_JPG,EM_JPG_720_576);
	return bRet;
}

//@CASEGROUP: CSTC_OSG_IT_0031
//@DESCRIPTION: YUVͼƬ����˳��ת����RGBͼƬ,������������ʾ
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@INPUT:1��ͼƬ"fastjpegYUVDataд�ļ�"�����ݵ�ַ
//@EXPECTATION: 1��YUVͼƬ����ת����RGBͼƬ,������ȷ��ʾ
//@REMARK: 
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface����EM_UDIOSG_PIXEL_FORMAT_YCBCR888_422 ��ʽ��,���Ϊ 816*612 ��YUV Surface1
//@EXECUTIONFLOW:3������CSUDIOSGCreateSurface����EM_UDIOSG_PIXEL_FORMAT_ARGB_8888��ʽ,���Ϊ 816*612 ��RGB Surface2
//@EXECUTIONFLOW:5������CSUDIOSGGetDisplaySurface,��ȡ�Դ���SurcaceDisplay
//@EXECUTIONFLOW:2��ͨ���ļ���д�ӿڰѡ�fastjpegYUVData���е����ݶ��� ���洴����YUV Surface��Ӧ���ڴ���
//@EXECUTIONFLOW:4������CSUDIOSGBlit,��EM_UDIOSG_OPMODE_COPYģʽ,��Surface1 �ϵ�����Copy��Surface2��
//@EXECUTIONFLOW:6������CSUDIOSGBlit,��EM_UDIOSG_OPMODE_COPYģʽ,��Surface2 �ϵ�����Copy��SurcaceDisplay��
//@EXECUTIONFLOW:7��������Ļ���ܿ�����"fastjpgTestImage.jpg"һ����ͼƬ��ʾ
//@EXECUTIONFLOW:8������,����surface,�ͷŵ�"fastjpegYUVData"ռ�õ��ڴ档
CSUDI_BOOL CSTC_OSG_IT_0031(void)
{
	return OSG_iTestYUVtoRGB(EM_UDIOSG_PIXEL_FORMAT_YCBCR888_422,EM_YUV_FASTJPG_DATA);
}

//@CASEGROUP: CSTC_OSG_IT_0032
//@DESCRIPTION: ���Ե���CSUDIOSGFill���߶�Ϊ1�����صľ���
//@PRECONDITION:ƽ̨�ѳɹ���ʼ��
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface�õ��Դ�surface ���
//@EXECUTIONFLOW:2������CSUDIOSGCreateSurface������ͼƽ��hDrawSurface������ʾ�߶�Ϊ1�ľ���
//@EXECUTIONFLOW:3������CSUDIOSGCreateSurface������ʱ��ͼƽ��hTempSurface�߶�Ϊ1�ľ������ڱ���
//@EXECUTIONFLOW:4������CSUDIOSGFillΪhDrawSurface����ɫ����
//@EXECUTIONFLOW:5����hDrawSurface�и߶�Ϊ1�ľ��ε����ݱ��ݵ�hTempSurface
//@EXECUTIONFLOW:6������CSUDIOSGSyncͬ��
//@EXECUTIONFLOW:7������CSUDIOSGFill��hDrawSurface�еľ������Ϊ��ɫ
//@EXECUTIONFLOW:8������CSUDIOSGSyncͬ��
//@EXECUTIONFLOW:9������CSUDIOSGBlit����hDrawSurface���Ƶ��Դ��ϣ�ȫ����ʾ
//@EXECUTIONFLOW:10������CSUDIOSGSyncͬ��
//@EXECUTIONFLOW:11��������Ļ�а�ɫ��������ʾһ����ɫ����(�߶�Ϊ1�ľ��ο�)
//@EXECUTIONFLOW:12������CSUDIOSGBlit��hTempSurface�����ݿ�����hDrawSurface�ľ��������ϣ��ָ�hDrawSurface
//@EXECUTIONFLOW:13������CSUDIOSGSyncͬ��
//@EXECUTIONFLOW:14������CSUDIOSGBlit��hDrawSurfaceȫ����ʾ���Դ���
//@EXECUTIONFLOW:15������ȫ����ʾ��ɫ����ɫ������ʧ
//@EXECUTIONFLOW:16������,����CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_0032(void)
{
	CSUDIOSGRect_S draw_rect;
	CSUDI_HANDLE hDrawSurface = NULL;
	CSUDI_HANDLE hDisplaySurface = NULL;
	CSUDI_HANDLE hTempSurface = NULL;
	CSUDI_Error_Code error_code;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);

	CSUDIOSGGetDisplaySurface(OSG_GetTestDisplaySurfaceIndex(), &hDisplaySurface); 
	
	 //������ʱSURFACE��������ʾ�߶�Ϊ1�ľ���
	CSTK_ASSERT_TRUE_FATAL(
							(CSUDI_SUCCESS ==CSUDIOSGCreateSurface(ePixelFormat, 720, 576, &hDrawSurface)),
							"create hDrawSurface failure"); 

	//������ʱSURFACE������hDrawSurface��δ�����ɫ֮ǰ������ 
	CSTK_ASSERT_TRUE_FATAL(
							(CSUDI_SUCCESS ==CSUDIOSGCreateSurface(ePixelFormat, 300, 1, &hTempSurface)),
							 "create hTempSurface failure");  
	// ����ɫ����
	error_code = CSUDIOSGFill(hDrawSurface, NULL, OSG_COLORKEY_WHITE);
	if(error_code!=CSUDI_SUCCESS)
	{
		CSTCPrint("Fill  hDrawSurface failure\n"); 	
		return FALSE;
	}
	draw_rect.m_nX = 100;
	draw_rect.m_nY = 100;
	draw_rect.m_nWidth = 300;
	draw_rect.m_nHeight = 1;

	// ���ݻ���
	CSUDIOSGBlit(hTempSurface, NULL, hDrawSurface, &draw_rect, EM_UDIOSG_OPMODE_COPY);
	CSUDIOSGSync();
	
	// ��仺��
	CSUDIOSGFill(hDrawSurface, &draw_rect,OSG_COLOR_RED_32BIT);
	CSUDIOSGSync();
	
	// ��ʾ���� ȫ��
	CSUDIOSGBlit(hDisplaySurface, NULL, hDrawSurface, NULL, EM_UDIOSG_OPMODE_COPY);
	CSUDIOSGSync();
	CSTCPrint(" ������ɫ��������ʾ��һ����ɫ����(�߶�Ϊ1)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), " Wait failure");
	
	// �ָ�����
	CSUDIOSGBlit( hDrawSurface, &draw_rect, hTempSurface, NULL, EM_UDIOSG_OPMODE_COPY);
	CSUDIOSGSync();
	
	CSUDIOSGBlit(hDisplaySurface, NULL , hDrawSurface, NULL, EM_UDIOSG_OPMODE_COPY);
	CSUDIOSGSync();  
	CSTCPrint(" ����ȫ����ʾ��ɫ����ɫ������ʧ\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), " Wait failure");

	CSTK_FATAL_POINT
		
	if(hDrawSurface != CSUDI_NULL)
	{
		if(CSUDIOSGDestroySurface(hDrawSurface)!=CSUDI_SUCCESS)
		{
			CSTCPrint("Destroy hDrawSurface failure\n"); 	
		}
		
		hDrawSurface = CSUDI_NULL;
	}
	if(hTempSurface != CSUDI_NULL)
	{
		if(CSUDIOSGDestroySurface(hTempSurface)!=CSUDI_SUCCESS)
		{
			CSTCPrint("Destroy hTempSurface failure"); 	
		}
		
		hTempSurface = CSUDI_NULL;
	}
	
	OSG_iClearScreen();
	
	return CSUDI_TRUE;
	
}

//@CASEGROUP: CSTC_OSG_IT_0033
//@DESCRIPTION: ���Ե���CSUDIOSGBlitģ��2D��Ч��moveЧ��
//@PRECONDITION:ƽ̨�ѳɹ���ʼ��
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface�õ��Դ�surface ���
//@EXECUTIONFLOW:2������CSUDIOSGCreateSurface����һ������ȫ����С�Ļ�ͼƽ��hBgSurface
//@EXECUTIONFLOW:3������CSUDIOSGCreateSurface����һ��100 * 100��С��hForeSurface
//@EXECUTIONFLOW:4������CSUDIOSGFillΪhForeSurface���0xFFFF0000����
//@EXECUTIONFLOW:5������CSUDIOSGBlit���Դ�����ݱ��ݵ�hbgsurface
//@EXECUTIONFLOW:6������CSUDIOSGBlit��hForeSurface������blit���Դ�
//@EXECUTIONFLOW:7������CSUDIOSGBlit�ָ��������ϰ벿���������ݡ�
//@EXECUTIONFLOW:8������CSUDIOSGBlit�ָ��������°벿���������ݡ�
//@EXECUTIONFLOW:9������CSUDIOSGSyncͬ��
//@EXECUTIONFLOW:11��ѭ��ִ������Ĳ���100�Σ�ͳ������
//@EXECUTIONFLOW:12������CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_0033(void)
{
	CSUDIOSGOpMode_E eOpMode = EM_UDIOSG_OPMODE_COPY;
	unsigned int	 unForeground_color = 0xFFFF0000;
	
	return OSG_iTESTPortalPerformance(eOpMode, unForeground_color);
}

//@CASEGROUP: CSTC_OSG_IT_0034
//@DESCRIPTION: ģ������ƵĲ�������
//@PRECONDITION:ƽ̨�ѳɹ���ʼ��
//@EXECUTIONFLOW:1����ȡ����������bmpͼƬ
//@EXECUTIONFLOW:2������CSUDIOSGCreateSurface����һ��hfontsurface����bmp��������䵽hfontsurface�С�
//@EXECUTIONFLOW:3������CSUDIOSGCreateSurface����һ��4000 * 50��hsurface_S��
//@EXECUTIONFLOW:4������CSUDIOSGFillΪ4000 * 50��hsurface_S����ɫ����
//@EXECUTIONFLOW:5������CSUDIOSGBlit����hfontsurface������blit��4000*50��hsurface_S�ϡ�
//@EXECUTIONFLOW:6������CSUDIOSGGetDisplaySurface�õ��Դ�hDispsurface �����
//@EXECUTIONFLOW:7������CSUDIOSGBlit����hsurface_S������blit��hDispsurface�ϡ�
//@EXECUTIONFLOW:8������CSUDIOSGSyncͬ��
//@EXECUTIONFLOW:9���ı�ڰ˲�blit��Ŀ��ƫ�����꣬�ظ�4~8��
//@EXECUTIONFLOW:10������,����CSUDIOSGDestroySurface���ٻ�ͼƽ�桢�ͷ�ͼƬ��Դ
CSUDI_BOOL CSTC_OSG_IT_0034(void)
{	
	return OSG_iTestMarquee();
}

float Q_rsqrt( float number )
{

	float  y = number;
	int i =0;

	for(i = 0; i< 3000; i++)
	{
	  y = 1/y;
	  y = y * 1005 /((y + 999)/1003 * 0.77);
	  y = y * 1006 /((y + 999)/1007 * 0.88);
	}
	
  	return 1/y;
}

/**************************************************************
The function for Thread module test
**************************************************************/

int g_Thread = 0;
void OSG_TestThreadEntry1(void * pvParam)
{
	unsigned int i = 999999;
	float ftemp = 0.0;

	while(g_Thread == 0)
	{
		DWORD dwStartTime, dwEndTime;
		dwStartTime = CSUDIPlusOSGetTickCount();	
		i++;
		
		ftemp = Q_rsqrt(i);
		
		dwEndTime = CSUDIPlusOSGetTickCount();

		if(i > 0xFFFFFF00)//��ֹ�������
		{
			i = 0;
		}
		CSUDIOSThreadSleep(3);
	};

	g_Thread++;

	CSTCPrint(" ��ת�߳��Զ��˳�\n");
}

void OSG_TestThreadEntry2(void * pvParam)
{
	CSUDIOSGOpMode_E eOpMode = EM_UDIOSG_OPMODE_COPY;
	unsigned int	 unForeground_color = 0xFFFF0000;
	int i;
	
	CSTCPrint(" OSG�߳̽���\n");
	for(i = 0; i< 10; i++)
	{
		if(g_Thread == 0)
		{
			OSG_iTESTPortalPerformance(eOpMode, unForeground_color);	
		}		
	}
	
	g_Thread++;

	CSTCPrint(" OSG�߳��Զ��˳�\n");
}

//@CASEGROUP: CSTC_OSG_IT_0035
//@DESCRIPTION:���Ե������������ȼ��ߵ��߳�ռ�ô���cpu��ʱ��osg��ͼ�Ƿ�������
//@PRECONDITION:ƽ̨�ѳɹ���ʼ��
//@EXPECTATION:ִ�е�move��Ч��������������Ļ�����Ϸ��ƶ������·���
//@EXECUTIONFLOW:1�������̵߳����ȼ�Ϊ255�� ����CSUDIOSThreadCreate��������OS_TestThread1������CSUDI_SUCCESS,�����������ʧ��
//@EXECUTIONFLOW:2�������̵߳����ȼ�Ϊ255�� ����CSUDIOSThreadCreate��������OS_TestThread2������CSUDI_SUCCESS,�����������ʧ��
//@EXECUTIONFLOW:3�������̵߳����ȼ�Ϊ255�� ����CSUDIOSThreadCreate��������OS_TestThread3������CSUDI_SUCCESS,�����������ʧ��
//@EXECUTIONFLOW:4�������̵߳����ȼ�Ϊ7�� ����CSUDIOSThreadCreate��������OS_TestThreadosg������CSUDI_SUCCESS,�����������ʧ��
//@EXECUTIONFLOW:5������OS_TestThread1��OS_TestThread2��OS_TestThread3��ִ���Լ����㡣����CSUDIOSThreadSleep �ȴ�1ms��Ȼ��g_bThreadExitΪTRUEʱ�˳��̡߳�
//@EXECUTIONFLOW:6������OS_TestThreadosg��ִ��10��move��Ч,ÿ�μ��5���롣��g_bThreadExitΪTRUEʱ�˳��̡߳�
//@EXECUTIONFLOW:7�����������е���CSUDIOSThreadSleep �ȴ�5�룬����g_bThreadExitΪTRUE��
//@EXECUTIONFLOW:8�����������е���CSUDIOSThreadDestroyɾ�����������񲢷���CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:9����Чִ����ϣ���osg�߳������˳�������CSUDI_SUCCESS��
CSUDI_BOOL CSTC_OSG_IT_0035( void )
{
	CSUDI_HANDLE hThreadhandle1 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle2 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle3 = CSUDI_NULL;	
	CSUDI_HANDLE hThreadhandleosg = CSUDI_NULL;		
	int nPriorityHigh = 32;
	int nPriorityLow  = 7;
	int nParam = 0;	
	int nStckSize =  128*1024;
	g_Thread = 0; 

	CSTCPrint("��ע����Ļ�Ϻ�ɫС������ƶ���Ч�Ƿ�����\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OSG_TestThread1", nPriorityHigh , nStckSize, 
							OSG_TestThreadEntry1, &nParam, &hThreadhandle1),
			 "step 1 ��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle1,"step 1 �����ɹ����������������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OSG_TestThread2", nPriorityHigh , nStckSize, 
							OSG_TestThreadEntry1, &nParam, &hThreadhandle2),
			 "step 2 ��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle2,"step 2 �����ɹ����������������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OSG_TestThread3", nPriorityHigh, nStckSize, 
							OSG_TestThreadEntry1, &nParam, &hThreadhandle3),
			 "step 3 ��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle3,"step 3 �����ɹ����������������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OSG_TestThread4", nPriorityLow , nStckSize, 
							OSG_TestThreadEntry2, &nParam, &hThreadhandleosg),
			 "step 4 ��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandleosg,"step 4 �����ɹ����������������������Ϊ��");

	CSUDIOSThreadSleep(8000);

	g_Thread++;	
	//�ȴ��߳��Զ��˳�
	while(1)
	{
		if(g_Thread == 5)
		{
			break;
		}
		CSUDIOSThreadSleep(100);
	}	

	CSTCPrint("��ɫС������ƶ���Ч�Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()),"2D��Ч�������߳�Ӱ������!");	

	CSTK_FATAL_POINT

	if (hThreadhandle1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle1)),"Destroy hThreadhandle1 failure!");
		hThreadhandle1 = CSUDI_NULL;
	}

	if (hThreadhandle2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle2)),"Destroy hThreadhandle2 failure!");
		hThreadhandle2 = CSUDI_NULL;
	}	
	
	if (hThreadhandle3 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle3)),"Destroy hThreadhandle3 failure!");
		hThreadhandle3 = CSUDI_NULL;
	}

	if (hThreadhandleosg != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandleosg)),"Destroy hThreadhandleosg failure!");
		hThreadhandleosg = CSUDI_NULL;
	}

	OSG_iClearScreen();
	
	return CSUDI_TRUE;
}


//@CASEGROUP: CSTC_OSG_IT_0036
//@DESCRIPTION: ����osg�Ƿ������ص�ƫ�ƣ�������Ļ��Χ��(����˼·�������εĿ������������˵��û��ƫ��)
//@PRECONDITION:ƽ̨�ѳɹ���ʼ��
//@EXECUTIONFLOW:1����ȡ������Դ档
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ�����Դ����Ϣ��
//@EXECUTIONFLOW:3������CSUDIOSGFill�����Դ��С��һ������(��ɫ)��
//@EXECUTIONFLOW:4��������ľ����ĸ���������һ�����أ�����CSUDIOSGFill���˾���(��ɫ)��
//@EXECUTIONFLOW:5��������ľ����ĸ���������һ�����أ�����CSUDIOSGFill���˾���(��ɫ)��
//@EXECUTIONFLOW:6��ѭ��ֱ�����εĸ߶�Ϊ1��
//@EXECUTIONFLOW:7������CSUDIOSGBlit����hsurface_S������blit��hDispsurface�ϡ�
//@EXECUTIONFLOW:8������CSUDIOSGSyncͬ��
//@EXECUTIONFLOW:10 ������,����CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_0036(void)
{	
	return OSG_iTestScreenOffset();
}

//@CASEGROUP: CSTC_OSG_IT_0037
//@DESCRIPTION: �����������ʱ��blit���Դ��Ƿ������صĽ���״���(����ͼƬ�����������Դ�)��
//@PRECONDITION:ƽ̨�ѳɹ���ʼ��
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface��������1280 * 720��surface1, surface2
//@EXECUTIONFLOW:2����surface1�����search.bmp���ݣ���surface2�����avsetting.bmp����
//@EXECUTIONFLOW:3������CSUDIOSGBlit����surface1�����ݣ�blit�������Դ棬surface1�Ļ�ͼ�߶�����Ϊ600
//@EXECUTIONFLOW:4������CSUDIOSGSyncͬ��
//@EXECUTIONFLOW:5������CSUDIOSGBlit����surface2�����ݣ�blit�������Դ棬surface2�Ļ�ͼ�߶�����Ϊ600
//@EXECUTIONFLOW:6������CSUDIOSGSyncͬ��
//@EXECUTIONFLOW:7������CSUDIOSGBlit����surface1�����ݣ�blit�������Դ棬surface1�Ļ�ͼ�߶�����Ϊ600
//@EXECUTIONFLOW:8������CSUDIOSGSyncͬ��
//@EXECUTIONFLOW:9������CSUDIOSGBlit����surface2�����ݣ�blit�������Դ棬surface2�Ļ�ͼ�߶�����Ϊ600
//@EXECUTIONFLOW:10������CSUDIOSGSyncͬ��
//@EXECUTIONFLOW:11���ֱ�ѭ������Ĳ���10�Σ��۲���Ļ�Ƿ������صĽ���״��ͼ
//@EXECUTIONFLOW:12�����ٶ�ȡ��������ԭʼͼƬ����
//@EXECUTIONFLOW:13 ������,����CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_0037(void)
{
	return OSG_iTestScreenSerrate();
}

//@CASEGROUP: CSTC_OSG_IT_0038
//@DESCRIPTION: ���Ե��Դ�ľֲ�ˢ���Ƿ���ڶ�ʧ���ݵ����(����˼���ǽ���Ļ�ֳ�16*16�ķ��飬������齻�����Դ�)
//@PRECONDITION:ƽ̨�ѳɹ���ʼ��
//@EXECUTIONFLOW:1������CSUDIOSGCreateSurface����һ��1280 * 720��surface
//@EXECUTIONFLOW:2����ȡsearch.bmp���ݣ�����������䵽surface��
//@EXECUTIONFLOW:3������CSUDIOSGFill�����Դ�Ϊ��ɫ
//@EXECUTIONFLOW:4������CSUDIOSGBlit����surface�����ݣ�blit�������Դ棬���ݵĽ�������Ϊ(0,0,15,15)
//@EXECUTIONFLOW:5������CSUDIOSGBlit����surface�����ݣ�blit�������Դ棬���ݵĽ�������Ϊ(16,0,31,0)
//@EXECUTIONFLOW:6������CSUDIOSGBlit����surface�����ݣ�blit�������Դ棬���ݵĽ�������Ϊ(32,0,47,0)
//@EXECUTIONFLOW:7�����մ����ң����ϵ��£����ε���CSUDIOSGBlitֱ���������򵽴���Ļ�����½�
//@EXECUTIONFLOW:8���۲���Ļ���Ƿ��к�ɫ�ķ������
//@EXECUTIONFLOW:9�����ٶ�ȡ��������ԭʼͼƬ����
//@EXECUTIONFLOW:10 ������,����CSUDIOSGDestroySurface���ٻ�ͼƽ��
CSUDI_BOOL CSTC_OSG_IT_0038(void)
{	
	return OSG_iTestScreenlost();
}

//@CASEGROUP: CSTC_OSG_IT_Resolution_0001
//@DESCRIPTION: �߱���ֱ�����������л���OSG�Ƿ������쳣
//@PRECONDITION:ƽ̨�ѳɹ���ʼ��
//@EXECUTIONFLOW:1������CSUDISCREENGetResolution��ȡ��ǰ��Ļ�ֱ��ʣ�
//@EXECUTIONFLOW:2������CSUDIOSGGetDisplaySurface��ȡ�߱���surface��
//@EXECUTIONFLOW:3������CSUDIOSGGetSurfaceInfo��ȡ�߱���surface����Ϣ��
//@EXECUTIONFLOW:4������CSUDIOSGCreateSurface������ʱsurface��
//@EXECUTIONFLOW:5������CSUDIOSGGetSurfaceInfo��ȡ��ʱsurface����Ϣ��
//@EXECUTIONFLOW:6������CSUDIOSGSetSurfaceInfo����������ʱsurface������
//@EXECUTIONFLOW:7������CSUDIOSGBlit����ʱsurface��������ʾ���߱���surface�ϣ�
//@EXECUTIONFLOW:8������CSUDIOSGSyncͬ����
//@EXECUTIONFLOW:9������CSUDIOSGDestroySurface������ʱsurface��
//@EXECUTIONFLOW:10����ε���CSUDISCREENSetResolution�л��ֱ��ʣ�
//@EXECUTIONFLOW:11������ʾ��������жϣ�
//@EXECUTIONFLOW:12������CSUDISCREENSetResolution�ָ��ֱ��ʣ�
//@EXECUTIONFLOW:13 ��������
BOOL CSTC_OSG_IT_Resolution_0001(void)
{
	CSUDISCREENType_E scScreenDevice[2] = {EM_UDI_VOUT_DEVICE_SD,EM_UDI_VOUT_DEVICE_HD};
	CSUDISCREENResolution_E scResolution[2][2];
	CSUDISCREENResolution_E aeOldResolution[2];
	CSUDI_BOOL bRet = CSUDI_TRUE;
	int i;
	int nIsHD = 0;
	int nMinDisplayId = 0;
	int nMaxDisplayId = 0;
	CSUDI_HANDLE hDisplaySurface = CSUDI_NULL;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;
	CSUDI_HANDLE ahTmpSurface[2] = {CSUDI_NULL};
	unsigned char * ppImage = NULL;
	unsigned int unImageSize = 0;
	CSUDI_Error_Code error_code = CSUDI_FAILURE;

	if (OSG_iTestSupportHDDisplay())
	{
		if (OSG_IsShareHDDisplaySurface())
		{
			nMinDisplayId = 1;
			nMaxDisplayId = 1;
			CSTCPrint("�߱���ͬԴ��ֻ���Ը����Դ�\n");
		}
		else
		{
			nMinDisplayId = 0;
			nMaxDisplayId = 1;
			CSTCPrint("�߱��岻ͬԴ���ֱ���Ը���ͱ����Դ�\n");
		}
	}
	else
	{
		nMinDisplayId = 0;
		nMaxDisplayId = 0;
		CSTCPrint("����ƽ̨��ֻ���Ա����Դ�\n");
	}

	for (nIsHD=nMinDisplayId; nIsHD<=nMaxDisplayId; nIsHD++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[nIsHD], &aeOldResolution[nIsHD]),
			"step 1 ��ȡ��Ļ�ֱ���ʧ��!");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(nIsHD, &hDisplaySurface), 
			"step 2 ��ȡ��ʾ�豸ʧ��!");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIOSGGetSurfaceInfo(hDisplaySurface, &sSurfaceInfo),
			"step 3 ��ȡ��ʾ�豸surface��Ϣʧ��!");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGCreateSurface(sSurfaceInfo.m_ePixelFormat, 1280, 720, &ahTmpSurface[nIsHD]),
			"step 4 ����surfaceʧ��!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != ahTmpSurface[nIsHD],
			"step 4 ����surfaceʧ��!");
			
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIOSGGetSurfaceInfo(ahTmpSurface[nIsHD], &sSurfaceInfo),
			"step 5 ��ȡ��ʾ�豸surface��Ϣʧ��!");
		
		unImageSize = OSG_iGetPhoto("UDI2OSG_search.bmp", &ppImage);
		CSTK_ASSERT_TRUE_FATAL(ppImage  != CSUDI_NULL, "step 8 ��ȡ����ͼƬ����ʧ��!");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iSetSurfaceBuf(ahTmpSurface[nIsHD], ppImage,unImageSize, 1280, 720),
			"step 5 ����surface������ʧ��!");

		CSUDIOSFree(ppImage);
		ppImage = NULL;
			
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGBlit(hDisplaySurface, NULL, ahTmpSurface[nIsHD], NULL, EM_UDIOSG_OPMODE_COPY),
			"step 7 Blit surfaceʧ��!");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGSync(), 
			"step 8 ͬ��surfaceʧ��!");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (error_code = CSUDIOSGDestroySurface(ahTmpSurface[nIsHD])), 
			"step 9 ������ʱsurfaceʧ��!");		

		ahTmpSurface[nIsHD] = CSUDI_NULL;

		CSTCPrint("���潫��ʼ�л��ֱ���(��10��)\n");

		switch(nIsHD)
		{
			case 0:
				CSTCPrint("��ע��۲��������ڷֱ����л�ʱͼƬ��ʾ�Ƿ�����\n");
				break;
			case 1:
				CSTCPrint("��ע��۲��������ڷֱ����л�ʱͼƬ��ʾ�Ƿ�����\n");
				break;
			default:
				CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "�Դ���������");
				break;
		}

		CSTKWaitAnyKey( );
		
		scResolution[0][0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[0][1] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1][0]  = EM_UDISCREEN_RESOLUTION_720P_50HZ;
		scResolution[1][1]  = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
		
		for(i = 0; i < 9; i ++)
		{
			CSTCPrint("��%d���л��ֱ���: %d(%d:PAL, %d:NTSC, %d:720P_50HZ, %d:1080I_50HZ)! \n", i+1, scResolution[nIsHD][i%2], \
				EM_UDISCREEN_RESOLUTION_PAL, EM_UDISCREEN_RESOLUTION_NTSC, EM_UDISCREEN_RESOLUTION_720P_50HZ, EM_UDISCREEN_RESOLUTION_1080I_50HZ);
			CSTCPrint("Now change resolution! \n");
			CSTKWaitAnyKey( );
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[nIsHD], &scResolution[nIsHD][i%2] , 1),
				"step 10 ������Ļ�ֱ���ʧ��!");

			CSTCPrint("ͼƬ�Ƿ���������ʾ? \"y\" ����, \"n\"�����쳣.\n");
			CSTCPrint("Whether osg is normal? \"y\" for normal, \"n\"if not.\n");
			if (nIsHD == 0)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == (bRet = CSTKWaitYes( )),
					"step 11 ����ֱ�����������л���OSG�����쳣");
			}
			else
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == (bRet = CSTKWaitYes( )),
					"step 11 ����ֱ�����������л���OSG�����쳣");
			}
		}
	}

	CSTK_FATAL_POINT;

	for (nIsHD=nMinDisplayId; nIsHD<=nMaxDisplayId; nIsHD++)
	{
		if(ahTmpSurface[nIsHD] != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGDestroySurface(ahTmpSurface[nIsHD]), 
				"������ʱsurfaceʧ��!");	
			ahTmpSurface[nIsHD] = CSUDI_NULL;
		}
				
		//�ָ�����ǰ�ֱ���
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[nIsHD], &aeOldResolution[nIsHD], 1),
			"step 12 �ָ���Ļ�ֱ���ʧ��!");
	}

	OSG_iClearScreen();
	
	return bRet;
}

//@CASEGROUP: CSTC_OSG_IT_POWERON_SCENE_0001
//@DESCRIPTION: ��������portal����
//@PRECONDITION:ƽ̨�ѳɹ���ʼ��
//@EXECUTIONFLOW:1������CSUDISCREENGetResolution��ȡ��ǰ��Ļ�ֱ��ʣ�
//@EXECUTIONFLOW:2������CSUDIOSGGetDisplaySurface��ȡ�߱���surface��
//@EXECUTIONFLOW:3������CSUDIOSGGetSurfaceInfo��ȡ�߱���surface����Ϣ��
//@EXECUTIONFLOW:4������CSUDISCREENSetResolution���ñ���ֱ��ʣ�
//@EXECUTIONFLOW:5������CSUDIOSGCreateSurface����һ��1280X720��hFrameSurface��
//@EXECUTIONFLOW:6������CSUDIOSGFill���hFrameSurfaceΪ��ɫ��
//@EXECUTIONFLOW:7������CSUDIOSGSyncͬ����
//@EXECUTIONFLOW:8������CSUDIOSGBlit��hFrameSurface��������ʾ���߱���surface�ϣ�
//@EXECUTIONFLOW:9������CSUDIOSGSyncͬ����
//@EXECUTIONFLOW:10������CSUDISCREENSetOSDTransparency����͸����Ϊ90��
//@EXECUTIONFLOW:11���ٴε���CSUDIOSGBlit��hFrameSurface��������ʾ���߱���surface�ϣ�
//@EXECUTIONFLOW:12������CSUDIOSGSyncͬ��;
//@EXECUTIONFLOW:13������CSUDIOSGCreateSurface����һ��1280X720��hBgSurface;
//@EXECUTIONFLOW:14������CSUDIOSGGetSurfaceInfo��ȡhBgSurface����Ϣ��
//@EXECUTIONFLOW:15������CSUDIOSGSetSurfaceInfo��������hBgSurface������;
//@EXECUTIONFLOW:16������CSUDIOSGBlit��hBgSurface������blit��hFrameSurface��
//@EXECUTIONFLOW:17������CSUDIOSGSyncͬ����
//@EXECUTIONFLOW:18������CSUDIOSGDestroySurface������ʱhBgSurface��
//@EXECUTIONFLOW:19������CSUDIOSGCreateSurface����115X110��hMapSurface��
//@EXECUTIONFLOW:20������CSUDIOSGGetSurfaceInfo��ȡhMapSurface����Ϣ��
//@EXECUTIONFLOW:21������CSUDIOSGSetSurfaceInfo��������hMapSurface������;
//@EXECUTIONFLOW:22������CSUDIOSGBlit����alphaģʽ��hMapSurface������blit ��hFrameSurface��
//@EXECUTIONFLOW:23������CSUDIOSGSyncͬ����
//@EXECUTIONFLOW:24������CSUDIOSGDestroySurface����115X110��hMapSurface��
//@EXECUTIONFLOW:25���ظ�18~23����7��
//@EXECUTIONFLOW:26������CSUDIOSGBlit��hFrameSurface��������ʾ���߱���surface�ϣ�
//@EXECUTIONFLOW:27������CSUDIOSGSyncͬ����
//@EXECUTIONFLOW:28��ȷ����ʾ�����
//@EXECUTIONFLOW:29������CSUDIOSGDestroySurface����1280X720��hFrameSurface��
//@EXECUTIONFLOW:30������CSUDISCREENSetResolution�ָ��ֱ��ʣ�
//@EXECUTIONFLOW:31��������
BOOL CSTC_OSG_IT_POWERON_SCENE_0001(void)
{
	CSUDI_HANDLE hDisplaySurface = CSUDI_NULL;
	CSUDI_HANDLE hFrameSurface = CSUDI_NULL;
	CSUDI_HANDLE hBgSurface = CSUDI_NULL;
	CSUDI_HANDLE hMapSurface = CSUDI_NULL;
	CSUDIOSGRect_S sBgRect = {0,0, 1280, 720};
	CSUDIOSGRect_S sMapRect[7];
	CSUDIOSGSurfaceInfo_S sDisplaySurfaceInfo;
    	CSUDI_HANDLE hDisplay_HD = CSUDI_NULL;
    	CSUDI_BOOL bIsHD = OSG_iTestSupportHDDisplay();
	int nIndex = 0;
	int i = 0;
	BOOL bRet = CSUDI_TRUE;
	CSUDISCREENType_E eScreenDevice[2] = {EM_UDI_VOUT_DEVICE_SD,EM_UDI_VOUT_DEVICE_HD};
	CSUDISCREENResolution_E scResolution = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
	CSUDISCREENResolution_E scOldResolution = EM_UDISCREEN_RESOLUTION_INVALID;
	unsigned char * ppImage = NULL;
	unsigned int unImageSize = 0;
	const char pcNvFile[7][128] = {
		"UDI2OSG_portal_nv1_115_110_32.bmp",
		"UDI2OSG_portal_nv2_115_110_32.bmp",
		"UDI2OSG_portal_nv3_115_110_32.bmp",
		"UDI2OSG_portal_nv4_115_110_32.bmp",
		"UDI2OSG_portal_nv5_115_110_32.bmp",
		"UDI2OSG_portal_nv6_115_110_32.bmp",
		"UDI2OSG_portal_nv7_115_110_32.bmp",
	};
	
	nIndex = OSG_GetTestDisplaySurfaceIndex();

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(eScreenDevice[nIndex], &scOldResolution),
		"step 1 ��ȡ��Ļ�ֱ���ʧ��!");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(nIndex, &hDisplaySurface),
		"step 2 ��ȡ��ʾsurfaceʧ��!");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIOSGGetSurfaceInfo(hDisplaySurface, &sDisplaySurfaceInfo),
		"step 3 ��ȡ��ʾsurface��Ϣʧ��!");

	//����ֱ���
	if(nIndex == 0)
	{
		scResolution = EM_UDISCREEN_RESOLUTION_PAL;
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&eScreenDevice[nIndex], &scResolution, 1),
		"step 4 ���÷ֱ���ʧ��!");

	if(bIsHD)
    {
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(1, &hDisplay_HD),
            		"step 3 ��ȡ��ʾsurfaceʧ��!!");
    }
    
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGCreateSurface(sDisplaySurfaceInfo.m_ePixelFormat, sBgRect.m_nWidth, sBgRect.m_nHeight, &hFrameSurface),
		"step 5 ����surfaceʧ��!");
	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iFillAndSync(hFrameSurface, &sBgRect, 0),
		"step 6 ���surfaceʧ��!");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hDisplaySurface, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
		"step 8 ��ʾsurfaceʧ��!");

	if(bIsHD)
    {
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hDisplay_HD, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
            		"step 3 ��ȡ��ʾsurfaceʧ��!!");
    }

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetOSDTransparency(90),
		"step 10 ����OSD ͸����ʧ��!");
	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hDisplaySurface, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
		"step 11 ��ʾsurfaceʧ��!");
		
    if(bIsHD)
    {
        CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hDisplay_HD, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
            "step 3 ��ʾsurface!!");
    }

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGCreateSurface(sDisplaySurfaceInfo.m_ePixelFormat, sBgRect.m_nWidth, sBgRect.m_nHeight, &hBgSurface),
		"step 13 ����surfaceʧ��!");

	unImageSize = OSG_iGetPhoto("UDI2OSG_portal_bg_1280_720_32.bmp", &ppImage);
	CSTK_ASSERT_TRUE_FATAL(ppImage  != CSUDI_NULL, "step 14 ��ȡ����ͼƬ����ʧ��!");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iSetSurfaceBuf(hBgSurface, ppImage,unImageSize, 1280, 720),
		"step 14 ����surface������ʧ��!");

	CSUDIOSFree(ppImage);
	ppImage = NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hFrameSurface, NULL, hBgSurface, NULL, EM_UDIOSG_OPMODE_COPY),
		"step 16 ��ʾsurfaceʧ��!");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hBgSurface),
		"step 18 ����surface ʧ��!");

	hBgSurface = NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hDisplaySurface, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
		"step 16 ��ʾsurfaceʧ��!");
		
    if(bIsHD)
    {
        CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hDisplay_HD, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
            "step 3 ��ʾsurface!!");
    
    }

	CSTCPrint("�������㽫������Ļ����ʾһ��ͼƬ! \n");
	CSTCPrint("Next you can see some pictures display on the screen! \n");
	CSTKWaitAnyKey();
	
	sMapRect[0].m_nX = 80;
	sMapRect[0].m_nY = 45;
	sMapRect[0].m_nWidth = 115;
	sMapRect[0].m_nHeight = 110;
	for(i = 0; i < 7; i ++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGCreateSurface(sDisplaySurfaceInfo.m_ePixelFormat, 115, 110, &hMapSurface),
			"step 19 ����hMapSurfaceʧ��!");

		unImageSize = OSG_iGetPhoto(pcNvFile[i], &ppImage);
		CSTK_ASSERT_TRUE_FATAL(ppImage  != CSUDI_NULL, "step 20 ��ȡ����ͼƬ����ʧ��!");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iSetSurfaceBuf(hMapSurface, ppImage,unImageSize, 115, 110),
			"step 20 ����surface������ʧ��!");

		CSUDIOSFree(ppImage);
		ppImage = NULL;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hFrameSurface, &sMapRect[i], hMapSurface, NULL, EM_UDIOSG_OPMODE_ALPHA),
			"step 22 blitʧ��!");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hMapSurface),
			"step 24 ����hMapSurfaceʧ��!");

		hMapSurface = CSUDI_NULL;

		if(i < 6)
		{
            //��Щ����ֻ���ڱ�������ϵ��Թ�
            int tmp = 0;
            switch(i)
            {
                case 0:
                    tmp = 20;
                    break;
                case 1:
                    tmp = 40;
                    break;
                case 2:
                    tmp = 90;
                    break;
                case 3:
                    tmp = 75;
                    break;
                case 4:
                    tmp = 65;
                    break;
                case 5:
                    tmp = 20;
                    break;
            }
			sMapRect[i + 1].m_nX = sMapRect[i].m_nX + sMapRect[i].m_nWidth + tmp;
			sMapRect[i + 1].m_nY = sMapRect[i].m_nY;
			sMapRect[i + 1].m_nWidth = 115;
			sMapRect[i + 1].m_nHeight = 110;
		}
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hDisplaySurface, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
		"step 26 blitʧ��!");
    if(bIsHD)
    {
        CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hDisplay_HD, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
            "step 3 blitʧ��!!!");
    }

	CSTCPrint("ͼƬ�Ƿ���������ʾ? \"y\" ����, \"n\"�����쳣.\n");
	CSTCPrint("Whether the pictures show  normal? \"y\" for normal, \"n\"if not.\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== (bRet = CSTKWaitYes( )),
		"step 28 portalͼƬ��ʾ������!");

	CSTK_FATAL_POINT
		
	if(hFrameSurface != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hFrameSurface),
			"step 29 ����hFrameSurfaceʧ��!");
	}

	if(hBgSurface!= CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hBgSurface),
			"����hBgSurfaceʧ��!");
	}

	if(hMapSurface != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hMapSurface),
			"����hMapSurfaceʧ��!");
	}

	if(ppImage != NULL)
	{
		CSUDIOSFree(ppImage);
	}

	//�ָ�����ǰ�ֱ���
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&eScreenDevice[nIndex], &scOldResolution, 1),
		"step 30 �ָ���Ļ�ֱ���ʧ��!");
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetOSDTransparency(100),
        	"����OSD ͸����ʧ��!");

	OSG_iClearScreen();
	
	return bRet;
}

//@CASEGROUP: CSTC_OSG_IT_GLOBAL_EFFECT_0001
//@DESCRIPTION: ȫ��������Ч����
//@PRECONDITION:ƽ̨�ѳɹ���ʼ��
//@INPUT:����1280*720��bmpͼƬ��λ����Ŀ��ƽ̨��λ���뱣��һ��
//@EXPECTATION: ��ʾbmpͼƬ�����ݣ����������ŵ�Ч��
//@EXECUTIONFLOW:1������CSUDIOSGGetDisplaySurface��ȡ�߱���surface��
//@EXECUTIONFLOW:2������CSUDIOSGGetSurfaceInfo��ȡ�߱���surface����Ϣ��
//@EXECUTIONFLOW:3�� ����CSUDIOSGCreateSurface����1280*720��frame_surface
//@EXECUTIONFLOW:4������CSUDIOSGFill���frame_surface͸��ɫ
//@EXECUTIONFLOW:5������CSUDIOSGSyncͬ����
//@EXECUTIONFLOW:6������CSUDIOSGBlit��copyģʽblit frame_surface ���Դ�
//@EXECUTIONFLOW:7������CSUDIOSGSyncͬ����
//@EXECUTIONFLOW:8������CSUDIOSGCreateSurface����1280*720��deflation_surface
//@EXECUTIONFLOW:9������CSUDIOSGBlit��copyģʽblit frame_surface ��deflation_surface
//@EXECUTIONFLOW:10������CSUDIOSGSyncͬ����
//@EXECUTIONFLOW:11������CSUDIOSGCreateSurface����1280*720��zoom_surface
//@EXECUTIONFLOW:12������CSUDIOSGBlit��copyģʽblit frame_surface ��zoom_surface
//@EXECUTIONFLOW:13������CSUDIOSGSyncͬ����
//@EXECUTIONFLOW:14������CSUDIOSGFill�Ժ�ɫ���frame_surface
//@EXECUTIONFLOW:15������CSUDIOSGSyncͬ����
//@EXECUTIONFLOW:16������CSUDIOSGCreateSurface����1280*720��icon_surface
//@EXECUTIONFLOW:17�� ����CSUDIOSGBlit��copyģʽblit frame_surface��icon_surface
//@EXECUTIONFLOW:18������CSUDIOSGSyncͬ����
//@EXECUTIONFLOW:19�� ����CSUDIOSGBlit��copyģʽblit icon_surface��frame_surface
//@EXECUTIONFLOW:20������CSUDIOSGSyncͬ����
//@EXECUTIONFLOW:21�� ����CSUDIOSGCreateSurface��������tmp_surface
//@EXECUTIONFLOW:22�� ����CSUDIOSGBlit��copyģʽblit frame_surface��tmp_surface
//@EXECUTIONFLOW:23������CSUDIOSGSyncͬ����
//@EXECUTIONFLOW:24�� ����CSUDIOSGBlit��alphaģʽblit tmp_surface��frame_surface
//@EXECUTIONFLOW:25������CSUDIOSGSyncͬ����
//@EXECUTIONFLOW:26�� ����CSUDIOSGSetSurfaceInfo����deflation_surface
//@EXECUTIONFLOW:27�� ����CSUDIOSGBlit��alphaģʽblit deflation_surface��frame_surface
//@EXECUTIONFLOW:28������CSUDIOSGSyncͬ����
//@EXECUTIONFLOW:29�� ����CSUDIOSGDestroySurface����tmp_surface
//@EXECUTIONFLOW:30�� ��ʾframe_surface
//@EXECUTIONFLOW:31���ظ�����19~30
//@EXECUTIONFLOW:32��ȷ����С��Ч����ʾ�����
//@EXECUTIONFLOW:33�� ����CSUDIOSGBlit��copyģʽblit icon_surface��frame_surface
//@EXECUTIONFLOW:34������CSUDIOSGSyncͬ����
//@EXECUTIONFLOW:35�� ����CSUDIOSGCreateSurface��������tmp_surface
//@EXECUTIONFLOW:36�� ����CSUDIOSGBlit��copyģʽblit frame_surface��tmp_surface
//@EXECUTIONFLOW:37������CSUDIOSGSyncͬ����
//@EXECUTIONFLOW:38�� ����CSUDIOSGBlit��alphaģʽblit tmp_surface��frame_surface
//@EXECUTIONFLOW:39������CSUDIOSGSyncͬ����
//@EXECUTIONFLOW:40�� ����CSUDIOSGSetSurfaceInfo����zoom_surface
//@EXECUTIONFLOW:41�� ����CSUDIOSGBlit��alphaģʽblit zoom_surface��frame_surface
//@EXECUTIONFLOW:42������CSUDIOSGSyncͬ����
//@EXECUTIONFLOW:43�� ����CSUDIOSGDestroySurface����tmp_surface
//@EXECUTIONFLOW:44�� ��ʾframe_surface
//@EXECUTIONFLOW:45���ظ�����33~44
//@EXECUTIONFLOW:46��ȷ����С��Ч����ʾ�����
//@EXECUTIONFLOW:47�� ����CSUDIOSGDestroySurface����icon_surface
//@EXECUTIONFLOW:48�� ����CSUDIOSGDestroySurface����deflation_surface
//@EXECUTIONFLOW:49�� ����CSUDIOSGDestroySurface����zoom_surface
//@EXECUTIONFLOW:50�� ����CSUDIOSGDestroySurface����frame_surface
//@EXECUTIONFLOW:51��������
BOOL CSTC_OSG_IT_GLOBAL_EFFECT_0001(void)
{
	BOOL bRet = CSUDI_TRUE;
	CSUDI_HANDLE hFrameSurface = CSUDI_NULL;
	CSUDI_HANDLE hDisplaySurface = CSUDI_NULL;
	CSUDI_HANDLE hIconSurface = CSUDI_NULL;
	CSUDI_HANDLE hDeflationSurface = CSUDI_NULL;
	CSUDI_HANDLE hZoomSurface = CSUDI_NULL;
	CSUDI_HANDLE hTmpSurface = CSUDI_NULL;
    CSUDI_HANDLE hDisplay_HD = CSUDI_NULL;
    CSUDI_BOOL bIsHD = OSG_iTestSupportHDDisplay();

	int nWidth = 1280;
	int nHeight = 720;
	int nStep = 0;
	int i = 0;
	int nIndex;
	CSUDIOSGSurfaceInfo_S sDisplaySurfaceInfo;
	CSUDIOSGRect_S sScreenRect = {0, 0, 1280, 720};
	CSUDIOSGRect_S sZoomRect = {0, 0, 1280, 720};
	unsigned int unFrameBgColor = 0xFF000000;
	unsigned char * ppImage = NULL;
	unsigned int unImageSize = 0;
	
	nIndex = OSG_GetTestDisplaySurfaceIndex();
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(nIndex, &hDisplaySurface),
		"step 1 ��ȡ��ʾsurfaceʧ��!");

	if(bIsHD)
    	{
        	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(1, &hDisplay_HD),
            		"step 3 ��ȡ��ʾsurfaceʧ��!!");
    	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hDisplaySurface, &sDisplaySurfaceInfo),
		"step 2 ��ȡsurface����Ϣ!");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGCreateSurface(sDisplaySurfaceInfo.m_ePixelFormat, nWidth, nHeight, &hFrameSurface),
		"step 3 ����hFrameSurfaceʧ��!");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iFillAndSync(hFrameSurface, &sScreenRect, 0),
		"step 4 ���hFrameSurfaceʧ��!");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hDisplaySurface, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
		"step 6 ��ʾʧ��!");
		
    if(bIsHD)
    {
        CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hDisplay_HD, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
            "step 6 blitʧ��!!!");
    
    }

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGCreateSurface(sDisplaySurfaceInfo.m_ePixelFormat, nWidth, nHeight, &hDeflationSurface),
		"step 8 ������Сsurfaceʧ��!");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hDeflationSurface, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
		"step 9 blitʧ��!");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGCreateSurface(sDisplaySurfaceInfo.m_ePixelFormat, nWidth, nHeight, &hZoomSurface),
		"step 11 �����Ŵ�surfaceʧ��!");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hZoomSurface, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
		"step 12 blitʧ��!");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iFillAndSync(hFrameSurface, NULL, unFrameBgColor),
		"step 14 ���ʧ��!");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGCreateSurface(sDisplaySurfaceInfo.m_ePixelFormat, nWidth, nHeight, &hIconSurface),
		"step 16 ��������surfaceʧ��!");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hIconSurface, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
		"step 17 blit ʧ��!");

	CSTCPrint("������������Ļ��ʾһ������С��ͼƬ! \n");
	CSTCPrint("Next you can see a deflation picture displayed on the screen! \n");
	CSTKWaitAnyKey();
	
	nStep = (nWidth/68) > (nHeight/36)  ? nHeight/36 : nWidth/68;

	//��С
	do
	{
		i++;
		sZoomRect.m_nWidth = (nStep - i) * nWidth/nStep;
		sZoomRect.m_nHeight= (nStep - i) * nHeight/nStep;
		sZoomRect.m_nX = (nWidth -sZoomRect.m_nWidth)/2;
		sZoomRect.m_nY = (nHeight -sZoomRect.m_nHeight)/2;

		CSTCPrint("{%d, %d, %d, %d}\n", sZoomRect.m_nX, sZoomRect.m_nY, sZoomRect.m_nWidth, sZoomRect.m_nHeight);
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hFrameSurface, NULL, hIconSurface, NULL, EM_UDIOSG_OPMODE_COPY),
			"step 19 blitʧ��!");
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGCreateSurface(sDisplaySurfaceInfo.m_ePixelFormat, sZoomRect.m_nWidth, sZoomRect.m_nHeight, &hTmpSurface),
			"step 21 ������ʱsurfaceʧ��!");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hTmpSurface, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
			"step 22 blit ʧ��!");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hFrameSurface, &sZoomRect, hTmpSurface, NULL, EM_UDIOSG_OPMODE_ALPHA),
			"step 24 blit ʧ��!");

		unImageSize = OSG_iGetPhoto("UDI2OSG_search.bmp", &ppImage);

		CSTK_ASSERT_TRUE_FATAL((ppImage  != CSUDI_NULL) && (unImageSize != 0), "step 16 ��ȡ����ͼƬ����ʧ��!");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iSetSurfaceBuf(hDeflationSurface, ppImage, unImageSize, 1280, 720),
			"step 26 ������Сsurface������ʧ��!");

		CSUDIOSFree(ppImage);
		ppImage = NULL;
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hFrameSurface, &sZoomRect, hDeflationSurface, NULL, EM_UDIOSG_OPMODE_ALPHA),
			"step 27 blit ʧ��!");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hTmpSurface),
			"step 29 ������ʱsurfaceʧ��!");
		
		hTmpSurface = CSUDI_NULL;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hDisplaySurface, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
			"step 30 blitʧ��!");

		if(bIsHD)
        	{
            		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hDisplay_HD, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
                		"step 30 blitʧ��!!!");
        	}
	}while(i < nStep - 1);

	CSTCPrint("ͼƬ�Ƿ���������ʾ����С����Ч? \"y\" ����, \"n\"�����쳣\n");
	CSTCPrint("Whether the picture show  with deflation effect? \"y\" for normal, \"n\"if not.\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== (bRet = CSTKWaitYes( )),
		"step 32 portalͼƬ��ʾ������!");

	CSTCPrint("������������Ļ��ʾһ���𽥷Ŵ��ͼƬ! \n");
	CSTCPrint("Next you can see a deflation picture displayed on the screen! \n");
	CSTKWaitAnyKey();
	
	i = 2;
	do
	{
		i++;
		sZoomRect.m_nWidth = i * nWidth/nStep;
		sZoomRect.m_nHeight= i * nHeight/nStep;
		sZoomRect.m_nX = (nWidth -sZoomRect.m_nWidth)/2;
		sZoomRect.m_nY = (nHeight -sZoomRect.m_nHeight)/2;
		CSTCPrint("{%d, %d, %d, %d}\n", sZoomRect.m_nX, sZoomRect.m_nY, sZoomRect.m_nWidth, sZoomRect.m_nHeight);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hFrameSurface, NULL, hIconSurface, NULL, EM_UDIOSG_OPMODE_COPY),
			"step 33 blit ʧ��!");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGCreateSurface(sDisplaySurfaceInfo.m_ePixelFormat, sZoomRect.m_nWidth, sZoomRect.m_nHeight, &hTmpSurface),
			"step 35 ������ʱsurfaceʧ��!");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hTmpSurface, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
			"step 36 blitʧ��!");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hFrameSurface, &sZoomRect, hTmpSurface, NULL, EM_UDIOSG_OPMODE_ALPHA), 
			"step 38 blit ʧ��!");

		unImageSize = OSG_iGetPhoto("UDI2OSG_avsetting.bmp", &ppImage);
		CSTK_ASSERT_TRUE_FATAL(ppImage  != CSUDI_NULL, "step 25 ��ȡ����ͼƬ����ʧ��!");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iSetSurfaceBuf(hZoomSurface, ppImage, unImageSize, 1280, 720),
			"step 40 ���÷Ŵ�surface����ʾ����!");
			
		CSUDIOSFree(ppImage);
		ppImage = NULL;
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hFrameSurface, &sZoomRect, hZoomSurface, NULL, EM_UDIOSG_OPMODE_ALPHA),
			"step 41 blit ʧ��!");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hTmpSurface),
			"step 43 ������ʱsurfaceʧ��!");
		
		hTmpSurface = CSUDI_NULL;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hDisplaySurface, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY), 
			"step 44 blitʧ��!");

		if(bIsHD)
        	{
            		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hDisplay_HD, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
                		"step 44 blitʧ��!!!");
        	}
	}while(i < nStep - 1);

	CSTCPrint("ͼƬ�Ƿ���������ʾ�𽥷Ŵ����Ч? \"y\" ����, \"n\"�����쳣\n");
	CSTCPrint("Whether the picture show  with  zoom effect? \"y\" for normal, \"n\"if not.\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== (bRet = CSTKWaitYes( )),
		"step 46 portalͼƬ��ʾ������!");
	
	CSTK_FATAL_POINT
	if(hTmpSurface != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hTmpSurface),
			"����hTmpSurfaceʧ��!");
	}

	if(hIconSurface != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hIconSurface),
			"step 47 ����hIconSurfaceʧ��!");
	}

	if(hDeflationSurface != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hDeflationSurface),
			"step 48 ����hDeflationSurfaceʧ��!");
	}
	
	if(hZoomSurface != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hZoomSurface),
			"step 49 ����hZoomSurfaceʧ��!");
	}
	
	if(hFrameSurface != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hFrameSurface),
			"step 50 ����hFrameSurfaceʧ��!");
	}

	if(ppImage != NULL)
	{
		CSUDIOSFree(ppImage);
	}

	OSG_iClearScreen();
	
	return bRet;
}


