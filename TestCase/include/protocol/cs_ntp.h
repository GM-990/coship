/******************************************************************************
**   
**   Notice:     Copyright (c) 2008， Shenzhen Coship Electronics Co., Ltd. 
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
    EM_NTP_MSGTYPE_GETTIME = 0,  	/*得到时间*/
    EM_NTP_MSGTYPE_TIMEOUT,      	/*NTP超时*/
    EM_NTP_MSGTYPE_UNRESOLVED,    	/*主机名不能被解析*/
    EM_NTP_MSGTYPE_FAILURE             	/*未知原因失败*/
}CSNTPMsgType_E; 

/* ret code define */
typedef enum CSNTPRet_E
{
	EM_NTP_RET_SUCCESS = 0,          	/*成功*/
	EM_NTP_RET_FAILURE,              	/*未知原因失败*/
	EM_NTP_RET_UNRESOLVED,		/*主机名不能被解析*/
	EM_NTP_RET_TIMEOUT,			/*NTP超时，主机无响应*/
	EM_NTP_RET_INVALPARA,            	/*函数参数无效*/
	EM_NTP_RET_CEATE_INSTANCE_ERROR  /*创建实例句柄失败*/
} CSNTPRet_E;


/* state code define */
typedef enum CSNTPState_E
{
    EM_NTP_STATE_REQUESTING = 0,     /*NTP状态为正在请求时间*/
    EM_NTP_STATE_UPDATED,      		 /*NTP状态为已更新时间*/
    EM_NTP_STATE_FAILURE       		 /*NTP状态为失败*/
}CSNTPState_E;


typedef struct CSNTPPara_S
{
	int  		m_nState;       /*NTP模块状态*/
	int  		m_nPID;         /*NTP线程PID*/ 
	DWORD 	m_dwNTPTime;/*对时时间自1970年1月1日起至现在的0时区总秒数*/
}CSNTPPara_S;


typedef BOOL (*FOnNTPState)(int nMsgType, void *pPara);

typedef struct CSNTPInitPara_S
{
	DWORD 	m_dwFlag;       /*dwFlag为0表示需要启动NTP服务线程，为其他值则不启动*/
    	DWORD 	m_dwInterval;   /*如果dwFlag为0，NTP服务时间同步间隔，单位：秒*/
    	DWORD 	m_dwTimeout;    /*如果dwFlag不为0，需传入NTP同步的超时时间，单位：毫秒*/
    	char  	*m_pcServer;   /*NTP服务器地址,可以是域名或I*/
    	BOOL  	(*m_fCallback)(int nMsgType, void *pPara);/* NTP服务回调函数指针，可以为NULL */	
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

