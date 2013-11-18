#ifndef SHELL_NVOD_H
#define SHELL_NVOD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"

#define NVODMGR_MAX_DELIVER_COUNT 8

/**
* nvod初始化配置参数
*/
typedef struct _CSNvodParam_S
{
	/**
	*NVOD搜索频点个数
	*/
	int         m_nDeliverCount;

	/**
	* nvod搜索的各个频点
	*/
	Deliver    m_stDeliver[NVODMGR_MAX_DELIVER_COUNT];

	/**存储tshiftEvent数据的天数*/
	int 		m_nEventDurationDays;

	/**是否强制指定依赖于数据管理搜索*/
	BOOL 	m_bSearchDependDM;

	/**"表示NIT表中是否存在Servcie_List_Descriptor描述符。*/
	BOOL 	m_bSearchNitList;

	/**搜索超时时间，单位为ms*/
	int 		m_nSearchTimeOutMs;

	/**
		* "true"表示前端把TimeShiftEvent数据在TableID 等于0x4e或0x4f的表中发送。 
		* "false"表示前端把TimeShiftEvent数据在TableID 等于0x50-0x5f或0x60-0x6f的表中发送。
	**/
	BOOL 	m_bIsScheduleToSimple;

	/**表示NIT表中是否存在Servcie_List_Descriptor描述符。*/
	BOOL 	m_bIsSearchScheduleEIT;

	/**默认频点是否为NVOD频点
	 *此参数在参数tuneParams存在时有用。
	 *等于true，表示参数tuneParams中的频点包含NVOD频点，则在下面的流程中不搜索NVOD频点。
	 *等于false，表示用户不知道参数tuneParams中的频点是否包含NVOD频点，则在下面的流程中搜索NVOD频点。
	 **/
	BOOL 	m_bIsNVODFreq;

	/**
	 * 是否搜索EIT OTHER表,
	 * 对于NVOD频点不发EIT OTHER表的情况（即只发当前频点NVOD信息）,应该将此设为false
	**/
	BOOL 	m_bIsFindOther;

	/**每次NVOD搜索过后要不要删除老的数据
	 * 如果有多个NVOD频点，且每个频点都只发自己频点的NVOD信息，且应该将下面这句设为false
	 * 否则会导致已经搜索到的其它频点的数据被删除，
	  *也就是说只有每个NVOD频点都将其它NVOD频点信息发全时才设为true
	**/
	BOOL 	m_bDelOldDataInSearch;

	/**
	* 要不要显示没有时移事件的参考事件
	*/
	BOOL 	m_bDisplayEmptyRefEvt;

	/**
	* 时区偏移(单位为小时)
	*/
	int 		m_nTimeZoneHour;

	/**
	* 用于nvod存储节目的内存起始地址
	*/
	BYTE*	m_pMem;

	/**
	* nvod内存大小
	*/
	DWORD	m_nMemLen;

}CSNvodParam_S;

/**
* 获取默认nvod配置参数
*/
void CSSHELLNvodGetDefaultParam(CSNvodParam_S* pOutParam);

/**
* 初始化nvod模块，
* 返回初始化成功后的NVODInterface.h中定义的nvod句柄
* 项目中可以使用该句柄调用NVODExtendInterface.h的接口注册并解析扩展数据。
*/
HCSHANDLE CSSHELLNvodInit(CSNvodParam_S* sParam);


#ifdef __cplusplus
}
#endif

#endif  /* SHELL_SUBTITLE_H */

