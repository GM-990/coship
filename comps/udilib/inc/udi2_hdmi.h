/**@defgroup CSHDMI[CSHDMI]UDI HDMI驱动接口
@brief 本模块主要定义了UDI HDMI驱动接口
@brief HDMI，英文全称是High Definition Multimedia Interface，中文名称是高清晰多媒体接口的缩写。2002年4月，日立、松下、飞利浦、索尼、汤姆逊、东芝和Silicon Image七家公司联合组成HDMI组织。HDMI能高品质地传输未经压缩的高清视频和多声道音频数据，最高数据传输速度为5Gbps。同时无需在信号传送前进行数/模或者模/数转换，可以保证最高质量的影音信号传送 。基于udi的MMCP中间层不会用到这个模块，但是在具体的基于MMCP的项目的配置文件中应该会用到。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

@version 0.2 2008/10/17 评审后的第一个版本
@version 0.1 2008/10/16 演示版本
@{
*/

#ifndef  _CS_UDIHDMI_H
#define  _CS_UDIHDMI_H

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_error.h"
#include "udi2_screen.h"

#ifdef  __cplusplus
extern "C" {
#endif

#ifndef CSUDI_HDMI_ERROR_BASE
#define CSUDI_HDMI_ERROR_BASE (CSUDI_VOUT_ERROR_BASE + 100)
#endif /* !CSUDI_HDMI_ERROR_BASE */

/**@brief HDMI设备操作相关错误代码值*/
enum
{
	CSUDIHDMI_ERROR_BAD_PARAMETER = CSUDI_HDMI_ERROR_BASE,	///< 参数错误
	CSUDIHDMI_ERROR_FEATURE_NOT_SUPPORTED, 				///< 不支持操作
	CSUDIHDMI_ERROR_UNKNOWN_ERROR, 						///< 一般错误
	CSUDIHDMI_ERROR_NO_MEMORY, 								///< 无可用内存
	CSUDIHDMI_ERROR_INVALID_DEVICE_ID,						///< 非法设备ID
	CSUDIHDMI_ERROR_ALREADY_ADDED,							///< 回调函数已经存在
	CSUDIHDMI_ERROR_NO_CALLBACK								///< 回调函数不存在或已删除
};

/**@brief  选择HDMI 音频*/
typedef enum _CSUDIHDMISinkAudioMode_E
{
	EM_UDIHDMI_SINK_AUDIO_MODE_INVALID,		/**< 无效格式 */
	EM_UDIHDMI_SINK_AUDIO_MODE_PCM,				/**< 立体声 PCM */
	EM_UDIHDMI_SINK_AUDIO_MODE_PCM_2CH=EM_UDIHDMI_SINK_AUDIO_MODE_PCM,	/**< 立体声 PCM */
	EM_UDIHDMI_SINK_AUDIO_MODE_COMPRESSED,	/**<   AC3 压缩*/
	EM_UDIHDMI_SINK_AUDIO_MODE_PCM_6CH,		/**< 6 声道PCM*/
	EM_UDIHDMI_SINK_AUDIO_MODE_COUNT			/**< mode 计数*/
}CSUDIHDMISinkAudioMode_E;

/**@brief  HDMI Sink Capabilities 定义*/
typedef struct CSUDIHDMISinkCapabilities_S
{
	CSUDISCREENResolution_E m_ePreferVideoResolution;								/**< video Timing Mode选择 */
	CSUDISCREENResolution_E m_eSupportedVideoMode[EM_UDISCREEN_RESOLUTION_NUM];				/**< 某种video Resolution是否支持*/
	CSUDIHDMISinkAudioMode_E m_ePreferAudioMode;						/**< HDMI音频输出模式选择*/
	CSUDIHDMISinkAudioMode_E m_eSupportedAudioMode[EM_UDIHDMI_SINK_AUDIO_MODE_COUNT];	/**< 某种HDMI音频输出模式 是否支持*/
}CSUDIHDMISinkCapabilities_S;

/**@brief  HDCP参数定义,HDCP是High-bandwidth Digital Content Protection的缩写,中文可称作"HDCP数字内容保护"*/
typedef struct _CSUDIHdcpKeyInfo_S
{ 
   void     *pucHdcpKsv ;                	/**< 指向存放密钥选择向量的buffer指针,HDCP IV[2]*/
   void     *pucEncryptedHdcpKeys;        	/**< 指向存放设备私有密钥集合的buffer指针,HDCP Key Set*/
   int     nKsvLength;                  	/**< 密钥选择向量的buffer的大小,must be 8*/
   int     nHdcpKeysLength;             	/**< 设备私有密钥集合的buffer大小,must be 312*/
}CSUDIHdcpKeyInfo_S;

/**@brief HDMI属性定义,用于设置/获取HDMI的一些属性*/
typedef enum _CSUDIHDMIPropertyType_E
{
	EM_UDIHDMI_VENDOR_NAME,    		/**< HDMI SPD Vendor Name字段，其值为字符串，长度最多8字节*/
	EM_UDIHDMI_PRODUCT_DESCRIPTION,	/**< HDMI SPD Product Description字段，其值为字符串，长度最多16字节*/
	EM_UDIHDMI_HDCP_KEY_INFO,  		/**< HDMI HDCP KEY,  CSUDIHdcpKeyInfo_S 指针 */
	EM_UDIHDMI_LINK_STATUS,    		/**< HDMI LINK STATUS 参见CSUDIHDMILinkStatus_E*/
	EM_UDIHDMI_PROPERT_COUNT		
}CSUDIHDMIPropertyType_E;

/**@brief HDMI 事件类型*/
typedef enum
{
    EM_UDIHDMI_RECEIVER_PLUGIN,	/**< HDMI sink plug in */
    EM_UDIHDMI_RECEIVER_PLUGOUT	/**< HDMI sink plug out */
}CSUDIHDMIEvent_E;

/**@brief HDMI 状态类型*/
typedef enum
{
	EM_UDIHDMI_STATUS_UNLINK,				/**< HDMI not link */
    EM_UDIHDMI_STATUS_LINK_HDCP_SUCCESSED,  /**< HDMI link, HDCP 交互成功 */
	EM_UDIHDMI_STATUS_LINK_HDCP_FAILED,  	/**< HDMI link, HDCP 交互失败 */
	EM_UDIHDMI_STATUS_LINK_HDCP_IGNORED, 	/**< HDMI link, HDCP 没有启动*/
	EM_UDIHDMI_STATUS_MAX
}CSUDIHDMILinkStatus_E;

/**
@brief HDMI事件回调函数定义
@param[in] eEvt  事件类型，见CSUDIHDMIEvent_E
@param[in] nHdmiIndex HDMI设备索引
@param[in] pvEventData 事件的附加数据，见CSUDIHDMIEvent_E里面的具体说明，如果没有特别说时，为NULL
@param[in] pvUserData 用户注册事件时传入的数据
@return none
*/
typedef void (*CSUDIHDMICallback_F)(CSUDIHDMIEvent_E eEvt, int nHdmiIndex, void* pvEventData, void * pvUserData);

/**
@brief 获取HDMI接口的总数
@param[out] pnHdmiDeviceCount 返回HDMI接口的总数
@return 成功获取则返回CSUDI_SUCCESS；失败则返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSUDIHDMIGetCount(int * pnHdmiDeviceCount);

/**
@brief	获取HDMI接收端设备的特性

@param[in] nHdmiIndex HDMI设备索引，通过CSUDIHDMIGetCount可以得到索引范围
@param[in]	pCaps 	指向HDMI逻辑设备的句柄	暂时没有用到这个参数，直接传NULL
@return 成功获取则返回CSUDI_SUCCESS；失败则返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSUDIHDMIGetSinkCapabilities(int nHdmiIndex, CSUDIHDMISinkCapabilities_S *pCaps);

/**
@brief 设置HDMI属性

用于设置HDMI的属性,进行HDMI的配置
@param[in] nHdmiIndex HDMI设备索引，通过CSUDIHDMIGetCount可以得到索引范围
@param[in] ePropetyType 待设置的属性项目
@param[in] pvData 属性项值,其内容见CSUDIHDMIPropertyType_E枚举中对各种属性的描述
@return 成功获取则返回CSUDI_SUCCESS；失败则返回CSUDI_FAILURE
@note 数据解析出错会返回FALSE，但字符串数据超长时会自动截断
*/
CSUDI_Error_Code CSUDIHDMISetProperty(int nHdmiIndex, CSUDIHDMIPropertyType_E ePropetyType, void* pvData);

/**
@brief  注册指定事件类型的回调函数

@param[in] nHdmiIndex HDMI设备索引，通过CSUDIHDMIGetCount可以得到索引范围
@param[in] fnHdmiCallback 回调函数指针，详见CSUDIHDMICallback_F定义
@param[in] pvUserData 用户传入的自定义指针，用来通知驱动返回给用户的数据信息，同CSUDISCREENCallback的pvUserData
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 回调函数最少支持8个。如果用户注册个数超过所支持个数，则允许失败并返回错误代码值。
@note fnHdmiCallback和pvUserData唯一确定一个注册，当两次注册它们完全一样时，第二次将返回CSUDIHDMI_ERROR_ALREADY_ADDED，但只会通知一次；多次注册的两个参数不完全一样时则通知多次
*/
CSUDI_Error_Code CSUDIHDMIAddCallback(int nHdmiIndex, CSUDIHDMICallback_F fnHdmiCallback,void * pvUserData );

/**
@brief 删除指定事件类型的回调函数

@param[in] nHdmiIndex HDMI设备索引，通过CSUDIHDMIGetCount可以得到索引范围
@param[in] fnHdmiCallback 回调函数指针，详见CSUDIHDMICallback_F定义
@param[in] pvUserData 同CSUDISCREENCallback的pvUserData
@return 成功返回CSUDI_SUCCESS；如果该回调函数根本就没有注册，将返回CSUDIHDMI_ERROR_NO_CALLBACK
@note fnScreenCallback和pvUserData唯一确定一个注册，必须两者与注册时完全一样方能成功删除
*/
CSUDI_Error_Code CSUDIHDMIDelCallback(int nHdmiIndex, CSUDIHDMICallback_F fnHdmiCallback, void * pvUserData );
	
/**
@brief 获取HDMI属性

用于获取HDMI的属性
@param[in] nHdmiIndex HDMI设备索引，通过CSUDIHDMIGetCount可以得到索引范围
@param[in] ePropetyType 待获取的属性项目
@param[out] pvData 属性项值,其内容见CSUDIHDMIPropertyType_E枚举中对各种属性的描述
@param[out] pnLen 属性值长度
@return 成功获取则返回CSUDI_SUCCESS；失败则返回CSUDI_FAILURE
@note 如果CSUDIHDMIPropertyType_E为EM_UDIHDMI_LINK_STATUS:则pvData为CSUDIHDMILinkStatus_E，pnLen不使用;
*/
CSUDI_Error_Code CSUDIHDMIGetProperty(int nHdmiIndex, CSUDIHDMIPropertyType_E ePropetyType, void* pvData, int* pnLen);


 #ifdef  __cplusplus
}
#endif

/** @} */

#endif  /* _CS_UDIHDMI_H */


