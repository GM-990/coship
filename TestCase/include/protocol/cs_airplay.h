#ifndef _CS_AIRPLAY_H_
#define _CS_AIRPLAY_H_

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C" 
{
#endif

/**@brief  回调消息类型 */
typedef enum
{
    EM_AIRPLAY_MEDIA_STOP = 0,              ///<停止指令       ，消息值(pvPara)无 
    EM_AIRPLAY_MEDIA_PLAY,                    ///<播放音视频或图片指令 ，消息值(pvPara) 为ascii码的字符串类型的JSON字符串，如：{fileUrl:'%s', metaData:{fileType:2,fileName:''}
    									    ///<fileUrl: 要播放的音视频或图片的URL地址;
    									    ///<fileType：播放的资源类型：1 代表音乐；2代表视频；3代表图片
    									    ///<fileName: 要播放的文件名，可为空。
    EM_AIRPLAY_MEDIA_PAUSE,		    ///<暂停指令       ，消息值(pvPara)无 
    EM_AIRPLAY_MEDIA_RESUME,               ///<恢复指令       ，消息值(pvPara):开始播放的位置，单位为秒
    EM_AIRPLAY_MEDIA_SEEK,			    ///<查找指令       ，消息值(pvPara)为ascii码的JSON字符串类型，如：{time:'%02d:%02d:%02d'}，time的单位为如00：00：00的格式，冒号两边的数据单位为：时：分：秒
    EM_AIRPLAY_UNKNOWN = 1000
}CSAirplayMediaMsg_E;

/**
@brief    回调函数
@param[in] eMsg 消息类型, 见CSAirplayMediaMsg_E定义
@param[in] pvPara 消息值
@param[in] pvUserData 用户自定义数据，注册回调函数时传入的
@return 
- 成功返回0。
- 失败返回-1
*/
typedef int (*CSOnAirplayAction_F)(CSAirplayMediaMsg_E eMsg, void* pvPara, void* pvUserData);

/**
@brief	 注册通知回调函数。
@param[in] fnCb 回调处理函数
@param[in] pvUserData 用户自定义数据，注册回调函数时传入的
@return 
- 成功返回0。
- 失败返回-1
*/
int CSAirplayAddCallback(CSOnAirplayAction_F fnCb, void* pvUserData);

#ifdef __cplusplus
}
#endif

#endif


