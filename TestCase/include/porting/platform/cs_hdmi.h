/**@defgroup CSHDMI[CSHDMI]ͬ����Ŀר��HDMI�����ӿ�
@brief ��ģ����Ҫ������ͬ����Ŀר��ƽ̨��HDMI�����ӿ�
@brief HDMI��Ӣ��ȫ����High Definition Multimedia Interface�����������Ǹ�������ý��ӿڵ���д��2002��4�£����������¡������֡����ᡢ��ķѷ����֥��Silicon Image�߼ҹ�˾�������HDMI��֯��HDMI�ܸ�Ʒ�ʵش���δ��ѹ���ĸ�����Ƶ�Ͷ�������Ƶ���ݣ�������ݴ����ٶ�Ϊ5Gbps��ͬʱ�������źŴ���ǰ������/ģ����ģ/��ת�������Ա�֤���������Ӱ���źŴ��� ������udi��MMCP�м�㲻���õ����ģ�飬�����ھ���Ļ���MMCP����Ŀ�������ļ���Ӧ�û��õ���

@version 0.2 2008/10/17 �����ĵ�һ���汾
@version 0.1 2008/10/16 ��ʾ�汾
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


/**@brief  ѡ��HDMI ��Ƶ*/
typedef enum _CSHDMIOutputAudioMode_E
{
	EM_HDMI_OUTPUT_AUDIO_MODE_PCM,				/**< ������ PCM */
	EM_HDMI_OUTPUT_AUDIO_MODE_PCM_2CH=EM_HDMI_OUTPUT_AUDIO_MODE_PCM,	/**< ������ PCM */
	EM_HDMI_OUTPUT_AUDIO_MODE_COMPRESSED,	/**<   AC3 ѹ��*/
	EM_HDMI_OUTPUT_AUDIO_MODE_PCM_6CH,		/**< 6 ����PCM*/
	EM_HDMI_OUTPUT_AUDIO_MODE_COUNT			/**< ����ģʽ*/
}CSHDMIOutputAudioMode_E;

/**@brief  HDMI Capabilities ����*/
typedef struct CSHDMICapabilities_S
{
	CSVIDTimingMode m_ePreferVideoMode;								/**< video Timing Modeѡ�� */
	BOOL m_bVideoModeSupported[CSVID_TIMING_MODE_NUM];				/**< ĳ��video Timing Mode�Ƿ�֧��*/
	CSHDMIOutputAudioMode_E m_ePreferAudioMode;						/**< HDMI��Ƶ���ģʽѡ��*/
	BOOL m_bAudioModeSupported[EM_HDMI_OUTPUT_AUDIO_MODE_COUNT];	/**< ĳ��HDMI��Ƶ���ģʽ �Ƿ�֧��*/
}CSHDMICapabilities_S;



/**@brief  HDCP��������,HDCP��High-bandwidth Digital Content Protection����д,���Ŀɳ���"HDCP�������ݱ���"*/
typedef struct _CSHdcpKeyInfo_S
{ 
   BYTE     *pucHdcpKsv ;                	/**< ָ������Կѡ��������bufferָ��,HDCP IV[2]*/
   BYTE     *pucEncryptedHdcpKeys;        	/**< ָ�����豸˽����Կ���ϵ�bufferָ��,HDCP Key Set*/
   WORD     wKsvLength;                  	/**< ��Կѡ��������buffer�Ĵ�С,must be 8*/
   WORD     wHdcpKeysLength;             	/**< �豸˽����Կ���ϵ�buffer��С,must be 312*/
}CSHdcpKeyInfo_S;


/**
@brief  ����HDCP KEY

@param[in]  hHandle  ָ��HDCP KEY��Ϣ��CSHdcpKeyInfo_S�ṹָ��
@param[in]  pCaps   ָ��HDMI�߼��豸�ľ��  ��ʱû���õ����������ֱ�Ӵ�NULL
@return  ����1����SUCESS,����2����FAILURE
*/
BOOL CSHDMOutputGetCapabilities(HCSHANDLE hHandle,CSHDMICapabilities_S *pCaps);

/**
@brief  ����HDCP KEY

@param[in] hHdcpDevice ָ��HDMI�߼��豸�ľ��  ��ʱû���õ����������ֱ�Ӵ�NULL
@param[in] pHdcpKey ָ��HDCP KEY��Ϣ��CSHdcpKeyInfo_S�ṹָ��
@return ����1����SUCESS,����2����FAILURE
*/
int CSSetHdcpKey (HCSHANDLE  hHdcpDevice,CSHdcpKeyInfo_S * pHdcpKey );

/**
@brief  ��ʼ��HDCP KEY

@param[in]  pOutHdcpKeyInfo   ָ����Ҫ��ʼ��CSHdcpKeyInfo_S�ṹ��ָ��
@return  None
@ this function is meaningless in ST710X
*/
void CSInitHdcpKeyInfo(CSHdcpKeyInfo_S * pOutHdcpKeyInfo);

/**@brief HDMI���Զ���,��������/��ȡHDMI��һЩ����*/
typedef enum _CSHDMIPropetyType_E
{
	EM_HDMI_VENDOR_NAME,    /**< HDMI SPD Vendor Name�ֶΣ���ֵΪ�ַ������������8�ֽ�*/
	EM_HDMI_PRODUCT_DES,    /**< HDMI SPD Product Description�ֶΣ���ֵΪ�ַ������������16�ֽ�*/
	EM_HDMI_HDCP_KEY_INFO,  /**< HDMI HDCP KEY,  CSHdcpKeyInfo_S ָ�� */
	EM_HDMI_INFO_COUNT		
}CSHDMIPropetyType_E;

/**
@brief ����HDMI����

��������HDMI������,����HDMI������
@param[in] hHandle ָ��HDMI�߼��豸�ľ������ʱû���õ�������������봫NULL
@param[in] ePropetyType �����õ�������Ŀ
@param[in] pvData ������ֵ,�����ݼ�CSHDMIPropetyType_Eö���жԸ������Ե�����
@param[in] dwDataLength pvDataָ�����ݵĳ���
@return ���óɹ�����TRUE;ʧ�ܷ���FALSE;
@note ���ݽ�������᷵��FALSE�����ַ������ݳ���ʱ���Զ��ض�
*/
BOOL CSHDMISetProperty(HCSHANDLE hHandle,CSHDMIPropetyType_E ePropetyType,PVOID pvData,DWORD dwDataLength);
/**
@brief ����HDMI����

���ڻ�ȡHDMI������,����HDMI������
@param[in] hHandle ָ��HDMI�߼��豸�ľ������ʱû���õ�������������봫NULL
@param[in] ePropetyType �����õ�������Ŀ
@param[out] pvData ������ֵ,�����ݼ�CSHDMIPropetyType_Eö���жԸ������Ե�����
@param[out] dwDataLength pvDataָ�����ݵĳ���
@return ��ȡ�ɹ�����TRUE;ʧ�ܷ���FALSE;
@note ���ݽ�������᷵��FALSE�����ַ������ݳ���ʱ���Զ��ض�
*/
BOOL CSHDMIGetProperty(HCSHANDLE hHandle,CSHDMIPropetyType_E ePropetyType,PVOID pvData,DWORD *pdwDataLength);

/**
@brief ��ȡHDMI�ӿڵ�����
@param[out] pnHdmiDeviceCount ����HDMI�ӿڵ�����
@return �ɹ���ȡ�򷵻�TRUE��ʧ���򷵻�FALSE
*/
BOOL CSHDMIGetCount(int * pnHdmiDeviceCount);

#ifdef  __cplusplus
}
#endif

/** @} */

#endif 


