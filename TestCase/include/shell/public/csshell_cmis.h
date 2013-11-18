#ifndef __CMIS_H_
#define __CMIS_H_


#include "udi2_typedef.h"
#include "udiplus_typedef.h"



#ifdef __cplusplus
extern "C"
{
#endif

#define MAX_SECTION (255)
#define MAX_SECTION_DATA (4096)

typedef enum 
{
    CMIS_MSG_POSTER_SHOW,//0
    CMIS_MSG_POSTER_UPDATE,//1
    CMIS_MSG_REQUEST_AD_DATA,//2
    CMIS_MSG_REQUEST_AD_DATA_STOP,//3
    CMIS_MSG_REQUEST_AD_DATA_SUCCESS,//4
    CMIS_MSG_SDT_PARSE_FINISH,//5
}CMIS_Message_E;

typedef enum 
{
    CMIS_SERVICE_TYPE_MAIN = 1,
    CMIS_SERVICE_TYPE_CHARGE_BG,
    CMIS_SERVICE_TYPE_VOD,
    CMIS_SERVICE_TYPE_TVGRAPHIC,
}CMIS_ServiceType_E;

typedef struct _Buffer
{
    BYTE abData[MAX_SECTION_DATA];
    int nLength;
    BOOL bReceived;
}SectionBuffer_S;

typedef enum
{
    POSTER_REQUEST_STATUS_FAILURE,
//    POSTER_REQUEST_STATUS_REQUESTING,
    POSTER_REQUEST_STATUS_SUCCESS,
    POSTER_REQUEST_STATUS_UP_TO_DATE,
} PosterRequestStatus_E;

typedef struct _PicBuffer
{
    CSUDI_HANDLE hSem;
	CMIS_ServiceType_E eType;
    PosterRequestStatus_E eStatus;
    BYTE ucOldVersionNum;
    BYTE ucVersionNum;
    int nFirstSecNum;
    int nTotalSecNum;
    SectionBuffer_S astBuffer[MAX_SECTION];
}PicBuffer_S;

typedef struct{
    int nEventCode;       // NetManagerEvent
    void *nEventData;
    void (*fnFree)(void *pData);
}CMIS_Event_S;

void CMIS_RequestADPictureOfCurrentService();
void CMIS_RequestADPictureOfCurrentServiceStop();

void CMIS_RequestMessageSend(CMIS_Message_E eMessage);
void CMIS_RequestMessageClear();



BOOL CMIS_Init();

//该凼数为非重入凼数,请避免会同时调用的情况
BOOL CMIS_RequestADPicture(BYTE ucVersion,WORD wPid,DWORD dwTimeout);

void CMIS_SetTVNum();
void CMIS_ClearTVNum();

const char *CMIS_GetTVNum();


HCSHANDLE CMIS_FindService(int nTSID,int nONID,int nServiceId);
HCSHANDLE CMIS_FindSpecialService(CMIS_ServiceType_E eType);

int CMIS_DoReadCommand(const char *pcCommand,char *pcValue,int nLength);

BOOL CMIS_DMGetIntValue(const char *pcKey,int *pnValue);
BOOL CMIS_DMSetIntValue(const char *pcKey,int nValue);
BOOL CMIS_DMSave();



#define CMIS_Malloc(dwSize)  _CMIS_Malloc(dwSize,__FILE__,__FUNCTION__,__LINE__)
#define CMIS_Free(pvMem) _CMIS_Free(pvMem,__FILE__,__FUNCTION__,__LINE__)
void *_CMIS_Malloc(DWORD dwSize,const char *pcFileName,const char *pcFunction,DWORD nLine);
BOOL _CMIS_Free(void *pvMem,const char *pcFileName,const char *pcFunction,DWORD nLine);

void CMIS_PosterHide();
BOOL CMIS_PosterShow();
BOOL CMIS_CAMailProcess(const char * pcMessage);



#ifdef WIN32
#define snprintf _snprintf
#endif

#ifdef __cplusplus
}
#endif

#endif //__CMIS_H_
