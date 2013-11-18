/******************************************************************************
FileName:  cs_pvrmgrconfig.h
Copyright (c) 2008�� Shen Zhen Coship Electronics CO.,LTD.��

Version:       Ver 1.0
Author:        luojinhua (luojinhua@coship.com)

�ļ�����:
MP3�������ӿ�

�޸ļ�¼:
Version     Date            Author          Description
--------    ----------    --------          ------------
1.0         2008-01-16      �޽�           Create
1.1         2008-08-25      �޽�           ���Ӷ�mo3����ע�벥�ŵ�֧��
1.2         2009-07-31      903571           ����Ƶ�׷����ӿ�
*******************************************************************************/
#ifndef _CS_MP3_PLAYER_H_
#define _CS_MP3_PLAYER_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "mmcp_typedef.h"

#define  CSMP3_APIC_SIZE 256



typedef enum _GetMp3PicType_E
{
	GETMP3PICTYPE_POINTER,
	GETMP3PICTYPE_PATH,
	GETMP3PICTYPE_NONEED,
	GETMP3PICTYPE_UNKNOW
}GetMp3PicType_E;

typedef void  (*Cb_Mp3Pic_Free)(char *);
/*!
	Mp3��Ϣ
*/
typedef struct _CSMP3Info_S
{
	BYTE m_cTAG[3];		/*! ��ǩ*/	
	BYTE m_cSong[30];	/*! ������(���ļ��н��������ĸ�����)*/
	BYTE m_cArtist[30];	/*! �ݳ�����*/
	BYTE m_cAlbum[30];	/*! ר��*/   
	BYTE m_cYear[4];		/*! �������*/
	BYTE m_cComment[30];/*!��ע*/      
	BYTE m_cGenre;
	DWORD m_dwTimeLen;/*!�����ɲ��ų���*/
	int m_nNumberOfCh; //������
	UINT m_unSampRate;  //������
    BYTE    m_ApicPath[CSMP3_APIC_SIZE];      /*MP3 ���������·��*/
	
	char                  *m_pcPicStr;
	unsigned int         m_nPicSize;
	Cb_Mp3Pic_Free   m_fnPicFree;
} CSMP3Info_S;

/*!
	�ص�֪ͨ����
*/
typedef enum _MP3NotifyType_E
{
	EM_MP3_PLAY_END = 0,  /*��ʾ�������Ž���*/
	EM_MP3_PLAY_INVALID_FILE ,   /*��ʾ�����������ܲ��ţ������粻��MP3�ļ�*/
	EM_MP3_PLAY_INTERRUPT, /*��ʾ�������Ź�����ֹͣ����U�̵��ˣ��ļ������𻵵�*/
	EM_MP3_PLAY_FLUSH_FINISH,    //��ʽ����flush���
	EM_MP3_PLAY_UNKOWN/*����*/
}MP3NotifyType_E;

/*!
	�ص�֪ͨ��������
*/
typedef void (*MP3PlayerNotify_F)(HCSHANDLE hMp3Player,MP3NotifyType_E eType,DWORD dwNotifyData,void* pUserData);

/*!
	��������MP3�ļ�
@param [in] hMp3Player ������������
@param [in] szFile ��Ҫ���ŵ�mp3�ļ�
\return �����ɹ�����TRUE, ���򷵻�FALSE
\note �������ڻ�����һ���첽�߳����ڲ���
*/
BOOL CSMP3PlayerStart(HCSHANDLE hMp3Player, const char *szFile);

/*!
	����MP3����������
@param [in] fnCb �������ص���Ӧ����(����Ϊ��)
@param [in] pUserData �û��Զ������ݣ��ص�ʱ��Ϊ�����ش����û�
\return �����ɹ��򷵻ز�������������򷵻�NULL
\note ���������Ĳ���������ȿ�����mp3�ļ����ţ�Ҳ��������mp3����ע�벥��
*/
HCSHANDLE CSMP3PlayerCreate(MP3PlayerNotify_F fnCb, void *pUserData);

/*!
	ֹͣMP3����
@param [in] hMp3Player ������������
\return �����ɹ�����TRUE, ���򷵻�FALSE
\note ֹͣ������ͬ��ֹͣ�ģ�������ȵ��첽�����߳̽����ŷ���
*/
BOOL CSMP3PlayerStop(HCSHANDLE hMp3Player);

/*!
	��ͣMP3����
@param [in] hMp3Player ������������
\return �����ɹ�����TRUE, ���򷵻�FALSE
*/
BOOL CSMP3PlayerPause(HCSHANDLE hMp3Player);

/*!	
	�ָ�MP3����
@param [in] hMp3Player ������������
\return �����ɹ�����TRUE, ���򷵻�FALSE
*/
BOOL CSMP3PlayerResume(HCSHANDLE hMp3Player);


/* �����ӿ�
    +--------------------------------------------------------------------------------------------------
    ������� bHaspic ��Ϊ1ʱ������psOutInfo �ڵ�ͼƬָ�����ݣ�ͬʱ����һ���ͷ�
               ��ָ��ĺ�������ʹ�ø�ͼƬָ��󣬵��øú����ͷŸ�ָ�롣
               
   [ע��] 

   ������� bHaspic ��Ϊ0ʱ������psOutInfo �ڵ�ͼƬ·������·��Ӧ��Ϊ�ڴ�ռ�
               �ļ�ϵͳ���Ա�֤MP3����ͼƬ�Ľ����ٶȡ�  
               
             ��ע��  APIC_JPG_PATH ��APIC_PNG_PATH �����·��

  \return �����ɹ�����TRUE, ���ⷵ��FALSE
     --------------------------------------------------------------------------------------------------+*/
BOOL CSMP3PlayerGetMp3InfoWithPic(const char *szFile,CSMP3Info_S *psOutInfo, GetMp3PicType_E eGetpic);

/*!
	��ȡMP3�ļ���Ϣ
@param [in] szFile mp3�ļ�
@param [out] psOutInfo �ļ���mp3��Ϣ
\return �����ɹ�����TRUE, ���򷵻�FALSE
*/
BOOL CSMP3PlayerGetMp3Info(const char *szFile,CSMP3Info_S *psOutInfo);

/*!
	��ȡ��ǰ����λ��
@param [in] hMp3Player ������������
\return ��ǰ����λ��
\note ������δ����ʱ,����0
*/
DWORD CSMP3PlayerGetTime(HCSHANDLE hMp3Player);

/*!
	��ȡ��ǰ���ڲ��ŵ��ļ���·��
@param [in] hMp3Player ������������
\return ���ز��������ڲ��ŵ��ļ�·��
\note ����mp3�ļ���������
\note ���������δ��������mp3����ע�벥��ʱ����NULL
*/
const char* CSMP3PlayerGetFilePath(HCSHANDLE hMp3Player);

/*!
	����mp3����ע�벥��
@param [in] hMp3Player ������������
\return �����ɹ�����TRUE, ���򷵻�FALSE
\note �����ڻ�����һ���첽�����߳�
\note ��ÿ�β����µ�һ��mp3����ʱ,���ñ�������������mp3����ע�벥��
*/
BOOL CSMP3PlayerStartInject(HCSHANDLE hMp3Player);

/*!
	Mp3����ע��
@param [in] hMp3Player ������������
@param [in] pMp3Buffer mp3����
@param [in] nBufLen ע���mp3���ݳ���
\note mp3����ע��ǰ�ȵ���CSMP3PlayerGetFreeSize��ȡ��ע������ݳ���
\note ÿ��ע������ݳ��Ȳ��ô��ڲ�������ע��ĳ���
\note �µ�һ��mp3��������ע��ǰ,���ȵ���CSMP3PlayerStartInject��������mp3ע�벥��
\note ���������mp3������������ע��
\note �������ǷǶ�����,��������
\note ����ֵ����ע��ɹ����
*/
BOOL CSMP3PlayerInject(HCSHANDLE hMp3Player, BYTE *pMp3Buffer, int nBufLen);

/*! 
	��ȡ��ע������ݳ���
@param [in] hMp3Player ������������
\return ��������ע������ݳ���
\note ���ӿڽ���mp3����ע�벥������
\note ���������δ��������mp3�ļ�����,�򷵻�0
*/
int CSMP3PlayerGetFreeSize(HCSHANDLE hMp3Player);

/*!
	���Ŷ�λ
@param [in] hMp3Player ������������
@param [in] dwSec ��Ҫ��λ��Ŀ��λ��( ��λΪ��)
\return �����ɹ�����TRUE, ���򷵻�FALSE
\note Ŀǰ���ӿڽ�������ԴΪ�ļ��ķ�ʽ����
\note �ļ�seek������ǰ���ȵ���CSMP3PlayerGetMp3Info()�������ʱ�䣬�Ա㴫��һ���Ϸ���ʱ�䡣
\note ע��ú������첽ִ�еģ�������seek���ڶ��ļ��߳���ʵ�ֵ�
*/
BOOL CSMP3PlayerSeek(HCSHANDLE hMp3Player,DWORD dwSec);

/*!
	��ð汾��
@param [in] pcVersion ���հ汾�ִ���buffer
@param [in] nSize buffer���ô�С
\return �����ɹ�����ʵ�ʿ����ɹ�������
*/
int CSMP3GetVersion( char * pcVersion, int nSize );

/*!
   �ò�������������������ȫ�������꣬�������ݲ������һ����ϢEM_MP3_PLAY_FLUSH_FINISH
@param[in]     hMp3Player   ��Ч�Ĳ��������
\return: ����ִ�гɹ����
\note: ������ֻӦ������ʽ���ţ��ļ����Ų�Ӧ��ʹ�øýӿ�
\note: ���������첽ִ�У�����ֵ�ɹ����������ݲ��ֻ꣬��Ӧ����ע��Ļص��������յ�EM_MP3_PLAY_FLUSH_FINISHʱ���ݲ�����������
\note: ����ʽ����ʱһ���øú�����ȷ��һ�׸���������ɣ������л�����һ�׸���
\note: ֻ�����ڲ��ŵ�״̬�²���flush,���������״̬�£�flush��Ч
*/
BOOL CSMP3PlayerFlush(HCSHANDLE hMp3Player);

/*!
	���һ���ص�
@param[in]    hMp3Player   ��Ч�Ĳ��������
@param [in]   fnCb �ص�����
@param [in]   pUserData �û��Զ������ݣ��ص�ʱ��Ϊ�����ش����û������Դ���
\return ��ӳɹ�����TRUE��ʧ��FALSE
\note  �ص��������û��������߽��ȷ��Ψһ�Ļص���ֻҪ����һ����һ������Ϊ���µĻص�
\note  ͬһ���ص��ٴ�ע�᷵��ʧ��
\note  pUserData���û�������ڴ棬ͨ������Ҫȫ�ֱ��棬�������ڲ���������
\note  ����Ѵ����ص�������������ӣ��µ���ӻ�ʧ��
*/
BOOL CSMP3PlayerAddCallback(HCSHANDLE hMp3Player, MP3PlayerNotify_F fnCb, void *pUserData);

/*!
	�Ƴ�һ���ص�
@param[in]    hMp3Player   ��Ч�Ĳ��������
@param [in]   fnCb �ص�����
@param [in]   pUserData �û��Զ������ݣ����Դ���
\return ��ӳɹ�����TRUE��ʧ��FALSE
\note  �ص��������û��������߽��ȷ��Ψһ�Ļص���ֻҪ����һ����һ������Ϊ�ǲ�ͬ�Ļص�
\note  �Ƴ������ڵĻص�����ʧ��
\note  pUserData��fnCb�������ע��ʱ��ͬ
*/
BOOL CSMP3PlayerRemoveCallback(HCSHANDLE hMp3Player, MP3PlayerNotify_F fnCb, void *pUserData);

/*!
	����һ��mp3������
@param[in]    hMp3Player   ��Ч�Ĳ��������
*/
void CSMP3PlayerDestroy(HCSHANDLE hMp3Player);
/*!
��ȡ��ǰ����ʹ�õ�MP3���
return ��ǰMP3�ľ��
*/

HCSHANDLE  CSMP3PlayerGetUsedHandle();
/*!
��ȡ��ǰMP3 ��ʹ��������DEVICE ID
@param[in]    hMp3Player   ��Ч�Ĳ��������
*/
int  CSMP3PlayerGetDeviceId(HCSHANDLE hMp3Player);

/*!
	��ȡHTTP ע�벥��MP3�ļ���duration �͵�ǰ����ʱ��
@param [in] http mp3�ļ�locator
@param [in] nHTTPFilePos �ļ���mp3����λ��
\return mp3����ʱ���duration����λ����
*/
DWORD CSMP3PlayerGetHttpMp3TimeInfo(const char *pcURL,int nHTTPFilePos);

/*!
	��ȡHTTP ע�벥��MP3�ļ��ĵ�ǰ����ʱ��
@param [in] hMp3Player   ��Ч�Ĳ��������
\return mp3����ʱ�䵥λ����
*/
DWORD CSMP3PlayerGetHttpMp3CurTime(HCSHANDLE hMp3Player);

/*!
	http����MP3����ʱ���ȡ��Ӧ�ļ�λ��
@param [in] http mp3�ļ�locator
@param [in] dwSec ��Ҫ��λ��Ŀ��ʱ��(��λΪ��)
\return Ŀ��ʱ���Ӧ���ļ�λ��(��λΪByte)
\note ���ӿڽ�������ԴΪhttp�ļ��ķ�ʽ����
\note ���ñ��ӿ�ǰ��ͨ����ؽӿ��������ʱ�䣬�Ա㴫��һ���Ϸ���ʱ�䣬��������ʱ�䳬����ʱ���򷵻ؽ��δ֪
*/
int CSMP3PlayerHttpMp3GetPosByTime(const char* pcUrl, DWORD dwSec);

/*!
	��շ�MP3ע�벥����ػ�����
@param [in] hMp3Player ���Ŷ�����
\return �����ɹ�����TRUE, ���򷵻�FALSE
\note ���ӿ���յĻ������������뻺������PCMע�뻺����
*/
BOOL CSMP3PlayerResetBuffer(HCSHANDLE hMp3Player);

#ifdef __cplusplus
}
#endif

#endif

