#ifndef CSSHELL_NVOD_H
#define CSSHELL_NVOD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"
#include "CSUSPDMCommonType.h"
#include "NVODInterface.h"

#define CSSHELL_NVOD_MAX_DELIVER_COUNT 8

/**
* nvod初始化配置参数
*/
typedef struct _CSShellNvodParam_S
{
	/**
	* 用于nvod存储节目的内存起始地址
	*/
	BYTE*	m_pvMemAddr;

	/**
	* nvod内存大小
	*/
	int		m_nMemSize;
	/**
	*NVOD搜索频点个数
	*/
	int     m_nDeliverCount;

	/**
	* nvod搜索的各个频点
	*/
	Deliver  m_sDeliver[CSSHELL_NVOD_MAX_DELIVER_COUNT];

	/**存储tshiftEvent数据的天数*/
	int 	m_nEventDurationDays;

	/**是否强制指定依赖于数据管理搜索,注意：当为true时是不依赖于DM,false为依赖于DM*/
	BOOL 	m_bSearchDependDM;

	/**"表示NIT表中是否存在Servcie_List_Descriptor描述符。*/
	BOOL 	m_bSearchNitList;

	/**搜索超时时间，单位为ms*/
	int 	m_nSearchTimeout;

	/**
		* "true"表示前端把TimeShiftEvent数据在TableID 等于0x4e或0x4f的表中发送。 
		* "false"表示前端把TimeShiftEvent数据在TableID 等于0x50-0x5f或0x60-0x6f的表中发送。
	**/
	BOOL 	m_bScheduleToSimple;

	/**表示Schedule EIT表中是否存在Servcie_List_Descriptor描述符。*/
	BOOL 	m_bSearchScheduleEIT;

	/**默认频点是否为NVOD频点
	 *此参数在参数tuneParams存在时有用。
	 *等于true，表示参数tuneParams中的频点包含NVOD频点，则在下面的流程中不搜索NVOD频点。
	 *等于false，表示用户不知道参数tuneParams中的频点是否包含NVOD频点，则在下面的流程中搜索NVOD频点。
	 **/
	BOOL 	m_bIsNvodFreq;

	/**
	 * 是否搜索EIT OTHER表,
	 * 对于NVOD频点不发EIT OTHER表的情况（即只发当前频点NVOD信息）,应该将此设为false
	**/
	BOOL 	m_bSearchEitOther;

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

	NVODExt 	m_fnParseData;
}CSShellNvodInitParam_S;

/**
@brief 设置Nvod初始化参数
@param [in] psNvod, 初始化配置参数
@return 成功返回TRUE,否则返回FALSE
*/
BOOL CSShellNvodSetParam(const CSShellNvodInitParam_S* psNvod);

/**
@brief 获取初始化过的Nvod句柄
@param 无
@return 返回nvod句柄。如果nvod未初始化成功，则返回NULL
*/
HCSHANDLE CSShellNvodGetHandle();

/**
@brief 用于进入NVOD界面时启动NVOD更新机制等
@return 成功返回TRUE,失败返回FALSE
@note 与CSShellNvodExit_F 函数配对使用必须被调用
*/
typedef BOOL (*CSShellNvodEnter_F)(void);

/**
@brief 用于退出NVOD界面时同时退出NVOD状态
@return 成功返回TRUE,失败返回FALSE
@note 与CSShellNvodEnter_F 函数配对使用必须被调用
*/
typedef BOOL (*CSShellNvodExit_F)(void);

/**
@brief 获取分组的个数
@return 分组的个数
@note 
*/
typedef int (*CSShellNvodGetGroupCount_F)(void);

/**
@brief 获取指定分组的名称
@param [in] nGroupIndex, 分组的索引
@param [out] pcGroupName, 指定索引的分组的名称字符串,必须返回unicode码
@param [in] nLength, 输出参数pcGroupName的长度
@return 成功返回TRUE,失败返回FALSE
@note 
*/
typedef BOOL (*CSShellNvodGetGroupName_F)(int nGroupIndex, char* pcGroupName, int nLength);

/**
@brief 获取指定分组的搜索到的参考事件的个数
@param [in] nGroupIndex, 分组的索引
@return 指定分组的搜索到的参考事件的个数
@note 
*/
typedef int (*CSShellNvodGetRefEventCount_F)(int nGroupIndex);

/**
@brief 获取指定分组的搜索到的参考事件的信息
@param [in] nGroupIndex, 分组的索引
@param [in] nRefEventIndex, 参考事件的索引
@param [out] pstRefEvent,  参考事件的信息
@return 成功返回TRUE,失败返回FALSE
@note 
*/
typedef BOOL (*CSShellNvodGetRefEventParam_F)(int nGroupIndex, int nRefEventIndex, ReferenceEvent *pstRefEvent);

/**
@brief 获取指定分组所在参考事件的时移事件的个数
@param [in] nGroupIndex, 分组的索引
@param [in] nRefEventIndex, 参考事件的索引
@return 指定分组所在参考事件的时移事件的个数
@note 
*/
typedef int (*CSShellNvodGetTimeShiftEventCount_F)(int nGroupIndex, int nRefEventIndex);

/**
@brief 获取指定的时移事件的信息
@param [in] nGroupIndex, 分组的索引
@param [in] nRefEventIndex, 参考事件的索引
@param [in] nTimeshiftEventIndex, 时移事件的索引
@param [out] pstTimeShiftEvent,  时移事件的信息
@return 成功返回TRUE,失败返回FALSE
@note 
*/
typedef BOOL (*CSShellNvodGetTimeShiftEventParam_F)(int nGroupIndex, int nRefEventIndex, int nTimeshiftEventIndex, TimeShiftedEvent *pstTimeShiftEvent);


/**
本结构定义了获取DM中的信息时用到的信息。
，最后可获取CSShellDMFunc_S
*/
typedef struct _CSShellNvodFunc_S
{
    int                                 m_nSize;            ///< 本结构体的大小，必须赋值为sizeof(CSShellNvodFunc_S)
    CSShellNvodEnter_F           m_fnEnter; ///< 进入NVOD界面时启动NVOD更新
    CSShellNvodExit_F           m_fnExit; ///< 退出NVOD界面时同时退出NVOD状态
    CSShellNvodGetGroupCount_F           m_fnGetGroupCount; ///< 获取分组的个数
    CSShellNvodGetGroupName_F           m_fnGetGroupName; ///< 获取指定分组的名称
    CSShellNvodGetRefEventCount_F           m_fnGetRefEventCount; ///< 获取指定分组的搜索到的参考事件的个数
    CSShellNvodGetRefEventParam_F           m_fnGetRefEventParam; ///< 获取指定分组的搜索到的参考事件的信息
    CSShellNvodGetTimeShiftEventCount_F           m_fnGetTimeShiftEventCount; ///< 获取指定分组所在参考事件的时移事件的个数
    CSShellNvodGetTimeShiftEventParam_F           m_fnGetTimeShiftEventParam; ///< 获取指定的时移事件的信息
}CSShellNvodFunc_S;

/**
@brief 设置NVOD相关操作接口给MMCP
@param[in] psNvodFunc  NVOD信息及相关操作结构体实例
@return 成功返回TRUE,失败返回FALSE
@code
//绝大多是情况下按此方式调用此接口即可。
CSShellNvodSetFunc(CSShellNvodGetDefaulFunc());
@endcode
*/
BOOL CSShellNvodSetFunc(const CSShellNvodFunc_S* psNvodFunc);

/**
@brief 获取NVOD默认实现参数
@return MMCP提供NVOD Shell接口的默认实现
@note 本接口实际上实现了典型项目的配置信息，项目若不需要修改则可以直接将此典型实现设置给MMCP Shell模块
@note MMCP内部(dtvm native或jsext)不得直接使用本接口，若MMCP需要获得NVOD的对象须使用csshell_nvod_private.h中的CSShellNvodGetFunc接口
@see CSShellNvodGetFunc
*/
const CSShellNvodFunc_S* CSShellNvodGetDefaulFunc(void);

#ifdef __cplusplus
}
#endif

#endif  /* SHELL_SUBTITLE_H */

