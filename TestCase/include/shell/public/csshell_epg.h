
#ifndef CSSHELL_EPG_H
#define CSSHELL_EPG_H

#ifdef __cplusplus
extern "C" {
#endif
#include "mmcp_typedef.h"
#include "EPGReceiverInterface.h"
#include "SectionInterface2C.h"
#include "MemoryPoolInterface.h"

#define CSSHELL_EPG_MAX_TAG_COUNT 8

/**@brief 分类显示EPG的分类方式*/
typedef enum _CSShellEPGLevelType
{
	EPGTYPE_NONE,            //不含分类功能
	EPGTYPE_LEVEL1_PRESENT,  //对当前信息进行分类
	EPGTYPE_LEVEL1_SCHEDULE  //对所有信息进行分类
}CSShellEPGLevelType_E;

typedef int (*CSShellEpgExtProc_F)(EPGCallbackType emType, void *pParam);

typedef void (*CSShellEpgPriInit_F)(HCSHANDLE hEpgHandle);

/**@brief EPG初始化参数 */
typedef struct _CSShellEpgInitParam
{
	int                    m_nSize;                    ///< 本结构体的大小，必须赋值为sizeof(CSShellEpgInitParam_S)
	void*                  m_pvUserData;                            ///< 用户数据，此参数将随着本结构中的函数指针一起传递给函数实现

	//EPG数据区的地址和大小
	PBYTE        m_pbMemAddr;
	int          m_nMemSize;

	//简要/一周EPG的搜索/保存策略
	int          m_nPFRecvPolicy;
	int          m_nPFSavePolicy;
	int          m_nScheduleRecvPolicy;
	int          m_nScheduleSavePolicy;

	//要处理的回调消息设置
	int 		 m_nRegCallbackType;

	//搜索超时设置
	int          m_nTimeOut;

	//节目描述信息来源
	int          m_nDescriptionFrom;	
	//pf event的节目简介的搜索和保存策略
	int          m_nPFDescriptionPolicy;
	//schedule event节目简介的搜索和保存策略
	int          m_nScheduleDescriptionPolicy;

	//过期是否显示
	BOOL         m_bPastShown;

	//分类显示方式
	CSShellEPGLevelType_E          m_eTypeMode;

	//分类显示所需缓冲区
	PBYTE        m_pbTypeMemAddr;
	int          m_nTypeMemSize;

	//分类时间范围:从当前时间起XXmin.
	int          m_nTypeDuraMin;
	//时区
	float 		m_fTimezone;

    //EPG扩展功能支持,该回调属于EPG的间接回调。直接回调在dtvm native中实现
	CSShellEpgExtProc_F m_fnExpProc;

	//P/F PEG需保存的描述符信息
	int				m_nSimpleTagCount;                         ///< 需保存的描述符个数
	BYTE			m_aucSimpleTags[CSSHELL_EPG_MAX_TAG_COUNT]; ///< 需保存的描述符Tags

	//Schedule PEG需保存的描述符信息
	int				m_nScheduleTagCount;                         ///< 需保存的描述符个数
	BYTE			m_aucScheduleTags[CSSHELL_EPG_MAX_TAG_COUNT]; ///< 需保存的描述符Tags
	CSShellEpgPriInit_F m_fnPrivateInit;		///<该回调在EPG初始化时被调用，使应用可以设置一些私有的需求

	//是否取得原始节目码表类型标志位
    BOOL        m_bGetOriginalLanguage;

}CSShellEpgInitParam_S;

/*********************设置和获取pf epg *******************************/
/**
@brief 设置EPG初始化参数
@param psEpg, 初始化参数
@return 成功返回FALSE，失败返回TRUE
*/
BOOL CSShellEpgSetPFParam(const CSShellEpgInitParam_S* psEpg);

/**
@brief 获取初始化过的epg句柄
@param 无
@return 返回epg句柄。如果epg未初始化，则返回NULL
*/
HCSHANDLE CSShellEpgGetPFHandle();
/*********************end  pf epg *************************************/


/*********************设置和获取Schedule epg *******************************/
/**
@brief 设置EPG初始化参数
@param psEpg, 初始化参数
@return 成功返回FALSE，失败返回TRUE
*/
BOOL CSShellEpgSetScheduleParam(const CSShellEpgInitParam_S* psEpg);

/**
@brief 获取初始化过的epg句柄
@param 无
@return 返回epg句柄。如果epg未初始化，则返回NULL
*/
HCSHANDLE CSShellEpgGetScheduleHandle();
/*********************end  Schedule epg *************************************/

/**
@brief 获取schedule sort epg 句柄
@param 无
@return schedule sort epg 句柄
*/
HCSHANDLE CSShellEpgGetScheduelSortHandle();

/*********************end  pf epg *************************************/

/**
@brief 获取p sort epg 句柄
@param 无
@return p sort epg 句柄
*/
HCSHANDLE CSShellEpgGetPSortHandle();


/**
@brief 获取f sort epg 句柄
@param 无
@return f sort epg 句柄
*/
HCSHANDLE CSShellEpgGetFSortHandle();

#ifdef __cplusplus
}
#endif

#endif  /* SHELL_EITLOADER_H */




