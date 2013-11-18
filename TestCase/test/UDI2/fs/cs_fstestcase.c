/* --------------------------------------------------------------------
注意：
1.在需要与用户交互的测试用例中，可以：
	a. 使用CSTKWaitAnyKey等待用户输入任意按键
	b. 使用CSTKWaitYes等待用户输入YES
2.测试用例函数命名：测试用例ID，"测试用例ID"定义在测试用例文档中
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
#define FS_TEST_MAX_DIR_CNT       32  // 文件夹中可创建的文件夹个数
#define FS_TEST_MAX_FILE_CNT      256 // 文件夹中可创建的文件个数
#define FS_TEST_MAX_USBDEVICE_CNT   8 // 支持最多的USB设备及分卷个数
#define FS_TEST_USB_BRAND_CNT		   12 // 支持的USB品牌数量


// 是否有文件系统
CSUDI_BOOL g_bHaveFs = CSUDI_TRUE;
// 本地文件系统可写挂载点
char g_aucLocalFsReadWritePath[CSUDIFS_MAX_FILE_PATH] = {0};
// 本地文件系统只读挂载点
char g_aucLocalFsReadOnlyPath[CSUDIFS_MAX_FILE_PATH] = {0};
// 本地文件系统可写分卷大小
int g_nLocalFsReadWriteSizeKByte = 0;
// 本地文件系统只读分卷大小
int g_nLocalFsReadOnlySizeKByte = 0;
//该平台支持的USB数量
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
		CSTCPrint("请检查配置文件中是否已经包含[USB]段、CS_USBUDI2_USB_COUNT是否已正确配置\n");
	}
	
	CSTCPrint("本模块测试用例默认插入u盘，若未插入可能导致测试不通过\n");

	return bRet;
}

CSUDI_BOOL CSTC_FS_IT_UnInit()
{
	//在本测试用例集执行后调用
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
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt), "获取设备id失败\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "一个设备也不存在\n");
	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			dwVolumeId = dwIds[i];
            
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo), "获取分卷信息失败\n");
			CSTK_ASSERT_TRUE_FATAL(sDeviceInfo.m_szMountPoint != CSUDI_NULL, "分卷设备无挂载点\n");
			CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "分卷设备载点错误\n");

			memset(g_szVolumeMountPoint, 0, CSUDI_MAX_FILE_NAME_LEN);
			strcpy(g_szVolumeMountPoint, sDeviceInfo.m_szMountPoint);

			nLen = strlen(g_szVolumeMountPoint);
            
			CSTK_ASSERT_TRUE_FATAL(nLen < CSUDI_MAX_FILE_NAME_LEN, "路径过长或有误\n");

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

// 获取本地文件系统可写分卷挂载点
static CSUDI_BOOL FS_iLocalFsGetRWVolumeMountPoint(void)
{
	CSUDI_UINT32 dwIds[32];
	int nActIdCnt = 0; 
	int i = 0;
	int nLen = 0;
	CSUDIFSDeviceInfo_S sDeviceInfo;
	CSUDIFSStatFs_S sFsStat;

	FS_iInitCfg();
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt), "获取设备id失败");

	for (i = 0; i < nActIdCnt; i++)
	{
		if (CSUDI_ISREMOVEABLE(dwIds[i]))
		{
			continue;
		}

		memset(&sDeviceInfo, 0x00, sizeof(CSUDIFSDeviceInfo_S));
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwIds[i], &sDeviceInfo), "获取分卷信息失败");
		if (sDeviceInfo.m_eDeviceType != EM_UDIFS_DEVTYPE_VOLUME)
		{
			continue;
		}

		CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "分卷设备挂载点错误");
        memset(&sFsStat,0,sizeof(sFsStat));
		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStatfs(sDeviceInfo.m_szMountPoint, &sFsStat), "获取分卷信息错误");

		if ((sFsStat.m_eFsType > EM_UDIFS_PARTITION_RW_START) && (sFsStat.m_eFsType < EM_UDIFS_PARTITION_RW_END))
		{
			CSTK_ASSERT_TRUE_FATAL(!strcmp(sDeviceInfo.m_szMountPoint, g_aucLocalFsReadWritePath), "本地可写分卷挂载点与配置文件不同");
			strcpy(g_szVolumeMountPoint, sDeviceInfo.m_szMountPoint);
			nLen = strlen(g_szVolumeMountPoint);
			CSTK_ASSERT_TRUE_FATAL(nLen < CSUDI_MAX_FILE_NAME_LEN, "路径过长或有误");
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
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt), "获取设备id失败\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "一个设备也不存在\n");
	for(i = 0; i < nActIdCnt; i++)
	{
		if(CSUDI_ISREMOVEABLE(dwIds[i]) == 0x00000000)
		{
			dwVolumeId = dwIds[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo), "获取分卷信息失败\n");
			CSTK_ASSERT_TRUE_FATAL(sDeviceInfo.m_szMountPoint != CSUDI_NULL, "分卷设备无挂载点\n");
			CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "分卷设备载点错误\n");
			
			memset(g_szVolumeMountPoint, 0, CSUDI_MAX_FILE_NAME_LEN);
			strcpy(g_szVolumeMountPoint, sDeviceInfo.m_szMountPoint);

			nLen = strlen(g_szVolumeMountPoint);
            
			CSTK_ASSERT_TRUE_FATAL(nLen < CSUDI_MAX_FILE_NAME_LEN, "路径过长或有误\n");

			if(g_szVolumeMountPoint[nLen -1] != '/')
			{
				g_szVolumeMountPoint[nLen] = '/';
				g_szVolumeMountPoint[nLen + 1] = '\0';
			}
            
			CSFSPrint("测试分区的设备ID为%lu\n", sDeviceInfo.m_dwDeviceId);
			CSFSPrint("测试分区的设备名称为%s\n", sDeviceInfo.m_szDeviceName);
			CSFSPrint("测试分区的设备大小为%lu\n\n", sDeviceInfo.m_dwDeviceSize);        
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
//@DESCRIPTION: 正常注册回调，包括回调函数和用户数据
//@PRECONDITION: 无
//@INPUT: fnCallback = CSUDIFSRDIAddCallback_0001_Callback
//@INPUT: pvUserData =  &nUserData, 其中int  nUserData = 1;
//@EXPECTATION: 注册成功，当插入U盘时在回调函数中收到EM_UDIRDI_EVENT_READY消息
//@EXECUTIONFLOW: 1、调用CSUDIFSRDIAddCallback(CSUDIFSRDIAddCallback_0001_Callback, &nUserData)注册回调，返回值应该等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、根据提示插入U盘，待插入并挂载成功后按'Y'键确认，检查在回调函数CSUDIFSRDIAddCallback_0001_Callback中是否收到EM_UDIRDI_EVENT_READY消息，并且用户数据是否与注册时一致，无则失败
//@EXECUTIONFLOW: 3、调用CSUDIFSRDIRemoveCallback(CSUDIFSRDIAddCallback_0001_Callback, &nUserData)移除回调，恢复环境
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIAddCallback_0001(void)
{
	//手动测试
	CSUDI_Error_Code nErrorCode = CSUDI_SUCCESS;
	
	g_bCSUDIFSRDIAddCallback_0001 = CSUDI_FALSE;
	g_nCSUDIFSRDIAddCallback_0001 = 1;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCode = CSUDIFSRDIAddCallback(CSUDIFSRDIAddCallback_0001_Callback,
		&g_nCSUDIFSRDIAddCallback_0001)),"添加回调失败");

	CSFSPrint("请重新插入u盘，待挂载成功后，请按Y键确认\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，用例失败");

	CSTK_ASSERT_TRUE_FATAL(g_bCSUDIFSRDIAddCallback_0001, "插盘后回调函数中没有收到EM_UDIRDI_EVENT_READY消息");

	CSTK_FATAL_POINT
	{
		if (nErrorCode == CSUDI_SUCCESS)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(CSUDIFSRDIAddCallback_0001_Callback, 
				&g_nCSUDIFSRDIAddCallback_0001), "移除回调失败");
		}
	}    
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSRDIAddCallback(CSUDIRDICallback_F fnCallback,const void * pvUserData)
//@DESCRIPTION: 添加空的回调函数
//@PRECONDITION: 无
//@INPUT: fnCallback = CSUDI_NULL
//@INPUT:  pvUserData = &nUserData, 其中int  nUserData = 1
//@EXPECTATION: 注册失败，返回值不等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIFSRDIAddCallback(CSUDI_NULL, &nUserData)注册回调，返回值应该不等于CSUDI_SUCCESS
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIAddCallback_0002(void)
{
	int  nUserData = 1;
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFSRDIAddCallback(CSUDI_NULL,&nUserData),"添加空回调函数不应该成功\n");	

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
//@DESCRIPTION: 指定用户数据为空
//@PRECONDITION: 无
//@INPUT: fnCallback = CSUDIFSRDIAddCallback_0003_Callback
//@INPUT: pvUserData = CSUDI_NULL
//@EXPECTATION: 注册成功，且在回调函数CSUDIFSRDIAddCallback_0003_Callback中收到EM_UDIRDI_EVENT_READY消息
//@EXECUTIONFLOW: 1、调用SUDIFSRDIAddCallback(CSUDIFSRDIAddCallback_0003_Callback, CSUDI_NULL)注册回调，应当返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、根据提示插入U盘，待插入并挂载成功后按'Y'键确认，检查在回调函数CSUDIFSRDIAddCallback_0003_Callback中是否收到EM_UDIRDI_EVENT_READY消息，无则失败
//@EXECUTIONFLOW: 3、调用CSUDIFSRDIRemoveCallback(CSUDIFSRDIAddCallback_0003_Callback, CSUDI_NULL)移除回调，恢复环境
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIAddCallback_0003(void)
{
	//手动测试
	CSUDI_Error_Code nErrorCode = CSUDI_SUCCESS;
	g_eCSUDIFSRDIAddCallback_0003 = EM_UDIRDI_EVENT_UNKNOWN;
    
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCode = CSUDIFSRDIAddCallback(CSUDIFSRDIAddCallback_0003_Callback,
		CSUDI_NULL)),"添加回调失败\n");

	CSFSPrint("请重新插入u盘，待挂载成功后，请按Y键确认\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，用例失败");  

	CSTK_ASSERT_TRUE_FATAL(g_eCSUDIFSRDIAddCallback_0003 == EM_UDIRDI_EVENT_READY, "插盘后回调函数中没有收到EM_UDIRDI_EVENT_READY消息");

	CSTK_FATAL_POINT
	{
		if (nErrorCode == CSUDI_SUCCESS)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(CSUDIFSRDIAddCallback_0003_Callback, 
			  	CSUDI_NULL), "移除回调失败\n");
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
//@DESCRIPTION: 同一回调函数和数据两次注册
//@PRECONDITION: 
//@INPUT: fnCallback = CSUDIFSRDIAddCallback_0004_Callback
//@INPUT: pvUserData = &nUserData, 其中int  nUserData = 1
//@EXPECTATION: 第一次注册成功，第二次注册失败，且发生一个设备事件回调函数只调用一次
//@EXECUTIONFLOW: 1、调用CSUDIFSRDIAddCallback(CSUDIFSRDIAddCallback_0004_Callback, &nUserData)注册回调，应当返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIFSRDIAddCallback(CSUDIFSRDIAddCallback_0004_Callback, &nUserData)再次注册回调,应当返回CSUDIFS_ERROR_ALREADY_ADDED
//@EXECUTIONFLOW: 3、根据提示插入U盘，待挂载成功后按'Y'键确认，检查对于同一设备事件，回调函数执行了几次，应当只执行一次
//@EXECUTIONFLOW: 4、调用CSUDIFSRDIRemoveCallback(CSUDIFSRDIAddCallback_0004_Callback, &nUserData)移除回调，恢复环境
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIAddCallback_0004(void)
{
	//手动测试
	int  nUserData = 1;
	CSUDI_Error_Code nErrorCode = CSUDI_SUCCESS;

	g_nCSUDIFSRDIAddCallback_0004 = 0;
	g_nCSUDIFSRDIAddCallback_0004_DeviceId = -1;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCode = CSUDIFSRDIAddCallback(CSUDIFSRDIAddCallback_0004_Callback,
		&nUserData)),"添加回调失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDIFS_ERROR_ALREADY_ADDED == (nErrorCode = CSUDIFSRDIAddCallback(CSUDIFSRDIAddCallback_0004_Callback,
	&nUserData)),"再次添加回调应当返回CSUDIFS_ERROR_ALREADY_ADDED\n");

	CSFSPrint("请重新插入u盘，待挂载成功后，请按Y键确认\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，用例失败");  

	CSTK_ASSERT_TRUE_FATAL(g_nCSUDIFSRDIAddCallback_0004 == 1, "插盘后回调函数中同一事件或者没收到或者收到多次\n");
			
	CSTK_FATAL_POINT
	{
		if (nErrorCode == CSUDIFS_ERROR_ALREADY_ADDED)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(CSUDIFSRDIAddCallback_0004_Callback, 
			  	&nUserData), "移除回调失败\n");
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
//@DESCRIPTION: 同一回调函数重复注册，但用户数据不重复
//@PRECONDITION: 
//@INPUT: fnCallback = CSUDIFSRDIAddCallback_0005_Callback
//@INPUT: pvUserData = &g_nCSUDIFSRDIAddCallback_0005_UserDataB，其中int g_nCSUDIFSRDIAddCallback_0005_UserDataB 
//@EXPECTATION: 
//@EXECUTIONFLOW: 1、调用CSUDIFSRDIAddCallback(CSUDIFSRDIAddCallback_0005_Callback, &g_nCSUDIFSRDIAddCallback_0005_UserDataA)第一次注册回调函数，应当返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIFSRDIAddCallback(CSUDIFSRDIAddCallback_0005_Callback, &g_nCSUDIFSRDIAddCallback_0005_UserDataB)第二次注册回调函数，应当返回CSUDI_SUCCESS	
//@EXECUTIONFLOW: 3、按提示插上U盘，待挂载成功后，检查对于同一设备同一事件回调函数的执行情况，回调函数应当被调用两次，且用户数据分别为g_nCSUDIFSRDIAddCallback_0005_UserDataA和g_nCSUDIFSRDIAddCallback_0005_UserDataB
//@EXECUTIONFLOW: 4、调用CSUDIFSRDIRemoveCallback(CSUDIFSRDIAddCallback_0005_Callback, &g_nCSUDIFSRDIAddCallback_0005_UserDataA)移除第一次回调，恢复环境
//@EXECUTIONFLOW: 5、调用CSUDIFSRDIRemoveCallback(CSUDIFSRDIAddCallback_0005_Callback, &g_nCSUDIFSRDIAddCallback_0005_UserDataB)移除第二次回调，恢复环境
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIAddCallback_0005(void)
{

	//手动测试
	CSUDI_Error_Code nErrorCodeA = CSUDI_SUCCESS;
	CSUDI_Error_Code nErrorCodeB = CSUDI_SUCCESS;
	g_nCSUDIFSRDIAddCallback_0005_DeviceId = -1;
	g_nCSUDIFSRDIAddCallback_0005 = 0;
	g_nCSUDIFSRDIAddCallback_0005_UserDataA = 0;
	g_nCSUDIFSRDIAddCallback_0005_UserDataB = 0;
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCodeA = CSUDIFSRDIAddCallback(CSUDIFSRDIAddCallback_0005_Callback,
		&g_nCSUDIFSRDIAddCallback_0005_UserDataA)),"添加回调失败\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCodeB = CSUDIFSRDIAddCallback(CSUDIFSRDIAddCallback_0005_Callback,
	&g_nCSUDIFSRDIAddCallback_0005_UserDataB)),"再次添加回调失败\n");

	CSFSPrint("请重新插入u盘，待挂载成功后，请按Y键确认\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，用例失败");  

	CSTK_ASSERT_TRUE_FATAL(g_nCSUDIFSRDIAddCallback_0005 == 2, "同一设备同一事件收到的消息不是两次，或多或少\n");

	CSTK_ASSERT_TRUE_FATAL(g_nCSUDIFSRDIAddCallback_0005_UserDataA == 1, "调用不是一次\n");

	CSTK_ASSERT_TRUE_FATAL(g_nCSUDIFSRDIAddCallback_0005_UserDataB == 1, "调用不是一次\n");
	
	CSTK_FATAL_POINT
	{
		if (nErrorCodeA == CSUDI_SUCCESS)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(CSUDIFSRDIAddCallback_0005_Callback, 
			  	&g_nCSUDIFSRDIAddCallback_0005_UserDataA), "移除回调失败\n");
		}
		if (nErrorCodeB == CSUDI_SUCCESS)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(CSUDIFSRDIAddCallback_0005_Callback, 
			  	&g_nCSUDIFSRDIAddCallback_0005_UserDataB), "移除回调失败\n");
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
//@DESCRIPTION: 同一用户数据重复注册，但回调函数不重复
//@PRECONDITION: 
//@INPUT: fnCallback = CSUDIFSRDIAddCallback_0006_Callback2
//@INPUT: pvUserData = &nUserData，其中int nUserData 
//@EXPECTATION: 两次注册都成功，且当有设备事件发生时两个回调函数都能被调用
//@EXECUTIONFLOW: 1、调用CSUDIFSRDIAddCallback(CSUDIFSRDIAddCallback_0006_Callback1, &nUserData)注册回调函数
//@EXECUTIONFLOW: 2、调用CSUDIFSRDIAddCallback(CSUDIFSRDIAddCallback_0006_Callback2, &nUserData)再次注册回调函数
//@EXECUTIONFLOW: 3、插入U盘，查看两个回调函数中是否各自收到回调
//@EXECUTIONFLOW: 4、调用CSUDIFSRDIRemoveCallback(CSUDIFSRDIAddCallback_0006_Callback1, &nUserData)移除回调，恢复环境
//@EXECUTIONFLOW: 5、调用CSUDIFSRDIRemoveCallback(CSUDIFSRDIAddCallback_0006_Callback2, &nUserData)移除回调，恢复环境
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIAddCallback_0006(void)
{
	//手动测试
	int  nUserData = 1;
	CSUDI_Error_Code nErrorCodeA = CSUDI_SUCCESS;
	CSUDI_Error_Code nErrorCodeB = CSUDI_SUCCESS;

	g_nCSUDIFSRDIAddCallback_0006_A = CSUDI_FALSE;
	g_nCSUDIFSRDIAddCallback_0006_B = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCodeA = CSUDIFSRDIAddCallback(CSUDIFSRDIAddCallback_0006_Callback1,
		&nUserData)),"添加回调失败\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCodeB = CSUDIFSRDIAddCallback(CSUDIFSRDIAddCallback_0006_Callback2,
	&nUserData)),"再次添加回调失败\n");

	CSFSPrint("请重新插入u盘，待挂载成功后，请按Y键确认\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，用例失败");  

	CSTK_ASSERT_TRUE_FATAL((g_nCSUDIFSRDIAddCallback_0006_A && g_nCSUDIFSRDIAddCallback_0006_B),
		"不是两个回调函数都收到的事件\n");
		
	CSTK_FATAL_POINT
	{
		if (nErrorCodeA == CSUDI_SUCCESS)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(CSUDIFSRDIAddCallback_0006_Callback1, 
			  	&nUserData), "移除回调失败\n");
		}
		if (nErrorCodeB == CSUDI_SUCCESS)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(CSUDIFSRDIAddCallback_0006_Callback2, 
			  	&nUserData), "移除回调失败\n");
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
//@DESCRIPTION: 删除已经添加的回调函数和用户数据
//@PRECONDITION: 
//@INPUT: fnCallback = CSUDIFSRDIRemoveCallback_0001_Callback
//@INPUT: pvUserData = &g_nCSUDIFSRDIRemoveCallback_0001, 其中int g_nCSUDIFSRDIRemoveCallback_0001 
//@EXPECTATION: 能够成功删除已经添加的回调函数
//@EXECUTIONFLOW: 1、调用CSUDIFSRDIAddCallback(CSUDIFSRDIRemoveCallback_0001_Callback, &g_nCSUDIFSRDIRemoveCallback_0001)注册回调，应当返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0001_Callback, &g_nCSUDIFSRDIRemoveCallback_0001)移除回调，应当返回CSUDI_SUCCESS	
//@EXECUTIONFLOW: 3、调用CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0001_Callback, &g_nCSUDIFSRDIRemoveCallback_0001)移除已经移除的回调，应当返回非CSUDI_SUCCESS	
//@EXECUTIONFLOW: 4、按提示插入U盘，待挂载成功后按'Y'键确认，查看回调函数是否被调用，应当不会被调用
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIRemoveCallback_0001(void)
{
	//手动测试
	CSUDI_Error_Code nErrorCode = CSUDI_SUCCESS;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCode = CSUDIFSRDIAddCallback(CSUDIFSRDIRemoveCallback_0001_Callback,
		&g_nCSUDIFSRDIRemoveCallback_0001)),"添加回调失败\n");

	g_nCSUDIFSRDIRemoveCallback_0001 = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0001_Callback, 
		&g_nCSUDIFSRDIRemoveCallback_0001), "移除回调失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0001_Callback, 
		&g_nCSUDIFSRDIRemoveCallback_0001), "移除已经移除的回调不应该成功\n");

	CSFSPrint("请重新插入u盘，待挂载成功后，请按Y键确认\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，用例失败");  

	CSTK_ASSERT_TRUE_FATAL(g_nCSUDIFSRDIRemoveCallback_0001 == 0, "移除回调后还收到了事件\n");

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
//@DESCRIPTION: 删除不存在的回调(注册时不带用户数据，删除时带用户数据)
//@PRECONDITION: 
//@INPUT: fnCallback = CSUDIFSRDIRemoveCallback_0002_Callback
//@INPUT: pvUserData = &nUserData，其中int nUserData
//@EXPECTATION: 删除失败
//@EXECUTIONFLOW: 1、调用CSUDIFSRDIAddCallback(CSUDIFSRDIRemoveCallback_0002_Callback, CSUDI_NULL)注册回调，应当返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0002_Callback, &nUserData)移除回调，返回值应当不等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、按提示插入U盘，待挂载成功后按'Y'键确认，查看回调函数是否被调用，应当被调用
//@EXECUTIONFLOW: 4、调用CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0002_Callback, CSUDI_NULL)移除回调, 返回值应当等于CSUDI_SUCCESS
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIRemoveCallback_0002(void)
{
	//手动测试
	int  nUserData = 1;
	CSUDI_Error_Code nErrorCode = CSUDI_SUCCESS;
	g_eCSUDIFSRDIRemoveCallback_0002 = EM_UDIRDI_EVENT_UNKNOWN;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCode = CSUDIFSRDIAddCallback(CSUDIFSRDIRemoveCallback_0002_Callback,
		CSUDI_NULL)),"添加回调失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0002_Callback, 
		&nUserData), "移除回调不应该成功\n");

	CSFSPrint("请重新插入u盘，待挂载成功后，请按Y键确认\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，用例失败");  

	CSTK_ASSERT_TRUE_FATAL(g_eCSUDIFSRDIRemoveCallback_0002 == EM_UDIRDI_EVENT_FOUND, 
		"回调函数中没有收到事件");
	
	CSTK_FATAL_POINT
	{
		if (nErrorCode == CSUDI_SUCCESS)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == 
				CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0002_Callback, CSUDI_NULL), "移除回调失败\n");;
		}
	}

	return CSUDI_TRUE;
}

void CSUDIFSRDIRemoveCallback_0003_Callback( CSUDIRDIEvent_E eType, CSUDI_UINT32 dwDeviceId,const void * pvEventData, void * pvUserData)
{
	return;
}
//@CASEGROUP: CSUDIFSRDIRemoveCallback
//@DESCRIPTION: 删除不存在的回调(回调函数和用户数据都未注册)
//@PRECONDITION: 
//@INPUT: fnCallback = CSUDIFSRDIRemoveCallback_0003_Callback
//@INPUT: pvUserData = &nUserData,其中int nUserData
//@EXPECTATION: 删除失败，返回值不等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0003_Callback, &nUserData)删除回调，不应当返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIRemoveCallback_0003(void)
{
	int  nUserData = 1;
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0003_Callback, 
		&nUserData), "移除回调不应该成功\n");

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
//@DESCRIPTION: 删除不存在的回调(注册时带用户数据，删除时不带用户数据)
//@PRECONDITION: 
//@INPUT: fnCallback = CSUDIFSRDIRemoveCallback_0004_Callback
//@INPUT: pvUserData = CSUDI_NULL
//@EXPECTATION: 删除失败，返回值不等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIFSRDIAddCallback(CSUDIFSRDIRemoveCallback_0004_Callback,&nUserData)注册回调，应当返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0004_Callback, CSUDI_NULL)移除回调，返回应该不等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0004_Callback, &nUserData)移除回调，返回值应该等于CSUDI_SUCCESS
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIRemoveCallback_0004(void)
{
	int  nUserData = 1;
	CSUDI_Error_Code nErrorCode = CSUDI_SUCCESS;
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCode = CSUDIFSRDIAddCallback(CSUDIFSRDIRemoveCallback_0004_Callback,
		&nUserData)),"添加回调失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0004_Callback, 
		CSUDI_NULL), "移除回调不应该成功\n");

	CSTK_FATAL_POINT
	{
		if(nErrorCode == CSUDI_SUCCESS)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0004_Callback, 
				&nUserData), "移除回调失败\n");
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
//@DESCRIPTION: 删除已经存在的回调（注册时不带用户数据，删除时也不带用户数据）
//@PRECONDITION: 
//@INPUT: fnCallback = CSUDIFSRDIRemoveCallback_0005_Callback
//@INPUT: pvUserData = CSUDI_NULL
//@EXPECTATION: 删除成功，返回值等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIFSRDIAddCallback(fnCallback, CSUDI_NULL)，应当返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIFSRDIRemoveCallback(fnCallback, CSUDI_NULL),应当返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、按提示插入U盘，待挂载成功后按'Y'键确认，查看回调函数是否被调用，预期不应当被调用
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIRemoveCallback_0005(void)
{
	//手动测试
	CSUDI_Error_Code nErrorCode = CSUDI_SUCCESS;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCode = CSUDIFSRDIAddCallback(CSUDIFSRDIRemoveCallback_0005_Callback,
		CSUDI_NULL)),"添加回调失败\n");

	CSUDIFSRDIRemoveCallback_0005 = EM_UDIRDI_EVENT_UNKNOWN;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0005_Callback, 
		CSUDI_NULL), "移除回调失败\n");

	CSFSPrint("请重新插入u盘，待挂载成功后，请按Y键确认\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，用例失败");  

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSRDIRemoveCallback_0005 == EM_UDIRDI_EVENT_UNKNOWN, "删除回调后回调函数中不应该收到事件");

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
//@DESCRIPTION: 删除不存在的回调(回调函数和用户数据都未注册)
//@PRECONDITION: 未添加回调函数
//@INPUT: fnCallback正常
//@INPUT: pvUserData = &nUserData, int nUserData
//@EXPECTATION: 删除失败，返回值不等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0006_Callback, &nUserData)删除回调，应当失败且返回值不等于CSUDI_SUCCESS	
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIRemoveCallback_0006(void)
{
	int  nUserData = 1;
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveCallback_0006_Callback, 
		&nUserData), "移除回调不应该成功\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSRDIRemoveCallback(CSUDIRDICallback_F fnCallback,const void * pvUserData)
//@DESCRIPTION: 删除不存在的回调（没指定要删除的回调函数）
//@PRECONDITION: 
//@INPUT: fnCallback = CSUDI_NULL
//@INPUT: pvUserData = &nUserData, 其中int nUserData
//@EXPECTATION: 删除失败，返回值不等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIFSRDIRemoveCallback(CSUDI_NULL, &nUserData)返回值应当不等于CSUDI_SUCCESS	
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIRemoveCallback_0007(void)
{
	int  nUserData = 1;
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFSRDIRemoveCallback(CSUDI_NULL, &nUserData), "移除回调不应该成功\n");

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
//@DESCRIPTION: 移除移动设备
//@PRECONDITION: 1、有移动设备插上，并已识别
//@INPUT: dwDeviceId = dwDeviceId1，dwDeviceId1是通过调用CSUDIFSGetAllDeviceId()得到的移动设备
//@EXPECTATION: 移除成功，且回调函数中收到事件EM_UDIRDI_EVENT_PLUGOUT，移除后不能再获得该设备的信息
//@EXECUTIONFLOW: 1、调用CSUDIFSRDIAddCallback()添加一个回调函数，应当返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIFSGetAllDeviceId()得到一个所有设备id,并根据主设备号从中找到一个移动设备dwDeviceId1，如果没有移动则测试失败
//@EXECUTIONFLOW: 3、调用CSUDIFSRDIRemoveDev(dwDeviceId1)移除设备，应当返回CSUDI_SUCCESS	
//@EXECUTIONFLOW: 4、检查回调函数是否收到EM_UDIRDI_PLUGOUT_SAFE类型的事件EM_UDIRDI_EVENT_PLUGOUT
//@EXECUTIONFLOW: 5、调用CSUDIFSRDIRemoveCallback()删除回调，恢复环境
//@EXECUTIONFLOW: 6、调用CSUDIFSGetDeviceInfo(dwDeviceId1, &sDeviceInfo)获取设备信息，应当失败且返回值不等于CSUDI_SUCCESS
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIRemoveDev_0001(void)
{
	//手动测试
	CSUDI_Error_Code nErrorCode = CSUDI_SUCCESS;
	CSUDI_UINT32  dwIds[32];
	int nActIdCnt = 0;
	int i = 0;
	CSUDIFSDeviceInfo_S  sDeviceInfo;
	CSUDI_BOOL bRemoved = CSUDI_FALSE;

	g_eCSUDIFSRDIRemoveDev_0001 = EM_UDIRDI_EVENT_UNKNOWN;
	g_nCSUDIFSRDIRemoveDev_0001 = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCode = CSUDIFSRDIAddCallback(CSUDIFSRDIRemoveDev_0001_Callback,
		CSUDI_NULL)),"添加回调失败\n");

	CSFSPrint("请重新插入u盘，待挂载成功后，请按Y键确认\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，用例失败"); 

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt), "获取设备id失败\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "一个设备也未获得\n");
	for(i = 0; i < nActIdCnt; i++)
	{
		if(CSUDI_ISREMOVEABLE(dwIds[i]) && (dwIds[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_STORAGE_MAJOR_DEVICEID)
		{
			g_nCSUDIFSRDIRemoveDev_0001  = dwIds[i];

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveDev(dwIds[i]), "移除移动设备失败\n");
			bRemoved = CSUDI_TRUE;
			break;
		}	
	}

	CSTK_ASSERT_TRUE_FATAL(g_eCSUDIFSRDIRemoveDev_0001 == EM_UDIRDI_EVENT_PLUGOUT, "删除移动设备后回调函数中没有收到EM_UDIRDI_EVENT_PLUGOUT消息");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFSGetDeviceInfo(g_nCSUDIFSRDIRemoveDev_0001, &sDeviceInfo), "获取设备信息不应该成功\n");

	CSTK_FATAL_POINT
	{
		if (nErrorCode == CSUDI_SUCCESS)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveDev_0001_Callback, 
				NULL), "移除回调失败\n");
		}

		if(bRemoved == CSUDI_TRUE)
		{
			CSFSPrint("U盘已经移除，请拔掉后重新插入，待挂载成功后，按Y确认!\n");
			CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，恢复环境失败，会影响后续测试用例执行");  
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSRDIRemoveDev(CSUDI_UINT32 dwDeviceId)
//@DESCRIPTION: 移除非移动类型设备
//@PRECONDITION: 系统中有已经识别的非移动设备
//@INPUT: dwDeviceId = dwDeviceId1，dwDeviceId1是通过调用CSUDIFSGetAllDeviceId()得到的非移动设备
//@EXPECTATION: 移除失败，返回值不等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个非移动设备dwDeviceId1
//@EXECUTIONFLOW: 2、调用CSUDIFSRDIRemoveDev(dwDeviceId1)移除设备，应当返回CSUDIFS_ERROR_FEATURE_NOT_SUPPORTED	
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIRemoveDev_0002(void)
{
	CSUDI_UINT32  dwIds[32];
	int nActIdCnt = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt), "获取设备id失败\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "一个设备也未获得\n");

	while(nActIdCnt-- > 0)
	{
		if(!CSUDI_ISREMOVEABLE(dwIds[nActIdCnt]))
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDIFS_ERROR_FEATURE_NOT_SUPPORTED == CSUDIFSRDIRemoveDev(dwIds[nActIdCnt]), "非移动设备不支持移除\n");
			break;
		}
	}

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt >= 0, "找不到非移动设备");
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSRDIRemoveDev(CSUDI_UINT32 dwDeviceId)
//@DESCRIPTION: 移除不存在的设备
//@PRECONDITION: 
//@INPUT: dwDeviceId = 0x0001FFFF(假设0x0001FFFF为非法的设备id)
//@EXPECTATION: 移除失败，返回值不等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIFSRDIRemoveDev(0x0001FFFF)移除设备，返回值应当不等于CSUDI_SUCCESS	
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIRemoveDev_0003(void)
{
	CSTK_ASSERT_TRUE(CSUDI_SUCCESS != CSUDIFSRDIRemoveDev(0x0001FFFF), "移除不存在的设备不应该成功\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSRDIRemoveDev(CSUDI_UINT32 dwDeviceId)
//@DESCRIPTION: 移除正在被使用的设备
//@PRECONDITION: 有移动设备插上，并已识别
//@INPUT: dwDeviceId = dwDeviceId1，dwDeviceId1是通过调用CSUDIFSGetAllDeviceId()得到的移动设备
//@EXPECTATION: 移除失败，返回值不等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个移动设备dwDeviceId1
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()得到该设备的挂载点
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen("分区挂载点"+"CSUDIFSRDIRemoveDev_0004")创建并打开一个文件，得到其句柄hFile，句柄应当不等于CSUDI_NULL
//@EXECUTIONFLOW: 4、调用CSUDIFSRDIRemoveDev(dwDeviceId1)移除设备，应当失败且返回值不等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 5、调用CSUDIFSClose(hFile)关闭文件，返回值应当等于0
//@EXECUTIONFLOW: 6、调用CSUDIFSRemove("分区挂载点"+"CSUDIFSRDIRemoveDev_0004")删除文件，返回值应当为0
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIRemoveDev_0004(void)
{
	//手动测试
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
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt), "获取设备id失败\n");
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "一个设备也未获得\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwIds[i], &sDeviceInfo), "获取分卷信息失败\n");

			strcpy(szFilePath, sDeviceInfo.m_szMountPoint);
			nLen = strlen(szFilePath);
			CSTK_ASSERT_TRUE_FATAL(nLen < CSUDI_MAX_FILE_NAME_LEN, "路径过长或有误\n");

			if(szFilePath[nLen -1] != '/')
			{
				szFilePath[nLen] = '/';
				szFilePath[nLen + 1] = '\0';
			}
			strcat(szFilePath, szTest);

			CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w")), "创建并打开文件失败\n");

			bRemoved = CSUDI_TRUE;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFSRDIRemoveDev(sDeviceInfo.m_dwParentDeviceId), "设备上有文件被打开应当不能被移除\n");
			bRemoved = CSUDI_FALSE;

			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");
			
			break;
		}	
	}

	CSTK_FATAL_POINT
	{
		if (hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRemove(szFilePath), "删除文件失败\n");
		}

		/* 防止CSUDIFSRDIRemoveDev接口调用成功，没有恢复环境(默认插入u盘) */
		if (bRemoved == CSUDI_TRUE)
		{
			CSTCPrint("请拔掉U盘后重新插入，待挂载成功后，按Y确认!\n");
			CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，恢复环境失败，会影响后续测试用例执行");  
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
//@DESCRIPTION: 移除一个至少有两个分卷的移动存储设备的其中一个分卷
//@PRECONDITION: 1、移动存储设备插上，并已被识别
//@INPUT: 通过调用CSUDIFSGetAllDeviceId()得到的分卷设备id
//@EXPECTATION: 移动存储设备移除成功，且在卸载存储设备时，回调函数中收到事件EM_UDIRDI_EVENT_PLUGOUT
//@EXECUTIONFLOW: 1、调用CSUDIFSRDIAddCallback()添加一个回调函数，应当返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIFSGetAllDeviceId()得到所有设备id,并根据主设备号从中找到分卷设备id，如果没有找到分卷设备则测试失败
//@EXECUTIONFLOW: 3、调用CSUDIFSGetDeviceInfo()获取分卷设备信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、调用CSUDIFSRDIRemoveDev(dwDeviceId1)移除存储设备，应当返回CSUDI_SUCCESS	
//@EXECUTIONFLOW: 5、检查回调函数是否收到EM_UDIRDI_PLUGOUT_SAFE类型的事件EM_UDIRDI_EVENT_PLUGOUT
//@EXECUTIONFLOW: 6、调用CSUDIFSRDIRemoveCallback()删除回调，恢复环境
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIRemoveDev_0005(void)
{
	//手动测试
	CSUDI_Error_Code nErrorCode = CSUDI_SUCCESS;
	CSUDI_UINT32  dwIds[32];
	int nActIdCnt = 0;
	int i = 0;
	CSUDI_BOOL bRemoved = CSUDI_FALSE;

	CSUDIFSDeviceInfo_S sDeviceInfo;

	g_nRDIRemoveDevId_0005 = 0;
	g_eRDIRemoveDevEvent_0005 = EM_UDIRDI_EVENT_UNKNOWN;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCode = CSUDIFSRDIAddCallback(CSUDIFSRDIRemoveDev_0005_Callback,
		CSUDI_NULL)),"添加回调失败\n");

	CSFSPrint("请插入一个至少有两个分卷的移动存储设备，插入并挂载成功后，请按Y确认\n");

	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，用例失败"); 

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt), "获取设备id失败\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "一个设备也未获得\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwIds[i], &sDeviceInfo), "获取分卷设备信息失败\n");

			g_nRDIRemoveDevId_0005 = dwIds[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveDev(dwIds[i]), "移除移动分卷设备失败\n");
			bRemoved = CSUDI_TRUE;

			break;
		}
	}

	CSTK_ASSERT_TRUE_FATAL(g_eRDIRemoveDevEvent_0005 == EM_UDIRDI_EVENT_PLUGOUT, "删除移动设备后回调函数中没有收到EM_UDIRDI_EVENT_PLUGOUT消息");
	
	CSTK_FATAL_POINT
	{
		if (nErrorCode == CSUDI_SUCCESS)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(CSUDIFSRDIRemoveDev_0005_Callback, 
				NULL), "移除回调失败\n");
		}
		if(bRemoved == CSUDI_TRUE)
		{
			CSFSPrint("请拔掉该设备插入U盘，待挂载成功后，请按Y确认\n");
			CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，恢复环境失败，会影响后续测试用例执行");  
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
			CSTCPrint("ThreadEntry1获取设备id失败\n");
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
			CSTCPrint("ThreadEntry2获取设备id失败\n");
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
//@DESCRIPTION: 同时获得所有的设备id和设备数
//@PRECONDITION: 系统中无可移动存储设备
//@INPUT: pdwIds = pdwIds1，其中CSUDI_UINT32 pdwIds1[32]
//@INPUT: nMaxIdCnt = 32
//@INPUT: pnActIdCnt = &nActIdCnt，其中int nActIdCnt
//@EXPECTATION: 返回值等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、拔掉U盘，若没有插U盘则不用拔除，拔除后按Y确认
//@EXECUTIONFLOW: 2、调用CSUDIFSGetAllDeviceId(pdwIds1,32,&nActIdCnt)获取设备数量，并保存在变量g_nCSUDIFSGetAllDeviceId_0001中	
//@EXECUTIONFLOW: 3、请插入U盘，待挂载成功，按Y确认
//@EXECUTIONFLOW: 4、调用CSUDIFSGetAllDeviceId(pdwIds1,32,&nActIdCnt)再次获取设备数量，判断第二次获取数是否大于第一次，是则成功，否则失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetAllDeviceId_0001(void)
{
	//手动测试
	CSUDI_UINT32  pdwIds1[32];
	int nActIdCnt = -1; 

	CSFSPrint("请拔掉U盘，按Y确认\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，用例失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(pdwIds1, 32, &nActIdCnt), "获取设备id失败\n");

	g_nCSUDIFSGetAllDeviceId_0001 = nActIdCnt;

	CSFSPrint("请插入U盘，待挂载成功后按Y确认\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，用例失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(pdwIds1, 32, &nActIdCnt), "获取设备id失败\n");
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > g_nCSUDIFSGetAllDeviceId_0001, "获取设备数量不对\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetAllDeviceId(CSUDI_UINT32 * pdwIds,int nMaxIdCnt, int * pnActIdCnt)
//@DESCRIPTION: 获取一个存储设备的分区数、分区id、分区挂载点
//@PRECONDITION: 系统中无可移动存储设备
//@INPUT: pdwIds = pdwIds1，其中CSUDI_UINT32 pdwIds1[32]
//@INPUT: nMaxIdCnt = 32
//@INPUT: pnActIdCnt = &nActIdCnt，其中int nActIdCnt
//@EXPECTATION: 得到的分区数和挂载点与实际一致(需要人工判断)
//@EXECUTIONFLOW: 1、插入已分区的U盘，待挂载成功后，按Y键确认
//@EXECUTIONFLOW: 2、调用CSUDIFSGetAllDeviceId(pdwIds1,32,&nActIdCnt)得到所有的设备
//@EXECUTIONFLOW: 3、根据主设备号打印出所有的分卷设备id、挂载点、父设备id，如果一个分卷设备也未发现或者所有的分卷设备的父设备id不相同则测试失败
//@EXECUTIONFLOW: 4、根据提示人工判断打印信息与实际是否相符，如果相符由按'Y'键，否则按'N'键
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetAllDeviceId_0002(void)
{
	//手动测试
	CSUDI_UINT32  dwIds[32];
	int nActIdCnt = 0;
	int i = 0;
	int nStorageId = -1;
	CSUDIFSDeviceInfo_S    sDeviceInfo;
	int   nCount = 0;
 	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt), "获取设备id失败\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "一个设备也未获得\n");
	
	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwIds[i], &sDeviceInfo), "获取设备信息失败\n");
			if(nStorageId == -1)
			{
				nStorageId = sDeviceInfo.m_dwParentDeviceId;
			}

			if((int)sDeviceInfo.m_dwParentDeviceId == nStorageId)
			{
				CSFSPrint("分区id = %d, 挂载路径 = %s, 父存储设备id = %d\n", dwIds[i], sDeviceInfo.m_szMountPoint, nStorageId);
				nCount++;
			}
			else
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "发现多个移动存储设备，应该只有一个\n");
			}
		}
	}

	CSTK_ASSERT_TRUE_FATAL(nCount != 0, "一个分卷设备也没发现\n");

	CSFSPrint("移动设备 %d 共有分区 %d 个\n", nStorageId, nCount);

	CSFSPrint("请确认分区个数与实际一致、id不为负、挂载路径不为空，三条信息全正确请按Y键，否则按N键\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "你选择了不属实，测试失败\n");
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
	
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetAllDeviceId(CSUDI_UINT32 * pdwIds,int nMaxIdCnt, int * pnActIdCnt)
//@DESCRIPTION: 只获得设备数，不获得设备id
//@PRECONDITION: 无
//@INPUT: pdwIds = CSUDI_NULL
//@INPUT: nMaxIdCnt = 0
//@INPUT: pnActIdCnt = &nActIdCnt，其中int nActIdCnt
//@EXPECTATION: 获取成功，返回值等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId(CSUDI_NULL, 0, &nActIdCnt)，返回值应该等于CSUDI_SUCCESS，同时nActIdCnt不小于零
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetAllDeviceId_0003(void)
{
	int nActIdCnt = 0; 

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(CSUDI_NULL, 0, &nActIdCnt), "获取设备id失败\n");
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt >= 0, "获取设备数量不对");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetAllDeviceId(CSUDI_UINT32 * pdwIds,int nMaxIdCnt, int * pnActIdCnt)
//@DESCRIPTION: 传入pnActIdCnt为非法值CSUDI_NULL
//@PRECONDITION: 无
//@INPUT: pdwIds = pdwIds1，其中CSUDI_UINT32 pdwIds1[32]
//@INPUT: nMaxIdCnt = 32
//@INPUT: pnActIdCnt = CSUDI_NULL
//@EXPECTATION: 获取失败，返回值不等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId(pdwIds1,32,CSUDI_NULL)获取设备id，返回值应该不等于CSUDI_SUCCESS
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetAllDeviceId_0004(void)
{
	CSUDI_UINT32  pdwIds1[32];

	CSTK_ASSERT_TRUE(CSUDI_SUCCESS != CSUDIFSGetAllDeviceId(pdwIds1, 32, CSUDI_NULL), "获取设备不应该成功\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetAllDeviceId(CSUDI_UINT32 * pdwIds,int nMaxIdCnt, int * pnActIdCnt)
//@DESCRIPTION: 接收id的数组大小非法
//@PRECONDITION: 无
//@INPUT: pdwIds = pdwIds1，其中CSUDI_UINT32 pdwIds1[32]
//@INPUT: nMaxIdCnt = -1
//@INPUT: pnActIdCnt = &nActIdCnt，其中int nActIdCnt
//@EXPECTATION: 获取失败，返回值不等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId(pdwIds1,0,&nActIdCnt)，返回值应当不等于CSUDI_SUCCESS	
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetAllDeviceId_0005(void)
{
	CSUDI_UINT32  pdwIds1[32];
	int nActIdCnt = 0;

	CSTK_ASSERT_TRUE(CSUDI_SUCCESS != CSUDIFSGetAllDeviceId(pdwIds1, -1, &nActIdCnt), "获取设备id不应该成功\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetAllDeviceId(CSUDI_UINT32 * pdwIds,int nMaxIdCnt, int * pnActIdCnt)
//@DESCRIPTION: 只获取部分设备
//@PRECONDITION: 已插入一个已格式化的u盘，并已识别
//@INPUT: pdwIds = pdwIds1，其中CSUDI_UINT32 pdwIds1[1]
//@INPUT: nMaxIdCnt = 1
//@INPUT: pnActIdCnt = &nActIdCnt，其中int nActIdCnt
//@EXPECTATION: 获取成功，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId(pdwIds1,1,&nActIdCnt)获取设备，应当返回CSUDI_SUCCESS，且nActIdCnt=1
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetAllDeviceId_0006(void)
{
	CSUDI_UINT32  pdwIds1[1];
	int nActIdCnt = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(pdwIds1, 1, &nActIdCnt), "获取设备id失败\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt >= 1, "获取设备数量不对");
		
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetAllDeviceId(CSUDI_UINT32 * pdwIds,int nMaxIdCnt, int * pnActIdCnt)
//@DESCRIPTION: 只获得设备数，不获得设备id，但nMaxIdCnt不为0
//@PRECONDITION: 无
//@INPUT: pdwIds = CSUDI_NULL
//@INPUT: nMaxIdCnt = 32
//@INPUT: pnActIdCnt = &nActIdCnt，其中int nActIdCnt
//@EXPECTATION: 获取成功，返回值等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId(CSUDI_NULL, 32, &nActIdCnt)，返回值应该等于CSUDI_SUCCESS，同时nActIdCnt不小于零
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetAllDeviceId_0007(void)
{
	int nActIdCnt = 0; 

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(CSUDI_NULL, 32, &nActIdCnt), "获取设备id失败\n");
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt >= 0, "获取设备数量不对\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetAllDeviceId(CSUDI_UINT32 * pdwIds,int nMaxIdCnt, int * pnActIdCnt)
//@DESCRIPTION: 在两个线程同时调用CSUDIFSGetAllDeviceId获取设备ID时会不会造成死锁
//@PRECONDITION: 无
//@INPUT: pdwIds = CSUDI_NULL
//@INPUT: nMaxIdCnt = 32
//@INPUT: pnActIdCnt = &nActIdCnt，其中int nActIdCnt
//@EXPECTATION: 两者都可以正常获取所有的设备ID
//@EXECUTIONFLOW: 1、创建线程1获取所有的设备ID
//@EXECUTIONFLOW: 2、创建线程2获取所有的设备ID
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetAllDeviceId_0008(void)
{
	int nPriorityHigh = 32;
	int nStckSize = 128*1024;
	CSUDI_HANDLE hThreadhandle1 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle2 = CSUDI_NULL;
	int nInitialCount = 1;
	int nMaxCount = 10;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("FS_LockTestSem", nInitialCount, nMaxCount, &g_hSemaphore), "信号量创建失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("Lock_TestThread1", nPriorityHigh , nStckSize, 
							FS_LockTestThreadEntry1, CSUDI_NULL, &hThreadhandle1),
							 "step 1 创建任务失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("Lock_TestThread2", nPriorityHigh , nStckSize, 
							FS_LockTestThreadEntry2, CSUDI_NULL, &hThreadhandle2),
							 "step 2 创建任务失败");
	while (!g_bThread2Return || !g_bThread2Return)
	{
		CSUDIOSThreadSleep(1000);
	}
	CSTK_ASSERT_TRUE_FATAL((!g_bThreadErr), "获取设备失败");

	CSTK_FATAL_POINT;
	if (g_hSemaphore != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(g_hSemaphore), "删除信号量失败");
			g_hSemaphore = CSUDI_NULL;
	}

	if (hThreadhandle1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle1), "删除任务失败");
			hThreadhandle1 = CSUDI_NULL;
	}

	if (hThreadhandle2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle2), "删除任务失败");
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
//@DESCRIPTION: 获得正常设备的信息
//@PRECONDITION: 系统中至少存在一个已识别的设备
//@INPUT: dwDeviceId = dwDeviceId1，dwDeviceId1是由CSUDIFSGetAllDeviceId()获得的设备号
//@INPUT: psDeviceInfo = &sDeviceInfo, 其中CSUDIFSDeviceInfo_S  sDeviceInfo
//@EXPECTATION: 返回值等于 CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已识别的设备dwDeviceId1	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo(dwDeviceId1, &sDeviceInfo)，判断返回值	
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetDeviceInfo_0001(void)
{
	CSUDI_UINT32   pdwIds1[32];
	int nActIdCnt = 0;
	CSUDIFSDeviceInfo_S  sDeviceInfo;
	int i = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(pdwIds1, 32, &nActIdCnt), "获取设备id失败\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(pdwIds1[i], &sDeviceInfo), "获取设备信息失败\n");
		
		if((pdwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_STORAGE_MAJOR_DEVICEID)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == strlen(sDeviceInfo.m_szMountPoint), "主设备不应该有挂载点" );
		}
		else if((pdwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "分卷设备应该有挂载点");
		}
		else if((pdwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_TUNER_MAJOR_DEVICEID)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == strlen(sDeviceInfo.m_szMountPoint), "Tuner设备不应该有挂载点" );
		}
		else
		{
			if (CSUDI_ISREMOVEABLE(pdwIds1[i]))
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "获得的设备号不是已知的设备类型" );
			}
			else if (sDeviceInfo.m_eDeviceType == EM_UDIFS_DEVTYPE_VOLUME)
			{
			    CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "本地分卷设备应该有挂载点");
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
//@DESCRIPTION: 获得不存在的设备对应的信息
//@PRECONDITION: 无
//@INPUT: dwDeviceId = 0x0001FFFF(假设0x0001FFFF为不存在的设备id)
//@INPUT: psDeviceInfo = &sDeviceInfo,其中CSUDIFSDeviceInfo_S sDeviceInfo
//@EXPECTATION: 返回值不等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIFSGetDeviceInfo(0x0001FFFF, &sDeviceInfo)，判断返回值	
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetDeviceInfo_0002(void)
{
	CSUDIFSDeviceInfo_S sDeviceInfo;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFSGetDeviceInfo(0x0001FFFF, &sDeviceInfo), "获取不存在的设备信息不应该成功\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDI_Error_Code CSUDIFSGetDeviceInfo(CSUDI_UINT32 dwDeviceId,CSUDIFSDeviceInfo_S * psDeviceInfo)
//@DESCRIPTION: 接收设备信息的指针非法
//@PRECONDITION: 系统中至少存在一个已识别的设备
//@INPUT: dwDeviceId = dwDeviceId1，dwDeviceId1是由CSUDIFSGetAllDeviceId()获得的设备号
//@INPUT: psDeviceInfo = NULL
//@EXPECTATION: 返回值不等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()获得一个已识别的设备dwDeviceId1	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo(dwDeviceId1, NULL)获取设备停息，判断返回值	
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetDeviceInfo_0003(void)
{
	CSUDI_UINT32   pdwIds1[32];
	int nActIdCnt = 0;
	int i = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(pdwIds1, 32, &nActIdCnt), "获取设备id失败\n");
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "一个设备也不存在，无法获得设备信息\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFSGetDeviceInfo(pdwIds1[i], NULL), "获取设备信息不应该成功\n");
	}

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetDeviceInfo(CSUDI_UINT32 dwDeviceId,CSUDIFSDeviceInfo_S * psDeviceInfo)
//@DESCRIPTION: 判断系统是否支持FAT格式
//@PRECONDITION: 
//@INPUT: dwDeviceId = dwVolumeId，dwVolumeId是由CSUDIFSGetAllDeviceId()获得的移动分卷设备号
//@INPUT: psDeviceInfo = &sDeviceInfo, 其中CSUDIFSDeviceInfo_S  sDeviceInfo
//@EXPECTATION: 返回值等于 CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、插入一个有且只有一个分区，且分区格式为FAT格式的盘
//@EXECUTIONFLOW: 2、调用CSUDIFSGetAllDeviceId()得到所有的设备，并从这些设备中找到移动分卷设备dwVolumeId，应该能找到
//@EXECUTIONFLOW: 3、调用CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo)，应该获取成功，且有挂载点
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetDeviceInfo_0004(void)
{
	CSUDI_UINT32  dwIds1[32];
	int  nActIdCnt = 0;
	CSUDI_UINT32 dwVolumeId = -1;
	int i = 0;
	CSUDIFSDeviceInfo_S sDeviceInfo;
	CSUDIFSStatFs_S sStatFs;

	CSFSPrint("请插入一个只有一个分区，且分区格式为FAT32的盘，插入且被认识后按Y确认\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，用例失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds1, 32, &nActIdCnt), "获取所有的设备失败\n");
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "一个设备也没有，失败\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			dwVolumeId = dwIds1[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo), "获取设备信息失败\n");
			CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "分卷设备应该有挂载点");
			memset(&sStatFs, 0x00, sizeof(CSUDIFSStatFs_S));
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStatfs(sDeviceInfo.m_szMountPoint, &sStatFs), "获取分卷信息失败");
			CSTK_ASSERT_TRUE_FATAL(EM_UDIFS_PARTITION_FAT32 == sStatFs.m_eFsType, "分区格式不为FAT32");
			return CSUDI_TRUE;
		}
	}

	CSFSPrint("找不到分卷设备\n");

	CSTK_FATAL_POINT
	{
		;
	}

	return CSUDI_FALSE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetDeviceInfo(CSUDI_UINT32 dwDeviceId,CSUDIFSDeviceInfo_S * psDeviceInfo)
//@DESCRIPTION: 判断系统是否支持NTFS格式
//@PRECONDITION: 
//@INPUT: dwDeviceId = dwVolumeId，dwVolumeId是由CSUDIFSGetAllDeviceId()获得的移动分卷设备号
//@INPUT: psDeviceInfo = &sDeviceInfo, 其中CSUDIFSDeviceInfo_S  sDeviceInfo
//@EXPECTATION: 返回值等于 CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、插入一个有且只有一个分区，且分区格式为NTFS格式的盘
//@EXECUTIONFLOW: 2、调用CSUDIFSGetAllDeviceId()得到所有的设备，并从这些设备中找到移动分卷设备dwVolumeId，应该能找到
//@EXECUTIONFLOW: 3、调用CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo)，应该获取成功，且有挂载点
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetDeviceInfo_0005(void)
{
	CSUDI_UINT32  dwIds1[32];
	int  nActIdCnt = 0;
	CSUDI_UINT32 dwVolumeId = -1;
	int i = 0;
	CSUDIFSDeviceInfo_S sDeviceInfo;
	CSUDIFSStatFs_S sStatFs;

	CSFSPrint("请插入一个只有一个分区，且分区格式为NTFS的盘，插入且被认识后按Y确认\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，用例失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds1, 32, &nActIdCnt), "获取所有的设备失败\n");
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "一个设备也没有，失败\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			dwVolumeId = dwIds1[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo), "获取设备信息失败\n");
			CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "分卷设备应该有挂载点");
			memset(&sStatFs, 0x00, sizeof(CSUDIFSStatFs_S));
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStatfs(sDeviceInfo.m_szMountPoint, &sStatFs), "获取分卷信息失败");
			CSTK_ASSERT_TRUE_FATAL(EM_UDIFS_PARTITION_NTFS == sStatFs.m_eFsType, "分区格式不为NTFS");
			return CSUDI_TRUE;
		}
	}

	CSFSPrint("找不到分卷设备\n");

	CSTK_FATAL_POINT
	{
		;
	}

	return CSUDI_FALSE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetDeviceInfo(CSUDI_UINT32 dwDeviceId,CSUDIFSDeviceInfo_S * psDeviceInfo)
//@DESCRIPTION: 判断系统是否支持EXT2格式
//@PRECONDITION: 
//@INPUT: dwDeviceId = dwVolumeId，dwVolumeId是由CSUDIFSGetAllDeviceId()获得的移动分卷设备号
//@INPUT: psDeviceInfo = &sDeviceInfo, 其中CSUDIFSDeviceInfo_S  sDeviceInfo
//@EXPECTATION: 返回值等于 CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、插入一个有且只有一个分区，且分区格式为EXT2格式的盘
//@EXECUTIONFLOW: 2、调用CSUDIFSGetAllDeviceId()得到所有的设备，并从这些设备中找到移动分卷设备dwVolumeId，应该能找到
//@EXECUTIONFLOW: 3、调用CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo)，应该获取成功，且有挂载点
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetDeviceInfo_0006(void)
{
	CSUDI_UINT32  dwIds1[32];
	int  nActIdCnt = 0;
	CSUDI_UINT32 dwVolumeId = -1;
	int i = 0;
	CSUDIFSDeviceInfo_S sDeviceInfo;
	CSUDIFSStatFs_S sStatFs;

	CSFSPrint("请插入一个只有一个分区，且分区格式为EXT2的盘，插入且被认识后按Y确认\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，用例失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds1, 32, &nActIdCnt), "获取所有的设备失败\n");
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "一个设备也没有，失败\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			dwVolumeId = dwIds1[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo), "获取设备信息失败\n");
			CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "分卷设备应该有挂载点");
			memset(&sStatFs, 0x00, sizeof(CSUDIFSStatFs_S));
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStatfs(sDeviceInfo.m_szMountPoint, &sStatFs), "获取分卷信息失败");
			CSTK_ASSERT_TRUE_FATAL(EM_UDIFS_PARTITION_EXT2 == sStatFs.m_eFsType, "分区格式不为EXT2");
			return CSUDI_TRUE;
		}
	}

	CSFSPrint("找不到分卷设备\n");

	CSTK_FATAL_POINT
	{
		;
	}

	return CSUDI_FALSE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetDeviceInfo(CSUDI_UINT32 dwDeviceId,CSUDIFSDeviceInfo_S * psDeviceInfo)
//@DESCRIPTION: 判断系统是否支持EXT3格式
//@PRECONDITION: 
//@INPUT: dwDeviceId = dwVolumeId，dwVolumeId是由CSUDIFSGetAllDeviceId()获得的移动分卷设备号
//@INPUT: psDeviceInfo = &sDeviceInfo, 其中CSUDIFSDeviceInfo_S  sDeviceInfo
//@EXPECTATION: 返回值等于 CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、插入一个有且只有一个分区，且分区格式为EXT3格式的盘
//@EXECUTIONFLOW: 2、调用CSUDIFSGetAllDeviceId()得到所有的设备，并从这些设备中找到移动分卷设备dwVolumeId，应该能找到
//@EXECUTIONFLOW: 3、调用CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo)，应该获取成功，且有挂载点
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetDeviceInfo_0007(void)
{
	CSUDI_UINT32  dwIds1[32];
	int  nActIdCnt = 0;
	CSUDI_UINT32 dwVolumeId = -1;
	int i = 0;
	CSUDIFSDeviceInfo_S sDeviceInfo;
	CSUDIFSStatFs_S sStatFs;

	CSFSPrint("请插入一个只有一个分区，且分区格式为EXT3的盘，插入且被认识后按Y确认\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，用例失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds1, 32, &nActIdCnt), "获取所有的设备失败\n");
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "一个设备也没有，失败\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			dwVolumeId = dwIds1[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo), "获取设备信息失败\n");
			CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "分卷设备应该有挂载点");
			memset(&sStatFs, 0x00, sizeof(CSUDIFSStatFs_S));
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStatfs(sDeviceInfo.m_szMountPoint, &sStatFs), "获取分卷信息失败");
			CSTK_ASSERT_TRUE_FATAL(EM_UDIFS_PARTITION_EXT3 == sStatFs.m_eFsType, "分区格式不为EXT3");
			return CSUDI_TRUE;
		}
	}

	CSFSPrint("找不到分卷设备\n");

	CSTK_FATAL_POINT
	{
		;
	}

	return CSUDI_FALSE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetDeviceInfo(CSUDI_UINT32 dwDeviceId,CSUDIFSDeviceInfo_S * psDeviceInfo)
//@DESCRIPTION: 判断系统是否支持JFFS2格式
//@PRECONDITION: 
//@INPUT: dwDeviceId = dwVolumeId，dwVolumeId是由CSUDIFSGetAllDeviceId()获得的移动分卷设备号
//@INPUT: psDeviceInfo = &sDeviceInfo, 其中CSUDIFSDeviceInfo_S  sDeviceInfo
//@EXPECTATION: 返回值等于 CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、插入一个有且只有一个分区，且分区格式为JFFS2格式的盘
//@EXECUTIONFLOW: 2、调用CSUDIFSGetAllDeviceId()得到所有的设备，并从这些设备中找到移动分卷设备dwVolumeId，应该能找到
//@EXECUTIONFLOW: 3、调用CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo)，应该获取成功，且有挂载点
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetDeviceInfo_0008(void)
{
	CSUDI_UINT32  dwIds1[32];
	int  nActIdCnt = 0;
	CSUDI_UINT32 dwVolumeId = -1;
	int i = 0;
	CSUDIFSDeviceInfo_S sDeviceInfo;
	CSUDIFSStatFs_S sStatFs;

	CSFSPrint("请插入一个只有一个分区，且分区格式为JFFS2的盘，插入且被认识后按Y确认\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，用例失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds1, 32, &nActIdCnt), "获取所有的设备失败\n");
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "一个设备也没有，失败\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			dwVolumeId = dwIds1[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo), "获取设备信息失败\n");
			CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "分卷设备应该有挂载点");
			memset(&sStatFs, 0x00, sizeof(CSUDIFSStatFs_S));
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStatfs(sDeviceInfo.m_szMountPoint, &sStatFs), "获取分卷信息失败");
			CSTK_ASSERT_TRUE_FATAL(EM_UDIFS_PARTITION_JFFS2 == sStatFs.m_eFsType, "分区格式不为JFFS2");
			return CSUDI_TRUE;
		}
	}

	CSFSPrint("找不到分卷设备\n");

	CSTK_FATAL_POINT
	{
		;
	}

	return CSUDI_FALSE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetDeviceInfo(CSUDI_UINT32 dwDeviceId,CSUDIFSDeviceInfo_S * psDeviceInfo)
//@DESCRIPTION: 获取一个可写的固定分卷
//@PRECONDITION: 
//@INPUT: dwDeviceId = dwVolumeId，dwVolumeId是由CSUDIFSGetAllDeviceId()获得的固定分卷设备号
//@INPUT: psDeviceInfo = &sDeviceInfo, 其中CSUDIFSDeviceInfo_S  sDeviceInfo
//@EXPECTATION: 能找到一个可写的固定分卷设备
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到所有的设备，并从这些设备中找到固定分卷设备dwVolumeId，应该能找到
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo)，应该获取成功，且有挂载点
//@EXECUTIONFLOW: 3、调用CSUDIFSStatfs(分区挂载点, &sStatFs)获取文件系统信息，应该获取成功
//@EXECUTIONFLOW: 4、根据文件系统的类型的取值范围是否在EM_UDIFS_PARTITION_RW_START到EM_UDIFS_PARTITION_RW_END之间，若是则获取成功
//@EXECUTIONFLOW: 5、循环1到4步，若一个也找不到则失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetDeviceInfo_0009(void)
{
	CSUDI_UINT32  dwIds1[32];
	int  nActIdCnt = 0;
	CSUDI_UINT32 dwVolumeId = -1;
	int i = 0;
	CSUDIFSDeviceInfo_S sDeviceInfo;
	CSUDIFSStatFs_S  sStatFs;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds1, 32, &nActIdCnt), "获取所有的设备失败\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "一个设备也没有，失败\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) != CSUDI_RDI_MAJOR_DEVICEID_MASK)
		{
			dwVolumeId = dwIds1[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo), "获取设备信息失败\n");
			if(sDeviceInfo.m_eDeviceType == EM_UDIFS_DEVTYPE_VOLUME)
			{
				CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "分卷设备应该有挂载点\n");
				CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStatfs(sDeviceInfo.m_szMountPoint, &sStatFs), "获取文件系统信息失败\n");

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
	CSFSPrint("找不到一个可写的固定分卷\n");
	return CSUDI_FALSE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetDeviceInfo(CSUDI_UINT32 dwDeviceId,CSUDIFSDeviceInfo_S * psDeviceInfo)
//@DESCRIPTION: 获取一个只读的固定分卷
//@PRECONDITION: 
//@INPUT: dwDeviceId = dwVolumeId，dwVolumeId是由CSUDIFSGetAllDeviceId()获得的固定分卷设备号
//@INPUT: psDeviceInfo = &sDeviceInfo, 其中CSUDIFSDeviceInfo_S  sDeviceInfo
//@EXPECTATION: 能找到一个可读的固定分卷设备
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到所有的设备，并从这些设备中找到固定分卷设备dwVolumeId，应该能找到
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo)，应该获取成功，且有挂载点
//@EXECUTIONFLOW: 3、调用CSUDIFSStatfs(分区挂载点, &sStatFs)获取文件系统信息，应该获取成功
//@EXECUTIONFLOW: 4、根据文件系统的类型的取值范围是否在EM_UDIFS_PARTITION_READONLY_START到EM_UDIFS_PARTITION_READONLY_END之间，若是则获取成功
//@EXECUTIONFLOW: 5、循环1到4步，若一个也找不到则失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetDeviceInfo_0010(void)
{
	CSUDI_UINT32  dwIds1[32];
	int  nActIdCnt = 0;
	CSUDI_UINT32 dwVolumeId = -1;
	int i = 0;
	CSUDIFSDeviceInfo_S sDeviceInfo;
	CSUDIFSStatFs_S  sStatFs;
	
	CSFSPrint("请插入一个至少有一个只读分卷的设备，插入且被认识后按Y确认\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，用例失败");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds1, 32, &nActIdCnt), "获取所有的设备失败\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "一个设备也没有，失败\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) != CSUDI_RDI_MAJOR_DEVICEID_MASK)
		{
			dwVolumeId = dwIds1[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo), "获取设备信息失败\n");
			if(sDeviceInfo.m_eDeviceType == EM_UDIFS_DEVTYPE_VOLUME)
			{
				CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "分卷设备应该有挂载点\n");
				CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStatfs(sDeviceInfo.m_szMountPoint, &sStatFs), "获取文件系统信息失败\n");

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
	CSFSPrint("找不到一个只读的固定分卷\n");
	return CSUDI_FALSE;
}


//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetDeviceInfo(CSUDI_UINT32 dwDeviceId,CSUDIFSDeviceInfo_S * psDeviceInfo)
//@DESCRIPTION: UDI FS 要至少有两个分卷设备,一个只读,一个可写,并且有父存储设备
//@PRECONDITION: 
//@INPUT: dwDeviceId = dwVolumeId，dwVolumeId是由CSUDIFSGetAllDeviceId()获得的固定分卷设备号
//@INPUT: psDeviceInfo = &sDeviceInfo, 其中CSUDIFSDeviceInfo_S  sDeviceInfo
//@EXPECTATION: UDI FS 要至少有两个分卷设备,一个只读,一个可写,并且有父存储设备
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到所有的设备，并从这些设备中找到固定分卷设备dwVolumeId，应该能找到
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo)，应该获取成功，且有挂载点
//@EXECUTIONFLOW: 3、若当前是分卷设备主设备,则sDeviceInfo.m_dwParentDeviceId为父存储设备ID
//@EXECUTIONFLOW: 4、调用CSUDIFSStatfs(分区挂载点, &sStatFs)获取文件系统信息，应该获取成功
//@EXECUTIONFLOW: 5、根据文件系统的类型的取值范围是否在EM_UDIFS_PARTITION_READONLY_START到EM_UDIFS_PARTITION_READONLY_END之间，若是则获取成功
//@EXECUTIONFLOW: 6、根据文件系统的类型的取值范围是否在EM_UDIFS_PARTITION_RW_START到EM_UDIFS_PARTITION_RW_END之间，若是则获取成功
//@EXECUTIONFLOW: 7、循环1到6步，若找到至少一个只读分卷设备,至少一个可写分卷设备,并且有父存诸设备则成功
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

	CSFSPrint("请插入一个至少有两个分卷设备,一个只读,一个可写,并且有父存储设备的移动设备，插入且被认识后按Y确认\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，用例失败");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds1, 32, &nActIdCnt), "获取所有的设备失败\n");
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "一个设备也没有，失败\n");

	CSTCPrint("the total is %d \n",nActIdCnt);

	for(i = 0; i < nActIdCnt; i++)
	{
		CSTCPrint("the dwIds1[%d] is 0x%x \n",i,dwIds1[i]);
	
		if((dwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwIds1[i], &sDeviceInfo), "获取设备信息失败\n");
			if(nStorageId == -1)
			{
				nStorageId = sDeviceInfo.m_dwParentDeviceId;
			}

			if((int)sDeviceInfo.m_dwParentDeviceId == nStorageId)
			{
				//CSFSPrint("分区id = %d, 挂载路径 = %s, 父存储设备id = %d\n", dwIds1[i], sDeviceInfo.m_szMountPoint, nStorageId);
				//nCount++;
			}
			else
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "发现多个移动存储设备，应该只有一个\n");
			}

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwIds1[i], &sDeviceInfo), "获取设备信息失败\n");
			CSTCPrint("sDeviceInfo.m_eDeviceType = 0x%x\n",sDeviceInfo.m_eDeviceType);
			CSTCPrint("sDeviceInfo.m_szMountPoint = %s\n",sDeviceInfo.m_szMountPoint);
			
			CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "分卷设备应该有挂载点\n");
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStatfs(sDeviceInfo.m_szMountPoint, &sStatFs), "获取文件系统信息失败\n");

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
	CSTK_ASSERT_TRUE_FATAL(nStorageId >= 0 , "获取父存储设备失败\n");
	CSTK_ASSERT_TRUE_FATAL(nReadCount >= 1, "获取只读分卷设备失败\n");
	CSTK_ASSERT_TRUE_FATAL(nWriteCount >= 1 , "获取可写分卷设备失败\n");
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
//@DESCRIPTION: 使一个已存在的存储设备进入待机状态
//@PRECONDITION: 系统中至少存在一个已识别的设备
//@INPUT: dwDeviceId = dwDeviceId1, 其中dwDeviceId1是由CSUDIFSGetAllDeviceId()获得的存储设备
//@EXPECTATION: 返回值等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已识别的存储设备dwDeviceId1	
//@EXECUTIONFLOW: 2、调用CSUDIFSStandby(dwDeviceId1)使设备待机，若返回值不为CSUDI_SUCCESS则失败
//@EXECUTIONFLOW: 3、调用CSUDIFSWakeup(dwDeviceId1)唤醒设备，恢复环境
CSUDI_BOOL CSTC_FS_IT_CSUDIFSStandby_0001(void)
{
	CSUDI_UINT32  dwIds1[32];
	int  nActIdCnt = 0;
	int dwDeviceId1 = -1;
	int i = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds1, 32, &nActIdCnt), "获取所有的设备失败\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "一个设备也没有，无法待机\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_STORAGE_MAJOR_DEVICEID)
		{
			dwDeviceId1 = (int)dwIds1[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSStandby(dwDeviceId1), "待机存储设备失败\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSWakeup(dwDeviceId1), "待机后唤醒存储设备失败\n");	
			return CSUDI_TRUE;
		}
	}

	CSTK_ASSERT_TRUE(dwDeviceId1 != -1, "找不到一个存储设备，无法待机\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_FALSE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSStandby(CSUDI_UINT32 dwDeviceId)
//@DESCRIPTION: 使一个不存在的存储设备进入待机状态
//@PRECONDITION: 无
//@INPUT: dwDeviceId = 0x7FFFFFFF(假设0x7FFFFFFF为不存在的设备)
//@EXPECTATION: 返回值不等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIFSStandby(0x7FFFFFFF)待机存储设备	
CSUDI_BOOL CSTC_FS_IT_CSUDIFSStandby_0002(void)
{
	CSTK_ASSERT_TRUE(CSUDI_SUCCESS != CSUDIFSStandby(0x7FFFFFFF), "待机不存在的设备不应该成功\n");
	return CSUDI_TRUE;
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_FALSE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSStandby(CSUDI_UINT32 dwDeviceId)
//@DESCRIPTION: 使一个分卷设备进入待机状态
//@PRECONDITION: 系统中至少存在一个已识别的分卷设备
//@INPUT: dwDeviceId = dwVolumeId, 其中dwVolumeId是由CSUDIFSGetAllDeviceId()获得的分卷设备
//@EXPECTATION: 返回值不等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到所有的设备，并根据主设备号找到一个分卷设备dwVolumeId，若找不到分卷设备则测试失败
//@EXECUTIONFLOW: 2、调用CSUDIFSStandby(dwVolumeId)使分卷设备待机，判断返回值	
CSUDI_BOOL CSTC_FS_IT_CSUDIFSStandby_0003(void)
{
	CSUDI_UINT32  dwIds1[32];
	int  nActIdCnt = 0;
	int dwVolumeId = -1;
	int i = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds1, 32, &nActIdCnt), "获取所有的设备失败\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "一个设备也没有，无法待机\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			dwVolumeId = (int)dwIds1[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFSStandby(dwVolumeId), "待机分卷设备不应该成功\n");
			return CSUDI_TRUE;
		}
	}

	CSTK_ASSERT_TRUE_FATAL(dwVolumeId != -1, "找不到一个分卷设备，无法待机\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_FALSE;	
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSStandby(CSUDI_UINT32 dwDeviceId)
//@DESCRIPTION: 使一个已待机的存储设备进入待机状态
//@PRECONDITION: 系统中已有一个存储设备待机
//@INPUT: dwDeviceId = dwDeviceId1, 其中dwDeviceId1是由CSUDIFSGetAllDeviceId()获得的存储设备id
//@EXPECTATION: 返回值等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已识别的存储设备dwDeviceId1
//@EXECUTIONFLOW: 2、调用CSUDIFSStandby(dwDeviceId1)使存储设备dwDeviceId1待机
//@EXECUTIONFLOW: 3、调用CSUDIFSStandby(dwDeviceId1)再次使存储设备dwDeviceId1待机
//@EXECUTIONFLOW: 4、调用CSUDIFSWakeup(dwDeviceId1)唤醒设备，恢复环境
CSUDI_BOOL CSTC_FS_IT_CSUDIFSStandby_0004(void)
{
	CSUDI_UINT32  dwIds1[32];
	int  nActIdCnt = 0;
	int dwDeviceId1 = -1;
	int i = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds1, 32, &nActIdCnt), "获取所有的设备失败\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "一个设备也没有，无法待机\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_STORAGE_MAJOR_DEVICEID)
		{
			dwDeviceId1 = (int)dwIds1[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSStandby(dwDeviceId1), "待机存储设备失败\n");

			CSTK_ASSERT_TRUE(CSUDI_SUCCESS == CSUDIFSStandby(dwDeviceId1), "待机已待机的存储设备失败\n");
			
			CSTK_ASSERT_TRUE(CSUDI_SUCCESS == CSUDIFSWakeup(dwDeviceId1), "待机后唤醒存储设备失败\n");	
			
			return CSUDI_TRUE;
		}
	}
	
	CSTK_ASSERT_TRUE(dwDeviceId1 != -1, "找不到一个存储设备，无法待机\n");
	
	CSTK_FATAL_POINT
	{
		;
	}
	return  CSUDI_FALSE;
}
/****************************CSUDIFSStandby  End *******************************/

/****************************CSUDIFSWakeup  Start ******************************/ 
//@CASEGROUP: CSUDI_Error_Code CSUDIFSWakeup(CSUDI_UINT32 dwDeviceId) 
//@DESCRIPTION: 唤醒一个处于待机状态的存储设备
//@PRECONDITION: 系统中至少存在一个存储设备
//@INPUT: dwDeviceId为一个处于待机状态的存储设备的dwDeviceId1，由CSUDIFSGetAllDeviceId()获得
//@EXPECTATION: 返回值等于 CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已识别的存储设备dwDeviceId1
//@EXECUTIONFLOW: 2、调用CSUDIFSStandby(dwDeviceId1)使设备待机
//@EXECUTIONFLOW: 3、调用CSUDIFSWakeup(dwDeviceId1)唤醒设备，判断返回值
CSUDI_BOOL CSTC_FS_IT_CSUDIFSWakeup_0001(void)
{
	CSUDI_UINT32  dwIds1[32];
	int  nActIdCnt = 0;
	int dwDeviceId1 = -1;
	int i = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds1, 32, &nActIdCnt), "获取所有的设备失败\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "一个设备也没有\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_STORAGE_MAJOR_DEVICEID)
		{
			dwDeviceId1 = (int)dwIds1[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSStandby(dwDeviceId1), "待机存储设备失败\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSWakeup(dwDeviceId1), "唤醒存储设备失败\n");	
			return CSUDI_TRUE;
		}
	}

	CSTK_ASSERT_TRUE(dwDeviceId1 != -1, "找不到一个存储设备，无法唤醒\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_FALSE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSWakeup(CSUDI_UINT32 dwDeviceId) 
//@DESCRIPTION: 唤醒一个不存在的存储设备
//@PRECONDITION: 无
//@INPUT: dwDeviceId = 0x7FFFFFFF(假设0x7FFFFFFF为不存在的设备)
//@EXPECTATION: 返回值不等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIFSWakeup(0x7FFFFFFF)唤醒设备，判断返回值	
CSUDI_BOOL CSTC_FS_IT_CSUDIFSWakeup_0002(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFSWakeup(0x7FFFFFFF), "唤醒不存在的设备不应该成功\n");

	return CSUDI_TRUE;
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_FALSE;		
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSWakeup(CSUDI_UINT32 dwDeviceId) 
//@DESCRIPTION: 唤醒一个分卷设备
//@PRECONDITION: 系统中至少存在一个已识别的分卷设备
//@INPUT: dwDeviceId = dwVolumeId，dwVolumeId是由CSUDIFSGetAllDeviceId()获得的分卷设备
//@EXPECTATION: 返回值不等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已识别的分卷设备dwVolumeId
//@EXECUTIONFLOW: 2、调用CSUDIFSWakeup(dwVolumeId)唤醒分卷设备	
CSUDI_BOOL CSTC_FS_IT_CSUDIFSWakeup_0003(void)
{
	CSUDI_UINT32  dwIds1[32];
	int  nActIdCnt = 0;
	int dwVolumeId = -1;
	int i = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds1, 32, &nActIdCnt), "获取所有的设备失败\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "一个设备也没有，无法唤醒\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			dwVolumeId = (int)dwIds1[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFSWakeup(dwVolumeId), "唤醒分卷设备不应该成功\n");	
			return CSUDI_TRUE;
		}
	}

	CSTK_ASSERT_TRUE(dwVolumeId != -1, "找不到一个分卷设备，无法唤醒\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_FALSE;	
}

//@CASEGROUP: CSUDIFSWakeup 
//@DESCRIPTION: 唤醒一个未待机的存储设备
//@PRECONDITION: 系统中已有一个存储设备未待机
//@INPUT: dwDeviceId = dwStorageId，其中dwStorageId由CSUDIFSGetAllDeviceId()获得
//@EXPECTATION: 返回值等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已识别的存储设备dwStorageId
//@EXECUTIONFLOW: 2、调用CSUDIFSWakeup(dwStorageId)唤醒存储设备
CSUDI_BOOL CSTC_FS_IT_CSUDIFSWakeup_0004(void)
{
	CSUDI_UINT32  dwIds1[32];
	int  nActIdCnt = 0;
	int dwStorageId = -1;
	int i = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds1, 32, &nActIdCnt), "获取所有的设备失败\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "一个设备也没有，无法唤醒\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds1[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_STORAGE_MAJOR_DEVICEID)
		{
			dwStorageId = (int)dwIds1[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSWakeup(dwStorageId), "唤醒存储设备失败\n");	
			return CSUDI_TRUE;
		}
	}

	CSTK_ASSERT_TRUE(dwStorageId != -1, "找不到一个存储设备，无法唤醒\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_FALSE;		
}
/****************************CSUDIFSWakeup  End *******************************/

/****************************CSUDIFSFormat  Start ******************************/ 
//@CASEGROUP: CSUDI_Error_Code CSUDIFSFormat(CSUDI_UINT32 dwDeviceId,CSUDIFSPartitionType_E eType)
//@DESCRIPTION: 格式化一个已识别的分卷
//@PRECONDITION: 1、系统中至少存在一个已识别的分卷
//@PRECONDITION: 2、平台支持格式化成EM_STRG_PARTITION_FAT32
//@INPUT: dwDeviceId = dwVolumeId，dwVolume是由CSUDIFSGetAllDeviceId()获得的分卷设备
//@INPUT: eType = EM_UDIFS_PARTITION_FAT32
//@EXPECTATION: 格式化成功，格式化前的文件不复存在
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已识别的分卷设备dwVolumeId
//@EXECUTIONFLOW: 2、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSFormat_0001", "w")创建并打开文件，返回句柄hFile，句柄不为空
//@EXECUTIONFLOW: 3、调用CSUDIFSClose(hFile)关闭该文件，返回值应当为0
//@EXECUTIONFLOW: 4、调用CSUDIFSFormat(dwVolumeId, EM_UDIFS_PARTITION_FAT32)格式化分卷设备
//@EXECUTIONFLOW: 5、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSFormat_0001", "r")以只读方式打开格式化前创建的文件，应当失败，返回CSUDI_NULL
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFormat_0001(void)
{
	//手动测试
	CSUDI_UINT32   dwIds[32];
	int nActIdCnt = 0;
	int i = 0;
	int dwVolumeId = 0;
	CSUDI_HANDLE  hHandle = CSUDI_NULL;
	char szFilePath[CSUDIFS_MAX_FILE_PATH];
	CSUDIFSDeviceInfo_S   sDeviceInfo;
	int  nLen = 0;

	CSFSPrint("本测试用例会对插入设备进行格式化，确认继续请按(Y)/否则请按(N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "选择了退出");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt), "获取设备id失败\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "一个设备也不存在，无法获得设备信息\n");
	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			dwVolumeId = (int)dwIds[i];

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo), "获取设备信息失败\n");        
			CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "分卷挂载点为空\n");
			CSTK_ASSERT_TRUE_FATAL(sDeviceInfo.m_szMountPoint != CSUDI_NULL, "无分卷挂载点\n");
			
			memset(szFilePath, 0, sizeof(szFilePath));
			nLen = strlen(sDeviceInfo.m_szMountPoint);
			memcpy(szFilePath, sDeviceInfo.m_szMountPoint, nLen);

			if(szFilePath[nLen -1] != '/') szFilePath[nLen] = '/';

			strcat(szFilePath, "CSUDIFSFormat_0001");
			
			CSTK_ASSERT_TRUE_FATAL((hHandle = CSUDIFSOpen(szFilePath, "w")) != CSUDI_NULL, "创建文件失败\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDIFSClose(hHandle) == 0, "关闭文件失败\n");

			hHandle = CSUDI_NULL;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSFormat(dwVolumeId, EM_UDIFS_PARTITION_FAT32), "格式化分卷失败\n");
			CSTK_ASSERT_TRUE_FATAL((hHandle = CSUDIFSOpen(szFilePath, "r")) == CSUDI_NULL, "打开格式化前的文件应该失败\n");
				
			return CSUDI_TRUE;
		}
	}

	CSTK_ASSERT_TRUE_FATAL(dwVolumeId != -1, "找不到一个分卷设备\n");
	
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
//@DESCRIPTION: 格式化只有一个分区的存储设备
//@PRECONDITION: 1、系统中至少存在一个已识别的存储设备，且该存储设备至少有一个已格式化的分区
//@PRECONDITION: 2、平台支持格式化成EM_UDIFS_PARTITION_EXT3
//@INPUT: dwDeviceId = dwStorageId，dwStorageId是由CSUDIFSGetAllDeviceId()获得的存储设备
//@INPUT: eType = EM_UDIFS_PARTITION_EXT3
//@EXPECTATION: 格式化成功，且在已注册的回调中收到EM_UDIRDI_EVENT_PLUGOUT和EM_UDIRDI_EVENT_FOUND事件
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已识别的存储设备dwStorageId
//@EXECUTIONFLOW: 2、调用CSUDIFSRDIAddCallback(CSUDIFSFormat_0002_Callback, CSUDI_NULL)注册回调函数，应当返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIFSFormat(dwStorageId, EM_UDIFS_PARTITION_EXT3)格式化存储设备，应当返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、检查回调函数CSUDIFSFormat_0002_Callback中是否有EM_UDIRDI_EVENT_PLUGOUT和EM_UDIRDI_EVENT_FOUND事件
//@EXECUTIONFLOW: 5、调用CSUDIFSRDIRemoveCallback(CSUDIFSFormat_0002_Callback, CSUDI_NULL)删除回调函数，恢复环境
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFormat_0002(void)
{
	//手动测试
	CSUDI_UINT32   dwIds[32];
	int nActIdCnt = 0;
	int i =0;
	CSUDI_UINT32  dwStorageId = 0;
	CSUDI_Error_Code nErrorCode = CSUDI_SUCCESS;

	CSFSPrint("本测试用例会对插入设备进行格式化，确认继续请按(Y)/否则请按(N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "选择了退出");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCode = CSUDIFSRDIAddCallback(CSUDIFSFormat_0002_Callback,
		CSUDI_NULL)),"添加回调失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt), "获取设备id失败\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "一个设备也不存在，无法获得设备信息\n");

	g_eCSUDIFSFormat_0002 = EM_UDIRDI_EVENT_UNKNOWN;

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_STORAGE_MAJOR_DEVICEID)
		{
			dwStorageId = dwIds[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSFormat(dwStorageId, EM_UDIFS_PARTITION_EXT3), "格式化存储设备失败\n");
            break;
        }
	}

	CSTK_ASSERT_TRUE_FATAL(g_eCSUDIFSFormat_0002 == EM_UDIRDI_EVENT_FOUND, "回调函数中没有收到格式化事件\n");

	CSTK_FATAL_POINT
	{
		if(nErrorCode == CSUDI_SUCCESS)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(CSUDIFSFormat_0002_Callback, 
			CSUDI_NULL), "移除回调失败\n");		
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
//@DESCRIPTION: 格式化一个有两个分区的存储设备
//@PRECONDITION: 1、系统中至少存在一个已识别的存储设备，且该存储设备两个分区
//@PRECONDITION: 2、平台支持格式化成EM_UDIFS_PARTITION_EXT3
//@INPUT: dwDeviceId = dwStorageId，dwStorageId是由CSUDIFSGetAllDeviceId()获得的存储设备
//@INPUT: eType = EM_UDIFS_PARTITION_EXT3
//@EXPECTATION: 格式化成功，且在已注册的回调中收到两次EM_UDIRDI_EVENT_PLUGOUT事件和一个EM_UDIRDI_EVENT_FOUND事件
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已识别的分卷设备dwStorageId
//@EXECUTIONFLOW: 2、调用CSUDIFSRDIAddCallback(CSUDIFSFormat_0002_Callback, CSUDI_NULL)注册回调函数，应当返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIFSFormat(dwStorageId, EM_UDIFS_PARTITION_EXT3)格式化存储设备，应当返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、检查回调函数CSUDIFSFormat_0002_Callback中是否两次EM_UDIRDI_EVENT_PLUGOUT事件和一次EM_UDIRDI_EVENT_FOUND事件
//@EXECUTIONFLOW: 5、调用CSUDIFSRDIRemoveCallback(CSUDIFSFormat_0002_Callback, CSUDI_NULL)删除回调函数，恢复环境
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFormat_0003(void)
{
	//手动测试
	CSUDI_UINT32   dwIds[32];
	int nActIdCnt = -1;
	int i =0;
	CSUDI_UINT32  dwStorageId = 0;
	CSUDI_Error_Code nErrorCode = CSUDI_SUCCESS;

	CSFSPrint("本测试用例会对插入设备进行格式化，确认继续请按(Y)/否则请按(N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "选择了退出");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCode = CSUDIFSRDIAddCallback(CSUDIFSFormat_0003_Callback,
		CSUDI_NULL)),"添加回调失败\n");

	CSFSPrint("请确认U盘是否有两个分区，有请按(Y)/没有请按(N)\n");

	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"没有两个分区\n");


	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt), "获取设备id失败\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "一个设备也不存在，无法获得设备信息\n");
	g_eCSUDIFSFormat_0003A = 0;
	g_eCSUDIFSFormat_0003B = 0;

	
	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_STORAGE_MAJOR_DEVICEID)
		{
			dwStorageId = dwIds[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSFormat(dwStorageId, EM_UDIFS_PARTITION_EXT3), "格式化存储设备失败\n");
			break;
		}
	}

	CSTK_ASSERT_TRUE_FATAL(g_eCSUDIFSFormat_0003A == 2, "分区拔除的消息个数不对\n");

	CSTK_ASSERT_TRUE_FATAL(g_eCSUDIFSFormat_0003B == 2, "发现分区的消息个数不对\n");

	CSTK_FATAL_POINT
	{
		if(nErrorCode == CSUDI_SUCCESS)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(CSUDIFSFormat_0003_Callback, 
			CSUDI_NULL), "移除回调失败\n");		
		}
	}
	
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSFormat(CSUDI_UINT32 dwDeviceId,CSUDIFSPartitionType_E eType)
//@DESCRIPTION: 格式化一个不存在的设备，包括分卷设备和存储设备
//@PRECONDITION: 无
//@INPUT: dwDeviceId = 0x0001FFFF(假设0x0001FFFF为不存在的设备id)
//@INPUT: eType = EM_UDIFS_PARTITION_EXT2
//@EXPECTATION: 格式化失败，返回值不等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIFSFormat(0x0001FFFF, EM_UDIFS_PARTITION_EXT2)格式化设备，返回值应当不等于CSUDI_SUCCESS
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFormat_0004(void)
{
	CSTK_ASSERT_TRUE(CSUDI_SUCCESS != CSUDIFSFormat(0x0001FFFF, EM_UDIFS_PARTITION_EXT2), "格式化不存在的设备不应该成功\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSFormat(CSUDI_UINT32 dwDeviceId,CSUDIFSPartitionType_E eType)
//@DESCRIPTION: 格式化一个正在使用的分卷设备
//@PRECONDITION: 1、系统中至少存在一个已识别的分卷
//@PRECONDITION: 2、平台支持格式化成EM_STRG_PARTITION_FAT32
//@INPUT: dwDeviceId = dwVolumeId，dwVolume是由CSUDIFSGetAllDeviceId()获得的分卷设备
//@INPUT: eType = EM_UDIFS_PARTITION_FAT32
//@EXPECTATION: 格式化失败，返回值不等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已识别的分卷设备dwVolumeId
//@EXECUTIONFLOW: 2、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSFormat_0005", "w")创建并打开文件，返回句柄hFile，句柄应当不为空
//@EXECUTIONFLOW: 3、调用CSUDIFSFormat(dwVolumeId, EM_UDIFS_PARTITION_FAT32)格式化分卷设备，返回值应当不等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、调用CSUDIFSClose(hFile)关闭该文件，返回值应当为0
//@EXECUTIONFLOW: 5、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSFormat_0005")删除该文件，返回值应当为0
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFormat_0005(void)
{
	//手动测试
	CSUDI_UINT32   dwIds[32];
	int nActIdCnt = 0;
	int i = 0;
	CSUDI_UINT32 dwVolumeId = 0;
	CSUDI_HANDLE  hHandle = CSUDI_NULL;
	char szFilePath[CSUDIFS_MAX_FILE_PATH];
	CSUDIFSDeviceInfo_S   sDeviceInfo;
	int  nLen = 0;

	CSFSPrint("本测试用例会对插入设备进行格式化，确认继续请按(Y)/否则请按(N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "选择了退出");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt), "获取设备id失败\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "一个设备也不存在，无法获得设备信息\n");
	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			dwVolumeId = dwIds[i];

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo), "获取设备信息失败\n");

			CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "分卷挂载点为空\n");

			CSTK_ASSERT_TRUE_FATAL(sDeviceInfo.m_szMountPoint != CSUDI_NULL, "无分卷挂载点\n");
			
			memset(szFilePath, 0, sizeof(szFilePath));

			nLen = strlen(sDeviceInfo.m_szMountPoint);
			
			memcpy(szFilePath, sDeviceInfo.m_szMountPoint, nLen);

			if(szFilePath[nLen -1] != '/') szFilePath[nLen] = '/';

			strcat(szFilePath, "CSUDIFSFormat_0005");
			
			CSTK_ASSERT_TRUE_FATAL((hHandle = CSUDIFSOpen(szFilePath, "w")) != CSUDI_NULL, "创建文件失败\n");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFSFormat(dwVolumeId, EM_UDIFS_PARTITION_FAT32), "格式化分卷正在使用的分卷不应当成功\n");
			break;
		}
	}

	CSTK_ASSERT_TRUE_FATAL(dwVolumeId != -1, "找不到一个分卷设备\n");
	
	CSTK_FATAL_POINT
	{
		if(hHandle != CSUDI_NULL)
		{	
			CSTK_ASSERT_TRUE_FATAL(CSUDIFSClose(hHandle) == 0, "关闭文件失败\n");
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRemove(szFilePath), "删除文件失败\n");
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
//@DESCRIPTION: 格式化一个分卷设备10次
//@PRECONDITION: 1、系统中至少存在一个已识别的存储设备
//@PRECONDITION: 2、平台支持格式化成EM_UDIFS_PARTITION_EXT3
//@INPUT: dwDeviceId = dwStorageId，dwStorageId是由CSUDIFSGetAllDeviceId()获得的存储设备
//@INPUT: eType = EM_UDIFS_PARTITION_EXT3
//@EXPECTATION: 格式化成功，且在已注册的回调中收到10次EM_UDIRDI_EVENT_PLUGOUT事件和10次EM_UDIRDI_EVENT_FOUND事件
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已识别的分卷设备dwVolumeId
//@EXECUTIONFLOW: 2、调用CSUDIFSRDIAddCallback(CSUDIFSFormat_0006_Callback, CSUDI_NULL)注册回调函数，应当返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、反复调用CSUDIFSFormat(dwVolumeId, EM_UDIFS_PARTITION_EXT3)格式化10存储设备，应当返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、检查回调函数CSUDIFSFormat_0002_Callback中是否10次EM_UDIRDI_EVENT_PLUGOUT事件和10次EM_UDIRDI_EVENT_FOUND事件
//@EXECUTIONFLOW: 5、调用CSUDIFSRDIRemoveCallback(CSUDIFSFormat_0006_Callback, CSUDI_NULL)删除回调函数，恢复环境
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFormat_0006(void)
{
	//手动测试
	CSUDI_UINT32   dwIds[32];
	int nActIdCnt = 0;
	int i =0;
	CSUDI_UINT32  dwVolumeId = 0;
	CSUDI_Error_Code nErrorCode = CSUDI_SUCCESS;
	int   nFormatCount = 10;

	CSFSPrint("本测试用例会对插入设备进行格式化，确认继续请按(Y)/否则请按(N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "选择了退出");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCode = CSUDIFSRDIAddCallback(CSUDIFSFormat_0006_Callback,
		CSUDI_NULL)),"添加回调失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt), "获取设备id失败\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "一个设备也不存在，无法获得设备信息\n");

	g_eCSUDIFSFormat_0006A = 0;
	g_eCSUDIFSFormat_0006B = 0;

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			dwVolumeId = dwIds[i];
			while(nFormatCount-- > 0)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSFormat(dwVolumeId, EM_UDIFS_PARTITION_EXT3), "格式化存储设备失败\n");
				CSTCPrint("第%d次格式成功\n", 10 - nFormatCount);
			}
			break;
		}
	}

	CSTK_ASSERT_TRUE_FATAL(g_eCSUDIFSFormat_0006A  == 10, "分区拔除的消息个数不对\n");
	CSTK_ASSERT_TRUE_FATAL(g_eCSUDIFSFormat_0006B  == 10, "发现分区的消息个数不对\n");

	CSTK_FATAL_POINT
	{
		if(nErrorCode == CSUDI_SUCCESS)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(CSUDIFSFormat_0006_Callback, 
			CSUDI_NULL), "移除回调失败\n");		
		}
	}
	
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSFormat(CSUDI_UINT32 dwDeviceId,CSUDIFSPartitionType_E eType)
//@DESCRIPTION: 格式化一个分卷为非法的文件系统类型
//@PRECONDITION: 1、系统中至少存在一个已识别的分卷
//@INPUT: dwDeviceId = dwVolumeId，dwVolume是由CSUDIFSGetAllDeviceId()获得的分卷设备
//@INPUT: eType = 0x7FFFFFFF(假设0x7FFFFFFF为非法的文件系统类型)
//@EXPECTATION: 格式化成功，格式化前的文件不复存在
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到所有的设备，根据主设备号找到一个分卷设备dwVolumeId
//@EXECUTIONFLOW: 2、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSFormat_0007", "w")创建并打开文件，返回句柄hFile，句柄不为空
//@EXECUTIONFLOW: 3、调用CSUDIFSClose(hFile)关闭该文件，返回值应当为0
//@EXECUTIONFLOW: 4、调用CSUDIFSFormat(dwVolumeId, 0x7FFFFFFF)格式化分卷设备，格式化失败，返回值应当不等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 5、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSFormat_0007", "r")以只读方式打开格式化前创建的文件，应当成功，返回值不等于CSUDI_NULL
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFormat_0007(void)
{
	//手动测试
	CSUDI_UINT32   dwIds[32];
	int nActIdCnt = 0;
	int i = 0;
	CSUDI_UINT32 dwVolumeId = 0;
	CSUDI_HANDLE  hHandle = CSUDI_NULL;
	char szFilePath[CSUDIFS_MAX_FILE_PATH];
	CSUDIFSDeviceInfo_S   sDeviceInfo;
	int  nLen = 0;
	CSUDI_BOOL   bNeedRemove = CSUDI_FALSE;

	CSFSPrint("本测试用例会对插入设备进行格式化，确认继续请按(Y)/否则请按(N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "选择了退出");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt), "获取设备id失败\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "一个设备也不存在，无法获得设备信息\n");

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			dwVolumeId = dwIds[i];

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo), "获取设备信息失败\n");
			CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "分卷挂载点为空\n");
			CSTK_ASSERT_TRUE_FATAL(sDeviceInfo.m_szMountPoint != CSUDI_NULL, "无分卷挂载点\n");
			
			memset(szFilePath, 0, sizeof(szFilePath));
			nLen = strlen(sDeviceInfo.m_szMountPoint);
			memcpy(szFilePath, sDeviceInfo.m_szMountPoint, nLen);

			if(szFilePath[nLen -1] != '/') szFilePath[nLen] = '/';

			strcat(szFilePath, "CSUDIFSFormat_0007");
			
			CSTK_ASSERT_TRUE_FATAL((hHandle = CSUDIFSOpen(szFilePath, "w")) != CSUDI_NULL, "创建文件失败\n");

			bNeedRemove = CSUDI_TRUE;
			
			CSTK_ASSERT_TRUE_FATAL(CSUDIFSClose(hHandle) == 0, "关闭文件失败\n");

			hHandle = CSUDI_NULL;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFSFormat(dwVolumeId, (CSUDIFSPartitionType_E)0x7FFFFFFF), "格式化分卷不应该成功\n");

			bNeedRemove = CSUDI_TRUE;
			CSTK_ASSERT_TRUE_FATAL((hHandle = CSUDIFSOpen(szFilePath, "r")) != CSUDI_NULL, "打文件失败\n");

			CSTK_ASSERT_TRUE_FATAL(CSUDIFSClose(hHandle) == 0, "关闭文件失败\n");

			hHandle = CSUDI_NULL;
		}
	}

	CSTK_ASSERT_TRUE_FATAL(dwVolumeId != -1, "找不到一个分卷设备\n");
	
	CSTK_FATAL_POINT
	{
		if(hHandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hHandle), "关闭文件失败\n");
			hHandle = CSUDI_NULL;
		}
		if(bNeedRemove)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSRemove(szFilePath), "删除文件失败\n");
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
//@DESCRIPTION: 格式化一个还未格式化的存储设备
//@PRECONDITION: 1、系统中至少存在一个已识别的存储设备
//@PRECONDITION: 2、平台支持格式化成EM_UDIFS_PARTITION_FAT32
//@INPUT: dwDeviceId = dwStorageId，dwStorageId是由CSUDIFSGetAllDeviceId()获得的存储设备
//@INPUT: eType = EM_UDIFS_PARTITION_EXT3
//@EXPECTATION: 格式化成功，且在已注册的回调中收到EM_UDIRDI_EVENT_FOUND事件
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已识别的存储设备dwStorageId
//@EXECUTIONFLOW: 2、调用CSUDIFSRDIAddCallback(CSUDIFSFormat_0008_Callback, CSUDI_NULL)注册回调函数，应当返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIFSFormat(dwStorageId, EM_UDIFS_PARTITION_EXT3)格式化存储设备，应当返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、检查回调函数CSUDIFSFormat_0008_Callback中是否有EM_UDIRDI_EVENT_FOUND事件
//@EXECUTIONFLOW: 5、调用CSUDIFSRDIRemoveCallback(CSUDIFSFormat_0002_Callback, CSUDI_NULL)删除回调函数，恢复环境
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFormat_0008(void)
{
	//手动测试
	CSUDI_UINT32   dwIds[32];
	int nActIdCnt = 0;
	int i =0;
	CSUDI_UINT32  dwStorageId = 0;
	CSUDI_Error_Code nErrorCode = CSUDI_SUCCESS;

	CSFSPrint("本测试用例会对插入设备进行格式化，确认继续请按(Y)/否则请按(N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "选择了退出");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (nErrorCode = CSUDIFSRDIAddCallback(CSUDIFSFormat_0008_Callback,
		CSUDI_NULL)),"添加回调失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt), "获取设备id失败\n");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "一个设备也不存在，无法获得设备信息\n");

	g_eCSUDIFSFormat_0008 = EM_UDIRDI_EVENT_UNKNOWN;

	for(i = 0; i < nActIdCnt; i++)
	{
		if((dwIds[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_STORAGE_MAJOR_DEVICEID)
		{
			dwStorageId = dwIds[i];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSFormat(dwStorageId, EM_UDIFS_PARTITION_FAT32), "格式化存储设备失败\n");
			break;
		}
	}

	CSTK_ASSERT_TRUE_FATAL(g_eCSUDIFSFormat_0008 == EM_UDIRDI_EVENT_FOUND, "回调函数中没有收到格式化事件\n");

	CSTK_FATAL_POINT
	{
		if(nErrorCode == CSUDI_SUCCESS)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(CSUDIFSFormat_0008_Callback, 
			CSUDI_NULL), "移除回调失败\n");		
		}
	}
	
	return CSUDI_TRUE;	
}
/****************************CSUDIFSFormat  End *******************************/


/****************************CSUDIFSGetCodePage  Start ******************************/ 
//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetCodePage(int * pnCodePage)
//@DESCRIPTION: 正常获取编码方式
//@PRECONDITION: 无
//@INPUT: pnCodePage = &nCodePage，其中int nCodePage
//@EXPECTATION: 返回值等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIFSGetCodePage(&nCodePage)
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetCodePage_0001(void)
{
	int  nCodePage;
	
	CSTK_ASSERT_TRUE(CSUDI_SUCCESS == CSUDIFSGetCodePage(&nCodePage), "获得本地码失败\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetCodePage(int * pnCodePage)
//@DESCRIPTION: 接收编码方式的指针非法
//@PRECONDITION: 无
//@INPUT: pnCodePage = CSUDI_NULL
//@EXPECTATION: 返回值不等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIFSGetCodePage(CSUDI_NULL)
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetCodePage_0002(void)
{
	CSTK_ASSERT_TRUE(CSUDI_SUCCESS != CSUDIFSGetCodePage(CSUDI_NULL), "获得本地码不应该成功\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}
/****************************CSUDIFSGetCodePage  End *******************************/

/****************************CSUDIFSOpen  Start ***********************************/ 
//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpen(const char * pcFileName, const char * pcMode)
//@DESCRIPTION: 创建一个只写的新文件
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcFileName = 分区挂载路径 + "CSUDIFSOpen_0001"
//@INPUT: pcMode = "w"
//@EXPECTATION: 返回值不等于CSUDI_NULL，且文件只可写
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载目录	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSOpen_0001", "w")创建一个只写的新文件
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite()往新文件写数据
//@EXECUTIONFLOW: 5、调用CSUDIFSRead()读新文件中的数据
//@EXECUTIONFLOW: 6、调用CSUDIFSClose()关闭创建的文件
//@EXECUTIONFLOW: 7、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSOpen_0001")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpen_0001(void)
{
	CSUDI_HANDLE  hFile = CSUDI_NULL;
	char  szBuf[10];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分卷设备挂载点失败\n");
	strcat(g_szVolumeMountPoint, "CSUDIFSOpen_0001");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(g_szVolumeMountPoint, "w")), "创建只写文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(strlen("hello") == CSUDIFSWrite(hFile, "hello", strlen("hello")), "向文件写数据失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSRead(hFile, szBuf, sizeof(szBuf)) < 0, "读只写文件应该失败\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");
			CSTK_ASSERT_TRUE_FATAL(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");		
		}
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpen(const char * pcFileName, const char * pcMode)
//@DESCRIPTION: 以只读方式打开一个已存在的文件
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcFileName = 分区挂载路径 + "CSUDIFSOpen_0002"
//@INPUT: pcMode = "r"
//@EXPECTATION: 返回值不为CSUDI_NULL， 且文件只可读不可写
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载目录	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSOpen_0002", "w")创建文件
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite()往新文件写数据
//@EXECUTIONFLOW: 5、调用CSUDIFSClose()关闭创建的文件
//@EXECUTIONFLOW: 6、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSOpen_0002", "r")以只读方式打开文件
//@EXECUTIONFLOW: 7、调用CSUDIFSRead()读新文件中的数据
//@EXECUTIONFLOW: 8、调用CSUDIFSWrite()写只读文件
//@EXECUTIONFLOW: 9、调用CSUDIFSClose()关闭文件
//@EXECUTIONFLOW: 10、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSOpen_0002")
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpen_0002(void)
{
	CSUDI_HANDLE  hFile = CSUDI_NULL;
	char  szBuf[10];
	CSUDI_BOOL   bNeedRemove = CSUDI_FALSE;

	//获得文件全路径
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分卷设备挂载点失败\n");
	strcat(g_szVolumeMountPoint, "CSUDIFSOpen_0002");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(g_szVolumeMountPoint, "w")), "创建只写文件失败\n");

	bNeedRemove = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(strlen("CSUDIFSOpen_0002") == CSUDIFSWrite(hFile, "CSUDIFSOpen_0002", strlen("CSUDIFSOpen_0002")), "向文件写数据失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");

	hFile = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(g_szVolumeMountPoint, "r")), "以只读方式打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSRead(hFile, szBuf, sizeof(szBuf)) > 0, "读文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSWrite(hFile, "CSUDIFSOpen_0002", strlen("CSUDIFSOpen_0002")) < 0, "写只读文件应该失败\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");
		}

		if(bNeedRemove)
		{
			CSTK_ASSERT_TRUE_FATAL(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");
		}		
	}
	
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpen(const char * pcFileName, const char * pcMode)
//@DESCRIPTION: 以可读可写方式创建一个新文件
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcFileName = 分区挂载路径 + "CSUDIFSOpen_0003"
//@INPUT: pcMode = "w+"
//@EXPECTATION: 返回值不为CSUDI_NULL， 且文件可读可写
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载目录	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSOpen_0003", "w+")创建文件
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite()往新文件写数据
//@EXECUTIONFLOW: 5、调用CSUDIFSRead()读新文件中的数据
//@EXECUTIONFLOW: 6、调用CSUDIFSClose()关闭文件
//@EXECUTIONFLOW: 7、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSOpen_0003")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpen_0003(void)
{
	CSUDI_HANDLE  hFile = CSUDI_NULL;
	char  szBuf[100];
	CSUDI_BOOL   bNeedRemove = CSUDI_FALSE;

	//获得文件全路径
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分卷设备挂载点失败\n");
	strcat(g_szVolumeMountPoint, "CSUDIFSOpen_0003");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "创建可读可写文件失败\n");

	bNeedRemove = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(strlen("CSUDIFSOpen_0003") == CSUDIFSWrite(hFile, "CSUDIFSOpen_0003", strlen("CSUDIFSOpen_0003")), "向文件写数据失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile, 0, CSUDIFS_SEEK_SET), "重置文件指针失败\n");

	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(strlen("CSUDIFSOpen_0003") == CSUDIFSRead(hFile, szBuf, strlen("CSUDIFSOpen_0003")), "读文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(szBuf, "CSUDIFSOpen_0003"), "读出的数据与写入的不一致\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");
			CSTK_ASSERT_TRUE_FATAL(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");
		}	
	}
	
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpen(const char * pcFileName, const char * pcMode)
//@DESCRIPTION: 以可读可写方式打开一个已存在的文件
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcFileName = 分区挂载路径 + "CSUDIFSOpen_0004"
//@INPUT: pcMode = "r+"
//@EXPECTATION: 返回值不为CSUDI_NULL， 且文件可读可写
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载目录	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSOpen_0004", "w")创建文件
//@EXECUTIONFLOW: 4、调用CSUDIFSClose()关闭创建的文件
//@EXECUTIONFLOW: 5、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSOpen_0004", "r+")以可读可写方式打开文件
//@EXECUTIONFLOW: 6、调用CSUDIFSWrite()写文件
//@EXECUTIONFLOW: 7、调用CSUDIFSRead()读新文件中的数据，判断是否与写入的一致
//@EXECUTIONFLOW: 8、调用CSUDIFSClose()关闭文件
//@EXECUTIONFLOW: 9、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSOpen_0004")
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpen_0004(void)
{
	CSUDI_HANDLE  hFile = CSUDI_NULL;
	char  szBuf[100];
	CSUDI_BOOL   bNeedRemove = CSUDI_FALSE;
	char   szTest[] = "CSUDIFSOpen_0004";
	int  nLen = strlen(szTest);
	
	//获得文件全路径
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分卷设备挂载点失败\n");
	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(g_szVolumeMountPoint, "w")), "创建只写文件失败\n");

	bNeedRemove = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");

	hFile = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(g_szVolumeMountPoint, "r+")), "以可读可写方式打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "写入文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile, 0, CSUDIFS_SEEK_SET), "重置文件指针失败\n");

	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSRead(hFile, szBuf, nLen) > 0, "读文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(szBuf, szTest), "读出的与写入的不一致\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");
		}

		if(bNeedRemove)
		{
			CSTK_ASSERT_TRUE_FATAL(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");
		}		
	}
	
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpen(const char * pcFileName, const char * pcMode)
//@DESCRIPTION: 以追加方式创建一个只写文件
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcFileName = 分区挂载路径 + "CSUDIFSOpen_0005"
//@INPUT: pcMode = "a"
//@EXPECTATION: 返回值不为CSUDI_NULL， 且文件只可写不可读
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSOpen_0005", "a")创建文件
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite()写文件
//@EXECUTIONFLOW: 5、调用CSUDIFSRead()读取文件，应当读取失败
//@EXECUTIONFLOW: 6、调用CSUDIFSClose()关闭文件
//@EXECUTIONFLOW: 7、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSOpen_0005")移除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpen_0005(void)
{
	CSUDI_HANDLE  hFile = CSUDI_NULL;
	char  szBuf[100];
	CSUDI_BOOL   bNeedRemove = CSUDI_FALSE;
	char   szTest[] = "CSUDIFSOpen_0005";
	int  nLen = strlen(szTest);
	
	//获得文件全路径
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分卷设备挂载点失败\n");
	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(g_szVolumeMountPoint, "a")), "创建一个只写文件失败\n");

	bNeedRemove = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "写入文件失败\n");

	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSRead(hFile, szBuf, nLen) < 0, "读取追加只可写文件应该失败\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");
		}

		if(bNeedRemove)
		{
			CSTK_ASSERT_TRUE_FATAL(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");
		}		
	}
	
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpen(const char * pcFileName, const char * pcMode)
//@DESCRIPTION: 以追加方式创建一个可读可写文件
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcFileName = 分区挂载路径 + "CSUDIFSOpen_0006"
//@INPUT: pcMode = "a+"
//@EXPECTATION: 返回值不为CSUDI_NULL， 且文件可写可读
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSOpen_0006", "a+")创建一可读可写文件
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite()写文件
//@EXECUTIONFLOW: 5、调用CSUDIFSSeek()移动文件指针到文件头
//@EXECUTIONFLOW: 6、调用CSUDIFSRead()读取文件，读出数据应当与写入时一致
//@EXECUTIONFLOW: 7、调用CSUDIFSClose()关闭文件
//@EXECUTIONFLOW: 8、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSOpen_0006")移除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpen_0006(void)
{
	CSUDI_HANDLE  hFile = CSUDI_NULL;
	char  szBuf[100];
	CSUDI_BOOL   bNeedRemove = CSUDI_FALSE;
	char   szTest[] = "CSUDIFSOpen_0006";
	int  nLen = strlen(szTest);

	//获得文件全路径
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分卷设备挂载点失败\n");
	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(g_szVolumeMountPoint, "a+")), "以追加方式创建一个可读可写文件失败\n");

	bNeedRemove = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "写入文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile, 0, CSUDIFS_SEEK_SET), "改变文件指针到失败\n");

	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSRead(hFile, szBuf, nLen) == nLen, "读取文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(szTest, szBuf), "读取的数据与写入时不一致\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");
		}

		if(bNeedRemove)
		{
			CSTK_ASSERT_TRUE_FATAL(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");
		}		
	}
	
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpen(const char * pcFileName, const char * pcMode)
//@DESCRIPTION: 以追加方式创建一个已经存在的文件
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcFileName = 分区挂载路径 + "CSUDIFSOpen_0007"
//@INPUT: pcMode = "a+"
//@EXPECTATION: 返回值不为CSUDI_NULL， 且原文件数据被清空
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSOpen_0007", "w")创建一可写文件
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite()写文件
//@EXECUTIONFLOW: 5、调用CSUDIFSClose()关闭文件
//@EXECUTIONFLOW: 6、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSOpen_0007", "a+")打开文件
//@EXECUTIONFLOW: 7、调用CSUDIFSRead()读取文件，读出数据应当为0
//@EXECUTIONFLOW: 8、调用CSUDIFSClose()关闭文件
//@EXECUTIONFLOW: 9、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSOpen_0007")移除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpen_0007(void)
{
	CSUDI_HANDLE  hFile = CSUDI_NULL;
	char  szBuf[100];
	CSUDI_BOOL   bNeedRemove = CSUDI_FALSE;
	char   szTest[] = "CSUDIFSOpen_0007";
	int  nLen = strlen(szTest);
	
	//获得文件全路径
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分卷设备挂载点失败\n");
	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(g_szVolumeMountPoint, "w")), "创建文件失败\n");

	bNeedRemove = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "写入文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");

	hFile = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(g_szVolumeMountPoint, "a+")), "以追加方式打开文件失败\n");

	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSRead(hFile, szBuf, nLen) != 0, "读取数据应当为0\n");

	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");
		}

		if(bNeedRemove)
		{
			CSTK_ASSERT_TRUE_FATAL(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");
		}		
	}
	
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpen(const char * pcFileName, const char * pcMode)
//@DESCRIPTION: 打开一个文件夹
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcFileName = 分区挂载路径 + "CSUDIFSOpen_0008"
//@INPUT: pcMode = "r"
//@EXPECTATION: 返回值等于CSUDI_NULL
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径
//@EXECUTIONFLOW: 3、调用CSUDIFSMkdir(分区挂载路径 + "CSUDIFSOpen_0008",创建文件夹
//@EXECUTIONFLOW: 4、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSOpen_0008", "r")，应当失败
//@EXECUTIONFLOW: 5、调用CSUDIFSRmdir(分区挂载路径 + "CSUDIFSOpen_0008"),删除文件夹
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpen_0008(void)
{
	char   szTest[] = "CSUDIFSOpen_0008";
	int   nMkDirRet = -1; 
	
	//获得文件全路径
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分卷设备挂载点失败\n");
	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(0 == (nMkDirRet = CSUDIFSMkdir(g_szVolumeMountPoint, 0)), "创建文件夹失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == CSUDIFSOpen(g_szVolumeMountPoint, "r"), "打开文件夹应该失败\n");
	
	CSTK_FATAL_POINT
	{
		if(nMkDirRet == 0)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(g_szVolumeMountPoint), "删除文件夹失败\n");
		}
	}	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpen(const char * pcFileName, const char * pcMode)
//@DESCRIPTION: 打开不存在的文件
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcFileName = 分区挂载路径 + "CSUDIFSOpen_0009"
//@INPUT: pcMode = "r"
//@EXPECTATION: 返回值等于CSUDI_NULL
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSOpen_0009", "r")，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpen_0009(void)
{
	char   szTest[] = "CSUDIFSOpen_0009";
	
	//获得文件全路径
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分卷设备挂载点失败\n");
	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == CSUDIFSOpen(g_szVolumeMountPoint, "r"), "打开不存在的文件应该失败\n");

	CSTK_FATAL_POINT
	{
		;
	}	
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpen(const char * pcFileName, const char * pcMode)
//@DESCRIPTION: 创建中文名字的文件
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcFileName = 分区挂载路径 + "CSUDIFSOpen_0010"
//@INPUT: pcMode = "w"
//@EXPECTATION: 返回值不等CSUDI_NULL
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSOpen_0010_中文", "w")创建中文文件，应当成功
//@EXECUTIONFLOW: 4、调用CSUDIFSClose()关闭文件
//@EXECUTIONFLOW: 5、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSOpen_0010_中文")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpen_0010(void)
{
	char   szTest[] = "CSUDIFSOpen_0010_中文";
	CSUDI_HANDLE     hFile = CSUDI_NULL;
	
	//获得文件全路径
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分卷设备挂载点失败\n");
	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(g_szVolumeMountPoint, "w")), "创建中文名文件失败\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");
			CSTK_ASSERT_TRUE_FATAL(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");
		}
	}	
	return CSUDI_TRUE;	
}


//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpen(const char * pcFileName, const char * pcMode)
//@DESCRIPTION: 创建空名文件
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcFileName = CSUDI_NULL
//@INPUT: pcMode = "w"
//@EXPECTATION: 返回值等于CSUDI_NULL	
//@EXECUTIONFLOW: 1、调用CSUDIFSOpen(CSUDI_NULL, "w")创建文件，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpen_0011(void)
{
	CSTK_ASSERT_TRUE(CSUDI_NULL == CSUDIFSOpen(CSUDI_NULL, "w"), "打开空指针文件应该失败\n");

	CSTK_FATAL_POINT
	{
		;	
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpen(const char * pcFileName, const char * pcMode)
//@DESCRIPTION: 创建一个文件名超过最大长度的文件
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcFileName = 分区挂载路径 + "CSUDIFSOpen_0012"
//@INPUT: pcMode = "w"
//@EXPECTATION: 创建失败，返回值等于CSUDI_NULL
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSOpen_0012", "w")创建文件，应当返回CSUDI_NULL
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpen_0012(void)
{
	char   szTest[] = "CSUDIFSOpen_0012";
	CSUDI_HANDLE     hFile = CSUDI_NULL;
	char   szFilePath[1024];
	int     nLen = 0;
	int     nCount;
	nLen = strlen(szTest);
	nCount = 640/nLen;
	
	//获得文件全路径
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分卷设备挂载点失败\n");
	memset(szFilePath, 0, 1024);
	strcpy(szFilePath, g_szVolumeMountPoint);
	while(nCount -- > 0)
	{
		strcat(szFilePath, szTest);
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == (hFile = CSUDIFSOpen(szFilePath, "w")), "文件名太长，创建文件失败\n");
	
	CSTK_FATAL_POINT
	{
		;
	}	
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpen(const char * pcFileName, const char * pcMode)
//@DESCRIPTION: 在打开一个可读可写(或只写)文件时，文件应该先清空
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcFileName = 分区挂载路径 + "CSUDIFSOpen_0013"
//@INPUT: pcMode = "w" 或者pcMode = "w+"
//@EXPECTATION: 文件长度清零
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载目录	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSOpen_0013", "w+")创建文件
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite()往新文件写数据"CSUDIFSOpen_0013"
//@EXECUTIONFLOW: 5、调用CSUDIFSSeek()重置文件指针
//@EXECUTIONFLOW: 6、调用CSUDIFSRead()读新文件中的数据，期望读取到"CSUDIFSOpen_0013", 并且返回长度为strlen("CSUDIFSOpen_0013")
//@EXECUTIONFLOW: 7、调用CSUDIFSClose()关闭文件
//@EXECUTIONFLOW: 8、调用CSUDIFSOpen()(分区挂载路径 + "CSUDIFSOpen_0013", "w")创建文件
//@EXECUTIONFLOW: 9、调用CSUDIFSWrite()往新文件写数据"CSUDIFSWrite"
//@EXECUTIONFLOW: 10、调用CSUDIFSClose()关闭文件
//@EXECUTIONFLOW: 11、调用CSUDIFSOpen()(分区挂载路径 + "CSUDIFSOpen_0013", "r")创建文件
//@EXECUTIONFLOW: 12、调用CSUDIFSRead()读新文件中的数据，期望读取到数据"CSUDIFSWrite"，并且长度为strlen("CSUDIFSWrite")
//@EXECUTIONFLOW: 13、调用CSUDIFSClose()关闭文件
//@EXECUTIONFLOW: 14、重复步骤3-步骤13，同时将步骤8中CSUDIFSOpen模式改为w+
//@EXECUTIONFLOW: 15、调用CSUDIFSClose()关闭文件
//@EXECUTIONFLOW: 16、调用CSUDIFSRemove()移除分区挂载路径
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpen_0013(void)
{
	CSUDI_HANDLE  hFile = CSUDI_NULL;
	char acBufOpen[23] = "CSTC_FS_IT_CSUDIFSOpen";
	char acBufWrite[13] = "CSUDIFSWrite";   
	char  szBuf[100];
	CSUDI_BOOL bModeFlag = CSUDI_FALSE;
	int i = 0;
	//获得文件全路径
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "步骤1，获取分卷设备挂载点失败\n");
	strcat(g_szVolumeMountPoint, acBufOpen);

	while( i++ < 2 )
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "步骤3，创建可读可写文件失败\n");
		
		CSTK_ASSERT_TRUE_FATAL(strlen(acBufOpen) == CSUDIFSWrite(hFile, acBufOpen, strlen(acBufOpen)), "步骤4，向文件写数据失败\n");
		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile, 0, CSUDIFS_SEEK_SET), "步骤5，重置文件指针失败\n");
		
		memset(szBuf, 0, sizeof(szBuf));
		CSTK_ASSERT_TRUE_FATAL(strlen(acBufOpen) == CSUDIFSRead(hFile, szBuf, strlen(acBufOpen)), "步骤6，读文件失败\n");
		CSTK_ASSERT_TRUE_FATAL(0 == strcmp(szBuf, acBufOpen), "步骤6，读出的数据与写入的不一致\n");
		
		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "步骤7，关闭文件失败\n");
	
		if( CSUDI_FALSE == bModeFlag)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(g_szVolumeMountPoint, "w")), "步骤8，创建可读可写文件失败\n");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "步骤14，创建可读可写文件失败\n");
		}
		bModeFlag = CSUDI_TRUE;
		
		CSTK_ASSERT_TRUE_FATAL(strlen(acBufWrite) == CSUDIFSWrite(hFile, acBufWrite, strlen(acBufWrite)), "步骤9，向文件写数据失败\n");

		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "步骤10，关闭文件失败\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(g_szVolumeMountPoint, "r")), "步骤11，创建可读可写文件失败\n");

		memset(szBuf, 0, sizeof(szBuf));
		CSTK_ASSERT_TRUE_FATAL(strlen(acBufWrite) == CSUDIFSRead(hFile, szBuf, strlen(acBufOpen)), "步骤12，读文件失败\n");
		
		CSTCPrint("写入的数据是:CSUDIFSWrite，读出的数据是:%s \n\n", szBuf);

		CSTK_ASSERT_TRUE_FATAL(0 == strcmp(szBuf, acBufWrite), "步骤12，读出的数据与写入的不一致\n");
		
		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "步骤13，关闭文件失败\n");

		hFile = CSUDI_NULL;
	}

	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "步骤14，关闭文件失败\n");
		}
		
		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRemove(g_szVolumeMountPoint), "步骤15，移除文件失败\n");
	}
	
	return CSUDI_TRUE;	
}

/****************************CSUDIFSOpen  End *******************************/

/****************************CSUDIFSRead  Start *****************************/ 
//@CASEGROUP: int CSUDIFSRead(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount)
//@DESCRIPTION: 读取数据不大于文件大小
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = hFile1, hFile1为CSUDIFSOpen()返回的合法句柄
//@INPUT: pcBuf = szBuf, 其中char szBuf[5]
//@INPUT: uCount = sizeof(szBuf) - 1
//@EXPECTATION: 返回值等于sizeof(szBuf) - 1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSRead_0001", "w+")创建并打开文件，得到句柄hFile1
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile1, "CSUDIFSRead_0001", strlen("CSUDIFSRead_0001"))写入数据
//@EXECUTIONFLOW: 5、调用CSUDIFSSeek(hFile1, 0, CSFS_SEEK_SET)重置文件指针
//@EXECUTIONFLOW: 6、调用CSUDIFSRead(hFile1, szBuf, sizeof(szBuf) - 1)读取小于文件的数据，应该为写入时的字串
//@EXECUTIONFLOW: 7、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 8、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSRead_0001")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRead_0001(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSRead_0001";
	int     nLen = strlen(szTest);
	char   szBuf[30]="aaaaaaaaaaaaaaaaaaaaaa";
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "创建并打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "写入数据失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile1, 0, CSUDIFS_SEEK_SET), "重置文件失败\n");
	
	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(16 == CSUDIFSRead(hFile1, szBuf, 20), "读取数据失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != strstr(szTest, szBuf), "读出数据不是写入时的字串\n");

	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");
		}
	}

	return  CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSRead(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount)
//@DESCRIPTION: 读取数据大于文件大小
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = hFile1, hFile1为CSUDIFSOpen()返回的合法句柄
//@INPUT: pcBuf = szBuf, 其中char szBuf[100]
//@INPUT: uCount = sizeof(szBuf)- 1
//@EXPECTATION: = nLen, nlen 为文件总长
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSRead_0002", "w+")创建并打开文件
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile1, "CSUDIFSRead_0002", strlen("CSUDIFSRead_0002"))写入数据
//@EXECUTIONFLOW: 5、调用CSUDIFSSeek(hFile1, 0, CSFS_SEEK_SET)重置文件指针
//@EXECUTIONFLOW: 6、调用CSUDIFSRead(hFile1, szBuf, sizeof(szBuf)-1)读取文件，读出数据应该与写入时一致
//@EXECUTIONFLOW: 7、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 8、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSRead_0002")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRead_0002(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSRead_0002";
	int     nLen = strlen(szTest);
	char   szBuf[100];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "创建并打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "写入数据失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile1, 0, CSUDIFS_SEEK_SET), "重置文件失败\n");
	
	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSRead(hFile1, szBuf, sizeof(szBuf) -1), "读取数据失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(szTest, szBuf), "读出数据与写入时不一致\n");

	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");
		}
	}

	return  CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSRead(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount)
//@DESCRIPTION: 读取零字节数据
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = hFile1, hFile1为CSUDIFSOpen()返回的合法句柄
//@INPUT: pcBuf = szBuf, 其中char szBuf[100]
//@INPUT: uCount = 0
//@EXPECTATION: 返回值等于0
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSRead_0003", "w+")创建并打开文件
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile1, "CSUDIFSRead_0003", strlen("CSUDIFSRead_0003"))写入数据
//@EXECUTIONFLOW: 5、调用CSUDIFSSeek(hFile1, 0, CSFS_SEEK_SET)重置文件指针
//@EXECUTIONFLOW: 6、调用CSUDIFSRead(hFile1, szBuf, 0)读取数据，应当为0
//@EXECUTIONFLOW: 7、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 8、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSRead_0003")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRead_0003(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSRead_0003";
	int     nLen = strlen(szTest);
	char   szBuf[100];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "创建并打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "写入数据失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile1, 0, CSUDIFS_SEEK_SET), "重置文件失败\n");
	
	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRead(hFile1, szBuf, 0), "读取数据失败\n");

	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");
		}
	}

	return  CSUDI_TRUE;		
}

//@CASEGROUP: int CSUDIFSRead(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount)
//@DESCRIPTION: 从未写数据的文件中读取数据
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = hFile1, hFile1为CSUDIFSOpen()返回的合法句柄
//@INPUT: pcBuf = szBuf, 其中char szBuf[100]
//@INPUT: uCount = sizeof(szBuf) -1
//@EXPECTATION: 返回值等于0
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSRead_0004", "w+")创建文件，获得句柄hFile1
//@EXECUTIONFLOW: 4、调用CSUDIFSSeek(hFile1, 0, CSFS_SEEK_SET)重置文件指针
//@EXECUTIONFLOW: 5、调用CSUDIFSRead(hFile1, szBuf, sizeof(szBuf)-1)读取数据
//@EXECUTIONFLOW: 6、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 7、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSRead_0004")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRead_0004(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSRead_0004";
	char   szBuf[100];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "创建并打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile1, 0, CSUDIFS_SEEK_SET), "重置文件失败\n");
	
	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRead(hFile1, szBuf, sizeof(szBuf)-1), "读取数据失败\n");

	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");
		}
	}

	return  CSUDI_TRUE;	
}


//@CASEGROUP: int CSUDIFSRead(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount)
//@DESCRIPTION: 使用空文件句柄读数据
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = CSUDI_NULL
//@INPUT: pcBuf = szBuf, 其中char szBuf[100];
//@INPUT: uCount = sizeof(szBuf) -1
//@EXPECTATION: 返回值小于0
//@EXECUTIONFLOW: 1、调用CSUDIFSRead(CSUDI_NULL, szBuf, sizeof(szBuf)-1)读取数据
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRead_0005(void)
{
	char szBuf[100];

	CSTK_ASSERT_TRUE(0 > CSUDIFSRead(CSUDI_NULL, szBuf, sizeof(szBuf) -1), "读取数据不应该成功\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
 }

///@CASEGROUP: int CSUDIFSRead(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount)
//@DESCRIPTION: 使用空指针来存读出的数据
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = hFile1, hFile1为CSUDIFSOpen()返回的合法句柄
//@INPUT: pcBuf = CSUDI_NULL
//@INPUT: uCount = 0
//@EXPECTATION: 返回值小于0
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSRead_0006", "w+")创建文件，获得句柄hFile1
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile1, "CSUDIFSRead_0006", strlen("CSUDIFSRead_0006"))写入数据
//@EXECUTIONFLOW: 5、调用CSUDIFSSeek(hFile1, 0, CSFS_SEEK_SET)重置文件指针
//@EXECUTIONFLOW: 6、调用CSUDIFSRead(hFile1, NULL, 0)读取数据，应该失败
//@EXECUTIONFLOW: 7、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 8、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSRead_0006")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRead_0006(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSRead_0006";
	int     nLen = strlen(szTest);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "创建并打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "写入数据失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile1, 0, CSUDIFS_SEEK_SET), "重置文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 > CSUDIFSRead(hFile1, CSUDI_NULL, 0), "读取数据不应该成功\n");

	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");
		}
	}

	return  CSUDI_TRUE;			
}

//@CASEGROUP: int CSUDIFSRead(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount)
//@DESCRIPTION: 使用不存在的文件句柄读数据
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = 0x7FFFFFFF(假设0x7FFFFFFF为不存在的文件句柄)
//@INPUT: pcBuf = szBuf，其中char szBuf[100]
//@INPUT: uCount = sizeof(szBuf)-1
//@EXPECTATION: 返回值小于0
//@EXECUTIONFLOW: 1、调用CSUDIFSRead(0x7FFFFFFF, szBuf, sizeof(szBuf)-1)读取文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRead_0007(void)
{
	char szBuf[100];

	CSTK_ASSERT_TRUE(0 > CSUDIFSRead((CSUDI_HANDLE)0x7FFFFFFF, szBuf,sizeof(szBuf)-1), "读取不存在的文件不应该成功成功\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}
/****************************CSUDIFSRead  End *******************************/

/****************************CSUDIFSWrite  Start *****************************/ 
//@CASEGROUP: int CSUDIFSWrite(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount)
//@DESCRIPTION: 写入大于零字节数的数据
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = hFile1，其中hFile1为CSUDIFSOpen()返回的合法句柄
//@INPUT: pcBuf = "CSUDIFSWrite_0001"
//@INPUT: uCount = strlen("CSUDIFSWrite_0001")
//@EXPECTATION: 写入成功，读出文件中的数据与写入时一致
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "strlen("CSUDIFSWrite_0001")", "w+")创建文件，获得句柄hFile1
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile1, "CSUDIFSWrite_0001", strlen("CSUDIFSWrite_0001"))写入数据
//@EXECUTIONFLOW: 5、调用CSUDIFSSeek(hFile1, 0, CSFS_SEEK_SET)重置文件指针
//@EXECUTIONFLOW: 5、调用CSUDIFSRead(hFile1, szBuf, sizeof(szBuf)-1)读取文件，应当与写入时一致
//@EXECUTIONFLOW: 5、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 6、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSWrite_0001")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSWrite_0001(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSWrite_0001";
	int     nLen = strlen(szTest);
	char   szBuf[100];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "创建并打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "写入数据失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile1, 0, CSUDIFS_SEEK_SET), "重置文件失败\n");
	
	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(0 < CSUDIFSRead(hFile1, szBuf, sizeof(szBuf) -1), "读取数据失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(szTest, szBuf), "读出数据不是写入时的字串\n");

	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");
		}
	}

	return  CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSWrite(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount)
//@DESCRIPTION: 写入零字节
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = hFile1，其中hFile1为CSUDIFSOpen()返回的合法句柄
//@INPUT: pcBuf = "CSUDIFSWrite_0002"
//@INPUT: uCount = 0
//@EXPECTATION: 返回值等于0
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSWrite_0002", "w+")创建并打开文件，获得句柄hFile1
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile1, "CSUDIFSWrite_0002", 0)写入0字节
//@EXECUTIONFLOW: 5、调用CSUDIFSRead(hFile1, szBuf, sizeof(szBuf)-1)读取数据，返回值应该为0
//@EXECUTIONFLOW: 6、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 7、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSWrite_0002")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSWrite_0002(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSWrite_0001";
	char   szBuf[100];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "创建并打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSWrite(hFile1, szTest, 0), "写入数据应该为0\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile1, 0, CSUDIFS_SEEK_SET), "重置文件失败\n");
	
	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRead(hFile1, szBuf, sizeof(szBuf) -1), "读出数据应该为0\n");

	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");
		}
	}

	return  CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSWrite(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount)
//@DESCRIPTION: 使用空文件句柄写写数据
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = CSUDI_NULL
//@INPUT: pcBuf = "CSUDIFSWrite_0003"
//@INPUT: uCount = strlen("CSUDIFSWrite_0003")
//@EXPECTATION: 返回值小于0
//@EXECUTIONFLOW: 1、调用CSUDIFSWrite(CSUDI_NULL, "CSUDIFSWrite_0003", strlen("CSUDIFSWrite_0003"))写入数据，应该失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSWrite_0003(void)
{
	CSTK_ASSERT_TRUE(0 > CSUDIFSWrite(CSUDI_NULL, "CSUDIFSWrite_0003", strlen("CSUDIFSWrite_0003")), "写入文件应该失败\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSWrite(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount)
//@DESCRIPTION: 使用不存在的句柄写数据
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = 0x7FFFFFFF(假设0x7FFFFFFF为不存在的句柄)
//@INPUT: pcBuf = "CSUDIFSWrite_0004"
//@INPUT: uCount = strlen("CSUDIFSWrite_0004")
//@EXPECTATION: 返回值小于0
//@EXECUTIONFLOW: 1、调用CSUDIFSWrite(0x7FFFFFFF, "CSUDIFSWrite_0004", strlen("CSUDIFSWrite_0004"))写入数据，应该失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSWrite_0004(void)
{
	CSTK_ASSERT_TRUE(0 > CSUDIFSWrite((CSUDI_HANDLE)0x7FFFFFFF, "CSUDIFSWrite_0004", strlen("CSUDIFSWrite_0004")), "写入文件应该失败\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSWrite(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount)
//@DESCRIPTION: 从空指针写入数据
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = hFile1， 其中hFile1为CSUDIFSOpen()返回的合法句柄
//@INPUT: pcBuf = CSUDI_NULL
//@INPUT: uCount = 0
//@EXPECTATION: 返回值小于0
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSWrite_0005", "w+")
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile1, CSUDI_NULL, 0)写入数据，应该失败
//@EXECUTIONFLOW: 5、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 6、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSWrite_0005")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSWrite_0005(void)
{
	char   szTest[] = "CSUDIFSWrite_0001";
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "创建并打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 > CSUDIFSWrite(hFile1, CSUDI_NULL, 0), "写入数据应该失败\n");

	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");
		}
	}

	return  CSUDI_TRUE;	
}
/****************************CSUDIFSWrite  End *******************************/

/****************************CSUDIFSClose  Start *****************************/ 
//@CASEGROUP: int CSUDIFSClose(CSUDI_HANDLE hFile)
//@DESCRIPTION: 关闭一个合法的文件句柄
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = hFile1, 其中hFile1为CSUDIFSOpen()返回的合法句柄
//@EXPECTATION: 返回值等于0
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSClose_0001", "w+")创建并打开一个文件，并获得句柄hFile1
//@EXECUTIONFLOW: 4、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 5、调用CSUDIFSClose(hFile1)再次关闭文件，应当失败
//@EXECUTIONFLOW: 6、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSClose_0001")
CSUDI_BOOL CSTC_FS_IT_CSUDIFSClose_0001(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSClose_0001"; 
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "创建并打开文件失败\n");

	CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");

	CSTK_ASSERT_TRUE(0 >  CSUDIFSClose(hFile1), "再次关闭文件不应该成功\n");
	
	CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");

	CSTK_FATAL_POINT
	{
		;	
	}

	return  CSUDI_TRUE;		
}

//@CASEGROUP: int CSUDIFSClose(CSUDI_HANDLE hFile)
//@DESCRIPTION: 关闭空文件句柄
//@PRECONDITION: 
//@INPUT: hFile = CSUDI_NULL
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSClose(CSUDI_NULL)关闭句柄，应该失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSClose_0002(void)
{
	CSTK_ASSERT_TRUE(-1 == CSUDIFSClose(CSUDI_NULL), "关闭文件不应该成功\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSClose(CSUDI_HANDLE hFile)
//@DESCRIPTION: 关闭不存在的文件句柄
//@PRECONDITION:
//@INPUT: hFile = 0x7FFFFFFF(假设0x7FFFFFFF为不存在的文件句柄)
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSClose(0x7FFFFFFF)关闭句柄，应该失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSClose_0003(void)
{
	CSTK_ASSERT_TRUE(-1 == CSUDIFSClose((CSUDI_HANDLE)0x7FFFFFFF), "关闭文件不应该成功\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}
/****************************CSUDIFSClose  End *******************************/

/****************************CSUDIFSSeek  Start *****************************/ 
//@CASEGROUP: int CSUDIFSSeek(CSUDI_HANDLE hFile, long lOffset, unsigned int uOrigin)
//@DESCRIPTION: 写入数据再定位到文件头读出所有数据
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = hFile1，其中hFile1为CSUDIFSOpen()返回的合法句柄
//@INPUT: lOffset = 0
//@INPUT: uOrigin = CSFS_SEEK_SET
//@EXPECTATION: 返回值等于0，且读出的数据与写入一致
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSSeek_0001", "w+")创建一个文件，获得相应句柄hFile1
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile1, "CSUDIFSSeek_0001", strlen("CSUDIFSSeek_0001"))写入数据
//@EXECUTIONFLOW: 5、调用CSUDIFSSeek(hFile1, 0, CSUDIFS_SEEK_SET)重置文件指针
//@EXECUTIONFLOW: 6、调用CSUDIFSRead(hFile1, szBuf, sizeof(szBuf)-1)读取写入的数据，应该与写入时一致
//@EXECUTIONFLOW: 7、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 8、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSSeek_0001")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSSeek_0001(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSSeek_0001";
	int     nLen = strlen(szTest);
	char   szBuf[100];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "创建并打开文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "写入数据失败\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile1, 0, CSUDIFS_SEEK_SET), "重置文件失败\n");
	
	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSRead(hFile1, szBuf, sizeof(szBuf) -1), "读取数据失败\n");

	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(szTest, szBuf), "读出数据与写入时不一致\n");

	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");
		}
	}

	return  CSUDI_TRUE;			
}

//@CASEGROUP: int CSUDIFSSeek(CSUDI_HANDLE hFile, long lOffset, unsigned int uOrigin)
//@DESCRIPTION: 以文件尾为基础向前偏移整个文件大小
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = hFile1，其中hFile1为CSUDIFSOpen()返回的合法句柄
//@INPUT: lOffset = 0 - strlen("CSUDIFSSeek_0002")
//@INPUT: uOrigin = CSUDIFS_SEEK_CUR
//@EXPECTATION: 返回值等于0，且读出的数据与写入一致
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSSeek_0002", "w+")创建一个文件，获得相应句柄hFile1
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile1, "CSUDIFSSeek_0002", strlen("CSUDIFSSeek_0002"))写入数据
//@EXECUTIONFLOW: 5、调用CSUDIFSSeek(hFile1, 0 - strlen("CSUDIFSSeek_0002"), CSUDIFS_SEEK_CUR)重置文件指针
//@EXECUTIONFLOW: 6、调用CSUDIFSRead(hFile1, szBuf, sizeof(szBuf)-1)读取写入的数据，应该与写入时一致
//@EXECUTIONFLOW: 7、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 8、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSSeek_0002")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSSeek_0002(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSSeek_0002";
	int     nLen = strlen(szTest);
	char   szBuf[100];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "创建并打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "写入数据失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile1, 0 - nLen, CSUDIFS_SEEK_CUR), "重置文件失败\n");
	
	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSRead(hFile1, szBuf, sizeof(szBuf) -1), "读取数据失败\n");

	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(szTest, szBuf), "读出数据与写入时不一致\n");

	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");
		}
	}

	return  CSUDI_TRUE;				
}

//@CASEGROUP: int CSUDIFSSeek(CSUDI_HANDLE hFile, long lOffset, unsigned int uOrigin)
//@DESCRIPTION: 定位空文件句柄
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = CSUDI_NULL
//@INPUT: lOffset = 0
//@INPUT: uOrigin = CSUDIFS_SEEK_SET
//@EXPECTATION: = -1
//@EXECUTIONFLOW: 1、调用CSUDIFSSeek(CSUDI_NULL, 0, CSUDIFS_SEEK_SET)改变文件指针，应该失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSSeek_0003(void)
{
	CSTK_ASSERT_TRUE(-1 == CSUDIFSSeek(CSUDI_NULL, 0, CSUDIFS_SEEK_SET), "重置不存在的句柄文件不应该成功\n");	

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSSeek(CSUDI_HANDLE hFile, long lOffset, unsigned int uOrigin)
//@DESCRIPTION: 定位不存在的文件句柄
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = 0x7FFFFFFF(假设0x7FFFFFFF为不存在的文件句柄)
//@INPUT: lOffset = 10
//@INPUT: uOrigin = CSUDIFS_SEEK_SET 
//@EXPECTATION: = -1
//@EXECUTIONFLOW: 1、调用CSUDIFSSeek(0x7FFFFFFF, 10, CSUDIFS_SEEK_SET)改变文件指针，应该失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSSeek_0004(void)
{
	CSTK_ASSERT_TRUE(-1 == CSUDIFSSeek((CSUDI_HANDLE)0x7FFFFFFF, 10, CSUDIFS_SEEK_SET), "重置不存在的句柄文件不应该成功\n");	

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSSeek(CSUDI_HANDLE hFile, long lOffset, unsigned int uOrigin)
//@DESCRIPTION: 基准位置非法
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = hFile1, 其中hFile1为CSUDIFSOpen()返回的合法句柄
//@INPUT: lOffset =  0
//@INPUT: uOrigin = 0x7FFFFFFF
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSSeek_0005", "w+")创建并打开一个文件，获得句柄hFile1
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile1, "CSUDIFSSeek_0005", strlen("CSUDIFSSeek_0005"))写入数据
//@EXECUTIONFLOW: 5、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 6、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSSeek_0005", "r")打开文件
//@EXECUTIONFLOW: 7、调用CSUDIFSSeek(hFile1, 0, 0x7FFFFFFF)改变文件指针
//@EXECUTIONFLOW: 8、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 9、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSSeek_0005")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSSeek_0005(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSSeek_0005";
	int     nLen = strlen(szTest);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "创建并打开文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "写入数据失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "r")), "打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSSeek(hFile1,  0, 0x7FFFFFFF), "基准位置非法重置文件不应该成功\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");
		}
	}

	return  CSUDI_TRUE;		
}

//@CASEGROUP: int CSUDIFSSeek(CSUDI_HANDLE hFile, long lOffset, unsigned int uOrigin)
//@DESCRIPTION: 从文件开头向后偏移越界
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = hFile1,其中hFile1为CSUDIFSOpen()返回的合法句柄
//@INPUT: lOffset = strlen("CSUDIFSSeek_0006")+10
//@INPUT: uOrigin = CSFS_SEEK_SET
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSSeek_0006", "w+")创建并打开一个文件，获得句柄hFile1
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile1, "CSUDIFSSeek_0006", strlen("CSUDIFSSeek_0006"))写入数据
//@EXECUTIONFLOW: 5、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 6、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSSeek_0006", "r")打开文件
//@EXECUTIONFLOW: 7、调用CSUDIFSSeek(hFile1, strlen("CSUDIFSSeek_0006") + 10, CSUDIFS_SEEK_SET)改变文件指针，应该失败
//@EXECUTIONFLOW: 8、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 9、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSSeek_0006")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSSeek_0006(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSSeek_0006";
	int     nLen = strlen(szTest);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "创建并打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "写入数据失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "r")), "打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSSeek(hFile1,  nLen + 10, CSUDIFS_SEEK_SET), "文件指针越界不应该成功\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");
		}
	}

	return  CSUDI_TRUE;				
}

//@CASEGROUP: int CSUDIFSSeek(CSUDI_HANDLE hFile, long lOffset, unsigned int uOrigin)
//@DESCRIPTION: 从文件开头向前偏移越界
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = hFile1,其中hFile1为CSUDIFSOpen()返回的合法句柄
//@INPUT: lOffset = -1
//@INPUT: uOrigin = CSFS_SEEK_SET
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSSeek_0007", "w+")创建并打开一个文件，获得句柄hFile1
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile1, "CSUDIFSSeek_0007", strlen("CSUDIFSSeek_0007"))写入数据
//@EXECUTIONFLOW: 5、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 6、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSSeek_0007", "r")打开文件
//@EXECUTIONFLOW: 7、调用CSUDIFSSeek(hFile1, -1, CSUDIFS_SEEK_SET)改变文件指针，应该失败
//@EXECUTIONFLOW: 8、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 9、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSSeek_0007")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSSeek_0007(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSSeek_0007";
	int     nLen = strlen(szTest);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "创建并打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "写入数据失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "r")), "打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSSeek(hFile1, -1, CSUDIFS_SEEK_SET), "文件指针越界不应该成功\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");
		}
	}

	return  CSUDI_TRUE;				
}

//@CASEGROUP: int CSUDIFSSeek(CSUDI_HANDLE hFile, long lOffset, unsigned int uOrigin)
//@DESCRIPTION: 从文件尾向后偏移越界
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = hFile1,其中hFile1为CSUDIFSOpen()返回的合法句柄
//@INPUT: lOffset = -1
//@INPUT: uOrigin = CSFS_SEEK_END
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSSeek_0008", "w+")创建并打开一个文件，获得句柄hFile1
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile1, "CSUDIFSSeek_0008", strlen("CSUDIFSSeek_0008"))写入数据
//@EXECUTIONFLOW: 5、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 6、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSSeek_0008", "r")打开文件
//@EXECUTIONFLOW: 7、调用CSUDIFSSeek(hFile1, 1, CSUDIFS_SEEK_END)改变文件指针，应该失败
//@EXECUTIONFLOW: 8、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 9、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSSeek_0008")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSSeek_0008(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSSeek_0008";
	int     nLen = strlen(szTest);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "创建并打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "写入数据失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "r")), "打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSSeek(hFile1, 1, CSUDIFS_SEEK_END), "文件指针越界不应该成功\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");
		}
	}

	return  CSUDI_TRUE;				
}

//@CASEGROUP: int CSUDIFSSeek(CSUDI_HANDLE hFile, long lOffset, unsigned int uOrigin)
//@DESCRIPTION: 从文件中向后偏移越界
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = hFile1,其中hFile1为CSUDIFSOpen()返回的合法句柄
//@INPUT: lOffset = strlen("CSUDIFSSeek_0006")
//@INPUT: uOrigin = CSFS_SEEK_CUR
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSSeek_0009", "w+")创建并打开一个文件，获得句柄hFile1
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile1, "CSUDIFSSeek_0006", strlen("CSUDIFSSeek_0009"))写入数据
//@EXECUTIONFLOW: 5、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 6、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSSeek_0009", "r")打开文件
//@EXECUTIONFLOW: 7、调用CSUDIFSSeek(hFile1, strlen("CSUDIFSSeek_0009")/2, CSUDIFS_SEEK_SET)改变文件指针，应该成功
//@EXECUTIONFLOW: 8、调用CSUDIFSSeek(hFile1, strlen("CSUDIFSSeek_0009"), CSUDIFS_SEEK_CUR)改变文件指针，应该失败
//@EXECUTIONFLOW: 9、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW:10、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSSeek_0009")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSSeek_0009(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSSeek_0009";
	int     nLen = strlen(szTest);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "创建并打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "写入数据失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "r")), "打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile1,  nLen/2, CSUDIFS_SEEK_SET), "文件指针seek失败\n");
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSSeek(hFile1,  nLen, CSUDIFS_SEEK_CUR), "文件指针越界不应该成功\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");
		}
	}

	return  CSUDI_TRUE;				
}

//@CASEGROUP: int CSUDIFSSeek(CSUDI_HANDLE hFile, long lOffset, unsigned int uOrigin)
//@DESCRIPTION: 文件write,seek、read等操作到结尾时返回值
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = hFile1，其中hFile1为CSUDIFSOpen()返回的合法句柄
//@INPUT: lOffset = 0
//@INPUT: uOrigin = CSFS_SEEK_SET
//@EXPECTATION: 返回值等于0，且读出的数据与写入一致
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSSeek_0001", "w+")创建一个文件，获得相应句柄hFile1
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile1, "CSUDIFSSeek_0010", strlen("CSUDIFSSeek_0010"))写入数据
//@EXECUTIONFLOW: 5、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 6、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSSeek_0010", "r")打开文件
//@EXECUTIONFLOW: 7、调用CSUDIFSSeek(hFile1, 0, CSUDIFS_SEEK_SET)重置文件指针
//@EXECUTIONFLOW: 8、调用CSUDIFSRead(hFile1, szBuf, sizeof(szBuf)-1)读取写入的数据，应该与写入时一致
//@EXECUTIONFLOW: 9、调用CSUDIFSSeek(hFile1, 0, CSUDIFS_SEEK_SET)重置文件指针到文件尾
//@EXECUTIONFLOW:10、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 11、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSSeek_0010")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSSeek_0010(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSSeek_0010";
	int     nLen = strlen(szTest);
	char   szBuf[100];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "创建并打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "写入数据失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "r")), "打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile1, 0, CSUDIFS_SEEK_SET), "重置文件失败\n");
	
	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSRead(hFile1, szBuf, sizeof(szBuf) -1), "读取数据失败\n");

	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(szTest, szBuf), "读出数据与写入时不一致\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile1, 0, CSUDIFS_SEEK_END), "重置文件失败\n");
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");
		}
	}
	return  CSUDI_TRUE;	
}

static char s_szSeekText[100 * 1024]	= {0};						// 静态全局变量，存放写入文件的内容
static char s_szSeekBuffer[100 * 1024]	= {0};						// 静态全局变量，存放从文件中读取的内容

//@CASEGROUP: int CSUDIFSSeek(CSUDI_HANDLE hFile, long lOffset, unsigned int uOrigin)
//@DESCRIPTION: uOrigin为CSUDIFS_SEEK_SET时，不断地定位数据、读出数据，并与原始数据进行比较
//@PRECONDITION: 1、系统中存在一个已挂载的分卷设备
//@INPUT: 1、hFile为使用CSUDIFSOpen()返回的文件句柄
//@INPUT: 2、s_szSeekText[100 * 1024] = {0, 1, ..., 127, 0, 1, ... 127, ...}为文件的实际内容
//@INPUT: 3、s_szSeekBuffer[100 * 1024] = {0}为存放读入数据的缓存
//@INPUT: 4、nTextLen = sizeof(s_szSeekText);
//@INPUT: 5、nGap = nTextLen/128 + 1(每次i增加nGap),最多调用128次CSUDIFSSeek()
//@INPUT: 6、uOrigin为CSUDIFS_SEEK_SET
//@EXPECTATION: 每一步都返回预期结果
//@EXECUTIONFLOW: 1、为s_szSeekText赋值{0, 1, ..., 127, 0, 1, ..., 127}，为s_szSeekBuffer赋值为{0}
//@EXECUTIONFLOW: 2、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 3、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 4、调用创建文件CSUDIFSOpen(分区挂载路径 + "cstc_fs_jvm_temp", "w+")，获得句柄hFile
//@EXECUTIONFLOW: 5、调用CSUDIFSWrite(hFile, s_szSeekText, nTextLen)写入数据
//@EXECUTIONFLOW: 6、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 7、调用CSUDIFSOpen(分区挂载路径 + "cstc_fs_jvm_temp", "r+")打开文件，获取句柄hFile
//@EXECUTIONFLOW: 8、调用CSUDIFSTell(hFile)获取当前指针位置，判断文件指针位置是否正确
//@EXECUTIONFLOW: 9、调用CSUDIFSRead(hFile, s_szSeekBuffer, nTextLen)读取文件的数据
//@EXECUTIONFLOW: 10、调用memcmp(s_szSeekBuffer, s_szSeekText, nTextLen)比较读入的数据与写入的数据是否一致
//@EXECUTIONFLOW: 11、调用memset(s_szSeekBuffer, 0, nTextLen)清零s_szSeekBuffer
//@EXECUTIONFLOW: 12、判断i是否小于nTextLen/2。若是，执行下一步操作；否则，跳转至第20步
//@EXECUTIONFLOW: 13、调用CSUDIFSSeek(hFile, nTextLen - 1 - i, CSUDIFS_SEEK_SET)将文件指针移动至nTextLen-1-i位置处
//@EXECUTIONFLOW: 14、调用CSUDIFSTell(hFile)获取当前指针位置，判断文件指针位置是否正确
//@EXECUTIONFLOW: 15、调用CSUDIFSSeek(hFile, i, CSUDIFS_SEEK_SET)将文件指针移动至i位置处
//@EXECUTIONFLOW: 16、调用CSUDIFSTell(hFile)获取当前指针位置，判断文件指针位置是否正确
//@EXECUTIONFLOW: 17、调用CSUDIFSRead(hFile, s_szSeekBuffer, nTextLen - 2 * i)获取文件的第i位置与第nTextLen-1-i位置之间的数据
//@EXECUTIONFLOW: 18、调用memcmp(s_szSeekBuffer, s_szSeekText + i, nTextLen - 2 * i)比较读入的数据与写入的数据是否一致。若相等，执行下一步操作；否则，跳转至第20步
//@EXECUTIONFLOW: 19、调用memset(s_szSeekBuffer, 0, nTextLen - 2 * i)清零s_szSeekBuffer，更新i += nGap，然后跳转至第12步
//@EXECUTIONFLOW: 20、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 21、调用CSUDIFSRemove(分区挂载路径 + "cstc_fs_jvm_temp")移除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSSeek_0011(void)
{
	int i				= 0;
	int nTempLen		= 0;
	int nTextLen		= (sizeof(s_szSeekText) > 102400) ? 102400 : sizeof(s_szSeekText);	// 最大不超过100K
	int nHalfTextLen	= nTextLen >> 1;			
	int nGap			= (nTextLen >> 7) + 1;		// nGap = nTextLen/128 + 1(i每次增加nGap)
	CSUDI_HANDLE hFile	= CSUDI_NULL;
	char szFileName []	= "CSUDIFSSeek_0011";			

	for (i = 0; i < nTextLen; i++)
	{
		s_szSeekText[i] = i % 128;
	}

	memset(s_szSeekBuffer, 0, sizeof(s_szSeekBuffer));

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "步骤2失败：获取分区挂载点失败");

	strcat(g_szVolumeMountPoint, szFileName);

	hFile = CSUDIFSOpen(g_szVolumeMountPoint, "w+");
	CSTK_ASSERT_TRUE_FATAL(hFile != CSUDI_NULL, "步骤4失败：创建并打开文件失败");

	CSTK_ASSERT_TRUE_FATAL(
		nTextLen == CSUDIFSWrite(hFile, s_szSeekText, nTextLen), 
		"步骤5失败：写入数据失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSClose(hFile) == 0, "步骤6失败：关闭文件失败");

	hFile = CSUDIFSOpen(g_szVolumeMountPoint, "r+");
	CSTK_ASSERT_TRUE_FATAL(hFile != CSUDI_NULL, "步骤7失败：打开文件失败");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIFSTell(hFile) == 0,
		"步骤8失败：获取文件当前位置失败");

	CSTK_ASSERT_TRUE_FATAL(
		nTextLen == CSUDIFSRead(hFile, s_szSeekBuffer, nTextLen),
		"步骤9失败：读取数据失败");

	CSTK_ASSERT_TRUE_FATAL(
		memcmp(s_szSeekBuffer, s_szSeekText, nTextLen) == 0,
		"步骤10失败：读出数据与写入时的数据不一致");

	memset(s_szSeekBuffer, 0, nTextLen);

	for (i = 0; i < nHalfTextLen; i += nGap)
	{
		CSTK_ASSERT_TRUE_FATAL(
			CSUDIFSSeek(hFile, nTextLen - 1 - i, CSUDIFS_SEEK_SET) == 0,
			"步骤13失败：移动文件指针失败");

		CSTK_ASSERT_TRUE_FATAL(
			CSUDIFSTell(hFile) == nTextLen - 1 - i,
			"步骤14失败：获取文件当前位置失败");

		CSTK_ASSERT_TRUE_FATAL(
			CSUDIFSSeek(hFile, i, CSUDIFS_SEEK_SET) == 0,
			"步骤15失败：移动文件指针失败");

		CSTK_ASSERT_TRUE_FATAL(
			CSUDIFSTell(hFile) == i,
			"步骤16失败：获取文件当前位置失败");

		nTempLen = nTextLen - 2 * i;
		CSTK_ASSERT_TRUE_FATAL(
			nTempLen == CSUDIFSRead(hFile, s_szSeekBuffer, nTempLen),
			"步骤17失败：读取数据失败");

		CSTK_ASSERT_TRUE_FATAL(
			memcmp(s_szSeekBuffer, s_szSeekText + i, nTempLen) == 0,
			"步骤18失败：读出数据与写入时的数据不一致");

		memset(s_szSeekBuffer, 0, nTempLen);
	}

	CSTK_FATAL_POINT
	{
		if (hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(
				CSUDIFSClose(hFile) == 0, 
				"步骤20失败：关闭文件失败");

			CSTK_ASSERT_TRUE(
				CSUDIFSRemove(g_szVolumeMountPoint) == 0, 
				"步骤21失败：移除文件失败\n");
		}
	}

	return  CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSSeek(CSUDI_HANDLE hFile, long lOffset, unsigned int uOrigin)
//@DESCRIPTION: uOrigin为CSUDIFS_SEEK_END时，不断地定位数据、读出数据，并与原始数据进行比较
//@PRECONDITION: 1、系统中存在一个已挂载的分卷设备
//@INPUT: 1、hFile为使用CSUDIFSOpen()返回的文件句柄
//@INPUT: 2、s_szSeekText[100 * 1024] = {0, 1, ..., 127, 0, 1, ... 127, ...}为文件的实际内容
//@INPUT: 3、s_szSeekBuffer[100 * 1024] = {0}为存放读入数据的缓存
//@INPUT: 4、nTextLen = sizeof(s_szSeekText);
//@INPUT: 5、nGap = nTextLen/128 + 1(i每次增加nGap),最多调用128次CSUDIFSSeek()
//@INPUT: 6、uOrigin为CSUDIFS_SEEK_END//@EXPECTATION: 每一步都返回预期结果
//@EXECUTIONFLOW: 1、为s_szSeekText赋值{0, 1, ..., 127, 0, 1, ..., 127}，为s_szSeekBuffer赋值为{0}
//@EXECUTIONFLOW: 2、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 3、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 4、调用创建文件CSUDIFSOpen(分区挂载路径 + "cstc_fs_jvm_temp", "w+")，获得句柄hFile
//@EXECUTIONFLOW: 5、调用CSUDIFSWrite(hFile, s_szSeekText, nTextLen)写入数据
//@EXECUTIONFLOW: 6、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 7、调用CSUDIFSOpen(分区挂载路径 + "cstc_fs_jvm_temp", "r+")打开文件，获取句柄hFile
//@EXECUTIONFLOW: 8、调用CSUDIFSTell(hFile)获取当前指针位置，判断文件指针位置是否正确
//@EXECUTIONFLOW: 9、调用CSUDIFSRead(hFile, s_szSeekBuffer, nTextLen)读取文件的数据
//@EXECUTIONFLOW: 10、调用memcmp(s_szSeekBuffer, s_szSeekText, nTextLen)比较读入的数据与写入的数据是否一致
//@EXECUTIONFLOW: 11、调用memset(s_szSeekBuffer, 0, nTextLen)清零s_szSeekBuffer
//@EXECUTIONFLOW: 12、判断i是否小于nTextLen/2。若是，执行下一步操作；否则，跳转至第20步
//@EXECUTIONFLOW: 13、调用CSUDIFSSeek(hFile, -1 * (i + 1), CSUDIFS_SEEK_END)将文件指针移动至nTextLen-1-i位置处
//@EXECUTIONFLOW: 14、调用CSUDIFSTell(hFile)获取当前指针位置，判断文件指针位置是否正确
//@EXECUTIONFLOW: 15、调用CSUDIFSSeek(hFile, -1 * (nTextLen - i), CSUDIFS_SEEK_END)将文件指针移动至i位置处
//@EXECUTIONFLOW: 16、调用CSUDIFSTell(hFile)获取当前指针位置，判断文件指针位置是否正确
//@EXECUTIONFLOW: 17、调用CSUDIFSRead(hFile, s_szSeekBuffer, nTextLen - 2 * i)获取文件的第i位置与第nTextLen-1-i位置之间的数据
//@EXECUTIONFLOW: 18、调用memcmp(s_szSeekBuffer, s_szSeekText + i, nTextLen - 2 * i)比较读入的数据与写入的数据是否一致。若相等，执行下一步操作；否则，跳转至第20步
//@EXECUTIONFLOW: 19、调用memset(s_szSeekBuffer, 0, nTextLen - 2 * i)清零s_szSeekBuffer，更新i += nGap，然后跳转至第12步
//@EXECUTIONFLOW: 20、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 21、调用CSUDIFSRemove(分区挂载路径 + "cstc_fs_jvm_temp")移除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSSeek_0012(void)
{
	int i				= 0;
	int nTempLen		= 0;
	int nTextLen		= (sizeof(s_szSeekText) > 102400) ? 102400 : sizeof(s_szSeekText);		// 最大不超过100K
	int nHalfTextLen	= nTextLen >> 1;			
	int nGap			= (nTextLen >> 7) + 1;		// nGap = nTextLen/128 + 1(i每次增加nGap)
	CSUDI_HANDLE hFile	= CSUDI_NULL;
	char szFileName []	= "CSUDIFSSeek_0012";			

	for (i = 0; i < nTextLen; i++)
	{
		s_szSeekText[i] = i % 128;
	}

	memset(s_szSeekBuffer, 0, sizeof(s_szSeekBuffer));

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "步骤2失败：获取分区挂载点失败");

	strcat(g_szVolumeMountPoint, szFileName);

	hFile = CSUDIFSOpen(g_szVolumeMountPoint, "w+");
	CSTK_ASSERT_TRUE_FATAL(hFile != CSUDI_NULL, "步骤4失败：创建并打开文件失败");

	CSTK_ASSERT_TRUE_FATAL(
		nTextLen == CSUDIFSWrite(hFile, s_szSeekText, nTextLen), 
		"步骤5失败：写入数据失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSClose(hFile) == 0, "步骤6失败：关闭文件失败");

	hFile = CSUDIFSOpen(g_szVolumeMountPoint, "r+");
	CSTK_ASSERT_TRUE_FATAL(hFile != CSUDI_NULL, "步骤7失败：打开文件失败");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIFSTell(hFile) == 0,
		"步骤8失败：获取文件当前位置失败");

	CSTK_ASSERT_TRUE_FATAL(
		nTextLen == CSUDIFSRead(hFile, s_szSeekBuffer, nTextLen),
		"步骤9失败：读取数据失败");

	CSTK_ASSERT_TRUE_FATAL(
		memcmp(s_szSeekBuffer, s_szSeekText, nTextLen) == 0,
		"步骤10失败：读出数据与写入时的数据不一致");

	memset(s_szSeekBuffer, 0, nTextLen);

	for (i = 0; i < nHalfTextLen; i += nGap)
	{
		// 将文件指针移动到第nTextLen-1-i位置处
		CSTK_ASSERT_TRUE_FATAL(
			CSUDIFSSeek(hFile, -1 * (i + 1), CSUDIFS_SEEK_END) == 0,
			"步骤13失败：移动文件指针失败");

		CSTK_ASSERT_TRUE_FATAL(
			CSUDIFSTell(hFile) == nTextLen - 1 - i,
			"步骤14失败：获取文件当前位置失败");

		// 将文件指针移动到第i位置处
		CSTK_ASSERT_TRUE_FATAL(
			CSUDIFSSeek(hFile, -1 * (nTextLen - i), CSUDIFS_SEEK_END) == 0,
			"步骤15失败：移动文件指针失败");

		CSTK_ASSERT_TRUE_FATAL(
			CSUDIFSTell(hFile) == i,
			"步骤16失败：获取文件当前位置失败");

		nTempLen = nTextLen - 2 * i;
		CSTK_ASSERT_TRUE_FATAL(
			nTempLen == CSUDIFSRead(hFile, s_szSeekBuffer, nTempLen),
			"步骤17失败：读取数据失败");

		CSTK_ASSERT_TRUE_FATAL(
			memcmp(s_szSeekBuffer, s_szSeekText + i, nTempLen) == 0,
			"步骤18失败：读出数据与写入时的数据不一致");

		memset(s_szSeekBuffer, 0, nTempLen);
	}

	CSTK_FATAL_POINT
	{
		if (hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(
				CSUDIFSClose(hFile) == 0, 
				"步骤20失败：关闭文件失败");

			CSTK_ASSERT_TRUE(
				CSUDIFSRemove(g_szVolumeMountPoint) == 0, 
				"步骤21失败：移除文件失败\n");
		}
	}

	return  CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSSeek(CSUDI_HANDLE hFile, long lOffset, unsigned int uOrigin)
//@DESCRIPTION: uOrigin为CSUDIFS_SEEK_CUR时，不断地定位数据、读出数据，并与原始数据进行比较
//@PRECONDITION: 1、系统中存在一个已挂载的分卷设备
//@INPUT: 1、hFile为使用CSUDIFSOpen()返回的文件句柄
//@INPUT: 2、s_szSeekText[100 * 1024] = {0, 1, ..., 127, 0, 1, ... 127, ...}为文件的实际内容
//@INPUT: 3、s_szSeekBuffer[100 * 1024] = {0}为存放读入数据的缓存
//@INPUT: 4、nTextLen = sizeof(s_szSeekText);
//@INPUT: 5、nGap = nTextLen/64 + 1(i每次增加nGap),最多调用128次CSUDIFSSeek()
//@INPUT: 6、uOrigin为CSUDIFS_SEEK_CUR//@EXPECTATION: 每一步都返回预期结果
//@EXECUTIONFLOW: 1、为s_szSeekText赋值{0, 1, ..., 127, 0, 1, ..., 127}，为s_szSeekBuffer赋值为{0}
//@EXECUTIONFLOW: 2、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 3、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 4、调用创建文件CSUDIFSOpen(分区挂载路径 + "cstc_fs_jvm_temp", "w+")，获得句柄hFile
//@EXECUTIONFLOW: 5、调用CSUDIFSWrite(hFile, s_szSeekText, nTextLen)写入数据
//@EXECUTIONFLOW: 6、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 7、调用CSUDIFSOpen(分区挂载路径 + "cstc_fs_jvm_temp", "r+")打开文件，获取句柄hFile
//@EXECUTIONFLOW: 8、调用CSUDIFSTell(hFile)获取当前指针位置，判断文件指针位置是否正确
//@EXECUTIONFLOW: 9、调用CSUDIFSRead(hFile, s_szSeekBuffer, nTextLen)读取文件的数据
//@EXECUTIONFLOW: 10、调用memcmp(s_szSeekBuffer, s_szSeekText, nTextLen)比较读入的数据与写入的数据是否一致
//@EXECUTIONFLOW: 11、调用memset(s_szSeekBuffer, 0, nTextLen)清零s_szSeekBuffer
//@EXECUTIONFLOW: 12、调用CSUDIFSSeek(hFile, 0, CSUDIFS_SEEK_SET)将文件指针移动至开始位置
//@EXECUTIONFLOW: 13、调用CSUDIFSTell(hFile)获取当前指针位置，判断文件指针位置是否正确
//@EXECUTIONFLOW: 14、判断i + Gap是否小于等于nTextLen。若是，执行下一步操作；否则，跳转至第22步
//@EXECUTIONFLOW: 15、调用CSUDIFSSeek(hFile, Gap, CSUDIFS_SEEK_CUR)将文件指针移动至i + nGap位置处
//@EXECUTIONFLOW: 16、调用CSUDIFSTell(hFile)获取当前指针位置，判断文件指针位置是否正确
//@EXECUTIONFLOW: 17、调用CSUDIFSSeek(hFile, -nGap, CSUDIFS_SEEK_CUR)将文件指针移动至i位置处
//@EXECUTIONFLOW: 18、调用CSUDIFSTell(hFile)获取当前指针位置，判断文件指针位置是否正确
//@EXECUTIONFLOW: 19、调用CSUDIFSRead(hFile, s_szSeekBuffer, nGap)获取文件的第i位置与第i + nGap位置之间的数据
//@EXECUTIONFLOW: 20、调用memcmp(s_szSeekBuffer, s_szSeekText + i, nGap)比较读入的数据与写入的数据是否一致。若是，执行下一步操作；否则，跳转至第26步
//@EXECUTIONFLOW: 21、调用memset(s_szSeekBuffer, 0, nGap)清零s_szSeekBuffer，更新i+=nGap，然后跳转至第14步
//@EXECUTIONFLOW: 22、调用CSUDIFSSeek(hFile, -i, CSUDIFS_SEEK_CUR)将文件指针移动至开始位置处
//@EXECUTIONFLOW: 23、调用CSUDIFSTell(hFile)获取当前指针位置，判断文件指针位置是否正确
//@EXECUTIONFLOW: 24、调用CSUDIFSRead(hFile, s_szSeekBuffer, nTextLen)读取文件的数据
//@EXECUTIONFLOW: 25、调用memcmp(s_szSeekBuffer, s_szSeekText, nTextLen)比较读入的数据与写入的数据是否一致
//@EXECUTIONFLOW: 26、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 27、调用CSUDIFSRemove(分区挂载路径 + "cstc_fs_jvm_temp")移除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSSeek_0013(void)
{
	int i				= 0;
	//int nTempLen		= 0;
	CSUDI_HANDLE hFile	= CSUDI_NULL;
	char szFileName []	= "CSUDIFSSeek_0013";
	int nTextLen		= (sizeof(s_szSeekText) > 102400) ? 102400 : sizeof(s_szSeekText);		// 最大不超过100K
	int nGap			= (nTextLen >> 7) + 1;		// nGap = nTextLen/128 + 1(i每次增加nGap)

	for (i = 0; i < nTextLen; i++)
	{
		s_szSeekText[i] = i % 128;
	}

	memset(s_szSeekBuffer, 0, sizeof(s_szSeekBuffer));

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "步骤2失败：获取分区挂载点失败");

	strcat(g_szVolumeMountPoint, szFileName);

	hFile = CSUDIFSOpen(g_szVolumeMountPoint, "w+");
	CSTK_ASSERT_TRUE_FATAL(hFile != CSUDI_NULL, "步骤4失败：创建并打开文件失败");

	CSTK_ASSERT_TRUE_FATAL(
		nTextLen == CSUDIFSWrite(hFile, s_szSeekText, nTextLen), 
		"步骤5失败：写入数据失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSClose(hFile) == 0, "步骤6失败：关闭文件失败");

	hFile = CSUDIFSOpen(g_szVolumeMountPoint, "r+");
	CSTK_ASSERT_TRUE_FATAL(hFile != CSUDI_NULL, "步骤7失败：打开文件失败");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIFSTell(hFile) == 0,
		"步骤8失败：获取文件当前位置失败");

	CSTK_ASSERT_TRUE_FATAL(
		nTextLen == CSUDIFSRead(hFile, s_szSeekBuffer, nTextLen),
		"步骤9失败：读取数据失败");

	CSTK_ASSERT_TRUE_FATAL(
		memcmp(s_szSeekBuffer, s_szSeekText, nTextLen) == 0,
		"步骤10失败：读出数据与写入时的数据不一致");

	memset(s_szSeekBuffer, 0, nTextLen);

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIFSSeek(hFile, 0, CSUDIFS_SEEK_SET) == 0,
		"步骤12失败：移动文件指针失败");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIFSTell(hFile) == 0,
		"步骤13失败：获取文件当前位置失败");

	for (i = 0; i + nGap <= nTextLen; i += nGap)
	{
		CSTK_ASSERT_TRUE_FATAL(
			CSUDIFSSeek(hFile, nGap, CSUDIFS_SEEK_CUR) == 0,
			"步骤15失败：移动文件指针失败");

		CSTK_ASSERT_TRUE_FATAL(
			CSUDIFSTell(hFile) == i + nGap,
			"步骤16失败：获取文件当前位置失败");

		CSTK_ASSERT_TRUE_FATAL(
			CSUDIFSSeek(hFile, -nGap, CSUDIFS_SEEK_CUR) == 0,
			"步骤17失败：移动文件指针失败");

		CSTK_ASSERT_TRUE_FATAL(
			CSUDIFSTell(hFile) == i,
			"步骤18失败：获取文件当前位置失败");

		CSTK_ASSERT_TRUE_FATAL(
			nGap == CSUDIFSRead(hFile, s_szSeekBuffer, nGap),
			"步骤19失败：读取数据失败");

		CSTK_ASSERT_TRUE_FATAL(
			memcmp(s_szSeekBuffer, s_szSeekText + i, nGap) == 0,
			"步骤20失败：读出数据与写入时的数据不一致");

		memset(s_szSeekBuffer, 0, nGap);
	}

	// 将文件指针移动至开始处
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIFSSeek(hFile, -i, CSUDIFS_SEEK_CUR) == 0,
		"步骤22失败：移动文件指针失败");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIFSTell(hFile) == 0,
		"步骤23失败：获取文件当前位置失败");

	CSTK_ASSERT_TRUE_FATAL(
		nTextLen == CSUDIFSRead(hFile, s_szSeekBuffer, nTextLen),
		"步骤24失败：读取数据失败");

	CSTK_ASSERT_TRUE_FATAL(
		memcmp(s_szSeekBuffer, s_szSeekText, nTextLen) == 0,
		"步骤25失败：读出数据与写入时的数据不一致");

	CSTK_FATAL_POINT
	{
		if (hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(
				CSUDIFSClose(hFile) == 0, 
				"步骤26失败：关闭文件失败");

			CSTK_ASSERT_TRUE(
				CSUDIFSRemove(g_szVolumeMountPoint) == 0, 
				"步骤27失败：移除文件失败\n");
		}
	}

	return  CSUDI_TRUE;
}
/****************************CSUDIFSSeek  End *******************************/

/****************************CSUDIFSLseek  Start *****************************/ 
//@CASEGROUP: int CSUDIFSLseek(CSUDI_HANDLE hFile,CSUDI_INT64 i64Offset,unsigned int uOrigin)
//@DESCRIPTION: 正常定位
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = hFile1，其中hFile1为CSUDIFSOpen()返回的合法句柄
//@INPUT: i64Offset = 0
//@INPUT: uOrigin = CSUDIFS_SEEK_SET
//@EXPECTATION: = 0
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSLseek_0001", "w+")创建和打开一个文件，得到句柄hFile1
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile1, "CSUDIFSLseek_0001", strlen("CSUDIFSLseek_0001"))写入数据
//@EXECUTIONFLOW: 5、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 6、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSLseek_0001", "r")打开文件
//@EXECUTIONFLOW: 7、调用CSUDIFSLseek(hFile1, 0, CSFS_SEEK_SET)重置文件指针
//@EXECUTIONFLOW: 8、调用CSUDIFSRead(hFile1, szBuf, sizeof(szBuf)-1)读取数据，应该与写入时的一致
//@EXECUTIONFLOW: 9、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 10、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSLseek_0001")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLseek_0001(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSLseek_0001";
	int     nLen = strlen(szTest);
	char   szBuf[100];
	CSUDI_INT64  i64Offset;
	
	ll_setZero(i64Offset);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "创建并打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "写入数据失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "r")), "打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSLseek(hFile1, i64Offset, CSUDIFS_SEEK_SET), "重置文件失败\n");
	
	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSRead(hFile1, szBuf, sizeof(szBuf) -1), "读取数据失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(szTest, szBuf), "读出数据与写入时不一致\n");

	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");
		}
	}

	return  CSUDI_TRUE;			
}

//@CASEGROUP: int CSUDIFSLseek(CSUDI_HANDLE hFile,CSUDI_INT64 i64Offset,unsigned int uOrigin)
//@DESCRIPTION: 以文件尾为基础向前偏移整个文件大小
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = hFile1，其中hFile1为CSUDIFSOpen()返回的合法句柄
//@INPUT: i64Offset = 0 - strlen("CSUDIFSLseek_0002")
//@INPUT: uOrigin = CSUDIFS_SEEK_CUR
//@EXPECTATION: 返回值等于0，且读出的数据与写入一致
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSLseek_0002", "w+")创建一个文件，获得相应句柄hFile1
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile1, "CSUDIFSLseek_0002", strlen("CSUDIFSLseek_0002"))写入数据
//@EXECUTIONFLOW: 5、调用CSUDIFSFlush(hFile1)同步到磁盘
//@EXECUTIONFLOW: 6、调用CSUDIFSLseek(hFile1, 0 - strlen("CSUDIFSLseek_0002"), CSUDIFS_SEEK_CUR)重置文件指针
//@EXECUTIONFLOW: 7、调用CSUDIFSRead(hFile1, szBuf, sizeof(szBuf)-1)读取写入的数据，应该与写入时一致
//@EXECUTIONFLOW: 8、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 9、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSSeek_0002")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLseek_0002(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSLseek_0002";
	int     nLen = strlen(szTest);
	char   szBuf[100];
	CSUDI_INT64  i64Offset;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "创建并打开文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "写入数据失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSFlush(hFile1), "同步到磁盘失败\n");
	
	i64Offset = int2ll(0-nLen);

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSLseek(hFile1, i64Offset, CSUDIFS_SEEK_CUR), "重置文件失败\n");
	
	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSRead(hFile1, szBuf, sizeof(szBuf) -1), "读取数据失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(szTest, szBuf), "读出数据与写入时不一致\n");

	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");
		}
	}

	return  CSUDI_TRUE;				
}

//@CASEGROUP: int CSUDIFSLseek(CSUDI_HANDLE hFile,CSUDI_INT64 i64Offset,unsigned int uOrigin)
//@DESCRIPTION: 定位空文件句柄
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = CSUDI_NULL
//@INPUT: i64Offset = 0 
//@INPUT: uOrigin = CSUDIFS_SEEK_SET
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSLseek(NULL, 0, CSUDIFS_SEEK_SET)设置文件指针，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLseek_0003(void)
{
	CSUDI_INT64 i64Offset;

	ll_setZero(i64Offset);
		
	CSTK_ASSERT_TRUE(-1 == CSUDIFSLseek(CSUDI_NULL, i64Offset, CSUDIFS_SEEK_SET), "重置不存在的句柄文件不应该成功\n");	

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSLseek(CSUDI_HANDLE hFile,CSUDI_INT64 i64Offset,unsigned int uOrigin)
//@DESCRIPTION: 定位不存在的文件句柄
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = 0x7FFFFFFF(假设0x7FFFFFFF为不存在的文件句柄)
//@INPUT: i64Offset = 10
//@INPUT: uOrigin = CSUDIFS_SEEK_SET
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSLseek(hFile, 10, CSFS_SEEK_SET)设置文件指针
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLseek_0004(void)
{
	CSUDI_INT64 i64Offset;

	ll_int2long(i64Offset, 10);

	CSTK_ASSERT_TRUE(-1 == CSUDIFSLseek((CSUDI_HANDLE)0x7FFFFFFF, i64Offset, CSUDIFS_SEEK_SET), "重置不存在的句柄文件不应该成功\n");	

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSLseek(CSUDI_HANDLE hFile,CSUDI_INT64 i64Offset,unsigned int uOrigin)
//@DESCRIPTION: 基准位置非法
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = hFile1，hFile1为CSUDIFSOpen()返回的合法句柄
//@INPUT: i64Offset = 0
//@INPUT: uOrigin = 0x7FFFFFFF
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载目录	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSLseek_0005", "w+")创建并打开一个文件，获得句柄hFile1
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile1, "CSUDIFSLseek_0005", strlen("CSUDIFSLseek_0005"))写入数据
//@EXECUTIONFLOW: 5、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 6、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSLseek_0005", "r")打开文件
//@EXECUTIONFLOW: 7、调用CSUDIFSLseek(hFile1, 0, 0x7FFFFFFF)设置文件指针，应当失败
//@EXECUTIONFLOW: 8、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 9、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSLseek_0005")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLseek_0005(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSLseek_0005";
	int     nLen = strlen(szTest);
	CSUDI_INT64 i64Offset;

	ll_setZero(i64Offset);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "创建并打开文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "写入数据失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "r")), "打开文件失败\n");
	
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSLseek(hFile1,  i64Offset, 0x7FFFFFFF), "基准位置非法重置文件不应该成功\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");
		}
	}

	return  CSUDI_TRUE;			
}

//@CASEGROUP: int CSUDIFSLseek(CSUDI_HANDLE hFile,CSUDI_INT64 i64Offset,unsigned int uOrigin)
//@DESCRIPTION: 定位的目标位置越界
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = hFile1，hFile1为CSUDIFSOpen()返回的合法句柄
//@INPUT: i64Offset = sizeof("CSUDIFSLseek_0006")+10
//@INPUT: uOrigin = CSUDIFS_SEEK_SET
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSLseek_0006", "w+")创建并打开一个文件，得到句柄hFile1
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile1, "CSUDIFSLseek_0006", strlen("CSUDIFSLseek_0006"))写入数据
//@EXECUTIONFLOW: 5、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 6、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSLseek_0006", "r")打开文件
//@EXECUTIONFLOW: 7、调用CSUDIFSLseek(hFile1, strlen("CSUDIFSLseek_0006") + 10, CSFS_SEEK_SET)设置文件指针，应当失败
//@EXECUTIONFLOW: 8、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 9、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSLseek_0006")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLseek_0006(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSLseek_0006";
	int     nLen = strlen(szTest);
	CSUDI_INT64 i64Offset;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "创建并打开文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "写入数据失败\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "r")), "打开文件失败\n");
	
	ll_int2long(i64Offset, (nLen+10));

	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSLseek(hFile1,  i64Offset, CSUDIFS_SEEK_SET), "文件指针越界不应该成功\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");
		}
	}

	return  CSUDI_TRUE;				
}
/****************************CSUDIFSLseek  End *******************************/

/****************************CSUDIFSTell  Start *****************************/ 
//@CASEGROUP: long CSUDIFSTell(CSUDI_HANDLE hFile)
//@DESCRIPTION: 正常获取文件当前位置
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = hFile1，hFile1为CSUDIFSOpen()返回的合法句柄
//@EXPECTATION: 返回值等于strlen("CSUDIFSTell_0001")-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSTell_0001", "w+")创建并打开一个文件，获得句柄hFile1
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile1, "CSUDIFSTell_0001", strlen("CSUDIFSTell_0001"))写入数据
//@EXECUTIONFLOW: 5、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 6、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSTell_0001", "r")打开文件
//@EXECUTIONFLOW: 7、调用CSUDIFSSeek(hFile1, 文件长度,  CSUDIFS_SEEK_CUR)移动文件指针
//@EXECUTIONFLOW: 8、调用CSUDIFSTell(hFile1)获取文件指针当前位置，应当等于strlen("CSUDIFSTell_0001")
//@EXECUTIONFLOW: 9、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 10、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSTell_0001")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSTell_0001(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSTell_0001";
	int     nLen = strlen(szTest);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "创建并打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "写入数据失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "r")), "打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile1,  nLen, CSUDIFS_SEEK_CUR), "移动文件指针失败\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSTell(hFile1), "获取文件当前位置失败\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");
		}
	}

	return  CSUDI_TRUE;		
}

//@CASEGROUP: long CSUDIFSTell(CSUDI_HANDLE hFile)
//@DESCRIPTION: 从空文件句柄获取当前位置
//@PRECONDITION: 
//@INPUT: hFile = CSUDI_NULL
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSTell(CSUDI_NULL)获取文件指针当前位置，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSTell_0002(void)
{
	CSTK_ASSERT_TRUE( -1 == CSUDIFSTell(CSUDI_NULL), "从空文件句柄获取文件指针当前位置不应该成功\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: long CSUDIFSTell(CSUDI_HANDLE hFile)
//@DESCRIPTION: 从不存在的文件句柄获取当前位置
//@PRECONDITION: 
//@INPUT: hFile = 0x7FFFFFFF(假设0x7FFFFFFF为不存在的文件句柄)
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSTell(0x7FFFFFFF)获取文件指针当前位置，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSTell_0003(void)
{
	CSTK_ASSERT_TRUE( -1 == CSUDIFSTell((CSUDI_HANDLE)0x7FFFFFFF), "获取不存在的文件句柄的当前位置不应当成功\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: long CSUDIFSTell(CSUDI_HANDLE hFile)
//@DESCRIPTION: 获取超过2G的大文件的文件当前位置
//@PRECONDITION: 1、系统中存在一个已挂载的大于2G的分卷设备
//@PRECONDITION: 2、文件系统支持2G以上的大文件
//@INPUT: hFile = hFile1，hFile1为CSUDIFSOpen()返回的合法句柄
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSTell_0004", "w+")创建并打开一个文件，得到句柄hFile1
//@EXECUTIONFLOW: 4、循环调用CSUDIFSWrite(hFile1)写入超过2G的数据，根据磁盘速度不一样，可能超过10分钟，需耐心等待
//@EXECUTIONFLOW: 5、调用CSUDIFSTell(hFile1)获取文件指针当前位置
//@EXECUTIONFLOW: 6、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 7、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSTell_0004")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSTell_0004(void)
{
	CSUDIFSFileStat_S  sFileStat;
	//手动测试
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSTell_0004";  //16字节
	//int     nLen = strlen(szTest);
	int     nMaxCount =  1024 + 50;  //每次写1M字节，1024*2次为2G，1024*2 + 50次则超过了2G
	int nTemp = nMaxCount;
	int rateTmp = -1;
	//int     nMaxCount =  1024 * 128;
	int     nLen1 = 1024*1024 * 2;
	char *szTest1 = (char*)CSUDIOSMalloc(nLen1);
	memset(szTest1, (int)'a', nLen1);
	CSFSPrint("请确定U盘容量是否大于2G，是请按(Y) / 否请按(N)\n");

	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"U盘容量小于2G\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "创建并打开文件失败\n");

	CSFSPrint("根据磁盘速度不一样，此用例用时可能超过10分钟，请耐心等待...\n");
	
	while(nTemp -- > 0)
	{
		CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIFSWrite(hFile1, szTest1, nLen1), "写入数据失败\n");

		if((nMaxCount - nTemp) * 100 / nMaxCount > rateTmp)
		{
			rateTmp = (nMaxCount - nTemp) * 100 / nMaxCount;
			
			CSFSPrint("rate: %d%%\n",rateTmp);
		}
	}
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSFstat(hFile1, &sFileStat), "获取文件信息失败\n");
	CSFSPrint("文件大小为: %d M\n", sFileStat.m_lSize / 1024 /1024);
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSTell(hFile1), "获取文件指针当前位置不应该成功\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSFSPrint("关闭文件\n");
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");
		}
	}

	return  CSUDI_TRUE;			
}
/****************************CSUDIFSTell  End *******************************/

/****************************CSUDIFSLtell  Start *****************************/ 
//@CASEGROUP: CSUDI_INT64 CSUDIFSLtell(CSUDI_HANDLE hFile)
//@DESCRIPTION: 正常获取文件当前位置
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = hFile1, hFile1为CSUDIFSOpen()返回的合法句柄
//@EXPECTATION: 返回值等于strlen("CSUDIFSLtell_0001")-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSLtell_0001", "w+")创建并打开一个文件，得到句柄hFile1
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile1, "CSUDIFSLtell_0001", strlen("CSUDIFSLtell_0001"))写入数据
//@EXECUTIONFLOW: 5、CSUDIFSFlush(hFile1)写入文件
///@EXECUTIONFLOW: 6、调用CSUDIFSSeek(hFile1, -1,  CSUDIFS_SEEK_CUR)移动文件指针
//@EXECUTIONFLOW: 7、调用CSUDIFSLtell(hFile1)获得文件指针当前位置，应当为strlen("CSUDIFSLtell_0001")-1
//@EXECUTIONFLOW: 8、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 9、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSLtell_0001")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLtell_0001(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSLtell_0001";
	int     nLen = strlen(szTest);
	CSUDI_INT64  i64OffsetA;
	CSUDI_INT64  i64OffsetB;
	CSUDI_BOOL   bEqual = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "创建并打开文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "写入数据失败\n");
	
	CSTK_ASSERT_TRUE(CSUDI_SUCCESS== CSUDIFSFlush(hFile1), "同步到磁盘不成功\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile1,  -1, CSUDIFS_SEEK_CUR), "移动文件指针失败\n");

	ll_int2long(i64OffsetA, (nLen -1));
	i64OffsetB = CSUDIFSLtell(hFile1);
	bEqual = ll_eq(i64OffsetA, i64OffsetB);

	CSTK_ASSERT_TRUE_FATAL(bEqual, "获取文件指针当前位置失败\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");
		}
	}

	return  CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_INT64 CSUDIFSLtell(CSUDI_HANDLE hFile)
//@DESCRIPTION: 从空文件句柄获取当前位置
//@PRECONDITION: 
//@INPUT: hFile = CSUDI_NULL
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSLtell(CSUDI_NULL)获取文件当前位置，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLtell_0002(void)
{
	CSUDI_INT64  i64OffsetA;
	CSUDI_INT64  i64OffsetB;
	CSUDI_BOOL   bEqual = CSUDI_FALSE;

	ll_int2long(i64OffsetA, (-1));
	i64OffsetB = CSUDIFSLtell(CSUDI_NULL);
	bEqual = ll_eq(i64OffsetA, i64OffsetB);
	
	CSTK_ASSERT_TRUE(bEqual, "从空文件句柄获取文件指针当前位置不应该成功\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_INT64 CSUDIFSLtell(CSUDI_HANDLE hFile)
//@DESCRIPTION: 从不存在的文件句柄获取当前位置
//@PRECONDITION: 
//@INPUT: hFile = 0x7FFFFFFF(假设0x7FFFFFFF为不存在的文件句柄)
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSLtell(0x7FFFFFFF)获取文件当前位置，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLtell_0003(void)
{
	CSUDI_INT64  i64OffsetA;
	CSUDI_INT64  i64OffsetB;
	CSUDI_BOOL   bEqual = CSUDI_FALSE;

	ll_int2long(i64OffsetA, (-1));
	i64OffsetB = CSUDIFSLtell(CSUDI_NULL);
	bEqual = ll_eq(i64OffsetA, i64OffsetB);

	CSTK_ASSERT_TRUE(bEqual, "获取不存在的文件句柄的当前位置不应当成功\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_INT64 CSUDIFSLtell(CSUDI_HANDLE hFile)
//@DESCRIPTION: 获取超过2G的大文件的文件当前位置
//@PRECONDITION: 1、系统中存在一个已挂载大于2G的分卷设备
//@PRECONDITION: 2、文件系统支持2G以上的大文件
//@INPUT: hFile = hFile1, hFile1为CSUDIFSOpen()返回的合法句柄
//@EXPECTATION: 返回值等于(1024*1024*128 + 1)*strlen("CSUDIFSLtell_0004")
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSLtell_0004", "w+")创建并打开一个文件，得到句柄hFile1
//@EXECUTIONFLOW: 4、循环调用CSUDIFSWrite(hFile1, "CSUDIFSLtell_0004", strlen("CSUDIFSLtell_0004"))nMaxCount次，nMaxCount = 1024*1024*128 + 1
//@EXECUTIONFLOW: 5、调用CSUDIFSLtell(hFile1)获得文件当前位置，应当等于(1024*1024*128 + 1)*strlen("CSUDIFSLtell_0004")
//@EXECUTIONFLOW: 6、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 7、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSLtell_0004")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLtell_0004(void)
{
	//手动测试
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSLtell_0004";  //17字节
	int     nLen = 1024*1024*2;
	char *szTest1 = (char*)CSUDIOSMalloc(nLen);
	int     nMaxCount =  1024+50;  //每次写2M 字节，1024+50次则超过了2G
	int     nTemp = nMaxCount;
	int rateTmp = -1;
	CSUDI_INT64  i64OffsetA;
	CSUDI_INT64  i64OffsetB;
	CSUDI_BOOL   bEqual = CSUDI_FALSE;

	memset(szTest1, 1, nLen);

	CSFSPrint("请确定U盘容量是否大于2G，是请按(Y)/否请按(N)\n");

	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"U盘容量小于2G\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "创建并打开文件失败\n");

	CSFSPrint("根据磁盘速度不一样，此用例用时可能超过10分钟，请耐心等待...\n");
	
	while(nTemp -- > 0)
	{
		CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest1, nLen), "写入数据失败\n");

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
	
	CSTK_ASSERT_TRUE_FATAL(bEqual, "获取文件当前位置失败\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");
		}
	}

	return  CSUDI_TRUE;
}
/****************************CSUDIFSLtell  End *******************************/

/****************************CSUDIFSFlush  Start *****************************/ 
//@CASEGROUP: int CSUDIFSFlush(CSUDI_HANDLE hFile)
//@DESCRIPTION: 正常flush文件
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = hFile1, hFile1为CSUDIFSOpen()返回的合法句柄
//@EXPECTATION: 返回值等于0
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径直	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSFlush_0001", "w+")创建并打开一个文件，得到句柄hFile1
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile1, "CSUDIFSFlush_0001", strlen("CSUDIFSFlush_0001"))写入数据
//@EXECUTIONFLOW: 5、调用CSUDIFSFlush(hFile1)同步到磁盘，应当成功
//@EXECUTIONFLOW: 6、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 7、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSFlush_0001")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFlush_0001(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSFlush_0001";  
	int     nLen = strlen(szTest);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w+")), "创建并打开文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "写入数据失败\n");
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSFlush(hFile1), "同步到磁盘失败\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");
		}
	}

	return  CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSFlush(CSUDI_HANDLE hFile)
//@DESCRIPTION: flush空文件句柄
//@PRECONDITION: 
//@INPUT: hFile = CSUDI_NULL
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSFlush(CSUDI_NULL)同步到磁盘，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFlush_0002(void)
{
	CSTK_ASSERT_TRUE(-1 == CSUDIFSFlush(CSUDI_NULL), "同步到磁盘不应该成功\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSFlush(CSUDI_HANDLE hFile)
//@DESCRIPTION: flush不存在的文件句柄
//@PRECONDITION: 
//@INPUT: hFile = 0x7FFFFFFF(假设0x7FFFFFFF为不存在的文件句柄)
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSFlush(0x7FFFFFFF)同步到磁盘，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFlush_0003(void)
{
	CSTK_ASSERT_TRUE(-1 == CSUDIFSFlush((CSUDI_HANDLE)0x7FFFFFFF), "同步到磁盘不应该成功\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}
/****************************CSUDIFSFlush  End *******************************/

/****************************CSUDIFSRemove  Start *****************************/ 
//@CASEGROUP: int CSUDIFSRemove(const char * pcFileName)
//@DESCRIPTION: 移除一个已存在的文件
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcFileName = 分区挂载路径 + "CSUDIFSRemove_0001"
//@EXPECTATION: 返回值等于0
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSRemove_0001", "w")创建一个文件，并得到句柄hFile1
//@EXECUTIONFLOW: 4、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 5、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSRemove_0001")删除文件
//@EXECUTIONFLOW: 6、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSRemove_0001", "r")，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRemove_0001(void)
{
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	char   szTest[] = "CSUDIFSRemove_0001";  
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(g_szVolumeMountPoint, "w")), "创建并打开文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件失败\n");

			CSTK_ASSERT_TRUE(CSUDI_NULL == CSUDIFSOpen(g_szVolumeMountPoint, "r"), "打开不存在的文件不应该成功\n");
		}
	}

	return  CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSRemove(const char * pcFileName)
//@DESCRIPTION: 移除不存在的文件
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcFileName = 分区挂载路径 + "CSUDIFSRemove_0002"
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSRemove_0002")删除文件，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRemove_0002(void)
{
	char   szTest[] = "CSUDIFSRemove_0002";  
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE(-1 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除不存在的文件不应该成功\n");

	CSTK_FATAL_POINT
	{
		;
	}
	
	return  CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSRemove(const char * pcFileName)
//@DESCRIPTION: 移除文件夹
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcFileName = 分区挂载路径 + "CSUDIFSRemove_0003"
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径 
//@EXECUTIONFLOW: 3、调用CSUDIFSMkdir(分区挂载路径 + "CSUDIFSRemove_0003", 0)创建文件夹
//@EXECUTIONFLOW: 4、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSRemove_0003")删除文件夹，应当失败
//@EXECUTIONFLOW: 5、调用CSUDIFSRmdir(分区挂载路径 + "CSUDIFSRemove_0003")删除文件夹，应当成功
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRemove_0003(void)
{
	char   szTest[] = "CSUDIFSRemove_0003";  
	int     nMkDirRet = -1;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE_FATAL(0 == (nMkDirRet = CSUDIFSMkdir(g_szVolumeMountPoint, 0)), "创建文件夹失败\n");
	CSTK_ASSERT_TRUE_FATAL(-1 ==CSUDIFSRemove(g_szVolumeMountPoint), "移除文件夹不应该成功\n");

	CSTK_FATAL_POINT
	{
		if(nMkDirRet == 0)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSRmdir(g_szVolumeMountPoint), "删除文件夹失败\n");
		}
	}
	
	return  CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSRemove(const char * pcFileName)
//@DESCRIPTION: 移除空指针
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcFileName = CSUDI_NULL
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSRemove(CSUDI_NULL)删除文件，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRemove_0004(void)
{
	CSTK_ASSERT_TRUE(-1 ==CSUDIFSRemove(CSUDI_NULL), "移除不存在的文件不应该成功\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}
/****************************CSUDIFSRemove  End *******************************/

/****************************CSUDIFSRemove  Start *****************************/ 
//@CASEGROUP: int CSUDIFSRename(const char * pcOldPath, const char * pcNewPath)
//@DESCRIPTION:  同目录下重命名文件
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcOldPath = 分区挂载路径 + "CSUDIFSRename_0001a"
//@INPUT: pcNewPath = 分区挂载路径 + "CSUDIFSRename_0001b"
//@EXPECTATION: 返回值等于0
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSRename_0001a", "w")创建并打开文件，得到句柄hFileA
//@EXECUTIONFLOW: 4、调用CSUDIFSClose(hFileA)关闭文件
//@EXECUTIONFLOW: 5、调用CSUDIFSRename(分区挂载路径 + "CSUDIFSRename_0001a", 分区挂载路径 + "CSUDIFSRename_0001b")重命名文件
//@EXECUTIONFLOW: 6、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSRename_0001a", "r")打开文件，应该失败
//@EXECUTIONFLOW: 7、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSRename_0001b", "r")打开文件，得到句柄hFileB
//@EXECUTIONFLOW: 8、调用CSUDIFSClose(hFileB)关闭文件
//@EXECUTIONFLOW: 9、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSRename_0001b")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRename_0001(void)
{
	char   szTestA[] = "CSUDIFSRename_0001a";  
	char   szTestB[] = "CSUDIFSRename_0001b";  
	CSUDI_HANDLE    hFileA = CSUDI_NULL;
	CSUDI_HANDLE    hFileB = CSUDI_NULL;
	char   szFilePathA[CSUDI_MAX_FILE_NAME_LEN];
	char   szFilePathB[CSUDI_MAX_FILE_NAME_LEN];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePathA, 0, CSUDI_MAX_FILE_NAME_LEN);
	memset(szFilePathB, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePathA, g_szVolumeMountPoint);
	strcpy(szFilePathB, g_szVolumeMountPoint);
	strcat(szFilePathA, szTestA);
	strcat(szFilePathB, szTestB);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFileA = CSUDIFSOpen(szFilePathA, "w")), "创建并打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFileA), "关闭文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRename(szFilePathA, szFilePathB), "重命名文件失败\n");

	hFileA = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == CSUDIFSOpen(szFilePathA, "r"), "打开文件不应该成功\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFileB = CSUDIFSOpen(szFilePathB, "r")) , "打开文件失败\n");
	CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFileB), "关闭文件失败\n");
	CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePathB), "移除文件失败\n");
	
	CSTK_FATAL_POINT
	{
		if(hFileA != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePathA), "移除文件失败\n");
		}
	}
	
	return  CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSRename(const char * pcOldPath, const char * pcNewPath)
//@DESCRIPTION:  重命名不存在的文件
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcOldPath = 分区挂载路径 + "CSUDIFSRename_0002a"
//@INPUT: pcNewPath = 分区挂载路径 + "CSUDIFSRename_0002b"
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSRename(分区挂载路径 + "CSUDIFSRename_0002a", 分区挂载路径 + "CSUDIFSRename_0002b")重命名文件，应当失败
//@EXECUTIONFLOW: 4、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSRename_0002a", "r")打开文件，应该失败
//@EXECUTIONFLOW: 5、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSRename_0002b", "r")打开文件，应该失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRename_0002(void)
{
	char   szTestA[] = "CSUDIFSRename_0002a";  
	char   szTestB[] = "CSUDIFSRename_0002b";  
	char   szFilePathA[CSUDI_MAX_FILE_NAME_LEN];
	char   szFilePathB[CSUDI_MAX_FILE_NAME_LEN];
	
	CSTK_ASSERT_TRUE(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePathA, 0, CSUDI_MAX_FILE_NAME_LEN);
	memset(szFilePathB, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePathA, g_szVolumeMountPoint);
	strcpy(szFilePathB, g_szVolumeMountPoint);
	strcat(szFilePathA, szTestA);
	strcat(szFilePathB, szTestB);	

	CSTK_ASSERT_TRUE(-1 == CSUDIFSRename(szFilePathA, szFilePathB), "重命名不存在的文件不应该成功\n");

	CSTK_ASSERT_TRUE(CSUDI_NULL == CSUDIFSOpen(szFilePathA, "r"), "打开文件不应该成功\n");

	CSTK_ASSERT_TRUE(CSUDI_NULL == CSUDIFSOpen(szFilePathB, "r"), "打开文件不应该成功\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSRename(const char * pcOldPath, const char * pcNewPath)
//@DESCRIPTION:  重命名空指针
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcOldPath = CSUDI_NULL
//@INPUT: pcNewPath = 分区挂载路径 + "CSUDIFSRename_0003"
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSRename(CSUDI_NULL, 分区挂载路径 + "CSUDIFSRename_0003")重命名文件，应该失败
//@EXECUTIONFLOW: 4、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSRename_0003", "r")打开文件，应该失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRename_0003(void)
{
	char   szTest[] = "CSUDIFSRename_0003";  
	
	CSTK_ASSERT_TRUE(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	strcat(g_szVolumeMountPoint, szTest);

	CSTK_ASSERT_TRUE(-1 == CSUDIFSRename(CSUDI_NULL, g_szVolumeMountPoint), "重命名不存在的文件不应该成功\n");

	CSTK_ASSERT_TRUE(CSUDI_NULL == CSUDIFSOpen(g_szVolumeMountPoint, "r"), "打开文件不应该成功\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSRename(const char * pcOldPath, const char * pcNewPath)
//@DESCRIPTION:  重命名文件夹
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcOldPath = 分区挂载路径 + "CSUDIFSRename_0004a"
//@INPUT: pcNewPath = 分区挂载路径 + "CSUDIFSRename_0004b"
//@EXPECTATION: 返回值等于0
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSMkdir(分区挂载路径 + "CSUDIFSRename_0004a", 0)创建目录
//@EXECUTIONFLOW: 4、调用CSUDIFSRename(分区挂载路径 + "CSUDIFSRename_0004a", 分区挂载路径 + "CSUDIFSRename_0004b")  
//@EXECUTIONFLOW: 5、调用CSUDIFSRmdir(分区挂载路径 + "CSUDIFSRename_0004b")删除目录,应该成功
//@EXECUTIONFLOW: 6、调用CSUDIFSRmdir(分区挂载路径 + "CSUDIFSRename_0004a")删除目录，应该失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRename_0004(void)
{
	char   szTestA[] = "CSUDIFSRename_0004a";  
	char   szTestB[] = "CSUDIFSRename_0004b";  
	char   szFilePathA[CSUDI_MAX_FILE_NAME_LEN];
	char   szFilePathB[CSUDI_MAX_FILE_NAME_LEN];
	
	CSTK_ASSERT_TRUE(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePathA, 0, CSUDI_MAX_FILE_NAME_LEN);
	memset(szFilePathB, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePathA, g_szVolumeMountPoint);
	strcpy(szFilePathB, g_szVolumeMountPoint);
	strcat(szFilePathA, szTestA);
	strcat(szFilePathB, szTestB);

	CSTK_ASSERT_TRUE(0 == CSUDIFSMkdir(szFilePathA, 0), "创建目录失败\n");
	CSTK_ASSERT_TRUE(0 == CSUDIFSRename(szFilePathA, szFilePathB), "重命名文件夹失败\n");
	CSTK_ASSERT_TRUE(0 == CSUDIFSRmdir(szFilePathB), "删除文件夹失败\n");
	CSTK_ASSERT_TRUE(0 != CSUDIFSRmdir(szFilePathA), "删除文件夹不应该成功\n");
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSRename(const char * pcOldPath, const char * pcNewPath)
//@DESCRIPTION:  重命名文件到另一目录，原目录下文件被删除
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcOldPath = 分区挂载路径 + "CSUDIFSRename_0005a"
//@INPUT: pcNewPath = 分区挂载路径 + "CSUDIFSRename_0005_dir"+"CSUDIFSRename_0005b"
//@EXPECTATION: 返回值等于0
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSRename_0005", "w")创建并打开文件，得到句柄hFile1
//@EXECUTIONFLOW: 4、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 5、调用CSUDIFSMkdir(分区挂载路径 + "CSUDIFSRename_0005_dir", 0)创建目录
//@EXECUTIONFLOW: 6、调用CSUDIFSRename(分区挂载路径 + "CSUDIFSRename_0005", 分区挂载路径 + "CSUDIFSRename_0005_dir"+"CSUDIFSRename_0005")，应当成功  
//@EXECUTIONFLOW: 7、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSRename_0005_dir"+"CSUDIFSRename_0005")删除文件,应当成功
//@EXECUTIONFLOW: 8、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSRename_0005")删除文件,应当失败
//@EXECUTIONFLOW: 9、调用CSUDIFSRmdir(分区挂载路径 + "CSUDIFSRename_0005_dir")删除目录
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRename_0005(void)
{
	char   szTestA[] = "CSUDIFSRename_0004a";  
	char   szTestB[] = "CSUDIFSRename_0004b";  
	char   szFilePathA[CSUDI_MAX_FILE_NAME_LEN];
	char   szFilePathB[CSUDI_MAX_FILE_NAME_LEN];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePathA, 0, CSUDI_MAX_FILE_NAME_LEN);
	memset(szFilePathB, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePathA, g_szVolumeMountPoint);
	strcpy(szFilePathB, g_szVolumeMountPoint);
	strcat(szFilePathA, szTestA);
	strcat(szFilePathB, szTestB);

	CSTK_ASSERT_TRUE(0 == CSUDIFSMkdir(szFilePathA, 0), "创建目录失败\n");

	CSTK_ASSERT_TRUE(0 == CSUDIFSRename(szFilePathA, szFilePathB), "重命名文件夹失败\n");

	CSTK_ASSERT_TRUE(0 == CSUDIFSRmdir(szFilePathB), "删除文件夹失败\n");

	CSTK_ASSERT_TRUE(0 != CSUDIFSRmdir(szFilePathA), "删除文件夹不应该成功\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSRename(const char * pcOldPath, const char * pcNewPath)
//@DESCRIPTION:  重命名文件到的新目录不存在
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcOldPath = 分区挂载路径 + "CSUDIFSRename_0006a"
//@INPUT: pcNewPath = 分区挂载路径 + "CSUDIFSRename_0006_dir"+"CSUDIFSRename_0006b"
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSRename_0006a", "w")创建并打开文件文件，得到文件句柄hFile
//@EXECUTIONFLOW: 4、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 5、调用CSUDIFSRename(分区挂载路径 + "CSUDIFSRename_0006a", 分区挂载路径 + "CSUDIFSRename_0006_dir"+"CSUDIFSRename_0006b")重命名，应当失败  
//@EXECUTIONFLOW: 6、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSRename_0006_dir"+"CSUDIFSRename_0006b")打开文件，应当失败
//@EXECUTIONFLOW: 7、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSRename_0006a")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRename_0006(void)
{
	char   szTestA[] = "CSUDIFSRename_0004a";  
	char   szTestB[] = "CSUDIFSRename_0006_dir/CSUDIFSRename_0004b";  
	char   szFilePathA[CSUDI_MAX_FILE_NAME_LEN];
	char   szFilePathB[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE  hFile = CSUDI_NULL;  
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePathA, 0, CSUDI_MAX_FILE_NAME_LEN);
	memset(szFilePathB, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePathA, g_szVolumeMountPoint);
	strcpy(szFilePathB, g_szVolumeMountPoint);
	strcat(szFilePathA, szTestA);
	strcat(szFilePathB, szTestB);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePathA, "w")), "创建并打开文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSRename(szFilePathA, szFilePathB), "重命名文件不应该成功\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == CSUDIFSOpen(szFilePathB, "r"), "打开文件不应该成功\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePathA), "移除文件失败\n");
		}
	}
	
	return  CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSRename(const char * pcOldPath, const char * pcNewPath)
//@DESCRIPTION:  重命名为空指针
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcOldPath = 分区挂载路径 + "CSUDIFSRename_0007"
//@INPUT: pcNewPath = CSUDI_NULL
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSRename_0007", "w")创建并打开文件，获得句柄hFile
//@EXECUTIONFLOW: 4、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 5、调用CSUDIFSRename(分区挂载路径 + "CSUDIFSRename_0007", CSUDI_NULL)重命名文件，应当失败
//@EXECUTIONFLOW: 6、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSRename_0007")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRename_0007(void)
{
	char   szTest[] = "CSUDIFSRename_0007";  
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE  hFile = CSUDI_NULL;  
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w")), "创建并打开文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSRename(szFilePath, CSUDI_NULL), "重命名文件不应该成功\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "移除文件失败\n");
		}
	}
	
	return  CSUDI_TRUE;		
}

//@CASEGROUP: int CSUDIFSRename(const char * pcOldPath, const char * pcNewPath)
//@DESCRIPTION:  重命名为空字符串
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcOldPath = 分区挂载路径 + "CSUDIFSRename_0008"
//@INPUT: pcNewPath = 分区挂载路径 + ""
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSRename_0008", "w")创建并打开文件，得到句柄hFile
//@EXECUTIONFLOW: 4、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 5、调用CSUDIFSRename(分区挂载路径 + "CSUDIFSRename_0008", 分区挂载路径 + "")重命名文件应该失败
//@EXECUTIONFLOW: 6、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSRename_0008"删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRename_0008(void)
{
	char   szTest[] = "CSUDIFSRename_0008";  
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE  hFile = CSUDI_NULL;  
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w")), "创建并打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSRename(szFilePath, g_szVolumeMountPoint), "重命名文件不应该成功\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "移除文件失败\n");
		}
	}
	
	return  CSUDI_TRUE;		
}

//@CASEGROUP: int CSUDIFSRename(const char * pcOldPath, const char * pcNewPath)
//@DESCRIPTION:  重命名的文件在相应目录下已经存在
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcOldPath = 分区挂载路径 + "CSUDIFSRename_0009a"
//@INPUT: pcNewPath = 分区挂载路径 + "CSUDIFSRename_0009b"
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(pcOldPath, "w")创建并打开文件，得到句柄hFile1
//@EXECUTIONFLOW: 4、调用CSUDIFSOpen(pcNewPath, "w")创建并打开文件，得到句柄hFile2
//@EXECUTIONFLOW: 5、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 6、调用CSUDIFSClose(hFile2)关闭文件
//@EXECUTIONFLOW: 7、调用CSUDIFSRename(分区挂载路径 + "CSUDIFSRename_0009a", 分区挂载路径 + "CSUDIFSRename_0009b")，应当失败  
//@EXECUTIONFLOW: 8、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSRename_0009a")删除文件
//@EXECUTIONFLOW: 9、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSRename_0009b")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRename_0009(void)
{
	char   szTestA[] = "CSUDIFSRename_0009a";  
	char   szTestB[] = "CSUDIFSRename_0009b";  
	char   szFilePathA[CSUDI_MAX_FILE_NAME_LEN];
	char   szFilePathB[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE  hFile1 = CSUDI_NULL;  
	CSUDI_HANDLE  hFile2 = CSUDI_NULL;  
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePathA, 0, CSUDI_MAX_FILE_NAME_LEN);
	memset(szFilePathB, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePathA, g_szVolumeMountPoint);
	strcpy(szFilePathB, g_szVolumeMountPoint);
	strcat(szFilePathA, szTestA);
	strcat(szFilePathB, szTestB);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(szFilePathA, "w")), "创建并打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile2 = CSUDIFSOpen(szFilePathB, "w")), "创建并打开文件失败\n");
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile2), "关闭文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSRename(szFilePathA, szFilePathB), "重命名文件不应该成功\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePathA), "移除文件失败\n");
		}
		if(hFile2 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePathB), "移除文件失败\n");
		}
	}
	
	return  CSUDI_TRUE;		
}
/****************************CSUDIFSRemove  End *******************************/

/****************************CSUDIFSTruncate  Start *****************************/ 
//@CASEGROUP: int CSUDIFSTruncate(const char * pcPathName, unsigned long ulLength)
//@DESCRIPTION:  将文件截小
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcPathName = 分区挂载路径 + "CSUDIFSTruncate_0001"
//@INPUT: ulLength = ulLength1, 其中ulLength1小于文件大小1
//@EXPECTATION: 返回值等于0，且文件大小变为ulLength1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSTruncate_0001", "w+")创建并打开文件，得到句柄hFile
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile, "CSUDIFSTruncate_0001", strlen("CSUDIFSTruncate_0001"))写入数据
//@EXECUTIONFLOW: 5、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 6、调用CSUDIFSTruncate(分区挂载路径 + "CSUDIFSTruncate_0001", strlen("CSUDIFSTruncate_0001")/2)截小文件
//@EXECUTIONFLOW: 7、调用CSUDIFSStat(分区挂载路径 + "CSUDIFSTruncate_0001", psFileStat)获得文件大小，与ulLength1比较，应该相等
//@EXECUTIONFLOW: 8、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSTruncate_0001")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSTruncate_0001(void)
{
	char   szTest[] = "CSUDIFSTruncate_0001";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE  hFile = CSUDI_NULL;  
	int  nLen = strlen(szTest);
	int  ulLength1 = nLen/2;
	CSUDIFSFileStat_S   sFileStat; 

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "创建并打开文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "写入文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSTruncate(szFilePath, ulLength1), "截断文件失败\n");	

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStat(szFilePath, &sFileStat), "获取文件信息失败\n");

	CSTK_ASSERT_TRUE_FATAL((int)sFileStat.m_lSize == ulLength1, "截断后文件大小不对\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "移除文件失败\n");
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSTruncate(const char * pcPathName, unsigned long ulLength)
//@DESCRIPTION:  将文件截大
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcPathName = 分区挂载路径 + "CSUDIFSTruncate_0002"
//@INPUT: ulLength = ulLength1，其中ulLength1大小文件原大小
//@EXPECTATION: 返回值等于0
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSTruncate_0002", "w+")创建并打开文件，得到句柄hFile
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile, "CSUDIFSTruncate_0002", strlen("CSUDIFSTruncate_0002"))
//@EXECUTIONFLOW: 5、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 6、调用CSUDIFSTruncate(分区挂载路径 + "CSUDIFSTruncate_0002", strlen("this is a test")*2)把文件截大
//@EXECUTIONFLOW: 7、调用CSUDIFSStat(分区挂载路径 + "CSUDIFSTruncate_0001", psFileStat)获得文件大小，与ulLength1比较，应该相等
//@EXECUTIONFLOW: 8、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSTruncate_0002")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSTruncate_0002(void)
{
	char   szTest[] = "CSUDIFSTruncate_0002";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE  hFile = CSUDI_NULL;  
	int  nLen = strlen(szTest);
	int  ulLength1 = nLen*2;
	CSUDIFSFileStat_S   sFileStat; 

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "创建并打开文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "写入文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSTruncate(szFilePath, ulLength1), "截断文件失败\n");	

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStat(szFilePath, &sFileStat), "获取文件信息失败\n");

	CSTK_ASSERT_TRUE_FATAL((int)sFileStat.m_lSize == ulLength1, "截断后文件大小不对\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "移除文件失败\n");
		}
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSTruncate(const char * pcPathName, unsigned long ulLength)
//@DESCRIPTION:  截文件夹
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcPathName = 分区挂载路径 + "CSUDIFSTruncate_0003"
//@INPUT: ulLength = 10
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSMkdir(分区挂载路径 + "CSUDIFSTruncate_0003", 0)创建文件夹
//@EXECUTIONFLOW: 4、调用CSUDIFSTruncate(分区挂载路径 + "CSUDIFSTruncate_0003", 10)截断文件夹，应当失败
//@EXECUTIONFLOW: 5、调用CSUDIFSRmdir(分区挂载路径 + "CSUDIFSTruncate_0003")删除文件夹
CSUDI_BOOL CSTC_FS_IT_CSUDIFSTruncate_0003(void)
{
	char   szTest[] = "CSUDIFSTruncate_0003";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSMkdir(szFilePath, 0), "创建目录失败\n");

	CSTK_ASSERT_TRUE(-1 == CSUDIFSTruncate(szFilePath, 10), "截断文件夹不应该成功\n");	

	CSTK_ASSERT_TRUE(0 == CSUDIFSRmdir(szFilePath), "删除文件夹失败\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSTruncate(const char * pcPathName, unsigned long ulLength)
//@DESCRIPTION: 截空指针
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcPathName = CSUDI_NULL
//@INPUT: ulLength = 10
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSTruncate(CSUDI_NULL, 10)截文件，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSTruncate_0004(void)
{
	CSTK_ASSERT_TRUE(-1 == CSUDIFSTruncate(CSUDI_NULL, 10), "截断空指针不应该成功\n");	
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSTruncate(const char * pcPathName, unsigned long ulLength)
//@DESCRIPTION:  截不存在的文件（包括路径不合法、不存在、非法字符等）
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcPathName = 分区挂载路径 + "CSUDIFSTruncate_0005"
//@INPUT: ulLength = 10
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSTruncate(分区挂载路径 + "CSUDIFSTruncate_0005", 10)截文件，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSTruncate_0005(void)
{
	char   szTest[] = "CSUDIFSTruncate_0005";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE(-1 == CSUDIFSTruncate(szFilePath, 10), "截断不存在的文件不应该成功\n");	

	CSTK_FATAL_POINT
	{
		;
	}
	
	return CSUDI_TRUE;	
}
/****************************CSUDIFSTruncate  End *******************************/

/****************************CSUDIFSFTruncate  Start *****************************/ 
//@CASEGROUP: int CSUDIFSFTruncate(CSUDI_HANDLE hFile, unsigned long ulLength)
//@DESCRIPTION:  将文件截小
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile 为合法的文件句柄，由CSUDIFSOpen()返回
//@INPUT: ulLength = ulLength1，ulLength1为文件原大小
//@EXPECTATION: 返回值等于0
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSFTruncate_0001", "w+")创建并打开文件，得到文件句柄hFile
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile, "CSUDIFSFTruncate_0001", strlen("CSUDIFSFTruncate_0001"))写入数据
//@EXECUTIONFLOW: 5、调用CSUDIFSFTruncate(hFile, strlen("CSUDIFSFTruncate_0001")/2)截小文件
//@EXECUTIONFLOW: 6、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 7、调用CSUDIFSStat(分区挂载路径 + "CSUDIFSTruncate_0001", psFileStat)获得文件大小，与ulLength1比较，应该相等
//@EXECUTIONFLOW: 8、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSFTruncate_0001")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFTruncate_0001(void)
{
	char   szTest[] = "CSUDIFSFTruncate_0001";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE  hFile = CSUDI_NULL;  
	int  nLen = strlen(szTest);
	int  ulLength1 = nLen/2;
	CSUDIFSFileStat_S   sFileStat; 

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "创建并打开文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "写入文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSFTruncate(hFile, ulLength1), "截断文件失败\n");
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStat(szFilePath, &sFileStat), "获取文件信息失败\n");

	CSTK_ASSERT_TRUE_FATAL((int)sFileStat.m_lSize == ulLength1, "截断后文件大小不对\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");	

			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "移除文件失败\n");
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSFTruncate(CSUDI_HANDLE hFile, unsigned long ulLength)
//@DESCRIPTION:  将文件截断
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile 为合法的文件句柄，由CSUDIFSOpen()返回
//@INPUT: ulLength = ulLength1，ulLength大于文件原大小
//@EXPECTATION: 返回值等于0
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSFTruncate_0002", "w")创建并打开文件，得到句柄hFile
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile, "this is a test", strlen("this is a test"))写入数据
//@EXECUTIONFLOW: 5、调用CSUDIFSFTruncate(hFile, strlen("CSUDIFSFTruncate_0002")*2)截断文件
//@EXECUTIONFLOW: 6、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 7、调用CSUDIFSStat(分区挂载路径 + "CSUDIFSFTruncate_0002", psFileStat)获得文件大小，与ulLength1比较，应该相等
//@EXECUTIONFLOW: 8、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSFTruncate_0002")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFTruncate_0002(void)
{
	char   szTest[] = "CSUDIFSFTruncate_0002";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE  hFile = CSUDI_NULL;  
	int  nLen = strlen(szTest);
	int  ulLength1 = nLen*2;
	CSUDIFSFileStat_S   sFileStat; 

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "创建并打开文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "写入文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSFTruncate(hFile, ulLength1), "截断文件失败\n");
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStat(szFilePath, &sFileStat), "获取文件信息失败\n");

	CSTK_ASSERT_TRUE_FATAL((int)sFileStat.m_lSize == ulLength1, "截断后文件大小不对\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");	
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "移除文件失败\n");
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSFTruncate(CSUDI_HANDLE hFile, unsigned long ulLength)
//@DESCRIPTION:  截空文件句柄
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = CSUDI_NULL
//@INPUT: ulLength = 10 
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSFTruncate(CSUDI_NULL, 10)截文件，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFTruncate_0003(void)
{
	CSTK_ASSERT_TRUE(-1 == CSUDIFSFTruncate(CSUDI_NULL, 10), "截断空句柄不应该成功\n");	
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSFTruncate(CSUDI_HANDLE hFile, unsigned long ulLength)
//@DESCRIPTION:  截不存在的句柄
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = 0x7FFFFFFF(假设0x7FFFFFFF为不存在的句柄)
//@INPUT: ulLength = 10
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSFTruncate(hFile, 10)截文件，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFTruncate_0004(void)
{
	CSTK_ASSERT_TRUE(-1 == CSUDIFSFTruncate((CSUDI_HANDLE)0x7FFFFFFF, 10), "截断不存在的文件句柄应该成功\n");	
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSFTruncate(CSUDI_HANDLE hFile, unsigned long ulLength)
//@DESCRIPTION:  截文件夹
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile 文件夹句柄，由CSUDIFSMkdir()返回
//@INPUT: ulLength = 10 
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSMkdir(分区挂载路径 + "CSUDIFSFTruncate_0005", 0)创建文件夹
//@EXECUTIONFLOW: 4、调用CSUDIFSOpenDir(分区挂载路径 + "CSUDIFSFTruncate_0005")打开文件夹，得到句柄hDir
//@EXECUTIONFLOW: 5、调用CSUDIFSFTruncate(hDir, 10)截断文件夹
//@EXECUTIONFLOW: 6、调用CSUDIFSCloseDir(hDir)关闭文件夹
//@EXECUTIONFLOW: 7、调用CSUDIFSRmdir(分区挂载路径 + "CSUDIFSFTruncate_0005")删除文件夹
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFTruncate_0005(void)
{
	char   szTest[] = "CSUDIFSFTruncate_0005";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE  hDir = CSUDI_NULL;  

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSMkdir(szFilePath, 0), "创建文件夹失败\n");

	CSTK_ASSERT_TRUE(CSUDI_NULL != (hDir = CSUDIFSOpenDir(szFilePath)), "打开文件夹失败\n");

	CSTK_ASSERT_TRUE(-1 == CSUDIFSFTruncate(hDir, 10), "截断文件夹不应该成功\n");	

	CSTK_FATAL_POINT
	{
		CSTK_ASSERT_TRUE(0 == CSUDIFSCloseDir(hDir), "关闭文件夹失败\n");

		CSTK_ASSERT_TRUE(0 == CSUDIFSRmdir(szFilePath), "删除文件夹失败\n");

	}
	return CSUDI_TRUE;		
}
/****************************CSUDIFSFTruncate  End *******************************/

/****************************CSUDIFSLTruncate  Start *****************************/ 
//@CASEGROUP: int CSUDIFSLTruncate(const char * pcPathName,CSUDI_UINT64 u64Length)
//@DESCRIPTION:  将文件截小
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcPathName = 分区挂载路径 + "CSUDIFSLTruncate_0001"
//@INPUT: u64Length = u64Length1，u64Length1小于文件原大小
//@EXPECTATION: 返回值等于0
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSLTruncate_0001", "w+")创建并打开文件，得到句柄hFile
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile, "CSUDIFSLTruncate_0001", strlen("CSUDIFSLTruncate_0001"))写入数据
//@EXECUTIONFLOW: 5、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 6、调用CSUDIFSLTruncate(分区挂载路径 + "CSUDIFSLTruncate_0001", strlen("CSUDIFSLTruncate_0001")/2)截小文件，应当成功
//@EXECUTIONFLOW: 7、调用CSUDIFSStat(分区挂载路径 + "CSUDIFSLTruncate_0001", psFileStat)获得文件大小，与u64Length1比较，应该相等
//@EXECUTIONFLOW: 8、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSLTruncate_0001")删除文件
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

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "创建并打开文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "写入文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSLTruncate(szFilePath, u64Length1), "截断文件失败\n");	

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStat(szFilePath, &sFileStat), "获取文件信息失败\n");

	ll_int2long(u64RealLen, (sFileStat.m_lSize));

	bEqual = ll_eq(u64Length1, u64RealLen);

	CSTK_ASSERT_TRUE_FATAL(bEqual, "截断后文件大小不对\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "移除文件失败\n");
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSLTruncate(const char * pcPathName,CSUDI_UINT64 u64Length)
//@DESCRIPTION:  将文件截大
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcPathName = 分区挂载路径 + "CSUDIFSLTruncate_0002"
//@INPUT: u64Length = u64Length1，u64Length1大于文件原大小
//@EXPECTATION: 返回值等于0
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSLTruncate_0002", "w+")创建并打开文件，得到句柄hFile
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile, "CSUDIFSLTruncate_0002", strlen("CSUDIFSLTruncate_0002"))写入数据
//@EXECUTIONFLOW: 5、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 6、调用CSUDIFSLTruncate(分区挂载路径 + "CSUDIFSLTruncate_0002", strlen("CSUDIFSLTruncate_0002")*2)把文件截大
//@EXECUTIONFLOW: 7、调用CSUDIFSStat(分区挂载路径 + "CSUDIFSLTruncate_0002", psFileStat)获得文件大小，与u64Length1比较，应该相等
//@EXECUTIONFLOW: 8、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSLTruncate_0002")删除文件
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

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "创建并打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "写入文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");
    CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSLTruncate(szFilePath, u64Length1), "截断文件失败\n");	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStat(szFilePath, &sFileStat), "获取文件信息失败\n");

	ll_int2long(u64RealLen, (sFileStat.m_lSize));

	bEqual = ll_eq(u64RealLen, u64Length1);
	
	CSTK_ASSERT_TRUE_FATAL(bEqual, "截断后文件大小不对\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "移除文件失败\n");
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSLTruncate(const char * pcPathName,CSUDI_UINT64 u64Length)
//@DESCRIPTION:  截文件夹
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcPathName = 分区挂载路径 + "CSUDIFSLTruncate_0003"
//@INPUT: u64Length = 10
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSMkdir(pcPathName, 0)创建文件夹
//@EXECUTIONFLOW: 4、调用CSUDIFSLTruncate(分区挂载路径 + "CSUDIFSLTruncate_0003", 10)截断文件夹，应当失败
//@EXECUTIONFLOW: 5、调用CSUDIFSRmdir(分区挂载路径 + "CSUDIFSLTruncate_0003")删除文件夹
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLTruncate_0003(void)
{
	char   szTest[] = "CSUDIFSLTruncate_0003";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_UINT64  u64Length1;

	ll_int2long(u64Length1, 10);

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSMkdir(szFilePath, 0), "创建目录失败\n");
	CSTK_ASSERT_TRUE(-1 == CSUDIFSLTruncate(szFilePath, u64Length1), "截断文件夹不应该成功\n");	
	CSTK_ASSERT_TRUE(0 == CSUDIFSRmdir(szFilePath), "删除文件夹失败\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSLTruncate(const char * pcPathName,CSUDI_UINT64 u64Length)
//@DESCRIPTION: 截空指针
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcPathName = CSUDI_NULL
//@INPUT: u64Length = 10
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSLTruncate(CSUDI_NULL, 10)，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLTruncate_0004(void)
{
	CSUDI_UINT64  u64Length1;

	ll_int2long(u64Length1, 10);
	
	CSTK_ASSERT_TRUE(-1 == CSUDIFSLTruncate(CSUDI_NULL, u64Length1), "截断文件夹不应该成功\n");
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSLTruncate(const char * pcPathName,CSUDI_UINT64 u64Length)
//@DESCRIPTION:  截不存在的文件（包括路径不合法、不存在、非法字符等）
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcPathName = 分区挂载路径 + "CSUDIFSLTruncate_0005"
//@INPUT: u64Length = 10
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSLTruncate(分区挂载路径 + "CSUDIFSLTruncate_0005", 10)截断文件，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLTruncate_0005(void)
{
	char   szTest[] = "CSUDIFSLTruncate_0005";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_UINT64  u64Length1;

	ll_int2long(u64Length1, 10);

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE(-1 == CSUDIFSLTruncate(szFilePath, u64Length1), "截断不存在的文件不应该成功\n");	

	CSTK_FATAL_POINT
	{
		;
	}
	
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSLTruncate(const char * pcPathName,CSUDI_UINT64 u64Length)
//@DESCRIPTION:  将文件截大为超大文件
//@PRECONDITION: 1、系统中存在一个已挂载的分卷设备
//@PRECONDITION: 2、系统支持大文件，且盘较大
//@INPUT: pcPathName = 分区挂载路径 + "CSUDIFSLTruncate_0006"
//@INPUT: u64Length = 4294967296，即4G
//@EXPECTATION: 返回值等于0
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSLTruncate_0006", "w+")创建并打开文件，得到句柄hFile
//@EXECUTIONFLOW: 4、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 5、调用CSUDIFSLTruncate(分区挂载路径 + "CSUDIFSLTruncate_0006", 4294967296)截文件，应当成功
//@EXECUTIONFLOW: 6、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSLTruncate_0006", "r")创建并打开文件，得到句柄hFile
//@EXECUTIONFLOW: 7、调用CSUDIFSLseek(hFile)移动文件指针到文件末尾
//@EXECUTIONFLOW: 8、调用CSUDIFSLtell(hFile)获得文件大小，与u64Length比较，应当相等
//@EXECUTIONFLOW: 9、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 10、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSLTruncate_0006")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLTruncate_0006(void)
{
	//手动测试
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

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "创建并打开文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSLTruncate(szFilePath, u64Length1), "截断为超大文件失败\n");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "r")), "打开文件失败\n");
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSLseek(hFile, u64Offset, CSUDIFS_SEEK_END), "移动文件指针失败\n");

	u64RealLen = CSUDIFSLtell(hFile);

	bEqual = ll_eq((CSUDI_UINT64)u64RealLen, u64Length1);

	CSTK_ASSERT_TRUE_FATAL(bEqual , "获取文件指针当前位置失败\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");

	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSRemove(szFilePath), "移除文件失败\n");
		}
	}
	
	return CSUDI_TRUE;	
}
/****************************CSUDIFSLTruncate  End *******************************/

/****************************CSUDIFSLFTruncate  Start *****************************/ 
//@CASEGROUP: int CSUDIFSLFTruncate(CSUDI_HANDLE hFile,CSUDI_UINT64 u64Length)
//@DESCRIPTION:  将文件截小
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile 为合法的文件句柄，由CSUDIFSOpen()返回
//@INPUT: u64Length = u64Length1, u64Length1小于文件原大小
//@EXPECTATION: 返回值等于0
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSLFTruncate_0001", "w+")创建并打开文件，得到句柄hFile
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile, "CSUDIFSLFTruncate_0001", strlen("CSUDIFSLFTruncate_0001"))写入数据
//@EXECUTIONFLOW: 5、调用CSUDIFSLFTruncate(hFile, strlen("CSUDIFSLFTruncate_0001")/2)截小文件
//@EXECUTIONFLOW: 6、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 7、调用CSUDIFSStat(分区挂载路径 + "CSUDIFSLFTruncate_0001", psFileStat)获得文件大小，与u64Length1比较，应该相等
//@EXECUTIONFLOW: 8、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSLFTruncate_0001")删除文件
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

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "创建并打开文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "写入文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSLFTruncate(hFile, u64Length1), "截断文件失败\n");
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");	

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStat(szFilePath, &sFileStat), "获取文件信息失败\n");

	ll_int2long(u64RealLen, (sFileStat.m_lSize));
	bEqual = ll_eq(u64RealLen, u64Length1);
	
	CSTK_ASSERT_TRUE_FATAL(bEqual, "截断后文件大小不对\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "移除文件失败\n");
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSLFTruncate(CSUDI_HANDLE hFile,CSUDI_UINT64 u64Length)
//@DESCRIPTION:  将文件截大
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile 为合法的文件句柄，由CSUDIFSOpen()返回
//@INPUT: u64Length = u64Length1, u64Length1大于文件原大小
//@EXPECTATION: 返回值等于0
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSLFTruncate_0002", "w+")创建并打开文件，得到句柄hFile
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile, "CSUDIFSLFTruncate_0002", strlen("CSUDIFSLFTruncate_0002"))写入数据
//@EXECUTIONFLOW: 5、调用CSUDIFSLFTruncate(hFile, strlen("CSUDIFSLFTruncate_0002")*2)截大文件
//@EXECUTIONFLOW: 6、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 7、调用CSUDIFSStat(分区挂载路径 + "CSUDIFSLFTruncate_0002", psFileStat)获得文件大小，与u64Length1比较，应该相等
//@EXECUTIONFLOW: 8、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSLFTruncate_0002")删除文件
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

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "创建并打开文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "写入文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSLFTruncate(hFile, u64Length1), "截断文件失败\n");
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");	

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStat(szFilePath, &sFileStat), "获取文件信息失败\n");

	ll_int2long(u64RealLen, (sFileStat.m_lSize));
	bEqual = ll_eq(u64RealLen, u64Length1);
	
	CSTK_ASSERT_TRUE_FATAL(bEqual, "截断后文件大小不对\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "移除文件失败\n");
		}
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSLFTruncate(CSUDI_HANDLE hFile,CSUDI_UINT64 u64Length)
//@DESCRIPTION:  截空文件句柄
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = CSUDI_NULL
//@INPUT: u64Length = 10
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSLFTruncate(CSUDI_NULL, 10)截文件，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLFTruncate_0003(void)
{
	CSUDI_UINT64  u64Length1;

	ll_int2long(u64Length1, 10);
	
	CSTK_ASSERT_TRUE(-1 == CSUDIFSLFTruncate(CSUDI_NULL, u64Length1), "截断文件失败\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSLFTruncate(CSUDI_HANDLE hFile,CSUDI_UINT64 u64Length)
//@DESCRIPTION:  截不存在的句柄
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = 0x7FFFFFFF(假设0x7FFFFFFF为不存在的句柄)
//@INPUT: u64Length = 10
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSLFTruncate(0x7FFFFFFF, 10)截文件，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLFTruncate_0004(void)
{
	CSUDI_UINT64  u64Length1;

	ll_int2long(u64Length1, 10);
	CSTK_ASSERT_TRUE(-1 == CSUDIFSLFTruncate((CSUDI_HANDLE)0x7FFFFFFF, u64Length1), "截断文件失败\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSLFTruncate(CSUDI_HANDLE hFile,CSUDI_UINT64 u64Length)
//@DESCRIPTION:  截文件夹
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile 文件夹句柄，由CSUDIFSMkdir()返回
//@INPUT: u64Length = 10
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSMkdir(分区挂载路径 + "CSUDIFSLFTruncate_0005", 0)创建文件夹
//@EXECUTIONFLOW: 4、调用CSUDIFSOpenDir(分区挂载路径 + "CSUDIFSLFTruncate_0005")打开文件夹，得到句柄hDir
//@EXECUTIONFLOW: 5、调用CSUDIFSLFTruncate(hDir, 10)截断文件夹，应当失败
//@EXECUTIONFLOW: 6、调用CSUDIFSCloseDir(hDir)关闭文件夹
//@EXECUTIONFLOW: 7、调用CSUDIFSRmdir(分区挂载路径 + "CSUDIFSLFTruncate_0005")删除文件夹
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLFTruncate_0005(void)
{
	char   szTest[] = "CSUDIFSLFTruncate_0005";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE  hDir = CSUDI_NULL;  
	CSUDI_UINT64  u64Length1;

	ll_int2long(u64Length1, 10);

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSMkdir(szFilePath, 0), "创建文件夹失败\n");

	CSTK_ASSERT_TRUE(CSUDI_NULL != (hDir = CSUDIFSOpenDir(szFilePath)), "打开文件夹失败\n");

	CSTK_ASSERT_TRUE(-1 == CSUDIFSLFTruncate(hDir, u64Length1), "截断文件夹不应该成功\n");	

	CSTK_ASSERT_TRUE(0 == CSUDIFSCloseDir(hDir), "关闭文件夹失败\n");

	CSTK_ASSERT_TRUE(0 == CSUDIFSRmdir(szFilePath), "删除文件夹失败\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;		
}

//@CASEGROUP: int CSUDIFSLFTruncate(CSUDI_HANDLE hFile,CSUDI_UINT64 u64Length)
//@DESCRIPTION:  截大为超大文件
//@PRECONDITION: 1、系统中存在一个已挂载的分卷设备
//@PRECONDITION: 2、需要系统支持大文件，且盘足够大
//@INPUT: hFile 为合法的文件句柄，由CSUDIFSOpen()返回
//@INPUT: u64Length = 4294967296，即4G
//@EXPECTATION: 返回值等于0
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSLFTruncate_0006", "w+")创建并打开文件，得到句柄hFile
//@EXECUTIONFLOW: 4、调用CSUDIFSLFTruncate(hFile, 4294967296)截大为超大文件，应当成功
//@EXECUTIONFLOW: 5、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 6、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSLFTruncate_0006", "r")创建并打开文件，得到句柄hFile
//@EXECUTIONFLOW: 7、调用CSUDIFSLseek(hFile)移动文件指针到文件末尾
//@EXECUTIONFLOW: 8、调用CSUDIFSLtell(hFile)获得文件大小，与u64Length比较，应当相等
//@EXECUTIONFLOW: 9、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 10、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSLFTruncate_0006")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSLFTruncate_0006(void)
{
	//手动测试
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
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "创建并打开文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSLFTruncate(hFile, u64Length1), "截断为超大文件失败\n");	
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "r")), "打开文件失败\n");
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSLseek(hFile, i64Offset, CSUDIFS_SEEK_END), "移动文件指针失败\n");

	i64RealLen = CSUDIFSLtell(hFile);
	bEqual = ll_eq((CSUDI_UINT64)i64Offset, u64Length1);

	CSTK_ASSERT_TRUE_FATAL(bEqual, "获取文件指针当前位置失败\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");

	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSRemove(szFilePath), "移除文件失败\n");
		}
	}
	
	return CSUDI_TRUE;	
}
/****************************CSUDIFSLFTruncate  End *******************************/

/****************************CSUDIFSStat  Start *****************************/ 
//@CASEGROUP: int CSUDIFSStat(const char * pcFileName, CSUDIFSFileStat_S * psFileStat)
//@DESCRIPTION:  正常获取文件属性
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcFileName = 分区挂载路径 + "CSUDIFSStat_0001"
//@INPUT: psFileStat = sFileStat，其中CSUDIFSFileStat_S sFileStat
//@EXPECTATION: 返回值等于0
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSStat_0001", "w+")创建并打开文件，得到句柄hFile
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile, "CSUDIFSStat_0001", strlen("CSUDIFSStat_0001"))写入数据
//@EXECUTIONFLOW: 5、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 6、调用CSUDIFSStat(分区挂载路径 + "CSUDIFSStat_0001", &sFileStat)获得文件属性，取出文件大小与写入时的比较，应当相等
//@EXECUTIONFLOW: 7、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSStat_0001")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSStat_0001(void)
{
	char   szTest[] = "CSUDIFSStat_0001";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE   hFile = CSUDI_NULL;
	int    nLen = strlen(szTest);
	CSUDIFSFileStat_S  sFileStat;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "创建并打开文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "写入文件失败\n");
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");	

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStat(szFilePath, &sFileStat), "获取文件信息失败\n");

	CSTK_ASSERT_TRUE_FATAL((int)sFileStat.m_lSize == nLen, "获取文件信息大小不对\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "移除文件失败\n");
		}
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSStat(const char * pcFileName, CSUDIFSFileStat_S * psFileStat)
//@DESCRIPTION:  获取空指针文件属性
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcFileName = CSUDI_NULL
//@INPUT: psFileStat = &sFileStat，其中CSUDIFSFileStat_S psFileStat
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSStat(CSUDI_NULL, psFileStat1)获取文件属性，应当失败	
CSUDI_BOOL CSTC_FS_IT_CSUDIFSStat_0002(void)
{
	CSUDIFSFileStat_S  sFileStat;
	
	CSTK_ASSERT_TRUE(-1 == CSUDIFSStat(CSUDI_NULL, &sFileStat), "获取文件信息失败\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSStat(const char * pcFileName, CSUDIFSFileStat_S * psFileStat)
//@DESCRIPTION:  获取不存在的文件的属性
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcFileName = 分区挂载路径 + "CSUDIFSStat_0003"
//@INPUT: psFileStat = &sFileStat，其中CSUDIFSFileStat_S sFileStat
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSStat(分区挂载路径 + "CSUDIFSStat_0003", &sFileStat)获取文件属性，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSStat_0003(void)
{
	char   szTest[] = "CSUDIFSStat_0003";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDIFSFileStat_S  sFileStat;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);	

	CSTK_ASSERT_TRUE(-1 == CSUDIFSStat(szFilePath, &sFileStat), "获取文件信息不应当成功\n");

	CSTK_FATAL_POINT
	{
		;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSStat(const char * pcFileName, CSUDIFSFileStat_S * psFileStat)
//@DESCRIPTION:  获取文件夹属性
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcFileName = 分区挂载路径 + "CSUDIFSStat_0004"
//@INPUT: psFileStat = &sFileStat，其中CSUDIFSFileStat_S sFileStat
//@EXPECTATION: 返回值等于0
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSMkdir(分区挂载路径 + "CSUDIFSStat_0004", 0)创建文件夹
//@EXECUTIONFLOW: 4、调用CSUDIFSStat(分区挂载路径 + "CSUDIFSStat_0004", &sFileStat)获取文件夹属性，应当成功
//@EXECUTIONFLOW: 5、调用CSUDIFSRmdir(分区挂载路径 + "CSUDIFSStat_0004")删除文件夹
CSUDI_BOOL CSTC_FS_IT_CSUDIFSStat_0004(void)
{
	char   szTest[] = "CSUDIFSStat_0004";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDIFSFileStat_S sFileStat;

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSMkdir(szFilePath, 0), "创建文件夹失败\n");

	CSTK_ASSERT_TRUE(0 == CSUDIFSStat(szFilePath, &sFileStat), "获取文件夹信息失败\n");
	
	CSTK_ASSERT_TRUE(0 == CSUDIFSRmdir(szFilePath), "删除文件夹失败\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSStat(const char * pcFileName, CSUDIFSFileStat_S * psFileStat)
//@DESCRIPTION:  接收文件属性的指针非法
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcFileName = 分区挂载路径 + "CSUDIFSStat_0005"
//@INPUT: psFileStat = CSUDI_NULL
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSStat_0005", "w+")创建并打开文件，得到句柄hFile
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile, "CSUDIFSStat_0005", strlen("CSUDIFSStat_0005"))写入数据
//@EXECUTIONFLOW: 5、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 6、调用CSUDIFSStat(分区挂载路径 + "CSUDIFSStat_0005", CSUDI_NULL)获取文件属性，应当失败
//@EXECUTIONFLOW: 7、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSStat_0005")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSStat_0005(void)
{
	char   szTest[] = "CSUDIFSStat_0005";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE   hFile = CSUDI_NULL;
	int    nLen = strlen(szTest);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "创建并打开文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "写入文件失败\n");
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");	

	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSStat(szFilePath, CSUDI_NULL), "获取文件信息不应当成功\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "移除文件失败\n");
		}
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSStat(const char * pcFileName, CSUDIFSFileStat_S * psFileStat)
//@DESCRIPTION:  验证CSUDIFSFileStat_S里的时间属性在创建、修改、访问后都正常
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcFileName = 分区挂载路径 + "CSUDIFSStat_0006"
//@INPUT: psFileStat = &sFileStat
//@EXPECTATION: 每次对文件进行操作之后时间属性都正常变化
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSStat_0006", "w+")创建并打开文件，得到句柄hFile
//@EXECUTIONFLOW: 4、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 5、调用CSUDIFSStat获取文件属性中的时间信息，并与调用CSUDIPlusOSClockGet()获得的时间信息相比较,期望时间正常（创建文件时Ctime == Atime == Mtime == 实际创建时间）
//@EXECUTIONFLOW: 6、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSStat_0006", "w+")打开文件，得到句柄hFile
//@EXECUTIONFLOW: 7、调用CSUDIFSWrite(hFile, "CSUDIFSStat_0006", strlen("CSUDIFSStat_0006"))写入数据
//@EXECUTIONFLOW: 8、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 9、调用CSUDIFSStat获取文件属性中的时间信息，并与调用CSUDIPlusOSClockGet()获得的时间信息相比较,期望时间正常（修改文件时Atime不变，Ctime == Mtime == 实际修改时间）
//@EXECUTIONFLOW: 10、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSStat_0006", "r")打开文件，得到句柄hFile
//@EXECUTIONFLOW: 11、调用CSUDIFSRead(hFile, buf, buflen))读出数据
//@EXECUTIONFLOW: 12、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 13、调用CSUDIFSStat获取文件属性中的时间信息，并与调用CSUDIPlusOSClockGet()获得的时间信息相比较,期望时间正常（访问文件时Ctime、Mtime不变，Atime == 实际访问时间）
//@EXECUTIONFLOW: 14、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSStat_0006")删除文件
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

    CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败");

    memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
    strcpy(szFilePath, g_szVolumeMountPoint);
    strcat(szFilePath, szTest);


    /*************************
     * 创建文件并校验时间*****
    *************************/
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "创建文件：打开文件失败");

    CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "创建：关闭文件失败");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPlusOSClockGet(&curTime), "创建文件：获取当前时间失败");

    CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStat(szFilePath, &sFileStat), "创建文件：获取文件属性失败");

    CSTK_ASSERT_TRUE_FATAL((sFileStat.m_lCtime == sFileStat.m_lMtime) && (sFileStat.m_lMtime == sFileStat.m_lAtime), "创建文件：文件时间属性异常");
    CSTK_ASSERT_TRUE_FATAL((sFileStat.m_lCtime <= curTime) && ((curTime-sFileStat.m_lCtime)<=1), "创建文件：文件属性异常");

    // 保存最后访问时间
    lastOpTime = sFileStat.m_lAtime;

    // 等待3s后操作，让文件时间属性变化明显
    CSTCPrint("时间信息正常，等待3s继续测试...\n");
    CSUDIOSThreadSleep(3000);

    /*************************
     * 修改文件并校验时间*****
    *************************/
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "修改文件：打开文件失败");

    CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "写入文件失败");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPlusOSClockGet(&curTime), "修改文件：获取当前时间失败");

    CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败");

    CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStat(szFilePath, &sFileStat), "修改文件：获取文件属性失败");

    CSTK_ASSERT_TRUE_FATAL((sFileStat.m_lCtime == sFileStat.m_lMtime) && (sFileStat.m_lAtime == lastOpTime), "修改文件：文件时间属性异常");
    CSTK_ASSERT_TRUE_FATAL((sFileStat.m_lMtime <= curTime) && ((curTime-sFileStat.m_lMtime)<=1), "修改文件：文件属性异常");

    lastOpTime = sFileStat.m_lMtime;

    // 等待3s后操作，让文件时间属性变化明显
    CSTCPrint("时间信息正常，等待3s继续测试...\n");
    CSUDIOSThreadSleep(3000);

    /*************************
     * 访问文件并校验时间*****
    *************************/
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "r")), "创建并打开文件失败");
    
    CSTK_ASSERT_TRUE_FATAL(0 <= (nLen = CSUDIFSRead(hFile, szBuf, 100)), "读取文件失败");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPlusOSClockGet(&curTime), "访问文件：获取当前时间失败");

    CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "访问文件：关闭文件失败");

    CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStat(szFilePath, &sFileStat), "访问文件：获取文件信息失败");

    CSTK_ASSERT_TRUE_FATAL((sFileStat.m_lCtime == sFileStat.m_lMtime) && (sFileStat.m_lMtime == lastOpTime), "访问文件：文件时间属性异常");
    CSTK_ASSERT_TRUE_FATAL((sFileStat.m_lAtime <= curTime) && ((curTime-sFileStat.m_lAtime)<=1), "访问文件：文件属性异常");

    CSTK_FATAL_POINT
		
    if (CSUDI_NULL != hFile)
    {
        CSTK_ASSERT_TRUE(0 == CSUDIFSRemove(szFilePath), "移除文件失败");

		CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile), "释放句柄失败");

		hFile = CSUDI_NULL;
    }

    return CSUDI_TRUE;
}
/****************************CSUDIFSStat  End *******************************/

/****************************CSUDIFSStat  Start *****************************/ 
//@CASEGROUP: int CSUDIFSFstat(CSUDI_HANDLE hFile, CSUDIFSFileStat_S * psFileStat)
//@DESCRIPTION:  正常获取文件属性
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile 为合法的文件句柄，由CSUDIOpen()返回
//@INPUT: psFileStat = &sFileStat，其中CSUDIFSFileStat_S sFileStat
//@EXPECTATION: 返回值等于0
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径+"CSUDIFSFstat_0001", "w+")创建并打开文件，得到句柄hFile
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile, "CSUDIFSFstat_0001", strlen("CSUDIFSFstat_0001"))写入数据
//@EXECUTIONFLOW: 5、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 6、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSFstat_0001", "r")打开文件
//@EXECUTIONFLOW: 7、调用CSUDIFSFstat(hFile, &sFileStat)获取文件属性，应当成功
//@EXECUTIONFLOW: 8、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 9、调用CSUDIFSRemove(分区挂载路径+"CSUDIFSFstat_0001")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFstat_0001(void)
{
	char   szTest[] = "CSUDIFSFstat_0001";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE   hFile = CSUDI_NULL;
	int    nLen = strlen(szTest);
	CSUDIFSFileStat_S  sFileStat;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	//CSTCPrint("路径= %s\n",szFilePath);
	strcat(szFilePath, szTest);	
        //CSTCPrint("路径= %s\n",szFilePath);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "创建并打开文件失败1\n");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "写入文件失败\n");
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "r")), "打开文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSFstat(hFile, &sFileStat), "获取文件信息失败\n");
	CSTK_ASSERT_TRUE_FATAL((int)sFileStat.m_lSize == nLen, "获取文件信息大小不对\n");	

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");	

	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "移除文件失败\n");
		}
	}
	return CSUDI_TRUE;		
}

//@CASEGROUP: int CSUDIFSFstat(CSUDI_HANDLE hFile, CSUDIFSFileStat_S * psFileStat)
//@DESCRIPTION:  获取空文件句柄的属性
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = CSUDI_NULL
//@INPUT: psFileStat = &sFileStat，其中CSUDIFSFileStat_S sFileStat
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSFstat(CSUDI_NULL, &sFileStat)，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFstat_0002(void)
{
	CSUDIFSFileStat_S  sFileStat;

	CSTK_ASSERT_TRUE(-1 == CSUDIFSFstat(CSUDI_NULL, &sFileStat), "获取文件信息不应该成功\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSFstat(CSUDI_HANDLE hFile, CSUDIFSFileStat_S * psFileStat)
//@DESCRIPTION:  获取不存在的文件句柄的属性
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pFile = 0x7FFFFFFF(假设0x7FFFFFFF为不存的文件句柄)
//@INPUT: psFileStat = &sFileStat，其中CSUDIFSFileStat_S sFileStat
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSFstat(0x7FFFFFFF, &sFileStat)
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFstat_0003(void)
{
	CSUDIFSFileStat_S  sFileStat;

	CSTK_ASSERT_TRUE(-1 == CSUDIFSFstat((CSUDI_HANDLE)0x7FFFFFFF, &sFileStat), "获取文件信息失败\n");
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSFstat(CSUDI_HANDLE hFile, CSUDIFSFileStat_S * psFileStat)
//@DESCRIPTION:  获取文件夹属性
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = hDir, hDir为CSUDIFSMkdir返回的句柄
//@INPUT: psFileStat = &sFileStat，其中CSUDIFSFileStat_S sFileStat
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSMkdir(分区挂载路径 + "CSUDIFSFstat_0004", 0)创建文件夹
//@EXECUTIONFLOW: 4、调用CSUDIFSOpenDir(分区挂载路径 + "CSUDIFSFstat_0004")打开文件夹，得到文件夹句柄hDir
//@EXECUTIONFLOW: 5、调用CSUDIFSFstat(hDir, &sFileStat)获取文件夹属性
//@EXECUTIONFLOW: 6、调用CSUDIFSCloseDir(hDir)关闭文件夹
//@EXECUTIONFLOW: 7、调用CSUDIFSRmdir(分区挂载路径 + "CSUDIFSFstat_0004")删除文件夹
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFstat_0004(void)
{
	char   szTest[] = "CSUDIFSFstat_0004";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDIFSFileStat_S sFileStat;
	CSUDI_HANDLE     hDir = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSMkdir(szFilePath, 0), "创建文件夹失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hDir = CSUDIFSOpenDir(szFilePath)), "打开文件夹失败\n");
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSFstat(hDir, &sFileStat), "获取文件夹信息不应该成功\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSCloseDir(hDir), "关闭文件夹失败\n");
	
	CSTK_FATAL_POINT
	{
		if(hDir != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szFilePath), "删除文件夹失败\n");
		}
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSFstat(CSUDI_HANDLE hFile, CSUDIFSFileStat_S * psFileStat)
//@DESCRIPTION:  接收文件属性的指针非法
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = hFile1, hFile1为合法的文件句柄，由CSUDIOpen()返回
//@INPUT: psFileStat = CSUDI_NULL
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSFstat_0005", "w+")创建并打开文件，得到句柄hFile1
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile1, "CSUDIFSFstat_0005", strlen("CSUDIFSFstat_0005"))写入数据
//@EXECUTIONFLOW: 5、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 6、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSFstat_0005", "r")打开文件
//@EXECUTIONFLOW: 7、调用CSUDIFSFstat(hFile1, CSUDI_NULL)获取文件信息
//@EXECUTIONFLOW: 8、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 9、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSFstat_0005")删除 文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFstat_0005(void)
{
	char   szTest[] = "CSUDIFSFstat_0005";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	int    nLen = strlen(szTest);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(szFilePath, "w+")), "创建并打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile1, szTest, nLen), "写入文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(szFilePath, "r")), "打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSFstat(hFile1, CSUDI_NULL), "获取文件信息不应当成功\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");	

	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "移除文件失败\n");
		}
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSFstat(CSUDI_HANDLE hFile, CSUDIFSFileStat_S * psFileStat)
//@DESCRIPTION:  验证调用CSUDIFSFstat返回的CSUDIFSFileStat_S里的时间属性在创建、修改、访问后都正常
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile 为合法文件句柄，由CSUDIOpen()返回
//@INPUT: psFileStat = &sFileStat
//@EXPECTATION: 每次对文件进行操作之后时间属性都正常变化
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSStat_0006", "w+")创建并打开文件，得到句柄hFile
//@EXECUTIONFLOW: 4、调用CSUDIFSFstat获取文件属性中的时间信息，并调用CSUDIPlusOSClockGet()获得时间
//@EXECUTIONFLOW: 5、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 6、比较获取到的文件属性中的文件信息与获得的系统时间，期望时间正常（创建文件时Ctime == Atime == Mtime == 实际创建时间）
//@EXECUTIONFLOW: 7、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSStat_0006", "w+")打开文件，得到句柄hFile
//@EXECUTIONFLOW: 8、调用CSUDIFSWrite(hFile, "CSUDIFSStat_0006", strlen("CSUDIFSStat_0006"))写入数据
//@EXECUTIONFLOW: 9、调用CSUDIFSFstat获取文件属性中的时间信息，并调用CSUDIPlusOSClockGet()获得时间
//@EXECUTIONFLOW: 10、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 11、比较获取到的文件属性中的文件信息与获得的系统时间，期望时间正常（修改文件时Atime不变，Ctime == Mtime == 实际修改时间）
//@EXECUTIONFLOW: 12、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSStat_0006", "r")打开文件，得到句柄hFile
//@EXECUTIONFLOW: 13、调用CSUDIFSRead(hFile, buf, buflen))读出数据
//@EXECUTIONFLOW: 14、调用CSUDIFSFstat获取文件属性中的时间信息，并调用CSUDIPlusOSClockGet()获得时间
//@EXECUTIONFLOW: 15、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 16、比较获取到的文件属性中的文件信息与获得的系统时间，期望时间正常（访问文件时Ctime、Mtime不变，Atime == 实际访问时间）
//@EXECUTIONFLOW: 17、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSStat_0006")删除文件
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

    CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败");

    memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
    strcpy(szFilePath, g_szVolumeMountPoint);
    strcat(szFilePath, szTest);

    /*************************
     * 创建文件并校验时间*****
    *************************/
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "创建文件：打开文件失败");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPlusOSClockGet(&lCurTime), "创建文件：获取当前时间失败");

    CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSFstat(hFile, &sFileStat), "创建文件：获取文件属性失败");

    CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "创建：关闭文件失败");

    CSTK_ASSERT_TRUE_FATAL((sFileStat.m_lCtime == sFileStat.m_lMtime) && (sFileStat.m_lMtime == sFileStat.m_lAtime), "创建文件：文件时间属性异常");
    CSTK_ASSERT_TRUE_FATAL((sFileStat.m_lCtime <= lCurTime) && ((lCurTime-sFileStat.m_lCtime)<=1), "创建文件：文件属性异常");

    // 保存最后访问时间
    lLastOpTime = sFileStat.m_lAtime;

    // 等待3s后操作，让文件时间属性变化明显
    CSTCPrint("时间信息正常，等待3s继续测试...\n");
    CSUDIOSThreadSleep(3000);

    /*************************
     * 修改文件并校验时间*****
    *************************/
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "修改文件：打开文件失败");

    CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "写入文件失败");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPlusOSClockGet(&lCurTime), "修改文件：获取当前时间失败");

    CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSFstat(hFile, &sFileStat), "修改文件：获取文件属性失败");

    CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败");

    CSTK_ASSERT_TRUE_FATAL((sFileStat.m_lCtime == sFileStat.m_lMtime) && (sFileStat.m_lAtime == lLastOpTime), "修改文件：文件时间属性异常");
    CSTK_ASSERT_TRUE_FATAL((sFileStat.m_lMtime <= lCurTime) && ((lCurTime-sFileStat.m_lMtime)<=1), "修改文件：文件属性异常");

    lLastOpTime = sFileStat.m_lMtime;

    // 等待3s后操作，让文件时间属性变化明显
    CSTCPrint("时间信息正常，等待3s继续测试...\n");
    CSUDIOSThreadSleep(3000);

    /*************************
     * 访问文件并校验时间*****
    *************************/
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "r")), "创建并打开文件失败");
    
    CSTK_ASSERT_TRUE_FATAL(-1 != (nLen = CSUDIFSRead(hFile, szBuf, 100)), "读取文件失败");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPlusOSClockGet(&lCurTime), "访问文件：获取当前时间失败");

    CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSFstat(hFile, &sFileStat), "访问文件：获取文件信息失败");

    CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "访问文件：关闭文件失败");


    CSTK_ASSERT_TRUE_FATAL((sFileStat.m_lCtime == sFileStat.m_lMtime) && (sFileStat.m_lMtime == lLastOpTime), "访问文件：文件时间属性异常");
    CSTK_ASSERT_TRUE_FATAL((sFileStat.m_lAtime <= lCurTime) && ((lCurTime-sFileStat.m_lAtime)<=1), "访问文件：文件属性异常");

    CSTK_FATAL_POINT

    if (CSUDI_NULL != hFile)
    {
        CSTK_ASSERT_TRUE(0 == CSUDIFSRemove(szFilePath), "移除文件失败");

		CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile), "释放句柄失败");

		hFile = CSUDI_NULL;
    }

    return CSUDI_TRUE;
}
/****************************CSUDIFSFstat  End *******************************/

/****************************CSUDIFSMkdir  Start *****************************/ 
//@CASEGROUP: int CSUDIFSMkdir(const char * pcPathName, int nReserved)
//@DESCRIPTION:  正常创建文件夹	
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcPathName = 分区挂载路径 + "CSUDIFSMkdir_0001"
//@INPUT: nReserved = 0
//@EXPECTATION: 返回值等于0
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSMkdir(分区挂载路径 + "CSUDIFSMkdir_0001", 0)创建文件夹，应当成功
//@EXECUTIONFLOW: 4、调用CSUDIFSOpenDir(分区挂载路径 + "CSUDIFSFstat_0004")打开文件夹，得到文件夹句柄hDir
//@EXECUTIONFLOW: 5、调用CSUDIFSCloseDir(hDir)关闭文件夹
//@EXECUTIONFLOW: 6、调用CSUDIFSRmdir(分区挂载路径 + "CSUDIFSMkdir_0001")删除文件夹
CSUDI_BOOL CSTC_FS_IT_CSUDIFSMkdir_0001(void)
{
	char   szTest[] = "CSUDIFSMkdir_0001";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE     hDir = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSMkdir(szFilePath, 0), "创建文件夹失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hDir = CSUDIFSOpenDir(szFilePath)), "打开文件夹失败\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSCloseDir(hDir), "关闭文件夹失败\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szFilePath), "删除文件夹失败\n");
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSMkdir(const char * pcPathName, int nReserved)
//@DESCRIPTION:  在一个不存在的文件夹下创建文件夹
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcPathName = 分区挂载路径 + "CSUDIFSMkdir_0002_dir" + "CSUDIFSMkdir_0002"
//@INPUT: nReserved = 0
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSMkdir(分区挂载路径 + "CSUDIFSMkdir_0002_dir" + "CSUDIFSMkdir_0002", 0)创建文件夹，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSMkdir_0002(void)
{
	char   szTest[] = "CSUDIFSMkdir_0002_dir/CSUDIFSMkdir_0002";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);
	
	CSTK_ASSERT_TRUE(-1 == CSUDIFSMkdir(szFilePath, 0), "创建文件夹不应当成功\n");
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSMkdir(const char * pcPathName, int nReserved)
//@DESCRIPTION:  创建空名文件夹
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcPathName = 分区挂载路径 + ""
//@INPUT: nReserved = 0
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSMkdir(pcPathName, 0)，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSMkdir_0003(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");
	
	CSTK_ASSERT_TRUE(-1 == CSUDIFSMkdir(g_szVolumeMountPoint, 0), "创建空名文件夹不应当成功\n");
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSMkdir(const char * pcPathName, int nReserved)
//@DESCRIPTION:  创建空指针文件夹
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcPathName = CSUDI_NULL
//@INPUT: nReserved = 0
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSMkdir(pcPathName, 0)创建文件,应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSMkdir_0004(void)
{
	CSTK_ASSERT_TRUE(-1 == CSUDIFSMkdir(CSUDI_NULL, 0), "创建空名文件夹不应当成功\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSMkdir(const char * pcPathName, int nReserved)
//@DESCRIPTION:  保留参数传其非零
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcPathName = 分区挂载路径 + "CSUDIFSMkdir_0005"
//@INPUT: nReserved = -1
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSMkdir(分区挂载路径 + "CSUDIFSMkdir_0005", -1)创建文件夹，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSMkdir_0005(void)
{
	char   szTest[] = "CSUDIFSMkdir_0005";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);
	
	CSTK_ASSERT_TRUE(-1 == CSUDIFSMkdir(szFilePath, -1), "创建文件夹不应当成功\n");
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}
/****************************CSUDIFSMkdir  End *******************************/

/****************************CSUDIFSRmdir  Start *****************************/ 
//@CASEGROUP: int CSUDIFSRmdir(const char * pcPathName)
//@DESCRIPTION:  正常删除文件夹	
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcPathName = 分区挂载路径 + "CSUDIFSRmdir_0001"
//@EXPECTATION: 返回值等于0
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSMkdir(分区挂载路径 + "CSUDIFSRmdir_0001", 0)
//@EXECUTIONFLOW: 4、调用CSUDIFSRmdir(分区挂载路径 + "CSUDIFSRmdir_0001")
//@EXECUTIONFLOW: 5、调用CSUDIFSOpendir(分区挂载路径 + "CSUDIFSRmdir_0001")检查是否确实已删除
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRmdir_0001(void)
{
	char   szTest[] = "CSUDIFSRmdir_0001";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE     hDir = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSMkdir(szFilePath, 0), "创建文件夹失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szFilePath), "删除文件夹失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == (hDir = CSUDIFSOpenDir(szFilePath)), "打开文件夹不应该成功\n");
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
	
}

//@CASEGROUP: int CSUDIFSRmdir(const char * pcPathName)
//@DESCRIPTION:  删除不存在的文件夹
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcPathName = 分区挂载路径 + "CSUDIFSRmdir_0002"
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSRmdir(分区挂载路径 + "CSUDIFSRmdir_0002")删除文件夹，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRmdir_0002(void)
{
	char   szTest[] = "CSUDIFSRmdir_0001";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSRmdir(szFilePath), "删除文件夹失败\n");
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSRmdir(const char * pcPathName)
//@DESCRIPTION:  删除空指针文件夹
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcPathName = CSUDI_NULL
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSRmdir(CSUDI_NULL)删除文件，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRmdir_0003(void)
{
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSRmdir(CSUDI_NULL), "删除文件夹不应当成功\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSRmdir(const char * pcPathName)
//@DESCRIPTION:  删除非空文件夹
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcPathName = 分区挂载路径 + "CSUDIFSRmdir_0004"
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSMkdir(分区挂载路径 + "CSUDIFSRmdir_0004", 0)创建文件夹
//@EXECUTIONFLOW: 4、调用CSUDIFSMkdir(分区挂载路径 + "CSUDIFSRmdir_0004"+"CSUDIFSRmdir_0004", 0)创建子文件夹
//@EXECUTIONFLOW: 5、调用CSUDIFSRmdir(分区挂载路径 + "CSUDIFSRmdir_0004")删除文件夹，应当失败
//@EXECUTIONFLOW: 6、调用CSUDIFSRmdir(分区挂载路径 + "CSUDIFSRmdir_0004"+"CSUDIFSRmdir_0004")删除子文件夹
//@EXECUTIONFLOW: 7、调用CSUDIFSRmdir(分区挂载路径 + "CSUDIFSRmdir_0004")删除文件夹
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRmdir_0004(void)
{
	char   szTest[] = "CSUDIFSRmdir_0004";   
	char   szFilePathA[CSUDI_MAX_FILE_NAME_LEN];
	char   szFilePathB[CSUDI_MAX_FILE_NAME_LEN];

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePathA, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePathA, g_szVolumeMountPoint);
	strcat(szFilePathA, szTest);
	
	memset(szFilePathB, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePathB, szFilePathA);
	strcat(szFilePathB, "/");
	strcat(szFilePathB, szTest);

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSMkdir(szFilePathA, 0), "创建文件夹失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSMkdir(szFilePathB, 0), "创建子文件夹失败\n");
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSRmdir(szFilePathA), "删除非空文件夹不应当成功\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szFilePathB), "删除文件夹失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szFilePathA), "删除文件夹失败\n");
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSRmdir(const char * pcPathName)
//@DESCRIPTION:  删除文件
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcPathName = 分区挂载路径 + "CSUDIFSRmdir_0005"
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSRmdir_0005", "w+")创建并打开文件，得到句柄hFile
//@EXECUTIONFLOW: 4、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 5、调用CSUDIFSRmdir(分区挂载路径 + "CSUDIFSRmdir_0005")删除文件，应当失败
//@EXECUTIONFLOW: 6、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSRmdir_0005")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRmdir_0005(void)
{
	char   szTest[] = "CSUDIFSRmdir_0005";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE   hFile = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "创建并打开文件失败\n");
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");	
	
	CSTK_ASSERT_TRUE(-1 == CSUDIFSRmdir(szFilePath), "删除文件夹失败\n");

	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "移除文件失败\n");
		}
	}
	return CSUDI_TRUE;		
	
}
/****************************CSUDIFSRmdir  End *******************************/

/****************************CSUDIFSOpenDir  Start *****************************/ 
//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpenDir(const char * pcPathName)
//@DESCRIPTION:  正常打开文件夹
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcPathName = 分区挂载路径 + "CSUDIFSOpenDir_0001"
//@EXPECTATION: 返回值不等于CSUDI_NULL
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSMkdir(分区挂载路径 + "CSUDIFSOpenDir_0001", 0)创建文件夹
//@EXECUTIONFLOW: 4、调用CSUDIFSOpenDir(分区挂载路径 + "CSUDIFSOpenDir_0001")打开文件夹，得到句柄hDir
//@EXECUTIONFLOW: 5、调用CSUDIFSCloseDir(hDir)关闭文件夹
//@EXECUTIONFLOW: 6、调用CSUDIFSRmdir(分区挂载路径 + "CSUDIFSOpenDir_0001")删除文件夹
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpenDir_0001(void)
{
	char   szTest[] = "CSUDIFSOpenDir_0001";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE     hDir = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSMkdir(szFilePath, 0), "创建文件夹失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hDir = CSUDIFSOpenDir(szFilePath)), "打开文件夹失败\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSCloseDir(hDir), "关闭文件夹失败\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szFilePath), "删除文件夹失败\n");
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpenDir(const char * pcPathName)
//@DESCRIPTION:  打开文件
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcPathName = 分区挂载路径 + "CSUDIFSOpenDir_0002"
//@EXPECTATION: 返回值等于CSUDI_NULL
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSOpenDir_0002", "w+")创建文件，得到句柄hFile
//@EXECUTIONFLOW: 4、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 5、调用CSUDIFSOpenDir(分区挂载路径 + "CSUDIFSOpenDir_0002")打开文件夹，应当失败
//@EXECUTIONFLOW: 6、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSOpenDir_0002")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpenDir_0002(void)
{
	char   szTest[] = "CSUDIFSOpenDir_0002";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE   hFile = CSUDI_NULL;
	CSUDI_HANDLE   hDir = CSUDI_NULL;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "创建并打开文件失败\n");
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == (hDir = CSUDIFSOpenDir(szFilePath)), "打开文件不应当成功\n");

	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "移除文件失败\n");
		}
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpenDir(const char * pcPathName)
//@DESCRIPTION:  打开空指针
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcPathName = CSUDI_NULL
//@EXPECTATION: 返回值等于CSUDI_NULL
//@EXECUTIONFLOW: 1、调用CSUDIFSOpenDir(CSUDI_NULL)打开文件夹，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpenDir_0003(void)
{
	CSTK_ASSERT_TRUE(CSUDI_NULL == CSUDIFSOpenDir(CSUDI_NULL), "打开文件夹不应当成功\n");

	CSTK_FATAL_POINT
	{
		;	
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: CSUDI_HANDLE CSUDIFSOpenDir(const char * pcPathName)
//@DESCRIPTION:  打开不存在的文件夹
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcPathName = 分区挂载路径 + "CSUDIFSOpenDir_0004"
//@EXPECTATION: 返回值等于CSUDI_NULL
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpenDir(分区挂载路径 + "CSUDIFSOpenDir_0004")打开文件夹，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSOpenDir_0004(void)
{
	char   szTest[] = "CSUDIFSOpenDir_0004";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);	

	CSTK_ASSERT_TRUE(CSUDI_NULL == CSUDIFSOpenDir(szFilePath), "打开不存在的文件夹不应当成功\n");

	CSTK_FATAL_POINT
	{
		;	
	}
	return CSUDI_TRUE;
}
/****************************CSUDIFSOpenDir  End *******************************/

/****************************CSUDIFSCloseDir  Start *****************************/ 
//@CASEGROUP: int CSUDIFSCloseDir(CSUDI_HANDLE hDir)
//@DESCRIPTION:  正常关闭文件夹
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hDir = hDir1， hDir1为CSUDIFSOpenDir()返回的合法值
//@EXPECTATION: 返回值等于0
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSMkdir(分区挂载路径 + "CSUDIFSCloseDir_0001", 0)创建文件夹
//@EXECUTIONFLOW: 4、调用CSUDIFSOpenDir(分区挂载路径 + "CSUDIFSCloseDir_0001")打开文件夹
//@EXECUTIONFLOW: 5、调用CSUDIFSCloseDir(hDir)关闭文件夹，应当成功
//@EXECUTIONFLOW: 6、调用CSUDIFSRmdir(分区挂载路径 + "CSUDIFSCloseDir_0001")删除文件夹
CSUDI_BOOL CSTC_FS_IT_CSUDIFSCloseDir_0001(void)
{
	char   szTest[] = "CSUDIFSCloseDir_0001";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE     hDir1 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSMkdir(szFilePath, 0), "创建文件夹失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hDir1 = CSUDIFSOpenDir(szFilePath)), "打开文件夹失败\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSCloseDir(hDir1), "关闭文件夹失败\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szFilePath), "删除文件夹失败\n");
	
	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSCloseDir(CSUDI_HANDLE hDir)
//@DESCRIPTION:  关闭空文件夹句柄
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hDir = CSUDI_NULL 
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSCloseDir(CSUDI_NULL)关闭文件夹，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSCloseDir_0002(void)
{
	CSTK_ASSERT_TRUE(-1 == CSUDIFSCloseDir(CSUDI_NULL), "关闭文件夹不应该成功\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSCloseDir(CSUDI_HANDLE hDir)
//@DESCRIPTION:  关闭文件句柄
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hDir = hFile, hDir1为CSUDIFSOpen()返回的句柄
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSCloseDir_0003", "w+")创建文件，得到句柄hFile
//@EXECUTIONFLOW: 4、调用CSUDIFSCloseDir(hFile)关闭文件夹，应当失败
//@EXECUTIONFLOW: 5、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 6、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSCloseDir_0003")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSCloseDir_0003(void)
{
	char   szTest[] = "CSUDIFSCloseDir_0003";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE   hFile = CSUDI_NULL;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "创建并打开文件失败\n");
	
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSCloseDir(hFile), "关闭文件不应当成功\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");	

	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "移除文件失败\n");
		}
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSCloseDir(CSUDI_HANDLE hDir)
//@DESCRIPTION:  关闭不存在的文件夹句柄
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hDir = 0x7FFFFFFF(假设0x7FFFFFFF为不存在的文件夹句柄)
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSCloseDir(0x7FFFFFFF)关闭文件夹，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSCloseDir_0004(void)
{
	CSTK_ASSERT_TRUE(-1 == CSUDIFSCloseDir((CSUDI_HANDLE)0x7FFFFFFF), "关闭文件夹不应该成功\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}
/****************************CSUDIFSCloseDir  End *******************************/

/****************************CSUDIFSReadDir  Start *****************************/ 
//@CASEGROUP: CSUDIFSDirent_S *CSUDIFSReadDir(CSUDI_HANDLE hDir)
//@DESCRIPTION:  读文件夹，文件夹下不为空
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hDir = hDir1, hDir1为CSUDIFSOpenDir()返回的合法值
//@EXPECTATION: 返回值不等于CSUDI_NULL
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSMkdir(分区挂载路径 + "CSUDIFSReadDir_0001", 0)创建文件夹
//@EXECUTIONFLOW: 4、调用CSUDIFSMkdir(分区挂载路径 + "CSUDIFSReadDir_0001" + "CSUDIFSReadDir_0001_sub", 0)创建子文件夹
//@EXECUTIONFLOW: 5、调用CSUDIFSOpenDir(分区挂载路径 + "CSUDIFSReadDir_0001")打开文件夹，得到句柄hDir1
//@EXECUTIONFLOW: 6、调用CSUDIFSReadDir(hDir1)读文件夹，应当成功，比较节点信息，查看子文件夹名和类型是否正确
//@EXECUTIONFLOW: 7、调用CSUDIFSCloseDir(hDir1)关闭文件夹
//@EXECUTIONFLOW: 8、调用CSUDIFSRmdir(分区挂载路径 + "CSUDIFSReadDir_0001" + "CSUDIFSReadDir_0001_sub")删除子文件夹
//@EXECUTIONFLOW: 9、调用CSUDIFSRmdir(分区挂载路径 + "CSUDIFSReadDir_0001")删除文件夹
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

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	memset(szSubFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szSubFilePath, szFilePath);
	strcat(szSubFilePath, "/");
	strcat(szSubFilePath, szSubTest);
	
	CSTK_ASSERT_TRUE_FATAL(0 == (nRet = CSUDIFSMkdir(szFilePath, 0)), "创建文件夹失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == (nSubRet = CSUDIFSMkdir(szSubFilePath, 0)), "创建子文件夹失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hDir1 = CSUDIFSOpenDir(szFilePath)), "打开文件夹失败\n");

	while(1)
	{
		psDirent = CSUDIFSReadDir(hDir1);
		if(psDirent != CSUDI_NULL && 0 != strcmp(psDirent->m_cDirName, ".") && 0 != strcmp(psDirent->m_cDirName, ".."))
		{
			CSTK_ASSERT_TRUE_FATAL(0 == strcmp(psDirent->m_cDirName,szSubTest), "读文件夹失败\n");
			CSTK_ASSERT_TRUE_FATAL(1 == psDirent->m_ucDirType, "读文件夹失败\n");
			break;
		}
	}

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSCloseDir(hDir1), "关闭文件夹失败\n")

	CSTK_FATAL_POINT
	{
		if(nSubRet == 0)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szSubFilePath), "删除子文件夹失败\n");
		}	
		if(nRet == 0)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szFilePath), "删除文件夹失败\n");
		}	
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIFSDirent_S *CSUDIFSReadDir(CSUDI_HANDLE hDir)
//@DESCRIPTION: 读完文件夹下的所有结点
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hDir = hDir1, hDir1为CSUDIFSOpenDir()返回的合法值
//@EXPECTATION: 每一步都返回预期结果
//@EXECUTIONFLOW:1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW:2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW:3、调用CSUDIFSMkdir创建文件夹，要求返回成功
//@EXECUTIONFLOW:4、循环32次调用CSUDIFSMkdir创建子文件夹，要求返回成功
//@EXECUTIONFLOW:5、循环256次调用CSUDIFSOpen创建并打开文件，要求返回成功
//@EXECUTIONFLOW:6、调用CSUDIFSOpenDir打开文件夹，要求返回成功
//@EXECUTIONFLOW:7、调用CSUDIFSReadDir读文件夹，期望个数为130
//@EXECUTIONFLOW:8、调用CSUDIFSCloseDir关闭文件夹，要求返回成功
//@EXECUTIONFLOW:9、循环32次调用CSUDIFSRmdir移除子文件夹，要求返回成功
//@EXECUTIONFLOW:10、循环256次调用CSUDIFSClose关闭文件，要求返回成功
//@EXECUTIONFLOW:11、循环256次调用CSUDIFSRemove移除文件，要求返回成功
//@EXECUTIONFLOW:12、调用CSUDIFSRmdir移除文件夹，要求返回成功
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

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	memset(szSubFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szSubFilePath, szFilePath);
	strcat(szSubFilePath, "/");
	strcat(szSubFilePath, szSubTest);
	nLen = strlen(szSubFilePath);

	CSTK_ASSERT_TRUE_FATAL(0 == (nDirRet = CSUDIFSMkdir(szFilePath, 0)), "创建文件夹失败");

	for (i=0; i<FS_TEST_MAX_DIR_CNT; i++)
	{
		sprintf(szSubFilePath+nLen, "_Dir_%03d", i+1);
		CSTK_ASSERT_TRUE_FATAL(0 == (anSubDirRet[i] = CSUDIFSMkdir(szSubFilePath, 0)), "创建子文件夹失败");
	}
	for (i=0; i<FS_TEST_MAX_FILE_CNT; i++)
	{
		sprintf(szSubFilePath+nLen, "_File_%03d", i+1);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (ahFile[i] = CSUDIFSOpen(szSubFilePath, "w+")), "创建并打开文件失败");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hDir = CSUDIFSOpenDir(szFilePath)), "打开文件夹失败");
	while (1)
	{
		psDirent = CSUDIFSReadDir(hDir);
		if(psDirent == CSUDI_NULL )
		{
			break;
		}
		nNodeCount++;
	}

	CSTK_ASSERT_TRUE_FATAL((2+FS_TEST_MAX_DIR_CNT+FS_TEST_MAX_FILE_CNT) == nNodeCount, "遍历次数不对");

	CSTK_FATAL_POINT
	if (hDir != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSCloseDir(hDir), "关闭文件夹失败");
	}
	for (i=0; i<FS_TEST_MAX_DIR_CNT; i++)
	{
		if (anSubDirRet[i] == 0)
		{
			sprintf(szSubFilePath+nLen, "_Dir_%03d", i+1);
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szSubFilePath), "移除子文件夹失败");
		}
	}
	for (i=0; i<FS_TEST_MAX_FILE_CNT; i++)
	{
		if (ahFile[i] != CSUDI_NULL)
		{
			sprintf(szSubFilePath+nLen, "_File_%03d", i+1);
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(ahFile[i]), "关闭文件失败");
			CSTK_ASSERT_TRUE(0 == CSUDIFSRemove(szSubFilePath), "移除文件失败");
		}
	}
	if (nDirRet == 0)
	{
		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szFilePath), "移除文件夹失败");
	}	

	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIFSDirent_S *CSUDIFSReadDir(CSUDI_HANDLE hDir)
//@DESCRIPTION:  从空文件夹句柄读结点信息
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hDir = CSUDI_NULL
//@EXPECTATION: 返回值等于CSUDI_NULL
//@EXECUTIONFLOW: 1、调用CSUDIFSReadDir(CSUDI_NULL)读取文件夹，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSReadDir_0003(void)
{
	CSTK_ASSERT_TRUE(CSUDI_NULL == CSUDIFSReadDir(CSUDI_NULL),"读取不存在的文件夹不应当成功\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIFSDirent_S *CSUDIFSReadDir(CSUDI_HANDLE hDir)
//@DESCRIPTION:  从不存在的文件夹句柄读
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hDir = 0x7FFFFFFF(假设0x7FFFFFFF为不存在的文件夹句柄)
//@EXPECTATION: 返回值等于CSUDI_NULL
//@EXECUTIONFLOW: 1、调用CSUDIFSReadDir(0x7FFFFFFF)，读取文件夹，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSReadDir_0004(void)
{
	CSTK_ASSERT_TRUE(CSUDI_NULL == CSUDIFSReadDir((CSUDI_HANDLE)0x7FFFFFFF),"读取不存在的文件夹不应当成功\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIFSDirent_S *CSUDIFSReadDir(CSUDI_HANDLE hDir)
//@DESCRIPTION:  读文件
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hDir = hFile, hFile为CSUDIFSOpen()返回的句柄
//@EXPECTATION: 返回值等于CSUDI_NULL
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSReadDir_0005", "w+")创建文件，得到句柄hFile
//@EXECUTIONFLOW: 4、调用CSUDIFSReadDir(hFile)读文件夹
//@EXECUTIONFLOW: 5、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 6、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSReadDir_0005")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSReadDir_0005(void)
{
	char   szTest[] = "CSUDIFSReadDir_0005";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE   hFile = CSUDI_NULL;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "创建并打开文件失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == CSUDIFSReadDir(hFile), "打开文件不应当成功\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");	

	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "移除文件失败\n");
		}
	}
	return CSUDI_TRUE;	
}
/****************************CSUDIFSReadDir  End *******************************/

/****************************CSUDIFSStatfs  Start *****************************/ 
//@CASEGROUP: int CSUDIFSStatfs(const char * pcPathName, CSUDIFSStatFs_S * psFsStat)
//@DESCRIPTION:  通过文件获取分卷信息
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcPathName = 分区挂载路径 + "CSUDIFSStatfs_0001"
//@INPUT: psFsStat = &sFsStat，其中CSUDIFSStatFs_S  sFsStat
//@EXPECTATION:  返回值等于0
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径 + "CSUDIFSStatfs_0001", "w+")创建并打开文件，得到句柄hFile
//@EXECUTIONFLOW: 4、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 5、调用CSUDIFSStatfs(分区挂载路径 + "CSUDIFSStatfs_0001", &sFsStat)获取文件系统信息
//@EXECUTIONFLOW: 6、调用CSUDIFSRemove(分区挂载路径 + "CSUDIFSStatfs_0001")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSStatfs_0001(void)
{
	char   szTest[] = "CSUDIFSStatfs_0001";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE   hFile = CSUDI_NULL;
	CSUDIFSStatFs_S  sFsStat;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "创建并打开文件失败\n");
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");	

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStatfs(szFilePath, &sFsStat), "获取文件系统信息失败\n");
	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "移除文件失败\n");
		}
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSStatfs(const char * pcPathName, CSUDIFSStatFs_S * psFsStat)
//@DESCRIPTION:  通过文件夹获取分卷信息
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcPathName = 分区挂载路径 + "CSUDIFSStatfs_0002"
//@INPUT: psFsStat = &sFsStat，其中CSUDIFSStatFs_S  sFsStat
//@EXPECTATION: 返回值等于0
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSMkdir(分区挂载路径 + "CSUDIFSStatfs_0002", 0)创建文件夹
//@EXECUTIONFLOW: 4、调用CSUDIFSStatfs(分区挂载路径 + "CSUDIFSStatfs_0002", &sFsStat)获取文件系统信息，应当成功
//@EXECUTIONFLOW: 5、调用CSUDIFSRmdir(分区挂载路径 + "CSUDIFSStatfs_0002")删除文件夹
CSUDI_BOOL CSTC_FS_IT_CSUDIFSStatfs_0002(void)
{
	char   szTest[] = "CSUDIFSStatfs_0002";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDIFSStatFs_S  sFsStat;

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSMkdir(szFilePath, 0), "创建文件夹失败\n");

	CSTK_ASSERT_TRUE(0 == CSUDIFSStatfs(szFilePath, &sFsStat), "获取文件系统信息失败\n");
	
	CSTK_ASSERT_TRUE(0 == CSUDIFSRmdir(szFilePath), "删除文件夹失败\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSStatfs(const char * pcPathName, CSUDIFSStatFs_S * psFsStat)
//@DESCRIPTION:  通过空指针路径获取分卷信息
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcPathName = CSUDI_NULL
//@INPUT: psFsStat = &sFsStat，其中CSUDIFSStatFs_S  sFsStat
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSStatfs(CSUDI_NULL, &sFsStat)获取文件系统信息，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSStatfs_0003(void)
{
	CSUDIFSStatFs_S  sFsStat;

	CSTK_ASSERT_TRUE(-1 == CSUDIFSStatfs(CSUDI_NULL, &sFsStat), "获取文件系统信息不应该成功\n");	

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSStatfs(const char * pcPathName, CSUDIFSStatFs_S * psFsStat)
//@DESCRIPTION:  通过不存在的路径获取分卷信息
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcPathName = 分区挂载路径 + "CSUDIFSStatfs_0004"
//@INPUT: psFsStat = &sFsStat，其中CSUDIFSStatFs_S  sFsStat
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSStatfs(分区挂载路径 + "CSUDIFSStatfs_0004", &sFsStat)获取文件系统信息
CSUDI_BOOL CSTC_FS_IT_CSUDIFSStatfs_0004(void)
{
	char   szTest[] = "CSUDIFSStatfs_0004";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDIFSStatFs_S  sFsStat;

	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE(-1 == CSUDIFSStatfs(szFilePath, &sFsStat), "获取文件系统信息不应当成功\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSStatfs(const char * pcPathName, CSUDIFSStatFs_S * psFsStat)
//@DESCRIPTION:  接收分卷信息的指针为空
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: pcPathName = 分区挂载路径 + "CSUDIFSStatfs_0005"
//@INPUT: psFsStat = &sFsStat，其中CSUDIFSStatFs_S  sFsStat
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSMkdir(分区挂载路径 + "CSUDIFSStatfs_0005", 0)创建文件夹
//@EXECUTIONFLOW: 4、调用CSUDIFSStatfs(分区挂载路径 + "CSUDIFSStatfs_0005", CSUDI_NULL)获取文件系统信息，应当失败
//@EXECUTIONFLOW: 5、调用CSUDIFSRmdir(分区挂载路径 + "CSUDIFSStatfs_0005")删除文件夹
CSUDI_BOOL CSTC_FS_IT_CSUDIFSStatfs_0005(void)
{
	char   szTest[] = "CSUDIFSStatfs_0005";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);
	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSMkdir(szFilePath, 0), "创建文件夹失败\n");
	CSTK_ASSERT_TRUE(-1 == CSUDIFSStatfs(szFilePath, CSUDI_NULL), "获取文件系统信息不应当成功\n");
	CSTK_ASSERT_TRUE(0 == CSUDIFSRmdir(szFilePath), "删除文件夹失败\n");

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
			CSFSPrint("从底层获取的文件系统类型不得为EM_UDIFS_PARTITION_AUTO类型\n");
			bRet = CSUDI_FALSE;
			break;
		case EM_UDIFS_PARTITION_CRAMFS:
			CSFSPrint("文件系统类型:CRAMFS类分区\n");
			break;
		case EM_UDIFS_PARTITION_ROMFS:
			CSFSPrint("文件系统类型:ROMFS类分区\n");
			break;
		case EM_UDIFS_PARTITION_FAT32:
			CSFSPrint("文件系统类型:FAT32类型分区\n");
			break;
		case EM_UDIFS_PARTITION_EXT2:
			CSFSPrint("文件系统类型:文件系统类型:E2FS类型分区\n");
			break;
		case EM_UDIFS_PARTITION_EXT3:
			CSFSPrint("文件系统类型:EXT3类型分区\n");
			break;
		case EM_UDIFS_PARTITION_JFFS2:
			CSFSPrint("文件系统类型:JFFS2类型分区\n");
			break;
		case EM_UDIFS_PARTITION_NTFS:
			CSFSPrint("文件系统类型:NTFS类型分区\n");
			break;
		case EM_UDIFS_PARTITION_UBIFS:
			CSFSPrint("文件系统类型:UBIFS类型分区\n");
			break;
		case EM_UDIFS_PARTITION_YAFFS2:
			CSFSPrint("文件系统类型:YAFFS2类型分区\n");
			break;
		default:
			CSFSPrint("获取文件系统类型错误\n");
			bRet = CSUDI_FALSE;
			break;
	}

	return bRet;
}

//@CASEGROUP: int CSUDIFSFstatfs(CSUDI_HANDLE hFile, CSUDIFSStatFs_S * psFsStat)
//@DESCRIPTION:  正常获取分卷信息
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = hFile1，hFile1为合法的文件句柄，由CSUDIOpen()返回
//@INPUT: psFsStat = &sFsStat，其中CSUDIFSStatFs_S  sFsStat
//@EXPECTATION: 获取成功，且分卷总大小、已用空间、空闲大小与电脑上一致
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径+"CSUDIFSFstatfs_0001", "w+")创建并打开文件，得到句柄hFile1
//@EXECUTIONFLOW: 4、调用CSUDIFSFstatfs(hFile1, &sFsStat)获取文件系统信息，应当获取成功，且分卷总大小、已用空间、空闲大小与电脑上一致
//@EXECUTIONFLOW: 5、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 6、调用CSUDIFSRemove(分区挂载路径+"CSUDIFSFstatfs_0001")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFstatfs_0001(void)
{
	//手动测试
	char   szTest[] = "CSUDIFSFstatfs_0001";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	CSUDIFSStatFs_S  sFsStat;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(szFilePath, "w+")), "创建并打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSFstatfs(hFile1, &sFsStat), "获取文件系统信息失败\n");

	CSFSPrint("分卷信息如下:\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == FS_iPrintPartitionType(sFsStat.m_eFsType), "获取文件系统类型错误\n");
	CSFSPrint("块大小: %ld  总块数: %ld  空闲块数: %ld  非超级用户合法可用块数：%ld \n", sFsStat.m_lBsize, sFsStat.m_lBlocks, sFsStat.m_lBfree, sFsStat.m_lBavail);

	CSFSPrint("分卷总大小：%.3f Byte 空闲大小: %.3f Byte 非超级用户可用大小: %.3f Byte \n", (double)sFsStat.m_lBlocks*sFsStat.m_lBsize, (double)sFsStat.m_lBfree*sFsStat.m_lBsize , (double)sFsStat.m_lBavail*sFsStat.m_lBsize);
	CSFSPrint("以上信息是否和分卷在PC机上一致，如果是则按Y, 如果不是则按N\n");

	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "你选择了文件系统信息不对，测试没有通过\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");	
	
	
	
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "移除文件失败\n");
		}
	}
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSFstatfs(CSUDI_HANDLE hFile, CSUDIFSStatFs_S * psFsStat)
//@DESCRIPTION:  从空文件句柄获取分卷信息
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = CSUDI_NULL
//@INPUT: psFsStat = &sFsStat，其中CSUDIFSStatFs_S  sFsStat
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSFstatfs(CSUDI_NULL, &sFsStat)获取文件系统信息，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFstatfs_0002(void)
{
	CSUDIFSStatFs_S  sFsStat;
	
	CSTK_ASSERT_TRUE(-1 == CSUDIFSFstatfs(CSUDI_NULL, &sFsStat), "获取文件系统信息不应当成功\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSFstatfs(CSUDI_HANDLE hFile, CSUDIFSStatFs_S * psFsStat)
//@DESCRIPTION:  不存在的句柄获取分卷信息
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = 0x7FFFFFFF(假设0x7FFFFFFF为非法的文件句柄)
//@INPUT: psFsStat = &sFsStat，其中CSUDIFSStatFs_S  sFsStat
//@EXPECTATION: 返回值等于-1
//@EXECUTIONFLOW: 1、调用CSUDIFSFstatfs(0x7FFFFFFF, &sFsStat)获取文件系统信息，应当失败
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFstatfs_0003(void)
{
	CSUDIFSStatFs_S  sFsStat;
	
	CSTK_ASSERT_TRUE(-1 == CSUDIFSFstatfs((CSUDI_HANDLE)0x7FFFFFFF, &sFsStat), "获取文件系统信息不应当成功\n");

	CSTK_FATAL_POINT
	{
		;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: int CSUDIFSFstatfs(CSUDI_HANDLE hFile, CSUDIFSStatFs_S * psFsStat)
//@DESCRIPTION:  通过文件夹句柄获取分卷信息
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = hDir, hDir为CSUDIFSMkdir()返回的句柄
//@INPUT: psFsStat = &sFsStat，其中CSUDIFSStatFs_S  sFsStat
//@EXPECTATION: 获取分卷信息失败
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSMkdir(分区挂载路径 + "CSUDIFSFstatfs_0004", 0)创建文件夹
//@EXECUTIONFLOW: 4、调用CSUDIFSOpenDir(分区挂载路径 + "CSUDIFSFstatfs_0004")打开文件夹，得到句柄hDir
//@EXECUTIONFLOW: 5、调用CSUDIFSStatfs(hDir, &sFsStat)获取文件系统信息，应当失败，返回值为-1
//@EXECUTIONFLOW: 6、调用CSUDIFSCloseDir(hDir)关闭文件夹
//@EXECUTIONFLOW: 7、调用CSUDIFSRmdir(分区挂载路径 + "CSUDIFSFstatfs_0004")删除文件夹
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFstatfs_0004(void)
{
	char   szTest[] = "CSUDIFSFstatfs_0004";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE   hDir = CSUDI_NULL;
	CSUDIFSStatFs_S  sFsStat;
	int    nRet = -1;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);	

	CSTK_ASSERT_TRUE_FATAL(0 == (nRet = CSUDIFSMkdir(szFilePath, 0)), "创建文件夹失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hDir = CSUDIFSOpenDir(szFilePath)), "打开文件夹失败\n");

	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSFstatfs(hDir, &sFsStat), "获取文件夹信息不应该成功\n");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSCloseDir(hDir), "关闭文件夹失败\n");

	CSTK_FATAL_POINT
	{
		if(nRet == 0)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szFilePath), "删除文件夹失败\n");
		}
	}
	
	return CSUDI_TRUE;	
}

//@CASEGROUP: int CSUDIFSFstatfs(CSUDI_HANDLE hFile, CSUDIFSStatFs_S * psFsStat)
//@DESCRIPTION:  接收分卷信息的指针为空
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile = hFile1，hFile1为合法的文件句柄，由CSUDIOpen()返回
//@INPUT: psFsStat = CSUDI_NULL
//@EXPECTATION: 获取分卷信息失败
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径+"CSUDIFSFstatfs_0005", "w+")创建并打开文件，得到句柄hFile1
//@EXECUTIONFLOW: 4、调用CSUDIFSFstatfs(hFile1, CSUDI_NULL)获取文件信息，应当失败
//@EXECUTIONFLOW: 5、调用CSUDIFSClose(hFile1)关闭文件
//@EXECUTIONFLOW: 6、调用CSUDIFSRemove(分区挂载路径+"CSUDIFSFstatfs_0005")删除文件
CSUDI_BOOL CSTC_FS_IT_CSUDIFSFstatfs_0005(void)
{
	char   szTest[] = "CSUDIFSFstatfs_0005";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE   hFile1 = CSUDI_NULL;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile1 = CSUDIFSOpen(szFilePath, "w+")), "创建并打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIFSFstatfs(hFile1, CSUDI_NULL), "获取文件系统信息不应当成功\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile1), "关闭文件失败\n");	
	
	CSTK_FATAL_POINT
	{
		if(hFile1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSRemove(szFilePath), "移除文件失败\n");
		}
	}
	return CSUDI_TRUE;	
}

//@DESCRIPTION: 测试系统读写文件的性能
//@PRECONDITION: 测试系统对文件的读取，写入以获得文件的读写速率
//@INPUT: hFile为合法的文件句柄，由CSUDIOpen()返回
//@INPUT: pucMBuffer,pucKBuffer文件写入和读取所使用的内存缓冲区
//@EXPECTATION: 获取分卷信息失败
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径+"FSWriteReadTest", "w+")创建并打开文件，得到句柄hFile
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(),每次写入1MB，循环10次,并获取运行时间
//@EXECUTIONFLOW: 5、调用CSUDIFSRead(), 每次读入1MB，循环10次,并获取运行时间
//@EXECUTIONFLOW: 6、调用CSUDIFSWrite(),每次写入10K，循环100次,并获取运行时间
//@EXECUTIONFLOW: 7、调用CSUDIFSRead(),每次写入10K，循环100次,并获取运行时间
//@EXECUTIONFLOW: 8、打印运行结果，并销毁资源
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
//		CSFSPrint("读取配置文件中的CS_FSUDI2_TEST_START_ADDRESS字段失败\n");
//		return CSUDI_FALSE;
//	}
//	
//	uTestStartAddress = CSTKGetIntFromStr(cField, 16);
//
//	memset(cField, '\0', sizeof(cField));
//	
//	if(CS_TK_CONFIG_SUCCESS != CSTKGetConfigInfo("FS", "CS_FSUDI2_TEST_FLASH_SIZE", cField, sizeof(cField)))
//	{
//		CSFSPrint("读取配置文件中的CS_FSUDI2_TEST_FLASH_SIZE字段失败\n");
//		return CSUDI_FALSE;
//	}
//	
//	uTestFlashSize = CSTKGetIntFromStr(cField, 16);
//
//	memset(cField, '\0', sizeof(cField));
//	
//	if(CS_TK_CONFIG_SUCCESS != CSTKGetConfigInfo("FS", "CS_FSUDI2_TEST_FS_BLOCK", cField, sizeof(cField)))
//	{
//		CSFSPrint("读取配置文件中的CS_FSUDI2_TEST_FS_BLOCK字段失败\n");
//		return CSUDI_FALSE;
//	}	
//
//	CSFSPrint("正在擦除指定flash分区，请等待!!!\n");
//
//	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(uTestStartAddress, uTestFlashSize), "擦除指定flash分区失败!!!");
//
//	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSAddPartition(cField, ePartitionType), "挂载分区设备失败!!!");
//
//	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSUDIFSGetDeviceVolumeMountPoint(), "获取分区挂载点失败\n");
//
//	strcat(g_szVolumeMountPoint, szTest);
//
//	pcBufferByMB= (char *)CSUDIOSMalloc(uReadWriteByMB);
//	
//    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pcBufferByMB, "分配1M内存失败");
//	
//    memset(pcBufferByMB, 0x55, uReadWriteByMB);
//
//	hFile = CSUDIFSOpen(g_szVolumeMountPoint, "w+");
//
//    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hFile, "创建并打开文件失败\n");
//
//	CSFSPrint("正在进行以1MB为单位的写入操作，请等待!!!\n");
//    //每次写入1MB，循环10次
//    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sStartTime), "获取以1MB为单位的写入操作的开始时间失败");
//	
//	if(uTestFlashSize >= (uLoopTimeByMB * uReadWriteByMB))
//	{
//	    for( i = 0; i < uLoopTimeByMB; i++)
//	    {	
//	    	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile, i*uReadWriteByMB, CSUDIFS_SEEK_SET), "第四步重置文件失败\n");
//	        CSTK_ASSERT_TRUE_FATAL(uReadWriteByMB == CSUDIFSWrite(hFile, pcBufferByMB, uReadWriteByMB), "第四步写入文件失败\n");
//	    }
//	}
//	else
//	{
//		CSTK_ASSERT_TRUE_FATAL(uReadWriteByMB <= uTestFlashSize, "挂载的分区size过小，没办法进行性能测试\n");
//		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile, 0, CSUDIFS_SEEK_SET), "第四步重置文件失败\n");
//		for( i = 0; i < uLoopTimeByMB; i++)
//		{
//			CSTK_ASSERT_TRUE_FATAL(uReadWriteByMB == CSUDIFSWrite(hFile, pcBufferByMB, uReadWriteByMB), "第四步写入文件失败");;
//		}
//	}
//	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sStopTime), "获取以1MB为单位的写入操作的结束时间失败");
//	
//	sCostTime.m_nSecond = sStopTime.m_nSecond- sStartTime.m_nSecond;
//	sCostTime.m_nMiliSecond = sStopTime.m_nMiliSecond- sStartTime.m_nMiliSecond;
//	sCostTime.m_lUSecond = sStopTime.m_lUSecond- sStartTime.m_lUSecond;
//
//	lSpeedPerSecond = uLoopTimeByMB*1000.00/((sStopTime.m_nSecond*1000.00+sStopTime.m_nMiliSecond)-(sStartTime.m_nSecond*1000.00+sStartTime.m_nMiliSecond));
//	
//	CSFSPrint("每次写入1MB，循环10次的速率为:%.2f MB/s\r\n\n", lSpeedPerSecond);
//
//	CSFSPrint("正在进行以1MB为单位的读取操作，请等待!!!\n");
//    //每次读出1MB,循环10次
//    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sStartTime), "获取以1MB为单位的读入操作的开始时间失败");
//
//	if(uTestFlashSize >= (uLoopTimeByMB * uReadWriteByMB))
//	{
//	    for( i = 0; i < uLoopTimeByMB; i++)
//	    {	
//	    	memset(pcBufferByMB, 0x00, uReadWriteByMB);
//	    	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile, i*uReadWriteByMB, CSUDIFS_SEEK_SET), "第四步重置文件失败\n");
//	        CSTK_ASSERT_TRUE_FATAL(uReadWriteByMB == CSUDIFSRead(hFile, pcBufferByMB, uReadWriteByMB), "第四步读取文件失败\n");
//			for(j = 0; j < uReadWriteByMB; j++)
//			{
//            	CSTK_ASSERT_TRUE_FATAL(0x55 == pcBufferByMB[j], "第五步校验数据失败\n");
//        	}
//	    }
//	}
//	else
//	{
//		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile, 0, CSUDIFS_SEEK_SET), "第四步重置文件失败\n");
//		for( i = 0; i < uLoopTimeByMB; i++)
//		{
//			memset(pcBufferByMB, 0x00, uReadWriteByMB);
//			CSTK_ASSERT_TRUE_FATAL(uReadWriteByMB == CSUDIFSRead(hFile, pcBufferByMB, uReadWriteByMB), "第四步读取文件失败");;
//			for(j = 0; j < uReadWriteByMB; j++)
//			{
//            	CSTK_ASSERT_TRUE_FATAL(0x55 == (UINT8)pcBufferByMB[j], "第五步校验数据失败\n");
//        	}
//		}
//	}
//	
//	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sStopTime), "获取以1MB为单位的读取操作的结束时间失败");
//	
//	sCostTime.m_nSecond = sStopTime.m_nSecond- sStartTime.m_nSecond;
//	sCostTime.m_nMiliSecond = sStopTime.m_nMiliSecond- sStartTime.m_nMiliSecond;
//	sCostTime.m_lUSecond = sStopTime.m_lUSecond- sStartTime.m_lUSecond;
//	
//	lSpeedPerSecond = uLoopTimeByMB*1000.00/((sStopTime.m_nSecond*1000.00+sStopTime.m_nMiliSecond)-(sStartTime.m_nSecond*1000.00+sStartTime.m_nMiliSecond));
//	
//	CSFSPrint("每次读取1MB，循环10次的速率为:%.2f MB/s\r\n\n", lSpeedPerSecond);
//	
//	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");
//
//	CSFSPrint("正在进行以10KB为单位的写入操作，请等待!!!\n");
//    //每次写入10KB，循环100次
//	pcBufferByKB = (char *)CSUDIOSMalloc(uReadWriteByKB);
//	
//    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pcBufferByKB,"分配内存2失败");
//	
//    memset(pcBufferByKB, 0x88, uReadWriteByKB);
//	
//	hFile = CSUDIFSOpen(g_szVolumeMountPoint, "w+");
//	
//    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hFile, "打开文件失败\n");
//
//    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sStartTime), "获取以10KB为单位的写入操作的开始时间失败");
//	
//    for(i =0; i < uLoopTimeByKB; i++)
//    {
//    	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile, i*uLoopTimeByKB , CSUDIFS_SEEK_SET), "第六步重置文件失败\n");
//        CSTK_ASSERT_TRUE_FATAL(uReadWriteByKB == CSUDIFSWrite(hFile, pcBufferByKB, uReadWriteByKB), "第六步写入文件失败\n");
//    }
//
//	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sStopTime), "获取以10KB为单位的写入操作的结束时间失败");
//	
//	sCostTime.m_nSecond = sStopTime.m_nSecond- sStartTime.m_nSecond;
//	sCostTime.m_nMiliSecond = sStopTime.m_nMiliSecond- sStartTime.m_nMiliSecond;
//	sCostTime.m_lUSecond = sStopTime.m_lUSecond- sStartTime.m_lUSecond;
//	
//	lSpeedPerSecond = (uLoopTimeByKB*uReadWriteByKB)*1000.00/(((sStopTime.m_nSecond*1000.00+sStopTime.m_nMiliSecond)-(sStartTime.m_nSecond*1000.00+sStartTime.m_nMiliSecond))*M);
//	
//	CSFSPrint("每次写入10KB，循环100次的速率为:%.2f MB/s\r\n\n", lSpeedPerSecond);
//
//    //每次读入10KB，循环100次
//    CSFSPrint("正在进行以10KB为单位的读取操作，请等待!!!\n");
//    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sStartTime), "获取以10KB为单位的写入操作的开始时间失败");
//	
//	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile, 0 , CSUDIFS_SEEK_SET), "第七步重置文件失败\n");
//	
//    for(i =0; i < uLoopTimeByKB; i++)
//    {  
//    	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile, i*uLoopTimeByKB , CSUDIFS_SEEK_SET), "第六步重置文件失败\n");
//        CSTK_ASSERT_TRUE_FATAL(uReadWriteByKB == CSUDIFSRead(hFile, pcBufferByKB, uReadWriteByKB), "第七步读取数据失败\n");
//
//        for(j = 0; j < uReadWriteByKB; j++)
//        {
//            CSTK_ASSERT_TRUE_FATAL(0x88 == (UINT8)pcBufferByKB[j] , "第七步校验数据失败\n");
//        }
//    }
//	
//	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sStopTime), "获取以10KB为单位的写入操作的结束时间失败");
//	
//	sCostTime.m_nSecond = sStopTime.m_nSecond- sStartTime.m_nSecond;
//	sCostTime.m_nMiliSecond = sStopTime.m_nMiliSecond- sStartTime.m_nMiliSecond;
//	sCostTime.m_lUSecond = sStopTime.m_lUSecond- sStartTime.m_lUSecond;
//	
//	lSpeedPerSecond = (uLoopTimeByKB*uReadWriteByKB)*1000.00/(((sStopTime.m_nSecond*1000.00+sStopTime.m_nMiliSecond)-(sStartTime.m_nSecond*1000.00+sStartTime.m_nMiliSecond))*M);
//	
//	CSFSPrint("每次读取10KB，循环100次的速率为:%.2f MB/s\r\n\n", lSpeedPerSecond);
//   	
//	CSTK_FATAL_POINT
//	{
//		if(pcBufferByMB != CSUDI_NULL)
//		{
//	    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pcBufferByMB),"销毁资源1失败");
//		}
//		if(pcBufferByKB != CSUDI_NULL)
//		{
//	    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pcBufferByKB),"销毁资源2失败");
//		}
//		if(hFile != CSUDI_NULL)
//		{
//			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");
//		}
//		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRemove(g_szVolumeMountPoint), "移除挂载点失败\n");
//	}	
	return CSUDI_TRUE;	
}


//@DESCRIPTION: USB下对长文件名文件的支持
//@PRECONDITION: 测试系统对长文件名文件的打开，写入关闭操作，长文件名除包括8.3格式外，还要能支持中文，英文，符号及非8.3格式的文件?
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile为合法的文件句柄，由CSUDIOpen()返回
//@EXPECTATION:  打开，写入和关闭操作成功
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径+"coshipfs.txt", "w+")创建并打开文件，得到句柄hFile
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile, "coship", strlen("coship"))写入数据
//@EXECUTIONFLOW: 5、调用CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");
//@EXECUTIONFLOW: 6、调用CSUDIFSOpen(分区挂载路径+"深圳市同洲电子欢迎您.txt", "w+")创建并打开文件，得到句柄hFile
//@EXECUTIONFLOW: 7、调用CSUDIFSWrite(hFile, "深圳市同洲电子欢迎您.txt", strlen("深圳市同洲电子欢迎您.txt"))写入数据
//@EXECUTIONFLOW: 8、调用CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");
//@EXECUTIONFLOW: 9、调用CSUDIFSOpen(分区挂载路径+"深圳市同洲电子欢迎您 welcome to coship 2012-12-12.txt", "w+")创建并打开文件，得到句柄hFile	
//@EXECUTIONFLOW: 10、调用CSUDIFSWrite(hFile, "深圳市同洲电子欢迎您 welcome to coship 2012-12-12.txt", strlen("深圳市同洲电子欢迎您 welcome to coship 2010-12-12.txt"))写入数据
//@EXECUTIONFLOW: 11、调用CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");
//@EXECUTIONFLOW: 12、调用CSUDIFSRemove(分区挂载路径+"CSUDIFSFstat_0001")删除文件
CSUDI_BOOL CSTC_FS_IT_LongNameSupport_0001(void)
{
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	char   szFilenamea[] = "coshipfs.txt";   
	char   szFilenameb[] = "深圳市同洲电子欢迎您.txt";  
	char   szFilenamec[] = "深圳市同洲电子欢迎您 welcome to coship 2012-12-12.txt";  
	char   szBuf[100];
	CSUDI_HANDLE   hFile = CSUDI_NULL;
	int    nLen = 0;
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");
	
	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szFilenamea);	
	nLen = strlen(szFilenamea);

	memset(szBuf,0,sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "创建并打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(strlen("coship") == CSUDIFSWrite(hFile, "coship", strlen("coship")), "写入文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "r")), "打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL( strlen("coship")==CSUDIFSRead(hFile, szBuf, strlen("coship")), "读取数据失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == memcmp("coship", szBuf,strlen("coship")), "读出数据不是写入时的字串\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");
    CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "移除文件失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	memset(szBuf,0,sizeof(szBuf)); 
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szFilenameb);
	nLen = strlen(szFilenameb);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "创建并打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(nLen== CSUDIFSWrite(hFile, szFilenameb, nLen ), "写入文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "r")), "打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(nLen==CSUDIFSRead(hFile, szBuf, nLen), "读取数据失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == memcmp(szFilenameb, szBuf,nLen), "读出数据不是写入时的字串\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");	
    CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "移除文件失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	memset(szBuf,0,sizeof(szBuf));
	strcpy(szFilePath, g_szVolumeMountPoint);
	nLen = strlen(szFilenamec);
	strcat(szFilePath, szFilenamec);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "创建并打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szFilenamec, nLen), "写入文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "r")), "打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(nLen==CSUDIFSRead(hFile, szBuf, nLen), "读取数据失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == memcmp(szFilenamec, szBuf,nLen), "读出数据不是写入时的字串\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");	
    CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "移除文件失败\n");
    hFile = CSUDI_NULL;

	CSTK_FATAL_POINT
	if(hFile != CSUDI_NULL)
	{
	CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "移除文件失败\n");
	}
	return CSUDI_TRUE;	
}

//@DESCRIPTION: 获取文件属性的指针，检查文件指针的位置，确保文件指针正确
//@DESCRIPTION:  正常打开文件属性信息
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: hFile 为合法的文件句柄，由CSUDIOpen()返回
//@INPUT: psFileStat = &sFileStat，其中CSUDIFSFileStat_S sFileStat
//@EXPECTATION: 文件指针位置正确，CSUDIFSCSUDIFSTell()获取的偏移量与定义的偏移量一致
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到一个已挂载的分卷设备id	
//@EXECUTIONFLOW: 2、调用CSUDIFSGetDeviceInfo()获得分卷的挂载路径	
//@EXECUTIONFLOW: 3、调用CSUDIFSOpen(分区挂载路径+"CSTC_FS_IT_FilePointCheck", "w+")创建并打开文件，得到句柄hFile
//@EXECUTIONFLOW: 4、调用CSUDIFSWrite(hFile, "CSTC_FS_IT_FilePointCheck", strlen("CSTC_FS_IT_FilePointCheck"))写入数据
//@EXECUTIONFLOW: 5、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 6、调用CSUDIFSOpen(分区挂载路径+"CSTC_FS_IT_FilePointCheck", "w+")打开文件，得到句柄hFile
//@EXECUTIONFLOW: 7、调用CSUDIFSFstat(hFile, &sFileStat)获取文件属性信息
//@EXECUTIONFLOW: 8、调用CSUDIFSCSUDIFSTell()，获取文件当前位置判断是否在文件头
//@EXECUTIONFLOW: 9、调用CSUDIFSSeek()，设置从文件开头处CSUDIFS_SEEK_SET移动偏移loffset，期望返回成功
//@EXECUTIONFLOW: 10、调用CSUDIFSCSUDIFSTell()，获取文件当前位置
//@EXECUTIONFLOW: 11、判断当前位置和偏移loffset是否相等
//@EXECUTIONFLOW: 12、调用CSUDIFSClose(hFile)关闭文件
//@EXECUTIONFLOW: 13、调用CSUDIFSRemove(分区挂载路径+"CSTC_FS_IT_FilePointCheck")删除文件
CSUDI_BOOL CSTC_FS_IT_FilePointCheck_0001(void)
{
	char   szTest[] = "CSTC_FS_IT_FilePointCheck";   
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	CSUDI_HANDLE   hFile = CSUDI_NULL;
	int    nLen = strlen(szTest);
	long    nlofflen = 0;
	CSUDIFSFileStat_S  sFileStat;
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败\n");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);	
     
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "创建并打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "写入文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");	
	hFile = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "打开文件失败\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSFstat(hFile, &sFileStat), "获取文件信息失败\n");
	CSTK_ASSERT_TRUE_FATAL(0==CSUDIFSTell( hFile),"文件指针位置错误\n");

	nlofflen = sFileStat.m_lSize/2;
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile, nlofflen, CSUDIFS_SEEK_SET),"文件指针seek 操作失败\n");
	CSTK_ASSERT_TRUE_FATAL(nlofflen==CSUDIFSTell( hFile),"文件指针位置错误\n");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFile), "关闭文件失败\n");	
	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePath), "移除文件失败\n");
		}
	}
	return CSUDI_TRUE;		
}

//@CASEGROUP: 本地文件系统
//@DESCRIPTION: 测试本地文件系统只读可写挂载点
//@PRECONDITION: FS和FLASH模块之前已成功初始化
//@PRECONDITION: 系统中存在分卷设备
//@INPUT: 本地文件系统可写分区路径
//@EXPECTATION:每一步都返回预期结果
//@EXECUTIONFLOW:1、调用CSUDIFSGetAllDeviceId()得到所有已挂载的分卷设备id	
//@EXECUTIONFLOW:2、调用宏判断设备是否为可移动设备
//@EXECUTIONFLOW:3、如果不是可移动设备调用CSUDIFSGetDeviceInfo获得分卷的挂载路径
//@EXECUTIONFLOW:4、调用CSUDIFSStatfs获取分卷信息并判断文件系统类型
//@EXECUTIONFLOW:5、如果文件系统是只读类型则调用CSUDIFSOpen(szFilePath, "w+"))创建文件,要求返回失败
//@EXECUTIONFLOW:6、如果文件系统是只读类型则调用strcmp与配置文件比较分卷大小和文件系统类型
//@EXECUTIONFLOW:7、如果文件系统是可读写类型则调用strcmp与配置文件比较分卷大小和文件系统类型
//@EXECUTIONFLOW:8、对所有分卷设备重复步骤2~7
//@EXECUTIONFLOW:9、要求本地文件系统存在只读和可写分卷
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
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt), "获取设备id失败");
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt >= 2, "获取设备数目至少大于2");

	for (i = 0; i < nActIdCnt; i++)
	{
		if (CSUDI_ISREMOVEABLE(dwIds[i]))
		{
			continue;
		}

		memset(&sDeviceInfo, 0x00, sizeof(CSUDIFSDeviceInfo_S));
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(dwIds[i], &sDeviceInfo), "获取分卷信息失败");
		if (sDeviceInfo.m_eDeviceType != EM_UDIFS_DEVTYPE_VOLUME)
		{
			continue;
		}

		CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "分卷设备挂载点错误");
        memset(&sFsStat,0,sizeof(sFsStat));
		CSUDIFSStatfs(sDeviceInfo.m_szMountPoint, &sFsStat);
		CSTCPrint("m_lBsize:%d, m_lBlocks:%d\n", sFsStat.m_lBsize, sFsStat.m_lBlocks);

		if ((sFsStat.m_eFsType > EM_UDIFS_PARTITION_READONLY_START) && (sFsStat.m_eFsType < EM_UDIFS_PARTITION_READONLY_END))
		{
			CSTK_ASSERT_TRUE_FATAL(!strcmp(sDeviceInfo.m_szMountPoint, g_aucLocalFsReadOnlyPath), "本地只读分卷挂载点与配置文件不同");
			CSTK_ASSERT_TRUE_FATAL(sFsStat.m_lBsize*sFsStat.m_lBlocks/1024 == g_nLocalFsReadOnlySizeKByte, "本地只读分卷大小与配置文件不同");
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
			CSTK_ASSERT_TRUE_FATAL(nLen < CSUDI_MAX_FILE_NAME_LEN, "路径过长或有误");
			hFile = CSUDIFSOpen(szFilePath, "w+");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == hFile, "在只读分卷上创建并打开文件不应该成功");
			nRetMkdir = CSUDIFSMkdir(szFilePath, 0);
			CSTK_ASSERT_TRUE_FATAL(-1 == nRetMkdir, "在只读分卷上创建文件夹不应该成功");
		}
		if ((sFsStat.m_eFsType > EM_UDIFS_PARTITION_RW_START) && (sFsStat.m_eFsType < EM_UDIFS_PARTITION_RW_END))
		{
			CSTK_ASSERT_TRUE_FATAL(!strcmp(sDeviceInfo.m_szMountPoint, g_aucLocalFsReadWritePath), "本地可写分卷挂载点与配置文件不同");
			CSTK_ASSERT_TRUE_FATAL((sFsStat.m_lBsize/1024*sFsStat.m_lBlocks) == g_nLocalFsReadWriteSizeKByte, "本地可写分卷大小与配置文件不同");
			bHaveReadWriteFs = CSUDI_TRUE;
		}
	}

	CSTK_ASSERT_TRUE_FATAL((bHaveReadOnlyFs && bHaveReadWriteFs) == CSUDI_TRUE, "至少存在一个只读和一个可写非移动分卷设备");

 	CSTK_FATAL_POINT
	if (hFile != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRemove(szFilePath), "移除文件失败");
	}
	if (nRetMkdir != -1)
	{
		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szFilePath), "移除文件夹失败");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP: 本地文件系统
//@DESCRIPTION: 测试本地文件系统文件夹操作
//@PRECONDITION:FS和FLASH模块之前已成功初始化
//@PRECONDITION: 系统中存在分卷设备
//@INPUT: 本地文件系统可写分区路径
//@EXPECTATION:每一步都返回预期结果
//@EXECUTIONFLOW:1、调用CSUDIFSGetAllDeviceId()得到所有已挂载的分卷设备id	
//@EXECUTIONFLOW:2、调用宏判断设备是否为可移动设备
//@EXECUTIONFLOW:3、如果不是可移动设备调用CSUDIFSGetDeviceInfo获得分卷的挂载路径
//@EXECUTIONFLOW:4、调用CSUDIFSStatfs获取分卷信息并判断文件系统类型
//@EXECUTIONFLOW:5、如果文件系统是可写类型则与配置文件比较挂载点是否相同
//@EXECUTIONFLOW:6、调用CSUDIFSMkdir创建文件夹，要求返回成功
//@EXECUTIONFLOW:7、调用CSUDIFSMkdir创建子文件夹，要求返回成功
//@EXECUTIONFLOW:8、调用CSUDIFSOpenDir打开文件夹，要求返回成功
//@EXECUTIONFLOW:9、调用CSUDIFSReadDir读文件夹，期望个数为3
//@EXECUTIONFLOW:10、调用CSUDIFSCloseDir关闭文件夹，要求返回成功
//@EXECUTIONFLOW:11、调用CSUDIFSRmdir移除子文件夹，要求返回成功
//@EXECUTIONFLOW:12、调用CSUDIFSRmdir移除文件夹，要求返回成功
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

	CSTK_ASSERT_TRUE_FATAL(FS_iLocalFsGetRWVolumeMountPoint(), "获取本地可写分卷挂载点失败");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	memset(szSubFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szSubFilePath, szFilePath);
	strcat(szSubFilePath, "/");
	strcat(szSubFilePath, szSubTest);

	CSTK_ASSERT_TRUE_FATAL(0 == (nDirRet = CSUDIFSMkdir(szFilePath, 0)), "创建文件夹失败");
	CSTK_ASSERT_TRUE_FATAL(0 == (nSubDirRet = CSUDIFSMkdir(szSubFilePath, 0)), "创建子文件夹失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hDir = CSUDIFSOpenDir(szFilePath)), "打开文件夹失败");
	while (1)
	{
		psDirent = CSUDIFSReadDir(hDir);
		if(psDirent == CSUDI_NULL )
		{
			break;
		}
		nNodeCount++;
	}

	CSTK_ASSERT_TRUE_FATAL(3 == nNodeCount, "遍历次数不对");

	CSTK_FATAL_POINT
	if(hDir != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSCloseDir(hDir), "关闭文件夹失败");
	}
	if(nSubDirRet == 0)
	{
		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szSubFilePath), "移除子文件夹失败");
	}	
	if(nDirRet == 0)
	{
		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szFilePath), "移除文件夹失败");
	}	

	return CSUDI_TRUE;
}

//@CASEGROUP: 本地文件系统
//@DESCRIPTION: 测试本地文件系统文件夹操作
//@PRECONDITION:FS和FLASH模块之前已成功初始化
//@PRECONDITION: 系统中存在分卷设备
//@INPUT: 本地文件系统可写分区路径
//@EXPECTATION:每一步都返回预期结果
//@EXECUTIONFLOW:1、调用CSUDIFSGetAllDeviceId()得到所有已挂载的分卷设备id	
//@EXECUTIONFLOW:2、调用宏判断设备是否为可移动设备
//@EXECUTIONFLOW:3、如果不是可移动设备调用CSUDIFSGetDeviceInfo获得分卷的挂载路径
//@EXECUTIONFLOW:4、调用CSUDIFSStatfs获取分卷信息并判断文件系统类型
//@EXECUTIONFLOW:5、如果文件系统是可写类型则与配置文件比较挂载点是否相同
//@EXECUTIONFLOW:6、调用CSUDIFSMkdir创建文件夹，要求返回成功
//@EXECUTIONFLOW:7、循环32次调用CSUDIFSMkdir创建子文件夹，要求返回成功
//@EXECUTIONFLOW:8、循环256次调用CSUDIFSOpen创建并打开文件，要求返回成功
//@EXECUTIONFLOW:9、调用CSUDIFSOpenDir打开文件夹，要求返回成功
//@EXECUTIONFLOW:10、调用CSUDIFSReadDir读文件夹，期望个数为130
//@EXECUTIONFLOW:11、调用CSUDIFSCloseDir关闭文件夹，要求返回成功
//@EXECUTIONFLOW:12、循环32次调用CSUDIFSRmdir移除子文件夹，要求返回成功
//@EXECUTIONFLOW:13、循环256次调用CSUDIFSClose关闭文件，要求返回成功
//@EXECUTIONFLOW:14、循环256次调用CSUDIFSRemove移除文件，要求返回成功
//@EXECUTIONFLOW:15、调用CSUDIFSRmdir移除文件夹，要求返回成功
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

	CSTK_ASSERT_TRUE_FATAL(FS_iLocalFsGetRWVolumeMountPoint(), "获取本地可写分卷挂载点失败");

	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	memset(szSubFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szSubFilePath, szFilePath);
	strcat(szSubFilePath, "/");
	strcat(szSubFilePath, szSubTest);
	nLen = strlen(szSubFilePath);

	CSTK_ASSERT_TRUE_FATAL(0 == (nDirRet = CSUDIFSMkdir(szFilePath, 0)), "创建文件夹失败");

	for (i=0; i<FS_TEST_MAX_DIR_CNT; i++)
	{
		sprintf(szSubFilePath+nLen, "_Dir_%03d", i+1);
		CSTK_ASSERT_TRUE_FATAL(0 == (anSubDirRet[i] = CSUDIFSMkdir(szSubFilePath, 0)), "创建子文件夹失败");
	}
	for (i=0; i<FS_TEST_MAX_FILE_CNT; i++)
	{
		sprintf(szSubFilePath+nLen, "_File_%03d", i+1);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (ahFile[i] = CSUDIFSOpen(szSubFilePath, "w+")), "创建并打开文件失败");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hDir = CSUDIFSOpenDir(szFilePath)), "打开文件夹失败");
	while (1)
	{
		psDirent = CSUDIFSReadDir(hDir);
		if(psDirent == CSUDI_NULL )
		{
			break;
		}
		nNodeCount++;
	}

	CSTK_ASSERT_TRUE_FATAL((2+FS_TEST_MAX_DIR_CNT+FS_TEST_MAX_FILE_CNT) == nNodeCount, "遍历次数不对");

	CSTK_FATAL_POINT
	if (hDir != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSCloseDir(hDir), "关闭文件夹失败");
	}
	for (i=0; i<FS_TEST_MAX_DIR_CNT; i++)
	{
		if (anSubDirRet[i] == 0)
		{
			sprintf(szSubFilePath+nLen, "_Dir_%03d", i+1);
			CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szSubFilePath), "移除子文件夹失败");
		}
	}
	for (i=0; i<FS_TEST_MAX_FILE_CNT; i++)
	{
		if (ahFile[i] != CSUDI_NULL)
		{
			sprintf(szSubFilePath+nLen, "_File_%03d", i+1);
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(ahFile[i]), "关闭文件失败");
			CSTK_ASSERT_TRUE(0 == CSUDIFSRemove(szSubFilePath), "移除文件失败");
		}
	}
	if (nDirRet == 0)
	{
		CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRmdir(szFilePath), "移除文件夹失败");
	}	

	return CSUDI_TRUE;
}

//@CASEGROUP: 本地文件系统
//@DESCRIPTION: 测试对本地文件系统文件操作
//@PRECONDITION:FS和FLASH模块之前已成功初始化
//@PRECONDITION: 系统中存在分卷设备
//@INPUT: 本地文件系统可写分区路径
//@EXPECTATION:每一步都返回预期结果
//@EXECUTIONFLOW:1、调用CSUDIFSGetAllDeviceId()得到所有已挂载的分卷设备id	
//@EXECUTIONFLOW:2、调用宏判断设备是否为可移动设备
//@EXECUTIONFLOW:3、如果不是可移动设备调用CSUDIFSGetDeviceInfo获得分卷的挂载路径
//@EXECUTIONFLOW:4、调用CSUDIFSStatfs获取分卷信息并判断文件系统类型
//@EXECUTIONFLOW:5、如果文件系统是可写类型则与配置文件比较挂载点是否相同
//@EXECUTIONFLOW:6、调用CSUDIFSOpen创建并打开文件，要求返回成功
//@EXECUTIONFLOW:7、调用CSUDIFSWrite写入数据，要求返回成功
//@EXECUTIONFLOW:8、调用CSUDIFSFlush同步到磁盘，要求返回成功
//@EXECUTIONFLOW:9、调用CSUDIFSTell获取文件当前位置，要求返回成功
//@EXECUTIONFLOW:10、调用CSUDIFSSeek重置文件，要求返回成功
//@EXECUTIONFLOW:11、调用CSUDIFSTell获取文件当前位置，要求返回成功
//@EXECUTIONFLOW:12、调用CSUDIFSFstat获取文件信息，要求返回成功
//@EXECUTIONFLOW:13、调用CSUDIFS_ISREG判断是否为文件
//@EXECUTIONFLOW:14、调用CSUDIFSRead读取数据，要求返回成功
//@EXECUTIONFLOW:15、调用CSUDIFSSeek判断读出数据与写入时是否一致，要求一致
//@EXECUTIONFLOW:16、调用CSUDIFSStat获取文件信息，要求返回成功
//@EXECUTIONFLOW:17、调用CSUDIFS_ISREG判断是否为文件
//@EXECUTIONFLOW:18、调用CSUDIFSClose关闭文件，要求返回成功
//@EXECUTIONFLOW:19、调用CSUDIFSRemove移除子文件，要求返回成功
CSUDI_BOOL CSTC_FS_IT_LocalFsWriteRead_0003(void)
{
	CSUDI_HANDLE hFile = CSUDI_NULL;
	char szTest[] = "LocalFsWriteRead_0003";
	char szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	int nLen = strlen(szTest);
	char szBuf[100];
	CSUDIFSFileStat_S sFileStat;
	CSUDI_BOOL bAlreadyClose = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(FS_iLocalFsGetRWVolumeMountPoint(), "获取本地可写分卷挂载点失败");
	memset(szFilePath, 0x00, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePath, g_szVolumeMountPoint);
	strcat(szFilePath, szTest);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFile = CSUDIFSOpen(szFilePath, "w+")), "创建并打开文件失败");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFile, szTest, nLen), "写入数据失败");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSFlush(hFile), "同步到磁盘失败");

	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSTell(hFile), "获取文件当前位置失败");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSSeek(hFile, 0, CSUDIFS_SEEK_SET), "重置文件失败");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSTell(hFile), "获取文件当前位置失败");

	memset(&sFileStat, 0x00, sizeof(CSUDIFSFileStat_S));
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSFstat(hFile, &sFileStat), "获取文件信息失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDIFS_ISREG(sFileStat.m_lMode), "获取到的应该是文件");
	CSTK_ASSERT_TRUE_FATAL((int)sFileStat.m_lSize == nLen, "获取文件信息中大小不对");

	memset(szBuf, 0, sizeof(szBuf));
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSRead(hFile, szBuf, sizeof(szBuf) -1), "读取数据失败");
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(szTest, szBuf), "读出数据与写入时不一致");

	CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile), "关闭文件失败");
	hFile = CSUDI_NULL;
	bAlreadyClose = CSUDI_TRUE;

	memset(&sFileStat, 0x00, sizeof(CSUDIFSFileStat_S));
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStat(szFilePath, &sFileStat), "获取文件信息失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDIFS_ISREG(sFileStat.m_lMode), "获取到的应该是文件");
	CSTK_ASSERT_TRUE_FATAL((int)sFileStat.m_lSize == nLen, "获取文件信息中大小不对");

	CSTK_FATAL_POINT
	{
		if(hFile != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFile), "关闭文件失败");
		}

		if(bAlreadyClose || (hFile != CSUDI_NULL))
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSRemove(szFilePath), "移除文件失败");
		}
	}

	return CSUDI_TRUE;			
}

//@CASEGROUP: 本地文件系统
//@DESCRIPTION: 测试对本地文件系统文件操作
//@PRECONDITION:FS和FLASH模块之前已成功初始化
//@PRECONDITION: 系统中存在分卷设备
//@INPUT: 本地文件系统可写分区路径
//@EXPECTATION:每一步都返回预期结果
//@EXECUTIONFLOW:1、调用CSUDIFSGetAllDeviceId()得到所有已挂载的分卷设备id	
//@EXECUTIONFLOW:2、调用宏判断设备是否为可移动设备
//@EXECUTIONFLOW:3、如果不是可移动设备调用CSUDIFSGetDeviceInfo获得分卷的挂载路径
//@EXECUTIONFLOW:4、调用CSUDIFSStatfs获取分卷信息并判断文件系统类型
//@EXECUTIONFLOW:5、如果文件系统是可写类型则与配置文件比较挂载点是否相同
//@EXECUTIONFLOW:6、调用CSUDIFSOpen创建并打开文件A，要求返回成功
//@EXECUTIONFLOW:7、调用CSUDIFSWrite写入数据，要求返回成功
//@EXECUTIONFLOW:8、调用CSUDIFSClose关闭文件A，要求返回成功
//@EXECUTIONFLOW:9、调用CSUDIFSTruncate截断文件，要求返回成功
//@EXECUTIONFLOW:10、调用CSUDIFSStat获取文件信息，要求返回成功
//@EXECUTIONFLOW:11、调用strcmp判断截断后文件大小是否正确
//@EXECUTIONFLOW:12、调用CSUDIFSRename重命名文件A为文件B，要求返回成功
//@EXECUTIONFLOW:13、调用CSUDIFSOpen打开文件A，要求返回失败
//@EXECUTIONFLOW:14、调用CSUDIFSOpen打开文件B，要求返回成功
//@EXECUTIONFLOW:15、调用CSUDIFSRead读取数据，要求返回成功
//@EXECUTIONFLOW:16、调用CSUDIFSStat获取文件信息，要求返回成功
//@EXECUTIONFLOW:17、调用strcmp判断重命名后文件大小是否正确
//@EXECUTIONFLOW:18、调用CSUDIFSClose关闭文件，要求返回成功
//@EXECUTIONFLOW:19、调用CSUDIFSRemove移除子文件，要求返回成功
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

	CSTK_ASSERT_TRUE_FATAL(FS_iLocalFsGetRWVolumeMountPoint(), "获取本地可写分卷挂载点失败");
	memset(szFilePathA, 0, CSUDI_MAX_FILE_NAME_LEN);
	memset(szFilePathB, 0, CSUDI_MAX_FILE_NAME_LEN);
	strcpy(szFilePathA, g_szVolumeMountPoint);
	strcpy(szFilePathB, g_szVolumeMountPoint);
	strcat(szFilePathA, szTestA);
	strcat(szFilePathB, szTestB);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFileA = CSUDIFSOpen(szFilePathA, "w")), "创建并打开文件失败");
	CSTK_ASSERT_TRUE_FATAL(nLen == CSUDIFSWrite(hFileA, szTestA, nLen), "写入文件失败");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSClose(hFileA), "关闭文件失败\n");
	hFileA = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSTruncate(szFilePathA, ulLength1), "截断文件失败");	
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStat(szFilePathA, &sFileStat), "获取文件信息失败");
	CSTK_ASSERT_TRUE_FATAL((int)sFileStat.m_lSize == ulLength1, "截断后文件大小不对");

	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSRename(szFilePathA, szFilePathB), "重命名文件失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == CSUDIFSOpen(szFilePathA, "r"), "打开文件不应该成功");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != (hFileB = CSUDIFSOpen(szFilePathB, "r")) , "打开文件失败");
	CSTK_ASSERT_TRUE_FATAL(0 == CSUDIFSStat(szFilePathB, &sFileStat), "获取文件信息失败");
	CSTK_ASSERT_TRUE_FATAL((int)sFileStat.m_lSize == ulLength1, "重命名后文件大小不对");

	CSTK_FATAL_POINT
	{
		if(hFileA != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFileA), "关闭文件失败");
			CSTK_ASSERT_TRUE(0 == CSUDIFSRemove(szFilePathA), "移除文件失败");
		}
		if(hFileB != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE(0 == CSUDIFSClose(hFileB), "关闭文件失败");
			CSTK_ASSERT_TRUE(0 ==CSUDIFSRemove(szFilePathB), "移除文件失败");
		}
	}

	return CSUDI_TRUE;			
}

static int g_nFSRDIAddCallback_007 = 0;

typedef struct
{
	int m_nMajorFound;    //存储设备收到的Found消息数量
	int m_nVolumeFound;   //分卷设备收到的Found消息数量
	int m_nMajorReady;    //存储设备收到的Ready消息数量
	int m_nVolumeReady;   //分卷设备收到的Ready消息数量
	int m_nMajorPlugout;  //存储设备收到的Plugout消息数量
	int m_nVolumePlugout; //分卷设备收到的Plugout消息数量
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
				CSFSPrint("未知类型");
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
				CSFSPrint("未知类型");
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
				CSFSPrint("未知类型");
				return;
			}
			break;
		}
	default:
		{
			CSFSPrint("非法消息，请严格按照测试流程测试");
			break;
		}
	}

	return;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSRDIAddCallback(CSUDIRDICallback_F fnCallback,const void * pvUserData)
//@DESCRIPTION: 支持多USB设备，正常注册回调，验证收到的存储、分卷设备的Found、Ready、Plugout消息
//@PRECONDITION: 无
//@INPUT: fnCallback = iFSRDIAddCallback_0007_Callback
//@INPUT: pvUserData =  &g_nFSRDIAddCallback_007, 其中int g_nFSRDIAddCallback_007 = 1;
//@EXPECTATION: 注册成功，当插入多个U盘时在回调函数中收到多个EM_UDIRDI_EVENT_READY消息，拔除U盘时收到多个EM_UDIRDI_EVENT_PLUGOUT消息
//@EXECUTIONFLOW: 1、调用CSUDIFSRDIAddCallback(iFSRDIAddCallback_0007_Callback, &g_nFSRDIAddCallback_007)注册回调，返回值应该等于CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、根据提示插入U盘，等5秒后按'Y'键确认，检查回调函数中主存储设备收到的消息数量
//@EXECUTIONFLOW: 3、检查回调函数中收到存储设备的Plugout消息数量
//@EXECUTIONFLOW: 4、检查回调函数中收到存储设备的Found消息数量
//@EXECUTIONFLOW: 5、检查回调函数中收到存储设备的Ready消息数量
//@EXECUTIONFLOW: 6、检查回调函数中收到分卷设备的Found消息数量
//@EXECUTIONFLOW: 7、检查回调函数中收到分卷设备的Ready消息数量
//@EXECUTIONFLOW: 8、根据提示移除U盘，按'Y'键确认
//@EXECUTIONFLOW: 9、检查回调函数中收到存储设备的Plugout消息数量
//@EXECUTIONFLOW: 10、检查回调函数中收到分卷设备的Plugout消息数量
//@EXECUTIONFLOW: 11、提示插回U盘，调用CSUDIFSRDIRemoveCallback(CSUDIFSRDIAddCallback_0001_Callback, &nUserData)移除回调，恢复环境
CSUDI_BOOL CSTC_FS_IT_CSUDIFSRDIAddCallback_0007(void)
{
	//手动测试
	CSUDI_BOOL bAddCallback = CSUDI_FALSE;
	CSUDI_BOOL bRemoved = CSUDI_FALSE;

	memset(&s_sCallbackCnt, 0x00, sizeof(FSCallbackCntInfo_S));
	
	CSTK_ASSERT_TRUE_FATAL(g_nUSBCount > 1, "该用例需要测试平台至少支持两个USB设备");
	CSTCPrint("请拔除所有U盘, 按Y键确认\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，用例失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIAddCallback(iFSRDIAddCallback_0007_Callback,
		&g_nFSRDIAddCallback_007),"添加回调失败");
	bAddCallback = CSUDI_TRUE;
	
	CSTCPrint("请插入%d个U盘, 每个U盘至少有一个分区, 插入完成后请等待5秒, 再按Y键确认\n", g_nUSBCount);
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，用例失败");

	CSTK_ASSERT_TRUE_FATAL(s_sCallbackCnt.m_nMajorPlugout == 0, "请严格按照测试步骤来测试");
	CSTK_ASSERT_TRUE_FATAL(s_sCallbackCnt.m_nMajorFound == g_nUSBCount, "收到的存储设备Found消息数量不正确");
	CSTK_ASSERT_TRUE_FATAL(s_sCallbackCnt.m_nMajorReady == g_nUSBCount , "收到的存储设备Ready消息数量不正确");
	CSTK_ASSERT_TRUE_FATAL(s_sCallbackCnt.m_nVolumeFound >= g_nUSBCount, "收到的分卷消息数量不正确");
	CSTK_ASSERT_TRUE_FATAL(s_sCallbackCnt.m_nVolumeFound == s_sCallbackCnt.m_nVolumeReady, "收到的分卷消息数量不正确");
	
	CSTCPrint("请移除全部U盘, 按Y键确认\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，用例失败");
	bRemoved = CSUDI_TRUE;
		
	CSTK_ASSERT_TRUE_FATAL(s_sCallbackCnt.m_nMajorPlugout == g_nUSBCount, "收到的存储设备Plugout消息数量不正确");
	CSTK_ASSERT_TRUE_FATAL(s_sCallbackCnt.m_nVolumePlugout >= g_nUSBCount, "收到的分卷Plugout消息数量不正确");
	CSTK_ASSERT_TRUE_FATAL(s_sCallbackCnt.m_nVolumePlugout == s_sCallbackCnt.m_nVolumeReady, "收到的分卷设备Plugout消息数量不正确");

	CSTK_FATAL_POINT
	{
		if (bAddCallback == CSUDI_TRUE)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSRDIRemoveCallback(iFSRDIAddCallback_0007_Callback, 
				&g_nFSRDIAddCallback_007), "移除回调失败\n");
		}
		if (bRemoved == CSUDI_TRUE)
		{
			CSFSPrint("U盘已经移除，请拔掉后重新插入，待挂载成功后，按Y确认!\n");
			CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，恢复环境失败，会影响后续测试用例执行");  
		}
	}
	
	return CSUDI_TRUE;
}



//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetDeviceInfo(CSUDI_UINT32 dwDeviceId,CSUDIFSDeviceInfo_S * psDeviceInfo)
//@DESCRIPTION: 正常获得多个USB设备的信息
//@PRECONDITION: 系统中至少存在两个已识别的USB设备
//@INPUT: dwDeviceId = adwIds[n]，adwIds是由CSUDIFSGetAllDeviceId()获得的设备号数组
//@INPUT: psDeviceInfo = &sDeviceInfo, 其中CSUDIFSDeviceInfo_S  sDeviceInfo
//@EXPECTATION: 返回值等于 CSUDI_SUCCESS，且多USB设备的挂载点不同
//@EXECUTIONFLOW: 1、调用CSUDIFSGetAllDeviceId()得到全部设备的id数组adwIds[]
//@EXECUTIONFLOW: 2、遍历所有设备，调用CSUDIFSGetDeviceInfo(adwIds[n], &sDeviceInfo)，判断返回值，将主设备以及分卷设备区分出来,并将分卷设备的挂载点保存到数组当中
//@EXECUTIONFLOW: 3、检查获取的主、分卷设备数量是否符合预期
//@EXECUTIONFLOW: 4、检查分卷设备的挂载路径是否有相同的
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetDeviceInfo_0012(void)
{	
	//手动测试
	CSUDI_UINT32   adwIds[32] = {0};
	int nActIdCnt = 0;
	CSUDIFSDeviceInfo_S  sDeviceInfo;
	int i = 0;
	int k = 0;
	int nVolumeDev = 0;//分卷数量
	int nMajorDev = 0;//存储设备数量
	CSUDI_BOOL bResult = CSUDI_FALSE;	
	char aszVolumeDev[FS_TEST_MAX_USBDEVICE_CNT][256] = {"\0"};//分卷的挂载点
	
	memset(aszVolumeDev, 0x00, sizeof(aszVolumeDev));
	memset(&sDeviceInfo, 0x00, sizeof(CSUDIFSDeviceInfo_S));
	
	CSTK_ASSERT_TRUE_FATAL(g_nUSBCount > 1, "该用例需要测试平台至少支持两个USB设备");
	
	CSTCPrint("请确保已经插入了%d个U盘, 每个U盘至少有一个分区,插入完成后请等待5秒，请按Y键确认\n", g_nUSBCount);
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，用例失败"); 
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(adwIds, 32, &nActIdCnt), "获取设备id失败");
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "未获取到任何设备");
	
	for (i = 0; i < nActIdCnt; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(adwIds[i], &sDeviceInfo), "获取设备信息失败");		
		if ((adwIds[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_STORAGE_MAJOR_DEVICEID)
		{
			CSTK_ASSERT_TRUE_FATAL(0 == strlen(sDeviceInfo.m_szMountPoint), "存储设备不应该有挂载点" );
			nMajorDev++;
			
		}
		else if ((adwIds[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
		{
			CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "分卷设备应该有挂载点");
			strncpy(aszVolumeDev[nVolumeDev], sDeviceInfo.m_szMountPoint, sizeof(sDeviceInfo.m_szMountPoint));
			nVolumeDev++;
		}
		else
		{
			if (CSUDI_ISREMOVEABLE(adwIds[i]))
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "获得的设备号不是已知的设备类型" );
			}
			else
			{
			    CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "本地分卷设备应该有挂载点");
			}
		}
	}
	
	CSTK_ASSERT_TRUE_FATAL(nMajorDev == g_nUSBCount, "获取存储设备数量不正确" );
	CSTK_ASSERT_TRUE_FATAL(nVolumeDev >= g_nUSBCount, "获取分卷数量不正确" );

	for (i = 0; i < nVolumeDev; i++)
	{
		for (k = i + 1 ; k < nVolumeDev; k++)
		{
			bResult = (strcmp(aszVolumeDev[i], aszVolumeDev[k]) != 0 );
			CSTK_ASSERT_TRUE_FATAL(bResult , "分卷挂载路径不能相同" );
		}
	}
	 
	CSTK_FATAL_POINT
	{
		;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDI_Error_Code CSUDIFSGetDeviceInfo(CSUDI_UINT32 dwDeviceId,CSUDIFSDeviceInfo_S * psDeviceInfo)
//@DESCRIPTION: 获得正常多USB设备的信息
//@PRECONDITION: 准备多种不同品牌的U盘
//@INPUT: dwDeviceId = adwIds[n]，adwIds是由CSUDIFSGetAllDeviceId()获得的设备号数组
//@INPUT: psDeviceInfo = &sDeviceInfo, 其中CSUDIFSDeviceInfo_S  sDeviceInfo
//@EXPECTATION: 返回值等于 CSUDI_SUCCESS，且主设备无挂载点，分卷设备有挂载点
//@EXECUTIONFLOW: 1、提示用户插入指定品牌的U盘
//@EXECUTIONFLOW: 2、调用CSUDIFSGetAllDeviceId()得到全部设备的id数组adwIds[]
//@EXECUTIONFLOW: 3、遍历所有设备，调用CSUDIFSGetDeviceInfo(adwIds[n], &sDeviceInfo)，判断返回值，并验证主设备、分卷设备是否存在挂载点
//@EXECUTIONFLOW: 4、检查获取的主、分卷设备数量是否符合预期
//@EXECUTIONFLOW: 5、循环验证不同品牌的U盘
CSUDI_BOOL CSTC_FS_IT_CSUDIFSGetDeviceInfo_0013(void)
{	
	//手动测试
	CSUDI_UINT32   adwIds[32] = {0};
	int nActIdCnt = 0;
	CSUDIFSDeviceInfo_S  sDeviceInfo;
	int i = 0;
	int k = 0;	
	int bJudgeMajor = CSUDI_FALSE;
	int bJudgeVolume = CSUDI_FALSE;
	//u盘品牌数组，数组的元耸必个数需要和FS_TEST_USB_BRAND_CNT宏相匹配
	const char awsCSUDIFSUsbBrand[FS_TEST_USB_BRAND_CNT][256] = {"KINGSTON金士顿",  "SSK飚王", "SANDISK闪迪", "NETAC朗科", "ADATA威刚",
		                                                     "PNY必恩威", "TOSHIBA东芝", "VERBATIM威宝", "MAXELL麦克赛尔", "SONY索尼", 
		                                                     "AIGO爱国者", "APACER宇瞻"};

	memset(&sDeviceInfo, 0x00, sizeof(CSUDIFSDeviceInfo_S));
	
	for (k = 0; k < FS_TEST_USB_BRAND_CNT; k++)
	{		
		CSTCPrint("请重新插入[%s]的u盘,该U盘至少要有一个分区, 请按Y键确认\n", awsCSUDIFSUsbBrand[k]);
		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "输入了N，用例失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(adwIds, 32, &nActIdCnt), "获取设备id失败");
		CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "未获取到任何设备");

		for (i = 0; i < nActIdCnt; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFSGetDeviceInfo(adwIds[i], &sDeviceInfo), "获取设备信息失败");
			   
			
			if ((adwIds[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_STORAGE_MAJOR_DEVICEID)
			{
				CSTK_ASSERT_TRUE_FATAL(0 == strlen(sDeviceInfo.m_szMountPoint), "主设备不应该有挂载点" );
				bJudgeMajor = CSUDI_TRUE;
			}
			else if ((adwIds[i] & CSUDI_RDI_MAJOR_DEVICEID_MASK) == CSUDI_RDI_VOLUME_MAJOR_DEVICEID)
			{
				CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "分卷设备应该有挂载点");
				bJudgeVolume = CSUDI_TRUE;
			}
			else
			{
				if (CSUDI_ISREMOVEABLE(adwIds[i]))
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "获得的设备号不是已知的设备类型" );
				}
				else
				{
				    CSTK_ASSERT_TRUE_FATAL(0 != strlen(sDeviceInfo.m_szMountPoint), "本地分卷设备应该有挂载点");
				}
			}
		}

		CSTK_ASSERT_TRUE_FATAL((bJudgeMajor && bJudgeVolume) == CSUDI_TRUE, "未正确获取到移动设备");
	
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
//@DESCRIPTION:  正常创建遍历删除文件和文件夹
//@PRECONDITION: 系统中存在一个已挂载的分卷设备
//@INPUT: 
//@EXPECTATION: 各步骤能够创建遍历删除文件和文件夹成功
//@EXECUTIONFLOW: 1、获取分区挂载点路径，期望成功
//@EXECUTIONFLOW: 2、创建测试目录CSUDIFSMkdir(挂载点路径+"CSTC_FS_IT_0001", 0)，期望成功
//@EXECUTIONFLOW: 3、创建文件夹CSUDIFSMkdir(挂载点路径+"CSTC_FS_IT_0001"+"TestDir", 0)，期望成功
//@EXECUTIONFLOW: 4、创建文件CSUDIFSOpen(挂载点路径+"CSTC_FS_IT_0001"+"TestFile")，期望成功
//@EXECUTIONFLOW: 5、创建文件夹CSUDIFSMkdir(挂载点路径+"CSTC_FS_IT_0001"+"TestDir"+"SubTestDir", 0)，期望成功
//@EXECUTIONFLOW: 6、创建文件CSUDIFSOpen(挂载点路径+"CSTC_FS_IT_0001"+"TestDir"+"SubTestFile")，期望成功
//@EXECUTIONFLOW: 7、调用CSUDIFSOpenDir, CSUDIFSReadDir, CSUDIFSCloseDir等接口循环遍历挂载点路径+"CSTC_FS_IT_0001"目录，期望成功
//@EXECUTIONFLOW: 8、判断遍历到的文件数量是否为2(TestFile和SubTestFile)
//@EXECUTIONFLOW: 9、判断遍历到的文件夹数量是否为2(TestDir和SubTestDir)
//@EXECUTIONFLOW: 10、关闭文件(挂载点路径+"CSTC_FS_IT_0001"+"TestFile")，期望成功
//@EXECUTIONFLOW: 11、删除文件(挂载点路径+"CSTC_FS_IT_0001"+"TestFile")，期望成功
//@EXECUTIONFLOW: 12、关闭文件(挂载点路径+"CSTC_FS_IT_0001"+"SubTestFile")，期望成功
//@EXECUTIONFLOW: 13、删除文件(挂载点路径+"CSTC_FS_IT_0001"+"SubTestFile")，期望成功
//@EXECUTIONFLOW: 14、删除文件夹CSUDIFSRmdir(挂载点路径+"CSTC_FS_IT_0001"+"TestDir"+"SubTestDir")，期望成功
//@EXECUTIONFLOW: 15、删除文件夹CSUDIFSRmdir(挂载点路径+"CSTC_FS_IT_0001"+"TestDir"+"TestDir")，期望成功
//@EXECUTIONFLOW: 16、删除文件夹CSUDIFSRmdir(挂载点路径+"CSTC_FS_IT_0001"+"TestDir")，期望成功
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
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIFSGetVolumeMountPoint(), "获取分区挂载点失败");

	sprintf(acDirPath, "%s/%s", g_szVolumeMountPoint, __FUNCTION__);
	nMkDir = CSUDIFSMkdir(acDirPath, 0);
	CSTK_ASSERT_TRUE_FATAL(nMkDir == 0, "步骤2创建测试目录失败");

	sprintf(acTestDirPath, "%s/%s", acDirPath, pcTestDir);
	nMkTestDir = CSUDIFSMkdir(acTestDirPath, 0);
	CSTK_ASSERT_TRUE_FATAL(nMkTestDir == 0, "步骤3创建文件夹失败");

	sprintf(acTestFilePath, "%s/%s", acDirPath, pcTestFile);
	hTestFile = CSUDIFSOpen(acTestFilePath, "w");
	CSTK_ASSERT_TRUE_FATAL(hTestFile != CSUDI_NULL, "步骤4创建文件失败");

	sprintf(acSubTestDirPath, "%s/%s", acTestDirPath, pcSubTestDir);
	nMkSubTestDir = CSUDIFSMkdir(acSubTestDirPath, 0);
	CSTK_ASSERT_TRUE_FATAL(nMkSubTestDir == 0, "步骤5创建文件夹失败");

	sprintf(acSubTestFilePath, "%s/%s", acTestDirPath, pcSubTestFile);
	hSubTestFile = CSUDIFSOpen(acSubTestFilePath, "w");
	CSTK_ASSERT_TRUE_FATAL(hSubTestFile != CSUDI_NULL, "步骤6创建文件失败");
	
	CSTK_ASSERT_TRUE_FATAL(FSBrowseDir(acDirPath, &nFileCount, &nDirCount), "遍历分区挂载点目录失败");

	CSTCPrint("nFileCount = %d, nDirCount = %d\n", nFileCount, nDirCount);

	CSTK_ASSERT_TRUE_FATAL(nFileCount == 2, "遍历获取到的文件数量不正确");

	CSTK_ASSERT_TRUE_FATAL(nDirCount == 2, "遍历获取到的文件夹数量不正确");
	
	CSTK_FATAL_POINT;

	if (hTestFile != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIFSClose(hTestFile) == 0, "步骤10关闭文件失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDIFSRemove(acTestFilePath) == 0, "步骤11删除文件失败");
	}

	if (hSubTestFile != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIFSClose(hSubTestFile) == 0, "步骤12关闭文件失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDIFSRemove(acSubTestFilePath) == 0, "步骤13删除文件失败");
	}

	if (nMkTestDir == 0)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIFSRmdir(acSubTestDirPath) == 0, "步骤14删除文件夹失败");
	}

	if (nMkSubTestDir == 0)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIFSRmdir(acTestDirPath) == 0, "步骤15删除文件夹失败");
	}

	if (nMkDir == 0)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIFSRmdir(acDirPath) == 0, "步骤16删除测试目录失败");
	}
	
	return CSUDI_TRUE;
}

