/******************************************************************************
FileName:  PCM_SIN.H
Copyright (c) 2010£¬ Shen Zhen Coship Electronics CO.,LTD.¡£

Version:       Ver 1.0
Author:        wuzhengbing (wuzhengbing@coship.com)

File Description:
        File Description, Please add you description here.

Modify History:
Version     Date        Author          Description
--------    ----------  --------        ------------
1.0         2010-10-08  wuzhengbing     Create
*******************************************************************************/

#ifndef PCM_SIN_H
#define PCM_SIN_H

#include "udi2_inject.h"

/******************************************************************************
 *                                 Header File Include                                 *
 ******************************************************************************/

/******************************************************************************
 *                                 Macro/Define/Structus                           *
 ******************************************************************************/

/******************************************************************************
 *                                 Global Function Declare                                   *
 ******************************************************************************/
CSUDI_HANDLE CSPcmSin_Open(const CSUDIINJECTERPcmStartParam_S *pStartParam, CSUDIINJECTERPcmEndian_E eEndian, int nFreq);

int CSPcmSin_Read(CSUDI_HANDLE hPcmSin, void* pBuffer, int nBufSize);

void CSPcmSin_Close(CSUDI_HANDLE hPcmSin);

#endif  /* PCM_SIN_H */

/* End of File */

