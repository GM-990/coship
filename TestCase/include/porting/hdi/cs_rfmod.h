/**@defgroup CSRFMOD [CSRFMOD]ͬ����Ŀר��RFMOD�ӿ�
@brief ��ģ����Ҫ������ͬ����Ŀר��ƽ̨�޹�RFMOD�����ӿ�
@brief  RFMOD����Ƶ����ģ�顣����udi��MMCP�м�㲻���õ����ģ�飬�����ھ���Ļ���MMCP����Ŀ�������ļ���Ӧ�û��õ���
 
@version 0.3 2008/11/5  ��һ��ϸ���汾
@version 0.2 2008/10/17 �����ĵ�һ���汾
@version 0.1 2008/10/16 ��ʾ�汾
@{
*/

#ifndef _CS_RFMOD_H_
#define _CS_RFMOD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include  "CSHDI_typedef.h"

/**@brief ������ʽ*/
typedef enum
{
	NTSC_M,							/**< ��Ҫ���˵�� */
	PAL_BG,							/**< ��Ҫ���˵�� */
	PAL_I,							/**< ��Ҫ���˵�� */
	PAL_DK							/**< ��Ҫ���˵�� */
}AUDIOMODE ;

/**@brief ����������*/
typedef enum 
{
	RFMOD_2170,					/**< ��Ҫ���˵�� */
	RFMOD_74055,				/**< ��Ҫ���˵�� */
	RFMOD_2270,					/**< ��Ҫ���˵�� */
	RFMOD_MAXTYPE				/**< ��Ҫ���˵�� */
}RFMODEType_t;

/**@brief ��������׼*/
typedef enum
{
	RF_STANDARD_EUROPE,      /*ŷ�ޱ�׼*/
	RF_STANDARD_AUSTRALIA    /*���ޱ�׼*/
}RFSTANDARD_t;

/**
@brief ���ñ�׼

*���õ�����ʹ�õı�׼
@param[in] enmStandardType ������ʹ�õı�׼����ѡֵ
@return ����ɹ�����CSHDI_SUCCESS;���ʧ�ܷ���CSHDI_FAILURE
*/
CSHDI_Error_t CSRFVSetStandard(RFSTANDARD_t  enmStandardType);

/**
@brief ��Ƶ��������ʼ��

*��Ƶ��������ʼ��(������ʽĬ��ΪDK�ƣ���ʼƵ��ΪCH25)
@param[in] nIndex ������I2C����������ѡֵ
@param[in] bRFModType ����������
@param[in] bControlPinIndex ���ƽ�
@return ����ɹ�����CSHDI_SUCCESS;���ʧ�ܷ���CSHDI_FAILURE
*/
CSHDI_Error_t CSRFVInit( CSHDII2CIndex nIndex, RFMODEType_t bRFModType, BYTE bControlPinIndex);

/**
@brief ���õ�гƵ��

@param[in] nCHNumber Ƶ����� (21-69)
@return ����ɹ�����CSHDI_SUCCESS;���ʧ�ܷ���CSHDI_FAILURE
*/
CSHDI_Error_t CSRFVSetChannel ( int nCHNumber ) ;

/**
@brief ���õ�г����Ƶ�ز�Ƶ��

@param[in] dwFrequency ��Ƶ�ز�Ƶ��(��KHzΪ��λ)
@return ����ɹ�����CSHDI_SUCCESS;���ʧ�ܷ���CSHDI_FAILURE
*/
CSHDI_Error_t CSRFVSetFrequency(DWORD dwFrequency) ;

/**
@brief ������Ƶ������������ʽ

@param[in] bAudioMode ������ʽ(NTSC_M, PAL_BG, PAL_I, PAL_DK)	
@return ����ɹ�����CSHDI_SUCCESS;���ʧ�ܷ���CSHDI_FAILURE
*/
CSHDI_Error_t CSRFVSetAudioMode(AUDIOMODE bAudioMode) ;

/**
@brief ������Ƶ������������ʽ

@param[in] bPowerState  ״̬(����: ���������� 0: STANDBYģʽ)
@return ����ɹ�����CSHDI_SUCCESS;���ʧ�ܷ���CSHDI_FAILURE
*/
CSHDI_Error_t CSRFSetPowerState(BYTE bPowerState);

/**
@brief ��ȡRFMOD�����İ汾��

@param[in] nSize Ҫ��ȡ�汾�ŵĳ���
@param[out] pchVer Ҫ��ȡ�İ汾�Ŵ��λ��
@return ���pchVerΪ�ջ��ߵ���ʧ���򷵻�0�����pchVerΪ�ǿշ��ذ汾����Ϣ����
*/
int CSRFMODGetVersion( PCHAR pchVer, int nSize );

#if 0 /*�����һ��ƽ̨���ڶ��rf�������Ľӿ�*/
typedef struct _CSRFModInitParam_S
{
	DWORD	dwReserved0;
	CSHDII2CIndex nIndex;					/*i2c��������*/
	RFMODEType_t eRFModType;			/*��г������*/
	BYTE ucControlPinIndex;				/*��������*/
	DWORD	dwReserved1;
	DWORD	dwReserved2;
} CSRFModInitParam_S;

/***********************************************************
*  Function:	��Ƶ��������ʼ��(������ʽĬ��ΪDK�ƣ���ʼƵ��ΪCH25)
*  Input:		pInitParam����ʼ������
*  Output:	��Ҫ���ص�г���ľ��
*  Return:	CSHDI_Error_t
************************************************************/
CSHDI_Error_t CSRFModInit( CSRFModInitParam_S* pInitParam,HANDLE* phRFModHndl);
/***********************************************************
*  Function:	���õ�г����Ƶ�ز�Ƶ��
*  Input:		hRFModHndl:��г�����
*			dwFrequency:��Ƶ�ز�Ƶ��(��KHzΪ��λ)
*  Output:	��			
*  Return:	CSHDI_Error_t
************************************************************/
CSHDI_Error_t CSRFModSetFrequency(HANDLE hRFModHndl,DWORD dwFrequency) ;

/***********************************************************
*  Function:	���ð�����ʽ
*  Input:		hRFModHndl:��г�����
*			bAudioMode:������ʽ(NTSC_M, PAL_BG, PAL_I, PAL_DK)	
*  Output:	��			
*  Return:	CSHDI_Error_t
************************************************************/
CSHDI_Error_t CSRFModSetAudioMode(HANDLE hRFModHndl,AUDIOMODE bAudioMode) ;

/***********************************************************
*  Function:	���õ�Դ״̬
*  Input:		hRFModHndl:��г�����
*			bPowerState:��Դ״̬
			-��0 ��������
			-	0  ����
*  Output:	��			
*  Return:	CSHDI_Error_t
************************************************************/
CSHDI_Error_t CSRFModSetPowerState(HANDLE hRFModHndl,BYTE bPowerState);

#endif
#ifdef __cplusplus
}
#endif
/** @} */

#endif
