#ifndef _CS_DLNA_MEDIA_RENDER_H_
#define _CS_DLNA_MEDIA_RENDER_H_

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C" 
{
#endif

typedef enum 
{
    EM_MEDIARENDER_PLAY = 0,				///<����ָ��       ����Ϣֵ(pPara):�����ļ���URL,�䳤�ȿ�ͨ��strlen((char *)pvPara)���������
    EM_MEDIARENDER_PAUSE,					///<��ָͣ��       ����Ϣֵ:��         
    EM_MEDIARENDER_STOP,					///<ָֹͣ��       ����Ϣֵ:��         
    EM_MEDIARENDER_SEEK,					///<����ָ��       ����Ϣֵ(pPara):��ʼ���ŵ�λ�ã���λΪ��      
    EM_MEDIARENDER_NEXT,					///<��һ����Ŀָ��       ����Ϣֵ(pPara):��һ��ĿURL
    EM_MEDIARENDER_PREVIOUS,				///<��һ����Ŀָ��       ����Ϣֵ(pPara):��һ��ĿURL        
    EM_MEDIARENDER_SETMUTE,				///<����ָ��       ����Ϣֵ:��         
    EM_MEDIARENDER_SETVOLUME,				///<����ָ��       ����Ϣֵ(pPara):����ֵ �û���Ҫ���������ֵ����[0,31]���������㣻
    EM_MEDIARENDER_UNKNOWN = 1000			///<δָ֪��
}CSDLNAMediaRenderMsg_E;


/**
@brief    �ص�������
@param[in] hController  Controller���
@param[in] eMsg ��Ϣ����
@param[in] pPara ��Ϣֵ�����ֲ�ͬ����Ϣ��������Ϣֵ��ͬ�����CSDLNAMediaRenderMsg_E����
@param[in] pUserData �û��Զ������ݣ�ע��ص�����ʱ�����
@return �ޡ�
*/
typedef int (*CSOnDLNAMediaRenderAction_F)(HCSHANDLE hRender, CSDLNAMediaRenderMsg_E eMsg, void* pPara, void* pUserData);

/**
@brief ����һ��Renderer
@param[in] pcRenderName Render������
@return 
- �ɹ�����Renderer�����
- ʧ�ܷ���NULL��
*/
HCSHANDLE CSDLNAMediaRenderCreate(char * pcRenderName);

/**
@brief ����һ��Render
@param[in] hRender Render�ľ��
@return 
- �ɹ�����SUCCESS��
- ʧ�ܷ���FAILURE��
*/
int CSDLNAMediaRenderDestroy(HCSHANDLE hRender);

/**
@brief ����Render
@param[in] hRender Render���
@return 
- �ɹ�����SUCCESS��
- ʧ�ܷ���FAILURE��
*/
int CSDLNAMediaRenderStart(HCSHANDLE hRender);

/**
@brief ֹͣRender
@param[in] hRender Render���
@return 
- �ɹ�����SUCCESS��
- ʧ�ܷ���FAILURE��
*/
int CSDLNAMediaRenderStop(HCSHANDLE hRender);

/**
@brief ֹͣRender
@param[in] hRender Render���
@param[in] pcName Render���ƣ��ַ���
@return 
- �ɹ�����SUCCESS��
- ʧ�ܷ���FAILURE��
*/
int CSDLNAMediaRenderSetName(HCSHANDLE hRender, char* pcName);

/**
@brief	 ע��֪ͨ�ص�������
@param[in] fnCb �ص�������
@param[in] pUserData �û��Զ������ݣ�ע��ص�����ʱ�����
@return 
- �ɹ�����SUCCESS��
- ʧ�ܷ���FAILURE��
*/
int CSDLNAMediaRenderAddCallback(CSOnDLNAMediaRenderAction_F fnCb, void* pUserData);

/**
@brief	ɾ���ص�������
@param[in] fnCb �ص�������
@param[in] pUserData �û��Զ������ݣ�ע��ص�����ʱ�����
@return 
- �ɹ�����SUCCESS��
- ʧ�ܷ���FAILURE��
*/
int CSDLNAMediaRenderRemoveCallback(CSOnDLNAMediaRenderAction_F fnCb, void* pUserData);


#ifdef __cplusplus
}
#endif

#endif

