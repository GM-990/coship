/************************************************************************
 * Copyright (c) 2005, SHENZHEN COSHIP ELECTRONICS CO., LTD.
 * All rights reserved.
 * 
 * File Name: tftpclient.h
 * Note: PC DEBUG Version.
 *
 ************************************************************************/

#ifndef _CS_TFCTPCLIENT_H_
#define _CS_TFCTPCLIENT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"

typedef void (*tftp_callback_t) (int nCurrentBytes, int nTotalBytes, int* pnCancelFlag,BOOL bFinished);

/** macros of types that functions returns definition */
#define TFTP_SUCCESS			 		0
#define FAILED_TO_GET_IP			-1
#define FAILED_TO_GET_FILENAME		-2
#define FAILED_TO_CREATE_SOCKET		-3
#define FAILED_TO_GET_SVR_ADDR		-4
#define FAILED_TO_BIND				-5
#define FAILED_TO_GET_FILE    		-7
#define FAILED_INVALID_PARAMETER	-8

/** macros of types that tftp packet type */
#define TFTP_REQUEST_RRQ			1
#define TFTP_REQUEST_WRQ			2
#define TFTP_DATA					3
#define TFTP_ACK					4
#define TFTP_ERROR					5

#define TFTP_MODE_OCTET 			0
#define TFTP_MODE_NETASCII			1


int CSTftpXfer( const char* pHost,			/** host name or address */
		const char* pFilename,				/** remote filename */
		const unsigned char ucCommand,		/** TFTP command, 0: get, 1: put  */
		const unsigned char ucMode,			/** TFTP transfer mode, 0: binary, 1: ascii */
		unsigned char* ppData,				/** receive buffer pointer */
		const unsigned int* pdwTrans,        /** transfer data length*/
		tftp_callback_t pCall_Back,          
		unsigned long ulTimeOut);             /** transfer timeout*/




#ifdef __cplusplus
}
#endif

#endif
