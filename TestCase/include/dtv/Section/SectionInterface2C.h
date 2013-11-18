#ifndef SectionInterface2C_H
#define SectionInterface2C_H

#ifdef PRAGMA
#pragma interface "SectionInterface2C.h"
#endif

/* {{{USR

   }}}USR */

#include "mmcp_typedef.h"
#ifdef MMCP_UDI2
#include "udi2_demux.h"
#include "udi2_section.h"
#else
#include "udi_demux.h"
#endif

// {{{USR
#ifdef __cplusplus
extern "C" 
{
#endif

//#define _SRC_INC_
#ifndef _SRC_INC_

//section回调的类型
typedef enum  
{   
    SECTION_AVAIL,				// 有Section到来   
    SECTION_TIMEOUT,			// 超时    
    SECTION_COMPLETE,			// 收到所有Section    对于表模式，需要将pSection转换为SectionTableData_S类型
    SECTION_REPEATED,			// Section包重复    
    SECTION_VERSION_CHANGED 
}SectionEventType;

typedef enum
{
	SECTION_PSI_CONTINUE = 0,
	SECTION_PES = 1,
	SECTION_PSI_TABLE = 2,
	SECTION_PSI_SIMPLE= 3
}SectionRequestType;

//section的数据结构
typedef struct 
{
	DWORD m_dwDemuxID;
	DWORD m_dwPID;
	BYTE * m_pucDataBuf;
	int m_nDataBufLen;
	int m_nDataLen;
} Section;

#ifdef MMCP_UDI2
/**@brief 数据请求的优先级，高优先级的section会比低优先级的section提前分发，在Demux模块中无效*/
typedef enum  
{
    SECTION_PRIORITY_HIGH,		/**< 高优先级*/
    SECTION_PRIORITY_MIDDLE,	/**< 中优先级*/  
    SECTION_PRIORITY_LOW,		/**< 低优先级*/
    NUM_SECTION_PRIORITIES		/**< 必须在枚举的最后一位，表示Section优先级的个数，不可当优先级用*/
} SectionPriority ;

/**@brief Filter信息，该信息描述所要请求的数据的特征*/
typedef struct _RequestInfo_S
{	    
	unsigned int dwPID;		        /**< 请求数据的pid*/
	unsigned int dwDemuxID;		    /**< 用于请求数据的Demux设备句柄*/
	long lTimeout;		        /**< 超时时间，超过这个时间就会发送超时消息，对于Demux接口无效*/
	void* pvAppData;		    /**< 用户数据，当返回Section时，该数据也会原封不动的返回给用户，对于Demux接口无效*/	
	CSUDI_BOOL bCRCCheck;		        /**< 是否进行CRC检查*/
	SectionPriority enmPriority;/**< 该请求的优先级 ，对于Demux接口无效*/
	CSUDIFILTERMatchMask_S MMFilter;		    /**< Filter 信息*/
	int m_nPES;					/**< SectionRequestType中定义的数值，对于Demux接口无效*/
	int m_nTaskIndex;           /**< 请求所在线程，对于Demux接口无效*/
} RequestInfo;
#endif

typedef RequestInfo RequestInfoEx;

typedef struct 
{
	int m_nPriorityIndex;
	int m_nQueueSize;
}SectionQueuePar;

typedef struct 
{
	int m_nTaskPriorityIndex;
	int m_nTaskPriority;
	int m_nStackSize;
	int m_nOneBufSize;
	int m_nSectionQueueParCount;
	SectionQueuePar * m_pSectionQueuePar;
}TaskInitPar;


//wanglin add
typedef struct///新增
{
	Section *m_psSections;
	WORD m_wSectionCount; //一共多少个section
	BYTE m_bVersion;	
}SectionTableData_S;

//typedef void (*SectionTableCallback_F)(SectionEventType eEvent, SectionTableData_S* psTable,void* pUserData, HCSHANDLE hRequest);
//end add

#endif

#define CS_DTV_SECTION "dtv_section"

/******************************************************************************************
 ******************************************************************************************/
/*
 *	得到section或超时的回调函数原型
 *
 *  event 回调类型，含义参见枚举描述
 *  pSection 包含section的结构体指针，若event是SECTION_TIMEOUT，则值为NULL
 *	pUserData，用户请求section时传递的用户数据。
 *	hRequest 和用户请求时返回的handle相同
 */
typedef void (*Section2CCallback)(SectionEventType event, Section* pSection,void* pUserData, HCSHANDLE hRequest);

#define Section2CSectionRequestEx Section2CSectionRequest


// }}}USR

/* {{{USR
**************************************************************************
* Function Name: CSUSPSectionSetDemuxCount
*   设置可用Demux的个数，以便于统计每个demux的filter的个数
*
*Parameters	
*	nDemuxCount
*		[in]系统中可用Demux的个数
*
*Return Values
*		成功返回TRUE，否则返回FALSE.
*Remarks
*		DemuxCount被用来获取每个demux的filter，以便统计出共有多少个filter。
*		此函数需要在Section2CCreateSectionSource...也就是初始化section模块之前调用
*		如果不调用此函数，初始化section模块时，默认为只有1个demux。
**************************************************************************
   }}}USR */
BOOL CSUSPSectionSetDemuxCount( int nDemuxCount );

/* {{{USR
**************************************************************************
* Function Name: CSUSPSectionSetFilterBufSize
*   设置porting的buffer大小
*
*Parameters	
*	hRequest
*		[in]请求section时返回的handle
*     nBufSize
*		[in]porting的buffer大小
*
*Return Values
*		成功返回TRUE，否则返回FALSE.
*Remarks
*		只有在filter处于stop状态时才能设置buffer大小，否则返回FALSE
**************************************************************************
   }}}USR */
BOOL CSUSPSectionSetFilterBufSize(HCSHANDLE hRequest, int nBufSize);

/* {{{USR
**************************************************************************
* Function Name: CSUSPCreateSectionSource&CSUSPGetSectionSourceHandle
*   创建及返回SectionHandle
*
*Parameters	
*	pbSectionMemory
*		[in] 请求信息的指针.
*
*Return Values
*		成功返回SectionSource的句柄，否则返回NULL.
**************************************************************************
   }}}USR */
HCSHANDLE Section2CCreateSectionSource( PBYTE pbSectionMemory, int nMemorySize );

/* {{{USR

   }}}USR */
HCSHANDLE Section2CCreateSectionSourceEx( PBYTE pbSectionMemory, int nMemorySize, TaskInitPar * pTaskInitPar, int nTaskInitParCount );

/* {{{USR

   }}}USR */
HCSHANDLE Section2CCreateSectionSourceTimeOutTaskEx( PBYTE pbSectionMemory, int nMemorySize, TaskInitPar * pTaskInitPar, int nTaskInitParCount, int nTimeOutTaskPriority, int nTimeOutTaskStackSize );

/* {{{USR

   }}}USR */
HCSHANDLE Section2CGetSectionSourceHandle( void );

/* {{{USR
**************************************************************************
* Function Name: CSUSPSectionRequest&CSUSPSectionRequestEx
*   请求section
*
*Parameters	
*	pInfo
*		[in] 请求信息的指针.
*
*	pCallback
*		[in] 回调函数指针.
*
*Return Values
*		请求成功返回请求分配到的section句柄，否则返回NULL.
**************************************************************************
   }}}USR */
HCSHANDLE Section2CSectionRequest( RequestInfo * pInfo, Section2CCallback pCallback );

/* {{{USR

   }}}USR */
//HCSHANDLE Section2CSectionRequestEx( RequestInfoEx * pInfo, Section2CCallback pCallback );

/***************************************************************
通过hRequest获取RequestInfo, 返回的RequestInfo不能修改
example: const* RequestInfo *psReqInfo = Section2CSectionGetRequestInfo(...);
返回NULL表示失败
****************************************************************/
const RequestInfo* Section2CSectionGetRequestInfo(HCSHANDLE hRequest);
/* {{{USR
**************************************************************************
* Function Name: CSUSPSectionCancel
*   取消section请求
*
*Parameters	
*	hRequest
*		[in]请求section时返回的handle，也可以从回调的函数参数获得创建任务.
*
*Return Values
*		取消成功返回TRUE，否则返回FALSE.
**************************************************************************
   }}}USR */
BOOL Section2CSectionCancel( HCSHANDLE hRequest );

/* {{{USR

   }}}USR */
HCSHANDLE Section2CSectionAllocate( RequestInfoEx * pInfo, Section2CCallback pCallback );

/* {{{USR

   }}}USR */
BOOL Section2CSectionModify( HCSHANDLE hRequest, RequestInfoEx * pInfo );

/* {{{USR
**************************************************************************
* Function Name: CSUSPSectionStop
*   暂停section请求
*
*Parameters	
*	hRequest
*		[in]请求section时返回的handle，也可以从回调的函数参数获得创建任务.
*
*Return Values
*		停止成功返回TRUE，否则返回FALSE.
**************************************************************************
   }}}USR */
BOOL Section2CSectionStop( HCSHANDLE hRequest );

/* {{{USR
**************************************************************************
* Function Name: CSUSPSectionStart
*   重新开始section请求
*
*Parameters	
*	hRequest
*		[in]请求section时返回的handle，也可以从回调的函数参数获得创建任务.
*
*Return Values
*		启动成功返回TRUE，否则返回FALSE.
**************************************************************************
   }}}USR */
BOOL Section2CSectionStart(HCSHANDLE hRequest);

/* {{{USR
**************************************************************************
* Function Name: CSUSPSectionGetVersionInfo
*		获取模块版本信息
*
*Parameters	
*	pcVersionInfo
*		[out]存放版本信息的缓冲区首地址，输出参数,存放版本信息.
*	nSize
*		[in]缓冲区的长度
*
*Return Values
*	1)如果第一个参数pcVersionInfo为空，返回版本信息的实际长度。
*	建议用户输入第二个参数为：版本信息的实际长度+       
*	2)如果第一个参数pcVersionInfo不为空，且函数调用成功，返回版本信息的实际长度；
*	3)如果失败，返回0。
**************************************************************************
   }}}USR */
int CSUSPSectionGetVersionInfo(char * pcVersionInfo, int nSize );

/* {{{USR
**************************************************************************
* Function Name: CSUSPSectionGetMemInfo
*   获取Section的内存属性（最小内存和最大内存）
*
*Parameters	
*	pnMin
*		[out]模块所需最小内存 (in bytes)
*	pnMax
*		[out]模块所需最大内存(in bytes)
*
*Return Values
*		成功返回TRUE，否则返回FALSE.
**************************************************************************

   }}}USR */
BOOL CSUSPSectionGetMemInfo( int * pnMin, int * pnMax );


BOOL SectionTableFreeData(HCSHANDLE hRequest, SectionTableData_S *psData);

const SectionTableData_S * SectionTableGetData(HCSHANDLE hRequest);

// {{{USR
#ifdef __cplusplus
}
#endif
// }}}USR

#endif /* SectionInterface2C_H */
