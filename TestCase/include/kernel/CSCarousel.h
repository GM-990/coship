#ifndef __CS_CRSL_H
#define __CS_CRSL_H

#ifdef  __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"

BOOL CSCRSL_GetPathByLocator(const char * szLocator,char * szOutFsPath);

BOOL CSCRSL_LoadCarousel(WORD wOnId,WORD wTsId ,WORD wSrvId ,
	BYTE ucComponentTag,DWORD dwCarouselId, char * szOutFsPath);
BOOL CSCRSL_LoadCarouselEx(WORD wOnId,WORD wTsId ,WORD wSrvId ,
	BYTE ucComponentTag, DWORD dwCarouselId,char * szStoragePath, char * szOutFsPath);
	
BOOL CSCRSL_LoadCarouselByLocator(const char * szLocator, DWORD dwCarouselId,char * szMountPointh);
BOOL CSCRSL_LoadCarouselByLocatorEx(const char * szLocator,DWORD dwCarouselId, char * szStoragePath,char * szOutFsPath);

BOOL CSCRSL_UnloadCarousel(const char * szFsPath);


#ifdef  __cplusplus
}
#endif

#endif


