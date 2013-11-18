#ifndef _UDI_SYSPUBLIC_H_
#define _UDI_SYSPUBLIC_H_

#include "udi_typedef.h"
#include "udi_assert.h"
#include "udi_device.h"

#ifdef  __cplusplus
extern "C" {
#endif

//这里描述整个UDI的总体描述及版本信息，所有涉及UDI的改动，请在这里记录

/**@mainpage Unified Driver Interface(UDI) 文档
@version <B>UDI_V0.3</B> 尚未发布
- 文档正式命名为UDI
- 将部分ES注入的结构等移出UDI
- 添加遗漏的CSSYSEventMsg、CSSYSRegisterMsg枚举定义
- 增加CSDMXStart回调时机的详细说明
- 增加Filter只保证16字节深度的描述
- 细化I帧格式的详细说明

@version <B>UDI_V0.2</B> 2008/11/12
- 整理音视频播放接口，移出部分重复或者不再推荐使用的接口
- 增加pthread模块的描述
- 增加udi统一初始化接口
- 增加应用环境信息获取接口
- 增加大量模块的细节描述，移出少量接口

@version <B>UDI_V0.1</B> 2008/10/21
- 初始版本
*/

/**@brief 音视频数据打包格式及来源*/
typedef enum _CSSYSServiceType
{	
	CSSYS_SERVICE_TS_HW,	 	/**< TS硬件注入，一般为Tuner*/
	CSSYS_SERVICE_TS_SW,		/**< TS使用注入接口软件注入,如IPTV、PVR*/
	CSSYS_SERVICE_ES_HW,		/**< ES硬件注入，一般为Tuner*/
	CSSYS_SERVICE_ES_SW	 	    /**< ES使用注入接口软件注入,如IPTV、PVR*/
}CSSYSServiceType;

/**@brief Input Vid stream content information*/
typedef struct _CSSYSPCRInputInfo
{
	int nPID;			/**< PCR Stream PID*/
}CSSYSPCRInputInfo;

/**@brief 音视频播放事件,2个为一组,小的优先级高*/
typedef enum _CSSYSEventMsg
{
	CSSYSEVENT_SYSTEM_RUNNING,		        /**< system run */
	CSSYSEVENT_SYSTEM_STANDBY,		        /**< system stanby */	
	CSSYSEVENT_SIGNAL_LOCK, 		        /**< tuner locked */
	CSSYSEVENT_SIGNAL_LOST,			        /**< tuner lost */	
	CSSYSEVENT_SMARTCARD_IN,  		        /**< insert smart card*/
	CSSYSEVENT_SMARTCARD_OUT,		        /**< remove smart card*/	
	CSSYSEVENT_FRONTTS_RUN,			        /**< front TS be running event*/
	CSSYSEVENT_FRONTTS_STOP,		        /**< front TS stop event*/	
	CSSYSEVENT_PG_UNLOCK,			        /**< video decoder stop event*/
	CSSYSEVENT_PG_LOCK,			 	        /**< video decoder be running event*/
	CSSYSEVETN_DEOCODE_FRAME_COUNT_CONTINUE,/**< 10 需要添加说明*/
	CSSYSEVETN_DEOCODE_FRAME_COUNT_STOP,	/**< 11 需要添加说明*/
	CSSYSEVENT_SERVICE_NOLOCK,				/**< 12 需要添加说明*/
	CSSYSEVENT_SERVICE_LOCK,				/**< 13 需要添加说明*/
	CSSYSEVENT_USER_OPEN,					/**< 14 需要添加说明*/
	CSSYSEVENT_USER_CLOSE,					/**< 15 需要添加说明*/
	CSSYSEVENT_SMS_OPEN,					/**< 为了兼容 CSUSPSMSEnable/CSUSPSMSDisable*/
	CSSYSEVENT_SMS_CLOSE,					/**< 需要添加说明*/
	CSSYSEVENT_START_PLAY, 				    /**< 播放*/
	CSSYSEVENT_STOP_PLAY,					/**< 需要添加说明*/
	CSSYSEVENT_VIDEO_IFRAME_SHOW,		    /**< video层显示Iframe*/
	CSSYSEVENT_VIDEO_IFRAME_HIDE,		    /**< video层隐藏Iframe*/
	CSSYSEVENT_MAX
}CSSYSEventMsg; 

/**@brief 视频注册事件类型*/
typedef enum _CSSYSRegisterMsg
{
	CSSYS_MSG_FRONT_TS		/**< 前端停播相关事件*/
}CSSYSRegisterMsg;

#ifdef  __cplusplus
}
#endif

#endif


