#ifndef __CS_UTILS_H__
#define __CS_UTILS_H__

#include "mmcp_typedef.h"
#include "mmcp_os.h"
#include "cs_rsa.h"

/*! make a 32-bits value with two words. */
#define MKINT32(v1, v2) (int)(((v1) << 0x10) | (v2))

/*! get high word of a 32-bits value. */
#define HIWORD(v) (signed short)((v) >> 0x10 & 0xffff)

/*! get low word of a 32-bits value. */ 
#define LOWORD(v) (signed short)((v) & 0xffff)

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * UTC+MJD time format.
   */
#define CSLOCATOR_LENGTH        (1024)

	typedef struct _CSUtcTime
	{
		WORD mjd_date;
		char  hour;
		char  minute;
		char  second;
	} CSUtcTime;

	typedef enum CSLocatorType_E
	{
		CSLOCATOR_UNSUPPORT = 0,	    /* UNSUPPORT */
		CSLOCATOR_DVB,	    /* DVB */
		CSLOCATOR_FILE,  	/* FILE */
		CSLOCATOR_IGMP,     /*IP ×é²¥*/
		CSLOCATOR_RTSP      /*IP µã²¥*/
	}CSLocatorType_E;

	typedef struct
	{
		char  *pszIP;
		WORD   wPort;
		char  *pszFileName;      
	}CSLocatorIPInfo_S;

	typedef struct
	{
		DWORD  dwServiceID;
		DWORD  dwTsID;
		DWORD  dwOriNetworkID;
	}CSLocatorDVBInfo_S;

	typedef struct
	{
		char   *pszFileName;      
	}CSLocatorFILEInfo_S;

	typedef struct
	{
		int  nDataSourceType;  
		CSLocatorIPInfo_S  sIPInfo;
		CSLocatorDVBInfo_S sDVBInfo;
		CSLocatorFILEInfo_S sFILEInfo; 
	}CSLocatorInfo_S;

	/**
	* converts a int16 from TS byte order to host byte order.
	*/
	signed short
	MMCPttohs (signed short ts_int16);

	/**
	* converts a int32 from TS byte order to host byte order.
	*/
	int
	MMCPttohl (int ts_int32);

	/**
	* Translate the time in UTC+MJD to computer representation.
	*/
	CSUDIPLUS_TIME_T MMCPTranslateUtcTime2SysTime (char time[5]);

	/**
	* Translate the sys time to Dvb Time UTC+MJD 
	*/

	BOOL MMCPTranslateSysTime2UtcTime (const CSUDIPLUS_TIME_T time, CSUtcTime *psUtcTime);

	/**
	* Converts the BCD code to decimal number.
	*/
	int
	MMCPBCD2Decimal (char bcd[3]);

	void
	MMCPHexExport (void *buffer, int size, char *prefix);


#ifdef __cplusplus
}
#endif

#endif /* __MMCP_UTILS_H__ */

