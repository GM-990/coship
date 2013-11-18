#ifndef _CS_PVRSTREAM_H_
#define _CS_PVRSTREAM_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "CSPvrBase.h"

#define MAX_FILE_NAME_LEN (256)

typedef enum
{
	EM_PVRSTREAM_SUCCESS = 0,
	EM_PVRSTREAM_FAILED,
	EM_PVRSTREM_NOT_SUPPORTED
}EPvrStreamResult;

typedef struct 
{
	char szStreamName[MAX_FILE_NAME_LEN];
	SPvrBasePidData sPids;	
	int nSecondLength;
}SPvrStreamInfo;

typedef struct
{
	UINT uStartTimeSecond;
	UINT uEndTimeSecond;
}SPvrStreamCutMark;


/*!
����������
	��ȡ��ͨ¼�Ƶ����ļ���Ϣ��

������
	pOrgStreamName
		[in]	���ļ���·��
	pInfo
		[out]	���ļ���Ϣ

����ֵ
	�ɹ���ȡ����EM_PVRSTREAM_SUCCESS
	���򷵻�EM_PVRSTREAM_FAILED

��ע��
	��
*/
EPvrStreamResult	PvrStream_GetNormalInfo(const char *pPvrStreamName, SPvrStreamInfo *pInfo);
/*!
����������
	��ȡ��ͨ¼�Ƶ����ļ���Ϣ��

������
	pOrgStreamName
		[in]	���ļ���·��
	pInfo
		[out]	���ļ���Ϣ

����ֵ
	�ɹ���ȡ����EM_PVRSTREAM_SUCCESS
	���򷵻�EM_PVRSTREAM_FAILED

��ע��
	��
*/
EPvrStreamResult	PvrStream_GetTShiftInfo( HCSHANDLE hTShiftFile, SPvrStreamInfo *pInfo);

/**
[[[ add by luo jinhua 2007-07-25
����������
	�����ļ������ļ��Ŀ�ͷ���ּ���������ɾ��0 -- nCropSecondPos�����ݣ�
������
	szPvrStreamName
		[in]	���ļ���·��
	nCropSecondPos
		[in]	��Ҫ���е���ͷ������

����ֵ
	�ɹ�����EM_PVRSTREAM_SUCCESS
	���򷵻�EM_PVRSTREAM_FAILED

��ע��
	��
*/
EPvrStreamResult PvrStream_CropStreamHead(const char *szPvrStreamName, int nCropSecondPos);
/*]]]*/

/**
[[[ add by luo jinhua 2007-07-25
����������
	PVR¼�����ļ��й���
������
	szStreamName
		[in]	��Ҫ�����е�¼����
	pMarks
		[in]	���е���ֹʱ�������
	nMarksCount
		[in] ���е�ʱ�������
	pOutCutLen
		[out] ¼�������е����ܳ���
	pPlayPosInSec
		[in/out] ����Ϊ��ǰ���ŵ�λ�ã�
				���Ϊ���к�ǰ�Ĳ���λ��				

����ֵ
	�ɹ�����EM_PVRSTREAM_SUCCESS
	���򷵻�EM_PVRSTREAM_FAILED

��ע��
	���磬һ��1000S��¼�ƣ���ǰ����λ��Ϊ800��
	����200��400�ε����ݣ�
	��ô��ǰ�Ĳ���λ�þ�Ϊ800 -��400 - 200��= 600S  
*/
EPvrStreamResult	PvrStream_Cut(const char *szStreamName,  SPvrStreamCutMark *pMarks, int nMarksCount, UINT *pOutCutLen,UINT *pPlayPosInSec);

/*!
����������
	��ʱ��¼�ƿ���Ϊ��ͨ¼��

������
	hTShiftFile 
		[in]	ʱ��¼�Ƶľ��
	uStartSecond 
		[in]	������ʼ����ʱ��¼���е�λ�� 
	uEndSecond  
		[in]	��������������¼���е�λ��
	szNewStreamName  
		[in]	�������µ��ļ���


����ֵ
	�ɹ�����EM_PVRSTREAM_SUCCESS
	���򷵻�EM_PVRSTREAM_FAILED

��ע��
	��
*/
EPvrStreamResult	PvrStream_CopyTshiftToFile( HCSHANDLE hTShiftFile, UINT uStartSecond, UINT uEndSecond, const char *szNewStreamName);

#ifdef __cplusplus
}
#endif

#endif

