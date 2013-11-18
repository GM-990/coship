/****************************************************************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*              All rights reserved.
* Filename:    CS_IRDETO_Loader.c  
* Author:      wansuanlin
* Date:        Match 2006 
* Description: this file provide loader function          
*****************************************************************************************************************************/
#include "CS_IRDETOCA_Loader.h"
#include <stdio.h>
#include <stdlib.h>
#include "cs_testkit.h"

#define CA_DBG_LOADER    (TRUE)

#if(CA_DBG_LOADER == TRUE)
#define DBG_LOADER_Print CSTTPrint
#else
#define DBG_LOADER_Print
#endif
/*****************************************************************************************************************************/
#define CA_HAVE_LOADER  (FALSE)

//内部函数声明
/*****************************************************************************************************************************/
#if (CA_HAVE_LOADER == TRUE)

static void ReCoverKeyStatus(BYTE* pcKeyStatus, KEY_STATUS* pstKeyStatus);
#endif
/*****************************************************************************************************************************/
void LFromMJDUTC(BYTE* MjdUtc,BYTE* year,BYTE* month,BYTE* day,BYTE* hour,BYTE* min,BYTE* second)
{
	DWORD mjd;
	DWORD utc;
	char szBuf[16];
	int y1,m1;
	int k;

	mjd = MjdUtc[0]<<8 | MjdUtc[1];
	sprintf(szBuf,"%02x%02x%02x",MjdUtc[2],MjdUtc[3],MjdUtc[4]);
	utc = atol(szBuf);
	//
	y1 = (int)((mjd-15078.2)/365.25);
	m1 = (int)((mjd-14956.1-(int)(y1*365.25))/30.6001);

	k = 0;
	if(m1 == 14 || m1 == 15)
	{
		k = 1;
	}
	*day = mjd-14956-(int)(y1*365.25)-(int)(m1*30.6001);
	*year = (y1 + k)%100;
	*month = m1 - 1 - k*12;

	*hour = utc/10000;
	*min = (utc-(*hour)*10000)/100;
	*second = utc-(*hour)*10000 -(*min)*100;
}

int  IMin(int x1,int x2)
{
	return (x1<x2?x1:x2);
}

#if (CA_HAVE_LOADER == TRUE)
INT CS_CA_Loader_Init()
{
	DBG_LOADER_Print("\nLoader Init Starting...\n");
	NVM_DRV_Initialise();
	I_LDRSUP_Initialise();

	return SUCCESS;
}

INT CS_CA_Loader_GetCPCB(T_CpcbInfo*  pstBuf )
{
	CPCB_STRUCT  stCpcbInfo;

	DBG_LOADER_Print("\n[CS_CA_Loader_GetCPCB] Start---pstBuf:%08x\n", pstBuf);

	ASSERT(NULL != pstBuf);
	if(NULL == pstBuf)
	{
		ASSERT(FALSE);
		return FAILURE;
	}

	memset((BYTE*)&stCpcbInfo, 0, sizeof(stCpcbInfo));
	FLSH_MNG_ReadCPCB(&stCpcbInfo);

	pstBuf->m_cLoadNum           = stCpcbInfo.bLoad_sequence_number;
	pstBuf->m_dwBin2irdVer       = stCpcbInfo.uBin2ird_version;
	pstBuf->m_dwBin2irdSerialNum = stCpcbInfo.uBin2ird_serial_number;
	pstBuf->m_wDownloadDuration  = stCpcbInfo.wDownload_duration;
	pstBuf->m_dwImageSize        = stCpcbInfo.uImage_size;
	memcpy( pstBuf->m_acSignTimeDate,
			stCpcbInfo.abSign_time_date,
			sizeof(stCpcbInfo.abSign_time_date));
	memcpy( pstBuf->m_acDownloadTimeDate,
			stCpcbInfo.abDownload_time_date,
			sizeof(stCpcbInfo.abDownload_time_date));

	DBG_LOADER_Print("\n[CS_CA_Loader_GetCPCB] Exit---LoadNum:%d\n", pstBuf->m_cLoadNum);

	return SUCCESS;
}

INT CS_CA_Loader_GetBBCB(T_BbcbInfo*  pstBuf)
{
	FLASH_BB_VERSION_STRUCTURE stBbcbInfo;

	DBG_LOADER_Print("\n[CS_CA_Loader_GetBBCB] Start---pstBuf:%08x\n", pstBuf);

	ASSERT(NULL != pstBuf);
	if(NULL == pstBuf)
	{
		ASSERT(FALSE);
		return FAILURE;
	}

	memset((BYTE*)&stBbcbInfo, 0, sizeof(stBbcbInfo));
	FLSH_MNG_ReadBBCB(&stBbcbInfo);

	pstBuf->m_wCRC             = stBbcbInfo.ulCRC;
	pstBuf->m_cManufactureCode = stBbcbInfo.bManufactureCode;
	pstBuf->m_cHW_Version      = stBbcbInfo.bHW_Version;
	pstBuf->m_cVerMajor        = stBbcbInfo.bLDR_Version_Major;
	pstBuf->m_cVerMinor        = stBbcbInfo.bLDR_Version_Minor;
	memcpy( pstBuf->m_acSerialNumber,
			stBbcbInfo.abSerialNumber,
			sizeof(stBbcbInfo.abSerialNumber));
	memcpy( pstBuf->m_acRandomNumber,
			stBbcbInfo.abRandomNumber,
			sizeof(stBbcbInfo.abRandomNumber));

	//HexPrintStr("pstBuf->m_acRandomNumber", pstBuf->m_acRandomNumber, 4);
	//HexPrintStr("stBbcbInfo.abRandomNumber", stBbcbInfo.abRandomNumber, 4);

	DBG_LOADER_Print("\n[CS_CA_Loader_GetBBCB] Exit---ManCode:%d, HwVer:%d\n", pstBuf->m_cManufactureCode, pstBuf->m_cHW_Version);

	return SUCCESS;
}

static void ReCoverKeyStatus(BYTE* pcKeyStatus, KEY_STATUS* pstKeyStatus)
{
	//[KEY STATUS:  01 e2 ff ff 00 00 00 21 00 00 07 4f 56 08 00 00   KEY STATUS]
	
	if((NULL == pcKeyStatus) || (NULL == pstKeyStatus))	
		return;

	pstKeyStatus->KeyStatus     = pcKeyStatus[0];
	pstKeyStatus->LastImage     = pcKeyStatus[1];
	pstKeyStatus->SysID         = (pcKeyStatus[2]<<8) | pcKeyStatus[3];
	pstKeyStatus->Key_Ver       = (pcKeyStatus[4]<<8) | pcKeyStatus[5];
	
	pstKeyStatus->Variant       = (pcKeyStatus[6]<<8) | pcKeyStatus[7];
	pstKeyStatus->Sig_Ver       = (pcKeyStatus[8]<<8) | pcKeyStatus[9];
	pstKeyStatus->Random_number = (pcKeyStatus[10]<<24) | (pcKeyStatus[11]<<16) | (pcKeyStatus[12]<<8) | (pcKeyStatus[13]<<0);
	pstKeyStatus->Reserved[0]   = pcKeyStatus[14];
	pstKeyStatus->Reserved[1]   = pcKeyStatus[15];
}

INT CS_CA_Loader_GetKeyStatus(T_KeyStatusEx*  pstKeyStatus)
{
	KEY_STATUS  stKeyStatus;

	DBG_LOADER_Print("\n[CS_CA_Loader_GetKeyStatus] Start---pstKeyStatus:%08x\n", pstKeyStatus);
	
	ASSERT(NULL != pstKeyStatus);
	if(NULL == pstKeyStatus)
	{
		ASSERT(FALSE);
		return FAILURE;
	}

	memset((BYTE*)&stKeyStatus, 0, sizeof(stKeyStatus));
	I_LDRSUP_GetKeyStatus((BYTE*)&stKeyStatus);
	ReCoverKeyStatus((BYTE*)&stKeyStatus, &stKeyStatus);
	
	#if 0
	pstKeyStatus->m_cKeyStatus   = stKeyStatus.KeyStatus;
	pstKeyStatus->m_cLastImage   = stKeyStatus.LastImage;
	pstKeyStatus->m_wSysID       = stKeyStatus.SysID;
	pstKeyStatus->m_wKeyVer      = stKeyStatus.Key_Ver;
	pstKeyStatus->m_wVariant     = stKeyStatus.Variant;
	pstKeyStatus->m_wSigVer      = stKeyStatus.Sig_Ver;
	pstKeyStatus->m_dwRandomNum  = stKeyStatus.Random_number;
	#else
	pstKeyStatus->m_cKeyStatus   = stKeyStatus.KeyStatus;
	pstKeyStatus->m_cLastImage   = stKeyStatus.LastImage;
	pstKeyStatus->m_wSysID       = 0x61;
	pstKeyStatus->m_wKeyVer      = stKeyStatus.Key_Ver;
	pstKeyStatus->m_wVariant     = 0x255;
	pstKeyStatus->m_wSigVer      = 0x2;
	pstKeyStatus->m_dwRandomNum  = stKeyStatus.Random_number;
	#endif

	DBG_LOADER_Print("\n[CS_CA_Loader_GetKeyStatus] Exit---\n");

	return SUCCESS;
}

INT CS_CA_Loader_SaveOtaInfo(T_OtaCfg*  pstOtaInfo)
{
	INT ii;
	LOADER_PT_STRUCT  	stLoaderPt;
	OTA_SYSTEM_T      	stSystem;
	INT nRet = FAILURE;

	ASSERT(NULL != pstOtaInfo);
	if(NULL == pstOtaInfo)
		return FAILURE;

	ASSERT(CS_CA_AppDependence_GetFtaType() == pstOtaInfo->m_eType);
	if(CS_CA_AppDependence_GetFtaType() != pstOtaInfo->m_eType)
		return FAILURE;
	
	memset((BYTE*)&stLoaderPt, 0, sizeof(stLoaderPt));
	memset((BYTE*)&stSystem, 0, sizeof(stSystem));
	
	if(E_CA_FTA_CABLE == pstOtaInfo->m_eType) 
	{
		stSystem.cable.dwFrequencyKHz = pstOtaInfo->m_stDlySys.m_stCableStytle.m_dwFrequency;
		stSystem.cable.dwSymbolRate   = pstOtaInfo->m_stDlySys.m_stCableStytle.m_dwSymbolRate;
		stSystem.cable.nAnnexBQAMMode = pstOtaInfo->m_stDlySys.m_stCableStytle.m_cModulation;
	}

	CRC_CreateTables();
	
	stLoaderPt.fEnableOtaDL	= TRUE;
	stLoaderPt.fFlashCorrupted	= FALSE;

	for(ii = 0; ii < 2; ii++)
	{
		memset((BYTE*)&stLoaderPt.astTS[ii], 0, sizeof(stLoaderPt.astTS[ii]));
		stLoaderPt.astTS[ii].wPID      = pstOtaInfo->m_wOtaPid;
		stLoaderPt.astTS[ii].bTable_ID = pstOtaInfo->m_cOtaTableId;
		memcpy(stLoaderPt.astTS[ii].abDS, &stSystem, sizeof(stSystem));
		stLoaderPt.astTS[ii].wCRC16    = CRC_16((void*)&stLoaderPt.astTS[ii], sizeof(stLoaderPt.astTS[ii])-sizeof(stLoaderPt.astTS[ii].wCRC16));
	}
	
	stLoaderPt.wCRC16 = CRC_16((void*)&stLoaderPt, sizeof(stLoaderPt)- sizeof(stLoaderPt.wCRC16));

	if(NVM_MNG_WriteLoaderPt(&stLoaderPt) != SUCCESS)
	{
		ASSERT(FALSE);

		return FAILURE;
	}

	return SUCCESS;
}

BOOL CS_CA_Loader_ReadIrdCfg(T_LoaderIrdInfo*  pIrdCfg)
{
	INT nRet = FAILURE;
	T_CpcbInfo  stCpcbInfo;
	T_BbcbInfo  stBbcbInfo;
	T_KeyStatusEx stKeyStatus;
	
	ASSERT(NULL != pIrdCfg);
	if(NULL == pIrdCfg)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	DBG_LOADER_Print("\nEnter Read LDR OK in ReadIRDCfg {");

	memset((BYTE*)&stCpcbInfo, 0, sizeof(stCpcbInfo));
	memset((BYTE*)&stBbcbInfo, 0, sizeof(stBbcbInfo));
	memset((BYTE*)&stKeyStatus, 0, sizeof(stKeyStatus));

	nRet = CS_CA_Loader_GetCPCB(&stCpcbInfo);
	if(SUCCESS != nRet)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	nRet = CS_CA_Loader_GetBBCB(&stBbcbInfo);
	if(SUCCESS != nRet)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	nRet = CS_CA_Loader_GetKeyStatus(&stKeyStatus);
	if(SUCCESS != nRet)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	pIrdCfg->m_wManufacturerCode = stBbcbInfo.m_cManufactureCode;
	pIrdCfg->m_wHardwareCode     = stBbcbInfo.m_cHW_Version;
	pIrdCfg->m_wVariant          = stKeyStatus.m_wVariant;
	pIrdCfg->m_wLoadNum          = stCpcbInfo.m_cLoadNum;

	DBG_LOADER_Print("\n(Mc:%d, Hc:%d, Va:%d, Ln:%d)", stBbcbInfo.m_cManufactureCode, stBbcbInfo.m_cHW_Version,
												stKeyStatus.m_wVariant, stCpcbInfo.m_cLoadNum);
	DBG_LOADER_Print("\nLeave Read LDR OK in ReadIRDCfg}\n");

	return TRUE;
}

BOOL CS_CA_Loader_GetIrdStatus(T_CaIrdStatus *pstIrdStatus)
{
	T_KeyStatusEx stKeyStatus;
	T_BbcbInfo  stBbcbInfo;
	T_CpcbInfo  stCpcbInfo;
	char szLDRStatus[16];


	DBG_LOADER_Print("\nEnter Read LDR OK in GetIRDCfg{");
	//CS_CA_GetFTAVer(FTA_FIRMWARE,(char*)pstIrdStatus->m_szFirmware, sizeof(pstIrdStatus->m_szFirmware));
	//CS_CA_GetFTAVer(FTA_IRDSW,(char*)pstIrdStatus->m_szIRDSW, sizeof(pstIrdStatus->m_szIRDSW));
	//CS_CA_GetFTAVer(FTA_APPL,(char*)pstIrdStatus->m_szAPPSW, sizeof(pstIrdStatus->m_szAPPSW));
	//CS_CA_GetFTAVer(FTA_DRIVER,(char*)pstIrdStatus->m_szDRIVERSW, sizeof(pstIrdStatus->m_szDRIVERSW));
	//CS_CA_GetFTAVer(FTA_MIDDLEWARE,(char*)pstIrdStatus->m_szMiddleSW,sizeof(pstIrdStatus->m_szMiddleSW));

	if(SUCCESS != CS_CA_Loader_GetBBCB(&stBbcbInfo))
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	if(SUCCESS != CS_CA_Loader_GetKeyStatus(&stKeyStatus))
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	if(SUCCESS != CS_CA_Loader_GetCPCB(&stCpcbInfo))
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	DBG_LOADER_Print("\n[FROM LDR]SerialNumber = %02d%02d%02d%02d", stBbcbInfo.m_acSerialNumber[3], stBbcbInfo.m_acSerialNumber[2], 
														stBbcbInfo.m_acSerialNumber[1], stBbcbInfo.m_acSerialNumber[0]);
	DBG_LOADER_Print("\n[FROM LDR]bManufactureCode = %d", stBbcbInfo.m_cManufactureCode);
	DBG_LOADER_Print("\n[FROM LDR]bHW_Version = %d", stBbcbInfo.m_cHW_Version);
	DBG_LOADER_Print("\n[FROM LDR]Variant = %d", stKeyStatus.m_wVariant);
	DBG_LOADER_Print("\n[FROM LDR]LoadNum = %d", stCpcbInfo.m_cLoadNum);

	sprintf(pstIrdStatus->m_szSerialNumber,"%d",
			stBbcbInfo.m_acSerialNumber[3]|
			(stBbcbInfo.m_acSerialNumber[2]<<8)|
			(stBbcbInfo.m_acSerialNumber[1]<<16)|
			(stBbcbInfo.m_acSerialNumber[0]<<24));
	
	sprintf(pstIrdStatus->m_szRandomNumber,"%06d",
			stBbcbInfo.m_acRandomNumber[3]|
			(stBbcbInfo.m_acRandomNumber[2]<<8)|
			(stBbcbInfo.m_acRandomNumber[1]<<16)|
			(stBbcbInfo.m_acRandomNumber[0]<<24));
	
	sprintf(pstIrdStatus->m_szIrdVer,"%d.%d.%d", stBbcbInfo.m_cManufactureCode,
											stBbcbInfo.m_cHW_Version,
											stKeyStatus.m_wVariant);

	sprintf(pstIrdStatus->m_szFirmware, "%s", "1.1");

	memset(szLDRStatus,0,sizeof(szLDRStatus));
	switch(stKeyStatus.m_cKeyStatus)
	{
	case 0:
		strcpy(szLDRStatus,"KDO");
		break;
	case 1:
		strcpy(szLDRStatus,"OK");
		break;
	case 2:
		strcpy(szLDRStatus,"ERR");
		break;
	default:
		strcpy(szLDRStatus,"NEW");
		break;		
	}

	sprintf(pstIrdStatus->m_szLoader,
		"%c.%c        %d:%d:%d  %s       L-%d",
		(stBbcbInfo.m_cVerMajor==0xFF?-1:stBbcbInfo.m_cVerMajor),	(stBbcbInfo.m_cVerMinor==0xFF?-1:stBbcbInfo.m_cVerMinor),
		(stKeyStatus.m_wSysID==0xFFFF?-1:stKeyStatus.m_wSysID),(stKeyStatus.m_wKeyVer==0xFFFF?-1:stKeyStatus.m_wKeyVer),(stKeyStatus.m_wSigVer==0xFFFF?-1:stKeyStatus.m_wSigVer),
		szLDRStatus,
		stCpcbInfo.m_cLoadNum);

	return TRUE;
}

BOOL CS_CA_GetLdrStatus(T_CaLoaderStatus* pstLoaderStatus)
{
	T_KeyStatusEx  stKeyStatus;
	T_BbcbInfo stBbcbInfo;
	T_CpcbInfo stCpcbInfo;
	BYTE y,m,d,h,m1,s;
	char szLDRStatus[16];

	DBG_LOADER_Print("\nEnter Read LDR OK in GetLDRStatus{");
	if(SUCCESS != CS_CA_Loader_GetBBCB(&stBbcbInfo))
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	if(SUCCESS != CS_CA_Loader_GetKeyStatus(&stKeyStatus))
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	if(SUCCESS != CS_CA_Loader_GetCPCB(&stCpcbInfo))
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	DBG_LOADER_Print("\nLeave Read LDR OK in GetLDRStatus}");

	DBG_LOADER_Print("\n[FROM LDR]SerialNumber =%02d%02d%02d", stBbcbInfo.m_acSerialNumber[3], stBbcbInfo.m_acSerialNumber[2], 
														stBbcbInfo.m_acSerialNumber[1], stBbcbInfo.m_acSerialNumber[0]);
	DBG_LOADER_Print("\n[FROM LDR]bManufactureCode = %d", stBbcbInfo.m_cManufactureCode);
	DBG_LOADER_Print("\n[FROM LDR]bHW_Version = %d", stBbcbInfo.m_cHW_Version);
	DBG_LOADER_Print("\n[FROM LDR]Variant = %d", stKeyStatus.m_wVariant);
	DBG_LOADER_Print("\n[FROM LDR]uBin2ird_version = %d", stCpcbInfo.m_dwBin2irdVer);
	DBG_LOADER_Print("\n[FROM LDR]uBin2ird_serial_number = %d",stCpcbInfo.m_dwBin2irdSerialNum);
	
	sprintf(pstLoaderStatus->m_szSerialNumber,"%d",
			stBbcbInfo.m_acSerialNumber[3]|
			(stBbcbInfo.m_acSerialNumber[2]<<8)|
			(stBbcbInfo.m_acSerialNumber[1]<<16)|
			(stBbcbInfo.m_acSerialNumber[0]<<24));
	
	sprintf(pstLoaderStatus->m_szLoader,"%c.%c",(stBbcbInfo.m_cVerMajor==0xFF?-1:stBbcbInfo.m_cVerMajor),(stBbcbInfo.m_cVerMinor==0xFF?-1:stBbcbInfo.m_cVerMinor));		
	sprintf(pstLoaderStatus->m_szManuID,"%d",stBbcbInfo.m_cManufactureCode);
	sprintf(pstLoaderStatus->m_szHWVer,"%d",stBbcbInfo.m_cHW_Version);

	memset(szLDRStatus,0,sizeof(szLDRStatus));
	switch(stKeyStatus.m_cKeyStatus)
	{
	case 0:
		strcpy(szLDRStatus,"KDO");
		break;
	case 1:
		strcpy(szLDRStatus,"OK");
		break;
	case 2:
		strcpy(szLDRStatus,"ERR");
		break;
	default:
		strcpy(szLDRStatus,"NEW");
		break;		
	}

	sprintf(pstLoaderStatus->m_szLDSeqNo,"%d",stCpcbInfo.m_cLoadNum);
	sprintf(pstLoaderStatus->m_szKEYSeqNo,"%d",stKeyStatus.m_wSigVer);
	sprintf(pstLoaderStatus->m_szVARIANT,"%d",stKeyStatus.m_wVariant);
	sprintf(pstLoaderStatus->m_szKEYSysId,"%d",(stKeyStatus.m_wSysID==0xFFFF?-1:stKeyStatus.m_wSysID));	
	sprintf(pstLoaderStatus->m_szKEYStatus,"%s",szLDRStatus);
	sprintf(pstLoaderStatus->m_szKEYVer,"%d",stKeyStatus.m_wKeyVer);

	sprintf(pstLoaderStatus->m_szBIN2IRDVer,"%c%c%c %c",
		(stCpcbInfo.m_dwBin2irdVer)>>24,
		0x2e,
		(stCpcbInfo.m_dwBin2irdVer & 0x00FF0000)>>16,
		(stCpcbInfo.m_dwBin2irdVer & 0x000000FF)
		);
	sprintf(pstLoaderStatus->m_szBIN2IRDSer,"%c%c%c%c",
		stCpcbInfo.m_dwBin2irdSerialNum>>24,
		(stCpcbInfo.m_dwBin2irdSerialNum <<8) >> 24,
		(stCpcbInfo.m_dwBin2irdSerialNum <<16) >> 24,
		(stCpcbInfo.m_dwBin2irdSerialNum <<24) >> 24
		);
	LFromMJDUTC(stCpcbInfo.m_acSignTimeDate,&y,&m,&d,&h,&m1,&s);
	sprintf(pstLoaderStatus->m_szSignDate,"%02d/%02d/%02d",d,m,y);
	sprintf(pstLoaderStatus->m_szSignTime,"%02d:%02d:%02d",h,m1,s);

	LFromMJDUTC(stCpcbInfo.m_acDownloadTimeDate,&y,&m,&d,&h,&m1,&s);
	sprintf(pstLoaderStatus->m_szDLDate,"%02d/%02d/%02d",d,m,y);
	sprintf(pstLoaderStatus->m_szDLTime,"%02d:%02d:%02d",h,m1,s);
	
	h = 0; m1 = 0; s= 0;
	h = stCpcbInfo.m_wDownloadDuration / 3600;
	m1 = (stCpcbInfo.m_wDownloadDuration - h*3600) /60;
	s = stCpcbInfo.m_wDownloadDuration - h*3600 - m1*60;
	sprintf(pstLoaderStatus->m_szDLDur,"%d:%d:%d",h,m1,s);
	sprintf(pstLoaderStatus->m_szImageSize,"%d",stCpcbInfo.m_dwImageSize);
	
	return TRUE;
}
#else
INT CS_CA_Loader_Init()
{
	return SUCCESS;
}

INT CS_CA_Loader_GetCPCB(T_CpcbInfo*  pstBuf )
{
	return SUCCESS;
}

INT CS_CA_Loader_GetBBCB(T_BbcbInfo*  pstBuf)
{
	return SUCCESS;
}

INT CS_CA_Loader_GetKeyStatus(T_KeyStatusEx*  pstKeyStatus)
{
	return SUCCESS;
}

INT CS_CA_Loader_SaveOtaInfo(T_OtaCfg*  pstOtaInfo)
{
	return SUCCESS;
}

BOOL CS_CA_Loader_ReadIrdCfg(T_LoaderIrdInfo*  pIrdCfg)
{
	return TRUE;
}

BOOL CS_CA_Loader_GetIrdStatus(T_CaIrdStatus *pstIrdStatus)
{
	return TRUE;
}

BOOL CS_CA_GetLdrStatus(T_CaLoaderStatus* pstLoaderStatus)
{
	return TRUE;
}
#endif


#if(CA_HAVE_LOADER == FALSE)
WORD I_HLSUP_GetStatusInfo (IHL_PSTATUS pstStatus)
{	
	CSTTPrint("\n%s\r\n",__FUNCTION__);
	return 0;
	pstStatus->u16SysID = 0x61;
	pstStatus->s16SysIDValid = 0;
	pstStatus->u16KeyVer = 0;
	pstStatus->s16KeyVerValid = 0;
	pstStatus->u16SigVer = 0x2;
	pstStatus->s16SigVerValid = 0;
	pstStatus->u16Variant = 0x255;
	pstStatus->s16VariantValid = 0;
	pstStatus->u8BlockFailMask = 0;

	return 0;
}
#else
WORD I_HLSUP_GetStatusInfo (IHL_PSTATUS pstStatus)
{
	T_KeyStatusEx	stKeyStatus;
	CSTTPrint("\n%s\r\n",__FUNCTION__);
	return 0;
	CS_CA_Loader_GetKeyStatus(&stKeyStatus);

	pstStatus->u16SysID = stKeyStatus.m_wSysID;
	pstStatus->s16SysIDValid = 0;//TEST
	pstStatus->u16KeyVer = stKeyStatus.m_wKeyVer;
	pstStatus->s16KeyVerValid = 0;//TEST
	pstStatus->u16SigVer = stKeyStatus.m_wSigVer;
	pstStatus->s16SigVerValid =  0;//TEST
	pstStatus->u16Variant = stKeyStatus.m_wVariant;
	pstStatus->s16VariantValid =  0;//TEST
	pstStatus->u8BlockFailMask = 0;//TEST

	return 0;//I_HLSUP_OK;	
}
#endif


