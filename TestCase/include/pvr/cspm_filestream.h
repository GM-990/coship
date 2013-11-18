/******************************************************************************
FileName:  cspm_filestream.h
Copyright (c) 2008�� Shen Zhen Coship Electronics CO.,LTD.��

Version:       Ver 1.0
Author:        luojinhua (luojinhua@coship.com)

�ļ�����:
������PVR�ط��ļ�������ؽӿ�
ģ����PVRSTREAM

�޸ļ�¼:
Version     Date            Author          Description
--------    ----------    --------          ------------
1.0         2008-01-16      �޽�           Create

*******************************************************************************/
#ifndef CSPM_FILESTREAM_H
#define CSPM_FILESTREAM_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "mmcp_typedef.h"
#include "CSUSPCommonType.h"
#include "cspm_record.h"

/*!����Ƶ���������ַ������ȳ���*/
#define CSPVR_AUDIO_NAME_LEN 6
/*!֧�ֵ������Ƶ����*/
#define CSPVR_AUDIO_COUNT_MAX 6
/*!�Ƿ�����ǩֵ*/
#define CSPVR_INVALID_BOOKMARK_VALUE 0xffffffff

    /*!֧�ֵ������Ļ����*/
#define CSPVR_SUBTITLE_COUNT_MAX 8

#define CSPVR_SUBTITLE_MAXDESCNT_PERSTREAM (20)

    /*!��Ƶ��Ϣ*/
    typedef struct _CSPVRSTREAMAudInfo_S
    {
        /*!�������������(ASII���ַ���)*/
        char szLeftName[CSPVR_AUDIO_NAME_LEN];
        /*!�������������(ASII���ַ���)*/
        char szRightName[CSPVR_AUDIO_NAME_LEN];
        /*!��Ƶpid*/
        WORD wPid;
    } CSPVRSTREAMAudInfo_S;


    /*!Subtitle ��Ϣ*/
    typedef struct _CSPVRSubtitleEntity_S
    {
        /*!Subtitle ������Ϣ*/
        BYTE   aucLanguage[3];
        BYTE   ucSubtitleType;
        WORD   wCompPageID;
        WORD   wAnciPageID;
    } CSPVRSubtitleEntity_S;

    typedef struct _CSPVRSubtitileParams_S
    {
        BYTE    ucStreamType;
        WORD    wSubtitlePID;
        BYTE    ucDescriptorCount;
        CSPVRSubtitleEntity_S sDescriptor[CSPVR_SUBTITLE_MAXDESCNT_PERSTREAM];
    } CSPVRSubtitileParams_S;

    /*!
    	PVR�ļ�����Ϣ
    */
    typedef struct _CSPVRSTREAMPvrInfo_S
    {
        /*!Ƶ����(WideWord�ַ���)*/
        char				wsChnlName[CSPVR_NAME_SIZE];
        /*!�û����޸ĵ��û��Ѻ�����,WideWord�ַ���*/
        char				wsUserName[CSPVR_NAME_SIZE];
        /*!����Ƶ��Ϣ*/
        CSPVRSTREAMAudInfo_S	sMutiAud[CSPVR_AUDIO_COUNT_MAX];
        /*!����Ƶ�ĸ���*/
        int					nAudCount;
        /*!���ļ�������¼��ʱ��*/
        CSUDIPLUSOSSysTime_S			sStartRecordTime;
        /*!Subtitle ��Ϣ*/
        CSPVRSubtitileParams_S m_asSubtitle[CSPVR_SUBTITLE_COUNT_MAX];
        /*!Subtitle �ĸ���*/
        int m_nSubtitleCount;
    } CSPVRSTREAMPvrInfo_S;

/*!
	��һ��PVR�ļ���
@param[in] PVR�ļ���·��������PVR�ļ�����ģ�����CSPVRFMFGetFilePath������ȡ�õ���
\return PVR�ļ����ľ��
\note ��һ��PVR���ļ����ڻط�
\note���ȵ���CSPVRFMFGetFilePath��ȡ�ļ�·����,�ٵ��ñ�����Open���ļ�
*/
HCSHANDLE CSPVRSTREAMOpen(const char *szFilePath);

/*!
	��ȡPVR�ļ�����Ϣ
@param[in] hPvrStream PVR�ļ���
@param[out] psOutInfo ���PVR�ļ�����Ϣ
\return �ɹ�����TRUE,���򷵻�FALSE
\note
*/
BOOL CSPVRSTREAMGetInfo(HCSHANDLE hPvrStream, CSPVRSTREAMPvrInfo_S *psOutInfo);

/*!
	�ر�PVR�ļ���
@param[in] hPvrStream PVR�ļ������
\return �ɹ�����TRUE,���򷵻�FALSE
\note ���ڻطŵ�PVR�ļ������ܱ��ر�
*/
BOOL CSPVRSTREAMClose(HCSHANDLE hPvrStream);

/*!
	��ȡPVR�ļ�������ǩ����
@param[in] hPvrStream PVR�ļ������
\return PVR�ļ�������ǩ����
*/
DWORD CSPVRSTREAMGetBookMarkCount(HCSHANDLE hPvrStream);

/*!
	��ȡPVR�ļ�������ǩ��λ��
@param[in] hPvrStream PVR�ļ������
\return PVR�ļ�������ǩ��λ��(Ϊ�طŵ�ʱ��λ��,��λΪ��), ����ʱ���� PVRPF_INVALID_TIME
*/
DWORD CSPVRSTREAMGetBookMark(HCSHANDLE hPvrStream, int nIdx);

/*!����ǩ��ǵĽ��*/
typedef enum _CSPVRSTREAMMarkRet_E
{
	/*!�ɹ�����˵�ǰλ��*/
	EM_PVRSTREAM_MARK_SUCCESS=0,
	/*!�Ƴ��˵�ǰλ�õı��*/
	EM_PVRSTREAM_MARK_REMOVE,
	/*!�������*/
	EM_PVRSTREAM_MARK_FULL,
	/*!��ǳ���*/
	EM_PVRSTREAM_MARK_FAILED
}CSPVRSTREAMMarkRet_E;

/*!
	��ǩ���PVR�ļ�����ָ��λ��
@param[in] hPvrStream PVR�ļ������
@param[in] dwMarkValueInS ���ǵ�PVR�ļ�����λ��(�طŵ�ʱ��λ��,��λΪ��)
\return ��ǵĽ��(��CSPVRSTREAMMarkRet_E�Ķ���)
\note 
*/
CSPVRSTREAMMarkRet_E CSPVRSTREAMSetBookMark(HCSHANDLE hPvrStream, DWORD dwMarkValueSec);

/*!
	Ƭ������
@param[in]  hPvrStream PVR�ļ������
@param[in] dwStartPosInS ��Ҫ����Ƭ�ϵĿ�ʼλ��(�طŵ�ʱ��λ��,��λΪ��)
@param[in] dwEndPodInS ��Ҫ����Ƭ�ϵĽ���λ��(�طŵ�ʱ��λ��,��λΪ��)
\return �ɹ�����TRUE,���򷵻�FALSE
\note Ƭ�����ضԵ�ǰ���ڽ��еĻط��Լ��Ժ�(�����ؿ�����)�ĸ�PVR�ļ��طž���Ч
*/
BOOL CSPVRSTREAMHideSegment(HCSHANDLE hPvrStream, DWORD dwStartPosSec,DWORD dwEndPosSec);

/*!
	�ָ����������ص�Ƭ��
@param[in] hPvrStream PVR�ļ������
\return �ɹ�����TRUE,���򷵻�FALSE
\note 
*/
BOOL CSPVRSTREAMUnHideAllSegment(HCSHANDLE hPvrStream);

#ifdef __cplusplus
}
#endif

#endif

/* End of File */

