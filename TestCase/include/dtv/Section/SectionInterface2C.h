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

//section�ص�������
typedef enum  
{   
    SECTION_AVAIL,				// ��Section����   
    SECTION_TIMEOUT,			// ��ʱ    
    SECTION_COMPLETE,			// �յ�����Section    ���ڱ�ģʽ����Ҫ��pSectionת��ΪSectionTableData_S����
    SECTION_REPEATED,			// Section���ظ�    
    SECTION_VERSION_CHANGED 
}SectionEventType;

typedef enum
{
	SECTION_PSI_CONTINUE = 0,
	SECTION_PES = 1,
	SECTION_PSI_TABLE = 2,
	SECTION_PSI_SIMPLE= 3
}SectionRequestType;

//section�����ݽṹ
typedef struct 
{
	DWORD m_dwDemuxID;
	DWORD m_dwPID;
	BYTE * m_pucDataBuf;
	int m_nDataBufLen;
	int m_nDataLen;
} Section;

#ifdef MMCP_UDI2
/**@brief ������������ȼ��������ȼ���section��ȵ����ȼ���section��ǰ�ַ�����Demuxģ������Ч*/
typedef enum  
{
    SECTION_PRIORITY_HIGH,		/**< �����ȼ�*/
    SECTION_PRIORITY_MIDDLE,	/**< �����ȼ�*/  
    SECTION_PRIORITY_LOW,		/**< �����ȼ�*/
    NUM_SECTION_PRIORITIES		/**< ������ö�ٵ����һλ����ʾSection���ȼ��ĸ��������ɵ����ȼ���*/
} SectionPriority ;

/**@brief Filter��Ϣ������Ϣ������Ҫ��������ݵ�����*/
typedef struct _RequestInfo_S
{	    
	unsigned int dwPID;		        /**< �������ݵ�pid*/
	unsigned int dwDemuxID;		    /**< �����������ݵ�Demux�豸���*/
	long lTimeout;		        /**< ��ʱʱ�䣬�������ʱ��ͻᷢ�ͳ�ʱ��Ϣ������Demux�ӿ���Ч*/
	void* pvAppData;		    /**< �û����ݣ�������Sectionʱ��������Ҳ��ԭ�ⲻ���ķ��ظ��û�������Demux�ӿ���Ч*/	
	CSUDI_BOOL bCRCCheck;		        /**< �Ƿ����CRC���*/
	SectionPriority enmPriority;/**< ����������ȼ� ������Demux�ӿ���Ч*/
	CSUDIFILTERMatchMask_S MMFilter;		    /**< Filter ��Ϣ*/
	int m_nPES;					/**< SectionRequestType�ж������ֵ������Demux�ӿ���Ч*/
	int m_nTaskIndex;           /**< ���������̣߳�����Demux�ӿ���Ч*/
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
typedef struct///����
{
	Section *m_psSections;
	WORD m_wSectionCount; //һ�����ٸ�section
	BYTE m_bVersion;	
}SectionTableData_S;

//typedef void (*SectionTableCallback_F)(SectionEventType eEvent, SectionTableData_S* psTable,void* pUserData, HCSHANDLE hRequest);
//end add

#endif

#define CS_DTV_SECTION "dtv_section"

/******************************************************************************************
 ******************************************************************************************/
/*
 *	�õ�section��ʱ�Ļص�����ԭ��
 *
 *  event �ص����ͣ�����μ�ö������
 *  pSection ����section�Ľṹ��ָ�룬��event��SECTION_TIMEOUT����ֵΪNULL
 *	pUserData���û�����sectionʱ���ݵ��û����ݡ�
 *	hRequest ���û�����ʱ���ص�handle��ͬ
 */
typedef void (*Section2CCallback)(SectionEventType event, Section* pSection,void* pUserData, HCSHANDLE hRequest);

#define Section2CSectionRequestEx Section2CSectionRequest


// }}}USR

/* {{{USR
**************************************************************************
* Function Name: CSUSPSectionSetDemuxCount
*   ���ÿ���Demux�ĸ������Ա���ͳ��ÿ��demux��filter�ĸ���
*
*Parameters	
*	nDemuxCount
*		[in]ϵͳ�п���Demux�ĸ���
*
*Return Values
*		�ɹ�����TRUE�����򷵻�FALSE.
*Remarks
*		DemuxCount��������ȡÿ��demux��filter���Ա�ͳ�Ƴ����ж��ٸ�filter��
*		�˺�����Ҫ��Section2CCreateSectionSource...Ҳ���ǳ�ʼ��sectionģ��֮ǰ����
*		��������ô˺�������ʼ��sectionģ��ʱ��Ĭ��Ϊֻ��1��demux��
**************************************************************************
   }}}USR */
BOOL CSUSPSectionSetDemuxCount( int nDemuxCount );

/* {{{USR
**************************************************************************
* Function Name: CSUSPSectionSetFilterBufSize
*   ����porting��buffer��С
*
*Parameters	
*	hRequest
*		[in]����sectionʱ���ص�handle
*     nBufSize
*		[in]porting��buffer��С
*
*Return Values
*		�ɹ�����TRUE�����򷵻�FALSE.
*Remarks
*		ֻ����filter����stop״̬ʱ��������buffer��С�����򷵻�FALSE
**************************************************************************
   }}}USR */
BOOL CSUSPSectionSetFilterBufSize(HCSHANDLE hRequest, int nBufSize);

/* {{{USR
**************************************************************************
* Function Name: CSUSPCreateSectionSource&CSUSPGetSectionSourceHandle
*   ����������SectionHandle
*
*Parameters	
*	pbSectionMemory
*		[in] ������Ϣ��ָ��.
*
*Return Values
*		�ɹ�����SectionSource�ľ�������򷵻�NULL.
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
*   ����section
*
*Parameters	
*	pInfo
*		[in] ������Ϣ��ָ��.
*
*	pCallback
*		[in] �ص�����ָ��.
*
*Return Values
*		����ɹ�����������䵽��section��������򷵻�NULL.
**************************************************************************
   }}}USR */
HCSHANDLE Section2CSectionRequest( RequestInfo * pInfo, Section2CCallback pCallback );

/* {{{USR

   }}}USR */
//HCSHANDLE Section2CSectionRequestEx( RequestInfoEx * pInfo, Section2CCallback pCallback );

/***************************************************************
ͨ��hRequest��ȡRequestInfo, ���ص�RequestInfo�����޸�
example: const* RequestInfo *psReqInfo = Section2CSectionGetRequestInfo(...);
����NULL��ʾʧ��
****************************************************************/
const RequestInfo* Section2CSectionGetRequestInfo(HCSHANDLE hRequest);
/* {{{USR
**************************************************************************
* Function Name: CSUSPSectionCancel
*   ȡ��section����
*
*Parameters	
*	hRequest
*		[in]����sectionʱ���ص�handle��Ҳ���Դӻص��ĺ���������ô�������.
*
*Return Values
*		ȡ���ɹ�����TRUE�����򷵻�FALSE.
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
*   ��ͣsection����
*
*Parameters	
*	hRequest
*		[in]����sectionʱ���ص�handle��Ҳ���Դӻص��ĺ���������ô�������.
*
*Return Values
*		ֹͣ�ɹ�����TRUE�����򷵻�FALSE.
**************************************************************************
   }}}USR */
BOOL Section2CSectionStop( HCSHANDLE hRequest );

/* {{{USR
**************************************************************************
* Function Name: CSUSPSectionStart
*   ���¿�ʼsection����
*
*Parameters	
*	hRequest
*		[in]����sectionʱ���ص�handle��Ҳ���Դӻص��ĺ���������ô�������.
*
*Return Values
*		�����ɹ�����TRUE�����򷵻�FALSE.
**************************************************************************
   }}}USR */
BOOL Section2CSectionStart(HCSHANDLE hRequest);

/* {{{USR
**************************************************************************
* Function Name: CSUSPSectionGetVersionInfo
*		��ȡģ��汾��Ϣ
*
*Parameters	
*	pcVersionInfo
*		[out]��Ű汾��Ϣ�Ļ������׵�ַ���������,��Ű汾��Ϣ.
*	nSize
*		[in]�������ĳ���
*
*Return Values
*	1)�����һ������pcVersionInfoΪ�գ����ذ汾��Ϣ��ʵ�ʳ��ȡ�
*	�����û�����ڶ�������Ϊ���汾��Ϣ��ʵ�ʳ���+       
*	2)�����һ������pcVersionInfo��Ϊ�գ��Һ������óɹ������ذ汾��Ϣ��ʵ�ʳ��ȣ�
*	3)���ʧ�ܣ�����0��
**************************************************************************
   }}}USR */
int CSUSPSectionGetVersionInfo(char * pcVersionInfo, int nSize );

/* {{{USR
**************************************************************************
* Function Name: CSUSPSectionGetMemInfo
*   ��ȡSection���ڴ����ԣ���С�ڴ������ڴ棩
*
*Parameters	
*	pnMin
*		[out]ģ��������С�ڴ� (in bytes)
*	pnMax
*		[out]ģ����������ڴ�(in bytes)
*
*Return Values
*		�ɹ�����TRUE�����򷵻�FALSE.
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
