#ifndef _UDI_TYPEDEF_H_
#define _UDI_TYPEDEF_H_

#include "udiplus_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief 需添加注释*/
typedef struct _TDATE
{
	unsigned int m_dwYear_13         :13;	/**< Year*/
	unsigned int m_dwMonth_8         :8;	/**< Month*/
	unsigned int m_dwDay_8           :8;		/**< Day*/
	unsigned int m_dwWeek_3          :3;	/**< Week*/
}TDATE,*PTDATE;

/**@brief 需添加注释*/
typedef struct _TTIME
{
	unsigned int m_dwHour_8         :8;		/**< Hours*/
	unsigned int m_dwMinute_8       :8;		/**< Minutes*/
	unsigned int m_dwSecond_8       :8;	/**< Seconds*/
	unsigned int m_dwMilSecond_8    :8;	/**< 毫秒,此处定义不合理，1s=1000ms,用八位表示是不够的*/
}TTIME,*PTTIME;

/**@brief 需添加注释*/
typedef struct _TSYSTEMTIME
{
	TDATE m_TDATE;				/**< 系统日期*/
	TTIME m_TTIME;				/**< 系统时间*/
}TSYSTEMTIME,*PTSYSTEMTIME;


#ifdef __cplusplus
}
#endif

#endif
