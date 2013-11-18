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
@brief 判断Config 文件中的信息的存放进制
Input------
[pResult]:需要判断的字符串
Output-----
字符串所对应的进制[int]
*/
int CSTC_VIDEO_CheckStr(const char *pResult);

/*
@brief 将VideoCapability 内的信息与Config 文件内的信息进行比较
Input------
[pSection]:需要比较的段位名称
[pKey]:需要比较的段位中的关键字名称
[nCheck]:需要比较的值
Output-----
TRUE or FALSE
*/
CSUDI_BOOL CSTC_VIDEO_CheckFromVideoToConfig(const char *pSection,const char *pKey, int nCheck);

/*
@brief 将Config 内的信息与VideoCapability 内的信息进行比较
Input------
[psCapabilityInfo]: 需要比较的VideoCapability
[eStreamType]:需要比较的StreamType
[nType]:需要比较的StreamType 所对应的能力
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
@brief 检测VideoCapability 的信息是否与Config 配置文件相等
Input------
None
Output-----
TRUE or FALSE
*/
CSUDI_BOOL CSTC_VIDEO_IsVideoCapabilityCorrect(void);

/*
@brief 检测获取解码器播放状态信息 是否正确
Input------
None
Output-----
TRUE or FALSE
*/
CSUDI_BOOL CSTC_VIDEO_IsGetStreamStatusCorrect(void);

/*
@brief 检测视频停止模式是否正确
Input------
[eStopModeType]视频停止的模式
Output-----
TRUE or FALSE
*/
CSUDI_BOOL CSTC_VIDEO_IsSetStopModeCorrect(CSUDIVIDEOStopMode_E eStopModeType);

/*
@brief 检测视频截图(快照)接口，将当前正在显示的视频图像的部分(或全部)
    截取并输出到目标图像的指定区域。
Input------
[nTestID]需要的测试ID
Output-----
TRUE or FALSE
*/
CSUDI_BOOL CSTC_VIDEO_IsSnapshotCorrect(int nTestID);

/*
@brief 测试CSUDIVIDEOSetAspectRatio 和CSUDIVIDEOSetMatchMethod 的联合效果
Input------
[nTestID]需要的测试ID
Output-----
TRUE or FALSE
*/
CSUDI_BOOL CSTC_VIDEO_IsMatchMethodAspectRatioCorrect(int nTestID);

/*
@brief 检测指定视频解码输出的窗口大小和输出模式实现缩放功能
Input------
[nTestID]需要测试的ID
[eScreenDevice]视频输出模式(高清标清)
Output-----
TRUE or FALSE
*/
CSUDI_BOOL CSTC_VIDEO_IsSetWindowSizeCorrectEx(int nTestID, CSUDISCREENType_E eScreenDevice);

/*
@brief 检测指定视频解码输出的窗口大小实现缩放功能，一次比较所有输出模式
Input------
[nTestID]需要测试的ID
Output-----
TRUE or FALSE
*/
CSUDI_BOOL CSTC_VIDEO_IsSetWindowSizeCorrect(int nTestID);

/*
@brief 检测显示区域的视频，用于实现局部放大缩小等操
Input------
[nTestID]需要测试的ID
[eScreenDevice]视频输入模式(高清/标清)
Output-----
TRUE or FALSE
*/
CSUDI_BOOL CSTC_VIDEO_IsSetWinClipCorrectEx(int nTestID, CSUDISCREENType_E eScreenDevice);

/*
@brief 检测显示区域的视频，用于实现局部放大缩小等操作,一次性比较所有
Input------
[nTestID]指定测试ID
Output-----
TRUE or FALSE
*/
CSUDI_BOOL CSTC_VIDEO_IsSetWinClipCorrect(int nTestID);

#ifdef __cplusplus
}
#endif

#endif

