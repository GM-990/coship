/* --------------------------------------------------------------------
注意：
1.在需要与用户交互的测试用例中，可以：
	a. 使用CSTKWaitAnyKey等待用户输入任意按键
	b. 使用CSTKWaitYes等待用户输入YES
2.测试用例函数命名：测试用例ID，"测试用例ID"定义在测试用例文档中
-----------------------------------------------------------------------*/
#include <stdio.h>
#include "udi2_hdmi.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "cs_testkit.h"
#include "cs_hdmitestcase.h"
#include "udi2_os.h"

#define  EM_UDIHDMI_ERROR 0XFFFF
#define  UDIHDMI_MAX_SUPPORT_CALLBACK_NUM 8

int g_nHDMICount = -1;//HDMI接口数量

CSUDI_BOOL CSTC_HDMI_IT_Init()
{
	char acCfgBuf[32];
	CSUDI_BOOL bRet = CSUDI_FALSE;

	CSTCPrint("本模块用例需要机顶盒有HDMI接口\n");

	g_nHDMICount = 0;
	memset(acCfgBuf, '\0', sizeof(acCfgBuf));
	
	if (CSTKGetConfigInfo("HDMI", "CS_HDMIUDI2_HDMI_COUNT", acCfgBuf, sizeof(acCfgBuf)) == CS_TK_CONFIG_SUCCESS)
	{
		g_nHDMICount = CSTKGetIntFromStr(acCfgBuf, 10);
		bRet = CSUDI_TRUE;
	}
	else
	{
		bRet = CSUDI_FALSE;
		CSTCPrint("请检查配置文件中是否已经包含[HDMI]段、CS_HDMIUDI2_HDMI_COUNT是否已正确配置\n");
	}

	return bRet;
}

CSUDI_BOOL CSTC_HDMI_IT_UnInit()
{
	//在本测试用例集执行后调用
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIHDMIGetCount(int * pnHdmiDeviceCount)
//@DESCRIPTION: 获取HDMI接口数量
//@PRECONDITION: 该平台实现了HDMI相关接口,配置文件中有相应配置项
//@INPUT: pnHdmiDeviceCount = &nHDMIcount
//@EXPECTATION: 返回值等于CSUDI_SUCCESS, 且获取到的HDMI接口数量符合预期结果
//@EXECUTIONFLOW: 1、检验该平台支持的HDMI接口数量，必须大于0
//@EXECUTIONFLOW: 2、调用CSUDIHDMIGetCount获取该平台的HDMI接口数量，成功获取返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、将获取到的HDMI接口数量与配置文件中的预期结果进行比较
//@EXECUTIONFLOW: 4、传入NULL值来获取接口个数，应返回CSUDIHDMI_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_HDMI_IT_CSUDIHDMIGetCount(void)
{
	int nHDMIcount = 0;
	
	CSTK_ASSERT_TRUE_FATAL(g_nHDMICount > 0, "step1:该用例需要测试平台至少有一个HDMI接口");		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIGetCount(&nHDMIcount), "step2:获取HDMI接口的总数失败");
	CSTK_ASSERT_TRUE_FATAL(nHDMIcount == g_nHDMICount, "step3:获取HDMI接口的总数不正确");

	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_BAD_PARAMETER == CSUDIHDMIGetCount(CSUDI_NULL), "step4:异常测试返回值不正确");

	CSTK_FATAL_POINT
	{
		;
	}
	
	return CSUDI_TRUE;
}
 
//@CASEGROUP: CSUDI_Error_Code CSUDIHDMIGetSinkCapabilities(int nHdmiIndex, CSUDIHDMISinkCapabilities_S *pCaps)
//@DESCRIPTION: 获取HDMI接收端设备的特性
//@PRECONDITION: 该平台实现了HDMI相关接口,配置文件中有相应配置项
//@INPUT: nHdmiIndex = i, pCaps = sCapabilities
//@EXPECTATION:  能获取到值，且获取到的值符合预期结果
//@EXECUTIONFLOW: 1、检验该平台支持的HDMI接口数量，必须大于0
//@EXECUTIONFLOW: 2、调用CSUDIHDMIGetSinkCapabilities获取该平台的HDMI接收端设备的特性，成功获取返回CSUDI_SUCCESS 
//@EXECUTIONFLOW: 3、获取到m_ePreferAudioMode
//@EXECUTIONFLOW: 4、获取到m_ePreferVideoResolution
//@EXECUTIONFLOW: 5、获取到m_eSupportedAudioMode[0]
//@EXECUTIONFLOW: 6、获取到m_eSupportedVideoMode[0]
CSUDI_BOOL CSTC_HDMI_IT_CSUDIHDMIGetSinkCapabilities_001(void)
{
	int i = 0;
	CSUDIHDMISinkCapabilities_S sCapabilities;

	CSTK_ASSERT_TRUE_FATAL(g_nHDMICount > 0, "step1:该用例需要测试平台至少有一个HDMI接口");

	for(i = 0; i < g_nHDMICount; i++)
	{
		sCapabilities.m_ePreferAudioMode = -1;
		sCapabilities.m_ePreferVideoResolution = -1;
		sCapabilities.m_eSupportedAudioMode[0] = -1;
		sCapabilities.m_eSupportedVideoMode[0] = -1;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIGetSinkCapabilities(i, &sCapabilities), "step2:获取HDMI接收端设备的特性失败");

		CSTK_ASSERT_TRUE_FATAL((sCapabilities.m_ePreferAudioMode > EM_UDIHDMI_SINK_AUDIO_MODE_INVALID) && (sCapabilities.m_ePreferAudioMode < EM_UDIHDMI_SINK_AUDIO_MODE_COUNT), "step3:PreferAudioMode未能正确获取到");
		CSTK_ASSERT_TRUE_FATAL((sCapabilities.m_ePreferVideoResolution > EM_UDISCREEN_RESOLUTION_INVALID) && (sCapabilities.m_ePreferVideoResolution <= EM_UDISCREEN_RESOLUTION_NUM), "step4:PreferVideoResolution未能正确获取到");
		CSTK_ASSERT_TRUE_FATAL(sCapabilities.m_eSupportedAudioMode[0] != -1, "step5:SupportedAudioMode未能正确获取到");
		CSTK_ASSERT_TRUE_FATAL(sCapabilities.m_eSupportedVideoMode[0] != -1, "step6:SupportedVideoMode未能正确获取到");
	}
	
	CSTK_FATAL_POINT
	{
		;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIHDMIGetSinkCapabilities(int nHdmiIndex, CSUDIHDMISinkCapabilities_S *pCaps)
//@DESCRIPTION: 获取HDMI接收端设备的特性，传入空句柄
//@PRECONDITION: 该平台实现了HDMI相关接口,配置文件中有相应配置项
//@INPUT: nHdmiIndex = i, pCaps = CSUDI_NULL
//@EXPECTATION:  返回值等于CSUDIHDMI_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW: 1、检验该平台支持的HDMI接口数量，必须大于0
//@EXECUTIONFLOW: 2、调用CSUDIHDMIGetSinkCapabilities获取该平台的HDMI接收端设备的特性，返回CSUDIHDMI_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_HDMI_IT_CSUDIHDMIGetSinkCapabilities_002(void)
{
	int i = 0;

	CSTK_ASSERT_TRUE_FATAL(g_nHDMICount > 0, "step1:该用例需要测试平台至少有一个HDMI接口");

	for(i = 0; i < g_nHDMICount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_BAD_PARAMETER == CSUDIHDMIGetSinkCapabilities(i, CSUDI_NULL), "step2:返回错误代码不正确");
	}
	
	CSTK_FATAL_POINT
	{
		;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIHDMIGetSinkCapabilities(int nHdmiIndex, CSUDIHDMISinkCapabilities_S *pCaps)
//@DESCRIPTION: 获取HDMI接收端设备的特性,传入非法设备索引
//@PRECONDITION: 该平台实现了HDMI相关接口,配置文件中有相应配置项
//@INPUT: nHdmiIndex = g_nHDMICount, pCaps = sCapabilities
//@EXPECTATION:  返回值等于CSUDIHDMI_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW: 1、检验该平台支持的HDMI接口数量，必须大于0
//@EXECUTIONFLOW: 2、调用CSUDIHDMIGetSinkCapabilities获取该平台的HDMI接收端设备的特性，应该返回CSUDIHDMI_ERROR_INVALID_DEVICE_ID
CSUDI_BOOL CSTC_HDMI_IT_CSUDIHDMIGetSinkCapabilities_003(void)
{
	CSUDIHDMISinkCapabilities_S sCapabilities;
	
	memset(&sCapabilities, 0x00, sizeof(CSUDIHDMISinkCapabilities_S));
	
 	CSTK_ASSERT_TRUE_FATAL(g_nHDMICount > 0, "step1:该用例需要测试平台至少有一个HDMI接口");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_INVALID_DEVICE_ID == CSUDIHDMIGetSinkCapabilities(g_nHDMICount, &sCapabilities), "step2:返回错误代码不正确");
	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_INVALID_DEVICE_ID == CSUDIHDMIGetSinkCapabilities(-1, &sCapabilities), "step3:返回错误代码不正确");

	CSTK_FATAL_POINT
	{
		;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIHDMISetProperty(int nHdmiIndex, CSUDIHDMIPropertyType_E ePropetyType, void* pvData);
//@DESCRIPTION: 设置HDMI的属性,进行HDMI各项的设置和读取
//@PRECONDITION: 该平台实现了HDMI相关接口,配置文件中有相应配置项
//@INPUT: nHdmiIndex = i, ePropetyType = CSUDIHDMIPropertyType_E, pvData = void*
//@EXPECTATION: 返回值等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、检验该平台支持的HDMI接口数量，必须大于0
//@EXECUTIONFLOW: 2、调用CSUDIHDMISetProperty设置该平台的EM_UDIHDMI_VENDOR_NAME
//@EXECUTIONFLOW: 3、调用CSUDIHDMIGetProperty获取该平台的EM_UDIHDMI_VENDOR_NAME
//@EXECUTIONFLOW: 4、检验EM_UDIHDMI_VENDOR_NAME是否符合预期
//@EXECUTIONFLOW: 5、检验EM_UDIHDMI_VENDOR_NAME长度是否符合预期
//@EXECUTIONFLOW: 6、调用CSUDIHDMISetProperty设置该平台的EM_UDIHDMI_PRODUCT_DESCRIPTION
//@EXECUTIONFLOW: 7、调用CSUDIHDMIGetProperty获取该平台的EM_UDIHDMI_PRODUCT_DESCRIPTION
//@EXECUTIONFLOW: 8、检验EM_UDIHDMI_PRODUCT_DESCRIPTION是否符合预期
//@EXECUTIONFLOW: 9、检验EM_UDIHDMI_PRODUCT_DESCRIPTION长度是否符合预期
//@EXECUTIONFLOW: 10、调用CSUDIHDMISetProperty设置该平台的EM_UDIHDMI_HDCP_KEY_INFO
//@EXECUTIONFLOW: 11、调用CSUDIHDMIGetProperty获取该平台的EM_UDIHDMI_HDCP_KEY_INFO
//@EXECUTIONFLOW: 12、检验EM_UDIHDMI_HDCP_KEY_INFO是否符合预期
//@EXECUTIONFLOW: 13、调用CSUDIHDMISetProperty设置该平台的EM_UDIHDMI_LINK_STATUS
//@EXECUTIONFLOW: 14、调用CSUDIHDMIGetProperty获取该平台的EM_UDIHDMI_LINK_STATUS
CSUDI_BOOL CSTC_HDMI_IT_CSUDIHDMISetGetProperty_001(void)
{
	int i = 0;
	CSUDIHdcpKeyInfo_S sCSUDIHdcpKeyInfo;
	CSUDIHDMILinkStatus_E eCSUDIHDMILinkStatus = EM_UDIHDMI_STATUS_LINK_HDCP_SUCCESSED;
	char szSetVendor[8] = "Coship";
	char szSetProduct[16] = "CoshipProduct";
	char szGetVendor[8] = "\0";
	char szGetProduct[16] = "\0";
	int nLen = 0;
	int bRet = CSUDI_FALSE;
		
 	CSTK_ASSERT_TRUE_FATAL(g_nHDMICount > 0, "step1:该用例需要测试平台至少有一个HDMI接口");

	for (i = 0; i < g_nHDMICount; i++)
	{
		memset(&sCSUDIHdcpKeyInfo, 0x00, sizeof(CSUDIHdcpKeyInfo_S));

		bRet = CSUDI_FALSE;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMISetProperty(i, EM_UDIHDMI_VENDOR_NAME, szSetVendor), "step2:厂商设置不成功");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIGetProperty(i, EM_UDIHDMI_VENDOR_NAME, szGetVendor, &nLen), "step3:厂商获取不成功");
		bRet = (strcmp(szGetVendor, szSetVendor) == 0);
		CSTK_ASSERT_TRUE_FATAL(bRet, "step4:获取的厂商名称不正确");
		CSTK_ASSERT_TRUE_FATAL(nLen == strlen(szSetVendor), "step5:获取的厂商名称长度不正确");

		bRet = CSUDI_FALSE;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMISetProperty(i, EM_UDIHDMI_PRODUCT_DESCRIPTION, szSetProduct), "step6:产品描述符设置不正确");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIGetProperty(i, EM_UDIHDMI_PRODUCT_DESCRIPTION, szGetProduct, &nLen), "step7:产品描述符设置获取不成功");
		bRet = (strcmp(szGetVendor, szSetProduct) == 0);
		CSTK_ASSERT_TRUE_FATAL(bRet, "step8:产品描述符设置不正确");
		CSTK_ASSERT_TRUE_FATAL(nLen == strlen(szSetProduct), "step9:产品描述符设置长度不正确");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMISetProperty(i, EM_UDIHDMI_HDCP_KEY_INFO, &sCSUDIHdcpKeyInfo), "step10:HDMI HDCP KEY设置不正确");				
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIGetProperty(i, EM_UDIHDMI_HDCP_KEY_INFO, &sCSUDIHdcpKeyInfo, &nLen), "step11:HDMI HDCP KEY获取不成功");
		CSTK_ASSERT_TRUE_FATAL(nLen != 0, "step12:HDMI HDCP KEY设置长度不正确");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMISetProperty(i, EM_UDIHDMI_LINK_STATUS, &eCSUDIHDMILinkStatus), "step13:HDMI LINK STATUS设置不正确");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIGetProperty(i, EM_UDIHDMI_LINK_STATUS, &eCSUDIHDMILinkStatus, &nLen), "step14:HDMI LINK STATUS设置获取不成功");		
		CSTK_ASSERT_TRUE_FATAL((eCSUDIHDMILinkStatus >= EM_UDIHDMI_STATUS_UNLINK) && (eCSUDIHDMILinkStatus <EM_UDIHDMI_STATUS_MAX), "step15:HDMI HDCP KEY获取不正确");
	}

	CSTK_FATAL_POINT
	{	
	 	;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIHDMISetProperty(int nHdmiIndex, CSUDIHDMIPropertyType_E ePropetyType, void* pvData);
//@DESCRIPTION: 设置HDMI的属性,进行HDMI各项的配置，传入非法索引
//@PRECONDITION: 该平台实现了HDMI相关接口,配置文件中有相应配置项
//@INPUT: nHdmiIndex = g_nHDMICount、-1, ePropetyType = CSUDIHDMIPropertyType_E, pvData = void*
//@EXPECTATION:  返回值等于CSUDIHDMI_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW: 1、检验该平台支持的HDMI接口数量，必须大于0
//@EXECUTIONFLOW: 2、设置该平台的EM_UDIHDMI_VENDOR_NAME，传入nHdmiIndex=g_nHDMICount，应返回CSUDIHDMI_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW: 3、设置该平台的EM_UDIHDMI_PRODUCT_DESCRIPTION，传入nHdmiIndex=g_nHDMICount，应返回CSUDIHDMI_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW: 4、设置该平台的EM_UDIHDMI_HDCP_KEY_INFO，传入nHdmiIndex=g_nHDMICount，应返回CSUDIHDMI_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW: 5、设置该平台的EM_UDIHDMI_LINK_STATUS，传入nHdmiIndex=g_nHDMICount，应返回CSUDIHDMI_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW: 6、设置该平台的EM_UDIHDMI_VENDOR_NAME，传入nHdmiIndex=-1，应返回CSUDIHDMI_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW: 7、设置该平台的EM_UDIHDMI_PRODUCT_DESCRIPTION，传入nHdmiIndex=-1，应返回CSUDIHDMI_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW: 8、设置该平台的EM_UDIHDMI_HDCP_KEY_INFO，传入nHdmiIndex=-1，应返回CSUDIHDMI_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW: 9、设置该平台的EM_UDIHDMI_LINK_STATUS，传入nHdmiIndex=-1，应返回CSUDIHDMI_ERROR_INVALID_DEVICE_ID
CSUDI_BOOL CSTC_HDMI_IT_CSUDIHDMISetProperty_001(void)
{
	CSUDIHdcpKeyInfo_S sCSUDIHdcpKeyInfo;
	CSUDIHDMILinkStatus_E eCSUDIHDMILinkStatus = EM_UDIHDMI_STATUS_LINK_HDCP_SUCCESSED;
	
	memset(&sCSUDIHdcpKeyInfo, 0x00, sizeof(CSUDIHdcpKeyInfo_S));
		
 	CSTK_ASSERT_TRUE_FATAL(g_nHDMICount > 0, "step1:该用例需要测试平台至少有一个HDMI接口");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_INVALID_DEVICE_ID == CSUDIHDMISetProperty(g_nHDMICount, EM_UDIHDMI_VENDOR_NAME, "Coship"), "step2:卖方名称设置不应该成功");
	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_INVALID_DEVICE_ID == CSUDIHDMISetProperty(g_nHDMICount, EM_UDIHDMI_PRODUCT_DESCRIPTION, "CoshipProduct"), "step3:产品描述符设置不应该成功");
	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_INVALID_DEVICE_ID == CSUDIHDMISetProperty(g_nHDMICount, EM_UDIHDMI_HDCP_KEY_INFO, &sCSUDIHdcpKeyInfo), "step4:HDMI HDCP KEY设置不应该成功");		
	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_INVALID_DEVICE_ID == CSUDIHDMISetProperty(g_nHDMICount, EM_UDIHDMI_LINK_STATUS, &eCSUDIHDMILinkStatus), "step5:HDMI LINK STATUS设置不应该成功");

	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_INVALID_DEVICE_ID == CSUDIHDMISetProperty(-1, EM_UDIHDMI_VENDOR_NAME, "Coship"), "step6:卖方名称设置不应该成功");
	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_INVALID_DEVICE_ID == CSUDIHDMISetProperty(-1, EM_UDIHDMI_PRODUCT_DESCRIPTION, "CoshipProduct"), "step7:产品描述符设置不应该成功");
	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_INVALID_DEVICE_ID == CSUDIHDMISetProperty(-1, EM_UDIHDMI_HDCP_KEY_INFO, &sCSUDIHdcpKeyInfo), "step8:HDMI HDCP KEY设置不应该成功");		
	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_INVALID_DEVICE_ID == CSUDIHDMISetProperty(-1, EM_UDIHDMI_LINK_STATUS, &eCSUDIHDMILinkStatus), "step9:HDMI LINK STATUS设置不应该成功");

	CSTK_FATAL_POINT
	{	
	 	;
	}
	
	return CSUDI_TRUE;
}


//@CASEGROUP: CSUDI_Error_Code CSUDIHDMISetProperty(int nHdmiIndex, CSUDIHDMIPropertyType_E ePropetyType, void* pvData);
//@DESCRIPTION: 设置HDMI的属性,进行HDMI各项的配置，传入非法枚举类型
//@PRECONDITION: 该平台实现了HDMI相关接口,配置文件中有相应配置项
//@INPUT: nHdmiIndex = 0, ePropetyType = EM_UDIHDMI_ERROR, pvData = void*
//@EXPECTATION:  返回值等于CSUDIHDMI_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW: 1、检验该平台支持的HDMI接口数量，必须大于0
//@EXECUTIONFLOW: 2、调用CSUDIHDMISetProperty设置该平台的EM_UDIHDMI_ERROR(不存在)，应返回CSUDIHDMI_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW: 3、调用CSUDIHDMISetProperty设置该平台的EM_UDIHDMI_VENDOR_NAME(数据长度超过允许值)，应返回CSUDI_SUCCESS，数据被截断
CSUDI_BOOL CSTC_HDMI_IT_CSUDIHDMISetProperty_002(void)
{
 	CSTK_ASSERT_TRUE_FATAL(g_nHDMICount > 0, "step1:该用例需要测试平台至少有一个HDMI接口");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_BAD_PARAMETER == CSUDIHDMISetProperty(0, EM_UDIHDMI_PROPERT_COUNT, "Coship"), "step2:应该返回CSUDIHDMI_ERROR_BAD_PARAMETER");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMISetProperty(0, EM_UDIHDMI_VENDOR_NAME, "CoshipErrorTest"), "step3:设置超过指定长度的值会被阶段，应该返回成功");

	CSTK_FATAL_POINT
	{	
	  	;
	}
	
	return CSUDI_TRUE;
}

static int *g_pnUserData_001 = CSUDI_NULL;
static CSUDIHDMIEvent_E *g_peEvt = CSUDI_NULL;
static void iCSUDIHDMIAddCallback_001_Callback(CSUDIHDMIEvent_E eEvt, int nHdmiIndex, void* pvEventData, void * pvUserData)
{
	if ((eEvt != EM_UDIHDMI_RECEIVER_PLUGIN) && (eEvt != EM_UDIHDMI_RECEIVER_PLUGOUT))
	{
		return ;
	}
	if ((nHdmiIndex < 0) || (nHdmiIndex >= g_nHDMICount))
	{
		return ;
	}	
	if (nHdmiIndex != *((int *)pvUserData))
	{
		return ;
	}
	
	g_peEvt[nHdmiIndex] = eEvt;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIHDMIAddCallback(int nHdmiIndex, CSUDIHDMICallback_F fnHdmiCallback,void * pvUserData )
//@DESCRIPTION: 添加回调，提示用户插入和拔除HDMI，验证回调中相应的消息数量
//@PRECONDITION: 该平台实现了HDMI相关接口,配置文件中有相应配置项
//@INPUT: nHdmiIndex = i, CSUDIHDMICallback_F = iCSUDIHDMIAddCallback_001_Callback, pvUserData = g_pnUserData_001
//@EXPECTATION:  返回值等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、检验该平台支持的HDMI接口数量，必须大于0
//@EXECUTIONFLOW: 2、调用CSUDIHDMIAddCallback添加回调，应返回CSUDIHDMIAddCallback
//@EXECUTIONFLOW: 3、插入HDMI，验证收到的EM_UDIHDMI_RECEIVER_PLUGIN消息数量
//@EXECUTIONFLOW: 4、拔除HDMI，验证收到的EM_UDIHDMI_RECEIVER_PLUGOUT消息数量
CSUDI_BOOL CSTC_HDMI_IT_CSUDIHDMIAddCallback_001(void)
{
	int i = 0;
	int nCallbackCnt = 0;
	
	g_pnUserData_001 = (int*)CSUDIOSMalloc(g_nHDMICount * sizeof(int));
	CSTK_ASSERT_TRUE_FATAL(g_pnUserData_001 != CSUDI_NULL, "分配内存失败");
	
	g_peEvt = (CSUDIHDMIEvent_E*)CSUDIOSMalloc(g_nHDMICount * sizeof(CSUDIHDMIEvent_E));
	CSTK_ASSERT_TRUE_FATAL(g_peEvt != CSUDI_NULL, "分配内存失败");

	for (i = 0; i < g_nHDMICount; i++)
	{
		g_pnUserData_001[i] = i;
		g_peEvt[i] = -1;
	}

	CSTK_ASSERT_TRUE_FATAL(g_nHDMICount > 0, "step1:该用例需要测试平台至少有一个HDMI接口");
	CSTCPrint("请拔除所有HDMI设备， 拔除后按Y键确认\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，用例失败");

	for (i = 0; i < g_nHDMICount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIHDMIAddCallback(i, iCSUDIHDMIAddCallback_001_Callback, g_pnUserData_001 + i)), "step2:添加回调失败");
		nCallbackCnt++;
	}

	
	CSTCPrint("请插入全部的HDMI设备， 插入后按Y键确认\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，用例失败");
	
	for (i = 0; i < g_nHDMICount; i++)
	{	
		CSTK_ASSERT_TRUE_FATAL(g_peEvt[i] == EM_UDIHDMI_RECEIVER_PLUGIN, "step3:收到的EM_UDIHDMI_RECEIVER_PLUGIN数量不正确");
	}

	CSTCPrint("请拔除全部的HDMI设备， 插入后按Y键确认\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，用例失败");
	
	for (i = 0; i < g_nHDMICount; i++)
	{	
		CSTK_ASSERT_TRUE_FATAL(g_peEvt[i] == EM_UDIHDMI_RECEIVER_PLUGOUT, "step4:收到的EM_UDIHDMI_RECEIVER_PLUGOUT数量不正确");
	}

	CSTK_FATAL_POINT
	{		
		for (i = 0; i < nCallbackCnt; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIDelCallback(i, iCSUDIHDMIAddCallback_001_Callback, g_pnUserData_001 + i), "移除回调失败，会影响后续用例"); 
		}
		if (g_pnUserData_001 != CSUDI_NULL)
		{
			CSUDIOSFree(g_pnUserData_001);
			g_pnUserData_001 = CSUDI_NULL;
		}
		if (g_peEvt != CSUDI_NULL)
		{
			CSUDIOSFree(g_peEvt);
			g_peEvt = CSUDI_NULL;
		}
	}
	
	return CSUDI_TRUE;
}

int g_nUserData = 0;
//@CASEGROUP: CSUDI_Error_Code CSUDIHDMIAddCallback(int nHdmiIndex, CSUDIHDMICallback_F fnHdmiCallback,void * pvUserData )
//@DESCRIPTION:  添加回调，传入各种非法参数，验证返回值
//@PRECONDITION: 该平台实现了HDMI相关接口,配置文件中有相应配置项
//@INPUT: nHdmiIndex = g_nHDMICount, CSUDIHDMICallback_F = CSUDI_NULL, pvUserData = CSUDI_NULL
//@EXPECTATION:  返回值等于CSUDIHDMI_ERROR_BAD_PARAMETER、CSUDIHDMI_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW: 1、检验该平台支持的HDMI接口数量，必须大于0
//@EXECUTIONFLOW: 2、调用CSUDIHDMIAddCallback传入非法索引，应返回CSUDIHDMI_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW: 3、调用CSUDIHDMIAddCallback传入非法索引，应返回CSUDIHDMI_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW: 4、调用CSUDIHDMIAddCallback传入非法回调，应返回CSUDIHDMI_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW: 5、调用CSUDIHDMIAddCallback传入非法pvUserData，应返回CSUDIHDMI_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_HDMI_IT_CSUDIHDMIAddCallback_002(void)
{
 	CSTK_ASSERT_TRUE_FATAL(g_nHDMICount > 0, "step1:该用例需要测试平台至少有一个HDMI接口");

	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_INVALID_DEVICE_ID == CSUDIHDMIAddCallback(g_nHDMICount, iCSUDIHDMIAddCallback_001_Callback, &g_nUserData), "step2:添加回调传入错误nHdmiIndex，返回值错误");

	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_INVALID_DEVICE_ID == CSUDIHDMIAddCallback(-1, iCSUDIHDMIAddCallback_001_Callback, &g_nUserData), "step3:添加回调传入错误nHdmiIndex，返回值错误");

	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_BAD_PARAMETER == CSUDIHDMIAddCallback(0, CSUDI_NULL, &g_nUserData), "step4:添加回调传入错误回调，返回值错误");

	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_BAD_PARAMETER == CSUDIHDMIAddCallback(0, iCSUDIHDMIAddCallback_001_Callback, CSUDI_NULL), "step5:添加回调传入错误pvUserData，返回值错误");//需要确认

	CSTK_FATAL_POINT
	{	
	  ;
	}
	
	return CSUDI_TRUE;

}


//@CASEGROUP: CSUDI_Error_Code CSUDIHDMIAddCallback(int nHdmiIndex, CSUDIHDMICallback_F fnHdmiCallback,void * pvUserData )
//@DESCRIPTION: 重复添加多个回调
//@PRECONDITION: 该平台实现了HDMI相关接口,配置文件中有相应配置项
//@INPUT: nHdmiIndex = 0, CSUDIHDMICallback_F = iCSUDIHDMIAddCallback_001_Callback, pvUserData = &anUserData[i]、&g_nUserData
//@EXPECTATION:  返回值等于CSUDI_SUCCESS、CSUDIHDMI_ERROR_ALREADY_ADDED
//@EXECUTIONFLOW: 1、检验该平台支持的HDMI接口数量，必须大于0
//@EXECUTIONFLOW: 2、调用CSUDIHDMIAddCallback添加回调，应返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIHDMIAddCallback添加一样的回调，应返回CSUDIHDMI_ERROR_ALREADY_ADDED
//@EXECUTIONFLOW: 4、调用CSUDIHDMIAddCallback添加一样的回调，应返回CSUDIHDMI_ERROR_ALREADY_ADDED
//@EXECUTIONFLOW: 5、调用CSUDIHDMIAddCallback添加8个不同的回调，应返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_HDMI_IT_CSUDIHDMIAddCallback_003(void)
{
	int i = 0;
	int nCallbackCnt = 0;
	CSUDI_BOOL bCallbackAdded = CSUDI_FALSE;
	int anUserData[UDIHDMI_MAX_SUPPORT_CALLBACK_NUM];

	for (i = 0; i < UDIHDMI_MAX_SUPPORT_CALLBACK_NUM; i++)
	{
		anUserData[i] = i + 100;
	}

 	CSTK_ASSERT_TRUE_FATAL(g_nHDMICount > 0, "step1:该用例需要测试平台至少有一个HDMI接口");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIAddCallback(0, iCSUDIHDMIAddCallback_001_Callback, &g_nUserData), "step2:添加回调失败");
	bCallbackAdded = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_ALREADY_ADDED == CSUDIHDMIAddCallback(0, iCSUDIHDMIAddCallback_001_Callback, &g_nUserData), "step3:重复添加回调返回不正确");

	for (i = 0; i < UDIHDMI_MAX_SUPPORT_CALLBACK_NUM; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIAddCallback(0, iCSUDIHDMIAddCallback_001_Callback, &anUserData[i]), "step5:添加多个回调失败");	
		nCallbackCnt = i;
	}

	CSTK_FATAL_POINT
	{	
		if (bCallbackAdded == CSUDI_TRUE)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIDelCallback(0, iCSUDIHDMIAddCallback_001_Callback, &g_nUserData), "移除回调失败，会影响后续用例"); 
		}
		for (i = 0; i <= nCallbackCnt; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIDelCallback(0, iCSUDIHDMIAddCallback_001_Callback, &anUserData[i]), "移除回调失败，会影响后续用例"); 
		}
	}
	
	return CSUDI_TRUE;

}


//@CASEGROUP: CSUDI_Error_Code CSUDIHDMIDelCallback(int nHdmiIndex, CSUDIHDMICallback_F fnHdmiCallback, void * pvUserData )
//@DESCRIPTION: 移除回调，提示用户插入和拔除HDMI，验证回调中相应的消息数量
//@PRECONDITION: 该平台实现了HDMI相关接口,配置文件中有相应配置项
//@INPUT: nHdmiIndex = k, CSUDIHDMICallback_F = iCSUDIHDMIAddCallback_001_Callback, pvUserData = &g_nUserData
//@EXPECTATION:  返回值等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、检验该平台支持的HDMI接口数量，必须大于0
//@EXECUTIONFLOW: 2、调用CSUDIHDMIAddCallback添加回调，应返回CSUDIHDMIAddCallback
//@EXECUTIONFLOW: 3、验证收到的EM_UDIHDMI_RECEIVER_PLUGIN消息数量
//@EXECUTIONFLOW: 4、验证收到的EM_UDIHDMI_RECEIVER_PLUGOUT消息数量
//@EXECUTIONFLOW: 5、调用CSUDIHDMIDelCallback移除回调
//@EXECUTIONFLOW: 6、移除回调后，插入HDMI，验证收到的EM_UDIHDMI_RECEIVER_PLUGIN消息数量
//@EXECUTIONFLOW: 7、移除回调后，拔除HDMI，验证收到的EM_UDIHDMI_RECEIVER_PLUGOUT消息数量
CSUDI_BOOL CSTC_HDMI_IT_CSUDIHDMIDelCallback_001(void)
{
	int i = 0;
	
	g_pnUserData_001 = (int*)CSUDIOSMalloc(g_nHDMICount * sizeof(int));
	CSTK_ASSERT_TRUE_FATAL(g_pnUserData_001 != CSUDI_NULL, "分配内存失败");
	
	g_peEvt = (CSUDIHDMIEvent_E*)CSUDIOSMalloc(g_nHDMICount * sizeof(CSUDIHDMIEvent_E));
	CSTK_ASSERT_TRUE_FATAL(g_peEvt != CSUDI_NULL, "分配内存失败");

	for (i = 0; i < g_nHDMICount; i++)
	{
		g_pnUserData_001[i] = i;
		g_peEvt[i] = -1;
	}

	CSTK_ASSERT_TRUE_FATAL(g_nHDMICount > 0, "step1:该用例需要测试平台至少有一个HDMI接口");
	CSTCPrint("请拔除所有HDMI设备， 拔除后按Y键确认\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，用例失败");

	for (i = 0; i < g_nHDMICount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIAddCallback(i, iCSUDIHDMIAddCallback_001_Callback, g_pnUserData_001 + i), "step2:添加回调失败");
	}

	
	CSTCPrint("请插入全部的HDMI设备， 插入后按Y键确认\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，用例失败");
	
	for (i = 0; i < g_nHDMICount; i++)
	{	
		CSTK_ASSERT_TRUE_FATAL(g_peEvt[i] == EM_UDIHDMI_RECEIVER_PLUGIN, "step3:收到的EM_UDIHDMI_RECEIVER_PLUGIN数量不正确");
	}

	CSTCPrint("请拔除全部的HDMI设备， 插入后按Y键确认\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，用例失败");
	
	for (i = 0; i < g_nHDMICount; i++)
	{	
		CSTK_ASSERT_TRUE_FATAL(g_peEvt[i] == EM_UDIHDMI_RECEIVER_PLUGOUT, "step4:收到的EM_UDIHDMI_RECEIVER_PLUGOUT数量不正确");
	}

	for (i = 0; i < g_nHDMICount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIDelCallback(i, iCSUDIHDMIAddCallback_001_Callback, g_pnUserData_001+i), "step5:移除回调失败"); 
	}

	for (i = 0; i < g_nHDMICount; i++)
	{
		g_peEvt[i] = -1;
	}

	CSTCPrint("请插入全部的HDMI设备， 插入后按Y键确认\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，用例失败");

	for (i = 0; i < g_nHDMICount; i++)
	{	
		CSTK_ASSERT_TRUE_FATAL(g_peEvt[i] != EM_UDIHDMI_RECEIVER_PLUGIN, "step6:回调已移除，不应该再收到消息");
		g_peEvt[i] = -1;
	}

	CSTCPrint("请拔除全部的HDMI设备， 插入后按Y键确认\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，用例失败");

	for (i = 0; i < g_nHDMICount; i++)
	{	
		CSTK_ASSERT_TRUE_FATAL(g_peEvt[i] != EM_UDIHDMI_RECEIVER_PLUGOUT, "step7:回调已移除，不应该再收到消息");
	}

	CSTK_FATAL_POINT
	{		
		if (g_pnUserData_001 != CSUDI_NULL)
		{
			CSUDIOSFree(g_pnUserData_001);
			g_pnUserData_001 = CSUDI_NULL;
		}
		if (g_peEvt != CSUDI_NULL)
		{
			CSUDIOSFree(g_peEvt);
			g_peEvt = CSUDI_NULL;
		}
	}
	
	return CSUDI_TRUE;
}


//@CASEGROUP: CSUDI_Error_Code CSUDIHDMIDelCallback(int nHdmiIndex, CSUDIHDMICallback_F fnHdmiCallback, void * pvUserData )
//@DESCRIPTION: 添加多个回调后移除
//@PRECONDITION: 该平台实现了HDMI相关接口,配置文件中有相应配置项
//@INPUT: nHdmiIndex = 0, CSUDIHDMICallback_F = iCSUDIHDMIAddCallback_001_Callback, pvUserData = &anUserData[i]
//@EXPECTATION:  返回值等于CSUDI_SUCCESS、CSUDIHDMI_ERROR_ALREADY_ADDED
//@EXECUTIONFLOW: 1、检验该平台支持的HDMI接口数量，必须大于0
//@EXECUTIONFLOW: 2、调用CSUDIHDMIAddCallback添加应支持的最大个数回调，应返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIHDMIDelCallback移除全部回调，应返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_HDMI_IT_CSUDIHDMIDelCallback_002(void)
{
	int i = 0;
	CSUDI_BOOL bCallbackAdded = CSUDI_TRUE;
	int anUserData[UDIHDMI_MAX_SUPPORT_CALLBACK_NUM];

	for (i = 0; i < UDIHDMI_MAX_SUPPORT_CALLBACK_NUM; i++)
	{
		anUserData[i] = i + 100;
	}

	CSTK_ASSERT_TRUE_FATAL(g_nHDMICount > 0, "step1:该用例需要测试平台至少有一个HDMI接口");

	for (i = 0; i < UDIHDMI_MAX_SUPPORT_CALLBACK_NUM; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIAddCallback(0, iCSUDIHDMIAddCallback_001_Callback, &anUserData[i]), "step2:添加多个回调失败");	
	}
	bCallbackAdded = CSUDI_FALSE;
	
	for (i = 0; i < UDIHDMI_MAX_SUPPORT_CALLBACK_NUM; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIDelCallback(0, iCSUDIHDMIAddCallback_001_Callback, &anUserData[i]), "step3:移除多个回调失败"); 
	}
	bCallbackAdded = CSUDI_TRUE;

	CSTK_FATAL_POINT
	{	
		if (bCallbackAdded == CSUDI_FALSE)
		{
			CSTCPrint("===移除回调失败，可能会影响后续用例===\n");
		}
	}
	
	return CSUDI_TRUE;
}


//@CASEGROUP: CSUDI_Error_Code CSUDIHDMIDelCallback(int nHdmiIndex, CSUDIHDMICallback_F fnHdmiCallback,void * pvUserData )
//@DESCRIPTION: 移除回调传入错误参数
//@PRECONDITION: 该平台实现了HDMI相关接口,配置文件中有相应配置项
//@INPUT: nHdmiIndex = 0、g_nHDMICount, CSUDIHDMICallback_F = iCSUDIHDMIAddCallback_001_Callback、CSUDI_NULL, pvUserData = CSUDI_NULL、&g_nUserData
//@EXPECTATION:  返回值等于CSUDIHDMI_ERROR_INVALID_DEVICE_ID、CSUDIHDMI_ERROR_ALREADY_ADDED
//@EXECUTIONFLOW: 1、检验该平台支持的HDMI接口数量，必须大于0
//@EXECUTIONFLOW: 2、调用CSUDIHDMIDelCallback移除nHdmiIndex = g_nHDMICount的回调，应返回CSUDIHDMI_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW: 3、调用CSUDIHDMIDelCallback移除nHdmiIndex = -1的回调，应返回CSUDIHDMI_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW: 4、调用CSUDIHDMIDelCallback移除CSUDIHDMICallback_F = CSUDI_NULL的回调，应返回CSUDIHDMI_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW: 5、调用CSUDIHDMIDelCallback移除pvUserData = CSUDI_NULL的回调，应返回CSUDIHDMI_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_HDMI_IT_CSUDIHDMIDelCallback_003(void)
{
 	CSTK_ASSERT_TRUE_FATAL(g_nHDMICount > 0, "step1:该用例需要测试平台至少有一个HDMI接口");

	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_INVALID_DEVICE_ID == CSUDIHDMIDelCallback(g_nHDMICount, iCSUDIHDMIAddCallback_001_Callback, &g_nUserData), "step2:移除回调失败"); 
	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_INVALID_DEVICE_ID == CSUDIHDMIDelCallback(-1, iCSUDIHDMIAddCallback_001_Callback, &g_nUserData), "step3:移除回调失败"); 

	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_BAD_PARAMETER == CSUDIHDMIDelCallback(0, CSUDI_NULL, &g_nUserData), "step4:移除回调失败"); 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS== CSUDIHDMIDelCallback(0, iCSUDIHDMIAddCallback_001_Callback, CSUDI_NULL), "step5:移除回调失败"); 

	CSTK_FATAL_POINT
	{	
		;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIHDMIDelCallback(int nHdmiIndex, CSUDIHDMICallback_F fnHdmiCallback,void * pvUserData )
//@DESCRIPTION: 重复移除回调
//@PRECONDITION: 该平台实现了HDMI相关接口,配置文件中有相应配置项
//@INPUT: nHdmiIndex = 0, CSUDIHDMICallback_F = iCSUDIHDMIAddCallback_001_Callback, pvUserData = &g_nUserData
//@EXPECTATION:  返回值等于CSUDI_SUCCESS、CSUDIHDMI_ERROR_NO_CALLBACK
//@EXECUTIONFLOW: 1、检验该平台支持的HDMI接口数量，必须大于0
//@EXECUTIONFLOW: 2、调用CSUDIHDMIAddCallback添加回调，应返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIHDMIDelCallback移除回调，应返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、调用CSUDIHDMIDelCallback移除相同的回调，应返回CSUDIHDMI_ERROR_NO_CALLBACK
//@EXECUTIONFLOW: 5、调用CSUDIHDMIDelCallback移除相同的回调，应返回CSUDIHDMI_ERROR_NO_CALLBACK
CSUDI_BOOL CSTC_HDMI_IT_CSUDIHDMIDelCallback_004(void)
{
	CSUDI_BOOL bCallbackAdded = CSUDI_TRUE;
	
 	CSTK_ASSERT_TRUE_FATAL(g_nHDMICount > 0, "step1:该用例需要测试平台至少有一个HDMI接口");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIAddCallback(0, iCSUDIHDMIAddCallback_001_Callback, &g_nUserData), "step2:添加回调失败");
	bCallbackAdded = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIDelCallback(0, iCSUDIHDMIAddCallback_001_Callback, &g_nUserData), "step3:移除回调失败"); 
	bCallbackAdded = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_NO_CALLBACK == CSUDIHDMIDelCallback(0, iCSUDIHDMIAddCallback_001_Callback, &g_nUserData), "step4:移除不存在的回调，应返回CSUDIHDMI_ERROR_NO_CALLBACK"); 

	CSTK_FATAL_POINT
	{	
		if (bCallbackAdded == CSUDI_FALSE)
		{
			CSTCPrint("===移除回调失败，可能会影响后续用例===\n");
		}	
	}
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDI_Error_Code CSUDIHDMIGetProperty(int nHdmiIndex, CSUDIHDMIPropertyType_E ePropetyType, void* pvData, int* pnLen)
//@DESCRIPTION: 设置异常参数，获取HDMI的属性
//@PRECONDITION: 该平台实现了HDMI相关接口,配置文件中有相应配置项
//@INPUT: nHdmiIndex = g_nHDMICount, ePropetyType = CSUDIHDMIPropertyType_E、CSUDI_NULL, pvData = szVendorName、CSUDI_NULL，pnLen = CSUDI_NULL
//@EXPECTATION: 返回值等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、检验该平台支持的HDMI接口数量，必须大于0
//@EXECUTIONFLOW: 2、调用CSUDIHDMIGetProperty获取HDMI相关信息，ePropetyType = -1，返回CSUDIHDMI_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW: 3、调用CSUDIHDMIGetProperty获取HDMI相关信息，nHdmiIndex = g_nHDMICount，返回CSUDIHDMI_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW: 3、调用CSUDIHDMIGetProperty获取HDMI相关信息，nHdmiIndex = -1，返回CSUDIHDMI_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW: 4、调用CSUDIHDMIGetProperty获取HDMI相关信息，pvData = CSUDI_NULL，返回CSUDIHDMI_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW: 5、调用CSUDIHDMIGetProperty获取HDMI相关信息，pnLen = CSUDI_NULL，返回CSUDIHDMI_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_HDMI_IT_CSUDIHDMIGetProperty_001(void)
{
	CSUDIHDMILinkStatus_E eCSUDIHDMILinkStatus = EM_UDIHDMI_STATUS_LINK_HDCP_SUCCESSED;
	char szGetVendor[8] = "\0";
	int nLen = 0;
		
 	CSTK_ASSERT_TRUE_FATAL(g_nHDMICount > 0, "step1:该用例需要测试平台至少有一个HDMI接口");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_FAILURE == CSUDIHDMIGetProperty(0, -1, szGetVendor, &nLen), "step2:传入错误枚举类型，应返回CSUDIHDMI_ERROR_BAD_PARAMETER");

	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_INVALID_DEVICE_ID == CSUDIHDMIGetProperty(g_nHDMICount, EM_UDIHDMI_VENDOR_NAME, szGetVendor, &nLen), "step3:传入错误索引，应返回CSUDIHDMI_ERROR_INVALID_DEVICE_ID");
	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_INVALID_DEVICE_ID == CSUDIHDMIGetProperty(-1, EM_UDIHDMI_VENDOR_NAME, szGetVendor, &nLen), "step4:传入错误索引，应返回CSUDIHDMI_ERROR_INVALID_DEVICE_ID");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_BAD_PARAMETER == CSUDIHDMIGetProperty(0, EM_UDIHDMI_LINK_STATUS, CSUDI_NULL, &nLen), "step5:传入错误pvData，应返回CSUDIHDMI_ERROR_BAD_PARAMETER");
	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_BAD_PARAMETER == CSUDIHDMIGetProperty(0, EM_UDIHDMI_LINK_STATUS, &eCSUDIHDMILinkStatus, CSUDI_NULL), "step6:传入错误pnLen，应返回CSUDIHDMI_ERROR_BAD_PARAMETER");

	CSTK_FATAL_POINT
	{	
	 	;
	}
	
	return CSUDI_TRUE;
}


