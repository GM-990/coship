/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
-----------------------------------------------------------------------*/
#include <stdio.h>
#include "udi2_fs.h"
#include "udi2_os.h"
#include "udi2_rdi.h"
#include "udi2_flash.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "cs_long.h"
#include "cs_testkit.h"
#include "cs_fs_plus.h"
#include "cs_fstestcase.h"
#include "udiplus_os.h"

#define  CSFSPrint                CSTCPrint          
#define  CSUDIFS_MAX_FILE_PATH    256
#define  M                        (1024*1024)
#define  K                        (1*1024)
#define FS_TEST_MAX_DIR_CNT       32  // �ļ����пɴ������ļ��и���
#define FS_TEST_MAX_FILE_CNT      256 // �ļ����пɴ������ļ�����
#define FS_TEST_MAX_USBDEVICE_CNT   8 // ֧������USB�豸���־����
#define FS_TEST_USB_BRAND_CNT		   12 // ֧�ֵ�USBƷ������


// �Ƿ����ļ�ϵͳ
CSUDI_BOOL g_bHaveFs = CSUDI_TRUE;
// �����ļ�ϵͳ��д���ص�
char g_aucLocalFsReadWritePath[CSUDIFS_MAX_FILE_PATH] = {0};
// �����ļ�ϵͳֻ�����ص�
char g_aucLocalFsReadOnlyPath[CSUDIFS_MAX_FILE_PATH] = {0};
// �����ļ�ϵͳ��д�־��С
int g_nLocalFsReadWriteSizeKByte = 0;
// �����ļ�ϵͳֻ���־��С
int g_nLocalFsReadOnlySizeKByte = 0;
//��ƽ̨֧�ֵ�USB����
static unsigned int g_nUSBCount = 0;

static void FS_iInitCfg(void)
{
	char cBuf[CSUDIFS_MAX_FILE_PATH];
	int nHaveFs = 0;

	memset(cBuf, '\0', sizeof(cBuf));
	if (CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("FS", "CS_FSUDI2_HAVE_LOCAL_FS", cBuf, sizeof(cBuf)))
	{
		nHaveFs = CSTKGetIntFromStr(cBuf, 16);
		if (nHaveFs == 0)
		{
			g_bHaveFs = CSUDI_FALSE;
			CSTCPrint("doesn't have file system\n");
		}
		else
		{
			CSTCPrint("have file system\n");
		}
	}

	memset(cBuf, '\0', sizeof(cBuf));
	if (CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("FS", "CS_FSUDI2_LOCAL_FS_READWRITE_PATH", cBuf, sizeof(cBuf)))
	{
		memcpy(g_aucLocalFsReadWritePath, cBuf, CSUDIFS_MAX_FILE_PATH);
		CSTCPrint("read write path is: %s\n", g_aucLocalFsReadWritePath);
	}
	else
	{
		CSTCPrint("read write path error!\n");
	}

	memset(cBuf, '\0', sizeof(cBuf));
	if (CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("FS", "CS_FSUDI2_LOCAL_FS_READONLY_PATH", cBuf, sizeof(cBuf)))
	{
		memcpy(g_aucLocalFsReadOnlyPath, cBuf, CSUDIFS_MAX_FILE_PATH);
		CSTCPrint("readonly path is: %s\n", g_aucLocalFsReadOnlyPath);
	}
	else
	{
		CSTCPrint("read readonly path error!\n");
	}

	memset(cBuf, '\0', sizeof(cBuf));
	if (CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("FS", "CS_FSUDI2_LOCAL_FS_READWRITE_SIZE_KBYTE", cBuf, sizeof(cBuf)))
	{
		g_nLocalFsReadWriteSizeKByte = CSTKGetIntFromStr(cBuf, 10);
		CSTCPrint("read write size is: %d(K)\n", g_nLocalFsReadWriteSizeKByte);
	}
	else
	{
		CSTCPrint("read write size error!\n");
	}

	memset(cBuf, '\0', sizeof(cBuf));
	if (CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("FS", "CS_FSUDI2_LOCAL_FS_READONLY_SIZE_KBYTE", cBuf, sizeof(cBuf)))
	{
		g_nLocalFsReadOnlySizeKByte = CSTKGetIntFromStr(cBuf, 10);
		CSTCPrint("readonly size is: %d(K)\n", g_nLocalFsReadOnlySizeKByte);
	}
	else
	{
		CSTCPrint("read readonly size error!\n");
	}
}


CSUDI_BOOL CSTC_FS_IT_Init()
{
	CSUDI_BOOL bRet = CSUDI_FALSE;
	char acCfgBuf[32];

	memset(acCfgBuf, '\0', sizeof(acCfgBuf));
	if (CSTKGetConfigInfo("USB", "CS_USBUDI2_USB_COUNT", acCfgBuf, sizeof(acCfgBuf)) == CS_TK_CONFIG_SUCCESS)
	{
		g_nUSBCount = CSTKGetIntFromStr(acCfgBuf, 10);
		bRet = CSUDI_TRUE;
	}
	else
	{
		bRet = CSUDI_FALSE;
		CSTCPrint("���������ļ����Ƿ��Ѿ�����[USB]�Ρ�CS_USBUDI2_USB_COUNT�Ƿ�����ȷ����\n");
	}
	
	CSTCPrint("��ģ���������Ĭ�ϲ���u�̣���δ������ܵ��²��Բ�ͨ��\n");

	return bRet;
}

CSUDI_BOOL CSTC_FS_IT_UnInit()
{
	//�ڱ�����������ִ�к����
	return CSUDI_TRUE;
}

static  char  g_szVolumeMountPoint[CSUDI_MAX_FILE_NAME_LEN];

CSUDI_BOOL CSUDIFSGetVolumeMountPoint()
{
	CSUDI_UINT32 dwIds[32] = {0};
	int  nActIdCnt = 0; 
	int i = 0;
	CSUDI_UINT32 dwVolumeId = 0;
	CSUDIFSDeviceInfo_S sDeviceInfo;
	int nLen = 0;
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt), "��ȡ�豸idʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "һ���豸Ҳ������\n");
	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			dwVolumeId = dwIds[i];
            
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo), "��ȡ�־���Ϣʧ��\n");
			CSTK_ASSERT_TRUE_FATAL(sDeviceInfo.m_szMountPoint != CSUDI_NULL, "�־��豸�޹��ص�\n");
			CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "�־��豸�ص����\n");

			memset(g_szVolumeMountPoint, 0, CSUDI_MAX_FILE_NAME_LEN);
			strcpy(g_szVolumeMountPoint, sDeviceInfo.m_szMountPoint);

			nLen = strlen(g_szVolumeMountPoint);
            
			CSTK_ASSERT_TRUE_FATAL(nLen < CSUDI_MAX_FILE_NAME_LEN, "·������������\n");

			if(g_szVolumeMountPoint[nLen -1] != '/')
			{
				g_szVolumeMountPoint[nLen] = '/';
				g_szVolumeMountPoint[nLen + 1] = '\0';
			}
			return CSUDI_TRUE;
		}
	}	

	CSTK_FATAL_POINT
	{
		g_bAssertBeforeFatalPoint = CSUDI_TRUE;
		return CSUDI_FALSE;
	}
}

// ��ȡ�����ļ�ϵͳ��д�־���ص�
static CSUDI_BOOL FS_iLocalFsGetRWVolumeMountPoint(void)
{
	CSUDI_UINT32 dwIds[32];
	int nActIdCnt = 0; 
	int i = 0;
	int nLen = 0;
	CSUDIFSDeviceInfo_S sDeviceInfo;
	CSUDIFSStatFs_S sFsStat;

	FS_iInitCfg();
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt), "��ȡ�豸idʧ��");

	for (i = 0; i < nActIdCnt; i++)
	{
		if (CSUDI_ISREMOVEABLE(dwIds[i]))
		{
			continue;
		}

		memset(&sDeviceInfo, 0x00, sizeof(CSUDIFSDeviceInfo_S));
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwIds[i], &sDeviceInfo), "��ȡ�־���Ϣʧ��");
		if (sDeviceInfo.m_eDeviceType != EM_UDIFS_DEVTYPE_VOLUME)
		{
			continue;
		}

		CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "�־��豸���ص����");
        memset(&sFsStat,0,sizeof(sFsStat));
		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStatfs(sDeviceInfo.m_szMountPoint, &sFsStat), "��ȡ�־���Ϣ����");

		if ((sFsStat.m_eFsType > EM_UDIFS_PARTITION_RW_START) && (sFsStat.m_eFsType < EM_UDIFS_PARTITION_RW_END))
		{
			CSTK_ASSERT_TRUE_FATAL(!strcmp(sDeviceInfo.m_szMountPoint, g_aucLocalFsReadWritePath), "���ؿ�д�־���ص��������ļ���ͬ");
			strcpy(g_szVolumeMountPoint, sDeviceInfo.m_szMountPoint);
			nLen = strlen(g_szVolumeMountPoint);
			CSTK_ASSERT_TRUE_FATAL(nLen < CSUDI_MAX_FILE_NAME_LEN, "·������������");
			if(g_szVolumeMountPoint[nLen -1] != '/')
			{
				g_szVolumeMountPoint[nLen] = '/';
				g_szVolumeMountPoint[nLen + 1] = '\0';
			}
			return CSUDI_TRUE;
		}
	}

 	CSTK_FATAL_POINT
 	{
		g_bAssertBeforeFatalPoint = CSUDI_TRUE;
 	}

	return CSUDI_FALSE;
}


CSUDI_BOOL CSUDIFSGetDeviceVolumeMountPoint()
{
	CSUDI_UINT32 dwIds[32];
	int  nActIdCnt = 0; 
	int i = 0;
	CSUDI_UINT32 dwVolumeId = 0;
	CSUDIFSDeviceInfo_S sDeviceInfo;
	int nLen = 0;
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt), "��ȡ�豸idʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "һ���豸Ҳ������\n");
	for(i = 0; i < nActIdCnt; i++)
	{
		if(CSUDI_ISREMOVEABLE(dwIds[i]) == 0x00000000)
		{
			dwVolumeId = dwIds[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo), "��ȡ�־���Ϣʧ��\n");
			CSTK_ASSERT_TRUE_FATAL(sDeviceInfo.m_szMountPoint != CSUDI_NULL, "�־��豸�޹��ص�\n");
			CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "�־��豸�ص����\n");
			
			memset(g_szVolumeMountPoint, 0, CSUDI_MAX_FILE_NAME_LEN);
			strcpy(g_szVolumeMountPoint, sDeviceInfo.m_szMountPoint);

			nLen = strlen(g_szVolumeMountPoint);
            
			CSTK_ASSERT_TRUE_FATAL(nLen < CSUDI_MAX_FILE_NAME_LEN, "·������������\n");

			if(g_szVolumeMountPoint[nLen -1] != '/')
			{
				g_szVolumeMountPoint[nLen] = '/';
				g_szVolumeMountPoint[nLen + 1] = '\0';
			}
            
			CSFSPrint("���Է������豸IDΪ%lu\n", sDeviceInfo.m_dwDeviceId);
			CSFSPrint("���Է������豸����Ϊ%s\n", sDeviceInfo.m_szDeviceName);
			CSFSPrint("���Է������豸��СΪ%lu\n\n", sDeviceInfo.m_dwDeviceSize);        
			return CSUDI_TRUE;
		}
	}	

	CSTK_FATAL_POINT
	{
		return CSUDI_FALSE;
	}
}
/****************************************************************************************/

/****************************CSUDIFSRDIAddCallback  Start *******************************/
static CSUDI_BOOL g_bCSUDIFSRDIAddCallback_0001 = CSUDI_FALSE;
static int g_nCSUDIFSRDIAddCallback_0001 = 0;
void CSUDIFSRDIAddCallback_0001_Callback(CSUDIRDIEvent_E eType, CSUDI_UINT32 dwDeviceId,const void * pvEventData, void * pvUserData)
{
	if(eType == EM_UDIRDI_EVENT_READY && pvUserData == &g_nCSUDIFSRDIAddCallback_0001)
	{
		g_bCSUDIFSRDIAddCallback_0001 = CSUDI_TRUE;
	}
	return;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSRDIAddCallback(CSUDIRDICallback_F fnCallback,const void * pvUserData)
//@DESCRIPTION: ����ע��ص��������ص��������û�����
//@PRECONDITION: ��
//@INPUT: fnCallback = CSUDIFSRDIAddCallback_0001_Callback
//@INPUT: pvUserData =  &nUserData, ����int  nUserData = 1;
//@EXPECTATION: ע��ɹ���������U��ʱ�ڻص��������յ�EM_UDIRDI_EVENT_READY��Ϣ
//@EXECUTIONFLOW: 1������CSUDIFSRDIAddCallback(CSUDIFSRDIAddCallback_0001_Callback, &nUserData)ע��ص�������ֵӦ�õ���CSUDI_SUCCESS
//@EXECUTIONFLOW: 2��������ʾ����U�̣������벢���سɹ���'Y'��ȷ�ϣ�����ڻص�����CSUDIFSRDIAddCallback_0001_Callback���Ƿ��յ�EM_UDIRDI_EVENT_READY��Ϣ�������û������Ƿ���ע��ʱһ�£�����ʧ��
//@EXECUTIONFLOW: 3������CSUDIFSRDIRemoveCallback(CSUDIFSRDIAddCallback_0001_Callback, &nUserData)�Ƴ��ص����ָ�����
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIAddCallback_0001(void)
{
	//�ֶ�����
	CSUDI_Error_Code nErrorCode = CSUDI_SUCCESS;
	
	g_bCSUDIFSRDIAddCallback_0001 = CSUDI_FALSE;
	g_nCSUDIFSRDIAddCallback_0001 = 1;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCode = CSUDIFSRDIAddCallback(CSUDIFSRDIAddCallback_0001_Callback,
		&g_nCSUDIFSRDIAddCallback_0001)),"��ӻص�ʧ��");

	CSFSPrint("�����²���u�̣������سɹ����밴Y��ȷ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(g_bCSUDIFSRDIAddCallback_0001, "���̺�ص�������û���յ�EM_UDIRDI_EVENT_READY��Ϣ");

	CSTK_FATAL_POINT
	{
		if (nErrorCode == CSUDI_SUCCESS)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(CSUDIFSRDIAddCallback_0001_Callback, 
				&g_nCSUDIFSRDIAddCallback_0001), "�Ƴ��ص�ʧ��");
		}
	}    
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSRDIAddCallback(CSUDIRDICallback_F fnCallback,const void * pvUserData)
//@DESCRIPTION: ��ӿյĻص�����
//@PRECONDITION: ��
//@INPUT: fnCallback = CSUDI_NULL
//@INPUT:  pvUserData = &nUserData, ����int  nUserData = 1
//@EXPECTATION: ע��ʧ�ܣ�����ֵ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIFSRDIAddCallback(CSUDI_NULL, &nUserData)ע��ص�������ֵӦ�ò�����CSUDI_SUCCESS
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIAddCallback_0002(void)
{
	int  nUserData = 1;
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFSRDIAddCallback(CSUDI_NULL,&nUserData),"��ӿջص�������Ӧ�óɹ�\n");	

	CSTK_FATAL_POINT
	{
		;
	}   
	return CSUDI_TRUE;
}

static CSUDIRDIEvent_E g_eCSUDIFSRDIAddCallback_0003;
void CSUDIFSRDIAddCallback_0003_Callback( CSUDIRDIEvent_E eType, CSUDI_UINT32 dwDeviceId,const void * pvEventData, void * pvUserData)
{
	if(eType == EM_UDIRDI_EVENT_READY) g_eCSUDIFSRDIAddCallback_0003 = eType;
}
//@CASEGROUP: CSUDI_Error_Code CSUDIFSRDIAddCallback(CSUDIRDICallback_F fnCallback,const void * pvUserData)
//@DESCRIPTION: ָ���û�����Ϊ��
//@PRECONDITION: ��
//@INPUT: fnCallback = CSUDIFSRDIAddCallback_0003_Callback
//@INPUT: pvUserData = CSUDI_NULL
//@EXPECTATION: ע��ɹ������ڻص�����CSUDIFSRDIAddCallback_0003_Callback���յ�EM_UDIRDI_EVENT_READY��Ϣ
//@EXECUTIONFLOW: 1������SUDIFSRDIAddCallback(CSUDIFSRDIAddCallback_0003_Callback, CSUDI_NULL)ע��ص���Ӧ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2��������ʾ����U�̣������벢���سɹ���'Y'��ȷ�ϣ�����ڻص�����CSUDIFSRDIAddCallback_0003_Callback���Ƿ��յ�EM_UDIRDI_EVENT_READY��Ϣ������ʧ��
//@EXECUTIONFLOW: 3������CSUDIFSRDIRemoveCallback(CSUDIFSRDIAddCallback_0003_Callback, CSUDI_NULL)�Ƴ��ص����ָ�����
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIAddCallback_0003(void)
{
	//�ֶ�����
	CSUDI_Error_Code nErrorCode = CSUDI_SUCCESS;
	g_eCSUDIFSRDIAddCallback_0003 = EM_UDIRDI_EVENT_UNKNOWN;
    
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCode = CSUDIFSRDIAddCallback(CSUDIFSRDIAddCallback_0003_Callback,
		CSUDI_NULL)),"��ӻص�ʧ��\n");

	CSFSPrint("�����²���u�̣������سɹ����밴Y��ȷ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N������ʧ��");  

	CSTK_ASSERT_TRUE_FATAL(g_eCSUDIFSRDIAddCallback_0003 == EM_UDIRDI_EVENT_READY, "���̺�ص�������û���յ�EM_UDIRDI_EVENT_READY��Ϣ");

	CSTK_FATAL_POINT
	{
		if (nErrorCode == CSUDI_SUCCESS)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(CSUDIFSRDIAddCallback_0003_Callback, 
			  	CSUDI_NULL), "�Ƴ��ص�ʧ��\n");
		}
	}   
	return CSUDI_TRUE;
}

static int g_nCSUDIFSRDIAddCallback_0004;
static int g_nCSUDIFSRDIAddCallback_0004_DeviceId;
void CSUDIFSRDIAddCallback_0004_Callback( CSUDIRDIEvent_E eType, CSUDI_UINT32 dwDeviceId,const void * pvEventData, void * pvUserData)
{
	if(eType == EM_UDIRDI_EVENT_FOUND)
	{
		if(g_nCSUDIFSRDIAddCallback_0004_DeviceId == -1)
		{
			g_nCSUDIFSRDIAddCallback_0004_DeviceId = dwDeviceId;
		}
		if((int)dwDeviceId == g_nCSUDIFSRDIAddCallback_0004_DeviceId)
		{
			g_nCSUDIFSRDIAddCallback_0004++;
		}
	}
}
//@CASEGROUP: CSUDI_Error_Code CSUDIFSRDIAddCallback(CSUDIRDICallback_F fnCallback,const void * pvUserData)
//@DESCRIPTION: ͬһ�ص���������������ע��
//@PRECONDITION: 
//@INPUT: fnCallback = CSUDIFSRDIAddCallback_0004_Callback
//@INPUT: pvUserData = &nUserData, ����int  nUserData = 1
//@EXPECTATION: ��һ��ע��ɹ����ڶ���ע��ʧ�ܣ��ҷ���һ���豸�¼��ص�����ֻ����һ��
//@EXECUTIONFLOW: 1������CSUDIFSRDIAddCallback(CSUDIFSRDIAddCallback_0004_Callback, &nUserData)ע��ص���Ӧ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIFSRDIAddCallback(CSUDIFSRDIAddCallback_0004_Callback, &nUserData)�ٴ�ע��ص�,Ӧ������CSUDIFS_ERROR_ALREADY_ADDED
//@EXECUTIONFLOW: 3��������ʾ����U�̣������سɹ���'Y'��ȷ�ϣ�������ͬһ�豸�¼����ص�����ִ���˼��Σ�Ӧ��ִֻ��һ��
//@EXECUTIONFLOW: 4������CSUDIFSRDIRemoveCallback(CSUDIFSRDIAddCallback_0004_Callback, &nUserData)�Ƴ��ص����ָ�����
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIAddCallback_0004(void)
{
	//�ֶ�����
	int  nUserData = 1;
	CSUDI_Error_Code nErrorCode = CSUDI_SUCCESS;

	g_nCSUDIFSRDIAddCallback_0004 = 0;
	g_nCSUDIFSRDIAddCallback_0004_DeviceId = -1;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCode = CSUDIFSRDIAddCallback(CSUDIFSRDIAddCallback_0004_Callback,
		&nUserData)),"��ӻص�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDIFS_ERROR_ALREADY_ADDED == (nErrorCode = CSUDIFSRDIAddCallback(CSUDIFSRDIAddCallback_0004_Callback,
	&nUserData)),"�ٴ���ӻص�Ӧ������CSUDIFS_ERROR_ALREADY_ADDED\n");

	CSFSPrint("�����²���u�̣������سɹ����밴Y��ȷ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N������ʧ��");  

	CSTK_ASSERT_TRUE_FATAL(g_nCSUDIFSRDIAddCallback_0004 == 1, "���̺�ص�������ͬһ�¼�����û�յ������յ����\n");
			
	CSTK_FATAL_POINT
	{
		if (nErrorCode == CSUDIFS_ERROR_ALREADY_ADDED)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(CSUDIFSRDIAddCallback_0004_Callback, 
			  	&nUserData), "�Ƴ��ص�ʧ��\n");
		}
	}    
	return CSUDI_TRUE;	
}

static  int  g_nCSUDIFSRDIAddCallback_0005_DeviceId;
static  int  g_nCSUDIFSRDIAddCallback_0005_UserDataA;
static  int  g_nCSUDIFSRDIAddCallback_0005_UserDataB;
static  int  g_nCSUDIFSRDIAddCallback_0005;
void CSUDIFSRDIAddCallback_0005_Callback( CSUDIRDIEvent_E eType, CSUDI_UINT32 dwDeviceId,const void * pvEventData, void * pvUserData)
{
	if(eType == EM_UDIRDI_EVENT_FOUND)
	{
		if(g_nCSUDIFSRDIAddCallback_0005_DeviceId == -1)
		{
			g_nCSUDIFSRDIAddCallback_0005_DeviceId = dwDeviceId;
		}
		if((int)dwDeviceId == g_nCSUDIFSRDIAddCallback_0005_DeviceId)
		{
			g_nCSUDIFSRDIAddCallback_0005++;
		
			if(pvUserData == &g_nCSUDIFSRDIAddCallback_0005_UserDataA)
			{
				g_nCSUDIFSRDIAddCallback_0005_UserDataA++;
			}
			else if(pvUserData == &g_nCSUDIFSRDIAddCallback_0005_UserDataB)
			{
				g_nCSUDIFSRDIAddCallback_0005_UserDataB++;
			}
		}
	}
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSRDIAddCallback(CSUDIRDICallback_F fnCallback,const void * pvUserData)
//@DESCRIPTION: ͬһ�ص������ظ�ע�ᣬ���û����ݲ��ظ�
//@PRECONDITION: 
//@INPUT: fnCallback = CSUDIFSRDIAddCallback_0005_Callback
//@INPUT: pvUserData = &g_nCSUDIFSRDIAddCallback_0005_UserDataB������int g_nCSUDIFSRDIAddCallback_0005_UserDataB 
//@EXPECTATION: 
//@EXECUTIONFLOW: 1������CSUDIFSRDIAddCallback(CSUDIFSRDIAddCallback_0005_Callback, &g_nCSUDIFSRDIAddCallback_0005_UserDataA)��һ��ע��ص�������Ӧ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIFSRDIAddCallback(CSUDIFSRDIAddCallback_0005_Callback, &g_nCSUDIFSRDIAddCallback_0005_UserDataB)�ڶ���ע��ص�������Ӧ������CSUDI_SUCCESS	
//@EXECUTIONFLOW: 3������ʾ����U�̣������سɹ��󣬼�����ͬһ�豸ͬһ�¼��ص�������ִ��������ص�����Ӧ�����������Σ����û����ݷֱ�Ϊg_nCSUDIFSRDIAddCallback_0005_UserDataA��g_nCSUDIFSRDIAddCallback_0005_UserDataB
//@EXECUTIONFLOW: 4������CSUDIFSRDIRemoveCallback(CSUDIFSRDIAddCallback_0005_Callback, &g_nCSUDIFSRDIAddCallback_0005_UserDataA)�Ƴ���һ�λص����ָ�����
//@EXECUTIONFLOW: 5������CSUDIFSRDIRemoveCallback(CSUDIFSRDIAddCallback_0005_Callback, &g_nCSUDIFSRDIAddCallback_0005_UserDataB)�Ƴ��ڶ��λص����ָ�����
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIAddCallback_0005(void)
{

	//�ֶ�����
	CSUDI_Error_Code nErrorCodeA = CSUDI_SUCCESS;
	CSUDI_Error_Code nErrorCodeB = CSUDI_SUCCESS;
	g_nCSUDIFSRDIAddCallback_0005_DeviceId = -1;
	g_nCSUDIFSRDIAddCallback_0005 = 0;
	g_nCSUDIFSRDIAddCallback_0005_UserDataA = 0;
	g_nCSUDIFSRDIAddCallback_0005_UserDataB = 0;
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCodeA = CSUDIFSRDIAddCallback(CSUDIFSRDIAddCallback_0005_Callback,
		&g_nCSUDIFSRDIAddCallback_0005_UserDataA)),"��ӻص�ʧ��\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCodeB = CSUDIFSRDIAddCallback(CSUDIFSRDIAddCallback_0005_Callback,
	&g_nCSUDIFSRDIAddCallback_0005_UserDataB)),"�ٴ���ӻص�ʧ��\n");

	CSFSPrint("�����²���u�̣������سɹ����밴Y��ȷ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N������ʧ��");  

	CSTK_ASSERT_TRUE_FATAL(g_nCSUDIFSRDIAddCallback_0005 == 2, "ͬһ�豸ͬһ�¼��յ�����Ϣ�������Σ�������\n");

	CSTK_ASSERT_TRUE_FATAL(g_nCSUDIFSRDIAddCallback_0005_UserDataA == 1, "���ò���һ��\n");

	CSTK_ASSERT_TRUE_FATAL(g_nCSUDIFSRDIAddCallback_0005_UserDataB == 1, "���ò���һ��\n");
	
	CSTK_FATAL_POINT
	{
		if (nErrorCodeA == CSUDI_SUCCESS)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(CSUDIFSRDIAddCallback_0005_Callback, 
			  	&g_nCSUDIFSRDIAddCallback_0005_UserDataA), "�Ƴ��ص�ʧ��\n");
		}
		if (nErrorCodeB == CSUDI_SUCCESS)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(CSUDIFSRDIAddCallback_0005_Callback, 
			  	&g_nCSUDIFSRDIAddCallback_0005_UserDataB), "�Ƴ��ص�ʧ��\n");
		}
	}    
	return CSUDI_TRUE;	
}

static int g_nCSUDIFSRDIAddCallback_0006_A;
static int g_nCSUDIFSRDIAddCallback_0006_B;
void CSUDIFSRDIAddCallback_0006_Callback1( CSUDIRDIEvent_E eType, CSUDI_UINT32 dwDeviceId,const void * pvEventData, void * pvUserData)
{
	g_nCSUDIFSRDIAddCallback_0006_A = CSUDI_TRUE;
}

void CSUDIFSRDIAddCallback_0006_Callback2( CSUDIRDIEvent_E eType, CSUDI_UINT32 dwDeviceId,const void * pvEventData, void * pvUserData)
{
	g_nCSUDIFSRDIAddCallback_0006_B = CSUDI_TRUE;
}
//@CASEGROUP: CSUDI_Error_Code CSUDIFSRDIAddCallback(CSUDIRDICallback_F fnCallback,const void * pvUserData)
//@DESCRIPTION: ͬһ�û������ظ�ע�ᣬ���ص��������ظ�
//@PRECONDITION: 
//@INPUT: fnCallback = CSUDIFSRDIAddCallback_0006_Callback2
//@INPUT: pvUserData = &nUserData������int nUserData 
//@EXPECTATION: ����ע�ᶼ�ɹ����ҵ����豸�¼�����ʱ�����ص��������ܱ�����
//@EXECUTIONFLOW: 1������CSUDIFSRDIAddCallback(CSUDIFSRDIAddCallback_0006_Callback1, &nUserData)ע��ص�����
//@EXECUTIONFLOW: 2������CSUDIFSRDIAddCallback(CSUDIFSRDIAddCallback_0006_Callback2, &nUserData)�ٴ�ע��ص�����
//@EXECUTIONFLOW: 3������U�̣��鿴�����ص��������Ƿ�����յ��ص�
//@EXECUTIONFLOW: 4������CSUDIFSRDIRemoveCallback(CSUDIFSRDIAddCallback_0006_Callback1, &nUserData)�Ƴ��ص����ָ�����
//@EXECUTIONFLOW: 5������CSUDIFSRDIRemoveCallback(CSUDIFSRDIAddCallback_0006_Callback2, &nUserData)�Ƴ��ص����ָ�����
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIAddCallback_0006(void)
{
	//�ֶ�����
	int  nUserData = 1;
	CSUDI_Error_Code nErrorCodeA = CSUDI_SUCCESS;
	CSUDI_Error_Code nErrorCodeB = CSUDI_SUCCESS;

	g_nCSUDIFSRDIAddCallback_0006_A = CSUDI_FALSE;
	g_nCSUDIFSRDIAddCallback_0006_B = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCodeA = CSUDIFSRDIAddCallback(CSUDIFSRDIAddCallback_0006_Callback1,
		&nUserData)),"��ӻص�ʧ��\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCodeB = CSUDIFSRDIAddCallback(CSUDIFSRDIAddCallback_0006_Callback2,
	&nUserData)),"�ٴ���ӻص�ʧ��\n");

	CSFSPrint("�����²���u�̣������سɹ����밴Y��ȷ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N������ʧ��");  

	CSTK_ASSERT_TRUE_FATAL((g_nCSUDIFSRDIAddCallback_0006_A && g_nCSUDIFSRDIAddCallback_0006_B),
		"���������ص��������յ����¼�\n");
		
	CSTK_FATAL_POINT
	{
		if (nErrorCodeA == CSUDI_SUCCESS)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(CSUDIFSRDIAddCallback_0006_Callback1, 
			  	&nUserData), "�Ƴ��ص�ʧ��\n");
		}
		if (nErrorCodeB == CSUDI_SUCCESS)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(CSUDIFSRDIAddCallback_0006_Callback2, 
			  	&nUserData), "�Ƴ��ص�ʧ��\n");
		}
	}
	return CSUDI_TRUE;	
}
/****************************CSUDIFSRDIAddCallback  End *******************************/

/****************************CSUDIFSRDIRemoveCallback  Start **************************/
static int g_nCSUDIFSRDIRemoveCallback_0001;
void CSUDIFSRDIRemoveCallback_0001_Callback( CSUDIRDIEvent_E eType, CSUDI_UINT32 dwDeviceId,const void * pvEventData, void * pvUserData)
{
	if(pvUserData == &g_nCSUDIFSRDIRemoveCallback_0001)
	{
		g_nCSUDIFSRDIRemoveCallback_0001 = 1;
	}
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSRDIRemoveCallback(CSUDIRDICallback_F fnCallback,const void * pvUserData)
//@DESCRIPTION: ɾ���Ѿ���ӵĻص��������û�����
//@PRECONDITION: 
//@INPUT: fnCallback = CSUDIFSRDIRemoveCallback_0001_Callback
//@INPUT: pvUserData = &g_nCSUDIFSRDIRemoveCallback_0001, ����int g_nCSUDIFSRDIRemoveCallback_0001 
//@EXPECTATION: �ܹ��ɹ�ɾ���Ѿ���ӵĻص�����
//@EXECUTIONFLOW: 1������CSUDIFSRDIAddCallback(CSUDIFSRDIRemoveCallback_0001_Callback, &g_nCSUDIFSRDIRemoveCallback_0001)ע��ص���Ӧ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0001_Callback, &g_nCSUDIFSRDIRemoveCallback_0001)�Ƴ��ص���Ӧ������CSUDI_SUCCESS	
//@EXECUTIONFLOW: 3������CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0001_Callback, &g_nCSUDIFSRDIRemoveCallback_0001)�Ƴ��Ѿ��Ƴ��Ļص���Ӧ�����ط�CSUDI_SUCCESS	
//@EXECUTIONFLOW: 4������ʾ����U�̣������سɹ���'Y'��ȷ�ϣ��鿴�ص������Ƿ񱻵��ã�Ӧ�����ᱻ����
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIRemoveCallback_0001(void)
{
	//�ֶ�����
	CSUDI_Error_Code nErrorCode = CSUDI_SUCCESS;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCode = CSUDIFSRDIAddCallback(CSUDIFSRDIRemoveCallback_0001_Callback,
		&g_nCSUDIFSRDIRemoveCallback_0001)),"��ӻص�ʧ��\n");

	g_nCSUDIFSRDIRemoveCallback_0001 = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0001_Callback, 
		&g_nCSUDIFSRDIRemoveCallback_0001), "�Ƴ��ص�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0001_Callback, 
		&g_nCSUDIFSRDIRemoveCallback_0001), "�Ƴ��Ѿ��Ƴ��Ļص���Ӧ�óɹ�\n");

	CSFSPrint("�����²���u�̣������سɹ����밴Y��ȷ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N������ʧ��");  

	CSTK_ASSERT_TRUE_FATAL(g_nCSUDIFSRDIRemoveCallback_0001 == 0, "�Ƴ��ص����յ����¼�\n");

	CSTK_FATAL_POINT
	{
		;
	}    
	return CSUDI_TRUE;
}

static  CSUDIRDIEvent_E  g_eCSUDIFSRDIRemoveCallback_0002;
void CSUDIFSRDIRemoveCallback_0002_Callback( CSUDIRDIEvent_E eType, CSUDI_UINT32 dwDeviceId,const void * pvEventData, void * pvUserData)
{
	if(eType == EM_UDIRDI_EVENT_FOUND)
	{
		g_eCSUDIFSRDIRemoveCallback_0002 = EM_UDIRDI_EVENT_FOUND;
	} 
}
//@CASEGROUP: CSUDI_Error_Code CSUDIFSRDIRemoveCallback(CSUDIRDICallback_F fnCallback,const void * pvUserData)
//@DESCRIPTION: ɾ�������ڵĻص�(ע��ʱ�����û����ݣ�ɾ��ʱ���û�����)
//@PRECONDITION: 
//@INPUT: fnCallback = CSUDIFSRDIRemoveCallback_0002_Callback
//@INPUT: pvUserData = &nUserData������int nUserData
//@EXPECTATION: ɾ��ʧ��
//@EXECUTIONFLOW: 1������CSUDIFSRDIAddCallback(CSUDIFSRDIRemoveCallback_0002_Callback, CSUDI_NULL)ע��ص���Ӧ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0002_Callback, &nUserData)�Ƴ��ص�������ֵӦ��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������ʾ����U�̣������سɹ���'Y'��ȷ�ϣ��鿴�ص������Ƿ񱻵��ã�Ӧ��������
//@EXECUTIONFLOW: 4������CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0002_Callback, CSUDI_NULL)�Ƴ��ص�, ����ֵӦ������CSUDI_SUCCESS
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIRemoveCallback_0002(void)
{
	//�ֶ�����
	int  nUserData = 1;
	CSUDI_Error_Code nErrorCode = CSUDI_SUCCESS;
	g_eCSUDIFSRDIRemoveCallback_0002 = EM_UDIRDI_EVENT_UNKNOWN;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCode = CSUDIFSRDIAddCallback(CSUDIFSRDIRemoveCallback_0002_Callback,
		CSUDI_NULL)),"��ӻص�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0002_Callback, 
		&nUserData), "�Ƴ��ص���Ӧ�óɹ�\n");

	CSFSPrint("�����²���u�̣������سɹ����밴Y��ȷ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N������ʧ��");  

	CSTK_ASSERT_TRUE_FATAL(g_eCSUDIFSRDIRemoveCallback_0002 == EM_UDIRDI_EVENT_FOUND, 
		"�ص�������û���յ��¼�");
	
	CSTK_FATAL_POINT
	{
		if (nErrorCode == CSUDI_SUCCESS)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == 
				CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0002_Callback, CSUDI_NULL), "�Ƴ��ص�ʧ��\n");;
		}
	}

	return CSUDI_TRUE;
}

void CSUDIFSRDIRemoveCallback_0003_Callback( CSUDIRDIEvent_E eType, CSUDI_UINT32 dwDeviceId,const void * pvEventData, void * pvUserData)
{
	return;
}
//@CASEGROUP: CSUDIFSRDIRemoveCallback
//@DESCRIPTION: ɾ�������ڵĻص�(�ص��������û����ݶ�δע��)
//@PRECONDITION: 
//@INPUT: fnCallback = CSUDIFSRDIRemoveCallback_0003_Callback
//@INPUT: pvUserData = &nUserData,����int nUserData
//@EXPECTATION: ɾ��ʧ�ܣ�����ֵ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0003_Callback, &nUserData)ɾ���ص�����Ӧ������CSUDI_SUCCESS
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIRemoveCallback_0003(void)
{
	int  nUserData = 1;
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0003_Callback, 
		&nUserData), "�Ƴ��ص���Ӧ�óɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}    
	return CSUDI_TRUE;
}

void CSUDIFSRDIRemoveCallback_0004_Callback( CSUDIRDIEvent_E eType, CSUDI_UINT32 dwDeviceId,const void * pvEventData, void * pvUserData)
{
	return;
}
//@CASEGROUP: CSUDI_Error_Code CSUDIFSRDIRemoveCallback(CSUDIRDICallback_F fnCallback,const void * pvUserData)
//@DESCRIPTION: ɾ�������ڵĻص�(ע��ʱ���û����ݣ�ɾ��ʱ�����û�����)
//@PRECONDITION: 
//@INPUT: fnCallback = CSUDIFSRDIRemoveCallback_0004_Callback
//@INPUT: pvUserData = CSUDI_NULL
//@EXPECTATION: ɾ��ʧ�ܣ�����ֵ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIFSRDIAddCallback(CSUDIFSRDIRemoveCallback_0004_Callback,&nUserData)ע��ص���Ӧ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0004_Callback, CSUDI_NULL)�Ƴ��ص�������Ӧ�ò�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0004_Callback, &nUserData)�Ƴ��ص�������ֵӦ�õ���CSUDI_SUCCESS
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIRemoveCallback_0004(void)
{
	int  nUserData = 1;
	CSUDI_Error_Code nErrorCode = CSUDI_SUCCESS;
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCode = CSUDIFSRDIAddCallback(CSUDIFSRDIRemoveCallback_0004_Callback,
		&nUserData)),"��ӻص�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0004_Callback, 
		CSUDI_NULL), "�Ƴ��ص���Ӧ�óɹ�\n");

	CSTK_FATAL_POINT
	{
		if(nErrorCode == CSUDI_SUCCESS)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0004_Callback, 
				&nUserData), "�Ƴ��ص�ʧ��\n");
		}
	}    
	return CSUDI_TRUE;	
}

static CSUDIRDIEvent_E CSUDIFSRDIRemoveCallback_0005;
void CSUDIFSRDIRemoveCallback_0005_Callback( CSUDIRDIEvent_E eType, CSUDI_UINT32 dwDeviceId,const void * pvEventData, void * pvUserData)
{
	CSUDIFSRDIRemoveCallback_0005 = eType;
}
//@CASEGROUP: CSUDI_Error_Code CSUDIFSRDIRemoveCallback(CSUDIRDICallback_F fnCallback,const void * pvUserData)
//@DESCRIPTION: ɾ���Ѿ����ڵĻص���ע��ʱ�����û����ݣ�ɾ��ʱҲ�����û����ݣ�
//@PRECONDITION: 
//@INPUT: fnCallback = CSUDIFSRDIRemoveCallback_0005_Callback
//@INPUT: pvUserData = CSUDI_NULL
//@EXPECTATION: ɾ���ɹ�������ֵ����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIFSRDIAddCallback(fnCallback, CSUDI_NULL)��Ӧ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIFSRDIRemoveCallback(fnCallback, CSUDI_NULL),Ӧ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������ʾ����U�̣������سɹ���'Y'��ȷ�ϣ��鿴�ص������Ƿ񱻵��ã�Ԥ�ڲ�Ӧ��������
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIRemoveCallback_0005(void)
{
	//�ֶ�����
	CSUDI_Error_Code nErrorCode = CSUDI_SUCCESS;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCode = CSUDIFSRDIAddCallback(CSUDIFSRDIRemoveCallback_0005_Callback,
		CSUDI_NULL)),"��ӻص�ʧ��\n");

	CSUDIFSRDIRemoveCallback_0005 = EM_UDIRDI_EVENT_UNKNOWN;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0005_Callback, 
		CSUDI_NULL), "�Ƴ��ص�ʧ��\n");

	CSFSPrint("�����²���u�̣������سɹ����밴Y��ȷ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N������ʧ��");  

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSRDIRemoveCallback_0005 == EM_UDIRDI_EVENT_UNKNOWN, "ɾ���ص���ص������в�Ӧ���յ��¼�");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

void CSUDIFSRDIRemoveCallback_0006_Callback( CSUDIRDIEvent_E eType, CSUDI_UINT32 dwDeviceId,const void * pvEventData, void * pvUserData)
{
	return;
}
//@CASEGROUP: CSUDI_Error_Code CSUDIFSRDIRemoveCallback(CSUDIRDICallback_F fnCallback,const void * pvUserData)
//@DESCRIPTION: ɾ�������ڵĻص�(�ص��������û����ݶ�δע��)
//@PRECONDITION: δ��ӻص�����
//@INPUT: fnCallback����
//@INPUT: pvUserData = &nUserData, int nUserData
//@EXPECTATION: ɾ��ʧ�ܣ�����ֵ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0006_Callback, &nUserData)ɾ���ص���Ӧ��ʧ���ҷ���ֵ������CSUDI_SUCCESS	
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIRemoveCallback_0006(void)
{
	int  nUserData = 1;
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0006_Callback, 
		&nUserData), "�Ƴ��ص���Ӧ�óɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSRDIRemoveCallback(CSUDIRDICallback_F fnCallback,const void * pvUserData)
//@DESCRIPTION: ɾ�������ڵĻص���ûָ��Ҫɾ���Ļص�������
//@PRECONDITION: 
//@INPUT: fnCallback = CSUDI_NULL
//@INPUT: pvUserData = &nUserData, ����int nUserData
//@EXPECTATION: ɾ��ʧ�ܣ�����ֵ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIFSRDIRemoveCallback(CSUDI_NULL, &nUserData)����ֵӦ��������CSUDI_SUCCESS	
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIRemoveCallback_0007(void)
{
	int  nUserData = 1;
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFSRDIRemoveCallback(CSUDI_NULL, &nUserData), "�Ƴ��ص���Ӧ�óɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}
/****************************CSUDIFSRDIRemoveCallback  End *******************************/

/****************************CSUDIFSRDIRemoveDev  Start **********************************/
static CSUDIRDIEvent_E g_eCSUDIFSRDIRemoveDev_0001;
static  CSUDI_UINT32  g_nCSUDIFSRDIRemoveDev_0001;
void CSUDIFSRDIRemoveDev_0001_Callback(CSUDIRDIEvent_E eType, CSUDI_UINT32 dwDeviceId,const void * pvEventData, void * pvUserData)
{
	//CSUDIRDIPlugOutType_E  *eEventData = (CSUDIRDIPlugOutType_E  *)pvEventData;
	
	//if(dwDeviceId  == g_nCSUDIFSRDIRemoveDev_0001 && eType == EM_UDIRDI_EVENT_PLUGOUT && (*eEventData == EM_UDIRDI_PLUGOUT_SAFE))
	
	if(dwDeviceId  == g_nCSUDIFSRDIRemoveDev_0001 && eType == EM_UDIRDI_EVENT_PLUGOUT)

	{
		g_eCSUDIFSRDIRemoveDev_0001 = eType;
	}
	return;
}
//@CASEGROUP: CSUDI_Error_Code CSUDIFSRDIRemoveDev(CSUDI_UINT32 dwDeviceId)
//@DESCRIPTION: �Ƴ��ƶ��豸
//@PRECONDITION: 1�����ƶ��豸���ϣ�����ʶ��
//@INPUT: dwDeviceId = dwDeviceId1��dwDeviceId1��ͨ������CSUDIFSGetAllDeviceId()�õ����ƶ��豸
//@EXPECTATION: �Ƴ��ɹ����һص��������յ��¼�EM_UDIRDI_EVENT_PLUGOUT���Ƴ������ٻ�ø��豸����Ϣ
//@EXECUTIONFLOW: 1������CSUDIFSRDIAddCallback()���һ���ص�������Ӧ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIFSGetAllDeviceId()�õ�һ�������豸id,���������豸�Ŵ����ҵ�һ���ƶ��豸dwDeviceId1�����û���ƶ������ʧ��
//@EXECUTIONFLOW: 3������CSUDIFSRDIRemoveDev(dwDeviceId1)�Ƴ��豸��Ӧ������CSUDI_SUCCESS	
//@EXECUTIONFLOW: 4�����ص������Ƿ��յ�EM_UDIRDI_PLUGOUT_SAFE���͵��¼�EM_UDIRDI_EVENT_PLUGOUT
//@EXECUTIONFLOW: 5������CSUDIFSRDIRemoveCallback()ɾ���ص����ָ�����
//@EXECUTIONFLOW: 6������CSUDIFSGetDeviceInfo(dwDeviceId1, &sDeviceInfo)��ȡ�豸��Ϣ��Ӧ��ʧ���ҷ���ֵ������CSUDI_SUCCESS
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIRemoveDev_0001(void)
{
	//�ֶ�����
	CSUDI_Error_Code nErrorCode = CSUDI_SUCCESS;
	CSUDI_UINT32  dwIds[32];
	int nActIdCnt = 0;
	int i = 0;
	CSUDIFSDeviceInfo_S  sDeviceInfo;
	CSUDI_BOOL bRemoved = CSUDI_FALSE;

	g_eCSUDIFSRDIRemoveDev_0001 = EM_UDIRDI_EVENT_UNKNOWN;
	g_nCSUDIFSRDIRemoveDev_0001 = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCode = CSUDIFSRDIAddCallback(CSUDIFSRDIRemoveDev_0001_Callback,
		CSUDI_NULL)),"��ӻص�ʧ��\n");

	CSFSPrint("�����²���u�̣������سɹ����밴Y��ȷ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N������ʧ��"); 

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt), "��ȡ�豸idʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "һ���豸Ҳδ���\n");
	for(i = 0; i < nActIdCnt; i++)
	{
		if(CSUDI_ISREMOVEABLE(dwIds[i]) && (dwIds[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_STORAGE_MAJOR_DEVICEID)
		{
			g_nCSUDIFSRDIRemoveDev_0001  = dwIds[i];

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveDev(dwIds[i]), "�Ƴ��ƶ��豸ʧ��\n");
			bRemoved = CSUDI_TRUE;
			break;
		}	
	}

	CSTK_ASSERT_TRUE_FATAL(g_eCSUDIFSRDIRemoveDev_0001 == EM_UDIRDI_EVENT_PLUGOUT, "ɾ���ƶ��豸��ص�������û���յ�EM_UDIRDI_EVENT_PLUGOUT��Ϣ");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFSGetDeviceInfo(g_nCSUDIFSRDIRemoveDev_0001, &sDeviceInfo), "��ȡ�豸��Ϣ��Ӧ�óɹ�\n");

	CSTK_FATAL_POINT
	{
		if (nErrorCode == CSUDI_SUCCESS)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveDev_0001_Callback, 
				NULL), "�Ƴ��ص�ʧ��\n");
		}

		if(bRemoved == CSUDI_TRUE)
		{
			CSFSPrint("U���Ѿ��Ƴ�����ε������²��룬�����سɹ��󣬰�Yȷ��!\n");
			CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N���ָ�����ʧ�ܣ���Ӱ�������������ִ��");  
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSRDIRemoveDev(CSUDI_UINT32 dwDeviceId)
//@DESCRIPTION: �Ƴ����ƶ������豸
//@PRECONDITION: ϵͳ�����Ѿ�ʶ��ķ��ƶ��豸
//@INPUT: dwDeviceId = dwDeviceId1��dwDeviceId1��ͨ������CSUDIFSGetAllDeviceId()�õ��ķ��ƶ��豸
//@EXPECTATION: �Ƴ�ʧ�ܣ�����ֵ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ�����ƶ��豸dwDeviceId1
//@EXECUTIONFLOW: 2������CSUDIFSRDIRemoveDev(dwDeviceId1)�Ƴ��豸��Ӧ������CSUDIFS_ERROR_FEATURE_NOT_SUPPORTED	
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIRemoveDev_0002(void)
{
	CSUDI_UINT32  dwIds[32];
	int nActIdCnt = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt), "��ȡ�豸idʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "һ���豸Ҳδ���\n");

	while(nActIdCnt-- > 0)
	{
		if(!CSUDI_ISREMOVEABLE(dwIds[nActIdCnt]))
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDIFS_ERROR_FEATURE_NOT_SUPPORTED == CSUDIFSRDIRemoveDev(dwIds[nActIdCnt]), "���ƶ��豸��֧���Ƴ�\n");
			break;
		}
	}

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt >= 0, "�Ҳ������ƶ��豸");
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSRDIRemoveDev(CSUDI_UINT32 dwDeviceId)
//@DESCRIPTION: �Ƴ������ڵ��豸
//@PRECONDITION: 
//@INPUT: dwDeviceId = 0x0001FFFF(����0x0001FFFFΪ�Ƿ����豸id)
//@EXPECTATION: �Ƴ�ʧ�ܣ�����ֵ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIFSRDIRemoveDev(0x0001FFFF)�Ƴ��豸������ֵӦ��������CSUDI_SUCCESS	
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIRemoveDev_0003(void)
{
	CSTK_ASSERT_TRUE(CSUDI_SUCCESS != CSUDIFSRDIRemoveDev(0x0001FFFF), "�Ƴ������ڵ��豸��Ӧ�óɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSRDIRemoveDev(CSUDI_UINT32 dwDeviceId)
//@DESCRIPTION: �Ƴ����ڱ�ʹ�õ��豸
//@PRECONDITION: ���ƶ��豸���ϣ�����ʶ��
//@INPUT: dwDeviceId = dwDeviceId1��dwDeviceId1��ͨ������CSUDIFSGetAllDeviceId()�õ����ƶ��豸
//@EXPECTATION: �Ƴ�ʧ�ܣ�����ֵ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ƶ��豸dwDeviceId1
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()�õ����豸�Ĺ��ص�
//@EXECUTIONFLOW: 3������CSUDIFSOpen("�������ص�"+"CSUDIFSRDIRemoveDev_0004")��������һ���ļ����õ�����hFile�����Ӧ��������CSUDI_NULL
//@EXECUTIONFLOW: 4������CSUDIFSRDIRemoveDev(dwDeviceId1)�Ƴ��豸��Ӧ��ʧ���ҷ���ֵ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 5������CSUDIFSClose(hFile)�ر��ļ�������ֵӦ������0
//@EXECUTIONFLOW: 6������CSUDIFSRemove("�������ص�"+"CSUDIFSRDIRemoveDev_0004")ɾ���ļ�������ֵӦ��Ϊ0
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIRemoveDev_0004(void)
{
	//�ֶ�����
	CSUDI_UINT32  dwIds[32];
	int nActIdCnt = 0;
	int i = 0;
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDIFSDeviceInfo_S     sDeviceInfo;
	int   nLen; 
	char  szTest[] = "CSUDIFSRDIRemoveDev_0004";
	CSUDI_HANDLE    hFile = CSUDI_NULL;
	CSUDI_BOOL bRemoved = CSUDI_FALSE;

	memset(szFilePath, 0, sizeof(szFilePath));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt), "��ȡ�豸idʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "һ���豸Ҳδ���\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwIds[i], &sDeviceInfo), "��ȡ�־���Ϣʧ��\n");

			strcpy(szFilePath, sDeviceInfo.m_szMountPoint);
			nLen = strlen(szFilePath);
			CSTK_ASSERT_TRUE_FATAL(nLen < CSUDI_MAX_FILE_NAME_LEN, "·������������\n");

			if(szFilePath[nLen -1] != '/')
			{
				szFilePath[nLen] = '/';
				szFilePath[nLen + 1] = '\0';
			}
			strcat(szFilePath, szTest);

			CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w")), "���������ļ�ʧ��\n");

			bRemoved = CSUDI_TRUE;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFSRDIRemoveDev(sDeviceInfo.m_dwParentDeviceId), "�豸�����ļ�����Ӧ�����ܱ��Ƴ�\n");
			bRemoved = CSUDI_FALSE;

			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");
			
			break;
		}	
	}

	CSTK_FATAL_POINT
	{
		if (hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRemove(szFilePath), "ɾ���ļ�ʧ��\n");
		}

		/* ��ֹCSUDIFSRDIRemoveDev�ӿڵ��óɹ���û�лָ�����(Ĭ�ϲ���u��) */
		if (bRemoved == CSUDI_TRUE)
		{
			CSTCPrint("��ε�U�̺����²��룬�����سɹ��󣬰�Yȷ��!\n");
			CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N���ָ�����ʧ�ܣ���Ӱ�������������ִ��");  
		}
	}

	return CSUDI_TRUE;
}
static CSUDIRDIEvent_E g_eRDIRemoveDevEvent_0005;
static CSUDI_UINT32  g_nRDIRemoveDevId_0005;

static void CSUDIFSRDIRemoveDev_0005_Callback(CSUDIRDIEvent_E eType, CSUDI_UINT32 dwDeviceId,const void * pvEventData, void * pvUserData)
{
	if(dwDeviceId  == g_nRDIRemoveDevId_0005 && eType == EM_UDIRDI_EVENT_PLUGOUT)
	{
		g_eRDIRemoveDevEvent_0005 = eType;
	}

}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSRDIRemoveDev(CSUDI_UINT32 dwDeviceId)
//@DESCRIPTION: �Ƴ�һ�������������־���ƶ��洢�豸������һ���־�
//@PRECONDITION: 1���ƶ��洢�豸���ϣ����ѱ�ʶ��
//@INPUT: ͨ������CSUDIFSGetAllDeviceId()�õ��ķ־��豸id
//@EXPECTATION: �ƶ��洢�豸�Ƴ��ɹ�������ж�ش洢�豸ʱ���ص��������յ��¼�EM_UDIRDI_EVENT_PLUGOUT
//@EXECUTIONFLOW: 1������CSUDIFSRDIAddCallback()���һ���ص�������Ӧ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIFSGetAllDeviceId()�õ������豸id,���������豸�Ŵ����ҵ��־��豸id�����û���ҵ��־��豸�����ʧ��
//@EXECUTIONFLOW: 3������CSUDIFSGetDeviceInfo()��ȡ�־��豸��Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 4������CSUDIFSRDIRemoveDev(dwDeviceId1)�Ƴ��洢�豸��Ӧ������CSUDI_SUCCESS	
//@EXECUTIONFLOW: 5�����ص������Ƿ��յ�EM_UDIRDI_PLUGOUT_SAFE���͵��¼�EM_UDIRDI_EVENT_PLUGOUT
//@EXECUTIONFLOW: 6������CSUDIFSRDIRemoveCallback()ɾ���ص����ָ�����
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIRemoveDev_0005(void)
{
	//�ֶ�����
	CSUDI_Error_Code nErrorCode = CSUDI_SUCCESS;
	CSUDI_UINT32  dwIds[32];
	int nActIdCnt = 0;
	int i = 0;
	CSUDI_BOOL bRemoved = CSUDI_FALSE;

	CSUDIFSDeviceInfo_S sDeviceInfo;

	g_nRDIRemoveDevId_0005 = 0;
	g_eRDIRemoveDevEvent_0005 = EM_UDIRDI_EVENT_UNKNOWN;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCode = CSUDIFSRDIAddCallback(CSUDIFSRDIRemoveDev_0005_Callback,
		CSUDI_NULL)),"��ӻص�ʧ��\n");

	CSFSPrint("�����һ�������������־���ƶ��洢�豸�����벢���سɹ����밴Yȷ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N������ʧ��"); 

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt), "��ȡ�豸idʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "һ���豸Ҳδ���\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwIds[i], &sDeviceInfo), "��ȡ�־��豸��Ϣʧ��\n");

			g_nRDIRemoveDevId_0005 = dwIds[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveDev(dwIds[i]), "�Ƴ��ƶ��־��豸ʧ��\n");
			bRemoved = CSUDI_TRUE;

			break;
		}
	}

	CSTK_ASSERT_TRUE_FATAL(g_eRDIRemoveDevEvent_0005 == EM_UDIRDI_EVENT_PLUGOUT, "ɾ���ƶ��豸��ص�������û���յ�EM_UDIRDI_EVENT_PLUGOUT��Ϣ");
	
	CSTK_FATAL_POINT
	{
		if (nErrorCode == CSUDI_SUCCESS)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveDev_0005_Callback, 
				NULL), "�Ƴ��ص�ʧ��\n");
		}
		if(bRemoved == CSUDI_TRUE)
		{
			CSFSPrint("��ε����豸����U�̣������سɹ����밴Yȷ��\n");
			CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N���ָ�����ʧ�ܣ���Ӱ�������������ִ��");  
		}
	}
	
	return CSUDI_TRUE;
}


/****************************CSUDIFSRDIRemoveDev  End *******************************/

/****************************CSUDIFSGetAllDeviceId  Start **********************************/
static int g_nCSUDIFSGetAllDeviceId_0001;
static CSUDI_BOOL g_bThreadErr = CSUDI_FALSE;
static CSUDI_BOOL g_bThread1Return = CSUDI_FALSE;
static CSUDI_BOOL g_bThread2Return = CSUDI_FALSE;
static CSUDI_HANDLE g_hSemaphore = CSUDI_NULL;
//Lock test function
void FS_LockTestThreadEntry1(void * pvParam)
{
	CSUDI_UINT32 pdwIds[32];
	int nActIdCnt = -1; 
	int nParam = 32;
	int i=0;

	for(i=0; i<100; i++)
	{
		if(g_bThreadErr)
		{
			break;
		}
		CSUDIOSSemWait(g_hSemaphore, CSUDIOS_TIMEOUT_INFINITY);

		if(CSUDI_SUCCESS != CSUDIFSGetAllDeviceId(pdwIds,nParam, &nActIdCnt))
		{
			CSTCPrint("ThreadEntry1��ȡ�豸idʧ��\n");
			g_bThreadErr = CSUDI_TRUE;
			break;
		}

		CSUDIOSSemRelease(g_hSemaphore);

		CSUDIOSThreadSleep(80);
	}
	g_bThread1Return = CSUDI_TRUE;

	return;
}

void FS_LockTestThreadEntry2(void * pvParam)
{
	CSUDI_UINT32 pdwIds[32];
	int nActIdCnt = -1; 
	int nParam = 32;
	int i=0;

	for(i=0; i<100; i++)
	{
		if(g_bThreadErr)
		{
			break;
		}
		CSUDIOSSemWait(g_hSemaphore, CSUDIOS_TIMEOUT_INFINITY);
		if(CSUDI_SUCCESS != CSUDIFSGetAllDeviceId(pdwIds,nParam, &nActIdCnt))
		{
			CSTCPrint("ThreadEntry2��ȡ�豸idʧ��\n");
			g_bThreadErr = CSUDI_TRUE;
			break;
		}
		CSUDIOSSemRelease(g_hSemaphore);
		CSUDIOSThreadSleep(70);
	}
	g_bThread2Return = CSUDI_TRUE;

	return;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetAllDeviceId(CSUDI_UINT32 * pdwIds,int nMaxIdCnt, int * pnActIdCnt)
//@DESCRIPTION: ͬʱ������е��豸id���豸��
//@PRECONDITION: ϵͳ���޿��ƶ��洢�豸
//@INPUT: pdwIds = pdwIds1������CSUDI_UINT32 pdwIds1[32]
//@INPUT: nMaxIdCnt = 32
//@INPUT: pnActIdCnt = &nActIdCnt������int nActIdCnt
//@EXPECTATION: ����ֵ����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1���ε�U�̣���û�в�U�����ðγ����γ���Yȷ��
//@EXECUTIONFLOW: 2������CSUDIFSGetAllDeviceId(pdwIds1,32,&nActIdCnt)��ȡ�豸�������������ڱ���g_nCSUDIFSGetAllDeviceId_0001��	
//@EXECUTIONFLOW: 3�������U�̣������سɹ�����Yȷ��
//@EXECUTIONFLOW: 4������CSUDIFSGetAllDeviceId(pdwIds1,32,&nActIdCnt)�ٴλ�ȡ�豸�������жϵڶ��λ�ȡ���Ƿ���ڵ�һ�Σ�����ɹ�������ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetAllDeviceId_0001(void)
{
	//�ֶ�����
	CSUDI_UINT32  pdwIds1[32];
	int nActIdCnt = -1; 

	CSFSPrint("��ε�U�̣���Yȷ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N������ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(pdwIds1, 32, &nActIdCnt), "��ȡ�豸idʧ��\n");

	g_nCSUDIFSGetAllDeviceId_0001 = nActIdCnt;

	CSFSPrint("�����U�̣������سɹ���Yȷ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(pdwIds1, 32, &nActIdCnt), "��ȡ�豸idʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > g_nCSUDIFSGetAllDeviceId_0001, "��ȡ�豸��������\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetAllDeviceId(CSUDI_UINT32 * pdwIds,int nMaxIdCnt, int * pnActIdCnt)
//@DESCRIPTION: ��ȡһ���洢�豸�ķ�����������id���������ص�
//@PRECONDITION: ϵͳ���޿��ƶ��洢�豸
//@INPUT: pdwIds = pdwIds1������CSUDI_UINT32 pdwIds1[32]
//@INPUT: nMaxIdCnt = 32
//@INPUT: pnActIdCnt = &nActIdCnt������int nActIdCnt
//@EXPECTATION: �õ��ķ������͹��ص���ʵ��һ��(��Ҫ�˹��ж�)
//@EXECUTIONFLOW: 1�������ѷ�����U�̣������سɹ��󣬰�Y��ȷ��
//@EXECUTIONFLOW: 2������CSUDIFSGetAllDeviceId(pdwIds1,32,&nActIdCnt)�õ����е��豸
//@EXECUTIONFLOW: 3���������豸�Ŵ�ӡ�����еķ־��豸id�����ص㡢���豸id�����һ���־��豸Ҳδ���ֻ������еķ־��豸�ĸ��豸id����ͬ�����ʧ��
//@EXECUTIONFLOW: 4��������ʾ�˹��жϴ�ӡ��Ϣ��ʵ���Ƿ�������������ɰ�'Y'��������'N'��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetAllDeviceId_0002(void)
{
	//�ֶ�����
	CSUDI_UINT32  dwIds[32];
	int nActIdCnt = 0;
	int i = 0;
	int nStorageId = -1;
	CSUDIFSDeviceInfo_S    sDeviceInfo;
	int   nCount = 0;
 	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt), "��ȡ�豸idʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "һ���豸Ҳδ���\n");
	
	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwIds[i], &sDeviceInfo), "��ȡ�豸��Ϣʧ��\n");
			if(nStorageId == -1)
			{
				nStorageId = sDeviceInfo.m_dwParentDeviceId;
			}

			if((int)sDeviceInfo.m_dwParentDeviceId == nStorageId)
			{
				CSFSPrint("����id = %d, ����·�� = %s, ���洢�豸id = %d\n", dwIds[i], sDeviceInfo.m_szMountPoint, nStorageId);
				nCount++;
			}
			else
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "���ֶ���ƶ��洢�豸��Ӧ��ֻ��һ��\n");
			}
		}
	}

	CSTK_ASSERT_TRUE_FATAL(nCount != 0, "һ���־��豸Ҳû����\n");

	CSFSPrint("�ƶ��豸 %d ���з��� %d ��\n", nStorageId, nCount);

	CSFSPrint("��ȷ�Ϸ���������ʵ��һ�¡�id��Ϊ��������·����Ϊ�գ�������Ϣȫ��ȷ�밴Y��������N��\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "��ѡ���˲���ʵ������ʧ��\n");
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
	
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetAllDeviceId(CSUDI_UINT32 * pdwIds,int nMaxIdCnt, int * pnActIdCnt)
//@DESCRIPTION: ֻ����豸����������豸id
//@PRECONDITION: ��
//@INPUT: pdwIds = CSUDI_NULL
//@INPUT: nMaxIdCnt = 0
//@INPUT: pnActIdCnt = &nActIdCnt������int nActIdCnt
//@EXPECTATION: ��ȡ�ɹ�������ֵ����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId(CSUDI_NULL, 0, &nActIdCnt)������ֵӦ�õ���CSUDI_SUCCESS��ͬʱnActIdCnt��С����
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetAllDeviceId_0003(void)
{
	int nActIdCnt = 0; 

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(CSUDI_NULL, 0, &nActIdCnt), "��ȡ�豸idʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt >= 0, "��ȡ�豸��������");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetAllDeviceId(CSUDI_UINT32 * pdwIds,int nMaxIdCnt, int * pnActIdCnt)
//@DESCRIPTION: ����pnActIdCntΪ�Ƿ�ֵCSUDI_NULL
//@PRECONDITION: ��
//@INPUT: pdwIds = pdwIds1������CSUDI_UINT32 pdwIds1[32]
//@INPUT: nMaxIdCnt = 32
//@INPUT: pnActIdCnt = CSUDI_NULL
//@EXPECTATION: ��ȡʧ�ܣ�����ֵ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId(pdwIds1,32,CSUDI_NULL)��ȡ�豸id������ֵӦ�ò�����CSUDI_SUCCESS
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetAllDeviceId_0004(void)
{
	CSUDI_UINT32  pdwIds1[32];

	CSTK_ASSERT_TRUE(CSUDI_SUCCESS != CSUDIFSGetAllDeviceId(pdwIds1, 32, CSUDI_NULL), "��ȡ�豸��Ӧ�óɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetAllDeviceId(CSUDI_UINT32 * pdwIds,int nMaxIdCnt, int * pnActIdCnt)
//@DESCRIPTION: ����id�������С�Ƿ�
//@PRECONDITION: ��
//@INPUT: pdwIds = pdwIds1������CSUDI_UINT32 pdwIds1[32]
//@INPUT: nMaxIdCnt = -1
//@INPUT: pnActIdCnt = &nActIdCnt������int nActIdCnt
//@EXPECTATION: ��ȡʧ�ܣ�����ֵ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId(pdwIds1,0,&nActIdCnt)������ֵӦ��������CSUDI_SUCCESS	
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetAllDeviceId_0005(void)
{
	CSUDI_UINT32  pdwIds1[32];
	int nActIdCnt = 0;

	CSTK_ASSERT_TRUE(CSUDI_SUCCESS != CSUDIFSGetAllDeviceId(pdwIds1, -1, &nActIdCnt), "��ȡ�豸id��Ӧ�óɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetAllDeviceId(CSUDI_UINT32 * pdwIds,int nMaxIdCnt, int * pnActIdCnt)
//@DESCRIPTION: ֻ��ȡ�����豸
//@PRECONDITION: �Ѳ���һ���Ѹ�ʽ����u�̣�����ʶ��
//@INPUT: pdwIds = pdwIds1������CSUDI_UINT32 pdwIds1[1]
//@INPUT: nMaxIdCnt = 1
//@INPUT: pnActIdCnt = &nActIdCnt������int nActIdCnt
//@EXPECTATION: ��ȡ�ɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId(pdwIds1,1,&nActIdCnt)��ȡ�豸��Ӧ������CSUDI_SUCCESS����nActIdCnt=1
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetAllDeviceId_0006(void)
{
	CSUDI_UINT32  pdwIds1[1];
	int nActIdCnt = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(pdwIds1, 1, &nActIdCnt), "��ȡ�豸idʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt >= 1, "��ȡ�豸��������");
		
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetAllDeviceId(CSUDI_UINT32 * pdwIds,int nMaxIdCnt, int * pnActIdCnt)
//@DESCRIPTION: ֻ����豸����������豸id����nMaxIdCnt��Ϊ0
//@PRECONDITION: ��
//@INPUT: pdwIds = CSUDI_NULL
//@INPUT: nMaxIdCnt = 32
//@INPUT: pnActIdCnt = &nActIdCnt������int nActIdCnt
//@EXPECTATION: ��ȡ�ɹ�������ֵ����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId(CSUDI_NULL, 32, &nActIdCnt)������ֵӦ�õ���CSUDI_SUCCESS��ͬʱnActIdCnt��С����
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetAllDeviceId_0007(void)
{
	int nActIdCnt = 0; 

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(CSUDI_NULL, 32, &nActIdCnt), "��ȡ�豸idʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt >= 0, "��ȡ�豸��������\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetAllDeviceId(CSUDI_UINT32 * pdwIds,int nMaxIdCnt, int * pnActIdCnt)
//@DESCRIPTION: �������߳�ͬʱ����CSUDIFSGetAllDeviceId��ȡ�豸IDʱ�᲻���������
//@PRECONDITION: ��
//@INPUT: pdwIds = CSUDI_NULL
//@INPUT: nMaxIdCnt = 32
//@INPUT: pnActIdCnt = &nActIdCnt������int nActIdCnt
//@EXPECTATION: ���߶�����������ȡ���е��豸ID
//@EXECUTIONFLOW: 1�������߳�1��ȡ���е��豸ID
//@EXECUTIONFLOW: 2�������߳�2��ȡ���е��豸ID
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetAllDeviceId_0008(void)
{
	int nPriorityHigh = 32;
	int nStckSize = 128*1024;
	CSUDI_HANDLE hThreadhandle1 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle2 = CSUDI_NULL;
	int nInitialCount = 1;
	int nMaxCount = 10;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("FS_LockTestSem", nInitialCount, nMaxCount, &g_hSemaphore), "�ź�������ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("Lock_TestThread1", nPriorityHigh , nStckSize, 
							FS_LockTestThreadEntry1, CSUDI_NULL, &hThreadhandle1),
							 "step 1 ��������ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("Lock_TestThread2", nPriorityHigh , nStckSize, 
							FS_LockTestThreadEntry2, CSUDI_NULL, &hThreadhandle2),
							 "step 2 ��������ʧ��");
	while (!g_bThread2Return || !g_bThread2Return)
	{
		CSUDIOSThreadSleep(1000);
	}
	CSTK_ASSERT_TRUE_FATAL((!g_bThreadErr), "��ȡ�豸ʧ��");

	CSTK_FATAL_POINT;
	if (g_hSemaphore != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(g_hSemaphore), "ɾ���ź���ʧ��");
			g_hSemaphore = CSUDI_NULL;
	}

	if (hThreadhandle1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle1), "ɾ������ʧ��");
			hThreadhandle1 = CSUDI_NULL;
	}

	if (hThreadhandle2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle2), "ɾ������ʧ��");
			hThreadhandle2 = CSUDI_NULL;
	}
	g_bThreadErr = CSUDI_FALSE;
	g_bThread1Return = CSUDI_FALSE;
	g_bThread2Return = CSUDI_FALSE;
	g_hSemaphore = CSUDI_NULL;

	return CSUDI_TRUE;
}

/****************************CSUDIFSGetAllDeviceId  End *******************************/

/****************************CSUDIFSGetDeviceInfo  Start ******************************/
//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetDeviceInfo(CSUDI_UINT32 dwDeviceId,CSUDIFSDeviceInfo_S * psDeviceInfo)
//@DESCRIPTION: ��������豸����Ϣ
//@PRECONDITION: ϵͳ�����ٴ���һ����ʶ����豸
//@INPUT: dwDeviceId = dwDeviceId1��dwDeviceId1����CSUDIFSGetAllDeviceId()��õ��豸��
//@INPUT: psDeviceInfo = &sDeviceInfo, ����CSUDIFSDeviceInfo_S  sDeviceInfo
//@EXPECTATION: ����ֵ���� CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ����ʶ����豸dwDeviceId1	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo(dwDeviceId1, &sDeviceInfo)���жϷ���ֵ	
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetDeviceInfo_0001(void)
{
	CSUDI_UINT32   pdwIds1[32];
	int nActIdCnt = 0;
	CSUDIFSDeviceInfo_S  sDeviceInfo;
	int i = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(pdwIds1, 32, &nActIdCnt), "��ȡ�豸idʧ��\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(pdwIds1[i], &sDeviceInfo), "��ȡ�豸��Ϣʧ��\n");
		
		if((pdwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_STORAGE_MAJOR_DEVICEID)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == strlen(sDeviceInfo.m_szMountPoint), "���豸��Ӧ���й��ص�" );
		}
		else if((pdwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "�־��豸Ӧ���й��ص�");
		}
		else if((pdwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_TUNER_MAJOR_DEVICEID)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == strlen(sDeviceInfo.m_szMountPoint), "Tuner�豸��Ӧ���й��ص�" );
		}
		else
		{
			if (CSUDI_ISREMOVEABLE(pdwIds1[i]))
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "��õ��豸�Ų�����֪���豸����" );
			}
			else if (sDeviceInfo.m_eDeviceType == EM_UDIFS_DEVTYPE_VOLUME)
			{
			    CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "���ط־��豸Ӧ���й��ص�");
			}
			else
			{
				continue;
			}
		}
	}

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetDeviceInfo(CSUDI_UINT32 dwDeviceId,CSUDIFSDeviceInfo_S * psDeviceInfo)
//@DESCRIPTION: ��ò����ڵ��豸��Ӧ����Ϣ
//@PRECONDITION: ��
//@INPUT: dwDeviceId = 0x0001FFFF(����0x0001FFFFΪ�����ڵ��豸id)
//@INPUT: psDeviceInfo = &sDeviceInfo,����CSUDIFSDeviceInfo_S sDeviceInfo
//@EXPECTATION: ����ֵ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIFSGetDeviceInfo(0x0001FFFF, &sDeviceInfo)���жϷ���ֵ	
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetDeviceInfo_0002(void)
{
	CSUDIFSDeviceInfo_S sDeviceInfo;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFSGetDeviceInfo(0x0001FFFF, &sDeviceInfo), "��ȡ�����ڵ��豸��Ϣ��Ӧ�óɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDI_Error_Code CSUDIFSGetDeviceInfo(CSUDI_UINT32 dwDeviceId,CSUDIFSDeviceInfo_S * psDeviceInfo)
//@DESCRIPTION: �����豸��Ϣ��ָ��Ƿ�
//@PRECONDITION: ϵͳ�����ٴ���һ����ʶ����豸
//@INPUT: dwDeviceId = dwDeviceId1��dwDeviceId1����CSUDIFSGetAllDeviceId()��õ��豸��
//@INPUT: psDeviceInfo = NULL
//@EXPECTATION: ����ֵ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()���һ����ʶ����豸dwDeviceId1	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo(dwDeviceId1, NULL)��ȡ�豸ͣϢ���жϷ���ֵ	
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetDeviceInfo_0003(void)
{
	CSUDI_UINT32   pdwIds1[32];
	int nActIdCnt = 0;
	int i = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(pdwIds1, 32, &nActIdCnt), "��ȡ�豸idʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "һ���豸Ҳ�����ڣ��޷�����豸��Ϣ\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFSGetDeviceInfo(pdwIds1[i], NULL), "��ȡ�豸��Ϣ��Ӧ�óɹ�\n");
	}

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetDeviceInfo(CSUDI_UINT32 dwDeviceId,CSUDIFSDeviceInfo_S * psDeviceInfo)
//@DESCRIPTION: �ж�ϵͳ�Ƿ�֧��FAT��ʽ
//@PRECONDITION: 
//@INPUT: dwDeviceId = dwVolumeId��dwVolumeId����CSUDIFSGetAllDeviceId()��õ��ƶ��־��豸��
//@INPUT: psDeviceInfo = &sDeviceInfo, ����CSUDIFSDeviceInfo_S  sDeviceInfo
//@EXPECTATION: ����ֵ���� CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������һ������ֻ��һ���������ҷ�����ʽΪFAT��ʽ����
//@EXECUTIONFLOW: 2������CSUDIFSGetAllDeviceId()�õ����е��豸��������Щ�豸���ҵ��ƶ��־��豸dwVolumeId��Ӧ�����ҵ�
//@EXECUTIONFLOW: 3������CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo)��Ӧ�û�ȡ�ɹ������й��ص�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetDeviceInfo_0004(void)
{
	CSUDI_UINT32  dwIds1[32];
	int  nActIdCnt = 0;
	CSUDI_UINT32 dwVolumeId = -1;
	int i = 0;
	CSUDIFSDeviceInfo_S sDeviceInfo;
	CSUDIFSStatFs_S sStatFs;

	CSFSPrint("�����һ��ֻ��һ���������ҷ�����ʽΪFAT32���̣������ұ���ʶ��Yȷ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N������ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds1, 32, &nActIdCnt), "��ȡ���е��豸ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "һ���豸Ҳû�У�ʧ��\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			dwVolumeId = dwIds1[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo), "��ȡ�豸��Ϣʧ��\n");
			CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "�־��豸Ӧ���й��ص�");
			memset(&sStatFs, 0x00, sizeof(CSUDIFSStatFs_S));
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStatfs(sDeviceInfo.m_szMountPoint, &sStatFs), "��ȡ�־���Ϣʧ��");
			CSTK_ASSERT_TRUE_FATAL(EM_UDIFS_PARTITION_FAT32 == sStatFs.m_eFsType, "������ʽ��ΪFAT32");
			return CSUDI_TRUE;
		}
	}

	CSFSPrint("�Ҳ����־��豸\n");

	CSTK_FATAL_POINT
	{
		;
	}

	return CSUDI_FALSE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetDeviceInfo(CSUDI_UINT32 dwDeviceId,CSUDIFSDeviceInfo_S * psDeviceInfo)
//@DESCRIPTION: �ж�ϵͳ�Ƿ�֧��NTFS��ʽ
//@PRECONDITION: 
//@INPUT: dwDeviceId = dwVolumeId��dwVolumeId����CSUDIFSGetAllDeviceId()��õ��ƶ��־��豸��
//@INPUT: psDeviceInfo = &sDeviceInfo, ����CSUDIFSDeviceInfo_S  sDeviceInfo
//@EXPECTATION: ����ֵ���� CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������һ������ֻ��һ���������ҷ�����ʽΪNTFS��ʽ����
//@EXECUTIONFLOW: 2������CSUDIFSGetAllDeviceId()�õ����е��豸��������Щ�豸���ҵ��ƶ��־��豸dwVolumeId��Ӧ�����ҵ�
//@EXECUTIONFLOW: 3������CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo)��Ӧ�û�ȡ�ɹ������й��ص�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetDeviceInfo_0005(void)
{
	CSUDI_UINT32  dwIds1[32];
	int  nActIdCnt = 0;
	CSUDI_UINT32 dwVolumeId = -1;
	int i = 0;
	CSUDIFSDeviceInfo_S sDeviceInfo;
	CSUDIFSStatFs_S sStatFs;

	CSFSPrint("�����һ��ֻ��һ���������ҷ�����ʽΪNTFS���̣������ұ���ʶ��Yȷ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N������ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds1, 32, &nActIdCnt), "��ȡ���е��豸ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "һ���豸Ҳû�У�ʧ��\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			dwVolumeId = dwIds1[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo), "��ȡ�豸��Ϣʧ��\n");
			CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "�־��豸Ӧ���й��ص�");
			memset(&sStatFs, 0x00, sizeof(CSUDIFSStatFs_S));
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStatfs(sDeviceInfo.m_szMountPoint, &sStatFs), "��ȡ�־���Ϣʧ��");
			CSTK_ASSERT_TRUE_FATAL(EM_UDIFS_PARTITION_NTFS == sStatFs.m_eFsType, "������ʽ��ΪNTFS");
			return CSUDI_TRUE;
		}
	}

	CSFSPrint("�Ҳ����־��豸\n");

	CSTK_FATAL_POINT
	{
		;
	}

	return CSUDI_FALSE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetDeviceInfo(CSUDI_UINT32 dwDeviceId,CSUDIFSDeviceInfo_S * psDeviceInfo)
//@DESCRIPTION: �ж�ϵͳ�Ƿ�֧��EXT2��ʽ
//@PRECONDITION: 
//@INPUT: dwDeviceId = dwVolumeId��dwVolumeId����CSUDIFSGetAllDeviceId()��õ��ƶ��־��豸��
//@INPUT: psDeviceInfo = &sDeviceInfo, ����CSUDIFSDeviceInfo_S  sDeviceInfo
//@EXPECTATION: ����ֵ���� CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������һ������ֻ��һ���������ҷ�����ʽΪEXT2��ʽ����
//@EXECUTIONFLOW: 2������CSUDIFSGetAllDeviceId()�õ����е��豸��������Щ�豸���ҵ��ƶ��־��豸dwVolumeId��Ӧ�����ҵ�
//@EXECUTIONFLOW: 3������CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo)��Ӧ�û�ȡ�ɹ������й��ص�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetDeviceInfo_0006(void)
{
	CSUDI_UINT32  dwIds1[32];
	int  nActIdCnt = 0;
	CSUDI_UINT32 dwVolumeId = -1;
	int i = 0;
	CSUDIFSDeviceInfo_S sDeviceInfo;
	CSUDIFSStatFs_S sStatFs;

	CSFSPrint("�����һ��ֻ��һ���������ҷ�����ʽΪEXT2���̣������ұ���ʶ��Yȷ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N������ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds1, 32, &nActIdCnt), "��ȡ���е��豸ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "һ���豸Ҳû�У�ʧ��\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			dwVolumeId = dwIds1[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo), "��ȡ�豸��Ϣʧ��\n");
			CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "�־��豸Ӧ���й��ص�");
			memset(&sStatFs, 0x00, sizeof(CSUDIFSStatFs_S));
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStatfs(sDeviceInfo.m_szMountPoint, &sStatFs), "��ȡ�־���Ϣʧ��");
			CSTK_ASSERT_TRUE_FATAL(EM_UDIFS_PARTITION_EXT2 == sStatFs.m_eFsType, "������ʽ��ΪEXT2");
			return CSUDI_TRUE;
		}
	}

	CSFSPrint("�Ҳ����־��豸\n");

	CSTK_FATAL_POINT
	{
		;
	}

	return CSUDI_FALSE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetDeviceInfo(CSUDI_UINT32 dwDeviceId,CSUDIFSDeviceInfo_S * psDeviceInfo)
//@DESCRIPTION: �ж�ϵͳ�Ƿ�֧��EXT3��ʽ
//@PRECONDITION: 
//@INPUT: dwDeviceId = dwVolumeId��dwVolumeId����CSUDIFSGetAllDeviceId()��õ��ƶ��־��豸��
//@INPUT: psDeviceInfo = &sDeviceInfo, ����CSUDIFSDeviceInfo_S  sDeviceInfo
//@EXPECTATION: ����ֵ���� CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������һ������ֻ��һ���������ҷ�����ʽΪEXT3��ʽ����
//@EXECUTIONFLOW: 2������CSUDIFSGetAllDeviceId()�õ����е��豸��������Щ�豸���ҵ��ƶ��־��豸dwVolumeId��Ӧ�����ҵ�
//@EXECUTIONFLOW: 3������CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo)��Ӧ�û�ȡ�ɹ������й��ص�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetDeviceInfo_0007(void)
{
	CSUDI_UINT32  dwIds1[32];
	int  nActIdCnt = 0;
	CSUDI_UINT32 dwVolumeId = -1;
	int i = 0;
	CSUDIFSDeviceInfo_S sDeviceInfo;
	CSUDIFSStatFs_S sStatFs;

	CSFSPrint("�����һ��ֻ��һ���������ҷ�����ʽΪEXT3���̣������ұ���ʶ��Yȷ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N������ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds1, 32, &nActIdCnt), "��ȡ���е��豸ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "һ���豸Ҳû�У�ʧ��\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			dwVolumeId = dwIds1[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo), "��ȡ�豸��Ϣʧ��\n");
			CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "�־��豸Ӧ���й��ص�");
			memset(&sStatFs, 0x00, sizeof(CSUDIFSStatFs_S));
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStatfs(sDeviceInfo.m_szMountPoint, &sStatFs), "��ȡ�־���Ϣʧ��");
			CSTK_ASSERT_TRUE_FATAL(EM_UDIFS_PARTITION_EXT3 == sStatFs.m_eFsType, "������ʽ��ΪEXT3");
			return CSUDI_TRUE;
		}
	}

	CSFSPrint("�Ҳ����־��豸\n");

	CSTK_FATAL_POINT
	{
		;
	}

	return CSUDI_FALSE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetDeviceInfo(CSUDI_UINT32 dwDeviceId,CSUDIFSDeviceInfo_S * psDeviceInfo)
//@DESCRIPTION: �ж�ϵͳ�Ƿ�֧��JFFS2��ʽ
//@PRECONDITION: 
//@INPUT: dwDeviceId = dwVolumeId��dwVolumeId����CSUDIFSGetAllDeviceId()��õ��ƶ��־��豸��
//@INPUT: psDeviceInfo = &sDeviceInfo, ����CSUDIFSDeviceInfo_S  sDeviceInfo
//@EXPECTATION: ����ֵ���� CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������һ������ֻ��һ���������ҷ�����ʽΪJFFS2��ʽ����
//@EXECUTIONFLOW: 2������CSUDIFSGetAllDeviceId()�õ����е��豸��������Щ�豸���ҵ��ƶ��־��豸dwVolumeId��Ӧ�����ҵ�
//@EXECUTIONFLOW: 3������CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo)��Ӧ�û�ȡ�ɹ������й��ص�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetDeviceInfo_0008(void)
{
	CSUDI_UINT32  dwIds1[32];
	int  nActIdCnt = 0;
	CSUDI_UINT32 dwVolumeId = -1;
	int i = 0;
	CSUDIFSDeviceInfo_S sDeviceInfo;
	CSUDIFSStatFs_S sStatFs;

	CSFSPrint("�����һ��ֻ��һ���������ҷ�����ʽΪJFFS2���̣������ұ���ʶ��Yȷ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N������ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds1, 32, &nActIdCnt), "��ȡ���е��豸ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "һ���豸Ҳû�У�ʧ��\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			dwVolumeId = dwIds1[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo), "��ȡ�豸��Ϣʧ��\n");
			CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "�־��豸Ӧ���й��ص�");
			memset(&sStatFs, 0x00, sizeof(CSUDIFSStatFs_S));
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStatfs(sDeviceInfo.m_szMountPoint, &sStatFs), "��ȡ�־���Ϣʧ��");
			CSTK_ASSERT_TRUE_FATAL(EM_UDIFS_PARTITION_JFFS2 == sStatFs.m_eFsType, "������ʽ��ΪJFFS2");
			return CSUDI_TRUE;
		}
	}

	CSFSPrint("�Ҳ����־��豸\n");

	CSTK_FATAL_POINT
	{
		;
	}

	return CSUDI_FALSE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetDeviceInfo(CSUDI_UINT32 dwDeviceId,CSUDIFSDeviceInfo_S * psDeviceInfo)
//@DESCRIPTION: ��ȡһ����д�Ĺ̶��־�
//@PRECONDITION: 
//@INPUT: dwDeviceId = dwVolumeId��dwVolumeId����CSUDIFSGetAllDeviceId()��õĹ̶��־��豸��
//@INPUT: psDeviceInfo = &sDeviceInfo, ����CSUDIFSDeviceInfo_S  sDeviceInfo
//@EXPECTATION: ���ҵ�һ����д�Ĺ̶��־��豸
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ����е��豸��������Щ�豸���ҵ��̶��־��豸dwVolumeId��Ӧ�����ҵ�
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo)��Ӧ�û�ȡ�ɹ������й��ص�
//@EXECUTIONFLOW: 3������CSUDIFSStatfs(�������ص�, &sStatFs)��ȡ�ļ�ϵͳ��Ϣ��Ӧ�û�ȡ�ɹ�
//@EXECUTIONFLOW: 4�������ļ�ϵͳ�����͵�ȡֵ��Χ�Ƿ���EM_UDIFS_PARTITION_RW_START��EM_UDIFS_PARTITION_RW_END֮�䣬�������ȡ�ɹ�
//@EXECUTIONFLOW: 5��ѭ��1��4������һ��Ҳ�Ҳ�����ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetDeviceInfo_0009(void)
{
	CSUDI_UINT32  dwIds1[32];
	int  nActIdCnt = 0;
	CSUDI_UINT32 dwVolumeId = -1;
	int i = 0;
	CSUDIFSDeviceInfo_S sDeviceInfo;
	CSUDIFSStatFs_S  sStatFs;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds1, 32, &nActIdCnt), "��ȡ���е��豸ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "һ���豸Ҳû�У�ʧ��\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) != CSUDI_RDI_MAJOR_DEVICEID_MASK)
		{
			dwVolumeId = dwIds1[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo), "��ȡ�豸��Ϣʧ��\n");
			if(sDeviceInfo.m_eDeviceType == EM_UDIFS_DEVTYPE_VOLUME)
			{
				CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "�־��豸Ӧ���й��ص�\n");
				CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStatfs(sDeviceInfo.m_szMountPoint, &sStatFs), "��ȡ�ļ�ϵͳ��Ϣʧ��\n");

				if(sStatFs.m_eFsType > EM_UDIFS_PARTITION_RW_START && sStatFs.m_eFsType < EM_UDIFS_PARTITION_RW_END)
				{
					return CSUDI_TRUE;
				}
			}
		}
	}

	CSTK_FATAL_POINT
	{
		;
	}
	CSFSPrint("�Ҳ���һ����д�Ĺ̶��־�\n");
	return CSUDI_FALSE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetDeviceInfo(CSUDI_UINT32 dwDeviceId,CSUDIFSDeviceInfo_S * psDeviceInfo)
//@DESCRIPTION: ��ȡһ��ֻ���Ĺ̶��־�
//@PRECONDITION: 
//@INPUT: dwDeviceId = dwVolumeId��dwVolumeId����CSUDIFSGetAllDeviceId()��õĹ̶��־��豸��
//@INPUT: psDeviceInfo = &sDeviceInfo, ����CSUDIFSDeviceInfo_S  sDeviceInfo
//@EXPECTATION: ���ҵ�һ���ɶ��Ĺ̶��־��豸
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ����е��豸��������Щ�豸���ҵ��̶��־��豸dwVolumeId��Ӧ�����ҵ�
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo)��Ӧ�û�ȡ�ɹ������й��ص�
//@EXECUTIONFLOW: 3������CSUDIFSStatfs(�������ص�, &sStatFs)��ȡ�ļ�ϵͳ��Ϣ��Ӧ�û�ȡ�ɹ�
//@EXECUTIONFLOW: 4�������ļ�ϵͳ�����͵�ȡֵ��Χ�Ƿ���EM_UDIFS_PARTITION_READONLY_START��EM_UDIFS_PARTITION_READONLY_END֮�䣬�������ȡ�ɹ�
//@EXECUTIONFLOW: 5��ѭ��1��4������һ��Ҳ�Ҳ�����ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetDeviceInfo_0010(void)
{
	CSUDI_UINT32  dwIds1[32];
	int  nActIdCnt = 0;
	CSUDI_UINT32 dwVolumeId = -1;
	int i = 0;
	CSUDIFSDeviceInfo_S sDeviceInfo;
	CSUDIFSStatFs_S  sStatFs;
	
	CSFSPrint("�����һ��������һ��ֻ���־���豸�������ұ���ʶ��Yȷ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N������ʧ��");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds1, 32, &nActIdCnt), "��ȡ���е��豸ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "һ���豸Ҳû�У�ʧ��\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) != CSUDI_RDI_MAJOR_DEVICEID_MASK)
		{
			dwVolumeId = dwIds1[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo), "��ȡ�豸��Ϣʧ��\n");
			if(sDeviceInfo.m_eDeviceType == EM_UDIFS_DEVTYPE_VOLUME)
			{
				CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "�־��豸Ӧ���й��ص�\n");
				CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStatfs(sDeviceInfo.m_szMountPoint, &sStatFs), "��ȡ�ļ�ϵͳ��Ϣʧ��\n");

				if(sStatFs.m_eFsType > EM_UDIFS_PARTITION_READONLY_START && sStatFs.m_eFsType < EM_UDIFS_PARTITION_READONLY_END)
				{
					return CSUDI_TRUE;
				}
			}
		}
	}

	CSTK_FATAL_POINT
	{
		;
	}
	CSFSPrint("�Ҳ���һ��ֻ���Ĺ̶��־�\n");
	return CSUDI_FALSE;
}


//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetDeviceInfo(CSUDI_UINT32 dwDeviceId,CSUDIFSDeviceInfo_S * psDeviceInfo)
//@DESCRIPTION: UDI FS Ҫ�����������־��豸,һ��ֻ��,һ����д,�����и��洢�豸
//@PRECONDITION: 
//@INPUT: dwDeviceId = dwVolumeId��dwVolumeId����CSUDIFSGetAllDeviceId()��õĹ̶��־��豸��
//@INPUT: psDeviceInfo = &sDeviceInfo, ����CSUDIFSDeviceInfo_S  sDeviceInfo
//@EXPECTATION: UDI FS Ҫ�����������־��豸,һ��ֻ��,һ����д,�����и��洢�豸
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ����е��豸��������Щ�豸���ҵ��̶��־��豸dwVolumeId��Ӧ�����ҵ�
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo)��Ӧ�û�ȡ�ɹ������й��ص�
//@EXECUTIONFLOW: 3������ǰ�Ƿ־��豸���豸,��sDeviceInfo.m_dwParentDeviceIdΪ���洢�豸ID
//@EXECUTIONFLOW: 4������CSUDIFSStatfs(�������ص�, &sStatFs)��ȡ�ļ�ϵͳ��Ϣ��Ӧ�û�ȡ�ɹ�
//@EXECUTIONFLOW: 5�������ļ�ϵͳ�����͵�ȡֵ��Χ�Ƿ���EM_UDIFS_PARTITION_READONLY_START��EM_UDIFS_PARTITION_READONLY_END֮�䣬�������ȡ�ɹ�
//@EXECUTIONFLOW: 6�������ļ�ϵͳ�����͵�ȡֵ��Χ�Ƿ���EM_UDIFS_PARTITION_RW_START��EM_UDIFS_PARTITION_RW_END֮�䣬�������ȡ�ɹ�
//@EXECUTIONFLOW: 7��ѭ��1��6�������ҵ�����һ��ֻ���־��豸,����һ����д�־��豸,�����и������豸��ɹ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetDeviceInfo_0011(void)
{
	CSUDI_UINT32  dwIds1[32];
	int  nActIdCnt = 0;
	int i = 0;
	CSUDIFSDeviceInfo_S sDeviceInfo;
	CSUDIFSStatFs_S  sStatFs;
	int nReadCount = 0;
	int nWriteCount = 0;
	int nStorageId = -1;

	CSFSPrint("�����һ�������������־��豸,һ��ֻ��,һ����д,�����и��洢�豸���ƶ��豸�������ұ���ʶ��Yȷ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N������ʧ��");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds1, 32, &nActIdCnt), "��ȡ���е��豸ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "һ���豸Ҳû�У�ʧ��\n");

	CSTCPrint("the total is %d \n",nActIdCnt);

	for(i = 0; i < nActIdCnt; i++)
	{
		CSTCPrint("the dwIds1[%d] is 0x%x \n",i,dwIds1[i]);
	
		if((dwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwIds1[i], &sDeviceInfo), "��ȡ�豸��Ϣʧ��\n");
			if(nStorageId == -1)
			{
				nStorageId = sDeviceInfo.m_dwParentDeviceId;
			}

			if((int)sDeviceInfo.m_dwParentDeviceId == nStorageId)
			{
				//CSFSPrint("����id = %d, ����·�� = %s, ���洢�豸id = %d\n", dwIds1[i], sDeviceInfo.m_szMountPoint, nStorageId);
				//nCount++;
			}
			else
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "���ֶ���ƶ��洢�豸��Ӧ��ֻ��һ��\n");
			}

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwIds1[i], &sDeviceInfo), "��ȡ�豸��Ϣʧ��\n");
			CSTCPrint("sDeviceInfo.m_eDeviceType = 0x%x\n",sDeviceInfo.m_eDeviceType);
			CSTCPrint("sDeviceInfo.m_szMountPoint = %s\n",sDeviceInfo.m_szMountPoint);
			
			CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "�־��豸Ӧ���й��ص�\n");
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStatfs(sDeviceInfo.m_szMountPoint, &sStatFs), "��ȡ�ļ�ϵͳ��Ϣʧ��\n");

			if(sStatFs.m_eFsType > EM_UDIFS_PARTITION_READONLY_START && sStatFs.m_eFsType < EM_UDIFS_PARTITION_READONLY_END)
			{
				nReadCount ++;
			}
			if(sStatFs.m_eFsType > EM_UDIFS_PARTITION_RW_START && sStatFs.m_eFsType < EM_UDIFS_PARTITION_RW_END)
			{
				nWriteCount ++;
			}	

		}
	
	}
    
    CSTCPrint("nStorageId= %d  nReadCount=%d nWriteCount=%d \n",nStorageId,nReadCount,nWriteCount);
	CSTK_ASSERT_TRUE_FATAL(nStorageId >= 0 , "��ȡ���洢�豸ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nReadCount >= 1, "��ȡֻ���־��豸ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nWriteCount >= 1 , "��ȡ��д�־��豸ʧ��\n");
	return CSUDI_TRUE;
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_FALSE;
}
/****************************CSUDIFSGetDeviceInfo  End *******************************/

/****************************CSUDIFSStandby  Start ******************************/
//@CASEGROUP: CSUDI_Error_Code CSUDIFSStandby(CSUDI_UINT32 dwDeviceId)
//@DESCRIPTION: ʹһ���Ѵ��ڵĴ洢�豸�������״̬
//@PRECONDITION: ϵͳ�����ٴ���һ����ʶ����豸
//@INPUT: dwDeviceId = dwDeviceId1, ����dwDeviceId1����CSUDIFSGetAllDeviceId()��õĴ洢�豸
//@EXPECTATION: ����ֵ����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ����ʶ��Ĵ洢�豸dwDeviceId1	
//@EXECUTIONFLOW: 2������CSUDIFSStandby(dwDeviceId1)ʹ�豸������������ֵ��ΪCSUDI_SUCCESS��ʧ��
//@EXECUTIONFLOW: 3������CSUDIFSWakeup(dwDeviceId1)�����豸���ָ�����
CSUDI_BOOL CSTC_FS_IT_CSUDIFSStandby_0001(void)
{
	CSUDI_UINT32  dwIds1[32];
	int  nActIdCnt = 0;
	int dwDeviceId1 = -1;
	int i = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds1, 32, &nActIdCnt), "��ȡ���е��豸ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "һ���豸Ҳû�У��޷�����\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_STORAGE_MAJOR_DEVICEID)
		{
			dwDeviceId1 = (int)dwIds1[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSStandby(dwDeviceId1), "�����洢�豸ʧ��\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSWakeup(dwDeviceId1), "�������Ѵ洢�豸ʧ��\n");	
			return CSUDI_TRUE;
		}
	}

	CSTK_ASSERT_TRUE(dwDeviceId1 != -1, "�Ҳ���һ���洢�豸���޷�����\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_FALSE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSStandby(CSUDI_UINT32 dwDeviceId)
//@DESCRIPTION: ʹһ�������ڵĴ洢�豸�������״̬
//@PRECONDITION: ��
//@INPUT: dwDeviceId = 0x7FFFFFFF(����0x7FFFFFFFΪ�����ڵ��豸)
//@EXPECTATION: ����ֵ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIFSStandby(0x7FFFFFFF)�����洢�豸	
CSUDI_BOOL CSTC_FS_IT_CSUDIFSStandby_0002(void)
{
	CSTK_ASSERT_TRUE(CSUDI_SUCCESS != CSUDIFSStandby(0x7FFFFFFF), "���������ڵ��豸��Ӧ�óɹ�\n");
	return CSUDI_TRUE;
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_FALSE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSStandby(CSUDI_UINT32 dwDeviceId)
//@DESCRIPTION: ʹһ���־��豸�������״̬
//@PRECONDITION: ϵͳ�����ٴ���һ����ʶ��ķ־��豸
//@INPUT: dwDeviceId = dwVolumeId, ����dwVolumeId����CSUDIFSGetAllDeviceId()��õķ־��豸
//@EXPECTATION: ����ֵ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ����е��豸�����������豸���ҵ�һ���־��豸dwVolumeId�����Ҳ����־��豸�����ʧ��
//@EXECUTIONFLOW: 2������CSUDIFSStandby(dwVolumeId)ʹ�־��豸�������жϷ���ֵ	
CSUDI_BOOL CSTC_FS_IT_CSUDIFSStandby_0003(void)
{
	CSUDI_UINT32  dwIds1[32];
	int  nActIdCnt = 0;
	int dwVolumeId = -1;
	int i = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds1, 32, &nActIdCnt), "��ȡ���е��豸ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "һ���豸Ҳû�У��޷�����\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			dwVolumeId = (int)dwIds1[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFSStandby(dwVolumeId), "�����־��豸��Ӧ�óɹ�\n");
			return CSUDI_TRUE;
		}
	}

	CSTK_ASSERT_TRUE_FATAL(dwVolumeId != -1, "�Ҳ���һ���־��豸���޷�����\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_FALSE;	
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSStandby(CSUDI_UINT32 dwDeviceId)
//@DESCRIPTION: ʹһ���Ѵ����Ĵ洢�豸�������״̬
//@PRECONDITION: ϵͳ������һ���洢�豸����
//@INPUT: dwDeviceId = dwDeviceId1, ����dwDeviceId1����CSUDIFSGetAllDeviceId()��õĴ洢�豸id
//@EXPECTATION: ����ֵ����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ����ʶ��Ĵ洢�豸dwDeviceId1
//@EXECUTIONFLOW: 2������CSUDIFSStandby(dwDeviceId1)ʹ�洢�豸dwDeviceId1����
//@EXECUTIONFLOW: 3������CSUDIFSStandby(dwDeviceId1)�ٴ�ʹ�洢�豸dwDeviceId1����
//@EXECUTIONFLOW: 4������CSUDIFSWakeup(dwDeviceId1)�����豸���ָ�����
CSUDI_BOOL CSTC_FS_IT_CSUDIFSStandby_0004(void)
{
	CSUDI_UINT32  dwIds1[32];
	int  nActIdCnt = 0;
	int dwDeviceId1 = -1;
	int i = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds1, 32, &nActIdCnt), "��ȡ���е��豸ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "һ���豸Ҳû�У��޷�����\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_STORAGE_MAJOR_DEVICEID)
		{
			dwDeviceId1 = (int)dwIds1[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSStandby(dwDeviceId1), "�����洢�豸ʧ��\n");

			CSTK_ASSERT_TRUE(CSUDI_SUCCESS == CSUDIFSStandby(dwDeviceId1), "�����Ѵ����Ĵ洢�豸ʧ��\n");
			
			CSTK_ASSERT_TRUE(CSUDI_SUCCESS == CSUDIFSWakeup(dwDeviceId1), "�������Ѵ洢�豸ʧ��\n");	
			
			return CSUDI_TRUE;
		}
	}
	
	CSTK_ASSERT_TRUE(dwDeviceId1 != -1, "�Ҳ���һ���洢�豸���޷�����\n");
	
	CSTK_FATAL_POINT
	{
		;
	}
	return  CSUDI_FALSE;
}
/****************************CSUDIFSStandby  End *******************************/

/****************************CSUDIFSWakeup  Start ******************************/ 
//@CASEGROUP: CSUDI_Error_Code CSUDIFSWakeup(CSUDI_UINT32 dwDeviceId) 
//@DESCRIPTION: ����һ�����ڴ���״̬�Ĵ洢�豸
//@PRECONDITION: ϵͳ�����ٴ���һ���洢�豸
//@INPUT: dwDeviceIdΪһ�����ڴ���״̬�Ĵ洢�豸��dwDeviceId1����CSUDIFSGetAllDeviceId()���
//@EXPECTATION: ����ֵ���� CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ����ʶ��Ĵ洢�豸dwDeviceId1
//@EXECUTIONFLOW: 2������CSUDIFSStandby(dwDeviceId1)ʹ�豸����
//@EXECUTIONFLOW: 3������CSUDIFSWakeup(dwDeviceId1)�����豸���жϷ���ֵ
CSUDI_BOOL CSTC_FS_IT_CSUDIFSWakeup_0001(void)
{
	CSUDI_UINT32  dwIds1[32];
	int  nActIdCnt = 0;
	int dwDeviceId1 = -1;
	int i = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds1, 32, &nActIdCnt), "��ȡ���е��豸ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "һ���豸Ҳû��\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_STORAGE_MAJOR_DEVICEID)
		{
			dwDeviceId1 = (int)dwIds1[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSStandby(dwDeviceId1), "�����洢�豸ʧ��\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSWakeup(dwDeviceId1), "���Ѵ洢�豸ʧ��\n");	
			return CSUDI_TRUE;
		}
	}

	CSTK_ASSERT_TRUE(dwDeviceId1 != -1, "�Ҳ���һ���洢�豸���޷�����\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_FALSE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSWakeup(CSUDI_UINT32 dwDeviceId) 
//@DESCRIPTION: ����һ�������ڵĴ洢�豸
//@PRECONDITION: ��
//@INPUT: dwDeviceId = 0x7FFFFFFF(����0x7FFFFFFFΪ�����ڵ��豸)
//@EXPECTATION: ����ֵ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIFSWakeup(0x7FFFFFFF)�����豸���жϷ���ֵ	
CSUDI_BOOL CSTC_FS_IT_CSUDIFSWakeup_0002(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFSWakeup(0x7FFFFFFF), "���Ѳ����ڵ��豸��Ӧ�óɹ�\n");

	return CSUDI_TRUE;
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_FALSE;		
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSWakeup(CSUDI_UINT32 dwDeviceId) 
//@DESCRIPTION: ����һ���־��豸
//@PRECONDITION: ϵͳ�����ٴ���һ����ʶ��ķ־��豸
//@INPUT: dwDeviceId = dwVolumeId��dwVolumeId����CSUDIFSGetAllDeviceId()��õķ־��豸
//@EXPECTATION: ����ֵ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ����ʶ��ķ־��豸dwVolumeId
//@EXECUTIONFLOW: 2������CSUDIFSWakeup(dwVolumeId)���ѷ־��豸	
CSUDI_BOOL CSTC_FS_IT_CSUDIFSWakeup_0003(void)
{
	CSUDI_UINT32  dwIds1[32];
	int  nActIdCnt = 0;
	int dwVolumeId = -1;
	int i = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds1, 32, &nActIdCnt), "��ȡ���е��豸ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "һ���豸Ҳû�У��޷�����\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			dwVolumeId = (int)dwIds1[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFSWakeup(dwVolumeId), "���ѷ־��豸��Ӧ�óɹ�\n");	
			return CSUDI_TRUE;
		}
	}

	CSTK_ASSERT_TRUE(dwVolumeId != -1, "�Ҳ���һ���־��豸���޷�����\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_FALSE;	
}

//@CASEGROUP: CSUDIFSWakeup 
//@DESCRIPTION: ����һ��δ�����Ĵ洢�豸
//@PRECONDITION: ϵͳ������һ���洢�豸δ����
//@INPUT: dwDeviceId = dwStorageId������dwStorageId��CSUDIFSGetAllDeviceId()���
//@EXPECTATION: ����ֵ����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ����ʶ��Ĵ洢�豸dwStorageId
//@EXECUTIONFLOW: 2������CSUDIFSWakeup(dwStorageId)���Ѵ洢�豸
CSUDI_BOOL CSTC_FS_IT_CSUDIFSWakeup_0004(void)
{
	CSUDI_UINT32  dwIds1[32];
	int  nActIdCnt = 0;
	int dwStorageId = -1;
	int i = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds1, 32, &nActIdCnt), "��ȡ���е��豸ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "һ���豸Ҳû�У��޷�����\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_STORAGE_MAJOR_DEVICEID)
		{
			dwStorageId = (int)dwIds1[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSWakeup(dwStorageId), "���Ѵ洢�豸ʧ��\n");	
			return CSUDI_TRUE;
		}
	}

	CSTK_ASSERT_TRUE(dwStorageId != -1, "�Ҳ���һ���洢�豸���޷�����\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_FALSE;		
}
/****************************CSUDIFSWakeup  End *******************************/

/****************************CSUDIFSFormat  Start ******************************/ 
//@CASEGROUP: CSUDI_Error_Code CSUDIFSFormat(CSUDI_UINT32 dwDeviceId,CSUDIFSPartitionType_E eType)
//@DESCRIPTION: ��ʽ��һ����ʶ��ķ־�
//@PRECONDITION: 1��ϵͳ�����ٴ���һ����ʶ��ķ־�
//@PRECONDITION: 2��ƽ̨֧�ָ�ʽ����EM_STRG_PARTITION_FAT32
//@INPUT: dwDeviceId = dwVolumeId��dwVolume����CSUDIFSGetAllDeviceId()��õķ־��豸
//@INPUT: eType = EM_UDIFS_PARTITION_FAT32
//@EXPECTATION: ��ʽ���ɹ�����ʽ��ǰ���ļ���������
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ����ʶ��ķ־��豸dwVolumeId
//@EXECUTIONFLOW: 2������CSUDIFSOpen(��������·�� + "CSUDIFSFormat_0001", "w")���������ļ������ؾ��hFile�������Ϊ��
//@EXECUTIONFLOW: 3������CSUDIFSClose(hFile)�رո��ļ�������ֵӦ��Ϊ0
//@EXECUTIONFLOW: 4������CSUDIFSFormat(dwVolumeId, EM_UDIFS_PARTITION_FAT32)��ʽ���־��豸
//@EXECUTIONFLOW: 5������CSUDIFSOpen(��������·�� + "CSUDIFSFormat_0001", "r")��ֻ����ʽ�򿪸�ʽ��ǰ�������ļ���Ӧ��ʧ�ܣ�����CSUDI_NULL
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFormat_0001(void)
{
	//�ֶ�����
	CSUDI_UINT32   dwIds[32];
	int nActIdCnt = 0;
	int i = 0;
	int dwVolumeId = 0;
	CSUDI_HANDLE  hHandle = CSUDI_NULL;
	char szFilePath[CSUDIFS_MAX_FILE_PATH];
	CSUDIFSDeviceInfo_S   sDeviceInfo;
	int  nLen = 0;

	CSFSPrint("������������Բ����豸���и�ʽ����ȷ�ϼ����밴(Y)/�����밴(N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "ѡ�����˳�");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt), "��ȡ�豸idʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "һ���豸Ҳ�����ڣ��޷�����豸��Ϣ\n");
	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			dwVolumeId = (int)dwIds[i];

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo), "��ȡ�豸��Ϣʧ��\n");        
			CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "�־���ص�Ϊ��\n");
			CSTK_ASSERT_TRUE_FATAL(sDeviceInfo.m_szMountPoint != CSUDI_NULL, "�޷־���ص�\n");
			
			memset(szFilePath, 0, sizeof(szFilePath));
			nLen = strlen(sDeviceInfo.m_szMountPoint);
			memcpy(szFilePath, sDeviceInfo.m_szMountPoint, nLen);

			if(szFilePath[nLen -1] != '/') szFilePath[nLen] = '/';

			strcat(szFilePath, "CSUDIFSFormat_0001");
			
			CSTK_ASSERT_TRUE_FATAL((hHandle = CSUDIFSOpen(szFilePath, "w")) != CSUDI_NULL, "�����ļ�ʧ��\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDIFSClose(hHandle) == 0, "�ر��ļ�ʧ��\n");

			hHandle = CSUDI_NULL;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSFormat(dwVolumeId, EM_UDIFS_PARTITION_FAT32), "��ʽ���־�ʧ��\n");
			CSTK_ASSERT_TRUE_FATAL((hHandle = CSUDIFSOpen(szFilePath, "r")) == CSUDI_NULL, "�򿪸�ʽ��ǰ���ļ�Ӧ��ʧ��\n");
				
			return CSUDI_TRUE;
		}
	}

	CSTK_ASSERT_TRUE_FATAL(dwVolumeId != -1, "�Ҳ���һ���־��豸\n");
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_FALSE;	
}

static  CSUDIRDIEvent_E  g_eCSUDIFSFormat_0002;
void CSUDIFSFormat_0002_Callback(CSUDIRDIEvent_E eType, CSUDI_UINT32 dwDeviceId,const void * pvEventData, void * pvUserData)
{
	if(eType == EM_UDIRDI_EVENT_PLUGOUT || eType == EM_UDIRDI_EVENT_FOUND)
	{
		g_eCSUDIFSFormat_0002 = EM_UDIRDI_EVENT_FOUND;
	}
}
//@CASEGROUP: CSUDI_Error_Code CSUDIFSFormat(CSUDI_UINT32 dwDeviceId,CSUDIFSPartitionType_E eType)
//@DESCRIPTION: ��ʽ��ֻ��һ�������Ĵ洢�豸
//@PRECONDITION: 1��ϵͳ�����ٴ���һ����ʶ��Ĵ洢�豸���Ҹô洢�豸������һ���Ѹ�ʽ���ķ���
//@PRECONDITION: 2��ƽ̨֧�ָ�ʽ����EM_UDIFS_PARTITION_EXT3
//@INPUT: dwDeviceId = dwStorageId��dwStorageId����CSUDIFSGetAllDeviceId()��õĴ洢�豸
//@INPUT: eType = EM_UDIFS_PARTITION_EXT3
//@EXPECTATION: ��ʽ���ɹ���������ע��Ļص����յ�EM_UDIRDI_EVENT_PLUGOUT��EM_UDIRDI_EVENT_FOUND�¼�
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ����ʶ��Ĵ洢�豸dwStorageId
//@EXECUTIONFLOW: 2������CSUDIFSRDIAddCallback(CSUDIFSFormat_0002_Callback, CSUDI_NULL)ע��ص�������Ӧ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIFSFormat(dwStorageId, EM_UDIFS_PARTITION_EXT3)��ʽ���洢�豸��Ӧ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 4�����ص�����CSUDIFSFormat_0002_Callback���Ƿ���EM_UDIRDI_EVENT_PLUGOUT��EM_UDIRDI_EVENT_FOUND�¼�
//@EXECUTIONFLOW: 5������CSUDIFSRDIRemoveCallback(CSUDIFSFormat_0002_Callback, CSUDI_NULL)ɾ���ص��������ָ�����
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFormat_0002(void)
{
	//�ֶ�����
	CSUDI_UINT32   dwIds[32];
	int nActIdCnt = 0;
	int i =0;
	CSUDI_UINT32  dwStorageId = 0;
	CSUDI_Error_Code nErrorCode = CSUDI_SUCCESS;

	CSFSPrint("������������Բ����豸���и�ʽ����ȷ�ϼ����밴(Y)/�����밴(N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "ѡ�����˳�");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCode = CSUDIFSRDIAddCallback(CSUDIFSFormat_0002_Callback,
		CSUDI_NULL)),"��ӻص�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt), "��ȡ�豸idʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "һ���豸Ҳ�����ڣ��޷�����豸��Ϣ\n");

	g_eCSUDIFSFormat_0002 = EM_UDIRDI_EVENT_UNKNOWN;

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_STORAGE_MAJOR_DEVICEID)
		{
			dwStorageId = dwIds[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSFormat(dwStorageId, EM_UDIFS_PARTITION_EXT3), "��ʽ���洢�豸ʧ��\n");
            break;
        }
	}

	CSTK_ASSERT_TRUE_FATAL(g_eCSUDIFSFormat_0002 == EM_UDIRDI_EVENT_FOUND, "�ص�������û���յ���ʽ���¼�\n");

	CSTK_FATAL_POINT
	{
		if(nErrorCode == CSUDI_SUCCESS)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(CSUDIFSFormat_0002_Callback, 
			CSUDI_NULL), "�Ƴ��ص�ʧ��\n");		
		}
	}
	return CSUDI_TRUE;	
}

static  int  g_eCSUDIFSFormat_0003A;
static  int  g_eCSUDIFSFormat_0003B;
void CSUDIFSFormat_0003_Callback(CSUDIRDIEvent_E eType, CSUDI_UINT32 dwDeviceId,const void * pvEventData, void * pvUserData)
{
	if(eType == EM_UDIRDI_EVENT_PLUGOUT)
	{
		g_eCSUDIFSFormat_0003A ++;
	}
	if(eType == EM_UDIRDI_EVENT_FOUND)
	{
		g_eCSUDIFSFormat_0003B++;
	}
}
//@CASEGROUP: CSUDI_Error_Code CSUDIFSFormat(CSUDI_UINT32 dwDeviceId,CSUDIFSPartitionType_E eType)
//@DESCRIPTION: ��ʽ��һ�������������Ĵ洢�豸
//@PRECONDITION: 1��ϵͳ�����ٴ���һ����ʶ��Ĵ洢�豸���Ҹô洢�豸��������
//@PRECONDITION: 2��ƽ̨֧�ָ�ʽ����EM_UDIFS_PARTITION_EXT3
//@INPUT: dwDeviceId = dwStorageId��dwStorageId����CSUDIFSGetAllDeviceId()��õĴ洢�豸
//@INPUT: eType = EM_UDIFS_PARTITION_EXT3
//@EXPECTATION: ��ʽ���ɹ���������ע��Ļص����յ�����EM_UDIRDI_EVENT_PLUGOUT�¼���һ��EM_UDIRDI_EVENT_FOUND�¼�
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ����ʶ��ķ־��豸dwStorageId
//@EXECUTIONFLOW: 2������CSUDIFSRDIAddCallback(CSUDIFSFormat_0002_Callback, CSUDI_NULL)ע��ص�������Ӧ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIFSFormat(dwStorageId, EM_UDIFS_PARTITION_EXT3)��ʽ���洢�豸��Ӧ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 4�����ص�����CSUDIFSFormat_0002_Callback���Ƿ�����EM_UDIRDI_EVENT_PLUGOUT�¼���һ��EM_UDIRDI_EVENT_FOUND�¼�
//@EXECUTIONFLOW: 5������CSUDIFSRDIRemoveCallback(CSUDIFSFormat_0002_Callback, CSUDI_NULL)ɾ���ص��������ָ�����
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFormat_0003(void)
{
	//�ֶ�����
	CSUDI_UINT32   dwIds[32];
	int nActIdCnt = -1;
	int i =0;
	CSUDI_UINT32  dwStorageId = 0;
	CSUDI_Error_Code nErrorCode = CSUDI_SUCCESS;

	CSFSPrint("������������Բ����豸���и�ʽ����ȷ�ϼ����밴(Y)/�����밴(N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "ѡ�����˳�");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCode = CSUDIFSRDIAddCallback(CSUDIFSFormat_0003_Callback,
		CSUDI_NULL)),"��ӻص�ʧ��\n");

	CSFSPrint("��ȷ��U���Ƿ����������������밴(Y)/û���밴(N)\n");

	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"û����������\n");


	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt), "��ȡ�豸idʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "һ���豸Ҳ�����ڣ��޷�����豸��Ϣ\n");
	g_eCSUDIFSFormat_0003A = 0;
	g_eCSUDIFSFormat_0003B = 0;

	
	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_STORAGE_MAJOR_DEVICEID)
		{
			dwStorageId = dwIds[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSFormat(dwStorageId, EM_UDIFS_PARTITION_EXT3), "��ʽ���洢�豸ʧ��\n");
			break;
		}
	}

	CSTK_ASSERT_TRUE_FATAL(g_eCSUDIFSFormat_0003A == 2, "�����γ�����Ϣ��������\n");

	CSTK_ASSERT_TRUE_FATAL(g_eCSUDIFSFormat_0003B == 2, "���ַ�������Ϣ��������\n");

	CSTK_FATAL_POINT
	{
		if(nErrorCode == CSUDI_SUCCESS)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(CSUDIFSFormat_0003_Callback, 
			CSUDI_NULL), "�Ƴ��ص�ʧ��\n");		
		}
	}
	
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSFormat(CSUDI_UINT32 dwDeviceId,CSUDIFSPartitionType_E eType)
//@DESCRIPTION: ��ʽ��һ�������ڵ��豸�������־��豸�ʹ洢�豸
//@PRECONDITION: ��
//@INPUT: dwDeviceId = 0x0001FFFF(����0x0001FFFFΪ�����ڵ��豸id)
//@INPUT: eType = EM_UDIFS_PARTITION_EXT2
//@EXPECTATION: ��ʽ��ʧ�ܣ�����ֵ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIFSFormat(0x0001FFFF, EM_UDIFS_PARTITION_EXT2)��ʽ���豸������ֵӦ��������CSUDI_SUCCESS
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFormat_0004(void)
{
	CSTK_ASSERT_TRUE(CSUDI_SUCCESS != CSUDIFSFormat(0x0001FFFF, EM_UDIFS_PARTITION_EXT2), "��ʽ�������ڵ��豸��Ӧ�óɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSFormat(CSUDI_UINT32 dwDeviceId,CSUDIFSPartitionType_E eType)
//@DESCRIPTION: ��ʽ��һ������ʹ�õķ־��豸
//@PRECONDITION: 1��ϵͳ�����ٴ���һ����ʶ��ķ־�
//@PRECONDITION: 2��ƽ̨֧�ָ�ʽ����EM_STRG_PARTITION_FAT32
//@INPUT: dwDeviceId = dwVolumeId��dwVolume����CSUDIFSGetAllDeviceId()��õķ־��豸
//@INPUT: eType = EM_UDIFS_PARTITION_FAT32
//@EXPECTATION: ��ʽ��ʧ�ܣ�����ֵ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ����ʶ��ķ־��豸dwVolumeId
//@EXECUTIONFLOW: 2������CSUDIFSOpen(��������·�� + "CSUDIFSFormat_0005", "w")���������ļ������ؾ��hFile�����Ӧ����Ϊ��
//@EXECUTIONFLOW: 3������CSUDIFSFormat(dwVolumeId, EM_UDIFS_PARTITION_FAT32)��ʽ���־��豸������ֵӦ��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 4������CSUDIFSClose(hFile)�رո��ļ�������ֵӦ��Ϊ0
//@EXECUTIONFLOW: 5������CSUDIFSRemove(��������·�� + "CSUDIFSFormat_0005")ɾ�����ļ�������ֵӦ��Ϊ0
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFormat_0005(void)
{
	//�ֶ�����
	CSUDI_UINT32   dwIds[32];
	int nActIdCnt = 0;
	int i = 0;
	CSUDI_UINT32 dwVolumeId = 0;
	CSUDI_HANDLE  hHandle = CSUDI_NULL;
	char szFilePath[CSUDIFS_MAX_FILE_PATH];
	CSUDIFSDeviceInfo_S   sDeviceInfo;
	int  nLen = 0;

	CSFSPrint("������������Բ����豸���и�ʽ����ȷ�ϼ����밴(Y)/�����밴(N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "ѡ�����˳�");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt), "��ȡ�豸idʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "һ���豸Ҳ�����ڣ��޷�����豸��Ϣ\n");
	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			dwVolumeId = dwIds[i];

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo), "��ȡ�豸��Ϣʧ��\n");

			CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "�־���ص�Ϊ��\n");

			CSTK_ASSERT_TRUE_FATAL(sDeviceInfo.m_szMountPoint != CSUDI_NULL, "�޷־���ص�\n");
			
			memset(szFilePath, 0, sizeof(szFilePath));

			nLen = strlen(sDeviceInfo.m_szMountPoint);
			
			memcpy(szFilePath, sDeviceInfo.m_szMountPoint, nLen);

			if(szFilePath[nLen -1] != '/') szFilePath[nLen] = '/';

			strcat(szFilePath, "CSUDIFSFormat_0005");
			
			CSTK_ASSERT_TRUE_FATAL((hHandle = CSUDIFSOpen(szFilePath, "w")) != CSUDI_NULL, "�����ļ�ʧ��\n");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFSFormat(dwVolumeId, EM_UDIFS_PARTITION_FAT32), "��ʽ���־�����ʹ�õķ־�Ӧ���ɹ�\n");
			break;
		}
	}

	CSTK_ASSERT_TRUE_FATAL(dwVolumeId != -1, "�Ҳ���һ���־��豸\n");
	
	CSTK_FATAL_POINT
	{
		if(hHandle != CSUDI_NULL)
		{	
			CSTK_ASSERT_TRUE_FATAL(CSUDIFSClose(hHandle) == 0, "�ر��ļ�ʧ��\n");
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRemove(szFilePath), "ɾ���ļ�ʧ��\n");
		}
	}
	return CSUDI_TRUE;	
}

static  int  g_eCSUDIFSFormat_0006A;
static  int  g_eCSUDIFSFormat_0006B;
void CSUDIFSFormat_0006_Callback(CSUDIRDIEvent_E eType, CSUDI_UINT32 dwDeviceId,const void * pvEventData, void * pvUserData)
{

	if(eType == EM_UDIRDI_EVENT_PLUGOUT)
	{
		g_eCSUDIFSFormat_0006A ++;
	}
	if(eType == EM_UDIRDI_EVENT_FOUND)
	{
		g_eCSUDIFSFormat_0006B++;
	}
}
//@CASEGROUP: CSUDI_Error_Code CSUDIFSFormat(CSUDI_UINT32 dwDeviceId,CSUDIFSPartitionType_E eType)
//@DESCRIPTION: ��ʽ��һ���־��豸10��
//@PRECONDITION: 1��ϵͳ�����ٴ���һ����ʶ��Ĵ洢�豸
//@PRECONDITION: 2��ƽ̨֧�ָ�ʽ����EM_UDIFS_PARTITION_EXT3
//@INPUT: dwDeviceId = dwStorageId��dwStorageId����CSUDIFSGetAllDeviceId()��õĴ洢�豸
//@INPUT: eType = EM_UDIFS_PARTITION_EXT3
//@EXPECTATION: ��ʽ���ɹ���������ע��Ļص����յ�10��EM_UDIRDI_EVENT_PLUGOUT�¼���10��EM_UDIRDI_EVENT_FOUND�¼�
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ����ʶ��ķ־��豸dwVolumeId
//@EXECUTIONFLOW: 2������CSUDIFSRDIAddCallback(CSUDIFSFormat_0006_Callback, CSUDI_NULL)ע��ص�������Ӧ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3����������CSUDIFSFormat(dwVolumeId, EM_UDIFS_PARTITION_EXT3)��ʽ��10�洢�豸��Ӧ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 4�����ص�����CSUDIFSFormat_0002_Callback���Ƿ�10��EM_UDIRDI_EVENT_PLUGOUT�¼���10��EM_UDIRDI_EVENT_FOUND�¼�
//@EXECUTIONFLOW: 5������CSUDIFSRDIRemoveCallback(CSUDIFSFormat_0006_Callback, CSUDI_NULL)ɾ���ص��������ָ�����
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFormat_0006(void)
{
	//�ֶ�����
	CSUDI_UINT32   dwIds[32];
	int nActIdCnt = 0;
	int i =0;
	CSUDI_UINT32  dwVolumeId = 0;
	CSUDI_Error_Code nErrorCode = CSUDI_SUCCESS;
	int   nFormatCount = 10;

	CSFSPrint("������������Բ����豸���и�ʽ����ȷ�ϼ����밴(Y)/�����밴(N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "ѡ�����˳�");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCode = CSUDIFSRDIAddCallback(CSUDIFSFormat_0006_Callback,
		CSUDI_NULL)),"��ӻص�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt), "��ȡ�豸idʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "һ���豸Ҳ�����ڣ��޷�����豸��Ϣ\n");

	g_eCSUDIFSFormat_0006A = 0;
	g_eCSUDIFSFormat_0006B = 0;

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			dwVolumeId = dwIds[i];
			while(nFormatCount-- > 0)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSFormat(dwVolumeId, EM_UDIFS_PARTITION_EXT3), "��ʽ���洢�豸ʧ��\n");
				CSTCPrint("��%d�θ�ʽ�ɹ�\n", 10 - nFormatCount);
			}
			break;
		}
	}

	CSTK_ASSERT_TRUE_FATAL(g_eCSUDIFSFormat_0006A  == 10, "�����γ�����Ϣ��������\n");
	CSTK_ASSERT_TRUE_FATAL(g_eCSUDIFSFormat_0006B  == 10, "���ַ�������Ϣ��������\n");

	CSTK_FATAL_POINT
	{
		if(nErrorCode == CSUDI_SUCCESS)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(CSUDIFSFormat_0006_Callback, 
			CSUDI_NULL), "�Ƴ��ص�ʧ��\n");		
		}
	}
	
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSFormat(CSUDI_UINT32 dwDeviceId,CSUDIFSPartitionType_E eType)
//@DESCRIPTION: ��ʽ��һ���־�Ϊ�Ƿ����ļ�ϵͳ����
//@PRECONDITION: 1��ϵͳ�����ٴ���һ����ʶ��ķ־�
//@INPUT: dwDeviceId = dwVolumeId��dwVolume����CSUDIFSGetAllDeviceId()��õķ־��豸
//@INPUT: eType = 0x7FFFFFFF(����0x7FFFFFFFΪ�Ƿ����ļ�ϵͳ����)
//@EXPECTATION: ��ʽ���ɹ�����ʽ��ǰ���ļ���������
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ����е��豸���������豸���ҵ�һ���־��豸dwVolumeId
//@EXECUTIONFLOW: 2������CSUDIFSOpen(��������·�� + "CSUDIFSFormat_0007", "w")���������ļ������ؾ��hFile�������Ϊ��
//@EXECUTIONFLOW: 3������CSUDIFSClose(hFile)�رո��ļ�������ֵӦ��Ϊ0
//@EXECUTIONFLOW: 4������CSUDIFSFormat(dwVolumeId, 0x7FFFFFFF)��ʽ���־��豸����ʽ��ʧ�ܣ�����ֵӦ��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 5������CSUDIFSOpen(��������·�� + "CSUDIFSFormat_0007", "r")��ֻ����ʽ�򿪸�ʽ��ǰ�������ļ���Ӧ���ɹ�������ֵ������CSUDI_NULL
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFormat_0007(void)
{
	//�ֶ�����
	CSUDI_UINT32   dwIds[32];
	int nActIdCnt = 0;
	int i = 0;
	CSUDI_UINT32 dwVolumeId = 0;
	CSUDI_HANDLE  hHandle = CSUDI_NULL;
	char szFilePath[CSUDIFS_MAX_FILE_PATH];
	CSUDIFSDeviceInfo_S   sDeviceInfo;
	int  nLen = 0;
	CSUDI_BOOL   bNeedRemove = CSUDI_FALSE;

	CSFSPrint("������������Բ����豸���и�ʽ����ȷ�ϼ����밴(Y)/�����밴(N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "ѡ�����˳�");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt), "��ȡ�豸idʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "һ���豸Ҳ�����ڣ��޷�����豸��Ϣ\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			dwVolumeId = dwIds[i];

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo), "��ȡ�豸��Ϣʧ��\n");
			CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "�־���ص�Ϊ��\n");
			CSTK_ASSERT_TRUE_FATAL(sDeviceInfo.m_szMountPoint != CSUDI_NULL, "�޷־���ص�\n");
			
			memset(szFilePath, 0, sizeof(szFilePath));
			nLen = strlen(sDeviceInfo.m_szMountPoint);
			memcpy(szFilePath, sDeviceInfo.m_szMountPoint, nLen);

			if(szFilePath[nLen -1] != '/') szFilePath[nLen] = '/';

			strcat(szFilePath, "CSUDIFSFormat_0007");
			
			CSTK_ASSERT_TRUE_FATAL((hHandle = CSUDIFSOpen(szFilePath, "w")) != CSUDI_NULL, "�����ļ�ʧ��\n");

			bNeedRemove = CSUDI_TRUE;
			
			CSTK_ASSERT_TRUE_FATAL(CSUDIFSClose(hHandle) == 0, "�ر��ļ�ʧ��\n");

			hHandle = CSUDI_NULL;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFSFormat(dwVolumeId, (CSUDIFSPartitionType_E)0x7FFFFFFF), "��ʽ���־�Ӧ�óɹ�\n");

			bNeedRemove = CSUDI_TRUE;
			CSTK_ASSERT_TRUE_FATAL((hHandle = CSUDIFSOpen(szFilePath, "r")) != CSUDI_NULL, "���ļ�ʧ��\n");

			CSTK_ASSERT_TRUE_FATAL(CSUDIFSClose(hHandle) == 0, "�ر��ļ�ʧ��\n");

			hHandle = CSUDI_NULL;
		}
	}

	CSTK_ASSERT_TRUE_FATAL(dwVolumeId != -1, "�Ҳ���һ���־��豸\n");
	
	CSTK_FATAL_POINT
	{
		if(hHandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hHandle), "�ر��ļ�ʧ��\n");
			hHandle = CSUDI_NULL;
		}
		if(bNeedRemove)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSRemove(szFilePath), "ɾ���ļ�ʧ��\n");
			bNeedRemove = CSUDI_FALSE;
		}
	}
	return CSUDI_TRUE;	
}

static  CSUDIRDIEvent_E  g_eCSUDIFSFormat_0008;
void CSUDIFSFormat_0008_Callback(CSUDIRDIEvent_E eType, CSUDI_UINT32 dwDeviceId,const void * pvEventData, void * pvUserData)
{
	if(eType == EM_UDIRDI_EVENT_FOUND)
	{
		g_eCSUDIFSFormat_0008 = EM_UDIRDI_EVENT_FOUND;
	}
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSFormat(CSUDI_UINT32 dwDeviceId,CSUDIFSPartitionType_E eType)
//@DESCRIPTION: ��ʽ��һ����δ��ʽ���Ĵ洢�豸
//@PRECONDITION: 1��ϵͳ�����ٴ���һ����ʶ��Ĵ洢�豸
//@PRECONDITION: 2��ƽ̨֧�ָ�ʽ����EM_UDIFS_PARTITION_FAT32
//@INPUT: dwDeviceId = dwStorageId��dwStorageId����CSUDIFSGetAllDeviceId()��õĴ洢�豸
//@INPUT: eType = EM_UDIFS_PARTITION_EXT3
//@EXPECTATION: ��ʽ���ɹ���������ע��Ļص����յ�EM_UDIRDI_EVENT_FOUND�¼�
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ����ʶ��Ĵ洢�豸dwStorageId
//@EXECUTIONFLOW: 2������CSUDIFSRDIAddCallback(CSUDIFSFormat_0008_Callback, CSUDI_NULL)ע��ص�������Ӧ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIFSFormat(dwStorageId, EM_UDIFS_PARTITION_EXT3)��ʽ���洢�豸��Ӧ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 4�����ص�����CSUDIFSFormat_0008_Callback���Ƿ���EM_UDIRDI_EVENT_FOUND�¼�
//@EXECUTIONFLOW: 5������CSUDIFSRDIRemoveCallback(CSUDIFSFormat_0002_Callback, CSUDI_NULL)ɾ���ص��������ָ�����
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFormat_0008(void)
{
	//�ֶ�����
	CSUDI_UINT32   dwIds[32];
	int nActIdCnt = 0;
	int i =0;
	CSUDI_UINT32  dwStorageId = 0;
	CSUDI_Error_Code nErrorCode = CSUDI_SUCCESS;

	CSFSPrint("������������Բ����豸���и�ʽ����ȷ�ϼ����밴(Y)/�����밴(N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "ѡ�����˳�");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCode = CSUDIFSRDIAddCallback(CSUDIFSFormat_0008_Callback,
		CSUDI_NULL)),"��ӻص�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt), "��ȡ�豸idʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "һ���豸Ҳ�����ڣ��޷�����豸��Ϣ\n");

	g_eCSUDIFSFormat_0008 = EM_UDIRDI_EVENT_UNKNOWN;

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_STORAGE_MAJOR_DEVICEID)
		{
			dwStorageId = dwIds[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSFormat(dwStorageId, EM_UDIFS_PARTITION_FAT32), "��ʽ���洢�豸ʧ��\n");
			break;
		}
	}

	CSTK_ASSERT_TRUE_FATAL(g_eCSUDIFSFormat_0008 == EM_UDIRDI_EVENT_FOUND, "�ص�������û���յ���ʽ���¼�\n");

	CSTK_FATAL_POINT
	{
		if(nErrorCode == CSUDI_SUCCESS)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(CSUDIFSFormat_0008_Callback, 
			CSUDI_NULL), "�Ƴ��ص�ʧ��\n");		
		}
	}
	
	return CSUDI_TRUE;	
}
/****************************CSUDIFSFormat  End *******************************/


/****************************CSUDIFSGetCodePage  Start ******************************/ 
//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetCodePage(int * pnCodePage)
//@DESCRIPTION: ������ȡ���뷽ʽ
//@PRECONDITION: ��
//@INPUT: pnCodePage = &nCodePage������int nCodePage
//@EXPECTATION: ����ֵ����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIFSGetCodePage(&nCodePage)
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetCodePage_0001(void)
{
	int  nCodePage;
	
	CSTK_ASSERT_TRUE(CSUDI_SUCCESS == CSUDIFSGetCodePage(&nCodePage), "��ñ�����ʧ��\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetCodePage(int * pnCodePage)
//@DESCRIPTION: ���ձ��뷽ʽ��ָ��Ƿ�
//@PRECONDITION: ��
//@INPUT: pnCodePage = CSUDI_NULL
//@EXPECTATION: ����ֵ������CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIFSGetCodePage(CSUDI_NULL)
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetCodePage_0002(void)
{
	CSTK_ASSERT_TRUE(CSUDI_SUCCESS != CSUDIFSGetCodePage(CSUDI_NULL), "��ñ����벻Ӧ�óɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}
/****************************CSUDIFSGetCodePage  End *******************************/

/****************************CSUDIFSOpen  Start ***********************************/ 
//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpen(const char * pcFileName, const char * pcMode)
//@DESCRIPTION: ����һ��ֻд�����ļ�
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcFileName = ��������·�� + "CSUDIFSOpen_0001"
//@INPUT: pcMode = "w"
//@EXPECTATION: ����ֵ������CSUDI_NULL�����ļ�ֻ��д
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���Ŀ¼	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSOpen_0001", "w")����һ��ֻд�����ļ�
//@EXECUTIONFLOW: 4������CSUDIFSWrite()�����ļ�д����
//@EXECUTIONFLOW: 5������CSUDIFSRead()�����ļ��е�����
//@EXECUTIONFLOW: 6������CSUDIFSClose()�رմ������ļ�
//@EXECUTIONFLOW: 7������CSUDIFSRemove(��������·�� + "CSUDIFSOpen_0001")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpen_0001(void)
{
	CSUDI_HANDLE  hFile = CSUDI_NULL;
	char  szBuf[10];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�־��豸���ص�ʧ��\n");
	strcat(g_szVolumeMountPoint, "CSUDIFSOpen_0001");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(g_szVolumeMountPoint, "w")), "����ֻд�ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(strlen("hello") == CSUDIFSWrite(hFile, "hello", strlen("hello")), "���ļ�д����ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSRead(hFile, szBuf, sizeof(szBuf)) < 0, "��ֻд�ļ�Ӧ��ʧ��\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");
			CSTK_ASSERT_TRUE_FATAL(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");		
		}
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpen(const char * pcFileName, const char * pcMode)
//@DESCRIPTION: ��ֻ����ʽ��һ���Ѵ��ڵ��ļ�
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcFileName = ��������·�� + "CSUDIFSOpen_0002"
//@INPUT: pcMode = "r"
//@EXPECTATION: ����ֵ��ΪCSUDI_NULL�� ���ļ�ֻ�ɶ�����д
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���Ŀ¼	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSOpen_0002", "w")�����ļ�
//@EXECUTIONFLOW: 4������CSUDIFSWrite()�����ļ�д����
//@EXECUTIONFLOW: 5������CSUDIFSClose()�رմ������ļ�
//@EXECUTIONFLOW: 6������CSUDIFSOpen(��������·�� + "CSUDIFSOpen_0002", "r")��ֻ����ʽ���ļ�
//@EXECUTIONFLOW: 7������CSUDIFSRead()�����ļ��е�����
//@EXECUTIONFLOW: 8������CSUDIFSWrite()дֻ���ļ�
//@EXECUTIONFLOW: 9������CSUDIFSClose()�ر��ļ�
//@EXECUTIONFLOW: 10������CSUDIFSRemove(��������·�� + "CSUDIFSOpen_0002")
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpen_0002(void)
{
	CSUDI_HANDLE  hFile = CSUDI_NULL;
	char  szBuf[10];
	CSUDI_BOOL   bNeedRemove = CSUDI_FALSE;

	//����ļ�ȫ·��
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�־��豸���ص�ʧ��\n");
	strcat(g_szVolumeMountPoint, "CSUDIFSOpen_0002");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(g_szVolumeMountPoint, "w")), "����ֻд�ļ�ʧ��\n");

	bNeedRemove = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(strlen("CSUDIFSOpen_0002") == CSUDIFSWrite(hFile, "CSUDIFSOpen_0002", strlen("CSUDIFSOpen_0002")), "���ļ�д����ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");

	hFile = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(g_szVolumeMountPoint, "r")), "��ֻ����ʽ���ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSRead(hFile, szBuf, sizeof(szBuf)) > 0, "���ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSWrite(hFile, "CSUDIFSOpen_0002", strlen("CSUDIFSOpen_0002")) < 0, "дֻ���ļ�Ӧ��ʧ��\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");
		}

		if(bNeedRemove)
		{
			CSTK_ASSERT_TRUE_FATAL(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");
		}		
	}
	
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpen(const char * pcFileName, const char * pcMode)
//@DESCRIPTION: �Կɶ���д��ʽ����һ�����ļ�
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcFileName = ��������·�� + "CSUDIFSOpen_0003"
//@INPUT: pcMode = "w+"
//@EXPECTATION: ����ֵ��ΪCSUDI_NULL�� ���ļ��ɶ���д
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���Ŀ¼	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSOpen_0003", "w+")�����ļ�
//@EXECUTIONFLOW: 4������CSUDIFSWrite()�����ļ�д����
//@EXECUTIONFLOW: 5������CSUDIFSRead()�����ļ��е�����
//@EXECUTIONFLOW: 6������CSUDIFSClose()�ر��ļ�
//@EXECUTIONFLOW: 7������CSUDIFSRemove(��������·�� + "CSUDIFSOpen_0003")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpen_0003(void)
{
	CSUDI_HANDLE  hFile = CSUDI_NULL;
	char  szBuf[100];
	CSUDI_BOOL   bNeedRemove = CSUDI_FALSE;

	//����ļ�ȫ·��
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�־��豸���ص�ʧ��\n");
	strcat(g_szVolumeMountPoint, "CSUDIFSOpen_0003");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "�����ɶ���д�ļ�ʧ��\n");

	bNeedRemove = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(strlen("CSUDIFSOpen_0003") == CSUDIFSWrite(hFile, "CSUDIFSOpen_0003", strlen("CSUDIFSOpen_0003")), "���ļ�д����ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile, 0, CSUDIFS_SEEK_SET), "�����ļ�ָ��ʧ��\n");

	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(strlen("CSUDIFSOpen_0003") == CSUDIFSRead(hFile, szBuf, strlen("CSUDIFSOpen_0003")), "���ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(szBuf, "CSUDIFSOpen_0003"), "������������д��Ĳ�һ��\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");
			CSTK_ASSERT_TRUE_FATAL(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");
		}	
	}
	
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpen(const char * pcFileName, const char * pcMode)
//@DESCRIPTION: �Կɶ���д��ʽ��һ���Ѵ��ڵ��ļ�
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcFileName = ��������·�� + "CSUDIFSOpen_0004"
//@INPUT: pcMode = "r+"
//@EXPECTATION: ����ֵ��ΪCSUDI_NULL�� ���ļ��ɶ���д
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���Ŀ¼	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSOpen_0004", "w")�����ļ�
//@EXECUTIONFLOW: 4������CSUDIFSClose()�رմ������ļ�
//@EXECUTIONFLOW: 5������CSUDIFSOpen(��������·�� + "CSUDIFSOpen_0004", "r+")�Կɶ���д��ʽ���ļ�
//@EXECUTIONFLOW: 6������CSUDIFSWrite()д�ļ�
//@EXECUTIONFLOW: 7������CSUDIFSRead()�����ļ��е����ݣ��ж��Ƿ���д���һ��
//@EXECUTIONFLOW: 8������CSUDIFSClose()�ر��ļ�
//@EXECUTIONFLOW: 9������CSUDIFSRemove(��������·�� + "CSUDIFSOpen_0004")
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpen_0004(void)
{
	CSUDI_HANDLE  hFile = CSUDI_NULL;
	char  szBuf[100];
	CSUDI_BOOL   bNeedRemove = CSUDI_FALSE;
	char   szTest[] = "CSUDIFSOpen_0004";
	int  nLen = strlen(szTest);
	
	//����ļ�ȫ·��
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�־��豸���ص�ʧ��\n");
	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(g_szVolumeMountPoint, "w")), "����ֻд�ļ�ʧ��\n");

	bNeedRemove = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");

	hFile = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(g_szVolumeMountPoint, "r+")), "�Կɶ���д��ʽ���ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "д���ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile, 0, CSUDIFS_SEEK_SET), "�����ļ�ָ��ʧ��\n");

	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSRead(hFile, szBuf, nLen) > 0, "���ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(szBuf, szTest), "��������д��Ĳ�һ��\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");
		}

		if(bNeedRemove)
		{
			CSTK_ASSERT_TRUE_FATAL(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");
		}		
	}
	
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpen(const char * pcFileName, const char * pcMode)
//@DESCRIPTION: ��׷�ӷ�ʽ����һ��ֻд�ļ�
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcFileName = ��������·�� + "CSUDIFSOpen_0005"
//@INPUT: pcMode = "a"
//@EXPECTATION: ����ֵ��ΪCSUDI_NULL�� ���ļ�ֻ��д���ɶ�
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSOpen_0005", "a")�����ļ�
//@EXECUTIONFLOW: 4������CSUDIFSWrite()д�ļ�
//@EXECUTIONFLOW: 5������CSUDIFSRead()��ȡ�ļ���Ӧ����ȡʧ��
//@EXECUTIONFLOW: 6������CSUDIFSClose()�ر��ļ�
//@EXECUTIONFLOW: 7������CSUDIFSRemove(��������·�� + "CSUDIFSOpen_0005")�Ƴ��ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpen_0005(void)
{
	CSUDI_HANDLE  hFile = CSUDI_NULL;
	char  szBuf[100];
	CSUDI_BOOL   bNeedRemove = CSUDI_FALSE;
	char   szTest[] = "CSUDIFSOpen_0005";
	int  nLen = strlen(szTest);
	
	//����ļ�ȫ·��
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�־��豸���ص�ʧ��\n");
	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(g_szVolumeMountPoint, "a")), "����һ��ֻд�ļ�ʧ��\n");

	bNeedRemove = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "д���ļ�ʧ��\n");

	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSRead(hFile, szBuf, nLen) < 0, "��ȡ׷��ֻ��д�ļ�Ӧ��ʧ��\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");
		}

		if(bNeedRemove)
		{
			CSTK_ASSERT_TRUE_FATAL(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");
		}		
	}
	
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpen(const char * pcFileName, const char * pcMode)
//@DESCRIPTION: ��׷�ӷ�ʽ����һ���ɶ���д�ļ�
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcFileName = ��������·�� + "CSUDIFSOpen_0006"
//@INPUT: pcMode = "a+"
//@EXPECTATION: ����ֵ��ΪCSUDI_NULL�� ���ļ���д�ɶ�
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSOpen_0006", "a+")����һ�ɶ���д�ļ�
//@EXECUTIONFLOW: 4������CSUDIFSWrite()д�ļ�
//@EXECUTIONFLOW: 5������CSUDIFSSeek()�ƶ��ļ�ָ�뵽�ļ�ͷ
//@EXECUTIONFLOW: 6������CSUDIFSRead()��ȡ�ļ�����������Ӧ����д��ʱһ��
//@EXECUTIONFLOW: 7������CSUDIFSClose()�ر��ļ�
//@EXECUTIONFLOW: 8������CSUDIFSRemove(��������·�� + "CSUDIFSOpen_0006")�Ƴ��ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpen_0006(void)
{
	CSUDI_HANDLE  hFile = CSUDI_NULL;
	char  szBuf[100];
	CSUDI_BOOL   bNeedRemove = CSUDI_FALSE;
	char   szTest[] = "CSUDIFSOpen_0006";
	int  nLen = strlen(szTest);

	//����ļ�ȫ·��
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�־��豸���ص�ʧ��\n");
	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(g_szVolumeMountPoint, "a+")), "��׷�ӷ�ʽ����һ���ɶ���д�ļ�ʧ��\n");

	bNeedRemove = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "д���ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile, 0, CSUDIFS_SEEK_SET), "�ı��ļ�ָ�뵽ʧ��\n");

	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSRead(hFile, szBuf, nLen) == nLen, "��ȡ�ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(szTest, szBuf), "��ȡ��������д��ʱ��һ��\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");
		}

		if(bNeedRemove)
		{
			CSTK_ASSERT_TRUE_FATAL(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");
		}		
	}
	
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpen(const char * pcFileName, const char * pcMode)
//@DESCRIPTION: ��׷�ӷ�ʽ����һ���Ѿ����ڵ��ļ�
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcFileName = ��������·�� + "CSUDIFSOpen_0007"
//@INPUT: pcMode = "a+"
//@EXPECTATION: ����ֵ��ΪCSUDI_NULL�� ��ԭ�ļ����ݱ����
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSOpen_0007", "w")����һ��д�ļ�
//@EXECUTIONFLOW: 4������CSUDIFSWrite()д�ļ�
//@EXECUTIONFLOW: 5������CSUDIFSClose()�ر��ļ�
//@EXECUTIONFLOW: 6������CSUDIFSOpen(��������·�� + "CSUDIFSOpen_0007", "a+")���ļ�
//@EXECUTIONFLOW: 7������CSUDIFSRead()��ȡ�ļ�����������Ӧ��Ϊ0
//@EXECUTIONFLOW: 8������CSUDIFSClose()�ر��ļ�
//@EXECUTIONFLOW: 9������CSUDIFSRemove(��������·�� + "CSUDIFSOpen_0007")�Ƴ��ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpen_0007(void)
{
	CSUDI_HANDLE  hFile = CSUDI_NULL;
	char  szBuf[100];
	CSUDI_BOOL   bNeedRemove = CSUDI_FALSE;
	char   szTest[] = "CSUDIFSOpen_0007";
	int  nLen = strlen(szTest);
	
	//����ļ�ȫ·��
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�־��豸���ص�ʧ��\n");
	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(g_szVolumeMountPoint, "w")), "�����ļ�ʧ��\n");

	bNeedRemove = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "д���ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");

	hFile = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(g_szVolumeMountPoint, "a+")), "��׷�ӷ�ʽ���ļ�ʧ��\n");

	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSRead(hFile, szBuf, nLen) != 0, "��ȡ����Ӧ��Ϊ0\n");

	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");
		}

		if(bNeedRemove)
		{
			CSTK_ASSERT_TRUE_FATAL(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");
		}		
	}
	
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpen(const char * pcFileName, const char * pcMode)
//@DESCRIPTION: ��һ���ļ���
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcFileName = ��������·�� + "CSUDIFSOpen_0008"
//@INPUT: pcMode = "r"
//@EXPECTATION: ����ֵ����CSUDI_NULL
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��
//@EXECUTIONFLOW: 3������CSUDIFSMkdir(��������·�� + "CSUDIFSOpen_0008",�����ļ���
//@EXECUTIONFLOW: 4������CSUDIFSOpen(��������·�� + "CSUDIFSOpen_0008", "r")��Ӧ��ʧ��
//@EXECUTIONFLOW: 5������CSUDIFSRmdir(��������·�� + "CSUDIFSOpen_0008"),ɾ���ļ���
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpen_0008(void)
{
	char   szTest[] = "CSUDIFSOpen_0008";
	int   nMkDirRet = -1; 
	
	//����ļ�ȫ·��
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�־��豸���ص�ʧ��\n");
	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(0 == (nMkDirRet = CSUDIFSMkdir(g_szVolumeMountPoint, 0)), "�����ļ���ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == CSUDIFSOpen(g_szVolumeMountPoint, "r"), "���ļ���Ӧ��ʧ��\n");
	
	CSTK_FATAL_POINT
	{
		if(nMkDirRet == 0)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(g_szVolumeMountPoint), "ɾ���ļ���ʧ��\n");
		}
	}	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpen(const char * pcFileName, const char * pcMode)
//@DESCRIPTION: �򿪲����ڵ��ļ�
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcFileName = ��������·�� + "CSUDIFSOpen_0009"
//@INPUT: pcMode = "r"
//@EXPECTATION: ����ֵ����CSUDI_NULL
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSOpen_0009", "r")��Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpen_0009(void)
{
	char   szTest[] = "CSUDIFSOpen_0009";
	
	//����ļ�ȫ·��
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�־��豸���ص�ʧ��\n");
	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == CSUDIFSOpen(g_szVolumeMountPoint, "r"), "�򿪲����ڵ��ļ�Ӧ��ʧ��\n");

	CSTK_FATAL_POINT
	{
		;
	}	
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpen(const char * pcFileName, const char * pcMode)
//@DESCRIPTION: �����������ֵ��ļ�
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcFileName = ��������·�� + "CSUDIFSOpen_0010"
//@INPUT: pcMode = "w"
//@EXPECTATION: ����ֵ����CSUDI_NULL
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSOpen_0010_����", "w")���������ļ���Ӧ���ɹ�
//@EXECUTIONFLOW: 4������CSUDIFSClose()�ر��ļ�
//@EXECUTIONFLOW: 5������CSUDIFSRemove(��������·�� + "CSUDIFSOpen_0010_����")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpen_0010(void)
{
	char   szTest[] = "CSUDIFSOpen_0010_����";
	CSUDI_HANDLE     hFile = CSUDI_NULL;
	
	//����ļ�ȫ·��
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�־��豸���ص�ʧ��\n");
	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(g_szVolumeMountPoint, "w")), "�����������ļ�ʧ��\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");
			CSTK_ASSERT_TRUE_FATAL(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");
		}
	}	
	return CSUDI_TRUE;	
}


//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpen(const char * pcFileName, const char * pcMode)
//@DESCRIPTION: ���������ļ�
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcFileName = CSUDI_NULL
//@INPUT: pcMode = "w"
//@EXPECTATION: ����ֵ����CSUDI_NULL	
//@EXECUTIONFLOW: 1������CSUDIFSOpen(CSUDI_NULL, "w")�����ļ���Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpen_0011(void)
{
	CSTK_ASSERT_TRUE(CSUDI_NULL == CSUDIFSOpen(CSUDI_NULL, "w"), "�򿪿�ָ���ļ�Ӧ��ʧ��\n");

	CSTK_FATAL_POINT
	{
		;	
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpen(const char * pcFileName, const char * pcMode)
//@DESCRIPTION: ����һ���ļ���������󳤶ȵ��ļ�
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcFileName = ��������·�� + "CSUDIFSOpen_0012"
//@INPUT: pcMode = "w"
//@EXPECTATION: ����ʧ�ܣ�����ֵ����CSUDI_NULL
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSOpen_0012", "w")�����ļ���Ӧ������CSUDI_NULL
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpen_0012(void)
{
	char   szTest[] = "CSUDIFSOpen_0012";
	CSUDI_HANDLE     hFile = CSUDI_NULL;
	char   szFilePath[1024];
	int     nLen = 0;
	int     nCount;
	nLen = strlen(szTest);
	nCount = 640/nLen;
	
	//����ļ�ȫ·��
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�־��豸���ص�ʧ��\n");
	memset(szFilePath, 0, 1024);
	strcpy(szFilePath, g_szVolumeMountPoint);
	while(nCount -- > 0)
	{
		strcat(szFilePath, szTest);
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == (hFile = CSUDIFSOpen(szFilePath, "w")), "�ļ���̫���������ļ�ʧ��\n");
	
	CSTK_FATAL_POINT
	{
		;
	}	
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpen(const char * pcFileName, const char * pcMode)
//@DESCRIPTION: �ڴ�һ���ɶ���д(��ֻд)�ļ�ʱ���ļ�Ӧ�������
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcFileName = ��������·�� + "CSUDIFSOpen_0013"
//@INPUT: pcMode = "w" ����pcMode = "w+"
//@EXPECTATION: �ļ���������
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���Ŀ¼	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSOpen_0013", "w+")�����ļ�
//@EXECUTIONFLOW: 4������CSUDIFSWrite()�����ļ�д����"CSUDIFSOpen_0013"
//@EXECUTIONFLOW: 5������CSUDIFSSeek()�����ļ�ָ��
//@EXECUTIONFLOW: 6������CSUDIFSRead()�����ļ��е����ݣ�������ȡ��"CSUDIFSOpen_0013", ���ҷ��س���Ϊstrlen("CSUDIFSOpen_0013")
//@EXECUTIONFLOW: 7������CSUDIFSClose()�ر��ļ�
//@EXECUTIONFLOW: 8������CSUDIFSOpen()(��������·�� + "CSUDIFSOpen_0013", "w")�����ļ�
//@EXECUTIONFLOW: 9������CSUDIFSWrite()�����ļ�д����"CSUDIFSWrite"
//@EXECUTIONFLOW: 10������CSUDIFSClose()�ر��ļ�
//@EXECUTIONFLOW: 11������CSUDIFSOpen()(��������·�� + "CSUDIFSOpen_0013", "r")�����ļ�
//@EXECUTIONFLOW: 12������CSUDIFSRead()�����ļ��е����ݣ�������ȡ������"CSUDIFSWrite"�����ҳ���Ϊstrlen("CSUDIFSWrite")
//@EXECUTIONFLOW: 13������CSUDIFSClose()�ر��ļ�
//@EXECUTIONFLOW: 14���ظ�����3-����13��ͬʱ������8��CSUDIFSOpenģʽ��Ϊw+
//@EXECUTIONFLOW: 15������CSUDIFSClose()�ر��ļ�
//@EXECUTIONFLOW: 16������CSUDIFSRemove()�Ƴ���������·��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpen_0013(void)
{
	CSUDI_HANDLE  hFile = CSUDI_NULL;
	char acBufOpen[23] = "CSTC_FS_IT_CSUDIFSOpen";
	char acBufWrite[13] = "CSUDIFSWrite";   
	char  szBuf[100];
	CSUDI_BOOL bModeFlag = CSUDI_FALSE;
	int i = 0;
	//����ļ�ȫ·��
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "����1����ȡ�־��豸���ص�ʧ��\n");
	strcat(g_szVolumeMountPoint, acBufOpen);

	while( i++ < 2 )
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "����3�������ɶ���д�ļ�ʧ��\n");
		
		CSTK_ASSERT_TRUE_FATAL(strlen(acBufOpen) == CSUDIFSWrite(hFile, acBufOpen, strlen(acBufOpen)), "����4�����ļ�д����ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile, 0, CSUDIFS_SEEK_SET), "����5�������ļ�ָ��ʧ��\n");
		
		memset(szBuf, 0, sizeof(szBuf));
		CSTK_ASSERT_TRUE_FATAL(strlen(acBufOpen) == CSUDIFSRead(hFile, szBuf, strlen(acBufOpen)), "����6�����ļ�ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(0 == strcmp(szBuf, acBufOpen), "����6��������������д��Ĳ�һ��\n");
		
		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "����7���ر��ļ�ʧ��\n");
	
		if( CSUDI_FALSE == bModeFlag)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(g_szVolumeMountPoint, "w")), "����8�������ɶ���д�ļ�ʧ��\n");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "����14�������ɶ���д�ļ�ʧ��\n");
		}
		bModeFlag = CSUDI_TRUE;
		
		CSTK_ASSERT_TRUE_FATAL(strlen(acBufWrite) == CSUDIFSWrite(hFile, acBufWrite, strlen(acBufWrite)), "����9�����ļ�д����ʧ��\n");

		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "����10���ر��ļ�ʧ��\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(g_szVolumeMountPoint, "r")), "����11�������ɶ���д�ļ�ʧ��\n");

		memset(szBuf, 0, sizeof(szBuf));
		CSTK_ASSERT_TRUE_FATAL(strlen(acBufWrite) == CSUDIFSRead(hFile, szBuf, strlen(acBufOpen)), "����12�����ļ�ʧ��\n");
		
		CSTCPrint("д���������:CSUDIFSWrite��������������:%s \n\n", szBuf);

		CSTK_ASSERT_TRUE_FATAL(0 == strcmp(szBuf, acBufWrite), "����12��������������д��Ĳ�һ��\n");
		
		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "����13���ر��ļ�ʧ��\n");

		hFile = CSUDI_NULL;
	}

	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "����14���ر��ļ�ʧ��\n");
		}
		
		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRemove(g_szVolumeMountPoint), "����15���Ƴ��ļ�ʧ��\n");
	}
	
	return CSUDI_TRUE;	
}

/****************************CSUDIFSOpen  End *******************************/

/****************************CSUDIFSRead  Start *****************************/ 
//@CASEGROUP: int CSUDIFSRead(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount)
//@DESCRIPTION: ��ȡ���ݲ������ļ���С
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = hFile1, hFile1ΪCSUDIFSOpen()���صĺϷ����
//@INPUT: pcBuf = szBuf, ����char szBuf[5]
//@INPUT: uCount = sizeof(szBuf) - 1
//@EXPECTATION: ����ֵ����sizeof(szBuf) - 1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSRead_0001", "w+")���������ļ����õ����hFile1
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile1, "CSUDIFSRead_0001", strlen("CSUDIFSRead_0001"))д������
//@EXECUTIONFLOW: 5������CSUDIFSSeek(hFile1, 0, CSFS_SEEK_SET)�����ļ�ָ��
//@EXECUTIONFLOW: 6������CSUDIFSRead(hFile1, szBuf, sizeof(szBuf) - 1)��ȡС���ļ������ݣ�Ӧ��Ϊд��ʱ���ִ�
//@EXECUTIONFLOW: 7������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 8������CSUDIFSRemove(��������·�� + "CSUDIFSRead_0001")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRead_0001(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSRead_0001";
	int     nLen = strlen(szTest);
	char   szBuf[30]="aaaaaaaaaaaaaaaaaaaaaa";
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "���������ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "д������ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile1, 0, CSUDIFS_SEEK_SET), "�����ļ�ʧ��\n");
	
	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(16 == CSUDIFSRead(hFile1, szBuf, 20), "��ȡ����ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != strstr(szTest, szBuf), "�������ݲ���д��ʱ���ִ�\n");

	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");
		}
	}

	return  CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSRead(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount)
//@DESCRIPTION: ��ȡ���ݴ����ļ���С
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = hFile1, hFile1ΪCSUDIFSOpen()���صĺϷ����
//@INPUT: pcBuf = szBuf, ����char szBuf[100]
//@INPUT: uCount = sizeof(szBuf)- 1
//@EXPECTATION: = nLen, nlen Ϊ�ļ��ܳ�
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSRead_0002", "w+")���������ļ�
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile1, "CSUDIFSRead_0002", strlen("CSUDIFSRead_0002"))д������
//@EXECUTIONFLOW: 5������CSUDIFSSeek(hFile1, 0, CSFS_SEEK_SET)�����ļ�ָ��
//@EXECUTIONFLOW: 6������CSUDIFSRead(hFile1, szBuf, sizeof(szBuf)-1)��ȡ�ļ�����������Ӧ����д��ʱһ��
//@EXECUTIONFLOW: 7������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 8������CSUDIFSRemove(��������·�� + "CSUDIFSRead_0002")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRead_0002(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSRead_0002";
	int     nLen = strlen(szTest);
	char   szBuf[100];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "���������ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "д������ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile1, 0, CSUDIFS_SEEK_SET), "�����ļ�ʧ��\n");
	
	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSRead(hFile1, szBuf, sizeof(szBuf) -1), "��ȡ����ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(szTest, szBuf), "����������д��ʱ��һ��\n");

	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");
		}
	}

	return  CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSRead(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount)
//@DESCRIPTION: ��ȡ���ֽ�����
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = hFile1, hFile1ΪCSUDIFSOpen()���صĺϷ����
//@INPUT: pcBuf = szBuf, ����char szBuf[100]
//@INPUT: uCount = 0
//@EXPECTATION: ����ֵ����0
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSRead_0003", "w+")���������ļ�
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile1, "CSUDIFSRead_0003", strlen("CSUDIFSRead_0003"))д������
//@EXECUTIONFLOW: 5������CSUDIFSSeek(hFile1, 0, CSFS_SEEK_SET)�����ļ�ָ��
//@EXECUTIONFLOW: 6������CSUDIFSRead(hFile1, szBuf, 0)��ȡ���ݣ�Ӧ��Ϊ0
//@EXECUTIONFLOW: 7������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 8������CSUDIFSRemove(��������·�� + "CSUDIFSRead_0003")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRead_0003(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSRead_0003";
	int     nLen = strlen(szTest);
	char   szBuf[100];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "���������ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "д������ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile1, 0, CSUDIFS_SEEK_SET), "�����ļ�ʧ��\n");
	
	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRead(hFile1, szBuf, 0), "��ȡ����ʧ��\n");

	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");
		}
	}

	return  CSUDI_TRUE;		
}

//@CASEGROUP: int CSUDIFSRead(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount)
//@DESCRIPTION: ��δд���ݵ��ļ��ж�ȡ����
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = hFile1, hFile1ΪCSUDIFSOpen()���صĺϷ����
//@INPUT: pcBuf = szBuf, ����char szBuf[100]
//@INPUT: uCount = sizeof(szBuf) -1
//@EXPECTATION: ����ֵ����0
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSRead_0004", "w+")�����ļ�����þ��hFile1
//@EXECUTIONFLOW: 4������CSUDIFSSeek(hFile1, 0, CSFS_SEEK_SET)�����ļ�ָ��
//@EXECUTIONFLOW: 5������CSUDIFSRead(hFile1, szBuf, sizeof(szBuf)-1)��ȡ����
//@EXECUTIONFLOW: 6������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 7������CSUDIFSRemove(��������·�� + "CSUDIFSRead_0004")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRead_0004(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSRead_0004";
	char   szBuf[100];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "���������ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile1, 0, CSUDIFS_SEEK_SET), "�����ļ�ʧ��\n");
	
	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRead(hFile1, szBuf, sizeof(szBuf)-1), "��ȡ����ʧ��\n");

	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");
		}
	}

	return  CSUDI_TRUE;	
}


//@CASEGROUP: int CSUDIFSRead(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount)
//@DESCRIPTION: ʹ�ÿ��ļ����������
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = CSUDI_NULL
//@INPUT: pcBuf = szBuf, ����char szBuf[100];
//@INPUT: uCount = sizeof(szBuf) -1
//@EXPECTATION: ����ֵС��0
//@EXECUTIONFLOW: 1������CSUDIFSRead(CSUDI_NULL, szBuf, sizeof(szBuf)-1)��ȡ����
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRead_0005(void)
{
	char szBuf[100];

	CSTK_ASSERT_TRUE(0 > CSUDIFSRead(CSUDI_NULL, szBuf, sizeof(szBuf) -1), "��ȡ���ݲ�Ӧ�óɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
 }

///@CASEGROUP: int CSUDIFSRead(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount)
//@DESCRIPTION: ʹ�ÿ�ָ���������������
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = hFile1, hFile1ΪCSUDIFSOpen()���صĺϷ����
//@INPUT: pcBuf = CSUDI_NULL
//@INPUT: uCount = 0
//@EXPECTATION: ����ֵС��0
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSRead_0006", "w+")�����ļ�����þ��hFile1
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile1, "CSUDIFSRead_0006", strlen("CSUDIFSRead_0006"))д������
//@EXECUTIONFLOW: 5������CSUDIFSSeek(hFile1, 0, CSFS_SEEK_SET)�����ļ�ָ��
//@EXECUTIONFLOW: 6������CSUDIFSRead(hFile1, NULL, 0)��ȡ���ݣ�Ӧ��ʧ��
//@EXECUTIONFLOW: 7������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 8������CSUDIFSRemove(��������·�� + "CSUDIFSRead_0006")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRead_0006(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSRead_0006";
	int     nLen = strlen(szTest);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "���������ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "д������ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile1, 0, CSUDIFS_SEEK_SET), "�����ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 > CSUDIFSRead(hFile1, CSUDI_NULL, 0), "��ȡ���ݲ�Ӧ�óɹ�\n");

	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");
		}
	}

	return  CSUDI_TRUE;			
}

//@CASEGROUP: int CSUDIFSRead(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount)
//@DESCRIPTION: ʹ�ò����ڵ��ļ����������
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = 0x7FFFFFFF(����0x7FFFFFFFΪ�����ڵ��ļ����)
//@INPUT: pcBuf = szBuf������char szBuf[100]
//@INPUT: uCount = sizeof(szBuf)-1
//@EXPECTATION: ����ֵС��0
//@EXECUTIONFLOW: 1������CSUDIFSRead(0x7FFFFFFF, szBuf, sizeof(szBuf)-1)��ȡ�ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRead_0007(void)
{
	char szBuf[100];

	CSTK_ASSERT_TRUE(0 > CSUDIFSRead((CSUDI_HANDLE)0x7FFFFFFF, szBuf,sizeof(szBuf)-1), "��ȡ�����ڵ��ļ���Ӧ�óɹ��ɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}
/****************************CSUDIFSRead  End *******************************/

/****************************CSUDIFSWrite  Start *****************************/ 
//@CASEGROUP: int CSUDIFSWrite(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount)
//@DESCRIPTION: д��������ֽ���������
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = hFile1������hFile1ΪCSUDIFSOpen()���صĺϷ����
//@INPUT: pcBuf = "CSUDIFSWrite_0001"
//@INPUT: uCount = strlen("CSUDIFSWrite_0001")
//@EXPECTATION: д��ɹ��������ļ��е�������д��ʱһ��
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "strlen("CSUDIFSWrite_0001")", "w+")�����ļ�����þ��hFile1
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile1, "CSUDIFSWrite_0001", strlen("CSUDIFSWrite_0001"))д������
//@EXECUTIONFLOW: 5������CSUDIFSSeek(hFile1, 0, CSFS_SEEK_SET)�����ļ�ָ��
//@EXECUTIONFLOW: 5������CSUDIFSRead(hFile1, szBuf, sizeof(szBuf)-1)��ȡ�ļ���Ӧ����д��ʱһ��
//@EXECUTIONFLOW: 5������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 6������CSUDIFSRemove(��������·�� + "CSUDIFSWrite_0001")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSWrite_0001(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSWrite_0001";
	int     nLen = strlen(szTest);
	char   szBuf[100];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "���������ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "д������ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile1, 0, CSUDIFS_SEEK_SET), "�����ļ�ʧ��\n");
	
	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(0 < CSUDIFSRead(hFile1, szBuf, sizeof(szBuf) -1), "��ȡ����ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(szTest, szBuf), "�������ݲ���д��ʱ���ִ�\n");

	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");
		}
	}

	return  CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSWrite(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount)
//@DESCRIPTION: д�����ֽ�
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = hFile1������hFile1ΪCSUDIFSOpen()���صĺϷ����
//@INPUT: pcBuf = "CSUDIFSWrite_0002"
//@INPUT: uCount = 0
//@EXPECTATION: ����ֵ����0
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSWrite_0002", "w+")���������ļ�����þ��hFile1
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile1, "CSUDIFSWrite_0002", 0)д��0�ֽ�
//@EXECUTIONFLOW: 5������CSUDIFSRead(hFile1, szBuf, sizeof(szBuf)-1)��ȡ���ݣ�����ֵӦ��Ϊ0
//@EXECUTIONFLOW: 6������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 7������CSUDIFSRemove(��������·�� + "CSUDIFSWrite_0002")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSWrite_0002(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSWrite_0001";
	char   szBuf[100];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "���������ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSWrite(hFile1, szTest, 0), "д������Ӧ��Ϊ0\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile1, 0, CSUDIFS_SEEK_SET), "�����ļ�ʧ��\n");
	
	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRead(hFile1, szBuf, sizeof(szBuf) -1), "��������Ӧ��Ϊ0\n");

	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");
		}
	}

	return  CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSWrite(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount)
//@DESCRIPTION: ʹ�ÿ��ļ����дд����
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = CSUDI_NULL
//@INPUT: pcBuf = "CSUDIFSWrite_0003"
//@INPUT: uCount = strlen("CSUDIFSWrite_0003")
//@EXPECTATION: ����ֵС��0
//@EXECUTIONFLOW: 1������CSUDIFSWrite(CSUDI_NULL, "CSUDIFSWrite_0003", strlen("CSUDIFSWrite_0003"))д�����ݣ�Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSWrite_0003(void)
{
	CSTK_ASSERT_TRUE(0 > CSUDIFSWrite(CSUDI_NULL, "CSUDIFSWrite_0003", strlen("CSUDIFSWrite_0003")), "д���ļ�Ӧ��ʧ��\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSWrite(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount)
//@DESCRIPTION: ʹ�ò����ڵľ��д����
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = 0x7FFFFFFF(����0x7FFFFFFFΪ�����ڵľ��)
//@INPUT: pcBuf = "CSUDIFSWrite_0004"
//@INPUT: uCount = strlen("CSUDIFSWrite_0004")
//@EXPECTATION: ����ֵС��0
//@EXECUTIONFLOW: 1������CSUDIFSWrite(0x7FFFFFFF, "CSUDIFSWrite_0004", strlen("CSUDIFSWrite_0004"))д�����ݣ�Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSWrite_0004(void)
{
	CSTK_ASSERT_TRUE(0 > CSUDIFSWrite((CSUDI_HANDLE)0x7FFFFFFF, "CSUDIFSWrite_0004", strlen("CSUDIFSWrite_0004")), "д���ļ�Ӧ��ʧ��\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSWrite(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount)
//@DESCRIPTION: �ӿ�ָ��д������
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = hFile1�� ����hFile1ΪCSUDIFSOpen()���صĺϷ����
//@INPUT: pcBuf = CSUDI_NULL
//@INPUT: uCount = 0
//@EXPECTATION: ����ֵС��0
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSWrite_0005", "w+")
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile1, CSUDI_NULL, 0)д�����ݣ�Ӧ��ʧ��
//@EXECUTIONFLOW: 5������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 6������CSUDIFSRemove(��������·�� + "CSUDIFSWrite_0005")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSWrite_0005(void)
{
	char   szTest[] = "CSUDIFSWrite_0001";
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "���������ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 > CSUDIFSWrite(hFile1, CSUDI_NULL, 0), "д������Ӧ��ʧ��\n");

	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");
		}
	}

	return  CSUDI_TRUE;	
}
/****************************CSUDIFSWrite  End *******************************/

/****************************CSUDIFSClose  Start *****************************/ 
//@CASEGROUP: int CSUDIFSClose(CSUDI_HANDLE hFile)
//@DESCRIPTION: �ر�һ���Ϸ����ļ����
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = hFile1, ����hFile1ΪCSUDIFSOpen()���صĺϷ����
//@EXPECTATION: ����ֵ����0
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSClose_0001", "w+")��������һ���ļ�������þ��hFile1
//@EXECUTIONFLOW: 4������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 5������CSUDIFSClose(hFile1)�ٴιر��ļ���Ӧ��ʧ��
//@EXECUTIONFLOW: 6������CSUDIFSRemove(��������·�� + "CSUDIFSClose_0001")
CSUDI_BOOL CSTC_FS_IT_CSUDIFSClose_0001(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSClose_0001"; 
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "���������ļ�ʧ��\n");

	CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");

	CSTK_ASSERT_TRUE(0 >  CSUDIFSClose(hFile1), "�ٴιر��ļ���Ӧ�óɹ�\n");
	
	CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");

	CSTK_FATAL_POINT
	{
		;	
	}

	return  CSUDI_TRUE;		
}

//@CASEGROUP: int CSUDIFSClose(CSUDI_HANDLE hFile)
//@DESCRIPTION: �رտ��ļ����
//@PRECONDITION: 
//@INPUT: hFile = CSUDI_NULL
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSClose(CSUDI_NULL)�رվ����Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSClose_0002(void)
{
	CSTK_ASSERT_TRUE(-1 == CSUDIFSClose(CSUDI_NULL), "�ر��ļ���Ӧ�óɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSClose(CSUDI_HANDLE hFile)
//@DESCRIPTION: �رղ����ڵ��ļ����
//@PRECONDITION:
//@INPUT: hFile = 0x7FFFFFFF(����0x7FFFFFFFΪ�����ڵ��ļ����)
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSClose(0x7FFFFFFF)�رվ����Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSClose_0003(void)
{
	CSTK_ASSERT_TRUE(-1 == CSUDIFSClose((CSUDI_HANDLE)0x7FFFFFFF), "�ر��ļ���Ӧ�óɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}
/****************************CSUDIFSClose  End *******************************/

/****************************CSUDIFSSeek  Start *****************************/ 
//@CASEGROUP: int CSUDIFSSeek(CSUDI_HANDLE hFile, long lOffset, unsigned int uOrigin)
//@DESCRIPTION: д�������ٶ�λ���ļ�ͷ������������
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = hFile1������hFile1ΪCSUDIFSOpen()���صĺϷ����
//@INPUT: lOffset = 0
//@INPUT: uOrigin = CSFS_SEEK_SET
//@EXPECTATION: ����ֵ����0���Ҷ�����������д��һ��
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSSeek_0001", "w+")����һ���ļ��������Ӧ���hFile1
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile1, "CSUDIFSSeek_0001", strlen("CSUDIFSSeek_0001"))д������
//@EXECUTIONFLOW: 5������CSUDIFSSeek(hFile1, 0, CSUDIFS_SEEK_SET)�����ļ�ָ��
//@EXECUTIONFLOW: 6������CSUDIFSRead(hFile1, szBuf, sizeof(szBuf)-1)��ȡд������ݣ�Ӧ����д��ʱһ��
//@EXECUTIONFLOW: 7������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 8������CSUDIFSRemove(��������·�� + "CSUDIFSSeek_0001")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSSeek_0001(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSSeek_0001";
	int     nLen = strlen(szTest);
	char   szBuf[100];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "���������ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "д������ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile1, 0, CSUDIFS_SEEK_SET), "�����ļ�ʧ��\n");
	
	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSRead(hFile1, szBuf, sizeof(szBuf) -1), "��ȡ����ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(szTest, szBuf), "����������д��ʱ��һ��\n");

	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");
		}
	}

	return  CSUDI_TRUE;			
}

//@CASEGROUP: int CSUDIFSSeek(CSUDI_HANDLE hFile, long lOffset, unsigned int uOrigin)
//@DESCRIPTION: ���ļ�βΪ������ǰƫ�������ļ���С
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = hFile1������hFile1ΪCSUDIFSOpen()���صĺϷ����
//@INPUT: lOffset = 0 - strlen("CSUDIFSSeek_0002")
//@INPUT: uOrigin = CSUDIFS_SEEK_CUR
//@EXPECTATION: ����ֵ����0���Ҷ�����������д��һ��
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSSeek_0002", "w+")����һ���ļ��������Ӧ���hFile1
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile1, "CSUDIFSSeek_0002", strlen("CSUDIFSSeek_0002"))д������
//@EXECUTIONFLOW: 5������CSUDIFSSeek(hFile1, 0 - strlen("CSUDIFSSeek_0002"), CSUDIFS_SEEK_CUR)�����ļ�ָ��
//@EXECUTIONFLOW: 6������CSUDIFSRead(hFile1, szBuf, sizeof(szBuf)-1)��ȡд������ݣ�Ӧ����д��ʱһ��
//@EXECUTIONFLOW: 7������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 8������CSUDIFSRemove(��������·�� + "CSUDIFSSeek_0002")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSSeek_0002(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSSeek_0002";
	int     nLen = strlen(szTest);
	char   szBuf[100];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "���������ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "д������ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile1, 0 - nLen, CSUDIFS_SEEK_CUR), "�����ļ�ʧ��\n");
	
	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSRead(hFile1, szBuf, sizeof(szBuf) -1), "��ȡ����ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(szTest, szBuf), "����������д��ʱ��һ��\n");

	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");
		}
	}

	return  CSUDI_TRUE;				
}

//@CASEGROUP: int CSUDIFSSeek(CSUDI_HANDLE hFile, long lOffset, unsigned int uOrigin)
//@DESCRIPTION: ��λ���ļ����
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = CSUDI_NULL
//@INPUT: lOffset = 0
//@INPUT: uOrigin = CSUDIFS_SEEK_SET
//@EXPECTATION: = -1
//@EXECUTIONFLOW: 1������CSUDIFSSeek(CSUDI_NULL, 0, CSUDIFS_SEEK_SET)�ı��ļ�ָ�룬Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSSeek_0003(void)
{
	CSTK_ASSERT_TRUE(-1 == CSUDIFSSeek(CSUDI_NULL, 0, CSUDIFS_SEEK_SET), "���ò����ڵľ���ļ���Ӧ�óɹ�\n");	

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSSeek(CSUDI_HANDLE hFile, long lOffset, unsigned int uOrigin)
//@DESCRIPTION: ��λ�����ڵ��ļ����
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = 0x7FFFFFFF(����0x7FFFFFFFΪ�����ڵ��ļ����)
//@INPUT: lOffset = 10
//@INPUT: uOrigin = CSUDIFS_SEEK_SET 
//@EXPECTATION: = -1
//@EXECUTIONFLOW: 1������CSUDIFSSeek(0x7FFFFFFF, 10, CSUDIFS_SEEK_SET)�ı��ļ�ָ�룬Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSSeek_0004(void)
{
	CSTK_ASSERT_TRUE(-1 == CSUDIFSSeek((CSUDI_HANDLE)0x7FFFFFFF, 10, CSUDIFS_SEEK_SET), "���ò����ڵľ���ļ���Ӧ�óɹ�\n");	

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSSeek(CSUDI_HANDLE hFile, long lOffset, unsigned int uOrigin)
//@DESCRIPTION: ��׼λ�÷Ƿ�
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = hFile1, ����hFile1ΪCSUDIFSOpen()���صĺϷ����
//@INPUT: lOffset =  0
//@INPUT: uOrigin = 0x7FFFFFFF
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSSeek_0005", "w+")��������һ���ļ�����þ��hFile1
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile1, "CSUDIFSSeek_0005", strlen("CSUDIFSSeek_0005"))д������
//@EXECUTIONFLOW: 5������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 6������CSUDIFSOpen(��������·�� + "CSUDIFSSeek_0005", "r")���ļ�
//@EXECUTIONFLOW: 7������CSUDIFSSeek(hFile1, 0, 0x7FFFFFFF)�ı��ļ�ָ��
//@EXECUTIONFLOW: 8������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 9������CSUDIFSRemove(��������·�� + "CSUDIFSSeek_0005")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSSeek_0005(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSSeek_0005";
	int     nLen = strlen(szTest);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "���������ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "д������ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "r")), "���ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSSeek(hFile1,  0, 0x7FFFFFFF), "��׼λ�÷Ƿ������ļ���Ӧ�óɹ�\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");
		}
	}

	return  CSUDI_TRUE;		
}

//@CASEGROUP: int CSUDIFSSeek(CSUDI_HANDLE hFile, long lOffset, unsigned int uOrigin)
//@DESCRIPTION: ���ļ���ͷ���ƫ��Խ��
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = hFile1,����hFile1ΪCSUDIFSOpen()���صĺϷ����
//@INPUT: lOffset = strlen("CSUDIFSSeek_0006")+10
//@INPUT: uOrigin = CSFS_SEEK_SET
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSSeek_0006", "w+")��������һ���ļ�����þ��hFile1
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile1, "CSUDIFSSeek_0006", strlen("CSUDIFSSeek_0006"))д������
//@EXECUTIONFLOW: 5������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 6������CSUDIFSOpen(��������·�� + "CSUDIFSSeek_0006", "r")���ļ�
//@EXECUTIONFLOW: 7������CSUDIFSSeek(hFile1, strlen("CSUDIFSSeek_0006") + 10, CSUDIFS_SEEK_SET)�ı��ļ�ָ�룬Ӧ��ʧ��
//@EXECUTIONFLOW: 8������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 9������CSUDIFSRemove(��������·�� + "CSUDIFSSeek_0006")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSSeek_0006(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSSeek_0006";
	int     nLen = strlen(szTest);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "���������ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "д������ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "r")), "���ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSSeek(hFile1,  nLen + 10, CSUDIFS_SEEK_SET), "�ļ�ָ��Խ�粻Ӧ�óɹ�\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");
		}
	}

	return  CSUDI_TRUE;				
}

//@CASEGROUP: int CSUDIFSSeek(CSUDI_HANDLE hFile, long lOffset, unsigned int uOrigin)
//@DESCRIPTION: ���ļ���ͷ��ǰƫ��Խ��
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = hFile1,����hFile1ΪCSUDIFSOpen()���صĺϷ����
//@INPUT: lOffset = -1
//@INPUT: uOrigin = CSFS_SEEK_SET
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSSeek_0007", "w+")��������һ���ļ�����þ��hFile1
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile1, "CSUDIFSSeek_0007", strlen("CSUDIFSSeek_0007"))д������
//@EXECUTIONFLOW: 5������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 6������CSUDIFSOpen(��������·�� + "CSUDIFSSeek_0007", "r")���ļ�
//@EXECUTIONFLOW: 7������CSUDIFSSeek(hFile1, -1, CSUDIFS_SEEK_SET)�ı��ļ�ָ�룬Ӧ��ʧ��
//@EXECUTIONFLOW: 8������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 9������CSUDIFSRemove(��������·�� + "CSUDIFSSeek_0007")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSSeek_0007(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSSeek_0007";
	int     nLen = strlen(szTest);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "���������ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "д������ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "r")), "���ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSSeek(hFile1, -1, CSUDIFS_SEEK_SET), "�ļ�ָ��Խ�粻Ӧ�óɹ�\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");
		}
	}

	return  CSUDI_TRUE;				
}

//@CASEGROUP: int CSUDIFSSeek(CSUDI_HANDLE hFile, long lOffset, unsigned int uOrigin)
//@DESCRIPTION: ���ļ�β���ƫ��Խ��
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = hFile1,����hFile1ΪCSUDIFSOpen()���صĺϷ����
//@INPUT: lOffset = -1
//@INPUT: uOrigin = CSFS_SEEK_END
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSSeek_0008", "w+")��������һ���ļ�����þ��hFile1
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile1, "CSUDIFSSeek_0008", strlen("CSUDIFSSeek_0008"))д������
//@EXECUTIONFLOW: 5������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 6������CSUDIFSOpen(��������·�� + "CSUDIFSSeek_0008", "r")���ļ�
//@EXECUTIONFLOW: 7������CSUDIFSSeek(hFile1, 1, CSUDIFS_SEEK_END)�ı��ļ�ָ�룬Ӧ��ʧ��
//@EXECUTIONFLOW: 8������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 9������CSUDIFSRemove(��������·�� + "CSUDIFSSeek_0008")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSSeek_0008(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSSeek_0008";
	int     nLen = strlen(szTest);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "���������ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "д������ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "r")), "���ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSSeek(hFile1, 1, CSUDIFS_SEEK_END), "�ļ�ָ��Խ�粻Ӧ�óɹ�\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");
		}
	}

	return  CSUDI_TRUE;				
}

//@CASEGROUP: int CSUDIFSSeek(CSUDI_HANDLE hFile, long lOffset, unsigned int uOrigin)
//@DESCRIPTION: ���ļ������ƫ��Խ��
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = hFile1,����hFile1ΪCSUDIFSOpen()���صĺϷ����
//@INPUT: lOffset = strlen("CSUDIFSSeek_0006")
//@INPUT: uOrigin = CSFS_SEEK_CUR
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSSeek_0009", "w+")��������һ���ļ�����þ��hFile1
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile1, "CSUDIFSSeek_0006", strlen("CSUDIFSSeek_0009"))д������
//@EXECUTIONFLOW: 5������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 6������CSUDIFSOpen(��������·�� + "CSUDIFSSeek_0009", "r")���ļ�
//@EXECUTIONFLOW: 7������CSUDIFSSeek(hFile1, strlen("CSUDIFSSeek_0009")/2, CSUDIFS_SEEK_SET)�ı��ļ�ָ�룬Ӧ�óɹ�
//@EXECUTIONFLOW: 8������CSUDIFSSeek(hFile1, strlen("CSUDIFSSeek_0009"), CSUDIFS_SEEK_CUR)�ı��ļ�ָ�룬Ӧ��ʧ��
//@EXECUTIONFLOW: 9������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW:10������CSUDIFSRemove(��������·�� + "CSUDIFSSeek_0009")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSSeek_0009(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSSeek_0009";
	int     nLen = strlen(szTest);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "���������ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "д������ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "r")), "���ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile1,  nLen/2, CSUDIFS_SEEK_SET), "�ļ�ָ��seekʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSSeek(hFile1,  nLen, CSUDIFS_SEEK_CUR), "�ļ�ָ��Խ�粻Ӧ�óɹ�\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");
		}
	}

	return  CSUDI_TRUE;				
}

//@CASEGROUP: int CSUDIFSSeek(CSUDI_HANDLE hFile, long lOffset, unsigned int uOrigin)
//@DESCRIPTION: �ļ�write,seek��read�Ȳ�������βʱ����ֵ
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = hFile1������hFile1ΪCSUDIFSOpen()���صĺϷ����
//@INPUT: lOffset = 0
//@INPUT: uOrigin = CSFS_SEEK_SET
//@EXPECTATION: ����ֵ����0���Ҷ�����������д��һ��
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSSeek_0001", "w+")����һ���ļ��������Ӧ���hFile1
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile1, "CSUDIFSSeek_0010", strlen("CSUDIFSSeek_0010"))д������
//@EXECUTIONFLOW: 5������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 6������CSUDIFSOpen(��������·�� + "CSUDIFSSeek_0010", "r")���ļ�
//@EXECUTIONFLOW: 7������CSUDIFSSeek(hFile1, 0, CSUDIFS_SEEK_SET)�����ļ�ָ��
//@EXECUTIONFLOW: 8������CSUDIFSRead(hFile1, szBuf, sizeof(szBuf)-1)��ȡд������ݣ�Ӧ����д��ʱһ��
//@EXECUTIONFLOW: 9������CSUDIFSSeek(hFile1, 0, CSUDIFS_SEEK_SET)�����ļ�ָ�뵽�ļ�β
//@EXECUTIONFLOW:10������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 11������CSUDIFSRemove(��������·�� + "CSUDIFSSeek_0010")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSSeek_0010(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSSeek_0010";
	int     nLen = strlen(szTest);
	char   szBuf[100];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "���������ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "д������ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "r")), "���ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile1, 0, CSUDIFS_SEEK_SET), "�����ļ�ʧ��\n");
	
	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSRead(hFile1, szBuf, sizeof(szBuf) -1), "��ȡ����ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(szTest, szBuf), "����������д��ʱ��һ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile1, 0, CSUDIFS_SEEK_END), "�����ļ�ʧ��\n");
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");
		}
	}
	return  CSUDI_TRUE;	
}

static char s_szSeekText[100 * 1024]	= {0};						// ��̬ȫ�ֱ��������д���ļ�������
static char s_szSeekBuffer[100 * 1024]	= {0};						// ��̬ȫ�ֱ�������Ŵ��ļ��ж�ȡ������

//@CASEGROUP: int CSUDIFSSeek(CSUDI_HANDLE hFile, long lOffset, unsigned int uOrigin)
//@DESCRIPTION: uOriginΪCSUDIFS_SEEK_SETʱ�����ϵض�λ���ݡ��������ݣ�����ԭʼ���ݽ��бȽ�
//@PRECONDITION: 1��ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: 1��hFileΪʹ��CSUDIFSOpen()���ص��ļ����
//@INPUT: 2��s_szSeekText[100 * 1024] = {0, 1, ..., 127, 0, 1, ... 127, ...}Ϊ�ļ���ʵ������
//@INPUT: 3��s_szSeekBuffer[100 * 1024] = {0}Ϊ��Ŷ������ݵĻ���
//@INPUT: 4��nTextLen = sizeof(s_szSeekText);
//@INPUT: 5��nGap = nTextLen/128 + 1(ÿ��i����nGap),������128��CSUDIFSSeek()
//@INPUT: 6��uOriginΪCSUDIFS_SEEK_SET
//@EXPECTATION: ÿһ��������Ԥ�ڽ��
//@EXECUTIONFLOW: 1��Ϊs_szSeekText��ֵ{0, 1, ..., 127, 0, 1, ..., 127}��Ϊs_szSeekBuffer��ֵΪ{0}
//@EXECUTIONFLOW: 2������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 3������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 4�����ô����ļ�CSUDIFSOpen(��������·�� + "cstc_fs_jvm_temp", "w+")����þ��hFile
//@EXECUTIONFLOW: 5������CSUDIFSWrite(hFile, s_szSeekText, nTextLen)д������
//@EXECUTIONFLOW: 6������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 7������CSUDIFSOpen(��������·�� + "cstc_fs_jvm_temp", "r+")���ļ�����ȡ���hFile
//@EXECUTIONFLOW: 8������CSUDIFSTell(hFile)��ȡ��ǰָ��λ�ã��ж��ļ�ָ��λ���Ƿ���ȷ
//@EXECUTIONFLOW: 9������CSUDIFSRead(hFile, s_szSeekBuffer, nTextLen)��ȡ�ļ�������
//@EXECUTIONFLOW: 10������memcmp(s_szSeekBuffer, s_szSeekText, nTextLen)�Ƚ϶����������д��������Ƿ�һ��
//@EXECUTIONFLOW: 11������memset(s_szSeekBuffer, 0, nTextLen)����s_szSeekBuffer
//@EXECUTIONFLOW: 12���ж�i�Ƿ�С��nTextLen/2�����ǣ�ִ����һ��������������ת����20��
//@EXECUTIONFLOW: 13������CSUDIFSSeek(hFile, nTextLen - 1 - i, CSUDIFS_SEEK_SET)���ļ�ָ���ƶ���nTextLen-1-iλ�ô�
//@EXECUTIONFLOW: 14������CSUDIFSTell(hFile)��ȡ��ǰָ��λ�ã��ж��ļ�ָ��λ���Ƿ���ȷ
//@EXECUTIONFLOW: 15������CSUDIFSSeek(hFile, i, CSUDIFS_SEEK_SET)���ļ�ָ���ƶ���iλ�ô�
//@EXECUTIONFLOW: 16������CSUDIFSTell(hFile)��ȡ��ǰָ��λ�ã��ж��ļ�ָ��λ���Ƿ���ȷ
//@EXECUTIONFLOW: 17������CSUDIFSRead(hFile, s_szSeekBuffer, nTextLen - 2 * i)��ȡ�ļ��ĵ�iλ�����nTextLen-1-iλ��֮�������
//@EXECUTIONFLOW: 18������memcmp(s_szSeekBuffer, s_szSeekText + i, nTextLen - 2 * i)�Ƚ϶����������д��������Ƿ�һ�¡�����ȣ�ִ����һ��������������ת����20��
//@EXECUTIONFLOW: 19������memset(s_szSeekBuffer, 0, nTextLen - 2 * i)����s_szSeekBuffer������i += nGap��Ȼ����ת����12��
//@EXECUTIONFLOW: 20������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 21������CSUDIFSRemove(��������·�� + "cstc_fs_jvm_temp")�Ƴ��ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSSeek_0011(void)
{
	int i				= 0;
	int nTempLen		= 0;
	int nTextLen		= (sizeof(s_szSeekText) > 102400) ? 102400 : sizeof(s_szSeekText);	// ��󲻳���100K
	int nHalfTextLen	= nTextLen >> 1;			
	int nGap			= (nTextLen >> 7) + 1;		// nGap = nTextLen/128 + 1(iÿ������nGap)
	CSUDI_HANDLE hFile	= CSUDI_NULL;
	char szFileName []	= "CSUDIFSSeek_0011";			

	for (i = 0; i < nTextLen; i++)
	{
		s_szSeekText[i] = i % 128;
	}

	memset(s_szSeekBuffer, 0, sizeof(s_szSeekBuffer));

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "����2ʧ�ܣ���ȡ�������ص�ʧ��");

	strcat(g_szVolumeMountPoint, szFileName);

	hFile = CSUDIFSOpen(g_szVolumeMountPoint, "w+");
	CSTK_ASSERT_TRUE_FATAL(hFile != CSUDI_NULL, "����4ʧ�ܣ����������ļ�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		nTextLen == CSUDIFSWrite(hFile, s_szSeekText, nTextLen), 
		"����5ʧ�ܣ�д������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSClose(hFile) == 0, "����6ʧ�ܣ��ر��ļ�ʧ��");

	hFile = CSUDIFSOpen(g_szVolumeMountPoint, "r+");
	CSTK_ASSERT_TRUE_FATAL(hFile != CSUDI_NULL, "����7ʧ�ܣ����ļ�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIFSTell(hFile) == 0,
		"����8ʧ�ܣ���ȡ�ļ���ǰλ��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		nTextLen == CSUDIFSRead(hFile, s_szSeekBuffer, nTextLen),
		"����9ʧ�ܣ���ȡ����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		memcmp(s_szSeekBuffer, s_szSeekText, nTextLen) == 0,
		"����10ʧ�ܣ�����������д��ʱ�����ݲ�һ��");

	memset(s_szSeekBuffer, 0, nTextLen);

	for (i = 0; i < nHalfTextLen; i += nGap)
	{
		CSTK_ASSERT_TRUE_FATAL(
			CSUDIFSSeek(hFile, nTextLen - 1 - i, CSUDIFS_SEEK_SET) == 0,
			"����13ʧ�ܣ��ƶ��ļ�ָ��ʧ��");

		CSTK_ASSERT_TRUE_FATAL(
			CSUDIFSTell(hFile) == nTextLen - 1 - i,
			"����14ʧ�ܣ���ȡ�ļ���ǰλ��ʧ��");

		CSTK_ASSERT_TRUE_FATAL(
			CSUDIFSSeek(hFile, i, CSUDIFS_SEEK_SET) == 0,
			"����15ʧ�ܣ��ƶ��ļ�ָ��ʧ��");

		CSTK_ASSERT_TRUE_FATAL(
			CSUDIFSTell(hFile) == i,
			"����16ʧ�ܣ���ȡ�ļ���ǰλ��ʧ��");

		nTempLen = nTextLen - 2 * i;
		CSTK_ASSERT_TRUE_FATAL(
			nTempLen == CSUDIFSRead(hFile, s_szSeekBuffer, nTempLen),
			"����17ʧ�ܣ���ȡ����ʧ��");

		CSTK_ASSERT_TRUE_FATAL(
			memcmp(s_szSeekBuffer, s_szSeekText + i, nTempLen) == 0,
			"����18ʧ�ܣ�����������д��ʱ�����ݲ�һ��");

		memset(s_szSeekBuffer, 0, nTempLen);
	}

	CSTK_FATAL_POINT
	{
		if (hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(
				CSUDIFSClose(hFile) == 0, 
				"����20ʧ�ܣ��ر��ļ�ʧ��");

			CSTK_ASSERT_TRUE(
				CSUDIFSRemove(g_szVolumeMountPoint) == 0, 
				"����21ʧ�ܣ��Ƴ��ļ�ʧ��\n");
		}
	}

	return  CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSSeek(CSUDI_HANDLE hFile, long lOffset, unsigned int uOrigin)
//@DESCRIPTION: uOriginΪCSUDIFS_SEEK_ENDʱ�����ϵض�λ���ݡ��������ݣ�����ԭʼ���ݽ��бȽ�
//@PRECONDITION: 1��ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: 1��hFileΪʹ��CSUDIFSOpen()���ص��ļ����
//@INPUT: 2��s_szSeekText[100 * 1024] = {0, 1, ..., 127, 0, 1, ... 127, ...}Ϊ�ļ���ʵ������
//@INPUT: 3��s_szSeekBuffer[100 * 1024] = {0}Ϊ��Ŷ������ݵĻ���
//@INPUT: 4��nTextLen = sizeof(s_szSeekText);
//@INPUT: 5��nGap = nTextLen/128 + 1(iÿ������nGap),������128��CSUDIFSSeek()
//@INPUT: 6��uOriginΪCSUDIFS_SEEK_END//@EXPECTATION: ÿһ��������Ԥ�ڽ��
//@EXECUTIONFLOW: 1��Ϊs_szSeekText��ֵ{0, 1, ..., 127, 0, 1, ..., 127}��Ϊs_szSeekBuffer��ֵΪ{0}
//@EXECUTIONFLOW: 2������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 3������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 4�����ô����ļ�CSUDIFSOpen(��������·�� + "cstc_fs_jvm_temp", "w+")����þ��hFile
//@EXECUTIONFLOW: 5������CSUDIFSWrite(hFile, s_szSeekText, nTextLen)д������
//@EXECUTIONFLOW: 6������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 7������CSUDIFSOpen(��������·�� + "cstc_fs_jvm_temp", "r+")���ļ�����ȡ���hFile
//@EXECUTIONFLOW: 8������CSUDIFSTell(hFile)��ȡ��ǰָ��λ�ã��ж��ļ�ָ��λ���Ƿ���ȷ
//@EXECUTIONFLOW: 9������CSUDIFSRead(hFile, s_szSeekBuffer, nTextLen)��ȡ�ļ�������
//@EXECUTIONFLOW: 10������memcmp(s_szSeekBuffer, s_szSeekText, nTextLen)�Ƚ϶����������д��������Ƿ�һ��
//@EXECUTIONFLOW: 11������memset(s_szSeekBuffer, 0, nTextLen)����s_szSeekBuffer
//@EXECUTIONFLOW: 12���ж�i�Ƿ�С��nTextLen/2�����ǣ�ִ����һ��������������ת����20��
//@EXECUTIONFLOW: 13������CSUDIFSSeek(hFile, -1 * (i + 1), CSUDIFS_SEEK_END)���ļ�ָ���ƶ���nTextLen-1-iλ�ô�
//@EXECUTIONFLOW: 14������CSUDIFSTell(hFile)��ȡ��ǰָ��λ�ã��ж��ļ�ָ��λ���Ƿ���ȷ
//@EXECUTIONFLOW: 15������CSUDIFSSeek(hFile, -1 * (nTextLen - i), CSUDIFS_SEEK_END)���ļ�ָ���ƶ���iλ�ô�
//@EXECUTIONFLOW: 16������CSUDIFSTell(hFile)��ȡ��ǰָ��λ�ã��ж��ļ�ָ��λ���Ƿ���ȷ
//@EXECUTIONFLOW: 17������CSUDIFSRead(hFile, s_szSeekBuffer, nTextLen - 2 * i)��ȡ�ļ��ĵ�iλ�����nTextLen-1-iλ��֮�������
//@EXECUTIONFLOW: 18������memcmp(s_szSeekBuffer, s_szSeekText + i, nTextLen - 2 * i)�Ƚ϶����������д��������Ƿ�һ�¡�����ȣ�ִ����һ��������������ת����20��
//@EXECUTIONFLOW: 19������memset(s_szSeekBuffer, 0, nTextLen - 2 * i)����s_szSeekBuffer������i += nGap��Ȼ����ת����12��
//@EXECUTIONFLOW: 20������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 21������CSUDIFSRemove(��������·�� + "cstc_fs_jvm_temp")�Ƴ��ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSSeek_0012(void)
{
	int i				= 0;
	int nTempLen		= 0;
	int nTextLen		= (sizeof(s_szSeekText) > 102400) ? 102400 : sizeof(s_szSeekText);		// ��󲻳���100K
	int nHalfTextLen	= nTextLen >> 1;			
	int nGap			= (nTextLen >> 7) + 1;		// nGap = nTextLen/128 + 1(iÿ������nGap)
	CSUDI_HANDLE hFile	= CSUDI_NULL;
	char szFileName []	= "CSUDIFSSeek_0012";			

	for (i = 0; i < nTextLen; i++)
	{
		s_szSeekText[i] = i % 128;
	}

	memset(s_szSeekBuffer, 0, sizeof(s_szSeekBuffer));

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "����2ʧ�ܣ���ȡ�������ص�ʧ��");

	strcat(g_szVolumeMountPoint, szFileName);

	hFile = CSUDIFSOpen(g_szVolumeMountPoint, "w+");
	CSTK_ASSERT_TRUE_FATAL(hFile != CSUDI_NULL, "����4ʧ�ܣ����������ļ�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		nTextLen == CSUDIFSWrite(hFile, s_szSeekText, nTextLen), 
		"����5ʧ�ܣ�д������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSClose(hFile) == 0, "����6ʧ�ܣ��ر��ļ�ʧ��");

	hFile = CSUDIFSOpen(g_szVolumeMountPoint, "r+");
	CSTK_ASSERT_TRUE_FATAL(hFile != CSUDI_NULL, "����7ʧ�ܣ����ļ�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIFSTell(hFile) == 0,
		"����8ʧ�ܣ���ȡ�ļ���ǰλ��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		nTextLen == CSUDIFSRead(hFile, s_szSeekBuffer, nTextLen),
		"����9ʧ�ܣ���ȡ����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		memcmp(s_szSeekBuffer, s_szSeekText, nTextLen) == 0,
		"����10ʧ�ܣ�����������д��ʱ�����ݲ�һ��");

	memset(s_szSeekBuffer, 0, nTextLen);

	for (i = 0; i < nHalfTextLen; i += nGap)
	{
		// ���ļ�ָ���ƶ�����nTextLen-1-iλ�ô�
		CSTK_ASSERT_TRUE_FATAL(
			CSUDIFSSeek(hFile, -1 * (i + 1), CSUDIFS_SEEK_END) == 0,
			"����13ʧ�ܣ��ƶ��ļ�ָ��ʧ��");

		CSTK_ASSERT_TRUE_FATAL(
			CSUDIFSTell(hFile) == nTextLen - 1 - i,
			"����14ʧ�ܣ���ȡ�ļ���ǰλ��ʧ��");

		// ���ļ�ָ���ƶ�����iλ�ô�
		CSTK_ASSERT_TRUE_FATAL(
			CSUDIFSSeek(hFile, -1 * (nTextLen - i), CSUDIFS_SEEK_END) == 0,
			"����15ʧ�ܣ��ƶ��ļ�ָ��ʧ��");

		CSTK_ASSERT_TRUE_FATAL(
			CSUDIFSTell(hFile) == i,
			"����16ʧ�ܣ���ȡ�ļ���ǰλ��ʧ��");

		nTempLen = nTextLen - 2 * i;
		CSTK_ASSERT_TRUE_FATAL(
			nTempLen == CSUDIFSRead(hFile, s_szSeekBuffer, nTempLen),
			"����17ʧ�ܣ���ȡ����ʧ��");

		CSTK_ASSERT_TRUE_FATAL(
			memcmp(s_szSeekBuffer, s_szSeekText + i, nTempLen) == 0,
			"����18ʧ�ܣ�����������д��ʱ�����ݲ�һ��");

		memset(s_szSeekBuffer, 0, nTempLen);
	}

	CSTK_FATAL_POINT
	{
		if (hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(
				CSUDIFSClose(hFile) == 0, 
				"����20ʧ�ܣ��ر��ļ�ʧ��");

			CSTK_ASSERT_TRUE(
				CSUDIFSRemove(g_szVolumeMountPoint) == 0, 
				"����21ʧ�ܣ��Ƴ��ļ�ʧ��\n");
		}
	}

	return  CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSSeek(CSUDI_HANDLE hFile, long lOffset, unsigned int uOrigin)
//@DESCRIPTION: uOriginΪCSUDIFS_SEEK_CURʱ�����ϵض�λ���ݡ��������ݣ�����ԭʼ���ݽ��бȽ�
//@PRECONDITION: 1��ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: 1��hFileΪʹ��CSUDIFSOpen()���ص��ļ����
//@INPUT: 2��s_szSeekText[100 * 1024] = {0, 1, ..., 127, 0, 1, ... 127, ...}Ϊ�ļ���ʵ������
//@INPUT: 3��s_szSeekBuffer[100 * 1024] = {0}Ϊ��Ŷ������ݵĻ���
//@INPUT: 4��nTextLen = sizeof(s_szSeekText);
//@INPUT: 5��nGap = nTextLen/64 + 1(iÿ������nGap),������128��CSUDIFSSeek()
//@INPUT: 6��uOriginΪCSUDIFS_SEEK_CUR//@EXPECTATION: ÿһ��������Ԥ�ڽ��
//@EXECUTIONFLOW: 1��Ϊs_szSeekText��ֵ{0, 1, ..., 127, 0, 1, ..., 127}��Ϊs_szSeekBuffer��ֵΪ{0}
//@EXECUTIONFLOW: 2������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 3������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 4�����ô����ļ�CSUDIFSOpen(��������·�� + "cstc_fs_jvm_temp", "w+")����þ��hFile
//@EXECUTIONFLOW: 5������CSUDIFSWrite(hFile, s_szSeekText, nTextLen)д������
//@EXECUTIONFLOW: 6������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 7������CSUDIFSOpen(��������·�� + "cstc_fs_jvm_temp", "r+")���ļ�����ȡ���hFile
//@EXECUTIONFLOW: 8������CSUDIFSTell(hFile)��ȡ��ǰָ��λ�ã��ж��ļ�ָ��λ���Ƿ���ȷ
//@EXECUTIONFLOW: 9������CSUDIFSRead(hFile, s_szSeekBuffer, nTextLen)��ȡ�ļ�������
//@EXECUTIONFLOW: 10������memcmp(s_szSeekBuffer, s_szSeekText, nTextLen)�Ƚ϶����������д��������Ƿ�һ��
//@EXECUTIONFLOW: 11������memset(s_szSeekBuffer, 0, nTextLen)����s_szSeekBuffer
//@EXECUTIONFLOW: 12������CSUDIFSSeek(hFile, 0, CSUDIFS_SEEK_SET)���ļ�ָ���ƶ�����ʼλ��
//@EXECUTIONFLOW: 13������CSUDIFSTell(hFile)��ȡ��ǰָ��λ�ã��ж��ļ�ָ��λ���Ƿ���ȷ
//@EXECUTIONFLOW: 14���ж�i + Gap�Ƿ�С�ڵ���nTextLen�����ǣ�ִ����һ��������������ת����22��
//@EXECUTIONFLOW: 15������CSUDIFSSeek(hFile, Gap, CSUDIFS_SEEK_CUR)���ļ�ָ���ƶ���i + nGapλ�ô�
//@EXECUTIONFLOW: 16������CSUDIFSTell(hFile)��ȡ��ǰָ��λ�ã��ж��ļ�ָ��λ���Ƿ���ȷ
//@EXECUTIONFLOW: 17������CSUDIFSSeek(hFile, -nGap, CSUDIFS_SEEK_CUR)���ļ�ָ���ƶ���iλ�ô�
//@EXECUTIONFLOW: 18������CSUDIFSTell(hFile)��ȡ��ǰָ��λ�ã��ж��ļ�ָ��λ���Ƿ���ȷ
//@EXECUTIONFLOW: 19������CSUDIFSRead(hFile, s_szSeekBuffer, nGap)��ȡ�ļ��ĵ�iλ�����i + nGapλ��֮�������
//@EXECUTIONFLOW: 20������memcmp(s_szSeekBuffer, s_szSeekText + i, nGap)�Ƚ϶����������д��������Ƿ�һ�¡����ǣ�ִ����һ��������������ת����26��
//@EXECUTIONFLOW: 21������memset(s_szSeekBuffer, 0, nGap)����s_szSeekBuffer������i+=nGap��Ȼ����ת����14��
//@EXECUTIONFLOW: 22������CSUDIFSSeek(hFile, -i, CSUDIFS_SEEK_CUR)���ļ�ָ���ƶ�����ʼλ�ô�
//@EXECUTIONFLOW: 23������CSUDIFSTell(hFile)��ȡ��ǰָ��λ�ã��ж��ļ�ָ��λ���Ƿ���ȷ
//@EXECUTIONFLOW: 24������CSUDIFSRead(hFile, s_szSeekBuffer, nTextLen)��ȡ�ļ�������
//@EXECUTIONFLOW: 25������memcmp(s_szSeekBuffer, s_szSeekText, nTextLen)�Ƚ϶����������д��������Ƿ�һ��
//@EXECUTIONFLOW: 26������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 27������CSUDIFSRemove(��������·�� + "cstc_fs_jvm_temp")�Ƴ��ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSSeek_0013(void)
{
	int i				= 0;
	//int nTempLen		= 0;
	CSUDI_HANDLE hFile	= CSUDI_NULL;
	char szFileName []	= "CSUDIFSSeek_0013";
	int nTextLen		= (sizeof(s_szSeekText) > 102400) ? 102400 : sizeof(s_szSeekText);		// ��󲻳���100K
	int nGap			= (nTextLen >> 7) + 1;		// nGap = nTextLen/128 + 1(iÿ������nGap)

	for (i = 0; i < nTextLen; i++)
	{
		s_szSeekText[i] = i % 128;
	}

	memset(s_szSeekBuffer, 0, sizeof(s_szSeekBuffer));

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "����2ʧ�ܣ���ȡ�������ص�ʧ��");

	strcat(g_szVolumeMountPoint, szFileName);

	hFile = CSUDIFSOpen(g_szVolumeMountPoint, "w+");
	CSTK_ASSERT_TRUE_FATAL(hFile != CSUDI_NULL, "����4ʧ�ܣ����������ļ�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		nTextLen == CSUDIFSWrite(hFile, s_szSeekText, nTextLen), 
		"����5ʧ�ܣ�д������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSClose(hFile) == 0, "����6ʧ�ܣ��ر��ļ�ʧ��");

	hFile = CSUDIFSOpen(g_szVolumeMountPoint, "r+");
	CSTK_ASSERT_TRUE_FATAL(hFile != CSUDI_NULL, "����7ʧ�ܣ����ļ�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIFSTell(hFile) == 0,
		"����8ʧ�ܣ���ȡ�ļ���ǰλ��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		nTextLen == CSUDIFSRead(hFile, s_szSeekBuffer, nTextLen),
		"����9ʧ�ܣ���ȡ����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		memcmp(s_szSeekBuffer, s_szSeekText, nTextLen) == 0,
		"����10ʧ�ܣ�����������д��ʱ�����ݲ�һ��");

	memset(s_szSeekBuffer, 0, nTextLen);

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIFSSeek(hFile, 0, CSUDIFS_SEEK_SET) == 0,
		"����12ʧ�ܣ��ƶ��ļ�ָ��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIFSTell(hFile) == 0,
		"����13ʧ�ܣ���ȡ�ļ���ǰλ��ʧ��");

	for (i = 0; i + nGap <= nTextLen; i += nGap)
	{
		CSTK_ASSERT_TRUE_FATAL(
			CSUDIFSSeek(hFile, nGap, CSUDIFS_SEEK_CUR) == 0,
			"����15ʧ�ܣ��ƶ��ļ�ָ��ʧ��");

		CSTK_ASSERT_TRUE_FATAL(
			CSUDIFSTell(hFile) == i + nGap,
			"����16ʧ�ܣ���ȡ�ļ���ǰλ��ʧ��");

		CSTK_ASSERT_TRUE_FATAL(
			CSUDIFSSeek(hFile, -nGap, CSUDIFS_SEEK_CUR) == 0,
			"����17ʧ�ܣ��ƶ��ļ�ָ��ʧ��");

		CSTK_ASSERT_TRUE_FATAL(
			CSUDIFSTell(hFile) == i,
			"����18ʧ�ܣ���ȡ�ļ���ǰλ��ʧ��");

		CSTK_ASSERT_TRUE_FATAL(
			nGap == CSUDIFSRead(hFile, s_szSeekBuffer, nGap),
			"����19ʧ�ܣ���ȡ����ʧ��");

		CSTK_ASSERT_TRUE_FATAL(
			memcmp(s_szSeekBuffer, s_szSeekText + i, nGap) == 0,
			"����20ʧ�ܣ�����������д��ʱ�����ݲ�һ��");

		memset(s_szSeekBuffer, 0, nGap);
	}

	// ���ļ�ָ���ƶ�����ʼ��
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIFSSeek(hFile, -i, CSUDIFS_SEEK_CUR) == 0,
		"����22ʧ�ܣ��ƶ��ļ�ָ��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIFSTell(hFile) == 0,
		"����23ʧ�ܣ���ȡ�ļ���ǰλ��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		nTextLen == CSUDIFSRead(hFile, s_szSeekBuffer, nTextLen),
		"����24ʧ�ܣ���ȡ����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		memcmp(s_szSeekBuffer, s_szSeekText, nTextLen) == 0,
		"����25ʧ�ܣ�����������д��ʱ�����ݲ�һ��");

	CSTK_FATAL_POINT
	{
		if (hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(
				CSUDIFSClose(hFile) == 0, 
				"����26ʧ�ܣ��ر��ļ�ʧ��");

			CSTK_ASSERT_TRUE(
				CSUDIFSRemove(g_szVolumeMountPoint) == 0, 
				"����27ʧ�ܣ��Ƴ��ļ�ʧ��\n");
		}
	}

	return  CSUDI_TRUE;
}
/****************************CSUDIFSSeek  End *******************************/

/****************************CSUDIFSLseek  Start *****************************/ 
//@CASEGROUP: int CSUDIFSLseek(CSUDI_HANDLE hFile,CSUDI_INT64 i64Offset,unsigned int uOrigin)
//@DESCRIPTION: ������λ
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = hFile1������hFile1ΪCSUDIFSOpen()���صĺϷ����
//@INPUT: i64Offset = 0
//@INPUT: uOrigin = CSUDIFS_SEEK_SET
//@EXPECTATION: = 0
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSLseek_0001", "w+")�����ʹ�һ���ļ����õ����hFile1
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile1, "CSUDIFSLseek_0001", strlen("CSUDIFSLseek_0001"))д������
//@EXECUTIONFLOW: 5������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 6������CSUDIFSOpen(��������·�� + "CSUDIFSLseek_0001", "r")���ļ�
//@EXECUTIONFLOW: 7������CSUDIFSLseek(hFile1, 0, CSFS_SEEK_SET)�����ļ�ָ��
//@EXECUTIONFLOW: 8������CSUDIFSRead(hFile1, szBuf, sizeof(szBuf)-1)��ȡ���ݣ�Ӧ����д��ʱ��һ��
//@EXECUTIONFLOW: 9������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 10������CSUDIFSRemove(��������·�� + "CSUDIFSLseek_0001")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLseek_0001(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSLseek_0001";
	int     nLen = strlen(szTest);
	char   szBuf[100];
	CSUDI_INT64  i64Offset;
	
	ll_setZero(i64Offset);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "���������ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "д������ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "r")), "���ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSLseek(hFile1, i64Offset, CSUDIFS_SEEK_SET), "�����ļ�ʧ��\n");
	
	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSRead(hFile1, szBuf, sizeof(szBuf) -1), "��ȡ����ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(szTest, szBuf), "����������д��ʱ��һ��\n");

	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");
		}
	}

	return  CSUDI_TRUE;			
}

//@CASEGROUP: int CSUDIFSLseek(CSUDI_HANDLE hFile,CSUDI_INT64 i64Offset,unsigned int uOrigin)
//@DESCRIPTION: ���ļ�βΪ������ǰƫ�������ļ���С
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = hFile1������hFile1ΪCSUDIFSOpen()���صĺϷ����
//@INPUT: i64Offset = 0 - strlen("CSUDIFSLseek_0002")
//@INPUT: uOrigin = CSUDIFS_SEEK_CUR
//@EXPECTATION: ����ֵ����0���Ҷ�����������д��һ��
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSLseek_0002", "w+")����һ���ļ��������Ӧ���hFile1
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile1, "CSUDIFSLseek_0002", strlen("CSUDIFSLseek_0002"))д������
//@EXECUTIONFLOW: 5������CSUDIFSFlush(hFile1)ͬ��������
//@EXECUTIONFLOW: 6������CSUDIFSLseek(hFile1, 0 - strlen("CSUDIFSLseek_0002"), CSUDIFS_SEEK_CUR)�����ļ�ָ��
//@EXECUTIONFLOW: 7������CSUDIFSRead(hFile1, szBuf, sizeof(szBuf)-1)��ȡд������ݣ�Ӧ����д��ʱһ��
//@EXECUTIONFLOW: 8������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 9������CSUDIFSRemove(��������·�� + "CSUDIFSSeek_0002")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLseek_0002(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSLseek_0002";
	int     nLen = strlen(szTest);
	char   szBuf[100];
	CSUDI_INT64  i64Offset;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "���������ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "д������ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSFlush(hFile1), "ͬ��������ʧ��\n");
	
	i64Offset = int2ll(0-nLen);

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSLseek(hFile1, i64Offset, CSUDIFS_SEEK_CUR), "�����ļ�ʧ��\n");
	
	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSRead(hFile1, szBuf, sizeof(szBuf) -1), "��ȡ����ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(szTest, szBuf), "����������д��ʱ��һ��\n");

	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");
		}
	}

	return  CSUDI_TRUE;				
}

//@CASEGROUP: int CSUDIFSLseek(CSUDI_HANDLE hFile,CSUDI_INT64 i64Offset,unsigned int uOrigin)
//@DESCRIPTION: ��λ���ļ����
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = CSUDI_NULL
//@INPUT: i64Offset = 0 
//@INPUT: uOrigin = CSUDIFS_SEEK_SET
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSLseek(NULL, 0, CSUDIFS_SEEK_SET)�����ļ�ָ�룬Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLseek_0003(void)
{
	CSUDI_INT64 i64Offset;

	ll_setZero(i64Offset);
		
	CSTK_ASSERT_TRUE(-1 == CSUDIFSLseek(CSUDI_NULL, i64Offset, CSUDIFS_SEEK_SET), "���ò����ڵľ���ļ���Ӧ�óɹ�\n");	

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSLseek(CSUDI_HANDLE hFile,CSUDI_INT64 i64Offset,unsigned int uOrigin)
//@DESCRIPTION: ��λ�����ڵ��ļ����
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = 0x7FFFFFFF(����0x7FFFFFFFΪ�����ڵ��ļ����)
//@INPUT: i64Offset = 10
//@INPUT: uOrigin = CSUDIFS_SEEK_SET
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSLseek(hFile, 10, CSFS_SEEK_SET)�����ļ�ָ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLseek_0004(void)
{
	CSUDI_INT64 i64Offset;

	ll_int2long(i64Offset, 10);

	CSTK_ASSERT_TRUE(-1 == CSUDIFSLseek((CSUDI_HANDLE)0x7FFFFFFF, i64Offset, CSUDIFS_SEEK_SET), "���ò����ڵľ���ļ���Ӧ�óɹ�\n");	

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSLseek(CSUDI_HANDLE hFile,CSUDI_INT64 i64Offset,unsigned int uOrigin)
//@DESCRIPTION: ��׼λ�÷Ƿ�
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = hFile1��hFile1ΪCSUDIFSOpen()���صĺϷ����
//@INPUT: i64Offset = 0
//@INPUT: uOrigin = 0x7FFFFFFF
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���Ŀ¼	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSLseek_0005", "w+")��������һ���ļ�����þ��hFile1
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile1, "CSUDIFSLseek_0005", strlen("CSUDIFSLseek_0005"))д������
//@EXECUTIONFLOW: 5������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 6������CSUDIFSOpen(��������·�� + "CSUDIFSLseek_0005", "r")���ļ�
//@EXECUTIONFLOW: 7������CSUDIFSLseek(hFile1, 0, 0x7FFFFFFF)�����ļ�ָ�룬Ӧ��ʧ��
//@EXECUTIONFLOW: 8������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 9������CSUDIFSRemove(��������·�� + "CSUDIFSLseek_0005")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLseek_0005(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSLseek_0005";
	int     nLen = strlen(szTest);
	CSUDI_INT64 i64Offset;

	ll_setZero(i64Offset);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "���������ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "д������ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "r")), "���ļ�ʧ��\n");
	
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSLseek(hFile1,  i64Offset, 0x7FFFFFFF), "��׼λ�÷Ƿ������ļ���Ӧ�óɹ�\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");
		}
	}

	return  CSUDI_TRUE;			
}

//@CASEGROUP: int CSUDIFSLseek(CSUDI_HANDLE hFile,CSUDI_INT64 i64Offset,unsigned int uOrigin)
//@DESCRIPTION: ��λ��Ŀ��λ��Խ��
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = hFile1��hFile1ΪCSUDIFSOpen()���صĺϷ����
//@INPUT: i64Offset = sizeof("CSUDIFSLseek_0006")+10
//@INPUT: uOrigin = CSUDIFS_SEEK_SET
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSLseek_0006", "w+")��������һ���ļ����õ����hFile1
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile1, "CSUDIFSLseek_0006", strlen("CSUDIFSLseek_0006"))д������
//@EXECUTIONFLOW: 5������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 6������CSUDIFSOpen(��������·�� + "CSUDIFSLseek_0006", "r")���ļ�
//@EXECUTIONFLOW: 7������CSUDIFSLseek(hFile1, strlen("CSUDIFSLseek_0006") + 10, CSFS_SEEK_SET)�����ļ�ָ�룬Ӧ��ʧ��
//@EXECUTIONFLOW: 8������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 9������CSUDIFSRemove(��������·�� + "CSUDIFSLseek_0006")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLseek_0006(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSLseek_0006";
	int     nLen = strlen(szTest);
	CSUDI_INT64 i64Offset;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "���������ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "д������ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "r")), "���ļ�ʧ��\n");
	
	ll_int2long(i64Offset, (nLen+10));

	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSLseek(hFile1,  i64Offset, CSUDIFS_SEEK_SET), "�ļ�ָ��Խ�粻Ӧ�óɹ�\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");
		}
	}

	return  CSUDI_TRUE;				
}
/****************************CSUDIFSLseek  End *******************************/

/****************************CSUDIFSTell  Start *****************************/ 
//@CASEGROUP: long CSUDIFSTell(CSUDI_HANDLE hFile)
//@DESCRIPTION: ������ȡ�ļ���ǰλ��
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = hFile1��hFile1ΪCSUDIFSOpen()���صĺϷ����
//@EXPECTATION: ����ֵ����strlen("CSUDIFSTell_0001")-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSTell_0001", "w+")��������һ���ļ�����þ��hFile1
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile1, "CSUDIFSTell_0001", strlen("CSUDIFSTell_0001"))д������
//@EXECUTIONFLOW: 5������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 6������CSUDIFSOpen(��������·�� + "CSUDIFSTell_0001", "r")���ļ�
//@EXECUTIONFLOW: 7������CSUDIFSSeek(hFile1, �ļ�����,  CSUDIFS_SEEK_CUR)�ƶ��ļ�ָ��
//@EXECUTIONFLOW: 8������CSUDIFSTell(hFile1)��ȡ�ļ�ָ�뵱ǰλ�ã�Ӧ������strlen("CSUDIFSTell_0001")
//@EXECUTIONFLOW: 9������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 10������CSUDIFSRemove(��������·�� + "CSUDIFSTell_0001")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSTell_0001(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSTell_0001";
	int     nLen = strlen(szTest);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "���������ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "д������ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "r")), "���ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile1,  nLen, CSUDIFS_SEEK_CUR), "�ƶ��ļ�ָ��ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSTell(hFile1), "��ȡ�ļ���ǰλ��ʧ��\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");
		}
	}

	return  CSUDI_TRUE;		
}

//@CASEGROUP: long CSUDIFSTell(CSUDI_HANDLE hFile)
//@DESCRIPTION: �ӿ��ļ������ȡ��ǰλ��
//@PRECONDITION: 
//@INPUT: hFile = CSUDI_NULL
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSTell(CSUDI_NULL)��ȡ�ļ�ָ�뵱ǰλ�ã�Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSTell_0002(void)
{
	CSTK_ASSERT_TRUE( -1 == CSUDIFSTell(CSUDI_NULL), "�ӿ��ļ������ȡ�ļ�ָ�뵱ǰλ�ò�Ӧ�óɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: long CSUDIFSTell(CSUDI_HANDLE hFile)
//@DESCRIPTION: �Ӳ����ڵ��ļ������ȡ��ǰλ��
//@PRECONDITION: 
//@INPUT: hFile = 0x7FFFFFFF(����0x7FFFFFFFΪ�����ڵ��ļ����)
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSTell(0x7FFFFFFF)��ȡ�ļ�ָ�뵱ǰλ�ã�Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSTell_0003(void)
{
	CSTK_ASSERT_TRUE( -1 == CSUDIFSTell((CSUDI_HANDLE)0x7FFFFFFF), "��ȡ�����ڵ��ļ�����ĵ�ǰλ�ò�Ӧ���ɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: long CSUDIFSTell(CSUDI_HANDLE hFile)
//@DESCRIPTION: ��ȡ����2G�Ĵ��ļ����ļ���ǰλ��
//@PRECONDITION: 1��ϵͳ�д���һ���ѹ��صĴ���2G�ķ־��豸
//@PRECONDITION: 2���ļ�ϵͳ֧��2G���ϵĴ��ļ�
//@INPUT: hFile = hFile1��hFile1ΪCSUDIFSOpen()���صĺϷ����
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSTell_0004", "w+")��������һ���ļ����õ����hFile1
//@EXECUTIONFLOW: 4��ѭ������CSUDIFSWrite(hFile1)д�볬��2G�����ݣ����ݴ����ٶȲ�һ�������ܳ���10���ӣ������ĵȴ�
//@EXECUTIONFLOW: 5������CSUDIFSTell(hFile1)��ȡ�ļ�ָ�뵱ǰλ��
//@EXECUTIONFLOW: 6������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 7������CSUDIFSRemove(��������·�� + "CSUDIFSTell_0004")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSTell_0004(void)
{
	CSUDIFSFileStat_S  sFileStat;
	//�ֶ�����
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSTell_0004";  //16�ֽ�
	//int     nLen = strlen(szTest);
	int     nMaxCount =  1024 + 50;  //ÿ��д1M�ֽڣ�1024*2��Ϊ2G��1024*2 + 50���򳬹���2G
	int nTemp = nMaxCount;
	int rateTmp = -1;
	//int     nMaxCount =  1024 * 128;
	int     nLen1 = 1024*1024 * 2;
	char *szTest1 = (char*)CSUDIOSMalloc(nLen1);
	memset(szTest1, (int)'a', nLen1);
	CSFSPrint("��ȷ��U�������Ƿ����2G�����밴(Y) / ���밴(N)\n");

	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"U������С��2G\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "���������ļ�ʧ��\n");

	CSFSPrint("���ݴ����ٶȲ�һ������������ʱ���ܳ���10���ӣ������ĵȴ�...\n");
	
	while(nTemp -- > 0)
	{
		CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIFSWrite(hFile1, szTest1, nLen1), "д������ʧ��\n");

		if((nMaxCount - nTemp) * 100 / nMaxCount > rateTmp)
		{
			rateTmp = (nMaxCount - nTemp) * 100 / nMaxCount;
			
			CSFSPrint("rate: %d%%\n",rateTmp);
		}
	}
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSFstat(hFile1, &sFileStat), "��ȡ�ļ���Ϣʧ��\n");
	CSFSPrint("�ļ���СΪ: %d M\n", sFileStat.m_lSize / 1024 /1024);
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSTell(hFile1), "��ȡ�ļ�ָ�뵱ǰλ�ò�Ӧ�óɹ�\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSFSPrint("�ر��ļ�\n");
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");
		}
	}

	return  CSUDI_TRUE;			
}
/****************************CSUDIFSTell  End *******************************/

/****************************CSUDIFSLtell  Start *****************************/ 
//@CASEGROUP: CSUDI_INT64 CSUDIFSLtell(CSUDI_HANDLE hFile)
//@DESCRIPTION: ������ȡ�ļ���ǰλ��
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = hFile1, hFile1ΪCSUDIFSOpen()���صĺϷ����
//@EXPECTATION: ����ֵ����strlen("CSUDIFSLtell_0001")-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSLtell_0001", "w+")��������һ���ļ����õ����hFile1
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile1, "CSUDIFSLtell_0001", strlen("CSUDIFSLtell_0001"))д������
//@EXECUTIONFLOW: 5��CSUDIFSFlush(hFile1)д���ļ�
///@EXECUTIONFLOW: 6������CSUDIFSSeek(hFile1, -1,  CSUDIFS_SEEK_CUR)�ƶ��ļ�ָ��
//@EXECUTIONFLOW: 7������CSUDIFSLtell(hFile1)����ļ�ָ�뵱ǰλ�ã�Ӧ��Ϊstrlen("CSUDIFSLtell_0001")-1
//@EXECUTIONFLOW: 8������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 9������CSUDIFSRemove(��������·�� + "CSUDIFSLtell_0001")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLtell_0001(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSLtell_0001";
	int     nLen = strlen(szTest);
	CSUDI_INT64  i64OffsetA;
	CSUDI_INT64  i64OffsetB;
	CSUDI_BOOL   bEqual = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "���������ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "д������ʧ��\n");
	
	CSTK_ASSERT_TRUE(CSUDI_SUCCESS== CSUDIFSFlush(hFile1), "ͬ�������̲��ɹ�\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile1,  -1, CSUDIFS_SEEK_CUR), "�ƶ��ļ�ָ��ʧ��\n");

	ll_int2long(i64OffsetA, (nLen -1));
	i64OffsetB = CSUDIFSLtell(hFile1);
	bEqual = ll_eq(i64OffsetA, i64OffsetB);

	CSTK_ASSERT_TRUE_FATAL(bEqual, "��ȡ�ļ�ָ�뵱ǰλ��ʧ��\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");
		}
	}

	return  CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_INT64 CSUDIFSLtell(CSUDI_HANDLE hFile)
//@DESCRIPTION: �ӿ��ļ������ȡ��ǰλ��
//@PRECONDITION: 
//@INPUT: hFile = CSUDI_NULL
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSLtell(CSUDI_NULL)��ȡ�ļ���ǰλ�ã�Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLtell_0002(void)
{
	CSUDI_INT64  i64OffsetA;
	CSUDI_INT64  i64OffsetB;
	CSUDI_BOOL   bEqual = CSUDI_FALSE;

	ll_int2long(i64OffsetA, (-1));
	i64OffsetB = CSUDIFSLtell(CSUDI_NULL);
	bEqual = ll_eq(i64OffsetA, i64OffsetB);
	
	CSTK_ASSERT_TRUE(bEqual, "�ӿ��ļ������ȡ�ļ�ָ�뵱ǰλ�ò�Ӧ�óɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_INT64 CSUDIFSLtell(CSUDI_HANDLE hFile)
//@DESCRIPTION: �Ӳ����ڵ��ļ������ȡ��ǰλ��
//@PRECONDITION: 
//@INPUT: hFile = 0x7FFFFFFF(����0x7FFFFFFFΪ�����ڵ��ļ����)
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSLtell(0x7FFFFFFF)��ȡ�ļ���ǰλ�ã�Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLtell_0003(void)
{
	CSUDI_INT64  i64OffsetA;
	CSUDI_INT64  i64OffsetB;
	CSUDI_BOOL   bEqual = CSUDI_FALSE;

	ll_int2long(i64OffsetA, (-1));
	i64OffsetB = CSUDIFSLtell(CSUDI_NULL);
	bEqual = ll_eq(i64OffsetA, i64OffsetB);

	CSTK_ASSERT_TRUE(bEqual, "��ȡ�����ڵ��ļ�����ĵ�ǰλ�ò�Ӧ���ɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_INT64 CSUDIFSLtell(CSUDI_HANDLE hFile)
//@DESCRIPTION: ��ȡ����2G�Ĵ��ļ����ļ���ǰλ��
//@PRECONDITION: 1��ϵͳ�д���һ���ѹ��ش���2G�ķ־��豸
//@PRECONDITION: 2���ļ�ϵͳ֧��2G���ϵĴ��ļ�
//@INPUT: hFile = hFile1, hFile1ΪCSUDIFSOpen()���صĺϷ����
//@EXPECTATION: ����ֵ����(1024*1024*128 + 1)*strlen("CSUDIFSLtell_0004")
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSLtell_0004", "w+")��������һ���ļ����õ����hFile1
//@EXECUTIONFLOW: 4��ѭ������CSUDIFSWrite(hFile1, "CSUDIFSLtell_0004", strlen("CSUDIFSLtell_0004"))nMaxCount�Σ�nMaxCount = 1024*1024*128 + 1
//@EXECUTIONFLOW: 5������CSUDIFSLtell(hFile1)����ļ���ǰλ�ã�Ӧ������(1024*1024*128 + 1)*strlen("CSUDIFSLtell_0004")
//@EXECUTIONFLOW: 6������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 7������CSUDIFSRemove(��������·�� + "CSUDIFSLtell_0004")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLtell_0004(void)
{
	//�ֶ�����
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSLtell_0004";  //17�ֽ�
	int     nLen = 1024*1024*2;
	char *szTest1 = (char*)CSUDIOSMalloc(nLen);
	int     nMaxCount =  1024+50;  //ÿ��д2M �ֽڣ�1024+50���򳬹���2G
	int     nTemp = nMaxCount;
	int rateTmp = -1;
	CSUDI_INT64  i64OffsetA;
	CSUDI_INT64  i64OffsetB;
	CSUDI_BOOL   bEqual = CSUDI_FALSE;

	memset(szTest1, 1, nLen);

	CSFSPrint("��ȷ��U�������Ƿ����2G�����밴(Y)/���밴(N)\n");

	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"U������С��2G\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "���������ļ�ʧ��\n");

	CSFSPrint("���ݴ����ٶȲ�һ������������ʱ���ܳ���10���ӣ������ĵȴ�...\n");
	
	while(nTemp -- > 0)
	{
		CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest1, nLen), "д������ʧ��\n");

		if((nMaxCount - nTemp) * 100 / nMaxCount > rateTmp)
		{
			rateTmp = (nMaxCount - nTemp) * 100 / nMaxCount;
			
			CSFSPrint("rate: %d%%\n",rateTmp);
		}
	}

	//ll_int2long(i64OffsetA, (nLen * nMaxCount));
	ll_uint2long(i64OffsetA, ((unsigned long)nLen * (unsigned long)nMaxCount));
	i64OffsetB = CSUDIFSLtell(hFile1);
	bEqual = ll_eq(i64OffsetA, i64OffsetB);
	
	CSTK_ASSERT_TRUE_FATAL(bEqual, "��ȡ�ļ���ǰλ��ʧ��\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");
		}
	}

	return  CSUDI_TRUE;
}
/****************************CSUDIFSLtell  End *******************************/

/****************************CSUDIFSFlush  Start *****************************/ 
//@CASEGROUP: int CSUDIFSFlush(CSUDI_HANDLE hFile)
//@DESCRIPTION: ����flush�ļ�
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = hFile1, hFile1ΪCSUDIFSOpen()���صĺϷ����
//@EXPECTATION: ����ֵ����0
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��ֱ	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSFlush_0001", "w+")��������һ���ļ����õ����hFile1
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile1, "CSUDIFSFlush_0001", strlen("CSUDIFSFlush_0001"))д������
//@EXECUTIONFLOW: 5������CSUDIFSFlush(hFile1)ͬ�������̣�Ӧ���ɹ�
//@EXECUTIONFLOW: 6������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 7������CSUDIFSRemove(��������·�� + "CSUDIFSFlush_0001")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFlush_0001(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSFlush_0001";  
	int     nLen = strlen(szTest);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "���������ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "д������ʧ��\n");
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSFlush(hFile1), "ͬ��������ʧ��\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");
		}
	}

	return  CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSFlush(CSUDI_HANDLE hFile)
//@DESCRIPTION: flush���ļ����
//@PRECONDITION: 
//@INPUT: hFile = CSUDI_NULL
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSFlush(CSUDI_NULL)ͬ�������̣�Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFlush_0002(void)
{
	CSTK_ASSERT_TRUE(-1 == CSUDIFSFlush(CSUDI_NULL), "ͬ�������̲�Ӧ�óɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSFlush(CSUDI_HANDLE hFile)
//@DESCRIPTION: flush�����ڵ��ļ����
//@PRECONDITION: 
//@INPUT: hFile = 0x7FFFFFFF(����0x7FFFFFFFΪ�����ڵ��ļ����)
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSFlush(0x7FFFFFFF)ͬ�������̣�Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFlush_0003(void)
{
	CSTK_ASSERT_TRUE(-1 == CSUDIFSFlush((CSUDI_HANDLE)0x7FFFFFFF), "ͬ�������̲�Ӧ�óɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}
/****************************CSUDIFSFlush  End *******************************/

/****************************CSUDIFSRemove  Start *****************************/ 
//@CASEGROUP: int CSUDIFSRemove(const char * pcFileName)
//@DESCRIPTION: �Ƴ�һ���Ѵ��ڵ��ļ�
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcFileName = ��������·�� + "CSUDIFSRemove_0001"
//@EXPECTATION: ����ֵ����0
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSRemove_0001", "w")����һ���ļ������õ����hFile1
//@EXECUTIONFLOW: 4������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 5������CSUDIFSRemove(��������·�� + "CSUDIFSRemove_0001")ɾ���ļ�
//@EXECUTIONFLOW: 6������CSUDIFSOpen(��������·�� + "CSUDIFSRemove_0001", "r")��Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRemove_0001(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSRemove_0001";  
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w")), "���������ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ�ʧ��\n");

			CSTK_ASSERT_TRUE(CSUDI_NULL == CSUDIFSOpen(g_szVolumeMountPoint, "r"), "�򿪲����ڵ��ļ���Ӧ�óɹ�\n");
		}
	}

	return  CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSRemove(const char * pcFileName)
//@DESCRIPTION: �Ƴ������ڵ��ļ�
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcFileName = ��������·�� + "CSUDIFSRemove_0002"
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSRemove(��������·�� + "CSUDIFSRemove_0002")ɾ���ļ���Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRemove_0002(void)
{
	char   szTest[] = "CSUDIFSRemove_0002";  
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE(-1 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ������ڵ��ļ���Ӧ�óɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	
	return  CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSRemove(const char * pcFileName)
//@DESCRIPTION: �Ƴ��ļ���
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcFileName = ��������·�� + "CSUDIFSRemove_0003"
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·�� 
//@EXECUTIONFLOW: 3������CSUDIFSMkdir(��������·�� + "CSUDIFSRemove_0003", 0)�����ļ���
//@EXECUTIONFLOW: 4������CSUDIFSRemove(��������·�� + "CSUDIFSRemove_0003")ɾ���ļ��У�Ӧ��ʧ��
//@EXECUTIONFLOW: 5������CSUDIFSRmdir(��������·�� + "CSUDIFSRemove_0003")ɾ���ļ��У�Ӧ���ɹ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRemove_0003(void)
{
	char   szTest[] = "CSUDIFSRemove_0003";  
	int     nMkDirRet = -1;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(0 == (nMkDirRet = CSUDIFSMkdir(g_szVolumeMountPoint, 0)), "�����ļ���ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(-1 ==CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ��ļ��в�Ӧ�óɹ�\n");

	CSTK_FATAL_POINT
	{
		if(nMkDirRet == 0)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSRmdir(g_szVolumeMountPoint), "ɾ���ļ���ʧ��\n");
		}
	}
	
	return  CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSRemove(const char * pcFileName)
//@DESCRIPTION: �Ƴ���ָ��
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcFileName = CSUDI_NULL
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSRemove(CSUDI_NULL)ɾ���ļ���Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRemove_0004(void)
{
	CSTK_ASSERT_TRUE(-1 ==CSUDIFSRemove(CSUDI_NULL), "�Ƴ������ڵ��ļ���Ӧ�óɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}
/****************************CSUDIFSRemove  End *******************************/

/****************************CSUDIFSRemove  Start *****************************/ 
//@CASEGROUP: int CSUDIFSRename(const char * pcOldPath, const char * pcNewPath)
//@DESCRIPTION:  ͬĿ¼���������ļ�
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcOldPath = ��������·�� + "CSUDIFSRename_0001a"
//@INPUT: pcNewPath = ��������·�� + "CSUDIFSRename_0001b"
//@EXPECTATION: ����ֵ����0
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSRename_0001a", "w")���������ļ����õ����hFileA
//@EXECUTIONFLOW: 4������CSUDIFSClose(hFileA)�ر��ļ�
//@EXECUTIONFLOW: 5������CSUDIFSRename(��������·�� + "CSUDIFSRename_0001a", ��������·�� + "CSUDIFSRename_0001b")�������ļ�
//@EXECUTIONFLOW: 6������CSUDIFSOpen(��������·�� + "CSUDIFSRename_0001a", "r")���ļ���Ӧ��ʧ��
//@EXECUTIONFLOW: 7������CSUDIFSOpen(��������·�� + "CSUDIFSRename_0001b", "r")���ļ����õ����hFileB
//@EXECUTIONFLOW: 8������CSUDIFSClose(hFileB)�ر��ļ�
//@EXECUTIONFLOW: 9������CSUDIFSRemove(��������·�� + "CSUDIFSRename_0001b")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRename_0001(void)
{
	char   szTestA[] = "CSUDIFSRename_0001a";  
	char   szTestB[] = "CSUDIFSRename_0001b";  
	CSUDI_HANDLE    hFileA = CSUDI_NULL;
	CSUDI_HANDLE    hFileB = CSUDI_NULL;
	char   szFilePathA[CSUDI_MAX_FILE_NAME_LEN];
	char   szFilePathB[CSUDI_MAX_FILE_NAME_LEN];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePathA, 0, CSUDI_MAX_FILE_NAME_LEN);
	memset(szFilePathB, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePathA, g_szVolumeMountPoint);
	strcpy(szFilePathB, g_szVolumeMountPoint);
	strcat(szFilePathA, szTestA);
	strcat(szFilePathB, szTestB);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFileA = CSUDIFSOpen(szFilePathA, "w")), "���������ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFileA), "�ر��ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRename(szFilePathA, szFilePathB), "�������ļ�ʧ��\n");

	hFileA = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == CSUDIFSOpen(szFilePathA, "r"), "���ļ���Ӧ�óɹ�\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFileB = CSUDIFSOpen(szFilePathB, "r")) , "���ļ�ʧ��\n");
	CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFileB), "�ر��ļ�ʧ��\n");
	CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePathB), "�Ƴ��ļ�ʧ��\n");
	
	CSTK_FATAL_POINT
	{
		if(hFileA != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePathA), "�Ƴ��ļ�ʧ��\n");
		}
	}
	
	return  CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSRename(const char * pcOldPath, const char * pcNewPath)
//@DESCRIPTION:  �����������ڵ��ļ�
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcOldPath = ��������·�� + "CSUDIFSRename_0002a"
//@INPUT: pcNewPath = ��������·�� + "CSUDIFSRename_0002b"
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSRename(��������·�� + "CSUDIFSRename_0002a", ��������·�� + "CSUDIFSRename_0002b")�������ļ���Ӧ��ʧ��
//@EXECUTIONFLOW: 4������CSUDIFSOpen(��������·�� + "CSUDIFSRename_0002a", "r")���ļ���Ӧ��ʧ��
//@EXECUTIONFLOW: 5������CSUDIFSOpen(��������·�� + "CSUDIFSRename_0002b", "r")���ļ���Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRename_0002(void)
{
	char   szTestA[] = "CSUDIFSRename_0002a";  
	char   szTestB[] = "CSUDIFSRename_0002b";  
	char   szFilePathA[CSUDI_MAX_FILE_NAME_LEN];
	char   szFilePathB[CSUDI_MAX_FILE_NAME_LEN];
	
	CSTK_ASSERT_TRUE(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePathA, 0, CSUDI_MAX_FILE_NAME_LEN);
	memset(szFilePathB, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePathA, g_szVolumeMountPoint);
	strcpy(szFilePathB, g_szVolumeMountPoint);
	strcat(szFilePathA, szTestA);
	strcat(szFilePathB, szTestB);	

	CSTK_ASSERT_TRUE(-1 == CSUDIFSRename(szFilePathA, szFilePathB), "�����������ڵ��ļ���Ӧ�óɹ�\n");

	CSTK_ASSERT_TRUE(CSUDI_NULL == CSUDIFSOpen(szFilePathA, "r"), "���ļ���Ӧ�óɹ�\n");

	CSTK_ASSERT_TRUE(CSUDI_NULL == CSUDIFSOpen(szFilePathB, "r"), "���ļ���Ӧ�óɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSRename(const char * pcOldPath, const char * pcNewPath)
//@DESCRIPTION:  ��������ָ��
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcOldPath = CSUDI_NULL
//@INPUT: pcNewPath = ��������·�� + "CSUDIFSRename_0003"
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSRename(CSUDI_NULL, ��������·�� + "CSUDIFSRename_0003")�������ļ���Ӧ��ʧ��
//@EXECUTIONFLOW: 4������CSUDIFSOpen(��������·�� + "CSUDIFSRename_0003", "r")���ļ���Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRename_0003(void)
{
	char   szTest[] = "CSUDIFSRename_0003";  
	
	CSTK_ASSERT_TRUE(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE(-1 == CSUDIFSRename(CSUDI_NULL, g_szVolumeMountPoint), "�����������ڵ��ļ���Ӧ�óɹ�\n");

	CSTK_ASSERT_TRUE(CSUDI_NULL == CSUDIFSOpen(g_szVolumeMountPoint, "r"), "���ļ���Ӧ�óɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSRename(const char * pcOldPath, const char * pcNewPath)
//@DESCRIPTION:  �������ļ���
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcOldPath = ��������·�� + "CSUDIFSRename_0004a"
//@INPUT: pcNewPath = ��������·�� + "CSUDIFSRename_0004b"
//@EXPECTATION: ����ֵ����0
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSMkdir(��������·�� + "CSUDIFSRename_0004a", 0)����Ŀ¼
//@EXECUTIONFLOW: 4������CSUDIFSRename(��������·�� + "CSUDIFSRename_0004a", ��������·�� + "CSUDIFSRename_0004b")  
//@EXECUTIONFLOW: 5������CSUDIFSRmdir(��������·�� + "CSUDIFSRename_0004b")ɾ��Ŀ¼,Ӧ�óɹ�
//@EXECUTIONFLOW: 6������CSUDIFSRmdir(��������·�� + "CSUDIFSRename_0004a")ɾ��Ŀ¼��Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRename_0004(void)
{
	char   szTestA[] = "CSUDIFSRename_0004a";  
	char   szTestB[] = "CSUDIFSRename_0004b";  
	char   szFilePathA[CSUDI_MAX_FILE_NAME_LEN];
	char   szFilePathB[CSUDI_MAX_FILE_NAME_LEN];
	
	CSTK_ASSERT_TRUE(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePathA, 0, CSUDI_MAX_FILE_NAME_LEN);
	memset(szFilePathB, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePathA, g_szVolumeMountPoint);
	strcpy(szFilePathB, g_szVolumeMountPoint);
	strcat(szFilePathA, szTestA);
	strcat(szFilePathB, szTestB);

	CSTK_ASSERT_TRUE(0 == CSUDIFSMkdir(szFilePathA, 0), "����Ŀ¼ʧ��\n");
	CSTK_ASSERT_TRUE(0 == CSUDIFSRename(szFilePathA, szFilePathB), "�������ļ���ʧ��\n");
	CSTK_ASSERT_TRUE(0 == CSUDIFSRmdir(szFilePathB), "ɾ���ļ���ʧ��\n");
	CSTK_ASSERT_TRUE(0 != CSUDIFSRmdir(szFilePathA), "ɾ���ļ��в�Ӧ�óɹ�\n");
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSRename(const char * pcOldPath, const char * pcNewPath)
//@DESCRIPTION:  �������ļ�����һĿ¼��ԭĿ¼���ļ���ɾ��
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcOldPath = ��������·�� + "CSUDIFSRename_0005a"
//@INPUT: pcNewPath = ��������·�� + "CSUDIFSRename_0005_dir"+"CSUDIFSRename_0005b"
//@EXPECTATION: ����ֵ����0
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSRename_0005", "w")���������ļ����õ����hFile1
//@EXECUTIONFLOW: 4������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 5������CSUDIFSMkdir(��������·�� + "CSUDIFSRename_0005_dir", 0)����Ŀ¼
//@EXECUTIONFLOW: 6������CSUDIFSRename(��������·�� + "CSUDIFSRename_0005", ��������·�� + "CSUDIFSRename_0005_dir"+"CSUDIFSRename_0005")��Ӧ���ɹ�  
//@EXECUTIONFLOW: 7������CSUDIFSRemove(��������·�� + "CSUDIFSRename_0005_dir"+"CSUDIFSRename_0005")ɾ���ļ�,Ӧ���ɹ�
//@EXECUTIONFLOW: 8������CSUDIFSRemove(��������·�� + "CSUDIFSRename_0005")ɾ���ļ�,Ӧ��ʧ��
//@EXECUTIONFLOW: 9������CSUDIFSRmdir(��������·�� + "CSUDIFSRename_0005_dir")ɾ��Ŀ¼
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRename_0005(void)
{
	char   szTestA[] = "CSUDIFSRename_0004a";  
	char   szTestB[] = "CSUDIFSRename_0004b";  
	char   szFilePathA[CSUDI_MAX_FILE_NAME_LEN];
	char   szFilePathB[CSUDI_MAX_FILE_NAME_LEN];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePathA, 0, CSUDI_MAX_FILE_NAME_LEN);
	memset(szFilePathB, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePathA, g_szVolumeMountPoint);
	strcpy(szFilePathB, g_szVolumeMountPoint);
	strcat(szFilePathA, szTestA);
	strcat(szFilePathB, szTestB);

	CSTK_ASSERT_TRUE(0 == CSUDIFSMkdir(szFilePathA, 0), "����Ŀ¼ʧ��\n");

	CSTK_ASSERT_TRUE(0 == CSUDIFSRename(szFilePathA, szFilePathB), "�������ļ���ʧ��\n");

	CSTK_ASSERT_TRUE(0 == CSUDIFSRmdir(szFilePathB), "ɾ���ļ���ʧ��\n");

	CSTK_ASSERT_TRUE(0 != CSUDIFSRmdir(szFilePathA), "ɾ���ļ��в�Ӧ�óɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSRename(const char * pcOldPath, const char * pcNewPath)
//@DESCRIPTION:  �������ļ�������Ŀ¼������
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcOldPath = ��������·�� + "CSUDIFSRename_0006a"
//@INPUT: pcNewPath = ��������·�� + "CSUDIFSRename_0006_dir"+"CSUDIFSRename_0006b"
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSRename_0006a", "w")���������ļ��ļ����õ��ļ����hFile
//@EXECUTIONFLOW: 4������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 5������CSUDIFSRename(��������·�� + "CSUDIFSRename_0006a", ��������·�� + "CSUDIFSRename_0006_dir"+"CSUDIFSRename_0006b")��������Ӧ��ʧ��  
//@EXECUTIONFLOW: 6������CSUDIFSOpen(��������·�� + "CSUDIFSRename_0006_dir"+"CSUDIFSRename_0006b")���ļ���Ӧ��ʧ��
//@EXECUTIONFLOW: 7������CSUDIFSRemove(��������·�� + "CSUDIFSRename_0006a")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRename_0006(void)
{
	char   szTestA[] = "CSUDIFSRename_0004a";  
	char   szTestB[] = "CSUDIFSRename_0006_dir/CSUDIFSRename_0004b";  
	char   szFilePathA[CSUDI_MAX_FILE_NAME_LEN];
	char   szFilePathB[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE  hFile = CSUDI_NULL;  
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePathA, 0, CSUDI_MAX_FILE_NAME_LEN);
	memset(szFilePathB, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePathA, g_szVolumeMountPoint);
	strcpy(szFilePathB, g_szVolumeMountPoint);
	strcat(szFilePathA, szTestA);
	strcat(szFilePathB, szTestB);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePathA, "w")), "���������ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSRename(szFilePathA, szFilePathB), "�������ļ���Ӧ�óɹ�\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == CSUDIFSOpen(szFilePathB, "r"), "���ļ���Ӧ�óɹ�\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePathA), "�Ƴ��ļ�ʧ��\n");
		}
	}
	
	return  CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSRename(const char * pcOldPath, const char * pcNewPath)
//@DESCRIPTION:  ������Ϊ��ָ��
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcOldPath = ��������·�� + "CSUDIFSRename_0007"
//@INPUT: pcNewPath = CSUDI_NULL
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSRename_0007", "w")���������ļ�����þ��hFile
//@EXECUTIONFLOW: 4������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 5������CSUDIFSRename(��������·�� + "CSUDIFSRename_0007", CSUDI_NULL)�������ļ���Ӧ��ʧ��
//@EXECUTIONFLOW: 6������CSUDIFSRemove(��������·�� + "CSUDIFSRename_0007")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRename_0007(void)
{
	char   szTest[] = "CSUDIFSRename_0007";  
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE  hFile = CSUDI_NULL;  
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w")), "���������ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSRename(szFilePath, CSUDI_NULL), "�������ļ���Ӧ�óɹ�\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "�Ƴ��ļ�ʧ��\n");
		}
	}
	
	return  CSUDI_TRUE;		
}

//@CASEGROUP: int CSUDIFSRename(const char * pcOldPath, const char * pcNewPath)
//@DESCRIPTION:  ������Ϊ���ַ���
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcOldPath = ��������·�� + "CSUDIFSRename_0008"
//@INPUT: pcNewPath = ��������·�� + ""
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSRename_0008", "w")���������ļ����õ����hFile
//@EXECUTIONFLOW: 4������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 5������CSUDIFSRename(��������·�� + "CSUDIFSRename_0008", ��������·�� + "")�������ļ�Ӧ��ʧ��
//@EXECUTIONFLOW: 6������CSUDIFSRemove(��������·�� + "CSUDIFSRename_0008"ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRename_0008(void)
{
	char   szTest[] = "CSUDIFSRename_0008";  
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE  hFile = CSUDI_NULL;  
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w")), "���������ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSRename(szFilePath, g_szVolumeMountPoint), "�������ļ���Ӧ�óɹ�\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "�Ƴ��ļ�ʧ��\n");
		}
	}
	
	return  CSUDI_TRUE;		
}

//@CASEGROUP: int CSUDIFSRename(const char * pcOldPath, const char * pcNewPath)
//@DESCRIPTION:  ���������ļ�����ӦĿ¼���Ѿ�����
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcOldPath = ��������·�� + "CSUDIFSRename_0009a"
//@INPUT: pcNewPath = ��������·�� + "CSUDIFSRename_0009b"
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��
//@EXECUTIONFLOW: 3������CSUDIFSOpen(pcOldPath, "w")���������ļ����õ����hFile1
//@EXECUTIONFLOW: 4������CSUDIFSOpen(pcNewPath, "w")���������ļ����õ����hFile2
//@EXECUTIONFLOW: 5������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 6������CSUDIFSClose(hFile2)�ر��ļ�
//@EXECUTIONFLOW: 7������CSUDIFSRename(��������·�� + "CSUDIFSRename_0009a", ��������·�� + "CSUDIFSRename_0009b")��Ӧ��ʧ��  
//@EXECUTIONFLOW: 8������CSUDIFSRemove(��������·�� + "CSUDIFSRename_0009a")ɾ���ļ�
//@EXECUTIONFLOW: 9������CSUDIFSRemove(��������·�� + "CSUDIFSRename_0009b")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRename_0009(void)
{
	char   szTestA[] = "CSUDIFSRename_0009a";  
	char   szTestB[] = "CSUDIFSRename_0009b";  
	char   szFilePathA[CSUDI_MAX_FILE_NAME_LEN];
	char   szFilePathB[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE  hFile1 = CSUDI_NULL;  
	CSUDI_HANDLE  hFile2 = CSUDI_NULL;  
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePathA, 0, CSUDI_MAX_FILE_NAME_LEN);
	memset(szFilePathB, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePathA, g_szVolumeMountPoint);
	strcpy(szFilePathB, g_szVolumeMountPoint);
	strcat(szFilePathA, szTestA);
	strcat(szFilePathB, szTestB);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(szFilePathA, "w")), "���������ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile2 = CSUDIFSOpen(szFilePathB, "w")), "���������ļ�ʧ��\n");
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile2), "�ر��ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSRename(szFilePathA, szFilePathB), "�������ļ���Ӧ�óɹ�\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePathA), "�Ƴ��ļ�ʧ��\n");
		}
		if(hFile2 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePathB), "�Ƴ��ļ�ʧ��\n");
		}
	}
	
	return  CSUDI_TRUE;		
}
/****************************CSUDIFSRemove  End *******************************/

/****************************CSUDIFSTruncate  Start *****************************/ 
//@CASEGROUP: int CSUDIFSTruncate(const char * pcPathName, unsigned long ulLength)
//@DESCRIPTION:  ���ļ���С
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcPathName = ��������·�� + "CSUDIFSTruncate_0001"
//@INPUT: ulLength = ulLength1, ����ulLength1С���ļ���С1
//@EXPECTATION: ����ֵ����0�����ļ���С��ΪulLength1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSTruncate_0001", "w+")���������ļ����õ����hFile
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile, "CSUDIFSTruncate_0001", strlen("CSUDIFSTruncate_0001"))д������
//@EXECUTIONFLOW: 5������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 6������CSUDIFSTruncate(��������·�� + "CSUDIFSTruncate_0001", strlen("CSUDIFSTruncate_0001")/2)��С�ļ�
//@EXECUTIONFLOW: 7������CSUDIFSStat(��������·�� + "CSUDIFSTruncate_0001", psFileStat)����ļ���С����ulLength1�Ƚϣ�Ӧ�����
//@EXECUTIONFLOW: 8������CSUDIFSRemove(��������·�� + "CSUDIFSTruncate_0001")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSTruncate_0001(void)
{
	char   szTest[] = "CSUDIFSTruncate_0001";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE  hFile = CSUDI_NULL;  
	int  nLen = strlen(szTest);
	int  ulLength1 = nLen/2;
	CSUDIFSFileStat_S   sFileStat; 

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "���������ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "д���ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSTruncate(szFilePath, ulLength1), "�ض��ļ�ʧ��\n");	

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStat(szFilePath, &sFileStat), "��ȡ�ļ���Ϣʧ��\n");

	CSTK_ASSERT_TRUE_FATAL((int)sFileStat.m_lSize == ulLength1, "�ضϺ��ļ���С����\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "�Ƴ��ļ�ʧ��\n");
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSTruncate(const char * pcPathName, unsigned long ulLength)
//@DESCRIPTION:  ���ļ��ش�
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcPathName = ��������·�� + "CSUDIFSTruncate_0002"
//@INPUT: ulLength = ulLength1������ulLength1��С�ļ�ԭ��С
//@EXPECTATION: ����ֵ����0
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSTruncate_0002", "w+")���������ļ����õ����hFile
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile, "CSUDIFSTruncate_0002", strlen("CSUDIFSTruncate_0002"))
//@EXECUTIONFLOW: 5������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 6������CSUDIFSTruncate(��������·�� + "CSUDIFSTruncate_0002", strlen("this is a test")*2)���ļ��ش�
//@EXECUTIONFLOW: 7������CSUDIFSStat(��������·�� + "CSUDIFSTruncate_0001", psFileStat)����ļ���С����ulLength1�Ƚϣ�Ӧ�����
//@EXECUTIONFLOW: 8������CSUDIFSRemove(��������·�� + "CSUDIFSTruncate_0002")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSTruncate_0002(void)
{
	char   szTest[] = "CSUDIFSTruncate_0002";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE  hFile = CSUDI_NULL;  
	int  nLen = strlen(szTest);
	int  ulLength1 = nLen*2;
	CSUDIFSFileStat_S   sFileStat; 

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "���������ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "д���ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSTruncate(szFilePath, ulLength1), "�ض��ļ�ʧ��\n");	

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStat(szFilePath, &sFileStat), "��ȡ�ļ���Ϣʧ��\n");

	CSTK_ASSERT_TRUE_FATAL((int)sFileStat.m_lSize == ulLength1, "�ضϺ��ļ���С����\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "�Ƴ��ļ�ʧ��\n");
		}
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSTruncate(const char * pcPathName, unsigned long ulLength)
//@DESCRIPTION:  ���ļ���
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcPathName = ��������·�� + "CSUDIFSTruncate_0003"
//@INPUT: ulLength = 10
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSMkdir(��������·�� + "CSUDIFSTruncate_0003", 0)�����ļ���
//@EXECUTIONFLOW: 4������CSUDIFSTruncate(��������·�� + "CSUDIFSTruncate_0003", 10)�ض��ļ��У�Ӧ��ʧ��
//@EXECUTIONFLOW: 5������CSUDIFSRmdir(��������·�� + "CSUDIFSTruncate_0003")ɾ���ļ���
CSUDI_BOOL CSTC_FS_IT_CSUDIFSTruncate_0003(void)
{
	char   szTest[] = "CSUDIFSTruncate_0003";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSMkdir(szFilePath, 0), "����Ŀ¼ʧ��\n");

	CSTK_ASSERT_TRUE(-1 == CSUDIFSTruncate(szFilePath, 10), "�ض��ļ��в�Ӧ�óɹ�\n");	

	CSTK_ASSERT_TRUE(0 == CSUDIFSRmdir(szFilePath), "ɾ���ļ���ʧ��\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSTruncate(const char * pcPathName, unsigned long ulLength)
//@DESCRIPTION: �ؿ�ָ��
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcPathName = CSUDI_NULL
//@INPUT: ulLength = 10
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSTruncate(CSUDI_NULL, 10)���ļ���Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSTruncate_0004(void)
{
	CSTK_ASSERT_TRUE(-1 == CSUDIFSTruncate(CSUDI_NULL, 10), "�ضϿ�ָ�벻Ӧ�óɹ�\n");	
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSTruncate(const char * pcPathName, unsigned long ulLength)
//@DESCRIPTION:  �ز����ڵ��ļ�������·�����Ϸ��������ڡ��Ƿ��ַ��ȣ�
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcPathName = ��������·�� + "CSUDIFSTruncate_0005"
//@INPUT: ulLength = 10
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSTruncate(��������·�� + "CSUDIFSTruncate_0005", 10)���ļ���Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSTruncate_0005(void)
{
	char   szTest[] = "CSUDIFSTruncate_0005";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE(-1 == CSUDIFSTruncate(szFilePath, 10), "�ضϲ����ڵ��ļ���Ӧ�óɹ�\n");	

	CSTK_FATAL_POINT
	{
		;
	}
	
	return CSUDI_TRUE;	
}
/****************************CSUDIFSTruncate  End *******************************/

/****************************CSUDIFSFTruncate  Start *****************************/ 
//@CASEGROUP: int CSUDIFSFTruncate(CSUDI_HANDLE hFile, unsigned long ulLength)
//@DESCRIPTION:  ���ļ���С
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile Ϊ�Ϸ����ļ��������CSUDIFSOpen()����
//@INPUT: ulLength = ulLength1��ulLength1Ϊ�ļ�ԭ��С
//@EXPECTATION: ����ֵ����0
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSFTruncate_0001", "w+")���������ļ����õ��ļ����hFile
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile, "CSUDIFSFTruncate_0001", strlen("CSUDIFSFTruncate_0001"))д������
//@EXECUTIONFLOW: 5������CSUDIFSFTruncate(hFile, strlen("CSUDIFSFTruncate_0001")/2)��С�ļ�
//@EXECUTIONFLOW: 6������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 7������CSUDIFSStat(��������·�� + "CSUDIFSTruncate_0001", psFileStat)����ļ���С����ulLength1�Ƚϣ�Ӧ�����
//@EXECUTIONFLOW: 8������CSUDIFSRemove(��������·�� + "CSUDIFSFTruncate_0001")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFTruncate_0001(void)
{
	char   szTest[] = "CSUDIFSFTruncate_0001";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE  hFile = CSUDI_NULL;  
	int  nLen = strlen(szTest);
	int  ulLength1 = nLen/2;
	CSUDIFSFileStat_S   sFileStat; 

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "���������ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "д���ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSFTruncate(hFile, ulLength1), "�ض��ļ�ʧ��\n");
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStat(szFilePath, &sFileStat), "��ȡ�ļ���Ϣʧ��\n");

	CSTK_ASSERT_TRUE_FATAL((int)sFileStat.m_lSize == ulLength1, "�ضϺ��ļ���С����\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");	

			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "�Ƴ��ļ�ʧ��\n");
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSFTruncate(CSUDI_HANDLE hFile, unsigned long ulLength)
//@DESCRIPTION:  ���ļ��ض�
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile Ϊ�Ϸ����ļ��������CSUDIFSOpen()����
//@INPUT: ulLength = ulLength1��ulLength�����ļ�ԭ��С
//@EXPECTATION: ����ֵ����0
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSFTruncate_0002", "w")���������ļ����õ����hFile
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile, "this is a test", strlen("this is a test"))д������
//@EXECUTIONFLOW: 5������CSUDIFSFTruncate(hFile, strlen("CSUDIFSFTruncate_0002")*2)�ض��ļ�
//@EXECUTIONFLOW: 6������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 7������CSUDIFSStat(��������·�� + "CSUDIFSFTruncate_0002", psFileStat)����ļ���С����ulLength1�Ƚϣ�Ӧ�����
//@EXECUTIONFLOW: 8������CSUDIFSRemove(��������·�� + "CSUDIFSFTruncate_0002")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFTruncate_0002(void)
{
	char   szTest[] = "CSUDIFSFTruncate_0002";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE  hFile = CSUDI_NULL;  
	int  nLen = strlen(szTest);
	int  ulLength1 = nLen*2;
	CSUDIFSFileStat_S   sFileStat; 

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "���������ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "д���ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSFTruncate(hFile, ulLength1), "�ض��ļ�ʧ��\n");
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStat(szFilePath, &sFileStat), "��ȡ�ļ���Ϣʧ��\n");

	CSTK_ASSERT_TRUE_FATAL((int)sFileStat.m_lSize == ulLength1, "�ضϺ��ļ���С����\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");	
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "�Ƴ��ļ�ʧ��\n");
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSFTruncate(CSUDI_HANDLE hFile, unsigned long ulLength)
//@DESCRIPTION:  �ؿ��ļ����
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = CSUDI_NULL
//@INPUT: ulLength = 10 
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSFTruncate(CSUDI_NULL, 10)���ļ���Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFTruncate_0003(void)
{
	CSTK_ASSERT_TRUE(-1 == CSUDIFSFTruncate(CSUDI_NULL, 10), "�ضϿվ����Ӧ�óɹ�\n");	
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSFTruncate(CSUDI_HANDLE hFile, unsigned long ulLength)
//@DESCRIPTION:  �ز����ڵľ��
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = 0x7FFFFFFF(����0x7FFFFFFFΪ�����ڵľ��)
//@INPUT: ulLength = 10
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSFTruncate(hFile, 10)���ļ���Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFTruncate_0004(void)
{
	CSTK_ASSERT_TRUE(-1 == CSUDIFSFTruncate((CSUDI_HANDLE)0x7FFFFFFF, 10), "�ضϲ����ڵ��ļ����Ӧ�óɹ�\n");	
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSFTruncate(CSUDI_HANDLE hFile, unsigned long ulLength)
//@DESCRIPTION:  ���ļ���
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile �ļ��о������CSUDIFSMkdir()����
//@INPUT: ulLength = 10 
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSMkdir(��������·�� + "CSUDIFSFTruncate_0005", 0)�����ļ���
//@EXECUTIONFLOW: 4������CSUDIFSOpenDir(��������·�� + "CSUDIFSFTruncate_0005")���ļ��У��õ����hDir
//@EXECUTIONFLOW: 5������CSUDIFSFTruncate(hDir, 10)�ض��ļ���
//@EXECUTIONFLOW: 6������CSUDIFSCloseDir(hDir)�ر��ļ���
//@EXECUTIONFLOW: 7������CSUDIFSRmdir(��������·�� + "CSUDIFSFTruncate_0005")ɾ���ļ���
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFTruncate_0005(void)
{
	char   szTest[] = "CSUDIFSFTruncate_0005";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE  hDir = CSUDI_NULL;  

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSMkdir(szFilePath, 0), "�����ļ���ʧ��\n");

	CSTK_ASSERT_TRUE(CSUDI_NULL != (hDir = CSUDIFSOpenDir(szFilePath)), "���ļ���ʧ��\n");

	CSTK_ASSERT_TRUE(-1 == CSUDIFSFTruncate(hDir, 10), "�ض��ļ��в�Ӧ�óɹ�\n");	

	CSTK_FATAL_POINT
	{
		CSTK_ASSERT_TRUE(0 == CSUDIFSCloseDir(hDir), "�ر��ļ���ʧ��\n");

		CSTK_ASSERT_TRUE(0 == CSUDIFSRmdir(szFilePath), "ɾ���ļ���ʧ��\n");

	}
	return CSUDI_TRUE;		
}
/****************************CSUDIFSFTruncate  End *******************************/

/****************************CSUDIFSLTruncate  Start *****************************/ 
//@CASEGROUP: int CSUDIFSLTruncate(const char * pcPathName,CSUDI_UINT64 u64Length)
//@DESCRIPTION:  ���ļ���С
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcPathName = ��������·�� + "CSUDIFSLTruncate_0001"
//@INPUT: u64Length = u64Length1��u64Length1С���ļ�ԭ��С
//@EXPECTATION: ����ֵ����0
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSLTruncate_0001", "w+")���������ļ����õ����hFile
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile, "CSUDIFSLTruncate_0001", strlen("CSUDIFSLTruncate_0001"))д������
//@EXECUTIONFLOW: 5������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 6������CSUDIFSLTruncate(��������·�� + "CSUDIFSLTruncate_0001", strlen("CSUDIFSLTruncate_0001")/2)��С�ļ���Ӧ���ɹ�
//@EXECUTIONFLOW: 7������CSUDIFSStat(��������·�� + "CSUDIFSLTruncate_0001", psFileStat)����ļ���С����u64Length1�Ƚϣ�Ӧ�����
//@EXECUTIONFLOW: 8������CSUDIFSRemove(��������·�� + "CSUDIFSLTruncate_0001")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLTruncate_0001(void)
{
	char   szTest[] = "CSUDIFSLTruncate_0001";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE  hFile = CSUDI_NULL;  
	int  nLen = strlen(szTest);
	CSUDIFSFileStat_S   sFileStat; 
	CSUDI_UINT64  u64Length1 ;
	CSUDI_UINT64  u64RealLen ;
	CSUDI_BOOL    bEqual = CSUDI_FALSE;
	

	ll_int2long(u64Length1, (nLen/2));

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "���������ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "д���ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSLTruncate(szFilePath, u64Length1), "�ض��ļ�ʧ��\n");	

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStat(szFilePath, &sFileStat), "��ȡ�ļ���Ϣʧ��\n");

	ll_int2long(u64RealLen, (sFileStat.m_lSize));

	bEqual = ll_eq(u64Length1, u64RealLen);

	CSTK_ASSERT_TRUE_FATAL(bEqual, "�ضϺ��ļ���С����\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "�Ƴ��ļ�ʧ��\n");
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSLTruncate(const char * pcPathName,CSUDI_UINT64 u64Length)
//@DESCRIPTION:  ���ļ��ش�
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcPathName = ��������·�� + "CSUDIFSLTruncate_0002"
//@INPUT: u64Length = u64Length1��u64Length1�����ļ�ԭ��С
//@EXPECTATION: ����ֵ����0
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSLTruncate_0002", "w+")���������ļ����õ����hFile
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile, "CSUDIFSLTruncate_0002", strlen("CSUDIFSLTruncate_0002"))д������
//@EXECUTIONFLOW: 5������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 6������CSUDIFSLTruncate(��������·�� + "CSUDIFSLTruncate_0002", strlen("CSUDIFSLTruncate_0002")*2)���ļ��ش�
//@EXECUTIONFLOW: 7������CSUDIFSStat(��������·�� + "CSUDIFSLTruncate_0002", psFileStat)����ļ���С����u64Length1�Ƚϣ�Ӧ�����
//@EXECUTIONFLOW: 8������CSUDIFSRemove(��������·�� + "CSUDIFSLTruncate_0002")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLTruncate_0002(void)
{
	char   szTest[] = "CSUDIFSLTruncate_0002";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE  hFile = CSUDI_NULL;  
	int  nLen = strlen(szTest);
	CSUDI_UINT64  u64Length1;
	CSUDI_UINT64  u64RealLen ;
	CSUDI_BOOL     bEqual = CSUDI_FALSE;
	CSUDIFSFileStat_S   sFileStat; 

	ll_int2long(u64Length1, (nLen*2));

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "���������ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "д���ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");
    CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSLTruncate(szFilePath, u64Length1), "�ض��ļ�ʧ��\n");	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStat(szFilePath, &sFileStat), "��ȡ�ļ���Ϣʧ��\n");

	ll_int2long(u64RealLen, (sFileStat.m_lSize));

	bEqual = ll_eq(u64RealLen, u64Length1);
	
	CSTK_ASSERT_TRUE_FATAL(bEqual, "�ضϺ��ļ���С����\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "�Ƴ��ļ�ʧ��\n");
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSLTruncate(const char * pcPathName,CSUDI_UINT64 u64Length)
//@DESCRIPTION:  ���ļ���
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcPathName = ��������·�� + "CSUDIFSLTruncate_0003"
//@INPUT: u64Length = 10
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSMkdir(pcPathName, 0)�����ļ���
//@EXECUTIONFLOW: 4������CSUDIFSLTruncate(��������·�� + "CSUDIFSLTruncate_0003", 10)�ض��ļ��У�Ӧ��ʧ��
//@EXECUTIONFLOW: 5������CSUDIFSRmdir(��������·�� + "CSUDIFSLTruncate_0003")ɾ���ļ���
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLTruncate_0003(void)
{
	char   szTest[] = "CSUDIFSLTruncate_0003";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_UINT64  u64Length1;

	ll_int2long(u64Length1, 10);

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSMkdir(szFilePath, 0), "����Ŀ¼ʧ��\n");
	CSTK_ASSERT_TRUE(-1 == CSUDIFSLTruncate(szFilePath, u64Length1), "�ض��ļ��в�Ӧ�óɹ�\n");	
	CSTK_ASSERT_TRUE(0 == CSUDIFSRmdir(szFilePath), "ɾ���ļ���ʧ��\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSLTruncate(const char * pcPathName,CSUDI_UINT64 u64Length)
//@DESCRIPTION: �ؿ�ָ��
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcPathName = CSUDI_NULL
//@INPUT: u64Length = 10
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSLTruncate(CSUDI_NULL, 10)��Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLTruncate_0004(void)
{
	CSUDI_UINT64  u64Length1;

	ll_int2long(u64Length1, 10);
	
	CSTK_ASSERT_TRUE(-1 == CSUDIFSLTruncate(CSUDI_NULL, u64Length1), "�ض��ļ��в�Ӧ�óɹ�\n");
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSLTruncate(const char * pcPathName,CSUDI_UINT64 u64Length)
//@DESCRIPTION:  �ز����ڵ��ļ�������·�����Ϸ��������ڡ��Ƿ��ַ��ȣ�
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcPathName = ��������·�� + "CSUDIFSLTruncate_0005"
//@INPUT: u64Length = 10
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSLTruncate(��������·�� + "CSUDIFSLTruncate_0005", 10)�ض��ļ���Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLTruncate_0005(void)
{
	char   szTest[] = "CSUDIFSLTruncate_0005";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_UINT64  u64Length1;

	ll_int2long(u64Length1, 10);

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE(-1 == CSUDIFSLTruncate(szFilePath, u64Length1), "�ضϲ����ڵ��ļ���Ӧ�óɹ�\n");	

	CSTK_FATAL_POINT
	{
		;
	}
	
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSLTruncate(const char * pcPathName,CSUDI_UINT64 u64Length)
//@DESCRIPTION:  ���ļ��ش�Ϊ�����ļ�
//@PRECONDITION: 1��ϵͳ�д���һ���ѹ��صķ־��豸
//@PRECONDITION: 2��ϵͳ֧�ִ��ļ������̽ϴ�
//@INPUT: pcPathName = ��������·�� + "CSUDIFSLTruncate_0006"
//@INPUT: u64Length = 4294967296����4G
//@EXPECTATION: ����ֵ����0
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSLTruncate_0006", "w+")���������ļ����õ����hFile
//@EXECUTIONFLOW: 4������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 5������CSUDIFSLTruncate(��������·�� + "CSUDIFSLTruncate_0006", 4294967296)���ļ���Ӧ���ɹ�
//@EXECUTIONFLOW: 6������CSUDIFSOpen(��������·�� + "CSUDIFSLTruncate_0006", "r")���������ļ����õ����hFile
//@EXECUTIONFLOW: 7������CSUDIFSLseek(hFile)�ƶ��ļ�ָ�뵽�ļ�ĩβ
//@EXECUTIONFLOW: 8������CSUDIFSLtell(hFile)����ļ���С����u64Length�Ƚϣ�Ӧ�����
//@EXECUTIONFLOW: 9������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 10������CSUDIFSRemove(��������·�� + "CSUDIFSLTruncate_0006")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLTruncate_0006(void)
{
	//�ֶ�����
	char   szTest[] = "CSUDIFSLTruncate_0006";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE   hFile = CSUDI_NULL;
	CSUDI_UINT64 u64Length1;    
	CSUDI_INT64 u64Offset;  
	CSUDI_INT64 u64RealLen;  
	CSUDI_BOOL    bEqual = CSUDI_FALSE;
	unsigned long nSize = 0x7FFFFFFF;  

 	nSize =  nSize*2;// 4G
	ll_uint2long(u64Length1,nSize);
	ll_int2long(u64Offset, 0);

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "���������ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSLTruncate(szFilePath, u64Length1), "�ض�Ϊ�����ļ�ʧ��\n");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "r")), "���ļ�ʧ��\n");
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSLseek(hFile, u64Offset, CSUDIFS_SEEK_END), "�ƶ��ļ�ָ��ʧ��\n");

	u64RealLen = CSUDIFSLtell(hFile);

	bEqual = ll_eq((CSUDI_UINT64)u64RealLen, u64Length1);

	CSTK_ASSERT_TRUE_FATAL(bEqual , "��ȡ�ļ�ָ�뵱ǰλ��ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");

	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSRemove(szFilePath), "�Ƴ��ļ�ʧ��\n");
		}
	}
	
	return CSUDI_TRUE;	
}
/****************************CSUDIFSLTruncate  End *******************************/

/****************************CSUDIFSLFTruncate  Start *****************************/ 
//@CASEGROUP: int CSUDIFSLFTruncate(CSUDI_HANDLE hFile,CSUDI_UINT64 u64Length)
//@DESCRIPTION:  ���ļ���С
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile Ϊ�Ϸ����ļ��������CSUDIFSOpen()����
//@INPUT: u64Length = u64Length1, u64Length1С���ļ�ԭ��С
//@EXPECTATION: ����ֵ����0
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSLFTruncate_0001", "w+")���������ļ����õ����hFile
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile, "CSUDIFSLFTruncate_0001", strlen("CSUDIFSLFTruncate_0001"))д������
//@EXECUTIONFLOW: 5������CSUDIFSLFTruncate(hFile, strlen("CSUDIFSLFTruncate_0001")/2)��С�ļ�
//@EXECUTIONFLOW: 6������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 7������CSUDIFSStat(��������·�� + "CSUDIFSLFTruncate_0001", psFileStat)����ļ���С����u64Length1�Ƚϣ�Ӧ�����
//@EXECUTIONFLOW: 8������CSUDIFSRemove(��������·�� + "CSUDIFSLFTruncate_0001")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLFTruncate_0001(void)
{
	char   szTest[] = "CSUDIFSLFTruncate_0001";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE  hFile = CSUDI_NULL;  
	int  nLen = strlen(szTest);
	CSUDI_UINT64  u64Length1;
	CSUDI_UINT64  u64RealLen;
	CSUDI_BOOL     bEqual = CSUDI_FALSE;
	CSUDIFSFileStat_S   sFileStat; 

	ll_int2long(u64Length1,(nLen/2));

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "���������ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "д���ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSLFTruncate(hFile, u64Length1), "�ض��ļ�ʧ��\n");
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");	

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStat(szFilePath, &sFileStat), "��ȡ�ļ���Ϣʧ��\n");

	ll_int2long(u64RealLen, (sFileStat.m_lSize));
	bEqual = ll_eq(u64RealLen, u64Length1);
	
	CSTK_ASSERT_TRUE_FATAL(bEqual, "�ضϺ��ļ���С����\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "�Ƴ��ļ�ʧ��\n");
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSLFTruncate(CSUDI_HANDLE hFile,CSUDI_UINT64 u64Length)
//@DESCRIPTION:  ���ļ��ش�
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile Ϊ�Ϸ����ļ��������CSUDIFSOpen()����
//@INPUT: u64Length = u64Length1, u64Length1�����ļ�ԭ��С
//@EXPECTATION: ����ֵ����0
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSLFTruncate_0002", "w+")���������ļ����õ����hFile
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile, "CSUDIFSLFTruncate_0002", strlen("CSUDIFSLFTruncate_0002"))д������
//@EXECUTIONFLOW: 5������CSUDIFSLFTruncate(hFile, strlen("CSUDIFSLFTruncate_0002")*2)�ش��ļ�
//@EXECUTIONFLOW: 6������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 7������CSUDIFSStat(��������·�� + "CSUDIFSLFTruncate_0002", psFileStat)����ļ���С����u64Length1�Ƚϣ�Ӧ�����
//@EXECUTIONFLOW: 8������CSUDIFSRemove(��������·�� + "CSUDIFSLFTruncate_0002")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLFTruncate_0002(void)
{
	char   szTest[] = "CSUDIFSLFTruncate_0002";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE  hFile = CSUDI_NULL;  
	int  nLen = strlen(szTest);
	CSUDI_UINT64  u64Length1;
	CSUDI_UINT64  u64RealLen;
	CSUDI_BOOL     bEqual = CSUDI_FALSE;
	CSUDIFSFileStat_S   sFileStat; 

	ll_int2long(u64Length1, (nLen*2));

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "���������ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "д���ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSLFTruncate(hFile, u64Length1), "�ض��ļ�ʧ��\n");
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");	

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStat(szFilePath, &sFileStat), "��ȡ�ļ���Ϣʧ��\n");

	ll_int2long(u64RealLen, (sFileStat.m_lSize));
	bEqual = ll_eq(u64RealLen, u64Length1);
	
	CSTK_ASSERT_TRUE_FATAL(bEqual, "�ضϺ��ļ���С����\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "�Ƴ��ļ�ʧ��\n");
		}
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSLFTruncate(CSUDI_HANDLE hFile,CSUDI_UINT64 u64Length)
//@DESCRIPTION:  �ؿ��ļ����
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = CSUDI_NULL
//@INPUT: u64Length = 10
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSLFTruncate(CSUDI_NULL, 10)���ļ���Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLFTruncate_0003(void)
{
	CSUDI_UINT64  u64Length1;

	ll_int2long(u64Length1, 10);
	
	CSTK_ASSERT_TRUE(-1 == CSUDIFSLFTruncate(CSUDI_NULL, u64Length1), "�ض��ļ�ʧ��\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSLFTruncate(CSUDI_HANDLE hFile,CSUDI_UINT64 u64Length)
//@DESCRIPTION:  �ز����ڵľ��
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = 0x7FFFFFFF(����0x7FFFFFFFΪ�����ڵľ��)
//@INPUT: u64Length = 10
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSLFTruncate(0x7FFFFFFF, 10)���ļ���Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLFTruncate_0004(void)
{
	CSUDI_UINT64  u64Length1;

	ll_int2long(u64Length1, 10);
	CSTK_ASSERT_TRUE(-1 == CSUDIFSLFTruncate((CSUDI_HANDLE)0x7FFFFFFF, u64Length1), "�ض��ļ�ʧ��\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSLFTruncate(CSUDI_HANDLE hFile,CSUDI_UINT64 u64Length)
//@DESCRIPTION:  ���ļ���
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile �ļ��о������CSUDIFSMkdir()����
//@INPUT: u64Length = 10
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSMkdir(��������·�� + "CSUDIFSLFTruncate_0005", 0)�����ļ���
//@EXECUTIONFLOW: 4������CSUDIFSOpenDir(��������·�� + "CSUDIFSLFTruncate_0005")���ļ��У��õ����hDir
//@EXECUTIONFLOW: 5������CSUDIFSLFTruncate(hDir, 10)�ض��ļ��У�Ӧ��ʧ��
//@EXECUTIONFLOW: 6������CSUDIFSCloseDir(hDir)�ر��ļ���
//@EXECUTIONFLOW: 7������CSUDIFSRmdir(��������·�� + "CSUDIFSLFTruncate_0005")ɾ���ļ���
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLFTruncate_0005(void)
{
	char   szTest[] = "CSUDIFSLFTruncate_0005";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE  hDir = CSUDI_NULL;  
	CSUDI_UINT64  u64Length1;

	ll_int2long(u64Length1, 10);

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSMkdir(szFilePath, 0), "�����ļ���ʧ��\n");

	CSTK_ASSERT_TRUE(CSUDI_NULL != (hDir = CSUDIFSOpenDir(szFilePath)), "���ļ���ʧ��\n");

	CSTK_ASSERT_TRUE(-1 == CSUDIFSLFTruncate(hDir, u64Length1), "�ض��ļ��в�Ӧ�óɹ�\n");	

	CSTK_ASSERT_TRUE(0 == CSUDIFSCloseDir(hDir), "�ر��ļ���ʧ��\n");

	CSTK_ASSERT_TRUE(0 == CSUDIFSRmdir(szFilePath), "ɾ���ļ���ʧ��\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;		
}

//@CASEGROUP: int CSUDIFSLFTruncate(CSUDI_HANDLE hFile,CSUDI_UINT64 u64Length)
//@DESCRIPTION:  �ش�Ϊ�����ļ�
//@PRECONDITION: 1��ϵͳ�д���һ���ѹ��صķ־��豸
//@PRECONDITION: 2����Ҫϵͳ֧�ִ��ļ��������㹻��
//@INPUT: hFile Ϊ�Ϸ����ļ��������CSUDIFSOpen()����
//@INPUT: u64Length = 4294967296����4G
//@EXPECTATION: ����ֵ����0
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSLFTruncate_0006", "w+")���������ļ����õ����hFile
//@EXECUTIONFLOW: 4������CSUDIFSLFTruncate(hFile, 4294967296)�ش�Ϊ�����ļ���Ӧ���ɹ�
//@EXECUTIONFLOW: 5������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 6������CSUDIFSOpen(��������·�� + "CSUDIFSLFTruncate_0006", "r")���������ļ����õ����hFile
//@EXECUTIONFLOW: 7������CSUDIFSLseek(hFile)�ƶ��ļ�ָ�뵽�ļ�ĩβ
//@EXECUTIONFLOW: 8������CSUDIFSLtell(hFile)����ļ���С����u64Length�Ƚϣ�Ӧ�����
//@EXECUTIONFLOW: 9������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 10������CSUDIFSRemove(��������·�� + "CSUDIFSLFTruncate_0006")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLFTruncate_0006(void)
{
	//�ֶ�����
	char   szTest[] = "CSUDIFSLFTruncate_0006";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE   hFile = CSUDI_NULL;
	CSUDI_UINT64 u64Length1;    
	CSUDI_INT64   i64RealLen;
	CSUDI_INT64   i64Offset;
	CSUDI_BOOL    bEqual = CSUDI_FALSE;
	unsigned long   nTmp = 0x7FFFFFFF;

	nTmp = nTmp*2;  // 4G
	ll_uint2long(u64Length1, nTmp);
	ll_int2long(i64Offset, 0);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "���������ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSLFTruncate(hFile, u64Length1), "�ض�Ϊ�����ļ�ʧ��\n");	
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "r")), "���ļ�ʧ��\n");
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSLseek(hFile, i64Offset, CSUDIFS_SEEK_END), "�ƶ��ļ�ָ��ʧ��\n");

	i64RealLen = CSUDIFSLtell(hFile);
	bEqual = ll_eq((CSUDI_UINT64)i64Offset, u64Length1);

	CSTK_ASSERT_TRUE_FATAL(bEqual, "��ȡ�ļ�ָ�뵱ǰλ��ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");

	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSRemove(szFilePath), "�Ƴ��ļ�ʧ��\n");
		}
	}
	
	return CSUDI_TRUE;	
}
/****************************CSUDIFSLFTruncate  End *******************************/

/****************************CSUDIFSStat  Start *****************************/ 
//@CASEGROUP: int CSUDIFSStat(const char * pcFileName, CSUDIFSFileStat_S * psFileStat)
//@DESCRIPTION:  ������ȡ�ļ�����
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcFileName = ��������·�� + "CSUDIFSStat_0001"
//@INPUT: psFileStat = sFileStat������CSUDIFSFileStat_S sFileStat
//@EXPECTATION: ����ֵ����0
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSStat_0001", "w+")���������ļ����õ����hFile
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile, "CSUDIFSStat_0001", strlen("CSUDIFSStat_0001"))д������
//@EXECUTIONFLOW: 5������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 6������CSUDIFSStat(��������·�� + "CSUDIFSStat_0001", &sFileStat)����ļ����ԣ�ȡ���ļ���С��д��ʱ�ıȽϣ�Ӧ�����
//@EXECUTIONFLOW: 7������CSUDIFSRemove(��������·�� + "CSUDIFSStat_0001")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSStat_0001(void)
{
	char   szTest[] = "CSUDIFSStat_0001";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE   hFile = CSUDI_NULL;
	int    nLen = strlen(szTest);
	CSUDIFSFileStat_S  sFileStat;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "���������ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "д���ļ�ʧ��\n");
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");	

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStat(szFilePath, &sFileStat), "��ȡ�ļ���Ϣʧ��\n");

	CSTK_ASSERT_TRUE_FATAL((int)sFileStat.m_lSize == nLen, "��ȡ�ļ���Ϣ��С����\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "�Ƴ��ļ�ʧ��\n");
		}
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSStat(const char * pcFileName, CSUDIFSFileStat_S * psFileStat)
//@DESCRIPTION:  ��ȡ��ָ���ļ�����
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcFileName = CSUDI_NULL
//@INPUT: psFileStat = &sFileStat������CSUDIFSFileStat_S psFileStat
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSStat(CSUDI_NULL, psFileStat1)��ȡ�ļ����ԣ�Ӧ��ʧ��	
CSUDI_BOOL CSTC_FS_IT_CSUDIFSStat_0002(void)
{
	CSUDIFSFileStat_S  sFileStat;
	
	CSTK_ASSERT_TRUE(-1 == CSUDIFSStat(CSUDI_NULL, &sFileStat), "��ȡ�ļ���Ϣʧ��\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSStat(const char * pcFileName, CSUDIFSFileStat_S * psFileStat)
//@DESCRIPTION:  ��ȡ�����ڵ��ļ�������
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcFileName = ��������·�� + "CSUDIFSStat_0003"
//@INPUT: psFileStat = &sFileStat������CSUDIFSFileStat_S sFileStat
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSStat(��������·�� + "CSUDIFSStat_0003", &sFileStat)��ȡ�ļ����ԣ�Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSStat_0003(void)
{
	char   szTest[] = "CSUDIFSStat_0003";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDIFSFileStat_S  sFileStat;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);	

	CSTK_ASSERT_TRUE(-1 == CSUDIFSStat(szFilePath, &sFileStat), "��ȡ�ļ���Ϣ��Ӧ���ɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSStat(const char * pcFileName, CSUDIFSFileStat_S * psFileStat)
//@DESCRIPTION:  ��ȡ�ļ�������
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcFileName = ��������·�� + "CSUDIFSStat_0004"
//@INPUT: psFileStat = &sFileStat������CSUDIFSFileStat_S sFileStat
//@EXPECTATION: ����ֵ����0
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSMkdir(��������·�� + "CSUDIFSStat_0004", 0)�����ļ���
//@EXECUTIONFLOW: 4������CSUDIFSStat(��������·�� + "CSUDIFSStat_0004", &sFileStat)��ȡ�ļ������ԣ�Ӧ���ɹ�
//@EXECUTIONFLOW: 5������CSUDIFSRmdir(��������·�� + "CSUDIFSStat_0004")ɾ���ļ���
CSUDI_BOOL CSTC_FS_IT_CSUDIFSStat_0004(void)
{
	char   szTest[] = "CSUDIFSStat_0004";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDIFSFileStat_S sFileStat;

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSMkdir(szFilePath, 0), "�����ļ���ʧ��\n");

	CSTK_ASSERT_TRUE(0 == CSUDIFSStat(szFilePath, &sFileStat), "��ȡ�ļ�����Ϣʧ��\n");
	
	CSTK_ASSERT_TRUE(0 == CSUDIFSRmdir(szFilePath), "ɾ���ļ���ʧ��\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSStat(const char * pcFileName, CSUDIFSFileStat_S * psFileStat)
//@DESCRIPTION:  �����ļ����Ե�ָ��Ƿ�
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcFileName = ��������·�� + "CSUDIFSStat_0005"
//@INPUT: psFileStat = CSUDI_NULL
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSStat_0005", "w+")���������ļ����õ����hFile
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile, "CSUDIFSStat_0005", strlen("CSUDIFSStat_0005"))д������
//@EXECUTIONFLOW: 5������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 6������CSUDIFSStat(��������·�� + "CSUDIFSStat_0005", CSUDI_NULL)��ȡ�ļ����ԣ�Ӧ��ʧ��
//@EXECUTIONFLOW: 7������CSUDIFSRemove(��������·�� + "CSUDIFSStat_0005")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSStat_0005(void)
{
	char   szTest[] = "CSUDIFSStat_0005";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE   hFile = CSUDI_NULL;
	int    nLen = strlen(szTest);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "���������ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "д���ļ�ʧ��\n");
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");	

	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSStat(szFilePath, CSUDI_NULL), "��ȡ�ļ���Ϣ��Ӧ���ɹ�\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "�Ƴ��ļ�ʧ��\n");
		}
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSStat(const char * pcFileName, CSUDIFSFileStat_S * psFileStat)
//@DESCRIPTION:  ��֤CSUDIFSFileStat_S���ʱ�������ڴ������޸ġ����ʺ�����
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcFileName = ��������·�� + "CSUDIFSStat_0006"
//@INPUT: psFileStat = &sFileStat
//@EXPECTATION: ÿ�ζ��ļ����в���֮��ʱ�����Զ������仯
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSStat_0006", "w+")���������ļ����õ����hFile
//@EXECUTIONFLOW: 4������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 5������CSUDIFSStat��ȡ�ļ������е�ʱ����Ϣ���������CSUDIPlusOSClockGet()��õ�ʱ����Ϣ��Ƚ�,����ʱ�������������ļ�ʱCtime == Atime == Mtime == ʵ�ʴ���ʱ�䣩
//@EXECUTIONFLOW: 6������CSUDIFSOpen(��������·�� + "CSUDIFSStat_0006", "w+")���ļ����õ����hFile
//@EXECUTIONFLOW: 7������CSUDIFSWrite(hFile, "CSUDIFSStat_0006", strlen("CSUDIFSStat_0006"))д������
//@EXECUTIONFLOW: 8������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 9������CSUDIFSStat��ȡ�ļ������е�ʱ����Ϣ���������CSUDIPlusOSClockGet()��õ�ʱ����Ϣ��Ƚ�,����ʱ���������޸��ļ�ʱAtime���䣬Ctime == Mtime == ʵ���޸�ʱ�䣩
//@EXECUTIONFLOW: 10������CSUDIFSOpen(��������·�� + "CSUDIFSStat_0006", "r")���ļ����õ����hFile
//@EXECUTIONFLOW: 11������CSUDIFSRead(hFile, buf, buflen))��������
//@EXECUTIONFLOW: 12������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 13������CSUDIFSStat��ȡ�ļ������е�ʱ����Ϣ���������CSUDIPlusOSClockGet()��õ�ʱ����Ϣ��Ƚ�,����ʱ�������������ļ�ʱCtime��Mtime���䣬Atime == ʵ�ʷ���ʱ�䣩
//@EXECUTIONFLOW: 14������CSUDIFSRemove(��������·�� + "CSUDIFSStat_0006")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSStat_0006(void)
{
    char szTest[] = "CSUDIFSStat_0006";
    char szBuf[100] = {0};
    char szFilePath[CSUDI_MAX_FILE_NAME_LEN] = {0};
    CSUDI_HANDLE hFile = CSUDI_NULL;
    int nLen = strlen(szTest);
    CSUDIFSFileStat_S sFileStat;
    CSUDIPLUS_TIME_T curTime = 0;
    CSUDIPLUS_TIME_T lastOpTime = 0;

    CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��");

    memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
    strcpy(szFilePath, g_szVolumeMountPoint);
    strcat(szFilePath, szTest);


    /*************************
     * �����ļ���У��ʱ��*****
    *************************/
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "�����ļ������ļ�ʧ��");

    CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�������ر��ļ�ʧ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPlusOSClockGet(&curTime), "�����ļ�����ȡ��ǰʱ��ʧ��");

    CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStat(szFilePath, &sFileStat), "�����ļ�����ȡ�ļ�����ʧ��");

    CSTK_ASSERT_TRUE_FATAL((sFileStat.m_lCtime == sFileStat.m_lMtime) && (sFileStat.m_lMtime == sFileStat.m_lAtime), "�����ļ����ļ�ʱ�������쳣");
    CSTK_ASSERT_TRUE_FATAL((sFileStat.m_lCtime <= curTime) && ((curTime-sFileStat.m_lCtime)<=1), "�����ļ����ļ������쳣");

    // ����������ʱ��
    lastOpTime = sFileStat.m_lAtime;

    // �ȴ�3s����������ļ�ʱ�����Ա仯����
    CSTCPrint("ʱ����Ϣ�������ȴ�3s��������...\n");
    CSUDIOSThreadSleep(3000);

    /*************************
     * �޸��ļ���У��ʱ��*****
    *************************/
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "�޸��ļ������ļ�ʧ��");

    CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "д���ļ�ʧ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPlusOSClockGet(&curTime), "�޸��ļ�����ȡ��ǰʱ��ʧ��");

    CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��");

    CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStat(szFilePath, &sFileStat), "�޸��ļ�����ȡ�ļ�����ʧ��");

    CSTK_ASSERT_TRUE_FATAL((sFileStat.m_lCtime == sFileStat.m_lMtime) && (sFileStat.m_lAtime == lastOpTime), "�޸��ļ����ļ�ʱ�������쳣");
    CSTK_ASSERT_TRUE_FATAL((sFileStat.m_lMtime <= curTime) && ((curTime-sFileStat.m_lMtime)<=1), "�޸��ļ����ļ������쳣");

    lastOpTime = sFileStat.m_lMtime;

    // �ȴ�3s����������ļ�ʱ�����Ա仯����
    CSTCPrint("ʱ����Ϣ�������ȴ�3s��������...\n");
    CSUDIOSThreadSleep(3000);

    /*************************
     * �����ļ���У��ʱ��*****
    *************************/
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "r")), "���������ļ�ʧ��");
    
    CSTK_ASSERT_TRUE_FATAL(0 <= (nLen = CSUDIFSRead(hFile, szBuf, 100)), "��ȡ�ļ�ʧ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPlusOSClockGet(&curTime), "�����ļ�����ȡ��ǰʱ��ʧ��");

    CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�����ļ����ر��ļ�ʧ��");

    CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStat(szFilePath, &sFileStat), "�����ļ�����ȡ�ļ���Ϣʧ��");

    CSTK_ASSERT_TRUE_FATAL((sFileStat.m_lCtime == sFileStat.m_lMtime) && (sFileStat.m_lMtime == lastOpTime), "�����ļ����ļ�ʱ�������쳣");
    CSTK_ASSERT_TRUE_FATAL((sFileStat.m_lAtime <= curTime) && ((curTime-sFileStat.m_lAtime)<=1), "�����ļ����ļ������쳣");

    CSTK_FATAL_POINT
		
    if (CSUDI_NULL != hFile)
    {
        CSTK_ASSERT_TRUE(0 == CSUDIFSRemove(szFilePath), "�Ƴ��ļ�ʧ��");

		CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile), "�ͷž��ʧ��");

		hFile = CSUDI_NULL;
    }

    return CSUDI_TRUE;
}
/****************************CSUDIFSStat  End *******************************/

/****************************CSUDIFSStat  Start *****************************/ 
//@CASEGROUP: int CSUDIFSFstat(CSUDI_HANDLE hFile, CSUDIFSFileStat_S * psFileStat)
//@DESCRIPTION:  ������ȡ�ļ�����
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile Ϊ�Ϸ����ļ��������CSUDIOpen()����
//@INPUT: psFileStat = &sFileStat������CSUDIFSFileStat_S sFileStat
//@EXPECTATION: ����ֵ����0
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·��+"CSUDIFSFstat_0001", "w+")���������ļ����õ����hFile
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile, "CSUDIFSFstat_0001", strlen("CSUDIFSFstat_0001"))д������
//@EXECUTIONFLOW: 5������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 6������CSUDIFSOpen(��������·�� + "CSUDIFSFstat_0001", "r")���ļ�
//@EXECUTIONFLOW: 7������CSUDIFSFstat(hFile, &sFileStat)��ȡ�ļ����ԣ�Ӧ���ɹ�
//@EXECUTIONFLOW: 8������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 9������CSUDIFSRemove(��������·��+"CSUDIFSFstat_0001")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFstat_0001(void)
{
	char   szTest[] = "CSUDIFSFstat_0001";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE   hFile = CSUDI_NULL;
	int    nLen = strlen(szTest);
	CSUDIFSFileStat_S  sFileStat;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	//CSTCPrint("·��= %s\n",szFilePath);
	strcat(szFilePath, szTest);	
        //CSTCPrint("·��= %s\n",szFilePath);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "���������ļ�ʧ��1\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "д���ļ�ʧ��\n");
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "r")), "���ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSFstat(hFile, &sFileStat), "��ȡ�ļ���Ϣʧ��\n");
	CSTK_ASSERT_TRUE_FATAL((int)sFileStat.m_lSize == nLen, "��ȡ�ļ���Ϣ��С����\n");	

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");	

	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "�Ƴ��ļ�ʧ��\n");
		}
	}
	return CSUDI_TRUE;		
}

//@CASEGROUP: int CSUDIFSFstat(CSUDI_HANDLE hFile, CSUDIFSFileStat_S * psFileStat)
//@DESCRIPTION:  ��ȡ���ļ����������
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = CSUDI_NULL
//@INPUT: psFileStat = &sFileStat������CSUDIFSFileStat_S sFileStat
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSFstat(CSUDI_NULL, &sFileStat)��Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFstat_0002(void)
{
	CSUDIFSFileStat_S  sFileStat;

	CSTK_ASSERT_TRUE(-1 == CSUDIFSFstat(CSUDI_NULL, &sFileStat), "��ȡ�ļ���Ϣ��Ӧ�óɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSFstat(CSUDI_HANDLE hFile, CSUDIFSFileStat_S * psFileStat)
//@DESCRIPTION:  ��ȡ�����ڵ��ļ����������
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pFile = 0x7FFFFFFF(����0x7FFFFFFFΪ������ļ����)
//@INPUT: psFileStat = &sFileStat������CSUDIFSFileStat_S sFileStat
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSFstat(0x7FFFFFFF, &sFileStat)
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFstat_0003(void)
{
	CSUDIFSFileStat_S  sFileStat;

	CSTK_ASSERT_TRUE(-1 == CSUDIFSFstat((CSUDI_HANDLE)0x7FFFFFFF, &sFileStat), "��ȡ�ļ���Ϣʧ��\n");
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSFstat(CSUDI_HANDLE hFile, CSUDIFSFileStat_S * psFileStat)
//@DESCRIPTION:  ��ȡ�ļ�������
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = hDir, hDirΪCSUDIFSMkdir���صľ��
//@INPUT: psFileStat = &sFileStat������CSUDIFSFileStat_S sFileStat
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSMkdir(��������·�� + "CSUDIFSFstat_0004", 0)�����ļ���
//@EXECUTIONFLOW: 4������CSUDIFSOpenDir(��������·�� + "CSUDIFSFstat_0004")���ļ��У��õ��ļ��о��hDir
//@EXECUTIONFLOW: 5������CSUDIFSFstat(hDir, &sFileStat)��ȡ�ļ�������
//@EXECUTIONFLOW: 6������CSUDIFSCloseDir(hDir)�ر��ļ���
//@EXECUTIONFLOW: 7������CSUDIFSRmdir(��������·�� + "CSUDIFSFstat_0004")ɾ���ļ���
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFstat_0004(void)
{
	char   szTest[] = "CSUDIFSFstat_0004";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDIFSFileStat_S sFileStat;
	CSUDI_HANDLE     hDir = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSMkdir(szFilePath, 0), "�����ļ���ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hDir = CSUDIFSOpenDir(szFilePath)), "���ļ���ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSFstat(hDir, &sFileStat), "��ȡ�ļ�����Ϣ��Ӧ�óɹ�\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSCloseDir(hDir), "�ر��ļ���ʧ��\n");
	
	CSTK_FATAL_POINT
	{
		if(hDir != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szFilePath), "ɾ���ļ���ʧ��\n");
		}
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSFstat(CSUDI_HANDLE hFile, CSUDIFSFileStat_S * psFileStat)
//@DESCRIPTION:  �����ļ����Ե�ָ��Ƿ�
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = hFile1, hFile1Ϊ�Ϸ����ļ��������CSUDIOpen()����
//@INPUT: psFileStat = CSUDI_NULL
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSFstat_0005", "w+")���������ļ����õ����hFile1
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile1, "CSUDIFSFstat_0005", strlen("CSUDIFSFstat_0005"))д������
//@EXECUTIONFLOW: 5������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 6������CSUDIFSOpen(��������·�� + "CSUDIFSFstat_0005", "r")���ļ�
//@EXECUTIONFLOW: 7������CSUDIFSFstat(hFile1, CSUDI_NULL)��ȡ�ļ���Ϣ
//@EXECUTIONFLOW: 8������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 9������CSUDIFSRemove(��������·�� + "CSUDIFSFstat_0005")ɾ�� �ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFstat_0005(void)
{
	char   szTest[] = "CSUDIFSFstat_0005";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	int    nLen = strlen(szTest);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(szFilePath, "w+")), "���������ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "д���ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(szFilePath, "r")), "���ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSFstat(hFile1, CSUDI_NULL), "��ȡ�ļ���Ϣ��Ӧ���ɹ�\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");	

	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "�Ƴ��ļ�ʧ��\n");
		}
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSFstat(CSUDI_HANDLE hFile, CSUDIFSFileStat_S * psFileStat)
//@DESCRIPTION:  ��֤����CSUDIFSFstat���ص�CSUDIFSFileStat_S���ʱ�������ڴ������޸ġ����ʺ�����
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile Ϊ�Ϸ��ļ��������CSUDIOpen()����
//@INPUT: psFileStat = &sFileStat
//@EXPECTATION: ÿ�ζ��ļ����в���֮��ʱ�����Զ������仯
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSStat_0006", "w+")���������ļ����õ����hFile
//@EXECUTIONFLOW: 4������CSUDIFSFstat��ȡ�ļ������е�ʱ����Ϣ��������CSUDIPlusOSClockGet()���ʱ��
//@EXECUTIONFLOW: 5������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 6���Ƚϻ�ȡ�����ļ������е��ļ���Ϣ���õ�ϵͳʱ�䣬����ʱ�������������ļ�ʱCtime == Atime == Mtime == ʵ�ʴ���ʱ�䣩
//@EXECUTIONFLOW: 7������CSUDIFSOpen(��������·�� + "CSUDIFSStat_0006", "w+")���ļ����õ����hFile
//@EXECUTIONFLOW: 8������CSUDIFSWrite(hFile, "CSUDIFSStat_0006", strlen("CSUDIFSStat_0006"))д������
//@EXECUTIONFLOW: 9������CSUDIFSFstat��ȡ�ļ������е�ʱ����Ϣ��������CSUDIPlusOSClockGet()���ʱ��
//@EXECUTIONFLOW: 10������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 11���Ƚϻ�ȡ�����ļ������е��ļ���Ϣ���õ�ϵͳʱ�䣬����ʱ���������޸��ļ�ʱAtime���䣬Ctime == Mtime == ʵ���޸�ʱ�䣩
//@EXECUTIONFLOW: 12������CSUDIFSOpen(��������·�� + "CSUDIFSStat_0006", "r")���ļ����õ����hFile
//@EXECUTIONFLOW: 13������CSUDIFSRead(hFile, buf, buflen))��������
//@EXECUTIONFLOW: 14������CSUDIFSFstat��ȡ�ļ������е�ʱ����Ϣ��������CSUDIPlusOSClockGet()���ʱ��
//@EXECUTIONFLOW: 15������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 16���Ƚϻ�ȡ�����ļ������е��ļ���Ϣ���õ�ϵͳʱ�䣬����ʱ�������������ļ�ʱCtime��Mtime���䣬Atime == ʵ�ʷ���ʱ�䣩
//@EXECUTIONFLOW: 17������CSUDIFSRemove(��������·�� + "CSUDIFSStat_0006")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFstat_0006(void)
{
    char szTest[] = "CSUDIFSFstat_0006";
    char szBuf[100] = {0};
    char szFilePath[CSUDI_MAX_FILE_NAME_LEN] = {0};
    CSUDI_HANDLE hFile = CSUDI_NULL;
    int nLen = strlen(szTest);
    CSUDIFSFileStat_S sFileStat;
    CSUDIPLUS_TIME_T lCurTime = 0;
    CSUDIPLUS_TIME_T lLastOpTime = 0;

    CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��");

    memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
    strcpy(szFilePath, g_szVolumeMountPoint);
    strcat(szFilePath, szTest);

    /*************************
     * �����ļ���У��ʱ��*****
    *************************/
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "�����ļ������ļ�ʧ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPlusOSClockGet(&lCurTime), "�����ļ�����ȡ��ǰʱ��ʧ��");

    CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSFstat(hFile, &sFileStat), "�����ļ�����ȡ�ļ�����ʧ��");

    CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�������ر��ļ�ʧ��");

    CSTK_ASSERT_TRUE_FATAL((sFileStat.m_lCtime == sFileStat.m_lMtime) && (sFileStat.m_lMtime == sFileStat.m_lAtime), "�����ļ����ļ�ʱ�������쳣");
    CSTK_ASSERT_TRUE_FATAL((sFileStat.m_lCtime <= lCurTime) && ((lCurTime-sFileStat.m_lCtime)<=1), "�����ļ����ļ������쳣");

    // ����������ʱ��
    lLastOpTime = sFileStat.m_lAtime;

    // �ȴ�3s����������ļ�ʱ�����Ա仯����
    CSTCPrint("ʱ����Ϣ�������ȴ�3s��������...\n");
    CSUDIOSThreadSleep(3000);

    /*************************
     * �޸��ļ���У��ʱ��*****
    *************************/
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "�޸��ļ������ļ�ʧ��");

    CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "д���ļ�ʧ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPlusOSClockGet(&lCurTime), "�޸��ļ�����ȡ��ǰʱ��ʧ��");

    CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSFstat(hFile, &sFileStat), "�޸��ļ�����ȡ�ļ�����ʧ��");

    CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��");

    CSTK_ASSERT_TRUE_FATAL((sFileStat.m_lCtime == sFileStat.m_lMtime) && (sFileStat.m_lAtime == lLastOpTime), "�޸��ļ����ļ�ʱ�������쳣");
    CSTK_ASSERT_TRUE_FATAL((sFileStat.m_lMtime <= lCurTime) && ((lCurTime-sFileStat.m_lMtime)<=1), "�޸��ļ����ļ������쳣");

    lLastOpTime = sFileStat.m_lMtime;

    // �ȴ�3s����������ļ�ʱ�����Ա仯����
    CSTCPrint("ʱ����Ϣ�������ȴ�3s��������...\n");
    CSUDIOSThreadSleep(3000);

    /*************************
     * �����ļ���У��ʱ��*****
    *************************/
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "r")), "���������ļ�ʧ��");
    
    CSTK_ASSERT_TRUE_FATAL(-1 != (nLen = CSUDIFSRead(hFile, szBuf, 100)), "��ȡ�ļ�ʧ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPlusOSClockGet(&lCurTime), "�����ļ�����ȡ��ǰʱ��ʧ��");

    CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSFstat(hFile, &sFileStat), "�����ļ�����ȡ�ļ���Ϣʧ��");

    CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�����ļ����ر��ļ�ʧ��");


    CSTK_ASSERT_TRUE_FATAL((sFileStat.m_lCtime == sFileStat.m_lMtime) && (sFileStat.m_lMtime == lLastOpTime), "�����ļ����ļ�ʱ�������쳣");
    CSTK_ASSERT_TRUE_FATAL((sFileStat.m_lAtime <= lCurTime) && ((lCurTime-sFileStat.m_lAtime)<=1), "�����ļ����ļ������쳣");

    CSTK_FATAL_POINT

    if (CSUDI_NULL != hFile)
    {
        CSTK_ASSERT_TRUE(0 == CSUDIFSRemove(szFilePath), "�Ƴ��ļ�ʧ��");

		CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile), "�ͷž��ʧ��");

		hFile = CSUDI_NULL;
    }

    return CSUDI_TRUE;
}
/****************************CSUDIFSFstat  End *******************************/

/****************************CSUDIFSMkdir  Start *****************************/ 
//@CASEGROUP: int CSUDIFSMkdir(const char * pcPathName, int nReserved)
//@DESCRIPTION:  ���������ļ���	
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcPathName = ��������·�� + "CSUDIFSMkdir_0001"
//@INPUT: nReserved = 0
//@EXPECTATION: ����ֵ����0
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSMkdir(��������·�� + "CSUDIFSMkdir_0001", 0)�����ļ��У�Ӧ���ɹ�
//@EXECUTIONFLOW: 4������CSUDIFSOpenDir(��������·�� + "CSUDIFSFstat_0004")���ļ��У��õ��ļ��о��hDir
//@EXECUTIONFLOW: 5������CSUDIFSCloseDir(hDir)�ر��ļ���
//@EXECUTIONFLOW: 6������CSUDIFSRmdir(��������·�� + "CSUDIFSMkdir_0001")ɾ���ļ���
CSUDI_BOOL CSTC_FS_IT_CSUDIFSMkdir_0001(void)
{
	char   szTest[] = "CSUDIFSMkdir_0001";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE     hDir = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSMkdir(szFilePath, 0), "�����ļ���ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hDir = CSUDIFSOpenDir(szFilePath)), "���ļ���ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSCloseDir(hDir), "�ر��ļ���ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szFilePath), "ɾ���ļ���ʧ��\n");
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSMkdir(const char * pcPathName, int nReserved)
//@DESCRIPTION:  ��һ�������ڵ��ļ����´����ļ���
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcPathName = ��������·�� + "CSUDIFSMkdir_0002_dir" + "CSUDIFSMkdir_0002"
//@INPUT: nReserved = 0
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSMkdir(��������·�� + "CSUDIFSMkdir_0002_dir" + "CSUDIFSMkdir_0002", 0)�����ļ��У�Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSMkdir_0002(void)
{
	char   szTest[] = "CSUDIFSMkdir_0002_dir/CSUDIFSMkdir_0002";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);
	
	CSTK_ASSERT_TRUE(-1 == CSUDIFSMkdir(szFilePath, 0), "�����ļ��в�Ӧ���ɹ�\n");
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSMkdir(const char * pcPathName, int nReserved)
//@DESCRIPTION:  ���������ļ���
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcPathName = ��������·�� + ""
//@INPUT: nReserved = 0
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSMkdir(pcPathName, 0)��Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSMkdir_0003(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");
	
	CSTK_ASSERT_TRUE(-1 == CSUDIFSMkdir(g_szVolumeMountPoint, 0), "���������ļ��в�Ӧ���ɹ�\n");
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSMkdir(const char * pcPathName, int nReserved)
//@DESCRIPTION:  ������ָ���ļ���
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcPathName = CSUDI_NULL
//@INPUT: nReserved = 0
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSMkdir(pcPathName, 0)�����ļ�,Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSMkdir_0004(void)
{
	CSTK_ASSERT_TRUE(-1 == CSUDIFSMkdir(CSUDI_NULL, 0), "���������ļ��в�Ӧ���ɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSMkdir(const char * pcPathName, int nReserved)
//@DESCRIPTION:  ���������������
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcPathName = ��������·�� + "CSUDIFSMkdir_0005"
//@INPUT: nReserved = -1
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSMkdir(��������·�� + "CSUDIFSMkdir_0005", -1)�����ļ��У�Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSMkdir_0005(void)
{
	char   szTest[] = "CSUDIFSMkdir_0005";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);
	
	CSTK_ASSERT_TRUE(-1 == CSUDIFSMkdir(szFilePath, -1), "�����ļ��в�Ӧ���ɹ�\n");
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}
/****************************CSUDIFSMkdir  End *******************************/

/****************************CSUDIFSRmdir  Start *****************************/ 
//@CASEGROUP: int CSUDIFSRmdir(const char * pcPathName)
//@DESCRIPTION:  ����ɾ���ļ���	
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcPathName = ��������·�� + "CSUDIFSRmdir_0001"
//@EXPECTATION: ����ֵ����0
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSMkdir(��������·�� + "CSUDIFSRmdir_0001", 0)
//@EXECUTIONFLOW: 4������CSUDIFSRmdir(��������·�� + "CSUDIFSRmdir_0001")
//@EXECUTIONFLOW: 5������CSUDIFSOpendir(��������·�� + "CSUDIFSRmdir_0001")����Ƿ�ȷʵ��ɾ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRmdir_0001(void)
{
	char   szTest[] = "CSUDIFSRmdir_0001";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE     hDir = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSMkdir(szFilePath, 0), "�����ļ���ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szFilePath), "ɾ���ļ���ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == (hDir = CSUDIFSOpenDir(szFilePath)), "���ļ��в�Ӧ�óɹ�\n");
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
	
}

//@CASEGROUP: int CSUDIFSRmdir(const char * pcPathName)
//@DESCRIPTION:  ɾ�������ڵ��ļ���
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcPathName = ��������·�� + "CSUDIFSRmdir_0002"
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSRmdir(��������·�� + "CSUDIFSRmdir_0002")ɾ���ļ��У�Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRmdir_0002(void)
{
	char   szTest[] = "CSUDIFSRmdir_0001";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSRmdir(szFilePath), "ɾ���ļ���ʧ��\n");
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSRmdir(const char * pcPathName)
//@DESCRIPTION:  ɾ����ָ���ļ���
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcPathName = CSUDI_NULL
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSRmdir(CSUDI_NULL)ɾ���ļ���Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRmdir_0003(void)
{
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSRmdir(CSUDI_NULL), "ɾ���ļ��в�Ӧ���ɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSRmdir(const char * pcPathName)
//@DESCRIPTION:  ɾ���ǿ��ļ���
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcPathName = ��������·�� + "CSUDIFSRmdir_0004"
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSMkdir(��������·�� + "CSUDIFSRmdir_0004", 0)�����ļ���
//@EXECUTIONFLOW: 4������CSUDIFSMkdir(��������·�� + "CSUDIFSRmdir_0004"+"CSUDIFSRmdir_0004", 0)�������ļ���
//@EXECUTIONFLOW: 5������CSUDIFSRmdir(��������·�� + "CSUDIFSRmdir_0004")ɾ���ļ��У�Ӧ��ʧ��
//@EXECUTIONFLOW: 6������CSUDIFSRmdir(��������·�� + "CSUDIFSRmdir_0004"+"CSUDIFSRmdir_0004")ɾ�����ļ���
//@EXECUTIONFLOW: 7������CSUDIFSRmdir(��������·�� + "CSUDIFSRmdir_0004")ɾ���ļ���
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRmdir_0004(void)
{
	char   szTest[] = "CSUDIFSRmdir_0004";   
	char   szFilePathA[CSUDI_MAX_FILE_NAME_LEN];
	char   szFilePathB[CSUDI_MAX_FILE_NAME_LEN];

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePathA, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePathA, g_szVolumeMountPoint);
	strcat(szFilePathA, szTest);
	
	memset(szFilePathB, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePathB, szFilePathA);
	strcat(szFilePathB, "/");
	strcat(szFilePathB, szTest);

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSMkdir(szFilePathA, 0), "�����ļ���ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSMkdir(szFilePathB, 0), "�������ļ���ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSRmdir(szFilePathA), "ɾ���ǿ��ļ��в�Ӧ���ɹ�\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szFilePathB), "ɾ���ļ���ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szFilePathA), "ɾ���ļ���ʧ��\n");
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSRmdir(const char * pcPathName)
//@DESCRIPTION:  ɾ���ļ�
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcPathName = ��������·�� + "CSUDIFSRmdir_0005"
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSRmdir_0005", "w+")���������ļ����õ����hFile
//@EXECUTIONFLOW: 4������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 5������CSUDIFSRmdir(��������·�� + "CSUDIFSRmdir_0005")ɾ���ļ���Ӧ��ʧ��
//@EXECUTIONFLOW: 6������CSUDIFSRemove(��������·�� + "CSUDIFSRmdir_0005")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRmdir_0005(void)
{
	char   szTest[] = "CSUDIFSRmdir_0005";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE   hFile = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "���������ļ�ʧ��\n");
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");	
	
	CSTK_ASSERT_TRUE(-1 == CSUDIFSRmdir(szFilePath), "ɾ���ļ���ʧ��\n");

	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "�Ƴ��ļ�ʧ��\n");
		}
	}
	return CSUDI_TRUE;		
	
}
/****************************CSUDIFSRmdir  End *******************************/

/****************************CSUDIFSOpenDir  Start *****************************/ 
//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpenDir(const char * pcPathName)
//@DESCRIPTION:  �������ļ���
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcPathName = ��������·�� + "CSUDIFSOpenDir_0001"
//@EXPECTATION: ����ֵ������CSUDI_NULL
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSMkdir(��������·�� + "CSUDIFSOpenDir_0001", 0)�����ļ���
//@EXECUTIONFLOW: 4������CSUDIFSOpenDir(��������·�� + "CSUDIFSOpenDir_0001")���ļ��У��õ����hDir
//@EXECUTIONFLOW: 5������CSUDIFSCloseDir(hDir)�ر��ļ���
//@EXECUTIONFLOW: 6������CSUDIFSRmdir(��������·�� + "CSUDIFSOpenDir_0001")ɾ���ļ���
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpenDir_0001(void)
{
	char   szTest[] = "CSUDIFSOpenDir_0001";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE     hDir = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSMkdir(szFilePath, 0), "�����ļ���ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hDir = CSUDIFSOpenDir(szFilePath)), "���ļ���ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSCloseDir(hDir), "�ر��ļ���ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szFilePath), "ɾ���ļ���ʧ��\n");
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpenDir(const char * pcPathName)
//@DESCRIPTION:  ���ļ�
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcPathName = ��������·�� + "CSUDIFSOpenDir_0002"
//@EXPECTATION: ����ֵ����CSUDI_NULL
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSOpenDir_0002", "w+")�����ļ����õ����hFile
//@EXECUTIONFLOW: 4������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 5������CSUDIFSOpenDir(��������·�� + "CSUDIFSOpenDir_0002")���ļ��У�Ӧ��ʧ��
//@EXECUTIONFLOW: 6������CSUDIFSRemove(��������·�� + "CSUDIFSOpenDir_0002")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpenDir_0002(void)
{
	char   szTest[] = "CSUDIFSOpenDir_0002";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE   hFile = CSUDI_NULL;
	CSUDI_HANDLE   hDir = CSUDI_NULL;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "���������ļ�ʧ��\n");
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == (hDir = CSUDIFSOpenDir(szFilePath)), "���ļ���Ӧ���ɹ�\n");

	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "�Ƴ��ļ�ʧ��\n");
		}
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpenDir(const char * pcPathName)
//@DESCRIPTION:  �򿪿�ָ��
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcPathName = CSUDI_NULL
//@EXPECTATION: ����ֵ����CSUDI_NULL
//@EXECUTIONFLOW: 1������CSUDIFSOpenDir(CSUDI_NULL)���ļ��У�Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpenDir_0003(void)
{
	CSTK_ASSERT_TRUE(CSUDI_NULL == CSUDIFSOpenDir(CSUDI_NULL), "���ļ��в�Ӧ���ɹ�\n");

	CSTK_FATAL_POINT
	{
		;	
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpenDir(const char * pcPathName)
//@DESCRIPTION:  �򿪲����ڵ��ļ���
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcPathName = ��������·�� + "CSUDIFSOpenDir_0004"
//@EXPECTATION: ����ֵ����CSUDI_NULL
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpenDir(��������·�� + "CSUDIFSOpenDir_0004")���ļ��У�Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpenDir_0004(void)
{
	char   szTest[] = "CSUDIFSOpenDir_0004";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);	

	CSTK_ASSERT_TRUE(CSUDI_NULL == CSUDIFSOpenDir(szFilePath), "�򿪲����ڵ��ļ��в�Ӧ���ɹ�\n");

	CSTK_FATAL_POINT
	{
		;	
	}
	return CSUDI_TRUE;
}
/****************************CSUDIFSOpenDir  End *******************************/

/****************************CSUDIFSCloseDir  Start *****************************/ 
//@CASEGROUP: int CSUDIFSCloseDir(CSUDI_HANDLE hDir)
//@DESCRIPTION:  �����ر��ļ���
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hDir = hDir1�� hDir1ΪCSUDIFSOpenDir()���صĺϷ�ֵ
//@EXPECTATION: ����ֵ����0
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSMkdir(��������·�� + "CSUDIFSCloseDir_0001", 0)�����ļ���
//@EXECUTIONFLOW: 4������CSUDIFSOpenDir(��������·�� + "CSUDIFSCloseDir_0001")���ļ���
//@EXECUTIONFLOW: 5������CSUDIFSCloseDir(hDir)�ر��ļ��У�Ӧ���ɹ�
//@EXECUTIONFLOW: 6������CSUDIFSRmdir(��������·�� + "CSUDIFSCloseDir_0001")ɾ���ļ���
CSUDI_BOOL CSTC_FS_IT_CSUDIFSCloseDir_0001(void)
{
	char   szTest[] = "CSUDIFSCloseDir_0001";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE     hDir1 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSMkdir(szFilePath, 0), "�����ļ���ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hDir1 = CSUDIFSOpenDir(szFilePath)), "���ļ���ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSCloseDir(hDir1), "�ر��ļ���ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szFilePath), "ɾ���ļ���ʧ��\n");
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSCloseDir(CSUDI_HANDLE hDir)
//@DESCRIPTION:  �رտ��ļ��о��
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hDir = CSUDI_NULL 
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSCloseDir(CSUDI_NULL)�ر��ļ��У�Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSCloseDir_0002(void)
{
	CSTK_ASSERT_TRUE(-1 == CSUDIFSCloseDir(CSUDI_NULL), "�ر��ļ��в�Ӧ�óɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSCloseDir(CSUDI_HANDLE hDir)
//@DESCRIPTION:  �ر��ļ����
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hDir = hFile, hDir1ΪCSUDIFSOpen()���صľ��
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSCloseDir_0003", "w+")�����ļ����õ����hFile
//@EXECUTIONFLOW: 4������CSUDIFSCloseDir(hFile)�ر��ļ��У�Ӧ��ʧ��
//@EXECUTIONFLOW: 5������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 6������CSUDIFSRemove(��������·�� + "CSUDIFSCloseDir_0003")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSCloseDir_0003(void)
{
	char   szTest[] = "CSUDIFSCloseDir_0003";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE   hFile = CSUDI_NULL;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "���������ļ�ʧ��\n");
	
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSCloseDir(hFile), "�ر��ļ���Ӧ���ɹ�\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");	

	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "�Ƴ��ļ�ʧ��\n");
		}
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSCloseDir(CSUDI_HANDLE hDir)
//@DESCRIPTION:  �رղ����ڵ��ļ��о��
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hDir = 0x7FFFFFFF(����0x7FFFFFFFΪ�����ڵ��ļ��о��)
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSCloseDir(0x7FFFFFFF)�ر��ļ��У�Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSCloseDir_0004(void)
{
	CSTK_ASSERT_TRUE(-1 == CSUDIFSCloseDir((CSUDI_HANDLE)0x7FFFFFFF), "�ر��ļ��в�Ӧ�óɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}
/****************************CSUDIFSCloseDir  End *******************************/

/****************************CSUDIFSReadDir  Start *****************************/ 
//@CASEGROUP: CSUDIFSDirent_S *CSUDIFSReadDir(CSUDI_HANDLE hDir)
//@DESCRIPTION:  ���ļ��У��ļ����²�Ϊ��
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hDir = hDir1, hDir1ΪCSUDIFSOpenDir()���صĺϷ�ֵ
//@EXPECTATION: ����ֵ������CSUDI_NULL
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSMkdir(��������·�� + "CSUDIFSReadDir_0001", 0)�����ļ���
//@EXECUTIONFLOW: 4������CSUDIFSMkdir(��������·�� + "CSUDIFSReadDir_0001" + "CSUDIFSReadDir_0001_sub", 0)�������ļ���
//@EXECUTIONFLOW: 5������CSUDIFSOpenDir(��������·�� + "CSUDIFSReadDir_0001")���ļ��У��õ����hDir1
//@EXECUTIONFLOW: 6������CSUDIFSReadDir(hDir1)���ļ��У�Ӧ���ɹ����ȽϽڵ���Ϣ���鿴���ļ������������Ƿ���ȷ
//@EXECUTIONFLOW: 7������CSUDIFSCloseDir(hDir1)�ر��ļ���
//@EXECUTIONFLOW: 8������CSUDIFSRmdir(��������·�� + "CSUDIFSReadDir_0001" + "CSUDIFSReadDir_0001_sub")ɾ�����ļ���
//@EXECUTIONFLOW: 9������CSUDIFSRmdir(��������·�� + "CSUDIFSReadDir_0001")ɾ���ļ���
CSUDI_BOOL CSTC_FS_IT_CSUDIFSReadDir_0001(void)
{
	char   szTest[] = "CSUDIFSReadDir_0001";   
	char   szSubTest[] = "CSUDIFSReadDir_0001_sub";
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	char   szSubFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE     hDir1 = CSUDI_NULL;
	CSUDIFSDirent_S  *psDirent = CSUDI_NULL;
	int   nRet = 0;
	int   nSubRet = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	memset(szSubFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szSubFilePath, szFilePath);
	strcat(szSubFilePath, "/");
	strcat(szSubFilePath, szSubTest);
	
	CSTK_ASSERT_TRUE_FATAL(0 == (nRet = CSUDIFSMkdir(szFilePath, 0)), "�����ļ���ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == (nSubRet = CSUDIFSMkdir(szSubFilePath, 0)), "�������ļ���ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hDir1 = CSUDIFSOpenDir(szFilePath)), "���ļ���ʧ��\n");

	while(1)
	{
		psDirent = CSUDIFSReadDir(hDir1);
		if(psDirent != CSUDI_NULL && 0 != strcmp(psDirent->m_cDirName, ".") && 0 != strcmp(psDirent->m_cDirName, ".."))
		{
			CSTK_ASSERT_TRUE_FATAL(0 == strcmp(psDirent->m_cDirName,szSubTest), "���ļ���ʧ��\n");
			CSTK_ASSERT_TRUE_FATAL(1 == psDirent->m_ucDirType, "���ļ���ʧ��\n");
			break;
		}
	}

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSCloseDir(hDir1), "�ر��ļ���ʧ��\n")

	CSTK_FATAL_POINT
	{
		if(nSubRet == 0)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szSubFilePath), "ɾ�����ļ���ʧ��\n");
		}	
		if(nRet == 0)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szFilePath), "ɾ���ļ���ʧ��\n");
		}	
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIFSDirent_S *CSUDIFSReadDir(CSUDI_HANDLE hDir)
//@DESCRIPTION: �����ļ����µ����н��
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hDir = hDir1, hDir1ΪCSUDIFSOpenDir()���صĺϷ�ֵ
//@EXPECTATION: ÿһ��������Ԥ�ڽ��
//@EXECUTIONFLOW:1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW:2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW:3������CSUDIFSMkdir�����ļ��У�Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:4��ѭ��32�ε���CSUDIFSMkdir�������ļ��У�Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:5��ѭ��256�ε���CSUDIFSOpen���������ļ���Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:6������CSUDIFSOpenDir���ļ��У�Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:7������CSUDIFSReadDir���ļ��У���������Ϊ130
//@EXECUTIONFLOW:8������CSUDIFSCloseDir�ر��ļ��У�Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:9��ѭ��32�ε���CSUDIFSRmdir�Ƴ����ļ��У�Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:10��ѭ��256�ε���CSUDIFSClose�ر��ļ���Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:11��ѭ��256�ε���CSUDIFSRemove�Ƴ��ļ���Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:12������CSUDIFSRmdir�Ƴ��ļ��У�Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSReadDir_0002(void)
{
	char szTest[] = "CSUDIFSReadDir_0002";
	char szSubTest[] = "CSUDIFSReadDir_0002_Sub";
	int nDirRet = -1;
	int anSubDirRet[FS_TEST_MAX_DIR_CNT];
	CSUDI_HANDLE ahFile[FS_TEST_MAX_FILE_CNT];
	int nNodeCount = 0;
	int i = 0;
	int nLen = 0;
	char szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	char szSubFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE hDir = CSUDI_NULL;
	CSUDIFSDirent_S *psDirent = CSUDI_NULL;

	for (i=0; i<FS_TEST_MAX_DIR_CNT; i++)
	{
		anSubDirRet[i] = -1;
	}
	for (i=0; i<FS_TEST_MAX_FILE_CNT; i++)
	{
		ahFile[i] = CSUDI_NULL;
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	memset(szSubFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szSubFilePath, szFilePath);
	strcat(szSubFilePath, "/");
	strcat(szSubFilePath, szSubTest);
	nLen = strlen(szSubFilePath);

	CSTK_ASSERT_TRUE_FATAL(0 == (nDirRet = CSUDIFSMkdir(szFilePath, 0)), "�����ļ���ʧ��");

	for (i=0; i<FS_TEST_MAX_DIR_CNT; i++)
	{
		sprintf(szSubFilePath+nLen, "_Dir_%03d", i+1);
		CSTK_ASSERT_TRUE_FATAL(0 == (anSubDirRet[i] = CSUDIFSMkdir(szSubFilePath, 0)), "�������ļ���ʧ��");
	}
	for (i=0; i<FS_TEST_MAX_FILE_CNT; i++)
	{
		sprintf(szSubFilePath+nLen, "_File_%03d", i+1);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (ahFile[i] = CSUDIFSOpen(szSubFilePath, "w+")), "���������ļ�ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hDir = CSUDIFSOpenDir(szFilePath)), "���ļ���ʧ��");
	while (1)
	{
		psDirent = CSUDIFSReadDir(hDir);
		if(psDirent == CSUDI_NULL )
		{
			break;
		}
		nNodeCount++;
	}

	CSTK_ASSERT_TRUE_FATAL((2+FS_TEST_MAX_DIR_CNT+FS_TEST_MAX_FILE_CNT) == nNodeCount, "������������");

	CSTK_FATAL_POINT
	if (hDir != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSCloseDir(hDir), "�ر��ļ���ʧ��");
	}
	for (i=0; i<FS_TEST_MAX_DIR_CNT; i++)
	{
		if (anSubDirRet[i] == 0)
		{
			sprintf(szSubFilePath+nLen, "_Dir_%03d", i+1);
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szSubFilePath), "�Ƴ����ļ���ʧ��");
		}
	}
	for (i=0; i<FS_TEST_MAX_FILE_CNT; i++)
	{
		if (ahFile[i] != CSUDI_NULL)
		{
			sprintf(szSubFilePath+nLen, "_File_%03d", i+1);
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(ahFile[i]), "�ر��ļ�ʧ��");
			CSTK_ASSERT_TRUE(0 == CSUDIFSRemove(szSubFilePath), "�Ƴ��ļ�ʧ��");
		}
	}
	if (nDirRet == 0)
	{
		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szFilePath), "�Ƴ��ļ���ʧ��");
	}	

	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIFSDirent_S *CSUDIFSReadDir(CSUDI_HANDLE hDir)
//@DESCRIPTION:  �ӿ��ļ��о���������Ϣ
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hDir = CSUDI_NULL
//@EXPECTATION: ����ֵ����CSUDI_NULL
//@EXECUTIONFLOW: 1������CSUDIFSReadDir(CSUDI_NULL)��ȡ�ļ��У�Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSReadDir_0003(void)
{
	CSTK_ASSERT_TRUE(CSUDI_NULL == CSUDIFSReadDir(CSUDI_NULL),"��ȡ�����ڵ��ļ��в�Ӧ���ɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIFSDirent_S *CSUDIFSReadDir(CSUDI_HANDLE hDir)
//@DESCRIPTION:  �Ӳ����ڵ��ļ��о����
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hDir = 0x7FFFFFFF(����0x7FFFFFFFΪ�����ڵ��ļ��о��)
//@EXPECTATION: ����ֵ����CSUDI_NULL
//@EXECUTIONFLOW: 1������CSUDIFSReadDir(0x7FFFFFFF)����ȡ�ļ��У�Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSReadDir_0004(void)
{
	CSTK_ASSERT_TRUE(CSUDI_NULL == CSUDIFSReadDir((CSUDI_HANDLE)0x7FFFFFFF),"��ȡ�����ڵ��ļ��в�Ӧ���ɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIFSDirent_S *CSUDIFSReadDir(CSUDI_HANDLE hDir)
//@DESCRIPTION:  ���ļ�
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hDir = hFile, hFileΪCSUDIFSOpen()���صľ��
//@EXPECTATION: ����ֵ����CSUDI_NULL
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSReadDir_0005", "w+")�����ļ����õ����hFile
//@EXECUTIONFLOW: 4������CSUDIFSReadDir(hFile)���ļ���
//@EXECUTIONFLOW: 5������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 6������CSUDIFSRemove(��������·�� + "CSUDIFSReadDir_0005")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSReadDir_0005(void)
{
	char   szTest[] = "CSUDIFSReadDir_0005";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE   hFile = CSUDI_NULL;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "���������ļ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == CSUDIFSReadDir(hFile), "���ļ���Ӧ���ɹ�\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");	

	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "�Ƴ��ļ�ʧ��\n");
		}
	}
	return CSUDI_TRUE;	
}
/****************************CSUDIFSReadDir  End *******************************/

/****************************CSUDIFSStatfs  Start *****************************/ 
//@CASEGROUP: int CSUDIFSStatfs(const char * pcPathName, CSUDIFSStatFs_S * psFsStat)
//@DESCRIPTION:  ͨ���ļ���ȡ�־���Ϣ
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcPathName = ��������·�� + "CSUDIFSStatfs_0001"
//@INPUT: psFsStat = &sFsStat������CSUDIFSStatFs_S  sFsStat
//@EXPECTATION:  ����ֵ����0
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·�� + "CSUDIFSStatfs_0001", "w+")���������ļ����õ����hFile
//@EXECUTIONFLOW: 4������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 5������CSUDIFSStatfs(��������·�� + "CSUDIFSStatfs_0001", &sFsStat)��ȡ�ļ�ϵͳ��Ϣ
//@EXECUTIONFLOW: 6������CSUDIFSRemove(��������·�� + "CSUDIFSStatfs_0001")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSStatfs_0001(void)
{
	char   szTest[] = "CSUDIFSStatfs_0001";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE   hFile = CSUDI_NULL;
	CSUDIFSStatFs_S  sFsStat;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "���������ļ�ʧ��\n");
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");	

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStatfs(szFilePath, &sFsStat), "��ȡ�ļ�ϵͳ��Ϣʧ��\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "�Ƴ��ļ�ʧ��\n");
		}
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSStatfs(const char * pcPathName, CSUDIFSStatFs_S * psFsStat)
//@DESCRIPTION:  ͨ���ļ��л�ȡ�־���Ϣ
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcPathName = ��������·�� + "CSUDIFSStatfs_0002"
//@INPUT: psFsStat = &sFsStat������CSUDIFSStatFs_S  sFsStat
//@EXPECTATION: ����ֵ����0
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSMkdir(��������·�� + "CSUDIFSStatfs_0002", 0)�����ļ���
//@EXECUTIONFLOW: 4������CSUDIFSStatfs(��������·�� + "CSUDIFSStatfs_0002", &sFsStat)��ȡ�ļ�ϵͳ��Ϣ��Ӧ���ɹ�
//@EXECUTIONFLOW: 5������CSUDIFSRmdir(��������·�� + "CSUDIFSStatfs_0002")ɾ���ļ���
CSUDI_BOOL CSTC_FS_IT_CSUDIFSStatfs_0002(void)
{
	char   szTest[] = "CSUDIFSStatfs_0002";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDIFSStatFs_S  sFsStat;

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSMkdir(szFilePath, 0), "�����ļ���ʧ��\n");

	CSTK_ASSERT_TRUE(0 == CSUDIFSStatfs(szFilePath, &sFsStat), "��ȡ�ļ�ϵͳ��Ϣʧ��\n");
	
	CSTK_ASSERT_TRUE(0 == CSUDIFSRmdir(szFilePath), "ɾ���ļ���ʧ��\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSStatfs(const char * pcPathName, CSUDIFSStatFs_S * psFsStat)
//@DESCRIPTION:  ͨ����ָ��·����ȡ�־���Ϣ
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcPathName = CSUDI_NULL
//@INPUT: psFsStat = &sFsStat������CSUDIFSStatFs_S  sFsStat
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSStatfs(CSUDI_NULL, &sFsStat)��ȡ�ļ�ϵͳ��Ϣ��Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSStatfs_0003(void)
{
	CSUDIFSStatFs_S  sFsStat;

	CSTK_ASSERT_TRUE(-1 == CSUDIFSStatfs(CSUDI_NULL, &sFsStat), "��ȡ�ļ�ϵͳ��Ϣ��Ӧ�óɹ�\n");	

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSStatfs(const char * pcPathName, CSUDIFSStatFs_S * psFsStat)
//@DESCRIPTION:  ͨ�������ڵ�·����ȡ�־���Ϣ
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcPathName = ��������·�� + "CSUDIFSStatfs_0004"
//@INPUT: psFsStat = &sFsStat������CSUDIFSStatFs_S  sFsStat
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSStatfs(��������·�� + "CSUDIFSStatfs_0004", &sFsStat)��ȡ�ļ�ϵͳ��Ϣ
CSUDI_BOOL CSTC_FS_IT_CSUDIFSStatfs_0004(void)
{
	char   szTest[] = "CSUDIFSStatfs_0004";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDIFSStatFs_S  sFsStat;

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE(-1 == CSUDIFSStatfs(szFilePath, &sFsStat), "��ȡ�ļ�ϵͳ��Ϣ��Ӧ���ɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSStatfs(const char * pcPathName, CSUDIFSStatFs_S * psFsStat)
//@DESCRIPTION:  ���շ־���Ϣ��ָ��Ϊ��
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: pcPathName = ��������·�� + "CSUDIFSStatfs_0005"
//@INPUT: psFsStat = &sFsStat������CSUDIFSStatFs_S  sFsStat
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSMkdir(��������·�� + "CSUDIFSStatfs_0005", 0)�����ļ���
//@EXECUTIONFLOW: 4������CSUDIFSStatfs(��������·�� + "CSUDIFSStatfs_0005", CSUDI_NULL)��ȡ�ļ�ϵͳ��Ϣ��Ӧ��ʧ��
//@EXECUTIONFLOW: 5������CSUDIFSRmdir(��������·�� + "CSUDIFSStatfs_0005")ɾ���ļ���
CSUDI_BOOL CSTC_FS_IT_CSUDIFSStatfs_0005(void)
{
	char   szTest[] = "CSUDIFSStatfs_0005";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSMkdir(szFilePath, 0), "�����ļ���ʧ��\n");
	CSTK_ASSERT_TRUE(-1 == CSUDIFSStatfs(szFilePath, CSUDI_NULL), "��ȡ�ļ�ϵͳ��Ϣ��Ӧ���ɹ�\n");
	CSTK_ASSERT_TRUE(0 == CSUDIFSRmdir(szFilePath), "ɾ���ļ���ʧ��\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}
/****************************CSUDIFSStatfs  End *******************************/

/****************************CSUDIFSFstatfs  Start *****************************/ 

static CSUDI_BOOL FS_iPrintPartitionType(CSUDIFSPartitionType_E eType)
{
	CSUDI_BOOL bRet = CSUDI_TRUE;

	switch(eType)
	{
		case EM_UDIFS_PARTITION_AUTO:
			CSFSPrint("�ӵײ��ȡ���ļ�ϵͳ���Ͳ���ΪEM_UDIFS_PARTITION_AUTO����\n");
			bRet = CSUDI_FALSE;
			break;
		case EM_UDIFS_PARTITION_CRAMFS:
			CSFSPrint("�ļ�ϵͳ����:CRAMFS�����\n");
			break;
		case EM_UDIFS_PARTITION_ROMFS:
			CSFSPrint("�ļ�ϵͳ����:ROMFS�����\n");
			break;
		case EM_UDIFS_PARTITION_FAT32:
			CSFSPrint("�ļ�ϵͳ����:FAT32���ͷ���\n");
			break;
		case EM_UDIFS_PARTITION_EXT2:
			CSFSPrint("�ļ�ϵͳ����:�ļ�ϵͳ����:E2FS���ͷ���\n");
			break;
		case EM_UDIFS_PARTITION_EXT3:
			CSFSPrint("�ļ�ϵͳ����:EXT3���ͷ���\n");
			break;
		case EM_UDIFS_PARTITION_JFFS2:
			CSFSPrint("�ļ�ϵͳ����:JFFS2���ͷ���\n");
			break;
		case EM_UDIFS_PARTITION_NTFS:
			CSFSPrint("�ļ�ϵͳ����:NTFS���ͷ���\n");
			break;
		case EM_UDIFS_PARTITION_UBIFS:
			CSFSPrint("�ļ�ϵͳ����:UBIFS���ͷ���\n");
			break;
		case EM_UDIFS_PARTITION_YAFFS2:
			CSFSPrint("�ļ�ϵͳ����:YAFFS2���ͷ���\n");
			break;
		default:
			CSFSPrint("��ȡ�ļ�ϵͳ���ʹ���\n");
			bRet = CSUDI_FALSE;
			break;
	}

	return bRet;
}

//@CASEGROUP: int CSUDIFSFstatfs(CSUDI_HANDLE hFile, CSUDIFSStatFs_S * psFsStat)
//@DESCRIPTION:  ������ȡ�־���Ϣ
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = hFile1��hFile1Ϊ�Ϸ����ļ��������CSUDIOpen()����
//@INPUT: psFsStat = &sFsStat������CSUDIFSStatFs_S  sFsStat
//@EXPECTATION: ��ȡ�ɹ����ҷ־��ܴ�С�����ÿռ䡢���д�С�������һ��
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·��+"CSUDIFSFstatfs_0001", "w+")���������ļ����õ����hFile1
//@EXECUTIONFLOW: 4������CSUDIFSFstatfs(hFile1, &sFsStat)��ȡ�ļ�ϵͳ��Ϣ��Ӧ����ȡ�ɹ����ҷ־��ܴ�С�����ÿռ䡢���д�С�������һ��
//@EXECUTIONFLOW: 5������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 6������CSUDIFSRemove(��������·��+"CSUDIFSFstatfs_0001")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFstatfs_0001(void)
{
	//�ֶ�����
	char   szTest[] = "CSUDIFSFstatfs_0001";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	CSUDIFSStatFs_S  sFsStat;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(szFilePath, "w+")), "���������ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSFstatfs(hFile1, &sFsStat), "��ȡ�ļ�ϵͳ��Ϣʧ��\n");

	CSFSPrint("�־���Ϣ����:\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == FS_iPrintPartitionType(sFsStat.m_eFsType), "��ȡ�ļ�ϵͳ���ʹ���\n");
	CSFSPrint("���С: %ld  �ܿ���: %ld  ���п���: %ld  �ǳ����û��Ϸ����ÿ�����%ld \n", sFsStat.m_lBsize, sFsStat.m_lBlocks, sFsStat.m_lBfree, sFsStat.m_lBavail);

	CSFSPrint("�־��ܴ�С��%.3f Byte ���д�С: %.3f Byte �ǳ����û����ô�С: %.3f Byte \n", (double)sFsStat.m_lBlocks*sFsStat.m_lBsize, (double)sFsStat.m_lBfree*sFsStat.m_lBsize , (double)sFsStat.m_lBavail*sFsStat.m_lBsize);
	CSFSPrint("������Ϣ�Ƿ�ͷ־���PC����һ�£��������Y, ���������N\n");

	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "��ѡ�����ļ�ϵͳ��Ϣ���ԣ�����û��ͨ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");	
	
	
	
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "�Ƴ��ļ�ʧ��\n");
		}
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSFstatfs(CSUDI_HANDLE hFile, CSUDIFSStatFs_S * psFsStat)
//@DESCRIPTION:  �ӿ��ļ������ȡ�־���Ϣ
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = CSUDI_NULL
//@INPUT: psFsStat = &sFsStat������CSUDIFSStatFs_S  sFsStat
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSFstatfs(CSUDI_NULL, &sFsStat)��ȡ�ļ�ϵͳ��Ϣ��Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFstatfs_0002(void)
{
	CSUDIFSStatFs_S  sFsStat;
	
	CSTK_ASSERT_TRUE(-1 == CSUDIFSFstatfs(CSUDI_NULL, &sFsStat), "��ȡ�ļ�ϵͳ��Ϣ��Ӧ���ɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSFstatfs(CSUDI_HANDLE hFile, CSUDIFSStatFs_S * psFsStat)
//@DESCRIPTION:  �����ڵľ����ȡ�־���Ϣ
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = 0x7FFFFFFF(����0x7FFFFFFFΪ�Ƿ����ļ����)
//@INPUT: psFsStat = &sFsStat������CSUDIFSStatFs_S  sFsStat
//@EXPECTATION: ����ֵ����-1
//@EXECUTIONFLOW: 1������CSUDIFSFstatfs(0x7FFFFFFF, &sFsStat)��ȡ�ļ�ϵͳ��Ϣ��Ӧ��ʧ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFstatfs_0003(void)
{
	CSUDIFSStatFs_S  sFsStat;
	
	CSTK_ASSERT_TRUE(-1 == CSUDIFSFstatfs((CSUDI_HANDLE)0x7FFFFFFF, &sFsStat), "��ȡ�ļ�ϵͳ��Ϣ��Ӧ���ɹ�\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSFstatfs(CSUDI_HANDLE hFile, CSUDIFSStatFs_S * psFsStat)
//@DESCRIPTION:  ͨ���ļ��о����ȡ�־���Ϣ
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = hDir, hDirΪCSUDIFSMkdir()���صľ��
//@INPUT: psFsStat = &sFsStat������CSUDIFSStatFs_S  sFsStat
//@EXPECTATION: ��ȡ�־���Ϣʧ��
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSMkdir(��������·�� + "CSUDIFSFstatfs_0004", 0)�����ļ���
//@EXECUTIONFLOW: 4������CSUDIFSOpenDir(��������·�� + "CSUDIFSFstatfs_0004")���ļ��У��õ����hDir
//@EXECUTIONFLOW: 5������CSUDIFSStatfs(hDir, &sFsStat)��ȡ�ļ�ϵͳ��Ϣ��Ӧ��ʧ�ܣ�����ֵΪ-1
//@EXECUTIONFLOW: 6������CSUDIFSCloseDir(hDir)�ر��ļ���
//@EXECUTIONFLOW: 7������CSUDIFSRmdir(��������·�� + "CSUDIFSFstatfs_0004")ɾ���ļ���
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFstatfs_0004(void)
{
	char   szTest[] = "CSUDIFSFstatfs_0004";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE   hDir = CSUDI_NULL;
	CSUDIFSStatFs_S  sFsStat;
	int    nRet = -1;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);	

	CSTK_ASSERT_TRUE_FATAL(0 == (nRet = CSUDIFSMkdir(szFilePath, 0)), "�����ļ���ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hDir = CSUDIFSOpenDir(szFilePath)), "���ļ���ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSFstatfs(hDir, &sFsStat), "��ȡ�ļ�����Ϣ��Ӧ�óɹ�\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSCloseDir(hDir), "�ر��ļ���ʧ��\n");

	CSTK_FATAL_POINT
	{
		if(nRet == 0)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szFilePath), "ɾ���ļ���ʧ��\n");
		}
	}
	
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSFstatfs(CSUDI_HANDLE hFile, CSUDIFSStatFs_S * psFsStat)
//@DESCRIPTION:  ���շ־���Ϣ��ָ��Ϊ��
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile = hFile1��hFile1Ϊ�Ϸ����ļ��������CSUDIOpen()����
//@INPUT: psFsStat = CSUDI_NULL
//@EXPECTATION: ��ȡ�־���Ϣʧ��
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·��+"CSUDIFSFstatfs_0005", "w+")���������ļ����õ����hFile1
//@EXECUTIONFLOW: 4������CSUDIFSFstatfs(hFile1, CSUDI_NULL)��ȡ�ļ���Ϣ��Ӧ��ʧ��
//@EXECUTIONFLOW: 5������CSUDIFSClose(hFile1)�ر��ļ�
//@EXECUTIONFLOW: 6������CSUDIFSRemove(��������·��+"CSUDIFSFstatfs_0005")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFstatfs_0005(void)
{
	char   szTest[] = "CSUDIFSFstatfs_0005";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(szFilePath, "w+")), "���������ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSFstatfs(hFile1, CSUDI_NULL), "��ȡ�ļ�ϵͳ��Ϣ��Ӧ���ɹ�\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile1), "�ر��ļ�ʧ��\n");	
	
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSRemove(szFilePath), "�Ƴ��ļ�ʧ��\n");
		}
	}
	return CSUDI_TRUE;	
}

//@DESCRIPTION: ����ϵͳ��д�ļ�������
//@PRECONDITION: ����ϵͳ���ļ��Ķ�ȡ��д���Ի���ļ��Ķ�д����
//@INPUT: hFileΪ�Ϸ����ļ��������CSUDIOpen()����
//@INPUT: pucMBuffer,pucKBuffer�ļ�д��Ͷ�ȡ��ʹ�õ��ڴ滺����
//@EXPECTATION: ��ȡ�־���Ϣʧ��
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·��+"FSWriteReadTest", "w+")���������ļ����õ����hFile
//@EXECUTIONFLOW: 4������CSUDIFSWrite(),ÿ��д��1MB��ѭ��10��,����ȡ����ʱ��
//@EXECUTIONFLOW: 5������CSUDIFSRead(), ÿ�ζ���1MB��ѭ��10��,����ȡ����ʱ��
//@EXECUTIONFLOW: 6������CSUDIFSWrite(),ÿ��д��10K��ѭ��100��,����ȡ����ʱ��
//@EXECUTIONFLOW: 7������CSUDIFSRead(),ÿ��д��10K��ѭ��100��,����ȡ����ʱ��
//@EXECUTIONFLOW: 8����ӡ���н������������Դ
CSUDI_BOOL CSTC_FS_IT_WriteReadPerformance_0001(void)
{
//	CSUDI_HANDLE hFile = CSUDI_NULL;
//	char szTest[] = "FSWriteReadTest";
//	char cField[64];
//	CSUDIFSPartitionType_E ePartitionType = EM_UDIFS_PARTITION_JFFS2;
//	int uReadWriteByMB = 1*M;
//	int uReadWriteByKB = 10*K;
//	UINT32 uLoopTimeByMB = 10;
//	UINT32 uLoopTimeByKB = 100;
//	UINT32 uTestStartAddress = 0;
//	UINT32 uTestFlashSize = 0;
//	char *pcBufferByMB = CSUDI_NULL;
//	char *pcBufferByKB = CSUDI_NULL;
//	CSUDIOSTimeVal_S sStartTime;
//	CSUDIOSTimeVal_S sStopTime;
//	CSUDIOSTimeVal_S sCostTime;
//	memset(&sStartTime, 0, sizeof(sStartTime));
//	memset(&sStopTime, 0, sizeof(sStopTime));
//	memset(&sCostTime, 0, sizeof(sCostTime));
//	int i = 0;
//	int j = 0;
//	float lSpeedPerSecond = 0;
//
//	memset(cField, '\0', sizeof(cField));
//	
//	if(CS_TK_CONFIG_SUCCESS != CSTKGetConfigInfo("FS", "CS_FSUDI2_TEST_START_ADDRESS", cField, sizeof(cField)))
//	{
//		CSFSPrint("��ȡ�����ļ��е�CS_FSUDI2_TEST_START_ADDRESS�ֶ�ʧ��\n");
//		return CSUDI_FALSE;
//	}
//	
//	uTestStartAddress = CSTKGetIntFromStr(cField, 16);
//
//	memset(cField, '\0', sizeof(cField));
//	
//	if(CS_TK_CONFIG_SUCCESS != CSTKGetConfigInfo("FS", "CS_FSUDI2_TEST_FLASH_SIZE", cField, sizeof(cField)))
//	{
//		CSFSPrint("��ȡ�����ļ��е�CS_FSUDI2_TEST_FLASH_SIZE�ֶ�ʧ��\n");
//		return CSUDI_FALSE;
//	}
//	
//	uTestFlashSize = CSTKGetIntFromStr(cField, 16);
//
//	memset(cField, '\0', sizeof(cField));
//	
//	if(CS_TK_CONFIG_SUCCESS != CSTKGetConfigInfo("FS", "CS_FSUDI2_TEST_FS_BLOCK", cField, sizeof(cField)))
//	{
//		CSFSPrint("��ȡ�����ļ��е�CS_FSUDI2_TEST_FS_BLOCK�ֶ�ʧ��\n");
//		return CSUDI_FALSE;
//	}	
//
//	CSFSPrint("���ڲ���ָ��flash��������ȴ�!!!\n");
//
//	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(uTestStartAddress, uTestFlashSize), "����ָ��flash����ʧ��!!!");
//
//	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSAddPartition(cField, ePartitionType), "���ط����豸ʧ��!!!");
//
//	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSUDIFSGetDeviceVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");
//
//	strcat(g_szVolumeMountPoint, szTest);
//
//	pcBufferByMB= (char *)CSUDIOSMalloc(uReadWriteByMB);
//	
//    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pcBufferByMB, "����1M�ڴ�ʧ��");
//	
//    memset(pcBufferByMB, 0x55, uReadWriteByMB);
//
//	hFile = CSUDIFSOpen(g_szVolumeMountPoint, "w+");
//
//    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hFile, "���������ļ�ʧ��\n");
//
//	CSFSPrint("���ڽ�����1MBΪ��λ��д���������ȴ�!!!\n");
//    //ÿ��д��1MB��ѭ��10��
//    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sStartTime), "��ȡ��1MBΪ��λ��д������Ŀ�ʼʱ��ʧ��");
//	
//	if(uTestFlashSize >= (uLoopTimeByMB * uReadWriteByMB))
//	{
//	    for( i = 0; i < uLoopTimeByMB; i++)
//	    {	
//	    	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile, i*uReadWriteByMB, CSUDIFS_SEEK_SET), "���Ĳ������ļ�ʧ��\n");
//	        CSTK_ASSERT_TRUE_FATAL(uReadWriteByMB == CSUDIFSWrite(hFile, pcBufferByMB, uReadWriteByMB), "���Ĳ�д���ļ�ʧ��\n");
//	    }
//	}
//	else
//	{
//		CSTK_ASSERT_TRUE_FATAL(uReadWriteByMB <= uTestFlashSize, "���صķ���size��С��û�취�������ܲ���\n");
//		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile, 0, CSUDIFS_SEEK_SET), "���Ĳ������ļ�ʧ��\n");
//		for( i = 0; i < uLoopTimeByMB; i++)
//		{
//			CSTK_ASSERT_TRUE_FATAL(uReadWriteByMB == CSUDIFSWrite(hFile, pcBufferByMB, uReadWriteByMB), "���Ĳ�д���ļ�ʧ��");;
//		}
//	}
//	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sStopTime), "��ȡ��1MBΪ��λ��д������Ľ���ʱ��ʧ��");
//	
//	sCostTime.m_nSecond = sStopTime.m_nSecond- sStartTime.m_nSecond;
//	sCostTime.m_nMiliSecond = sStopTime.m_nMiliSecond- sStartTime.m_nMiliSecond;
//	sCostTime.m_lUSecond = sStopTime.m_lUSecond- sStartTime.m_lUSecond;
//
//	lSpeedPerSecond = uLoopTimeByMB*1000.00/((sStopTime.m_nSecond*1000.00+sStopTime.m_nMiliSecond)-(sStartTime.m_nSecond*1000.00+sStartTime.m_nMiliSecond));
//	
//	CSFSPrint("ÿ��д��1MB��ѭ��10�ε�����Ϊ:%.2f MB/s\r\n\n", lSpeedPerSecond);
//
//	CSFSPrint("���ڽ�����1MBΪ��λ�Ķ�ȡ��������ȴ�!!!\n");
//    //ÿ�ζ���1MB,ѭ��10��
//    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sStartTime), "��ȡ��1MBΪ��λ�Ķ�������Ŀ�ʼʱ��ʧ��");
//
//	if(uTestFlashSize >= (uLoopTimeByMB * uReadWriteByMB))
//	{
//	    for( i = 0; i < uLoopTimeByMB; i++)
//	    {	
//	    	memset(pcBufferByMB, 0x00, uReadWriteByMB);
//	    	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile, i*uReadWriteByMB, CSUDIFS_SEEK_SET), "���Ĳ������ļ�ʧ��\n");
//	        CSTK_ASSERT_TRUE_FATAL(uReadWriteByMB == CSUDIFSRead(hFile, pcBufferByMB, uReadWriteByMB), "���Ĳ���ȡ�ļ�ʧ��\n");
//			for(j = 0; j < uReadWriteByMB; j++)
//			{
//            	CSTK_ASSERT_TRUE_FATAL(0x55 == pcBufferByMB[j], "���岽У������ʧ��\n");
//        	}
//	    }
//	}
//	else
//	{
//		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile, 0, CSUDIFS_SEEK_SET), "���Ĳ������ļ�ʧ��\n");
//		for( i = 0; i < uLoopTimeByMB; i++)
//		{
//			memset(pcBufferByMB, 0x00, uReadWriteByMB);
//			CSTK_ASSERT_TRUE_FATAL(uReadWriteByMB == CSUDIFSRead(hFile, pcBufferByMB, uReadWriteByMB), "���Ĳ���ȡ�ļ�ʧ��");;
//			for(j = 0; j < uReadWriteByMB; j++)
//			{
//            	CSTK_ASSERT_TRUE_FATAL(0x55 == (UINT8)pcBufferByMB[j], "���岽У������ʧ��\n");
//        	}
//		}
//	}
//	
//	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sStopTime), "��ȡ��1MBΪ��λ�Ķ�ȡ�����Ľ���ʱ��ʧ��");
//	
//	sCostTime.m_nSecond = sStopTime.m_nSecond- sStartTime.m_nSecond;
//	sCostTime.m_nMiliSecond = sStopTime.m_nMiliSecond- sStartTime.m_nMiliSecond;
//	sCostTime.m_lUSecond = sStopTime.m_lUSecond- sStartTime.m_lUSecond;
//	
//	lSpeedPerSecond = uLoopTimeByMB*1000.00/((sStopTime.m_nSecond*1000.00+sStopTime.m_nMiliSecond)-(sStartTime.m_nSecond*1000.00+sStartTime.m_nMiliSecond));
//	
//	CSFSPrint("ÿ�ζ�ȡ1MB��ѭ��10�ε�����Ϊ:%.2f MB/s\r\n\n", lSpeedPerSecond);
//	
//	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");
//
//	CSFSPrint("���ڽ�����10KBΪ��λ��д���������ȴ�!!!\n");
//    //ÿ��д��10KB��ѭ��100��
//	pcBufferByKB = (char *)CSUDIOSMalloc(uReadWriteByKB);
//	
//    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pcBufferByKB,"�����ڴ�2ʧ��");
//	
//    memset(pcBufferByKB, 0x88, uReadWriteByKB);
//	
//	hFile = CSUDIFSOpen(g_szVolumeMountPoint, "w+");
//	
//    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hFile, "���ļ�ʧ��\n");
//
//    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sStartTime), "��ȡ��10KBΪ��λ��д������Ŀ�ʼʱ��ʧ��");
//	
//    for(i =0; i < uLoopTimeByKB; i++)
//    {
//    	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile, i*uLoopTimeByKB , CSUDIFS_SEEK_SET), "�����������ļ�ʧ��\n");
//        CSTK_ASSERT_TRUE_FATAL(uReadWriteByKB == CSUDIFSWrite(hFile, pcBufferByKB, uReadWriteByKB), "������д���ļ�ʧ��\n");
//    }
//
//	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sStopTime), "��ȡ��10KBΪ��λ��д������Ľ���ʱ��ʧ��");
//	
//	sCostTime.m_nSecond = sStopTime.m_nSecond- sStartTime.m_nSecond;
//	sCostTime.m_nMiliSecond = sStopTime.m_nMiliSecond- sStartTime.m_nMiliSecond;
//	sCostTime.m_lUSecond = sStopTime.m_lUSecond- sStartTime.m_lUSecond;
//	
//	lSpeedPerSecond = (uLoopTimeByKB*uReadWriteByKB)*1000.00/(((sStopTime.m_nSecond*1000.00+sStopTime.m_nMiliSecond)-(sStartTime.m_nSecond*1000.00+sStartTime.m_nMiliSecond))*M);
//	
//	CSFSPrint("ÿ��д��10KB��ѭ��100�ε�����Ϊ:%.2f MB/s\r\n\n", lSpeedPerSecond);
//
//    //ÿ�ζ���10KB��ѭ��100��
//    CSFSPrint("���ڽ�����10KBΪ��λ�Ķ�ȡ��������ȴ�!!!\n");
//    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sStartTime), "��ȡ��10KBΪ��λ��д������Ŀ�ʼʱ��ʧ��");
//	
//	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile, 0 , CSUDIFS_SEEK_SET), "���߲������ļ�ʧ��\n");
//	
//    for(i =0; i < uLoopTimeByKB; i++)
//    {  
//    	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile, i*uLoopTimeByKB , CSUDIFS_SEEK_SET), "�����������ļ�ʧ��\n");
//        CSTK_ASSERT_TRUE_FATAL(uReadWriteByKB == CSUDIFSRead(hFile, pcBufferByKB, uReadWriteByKB), "���߲���ȡ����ʧ��\n");
//
//        for(j = 0; j < uReadWriteByKB; j++)
//        {
//            CSTK_ASSERT_TRUE_FATAL(0x88 == (UINT8)pcBufferByKB[j] , "���߲�У������ʧ��\n");
//        }
//    }
//	
//	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sStopTime), "��ȡ��10KBΪ��λ��д������Ľ���ʱ��ʧ��");
//	
//	sCostTime.m_nSecond = sStopTime.m_nSecond- sStartTime.m_nSecond;
//	sCostTime.m_nMiliSecond = sStopTime.m_nMiliSecond- sStartTime.m_nMiliSecond;
//	sCostTime.m_lUSecond = sStopTime.m_lUSecond- sStartTime.m_lUSecond;
//	
//	lSpeedPerSecond = (uLoopTimeByKB*uReadWriteByKB)*1000.00/(((sStopTime.m_nSecond*1000.00+sStopTime.m_nMiliSecond)-(sStartTime.m_nSecond*1000.00+sStartTime.m_nMiliSecond))*M);
//	
//	CSFSPrint("ÿ�ζ�ȡ10KB��ѭ��100�ε�����Ϊ:%.2f MB/s\r\n\n", lSpeedPerSecond);
//   	
//	CSTK_FATAL_POINT
//	{
//		if(pcBufferByMB != CSUDI_NULL)
//		{
//	    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pcBufferByMB),"������Դ1ʧ��");
//		}
//		if(pcBufferByKB != CSUDI_NULL)
//		{
//	    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pcBufferByKB),"������Դ2ʧ��");
//		}
//		if(hFile != CSUDI_NULL)
//		{
//			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");
//		}
//		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRemove(g_szVolumeMountPoint), "�Ƴ����ص�ʧ��\n");
//	}	
	return CSUDI_TRUE;	
}


//@DESCRIPTION: USB�¶Գ��ļ����ļ���֧��
//@PRECONDITION: ����ϵͳ�Գ��ļ����ļ��Ĵ򿪣�д��رղ��������ļ���������8.3��ʽ�⣬��Ҫ��֧�����ģ�Ӣ�ģ����ż���8.3��ʽ���ļ�?
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFileΪ�Ϸ����ļ��������CSUDIOpen()����
//@EXPECTATION:  �򿪣�д��͹رղ����ɹ�
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·��+"coshipfs.txt", "w+")���������ļ����õ����hFile
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile, "coship", strlen("coship"))д������
//@EXECUTIONFLOW: 5������CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");
//@EXECUTIONFLOW: 6������CSUDIFSOpen(��������·��+"������ͬ�޵��ӻ�ӭ��.txt", "w+")���������ļ����õ����hFile
//@EXECUTIONFLOW: 7������CSUDIFSWrite(hFile, "������ͬ�޵��ӻ�ӭ��.txt", strlen("������ͬ�޵��ӻ�ӭ��.txt"))д������
//@EXECUTIONFLOW: 8������CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");
//@EXECUTIONFLOW: 9������CSUDIFSOpen(��������·��+"������ͬ�޵��ӻ�ӭ�� welcome to coship 2012-12-12.txt", "w+")���������ļ����õ����hFile	
//@EXECUTIONFLOW: 10������CSUDIFSWrite(hFile, "������ͬ�޵��ӻ�ӭ�� welcome to coship 2012-12-12.txt", strlen("������ͬ�޵��ӻ�ӭ�� welcome to coship 2010-12-12.txt"))д������
//@EXECUTIONFLOW: 11������CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");
//@EXECUTIONFLOW: 12������CSUDIFSRemove(��������·��+"CSUDIFSFstat_0001")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_LongNameSupport_0001(void)
{
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	char   szFilenamea[] = "coshipfs.txt";   
	char   szFilenameb[] = "������ͬ�޵��ӻ�ӭ��.txt";  
	char   szFilenamec[] = "������ͬ�޵��ӻ�ӭ�� welcome to coship 2012-12-12.txt";  
	char   szBuf[100];
	CSUDI_HANDLE   hFile = CSUDI_NULL;
	int    nLen = 0;
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");
	
	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szFilenamea);	
	nLen = strlen(szFilenamea);

	memset(szBuf,0,sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "���������ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(strlen("coship") == CSUDIFSWrite(hFile, "coship", strlen("coship")), "д���ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "r")), "���ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL( strlen("coship")==CSUDIFSRead(hFile, szBuf, strlen("coship")), "��ȡ����ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == memcmp("coship", szBuf,strlen("coship")), "�������ݲ���д��ʱ���ִ�\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");
    CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "�Ƴ��ļ�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	memset(szBuf,0,sizeof(szBuf)); 
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szFilenameb);
	nLen = strlen(szFilenameb);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "���������ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nLen== CSUDIFSWrite(hFile, szFilenameb, nLen ), "д���ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "r")), "���ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nLen==CSUDIFSRead(hFile, szBuf, nLen), "��ȡ����ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == memcmp(szFilenameb, szBuf,nLen), "�������ݲ���д��ʱ���ִ�\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");	
    CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "�Ƴ��ļ�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	memset(szBuf,0,sizeof(szBuf));
	strcpy(szFilePath, g_szVolumeMountPoint);
	nLen = strlen(szFilenamec);
	strcat(szFilePath, szFilenamec);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "���������ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szFilenamec, nLen), "д���ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "r")), "���ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nLen==CSUDIFSRead(hFile, szBuf, nLen), "��ȡ����ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == memcmp(szFilenamec, szBuf,nLen), "�������ݲ���д��ʱ���ִ�\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");	
    CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "�Ƴ��ļ�ʧ��\n");
    hFile = CSUDI_NULL;

	CSTK_FATAL_POINT
	if(hFile != CSUDI_NULL)
	{
	CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "�Ƴ��ļ�ʧ��\n");
	}
	return CSUDI_TRUE;	
}

//@DESCRIPTION: ��ȡ�ļ����Ե�ָ�룬����ļ�ָ���λ�ã�ȷ���ļ�ָ����ȷ
//@DESCRIPTION:  �������ļ�������Ϣ
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: hFile Ϊ�Ϸ����ļ��������CSUDIOpen()����
//@INPUT: psFileStat = &sFileStat������CSUDIFSFileStat_S sFileStat
//@EXPECTATION: �ļ�ָ��λ����ȷ��CSUDIFSCSUDIFSTell()��ȡ��ƫ�����붨���ƫ����һ��
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�һ���ѹ��صķ־��豸id	
//@EXECUTIONFLOW: 2������CSUDIFSGetDeviceInfo()��÷־�Ĺ���·��	
//@EXECUTIONFLOW: 3������CSUDIFSOpen(��������·��+"CSTC_FS_IT_FilePointCheck", "w+")���������ļ����õ����hFile
//@EXECUTIONFLOW: 4������CSUDIFSWrite(hFile, "CSTC_FS_IT_FilePointCheck", strlen("CSTC_FS_IT_FilePointCheck"))д������
//@EXECUTIONFLOW: 5������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 6������CSUDIFSOpen(��������·��+"CSTC_FS_IT_FilePointCheck", "w+")���ļ����õ����hFile
//@EXECUTIONFLOW: 7������CSUDIFSFstat(hFile, &sFileStat)��ȡ�ļ�������Ϣ
//@EXECUTIONFLOW: 8������CSUDIFSCSUDIFSTell()����ȡ�ļ���ǰλ���ж��Ƿ����ļ�ͷ
//@EXECUTIONFLOW: 9������CSUDIFSSeek()�����ô��ļ���ͷ��CSUDIFS_SEEK_SET�ƶ�ƫ��loffset���������سɹ�
//@EXECUTIONFLOW: 10������CSUDIFSCSUDIFSTell()����ȡ�ļ���ǰλ��
//@EXECUTIONFLOW: 11���жϵ�ǰλ�ú�ƫ��loffset�Ƿ����
//@EXECUTIONFLOW: 12������CSUDIFSClose(hFile)�ر��ļ�
//@EXECUTIONFLOW: 13������CSUDIFSRemove(��������·��+"CSTC_FS_IT_FilePointCheck")ɾ���ļ�
CSUDI_BOOL CSTC_FS_IT_FilePointCheck_0001(void)
{
	char   szTest[] = "CSTC_FS_IT_FilePointCheck";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE   hFile = CSUDI_NULL;
	int    nLen = strlen(szTest);
	long    nlofflen = 0;
	CSUDIFSFileStat_S  sFileStat;
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);	
     
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "���������ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "д���ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");	
	hFile = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "���ļ�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSFstat(hFile, &sFileStat), "��ȡ�ļ���Ϣʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0==CSUDIFSTell( hFile),"�ļ�ָ��λ�ô���\n");

	nlofflen = sFileStat.m_lSize/2;
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile, nlofflen, CSUDIFS_SEEK_SET),"�ļ�ָ��seek ����ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(nlofflen==CSUDIFSTell( hFile),"�ļ�ָ��λ�ô���\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��\n");	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "�Ƴ��ļ�ʧ��\n");
		}
	}
	return CSUDI_TRUE;		
}

//@CASEGROUP: �����ļ�ϵͳ
//@DESCRIPTION: ���Ա����ļ�ϵͳֻ����д���ص�
//@PRECONDITION: FS��FLASHģ��֮ǰ�ѳɹ���ʼ��
//@PRECONDITION: ϵͳ�д��ڷ־��豸
//@INPUT: �����ļ�ϵͳ��д����·��
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@EXECUTIONFLOW:1������CSUDIFSGetAllDeviceId()�õ������ѹ��صķ־��豸id	
//@EXECUTIONFLOW:2�����ú��ж��豸�Ƿ�Ϊ���ƶ��豸
//@EXECUTIONFLOW:3��������ǿ��ƶ��豸����CSUDIFSGetDeviceInfo��÷־�Ĺ���·��
//@EXECUTIONFLOW:4������CSUDIFSStatfs��ȡ�־���Ϣ���ж��ļ�ϵͳ����
//@EXECUTIONFLOW:5������ļ�ϵͳ��ֻ�����������CSUDIFSOpen(szFilePath, "w+"))�����ļ�,Ҫ�󷵻�ʧ��
//@EXECUTIONFLOW:6������ļ�ϵͳ��ֻ�����������strcmp�������ļ��ȽϷ־��С���ļ�ϵͳ����
//@EXECUTIONFLOW:7������ļ�ϵͳ�ǿɶ�д���������strcmp�������ļ��ȽϷ־��С���ļ�ϵͳ����
//@EXECUTIONFLOW:8�������з־��豸�ظ�����2~7
//@EXECUTIONFLOW:9��Ҫ�󱾵��ļ�ϵͳ����ֻ���Ϳ�д�־�
CSUDI_BOOL CSTC_FS_IT_LocalFsCheck(void)
{
	CSUDI_UINT32 dwIds[32];
	int nActIdCnt = 0; 
	int i = 0;
	int nLen = 0;
	char szTest[] = "LocalFsCheck"; 
	char szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDIFSDeviceInfo_S sDeviceInfo;
	CSUDIFSStatFs_S sFsStat;
	BOOL bHaveReadOnlyFs = CSUDI_FALSE;
	BOOL bHaveReadWriteFs = CSUDI_FALSE;
	CSUDI_HANDLE hFile = CSUDI_NULL;
	int nRetMkdir = -1;

	FS_iInitCfg();
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt), "��ȡ�豸idʧ��");
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt >= 2, "��ȡ�豸��Ŀ���ٴ���2");

	for (i = 0; i < nActIdCnt; i++)
	{
		if (CSUDI_ISREMOVEABLE(dwIds[i]))
		{
			continue;
		}

		memset(&sDeviceInfo, 0x00, sizeof(CSUDIFSDeviceInfo_S));
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwIds[i], &sDeviceInfo), "��ȡ�־���Ϣʧ��");
		if (sDeviceInfo.m_eDeviceType != EM_UDIFS_DEVTYPE_VOLUME)
		{
			continue;
		}

		CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "�־��豸���ص����");
        memset(&sFsStat,0,sizeof(sFsStat));
		CSUDIFSStatfs(sDeviceInfo.m_szMountPoint, &sFsStat);
		CSTCPrint("m_lBsize:%d, m_lBlocks:%d\n", sFsStat.m_lBsize, sFsStat.m_lBlocks);

		if ((sFsStat.m_eFsType > EM_UDIFS_PARTITION_READONLY_START) && (sFsStat.m_eFsType < EM_UDIFS_PARTITION_READONLY_END))
		{
			CSTK_ASSERT_TRUE_FATAL(!strcmp(sDeviceInfo.m_szMountPoint, g_aucLocalFsReadOnlyPath), "����ֻ���־���ص��������ļ���ͬ");
			CSTK_ASSERT_TRUE_FATAL(sFsStat.m_lBsize*sFsStat.m_lBlocks/1024 == g_nLocalFsReadOnlySizeKByte, "����ֻ���־��С�������ļ���ͬ");
			bHaveReadOnlyFs = CSUDI_TRUE;
			nLen = strlen(sDeviceInfo.m_szMountPoint);
			memset(szFilePath, 0x00, CSUDI_MAX_FILE_NAME_LEN);
			strcpy(szFilePath, sDeviceInfo.m_szMountPoint);
			if(szFilePath[nLen -1] != '/')
			{
				szFilePath[nLen] = '/';
				szFilePath[nLen + 1] = '\0';
			}
			strcat(szFilePath, szTest);
			CSTK_ASSERT_TRUE_FATAL(nLen < CSUDI_MAX_FILE_NAME_LEN, "·������������");
			hFile = CSUDIFSOpen(szFilePath, "w+");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == hFile, "��ֻ���־��ϴ��������ļ���Ӧ�óɹ�");
			nRetMkdir = CSUDIFSMkdir(szFilePath, 0);
			CSTK_ASSERT_TRUE_FATAL(-1 == nRetMkdir, "��ֻ���־��ϴ����ļ��в�Ӧ�óɹ�");
		}
		if ((sFsStat.m_eFsType > EM_UDIFS_PARTITION_RW_START) && (sFsStat.m_eFsType < EM_UDIFS_PARTITION_RW_END))
		{
			CSTK_ASSERT_TRUE_FATAL(!strcmp(sDeviceInfo.m_szMountPoint, g_aucLocalFsReadWritePath), "���ؿ�д�־���ص��������ļ���ͬ");
			CSTK_ASSERT_TRUE_FATAL((sFsStat.m_lBsize/1024*sFsStat.m_lBlocks) == g_nLocalFsReadWriteSizeKByte, "���ؿ�д�־��С�������ļ���ͬ");
			bHaveReadWriteFs = CSUDI_TRUE;
		}
	}

	CSTK_ASSERT_TRUE_FATAL((bHaveReadOnlyFs && bHaveReadWriteFs) == CSUDI_TRUE, "���ٴ���һ��ֻ����һ����д���ƶ��־��豸");

 	CSTK_FATAL_POINT
	if (hFile != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRemove(szFilePath), "�Ƴ��ļ�ʧ��");
	}
	if (nRetMkdir != -1)
	{
		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szFilePath), "�Ƴ��ļ���ʧ��");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP: �����ļ�ϵͳ
//@DESCRIPTION: ���Ա����ļ�ϵͳ�ļ��в���
//@PRECONDITION:FS��FLASHģ��֮ǰ�ѳɹ���ʼ��
//@PRECONDITION: ϵͳ�д��ڷ־��豸
//@INPUT: �����ļ�ϵͳ��д����·��
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@EXECUTIONFLOW:1������CSUDIFSGetAllDeviceId()�õ������ѹ��صķ־��豸id	
//@EXECUTIONFLOW:2�����ú��ж��豸�Ƿ�Ϊ���ƶ��豸
//@EXECUTIONFLOW:3��������ǿ��ƶ��豸����CSUDIFSGetDeviceInfo��÷־�Ĺ���·��
//@EXECUTIONFLOW:4������CSUDIFSStatfs��ȡ�־���Ϣ���ж��ļ�ϵͳ����
//@EXECUTIONFLOW:5������ļ�ϵͳ�ǿ�д�������������ļ��ȽϹ��ص��Ƿ���ͬ
//@EXECUTIONFLOW:6������CSUDIFSMkdir�����ļ��У�Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:7������CSUDIFSMkdir�������ļ��У�Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:8������CSUDIFSOpenDir���ļ��У�Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:9������CSUDIFSReadDir���ļ��У���������Ϊ3
//@EXECUTIONFLOW:10������CSUDIFSCloseDir�ر��ļ��У�Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:11������CSUDIFSRmdir�Ƴ����ļ��У�Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:12������CSUDIFSRmdir�Ƴ��ļ��У�Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_FS_IT_LocalFsWriteRead_0001(void)
{
	char szTest[] = "LocalFsWriteRead_0001";   
	char szSubTest[] = "LocalFsWriteRead_0001_Sub";
	int nDirRet = -1;
	int nSubDirRet = -1;
	int nNodeCount = 0;
	char szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	char szSubFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE hDir = CSUDI_NULL;
	CSUDIFSDirent_S *psDirent = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(FS_iLocalFsGetRWVolumeMountPoint(), "��ȡ���ؿ�д�־���ص�ʧ��");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	memset(szSubFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szSubFilePath, szFilePath);
	strcat(szSubFilePath, "/");
	strcat(szSubFilePath, szSubTest);

	CSTK_ASSERT_TRUE_FATAL(0 == (nDirRet = CSUDIFSMkdir(szFilePath, 0)), "�����ļ���ʧ��");
	CSTK_ASSERT_TRUE_FATAL(0 == (nSubDirRet = CSUDIFSMkdir(szSubFilePath, 0)), "�������ļ���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hDir = CSUDIFSOpenDir(szFilePath)), "���ļ���ʧ��");
	while (1)
	{
		psDirent = CSUDIFSReadDir(hDir);
		if(psDirent == CSUDI_NULL )
		{
			break;
		}
		nNodeCount++;
	}

	CSTK_ASSERT_TRUE_FATAL(3 == nNodeCount, "������������");

	CSTK_FATAL_POINT
	if(hDir != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSCloseDir(hDir), "�ر��ļ���ʧ��");
	}
	if(nSubDirRet == 0)
	{
		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szSubFilePath), "�Ƴ����ļ���ʧ��");
	}	
	if(nDirRet == 0)
	{
		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szFilePath), "�Ƴ��ļ���ʧ��");
	}	

	return CSUDI_TRUE;
}

//@CASEGROUP: �����ļ�ϵͳ
//@DESCRIPTION: ���Ա����ļ�ϵͳ�ļ��в���
//@PRECONDITION:FS��FLASHģ��֮ǰ�ѳɹ���ʼ��
//@PRECONDITION: ϵͳ�д��ڷ־��豸
//@INPUT: �����ļ�ϵͳ��д����·��
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@EXECUTIONFLOW:1������CSUDIFSGetAllDeviceId()�õ������ѹ��صķ־��豸id	
//@EXECUTIONFLOW:2�����ú��ж��豸�Ƿ�Ϊ���ƶ��豸
//@EXECUTIONFLOW:3��������ǿ��ƶ��豸����CSUDIFSGetDeviceInfo��÷־�Ĺ���·��
//@EXECUTIONFLOW:4������CSUDIFSStatfs��ȡ�־���Ϣ���ж��ļ�ϵͳ����
//@EXECUTIONFLOW:5������ļ�ϵͳ�ǿ�д�������������ļ��ȽϹ��ص��Ƿ���ͬ
//@EXECUTIONFLOW:6������CSUDIFSMkdir�����ļ��У�Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:7��ѭ��32�ε���CSUDIFSMkdir�������ļ��У�Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:8��ѭ��256�ε���CSUDIFSOpen���������ļ���Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:9������CSUDIFSOpenDir���ļ��У�Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:10������CSUDIFSReadDir���ļ��У���������Ϊ130
//@EXECUTIONFLOW:11������CSUDIFSCloseDir�ر��ļ��У�Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:12��ѭ��32�ε���CSUDIFSRmdir�Ƴ����ļ��У�Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:13��ѭ��256�ε���CSUDIFSClose�ر��ļ���Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:14��ѭ��256�ε���CSUDIFSRemove�Ƴ��ļ���Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:15������CSUDIFSRmdir�Ƴ��ļ��У�Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_FS_IT_LocalFsWriteRead_0002(void)
{
	char szTest[] = "LocalFsWriteRead_0002";
	char szSubTest[] = "LocalFsWriteRead_0002_Sub";
	int nDirRet = -1;
	int anSubDirRet[FS_TEST_MAX_DIR_CNT];
	CSUDI_HANDLE ahFile[FS_TEST_MAX_FILE_CNT];
	int nNodeCount = 0;
	int i = 0;
	int nLen = 0;
	char szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	char szSubFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE hDir = CSUDI_NULL;
	CSUDIFSDirent_S *psDirent = CSUDI_NULL;

	for (i=0; i<FS_TEST_MAX_DIR_CNT; i++)
	{
		anSubDirRet[i] = -1;
	}
	for (i=0; i<FS_TEST_MAX_FILE_CNT; i++)
	{
		ahFile[i] = CSUDI_NULL;
	}

	CSTK_ASSERT_TRUE_FATAL(FS_iLocalFsGetRWVolumeMountPoint(), "��ȡ���ؿ�д�־���ص�ʧ��");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	memset(szSubFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szSubFilePath, szFilePath);
	strcat(szSubFilePath, "/");
	strcat(szSubFilePath, szSubTest);
	nLen = strlen(szSubFilePath);

	CSTK_ASSERT_TRUE_FATAL(0 == (nDirRet = CSUDIFSMkdir(szFilePath, 0)), "�����ļ���ʧ��");

	for (i=0; i<FS_TEST_MAX_DIR_CNT; i++)
	{
		sprintf(szSubFilePath+nLen, "_Dir_%03d", i+1);
		CSTK_ASSERT_TRUE_FATAL(0 == (anSubDirRet[i] = CSUDIFSMkdir(szSubFilePath, 0)), "�������ļ���ʧ��");
	}
	for (i=0; i<FS_TEST_MAX_FILE_CNT; i++)
	{
		sprintf(szSubFilePath+nLen, "_File_%03d", i+1);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (ahFile[i] = CSUDIFSOpen(szSubFilePath, "w+")), "���������ļ�ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hDir = CSUDIFSOpenDir(szFilePath)), "���ļ���ʧ��");
	while (1)
	{
		psDirent = CSUDIFSReadDir(hDir);
		if(psDirent == CSUDI_NULL )
		{
			break;
		}
		nNodeCount++;
	}

	CSTK_ASSERT_TRUE_FATAL((2+FS_TEST_MAX_DIR_CNT+FS_TEST_MAX_FILE_CNT) == nNodeCount, "������������");

	CSTK_FATAL_POINT
	if (hDir != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSCloseDir(hDir), "�ر��ļ���ʧ��");
	}
	for (i=0; i<FS_TEST_MAX_DIR_CNT; i++)
	{
		if (anSubDirRet[i] == 0)
		{
			sprintf(szSubFilePath+nLen, "_Dir_%03d", i+1);
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szSubFilePath), "�Ƴ����ļ���ʧ��");
		}
	}
	for (i=0; i<FS_TEST_MAX_FILE_CNT; i++)
	{
		if (ahFile[i] != CSUDI_NULL)
		{
			sprintf(szSubFilePath+nLen, "_File_%03d", i+1);
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(ahFile[i]), "�ر��ļ�ʧ��");
			CSTK_ASSERT_TRUE(0 == CSUDIFSRemove(szSubFilePath), "�Ƴ��ļ�ʧ��");
		}
	}
	if (nDirRet == 0)
	{
		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szFilePath), "�Ƴ��ļ���ʧ��");
	}	

	return CSUDI_TRUE;
}

//@CASEGROUP: �����ļ�ϵͳ
//@DESCRIPTION: ���ԶԱ����ļ�ϵͳ�ļ�����
//@PRECONDITION:FS��FLASHģ��֮ǰ�ѳɹ���ʼ��
//@PRECONDITION: ϵͳ�д��ڷ־��豸
//@INPUT: �����ļ�ϵͳ��д����·��
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@EXECUTIONFLOW:1������CSUDIFSGetAllDeviceId()�õ������ѹ��صķ־��豸id	
//@EXECUTIONFLOW:2�����ú��ж��豸�Ƿ�Ϊ���ƶ��豸
//@EXECUTIONFLOW:3��������ǿ��ƶ��豸����CSUDIFSGetDeviceInfo��÷־�Ĺ���·��
//@EXECUTIONFLOW:4������CSUDIFSStatfs��ȡ�־���Ϣ���ж��ļ�ϵͳ����
//@EXECUTIONFLOW:5������ļ�ϵͳ�ǿ�д�������������ļ��ȽϹ��ص��Ƿ���ͬ
//@EXECUTIONFLOW:6������CSUDIFSOpen���������ļ���Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:7������CSUDIFSWriteд�����ݣ�Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:8������CSUDIFSFlushͬ�������̣�Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:9������CSUDIFSTell��ȡ�ļ���ǰλ�ã�Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:10������CSUDIFSSeek�����ļ���Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:11������CSUDIFSTell��ȡ�ļ���ǰλ�ã�Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:12������CSUDIFSFstat��ȡ�ļ���Ϣ��Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:13������CSUDIFS_ISREG�ж��Ƿ�Ϊ�ļ�
//@EXECUTIONFLOW:14������CSUDIFSRead��ȡ���ݣ�Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:15������CSUDIFSSeek�ж϶���������д��ʱ�Ƿ�һ�£�Ҫ��һ��
//@EXECUTIONFLOW:16������CSUDIFSStat��ȡ�ļ���Ϣ��Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:17������CSUDIFS_ISREG�ж��Ƿ�Ϊ�ļ�
//@EXECUTIONFLOW:18������CSUDIFSClose�ر��ļ���Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:19������CSUDIFSRemove�Ƴ����ļ���Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_FS_IT_LocalFsWriteRead_0003(void)
{
	CSUDI_HANDLE hFile = CSUDI_NULL;
	char szTest[] = "LocalFsWriteRead_0003";
	char szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	int nLen = strlen(szTest);
	char szBuf[100];
	CSUDIFSFileStat_S sFileStat;
	CSUDI_BOOL bAlreadyClose = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(FS_iLocalFsGetRWVolumeMountPoint(), "��ȡ���ؿ�д�־���ص�ʧ��");
	memset(szFilePath, 0x00, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "���������ļ�ʧ��");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "д������ʧ��");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSFlush(hFile), "ͬ��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSTell(hFile), "��ȡ�ļ���ǰλ��ʧ��");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile, 0, CSUDIFS_SEEK_SET), "�����ļ�ʧ��");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSTell(hFile), "��ȡ�ļ���ǰλ��ʧ��");

	memset(&sFileStat, 0x00, sizeof(CSUDIFSFileStat_S));
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSFstat(hFile, &sFileStat), "��ȡ�ļ���Ϣʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDIFS_ISREG(sFileStat.m_lMode), "��ȡ����Ӧ�����ļ�");
	CSTK_ASSERT_TRUE_FATAL((int)sFileStat.m_lSize == nLen, "��ȡ�ļ���Ϣ�д�С����");

	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSRead(hFile, szBuf, sizeof(szBuf) -1), "��ȡ����ʧ��");
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(szTest, szBuf), "����������д��ʱ��һ��");

	CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��");
	hFile = CSUDI_NULL;
	bAlreadyClose = CSUDI_TRUE;

	memset(&sFileStat, 0x00, sizeof(CSUDIFSFileStat_S));
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStat(szFilePath, &sFileStat), "��ȡ�ļ���Ϣʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDIFS_ISREG(sFileStat.m_lMode), "��ȡ����Ӧ�����ļ�");
	CSTK_ASSERT_TRUE_FATAL((int)sFileStat.m_lSize == nLen, "��ȡ�ļ���Ϣ�д�С����");

	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile), "�ر��ļ�ʧ��");
		}

		if(bAlreadyClose || (hFile != CSUDI_NULL))
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSRemove(szFilePath), "�Ƴ��ļ�ʧ��");
		}
	}

	return CSUDI_TRUE;			
}

//@CASEGROUP: �����ļ�ϵͳ
//@DESCRIPTION: ���ԶԱ����ļ�ϵͳ�ļ�����
//@PRECONDITION:FS��FLASHģ��֮ǰ�ѳɹ���ʼ��
//@PRECONDITION: ϵͳ�д��ڷ־��豸
//@INPUT: �����ļ�ϵͳ��д����·��
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@EXECUTIONFLOW:1������CSUDIFSGetAllDeviceId()�õ������ѹ��صķ־��豸id	
//@EXECUTIONFLOW:2�����ú��ж��豸�Ƿ�Ϊ���ƶ��豸
//@EXECUTIONFLOW:3��������ǿ��ƶ��豸����CSUDIFSGetDeviceInfo��÷־�Ĺ���·��
//@EXECUTIONFLOW:4������CSUDIFSStatfs��ȡ�־���Ϣ���ж��ļ�ϵͳ����
//@EXECUTIONFLOW:5������ļ�ϵͳ�ǿ�д�������������ļ��ȽϹ��ص��Ƿ���ͬ
//@EXECUTIONFLOW:6������CSUDIFSOpen���������ļ�A��Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:7������CSUDIFSWriteд�����ݣ�Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:8������CSUDIFSClose�ر��ļ�A��Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:9������CSUDIFSTruncate�ض��ļ���Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:10������CSUDIFSStat��ȡ�ļ���Ϣ��Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:11������strcmp�жϽضϺ��ļ���С�Ƿ���ȷ
//@EXECUTIONFLOW:12������CSUDIFSRename�������ļ�AΪ�ļ�B��Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:13������CSUDIFSOpen���ļ�A��Ҫ�󷵻�ʧ��
//@EXECUTIONFLOW:14������CSUDIFSOpen���ļ�B��Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:15������CSUDIFSRead��ȡ���ݣ�Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:16������CSUDIFSStat��ȡ�ļ���Ϣ��Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:17������strcmp�ж����������ļ���С�Ƿ���ȷ
//@EXECUTIONFLOW:18������CSUDIFSClose�ر��ļ���Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:19������CSUDIFSRemove�Ƴ����ļ���Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_FS_IT_LocalFsWriteRead_0004(void)
{
	CSUDI_HANDLE hFileA = CSUDI_NULL;
	CSUDI_HANDLE hFileB = CSUDI_NULL;
	char szTestA[] = "LocalFsWriteRead_0004a";  
	char szTestB[] = "LocalFsWriteRead_0004b";  
	char szFilePathA[CSUDI_MAX_FILE_NAME_LEN];
	char szFilePathB[CSUDI_MAX_FILE_NAME_LEN];
	int nLen = strlen(szTestA);
	int  ulLength1 = nLen/2;
	CSUDIFSFileStat_S sFileStat;

	CSTK_ASSERT_TRUE_FATAL(FS_iLocalFsGetRWVolumeMountPoint(), "��ȡ���ؿ�д�־���ص�ʧ��");
	memset(szFilePathA, 0, CSUDI_MAX_FILE_NAME_LEN);
	memset(szFilePathB, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePathA, g_szVolumeMountPoint);
	strcpy(szFilePathB, g_szVolumeMountPoint);
	strcat(szFilePathA, szTestA);
	strcat(szFilePathB, szTestB);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFileA = CSUDIFSOpen(szFilePathA, "w")), "���������ļ�ʧ��");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFileA, szTestA, nLen), "д���ļ�ʧ��");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFileA), "�ر��ļ�ʧ��\n");
	hFileA = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSTruncate(szFilePathA, ulLength1), "�ض��ļ�ʧ��");	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStat(szFilePathA, &sFileStat), "��ȡ�ļ���Ϣʧ��");
	CSTK_ASSERT_TRUE_FATAL((int)sFileStat.m_lSize == ulLength1, "�ضϺ��ļ���С����");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRename(szFilePathA, szFilePathB), "�������ļ�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == CSUDIFSOpen(szFilePathA, "r"), "���ļ���Ӧ�óɹ�");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFileB = CSUDIFSOpen(szFilePathB, "r")) , "���ļ�ʧ��");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStat(szFilePathB, &sFileStat), "��ȡ�ļ���Ϣʧ��");
	CSTK_ASSERT_TRUE_FATAL((int)sFileStat.m_lSize == ulLength1, "���������ļ���С����");

	CSTK_FATAL_POINT
	{
		if(hFileA != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFileA), "�ر��ļ�ʧ��");
			CSTK_ASSERT_TRUE(0 == CSUDIFSRemove(szFilePathA), "�Ƴ��ļ�ʧ��");
		}
		if(hFileB != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFileB), "�ر��ļ�ʧ��");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePathB), "�Ƴ��ļ�ʧ��");
		}
	}

	return CSUDI_TRUE;			
}

static int g_nFSRDIAddCallback_007 = 0;

typedef struct
{
	int m_nMajorFound;    //�洢�豸�յ���Found��Ϣ����
	int m_nVolumeFound;   //�־��豸�յ���Found��Ϣ����
	int m_nMajorReady;    //�洢�豸�յ���Ready��Ϣ����
	int m_nVolumeReady;   //�־��豸�յ���Ready��Ϣ����
	int m_nMajorPlugout;  //�洢�豸�յ���Plugout��Ϣ����
	int m_nVolumePlugout; //�־��豸�յ���Plugout��Ϣ����
}FSCallbackCntInfo_S;

static FSCallbackCntInfo_S s_sCallbackCnt;

static void iFSRDIAddCallback_0007_Callback(CSUDIRDIEvent_E eType, CSUDI_UINT32 dwDeviceId,const void * pvEventData, void * pvUserData)
{
	if(*(int*)pvUserData != g_nFSRDIAddCallback_007)
	{
		return ;
	}

	if(!CSUDI_ISREMOVEABLE(dwDeviceId))
	{
		return ;
	}

	switch(eType)
	{
	case EM_UDIRDI_EVENT_FOUND:
		{
			if ((dwDeviceId & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_STORAGE_MAJOR_DEVICEID)
			{
				s_sCallbackCnt.m_nMajorFound++;
			}
			else if((dwDeviceId & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
			{
				s_sCallbackCnt.m_nVolumeFound++;
			}
			else
			{	
				CSFSPrint("δ֪����");
				return;
			}
			break;
		}
	case EM_UDIRDI_EVENT_READY:
		{
			if ((dwDeviceId & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_STORAGE_MAJOR_DEVICEID)
			{
				s_sCallbackCnt.m_nMajorReady++;
			}
			else if((dwDeviceId & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
			{
				s_sCallbackCnt.m_nVolumeReady++;
			}
			else
			{
				CSFSPrint("δ֪����");
				return;
			}
			break;
		}
	case EM_UDIRDI_EVENT_PLUGOUT:
		{
			if ((dwDeviceId & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_STORAGE_MAJOR_DEVICEID)
			{
				s_sCallbackCnt.m_nMajorPlugout++;
			}
			else if((dwDeviceId & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
			{
				s_sCallbackCnt.m_nVolumePlugout++;
			}
			else
			{
				CSFSPrint("δ֪����");
				return;
			}
			break;
		}
	default:
		{
			CSFSPrint("�Ƿ���Ϣ�����ϸ��ղ������̲���");
			break;
		}
	}

	return;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSRDIAddCallback(CSUDIRDICallback_F fnCallback,const void * pvUserData)
//@DESCRIPTION: ֧�ֶ�USB�豸������ע��ص�����֤�յ��Ĵ洢���־��豸��Found��Ready��Plugout��Ϣ
//@PRECONDITION: ��
//@INPUT: fnCallback = iFSRDIAddCallback_0007_Callback
//@INPUT: pvUserData =  &g_nFSRDIAddCallback_007, ����int g_nFSRDIAddCallback_007 = 1;
//@EXPECTATION: ע��ɹ�����������U��ʱ�ڻص��������յ����EM_UDIRDI_EVENT_READY��Ϣ���γ�U��ʱ�յ����EM_UDIRDI_EVENT_PLUGOUT��Ϣ
//@EXECUTIONFLOW: 1������CSUDIFSRDIAddCallback(iFSRDIAddCallback_0007_Callback, &g_nFSRDIAddCallback_007)ע��ص�������ֵӦ�õ���CSUDI_SUCCESS
//@EXECUTIONFLOW: 2��������ʾ����U�̣���5���'Y'��ȷ�ϣ����ص����������洢�豸�յ�����Ϣ����
//@EXECUTIONFLOW: 3�����ص��������յ��洢�豸��Plugout��Ϣ����
//@EXECUTIONFLOW: 4�����ص��������յ��洢�豸��Found��Ϣ����
//@EXECUTIONFLOW: 5�����ص��������յ��洢�豸��Ready��Ϣ����
//@EXECUTIONFLOW: 6�����ص��������յ��־��豸��Found��Ϣ����
//@EXECUTIONFLOW: 7�����ص��������յ��־��豸��Ready��Ϣ����
//@EXECUTIONFLOW: 8��������ʾ�Ƴ�U�̣���'Y'��ȷ��
//@EXECUTIONFLOW: 9�����ص��������յ��洢�豸��Plugout��Ϣ����
//@EXECUTIONFLOW: 10�����ص��������յ��־��豸��Plugout��Ϣ����
//@EXECUTIONFLOW: 11����ʾ���U�̣�����CSUDIFSRDIRemoveCallback(CSUDIFSRDIAddCallback_0001_Callback, &nUserData)�Ƴ��ص����ָ�����
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIAddCallback_0007(void)
{
	//�ֶ�����
	CSUDI_BOOL bAddCallback = CSUDI_FALSE;
	CSUDI_BOOL bRemoved = CSUDI_FALSE;

	memset(&s_sCallbackCnt, 0x00, sizeof(FSCallbackCntInfo_S));
	
	CSTK_ASSERT_TRUE_FATAL(g_nUSBCount > 1, "��������Ҫ����ƽ̨����֧������USB�豸");
	CSTCPrint("��γ�����U��, ��Y��ȷ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N������ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIAddCallback(iFSRDIAddCallback_0007_Callback,
		&g_nFSRDIAddCallback_007),"��ӻص�ʧ��");
	bAddCallback = CSUDI_TRUE;
	
	CSTCPrint("�����%d��U��, ÿ��U��������һ������, ������ɺ���ȴ�5��, �ٰ�Y��ȷ��\n", g_nUSBCount);
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(s_sCallbackCnt.m_nMajorPlugout == 0, "���ϸ��ղ��Բ���������");
	CSTK_ASSERT_TRUE_FATAL(s_sCallbackCnt.m_nMajorFound == g_nUSBCount, "�յ��Ĵ洢�豸Found��Ϣ��������ȷ");
	CSTK_ASSERT_TRUE_FATAL(s_sCallbackCnt.m_nMajorReady == g_nUSBCount , "�յ��Ĵ洢�豸Ready��Ϣ��������ȷ");
	CSTK_ASSERT_TRUE_FATAL(s_sCallbackCnt.m_nVolumeFound >= g_nUSBCount, "�յ��ķ־���Ϣ��������ȷ");
	CSTK_ASSERT_TRUE_FATAL(s_sCallbackCnt.m_nVolumeFound == s_sCallbackCnt.m_nVolumeReady, "�յ��ķ־���Ϣ��������ȷ");
	
	CSTCPrint("���Ƴ�ȫ��U��, ��Y��ȷ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N������ʧ��");
	bRemoved = CSUDI_TRUE;
		
	CSTK_ASSERT_TRUE_FATAL(s_sCallbackCnt.m_nMajorPlugout == g_nUSBCount, "�յ��Ĵ洢�豸Plugout��Ϣ��������ȷ");
	CSTK_ASSERT_TRUE_FATAL(s_sCallbackCnt.m_nVolumePlugout >= g_nUSBCount, "�յ��ķ־�Plugout��Ϣ��������ȷ");
	CSTK_ASSERT_TRUE_FATAL(s_sCallbackCnt.m_nVolumePlugout == s_sCallbackCnt.m_nVolumeReady, "�յ��ķ־��豸Plugout��Ϣ��������ȷ");

	CSTK_FATAL_POINT
	{
		if (bAddCallback == CSUDI_TRUE)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(iFSRDIAddCallback_0007_Callback, 
				&g_nFSRDIAddCallback_007), "�Ƴ��ص�ʧ��\n");
		}
		if (bRemoved == CSUDI_TRUE)
		{
			CSFSPrint("U���Ѿ��Ƴ�����ε������²��룬�����سɹ��󣬰�Yȷ��!\n");
			CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N���ָ�����ʧ�ܣ���Ӱ�������������ִ��");  
		}
	}
	
	return CSUDI_TRUE;
}



//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetDeviceInfo(CSUDI_UINT32 dwDeviceId,CSUDIFSDeviceInfo_S * psDeviceInfo)
//@DESCRIPTION: ������ö��USB�豸����Ϣ
//@PRECONDITION: ϵͳ�����ٴ���������ʶ���USB�豸
//@INPUT: dwDeviceId = adwIds[n]��adwIds����CSUDIFSGetAllDeviceId()��õ��豸������
//@INPUT: psDeviceInfo = &sDeviceInfo, ����CSUDIFSDeviceInfo_S  sDeviceInfo
//@EXPECTATION: ����ֵ���� CSUDI_SUCCESS���Ҷ�USB�豸�Ĺ��ص㲻ͬ
//@EXECUTIONFLOW: 1������CSUDIFSGetAllDeviceId()�õ�ȫ���豸��id����adwIds[]
//@EXECUTIONFLOW: 2�����������豸������CSUDIFSGetDeviceInfo(adwIds[n], &sDeviceInfo)���жϷ���ֵ�������豸�Լ��־��豸���ֳ���,�����־��豸�Ĺ��ص㱣�浽���鵱��
//@EXECUTIONFLOW: 3������ȡ�������־��豸�����Ƿ����Ԥ��
//@EXECUTIONFLOW: 4�����־��豸�Ĺ���·���Ƿ�����ͬ��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetDeviceInfo_0012(void)
{	
	//�ֶ�����
	CSUDI_UINT32   adwIds[32] = {0};
	int nActIdCnt = 0;
	CSUDIFSDeviceInfo_S  sDeviceInfo;
	int i = 0;
	int k = 0;
	int nVolumeDev = 0;//�־�����
	int nMajorDev = 0;//�洢�豸����
	CSUDI_BOOL bResult = CSUDI_FALSE;	
	char aszVolumeDev[FS_TEST_MAX_USBDEVICE_CNT][256] = {"\0"};//�־�Ĺ��ص�
	
	memset(aszVolumeDev, 0x00, sizeof(aszVolumeDev));
	memset(&sDeviceInfo, 0x00, sizeof(CSUDIFSDeviceInfo_S));
	
	CSTK_ASSERT_TRUE_FATAL(g_nUSBCount > 1, "��������Ҫ����ƽ̨����֧������USB�豸");
	
	CSTCPrint("��ȷ���Ѿ�������%d��U��, ÿ��U��������һ������,������ɺ���ȴ�5�룬�밴Y��ȷ��\n", g_nUSBCount);
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N������ʧ��"); 
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(adwIds, 32, &nActIdCnt), "��ȡ�豸idʧ��");
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "δ��ȡ���κ��豸");
	
	for (i = 0; i < nActIdCnt; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(adwIds[i], &sDeviceInfo), "��ȡ�豸��Ϣʧ��");		
		if ((adwIds[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_STORAGE_MAJOR_DEVICEID)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == strlen(sDeviceInfo.m_szMountPoint), "�洢�豸��Ӧ���й��ص�" );
			nMajorDev++;
			
		}
		else if ((adwIds[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "�־��豸Ӧ���й��ص�");
			strncpy(aszVolumeDev[nVolumeDev], sDeviceInfo.m_szMountPoint, sizeof(sDeviceInfo.m_szMountPoint));
			nVolumeDev++;
		}
		else
		{
			if (CSUDI_ISREMOVEABLE(adwIds[i]))
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "��õ��豸�Ų�����֪���豸����" );
			}
			else
			{
			    CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "���ط־��豸Ӧ���й��ص�");
			}
		}
	}
	
	CSTK_ASSERT_TRUE_FATAL(nMajorDev == g_nUSBCount, "��ȡ�洢�豸��������ȷ" );
	CSTK_ASSERT_TRUE_FATAL(nVolumeDev >= g_nUSBCount, "��ȡ�־���������ȷ" );

	for (i = 0; i < nVolumeDev; i++)
	{
		for (k = i + 1 ; k < nVolumeDev; k++)
		{
			bResult = (strcmp(aszVolumeDev[i], aszVolumeDev[k]) != 0 );
			CSTK_ASSERT_TRUE_FATAL(bResult , "�־����·��������ͬ" );
		}
	}
	 
	CSTK_FATAL_POINT
	{
		;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetDeviceInfo(CSUDI_UINT32 dwDeviceId,CSUDIFSDeviceInfo_S * psDeviceInfo)
//@DESCRIPTION: ���������USB�豸����Ϣ
//@PRECONDITION: ׼�����ֲ�ͬƷ�Ƶ�U��
//@INPUT: dwDeviceId = adwIds[n]��adwIds����CSUDIFSGetAllDeviceId()��õ��豸������
//@INPUT: psDeviceInfo = &sDeviceInfo, ����CSUDIFSDeviceInfo_S  sDeviceInfo
//@EXPECTATION: ����ֵ���� CSUDI_SUCCESS�������豸�޹��ص㣬�־��豸�й��ص�
//@EXECUTIONFLOW: 1����ʾ�û�����ָ��Ʒ�Ƶ�U��
//@EXECUTIONFLOW: 2������CSUDIFSGetAllDeviceId()�õ�ȫ���豸��id����adwIds[]
//@EXECUTIONFLOW: 3�����������豸������CSUDIFSGetDeviceInfo(adwIds[n], &sDeviceInfo)���жϷ���ֵ������֤���豸���־��豸�Ƿ���ڹ��ص�
//@EXECUTIONFLOW: 4������ȡ�������־��豸�����Ƿ����Ԥ��
//@EXECUTIONFLOW: 5��ѭ����֤��ͬƷ�Ƶ�U��
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetDeviceInfo_0013(void)
{	
	//�ֶ�����
	CSUDI_UINT32   adwIds[32] = {0};
	int nActIdCnt = 0;
	CSUDIFSDeviceInfo_S  sDeviceInfo;
	int i = 0;
	int k = 0;	
	int bJudgeMajor = CSUDI_FALSE;
	int bJudgeVolume = CSUDI_FALSE;
	//u��Ʒ�����飬�����Ԫ�ʱظ�����Ҫ��FS_TEST_USB_BRAND_CNT����ƥ��
	const char awsCSUDIFSUsbBrand[FS_TEST_USB_BRAND_CNT][256] = {"KINGSTON��ʿ��",  "SSK���", "SANDISK����", "NETAC�ʿ�", "ADATA����",
		                                                     "PNY�ض���", "TOSHIBA��֥", "VERBATIM����", "MAXELL�������", "SONY����", 
		                                                     "AIGO������", "APACER��հ"};

	memset(&sDeviceInfo, 0x00, sizeof(CSUDIFSDeviceInfo_S));
	
	for (k = 0; k < FS_TEST_USB_BRAND_CNT; k++)
	{		
		CSTCPrint("�����²���[%s]��u��,��U������Ҫ��һ������, �밴Y��ȷ��\n", awsCSUDIFSUsbBrand[k]);
		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N������ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(adwIds, 32, &nActIdCnt), "��ȡ�豸idʧ��");
		CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "δ��ȡ���κ��豸");

		for (i = 0; i < nActIdCnt; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(adwIds[i], &sDeviceInfo), "��ȡ�豸��Ϣʧ��");
			   
			
			if ((adwIds[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_STORAGE_MAJOR_DEVICEID)
			{
				CSTK_ASSERT_TRUE_FATAL(0 == strlen(sDeviceInfo.m_szMountPoint), "���豸��Ӧ���й��ص�" );
				bJudgeMajor = CSUDI_TRUE;
			}
			else if ((adwIds[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
			{
				CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "�־��豸Ӧ���й��ص�");
				bJudgeVolume = CSUDI_TRUE;
			}
			else
			{
				if (CSUDI_ISREMOVEABLE(adwIds[i]))
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "��õ��豸�Ų�����֪���豸����" );
				}
				else
				{
				    CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "���ط־��豸Ӧ���й��ص�");
				}
			}
		}

		CSTK_ASSERT_TRUE_FATAL((bJudgeMajor && bJudgeVolume) == CSUDI_TRUE, "δ��ȷ��ȡ���ƶ��豸");
	
	}

	
	CSTK_FATAL_POINT
	{
		;
	}
	
	return CSUDI_TRUE;
}

static CSUDI_BOOL FSBrowseDir(const char* pcDirName, int* pnDirCount, int* pnFileCount)
{
	CSUDI_BOOL bRet = CSUDI_TRUE;
	int nRet = -1;
	CSUDI_HANDLE hDir = CSUDI_NULL;
	int i = 0;
	CSUDIFSDirent_S * psDirent = CSUDI_NULL;
	char acSubDirName[CSUDI_MAX_FILE_NAME_LEN] = {0};
	char acFileName[CSUDI_MAX_FILE_NAME_LEN] = {0};
	CSUDIFSFileStat_S sFileStat;

	if (pcDirName == CSUDI_NULL || pnDirCount == CSUDI_NULL || pnFileCount == CSUDI_NULL)
	{
		CSTCPrint("[%s, %d] Parameters error!\n", __FUNCTION__, __LINE__);
		return CSUDI_FALSE;
	}
	
	CSTCPrint("%s enter, pcDirName=%s\n", __FUNCTION__, pcDirName);
	
	hDir = CSUDIFSOpenDir(pcDirName);
	if (hDir == CSUDI_NULL)
	{
		CSTCPrint("OpenDir failed, DirName=%s\n", pcDirName);
		return CSUDI_FALSE;
	}

	for (i=0; i<1000; i++)
	{
		psDirent = CSUDIFSReadDir(hDir);
		if (psDirent == CSUDI_NULL)
		{
			break;
		}
		else
		{
			if (strcmp(psDirent->m_cDirName, ".") != 0 && strcmp(psDirent->m_cDirName, "..") != 0)
			{
				if (psDirent->m_ucDirType == 0)
				{
					memset(acFileName, 0, CSUDI_MAX_FILE_NAME_LEN);
					strcpy(acFileName, pcDirName);
					strcat(acFileName, "/");
					strcat(acFileName, psDirent->m_cDirName);
					
					nRet = CSUDIFSStat(acFileName, &sFileStat);
					if (nRet != 0)
					{
						CSTCPrint("File error, FileName=%s\n", acFileName);
						bRet = CSUDI_FALSE;
						break;
					}
					else
					{
						(*pnFileCount)++;
						CSTCPrint("File[%d]: %s\n", *pnFileCount, acFileName);
					}
				}
				else if (psDirent->m_ucDirType == 1)
				{
					memset(acSubDirName, 0, CSUDI_MAX_FILE_NAME_LEN);
					strcpy(acSubDirName, pcDirName);
					strcat(acSubDirName, "/");
					strcat(acSubDirName, psDirent->m_cDirName);

					bRet = FSBrowseDir(acSubDirName, pnDirCount, pnFileCount);
					if (!bRet)
					{
						CSTCPrint("BrowseDir Failed, DirName=%s\n", acSubDirName);
						break;
					}
					else
					{
						(*pnDirCount)++;
						CSTCPrint("Folder[%d]: %s\n", *pnDirCount, acSubDirName);
					}
				}
			}
		}
	}

	nRet = CSUDIFSCloseDir(hDir);
	if (nRet != 0)
	{
		CSTCPrint("CloseDir failed, DirName=%s\n", pcDirName);
		bRet = CSUDI_FALSE;
	}

	return bRet;
}

//@CASEGROUP: int CSUDIFSMkdir(const char * pcPathName, int nReserved)
//@DESCRIPTION:  ������������ɾ���ļ����ļ���
//@PRECONDITION: ϵͳ�д���һ���ѹ��صķ־��豸
//@INPUT: 
//@EXPECTATION: �������ܹ���������ɾ���ļ����ļ��гɹ�
//@EXECUTIONFLOW: 1����ȡ�������ص�·���������ɹ�
//@EXECUTIONFLOW: 2����������Ŀ¼CSUDIFSMkdir(���ص�·��+"CSTC_FS_IT_0001", 0)�������ɹ�
//@EXECUTIONFLOW: 3�������ļ���CSUDIFSMkdir(���ص�·��+"CSTC_FS_IT_0001"+"TestDir", 0)�������ɹ�
//@EXECUTIONFLOW: 4�������ļ�CSUDIFSOpen(���ص�·��+"CSTC_FS_IT_0001"+"TestFile")�������ɹ�
//@EXECUTIONFLOW: 5�������ļ���CSUDIFSMkdir(���ص�·��+"CSTC_FS_IT_0001"+"TestDir"+"SubTestDir", 0)�������ɹ�
//@EXECUTIONFLOW: 6�������ļ�CSUDIFSOpen(���ص�·��+"CSTC_FS_IT_0001"+"TestDir"+"SubTestFile")�������ɹ�
//@EXECUTIONFLOW: 7������CSUDIFSOpenDir, CSUDIFSReadDir, CSUDIFSCloseDir�Ƚӿ�ѭ���������ص�·��+"CSTC_FS_IT_0001"Ŀ¼�������ɹ�
//@EXECUTIONFLOW: 8���жϱ��������ļ������Ƿ�Ϊ2(TestFile��SubTestFile)
//@EXECUTIONFLOW: 9���жϱ��������ļ��������Ƿ�Ϊ2(TestDir��SubTestDir)
//@EXECUTIONFLOW: 10���ر��ļ�(���ص�·��+"CSTC_FS_IT_0001"+"TestFile")�������ɹ�
//@EXECUTIONFLOW: 11��ɾ���ļ�(���ص�·��+"CSTC_FS_IT_0001"+"TestFile")�������ɹ�
//@EXECUTIONFLOW: 12���ر��ļ�(���ص�·��+"CSTC_FS_IT_0001"+"SubTestFile")�������ɹ�
//@EXECUTIONFLOW: 13��ɾ���ļ�(���ص�·��+"CSTC_FS_IT_0001"+"SubTestFile")�������ɹ�
//@EXECUTIONFLOW: 14��ɾ���ļ���CSUDIFSRmdir(���ص�·��+"CSTC_FS_IT_0001"+"TestDir"+"SubTestDir")�������ɹ�
//@EXECUTIONFLOW: 15��ɾ���ļ���CSUDIFSRmdir(���ص�·��+"CSTC_FS_IT_0001"+"TestDir"+"TestDir")�������ɹ�
//@EXECUTIONFLOW: 16��ɾ���ļ���CSUDIFSRmdir(���ص�·��+"CSTC_FS_IT_0001"+"TestDir")�������ɹ�
CSUDI_BOOL CSTC_FS_IT_0001(void)
{
	char* pcTestDir = "TestDir";
	char* pcSubTestDir = "SubTestDir";
	char* pcTestFile = "TestFile";
	char* pcSubTestFile = "SubTestFile";
	char acDirPath[CSUDI_MAX_FILE_NAME_LEN] = {0};
	char acTestDirPath[CSUDI_MAX_FILE_NAME_LEN] = {0};
	char acSubTestDirPath[CSUDI_MAX_FILE_NAME_LEN] = {0};
	char acTestFilePath[CSUDI_MAX_FILE_NAME_LEN] = {0};
	char acSubTestFilePath[CSUDI_MAX_FILE_NAME_LEN] = {0};
	CSUDI_HANDLE hTestFile = CSUDI_NULL;
	CSUDI_HANDLE hSubTestFile = CSUDI_NULL;
	int nMkDir = -1;
	int nMkTestDir = -1;
	int nMkSubTestDir = -1;
	int nFileCount = 0;
	int nDirCount = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "��ȡ�������ص�ʧ��");

	sprintf(acDirPath, "%s/%s", g_szVolumeMountPoint, __FUNCTION__);
	nMkDir = CSUDIFSMkdir(acDirPath, 0);
	CSTK_ASSERT_TRUE_FATAL(nMkDir == 0, "����2��������Ŀ¼ʧ��");

	sprintf(acTestDirPath, "%s/%s", acDirPath, pcTestDir);
	nMkTestDir = CSUDIFSMkdir(acTestDirPath, 0);
	CSTK_ASSERT_TRUE_FATAL(nMkTestDir == 0, "����3�����ļ���ʧ��");

	sprintf(acTestFilePath, "%s/%s", acDirPath, pcTestFile);
	hTestFile = CSUDIFSOpen(acTestFilePath, "w");
	CSTK_ASSERT_TRUE_FATAL(hTestFile != CSUDI_NULL, "����4�����ļ�ʧ��");

	sprintf(acSubTestDirPath, "%s/%s", acTestDirPath, pcSubTestDir);
	nMkSubTestDir = CSUDIFSMkdir(acSubTestDirPath, 0);
	CSTK_ASSERT_TRUE_FATAL(nMkSubTestDir == 0, "����5�����ļ���ʧ��");

	sprintf(acSubTestFilePath, "%s/%s", acTestDirPath, pcSubTestFile);
	hSubTestFile = CSUDIFSOpen(acSubTestFilePath, "w");
	CSTK_ASSERT_TRUE_FATAL(hSubTestFile != CSUDI_NULL, "����6�����ļ�ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(FSBrowseDir(acDirPath, &nFileCount, &nDirCount), "�����������ص�Ŀ¼ʧ��");

	CSTCPrint("nFileCount = %d, nDirCount = %d\n", nFileCount, nDirCount);

	CSTK_ASSERT_TRUE_FATAL(nFileCount == 2, "������ȡ�����ļ���������ȷ");

	CSTK_ASSERT_TRUE_FATAL(nDirCount == 2, "������ȡ�����ļ�����������ȷ");
	
	CSTK_FATAL_POINT;

	if (hTestFile != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIFSClose(hTestFile) == 0, "����10�ر��ļ�ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDIFSRemove(acTestFilePath) == 0, "����11ɾ���ļ�ʧ��");
	}

	if (hSubTestFile != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIFSClose(hSubTestFile) == 0, "����12�ر��ļ�ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDIFSRemove(acSubTestFilePath) == 0, "����13ɾ���ļ�ʧ��");
	}

	if (nMkTestDir == 0)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIFSRmdir(acSubTestDirPath) == 0, "����14ɾ���ļ���ʧ��");
	}

	if (nMkSubTestDir == 0)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIFSRmdir(acTestDirPath) == 0, "����15ɾ���ļ���ʧ��");
	}

	if (nMkDir == 0)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIFSRmdir(acDirPath) == 0, "����16ɾ������Ŀ¼ʧ��");
	}
	
	return CSUDI_TRUE;
}

