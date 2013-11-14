/**@defgroup CSHDMI[CSHDMI]UDI HDMI�����ӿ�
@brief ��ģ����Ҫ������UDI HDMI�����ӿ�
@brief HDMI��Ӣ��ȫ����High Definition Multimedia Interface�����������Ǹ�������ý��ӿڵ���д��2002��4�£����������¡������֡����ᡢ��ķѷ����֥��Silicon Image�߼ҹ�˾�������HDMI��֯��HDMI�ܸ�Ʒ�ʵش���δ��ѹ���ĸ�����Ƶ�Ͷ�������Ƶ���ݣ�������ݴ����ٶ�Ϊ5Gbps��ͬʱ�������źŴ���ǰ������/ģ����ģ/��ת�������Ա�֤���������Ӱ���źŴ��� ������udi��MMCP�м�㲻���õ����ģ�飬�����ھ���Ļ���MMCP����Ŀ�������ļ���Ӧ�û��õ���

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

@version 0.2 2008/10/17 �����ĵ�һ���汾
@version 0.1 2008/10/16 ��ʾ�汾
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

/**@brief HDMI�豸������ش������ֵ*/
enum
{
	CSUDIHDMI_ERROR_BAD_PARAMETER = CSUDI_HDMI_ERROR_BASE,	///< ��������
	CSUDIHDMI_ERROR_FEATURE_NOT_SUPPORTED, 				///< ��֧�ֲ���
	CSUDIHDMI_ERROR_UNKNOWN_ERROR, 						///< һ�����
	CSUDIHDMI_ERROR_NO_MEMORY, 								///< �޿����ڴ�
	CSUDIHDMI_ERROR_INVALID_DEVICE_ID,						///< �Ƿ��豸ID
	CSUDIHDMI_ERROR_ALREADY_ADDED,							///< �ص������Ѿ�����
	CSUDIHDMI_ERROR_NO_CALLBACK								///< �ص����������ڻ���ɾ��
};

/**@brief  ѡ��HDMI ��Ƶ*/
typedef enum _CSUDIHDMISinkAudioMode_E
{
	EM_UDIHDMI_SINK_AUDIO_MODE_INVALID,		/**< ��Ч��ʽ */
	EM_UDIHDMI_SINK_AUDIO_MODE_PCM,				/**< ������ PCM */
	EM_UDIHDMI_SINK_AUDIO_MODE_PCM_2CH=EM_UDIHDMI_SINK_AUDIO_MODE_PCM,	/**< ������ PCM */
	EM_UDIHDMI_SINK_AUDIO_MODE_COMPRESSED,	/**<   AC3 ѹ��*/
	EM_UDIHDMI_SINK_AUDIO_MODE_PCM_6CH,		/**< 6 ����PCM*/
	EM_UDIHDMI_SINK_AUDIO_MODE_COUNT			/**< mode ����*/
}CSUDIHDMISinkAudioMode_E;

/**@brief  HDMI Sink Capabilities ����*/
typedef struct CSUDIHDMISinkCapabilities_S
{
	CSUDISCREENResolution_E m_ePreferVideoResolution;								/**< video Timing Modeѡ�� */
	CSUDISCREENResolution_E m_eSupportedVideoMode[EM_UDISCREEN_RESOLUTION_NUM];				/**< ĳ��video Resolution�Ƿ�֧��*/
	CSUDIHDMISinkAudioMode_E m_ePreferAudioMode;						/**< HDMI��Ƶ���ģʽѡ��*/
	CSUDIHDMISinkAudioMode_E m_eSupportedAudioMode[EM_UDIHDMI_SINK_AUDIO_MODE_COUNT];	/**< ĳ��HDMI��Ƶ���ģʽ �Ƿ�֧��*/
}CSUDIHDMISinkCapabilities_S;

/**@brief  HDCP��������,HDCP��High-bandwidth Digital Content Protection����д,���Ŀɳ���"HDCP�������ݱ���"*/
typedef struct _CSUDIHdcpKeyInfo_S
{ 
   void     *pucHdcpKsv ;                	/**< ָ������Կѡ��������bufferָ��,HDCP IV[2]*/
   void     *pucEncryptedHdcpKeys;        	/**< ָ�����豸˽����Կ���ϵ�bufferָ��,HDCP Key Set*/
   int     nKsvLength;                  	/**< ��Կѡ��������buffer�Ĵ�С,must be 8*/
   int     nHdcpKeysLength;             	/**< �豸˽����Կ���ϵ�buffer��С,must be 312*/
}CSUDIHdcpKeyInfo_S;

/**@brief HDMI���Զ���,��������/��ȡHDMI��һЩ����*/
typedef enum _CSUDIHDMIPropertyType_E
{
	EM_UDIHDMI_VENDOR_NAME,    		/**< HDMI SPD Vendor Name�ֶΣ���ֵΪ�ַ������������8�ֽ�*/
	EM_UDIHDMI_PRODUCT_DESCRIPTION,	/**< HDMI SPD Product Description�ֶΣ���ֵΪ�ַ������������16�ֽ�*/
	EM_UDIHDMI_HDCP_KEY_INFO,  		/**< HDMI HDCP KEY,  CSUDIHdcpKeyInfo_S ָ�� */
	EM_UDIHDMI_LINK_STATUS,    		/**< HDMI LINK STATUS �μ�CSUDIHDMILinkStatus_E*/
	EM_UDIHDMI_PROPERT_COUNT		
}CSUDIHDMIPropertyType_E;

/**@brief HDMI �¼�����*/
typedef enum
{
    EM_UDIHDMI_RECEIVER_PLUGIN,	/**< HDMI sink plug in */
    EM_UDIHDMI_RECEIVER_PLUGOUT	/**< HDMI sink plug out */
}CSUDIHDMIEvent_E;

/**@brief HDMI ״̬����*/
typedef enum
{
	EM_UDIHDMI_STATUS_UNLINK,				/**< HDMI not link */
    EM_UDIHDMI_STATUS_LINK_HDCP_SUCCESSED,  /**< HDMI link, HDCP �����ɹ� */
	EM_UDIHDMI_STATUS_LINK_HDCP_FAILED,  	/**< HDMI link, HDCP ����ʧ�� */
	EM_UDIHDMI_STATUS_LINK_HDCP_IGNORED, 	/**< HDMI link, HDCP û������*/
	EM_UDIHDMI_STATUS_MAX
}CSUDIHDMILinkStatus_E;

/**
@brief HDMI�¼��ص���������
@param[in] eEvt  �¼����ͣ���CSUDIHDMIEvent_E
@param[in] nHdmiIndex HDMI�豸����
@param[in] pvEventData �¼��ĸ������ݣ���CSUDIHDMIEvent_E����ľ���˵�������û���ر�˵ʱ��ΪNULL
@param[in] pvUserData �û�ע���¼�ʱ���������
@return none
*/
typedef void (*CSUDIHDMICallback_F)(CSUDIHDMIEvent_E eEvt, int nHdmiIndex, void* pvEventData, void * pvUserData);

/**
@brief ��ȡHDMI�ӿڵ�����
@param[out] pnHdmiDeviceCount ����HDMI�ӿڵ�����
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻�CSUDI_FAILURE
*/
CSUDI_Error_Code CSUDIHDMIGetCount(int * pnHdmiDeviceCount);

/**
@brief	��ȡHDMI���ն��豸������

@param[in] nHdmiIndex HDMI�豸������ͨ��CSUDIHDMIGetCount���Եõ�������Χ
@param[in]	pCaps 	ָ��HDMI�߼��豸�ľ��	��ʱû���õ����������ֱ�Ӵ�NULL
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻�CSUDI_FAILURE
*/
CSUDI_Error_Code CSUDIHDMIGetSinkCapabilities(int nHdmiIndex, CSUDIHDMISinkCapabilities_S *pCaps);

/**
@brief ����HDMI����

��������HDMI������,����HDMI������
@param[in] nHdmiIndex HDMI�豸������ͨ��CSUDIHDMIGetCount���Եõ�������Χ
@param[in] ePropetyType �����õ�������Ŀ
@param[in] pvData ������ֵ,�����ݼ�CSUDIHDMIPropertyType_Eö���жԸ������Ե�����
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻�CSUDI_FAILURE
@note ���ݽ�������᷵��FALSE�����ַ������ݳ���ʱ���Զ��ض�
*/
CSUDI_Error_Code CSUDIHDMISetProperty(int nHdmiIndex, CSUDIHDMIPropertyType_E ePropetyType, void* pvData);

/**
@brief  ע��ָ���¼����͵Ļص�����

@param[in] nHdmiIndex HDMI�豸������ͨ��CSUDIHDMIGetCount���Եõ�������Χ
@param[in] fnHdmiCallback �ص�����ָ�룬���CSUDIHDMICallback_F����
@param[in] pvUserData �û�������Զ���ָ�룬����֪ͨ�������ظ��û���������Ϣ��ͬCSUDISCREENCallback��pvUserData
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note �ص���������֧��8��������û�ע�����������֧�ָ�����������ʧ�ܲ����ش������ֵ��
@note fnHdmiCallback��pvUserDataΨһȷ��һ��ע�ᣬ������ע��������ȫһ��ʱ���ڶ��ν�����CSUDIHDMI_ERROR_ALREADY_ADDED����ֻ��֪ͨһ�Σ����ע���������������ȫһ��ʱ��֪ͨ���
*/
CSUDI_Error_Code CSUDIHDMIAddCallback(int nHdmiIndex, CSUDIHDMICallback_F fnHdmiCallback,void * pvUserData );

/**
@brief ɾ��ָ���¼����͵Ļص�����

@param[in] nHdmiIndex HDMI�豸������ͨ��CSUDIHDMIGetCount���Եõ�������Χ
@param[in] fnHdmiCallback �ص�����ָ�룬���CSUDIHDMICallback_F����
@param[in] pvUserData ͬCSUDISCREENCallback��pvUserData
@return �ɹ�����CSUDI_SUCCESS������ûص�����������û��ע�ᣬ������CSUDIHDMI_ERROR_NO_CALLBACK
@note fnScreenCallback��pvUserDataΨһȷ��һ��ע�ᣬ����������ע��ʱ��ȫһ�����ܳɹ�ɾ��
*/
CSUDI_Error_Code CSUDIHDMIDelCallback(int nHdmiIndex, CSUDIHDMICallback_F fnHdmiCallback, void * pvUserData );
	
/**
@brief ��ȡHDMI����

���ڻ�ȡHDMI������
@param[in] nHdmiIndex HDMI�豸������ͨ��CSUDIHDMIGetCount���Եõ�������Χ
@param[in] ePropetyType ����ȡ��������Ŀ
@param[out] pvData ������ֵ,�����ݼ�CSUDIHDMIPropertyType_Eö���жԸ������Ե�����
@param[out] pnLen ����ֵ����
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻�CSUDI_FAILURE
@note ���CSUDIHDMIPropertyType_EΪEM_UDIHDMI_LINK_STATUS:��pvDataΪCSUDIHDMILinkStatus_E��pnLen��ʹ��;
*/
CSUDI_Error_Code CSUDIHDMIGetProperty(int nHdmiIndex, CSUDIHDMIPropertyType_E ePropetyType, void* pvData, int* pnLen);


 #ifdef  __cplusplus
}
#endif

/** @} */

#endif  /* _CS_UDIHDMI_H */


