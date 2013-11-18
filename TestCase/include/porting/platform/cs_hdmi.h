/**@defgroup CSHDMI[CSHDMI]同洲项目专用HDMI驱动接口
@brief 本模块主要定义了同洲项目专用平台无HDMI驱动接口
@brief HDMI，英文全称是High Definition Multimedia Interface，中文名称是高清晰多媒体接口的缩写。2002年4月，日立、松下、飞利浦、索尼、汤姆逊、东芝和Silicon Image七家公司联合组成HDMI组织。HDMI能高品质地传输未经压缩的高清视频和多声道音频数据，最高数据传输速度为5Gbps。同时无需在信号传送前进行数/模或者模/数转换，可以保证最高质量的影音信号传送 。基于udi的MMCP中间层不会用到这个模块，但是在具体的基于MMCP的项目的配置文件中应该会用到。

@version 0.2 2008/10/17 评审后的第一个版本
@version 0.1 2008/10/16 演示版本
@{
*/

#ifndef  _CS_HDMI_H
#define  _CS_HDMI_H

#include "cs_typedef.h"
#include "cs_syspublic.h"
#include "cs_sysvideo.h"

#ifdef  __cplusplus
extern "C" {
#endif


/**@brief  选择HDMI 音频*/
typedef enum _CSHDMIOutputAudioMode_E
{
	EM_HDMI_OUTPUT_AUDIO_MODE_PCM,				/**< 立体声 PCM */
	EM_HDMI_OUTPUT_AUDIO_MODE_PCM_2CH=EM_HDMI_OUTPUT_AUDIO_MODE_PCM,	/**< 立体声 PCM */
	EM_HDMI_OUTPUT_AUDIO_MODE_COMPRESSED,	/**<   AC3 压缩*/
	EM_HDMI_OUTPUT_AUDIO_MODE_PCM_6CH,		/**< 6 声道PCM*/
	EM_HDMI_OUTPUT_AUDIO_MODE_COUNT			/**< 计数模式*/
}CSHDMIOutputAudioMode_E;

/**@brief  HDMI Capabilities 定义*/
typedef struct CSHDMICapabilities_S
{
	CSVIDTimingMode m_ePreferVideoMode;								/**< video Timing Mode选择 */
	BOOL m_bVideoModeSupported[CSVID_TIMING_MODE_NUM];				/**< 某种video Timing Mode是否支持*/
	CSHDMIOutputAudioMode_E m_ePreferAudioMode;						/**< HDMI音频输出模式选择*/
	BOOL m_bAudioModeSupported[EM_HDMI_OUTPUT_AUDIO_MODE_COUNT];	/**< 某种HDMI音频输出模式 是否支持*/
}CSHDMICapabilities_S;



/**@brief  HDCP参数定义,HDCP是High-bandwidth Digital Content Protection的缩写,中文可称作"HDCP数字内容保护"*/
typedef struct _CSHdcpKeyInfo_S
{ 
   BYTE     *pucHdcpKsv ;                	/**< 指向存放密钥选择向量的buffer指针,HDCP IV[2]*/
   BYTE     *pucEncryptedHdcpKeys;        	/**< 指向存放设备私有密钥集合的buffer指针,HDCP Key Set*/
   WORD     wKsvLength;                  	/**< 密钥选择向量的buffer的大小,must be 8*/
   WORD     wHdcpKeysLength;             	/**< 设备私有密钥集合的buffer大小,must be 312*/
}CSHdcpKeyInfo_S;


/**
@brief  设置HDCP KEY

@param[in]  hHandle  指向HDCP KEY信息的CSHdcpKeyInfo_S结构指针
@param[in]  pCaps   指向HDMI逻辑设备的句柄  暂时没有用到这个参数，直接传NULL
@return  返回1代表SUCESS,返回2代表FAILURE
*/
BOOL CSHDMOutputGetCapabilities(HCSHANDLE hHandle,CSHDMICapabilities_S *pCaps);

/**
@brief  设置HDCP KEY

@param[in] hHdcpDevice 指向HDMI逻辑设备的句柄  暂时没有用到这个参数，直接传NULL
@param[in] pHdcpKey 指向HDCP KEY信息的CSHdcpKeyInfo_S结构指针
@return 返回1代表SUCESS,返回2代表FAILURE
*/
int CSSetHdcpKey (HCSHANDLE  hHdcpDevice,CSHdcpKeyInfo_S * pHdcpKey );

/**
@brief  初始化HDCP KEY

@param[in]  pOutHdcpKeyInfo   指向需要初始化CSHdcpKeyInfo_S结构的指针
@return  None
@ this function is meaningless in ST710X
*/
void CSInitHdcpKeyInfo(CSHdcpKeyInfo_S * pOutHdcpKeyInfo);

/**@brief HDMI属性定义,用于设置/获取HDMI的一些属性*/
typedef enum _CSHDMIPropetyType_E
{
	EM_HDMI_VENDOR_NAME,    /**< HDMI SPD Vendor Name字段，其值为字符串，长度最多8字节*/
	EM_HDMI_PRODUCT_DES,    /**< HDMI SPD Product Description字段，其值为字符串，长度最多16字节*/
	EM_HDMI_HDCP_KEY_INFO,  /**< HDMI HDCP KEY,  CSHdcpKeyInfo_S 指针 */
	EM_HDMI_INFO_COUNT		
}CSHDMIPropetyType_E;

/**
@brief 设置HDMI属性

用于设置HDMI的属性,进行HDMI的配置
@param[in] hHandle 指向HDMI逻辑设备的句柄，暂时没有用到这个参数，必须传NULL
@param[in] ePropetyType 待设置的属性项目
@param[in] pvData 属性项值,其内容见CSHDMIPropetyType_E枚举中对各种属性的描述
@param[in] dwDataLength pvData指向内容的长度
@return 设置成功返回TRUE;失败返回FALSE;
@note 数据解析出错会返回FALSE，但字符串数据超长时会自动截断
*/
BOOL CSHDMISetProperty(HCSHANDLE hHandle,CSHDMIPropetyType_E ePropetyType,PVOID pvData,DWORD dwDataLength);
/**
@brief 设置HDMI属性

用于获取HDMI的属性,进行HDMI的配置
@param[in] hHandle 指向HDMI逻辑设备的句柄，暂时没有用到这个参数，必须传NULL
@param[in] ePropetyType 待设置的属性项目
@param[out] pvData 属性项值,其内容见CSHDMIPropetyType_E枚举中对各种属性的描述
@param[out] dwDataLength pvData指向内容的长度
@return 获取成功返回TRUE;失败返回FALSE;
@note 数据解析出错会返回FALSE，但字符串数据超长时会自动截断
*/
BOOL CSHDMIGetProperty(HCSHANDLE hHandle,CSHDMIPropetyType_E ePropetyType,PVOID pvData,DWORD *pdwDataLength);

/**
@brief 获取HDMI接口的总数
@param[out] pnHdmiDeviceCount 返回HDMI接口的总数
@return 成功获取则返回TRUE；失败则返回FALSE
*/
BOOL CSHDMIGetCount(int * pnHdmiDeviceCount);

#ifdef  __cplusplus
}
#endif

/** @} */

#endif 


