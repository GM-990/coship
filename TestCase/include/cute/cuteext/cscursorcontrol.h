#ifndef CSCURSORCONTROL_H
#define CSCURSORCONTROL_H

#ifdef __cplusplus
#include "cutepublic.h"
#include "cutesingleton.h"

CSCUTE_PRIVATE_CLASS(CSCursorControl)

//**********************| C++ interface |**********************
class CSCUTE_DECL_EXPORT CSCursorControl: public CSCuteSingleton<CSCursorControl>
{
    CSCUTE_DECLARE_PRIVATE(CSCursorControl)
public:
    CSCursorControl();
    ~CSCursorControl();

/**
@brief  功能描述:
        使能自动隐藏光标功能
        默认光标保持显示时间为2S，由于效率的考虑会有0-200ms的误差
       （由于版本的改变此默认值或误差可能会改变，如果需要请使用setKeepTime设置光标保持显示的时间）

@param[in]
int nFlag   自动隐藏光标功能的开关标志，0为关闭，1为开启

@return
int         设置的nFlag参数值和之前设置的值一致，返回   0
            设置为无法识别的参数（目前只识别0和1）返回  -1
            设置成功返回                            1

@exception
@note
*/
    int enableAutoHideCursor(int nFlag);

/**
@brief  功能描述:
        设置光标保持显示的时间，目前当设置时间值小于300ms将自动设为保持显示时间为300ms（自动隐藏光标功能开启）

@param[in]
int nTime   光标保持显示的时间，单位为ms

@return
int         传入参数为负值返回            -1
            设置的值与当前使用的值相同返回   0
            设置成功返回                  1

@exception
@note
*/
    int setKeepTime(int nTime);

/**
@brief  功能描述:
        隐藏光标

@param[in]
            无

@return
int         执行成功返回                            1
            如果光标已经是隐藏状态，不作任何动作返回    0

@exception
@note
*/
    int hideCursor();

/**
@brief  功能描述:
        显示光标

@param[in]
            无

@return
int         执行成功返回                            1
            如果光标已经是显示状态，不作任何动作返回    0

@exception
@note
*/
    int showCursor();

/**
@brief  功能描述:
        移动光标（目前由于依赖平台，可能无法正常运行）

@param[in]
int nX      设置光标移动位置的x横向坐标值
int nY      设置光标移动位置的y纵向坐标值

@return
int         执行成功返回                      1
            如果设置坐标与当前光标位置一致返回   0

@exception
@note
*/
    int moveCursor(int nX, int nY);

/**
@brief  功能描述:
        获取当前光标位置

@param[out]
int nX      获取当前光标位置的x横向坐标值
int nY      获取当前光标位置的y纵向坐标值

@return
int         传入指针pnX或pnY为NULL返回  -1
            执行成功返回                1

@exception
@note
*/
    int getCursorPos(int* pnX, int* pnY);

/**
=============================================================
    3 static public members inherited from CSCuteSingleton
=============================================================

@brief  功能描述:
        获得光标控制类的唯一实例，如果实例并不存在将创建一个，如果存在则返回实例指针

@param[in]
        无

@return
CSCursorControl*  返回光标控制唯一实例指针

@exception
@note

    static CSCursorControl* getInstance();

@brief  功能描述:
        获得光标控制类的唯一实例的指针，如果实例不存在不会创建，返回NULL

@param[in]
        无

@return
CSCursorControl*  返回光标控制唯一实例指针

@exception
@note

    static CSCursorControl* instance();


@brief  功能描述:
        释放光标控制类唯一实例

@param[in]
        无

@return
int         实例存在并成功删除返回     1
            实例不存在返回           0

@exception
@note
    static int releaseInstance();
*/
};
#endif

#ifdef __cplusplus
CSCUTE_BEGIN_C_HEADER
#endif

//**********************| C interface |**********************
/**
@brief  功能描述:
        使能自动隐藏光标功能，默认光标保持显示时间为2S，由于效率的考虑会有0-200ms的误差
       （由于版本的改变此默认值或误差可能会改变，如果需要请使用CSQTSetKeepTime设置光标保持显示的时间）

@param[in]
int nFlag   自动隐藏光标功能的开关标志，0为关闭，1为开启

@return
int         设置的nFlag参数值和之前设置的值一致，返回   0
            设置为无法识别的参数（目前只识别0和1）返回  -1
            设置成功返回                            1

@exception
@note
*/
    int CSCuteEnableAutoHideCursor(int nFlag);

/**
@brief  功能描述:
        设置光标保持显示的时间，目前当设置时间值小于300ms将自动设为保持显示时间为300ms（自动隐藏光标功能开启）

@param[in]
int nTime   光标保持显示的时间，单位为ms

@return
int         传入参数为负值返回            -1
            设置的值与当前使用的值相同返回   0
            设置成功返回                  1

@exception
@note
*/
    int CSCuteSetKeepTime(int nTime);

/**
@brief  功能描述:
        隐藏光标

@param[in]
            无

@return
int         执行成功返回                            1
            如果光标已经是隐藏状态，不作任何动作返回    0

@exception
@note
*/
    int CSCuteHideCursor();

/**
@brief  功能描述:
        显示光标

@param[in]
            无

@return
int         执行成功返回                            1
            如果光标已经是显示状态，不作任何动作返回    0

@exception
@note
*/
    int CSCuteShowCursor();

/**
@brief  功能描述:
        移动光标（目前由于依赖平台，可能无法正常运行）

@param[in]
int nX      设置光标移动位置的x横向坐标值
int nY      设置光标移动位置的y纵向坐标值

@return
int         执行成功返回                      1
            如果设置坐标与当前光标位置一致返回   0

@exception
@note
*/
    int CSCuteMoveCursor(int nX, int nY);

/**
@brief  功能描述:
        获取当前光标位置

@param[out]
int nX      获取当前光标位置的x横向坐标值
int nY      获取当前光标位置的y纵向坐标值

@return
int         传入指针pnX或pnY为NULL返回  -1
            执行成功返回                1

@exception
@note
*/
    int CSCuteGetCursorPos(int* pnX, int* pnY);

/**
@brief  功能描述:
        释放使用光标控制功能所占用的资源

@param[in]
        无

@return
int         成功释放资源返回                 1
            没有资源被占用没有必要释放返回     0

@exception
@note
*/
    int CSCuteReleaseCursorControl();

#ifdef __cplusplus
CSCUTE_END_C_HEADER
#endif

#endif // CSCURSORCONTROL_H
