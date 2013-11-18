/******************************************************************************
FileName:	CSIP_HTTP.H
Copyright (c) 2006， Shen Zhen Coship Electronics CO.,LTD.。

Version:		Ver 1.0    
Author:		likunpeng (likunpeng@coship.com)

File Description:
		This is a  public head file of HTTP module.

Modify History:
Version		Date			Author			Description
--------	----------	--------		------------
1.0			2006-9-16	likunpeng		Create
*******************************************************************************/

#ifndef CSIP_HTTP_H
#define CSIP_HTTP_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 *                                 Header File Include                         *
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "mmcp_os.h"
#include "mmcp_typedef.h"

/******************************************************************************
 *                                 Macro/Define/Structus                       *
 ******************************************************************************/
/*#define HTTP_CONNECTION_OPTION*/

#define  HTTP_URL_MAX_LENGTH               1024
#define  HTTP_AUTHENTICATE_MAX_LENGTH      512
#define  HTTP_TRASFER_ENCODING_MAX_LENGTH  16
#define  HTTP_CONTENT_TYPE_MAX_LENGTH      32
#define  HTTP_CONTENT_RANGE_MAX_LENGTH     64
#define  HTTP_LAST_MODIFIED_LENGTH         128
#define  HTTP_CACHE_CONTROL_LENGTH         128
#define  HTTP_EXPRIES_LENGTH         128
#define  HTTP_COOKIE_LENGTH         128

typedef struct _CSHTTPSSLCertFile
{
     	/**
     	@brief https ssl 认证的certificate file path
     	*/
	char *pcCertFile;
	/**
     	@brief https ssl 认证的certificate file path len
     	*/
	int 	nCertFileLen;
	/**
     	@brief https ssl 认证的key file  path
     	*/
	char *pcKeyFile;
	/**
     	@brief https ssl 认证的key file  path len
     	*/
	int    nKeyFileLen;
	/**
     	@brief https ssl 认证的certificate file  and key file  password
     	*/
	char *pcPassWord;
	/**
     	@brief https ssl 认证的password len
     	*/
	int    nPassWordLen;	
	/**
     	@brief https ssl 认证的ca file path 
     	*/
	char *pcCaFile;	
	/**
     	@brief https ssl 认证的ca file  path len
     	*/
	int 	nCaFileLen;
}CSHTTPSSLCertFile_S;

typedef struct _CSHTTPSSLOperation
{
     	/**
     	@brief https ssl 认证的回调函数
     	@param hHttpHandle http 句柄；nSocketHandle  socket 句柄
     	*/
	int (* m_fnSSLAuth)(HCSHANDLE hHttpHandle,int nSocketHandle);

     	/**
     	@brief https ssl 读数据的回调函数
     	@param  hHttpHandle http 句柄；pcReadBuf  存放读数据的缓冲区的指针，nLen 存放读数据缓冲区的长度
     	*/
     	int (* m_fnSSLRead)(HCSHANDLE hHttpHandle,void *pcReadBuf,int nLen);
		
     	/**
     	@brief https ssl 写数据的回调函数
     	@param hHttpHandle http 句柄；pcReadBuf  存放写数据的缓冲区的指针，nLen 需要写的数据的长度
     	*/
	int (* m_fnSSLWrite)(HCSHANDLE hHttpHandle,void *pcWriteBuf,int nLen);

     	/**
     	@brief https ssl 关闭连接的回调函数
     	@param hHttpHandle http 句柄
     	*/
	int (* m_fnSSLClose)(HCSHANDLE hHttpHandle);	
		
}CSHTTPSSLOperation_S;


typedef struct
{
	int     nContentLength;
	char    acContentType[HTTP_CONTENT_TYPE_MAX_LENGTH];
	char    acContentRange[HTTP_CONTENT_RANGE_MAX_LENGTH];
	char    acLocation[HTTP_URL_MAX_LENGTH];
	char    acAuthenticate[HTTP_AUTHENTICATE_MAX_LENGTH];
	char    acTransferEncoding[HTTP_TRASFER_ENCODING_MAX_LENGTH];
	DWORD   dwBufferLen;
	char   *pcBuffer;
    char    acLastmodified[HTTP_LAST_MODIFIED_LENGTH];
    char    acEtag[HTTP_LAST_MODIFIED_LENGTH];
	char   acCacheControl[HTTP_CACHE_CONTROL_LENGTH];
	char   acExpires[HTTP_EXPRIES_LENGTH];
}CSHTTPHeadInfo_S;

typedef enum
{
	EM_HTTP_NO_WHENCE = -1,
	EM_HTTP_SET,
	EM_HTTP_CUR,
	EM_HTTP_END
}CSHTTPWhence_E;

typedef enum  CSHTTPRet_E
{
	EM_HTTP_SUCCESS = 0, 
	EM_HTTP_FAILURE,
	EM_HTTP_TIMEOUT,
	EM_HTTP_INVALPARA,     /*参数错误*/
	EM_HTTP_NOMEM,
	EM_HTTP_NOSUPPORT_RANGE, /*不支持断点续传*/
	EM_HTTP_URL_ERROR, /*URL错误*/
	EM_HTTP_SOCKET_ERROR,/*socket 错误*/
	EM_HTTP_EOF_ERROR, /*已经到达文件的结尾了*/
	EM_HTTP_LOCATION_ERROR,/*URL被重定向*/
	EM_HTTP_UNAUTH,
	EM_HTTP_FINISH
} CSHTTPRet_E;

typedef enum CSIP_HTTP_IISErr_E
{
    EM_HTTP_UNKNOW_ERROR = -1,
    EM_HTTP_CONTINUE = 1000,
    EM_HTTP_SWITCHING_PROTOCOL = 1010,
    EM_HTTP_OK = 2000,
    EM_HTTP_CREATE = 2010,
    EM_HTTP_ACCEPTED = 2020,
    EM_HTTP_NON_AUTHORIZATIVE_INFO = 2030,
    EM_HTTP_NO_CONTENT = 2040,
    EM_HTTP_RESET_CONTENT = 2050,
    EM_HTTP_PARTIAL_CONTENT = 2060,
    EM_HTTP_MUTIPLE_CHOICES = 3000,
    EM_HTTP_MOVE_PERMANENTLY = 3010,
    EM_HTTP_FOUND = 3020,
    EM_HTTP_SEE_OTHER = 3030,
    EM_HTTP_NOT_MODIFIED = 3040,
    EM_HTTP_USE_PROXY = 3050,
    EM_HTTP_TEMPORARY_REDIRECT = 3070,
    EM_HTTP_BAD_REQUEST = 4000,
    EM_HTTP_UNAUTHORIZED = 4010,
    EM_HTTP_CREDENTIAL_INVALID, 
    EM_HTTP_AUTHORIZATION_METHOD_ERROR, 
    EM_HTTP_ACL_SETUP,
    EM_HTTP_SERVER_FILTER,
    EM_HTTP_PROGRAMM_AUTHORIZATION_FAIL,
    EM_HTTP_URL_AUTHORIZATION_POLICY,
    EM_HTTP_PAYMENT_REQUIRED = 4020,
    EM_HTTP_FORBIDDEN = 4030,    
    EM_HTTP_EXECUTE_ACCESS_REFUSE,
    EM_HTTP_READ_ACCESS_REFUSE,
    EM_HTTP_WRITE_ACCESS_REFUSE,
    EM_HTTP_NEED_SLL,
    EM_HTTP_NEED_SLL128,
    EM_HTTP_GUEST_IP_REFUSED,
    EM_HTTP_NEED_SLL_CERTIFICATE,
    EM_HTTP_GUEST_DNS_REFUSED,
    EM_HTTP_TOO_MANY_GUEST,
    EM_HTTP_NOT_FOUND = 4040,
    EM_HTTP_PORT_NOT_ACCESS,
    EM_HTTP_LOCK_POLICY_FORBID,
    EM_HTTP_MAP_POLICY_FORBID,
    EM_HTTP_METHOD_NOT_ALLOWED = 4050, 
    EM_HTTP_NOT_ACCEPTABLE = 4060,
    EM_HTTP_PROXY_AUTHORIZATION_REQUIRED = 4070,
    EM_HTTP_REQUEST_TIMEOUT = 4080,
    EM_HTTP_CONFLICT  = 4090,
    EM_HTTP_GONE = 4100,
    EM_HTTP_LENGTH_REQUIRED = 4110,
    EM_HTTP_PRECONDITION_REQUIRED = 4120,
    EM_HTTP_REQUEST_ENTITY_TOO_LARGE = 4130,
    EM_HTTP_REQUEST_URI_TOO_LONG = 4140,
    EM_HTTP_UNSUPPORTED_MEDIA_TYPE = 4150,
    EM_HTTP_REQUESTED_RANGE_NOT_SATISFIABLE = 4160,
    EM_HTTP_EXPECTATION_FAILED = 4170,
    EM_HTTP_INTERNAL_SERVER_ERROR = 5000,    
    EM_HTTP_SERVER_PROGRAMM_CLOSE,
    EM_HTTP_SERVER_PROGRAMM_REBOOT,
    EM_HTTP_SERVER_BUSY,
    EM_HTTP_SERVER_SETUP_INVALID,
    EM_HTTP_DIRECT_REQUEAT_GLOBALASA,
    EM_HTTP_UNC_CREDENTIAL_NOT_CORRECT,
    EM_HTTP_URL_MEMROU_NOT_FOUND,
    EM_HTTP_URL_MEMROU_NOT_OPEN,
    EM_HTTP_FILE_STATION_NOT_CORRECT,
    EM_HTTP_URL_DOMAIN_NOT_FOUND,
    EM_HTTP_ASP_ERROR,
    EM_HTTP_NOT_IMPLEMENT = 5010,
    EM_HTTP_BAD_GATEWAY = 5020,
    EM_HTTP_SERVICE_UNAVAILALBE = 5030,
    EM_HTTP_GATEWAY_TIMEOUT = 5040,
    EM_HTTP_VERSION_NOT_SUPPORT = 5050
} CSIP_HTTP_IISErr_E;


/******************************************************************************
 *                                 Global Function Declare                     *
 ******************************************************************************/

/******************************************************************************
Function Name: 	CSIPHTTPOpen

Description:
		create socket and init http module.

Input:
	pszUserName:    user name.
	pszPassword:    password.
    pszUrl:         HTTP URL.
    dwTimeout:      time out(ms).

Output:
	phHttpHandle:  HTTP Handle

Return Value:
    SUCCESS:  EM_HTTP_SUCCESS
    FAILURE:  EM_HTTP_FAILURE or other

Other:
*******************************************************************************/ 
int  CSHTTPOpen ( const char    *pszUserName,
						const char    *pszPassword,
						const char    *pszUrl,
					    DWORD          dwTimeout,
						HCSHANDLE     *phHttpHandle);


/******************************************************************************
Function Name: 	CSIP_HTTP_Close

Description:
		close socket and free memory.

Input:
	hHttpHandle: http handle

Output:
	None

Return Value:
    SUCCESS:  EM_HTTP_SUCCESS
    FAILURE:  EM_HTTP_FAILURE or other

Other:
*******************************************************************************/										
int CSHTTPClose (HCSHANDLE  hHttpHandle);


/******************************************************************************
Function Name: 	CSIPHTTPHead

Description:
		http head method

Input:
	hHttpHandle:  http handle
	unTimeOut:     timeout(ms)
	pcExtHeaderData:  the extend header data.

Output:
	psHeadInfo:   http handle info
	pnIISErr:     IIS  Error code
	
Return Value:
    SUCCESS:  EM_HTTP_SUCCESS
    FAILURE:  EM_HTTP_FAILURE or other

Other:
*******************************************************************************/
int    CSHTTPHead ( HCSHANDLE          hHttpHandle,
               			  char              *pcExtHeaderData,
               			  DWORD              dwExtHeaderDataLen,
               			  DWORD              dwTimeout,
               			  CSHTTPHeadInfo_S  *psHeadInfo,
               			  int               *pnIISErr);

/******************************************************************************
Function Name: 	CSIPHTTPGet

Description:
		HTTP Get method

Input:
	hHttpHandle:  http handle.
	nOffset:      offset.
	nWhence:      whence
    puLen: 		  buffer length
    uTimeOut:     timeout(ms)
    pcExtHeaderData:  the extend header data.

Output:
	pcBuffer:     date buffer
    pnIISErr:     IIS Error code	
	puLen:        date length in fact

Return Value:
    SUCCESS:  EM_HTTP_SUCCESS
    FAILURE:  EM_HTTP_FAILURE or other

Other:
*******************************************************************************/
int      CSHTTPGet (HCSHANDLE    hHttpHandle, 
						 int          nOffset,
						 int          nWhence,
						 char        *pcExtHeaderData,
						 DWORD        dwExtHeaderDataLen,
						 DWORD        dwTimeout,
						 char        *pcBuffer, 
						 DWORD        *pdwLen,
						 int         *pnIISErr);

/******************************************************************************
Function Name: 	CSIPHTTPPost

Description:
		HTTP Post method.

Input:
	hHttpHandle: http handle.
    puLen:       buffer length
    pnIISErr:    IIS Error code
    uTimeOut:    timeout(ms)
    pcExtHeaderData:  the extend header data.
    
Output:
	pcBuffer:    date buffer
    puLen:       buffer length in fact
    
Return Value:
    SUCCESS:  EM_HTTP_SUCCESS
    FAILURE:  EM_HTTP_FAILURE or other
    
Other:
*******************************************************************************/									
int  CSHTTPPost (HCSHANDLE   hHttpHandle,
					  char       *pcBuffer,
					  DWORD       dwBufLen,
					  char       *pcExtHeaderData,
					  DWORD       dwExtHeaderDataLen,
					  DWORD       dwTimeout,
					  int        *pnIISErr,
					  char       *pcResponse,
					  DWORD      *pdwResLen);
/******************************************************************************
Function Name: 	CSHTTPGetCommand

Description:
		Just send Get Command and parse the head of response. After calling this fuction
		you must call CSHTTPGetData to recv data then ,and this is the difference from  CSHTTPGet,which
		both send command and get data.	
Input:
	hHttpHandle:  http handle.
	nOffset:      offset.
	nWhence:      whence
    puLen: 		  buffer length
    uTimeOut:     timeout(ms)
    pcExtHeaderData:  the extend header data.

Output:
    pnIISErr:     IIS Error code	
	
Return Value:
    SUCCESS:  EM_HTTP_SUCCESS
    FAILURE:  EM_HTTP_FAILURE or other

Other:Note that the difference between CSHTTPGet and CSHTTPGetCommmand is CSHTTPGetCommmand 
do not get data 
*******************************************************************************/
int      CSHTTPGetCommand (HCSHANDLE    hHttpHandle, 
						 int          nOffset,
						 int          nWhence,
						 char        *pcExtHeaderData,
						 DWORD        dwExtHeaderDataLen,
						 DWORD        dwTimeout,
						 int         *pnIISErr);
/******************************************************************************
Function Name: 	CSHTTPPostCommand

Description:
		Just send Post Command and parse the head of response. After calling this fuction
		you must call CSHTTPGetData to recv data then .
Input:
	hHttpHandle: http handle.
    puLen:       buffer length
    pnIISErr:    IIS Error code
    uTimeOut:    timeout(ms)
    pcExtHeaderData:  the extend header data.
    
Output:
	pcBuffer:    date buffer
    puLen:       buffer length in fact
    
Return Value:
    SUCCESS:  EM_HTTP_SUCCESS
    FAILURE:  EM_HTTP_FAILURE or other
    
Other:Note that the difference between CSHTTPPost and CSHTTPPostCommand is CSHTTPGetCommmand 
do not get data 
*******************************************************************************/	
int  CSHTTPPostCommand (HCSHANDLE   hHttpHandle,
					  char       *pcBuffer,
					  DWORD       dwBufLen,
					  char       *pcExtHeaderData,
					  DWORD       dwExtHeaderDataLen,
					  DWORD       dwTimeout,
					  int        *pnIISErr);
/******************************************************************************
Function Name: 	CSHTTPGetData

Description:
	Used to get data after calling CSHTTPGetCommand and CSHTTPPostCommand.

Input:
	hHttpHandle: http handle.
    pnIISErr:    IIS Error code
    uTimeOut:    timeout(ms)
    
Output:
	pcBuffer:    date buffer
    puLen:       buffer length in fact
    
Return Value:
    SUCCESS:  EM_HTTP_SUCCESS
    FAILURE:  EM_HTTP_FAILURE or other
    
Other:
*******************************************************************************/					
int  CSHTTPGetData (HCSHANDLE   hHttpHandle,
					DWORD       dwTimeout,
					char       *pcDataBuffer,
					DWORD     	 *pdwLen);


/******************************************************************************
Function Name: 	CSHTTPSSLSetFilePath

Description:
	Used to set https certificate /key /password and cafile path.

Input:
	sCertFile certfile path struct
    
Output:
    none
    
Return Value:
    SUCCESS:  EM_HTTP_SUCCESS
    FAILURE:  EM_HTTP_FAILURE or other
    
Other:
*******************************************************************************/
int CSHTTPSSLSetCertFilePath(CSHTTPSSLCertFile_S sCertFile);



/******************************************************************************
Function Name: 	CSHTTPGetSSLOperation

Description:
	Used to get https ssl para.

Input:
    none
    
Output:
    none
    
Return Value:
    SUCCESS:  CSHTTPSSLParam_S 
    FAILURE:  NULL
    
Other:
*******************************************************************************/
const CSHTTPSSLOperation_S *CSHTTPGetSSLOperation(void);



/******************************************************************************
Function Name: 	CSHTTPRegistSSLOperation

Description:
	Used to set https ssl para.

Input:
    psParam: https ssl para.
    
Output:
    none
    
Return Value:
    SUCCESS:  EM_HTTP_SUCCESS
    FAILURE:  EM_HTTP_FAILURE or other
    
Other:
*******************************************************************************/
int CSHTTPRegistSSLOperation(CSHTTPSSLOperation_S * psOperation);


#ifdef __cplusplus
}
#endif

#endif  /* CSIP_HTTP_H */

/* End of File */


