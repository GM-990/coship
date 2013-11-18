#include <string.h>
#include "../include/kernel/cs_fsapi.h"
#include "CSUSPDMCreateIterate.h"
#include "SectionInterface2C.h"
#include "CSUSPDMParseData.h"
#include "MemoryPoolInterface.h"
#include "CSUSPDMCreateContainer.h"
#include "SourceInterface.h"
#include "SearchUI.h"
#include "cs_sicache.h"
#include "udi2_tuner.h"
#include "udi2_os.h"


#define SECTION_MEMPOOL_SIZE	(1024 * 600)
#define DATAMANAGER_MEMPOOL_SIZE	(128*1024)

#define MAX_TP_COUNT	128
#define MAX_SERVICE_COUNT	400
#define MAX_ITERATOR_COUNT	10

#define  FLASH_BLOCK_SIZE		(128*1024)

static BYTE	g_szSectionMemPool[SECTION_MEMPOOL_SIZE];
static BYTE	g_szDMMemPool[DATAMANAGER_MEMPOOL_SIZE];

static HCSHANDLE g_hSource = 0;
static void InitSectionSource(void)
{

	TaskInitPar     tempTaskInitPar;
	SectionQueuePar tempQueuePar[3];

	tempQueuePar[0].m_nQueueSize = 500*1024;
	tempQueuePar[0].m_nPriorityIndex = SECTION_PRIORITY_HIGH;

	tempQueuePar[1].m_nQueueSize = 0;
	tempQueuePar[1].m_nPriorityIndex = SECTION_PRIORITY_MIDDLE;

	tempQueuePar[2].m_nQueueSize = 0;
	tempQueuePar[2].m_nPriorityIndex = SECTION_PRIORITY_LOW;

	tempTaskInitPar.m_nOneBufSize           = 0;
	tempTaskInitPar.m_nSectionQueueParCount = 3;
	tempTaskInitPar.m_nStackSize            = 64*1024; //Section task stack size
	tempTaskInitPar.m_nTaskPriority         = 4*17;    //Section task priority
	tempTaskInitPar.m_nTaskPriorityIndex    = 0;
	tempTaskInitPar.m_pSectionQueuePar      = tempQueuePar;

	Section2CCreateSectionSourceTimeOutTaskEx \
		(g_szSectionMemPool, SECTION_MEMPOOL_SIZE, &tempTaskInitPar,1,3*17, 8*1024);

}

static HCSHANDLE s_hDmHandle = 0;

HCSHANDLE USPHANDLE_SOURCE(void)
{
	return g_hSource;

}

HCSHANDLE SETUPDTVGetDmHandle(void)
{
    return s_hDmHandle;
}
#if 0
static BYTE 	g_szDMFlashSpace[FLASH_BLOCK_SIZE];
HCSHANDLE InitDataManager(void)
{
	DATACONTAINERPAR  meidaDataContainerPar;
	DATACONTAINERPAR  objectblockDataContainerPar[1];
	DATACONTAINERPAR  cableDataContainerPar[3];

	DWORD             dwContainerAddr  = 0;
	DWORD             dwContainerLen   = 0;;

	HCSHANDLE         hMedia = 0;			//Flash对象的handle
	HCSHANDLE         hServiceBlock = 0;		//一块逻辑可用的Flash的handle
	HCSHANDLE         hMemoryPool = 0;

	HCSHANDLE         hRetHandle = 0;

	int               nMemSize, nFlsSize;
	int               nOffset = 0;

	BOOL              bRet    = FALSE;

//#if defined(PLATFORM_X86)/**/
	memset(g_szDMFlashSpace, 0xff, FLASH_BLOCK_SIZE);
	dwContainerAddr = (DWORD) g_szDMFlashSpace;
//#else
//	dwContainerAddr  = FLASH_BASE_ADDRESS+FLASH_DM_DATA_OFF;//高清是这个配置吗?
//#endif
	dwContainerLen   = 64*1024;

	//===================================================================
	meidaDataContainerPar.pVoid	= (PVOID)dwContainerAddr;	//USP-DM使用的FLASH首地址
	meidaDataContainerPar.nLen	= dwContainerLen;			//USP-DM使用的FLASH长度

	//对FlashMedia进行具体划分
	objectblockDataContainerPar[0].nType = 1;				//USP-DM管理的Service-TP数据
	objectblockDataContainerPar[0].nLen = dwContainerLen;

	//用于USP-DM数据交换的内存池
	hMemoryPool = CSUSPCreateMemoryPool( g_szDMMemPool, sizeof(g_szDMMemPool) );
	CSASSERT( hMemoryPool != 0 );

	//创建FlashMedia 
	hMedia = CSUSPDMCreateFlashMediaEx( hMemoryPool,
		                                &meidaDataContainerPar,
		                                sizeof(meidaDataContainerPar)/sizeof(DATACONTAINERPAR),
	 	                                objectblockDataContainerPar,
		                                sizeof(objectblockDataContainerPar)/sizeof(DATACONTAINERPAR)
		                              );
	CSASSERT( hMedia != 0 );

	//===========================================================================
	//选取FlashMedia的第一个部分作为节目保存之?
	hServiceBlock = CSUSPDMGetFlashStorageBlock( 1, hMedia );
	CSASSERT( hServiceBlock != 0 );

	//划分节目数据?
	nOffset = 0;	
	cableDataContainerPar[0].nType = nOffset;	 //频点数据
	cableDataContainerPar[0].nLen = 8*1024;	 //所占用大小

	nOffset += cableDataContainerPar[0].nLen;
	
	cableDataContainerPar[1].nType = nOffset;	 //节目非频繁变化数?
	cableDataContainerPar[1].nLen = 30*1024;	 //所占用大小

	nOffset += cableDataContainerPar[1].nLen;
	
	cableDataContainerPar[2].nType = nOffset;	 //节目频繁变化数据
	cableDataContainerPar[2].nLen  = 24*1024;  //所占用大小

	CSASSERT((cableDataContainerPar[0].nLen + cableDataContainerPar[1].nLen + \
		cableDataContainerPar[2].nLen) < dwContainerLen);

	
	//===========================================================================
	//USP-DM自定义数据长度注?
	bRet = CSUSPDMSetUserDataLenEx( 40, //UserData全部数据长度
	                                0,                //掉电保存数据起始偏移
	                                2*sizeof(int),    //掉电保存覆盖部分长度
	                                4*sizeof(int)     //整个掉电保存部分长度
	                              );   
	CSASSERT(bRet);
	//===========================================================================

	//创建数据容器

	hRetHandle = CSUSPDMCreateContainer( hMemoryPool, USPDM_C_CONTAINER, cableDataContainerPar, 3, hServiceBlock );

	CSASSERT( hRetHandle != 0 );

	s_hDmHandle = hRetHandle;

	bRet = CSUSPDMSetIsAttendUniNum(hRetHandle, FALSE);  //音频类型采用type_8

	//===========================================================================
	//限定Service个数
	bRet = CSUSPDMSetMaxServiceNum( MAX_SERVICE_COUNT, hRetHandle );
	CSASSERT(bRet);

	bRet = CSUSPDMSetMaxTransponderSNum( MAX_TP_COUNT, hRetHandle );
	CSASSERT(bRet);

	bRet = CSUSPDMSetMaxIteratorNum( MAX_ITERATOR_COUNT, hRetHandle );
	CSASSERT(bRet);

	bRet = CSUSPDMGetMemFlsComfortableSize( &nMemSize, &nFlsSize, hRetHandle );
	CSASSERT(bRet);
	CSDebug("TestMain",DEBUG_LEVEL, "\nMaxSrv=%d MaxItr=%d\n", MAX_SERVICE_COUNT, MAX_ITERATOR_COUNT );
	CSDebug("TestMain",DEBUG_LEVEL,  "\nnMemSize=0x%08x nFlsSize=0x%08x\n", nMemSize, nFlsSize );

	//加载数据
	//bRet = CSUSPDMReLoadAllData( hRetHandle );
	//CSASSERT(bRet);

	return hRetHandle;
}
#else
HCSHANDLE InitDataManager(void)
{
	HCSHANDLE hMemPool;
	int nRet;
	DWORD dwContainerType = USPDM_C_CONTAINER|USPDM_S_CONTAINER|USPDM_T_CONTAINER;
	HCSHANDLE hContianer = 0;

	char caDir[] = "/ServiceData/";


	CSFSOperation_t sFSP = {
		NULL, //int (*Init)(int nRootDevice, int nRamFsKBytes);
	 	NULL,//int (*Mount)( const char *pcDeviceName,const char * pcDir,const char * pcFilesystemType,DWORD  dwMountFlags ,  const VOID * pData);
	 	NULL,//int (*Umount)(const char *pcDir);
	 	NULL,//int (*Mkdir)(const char * pcPathName, int nMode);
	 	CSFSOpen,//int (*Open)(const char * pcPathName, int nFlags,int nMode);
	 	CSFSRead,//int (*Read)(int nFileDescriptor,char * pcBuf, UINT uCount);
	 	CSFSWrite,//int (*Write)(int nFileDescriptor,char * pcBuf, UINT uCount);
	 	CSFSClose,//int (*Close)(int nFileDescriptor);
	 	CSFSLseek,//int (*Lseek)(int nFileDescriptor, LONG lOffset, UINT uOrigin);
	 	0,//int (*Llseek)(int nFileDescriptor,unsigned long  dwOffsetHigh,unsigned long  dwOffsetLow, long * plResult, UINT uOrigin);
	 	
	 };

	BOOL bRe = FALSE;
	nRet = CSFSInit(0x0100, 1024);
	nRet = CSFSMkdir(caDir, 0);
	nRet = CSFSMount("dm", caDir, "jffs", 0, "-size 1984");

	hMemPool = CSUSPCreateMemoryPool(g_szDMMemPool, DATAMANAGER_MEMPOOL_SIZE);

	//初始化Container
 	bRe = CSUSPDMSetCSFSFuns(&sFSP);
	CSASSERT(bRe);

	hContianer = CSUSPDMCreateFileSysContainer(hMemPool, dwContainerType, caDir);
	s_hDmHandle = hContianer;
	//CSUSPDMReLoadAllData(hContianer);

	return hContianer;
}
#endif

static BOOL g_bSearching = FALSE;
static int SearchCallback(CallBackType enmType, void* pParam)
{
	switch(enmType)
	{
		case SEARCH_FINISHED:        //全部搜索结束
		{
			g_bSearching = FALSE;
		}
		break;

	case SERVICE_ISNEEDSAVE:     //某个频点上的Service回调显示
		{
			if(pParam != NULL)
			{
				BOOL bRet = FALSE;
				SERVICEINFOFORCLIENT *pServiceInfoClient = (SERVICEINFOFORCLIENT*)pParam;
				HCSHANDLE hService = pServiceInfoClient->m_hServiceData;
				ServicePID	stPIDs;		
				CSUSPDMServiceType type = USPDM_SERVICE_TV;
				int nSerId = 0;
				CSUSPDMGetServiceType(&type, hService);

#if 0 //save all type service
				if (type != USPDM_SERVICE_TV && type != USPDM_SERVICE_RADIO)
				{
					CSTRACE(ERROR_LEVEL, "[Setup_dtv] wrong service type = %d\n", type);
					return 1;
				}
#endif

				memset( &stPIDs, 0, sizeof(stPIDs));					
				bRet = CSUSPDMGetServicePID( &stPIDs, hService );
				CSUSPDMGetService_id(&nSerId, hService);
				printf("the service id is %d\n", nSerId);
				CSASSERT(bRet);				

#if 0	//save all type service
				if (0 == stPIDs.m_dwAudioPID_13 && 0 == stPIDs.m_dwVideoPID_13)
				{
					CSTRACE(ERROR_LEVEL,"[Setup_dtv] filter for no av-Pid.\n");				
					return 1;
				}
#endif

			}
			return 0;
		}
	default :
		break;

	}

	return 0;
}

#define FREQUENCE_NUM 8
 BOOL DoSearch(void)
{
	HCSHANDLE hSearch = 0;
	HCSHANDLE hSearchUI = 0;
	HCSHANDLE *phContainer = NULL;
	HCSHANDLE hSource = 0;

	int  nRequest = 0;
	int nDeliverCount = 1;
	SearchExRequestInfo searchParam;

	Deliver sDeliver[FREQUENCE_NUM] ;
	BOOL bRet = FALSE;

	CSUDITunerInfo_S sTunerInfo;
	CSUDITunerGetDeviceInfo(0,&sTunerInfo);
	if(sTunerInfo.m_eType == EM_UDITUNER_TYPE_QAM)
	{
		nDeliverCount = 1;
		memset(sDeliver, 0, FREQUENCE_NUM*sizeof(Deliver));
		sDeliver[0].m_CableDeliver.m_dwFrequency = 659000;
		sDeliver[0].m_CableDeliver.m_dwModulation_8 = 3;
		sDeliver[0].m_CableDeliver.m_dwSymbolRate_24= 6875;

	/*	sDeliver[1].m_CableDeliver.m_dwFrequency = 547000;
		sDeliver[1].m_CableDeliver.m_dwModulation_8 = 3;
		sDeliver[1].m_CableDeliver.m_dwSymbolRate_24= 6875;

		sDeliver[2].m_CableDeliver.m_dwFrequency = 411000;
		sDeliver[2].m_CableDeliver.m_dwModulation_8 = 3;
		sDeliver[2].m_CableDeliver.m_dwSymbolRate_24= 6875;

		sDeliver[3].m_CableDeliver.m_dwFrequency = 435000; 
		sDeliver[3].m_CableDeliver.m_dwModulation_8 = 3;
		sDeliver[3].m_CableDeliver.m_dwSymbolRate_24= 6875;

		sDeliver[4].m_CableDeliver.m_dwFrequency = 387000; 
		sDeliver[4].m_CableDeliver.m_dwModulation_8 = 3;
		sDeliver[4].m_CableDeliver.m_dwSymbolRate_24= 6875;*/
		hSource = CreateHDICableSource(0);
		g_hSource = hSource;
		CSASSERT(hSource != 0);
	}
	else if(sTunerInfo.m_eType == EM_UDITUNER_TYPE_COFDM)
	{
		nDeliverCount = 1;
		memset(sDeliver, 0, FREQUENCE_NUM*sizeof(Deliver));
		sDeliver[0].m_TerrestrialDeliver.m_dwFrequency = 722000;
		sDeliver[0].m_TerrestrialDeliver.m_dwBanwith_3 = 0; //8M
		
		hSource = CreateHDITerrestriaSource(0);
		CSASSERT(hSource != 0);
	}
	CSUSPDMGetContainerHandle(&phContainer, NULL);
	CSASSERT(phContainer[0] != 0);

	hSearch = CreateSearch();
	CSASSERT(hSearch != 0);

	hSearchUI = CreatePATPMTSearchUI( hSearch, sDeliver, nDeliverCount, SearchCallback,  1000*10);
	CSASSERT(hSearchUI != 0);

	//nRequest |= SEARCH_SERVICE_SAVE_CUSTOM;
	nRequest |= SEARCH_BAT_GET_TAG;
	memset(&searchParam, 0, sizeof(searchParam));
 	searchParam.m_BATRequestInfo.m_nBouquetIDCount = 2;
	searchParam.m_BATRequestInfo.m_wBouquetID[0] = 0x4;
	searchParam.m_BATRequestInfo.m_wBouquetID[1] = 0x5;
	
	CSUSPSearchSetSupportMPlayer(TRUE);	
	bRet = StartSearchEx(hSearchUI, phContainer[0], hSource, 8192, nRequest,  &searchParam);

	if(!bRet )
	{
		return FALSE;
	}
	g_bSearching = TRUE;

	while(g_bSearching)
	{
		CSUDIOSThreadSleep(300);
	}

	bRet = CloseSearchUI(hSearchUI);
	CSASSERT(bRet);
	hSearchUI = 0;

	bRet = CloseSearch(hSearch);
	CSASSERT(bRet);
	hSearch = 0;

	return TRUE;

}

HCSHANDLE getDMContainer()
{
	static HCSHANDLE s_hContainer = (HCSHANDLE)NULL;
	if (s_hContainer ==  (HCSHANDLE)NULL)
	{
		BOOL bRet = FALSE;
		HCSHANDLE *phContainer = NULL;
		int nContainerCount = 0;

		bRet = CSUSPDMGetContainerHandle(&phContainer, &nContainerCount);
		CSASSERT(bRet);
		CSASSERT(nContainerCount > 0);
		CSASSERT(phContainer[0] !=  (HCSHANDLE)NULL);

		s_hContainer = phContainer[0];
	}

	return s_hContainer;
}

void PrintService(HCSHANDLE hService, HCSHANDLE hDataContainer)
{
	BOOL bRet =FALSE;
	int nServId = 0;
	BYTE aucServName[32] = {0};
	CSUSPDMCodingType enmServType = USP_CODING_UNKNOWEN;
	bRet = CSUSPDMGetService_id(&nServId, hService);
	CSASSERT(bRet);
	bRet = CSUSPDMGetServiceName((char*)aucServName, &enmServType, hService);
	CSASSERT(bRet);
	CSDebug("TestMain", DEBUG_LEVEL,  "	++++++Service++++++: service id=0x%x(%d), name=%s \r\n", nServId, nServId, aucServName);
}

void PrintTP(HCSHANDLE hTPHandle, HCSHANDLE hDataContainer)
{
	BOOL bRet = FALSE;
	int nTPType = CSUSPDMGetDataHandleType(hTPHandle);
	HCSHANDLE hServIter = CSUSPDMCreateIterateAllServiceBlgedTP(hDataContainer, hTPHandle);
	HCSHANDLE hService = CSUSPDMGetHeadDataHandle(hServIter);
	SourceID sSourceId = {0};
	bRet = CSUSPDMGetSourceID(&sSourceId, hTPHandle);
	CSASSERT(bRet);
	
	switch(nTPType)
	{
		case USPDM_CABLE_DELIVER:
		{
			CableDeliver sCabDeliver = {0};
			bRet = CSUSPDMGetCableDeliver( &sCabDeliver, hTPHandle);
			CSASSERT(bRet);
			CSDebug("TestMain", DEBUG_LEVEL,  "+++Cable TP+++ oriId=0x%x, tsId=0x%x, freq=%d, symb=%d, modulation=%d\r\n", 
				sSourceId.m_wOriginal_network_id, sSourceId.m_wTSID, 
				sCabDeliver.m_dwFrequency, sCabDeliver.m_dwSymbolRate_24, sCabDeliver.m_dwModulation_8);
		}
		break;
			
		case USPDM_TERRESTREA_DELIVER:	
		{
			TerrestrialDeliver sTerDeliver = {0};
			bRet = CSUSPDMGetTerrestrialDeliver(&sTerDeliver, hTPHandle);
			CSASSERT(bRet);			
			CSDebug("TestMain", DEBUG_LEVEL,  "+++Terrestreal TP+++ oriId=0x%x, tsId=0x%x, freq=%d, bandWidth=%d\r\n", 
				sSourceId.m_wOriginal_network_id, sSourceId.m_wTSID, 
				sTerDeliver.m_dwFrequency, sTerDeliver.m_dwBanwith_3);
		}
		break;
			
		case USPDM_SATELLITE_DELIVER:
		{
			SatelliteDeliver sSatDeliver = {0};
			bRet = CSUSPDMGetSatelliteDeliver(&sSatDeliver, hTPHandle);
			CSASSERT(bRet);			
			CSDebug("TestMain", DEBUG_LEVEL,  "+++Satellite TP+++ oriId=0x%x, tsId=0x%x, freq=%d, symb=%d, polarization=%d\r\n", 
				sSourceId.m_wOriginal_network_id, sSourceId.m_wTSID, 
				sSatDeliver.m_dwFrequency, sSatDeliver.m_dwSymbolRate_24, sSatDeliver.m_dwPolarization_2);
		}
			break;
			
		default:
			CSASSERT(0);
			break;
	}
	while(hService != (HCSHANDLE)NULL)
	{
		PrintService(hService, hDataContainer);
		hService = CSUSPDMGetNextDataHandle( hServIter);
	}
	
	CSUSPDMFreeIteratorHandle( hServIter, hDataContainer);
}

void PrintBouquet(HCSHANDLE hBouquet, HCSHANDLE hDataContainer)
{
	BOOL bRet = FALSE;
	HCSHANDLE hServiceIter;
	HCSHANDLE hService;
	
 	CSDTVDMBatInfo_S sBouquetInfo;
	memset(&sBouquetInfo, 0, sizeof(CSDTVDMBatInfo_S));		
		
	bRet = CSUSPDMGetBATINFO( hBouquet, &sBouquetInfo);
	CSASSERT(bRet);

	CSDebug("TestMain", DEBUG_LEVEL,  "[TEST]++++bouquet info id=0x%x(%d), name =%s \r\n", sBouquetInfo.uBouquetID, sBouquetInfo.uBouquetID, sBouquetInfo.aucBouquetName);

	hServiceIter = CSUSPDMCreateIterateAllService( hDataContainer,  0xff);
	bRet = CSUSPDMSetBouquetFilter( hDataContainer,  hServiceIter, hBouquet, TRUE);
	CSASSERT(bRet);
	
	hService = CSUSPDMGetHeadDataHandle( hServiceIter);
	while(hService != (HCSHANDLE)NULL)
	{
 		PrintService(hService, hDataContainer);
		hService = CSUSPDMGetNextDataHandle(hServiceIter);
	}	 
}

void PrintDM(HCSHANDLE hDataContainer)
{
	HCSHANDLE hTPIter = CSUSPDMCreateIterateAllTransponder(hDataContainer);
	HCSHANDLE hTP = CSUSPDMGetHeadDataHandle(hTPIter);
	
	CSDebug("TestMain", DEBUG_LEVEL,  " start to print service infomation in data container \r\n");	

	CSDebug("TestMain", DEBUG_LEVEL,  "htp=0x%x\n", hTP);	
	
	while(hTP != (HCSHANDLE)NULL)
	{
		PrintTP(hTP, hDataContainer);
		hTP = CSUSPDMGetNextDataHandle( hTPIter);	
	}

	CSUSPDMFreeIteratorHandle( hTPIter, hDataContainer);
	CSDebug("TestMain", DEBUG_LEVEL,  " end to print service infomation in data container \r\n\r\n");

	//print bouquet info
	CSDebug("TestMain", DEBUG_LEVEL,  " start to print bouquet infomation in data container \r\n");	

	{
		HCSHANDLE hBouquetIterator = CSUSPDMCreateIterateAllBouquet(hDataContainer);
		HCSHANDLE hBouquet = CSUSPDMGetHeadDataHandle(hBouquetIterator);	
	 	while(hBouquet != (HCSHANDLE)NULL)
	 	{
			PrintBouquet(hBouquet, hDataContainer);
			hBouquet = CSUSPDMGetNextDataHandle( hBouquetIterator);
	 	}
	}
	
	CSDebug("TestMain", DEBUG_LEVEL,  " end to print bouquet infomation in data container \r\n\r\n");
}

void doSearch_DTV(void)
{
	InitSectionSource();
	InitDataManager();
	
	DoSearch();
	PrintDM(getDMContainer());
	
	CSSICInit();
}




