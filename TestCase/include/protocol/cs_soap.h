/******************************************************************************
FileName:  csip_soap.h
Copyright (c) 2007£¬ Shen Zhen Coship Electronics CO.,LTD.¡£

Version:       Ver 1.0
Author:        likunpeng (likunpeng@coship.com)

File Description:
		This is a  public head file of SOAP module.

Modify History:
Version     Date        Author          Description
--------    ----------  --------        ------------
1.0         2007-04-03  likunpeng       Create

*******************************************************************************/
#ifndef CSIP_SOAP_H
#define CSIP_SOAP_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 *                                 Header File Include                         *
 ******************************************************************************/		  		

#include <stdio.h>
#include <stdlib.h>
#if defined(PLATFORM_ST40) || defined(PLATFORM_ST20)
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include "mmcp_os.h"
#endif
#include "mmcp_typedef.h"

/******************************************************************************
 *                                 Macro/Define/Structus                       *
 ******************************************************************************/



/*the max length URL*/
#define SOAP_URL_MAX_LENGTH       1024

/*In the soap module, the max length of the http header data, which is user definition*/
#define SOAP_HTTP_HANDDATA_LENGTH 1024

/*this is  the length of the buffer of receive soap message data*/
#define SOAP_RECEIVE_DATA_MAX_LENGTH 2048

typedef enum
{
	EM_SOAP_SUCCESS = 0, 
	EM_SOAP_FAILURE,
    EM_SOAP_TIMEOUT,
    EM_SOAP_INVALPARA,
    EM_SOAP_NOMEM,
    EM_SOAP_EXISTELEM,
    EM_SOAP_NOEXISTELEM
} CSSOAPRet_E;


/******************************************************************************
 *                                 Global Function Declare                     *
 ******************************************************************************/

/******************************************************************************
Function Name:    CSSOAPCreate 

Description:
        New soap method, which include the method name, the urn, some children element 
     and so on; the handle is the method; The SOAP module may be many method.

Input:
    pcMethod:  the method name, the string is the end of the '\0'.
    pcUrn:     the urn, the method namespace, the string is the end of the '\0'.
    
Output:
    phSoapHandle:  the soap handle.

Return Value:
    SUCCESS:  EM_SOAP_SUCCESS
    FAILURE:  EM_SOAP_FAILURE or other

Other:
*******************************************************************************/
int CSSOAPCreate (char         *pcMethod,
                        char         *pcUrn,
						HCSHANDLE    *phSoapHandle);


/******************************************************************************
Function Name:    CSSOAPAddElement

Description:
        Function Description.

Input:
    hSoapHandle: the soap handle.
    pcType:      the type of element, the string is the end of the '\0', such as: type="s:string";
    pcName:      the name of element, the string is the end of the '\0';
    pcVaule:     the value of element, the string is the end of the '\0';

Output:
    None

Return Value:
    SUCCESS:  EM_SOAP_SUCCESS
    FAILURE:  EM_SOAP_FAILURE or other

Other:
*******************************************************************************/
int CSSOAPAddElement (HCSHANDLE   hSoapHandle,
							  char        *pcName,
                              char        *pcValue,
                              char        *pcType); 

/******************************************************************************
Function Name:    CSSOAPDelElement

Description:
        Function Description.

Input:
    hSoapHandle: the soap handle.
    pcType:      the type of element, the string is the end of the '\0', such as: type="s:string";
    pcName:      the name of element, the string is the end of the '\0';

Output:
    None

Return Value:
    SUCCESS:  EM_SOAP_SUCCESS
    FAILURE:  EM_SOAP_FAILURE or other

Other:
*******************************************************************************/
int CSSOAPDelElement (HCSHANDLE   hSoapHandle,
							  char        *pcName,
                              char        *pcType); 


/******************************************************************************
Function Name:    CSSOAPAddElement

Description:
        Function Description.

Input:
    hSoapHandle: the soap handle.
    pcBuffer:    the buffer.
    dwLen:       the length of the buffer.
Output:
    None

Return Value:
    SUCCESS:  EM_SOAP_SUCCESS
    FAILURE:  EM_SOAP_FAILURE or other

Other:
*******************************************************************************/
int CSSOAPAddContent (HCSHANDLE   hSoapHandle,
							  char        *pcBuffer,
							  DWORD       dwLen); 


/******************************************************************************
Function Name:    CSSOAPAddHeader

Description:
        New http header data, user definition the http header, for example:
     SOAPAction: http://uniinterface.pkit.com.cn/webservices/Authenticate.

Input:
    hSoapHandle: the soap handle.
    pcName:      the name http, for example: "SOAPAction";
    pcValue      the value http header,
              for example: "http://uniinterface.pkit.com.cn/webservices/Authenticate";

Output:
    None

Return Value:
    SUCCESS:  EM_SOAP_SUCCESS
    FAILURE:  EM_SOAP_FAILURE or other

Other:
*******************************************************************************/
int CSSOAPAddHeader(HCSHANDLE      hSoapHandle,
                            char          *pcName,
                            char          *pcValue); 

/******************************************************************************
Function Name:    CSSOAPSend

Description:
        Invode soap, send soap messae, receive the soap response message, and parse it.

Input:
    hSoapHandle:  the soap handle.
    pcUrl:        the URL.
    dwTimeout:    time out, this time is send soap message time or receive soap message time, unit: ms;
Output:
    None

Return Value:
    SUCCESS:  EM_SOAP_SUCCESS
    FAILURE:  EM_SOAP_FAILURE or other

Other:
*******************************************************************************/
int CSSOAPSend(HCSHANDLE     hSoapHandle ,
					 char         *pcUrl,
					 DWORD         dwTimeout);


/******************************************************************************
Function Name:    CSSOAPGetElement

Description:
        Get the value of the element form the soap response message.

Input:
    hSoapHandle: the soap handle.
    pcName:      the name of element, the string is the end of the '\0';
    dwLen:        the length of buffer.

Output:
    pcBuffer:    the buffer, the value of element, the string is the end of the '\0';

Return Value:
    SUCCESS:  EM_SOAP_SUCCESS
    FAILURE:  EM_SOAP_FAILURE or other

Other:
*******************************************************************************/
int CSSOAPGetElement(HCSHANDLE          hSoapHandle,
                             const  char       *pszType,
							 const  char       *pcName,
							 char              *pcBuffer,
							 DWORD              dwLen);


/******************************************************************************
Function Name:    CSSOAPDestory

Description:
        Free the method.

Input:
    hSoapHandle:  the soap handle;

Output:
    None

Return Value:
    SUCCESS:  EM_SOAP_SUCCESS
    FAILURE:  EM_SOAP_FAILURE or other

Other:
*******************************************************************************/
int CSSOAPDestory( HCSHANDLE    hSoapHandle);

#ifdef __cplusplus
}
#endif

#endif  /* CSIP_HTTP_H */

/* End of File */


