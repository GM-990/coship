/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
-----------------------------------------------------------------------*/
#include "cs_usbtestcase.h"
#include "udi2_usb.h"

unsigned int g_nUSBCount = 0;

BOOL CSTC_USB_Init(void)
{
	//�ڱ�����������ִ��ǰ����
	CSUDI_BOOL bRet = CSUDI_FALSE;
	char acCfgBuf[32];

	memset(acCfgBuf, '\0', sizeof(acCfgBuf));
	if(CSTKGetConfigInfo("USB", "CS_USBUDI2_USB_COUNT", acCfgBuf, sizeof(acCfgBuf)) == CS_TK_CONFIG_SUCCESS)
	{
		g_nUSBCount = CSTKGetIntFromStr(acCfgBuf, 10);
		bRet = CSUDI_TRUE;
	}
	else
	{
		CSTCPrint("���������ļ����Ƿ��Ѿ�����[USB]�Ρ�CS_USBUDI2_USB_COUNT�Ƿ�����ȷ����\n");
	}
	
	return bRet;
}

BOOL CSTC_USB_UnInit(void)
{
	//�ڱ�����������ִ�к����
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIUSBGetCount   
//@DESCRIPTION:���Ի�ȡ��ǰ�����USB �豸��������ȷ��
//@PRECONDITION:
//@INPUT:�Ϸ�����
//@EXPECTATION:��ȡ���ĵ�ǰ�����USB �豸������ʵ�����һ��
//@EXECUTIONFLOW:1��ͨ�������ļ��е������ж�ƽ̨֧�ֵ�USB�ӿ��������Ƿ�0�����Ϊ0����������ͨ�����������������ԣ�����ִ�в���2
//@EXECUTIONFLOW:2����ʾ�����߲���ָ��������USB�豸�����δ׼�������������
//@EXECUTIONFLOW:3������CSUDIUSBGetCount��ȡ��ǰ�����USB�豸��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������3��ȡ���ĸ����͵�ǰʵ�ʸ�����Ƚϣ�Ӧ�����
//@EXECUTIONFLOW:5���ظ�2~4����ѭ������Ϊ�����ļ������õ�ƽ̨֧�ֵ����USB�ӿڸ���
BOOL CSTC_USB_IT_GetCount_0001(void)
{ 
	unsigned int uUSBCount = 0;
	unsigned int uTestCount = 0;
	unsigned int i = 0;

	CSTK_ASSERT_TRUE_FATAL(g_nUSBCount > 0, "��ƽ̨��֧���κ�USB�豸");

	for (i=0; i<=g_nUSBCount; i++)
	{
		CSTCPrint("��ȷ���������Ѳ�����ֻ�� %d ��USB�豸\n", i);
		CSTCPrint("��׼����:y��δ׼����:n\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "δ׼����ָ������USB�豸");

		uTestCount = i;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIUSBGetCount(&uUSBCount), "��ȡ��ǰ�����USB �豸����ʧ��");
		CSTK_ASSERT_TRUE_FATAL(uUSBCount == uTestCount, "�õ���USB������ʵ����������");
	}

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIUSBGetCount   
//@DESCRIPTION:CSUDIUSBGetCount�ӿڷǷ���������
//@PRECONDITION:
//@INPUT:�Ƿ�����CSUDI_NULL
//@EXPECTATION:����Ƿ�����Ӧ�÷���CSUDIUSB_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDIUSBGetCount(CSUDI_NULL)��Ӧ�÷���CSUDIUSB_ERROR_BAD_PARAMETER
BOOL CSTC_USB_IT_GetCount_0002(void)
{ 
	CSTK_ASSERT_TRUE_FATAL(CSUDIUSB_ERROR_BAD_PARAMETER == CSUDIUSBGetCount(CSUDI_NULL), "����Ƿ�������Ӧ�÷��ز����쳣����");

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIUSBGetInfo   
//@DESCRIPTION:���Ի�ȡָ��������USB�豸��Ϣ����ȷ��
//@PRECONDITION:
//@INPUT:1���Ϸ�USB�豸����
//@INPUT:2��CSUDIUSBInfo_Sָ��&sUsbInfo
//@EXPECTATION:��ȡ����USB�豸��Ϣ��ʵ�����һ��
//@EXECUTIONFLOW:1����ʾ�����߲���������ļ���������ƽ̨֧�ֵ����USB�ӿڸ�����ȵ�USB�豸�����δ׼�������������
//@EXECUTIONFLOW:2������CSUDIUSBGetCount��ȡ�Ѳ���USB�豸�ĸ�������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3���жϲ���2��ȡ���ĸ�����������ʵ�����һ��
//@EXECUTIONFLOW:4������CSUDIUSBGetInfo��ȡ�Ϸ�������USB�豸��Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:5���ж�sUsbInfo.m_bIsUsed == CSUDI_TRUE�������ɹ�
//@EXECUTIONFLOW:6���������жϻ�ȡ����USB�豸������Ϣ�Ƿ���ȷ
//@EXECUTIONFLOW:7���������жϻ�ȡ����USB�豸������Ϣ�Ƿ���ȷ
//@EXECUTIONFLOW:8�����0~count-1��Χ�ڵ����кϷ�����ִ�в���4~7
BOOL CSTC_USB_IT_GetInfo_0001(void)
{
	unsigned int uUSBCount = 0;
	unsigned int i = 0;
	CSUDIUSBInfo_S sUSBInfo;

	CSTCPrint("��ȷ���������Ѳ�����ֻ�� %d ��USB�洢�豸\n", g_nUSBCount);
	CSTCPrint("��׼����:y��δ׼����:n\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "δ׼����ָ������USB�豸");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIUSBGetCount(&uUSBCount), "��ȡ�Ѳ����USB�豸����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(uUSBCount == g_nUSBCount, "�õ���USB������ʵ����������");

	for (i=0; i<uUSBCount; i++)
	{
		CSTCPrint("��ȡindexΪ%d��USB�洢�豸��Ϣ\n", i);
		
		memset(&sUSBInfo, 0, sizeof(sUSBInfo));
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIUSBGetInfo(i, &sUSBInfo), "��ȡָ���Ϸ�������USB��Ϣʧ��");

		CSTK_ASSERT_TRUE_FATAL(sUSBInfo.m_bIsUsed == CSUDI_TRUE, "��ȡ����USB�豸��Ϣ����");

		CSTCPrint("��ȡ����USB�豸������ϢΪ: %s\n", sUSBInfo.m_szUSBType);
		CSTCPrint("������Ϣ��ʵ�����һ��:y����ʵ���������:n\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "��ȡ����USB�豸������Ϣ����");

		CSTCPrint("��ȡ����USB�豸������ϢΪ: \n%s\n", sUSBInfo.m_szUSBDescription);
		CSTCPrint("������Ϣ��ʵ�����һ��:y����ʵ���������:n\n");
		CSTCPrint("(��ע��:������USB�洢�豸�����ļ��������бȽϣ�ͨ����/proc/scsi/usb-storage/Ŀ¼��)\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "��ȡ����USB�豸������Ϣ����");
	}

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIUSBGetInfo   
//@DESCRIPTION:CSUDIUSBGetInfo�ӿڷǷ���������
//@PRECONDITION:
//@INPUT:1��indexΪcount+1
//@INPUT:2��psUsbInfoΪCSUDI_NULL
//@EXPECTATION:����Ƿ������ܹ���ȷ���ض�Ӧ�������
//@EXECUTIONFLOW:1����ʾ�����߲���ָ��������USB�豸�����δ׼�������������
//@EXECUTIONFLOW:2������CSUDIUSBGetInfo��indexȡ����2�����USB�豸������1��psUsbInfoȡ�Ϸ�ֵ����������CSUDIUSB_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW:3���ڻ������Ѳ���һ�������ϸ�����USB�豸�󣬵���CSUDIUSBGetInfo��indexȡ����2�����USB�豸������1��psUsbInfoȡCSUDI_NULL����������CSUDIUSB_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4���ظ�1~3����ѭ������Ϊ�����ļ������õ�ƽ̨֧�ֵ����USB�ӿڸ���
BOOL CSTC_USB_IT_GetInfo_0002(void)
{
	unsigned int i = 0;
	CSUDIUSBInfo_S stUsbInfo;

	memset(&stUsbInfo, 0, sizeof(stUsbInfo));

	for (i=0; i<=g_nUSBCount; i++)
	{
		CSTCPrint("��ȷ���������Ѳ�����ֻ�� %d ��USB�豸\n", i);
		CSTCPrint("��׼����:y��δ׼����:n\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "δ׼����ָ������USB�豸");

		memset(&stUsbInfo, 0, sizeof(stUsbInfo));
		CSTK_ASSERT_TRUE_FATAL(CSUDIUSB_ERROR_INVALID_DEVICE_ID == CSUDIUSBGetInfo(i, &stUsbInfo), "��������ֵcount+1��Ӧ�÷���CSUDIUSB_ERROR_INVALID_DEVICE_ID");

		if (i > 0)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDIUSB_ERROR_BAD_PARAMETER == CSUDIUSBGetInfo(i-1, CSUDI_NULL), "psUsbInfo��CSUDI_NULL��Ӧ�÷���CSUDIUSB_ERROR_BAD_PARAMETER");
		}
	}
	
	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}


