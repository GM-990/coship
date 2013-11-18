/**@defgroup CSCLOCKTIMER clock and time ϵͳʱ�估��ص�ת������ʽ���ӿ�
@brief ϵͳʱ�估��ص�ת������ʽ���ӿ�

@brief ��ģ��ӿ���ο�<time.h>�еĶ���
@version 0.1 2008/11/4 �޶��汾
@version 0.1 2008/10/21 ��ʼ�汾
@{
*/

#ifndef _UDI_CLOCKTIMER_H_
#define _UDI_CLOCKTIMER_H_

#include "udi_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef long cstime_t;
typedef int cs_clockid_t;
typedef unsigned int cs_size_t;

/**@brief �൱��time.h��CLOCK_REALTIME*/
#define CS_CLOCK_REALTIME	(cs_clockid_t)0

/**@brief �൱��time.h��CLOCK_MONOTONIC*/
#define CS_CLOCK_MONOTONIC	(cs_clockid_t)1


/**@brief cs_tm_s����*/
typedef struct cs_tm 
{
	int    m_tm_sec;   /**< seconds [0,61] */
	int    m_tm_min;   /**< minutes [0,59] */
	int    m_tm_hour;  /**< hour [0,23] */
	int    m_tm_mday;  /**< day of month [1,31] */
	int    m_tm_mon;   /**< month of year [0,11] */
	int    m_tm_year;  /**< years since 1900 */
	int    m_tm_wday;  /**< day of week [0,6] (Sunday = 0) */
	int    m_tm_yday;  /**< day of year [0,365] */
	int    m_tm_isdst; /**< daylight savings flag (-1/0/1) */
}cs_tm_s;

/**@brief cs_timespec_s����*/
typedef struct cs_timespec 
{
	long int  m_tv_sec;    /**< seconds */
	long int  m_tv_nsec;   /**< nanoseconds */
}cs_timespec_s;

/**
@brief Convert tm structure to string

Interprets the contents of the tm structure pointed by timeptr as a calendar time and converts it to a C string containing a human-readable version of the corresponding date and time.
@param[in] pstTime Pointer to a tm structure that contains a calendar time broken down into its components (see tm). 
@return A C string containing the date and time information in a human-readable format.
@note ת������ַ�����ʽΪ:Www Mmm dd hh:mm:ss yyyy,�൱��time.h�е�asctime
*/
char* CSAsctime(const cs_tm_s *  pstTime);

/**
@brief Convert time_t value to string

Converts the time_t object pointed by timer to a C string containing a human-readable version of the corresponding local time and date.
�൱��time.h�е�ctime
@param[in] npTime Pointer to a time_t object that contains a calendar time. 
@return A C string containing the date and time information in a human-readable format.
@note calendar time�ǰ���������Ϣ(���������ڼ�ʱ�������Ϣ)��ʱ��,����洢��ʽ������ʵ��(���²������ظ�����)
�������ĵ��ý���ȼ���CSAsctime(CSLocaltime(const cstime_t * npTime))�����ؽṹΪ����ʱ��
*/
char* CSCtime(const cstime_t * npTime);

/**
@brief Calculates the difference in seconds between time1 and time2.

�൱��time.h�е�difftime
��nTime2-nTime1
@param[in] nTime2 time_t object representing the latter of the two times. 
@param[in] nTime1 time_t object representing the earlier of the two times. 
@return The difference in seconds (time2-time1) as a floating point double.
*/
double CSDifftime(cstime_t nTime2, cstime_t nTime1);

/**
@brief Convert time_t to tm as UTC time

Uses the value pointed by timer to fill a tm structure with the values that represent the corresponding time, expressed as UTC (or GMT timezone).
�൱��time.h�е�gmtime
@param[in] npTime Pointer to a time_t value representing a calendar time (see time_t). 
@return A pointer to a tm structure with the time information filled in.
*/
cs_tm_s* CSGmtime(const cstime_t * npTime);

/**
@brief  Convert time_t to tm as local time

Uses the time pointed by timer to fill a tm structure with the values that represent the corresponding local time.
�൱��time.h�е�localtime
@param[in] npTime Pointer to a time_t value representing a calendar time (see time_t). 
@return A pointer to a tm structure with the time information filled in.
*/
cs_tm_s *CSLocaltime(const cstime_t * npTime);

/**
@brief Convert tm structure to time_t

Interprets the contents of the tm structure pointed by timeptr as a calendar time expressed in local time.
�൱��time.h�е�mktime
@param[in] pstTime Pointer to a tm structure that contains a calendar time broken down into its components (see tm). 
@return   A time_t value corresponding to the calendar time passed as argument.
On error, a -1 value is returned. 
*/
cstime_t CSMktime(cs_tm_s * pstTime);

/**
@brief Format time to string

Copies into ptr the content of format, expanding its format tags into the corresponding values as specified by timeptr, with a limit of maxsize characters.
�൱��time.h�е�strftime
@param[out] DesStr Pointer to the destination array where the resulting C string is copied. 
@param[in] MaxSize Maximum number of characters to be copied to ptr. 
@param[in] fmt C string containing any combination of regular characters and special format specifiers. These format specifiers are replaced by the function to the corresponding values to represent the time specified in timeptr. pls learn more in time.h.
@param[in] nstTime Pointer to a tm structure that contains a calendar time broken down into its components (see tm). 
@return If the resulting C string fits in less than maxsize characters including the terminating null-character, the total number of characters copied to ptr (not including the terminating null-character) is returned.
Otherwise, zero is returned and the contents of the array are indeterminate.
*/
cs_size_t CSStrftime(char * DesStr, cs_size_t MaxSize, const char * fmt, const cs_tm_s * nstTime );

/**
@brief Get current time

Get the current calendar time as a time_t object.
�൱��time.h�е�time
@param[out] npTime Pointer to an object of type time_t, where the time value is stored.
@return The current calendar time as a time_t object.
If the argument is not a null pointer, the return value is the same as the one stored in the location pointed by the argument.
If the function could not retrieve the calendar time, it returns a -1 value.
@note nAlternativelly, this parameter can be a null pointer, in which case the parameter is not used, but the a time_t object is still returned by the function. 
*/
cstime_t CSTime(cstime_t * npTime);

/**
@brief The tzset() function shall use the value of the environment variable TZ to set time conversion information used by ctime() , localtime() , mktime() , and strftime(). 
If TZ is absent from the environment, implementation-defined default timezone information shall be used.

*/
void  CSTzset(void);


/**
@brief ���ָ��ʱ�ӵ�ʱ��

�൱��time.h�е�clock_gettime
@param[in] clock_id ָ����ʱ��ID��Ŀǰ��֧��CS_CLOCK_REALTIME
@param[out] pTime �洢�õ���ʱ�䣬��1970��1��1��0ʱ0��0�������������ʾ
@return �ɹ�����0,������-1,����errnoָʾ�����Ĵ���
*/
int CSClockGettime(cs_clockid_t clock_id,cs_timespec_s * pTime);

/**
@brief �趨ָ��ʱ�ӵ�ʱ��

�൱��time.h�е�clock_settime
@param[in] clock_id ָ����ʱ��id��Ŀǰ��֧��CS_CLOCK_REALTIME
@param[in] pTime  ָ���趨��ʱ��ֵ����1970��1��1��0ʱ0��0�������������ʾ
@return �ɹ�����0,������-1,����errnoָʾ�����Ĵ���
*/
int CSClockSettime(cs_clockid_t clock_id,cs_timespec_s * pTime);

/**
*/
int CSClockGetres(cs_clockid_t clock_id,cs_timespec_s *tp);


#ifdef __cplusplus
}
#endif

/** @} */

#endif



