/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
-----------------------------------------------------------------------*/
#include "cs_paneltestcase.h"
#include "cs_panel_test.h"
#include <stdio.h>
#include <string.h>




//ƽ̨֧�ֵ���������������ļ�����
extern int g_PanelCnt;

//ƽ̨֧�ֵ��ı�����ʾ�ַ�����
extern int g_TextCnt;

//ƽ̨֧�ֵ���ϵ��ӵ�����
extern int g_SuitCnt;

//ƽ̨ǰ����Ƿ�֧�ָ���ʾ�ƵĲ�������
extern CSUDI_BOOL g_abSuppLight[EM_UDIPANEL_MAX - EM_UDIPANEL_CTRLSTART + 1];

//���Ƶ���ʾ�������飬������Ÿÿ��Ƶ������е�����
extern CSUDI_UINT32  g_CtrlFunMask[EM_UDIPANEL_CTRLALL - EM_UDIPANEL_CTRLSTART + 1];

//��ʾ��ö�����飬��Ÿ���ʾ�Ƶ�ö��ֵ
extern CSUDIPanelLightName_E g_aeLight[EM_UDIPANEL_MAX - EM_UDIPANEL_CTRLSTART];

//��ʾ���������飬��Ÿ���ʾ�Ƶ�����
extern char g_sLight[EM_UDIPANEL_MAX - EM_UDIPANEL_CTRLSTART][10];

static CSUDI_BOOL g_bThreadEnd1;

static CSUDI_BOOL g_bThreadEnd2;

#define PANEL_TEST_STACK_SIZE 4*1024

#define REPEAT_TIME 10

CSUDI_BOOL CSTC_PANEL_Init(void)
{
	//�ڱ�����������ִ��ǰ����
	CSTC_PANEL_InitCfg();

	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_PANEL_UnInit(void)
{
	//�ڱ�����������ִ�к����
	return CSUDI_TRUE;
}

//����TEXT0�ı��Ƶ���ʾ����Ϊ"abcd"
void PANEL_TestThread1Entry(void *pvParam)
{
	int i;
	
	CSUDIPanelLightValule_S  sTextValue;
	
	sTextValue.m_eLightName = EM_UDIPANEL_TEXT0;
	sTextValue.m_uValue.m_sTextValue.m_eDisplayType = EM_UDIPANEL_TEXT_STRING;
	strcpy(sTextValue.m_uValue.m_sTextValue.m_szStr,"abcd");
	sTextValue.m_uValue.m_sTextValue.m_nStrCount = 4;
	//printf("\nliubing test ++++++++++++ thread 1 has run00\n");
	for(i= 0; i < 10; ++i)
	{
		//printf("\nliubing test ++++++++++++ thread 1 has run11\n");
		CSUDIPanelDisplay(0, &sTextValue) ;

		CSUDIOSThreadSleep(2000);
	}
	
	g_bThreadEnd1 = CSUDI_TRUE;

}

//����TEXT0�ı��Ƶ���ʾ����Ϊ"8888"
void PANEL_TestThread2Entry(void *pvParam)
{
	int i;
	
	CSUDIPanelLightValule_S  sTextValue;
	
	sTextValue.m_eLightName = EM_UDIPANEL_TEXT0;
	sTextValue.m_uValue.m_sTextValue.m_eDisplayType = EM_UDIPANEL_TEXT_STRING;
	strcpy(sTextValue.m_uValue.m_sTextValue.m_szStr,"8888");
	sTextValue.m_uValue.m_sTextValue.m_nStrCount = 4;
	CSUDIOSThreadSleep(3000);
	//printf("\nliubing test ++++++++++++ thread 2 has run00\n");
	for(i = 0; i < 10; ++i)
	{
		//printf("\nliubing test ++++++++++++ thread 2 has run11\n");
		CSUDIPanelDisplay(0, &sTextValue) ;

		CSUDIOSThreadSleep(2000);
	}
	g_bThreadEnd2 = CSUDI_TRUE;
}



//@CASEGROUP:CSUDIPanelGetCapabilityByName
//@DESCRIPTION:�����ܷ���ȷ�õ���ʾ�Ƶ�������Ϣ 
//@PRECONDITION:ǰ����ѳɹ���ʼ�� 
//@INPUT:1����ʾ�豸������ֵ�Ϸ�
//@INPUT:2�������ʾ���ͺϷ�  
//@INPUT:3���ǿյ�CSUDIPanelLightCapability_S����ָ��psLightCapability
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@REMARK:g_PanelCntΪƽ̨֧�ֵ���������������ļ�����
//@EXECUTIONFLOW:1��ErrCode = CSUDIPanelGetCapabilityByName(0,EM_UDIPANEL_MAIL,psLightCapability)
//@EXECUTIONFLOW:2���������ļ�ָ��EM_UDIPANEL_MAIL�ƴ��ڣ�������ErrCode == CSUDI_SUCCESS��psLightCapability->m_eLightName == EM_UDIPANEL_MAIL������psLightCapability->m_uProperty�������ļ���������һ�¡�
//@EXECUTIONFLOW:3���������ļ�ָ��EM_UDIPANEL_MAIL�Ʋ����ڣ�������ErrCode == CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:4����EM_UDIPANEL_CTRLSTART--EM_UDIPANEL_MAX�����еƣ���CSUDIPanelGetCapabilityByName�ӿ���ȷ�����в���ʹ�õ�ö��ֵ�⣩�ظ����ϲ���
//@EXECUTIONFLOW:5�������д��ڵ����ID�ظ����ϲ���
CSUDI_BOOL CSTC_PANEL_IT_GetCapabilityByName_0001(void)
{
	CSUDIPanelLightCapability_S sLightCapability;

	CSUDI_Error_Code ErrCode;

	int i;
	
	int j;

	for(i = 0; i < g_PanelCnt; ++i)
		for(j = 1; j < EM_UDIPANEL_MAX - EM_UDIPANEL_CTRLSTART; ++j)
			{
				if(CSUDI_PANEL_IS_TEXT(g_aeLight[j]))
				{
					ErrCode = CSUDIPanelGetCapabilityByName(i,g_aeLight[j],&sLightCapability);

					if(CSUDI_TRUE == g_abSuppLight[j])
					{
						CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode,"�ڶ�����ȡ�ı�����Ϣ����ֵʧ��");
						
						CSTK_ASSERT_TRUE_FATAL(g_aeLight[j] == sLightCapability.m_eLightName,"�ڶ�����ȡ�ı�����Ϣ����ʧ��");
						
						CSTK_ASSERT_TRUE_FATAL(g_TextCnt == sLightCapability.m_uProperty.m_sText.m_nTextCount,"�ڶ�����ȡ�ı�����Ϣ����ʧ��");
					}
					else 
					{
						CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == ErrCode,"��ȡ�ı��Ƶ�����ʧ��");					
					}
				}
			
				if(CSUDI_PANEL_IS_CTRL(g_aeLight[j]))
				{
					ErrCode = CSUDIPanelGetCapabilityByName(i,g_aeLight[j],&sLightCapability);

					if(CSUDI_TRUE == g_abSuppLight[j])
					{
						CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode,"�ڶ�����ȡ���Ƶ���Ϣ����ֵʧ��");
						
						CSTK_ASSERT_TRUE_FATAL(g_aeLight[j] == sLightCapability.m_eLightName,"�ڶ�����ȡ���Ƶ���Ϣ����ʧ��");
						
						CSTK_ASSERT_TRUE_FATAL(g_CtrlFunMask[g_aeLight[j] -EM_UDIPANEL_CTRLSTART] == sLightCapability.m_uProperty.m_sCtrl.m_dwCtrFunMask,"�ڶ�����ȡ���Ƶ���Ϣ����ʧ��");
					}
					else 
					{
						CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == ErrCode,"��ȡ���ƵƵ�����ʧ��");					
					}
				}
				
				if(CSUDI_PANEL_IS_SUIT(g_aeLight[j]))
				{
					ErrCode = CSUDIPanelGetCapabilityByName(i,g_aeLight[j],&sLightCapability);

					if(CSUDI_TRUE == g_abSuppLight[j])
					{
						CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode,"�ڶ�����ȡ��ϵ���Ϣ����ʧ��");
						
						CSTK_ASSERT_TRUE_FATAL(g_aeLight[j] == sLightCapability.m_eLightName,"�ڶ�����ȡ��ϵ���Ϣ����ʧ��");
						
						CSTK_ASSERT_TRUE_FATAL(g_SuitCnt == sLightCapability.m_uProperty.m_sSuit.m_nSuitCount,"�ڶ�����ȡ��ϵ���Ϣ����ʧ��");
					}
					else 
					{
						CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == ErrCode,"��ȡ��ϵƵ�����ʧ��");					
					}
				
				}		
			}
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelGetCapabilityByName
//@DESCRIPTION:���������豸�Ŵ������� 
//@PRECONDITION:ǰ����ѳɹ���ʼ��
//@INPUT:1����ʾ�豸������Ϊ-1
//@INPUT:2�������ʾ����Ϊƽ̨֧�ֵ���ʾ������    
//@INPUT:3���ǿյ�CSUDIPanelLightCapability_S����ָ��psLightCapability
//@EXPECTATION:����CSUDIPANEL_ERROR_INVALID_DEVICE_ID
//@REMARK:
//@EXECUTIONFLOW:1������CSUDIPanelGetCapabilityByName��������������ֵ����ΪCSUDIPANEL_ERROR_INVALID_DEVICE_ID 
CSUDI_BOOL CSTC_PANEL_IT_GetCapabilityByName_0002(void)
{
	CSUDIPanelLightCapability_S sLightCapability;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_INVALID_DEVICE_ID == CSUDIPanelGetCapabilityByName(-1, EM_UDIPANEL_TEXT0, &sLightCapability),"����ʧ��") ;		

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelGetCapabilityByName
//@DESCRIPTION:���������豸�ų����豸������� 
//@PRECONDITION:ǰ����ѳɹ���ʼ��,ƽ̨֧�ֵ��豸���������ļ�����
//@INPUT:1�������ʾ����eNameΪƽ̨֧�ֵ���ʾ������  
//@INPUT:2���ǿյ�CSUDIPanelLightCapability_S����ָ��psLightCapability
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@REMARK:g_PanelCntΪƽ̨֧�ֵ���������������ļ�����
//@EXECUTIONFLOW:1������CSUDIPanelGetCapabilityByName(g_PanelCnt,eName,psLightCapability)�����õ��ķ���ֵ����ΪCSUDIPANEL_ERROR_INVALID_DEVICE_ID 
//@EXECUTIONFLOW:2������CSUDIPanelGetCapabilityByName(g_PanelCnt + 1,eName,psLightCapability)�����õ��ķ���ֵ����ΪCSUDIPANEL_ERROR_INVALID_DEVICE_ID 
//@EXECUTIONFLOW:3������CSUDIPanelGetCapabilityByName(g_PanelCnt + 100,eName,psLightCapability)�����õ��ķ���ֵ����ΪCSUDIPANEL_ERROR_INVALID_DEVICE_ID 
CSUDI_BOOL CSTC_PANEL_IT_GetCapabilityByName_0003(void)
{
	CSUDIPanelLightCapability_S sLightCapability;

	CSUDI_Error_Code ErrCode;

	ErrCode = CSUDIPanelGetCapabilityByName(g_PanelCnt, EM_UDIPANEL_TEXT0, &sLightCapability);

	CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_INVALID_DEVICE_ID == ErrCode,"����ʧ��") ;		

	CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_INVALID_DEVICE_ID == CSUDIPanelGetCapabilityByName(g_PanelCnt + 1, EM_UDIPANEL_TEXT0, &sLightCapability),"����ʧ��") ;		

	CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_INVALID_DEVICE_ID == CSUDIPanelGetCapabilityByName(g_PanelCnt + 100, EM_UDIPANEL_TEXT0, &sLightCapability),"����ʧ��") ;		
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelGetCapabilityByName
//@DESCRIPTION:������ʾ�豸�������Ƿ������ 
//@PRECONDITION:ǰ����ѳɹ���ʼ��
//@INPUT:1����ʾ�豸������ֵ�Ϸ�
//@INPUT:2���ǿյ� CSUDIPanelLightCapability_S  ����ָ��psLightCapability
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@REMARK:g_PanelCntΪƽ̨֧�ֵ���������������ļ�����
//@EXECUTIONFLOW:1����ʾ����ΪEM_UDIPANEL_CTRLSTART����CSUDIPanelGetCapabilityByName�����������õ��ķ���ֵ����ΪCSUDIPANEL_ERROR_BAD_PARAMETER 
//@EXECUTIONFLOW:2����ʾ����ΪEM_UDIPANEL_CTRLALL����CSUDIPanelGetCapabilityByName�����������õ��ķ���ֵ����ΪCSUDIPANEL_ERROR_BAD_PARAMETER 
//@EXECUTIONFLOW:3����ʾ����ΪEM_UDIPANEL_TEXTSTART����CSUDIPanelGetCapabilityByName�����������õ��ķ���ֵ����ΪCSUDIPANEL_ERROR_BAD_PARAMETER 
//@EXECUTIONFLOW:4����ʾ����ΪEM_UDIPANEL_TEXTALL����CSUDIPanelGetCapabilityByName�����������õ��ķ���ֵ����ΪCSUDIPANEL_ERROR_BAD_PARAMETER 
//@EXECUTIONFLOW:5����ʾ����ΪEM_UDIPANEL_SUITSTART����CSUDIPanelGetCapabilityByName�����������õ��ķ���ֵ����ΪCSUDIPANEL_ERROR_BAD_PARAMETER 
//@EXECUTIONFLOW:6����ʾ����ΪEM_UDIPANEL_SUITALL����CSUDIPanelGetCapabilityByName�����������õ��ķ���ֵ����ΪCSUDIPANEL_ERROR_BAD_PARAMETER 
//@EXECUTIONFLOW:7����ʾ����ΪEM_UDIPANEL_MAX����CSUDIPanelGetCapabilityByName�����������õ��ķ���ֵ����ΪCSUDIPANEL_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:8�������д��ڵ����ID�ظ����ϲ���
CSUDI_BOOL CSTC_PANEL_IT_GetCapabilityByName_0004(void)
{
	CSUDIPanelLightCapability_S sLightCapability;

	int i;

	for(i = 0; i <g_PanelCnt; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelGetCapabilityByName(i, EM_UDIPANEL_CTRLSTART, &sLightCapability),"��һ������ʧ��") ;	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelGetCapabilityByName(i, EM_UDIPANEL_CTRLALL, &sLightCapability),"�ڶ�������ʧ��") ;		

		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelGetCapabilityByName(i, EM_UDIPANEL_TEXTSTART, &sLightCapability),"����������ʧ��") ;		

		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelGetCapabilityByName(i, EM_UDIPANEL_TEXTALL, &sLightCapability),"���Ĳ�����ʧ��") ;		

		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelGetCapabilityByName(i, EM_UDIPANEL_SUITSTART, &sLightCapability),"���岽����ʧ��") ;		

		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelGetCapabilityByName(i, EM_UDIPANEL_SUITALL, &sLightCapability),"����������ʧ��") ;		
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelGetCapabilityByName(i, EM_UDIPANEL_MAX, &sLightCapability),"���߲�����ʧ��") ;		
	}
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelGetCapabilityByName
//@DESCRIPTION:����CSUDIPanelLightCapability_S����ָ��Ϊ�յ���� 
//@PRECONDITION:ǰ����ѳɹ���ʼ��
//@INPUT:1����ʾ�豸������ֵ�Ϸ�
//@INPUT:2�������ʾ����Ϊƽ̨֧�ֵ���ʾ����    
//@INPUT:3��psLightCapability = CSUDI_NULL
//@EXPECTATION:����CSUDIPANEL_ERROR_BAD_PARAMETER 
//@REMARK:g_PanelCntΪƽ̨֧�ֵ���������������ļ�����
//@EXECUTIONFLOW:1�������ϲ�������CSUDIPanelGetCapabilityByName�����������õ��ķ���ֵ����ΪCSUDIPANEL_ERROR_BAD_PARAMETER  
//@EXECUTIONFLOW:2�������д��ڵ����ID�ظ����ϲ���
CSUDI_BOOL CSTC_PANEL_IT_GetCapabilityByName_0005(void)
{
	int i;

	for(i = 0; i < g_PanelCnt; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelGetCapabilityByName(i,EM_UDIPANEL_TEXT0,CSUDI_NULL),"����ʧ��");
	}
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelDisplay
//@DESCRIPTION:�����ܷ�ɹ������ַ����ı�����ʾ���� 
//@PRECONDITION:ǰ����ѳɹ���ʼ��
//@INPUT:1����ʾ�豸������ֵ�Ϸ�
//@INPUT:2��CSUDIPanelLightValule_S����ָ��psValueֵ�Ϸ�
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@REMARK:g_PanelCntΪƽ̨֧�ֵ��������g_TextCntΪ�ı���֧����ʾ���ַ�����
//@EXECUTIONFLOW:1������Ҫ��ʾ����ʾ������ΪEM_UDIPANEL_TEXT0����ʾ����ΪEM_UDIPANEL_TEXT_STRING��Ҫ��ʾ������Ϊboot��Ҫ��ʾ���ַ�����Ϊ4
//@EXECUTIONFLOW:2��ErrCode = CSUDIPanelDisplay(0,psValue)
//@EXECUTIONFLOW:3���������ļ�ָ�����ı��Ʋ����ڣ�������ErrCode == CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED��ִ�е�47��
//@EXECUTIONFLOW:4���������ļ�ָ��EM_UDIPANEL_TEXT0�ƴ��ڣ�������ErrCode == CSUDI_SUCCESS��ִ��5--46��
//@EXECUTIONFLOW:5���������ļ�ָ��EM_UDIPANEL_TEXT0��֧�ֵ���ʾ����g_TextCntС�����õ�Ҫ��ʾ���ַ���������������ʾ����Ϊ���õ�����ǰg_TextCnt���ַ�
//@EXECUTIONFLOW:6���������ļ�ָ��EM_UDIPANEL_TEXT0��֧�ֵ���ʾ����g_TextCnt��С�����õ�Ҫ��ʾ���ַ���������������ʾ����Ϊ���õ�����boot

//@EXECUTIONFLOW:7���޸�Ҫ��ʾ����Ϊload��Ҫ��ʾ���ַ�����Ϊ4
//@EXECUTIONFLOW:8������CSUDIPanelDisplay(0,EM_UDIPANEL_TEXT0,psValue)��������CSUDI_SUCCESS
//@EXECUTIONFLOW:9���������ļ�ָ��EM_UDIPANEL_TEXT0��֧�ֵ���ʾ����g_TextCntС�����õ�Ҫ��ʾ���ַ���������������ʾ����Ϊ���õ�����ǰg_TextCnt���ַ�
//@EXECUTIONFLOW:10���������ļ�ָ��EM_UDIPANEL_TEXT0��֧�ֵ���ʾ����g_TextCnt��С�����õ�Ҫ��ʾ���ַ���������������ʾ����Ϊ���õ�����load

//@EXECUTIONFLOW:11���޸�Ҫ��ʾ����Ϊgood��Ҫ��ʾ���ַ�����Ϊ4
//@EXECUTIONFLOW:12������CSUDIPanelDisplay(0,EM_UDIPANEL_TEXT0,psValue)��������CSUDI_SUCCESS
//@EXECUTIONFLOW:13���������ļ�ָ��EM_UDIPANEL_TEXT0��֧�ֵ���ʾ����g_TextCntС�����õ�Ҫ��ʾ���ַ���������������ʾ����Ϊ���õ�����ǰg_TextCnt���ַ�
//@EXECUTIONFLOW:14���������ļ�ָ��EM_UDIPANEL_TEXT0��֧�ֵ���ʾ����g_TextCnt��С�����õ�Ҫ��ʾ���ַ���������������ʾ����Ϊ���õ�����good

//@EXECUTIONFLOW:15���޸�Ҫ��ʾ����Ϊ888888��Ҫ��ʾ���ַ�����Ϊ6
//@EXECUTIONFLOW:16������CSUDIPanelDisplay(0,EM_UDIPANEL_TEXT0,psValue)��������CSUDI_SUCCESS
//@EXECUTIONFLOW:17���������ļ�ָ��EM_UDIPANEL_TEXT0��֧�ֵ���ʾ����g_TextCntС�����õ�Ҫ��ʾ���ַ���������������ʾ����Ϊ���õ�����ǰg_TextCnt���ַ�
//@EXECUTIONFLOW:18���������ļ�ָ��EM_UDIPANEL_TEXT0��֧�ֵ���ʾ����g_TextCnt��С�����õ�Ҫ��ʾ���ַ���������������ʾ����Ϊ���õ�����888888

//@EXECUTIONFLOW:19���޸�Ҫ��ʾ����Ϊ�Ÿ��ո�Ҫ��ʾ���ַ�����Ϊ9
//@EXECUTIONFLOW:20������CSUDIPanelDisplay(0,EM_UDIPANEL_TEXT0,psValue)��������CSUDI_SUCCESS
//@EXECUTIONFLOW:21���������ļ�ָ��EM_UDIPANEL_TEXT0��֧�ֵ���ʾ����g_TextCntС�����õ�Ҫ��ʾ���ַ�������������ǰ����ϸ��ı�����ʾ״̬ΪϨ��
//@EXECUTIONFLOW:22���������ļ�ָ��EM_UDIPANEL_TEXT0��֧�ֵ���ʾ����g_TextCnt��С�����õ�Ҫ��ʾ���ַ�������������ǰ9���ַ�����ʾ״̬ΪϨ��

//@EXECUTIONFLOW:23���޸�Ҫ��ʾ����ΪSCAN��Ҫ��ʾ���ַ�����Ϊ4
//@EXECUTIONFLOW:24������CSUDIPanelDisplay(0,EM_UDIPANEL_TEXT0,psValue)��������CSUDI_SUCCESS
//@EXECUTIONFLOW:25���������ļ�ָ��EM_UDIPANEL_TEXT0��֧�ֵ���ʾ����g_TextCntС�����õ�Ҫ��ʾ���ַ���������������ʾ����Ϊ���õ�����ǰg_TextCnt���ַ�
//@EXECUTIONFLOW:26���������ļ�ָ��EM_UDIPANEL_TEXT0��֧�ֵ���ʾ����g_TextCnt��С�����õ�Ҫ��ʾ���ַ���������������ʾ����Ϊ���õ�����SCAN

//@EXECUTIONFLOW:24���޸�Ҫ��ʾ����ΪC0123��Ҫ��ʾ���ַ�����Ϊ5
//@EXECUTIONFLOW:25������CSUDIPanelDisplay(0,EM_UDIPANEL_TEXT0,psValue)��������CSUDI_SUCCESS
//@EXECUTIONFLOW:26���������ļ�ָ��EM_UDIPANEL_TEXT0��֧�ֵ���ʾ����g_TextCntС�����õ�Ҫ��ʾ���ַ���������������ʾ����Ϊ���õ�����ǰg_TextCnt���ַ�
//@EXECUTIONFLOW:27���������ļ�ָ��EM_UDIPANEL_TEXT0��֧�ֵ���ʾ����g_TextCnt��С�����õ�Ҫ��ʾ���ַ���������������ʾ����Ϊ���õ�����C0123

//@EXECUTIONFLOW:28���޸�Ҫ��ʾ����ΪR34567��Ҫ��ʾ���ַ�����Ϊ6
//@EXECUTIONFLOW:29������CSUDIPanelDisplay(0,EM_UDIPANEL_TEXT0,psValue)��������CSUDI_SUCCESS
//@EXECUTIONFLOW:30���������ļ�ָ��EM_UDIPANEL_TEXT0��֧�ֵ���ʾ����g_TextCntС�����õ�Ҫ��ʾ���ַ���������������ʾ����Ϊ���õ�����ǰg_TextCnt���ַ�
//@EXECUTIONFLOW:31���������ļ�ָ��EM_UDIPANEL_TEXT0��֧�ֵ���ʾ����g_TextCnt��С�����õ�Ҫ��ʾ���ַ���������������ʾ����Ϊ���õ�����R34567

//@EXECUTIONFLOW:32���޸�Ҫ��ʾ����ΪA78��Ҫ��ʾ���ַ�����Ϊ3
//@EXECUTIONFLOW:33������CSUDIPanelDisplay(0,EM_UDIPANEL_TEXT0,psValue)��������CSUDI_SUCCESS
//@EXECUTIONFLOW:34���������ļ�ָ��EM_UDIPANEL_TEXT0��֧�ֵ���ʾ����g_TextCntС�����õ�Ҫ��ʾ���ַ���������������ʾ����Ϊ���õ�����ǰg_TextCnt���ַ�
//@EXECUTIONFLOW:35���������ļ�ָ��EM_UDIPANEL_TEXT0��֧�ֵ���ʾ����g_TextCnt��С�����õ�Ҫ��ʾ���ַ���������������ʾ����Ϊ���õ�����A78

//@EXECUTIONFLOW:36��Ҫ��ʾ���ַ�����Ϊ0
//@EXECUTIONFLOW:37������CSUDIPanelDisplay(0,EM_UDIPANEL_TEXT0,psValue)��������CSUDI_SUCCESS
//@EXECUTIONFLOW:38������ǰ������κ��ַ����

//@EXECUTIONFLOW:39���޸�Ҫ��ʾ����ΪH307��Ҫ��ʾ���ַ�����Ϊ4
//@EXECUTIONFLOW:40������CSUDIPanelDisplay(0,EM_UDIPANEL_TEXT0,psValue)��������CSUDI_SUCCESS
//@EXECUTIONFLOW:41���������ļ�ָ��EM_UDIPANEL_TEXT0��֧�ֵ���ʾ����g_TextCntС�����õ�Ҫ��ʾ���ַ���������������ʾ����Ϊ���õ�����ǰg_TextCnt���ַ�
//@EXECUTIONFLOW:42���������ļ�ָ��EM_UDIPANEL_TEXT0��֧�ֵ���ʾ����g_TextCnt��С�����õ�Ҫ��ʾ���ַ���������������ʾ����Ϊ���õ�����H307

//@EXECUTIONFLOW:43���޸�Ҫ��ʾ����ΪPORT��Ҫ��ʾ���ַ�����Ϊ4
//@EXECUTIONFLOW:44������CSUDIPanelDisplay(0,EM_UDIPANEL_TEXT0,psValue)��������CSUDI_SUCCESS
//@EXECUTIONFLOW:45���������ļ�ָ��EM_UDIPANEL_TEXT0��֧�ֵ���ʾ����g_TextCntС�����õ�Ҫ��ʾ���ַ���������������ʾ����Ϊ���õ�����ǰg_TextCnt���ַ�
//@EXECUTIONFLOW:46���������ļ�ָ��EM_UDIPANEL_TEXT0��֧�ֵ���ʾ����g_TextCnt��С�����õ�Ҫ��ʾ���ַ���������������ʾ����Ϊ���õ�����PORt

//@EXECUTIONFLOW:47����EM_UDIPANEL_TEXTSTART--EM_UDIPANEL_TEXTALL�ĵ��ظ�ִ�����ϲ��衣
//@EXECUTIONFLOW:48�������д��ڵ����ID�ظ����ϲ��衣
CSUDI_BOOL CSTC_PANEL_IT_PanelDisplay_0001(void)
{
	int i;
	int j;
	CSUDI_Error_Code ErrCode;
	CSUDIPanelLightValule_S sValue;
	
	for(i = 0; i < g_PanelCnt; ++i)
	{
		for(j = 1; j < EM_UDIPANEL_TEXTALL - EM_UDIPANEL_TEXTSTART;++j)
		{
			sValue.m_eLightName = g_aeLight[EM_UDIPANEL_TEXTSTART + j];
			
			sValue.m_uValue.m_sTextValue.m_eDisplayType =  EM_UDIPANEL_TEXT_STRING;
			
			strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"boot");
			
			sValue.m_uValue.m_sTextValue.m_nStrCount = 4;

			ErrCode = CSUDIPanelDisplay(i,&sValue);

			if(CSUDI_TRUE != g_abSuppLight[EM_UDIPANEL_TEXTSTART + j])
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == ErrCode,"����������ʧ��");
				
				continue;
			}
			else
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode,"���Ĳ�����ʧ��");

				if(g_TextCnt < sValue.m_uValue.m_sTextValue.m_nStrCount)
				{
					CSTCPrint("ǰ�����%s�ı����Ƿ�����ַ���boot��ǰ%d���ַ�\n", g_sLight[EM_UDIPANEL_TEXTSTART + j], g_TextCnt);
					
					CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"���岽����ʧ��");			
				}
				else
				{
					CSTCPrint("ǰ�����%s�ı����Ƿ�����ַ���boot\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);
					
					CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����������ʧ��");
				}
			}
			
			strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"load");
			
			sValue.m_uValue.m_sTextValue.m_nStrCount = 4;
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i,&sValue),"�ڰ˲�����ʧ��");
			
			if(g_TextCnt < sValue.m_uValue.m_sTextValue.m_nStrCount)
			{
				CSTCPrint("ǰ�����%s�ı����Ƿ�����ַ���load��ǰ%d���ַ�\n",  g_sLight[EM_UDIPANEL_TEXTSTART + j], g_TextCnt);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"�ھŲ�����ʧ��");			
			}
			else
			{
				CSTCPrint("ǰ�����%s�ı����Ƿ�����ַ���load\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"��ʮ������ʧ��");
			}
			
			strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"good");
			
			sValue.m_uValue.m_sTextValue.m_nStrCount = 4;
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i,&sValue),"��ʮ��������ʧ��");
			
			if(g_TextCnt < sValue.m_uValue.m_sTextValue.m_nStrCount)
			{
				CSTCPrint("ǰ�����%s�ı����Ƿ�����ַ���good��ǰ%d���ַ�\n", g_sLight[EM_UDIPANEL_TEXTSTART + j], g_TextCnt);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"��ʮ��������ʧ��");			
			}
			else
			{
				CSTCPrint("ǰ�����%s�ı����Ƿ�����ַ���good\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"��ʮ�Ĳ�����ʧ��");
			}

			strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"888888");
			
			sValue.m_uValue.m_sTextValue.m_nStrCount = 6;
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i,&sValue),"��ʮ��������ʧ��");
			
			if(g_TextCnt < sValue.m_uValue.m_sTextValue.m_nStrCount)
			{
				CSTCPrint("ǰ�����%s�ı����Ƿ�����ַ���888888��ǰ%d���ַ�\n", g_sLight[EM_UDIPANEL_TEXTSTART + j], g_TextCnt);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"��ʮ�߲�����ʧ��");			
			}
			else
			{
				CSTCPrint("ǰ�����%s�ı����Ƿ�����ַ���888888\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"��ʮ�˲�����ʧ��");
			}

			strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"         ");
			
			sValue.m_uValue.m_sTextValue.m_nStrCount = 9;
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i,&sValue),"�ڶ�ʮ������ʧ��");
			
			if(g_TextCnt < sValue.m_uValue.m_sTextValue.m_nStrCount)
			{
				CSTCPrint("ǰ�����%s�ı����Ƿ�ΪϨ��״̬\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"�ڶ�ʮһ������ʧ��");			
			}
			else
			{
				CSTCPrint("ǰ�����%s�ı��Ƶ�ǰ�Ÿ��ַ��Ƿ�ΪϨ��״̬\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"�ڶ�ʮ��������ʧ��");
			}

			strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"SCAN");
			
			sValue.m_uValue.m_sTextValue.m_nStrCount = 4;
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i,&sValue),"�ڶ�ʮ�Ĳ�����ʧ��");
			
			if(g_TextCnt < sValue.m_uValue.m_sTextValue.m_nStrCount)
			{
				CSTCPrint("ǰ�����%s�ı����Ƿ�����ַ���SCAN��ǰ%d���ַ�\n", g_sLight[EM_UDIPANEL_TEXTSTART + j], g_TextCnt);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"�ڶ�ʮ�岽����ʧ��");			
			}
			else
			{
				CSTCPrint("ǰ�����%s�ı����Ƿ�����ַ���SCAN\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"�ڶ�ʮ��������ʧ��");
			}	

			strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"C0123");
			
			sValue.m_uValue.m_sTextValue.m_nStrCount = 5;
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i,&sValue),"�ڶ�ʮ�˲�����ʧ��");
			
			if(g_TextCnt < sValue.m_uValue.m_sTextValue.m_nStrCount)
			{
				CSTCPrint("ǰ�����%s�ı����Ƿ�����ַ���C0123��ǰ%d���ַ�\n", g_sLight[EM_UDIPANEL_TEXTSTART + j], g_TextCnt);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"�ڶ�ʮ�Ų�����ʧ��");			
			}
			else
			{
				CSTCPrint("ǰ�����%s�ı����Ƿ�����ַ���C0123\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����ʮ������ʧ��");
			}
			
			strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"R34567");
			
			sValue.m_uValue.m_sTextValue.m_nStrCount = 6;
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i,&sValue),"����ʮ��������ʧ��");
			
			if(g_TextCnt < sValue.m_uValue.m_sTextValue.m_nStrCount)
			{
				CSTCPrint("ǰ�����%s�ı����Ƿ�����ַ���R34567��ǰ%d���ַ�\n", g_sLight[EM_UDIPANEL_TEXTSTART + j], g_TextCnt);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����ʮ��������ʧ��");			
			}
			else
			{
				CSTCPrint("ǰ�����%s�ı����Ƿ�����ַ���R34567\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����ʮ�Ĳ�����ʧ��");
			}

			strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"A78");
			
			sValue.m_uValue.m_sTextValue.m_nStrCount = 3;
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i,&sValue),"����ʮ��������ʧ��");
			
			if(g_TextCnt < sValue.m_uValue.m_sTextValue.m_nStrCount)
			{
				CSTCPrint("ǰ�����%s�ı����Ƿ�����ַ���A78��ǰ%d���ַ�\n", g_sLight[EM_UDIPANEL_TEXTSTART + j], g_TextCnt);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����ʮ�߲�����ʧ��");			
			}
			else
			{
				CSTCPrint("ǰ�����%s�ı����Ƿ�����ַ���A78\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����ʮ�˲�����ʧ��");
			}

			sValue.m_uValue.m_sTextValue.m_nStrCount = 0;
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i,&sValue),"��ʾ�ַ�����Ϊ0 ҲӦ�óɹ�");
			
			CSTCPrint("ǰ������ı����Ƿ����κ��ַ����\n");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����0 ���ַ����ʧ��");

			strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"H307");
			
			sValue.m_uValue.m_sTextValue.m_nStrCount = 4;
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i,&sValue),"����ʮ�Ų�����ʧ��");
			
			if(g_TextCnt < sValue.m_uValue.m_sTextValue.m_nStrCount)
			{
				CSTCPrint("ǰ�����%s�ı����Ƿ�����ַ���H307��ǰ%d���ַ�\n", g_sLight[EM_UDIPANEL_TEXTSTART + j], g_TextCnt);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����ʮһ������ʧ��");			
			}
			else
			{
				CSTCPrint("ǰ�����%s�ı����Ƿ�����ַ���H307\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����ʮ��������ʧ��");
			}

			strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"PORT");
			
			sValue.m_uValue.m_sTextValue.m_nStrCount = 4;
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i,&sValue),"����ʮ�Ų�����ʧ��");
			
			if(g_TextCnt < sValue.m_uValue.m_sTextValue.m_nStrCount)
			{
				CSTCPrint("ǰ�����%s�ı����Ƿ�����ַ���PORT��ǰ%d���ַ�\n", g_sLight[EM_UDIPANEL_TEXTSTART + j], g_TextCnt);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����ʮһ������ʧ��");			
			}
			else
			{
				CSTCPrint("ǰ�����%s�ı����Ƿ�����ַ���PORT\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����ʮ��������ʧ��");
			}
		}
	}
		
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelDisplay
//@DESCRIPTION:�����ܷ�ɹ�����ʱ���ı�����ʾ���� 
//@PRECONDITION:ǰ����ѳɹ���ʼ���Ҹ�ƽ̨֧�ֵ��ı�����ʾ�����������ļ�����
//@INPUT:1����ʾ�豸������ֵ�Ϸ�
//@INPUT:2��CSUDIPanelLightValule_S����ָ��psValueֵ�Ϸ�
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@REMARK:g_PanelCntΪƽ̨֧�ֵ�������������ʱ����ʾ��ʽ��ƽ̨����
//@EXECUTIONFLOW:1������Ҫ��ʾ����ʾ������ΪEM_UDIPANEL_TEXT0����ʾ����ΪEM_UDIPANEL_TEXT_TIME��Ҫ��ʾ������Ϊ19991111111111��Ҫ��ʾ���ַ�����Ϊ14
//@EXECUTIONFLOW:2��ErrCode = CSUDIPanelDisplay(0,psValue)
//@EXECUTIONFLOW:3���������ļ�ָ�����ı��Ʋ����ڣ�������ErrCode == CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED��ִ�е�12��
//@EXECUTIONFLOW:4���������ļ�ָ��EM_UDIPANEL_TEXT0�ƴ��ڣ�������ErrCode == CSUDI_SUCCESS��ִ��5--11��
//@EXECUTIONFLOW:5������ǰ����ʱ����ʾ����Ϊ���õ�ʱ������
//@EXECUTIONFLOW:6���޸�Ҫ��ʾ����Ϊ20000101010101
//@EXECUTIONFLOW:7���ٴε���CSUDIPanelDisplay������������CSUDI_SUCCESS�������������ʾ���ݱ�Ϊ�޸ĺ������
//@EXECUTIONFLOW:8���޸�Ҫ��ʾ����Ϊ20080808080808
//@EXECUTIONFLOW:9���ٴε���CSUDIPanelDisplay������������CSUDI_SUCCESS�������������ʾ���ݱ�Ϊ�޸ĺ������
//@EXECUTIONFLOW:10���޸�Ҫ��ʾ����Ϊ12345678901234
//@EXECUTIONFLOW:11���ٴε���CSUDIPanelDisplay������������CSUDI_SUCCESS�������������ʾ���ݱ�Ϊ�޸ĺ������
//@EXECUTIONFLOW:12����EM_UDIPANEL_TEXTSTART--EM_UDIPANEL_TEXTALL�ĵ��ظ�ִ�����ϲ��衣
//@EXECUTIONFLOW:13�������д��ڵ����ID�ظ����ϲ��衣
CSUDI_BOOL CSTC_PANEL_IT_PanelDisplay_0002(void)
{	
	int i;
	int j;
	CSUDI_Error_Code ErrCode;
	CSUDIPanelLightValule_S sValue;
	
	for(i = 0; i < g_PanelCnt; ++i)
	{
		CSTCPrint("���²���ʱ���ı�����ʾ�����ƽ̨û��ʱ�������м��\" : \"����ʾ\n");
		CSTKWaitAnyKey();
		
		for(j = 1; j < EM_UDIPANEL_TEXTALL - EM_UDIPANEL_TEXTSTART; ++j)
		{
			sValue.m_eLightName = g_aeLight[EM_UDIPANEL_TEXTSTART + j];
			
			sValue.m_uValue.m_sTextValue.m_eDisplayType =  EM_UDIPANEL_TEXT_TIME;
			
			strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"19991111111111");
			
			sValue.m_uValue.m_sTextValue.m_nStrCount = 14;
			
			ErrCode = CSUDIPanelDisplay(i,&sValue);

			if(CSUDI_TRUE != g_abSuppLight[EM_UDIPANEL_TEXTSTART + j])
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == ErrCode,"����������ʧ��");
				
				continue;
			}
			else
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode,"���Ĳ�����ʧ��");

				CSTCPrint("ǰ�����%s�ı����Ƿ����1999��11��11��11ʱ11��11�������ʱ������(��ʱ��11:11)\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"���岽����ʧ��");			
	
			}

			strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"20000101010101");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i,&sValue),"���߲�����ʧ��");

			CSTCPrint("ǰ�����%s�ı����Ƿ�����ַ���2000��01��01��01ʱ01��01�������ʱ������(��ʱ��01:01)\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"���߲�����ʧ��");	

			//��Ӳ���Ҫ����ʾ�ַ�����Ϊ0 �����
			sValue.m_uValue.m_sTextValue.m_nStrCount = 0;
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i,&sValue),"��ʾ�ַ�����Ϊ0 ҲӦ�óɹ�");
			
			CSTCPrint("ǰ������ı����Ƿ����κ��ַ����\n");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����0 ���ַ����ʧ��");

			//
			strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"20080808080808");
			sValue.m_uValue.m_sTextValue.m_nStrCount = 14;
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i,&sValue),"�ھŲ�����ʧ��");

			CSTCPrint("ǰ�����%s�ı����Ƿ�����ַ���2008��08��08��08ʱ08��08�������ʱ������(��ʱ��08:08)\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"�ھŲ�����ʧ��");			

			strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"12345678901234");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i,&sValue),"��ʮһ������ʧ��");

			CSTCPrint("ǰ�����%s�ı����Ƿ�����ַ���1234��56��78��90ʱ12��34�������ʱ������(��ʱ��90:12��00:12)\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"��ʮһ������ʧ��");			

		}
	}
		
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelDisplay
//@DESCRIPTION:�����ܷ�ɹ����ÿ��Ƶ���ʾ״̬�����
//@PRECONDITION:ǰ����ѳɹ���ʼ��
//@INPUT:1����ʾ�豸������ֵ�Ϸ�
//@INPUT:2�������CSUDIPanelLightValule_S����ָ��psValueֵ�ǿ�  
//@EXPECTATION:ÿһ�������Ԥ�ڽ��
//@REMARK:g_PanelCntΪƽ̨֧�ֵ������
//@EXECUTIONFLOW:1������CSUDIPanelLightValule_S����ָ��psValue����ʾ������ΪEM_UDIPANEL_SIGNAL����ʾ״̬ΪEM_UDIPANEL_CTRL_ON
//@EXECUTIONFLOW:2��ErrCode = CSUDIPanelDisplay(0,psValue)
//@EXECUTIONFLOW:3�����ƽ̨��֧�ָÿ��Ƶ�������ErrCode == CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED��ִ�е�34��
//@EXECUTIONFLOW:4�����ƽ̨֧�ָÿ��Ƶ���ִ�����²���
//@EXECUTIONFLOW:5���ÿ��ƵƱ������EM_UDIPANEL_CTRL_ON����,����ErrCode == CSUDI_SUCCESS
//@EXECUTIONFLOW:6������ǰ����ϸÿ��ƵƱ�����
//@EXECUTIONFLOW:7������CSUDIPanelLightValule_S����ָ��psValue����ʾ״̬ΪEM_UDIPANEL_CTRL_OFF
//@EXECUTIONFLOW:8������CSUDIPanelDisplay(0,psValue)��������CSUDI_SUCCESS��ǰ����ϸÿ��Ƶ�ΪϨ��״̬
//@EXECUTIONFLOW:9������CSUDIPanelLightValule_S����ָ��psValue����ʾ״̬ΪEM_UDIPANEL_CTRL_FALSH
//@EXECUTIONFLOW:10������CSUDIPanelDisplay(0,psValue)��������CSUDI_SUCCESS��ǰ����ϸÿ��Ƶ�ΪĬ����ɫ��˸״̬
//@EXECUTIONFLOW:11������CSUDIPanelLightValule_S����ָ��psValue����ʾ״̬ΪEM_UDIPANEL_CTRL_COLOR2
//@EXECUTIONFLOW:12��ErrCode = CSUDIPanelDisplay(0,psValue)
//@EXECUTIONFLOW:13������ÿ��Ƶƾ���EM_UDIPANEL_CTRL_COLOR2����������ErrCode == CSUDI_SUCCESS
//@EXECUTIONFLOW:14������ǰ����ϸÿ��Ƶ�Ϊ�ڶ�����ɫ��ʾ״̬
//@EXECUTIONFLOW:15������ÿ��ƵƲ�����EM_UDIPANEL_CTRL_COLOR2����������ErrCode == CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:16������ÿ��Ƶƾ���EM_UDIPANEL_CTRL_COLOR2������ִ�в���17~18����������ֱ��ִ�в���19
//@EXECUTIONFLOW:17������CSUDIPanelLightValule_S����ָ��psValue����ʾ״̬ΪEM_UDIPANEL_CTRL_OFF
//@EXECUTIONFLOW:18������CSUDIPanelDisplay(0,psValue)��������CSUDI_SUCCESS��ǰ����ϸÿ��Ƶ�ΪϨ��״̬
//@EXECUTIONFLOW:19������CSUDIPanelLightValule_S����ָ��psValue����ʾ״̬ΪEM_UDIPANEL_CTRL_COLOR2FLASH
//@EXECUTIONFLOW:20��ErrCode = CSUDIPanelDisplay(0,psValue)
//@EXECUTIONFLOW:21������ÿ��Ƶƾ���EM_UDIPANEL_CTRL_COLOR2FLASH����������ErrCode == CSUDI_SUCCESS
//@EXECUTIONFLOW:22������ǰ����ϸÿ��Ƶ�Ϊ�ڶ�����ɫ��˸״̬
//@EXECUTIONFLOW:23������ÿ��ƵƲ�����EM_UDIPANEL_CTRL_COLOR2FLASH����������ErrCode == CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:24������CSUDIPanelLightValule_S����ָ��psValue����ʾ״̬ΪEM_UDIPANEL_CTRL_COLORSWITCH
//@EXECUTIONFLOW:25��ErrCode = CSUDIPanelDisplay(0,psValue)
//@EXECUTIONFLOW:26������ÿ��Ƶƾ���EM_UDIPANEL_CTRL_COLORSWITCH����������ErrCode == CSUDI_SUCCESS
//@EXECUTIONFLOW:27������ǰ����ϸÿ��Ƶ�Ϊ������ɫ������˸״̬
//@EXECUTIONFLOW:28������ÿ��ƵƲ�����EM_UDIPANEL_CTRL_COLORSWITCH����������ErrCode == CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:29����EM_UDIPANEL_CTRLSTART--EM_UDIPANEL_CTRLALL�����е��ظ����ϲ���
//@EXECUTIONFLOW:30�������д��ڵ����ID�ظ����ϲ��衣
CSUDI_BOOL CSTC_PANEL_IT_PanelDisplay_0003(void)
{
	int i;
	int j;
	
	CSUDI_Error_Code ErrCode;
	CSUDIPanelLightValule_S sValue;
	
	for(i = 0; i < g_PanelCnt; ++i)
	{
		for(j = 1; j < EM_UDIPANEL_CTRLALL - EM_UDIPANEL_CTRLSTART; ++j)
		{
			sValue.m_eLightName = g_aeLight[EM_UDIPANEL_CTRLSTART + j];
			sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_ON;

			ErrCode = CSUDIPanelDisplay(i, &sValue);

			if(CSUDI_TRUE != g_abSuppLight[EM_UDIPANEL_CTRLSTART + j])
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == ErrCode, "���ƽ̨��֧�ָÿ��Ƶ���Ӧ�÷���CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED");
				
				continue;
			}

			CSTCPrint("���²���%s���ƵƵ���ʾ״̬\n", g_sLight[EM_UDIPANEL_CTRLSTART + j]);
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode, "���ÿ��Ƶ�Ϊ����״̬ʧ��");

			CSTCPrint("ǰ�����%s���Ƶ��Ƿ�Ϊ����״̬\n", g_sLight[EM_UDIPANEL_CTRLSTART + j]);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "���ÿ��Ƶ�Ϊ����״̬ʧ��");

			sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_OFF;
		
			ErrCode = CSUDIPanelDisplay(i, &sValue);

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode, "���ÿ��Ƶ�ΪϨ��״̬ʧ��");

			CSTCPrint("ǰ�����%s���Ƶ��Ƿ�ΪϨ��״̬\n", g_sLight[EM_UDIPANEL_CTRLSTART + j]);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "���ÿ��Ƶ�ΪϨ��״̬ʧ��");
			
			sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_FLASH;
		
			ErrCode = CSUDIPanelDisplay(i, &sValue);

			if(0 != (g_CtrlFunMask[j]&EM_UDIPANEL_CTRL_FLASH))
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode, "���ÿ��Ƶ�ΪĬ����ɫ��˸״̬ʧ��");

				CSTCPrint("ǰ�����%s���Ƶ��Ƿ�ΪĬ����ɫ��˸\n", g_sLight[EM_UDIPANEL_CTRLSTART + j]);
			
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "���ÿ��Ƶ�ΪĬ����ɫ��˸״̬ʧ��");			
			}
			else
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == ErrCode, "�����֧����Ĭ����ɫ��˸Ӧ�÷���CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED");
			}
			
			sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_COLOR2;

			ErrCode = CSUDIPanelDisplay(i, &sValue);
			
			if(0 != (g_CtrlFunMask[j]&EM_UDIPANEL_CTRL_COLOR2))
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode, "���ÿ��Ƶ��Եڶ�����ɫ��ʾʧ��");

				CSTCPrint("ǰ�����%s���Ƶ��Ƿ��Եڶ�����ɫ��ʾ\n", g_sLight[EM_UDIPANEL_CTRLSTART + j]);
			
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "���ÿ��Ƶ��Եڶ�����ɫ��ʾʧ��");	

				sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_OFF;
		
				ErrCode = CSUDIPanelDisplay(i, &sValue);

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode, "���ÿ��Ƶ�ΪϨ��״̬ʧ��");

				CSTCPrint("ǰ�����%s���Ƶ��Ƿ�ΪϨ��״̬\n", g_sLight[EM_UDIPANEL_CTRLSTART + j]);
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "���ÿ��Ƶ�ΪϨ��״̬ʧ��");
			}
			else
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == ErrCode, "�����֧���Եڶ�����ɫ��ʾӦ�÷���CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED");
			}
	
			sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_COLOR2FLASH;
			
			ErrCode = CSUDIPanelDisplay(i, &sValue);

			if(0 != (g_CtrlFunMask[j]&EM_UDIPANEL_CTRL_COLOR2FLASH))
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode, "���ÿ��Ƶ��Եڶ�����ɫ��˸ʧ��");

				CSTCPrint("ǰ�����%s���Ƶ��Ƿ��Եڶ�����ɫ��˸\n", g_sLight[EM_UDIPANEL_CTRLSTART + j]);
			
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "���ÿ��Ƶ��Եڶ�����ɫ��˸ʧ��");			
			}
			else
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == ErrCode, "�����֧���Եڶ�����ɫ��˸Ӧ�÷���CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED");
			}
			
			sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_COLORSWITCH;
			
			ErrCode = CSUDIPanelDisplay(i, &sValue);

			if(0 != (g_CtrlFunMask[j]&EM_UDIPANEL_CTRL_COLORSWITCH))
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode, "���ÿ��Ƶ�������ɫ������˸ʧ��");

				CSTCPrint("ǰ�����%s���Ƶ��Ƿ���������ɫ������˸\n", g_sLight[EM_UDIPANEL_CTRLSTART + j]);
			
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "���ÿ��Ƶ�������ɫ������˸ʧ��");			
			}
			else
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == ErrCode, "�����֧��������ɫ������˸Ӧ�÷���CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED");
			}	
		}
	}
		
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelDisplay
//@DESCRIPTION:�����ܷ�ɹ�������ϵ���ʾ״̬�����
//@PRECONDITION:ǰ����ѳɹ���ʼ��
//@INPUT:1����ʾ�豸������ֵ�Ϸ�
//@INPUT:2�������CSUDIPanelLightValule_S����ָ��psValue�ǿ�
//@EXPECTATION:ÿһ�������Ԥ�ڽ��
//@REMARK:g_PanelCntΪƽ̨֧�ֵ��������g_SuitcntΪ��ϵƵ��ӵƸ������������ļ�����
//@EXECUTIONFLOW:1������psValue����ʾ������EM_UDIPANEL_SUIT0����ʾֵΪȫ1
//@EXECUTIONFLOW:2��ErrCode = CSUDIPanelDisplay(0,psValue)
//@EXECUTIONFLOW:3�����ƽ̨��֧�ָ���ϵ�������ErrCode == CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED��ִ�е�14��
//@EXECUTIONFLOW:4�����ƽ̨֧�ָ���ϵ�������ErrCode == CSUDI_SUCCESS��ִ�����²���
//@EXECUTIONFLOW:5������ǰ����ϸ���ϵƵ������ӵƱ�����
//@EXECUTIONFLOW:6������psValue����ʾֵΪȫ0
//@EXECUTIONFLOW:7������CSUDIPanelDisplay(0,psValue)��������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������ǰ����ϸ���ϵƵ������ӵ�Ϩ��
//@EXECUTIONFLOW:9������psValue����ʾֵĩβλΪ1������ȫΪ0
//@EXECUTIONFLOW:10������CSUDIPanelDisplay(0,psValue)��������CSUDI_SUCCESS
//@EXECUTIONFLOW:11������1λpsValue����ʾֵ
//@EXECUTIONFLOW:12���ظ�ִ��10--11��g_Suitcnt��
//@EXECUTIONFLOW:13������ǰ�����ʼ��ֻ��һ������ʾ��תȦЧ��
//@EXECUTIONFLOW:14����EM_UDIPANEL_SUITSTART--EM_UDIPANEL_SUITALL��������ϵ��ظ����ϲ���
//@EXECUTIONFLOW:15�������д��ڵ����ID�ظ����ϲ���
CSUDI_BOOL CSTC_PANEL_IT_PanelDisplay_0004(void)
{
	int i;
	int j;
	int k;

	CSUDI_Error_Code ErrCode;
	
	CSUDIPanelLightValule_S sValue;
	
	for(i = 0; i < g_PanelCnt; ++i)
	{
		for(j = 1; j < EM_UDIPANEL_SUITALL - EM_UDIPANEL_SUITSTART;++j)
		{
			sValue.m_eLightName = g_aeLight[EM_UDIPANEL_SUITSTART + j];

			sValue.m_uValue.m_sSuitValue.m_dwSuitValueMask = (1<<g_SuitCnt) - 1;

			ErrCode = CSUDIPanelDisplay(i, &sValue);

			if(CSUDI_TRUE != g_abSuppLight[EM_UDIPANEL_SUITSTART + j])
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == ErrCode,"����������ʧ��");
				CSTCPrint("Not supported !!!!\n");
				continue;
			}

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode,"���Ĳ�����ʧ��");
			
			CSTCPrint("ǰ�����%s��ϵ��Ƿ�ȫ������\t", g_sLight[EM_UDIPANEL_SUITSTART + j]);
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"���岽����ʧ��");			

			sValue.m_uValue.m_sSuitValue.m_dwSuitValueMask = 0;
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i, &sValue),"���߲�����ʧ��");
			
			CSTCPrint("ǰ�����%s��ϵ��Ƿ�ȫ��Ϩ��\t", g_sLight[EM_UDIPANEL_SUITSTART + j]);
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"�ڰ˲�����ʧ��");	

			k = 0; 

			while(k < g_SuitCnt)
			{
				sValue.m_uValue.m_sSuitValue.m_dwSuitValueMask = 1<<k;

				k++;

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i, &sValue),"��ʮ������ʧ��");

			}
			
			CSTCPrint("ǰ�����%s��ϵ��Ƿ�ʼ��ֻ��һ������ʾ��תȦЧ��\t", g_sLight[EM_UDIPANEL_SUITSTART + j]);
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"��ʮ��������ʧ��");	

		}
	}
		
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelDisplay
//@DESCRIPTION:�����豸�Ŵ���ʱ�����
//@PRECONDITION:ǰ����ѳɹ���ʼ��
//@INPUT:1����ʾ�豸������Ϊ-1
//@INPUT:2������ǿյ�CSUDIPanelLightValule_S����ָ��psValue
//@EXPECTATION:����CSUDIPANEL_ERROR_INVALID_DEVICE_ID
//@REMARK:g_PanelCntΪƽ̨֧�ֵ������
//@EXECUTIONFLOW:1������CSUDIPanelDisplay(-1,psValue)��������CSUDIPANEL_ERROR_INVALID_DEVICE_ID
CSUDI_BOOL CSTC_PANEL_IT_PanelDisplay_0005(void)
{
	CSUDIPanelLightValule_S  sValue;

	sValue.m_eLightName = EM_UDIPANEL_POWER;
	
	sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_ON;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_INVALID_DEVICE_ID == CSUDIPanelDisplay(-1,&sValue),"��������ʧ��");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelDisplay
//@DESCRIPTION:������ʾ�豸�������Ŵ���ƽ̨֧�ֵ�����豸�������
//@PRECONDITION:ǰ����ѳɹ���ʼ��
//@INPUT:1�������ʾ����ֵ�Ϸ�    
//@INPUT:2������ǿյ�CSUDIPanelLightValule_S����ָ��psValue
//@EXPECTATION:����CSUDIPANEL_ERROR_INVALID_DEVICE_ID
//@REMARK:g_PanelCntΪƽ̨֧�ֵ���������������ļ�����
//@EXECUTIONFLOW:1������CSUDIPanelDisplay(g_PanelCnt,psValue)�����õ��ķ���ֵ����ΪCSUDIPANEL_ERROR_INVALID_DEVICE_ID 
//@EXECUTIONFLOW:2������CSUDIPanelDisplay(g_PanelCnt + 1,psValue)�����õ��ķ���ֵ����ΪCSUDIPANEL_ERROR_INVALID_DEVICE_ID 
//@EXECUTIONFLOW:3������CSUDIPanelDisplay(g_PanelCnt + 100,psValue)�����õ��ķ���ֵ����ΪCSUDIPANEL_ERROR_INVALID_DEVICE_ID 
CSUDI_BOOL CSTC_PANEL_IT_PanelDisplay_0006(void)
{
	CSUDIPanelLightValule_S  sValue;

	sValue.m_eLightName = EM_UDIPANEL_POWER;
	
	sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_ON;
		
	CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_INVALID_DEVICE_ID == CSUDIPanelDisplay(g_PanelCnt,&sValue),"��һ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_INVALID_DEVICE_ID == CSUDIPanelDisplay(g_PanelCnt + 1,&sValue),"�ڶ�������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_INVALID_DEVICE_ID == CSUDIPanelDisplay(g_PanelCnt + 100,&sValue),"����������ʧ��");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelDisplay
//@DESCRIPTION:����CSUDIPanelLightValue_S����ָ��psValue = CSUDI_NULLʱ�����
//@PRECONDITION:ǰ����ѳɹ���ʼ��
//@INPUT:1����ʾ�豸������ֵ�Ϸ�
//@INPUT:2��CSUDIPanelLightValule_S����ָ��psValue = CSUDI_NULL
//@EXPECTATION:����CSUDIPANEL_ERROR_BAD_PARAMETER  
//@REMARK:g_PanelCntΪƽ̨֧�ֵ������
//@EXECUTIONFLOW:1������CSUDIPanelDisplay(0,CSUDI_NULL)��������CSUDIPANEL_ERROR_BAD_PARAMETER  
//@EXECUTIONFLOW:2�������д��ڵ����ID�ظ����ϲ���
CSUDI_BOOL CSTC_PANEL_IT_PanelDisplay_0007(void)
{
	int i;

	for(i = 0; i < g_PanelCnt; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelDisplay(i,CSUDI_NULL),"��������ʧ��");
	}
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
 }
//@CASEGROUP:CSUDIPanelDisplay
//@DESCRIPTION:������ʾ�������Ƿ������
//@PRECONDITION:ǰ����ѳɹ���ʼ��
//@INPUT:1����ʾ�豸������ֵ�Ϸ�
//@INPUT:2��CSUDIPanelLightValule_S����ָ��psValueֵ�ǿ�
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@REMARK:g_PanelCntΪƽ̨֧�ֵ������
//@EXECUTIONFLOW:1��psValue->m_eLightName = EM_UDIPANEL_CTRLSTARTʱ����CSUDIPanelDisplay(0,psValue)��������CSUDIPANEL_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:2��psValue->m_eLightName = EM_UDIPANEL_CTRLALLʱ����CSUDIPanelDisplay(0,psValue)��������CSUDIPANEL_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:3��psValue->m_eLightName = EM_UDIPANEL_TEXTSTARTʱ����CSUDIPanelDisplay(0,psValue)��������CSUDIPANEL_ERROR_BAD_PARAMETER 
//@EXECUTIONFLOW:4��psValue->m_eLightName = EM_UDIPANEL_TEXTALLʱ����CSUDIPanelDisplay(0,psValue)��������CSUDIPANEL_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5��psValue->m_eLightName = EM_UDIPANEL_SUITSTARTʱ����CSUDIPanelDisplay(0,psValue)��������CSUDIPANEL_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6��psValue->m_eLightName = EM_UDIPANEL_SUITALLʱ����CSUDIPanelDisplay(0,psValue)��������CSUDIPANEL_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:7��psValue->m_eLightName = EM_UDIPANEL_MAXʱ����CSUDIPanelDisplay(0,psValue)��������CSUDIPANEL_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:8�������д��ڵ����ID�ظ����ϲ���
CSUDI_BOOL CSTC_PANEL_IT_PanelDisplay_0008(void)
{
	int i;
	
	CSUDIPanelLightValule_S  sValue;
	
	for(i = 0; i < g_PanelCnt; ++i)
	{
		sValue.m_eLightName = EM_UDIPANEL_CTRLSTART;

		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelDisplay(i,&sValue),"��һ������ʧ��");

		sValue.m_eLightName = EM_UDIPANEL_CTRLALL;

		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelDisplay(i,&sValue),"�ڶ�������ʧ��");

		sValue.m_eLightName = EM_UDIPANEL_TEXTSTART;

		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelDisplay(i,&sValue),"����������ʧ��");

		sValue.m_eLightName = EM_UDIPANEL_TEXTALL;

		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelDisplay(i,&sValue),"���Ĳ�����ʧ��");

		sValue.m_eLightName = EM_UDIPANEL_SUITSTART;

		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelDisplay(i,&sValue),"���岽����ʧ��");

		sValue.m_eLightName = EM_UDIPANEL_SUITALL;

		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelDisplay(i,&sValue),"����������ʧ��");

		sValue.m_eLightName = EM_UDIPANEL_MAX;

		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelDisplay(i,&sValue),"���߲�����ʧ��");

	}
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelDisplay
//@DESCRIPTION:�����ظ�����CSUDIPanelDisplayʱ�����
//@PRECONDITION:ǰ����ѳɹ���ʼ��
//@INPUT:1����ʾ�豸������ֵ�Ϸ�
//@INPUT:2�������CSUDIPanelLightValule_S����ָ��psValueֵ�Ϸ�
//@EXPECTATION:ÿһ�������Ԥ�ڽ��
//@REMARK:g_RepeatCntΪ�ظ�ִ�еĴ���
//@EXECUTIONFLOW:1��������ʾ������Ϊƽ̨֧�ֵĿ��Ƶ����͡���ʾ״̬ΪEM_UDIPANEL_CTRL_OFF����������ʮ��CSUDIPanelDisplay����������ÿ�ζ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:2��������ʾ״̬ΪEM_UDIPANEL_CTRL_ON����������ʮ��CSUDIPanelDisplay��������ÿ�η���CSUDI_SUCCESS
//@EXECUTIONFLOW:3���ظ�ִ��1--2��g_RepeatCnt�Σ���������ϸÿ��Ƶ�ʱ��ʱ�������ÿ��Ƶ�Ϊ����״̬
//@EXECUTIONFLOW:4��������ʾ״̬ΪEM_UDIPANEL_CTRL_ON������CSUDIPanelDisplay������������CSUDI_SUCCESS
//@EXECUTIONFLOW:5��������ʾ״̬ΪEM_UDIPANEL_CTRL_OFF������CSUDIPanelDisplay��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:6���ظ�ִ��4--5��g_RepeatCnt�Σ���������ϸÿ��Ƶ�ʱ��ʱ�������ΪϨ��״̬
CSUDI_BOOL CSTC_PANEL_IT_PanelDisplay_0009(void)
{
	int i;

	int j;
	
	CSUDIPanelLightValule_S  sValue;
	
	memset(&sValue, 0, sizeof(sValue));
	sValue.m_eLightName = EM_UDIPANEL_SIGNAL;

	for(i=EM_UDIPANEL_CTRLSTART+1; i<EM_UDIPANEL_CTRLALL; i++)
	{
		if(g_abSuppLight[i])
		{
			sValue.m_eLightName = i;
			break;
		}
	}
	
	CSTCPrint("������ǰ�����%s���Ƿ�ʱ��ʱ����ʱ����ԼΪ2�������Ϊ����״̬�������������\n",g_sLight[sValue.m_eLightName - EM_UDIPANEL_CTRLSTART]);

	CSTKWaitAnyKey();
	
	for(i = 0; i < REPEAT_TIME; ++i)
	{
		sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_OFF;
		for(j = 0; j < 10; ++j)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(0,&sValue),"��һ������ʧ��");
		}

		CSUDIOSThreadSleep(2000);

		sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_ON;
		for(j = 0; j < 10; ++j)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(0,&sValue),"�ڶ�������ʧ��");
		}

		CSUDIOSThreadSleep(2000);
	}
	
	CSTCPrint("ǰ�����%s���Ƿ�ʱ��ʱ����ʱ����ԼΪ2�������Ϊ����״̬\n",g_sLight[sValue.m_eLightName - EM_UDIPANEL_CTRLSTART]);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����������ʧ��");	
	
	CSTCPrint("������ǰ�����%s���Ƿ�ʱ��ʱ����ʱ����ԼΪ2�����ΪϨ��״̬�������������\n",g_sLight[sValue.m_eLightName - EM_UDIPANEL_CTRLSTART]);

	CSTKWaitAnyKey();

	for(i = 0; i < REPEAT_TIME; ++i)
	{
		sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_ON;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(0,&sValue),"���Ĳ�����ʧ��");
		
		CSUDIOSThreadSleep(2000);

		sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_OFF;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(0,&sValue),"���岽����ʧ��");

		CSUDIOSThreadSleep(2000);
	}
	
	CSTCPrint("ǰ�����%s���Ƶ��Ƿ�ʱ��ʱ����ʱ����ԼΪ2�����ΪϨ��״̬\n",g_sLight[sValue.m_eLightName - EM_UDIPANEL_CTRLSTART]);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����������ʧ��");	
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;	
}

//@CASEGROUP:CSUDIPanelDisplay
//@DESCRIPTION:���Բ�ͬ��������ͬһ����ʾ�Ƶ����
//@PRECONDITION:ǰ����ѳɹ���ʼ��
//@INPUT:�Ϸ����������
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@REMARK:��
//@EXECUTIONFLOW:1�������������� 
//@EXECUTIONFLOW:2���ڵ�һ���������ظ�����TEXT0�ı��Ƶ���ʾ״̬Ϊabcd
//@EXECUTIONFLOW:3���ڵڶ����������ظ�����TEXT0�ı��Ƶ���ʾ״̬Ϊ8888
//@EXECUTIONFLOW:4������ǰ���TEXT0�ı��Ƶ���ʾ���ݷ����仯
//@EXECUTIONFLOW:5���ر�TEXT0����ʾ����
//@EXECUTIONFLOW:6����������������
CSUDI_BOOL CSTC_PANEL_IT_PanelDisplay_0010(void)
{
	CSUDIPanelLightValule_S  sTextValue;

	int nPriority = 128;
	
	int nParam = 0;
	
	CSUDI_HANDLE hThreadhandle1 = CSUDI_NULL;
	
	CSUDI_HANDLE hThreadhandle2 = CSUDI_NULL;
	
	g_bThreadEnd1 = CSUDI_FALSE;
	
	g_bThreadEnd2 = CSUDI_FALSE;

	CSTCPrint("������ǰ�����%s�Ƶ�״̬�������������\n",g_sLight[EM_UDIPANEL_TEXT0]);
	
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("PANEL_TestThread1", nPriority , PANEL_TEST_STACK_SIZE,PANEL_TestThread1Entry, &nParam, &hThreadhandle1),"��������ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle1,"�����ɹ����������������������Ϊ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("PANEL_TestThread2", nPriority , PANEL_TEST_STACK_SIZE,PANEL_TestThread2Entry, &nParam, &hThreadhandle2),"��������ʧ��\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle2,"�����ɹ����������������������Ϊ��\n");

	while(CSUDI_TRUE != g_bThreadEnd1&&CSUDI_TRUE != g_bThreadEnd2)
	{
		CSUDIOSThreadSleep(6000);
	}

	CSTCPrint("ǰ�����TEXT0�ı����Ƿ�仯\n");
	CSTCPrint("ǰ�����TEXT0�ı����Ƿ����abcd8 5���ַ�\n");
	CSTCPrint("ǰ�����TEXT0�ı��������ֵ�Ӣ���ַ����Ƿ�����Ұ�abcd ˳�����У��������λ\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"��������ʧ��");

	sTextValue.m_eLightName = EM_UDIPANEL_TEXT0;
	sTextValue.m_uValue.m_sTextValue.m_eDisplayType = EM_UDIPANEL_TEXT_STRING;
	strcpy(sTextValue.m_uValue.m_sTextValue.m_szStr,"    ");
	sTextValue.m_uValue.m_sTextValue.m_nStrCount = 4;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(0,  &sTextValue),"�ر���ʾ��ʧ��\n");

	CSTK_FATAL_POINT
	{
		if (hThreadhandle1 != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle1), "ɾ������ʧ��\n");
			   hThreadhandle1 = CSUDI_NULL;
		}
		if (hThreadhandle2 != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle2), "ɾ������ʧ��\n");
			   hThreadhandle2 = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelDisplay
//@DESCRIPTION:�����ı���CSUDIPanelLightValule_S ֵ��ֵ��������
//@PRECONDITION:ǰ����ѳɹ���ʼ��
//@INPUT:CSUDIPanelLightValule_S ֵ��ֵ����
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@REMARK:��
//@EXECUTIONFLOW:1������psValue����ʾ������Ϊ�ı�����
//@EXECUTIONFLOW:2��ErrCode = CSUDIPanelDisplay(0,psValue)
//@EXECUTIONFLOW:3��ϣ��Errcode ��Ϊ�ɹ�
CSUDI_BOOL CSTC_PANEL_IT_PanelDisplay_0011(void)
{
	CSUDIPanelLightValule_S sValue;
	int i = 0;
	int j = 0;
	CSUDI_Error_Code ErrCode = CSUDI_SUCCESS;
	CSUDIPanelTextValue_S sErrorParam[] =
	{
			{EM_UDIPANEL_TEXT_STRING -1, "abc", 1},
			{EM_UDIPANEL_TEXT_TIME + 1, "abcd", 2},
			//{EM_UDIPANEL_TEXT_STRING, (char)"", 3},
			{EM_UDIPANEL_TEXT_STRING, "abcde", 33},
			{EM_UDIPANEL_TEXT_STRING, "abcef", -1},
			{EM_UDIPANEL_TEXT_TIME, "abcefg", 3},
	};

	for(i = 0; i < g_PanelCnt; ++i)
	{
		for(j = 0; j < sizeof(sErrorParam)/sizeof(sErrorParam[0]); j++)
		{
			sValue.m_eLightName = EM_UDIPANEL_TEXT0;
			sValue.m_uValue.m_sTextValue.m_eDisplayType = sErrorParam[j].m_eDisplayType;
			strcpy(sValue.m_uValue.m_sTextValue.m_szStr, sErrorParam[j].m_szStr);
			sValue.m_uValue.m_sTextValue.m_nStrCount = sErrorParam[j].m_nStrCount;
		
			ErrCode = CSUDIPanelDisplay(i, &sValue);
			CSTCPrint("m_eDisplayType = %d, m_szStr = %s, m_nStrCount = %d \n",
					   sErrorParam[j].m_eDisplayType, sErrorParam[j].m_szStr, sErrorParam[j].m_nStrCount);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != ErrCode, "�������ʧ��!");
		}
	}

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelDisplay
//@DESCRIPTION:���Կ��Ƶ�CSUDIPanelLightValule_S ֵ��ֵ��������
//@PRECONDITION:ǰ����ѳɹ���ʼ��
//@INPUT:CSUDIPanelLightValule_S ֵ��ֵ����
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@REMARK:��
//@EXECUTIONFLOW:1������psValue����ʾ������Ϊ��������
//@EXECUTIONFLOW:2��ErrCode = CSUDIPanelDisplay(0,psValue)
//@EXECUTIONFLOW:3��ϣ��Errcode ��Ϊ�ɹ�
CSUDI_BOOL CSTC_PANEL_IT_PanelDisplay_0012(void)
{
	CSUDIPanelLightValule_S sValue;
	int i = 0;
	CSUDI_Error_Code ErrCode = CSUDI_SUCCESS;

	for(i = 0; i < g_PanelCnt; ++i)
	{
		sValue.m_eLightName = EM_UDIPANEL_SIGNAL;
		
		sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = 0;
		ErrCode = CSUDIPanelDisplay(i, &sValue);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != ErrCode, "�������ʧ��1 !");

		sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = -1;
		ErrCode = CSUDIPanelDisplay(i, &sValue);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != ErrCode, "�������ʧ��2 !");

		sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = 64;
		ErrCode = CSUDIPanelDisplay(i, &sValue);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != ErrCode, "�������ʧ��3 !");
	}

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelDisplay
//@DESCRIPTION:������ϵ�CSUDIPanelLightValule_S ֵ��ֵ��������
//@PRECONDITION:ǰ����ѳɹ���ʼ��
//@INPUT:CSUDIPanelLightValule_S ֵ��ֵ����
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@REMARK:��
//@EXECUTIONFLOW:1������psValue����ʾ������Ϊ�������
//@EXECUTIONFLOW:2��ErrCode = CSUDIPanelDisplay(0,psValue)
//@EXECUTIONFLOW:3��ϣ��Errcode ��Ϊ�ɹ�
CSUDI_BOOL CSTC_PANEL_IT_PanelDisplay_0013(void)
{
	CSUDIPanelLightValule_S sValue;
	int i = 0;
	CSUDI_Error_Code ErrCode = CSUDI_SUCCESS;

	for(i = 0; i < g_PanelCnt; ++i)
	{
		sValue.m_eLightName = EM_UDIPANEL_SUIT0;
		
		sValue.m_uValue.m_sSuitValue.m_dwSuitValueMask = -1;
		ErrCode = CSUDIPanelDisplay(i, &sValue);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != ErrCode, "�������ʧ��1 !");

		sValue.m_uValue.m_sSuitValue.m_dwSuitValueMask = 1<<g_SuitCnt;
		ErrCode = CSUDIPanelDisplay(i, &sValue);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != ErrCode, "�������ʧ��2 !");
	}

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelDisplay
//@DESCRIPTION:�����ܷ���ȷ��ʾʱ��㣬��ʱ����ܹ�������ʾ��Ӱ���ַ�����ʱ�����ʾ 
//@PRECONDITION:ǰ����ѳɹ���ʼ���Ҹ�ƽ̨֧�ֵ��ı�����ʾ�����������ļ�����
//@INPUT:1����ʾ�豸������ֵ�Ϸ�
//@INPUT:2��CSUDIPanelLightValule_S����ָ��sValueֵ�Ϸ�
//@EXPECTATION:��������ʾʱ��㡢�ַ�����ʱ��㣬ÿһ���趼���Ԥ�ڽ��
//@REMARK:ʱ���Ϊ��ʾʱ��ʱСʱ�ͷ���֮�������С��
//@EXECUTIONFLOW:1�����ƽ̨��֧�ִ˵����ͣ���ִ�к�������
//@EXECUTIONFLOW:2���������ʱ��㣬����ʱ���Ϊ����״̬
//@EXECUTIONFLOW:3��Ϩ�����ʱ��㣬����ʱ���ΪϨ��״̬
//@EXECUTIONFLOW:4���������ʱ��㣬����ʱ���Ϊ����״̬
//@EXECUTIONFLOW:5���ж��Ƿ�֧����ʾʱ������
//@EXECUTIONFLOW:6��7����֧����ʾʱ�����ݣ�������ʾʱ��������ȷ
//@EXECUTIONFLOW:8��9���ڲ���7������ʾ���ݻ����ϣ�����������ʾʱ��㣬����ʱ�������Ҳ�Ӱ����ǰ��ʾ����
//@EXECUTIONFLOW:10��11���ڲ���9 �Ļ�����Ϩ�����ʱ��㣬����ʱ���ΪϨ��״̬���������ǰ��ʾ�������ݲ���
//@EXECUTIONFLOW:12���ڲ���11 �Ļ����ϵ������ʱ��㣬����ʱ���Ϊ����״̬���������ǰ��ʾ�������ݲ���
//@EXECUTIONFLOW:13���ж��Ƿ�֧����ʾ�ַ�������
//@EXECUTIONFLOW:14��15��16����֧����ʾ�ַ������ݣ���������ʾ�ַ���������ȷ
//@EXECUTIONFLOW:17��18��������Ѿ���ʾ�ַ����Ļ����ϣ�����������ʾʱ��㣬����ʱ���Ϊ����״̬�Ҳ�Ӱ��ԭ���ַ�����ʾ����
//@EXECUTIONFLOW:19���ڲ���18 �Ļ����ϣ�Ϩ��ʱ��㣬����ʱ���ΪϨ��״̬���Ҳ�Ӱ��ԭ���ַ�����ʾ����
CSUDI_BOOL CSTC_PANEL_IT_PanelDisplay_0014(void)
{
	CSUDIPanelLightValule_S  sValue;
	CSUDI_Error_Code ErrCode;
	
	memset(&sValue, 0, sizeof(sValue));
	
	/***********************��ʾʱ��㿪ʼ****************************/
	sValue.m_eLightName = EM_UDIPANEL_CLOCK;
	if(CSUDI_TRUE != g_abSuppLight[EM_UDIPANEL_CLOCK])
	{
		CSTCPrint("��ƽ̨��֧�ִ������͵�!\t");
		return CSUDI_FALSE;
	}
	CSTCPrint("������ǰ�����%s�Ƶ�״̬�������������\t",g_sLight[sValue.m_eLightName - EM_UDIPANEL_CTRLSTART]);
	CSTKWaitAnyKey();
	sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_ON;
	ErrCode=CSUDIPanelDisplay(0,&sValue);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode,"��һ������ʧ��");
	CSTCPrint("ǰ�����ʱ�����Ƿ�Ϊ����״̬\t");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"�ڶ�������ʧ��");	
	sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_OFF;
	ErrCode=CSUDIPanelDisplay(0,&sValue);
	CSTCPrint("ǰ�����ʱ�����Ƿ�ΪϨ��״̬\t");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����������ʧ��");
	sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_ON;
	ErrCode=CSUDIPanelDisplay(0,&sValue);
	CSTCPrint("ǰ�����ʱ�����Ƿ�Ϊ����״̬\t");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"���Ĳ�����ʧ��");	
	 /***********************��ʾʱ������****************************/
	 
	 /****************��ʾʱ�俪ʼ***********************************/
	sValue.m_eLightName =EM_UDIPANEL_TEXT0;
	sValue.m_uValue.m_sTextValue.m_eDisplayType =  EM_UDIPANEL_TEXT_TIME;
	strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"19991111111111");
	sValue.m_uValue.m_sTextValue.m_nStrCount = 14;
	ErrCode = CSUDIPanelDisplay(0,&sValue);
	if(CSUDI_TRUE != g_abSuppLight[EM_UDIPANEL_TEXT0])
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == ErrCode,"���岽����ʧ��");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode,"����������ʧ��");
		CSTCPrint("ǰ�����%s�ı����Ƿ����1999��11��11��11ʱ11��11�������ʱ������(��ʱ��11:11)\t", g_sLight[EM_UDIPANEL_TEXT0]);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"���߲�����ʧ��");			
	}
	 /****************��ʾʱ�����***********************************/


	 /***********************��ʾʱ��㿪ʼ****************************/
	sValue.m_eLightName = EM_UDIPANEL_CLOCK;
	sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_ON;
	ErrCode=CSUDIPanelDisplay(0,&sValue);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode,"�ڰ˲�����ʧ��");
	CSTCPrint("ǰ�����ʱ�����Ƿ�Ϊ����״̬������������������ޱ仯(��ʾ11:11) \t");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"�ھŲ�����ʧ��");	
	sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_OFF;
	ErrCode=CSUDIPanelDisplay(0,&sValue);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode,"��ʮ������ʧ��");
	CSTCPrint("ǰ�����ʱ�����Ƿ�ΪϨ��״̬������������������ޱ仯(��ʾ11 11) \t");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"��ʮһ������ʧ��");
	sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_ON;
	ErrCode=CSUDIPanelDisplay(0,&sValue);
	CSTCPrint("ǰ�����ʱ�����Ƿ�Ϊ����״̬������������������ޱ仯(��ʾ11:11) \t");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"��ʮ��������ʧ��");	
	/***********************��ʾʱ������****************************/


	   /***********************��ʾ�ַ�����ʼ****************************/
	sValue.m_eLightName = g_aeLight[EM_UDIPANEL_TEXT0];
	sValue.m_uValue.m_sTextValue.m_eDisplayType =  EM_UDIPANEL_TEXT_STRING;
	strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"a6b54");
	sValue.m_uValue.m_sTextValue.m_nStrCount = 5;
	ErrCode = CSUDIPanelDisplay(0,&sValue);
	if(CSUDI_TRUE != g_abSuppLight[EM_UDIPANEL_TEXT0])
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == ErrCode,"��ʮ��������ʧ��");
       }
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode,"��ʮ�Ĳ�����ʧ��");
		if(g_TextCnt < sValue.m_uValue.m_sTextValue.m_nStrCount)
		{
			CSTCPrint("ǰ�����%s�ı����Ƿ���������ַ���a6b54��ǰ%d���ַ�(��ʾa6 b5)\t", g_sLight[EM_UDIPANEL_TEXT0], g_TextCnt);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"��ʮ�岽����ʧ��");			
		}
		else
		{
			CSTCPrint("ǰ�����%s�ı����Ƿ���������ַ���a6b54(��ʾa6 b5)\t", g_sLight[EM_UDIPANEL_TEXT0]);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"��ʮ��������ʧ��");
		}
        }
	/***********************��ʾ�ַ�������****************************/

	/***********************��ʾʱ��㿪ʼ****************************/
	sValue.m_eLightName = EM_UDIPANEL_CLOCK;
	sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_ON;
	ErrCode=CSUDIPanelDisplay(0,&sValue);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode,"��ʮ�߲�����ʧ��");
	CSTCPrint("ǰ�����ʱ�����Ƿ�Ϊ����״̬������������������ޱ仯(��ʾa6:b5)\t");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"��ʮ�˲�����ʧ��");
	 sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_OFF;
	ErrCode=CSUDIPanelDisplay(0,&sValue);
	 CSTCPrint("ǰ�����ʱ�����Ƿ�ΪϨ��״̬����������������ޱ仯(��ʾa6 b5)\t");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"��ʮ�Ų�����ʧ��");	
	 /***********************��ʾʱ������****************************/
	 
	CSTK_FATAL_POINT;
	return CSUDI_TRUE;	

}

//@CASEGROUP:CSUDIPanelClear
//@DESCRIPTION:���Գɹ�����������ʾ�Ƶ���ʾ��Ϣ 
//@PRECONDITION:ǰ����ѳɹ���ʼ��
//@INPUT:1����ʾ�豸������ֵ�Ϸ�
//@INPUT:2�������ʾ����ֵ�Ϸ�
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@REMARK:g_PanelCntΪƽ̨֧�ֵ������
//@EXECUTIONFLOW:1���������ļ�ָ��EM_UDIPANEL_MAIL�ƴ���������MAIL��ʾ�Ƶ���ʾ״̬Ϊ����
//@EXECUTIONFLOW:2������CSUDIPanelClear(0,EM_UDIPANEL_MAIL)��������CSUDI_SUCCESS��ǰ����ϸÿ��ƵƵ���ʾ��Ϣ�����
//@EXECUTIONFLOW:3���������ļ�ָ��EM_UDIPANEL_MAIL�Ʋ����ڣ�����CSUDIPanelClear(0,EM_UDIPANEL_MAIL)��������CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:4����EM_UDIPANEL_CTRLSTART--EM_UDIPANEL_MAX�����еƣ���EM_UDIPANEL_CTRLSTART��EM_UDIPANEL_CTRLALL��EM_UDIPANEL_TEXTSTART��EM_UDIPANEL_TEXTALL��EM_UDIPANEL_SUITSTART��EM_UDIPANEL_SUITALL��EM_UDIPANEL_MAXö��ֵ�⣩�ظ����ϲ���
//@EXECUTIONFLOW:5�������д��ڵ����ID�ظ����ϲ���
CSUDI_BOOL CSTC_PANEL_IT_PanelClear_0001(void)
{
	int i;
	int j;
	CSUDIPanelLightValule_S  sValue;

	for(i = 0; i < g_PanelCnt; ++i)
	{
		for(j = 1; j < EM_UDIPANEL_MAX - EM_UDIPANEL_CTRLSTART; ++j)
		{
			if(CSUDI_PANEL_IS_TEXT(g_aeLight[j]))
			{
				if(CSUDI_TRUE == g_abSuppLight[j])
				{
					sValue.m_eLightName = g_aeLight[j];
					
					sValue.m_uValue.m_sTextValue.m_eDisplayType =  EM_UDIPANEL_TEXT_STRING;
					
					strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"a6b54");
					
					sValue.m_uValue.m_sTextValue.m_nStrCount = 5;

					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i, &sValue), "TEXT��һ������ʧ��");

					CSTCPrint("ǰ�����%s��ʾ���Ƿ�����ַ���a6b54����Ϊ���ַ�����ǰ�����ַ�\n", g_sLight[EM_UDIPANEL_CTRLSTART + j]);

					CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"TEXT��һ������ʧ��");	

					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelClear(i, g_aeLight[j]), "TEXT�ڶ�������ʧ��");

					CSTCPrint("ǰ�����%s��ʾ����ʾ�����Ƿ����\n", g_sLight[EM_UDIPANEL_CTRLSTART + j]);

					CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"TEXT�ڶ�������ʧ��");	
			
				}
				else
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == CSUDIPanelClear(i, g_aeLight[j]),"TEXT����������ʧ��");
				}
			}
		
			if(CSUDI_PANEL_IS_CTRL(g_aeLight[j]))
			{
				if(CSUDI_TRUE == g_abSuppLight[j])
				{
					sValue.m_eLightName = g_aeLight[j];
											
					sValue.m_uValue.m_sCtrlValue.m_eCtrlValue  = EM_UDIPANEL_CTRL_ON;

					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i, &sValue), "CTRL��һ������ʧ��");

					CSTCPrint("ǰ�����%s��ʾ���Ƿ�Ϊ����״̬\n",g_sLight[EM_UDIPANEL_CTRLSTART + j]);

					CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"CTRL��һ������ʧ��");	

					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelClear(i, g_aeLight[j]), "CTRL�ڶ�������ʧ��");

					CSTCPrint("ǰ�����%s��ʾ����ʾ״̬�Ƿ����\n",g_sLight[EM_UDIPANEL_CTRLSTART + j]);

					CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"CTRL�ڶ�������ʧ��");	
			
				}
				else
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == CSUDIPanelClear(i, g_aeLight[j]),"CTRL����������ʧ��");
				}

			}
			
			if(CSUDI_PANEL_IS_SUIT(g_aeLight[j]))
			{
				if(CSUDI_TRUE == g_abSuppLight[EM_UDIPANEL_CTRLSTART + j])
				{
					sValue.m_eLightName = g_aeLight[j];
					
					sValue.m_uValue.m_sSuitValue.m_dwSuitValueMask = (1<<g_SuitCnt) - 1;

					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i, &sValue), "SUIT��һ������ʧ��");

					CSTCPrint("ǰ�����%s��ʾ���Ƿ�Ϊ����״̬\n",g_sLight[EM_UDIPANEL_CTRLSTART + j]);

					CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"SUIT��һ������ʧ��");	

					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelClear(i, g_aeLight[j]), "SUIT�ڶ�������ʧ��");

					CSTCPrint("ǰ�����%s��ʾ����ʾ�����Ƿ����\n",g_sLight[EM_UDIPANEL_CTRLSTART + j]);

					CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"SUIT�ڶ�������ʧ��");	
			
				}
				else
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == CSUDIPanelClear(i, g_aeLight[j]),"SUIT����������ʧ��");
				}
			}		
		}
	}

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelClear
//@DESCRIPTION:���Դ����豸����� 
//@PRECONDITION:ǰ����ѳɹ���ʼ����ǰ��
//@INPUT:1����ʾ�豸������Ϊ-1
//@INPUT:2�������ʾ����ֵ�Ϸ�       
//@EXPECTATION:����CSUDIPANEL_ERROR_INVALID_DEVICE_ID
//@REMARK:
//@EXECUTIONFLOW:1������CSUDIPanelClear������������CSUDIPANEL_ERROR_INVALID_DEVICE_ID
CSUDI_BOOL CSTC_PANEL_IT_PanelClear_0002(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_INVALID_DEVICE_ID == CSUDIPanelClear(-1, EM_UDIPANEL_PLAY),"����ʧ��");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelClear
//@DESCRIPTION:�����豸�ų����豸�������
//@PRECONDITION:ǰ����ѳɹ���ʼ��
//@INPUT:�����ʾ����ֵ�Ϸ�    
//@EXPECTATION:����CSUDIPANEL_ERROR_INVALID_DEVICE_ID
//@REMARK:g_PanelCntΪƽ̨֧�ֵ���������������ļ�����
//@EXECUTIONFLOW:1������CSUDIPanelClear(g_PanelCnt,EM_UDIPANEL_PLAY)�����������õ��ķ���ֵ����ΪCSUDIPANEL_ERROR_INVALID_DEVICE_ID 
//@EXECUTIONFLOW:2������CSUDIPanelClear(g_PanelCnt + 1,EM_UDIPANEL_PLAY)�����������õ��ķ���ֵ����ΪCSUDIPANEL_ERROR_INVALID_DEVICE_ID 
//@EXECUTIONFLOW:3������CSUDIPanelClear(g_PanelCnt + 100,EM_UDIPANEL_PLAY)�����������õ��ķ���ֵ����ΪCSUDIPANEL_ERROR_INVALID_DEVICE_ID 
CSUDI_BOOL CSTC_PANEL_IT_PanelClear_0003(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_INVALID_DEVICE_ID == CSUDIPanelClear(g_PanelCnt, EM_UDIPANEL_TEXT0),"����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_INVALID_DEVICE_ID == CSUDIPanelClear(g_PanelCnt + 1, EM_UDIPANEL_TEXT0),"����ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_INVALID_DEVICE_ID == CSUDIPanelClear(g_PanelCnt + 100, EM_UDIPANEL_TEXT0),"����ʧ��");
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;

}

//@CASEGROUP:CSUDIPanelClear
//@DESCRIPTION:���Բ���eNameֵ�Ƿ������ 
//@PRECONDITION:ǰ����ѳɹ���ʼ��
//@INPUT:��ʾ�豸������ֵ�Ϸ�
//@EXPECTATION:ÿһ�������Ԥ�ڽ��
//@REMARK:g_PanelCntΪƽ̨֧�ֵ������
//@EXECUTIONFLOW:1������CSUDIPanelClear(0,EM_UDIPANEL_CTRLSTART)��������CSUDIPANEL_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:2������CSUDIPanelClear(0,EM_UDIPANEL_TEXTSTART)��������CSUDIPANEL_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:3������CSUDIPanelClear(0,EM_UDIPANEL_SUITSTART)��������CSUDIPANEL_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4�������д��ڵ����ID�ظ����ϲ���
CSUDI_BOOL CSTC_PANEL_IT_PanelClear_0004(void)
{
	
	int i;

	for(i = 0; i < g_PanelCnt; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelClear(i,EM_UDIPANEL_CTRLSTART),"��һ������ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelClear(i,EM_UDIPANEL_TEXTSTART),"�ڶ�������ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelClear(i,EM_UDIPANEL_SUITSTART),"����������ʧ��");
	}

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelClear
//@DESCRIPTION:���������ʾ����ΪEM_UDIPANEL_TEXTALLʱ����� 
//@PRECONDITION:ǰ����ѳɹ���ʼ��
//@INPUT:1����ʾ�豸������ֵ�Ϸ�
//@INPUT:2�������ʾ����ΪEM_UDIPANEL_TEXTALL         
//@EXPECTATION:ÿһ�������Ԥ�ڽ��
//@REMARK:g_PanelCntΪƽ̨֧�ֵ������
//@EXECUTIONFLOW:1���������ļ�ָ��ǰ��岻֧���ı�����ִ�е�4��
//@EXECUTIONFLOW:2���������ļ�ָ��ǰ���֧���ı���������ǰ������ı��Ƶ���ʾ״̬
//@EXECUTIONFLOW:3������CSUDIPanelClear(0,EM_UDIPANEL_TEXTALL)��������CSUDI_SUCCESS����ǰ������ı���ȫ��Ϩ��
//@EXECUTIONFLOW:4������CSUDIPanelClear(0,EM_UDIPANEL_TEXTALL)��������CSUDI_SUCCESS����ǰ������ʾ״̬����
//@EXECUTIONFLOW:5�������д��ڵ����ID�ظ����ϲ���
CSUDI_BOOL CSTC_PANEL_IT_PanelClear_0005(void)
{
	int i;
	int j;
	int temp;
	CSUDIPanelLightValule_S  sValue;

	for(i = 0; i < g_PanelCnt; ++i)
	{
		for(j = 1; j < EM_UDIPANEL_TEXTALL - EM_UDIPANEL_TEXTSTART; ++j)
		{
			temp = 0;
		
			if(CSUDI_TRUE == g_abSuppLight[EM_UDIPANEL_TEXTSTART + j])
			{
				temp = 1;
				
				sValue.m_eLightName = g_aeLight[j + EM_UDIPANEL_TEXTSTART];
				
				sValue.m_uValue.m_sTextValue.m_eDisplayType =  EM_UDIPANEL_TEXT_STRING;
				
				strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"a6b54");
				
				sValue.m_uValue.m_sTextValue.m_nStrCount = 5;

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i, &sValue), "�ڶ�������ʧ��");

			}
			if(temp == 1)
			{
				CSTCPrint("ǰ�����%s��ʾ���Ƿ�����ַ���a6b54��ǰ%d���ַ�\n", g_sLight[EM_UDIPANEL_TEXTSTART + j],g_TextCnt);

				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"�ڶ�������ʧ��");	

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelClear(i, EM_UDIPANEL_TEXTALL), "����������ʧ��");

				CSTCPrint("ǰ�����%s�ı���ʾ�Ƶ���ʾ�����Ƿ����\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);

				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����������ʧ��");	
			}
						
		}
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelClear(i, EM_UDIPANEL_TEXTALL), "���Ĳ�����ʧ��");

		CSTCPrint("ǰ����ϵ���ʾ�����Ƿ��ޱ仯\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"���Ĳ�����ʧ��");	
	}
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelClear
//@DESCRIPTION:���������ʾ����ΪEM_UDIPANEL_CTRLALL��� 
//@PRECONDITION:ǰ����ѳɹ���ʼ��
//@INPUT:1����ʾ�豸������ֵ�Ϸ�
//@INPUT:2�������ʾ����ΪEM_UDIPANEL_CTRLALL         
//@EXPECTATION:ÿһ�������Ԥ�ڽ��
//@REMARK:g_PanelCntΪƽ̨֧�ֵ������
//@EXECUTIONFLOW:1���������ļ�ָ����֧�ֿ��Ƶ���ʾ��ִ�е�4��
//@EXECUTIONFLOW:2���������ļ�ָ��֧�ֿ��Ƶ���ʾ�����ÿ��ƵƵ���ʾ״̬Ϊ����
//@EXECUTIONFLOW:3������CSUDIPanelClear(0,EM_UDIPANEL_CTRLALL)��������CSUDI_SUCCESS��ǰ�����Ƶ�ȫ��Ϩ��
//@EXECUTIONFLOW:4������CSUDIPanelClear(0,EM_UDIPANEL_CTRLALL)��������CSUDI_SUCCESS��ǰ�����ʾ��Ϣ�ޱ仯
//@EXECUTIONFLOW:5�������д��ڵ����ID�ظ����ϲ���
CSUDI_BOOL CSTC_PANEL_IT_PanelClear_0006(void)
{
	int i;

	int j;

	int temp;

	CSUDIPanelLightValule_S  sValue;

	for(i = 0; i < g_PanelCnt; ++i)
	{
		temp = 0;
		
		for(j = 1; j < EM_UDIPANEL_CTRLALL - EM_UDIPANEL_CTRLSTART; ++j)
		{
			if(CSUDI_TRUE == g_abSuppLight[EM_UDIPANEL_CTRLSTART + j])
			{
				temp = 1;
				
				sValue.m_eLightName = g_aeLight[j];
		
				sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_ON;

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i, &sValue), "�ڶ�������ʧ��");
			}	
		}
		
		if(temp == 1)
		{
			CSTCPrint("ǰ����Ͽ��Ƶ��Ƿ�ȫ��������\n");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"�ڶ�������ʧ��");	

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelClear(i, EM_UDIPANEL_CTRLALL), "����������ʧ��");

			CSTCPrint("ǰ��������п��ƵƵ���ʾ�����Ƿ����\n");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����������ʧ��");	
		}

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelClear(i, EM_UDIPANEL_CTRLALL), "���Ĳ�����ʧ��");

		CSTCPrint("ǰ����ϵ���ʾ�����Ƿ��ޱ仯\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"���Ĳ�����ʧ��");	
		
	}
					
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelClear
//@DESCRIPTION:���������ʾ����ΪEM_UDIPANEL_SUITALL��� 
//@PRECONDITION:ǰ����ѳɹ���ʼ��
//@INPUT:1����ʾ�豸������ֵ�Ϸ�
//@INPUT:2�������ʾ����ΪEM_UDIPANEL_SUITALL         
//@EXPECTATION:ÿһ�������Ԥ�ڽ��
//@REMARK:g_PanelCntΪƽ̨֧�ֵ��������g_SuitCntΪ��ϵƵ��ӵƸ���
//@EXECUTIONFLOW:1���������ļ�ָ����֧����ϵ���ʾ��ִ�е�4��
//@EXECUTIONFLOW:2���������ļ�ָ��֧����ϵ���ʾ��������ϵƵ���ʾ״̬Ϊ����
//@EXECUTIONFLOW:3������CSUDIPanelClear(0,EM_UDIPANEL_SUITALL)��������CSUDI_SUCCESS��ǰ��������ʾ��ȫ��Ϩ��
//@EXECUTIONFLOW:4������CSUDIPanelClear(0,EM_UDIPANEL_SUITALL)��������CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:5�������д��ڵ����ID�ظ����ϲ���
CSUDI_BOOL CSTC_PANEL_IT_PanelClear_0007(void)
{
	int i;
	int j;
	int temp;
	CSUDIPanelLightValule_S  sValue;

	for(i = 0; i < g_PanelCnt; ++i)
	{
		temp = 0;
		
		for(j = 1; j < EM_UDIPANEL_SUITALL - EM_UDIPANEL_SUITSTART; ++j)
		{
			if(CSUDI_TRUE == g_abSuppLight[EM_UDIPANEL_SUITSTART + j])
			{
				temp = 1;
				
				sValue.m_eLightName = g_aeLight[j];
		
				sValue.m_uValue.m_sSuitValue.m_dwSuitValueMask = (1<<g_SuitCnt) - 1;

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i, &sValue), "�ڶ�������ʧ��");
			}	
		}
		
		if(temp == 1)
		{
			CSTCPrint("ǰ�������ϵ��Ƿ�ȫ��������");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"�ڶ�������ʧ��");	

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelClear(i, EM_UDIPANEL_SUITALL), "����������ʧ��");

			CSTCPrint("ǰ�����������ϵƵ���ʾ�����Ƿ����\n");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����������ʧ��");	
		}
		else 
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == CSUDIPanelClear(i, EM_UDIPANEL_SUITALL), "���Ĳ�����ʧ��");

		}
		
	}
					
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelClear
//@DESCRIPTION:���������ʾ����ΪEM_UDIPANEL_MAXʱ����� 
//@PRECONDITION:ǰ����ѳɹ���ʼ��
//@INPUT:1����ʾ�豸������ֵ�Ϸ�
//@INPUT:2�������ʾ����ΪEM_UDIPANEL_MAX         
//@EXPECTATION:ÿһ�������Ԥ�ڽ��
//@REMARK:g_PanelCntΪƽ̨֧�ֵ������
//@EXECUTIONFLOW:1�����������ļ����ö����ͬ���͵Ƶ���ʾ״̬Ϊ����
//@EXECUTIONFLOW:2������CSUDIPanelClear(0,EM_UDIPANEL_MAX)��������CSUDI_SUCCESS��ǰ���������ʾ��ȫ��Ϩ��
//@EXECUTIONFLOW:3���ٴε���CSUDIPanelClear(0,EM_UDIPANEL_MAX)��������CSUDI_SUCCESS��ǰ�����ʾ��Ϣ�ޱ仯
//@EXECUTIONFLOW:4�������д��ڵ����ID�ظ����ϲ���
CSUDI_BOOL CSTC_PANEL_IT_PanelClear_0008(void)
{
	int i;
	int j;
	CSUDI_Error_Code ErrCode;
	CSUDIPanelLightValule_S sValue;
	
	for(i = 0; i < g_PanelCnt; ++i)
	{
		sValue.m_eLightName = EM_UDIPANEL_TEXT0;
		
		sValue.m_uValue.m_sTextValue.m_eDisplayType = EM_UDIPANEL_TEXT_STRING;
		
		strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"88888");
		
		sValue.m_uValue.m_sTextValue.m_nStrCount = 5;

		ErrCode = CSUDIPanelDisplay(i,&sValue);

		if(CSUDI_TRUE == g_abSuppLight[EM_UDIPANEL_TEXT0 - EM_UDIPANEL_CTRLSTART])
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode,"��һ������ʧ��");

			if(g_TextCnt < sValue.m_uValue.m_sTextValue.m_nStrCount)
			{
				CSTCPrint("ǰ������Ƿ�����ַ���88888��ǰ%d���ַ�\n",g_TextCnt);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"��һ������ʧ��");			
			}
			else
			{
				CSTCPrint("ǰ������Ƿ�����ַ���88888\n");
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"��һ������ʧ��");
			}
		}
		
		for(j = 1; j < EM_UDIPANEL_CTRLALL - EM_UDIPANEL_CTRLSTART;++j)
		{
			sValue.m_eLightName = g_aeLight[EM_UDIPANEL_CTRLSTART + j];

			sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_ON;

			if(CSUDI_TRUE == g_abSuppLight[EM_UDIPANEL_CTRLSTART + j])
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i,&sValue),"��һ������ʧ��");
			}
		}
	
		CSTCPrint("ǰ��������п��Ƶ��Ƿ���ʾ\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"��һ������ʧ��");	
		
		for(j = 1; j < EM_UDIPANEL_SUITALL - EM_UDIPANEL_SUITSTART;++j)
		{
			sValue.m_eLightName = g_aeLight[EM_UDIPANEL_SUITSTART + j];

			sValue.m_uValue.m_sSuitValue.m_dwSuitValueMask = (1<<g_SuitCnt) - 1;

			if(CSUDI_TRUE == g_abSuppLight[EM_UDIPANEL_SUITSTART + j])
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i, &sValue),"��һ������ʧ��");
			}
		}
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelClear(i,EM_UDIPANEL_MAX),"�ڶ�������ʧ��");

		CSTCPrint("ǰ�����������ʾ�Ƶ���ʾ�����Ƿ����\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"�ڶ�������ʧ��");	

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelClear(i, EM_UDIPANEL_MAX), "����������ʧ��");

		CSTCPrint("ǰ����ϵ���ʾ�����Ƿ��ޱ仯\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����������ʧ��");	
	}
						
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}


