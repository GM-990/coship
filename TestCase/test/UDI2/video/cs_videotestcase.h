#ifndef _CS_VIDEO_TEST_CASE_H_
#define _CS_VIDEO_TEST_CASE_H_

#include "cs_testkit.h"
#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_video.h"
#include <math.h> 
#include "udi2_os.h"

#ifdef __cplusplus
extern "C" {
#endif

CSUDI_BOOL CSTC_VIDEO_Init(void);
CSUDI_BOOL CSTC_VIDEO_UnInit(void);

CSUDI_BOOL CSTC_VIDEO_IT_GetCount_0001(void);
CSUDI_BOOL CSTC_VIDEO_IT_GetCount_0002(void);

CSUDI_BOOL CSTC_VIDEO_IT_GetCapability_0001(void);
CSUDI_BOOL CSTC_VIDEO_IT_GetCapability_0002(void);
CSUDI_BOOL CSTC_VIDEO_IT_GetCapability_0003(void);

CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0001(void);
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0002(void);
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0003(void);
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0004(void);
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0005(void);
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0006(void);
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0007(void);
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0008(void);
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0009(void);
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0010(void);
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0011(void);
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0012(void);
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0013(void);
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0014(void);
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0015(void);
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0016(void);
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0017(void);
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0018(void);
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0019(void);

CSUDI_BOOL CSTC_VIDEO_IT_GetStreamStatus_0001(void);
CSUDI_BOOL CSTC_VIDEO_IT_GetStreamStatus_0002(void);
CSUDI_BOOL CSTC_VIDEO_IT_GetStreamStatus_0003(void);

CSUDI_BOOL CSTC_VIDEO_IT_Show_0001(void);
CSUDI_BOOL CSTC_VIDEO_IT_Show_0002(void);
CSUDI_BOOL CSTC_VIDEO_IT_Show_0003(void);

CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0001(void);
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0002(void);
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0003(void);
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0004(void);
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0005(void);
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0006(void);
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0007(void);
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0008(void);
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0009(void);
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0010(void);
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0011(void);
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0012(void);
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0013(void);
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0014(void);
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0015(void);
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0016(void);
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0017(void);
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0018(void);
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0019(void);
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0020(void);
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0021(void);
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0022(void);
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0023(void);
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0024(void);
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0025(void);
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0026(void);
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0027(void);
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0028(void);
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0029(void);
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0030(void);
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0031(void);
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0032(void);
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0033(void);

CSUDI_BOOL CSTC_VIDEO_IT_SetAspectRatio_0001(void);
CSUDI_BOOL CSTC_VIDEO_IT_SetAspectRatio_0002(void);
CSUDI_BOOL CSTC_VIDEO_IT_SetAspectRatio_0003(void);
CSUDI_BOOL CSTC_VIDEO_IT_SetAspectRatio_0004(void);
CSUDI_BOOL CSTC_VIDEO_IT_SetAspectRatio_0005(void);

CSUDI_BOOL CSTC_VIDEO_IT_SetMatchMethod_0001(void);
CSUDI_BOOL CSTC_VIDEO_IT_SetMatchMethod_0002(void);
CSUDI_BOOL CSTC_VIDEO_IT_SetMatchMethod_0003(void);

CSUDI_BOOL CSTC_VIDEO_IT_SetStopMode_0001(void);
CSUDI_BOOL CSTC_VIDEO_IT_SetStopMode_0002(void);
CSUDI_BOOL CSTC_VIDEO_IT_SetStopMode_0003(void);
CSUDI_BOOL CSTC_VIDEO_IT_SetStopMode_0004(void);
CSUDI_BOOL CSTC_VIDEO_IT_SetStopMode_0005(void);

CSUDI_BOOL  CSTC_VIDEO_IT_SetWindowSize_0001(void);
CSUDI_BOOL  CSTC_VIDEO_IT_SetWindowSize_0002(void);
CSUDI_BOOL  CSTC_VIDEO_IT_SetWindowSize_0003(void);
CSUDI_BOOL  CSTC_VIDEO_IT_SetWindowSize_0004(void);
CSUDI_BOOL  CSTC_VIDEO_IT_SetWindowSize_0005(void);
CSUDI_BOOL  CSTC_VIDEO_IT_SetWindowSize_0006(void);
CSUDI_BOOL  CSTC_VIDEO_IT_SetWindowSize_0007(void);
CSUDI_BOOL  CSTC_VIDEO_IT_SetWindowSize_0008(void);
CSUDI_BOOL  CSTC_VIDEO_IT_SetWindowSize_0009(void);
CSUDI_BOOL  CSTC_VIDEO_IT_SetWindowSize_0010(void);
CSUDI_BOOL  CSTC_VIDEO_IT_SetWindowSize_0011(void);
CSUDI_BOOL  CSTC_VIDEO_IT_SetWindowSize_0012(void);
CSUDI_BOOL  CSTC_VIDEO_IT_SetWindowSize_0013(void);
CSUDI_BOOL  CSTC_VIDEO_IT_SetWindowSize_0014(void);
CSUDI_BOOL  CSTC_VIDEO_IT_SetWindowSize_0015(void);
CSUDI_BOOL  CSTC_VIDEO_IT_SetWindowSize_0016(void);
CSUDI_BOOL  CSTC_VIDEO_IT_SetWindowSize_0017(void);
CSUDI_BOOL  CSTC_VIDEO_IT_SetWindowSize_0018(void);
CSUDI_BOOL  CSTC_VIDEO_IT_SetWindowSize_0019(void);
CSUDI_BOOL  CSTC_VIDEO_IT_SetWindowSize_0020(void);
CSUDI_BOOL  CSTC_VIDEO_IT_SetWindowSize_0021(void);
CSUDI_BOOL  CSTC_VIDEO_IT_SetWindowSize_0022(void);
CSUDI_BOOL	CSTC_VIDEO_IT_SetWindowSize_0023(void);
CSUDI_BOOL	CSTC_VIDEO_IT_SetWindowSize_0024(void);
CSUDI_BOOL	CSTC_VIDEO_IT_SetWindowSize_0025(void);
CSUDI_BOOL	CSTC_VIDEO_IT_SetWindowSize_0026(void);
CSUDI_BOOL CSTC_VIDEO_IT_WindowSizeMatchMethod_0001(void);
CSUDI_BOOL CSTC_VIDEO_IT_WindowSizeMatchMethod_0002(void);


CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0001(void);
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0002(void);
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0003(void);
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0004(void);
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0005(void);
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0006(void);
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0007(void);
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0008(void);
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0009(void);
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0010(void);
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0011(void);
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0012(void);
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0013(void);
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0014(void);
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0015(void);
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0016(void);
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0017(void);
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0018(void);
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0019(void);
CSUDI_BOOL CSTC_VIDEO_IT_SetVBIEnable_0001(void);
CSUDI_BOOL CSTC_VIDEO_IT_SetVBIEnable_0002(void);

CSUDI_BOOL CSTC_VIDEO_IT_GetVBIEnable_0001(void);
CSUDI_BOOL CSTC_VIDEO_IT_GetVBIEnable_0002(void);

CSUDI_BOOL CSTC_VOUT_DisableEnable_0001(void);
CSUDI_BOOL CSTC_VOUT_DisableEnable_0002(void);

/*
@brief �ж�Config �ļ��е���Ϣ�Ĵ�Ž���
Input------
[pResult]:��Ҫ�жϵ��ַ���
Output-----
�ַ�������Ӧ�Ľ���[int]
*/
int CSTC_VIDEO_CheckStr(const char *pResult);

/*
@brief ��VideoCapability �ڵ���Ϣ��Config �ļ��ڵ���Ϣ���бȽ�
Input------
[pSection]:��Ҫ�ȽϵĶ�λ����
[pKey]:��Ҫ�ȽϵĶ�λ�еĹؼ�������
[nCheck]:��Ҫ�Ƚϵ�ֵ
Output-----
TRUE or FALSE
*/
CSUDI_BOOL CSTC_VIDEO_CheckFromVideoToConfig(const char *pSection,const char *pKey, int nCheck);

/*
@brief ��Config �ڵ���Ϣ��VideoCapability �ڵ���Ϣ���бȽ�
Input------
[psCapabilityInfo]: ��Ҫ�Ƚϵ�VideoCapability
[eStreamType]:��Ҫ�Ƚϵ�StreamType
[nType]:��Ҫ�Ƚϵ�StreamType ����Ӧ������
Output-----
TRUE or FALSE
*/
CSUDI_BOOL CSTC_VIDEO_CheckFromConfigToVideo
(
    const CSUDIVIDEOCapability_S *psCapabilityInfo,  
    CSUDIVIDStreamType_E  eStreamType, 
    int nIsAble
);

/*
@brief ���VideoCapability ����Ϣ�Ƿ���Config �����ļ����
Input------
None
Output-----
TRUE or FALSE
*/
CSUDI_BOOL CSTC_VIDEO_IsVideoCapabilityCorrect(void);

/*
@brief ����ȡ����������״̬��Ϣ �Ƿ���ȷ
Input------
None
Output-----
TRUE or FALSE
*/
CSUDI_BOOL CSTC_VIDEO_IsGetStreamStatusCorrect(void);

/*
@brief �����Ƶֹͣģʽ�Ƿ���ȷ
Input------
[eStopModeType]��Ƶֹͣ��ģʽ
Output-----
TRUE or FALSE
*/
CSUDI_BOOL CSTC_VIDEO_IsSetStopModeCorrect(CSUDIVIDEOStopMode_E eStopModeType);

/*
@brief �����Ƶ��ͼ(����)�ӿڣ�����ǰ������ʾ����Ƶͼ��Ĳ���(��ȫ��)
    ��ȡ�������Ŀ��ͼ���ָ������
Input------
[nTestID]��Ҫ�Ĳ���ID
Output-----
TRUE or FALSE
*/
CSUDI_BOOL CSTC_VIDEO_IsSnapshotCorrect(int nTestID);

/*
@brief ����CSUDIVIDEOSetAspectRatio ��CSUDIVIDEOSetMatchMethod ������Ч��
Input------
[nTestID]��Ҫ�Ĳ���ID
Output-----
TRUE or FALSE
*/
CSUDI_BOOL CSTC_VIDEO_IsMatchMethodAspectRatioCorrect(int nTestID);

/*
@brief ���ָ����Ƶ��������Ĵ��ڴ�С�����ģʽʵ�����Ź���
Input------
[nTestID]��Ҫ���Ե�ID
[eScreenDevice]��Ƶ���ģʽ(�������)
Output-----
TRUE or FALSE
*/
CSUDI_BOOL CSTC_VIDEO_IsSetWindowSizeCorrectEx(int nTestID, CSUDISCREENType_E eScreenDevice);

/*
@brief ���ָ����Ƶ��������Ĵ��ڴ�Сʵ�����Ź��ܣ�һ�αȽ��������ģʽ
Input------
[nTestID]��Ҫ���Ե�ID
Output-----
TRUE or FALSE
*/
CSUDI_BOOL CSTC_VIDEO_IsSetWindowSizeCorrect(int nTestID);

/*
@brief �����ʾ�������Ƶ������ʵ�־ֲ��Ŵ���С�Ȳ�
Input------
[nTestID]��Ҫ���Ե�ID
[eScreenDevice]��Ƶ����ģʽ(����/����)
Output-----
TRUE or FALSE
*/
CSUDI_BOOL CSTC_VIDEO_IsSetWinClipCorrectEx(int nTestID, CSUDISCREENType_E eScreenDevice);

/*
@brief �����ʾ�������Ƶ������ʵ�־ֲ��Ŵ���С�Ȳ���,һ���ԱȽ�����
Input------
[nTestID]ָ������ID
Output-----
TRUE or FALSE
*/
CSUDI_BOOL CSTC_VIDEO_IsSetWinClipCorrect(int nTestID);

#ifdef __cplusplus
}
#endif

#endif

