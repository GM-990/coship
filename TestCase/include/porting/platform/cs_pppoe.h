/******************************************************************************
FileName:	CSIP_PPPOE.H
Copyright (c) 2006， Shen Zhen Coship Electronics CO.,LTD。

Version:		Ver 1.0    
Author:		Nevill (luoyangzhi@coship.com)

File Description:
Header file to support pppoe

Modify History:
Version	Date		Author		Description
--------	----------	--------	------------
1.0			2006-9-28	Nevill		Create
1.1         2007-5-9    Huhuajun    Modify
                                    Increase two Interfaces:
                                    CSPPPOESetServiceName()
                                    and CSPPPOEGetServiceName()
*******************************************************************************/

#ifndef CSPPPOE_H
#define CSPPPOE_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 *                                 Header File Include                        *
 ******************************************************************************/

#include "mmcp_os.h"

/******************************************************************************
 *                                 Macro/Define/Structus                      *
 ******************************************************************************/
#ifndef CS_IP4_LEN
#define CS_IP4_LEN  16
#endif


#ifndef ETH_ALEN
#define ETH_ALEN	6	
#endif

/* The max length of Service name.(unit:bytes) */
#ifndef CS_PPPOE_SERVICENAME_MAXLEN
#define CS_PPPOE_SERVICENAME_MAXLEN  256   
#endif



#ifndef CS_PPPOE_USERNAME_MAXLEN
#define CS_PPPOE_USERNAME_MAXLEN  256   
#endif

#ifndef CS_PPPOE_USERPWD_MAXLEN
#define CS_PPPOE_USERPWD_MAXLEN  256   
#endif


typedef enum CS_RET_Base_e
{
	CS_PPPOE_RET_SUCCESS = 0,   /* 成功(始终为0)     */
	CS_PPPOE_RET_FAILURE	, 	/* 未知原因失败(始终为1)   */
	CS_PPPOE_RET_TIMEOUT	, 	/* 超出超时时间限制错误  */
	CS_PPPOE_RET_FATAL  , 	/* 致命错误,通常是在调用操作系统API或CSIP平台
	                           OsPorting API函数失败时，需要给上层返回该错误号*/
	CS_PPPOE_RET_EXIST	    ,	/* 资源、文件已经存在或模块已经加载,通常是调用模块
	                           初始化函数(CSIP_XXX_Init)或资源创建函数
	                           (CSIP_XXX_CreateYYY)失败时返回                 */
	CS_PPPOE_RET_NOTEXIST	,	/* 资源、文件不存在或模块已卸载,通常是调用模块反初
	                           始化函数(CSIP_XXX_Cleanup)或资源销毁函数
	                           (CSIP_XXX_DestoryYYY)失败时返回                */ 
	CS_PPPOE_RET_INVALPARA,   	/* 非法参数,当输入参数不符合平台接口定义的参数范围
	                           或其他规定，通常返回该错误号;指针(或句柄)非法也可
	                           以返回该错误号，但是一般建议返回CS_PPPOE_INVALPOINT
	                           或 CS_PPPOE_INVALHANDLE */
	CS_PPPOE_RET_AUTHFAILURE,           /*认证失败*/
	CS_PPPOE_RET_OTHER

} CS_RET_Base_e;


typedef enum CS_PPPOE_MsgType_e
{
	PPPOE_MSGTYPE_PHASE_DEAD = 0,       /* link is dead */
	PPPOE_MSGTYPE_ESTABLISH,            /* establishing */
	PPPOE_MSGTYPE_AUTHENTICATE,         /* in authentication */
	PPPOE_MSGTYPE_NETWORK,              /* getting IP */
	PPPOE_MSGTYPE_RUNNING,              /* got ip */
	PPPOE_MSGTYPE_AUTH_FAILURE ,        /* authentication failed */
	PPPOE_MSGTYPE_SESSIONID_CHANGED,    /* session ID changed */
	PPPOE_MSGTYPE_SERVER_NO_RESPONSE,   /* no response from server */
	PPPOE_MSGTYPE_TIMEOUT,             /* timeout */
	PPPOE_MSGTYPE_CLEAR_TO_INIT,			/* 清除状态至无连接状态 */
	PPPOE_MSGTYPE_TERMINATEBYPEER     /*PPPOE客户端被踢出 */
} CS_PPPOE_MsgType_e;


typedef enum CS_PPPOE_State_e
{
	PPPOE_STATE_PHASE_DEAD = 0, /* dead */
	PPPOE_STATE_ESTABLISH,      /* establishing */
	PPPOE_STATE_AUTHENTICATE,   /* in authentication */
	PPPOE_STATE_NETWORK,        /* getting IP */
	PPPOE_STATE_RUNNING,         /* got ip */
	PPPOE_STATE_AUTH_FAILURE ,        /* authentication failed */
	PPPOE_STATE_SESSIONID_CHANGED,    /* session ID changed */
	PPPOE_STATE_SERVER_NO_RESPONSE,   /* no response from server */
	PPPOE_STATE_TIMEOUT ,              /* timeout */
	PPPOE_STATE_OTHER
} CS_PPPOE_State_e;

typedef struct CS_PPPOE_Para_t              /*该结构体由回调函数抛出*/
{
	int nState;                        /* pppoe state 其状态可以取得的值为CS_PPPOE_State_e类型中的值*/
	int nPID;                          /* pppd pid */
	/* local IP, a string ended with '\0', as "255.255.255.255" */
	char cLocalIP[CS_IP4_LEN];  		
	/* remote IP, a string ended with '\0',as "255.255.255.255" */
	char cRemoteIP [CS_IP4_LEN];     
	char cMask[CS_IP4_LEN];
	char cGateWay[CS_IP4_LEN];
	char cDNS1[CS_IP4_LEN];
	char cDNS2[CS_IP4_LEN];
	char cACMac[ETH_ALEN];           /* MAC of AC "FFFFFFFFFFFF"*/
	unsigned short usSessionId;                  /*  PPPoE session ID*/
} CS_PPPOE_Para_t;

/* callback fun */
typedef void (*PPPOE_Callback_fn)(int nMsgType, void *pPara);

typedef struct CS_PPPOE_InitPara_t
{    
       char cACMac[ETH_ALEN];   /* MAC of AC "FFFFFFFFFFFF" */
	unsigned short usSessionId;          /* PPPoE session ID */
	int unStackSize;           /* stack size for pppoe thread. */
	int unPriority;            /* priority for pppoe thread */
	PPPOE_Callback_fn fnCallback;   /* callback fn, could be NULL */
} CS_PPPOE_InitPara_t;


/******************************************************************************
 *                                 Global Function Declare                    *
 ******************************************************************************/
/******************************************************************************
Function Name: 	CSPPPOEInit

Description:
	Init the pppoe. Must be called before use any function in this function.

Input:
	ptInitPara - Info for init pppoe.

Output:
	None

Return Value:
	CS_PPPOE_RET_INVALPARA: Input error.
	CS_PPPOE_RET_EXIST: pppd existed.
	CS_PPPOE_RET_FATAL: linux api failed.
	CS_PPPOE_RET_SUCCESS: success.
	
Other:
*******************************************************************************/
unsigned int CSPPPOEInit (CS_PPPOE_InitPara_t *ptInitPara);


/******************************************************************************
Function Name: 	CSPPPOECleanup

Description:
		Cleanup this module.

Input:
	nFlag - kill the pppd gentle or rude.

Output:
	None

Return Value:
	CS_PPPOE_RET_NOTEXIST: no running pppd.
	CS_PPPOE_RET_INVALPARA: input error
	CS_PPPOE_RET_FATAL: failed.
	CS_PPPOE_RET_SUCCESS: success.
	

Other:
*******************************************************************************/
unsigned int CSPPPOECleanup (int nFlag);


/******************************************************************************
Function Name: 	CSPPPOEConnect

Description:
		connect to server

Input:
	nTimeout - wait for return.

Output:
	None

Return Value:
    CS_PPPOE_RET_NOTEXIST: no running pppd.
    CS_PPPOE_RET_FATAL: api failed.
    CS_PPPOE_RET_SUCCESS: success.
    CS_PPPOE_RET_EXIST: has linked up.
    CS_PPPOE_RET_TIMEOUT: timeout
    CS_PPPOE_RET_FAILURE: get time failed.
    CS_PPPOE_RET_AUTHFAILURE: auth to peer failed.
    CS_PPPOE_RET_OTHER: other errors.
	
Other:
*******************************************************************************/
unsigned int CSPPPOEConnect(const char *szUserName, const char *szPasswd, char *szServiceName, char *pcEthName, int nTimeout);


/******************************************************************************
Function Name: 	CSPPPOEDisconnect

Description:
	disconnect the link.

Input:
	None

Output:
	None

Return Value:
    CS_PPPOE_RET_NOTEXIST: no running pppd.
    CS_PPPOE_RET_FATAL: api failed.
    CS_PPPOE_RET_SUCCESS: success.
    CS_PPPOE_RET_NOTEXIST: has turned down.

Other:
*******************************************************************************/
unsigned int CSPPPOEDisconnect(int nTimeout );


/******************************************************************************
Function Name: 	CSPPPOEGetServiceName

Description:
	Get the name of PPPOE service.

Input:
	None

Output:
	szSevicename : Pointer to pointer that pointer to the name of PPPOE service will be set.
    Notes:The resource of szSevicename applied for by CSPPPOEGetServiceName().
Return Value:
    CS_PPPOE_RET_NOTEXIST: no service name.
    CS_PPPOE_RET_SUCCESS: success.
    CS_PPPOE_RET_FAILURE: failure.
    CS_PPPOE_RET_INVALPARA: invalid parameters.
    
Other:
*******************************************************************************/
unsigned int CSPPPOEGetServiceName( char *szSevicename);

/******************************************************************************
Function Name: 	CSPPPOEGetState

Description:
	Get link state

Input:
	pnState - buffer to get the state

Output:
	pnState - the link state

Return Value:
    CS_PPPOE_RET_NOTEXIST: no running pppd.
    CS_PPPOE_RET_SUCCESS: success.

Other:
*******************************************************************************/
unsigned int CSPPPOEGetState(int *pnState);


/******************************************************************************
Function Name: 	CSPPPOEGetACMacAddr

Description:
	Get AC MAC

Input:
	cMAC[] - buffer to store the MAC. should be longer then 6bytes.

Output:
	cMAC[] - MAC

Return Value:
    CS_PPPOE_RET_NOTEXIST: no running pppd.
    CS_PPPOE_RET_SUCCESS: success.

Other:
*******************************************************************************/
unsigned int CSPPPOEGetACMacAddr(char *cMAC);


/******************************************************************************
Function Name: 	CSPPPOEGetSessionID

Description:
	Get session ID.

Input:
	pSessionID - buffer to store the session ID.

Output:
	pSessionID - session ID

Return Value:
    CS_PPPOE_RET_NOTEXIST: no running pppd.
    CS_PPPOE_RET_SUCCESS: success.

Other:
*******************************************************************************/
unsigned int CSPPPOEGetSessionID(unsigned short *pusSessionID);
int CSPPPOEGetVersion( char *pchVer);


#ifdef __cplusplus
}
#endif
#endif  /* CSIP_PPPOE_H */

/* End of File */

