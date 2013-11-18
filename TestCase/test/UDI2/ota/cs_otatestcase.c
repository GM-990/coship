/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
-----------------------------------------------------------------------*/
#include "cs_otatestcase.h"
#include "udi2_ota.h"

//OTA�淶����ö�ٶ���
typedef enum 
{
	OTA_STANDARD_TYPE_UNKNOW,
	OTA_STANDARD_TYPE_NDS,		//NDS OTA
	OTA_STANDARD_TYPE_NOR,		//��ͨOTA
	OTA_STANDARD_TYPE_IRDETO		//Irdeto OTA
}OtaStandardType_E;

static OtaStandardType_E g_eOtaStandard = OTA_STANDARD_TYPE_UNKNOW;

CSUDI_BOOL CSTC_OTA_Init(void)
{
	char acBuf[5];
	int nValue = 0;
	
	memset(acBuf, '\0', sizeof(acBuf));

	CSTKGetConfigInfo("OTA", "OTA_STANDDARD_TYPE", acBuf, sizeof(acBuf));
	if(acBuf[0] != '\0')
	{
		nValue = CSTKGetIntFromStr(acBuf,10);
	}

	CSTCPrint("OTA standard type is %d(1:NDS, 2:Normal, 3:Irdeto, other:Unknow)\n", nValue);
	
	switch (nValue)
	{
		case 1:
		{
			g_eOtaStandard = OTA_STANDARD_TYPE_NDS;
			break;
		}
		case 2:
		{
			g_eOtaStandard = OTA_STANDARD_TYPE_NOR;
			break;
		}
		case 3:
		{
			g_eOtaStandard = OTA_STANDARD_TYPE_IRDETO;
			break;
		}
		default:
		{
			g_eOtaStandard = OTA_STANDARD_TYPE_UNKNOW;
			break;
		}
	}

	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_OTA_UnInit(void)
{
	//�ڱ�����������ִ�к����
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOTAGetInfo
//@DESCRIPTION:���Ե���CSUDIOTAGetInfo��ȡCSUDIOTAInfoType_E���������OTA��Ϣ�����
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT:1��CSUDIOTAInfoType_E���������OTA��Ϣ�Ϸ�ö��ֵ
//@INPUT:2��value,length��Ϊ�Ϸ�ֵ
//@EXPECTATION: ��ȡ����ָ��OTA��Ϣ�����гɹ��ĺͲ��ɹ�������
//@EXECUTIONFLOW:1������CSUDIOTAGetInfo���λ�ȡNDS OTA��Ϣ����¼����ֵ���
//@EXECUTIONFLOW:2������CSUDIOTAGetInfo���λ�ȡ��ͨ OTA��Ϣ����¼����ֵ���
//@EXECUTIONFLOW:3������CSUDIOTAGetInfo���λ�ȡIrdeto OTA��Ϣ����¼����ֵ���
//@EXECUTIONFLOW:4���жϲ���1~3�Ƿ��гɹ��������Ҫ�������
//@EXECUTIONFLOW:5���жϲ���1~3�Ƿ��в��ɹ��������Ҫ�������
CSUDI_BOOL CSTC_OTA_IT_GetInfo_0001(void)
{
	int i = 0;
	int nValue = 0;
	CSUDI_UINT8 uValue = 0;
	CSUDI_BOOL bValue = CSUDI_FALSE;
	CSUDI_Error_Code eRet = CSUDI_FAILURE;
	CSUDI_BOOL bNdsRet = CSUDI_FALSE;
	CSUDI_BOOL bNorRet = CSUDI_FALSE;
	CSUDI_BOOL bIrdetoRet = CSUDI_FALSE;

	for (i=EM_UDIOTA_NDS_BCA; i<=EM_UDIOTA_NDS_STANDBY_INFO; i++)
	{
		if (i == EM_UDIOTA_NDS_STANDBY_INFO)
		{
			bValue = CSUDI_FALSE;
			eRet = CSUDIOTAGetInfo(i, &bValue, sizeof(CSUDI_UINT8));
		}
		uValue = 0;
		eRet = CSUDIOTAGetInfo(i, &uValue, sizeof(CSUDI_UINT8));
		CSTCPrint("OTA Info type is 0x%x, eRet = 0x%x\n", i, eRet);
		
		if (CSUDI_SUCCESS == eRet)
		{
			bNdsRet = CSUDI_TRUE;
		}
	}

	for (i=EM_UDIOTA_NOR_MANUFACTURER_CODE; i<=EM_UDIOTA_NOR_HARDWARE_BATCHNO; i++)
	{
		nValue = 0;
		eRet = CSUDIOTAGetInfo(i, &nValue, sizeof(int));
		CSTCPrint("OTA Info type is 0x%x, eRet = 0x%x\n", i, eRet);
		
		if (CSUDI_SUCCESS == eRet)
		{
			bNorRet = CSUDI_TRUE;
		}
	}

	for (i=EM_UDIOTA_IRDETO_CPCB; i<=EM_UDIOTA_IRDETO_KEYSTATUS; i++)
	{
		uValue = 0;
		eRet = CSUDIOTAGetInfo(i, &uValue, sizeof(CSUDI_UINT8));
		CSTCPrint("OTA Info type is 0x%x, eRet = 0x%x\n", i, eRet);
		
		if (CSUDI_SUCCESS == eRet)
		{
			bIrdetoRet = CSUDI_TRUE;
		}
	}

	CSTK_ASSERT_TRUE_FATAL((bNdsRet || bNorRet || bIrdetoRet), "��ȡ����OTA��Ϣ������ʧ��");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_FALSE == (bNdsRet && bNorRet && bIrdetoRet)), "�ض�OTA�淶�»�ȡ����OTA��ϢӦ���в��ɹ������");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OTA_IT_SetInfo_0001
//@CASEGROUP:CSUDIOTASetInfo
//@DESCRIPTION:���Ե���CSUDIOTASetInfo����OTA��Ϣ�����������ֻ��NDS�淶�Ľ��в���
//@REMARK:Ŀǰֻ���NDS�淶���в��ԣ������淶Ҫ������Ŀ��������ʵ��
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT:1��EM_UDIOTA_NDS_FDCA��EM_UDIOTA_NDS_STANDBY_INFO
//@INPUT:2������������ȡ�Ϸ�ֵ
//@EXPECTATION: ����CSUDIOTASetInfo����FDCA, STANDBY_INFO�ܹ����سɹ�
//@EXECUTIONFLOW:1���ж��Ƿ�ΪNDS�淶���������������ֱ�ӷ���ʧ�ܣ�����������²���
//@EXECUTIONFLOW:2������CSUDIOTASetInfo���úϷ���FDCA��Ϣ���������سɹ�
//@EXECUTIONFLOW:3������CSUDIOTASetInfo���úϷ���STANDBY_INFO��Ϣ���������سɹ�
CSUDI_BOOL CSTC_OTA_IT_SetInfo_0001(void)
{
	if (g_eOtaStandard == OTA_STANDARD_TYPE_NDS)
	{
		CSUDI_UINT8 uFDCABuf[32] = {0};
		CSUDI_BOOL bStandby = CSUDI_FALSE;

		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOTASetInfo(EM_UDIOTA_NDS_FDCA, (void*)uFDCABuf, sizeof(uFDCABuf))),
				"NDS�淶����FDCAʧ��");

		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOTASetInfo(EM_UDIOTA_NDS_STANDBY_INFO, (void*)&bStandby, sizeof(CSUDI_BOOL))),
				"NDS�淶����STANDBY_INFOʧ��");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "��NDS�淶�ݲ�����");
	}

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OTA_IT_SetOta_0001
//@CASEGROUP:CSUDIOTASetOta
//@DESCRIPTION:���Ե���CSUDIOTASetOtaȡ����������������ﲻ���Բ�������Ч�ַ��������
//@REMARK:pUrl�ַ����ĸ�ʽ����Ŀ�����������ͬ��Ҫ����ݾ�����Ŀ����ʵ�֣����ﲻ��NDS�淶���в���
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT:1��CSUDI_NULL
//@EXPECTATION: ����CSUDIOTASetOta(CSUDI_NULL)ȡ�������ܹ����سɹ�
//@EXECUTIONFLOW:1���ж��Ƿ�ΪNDS�淶�������������ֱ�ӷ���ʧ�ܣ�����������²���
//@EXECUTIONFLOW:2������CSUDIOTASetOta(CSUDI_NULL)���������سɹ�
CSUDI_BOOL CSTC_OTA_IT_SetOta_0001(void)
{
	if (g_eOtaStandard != OTA_STANDARD_TYPE_NDS)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOTASetOta(CSUDI_NULL)), 
				"ȡ������ʧ��");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "NDS�淶�ݲ�����");
	}

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

void iOTAWIFICakllback(CSUDIOTAWIFIMsg_E eMsg, void *pvMsgData, void *pvUserData)
{
	CSTCPrint("Recive msg : %d\n", eMsg);
	
	return;
}

//@TESTCASEID:CSTC_OTA_IT_WIFIAddDelCallback_0001
//@CASEGROUP:CSUDIOTAWIFIAddCallback,CSUDIOTAWIFIDelCallback
//@DESCRIPTION:�����ܷ�������Ӻ�ɾ��WIFI�����ص�
//@REMARK:����ֻ�������ɾ���ص��ܷ񷵻سɹ���������������Ϣ����ȷ��
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION: ����CSUDIOTAWIFIAddCallback,CSUDIOTAWIFIDelCallback���ܹ����سɹ�
//@EXECUTIONFLOW:1������CSUDIOTAWIFIAddCallback��ӻص����������سɹ�
//@EXECUTIONFLOW:2������CSUDIOTAWIFIDelCallbackɾ���ص����������سɹ�
//@EXECUTIONFLOW:3���ٴε���CSUDIOTAWIFIDelCallbackɾ����ɾ���Ļص�����������ʧ��
CSUDI_BOOL CSTC_OTA_IT_WIFIAddDelCallback_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOTAWIFIAddCallback(iOTAWIFICakllback, CSUDI_NULL)), 
			"ע��WIFI�����ص�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOTAWIFIDelCallback(iOTAWIFICakllback, CSUDI_NULL)), 
			"ɾ����ע���WIFI�����ص�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIOTAWIFIDelCallback(iOTAWIFICakllback, CSUDI_NULL)), 
			"ɾ������ɾ����WIFI�����ص�Ӧ��ʧ��");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OTA_IT_WIFIAddDelCallback_0002
//@CASEGROUP:CSUDIOTAWIFIAddCallback,CSUDIOTAWIFIDelCallback
//@DESCRIPTION:������Ӻ�ɾ��WIFI�����ص�ʱ����Ƿ����������
//@PRECONDITION:ƽ̨UDI�Ѿ��ɹ���ʼ��
//@INPUT:�Ƿ�����
//@EXPECTATION: ����CSUDIOTAWIFIAddCallback,CSUDIOTAWIFIDelCallback���ܹ�����ʧ��
//@EXECUTIONFLOW:1������CSUDIOTAWIFIAddCallback(CSUDI_NULL, CSUDI_NULL)����������ʧ��
//@EXECUTIONFLOW:2������CSUDIOTAWIFIDelCallback(CSUDI_NULL, CSUDI_NULL)����������ʧ��
CSUDI_BOOL CSTC_OTA_IT_WIFIAddDelCallback_0002(void)
{
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIOTAWIFIAddCallback(CSUDI_NULL, CSUDI_NULL)), 
			"����Ƿ�����ע��WIFI�����ص�Ӧ��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIOTAWIFIDelCallback(CSUDI_NULL, CSUDI_NULL)), 
			"����Ƿ�����ɾ��WIFI�����ص�Ӧ��ʧ��");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

