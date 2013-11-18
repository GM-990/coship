#ifndef _CS_AIRPLAY_H_
#define _CS_AIRPLAY_H_

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C" 
{
#endif

/**@brief  �ص���Ϣ���� */
typedef enum
{
    EM_AIRPLAY_MEDIA_STOP = 0,              ///<ָֹͣ��       ����Ϣֵ(pvPara)�� 
    EM_AIRPLAY_MEDIA_PLAY,                    ///<��������Ƶ��ͼƬָ�� ����Ϣֵ(pvPara) Ϊascii����ַ������͵�JSON�ַ������磺{fileUrl:'%s', metaData:{fileType:2,fileName:''}
    									    ///<fileUrl: Ҫ���ŵ�����Ƶ��ͼƬ��URL��ַ;
    									    ///<fileType�����ŵ���Դ���ͣ�1 �������֣�2������Ƶ��3����ͼƬ
    									    ///<fileName: Ҫ���ŵ��ļ�������Ϊ�ա�
    EM_AIRPLAY_MEDIA_PAUSE,		    ///<��ָͣ��       ����Ϣֵ(pvPara)�� 
    EM_AIRPLAY_MEDIA_RESUME,               ///<�ָ�ָ��       ����Ϣֵ(pvPara):��ʼ���ŵ�λ�ã���λΪ��
    EM_AIRPLAY_MEDIA_SEEK,			    ///<����ָ��       ����Ϣֵ(pvPara)Ϊascii���JSON�ַ������ͣ��磺{time:'%02d:%02d:%02d'}��time�ĵ�λΪ��00��00��00�ĸ�ʽ��ð�����ߵ����ݵ�λΪ��ʱ���֣���
    EM_AIRPLAY_UNKNOWN = 1000
}CSAirplayMediaMsg_E;

/**
@brief    �ص�����
@param[in] eMsg ��Ϣ����, ��CSAirplayMediaMsg_E����
@param[in] pvPara ��Ϣֵ
@param[in] pvUserData �û��Զ������ݣ�ע��ص�����ʱ�����
@return 
- �ɹ�����0��
- ʧ�ܷ���-1
*/
typedef int (*CSOnAirplayAction_F)(CSAirplayMediaMsg_E eMsg, void* pvPara, void* pvUserData);

/**
@brief	 ע��֪ͨ�ص�������
@param[in] fnCb �ص�������
@param[in] pvUserData �û��Զ������ݣ�ע��ص�����ʱ�����
@return 
- �ɹ�����0��
- ʧ�ܷ���-1
*/
int CSAirplayAddCallback(CSOnAirplayAction_F fnCb, void* pvUserData);

#ifdef __cplusplus
}
#endif

#endif


