/**@defgroup PublicShellTF3CA PublicShellTF3CA 定义同方3 CA对外接口

@version 2.0.0 2009/12/16 初稿
@{
*/
#ifndef CSSHELL_UDIADAPTER_PRIVATE_H
#define CSSHELL_UDIADAPTER_PRIVATE_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
在udi1.0中，时间的结构为TTIME，在udi2.0中，改为CSUDIPLUSOSTIME_S。
CSUDIPLUSOSTIME_S与TTIME是不兼容的，二者中成员变量的位段分配不一致。
mmcp中间件为udi1.0和udi2.0做不同的发布。ca只发布一套库。
为了保持向前兼容，ca中时间结构仍然使用TTIME。ca的依赖接口也保持不变。中间件自己对此适配。
*/
//#ifdef MMCP_UDI2
	typedef struct _TTIME
	{
		unsigned int m_dwHour_8 		:8; 	/**< Hours*/
		unsigned int m_dwMinute_8		:8; 	/**< Minutes*/
		unsigned int m_dwSecond_8		:8; /**< Seconds*/
		unsigned int m_dwMilSecond_8	:8; /**< 毫秒,此处定义不合理，1s=1000ms,用八位表示是不够的*/
	}TTIME;
	
	typedef struct _TDATE
	{
		   unsigned int m_dwYear_13 		:13;   /**< Year*/
		   unsigned int m_dwMonth_8 		:8;    /**< Month*/
		   unsigned int m_dwDay_8			:8; 		   /**< Day*/
		   unsigned int m_dwWeek_3			:3;    /**< Week*/
	}TDATE;
//#endif



#ifdef __cplusplus
}
#endif
/** @} */
#endif


