/******************************************************************

@defgroup  ����CA�淶,   �ṩӦ�ó�����coship�淶Bootloader�����Ľӿڡ�
@brief ��ģ���Ӧͷ�ļ�<csbl_syssetting.h>�����������ݽ������ֵĽӿڡ�
�ֶԸ�ģ���������˵����


(1)  	PAL/ NTSC��ʽ����ʽͬ��, app�޸ĺ���ʽ�󣬴洢��flash��eeprom�е�syssetting��������������bootloader���޸ĳ�appһ������ʽ
(2)	tuner����ͬ���ȵ�,����Ŀǰ��Щ��Ŀbootloader��ͬʱ֧��AnnxB, AnnxA����Ҫ��syssetting�������ò���������loader��ʼ��tuner��ʱ�򣬻�ѡ���Ӧ�Ĳ�����ʼ��tuner.
(3)   Ŀǰ(2011.07.10) booter, loader��syssetting������ֻ��ȡ��  ��д���ԭ��.
(4)   app�Դ�����ɶ�����д��
(5)   ��¼�����г����ʱ�����û��syssetting,����Ҫapp������д����Ŀ��ȱʡֵ


@version    1.0.0   bootloader     2006/3/22
@version    2.0.0   bootloader     2011/3/22
@version    3.0.0   bootloader     2011/7/10
***********************************************************************/

#ifndef _CSBL_SYSSETTING_H_
#define _CSBL_SYSSETTING_H_

#include "udiplus_debug.h"
#include "csbl_gconfig.h"
#include "udi2_screen.h"

#ifdef	__cplusplus
extern "C" {
#endif


#define M_SYS_SETTING_TAG    "BLSYSSETTING"
#define M_SYS_SETTING_TAG_LENGTH   (16)


///SYSSETTING�ṹ����󳤶�,��Ҫ����0xf0�ֽ�  2012.01.20,
///��Ϊsyssetting�п��ܴ洢��ģ��eeprom, ��ģ��eeprom���Ƚ�С
#define M_SYS_SETTING_LENGTH       (0xF0)


/**@brief LIB�����������Ͷ���*/
typedef enum
{
	EM_CSBL_SYSSETTING_SUCCESS=0, ///< �ɹ�
	EM_CSBL_SYSSETTING_FAILURE =1 , ///< ʧ��,δ֪��ʧ��
	EM_CSBL_SYSSETTING_FLASH_ERROR_R=2, ///< ��flash����
	EM_CSBL_SYSSETTING_FLASH_ERROR_W=3, ///< дflash����	
	EM_CSBL_SYSSETTING_ADDR=4, ///δ����syssetting��ַ,ͬ���淶������sysconfig.flash���ñ�����syssetting��ַ,   irdeto�淶������δ����BLSetSysSettingSize(),�����Ǵ洢���ʹ���
	EM_CSBL_SYSSETTING_NOT_EXIST=5, ///flash��eeprom��û��syssetting, ��ͨ��m_aTag�ж�
	EM_CSBL_SYSSETTING_LENGTH =6,   ///yssetting�ĳ���m_dwLength����,����M_SYS_SETTING_LENGTH - 24
	EM_CSBL_SYSSETTING_CRC= 7 ,       ///syssetting��CRC����
	EM_CSBL_SYSSETTING_POINTER_ZERO = 8 ,       ///����Ĳ���Ϊ0
	EM_CSBL_SYSSETTING_NUM            ///����
} CSBLSyssettingErr_E;



/**
@brief �������ֵ���(QAM)tuner�ĵ�������ö�ٶ���
@note:bootloader  QAM tuner����ͬ��,����Ŀǰ��Щ��Ŀ��ͬʱ֧��AnnxB, AnnxA����Ҫ��syssetting�������ò���������loader��ʼ��tuner��ʱ�򣬻�ѡ���Ӧ��tuner.
@note: Ĭ�϶���AnnexA, �޴˼��ݵ�bootloader����Դ�ѡ��
*/
typedef enum 							
{
	EM_CSBL_TUNER_INVALID,
	EM_CSBL_TUNER_ANNEXA,    ///Annex A -    16, 32, 64, 128, 256 QAM
	EM_CSBL_TUNER_ANNEXB,    ///Annex B �C 64/256 QAM (Optional)
	EM_CSBL_TUNER_ANNEXC,   ///Annex C -     16, 32, 64, 128, 256 QAM
	EM_CSBL_TUNER_NUM         ///< ���ޡ�
}CSBLTunerQAM_E;


/**
@brief bootloader���ڴ�ӡ����ö�ٶ���
@note  : ����conax �淶Ҫ��bootloader�رմ���,��app����ͨ��syssetting��bl����.
@note  : ����淶û��Ҫ����������
*/
typedef enum 							
{
	EM_CSBL_UART_INVALID,
	EM_CSBL_UART_ON,    ///���ڴ�
	EM_CSBL_UART_OFF,    ///���ڹر�, 
	EM_CSBL_UART_NUM         ///< ���ޡ�
}CSBLUart_E;


/****
@brief    bootloader��app֮��ͬ�����ݵĽṹ��syssetting����

@note:�˽ṹ������������䣬������ܳ���M_SYS_SETTING_LENGTH,    2011.07.10

****/
typedef struct
{
	char m_aTag[M_SYS_SETTING_TAG_LENGTH]; /***��־ M_SYS_SETTING_TAG ***/
	unsigned int m_dwCrc32;	/***mpegcrc32, ���㷶ΧΪm_eBLScreenMode��ʼ�ֶ�,����Ϊm_dwLength, ***/
	unsigned int m_dwLength;	/***syssetting���ݵĳ���,���������ֶ�,m_dwLength = sizeof(CSBLSyssettingInfo_S) -  sizeof(m_aTag) - sizeof(m_dwCrc32) - sizeof(m_dwLength) ***/
	CSUDISCREENResolution_E	m_eBLScreenMode;/**@brief �������SD:��Ƶ����ֱ���udi2.0�ӿ�*/
	CSBLTunerQAM_E			m_eBLQAMType;/**@brief �������ֵ���(QAM)tuner�ĵ�������ö�ٶ���*/
	CSBLUart_E				m_eBLUart;	/**@brief bootloader���ڴ�ӡ����ö�ٶ���**/
	CSBLUILanguageType_E		m_eBLLanguage;	/**@brief bootloader������ʾö�ٶ���**/	
	unsigned int  				m_dwRFChannelNum; /**** bootloade��RF���Ƶ��CH3/4,   21---69  ,    ֱ������Ƶ��������,3,4,21,22,23....68,69****/
	CSUDISCREENResolution_E	m_eBLHDscreenMode;/**@brief �������HD:��Ƶ����ֱ���udi2.0�ӿ�*/
} CSBLSyssettingInfo_S;


/************************************************************
@brief ��ȡbootloader�д洢��syssetting

@return ����CSBLSyssettingErr_E  �ж���Ĵ���

@note: ��flash����eeprom�ж�ȡ syssetting����ȡ�������ݣ��򲻸ı�psBLSyssetting�κ�ֵ
@note: 
************************************************************/
CSBLSyssettingErr_E CSBLSyssettingRead(CSBLSyssettingInfo_S * psBLSyssetting);


/************************************************************
@brief ����bootloader�д洢��syssetting

@return ����CSBLSyssettingErr_E  �ж���Ĵ���

@note: ��syssetting����д��flash,����eeprom
@note: ���ڲ��������Զ��޸�:
@note: CSBLSyssettingInfo_S->m_aTag[] 
@note: CSBLSyssettingInfo_S->m_dwCrc32 
@note: CSBLSyssettingInfo_S->m_dwLength
@note: �����ⲿ������,ֻ��Ҫ�޸�m_eBLScreenMode�ֶο�ʼ�ı���
************************************************************/
CSBLSyssettingErr_E CSBLSyssettingWrite(CSBLSyssettingInfo_S * psBLSyssetting);






/************************************************************
booter,  loader,  app��дsyssetting����, Ŀǰ(2011.07.10) booter, loader��syssetting������ֻ��ȡ��  ��д���ԭ��.

void CSBL_Syssetting_Read(void)
{	
	CSBLSyssettingErr_E  eRet;
	CSBLSyssettingInfo_S  sSysetting;


	
	////���syssettingû������,��flash�ռ����޴�����
	////��������Զ�syssetting,����bl����Ƶ��ʽ.............
	eRet=CSBLSyssettingRead(&sSysetting);



	
	printf("CSBLSyssettingRead()=%d\r\n", eRet);	
	if(EM_CSBL_SYSSETTING_SUCCESS == eRet)
	{
		printf("m_eBLScreenMode=%d\r\n",sSysetting.m_eBLScreenMode);
		printf("m_eBLQAMType=%d\r\n",sSysetting.m_eBLQAMType);
		printf("m_eBLUart=%d\r\n",sSysetting.m_eBLUart);	
		������Խ�������Ҫ�Ĵ���.
	}
	else
	{
		printf("error!\r\n");
	}
}

void CSBL_Syssetting_Write(CSUDISCREENResolution_E eType,CSBLTunerQAM_E eQam, CSBLUart_E eUart)
{	
	CSBLSyssettingErr_E  eRet;
	CSBLSyssettingInfo_S  sSysetting;
	
	sSysetting.m_eBLScreenMode = eType;
	sSysetting.m_eBLQAMType =eQam;
	sSysetting.m_eBLUart = eUart;
	
	eRet=CSBLSyssettingWrite(&sSysetting);

	
	printf("CSBLSyssettingWrite()=%d\r\n", eRet);	
	if(EM_CSBL_SYSSETTING_SUCCESS == eRet)
	{
		printf("write  successfully!\r\n");	
	}
	else
	{
		printf("write error!\r\n");
	}
}
************************************************************/



#ifdef	__cplusplus
}
#endif


#endif


