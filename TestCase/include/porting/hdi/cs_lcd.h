/**@defgroup CSLCD[CSLCD]ͬ����Ŀר��LCD�����ӿ�
@brief ��ģ����Ҫ������ͬ����Ŀר��ƽ̨�޹�LCD�����ӿ�
@brief LCD����˼��Һ����ʾ��- Liquid Crystal Display �ļ�� ������udi��MMCP�м�㲻���õ����ģ�飬�����ھ���Ļ���MMCP����Ŀ�������ļ���Ӧ�û��õ���
 
@version 0.3 2008/11/5  ��һ��ϸ���汾
@version 0.2 2008/10/17 �����ĵ�һ���汾
@version 0.1 2008/10/16 ��ʾ�汾
@{
*/

#ifndef _CS_LCD_H_
#define _CS_LCD_H_
#include "cs_typedef.h"
#include "cs_assert.h"
#include "CSHDI_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief LCDģ��GPIO���ò�������*/
typedef struct
{
	BYTE uclcd_db0;				/**< GPIO config of data0 in Data bus*/
	BYTE uclcd_db1;				/**< GPIO config of data1 in Data bus*/
	BYTE uclcd_db2;				/**< GPIO config of data2 in Data bus*/
	BYTE uclcd_db3;				/**< GPIO config of data3 in Data bus*/
	BYTE uclcd_db4;				/**< GPIO config of data4 in Data bus*/
	BYTE uclcd_db5;				/**< GPIO config of data5 in Data bus*/
	BYTE uclcd_db6;				/**< GPIO config of data6 in Data bus*/
	BYTE uclcd_db7;				/**< GPIO config of data7 in Data bus*/

	BYTE uclcd_r_w;				/**< GPIO of Read/Write*/
	BYTE uclcd_enalble;		/**< GPIO of LCD Enable*/
	BYTE uclcd_rs;				/**< GPIO of Registor Select*/
	BYTE ucpower_on_off;	/**< GPIO of LCD power*/
	
}CSLCD_PIO_CFG_T;


/**@brief LCDģ���������߿�ȶ���*/
typedef enum
{
	MODE_8BIT,		/**< Data bus is 8 bits*/
	MODE_4BIT			/**< Data bus is 4 bits*/
}CSLCD_BUS_MODE_T;

/**@brief LCDģ��������оƬ���Ͷ���*/
typedef enum
{
	KS0070B,
	LCD_MAX_CHIP_TYPE
}CSLCD_CHIP_TYPE_T;


/**@brief LCDģ�������ƶ�������*/
typedef enum
{
	SHIFT_LEFT, 	/**< ����*/
	SHIFT_RIGHT, 	/**< ����*/
	SHIFT_UP, 		/**< ����*/
	SHIFT_DOWN 		/**< ����*/
}CSLCD_SHIFT_DIRECT_T;


/**@brief LCDģ��Ĺ��״̬����*/
typedef enum
{
	CURSOR_OFF,					/**< cursor is not displayed*/
	CURSOR_ON_NOBLINK,	/**< cursor is displayed and without blink*/
	CURSOR_ON_BLINK			/**< cursor is displayed and blink*/
	
}CSLCD_CURSOR_STATUS_T;


/**@brief LCDģ����Զ����ַ�����*/
typedef enum
{
	CHAR_RECEIVED,			/**< �����ַ�*/
	ARROW_UP,						/**< ���ϵļ�ͷ*/
	ARROW_DOWN,					/**< ���µļ�ͷ*/
	ARROW_LEFT,					/**< ����ļ�ͷ*/
	ARROW_RIGHT					/**< ���ҵļ�ͷ*/
}CSLCD_CUSTOM_CHARACTER_T;


/**@brief ģ��LCD ����Ƶ��׼*/
typedef enum
{
	EM_ALCD_NTSC,				/**< N��*/
	EM_ALCD_PAL,				/**< P��*/
	EM_ALCD_SECAM, 			/**< �����˵��*/
	EM_ALCD_NTSC_4,			/**< �����˵��*/
	EM_ALCD_PAL_M, 			/**< �����˵��*/
	EM_ALCD_PAL_CN,  		/**< �Զ����*/
	EM_ALCD_END		 			/**<  ���� */
}CSALCDVideoSTD_E;


/**@brief LCD����ʾģʽ16:9��4:3*/
typedef enum
{
	EM_ALCD_16_9,				/**< 16:9*/
	EM_ALCD_4_3					/**< 4:3*/
}CSALCDDispalyMode_E;

/**@brief LCD �ı������*/
typedef enum
{
	EM_ALCD_ON,				/**< LCD ���⿪*/
	EM_ALCD_OFF 			/**< LCD�����*/
}CSALCDSwitch_E;


/**@brief LCD �ɵ��ڲ���*/
typedef enum
{
	EM_ALCD_BRIGHTNESS,			 	/**< ���� 		 0-100*/
	EM_ALCD_CONTRAST,	       	/**< �Աȶ� 	 0-100*/
	EM_ALCD_SHARPNESS,	    	/**< ������ 	 0-10*/
	EM_ALCD_SAT,             	/**< ���Ͷ�		 0-100*/
	EM_ALCD_BACKGROUND,				/**< �������  0-10*/
	EM_ALCD_DAC,              /**< DAC����*/
	EM_ALCD_VCOMA, 	      		/**< VCOMA���� 0-32*/
	EM_ALCD_VCOMD             /**< VOMD����  0-32*/
}CSALCDPARAM_E;


/**@brief LCD OSD��ʾλ�ö���*/
typedef enum
{
	EM_ALCD_TOP_RIGHT,			/**< ���Ͻ�*/
	EM_ALCD_MIDDLE,					/**< �м�*/
	EM_ALCD_POSITION_MAX		/**< �����˵��*/
}CSALCD_OSD_POSITION_E;


/**@brief LCD ����BACKGROUP����*/
typedef enum
{
	EM_ALCD_BLUE,  							/**< ��ʾ����*/
	EM_ALCD_BLACK,   						/**< ��ʾ����*/  
	EM_ALCD_CLOSE_BACKGROUP,  	/**< �ر���*/
	EM_ALCD_MAX_BACKGROUP_TYPE 	/**<  ���� */
}CSALCD_BACKGROUP_E;


/**@brief ģ��LCD ����CVBS����ͨ������*/
typedef enum
{
	EM_ALCD_CVBS, 							/**< ѡ��CVBS����ͨ��*/
	EM_ALCD_YC, 								/**< ѡ��YC����ͨ����*/
	EM_ALCD_CHANNEL_MAX_NUM 		/**<  ���� */
}CSALCD_SourceType_E;


/**@brief ģ��LCD�ĳ�ʼ������*/
typedef struct
{
	DWORD							m_dwChipReset;	/**< dwChipResetΪT118оƬ��RESET���ŵ�GPIO������*/
	CSALCDVideoSTD_E 	m_eVideoStd;		/**< m_eVideoStdΪ�����ź���ʽ������ΪEM_ALCD_AUTO��ʾ�Զ����*/
}CSALCDInitCFG_T;

/** 
@brief LCD������ʼ����

@param[in] enmLCDType LCDоƬ���ͣ�Ŀǰֻ����KS0070B
@param[in] enmBusMode LCDģ���������߿��
@param[in] LCDPIOConfig ���ڿ��Ƶ�GPIO��������Ϣ��������ڽṹ��CSLCD_PIO_CFG_T��������
@return CSHDI_SUCCESS��ʾ��ʼ���ɹ���CSHDI_FAILURE��ʾ��ʼ��ʧ�ܡ� 
*/
CSHDI_Error_t CSLCDInit( CSLCD_CHIP_TYPE_T enmLCDType, CSLCD_BUS_MODE_T enmBusMode, CSLCD_PIO_CFG_T LCDPIOConfig);

/** 
@brief ���Ƶ�Դ���أ����ڿ��Ʊ�����

@param[in] enmLCDPower CSHDI_OFF����  CSHDI_ON����
@return CSHDI_SUCCESS��ʾ���óɹ���CSHDI_FAILURE��ʾ����ʧ�ܡ�
*/
CSHDI_Error_t CSLCDPower( CSHDISwitch enmLCDPower);

/** 
@brief ����

@return CSHDI_SUCCESS��ʾ���óɹ���CSHDI_FAILURE��ʾ����ʧ�ܡ�
*/
CSHDI_Error_t CSLCDDisplayClear(void);

/** 
@brief ��ָ������ʼλ����ʾ�ַ��������ܿ��У�

@param[in] nRowNo ��ʼλ�õ��к�
@param[in] nColumnNo ��ʼλ���к�
@param[in] pucStringToDisplay ���Ҫ��ʾ�ַ��Ļ�����׵�ַ
@param[in] nLength Ҫ��ʾ�ַ��ĳ���
@return CSHDI_SUCCESS��ʾ���óɹ���CSHDI_FAILURE��ʾ����ʧ�ܡ�
*/
CSHDI_Error_t CSLCDDisplay(int nRowNo, int nColumnNo, BYTE *pucStringToDisplay,int nLength);

/** 
@brief �ƶ����

@param[in] enmDirect ����ƶ�����,SHIFT_LEFT:����,SHIFT_RIGHT:����,SHIFT_UP:����,SHIFT_DOWN:����
@param[in] nTimes ����ƶ�����
@return CSHDI_SUCCESS��ʾ���óɹ���CSHDI_FAILURE��ʾ����ʧ�ܡ�
*/
CSHDI_Error_t CSLCDCursorShift(CSLCD_SHIFT_DIRECT_T enmDirect, int nTimes) ;

/** 
@brief �ƶ���Ļ������ʾ�ַ��͹��

@param[in] enmDirect ����ƶ�����,HIFT_LEFT:����,SHIFT_RIGHT:����
@param[in] nTimes �ƶ�����
@return CSHDI_SUCCESS��ʾ���óɹ���CSHDI_FAILURE��ʾ����ʧ�ܡ�
*/
CSHDI_Error_t CSLCDDisplayShift(CSLCD_SHIFT_DIRECT_T enmDirect, int nTimes);

/** 
@brief �ƶ���굽��Ļ���Ͻ�

@return CSHDI_SUCCESS��ʾ���óɹ���CSHDI_FAILURE��ʾ����ʧ�ܡ�
*/
CSHDI_Error_t CSLCDCursorReturnHome(void);

/** 
@brief ���ù����ʾ��ʽ

@param[in] enmStatus ��ʾ��ʽ,CURSOR_OFF:��겻��ʾ,CURSOR_ON_NOBLINK:�����ʾ������˸,CURSOR_ON_BLINK:�����ʾ����˸
@return CSHDI_SUCCESS��ʾ���óɹ���CSHDI_FAILURE��ʾ����ʧ�ܡ�
*/
CSHDI_Error_t CSLCDCursorStatusSet( CSLCD_CURSOR_STATUS_T enmStatus);

/** 
@brief �Ե�ǰ�������λ��Ϊ��ʼλ����ʾ�ַ���

@param[in] pucStringToDisplay �洢Ҫ��ʾ�ַ��Ļ�����׵�ַ
@param[in] nLength ��ʾ�ַ��ĳ���
@return CSHDI_SUCCESS��ʾ���óɹ���CSHDI_FAILURE��ʾ����ʧ�ܡ�
*/
CSHDI_Error_t CSLCDDisplayCurrentPos( BYTE *pucStringToDisplay,int nLength);

/** 
@brief ָ������λ��

@param[in] nRowNo ������
@param[in] nColumnNo ������
@return CSHDI_SUCCESS��ʾ���óɹ���CSHDI_FAILURE��ʾ����ʧ�ܡ�
*/
CSHDI_Error_t CSLCDSetPos(int nRowNo,int nColumnNo);

/** 
@brief ָ������λ��

@param[in] enmStatus CSHDI_OFF���أ�CSHDI_ON����
*/
void BackGroundLightCtr( CSHDISwitch enmStatus);

/** 
@brief ��ʾ�ض��ַ�

@param[in] code �ض��ַ������ͷ�����
@return ��ʾ�ɹ��򷵻�CSHDI_SUCCESS�����򷵻�CSHDI_FAILURE
*/
CSHDI_Error_t CSLCDDisplayCustomCharacter( CSLCD_CUSTOM_CHARACTER_T code);

/** 
@brief ��ȡLCD�����汾��

@param[in]   nSize  �汾���ַ�����
@param[out] pchVer �汾�ŵ�ַ
@return ��ȡ���汾�ŵĳ��ȡ�
*/
int CSLCDGetVersion( PCHAR pchVer, int nSize );

/** 
@brief ��LCD����оƬT118��ʼ��������������grmmaֵ�Ȳ�����

@param[in] eI2CIndex LCD����оƬT118���ڵ�I2C������
@param[in] nPriority ALCDģ���̵߳����ȼ������߳����ڼ��LCD�Ƿ����źź�NSCT�ƺ�PAL���Զ���⡣
@param[in] psInitConfig  ��������LCD��ʼ��������ָ�롣
@return CSHDI_FAILURE����ʼ��ʧ�ܡ�CSHDI_SUCCESS����ʼ���ɹ���
*/
CSHDI_Error_t CSALCDInitI2C( CSHDII2CIndex eI2CIndex, int nPriority, CSALCDInitCFG_T *psInitConfig);

/** 
@brief ѡ��LCD����ͨ����

@param[in] eSourceType EM_ALCD_CVBS��ѡ��CVBS����ͨ����EM_ALCD_YC��ѡ��YC����ͨ����
@return CSHDI_SUCCESS���ɹ���CSHDI_FAILURE��ʧ��
*/
CSHDI_Error_t CSALCDSourceSelect(CSALCD_SourceType_E eSourceType);

/** 
@brief ����LCD���⡣

@param[in] eLCDBGSwitch EM_ALCD_ON��LCD���⿪��EM_ALCD_OFF��LCD����ء�
@return CSHDI_SUCCESS���ɹ���CSHDI_FAILURE��ʧ�ܡ�
*/
CSHDI_Error_t CSALCDCntBackLight( CSALCDSwitch_E eLCDBGSwitch);

/** 
@brief ��LCD����ʾ��������塣

@param[in] eLCDDISSwitch ��ʾ�����OSD��ʾ��EM_ALCD_ON��ʾ��ʾ��EM_ALCD_OFF��ʾ�����
@param[in] eOSDPosition EM_ALCD_TOP_RIGHT��ʾ�����Ͻ���ʾ��EM_ALCD_MIDDLE��ʾ����Ļ�м���ʾ��
@param[in] pucOSDText ��ʾ���ַ�����Ŀǰ��ʾ���ַ��������ޡ�
@return CSHDI_SUCCESS���ɹ���CSHDI_FAILURE��ʧ�ܡ�
*/
CSHDI_Error_t CSALCDCntDisplay( CSALCDSwitch_E eLCDDISSwitch, CSALCD_OSD_POSITION_E eOSDPosition, PBYTE pucOSDText);

/** 
@brief ���OSD��ʾ��

@return CSHDI_SUCCESS���ɹ���CSHDI_FAILURE��ʧ�ܡ�
*/
CSHDI_Error_t CSALCDClearOSD(void);

/** 
@brief ����LCD�ı�����

@param[in] eBackGroupType EM_ALCD_BLUE����ʾ������EM_ALCD_BLACK����ʾ������EM_ALCD_CLOSE_BACKGROUP���ر���
@return CSHDI_SUCCESS���ɹ���CSHDI_FAILURE��ʧ�ܡ�
*/
CSHDI_Error_t CSALCDCntBackGround( CSALCD_BACKGROUP_E eBackGroupType);

/** 
@brief ����LCD������

@param[in] eLCDParam
- EM_ALCD_BRIGHTNESS����ʾ��������
- EM_ALCD_CONTRAST����ʾ���öԱȶ�
- EM_ALCD_SHARPNESS����ʾ����������
- EM_ALCD_SAT����ʾ���ûҶ�
- EM_ALCD_BACKGROUND����ʾ���ñ���
- EM_ALCD_DAC��DAC����
- EM_ALCD_VCOMA��VCOMA����
- EM_ALCD_VCOMD��VCOMD����
@param[in] nValue
@return CSHDI_SUCCESS���ɹ���CSHDI_FAILURE��ʧ�ܡ�
*/
CSHDI_Error_t CSALCDSetCntParam( CSALCDPARAM_E eLCDParam, int nValue);

/** 
@brief ��ȡLCD�Ĳ�����

@param[in] eLCDParam LCD��������
@param[in] pnValue LCD��Ӧ����ֵ��ָ�롣
@return CSHDI_SUCCESS���ɹ�,CSHDI_FAILURE��ʧ�ܡ�
*/
CSHDI_Error_t CSALCDGetCntParam( CSALCDPARAM_E eLCDParam, int *pnValue);

/** 
@brief ���LCD�Ƿ����ź����롣

@param[in] pbSignal ��ȡ�Ƿ����źŵ�ֵ�ĵ�ַ��TRUE��ʾ���źţ�FLASE��ʾ���źš�
@return CSHDI_SUCCESS���ɹ���CSHDI_FAILURE��ʧ�ܡ�
*/
CSHDI_Error_t CSALCDDetectSignal (BOOL *pbSignal);

/** 
@brief ��ȡ��Ƶ�����׼��

@param[in] peVideoType ��ŵ�ַ�����CSALCDVideoSTD_E���塣
@return CSHDI_SUCCESS���ɹ���CSHDI_FAILURE��ʧ�ܡ�
*/
CSHDI_Error_t CSALCDDetectSignalStd(CSALCDVideoSTD_E *peVideoType);

/** 
@brief �����ź������׼��

@param[in] eVideoType �ź������׼�����CSALCDVideoSTD_E���塣
@return CSHDI_SUCCESS���ɹ���CSHDI_FAILURE��ʧ�ܡ�
*/
CSHDI_Error_t CSALCDSetSignalStd(CSALCDVideoSTD_E eVideoType);

/** 
@brief ע��ص������������ص���ǰ�Ƿ����źŵ���Ϣ��

@param[in] fnCallBackFun �ص������ĵ�ַ
@return CSHDI_SUCCESS���ɹ���SHDI_FAILURE��ʧ�ܡ�
*/
CSHDI_Error_t CSALCDRegisterKeySendCallback( CSHDICallback fnCallBackFun);

/** 
@brief ��ȡģ��LCD�����汾��

@param[in] nSize  �汾���ַ�����
@param[out] pchVer �汾�ŵ�ַ
@return ��ȡ���汾�ŵĳ��ȡ�
*/
int CSALCDGetVersion( PCHAR pchVer, int nSize );

#ifdef __cplusplus
}
#endif

/** @} */

#endif

