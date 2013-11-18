#include "setup.h"
#include "cspm_filemgr.h"
#include "cs_cainterface.h"
#include "udi_assert.h"

#ifndef ONLY_TEST_PVREGNINE
DWORD iConvertYMDtoMJD( WORD wyear, WORD wmonth, WORD wday)
{
	INT L;

	if( (wmonth == 1) 
		|| (wmonth == 2) )
	{
		L = 1;
	}
	else
	{
		L = 0;
	}

	wyear -= 1900; 
	return (DWORD)(14956 + wday + (int) ( (wyear - L)* 365.25) + (int)((wmonth + 1 + L* 12) * 30.6001));
}
static int  iGetWeekDay(int nYear,int nMonth,int nDay)
{
	if(nMonth<3)  {  
		nMonth  +=  12;  
		--nYear;  
	}  
	return (nDay+1+2*nMonth+3*(nMonth+1)/5+nYear+(nYear>>2)-nYear/100+nYear/400)  %  7;  
}

void iConvertMJDtoYMD( DWORD dwmjd, CSUDIPLUSOSDATE_S* pOutDate)
{
	INT Y,M,D,Y1,M1,K;

	Y1 = (int) ((dwmjd-15078.2)/365.25);
	M1 = (int) ( (dwmjd-14956.1-(int)(Y1*365.25) ) /30.6001);
	D = dwmjd-14956-(int)(Y1*365.25)-(int)(M1*30.6001);
	if( (M1 == 14) 
		|| (M1 == 15) )
	{
		K = 1;
	}
	else
	{
		K = 0;
	}
	Y = Y1+K;
	M = M1-1-K*12;

	pOutDate->m_nYear_13= (1900+Y);
	pOutDate->m_nMonth_8   = M;
	pOutDate->m_nDay_8      = D;
	pOutDate->m_nWeekDay_3 = iGetWeekDay((1900+Y),M,D);
}
void TimeAdd(CSUDIPLUSOSSysTime_S *psTime, int nIncSec)
{
	int nUp;
	DWORD dwMjdDay;
	int nTemp;

	nTemp = psTime->m_sTime.m_nSecond_6+ nIncSec;
	nUp = nTemp/60;
	psTime->m_sTime.m_nSecond_6 = nTemp % 60;
	if(nUp<=0)return;
	
	nTemp = psTime->m_sTime.m_nMinute_6+ nUp;
	nUp = nTemp/60;
	psTime->m_sTime.m_nMinute_6 = nTemp % 60;
	if(nUp<=0)return;

	nTemp = psTime->m_sTime.m_nHour_5+ nUp;
	//nUp = psTime->m_TTIME.m_dwHour_8/24;
	nUp = nTemp/24;			//modified by zhangying 11/27
	psTime->m_sTime.m_nHour_5 = nTemp % 24;
	if(nUp<=0)return;

	dwMjdDay = iConvertYMDtoMJD(psTime->m_sDate.m_nYear_13,psTime->m_sDate.m_nMonth_8,psTime->m_sDate.m_nDay_8);
	dwMjdDay += nUp;
	iConvertMJDtoYMD(dwMjdDay,&psTime->m_sDate);
}
#if 0
extern TTIME getTime();
extern TDATE getDate();
void GetCurEpgTime(CSUDIPLUSOSSysTime_S *pOutSysTime)
{
	if(CSASSERT_FAILED(pOutSysTime)) return ;
	pOutSysTime->m_sTime = getTime();
	pOutSysTime->m_TDATE = getDate();
	
/*¼ÓÏµÍ³Æ«ÒÆ*/
	pOutSysTime->m_sTime.m_dwHour_8 += 8;
	if(pOutSysTime->m_TTIME.m_dwHour_8 >=24)
	{
		DWORD dwMjdDay;
		
		pOutSysTime->m_TTIME.m_dwHour_8 -= 24;
		dwMjdDay = iConvertYMDtoMJD(pOutSysTime->m_TDATE.m_dwYear_13,pOutSysTime->m_TDATE.m_dwMonth_8,pOutSysTime->m_TDATE.m_dwDay_8);
		dwMjdDay++;
		iConvertMJDtoYMD(dwMjdDay,&pOutSysTime->m_TDATE);
	}
}
#endif
static CSUDIPLUSOSSysTime_S s_sDefaultTime = {{2008,8,8,5},{8,8,8,8}};

void GetCurEpgTime(CSUDIPLUSOSSysTime_S *pOutSysTime)
{
	static unsigned int s_dwSysStartTick = 0;
	unsigned int dwCurTick = CSUDIPlusOSGetTickCount();
 
	TimeAdd(&s_sDefaultTime,(dwCurTick-s_dwSysStartTick)/1000);
	*pOutSysTime = s_sDefaultTime;
	s_dwSysStartTick = dwCurTick;
}

static BOOL iSaveToFlash(PVOID pData,int nDataLen)
{	
	return  TRUE;

}
static BOOL iReadFlash(PVOID pData,int nDataLen)
{	
	return  TRUE;
}
void SetupPVR(void)
{
	CSPVRInitParam_S sPvrInit;
	BOOL bRet;
	
	CSTRACE(0xff,"[%s:%d] SetUpPVR\n",__FUNCTION__,__LINE__);
	CSPVRGetDefaultParam(&sPvrInit);
	sPvrInit.asPlayDevice[0].nAudioDeviceIdx = 0;
	sPvrInit.asPlayDevice[0].nVideoDeviceIdx = 0;
	sPvrInit.asPlayDevice[0].nTtxDeviceIdx = 0;
	sPvrInit.asPlayDevice[0].nTvWinIdx = 0;
	sPvrInit.asPlayDevice[0].nTShiftRecIdx = 0;
	sPvrInit.dwFileRecCount = 1;
	sPvrInit.dwPlayCount = 1;
	sPvrInit.dwTShiftRecCount = 1;
	sPvrInit.dwMaxDataSize = CSPVR_DEFAULT_DATA_MAX_SIZE;
	sPvrInit.fnGetSysTime = (CSPVRGetSysTime_F)GetCurEpgTime;
	sPvrInit.fnLoadData = iReadFlash;
	sPvrInit.fnSaveData = iSaveToFlash;

	bRet = CSPVRInit(&sPvrInit);
	CSASSERT(bRet);
	
	CS_CA_Init();
#if defined(PLATFORM_ST40)
	{
		const int WAIT_TIME = 20;
		int i;
		for(i=0;i<WAIT_TIME;i++)
		{
			CSUDIOSThreadSleep(1000);
			CSTRACE(0xff,"[%s:%d]waiting fo mount device....\n",__FUNCTION__,__LINE__);
			if(EM_PVRFILE_STORAGE_USABLE == CSPVRFILEGetStorageStatus())
			{
				break;
			}
		}
		if(i==WAIT_TIME)
		{
			CSDebug("TestMain",ERROR_LEVEL, "NO STORAGE !!!!!!!!!!!!!!!!!!!! cannot test Pvr !!!!!!!!!!!\n");
		}
	}
#endif

	//add by wm 100402 for test init
	CSUDIOSThreadSleep(20*1000);
}
#else

#include "CSPvrPlay.h"
#include "CSRecord.h"

void SetupPVR(void)
{
	DWORD i;

	BOOL bRet = CSPvrBase_Init();
	if(CSASSERT_FAILED(bRet)) return FALSE;

	EFileResult eFileRet;

	eFileRet = CSFile_Init();
	CSASSERT(eFileRet == EM_FILE_SUCCESS);

	ERecordResult eRecRet = CSRecord_Init(1, 1);
	if(CSASSERT_FAILED(eRecRet == EM_RECORD_SUCCESS)) return FALSE;

	SPlayInit aPlayInit[2];
	memset(aPlayInit, 0, sizeof(aPlayInit));

	aPlayInit[1].nVideoIdx = INVALID_DEVICE_INDEX;
	aPlayInit[1].nAudioIdx = INVALID_DEVICE_INDEX;
	aPlayInit[1].nTtxIdx = INVALID_DEVICE_INDEX;

	EPlayResult ePlayRet = CSPlay_Init(aPlayInit);
	if(CSASSERT_FAILED(ePlayRet==EM_PLAY_SUCCESS)) return FALSE;
}
#endif


