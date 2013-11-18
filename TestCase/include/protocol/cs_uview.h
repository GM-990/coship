#ifndef _CS_UVIEW_H_
#define _CS_UVIEW_H_

#ifdef __cplusplus
extern "C" {
#endif


typedef enum
{
EM_UVIEW_SUCCESS=0,	
EM_UVIEW_LOGINIT_FAILED,                       /*��־��ʼ��ʧ��*/
EM_UVIEW_CHARSETINIT_FAILED,             /*�ַ������ʼ��ʧ��*/ 
EM_UVIEW_FILESYSTEM_FAILED,               /*�ļ�ϵͳ��ʼ��ʧ��*/
EM_UVIEW_FINDDEVICE_FAILED ,            /*�Ҳ����豸*/
EM_UVIEW_STOP_FAILED ,                         /*ж���豸ʧ��*/
EM_UVIEW_OTHER_FAILED                    /*��������*/
}CSUVIEWRet_E;


int CS_UViewStart(char *pcRootDir, int  nDirLen);

int CS_UViewStop();

#ifdef __cplusplus
}
#endif

#endif

