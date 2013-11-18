#include "setup.h"
#include "irdecode.h"
#include "cs_sysdemux.h"
#include "cs_testkit.h"
#include "udi_init.h"

#include "cs_cainterface.h"
#include "cs_cainterface_pvrext.h"
/*For test initialize time */

#define MAX_MODULE_TO_TEST	12
typedef int (*FOnCSRTSPHaveMsgToUser) (int  nUserData,int  nMsg,DWORD dwPara1,DWORD dwPara2);

typedef	struct  _InitialTimeCost_t
{
	char   *pModuleName;
	DWORD dwTimeCost;
}InitialTimeCost_t;

DWORD g_dwTimeStart;
DWORD g_dwTimeStop;
DWORD g_dwTimeCost;

int ModuleIndex = 0;

InitialTimeCost_t g_asTimeCost[MAX_MODULE_TO_TEST];	

void TimePrint(void) ; //打印初始化时间函数

/*udi_init is used to init porting,and different hardware config has defferent version of udi_int*/
void udi_init()
{
	HCSHANDLE hChipHandle = 0;
	CSHDI_Error_t   ErrorCode;
	CSPanelPinCfg_t sPanelPinCfg;
	CSRMDemuxConfig_S sDemuxInfo;

	/* init  demux*/
	CSRMSetRMSupport(TRUE);
	
	sDemuxInfo.m_nSupportType		= DEMUX_SUPPORT_PLAYBACK;
	sDemuxInfo.m_emLiveChannel		= EM_DEMUX_CHANNELCOUNT_0;
	sDemuxInfo.m_emRecordChannel	= EM_DEMUX_CHANNELCOUNT_0;
	sDemuxInfo.m_emPlaybackChannel	= EM_DEMUX_CHANNELCOUNT_1;
	sDemuxInfo.m_nTunerLinkMark 	= 0x00;
	if(!CSRMSetDemuxConfig(CSDMX_0,&sDemuxInfo))
	{
		CSDebug("PlatformInit.c",  ERROR_LEVEL, "[%s]CSRMSetDemuxConfig demux(%d) fail !\n", __FUNCTION__, CSDMX_0);
		return;
	}

	sDemuxInfo.m_nSupportType		= DEMUX_SUPPORT_PLAYBACK;
	sDemuxInfo.m_emLiveChannel		= EM_DEMUX_CHANNELCOUNT_0;
	sDemuxInfo.m_emRecordChannel	= EM_DEMUX_CHANNELCOUNT_0;
	sDemuxInfo.m_emPlaybackChannel	= EM_DEMUX_CHANNELCOUNT_1;
	sDemuxInfo.m_nTunerLinkMark 	= 0x00;
	if(!CSRMSetDemuxConfig(CSDMX_1,&sDemuxInfo))
	{
		CSDebug("PlatformInit.c",  ERROR_LEVEL, "[%s]CSRMSetDemuxConfig demux(%d) fail !\n", __FUNCTION__, CSDMX_1);
		return;
	}

    if(!CSSYSSetCfg(CSCFG_TSIN_DMX, CSTSIN_5, CSDMX_0, 0))
	{
		CSDebug("PlatformInit.c",  ERROR_LEVEL, "[%s]CSSYSSetCfg tsin(%d)-demux(%d) fail !\n", __FUNCTION__, CSTSIN_5, CSDMX_0);
		return;
	}
	
	if(!CSSYSSetCfg(CSCFG_TSIN_DMX, CSTSIN_6, CSDMX_1, 0))
	{
		CSDebug("PlatformInit.c",  ERROR_LEVEL, "[%s]CSSYSSetCfg tsin(%d)-demux(%d) fail !\n", __FUNCTION__, CSTSIN_6, CSDMX_1);
		return;
	}


    g_dwTimeStart = CSGetTickCount();
	
    if (!CSSYSChipBoot((HCSHANDLE)NULL))
	{
		CSDebug("PlatformInit.c",  ERROR_LEVEL, "[%s]CSSYSChipBoot fail !\n", __FUNCTION__);
        return;
    }

	hChipHandle = CSSYSChipInit(NULL, (HCSHANDLE)NULL);
	if (hChipHandle == (HCSHANDLE)NULL)
	{
		CSDebug("PlatformInit.c",  ERROR_LEVEL, "[%s]CSSYSChipInit fail !\n", __FUNCTION__);
		CSSYSChipHalt((HCSHANDLE)NULL);
		return;
	}

    g_dwTimeStop = CSGetTickCount();
//    CSTTPrint("\n^^^^^^^^^^^^^^^^start:%d\r\n", g_dwTimeStart);
//    CSTTPrint("\n^^^^^^^^^^^^^^^^stop:%d\r\n", g_dwTimeStop);
    g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
    g_asTimeCost[ModuleIndex].pModuleName = "ChipBoot + ChipInit";
    g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;

         
    /* init panel */
    g_dwTimeStart = CSGetTickCount();
    
    sPanelPinCfg.m_pucSegPinGroup[0] = 20;
	sPanelPinCfg.m_pucSegPinGroup[1] = 19;
    sPanelPinCfg.m_pucComPinGroup[0] = 21;
	sPanelPinCfg.m_ucKeyScanPin   	= 24;
    ErrorCode = CSPanelInitialize(220, &sPanelPinCfg);

     g_dwTimeStop = CSGetTickCount();

     CSASSERT(ErrorCode == CSHDI_SUCCESS);
    if(ErrorCode == CSHDI_SUCCESS)
    {
        g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
        g_asTimeCost[ModuleIndex].pModuleName = "Panel";
        g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;
    }
    //CSPanelRegisterKeySendCallback((CSHDICallback)UI_PanelControlUserProc);


    // IR init

     g_dwTimeStart = CSGetTickCount();
    
    UI_IRInitialize(5*17);
    g_dwTimeStop = CSGetTickCount();

    g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
    g_asTimeCost[ModuleIndex].pModuleName = "IR";
    g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;

    /* init EEPROM & FLASH */

     g_dwTimeStart = CSGetTickCount();
    
    ErrorCode = CSFLSSetFLSCount(1);
    ErrorCode |= CSFLSSetSpace( 0, 0X11000000, 0x7000000);
    ErrorCode |= CSFLSInitialize(13);

     g_dwTimeStop = CSGetTickCount();
     
    CSASSERT(ErrorCode == CSHDI_SUCCESS);
     if(ErrorCode == CSHDI_SUCCESS)
    {
        g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
        g_asTimeCost[ModuleIndex].pModuleName = "FLASH";
        g_asTimeCost[ModuleIndex].dwTimeCost = g_dwTimeCost;
    }


    TimePrint();//打印初始化时间

}

void TimePrint(void)  //打印初始化时间函数
{
    int ii =0;
	
    CSTTPrint("\n\n\n");	
    CSTTPrint("*********开始打印各个模块初始化所需要的时间*************\n\n");

    for(ii = 0;g_asTimeCost[ii].pModuleName != NULL; ii ++)
    {
        CSTTPrint("*********%s 模块初始化所需时间: %d ms\r\n", g_asTimeCost[ii].pModuleName,g_asTimeCost[ii].dwTimeCost);
    }
    CSTTPrint("\n\n");
	
    CSTTPrint("**********************打印结束**************************\n\n\n");
}




INT CS_CA_Init(void)
{
	return 0;
}


INT CS_CA_RegisterMsgCallback(T_CBMsg hcbMsg)
{
	return 0;
}


INT CS_CA_GetVersion(BYTE* pacVerBuf,  DWORD dwBufSize)
{
	return 0;
}

INT CS_CA_StartSrvPlay(T_CaFtaService* pstService)
{
	return 0;
}

INT CS_CA_StopSrvPlay(T_CaFtaService* pstService)
{
	return 0;
}

INT CS_CA_NewBatSectionNotify(BYTE* pcSectionData, INT nLen, BYTE cDemux)
{
	return 0;
}

INT CS_CA_NewNitSectionNotify(BYTE* pcSectionData, INT nLen, BYTE cDemux)
{
	return 0;
}

INT CS_CA_NewPmtSectionNotify(BYTE* pcSectionData, INT nLen, BYTE cDemux)
{
	return 0;
}

INT CS_CA_NewCatSectionNotify(BYTE* pcSectionData, INT nLen, BYTE cDemux)
{
	return 0;
}

void CS_CA_SetDebugMode(T_CaTraceFlow dwMode)
{
	return;
}

INT CSCARegisterMsgCallback(DWORD dwUserData,F_CBMsg fcbMsg)
{
	return 0;
}

INT CSCARegisterMsgCallbackCancel(DWORD dwUserData,F_CBMsg fcbMsg)
{
	return 0;
}

INT CSCAStopService(HCSHANDLE hService)
{
	return 0;
}

HCSHANDLE CSCAStartService(CSCaFtaService_S* pstService)
{
	return 0;
}

INT CSCANewPmtSectionNotify(HCSHANDLE hService, BYTE* pcSectionData, INT nLen)
{
	return 0;
}

int CSGTPortingReadParam(char *name, char *buf, int len)
{
	return 0;
}

int CSGTPortingWriteParam(char *name, char *buf, int len)
{
	return 0;
}

int CSGTPortingStartTrailer(int x, int y, int w, int h, int trailer_id, const char *data)
{
	return 0;
}
int CSGTPortingSendCommand(const char *command, const char *data)
{
	return 0;
}
int shtelRTSPClientCreate(const char *pszURL, DWORD   dwTimeout, DWORD  dwUserData, 
	               FOnCSRTSPHaveMsgToUser 	fnCallback, 
	               HCSHANDLE *phRtspClient,int *pnSocket)
{
	return 0;
}

int shtelRTSPClientDestroy(HCSHANDLE hRtspClient)
{
	return 0;
}

int shtelRTSPClientSetTime(HCSHANDLE hRtspClient, DWORD dwTime)
{
	return 0;
}

int shtelRTSPClientGetTime(HCSHANDLE hRtspClient, int *pdwTime)
{
	return 0;
}

int shtelRTSPClientGetDuration(HCSHANDLE hRtspClient, DWORD *pdwTime)
{
	return 0;
}

int shtelRTSPClientPause(HCSHANDLE hRtspClient)
{
	return 0;
}

int shtelRTSPClientResume(HCSHANDLE hRtspClient)
{
	return 0;
}

int shtelRTSPClientSetRate(HCSHANDLE hRtspClient, int nRate)
{
	return 0;
}
int shtelRTSPClientSaveBookMark(HCSHANDLE hRtspClient, DWORD dwTime)
{
	return 0;
}

int shtelRTSPClientGetStartTime(HCSHANDLE hRtspClient, DWORD *pdwTime)
{
	return 0;
}

BOOL shtelRTSPClientIsSupportTrickMode(HCSHANDLE hRtspClient)
{
	return 0;
}
void restartStartTrailer(int nEos)
{
	return ;
}

int  ShtelReadStream(HCSHANDLE  hRtspClient, BYTE *pcBuffer, DWORD *pdwLen)
{
	return 0;
}
CSPcmEndian CSPcmPlaySetEndian(HCSHANDLE hPcm, CSPcmEndian enmEndian)
{
	return 0;
}

BOOL CSPcmPlayGetStatus(HCSHANDLE hPcm, CSPcmStatusParam_S *psPcmStatus)
{
    return TRUE;
}


BOOL CSTunerGetSignalInfo( HCSHANDLE hHandle, CSTUNSignalInfo_S *pSignalInfo)
{
	return 0;
}
BOOL USPSectionDemuxSetFilterBufSize(HCSHANDLE hHandle,int nFilter,int nBufSize)
{
	return 0;
}


