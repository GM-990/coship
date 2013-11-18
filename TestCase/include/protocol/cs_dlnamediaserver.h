#ifndef _CS_DLNA_MEDIA_SERVER_H_
#define _CS_DLNA_MEDIA_SERVER_H_

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C" 
{
#endif

/**
@brief ����һ��Server
@param[in] pcServerName Server�����ƣ�ASCII�룬����ܳ���256 
@param[in] pcSharedDir ���ӵĹ���Ŀ¼,��'\0'�����ַ���
@return 
- �ɹ�����Server�����
- ʧ�ܷ���NULL��
*/
HCSHANDLE CSDLNAMediaServerCreate(const char * pcServerName, const char *pcSharedDir);

/**
@brief ����һ��Server
@param[in] hServer Server�ľ��
@return 
- �ɹ�����SUCCESS��
- ʧ�ܷ���FAILURE��
*/
int CSDLNAMediaServerDestroy(HCSHANDLE hServer);

/**
@brief ����Server
@param[in] hServer Server���
@return 
- �ɹ�����SUCCESS��
- ʧ�ܷ���FAILURE��
*/
int CSDLNAMediaServerStart(HCSHANDLE hServer);

/**
@brief ֹͣServer
@param[in] hServer Server���
@return 
- �ɹ�����SUCCESS��
- ʧ�ܷ���FAILURE��
*/
int CSDLNAMediaServerStop(HCSHANDLE hServer);

#ifdef __cplusplus
}
#endif

#endif

























