#ifndef _CS_RECORD_H_
#define _CS_RECORD_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "CSPvrBase.h"
#include "CSPvrStream.h"

typedef enum
{
	EM_RECORD_SUCCESS = 0,
	EM_RECORD_FAILED,
	EM_RECORD_NOTWORKING,
	EM_RECORD_SPEED_LIMIT
}ERecordResult;

typedef enum
{
	EM_RECORD_FREE = 0,
	EM_RECORD_WORKING,
	EM_RECORD_NOT_SUPPORT_STATUS
}ERecordStatus;

typedef enum
{
	EM_RECORD_NORMAL = 0,
	EM_RECORD_TSHIFT
}ERecordType;

//add by luojinhua 2006-12-18
#define INVALID_SECOND_LEN  (-1)

typedef enum
{
	EM_RECORD_START = 0,
	EM_RECORD_STOP,
	EM_RECORD_WRAP_AROUND,
	///add by luojinhua 2007-09-19
	EM_RECORD_TSFILE_END,
	EM_RECORD_VOLUME_FULL
}ERecordEvent;

///[[[ add by luo jinhua 2007-09-17
typedef enum
{
	EM_RECORD_PVR = 0,
	EM_RECORD_TS,
	EM_RECORD_SREAM_TYPE_COUNT
}ERecordStreamType;
///]]]

typedef int(*FRecordNotify)(const int nRecId, const ERecordEvent eEvent,const void *pEventData, const int nEventData, void *pUserData);
void CSRecord_RegNotify(const int nRecordId,FRecordNotify fnRecordCallback, void * pUserData);
//add end

/*!
����������
	��ʼ��¼���豸��

������
	nNormalCount
		[in]	��ͨ¼���豸����Ŀ
	nTShiftCount
		[in]	Tshift¼���豸����Ŀ

����ֵ
	�ɹ���ʼ������EM_FILE_SUCCESS
	���򷵻�EM_FILE_FAILED

��ע��
	0 �� nNormalCount-1 Ϊ��ͨ¼�Ƶ�RecID
	nNormalCount �� ��nNormalCount + nTShiftCount - 1�� ΪTshift¼�Ƶ�RecID
*/
ERecordResult	CSRecord_Init(int nNormalCount, int nTShiftCount);

/*!
��ȡTShift record��ʼ��ţ���ֵΪnNormalCount
����: TShift record��ʼ���
*/
int CSRecord_GetTShiftRecordFirstIndex(void);

/*!
��ȡ���ɽ��ж���·TShift¼�ƣ���ֵΪnTShiftCount
����: ���ɽ��ж���·TShift¼��
*/
int CSRecord_GetTShiftRecordCount(void);

/*!
����������
	����¼�Ƶļ���״̬

������
	nRecId
		[in]	¼���豸ID
	bEncrypt
		[in]	�����Ƿ���м���¼��
����ֵ
	�ɹ�����¼�Ʒ���EM_RECORD_SUCCESS
��ע��
	��
*/
ERecordResult CSRecord_SetEncrypt(const int nRecId, BOOL bEncrypt);

/*!
����������
	������ͨ¼�ơ�

������
	nRecId
		[in]	¼���豸ID
	nDemuxIdx
		[in]	DemuxID
	pStreamName
		[in]	Ŀ�����ļ���
	SPvrBasePidData
		[in]	¼�Ƶ�PID

����ֵ
	�ɹ�����¼�Ʒ���EM_FILE_SUCCESS
	���򷵻�EM_FILE_FAILED

��ע��
	��
*/
ERecordResult	CSRecord_StartNormal(const int nRecId, const int nDemuxIdx, const char* pStreamName, const SPvrBasePidData *pPids);
///[[[ add by luo jinhua 2007-09-17
ERecordResult CSRecord_StartTs(const int nRecId, const int nDemuxIdx,const char* szFilename, const SPvrBasePidData *psPids);
//ERecordResult CSRecord_StartNormalEx(const int nRecId, const int nDemuxIdx, const char* pStreamName, const SPvrBasePidData *pPids, ERecordStreamType eType); 

///[[[ add by luo jinhua 2007-07-15
ERecordResult	CSRecord_RecordAppendToExistFile(const int nRecId, const int nDemuxIdx, const char* pStreamName, const SPvrBasePidData *pPids);

ERecordResult CSRecord_SwitchTshiftToNomalFile(const int nRecId, const char *szNomalFileName);

ERecordResult CSRecord_CheckTShiftSpace(HCSHANDLE hTShift, BOOL *pbOutIsSpaceEnough);
///]]]

/*!
����������
	����Tshift¼�ơ�

������
	nRecId
		[in]	¼���豸ID
	nDemuxIdx
		[in]	DemuxID
	pStreamName
		[in]	Ŀ�����ļ���
	SPvrBasePidData
		[in]	¼�Ƶ�PID

����ֵ
	�ɹ�����¼�Ʒ���EM_FILE_SUCCESS
	���򷵻�EM_FILE_FAILED

��ע��
	��
*/
ERecordResult	CSRecord_StartTShift(const int nRecId, const int nDemuxIdx,  const SPvrBasePidData *pPids, HCSHANDLE* phTShiftFile);
/*!
����������
	ֹͣ¼�ơ�

������
	nRecId
		[in]	¼���豸ID

����ֵ
	�ɹ�ֹͣ¼�Ʒ���EM_FILE_SUCCESS
	���򷵻�EM_FILE_FAILED

��ע��
	��
*/
ERecordResult	CSRecord_Stop(const int nRecId);
/*!
����������
	��ͣ¼�ơ�

������
	nRecId
		[in]	¼���豸ID

����ֵ
	�ɹ���ͣ¼�Ʒ���EM_FILE_SUCCESS
	���򷵻�EM_FILE_FAILED

��ע��
	��
*/
ERecordResult	CSRecord_Pause(const int nRecId);
/*!
����������
	�ָ�����ͣ��¼�ơ�

������
	nRecId
		[in]	¼���豸ID

����ֵ
	�ɹ��ָ�¼�Ʒ���EM_FILE_SUCCESS
	���򷵻�EM_FILE_FAILED

��ע��
	��
*/
ERecordResult	CSRecord_Resume(const int nRecId);
/*!
����������
	��ȡ��¼��ʱ����

������
	nRecId
		[in]	¼���豸ID

����ֵ
	��ȷ��ȡ������¼��ʱ��
	���򷵻�INVALID_SECOND_LEN

��ע��
	nRecId¼��Ϊ��Working״̬ʱ,��ȡʧ��
	��ȡ��¼��ʱ��Ϊ��Ч�ɲ���ʱ����
	����Tshift¼�ƣ���¼�ƻ�����ʱ����¼�����ݻḲ�Ǿ�¼�����ݣ�
	��Ч�ɲ���ʱ�����䣬�����ȡ��¼�Ƴ��ȱ��ֲ��䡣
*/
int CSRecord_GetSecondLength(const int nRecId);
/*!
����������
	��ȡ¼���豸״̬��

������
	nRecId
		[in]	¼���豸ID

����ֵ
	����¼���豸״̬

��ע��
	��
*/
ERecordStatus	CSRecord_GetStatus(const int nRecId);
/*!
����������
	��ȡ¼�����͡�

������
	nRecId
		[in]	¼���豸ID

����ֵ
	����¼������

��ע��
	��
*/
ERecordType	CSRecord_GetType(const int nRecId);

/*!
����������
	��ȡ¼�Ƶľ���ʱ��

������
	nRecId
		[in]	¼���豸ID

����ֵ
	����¼�Ƶľ���ʱ��

��ע��
	��
*/
int CSRecord_GetStartTimeInSecond(const int nRecId);


/*!
����������
	����TShift¼�ƿ��õĿռ��С
������
	uPacketSize
		[in]	TShift¼�ƿ��õĿռ��С,188�ֽ�Ϊ��λ
��ע:
	��
*/
void CSRecord_SetTShiftRecordPacketSpace(UINT uPacketSize);

/*��ȡ¼�ƵĽ����豸*/
ERecordResult CSRecord_GetCaDevice(const int nRecId,HCSHANDLE *phCaDevice);

/*!
����������
	����TShift¼��·��
������
	szTshiftPath
		[in]	TShift¼�Ƶ�·��
��ע:
	��
*/
void CSRecord_SetTshiftRecordPath(char * szTshiftPath);


#ifdef __cplusplus
}
#endif

#endif
