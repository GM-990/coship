#ifndef sipmt_H
#define sipmt_H

#ifdef PRAGMA
#pragma interface "sipmt.h"
#endif

// {{{USR
#ifdef __cplusplus
extern "C" 
{
#endif

#include "mmcp_typedef.h"

#include "SICommonType.h"


/*
 *	��Ƶ�ڵ�
 */
typedef struct _SIAudioNode
{
	BYTE bAudioType;					// ��Ƶ���ͣ��ο� iso-13818
	WORD wAudioPID;						//����Ƶ���� pid
	DWORD dwLanguage;					//��Ƶ���ԣ�����������Ϊ0������ο� iso_639 

} SIAudioInfo;


/*
������ SIPMTMessageCallback
	�ص����������ܱ�����,����������

	void(*SIPMTMessageCallback)(
		NOTIFY_TYPE enmType,
		WORD wServiceID
	);	

Parameters
	
	enmType
		[out]ö�� ��ʾ�ص�״̬�� COMPLETE:���,TIMEOUT:��ʱ,ABORT:�û�ȡ��������stop��
	wServiceID
		[out]�������service id

Return Values
		��
*/
typedef void (*SIPMTMessageCallback)(NOTIFY_TYPE enmType, WORD wServiceID);


/*
������ SIPMT_CreateInstance
	����pmt����ʵ�����ú���Ӧ����������֮ǰ����

	void SIPMT_CreateInstance();	

Parameters
	
	��

Return Values

HCSHANDLE:  �����ɹ�����ʵ���ľ����
            ʧ�ܷ���ΪNULL��
	
*/
HCSHANDLE SIPMT_CreateInstance();


/*
������ SIPMT_RegisterCallback
	ע��ص��������ú���Ӧ����SIPMT_CreateInstance֮��

	void SIPMT_RegisterCallback(
		HCSHANDLE hInstance,
		SIPMTMessageCallback callback
	);	

Parameters
	
	hInstance
		[in] pmt����ʵ���ľ�� SIPMT_CreateInstance������
	callback
		[in] �ص������ĵ�ַ

Return Values

HCSHANDLE:  �����ɹ�����ʵ���ľ����
            ʧ�ܷ���ΪNULL��
	
*/
void SIPMT_RegisterCallback(HCSHANDLE hInstance, SIPMTMessageCallback callback);


/*
������ SIPMT_StartSearch
	��ʼpmt����

	void SIPMT_StartSearch(
		HCSHANDLE hInstance,
		WORD wPMTPID,
		WORD wServiceID,
		DWORD dwTimeout,
		DWORD dwDemuxID
	);	

Parameters
	
	hInstance
		[in] pmt����ʵ���ľ�� SIPMT_CreateInstance������
	wPMTPID
		[in] Ҫ������service ��PMT_ID
	wServiceID
		[in] Ҫ������service ��SEVICE_ID
	dwTimeout
		[in] ��ʱʱ��
	dwDemuxID
		[in] demux�豸��ID����ֻ��һ��demux�豸�������ó� 0

Return Values

BOOL: ���������ɹ�����ΪTRUE
      ��������ʧ�ܷ���ΪFALSE
	
*/
BOOL SIPMT_StartSearch(HCSHANDLE hInstance, WORD wPMTPID, WORD wServiceID, DWORD dwTimeout, DWORD dwDemuxID );


/*
������ SIPMT_Stop
	ֹͣpmt����

	void SIPMT_Stop(
		HCSHANDLE hInstance,
	);	

Parameters
	
	hInstance
		[in] pmt����ʵ���ľ�� SIPMT_CreateInstance������

Return Values

BOOL: ֹͣ�����ɹ�����ΪTRUE
      ֹͣ����ʧ�ܷ���ΪFALSE
	
*/
BOOL SIPMT_Stop(HCSHANDLE hInstance);


/*
������ SIPMT_GetFirstAudio
	�õ���һ����Ƶ����Ϣ

	void SIPMT_GetFirstAudio(
		HCSHANDLE hInstance,
		SIAudioInfo* pAudio
	);	

Parameters
	
	hInstance
		[in] pmt����ʵ���ľ�� SIPMT_CreateInstance������
	pAudio
		[out]�����Ƶ��Ϣ�Ľṹ��ĵ�ַ

Return Values

HCSHANDLE:  �ɹ����ص�һ����Ƶ��Ϣ�ľ��
          NULL��ʾ�ò���û�еõ���Ƶ��Ϣ��
	
*/
HCSHANDLE SIPMT_GetFirstAudio(HCSHANDLE hInstance, SIAudioInfo* pAudio);


/*
������ SIPMT_GetNextAudio
	�õ���һ����Ƶ����Ϣ

	void SIPMT_GetNextAudio(
		HCSHANDLE hInstance,
		HCSHANDLE hPre,
		SIAudioInfo* pAudio
	);	

Parameters
	
	hInstance
		[in] pmt����ʵ���ľ�� SIPMT_CreateInstance������
	hPre
		[in] ǰһ����Ƶ���ľ��
	pAudio
		[out]�����Ƶ��Ϣ�Ľṹ��ĵ�ַ

Return Values

HCSHANDLE:  �ɹ�������һ����Ƶ��Ϣ�ľ��
          NULL��ʾ�ò���û�еõ���Ƶ��Ϣ��
	
*/
HCSHANDLE SIPMT_GetNextAudio(HCSHANDLE hInstance, HCSHANDLE hPre, SIAudioInfo* pAudio);


/*
������ SIPMT_GetFirstStream
	�õ�pmt�е�һ��������Ϣ

	void SIPMT_GetFirstStream(
		HCSHANDLE hInstance,
		SIMpegInfo* pStream)
	);	

Parameters
	
	hInstance
		[in] pmt����ʵ���ľ�� SIPMT_CreateInstance������
	pStream
		[out]�������Ϣ�Ľṹ�����ĵ�ַ

Return Values

HCSHANDLE:  �ɹ����ص�һ������Ϣ�ľ��
          NULL��ʾ�ò���û�еõ���Ƶ��Ϣ��
	
*/
HCSHANDLE SIPMT_GetFirstStream(HCSHANDLE hInstance, SIMpegInfo* pStream);


/*
������ SIPMT_GetNextStream
	�õ�pmt����һ��������Ϣ

	void SIPMT_GetNextStream(
		HCSHANDLE hInstance,
		HCSHANDLE hPreStream,
		SIMpegInfo* pStream
	);	

Parameters
	
	hInstance
		[in] pmt����ʵ���ľ�� SIPMT_CreateInstance������
	hPreStream
		[in] ǰһ����Ƶ���ľ��
	pStream
		[out]�������Ϣ�Ľṹ�����ĵ�ַ

Return Values

HCSHANDLE:  �ɹ�������һ������Ϣ�ľ��
          NULL��ʾ�ò���û�еõ�����Ϣ��
	
*/
HCSHANDLE SIPMT_GetNextStream(HCSHANDLE hInstance, HCSHANDLE hPreStream, SIMpegInfo* pStream);


/*
������ SIPMT_GetStreamDescriptor
	�õ�ĳһ�����е�ָ��descriptor��ԭʼ����

	void SIPMT_GetStreamDescriptor(
		HCSHANDLE hInstance,
		SIMpegInfo* pStream,
		BYTE tagValue,
		BYTE* pBuf,
		int* pnBuf
	);	

Parameters
	
	hInstance
		[in] pmt����ʵ���ľ�� SIPMT_CreateInstance������
	pStream
		[in]ָ��ĳ������ָ��
	tagValue
		[in]ָ����descriptor�ı�־
	pBuf
		[out]ָ�����е�descriptor��ԭʼ����
	pnBuf
		[out] ָ�����е�descriptor��ԭʼ���ݳ���

Return Values

BOOL:�ɹ�����ΪTRUE
      ʧ�ܷ���ΪFALSE

*/
BOOL SIPMT_GetStreamDescriptor(HCSHANDLE hInstance, SIMpegInfo* pStream, BYTE tagValue, BYTE* pBuf, int* pnBuf);


/*
������ SIPMT_GetFirstStreamByType
	�õ�pmt��ָ�����͵ĵ�һ������Ϣ

	void SIPMT_GetFirstStreamByType(
		HCSHANDLE hInstance,
		BYTE bType,
		SIMpegInfo* pStream
	);	

Parameters
	
	hInstance
		[in] pmt����ʵ���ľ�� SIPMT_CreateInstance������
	bType
		[in]ָ����������
	pStream
		[out] �������Ϣ�Ľṹ�����ĵ�ַ

Return Values

HCSHANDLE:  �ɹ����ص�һ������Ϣ�ľ��
          NULL��ʾ�ò���û�еõ�����Ϣ��

*/
HCSHANDLE SIPMT_GetFirstStreamByType(HCSHANDLE hInstance, BYTE bType, SIMpegInfo* pStream);


/*
������ SIPMT_GetNextStreamByType
	�õ�pmt��ָ�����͵���һ������Ϣ

	void SIPMT_GetNextStreamByType(
		HCSHANDLE hInstance,
		HCSHANDLE hPreStream,
		SIMpegInfo* pStream
	);	

Parameters
	
	hInstance
		[in] pmt����ʵ���ľ�� SIPMT_CreateInstance������
	hPreStream
		[in]ǰһ������Ϣ�ľ��
	pStream
		[out] �������Ϣ�Ľṹ�����ĵ�ַ

Return Values

HCSHANDLE:  �ɹ�������һ������Ϣ�ľ��
          NULL��ʾ�ò���û�еõ�����Ϣ��

*/
HCSHANDLE SIPMT_GetNextStreamByType(HCSHANDLE hInstance, HCSHANDLE hPreStream, SIMpegInfo* pStream);


/*
������ SIPMT_GetSectionData
	�õ�pmt��ԭʼ����

	void SIPMT_GetSectionData(
		HCSHANDLE hInstance,
		BYTE* pBuffer,
		int *pnLength
	);	

Parameters
	
	hInstance
		[in] pmt����ʵ���ľ�� SIPMT_CreateInstance������
	pBuffer
		[out]ָ��ԭʼ���ݵ�ָ��
	pnLength
		[out] ԭʼ���ݳ���ָ��

Return Values

BOOL:�ɹ�����ΪTRUE
      ʧ�ܷ���ΪFALSE

*/
BOOL SIPMT_GetSectionData(HCSHANDLE hInstance, BYTE* pBuffer, int *pnLength);


/*
������ SIPMT_GetFirstSubtitle
	�õ���һ��Subtitle����Ϣ

	void SIPMT_GetFirstSubtitle(
		HCSHANDLE hInstance,
		SISubtitle* pSubtitle
	);	

Parameters
	
	hInstance
		[in] pmt����ʵ���ľ�� SIPMT_CreateInstance������
	pSubtitle
		[out]ָ��õ���Subtitle����Ϣ��ָ��

Return Values

HCSHANDLE:  �ɹ����ص�һ��Subtitle��Ϣ�ľ��
          NULL��ʾ�ò���û�еõ�Subtitle��Ϣ��

*/
HCSHANDLE SIPMT_GetFirstSubtitle( HCSHANDLE hInstance, SISubtitle* pSubtitle );


/*
������ SIPMT_GetNextSubtitle
	�õ���һ��Subtitle����Ϣ

	void SIPMT_GetNextSubtitle(
		HCSHANDLE hInstance,
		HCSHANDLE hPre,
		SISubtitle* pSubtitle
	);	

Parameters
	
	hInstance
		[in] pmt����ʵ���ľ�� SIPMT_CreateInstance������
	hPre
		[in]ǰһ��Subtitle��Ϣ�ľ��
	pSubtitle
		[out] ָ��õ���Subtitle����Ϣ��ָ��

Return Values

HCSHANDLE:  �ɹ�������һ��Subtitle��Ϣ�ľ��
          NULL��ʾ�ò���û�еõ�Subtitle��Ϣ��

*/
HCSHANDLE SIPMT_GetNextSubtitle( HCSHANDLE hInstance, HCSHANDLE hPre, SISubtitle* pSubtitle );


/*
������ SIPMT_GetFirstTeletext
	�õ���һ��Teletext����Ϣ

	void SIPMT_GetFirstTeletext(
		HCSHANDLE hInstance,
		SITeletext* pTeletext
	);	

Parameters
	
	hInstance
		[in] pmt����ʵ���ľ�� SIPMT_CreateInstance������
	pTeletext
		[out]ָ��õ���Teletext����Ϣ��ָ��

Return Values

HCSHANDLE:  �ɹ����ص�һ��Teletext��Ϣ�ľ��
          NULL��ʾ�ò���û�еõ�Teletext��Ϣ��

*/
HCSHANDLE SIPMT_GetFirstTeletext( HCSHANDLE hInstance, SITeletext* pTeletext );


/*
������ SIPMT_GetNextTeletext
	�õ���һ��Teletext����Ϣ

	void SIPMT_GetNextTeletext(
		HCSHANDLE hInstance,
		HCSHANDLE hPre,
		SITeletext* pTeletext
	);	

Parameters
	
	hInstance
		[in] pmt����ʵ���ľ�� SIPMT_CreateInstance������
	hPre
		[in]ǰһ��Teletext��Ϣ�ľ��
	pTeletext
		[out] ָ��õ���Teletext����Ϣ��ָ��

Return Values

HCSHANDLE:  �ɹ�������һ��Teletext��Ϣ�ľ��
          NULL��ʾ�ò���û�еõ�Teletext��Ϣ��

*/
HCSHANDLE SIPMT_GetNextTeletext( HCSHANDLE hInstance, HCSHANDLE hPre, SITeletext* pTeletext );


#ifdef __cplusplus
}
#endif
// }}}USR

// {{{USR

// }}}USR

#endif /* sipmt_H */
