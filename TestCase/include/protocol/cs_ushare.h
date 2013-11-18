
#ifndef _CS_USHARE_H_
#define _CS_USHARE_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "mmcp_os.h"


#define CSUSHARE_DIR_PATH_MAX   256        
#define CSUSHARE_FILE_PATH_MAX 256

#define CSUSHARE_DIR_MAX 100   
#define  CSUSHARE_DEVICE_LEN  64  /*UPnP设备名称的最大长度*/
#define CSUSHARE_ETH_LEN   32         /*网卡设备名最大长度*/


typedef enum
{
	EM_USHARE_SUCCESS = 0,
	EM_USHARE_INIT_FAILED,
	EM_USHARE_FILESTAT_FAILED,	
	EM_USHARE_FILEOPEN_FAILED,
	EM_USHARE_FILEREAD_FAILED,
	EM_USHARE_ADDDIR_FAILED,
	EM_USHARE_DELETEDIR_FAILED,
	EM_USHARE_START_FAILED,
	EM_USHARE_STOP_FAILED,
	EM_USHARE_DESTROY_FAILED,
	EM_USHARE_OTHRE_FAILED
}CSUshareRet_E;

typedef struct
{
	char     acEthName[CSUSHARE_ETH_LEN];
	char    acUshareDeviceName[CSUSHARE_DEVICE_LEN];

}CSUshareInfo_S;    

CSUshareRet_E CSUshare_Init(CSUshareInfo_S *pUshareInfo);

CSUshareRet_E CSUshare_Destroy();

CSUshareRet_E CSUshare_AddShareDir(char*pcSharedir,int nDirLen);

CSUshareRet_E CSUshare_DeleteShareDir(char*pcSharedir,int nDirLen);

//BOOL CSUshare_Isshare(char*pcSharedir,int nDirLen);

CSUshareRet_E CSUshare_Start(); 

CSUshareRet_E CSUshare_Stop(void);

#ifdef __cplusplus
}
#endif

#endif

