/**@defgroup SMC SMARTCARD ���������ܿ���ؽӿ�
@brief ����ģ���Ӧͷ�ļ�<udi2_smc.h>���������ܿ���صĲ����ӿ� ��

���ܿ�ģ��Ӧ�ý������ܿ���ģ�飬ģ�����нӿڡ��豸��Ӧ�ģ�����ָ�ն��ϵ����ܿ���۶������ܿ���

���۵�������ָ�������õ��ҿ���Ҫ�������ܿ��Ŀ��ۡ������оƬ֧���������ۣ���ϵͳֻ����һ������ô��ʱӦ����ֻ��һ�����ܿ����ۡ�

ʵ����Ŀ�У����Ƿ��ֲ�ͬCA�����ܿ��������ϣ����ڽϴ���졣���UDI������Ҫ����ͬ��CA���в�ͬ���������ò����е��ԡ�UDI�Ĳ���ֻ��ʹ�����ղ�Ʒ��ѡ�õ����ܿ����У�����֤���ܿ�����������࿨����ͨ�š��������յĲ�ƷΪһ��Irdeto CA���նˣ����������ܿ���صĲ��Ծ���ʹ��Irdeto���ܿ����У������ע�ò�����ֲ���Ƿ������������CA������ͨ�š�

@note  ��ΪNDS CAʹ��ICAMģ�������ܿ�ͨѶ���������ΪNDS��Ʒ��������ѭNDS�淶ʹ��оƬ�����ṩ��ICAMģ�飬������ʵ�ֱ�ģ�顣

@version 2.0.1 2009/8/18 
@{
*/

#ifndef _UDI2_SMC_H_
#define _UDI2_SMC_H_

#include "udi2_typedef.h"
#include "udi2_error.h"

#ifdef  __cplusplus
extern "C" {
#endif


/**@brief ���ܿ���ش������ֵ*/
enum
{
	CSUDISMC_ERROR_BAD_PARAMETER = CSUDI_SMC_ERROR_BASE,		///< �������
	CSUDISMC_ERROR_ERROR_PARITY,	///< �����У��ֵ
	CSUDISMC_ERROR_ERROR_ANSWER,	///< Ӧ�����,����Ӧ�������У��ʧ��
	CSUDISMC_ERROR_TIMEOUT,			///< ��ʱ����
	
	CSUDISMC_ERROR_OUT ,			///< ���ܿ��γ�״̬�����      								 
	CSUDISMC_ERROR_IN,				///< ���ܿ�����״̬�����
	CSUDISMC_ERROR_CARD_BUSY,		///< ���ܿ���æ״̬�����
	CSUDISMC_ERROR_NOT_LOCK_OWNER,	///< ���ܿ�����״̬��������ִ������ָ��
	CSUDISMC_ERROR_READY,			///< ���ܿ�׼����

	CSUDISMC_ERROR_MUTE,			///< ��Ч�������練�忨,������Ƭ�����뷹���Ȳ���ʶ��Ŀ�Ƭ
	CSUDISMC_ERROR_ERROR_CARD,		///< ���󿨣��������Ǳ�CAϵͳ�Ļ�ĥ�����صĿ�Ƭ��������ʶ�����CA��
	CSUDISMC_ERROR_INVALID_PROTOCOL,///< �Ƿ�Э��
	CSUDISMC_ERROR_NOT_RESET,		///< ���ܿ�û������
	CSUDISMC_ERROR_FAILURE,			///< ���ܿ�ͨ�ô���
	CSUDISMC_ERROR_NOT_SUPPORT		///< ���ܿ���֧�ָ�����
};

/**@brief ���ܿ�Э��*/
typedef enum 
{
    	EM_UDISMC_PROTOCOL_UNKNOWN,		    ///< δ֪Э�� 
	EM_UDISMC_PROTOCOL_T0,				 
	EM_UDISMC_PROTOCOL_T1,				 
	EM_UDISMC_PROTOCOL_T14,			    
	EM_UDISMC_PROTOCOL_MAX				///< ����  
}CSUDISMCProtocol_E;

/**@brief ���ܿ���׼*/
typedef enum 
{
	EM_UDISMC_STANDARD_ISO,			///< ISO��׼��֧��T0��T1Э��
	EM_UDISMC_STANDARD_EMV2000,		///< EMV2000��׼ 
	EM_UDISMC_STANDARD_IRDETO,		///< ���ϵ±�׼ 
	EM_UDISMC_STANDARD_MAX			///< ���� 
}CSUDISMCStandard_E;

/**@brief ���ܿ�У��λ��Ϣ*/
typedef enum
{
	EM_UDISMC_8BITS_NO_PARITY,     ///< 8λ����żУ��
	EM_UDISMC_8BITS_ODD_PARITY,    ///< 8λ��У��
	EM_UDISMC_8BITS_EVEN_PARITY,   ///< 8λżУ��
	EM_UDISMC_PARITY_MAX                ///<����
}CSUDISMCDataBits_E;

/**@brief ֹͣλ����ö�ٶ���*/
typedef enum
{
	EM_UDISMC_STOP_0,    ///< ��ֹͣλ
	EM_UDISMC_STOP_1,    ///< 1��bit��ֹͣλ
	EM_UDISMC_STOP_2,    ///< 2��bit��ֹͣλ
	EM_UDISMC_STOP_3,     ///< 3��bit��ֹͣλ
	EM_UDISMC_STOP_MAX ///<����
}CSUDISMCStopBits_E;

/**@brief ���ܿ�������Ϣ*/
typedef struct
{
	int    m_nETU;                      ///< ETU:372,625...
	int    m_nBaudRate;                 ///< �����ʣ����磺9600,115200
	int    m_nFrequency;                ///< ʱ��Ƶ��:3570KHZ/4500KHZ
	CSUDISMCStandard_E  m_eStandard;    ///< ���ܿ���׼����ʼ��������󣬺���Ĳ���ͨ��get��ȡ���ֶ�
	CSUDISMCProtocol_E  m_eProtocol;    ///< ���ܿ�����Э��:T0/T1/T14
	CSUDISMCStopBits_E  m_eStopBit;     ///< ֹͣλ
	CSUDISMCDataBits_E  m_eCheckBit;    ///< У��λ:��У��/żУ��/��У��	
}CSUDISMCParam_S;


/**
@brief ���ܿ��ص�����

@param[in] nCardIndex ���ܿ�����
@param[in] eStatus ���ܿ�״̬�������ܿ���ش������ֵ�Ķ���
*/
typedef void (*CSUDISMCCallback)(int nCardIndex, CSUDI_Error_Code eStatus);

/**
@brief ��ȡϵͳ�п��õ����ܿ����۸���

@param[out] pnSMCCount ϵͳ�п��õ����ܿ����۸���
@note �����оƬ֧���������ۣ���ϵͳֻ����һ������ô�˽ӿڷ���1
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻ش������ֵ����pnSMCCount������δ����
*/
CSUDI_Error_Code CSUDISMCGetCount(int * pnSMCCount);

/**
@brief �����ܿ�

@param[in] nCardIndex ���ܿ������ţ���0��ʼ   
@param[in] fnSMCCallback �ص����������CSUDISMCCallback����
@return ����ɹ�����CSUDI_SUCCESS�����ʧ�ܷ��ش������ֵ
*/
CSUDI_Error_Code CSUDISMCOpen(int nCardIndex, CSUDISMCCallback fnSMCCallback);

/**
@brief �ر����ܿ�

@param[in] nCardIndex ���ܿ������ţ���0��ʼ��   
@return  ����ɹ�����CSUDI_SUCCESS�����ʧ�ܷ��ش������ֵ
*/
CSUDI_Error_Code CSUDISMCClose(int nCardIndex);

/**
@brief �������ܿ�

@param[in] nCardIndex ���ܿ������ţ���0��ʼ�� 
@param[in] bColdRst ���ȸ�λ��־��TRUEʱ�临λ��FALSEʱ�ȸ�λ
@param[out] pucAtr �������ܿ���Ӧ������
@param[out] pnAtrLength pATr�ĳ���
@return ����ɹ�����CSUDI_SUCCESS�����ʧ�ܷ��ش������ֵ
*/
CSUDI_Error_Code CSUDISMCReset(int nCardIndex, unsigned char * pucAtr, int * pnAtrLength, CSUDI_BOOL bColdRst);


/**
@brief �������ܿ�������Ϣ

@param[in] nCardIndex ���ܿ�������   
@param[in] psSMCParams  ���ܿ�������Ϣ
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ
@note ������ܿ������ڣ��򷵻�CSUDISMC_ERROR_OUT
*/
CSUDI_Error_Code CSUDISMCSetParams(int nCardIndex,const CSUDISMCParam_S * psSMCParams);


/**
@brief ��ȡ���ܿ�������Ϣ

@param[in] nCardIndex ���ܿ�������   
@param[out] psSMCParams  ���ܿ�������Ϣ
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ����psSMCParams������δ����
@note ������ܿ������ڣ��򷵻�CSUDISMC_ERROR_OUT
*/
CSUDI_Error_Code CSUDISMCGetParams(int nCardIndex,CSUDISMCParam_S * psSMCParams);

/**
@brief �豸�����ܿ�֮������ݴ���

@param[in] nCardIndex ���ܿ�������   
@param[in] pucWriteData д�����ܿ�������
@param[in] nNumberToWrite д�����ݳ���
@param[out] pcResponseData ��ȡ���ݵĴ洢λ��
@param[out] pnNumberRead ��ȡ���ݵĳ���
@param[out] pucStatusWord �����ṩ��״̬��
@return  ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ
*/
CSUDI_Error_Code CSUDISMCDataExchange(
									int             nCardIndex, 
									unsigned char  *pucWriteData,
	                                int             nNumberToWrite,
	                                unsigned char  *pcResponseData,
	                                int            *pnNumberRead,
	                                unsigned char  *pucStatusWord);

/**
@brief  �����ܿ�д����

@param[in] nCardIndex ���ܿ�������   
@param[in] pucWriteBuf ׼��д���ݵĴ洢��
@param[in] nNumberToWrite ׼��д���ݵĳ���
@param[out] pnNumberWrite ��д�����ݳ���
@param[in]  nTimeout ��ʱʱ��
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ
*/
CSUDI_Error_Code CSUDISMCSend(
					 int             nCardIndex,
				     unsigned char  *pucWriteBuf,
				     int             nNumberToWrite,
				     int            *pnNumberWrite,
				     int             nTimeout);

/**
@brief �����ܿ�������

@param[in] nCardIndex ���ܿ�������   
@param[in] pucReadBuf ��ȡ���ݴ洢��
@param[in] nNumberToRead ��ȡ�ռ��ڴ泤��
@param[out] pnNumberRead �Ѷ�ȡ�����ݳ���
@param[in] nTimeout ��ʱʱ��
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ
*/
CSUDI_Error_Code CSUDISMCReceive(
						int             nCardIndex,
				        unsigned char  *pucReadBuf,
				        int             nNumberToRead,
				        int            *pnNumberRead,
				        int             nTimeout);


/**
@brief �������ܿ�Э��ģʽ

@param[in] nCardIndex ���ܿ�������   
@param[in] pucWriteData һ��FI,DI��������
@param[in] nNumberToWriteLen �������ݵ��ֽڸ���
@note
- case  ((pucWriteData[0] & 0x10) != 0)    PPS1 valid
- case  ((pucWriteData[0] & 0x20) != 0)    PPS2 valid
- case  ((pucWriteData[0] & 0x40) != 0)    PPS3 valid
@param[out] pucResponseData ������Э��ģʽ��������Ӧ����
@param[out] pnResponseDataLen ��Ӧ���ݵ��ֽڸ���
@return ����ɹ�����CSUDI_SUCCESS�����ʧ�ܷ��ش������ֵ
@note �������Ƿ�Э��ģʽ���򷵻�CSUDISMC_ERROR_INVALID_PROTOCOL
*/
CSUDI_Error_Code CSUDISMCSetPPS(
						 int             nCardIndex,
	                    unsigned char  *pucWriteData,
                           int             nNumberToWriteLen,
	                    unsigned char  *pucResponseData,
                           int            *pnResponseDataLen);


/**
@brief  ������ܿ�

@param[in] nCardIndex ���ܿ�������   
@return ���ص�ǰ��״̬
@note ������ܿ������ڣ��򷵻�CSUDISMC_ERROR_OUT
*/
CSUDI_Error_Code CSUDISMCDetect(int nCardIndex);

/**
@brief  �������ܿ�

@param[in] nCardIndex ���ܿ�������   
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ
@note ������ܿ������ڣ��򷵻�CSUDISMC_ERROR_OUT
*/
CSUDI_Error_Code CSUDISMCActive(int nCardIndex);

/**
@brief  ȥ����

@param[in] nCardIndex ���ܿ�������   

@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ
@note ������ܿ������ڣ��򷵻�CSUDISMC_ERROR_OUT
*/
CSUDI_Error_Code CSUDISMCDeactive(int nCardIndex);


#ifdef __cplusplus
}
#endif
/**@}*/
#endif

