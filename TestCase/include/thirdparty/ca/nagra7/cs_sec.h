/******************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*              All rights reserved.
* Filename:    cs_sec.h
* Author:      zhangxing
* Date:        2011 12
* Description: 
*******************************************************************************/
#include "udiplus_typedef.h"
#include "CS_OS.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/*******************************************************************************
* 			                Revision History                                						     *
********************************************************************************
* - Revision 1.0  2011/11
*   Programmer:zhangxing
*   Create.
*******************************************************************************/
#ifndef _CS_SEC_H_
#define _CS_SEC_H_

#ifdef  __cplusplus
extern "C" {
#endif

typedef unsigned char SECNuid[4];

INT CSSECInitialize(void);

INT CSSECTerminate(void);

INT CSSECGetNuid(SECNuid *pxNuid);

INT CSSECGetChipsetRevision(const char** ppxChipsetRevision);

INT CSSECEncryptData(BYTE* pxDst, const BYTE* pxSrc, INT xDataLength);

INT CSSECDecryptData(BYTE* pxDst, const BYTE* pxSrc, INT xDataLength);

INT CSSECSet1LevelProtectedKeys(DWORD dwDescrambleID,
											  BYTE *pxL1ProtectingKey, BYTE xL1ProtectingKeyLength,
											  BYTE *pxOddProtectedKey, BYTE xOddProtectedKeyLength,
											  BYTE *pxEvenProtectedKey, BYTE xEvenProtectedKeyLength);

#ifdef  __cplusplus
}
#endif

#endif //_CS_SEC_H_
