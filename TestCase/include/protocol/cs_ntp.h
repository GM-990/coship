/******************************************************************************
**   
**   Notice:     Copyright (c) 2008�� Shenzhen Coship Electronics Co., Ltd. 
**                          All rights reserved.
**   
**   FileName:   cs_ntp.h
**   
**   Version:    Ver 1.0
**   
**   Author:     SunHuan
**   
**   Description:
**               The impelement of NTP subset SNTP, see RFC 1361.
**   
**   Modify History:
**
**   Version     Date        Author          Description
**   --------    ----------  --------        ------------
**   1.0      2008-06-05  Sun Huan       Create
*******************************************************************************/
#ifndef CS_NTP_H
#define CS_NTP_H

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************
 *                                 Header File Include                         *
 ******************************************************************************/
#include "mmcp_typedef.h"
#include "cs_long.h"

/******************************************************************************
 *                                 Macro/Define/Structus                       *
 ******************************************************************************/
#define  NTP_SERVER_NAME_LEN      (256)


typedef enum CSNTPMsgType_E
{
    EM_NTP_MSGTYPE_GETTIME = 0,  	/*�õ�ʱ��*/
    EM_NTP_MSGTYPE_TIMEOUT,      	/*NTP��ʱ*/
    EM_NTP_MSGTYPE_UNRESOLVED,    	/*���������ܱ�����*/
    EM_NTP_MSGTYPE_FAILURE             	/*δ֪ԭ��ʧ��*/
}CSNTPMsgType_E; 

/* ret code define */
typedef enum CSNTPRet_E
{
	EM_NTP_RET_SUCCESS = 0,          	/*�ɹ�*/
	EM_NTP_RET_FAILURE,              	/*δ֪ԭ��ʧ��*/
	EM_NTP_RET_UNRESOLVED,		/*���������ܱ�����*/
	EM_NTP_RET_TIMEOUT,			/*NTP��ʱ����������Ӧ*/
	EM_NTP_RET_INVALPARA,            	/*����������Ч*/
	EM_NTP_RET_CEATE_INSTANCE_ERROR  /*����ʵ�����ʧ��*/
} CSNTPRet_E;


/* state code define */
typedef enum CSNTPState_E
{
    EM_NTP_STATE_REQUESTING = 0,     /*NTP״̬Ϊ��������ʱ��*/
    EM_NTP_STATE_UPDATED,      		 /*NTP״̬Ϊ�Ѹ���ʱ��*/
    EM_NTP_STATE_FAILURE       		 /*NTP״̬Ϊʧ��*/
}CSNTPState_E;


typedef struct CSNTPPara_S
{
	int  		m_nState;       /*NTPģ��״̬*/
	int  		m_nPID;         /*NTP�߳�PID*/ 
	DWORD 	m_dwNTPTime;/*��ʱʱ����1970��1��1���������ڵ�0ʱ��������*/
}CSNTPPara_S;


typedef BOOL (*FOnNTPState)(int nMsgType, void *pPara);

typedef struct CSNTPInitPara_S
{
	DWORD 	m_dwFlag;       /*dwFlagΪ0��ʾ��Ҫ����NTP�����̣߳�Ϊ����ֵ������*/
    	DWORD 	m_dwInterval;   /*���dwFlagΪ0��NTP����ʱ��ͬ���������λ����*/
    	DWORD 	m_dwTimeout;    /*���dwFlag��Ϊ0���贫��NTPͬ���ĳ�ʱʱ�䣬��λ������*/
    	char  	*m_pcServer;   /*NTP��������ַ,������������I*/
    	BOOL  	(*m_fCallback)(int nMsgType, void *pPara);/* NTP����ص�����ָ�룬����ΪNULL */	
}CSNTPInitPara_S;


/******************************************************************************
 *                                 Global Function Declare                    *
 ******************************************************************************/


/******************************************************************************
Function Name: 	CSNTPStart
 
Description:
    Start a NTP service. 
 
Input:
	psInitPara:  the Init parameter of NTP module, must not be NULL.
 
Output:
    phNtpHandle    : Pointer allocated by the caller to a handle that receives the
handle of the NTP instance. on success,return valid message Queue handle,or return 
INVALID_HANDLE.
 
Return Value:
    return EM_NTP_RET_SUCCESS on success.  
    return EM_NTP_RET_FAILURE or other error codes on error.
 
Other:
     This function may be called from and thread context. When need DNS parse,this function is blocked
 because of gethostbyname, otherwise it is unblocked.
*******************************************************************************/
CSNTPRet_E CSNTPStart(CSNTPInitPara_S *psInitPara, HCSHANDLE *phNtpHandle);



/******************************************************************************
Function Name: 	CSNTPStop
 
Description:
    Stop NTP service.
 
Input:
	hNtpHandle: Handle to the NTP instance
 
Output:
	None
 
Return Value:
    return EM_NTP_RET_SUCCESS on success.  
    return EM_NTP_RET_FAILURE or other error codes on error.
 
Other:
    This call may be called from any thread context, and should only be call from
the same process  context as CSNTPStart.
*******************************************************************************/
CSNTPRet_E CSNTPStop( HCSHANDLE hNtpHandle );

/******************************************************************************
Function Name: 	CSNTPGetState
 
Description:
    Get the state of the NTP service.
 
Input:
	hNtpHandle: Handle to the NTP instance.
 
Output:
	pnState: ntp service state 
 
Return Value:
    return EM_NTP_RET_SUCCESS on success.  
    return EM_NTP_RET_FAILURE or other error codes on error.
 
Other:
    This function may be called from any thread context, and should only be call
  from the process context of CSNTPStart
*******************************************************************************/
CSNTPRet_E CSNTPGetState( HCSHANDLE hNtpHandle, int *pnState );


#ifdef __cplusplus
}
#endif

#endif  /* CSIP_NTP_H */

/* End of File */

