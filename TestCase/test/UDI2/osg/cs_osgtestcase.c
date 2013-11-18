/**
 *@version 1.0.8 2009/10/09 根据新的测试框架进行重构
 *@version 1.0.7 2009/09/30 重构为分支上新的框架
 *@version 1.0.6 2009/09/15 去掉从全屏到中央屏幕blit的用例，修改alpha为0时引起死机问题
 *@version 1.0.5 2009/09/03 代码走读，修改在其它平台上编译不过的变量声明位置问题
 *@version 1.0.4 2009/08/29 接口更改，用例进行同步
 *@version 1.0.3 2009/07/17 完成初始版本
 *@version 1.0.2 2009/07/18 增加预审后的用例
 *@version 1.0.1 2009/07/27 增加评审后的用例
 */
/* --------------------------------------------------------------------
注意：
1.在需要与用户交互的测试用例中，可以：
	a. 使用CSTKWaitAnyKey等待用户输入任意按键
	b. 使用CSTKWaitYes等待用户输入YES
2.测试用例函数命名：测试用例ID，"测试用例ID"定义在测试用例文档中
-----------------------------------------------------------------------*/
#include "cs_osgtestcase.h"
#include "cs_osgtest.h"
#include <time.h>
#include "udi2_player.h"
#include "barportal.h"
#include "coshipportal.h"
#include "udi2_screen.h"


//blit操作重复执行的次数
#define TEST_MAX_BLIT_TIME  (2<<10)

extern unsigned int g_uSurfaceMaxWidth;
extern unsigned int g_uSurfaceMaxHeight;
extern unsigned char bmp32data_h[1658882];


//@TESTCASEID: CSTC_OSG_TEST_Init
//@CASEGROUP: 
//@DESCRIPTION:初始化测试环境,读取配置文件信息。
//@DESCRIPTION:配置文件信息包括平台支持的颜色模式，平台是否支持高标清显存同时存在，平台支持创建的最大缓存空间大小。
//@REMARK:该接口必须在系统初始化时调用。
CSUDI_BOOL CSTC_OSG_Init(void)
{
	//在本测试用例集执行前调用
	return (CS_TK_CONFIG_SUCCESS == OSG_iInitCfg());
}

//@TESTCASEID:CSTC_OSG_TEST_UnInit
//@CASEGROUP:
//@DESCRIPTION:去初始化测试环境
CSUDI_BOOL CSTC_OSG_UnInit(void)
{
	//在本测试用例集执行后调用
	return (CS_TK_CONFIG_SUCCESS == OSG_iUnInitCfg());
}

//@TESTCASEID:CSTC_OSG_IT_CreateSurface_0001
//@CASEGROUP:CSUDIOSGCreateSurface
//@DESCRIPTION:测试创建绘图平面时传入非法的参数
//@PRECONDITION:平台UDI已经成功初始化
//@EXPECTATION:  创建绘图平面失败，返回为CSUDIOSG_ERROR_BAD_PARAMETER 
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建非法绘图，期望创建平面失败
CSUDI_BOOL CSTC_OSG_IT_CreateSurface_0001(void)
{   
	CSUDI_HANDLE				hSurface = CSUDI_NULL;
	CSUDIOSGPixelFormat_E 		eErrorPixelFormat[] = {-1, EM_UDIOSG_PIXEL_FORMAT_NUM};
	int 						nErrorWidth[] = {-1, 0};
	int 						nErrorHeight[] = {-1, 0};
	int							i = 0;
	CSUDI_Error_Code			nResult = CSUDI_FAILURE;
	CSUDIOSGPixelFormat_E		ePixelFormat = -1;

	//寻找一个支持的ePixelFormat
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"没有找到任何支持的满足要求的PixelFormat"
			);
	
	//对phSurface 为空的检测
	for(i = 0; i < 1; i++)
	{
		nResult = CSUDIOSGCreateSurface(ePixelFormat, 1, 1, CSUDI_NULL);
		CSTK_ASSERT_TRUE_FATAL((CSUDIOSG_ERROR_BAD_PARAMETER == nResult), "对phSurface 参数判断错误");
	}
	//对错误ePixelFormat 的检测
	for(i = 0; i < sizeof(eErrorPixelFormat)/sizeof(eErrorPixelFormat[0]); i++)
	{
		nResult = CSUDIOSGCreateSurface(eErrorPixelFormat[i], 1, 1, &hSurface);
		CSTK_ASSERT_TRUE_FATAL((CSUDIOSG_ERROR_BAD_PARAMETER == nResult)&&(CSUDI_NULL == hSurface), "对ePixelFormat 参数判断错误");
	}
	//对错误nWidth 的检测
	for(i = 0; i < sizeof(nErrorWidth)/sizeof(nErrorWidth[0]); i++)
	{
		nResult = CSUDIOSGCreateSurface(ePixelFormat, nErrorWidth[i], 1, &hSurface);
		CSTK_ASSERT_TRUE_FATAL((CSUDIOSG_ERROR_BAD_PARAMETER == nResult)&&(CSUDI_NULL == hSurface), "对nWidth 参数判断错误");
        if((CSUDIOSG_ERROR_BAD_PARAMETER == nResult)&&(CSUDI_NULL == hSurface))
		{
			CSTCPrint("错误的参数nWidth =%d\n",nErrorWidth[i]);
        }
    }
	//对错误Height 的检测
	for(i = 0; i < sizeof(nErrorHeight)/sizeof(nErrorHeight[0]); i++)
	{
		nResult = CSUDIOSGCreateSurface(ePixelFormat, 1, nErrorHeight[i], &hSurface);
		CSTK_ASSERT_TRUE_FATAL((CSUDIOSG_ERROR_BAD_PARAMETER == nResult)&&(CSUDI_NULL == hSurface), "对nHeight 参数判断错误");
        if((CSUDIOSG_ERROR_BAD_PARAMETER == nResult)&&(CSUDI_NULL == hSurface))
        {
		  	CSTCPrint("错误的参数nHeight =%d\n",nErrorHeight[i]);
        }
	}

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_CreateSurface_0002
//@CASEGROUP:CSUDIOSGCreateSurface
//@DESCRIPTION:确认可生成100个10*10的surface.使用配置文件中的CS_OSGUDI2_SURPORT_HD_SD_DISPLAY字段区分高标清界面.由该字段区分创建全屏surface的大小.确认可连续生成全屏surface的个数
//@PRECONDITION:平台UDI已经成功初始化
//@EXPECTATION:  创建绘图平面成功
//@EXECUTIONFLOW:1、调用获取配置文件接口，由CS_OSGUDI2_SURPORT_HD_SD_DISPLAY字段区分高标清界面，确定界面大小
//@EXECUTIONFLOW:2、调用CSUDIOSGCreateSurface创建绘图，颜色模式是EM_UDIOSG_PIXEL_FORMAT_ARGB_8888或EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,大小是10 * 10,期望创建平面成功
//@EXECUTIONFLOW:3、循环调用步骤1共100次.
//@EXECUTIONFLOW:4、循环调用CSUDIOSGDestroySurface,销毁步骤1-2生成的surface
//@EXECUTIONFLOW:5、调用CSUDIOSGCreateSurface创建绘图，颜色模式是EM_UDIOSG_PIXEL_FORMAT_ARGB_8888或EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,大小是全屏,期望创建平面成功
//@EXECUTIONFLOW:6、循环调用步骤4,当调用步骤4失败时打印调用步骤4的次数.
//@EXECUTIONFLOW:7、循环调用CSUDIOSGDestroySurface,销毁步骤4生成的surface
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
					"没有找到任何支持的满足要求的PixelFormat"
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
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == OSG_GetSize(nValue, &nWidth,&nHeight)), "判断高标清错误!!!!!");
	CSTCPrint("高标清标识=%d , surface Width =%d , Height =%d\n",nValue,nWidth,nHeight);
	
	for(i = 0; i < 100; i++)
	{
		nResult = CSUDIOSGCreateSurface(ePixelFormat, 10, 10, &hSurface[i]);
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult), "surface创建失败");
	}
	for(i = 0; i < 100; i++)
	{
		if( hSurface[i] != CSUDI_NULL)
		{
			nResult = CSUDIOSGDestroySurface( hSurface[i]);
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult), "surface销毁失败");
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
	CSTCPrint("共创建了%d个全屏surface(标清平台要求至少能创建5个，高清平台10个)\n",i);
	
	for(i = 0; i < 100; i++)
	{
		if( hSurface[i] != CSUDI_NULL)
		{
			nResult = CSUDIOSGDestroySurface( hSurface[i]);
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult), "surface销毁失败");
			hSurface[i] = CSUDI_NULL;
		}
	}	

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_CreateSurface_0003
//@CASEGROUP:CSUDIOSGCreateSurface
//@DESCRIPTION:测试平台可创建全屏surface个数，高清平台要求至少能够创建10个全屏surface，标清平台要求至少能够创建 5个全屏的Surface
//@PRECONDITION:平台UDI已经成功初始化
//@EXPECTATION:  创建绘图平面成功
//@EXECUTIONFLOW:1、调用获取配置文件接口，由CS_OSGUDI2_SURPORT_HD_SD_DISPLAY字段区分高标清界面，确定界面大小
//@EXECUTIONFLOW:2、如果是高清平台，调用CSUDIOSGCreateSurface连续创建绘图平面100个，颜色模式是ARGB8888或ARGB1555，大小是全屏
//@EXECUTIONFLOW:3、判断成功创建的个数，如果小于4个则返回失败
//@EXECUTIONFLOW:4、循环调用CSUDIOSGDestroySurface，销毁步骤2生成的surface
//@EXECUTIONFLOW:5、如果是标清平台，调用CSUDIOSGCreateSurface连续创建颜色模式是EARGB8888或ARGB1555，大小是全屏的绘图平面100个，大小是720*24的绘图平面100个
//@EXECUTIONFLOW:6、判断步骤5成功创建绘图平面的个数，如果小于2个则返回失败
//@EXECUTIONFLOW:7、循环调用CSUDIOSGDestroySurface，销毁步骤5生成的surface
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
					"没有找到任何支持的满足要求的PixelFormat"
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
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == OSG_GetSize(nValue, &nWidth,&nHeight)), "判断高标清错误!!!!!");

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
		CSTK_ASSERT_TRUE_FATAL(i >=4,"创建全屏高清surface个数不符合要求");	
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
		CSTK_ASSERT_TRUE_FATAL(i >=2,"创建surface个数不符合要求");
	}

	CSTK_FATAL_POINT;
		
	for(i = 0; i < 100; i++)
	{
		if( hSurface[i] != CSUDI_NULL)
		{
			nResult = CSUDIOSGDestroySurface( hSurface[i]);
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult), "surface销毁失败");
			hSurface[i] = CSUDI_NULL;
				
		}
		if( hSurfaceD[i] != CSUDI_NULL)
		{
			nResult = CSUDIOSGDestroySurface( hSurfaceD[i]);
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult), "surface销毁失败");
			hSurfaceD[i] = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_CreateSurface_0004
//@CASEGROUP:CSUDIOSGCreateSurface
//@DESCRIPTION:测试平台至少支持ARGB1555或者ARGB8888颜色模式中的一种
//@PRECONDITION:平台UDI已经成功初始化
//@EXPECTATION:  创建ARGB1555或者ARGB8888的surface成功
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建720*576的ARGB1555 surface
//@EXECUTIONFLOW:2、调用CSUDIOSGCreateSurface创建720*576的ARGB8888 surface
//@EXECUTIONFLOW:3、判断成功步骤1和步骤2至少有一个是成功的,并且返回的句柄不为空
//@EXECUTIONFLOW:4、调用CSUDIOSGDestroySurface销毁创建成功的surface
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
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface1555)), "step 4 销毁1555颜色模式surface失败!");
	}
	
	nResult = CSUDIOSGCreateSurface(ePixelFormat8888, nWidth, nHeight, &hSurface8888);
	if(nResult == CSUDI_SUCCESS)
	{
		bLastResult = CSUDI_TRUE;
	}

	if(hSurface8888 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface8888)), "step 4 销毁8888颜色模式surface失败!");
	}	

	CSTK_FATAL_POINT;
	
	return bLastResult;	
}

//@TESTCASEID:CSTC_OSG_IT_CreateSurface_0005
//@CASEGROUP:CSUDIOSGCreateSurface
//@DESCRIPTION:1.测试平台支持创建超过显存大小的surface
//@DESCRIPTION:2.测试平台支持创建细长的3000*50，细高50*3000的surface
//@PRECONDITION:平台UDI已经成功初始化
//@EXPECTATION:  创建surface 成功
//@EXECUTIONFLOW:1、获取平台的显存高宽,假设为Dis_Width,Dis_Weight,注意如果是高清平台应该以高清现存作为Dis_Width,Dis_Weight
//@EXECUTIONFLOW:2、调用CSUDIOSGCreateSurface创建(Dis_Width+100,Dis_Weight+100) 的surface,期望成功
//@EXECUTIONFLOW:3、调用CSUDIOSGDestroySurface销毁创建成功的surface
//@EXECUTIONFLOW:4、调用CSUDIOSGCreateSurface创建3000*50的surface,期望成功
//@EXECUTIONFLOW:5、调用CSUDIOSGDestroySurface销毁创建成功的surface
//@EXECUTIONFLOW:6、调用CSUDIOSGCreateSurface创建50*3000的surface,期望成功
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁创建成功的surface
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
					"没有找到任何支持的满足要求的PixelFormat"
				);

	memset(acConfigBuf, 0, sizeof(acConfigBuf));
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("OSG","CS_OSGUDI2_SURPORT_HD_SD_DISPLAY",acConfigBuf,sizeof(acConfigBuf)),"读取配置文件失败!!!\n");
	
	nValue = CSTKGetIntFromStr(acConfigBuf,10);
	//获取显存的大小
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == OSG_GetSize(nValue, &nDisplayWidth,&nDisplayHeight)), "step 1 获取显存信息失败!!!!!");

	//期望显存大小尺寸surface成功
	nWidth  = nDisplayWidth + 100;
	nHeight = nDisplayHeight+ 100;	
	
	nResult = CSUDIOSGCreateSurface(ePixelFormat, nWidth, nHeight, &hSurface);
	CSTK_ASSERT_TRUE_FATAL((nResult == CSUDI_SUCCESS && hSurface != CSUDI_NULL), "step 2创建超过显存大小为的surface失败!");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface)), "step 3销毁超过显存大小surface失败! ");

	//期望3000 * 50大小尺寸surface成功
	nWidth  = 3000;
	nHeight = 50;
	nResult = CSUDIOSGCreateSurface(ePixelFormat, nWidth, nHeight, &hSurface);
	CSTK_ASSERT_TRUE_FATAL((nResult == CSUDI_SUCCESS && hSurface != CSUDI_NULL), "step 4创建大小为(3000  *  50 )大小的surface失败!");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface)), "step 5 销毁surface(3000  *  50 )失败! ");

	//期望50 * 3000大小尺寸surface成功
	nWidth  = 50;
	nHeight = 3000;
	nResult = CSUDIOSGCreateSurface(ePixelFormat, nWidth, nHeight, &hSurface);
	CSTK_ASSERT_TRUE_FATAL((nResult == CSUDI_SUCCESS && hSurface != CSUDI_NULL), "step 6 创建大小为(50  *  3000 )大小的surface失败!");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface)), "step 7 销毁surface(50  *  3000 )失败! ");

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_CreateSurface_0006
//@CASEGROUP:CSUDIOSGCreateSurface
//@DESCRIPTION:1.测试平台反复创建、销毁surface无资源泄露
//@PRECONDITION:平台UDI已经成功初始化
//@EXPECTATION:  成功创建销毁surface 2000次
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建surface期望成功
//@EXECUTIONFLOW:2、调用CSUDIOSGDestroySurface销毁创建的surface
//@EXECUTIONFLOW:3、重复步骤1和2 一共2000次,期望每次成功
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
					"没有找到任何支持的满足要求的PixelFormat"
				);

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("OSG","CS_OSGUDI2_SURPORT_HD_SD_DISPLAY",acConfigBuf,sizeof(acConfigBuf)),"读取配置文件失败!!!\n");
	
	nValue = CSTKGetIntFromStr(acConfigBuf,10);
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == OSG_GetSize(nValue, &nDisplayWidth,&nDisplayHeight)), "获取显存信息失败!!!!!");

	CSTCPrint("此用例需要较长时间，请耐心等待\n");
	
	//循环创建、释放2000次
	for(i = 0; i< 2000; i++)
	{
		nResult = CSUDIOSGCreateSurface(ePixelFormat, nDisplayWidth, nDisplayHeight, &hSurface);
		CSTK_ASSERT_TRUE_FATAL((nResult == CSUDI_SUCCESS && hSurface != CSUDI_NULL), "step 1 创建显存大小的surface失败!");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface)), "step 2 销毁显存surface失败! ");

		if(i%20 == 0)
		{
			CSTCPrint("%% %d\n", i/20);
		}
	}

	CSTK_FATAL_POINT;
	if(i != 2000)
	{
		CSTCPrint("当前仅创建了%d次", i);
	}

	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_DestroySurface_0001
//@CASEGROUP:CSUDIOSGDestroySurface
//@DESCRIPTION:测试销毁一个绘图平面，测试非法值
//@PRECONDITION:平台UDI已经成功初始化
//@INPUT: CSUDI_NULL
//@EXPECTATION:  销毁失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDIOSGDestroySurface函数，结果返回CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_DestroySurface_0001(void)
{   
	CSTK_ASSERT_TRUE_FATAL((CSUDIOSG_ERROR_BAD_PARAMETER == CSUDIOSGDestroySurface(CSUDI_NULL)),"Step 1 Destroy an invalid surface should be failure !");
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_DestroySurface_0002
//@CASEGROUP:CSUDIOSGDestroySurface
//@DESCRIPTION: 测试销毁一个显存空间(高清，标清)
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT:  由CSUDIOSGGetDisplaySurface获取的句柄
//@EXPECTATION:  不允许销毁显存空间，销毁失败，函数返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄
//@EXECUTIONFLOW:2、调用CSUDIOSGDestroySurface函数销毁显存空间，结果返回CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_DestroySurface_0002(void)
{   
	CSUDI_HANDLE hSurface= CSUDI_NULL;
	
	if(!OSG_IsShareHDDisplaySurface())
	{
		//获取标清显存
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(0, &hSurface) && (CSUDI_NULL != hSurface)),"获取标清显存失败");
		//测试删除显存空间
		CSTK_ASSERT_TRUE_FATAL((CSUDIOSG_ERROR_BAD_PARAMETER == CSUDIOSGDestroySurface(hSurface)),"测试删除标清显存空间出错");
	}
	//在平台支持的情况下获取高清显存
	if(OSG_IsSupportSDHDDisplay())
	{
		hSurface= CSUDI_NULL;
		
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(1, &hSurface) && (CSUDI_NULL != hSurface)),"获取高清显存失败");
		CSTK_ASSERT_TRUE_FATAL((CSUDIOSG_ERROR_BAD_PARAMETER == CSUDIOSGDestroySurface(hSurface)),"测试删除高清显存空间出错");
	}
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_GetDisplaySurface_0001
//@CASEGROUP:CSUDIOSGGetDisplaySurface
//@DESCRIPTION:测试平台display surface 是否有效
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT:显存索引为0,1(如果支持)
//@EXPECTATION:  成功获取不为CSUDI_NULL 的句柄并将fill的颜色显示在屏幕上
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取该显存空间的信息，返回值为CSUDI_SUCCESS
//@EXECUTIONFLOW:3、判断获取到的Surface显示模式，是否为EM_UDIOSG_PIXEL_FORMAT_ARGB_8888或EM_UDIOSG_PIXEL_FORMAT_ARGB_1555
//@EXECUTIONFLOW:4、填充display surface memory 为全红 并显示在屏幕上
//@EXECUTIONFLOW:4、填充display surface memory 为全白 并显示在屏幕上
//@EXECUTIONFLOW:4、填充display surface memory 为全蓝并显示在屏幕上
//@EXECUTIONFLOW:4、填充display surface memory 为全黑 并显示在屏幕上
//@EXECUTIONFLOW:4、填充display surface memory 为全白 并显示在屏幕上
CSUDI_BOOL CSTC_OSG_IT_GetDisplaySurface_0001(void)
{  
	CSUDI_Error_Code errCode = CSUDI_FAILURE;
	CSUDI_HANDLE hSurface = CSUDI_NULL;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;

	errCode = CSUDIOSGGetDisplaySurface(OSG_GetTestDisplaySurfaceIndex(), &hSurface);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS== errCode), "获取显存失败!!!!!");

	memset(&sSurfaceInfo, 0, sizeof(sSurfaceInfo));

	errCode = CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == errCode), "获取显存信息失败!!!!!");

	CSTCPrint("请注意屏幕颜色变化\n");
	CSTKWaitAnyKey();

	//fill surface memory with red 
	CSTCPrint("在屏幕上满屏显示红色\n");
	errCode = CSUDIOSGFill(hSurface, CSUDI_NULL, OSG_COLOR_RED_32BIT);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == errCode), "填充surface失败!!!!!");
	/*<added by lengwenhua at 2010-12-01, after write dispaly buffer, we should use Sync  to display it>*/   
	errCode = CSUDIOSGSync();
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == errCode), "同步信息失败!!!!!");
	CSUDIOSThreadSleep(2000);

	//fill surface memory with white
	CSTCPrint("在屏幕上满屏显示白色\n");
	errCode = CSUDIOSGFill(hSurface, CSUDI_NULL, 0xffffffff);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == errCode), "填充surface失败!!!!!");
	errCode = CSUDIOSGSync();
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == errCode), "同步信息失败!!!!!");
	CSUDIOSThreadSleep(2000);

	//fill surface memory with blue
	CSTCPrint("在屏幕上满屏显示蓝色\n");
	errCode = CSUDIOSGFill(hSurface, CSUDI_NULL, OSG_COLOR_BLUE_32BIT);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == errCode), "填充surface失败!!!!!");
	errCode = CSUDIOSGSync();
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == errCode), "同步信息失败!!!!!");
	CSUDIOSThreadSleep(2000);

	//fill surface memory with black
	CSTCPrint("在屏幕上满屏显示黑色\n");
	errCode = CSUDIOSGFill(hSurface, CSUDI_NULL, 0xff000000);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == errCode), "填充surface失败!!!!!");
	errCode = CSUDIOSGSync();
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == errCode), "同步信息失败!!!!!");
	CSUDIOSThreadSleep(2000);

	//fill surface memory with green
	CSTCPrint("在屏幕上满屏显示绿色\n");
	errCode = CSUDIOSGFill(hSurface, CSUDI_NULL, OSG_COLOR_GREEN_32BIT);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == errCode), "填充surface失败!!!!!");
	errCode = CSUDIOSGSync();
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == errCode), "同步信息失败!!!!!");
	CSUDIOSThreadSleep(2000);

	CSTCPrint("请确认屏幕颜色依次显示为红白蓝黑绿\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "屏幕颜色显示不正确!!!!!");

	CSTK_FATAL_POINT;

	OSG_iClearScreen();

	return TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_GetDisplaySurfaceInfo_0001
//@CASEGROUP:CSUDIOSGGetDisplaySurface
//@DESCRIPTION:测试平台所支持的所有显存类型的创建，并检测其合理性
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT:显存索引为0,1(如果支持)
//@EXPECTATION:  成功获取不为CSUDI_NULL 的句柄
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取该显存空间的信息，返回值为CSUDI_SUCCESS
//@EXECUTIONFLOW:3、使用OSG_iCheckSD(HD)DisplayInfo 检测显存的各种属性
CSUDI_BOOL CSTC_OSG_IT_GetDisplaySurfaceInfo_0001(void)
{   
	CSUDI_HANDLE hSurface = CSUDI_NULL;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;

	if(!OSG_IsShareHDDisplaySurface())
	{
		//创建标清显存
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(0, &hSurface) && (CSUDI_NULL != hSurface)),"创建标清显存失败");
		//获取显存信息
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo)), "获取标清显存信息失败");
		//检测显存信息
		CSTK_ASSERT_TRUE_FATAL(OSG_iCheckSDDisplayInfo(hSurface), "创建标清显存信息与预期不符");
	}
	
	//在支持高清的情况下测试高清显存
	if(OSG_IsSupportSDHDDisplay())
	{
		hSurface = CSUDI_NULL;
		memset(&sSurfaceInfo, 0, sizeof(sSurfaceInfo));
		
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(1, &hSurface) && (CSUDI_NULL != hSurface)),"创建高清显存失败");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo)), "获取高清显存信息失败");
		CSTK_ASSERT_TRUE_FATAL(OSG_iCheckHDDisplayInfo(hSurface), "创建高清显存信息与预期不符");
	}

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_GetDisplaySurfaceInfo_0002
//@CASEGROUP:CSUDIOSGGetDisplaySurface
//@DESCRIPTION:测试显存索引为非法值时获取显存句柄失败
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT:显存索引为5
//@EXPECTATION: 获取显存句柄失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄
//@EXECUTIONFLOW:2、获取显存句柄失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_GetDisplaySurfaceInfo_0002(void)
{   
	CSUDI_HANDLE hSurface = CSUDI_NULL;
	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDIOSG_ERROR_BAD_PARAMETER==CSUDIOSGGetDisplaySurface(-1, &hSurface)) && (hSurface == CSUDI_NULL),
				"对nIndex 错误参数检测失败");
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDIOSG_ERROR_BAD_PARAMETER==CSUDIOSGGetDisplaySurface(2, &hSurface)) && (hSurface == CSUDI_NULL),
				"对nIndex 错误参数检测失败");
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDIOSG_ERROR_BAD_PARAMETER==CSUDIOSGGetDisplaySurface(0, CSUDI_NULL)) && (hSurface == CSUDI_NULL),
				"对phSurface 错误参数检测失败");
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGGetSurfaceInfo
//@DESCRIPTION:测试获取绘图平面信息时传入非法参数
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT:1、传入hSurface句柄为CSUDI_NULL
//@INPUT:2、传入非法sSurfaceInfo
//@EXPECTATION: 返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDIOSGGetSurfaceInfo，结果返回CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_GetSurfaceInfo_0001(void)
{   
	CSUDIOSGPixelFormat_E		ePixelFormat = -1;
	CSUDIOSGSurfaceInfo_S 		sSurfaceInfo;
	CSUDI_HANDLE 			hSurface = CSUDI_NULL;
	CSUDI_Error_Code			nResult = CSUDI_FAILURE;
	
	//寻找一个支持的ePixelFormat
	CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
					"没有找到任何支持的满足要求的PixelFormat"
				);
		
	//创建一个surface
	nResult = CSUDIOSGCreateSurface(ePixelFormat,SURFACE_WIDTH_720,SURFACE_HEIGHT_576,&hSurface);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult) && (CSUDI_NULL != hSurface), "创建surface 失败");

	//分别测试错误参数下执行GetSurfaceInfo
	CSTK_ASSERT_TRUE_FATAL((CSUDIOSG_ERROR_BAD_PARAMETER == CSUDIOSGGetSurfaceInfo(CSUDI_NULL, &sSurfaceInfo)),
			"对hSurface 错误参数检测失败");
	CSTK_ASSERT_TRUE_FATAL((CSUDIOSG_ERROR_BAD_PARAMETER == CSUDIOSGGetSurfaceInfo(hSurface, CSUDI_NULL)),
			"对psSurfaceInfo 错误参数检测失败");
	
	CSTK_FATAL_POINT
	{
		if(hSurface != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface)),"销毁Surface 失败");
			hSurface = CSUDI_NULL;
		}
	}
	
	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_GetSurfaceInfo_0002
//@CASEGROUP:CSUDIOSGGetSurfaceInfo
//@DESCRIPTION:测试获取已经销毁的绘图平面的信息
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT:1、传入的hSurface句柄已经被销毁
//@INPUT:2、传入合法的psSurfaceInfo
//@EXPECTATION: 返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建绘图平面
//@EXECUTIONFLOW:2、调用CSUDIOSGDestroySurface销毁绘图平面，成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIOSGGetSurfaceInfo，结果返回CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_GetSurfaceInfo_0002(void)
{   
	CSUDIOSGPixelFormat_E		ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	CSUDIOSGSurfaceInfo_S 		sSurfaceInfo;
	CSUDI_HANDLE 			hSurface = CSUDI_NULL;	
	CSUDI_Error_Code			nResult = CSUDI_FAILURE;

	//寻找一个支持的ePixelFormat
	CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
					"没有找到任何支持的满足要求的PixelFormat"
				);
		
	//创建一个surface
	nResult = CSUDIOSGCreateSurface(ePixelFormat,SURFACE_WIDTH_720,SURFACE_HEIGHT_576,&hSurface);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult) && (CSUDI_NULL != hSurface), "创建surface 失败");

	//销毁创建的surface
	//注意，此时销毁不将hSurface 至空，否则就成为错误参数的检测了
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface)),"销毁surface 失败");

	//逻辑检测获取一个已经销毁的surface
	CSTK_ASSERT_TRUE_FATAL((CSUDIOSG_ERROR_BAD_PARAMETER == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo)),
							"对已经销毁的surafce 执行CSUDIOSGGetSurfaceInfo 检测失败");
	hSurface = CSUDI_NULL;
	CSTK_FATAL_POINT
	{
		if(hSurface != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface)),"销毁Surface 失败");
			hSurface = CSUDI_NULL;
		}
	}
		
	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_GetSurfaceInfo_0003
//@CASEGROUP:CSUDIOSGGetSurfaceInfo
//@DESCRIPTION:测试可以通过CSUDIOSGGetSurfaceInfo获取到显存的信息
//@PRECONDITION: 平台UDI已经成功初始化
//@EXPECTATION: 1.成功获取到信息
//@EXPECTATION: 2.获取到的surface information与实际情况一致
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取标清显存句柄,期望成功
//@EXECUTIONFLOW:2、如果平台支持高清,调用CSUDIOSGGetDisplaySurface获取高清显存句柄,期望成功
//@EXECUTIONFLOW:3、调用CSUDIOSGGetSurfaceInfo，获取标清显存info,期望成功
//@EXECUTIONFLOW:4、检查m_pvSurfaceMem成员不为空，m_nMemSize大于0,且可以将m_pvSurfaceMem指向的内存m_nMemSize大小进行memset
//@EXECUTIONFLOW:5、检查m_nWidth和m_nHeight应该和实际的显存高宽一致且大于0
//@EXECUTIONFLOW:6、检查获取到的颜色模式是否与显存颜色模式一致
//@EXECUTIONFLOW:7、检查获取到的显存pitch应该大于等于显存宽乘以每个像素所占的字节数
//@EXECUTIONFLOW:8、如果是高清平台则调用CSUDIOSGGetDisplaySurface获取高清显存surface句柄
//@EXECUTIONFLOW:9、调用CSUDIOSGGetSurfaceInfo，获取高清显存info,期望成功
//@EXECUTIONFLOW:10、重复步骤4~7
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
			
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("OSG","CS_OSGUDI2_SURPORT_HD_SD_DISPLAY",acConfigBuf,sizeof(acConfigBuf)),"读取配置文件失败!!!\n");
	
	nValue = CSTKGetIntFromStr(acConfigBuf,10);

	for(i = 0; i<= nValue; i++)
	{
		if(i == 0)
		{
			if(OSG_IsShareHDDisplaySurface())
			{
				continue;
			}
			
			CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("OSG","CS_UDI2OSG_SD_DISPLAY_WIDTH",acConfigBuf,sizeof(acConfigBuf)),"读取配置文件失败!!!\n");
			nExpectWidth = CSTKGetIntFromStr(acConfigBuf,10);
			CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("OSG","CS_UDI2OSG_SD_DISPLAY_HEIGHT",acConfigBuf,sizeof(acConfigBuf)),"读取配置文件失败!!!\n");
			nExpectHeight = CSTKGetIntFromStr(acConfigBuf,10);
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("OSG","CS_UDI2OSG_HD_DISPLAY_WIDTH",acConfigBuf,sizeof(acConfigBuf)),"读取配置文件失败!!!\n");
			nExpectWidth = CSTKGetIntFromStr(acConfigBuf,10);
			CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("OSG","CS_UDI2OSG_HD_DISPLAY_HEIGHT",acConfigBuf,sizeof(acConfigBuf)),"读取配置文件失败!!!\n");
			nExpectHeight = CSTKGetIntFromStr(acConfigBuf,10);
		}
		
		CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("OSG","CS_UDI2OSG_DISPLAY_PIXEL_FORMAT",acConfigBuf,sizeof(acConfigBuf)),"读取配置文件失败!!!\n");
		ePixelFormat = CSTKGetIntFromStr(acConfigBuf,10);

		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(i, &hSurface)),"step 2 获取显存失败!!!!!\n");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo)), "step 3 获取显存信息失败!!!!!");

		//检查surface的信息
		CSTK_ASSERT_TRUE_FATAL((sSurfaceInfo.m_nWidth == nExpectWidth), "step 5 失败，surface info width is error!");
		CSTK_ASSERT_TRUE_FATAL((sSurfaceInfo.m_nHeight == nExpectHeight), "step 5 失败，surface info height is error!");
		CSTK_ASSERT_TRUE_FATAL((sSurfaceInfo.m_pvSurfaceMem != NULL), "step 4 失败，surface 内存不为空!");
		CSTK_ASSERT_TRUE_FATAL((sSurfaceInfo.m_nMemSize > 0), "step 4 失败，surface 的memsize is error!");

		if (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888)
		{
			CSTK_ASSERT_TRUE_FATAL((sSurfaceInfo.m_nPitch >= sSurfaceInfo.m_nWidth * 4), "step 7 失败，surface的pitch不正确!");
		}
		else if(ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)
		{
			CSTK_ASSERT_TRUE_FATAL((sSurfaceInfo.m_nPitch >= sSurfaceInfo.m_nWidth * 2), "step 7 失败，surface的pitch不正确!");
		}
		
		CSTK_ASSERT_TRUE_FATAL((sSurfaceInfo.m_ePixelFormat == ePixelFormat), "step 6 失败，surface的颜色模式不正确!");

		memset(sSurfaceInfo.m_pvSurfaceMem, 0, sSurfaceInfo.m_nMemSize);
	}

	CSTK_FATAL_POINT;

	return CSUDI_FAILURE;	
}

//@TESTCASEID:CSTC_OSG_IT_GetSurfaceInfo_0004
//@CASEGROUP:CSUDIOSGGetSurfaceInfo
//@DESCRIPTION:测试可以通过CSUDIOSGGetSurfaceInfo获取到用户创建的surface的信息
//@PRECONDITION: 平台UDI已经成功初始化
//@EXPECTATION: 1.成功获取到信息
//@EXPECTATION: 2.获取到的surface information与实际情况一致
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建测试surface
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo，获取创建surface的info,期望成功
//@EXECUTIONFLOW:4、检查m_pvSurfaceMem成员不为空，m_nMemSize大于0,且可以将m_pvSurfaceMem指向的内存m_nMemSize大小进行memset
//@EXECUTIONFLOW:5、检查m_nWidth和m_nHeight应该和创建时传入的高宽一致
//@EXECUTIONFLOW:6、检查获取到的颜色模式是否与创建指定的颜色模式一致
//@EXECUTIONFLOW:7、检查获取到的显存pitch应该大于等于surface宽乘以每个像素所占的字节数
//@EXECUTIONFLOW:8、调用CSUDIOSGDestroySurface销毁创建的surface
CSUDI_BOOL CSTC_OSG_IT_GetSurfaceInfo_0004(void)
{
	CSUDI_Error_Code			nResult = CSUDI_FAILURE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	CSUDI_HANDLE hSurface = CSUDI_NULL;
	int nWidth = 720;
	int nHeight= 576;
		
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;

	int nBytes = 0;
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == OSG_iFindSupportPixelFormatAndBytes(&ePixelFormat, &nBytes)), "没有找到任何支持的满足要求的PixelFormat");

	nResult = CSUDIOSGCreateSurface(ePixelFormat, nWidth, nHeight, &hSurface);
	CSTK_ASSERT_TRUE_FATAL((nResult == CSUDI_SUCCESS && hSurface != CSUDI_NULL), "step 1 创建显存大小的surface失败!");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS== CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo)), "step 2获取surface信息失败!!!!!");

	//检查surface的信息
	CSTK_ASSERT_TRUE_FATAL((sSurfaceInfo.m_nWidth == nWidth), "step 5 失败，surface info width is error!");
	CSTK_ASSERT_TRUE_FATAL((sSurfaceInfo.m_nHeight == nHeight), "step 5 失败，surface info height is error!");
	CSTK_ASSERT_TRUE_FATAL((sSurfaceInfo.m_pvSurfaceMem != NULL), "step 4 失败，surface 内存不为空!");
	CSTK_ASSERT_TRUE_FATAL((sSurfaceInfo.m_nMemSize > 0), "step 4 失败，surface 的memsize is error!");
	CSTK_ASSERT_TRUE_FATAL((sSurfaceInfo.m_nPitch >= sSurfaceInfo.m_nWidth * nBytes), "step 7 失败，surface的pitch不正确!");
	CSTK_ASSERT_TRUE_FATAL((sSurfaceInfo.m_ePixelFormat == ePixelFormat), "step 6 失败，surface的颜色模式不正确!");

	CSTK_FATAL_POINT;	
	if(hSurface != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface)),"step 8 销毁显存surface失败! ");
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
//@DESCRIPTION:测试绘图平面信息非法值的情况
//@PRECONDITION:平台UDI已经成功初始化
//@INPUT:1、由CSUDIOSGCreateSurface创建的句柄hSurface
//@INPUT:2、绘图平面信息结构体sSurfaceInfo，模式为ARGB8888或ARGB1555，平面宽高为nWidth = 720, g_nHeight = 576
//@INPUT:3、错误参数
//@EXPECTATION: 每一步返回预期的值
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建绘图平面，要求返回CSUDI_SUCCESS且句柄不为空
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取绘图平面信息，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIOSGSetSurfaceInfo接口设置错误的绘图平面信息，期望返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4、调用CSUDIOSGDestroySurface函数销毁绘图平面
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

	//寻找一个支持的ePixelFormat
	CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
					"没有找到任何支持的满足要求的PixelFormat"
				);
		
	//创建一个surface
	nResult = CSUDIOSGCreateSurface(ePixelFormat,SURFACE_WIDTH_720,SURFACE_HEIGHT_576,&hSurface);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult) && (CSUDI_NULL != hSurface), "创建surface 失败");

	//获得现有surface 的信息
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo),"获得surface 信息失败");
		
	//测试错误值
	for(i = 0; i<sizeof(aErrorPram)/sizeof(aErrorPram[0]); i++)
	{
		sSurfaceInfo.m_nAlphaFlag = aErrorPram[i].m_nAlphaFlag;
		sSurfaceInfo.m_uAlpha = aErrorPram[i].m_uAlpha;
		sSurfaceInfo.m_nColorKeyFlag = aErrorPram[i].m_nColorKeyFlag;
		
		nResult = CSUDIOSGSetSurfaceInfo(hSurface, &sSurfaceInfo);
		CSTK_ASSERT_TRUE_FATAL((CSUDIOSG_ERROR_BAD_PARAMETER == nResult), "检测错误参数失败");
	}
	
	CSTK_FATAL_POINT
	{
		if(hSurface != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface)),"销毁surface 失败");
			hSurface = CSUDI_NULL;
		}
	}
	
	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_SetSurfaceInfo_0002
//@CASEGROUP:CSUDIOSGSetSurfaceInfo
//@DESCRIPTION:测试对不能设置的显存进行设置
//@PRECONDITION:平台UDI已经成功初始化
//@INPUT:1、由GetDisplaySurfaceInfo获取到的显存surface句柄hSurface
//@INPUT:2、绘图平面信息结构体sSurfaceInfo
//@EXPECTATION: 每一步返回预期的值
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存surface，期望返回CSUDI_SUCCESS且句柄不为空
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、修改SurfaceInfo，调用CSUDIOSGSetSurfaceInfo，期望返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4、调用CSUDIOSGSetSurfaceInfo接口设置绘图平面信息，期望返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、比较步骤2和步骤5分别获取到的SurfaceInfo，要求相同
CSUDI_BOOL CSTC_OSG_IT_SetSurfaceInfo_0002(void)
{
	CSUDI_HANDLE				hSurface = CSUDI_NULL;
	CSUDIOSGSurfaceInfo_S		sSurfaceInfo;
	CSUDI_Error_Code			nResult = CSUDI_FAILURE;
	CSUDIOSGSurfaceInfo_S		sSurfaceInfoBake;

	//创建一个surface
	nResult = CSUDIOSGGetDisplaySurface(OSG_GetTestDisplaySurfaceIndex(), &hSurface);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult) && (CSUDI_NULL != hSurface), "创建surface 失败");

	//获得现有surface 的信息
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo),"获得surface 信息失败");

	memcpy(&sSurfaceInfoBake, &sSurfaceInfo, sizeof(CSUDIOSGSurfaceInfo_S));
	//尝试设置显存空间
	sSurfaceInfo.m_uAlpha = 0x11;	
	sSurfaceInfo.m_nAlphaFlag= 1;
	nResult = CSUDIOSGSetSurfaceInfo(hSurface, &sSurfaceInfo);
	CSTK_ASSERT_TRUE_FATAL((CSUDIOSG_ERROR_BAD_PARAMETER == nResult), "不能设置显存空间");

	//这里是用CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED 还是CSUDIOSG_ERROR_BAD_PARAMETER
	//再次获得显存的surface 的信息
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo),"获得surface 信息失败");
	
	CSTK_ASSERT_TRUE_FATAL(0 == memcmp(&sSurfaceInfoBake, &sSurfaceInfo, sizeof(CSUDIOSGSurfaceInfo_S)), "步骤2和步骤5分别获取到的SurfaceInfo不一致");
	
	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_SetSurfaceInfo_0003
//@CASEGROUP:CSUDIOSGSetSurfaceInfo
//@DESCRIPTION:测试对surface进行colorkey和alpha设置
//@PRECONDITION:平台UDI已经成功初始化
//@EXPECTATION: 1.新创建的surface其colorkey和alpha应该默认未设置
//@EXPECTATION: 2.调用CSUDIOSGSetSurfaceInfo能够成功对colorkey和alpha进行设置
//@EXPECTATION: 3.对surface进行colorkey和alpha的设置不影响surface的其他属性
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建绘图平面
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取新创建surface的信息，假设为stInfoA
//@EXECUTIONFLOW:3、检查获取到的信息sSurfaceInfoA，期望sSurfaceInfoA.m_nColorKeyFlag=0,sSurfaceInfoA.m_nAlphaFlag=0,sSurfaceInfoA.m_uColorkey=0,sSurfaceInfoA.m_uAlpha=0xFF
//@EXECUTIONFLOW:4、调用CSUDIOSGSetSurfaceInfo接口设置绘图平面信息成m_nColorKeyFlag = 1,m_nAlphaFlag=1,m_uColorkey=0xFFFF,m_uAlpha=0x55,期望返回成功
//@EXECUTIONFLOW:5、再次调用CSUDIOSGGetSurfaceInfo获取surface的信息假设为sSurfaceInfoB,期望返回成功
//@EXECUTIONFLOW:6、检查sSurfaceInfoB，期望sSurfaceInfoB.m_nColorKeyFlag = 1,sSurfaceInfoB.m_nAlphaFlag=1,sSurfaceInfoB.m_uColorkey=0xFFFF,sSurfaceInfoB.m_uAlpha=0x55
//@EXECUTIONFLOW:7、检查其他不能设置的成员是否发生变化,期望sSurfaceInfoA.m_ePixelFormat = sSurfaceInfoB.m_ePixelFormat;sSurfaceInfoA.m_nWidth = sSurfaceInfoB.m_nWidth;
//@EXECUTIONFLOW:7、sSurfaceInfoA.m_nHeight = stInfoB.m_nHeight;sSurfaceInfoA.m_nPitch = sSurfaceInfoB.m_nPitch;sSurfaceInfoA.m_pvSurfaceMem = sSurfaceInfoB.m_pvSurfaceMem;sSurfaceInfoA.m_nMemSize = sSurfaceInfoB.m_nMemSize
//@EXECUTIONFLOW:8、调用CSUDIOSGDestroySurface销毁surface
CSUDI_BOOL CSTC_OSG_IT_SetSurfaceInfo_0003(void)
{
	CSUDIOSGPixelFormat_E		ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	CSUDI_HANDLE				hSurface = CSUDI_NULL;
	CSUDIOSGSurfaceInfo_S		sSurfaceInfoA;
	CSUDIOSGSurfaceInfo_S		sSurfaceInfoB;	
	CSUDI_Error_Code			nResult = CSUDI_FAILURE;

	memset(&sSurfaceInfoA,0, sizeof(CSUDIOSGSurfaceInfo_S));
	memset(&sSurfaceInfoB,0, sizeof(CSUDIOSGSurfaceInfo_S));	

	//寻找一个支持的ePixelFormat
	CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
					"没有找到任何支持的满足要求的PixelFormat"
				);
		
	//创建一个surface
	nResult = CSUDIOSGCreateSurface(ePixelFormat, 720, 576, &hSurface);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult) && (CSUDI_NULL != hSurface), "创建surface 失败");

	//获得现有surface 的信息
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfoA),"获得surface 信息失败");

	//备份sSurfaceInfoA 的信息到sSurfaceInfoB
	memcpy(&sSurfaceInfoB, &sSurfaceInfoA, sizeof(CSUDIOSGSurfaceInfo_S));	
	
	//检查surface的信息
	CSTK_ASSERT_TRUE_FATAL((sSurfaceInfoB.m_nColorKeyFlag == 0), "step 3 失败，colorkey flag error!");
	CSTK_ASSERT_TRUE_FATAL((sSurfaceInfoB.m_nAlphaFlag == 0), "step 3 失败，alpha flag error!");
	CSTK_ASSERT_TRUE_FATAL((sSurfaceInfoB.m_uColorkey == 0), "step 3 失败，colorkey  error!");
	CSTK_ASSERT_TRUE_FATAL((sSurfaceInfoB.m_uAlpha == 0xff), "step 3 失败，alpha  error!");
	
	//设置surface的信息
	sSurfaceInfoB.m_nColorKeyFlag = 1 ;
	sSurfaceInfoB.m_nAlphaFlag = 1 ;
	sSurfaceInfoB.m_uColorkey = 0xFFFF ;
	sSurfaceInfoB.m_uAlpha = 0x55 ;
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS== CSUDIOSGSetSurfaceInfo(hSurface, &sSurfaceInfoB)), "step 4 failed !不能设置surface的空间");

	memset(&sSurfaceInfoB, 0, sizeof(sSurfaceInfoB));

	//获取现有surface 的信息
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfoB),"step 5 failed !获得surface 信息失败");
	CSTK_ASSERT_TRUE_FATAL((sSurfaceInfoB.m_nColorKeyFlag == 1), "step 4 colorkey flag error!");
	CSTK_ASSERT_TRUE_FATAL((sSurfaceInfoB.m_nAlphaFlag == 1), "step 4 alpha flag error!");
	CSTK_ASSERT_TRUE_FATAL((sSurfaceInfoB.m_uColorkey == 0xFFFF), "step 4 colorkey  error!");
	CSTK_ASSERT_TRUE_FATAL((sSurfaceInfoB.m_uAlpha == 0x55), "step 4 alpha  error!");

	sSurfaceInfoB.m_nColorKeyFlag = 0 ;
	sSurfaceInfoB.m_nAlphaFlag = 0 ;
	sSurfaceInfoB.m_uColorkey = 0 ;
	sSurfaceInfoB.m_uAlpha = 0xff ;

	//将当前的信息与比较之前保存的surfaceinfoB的信息比较
	CSTK_ASSERT_TRUE_FATAL((0== memcmp(&sSurfaceInfoA, &sSurfaceInfoB, sizeof(CSUDIOSGSurfaceInfo_S))), "step 7 校验surface信息失败!");

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
//@DESCRIPTION:测试创建绘图平面时支持创建的颜色模式(遍历创建CSUDIOSGPixelFormat_E枚举中定义的值)
//@DESCRIPTION:并且同时在每个正常创建的情况下正常删除
//@PRECONDITION:平台UDI已经成功初始化
//@INPUT:颜色模式为CSUDIOSGPixelFormat_E枚举中定义的值，并且平台支持该颜色模式
//@EXPECTATION: 成功创建绘图平面
//@REMARK:	此测试用例失败，则之后的测试用例测试结果无意义
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建绘图平面,平面宽度为720,平面高度为576
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取绘图平面的信息
//@EXECUTIONFLOW:3、期望获取的绘图平面的信息跟创建时传入的信息一致
//@EXECUTIONFLOW:4、调用CSUDIOSGDestroySurface函数销毁绘图平面
//@EXECUTIONFLOW:5、调用CSUDIOSGCreateSurface创建绘图平面,平面宽度为1280,平面高度为720
//@EXECUTIONFLOW:6、调用CSUDIOSGGetSurfaceInfo获取绘图平面的信息
//@EXECUTIONFLOW:7、期望获取的绘图平面的信息跟创建时传入的信息一致
//@EXECUTIONFLOW:8、调用CSUDIOSGDestroySurface函数销毁绘图平面
//@EXECUTIONFLOW:9、调用CSUDIOSGCreateSurface创建绘图平面,平面宽度为1920,平面高度为1080
//@EXECUTIONFLOW:10、调用CSUDIOSGGetSurfaceInfo获取绘图平面的信息
//@EXECUTIONFLOW:11、期望获取的绘图平面的信息跟创建时传入的信息一致
//@EXECUTIONFLOW:12、调用CSUDIOSGDestroySurface函数销毁绘图平面
//@EXECUTIONFLOW:循环CSUDIOSGPixelFormat_E枚举中定义的值，执行步骤1到步骤12,期望每一步都成功
CSUDI_BOOL CSTC_OSG_IT_CreateDestorySetGetSurface_0001(void)
{  
	CSUDI_BOOL bRet = CSUDI_TRUE;
	int nIndex = EM_UDIOSG_PIXEL_FORMAT_RGB_565;
	
	for(; nIndex < EM_UDIOSG_PIXEL_FORMAT_NUM; nIndex++)
	{
		bRet &= OSG_CreatSurface_TestPixelFormatAndSize(nIndex,EM_SURFACE_720_576);
		CSTK_ASSERT_TRUE_FATAL(bRet,"CreateDestorySetGetSurface EM_SURFACE_720_576 失败!!!");
		
		if(OSG_IsSupport1280_720())
		{
			bRet &= OSG_CreatSurface_TestPixelFormatAndSize(nIndex,EM_SURFACE_1280_720);
			CSTK_ASSERT_TRUE_FATAL(bRet,"CreateDestorySetGetSurface EM_SURFACE_1280_720 失败!!!");
		}
		
		if(OSG_IsSupport1920_1080())
		{
			bRet &= OSG_CreatSurface_TestPixelFormatAndSize(nIndex,EM_SURFACE_1920_1080);
            		CSTK_ASSERT_TRUE_FATAL(bRet,"CreateDestorySetGetSurface EM_SURFACE_1920_1080 失败!!!");
		}
	}
    
	CSTK_FATAL_POINT

	return bRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_0000
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:<font color=red>注意此用例只是用来说明Blit测试需要的各个参数，并没有实际作用</font>
//@PRECONDITION: 以下是测试blit操作时各种参数的说明
//@INPUT:  **********************************************************************************		      			      		     		    
//@INPUT:  标识符:  P1 
//@INPUT:  属  性:  源缓存空间大小       
//@INPUT:  取值范围:(0，∞）
//@INPUT:
//@INPUT:  测试取值:[720,576]	
//@INPUT:  替代符:  P1_1
//@INPUT:  ===================
//@INPUT:  测试取值:[1280,720]	
//@INPUT:  替代符:  P1_2
//@INPUT:  ------------------------------------------------------------------------------
//@INPUT:  标识符:  P2 
//@INPUT:  属  性:  源缓存空间大小       
//@INPUT:  取值范围:(0，∞）
//@INPUT:
//@INPUT:  测试取值:[720,576]	
//@INPUT:  替代符:  P2_1
//@INPUT:  ===================
//@INPUT:  测试取值:[1280,720]	
//@INPUT:  替代符:  P2_2
//@INPUT:  ------------------------------------------------------------------------------
//@INPUT:  标识符:  D
//@INPUT:  属  性:  显存空间大小       
//@INPUT:  取值范围:(0，∞）
//@INPUT:
//@INPUT:  测试取值:通过CSUDIOSGGetSurfaceInfo获取	
//@INPUT:  替代符:  D
//@INPUT:  ------------------------------------------------------------------------------
//@INPUT:  标识符:  C 
//@INPUT:  属  性:  Colorkey       
//@INPUT:  取值范围:[0x00000000,0xffffffff]
//@INPUT:
//@INPUT:  测试取值:0xff000000 	
//@INPUT:  替代符:  C1
//@INPUT:  ===================
//@INPUT:  测试取值:0xffffffff
//@INPUT:  替代符:  C2
//@INPUT:  ------------------------------------------------------------------------------
//@INPUT:  标识符:  A 
//@INPUT:  属  性:  Alpha       
//@INPUT:  取值范围:[0x00,0xff]
//@INPUT:
//@INPUT:  测试取值:0x40 	
//@INPUT:  替代符:  A1
//@INPUT:  ===================
//@INPUT:  测试取值:0xff
//@INPUT:  替代符:  A2	
//@INPUT:  ------------------------------------------------------------------------------
//@INPUT:  标识符:  Cr 
//@INPUT:  属  性:  颜色值       
//@INPUT:  取值范围:[0x00000000,0xffffffff]
//@INPUT:
//@INPUT:  测试取值:0xffff0000 	
//@INPUT:  替代符:  Cr1
//@INPUT:  ===================
//@INPUT:  测试取值:0xff00ff00
//@INPUT:  替代符:  Cr2	
//@INPUT:  ===================
//@INPUT:  测试取值:0x40ff0000
//@INPUT:  替代符:  Cr3
//@INPUT:  ===================
//@INPUT:  测试取值:0xff0000ff
//@INPUT:  替代符:  Cr4
//@INPUT:  ===================
//@INPUT:  测试取值:颜色值为cs_osgcolordata.c文件中的值
//@INPUT:  替代符:  Cr5
//@INPUT:  ------------------------------------------------------------------------------
//@INPUT:  标识符:  R 
//@INPUT:  属  性:  矩形区域(包括目标和源）       
//@INPUT:  取值范围:合法、非法、边界值(x,y)∈（0，+∞)|（w,h)∈（0，+∞）{x,y,w,h}
//@INPUT:  取值范围:与显存空间交互时，通过CSUDIOSGSurfaceInfo_S获取绘图（显存）空间的大小（w,h)
//@INPUT:
//@INPUT:  测试取值:CSUDI_NULL 	
//@INPUT:  替代符:  R1
//@INPUT:  ===================
//@INPUT:  测试取值:{0，0，w，h}（全屏）
//@INPUT:  替代符:  R2	
//@INPUT:  ===================
//@INPUT:  测试取值:{w/2-100，h/2-100，w/2+100，h/2+100}（中央位置)
//@INPUT:  替代符:  R3
//@INPUT:  ===================
//@INPUT:  测试取值:{0，0，0，0}（非法值）
//@INPUT:  替代符:  R4
//@INPUT:  ===================
//@INPUT:  测试取值:{0，0，-1，-1}（非法值）
//@INPUT:  替代符:  R5
//@INPUT:  ===================
//@INPUT:  测试取值:{0，0，1，1}（一个像素值）
//@INPUT:  替代符:  R6
//@INPUT:  ===================
//@INPUT:  测试取值:{0，0，15，15}（与r6结合测试伸缩15倍，不同绘图空间的情况）
//@INPUT:  替代符:  R7
//@INPUT:  ===================
//@INPUT:  测试取值:{0,h-100,100,h+100}（左下角越界）
//@INPUT:  替代符:  R8
//@INPUT:  ===================
//@INPUT:  测试取值:{w/2,h-100,w/2+100,h+100}（底中边越界）
//@INPUT:  替代符:  R9
//@INPUT:  ===================
//@INPUT:  测试取值:{w-100，h-100，w+100，h+100}（右下角越界）
//@INPUT:  替代符:  R10
//@INPUT:  ===================
//@INPUT:  测试取值:{w-100,h/2-100,w+100,h/2+100}（右中边越界）
//@INPUT:  替代符:  R11
//@INPUT:  ===================
//@INPUT:  测试取值:{w-100,0,w+100,100}（右上角越界）
//@INPUT:  替代符:  R12
//@INPUT:  ===================
//@INPUT:  测试取值:{1，1，16，16}（与r6结合测试伸缩15倍，区域不重叠的情况）
//@INPUT:  替代符:  R13
//@INPUT:  ===================
//@INPUT:  测试取值:{-11,-11,100,100}（左上角越界）
//@INPUT:  替代符:  R14
//@INPUT:  ===================
//@INPUT:  测试取值:{-11,h/2-100,100,h+100}（左中边越界）
//@INPUT:  替代符:  R15
//@INPUT:  ===================
//@INPUT:  测试取值:{w/2,-11,w/2+100,h+100}（上中边越界）
//@INPUT:  替代符:  R16
//@INPUT:  -------------------------------------------------------------------------------
//@INPUT:  标识符:  M
//@INPUT:  属  性:  混合模式 ）       
//@INPUT:  取值范围:17种CSUDIOSGOpMode_E模式
//@INPUT:
//@INPUT:  测试取值:
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
//@INPUT:  标识符:  X
//@INPUT:  属  性:  颜色模式       
//@INPUT:  取值范围:12种CSUDIOSGPixelFormat_E
//@INPUT:
//@INPUT:  测试取值:
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
//@INPUT:  可以根据覆盖率取其它颜色模式值
//@INPUT:
//@INPUT:*******************************************************************
//@INPUT:*************测试用例中透明度混合效果的计算公式********************
//@INPUT:*******************************************************************
//@INPUT: sColorA: 源平面颜色alpha值
//@INPUT: dColorA: 目的平面颜色alpha值
//@INPUT: sGA: 源平面的全局alpha值
//@INPUT: dGA: 目的平面的全局alpha值
//@INPUT: dA: 最终混合结果的alpha值
//@INPUT: dR: 最终混合结果的R分量值 
//@INPUT: dG: 最终混合结果的G分量值
//@INPUT: dB: 最终混合结果的B分量值
//@INPUT: sMixA = sColorA * sGA/0xff
//@INPUT: dMixA = dColorA * dGA/0xff
//@INPUT: Forum1: dA = sMixA + dMixA - ((sMixA * dMixA)/0xff)
//@INPUT: Forum2: dA = dGA
//@INPUT: Forum3: dA = sMixA + dColorA - ((sMixA * dColorA)/0xff)
//@INPUT: Forum4: dA = dColorA
//@INPUT: Forum5: dA = sColorA
//@INPUT: Forum6: dA = sColorA + dColorA * Global_Alpha (1- sColorA)
//@INPUT:（1）dA = 由以上公式(ForumX)得到
//@INPUT:（2）dR = dR + (sR - dR) * sMixA /dA
//@INPUT:（3）dG = dG + (sG - dG) * sMixA /dA
//@INPUT:（4）dB = dB + (sB - dB) * sMixA /dA
//@INPUT:
//@EXPECTATION: 提供以下测试用例用到的各种替代符
//@REMARK:  <b>根据以上各个参数可以组合出执行Blit操作的各种测试
//@REMARK:  假如输入组合为 <font color= blue>P1_1+P2_1+X1+SR1+DR1+M1</font>
//@REMARK:  表示测试数据：
//@REMARK:	1、源绘图平面大小P1_1=[720,576]
//@REMARK:	2、目的绘图平面大小P2_1=[720,576]
//@REMARK:	3、当前颜色模式X1=EM_UDIOSG_PIXEL_FORMAT_ARGB_1555
//@REMARK:	4、源矩形区域SR1=CSUDI_NULL
//@REMARK:  5、目的矩形区域DR1=CSUDI_NULL
//@REMARK:  6、混合模式M1=EM_UDIOSG_OPMODE_COPY</b>
//@EXECUTIONFLOW: 
CSUDI_BOOL CSTC_OSG_IT_Blit_0000(void)
{   
	return CSUDI_TRUE;
}

//**********************************************************************************//
//******************************Begin Test  Rect Blit******************************//
//**********************************************************************************//
//************************目标绘图空间为缓存空间************************************//

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0001
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源和目的矩形区域都为CSUDI_NULL时的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X1和X2
//@INPUT: P1_1+P2_1+X1+SR1+DR1+M1
//@EXPECTATION: 成功执行blit，返回CSUDI_SUCCESS
//@REMARK: 测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式   
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1和P2_1，颜色模式为X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M1）
//@EXECUTIONFLOW:4、调用CSUDISync完成blit操作
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取P2_1绘图平面信息P2_1_Info
//@EXECUTIONFLOW:6、期望目的绘图平面内的任意点颜色值为OSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
//@EXECUTIONFLOW:8、创建绘图平面的颜色模式为X2,重复以上步骤
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0001(void)
{  
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_1,EM_RECT_1,CSUDI_TRUE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_1,EM_RECT_1,CSUDI_TRUE);
	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0002
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为CSUDI_NULL，目的矩形区域为全屏大小时的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X1
//@INPUT: P1_1+P2_1+X1+SR1+DR2+M1
//@EXPECTATION: 成功执行blit，返回CSUDI_SUCCESS
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1和P2_1，颜色模式为X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit（P2_1,DR2,P1_1,SR1,M1）
//@EXECUTIONFLOW:4、调用CSUDISync完成blit操作
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取P2_1绘图平面信息P2_1_Info
//@EXECUTIONFLOW:6、期望目的绘图平面内的任意点颜色值为OSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
//@EXECUTIONFLOW:8、创建绘图平面的颜色模式为X2,重复以上步骤
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0002(void)
{   	
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_1,EM_RECT_2,CSUDI_TRUE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_1,EM_RECT_2,CSUDI_TRUE);
	return nRet;
}

//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为CSUDI_NULL，目的矩形区域大小为非法值（0，0）时的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X1
//@INPUT: P1_1+P2_1+X1+SR1+DR4+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1和P2_1，颜色模式为X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit（P2_1,DR4,P1_1,SR1,M1），返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4、调用CSUDIOSGDestroySurface销毁绘图平面
//@EXECUTIONFLOW:5、创建绘图平面的颜色模式为X2,重复以上步骤
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0003(void)
{   
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_1,EM_RECT_4,CSUDI_FALSE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_1,EM_RECT_4,CSUDI_FALSE);
	return nRet;
}

//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为CSUDI_NULL，目的矩形区域大小为非法值（-1，-1）时的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X1
//@INPUT: P1_1+P2_1+X1+SR1+DR5+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1和P2_1，颜色模式为X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit（P2_1,DR5,P1_1,SR1,M1），返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4、调用CSUDIOSGDestroySurface销毁绘图平面
//@EXECUTIONFLOW:5、创建绘图平面的颜色模式为X2,重复以上步骤
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0004(void)
{   
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_1,EM_RECT_5,CSUDI_FALSE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_1,EM_RECT_5,CSUDI_FALSE);
	return nRet;
}

//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为全屏大小，目的矩形区域CSUDI_NULL时的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X1
//@INPUT: P1_1+P2_1+X1+SR2+DR1+M1
//@EXPECTATION: 执行blit成功，返回CSUDI_SUCCESS
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1和P2_1，颜色模式为X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR2,M1）
//@EXECUTIONFLOW:4、调用CSUDISync完成blit操作
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取P2_1绘图平面信息P2_1_Info
//@EXECUTIONFLOW:6、期望目的绘图平面内的任意点颜色值为OSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
//@EXECUTIONFLOW:8、创建绘图平面的颜色模式为X2,重复以上步骤
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0005(void)
{   
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_2,EM_RECT_1,CSUDI_TRUE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_2,EM_RECT_1,CSUDI_TRUE);
	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0006
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为全屏大小，目的矩形区域为全屏大小时的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X1
//@INPUT: P1_1+P2_1+X1+SR2+DR2+M1
//@EXPECTATION: 成功执行blit，返回CSUDI_SUCCESS
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1和P2_1，颜色模式为X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit（P2_1,DR2,P1_1,SR2,M1）
//@EXECUTIONFLOW:4、调用CSUDISync完成blit操作
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取P2_1绘图平面信息P2_1_Info
//@EXECUTIONFLOW:6、期望绘图平面内的任意点颜色值为OSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
//@EXECUTIONFLOW:8、创建绘图平面的颜色模式为X2,重复以上步骤
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0006(void)
{   
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_2,EM_RECT_2,CSUDI_TRUE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_2,EM_RECT_2,CSUDI_TRUE);
	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0007
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为全屏大小，目的矩形区域为非法值（-1，-1）时的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X1
//@INPUT: P1_1+P2_1+X1+SR2+DR5+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1和P2_1，颜色模式为X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit（P2_1,DR5,P1_1,SR2,M1），返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4、调用CSUDIOSGDestroySurface销毁绘图平面
//@EXECUTIONFLOW:5、创建绘图平面的颜色模式为X2,重复以上步骤
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0007(void)
{   
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_2,EM_RECT_5,CSUDI_FALSE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_2,EM_RECT_5,CSUDI_FALSE);
	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0008
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源和目的矩形区域都为屏幕中央位置（200*200）时的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X1
//@INPUT: P1_1+P2_1+X1+SR3+DR3+M1
//@EXPECTATION: 成功执行blit，返回CSUDI_SUCCESS
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式   
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1和P2_1，颜色模式为X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit（P2_1,DR3,P1_1,SR3,M1），返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDISync完成blit操作
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取P2_1绘图平面信息P2_1_Info
//@EXECUTIONFLOW:6、期望目的绘图平面DR3区域内的任意点颜色值为OSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
//@EXECUTIONFLOW:8、创建绘图平面的颜色模式为X2,重复以上步骤
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0008(void)
{   
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_3,EM_RECT_3,CSUDI_TRUE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_3,EM_RECT_3,CSUDI_TRUE);
	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0009
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为全屏大小，目的矩形区域在屏幕左下角越界的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X1
//@INPUT: P1_1+P2_1+X1+SR3+DR8+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1和P2_1，颜色模式为X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit（P2_1,DR8,P1_1,SR3,M1），返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4、调用CSUDIOSGDestroySurface销毁绘图平面
//@EXECUTIONFLOW:5、创建绘图平面的颜色模式为X2,重复以上步骤
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0009(void)
{   
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_3,EM_RECT_8,CSUDI_FALSE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_3,EM_RECT_8,CSUDI_FALSE);
	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0010
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为全屏大小，目的矩形区域在屏幕底中边越界的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X1
//@INPUT: P1_1+P2_1+X1+SR3+DR9+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1和P2_1，颜色模式为X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit（P2_1,DR9,P1_1,SR3,M1），返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4、调用CSUDIOSGDestroySurface销毁绘图平面
//@EXECUTIONFLOW:5、创建绘图平面的颜色模式为X2,重复以上步骤
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0010(void)
{   
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_3,EM_RECT_9,CSUDI_FALSE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_3,EM_RECT_9,CSUDI_FALSE);
	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0011
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为全屏大小，目的矩形区域在屏幕右下角越界的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X1
//@INPUT: P1_1+P2_1+X1+SR3+DR10+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1和P2_1，颜色模式为X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit（P2_1,DR10,P1_1,SR3,M1），返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4、调用CSUDIOSGDestroySurface销毁绘图平面
//@EXECUTIONFLOW:5、创建绘图平面的颜色模式为X2,重复以上步骤
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0011(void)
{   
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_3,EM_RECT_10,CSUDI_FALSE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_3,EM_RECT_10,CSUDI_FALSE);
	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0012
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为全屏大小，目的矩形区域在屏幕右中边越界的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X1
//@INPUT: P1_1+P2_1+X1+SR3+DR11+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1和P2_1，颜色模式为X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit（P2_1,DR11,P1_1,SR3,M1），返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4、调用CSUDIOSGDestroySurface销毁绘图平面
//@EXECUTIONFLOW:5、创建绘图平面的颜色模式为X2,重复以上步骤
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0012(void)
{   	
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_3,EM_RECT_11,CSUDI_FALSE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_3,EM_RECT_11,CSUDI_FALSE);
	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0013
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为全屏大小，目的矩形区域在屏幕右上角越界的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X1
//@INPUT: P1_1+P2_1+X1+SR3+DR12+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式   
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1和P2_1，颜色模式为X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit（P2_1,DR12,P1_1,SR3,M1），返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4、调用CSUDIOSGDestroySurface销毁绘图平面
//@EXECUTIONFLOW:5、创建绘图平面的颜色模式为X2,重复以上步骤
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0013(void)
{   		
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_3,EM_RECT_12,CSUDI_FALSE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_3,EM_RECT_12,CSUDI_FALSE);
	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0014
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源和目的矩形区域都为非法值（0，0）时的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X1
//@INPUT: P1_1+P2_1+X1+SR4+DR4+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1和P2_1，颜色模式为X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit（P2_1,DR4,P1_1,SR4,M1），返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4、调用CSUDIOSGDestroySurface销毁绘图平面
//@EXECUTIONFLOW:5、创建绘图平面的颜色模式为X2,重复以上步骤
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0014(void)
{   
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_4,EM_RECT_4,CSUDI_FALSE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_4,EM_RECT_4,CSUDI_FALSE);
	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0015
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为非法值（0，0），目的矩形区域为一个像素（1，1）时的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X1
//@INPUT: P1_1+P2_1+X1+SR4+DR6+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1和P2_1，颜色模式为X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit（P2_1,DR6,P1_1,SR4,M1）
//@EXECUTIONFLOW:4、调用CSUDISync完成blit操作
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取P2_1绘图平面信息P2_1_Info
//@EXECUTIONFLOW:6、期望目的绘图平面DR6区域内的任意点颜色值为OSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
//@EXECUTIONFLOW:8、创建绘图平面的颜色模式为X2,重复以上步骤
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0015(void)
{   	
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_4,EM_RECT_6,CSUDI_FALSE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_4,EM_RECT_6,CSUDI_FALSE);
	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0016
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域都为非法值（-1，-1）时的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X1
//@INPUT: P1_1+P2_1+X1+SR5+DR5+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1和P2_1，颜色模式为X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit（P2_1,SR5,P1_1,SR5,M1），返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4、调用CSUDIOSGDestroySurface销毁绘图平面
//@EXECUTIONFLOW:5、创建绘图平面的颜色模式为X2,重复以上步骤
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0016(void)
{   
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_5,EM_RECT_5,CSUDI_FALSE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_5,EM_RECT_5,CSUDI_FALSE);
	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0017
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域都为一个像素（1，1）时的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X1
//@INPUT: P1_1+P2_1+X1+SR6+DR6+M1
//@EXPECTATION: 成功执行blit，返回CSUDI_SUCCESS
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1和P2_1，颜色模式为X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit（P2_1,SR6,P1_1,DR6,M1）
//@EXECUTIONFLOW:4、调用CSUDISync完成blit操作
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取P2_1绘图平面信息P2_1_Info
//@EXECUTIONFLOW:6、期望目的绘图平面DR6区域内的任意点颜色值为OSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
//@EXECUTIONFLOW:8、创建绘图平面的颜色模式为X2,重复以上步骤
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0017(void)
{     
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_6,EM_RECT_6,CSUDI_TRUE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_6,EM_RECT_6,CSUDI_TRUE);
	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0018
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为一个像素（1，1），目的矩形区域为非法值（-1，-1）时的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X1
//@INPUT: P1_1+P2_1+X1+SR6+DR5+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1和P2_1，颜色模式为X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit（P2_1,SR6,P1_1,DR5,M1），返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4、调用CSUDIOSGDestroySurface销毁绘图平面
//@EXECUTIONFLOW:5、创建绘图平面的颜色模式为X2,重复以上步骤
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0018(void)
{   
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_6,EM_RECT_5,CSUDI_FALSE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_6,EM_RECT_5,CSUDI_FALSE);
	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0019
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源放大15倍数的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X1
//@INPUT: P1_1+P2_1+X1+SR6+DR7+M1
//@EXPECTATION: 成功执行blit，返回CSUDI_SUCCESS
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1和P2_1，颜色模式为X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit（P2_1,SR6,P1_1,DR7,M1）
//@EXECUTIONFLOW:4、调用CSUDISync完成blit操作
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取P2_1绘图平面信息P2_1_Info
//@EXECUTIONFLOW:6、期望目的绘图平面DR7区域内的任意点颜色值为OSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
//@EXECUTIONFLOW:8、创建绘图平面的颜色模式为X2,重复以上步骤
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0019(void)
{   	
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_6,EM_RECT_7,CSUDI_TRUE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_6,EM_RECT_7,CSUDI_TRUE);
	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0020
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源缩小15倍数的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X1
//@INPUT: P1_1+P2_1+X1+SR7+DR6+M1
//@EXPECTATION: 成功执行blit，返回CSUDI_SUCCESS
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1和P2_1，颜色模式为X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit（P2_1,SR7,P1_1,DR6,M1）
//@EXECUTIONFLOW:4、调用CSUDISync完成blit操作
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取P2_1绘图平面信息P2_1_Info
//@EXECUTIONFLOW:6、期望目的绘图平面DR6区域内的任意点颜色值为OSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
//@EXECUTIONFLOW:8、创建绘图平面的颜色模式为X2,重复以上步骤
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0020(void)
{   
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_7,EM_RECT_6,CSUDI_TRUE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_7,EM_RECT_6,CSUDI_TRUE);
	return nRet;
}

//***************************目标绘图空间为显存空间************************//

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0021
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源和目的矩形区域都为CSUDI_NULL时的blit操作
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: P1_1+D+SR1+DR1+M1
//@EXPECTATION: 成功执行blit，返回CSUDI_SUCCESS
//@REMARK: 测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式   
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄D，期望成功
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取显存空间D的信息，大小和颜色模式
//@EXECUTIONFLOW:3、根据步骤(2)获取的信息调用CSUDIOSGCreateSurface创建缓存空间P1_1
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit（D,DR1,P1_1,SR1,M1）
//@EXECUTIONFLOW:6、调用CSUDISync完成blit操作
//@EXECUTIONFLOW:7、调用CSUDIOSGGetSurfaceInfo获取D显存空间的信息
//@EXECUTIONFLOW:8、期望显存空间内的任意点颜色值为OSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:9、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0021(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_1,EM_RECT_1,CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0022
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为CSUDI_NULL，目的矩形区域为全屏大小时的blit操作
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: P1_1+D+SR1+DR2+M1
//@EXPECTATION: 成功执行blit，返回CSUDI_SUCCESS
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式 
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄D，期望成功
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取显存空间D的信息，大小和颜色模式
//@EXECUTIONFLOW:3、根据步骤(2)获取的信息调用CSUDIOSGCreateSurface创建缓存空间P1_1
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit（D,DR2,P1_1,SR1,M1）
//@EXECUTIONFLOW:6、调用CSUDISync完成blit操作
//@EXECUTIONFLOW:7、调用CSUDIOSGGetSurfaceInfo获取D显存空间的信息
//@EXECUTIONFLOW:8、期望显存空间内的任意点颜色值为OSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:9、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0022(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_1,EM_RECT_2,CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0023
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为CSUDI_NULL，目的矩形区域大小为非法值（0，0）时的blit操作
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: P1_1+D+SR1+DR4+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄D，期望成功
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取显存空间D的信息，大小和颜色模式
//@EXECUTIONFLOW:3、根据步骤(2)获取的信息调用CSUDIOSGCreateSurface创建缓存空间P1_1
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit（D,DR4,P1_1,SR1,M1），返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0023(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_1,EM_RECT_4,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0024
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为CSUDI_NULL，目的矩形区域大小为非法值（-1，-1）时的blit操作
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: P1_1+D+SR1+DR5+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄D，期望成功
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取显存空间D的信息，大小和颜色模式
//@EXECUTIONFLOW:3、根据步骤(2)获取的信息调用CSUDIOSGCreateSurface创建缓存空间P1_1
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit（D,DR5,P1_1,SR1,M1），返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0024(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_1,EM_RECT_5,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0025
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为全屏大小，目的矩形区域CSUDI_NULL时的blit操作
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: P1_1+D+SR2+DR1+M1
//@EXPECTATION: 成功执行blit，返回CSUDI_SUCCESS
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄D，期望成功
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取显存空间D的信息，大小和颜色模式
//@EXECUTIONFLOW:3、根据步骤(2)获取的信息调用CSUDIOSGCreateSurface创建缓存空间P1_1
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit（D,DR1,P1_1,SR2,M1）
//@EXECUTIONFLOW:6、调用CSUDISync完成blit操作
//@EXECUTIONFLOW:7、调用CSUDIOSGGetSurfaceInfo获取D显存空间的信息
//@EXECUTIONFLOW:8、期望显存空间内的任意点颜色值为OSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:4、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0025(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_2,EM_RECT_1,CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0026
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为全屏大小，目的矩形区域为全屏大小时的blit操作
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: P1_1+D+SR2+DR2+M1
//@EXPECTATION: 成功执行blit，返回CSUDI_SUCCESS
//@REMARK: 测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式   
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄D，期望成功
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取显存空间D的信息，大小和颜色模式
//@EXECUTIONFLOW:3、根据步骤(2)获取的信息调用CSUDIOSGCreateSurface创建缓存空间P1_1
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit（D,DR2,P1_1,SR2,M1）
//@EXECUTIONFLOW:6、调用CSUDISync完成blit操作
//@EXECUTIONFLOW:7、调用CSUDIOSGGetSurfaceInfo获取D显存空间的信息
//@EXECUTIONFLOW:8、期望显存空间内的任意点颜色值为OSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:4、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0026(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_2,EM_RECT_2,CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0027
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为全屏大小，目的矩形区域为屏幕中央位置（200*200）时的blit操作
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: P1_1+D+SR2+DR3+M1
//@EXPECTATION: 成功执行blit，返回CSUDI_SUCCESS
//@REMARK: 测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式   
//@REMARK: 此用例暂无效
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄D，期望成功
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取显存空间D的信息，大小和颜色模式
//@EXECUTIONFLOW:3、根据步骤(2)获取的信息调用CSUDIOSGCreateSurface创建缓存空间P1_1
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit（D,DR3,P1_1,SR2,M1）
//@EXECUTIONFLOW:6、调用CSUDISync完成blit操作
//@EXECUTIONFLOW:7、调用CSUDIOSGGetSurfaceInfo获取D显存空间的信息
//@EXECUTIONFLOW:8、期望显存空间内的任意点颜色值为OSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:9、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0027(void)
{   		
	CSTCPrint("Not realized yet !!!\n");
	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0028
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为全屏大小，目的矩形区域为非法值（-1，-1）时的blit操作
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: P1_1+D+SR2+DR5+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄D，期望成功
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取显存空间D的信息，大小和颜色模式
//@EXECUTIONFLOW:3、根据步骤(2)获取的信息调用CSUDIOSGCreateSurface创建缓存空间P1_1
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit（D,DR5,P1_1,SR2,M1），返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0028(void)
{   	
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_2,EM_RECT_5,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0029
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源和矩形区域都为屏幕中央位置（200*200）时的blit操作
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: P1_1+D+SR3+DR3+M1
//@EXPECTATION: 成功执行blit，返回CSUDI_SUCCESS
//@REMARK: 测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式   
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄D，期望成功
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取显存空间D的信息，大小和颜色模式
//@EXECUTIONFLOW:3、根据步骤(2)获取的信息调用CSUDIOSGCreateSurface创建缓存空间P1_1
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit（D,DR3,P1_1,SR3,M1）
//@EXECUTIONFLOW:6、调用CSUDISync完成blit操作
//@EXECUTIONFLOW:7、调用CSUDIOSGGetSurfaceInfo获取D显存空间的信息
//@EXECUTIONFLOW:8、期望显存空间内的任意点颜色值为OSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:9、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0029(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_3,EM_RECT_3,CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0030
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为全屏大小，目的矩形区域在屏幕左下角越界的blit操作
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: P1_1+D+SR2+DR8+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄D，期望成功
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取显存空间D的信息，大小和颜色模式
//@EXECUTIONFLOW:3、根据步骤(2)获取的信息调用CSUDIOSGCreateSurface创建缓存空间P1_1
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit（D,DR8,P1_1,SR2,M1），返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0030(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_2,EM_RECT_8,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0031
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为全屏大小，目的矩形区域在屏幕底中边越界的blit操作
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: P1_1+D+SR2+DR9+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄D，期望成功
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取显存空间D的信息，大小和颜色模式
//@EXECUTIONFLOW:3、根据步骤(2)获取的信息调用CSUDIOSGCreateSurface创建缓存空间P1_1
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit（D,DR9,P1_1,SR2,M1），返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0031(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_2,EM_RECT_9,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0032
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为全屏大小，目的矩形区域在屏幕右下角越界的blit操作
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: P1_1+D+SR2+DR10+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄D，期望成功
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取显存空间D的信息，大小和颜色模式
//@EXECUTIONFLOW:3、根据步骤(2)获取的信息调用CSUDIOSGCreateSurface创建缓存空间P1_1
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit（D,DR10,P1_1,SR2,M1），返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0032(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_2,EM_RECT_10,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0033
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为全屏大小，目的矩形区域在屏幕右中边越界的blit操作
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: P1_1+D+SR2+DR11+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄D，期望成功
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取显存空间D的信息，大小和颜色模式
//@EXECUTIONFLOW:3、根据步骤(2)获取的信息调用CSUDIOSGCreateSurface创建缓存空间P1_1
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit（D,DR11,P1_1,SR2,M1），返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0033(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_2,EM_RECT_11,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0034
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为全屏大小，目的矩形区域在屏幕右上角越界的blit操作
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: P1_1+D+SR2+DR12+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄D，期望成功
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取显存空间D的信息，大小和颜色模式
//@EXECUTIONFLOW:3、根据步骤(2)获取的信息调用CSUDIOSGCreateSurface创建缓存空间P1_1
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit（D,DR12,P1_1,SR2,M1），返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0034(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_2,EM_RECT_12,CSUDI_FALSE);	
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0035
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域都为非法值（0，0）时的blit操作
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: P1_1+D+SR4+DR4+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄D，期望成功
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取显存空间D的信息，大小和颜色模式
//@EXECUTIONFLOW:3、根据步骤(2)获取的信息调用CSUDIOSGCreateSurface创建缓存空间P1_1
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit（D,DR4,P1_1,SR4,M1），返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0035(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_4,EM_RECT_4,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0036
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为非法值（0，0），目的矩形区域为一个像素（1，1）时的blit操作
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: P1_1+D+SR4+DR6+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄D，期望成功
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取显存空间D的信息，大小和颜色模式
//@EXECUTIONFLOW:3、根据步骤(2)获取的信息调用CSUDIOSGCreateSurface创建缓存空间P1_1
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit（D,DR6,P1_1,SR4,M1），返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0036(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_4,EM_RECT_6,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0037
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域都为非法值（-1，-1）时的blit操作
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: P1_1+D+SR5+DR5+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄D，期望成功
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取显存空间D的信息，大小和颜色模式
//@EXECUTIONFLOW:3、根据步骤(2)获取的信息调用CSUDIOSGCreateSurface创建缓存空间P1_1
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit（D,DR5,P1_1,SR5,M1），返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0037(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_5,EM_RECT_5,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0038
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源和目的矩形区域都为一个像素（1，1）时的blit操作
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: P1_1+D+SR6+DR6+M1
//@EXPECTATION: 成功执行blit，返回CSUDI_SUCCESS
//@REMARK: 测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式   
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄D，期望成功
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取显存空间D的信息，大小和颜色模式
//@EXECUTIONFLOW:3、根据步骤(2)获取的信息调用CSUDIOSGCreateSurface创建缓存空间P1_1
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit（D,DR6,P1_1,SR6,M1）
//@EXECUTIONFLOW:6、调用CSUDISync完成blit操作
//@EXECUTIONFLOW:7、调用CSUDIOSGGetSurfaceInfo获取D显存空间的信息
//@EXECUTIONFLOW:8、期望显存空间内的任意点颜色值为OSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:9、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0038(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_6,EM_RECT_6,CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0039
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为一个像素（1，1），目的矩形区域为非法值（-1，-1）时的blit操作
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: P1_1+D+SR6+DR5+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄D，期望成功
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取显存空间D的信息，大小和颜色模式
//@EXECUTIONFLOW:3、根据步骤(2)获取的信息调用CSUDIOSGCreateSurface创建缓存空间P1_1
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit（D,DR5,P1_1,SR6,M1），返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0039(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_6,EM_RECT_5,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0040
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源放大15倍数的blit操作
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: P1_1+D+SR7+DR6+M1
//@EXPECTATION: 成功执行blit，返回CSUDI_SUCCESS
//@REMARK: 测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式   
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄D，期望成功
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取显存空间D的信息，大小和颜色模式
//@EXECUTIONFLOW:3、根据步骤(2)获取的信息调用CSUDIOSGCreateSurface创建缓存空间P1_1
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit（D,DR7,P1_1,SR6,M1）
//@EXECUTIONFLOW:6、调用CSUDISync完成blit操作
//@EXECUTIONFLOW:7、调用CSUDIOSGGetSurfaceInfo获取D显存空间的信息
//@EXECUTIONFLOW:8、期望显存空间内的任意点颜色值为OSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:9、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0040(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_7,EM_RECT_6,CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0041
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源缩小15倍数的blit操作
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: P1_1+D+SR6+DR7+M1
//@EXPECTATION: 成功执行blit，返回CSUDI_SUCCESS
//@REMARK: 测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式   
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄D，期望成功
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取显存空间D的信息，大小和颜色模式
//@EXECUTIONFLOW:3、根据步骤(2)获取的信息调用CSUDIOSGCreateSurface创建缓存空间P1_1
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit（D,DR6,P1_1,SR7,M1）
//@EXECUTIONFLOW:6、调用CSUDISync完成blit操作
//@EXECUTIONFLOW:7、调用CSUDIOSGGetSurfaceInfo获取D显存空间的信息
//@EXECUTIONFLOW:8、期望显存空间内的任意点颜色值为OSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:9、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0041(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_6,EM_RECT_7,CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0042
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为全屏大小，目的矩形区域在屏幕左上角越界的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X1
//@INPUT: P1_1+P2_1+X1+SR2+DR14+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式   
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit（P2_1,DR14,P1_1,SR2,M1），返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0042(void)
{   	
	return OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_2,EM_RECT_14,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0043
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为全屏大小，目的矩形区域在屏幕左中边越界的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X1
//@INPUT: P1_1+P2_1+X1+SR2+DR15+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式   
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1，颜色模式为X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit（P2_1,DR15,P1_1,SR2,M1），返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0043(void)
{   		
	return OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_2,EM_RECT_15,CSUDI_FALSE);	
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0044
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为全屏大小，目的矩形区域在屏幕上中边越界的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X1
//@INPUT: P1_1+P2_1+X1+SR2+DR16+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式   
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1，颜色模式为X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit（P2_1,DR16,P1_1,SR2,M1），返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0044(void)
{   		
	return OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_2,EM_RECT_16,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0045
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为全屏大小，目的矩形区域在屏幕左上角越界的blit操作(目的为显存空间)
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: P1_1+D+SR2+DR14+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄D，期望成功
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取显存空间D的信息，大小和颜色模式
//@EXECUTIONFLOW:3、根据步骤(2)获取的信息调用CSUDIOSGCreateSurface创建缓存空间P1_1
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit（D,DR14,P1_1,SR2,M1），返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0045(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_2,EM_RECT_14,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0046
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为全屏大小，目的矩形区域在屏幕左中边越界的blit操作(目的为显存空间)
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: P1_1+D+SR2+DR15+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄D，期望成功
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取显存空间D的信息，大小和颜色模式
//@EXECUTIONFLOW:3、根据步骤(2)获取的信息调用CSUDIOSGCreateSurface创建缓存空间P1_1
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit（D,DR15,P1_1,SR2,M1），返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0046(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_2,EM_RECT_15,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0047
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源矩形区域为全屏大小，目的矩形区域在屏幕上中边越界的blit操作(目的为显存空间)
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: P1_1+D+SR2+DR16+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄D，期望成功
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取显存空间D的信息，大小和颜色模式
//@EXECUTIONFLOW:3、根据步骤(2)获取的信息调用CSUDIOSGCreateSurface创建缓存空间P1_1
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit（D,DR16,P1_1,SR2,M1），返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0047(void)
{   
	return OSG_Blit_TestRect_WithDisplay(0x88,EM_RECT_2,EM_RECT_16,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0048
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试缩小的blit操作(即目标区域大小比源区域要小)
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为当前支持的颜色模式X2或X1
//@EXPECTATION: 成功执行blit，返回CSUDI_SUCCESS
//@REMARK: 测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式   
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间SurfaceSrc和SurfaceDst，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit,将SurfaceSrc的内容blit到SurfaceDst上，源区域为(x,y,w,h):(0, 0, 720, 576),目标区域为(x,y,w,h):(180,144,360,288)
//@EXECUTIONFLOW:4、调用CSUDISync完成blit操作
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取SurfaceDst绘图平面信息P2_1_Info
//@EXECUTIONFLOW:6、期望目的绘图平面内的(x,y,w,h):(180,144,360,288)点颜色值为0x88
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0048(void)
{   
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	
	//寻找一个支持的ePixelFormat
	CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
					"没有找到任何支持的满足要求的PixelFormat"
				);
		
	bRet = OSG_Blit_TestRect(ePixelFormat,0x88,EM_RECT_1,EM_RECT_17,CSUDI_TRUE);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == bRet),"blit 收缩测试失败!");
	
	CSTK_FATAL_POINT;
	
	return bRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0049
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试缩小的blit操作(即目标区域大小比源区域要大)
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为当前支持的颜色模式X2或X1
//@EXPECTATION: 成功执行blit，返回CSUDI_SUCCESS
//@REMARK: 测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式   
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间SurfaceSrc和SurfaceDst，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit,将SurfaceSrc的内容blit到SurfaceDst上，源区域为(x,y,w,h):(180,144,360,288),目标区域为(x,y,w,h):(0, 0, 720, 576)
//@EXECUTIONFLOW:4、调用CSUDISync完成blit操作
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取SurfaceDst绘图平面信息P2_1_Info
//@EXECUTIONFLOW:6、期望目的绘图平面内的(x,y,w,h):(0, 0, 720, 576)点颜色值为0x88
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0049(void)
{   
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	
	//寻找一个支持的ePixelFormat
	CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
					"没有找到任何支持的满足要求的PixelFormat"
				);
		
	bRet = OSG_Blit_TestRect(ePixelFormat,0x88,EM_RECT_17,EM_RECT_1,CSUDI_TRUE);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == bRet),"blit 放大测试失败!");
	
	CSTK_FATAL_POINT;
	
	return bRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0050
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源和目的矩形区域大小相同，但偏移不同的对等的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为当前支持的颜色模式X2或X1
//@EXPECTATION: 成功执行blit，返回CSUDI_SUCCESS
//@REMARK: 测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式   
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间SurfaceSrc和SurfaceDst，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit,将SurfaceSrc的内容blit到SurfaceDst上，源区域为(x,y,w,h):(180,144,360,288),目标区域为(x,y,w,h):(190, 154, 360, 288)
//@EXECUTIONFLOW:4、调用CSUDISync完成blit操作
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取SurfaceDst绘图平面信息P2_1_Info
//@EXECUTIONFLOW:6、期望目的绘图平面内的(x,y,w,h):(190, 154, 360, 288)点颜色值为0x88
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0050(void)
{   
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	
	//寻找一个支持的ePixelFormat
	CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
					"没有找到任何支持的满足要求的PixelFormat"
				);
	
	bRet = OSG_Blit_TestRect(ePixelFormat,0x88,EM_RECT_17,EM_RECT_18,CSUDI_TRUE);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == bRet),"blit 不对等区域拷贝失败!");
	
	CSTK_FATAL_POINT;
	
	return bRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0051
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源和目的矩形区域大小相同，但偏移不同的对等的blit操作,但交换区域高度为奇数
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为当前支持的颜色模式X2或X1
//@EXPECTATION: 成功执行blit，返回CSUDI_SUCCESS
//@REMARK: 测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式   
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间SurfaceSrc和SurfaceDst，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit,将SurfaceSrc的内容blit到SurfaceDst上，源区域为(x,y,w,h):(180,144,361,289),目标区域为(x,y,w,h):(190, 154, 361, 289)
//@EXECUTIONFLOW:4、调用CSUDISync完成blit操作
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取SurfaceDst绘图平面信息P2_1_Info
//@EXECUTIONFLOW:6、期望目的绘图平面内的(x,y,w,h):(190, 154, 361, 289)点颜色值为OSG_FILL_COLOR_GREY_32BIT
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0051(void)
{   
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	
	//寻找一个支持的ePixelFormat
	CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
					"没有找到任何支持的满足要求的PixelFormat"
				);
		
	bRet = OSG_Blit_TestRect(ePixelFormat,0x88,EM_RECT_19,EM_RECT_23,CSUDI_TRUE);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == bRet),"blit 不对等区域拷贝，且高宽为奇数失败!");

	CSTK_FATAL_POINT;
	
	return bRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0052
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源和目的矩形区域大小相同(4000*100)，但偏移不同的对等的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: 
//@EXPECTATION: 成功执行blit，返回CSUDI_SUCCESS
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式   
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间sSurfaceSrc和sSurfaceDst，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit,将SurfaceSrc的内容blit到SurfaceDst上，源区域为(x,y,w,h):(10,10,3000,50),目标区域为(x,y,w,h):(20, 10, 3000, 50)
//@EXECUTIONFLOW:4、调用CSUDISync完成blit操作
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取SurfaceDst绘图平面信息P2_1_Info
//@EXECUTIONFLOW:6、期望目的绘图平面区域内的任意点颜色值为期望颜色OSG_FILL_COLOR_GREY_32BIT
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
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
					"没有找到任何支持的满足要求的PixelFormat"
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

	CSTK_ASSERT_TRUE_FATAL(((nRet == CSUDI_SUCCESS)||(nRet == CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED)),/*放大blit时可能不支持*/	"Step 4 Blit should be success !");
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
//@DESCRIPTION:测试源和目的矩形区域大小相同(100*4000)，但偏移不同的对等的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: 
//@EXPECTATION: 成功执行blit，返回CSUDI_SUCCESS
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式   
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间sSurfaceSrc和sSurfaceDst，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域为0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit,将SurfaceSrc的内容blit到SurfaceDst上，源区域为(x,y,w,h):(10,10,50,3000),目标区域为(x,y,w,h):(10, 20, 50, 3000)
//@EXECUTIONFLOW:4、调用CSUDISync完成blit操作
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取SurfaceDst绘图平面信息P2_1_Info
//@EXECUTIONFLOW:6、期望目的绘图平面区域内的任意点颜色值为期望颜色OSG_FILL_COLOR_GREY_32BIT
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
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

	//寻找一个支持的ePixelFormat
	CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
					"没有找到任何支持的满足要求的PixelFormat"
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

	CSTK_ASSERT_TRUE_FATAL(((nRet == CSUDI_SUCCESS)||(nRet == CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED)),/*放大blit时可能不支持*/	"Step 4 Blit should be success !");
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
//@DESCRIPTION:测试源放大大于15倍数的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: 1、P1_1+P2_1+X1+SR13+DR2+M1
//@INPUT: 1、P1_1+P2_1+X2+SR13+DR2+M1
//@EXPECTATION: 成功执行blit，返回CSUDI_SUCCESS
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1和P2_1，颜色模式为X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit（P2_1,SR13,P1_1,DR2,M1）
//@EXECUTIONFLOW:4、调用CSUDISync完成blit操作
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取P2_1绘图平面信息P2_1_Info
//@EXECUTIONFLOW:6、期望目的绘图平面DR2区域内的任意点颜色值为OSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
//@EXECUTIONFLOW:8、创建绘图平面的颜色模式为X2,重复以上步骤
CSUDI_BOOL CSTC_OSG_IT_Blit_Rect_0054(void)
{   	
	CSUDI_BOOL nRet = CSUDI_TRUE;
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,0x88,EM_RECT_13,EM_RECT_2,CSUDI_TRUE);
	nRet &= OSG_Blit_TestRect(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,0x88,EM_RECT_13,EM_RECT_2,CSUDI_TRUE);

	return nRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Rect_0055
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试源缩小大于15倍数的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: 1、P1_1+P2_1+X1+SR2+DR13+M1
//@INPUT: 2、P1_1+P2_1+X2+SR2+DR13+M1
//@EXPECTATION: 成功执行blit，返回CSUDI_SUCCESS
//@REMARK:  测试矩形区域大小时，并没有去关注颜色模式、colorkey、alpha、颜色值和混合模式    
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1和P2_1，颜色模式为X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit（P2_1,SR2,P1_1,DR13,M1）
//@EXECUTIONFLOW:4、调用CSUDISync完成blit操作
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取P2_1绘图平面信息P2_1_Info
//@EXECUTIONFLOW:6、期望目的绘图平面DR2区域内的任意点颜色值为OSG_FILL_COLOR_GREY_16BIT
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
//@EXECUTIONFLOW:8、创建绘图平面的颜色模式为X2,重复以上步骤
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
//@DESCRIPTION:测试源绘图平面ColorKey为C1时的blit操作,源绘图平面存在C1颜色的情况
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+C1+SR1+DR1+SCr1+DCr4+M1
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、对于SCr1颜色中的C1颜色值不进行blit处理
//@REMARK: 1、测试colorkey时需要有颜色数据值，并没有去关注矩形区域大小、颜色模式、alpha和混合模式   
//@REMARK: 2、只对X2或X1颜色模式进行测试，暂时不对其它颜色模式进行测试，可以根据测试覆盖率补充
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取源缓存空间P1_1的信息
//@EXECUTIONFLOW:3、调用CSUDIOSGSetSurfaceInfo将源缓存空间P1_1的ColorKey设置为C1
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域的颜色值为SCr1
//@EXECUTIONFLOW:5、初始化（填充）目标缓存空间区域的颜色值为DCr4
//@EXECUTIONFLOW:6、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M1）
//@EXECUTIONFLOW:7、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:8、从P2_1_SInfo信息中获取任意一个像素点的颜色值不存在SCr1颜色值
//@EXECUTIONFLOW:9、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试源绘图平面ColorKey为C1时的blit操作,目标绘图平面存在C1颜色的情况
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+C1+SR1+DR1+SCr1+DCr4+M1
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、对于SCr1颜色中的C1颜色值不进行blit处理
//@REMARK: 1、测试colorkey时需要有颜色数据值，并没有去关注矩形区域大小、颜色模式、alpha和混合模式   
//@REMARK: 2、只对X2或X1颜色模式进行测试，暂时不对其它颜色模式进行测试，可以根据测试覆盖率补充
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取源缓存空间P1_1的信息
//@EXECUTIONFLOW:3、调用CSUDIOSGSetSurfaceInfo将源缓存空间P1_1的ColorKey设置为C1
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域的颜色值为SCr4
//@EXECUTIONFLOW:5、初始化（填充）目标缓存空间区域的颜色值为DCr1
//@EXECUTIONFLOW:6、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M1）
//@EXECUTIONFLOW:7、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:8、从P2_1_SInfo信息中获取任意一个像素点的颜色值应该存在SCr4颜色值
//@EXECUTIONFLOW:9、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试源绘图平面ColorKey为C1时的blit操作,两个平面都不存在C1颜色值的情况
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2+C1+SR1+DR1+SCr3+DCr4+M1
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、对于SCr3颜色中的C1颜色值不进行blit处理
//@REMARK: 1、测试colorkey时需要有颜色数据值，并没有去关注矩形区域大小、颜色模式、alpha和混合模式   
//@REMARK: 2、只对X2或X1颜色模式进行测试，暂时不对其它颜色模式进行测试，可以根据测试覆盖率补充
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取源缓存空间P1_1的信息
//@EXECUTIONFLOW:3、调用CSUDIOSGSetSurfaceInfo将源缓存空间P1_1的ColorKey设置为C1
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域的颜色值为SCr2
//@EXECUTIONFLOW:5、初始化（填充）目标缓存空间区域的颜色值为DCr4
//@EXECUTIONFLOW:6、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M1），期望colorkey不对SCr3起作用
//@EXECUTIONFLOW:7、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:8、从P2_1_SInfo信息中获取任意一个像素点的颜色值应该存在SCr2颜色值
//@EXECUTIONFLOW:9、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试源绘图平面ColorKey为C2时的blit操作,两个平面都不存在C2颜色值的情况
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2+C2+SR1+DR1+SCr1+DCr4+M1
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、对于SCr1颜色中的C2颜色值不进行blit处理
//@REMARK: 1、测试colorkey时需要有颜色数据值，并没有去关注矩形区域大小、颜色模式、alpha和混合模式   
//@REMARK: 2、只对X2或X1颜色模式进行测试，暂时不对其它颜色模式进行测试，可以根据测试覆盖率补充
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取源缓存空间P1_1的信息
//@EXECUTIONFLOW:3、调用CSUDIOSGSetSurfaceInfo将源缓存空间P1_1的ColorKey设置为C2
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域的颜色值为SCr1
//@EXECUTIONFLOW:5、初始化（填充）目标缓存空间区域的颜色值为DCr4
//@EXECUTIONFLOW:6、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M1）
//@EXECUTIONFLOW:7、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:8、从P2_1_SInfo信息中获取任意一个像素点的颜色值应该存在SCr1颜色值
//@EXECUTIONFLOW:9、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试目的绘图平面ColorKey为C1时的blit操作,源绘图平面存在C1颜色值的情况
//@PRECONDITION: 1、平台UDI已经成功初始化
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、目标绘图平台存在SCr1颜色值
//@REMARK: 1、测试colorkey时需要有颜色数据值，并没有去关注矩形区域大小、颜色模式、alpha和混合模式   
//@REMARK: 2、暂时只对1555或者8888颜色模式测试
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间surface1、surface1，颜色模式为ARGB8888或ARGB1555
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间surface2的信息
//@EXECUTIONFLOW:3、调用CSUDIOSGSetSurfaceInfo将目的缓存空间的ColorKey设置为C1
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域的颜色值为SCr1
//@EXECUTIONFLOW:5、初始化（填充）目标缓存空间区域的颜色值为DCr4
//@EXECUTIONFLOW:6、调用CSUDIOSGBlit将蓝色往红色上拷贝
//@EXECUTIONFLOW:7、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间surface2的信息SInfo
//@EXECUTIONFLOW:8、从SInfo信息中获取任意一个像素点的颜色值存在SCr1颜色值
//@EXECUTIONFLOW:9、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试目的绘图平面ColorKey为红色时的blit操作,目标绘图平面存在C1颜色值的情况
//@PRECONDITION: 1、平台UDI已经成功初始化
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、目标绘图平台不存在C1颜色值
//@REMARK: 1、测试colorkey时需要有颜色数据值，并没有去关注矩形区域大小、颜色模式、alpha和混合模式   
//@REMARK: 2、暂时只对1555或者8888颜色模式测试
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间surface1、surface1，颜色模式为ARGB8888或ARGB1555
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间surface2的信息
//@EXECUTIONFLOW:3、调用CSUDIOSGSetSurfaceInfo将目的缓存空间的ColorKey设置为C1
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域的颜色值为SCr4
//@EXECUTIONFLOW:5、初始化（填充）目标缓存空间区域的颜色值为DCr1
//@EXECUTIONFLOW:6、调用CSUDIOSGBlit将蓝色往红色上拷贝
//@EXECUTIONFLOW:7、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间surface2的信息SInfo
//@EXECUTIONFLOW:8、从SInfo信息中获取任意一个像素点的颜色值不存在DCr1颜色值
//@EXECUTIONFLOW:9、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试目标绘图平面ColorKey为C1时的blit操作,两个平面都不存在C1颜色值的情况
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2+C1+SR1+DR1+SCr3+DCr4+M1
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、目标绘图平面存在SCr2颜色值
//@REMARK: 1、测试colorkey时需要有颜色数据值，并没有去关注矩形区域大小、颜色模式、alpha和混合模式   
//@REMARK: 2、只对X2或X1颜色模式进行测试，暂时不对其它颜色模式进行测试，可以根据测试覆盖率补充
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取源缓存空间P1_1的信息
//@EXECUTIONFLOW:3、调用CSUDIOSGSetSurfaceInfo将源缓存空间P1_1的ColorKey设置为C1
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域的颜色值为SCr2
//@EXECUTIONFLOW:5、初始化（填充）目标缓存空间区域的颜色值为DCr4
//@EXECUTIONFLOW:6、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M1），期望colorkey不对SCr3起作用
//@EXECUTIONFLOW:7、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:8、从P2_1_SInfo信息中获取任意一个像素点的颜色值应该存在SCr2颜色值
//@EXECUTIONFLOW:9、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试源绘图平面ColorKey为C2时的blit操作,两个平面都不存在C2颜色值的情况
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2+C2+SR1+DR1+SCr1+DCr4+M1
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、目标绘图平面存在SCr1颜色值
//@REMARK: 1、测试colorkey时需要有颜色数据值，并没有去关注矩形区域大小、颜色模式、alpha和混合模式   
//@REMARK: 2、只对X2或X1颜色模式进行测试，暂时不对其它颜色模式进行测试，可以根据测试覆盖率补充
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取源缓存空间P1_1的信息
//@EXECUTIONFLOW:3、调用CSUDIOSGSetSurfaceInfo将源缓存空间P1_1的ColorKey设置为C2
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域的颜色值为SCr1
//@EXECUTIONFLOW:5、初始化（填充）目标缓存空间区域的颜色值为DCr4
//@EXECUTIONFLOW:6、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M1）
//@EXECUTIONFLOW:7、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:8、从P2_1_SInfo信息中获取任意一个像素点的颜色值应该存在SCr1颜色值
//@EXECUTIONFLOW:9、调用CSUDIOSGDestroySurface销毁绘图平面
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
//**********目标绘图空间为缓存空间， 颜色模式为X2或X1**************//

//@TESTCASEID:CSTC_OSG_IT_Blit_Alpha_0001
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试设置源平面为全局alpha有效，目的绘图平面全局alpha无效情况下的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+SA1+DA1+SR3+DR3+SCr1+DCr2+M17
//@EXPECTATION:每一步都得到期望值
//@REMARK: 测试源全局alpha有效的情况
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取源P1_1
//@EXECUTIONFLOW:3、调用CSUDIOSGSetSurfaceInfo将源缓存空间P1_1的Alpha值设置为0xff(uAlpha以5的步长逐渐减小直到0)
//@EXECUTIONFLOW:4、初始化（填充）源缓存空间区域的颜色值为红色
//@EXECUTIONFLOW:5、初始化（填充）目标缓存空间区域的颜色值为绿色
//@EXECUTIONFLOW:6、调用CSUDIOSGBlit（P2_1,DR3,P1_1,SR3,M17）将源绘图平面的中央区域blit到目的绘图平面
//@EXECUTIONFLOW:7、调用CSUDIOSGSync完成blit操作
//@EXECUTIONFLOW:8、调用CSUDIOSGGetDisplaySurface获取显存空间句柄
//@EXECUTIONFLOW:9、调用CSUDIOSGBlit将P2_1绘图平面的颜色blit到显存空间
//@EXECUTIONFLOW:10、调用CSUDIOSGSync完成blit操作
//@EXECUTIONFLOW:11、循环执行步骤3到步骤10，观察平面的颜色，期望平面中央的红色逐渐变成透明色
//@EXECUTIONFLOW:12、调用CSUDIOSGDestroySurface销毁绘图平面
//@EXECUTIONFLOW:13、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:14、调用CSUDIOSGGetSurfaceInfo获取源P1_1
//@EXECUTIONFLOW:15、调用CSUDIOSGSetSurfaceInfo将源缓存空间P1_1的Alpha值设置为0xff(uAlpha以5的步长逐渐减小直到0)
//@EXECUTIONFLOW:16、初始化（填充）源缓存空间区域的颜色值为蓝色
//@EXECUTIONFLOW:17、初始化（填充）目标缓存空间区域的颜色值为绿色
//@EXECUTIONFLOW:18、调用CSUDIOSGBlit（P2_1,DR3,P1_1,SR3,M17）将源绘图平面的中央区域blit到目的绘图平面
//@EXECUTIONFLOW:19、调用CSUDIOSGSync完成blit操作
//@EXECUTIONFLOW:20、调用CSUDIOSGGetDisplaySurface获取显存空间句柄
//@EXECUTIONFLOW:21、调用CSUDIOSGBlit将P2_1绘图平面的颜色blit到显存空间
//@EXECUTIONFLOW:22、调用CSUDIOSGSync完成blit操作
//@EXECUTIONFLOW:23、循环执行步骤15到步骤22，观察平面的颜色，期望平面中央的红色逐渐变成透明色
//@EXECUTIONFLOW:24、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Alpha_0001(void)
{   	
	CSUDI_BOOL bRet = CSUDI_TRUE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	unsigned int uSrcColorRed = OSG_COLOR_RED_32BIT;
	unsigned int uDstColorGreen = OSG_COLOR_GREEN_32BIT;
	unsigned int uDstColorBlue = OSG_COLOR_BLUE_32BIT;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"没有找到任何支持的满足要求的PixelFormat"
			);

	if(ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)
	{
		uSrcColorRed = OSG_COLOR_RED_16BIT;
		uDstColorGreen = OSG_COLOR_GREEN_16BIT;
		uDstColorBlue = OSG_COLOR_BLUE_16BIT;
	}
	
	CSTCPrint("注意屏幕中央的红色逐渐变成透明色!!\r\n");
	CSTKWaitAnyKey();	
	bRet &= OSG_Blit_TestAlphaManual(ePixelFormat, uSrcColorRed, uDstColorGreen);
	CSTCPrint("以上现象是否符合期望值!!\r\n");
	CSTCPrint("Yes,与期望现象相同 .\r\n");
	CSTCPrint("No,与期望现象不同.\r\n");
	bRet &= CSTKWaitYes();

	CSTCPrint("注意屏幕中央的红色逐渐变成透明色!!\r\n");
	CSTKWaitAnyKey();
	bRet = OSG_Blit_TestAlphaManual(ePixelFormat, uSrcColorRed, uDstColorBlue);
	CSTCPrint("以上现象是否符合期望值!!\r\n");
	CSTCPrint("Yes,与期望现象相同 .\r\n");
	CSTCPrint("No,与期望现象不同.\r\n");
	bRet &= CSTKWaitYes();

	CSTCPrint("注意屏幕中央的蓝色逐渐变成透明色!!\r\n");
	CSTKWaitAnyKey();
	bRet &= OSG_Blit_TestAlphaManual(ePixelFormat, uDstColorBlue, uDstColorGreen);
	CSTCPrint("以上现象是否符合期望值!!\r\n");
	CSTCPrint("Yes,与期望现象相同 .\r\n");
	CSTCPrint("No,与期望现象不同.\r\n");
	bRet &= CSTKWaitYes();
	
	CSTCPrint("注意屏幕中央的蓝色逐渐变成透明色!!\r\n");
	CSTKWaitAnyKey();
	bRet &= OSG_Blit_TestAlphaManual(ePixelFormat, uDstColorBlue, uSrcColorRed);
	CSTCPrint("以上现象是否符合期望值!!\r\n");
	CSTCPrint("Yes,与期望现象相同 .\r\n");
	CSTCPrint("No,与期望现象不同.\r\n");
	bRet &= CSTKWaitYes();
	
	CSTCPrint("注意屏幕中央的绿色逐渐变成透明色!!\r\n");
	CSTKWaitAnyKey();
	bRet &= OSG_Blit_TestAlphaManual(ePixelFormat, uDstColorGreen, uSrcColorRed);
	CSTCPrint("以上现象是否符合期望值!!\r\n");
	CSTCPrint("Yes,与期望现象相同 .\r\n");
	CSTCPrint("No,与期望现象不同.\r\n");
	bRet &= CSTKWaitYes();	

	CSTCPrint("注意屏幕中央的绿色逐渐变成透明色!!\r\n");
	CSTKWaitAnyKey();
	bRet &= OSG_Blit_TestAlphaManual(ePixelFormat, uDstColorGreen, uDstColorBlue);
	CSTCPrint("以上现象是否符合期望值!!\r\n");
	CSTCPrint("Yes,与期望现象相同 .\r\n");
	CSTCPrint("No,与期望现象不同.\r\n");
	bRet &= CSTKWaitYes();	

	CSTK_FATAL_POINT;
	
	return bRet;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Alpha_0002
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试在alpha模式下的blit操作,颜色值分别为SCr1、DCr2；alpha值分别为SA1、DA1
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+SA1+DA1+SR1+DR1+SCr1+DCr2+M17
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值和alpha值符合用例CSTC_OSG_IT_Blit_0000中的公式(1)(2)(3)(4)
//@REMARK: 1、测试alpha时需要颜色数据值，并没有关注颜色模式和矩形区域   
//@REMARK: 2、只对X2或X1颜色模式进行测试，暂时不对其它颜色模式进行测试，可以根据测试覆盖率补充
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取源P1_1和目标缓存空间P2_1的信息
//@EXECUTIONFLOW:3、调用CSUDIOSGSetSurfaceInfo将源缓存空间P1_1的Alpha值设置为SA1
//@EXECUTIONFLOW:4、调用CSUDIOSGSetSurfaceInfo将目标缓存空间P2_1的Alpha值设置为DA1
//@EXECUTIONFLOW:5、初始化（填充）源缓存空间区域的颜色值为SCr1
//@EXECUTIONFLOW:6、初始化（填充）目标缓存空间区域的颜色值为DCr2
//@EXECUTIONFLOW:7、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M17）
//@EXECUTIONFLOW:8、调用CSUDIOSGSync完成blit操作
//@EXECUTIONFLOW:9、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:10、从P2_1_SInfo信息中获取任意一个点的颜色值，符合公式(1)(2)(3)(4)
//@EXECUTIONFLOW:11、调用CSUDIOSGDestroySurface销毁绘图平面
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
				"没有找到任何支持的满足要求的PixelFormat"
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
//@DESCRIPTION:测试在alpha模式下的blit操作,颜色值分别为SCr2、DCr1；alpha值分别为SA1、DA1
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+SA1+DA1+SR1+DR1+SCr2+DCr1+M17
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值和alpha值符合用例CSTC_OSG_IT_Blit_0000中的公式(1)(2)(3)(4)
//@REMARK: 1、测试alpha时需要颜色数据值，并没有关注颜色模式和矩形区域   
//@REMARK: 2、只对X2颜色模式进行测试，暂时不对其它颜色模式进行测试，可以根据测试覆盖率补充
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取源P1_1和目标缓存空间P2_1的信息
//@EXECUTIONFLOW:3、调用CSUDIOSGSetSurfaceInfo将源缓存空间P1_1的Alpha值设置为SA1
//@EXECUTIONFLOW:4、调用CSUDIOSGSetSurfaceInfo将目标缓存空间P2_1的Alpha值设置为DA1
//@EXECUTIONFLOW:5、初始化（填充）源缓存空间区域的颜色值为SCr2
//@EXECUTIONFLOW:6、初始化（填充）目标缓存空间区域的颜色值为DCr1
//@EXECUTIONFLOW:7、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M17）
//@EXECUTIONFLOW:8、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:9、从P2_1_SInfo信息中获取任意一个点的颜色值，符合公式(1)(2)(3)(4)
//@EXECUTIONFLOW:10、调用CSUDIOSGDestroySurface销毁绘图平面
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
				"没有找到任何支持的满足要求的PixelFormat"
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
//@DESCRIPTION:测试在alpha模式下的blit操作,颜色值分别为SCr2、DCr2；alpha值分别为SA1、DA1
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+SA1+DA1+SR1+DR1+SCr2+DCr2+M17
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值和alpha值符合用例CSTC_OSG_IT_Blit_0000中的公式(1)(2)(3)(4)
//@REMARK: 1、测试alpha时需要颜色数据值，并没有关注颜色模式和矩形区域   
//@REMARK: 2、只对X2或X1颜色模式进行测试，暂时不对其它颜色模式进行测试，可以根据测试覆盖率补充
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取源P1_1和目标缓存空间P2_1的信息
//@EXECUTIONFLOW:3、调用CSUDIOSGSetSurfaceInfo将源缓存空间P1_1的Alpha值设置为SA1
//@EXECUTIONFLOW:4、调用CSUDIOSGSetSurfaceInfo将目标缓存空间P2_1的Alpha值设置为DA1
//@EXECUTIONFLOW:5、初始化（填充）源缓存空间区域的颜色值为SCr2
//@EXECUTIONFLOW:6、初始化（填充）目标缓存空间区域的颜色值为DCr2
//@EXECUTIONFLOW:7、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M17）
//@EXECUTIONFLOW:8、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:9、从P2_1_SInfo信息中获取任意一个点的颜色值，符合公式(1)(2)(3)(4)
//@EXECUTIONFLOW:10、调用CSUDIOSGDestroySurface销毁绘图平面
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
				"没有找到任何支持的满足要求的PixelFormat"
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
//@DESCRIPTION:测试在alpha模式下的blit操作,颜色值分别为SCr1、DCr1；alpha值分别为SA1、DA2
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+SA1+DA2+SR1+DR1+SCr1+DCr1+M17
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值和alpha值符合用例CSTC_OSG_IT_Blit_0000中的公式(1)(2)(3)(4)
//@REMARK: 1、测试alpha时需要颜色数据值，并没有关注颜色模式和矩形区域   
//@REMARK: 2、只对X2或X1颜色模式进行测试，暂时不对其它颜色模式进行测试，可以根据测试覆盖率补充
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取源P1_1和目标缓存空间P2_1的信息
//@EXECUTIONFLOW:3、调用CSUDIOSGSetSurfaceInfo将源缓存空间P1_1的Alpha值设置为SA1
//@EXECUTIONFLOW:4、调用CSUDIOSGSetSurfaceInfo将目标缓存空间P2_1的Alpha值设置为DA2
//@EXECUTIONFLOW:5、初始化（填充）源缓存空间区域的颜色值为SCr1
//@EXECUTIONFLOW:6、初始化（填充）目标缓存空间区域的颜色值为DCr1
//@EXECUTIONFLOW:7、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M17）
//@EXECUTIONFLOW:8、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:9、从P2_1_SInfo信息中获取任意一个点的颜色值，符合公式(1)(2)(3)(4)
//@EXECUTIONFLOW:10、调用CSUDIOSGDestroySurface销毁绘图平面
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
				"没有找到任何支持的满足要求的PixelFormat"
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
//@DESCRIPTION:测试在alpha模式下的blit操作,颜色值分别为SCr1、DCr2；alpha值分别为SA1、DA2
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+SA1+DA2+SR1+DR1+SCr1+DCr2+M17
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值和alpha值符合用例CSTC_OSG_IT_Blit_0000中的公式(1)(2)(3)(4)
//@REMARK: 1、测试alpha时需要颜色数据值，并没有关注颜色模式和矩形区域   
//@REMARK: 2、只对X2或X1颜色模式进行测试，暂时不对其它颜色模式进行测试，可以根据测试覆盖率补充
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取源P1_1和目标缓存空间P2_1的信息
//@EXECUTIONFLOW:3、调用CSUDIOSGSetSurfaceInfo将源缓存空间P1_1的Alpha值设置为SA1
//@EXECUTIONFLOW:4、调用CSUDIOSGSetSurfaceInfo将目标缓存空间P2_1的Alpha值设置为DA2
//@EXECUTIONFLOW:5、初始化（填充）源缓存空间区域的颜色值为SCr1
//@EXECUTIONFLOW:6、初始化（填充）目标缓存空间区域的颜色值为DCr2
//@EXECUTIONFLOW:7、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M17）
//@EXECUTIONFLOW:8、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:9、从P2_1_SInfo信息中获取任意一个点的颜色值，符合公式(1)(2)(3)(4)
//@EXECUTIONFLOW:10、调用CSUDIOSGDestroySurface销毁绘图平面
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
				"没有找到任何支持的满足要求的PixelFormat"
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
//@DESCRIPTION:测试在alpha模式下的blit操作,颜色值分别为SCr2、DCr1；alpha值分别为SA1、DA2
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+SA1+DA2+SR1+DR1+SCr2+DCr1+M17
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值和alpha值符合用例CSTC_OSG_IT_Blit_0000中的公式(1)(2)(3)(4)
//@REMARK: 1、测试alpha时需要颜色数据值，并没有关注颜色模式和矩形区域   
//@REMARK: 2、只对X2或X1颜色模式进行测试，暂时不对其它颜色模式进行测试，可以根据测试覆盖率补充
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取源P1_1和目标缓存空间P2_1的信息
//@EXECUTIONFLOW:3、调用CSUDIOSGSetSurfaceInfo将源缓存空间P1_1的Alpha值设置为SA1
//@EXECUTIONFLOW:4、调用CSUDIOSGSetSurfaceInfo将目标缓存空间P2_1的Alpha值设置为DA2
//@EXECUTIONFLOW:5、初始化（填充）源缓存空间区域的颜色值为SCr2
//@EXECUTIONFLOW:6、初始化（填充）目标缓存空间区域的颜色值为DCr1
//@EXECUTIONFLOW:7、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M17）
//@EXECUTIONFLOW:8、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:9、从P2_1_SInfo信息中获取任意一个点的颜色值，符合公式(1)(2)(3)(4)
//@EXECUTIONFLOW:10、调用CSUDIOSGDestroySurface销毁绘图平面
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
				"没有找到任何支持的满足要求的PixelFormat"
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
//@DESCRIPTION:测试在alpha模式下的blit操作,颜色值分别为SCr2、DCr2；alpha值分别为SA1、DA2
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+SA1+DA2+SR1+DR1+SCr2+DCr2+M17
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值和alpha值符合用例CSTC_OSG_IT_Blit_0000中的公式(1)(2)(3)(4)
//@REMARK: 1、测试alpha时需要颜色数据值，并没有关注颜色模式和矩形区域   
//@REMARK: 2、只对X2或X1颜色模式进行测试，暂时不对其它颜色模式进行测试，可以根据测试覆盖率补充
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取源P1_1和目标缓存空间P2_1的信息
//@EXECUTIONFLOW:3、调用CSUDIOSGSetSurfaceInfo将源缓存空间P1_1的Alpha值设置为SA1
//@EXECUTIONFLOW:4、调用CSUDIOSGSetSurfaceInfo将目标缓存空间P2_1的Alpha值设置为DA2
//@EXECUTIONFLOW:5、初始化（填充）源缓存空间区域的颜色值为SCr2
//@EXECUTIONFLOW:6、初始化（填充）目标缓存空间区域的颜色值为DCr2
//@EXECUTIONFLOW:7、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M17）
//@EXECUTIONFLOW:8、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:9、从P2_1_SInfo信息中获取任意一个点的颜色值，符合公式(1)(2)(3)(4)
//@EXECUTIONFLOW:10、调用CSUDIOSGDestroySurface销毁绘图平面
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
				"没有找到任何支持的满足要求的PixelFormat"
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
//@DESCRIPTION:测试在alpha模式下的blit操作,颜色值分别为SCr1、DCr1；alpha值分别为SA2、DA1
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+SA2+DA1+SR1+DR1+SCr1+DCr1+M17
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值和alpha值符合用例CSTC_OSG_IT_Blit_0000中的公式(1)(2)(3)(4)
//@REMARK: 1、测试alpha时需要颜色数据值，并没有关注颜色模式和矩形区域   
//@REMARK: 2、只对X2或X1颜色模式进行测试，暂时不对其它颜色模式进行测试，可以根据测试覆盖率补充
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取源P1_1和目标缓存空间P2_1的信息
//@EXECUTIONFLOW:3、调用CSUDIOSGSetSurfaceInfo将源缓存空间P1_1的Alpha值设置为SA2
//@EXECUTIONFLOW:4、调用CSUDIOSGSetSurfaceInfo将目标缓存空间P2_1的Alpha值设置为DA1
//@EXECUTIONFLOW:5、初始化（填充）源缓存空间区域的颜色值为SCr1
//@EXECUTIONFLOW:6、初始化（填充）目标缓存空间区域的颜色值为DCr1
//@EXECUTIONFLOW:7、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M17）
//@EXECUTIONFLOW:8、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:9、从P2_1_SInfo信息中获取任意一个点的颜色值，符合公式(1)(2)(3)(4)
//@EXECUTIONFLOW:10、从P2_1_SInfo信息中获取平面的alpha值，符合公式(1)
//@EXECUTIONFLOW:11、调用CSUDIOSGDestroySurface销毁绘图平面
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
				"没有找到任何支持的满足要求的PixelFormat"
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
//@DESCRIPTION:测试在alpha模式下的blit操作,颜色值分别为SCr1、DCr2；alpha值分别为SA2、DA1
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+SA2+DA1+SR1+DR1+SCr1+DCr2+M17
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值和alpha值符合用例CSTC_OSG_IT_Blit_0000中的公式(1)(2)(3)(4)
//@REMARK: 1、测试alpha时需要颜色数据值，并没有关注颜色模式和矩形区域   
//@REMARK: 2、只对X2或X1颜色模式进行测试，暂时不对其它颜色模式进行测试，可以根据测试覆盖率补充
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取源P1_1和目标缓存空间P2_1的信息
//@EXECUTIONFLOW:3、调用CSUDIOSGSetSurfaceInfo将源缓存空间P1_1的Alpha值设置为SA2
//@EXECUTIONFLOW:4、调用CSUDIOSGSetSurfaceInfo将目标缓存空间P2_1的Alpha值设置为DA1
//@EXECUTIONFLOW:5、初始化（填充）源缓存空间区域的颜色值为SCr1
//@EXECUTIONFLOW:6、初始化（填充）目标缓存空间区域的颜色值为DCr2
//@EXECUTIONFLOW:7、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M17）
//@EXECUTIONFLOW:8、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:9、从P2_1_SInfo信息中获取任意一个点的颜色值，符合公式(1)(2)(3)(4)
//@EXECUTIONFLOW:10、调用CSUDIOSGDestroySurface销毁绘图平面
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
				"没有找到任何支持的满足要求的PixelFormat"
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
//@DESCRIPTION:测试在alpha模式下的blit操作,颜色值分别为SCr2、DCr1；alpha值分别为SA2、DA1
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+SA2+DA1+SR1+DR1+SCr2+DCr1+M17
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值和alpha值符合用例CSTC_OSG_IT_Blit_0000中的公式(1)(2)(3)(4)
//@REMARK: 1、测试alpha时需要颜色数据值，并没有关注颜色模式和矩形区域   
//@REMARK: 2、只对X2或X1颜色模式进行测试，暂时不对其它颜色模式进行测试，可以根据测试覆盖率补充
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取源P1_1和目标缓存空间P2_1的信息
//@EXECUTIONFLOW:3、调用CSUDIOSGSetSurfaceInfo将源缓存空间P1_1的Alpha值设置为SA2
//@EXECUTIONFLOW:4、调用CSUDIOSGSetSurfaceInfo将目标缓存空间P2_1的Alpha值设置为DA1
//@EXECUTIONFLOW:5、初始化（填充）源缓存空间区域的颜色值为SCr2
//@EXECUTIONFLOW:6、初始化（填充）目标缓存空间区域的颜色值为DCr1
//@EXECUTIONFLOW:7、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M17）
//@EXECUTIONFLOW:8、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:9、从P2_1_SInfo信息中获取任意一个点的颜色值，符合公式(1)(2)(3)(4)
//@EXECUTIONFLOW:10、调用CSUDIOSGDestroySurface销毁绘图平面
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
				"没有找到任何支持的满足要求的PixelFormat"
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
//@DESCRIPTION:测试在alpha模式下的blit操作,颜色值分别为SCr2、DCr2；alpha值分别为SA2、DA1
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+SA2+DA1+SR1+DR1+SCr2+DCr2+M17
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值和alpha值符合用例CSTC_OSG_IT_Blit_0000中的公式(1)(2)(3)(4)
//@REMARK: 1、测试alpha时需要颜色数据值，并没有关注颜色模式和矩形区域   
//@REMARK: 2、只对X2或X1颜色模式进行测试，暂时不对其它颜色模式进行测试，可以根据测试覆盖率补充
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取源P1_1和目标缓存空间P2_1的信息
//@EXECUTIONFLOW:3、调用CSUDIOSGSetSurfaceInfo将源缓存空间P1_1的Alpha值设置为SA2
//@EXECUTIONFLOW:4、调用CSUDIOSGSetSurfaceInfo将目标缓存空间P2_1的Alpha值设置为DA1
//@EXECUTIONFLOW:5、初始化（填充）源缓存空间区域的颜色值为SCr2
//@EXECUTIONFLOW:6、初始化（填充）目标缓存空间区域的颜色值为DCr2
//@EXECUTIONFLOW:7、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M17）
//@EXECUTIONFLOW:8、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:9、从P2_1_SInfo信息中获取任意一个点的颜色值，符合公式(1)(2)(3)(4)
//@EXECUTIONFLOW:10、调用CSUDIOSGDestroySurface销毁绘图平面
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
				"没有找到任何支持的满足要求的PixelFormat"
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
//@DESCRIPTION:测试在alpha模式下的blit操作,颜色值分别为SCr1、DCr1；alpha值分别为SA2、DA2
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+SA2+DA2+SR1+DR1+SCr1+DCr1+M17
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值和alpha值符合用例CSTC_OSG_IT_Blit_0000中的公式(1)(2)(3)(4)
//@REMARK: 1、测试alpha时需要颜色数据值，并没有关注颜色模式和矩形区域   
//@REMARK: 2、只对X2或X1颜色模式进行测试，暂时不对其它颜色模式进行测试，可以根据测试覆盖率补充
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取源P1_1和目标缓存空间P2_1的信息
//@EXECUTIONFLOW:3、调用CSUDIOSGSetSurfaceInfo将源缓存空间P1_1的Alpha值设置为SA2
//@EXECUTIONFLOW:4、调用CSUDIOSGSetSurfaceInfo将目标缓存空间P2_1的Alpha值设置为DA2
//@EXECUTIONFLOW:5、初始化（填充）源缓存空间区域的颜色值为SCr1
//@EXECUTIONFLOW:6、初始化（填充）目标缓存空间区域的颜色值为DCr1
//@EXECUTIONFLOW:7、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M17）
//@EXECUTIONFLOW:8、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:9、从P2_1_SInfo信息中获取任意一个点的颜色值，符合公式(1)(2)(3)(4)
//@EXECUTIONFLOW:10、调用CSUDIOSGDestroySurface销毁绘图平面
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
				"没有找到任何支持的满足要求的PixelFormat"
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
//@DESCRIPTION:测试在alpha模式下的blit操作,颜色值分别为SCr1、DCr2；alpha值分别为SA2、DA2
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+SA2+DA2+SR1+DR1+SCr1+DCr2+M17
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值和alpha值符合用例CSTC_OSG_IT_Blit_0000中的公式(1)(2)(3)(4)
//@REMARK: 1、测试alpha时需要颜色数据值，并没有关注颜色模式和矩形区域   
//@REMARK: 2、只对X2或X1颜色模式进行测试，暂时不对其它颜色模式进行测试，可以根据测试覆盖率补充
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取源P1_1和目标缓存空间P2_1的信息
//@EXECUTIONFLOW:3、调用CSUDIOSGSetSurfaceInfo将源缓存空间P1_1的Alpha值设置为SA2
//@EXECUTIONFLOW:4、调用CSUDIOSGSetSurfaceInfo将目标缓存空间P2_1的Alpha值设置为DA2
//@EXECUTIONFLOW:5、初始化（填充）源缓存空间区域的颜色值为SCr1
//@EXECUTIONFLOW:6、初始化（填充）目标缓存空间区域的颜色值为DCr2
//@EXECUTIONFLOW:7、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M17）
//@EXECUTIONFLOW:8、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:9、从P2_1_SInfo信息中获取任意一个点的颜色值，符合公式(1)(2)(3)(4)
//@EXECUTIONFLOW:10、调用CSUDIOSGDestroySurface销毁绘图平面
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
				"没有找到任何支持的满足要求的PixelFormat"
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
//@DESCRIPTION:测试在alpha模式下的blit操作,颜色值分别为SCr2、DCr1；alpha值分别为SA2、DA2
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+SA2+DA2+SR1+DR1+SCr2+DCr1+M17
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值和alpha值符合用例CSTC_OSG_IT_Blit_0000中的公式(1)(2)(3)(4)
//@REMARK: 1、测试alpha时需要颜色数据值，并没有关注颜色模式和矩形区域   
//@REMARK: 2、只对X2或X1颜色模式进行测试，暂时不对其它颜色模式进行测试，可以根据测试覆盖率补充
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取源P1_1和目标缓存空间P2_1的信息
//@EXECUTIONFLOW:3、调用CSUDIOSGSetSurfaceInfo将源缓存空间P1_1的Alpha值设置为SA2
//@EXECUTIONFLOW:4、调用CSUDIOSGSetSurfaceInfo将目标缓存空间P2_1的Alpha值设置为DA2
//@EXECUTIONFLOW:5、初始化（填充）源缓存空间区域的颜色值为SCr2
//@EXECUTIONFLOW:6、初始化（填充）目标缓存空间区域的颜色值为DCr1
//@EXECUTIONFLOW:7、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M17）
//@EXECUTIONFLOW:8、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:9、从P2_1_SInfo信息中获取任意一个点的颜色值，符合公式(1)(2)(3)(4)
//@EXECUTIONFLOW:10、从P2_1_SInfo信息中获取平面的alpha值，符合公式(1)
//@EXECUTIONFLOW:11、调用CSUDIOSGDestroySurface销毁绘图平面
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
				"没有找到任何支持的满足要求的PixelFormat"
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
//@DESCRIPTION:测试在alpha模式下的blit操作,颜色值分别为SCr2、DCr2；alpha值分别为SA2、DA2
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+SA2+DA2+SR1+DR1+SCr2+DCr2+M17
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值和alpha值符合用例CSTC_OSG_IT_Blit_0000中的公式(1)(2)(3)(4)
//@REMARK: 1、测试alpha时需要颜色数据值，并没有关注颜色模式和矩形区域   
//@REMARK: 2、只对X2或X1颜色模式进行测试，暂时不对其它颜色模式进行测试，可以根据测试覆盖率补充
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取源P1_1和目标缓存空间P2_1的信息
//@EXECUTIONFLOW:3、调用CSUDIOSGSetSurfaceInfo将源缓存空间P1_1的Alpha值设置为SA2
//@EXECUTIONFLOW:4、调用CSUDIOSGSetSurfaceInfo将目标缓存空间P2_1的Alpha值设置为DA2
//@EXECUTIONFLOW:5、初始化（填充）源缓存空间区域的颜色值为SCr2
//@EXECUTIONFLOW:6、初始化（填充）目标缓存空间区域的颜色值为DCr2
//@EXECUTIONFLOW:7、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M17）
//@EXECUTIONFLOW:8、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:9、从P2_1_SInfo信息中获取任意一个点的颜色值，符合公式(1)(2)(3)(4)
//@EXECUTIONFLOW:10、调用CSUDIOSGDestroySurface销毁绘图平面
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
				"没有找到任何支持的满足要求的PixelFormat"
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
//@DESCRIPTION:测试在alpha模式下的blit操作,颜色值为cs_osgcolordata.c文件中的值；alpha值分别为SA1、DA1
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2
//@INPUT: P1_1+P2_1+X2/X1+SA1+DA1+SR1+DR1+SCr5+DCr5+M17
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值和alpha值符合用例CSTC_OSG_IT_Blit_0000中的公式(1)(2)(3)(4)
//@REMARK: 1、测试alpha时需要颜色数据值，并没有关注颜色模式和矩形区域   
//@REMARK: 2、只对X2或X1颜色模式进行测试，暂时不对其它颜色模式进行测试，可以根据测试覆盖率补充
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取源P1_1和目标缓存空间P2_1的信息
//@EXECUTIONFLOW:3、调用CSUDIOSGSetSurfaceInfo将源缓存空间P1_1的Alpha值设置为SA1
//@EXECUTIONFLOW:4、调用CSUDIOSGSetSurfaceInfo将目标缓存空间P2_1的Alpha值设置为DA1
//@EXECUTIONFLOW:5、初始化（填充）源缓存空间区域的颜色值为SCr5
//@EXECUTIONFLOW:6、初始化（填充）目标缓存空间区域的颜色值为DCr5
//@EXECUTIONFLOW:7、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M17）
//@EXECUTIONFLOW:8、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:9、从P2_1_SInfo信息中获取任意一个点的颜色值，符合公式(1)(2)(3)(4)
//@EXECUTIONFLOW:10、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试在alpha模式下的blit操作,源数据为带coship字样的透明效果图片，目的亦为透明效果图片(没有全局透明度)
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2
//@INPUT: P1_1+P2_1+X2/X1+SA1+DA1+SR1+DR1
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值和alpha值符合用例CSTC_OSG_IT_Blit_0000中的公式(1)(2)(3)(4)
//@REMARK: 1、测试alpha时需要颜色数据值，并没有关注颜色模式和矩形区域   
//@REMARK: 2、只对X2或X1颜色模式进行测试，暂时不对其它颜色模式进行测试，可以根据测试覆盖率补充
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取源P1_1和目标缓存空间P2_1的信息
//@EXECUTIONFLOW:3、调用CSUDIOSGSetSurfaceInfo将源缓存空间P1_1的Alpha值设置为SA1
//@EXECUTIONFLOW:4、调用CSUDIOSGSetSurfaceInfo将目标缓存空间P2_1的Alpha值设置为DA1
//@EXECUTIONFLOW:5、初始化（填充）源缓存空间区域的数据为coship字样数据
//@EXECUTIONFLOW:6、初始化（填充）目标缓存空间区域的数据为蓝色长条图片。
//@EXECUTIONFLOW:7、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1）
//@EXECUTIONFLOW:8、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:9、从P2_1_SInfo信息中获取任意一个点的颜色值，符合公式(1)(2)(3)(4)
//@EXECUTIONFLOW:10、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试在alpha模式下的blit操作,源数据为带coship字样的透明效果图片，目的亦为透明效果图片(源有全局透明度)
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2
//@INPUT: P1_1+P2_1+X2+SA1+DA1+SR1+DR1
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值和alpha值符合用例CSTC_OSG_IT_Blit_0000中的公式(1)(2)(3)(4)
//@REMARK: 1、测试alpha时需要颜色数据值，并没有关注颜色模式和矩形区域   
//@REMARK: 2、只对X2颜色模式进行测试，暂时不对其它颜色模式进行测试，可以根据测试覆盖率补充
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取源P1_1和目标缓存空间P2_1的信息
//@EXECUTIONFLOW:3、调用CSUDIOSGSetSurfaceInfo将源缓存空间P1_1的Alpha值设置为SA1
//@EXECUTIONFLOW:4、调用CSUDIOSGSetSurfaceInfo将目标缓存空间P2_1的Alpha值设置为DA1
//@EXECUTIONFLOW:5、初始化（填充）源缓存空间区域的数据为coship字样数据
//@EXECUTIONFLOW:6、初始化（填充）目标缓存空间区域的数据为蓝色长条图片。
//@EXECUTIONFLOW:7、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1）
//@EXECUTIONFLOW:8、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:9、从P2_1_SInfo信息中获取任意一个点的颜色值，符合公式(1)(2)(3)(4)
//@EXECUTIONFLOW:10、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试在alpha模式下的blit操作,源数据为带coship字样的透明效果图片，目的亦为透明效果图片(目的有全局透明度)
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2
//@INPUT: P1_1+P2_1+X2/X1+SA1+DA1+SR1+DR1
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值和alpha值符合用例CSTC_OSG_IT_Blit_0000中的公式(1)(2)(3)(4)
//@REMARK: 1、测试alpha时需要颜色数据值，并没有关注颜色模式和矩形区域   
//@REMARK: 2、只对X2或X1颜色模式进行测试，暂时不对其它颜色模式进行测试，可以根据测试覆盖率补充
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取源P1_1和目标缓存空间P2_1的信息
//@EXECUTIONFLOW:3、调用CSUDIOSGSetSurfaceInfo将源缓存空间P1_1的Alpha值设置为SA1
//@EXECUTIONFLOW:4、调用CSUDIOSGSetSurfaceInfo将目标缓存空间P2_1的Alpha值设置为DA1
//@EXECUTIONFLOW:5、初始化（填充）源缓存空间区域的数据为coship字样数据
//@EXECUTIONFLOW:6、初始化（填充）目标缓存空间区域的数据为蓝色长条图片。
//@EXECUTIONFLOW:7、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1）
//@EXECUTIONFLOW:8、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:9、从P2_1_SInfo信息中获取任意一个点的颜色值，符合公式(1)(2)(3)(4)
//@EXECUTIONFLOW:10、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试在alpha模式下的blit操作,源数据为带coship字样的透明效果图片，目的亦为透明效果图片(源、目的均有全局透明度)
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2
//@INPUT: P1_1+P2_1+X2/X1+SA1+DA1+SR1+DR1
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值和alpha值符合用例CSTC_OSG_IT_Blit_0000中的公式(1)(2)(3)(4)
//@REMARK: 1、测试alpha时需要颜色数据值，并没有关注颜色模式和矩形区域   
//@REMARK: 2、只对X2或X1颜色模式进行测试，暂时不对其它颜色模式进行测试，可以根据测试覆盖率补充
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取源P1_1和目标缓存空间P2_1的信息
//@EXECUTIONFLOW:3、调用CSUDIOSGSetSurfaceInfo将源缓存空间P1_1的Alpha值设置为SA1
//@EXECUTIONFLOW:4、调用CSUDIOSGSetSurfaceInfo将目标缓存空间P2_1的Alpha值设置为DA1
//@EXECUTIONFLOW:5、初始化（填充）源缓存空间区域的数据为coship字样数据
//@EXECUTIONFLOW:6、初始化（填充）目标缓存空间区域的数据为蓝色长条图片。
//@EXECUTIONFLOW:7、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1）
//@EXECUTIONFLOW:8、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:9、从P2_1_SInfo信息中获取任意一个点的颜色值，符合公式(1)(2)(3)(4)
//@EXECUTIONFLOW:10、调用CSUDIOSGDestroySurface销毁绘图平面
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
//**************************应该针对该测试组合设计颜色值*******************************//

//@TESTCASEID:CSTC_OSG_IT_Blit_OpMode_0001
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:测试在EM_UDIOSG_OPMODE_COPY混合模式下的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+SR1+DR1+SCr1+DCr4+M1
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值与源绘图平面的颜色值一致
//@REMARK: 1、没有针对特殊矩形区域进行测试   
//@REMARK: 2、没有针对不同颜色模式进行测试
//@REMARK: 3、此处不再对alpha模式进行测试
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域的颜色值为SCr1
//@EXECUTIONFLOW:3、初始化（填充）目标缓存空间区域的颜色值为DCr4
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M1）
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:6、从P2_1_SInfo信息中获取任意一个点的颜色值，应该为SCr1
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试在EM_UDIOSG_OPMODE_XOR混合模式下的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+SR1+DR1+SCr1+DCr4+M2
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值与源绘图平面的颜色值符合XOR结果
//@REMARK: 1、没有针对特殊矩形区域进行测试   
//@REMARK: 2、没有针对不同颜色模式进行测试
//@REMARK: 3、此处不再对alpha模式进行测试
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域的颜色值为SCr1
//@EXECUTIONFLOW:3、初始化（填充）目标缓存空间区域的颜色值为DCr4
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M2）
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:6、从P2_1_SInfo信息中获取任意一个点的颜色值，应该为SCr1^DCr4
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试在EM_UDIOSG_OPMODE_OR混合模式下的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+SR1+DR1+SCr1+DCr4+M3
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值与源绘图平面的颜色值符合OR结果
//@REMARK: 1、没有针对特殊矩形区域进行测试   
//@REMARK: 2、没有针对不同颜色模式进行测试
//@REMARK: 3、此处不再对alpha模式进行测试
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域的颜色值为SCr1
//@EXECUTIONFLOW:3、初始化（填充）目标缓存空间区域的颜色值为DCr4
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M3）
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:6、从P2_1_SInfo信息中获取任意一个点的颜色值，应该为SCr1|DCr4
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试在EM_UDIOSG_OPMODE_AND混合模式下的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+SR1+DR1+SCr1+DCr4+M4
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值与源绘图平面的颜色值符合AND结果
//@REMARK: 1、没有针对特殊矩形区域进行测试   
//@REMARK: 2、没有针对不同颜色模式进行测试
//@REMARK: 3、此处不再对alpha模式进行测试
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域的颜色值为SCr1
//@EXECUTIONFLOW:3、初始化（填充）目标缓存空间区域的颜色值为DCr4
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M4）
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:6、从P2_1_SInfo信息中获取任意一个点的颜色值，应该为SCr1&DCr4
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试在EM_UDIOSG_OPMODE_CLEAR混合模式下的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+SR1+DR1+SCr1+DCr4+M5
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值与源绘图平面的颜色值符合CLEAR结果
//@REMARK: 1、没有针对特殊矩形区域进行测试   
//@REMARK: 2、没有针对不同颜色模式进行测试
//@REMARK: 3、此处不再对alpha模式进行测试
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域的颜色值为SCr1
//@EXECUTIONFLOW:3、初始化（填充）目标缓存空间区域的颜色值为DCr4
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M4）
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:6、从P2_1_SInfo信息中获取任意一个点的颜色值，应该为0
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试在EM_UDIOSG_OPMODE_SET混合模式下的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+SR1+DR1+SCr1+DCr4+M6
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值与源绘图平面的颜色值符合SET结果
//@REMARK: 1、没有针对特殊矩形区域进行测试   
//@REMARK: 2、没有针对不同颜色模式进行测试
//@REMARK: 3、此处不再对alpha模式进行测试
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域的颜色值为SCr1
//@EXECUTIONFLOW:3、初始化（填充）目标缓存空间区域的颜色值为DCr4
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M6）
//@EXECUTIONFLOW:5、调用CSUDIOSGSync完成blit操作
//@EXECUTIONFLOW:6、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:7、从P2_1_SInfo信息中获取任意一个点的颜色值，应该为0xffffffff
//@EXECUTIONFLOW:8、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试在EM_UDIOSG_OPMODE_EQUIV混合模式下的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+SR1+DR1+SCr1+DCr4+M7
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值与源绘图平面的颜色值符合EQUIV结果
//@REMARK: 1、没有针对特殊矩形区域进行测试   
//@REMARK: 2、没有针对不同颜色模式进行测试
//@REMARK: 3、此处不再对alpha模式进行测试
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域的颜色值为SCr1
//@EXECUTIONFLOW:3、初始化（填充）目标缓存空间区域的颜色值为DCr4
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M7）
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:6、从P2_1_SInfo信息中获取任意一个点的颜色值，应该为~（SCrr^DCr4)
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试在EM_UDIOSG_OPMODE_NOR混合模式下的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+SR1+DR1+SCr1+DCr4+M8
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值与源绘图平面的颜色值符合NOR结果
//@REMARK: 1、没有针对特殊矩形区域进行测试   
//@REMARK: 2、没有针对不同颜色模式进行测试
//@REMARK: 3、此处不再对alpha模式进行测试
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域的颜色值为SCr1
//@EXECUTIONFLOW:3、初始化（填充）目标缓存空间区域的颜色值为DCr4
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M8）
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:6、从P2_1_SInfo信息中获取任意一个点的颜色值，应该为~（SCr1|DCr4）
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试在EM_UDIOSG_OPMODE_NAND混合模式下的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+SR1+DR1+SCr1+DCr4+M9
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值与源绘图平面的颜色值符合NAND结果
//@REMARK: 1、没有针对特殊矩形区域进行测试   
//@REMARK: 2、没有针对不同颜色模式进行测试
//@REMARK: 3、此处不再对alpha模式进行测试
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域的颜色值为SCr1
//@EXECUTIONFLOW:3、初始化（填充）目标缓存空间区域的颜色值为DCr4
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M9）
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:6、从P2_1_SInfo信息中获取任意一个点的颜色值，应该为~（SCr1&DCr4）
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试在EM_UDIOSG_OPMODE_INVERT混合模式下的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+SR1+DR1+SCr1+DCr4+M10
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值与源绘图平面的颜色值符合INVERT结果
//@REMARK: 1、没有针对特殊矩形区域进行测试   
//@REMARK: 2、没有针对不同颜色模式进行测试
//@REMARK: 3、此处不再对alpha模式进行测试
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域的颜色值为SCr1
//@EXECUTIONFLOW:3、初始化（填充）目标缓存空间区域的颜色值为DCr4
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M10）
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:6、从P2_1_SInfo信息中获取任意一个点的颜色值，应该为~DCr4
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试在EM_UDIOSG_OPMODE_COPYINVERTED混合模式下的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+SR1+DR1+SCr1+DCr4+M11
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值与源绘图平面的颜色值符合COPYINVERTED结果
//@REMARK: 1、没有针对特殊矩形区域进行测试   
//@REMARK: 2、没有针对不同颜色模式进行测试
//@REMARK: 3、此处不再对alpha模式进行测试
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域的颜色值为SCr1
//@EXECUTIONFLOW:3、初始化（填充）目标缓存空间区域的颜色值为DCr4
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M11）
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:6、从P2_1_SInfo信息中获取任意一个点的颜色值，应该为~SCr1
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试在EM_UDIOSG_OPMODE_ORINVERTED混合模式下的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+SR1+DR1+SCr1+DCr4+M12
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值与源绘图平面的颜色值符合ORINVERTED结果
//@REMARK: 1、没有针对特殊矩形区域进行测试   
//@REMARK: 2、没有针对不同颜色模式进行测试
//@REMARK: 3、此处不再对alpha模式进行测试
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域的颜色值为SCr1
//@EXECUTIONFLOW:3、初始化（填充）目标缓存空间区域的颜色值为DCr4
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M12）
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:6、从P2_1_SInfo信息中获取任意一个点的颜色值，应该为~SCr1|DCr4
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试在EM_UDIOSG_OPMODE_ANDINVERTED混合模式下的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+SR1+DR1+SCr1+DCr4+M13
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值与源绘图平面的颜色值符合ANDINVERTED结果
//@REMARK: 1、没有针对特殊矩形区域进行测试   
//@REMARK: 2、没有针对不同颜色模式进行测试
//@REMARK: 3、此处不再对alpha模式进行测试
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域的颜色值为SCr1
//@EXECUTIONFLOW:3、初始化（填充）目标缓存空间区域的颜色值为DCr4
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M13）
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:6、从P2_1_SInfo信息中获取任意一个点的颜色值，应该为~SCr1&DCr4
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试在EM_UDIOSG_OPMODE_ORREVERSE混合模式下的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+SR1+DR1+SCr1+DCr4+M14
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值与源绘图平面的颜色值符合ORREVERSE结果
//@REMARK: 1、没有针对特殊矩形区域进行测试   
//@REMARK: 2、没有针对不同颜色模式进行测试
//@REMARK: 3、此处不再对alpha模式进行测试
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域的颜色值为SCr1
//@EXECUTIONFLOW:3、初始化（填充）目标缓存空间区域的颜色值为DCr4
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M14）
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:6、从P2_1_SInfo信息中获取任意一个点的颜色值，应该为SCr1| ~DCr4
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试在EM_UDIOSG_OPMODE_ANDREVERSE混合模式下的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+SR1+DR1+SCr1+DCr4+M15
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值与源绘图平面的颜色值符合ANDREVERSE结果
//@REMARK: 1、没有针对特殊矩形区域进行测试   
//@REMARK: 2、没有针对不同颜色模式进行测试
//@REMARK: 3、此处不再对alpha模式进行测试
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域的颜色值为SCr1
//@EXECUTIONFLOW:3、初始化（填充）目标缓存空间区域的颜色值为DCr4
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M15）
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:6、从P2_1_SInfo信息中获取任意一个点的颜色值，应该为SCr1 &~DCr4
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试在EM_UDIOSG_OPMODE_NOOP混合模式下的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_1+X2/X1+SR1+DR1+SCr1+DCr4+M16
//@EXPECTATION:1、成功执行blit，返回CSUDI_SUCCESS
//@EXPECTATION:2、blit得到的颜色值不变
//@REMARK: 1、没有针对特殊矩形区域进行测试   
//@REMARK: 2、没有针对不同颜色模式进行测试
//@REMARK: 3、此处不再对alpha模式进行测试
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1、P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域的颜色值为SCr1
//@EXECUTIONFLOW:3、初始化（填充）目标缓存空间区域的颜色值为DCr4
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M16）
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:6、从P2_1_SInfo信息中获取任意一个点的颜色值，应该为DCr4
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试传入源绘图平面句柄为CSUDI_NULL下的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_3+P2_1+SR1+DR1+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 无   
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGBlit（P2_1,DR1,P1_3,SR1,M1）
//@EXECUTIONFLOW:3、blit操作失败并返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Surface_0001(void)
{   
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hSurface_D = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"没有找到任何支持的满足要求的PixelFormat"
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
//@DESCRIPTION:测试传入目的绘图平面句柄为CSUDI_NULL下的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+P2_3+X2/X1+SR1+DR1+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 无   
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGBlit（P2_3,DR1,P1_1,SR1,M1）
//@EXECUTIONFLOW:3、blit操作失败并返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Blit_Surface_0002(void)
{   
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hSurface_D = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"没有找到任何支持的满足要求的PixelFormat"
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
//@DESCRIPTION:测试传入eOpMode 非法的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_3+P2_3+SR1+DR1+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 无   
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_3，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGCreateSurface创建缓存空间P2_3，颜色模式为X2或X1
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit(P2_3,DR1,P1_3,SR1,M1+1)，操作失败并返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit(P2_3,DR1,P1_3,SR1,M17-1)，操作失败并返回CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_Blit_Surface_0003(void)
{   
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hSurface_D = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"没有找到任何支持的满足要求的PixelFormat"
			);
	
	nRet = CSUDIOSGCreateSurface(ePixelFormat, SURFACE_WIDTH_720, SURFACE_HEIGHT_576, &hSurface_S);
	CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hSurface_S != CSUDI_NULL),
							"Step 1 Create P1_1 Surface Failure !");

	nRet = CSUDIOSGCreateSurface(ePixelFormat, SURFACE_WIDTH_720, SURFACE_HEIGHT_576, &hSurface_D);
	CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hSurface_D != CSUDI_NULL),
							"Step 1 Create P1_1 Surface Failure !");
					
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDIOSG_ERROR_BAD_PARAMETER == CSUDIOSGBlit(hSurface_D, CSUDI_NULL, hSurface_S, CSUDI_NULL, EM_UDIOSG_OPMODE_ALPHA+1)),
				"eOpMode 错误参数检查失败"
			);	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDIOSG_ERROR_BAD_PARAMETER == CSUDIOSGBlit(hSurface_D, CSUDI_NULL, hSurface_S, CSUDI_NULL, EM_UDIOSG_OPMODE_COPY-1)),
				"eOpMode 错误参数检查失败"
			);
	
	CSTK_FATAL_POINT
	{
		if(hSurface_S != CSUDI_NULL)
		{
		      if(CSUDIOSGDestroySurface(hSurface_S) != CSUDI_SUCCESS)
		      	{
				 CSTCPrint("销毁Surface 失败\n");
		      	}
			  
			hSurface_S = CSUDI_NULL;
		}
		if(hSurface_D != CSUDI_NULL)
		{
			 if(CSUDIOSGDestroySurface(hSurface_D) != CSUDI_SUCCESS)
			 {
				 CSTCPrint("销毁Surface 失败");
			 }
			 
			hSurface_D = CSUDI_NULL;
		}
	}
	
	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Surface_0004
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:检测blit前源surface的数据cache已经同步到内存
//@PRECONDITION: 1、平台UDI已经成功初始化
//@EXPECTATION: surface B的所有像素点被变化成Cr1
//@REMARK: 无   
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建大小为100 * 80大小的surfaceA, surfaceB.
//@EXECUTIONFLOW:2、直接对surfaceA的内存赋值为cr1
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit将surfaceA数据blit到surfaceB中
//@EXECUTIONFLOW:4、检测surfaceB中的数据是否为cr1
//@EXECUTIONFLOW:5、重复步骤3~4三遍
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
				"没有找到任何支持的满足要求的PixelFormat"
			);
	
	switch(ePixelFormat) //填充颜色为0x88，期望的颜色值与颜色模式有关
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
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface_S)),"销毁hSurface_S 失败");
		hSurface_S = CSUDI_NULL;
	}
	
	if(hSurface_D != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface_D)),"销毁hSurface_D 失败");
		hSurface_D = CSUDI_NULL;
	}	
	
	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OSG_IT_Blit_Surface_0005
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:检测blit后目的surface的数据cache是否和内存同步
//@PRECONDITION: 1、平台UDI已经成功初始化
//@EXPECTATION: surface B的所有像素点被变化成Cr1
//@REMARK: 无   
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建大小为100 * 80大小的surfaceA, surfaceB.
//@EXECUTIONFLOW:2、调用CSUDIOSGFill的填充SurfaceA为cr1 : 0x88888888
//@EXECUTIONFLOW:3、直接对surfaceB的内存赋值为0xff
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit将surfaceA数据blit到surfaceB中
//@EXECUTIONFLOW:5、检测surfaceB中的数据是否为cr1
//@EXECUTIONFLOW:6、重复2~5三遍
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
				"没有找到任何支持的满足要求的PixelFormat"
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
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface_S)),"销毁hSurface_S 失败");
		hSurface_S = CSUDI_NULL;
	}
	
	if(hSurface_D != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface_D)),"销毁hSurface_D 失败");
		hSurface_D = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
}

//**********************************************************************************//
//******************************Begin Test Fill************************************//
//**********************************************************************************//
//**************************应该针对该测试组设计颜色值*******************************//

//@TESTCASEID:CSTC_OSG_IT_Fill_0001
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:测试传入填充矩形区域为CSUDI_NULL下的fill操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P2_1+X2/X1+DR1+SCr1
//@EXPECTATION: 成功执行fill操作，填充区域为全屏，返回CSUDI_SUCCESS
//@REMARK: 1、只对一种颜色值进行fill操作，可以根据测试覆盖率进行补充其它颜色值   
//@REMARK: 2、只对一种颜色模式进行fill操作，可以根据测试覆盖率补充其它颜色模式下的fill操作
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域的颜色值为0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGFill（P2_1,DR1,SCr1）
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:6、从P2_1_SInfo信息中获取任意一个点的颜色值，应该为SCr1
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试传入填充矩形区域为整个绘图平面的fill操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P2_1+X2/X1+DR2+SCr1
//@EXPECTATION: 成功执行fill操作，填充区域为全屏，返回CSUDI_SUCCESS
//@REMARK: 1、只对一种颜色值进行fill操作，可以根据测试覆盖率进行补充其它颜色值   
//@REMARK: 2、只对一种颜色模式进行fill操作，可以根据测试覆盖率补充其它颜色模式下的fill操作
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域的颜色值为0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGFill（P2_1,DR2,SCr1）
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:6、从P2_1_SInfo信息中获取任意一个点的颜色值，应该为SCr1
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试传入填充矩形区域为绘图平面中央的fill操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P2_1+X2/X1+DR3+SCr1
//@EXPECTATION: 成功执行fill操作，返回CSUDI_SUCCESS
//@REMARK: 1、只对一种颜色值进行fill操作，可以根据测试覆盖率进行补充其它颜色值   
//@REMARK: 2、只对一种颜色模式进行fill操作，可以根据测试覆盖率补充其它颜色模式下的fill操作
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域的颜色值为0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGFill（P2_1,DR3,SCr1）
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:6、从P2_1_SInfo信息中获取任意一个点的颜色值，应该为SCr1
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试传入填充矩形区域为非法值（0，0）的fill操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P2_1+X2/X1+DR4+SCr1
//@EXPECTATION: 执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1、只对一种颜色值进行fill操作，可以根据测试覆盖率进行补充其它颜色值   
//@REMARK: 2、只对一种颜色模式进行fill操作，可以根据测试覆盖率补充其它颜色模式下的fill操作
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域的颜色值为0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGFill（P2_1,DR4,SCr1）
//@EXECUTIONFLOW:4、执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试传入填充矩形区域为非法值（-1，-1）的fill操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P2_1+X2/X1+DR5+SCr1
//@EXPECTATION: 执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1、只对一种颜色值进行fill操作，可以根据测试覆盖率进行补充其它颜色值   
//@REMARK: 2、只对一种颜色模式进行fill操作，可以根据测试覆盖率补充其它颜色模式下的fill操作
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域的颜色值为0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGFill（P2_1,DR5,SCr1）
//@EXECUTIONFLOW:4、执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试传入填充矩形区域为一个像素的fill操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P2_1+X2/X1+DR6+SCr1
//@EXPECTATION: 成功执行fill操作，返回CSUDI_SUCCESS
//@REMARK: 1、只对一种颜色值进行fill操作，可以根据测试覆盖率进行补充其它颜色值   
//@REMARK: 2、只对一种颜色模式进行fill操作，可以根据测试覆盖率补充其它颜色模式下的fill操作
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域的颜色值为0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGFill（P2_1,DR6,SCr1）
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:6、从P2_1_SInfo信息中获取矩形区域（DR6）点的颜色值，应该为SCr1
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试传入填充矩形区域为左下角越界的fill操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P2_1+X2/X1+DR8+SCr1
//@EXPECTATION: 执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1、只对一种颜色值进行fill操作，可以根据测试覆盖率进行补充其它颜色值   
//@REMARK: 2、只对一种颜色模式进行fill操作，可以根据测试覆盖率补充其它颜色模式下的fill操作
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域的颜色值为0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGFill（P2_1,DR8,SCr1）
//@EXECUTIONFLOW:4、执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试传入填充矩形区域为底中边越界的fill操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P2_1+X2/X1+DR9+SCr1
//@EXPECTATION: 执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1、只对一种颜色值进行fill操作，可以根据测试覆盖率进行补充其它颜色值   
//@REMARK: 2、只对一种颜色模式进行fill操作，可以根据测试覆盖率补充其它颜色模式下的fill操作
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域的颜色值为0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGFill（P2_1,DR9,SCr1）
//@EXECUTIONFLOW:4、执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试传入填充矩形区域为右下角越界的fill操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P2_1+X2/X1+DR10+SCr1
//@EXPECTATION: 执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1、只对一种颜色值进行fill操作，可以根据测试覆盖率进行补充其它颜色值   
//@REMARK: 2、只对一种颜色模式进行fill操作，可以根据测试覆盖率补充其它颜色模式下的fill操作
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域的颜色值为0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGFill（P2_1,DR10,SCr1）
//@EXECUTIONFLOW:4、执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试传入填充矩形区域为右中边越界的fill操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P2_1+X2/X1+DR11+SCr1
//@EXPECTATION: 执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1、只对一种颜色值进行fill操作，可以根据测试覆盖率进行补充其它颜色值   
//@REMARK: 2、只对一种颜色模式进行fill操作，可以根据测试覆盖率补充其它颜色模式下的fill操作
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域的颜色值为0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGFill（P2_1,DR11,SCr1）
//@EXECUTIONFLOW:4、执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试传入填充矩形区域为右上角越界的fill操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P2_1+X2/X1+DR12+SCr1
//@EXPECTATION: 执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1、只对一种颜色值进行fill操作，可以根据测试覆盖率进行补充其它颜色值   
//@REMARK: 2、只对一种颜色模式进行fill操作，可以根据测试覆盖率补充其它颜色模式下的fill操作
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域的颜色值为0x88
//@EXECUTIONFLOW:3、调用CSUDIOSGFill（P2_1,DR12,SCr1）
//@EXECUTIONFLOW:4、执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试显存空间传入填充矩形区域为CSUDI_NULL下的fill操作
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: D+DR1+SCr1
//@EXPECTATION: 成功执行fill操作，填充区域为全屏，返回CSUDI_SUCCESS
//@REMARK: 1、只对一种颜色值进行fill操作，可以根据测试覆盖率进行补充其它颜色值   
//@REMARK: 2、只对一种颜色模式进行fill操作，可以根据测试覆盖率补充其它颜色模式下的fill操作
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄
//@EXECUTIONFLOW:2、调用CSUDIOSGFill（D,DR1,SCr1）
//@EXECUTIONFLOW:3、调用CSUDIOSGGetSurfaceInfo获取显存空间D的信息D_SInfo
//@EXECUTIONFLOW:4、从D_SInfo信息中获取任意一个点的颜色值，应该为SCr1
CSUDI_BOOL CSTC_OSG_IT_Fill_0012(void)
{   
	return  OSG_iTestFillDisplay(OSG_COLOR_RED_32BIT,EM_RECT_1,CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0013
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:测试显存空间传入填充矩形区域为全屏区域的fill操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、获取显存空间大小对DR2进行初始化
//@INPUT: D+DR2+SCr1
//@EXPECTATION: 成功执行fill操作，填充区域为全屏，返回CSUDI_SUCCESS
//@REMARK: 1、只对一种颜色值进行fill操作，可以根据测试覆盖率进行补充其它颜色值   
//@REMARK: 2、只对一种颜色模式进行fill操作，可以根据测试覆盖率补充其它颜色模式下的fill操作
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄
//@EXECUTIONFLOW:2、调用CSUDIOSGFill（D,DR2,SCr1）
//@EXECUTIONFLOW:3、调用CSUDIOSGGetSurfaceInfo获取显存空间D的信息D_SInfo
//@EXECUTIONFLOW:4、从D_SInfo信息中获取任意一个点的颜色值，应该为SCr1
CSUDI_BOOL CSTC_OSG_IT_Fill_0013(void)
{   
	return  OSG_iTestFillDisplay(OSG_COLOR_RED_32BIT,EM_RECT_2,CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0014
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:测试显存空间传入填充矩形区域为绘图平面中央的fill操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、获取显存空间大小对DR3进行初始化
//@INPUT: D+DR3+SCr1
//@EXPECTATION: 成功执行fill操作，填充区域为全屏，返回CSUDI_SUCCESS
//@REMARK: 1、只对一种颜色值进行fill操作，可以根据测试覆盖率进行补充其它颜色值   
//@REMARK: 2、只对一种颜色模式进行fill操作，可以根据测试覆盖率补充其它颜色模式下的fill操作
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄
//@EXECUTIONFLOW:2、初始化（填充）显存存空间区域的颜色值为0
//@EXECUTIONFLOW:3、调用CSUDIOSGFill（D,DR3,SCr1）
//@EXECUTIONFLOW:4、调用CSUDIOSGGetSurfaceInfo获取显存空间D的信息D_SInfo
//@EXECUTIONFLOW:5、从D_SInfo信息中获取任意一个点的颜色值，应该为SCr1
CSUDI_BOOL CSTC_OSG_IT_Fill_0014(void)
{   
	return  OSG_iTestFillDisplay(OSG_COLOR_RED_32BIT,EM_RECT_3,CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0015
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:测试显存空间传入填充矩形区域为非法值（0，0）的fill操作
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: D+DR4+SCr1
//@EXPECTATION: 执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1、只对一种颜色值进行fill操作，可以根据测试覆盖率进行补充其它颜色值   
//@REMARK: 2、只对一种颜色模式进行fill操作，可以根据测试覆盖率补充其它颜色模式下的fill操作
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄
//@EXECUTIONFLOW:2、调用CSUDIOSGFill（D,DR4,SCr1）
//@EXECUTIONFLOW:3、执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_Fill_0015(void)
{   
	return  OSG_iTestFillDisplay(OSG_COLOR_RED_32BIT,EM_RECT_4,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0016
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:测试显存空间传入填充矩形区域为非法值（-1，-1）的fill操作
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: D+DR5+SCr1
//@EXPECTATION: 执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1、只对一种颜色值进行fill操作，可以根据测试覆盖率进行补充其它颜色值   
//@REMARK: 2、只对一种颜色模式进行fill操作，可以根据测试覆盖率补充其它颜色模式下的fill操作
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄
//@EXECUTIONFLOW:3、调用CSUDIOSGFill（D,DR4,SCr1）
//@EXECUTIONFLOW:4、执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_Fill_0016(void)
{   
	return  OSG_iTestFillDisplay(OSG_COLOR_RED_32BIT,EM_RECT_5,CSUDI_FALSE);
}


//@TESTCASEID:CSTC_OSG_IT_Fill_0017
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:测试显存空间传入填充矩形区域为一个像素的fill操作
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: D+DR6+SCr1
//@EXPECTATION: 成功执行fill操作，返回CSUDI_SUCCESS
//@REMARK: 1、只对一种颜色值进行fill操作，可以根据测试覆盖率进行补充其它颜色值   
//@REMARK: 2、只对一种颜色模式进行fill操作，可以根据测试覆盖率补充其它颜色模式下的fill操作
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄
//@EXECUTIONFLOW:2、初始化（填充）显存存空间区域的颜色值为0
//@EXECUTIONFLOW:3、调用CSUDIOSGFill（D,DR3,SCr1）
//@EXECUTIONFLOW:4、调用CSUDIOSGGetSurfaceInfo获取显存空间D的信息D_SInfo
//@EXECUTIONFLOW:5、从D_SInfo信息中获取像素DR6的颜色值，应该为SCr1
CSUDI_BOOL CSTC_OSG_IT_Fill_0017(void)
{   
	
	  return OSG_iTestFillDisplay(OSG_COLOR_RED_32BIT,EM_RECT_6,CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0018
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:测试显存空间传入填充矩形区域为左下角越界的fill操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、获取显存空间大小对DR8进行初始化
//@INPUT: D+DR8+SCr1
//@EXPECTATION: 执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1、只对一种颜色值进行fill操作，可以根据测试覆盖率进行补充其它颜色值   
//@REMARK: 2、只对一种颜色模式进行fill操作，可以根据测试覆盖率补充其它颜色模式下的fill操作
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄
//@EXECUTIONFLOW:2、调用CSUDIOSGFill（D,DR8,SCr1）
//@EXECUTIONFLOW:3、执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_Fill_0018(void)
{   
	return  OSG_iTestFillDisplay(OSG_COLOR_RED_32BIT,EM_RECT_8,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0019
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:测试显存空间传入填充矩形区域为底中边越界的fill操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、获取显存空间大小对DR9进行初始化
//@INPUT: D+DR9+SCr1
//@EXPECTATION: 执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1、只对一种颜色值进行fill操作，可以根据测试覆盖率进行补充其它颜色值   
//@REMARK: 2、只对一种颜色模式进行fill操作，可以根据测试覆盖率补充其它颜色模式下的fill操作
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄
//@EXECUTIONFLOW:2、调用CSUDIOSGFill（D,DR9,SCr1）
//@EXECUTIONFLOW:3、执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_Fill_0019(void)
{   
	return  OSG_iTestFillDisplay(OSG_COLOR_RED_32BIT,EM_RECT_9,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0020
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:测试显存空间传入填充矩形区域为右下角越界的fill操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、获取显存空间大小对DR10进行初始化
//@INPUT: D+DR10+SCr1
//@EXPECTATION: 执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1、只对一种颜色值进行fill操作，可以根据测试覆盖率进行补充其它颜色值   
//@REMARK: 2、只对一种颜色模式进行fill操作，可以根据测试覆盖率补充其它颜色模式下的fill操作
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄
//@EXECUTIONFLOW:2、调用CSUDIOSGFill（D,DR10,SCr1）
//@EXECUTIONFLOW:3、执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_Fill_0020(void)
{   
	return  OSG_iTestFillDisplay(OSG_COLOR_RED_32BIT,EM_RECT_10,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0021
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:测试显存空间传入填充矩形区域为右中边越界的fill操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、获取显存空间大小对DR11进行初始化
//@INPUT: D+DR11+SCr1
//@EXPECTATION: 执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1、只对一种颜色值进行fill操作，可以根据测试覆盖率进行补充其它颜色值   
//@REMARK: 2、只对一种颜色模式进行fill操作，可以根据测试覆盖率补充其它颜色模式下的fill操作
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄
//@EXECUTIONFLOW:2、调用CSUDIOSGFill（D,DR11,SCr1）
//@EXECUTIONFLOW:3、执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_Fill_0021(void)
{   
	return  OSG_iTestFillDisplay(OSG_COLOR_RED_32BIT,EM_RECT_11,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0022
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:测试显存空间传入填充矩形区域为右上角越界的fill操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、获取显存空间大小对DR12进行初始化
//@INPUT: D+DR12+SCr1+SCr1
//@EXPECTATION: 执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1、只对一种颜色值进行fill操作，可以根据测试覆盖率进行补充其它颜色值   
//@REMARK: 2、只对一种颜色模式进行fill操作，可以根据测试覆盖率补充其它颜色模式下的fill操作
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄
//@EXECUTIONFLOW:2、调用CSUDIOSGFill（D,DR12,SCr1）
//@EXECUTIONFLOW:3、执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_Fill_0022(void)
{   
	return  OSG_iTestFillDisplay(OSG_COLOR_RED_32BIT,EM_RECT_12,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0023
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:测试传入源绘图平面和填充平面都为CSUDI_NULL的fill操作
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: P2_3+DR1+SCr1
//@EXPECTATION: 执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1、只对一种颜色值进行fill操作，可以根据测试覆盖率进行补充其它颜色值   
//@REMARK: 2、只对一种颜色模式进行fill操作，可以根据测试覆盖率补充其它颜色模式下的fill操作
//@EXECUTIONFLOW:1、调用CSUDIOSGFill（P2_3,DR1,SCr1）
//@EXECUTIONFLOW:2、执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
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
//@DESCRIPTION:  测试传入填充矩形区域为全屏的fill操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P2_1+X2/X1+DR1+SCr1
//@EXPECTATION: 每次fill操作都返回CSUDI_SUCCESS，屏幕能显示正确红色
//@REMARK: 只对一种颜色模式进行fill操作，可以根据测试覆盖率补充其它非ARGB8888模式的颜色模式
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域的颜色值为0
//@EXECUTIONFLOW:3、调用CSUDIOSGFill（P2_1,DR1,SCr1）
//@EXECUTIONFLOW:4、调用CSUDIOSGGetSurfaceInfo获取目的缓存空间P2_1的信息P2_1_SInfo
//@EXECUTIONFLOW:5、从P2_1_SInfo信息中获取任意一个点的颜色值，应该为SCr1
//@EXECUTIONFLOW:6、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试传入填充矩形区域为左上角越界的fill操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P2_1+X2、X1+DR14+SCr1
//@EXPECTATION: 执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1、只对一种颜色值进行fill操作，可以根据测试覆盖率进行补充其它颜色值   
//@REMARK: 2、只对一种颜色模式进行fill操作，可以根据测试覆盖率补充其它颜色模式下的fill操作
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域的颜色值为0
//@EXECUTIONFLOW:3、调用CSUDIOSGFill（P2_1,DR14,SCr1）
//@EXECUTIONFLOW:4、执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试传入填充矩形区域为左中边越界的fill操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P2_1+X2/X1+DR15+SCr1
//@EXPECTATION: 执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1、只对一种颜色值进行fill操作，可以根据测试覆盖率进行补充其它颜色值   
//@REMARK: 2、只对一种颜色模式进行fill操作，可以根据测试覆盖率补充其它颜色模式下的fill操作
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、初始化（填充）源缓存空间区域的颜色值为0
//@EXECUTIONFLOW:3、调用CSUDIOSGFill（P2_1,DR15,SCr1）
//@EXECUTIONFLOW:4、执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试传入填充矩形区域为上中边越界的fill操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P2_1+X2/X1+DR16+SCr1
//@EXPECTATION: 执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1、只对一种颜色值进行fill操作，可以根据测试覆盖率进行补充其它颜色值   
//@REMARK: 2、只对一种颜色模式进行fill操作，可以根据测试覆盖率补充其它颜色模式下的fill操作
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P2_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGFill（P2_1,DR16,SCr1）
//@EXECUTIONFLOW:3、执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4、调用CSUDIOSGDestroySurface销毁绘图平面
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
//@DESCRIPTION:测试显存空间传入填充矩形区域为左上角越界的fill操作
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: D+DR14+SCr1
//@EXPECTATION: 执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1、只对一种颜色值进行fill操作，可以根据测试覆盖率进行补充其它颜色值   
//@REMARK: 2、只对一种颜色模式进行fill操作，可以根据测试覆盖率补充其它颜色模式下的fill操作
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄
//@EXECUTIONFLOW:2、调用CSUDIOSGFill（D,DR14,SCr1）
//@EXECUTIONFLOW:3、执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_Fill_0028(void)
{   
	return  OSG_iTestFillDisplay(OSG_COLOR_RED_32BIT,EM_RECT_14,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0029
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:测试显存空间传入填充矩形区域为左中边越界的fill操作
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: D+DR15+SCr1
//@EXPECTATION: 执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1、只对一种颜色值进行fill操作，可以根据测试覆盖率进行补充其它颜色值   
//@REMARK: 2、只对一种颜色模式进行fill操作，可以根据测试覆盖率补充其它颜色模式下的fill操作
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄
//@EXECUTIONFLOW:2、调用CSUDIOSGFill（D,DR15,SCr1）
//@EXECUTIONFLOW:3、执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_Fill_0029(void)
{   
	return  OSG_iTestFillDisplay(OSG_COLOR_RED_32BIT,EM_RECT_15,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0030
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:测试显存空间传入填充矩形区域为上中边越界的fill操作
//@PRECONDITION:平台UDI已经成功初始化
//@INPUT: D+DR16+SCr1
//@EXPECTATION: 执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 1、只对一种颜色值进行fill操作，可以根据测试覆盖率进行补充其它颜色值   
//@REMARK: 2、只对一种颜色模式进行fill操作，可以根据测试覆盖率补充其它颜色模式下的fill操作
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄
//@EXECUTIONFLOW:2、调用CSUDIOSGFill（D,DR16,SCr1）
//@EXECUTIONFLOW:3、执行fill操作失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_Fill_0030(void)
{   
	return  OSG_iTestFillDisplay(OSG_COLOR_RED_32BIT,EM_RECT_16,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0031
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:测试对细长或者细高区域颜色填充
//@PRECONDITION:平台UDI已经成功初始化
//@INPUT: 4000*50矩形框,50*4000矩形框
//@EXPECTATION: 执行fill成功且颜色填充正确
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建4000*50 缓存空间，期望返回成功
//@EXECUTIONFLOW:2、调用CSUDIOSGFill填充{0,0,4000,50}区域为颜色红色，期望返回成功
//@EXECUTIONFLOW:3、校验{0,0,4000,50}区域颜色期望为红色
//@EXECUTIONFLOW:4、调用CSUDIOSGCreateSurface创建50*4000 缓存空间，期望返回成功
//@EXECUTIONFLOW:5、调用CSUDIOSGFill填充{0,0,50,4000}为颜色红色，期望返回成功
//@EXECUTIONFLOW:6、校验{0,0,4000,50}区域颜色期望为红色
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁创建的surface
CSUDI_BOOL CSTC_OSG_IT_Fill_0031(void)
{
	CSUDIOSGRect_S sRect;
	CSUDI_BOOL bRet = CSUDI_FALSE;

	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;

	sRect.m_nX = sRect.m_nY = 0;
	sRect.m_nWidth  = 4000; 
	sRect.m_nHeight = 50;
	
	bRet  =  OSG_Blit_TestFillEx(ePixelFormat, OSG_COLOR_RED_32BIT, 4000, 50, &sRect);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == bRet),"测试4000*50的矩形失败!");
	
	sRect.m_nWidth  = 50; 
	sRect.m_nHeight = 4000;	
	
	bRet =  OSG_Blit_TestFillEx(ePixelFormat, OSG_COLOR_RED_32BIT, 50, 4000, &sRect);	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == bRet),"测试50*4000的矩形失败!");
	
	CSTK_FATAL_POINT;

	return bRet;
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0032
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:测试连续两次填充，且填充区域有交叉
//@PRECONDITION:平台UDI已经成功初始化
//@EXPECTATION:两次填充完成后，交叉区域的颜色应该为最后一次填充的颜色
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建720*576 缓存空间，期望返回成功
//@EXECUTIONFLOW:2、调用CSUDIOSGFill填充{0,0,720,576}区域为颜色SCr1，期望返回成功
//@EXECUTIONFLOW:3、校验整个区域的颜色,期望区域{0,0,720,576}为SCr1
//@EXECUTIONFLOW:4、调用CSUDIOSGFill填充{720/4,576/4,720/2,576/2}为颜色SCr2，期望返回成功
//@EXECUTIONFLOW:5、校验整个区域的颜色,期望区域{720/4,576/4,720/2,576/2}为SCr2
//@EXECUTIONFLOW:6、调用CSUDIOSGDestroySurface销毁创建的surface
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
				"没有找到任何支持的满足要求的PixelFormat"
			);

	nRet = CSUDIOSGCreateSurface(ePixelFormat,720,576,&hSurface);
	CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hSurface != CSUDI_NULL),	"Step 1 Create Surface Failure !");
			
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo)),	"Get Surface info failure !");	
	CSTK_ASSERT_TRUE_FATAL(((sSurfaceInfo.m_pvSurfaceMem != CSUDI_NULL)&&(sSurfaceInfo.m_nMemSize>0)),"Get Surface info failure !");
	
	memset(sSurfaceInfo.m_pvSurfaceMem, 0x88, sSurfaceInfo.m_nMemSize);

	//************************检查全屏区域红色*****************************************//
	ucTmpColor = OSG_iGetColorWithPixelMode(ucFillColorRed,ePixelFormat);

	nRet = CSUDIOSGFill(hSurface, NULL, ucFillColorRed);

	CSTK_ASSERT_TRUE_FATAL(((nRet == CSUDI_SUCCESS)),	"Step 2 fill should be success !");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync())," Sync Failure !"	);	

	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == OSG_iCheckExistColor(hSurface,ucTmpColor,NULL,ePixelFormat)),	"step 3 Check Surface color Failure !");

	//*************************检查1/2居中区域蓝色****************************************//
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
//@DESCRIPTION:测试CSUDIOSGFill对cache的同步
//@PRECONDITION:平台UDI已经成功初始化
//@EXPECTATION:fill后在surface的绘图区域应该保持内存和cache的一致性
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间，期望返回成功
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取surface的信息
//@EXECUTIONFLOW:3、将surface的所有像素内存空间memset成0x88
//@EXECUTIONFLOW:4、调用CSUDIOSGFill填充整个缓存空间为颜色0xFFFFFFFF，期望返回成功
//@EXECUTIONFLOW:5、校验每个像素的值,期望都为0xFFFFFFFF
//@EXECUTIONFLOW:6、重复步骤3到5一百次
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁创建的surface
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
				"没有找到任何支持的满足要求的PixelFormat"
			);
	
	nRet = CSUDIOSGCreateSurface(ePixelFormat,720,576,&hSurface);
	CSTK_ASSERT_TRUE_FATAL((nRet == CSUDI_SUCCESS && hSurface != CSUDI_NULL),	"Step 1 Create Surface Failure !");
			
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo)),	"step 2 Get Surface info failure !");	
	CSTK_ASSERT_TRUE_FATAL(((sSurfaceInfo.m_pvSurfaceMem != CSUDI_NULL)&&(sSurfaceInfo.m_nMemSize>0)),"step 2 Get Surface mem info failure !");


	//************************检查全屏区域红色*****************************************//
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
//@DESCRIPTION:测试CSUDIOSGFill对高度为1、宽度为1的矩形的填充
//@PRECONDITION:平台UDI已经成功初始化
//@EXPECTATION:执行fill成功且颜色填充正确
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建1*4000 缓存空间，期望返回成功
//@EXECUTIONFLOW:2、调用CSUDIOSGFill填充{0,0,4000,1}区域为颜色SCr1，期望返回成功
//@EXECUTIONFLOW:3、校验{0,0,4000,1}区域颜色期望为SCr1
//@EXECUTIONFLOW:4、调用CSUDIOSGCreateSurface创建1*4000 缓存空间，期望返回成功
//@EXECUTIONFLOW:5、调用CSUDIOSGFill填充{0,0,1,4000}区域为颜色SCr2，期望返回成功
//@EXECUTIONFLOW:6、校验{0,0,4000,1}区域颜色期望为SCr2
//@EXECUTIONFLOW:7、调用CSUDIOSGDestroySurface销毁创建的surface
CSUDI_BOOL CSTC_OSG_IT_Fill_0034(void)
{
	CSUDIOSGRect_S sRect;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;

	sRect.m_nX = sRect.m_nY = 0;
	
	sRect.m_nWidth  = 1; 
	sRect.m_nHeight = 4000;
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == OSG_Blit_TestFillEx(ePixelFormat,OSG_COLOR_RED_32BIT,1, 4000, &sRect)),"测试1* 4000的矩形失败!");
	
	sRect.m_nWidth  = 4000; 
	sRect.m_nHeight = 1;
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == OSG_Blit_TestFillEx(ePixelFormat,OSG_COLOR_BLUE_32BIT,4000, 1, &sRect)),"测试4000* 1的矩形失败!");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
	
}

//@TESTCASEID:CSTC_OSG_IT_Fill_0035
//@CASEGROUP: CSUDIOSGFill
//@DESCRIPTION:测试colorkey alpha属性是否对CSUDIOSGFill产生不利影响
//@PRECONDITION:平台UDI已经成功初始化
//@EXPECTATION:执行fill成功且颜色填充正确
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建720*576 缓存空间，期望返回成功
//@EXECUTIONFLOW:2、调用CSUDIOSGSetSurfaceInfo接口设置绘图平面信息成m_nColorKeyFlag = 1,m_nAlphaFlag=1,m_uColorkey=0xFFFF0000,m_uAlpha=0x55,期望返回成功
//@EXECUTIONFLOW:3、调用CSUDIOSGFill填充{0,0,720,576}区域为颜色0xFFFF0000，期望返回成功
//@EXECUTIONFLOW:4、校验{0,0,720,576}区域颜色期望为0xFFFF0000
//@EXECUTIONFLOW:5、调用CSUDIOSGDestroySurface销毁创建的surface
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
				"没有找到任何支持的满足要求的PixelFormat"
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

	//************************检查全屏区域红色*****************************************//
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
//@DESCRIPTION:测试CSUDIOSGFill对高度为1、宽度为1的矩形的填充
//@PRECONDITION:平台UDI已经成功初始化
//@EXPECTATION:执行fill成功且颜色填充正确
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建200*200 缓存空间，颜色模式为EM_UDIOSG_PIXEL_FORMAT_ARGB_8888期望返回成功
//@EXECUTIONFLOW:2、调用CSUDIOSGFill，每次填充一个像素，指定颜色为红色，循环填充直到{0,0,200,200}区域填充完毕，期望返回成功
//@EXECUTIONFLOW:3、校验{0,0,200,200}区域颜色期望为红色
//@EXECUTIONFLOW:4、调用CSUDIOSGDestroySurface销毁创建的surface
CSUDI_BOOL CSTC_OSG_IT_Fill_0036(void)
{
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;

	bRet = OSG_Blit_TestFillOnePixel(ePixelFormat, OSG_COLOR_RED_32BIT);
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == bRet),"测试一个像素的填充失败!");

	CSTK_FATAL_POINT;	
	
	return bRet;
}

//@CASEGROUP: CSUDIOSGSync
//@DESCRIPTION:测试(缓存空间和显存之间)执行blit操作之后连续调用多次Sync的情况(不带alpha模式)
//@PRECONDITION:平台UDI已经成功初始化
//@INPUT:  P1_1+D+X2+SR1+DR1+M1+SCr1+DCr4
//@EXPECTATION: 1、两次或者多次sync操作之后，不会改变blit的效果
//@EXPECTATION: 2、第二次或者之后的sync时间应该在5秒之内
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄D，期望成功
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取显存空间D的信息，大小和颜色模式
//@EXECUTIONFLOW:3、根据步骤(2)获取的信息调用CSUDIOSGCreateSurface创建缓存空间P1_1
//@EXECUTIONFLOW:4、调用CSUDIOSGFill填充显存空间D颜色值为DCr4，期望成功
//@EXECUTIONFLOW:5、调用CSUDISync完成操作
//@EXECUTIONFLOW:6、调用CSUDIOSGFill填充绘图平面P1_1颜色值为SCr1，期望成功
//@EXECUTIONFLOW:7、调用CSUDISync完成操作
//@EXECUTIONFLOW:8、调用CSUDIOSGBlit（D,DR1,P1_1,SR1,M1）
//@EXECUTIONFLOW:9、调用CSUDISync完成blit操作
//@EXECUTIONFLOW:10、调用CSUDIOSGGetSurfaceInfo获取D显存空间的信息
//@EXECUTIONFLOW:11、期望显存空间内的任意点颜色值为SCr1
//@EXECUTIONFLOW:12、再次调用CSUDISync，期望Sync时间在5秒之内
//@EXECUTIONFLOW:13、调用CSUDIOSGGetSurfaceInfo获取D显存空间的信息
//@EXECUTIONFLOW:14、期望显存空间内的任意点颜色值仍为SCr1
//@EXECUTIONFLOW:16、重复步骤4到14 10次，期望每次都成功。
//@EXECUTIONFLOW:15、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Sync_0001(void)
{   
	return  OSG_iTestSyncWithDisplay(CSUDI_FALSE,1);
}

//@CASEGROUP: CSUDIOSGSync
//@DESCRIPTION:测试(缓存空间之间)执行blit操作之后连续调用多次Sync的情况(不带alpha模式)
//@PRECONDITION:平台UDI已经成功初始化
//@PRECONDITION:系统支持颜色模式X2
//@INPUT:  P1_1+P2_1+X2+SR1+DR1+M1+SCr1+DCr4
//@EXPECTATION: 1、两次或者多次sync操作之后，不会改变blit的效果
//@EXPECTATION: 2、第二次或者之后的sync时间应该在5秒之内
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1,颜色模式为X2或X1，期望成功
//@EXECUTIONFLOW:2、调用CSUDIOSGCreateSurface创建缓存空间P2_1,颜色模式为X2或X1，期望成功
//@EXECUTIONFLOW:3、调用CSUDIOSGFill填充绘图屏幕P2_1颜色值为DCr4，期望成功
//@EXECUTIONFLOW:4、调用CSUDISync完成操作
//@EXECUTIONFLOW:5、调用CSUDIOSGFill填充绘图平面P1_1颜色值为SCr1，期望成功
//@EXECUTIONFLOW:6、调用CSUDISync完成操作
//@EXECUTIONFLOW:7、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M1）
//@EXECUTIONFLOW:8、调用CSUDISync完成操作
//@EXECUTIONFLOW:9、调用CSUDIOSGGetSurfaceInfo获取P2_1空间的信息
//@EXECUTIONFLOW:10、期望P2_1空间内的任意点颜色值为SCr1
//@EXECUTIONFLOW:11、再次调用CSUDISync，期望Sync时间在5秒之内
//@EXECUTIONFLOW:12、调用CSUDIOSGGetSurfaceInfo获取P2_1空间的信息
//@EXECUTIONFLOW:13、期望P2_1空间内的任意点颜色值仍为SCr1
//@EXECUTIONFLOW:14、重复步骤4到14 10次，期望每次都成功。
//@EXECUTIONFLOW:16、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Sync_0002(void)
{   
	return  OSG_iTestSync(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,CSUDI_FALSE,1);
}
//@CASEGROUP: CSUDIOSGSync
//@DESCRIPTION:测试(缓存空间之间)执行blit操作之后连续调用多次Sync的情况(带alpha模式)
//@PRECONDITION:平台UDI已经成功初始化
//@INPUT:  P1_1+P2_1+X2+SR1+DR1+M17+SCr1+DCr4+SA1+DA1
//@EXPECTATION: 1、两次或者多次sync操作之后，不会改变blit的效果
//@EXPECTATION: 2、第二次或者之后的sync时间应该在5秒之内
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1,颜色模式为X2或X1，期望成功
//@EXECUTIONFLOW:2、调用CSUDIOSGCreateSurface创建缓存空间P2_1,颜色模式为X2或X1，期望成功
//@EXECUTIONFLOW:3、设置缓存空间P1_1的全局alpha值为SA1,P2_1的全局alpha值为DA1
//@EXECUTIONFLOW:4、调用CSUDIOSGFill填充P2_1颜色值为DCr4，期望成功
//@EXECUTIONFLOW:5、调用CSUDISync完成操作
//@EXECUTIONFLOW:6、调用CSUDIOSGFill填充绘图平面P1_1颜色值为SCr1，期望成功
//@EXECUTIONFLOW:7、调用CSUDISync完成操作
//@EXECUTIONFLOW:8、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M17）
//@EXECUTIONFLOW:9、调用CSUDISync完成blit操作
//@EXECUTIONFLOW:10、调用CSUDIOSGGetSurfaceInfo获取P2_1空间的信息
//@EXECUTIONFLOW:11、期望空间P2_1中获取任意一个点的颜色值，符合公式(1)(2)(3)(4)
//@EXECUTIONFLOW:12、再次调用CSUDISync，期望Sync时间在5秒之内
//@EXECUTIONFLOW:13、调用CSUDIOSGGetSurfaceInfo获取D显存空间的信息
//@EXECUTIONFLOW:14、期望空间P2_1中获取任意一个点的颜色值，仍符合公式(1)(2)(3)(4)
//@EXECUTIONFLOW:16、重复步骤4到14 10次，期望每次都成功。
//@EXECUTIONFLOW:15、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_Sync_0003(void)
{   
	return  OSG_iTestSync(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,CSUDI_TRUE,1);
}
//**********************************************************************************//
//******************************Begin DecodeImage Test *****************************//
//**********************************************************************************//
//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: 测试传入接口参数为非法值
//@PRECONDITION: 1、平台已成功初始化
//@INPUT:1、传入图片的类型字符串为CSUDI_NULL
//@INPUT:2、图片数据内存地址为CSUDI_NULL
//@INPUT:3、图片数据大小为0
//@EXPECTATION:传入每个非法值都返回CSUDIOSG_ERROR_BAD_PARAMETER
//@REMARK: 
//@EXECUTIONFLOW:1、调用CSUDIOSGDecodeImage(CSUDI_NULL,CSUDI_NULL,0,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,CSUDI_NULL)，结果返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage(CSUDI_NULL,CSUDI_NULL,0,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,合法指针),结果返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:3、调用CSUDIOSGDecodeImage(CSUDI_NULL,CSUDI_NULL,10M_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,合法指针,结果返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4、调用CSUDIOSGDecodeImage(CSUDI_NULL,合法图片数据地址,图片数据长度,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,合法指针),结果返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5、调用CSUDIOSGDecodeImage("IFRAME",CSUDI_NULL,10,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,合法指针),结果返回CSUDIOSG_ERROR_BAD_PARAMETE
//@EXECUTIONFLOW:6、调用CSUDIOSGDecodeImage("BMP",CSUDI_NULL,10,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,合法指针),结果返回CSUDIOSG_ERROR_BAD_PARAMETE
//@EXECUTIONFLOW:7、调用CSUDIOSGDecodeImage("JPG",CSUDI_NULL,10,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,合法指针),结果返回CSUDIOSG_ERROR_BAD_PARAMETE
//@EXECUTIONFLOW:8、调用CSUDIOSGDecodeImage("PNG",CSUDI_NULL,10,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,合法指针),结果返回CSUDIOSG_ERROR_BAD_PARAMETE
//@EXECUTIONFLOW:9、调用CSUDIOSGDecodeImage("PNG",合法图片数据地址,图片数据长度,EM_UDIOSG_SCALE_OPTIMUM,CSUDI_NULL,合法指针),结果返回CSUDIOSG_ERROR_BAD_PARAMETE
//@EXECUTIONFLOW:10、调用CSUDIOSGDecodeImage("PNG",合法图片数据地址,图片数据长度,EM_UDIOSG_SCALE_FIXSIZE,CSUDI_NULL,合法指针),结果返回CSUDIOSG_ERROR_BAD_PARAMETE
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
//@DESCRIPTION: 测试解码图片类型为I帧720*576格式时的解码,输出模式为EM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、有空闲的视频解码器
//@PRECONDITION: 3、平台支持I帧类型的图片解码
//@INPUT:1、传入图片的类型字符串为"IFRAME"
//@INPUT:2、图片UDI2OSG_720_576.mpg的数据地址
//@INPUT:3、图片UDI2OSG_720_576.mpg的长度
//@EXPECTATION: 1、能够成功对图片进行解码，返回CSUDI_SUCCESS；
//@EXPECTATION: 2、解码得到的surface数据能够成功blit到显存输出。
//@REMARK: 
//@EXECUTIONFLOW:1、获取UDI2OSG_720_576.mpg图片数据的内存地址和长度
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDI_SUCCESS，解码得到的phSurface应该不为CSUDI_NULL
//@EXECUTIONFLOW:3、调用CSUDIOSGGetDisplaySurface接口获取显存空间hDisSurface句柄
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit接口将解码得到的图片信息blit到显存空间hDisSurface上，blit的矩形区域为CSUDI_NULL。
//@EXECUTIONFLOW:5、调用CSUDIOSGSync完成图片的blit操作，成功返回CSUDI_SUCCESS。
//@EXECUTIONFLOW:6、第5步完成之后，应该在屏幕上看到UDI2OSG_720_576.mpg图片的效果，如果看不到，则测试用例失败。
//@EXECUTIONFLOW:7、释放掉UDI2OSG_720_576.mpg占用的内存。
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0002(void)
{  
	return OSG_iTestDecImage(EM_OSG_DECIMAGE_IFRAME,EM_IFRAME_720_576);
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: 测试解码图片类型为I帧1280*720格式时的解码,输出模式为EM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、有空闲的视频解码器
//@PRECONDITION: 3、平台支持I帧类型的图片解码
//@INPUT:1、传入图片的类型字符串为"IFRAME"
//@INPUT:2、图片UDI2OSG_1280_720.mpg的数据地址
//@INPUT:3、图片UDI2OSG_1280_720.mpg的长度
//@EXPECTATION: 1、能够成功对图片进行解码，返回CSUDI_SUCCESS；
//@EXPECTATION: 2、解码得到的surface数据能够成功blit到显存输出。
//@REMARK: 
//@EXECUTIONFLOW:1、获取UDI2OSG_1280_720.mpg图片数据的内存地址和长度
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDI_SUCCESS，解码得到的phSurface应该不为CSUDI_NULL
//@EXECUTIONFLOW:3、调用CSUDIOSGGetDisplaySurface接口获取显存空间hDisSurface句柄
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit接口将解码得到的图片信息blit到显存空间hDisSurface上，bilt的矩形区域为CSUDI_NULL。
//@EXECUTIONFLOW:5、调用CSUDIOSGSync完成图片的blit操作，成功返回CSUDI_SUCCESS。
//@EXECUTIONFLOW:6、第5步完成之后，应该在屏幕上看到UDI2OSG_1280_720.mpg图片的效果，如果看不到，则测试用例失败。
//@EXECUTIONFLOW:7、释放掉UDI2OSG_1280_720.mpg占用的内存。
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0003(void)
{  
	return OSG_iTestDecImage(EM_OSG_DECIMAGE_IFRAME,EM_IFRAME_1280_720);
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: 测试解码图片类型为I帧1920*1080格式时的解码,输出模式为EM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、有空闲的视频解码器
//@PRECONDITION: 3、平台支持I帧类型的图片解码
//@INPUT:1、传入图片的类型字符串为"IFRAME"
//@INPUT:2、图片UDI2OSG_1920_1080.mpg的数据地址
//@INPUT:3、图片UDI2OSG_1920_1080.mpg的长度
//@EXPECTATION: 1、能够成功对图片进行解码，返回CSUDI_SUCCESS；
//@EXPECTATION: 2、解码得到的surface数据能够成功blit到显存输出。
//@REMARK: 
//@EXECUTIONFLOW:1、获取UDI2OSG_1920_1080.mpg图片数据的内存地址和长度
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDI_SUCCESS，解码得到的phSurface应该不为CSUDI_NULL
//@EXECUTIONFLOW:3、调用CSUDIOSGGetDisplaySurface接口获取显存空间hDisSurface句柄
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit接口将解码得到的图片信息blit到显存空间hDisSurface上，bilt的矩形区域为CSUDI_NULL。
//@EXECUTIONFLOW:5、调用CSUDIOSGSync完成图片的blit操作，成功返回CSUDI_SUCCESS。
//@EXECUTIONFLOW:6、第5步完成之后，应该在屏幕上看到UDI2OSG_1920_1080.mpg图片的效果，如果看不到，则测试用例失败。
//@EXECUTIONFLOW:7、释放掉UDI2OSG_1920_1080.mpg占用的内存。
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0004(void)
{  
	return OSG_iTestDecImage(EM_OSG_DECIMAGE_IFRAME,EM_IFRAME_1920_1080);
}


//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: 测试解码图片类型为bmp720*576格式时的解码,输出模式为EM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、有空闲的视频解码器
//@PRECONDITION: 3、平台支持bmp类型的图片解码
//@INPUT:1、传入图片的类型字符串为"BMP"
//@INPUT:2、图片UDI2OSG_720_576.bmp的数据地址
//@INPUT:3、图片UDI2OSG_720_576.bmp的长度
//@EXPECTATION: 1、能够成功对图片进行解码，返回CSUDI_SUCCESS；
//@EXPECTATION: 2、解码得到的surface数据能够成功blit到显存输出。
//@REMARK: 
//@EXECUTIONFLOW:1、获取UDI2OSG_720_576.bmp图片数据的内存地址和长度
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDI_SUCCESS，解码得到的phSurface应该不为CSUDI_NULL
//@EXECUTIONFLOW:3、调用CSUDIOSGGetDisplaySurface接口获取显存空间hDisSurface句柄
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit接口将解码得到的图片信息blit到显存空间hDisSurface上，bilt的矩形区域为CSUDI_NULL。
//@EXECUTIONFLOW:5、调用CSUDIOSGSync完成图片的blit操作，成功返回CSUDI_SUCCESS。
//@EXECUTIONFLOW:6、第5步完成之后，应该在屏幕上看到UDI2OSG_720_576.bmp图片的效果，如果看不到，则测试用例失败。
//@EXECUTIONFLOW:7、释放掉UDI2OSG_720_576.bmp占用的内存。
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0005(void)
{  
	return OSG_iTestDecImage(EM_OSG_DECIMAGE_BMP,EM_BMP_720_576);
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: 测试解码图片类型为bmp1280*720格式时的解码,输出模式为EM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、有空闲的视频解码器
//@PRECONDITION: 3、平台支持bmp类型的图片解码
//@INPUT:1、传入图片的类型字符串为"BMP"
//@INPUT:2、图片UDI2OSG_1280_720.bmp的数据地址
//@INPUT:3、图片UDI2OSG_1280_720.bmp的长度
//@EXPECTATION: 1、能够成功对图片进行解码，返回CSUDI_SUCCESS；
//@EXPECTATION: 2、解码得到的surface数据能够成功blit到显存输出。
//@REMARK: 
//@EXECUTIONFLOW:1、获取UDI2OSG_1280_720.bmp图片数据的内存地址和长度
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDI_SUCCESS，解码得到的phSurface应该不为CSUDI_NULL
//@EXECUTIONFLOW:3、调用CSUDIOSGGetDisplaySurface接口获取显存空间hDisSurface句柄
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit接口将解码得到的图片信息blit到显存空间hDisSurface上，bilt的矩形区域为CSUDI_NULL。
//@EXECUTIONFLOW:5、调用CSUDIOSGSync完成图片的blit操作，成功返回CSUDI_SUCCESS。
//@EXECUTIONFLOW:6、第5步完成之后，应该在屏幕上看到UDI2OSG_1280_720.bmp图片的效果，如果看不到，则测试用例失败。
//@EXECUTIONFLOW:7、释放掉UDI2OSG_1280_720.bmp占用的内存。
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0006(void)
{ 
	return OSG_iTestDecImage(EM_OSG_DECIMAGE_BMP,EM_BMP_1280_720);
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: 测试解码图片类型为bmp1920*1080格式时的解码,输出模式为EM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、有空闲的视频解码器
//@PRECONDITION: 3、平台支持bmp类型的图片解码
//@INPUT:1、传入图片的类型字符串为"BMP"
//@INPUT:2、图片UDI2OSG_1920_1080.bmp的数据地址
//@INPUT:3、图片UDI2OSG_1920_1080.bmp的长度
//@EXPECTATION: 1、能够成功对图片进行解码，返回CSUDI_SUCCESS；
//@EXPECTATION: 2、解码得到的surface数据能够成功blit到显存输出。
//@REMARK: 
//@EXECUTIONFLOW:1、获取UDI2OSG_1920_1080.bmp图片数据的内存地址和长度
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDI_SUCCESS，解码得到的phSurface应该不为CSUDI_NULL
//@EXECUTIONFLOW:3、调用CSUDIOSGGetDisplaySurface接口获取显存空间hDisSurface句柄
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit接口将解码得到的图片信息blit到显存空间hDisSurface上，bilt的矩形区域为CSUDI_NULL。
//@EXECUTIONFLOW:5、调用CSUDIOSGSync完成图片的blit操作，成功返回CSUDI_SUCCESS。
//@EXECUTIONFLOW:6、第5步完成之后，应该在屏幕上看到UDI2OSG_1920_1080.bmp图片的效果，如果看不到，则测试用例失败。
//@EXECUTIONFLOW:7、释放掉UDI2OSG_1920_1080.bmp占用的内存。
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0007(void)
{  
	return OSG_iTestDecImage(EM_OSG_DECIMAGE_BMP,EM_BMP_1920_1080);
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: 测试解码图片类型为jpg720*576格式时的解码,输出模式为EM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、有空闲的视频解码器
//@PRECONDITION: 3、平台支持jpg类型的图片解码
//@INPUT:1、传入图片的类型字符串为"JPG"
//@INPUT:2、图片UDI2OSG_720_576.jpg的数据地址
//@INPUT:3、图片UDI2OSG_720_576.jpg的长度
//@EXPECTATION: 1、能够成功对图片进行解码，返回CSUDI_SUCCESS；
//@EXPECTATION: 2、解码得到的surface数据能够成功blit到显存输出。
//@REMARK: 
//@EXECUTIONFLOW:1、获取UDI2OSG_720_576.jpg图片数据的内存地址和长度
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDI_SUCCESS，解码得到的phSurface应该不为CSUDI_NULL
//@EXECUTIONFLOW:3、调用CSUDIOSGGetDisplaySurface接口获取显存空间hDisSurface句柄
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit接口将解码得到的图片信息blit到显存空间hDisSurface上，bilt的矩形区域为CSUDI_NULL。
//@EXECUTIONFLOW:5、调用CSUDIOSGSync完成图片的blit操作，成功返回CSUDI_SUCCESS。
//@EXECUTIONFLOW:6、第5步完成之后，应该在屏幕上看到UDI2OSG_720_576.jpg图片的效果，如果看不到，则测试用例失败。
//@EXECUTIONFLOW:7、释放掉UDI2OSG_720_576.jpg占用的内存。
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0008(void)
{  
	return OSG_iTestDecImage(EM_OSG_DECIMAGE_JPG,EM_JPG_720_576);
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: 测试解码图片类型为jpg1280*720格式时的解码,输出模式为EM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、有空闲的视频解码器
//@PRECONDITION: 3、平台支持jpg类型的图片解码
//@INPUT:1、传入图片的类型字符串为"JPG"
//@INPUT:2、图片UDI2OSG_1280_720.jpg的数据地址
//@INPUT:3、图片UDI2OSG_1280_720.jpg的长度
//@EXPECTATION: 1、能够成功对图片进行解码，返回CSUDI_SUCCESS；
//@EXPECTATION: 2、解码得到的surface数据能够成功blit到显存输出。
//@REMARK: 
//@EXECUTIONFLOW:1、获取UDI2OSG_1280_720.jpg图片数据的内存地址和长度
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDI_SUCCESS，解码得到的phSurface应该不为CSUDI_NULL
//@EXECUTIONFLOW:3、调用CSUDIOSGGetDisplaySurface接口获取显存空间hDisSurface句柄
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit接口将解码得到的图片信息blit到显存空间hDisSurface上，bilt的矩形区域为CSUDI_NULL。
//@EXECUTIONFLOW:5、调用CSUDIOSGSync完成图片的blit操作，成功返回CSUDI_SUCCESS。
//@EXECUTIONFLOW:6、第5步完成之后，应该在屏幕上看到UDI2OSG_1280_720.jpg图片的效果，如果看不到，则测试用例失败。
//@EXECUTIONFLOW:7、释放掉UDI2OSG_1280_720.jpg占用的内存。
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0009(void)
{  
	return OSG_iTestDecImage(EM_OSG_DECIMAGE_JPG,EM_JPG_1280_720);
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: 测试解码图片类型为jpg1920*1080格式时的解码,输出模式为EM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、有空闲的视频解码器
//@PRECONDITION: 3、平台支持jpg类型的图片解码
//@INPUT:1、传入图片的类型字符串为"JPG"
//@INPUT:2、图片UDI2OSG_1920_1080.jpg的数据地址
//@INPUT:3、图片UDI2OSG_1920_1080.jpg的长度
//@EXPECTATION: 1、能够成功对图片进行解码，返回CSUDI_SUCCESS；
//@EXPECTATION: 2、解码得到的surface数据能够成功blit到显存输出。
//@REMARK: 
//@EXECUTIONFLOW:1、获取UDI2OSG_1920_1080.jpg图片数据的内存地址和长度
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDI_SUCCESS，解码得到的phSurface应该不为CSUDI_NULL
//@EXECUTIONFLOW:3、调用CSUDIOSGGetDisplaySurface接口获取显存空间hDisSurface句柄
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit接口将解码得到的图片信息blit到显存空间hDisSurface上，bilt的矩形区域为CSUDI_NULL。
//@EXECUTIONFLOW:5、调用CSUDIOSGSync完成图片的blit操作，成功返回CSUDI_SUCCESS。
//@EXECUTIONFLOW:6、第5步完成之后，应该在屏幕上看到UDI2OSG_1920_1080.jpg图片的效果，如果看不到，则测试用例失败。
//@EXECUTIONFLOW:7、释放掉UDI2OSG_1920_1080.jpg占用的内存。
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0010(void)
{  
	return OSG_iTestDecImage(EM_OSG_DECIMAGE_JPG,EM_JPG_1920_1080);
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: 测试解码图片类型为png720*576格式时的解码,输出模式为EM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、有空闲的视频解码器
//@PRECONDITION: 3、平台支持png类型的图片解码
//@INPUT:1、传入图片的类型字符串为"PNG"
//@INPUT:2、图片UDI2OSG_720_576.png的数据地址
//@INPUT:3、图片UDI2OSG_720_576.png的长度
//@EXPECTATION: 1、能够成功对图片进行解码，返回CSUDI_SUCCESS；
//@EXPECTATION: 2、解码得到的surface数据能够成功blit到显存输出。
//@REMARK: 
//@EXECUTIONFLOW:1、获取UDI2OSG_720_576.png图片数据的内存地址和长度
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDI_SUCCESS，解码得到的phSurface应该不为CSUDI_NULL
//@EXECUTIONFLOW:3、调用CSUDIOSGGetDisplaySurface接口获取显存空间hDisSurface句柄
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit接口将解码得到的图片信息blit到显存空间hDisSurface上，bilt的矩形区域为CSUDI_NULL。
//@EXECUTIONFLOW:5、调用CSUDIOSGSync完成图片的blit操作，成功返回CSUDI_SUCCESS。
//@EXECUTIONFLOW:6、第5步完成之后，应该在屏幕上看到UDI2OSG_720_576.png图片的效果，如果看不到，则测试用例失败。
//@EXECUTIONFLOW:7、释放掉UDI2OSG_720_576.png占用的内存。
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0011(void)
{  
	return OSG_iTestDecImage(EM_OSG_DECIMAGE_PNG,EM_PNG_720_576);
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: 测试解码图片类型为png1280*720格式时的解码,输出模式为EM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、有空闲的视频解码器
//@PRECONDITION: 3、平台支持png类型的图片解码
//@INPUT:1、传入图片的类型字符串为"PNG"
//@INPUT:2、图片UDI2OSG_1280_720.png的数据地址
//@INPUT:3、图片UDI2OSG_1280_720.png的长度
//@EXPECTATION: 1、能够成功对图片进行解码，返回CSUDI_SUCCESS；
//@EXPECTATION: 2、解码得到的surface数据能够成功blit到显存输出。
//@REMARK: 
//@EXECUTIONFLOW:1、获取UDI2OSG_1280_720.png图片数据的内存地址和长度
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDI_SUCCESS，解码得到的phSurface应该不为CSUDI_NULL
//@EXECUTIONFLOW:3、调用CSUDIOSGGetDisplaySurface接口获取显存空间hDisSurface句柄
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit接口将解码得到的图片信息blit到显存空间hDisSurface上，bilt的矩形区域为CSUDI_NULL。
//@EXECUTIONFLOW:5、调用CSUDIOSGSync完成图片的blit操作，成功返回CSUDI_SUCCESS。
//@EXECUTIONFLOW:6、第5步完成之后，应该在屏幕上看到UDI2OSG_1280_720.png图片的效果，如果看不到，则测试用例失败。
//@EXECUTIONFLOW:7、释放掉UDI2OSG_1280_720.png占用的内存。
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0012(void)
{  
	return OSG_iTestDecImage(EM_OSG_DECIMAGE_PNG,EM_PNG_1280_720);
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: 测试解码图片类型为png1920*1080格式时的解码,输出模式为EM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、有空闲的视频解码器
//@PRECONDITION: 3、平台支持png类型的图片解码
//@INPUT:1、传入图片的类型字符串为"PNG"
//@INPUT:2、图片UDI2OSG_1920_1080.png的数据地址
//@INPUT:3、图片UDI2OSG_1920_1080.png的长度
//@EXPECTATION: 1、能够成功对图片进行解码，返回CSUDI_SUCCESS；
//@EXPECTATION: 2、解码得到的surface数据能够成功blit到显存输出。
//@REMARK: 
//@EXECUTIONFLOW:1、获取UDI2OSG_1920_1080.png图片数据的内存地址和长度
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDI_SUCCESS，解码得到的phSurface应该不为CSUDI_NULL
//@EXECUTIONFLOW:3、调用CSUDIOSGGetDisplaySurface接口获取显存空间hDisSurface句柄
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit接口将解码得到的图片信息blit到显存空间hDisSurface上，bilt的矩形区域为CSUDI_NULL。
//@EXECUTIONFLOW:5、调用CSUDIOSGSync完成图片的blit操作，成功返回CSUDI_SUCCESS。
//@EXECUTIONFLOW:6、第5步完成之后，应该在屏幕上看到UDI2OSG_1920_1080.png图片的效果，如果看不到，则测试用例失败。
//@EXECUTIONFLOW:7、释放掉UDI2OSG_1920_1080.png占用的内存。
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0013(void)
{  
	return OSG_iTestDecImage(EM_OSG_DECIMAGE_PNG,EM_PNG_1920_1080);
}


//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: 测试解码的数据为非法的I帧图片数据
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、有空闲的视频解码器
//@PRECONDITION: 3、平台支持I帧类型的图片解码
//@INPUT:1、传入图片的类型字符串为"IFRAME"
//@INPUT:2、图片UDI2OSG_720_576_Error.mpg的数据地址
//@INPUT:3、图片UDI2OSG_720_576_Error.mpg的长度
//@EXPECTATION: 图片解码失败，返回CSUDIOSG_ERROR_UNKNOWN_ERROR
//@REMARK: 
//@EXECUTIONFLOW:1、获取UDI2OSG_720_576_Error.mpg图片数据的内存地址和长度
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDIOSG_ERROR_UNKNOWN_ERROR
//@EXECUTIONFLOW:4、释放掉UDI2OSG_720_576_Error.mpg占用的内存。
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
//@DESCRIPTION: 测试解码的数据为非法的JPEG图片数据
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、有空闲的视频解码器
//@PRECONDITION: 3、平台支持I帧类型的图片解码
//@INPUT:1、传入图片的类型字符串为"JPEG"
//@INPUT:2、图片UDI2OSG_Circle_576_576_Error.jpg的数据地址
//@INPUT:3、图片UDI2OSG_Circle_576_576_Error.jpg的长度
//@EXPECTATION: 图片解码失败，返回CSUDIOSG_ERROR_UNKNOWN_ERROR
//@REMARK: 
//@EXECUTIONFLOW:1、获取UDI2OSG_Circle_576_576_Error.jpg图片数据的内存地址和长度
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDIOSG_ERROR_UNKNOWN_ERROR
//@EXECUTIONFLOW:4、释放掉UDI2OSG_Circle_576_576_Error.jpg占用的内存
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0015(void)
{  
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hSurface_D = CSUDI_NULL;
	unsigned char* pcBuff = CSUDI_NULL;
	unsigned int nImageLen = 0;
	
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == OSG_IsSupportDecImage(EM_OSG_DECIMAGE_JPG)), 
		"平台不支持JPEG类型图片硬解码");
	
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
//@DESCRIPTION: 测试解码图片类型为bmp、jpg、png576*576格式时的解码,输出模式为EM_UDIOSG_SCALE_OPTIMUM
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、有空闲的视频解码器
//@PRECONDITION: 3、平台支持该类型的图片解码
//@INPUT:1、传入图片的类型字符串为相应的图片类型:"BMP"、"JPG"、"PNG"
//@INPUT:2、图片UDI2OSG_Circle_576_576的数据地址
//@INPUT:3、图片UDI2OSG_Circle_576_576的长度
//@INPUT:4、解码输出模式为EM_UDIOSG_SCALE_OPTIMUM
//@INPUT:5、解码输出Surface的大小为200*200
//@EXPECTATION: 1、能够成功对图片进行解码，返回CSUDI_SUCCESS；
//@EXPECTATION: 2、解码得到的surface大小应该为576:576的比例
//@EXPECTATION: 3、解码得到的surface数据能够成功blit到显存输出，图片的形状应该为正方形和圆形
//@REMARK: 
//@EXECUTIONFLOW:1、获取UDI2OSG_Circle_576_576图片数据的内存地址和长度
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDI_SUCCESS，解码得到的phSurface应该不为CSUDI_NULL
//@EXECUTIONFLOW:3、调用CSUDIOSGGetSurfaceInfo获取surface信息，期望解码得到的surface大小应该为576:576的比例
//@EXECUTIONFLOW:4、调用CSUDIOSGGetDisplaySurface接口获取显存空间hDisSurface句柄
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit接口将解码得到的图片信息blit到显存空间hDisSurface上，blit的矩形区域为CSUDI_NULL。
//@EXECUTIONFLOW:6、调用CSUDIOSGSync完成图片的blit操作，成功返回CSUDI_SUCCESS。
//@EXECUTIONFLOW:7、第5步完成之后，应该在屏幕上看到UDI2OSG_Circle_576_576图片的效果，图片的形状应该为正方形和圆形。
//@EXECUTIONFLOW:8、释放掉UDI2OSG_Circle_576_576占用的内存。
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
//@DESCRIPTION:  测试解码图片类型为bmp、jpg、png576*576格式时的解码,输出模式为EM_UDIOSG_SCALE_FIXSIZE
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、有空闲的视频解码器
//@PRECONDITION: 3、平台支持该类型的图片解码
//@INPUT:1、传入图片的类型字符串为相应的图片类型:"BMP"、"JPG"、"PNG"
//@INPUT:2、图片UDI2OSG_Circle_576_576的数据地址
//@INPUT:3、图片UDI2OSG_Circle_576_576的长度
//@INPUT:4、解码输出模式为EM_UDIOSG_SCALE_OPTIMUM
//@INPUT:5、解码输出Surface的大小为350*100
//@EXPECTATION: 1、能够成功对图片进行解码，返回CSUDI_SUCCESS；
//@EXPECTATION: 2、解码得到的surface数据能够成功blit到显存输出，图片的形状应该为长方形和椭圆
//@REMARK: 
//@EXECUTIONFLOW:1、获取UDI2OSG_Circle_576_576图片数据的内存地址和长度
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDI_SUCCESS，解码得到的phSurface应该不为CSUDI_NULL
//@EXECUTIONFLOW:3、调用CSUDIOSGGetSurfaceInfo获取surface信息，期望解码得到的surface大小应该为350*100
//@EXECUTIONFLOW:4、调用CSUDIOSGGetDisplaySurface接口获取显存空间hDisSurface句柄
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit接口将解码得到的图片信息blit到显存空间hDisSurface上，blit的矩形区域为CSUDI_NULL。
//@EXECUTIONFLOW:6、调用CSUDIOSGSync完成图片的blit操作，成功返回CSUDI_SUCCESS。
//@EXECUTIONFLOW:7、第5步完成之后，应该在屏幕上看到UDI2OSG_Circle_576_576图片的效果，图片的形状应该为长方形和椭圆
//@EXECUTIONFLOW:8、释放掉UDI2OSG_Circle_576_576占用的内存。
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
//@DESCRIPTION:  测试解码图片类型为bmp、jpg、png576*576格式时的解码,输出模式为EM_UDIOSG_SCALE_OPTIMUM
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、有空闲的视频解码器
//@PRECONDITION: 3、平台支持该类型的图片解码
//@INPUT:1、传入图片的类型字符串为相应的图片类型:"BMP"、"JPG"、"PNG"
//@INPUT:2、图片UDI2OSG_Circle_576_576的数据地址
//@INPUT:3、图片UDI2OSG_Circle_576_576的长度
//@INPUT:4、解码输出模式为EM_UDIOSG_SCALE_OPTIMUM
//@INPUT:5、解码输出Surface的大小为350*100
//@EXPECTATION: 1、能够成功对图片进行解码，返回CSUDI_SUCCESS；
//@EXPECTATION: 2、解码得到的surface数据能够成功blit到显存输出，图片的形状应该为长方形和椭圆
//@REMARK: 
//@EXECUTIONFLOW:1、获取UDI2OSG_Circle_576_576图片数据的内存地址和长度
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDI_SUCCESS，解码得到的phSurface应该不为CSUDI_NULL
//@EXECUTIONFLOW:3、调用CSUDIOSGGetSurfaceInfo获取surface信息，期望解码得到的surface大小应该为350*100
//@EXECUTIONFLOW:4、调用CSUDIOSGGetDisplaySurface接口获取显存空间hDisSurface句柄
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit接口将解码得到的图片信息blit到显存空间hDisSurface上，blit的矩形区域为CSUDI_NULL。
//@EXECUTIONFLOW:6、调用CSUDIOSGSync完成图片的blit操作，成功返回CSUDI_SUCCESS。
//@EXECUTIONFLOW:7、第5步完成之后，应该在屏幕上看到UDI2OSG_Circle_576_576图片的效果，图片的形状应该为长方形和椭圆,其大小在350*100的矩形内
//@EXECUTIONFLOW:8、释放掉UDI2OSG_Circle_576_576占用的内存。
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
//@DESCRIPTION:  测试解码图片类型为jpg,且jpg有两个SOF或者是SOF中水平和垂直数据采样比为1:2的情况下程序正常
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、平台支持该类型的图片解码
//@INPUT:1、传入图片的类型字符串为相应的图片类型:"JPG"
//@INPUT:2、图片EM_JPG_DOUBLE_SOF,EM_JPG_SOF_Y_SAMPLE_12,EM_PNG_CIRCLE_576_576 的数据地址
//@INPUT:3、图片EM_JPG_DOUBLE_SOF,EM_JPG_SOF_Y_SAMPLE_12,EM_PNG_CIRCLE_576_576的长度
//@INPUT:4、解码输出模式为EM_UDIOSG_SCALE_FULLSIZE
//@EXPECTATION: 1、如果支持EM_JPG_DOUBLE_SOF和EM_JPG_SOF_Y_SAMPLE_12两幅图片的解码则图片应该显示正常
//@EXPECTATION: 2、如果不支持EM_JPG_DOUBLE_SOF和EM_JPG_SOF_Y_SAMPLE_12两幅图片的解码则不程序不应该死机,也不能影响后面图片的正常浏览
//@REMARK: 
//@EXECUTIONFLOW:1、获取EM_JPG_DOUBLE_SOF的图片数据
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage解码图片,如果接口返回success 则将数据blit到标清输出上，否则不显示
//@EXECUTIONFLOW:3、如果解码成功则调用CSUDIOSGDestroySurface释放surface
//@EXECUTIONFLOW:4、获取EM_JPG_SOF_Y_SAMPLE_12的图片数据
//@EXECUTIONFLOW:5、调用CSUDIOSGDecodeImage解码图片,如果接口返回success 则将数据blit到标清输出上，否则不显示
//@EXECUTIONFLOW:6、如果解码成功则调用CSUDIOSGDestroySurface释放surface
//@EXECUTIONFLOW:7、获取EM_PNG_CIRCLE_576_576的图片数据
//@EXECUTIONFLOW:8、调用CSUDIOSGDecodeImage解码图片期望返回CSUDI_SUCCESS,并且图片显示正常
//@EXECUTIONFLOW:9、如果解码成功则调用CSUDIOSGDestroySurface释放surface
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0019(void)
{
	return OSG_iTestDecImageSeveralJpg();
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION:  测试解码图片类型为bmp、jpg、png576*576格式时的解码,输出模式为EM_UDIOSG_SCALE_OPTIMUM
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、有空闲的视频解码器
//@PRECONDITION: 3、平台支持该类型的图片解码
//@INPUT:1、传入图片的类型字符串为相应的图片类型:"BMP"、"JPG"、"PNG"
//@INPUT:2、图片UDI2OSG_Circle_576_576的数据地址
//@INPUT:3、图片UDI2OSG_Circle_576_576的长度
//@INPUT:4、解码输出模式为EM_UDIOSG_SCALE_OPTIMUM
//@INPUT:5、解码输出Surface的大小为500*500
//@EXPECTATION: 1、能够成功对图片进行解码，返回CSUDI_SUCCESS；
//@EXPECTATION: 2、解码得到的surface数据能够成功blit到显存输出，图片的形状应该为长方形和椭圆
//@REMARK: 
//@EXECUTIONFLOW:1、获取UDI2OSG_Circle_576_576图片数据的内存地址和长度
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDI_SUCCESS，解码得到的phSurface应该不为CSUDI_NULL
//@EXECUTIONFLOW:3、调用CSUDIOSGGetSurfaceInfo获取surface信息，期望解码得到的surface大小应该为500*500
//@EXECUTIONFLOW:4、调用CSUDIOSGGetDisplaySurface接口获取显存空间hDisSurface句柄
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit接口将解码得到的图片信息blit到显存空间hDisSurface上，blit的矩形区域为CSUDI_NULL。
//@EXECUTIONFLOW:6、调用CSUDIOSGSync完成图片的blit操作，成功返回CSUDI_SUCCESS。
//@EXECUTIONFLOW:7、第5步完成之后，应该在屏幕上看到UDI2OSG_Circle_576_576图片的效果，图片的形状应该为长方形和椭圆,其大小在500*500的矩形内
//@EXECUTIONFLOW:8、释放掉UDI2OSG_Circle_576_576占用的内存。
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
//@DESCRIPTION:  测试解码图片类型为jpg 8192*8192格式时的解码,输出模式为EM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、有空闲的视频解码器
//@PRECONDITION: 3、平台支持该类型的图片解码
//@INPUT:1、传入图片的类型字符串为相应的图片类型:"JPG"
//@INPUT:2、图片EM_JPG_8192_8192的数据地址
//@INPUT:3、图片EM_JPG_8192_8192的长度
//@INPUT:4、解码输出模式为EM_UDIOSG_SCALE_FULLSIZE
//@INPUT:5、解码输出Surface的大小为8192*8192
//@EXPECTATION: 1、能够成功对图片进行解码，返回CSUDI_SUCCESS；
//@EXPECTATION: 2、解码得到的surface数据能够成功blit到显存输出，图片的形状应该为长方形和椭圆
//@REMARK: 
//@EXECUTIONFLOW:1、获取EM_JPG_8192_8192图片数据的内存地址和长度
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDI_SUCCESS，解码得到的phSurface应该不为CSUDI_NULL
//@EXECUTIONFLOW:3、调用CSUDIOSGGetSurfaceInfo获取surface信息，期望解码得到的surface大小应该为8192*8192
//@EXECUTIONFLOW:4、调用CSUDIOSGGetDisplaySurface接口获取显存空间hDisSurface句柄
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit接口将解码得到的图片信息blit到显存空间hDisSurface上，blit的矩形区域为CSUDI_NULL。
//@EXECUTIONFLOW:6、调用CSUDIOSGSync完成图片的blit操作，成功返回CSUDI_SUCCESS。
//@EXECUTIONFLOW:7、第5步完成之后，应该在屏幕上看到EM_JPG_8192_8192图片的效果，图片的形状应该为长方形和椭圆,其大小在8192*8192的矩形内
//@EXECUTIONFLOW:8、释放掉EM_JPG_8192_8192占用的内存。
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
//@DESCRIPTION: 测试解码图片类型为IFRAME、JPG 720*576格式时的解码,输出模式为EM_UDIOSG_SCALE_OPTIMUM
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、有空闲的视频解码器
//@PRECONDITION: 3、平台支持该类型的图片解码
//@INPUT:1、传入图片的类型字符串为相应的图片类型:"IFRAME"
//@INPUT:2、图片EM_IFRAME_720_576的数据地址
//@INPUT:3、图片EM_IFRAME_720_576的长度
//@INPUT:4、解码输出模式为EM_UDIOSG_SCALE_OPTIMUM
//@INPUT:5、解码输出Surface的大小初始值为{600，300}
//@EXPECTATION: 1、能够成功对图片进行解码，返回CSUDI_SUCCESS；
//@EXPECTATION: 2、解码得到的surface的高宽应该为{720*300/576，300}
//@EXPECTATION: 3、解码得到的surface数据能够成功blit到显存输出
//@REMARK: 
//@EXECUTIONFLOW:1、获取EM_IFRAME_720_576图片数据的内存地址和长度
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDI_SUCCESS，解码得到的phSurface应该不为CSUDI_NULL
//@EXECUTIONFLOW:3、调用CSUDIOSGGetSurfaceInfo获取surface信息，期望解码得到的surface大小应该为{375，300}的比例
//@EXECUTIONFLOW:4、调用CSUDIOSGGetDisplaySurface接口获取显存空间hDisSurface句柄
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit接口将解码得到的图片信息blit到显存空间hDisSurface上，blit的矩形区域为CSUDI_NULL。
//@EXECUTIONFLOW:6、调用CSUDIOSGSync完成图片的blit操作，成功返回CSUDI_SUCCESS。
//@EXECUTIONFLOW:7、第5步完成之后，应该在屏幕上看到EM_IFRAME_720_576图片的效果。
//@EXECUTIONFLOW:8、释放掉EM_IFRAME_720_576占用的内存。
//@EXECUTIONFLOW:9、对EM_JPG_720_576图片重复以上步骤。
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0022(void)
{  
	CSUDIOSGRect_S sDstRect;
	CSUDI_BOOL bRet = CSUDI_TRUE;

	sDstRect.m_nWidth = 600;
	sDstRect.m_nHeight = 300;
	bRet = OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_IFRAME,EM_IFRAME_720_576,EM_UDIOSG_SCALE_OPTIMUM,&sDstRect);
	CSTK_ASSERT_TRUE_FATAL((bRet == CSUDI_TRUE),"解码MPEG失败!");
		
	bRet = OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_JPG,EM_JPG_720_576,EM_UDIOSG_SCALE_OPTIMUM,&sDstRect);
	CSTK_ASSERT_TRUE_FATAL((bRet == CSUDI_TRUE),"解码JPG失败!");
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: 测试解码图片类型为IFRAME、JPG 720*576格式时的解码,输出模式为EM_UDIOSG_SCALE_OPTIMUM
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、有空闲的视频解码器
//@PRECONDITION: 3、平台支持该类型的图片解码
//@INPUT:1、传入图片的类型字符串为相应的图片类型:"IFRAME"
//@INPUT:2、图片EM_IFRAME_720_576的数据地址
//@INPUT:3、图片EM_IFRAME_720_576的长度
//@INPUT:4、解码输出模式为EM_UDIOSG_SCALE_OPTIMUM
//@INPUT:5、解码输出Surface的大小初始值为{500，500}
//@EXPECTATION: 1、能够成功对图片进行解码，返回CSUDI_SUCCESS；
//@EXPECTATION: 2、解码得到的surface的高宽应该为{400*576/720，400}
//@EXPECTATION: 3、解码得到的surface数据能够成功blit到显存输出
//@REMARK: 
//@EXECUTIONFLOW:1、获取EM_IFRAME_720_576图片数据的内存地址和长度
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDI_SUCCESS，解码得到的phSurface应该不为CSUDI_NULL
//@EXECUTIONFLOW:3、调用CSUDIOSGGetSurfaceInfo获取surface信息，期望解码得到的surface大小应该为{500，400}的比例
//@EXECUTIONFLOW:4、调用CSUDIOSGGetDisplaySurface接口获取显存空间hDisSurface句柄
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit接口将解码得到的图片信息blit到显存空间hDisSurface上，blit的矩形区域为CSUDI_NULL。
//@EXECUTIONFLOW:6、调用CSUDIOSGSync完成图片的blit操作，成功返回CSUDI_SUCCESS。
//@EXECUTIONFLOW:7、第5步完成之后，应该在屏幕上看到EM_IFRAME_720_576图片的效果。
//@EXECUTIONFLOW:8、释放掉EM_IFRAME_720_576占用的内存。
//@EXECUTIONFLOW:9、对EM_JPG_720_576图片重复以上步骤。
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0023(void)
{  
	CSUDIOSGRect_S sDstRect;
	CSUDI_BOOL bRet = CSUDI_TRUE;

	sDstRect.m_nWidth = 500;
	sDstRect.m_nHeight = 500;
	
	bRet = OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_IFRAME,EM_IFRAME_720_576,EM_UDIOSG_SCALE_OPTIMUM,&sDstRect);
	CSTK_ASSERT_TRUE_FATAL((bRet == CSUDI_TRUE),"解码MPEG失败!");

	bRet = OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_JPG,EM_JPG_720_576,EM_UDIOSG_SCALE_OPTIMUM,&sDstRect);
	CSTK_ASSERT_TRUE_FATAL((bRet == CSUDI_TRUE),"解码JPG失败!");
	
	CSTK_FATAL_POINT;	
	return bRet;
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: 测试解码图片类型为IFRAME 720*576格式时的解码,输出模式为EM_UDIOSG_SCALE_OPTIMUM
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、有空闲的视频解码器
//@PRECONDITION: 3、平台支持该类型的图片解码
//@INPUT:1、传入图片的类型字符串为相应的图片类型:"IFRAME"
//@INPUT:2、图片EM_IFRAME_720_576的数据地址
//@INPUT:3、图片EM_IFRAME_720_576的长度
//@INPUT:4、解码输出模式为EM_UDIOSG_SCALE_OPTIMUM
//@INPUT:5、解码输出Surface的大小初始值为625*500
//@EXPECTATION: 1、能够成功对图片进行解码，返回CSUDI_SUCCESS；
//@EXPECTATION: 2、解码得到的surface的高宽应该为{625，500}
//@EXPECTATION: 3、解码得到的surface数据能够成功blit到显存输出
//@REMARK: 
//@EXECUTIONFLOW:1、获取EM_IFRAME_720_576图片数据的内存地址和长度
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDI_SUCCESS，解码得到的phSurface应该不为CSUDI_NULL
//@EXECUTIONFLOW:3、调用CSUDIOSGGetSurfaceInfo获取surface信息，期望解码得到的surface大小应该为{625，500}的比例
//@EXECUTIONFLOW:4、调用CSUDIOSGGetDisplaySurface接口获取显存空间hDisSurface句柄
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit接口将解码得到的图片信息blit到显存空间hDisSurface上，blit的矩形区域为CSUDI_NULL。
//@EXECUTIONFLOW:6、调用CSUDIOSGSync完成图片的blit操作，成功返回CSUDI_SUCCESS。
//@EXECUTIONFLOW:7、第5步完成之后，应该在屏幕上看到EM_IFRAME_720_576图片的效果。
//@EXECUTIONFLOW:8、释放掉EM_IFRAME_720_576占用的内存。
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0024(void)
{  
	CSUDIOSGRect_S sDstRect;
	CSUDI_BOOL bRet = CSUDI_TRUE;

	sDstRect.m_nWidth = 625;
	sDstRect.m_nHeight = 500;
	bRet = OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_IFRAME,EM_IFRAME_720_576,EM_UDIOSG_SCALE_OPTIMUM,&sDstRect);
	CSTK_ASSERT_TRUE_FATAL((bRet == CSUDI_TRUE),"解码MPEG失败!");
	
	CSTK_FATAL_POINT;
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: 测试解码图片类型为IFRAME 720*576格式时的解码,输出模式为EM_UDIOSG_SCALE_FIXSIZE
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、有空闲的视频解码器
//@PRECONDITION: 3、平台支持该类型的图片解码
//@INPUT:1、传入图片的类型字符串为相应的图片类型:"IFRAME"
//@INPUT:2、图片EM_IFRAME_720_576的数据地址
//@INPUT:3、图片EM_IFRAME_720_576的长度
//@INPUT:4、解码输出模式为EM_UDIOSG_SCALE_FIXSIZE
//@INPUT:5、解码输出Surface的大小初始值为625*500
//@EXPECTATION: 1、能够成功对图片进行解码，返回CSUDI_SUCCESS；
//@EXPECTATION: 2、解码得到的surface的高宽应该为{625，500}
//@EXPECTATION: 3、解码得到的surface数据能够成功blit到显存输出
//@REMARK: 
//@EXECUTIONFLOW:1、获取EM_IFRAME_720_576图片数据的内存地址和长度
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDI_SUCCESS，解码得到的phSurface应该不为CSUDI_NULL
//@EXECUTIONFLOW:3、调用CSUDIOSGGetSurfaceInfo获取surface信息，期望解码得到的surface大小应该为{625，500}的比例
//@EXECUTIONFLOW:4、调用CSUDIOSGGetDisplaySurface接口获取显存空间hDisSurface句柄
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit接口将解码得到的图片信息blit到显存空间hDisSurface上，blit的矩形区域为CSUDI_NULL。
//@EXECUTIONFLOW:6、调用CSUDIOSGSync完成图片的blit操作，成功返回CSUDI_SUCCESS。
//@EXECUTIONFLOW:7、第5步完成之后，应该在屏幕上看到EM_IFRAME_720_576图片的效果。
//@EXECUTIONFLOW:8、释放掉EM_IFRAME_720_576占用的内存。
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0025(void)
{  
	CSUDIOSGRect_S sDstRect;
	CSUDI_BOOL bRet = CSUDI_TRUE;

	sDstRect.m_nWidth = 625;
	sDstRect.m_nHeight = 500;
	bRet = OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_IFRAME,EM_IFRAME_720_576,EM_UDIOSG_SCALE_FIXSIZE,&sDstRect);

	CSTK_ASSERT_TRUE_FATAL((bRet == CSUDI_TRUE),"解码MPEG失败!");
	
	CSTK_FATAL_POINT;	
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: 测试解码图片类型为IFRAME 720*576格式时的解码,输出模式为EM_UDIOSG_SCALE_FIXSIZE
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、有空闲的视频解码器
//@PRECONDITION: 3、平台支持该类型的图片解码
//@INPUT:1、传入图片的类型字符串为相应的图片类型:"IFRAME"
//@INPUT:2、图片EM_IFRAME_720_576的数据地址
//@INPUT:3、图片EM_IFRAME_720_576的长度
//@INPUT:4、解码输出模式为EM_UDIOSG_SCALE_FIXSIZE
//@INPUT:5、解码输出Surface的大小初始值为500*500
//@EXPECTATION: 1、能够成功对图片进行解码，返回CSUDI_SUCCESS；
//@EXPECTATION: 2、解码得到的surface的高宽应该为{500，500}
//@EXPECTATION: 3、解码得到的surface数据能够成功blit到显存输出
//@REMARK: 
//@EXECUTIONFLOW:1、获取EM_IFRAME_720_576图片数据的内存地址和长度
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDI_SUCCESS，解码得到的phSurface应该不为CSUDI_NULL
//@EXECUTIONFLOW:3、调用CSUDIOSGGetSurfaceInfo获取surface信息，期望解码得到的surface大小应该为{500，500}的比例
//@EXECUTIONFLOW:4、调用CSUDIOSGGetDisplaySurface接口获取显存空间hDisSurface句柄
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit接口将解码得到的图片信息blit到显存空间hDisSurface上，blit的矩形区域为CSUDI_NULL。
//@EXECUTIONFLOW:6、调用CSUDIOSGSync完成图片的blit操作，成功返回CSUDI_SUCCESS。
//@EXECUTIONFLOW:7、第5步完成之后，应该在屏幕上看到EM_IFRAME_720_576图片的效果。
//@EXECUTIONFLOW:8、释放掉EM_IFRAME_720_576占用的内存。
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0026(void)
{  
	CSUDIOSGRect_S sDstRect;
	CSUDI_BOOL bRet = CSUDI_TRUE;

	sDstRect.m_nWidth = 500;
	sDstRect.m_nHeight = 500;
	bRet = OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_IFRAME,EM_IFRAME_720_576,EM_UDIOSG_SCALE_FIXSIZE,&sDstRect);

	CSTK_ASSERT_TRUE_FATAL((bRet == CSUDI_TRUE),"解码MPEG失败!");
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: 测试解码图片类型为IFRAME 720*576格式时的解码,输出模式为EM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、有空闲的视频解码器
//@PRECONDITION: 3、平台支持该类型的图片解码
//@INPUT:1、传入图片的类型字符串为相应的图片类型:"IFRAME"
//@INPUT:2、图片EM_IFRAME_720_576的数据地址
//@INPUT:3、图片EM_IFRAME_720_576的长度
//@INPUT:4、解码输出模式为EM_UDIOSG_SCALE_FULLSIZE
//@INPUT:5、解码输出Surface的大小初始值为500*500
//@EXPECTATION: 1、能够成功对图片进行解码，返回CSUDI_SUCCESS；
//@EXPECTATION: 2、解码得到的surface的高宽应该为{720，576}
//@EXPECTATION: 3、解码得到的surface数据能够成功blit到显存输出
//@REMARK: 
//@EXECUTIONFLOW:1、获取EM_IFRAME_720_576图片数据的内存地址和长度
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDI_SUCCESS，解码得到的phSurface应该不为CSUDI_NULL
//@EXECUTIONFLOW:3、调用CSUDIOSGGetSurfaceInfo获取surface信息，期望解码得到的surface大小应该为{720，576}的比例
//@EXECUTIONFLOW:4、调用CSUDIOSGGetDisplaySurface接口获取显存空间hDisSurface句柄
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit接口将解码得到的图片信息blit到显存空间hDisSurface上，blit的矩形区域为CSUDI_NULL。
//@EXECUTIONFLOW:6、调用CSUDIOSGSync完成图片的blit操作，成功返回CSUDI_SUCCESS。
//@EXECUTIONFLOW:7、第5步完成之后，应该在屏幕上看到EM_IFRAME_720_576图片的效果.
//@EXECUTIONFLOW:8、释放掉EM_IFRAME_720_576占用的内存。
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0027(void)
{  
	CSUDIOSGRect_S sDstRect;
	CSUDI_BOOL bRet = CSUDI_TRUE;

	sDstRect.m_nWidth = 500;
	sDstRect.m_nHeight = 500;
	bRet = OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_IFRAME,EM_IFRAME_720_576,EM_UDIOSG_SCALE_FULLSIZE,&sDstRect);
	
	CSTK_ASSERT_TRUE_FATAL((bRet == CSUDI_TRUE),"解码MPEG失败!");
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: 测试解码图片类型为IFRAME 720*576格式时的解码,输出模式为EM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、有空闲的视频解码器
//@PRECONDITION: 3、平台支持该类型的图片解码
//@INPUT:1、传入图片的类型字符串为相应的图片类型:"IFRAME"
//@INPUT:2、图片EM_IFRAME_PES_720_576的数据地址
//@INPUT:3、图片EM_IFRAME_PES_720_576的长度
//@INPUT:4、解码输出模式为EM_UDIOSG_SCALE_FULLSIZE
//@INPUT:5、解码输出Surface的大小初始值为500*500
//@EXPECTATION: 1、能够成功对图片进行解码，返回CSUDI_SUCCESS；
//@EXPECTATION: 2、解码得到的surface的高宽应该为{720，576}
//@EXPECTATION: 3、解码得到的surface数据能够成功blit到显存输出
//@REMARK: 
//@EXECUTIONFLOW:1、获取EM_IFRAME_PES_720_576图片数据的内存地址和长度
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDI_SUCCESS，解码得到的phSurface应该不为CSUDI_NULL
//@EXECUTIONFLOW:3、调用CSUDIOSGGetSurfaceInfo获取surface信息，期望解码得到的surface大小应该为{720，576}的比例
//@EXECUTIONFLOW:4、调用CSUDIOSGGetDisplaySurface接口获取显存空间hDisSurface句柄
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit接口将解码得到的图片信息blit到显存空间hDisSurface上，blit的矩形区域为CSUDI_NULL。
//@EXECUTIONFLOW:6、调用CSUDIOSGSync完成图片的blit操作，成功返回CSUDI_SUCCESS。
//@EXECUTIONFLOW:7、第5步完成之后，应该在屏幕上看到EM_IFRAME_PES_720_576图片的效果。
//@EXECUTIONFLOW:8、释放掉EM_IFRAME_PES_720_576占用的内存。
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0028(void)
{  
	CSUDIOSGRect_S sDstRect;
	CSUDI_BOOL bRet = CSUDI_TRUE;

	sDstRect.m_nX 		= sDstRect.m_nY = 0;
	sDstRect.m_nWidth 	= 500;
	sDstRect.m_nHeight 	= 500;
	bRet = OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_IFRAME, EM_IFRAME_PES_720_576,EM_UDIOSG_SCALE_FULLSIZE,&sDstRect);
	
	CSTK_ASSERT_TRUE_FATAL((bRet == CSUDI_TRUE),"解码MPEG失败!");
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: 测试解码图片类型为IFRAME PES 与 ES 格式时的轮流解码,大小为720*576输出模式为EM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、有空闲的视频解码器
//@PRECONDITION: 3、平台支持该类型的图片解码
//@INPUT:1、传入图片的类型字符串为相应的图片类型:"IFRAME"
//@INPUT:2、图片EM_IFRAME_PES_720_576的数据地址
//@INPUT:3、图片EM_IFRAME_PES_720_576的长度
//@INPUT:4、解码输出模式为EM_UDIOSG_SCALE_FULLSIZE
//@INPUT:5、解码输出Surface的大小初始值为500*500
//@INPUT:6、图片EM_IFRAME_ES_720_576的数据地址
//@INPUT:7、图片EM_IFRAME_ES_720_576的长度
//@INPUT:8、解码输出模式为EM_UDIOSG_SCALE_FULLSIZE
//@INPUT:9、解码输出Surface的大小为720*576
//@EXPECTATION: 1、能够成功对图片进行解码，返回CSUDI_SUCCESS；
//@EXPECTATION: 2、解码得到的surface的高宽应该为{720*576}
//@EXPECTATION: 3、解码得到的surface数据能够成功blit到显存输出
//@REMARK: 
//@EXECUTIONFLOW:1、获取EM_IFRAME_PES_720_576图片数据的内存地址和长度
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDI_SUCCESS，解码得到的phSurface应该不为CSUDI_NULL
//@EXECUTIONFLOW:3、调用CSUDIOSGGetSurfaceInfo获取surface信息，期望解码得到的surface大小应该为{720，576}的比例
//@EXECUTIONFLOW:4、调用CSUDIOSGGetDisplaySurface接口获取显存空间hDisSurface句柄
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit接口将解码得到的图片信息blit到显存空间hDisSurface上，blit的矩形区域为CSUDI_NULL。
//@EXECUTIONFLOW:6、调用CSUDIOSGSync完成图片的blit操作，成功返回CSUDI_SUCCESS。
//@EXECUTIONFLOW:7、释放掉EM_IFRAME_PES_720_576占用的内存。
//@EXECUTIONFLOW:8、获取EM_IFRAME_720_576图片数据的内存地址和长度
//@EXECUTIONFLOW:9、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDI_SUCCESS，解码得到的phSurface应该不为CSUDI_NULL
//@EXECUTIONFLOW:10、调用CSUDIOSGGetSurfaceInfo获取surface信息，期望解码得到的surface大小应该为{720，576}的比例
//@EXECUTIONFLOW:11、调用CSUDIOSGGetDisplaySurface接口获取显存空间hDisSurface句柄
//@EXECUTIONFLOW:12、调用CSUDIOSGBlit接口将解码得到的图片信息blit到显存空间hDisSurface上，blit的矩形区域为CSUDI_NULL。
//@EXECUTIONFLOW:13、调用CSUDIOSGSync完成图片的blit操作，成功返回CSUDI_SUCCESS。
//@EXECUTIONFLOW:14、释放掉EM_IFRAME_720_576占用的内存。
//@EXECUTIONFLOW:15、重复以上步骤100次，屏幕上可以看到两张IFRAME的图片在切换。
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
		CSTK_ASSERT_TRUE_FATAL((bRet == CSUDI_TRUE),"解码MPEG失败!");
		
		bRet = OSG_iTestDecImageWithScaleMode(EM_OSG_DECIMAGE_IFRAME, EM_IFRAME_720_576,EM_UDIOSG_SCALE_FULLSIZE,&sDstRect);
		CSTK_ASSERT_TRUE_FATAL((bRet == CSUDI_TRUE),"解码MPEG失败!");
	}
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: 模拟遍历解码显示指定路径的图片,输出模式为EM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、有空闲的解码器
//@PRECONDITION: 3、平台支持图片解码
//@INPUT:1、传入testdata目录下的图片路径
//@INPUT:2、图片路径数组中的数据地址
//@INPUT:3、图片路径数组中的数据长度
//@EXPECTATION: 1、能够成功对图片进行解码，返回CSUDI_SUCCESS；
//@EXPECTATION: 2、解码得到的surface数据能够成功blit到显存输出。
//@REMARK: 
//@EXECUTIONFLOW:1、获取testdat路径下的图片文件的路径。
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDI_SUCCESS，解码得到的phSurface应该不为CSUDI_NULL
//@EXECUTIONFLOW:3、调用CSUDIOSGGetDisplaySurface接口获取显存空间hDisSurface句柄
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit接口将解码得到的图片信息blit到显存空间hDisSurface上，bilt的矩形区域为CSUDI_NULL。
//@EXECUTIONFLOW:5、调用CSUDIOSGSync完成图片的blit操作，成功返回CSUDI_SUCCESS。
//@EXECUTIONFLOW:6、第5步完成之后，应该在屏幕上看到图片的效果，如果看不到，则测试用例失败。
//@EXECUTIONFLOW:7、释放掉解码占用的内存。
//@EXECUTIONFLOW:8、销毁图片数据。
//@EXECUTIONFLOW:9、获取下一张图片的路径，重复步骤2~7。
//@EXECUTIONFLOW:10、模拟遍历文件夹十遍。
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0030(void)
{  
	int i;
	CSUDI_BOOL bRet;
	bRet = CSUDI_FALSE;

	for(i = 0; i< 10; i++)
	{
		bRet = OSG_iTestDecFolderImages();
		CSTK_ASSERT_TRUE_FATAL((bRet == CSUDI_TRUE),"解码图片失败!");
	}
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIOSGDecodeImage
//@DESCRIPTION: 将指定路径的图片全部解码一遍，统计所用的时间,输出模式为EM_UDIOSG_SCALE_FULLSIZE
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、有空闲的解码器
//@PRECONDITION: 3、平台支持图片解码
//@INPUT:1、传入testdata目录下的图片路径
//@INPUT:2、图片路径数组中的数据地址
//@INPUT:3、图片路径数组中的数据长度
//@EXPECTATION: 1、能够成功对图片进行解码，返回CSUDI_SUCCESS；
//@EXPECTATION: 2、解码得到的surface数据能够成功blit到显存输出
//@REMARK: 
//@EXECUTIONFLOW:1、获取testdat路径下的图片文件的路径
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDI_SUCCESS，解码得到的phSurface应该不为CSUDI_NULL
//@EXECUTIONFLOW:3、输出CSUDIOSGDecodeImage解码图片耗用的时间。
//@EXECUTIONFLOW:4、调用CSUDIOSGGetDisplaySurface接口获取显存空间hDisSurface句柄
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit接口将解码得到的图片信息blit到显存空间hDisSurface上，bilt的矩形区域为CSUDI_NULL
//@EXECUTIONFLOW:6、调用CSUDIOSGSync完成图片的blit操作，成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、第5步完成之后，应该在屏幕上看到图片的效果，如果看不到，则测试用例失败
//@EXECUTIONFLOW:8、释放掉占用的内存
//@EXECUTIONFLOW:9、获取下一张图片的路径，重复步骤2~7
CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0031(void)
{  
	CSUDI_BOOL bRet;
	bRet = CSUDI_TRUE;

	bRet = OSG_iTestDecFolderImages();

	return bRet;
}

//@CASEGROUP: CSUDIOSGDecodeImage32
//@DESCRIPTION: 测试解码的数据量大于128kb的I帧图片数据
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、有空闲的视频解码器
//@PRECONDITION: 3、平台支持I帧类型的图片解码
//@INPUT:1、传入图片的类型字符串为"IFRAME"
//@INPUT:2、图片UDI2OSG_bigiframe.mpg的数据地址
//@INPUT:3、图片UDI2OSG_bigiframe.mpg的长度
//@EXPECTATION: 图片解码成功
//@REMARK: 
//@EXECUTIONFLOW:1、获取UDI2OSG_bigiframe.mpg图片数据的内存地址和长度
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、显示解码后的图片，确认结果。
//@EXECUTIONFLOW:4、销毁解码的surface。
//@EXECUTIONFLOW:5、释放掉UDI2OSG_bigiframe.mpg占用的内存。
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

	CSTCPrint("\r屏幕上有图片显示吗?\n");		
	
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

	//清屏
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
//@DESCRIPTION:  测试CSUDIOSGSurfaceInfo_S中绘图平面的内存大小参数
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT:P1_1+X2|X1
//@EXPECTATION: 从绘图平面信息中获取的为m_nMemSize = m_nHeight*m_nPitch
//@REMARK:
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建绘图平面P1_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取绘图平面P1_1的信息P1_1_Info
//@EXECUTIONFLOW:3、内存大小值P1_1_Info.m_nMemSize = P1_1_Info.m_nHeight*P1_1_Info.m_nPitch
//@EXECUTIONFLOW:4、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_0001(void)
{   
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"没有找到任何支持的满足要求的PixelFormat"
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
//@DESCRIPTION: 测试源和目标surface均为显存空间并且矩形区域重叠时的blit操作
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: D+SR6+DR7+M1
//@EXPECTATION: 执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄
//@EXECUTIONFLOW:2、调用CSUDIOSGFill显存空间区域D的颜色值为0
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit（D,DR7,D,SR6,M1）
//@EXECUTIONFLOW:4、执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_OSG_IT_0002(void)
{   
	return OSG_iTestBlitWithSameDisplay(EM_RECT_6,EM_RECT_7,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_0003
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION: 测试源和目标surface均为相同的缓存空间并且矩形区域重叠时的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+X2/X1+SR6+DR7+M1
//@EXPECTATION: 返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建绘图平面P1_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGFill缓存空间P1_1的颜色值为0
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit（P1_1,DR7,P1_1,SR6,M1）
//@EXECUTIONFLOW:4、执行blit失败，返回CSUDIOSG_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_0003(void)
{   
	return OSG_iTestBlitWithSameSurface(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,EM_RECT_6,EM_RECT_7,CSUDI_FALSE);
}

//@TESTCASEID:CSTC_OSG_IT_0004
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:  测试源和目标surface均为相同的缓存空间并且矩形区域有放大时的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+X2/X1+SR6+DR13+M1+Cr1
//@EXPECTATION:1、成功则返回CSUDI_SUCCESS，且目标区域能够根据矩形大小进行拉伸
//@EXPECTATION:2、如果不支持应该返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED
//@REMARK: DR13可以取值为不与SR6重叠的任意倍数(平台支持的倍数)
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建绘图平面P1_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGFill绘图平面的颜色值为 0
//@EXECUTIONFLOW:3、调用CSUDIOSGFill绘图平面区域SR6的颜色值为Cr1
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit（P1_1,DR13,P1_1,SR6,M1）
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取P1_1绘图平面信息P1_1_Info
//@EXECUTIONFLOW:6、获取绘图平面DR13区域内的颜色值应该为Cr1
//@EXECUTIONFLOW:7、如果步骤4返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED，步骤4、5可以省略
//@EXECUTIONFLOW:8、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_0004(void)
{   
	return OSG_iTestBlitWithSameSurface(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,EM_RECT_6,EM_RECT_13,CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_0005
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:  测试源和目标surface均为相同的缓存空间并且矩形区域有缩小时的blit操作
//@PRECONDITION: 1、平台UDI已经成功初始化
//@PRECONDITION: 2、创建绘图平面的颜色模式为X2或X1
//@INPUT: P1_1+X2/X1+SR13+DR6+M1+Cr1
//@EXPECTATION:1、成功则返回CSUDI_SUCCESS，且目标区域能够根据矩形大小进行缩放
//@EXPECTATION:2、如果不支持应该返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED
//@REMARK: DR13可以取值为不与SR6重叠的任意倍数
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建绘图平面P1_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGFill绘图平面的颜色值为0
//@EXECUTIONFLOW:3、调用CSUDIOSGFill绘图平面区域SR13的颜色值为Cr1
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit（P1_1,DR6,P1_1,SR13,M1）
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取P1_1绘图平面信息P1_1_Info
//@EXECUTIONFLOW:6、获取绘图平面DR6区域内的颜色值应该为Cr1
//@EXECUTIONFLOW:7、如果返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED，步骤4、5可以省略
//@EXECUTIONFLOW:8、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_0005(void)
{   
	return OSG_iTestBlitWithSameSurface(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,EM_RECT_13,EM_RECT_6,CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_0006
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:  测试源和目标surface均为相同的显存空间并且矩形区域有放大时的blit操作
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: D+SR6+DR13+M1+Cr1
//@EXPECTATION:1、成功则返回CSUDI_SUCCESS，且目标区域能够根据矩形大小进行拉伸
//@EXPECTATION:2、如果不支持应该返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED
//@REMARK: DR13可以取值为不与SR6重叠的任意倍数
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄D
//@EXECUTIONFLOW:2、调用CSUDIOSGFill显存空间区域的颜色值为0
//@EXECUTIONFLOW:3、调用CSUDIOSGFill显存空间区域SR6的颜色值为Cr1
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit（D,DR13,D,SR6,M1）
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取D显存空间信息D_Info
//@EXECUTIONFLOW:6、获取绘图平面DR13区域内的颜色值应该为Cr1
//@EXECUTIONFLOW:7、如果返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED，步骤4、5可以省略
CSUDI_BOOL CSTC_OSG_IT_0006(void)
{   
	return OSG_iTestBlitWithSameDisplay(EM_RECT_6,EM_RECT_13,CSUDI_TRUE);
}

//@TESTCASEID:CSTC_OSG_IT_0007
//@CASEGROUP: CSUDIOSGBlit
//@DESCRIPTION:  测试源和目标surface均为相同的显存空间并且矩形区域有缩小时的blit操作
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: D+SR13+DR6+M1+Cr1
//@EXPECTATION:1、成功则返回CSUDI_SUCCESS，且目标区域能够根据矩形大小进行缩放
//@EXPECTATION:2、如果不支持应该返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED
//@REMARK: DR13可以取值为不与SR6重叠的任意倍数
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄D
//@EXECUTIONFLOW:2、调用CSUDIOSGFill显存空间区域的颜色值为0
//@EXECUTIONFLOW:3、调用CSUDIOSGFill显存空间区域SR13的颜色值为Cr1
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit（D,DR6,D,SR13,M1）
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取P1_1绘图平面信息D_Info
//@EXECUTIONFLOW:6、获取绘图平面DR13区域内的颜色值应该为Cr1
//@EXECUTIONFLOW:7、如果返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED，步骤4、5可以省略
CSUDI_BOOL CSTC_OSG_IT_0007(void)
{   
	return OSG_iTestBlitWithSameDisplay(EM_RECT_13,EM_RECT_6,CSUDI_TRUE);	
}

//@TESTCASEID:CSTC_OSG_IT_0008
//@CASEGROUP: CSUDIOSGBlit
//@CASEGROUP: CSUDIOSGCreateSurface
//@CASEGROUP: CSUDIOSGSync
//@DESCRIPTION:  在执行Sync操作之前，进行多次的blit操作 
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: P1_1+D+SR1+DR1+M1+X2/X1 
//@EXPECTATION: 原则上可以执行无数次blit的操作，再进行Sync操作，不能死机，不能产生不可恢复的异常
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建绘图平面P1_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGGetDisplaySurface获取显存空间句柄D
//@EXECUTIONFLOW:3、调用CSUDIOSGFill缓存空间P1_1的颜色值为Cr1
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit（D,DR1,P1_1,SR1,M1）
//@EXECUTIONFLOW:5、循环进行N次步骤3和步骤4，建议每次改变Cr1的值
//@EXECUTIONFLOW:6、调用CSUDIOSGSync
//@EXECUTIONFLOW:7、调用CSUDIOSGGetSurfaceInfo获取缓存空间D的信息D_Info
//@EXECUTIONFLOW:8、获取绘图平面D区域内的颜色值应该为最后一次Fill的Cr1
//@EXECUTIONFLOW:9、调用CSUDIOSGDestroySurface销毁绘图平面
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
				"没有找到任何支持的满足要求的PixelFormat"
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

	CSTCPrint("此用例耗时较久，请耐心等待。。。\n");
	
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
//@CASEGROUP: 1、CSUDIOSGBlit
//@CASEGROUP: 2、CSUDIOSGDestroySurface
//@CASEGROUP: 3、CSUDIOSGCreateSurface
//@CASEGROUP: 4、CSUDIOSGSync
//@DESCRIPTION:  没有执行Sync之前，在blit的过程中删除源绘图平面 
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: P1_1+D+SR1+DR1+M1+X2/X1  
//@EXPECTATION: 最终的绘图效果不确定，但不能死机，不能产生不可恢复的异常
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建绘图平面P1_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGGetDisplaySurface获取显存空间句柄D
//@EXECUTIONFLOW:3、调用CSUDIOSGFill缓存空间P1_1的颜色值为Cr1
//@EXECUTIONFLOW:4、调用CSUDIOSGFill显存空间区域的颜色值为0
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit（D,DR1,P1_1,SR1,M1）
//@EXECUTIONFLOW:6、调用CSUDIOSGDestroySurface源绘图平面P1_1
//@EXECUTIONFLOW:7、调用CSUDIOSGSync
//@EXECUTIONFLOW:8、调用CSUDIOSGGetSurfaceInfo获取缓存空间D的信息D_Info
//@EXECUTIONFLOW:9、获取绘图平面D区域内的颜色值不确定
//@EXECUTIONFLOW:10、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_0009(void)
{   
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hSurface_D = CSUDI_NULL;
	CSUDI_Error_Code nRet;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"没有找到任何支持的满足要求的PixelFormat"
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
	
	//执行此步骤不应该出现死机等现象
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
//@CASEGROUP: 1、CSUDIOSGBlit
//@CASEGROUP: 2、CSUDIOSGDestroySurface
//@CASEGROUP: 3、CSUDIOSGCreateSurface
//@CASEGROUP: 4、CSUDIOSGSync
//@CASEGROUP: 5、CSUDIOSGFill
//@DESCRIPTION:  没有执行Sync之前，在blit的过程中，再对源绘图平面去执行fill操作
//@PRECONDITION: 平台UDI已经成功初始化
//@INPUT: P1_1+D+SR1+DR1+M1+X2/X1  
//@EXPECTATION: 最终显存的颜色值为Cr2
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建绘图平面P1_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGGetDisplaySurface获取显存空间句柄D
//@EXECUTIONFLOW:3、调用CSUDIOSGFill缓存空间P1_1的颜色值为Cr1
//@EXECUTIONFLOW:4、调用CSUDIOSGFill显存空间区域的颜色值为0
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit（D,DR1,P1_1,SR1,M1）
//@EXECUTIONFLOW:6、调用CSUDIOSGFill缓存空间P1_1的颜色值为Cr2
//@EXECUTIONFLOW:7、调用CSUDIOSGSync
//@EXECUTIONFLOW:8、调用CSUDIOSGGetSurfaceInfo获取缓存空间D的信息D_Info
//@EXECUTIONFLOW:9、获取绘图平面D区域内的颜色值应为Cr2
//@EXECUTIONFLOW:10、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_0010(void)
{   
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_HANDLE hSurface_D = CSUDI_NULL;
	CSUDI_Error_Code nRet;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"没有找到任何支持的满足要求的PixelFormat"
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
	
	//执行此步骤不应该出现死机等现象
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
//@CASEGROUP: 1、CSUDIOSGCreateSurface
//@CASEGROUP: 2、CSUDIOSGDestroySurface
//@DESCRIPTION:测试连续创建和删除绘图平面N多次后，还能够创建绘图平面
//@PRECONDITION:平台UDI已经成功初始化
//@INPUT:	P1_1+X2/X1
//@EXPECTATION: 每次创建绘图平面都成功，不会出现内存碎片（测试几万次后还能够继续创建绘图平面），不会出现死机
//@REMARK: 运行此测试用例会花费很长时间
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建绘图平面P1_1，颜色模式为X2或X1
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取绘图平面P1_1的信息
//@EXECUTIONFLOW:3、获取的绘图平面的信息应该跟创建时传入的信息一致
//@EXECUTIONFLOW:4、调用CSUDIOSGDestroySurface销毁绘图平面P1_1
//@EXECUTIONFLOW:5、循环执行步骤1至步骤4 N次（N>=10000)
CSUDI_BOOL CSTC_OSG_IT_0011(void)
{   
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_Error_Code nRet;
	int jj;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"没有找到任何支持的满足要求的PixelFormat"
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
//@DESCRIPTION:测试源为ARGB和目的为YUV颜色模式之间的blit操作
//@PRECONDITION:平台UDI已经成功初始化
//@INPUT: P1_1+P2_1+X1+X9+SR1+DR1+M1
//@EXPECTATION: 支持blit操作则返回CSUDI_SUCCESS，不支持则返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED。 
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1，颜色模式为X1
//@EXECUTIONFLOW:2、调用CSUDIOSGCreateSurface创建缓存空间P2_1，颜色模式为X9
//@EXECUTIONFLOW:3、调用CSUDIOSGFill填充缓存空间P1_1的颜色值为Cr1
//@EXECUTIONFLOW:4、调用CSUDIOSGSyn填充完成
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M1），完成从ARGB向YUV模式的blit操作
//@EXECUTIONFLOW:6、调用CSUDIOSGSync完成blit，并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIOSGFill填充源缓存空间P1_1的颜色值为0
//@EXECUTIONFLOW:8、调用CSUDIOSGSyn填充完成
//@EXECUTIONFLOW:9、调用CSUDIOSGBlit（P1_1,DR1,P2_1,SR1,M1），完成从YUV向ARGB模式的blit操作
//@EXECUTIONFLOW:10、调用CSUDIOSGSync完成blit，并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:11、调用CSUDIOSGGetSurfaceInfo获取缓存空间P1_1的信息D_Info
//@EXECUTIONFLOW:12、获取绘图平面D区域内的颜色值应为Cr1
//@EXECUTIONFLOW:13、调用CSUDIOSGDestroySurface销毁绘图平面
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
						"从ARGB向YUV模式的blit操作失败"
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
						"从YUV向ARGB模式的blit操作失败"
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
//@DESCRIPTION:测试16位和32位颜色模式之间的blit操作
//@PRECONDITION:平台UDI已经成功初始化
//@INPUT: P1_1+P2_1+X1+X2+SR1+DR1+M1
//@EXPECTATION: blit操作成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建缓存空间P1_1，颜色模式为X2，32位模式
//@EXECUTIONFLOW:2、调用CSUDIOSGCreateSurface创建缓存空间P2_1，颜色模式为X1，16位模式
//@EXECUTIONFLOW:3、调用CSUDIOSGFill填充缓存空间P1_1的颜色值为Cr1
//@EXECUTIONFLOW:4、调用CSUDIOSGSyn填充完成
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit（P2_1,DR1,P1_1,SR1,M1）,完成从32位向16模式的blit操作
//@EXECUTIONFLOW:6、调用CSUDIOSGSync完成blit，并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIOSGGetSurfaceInfo获取缓存空间P2_1的信息D_Info
//@EXECUTIONFLOW:8、获取绘图平面P2_1区域内的颜色值应为Cr1，16位模式
//@EXECUTIONFLOW:9、调用CSUDIOSGFill填充源缓存空间P1_1的颜色值为0
//@EXECUTIONFLOW:10、调用CSUDIOSGSyn填充完成
//@EXECUTIONFLOW:11、调用CSUDIOSGBlit（P1_1,DR1,P2_1,SR1,M1），完成从16向32位模式的blit操作
//@EXECUTIONFLOW:12、调用CSUDIOSGSync完成blit，并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIOSGGetSurfaceInfo获取缓存空间P1_1的信息D_Info
//@EXECUTIONFLOW:14、获取绘图平面P1_1区域内的颜色值应为Cr1，32位模式
//@EXECUTIONFLOW:15、调用CSUDIOSGDestroySurface销毁绘图平面
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
						"从32位向16模式的blit操作失败"
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
						"从16向32位模式的blit操作失败"
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
//@CASEGROUP: 1、CSUDIOSGCreateSurface
//@CASEGROUP: 2、CSUDIOSGDestroySurface
//@DESCRIPTION:测试能够创建平台提供给缓存空间的内存容量大小
//@PRECONDITION:平台UDI已经成功初始化
//@INPUT: 1、颜色模式X2或X1
//@INPUT: 2、宽度= 根据平台提供的内存容量计算
//@INPUT: 3、高度= 根据平台提供的内存容量计算
//@EXPECTATION: 能够创建出该大小的缓存空间
//@REMARK: 运行此测试用例会导致其它测试用例无法运行成功。
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建绘图平面P1_1
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取绘图平面P1_1的信息
//@EXECUTIONFLOW:3、获取的绘图平面的信息应该跟创建时传入的信息一致
CSUDI_BOOL CSTC_OSG_IT_0014(void)
{   
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDI_Error_Code nRet;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == OSG_iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"没有找到任何支持的满足要求的PixelFormat"
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
//@DESCRIPTION:测试两个缓存之间的blit操作的性能
//@PRECONDITION:平台UDI已经成功初始化
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
//@EXPECTATION: 打印出各个数据值的blit时间
//@EXECUTIONFLOW:无
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
//@DESCRIPTION:测试缓存和显存之间的blit操作的性能
//@PRECONDITION:平台UDI已经成功初始化
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
//@EXPECTATION: 打印出各个数据值的blit时间
//@EXECUTIONFLOW:无
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
//@DESCRIPTION:测试缓存的Fill操作的性能
//@PRECONDITION:平台UDI已经成功初始化
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
//@EXPECTATION: 打印出各个数据值的Fill时间
//@EXECUTIONFLOW:无
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
//@DESCRIPTION:测试显存的Fill操作的性能
//@PRECONDITION:平台UDI已经成功初始化
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
//@EXPECTATION: 打印出各个数据值的Fill时间
//@EXECUTIONFLOW:无
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
//@DESCRIPTION:测试两个缓存之间带alpha模式的blit操作的性能
//@PRECONDITION:平台UDI已经成功初始化
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
//@EXPECTATION: 打印出各个数据值的blit时间
//@EXECUTIONFLOW:设源和目的绘图平面alpha值为0x7f
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
//@DESCRIPTION:测试缓存和显存之间带alpha模式的blit操作的性能
//@PRECONDITION:平台UDI已经成功初始化
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
//@EXPECTATION: 打印出各个数据值的blit时间
//@EXECUTIONFLOW:设源绘图平面alpha值为0x7f
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
//@DESCRIPTION:测试两个缓存之间有放大方式的blit操作的性能
//@PRECONDITION:平台UDI已经成功初始化
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
//@EXPECTATION: 打印出各个数据值的blit时间
//@EXECUTIONFLOW:无
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
//@DESCRIPTION:测试两个缓存之间有缩小的blit操作的性能
//@PRECONDITION:平台UDI已经成功初始化
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
//@EXPECTATION: 打印出各个数据值的blit时间
//@EXECUTIONFLOW:无
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
//@DESCRIPTION:测试两个缓存之间有放大带alpha的blit操作的性能
//@PRECONDITION:平台UDI已经成功初始化
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
//@EXPECTATION: 打印出各个数据值的blit时间
//@EXECUTIONFLOW:设源绘图平面alpha值为0x7f
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
//@DESCRIPTION:测试两个缓存之间有缩小的带alpha的blit操作的性能
//@PRECONDITION:平台UDI已经成功初始化
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
//@EXPECTATION: 打印出各个数据值的blit时间
//@EXECUTIONFLOW:设源绘图平面alpha值为0x7f
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
//@DESCRIPTION:测试缓存和显存之间缩小的带alpha的blit操作的性能
//@PRECONDITION:平台UDI已经成功初始化
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
//@EXPECTATION: 打印出各个数据值的blit时间
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
//@DESCRIPTION:测试缓存和显存之间放大的带alpha的blit操作的性能
//@PRECONDITION:平台UDI已经成功初始化
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
//@EXPECTATION: 打印出各个数据值的blit时间
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
//@DESCRIPTION:测试绘图平面的pitch是否正确实现
//@PRECONDITION:平台UDI已经成功初始化
//@INPUT: 绘图平面的宽度=255,254,253，高度为255
//@EXPECTATION: 期望屏幕能够正常显示图片，不出现异常情况
//@EXECUTIONFLOW:1、分别创建绘图平面的宽度为255，254，253，高度为255的绘图平面
//@EXECUTIONFLOW:2、设置屏幕背景颜色为白色
//@EXECUTIONFLOW:3、然后往显存上进行blit操作，期望blit成功，并且图片显示没有异常
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
//@DESCRIPTION:  用红绿两种颜色交替相领刷新显存,期望不会丢失刷新操作
//@PRECONDITION: 平台UDI已经成功初始化
//@EXPECTATION: 用红绿两种颜色的矩形条交替刷新显存,如果在刷新过程中出现连续两种或以上的红色或绿色矩形条相邻,则测试失败
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
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄D
//@EXECUTIONFLOW:3、调用CSUDIOSGCreateSurface创建绘图平面P1_1，颜色模式为X2或X1,长720,宽是50
//@EXECUTIONFLOW:4、调用CSUDIOSGFill给绘图平面填充红色
//@EXECUTIONFLOW:5、调用CSUDIOSGCreateSurface创建绘图平面P1_2，颜色模式为X2或X1,长s720,宽是50
//@EXECUTIONFLOW:6、调用CSUDIOSGFill给绘图平面填充绿色
//@EXECUTIONFLOW:7、置循环值i为0
//@EXECUTIONFLOW:8、调用CSUDIOSGBlit（D,sSRect[i],P1_1,sSRect[i],EM_UDIOSG_OPMODE_COPY）,把绘图平面P1_1拷贝到显存中
//@EXECUTIONFLOW:9、置循环值i加1
//@EXECUTIONFLOW:10    调用CSUDIOSGBlit（D,sSRect[i],P1_2,sSRect[i],EM_UDIOSG_OPMODE_COPY）,把绘图平面P1_2拷贝到显存中
//@EXECUTIONFLOW:11    置循环值i加1
//@EXECUTIONFLOW:12    重复调用步骤8~步骤11共6次
//@EXECUTIONFLOW:13    调用CSUDIOSGSync进行同步
//@EXECUTIONFLOW:14    重复调用步骤7~步骤13共50次,每次调用时把步骤8和步骤10顺序对调
//@EXECUTIONFLOW:15    清屏,调用CSUDIOSGDestroySurface销毁绘图平面
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
				"没有找到任何支持的满足要求的PixelFormat"
			);

	CSTK_ASSERT_TRUE_FATAL(
		                     (CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(OSG_GetTestDisplaySurfaceIndex(), &hDisplay) && (CSUDI_NULL != hDisplay)),
		                     "创建显存失败"
		                  );
	CSTK_ASSERT_TRUE_FATAL(
		                    (CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hDisplay, &hSurfaceInfo)), 
		                     "获取存信息失败"
		                  );
	CSTK_ASSERT_TRUE_FATAL(
		                    (CSUDI_SUCCESS == CSUDIOSGCreateSurface( ePixelFormat, 720, 30, &phSurface1)),
		                    "创建平面失败"
		                   );
	CSTK_ASSERT_TRUE_FATAL(
		                    (CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(phSurface1, &sSurfaceInfo1)), 
		                    "获取存信息失败"
		                   );
	CSTK_ASSERT_TRUE_FATAL(
		                     (CSUDI_SUCCESS == CSUDIOSGCreateSurface( ePixelFormat, 720, 30, &phSurface2)),
		                     "创建平面失败"
		                    );
	CSTK_ASSERT_TRUE_FATAL(
		                     (CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(phSurface2, &sSurfaceInfo2)), 
		                     "获取存信息失败"
		                    );
       CSTK_ASSERT_TRUE_FATAL(
		   	                     (CSUDI_SUCCESS == CSUDIOSGFill(phSurface1, CSUDI_NULL, nColor1)),
	   	                     "Fill Failure !"
	   	                    );
	CSTK_ASSERT_TRUE_FATAL(
			                     (CSUDI_SUCCESS == CSUDIOSGFill(phSurface2, CSUDI_NULL, nColor2)),
		                     "Fill Failure !"
		                    );

	CSTCPrint("请注意屏幕在刷新过程中是否会出现连续两个或以上的红色(或绿色)矩形条相邻,每个矩形条大小为 %d*%d\n", 
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
						"拷贝绘图平面phSurface1到显存失败" );
					i++;
					CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGBlit(hDisplay,&sSRect[i],phSurface2,CSUDI_NULL,EM_UDIOSG_OPMODE_COPY)),
						"拷贝绘图平面phSurface2到显存失败");
					i++; 
					break;
				}
		            case 1:
				{
					CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGBlit(hDisplay,&sSRect[i],phSurface2,CSUDI_NULL,EM_UDIOSG_OPMODE_COPY)),
						"拷贝绘图平面phSurface2到显存失败" );
					i++;
					CSTK_ASSERT_TRUE_FATAL( (CSUDI_SUCCESS == CSUDIOSGBlit(hDisplay,&sSRect[i],phSurface1,CSUDI_NULL,EM_UDIOSG_OPMODE_COPY)),
                                         "拷贝绘图平面phSurface1到显存失败" );
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
	
	CSTCPrint(" 期望没有在刷新过程中出现连续两个或以上的红色(或绿色)矩形条相邻\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), " result failure");
	
	CSTK_FATAL_POINT
		
	if(phSurface1 != CSUDI_NULL)
	{
		if(CSUDIOSGDestroySurface(phSurface1)!=CSUDI_SUCCESS)
		{
			CSTCPrint("销毁绘图平面1失败\n"); 	
		}
		
		phSurface1 = CSUDI_NULL;
	}
	if(phSurface2 != CSUDI_NULL)
	{
		if(CSUDIOSGDestroySurface(phSurface2)!=CSUDI_SUCCESS)
		{
			CSTCPrint("销毁绘图平面2失败"); 	
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
//@DESCRIPTION:  使用图片多次刷新显存,观察界面是否抖动
//@PRECONDITION: 平台UDI已经成功初始化
//@EXPECTATION: 用数据直接blit到显存，观察界面是否抖动。
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取显存空间句柄D
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage把搜索界面的图片解码
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit把图片Blit到显存中
//@EXECUTIONFLOW:4、提示用户确认，图片是否显示正常
//@EXECUTIONFLOW:4、调用CSUDIOSGDestroySurface销毁图片绘图平面
CSUDI_BOOL CSTC_OSG_IT_0029(void)
{
	OSG_iTestDecImageToDisplay(EM_OSG_DECIMAGE_JPG,EM_JPG_720_576);
	OSG_iClearScreen();
    return CSUDI_TRUE;
}

//@CASEGROUP: CSTC_OSG_IT_0030
//@DESCRIPTION: JPG图片解码后的ALPHA值应为0xFF,图片可以直接通过memcpy方式到显存
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、平台支持JPG类型的图片解码
//@INPUT:1、图片UDI2OSG_720_576.jpg的数据地址
//@INPUT:2、图片UDI2OSG_720_576.jpg的长度
//@EXPECTATION: 1、能够成功对图片进行解码，返回CSUDI_SUCCESS；
//@EXPECTATION: 2、解码得到的数据可以通过memcpy的方式拷贝到显存中,并能正确显示
//@REMARK: 
//@EXECUTIONFLOW:1、获取UDI2OSG_720_576.jpg图片数据的内存地址和长度
//@EXECUTIONFLOW:2、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDI_SUCCESS，解码得到的phSurface应该不为CSUDI_NULL
//@EXECUTIONFLOW:3、调用CSUDIOSGGetSurfaceInfo获取解码后的图片内存和属性
//@EXECUTIONFLOW:4、调用CSUDIOSGGetDisplaySurface接口获取显存空间hDisSurface句柄
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取显存地址和属性
//@EXECUTIONFLOW:6、调用memcpy的方式,把图片拷贝到显存中
//@EXECUTIONFLOW:7、应该在屏幕上看到UDI2OSG_720_576.jpg图片的效果，如果看不到，则测试用例失败。
//@EXECUTIONFLOW:8、清屏,销毁surface,释放掉UDI2OSG_720_576.jpg占用的内存。
CSUDI_BOOL CSTC_OSG_IT_0030(void)
{
	CSUDI_BOOL bRet = CSUDI_TRUE;
	
	bRet &= OSG_iTestMemcpyImageToDisplay(EM_OSG_DECIMAGE_IFRAME,EM_IFRAME_720_576);
	
	bRet &= OSG_iTestMemcpyImageToDisplay(EM_OSG_DECIMAGE_JPG,EM_JPG_720_576);
	return bRet;
}

//@CASEGROUP: CSTC_OSG_IT_0031
//@DESCRIPTION: YUV图片可以顺利转换成RGB图片,并可以正常显示
//@PRECONDITION: 1、平台已成功初始化
//@INPUT:1、图片"fastjpegYUVData写文件"的数据地址
//@EXPECTATION: 1、YUV图片可以转换成RGB图片,并能正确显示
//@REMARK: 
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建EM_UDIOSG_PIXEL_FORMAT_YCBCR888_422 格式的,宽高为 816*612 的YUV Surface1
//@EXECUTIONFLOW:3、调用CSUDIOSGCreateSurface创建EM_UDIOSG_PIXEL_FORMAT_ARGB_8888格式,宽高为 816*612 的RGB Surface2
//@EXECUTIONFLOW:5、调用CSUDIOSGGetDisplaySurface,获取显存句柄SurcaceDisplay
//@EXECUTIONFLOW:2、通过文件读写接口把”fastjpegYUVData“中的数据读到 上面创建的YUV Surface对应的内存中
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit,以EM_UDIOSG_OPMODE_COPY模式,把Surface1 上的内容Copy到Surface2上
//@EXECUTIONFLOW:6、调用CSUDIOSGBlit,以EM_UDIOSG_OPMODE_COPY模式,把Surface2 上的内容Copy到SurcaceDisplay上
//@EXECUTIONFLOW:7、期望屏幕上能看到与"fastjpgTestImage.jpg"一样的图片显示
//@EXECUTIONFLOW:8、清屏,销毁surface,释放掉"fastjpegYUVData"占用的内存。
CSUDI_BOOL CSTC_OSG_IT_0031(void)
{
	return OSG_iTestYUVtoRGB(EM_UDIOSG_PIXEL_FORMAT_YCBCR888_422,EM_YUV_FASTJPG_DATA);
}

//@CASEGROUP: CSTC_OSG_IT_0032
//@DESCRIPTION: 测试调用CSUDIOSGFill填充高度为1个像素的矩形
//@PRECONDITION:平台已成功初始化
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface得到显存surface 句柄
//@EXECUTIONFLOW:2、调用CSUDIOSGCreateSurface创建绘图平面hDrawSurface用于显示高度为1的矩形
//@EXECUTIONFLOW:3、调用CSUDIOSGCreateSurface创建临时绘图平面hTempSurface高度为1的矩形用于备份
//@EXECUTIONFLOW:4、调用CSUDIOSGFill为hDrawSurface填充白色背景
//@EXECUTIONFLOW:5、将hDrawSurface中高度为1的矩形的内容备份到hTempSurface
//@EXECUTIONFLOW:6、调用CSUDIOSGSync同步
//@EXECUTIONFLOW:7、调用CSUDIOSGFill将hDrawSurface中的矩形填充为绿色
//@EXECUTIONFLOW:8、调用CSUDIOSGSync同步
//@EXECUTIONFLOW:9、调用CSUDIOSGBlit，将hDrawSurface复制到显存上，全屏显示
//@EXECUTIONFLOW:10、调用CSUDIOSGSync同步
//@EXECUTIONFLOW:11、期望屏幕中白色背景中显示一条绿色的线(高度为1的矩形框)
//@EXECUTIONFLOW:12、调用CSUDIOSGBlit将hTempSurface的内容拷贝到hDrawSurface的矩形区域上，恢复hDrawSurface
//@EXECUTIONFLOW:13、调用CSUDIOSGSync同步
//@EXECUTIONFLOW:14、调用CSUDIOSGBlit将hDrawSurface全屏显示在显存上
//@EXECUTIONFLOW:15、期望全屏显示白色，绿色线条消失
//@EXECUTIONFLOW:16、清屏,调用CSUDIOSGDestroySurface销毁绘图平面
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
				"没有找到任何支持的满足要求的PixelFormat"
			);

	CSUDIOSGGetDisplaySurface(OSG_GetTestDisplaySurfaceIndex(), &hDisplaySurface); 
	
	 //创建临时SURFACE，用于显示高度为1的矩形
	CSTK_ASSERT_TRUE_FATAL(
							(CSUDI_SUCCESS ==CSUDIOSGCreateSurface(ePixelFormat, 720, 576, &hDrawSurface)),
							"create hDrawSurface failure"); 

	//创建临时SURFACE，备份hDrawSurface在未填充颜色之前的内容 
	CSTK_ASSERT_TRUE_FATAL(
							(CSUDI_SUCCESS ==CSUDIOSGCreateSurface(ePixelFormat, 300, 1, &hTempSurface)),
							 "create hTempSurface failure");  
	// 填充白色背景
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

	// 备份缓存
	CSUDIOSGBlit(hTempSurface, NULL, hDrawSurface, &draw_rect, EM_UDIOSG_OPMODE_COPY);
	CSUDIOSGSync();
	
	// 填充缓存
	CSUDIOSGFill(hDrawSurface, &draw_rect,OSG_COLOR_RED_32BIT);
	CSUDIOSGSync();
	
	// 显示缓存 全屏
	CSUDIOSGBlit(hDisplaySurface, NULL, hDrawSurface, NULL, EM_UDIOSG_OPMODE_COPY);
	CSUDIOSGSync();
	CSTCPrint(" 期望白色背景中显示出一条红色的线(高度为1)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), " Wait failure");
	
	// 恢复缓存
	CSUDIOSGBlit( hDrawSurface, &draw_rect, hTempSurface, NULL, EM_UDIOSG_OPMODE_COPY);
	CSUDIOSGSync();
	
	CSUDIOSGBlit(hDisplaySurface, NULL , hDrawSurface, NULL, EM_UDIOSG_OPMODE_COPY);
	CSUDIOSGSync();  
	CSTCPrint(" 期望全屏显示白色，红色线条消失\n");
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
//@DESCRIPTION: 测试调用CSUDIOSGBlit模拟2D特效的move效果
//@PRECONDITION:平台已成功初始化
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface得到显存surface 句柄
//@EXECUTIONFLOW:2、调用CSUDIOSGCreateSurface创建一个标清全屏大小的绘图平面hBgSurface
//@EXECUTIONFLOW:3、调用CSUDIOSGCreateSurface创建一个100 * 100大小的hForeSurface
//@EXECUTIONFLOW:4、调用CSUDIOSGFill为hForeSurface填充0xFFFF0000背景
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit将显存的内容备份到hbgsurface
//@EXECUTIONFLOW:6、调用CSUDIOSGBlit将hForeSurface的内容blit到显存
//@EXECUTIONFLOW:7、调用CSUDIOSGBlit恢复背景的上半部分区域内容。
//@EXECUTIONFLOW:8、调用CSUDIOSGBlit恢复背景的下半部分区域内容。
//@EXECUTIONFLOW:9、调用CSUDIOSGSync同步
//@EXECUTIONFLOW:11、循环执行上面的步骤100次，统计性能
//@EXECUTIONFLOW:12、调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_0033(void)
{
	CSUDIOSGOpMode_E eOpMode = EM_UDIOSG_OPMODE_COPY;
	unsigned int	 unForeground_color = 0xFFFF0000;
	
	return OSG_iTESTPortalPerformance(eOpMode, unForeground_color);
}

//@CASEGROUP: CSTC_OSG_IT_0034
//@DESCRIPTION: 模拟跑马灯的测试用例
//@PRECONDITION:平台已成功初始化
//@EXECUTIONFLOW:1、读取跑马灯字体的bmp图片
//@EXECUTIONFLOW:2、调用CSUDIOSGCreateSurface创建一个hfontsurface，将bmp的数据填充到hfontsurface中。
//@EXECUTIONFLOW:3、调用CSUDIOSGCreateSurface创建一个4000 * 50的hsurface_S。
//@EXECUTIONFLOW:4、调用CSUDIOSGFill为4000 * 50的hsurface_S填充白色背景
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit，将hfontsurface的数据blit到4000*50的hsurface_S上。
//@EXECUTIONFLOW:6、调用CSUDIOSGGetDisplaySurface得到显存hDispsurface 句柄。
//@EXECUTIONFLOW:7、调用CSUDIOSGBlit，将hsurface_S的数据blit到hDispsurface上。
//@EXECUTIONFLOW:8、调用CSUDIOSGSync同步
//@EXECUTIONFLOW:9、改变第八步blit的目的偏移坐标，重复4~8步
//@EXECUTIONFLOW:10、清屏,调用CSUDIOSGDestroySurface销毁绘图平面、释放图片资源
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

		if(i > 0xFFFFFF00)//防止整形溢出
		{
			i = 0;
		}
		CSUDIOSThreadSleep(3);
	};

	g_Thread++;

	CSTCPrint(" 空转线程自动退出\n");
}

void OSG_TestThreadEntry2(void * pvParam)
{
	CSUDIOSGOpMode_E eOpMode = EM_UDIOSG_OPMODE_COPY;
	unsigned int	 unForeground_color = 0xFFFF0000;
	int i;
	
	CSTCPrint(" OSG线程进入\n");
	for(i = 0; i< 10; i++)
	{
		if(g_Thread == 0)
		{
			OSG_iTESTPortalPerformance(eOpMode, unForeground_color);	
		}		
	}
	
	g_Thread++;

	CSTCPrint(" OSG线程自动退出\n");
}

//@CASEGROUP: CSTC_OSG_IT_0035
//@DESCRIPTION:测试当有其他的优先级高的线程占用大量cpu的时候，osg绘图是否被阻塞。
//@PRECONDITION:平台已成功初始化
//@EXPECTATION:执行的move特效不被阻塞，从屏幕的左上方移动到右下方。
//@EXECUTIONFLOW:1、设置线程的优先级为255， 调用CSUDIOSThreadCreate创建任务OS_TestThread1并返回CSUDI_SUCCESS,否则测试用例失败
//@EXECUTIONFLOW:2、设置线程的优先级为255， 调用CSUDIOSThreadCreate创建任务OS_TestThread2并返回CSUDI_SUCCESS,否则测试用例失败
//@EXECUTIONFLOW:3、设置线程的优先级为255， 调用CSUDIOSThreadCreate创建任务OS_TestThread3并返回CSUDI_SUCCESS,否则测试用例失败
//@EXECUTIONFLOW:4、设置线程的优先级为7， 调用CSUDIOSThreadCreate创建任务OS_TestThreadosg并返回CSUDI_SUCCESS,否则测试用例失败
//@EXECUTIONFLOW:5、任务OS_TestThread1、OS_TestThread2、OS_TestThread3，执行自加运算。调用CSUDIOSThreadSleep 等待1ms，然后当g_bThreadExit为TRUE时退出线程。
//@EXECUTIONFLOW:6、任务OS_TestThreadosg，执行10次move特效,每次间隔5毫秒。当g_bThreadExit为TRUE时退出线程。
//@EXECUTIONFLOW:7、在主任务中调用CSUDIOSThreadSleep 等待5秒，设置g_bThreadExit为TRUE。
//@EXECUTIONFLOW:8、在主任务中调用CSUDIOSThreadDestroy删除创建的任务并返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:9、特效执行完毕，且osg线程正常退出，返回CSUDI_SUCCESS。
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

	CSTCPrint("请注意屏幕上红色小方块的移动特效是否流畅\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OSG_TestThread1", nPriorityHigh , nStckSize, 
							OSG_TestThreadEntry1, &nParam, &hThreadhandle1),
			 "step 1 创建任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle1,"step 1 创建成功的任务输出的任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OSG_TestThread2", nPriorityHigh , nStckSize, 
							OSG_TestThreadEntry1, &nParam, &hThreadhandle2),
			 "step 2 创建任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle2,"step 2 创建成功的任务输出的任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OSG_TestThread3", nPriorityHigh, nStckSize, 
							OSG_TestThreadEntry1, &nParam, &hThreadhandle3),
			 "step 3 创建任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle3,"step 3 创建成功的任务输出的任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OSG_TestThread4", nPriorityLow , nStckSize, 
							OSG_TestThreadEntry2, &nParam, &hThreadhandleosg),
			 "step 4 创建任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandleosg,"step 4 创建成功的任务输出的任务句柄不能为空");

	CSUDIOSThreadSleep(8000);

	g_Thread++;	
	//等待线程自动退出
	while(1)
	{
		if(g_Thread == 5)
		{
			break;
		}
		CSUDIOSThreadSleep(100);
	}	

	CSTCPrint("红色小方块的移动特效是否流畅?\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()),"2D特效受其他线程影响严重!");	

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
//@DESCRIPTION: 测试osg是否有严重的偏移，超过屏幕范围等(基本思路画回字形的框，如果框完整则说明没有偏移)
//@PRECONDITION:平台已成功初始化
//@EXECUTIONFLOW:1、获取标清的显存。
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取标清显存的信息。
//@EXECUTIONFLOW:3、调用CSUDIOSGFill标清显存大小的一个矩形(红色)。
//@EXECUTIONFLOW:4、将上面的矩形四个方向内缩一个像素，调用CSUDIOSGFill填充此矩形(白色)。
//@EXECUTIONFLOW:5、将上面的矩形四个方向内缩一个像素，调用CSUDIOSGFill填充此矩形(红色)。
//@EXECUTIONFLOW:6、循环直到矩形的高度为1。
//@EXECUTIONFLOW:7、调用CSUDIOSGBlit，将hsurface_S的数据blit到hDispsurface上。
//@EXECUTIONFLOW:8、调用CSUDIOSGSync同步
//@EXECUTIONFLOW:10 、清屏,调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_0036(void)
{	
	return OSG_iTestScreenOffset();
}

//@CASEGROUP: CSTC_OSG_IT_0037
//@DESCRIPTION: 测试有拉伸的时候，blit到显存是否有严重的阶梯状锯齿(两幅图片轮流交换到显存)。
//@PRECONDITION:平台已成功初始化
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建两个1280 * 720的surface1, surface2
//@EXECUTIONFLOW:2、向surface1中填充search.bmp数据，向surface2中填充avsetting.bmp数据
//@EXECUTIONFLOW:3、调用CSUDIOSGBlit，将surface1的数据，blit到标清显存，surface1的绘图高度设置为600
//@EXECUTIONFLOW:4、调用CSUDIOSGSync同步
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit，将surface2的数据，blit到标清显存，surface2的绘图高度设置为600
//@EXECUTIONFLOW:6、调用CSUDIOSGSync同步
//@EXECUTIONFLOW:7、调用CSUDIOSGBlit，将surface1的数据，blit到高清显存，surface1的绘图高度设置为600
//@EXECUTIONFLOW:8、调用CSUDIOSGSync同步
//@EXECUTIONFLOW:9、调用CSUDIOSGBlit，将surface2的数据，blit到高清显存，surface2的绘图高度设置为600
//@EXECUTIONFLOW:10、调用CSUDIOSGSync同步
//@EXECUTIONFLOW:11、分别循环上面的步骤10次，观察屏幕是否有严重的阶梯状绘图
//@EXECUTIONFLOW:12、销毁读取缓冲区的原始图片数据
//@EXECUTIONFLOW:13 、清屏,调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_0037(void)
{
	return OSG_iTestScreenSerrate();
}

//@CASEGROUP: CSTC_OSG_IT_0038
//@DESCRIPTION: 测试到显存的局部刷新是否存在丢失数据的情况(基本思想是将屏幕分成16*16的方块，逐个方块交换到显存)
//@PRECONDITION:平台已成功初始化
//@EXECUTIONFLOW:1、调用CSUDIOSGCreateSurface创建一个1280 * 720的surface
//@EXECUTIONFLOW:2、读取search.bmp数据，并将数据填充到surface中
//@EXECUTIONFLOW:3、调用CSUDIOSGFill标清显存为红色
//@EXECUTIONFLOW:4、调用CSUDIOSGBlit，将surface的数据，blit到标清显存，数据的交换区域为(0,0,15,15)
//@EXECUTIONFLOW:5、调用CSUDIOSGBlit，将surface的数据，blit到标清显存，数据的交换区域为(16,0,31,0)
//@EXECUTIONFLOW:6、调用CSUDIOSGBlit，将surface的数据，blit到标清显存，数据的交换区域为(32,0,47,0)
//@EXECUTIONFLOW:7、按照从左到右，从上到下，依次调用CSUDIOSGBlit直到交换区域到达屏幕的右下角
//@EXECUTIONFLOW:8、观察屏幕上是否有红色的方块残留
//@EXECUTIONFLOW:9、销毁读取缓冲区的原始图片数据
//@EXECUTIONFLOW:10 、清屏,调用CSUDIOSGDestroySurface销毁绘图平面
CSUDI_BOOL CSTC_OSG_IT_0038(void)
{	
	return OSG_iTestScreenlost();
}

//@CASEGROUP: CSTC_OSG_IT_Resolution_0001
//@DESCRIPTION: 高标清分辨率连续多次切换后OSG是否会产生异常
//@PRECONDITION:平台已成功初始化
//@EXECUTIONFLOW:1、调用CSUDISCREENGetResolution获取当前屏幕分辨率；
//@EXECUTIONFLOW:2、调用CSUDIOSGGetDisplaySurface获取高标清surface；
//@EXECUTIONFLOW:3、调用CSUDIOSGGetSurfaceInfo获取高标清surface的信息；
//@EXECUTIONFLOW:4、调用CSUDIOSGCreateSurface创建临时surface；
//@EXECUTIONFLOW:5、调用CSUDIOSGGetSurfaceInfo获取临时surface的信息；
//@EXECUTIONFLOW:6、调用CSUDIOSGSetSurfaceInfo设置设置临时surface的内容
//@EXECUTIONFLOW:7、调用CSUDIOSGBlit将临时surface的内容显示到高标清surface上；
//@EXECUTIONFLOW:8、调用CSUDIOSGSync同步；
//@EXECUTIONFLOW:9、调用CSUDIOSGDestroySurface销毁临时surface；
//@EXECUTIONFLOW:10、多次调用CSUDISCREENSetResolution切换分辨率；
//@EXECUTIONFLOW:11、对显示结果进行判断；
//@EXECUTIONFLOW:12、调用CSUDISCREENSetResolution恢复分辨率；
//@EXECUTIONFLOW:13 、清屏；
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
			CSTCPrint("高标清同源，只测试高清显存\n");
		}
		else
		{
			nMinDisplayId = 0;
			nMaxDisplayId = 1;
			CSTCPrint("高标清不同源，分别测试高清和标清显存\n");
		}
	}
	else
	{
		nMinDisplayId = 0;
		nMaxDisplayId = 0;
		CSTCPrint("标清平台，只测试标清显存\n");
	}

	for (nIsHD=nMinDisplayId; nIsHD<=nMaxDisplayId; nIsHD++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[nIsHD], &aeOldResolution[nIsHD]),
			"step 1 获取屏幕分辨率失败!");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(nIsHD, &hDisplaySurface), 
			"step 2 获取显示设备失败!");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIOSGGetSurfaceInfo(hDisplaySurface, &sSurfaceInfo),
			"step 3 获取显示设备surface信息失败!");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGCreateSurface(sSurfaceInfo.m_ePixelFormat, 1280, 720, &ahTmpSurface[nIsHD]),
			"step 4 创建surface失败!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != ahTmpSurface[nIsHD],
			"step 4 创建surface失败!");
			
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIOSGGetSurfaceInfo(ahTmpSurface[nIsHD], &sSurfaceInfo),
			"step 5 获取显示设备surface信息失败!");
		
		unImageSize = OSG_iGetPhoto("UDI2OSG_search.bmp", &ppImage);
		CSTK_ASSERT_TRUE_FATAL(ppImage  != CSUDI_NULL, "step 8 获取测试图片数据失败!");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iSetSurfaceBuf(ahTmpSurface[nIsHD], ppImage,unImageSize, 1280, 720),
			"step 5 设置surface的内容失败!");

		CSUDIOSFree(ppImage);
		ppImage = NULL;
			
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGBlit(hDisplaySurface, NULL, ahTmpSurface[nIsHD], NULL, EM_UDIOSG_OPMODE_COPY),
			"step 7 Blit surface失败!");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGSync(), 
			"step 8 同步surface失败!");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (error_code = CSUDIOSGDestroySurface(ahTmpSurface[nIsHD])), 
			"step 9 销毁临时surface失败!");		

		ahTmpSurface[nIsHD] = CSUDI_NULL;

		CSTCPrint("下面将开始切换分辨率(共10次)\n");

		switch(nIsHD)
		{
			case 0:
				CSTCPrint("请注意观察标清输出在分辨率切换时图片显示是否正常\n");
				break;
			case 1:
				CSTCPrint("请注意观察高清输出在分辨率切换时图片显示是否正常\n");
				break;
			default:
				CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "显存索引错误");
				break;
		}

		CSTKWaitAnyKey( );
		
		scResolution[0][0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[0][1] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1][0]  = EM_UDISCREEN_RESOLUTION_720P_50HZ;
		scResolution[1][1]  = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
		
		for(i = 0; i < 9; i ++)
		{
			CSTCPrint("第%d次切换分辨率: %d(%d:PAL, %d:NTSC, %d:720P_50HZ, %d:1080I_50HZ)! \n", i+1, scResolution[nIsHD][i%2], \
				EM_UDISCREEN_RESOLUTION_PAL, EM_UDISCREEN_RESOLUTION_NTSC, EM_UDISCREEN_RESOLUTION_720P_50HZ, EM_UDISCREEN_RESOLUTION_1080I_50HZ);
			CSTCPrint("Now change resolution! \n");
			CSTKWaitAnyKey( );
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[nIsHD], &scResolution[nIsHD][i%2] , 1),
				"step 10 设置屏幕分辨率失败!");

			CSTCPrint("图片是否能正常显示? \"y\" 正常, \"n\"存在异常.\n");
			CSTCPrint("Whether osg is normal? \"y\" for normal, \"n\"if not.\n");
			if (nIsHD == 0)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == (bRet = CSTKWaitYes( )),
					"step 11 标清分辨率连续多次切换后OSG产生异常");
			}
			else
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == (bRet = CSTKWaitYes( )),
					"step 11 高清分辨率连续多次切换后OSG产生异常");
			}
		}
	}

	CSTK_FATAL_POINT;

	for (nIsHD=nMinDisplayId; nIsHD<=nMaxDisplayId; nIsHD++)
	{
		if(ahTmpSurface[nIsHD] != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGDestroySurface(ahTmpSurface[nIsHD]), 
				"销毁临时surface失败!");	
			ahTmpSurface[nIsHD] = CSUDI_NULL;
		}
				
		//恢复测试前分辨率
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[nIsHD], &aeOldResolution[nIsHD], 1),
			"step 12 恢复屏幕分辨率失败!");
	}

	OSG_iClearScreen();
	
	return bRet;
}

//@CASEGROUP: CSTC_OSG_IT_POWERON_SCENE_0001
//@DESCRIPTION: 开机进入portal场景
//@PRECONDITION:平台已成功初始化
//@EXECUTIONFLOW:1、调用CSUDISCREENGetResolution获取当前屏幕分辨率；
//@EXECUTIONFLOW:2、调用CSUDIOSGGetDisplaySurface获取高标清surface；
//@EXECUTIONFLOW:3、调用CSUDIOSGGetSurfaceInfo获取高标清surface的信息；
//@EXECUTIONFLOW:4、调用CSUDISCREENSetResolution设置标清分辨率；
//@EXECUTIONFLOW:5、调用CSUDIOSGCreateSurface创建一个1280X720的hFrameSurface；
//@EXECUTIONFLOW:6、调用CSUDIOSGFill填充hFrameSurface为黑色；
//@EXECUTIONFLOW:7、调用CSUDIOSGSync同步；
//@EXECUTIONFLOW:8、调用CSUDIOSGBlit将hFrameSurface的内容显示到高标清surface上；
//@EXECUTIONFLOW:9、调用CSUDIOSGSync同步；
//@EXECUTIONFLOW:10、调用CSUDISCREENSetOSDTransparency设置透明度为90；
//@EXECUTIONFLOW:11、再次调用CSUDIOSGBlit将hFrameSurface的内容显示到高标清surface上；
//@EXECUTIONFLOW:12、调用CSUDIOSGSync同步;
//@EXECUTIONFLOW:13、调用CSUDIOSGCreateSurface创建一个1280X720的hBgSurface;
//@EXECUTIONFLOW:14、调用CSUDIOSGGetSurfaceInfo获取hBgSurface的信息；
//@EXECUTIONFLOW:15、调用CSUDIOSGSetSurfaceInfo设置设置hBgSurface的内容;
//@EXECUTIONFLOW:16、调用CSUDIOSGBlit将hBgSurface的内容blit到hFrameSurface；
//@EXECUTIONFLOW:17、调用CSUDIOSGSync同步；
//@EXECUTIONFLOW:18、调用CSUDIOSGDestroySurface销毁临时hBgSurface；
//@EXECUTIONFLOW:19、调用CSUDIOSGCreateSurface创建115X110的hMapSurface；
//@EXECUTIONFLOW:20、调用CSUDIOSGGetSurfaceInfo获取hMapSurface的信息；
//@EXECUTIONFLOW:21、调用CSUDIOSGSetSurfaceInfo设置设置hMapSurface的内容;
//@EXECUTIONFLOW:22、调用CSUDIOSGBlit，以alpha模式将hMapSurface的内容blit 到hFrameSurface；
//@EXECUTIONFLOW:23、调用CSUDIOSGSync同步；
//@EXECUTIONFLOW:24、调用CSUDIOSGDestroySurface销毁115X110的hMapSurface；
//@EXECUTIONFLOW:25、重复18~23步骤7次
//@EXECUTIONFLOW:26、调用CSUDIOSGBlit将hFrameSurface的内容显示到高标清surface上；
//@EXECUTIONFLOW:27、调用CSUDIOSGSync同步；
//@EXECUTIONFLOW:28、确定显示结果；
//@EXECUTIONFLOW:29、调用CSUDIOSGDestroySurface销毁1280X720的hFrameSurface；
//@EXECUTIONFLOW:30、调用CSUDISCREENSetResolution恢复分辨率；
//@EXECUTIONFLOW:31、清屏；
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
		"step 1 获取屏幕分辨率失败!");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(nIndex, &hDisplaySurface),
		"step 2 获取显示surface失败!");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIOSGGetSurfaceInfo(hDisplaySurface, &sDisplaySurfaceInfo),
		"step 3 获取显示surface信息失败!");

	//标清分辨率
	if(nIndex == 0)
	{
		scResolution = EM_UDISCREEN_RESOLUTION_PAL;
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&eScreenDevice[nIndex], &scResolution, 1),
		"step 4 设置分辨率失败!");

	if(bIsHD)
    {
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(1, &hDisplay_HD),
            		"step 3 获取显示surface失败!!");
    }
    
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGCreateSurface(sDisplaySurfaceInfo.m_ePixelFormat, sBgRect.m_nWidth, sBgRect.m_nHeight, &hFrameSurface),
		"step 5 创建surface失败!");
	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iFillAndSync(hFrameSurface, &sBgRect, 0),
		"step 6 填充surface失败!");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hDisplaySurface, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
		"step 8 显示surface失败!");

	if(bIsHD)
    {
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hDisplay_HD, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
            		"step 3 获取显示surface失败!!");
    }

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetOSDTransparency(90),
		"step 10 设置OSD 透明度失败!");
	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hDisplaySurface, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
		"step 11 显示surface失败!");
		
    if(bIsHD)
    {
        CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hDisplay_HD, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
            "step 3 显示surface!!");
    }

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGCreateSurface(sDisplaySurfaceInfo.m_ePixelFormat, sBgRect.m_nWidth, sBgRect.m_nHeight, &hBgSurface),
		"step 13 创建surface失败!");

	unImageSize = OSG_iGetPhoto("UDI2OSG_portal_bg_1280_720_32.bmp", &ppImage);
	CSTK_ASSERT_TRUE_FATAL(ppImage  != CSUDI_NULL, "step 14 获取测试图片数据失败!");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iSetSurfaceBuf(hBgSurface, ppImage,unImageSize, 1280, 720),
		"step 14 设置surface的内容失败!");

	CSUDIOSFree(ppImage);
	ppImage = NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hFrameSurface, NULL, hBgSurface, NULL, EM_UDIOSG_OPMODE_COPY),
		"step 16 显示surface失败!");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hBgSurface),
		"step 18 销毁surface 失败!");

	hBgSurface = NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hDisplaySurface, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
		"step 16 显示surface失败!");
		
    if(bIsHD)
    {
        CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hDisplay_HD, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
            "step 3 显示surface!!");
    
    }

	CSTCPrint("接下来你将看到屏幕上显示一组图片! \n");
	CSTCPrint("Next you can see some pictures display on the screen! \n");
	CSTKWaitAnyKey();
	
	sMapRect[0].m_nX = 80;
	sMapRect[0].m_nY = 45;
	sMapRect[0].m_nWidth = 115;
	sMapRect[0].m_nHeight = 110;
	for(i = 0; i < 7; i ++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGCreateSurface(sDisplaySurfaceInfo.m_ePixelFormat, 115, 110, &hMapSurface),
			"step 19 创建hMapSurface失败!");

		unImageSize = OSG_iGetPhoto(pcNvFile[i], &ppImage);
		CSTK_ASSERT_TRUE_FATAL(ppImage  != CSUDI_NULL, "step 20 获取测试图片数据失败!");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iSetSurfaceBuf(hMapSurface, ppImage,unImageSize, 115, 110),
			"step 20 设置surface的内容失败!");

		CSUDIOSFree(ppImage);
		ppImage = NULL;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hFrameSurface, &sMapRect[i], hMapSurface, NULL, EM_UDIOSG_OPMODE_ALPHA),
			"step 22 blit失败!");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hMapSurface),
			"step 24 销毁hMapSurface失败!");

		hMapSurface = CSUDI_NULL;

		if(i < 6)
		{
            //这些参数只是在标清电视上调试过
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
		"step 26 blit失败!");
    if(bIsHD)
    {
        CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hDisplay_HD, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
            "step 3 blit失败!!!");
    }

	CSTCPrint("图片是否能正常显示? \"y\" 正常, \"n\"存在异常.\n");
	CSTCPrint("Whether the pictures show  normal? \"y\" for normal, \"n\"if not.\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== (bRet = CSTKWaitYes( )),
		"step 28 portal图片显示不正常!");

	CSTK_FATAL_POINT
		
	if(hFrameSurface != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hFrameSurface),
			"step 29 销毁hFrameSurface失败!");
	}

	if(hBgSurface!= CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hBgSurface),
			"销毁hBgSurface失败!");
	}

	if(hMapSurface != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hMapSurface),
			"销毁hMapSurface失败!");
	}

	if(ppImage != NULL)
	{
		CSUDIOSFree(ppImage);
	}

	//恢复测试前分辨率
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&eScreenDevice[nIndex], &scOldResolution, 1),
		"step 30 恢复屏幕分辨率失败!");
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetOSDTransparency(100),
        	"设置OSD 透明度失败!");

	OSG_iClearScreen();
	
	return bRet;
}

//@CASEGROUP: CSTC_OSG_IT_GLOBAL_EFFECT_0001
//@DESCRIPTION: 全局缩放特效场景
//@PRECONDITION:平台已成功初始化
//@INPUT:两张1280*720的bmp图片，位深与目标平台的位深请保持一致
//@EXPECTATION: 显示bmp图片的内容，并且有缩放的效果
//@EXECUTIONFLOW:1、调用CSUDIOSGGetDisplaySurface获取高标清surface；
//@EXECUTIONFLOW:2、调用CSUDIOSGGetSurfaceInfo获取高标清surface的信息；
//@EXECUTIONFLOW:3、 调用CSUDIOSGCreateSurface创建1280*720的frame_surface
//@EXECUTIONFLOW:4、调用CSUDIOSGFill填充frame_surface透明色
//@EXECUTIONFLOW:5、调用CSUDIOSGSync同步；
//@EXECUTIONFLOW:6、调用CSUDIOSGBlit以copy模式blit frame_surface 到显存
//@EXECUTIONFLOW:7、调用CSUDIOSGSync同步；
//@EXECUTIONFLOW:8、调用CSUDIOSGCreateSurface创建1280*720的deflation_surface
//@EXECUTIONFLOW:9、调用CSUDIOSGBlit以copy模式blit frame_surface 到deflation_surface
//@EXECUTIONFLOW:10、调用CSUDIOSGSync同步；
//@EXECUTIONFLOW:11、调用CSUDIOSGCreateSurface创建1280*720的zoom_surface
//@EXECUTIONFLOW:12、调用CSUDIOSGBlit以copy模式blit frame_surface 到zoom_surface
//@EXECUTIONFLOW:13、调用CSUDIOSGSync同步；
//@EXECUTIONFLOW:14、调用CSUDIOSGFill以黑色填充frame_surface
//@EXECUTIONFLOW:15、调用CSUDIOSGSync同步；
//@EXECUTIONFLOW:16、调用CSUDIOSGCreateSurface创建1280*720的icon_surface
//@EXECUTIONFLOW:17、 调用CSUDIOSGBlit以copy模式blit frame_surface到icon_surface
//@EXECUTIONFLOW:18、调用CSUDIOSGSync同步；
//@EXECUTIONFLOW:19、 调用CSUDIOSGBlit以copy模式blit icon_surface到frame_surface
//@EXECUTIONFLOW:20、调用CSUDIOSGSync同步；
//@EXECUTIONFLOW:21、 调用CSUDIOSGCreateSurface创建创建tmp_surface
//@EXECUTIONFLOW:22、 调用CSUDIOSGBlit以copy模式blit frame_surface到tmp_surface
//@EXECUTIONFLOW:23、调用CSUDIOSGSync同步；
//@EXECUTIONFLOW:24、 调用CSUDIOSGBlit以alpha模式blit tmp_surface到frame_surface
//@EXECUTIONFLOW:25、调用CSUDIOSGSync同步；
//@EXECUTIONFLOW:26、 调用CSUDIOSGSetSurfaceInfo设置deflation_surface
//@EXECUTIONFLOW:27、 调用CSUDIOSGBlit以alpha模式blit deflation_surface到frame_surface
//@EXECUTIONFLOW:28、调用CSUDIOSGSync同步；
//@EXECUTIONFLOW:29、 调用CSUDIOSGDestroySurface销毁tmp_surface
//@EXECUTIONFLOW:30、 显示frame_surface
//@EXECUTIONFLOW:31、重复步骤19~30
//@EXECUTIONFLOW:32、确定缩小特效的显示结果；
//@EXECUTIONFLOW:33、 调用CSUDIOSGBlit以copy模式blit icon_surface到frame_surface
//@EXECUTIONFLOW:34、调用CSUDIOSGSync同步；
//@EXECUTIONFLOW:35、 调用CSUDIOSGCreateSurface创建创建tmp_surface
//@EXECUTIONFLOW:36、 调用CSUDIOSGBlit以copy模式blit frame_surface到tmp_surface
//@EXECUTIONFLOW:37、调用CSUDIOSGSync同步；
//@EXECUTIONFLOW:38、 调用CSUDIOSGBlit以alpha模式blit tmp_surface到frame_surface
//@EXECUTIONFLOW:39、调用CSUDIOSGSync同步；
//@EXECUTIONFLOW:40、 调用CSUDIOSGSetSurfaceInfo设置zoom_surface
//@EXECUTIONFLOW:41、 调用CSUDIOSGBlit以alpha模式blit zoom_surface到frame_surface
//@EXECUTIONFLOW:42、调用CSUDIOSGSync同步；
//@EXECUTIONFLOW:43、 调用CSUDIOSGDestroySurface销毁tmp_surface
//@EXECUTIONFLOW:44、 显示frame_surface
//@EXECUTIONFLOW:45、重复步骤33~44
//@EXECUTIONFLOW:46、确定缩小特效的显示结果；
//@EXECUTIONFLOW:47、 调用CSUDIOSGDestroySurface销毁icon_surface
//@EXECUTIONFLOW:48、 调用CSUDIOSGDestroySurface销毁deflation_surface
//@EXECUTIONFLOW:49、 调用CSUDIOSGDestroySurface销毁zoom_surface
//@EXECUTIONFLOW:50、 调用CSUDIOSGDestroySurface销毁frame_surface
//@EXECUTIONFLOW:51、清屏；
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
		"step 1 获取显示surface失败!");

	if(bIsHD)
    	{
        	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(1, &hDisplay_HD),
            		"step 3 获取显示surface失败!!");
    	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hDisplaySurface, &sDisplaySurfaceInfo),
		"step 2 获取surface的信息!");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGCreateSurface(sDisplaySurfaceInfo.m_ePixelFormat, nWidth, nHeight, &hFrameSurface),
		"step 3 创建hFrameSurface失败!");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iFillAndSync(hFrameSurface, &sScreenRect, 0),
		"step 4 填充hFrameSurface失败!");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hDisplaySurface, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
		"step 6 显示失败!");
		
    if(bIsHD)
    {
        CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hDisplay_HD, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
            "step 6 blit失败!!!");
    
    }

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGCreateSurface(sDisplaySurfaceInfo.m_ePixelFormat, nWidth, nHeight, &hDeflationSurface),
		"step 8 创建缩小surface失败!");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hDeflationSurface, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
		"step 9 blit失败!");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGCreateSurface(sDisplaySurfaceInfo.m_ePixelFormat, nWidth, nHeight, &hZoomSurface),
		"step 11 创建放大surface失败!");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hZoomSurface, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
		"step 12 blit失败!");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iFillAndSync(hFrameSurface, NULL, unFrameBgColor),
		"step 14 填充失败!");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGCreateSurface(sDisplaySurfaceInfo.m_ePixelFormat, nWidth, nHeight, &hIconSurface),
		"step 16 创建备份surface失败!");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hIconSurface, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
		"step 17 blit 失败!");

	CSTCPrint("接下来将在屏幕显示一组逐渐缩小的图片! \n");
	CSTCPrint("Next you can see a deflation picture displayed on the screen! \n");
	CSTKWaitAnyKey();
	
	nStep = (nWidth/68) > (nHeight/36)  ? nHeight/36 : nWidth/68;

	//缩小
	do
	{
		i++;
		sZoomRect.m_nWidth = (nStep - i) * nWidth/nStep;
		sZoomRect.m_nHeight= (nStep - i) * nHeight/nStep;
		sZoomRect.m_nX = (nWidth -sZoomRect.m_nWidth)/2;
		sZoomRect.m_nY = (nHeight -sZoomRect.m_nHeight)/2;

		CSTCPrint("{%d, %d, %d, %d}\n", sZoomRect.m_nX, sZoomRect.m_nY, sZoomRect.m_nWidth, sZoomRect.m_nHeight);
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hFrameSurface, NULL, hIconSurface, NULL, EM_UDIOSG_OPMODE_COPY),
			"step 19 blit失败!");
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGCreateSurface(sDisplaySurfaceInfo.m_ePixelFormat, sZoomRect.m_nWidth, sZoomRect.m_nHeight, &hTmpSurface),
			"step 21 创建临时surface失败!");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hTmpSurface, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
			"step 22 blit 失败!");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hFrameSurface, &sZoomRect, hTmpSurface, NULL, EM_UDIOSG_OPMODE_ALPHA),
			"step 24 blit 失败!");

		unImageSize = OSG_iGetPhoto("UDI2OSG_search.bmp", &ppImage);

		CSTK_ASSERT_TRUE_FATAL((ppImage  != CSUDI_NULL) && (unImageSize != 0), "step 16 获取测试图片数据失败!");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iSetSurfaceBuf(hDeflationSurface, ppImage, unImageSize, 1280, 720),
			"step 26 设置缩小surface的内容失败!");

		CSUDIOSFree(ppImage);
		ppImage = NULL;
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hFrameSurface, &sZoomRect, hDeflationSurface, NULL, EM_UDIOSG_OPMODE_ALPHA),
			"step 27 blit 失败!");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hTmpSurface),
			"step 29 销毁临时surface失败!");
		
		hTmpSurface = CSUDI_NULL;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hDisplaySurface, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
			"step 30 blit失败!");

		if(bIsHD)
        	{
            		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hDisplay_HD, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
                		"step 30 blit失败!!!");
        	}
	}while(i < nStep - 1);

	CSTCPrint("图片是否能正常显示逐渐缩小的特效? \"y\" 正常, \"n\"存在异常\n");
	CSTCPrint("Whether the picture show  with deflation effect? \"y\" for normal, \"n\"if not.\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== (bRet = CSTKWaitYes( )),
		"step 32 portal图片显示不正常!");

	CSTCPrint("接下来将在屏幕显示一组逐渐放大的图片! \n");
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
			"step 33 blit 失败!");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGCreateSurface(sDisplaySurfaceInfo.m_ePixelFormat, sZoomRect.m_nWidth, sZoomRect.m_nHeight, &hTmpSurface),
			"step 35 创建临时surface失败!");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hTmpSurface, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
			"step 36 blit失败!");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hFrameSurface, &sZoomRect, hTmpSurface, NULL, EM_UDIOSG_OPMODE_ALPHA), 
			"step 38 blit 失败!");

		unImageSize = OSG_iGetPhoto("UDI2OSG_avsetting.bmp", &ppImage);
		CSTK_ASSERT_TRUE_FATAL(ppImage  != CSUDI_NULL, "step 25 获取测试图片数据失败!");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iSetSurfaceBuf(hZoomSurface, ppImage, unImageSize, 1280, 720),
			"step 40 设置放大surface的显示内容!");
			
		CSUDIOSFree(ppImage);
		ppImage = NULL;
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hFrameSurface, &sZoomRect, hZoomSurface, NULL, EM_UDIOSG_OPMODE_ALPHA),
			"step 41 blit 失败!");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hTmpSurface),
			"step 43 销毁临时surface失败!");
		
		hTmpSurface = CSUDI_NULL;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hDisplaySurface, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY), 
			"step 44 blit失败!");

		if(bIsHD)
        	{
            		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == OSG_iBlitAndSync(hDisplay_HD, NULL, hFrameSurface, NULL, EM_UDIOSG_OPMODE_COPY),
                		"step 44 blit失败!!!");
        	}
	}while(i < nStep - 1);

	CSTCPrint("图片是否能正常显示逐渐放大的特效? \"y\" 正常, \"n\"存在异常\n");
	CSTCPrint("Whether the picture show  with  zoom effect? \"y\" for normal, \"n\"if not.\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== (bRet = CSTKWaitYes( )),
		"step 46 portal图片显示不正常!");
	
	CSTK_FATAL_POINT
	if(hTmpSurface != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hTmpSurface),
			"销毁hTmpSurface失败!");
	}

	if(hIconSurface != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hIconSurface),
			"step 47 销毁hIconSurface失败!");
	}

	if(hDeflationSurface != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hDeflationSurface),
			"step 48 销毁hDeflationSurface失败!");
	}
	
	if(hZoomSurface != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hZoomSurface),
			"step 49 销毁hZoomSurface失败!");
	}
	
	if(hFrameSurface != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGDestroySurface(hFrameSurface),
			"step 50 销毁hFrameSurface失败!");
	}

	if(ppImage != NULL)
	{
		CSUDIOSFree(ppImage);
	}

	OSG_iClearScreen();
	
	return bRet;
}


