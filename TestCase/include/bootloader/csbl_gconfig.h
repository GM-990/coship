/**@defgroup CONFIG CONFIG �ṩBootLoader�Ĺ������ýӿ�
@brief CONFIGģ���Ӧͷ�ļ�<csbl_gconfig.h>��������BootLoader�й���������ص����ýӿڡ�

@version 1.0.0 2011/3/10  
@version 1.0.1 2011/3/31   ��Ӧbl_ver:2.1.6.0
@version 1.0.2 2011/4/11   ��Ӧbl_ver:2.1.6.1
@version 1.0.3 2011/4/11   ��Ӧbl_ver:2.1.6.2
@version 1.0.4 2011/4/21   ��Ӧbl_ver:2.2.0.0
@version 1.0.5 2011/4/29   ��Ӧbl_ver:02.02.01.00

@{
*/
#ifndef _CSBL_GCONFIG_H_
#define _CSBL_GCONFIG_H_


#ifdef  __cplusplus
extern "C" {
#endif

//#include "udiplus_typedef.h"

/**@brief ������������*/
typedef enum
{
	EM_CSBL_UI_LANGUAGE_NONE,
	EM_CSBL_UI_LANGUAGE_CHS, ///< ���ļ���
	EM_CSBL_UI_LANGUAGE_ENG, ///< Ӣ��
	EM_CSBL_UI_LANGUAGE_GER, ///< ����
	EM_CSBL_UI_LANGUAGE_POR, ///< ��������
	EM_CSBL_UI_LANGUAGE_SPA, ///<��������
	
	EM_CSBL_UI_LANGUAGE_CNT
} CSBLUILanguageType_E;

/**@brief boot������������*/
typedef enum
{
	EM_CSBL_UI_PROMPT_COSHIP,	//coship boot��������
	EM_CSBL_UI_PROMPT_UNITEND,   //Unitend boot��������
	EM_CSBL_UI_PROMPT_NDS,		//NDS boot��������
	EM_CSBL_NO_UI_PROMPT,		//û����������(��������ʾͼƬ����Ƶʱ�����ô���)

	EM_CSBL_UI_PROMPT_CNT
}CSBLUIPromptType_E;

/**@brief booter��ѹʱУ�������*/
typedef enum
{
	EM_CSBL_DECCHECK_NONE, ///< ��У��
	EM_CSBL_DECCHECK_OCRC = 0x01, ///< ��ѹ���ԭʼ�ļ�У��
	EM_CSBL_DECCHECK_CCRC = 0x10, ///< ��ѹǰ��ѹ���ļ�У��У��
	EM_CSBL_DECCHECK_BOTH = 0x11 ///< ��ѹǰ��ѹ��У��
} CSBLDecompressCrcType_E;


/**@brief ˫loaderУ�鷽ʽ*/
typedef enum
{
	EM_CSBL_LOADRECOV_NONE, ///< ��У��
	EM_CSBL_LOADRECOV_EVER, ///< ÿ����������˫loaderУ�鲢�ָ�
	EM_CSBL_LOADRECOV_LOAD, ///< ֻ������loaderʱ��˫loaderУ�鲢�ָ�

	EM_CSBL_LOADRECOV_CNT
} CSBLLoadRecoverType_E;

/**@brief FLASH����*/
typedef enum
{
	EM_CSBL_FLSTYPE_NONE,
	EM_CSBL_FLSTYPE_NOR_SPI,
	EM_CSBL_FLSTYPE_NOR_CFI,
	EM_CSBL_FLSTYPE_NAND,
	EM_CSBL_FLSTYPE_CNT
} CSBLFlashType_E;


/**@brief �洢���ݵ��豸����*/
typedef enum
{	
	EM_CSBL_STORAGE_RAM = -1, ///< ram
	EM_CSBL_STORAGE_FLASH = 0, ///< flash
	EM_CSBL_STORAGE_E2PROM = 1, ///< ģ��eeprom
	EM_CSBL_STORAGE_MAX = 2, ///����,��ʷԭ�����ⲿ�ܶ๤��д����ֵΪ2�Ǵ�����������͡�
	EM_CSBL_STORAGE_REAL_E2PROM = 3  ///��ʵeeprom
} CSBLStorageType_E;

/**@brief �����ڴ洢�豸�е�λ����Ϣ*/
typedef struct 
{
	unsigned int dwAddr; ///< �豸�е�ƫ�Ƶ�ַ
	unsigned int dwSize; ///< ��С
	unsigned int dwStoreType; ///< �洢�豸����CSBLStorageType_E����
} CSBLDataPos_S;

#define LOADEER_VERSION_MAX_LENGTH (40)  ///< loader����汾����󳤶�
#define M_CSBL_MAX_TUNER_CNT (4)  ///< ��֧�ֵ�����tuner����

/**@brief У����ܵķ�ʽ*/
typedef enum
{
	EM_CSBL_CHECK_DECRYPT_TYPE_NONE, ///< none
	EM_CSBL_CHECK_DECRYPT_TYPE_UNKNOWN, ///< δ֪
	EM_CSBL_CHECK_DECRYPT_TYPE_CUSTOM, ///< �ⲿ����
	EM_CSBL_CHECK_DECRYPT_TYPE_COSHIP, ///< coship�淶Ҫ��
	EM_CSBL_CHECK_DECRYPT_TYPE_CONAX, ///< conax�淶Ҫ��
	EM_CSBL_CHECK_DECRYPT_TYPE_VERIMATRIX, ///< verimatrix�淶Ҫ��
	EM_CSBL_CHECK_DECRYPT_TYPE_VIACCESS, ///< viaccess�淶Ҫ��
	EM_CSBL_CHECK_DECRYPT_TYPE_NAGRA, ///< nagra�淶Ҫ��
	
	EM_CSBL_CHECK_DECRYPT_TYPE_CNT
} CSBLCheckDecryptType_E;

/**@brief У����ܵ������Ϣ*/
typedef struct
{
	unsigned int m_dwHeadLength; ///< У���������ͷ�ĳ���
	unsigned int m_dwSignatureLength; ///< ǩ�ֲ��ֵĳ���
	unsigned int m_dwDataLength; ///< ���ݲ��ֵ��ܳ��ȣ�head��signature�Լ�Data������֮��Ϊ�ܵ��ļ�����
	unsigned int m_dwSignDataLength; ///< Data��ǩ�ֲ��ֳ���
	unsigned int m_dwEncryptDataLength; ///< Data�м��ܲ��ֳ���
	CSBLCheckDecryptType_E m_eCheckDecryptType; ///< У����ܵķ�ʽ����CSBLCheckDecryptType_E����	
} CSBLCheckDecryptInfo_S;


/**@brief ����CA�淶*/
typedef enum
{
	EM_CSBL_CA_NONE,////
	EM_CSBL_CA_TF,    ////tfͬ���淶 
	EM_CSBL_CA_IRDETO, ////irdeto�淶 
	EM_CSBL_CA_NDS,   ////nds�淶  
	EM_CSBL_CA_CONAX, ////conax �淶
	EM_CSBL_CA_VERIMATRIX, ////verimatrix �淶	
	EM_CSBL_CA_NAGRA, ////nagra �淶
	EM_CSBL_CA_TIANJIN, ////�����淶
	EM_CSBL_CA_HENAN,  ////����ʡ�淶
	EM_CSBL_CA_UNITEND,	/////����̫��
	EM_CSBL_CA_GDSW,	////�㶫ʡ��(�Ϸ���ý�淶)
	EM_CSBL_CA_TIANDA,
	EM_CSBL_CA_JIANGXI,
	EM_CSBL_CA_LATENS,////Latens�淶
	EM_CSBL_CA_DEZHOU, ///����N8001C��Ŀ��˫CA
	
	EM_CSBL_CA_CNT
} CSBLCAType_E;

/**@brief ����оƬ����*/
typedef enum
{
	EM_CSBL_CHIP_DEFAULT,	
	EM_CSBL_CHIP_ALI3603,
	EM_CSBL_CHIP_ALI3281,
	EM_CSBL_CHIP_ALI3701,
	EM_CSBL_CHIP_CNT
} CSBLCHIPType_E;

/**@brief ��������̫����ص�ַ*/
typedef struct 
{
	unsigned int dwUnitendConAddr; ///����̫��config���ַ
	unsigned int dwUnitendConBakAddr; ///����̫��config��ı��ݵ�ַ
	unsigned int dwUnitendSigAddr; ///�������̫��ǩ�ֵĵ�ַ
	unsigned int dwUnitendCertAddr; ///�������̫��֤��ĵ�ַ
} CSBLUnitendConf_S;


/**@brief ��BootLoader�����õĻص���������

@param[in] Param1 ����һ������CSBLSetConfig�����õ��������Ͳ�ͬ����ͬ����CSBLConfig_E�Ķ�����˵��
@param[in] Param2 ������������CSBLSetConfig�����õ��������Ͳ�ͬ����ͬ����CSBLConfig_E�Ķ�����˵��
@return �ɹ�����SUCCESS��ʧ�ܷ���FAILURE
*/
typedef int (*CSBLCustomCallBack_F)(PVOID Param1, PVOID Param2);



/**@brief ��BootLoader�����õĻص���������

@return �ɹ�����SUCCESS��ʧ�ܷ���FAILURE
*/
typedef int (*CSBLCustomCallBack_F_P4)(PVOID Param1, PVOID Param2,PVOID Param3, PVOID Param4);



/**@brief CSBLSetConfig�ӿڿ������õ�����*/
typedef enum
{
	EM_CSBL_CONFIG_NONE = 0,
	EM_CSBL_CONFIG_UI_XOFFSET, ///< BootLoader����X�᷽��ƫ�ƣ�Param1Ϊƫ��ֵ��Param2��Ч
	EM_CSBL_CONFIG_UI_YOFFSET, ///< BootLoader����Y�᷽��ƫ�ƣ�Param1Ϊƫ��ֵ��Param2��Ч
	EM_CSBL_CONFIG_UI_LANGUAGE, ///< BootLoader��������ѡ��Param1Ϊ���֣���CSBLUILanguageType_E���壬Param2��Ч
	EM_CSBL_CONFIG_FPDISPLAY_NEEDSCAN, ///< Booter��ǰ�����ʾʱ�Ƿ���Ҫscan��Param1��BOOL���壬Param2��Ч
	EM_CSBL_CONFIG_DEC_FPDISPLAY, ///< Booter��ѹʱǰ����Ƿ���ʾ���ȣ�Param1��BOOL���壬Param2��Ч
	EM_CSBL_CONFIG_DEC_CRCCHECK, ///< Booter��ѹǰ��crc check����ѡ��Param1��CSBLDecompressCrcType_E���壬Param2��Ч
	EM_CSBL_CONFIG_LOADRECOVER, ///< �������ʱ˫loader���ݻ��Ƶ�ѡ��Param1��CSBLLoadRecoverType_E��Param2��Ч
	EM_CSBL_CONFIG_SYSINFO, ///< Coship�淶����sysinfo���ַ��Param1Ϊsysinfo���ַ��Param2��Ч
	EM_CSBL_CONFIG_IRDETO_BOOTINFO, ///< Irdeto�淶����Booter��Ϣ��Param1ΪCSBLDataPos_Sָ�룬Param2��Ч
	EM_CSBL_CONFIG_IRDETO_LOADINFO, ///< Irdeto�淶����Loader��Ϣ��Param1ΪCSBLDataPos_Sָ�룬Param2��Ч
	EM_CSBL_CONFIG_IRDETO_BBCBINFO, ///< Irdeto�淶����BBCB��Ϣ��Param1ΪCSBLDataPos_Sָ�룬Param2��Ч
	EM_CSBL_CONFIG_IRDETO_KEYINFO, ///< Irdeto�淶����Key��Ϣ��Param1ΪCSBLDataPos_Sָ�룬Param2��Ч
	EM_CSBL_CONFIG_IRDETO_LOADPTINFO, ///< Irdeto�淶����loadpt��Ϣ��Param1ΪCSBLDataPos_Sָ�룬Param2��Ч
	EM_CSBL_CONFIG_IRDETO_RESETINFO, ///< Irdeto�淶����reset��Ϣ��Param1ΪCSBLDataPos_Sָ�룬Param2��Ч
	EM_CSBL_CONFIG_IRDETO_CPCBINFO, ///< Irdeto�淶����CPCB��Ϣ��Param1ΪCSBLDataPos_Sָ�룬Param2��Ч
	EM_CSBL_CONFIG_IRDETO_APPINFO, ///< Irdeto�淶����App��Ϣ��Param1ΪCSBLDataPos_Sָ�룬Param2��Ч
	EM_CSBL_CONFIG_IRDETO_1STPARTITIONINFO, ///< Irdeto�淶����First partition��Ϣ��Param1ΪCSBLDataPos_Sָ�룬Param2��Ч
	EM_CSBL_CONFIG_IRDETO_PARTITIONINFO, ///< Irdeto�淶����Partition��Ϣ��Param1ΪCSBLDataPos_Sָ�룬Param2��Ч
	EM_CSBL_CONFIG_IRDETO_BOOTUARTUPG, ///< Irdeto�淶Booter���Ƿ��д����������ܣ�Param1��BOOL���壬Param2��Ч
	EM_CSBL_CONFIG_WRITEFS_NEEDERASE, ///< дFlash�ĵ��������ʱ�Ƿ���Ҫ�Ȳ�����Param1��BOOL���壬Param2��Ч
	EM_CSBL_CONFIG_VERSIONSTRING, ///< �ⲿ���ð汾�ţ�Param1Ϊcharָ�룬Param2Ϊcharָ��
	EM_CSBL_CONFIG_CHECKDECRYPT_BOOT, ///ע���ѹ����,booter����app��ʱ��ʹ��,CONAX(RSAУ��app,AES����app),NAGRAУ����������
	EM_CSBL_CONFIG_CHECKDECRYPT_LOAD, ///< ��������ʱʹ�õĽ�ǩ�ֺͽ��ܷ�ʽ��Param1Ϊ�ⲿ���õ�У����ܵĺ�������CSBLCustomCallBack_F���塣
	EM_CSBL_CONFIG_COSHIPLOAD_CUSTOMWRITEFIRST, ///< Coship loader���������ݲ����У��󣬽�ĳĳһ�������͵�����ͨ���ⲿ�ص���������Param1Ϊ�������ͣ���CSBLDataType_E���壬Param2Ϊ�ص���������CSBLCustomCallBack_F���塣�ص�������Param1Ϊ����buf��Param2Ϊ���ݳ���
	EM_CSBL_CONFIG_LOAD_VERIFY_SWVER, ///< ����Coship loader�������ع����У��Ƿ��������汾��У�顣Param1��BOOL���壬Param2��Ч
	EM_CSBL_CONFIG_UPGRADE_QAM, ///< �����Ƿ�����Qam������ʽ��Param1Ϊ�ⲿ���ú��������ڴӽ����ȡQam������������CSBLCustomCallBack_F���塣�ص�������Param1��Param2�ڵ���ʱ������NULL��Param2Ϊ�ⲿ���õĺ��������ڽ���Qam��������CSBLCustomCallBack_F���壬������Param1��Param2�ڵ���ʱ������NULL��
	EM_CSBL_CONFIG_UPGRADE_COFDM, ///< �����Ƿ�����Qam������ʽ��Param1Ϊ�ⲿ���ú��������ڴӽ����ȡCofdm������������CSBLCustomCallBack_F���塣�ص�������Param1��Param2�ڵ���ʱ������NULL��Param2Ϊ�ⲿ���õĺ��������ڽ���Cofdm��������CSBLCustomCallBack_F���壬������Param1��Param2�ڵ���ʱ������NULL��
	EM_CSBL_CONFIG_UPGRADE_QPSK, ///< �����Ƿ�����Qam������ʽ��Param1Ϊ�ⲿ���ú��������ڴӽ����ȡQpsk������������CSBLCustomCallBack_F���塣�ص�������Param1��Param2�ڵ���ʱ������NULL��Param2Ϊ�ⲿ���õĺ��������ڽ���Qpsk��������CSBLCustomCallBack_F���壬������Param1��Param2�ڵ���ʱ������NULL��
	EM_CSBL_CONFIG_UPGRADE_USB, ///< �����Ƿ�����usb������ʽ��Param1Ϊ�ⲿ���ú��������ڴӽ����ȡusb������������CSBLCustomCallBack_F���壬ΪNULL���޸ù��ܡ��ص�������Param1��Param2�ڵ���ʱ������NULL��Param2Ϊ�ⲿ���õĺ��������ڽ���USB��������CSBLCustomCallBack_F���壬������Param1��Param2�ڵ���ʱ������NULL��
	EM_CSBL_CONFIG_UPGRADE_IP_OLD, ///< �����Ƿ����Ӿɰ�ip������ʽ��Param1Ϊ�ⲿ���ú��������ڴӽ����ȡip������������CSBLCustomCallBack_F���壬ΪNULL���޸ù��ܡ��ص�������Param1��Param2�ڵ���ʱ������NULL��Param2Ϊ�ⲿ���õĺ��������ڽ���ip��������CSBLCustomCallBack_F���壬������Param1��Param2�ڵ���ʱ������NULL��
	EM_CSBL_CONFIG_UPGRADE_IP, ///< �����Ƿ�����ip������ʽ��Param1Ϊ�ⲿ���ú��������ڴӽ����ȡip������������CSBLCustomCallBack_F���壬ΪNULL���޸ù��ܡ��ص�������Param1��Param2�ڵ���ʱ������NULL��Param2Ϊ�ⲿ���õĺ��������ڽ���ip��������CSBLCustomCallBack_F���壬������Param1��Param2�ڵ���ʱ������NULL��
	EM_CSBL_CONFIG_CA_TYPE, ////����CA�淶��Param1ΪCSBLCAType_E����, Param2ע��OTASTART�������ο�Henan_OTA_Start
	EM_CSBL_CONFIG_OSG_NEEDBLIT, ///< OSG��ʾʱ�Ƿ���Ҫblit��Param1��BOOL���壬Param2��Ч
	EM_CSBL_CONFIG_CACHE_PROCESS,///<�����Ƿ���Ҫ����CACHE����ĿǰCACHE����ֻ��FLASH��дʱ��ʹ�ã�����PARAM1ΪCSBLCustomCallBack_F����Ϊ��CACHE������PARAM2ҲΪCSBLCustomCallBack_F����Ϊ��CACHE����
	EM_CSBL_CONFIG_IRDETO_SYSSETTINGINFO,///Irdeto�淶����syssetting��Ϣ��Param1ΪCSBLDataPos_Sָ�룬Param2��Ч
	EM_CSBL_CONFIG_SHOW_PROGRESSBAR,////����Booter����ʱ�Ƿ���ʾ��������Param1ΪTRUEʱ��ʾ��FALSE����ʾ��Param2��Ч
	EM_CSBL_CONFIG_SET_MASK, //�������룬Param1Ϊ���õ����룬param2��Ч.��û�����ã�Ĭ������Ϊ0xffffffff(λ��ǰ�󣬲�����Ӱ��),������Ҫ�����á�Ŀǰ�ڰ����ص�ʱʹ��(blgui.c:InputCallBack).
	EM_CSBL_CONFIG_IRDETO_CFG_APPINFO, ///< Irdeto�淶����cfg���� App��Ϣ��Param1ΪCSBLDataPos_Sָ�룬Param2��Ч
	EM_CSBL_CONFIG_UDI2_SECTION_REQUEST, ///<����OTA�������ݵ�ʱ�򣬹���section���ݰ�ʱ��ֱ��ʹ��UDI2_section.h�ӿ�, ��ʹ��DTV�⣬Param1Ϊ��Ч��Param2��Ч
	EM_CSBL_CONFIG_SYSDATA_CHECK,//����У��sysdata��sysdatabak; PARAM1Ϊsysdata�ĵ�ַ,PARAM2Ϊsysdatabak�ĵ�ַ(�߼���ַ);��δ����,��������һ����ַΪ0,����У��.
	EM_CSBL_CONFIG_INFLATE_FUN,//���ý�ѹ������ʹ���µĽ�ѹ����,ͨ���ⲿע������ĺ��� PARAM1Ϊע��ĺ�����ַ.��CSBLCustomCallBack_F���塣  PARAM2��Ч
	EM_CSBL_CONFIG_BOOT_IR_FORCE_UPGRADE,////�����Ƿ�ע��ң����ǿ�ƽ���loader������Param1Ϊң������⺯����Param2ΪNULL��	
	EM_CSBL_CONFIG_UNITEND_CONFIG,//��������̫����صĵ�ַ��Ϣ��Param1ΪCSBLUnitendConf_Sָ��,Param2��Ч
	EM_CSBL_CONFIG_UI_PROMPT_TYPE,//����booter�������棬Param1ΪҪ���õ������������ͣ�Param2ΪNULL����δ���ã�Ĭ��ʹ��Coship booter�������档
	EM_CSBL_CONFIG_FRONTPANEL_INFO,//���û������Ƿ���ǰ��壬Param1��ΪTRUE��������ǰ��壬FALSE��û��ǰ��壻Param2ΪNULL
	EM_CSBL_CONFIG_UPDATE_BLPARAM_HMAC,//���û��������ñ�(sysinfo, sysconfig, loaderpt�ȸ��º�)��Hmac �Ƿ�����д�룬Param1 Ϊ���µ����õĵ�ַ��Param2 ΪNULL
	EM_CSBL_CONFIG_FLS_TYPE,//��������FLASH���ͣ�Ŀǰ���ڿ����ǰ�BLOCK������������Ƭ������Param1����ΪCSBLSetConfigErr_E��Param2ΪNULL
	EM_CSBL_CONFIG_DVB_T2_INFO,//���õ����tuner����ΪT2������������
	EM_CSBL_CONFIG_AES_FUNC,//����AES�㷨��Param1��ΪAES��ע�ắ����Ĭ��ΪNULL��Param2ΪNULL
	EM_CSBL_CONFIG_SET_AES_RSA_KEY,//���û�ȡAES��RSA�㷨ʹ�õ�key��mod��Param1Ϊ��ȡAES key�Ľӿڣ�Param2Ϊ��ȡRSA�㷨ʹ�õ�key��mod�Ľӿ�
	EM_CSBL_CONFIG_ZSL_WRITE_FUNC, //����ͬ�޹淶OTA/USBдflash���ݺ�����Param1Ϊ��ע�ắ����Param2ΪNULL
	EM_CSBL_CONFIG_OTA_LED_FLICKER_FUNC, //����ͬ�޹淶OTA����,д����ʱ��ǰ����źŵ���˸�ĺ�����Param1Ϊ��ע�ắ����Param2ΪNULL
	EM_CSBL_CONFIG_IRDETO_MAC_FUNC, //����irdeto�淶,оƬƽ̨����MAC�ĺ�����Param1Ϊ��ע�ắ����Param2ΪNULL
	EM_CSBL_CONFIG_MSTAR_USB_FUNC,//����Mstarƽ̨��USB����ʱ��������U�̵ĳ�ʼ������,Param1Ϊ��ע�ắ��,Param2��ΪNULL
	EM_CSBL_CONFIG_GET_RSA_KEY, //����conax�淶��ȡRSAKEY��ʹ�õĻص�������Param1Ϊ��ע�ắ��,Param2��ΪNULL
	EM_CSBL_CONFIG_HISIOTA_WAIT_FUNC,//HeFei��Ŀ��Ҫ����Ƶ�ɹ����������ݵĵȴ�ʱ���5�����޸�Ϊ2����,ͨ�����ⲿע�᷽ʽ����ʵ��
	EM_CSBL_CONFIG_CHIP_TYPE,//���ö�BL ������Ҫ���оƬ������,Param1ΪоƬ���ͣ��������ͼ�CSBLCHIPType_E
	EM_CSBL_CONFIG_USBUPGRADE_CMPSW,//�����������usb�����Ƿ���Ҫ�Ƚ�����汾��
	EM_CSBL_CONFIG_CNT
} CSBLConfig_E;

/**@brief CSBLSetConfig�ӿڲ����������*/
typedef enum
{
	EM_CSBL_SETCONFIG_SUCCESS, ///< �ɹ�
	EM_CSBL_SETCONFIG_FAILURE, ///< ʧ��
	EM_CSBL_SETCONFIG_BADPARAM ///< ��������
	
} CSBLSetConfigErr_E;

/**
@brief ����BootLoader����

@param[in] eCfg �������ͣ���CSBLConfig_E����
@param[in] Param1 ����һ�������������eCfg��ͬ���仯���ο�CSBLConfig_E�����и���˵��
@param[in] Param2 �������������������eCfg��ͬ���仯���ο�CSBLConfig_E�����и���˵��
@return ����CSBLSetConfigErr_E�ж���Ĵ���
*/
CSBLSetConfigErr_E CSBLSetConfig(CSBLConfig_E eCfg, PVOID Param1, PVOID Param2);


#ifdef  __cplusplus
}
#endif

#endif



