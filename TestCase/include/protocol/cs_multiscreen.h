/**
Copyright (c) 2005,ShenZhen Coship Electronic Ltd Co.
All rights reserved.

File Name: cs_mulscreen.h
Summary: �㶫ʡ����������ͷ�ļ�
Current Version: 1.0
Author: Zheng Fen

History:
1.0.1 2012/04/11 1����
*/

#ifndef _MUL_SCREEN_H_
#define _MUL_SCREEN_H_


#ifdef  __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"

/**
@brief ��������ģ��ʹ�õĴ����豸
*/
typedef enum  
{
    EM_MULTISCREEN_TANSDEVTYPE_BLUETOOTH ,  /*�豸����:����*/
    EM_MULTISCREEN_TANSDEVTYPE_ETHERNET , 	/*�豸����:���߻���������̫�������豸*/
    EM_MULTISCREEN_TANSDEVTYPE_COM			/*�豸����:com��*/
} CSMultiScreenDevType_E;


/**
@brief ��������ģ��ָ������
*/
typedef enum  
{
	EM_MULTISCREEN_INSTRUCT_MATCH , 					/*ָ������:ƥ��ָ��*/
	EM_MULTISCREEN_INSTRUCT_KEYPRESS , 					/*ָ������:����ָ��*/
	EM_MULTISCREEN_INSTRUCT_INPUT,						/*ָ������:���뷨ָ��*/
	EM_MULTISCREEN_INSTRUCT_SENSOR,					/*ָ������:ģ�⴫����ָ��*/
	EM_MULTISCREEN_INSTRUCT_MOUSE,						/*ָ������:���ָ��*/
	EM_MULTISCREEN_INSTRUCT_MTOSTBSWITCH,				/*ָ������:�ֻ������������ָ��*/
	EM_MULTISCREEN_INSTRUCT_STBTOMSWITCH,				/*ָ������:���������ֻ�����ָ��*/

	EM_MULTISCREEN_INSTRUCT_ANSWERMATCH ,  			/*ָ������:ƥ��ָ���Ӧ��ָ��*/
	EM_MULTISCREEN_INSTRUCT_ANSWERKEYPRESS ,  			/*ָ������:����ָ���Ӧ��ָ��*/
	EM_MULTISCREEN_INSTRUCT_ANSWERINPUT ,  			/*ָ������:���뷨ָ���Ӧ��ָ��*/

	EM_MULTISCREEN_INSTRUCT_ANSWERSENSOR,
	EM_MULTISCREEN_INSTRUCT_ANSWERMOUSE ,  			/*ָ������:���ָ���Ӧ��ָ��*/
	EM_MULTISCREEN_INSTRUCT_ANSWERMTOSTBSWITCH ,  	/*ָ������:����(�ֻ��������)ָ���Ӧ��ָ��*/
	EM_MULTISCREEN_INSTRUCT_ANSWERSTBTOMSWITCH ,  	/*ָ������:����(���������ֻ�)ָ���Ӧ��ָ��*/
	  	
	EM_MULTISCREEN_INSTRUCT_OTHER						/*����ָ��*/

} CSMultiScreenInstructType_E;


typedef struct  
{
	CSMultiScreenInstructType_E 	eInstructType;	/*ָ�����*/
	unsigned int  		m_unSyn;    					/*ͬ����ʶ*/
	unsigned char   	m_acMobileId[32];			/*�ƶ��ն˱�ʶ��*/
	unsigned int 		m_unLen;					/*�������ݳ���*/
	char*  			m_pcAdditionalData;    		/*��������*/
} CSMultiScreenPacket_S;


/**
@brief ����CSMultiScreenAddCallback��CSMultiScreenRemoveCallback��ʱ��, ��Ҫ��������͵ĺ���ָ��
@param[in] sPacketInfo:��ʾ��ǰ�յ�����Ϣֵ
@param[in] pvUserData:���������û�����
*/
typedef void  (*CSOnMultiScreenMsg_F)(CSMultiScreenPacket_S *sPacketInfo, void * pvUserData);

/**
@brief 	������������ʵ��
@param[in]  eDeviceType: ��������ģ��ʹ�õĴ����豸,���嶨���CSMultiScreenDevType_E
@param[in] nPort:  �������������˿�
@return   ���ش�������������ʵ�����,ΪNULL��ʾδ�����ɹ�
*/
HCSHANDLE CSMultiScreenCreate(CSMultiScreenDevType_E eDevType, int nPort);

/**
@brief 	����������������
@param[in]  hMultiScreen: CSMultiScreenCreate��������������������ʵ�����
@return   �ɹ�CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
*/
CSUDI_Error_Code CSMultiScreenStart(HCSHANDLE hMultiScreen);

/**
@brief 	ֹͣ������������
@param[in]  hMultiScreen: CSMultiScreenCreate��������������������ʵ�����
@return   �ɹ�CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
*/
CSUDI_Error_Code CSMultiScreenStop(HCSHANDLE hMultiScreen);

/**
@brief 	������������ʵ��
@param[in]  hMultiScreen: CSMultiScreenCreate��������������������ʵ�����
@return   �ɹ�CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
*/
CSUDI_Error_Code CSMultiScreenDestroy(HCSHANDLE hMultiScreen);

/**
@brief 	����������������
@param[in]  hMultiScreen: CSMultiScreenCreate��������������������ʵ�����
@param[in] psPacketInfo:Ҫ���͵ı���
@return   �ɹ�CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
*/
CSUDI_Error_Code CSMultiScreenSend(HCSHANDLE hMultiScreen, CSMultiScreenPacket_S *psPacketInfo);

/**
@brief �����������֪ͨ�ص�����

@param[in] hMultiScreen CSMultiScreenCreate��������������������ʵ�����
@param[in] pCallback �ص�����,����������ͨ��ģ��״̬�����仯ʱ���øú���
@param[in] pvUserData �û�����,����Ϊ�ص�������pvUserData��������
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ���CSUDI_FAILURE
@note ��ӻص�ǰ������״̬�仯���Ჹ��֪ͨ
*/
CSUDI_Error_Code CSMultiScreenAddCallback(HCSHANDLE hMultiScreen, CSOnMultiScreenMsg_F pCallback, void* pvUserData);

/**
@brief ɾ�����������ص�����

@param[in] hMultiScreen CSMultiScreenCreate��������������������ʵ�����
@param[in] pCallback �ص�����,֮ǰע��ĺ���
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ���CSUDI_FAILURE
*/
CSUDI_Error_Code CSMultiScreenRemoveCallback(HCSHANDLE hMultiScreen, CSOnMultiScreenMsg_F pCallback);


#ifdef __cplusplus
}
#endif


#endif

