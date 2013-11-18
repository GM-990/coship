#ifndef _CS_UVIEW_H_
#define _CS_UVIEW_H_

#ifdef __cplusplus
extern "C" {
#endif


typedef enum
{
EM_UVIEW_SUCCESS=0,	
EM_UVIEW_LOGINIT_FAILED,                       /*日志初始化失败*/
EM_UVIEW_CHARSETINIT_FAILED,             /*字符编码初始化失败*/ 
EM_UVIEW_FILESYSTEM_FAILED,               /*文件系统初始化失败*/
EM_UVIEW_FINDDEVICE_FAILED ,            /*找不到设备*/
EM_UVIEW_STOP_FAILED ,                         /*卸载设备失败*/
EM_UVIEW_OTHER_FAILED                    /*其他错误*/
}CSUVIEWRet_E;


int CS_UViewStart(char *pcRootDir, int  nDirLen);

int CS_UViewStop();

#ifdef __cplusplus
}
#endif

#endif

