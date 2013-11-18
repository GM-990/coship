#ifndef _CS_PVRPLAY_H_
#define _CS_PVRPLAY_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "CSPvrStream.h"

#define INVALID_SPEED 10000000
#define INVALID_DEVICE_INDEX (-1)
#define INVALID_SECOND_LEN  (-1)

#define PVR_PAUSE_SPEED 0
#define PVR_NORMAL_SPEED 1000

typedef enum
{
	EM_PLAY_SUCCESS = 0,
	EM_PLAY_FAILED,
	EM_PLAY_NOT_SUPPORT
}EPlayResult;

typedef enum
{
	EM_PLAY_SEEK_FROM_HEAD = 0,
	EM_PLAY_SEEK_FROM_CURRENT,
	EM_PLAY_SEEK_FROM_END
}EPlayPosFlag;

typedef enum
{
	EM_PLAY_FREE = 0,
	EM_PLAY_WORKING,
	EM_PLAY_NOT_SUPPORT_STATUS
}EPlayStatus;

typedef enum
{
	EM_PLAY_FILE = 0,
	EM_PLAY_TSHIFT,
	EM_PLAY_LIVE,
	EM_PLAY_NOT_SUPPORT_TYPE
}EPlayType;

typedef struct
{
	int nVideoIdx;
	int nAudioIdx;	
	int nTtxIdx;
}SPlayInit;

typedef enum
{
	EM_PLAY_START = 0,
	EM_PLAY_STOP,
	EM_PLAY_END_OF_STREAM,/*pEventData, nEventData������*/
	EM_PLAY_BEGIN_OF_STREAM,/*pEventData, nEventData������*/
/*[[[ add by luo jinhua 2007-07-06*/
	EM_PLAY_POS/*nEventDataΪ��ǰ����λ��,pEventData��Ч*/
/*]]]*/
}EPlayEvent;

typedef int(*FPlayNotify)(const int nPlayId, const EPlayEvent eEvent, const void *pEventData, const int nEventData, void *pUserData);
typedef void(*CSOnPlayVideoEnable_F)(int nVideoIndex, BOOL bEnable, void* pUserData);
typedef void(*CSOnPlaySoundEnable_F)(int nSoundIndex, BOOL bEnable, void* pUserData);


/*!
����������
	��ʼ�������豸��

������
	SPlayInit
		[in]	���ò����豸��Ϣ�б�

����ֵ
	�ɹ���ʼ������EM_PLAY_SUCCESS
	���򷵻�EM_PLAY_FAILED

��ע��
	pInitList�����һ���VideoID��AudioID��ΪINVALID_DEVICE_INDEX,��ʾ�б����.
*/
EPlayResult	CSPlay_Init(const SPlayInit *pInitList);

/*!
����������
	���ò����豸�Ľ���״̬

������
	nPlayId
		[in]	�����豸��
	bEncrypt
		[in]  �����Ƿ���Ҫ����

����ֵ
	�ɹ���ʼ������EM_PLAY_SUCCESS
	���򷵻�EM_PLAY_FAILED

��ע��
*/
EPlayResult	CSPlay_SetEncrypt(const int nPlayId, BOOL bEncrypt);

/*!
����������
	�����ط��ļ���

������
	nPlayId
		[in]	�����豸ID
	szFilename
		[in]	�������ļ���·��
	nSpeed
		[in]	���ŵ��ٶ�
	nSecondPos
		[in]	���ŵ���ʼλ��
	pPids
		[in]	Ҫ���ŵ�PID

����ֵ
	�ɹ��������ŷ���EM_PLAY_SUCCESS
	���򷵻�EM_PLAY_FAILED

��ע��
	nSpeed=1000��ʾ����ֱ���ٶȡ�nSecondPosΪ�������ʼλ�õ�ʱ������λs
*/
EPlayResult	CSPlay_StartFile(const int nPlayId, const char *szFilename,  const int nSpeed, const int nSecondPos, const SPvrBasePidData *pPids);
/*!
����������
	����Tshift���š�

������
	nPlayId
		[in]	�����豸ID
	hTShiftFile
		[in]	����Tshift¼��ʱ���صľ��
	nSpeed
		[in]	���ŵ��ٶ�
	nSecondPos
		[in]	���ŵ���ʼλ��
	pPids
		[in]	Ҫ���ŵ�PID
����ֵ
	�ɹ��������ŷ���EM_PLAY_SUCCESS
	���򷵻�EM_PLAY_FAILED

��ע��
	������Ч�ɲ��ŵ���ʼλ����Ϊ0������nSecondPosΪ�����0��λ�á���������ͼ�CSPlay_GetCurSecondPos��ע�ͣ�
*/
EPlayResult	CSPlay_StartTShift(const int nPlayId, const HCSHANDLE hTShiftFile, const int nSpeed, const int nSecondPos, const SPvrBasePidData *pPids);
/*!
����������
	����ʵʱ��Ŀֱ����

������
	nPlayId
		[in]	�����豸ID
	nDemuxIdx
		[in]	DemuxID
	pPids
		[in]	���ŵĸ���PID

����ֵ
	�ɹ��������ŷ���EM_PLAY_SUCCESS
	���򷵻�EM_PLAY_FAILED

��ע��
	��
*/
EPlayResult	CSPlay_StartLive(const int nPlayId, const int nDemuxIdx, const SPvrBasePidData *pPids);
/*!
����������
	ֹͣʵʱ��Ŀֱ��/�ط��ļ�/Tshift���š�

������
	nPlayId
		[in]	�����豸ID

����ֵ
	�ɹ�ֹͣ���ŷ���EM_PLAY_SUCCESS
	���򷵻�EM_PLAY_FAILED

��ע��
	��
*/
EPlayResult	CSPlay_Stop(const int nPlayId);
/*!
����������
	�ָ�����ͣ�Ļط��ļ�/Tshift���š�

������
	nPlayId
		[in]	�����豸ID

����ֵ
	�ɹ��ָ�����EM_PLAY_SUCCESS
	���򷵻�EM_PLAY_FAILED

��ע��
	��
*/
EPlayResult	CSPlay_Resume(const int nPlayId);
/*!
����������
	��ͣ�ط��ļ�/Tshift���š�

������
	nPlayId
		[in]	�����豸ID

����ֵ
	�ɹ���ͣ���ŷ���EM_PLAY_SUCCESS
	���򷵻�EM_PLAY_FAILED

��ע��
	��
*/
EPlayResult	CSPlay_Pause(const int nPlayId);
/*!
����������
	���ûط��ļ�/Tshift���ŵ��ٶȡ�

������
	nPlayId
		[in]	�����豸ID
	nSpeed
		[in]	�����ٶ�

����ֵ
	�ɹ����ò����ٶȷ���EM_PLAY_SUCCESS
	���򷵻�EM_PLAY_FAILED

��ע��
	1000��ʾ���������ٶ�
*/
EPlayResult	CSPlay_SetSpeed(const int nPlayId, const int nSpeed);
/*!
����������
	��ȡ�����豸��״̬��

������
	nPlayId
		[in]	�����豸ID

����ֵ
	����Ŀǰ�����豸��״̬��

��ע��
	��
*/
EPlayStatus	CSPlay_GetStatus(const int nPlayId);
/*!
����������
	�޸Ĳ��ŵ�PID��

������
	nPlayId
		[in]	�����豸ID
	pPids
		[in]	PIDs

����ֵ
	�����ɹ�����EM_PLAY_SUCCESS
	���򷵻�EM_PLAY_FAILED

��ע��
	��
*/
EPlayResult	CSPlay_ModifyPids(const int nPlayId, const SPvrBasePidData *pPids);
/*!
����������
	��ȡ���Ÿ�����

������
	nPlayId
		[in]	�����豸ID

����ֵ
	��������

��ע��
	��
*/
int CSPlay_GetCount(void);
/*!
����������
	��ȡ�������͡�

������
	nPlayId
		[in]	�����豸ID

����ֵ
	��������

��ע��
	��
*/
EPlayType	CSPlay_GetType(const int nPlayId);
/*!
����������
	��ȡ�����ٶȡ�

������
	nPlayId
		[in]	�����豸ID

����ֵ
	ָ�������豸�ĵ�ǰ�����ٶ�

��ע��
	��
*/
int			CSPlay_GetSpeed(const int nPlayId);
/*!
����������
	���ò���λ�á�

������
	nPlayId
		[in]	�����豸ID
	nSecondPos
		[in]	ƫ����
	ePlayPosFlag
		[in]	ƫ�Ƶ����λ��

����ֵ
	�ɹ����ò���λ�÷���EM_PLAY_SUCCESS
	���򷵻�EM_PLAY_FAILED

��ע��
	ֻ�лط��ļ���Tshift���ſ������ò���λ��
	Tshift���ŵ���ʼλ��Ϊ��Ч�ɲ��ŵ���ʼλ�ã�������ͼ�CSPlay_GetCurSecondPos��ע�ͣ�
*/
EPlayResult	CSPlay_Seek(const int nPlayId, const int nSecondPos, const EPlayPosFlag ePlayPosFlag);
/*!
����������
	��ȡ�ط��ļ�/Tshift���ŵľ���ʱ����

������
	nPlayId
		[in]	�����豸ID

����ֵ
	�ط��ļ�/Tshift���ŵľ���ʱ����
	ʧ�ܷ���INVALID_SECOND_LEN

��ע��
*/
int	CSPlay_GetCurSecondPos(int nPlayId);

/*!*
��ȡ���طŵ�ִﵽ�ļ�ͷ��β֮����ͷβ�������ֿ��Կ�ʼ�طš��磺
TShift�������׷����¼�Ƶ����ݣ���ʱ����Ҫ��ͣ��¼�Ƽ������У���¼�Ƶĳ���
������ǰ�ط�λ��CSPlay_GetPlayValve()��󣬿��Լ����ط�
@return ���ŵķ�ֵ
*/
int CSPlay_GetPlayValve(void);
/*!
����������
	ע�Ქ��֪ͨ�ص���

������
	nPlayId
		[in]	�����豸ID
	fnNotify
		[in]	�ص�����
	pUserData
		[in]	�û�����

����ֵ
	��

��ע��
	��
*/
void			CSPlay_RegNotify(const int nPlayId, const FPlayNotify fnNotify, void *pUserData);


/*
��������:
	���õ�ǰ����Ƶ�������
����:
	bAc3Output
		[in] TRUE ��ʾ��ǰ�������ΪAC3���
		      FALSE ��ʾ��ǰ�������Ϊ��AC3���
����ֵ
	��
��ע :
	��
*/
void CSPlay_SetCurrentAudOutputType(BOOL bAc3Output);

EPlayResult CSPlay_GetCaDevice(const int nPlayId, HCSHANDLE *phOutCaDevice);

/*Flush ���ŵ�����Ƶ�������Լ�������*/
EPlayResult	CSPlay_Flush(const int nPlayId);

void CSPlay_RegVideoEnableCallback(CSOnPlayVideoEnable_F fnOnPlayVideoEnable, void* pOnPlayVideoEnable);


void CSPlay_RegSoundEnableCallback(CSOnPlaySoundEnable_F fnOnPlaySoundEnable, void* pOnPlaySoundEnable);

#ifdef __cplusplus
}
#endif

#endif

