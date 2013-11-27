#include "udiplus_os.h"
#include "udiplus_debug.h"
#include <string.h>

/*-----This code is copied from linux udftime.c--------------*/
#define EPOCH_YEAR 		 		1970
#define MAX_YEAR_SECONDS 		69
#define SPD 					0x15180 /*3600*24*/
#define SPY(y,l,s) 				(SPD * (365*y+l)+s)//l闰年调整,s秒调整(暂无)
#define	__isleap(year)			(((year) % 4 == 0 && ((year) % 100 != 0 || (year) % 400 == 0))?1:0)
#define SECS_PER_HOUR			(60 * 60)
#define SECS_PER_DAY			(SECS_PER_HOUR * 24)

#define DIV(a,b) 				((a) / (b) - (((a) % (b) < 0)?1:0))
#define LEAPS_THRU_END_OF(y)	(DIV (y, 4) - DIV (y, 100) + DIV (y, 400))

static CSUDIPLUS_TIME_T year_seconds[MAX_YEAR_SECONDS]= {
/*1970*/ SPY( 0, 0,0), SPY( 1, 0,0), SPY( 2, 0,0), SPY( 3, 1,0), 
/*1974*/ SPY( 4, 1,0), SPY( 5, 1,0), SPY( 6, 1,0), SPY( 7, 2,0), 
/*1978*/ SPY( 8, 2,0), SPY( 9, 2,0), SPY(10, 2,0), SPY(11, 3,0), 
/*1982*/ SPY(12, 3,0), SPY(13, 3,0), SPY(14, 3,0), SPY(15, 4,0), 
/*1986*/ SPY(16, 4,0), SPY(17, 4,0), SPY(18, 4,0), SPY(19, 5,0), 
/*1990*/ SPY(20, 5,0), SPY(21, 5,0), SPY(22, 5,0), SPY(23, 6,0), 
/*1994*/ SPY(24, 6,0), SPY(25, 6,0), SPY(26, 6,0), SPY(27, 7,0), 
/*1998*/ SPY(28, 7,0), SPY(29, 7,0), SPY(30, 7,0), SPY(31, 8,0), 
/*2002*/ SPY(32, 8,0), SPY(33, 8,0), SPY(34, 8,0), SPY(35, 9,0), 
/*2006*/ SPY(36, 9,0), SPY(37, 9,0), SPY(38, 9,0), SPY(39,10,0), 
/*2010*/ SPY(40,10,0), SPY(41,10,0), SPY(42,10,0), SPY(43,11,0), 
/*2014*/ SPY(44,11,0), SPY(45,11,0), SPY(46,11,0), SPY(47,12,0), 
/*2018*/ SPY(48,12,0), SPY(49,12,0), SPY(50,12,0), SPY(51,13,0), 
/*2022*/ SPY(52,13,0), SPY(53,13,0), SPY(54,13,0), SPY(55,14,0), 
/*2026*/ SPY(56,14,0), SPY(57,14,0), SPY(58,14,0), SPY(59,15,0), 
/*2030*/ SPY(60,15,0), SPY(61,15,0), SPY(62,15,0), SPY(63,16,0), 
/*2034*/ SPY(64,16,0), SPY(65,16,0), SPY(66,16,0), SPY(67,17,0), 
/*2038*/ SPY(68,17,0)
};

/* How many days come before each month (0-12).  */
static const unsigned short int __mon_yday[2][13] =
{
	/* Normal years.  */
	{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 },
	/* Leap years.  */
	{ 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 }
};

CSUDI_Error_Code CSUDIPlusOSSystime2Second(CSUDIPLUSOSSysTime_S* psDateTime,CSUDIPLUS_TIME_T* pnSeconds)
{
	int yday;
	int year,month,day,hour,minute,second;
	
	if(psDateTime == CSUDI_NULL || pnSeconds == CSUDI_NULL)
	{
		return CSUDIOSPLUS_ERROR_BAD_PARAMETER;
	}
	
	year = psDateTime->m_sDate.m_nYear_13;
	month = psDateTime->m_sDate.m_nMonth_8;
	day = psDateTime->m_sDate.m_nDay_8;
	hour = psDateTime->m_sTime.m_nHour_5;
	minute = psDateTime->m_sTime.m_nMinute_6;
	second = psDateTime->m_sTime.m_nSecond_6;
	
	if ((year < EPOCH_YEAR)
		||(year >= EPOCH_YEAR+MAX_YEAR_SECONDS)
		||(month < 1)
		||(month > 12)
		/*这几项不会造成数组溢出，可以容错		
		||(day < 1)
		||(day > 31)
		||(hour > 23)
		||(minute > 59)
		||(second > 59)
		*/
		)
	{
		return CSUDIOSPLUS_ERROR_BAD_PARAMETER;
	}
	
	*pnSeconds = year_seconds[year - EPOCH_YEAR];

	yday = __mon_yday[__isleap(year)][month-1] + (day-1);
	
	*pnSeconds += ( ( (yday* 24) + hour ) * 60 + minute ) * 60 + second;
	
	return CSUDI_SUCCESS;
}

CSUDI_Error_Code CSUDIPlusOSSecond2Systime(CSUDIPLUS_TIME_T nSeconds,CSUDIPLUSOSSysTime_S* psDateTime)
{
	long int days, rem, y;
	const unsigned short int *ip;

	if(psDateTime == CSUDI_NULL)
	{
		return CSUDIOSPLUS_ERROR_BAD_PARAMETER;
	}

	memset(psDateTime,0,sizeof(CSUDIPLUSOSSysTime_S));

	days = nSeconds / SECS_PER_DAY;
	rem = nSeconds % SECS_PER_DAY;
	psDateTime->m_sTime.m_nHour_5 = rem / SECS_PER_HOUR;
	rem %= SECS_PER_HOUR;
	psDateTime->m_sTime.m_nMinute_6 = rem / 60;
	psDateTime->m_sTime.m_nSecond_6 = rem % 60;

	psDateTime->m_sDate.m_nWeekDay_3 = (days+4)%7; /* day 0 was a thursday */
	
	y = 1970;

	//ok I can not very understand this code from udf_time_to_stamp()
	while (days < 0 || days >= (__isleap(y) ? 366 : 365))
	{
		long int yg = y + days / 365 - ((days % 365 < 0)?1:0);

		/* Adjust DAYS and Y to match the guessed year.  */
		days -= ((yg - y) * 365
			+ LEAPS_THRU_END_OF (yg - 1)
			- LEAPS_THRU_END_OF (y - 1));
		y = yg;
	}
	psDateTime->m_sDate.m_nYear_13 = y;
	ip = __mon_yday[__isleap(y)];
	for (y = 11; days < (long int) ip[y]; --y)
		continue;
	days -= ip[y];
	psDateTime->m_sDate.m_nMonth_8 = y + 1;
	psDateTime->m_sDate.m_nDay_8 = days + 1;

	return CSUDI_SUCCESS;
}

/*-----This code is copied from linux udftime.c--------------*/


#define TIMER_LOCK(handle)		{CSUDIOSMutexWait(handle, 2000);}
#define TIMER_UNLOCK(handle)	{CSUDIOSMutexRelease(handle);}

static CSUDI_HANDLE s_hClockMutex = CSUDI_NULL;

static CSUDIOSTimeVal_S s_tBaseTime_S = {0,0,0};
static CSUDIPLUS_TIME_T s_tBaseTime_t = 0;
		
static CSUDI_BOOL CSUDIPlusOSClockInit(void)
{
	CSUDI_BOOL bRet = CSUDI_FALSE;
	
	if(s_hClockMutex != CSUDI_NULL)
	{
		return CSUDI_TRUE;
	}
	else
	{
		CSUDI_Error_Code tError;
		
		tError = CSUDIOSMutexCreate("UdiPlusClock",0,&s_hClockMutex);
		CSASSERT(tError == CSUDI_SUCCESS);
		bRet = (tError == CSUDI_SUCCESS);
	}

	return bRet;
}

CSUDI_Error_Code CSUDIPlusOSClockSet(CSUDIPLUS_TIME_T nSeconds)
{
	CSUDI_Error_Code tError = CSUDI_FAILURE;
	
	if(!CSUDIPlusOSClockInit())
	{
		return CSUDI_FAILURE;
	}

	TIMER_LOCK(s_hClockMutex);
	
	if(CSUDI_SUCCESS == CSUDIOSGetTime(&s_tBaseTime_S))
	{
		s_tBaseTime_t = nSeconds;	
		tError = CSUDI_SUCCESS;
	}
	else
	{
		memset(&s_tBaseTime_S,0,sizeof(s_tBaseTime_S));
		CSASSERT(0);
	}

	TIMER_UNLOCK(s_hClockMutex);
	
	return tError;
}

CSUDI_Error_Code CSUDIPlusOSClockGet(CSUDIPLUS_TIME_T* pnSeconds)
{
	CSUDI_Error_Code tError = CSUDI_SUCCESS;
	CSUDIOSTimeVal_S s_tTimeNow_S;
	
	if(!CSUDIPlusOSClockInit())
	{
		return CSUDI_FAILURE;
	}

	if(pnSeconds == CSUDI_NULL)
	{
		return CSUDIOSPLUS_ERROR_NO_MEMORY;
	}

	TIMER_LOCK(s_hClockMutex);

	memset(&s_tTimeNow_S,0,sizeof(s_tTimeNow_S));	
	if(CSUDI_SUCCESS == CSUDIOSGetTime(&s_tTimeNow_S))
	{
		int nDiff = s_tTimeNow_S.m_nSecond-s_tBaseTime_S.m_nSecond;
		CSASSERT(nDiff >= 0);
		*pnSeconds = s_tBaseTime_t + nDiff;
		tError = CSUDI_SUCCESS;
	}

	TIMER_UNLOCK(s_hClockMutex);
	
	return tError;
}

unsigned int CSUDIPlusOSGetTickCount(void)
{
	CSUDIOSTimeVal_S tTime;
	
	if(CSUDI_SUCCESS == CSUDIOSGetTime(&tTime))
	{
		return ((tTime.m_nSecond * 1000) + tTime.m_nMiliSecond);
	}
	else
	{
		return -1;
	}
}

