/**@defgroup TOOLSET_PLUS TOOLSET_PLUS��Ҫ����ƽ̨��ʼ����ؽӿ�
@brief ����ƽ̨��ʼ����ؽӿ�

@version 2.0.1 2009/10/21 ����
@{
*/

#ifndef UDI2_TOOLSET_PLUS_H
#define UDI2_TOOLSET_PLUS_H

#include "udi2_toolset.h"
#include "cs_ir_plus.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

typedef enum
{
        CSUDISYS_HDCP_OPEN,		/**< ��HDCP */
        CSUDISYS_HDCP_CLOSE		/**< �ر�HDCP */
}CSUDISYSHdcpMode_E;

typedef enum
{
	CSUDISYS_MEM_PARTITION_HEAP,	/**< ϵͳ�ѷ��� */
	CSUDISYS_MEM_PARTITION_AVMEM	/**< AVMEM���������ڷ���Surface�� */
}CSUDISYSMemPartition_E;

typedef enum
{
	/** CSUDICFG_TUNER_MODE
	Tuner���в���, CSUDIPlatformInit֮ǰ����
	CSUDISYSSetCfg����˵��:
		eCfgType:		CSUDICFG_TUNER_MODE
		nDeviceIndex: 	tuner index
		unValue1:		CSHDITsModeö��
		unValue1:		0
	*/
	CSUDICFG_TUNER_MODE ,	

	/** CSUDICFG_TUNER_TSIN
	����TUNER��Ӧ��TSIN, CSUDIPlatformInit֮ǰ����
	CSUDISYSSetCfg����˵��:
		eCfgType:		CSUDICFG_TUNER_TSIN
		nDeviceIndex: 	tuner index
		unValue1:		TSIN index
		unValue1:		0
	*/
	CSUDICFG_TUNER_TSIN, 
	
	/** CSUDICFG_TSIN_DMX
	����TSIN��Ӧ��DMX, CSUDIPlatformInit֮ǰ����
	CSUDISYSSetCfg����˵��:
		eCfgType:		CSUDICFG_TSIN_DMX
		nDeviceIndex: 	TSIN index
		unValue1:		Demux index
		unValue1:		0
	*/
	CSUDICFG_TSIN_DMX,

	/** CSUDICFG_HDMI_I2C
	����HDMI��Ӧ��I2C, CSUDIPlatformInit֮ǰ����
	CSUDISYSSetCfg����˵��:
		eCfgType:		CSUDICFG_HDMI_I2C
		nDeviceIndex: 	HDMI index, һ��Ϊ0
		unValue1:		I2C index
		unValue1:		0
	*/
	CSUDICFG_HDMI_I2C,

	/** CSUDICFG_HDMI_PIO
	����HDMI��Ӧ��I2C, CSUDIPlatformInit֮ǰ����
	CSUDISYSSetCfg����˵��:
		eCfgType:		CSUDICFG_HDMI_PIO
		nDeviceIndex: 	HDMI index, һ��Ϊ0
		unValue1:		GPIO index
		unValue1:		0
	*/
	CSUDICFG_HDMI_PIO,

	/** CSUDICFG_HDMI_HDCP
	����ƽ̨��ʼ��ʱ�Ƿ��HDCP ����, CSUDIPlatformInit֮ǰ����
	CSUDISYSSetCfg����˵��:
		eCfgType:		CSUDICFG_HDMI_HDCP
		nDeviceIndex: 	HDMI index, һ��Ϊ0
		unValue1:		CSUDISYSHdcpMode_E,�������ö�ٵ�˵��
		unValue1:		0
	*/
	CSUDICFG_HDMI_HDCP,

	/** CSUDICFG_SCREEN_RESOLUTION
	����ƽ̨��ʼ��ʱ�ķֱ���,  CSUDIPlatformInit֮ǰ����
	CSUDISYSSetCfg����˵��:
		eCfgType:		CSUDICFG_SCREEN_RESOLUTION
		nDeviceIndex: 	CSUDISCREENType_E, �������ö�ٵ�˵��
		unValue1:		CSUDISCREENResolution_E, �������ö�ٵ�˵��
		unValue1:		0
	*/
	CSUDICFG_SCREEN_RESOLUTION,

	/*CSUDICFG_MEMORY_PARTATION
	�����ڴ����
	CSUDISYSSetCfg ����˵����
	eCfgType	��CSUDICFG_MEMORY_PARTATION
	nDeviceIndex	��CSUDISYSMemPartition_Eö��
	unValue1	��������ʼ��ַ
	unValue1	����������
	*/
	CSUDICFG_MEMORY_PARTATION,

	/*CSUDICFG_PIXEL_FORMAT
	�����Դ���ɫģʽ
	CSUDISYSSetCfg ����˵����
	eCfgType	��CSUDICFG_PIXEL_FORMAT
	nDeviceIndex	��Ĭ��Ϊ0
	unValue1	��CSUDIOSGPixelFormat_E���������ö�ٵ�˵��
	unValue1	��0
	*/
	CSUDICFG_PIXEL_FORMAT,
	
	/*CSUDICFG_BOARD_NO
	���������
	CSUDISYSSetCfg ����˵����
	eCfgType	��CSUDICFG_BOARD_NO
	nDeviceIndex	������ע��������
	unValue1	��������ַ���ָ��
	unValue2	������ע��������
	*/
	CSUDICFG_BOARD_NO,

	CSUDICFG_MAX_NUM
}CSUDISYSCfg_E;

/**@brief �������������ģʽ*/
typedef enum 
{
	CSUDISYS_WAKEUPE_MODE_IR /**< ͨ��ң��������*/
}CSUDISYSWakeUpMode_E;

/**@brief ����ң������ֵ�ṹ*/
typedef struct
{
	CSIRProtocol_t	m_enmIRProtocol; 	/**< Э�� */
	unsigned int 		m_nUserID; 				/**< ң���������룬����ʶ��ң�������� */
	unsigned int 		m_nKeyCode;			/**< ң���������ĵ������� */
}CSUDISYSWakeUpKey_S;

#define CSUDI_MAX_WAKEUP_KEY_NUM 16

/**@brief ������������Ѳ���*/
typedef struct
{
	CSUDISYSWakeUpMode_E m_eWakeUpMode; 	/**< ����ģʽ */
	BOOL 	m_bModeEnable;	/**< �Ƿ���������ģʽ */
	union 
	{
		struct
		{
			CSUDISYSWakeUpKey_S m_astKeys[CSUDI_MAX_WAKEUP_KEY_NUM];
			int m_nKeyNum;
		} 
		m_stIRKeys; /**< IR����ģʽ��Ӧ�ļ�ֵ���� */
	} 
	m_uWakeupParams; /**< ����ģʽ��Ӧ�Ĳ����� m_bModeEnableΪFALSEʱ��������Щ����*/
}CSUDISYSWakeupParams_S;

/**
@brief ��ֲ�����ýӿڣ�����ʱ���ɸ�eCfgType����,�����CSSYSCfg_E����

�ýӿڿɱ����ö�Σ�ͬһ���͵����������һ�εĵ���Ϊ׼
@param[in] eCfgType ��������,�����CSSYSCfg_E����;
@param[in] nDeviceIndex ��Ҫ���õ��豸index,�����eCfgType�޶�Ӧ���豸ʱ,����-1;
@param[in] unValue1	���ò���1,���庬����eCfgType����;
@param[in] unValue2 ���ò���2,���庬����eCfgType����;
@return ���óɹ�����CSUDI_SUCCESS ������ʧ�ܷ��ش���ֵ��
*/
CSUDI_Error_Code CSUDISYSSetCfg(CSUDISYSCfg_E eCfgType, int nDeviceIndex, CSUDI_UINT32 unValue1, CSUDI_UINT32 unValue2);

/**
@brief ��ֲ��ϵͳ��ʼ���ӿڣ�ʵ��ϵͳ�ķ������֣�����������ϵͳ��ϵͳ����ʱ��һ��������

@return ���óɹ�����CSUDI_SUCCESS ������ʧ�ܷ��ش���ֵ��
*/
CSUDI_Error_Code CSUDISYSInit(void);

/**
@brief ��ֲ��ϵͳ��ʼ���ӿ�

�ýӿ�ֻ�ɱ�����һ�Σ�������CSUDISYSInit�����
@return ���óɹ�����CSUDI_SUCCESS ������ʧ�ܷ��ش���ֵ��
@note 
- ��ʼ����������оƬ����豸��������AV decoder��Demux�ȵȣ�����ʼ����Ӧ����ֲ��ģ�飬
	��Player,Inject,Audio,Video,Screen,Demux,Section�ȣ���ʼ��֮�����.�û�����Ҫ���ĸ��豸�ĳ�ʼ��˳��
	��ʼ�������ò���ʹ��CSUDISYSSetCfg���ã����û�����ý���Ĭ��ֵ��ʼ����
*/
CSUDI_Error_Code CSUDIPlatformInit(void);

/**
@brief ������������Ѳ���

@param[in] pstStandbyParams ��CSUDISYSStandbyParams_S
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDISYSSetWakeupParams(const CSUDISYSWakeupParams_S* pstWakeupParams);

/**
@brief оƬ�����Ĺ��Ӻ�������
@param[in] bStandbyStatus 	TRUE:�������״̬;FALSE:�˳�����״̬
@param[in] nStandbyTime  ����ʱ��
@param[out] pbSkipDefault �Ƿ����Ĭ�ϲ�����TRUE��ʾִֻ��Hook��������Ĳ�����FALSE��ʾͬʱִ��CSUDISYSChipStandbyԭ�в�����Ĭ��ΪTRUE
@return CSUDI_SUCCESS ��ʾ���Ӻ���ִ�гɹ�������Ϊ���صĴ������ֵ��
*/
typedef CSUDI_Error_Code (*CSUDIChipStandbyHookFunc_F)(CSUDI_BOOL bStandby,int nStandbyTime, CSUDI_BOOL *pbSkipDefault);

/**
@brief ����оƬ�����Ĺ��Ӻ���

@param[in] fnHook ���Ӻ�������CSUDIChipStandbyHookFunc_F����
@return ������óɹ������ص���CSUDI_SUCCESS���������ʧ���򷵻ش������ֵ��
@note fnHook����ΪNULL����ʾȡ�����Ӻ���
*/
CSUDI_Error_Code CSUDISYSSetChipStandbyHook(CSUDIChipStandbyHookFunc_F fnHook); 

/**
@brief ����������Ϣ

���������������ö����������Ϣ��������CSUDIToolsetExecʱʹ����ͬ��Ŀ�������ɲ�ͬ�Ĺ�����
��loader��app����ʹ����ͬLinux�ں˵����������������ݱ��ӿ����õ������������ز�ͬ��rootfs��
@param[in] eType ������Ϣ
@param[in] pExecParam ��������������������CSUDIToolsetExec���о���������֪��ʼ��ģ�����ã������߱��뱣֤��ָ���ڴ�һֱ��Ч��
@return ���óɹ�����CSUDI_SUCCESS ������ʧ�ܷ��ش������
@see CSUDIToolsetExec
*/
CSUDI_Error_Code CSUDIToolsetSetExecInfo(CSUDIToolsetExecType_E eType,void* pExecParam);

/**
@brief ����Ŀ����������кź�Key����UDI�ӿ�CSUDIGetSerialKey��Ӧ

@param[in] psSerialKey ָ���Ż�ȡ�������кź�Key�Ľṹ��ָ�룬���кź�Key������0��β���ַ�������μ�CSUDISerialKey_S��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note ������coship�ڲ���CSUDIGetSerialKey�ӿ���������ʵ�֣�������������֪����λ�ȡ���кź�Key��������Ҫ��һ��set�ӿڣ���Ӧ���ڳ�ʼ��ʱ����
*/
CSUDI_Error_Code CSUDISetSerialKey(const CSUDISerialKey_S * psSerialKey);

#ifdef __cplusplus
}
#endif

/**@}*/

#endif  /* UDI2_TOOLSET_PLUS_H */

/* End of File */

