/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
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

int g_nHDMICount = -1;//HDMI�ӿ�����

CSUDI_BOOL CSTC_HDMI_IT_Init()
{
	char acCfgBuf[32];
	CSUDI_BOOL bRet = CSUDI_FALSE;

	CSTCPrint("��ģ��������Ҫ��������HDMI�ӿ�\n");

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
		CSTCPrint("���������ļ����Ƿ��Ѿ�����[HDMI]�Ρ�CS_HDMIUDI2_HDMI_COUNT�Ƿ�����ȷ����\n");
	}

	return bRet;
}

CSUDI_BOOL CSTC_HDMI_IT_UnInit()
{
	//�ڱ�����������ִ�к����
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIHDMIGetCount(int * pnHdmiDeviceCount)
//@DESCRIPTION: ��ȡHDMI�ӿ�����
//@PRECONDITION: ��ƽ̨ʵ����HDMI��ؽӿ�,�����ļ�������Ӧ������
//@INPUT: pnHdmiDeviceCount = &nHDMIcount
//@EXPECTATION: ����ֵ����CSUDI_SUCCESS, �һ�ȡ����HDMI�ӿ���������Ԥ�ڽ��
//@EXECUTIONFLOW: 1�������ƽ̨֧�ֵ�HDMI�ӿ��������������0
//@EXECUTIONFLOW: 2������CSUDIHDMIGetCount��ȡ��ƽ̨��HDMI�ӿ��������ɹ���ȡ����CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������ȡ����HDMI�ӿ������������ļ��е�Ԥ�ڽ�����бȽ�
//@EXECUTIONFLOW: 4������NULLֵ����ȡ�ӿڸ�����Ӧ����CSUDIHDMI_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_HDMI_IT_CSUDIHDMIGetCount(void)
{
	int nHDMIcount = 0;
	
	CSTK_ASSERT_TRUE_FATAL(g_nHDMICount > 0, "step1:��������Ҫ����ƽ̨������һ��HDMI�ӿ�");		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIGetCount(&nHDMIcount), "step2:��ȡHDMI�ӿڵ�����ʧ��");
	CSTK_ASSERT_TRUE_FATAL(nHDMIcount == g_nHDMICount, "step3:��ȡHDMI�ӿڵ���������ȷ");

	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_BAD_PARAMETER == CSUDIHDMIGetCount(CSUDI_NULL), "step4:�쳣���Է���ֵ����ȷ");

	CSTK_FATAL_POINT
	{
		;
	}
	
	return CSUDI_TRUE;
}
 
//@CASEGROUP: CSUDI_Error_Code CSUDIHDMIGetSinkCapabilities(int nHdmiIndex, CSUDIHDMISinkCapabilities_S *pCaps)
//@DESCRIPTION: ��ȡHDMI���ն��豸������
//@PRECONDITION: ��ƽ̨ʵ����HDMI��ؽӿ�,�����ļ�������Ӧ������
//@INPUT: nHdmiIndex = i, pCaps = sCapabilities
//@EXPECTATION:  �ܻ�ȡ��ֵ���һ�ȡ����ֵ����Ԥ�ڽ��
//@EXECUTIONFLOW: 1�������ƽ̨֧�ֵ�HDMI�ӿ��������������0
//@EXECUTIONFLOW: 2������CSUDIHDMIGetSinkCapabilities��ȡ��ƽ̨��HDMI���ն��豸�����ԣ��ɹ���ȡ����CSUDI_SUCCESS 
//@EXECUTIONFLOW: 3����ȡ��m_ePreferAudioMode
//@EXECUTIONFLOW: 4����ȡ��m_ePreferVideoResolution
//@EXECUTIONFLOW: 5����ȡ��m_eSupportedAudioMode[0]
//@EXECUTIONFLOW: 6����ȡ��m_eSupportedVideoMode[0]
CSUDI_BOOL CSTC_HDMI_IT_CSUDIHDMIGetSinkCapabilities_001(void)
{
	int i = 0;
	CSUDIHDMISinkCapabilities_S sCapabilities;

	CSTK_ASSERT_TRUE_FATAL(g_nHDMICount > 0, "step1:��������Ҫ����ƽ̨������һ��HDMI�ӿ�");

	for(i = 0; i < g_nHDMICount; i++)
	{
		sCapabilities.m_ePreferAudioMode = -1;
		sCapabilities.m_ePreferVideoResolution = -1;
		sCapabilities.m_eSupportedAudioMode[0] = -1;
		sCapabilities.m_eSupportedVideoMode[0] = -1;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIGetSinkCapabilities(i, &sCapabilities), "step2:��ȡHDMI���ն��豸������ʧ��");

		CSTK_ASSERT_TRUE_FATAL((sCapabilities.m_ePreferAudioMode > EM_UDIHDMI_SINK_AUDIO_MODE_INVALID) && (sCapabilities.m_ePreferAudioMode < EM_UDIHDMI_SINK_AUDIO_MODE_COUNT), "step3:PreferAudioModeδ����ȷ��ȡ��");
		CSTK_ASSERT_TRUE_FATAL((sCapabilities.m_ePreferVideoResolution > EM_UDISCREEN_RESOLUTION_INVALID) && (sCapabilities.m_ePreferVideoResolution <= EM_UDISCREEN_RESOLUTION_NUM), "step4:PreferVideoResolutionδ����ȷ��ȡ��");
		CSTK_ASSERT_TRUE_FATAL(sCapabilities.m_eSupportedAudioMode[0] != -1, "step5:SupportedAudioModeδ����ȷ��ȡ��");
		CSTK_ASSERT_TRUE_FATAL(sCapabilities.m_eSupportedVideoMode[0] != -1, "step6:SupportedVideoModeδ����ȷ��ȡ��");
	}
	
	CSTK_FATAL_POINT
	{
		;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIHDMIGetSinkCapabilities(int nHdmiIndex, CSUDIHDMISinkCapabilities_S *pCaps)
//@DESCRIPTION: ��ȡHDMI���ն��豸�����ԣ�����վ��
//@PRECONDITION: ��ƽ̨ʵ����HDMI��ؽӿ�,�����ļ�������Ӧ������
//@INPUT: nHdmiIndex = i, pCaps = CSUDI_NULL
//@EXPECTATION:  ����ֵ����CSUDIHDMI_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW: 1�������ƽ̨֧�ֵ�HDMI�ӿ��������������0
//@EXECUTIONFLOW: 2������CSUDIHDMIGetSinkCapabilities��ȡ��ƽ̨��HDMI���ն��豸�����ԣ�����CSUDIHDMI_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_HDMI_IT_CSUDIHDMIGetSinkCapabilities_002(void)
{
	int i = 0;

	CSTK_ASSERT_TRUE_FATAL(g_nHDMICount > 0, "step1:��������Ҫ����ƽ̨������һ��HDMI�ӿ�");

	for(i = 0; i < g_nHDMICount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_BAD_PARAMETER == CSUDIHDMIGetSinkCapabilities(i, CSUDI_NULL), "step2:���ش�����벻��ȷ");
	}
	
	CSTK_FATAL_POINT
	{
		;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIHDMIGetSinkCapabilities(int nHdmiIndex, CSUDIHDMISinkCapabilities_S *pCaps)
//@DESCRIPTION: ��ȡHDMI���ն��豸������,����Ƿ��豸����
//@PRECONDITION: ��ƽ̨ʵ����HDMI��ؽӿ�,�����ļ�������Ӧ������
//@INPUT: nHdmiIndex = g_nHDMICount, pCaps = sCapabilities
//@EXPECTATION:  ����ֵ����CSUDIHDMI_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW: 1�������ƽ̨֧�ֵ�HDMI�ӿ��������������0
//@EXECUTIONFLOW: 2������CSUDIHDMIGetSinkCapabilities��ȡ��ƽ̨��HDMI���ն��豸�����ԣ�Ӧ�÷���CSUDIHDMI_ERROR_INVALID_DEVICE_ID
CSUDI_BOOL CSTC_HDMI_IT_CSUDIHDMIGetSinkCapabilities_003(void)
{
	CSUDIHDMISinkCapabilities_S sCapabilities;
	
	memset(&sCapabilities, 0x00, sizeof(CSUDIHDMISinkCapabilities_S));
	
 	CSTK_ASSERT_TRUE_FATAL(g_nHDMICount > 0, "step1:��������Ҫ����ƽ̨������һ��HDMI�ӿ�");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_INVALID_DEVICE_ID == CSUDIHDMIGetSinkCapabilities(g_nHDMICount, &sCapabilities), "step2:���ش�����벻��ȷ");
	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_INVALID_DEVICE_ID == CSUDIHDMIGetSinkCapabilities(-1, &sCapabilities), "step3:���ش�����벻��ȷ");

	CSTK_FATAL_POINT
	{
		;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIHDMISetProperty(int nHdmiIndex, CSUDIHDMIPropertyType_E ePropetyType, void* pvData);
//@DESCRIPTION: ����HDMI������,����HDMI��������úͶ�ȡ
//@PRECONDITION: ��ƽ̨ʵ����HDMI��ؽӿ�,�����ļ�������Ӧ������
//@INPUT: nHdmiIndex = i, ePropetyType = CSUDIHDMIPropertyType_E, pvData = void*
//@EXPECTATION: ����ֵ����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1�������ƽ̨֧�ֵ�HDMI�ӿ��������������0
//@EXECUTIONFLOW: 2������CSUDIHDMISetProperty���ø�ƽ̨��EM_UDIHDMI_VENDOR_NAME
//@EXECUTIONFLOW: 3������CSUDIHDMIGetProperty��ȡ��ƽ̨��EM_UDIHDMI_VENDOR_NAME
//@EXECUTIONFLOW: 4������EM_UDIHDMI_VENDOR_NAME�Ƿ����Ԥ��
//@EXECUTIONFLOW: 5������EM_UDIHDMI_VENDOR_NAME�����Ƿ����Ԥ��
//@EXECUTIONFLOW: 6������CSUDIHDMISetProperty���ø�ƽ̨��EM_UDIHDMI_PRODUCT_DESCRIPTION
//@EXECUTIONFLOW: 7������CSUDIHDMIGetProperty��ȡ��ƽ̨��EM_UDIHDMI_PRODUCT_DESCRIPTION
//@EXECUTIONFLOW: 8������EM_UDIHDMI_PRODUCT_DESCRIPTION�Ƿ����Ԥ��
//@EXECUTIONFLOW: 9������EM_UDIHDMI_PRODUCT_DESCRIPTION�����Ƿ����Ԥ��
//@EXECUTIONFLOW: 10������CSUDIHDMISetProperty���ø�ƽ̨��EM_UDIHDMI_HDCP_KEY_INFO
//@EXECUTIONFLOW: 11������CSUDIHDMIGetProperty��ȡ��ƽ̨��EM_UDIHDMI_HDCP_KEY_INFO
//@EXECUTIONFLOW: 12������EM_UDIHDMI_HDCP_KEY_INFO�Ƿ����Ԥ��
//@EXECUTIONFLOW: 13������CSUDIHDMISetProperty���ø�ƽ̨��EM_UDIHDMI_LINK_STATUS
//@EXECUTIONFLOW: 14������CSUDIHDMIGetProperty��ȡ��ƽ̨��EM_UDIHDMI_LINK_STATUS
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
		
 	CSTK_ASSERT_TRUE_FATAL(g_nHDMICount > 0, "step1:��������Ҫ����ƽ̨������һ��HDMI�ӿ�");

	for (i = 0; i < g_nHDMICount; i++)
	{
		memset(&sCSUDIHdcpKeyInfo, 0x00, sizeof(CSUDIHdcpKeyInfo_S));

		bRet = CSUDI_FALSE;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMISetProperty(i, EM_UDIHDMI_VENDOR_NAME, szSetVendor), "step2:�������ò��ɹ�");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIGetProperty(i, EM_UDIHDMI_VENDOR_NAME, szGetVendor, &nLen), "step3:���̻�ȡ���ɹ�");
		bRet = (strcmp(szGetVendor, szSetVendor) == 0);
		CSTK_ASSERT_TRUE_FATAL(bRet, "step4:��ȡ�ĳ������Ʋ���ȷ");
		CSTK_ASSERT_TRUE_FATAL(nLen == strlen(szSetVendor), "step5:��ȡ�ĳ������Ƴ��Ȳ���ȷ");

		bRet = CSUDI_FALSE;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMISetProperty(i, EM_UDIHDMI_PRODUCT_DESCRIPTION, szSetProduct), "step6:��Ʒ���������ò���ȷ");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIGetProperty(i, EM_UDIHDMI_PRODUCT_DESCRIPTION, szGetProduct, &nLen), "step7:��Ʒ���������û�ȡ���ɹ�");
		bRet = (strcmp(szGetVendor, szSetProduct) == 0);
		CSTK_ASSERT_TRUE_FATAL(bRet, "step8:��Ʒ���������ò���ȷ");
		CSTK_ASSERT_TRUE_FATAL(nLen == strlen(szSetProduct), "step9:��Ʒ���������ó��Ȳ���ȷ");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMISetProperty(i, EM_UDIHDMI_HDCP_KEY_INFO, &sCSUDIHdcpKeyInfo), "step10:HDMI HDCP KEY���ò���ȷ");				
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIGetProperty(i, EM_UDIHDMI_HDCP_KEY_INFO, &sCSUDIHdcpKeyInfo, &nLen), "step11:HDMI HDCP KEY��ȡ���ɹ�");
		CSTK_ASSERT_TRUE_FATAL(nLen != 0, "step12:HDMI HDCP KEY���ó��Ȳ���ȷ");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMISetProperty(i, EM_UDIHDMI_LINK_STATUS, &eCSUDIHDMILinkStatus), "step13:HDMI LINK STATUS���ò���ȷ");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIGetProperty(i, EM_UDIHDMI_LINK_STATUS, &eCSUDIHDMILinkStatus, &nLen), "step14:HDMI LINK STATUS���û�ȡ���ɹ�");		
		CSTK_ASSERT_TRUE_FATAL((eCSUDIHDMILinkStatus >= EM_UDIHDMI_STATUS_UNLINK) && (eCSUDIHDMILinkStatus <EM_UDIHDMI_STATUS_MAX), "step15:HDMI HDCP KEY��ȡ����ȷ");
	}

	CSTK_FATAL_POINT
	{	
	 	;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIHDMISetProperty(int nHdmiIndex, CSUDIHDMIPropertyType_E ePropetyType, void* pvData);
//@DESCRIPTION: ����HDMI������,����HDMI��������ã�����Ƿ�����
//@PRECONDITION: ��ƽ̨ʵ����HDMI��ؽӿ�,�����ļ�������Ӧ������
//@INPUT: nHdmiIndex = g_nHDMICount��-1, ePropetyType = CSUDIHDMIPropertyType_E, pvData = void*
//@EXPECTATION:  ����ֵ����CSUDIHDMI_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW: 1�������ƽ̨֧�ֵ�HDMI�ӿ��������������0
//@EXECUTIONFLOW: 2�����ø�ƽ̨��EM_UDIHDMI_VENDOR_NAME������nHdmiIndex=g_nHDMICount��Ӧ����CSUDIHDMI_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW: 3�����ø�ƽ̨��EM_UDIHDMI_PRODUCT_DESCRIPTION������nHdmiIndex=g_nHDMICount��Ӧ����CSUDIHDMI_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW: 4�����ø�ƽ̨��EM_UDIHDMI_HDCP_KEY_INFO������nHdmiIndex=g_nHDMICount��Ӧ����CSUDIHDMI_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW: 5�����ø�ƽ̨��EM_UDIHDMI_LINK_STATUS������nHdmiIndex=g_nHDMICount��Ӧ����CSUDIHDMI_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW: 6�����ø�ƽ̨��EM_UDIHDMI_VENDOR_NAME������nHdmiIndex=-1��Ӧ����CSUDIHDMI_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW: 7�����ø�ƽ̨��EM_UDIHDMI_PRODUCT_DESCRIPTION������nHdmiIndex=-1��Ӧ����CSUDIHDMI_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW: 8�����ø�ƽ̨��EM_UDIHDMI_HDCP_KEY_INFO������nHdmiIndex=-1��Ӧ����CSUDIHDMI_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW: 9�����ø�ƽ̨��EM_UDIHDMI_LINK_STATUS������nHdmiIndex=-1��Ӧ����CSUDIHDMI_ERROR_INVALID_DEVICE_ID
CSUDI_BOOL CSTC_HDMI_IT_CSUDIHDMISetProperty_001(void)
{
	CSUDIHdcpKeyInfo_S sCSUDIHdcpKeyInfo;
	CSUDIHDMILinkStatus_E eCSUDIHDMILinkStatus = EM_UDIHDMI_STATUS_LINK_HDCP_SUCCESSED;
	
	memset(&sCSUDIHdcpKeyInfo, 0x00, sizeof(CSUDIHdcpKeyInfo_S));
		
 	CSTK_ASSERT_TRUE_FATAL(g_nHDMICount > 0, "step1:��������Ҫ����ƽ̨������һ��HDMI�ӿ�");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_INVALID_DEVICE_ID == CSUDIHDMISetProperty(g_nHDMICount, EM_UDIHDMI_VENDOR_NAME, "Coship"), "step2:�����������ò�Ӧ�óɹ�");
	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_INVALID_DEVICE_ID == CSUDIHDMISetProperty(g_nHDMICount, EM_UDIHDMI_PRODUCT_DESCRIPTION, "CoshipProduct"), "step3:��Ʒ���������ò�Ӧ�óɹ�");
	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_INVALID_DEVICE_ID == CSUDIHDMISetProperty(g_nHDMICount, EM_UDIHDMI_HDCP_KEY_INFO, &sCSUDIHdcpKeyInfo), "step4:HDMI HDCP KEY���ò�Ӧ�óɹ�");		
	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_INVALID_DEVICE_ID == CSUDIHDMISetProperty(g_nHDMICount, EM_UDIHDMI_LINK_STATUS, &eCSUDIHDMILinkStatus), "step5:HDMI LINK STATUS���ò�Ӧ�óɹ�");

	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_INVALID_DEVICE_ID == CSUDIHDMISetProperty(-1, EM_UDIHDMI_VENDOR_NAME, "Coship"), "step6:�����������ò�Ӧ�óɹ�");
	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_INVALID_DEVICE_ID == CSUDIHDMISetProperty(-1, EM_UDIHDMI_PRODUCT_DESCRIPTION, "CoshipProduct"), "step7:��Ʒ���������ò�Ӧ�óɹ�");
	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_INVALID_DEVICE_ID == CSUDIHDMISetProperty(-1, EM_UDIHDMI_HDCP_KEY_INFO, &sCSUDIHdcpKeyInfo), "step8:HDMI HDCP KEY���ò�Ӧ�óɹ�");		
	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_INVALID_DEVICE_ID == CSUDIHDMISetProperty(-1, EM_UDIHDMI_LINK_STATUS, &eCSUDIHDMILinkStatus), "step9:HDMI LINK STATUS���ò�Ӧ�óɹ�");

	CSTK_FATAL_POINT
	{	
	 	;
	}
	
	return CSUDI_TRUE;
}


//@CASEGROUP: CSUDI_Error_Code CSUDIHDMISetProperty(int nHdmiIndex, CSUDIHDMIPropertyType_E ePropetyType, void* pvData);
//@DESCRIPTION: ����HDMI������,����HDMI��������ã�����Ƿ�ö������
//@PRECONDITION: ��ƽ̨ʵ����HDMI��ؽӿ�,�����ļ�������Ӧ������
//@INPUT: nHdmiIndex = 0, ePropetyType = EM_UDIHDMI_ERROR, pvData = void*
//@EXPECTATION:  ����ֵ����CSUDIHDMI_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW: 1�������ƽ̨֧�ֵ�HDMI�ӿ��������������0
//@EXECUTIONFLOW: 2������CSUDIHDMISetProperty���ø�ƽ̨��EM_UDIHDMI_ERROR(������)��Ӧ����CSUDIHDMI_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW: 3������CSUDIHDMISetProperty���ø�ƽ̨��EM_UDIHDMI_VENDOR_NAME(���ݳ��ȳ�������ֵ)��Ӧ����CSUDI_SUCCESS�����ݱ��ض�
CSUDI_BOOL CSTC_HDMI_IT_CSUDIHDMISetProperty_002(void)
{
 	CSTK_ASSERT_TRUE_FATAL(g_nHDMICount > 0, "step1:��������Ҫ����ƽ̨������һ��HDMI�ӿ�");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_BAD_PARAMETER == CSUDIHDMISetProperty(0, EM_UDIHDMI_PROPERT_COUNT, "Coship"), "step2:Ӧ�÷���CSUDIHDMI_ERROR_BAD_PARAMETER");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMISetProperty(0, EM_UDIHDMI_VENDOR_NAME, "CoshipErrorTest"), "step3:���ó���ָ�����ȵ�ֵ�ᱻ�׶Σ�Ӧ�÷��سɹ�");

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
//@DESCRIPTION: ��ӻص�����ʾ�û�����Ͱγ�HDMI����֤�ص�����Ӧ����Ϣ����
//@PRECONDITION: ��ƽ̨ʵ����HDMI��ؽӿ�,�����ļ�������Ӧ������
//@INPUT: nHdmiIndex = i, CSUDIHDMICallback_F = iCSUDIHDMIAddCallback_001_Callback, pvUserData = g_pnUserData_001
//@EXPECTATION:  ����ֵ����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1�������ƽ̨֧�ֵ�HDMI�ӿ��������������0
//@EXECUTIONFLOW: 2������CSUDIHDMIAddCallback��ӻص���Ӧ����CSUDIHDMIAddCallback
//@EXECUTIONFLOW: 3������HDMI����֤�յ���EM_UDIHDMI_RECEIVER_PLUGIN��Ϣ����
//@EXECUTIONFLOW: 4���γ�HDMI����֤�յ���EM_UDIHDMI_RECEIVER_PLUGOUT��Ϣ����
CSUDI_BOOL CSTC_HDMI_IT_CSUDIHDMIAddCallback_001(void)
{
	int i = 0;
	int nCallbackCnt = 0;
	
	g_pnUserData_001 = (int*)CSUDIOSMalloc(g_nHDMICount * sizeof(int));
	CSTK_ASSERT_TRUE_FATAL(g_pnUserData_001 != CSUDI_NULL, "�����ڴ�ʧ��");
	
	g_peEvt = (CSUDIHDMIEvent_E*)CSUDIOSMalloc(g_nHDMICount * sizeof(CSUDIHDMIEvent_E));
	CSTK_ASSERT_TRUE_FATAL(g_peEvt != CSUDI_NULL, "�����ڴ�ʧ��");

	for (i = 0; i < g_nHDMICount; i++)
	{
		g_pnUserData_001[i] = i;
		g_peEvt[i] = -1;
	}

	CSTK_ASSERT_TRUE_FATAL(g_nHDMICount > 0, "step1:��������Ҫ����ƽ̨������һ��HDMI�ӿ�");
	CSTCPrint("��γ�����HDMI�豸�� �γ���Y��ȷ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N������ʧ��");

	for (i = 0; i < g_nHDMICount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIHDMIAddCallback(i, iCSUDIHDMIAddCallback_001_Callback, g_pnUserData_001 + i)), "step2:��ӻص�ʧ��");
		nCallbackCnt++;
	}

	
	CSTCPrint("�����ȫ����HDMI�豸�� �����Y��ȷ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N������ʧ��");
	
	for (i = 0; i < g_nHDMICount; i++)
	{	
		CSTK_ASSERT_TRUE_FATAL(g_peEvt[i] == EM_UDIHDMI_RECEIVER_PLUGIN, "step3:�յ���EM_UDIHDMI_RECEIVER_PLUGIN��������ȷ");
	}

	CSTCPrint("��γ�ȫ����HDMI�豸�� �����Y��ȷ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N������ʧ��");
	
	for (i = 0; i < g_nHDMICount; i++)
	{	
		CSTK_ASSERT_TRUE_FATAL(g_peEvt[i] == EM_UDIHDMI_RECEIVER_PLUGOUT, "step4:�յ���EM_UDIHDMI_RECEIVER_PLUGOUT��������ȷ");
	}

	CSTK_FATAL_POINT
	{		
		for (i = 0; i < nCallbackCnt; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIDelCallback(i, iCSUDIHDMIAddCallback_001_Callback, g_pnUserData_001 + i), "�Ƴ��ص�ʧ�ܣ���Ӱ���������"); 
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
//@DESCRIPTION:  ��ӻص���������ַǷ���������֤����ֵ
//@PRECONDITION: ��ƽ̨ʵ����HDMI��ؽӿ�,�����ļ�������Ӧ������
//@INPUT: nHdmiIndex = g_nHDMICount, CSUDIHDMICallback_F = CSUDI_NULL, pvUserData = CSUDI_NULL
//@EXPECTATION:  ����ֵ����CSUDIHDMI_ERROR_BAD_PARAMETER��CSUDIHDMI_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW: 1�������ƽ̨֧�ֵ�HDMI�ӿ��������������0
//@EXECUTIONFLOW: 2������CSUDIHDMIAddCallback����Ƿ�������Ӧ����CSUDIHDMI_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW: 3������CSUDIHDMIAddCallback����Ƿ�������Ӧ����CSUDIHDMI_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW: 4������CSUDIHDMIAddCallback����Ƿ��ص���Ӧ����CSUDIHDMI_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW: 5������CSUDIHDMIAddCallback����Ƿ�pvUserData��Ӧ����CSUDIHDMI_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_HDMI_IT_CSUDIHDMIAddCallback_002(void)
{
 	CSTK_ASSERT_TRUE_FATAL(g_nHDMICount > 0, "step1:��������Ҫ����ƽ̨������һ��HDMI�ӿ�");

	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_INVALID_DEVICE_ID == CSUDIHDMIAddCallback(g_nHDMICount, iCSUDIHDMIAddCallback_001_Callback, &g_nUserData), "step2:��ӻص��������nHdmiIndex������ֵ����");

	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_INVALID_DEVICE_ID == CSUDIHDMIAddCallback(-1, iCSUDIHDMIAddCallback_001_Callback, &g_nUserData), "step3:��ӻص��������nHdmiIndex������ֵ����");

	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_BAD_PARAMETER == CSUDIHDMIAddCallback(0, CSUDI_NULL, &g_nUserData), "step4:��ӻص��������ص�������ֵ����");

	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_BAD_PARAMETER == CSUDIHDMIAddCallback(0, iCSUDIHDMIAddCallback_001_Callback, CSUDI_NULL), "step5:��ӻص��������pvUserData������ֵ����");//��Ҫȷ��

	CSTK_FATAL_POINT
	{	
	  ;
	}
	
	return CSUDI_TRUE;

}


//@CASEGROUP: CSUDI_Error_Code CSUDIHDMIAddCallback(int nHdmiIndex, CSUDIHDMICallback_F fnHdmiCallback,void * pvUserData )
//@DESCRIPTION: �ظ���Ӷ���ص�
//@PRECONDITION: ��ƽ̨ʵ����HDMI��ؽӿ�,�����ļ�������Ӧ������
//@INPUT: nHdmiIndex = 0, CSUDIHDMICallback_F = iCSUDIHDMIAddCallback_001_Callback, pvUserData = &anUserData[i]��&g_nUserData
//@EXPECTATION:  ����ֵ����CSUDI_SUCCESS��CSUDIHDMI_ERROR_ALREADY_ADDED
//@EXECUTIONFLOW: 1�������ƽ̨֧�ֵ�HDMI�ӿ��������������0
//@EXECUTIONFLOW: 2������CSUDIHDMIAddCallback��ӻص���Ӧ����CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIHDMIAddCallback���һ���Ļص���Ӧ����CSUDIHDMI_ERROR_ALREADY_ADDED
//@EXECUTIONFLOW: 4������CSUDIHDMIAddCallback���һ���Ļص���Ӧ����CSUDIHDMI_ERROR_ALREADY_ADDED
//@EXECUTIONFLOW: 5������CSUDIHDMIAddCallback���8����ͬ�Ļص���Ӧ����CSUDI_SUCCESS
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

 	CSTK_ASSERT_TRUE_FATAL(g_nHDMICount > 0, "step1:��������Ҫ����ƽ̨������һ��HDMI�ӿ�");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIAddCallback(0, iCSUDIHDMIAddCallback_001_Callback, &g_nUserData), "step2:��ӻص�ʧ��");
	bCallbackAdded = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_ALREADY_ADDED == CSUDIHDMIAddCallback(0, iCSUDIHDMIAddCallback_001_Callback, &g_nUserData), "step3:�ظ���ӻص����ز���ȷ");

	for (i = 0; i < UDIHDMI_MAX_SUPPORT_CALLBACK_NUM; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIAddCallback(0, iCSUDIHDMIAddCallback_001_Callback, &anUserData[i]), "step5:��Ӷ���ص�ʧ��");	
		nCallbackCnt = i;
	}

	CSTK_FATAL_POINT
	{	
		if (bCallbackAdded == CSUDI_TRUE)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIDelCallback(0, iCSUDIHDMIAddCallback_001_Callback, &g_nUserData), "�Ƴ��ص�ʧ�ܣ���Ӱ���������"); 
		}
		for (i = 0; i <= nCallbackCnt; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIDelCallback(0, iCSUDIHDMIAddCallback_001_Callback, &anUserData[i]), "�Ƴ��ص�ʧ�ܣ���Ӱ���������"); 
		}
	}
	
	return CSUDI_TRUE;

}


//@CASEGROUP: CSUDI_Error_Code CSUDIHDMIDelCallback(int nHdmiIndex, CSUDIHDMICallback_F fnHdmiCallback, void * pvUserData )
//@DESCRIPTION: �Ƴ��ص�����ʾ�û�����Ͱγ�HDMI����֤�ص�����Ӧ����Ϣ����
//@PRECONDITION: ��ƽ̨ʵ����HDMI��ؽӿ�,�����ļ�������Ӧ������
//@INPUT: nHdmiIndex = k, CSUDIHDMICallback_F = iCSUDIHDMIAddCallback_001_Callback, pvUserData = &g_nUserData
//@EXPECTATION:  ����ֵ����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1�������ƽ̨֧�ֵ�HDMI�ӿ��������������0
//@EXECUTIONFLOW: 2������CSUDIHDMIAddCallback��ӻص���Ӧ����CSUDIHDMIAddCallback
//@EXECUTIONFLOW: 3����֤�յ���EM_UDIHDMI_RECEIVER_PLUGIN��Ϣ����
//@EXECUTIONFLOW: 4����֤�յ���EM_UDIHDMI_RECEIVER_PLUGOUT��Ϣ����
//@EXECUTIONFLOW: 5������CSUDIHDMIDelCallback�Ƴ��ص�
//@EXECUTIONFLOW: 6���Ƴ��ص��󣬲���HDMI����֤�յ���EM_UDIHDMI_RECEIVER_PLUGIN��Ϣ����
//@EXECUTIONFLOW: 7���Ƴ��ص��󣬰γ�HDMI����֤�յ���EM_UDIHDMI_RECEIVER_PLUGOUT��Ϣ����
CSUDI_BOOL CSTC_HDMI_IT_CSUDIHDMIDelCallback_001(void)
{
	int i = 0;
	
	g_pnUserData_001 = (int*)CSUDIOSMalloc(g_nHDMICount * sizeof(int));
	CSTK_ASSERT_TRUE_FATAL(g_pnUserData_001 != CSUDI_NULL, "�����ڴ�ʧ��");
	
	g_peEvt = (CSUDIHDMIEvent_E*)CSUDIOSMalloc(g_nHDMICount * sizeof(CSUDIHDMIEvent_E));
	CSTK_ASSERT_TRUE_FATAL(g_peEvt != CSUDI_NULL, "�����ڴ�ʧ��");

	for (i = 0; i < g_nHDMICount; i++)
	{
		g_pnUserData_001[i] = i;
		g_peEvt[i] = -1;
	}

	CSTK_ASSERT_TRUE_FATAL(g_nHDMICount > 0, "step1:��������Ҫ����ƽ̨������һ��HDMI�ӿ�");
	CSTCPrint("��γ�����HDMI�豸�� �γ���Y��ȷ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N������ʧ��");

	for (i = 0; i < g_nHDMICount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIAddCallback(i, iCSUDIHDMIAddCallback_001_Callback, g_pnUserData_001 + i), "step2:��ӻص�ʧ��");
	}

	
	CSTCPrint("�����ȫ����HDMI�豸�� �����Y��ȷ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N������ʧ��");
	
	for (i = 0; i < g_nHDMICount; i++)
	{	
		CSTK_ASSERT_TRUE_FATAL(g_peEvt[i] == EM_UDIHDMI_RECEIVER_PLUGIN, "step3:�յ���EM_UDIHDMI_RECEIVER_PLUGIN��������ȷ");
	}

	CSTCPrint("��γ�ȫ����HDMI�豸�� �����Y��ȷ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N������ʧ��");
	
	for (i = 0; i < g_nHDMICount; i++)
	{	
		CSTK_ASSERT_TRUE_FATAL(g_peEvt[i] == EM_UDIHDMI_RECEIVER_PLUGOUT, "step4:�յ���EM_UDIHDMI_RECEIVER_PLUGOUT��������ȷ");
	}

	for (i = 0; i < g_nHDMICount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIDelCallback(i, iCSUDIHDMIAddCallback_001_Callback, g_pnUserData_001+i), "step5:�Ƴ��ص�ʧ��"); 
	}

	for (i = 0; i < g_nHDMICount; i++)
	{
		g_peEvt[i] = -1;
	}

	CSTCPrint("�����ȫ����HDMI�豸�� �����Y��ȷ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N������ʧ��");

	for (i = 0; i < g_nHDMICount; i++)
	{	
		CSTK_ASSERT_TRUE_FATAL(g_peEvt[i] != EM_UDIHDMI_RECEIVER_PLUGIN, "step6:�ص����Ƴ�����Ӧ�����յ���Ϣ");
		g_peEvt[i] = -1;
	}

	CSTCPrint("��γ�ȫ����HDMI�豸�� �����Y��ȷ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N������ʧ��");

	for (i = 0; i < g_nHDMICount; i++)
	{	
		CSTK_ASSERT_TRUE_FATAL(g_peEvt[i] != EM_UDIHDMI_RECEIVER_PLUGOUT, "step7:�ص����Ƴ�����Ӧ�����յ���Ϣ");
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
//@DESCRIPTION: ��Ӷ���ص����Ƴ�
//@PRECONDITION: ��ƽ̨ʵ����HDMI��ؽӿ�,�����ļ�������Ӧ������
//@INPUT: nHdmiIndex = 0, CSUDIHDMICallback_F = iCSUDIHDMIAddCallback_001_Callback, pvUserData = &anUserData[i]
//@EXPECTATION:  ����ֵ����CSUDI_SUCCESS��CSUDIHDMI_ERROR_ALREADY_ADDED
//@EXECUTIONFLOW: 1�������ƽ̨֧�ֵ�HDMI�ӿ��������������0
//@EXECUTIONFLOW: 2������CSUDIHDMIAddCallback���Ӧ֧�ֵ��������ص���Ӧ����CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIHDMIDelCallback�Ƴ�ȫ���ص���Ӧ����CSUDI_SUCCESS
CSUDI_BOOL CSTC_HDMI_IT_CSUDIHDMIDelCallback_002(void)
{
	int i = 0;
	CSUDI_BOOL bCallbackAdded = CSUDI_TRUE;
	int anUserData[UDIHDMI_MAX_SUPPORT_CALLBACK_NUM];

	for (i = 0; i < UDIHDMI_MAX_SUPPORT_CALLBACK_NUM; i++)
	{
		anUserData[i] = i + 100;
	}

	CSTK_ASSERT_TRUE_FATAL(g_nHDMICount > 0, "step1:��������Ҫ����ƽ̨������һ��HDMI�ӿ�");

	for (i = 0; i < UDIHDMI_MAX_SUPPORT_CALLBACK_NUM; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIAddCallback(0, iCSUDIHDMIAddCallback_001_Callback, &anUserData[i]), "step2:��Ӷ���ص�ʧ��");	
	}
	bCallbackAdded = CSUDI_FALSE;
	
	for (i = 0; i < UDIHDMI_MAX_SUPPORT_CALLBACK_NUM; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIDelCallback(0, iCSUDIHDMIAddCallback_001_Callback, &anUserData[i]), "step3:�Ƴ�����ص�ʧ��"); 
	}
	bCallbackAdded = CSUDI_TRUE;

	CSTK_FATAL_POINT
	{	
		if (bCallbackAdded == CSUDI_FALSE)
		{
			CSTCPrint("===�Ƴ��ص�ʧ�ܣ����ܻ�Ӱ���������===\n");
		}
	}
	
	return CSUDI_TRUE;
}


//@CASEGROUP: CSUDI_Error_Code CSUDIHDMIDelCallback(int nHdmiIndex, CSUDIHDMICallback_F fnHdmiCallback,void * pvUserData )
//@DESCRIPTION: �Ƴ��ص�����������
//@PRECONDITION: ��ƽ̨ʵ����HDMI��ؽӿ�,�����ļ�������Ӧ������
//@INPUT: nHdmiIndex = 0��g_nHDMICount, CSUDIHDMICallback_F = iCSUDIHDMIAddCallback_001_Callback��CSUDI_NULL, pvUserData = CSUDI_NULL��&g_nUserData
//@EXPECTATION:  ����ֵ����CSUDIHDMI_ERROR_INVALID_DEVICE_ID��CSUDIHDMI_ERROR_ALREADY_ADDED
//@EXECUTIONFLOW: 1�������ƽ̨֧�ֵ�HDMI�ӿ��������������0
//@EXECUTIONFLOW: 2������CSUDIHDMIDelCallback�Ƴ�nHdmiIndex = g_nHDMICount�Ļص���Ӧ����CSUDIHDMI_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW: 3������CSUDIHDMIDelCallback�Ƴ�nHdmiIndex = -1�Ļص���Ӧ����CSUDIHDMI_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW: 4������CSUDIHDMIDelCallback�Ƴ�CSUDIHDMICallback_F = CSUDI_NULL�Ļص���Ӧ����CSUDIHDMI_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW: 5������CSUDIHDMIDelCallback�Ƴ�pvUserData = CSUDI_NULL�Ļص���Ӧ����CSUDIHDMI_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_HDMI_IT_CSUDIHDMIDelCallback_003(void)
{
 	CSTK_ASSERT_TRUE_FATAL(g_nHDMICount > 0, "step1:��������Ҫ����ƽ̨������һ��HDMI�ӿ�");

	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_INVALID_DEVICE_ID == CSUDIHDMIDelCallback(g_nHDMICount, iCSUDIHDMIAddCallback_001_Callback, &g_nUserData), "step2:�Ƴ��ص�ʧ��"); 
	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_INVALID_DEVICE_ID == CSUDIHDMIDelCallback(-1, iCSUDIHDMIAddCallback_001_Callback, &g_nUserData), "step3:�Ƴ��ص�ʧ��"); 

	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_BAD_PARAMETER == CSUDIHDMIDelCallback(0, CSUDI_NULL, &g_nUserData), "step4:�Ƴ��ص�ʧ��"); 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS== CSUDIHDMIDelCallback(0, iCSUDIHDMIAddCallback_001_Callback, CSUDI_NULL), "step5:�Ƴ��ص�ʧ��"); 

	CSTK_FATAL_POINT
	{	
		;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIHDMIDelCallback(int nHdmiIndex, CSUDIHDMICallback_F fnHdmiCallback,void * pvUserData )
//@DESCRIPTION: �ظ��Ƴ��ص�
//@PRECONDITION: ��ƽ̨ʵ����HDMI��ؽӿ�,�����ļ�������Ӧ������
//@INPUT: nHdmiIndex = 0, CSUDIHDMICallback_F = iCSUDIHDMIAddCallback_001_Callback, pvUserData = &g_nUserData
//@EXPECTATION:  ����ֵ����CSUDI_SUCCESS��CSUDIHDMI_ERROR_NO_CALLBACK
//@EXECUTIONFLOW: 1�������ƽ̨֧�ֵ�HDMI�ӿ��������������0
//@EXECUTIONFLOW: 2������CSUDIHDMIAddCallback��ӻص���Ӧ����CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIHDMIDelCallback�Ƴ��ص���Ӧ����CSUDI_SUCCESS
//@EXECUTIONFLOW: 4������CSUDIHDMIDelCallback�Ƴ���ͬ�Ļص���Ӧ����CSUDIHDMI_ERROR_NO_CALLBACK
//@EXECUTIONFLOW: 5������CSUDIHDMIDelCallback�Ƴ���ͬ�Ļص���Ӧ����CSUDIHDMI_ERROR_NO_CALLBACK
CSUDI_BOOL CSTC_HDMI_IT_CSUDIHDMIDelCallback_004(void)
{
	CSUDI_BOOL bCallbackAdded = CSUDI_TRUE;
	
 	CSTK_ASSERT_TRUE_FATAL(g_nHDMICount > 0, "step1:��������Ҫ����ƽ̨������һ��HDMI�ӿ�");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIAddCallback(0, iCSUDIHDMIAddCallback_001_Callback, &g_nUserData), "step2:��ӻص�ʧ��");
	bCallbackAdded = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIHDMIDelCallback(0, iCSUDIHDMIAddCallback_001_Callback, &g_nUserData), "step3:�Ƴ��ص�ʧ��"); 
	bCallbackAdded = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_NO_CALLBACK == CSUDIHDMIDelCallback(0, iCSUDIHDMIAddCallback_001_Callback, &g_nUserData), "step4:�Ƴ������ڵĻص���Ӧ����CSUDIHDMI_ERROR_NO_CALLBACK"); 

	CSTK_FATAL_POINT
	{	
		if (bCallbackAdded == CSUDI_FALSE)
		{
			CSTCPrint("===�Ƴ��ص�ʧ�ܣ����ܻ�Ӱ���������===\n");
		}	
	}
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDI_Error_Code CSUDIHDMIGetProperty(int nHdmiIndex, CSUDIHDMIPropertyType_E ePropetyType, void* pvData, int* pnLen)
//@DESCRIPTION: �����쳣��������ȡHDMI������
//@PRECONDITION: ��ƽ̨ʵ����HDMI��ؽӿ�,�����ļ�������Ӧ������
//@INPUT: nHdmiIndex = g_nHDMICount, ePropetyType = CSUDIHDMIPropertyType_E��CSUDI_NULL, pvData = szVendorName��CSUDI_NULL��pnLen = CSUDI_NULL
//@EXPECTATION: ����ֵ����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1�������ƽ̨֧�ֵ�HDMI�ӿ��������������0
//@EXECUTIONFLOW: 2������CSUDIHDMIGetProperty��ȡHDMI�����Ϣ��ePropetyType = -1������CSUDIHDMI_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW: 3������CSUDIHDMIGetProperty��ȡHDMI�����Ϣ��nHdmiIndex = g_nHDMICount������CSUDIHDMI_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW: 3������CSUDIHDMIGetProperty��ȡHDMI�����Ϣ��nHdmiIndex = -1������CSUDIHDMI_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW: 4������CSUDIHDMIGetProperty��ȡHDMI�����Ϣ��pvData = CSUDI_NULL������CSUDIHDMI_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW: 5������CSUDIHDMIGetProperty��ȡHDMI�����Ϣ��pnLen = CSUDI_NULL������CSUDIHDMI_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_HDMI_IT_CSUDIHDMIGetProperty_001(void)
{
	CSUDIHDMILinkStatus_E eCSUDIHDMILinkStatus = EM_UDIHDMI_STATUS_LINK_HDCP_SUCCESSED;
	char szGetVendor[8] = "\0";
	int nLen = 0;
		
 	CSTK_ASSERT_TRUE_FATAL(g_nHDMICount > 0, "step1:��������Ҫ����ƽ̨������һ��HDMI�ӿ�");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_FAILURE == CSUDIHDMIGetProperty(0, -1, szGetVendor, &nLen), "step2:�������ö�����ͣ�Ӧ����CSUDIHDMI_ERROR_BAD_PARAMETER");

	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_INVALID_DEVICE_ID == CSUDIHDMIGetProperty(g_nHDMICount, EM_UDIHDMI_VENDOR_NAME, szGetVendor, &nLen), "step3:�������������Ӧ����CSUDIHDMI_ERROR_INVALID_DEVICE_ID");
	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_INVALID_DEVICE_ID == CSUDIHDMIGetProperty(-1, EM_UDIHDMI_VENDOR_NAME, szGetVendor, &nLen), "step4:�������������Ӧ����CSUDIHDMI_ERROR_INVALID_DEVICE_ID");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_BAD_PARAMETER == CSUDIHDMIGetProperty(0, EM_UDIHDMI_LINK_STATUS, CSUDI_NULL, &nLen), "step5:�������pvData��Ӧ����CSUDIHDMI_ERROR_BAD_PARAMETER");
	CSTK_ASSERT_TRUE_FATAL(CSUDIHDMI_ERROR_BAD_PARAMETER == CSUDIHDMIGetProperty(0, EM_UDIHDMI_LINK_STATUS, &eCSUDIHDMILinkStatus, CSUDI_NULL), "step6:�������pnLen��Ӧ����CSUDIHDMI_ERROR_BAD_PARAMETER");

	CSTK_FATAL_POINT
	{	
	 	;
	}
	
	return CSUDI_TRUE;
}


