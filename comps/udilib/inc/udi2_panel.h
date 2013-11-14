/**@defgroup PANEL PANEL ������ǰ�����ʾ��ؽӿ�

@brief PANELģ���Ӧͷ�ļ�<udi2_panel.h>���������ն�LED��VFD��������ʾ���ֽӿڡ�

��ģ��������ʾ���ͻ��ֳ��ı��ơ����Ƶơ���ϵ�������ʾ���ͣ��Ե���Ψһ��ʶһ���ơ��������Ѿ��������ֵ��ӻ������ն˳�����������ʾ�ƹ��ܣ�������©����COSHIPЭ�����ӡ�

�ı�����ָ��������ı��ı���ʾ������(Ӣ�ĺ�����)�������ʾ�ƣ��������ϵķָ�����"��"�ȣ�����7�ιܣ����ֹܡ���ͼ��Ϊһ���ı��Ƶ�ʾ����

@image HTML udi_panel_text.png

�ı��Ƶ����Խ�����������ʾ���ַ�������ʾֵ����:
- �ı�ģʽ����ֱ��������ʾ���ı�����������ı��ַ��������ı��ӵ�����ʱ�����Գ����Ĳ��֣���δ�����Ĳ�������Ҫ������ʾ��
- ʱ��ģʽ�������뵱ǰ������ʱ�䣬������ʵ�����и������������������ʾ���ʱ�䣬�����˸�������

�ı��Ƶ��������ڱ�ʾ�ı��Ƶ����ι�ϵ������TEXT0��ʾ���ơ�TEXT1��֮��TEXT2�ٴΣ��Դ����ơ�һ����˵��Ӧ��������Ҫ���������ʾ��ǰ����״̬�����ȡTEXT0�ı��ƣ�����ܳɹ���ȡ����ݸ��ı��Ƶĳ��ȣ���������ʾ�����棬����������ʾ���衣

���Ƶ���ָֻ����ʾ�̶�ͼ��������ʾ�ƣ������ʼ�ָʾ�ƣ��ź�ָʾ�Ƶȡ���ͼ��ΪһЩ���ƵƵ�ʾ����

@image HTML udi_panel_ctrl.png

���ƵƵ����԰���������ʾ��������ʾ���������Ƿ���� - ��������˸���Եڶ�����ɫ�����Եڶ�����ɫ��˸��������ɫ������˸�ȼ�����������ϡ���ʾֵ���ǽ���״̬����Ϊ���ϼ���״̬֮һ��������������״̬Ϊ�������ܣ�����֧�֡�

���ƵƵ��������ڱ�ʾ���ƵƵ����壬���źŵ�һ��������ʾ�źŵ��������һ����˵Ӧ��������Ƶ�ɹ����ӱ�ģ���ȡ�źſ��Ƶƣ�����ܻ�ȡ������������������κβ������������Ƶ���Ȼ��

��ϵ���ָ�����صƵĵ��飬�繤�̻��ϵ��ź�ǿ�ȵ��飬VFD�ϳ�����Բ�ε���ȡ���Щ���������������һ�𣬱�ʾһЩ��������壬��һ�����֣���̬Loadingͼ��ȵȡ���ͼ��Ϊһ����ϵƵ�ʾ����

@image HTML udi_panel_suit.png

��ϵƵ����԰�������ư������ӵ���������ͼ�����ʾ����ϵƾͰ���8���ӵƣ��ӵƱ����ǿ��Ե�������״̬�ġ�����ʾֵ���Ǵӵ͵��ߣ���nλ��ʾn���ӵƵ�����״̬��0��ʾϨ��1��ʾ�����������ӵƵı�Ź�������ֲ�������������ѭ�������Ʊ�ʾ����ʱ��Ӧ����1�ŵƵ�������ʾ1��1��2�ŵ�����ʾ2��1��2��3�ŵ�����ʾ3���Դ����ơ�����ͼ�ұߵ���ϵ�Ϊ��������˳��Ӧ���Ǵ��������ε�����

�ն���������壬��һ��Ҫ֧������������ʾ�ƣ�����֧�ֻ��߲����ڸ����͵�ʱ��ֻ��Ҫ��GetCapabilityByNameʱ����CSUDIPANEL_ERROR_NOT_SUPPORTED���ɡ�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

@version 2.0.2 2009/09/02 ���ӿ��Ƶ�˫ɫ������˸����
@version 2.0.1 2009/08/26 �޸������ʾ����������ʽ����Ϊÿ����ʾ���Ͷ��ǵƵ�һ��
@version 2.0.1 2009/08/24 ��һ���ۺ�������޸ĵİ汾
@version 2.0.0 2009/07/23 ����
@{
*/
#ifndef _UDI2_PANEL_H_
#define _UDI2_PANEL_H_

#include "udi2_typedef.h"
#include "udi2_error.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief PANEL �豸��ش������ֵ*/
enum
{
	CSUDIPANEL_ERROR_BAD_PARAMETER = CSUDI_PANEL_ERROR_BASE,  ///< �������� 
	CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED,					  ///< ������֧��
	CSUDIPANEL_ERROR_UNKNOWN_ERROR,					  ///< һ�����
	CSUDIPANEL_ERROR_INVALID_DEVICE_ID						  ///< �Ƿ��豸ID
};


/**@brief �����ʾ����ö�ٶ���

@note �ı�����ʾ������Ҫָ��������ı��ı���ʾ������(Ӣ�ĺ�����)������7�ι�,���ֹܵ�
@note ���Ʒ�����ʾ������Ҫָ�̶�ͼ����ʾ����,��ǰ����ϵ��ʼ�ָʾ��,�ź�ָʾ�Ƶ�
@note ��ϵ���ʾ������Ҫ��ʾ�����صƵĵ���,�繤�̻��ϵ��ź�ǿ�ȵ���,VFD�ϳ�����Բ�ε���
*/
typedef enum
{
	EM_UDIPANEL_CTRLSTART,       	///< ���Ʒ�����ʾ������ʼֵ,����Ϊ���ͱ�ʶ
	
	EM_UDIPANEL_MAIL,     	        ///< Mail��
	EM_UDIPANEL_SIGNAL,  	        ///< Signal��
	EM_UDIPANEL_AUDIO, 	       		///< Audio��
	EM_UDIPANEL_PLAY, 				///< ���� 
	EM_UDIPANEL_PAUSE, 	        	///< ��ͣ 
	EM_UDIPANEL_STOP, 				///< ֹͣ 
	EM_UDIPANEL_HDTV,				///< HDTV 
	EM_UDIPANEL_BACKWARD,           ///< ����
	EM_UDIPANEL_FORWARD, 	        ///< ��� 
	EM_UDIPANEL_BALL, 				///< Բ
	EM_UDIPANEL_DUBY, 				///< �ű� 
	EM_UDIPANEL_PREVIOUS, 	        ///< ǰһ��
	EM_UDIPANEL_NEXT, 				///< ��һ��
	EM_UDIPANEL_IR, 				///< ң����
	EM_UDIPANEL_USB, 				///< USB
	EM_UDIPANEL_N,					///< N
	EM_UDIPANEL_VOD, 				///< VOD
	EM_UDIPANEL_POWER, 	        	///< ��Դ
	EM_UDIPANEL_TV, 				///< ����
	EM_UDIPANEL_NETWORK, 	        ///< ����
	EM_UDIPANEL_RADIO, 	        	///< �㲥
	EM_UDIPANEL_CLOCK,				///< ʱ��
	EM_UDIPANEL_MODEM,	        	///< MODEM 
	EM_UDIPANEL_REC,				///< ¼�� 
	EM_UDIPANEL_UP,					///< ��
	EM_UDIPANEL_DOWN,				///< ��
	EM_UDIPANEL_LEFT,				///< ��
	EM_UDIPANEL_RIGHT,				///< ��			
	
	EM_UDIPANEL_CTRLALL,       		///< ���Ʒ�����ʾ���ͽ���ֵ
	
	EM_UDIPANEL_TEXTSTART ,      ///< �ı�����ʾ������ʼֵ,����Ϊ���ͱ�ʶ,TEXT0-TEXT4����������˳������
	
	EM_UDIPANEL_TEXT0,	    	///< ��һ���ı�
	EM_UDIPANEL_TEXT1,          ///< �ڶ����ı�
	EM_UDIPANEL_TEXT2,	    	///< �������ı�	
	EM_UDIPANEL_TEXT3,          ///< �������ı�
	EM_UDIPANEL_TEXT4,          ///< �������ı�
	
	EM_UDIPANEL_TEXTALL,        ///< �ı�����ʾ���ͽ���ֵ
	
	EM_UDIPANEL_SUITSTART ,      ///< ��ϵ���ʾ������ʼֵ,����Ϊ���ͱ�ʶ
	
	EM_UDIPANEL_SUIT0,          ///< ��һ����ϵ�
	EM_UDIPANEL_SUIT1,          ///< �ڶ�����ϵ�
	EM_UDIPANEL_SUIT2,          ///< ��������ϵ�
	
	EM_UDIPANEL_SUITALL,	    ///< ��ϵ���ʾ���ͽ���ֵ

	EM_UDIPANEL_EXT_CTRL_START = 0x1000,  ///< ��չ���Ʒ�����ʾ������ʼֵ,����Ϊ���ͱ�ʶ������������ӿ��Ƶ����ڴ˺����
	EM_UDIPANEL_CMRS,					///< CM ״̬:  �շ�
	EM_UDIPANEL_CMONLINE,			///< CM ״̬:  ����
	EM_UDIPANEL_CMUPLINK,			///< CM ״̬:  ����
	EM_UDIPANEL_CMDOWNLINK,		///< CM ״̬:  ����
	EM_UDIPANEL_EXT_CTRL_END,

	EM_UDIPANEL_EXT_TEXT_START  =0x2000,  ///< ��չ�ı�����ʾ������ʼֵ,����Ϊ���ͱ�ʶ��������������ı������ڴ˺����
	EM_UDIPANEL_EXT_TEXT_END  ,
	
	EM_UDIPANEL_EXT_SUIT_START = 0x3000,  ///< ����ı�����ʾ������ʼֵ,����Ϊ���ͱ�ʶ���������������ϵ����ڴ˺����
	EM_UDIPANEL_EXT_SUIT_END ,

	EM_UDIPANEL_MAX             ///< �߽�ֵ	
}CSUDIPanelLightName_E;

#define CSUDI_PANEL_IS_TEXT(name)	(  ( (name) > EM_UDIPANEL_TEXTSTART &&  (name) < EM_UDIPANEL_TEXTALL ) || ( (name) > EM_UDIPANEL_EXT_TEXT_START &&  (name) < EM_UDIPANEL_EXT_TEXT_END )  ) ///< �ж�name�Ƿ����ı�����ʾ����
#define CSUDI_PANEL_IS_CTRL(name)   (  (  (name) > EM_UDIPANEL_CTRLSTART &&  (name) < EM_UDIPANEL_CTRLALL ) || (  (name) > EM_UDIPANEL_EXT_CTRL_START &&  (name) < EM_UDIPANEL_EXT_CTRL_END ) ) ///< �ж�name�Ƿ��ǿ��Ʒ�����ʾ����
#define CSUDI_PANEL_IS_SUIT(name)   (  (  (name) > EM_UDIPANEL_SUITSTART &&  (name) < EM_UDIPANEL_SUITALL ) || (  (name) > EM_UDIPANEL_EXT_SUIT_START &&  (name) < EM_UDIPANEL_EXT_SUIT_END ) )///< �ж�name�Ƿ�����ϵ���ʾ����

/**@brief �ı�����ʾ����������Ϣ */
typedef struct
{
	int             m_nTextCount;                 ///< �����ı�֧����ʾ���ַ�����	
}CSUDIPanelTextLightAttr_S;

/**@brief ��ϵ���ʾ����������Ϣ

@note ��ȡ����еƹܵĸ��������ڶ�ÿ���ƹܵ�״̬���в���
*/
typedef struct
{	
	int m_nSuitCount;                 ///< ��ϵ��ӵ�����
}CSUDIPanelSuitLightAttr_S;

/**@brief ���Ʒ�����ʾ���͹���ö�ٶ��� 
@note ���е�����Ϩ���ǻ������ܱ���֧��
*/
typedef enum
{
	EM_UDIPANEL_CTRL_OFF		 = 1 << 0,		///< �ÿ��Ƶƿ���Ϩ��
	EM_UDIPANEL_CTRL_ON			 = 1 << 1,		///< �ÿ��Ƶƿ��Ե���,���֧��color2��ʹ��Ĭ����ɫ��ʾ
	EM_UDIPANEL_CTRL_FLASH		 = 1 << 2,		///< �ÿ��Ƶƿ�����˸��Ĭ����ɫ����˸
	EM_UDIPANEL_CTRL_COLOR2		 = 1 << 3,		///< �ÿ��Ƶ�֧�ֵڶ�����ɫ
	EM_UDIPANEL_CTRL_COLOR2FLASH = 1 << 4,		///< �ÿ��Ƶ�֧���Եڶ�����ɫ��˸
	EM_UDIPANEL_CTRL_COLORSWITCH = 1 << 5     ///< ���Ƶ�֧��������ɫ������˸
}CSUDIPanelCtrlLightFun_E;

/**@brief ���Ʒ�����ʾ����������Ϣ*/
typedef struct
{
	CSUDI_UINT32	m_dwCtrFunMask;   ///< ��ӦλΪ1��ʾ���иù���ֵ,ȡֵΪCSUDIPanelCtrlLightFun_E�и�ֵ���, ����(EM_UDIPANEL_FLASH |EM_UDIPANEL_COLOR2)
}CSUDIPanelCtrlLightAttr_S;

/**@brief �豸������Ϣ*/
typedef struct
{
	CSUDIPanelLightName_E m_eLightName;         ///< ��ʾ�����ƣ���Ҫ����ȷ�ϵ����ͣ����CSUDIPanelLightName_E
	union
	{
		CSUDIPanelTextLightAttr_S	m_sText;    ///< �ı�����ʾ����
		CSUDIPanelSuitLightAttr_S	m_sSuit;	///< ��ϵ���ʾ����
		CSUDIPanelCtrlLightAttr_S	m_sCtrl;	///< ���Ʒ�����ʾ����
	}m_uProperty;                               ///< ��ʾ�����������Ͻṹ��,����m_eLightName�ֶε�����ȡ��Ӧ��ʾ�豸���������ֶ�
}CSUDIPanelLightCapability_S;

/**@brief �ı�����ʾ����ö��*/
typedef enum
{
	EM_UDIPANEL_TEXT_STRING,			///< �ַ�������
	EM_UDIPANEL_TEXT_TIME			///< ʱ������
}CSUDIPanelTextType_E;

/**@brief �ı�����ʾ���͵���ʾֵ*/
typedef struct
{
	CSUDIPanelTextType_E  m_eDisplayType;	///< ��ʾ����
	char				m_szStr[32];        /**< ��m_eDisplayTypeΪ�ı�������Ϊ��ʾ�ַ���,�ַ���������0��β,��Ӧ�ֽ�Ϊ�ո��ʾϨ����ַ�;���ַ������������ӵ��������Գ�������
												 ��m_eDisplayTypeΪʱ������������Ϊyyyymmddhhmmss��ʽ��ʱ��,������ʾ��ʽ��������ŵ���˸��ʽ����ֲ�����и���������ȷ�� */
    int 				m_nStrCount;        ///< Ҫ��ʾ���ַ���������󲻳���32���ַ�
}CSUDIPanelTextValue_S;

/**@brief ��ϵ���ʾ���͵���ʾֵ

@note Ĭ��һ�����������32����
*/
typedef struct
{
	CSUDI_UINT32	m_dwSuitValueMask;         ///< �ӵ͵��ߵ�nλ��ֵ��ʾ����е�n���Ƶ�״̬,0��ʾϨ��,1��ʾ����	
}CSUDIPanelSuitValue_S;


/**@brief ���Ʒ�����ʾ���͵���ʾֵ*/
typedef struct
{
	CSUDIPanelCtrlLightFun_E	m_eCtrlValue;	///< ��ʾ�ÿ��ƵƵ���ʾ״̬
}CSUDIPanelCtrlValue_S;

/**@brief ��ʾ���͵���ʾֵ */
typedef struct
{
	CSUDIPanelLightName_E m_eLightName;         ///< ��ʾ�����ƣ����CSUDIPanelLightName_E
	union
	{
		CSUDIPanelTextValue_S		m_sTextValue;
		CSUDIPanelSuitValue_S		m_sSuitValue;
		CSUDIPanelCtrlValue_S		m_sCtrlValue;
	}m_uValue;                         ///< ��ʾ������ʾֵ���Ͻṹ��,����m_eLightName�ֶε�����ȡ��Ӧ��ʾ�豸���������ֶ�	
}CSUDIPanelLightValule_S;

/**
@brief �õ�ָ�����ܵƵ�������Ϣ

@param[in] nPanelIndex ��ʾ�豸��������Ĭ�ϴ�0��ʼ��,ֱ��û�и�������
@param[in] eName  ��ʾ�豸������ 
@param[out] psLightCapablity  ���������ָ����ʾ���͵����Խṹ��Ϣ��ָ��
@return CSUDI_SUCCESS��ʾ�ɹ�,�����豸�ŷ���CSUDIPANEL_ERROR_INVALID_DEVICE_ID,�����ڶ�Ӧ�Ʒ���CSUDIPANEL_ERROR_NOT_SUPPORTED
@note eName������ָ��ΪEM_UDIPANEL_CTRLSTART��EM_UDIPANEL_CTRLALL��EM_UDIPANEL_TEXTSTART��
EM_UDIPANEL_TEXTALL��EM_UDIPANEL_SUITSTART��EM_UDIPANEL_SUITALL��EM_UDIPANEL_MAXö�����ͣ����򷵻�CSUDIPANEL_ERROR_BAD_PARAMETER
*/
CSUDI_Error_Code CSUDIPanelGetCapabilityByName(int nPanelIndex,CSUDIPanelLightName_E eName,CSUDIPanelLightCapability_S * psLightCapablity);

/**
@brief ����ָ����ʾ���͵���ʾ����
 
@param[in] nPanelIndex  ��ʾ�豸��������Ĭ�ϴ�0 ��ʼ����,ֱ��û�и�������
@param[in] psValue ָ���Ƶ���ʾֵ
@note �ýӿڵ��óɹ������õ������������������ʾ
@return CSUDI_SUCCESS ��ʾ�ɹ�,�����豸�ŷ���CSUDIPANEL_ERROR_INVALID_DEVICE_ID,�����ڶ�Ӧ�Ʒ���CSUDIPANEL_ERROR_NOT_SUPPORTED
@note psValue->m_eLightName������ָ��ΪEM_UDIPANEL_CTRLSTART��EM_UDIPANEL_CTRLALL��EM_UDIPANEL_TEXTSTART��
EM_UDIPANEL_TEXTALL��EM_UDIPANEL_SUITSTART��EM_UDIPANEL_SUITALL��EM_UDIPANEL_MAXö�����ͣ����򷵻�CSUDIPANEL_ERROR_BAD_PARAMETER
*/
CSUDI_Error_Code CSUDIPanelDisplay(int nPanelIndex,const CSUDIPanelLightValule_S * psValue);

/**
@brief ��������ʾ��Ϣ
@param[in] nPanelIndex  ��ʾ�豸��������Ĭ�ϴ�0 ��ʼ����,ֱ��û�и�������
@param[in] eName   ѡ��Ҫ�������ʾ���ͣ����CSUDIPanelLightName_E
@note 
    - ���eName����ΪEM_UDIPANEL_TEXTALL��������TEXT��ʾ���Ͷ���Ϩ��
    - ���eName����ΪEM_UDIPANEL_CTRLALL��������CTRL��ʾ���Ͷ���Ϩ��
    - ���eName����ΪEM_UDIPANEL_SUITALL��������SUIT��ʾ���Ͷ���Ϩ��
    - ���eName����ΪEM_UDIPANEL_MAX����������ʾ���Ͷ���Ϩ��
@return CSUDI_SUCCESS ��ʾ�ɹ���ʧ���򷵻ش������ֵ
*/
CSUDI_Error_Code CSUDIPanelClear(int nPanelIndex,CSUDIPanelLightName_E eName);

#ifdef __cplusplus
}
#endif

/** @} */

#endif



