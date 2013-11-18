/**@defgroup COSHIPLIB COSHIPLIB �ṩӦ�ó�����coship�淶Bootloader�����Ľӿڡ�
@brief COSHIPLIBģ���Ӧͷ�ļ�<csbl_coshiplib.h>��������ϵͳ���ݶ�ȡ���֡����ݽ������ֵĽӿڡ�
�ֶԸ�ģ���������˵����

- ϵͳ���ݶ�ȡ�����ṩӦ�ó����ȡcoship�淶BootLoaderϵͳ���ݵ�������
- ���ݽ��������ṩӦ�ó�����coship�淶BootLoader�����ݽ�����������loadpt�ȡ�

@version 1.0.0 2011/3/11  
@{
*/
#ifndef _CSBL_COSHIPLIB_H_
#define _CSBL_COSHIPLIB_H_

#ifdef  __cplusplus
extern "C" {
#endif



/**@brief coship�淶��������*/
typedef enum
{
	EM_CSBL_EXTEND, ///< ��չ����,����coship�淶���ⲿ���ʽ�Ĵ�����������
	EM_CSBL_LOADER, ///< loader
	EM_CSBL_APP, ///< app
	EM_CSBL_DEBUGER, ///< loader bak
	EM_CSBL_FONT, ///< font
	EM_CSBL_PROGRAMPARAM, ///< programparam
	EM_CSBL_BOOTER, ///< booter
	EM_CSBL_SYSCONFIG, ///< sysconfig
	EM_CSBL_IFRAME, ///< iframe
	EM_CSBL_GUIRC, ///< guirc
	EM_CSBL_CA_OTP, ///< ca_otp
	EM_CSBL_SYSSETTING, ///< syssetting
	EM_CSBL_SYSINFO, ///< sysinfo
	EM_CSBL_EMAIL, ///< email
	EM_CSBL_SN, ///< sn
	EM_CSBL_CA_COSHIP, ///< ca_coship
	
	EM_CSBL_DATA_CNT
} CSBLDataType_E;

enum data_length__
{
	EM_CSBL_MANUFACTURE_DES_LEN = 7, 
	EM_CSBL_MACHINE_DES_LEN = 16, 
	EM_CSBL_VERSION_LEN = 12, 
	EM_CSBL_SN_LEN = 8, 
	EM_CSBL_STRUCTION_DES_LEN = 10, 
	EM_CSBL_MAX_SN_LENGTH = 0xF0
};


/**@brief LIB�����������Ͷ���*/
typedef enum
{
	EM_CSBL_LIBERROR_SUCCESS, ///< �ɹ�
	EM_CSBL_LIBERROR_FAILURE, ///< ʧ��
	EM_CSBL_LIBERROR_BAD_PARAMETER, ///< ��������
	EM_CSBL_LIBERROR_INVALID, ///< lib����
	EM_CSBL_SYSINFO_ADDR_INVALID, ///< sysinfo���ַ��Ч
	EM_CSBL_INITSYSINFO_FAILURE, ///< sysinfo��ʼ��ʧ��
	EM_CSBL_INITSYSCFG_FAILURE, ///< syscfg��ʼ��ʧ��
	
} CSBLCoshipLibErr_E;

/**@brief  ���app������ҳ���������Ͷ���(���ר��)*/
typedef enum
{
	EM_CSBL_TD_ENABLE = 0x0,        /* ���app����������������  */ 
	EM_CSBL_TD_APP_FORCE = 0x1,			/*���appǿ����������ǿ���������ֶ�����:��*/
	EM_CSBL_TD_APP_FORCE_NIT = 0x2,		/*���appǿ����������ǿ���������ֶ�����:��*/	
} CSBLTDOtaStatus_E;

/**@brief  �����淶app������ҳ���������Ͷ���*/
typedef enum
{
	EM_CSBL_JX_ENABLE = 0x0,        /* app����������������  */ 
	EM_CSBL_JX_APP_FORCE = 0x1,			/*appǿ����������ǿ���������ֶ�����:��*/
	EM_CSBL_JX_APP_FORCE_NIT = 0x2,		/*appǿ����������ǿ���������ֶ�����:��*/	
} CSBLJXOtaStatus_E;

/**@brief Coship�淶��ϵͳ��Ϣ*/
typedef struct
{
	unsigned char m_aManuDes[EM_CSBL_MANUFACTURE_DES_LEN];  ///< ����������
	unsigned char m_aMachineDes[EM_CSBL_MACHINE_DES_LEN];   ///< ����������
	unsigned char m_aHwVer[EM_CSBL_VERSION_LEN]; ///< Ӳ���汾��
	unsigned char m_aSwVer[EM_CSBL_VERSION_LEN]; ///< ��������������汾��
	unsigned char m_aSn[EM_CSBL_SN_LEN]; ///< ��Ϣ���е�SN ���к�
	unsigned char m_aSnMask[EM_CSBL_SN_LEN]; ///< ��Ϣ���е�SN MASK
	unsigned char m_aSnEx[EM_CSBL_MAX_SN_LENGTH];    ///< �����SN�������Ͷ�Ӧ��flash��ַ�е�����
	unsigned int m_dwManuCode; ///< ���̴���
} CSBLCoshipDataInfo_S;

/**@brief Coship�淶OTA��info������Ϣ*/
typedef struct
{
	unsigned short m_wPId; ///< PID
	unsigned char m_ucTableId; ///< TableID
	unsigned char m_ucTunerId; ///< Tuner����
	unsigned char m_aReserve[32]; ///< ����
} CSBLCoshipOtaInfo_S;


/**@brief Coship�淶Usb������Data������Ϣ*/
typedef struct
{
	char m_acFilePathName[256]; ///< �����ļ����ص��·�����ļ���
	unsigned char m_aReserve[64]; ///< ����
} CSBLCoshipUsbData_S;


/**@brief Coship�淶Ip������Ϣ��Ӧ�ô���loader���ļ��洢·��*/
typedef struct
{
	char		m_acUrl[256];	//�����ļ�ȫ·��
	BYTE 	m_aReserve[64];
} CSBLCoshipIpData_S;


typedef enum
{
	EM_CSBL_GETIP_BY_STATIC= 0,
	EM_CSBL_GETIP_BY_DHCP,
	EM_CSBL_GETIP_BY_PPPOE
}CSBLGetIpType_E;


/**@brief Coship�淶Ip������Ӧ�ô���loader�ľ�̬ip����Ϣ*/
typedef struct
{
	CSBLGetIpType_E m_acGetIpType;
	char m_acMacAddr[13];
	char m_acReserve[3];
	
	union {
		
		struct {
			char m_acIPAddr[16];
			char m_acDNS[16];
			char m_acDNSBak[16];
			char m_acNetMask[16];
			char m_acGateWay[16];
		}staticIpInfo;

		struct {
			char m_aAcount[40];
			char m_aPassword[40];
		}ppoeInfo;

		struct {
			char m_acReserve[80];
		}dhcpInfo;
		
	}IpTypeInfo;
} CSBLCoshipIpInfo_S;


#define BUFFSIZE (160)

typedef struct
{
	unsigned char  m_aManuDes[BUFFSIZE * 2];  ///< ����������
	unsigned char  m_aMachineDes[BUFFSIZE * 2];   ///< ����������
	unsigned char  m_aHwVer[BUFFSIZE * 2]; ///< Ӳ���汾��
	unsigned char  m_aSwVer[BUFFSIZE * 2]; ///< ��������������汾��
	unsigned char  m_aSn[BUFFSIZE * 2]; ///< ���кŷ�Χ
	unsigned char  m_aIp[BUFFSIZE * 2]; ///< IP��Χ
	unsigned char  m_aRegion[BUFFSIZE * 2];    ///<�����뷶Χ
	unsigned char  m_aMac[BUFFSIZE * 2];    ///<Mac��Χ
	unsigned char  m_aManuCode[BUFFSIZE * 2]; ///< ���̴���	
	unsigned char  m_aFilePath[BUFFSIZE * 2];//�����ļ�·��
}CSBLXmlCfgInfo_S;


typedef struct
{
	char Name[BUFFSIZE];
	unsigned char Value[BUFFSIZE * 2];
}Item_S;

typedef struct
{

	DWORD dwItemCnt;
	Item_S XmlItem[];
	
}CSBLXmlItemInfo_S;



/**(����������Ŀ֧��)
@briefģ��eeprom��Ҫ������ģ���flash��Χ
@note:Ŀǰֻ֧����nor flash,  spi flash����ģ��eeprom
@note:��֧��nand flash
@param[in] dwFlashAddr ��Ҫģ���flash�ռ���߼���ַ��
@param[in] dwFlashSize ��Ҫģ���flash��Χ����(��λ��BYTE)��
@param[in] dwEEPROMSize ģ��eeprom�Ĵ�С(��λ��BYTE)��
@return CSUDI_SUCCESS��ʾ�ɹ�
*/
CSUDI_Error_Code CSBLVirEEPRomInit(DWORD dwFlashAddr, DWORD dwFlashSize, DWORD dwEEPROMSize);



/**
@brief ϵͳ���ݳ�ʼ��

@return ����CSBLCoshipSysErr_E�ж���Ĵ���
@note 
- ���ñ��ӿ�ǰ�����ȵ��ýӿ�
CSBLSetConfig(EM_CSBL_CONFIG_SYSINFO, xxxxxx, NULL);����infoλ��
- ֻ�б��ӿڵ��ò��ɹ���,�������ܽӿڲſ�������ʹ��
*/
CSBLCoshipLibErr_E CSBLCoshipSysInit(void);


/**
@brief ��ȡ�Ӱ汾��

@param[in] eDataType �������͡����嶨���CSBLDataType_Eö�١�
@param[out] pdwVer �ɹ����ض�Ӧ�������͵��Ӱ汾��
@return ����CSBLCoshipLibErr_E�ж���Ĵ���
*/
CSBLCoshipLibErr_E CSBLGetSubVersion(CSBLDataType_E eDataType, PDWORD pdwVer);


/**
@brief ��ȡSTBϵͳ��Ϣ

@param[out] psDataInfo �ɹ�����STBϵͳ��Ϣ��
@return ����CSBLCoshipLibErr_E�ж���Ĵ���
*/
CSBLCoshipLibErr_E CSBLGetCoshipDataInfo(CSBLCoshipDataInfo_S *psDataInfo);



/**
@brief ����OTA������Ϣ

@param[in] pOTAPara Ƶ�㲿�ֵ���Ϣ����udi2_tuner.h��CSUDITunerSRCDeliver_U �Ķ��塣
@param[in] pParaEx ��չ���ֵ���Ϣ��Ŀǰû��ʹ��,���Դ�NULL��
@param[in] pOTAInfo info������Ϣ����CSBLCoshipOtaInfo_S���塣
@return ����CSBLCoshipLibErr_E�ж���Ĵ���
*/
CSBLCoshipLibErr_E CSBLSetOTAUpgrade(unsigned char * pOTAPara, unsigned char * pParaEx, unsigned char * pOTAInfo);


/**
@brief ����OTA������Ϣ---------������ ���淶,

@param[in] pOTAPara Ƶ�㲿�ֵ���Ϣ����udi2_tuner.h��CSUDITunerSRCDeliver_U �Ķ��塣
@param[in] pParaEx ��չ���ֵ���Ϣ��Ŀǰû��ʹ��,���Դ�NULL��
@param[in] pOTAInfo info������Ϣ����CSBLCoshipOtaInfo_S���塣
@param[in] eTDOtaStatus �������ͣ���CSBLTDOtaStatus_E���塣
@return ����CSBLCoshipLibErr_E�ж���Ĵ���
*/
CSBLCoshipLibErr_E CSBLTDSetOTAUpgrade(unsigned char * pOTAPara, unsigned char * pParaEx, unsigned char * pOTAInfo,CSBLTDOtaStatus_E eTDOtaStatus);


/**
@brief ����OTA������Ϣ	----�����ڽ����淶

@param[in] pOTAPara Ƶ�㲿�ֵ���Ϣ����udi2_tuner.h��CSUDITunerSRCDeliver_U �Ķ��塣
@param[in] pParaEx ��չ���ֵ���Ϣ��Ŀǰû��ʹ��,���Դ�NULL��
@param[in] pOTAInfo info������Ϣ����CSBLCoshipOtaInfo_S���塣
@param[in] eTDOtaStatus �������ͣ���CSBLTDOtaStatus_E���塣
@return ����CSBLCoshipLibErr_E�ж���Ĵ���
*/
CSBLCoshipLibErr_E CSBLJXSetOTAUpgrade(unsigned char * pOTAPara, unsigned char * pParaEx, unsigned char * pOTAInfo,CSBLJXOtaStatus_E eJXOtaStatus);


/**
@brief ����Usb������Ϣ

@param[in] pUsbPara usb����Data���ֵ���Ϣ����CSBLCoshipUsbData_S �Ķ��塣
@param[in] pParaEx ��չ���ֵ���Ϣ��Ŀǰû��ʹ��,���Դ�NULL��
@param[in] pUsbInfo info������Ϣ��Ŀǰû��ʹ��,���Դ�NULL��
@return ����CSBLCoshipLibErr_E�ж���Ĵ���
*/
CSBLCoshipLibErr_E CSBLSetUsbUpgrade(unsigned char * pUsbPara, unsigned char * pParaEx, unsigned char * pUsbInfo);


/**
@brief ����V1 IPLOADER������Ϣ( Ŀǰֻ�н���ʹ��)

@param[in] pstIPUpgradePara �����������ͣ���������汾���Լ������ļ�·��,�ṹ�μ�struct DL_IP_STRUCT
@param[in] pIPconf �洢IP��ַ��MAC��ַ���Լ�������Ϣ�μ�struct IP_Config
@param[in] nIPStrCount �����ļ�����
@return ����CSBLCoshipLibErr_E�ж���Ĵ���
*/
CSBLCoshipLibErr_E CSBLSetIpLoaderV1Upgrade(const char* pstIPUpgradePara, const char* pIPconf, int nIPStrCount);


/**
@brief ����HTTP ������Ϣ

@param[in] pHttpPara Xml�ļ����ڵ�·������CSBLCoshipIpData_S �Ķ��塣
@param[in] pParaEx ��չ���ֵ���Ϣ��Ŀǰû��ʹ��,���Դ�NULL��
@param[in] pHttpInfo info������Ϣ����CSBLCoshipIpInfo_S���塣
@return ����CSBLCoshipLibErr_E�ж���Ĵ���
*/
CSBLCoshipLibErr_E CSBLSetHTTPUpgrade(unsigned char * pHttpPara, unsigned char * pParaEx, unsigned char * pHttpInfo);


/**
@brief ��������ȡXML�ļ��е���Ϣ(HTTP����)

@param[in] pXmlBuf ����XML�ļ���buf
@param[in] dwXmlBufLenΪpXmlBuf�ĳ���
@param[in] psDataInfo�����洢��XML�л�ȡ����Ϣ������ɼ���ṹ��
@param[in] psItemInfo��δʹ�ã���NULL���ɡ�
@return ����CSBLCoshipLibErr_E�ж���Ĵ���
*/
CSBLCoshipLibErr_E CSBLGetXmlCfgInfo(char *pXmlBuf, DWORD dwXmlBufLen, CSBLXmlCfgInfo_S *psDataInfo, CSBLXmlItemInfo_S *psItemInfo);

#ifdef  __cplusplus
}
#endif

#endif



